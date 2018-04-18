TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -L/usr/lib -lSDL2
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
    ../../src/triangle.cpp

HEADERS += \
    ../../src/engine.h \
    ../../src/material.h \
    ../../src/mesh.h \
    ../../src/renderer.h \
    ../../src/scene.h \
    ../../src/shader.h \
    ../../src/util.h
