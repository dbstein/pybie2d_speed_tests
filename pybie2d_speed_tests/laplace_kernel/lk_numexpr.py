import numpy as np
import numexpr as ne

def lk_numexpr(source, target, density, D):
    """
    2D Laplace Kernel (from source-->targets, no self-interaction testing)

    Implementation based on numexpr
    source:  array(2,N),  float
    targets: array(2,M),  float
    density: array(N),    float
    D:       array(M, N), float
        D is a preallocated array to avoid on-the-fly
        memory allocation of this large matrix
    """
    s_x = source[0]
    s_y = source[1]
    t_x = target[0][:, np.newaxis]
    t_y = target[1][:, np.newaxis]
    ne.evaluate('log((s_x-t_x)**2 + (s_y-t_y)**2)', out=D)
    scale = -0.25/np.pi
    return D.dot(density*scale)
