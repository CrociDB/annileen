#include <bx/file.h>

#include "engine.h"
#include "assetmanager.h"
#include "modelloader.h"

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

	std::tuple<bgfx::TextureHandle, bgfx::TextureInfo, bimg::ImageContainer*> AssetManager::loadTextureData(const std::string& file, const TextureDescriptor& descriptor)
	{
		auto textureData = loadBinaryFile(file);
		bx::Error err;
		auto imageContainer = bimg::imageParse(Engine::getAllocator(), textureData->data, textureData->size);

		const bgfx::Memory* mem = bgfx::makeRef(imageContainer->m_data, imageContainer->m_size);

		auto format = bgfx::TextureFormat::Enum(imageContainer->m_format);
		uint64_t flags = BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE;

		if (descriptor.m_Filtering == TextureDescriptor::Filtering::Point)
			flags |= BGFX_SAMPLER_POINT;


		bgfx::TextureHandle handle;
		
		if (imageContainer->m_cubeMap)
		{
			handle = bgfx::createTextureCube(
				uint16_t(imageContainer->m_width),
				1 < imageContainer->m_numMips,
				imageContainer->m_numLayers,
				format,
				flags,
				mem);
		}
		else
		{
			handle = bgfx::createTexture2D(
				uint16_t(imageContainer->m_width),
				uint16_t(imageContainer->m_height),
				1 < imageContainer->m_numMips,
				imageContainer->m_numLayers,
				format,
				flags,
				mem);
		}

		assert(bgfx::isValid(handle) && "Couldn't create texture");

		bgfx::setName(handle, file.c_str());

		bgfx::TextureInfo info;

		bgfx::calcTextureSize(
			info,
			uint16_t(imageContainer->m_width),
			uint16_t(imageContainer->m_height),
			uint16_t(imageContainer->m_depth),
			imageContainer->m_cubeMap,
			1 < imageContainer->m_numMips,
			imageContainer->m_numLayers,
			bgfx::TextureFormat::Enum(imageContainer->m_format));

		delete textureData;
		return std::make_tuple(handle, info, imageContainer);
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
			return static_cast<Texture*>(entry->m_Asset);
		}

		auto descriptor = loadTextureDescriptor(entry);
		
		bgfx::TextureHandle handle;
		bgfx::TextureInfo info;
		bimg::ImageContainer* imageContainer = nullptr;
		std::tie(handle, info, imageContainer) = loadTextureData(entry->m_Filepath, descriptor);

		Texture* texture = new Texture(handle, info, imageContainer->m_orientation);
		entry->m_Asset = static_cast<AssetObject*>(texture);
		entry->m_Loaded = true;
		return texture;
	}

	Cubemap* AssetManager::loadCubemap(const std::string& name)
	{
		auto entry = getAssetEntry(name);
		if (entry->m_Loaded)
		{
			return static_cast<Cubemap*>(entry->m_Asset);
		}

		auto descriptor = loadCubemapDescriptor(entry);

		bgfx::TextureHandle handle;
		bgfx::TextureInfo info;
		bimg::ImageContainer* imageContainer = nullptr;
		std::tie(handle, info, imageContainer) = loadTextureData(descriptor.m_StripFile, {});

		Cubemap* cubemap = new Cubemap(handle, info, imageContainer->m_orientation);
		entry->m_Asset = static_cast<AssetObject*>(cubemap);
		entry->m_Loaded = true;
		return cubemap;
	}

	MeshGroup* AssetManager::loadMesh(const std::string& name)
	{
		auto entry = getAssetEntry(name);
		if (entry->m_Loaded)
		{
			return static_cast<MeshGroup*>(entry->m_Asset);
		}

		const auto descriptor = loadMeshDescriptor(entry);

		ModelLoader loader;
		auto meshGroup = loader.loadMesh(entry->m_Filepath, descriptor);
		entry->m_Asset = static_cast<AssetObject*>(meshGroup);
		entry->m_Loaded = true;
		return meshGroup;
	}

	Font* AssetManager::loadFont(const std::string& name)
	{
		auto entry = getAssetEntry(name);
		if (entry->m_Loaded)
		{
			return static_cast<Font*>(entry->m_Asset);
		}

		Font* font = new Font(entry->m_Filepath);
		entry->m_Asset = static_cast<AssetObject*>(font);
		entry->m_Loaded = true;
		return font;
	}

	TextureDescriptor AssetManager::loadTextureDescriptor(AssetTableEntry* asset)
	{
		auto assetfile = asset->m_Filepath.substr(0, asset->m_Filepath.find_last_of(".")) + ".toml";
		auto data = toml::parse(assetfile);
		return {
			toml::find(data, "mipmap").as_boolean(),
			toml::find(data, "filter").as_string() == "point" ? TextureDescriptor::Filtering::Point : TextureDescriptor::Filtering::Linear
		};
	}

	MeshDescriptor AssetManager::loadMeshDescriptor(AssetTableEntry* asset)
	{
		auto assetfile = asset->m_Filepath.substr(0, asset->m_Filepath.find_last_of(".")) + ".toml";
		auto data = toml::parse(assetfile);

		auto normalsValue = toml::find(data, "normals").as_string();
		auto normals = MeshDescriptor::Normals::Auto;

		if (normalsValue == "generate") normals = MeshDescriptor::Normals::Generate;
		else if (normalsValue == "generate_smooth") normals = MeshDescriptor::Normals::GenerateSmooth;

		return {
			normals
		};
	}

	CubemapDescriptor AssetManager::loadCubemapDescriptor(AssetTableEntry* asset)
	{
		auto assetfile = asset->m_Filepath;
		auto data = toml::parse(assetfile);
		const auto cubemap = toml::find(data, "cubemap");
		return {
			cubemap.at("strip_file").as_string(),
		};
	}
};
