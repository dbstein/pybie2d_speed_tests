import numpy as np
import numexpr as ne

def lk_numexpr(source, target, density):
    """
    2D Laplace Kernel (from source-->targets, no self-interaction testing)

    Implementation based on numexpr
    source:  array(2,N),  float
    targets: array(2,M),  float
    density: array(N),    float
    """
    s_x = source[0]
    s_y = source[1]
    t_x = target[0][:, np.newaxis]
    t_y = target[1][:, np.newaxis]
    D = ne.evaluate('log((s_x-t_x)**2 + (s_y-t_y)**2)')
    scale = -0.25/np.pi
    return D.dot(density*scale)
