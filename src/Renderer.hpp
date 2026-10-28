#pragma once

#include "State.hpp"

class Renderer {
public:
    void draw(const State& state, const Camera3D& camera, double fixedDt, bool paused,
              double driveCommand, double steerCommand, Vector3 fixedForce,
              double frictionCoefficient, double frictionLimit) const;
};
