So right now we have linear velocity * delta time to get a moving cube. Where are we going next?

>[! Note]
>The end goal is to discretize a lagrangian for this system, and apply the variational integrator to it to get a calculated next step given the current state

> [!question]
> Where does the Lie group stuff fit into this?

See [[Interface]].

>[!question]
>Ok, but *how* is the Lie group used? where exactly?

So far, I see that we use that exponential mapping on the shape * the controls and multiply the result with the current configuration state $g_k$ to get the next configuration state $g_{k+1}$ 

>[!important]
> The discrete equations of motion are attained by substituting the reduced lagrangian and (holonomic?) connection into the discrete variational principle