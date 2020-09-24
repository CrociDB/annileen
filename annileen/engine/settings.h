#pragma once

#include <iostream>

namespace annileen
{
    class Settings final
    {
    private: 
        Settings();
        ~Settings();

        static const char* settingsFileName;

        void loadSettings();
        void saveSettings();

        std::string m_FontDefault = "droidsans.ttf";

        friend class Engine;
    public:
        struct Shadows
        {
            bool enabled;
            uint16_t shadowMapSize;
        };

        Shadows shadows;

        std::string getFontDefault() { return m_FontDefault; }
    };
}
