#pragma once

#include "PlanarGafro.hpp"

// The quantities that evolve over fixed simulation steps.
struct State {
    planar_gafro::Motor pose{planar_gafro::identity()};
    planar_gafro::Twist velocity{};
    double simulationTime{0.0};
};
