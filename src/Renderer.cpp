#include "Renderer.hpp"

#include <cstdio>

void Renderer::draw(const State& state, const Camera3D& camera, double fixedDt, bool paused) const {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    BeginMode3D(camera);
    DrawGrid(20, 1.0f);
    DrawLine3D({0.0f, 0.0f, 0.0f}, {3.0f, 0.0f, 0.0f}, RED);
    DrawLine3D({0.0f, 0.0f, 0.0f}, {0.0f, 3.0f, 0.0f}, GREEN);
    DrawLine3D({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 3.0f}, BLUE);
    DrawCube(state.position, 1.0f, 1.0f, 1.0f, MAROON);
    DrawCubeWires(state.position, 1.0f, 1.0f, 1.0f, DARKBROWN);
    EndMode3D();

    const int lineHeight = 22;
    char debugText[512]{};
    std::snprintf(
        debugText,
        sizeof(debugText),
        "FPS: %d\nSimulation time: %.4f s\nTimestep: %.6f s\nState: %s\n\n"
        "Controls:\n  Space  Pause/resume\n  R      Reset\n  N      Single step while paused\n  Esc    Quit",
        GetFPS(),
        state.simulationTime,
        fixedDt,
        paused ? "Paused" : "Running");
    DrawText(debugText, 20, 20, 20, DARKGRAY);
    EndDrawing();
}
