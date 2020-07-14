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
		std::ifstream file{ filename.c_str() };
		assert(file.is_open() && "Error loading asset file.");

		file.seekg(0, std::ios::end);
		size_t size = file.tellg();
		file.seekg(0, std::ios::beg);

		const bgfx::Memory* mem = bgfx::alloc(size + 1);
		file.read((char*)mem->data, size);
		file.close();

		mem->data[mem->size - 1] = '\0'; // Necessary?
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
				return dynamic_cast<Shader*>(entry->m_Asset);
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
};
