#include <iostream>
#include "engine.h"
#include "gamescene.h"
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

void debug_vec3(const glm::vec3& vec);

int main(int argc, char* argv[])
{
    Engine* engine = Engine::getInstance();
    engine->init(1024, 600);

    // Create triangle
    Mesh* cubeX = new Mesh();
    cubeX->init(vdata_cube,
        sizeof(vdata_cube) / sizeof(float),
        VERTEX_UV);
    cubeX->transform.translate(glm::vec3(5.0f, 0.0f, 0.0f));

    Mesh* cubeZ = new Mesh();
    cubeZ->init(vdata_cube,
        sizeof(vdata_cube) / sizeof(float),
        VERTEX_UV);
    cubeZ->transform.translate(glm::vec3(0.0f, 0.0f, 5.0f));
    cubeZ->transform.scale = glm::vec3(.5f);



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

    cubeX->setMaterial(material);
    cubeZ->setMaterial(material);
    floor->setMaterial(material);

    // Create scene
    GameScene* scene = new GameScene();
    /*scene->addMesh(cubeX);
    scene->addMesh(cubeZ);*/
    //scene->addMesh(floor);
    scene->buildMap();

    scene->setCubemap(new Cubemap(std::vector<std::string> {
            "../../assets/skybox/stormydays_ft.png",
            "../../assets/skybox/stormydays_bk.png",
            "../../assets/skybox/stormydays_up.png",
            "../../assets/skybox/stormydays_dn.png",
            "../../assets/skybox/stormydays_rt.png",
            "../../assets/skybox/stormydays_lf.png",
    }));

    Camera* camera = scene->getCamera();
    camera->clearType = CameraClearSkybox;
    camera->clearColor = glm::vec3(0.0f, 0.3f, 0.5f);

    engine->setScene(scene);

    float movementSpeed = 2.5f;
    float sensitivity = 18.0f;
    float pitch = 0.0f, yaw = 0.0f;

    while (engine->run())
    {
        auto dt = engine->getTime().deltaTime;

        engine->checkInputEvents();

        cubeZ->transform.rotate(glm::vec3(0.0f, 0.3f, 0.0f));

        if (engine->getInput()->getKeyDown(SDLK_s))
        {
            camera->transform.translate(dt * -movementSpeed * camera->getForward());
        }
        if (engine->getInput()->getKeyDown(SDLK_w))
        {
            camera->transform.translate(dt * movementSpeed * camera->getForward());
        }
        if (engine->getInput()->getKeyDown(SDLK_a)) 
        {
            camera->transform.translate(dt * movementSpeed * camera->getRight());
        }
        if (engine->getInput()->getKeyDown(SDLK_d))
        {
            camera->transform.translate(dt * -movementSpeed * camera->getRight());
        }

        if (engine->getInput()->getKeyDown(SDLK_ESCAPE))
        {
            engine->terminate();
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

        //SDL_Delay(12);
        std::cout << engine->getFPS() << std::endl;
        engine->renderFrame();
    }

    return EXIT_SUCCESS;
}

void debug_vec3(const glm::vec3& vec)
{
    std::cout << vec.x << ", "
              << vec.y << ", "
              << vec.z << std::endl;
}
