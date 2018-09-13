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

double test_kernel(int npts, int num_threads) {
    double *sPtr = new double[2 * npts]; // source coordinates
    double *tPtr = new double[2 * npts]; // target coordinates
    double *fPtr = new double[npts];     // force
    double *vPtr = new double[npts];     // velocity
    for (int i = 0; i < npts; i++){
        fPtr[i] = 1;
        vPtr[i] = 0;
    }
    for (int i = 0; i < 2*npts; i++){
        sPtr[i] = i+Pi;
        tPtr[i] = i+0.5;
    }

    const double factor4pi = -1 / (4 * Pi);

    omp_set_num_threads(num_threads);

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

    delete[] sPtr;
    delete[] tPtr;
    delete[] fPtr;
    delete[] vPtr;

    return timer.getTime();
}

int main() {
    double timer_helper;
    // values of n to test over
    int *ns = new int[3];
    ns[0] = 100;
    ns[1] = 1000;
    ns[2] = 10000;
    // number of repititions per n
    int *reps = new int[3];
    reps[0] = 100000;
    reps[1] = 1000;
    reps[2] = 10;
    // get the number of max threads for the machine
    int max_threads=omp_get_max_threads();
    printf("\nNumber of Threads: %ld\n", max_threads);
    // number of cores to test on
    int *cores = new int[3];
    cores[0] = 1;
    cores[1] = max_threads;
    cores[2] = max_threads/2;
    // to report which test we're doing
    const char *str[3];
    str[0] = "\n----- Testing serial versions -----\n";
    str[1] = "\n----- Testing parallel versions -----\n";
    str[2] = "\n----- Testing parallel versions, half cores -----\n";

    // this is a dummy execution to get threads set up
    test_kernel(1000, max_threads);
    // now the real tests
    for (int j=0; j<3; j++){ // loop over cores
        printf(str[j]);
        for (int i=0; i<3; i++){ // loop over ns
            timer_helper = 0;
            for (int k=0; k<reps[i]; k++){  // loop over repitions
                timer_helper += test_kernel(ns[i], cores[j]);
            }
            // print timing to user
            printf("    Time for n=%ld is %lf ms\n", ns[i], 1000*timer_helper/reps[i]);
        }
    }
    printf("\n");
    return 0;
}

/*
Compiling...
with GCC:
g++ c_laplace_kernel.cpp -fopenmp -O3 -march=native -o test_gcc.exe
with ICC:
icc c_laplace_kernel.cpp -std=c++11 -qopenmp -O3 -xHost -o test_icc.exe
*/
