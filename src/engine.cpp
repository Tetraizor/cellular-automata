#include "engine.h"

#include <iostream>
#include <chrono>
#include <imgui_impl_sdl2.h>

#include "world.h"

Engine::Engine() : ticks(0), millis(0), is_running(true) {}
Engine::~Engine() {}

Engine &Engine::get()
{
    static Engine instance;
    return instance;
}

int Engine::run()
{
    std::cout << "Starting the engine..." << std::endl;

    World world(WORLD_WIDTH, WORLD_HEIGHT);

    if (!wm.initialize(WORLD_WIDTH * ZOOM_FACTOR, WORLD_HEIGHT * ZOOM_FACTOR))
        return 1;

    if (!renderer.initialize(WORLD_WIDTH * ZOOM_FACTOR, WORLD_HEIGHT * ZOOM_FACTOR, &world, wm.get_sdl_renderer_ptr()))
        return 1;

    // Initialize game loop

    const double MAX_FRAME_TIME = 250.0; // For protection against sprial of death
    double accumulator = 0;
    int frame_counter = 0;
    double fps_accumulator = 0;

    auto previous_time = std::chrono::steady_clock::now();

    while (is_running)
    {
        auto frame_start_time = std::chrono::steady_clock::now();

        // Main loop
        auto current_time = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::milli> time_span = current_time - previous_time;
        double elapsed_ms = time_span.count();
        previous_time = current_time;

        if (elapsed_ms > MAX_FRAME_TIME)
        {
            std::cout << "Warning: Lag spike! Dropping "
                      << (elapsed_ms - MAX_FRAME_TIME) << "ms of time." << std::endl;

            elapsed_ms = MAX_FRAME_TIME;
        }

        millis += elapsed_ms;
        accumulator += elapsed_ms;

        // TODO: Handle input

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);

            if (event.type == SDL_QUIT)
            {
                is_running = false;
            }
        }

        while (accumulator >= 1000.0 / FIXED_TICK_RATE)
        {
            // TODO: Game logic

            // TODO: Physics logic
            world.update();

            // TODO: Rendering
            renderer.render();

            ticks++;
            accumulator -= 1000.0 / FIXED_TICK_RATE;
        }

        if (target_fps > 0)
        {
            double target_frame_ms = 1000.0 / target_fps;

            auto frame_end_time = std::chrono::steady_clock::now();
            std::chrono::duration<double, std::milli> work_time = frame_end_time - frame_start_time;

            double sleep_ms = target_frame_ms - work_time.count();

            if (sleep_ms > 0)
            {
                if (sleep_ms > 1.5)
                {
                    SDL_Delay(static_cast<Uint32>(sleep_ms - 1.5));
                }
            }

            while (true)
            {
                auto current_wait_time = std::chrono::steady_clock::now();
                std::chrono::duration<double, std::milli> total_time = current_wait_time - frame_start_time;

                if (total_time.count() >= target_frame_ms)
                    break;
            }
        }

        fps_accumulator += elapsed_ms;
        frame_counter++;

        if (fps_accumulator >= 1000.0)
        {
            target_fps = frame_counter;

            fps_accumulator = 0;
            frame_counter = 0;
        }
    }

    std::cout << "Stopping the engine..." << std::endl;
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    std::cout << "Total tick count: " << ticks << "\tTotal busy wait count: " << frame_counter << std::endl;

    return 0;
}