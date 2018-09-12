subroutine fortran_laplace_kernel(n,sx,sy,m,tx,ty,tau,out)
    implicit none

    integer, parameter                    :: dp=kind(0.d0)
    integer,                 intent(in)   :: n, m
    real(dp), dimension(n),  intent(in)   :: sx, sy, tau
    real(dp), dimension(m),  intent(in)   :: tx, ty
    real(dp), dimension(m),  intent(out)  :: out
    integer                               :: i, j
    real(dp)                              :: scale, d

    scale = -0.25_dp/(4.0_dp*atan(1.0_dp))

    out(:) = 0.0_dp
    !$omp simd
    do j = 1, m
        do i = 1, n
            d = (sx(i)-tx(j))**2 + (sy(i)-ty(j))**2
            out(j) = out(j) + tau(i)*log(d)
        end do
    end do
    out(:) = out(:)*scale

end subroutine fortran_laplace_kernel

subroutine fortran_laplace_kernel_parallel(n,sx,sy,m,tx,ty,tau,out)
    use omp_lib
    implicit none

    integer, parameter                      :: dp=kind(0.d0)
    integer,                 intent(in)     :: n, m
    real(dp), dimension(n),  intent(in)     :: sx, sy, tau
    real(dp), dimension(m),  intent(in)     :: tx, ty
    real(dp), dimension(m),  intent(inout)  :: out
    integer                                 :: i, j
    real(dp)                                :: scale, d

    scale = -0.25_dp/(4.0_dp*atan(1.0_dp))

    out(:) = 0.0_dp
    !$omp parallel do shared(sx, tx, sy, ty, out) private(d, i)
    do j = 1, m
        !$omp simd
        do i = 1, n
            d = (sx(i)-tx(j))**2 + (sy(i)-ty(j))**2
            out(j) = out(j) + tau(i)*log(d)
        end do
        !$omp end simd
    end do
    !$OMP end parallel do
    out(:) = out(:)*scale

end subroutine fortran_laplace_kernel_parallel
