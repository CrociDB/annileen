#pragma once

namespace annileen
{
    class Settings
    {
    private: 
        Settings();
        ~Settings();

        static const char* settingsFileName;

        void loadSettings();
        void saveSettings();

        friend class Engine;
    public:

        bool shadows;
    };
}
