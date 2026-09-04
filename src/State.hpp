#pragma once

#include "raylib.h"
#include <vsr/space/vsr_ega3D_types.h>

using Rotor = vsr::ega::Rot;
using Bivector = vsr::ega::Biv;

// The quantities that evolve over fixed simulation steps.
struct State {
    Vector3 position{0.0f, 0.0f, 0.0f};
    Vector3 prevPos{0.0f, 0.0f, 0.0f};
    Vector3 linearVelocity{0.0f, 0.0f, 0.0f};
    Rotor orientation{1.0};
    Bivector angularVelocity{0.0};
    double simulationTime{0.0};
};
