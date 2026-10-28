#include <cassert>
#include <cmath>

#include "PlanarGafro.hpp"

namespace {

constexpr double tolerance = 1e-5;

bool near(double actual, double expected) {
    return std::abs(actual - expected) < tolerance;
}

}  // namespace

int main() {
    const auto straight = planar_gafro::makeTwist({2.0f, 0.0f, 0.0f}, 0.0);
    const auto straightIncrement = planar_gafro::exp(straight, 0.25);
    const Vector3 straightTranslation = planar_gafro::translation(straightIncrement);
    assert(near(straightTranslation.x, 0.5));
    assert(near(straightTranslation.z, 0.0));
    assert(near(planar_gafro::yaw(straightIncrement), 0.0));

    // A forward-moving body that yaws through +90 degrees follows a quarter
    // circle. In this xz convention, positive yaw turns +x toward -z.
    const auto turning = planar_gafro::makeTwist({1.0f, 0.0f, 0.0f}, PI / 2.0);
    const auto turningIncrement = planar_gafro::exp(turning, 1.0);
    const Vector3 turningTranslation = planar_gafro::translation(turningIncrement);
    assert(near(turningTranslation.x, 2.0 / PI));
    assert(near(turningTranslation.z, -2.0 / PI));
    assert(near(planar_gafro::yaw(turningIncrement), PI / 2.0));

    auto pose = planar_gafro::exp(
        planar_gafro::makeTwist({0.0f, 0.0f, 0.0f}, PI / 2.0), 1.0);
    pose *= planar_gafro::exp(
        planar_gafro::makeTwist({1.0f, 0.0f, 0.0f}, 0.0), 1.0);
    const Vector3 movedTranslation = planar_gafro::translation(pose);
    assert(near(movedTranslation.x, 0.0));
    assert(near(movedTranslation.z, -1.0));
}
