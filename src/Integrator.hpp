#pragma once

#include "State.hpp"
#include <vsr/detail/vsr_generic_op.h>
#include "raymath.h"

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
        state.position = Vector3Add(state.position, state.linearVelocity * dt);

        // TODO(PHYSICS): Integrate position and velocity once a model exists.
        // TODO(INTEGRATOR): Add a real numerical integration method.
    }
};

class FreeParticleIntegrator : public Integrator {
  public:
    void step(State &state, double dt) override {
        state.simulationTime += dt;
        float h = dt;
        state.prevPos = state.position - Vector3Scale(state.linearVelocity, h);
        Vector3 increment = Vector3Subtract(state.position, state.prevPos); // Algebra Displacement
        Vector3 nextPosition = Vector3Add(state.position, increment); // Group composition

        state.prevPos = state.position;
        state.position = nextPosition;
    }
};

class RotatingFreeParticleIntegrator : public Integrator {
  public:
    void step(State &state, double dt) override {
        state.simulationTime += dt;

        const auto deltaRotor = vsr::nga::Gen::rot(
            state.angularVelocity * (static_cast<double>(dt) * 0.5));
        state.orientation = state.orientation * deltaRotor;

        // TODO(PHYSICS): Integrate angular velocity from rotational dynamics.
        // TODO(INTEGRATOR): Replace this kinematic test with derived equations.
    }
};
