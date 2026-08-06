#pragma once

#include "Renderer.hpp"
#include "Simulation.hpp"

class Application {
public:
    Application();
    void run();

private:
    static constexpr double fixedDt = 1.0 / 120.0;

    Camera3D camera_{};
    Renderer renderer_{};
    Simulation simulation_{};
    bool paused_{false};
    double accumulator_{0.0};
};

