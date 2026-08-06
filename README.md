# Geometric Mechanics Sandbox

This is a small C++20 application for learning mechanics, numerical integration,
and geometric methods by implementing them directly. The current simulation
contains a stationary cube and an intentionally minimal application shell.

## Build

Requirements: a C++20 compiler, CMake 3.20 or newer, and Git. CMake uses an
installed raylib package when available and otherwise downloads raylib with
`FetchContent`. On macOS, the Homebrew package can be installed with
`brew install raylib`.

```sh
cmake -S . -B build
cmake --build build
open build/geometric-mechanics-sandbox.app
```

On non-macOS systems, run `./build/geometric-mechanics-sandbox` instead.

## Controls

- Space: pause or resume the simulation
- R: reset to the initial state
- N: advance exactly one fixed step while paused
- Escape: quit
- Mouse and keyboard camera controls: raylib's free camera controls

## Execution flow

`main.cpp` constructs `Application` and starts its loop. `Application` updates
the camera and input, accumulates real frame time, and advances `Simulation` in
fixed `1/120` second increments. `Renderer` then draws the current state. When
paused, the accumulator stops and `N` invokes exactly one fixed simulation step.

## Classes and extension points

- `Application`: window lifetime, input, fixed timestep, renderer, and simulation.
- `State`: position, linear velocity, and simulation time.
- `Simulation`: current/initial state, reset, and update delegation.
- `Integrator`: one-step integration interface; `NullIntegrator` currently only
  advances simulation time.
- `Renderer`: read-only drawing of the state, grid, axes, cube, and debug text.

Future mathematical and physical work belongs primarily in `State.hpp`,
`Integrator.hpp`, and `Simulation.cpp`. The TODO comments in those files mark
the deliberate extension points.
