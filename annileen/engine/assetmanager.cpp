#include <bimg/decode.h>
#include <bx/file.h>

#include "engine.h"
#include "assetmanager.h"

namespace annileen
{
	AssetManager::AssetManager(const std::string& assetfile)
	{
		try
		{
			loadAssetTable(assetfile);
		}
		catch (const std::runtime_error&)
		{
			std::cout << "Oops, error loading assetfile." << std::endl;
		}
	}

	void AssetManager::loadAssetTable(const std::string& assetfile)
	{
		auto data = toml::parse(assetfile);
		auto assettable = toml::find(data, "asset").as_table();
		for (const auto& [k, v] : assettable)
		{
			AssetTableEntry aseet {
				toml::find<std::string>(v, "path"),
				getType(toml::find<std::string>(v, "type")),
				false,
				nullptr
			};

			m_Assets.insert_or_assign(k.c_str(), aseet);
		}
	}

	AssetType AssetManager::getType(const std::string& typetext)
	{
		if (typetext.compare("shader")) return AssetType::Shader;
		if (typetext.compare("texture")) return AssetType::Texture;
		if (typetext.compare("mesh")) return AssetType::Model;
		return AssetType::Undefined;
	}

	AssetTableEntry* AssetManager::getAssetEntry(const std::string& assetname)
	{
		assert(m_Assets.count(assetname) != 0 && "Trying to load an asset that does not exist.");
		return &m_Assets.at(assetname);
	}

	void AssetManager::unloadAssets()
	{
		for (const auto& [k, v] : m_Assets)
		{
			if (v.m_Loaded)
			{
				if (v.m_Type == AssetType::Shader)
				{
					delete dynamic_cast<Shader*>(v.m_Asset);
				}
			}
		}

		m_Assets.clear();
	}

	const bgfx::Memory* AssetManager::loadBinaryFile(const std::string& filename)
	{
		std::ifstream file;
		file.open(filename.c_str(), std::ios::binary);
		assert(file.is_open() && "Error loading asset file.");

		file.seekg(0, std::ios::end);
		size_t size = file.tellg();
		file.seekg(0, std::ios::beg);

		const bgfx::Memory* mem = bgfx::alloc(size + 1);
		file.read((char*)mem->data, size);
		file.close();

		mem->data[mem->size - 1] = '\0';
		return mem;
	}

	AssetManager::~AssetManager()
	{
		unloadAssets();
	}

	// Load functions

	Shader* AssetManager::loadShader(const std::string& vertex, const std::string& fragment)
	{
		std::string vertexfragment = vertex + fragment;
		if (m_Assets.count(vertexfragment) != 0)
		{
			auto entry = getAssetEntry(vertexfragment);
			if (entry->m_Loaded)
			{
				return static_cast<Shader*>(entry->m_Asset);
			}
		}

		// TODO: store both handles somewhere, otherwise it will compile both shaders everytime
		// there's a new combination of vertex+shader.

		auto vertexShaderData = loadBinaryFile(getAssetEntry(vertex)->m_Filepath);
		bgfx::ShaderHandle vertexHandle = bgfx::createShader(vertexShaderData);

		auto fragmentShaderData = loadBinaryFile(getAssetEntry(fragment)->m_Filepath);
		bgfx::ShaderHandle fragmentHandle = bgfx::createShader(fragmentShaderData);

		bgfx::ProgramHandle programHandle = bgfx::createProgram(vertexHandle, fragmentHandle, true);

		Shader* shader = new Shader();
		shader->init(programHandle);

		m_Assets[vertexfragment] = AssetTableEntry {
			"",
			AssetType::Shader,
			true,
			dynamic_cast<AssetObject*>(shader)
		};
	
		return shader;
	}

	Texture* AssetManager::loadTexture(const std::string& tex)
	{
		auto entry = getAssetEntry(tex);
		if (entry->m_Loaded)
		{
			return dynamic_cast<Texture*>(entry->m_Asset);
		}

		auto textureData = loadBinaryFile(entry->m_Filepath);
		//bimg::ImageContainer imageContainer;
		bx::Error err;
		//bool success = bimg::imageParse(imageContainer, textureData->data, textureData->size, &err);
		auto aimageContainer = bimg::imageParse(Engine::getAllocator(), textureData->data, textureData->size);
		auto imageContainer = (*aimageContainer);
		//assert(success && "Error parsing image");

		const bgfx::Memory* mem = bgfx::makeRef(imageContainer.m_data, imageContainer.m_size);

		auto format = bgfx::TextureFormat::Enum(imageContainer.m_format);
		uint64_t flags = BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE;
		auto handle = bgfx::createTexture2D(
			uint16_t(imageContainer.m_width),
			uint16_t(imageContainer.m_height),
			1 < imageContainer.m_numMips,
			imageContainer.m_numLayers,
			format,
			flags,
			mem);

		assert(bgfx::isValid(handle) && "Couldn't create texture");

		bgfx::setName(handle, tex.c_str());

		bgfx::TextureInfo info;

		bgfx::calcTextureSize(
			info,
			uint16_t(imageContainer.m_width),
			uint16_t(imageContainer.m_height),
			uint16_t(imageContainer.m_depth),
			imageContainer.m_cubeMap,
			1 < imageContainer.m_numMips,
			imageContainer.m_numLayers,
			bgfx::TextureFormat::Enum(imageContainer.m_format));

		Texture* texture = new Texture(handle, info, imageContainer.m_orientation);
		entry->m_Asset = static_cast<AssetObject*>(texture);

		delete textureData;
		return texture;
	}
};
