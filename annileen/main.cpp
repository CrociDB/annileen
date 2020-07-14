#include <iostream>
#include "engine/engine.h"

using namespace annileen;

int main(int argc, char* argv[])
{
    Engine* engine = Engine::getInstance();
    engine->init(1920, 1080, "build_assets/assets.toml");

    Scene* scene = new Scene();
    engine->setScene(scene);

    annileen::Shader* testShader = Engine::getInstance()->getAssetManager()->loadShader("unlit.vs", "unlit.fs");

    while (engine->run())
    {
        engine->checkInputEvents();

        if (engine->getInput()->getKeyDown(GLFW_KEY_ESCAPE))
        {
            engine->terminate();
        }

        // everything goes here.
        bgfx::setDebug(BGFX_DEBUG_STATS);

        engine->renderFrame();
    }

    return 0;
}
