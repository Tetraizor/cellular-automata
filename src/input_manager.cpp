#include "input_manager.h"

#include "engine.h"

void InputManager::check_for_events()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        parse_input(&event);
    }
}

void InputManager::parse_input(const SDL_Event *event)
{
    switch (event->type)
    {
    case SDL_QUIT:
        Engine::get().quit();
        break;

    case SDL_KEYDOWN:
        handle_key(event, true);
        break;

    case SDL_KEYUP:
        handle_key(event, false);
        break;

    case SDL_MOUSEBUTTONDOWN:
        handle_mouse_down(event);
        break;

    case SDL_MOUSEBUTTONUP:
        handle_mouse_up(event);
        break;

    case SDL_MOUSEMOTION:
        handle_mouse_motion(event->motion.x, event->motion.y);
        break;

    default:
        break;
    }
}

void InputManager::handle_key(const SDL_Event *key_event, bool is_down)
{
    switch (key_event->key.keysym.sym)
    {
    case SDLK_SPACE:
        if (is_down)
            Engine::get().get_world()->clear();
        break;

    case SDLK_ESCAPE:
        if (is_down)
            Engine::get().quit();

    default:
        break;
    }
}

void InputManager::handle_mouse_down(const SDL_Event *event)
{
    if (event->button.button == SDL_BUTTON_LEFT)
        is_cursor_down = true;
    else if (event->button.button == SDL_BUTTON_RIGHT)
        is_right_cursor_down = true;
}
void InputManager::handle_mouse_up(const SDL_Event *event)
{
    if (event->button.button == SDL_BUTTON_LEFT)
        is_cursor_down = false;
    else if (event->button.button == SDL_BUTTON_RIGHT)
        is_right_cursor_down = false;
}
void InputManager::handle_mouse_motion(int x, int y)
{
    cursor_x = x / Engine::get().get_zoom_factor();
    cursor_y = y / Engine::get().get_zoom_factor();
}
