#include "window_manager.h"

#include <iostream>

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>

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

    sdl_window_ptr = SDL_CreateWindow(
        window_title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    ImGui_ImplSDL2_InitForSDLRenderer(sdl_window_ptr, sdl_renderer_ptr);
    ImGui_ImplSDLRenderer2_Init(sdl_renderer_ptr);

    ImGui::StyleColorsDark();

    return true;
}