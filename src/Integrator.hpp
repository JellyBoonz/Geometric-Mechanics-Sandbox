#pragma once

#include "State.hpp"
#include "raymath.h"
#include <iostream>

class Integrator {
  public:
    virtual ~Integrator() = default;
    virtual void step(State &state, double dt) = 0;
};

// The first integration target is deliberately small: time advances while the
// cube remains stationary until a physical model is added.
class NullIntegrator final : public Integrator {
  public:
    void step(State &state, double dt) override {
        state.simulationTime += dt;
        state.linearVelocity = Vector3{1.0, 0.0, 0.0};
        state.position = Vector3Add(state.position, state.linearVelocity * dt);
        std::cout << "Pos: " << state.position.x << ", " << state.position.y
                  << ", " << state.position.z << "\n";

        // TODO(PHYSICS): Integrate position and velocity once a model exists.
        // TODO(INTEGRATOR): Add a real numerical integration method.
    }
};
