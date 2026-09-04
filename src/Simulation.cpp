#include "Simulation.hpp"

Simulation::Simulation() : initialState_{}, state_{initialState_} {
}

void Simulation::update(double dt) {
    integrator_.step(state_, commands_, model_, dt);
}

void Simulation::reset() {
    state_ = initialState_;
}

void Simulation::setCommands(ManeuverCommands commands) {
    commands_ = commands;
}

const State &Simulation::state() const {
    return state_;
}
