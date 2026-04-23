#include "window_manager.h"

#include <iostream>

#include "engine.h"

WindowManager::~WindowManager()
{
    std::cout << "Closing window..." << std::endl;

    // Graceful exit
    if (sdl_renderer_ptr)
    {
        SDL_DestroyRenderer(sdl_renderer_ptr);
        sdl_renderer_ptr = nullptr;
    }

    if (sdl_window_ptr)
    {
        SDL_DestroyWindow(sdl_window_ptr);
        sdl_renderer_ptr = nullptr;
    }

    SDL_Quit();
}

bool WindowManager::initialize()
{
    int right_panel_width = 120;
    int bottom_panel_height = 40;
    int top_panel_height = 40;

    int world_width = Engine::get().get_world_width();
    int world_height = Engine::get().get_world_height();

    WindowManager::zoom_factor = Engine::get().get_zoom_factor();
    WindowManager::unscaled_window_width = (world_width + right_panel_width);
    WindowManager::unscaled_window_height = (world_height + bottom_panel_height + top_panel_height);

    top_panel_rect.x = 0;
    top_panel_rect.y = 0;
    top_panel_rect.width = unscaled_window_width;
    top_panel_rect.height = top_panel_height;

    bottom_panel_rect.x = 0;
    bottom_panel_rect.y = 0;
    bottom_panel_rect.width = world_width;
    bottom_panel_rect.height = bottom_panel_height;

    right_panel_rect.x = world_width;
    right_panel_rect.y = 0;
    right_panel_rect.width = right_panel_width;
    right_panel_rect.height = unscaled_window_height;

    world_rect.x = 0;
    world_rect.y = top_panel_height;
    world_rect.width = world_width;
    world_rect.height = world_height;

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

    sdl_window_ptr = SDL_CreateWindow(
        window_title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        get_scaled_window_width(),
        get_scaled_window_height(),
        SDL_WINDOW_SHOWN);

    if (!sdl_window_ptr)
    {
        std::cerr << "Window failed to create: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    sdl_renderer_ptr = SDL_CreateRenderer(
        sdl_window_ptr,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!sdl_renderer_ptr)
    {
        std::cerr << "Renderer faield to create: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    return true;
}