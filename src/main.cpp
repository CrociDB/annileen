#include <iostream>
#include "engine.h"

int main(int argc, char* argv[])
{
    Engine* engine = new Engine();
    engine->init(800, 600);

    // Create triangle
    Mesh* triangle = new Mesh();
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };
    triangle->init(vertices, 9);

    // Triangle material
    Shader* shader = new Shader();
    shader->load("../../shaders/vertex.vert", "../../fragment.frag");
    Material* material = new Material();
    material->init(shader);

    triangle->setMaterial(material);

    // Create scene
    Scene* scene = new Scene();
    scene->addMesh(triangle);

    engine->setScene(scene);

    while (engine->isRunning())
    {
        engine->checkInputEvents();
        engine->renderFrame();
    }

    return EXIT_SUCCESS;
}
