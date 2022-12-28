module;

#include <toml.hpp>
#include <iostream>
#include <bgfx/bgfx.h>

export module settings;

export namespace annileen
{
	enum class AnnileenRenderer
	{
		OpenGL,
		DirectX11,
		DirectX12,
		Vulkan,
		Metal
	};

	struct Setting_Shadows
	{
		bool enabled;
		uint16_t shadowMapSize;
	};

	struct SettingsData
	{
		AnnileenRenderer renderer;
		Setting_Shadows shadows;
		std::string defaultFont = "droidsans.ttf";
	};

	class Settings final
	{
	private:
		Settings();
		~Settings();

		void loadSettings(const std::string& filename);
		void saveSettings();

		AnnileenRenderer parseRenderer(const std::string& rendererStr);

		friend class Engine;
	public:
		SettingsData m_Data;

		bgfx::RendererType::Enum getBGFXRendererType();
		SettingsData* getData() { return &m_Data; }
	};
}


namespace annileen
{
	Settings::Settings()
	{
	}

	Settings::~Settings()
	{
		saveSettings();
	}

	void Settings::loadSettings(const std::string& filename)
	{
		auto data = toml::parse(filename);

		m_Data.renderer = parseRenderer(toml::find<std::string>(data, "renderer"));
		m_Data.defaultFont = toml::find<std::string>(data, "font");

		auto shadows = toml::find(data, "shadows").as_table();

		m_Data.shadows.enabled = shadows.at("enabled").as_boolean();
		m_Data.shadows.shadowMapSize = static_cast<uint16_t>(shadows.at("shadowMapSize").as_integer());
	}

	void Settings::saveSettings()
	{
		// TODO: save settings to settings file.		
	}

	AnnileenRenderer Settings::parseRenderer(const std::string& rendererStr)
	{
		if (rendererStr.compare("directx11") == 0) return AnnileenRenderer::DirectX11;
		if (rendererStr.compare("directx12") == 0) return AnnileenRenderer::DirectX12;
		if (rendererStr.compare("vulkan") == 0) return AnnileenRenderer::Vulkan;
		if (rendererStr.compare("metal") == 0) return AnnileenRenderer::Metal;
		return AnnileenRenderer::OpenGL;
	}

	bgfx::RendererType::Enum Settings::getBGFXRendererType()
	{
		if (m_Data.renderer == AnnileenRenderer::DirectX11) return bgfx::RendererType::Direct3D11;
		if (m_Data.renderer == AnnileenRenderer::DirectX12) return bgfx::RendererType::Direct3D12;
		if (m_Data.renderer == AnnileenRenderer::Vulkan) return bgfx::RendererType::Vulkan;
		if (m_Data.renderer == AnnileenRenderer::Metal) return bgfx::RendererType::Metal;
		return bgfx::RendererType::OpenGL;
	}
}
