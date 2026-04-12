#include "window_manager.h"

#include <iostream>

WindowManager::~WindowManager()
{
    std::cout << "Closing window..." << std::endl;

    // Graceful exit
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool WindowManager::initialize(int width, int height)
{
    WindowManager::width = width;
    WindowManager::height = height;

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL failed to initialize: " << SDL_GetError() << std::endl;
        return false;
    }

#ifdef BUILD_RELEASE
    const char *window_title = "Cellular Automata";
#else
    const char *window_title = "Cellular Automata (DEBUG)";
#endif

    window = SDL_CreateWindow(
        window_title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_SHOWN);

    if (!window)
    {
        std::cerr << "Window failed to create: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!renderer)
    {
        std::cerr << "Renderer faield to create: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(renderer, 0, 0, width, height);
    SDL_RenderDrawLine(renderer, width, 0, 0, height);

    SDL_RenderPresent(renderer);

    return true;
}