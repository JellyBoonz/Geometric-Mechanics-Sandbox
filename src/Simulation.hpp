#pragma once

#include "Integrator.hpp"

class Simulation {
  public:
    Simulation();

    void update(double dt);
    void reset();
    void setCommands(ManeuverCommands commands);

    const State &state() const;
    const ManeuverCommands &commands() const;
    const ManeuverModel &model() const;

  private:
    State initialState_{};
    State state_{};
    ManeuverCommands commands_{};
    ManeuverModel model_{};
    ManeuverableBoxIntegrator integrator_{};
};
