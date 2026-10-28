#pragma once

#include <cmath>

#include <gafro/algebra.hpp>

#include "raylib.h"
#include "raymath.h"

// The box is constrained to the xz ground plane, but its configuration and
// motion use gafro's full 3D conformal-geometric-algebra motor types.
namespace planar_gafro {

using Motor = gafro::Motor<double>;
using Twist = Motor::Generator;

inline Motor identity() {
    return Motor::Unit();
}

// Generator order is (e12, e13, e23, e1i, e2i, e3i). The planar model uses
// e13 for yaw about y, and e1i/e3i for xz translation. The negative e13
// convention makes positive yaw turn local +x toward world -z.
inline Twist makeTwist(Vector3 bodyLinearVelocity, double yawRate) {
    return Twist({
        0.0, -yawRate, 0.0,
        bodyLinearVelocity.x, 0.0, bodyLinearVelocity.z,
    });
}

inline Vector3 linearVelocity(const Twist &twist) {
    const auto values = twist.vector();
    return {
        static_cast<float>(values[3]),
        0.0f,
        static_cast<float>(values[5]),
    };
}

inline double yawRate(const Twist &twist) {
    return -twist.vector()[1];
}

inline auto transformation(const Motor &motor) {
    return motor.toTransformationMatrix();
}

inline Vector3 translation(const Motor &motor) {
    const auto transform = transformation(motor);
    return {
        static_cast<float>(transform(0, 3)),
        static_cast<float>(transform(1, 3)),
        static_cast<float>(transform(2, 3)),
    };
}

inline double yaw(const Motor &motor) {
    const auto transform = transformation(motor);
    return std::atan2(transform(0, 2), transform(0, 0));
}

inline Vector3 bodyToWorld(const Motor &pose, Vector3 bodyVector) {
    const auto transform = transformation(pose);
    return {
        static_cast<float>(transform(0, 0) * bodyVector.x +
                           transform(0, 2) * bodyVector.z),
        bodyVector.y,
        static_cast<float>(transform(2, 0) * bodyVector.x +
                           transform(2, 2) * bodyVector.z),
    };
}

inline Vector3 worldToBody(const Motor &pose, Vector3 worldVector) {
    const auto transform = transformation(pose);
    return {
        static_cast<float>(transform(0, 0) * worldVector.x +
                           transform(2, 0) * worldVector.z),
        worldVector.y,
        static_cast<float>(transform(0, 2) * worldVector.x +
                           transform(2, 2) * worldVector.z),
    };
}

inline Motor exp(const Twist &twist, double dt) {
    const Vector3 velocity = linearVelocity(twist);
    const double rate = yawRate(twist);
    const double theta = rate * dt;

    // gafro's Motor(generator) constructs a translator and a rotor from the
    // supplied finite components. Convert the constant body twist to that
    // finite translation first, so reconstruction follows the correct curved
    // path while turning.
    double localX = velocity.x * dt;
    double localZ = velocity.z * dt;
    if (std::abs(rate) >= 1e-9) {
        const double sineOverRate = std::sin(theta) / rate;
        const double oneMinusCosineOverRate = (1.0 - std::cos(theta)) / rate;
        localX = sineOverRate * velocity.x + oneMinusCosineOverRate * velocity.z;
        localZ = -oneMinusCosineOverRate * velocity.x + sineOverRate * velocity.z;
    }

    return Motor(makeTwist({static_cast<float>(localX), 0.0f,
                            static_cast<float>(localZ)},
                           theta));
}

}  // namespace planar_gafro
