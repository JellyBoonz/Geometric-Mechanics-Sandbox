#include "Renderer.hpp"

#include <cstdio>
#include <vsr/detail/vsr_generic_op.h>
#include "rlgl.h"

void Renderer::draw(const State& state, const Camera3D& camera, double fixedDt,
                    bool paused, double driveCommand, double steerCommand,
                    Vector3 fixedForce, double frictionCoefficient,
                    double frictionLimit) const {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    BeginMode3D(camera);
    DrawGrid(20, 1.0f);
    DrawLine3D({0.0f, 0.0f, 0.0f}, {3.0f, 0.0f, 0.0f}, RED);
    DrawLine3D({0.0f, 0.0f, 0.0f}, {0.0f, 3.0f, 0.0f}, GREEN);
    DrawLine3D({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 3.0f}, BLUE);
    const auto axisAngle = vsr::nga::Gen::aa(state.orientation);
    rlPushMatrix();
    rlTranslatef(state.position.x, state.position.y, state.position.z);
    rlRotatef(static_cast<float>(axisAngle[0]), static_cast<float>(axisAngle[1]),
              static_cast<float>(axisAngle[2]), static_cast<float>(axisAngle[3]));
    DrawCube({0.0f, 0.0f, 0.0f}, 1.0f, 1.0f, 1.0f, MAROON);
    DrawCubeWires({0.0f, 0.0f, 0.0f}, 1.0f, 1.0f, 1.0f, DARKBROWN);
    DrawLine3D({0.0f, 0.0f, 0.0f}, {1.5f, 0.0f, 0.0f}, RED);
    DrawLine3D({0.0f, 0.0f, 0.0f}, {0.0f, 1.5f, 0.0f}, GREEN);
    DrawLine3D({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.5f}, BLUE);
    rlPopMatrix();
    EndMode3D();

    const int lineHeight = 22;
    char debugText[512]{};
    std::snprintf(
        debugText,
        sizeof(debugText),
        "FPS: %d\nSimulation time: %.4f s\nTimestep: %.6f s\nState: %s\n\n"
        "Commands:\n  Drive: %.2f\n  Steer: %.2f\n\n"
        "Force field:\n  Fixed force (x, z): (%.2f, %.2f) N\n"
        "  Friction coefficient at x: %.3f\n"
        "  Friction capacity: %.3f N\n\n"
        "Box state:\n  Position (x, z): (%.3f, %.3f) m\n"
        "  Linear velocity (x, z): (%.3f, %.3f) m/s\n"
        "  Yaw angular velocity: %.3f rad/s\n\n"
        "Controls:\n  W/S    Drive forward/backward\n  A/D    Steer left/right\n"
        "  Space  Pause/resume\n  R      Reset\n"
        "  N      Single step while paused\n  Esc    Quit",
        GetFPS(),
        state.simulationTime,
        fixedDt,
        paused ? "Paused" : "Running",
        driveCommand,
        steerCommand,
        fixedForce.x,
        fixedForce.z,
        frictionCoefficient,
        frictionLimit,
        state.position.x,
        state.position.z,
        state.linearVelocity.x,
        state.linearVelocity.z,
        state.angularVelocity[1]);
    DrawText(debugText, 20, 20, 20, DARKGRAY);
    EndDrawing();
}
