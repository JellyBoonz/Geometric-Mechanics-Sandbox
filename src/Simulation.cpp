#include "Simulation.hpp"

Simulation::Simulation() : initialState_{}, state_{initialState_} {
}

void Simulation::update(double dt) {
    integrator_.step(state_, dt);
    initialState_ = state_;
}

void Simulation::reset() {
    state_ = initialState_;
}

const State &Simulation::state() const {
    return state_;
}
