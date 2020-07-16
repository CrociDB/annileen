#include <iostream>
#include "engine/engine.h"

using namespace annileen;

// Example cube
struct PosColorVertex
{
    float pos[3];
    uint8_t col;
};
static PosColorVertex s_cubeVertices[] =
{
    {-1.0f,  1.0f,  1.0f, (uint8_t)0xff000000 },
    { 1.0f,  1.0f,  1.0f, (uint8_t)0xff0000ff },
    {-1.0f, -1.0f,  1.0f, (uint8_t)0xff00ff00 },
    { 1.0f, -1.0f,  1.0f, (uint8_t)0xff00ffff },
    {-1.0f,  1.0f, -1.0f, (uint8_t)0xffff0000 },
    { 1.0f,  1.0f, -1.0f, (uint8_t)0xffff00ff },
    {-1.0f, -1.0f, -1.0f, (uint8_t)0xffffff00 },
    { 1.0f, -1.0f, -1.0f, (uint8_t)0xffffffff },
};

static const uint16_t s_cubeTriList[] =
{
    0, 1, 2, // 0
    1, 3, 2,
    4, 6, 5, // 2
    5, 6, 7,
    0, 2, 4, // 4
    4, 2, 6,
    1, 5, 3, // 6
    5, 7, 3,
    0, 4, 1, // 8
    4, 5, 1,
    2, 3, 6, // 10
    6, 3, 7,
};

int main(int argc, char* argv[])
{
    Engine* engine = Engine::getInstance();
    engine->init(1920, 1080, "build_assets/assets.toml");

    Scene* scene = new Scene();
    engine->setScene(scene);

    annileen::Shader* shader = Engine::getInstance()->getAssetManager()->loadShader("unlit.vs", "unlit.fs");
    std::shared_ptr<Material> material = std::make_shared<Material>();
    material->init(shader);

    bgfx::VertexLayout vlayout;
    vlayout .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .end();

    auto vdata = bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices));
    auto idata = bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList));

    Mesh* mesh = new Mesh();
    mesh->init(vdata, vlayout, idata, sizeof(s_cubeTriList) / sizeof(uint16_t));

    std::shared_ptr<Model> model = std::make_shared<Model>();
    model->init(mesh, material);

    SceneNode* node = scene->createNode();
    node->setModel(model);
    node->getTransform().translate(glm::vec3(0.0, 0.0, 2.0));
    node->getTransform().rotate(glm::vec3(0.0, 45.0, 0.0));

    float speed = 3.0f;
    float movementSpeed = 2.8f;
    float sensitivity = 18.0f;
    float pitch = 0.0f, yaw = 0.0f;

    auto camera = scene->getCamera();
    camera->transform.translate(glm::vec3(0.0, 0.0, -2.0));

    while (engine->run())
    {
        auto dt = engine->getTime().deltaTime;
        engine->checkInputEvents();
        scene->update();

        //movementSpeed = engine->getInput()->getKeyDown(SDLK_LSHIFT) ? 5.0 * speed : speed;

        if (engine->getInput()->getKeyDown(GLFW_KEY_S))
        {
            camera->transform.translate(dt * -movementSpeed * camera->getForward());
        }
        if (engine->getInput()->getKeyDown(GLFW_KEY_W))
        {
            camera->transform.translate(dt * movementSpeed * camera->getForward());
        }
        if (engine->getInput()->getKeyDown(GLFW_KEY_A))
        {
            camera->transform.translate(dt * movementSpeed * camera->getRight());
        }
        if (engine->getInput()->getKeyDown(GLFW_KEY_D))
        {
            camera->transform.translate(dt * -movementSpeed * camera->getRight());
        }
        if (engine->getInput()->getKeyDown(GLFW_KEY_Q))
        {
            camera->transform.translate(dt * -movementSpeed * glm::vec3(0.0f, 1.0f, 0.0f));
        }
        if (engine->getInput()->getKeyDown(GLFW_KEY_E))
        {
            camera->transform.translate(dt * movementSpeed * glm::vec3(0.0f, 1.0f, 0.0f));
        }

        {
           // Camera mouse control
           auto mouseDelta = engine->getInput()->getMouseDelta();

           yaw += mouseDelta.x * sensitivity * dt;
           pitch += -mouseDelta.y * sensitivity * dt;
           pitch = glm::clamp(pitch, -89.0f, 89.0f);
           glm::vec3 cameraForward{
               cos(glm::radians(pitch)) * cos(glm::radians(yaw)),
               sin(glm::radians(pitch)),
               cos(glm::radians(pitch)) * sin(glm::radians(yaw))
           };
           camera->setForward(glm::normalize(cameraForward));
        }

        if (engine->getInput()->getKeyDown(GLFW_KEY_ESCAPE))
        {
            engine->terminate();
        }

        bgfx::setDebug(BGFX_DEBUG_STATS);

        engine->renderFrame();
    }

    return 0;
}
