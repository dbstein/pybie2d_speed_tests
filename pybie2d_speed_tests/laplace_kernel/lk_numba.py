import numpy as np
from numba import njit, prange

@njit(parallel=True)
def lk_numba1(source, target, density):
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
    output = np.zeros(M)
    # now the main kernel call
    for i in prange(M):
        tx = t_x[i]
        ty = t_y[i]
        for j in range(N):
            dx = s_x[j] - tx
            dy = s_y[j] - ty
            dr2 = dx**2 + dy**2
            ldr2 = np.log(dr2)
            output[i] += density[j]*ldr2
    # scale the output
    for i  in range(M):
        output[i] *= scale
    return output

@njit(parallel=True)
def lk_numba2(source, target, density):
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
    output = np.zeros(M)
    # now the main kernel call
    for i in prange(M):
        tx = t_x[i]
        ty = t_y[i]
        temp = np.zeros(N)
        for j in range(N):
            dx = s_x[j] - tx
            dy = s_y[j] - ty
            temp[j] = dx**2 + dy**2
        for j in range(N):
            temp[j] = np.log(temp[j])
        for j in range(N):
            output[i] += density[j]*temp[j]
    # scale the output
    for i  in range(M):
        output[i] *= scale
    return output
