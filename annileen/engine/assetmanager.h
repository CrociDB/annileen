#pragma once

#include <iostream>
#include <map>
#include <vector>

#include <bgfx/bgfx.h>
#include <toml.hpp>

#include "asset.h"
#include "shader.h"

class Shader;

namespace annileen
{
	class AssetManager
	{
	private:
		std::map<std::string, AssetTableEntry> m_Assets;

		void loadAssetTable(const std::string& assetfile);
		AssetType getType(const std::string& typetext);
		AssetTableEntry* getAssetEntry(const std::string& assetname);

		void unloadAssets();

		const bgfx::Memory* loadBinaryFile(const std::string& filename);

	public:
		AssetManager(const std::string& assetfile);
		~AssetManager();

		// Load functions
		Shader* loadShader(const std::string& vertex, const std::string& fragment);
	};
}
