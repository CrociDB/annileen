#pragma once

#include <iostream>
#include <bgfx/bgfx.h>

namespace annileen
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
