#pragma once
#include <SDL2/SDL.h>
#include <cstdint>
#include <iostream>

class Renderer
{
public:
    Renderer(int width, int height);
    ~Renderer();

    bool initialize();

private:
    int width;
    int height;
};