#include <iostream>
#include "engine.h"
#include "gamescene.h"
#include "gymscene.h"
#include "util.h"

void debug_vec3(const glm::vec3& vec);

int main(int argc, char* argv[])
{
    Engine* engine = Engine::getInstance();
    engine->init(1280, 720);

    // Voxel game scene
    //GameScene* scene = new GameScene();

    // Graphics GYM scene
    GymScene* scene = new GymScene();

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

    float speed = 3.0f;
    float movementSpeed = 2.8f;
    float sensitivity = 18.0f;
    float pitch = 0.0f, yaw = 0.0f;

    scene->start();

    while (engine->run())
    {
        auto dt = engine->getTime().deltaTime;
        engine->checkInputEvents();
        scene->update();

        movementSpeed = engine->getInput()->getKeyDown(SDLK_LSHIFT) ? 5.0 * speed : speed;

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
        if (engine->getInput()->getKeyDown(SDLK_q))
        {
            camera->transform.translate(dt * -movementSpeed * glm::vec3(0.0f, 1.0f, 0.0f));
        }
        if (engine->getInput()->getKeyDown(SDLK_e))
        {
            camera->transform.translate(dt * movementSpeed * glm::vec3(0.0f, 1.0f, 0.0f));
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
