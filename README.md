# Geometric Mechanics Sandbox

This is a small C++20 application for learning mechanics, numerical integration,
and geometric methods by implementing them directly. The current simulation
uses a gafro conformal-GA motor and a body-frame twist to move the box on the
ground.

## Build

Requirements: a C++20 compiler, CMake 3.20 or newer, and Git. CMake uses an
installed raylib package when available and otherwise downloads raylib with
`FetchContent`. It also downloads pinned Eigen and gafro sources. On macOS,
the Homebrew raylib package can be installed with `brew install raylib`.

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
- W/S: apply forward/backward driving force
- A/D: apply counterclockwise/clockwise yaw torque
- Escape: quit
- Mouse and keyboard camera controls: raylib's free camera controls

## Next experiment

The next planned experiment is a maneuverable planar box: keyboard input will
drive it forward/backward and steer its yaw on the `xz` ground plane. It will
then add a sideways applied force and a position-dependent ground-friction
field. The staged model and its open physical choices are in
[[notes/Box Maneuver and Friction Roadmap.md]].

## Execution flow

`main.cpp` constructs `Application` and starts its loop. `Application` updates
the camera and input, accumulates real frame time, and advances `Simulation` in
fixed `1/120` second increments. `Renderer` then draws the current state. When
paused, the accumulator stops and `N` invokes exactly one fixed simulation step.

## Classes and extension points

- `Application`: window lifetime, input, fixed timestep, renderer, and simulation.
- `State`: a gafro `Motor<double>` pose, its `Motor<double>::Generator`
  body-frame twist, and simulation time. The simulation constrains those 3D
  CGA values to ground translation and yaw; the twist holds forward, lateral,
  and yaw velocities.
- `Simulation`: current/initial state, reset, and update delegation.
- `ManeuverableBoxIntegrator`: updates the velocity from forces and torque,
  then reconstructs one fixed motor-valued pose step.
- `Renderer`: read-only drawing of the state, grid, axes, cube, and debug text.

The maneuver integrator maps the body-frame twist over one fixed timestep to a
motor increment with `exp`, then composes it with the current pose. Forces are
still evaluated in the world frame for this deliberately small model, and the
resulting velocity is converted back to the body frame before reconstruction.
