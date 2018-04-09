#include <iostream>
#include "engine.h"

int main(int argc, char* argv[])
{
    Engine* engine = new Engine();
    engine->init(800, 600);

    SDL_Event windowEvent;
    while (engine->isRunning())
    {
        if (SDL_PollEvent(&windowEvent))
        {
            if (windowEvent.type == SDL_QUIT)
            {
                break;
            }
        }

        engine->renderFrame();
    }

    return EXIT_SUCCESS;
}
