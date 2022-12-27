module;

#include <tuple>
#include <glm.hpp>
#include <bgfx/bgfx.h>
#include <engine/forward_decl.h>

#define ANNILEEN_APPLICATION
#include <engine/definitions.h>

export module applicationcube;

export ANNILEEN_APP_CLASS_DECLARATION(ApplicationCube)
{
private:
    annileen::SceneNodePtr m_ModelNode = nullptr;

    annileen::Scene* init();

    void update(float deltaTime);

    void finish() {}

public:
    ApplicationCube() {}
    ~ApplicationCube() {}
};
