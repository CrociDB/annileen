#include <iostream>
#include "engine.h"
#include "util.h"

float vdata_cube[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

float vdata_floor[] = {
    -0.5f,  0.0f, -0.5f,  0.0f, 0.5f,
     0.5f,  0.0f, -0.5f,  0.33, 0.5f,
     0.5f,  0.0f,  0.5f,  0.33, 0.0f,
     0.5f,  0.0f,  0.5f,  0.33, 0.0f,
    -0.5f,  0.0f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.0f, -0.5f,  0.0f, 0.5f
};


int main(int argc, char* argv[])
{
    Engine* engine = Engine::getInstance();
    engine->init(800, 600);

    // Create triangle
    Mesh* cube = new Mesh();
    cube->init(vdata_cube, 
                sizeof(vdata_cube) / sizeof(float), 
                VERTEX_UV);
    cube->transform.translate(glm::vec3(0.0f, 0.0f, -3.0f));

    Mesh* floor = new Mesh();
    floor->init(vdata_floor,
        sizeof(vdata_floor) / sizeof(float),
        VERTEX_UV);
    floor->transform.translate(glm::vec3(0.0f, -1.0f, -3.0f));
    floor->transform.scale *= 30.0f;

    // Mesh material
    Shader* shader = new Shader();
    shader->load("../../shaders/vertex.vert", "../../shaders/fragment.frag");

    Material* material = new Material();
    material->init(shader);
    material->addTexture("mainTex", Texture("../../assets/texture.png"));
    material->loadTextures();

    cube->setMaterial(material);
    floor->setMaterial(material);

    // Create scene
    Scene* scene = new Scene();
    scene->addMesh(cube);
    scene->addMesh(floor);

    Camera* camera = scene->getCamera();

    engine->setScene(scene);

    while (engine->isRunning())
    {
        engine->checkInputEvents();

        //mesh->transform.rotation = mesh->transform.FromEuler(glm::vec3(0.0, 0.3f * SDL_GetTicks(), 0.0));

        if (engine->getInput()->getKeyDown(SDLK_s))
        {
            camera->transform.translate(glm::vec3(0.0f, 0.0f, 0.3f));
        }
        if (engine->getInput()->getKeyDown(SDLK_w))
        {
            camera->transform.translate(glm::vec3(0.0f , 0.0f, -0.3f));
        }
        if (engine->getInput()->getKeyDown(SDLK_a))
        {
            camera->transform.translate(glm::vec3(-0.3f, 0.0f, 0.0f));
        }
        if (engine->getInput()->getKeyDown(SDLK_d))
        {
            camera->transform.translate(glm::vec3(0.3f, 0.0f, 0.0f));
        }

		if (engine->getInput()->getKeyDown(SDLK_LEFT))
		{
            camera->transform.rotate(glm::vec3(0.0f, 3.0f, 0.0f));
		}
		if (engine->getInput()->getKeyDown(SDLK_RIGHT))
		{
            camera->transform.rotate(glm::vec3(0.0f, -3.0f, 0.0f));
		}
		if (engine->getInput()->getKeyDown(SDLK_UP))
		{
            camera->transform.rotate(glm::vec3(3.0f, 0.0f, 0.0));
		}
		if (engine->getInput()->getKeyDown(SDLK_DOWN))
		{
            camera->transform.rotate(glm::vec3(-3.0f, 0.0f, 0.0));
		}

        engine->renderFrame();
    }

    return EXIT_SUCCESS;
}
