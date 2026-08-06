#pragma once

#include "State.hpp"

class Renderer {
public:
    void draw(const State& state, const Camera3D& camera, double fixedDt, bool paused) const;
};

