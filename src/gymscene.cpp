#include "gymscene.h"

#include "modelloader.h"

void GymScene::start()
{
    Shader* shader = new Shader();
    shader->load("../../shaders/standard.vert", "../../shaders/standard.frag");

    Material* meshMaterial = new Material();
    meshMaterial->init(shader);
    meshMaterial->addTexture("mainTex", Texture("../../assets/textures/statue.png"));
    meshMaterial->loadTextures();

    ModelLoader loader;
    for (auto& m : loader.loadMesh("../../assets/models/dragon.obj"))
    {
        if (m != nullptr)
        {
            m->transform.position = glm::vec3(0.0, 0.0, 0.0);
            m->transform.scale = glm::vec3(0.8, 0.8, 0.8);
            m->setMaterial(meshMaterial);
            this->addMesh(m);
        }
    }

    for (auto& m : loader.loadMesh("../../assets/models/bunny.obj"))
    {
        if (m != nullptr)
        {
            m->transform.position = glm::vec3(1.2, -0.2, 0.0);
            m->transform.scale = glm::vec3(0.5, 0.5, 0.5); 
            m->setMaterial(meshMaterial);
            this->addMesh(m);
        }
    }

    for (auto& m : loader.loadMesh("../../assets/models/head.obj"))
    {
        if (m != nullptr)
        {
            m->transform.position = glm::vec3(0.3, 0.0, 1.4);
            m->transform.scale = glm::vec3(1.8, 1.8, 1.8);
            m->setMaterial(meshMaterial);
            this->addMesh(m);
        }
    }

    getCamera()->transform.position = glm::vec3(-3.0f, 0.0f, -1.0f);

    Light* light = new Light();
    light->color = glm::vec3(0.5f, 0.6f, 1.0f);
    light->type = LightDirectional;
    light->intensity = 0.6f;
    light->transform.rotate(glm::vec3(-35.0f, 75.0f, 0.0f));

    this->addLight(light);
}

void GymScene::update()
{
}

GymScene::GymScene()
{
}

GymScene::~GymScene()
{
}
