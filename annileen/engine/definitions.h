#pragma once

#define ANNILEEN_APP_MAIN(__ApplicationClassName) \
    int main(int argc, char* argv[]) { \
        std::unique_ptr<__ApplicationClassName> __app__ = std::make_unique<__ApplicationClassName>(); \
        return __app__->run(); \
    }

#ifdef _DEBUG
#ifdef ANNILEEN_APPLICATION
    #include <editor/applicationeditor.h>
#endif
#define ANNILEEN_APP_CLASS_DECLARATION(__ApplicationClassName) class __ApplicationClassName : public annileen::ApplicationEditor
#else
#ifdef ANNILEEN_APPLICATION
    #include <engine/application.h>
#endif
    #define ANNILEEN_APP_CLASS_DECLARATION(__ApplicationClassName) class __ApplicationClassName : public annileen::Application
#endif // DEBUG

    