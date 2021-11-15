#pragma once

#define ANNILEEN_APP_MAIN(__ApplicationClassName, __ApplicationName) \
    int main(int argc, char* argv[]) { \
        std::unique_ptr<__ApplicationClassName> __app__ = std::make_unique<__ApplicationClassName>(); \
        return __app__->run(__ApplicationName); \
    }

#ifdef _ANNILEEN_COMPILER_EDITOR
#ifdef ANNILEEN_APPLICATION
    #include <editor/applicationeditor.h>
#endif
#define ANNILEEN_APP_CLASS_DECLARATION(__ApplicationClassName) class __ApplicationClassName : public annileen::ApplicationEditor
#else
#ifdef ANNILEEN_APPLICATION
    #include <engine/application.h>
#endif
    #define ANNILEEN_APP_CLASS_DECLARATION(__ApplicationClassName) class __ApplicationClassName : public annileen::Application
#endif // _ANNILEEN_COMPILER_EDITOR

namespace annileen
{
    // Template for enums that allow for bitwise operations, need to find a better 
    template<typename Enum>
    struct EnableBitMaskOperators
    {
        static const bool enable = false;
    };

    template<typename EnumClass>
    typename std::enable_if<EnableBitMaskOperators<EnumClass>::enable, EnumClass>::type
        operator |(EnumClass lhs, EnumClass rhs)
    {
        using underlying = typename std::underlying_type<EnumClass>::type;
        return static_cast<EnumClass> (
            static_cast<underlying>(lhs) |
            static_cast<underlying>(rhs)
            );
    }

    template<typename EnumClass>
    typename std::enable_if<EnableBitMaskOperators<EnumClass>::enable, EnumClass>::type
        operator &(EnumClass lhs, EnumClass rhs)
    {
        using underlying = typename std::underlying_type<EnumClass>::type;
        return static_cast<EnumClass> (
            static_cast<underlying>(lhs) &
            static_cast<underlying>(rhs)
            );
    }
}

#define ANNILEEN_ENABLE_BITMASK_OPERATORS(x) \
    template<>                       \
    struct EnableBitMaskOperators<x> \
    {                                \
        static const bool enable = true; \
    }   ;
