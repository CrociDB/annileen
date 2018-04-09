#include <iostream>
#include "engine.h"

int main(int argc, char* argv[])
{
    Engine* engine = new Engine();
    engine->init(800, 600);

    while (engine->isRunning())
    {
        engine->checkInputEvents();
        engine->renderFrame();
    }

    return EXIT_SUCCESS;
}
