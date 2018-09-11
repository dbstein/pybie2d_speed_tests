import numpy as np
from numba import njit, prange

@njit(parallel=True)
def lk_pure_numba(source, target, density, sd, output):
    """
    2D Laplace Kernel (from source-->targets, no self-interaction testing)

    Pure Numba Implementation
    source:  array(2,N),  float
    targets: array(2,M),  float
    density: array(N),    float
    output:  array(M), float
    """
    N = source.shape[1]
    M = target.shape[1]
    s_x = source[0]
    s_y = source[1]
    t_x = target[0]
    t_y = target[1]
    scale = -0.25/np.pi
    for i in prange(N):
        sd[i] = scale*density[i]
    for j in prange(M):
        output[j] = 0.0
    for j in prange(M):
    	for i in range(N):
            output[j] += sd[i]*np.log((s_x[i]-t_x[j])**2 + (s_y[i]-t_y[j])**2)
