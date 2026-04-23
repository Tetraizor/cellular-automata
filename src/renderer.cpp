#include "renderer.h"

#include "engine.h"

#include "ui/ui_panel.h"
#include "ui/ui_label.h"
#include "ui/ui_button.h"
#include "font_data.h"

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

static void draw_circle(SDL_Renderer *renderer, int cx, int cy, int radius)
{
    int x = radius - 1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (radius << 1);

    while (x >= y)
    {
        SDL_RenderDrawPoint(renderer, cx + x, cy + y);
        SDL_RenderDrawPoint(renderer, cx + x, cy - y);
        SDL_RenderDrawPoint(renderer, cx - x, cy + y);
        SDL_RenderDrawPoint(renderer, cx - x, cy - y);
        SDL_RenderDrawPoint(renderer, cx + y, cy + x);
        SDL_RenderDrawPoint(renderer, cx + y, cy - x);
        SDL_RenderDrawPoint(renderer, cx - y, cy + x);
        SDL_RenderDrawPoint(renderer, cx - y, cy - x);

        if (err <= 0)
        {
            y++;
            err += dy;
            dy += 2;
        }
        if (err > 0)
        {
            x--;
            dx += 2;
            err += dx - (radius << 1);
        }
    }
}

void draw_ui(SDL_Renderer *sdl_renderer_ptr, int width, int height)
{
    fps_label_ptr->set_text("");

    ui_manager->render(sdl_renderer_ptr);
}

