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

		if (engine->getInput()->getKeyDown(SDLK_LEFT) || engine->getInput()->getKeyDown(SDLK_a))
		{
			mesh->position += glm::vec3(-0.003f, 0.0, 0.0);
		}

		if (engine->getInput()->getKeyDown(SDLK_RIGHT) || engine->getInput()->getKeyDown(SDLK_d))
		{
			mesh->position += glm::vec3(0.003f, 0.0, 0.0);
		}

		if (engine->getInput()->getKeyDown(SDLK_UP) || engine->getInput()->getKeyDown(SDLK_w))
		{
			mesh->position += glm::vec3(0.0, 0.0, -0.003f);
		}

		if (engine->getInput()->getKeyDown(SDLK_DOWN) || engine->getInput()->getKeyDown(SDLK_s))
		{
			mesh->position += glm::vec3(0.0, 0.0, 0.003f);
		}

        engine->renderFrame();
    }

    return EXIT_SUCCESS;
}
