#include <iostream>
#include "engine.h"
#include "util.h"

float cube[] = {
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

int main(int argc, char* argv[])
{
    Engine* engine = Engine::getInstance();
    engine->init(800, 600);

    // Create triangle
    Mesh* mesh = new Mesh();
    mesh->init(cube, 
                sizeof(cube) / sizeof(float), 
                VERTEX_UV);
    mesh->transform.position += glm::vec3(0.0f, 0.0f, -3.0f);

    // Mesh material
    Shader* shader = new Shader();
    shader->load("../../shaders/vertex.vert", "../../shaders/fragment.frag");

    Material* material = new Material();
    material->init(shader);
    material->addTexture("mainTex", Texture("../../assets/texture.png"));
    material->loadTextures();

    mesh->setMaterial(material);

    // Create scene
    Scene* scene = new Scene();
    scene->addMesh(mesh);

    engine->setScene(scene);

    while (engine->isRunning())
    {
        engine->checkInputEvents();

        //mesh->transform.rotation = mesh->transform.FromEuler(glm::vec3(0.0, 1.0f * SDL_GetTicks(), 0.0));

		if (engine->getInput()->getKeyDown(SDLK_LEFT) || engine->getInput()->getKeyDown(SDLK_a))
		{
            mesh->transform.Rotate(-10.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		}

		if (engine->getInput()->getKeyDown(SDLK_RIGHT) || engine->getInput()->getKeyDown(SDLK_d))
		{
            mesh->transform.Rotate(10.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		}

		if (engine->getInput()->getKeyDown(SDLK_UP) || engine->getInput()->getKeyDown(SDLK_w))
		{
            mesh->transform.Rotate(10.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		}

		if (engine->getInput()->getKeyDown(SDLK_DOWN) || engine->getInput()->getKeyDown(SDLK_s))
		{
            mesh->transform.Rotate(-10.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		}

        engine->renderFrame();
    }

    return EXIT_SUCCESS;
}
