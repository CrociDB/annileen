#pragma once

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
            size_t shadowMapSize;
        };

        Shadows shadows;
    };
}
