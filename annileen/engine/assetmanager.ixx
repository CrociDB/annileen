module;

#include <bx/file.h>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <bimg/decode.h>
#include <bgfx/bgfx.h>
#include <toml.hpp>
#include "engine/core/logger.h"

export module assetmanager;

import allocators;
import asset;
import assetwatcher;
import shader;
import texture;
import cubemap;
import logger;
import mesh;
import font;
import modelloader;

export namespace annileen
{
	class AssetManager final
	{
	private:
		std::map<std::string, std::shared_ptr<AssetTableEntry>> m_Assets;
		std::unique_ptr<AssetWatcher> m_Watcher;

		void loadAssetTable(const std::string& assetfile);
		AssetType getType(const std::string& typetext);
		ShaderUniformType getUniformType(const std::string& typetext);
		std::shared_ptr<AssetTableEntry> getAssetEntry(const std::string& assetname);
		std::shared_ptr<AssetTableEntry> getAssetEntryIfExists(const std::string& assetname);

		void unloadAssets();

		const bgfx::Memory* loadBinaryFile(const std::string& filename);
		std::tuple<bgfx::TextureHandle, bgfx::TextureInfo, bimg::ImageContainer*> loadTextureData(const std::string& file, const TextureDescriptor& descriptor);

		void assetModified(const std::string& path, AssetFileStatus status);

		void loadAsset(std::shared_ptr<AssetTableEntry> asset);
		void loadAssetShader(std::shared_ptr<AssetTableEntry> asset);


		friend class Engine;

	public:
		AssetManager(const std::string& assetfile);
		~AssetManager();

		void updateAssetWatcher();

		// Asset loading functions
		std::shared_ptr<Shader> getShader(const std::string& basename);
		std::shared_ptr<Texture> getTexture(const std::string& tex);
		std::shared_ptr<Cubemap> getCubemap(const std::string& name);
		std::shared_ptr<MeshGroup> getMesh(const std::string& name);
		std::shared_ptr<Font> getFont(const std::string& name);

		// Asset descriptor loading functions
		ShaderDescriptor loadShaderDescriptor(std::shared_ptr<AssetTableEntry> asset);
		TextureDescriptor loadTextureDescriptor(std::shared_ptr<AssetTableEntry> asset);
		MeshDescriptor loadMeshDescriptor(std::shared_ptr<AssetTableEntry> asset);
		CubemapDescriptor loadCubemapDescriptor(std::shared_ptr<AssetTableEntry> asset);
	};

	inline bool __anni_check_shader_program(void* handle)
	{
		return *((uint16_t*)handle) != 65535;
	}
}

namespace annileen
{
	AssetManager::AssetManager(const std::string& assetfile)
	{
		try
		{
			loadAssetTable(assetfile);

			size_t pos = assetfile.find_last_of("\\/");
			auto path = std::string::npos == pos ? "" : assetfile.substr(0, pos);
			m_Watcher = std::make_unique<AssetWatcher>(path);
		}
		catch (const std::runtime_error&)
		{
			std::cout << "Oops, error loading assetfile." << std::endl;
		}
	}

	void AssetManager::loadAssetTable(const std::string& assetfile)
	{
		auto data = toml::parse(assetfile);
		auto& assettable = toml::find(data, "asset").as_table();
		for (const auto& [k, v] : assettable)
		{
			auto asset = std::make_shared<AssetTableEntry>(
				toml::find<std::string>(v, "path"),
				getType(toml::find<std::string>(v, "type")),
				false,
				nullptr
			);

			m_Assets.insert_or_assign(k.c_str(), asset);
		}
	}

	AssetType AssetManager::getType(const std::string& typetext)
	{
		if (typetext.compare("shader") == 0) return AssetType::Shader;
		if (typetext.compare("texture") == 0) return AssetType::Texture;
		if (typetext.compare("mesh") == 0) return AssetType::Model;
		if (typetext.compare("cubemap") == 0) return AssetType::Cubemap;
		if (typetext.compare("font") == 0) return AssetType::Font;
		return AssetType::Undefined;
	}

	ShaderUniformType AssetManager::getUniformType(const std::string& typetext)
	{
		std::string copy = typetext.c_str();
		std::transform(copy.begin(), copy.end(), copy.begin(),
			[](unsigned char c) { return std::tolower(c); });

		if (copy.compare("texture") == 0) return ShaderUniformType::Texture;
		if (copy.compare("cubemap") == 0) return ShaderUniformType::Cubemap;
		if (copy.compare("mat3") == 0) return ShaderUniformType::Mat3;
		if (copy.compare("mat4") == 0) return ShaderUniformType::Mat4;
		return ShaderUniformType::Vec4;
	}

	std::shared_ptr<AssetTableEntry> AssetManager::getAssetEntry(const std::string& assetname)
	{
		assert(m_Assets.count(assetname) != 0 && "Trying to load an asset that does not exist.");
		return m_Assets.at(assetname);
	}

