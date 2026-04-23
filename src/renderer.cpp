#include "renderer.h"

#include "engine.h"

#include "ui/ui_panel.h"
#include "ui/ui_label.h"
#include "ui/ui_button.h"

UIManager *ui_manager = nullptr;
WindowManager *window_manager = nullptr;

UILabel *fps_label_ptr = nullptr;

Renderer::Renderer() : world_ptr(nullptr), world_texture_ptr(nullptr), debug_font(nullptr) {}
Renderer::~Renderer()
{
    if (debug_font)
    {
        TTF_CloseFont(debug_font);
    }
}

void draw_ui(SDL_Renderer *sdl_renderer_ptr, int width, int height)
{
    fps_label_ptr->set_text(std::to_string(Engine::get().get_current_fps()));

    ui_manager->render(sdl_renderer_ptr);
}

bool Renderer::initialize(const World *world_ptr, SDL_Renderer *sdl_renderer_ptr)
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

    window_manager = Engine::get().get_window_manager();
    ui_manager = Engine::get().get_ui_manager();

    auto sidebar_panel = std::make_unique<UIPanel>(0, height - 100, width, height, 0xCC303030);
    ui_manager->add_element(std::move(sidebar_panel));

    auto fps_label = std::make_unique<UILabel>(0, 0, debug_font, "Test", 0xFFFFFFFF);
    fps_label_ptr = fps_label.get();
    ui_manager->add_element(std::move(fps_label));

    // Right panel
    UIRect right_panel = window_manager->get_right_panel_rect();
    int zoom = window_manager->get_zoom_factor();
    int rp_x = right_panel.x * zoom;
    int rp_w = right_panel.width * zoom;

    auto rp_bg = std::make_unique<UIPanel>(rp_x, 0, rp_w, height, 0xFF252525);
    ui_manager->add_element(std::move(rp_bg));

    struct MatDef { uint8_t type; const char *name; uint32_t color; };
    MatDef materials[] = {
        { MaterialType::SAND,  "Sand",  mat_colors[MaterialType::SAND]  },
        { MaterialType::STONE, "Stone", mat_colors[MaterialType::STONE] },
        { MaterialType::WATER, "Water", mat_colors[MaterialType::WATER] },
        { MaterialType::GAS,   "Gas",   mat_colors[MaterialType::GAS]   },
    };

    int btn_margin = 8;
    int btn_w = rp_w - btn_margin * 2;
    int btn_h = 32;
    int btn_gap = 6;
    int btn_start_y = 50;

    for (int i = 0; i < 4; ++i)
    {
        uint8_t mat_type = materials[i].type;
        int bx = rp_x + btn_margin;
        int by = btn_start_y + i * (btn_h + btn_gap);

        auto btn = std::make_unique<UIButton>(
            bx, by, btn_w, btn_h, zoom,
            debug_font, materials[i].name, materials[i].color,
            [mat_type]() { Engine::get().set_selected_material(mat_type); },
            [mat_type]() { return Engine::get().get_selected_material() == mat_type; });

        ui_manager->add_element(std::move(btn));
    }

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

        UIRect world_rect = window_manager->get_world_rect();
        int zoom_factor = window_manager->get_zoom_factor();

        SDL_Rect sdl_world_rect;
        sdl_world_rect.x = world_rect.x * zoom_factor;
        sdl_world_rect.y = world_rect.y * zoom_factor;
        sdl_world_rect.w = world_rect.width * zoom_factor;
        sdl_world_rect.h = world_rect.height * zoom_factor;

        SDL_RenderCopy(sdl_renderer_ptr, world_texture_ptr, NULL, &sdl_world_rect);
    }

    // UI
    draw_ui(sdl_renderer_ptr, width, height);

    SDL_RenderPresent(sdl_renderer_ptr);
}

const uint32_t *Renderer::get_pixel_buffer() const { return pixel_buffer.data(); }