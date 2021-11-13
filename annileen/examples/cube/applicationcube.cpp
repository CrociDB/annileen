#define ANNILEEN_APPLICATION
#include "definitions.h"

#include <glm.hpp>

#include <engine/scene.h>
#include <engine/shaderpass.h>
#include <engine/serviceprovider.h>
#include <engine/material.h>
#include <engine/mesh.h>

using namespace annileen;

ANNILEEN_APP_CLASS_DECLARATION(ApplicationCube)
{
private:
    SceneNodePtr m_ModelNode = nullptr;

    annileen::Scene* init()
    {
        Scene* scene = new Scene();
        //getEngine()->setScene(scene);

        annileen::Shader* shader = nullptr;
        if (ServiceProvider::getSettings()->getData()->shadows.enabled)
        {
            shader = ServiceProvider::getAssetManager()->loadShader("lit_shadow");
        }
        else
        {
            shader = ServiceProvider::getAssetManager()->loadShader("lit_noshadow");
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
        auto normalmap = ServiceProvider::getAssetManager()->loadTexture("statue_decoration_normal.jpg");
        auto texture = ServiceProvider::getAssetManager()->loadTexture("statue_decoration.jpg");
        material->setTexture("s_mainTex", texture);
        material->setTexture("s_mainNormal", normalmap);

        m_ModelNode = scene->createNode("Statue");
        ModelPtr model = m_ModelNode->addModule<Model>();
        model->init(ServiceProvider::getAssetManager()->loadMesh("statue_decoration.obj"), material);
        m_ModelNode->getTransform().translate(glm::vec3(-1.0, -1.0, -1.0));
        m_ModelNode->getTransform().scale(glm::vec3(.1, .1, .1));

        // Angel Statue
        std::shared_ptr<Material> material1 = std::make_shared<Material>();
        material1->addShaderPass(shaderPass);
        material1->setName("ModelMaterial1");

        auto normalmap1 = ServiceProvider::getAssetManager()->loadTexture("statue_normal.jpg");
        material1->setTexture("s_mainNormal", normalmap1);

        auto node1 = scene->createNode("AngelStatue");
        ModelPtr model1 = node1->addModule<Model>();
        model1->init(ServiceProvider::getAssetManager()->loadMesh("statue.obj"), material1);
        node1->getTransform().translate(glm::vec3(-15.0, -1.0, -1.0));
        node1->getTransform().scale(glm::vec3(.07, .07, .07));
         
        SceneNodePtr cameraNode = scene->createNode("Camera");
        Camera* camera = cameraNode->addModule<Camera>(); 
        camera->fieldOfView = 60.0f;
        camera->nearClip = 0.1f;
        camera->farClip = 300.0f;
        camera->getTransform().translate(glm::vec3(-5.0f, 0.0f, -5.0f));
        camera->setForward(m_ModelNode->getTransform().position() - camera->getTransform().position());
        SceneNodePtr lightNode = scene->createNode("Light");
        Light* light = lightNode->addModule<Light>();

        light->color = glm::vec3(1.0f, 1.0f, .8f);
        light->type = LightType::Directional;
        light->intensity = 0.8f;
        light->getTransform().rotate(glm::vec3(-40.0f, 0.0f, -40.0f));

        return scene;
    }

    void update(float deltaTime)
    {
        m_ModelNode->getTransform().rotate(glm::vec3(0.0, 15.0 * deltaTime, 0.0));
    }

    void finish() {}

public:
    ApplicationCube() {}
    ~ApplicationCube() {}
};

ANNILEEN_APP_MAIN(ApplicationCube, "Cube")
