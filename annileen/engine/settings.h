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

        friend class Engine;
    public:
        struct Shadows
        {
            bool enabled;
            uint16_t shadowMapSize;
        };

        Shadows shadows;
    };
}
