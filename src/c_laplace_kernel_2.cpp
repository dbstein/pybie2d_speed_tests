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

void test_kernel(int npts) {
    double *sPtr = new double[2 * npts]; // source coordinates
    double *tPtr = new double[2 * npts]; // target coordinates
    double *fPtr = new double[npts];     // force
    double *vPtr = new double[npts];     // velocity
    double tx, ty, trgValue, sx, sy, ff, rx, ry, rnorm2, logr2;

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

    for(int k=0; k<100; k++){
        for (int i = 0; i < npts; i++) {
            tx = tPtr[2*i];
            ty = tPtr[2*i + 1];
            trgValue = 0;
            for (int j = 0; j < npts; j++) {
                sx = sPtr[2*j];
                sy = sPtr[2*j + 1];
                ff = fPtr[j];
                rx = (tx - sx);
                ry = (ty - sy);
                rnorm2 = rx * rx + ry * ry;
                logr2 = Log(rnorm2);
                trgValue += ff*logr2;
            }
            vPtr[i] += trgValue*factor4pi;
        }
    }

    timer.stop();
    timer.dump();
    printf("    Time for n=%ld is %lf ms\n", npts, 1000*timer.getTime()/100);
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
    double tx, ty, trgValue, sx, sy, ff, rx, ry, rnorm2, logr2;
    int i, j;

    for (i = 0; i < npts; i++){
        fPtr[i] = 1;
        vPtr[i] = 0;
    }
    for (i = 0; i < 2*npts; i++){
        sPtr[i] = i;
        tPtr[i] = i+0.5;
    }

    const double factor4pi = -1 / (4 * Pi);

    Timer timer;
    timer.start();

    for(int k=0; k<100; k++){

    #pragma omp parallel for private(tx,ty,trgValue,sx,sy,ff,rx,ry,rnorm2,logr2,i,j)
    for (i = 0; i < npts; i++) {
        tx = tPtr[2*i];
        ty = tPtr[2*i + 1];
        trgValue = 0;
        for (j = 0; j < npts; j++) {
            sx = sPtr[2*j];
            sy = sPtr[2*j + 1];
            ff = fPtr[j];
            rx = (tx - sx);
            ry = (ty - sy);
            rnorm2 = rx * rx + ry * ry;
            logr2 = Log(rnorm2);
            trgValue += ff*logr2;
        }
        vPtr[i] += trgValue*factor4pi;
    }

    }

    timer.stop();
    timer.dump();
    printf("    Time for n=%ld is %lf ms\n", npts, 1000*timer.getTime()/100);
    delete[] sPtr;
    delete[] tPtr;
    delete[] fPtr;
    delete[] vPtr;
}

double test_kernel_parallel2(int npts) {
    double *sPtr = new double[2 * npts]; // source coordinates
    double *tPtr = new double[2 * npts]; // target coordinates
    double *fPtr = new double[npts];     // force
    double *vPtr = new double[npts];     // velocity
    double tx, ty, trgValue, sx, sy, ff, rx, ry, rnorm2, logr2;
    int i, j;

    for (i = 0; i < npts; i++){
        fPtr[i] = 1;
        vPtr[i] = 0;
    }
    for (i = 0; i < 2*npts; i++){
        sPtr[i] = i;
        tPtr[i] = i+0.5;
    }

    const double factor4pi = -1 / (4 * Pi);

    Timer timer;
    timer.start();

    #pragma omp parallel for private(tx,ty,trgValue,sx,sy,ff,rx,ry,rnorm2,logr2,i,j)
    for (i = 0; i < npts; i++) {
        tx = tPtr[2*i];
        ty = tPtr[2*i + 1];
        trgValue = 0;
        for (j = 0; j < npts; j++) {
            sx = sPtr[2*j];
            sy = sPtr[2*j + 1];
            ff = fPtr[j];
            rx = (tx - sx);
            ry = (ty - sy);
            rnorm2 = rx * rx + ry * ry;
            logr2 = Log(rnorm2);
            trgValue += ff*logr2;
        }
        vPtr[i] += trgValue*factor4pi;
    }

    timer.stop();
    timer.dump();

    delete[] sPtr;
    delete[] tPtr;
    delete[] fPtr;
    delete[] vPtr;

    return timer.getTime();
}

int main() {
    double timer_helper;
    printf("\n----- Testing serial versions -----\n");
    test_kernel(100);
    test_kernel(1000);
    test_kernel(10000);
    printf("\n----- Testing parallel versions -----\n");
    test_kernel_parallel(100);
    test_kernel_parallel(1000);
    test_kernel_parallel(10000);
    printf("\n----- Testing parallel versions -----\n");
    timer_helper = 0;
    for(int k=0; k<100; k++){
        timer_helper += test_kernel_parallel2(100);
    }
    printf("    Time for n=%ld is %lf ms\n", 100, 1000*timer_helper/100);
    timer_helper = 0;
    for(int k=0; k<100; k++){
        timer_helper += test_kernel_parallel2(1000);
    }
    printf("    Time for n=%ld is %lf ms\n", 1000, 1000*timer_helper/100);
    timer_helper = 0;
    for(int k=0; k<100; k++){
        timer_helper += test_kernel_parallel2(10000);
    }
    printf("    Time for n=%ld is %lf ms\n", 10000, 1000*timer_helper/100);
    return 0;
}

/*
Compiling...
with GCC:
g++ c_laplace_kernel.cpp -fopenmp -O3 -march=native -o test_gcc.exe
with ICC:
icc c_laplace_kernel.cpp -std=c++11 -qopenmp -O3 -xHost -o test_icc.exe
*/
