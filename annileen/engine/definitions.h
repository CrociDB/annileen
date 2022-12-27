#pragma once

#include <memory>

#define ANNILEEN_APP_MAIN(__ApplicationClassName, __ApplicationName) \
    int main(int argc, char* argv[]) { \
        std::unique_ptr<__ApplicationClassName> __app__ = std::make_unique<__ApplicationClassName>(); \
        return __app__->run(__ApplicationName); \
    }

#ifdef _ANNILEEN_COMPILER_EDITOR
#ifdef ANNILEEN_APPLICATION
    import applicationeditor;
#endif
#define ANNILEEN_APP_CLASS_DECLARATION(__ApplicationClassName) class __ApplicationClassName : public annileen::ApplicationEditor
#else
#ifdef ANNILEEN_APPLICATION
    import application;
#endif
    #define ANNILEEN_APP_CLASS_DECLARATION(__ApplicationClassName) class __ApplicationClassName : public annileen::Application
#endif // _ANNILEEN_COMPILER_EDITOR
