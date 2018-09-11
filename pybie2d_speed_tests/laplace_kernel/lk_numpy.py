import numpy as np

def lk_numpy(source, target, density):
    """
    2D Laplace Kernel (from source-->targets, no self-interaction testing)

    Basic numpy implementation
    source:  array(2,N), float
    target:  array(2,M), float
    density: array(N),   float
    """
    dx = source[0] - target[0][:, np.newaxis]
    dy = source[1] - target[1][:, np.newaxis]
    d = np.log(dx**2 + dy**2)
    scale = -0.25/np.pi
    return d.dot(density*scale)

def lk_numpy_loop(source, target, density):
    """
    2D Laplace Kernel (from source-->targets, no self-interaction testing)

    Basic numpy implementation, not memory hungry
    source:  array(2,N), float
    target:  array(2,M), float
    density: array(N),   float
    """
    N = source.shape[1]
    M = target.shape[1]
    out = np.zeros(N, dtype=float)
    scale = -0.25/np.pi
    sw = scale*density
    for j in range(M):
        dx = source[0] - target[0,j]
        dy = source[1] - target[1,j]
        out[j] = np.log(dx**2 + dy**2).dot(sw)
    return out