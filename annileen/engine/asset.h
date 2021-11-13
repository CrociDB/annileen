#pragma once

#include <iostream>
#include <vector>

#include <bgfx/bgfx.h>

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

	struct ShaderAvailableUniform
	{
		std::string m_UniformName;
		bgfx::UniformType::Enum m_Type;
		uint8_t m_Position;
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

	struct MeshDescriptor
	{
		enum class Normals
		{
			Auto,
			Generate,
			GenerateSmooth
		};

		Normals m_Normals;
	};

	struct ShaderDcescritor
	{
		std::vector<ShaderAvailableUniform> m_AvailableUniforms;
	};

	// TOML asset types
	struct CubemapDescriptor
	{
		std::string m_StripFile;
	};
}
