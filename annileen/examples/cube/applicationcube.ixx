module;

#include <tuple>
#include <unordered_map>
#include <glm.hpp>
#include <bgfx/bgfx.h>
#include <engine/forward_decl.h>

#define ANNILEEN_APPLICATION
#include <engine/definitions.h>

export module applicationcube;

import scenemanager;
import scene;
import scenenode;
import shaderpass;
import serviceprovider;
import material;
import mesh;
import shader;
import camera;
import model;
import light;

using namespace annileen;

export ANNILEEN_APP_CLASS_DECLARATION(ApplicationCube)
{
private:
    SceneNodePtr m_ModelNode = nullptr;

    Scene* init();

    void update(float deltaTime);

    void finish() override {}

public:
    ApplicationCube() {}
    ~ApplicationCube() {}
};

Scene* ApplicationCube::init()
{
    Scene* scene = SceneManager::getInstance()->createScene<Scene>();
    //getEngine()->setScene(scene);

    annileen::Shader* shader = nullptr;
    if (ServiceProvider::getSettings()->getData()->shadows.enabled)
    {
        shader = ServiceProvider::getAssetManager()->getShader("lit_shadow");
    }
    else
    {
        shader = ServiceProvider::getAssetManager()->getShader("lit_noshadow");
    }

    std::shared_ptr<ShaderPass> shaderPass = std::make_shared<ShaderPass>();
    shaderPass->init(shader);
    shaderPass->setState(BGFX_STATE_WRITE_RGB
        | BGFX_STATE_WRITE_A
        | BGFX_STATE_WRITE_Z
        | BGFX_STATE_DEPTH_TEST_LESS
        | BGFX_STATE_CULL_CW
        | BGFX_STATE_MSAA
        | UINT64_C(0));

    std::shared_ptr<Material> material = std::make_shared<Material>();
    material->addShaderPass(shaderPass);
    material->setName("ModelMaterial");


    // Statue Decoration
    auto normalmap = ServiceProvider::getAssetManager()->getTexture("statue_decoration_normal.jpg");
    auto texture = ServiceProvider::getAssetManager()->getTexture("statue_decoration.jpg");
    material->setTexture("s_mainTex", texture);
    material->setTexture("s_mainNormal", normalmap);

    m_ModelNode = scene->createNode("Statue");
    ModelPtr model = SceneManager::getInstance()->addModule<Model>(scene, m_ModelNode);
    model->init(ServiceProvider::getAssetManager()->getMesh("statue_decoration.obj"), material);
    m_ModelNode->getTransform().translate(glm::vec3(-1.0, -1.0, -1.0));
    m_ModelNode->getTransform().scale(glm::vec3(.1, .1, .1));

    // Angel Statue
    std::shared_ptr<Material> material1 = std::make_shared<Material>();
    material1->addShaderPass(shaderPass);
    material1->setName("ModelMaterial1");

    auto normalmap1 = ServiceProvider::getAssetManager()->getTexture("statue_normal.jpg");
    material1->setTexture("s_mainNormal", normalmap1);

    auto node1 = scene->createNode("AngelStatue");
    ModelPtr model1 = SceneManager::getInstance()->addModule<Model>(scene, node1);
    model1->init(ServiceProvider::getAssetManager()->getMesh("statue.obj"), material1);
    node1->getTransform().translate(glm::vec3(-15.0, -1.0, -1.0));
    node1->getTransform().scale(glm::vec3(.07, .07, .07));

    SceneNodePtr cameraNode = scene->createNode("Camera");
    Camera* camera = SceneManager::getInstance()->addModule<Camera>(scene, cameraNode);
    camera->fieldOfView = 60.0f;
    camera->nearClip = 0.1f;
    camera->farClip = 300.0f;
    camera->getTransform().translate(glm::vec3(-5.0f, 0.0f, -5.0f));
    camera->setForward(m_ModelNode->getTransform().position() - camera->getTransform().position());
    SceneNodePtr lightNode = scene->createNode("Light");
    Light* light = SceneManager::getInstance()->addModule<Light>(scene, lightNode);

    light->color = glm::vec3(1.0f, 1.0f, .8f);
    light->type = LightType::Directional;
    light->intensity = 0.8f;
    light->getTransform().rotate(glm::vec3(-40.0f, 0.0f, -40.0f));

    return scene;
}

void ApplicationCube::update(float deltaTime)
{
    m_ModelNode->getTransform().rotate(glm::vec3(0.0, 15.0 * deltaTime, 0.0));
}
