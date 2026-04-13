#pragma once

#include <vector>
#include <cstdint>

struct UIRect
{
    int x, y, width, height;
};

class InputManager;

class UIElement
{
public:
    UIRect bounds;
    bool is_dirty;

    UIElement(int x, int y, int w, int h) : bounds{x, y, w, h}, is_dirty(true) {}
    virtual ~UIElement() = default;

    virtual void update(InputManager *input) = 0;
    virtual void draw(std::vector<uint32_t> &pixel_buffer, int buffer_width) = 0;

    void mark_dirty() { is_dirty = true; }
};
