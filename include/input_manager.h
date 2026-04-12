#pragma once

#include <SDL2/SDL.h>

class InputManager
{
public:
    InputManager() : cursor_x(0), cursor_y(0), is_cursor_down(false) {};
    ~InputManager() = default;

    void parse_input(const SDL_Event *event);
    void check_for_events();

    void handle_key(const SDL_Event *key_event, bool is_down);
    void handle_mouse_down();
    void handle_mouse_up();
    void handle_mouse_motion(int x, int y);

    const int get_cursor_x() { return cursor_x; }
    const int get_cursor_y() { return cursor_y; }

    const bool get_is_cursor_down() { return is_cursor_down; }

private:
    int cursor_x;
    int cursor_y;

    bool is_cursor_down;
};