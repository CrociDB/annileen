#include <iostream>
#include "engine.h"
#include "util.h"

int main(int argc, char* argv[])
{
    Engine* engine = new Engine();
    engine->init(800, 600);

    // Create triangle
    Mesh* mesh = new Mesh();

    const float vertexData[] = {
        0.5f, 0.5f, 0.0f,           1.0f, 0.0f, 0.0f,       1.0f, 1.0f, 
        0.5f, -0.5f, 0.0f,          1.0f, 1.0f, 1.0f,       1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,         1.0f, 0.0f, 1.0f,       0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f,          0.0f, 1.0f, 0.0f,       0.0f, 1.0f
    };

    const uint32_t indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    mesh->init(vertexData, 
                sizeof(vertexData) / sizeof(float), 
                VERTEX_COLOR | VERTEX_UV,
                indices, 
                sizeof(indices) / sizeof(float));

    // Triangle material
    Shader* shader = new Shader();
    shader->load("../../shaders/vertex.vert", "../../shaders/fragment.frag");
    Material* material = new Material();
    material->init(shader);

    mesh->setMaterial(material);

    // Create scene
    Scene* scene = new Scene();
    scene->addMesh(mesh);

    engine->setScene(scene);

    while (engine->isRunning())
    {
        engine->checkInputEvents();
        engine->renderFrame();
    }

    return EXIT_SUCCESS;
}
