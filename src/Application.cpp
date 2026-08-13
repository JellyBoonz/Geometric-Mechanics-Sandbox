#include "Application.hpp"

#include <algorithm>

#include "raylib.h"

Application::Application() {
    camera_.position = {8.0f, 6.0f, 8.0f};
    camera_.target = {0.0f, 0.0f, 0.0f};
    camera_.up = {0.0f, 1.0f, 0.0f};
    camera_.fovy = 45.0f;
    camera_.projection = CAMERA_PERSPECTIVE;
}

void Application::run() {
    InitWindow(1280, 720, "Geometric Mechanics Sandbox");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        const double frameTime =
            std::min(static_cast<double>(GetFrameTime()), 0.25);

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            UpdateCamera(&camera_, CAMERA_THIRD_PERSON);

        if (IsKeyPressed(KEY_SPACE)) {
            paused_ = !paused_;
        }
        if (IsKeyPressed(KEY_R)) {
            simulation_.reset();
            accumulator_ = 0.0;
        }
        if (IsKeyPressed(KEY_N) && paused_) {
            simulation_.update(fixedDt);
        }
        if (IsKeyPressed(KEY_ESCAPE)) {
            break;
        }

        if (!paused_) {
            accumulator_ += frameTime;
            while (accumulator_ >= fixedDt) {
                // Instead of updating through simulation, consider using
                // the integrators step function here.
                simulation_.update(fixedDt);
                accumulator_ -= fixedDt;
            }
        }

        renderer_.draw(simulation_.state(), camera_, fixedDt, paused_);
    }

    CloseWindow();
}
