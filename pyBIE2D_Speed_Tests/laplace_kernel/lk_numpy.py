import numpy as np

def lk_numpy(source, target, density, qw):
    """
    2D Laplace Kernel (from source-->targets, no self-interaction testing)

    Basic numpy implementation
    source:  array(2,N), float
    target:  array(2,M), float
    density: array(N),   float
    qw:      array(N),   float
    """
    dx = source[0] - target[0][:, np.newaxis]
    dy = source[1] - target[1][:, np.newaxis]
    d = np.log(np.sqrt(dx**2 + dy**2))
    scale = -0.5/np.pi
    sw = scale*qw*density
    return d.dot(sw)
