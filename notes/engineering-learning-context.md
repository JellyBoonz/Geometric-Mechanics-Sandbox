# Engineering Learning Context

## Project goal

This is a small educational sandbox for learning mechanics, numerical
integration, geometric mechanics, Lie-group integrators, and eventually
geometric algebra.

The application shell is intentionally simple so that mathematical decisions
remain visible.

## Current learning target

The project currently has a free-particle example using the discrete update:

```text
qₖ₊₁ = 2qₖ - qₖ₋₁
```

The next conceptual target is understanding how group-valued configuration and
algebra-valued motion could be represented without making the simulation depend
on one particular algebra.

## Assistance preference

- Keep explanations brief and focused on the next question or decision.
- Preserve the user's ownership of mathematical derivations and architecture
  choices.
- Automate setup, repository searches, formatting, and other incidental work.
- Avoid introducing generic abstractions before a concrete nontrivial example
  shows that they are needed.

## Relevant notes

- [[Advisor Questions - Lie Algebra Interface]]
- [[free_particle]]
- [[rotating_free_particle]]
