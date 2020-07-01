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
    auto statueMeshes = loader.loadMesh("../../assets/models/statue.obj");
    for (auto& m : statueMeshes)
    {
        if (m != nullptr)
        {
            m->transform.position = glm::vec3(0.0, 0.0, 0.0);
            m->transform.scale = glm::vec3(0.03, 0.03, 0.03);
            m->setMaterial(meshMaterial);
            this->addMesh(m);
        }
    }

    getCamera()->transform.position = glm::vec3(-3.0f, 0.0f, -2.0f);

    Light* light = new Light();
    light->color = glm::vec3(0.5f, 0.6f, 1.0f);
    light->type = LightDirectional;
    light->intensity = 0.6f;
    light->transform.rotate(glm::vec3(0.0f, 90.0f, 0.0f));

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
