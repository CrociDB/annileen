#include <iostream>
#include "engine/engine.h"

int main(int argc, char* argv[])
{
    Engine* engine = Engine::getInstance();
    engine->init(1920, 1080);

    while (engine->run())
    {
        engine->checkInputEvents();

        // everything goes here.

        engine->renderFrame();
    }

    return 0;
}
