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

void test_kernel(int npts, int reps) {
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

    for(int k=0; k<reps; k++){
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
    printf("    Time for n=%ld is %lf ms\n", npts, 1000*timer.getTime()/reps);
    delete[] sPtr;
    delete[] tPtr;
    delete[] fPtr;
    delete[] vPtr;
}

void test_kernel_parallel(int npts, int reps) {
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

    for(int k=0; k<reps; k++){
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
    printf("    Time for n=%ld is %lf ms\n", npts, 1000*timer.getTime()/reps);
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

int main(int reps) {
    double timer_helper;
    int *ns = new int[npts];
    ns[0] = 100
    ns[1] = 1000
    ns[2] = 10000
    printf("\n----- Testing serial versions -----\n");
    for (int i=0; i<3; i++){
        test_kernel(ns[i], reps);
    }
    printf("\n----- Testing parallel versions -----\n");
    for (int i=0; i<3; i++){
        test_kernel_parallel(ns[i], reps);
    }
    printf("\n----- Testing parallel versions -----\n");
    for (int i=0; i<3; i++){
        timer_helper = 0;
        for(int k=0; k<reps; k++){
            timer_helper += test_kernel_parallel2(ns[i], reps);
        }
        printf("    Time for n=%ld is %lf ms\n", ns[i], 1000*timer_helper/reps);
    }
    return 0;
}

/*
Compiling...
with GCC:
g++ c_laplace_kernel.cpp -fopenmp -O3 -march=native -o test_gcc.exe
with ICC:
icc c_laplace_kernel.cpp -std=c++11 -qopenmp -O3 -xHost -o test_icc.exe
*/
