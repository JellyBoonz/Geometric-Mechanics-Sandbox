#pragma once

#include <algorithm>

#include "State.hpp"
#include "raymath.h"

class Integrator {
  public:
    virtual ~Integrator() = default;
    virtual void step(State &state, double dt) = 0;
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
        // The body-frame twist is converted only where the world-frame force
        // model needs it.  The state itself never stores a world velocity.
        Vector3 worldVelocity = planar_gafro::bodyToWorld(
            state.pose, planar_gafro::linearVelocity(state.velocity));
        const Vector3 worldForward =
            planar_gafro::bodyToWorld(state.pose, {1.0f, 0.0f, 0.0f});

        const Vector3 driveForce =
            Vector3Scale(worldForward,
                         static_cast<float>(commands.drive * model.maxDriveForce));

        // The fixed force is world-frame and always points in +x. Friction
        // grows with world x, making the ground progressively rougher in that
        // direction.
        const Vector3 nonFrictionForce = Vector3Add(driveForce, model.fixedForce);
        const float frictionLimit =
            static_cast<float>(model.frictionLimitAt(
                planar_gafro::translation(state.pose).x));
        const float speed = Vector3Length(worldVelocity);
        const float nonFrictionForceMagnitude = Vector3Length(nonFrictionForce);

        if (speed <= model.stopSpeed && nonFrictionForceMagnitude <= frictionLimit) {
            // At rest, friction exactly balances the other planar forces when
            // its local capacity is sufficient to hold the box still.
            worldVelocity = {};
        } else {
            const Vector3 frictionDirection =
                speed > model.stopSpeed
                    ? Vector3Scale(worldVelocity, 1.0f / speed)
                    : Vector3Scale(nonFrictionForce,
                                   1.0f / nonFrictionForceMagnitude);
            const Vector3 frictionForce =
                Vector3Scale(frictionDirection, -frictionLimit);
            const Vector3 netForce = Vector3Add(nonFrictionForce, frictionForce);
            const Vector3 linearAcceleration =
                Vector3Scale(netForce, static_cast<float>(1.0 / model.mass));
            const Vector3 nextVelocity = Vector3Add(
                worldVelocity,
                Vector3Scale(linearAcceleration, static_cast<float>(dt)));

            // If this step would reverse motion in a region that can hold the
            // box, stop instead of allowing a numerical back-and-forth.
            if (speed > model.stopSpeed &&
                Vector3DotProduct(nextVelocity, worldVelocity) <= 0.0f &&
                nonFrictionForceMagnitude <= frictionLimit) {
                worldVelocity = {};
            } else {
                worldVelocity = nextVelocity;
            }
        }

        const double yawAcceleration =
            commands.steer * model.maxYawTorque / model.yawMomentOfInertia;

        // Semi-implicit Euler: update velocity from force/torque, then use the
        // new velocity to reconstruct the configuration.
        const double nextYawRate = planar_gafro::yawRate(state.velocity) +
                                   yawAcceleration * dt;

        // Reconstruction: algebra-valued body motion -> motor increment ->
        // new motor-valued pose.  exp handles translation and rotation as one
        // SE(2) increment instead of separately adding a position vector and
        // composing a rotor.
        state.velocity = planar_gafro::makeTwist(
            planar_gafro::worldToBody(state.pose, worldVelocity), nextYawRate);
        state.pose *= planar_gafro::exp(state.velocity, dt);

        // Forces above are still deliberately world-frame.  Re-express the
        // same physical velocity in the new body frame for the next step.
        state.velocity = planar_gafro::makeTwist(
            planar_gafro::worldToBody(state.pose, worldVelocity), nextYawRate);
        state.simulationTime += dt;
    }
};
