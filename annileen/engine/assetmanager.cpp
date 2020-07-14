#include "assetmanager.h"

using namespace annileen;

AssetManager::AssetManager(std::string assetfile)
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

void AssetManager::loadAssetTable(std::string assetfile)
{
	auto data = toml::parse(assetfile);
	auto assettable = toml::find(data, "asset").as_table();
	for (const auto& [k, v] : assettable)
	{
		AssetTableEntry aseet {
			toml::find<std::string>(v, "path"),
			getType(toml::find<std::string>(v, "type")),
			false,
			nullptr,
			0
		};

		m_Assets.insert_or_assign(k.c_str(), aseet);
	}
}

AssetType AssetManager::getType(std::string typetext)
{
	if (typetext.compare("shader")) return AssetType::Shader;
	if (typetext.compare("texture")) return AssetType::Texture;
	if (typetext.compare("mesh")) return AssetType::Model;
	return AssetType::Undefined;
}

void AssetManager::unloadAssets()
{
	for (const auto& [k, v] : m_Assets)
	{
		if (v.m_Loaded)
		{
			// Unload asset
		}
	}

	m_Assets.clear();
}

AssetManager::~AssetManager()
{
	unloadAssets();
}
