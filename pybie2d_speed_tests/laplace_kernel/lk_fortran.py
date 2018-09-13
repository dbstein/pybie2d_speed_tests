import numpy as np
from fortran_laplace_kernel import fortran_laplace_kernel_serial
from fortran_laplace_kernel import fortran_laplace_kernel_parallel

def lk_fortran(source, target, density, parallel=True):
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
    if parallel:
        return fortran_laplace_kernel_parallel(s_x, s_y, t_x, t_y, density)
    else:
        return fortran_laplace_kernel_serial(s_x, s_y, t_x, t_y, density)

def lk_fortran_serial(source, target, density):
    return lk_fortran(source, target, density, False)

def lk_fortran_parallel(source, target, density):
    return lk_fortran(source, target, density, True)
