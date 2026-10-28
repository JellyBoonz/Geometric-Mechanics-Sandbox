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

        simulation_.setCommands({
            static_cast<double>(IsKeyDown(KEY_W)) -
                static_cast<double>(IsKeyDown(KEY_S)),
            static_cast<double>(IsKeyDown(KEY_D)) -
                static_cast<double>(IsKeyDown(KEY_A)),
        });

        if (!paused_) {
            accumulator_ += frameTime;
            while (accumulator_ >= fixedDt) {
                simulation_.update(fixedDt);
                accumulator_ -= fixedDt;
            }
        }

        const auto &commands = simulation_.commands();
        const auto &model = simulation_.model();
        renderer_.draw(simulation_.state(), camera_, fixedDt, paused_,
                       commands.drive, commands.steer, model.fixedForce,
                       model.frictionCoefficientAt(
                           planar_gafro::translation(simulation_.state().pose).x),
                       model.frictionLimitAt(
                           planar_gafro::translation(simulation_.state().pose).x));
    }

    CloseWindow();
}