	std::shared_ptr<AssetTableEntry> AssetManager::getAssetEntryIfExists(const std::string& assetname)
	{
		if (m_Assets.count(assetname) == 0) return nullptr;
		return m_Assets.at(assetname);
	}

	void AssetManager::unloadAssets()
	{
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

		const bgfx::Memory* mem = bgfx::alloc(static_cast<uint32_t>(size + 1));
		file.read((char*)mem->data, size);
		file.close();

		mem->data[mem->size - 1] = '\0';
		return mem;
	}

	std::tuple<bgfx::TextureHandle, bgfx::TextureInfo, bimg::ImageContainer*> AssetManager::loadTextureData(const std::string& file, const TextureDescriptor& descriptor)
	{
		auto textureData = loadBinaryFile(file);
		bx::Error err;
		auto imageContainer = bimg::imageParse(Allocators::getAllocator(), textureData->data, textureData->size);

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

	void AssetManager::assetModified(const std::string& path, AssetFileStatus status)
	{
		// Let's only deal with modified files for now
		if (status == AssetFileStatus::Modified)
		{
			// First we get the name of the asset
			auto asset_name = path.substr(path.find_last_of("/\\") + 1);

			auto entry = getAssetEntryIfExists(asset_name);
			if (entry == nullptr || !entry->m_Loaded) return;

			if (entry->m_Type == AssetType::Shader)
			{
				loadAsset(entry);
				ANNILEEN_LOG(LoggingLevel::Info, LoggingChannel::Asset, std::format("Assed reloaded: {}", asset_name));
			}
		}
	}

	void AssetManager::loadAsset(std::shared_ptr<AssetTableEntry> asset)
	{
		switch (asset->m_Type)
		{
			case AssetType::Shader:
				loadAssetShader(asset);
				break;
			default:
				return;
		}
	}

	void AssetManager::loadAssetShader(std::shared_ptr<AssetTableEntry> asset)
	{
		auto asset_name = asset->m_Filepath.substr(asset->m_Filepath.find_last_of("/\\") + 1);
		std::string::size_type const p(asset_name.find_last_of('.'));
		auto file_without_extension = asset_name.substr(0, p);
		auto extension = asset_name.substr(p, asset_name.length());

		// Need to get both shaders
		auto vert = asset;
		auto frag = asset;
		if (extension.compare(".vs") == 0) frag = getAssetEntry(file_without_extension.append(".fs"));
		else vert = getAssetEntry(file_without_extension.append(".vs"));

		auto descriptor = loadShaderDescriptor(asset);

		auto vertexShaderData = loadBinaryFile(vert->m_Filepath);
		bgfx::ShaderHandle vertexHandle = bgfx::createShader(vertexShaderData);
		ANNI_ASSERT(__anni_check_shader_program(&vertexHandle), "ERROR LOADING VERTEX SHADER");

		auto fragmentShaderData = loadBinaryFile(frag->m_Filepath);
		bgfx::ShaderHandle fragmentHandle = bgfx::createShader(fragmentShaderData);
		ANNI_ASSERT(__anni_check_shader_program(&fragmentHandle), "ERROR LOADING FRAGMENT SHADER");

		bgfx::ProgramHandle programHandle = bgfx::createProgram(vertexHandle, fragmentHandle, true);
		ANNI_ASSERT(__anni_check_shader_program(&programHandle), "ERROR CREATING SHADER PROGRAM");

		// Let's use only vertex shader to store the final shader
		if (!vert->m_Loaded)
		{
			auto shader = std::make_shared<Shader>();
			shader->setHandle(programHandle);
			shader->setAvailableShaders(descriptor.m_AvailableUniforms);

			vert->m_Asset = static_pointer_cast<AssetObject>(shader);
			vert->m_Loaded = true;
		}
		else
		{
			auto shader = static_pointer_cast<Shader>(vert->m_Asset);
			shader->destroy();
			shader->setHandle(programHandle);
			shader->setAvailableShaders(descriptor.m_AvailableUniforms);
		}
	}

	AssetManager::~AssetManager()
	{
		unloadAssets();

		// TODO: remove
		ANNI_LOG("AssetManager destroyed.")
	}

	void AssetManager::updateAssetWatcher()
	{
		m_Watcher->update();
		if (m_Watcher->hasModified())
		{
			for (auto& [f, s] : m_Watcher->getModified())
			{
				assetModified(f, s);
			}

			m_Watcher->resetModified();
		}
	}

	// Load functions
	std::shared_ptr<Shader> AssetManager::getShader(const std::string& basename)
	{
		std::string vertex = basename + ".vs";

		auto entry = getAssetEntry(vertex);
		if (!entry->m_Loaded)
		{
			loadAssetShader(entry);
		}
		
		return static_pointer_cast<Shader>(entry->m_Asset);
	}

	std::shared_ptr<Texture> AssetManager::getTexture(const std::string& tex)
	{
		auto entry = getAssetEntry(tex);
		if (entry->m_Loaded)
		{
			return static_pointer_cast<Texture>(entry->m_Asset);
		}

		auto descriptor = loadTextureDescriptor(entry);
		
		bgfx::TextureHandle handle;
		bgfx::TextureInfo info;
		bimg::ImageContainer* imageContainer = nullptr;
		std::tie(handle, info, imageContainer) = loadTextureData(entry->m_Filepath, descriptor);

		auto texture = std::make_shared<Texture>(handle, info, imageContainer->m_orientation);
		entry->m_Asset = static_pointer_cast<AssetObject>(texture);
		entry->m_Loaded = true;
		return texture;
	}

	std::shared_ptr<Cubemap> AssetManager::getCubemap(const std::string& name)
	{
		auto entry = getAssetEntry(name);
		if (entry->m_Loaded)
		{
			return static_pointer_cast<Cubemap>(entry->m_Asset);
		}

		auto descriptor = loadCubemapDescriptor(entry);

		bgfx::TextureHandle handle;
		bgfx::TextureInfo info;
		bimg::ImageContainer* imageContainer = nullptr;
		std::tie(handle, info, imageContainer) = loadTextureData(descriptor.m_StripFile, {});

		auto cubemap = std::make_shared<Cubemap>(handle, info, imageContainer->m_orientation);
		entry->m_Asset = static_pointer_cast<AssetObject>(cubemap);
		entry->m_Loaded = true;
		return cubemap;
	}

	std::shared_ptr<MeshGroup> AssetManager::getMesh(const std::string& name)
	{
		auto entry = getAssetEntry(name);
		if (entry->m_Loaded)
		{
			return static_pointer_cast<MeshGroup>(entry->m_Asset);
		}

		const auto descriptor = loadMeshDescriptor(entry);

		ModelLoader loader;
		auto meshGroup = loader.loadMesh(entry->m_Filepath, descriptor);
		entry->m_Asset = static_pointer_cast<AssetObject>(meshGroup);
		entry->m_Loaded = true;
		return meshGroup;
	}

	std::shared_ptr<Font> AssetManager::getFont(const std::string& name)
	{
		auto entry = getAssetEntry(name);
		if (entry->m_Loaded)
		{
			return static_pointer_cast<Font>(entry->m_Asset);
		}

		auto font = std::make_shared<Font>(entry->m_Filepath);
		entry->m_Asset = static_pointer_cast<AssetObject>(font);
		entry->m_Loaded = true;
		return font;
	}

	ShaderDescriptor AssetManager::loadShaderDescriptor(std::shared_ptr<AssetTableEntry> asset)
	{
		ShaderDescriptor descriptor;

		auto shaderdescriptor = asset->m_Filepath.substr(0, asset->m_Filepath.find_last_of(".")) + ".toml";
		auto data = toml::parse(shaderdescriptor);

		// Uniforms
		if (!data.contains("uniforms")) return descriptor;

		auto& uniforms = toml::find(data, "uniforms").as_array();
		auto size = uniforms.size();
		descriptor.m_AvailableUniforms.resize(size);

		int i = 0;
		for (const auto& v : uniforms)
		{
			ShaderAvailableUniform uniform {
				toml::find(v, "name").as_string(),
				getUniformType(toml::find(v, "type").as_string()),
				static_cast<uint8_t>(toml::find(v, "pos").as_integer())
			};

			descriptor.m_AvailableUniforms.at(i++) = uniform;
		}

		return descriptor;
	}

	TextureDescriptor AssetManager::loadTextureDescriptor(std::shared_ptr<AssetTableEntry> asset)
	{
		auto assetfile = asset->m_Filepath.substr(0, asset->m_Filepath.find_last_of(".")) + ".toml";
		auto data = toml::parse(assetfile);
		return {
			toml::find(data, "mipmap").as_boolean(),
			toml::find(data, "filter").as_string() == "point" ? TextureDescriptor::Filtering::Point : TextureDescriptor::Filtering::Linear
		};
	}

	MeshDescriptor AssetManager::loadMeshDescriptor(std::shared_ptr<AssetTableEntry> asset)
	{
		auto assetfile = asset->m_Filepath.substr(0, asset->m_Filepath.find_last_of(".")) + ".toml";
		auto data = toml::parse(assetfile);

		auto& normalsValue = toml::find(data, "normals").as_string();
		auto normals = MeshDescriptor::Normals::Auto;

		if (normalsValue == "generate") normals = MeshDescriptor::Normals::Generate;
		else if (normalsValue == "generate_smooth") normals = MeshDescriptor::Normals::GenerateSmooth;

		return {
			normals
		};
	}

	CubemapDescriptor AssetManager::loadCubemapDescriptor(std::shared_ptr<AssetTableEntry> asset)
	{
		auto& assetfile = asset->m_Filepath;
		auto data = toml::parse(assetfile);
		const auto& cubemap = toml::find(data, "cubemap");
		return {
			cubemap.at("strip_file").as_string(),
		};
	}
};
