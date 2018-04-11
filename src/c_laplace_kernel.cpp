#include <omp.h>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "AlignedMemory.hpp"
#include "Timer.hpp"

constexpr double Pi = 3.141592653589793238462643383279;

inline double Abs(double x) { return std::abs(x); }

inline double Sqrt(double x) { return std::sqrt(x); }

inline double Power(double x, double y) { return std::pow(x, y); }

inline double Log(double x) { return std::log(x); }

inline void LaplaceKernel(double *s, double *t, double *f, double *vel) {
    const double sx = s[0];
    const double sy = s[1];

    const double tx = t[0];
    const double ty = t[1];

    const double ff = f[0];

    double &v = vel[0];

    v = -ff*Log( (sx-tx)*(sx-tx) + (sy-ty)*(sy-ty) )/(4.*Pi);
}

void test_kernel(int npts) {
    double *sPtr = new double[2 * npts]; // source coordinates
    double *tPtr = new double[2 * npts]; // target coordinates
    double *fPtr = new double[npts];     // force
    double *vPtr = new double[npts];     // velocity
    for (int i = 0; i < npts; i++){
        fPtr[i] = 1;
        vPtr[i] = 0;
    }
    for (int i = 0; i < 2*npts; i++){
        sPtr[i] = i;
        tPtr[i] = i+0.5;
    }

    const double factor4pi = -1 / (4 * Pi);

    Timer timer;
    timer.start();

    for (int i = 0; i < npts; i++) {
        const double tx = tPtr[2*i];
        const double ty = tPtr[2*i + 1];
        double trgValue = 0;
        for (int j = 0; j < npts; j++) {
            const double sx = sPtr[2*j];
            const double sy = sPtr[2*j + 1];
            const double ff = fPtr[j];
            const double rx = (tx - sx);
            const double ry = (ty - sy);
            const double rnorm2 = rx * rx + ry * ry;
            const double logr2 = Log(rnorm2);
            trgValue += ff*logr2;
        }
        vPtr[i] += trgValue*factor4pi;
    }

    timer.stop();
    timer.dump();
    printf("    Time for n=%ld is %lf ms\n", npts, 1000*timer.getTime());
    delete[] sPtr;
    delete[] tPtr;
    delete[] fPtr;
    delete[] vPtr;
}

void test_kernel_parallel(int npts) {
    double *sPtr = new double[2 * npts]; // source coordinates
    double *tPtr = new double[2 * npts]; // target coordinates
    double *fPtr = new double[npts];     // force
    double *vPtr = new double[npts];     // velocity
    for (int i = 0; i < npts; i++){
        fPtr[i] = 1;
        vPtr[i] = 0;
    }
    for (int i = 0; i < 2*npts; i++){
        sPtr[i] = i;
        tPtr[i] = i+0.5;
    }

    const double factor4pi = -1 / (4 * Pi);

    Timer timer;
    timer.start();

    #pragma omp parallel for
    for (int i = 0; i < npts; i++) {
        const double tx = tPtr[2*i];
        const double ty = tPtr[2*i + 1];
        double trgValue = 0;
        for (int j = 0; j < npts; j++) {
            const double sx = sPtr[2*j];
            const double sy = sPtr[2*j + 1];
            const double ff = fPtr[j];
            const double rx = (tx - sx);
            const double ry = (ty - sy);
            const double rnorm2 = rx * rx + ry * ry;
            const double logr2 = Log(rnorm2);
            trgValue += ff*logr2;
        }
        vPtr[i] += trgValue*factor4pi;
    }

    timer.stop();
    timer.dump();
    printf("    Time for n=%ld is %lf ms\n", npts, 1000*timer.getTime());
    delete[] sPtr;
    delete[] tPtr;
    delete[] fPtr;
    delete[] vPtr;
}

int main() {
    printf("\n----- Testing serial versions -----\n");
    test_kernel(100);
    test_kernel(1000);
    test_kernel(10000);
    printf("\n----- Testing parallel versions -----\n");
    test_kernel_parallel(100);
    test_kernel_parallel(1000);
    test_kernel_parallel(10000);
    return 0;
}
