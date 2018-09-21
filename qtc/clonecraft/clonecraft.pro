TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++17

LIBS += -L/usr/lib -lSDL2
LIBS += -L/usr/lib -lSDL2_image
LIBS += -L/usr/lib/x86_64-linux-gnu -lGLEW -lGL

INCLUDEPATH += /usr/include/SDL2
INCLUDEPATH += glm/

SOURCES += \
    ../../src/engine.cpp \
    ../../src/main.cpp \
    ../../src/material.cpp \
    ../../src/mesh.cpp \
    ../../src/renderer.cpp \
    ../../src/scene.cpp \
    ../../src/shader.cpp \
    ../../src/camera.cpp \
    ../../src/input.cpp \
    ../../src/texture.cpp \
    ../../src/transform.cpp

HEADERS += \
    ../../src/engine.h \
    ../../src/material.h \
    ../../src/mesh.h \
    ../../src/renderer.h \
    ../../src/scene.h \
    ../../src/shader.h \
    ../../src/util.h \
    ../../src/camera.h \
    ../../src/input.h \
    ../../src/texture.h \
    ../../src/transform.h

