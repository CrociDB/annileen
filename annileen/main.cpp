#include <iostream>
#include "engine/engine.h"

using namespace annileen;

int main(int argc, char* argv[])
{
    Engine* engine = Engine::getInstance();
    engine->init(1920, 1080);

    Scene* scene = new Scene();
    engine->setScene(scene);

    while (engine->run())
    {
        engine->checkInputEvents();

        // everything goes here.
        bgfx::setDebug(BGFX_DEBUG_STATS);

        engine->renderFrame();
    }

    return 0;
}