bool Renderer::initialize(const World *world_ptr, SDL_Renderer *sdl_renderer_ptr)
{
    if (TTF_Init() == -1)
    {
        SDL_Log("Font system failed to initialize: %s", TTF_GetError());
    }

    SDL_RWops *font_rw = SDL_RWFromConstMem(pixel_font_data, (int)pixel_font_size);
    debug_font = TTF_OpenFontRW(font_rw, 1, 32);
    if (!debug_font)
    {
        SDL_Log("Font failed to load: %s", TTF_GetError());
    }

    window_manager = Engine::get().get_window_manager();
    ui_manager = Engine::get().get_ui_manager();

    auto sidebar_panel = std::make_unique<UIPanel>(0, height - 100, width, height, 0xCC303030);
    ui_manager->add_element(std::move(sidebar_panel));

    auto fps_label = std::make_unique<UILabel>(20, 12, debug_font, "FPS: 0", 0xFFFFFFFF);
    fps_label_ptr = fps_label.get();
    ui_manager->add_element(std::move(fps_label));

    // Right panel
    UIRect right_panel = window_manager->get_right_panel_rect();
    int zoom = window_manager->get_zoom_factor();
    int rp_x = right_panel.x * zoom;
    int rp_w = right_panel.width * zoom;

    auto rp_bg = std::make_unique<UIPanel>(rp_x, 0, rp_w, height, 0xFF252525);
    ui_manager->add_element(std::move(rp_bg));

    int btn_margin = 8;
    int btn_w = rp_w - btn_margin * 2;
    int btn_h = 32;
    int btn_gap = 6;

    // Material section
    auto mat_label = std::make_unique<UILabel>(rp_x + btn_margin, 14, debug_font, "Material", 0xFFAAAAAA);
    ui_manager->add_element(std::move(mat_label));

    struct MatDef
    {
        uint8_t type;
        const char *name;
        uint32_t color;
    };
    MatDef materials[] = {
        {MaterialType::SAND, "Sand", mat_colors[MaterialType::SAND]},
        {MaterialType::STONE, "Stone", mat_colors[MaterialType::STONE]},
        {MaterialType::WATER, "Water", mat_colors[MaterialType::WATER]},
        {MaterialType::GAS, "Gas", mat_colors[MaterialType::GAS]},
    };

    int mat_start_y = 46;
    for (int i = 0; i < 4; ++i)
    {
        uint8_t mat_type = materials[i].type;
        int bx = rp_x + btn_margin;
        int by = mat_start_y + i * (btn_h + btn_gap);

        auto btn = std::make_unique<UIButton>(
            bx, by, btn_w, btn_h, zoom,
            debug_font, materials[i].name, materials[i].color,
            [mat_type]()
            { Engine::get().set_selected_material(mat_type); },
            [mat_type]()
            { return Engine::get().get_selected_material() == mat_type; });

        ui_manager->add_element(std::move(btn));
    }

    // Brush size section
    int brush_section_y = mat_start_y + 4 * (btn_h + btn_gap) + 14;
    auto brush_label = std::make_unique<UILabel>(rp_x + btn_margin, brush_section_y, debug_font, "Brush Size", 0xFFAAAAAA);
    ui_manager->add_element(std::move(brush_label));

    struct BrushDef
    {
        int radius;
        const char *name;
    };
    BrushDef brushes[] = {
        {6, "Small"},
        {16, "Medium"},
        {24, "Large"},
        {48, "X-Large"},
    };

    int brush_start_y = brush_section_y + 32;
    for (int i = 0; i < 4; ++i)
    {
        int radius = brushes[i].radius;
        int bx = rp_x + btn_margin;
        int by = brush_start_y + i * (btn_h + btn_gap);

        auto btn = std::make_unique<UIButton>(
            bx, by, btn_w, btn_h, zoom,
            debug_font, brushes[i].name, 0xFF606060,
            [radius]()
            { Engine::get().set_brush_radius(radius); },
            [radius]()
            { return Engine::get().get_brush_radius() == radius; });

        ui_manager->add_element(std::move(btn));
    }

    UIRect divider = {rp_x + btn_margin, 410, btn_w, 2};
    auto divider_panel = std::make_unique<UIPanel>(divider.x, divider.y, divider.width, divider.height, 0x55FFFFFF);
    ui_manager->add_element(std::move(divider_panel));

    // Controls section
    int info_x = rp_x + btn_margin;
    int ctrl_y = 420;
    auto ctrl_label = std::make_unique<UILabel>(info_x, ctrl_y, debug_font, "Controls", 0xFFAAAAAA);
    ui_manager->add_element(std::move(ctrl_label));
    auto lmb_label = std::make_unique<UILabel>(info_x, ctrl_y + 28, debug_font, "LMB: Place", 0xFF888888);
    ui_manager->add_element(std::move(lmb_label));
    auto rmb_label = std::make_unique<UILabel>(info_x, ctrl_y + 56, debug_font, "RMB: Erase", 0xFF888888);
    ui_manager->add_element(std::move(rmb_label));
    // Credit

    auto credit_label = std::make_unique<UILabel>(info_x, ctrl_y + 326, debug_font, "Umuthan Özel @ 2026", 0xFF555555);
    ui_manager->add_element(std::move(credit_label));
    credit_label = std::make_unique<UILabel>(info_x, ctrl_y + 350, debug_font, "Cellular Automata v0.1", 0xFF555555);
    ui_manager->add_element(std::move(credit_label));

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

    // Cursor circle
    InputManager *im = Engine::get().get_input_manager();
    int zoom_factor = window_manager->get_zoom_factor();
    int cursor_x = im->get_cursor_x();
    int cursor_y = im->get_cursor_y();
    UIRect world_rect = window_manager->get_world_rect();

    bool on_canvas = cursor_x >= world_rect.x &&
                     cursor_x < world_rect.x + world_rect.width &&
                     cursor_y >= world_rect.y &&
                     cursor_y < world_rect.y + world_rect.height;

    if (on_canvas)
    {
        SDL_ShowCursor(SDL_DISABLE);
        int cx = cursor_x * zoom_factor;
        int cy = cursor_y * zoom_factor;
        int cr = Engine::get().get_brush_radius() * zoom_factor;
        SDL_SetRenderDrawBlendMode(sdl_renderer_ptr, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(sdl_renderer_ptr, 255, 255, 255, 180);
        draw_circle(sdl_renderer_ptr, cx, cy, cr);
        SDL_SetRenderDrawBlendMode(sdl_renderer_ptr, SDL_BLENDMODE_NONE);
    }
    else
    {
        SDL_ShowCursor(SDL_ENABLE);
    }

    SDL_RenderPresent(sdl_renderer_ptr);
}

const uint32_t *Renderer::get_pixel_buffer() const { return pixel_buffer.data(); }