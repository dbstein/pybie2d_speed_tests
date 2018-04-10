import numpy as np
from _fortran_laplace_kernel import fortran_laplace_kernel

def lk_fortran(source, target, density):
    """
    2D Laplace Kernel (from source-->targets, no self-interaction testing)

    Implementation based on numexpr
    source:  array(2,N),  float
    targets: array(2,M),  float
    density: array(N),    float
    output:  array(M),    float
    """
    s_x = source[0]
    s_y = source[1]
    t_x = target[0][:, np.newaxis]
    t_y = target[1][:, np.newaxis]
    return fortran_laplace_kernel(s_x, s_y, t_x, t_y, density)
