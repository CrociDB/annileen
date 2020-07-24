#pragma once

#define ANNILEEN_APP_MAIN(__ApplicationClassName) \
    int main(int argc, char* argv[]) { \
        std::unique_ptr<__ApplicationClassName> __app__ = std::make_unique<__ApplicationClassName>(); \
        return __app__->run(); \
    }
