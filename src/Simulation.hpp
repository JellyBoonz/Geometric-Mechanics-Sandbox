#pragma once

#include "Integrator.hpp"

class Simulation {
  public:
    Simulation();

    void update(double dt);
    void reset();

    const State &state() const;

  private:
    State initialState_{};
    State state_{};
    RotatingFreeParticleIntegrator integrator_{};

    // TODO(ARCHITECTURE): Revisit integrator ownership if runtime selection is
    // needed.
};
