*What is the configuration?*
$q \in \mathbb{R}^3$
*What is the velocity?*
$\dot{q}$
*What is the energy?*
$KE = \frac{1}2 mv^2$ 
$PE = 0$
$L = KE$

#### Calculating the path
Let's just calculate 2 samples $q_k$ and $q_{k+1}$:
For a timestep $h$,
$$
\dot{q} = \frac{q_{k+1} - q_k}h
$$
*How do we approximate the continuous Lagrangian between these two samples?*
$KE = \frac{1}2 m(\frac{q_{k+1} - q_k}h)^2 - 0 = L(q, \dot{q})$ 

*What is the discrete Lagrangian*
The action accumulated over one time step

*What is the action?*
The result of the Lagrangian.

*How do we calculate the* discrete *Lagrangian*?
$$L_d(q_k, q_{k+1}) = h * L(q, \dot{q}) = S_d$$

Applying the variational principle to this Lagrangian results in: 
$$
D_2L_d(q_{k-1},q_k)
+
D_1L_d(q_k,q_{k+1})
=
0
$$


Solving for this, the update step for a free particle is:
$$
q_{k+1} = 2q_k-q_{k-1}
$$

>[! Important] Thing I Learned
>The discrete equations of motion are derived manually! We just provide the program the update step.

>[! question] So where is the Lie stuff??
> It's actually implicit in the current update step. $q_k$ is the configuration (the group element) and $q_k - q_{k-1}$ is the displacement (or the algebra). Because the algebra is already in $R^3$, it is applied to the group element directly without needing exponential mapping. 