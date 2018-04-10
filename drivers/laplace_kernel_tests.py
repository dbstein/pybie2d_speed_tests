import numpy as np
import pyBIE2D_Speed_Tests as ST
from pyBIE2D_Speed_Tests.laplace_kernel.lk_numpy import lk_numpy
from pyBIE2D_Speed_Tests.laplace_kernel.lk_numexpr import lk_numexpr
from pyBIE2D_Speed_Tests.laplace_kernel.lk_numba import lk_pure_numba, lk_mixed_numba
import numexpr as ne
# import ctypes
# mkl_rt = ctypes.CDLL('libmkl_rt.so')
# def set_num_threads(threads=None):
#     if threads is None:
#         threads = mkl_rt.mkl_get_max_threads()
#     mkl_rt.mkl_set_num_threads(ctypes.byref(ctypes.c_int(threads)))

"""
Rigorous speed test of different kernel code for computing the Laplace greens
kernel in 2 dimensions
"""

NS = (100, 1000, 10000)
MS = (200, 1000, 10000)

for N, M in zip(NS, MS):

    print('\n----- 2D Laplace Kernel Source-->Target Timing Tests -----')
    print('Running for', N, 'source points and', M, 'target points.')

    source_x = np.random.rand(N)
    source_y = np.random.rand(N)
    target_x = np.random.rand(M)
    target_y = np.random.rand(M)
    source = np.row_stack([source_x, source_y])
    target = np.row_stack([target_x, target_y])
    tau = np.random.rand(N)
    qw = np.ones_like(tau)
    sd = np.empty(N, dtype=float)
    D = np.empty([M, N], dtype=float)
    out = np.empty(M, dtype=float)

    print('\nBasic numpy')
    result_numpy = lk_numpy(source, target, tau, qw)
    %timeit lk_numpy(source, target, tau, qw)

    print('\nNumexpr -- One Core')
    ne.set_num_threads(1)
    ne.set_vml_num_threads(1)
    result_numexpr = lk_numexpr(source, target, tau, qw, D)
    %timeit lk_numexpr(source, target, tau, qw, D)

    print('\nNumexpr -- Max Cores')
    ne.set_num_threads(ne.detect_number_of_cores())
    ne.set_vml_num_threads(1)
    %timeit lk_numexpr(source, target, tau, qw, D)

    print('\nNumexpr -- Max VML Cores')
    ne.set_num_threads(1)
    ne.set_vml_num_threads(ne.detect_number_of_cores())
    %timeit lk_numexpr(source, target, tau, qw, D)

    print('\nNumba -- Pure')
    lk_pure_numba(source, target, tau, qw, sd, out)
    result_pure_numba = out.copy()
    %timeit lk_pure_numba(source, target, tau, qw, sd, out)

    print('\nNumba -- Mixed')
    lk_mixed_numba(source, target, tau, qw, D, out)
    result_mixed_numba = out.copy()
    %timeit lk_mixed_numba(source, target, tau, qw, D, out)

    print('\nSanity Checks')
    print('Numpy vs. Numexpr    ', np.allclose(result_numpy, result_numexpr))
    print('Numpy vs. Pure Numba ', np.allclose(result_numpy, result_pure_numba))
    print('Numpy vs. Mixed Numba', np.allclose(result_numpy, result_mixed_numba))
