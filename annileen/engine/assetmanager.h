#pragma once

#include <iostream>
#include <map>
#include <vector>

#include <toml.hpp>

enum class AssetType
{
	Undefined,
	Shader,
	Texture,
	Model
};

struct AssetTableEntry
{
	std::string m_Filepath;
	AssetType m_Type;
	bool m_Loaded;

	void* m_Data;
	int m_DataSize;
};

class AssetManager
{
private:
	std::map<std::string, AssetTableEntry> m_Assets;

	void loadAssetTable(std::string assetfile);
	AssetType getType(std::string typetext);

	void unloadAssets();

public:
	AssetManager(std::string assetfile);
	~AssetManager();
};