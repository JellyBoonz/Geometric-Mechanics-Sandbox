#pragma once

#include <algorithm>

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

// Commands are normalized player requests.  They are not physical quantities:
// the model maps them to force and torque.
struct ManeuverCommands {
    double drive{0.0};  // W/S: forward/backward, in [-1, 1]
    double steer{0.0};  // D/A: clockwise/counterclockwise, in [-1, 1]
};

// Stable settings of this particular planar-box experiment.
struct ManeuverModel {
    double mass{1.0};
    double yawMomentOfInertia{0.5};
    double maxDriveForce{8.0};
    double maxYawTorque{2.0};
    Vector3 fixedForce{3.0f, 0.0f, 0.0f};
    double gravity{9.81};
    double minFrictionCoefficient{0.20};
    double frictionCoefficientSlopePerMeter{0.05};
    double maxFrictionCoefficient{0.80};
    double stopSpeed{0.01};

    double frictionCoefficientAt(double coordinate) const {
        return std::clamp(minFrictionCoefficient +
                              frictionCoefficientSlopePerMeter * coordinate,
                          minFrictionCoefficient, maxFrictionCoefficient);
    }

    double frictionLimitAt(double coordinate) const {
        return frictionCoefficientAt(coordinate) * mass * gravity;
    }
};

class ManeuverableBoxIntegrator final {
  public:
    void step(State &state, const ManeuverCommands &commands,
              const ManeuverModel &model, double dt) const {
        // The box's local +x direction is its forward direction.  Spinning it
        // by the orientation expresses that direction in the world frame.
        const auto worldForwardBlade = vsr::ega::Vec::x.spin(state.orientation);
        const Vector3 worldForward{
            static_cast<float>(worldForwardBlade[0]),
            0.0f,
            static_cast<float>(worldForwardBlade[2]),
        };

        const Vector3 driveForce =
            Vector3Scale(worldForward,
                         static_cast<float>(commands.drive * model.maxDriveForce));

        // The fixed force is world-frame and always points in +x. Friction
        // grows with world x, making the ground progressively rougher in that
        // direction.
        const Vector3 nonFrictionForce = Vector3Add(driveForce, model.fixedForce);
        const float frictionLimit =
            static_cast<float>(model.frictionLimitAt(state.position.x));
        const float speed = Vector3Length(state.linearVelocity);
        const float nonFrictionForceMagnitude = Vector3Length(nonFrictionForce);

        if (speed <= model.stopSpeed && nonFrictionForceMagnitude <= frictionLimit) {
            // At rest, friction exactly balances the other planar forces when
            // its local capacity is sufficient to hold the box still.
            state.linearVelocity = {};
        } else {
            const Vector3 frictionDirection =
                speed > model.stopSpeed
                    ? Vector3Scale(state.linearVelocity, 1.0f / speed)
                    : Vector3Scale(nonFrictionForce,
                                   1.0f / nonFrictionForceMagnitude);
            const Vector3 frictionForce =
                Vector3Scale(frictionDirection, -frictionLimit);
            const Vector3 netForce = Vector3Add(nonFrictionForce, frictionForce);
            const Vector3 linearAcceleration =
                Vector3Scale(netForce, static_cast<float>(1.0 / model.mass));
            const Vector3 nextVelocity = Vector3Add(
                state.linearVelocity,
                Vector3Scale(linearAcceleration, static_cast<float>(dt)));

            // If this step would reverse motion in a region that can hold the
            // box, stop instead of allowing a numerical back-and-forth.
            if (speed > model.stopSpeed &&
                Vector3DotProduct(nextVelocity, state.linearVelocity) <= 0.0f &&
                nonFrictionForceMagnitude <= frictionLimit) {
                state.linearVelocity = {};
            } else {
                state.linearVelocity = nextVelocity;
            }
        }

        const double yawAcceleration =
            commands.steer * model.maxYawTorque / model.yawMomentOfInertia;

        // Semi-implicit Euler: update velocity from force/torque, then use the
        // new velocity to reconstruct the configuration.
        state.angularVelocity = state.angularVelocity +
                                Bivector::xz * (yawAcceleration * dt);

        state.position = Vector3Add(
            state.position,
            Vector3Scale(state.linearVelocity, static_cast<float>(dt)));
        const auto deltaRotor = vsr::nga::Gen::rot(
            state.angularVelocity * (dt * 0.5));
            
        state.orientation = state.orientation * deltaRotor;
        state.simulationTime += dt;
    }
};
