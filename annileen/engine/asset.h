#pragma once

#include <iostream>

namespace annileen
{
	enum class AssetType
	{
		Undefined,
		Shader,
		Texture,
		Model
	};

	struct AssetObject
	{
		AssetObject() { }
		virtual ~AssetObject() { }
	};

	struct AssetTableEntry
	{
		std::string m_Filepath;
		AssetType m_Type;

		bool m_Loaded;
		AssetObject* m_Asset;
	};

	// Asset descriptors
	struct TextureDescriptor
	{
		enum class Filtering
		{
			Linear,
			Point
		};

		bool m_MipMap;
		Filtering m_Filtering;
	};

	// TOML asset types
	struct CubemapDescriptor
	{
		std::string m_StripFile;
	};
}
