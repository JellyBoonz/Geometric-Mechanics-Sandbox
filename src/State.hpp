#pragma once

#include "raylib.h"
#include <vsr/space/vsr_ega3D_types.h>

using Rotor = vsr::ega::Rot;
using Bivector = vsr::ega::Biv;

// The complete state needed by the initial simulation.
struct State {
    Vector3 position{0.0f, 0.0f, 0.0f};
    Vector3 prevPos{0.0f, 0.0f, 0.0f};
    Vector3 linearVelocity{1.0f, 0.0f, 0.0f};
    Rotor orientation{1.0};
    Bivector angularVelocity{Bivector::xy};
    double simulationTime{0.0};

    // TODO(MATH): Add any invariants or additional quantities required by the
    // model.
};
