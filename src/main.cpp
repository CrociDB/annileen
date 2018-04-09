#include <iostream>
#include <SDL.h>

#include "renderer.h"

int main(int argc, char* argv[])
{
    Renderer* renderer = new Renderer();
    renderer->init(800, 600);

    SDL_Event windowEvent;
    while (true)
    {
        if (SDL_PollEvent(&windowEvent))
        {
            if (windowEvent.type == SDL_QUIT)
            {
                break;
            }
        }

        renderer->renderFrame();
    }

    return EXIT_SUCCESS;
}
