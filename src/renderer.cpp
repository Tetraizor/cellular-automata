#include "renderer.h"

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>

#include "engine.h"

World *world = nullptr;
InputManager *input_manager = nullptr;

Renderer::Renderer() : world_ptr(nullptr), world_texture_ptr(nullptr), ui_texture_ptr(nullptr) {}

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

        if (input_manager != nullptr)
        {
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Mouse screen position: %d, %d", input_manager->get_cursor_x(), input_manager->get_cursor_y());
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Mouse cell position  : %d, %d", input_manager->get_cursor_x(), input_manager->get_cursor_y());
        }
    }
    ImGui::End();
}

bool Renderer::initialize(int width, int height, const World *world_ptr, SDL_Renderer *sdl_renderer_ptr)
{
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

    ui_texture_ptr = SDL_CreateTexture(
        sdl_renderer_ptr,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        width, height);

    SDL_SetTextureBlendMode(ui_texture_ptr, SDL_BLENDMODE_BLEND);

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

    // UI
    draw_diagnostics_ui();

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

    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), sdl_renderer_ptr);

    SDL_RenderPresent(sdl_renderer_ptr);
}

const uint32_t *Renderer::get_pixel_buffer() const { return pixel_buffer.data(); }