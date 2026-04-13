#include "renderer.h"

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>

#include "engine.h"

#include "ui/ui_panel.h"
#include "ui/ui_label.h"

UIManager *ui_manager = nullptr;

Renderer::Renderer() : world_ptr(nullptr), world_texture_ptr(nullptr), debug_font(nullptr) {}
Renderer::~Renderer()
{
    if (debug_font)
    {
        TTF_CloseFont(debug_font);
    }
}

void draw_diagnostics_ui()
{
    ImGuiWindowFlags overlay_flags = ImGuiWindowFlags_NoDecoration |
                                     ImGuiWindowFlags_AlwaysAutoResize |
                                     ImGuiWindowFlags_NoSavedSettings |
                                     ImGuiWindowFlags_NoFocusOnAppearing |
                                     ImGuiWindowFlags_NoNav |
                                     ImGuiWindowFlags_NoMove |
                                     ImGuiWindowFlags_NoBackground |
                                     ImGuiWindowFlags_NoInputs;

    ImGui::SetNextWindowPos(ImVec2(10.0f, 10.0f), ImGuiCond_Always);

    if (ImGui::Begin("Stats", nullptr, overlay_flags))
    {
        int fps = Engine::get().get_current_fps();
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "FPS: %d", fps);
    }
    ImGui::End();
}

void draw_ui(SDL_Renderer *sdl_renderer_ptr, int width, int height)
{
    ui_manager->render(sdl_renderer_ptr);
}

bool Renderer::initialize(int width, int height, const World *world_ptr, SDL_Renderer *sdl_renderer_ptr)
{
    if (TTF_Init() == -1)
    {
        SDL_Log("Font system failed to initialize: %s", TTF_GetError());
    }

    debug_font = TTF_OpenFont("assets/fonts/roboto.ttf", 16);
    if (!debug_font)
    {
        SDL_Log("Font failed to load: %s", TTF_GetError());
    }

    ui_manager = Engine::get().get_ui_manager();

    auto sidebar_panel = std::make_unique<UIPanel>(0, height - 100, width, height, 0xCC303030);
    ui_manager->add_element(std::move(sidebar_panel));

    auto title_label = std::make_unique<UILabel>(0, 0, debug_font, "Test", 0xFFFFFFFF);
    ui_manager->add_element(std::move(title_label));

    Renderer::width = width;
    Renderer::height = height;

    Renderer::world_ptr = world_ptr;

    Renderer::sdl_renderer_ptr = sdl_renderer_ptr;

    Renderer::pixel_buffer.reserve(width * height);

    world_texture_ptr = SDL_CreateTexture(
        sdl_renderer_ptr,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        world_ptr->get_width(), world_ptr->get_height());

    return true;
}

void Renderer::render()
{
    // Initialization
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    SDL_SetRenderDrawColor(sdl_renderer_ptr, 60, 60, 60, 255);
    SDL_RenderClear(sdl_renderer_ptr);

    // World
    if (world_texture_ptr == nullptr || world_ptr != nullptr)
    {
        const uint32_t *world_colors = world_ptr->get_cell_colors();

        SDL_UpdateTexture(
            world_texture_ptr,
            NULL,
            world_ptr->get_cell_colors(),
            world_ptr->get_width() * sizeof(uint32_t));

        SDL_RenderCopy(sdl_renderer_ptr, world_texture_ptr, NULL, NULL);
    }

    // UI
    draw_diagnostics_ui();
    draw_ui(sdl_renderer_ptr, width, height);

    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), sdl_renderer_ptr);

    SDL_RenderPresent(sdl_renderer_ptr);
}

const uint32_t *Renderer::get_pixel_buffer() const { return pixel_buffer.data(); }