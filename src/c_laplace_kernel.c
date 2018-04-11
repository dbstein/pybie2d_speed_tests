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

inline void LaplaceKernel(double *s, double *t, double *f, double *vel) {
    const double sx = s[0];
    const double sy = s[1];

    const double tx = t[0];
    const double ty = t[1];

    const double f = f[0];

    double &v = vel[0];

    v = -Log(((sx-tx)**2 + (sy-ty)**2))/(4.*Pi)
}

void test_simple(int npts) {
    double *sPtr = new double[2 * npts]; // source coordinates
    double *tPtr = new double[2 * npts]; // source coordinates
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

    Timer timer;
    timer.start();

    for (int i = 0; i < npts; i++) {
        double v[1] = {0};
        for (int j = 0; j < npts; j++) {
            double vij[1] = {0};
            // i is target
            // j is source
            StokesSLVel(sPtr + 2*j, tPtr + 2*i, fPtr + j, vij);
            v[0] += vij[0];
        }
        vPtr[i] = v[0];
    }

    timer.stop("simple sum complete, ");
    timer.dump();
    printf("result0: %lf; Speed: %lf M interactions/s\n", vPtr[0], (npts * npts) * (1e-6) / timer.getTime());

    delete[] sPtr;
    delete[] tPtr;
    delete[] fPtr;
    delete[] vPtr;
}

// void test_expand(int npts) {
//     double *cPtr = new double[3 * npts]; // coordinate
//     double *fPtr = new double[3 * npts]; // force
//     double *vPtr = new double[3 * npts]; // velocity
//     for (int i = 0; i < 3 * npts; i++) {
//         fPtr[i] = 1;
//         vPtr[i] = 0;
//         cPtr[i] = i;
//     }

//     const double factor8pi = 1 / (8 * Pi);

//     Timer timer;
//     timer.start();

//     for (int i = 0; i < npts; i++) {
//         const double tx = cPtr[3 * i];
//         const double ty = cPtr[3 * i + 1];
//         const double tz = cPtr[3 * i + 2];
//         double trgValueX = 0;
//         double trgValueY = 0;
//         double trgValueZ = 0;
//         // rely on compiler for auto-vectorization
//         for (int j = 0; j < npts; j++) {
//             const double lx = cPtr[3 * j];
//             const double ly = cPtr[3 * j + 1];
//             const double lz = cPtr[3 * j + 2];
//             const double fx = fPtr[3 * j];
//             const double fy = fPtr[3 * j + 1];
//             const double fz = fPtr[3 * j + 2];
//             const double rx = (tx - lx);
//             const double ry = (ty - ly);
//             const double rz = (tz - lz);
//             const double rnorm2 = rx * rx + ry * ry + rz * rz;
//             if (rnorm2 != 0) {
//                 const double rinv = 1 / sqrt(rnorm2);
//                 const double rinv3 = rinv * rinv * rinv;
//                 const double commonFac = (rx * fx + ry * fy + rz * fz);
//                 trgValueX += fx * rinv + commonFac * rx * rinv3;
//                 trgValueY += fy * rinv + commonFac * ry * rinv3;
//                 trgValueZ += fz * rinv + commonFac * rz * rinv3;
//             }
//         }
//         vPtr[3 * i] += trgValueX * factor8pi;
//         vPtr[3 * i + 1] += trgValueY * factor8pi;
//         vPtr[3 * i + 2] += trgValueZ * factor8pi;
//     }

//     timer.stop("expand sum complete, ");
//     timer.dump();
//     printf("result0: %lf; Speed: %lf M interactions/s\n", vPtr[0], (npts * npts) * (1e-6) / timer.getTime());
//     delete[] cPtr;
//     delete[] fPtr;
//     delete[] vPtr;
// }

int main() {
    test_simple(2000);
    test_expand(2000);
    return 0;
}
