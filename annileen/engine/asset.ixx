module;

#include <vector>
#include <string>
#include <memory>

export module anni.asset;

export namespace annileen
{
	enum class AssetType
	{
		Undefined,
		Shader,
		Texture,
		Model,
		Cubemap,
		Font
	};

	struct AssetObject
	{
		AssetObject() = default;
		virtual ~AssetObject() = default;
	};

	struct AssetTableEntry
	{
		std::string m_Filepath{""};
		AssetType m_Type{ AssetType::Undefined };

		bool m_Loaded{ false };
		std::shared_ptr<AssetObject> m_Asset{ nullptr };
	};

	enum class ShaderUniformType
	{
		Texture,
		Cubemap,
		Mat3,
		Mat4,
		Vec4
	};

	struct ShaderAvailableUniform
	{
		std::string m_UniformName;
		ShaderUniformType m_Type;
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

	struct ShaderDescriptor
	{
		std::vector<ShaderAvailableUniform> m_AvailableUniforms;
	};

	// TOML asset types
	struct CubemapDescriptor
	{
		std::string m_StripFile;
	};
}
