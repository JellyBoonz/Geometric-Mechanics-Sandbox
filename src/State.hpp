#pragma once

#include "raylib.h"

// The complete state needed by the initial simulation.
struct State {
    Vector3 position{0.0f, 0.0f, 0.0f};
    Vector3 linearVelocity{0.0f, 0.0f, 0.0f};
    double simulationTime{0.0};

    // TODO(MATH): Add any invariants or additional quantities required by the model.
};
