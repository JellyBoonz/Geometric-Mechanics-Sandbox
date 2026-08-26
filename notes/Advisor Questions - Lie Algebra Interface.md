## Context

The sandbox currently has a free-particle discrete update:

```text
qₖ₊₁ = qₖ + (qₖ - qₖ₋₁)
```

In the code, `qₖ` and `qₖ₋₁` are `Vector3` positions. The difference is being
treated as an algebra-valued displacement, and addition is being treated as
group composition. This is valid for the additive translation group `R³`, but
the two concepts are not yet represented separately.

The longer-term goal is to allow the simulation and integrator logic to work
with different geometric algebras or Lie groups without knowing their concrete
representation.

## Questions

1. **What should be the first nontrivial group for this project?** Should I
   move from the current additive translation group `R³` to `SO(2)`, `SO(3)`,
   or `SE(2)`? I want the smallest example that demonstrates a nontrivial
   exponential map.
	1. Work on box model adding rotation as if box is turning.

3. **How would geometric algebra represent the quantities in this project?**
   Given a particle or rigid-body pose and its velocity, which geometric-algebra
   objects correspond to the configuration, the motion, and the increment? What
   makes those representations appropriate?
	1. https://www.youtube.com/watch?v=tX4H_ctggYo
	2. https://depositonce.tu-berlin.de/items/12ff6379-bed1-451f-a2d6-0483bec32619

5. **Which geometric-algebra operations would the integrator actually need?**
   Starting from the current update, what operations would map a motion over one
   timestep into an updated configuration? Which of those operations should be
   exposed through the interface, and which should remain inside the concrete
   geometric-algebra implementation?

## Preliminary design — deliberately small

The first useful boundary is not a general `LieGroup` base class. It is the
small set of operations the integrator needs:

```text
Group element:
    compose another group increment

Algebra element:
    be scaled by dt
    be mapped to a group increment by exp

Integrator:
    compute or receive an algebra increment
    map it to a group increment
    compose it with the current pose
```

In C++-like pseudocode:

```cpp
struct Algebra {
    // The concrete representation is chosen by the system.
};

struct GroupElement {
    // The concrete representation is chosen by the system.
};

GroupElement exp(const Algebra& algebra);
GroupElement compose(const GroupElement& a, const GroupElement& b);
Algebra scale(const Algebra& algebra, double amount);
```

The intended update shape is:

```cpp
Algebra algebraIncrement = scale(velocity, dt);
GroupElement groupIncrement = exp(algebraIncrement);
pose = compose(pose, groupIncrement);
```

For the current `R³` experiment, these operations reduce to:

```cpp
algebraIncrement = velocity * dt;
groupIncrement = algebraIncrement;
pose = position + groupIncrement;
```

For a future nontrivial group, the same sequence remains, but `exp` and
`compose` are no longer ordinary vector operations.

## Important limitation

This interface only describes kinematic reconstruction. It does not derive the
variational equations or hide every possible algebra. The chosen integrator
still needs a way to compute its velocity, momentum, or algebra increment.

That suggests keeping two concerns separate:

```text
discrete dynamics → algebra increment / velocity
group reconstruction → exp and compose
```

The next implementation experiment should therefore be one concrete nontrivial
group operation. `SO(2)` is a candidate because it introduces a real exponential map while keeping the state small.

## Current code mapping

```text
State::position                         group element qₖ
State::prevPos                          previous group element qₖ₋₁
Vector3Subtract(position, prevPos)      additive group increment
Vector3Add(position, increment)          group composition in R³
FreeParticleIntegrator::step            discrete dynamics plus reconstruction
```

The design question is whether the future code should split the last line into
two explicit responsibilities, while keeping the public simulation call as
simple as:

```cpp
simulation.update(dt);
```
