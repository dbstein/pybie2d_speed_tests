# pyBIE2D: Speed Tests
 ---
This is a submoddule meant for rigorously benchmarking several representative boundary integral related computational tasks, with the goal of choosing how to develop the bulk of the toolkit. These tasks should be simple enough that they can be implemented rapidly and efffectively using a variety of different methods, but representative enough of the overall required tasks that we can be confident in our decisions regarding development. An alternative choice might be to allow the user to choose from among a variety of computational backends.

## Tasks

1. Laplace Kernel Evaluation

## Methods

1. Basic numpy implementations
2. Implementations based on numexpr
3. Implementations based on numba
4. Compiled fortran
5. Compiled C

## Notes

The speed of the function calls will depend on the users machine and underlying python/scipy/numpy installation. What is our goal? Do we want code that will run fast on *any machine*?  Do we want to write the simplest possible code that will run fast on *most peoples machines*?  Are we okay with saying *for optimal performance you must install the Intel Distribution for Python* or use *ICC* to compile your code?

## Running the speed tests

# Python and Fortran tests
To build and install using the Intel Fortran compiler, execute, e.g.:
```bash
python setup.py build --fcompiler=intelem
pip install .
```

To build and install the python and Fortran code using your default fortran compiler, first modify the appropriate line of setup.py to use the relevant flags for your compiler and then execute:
```bash
pip install .
```

To run, for example, the laplace kernel tests, simply execute:
```bash
ipython drivers/laplace_kernel_tests.ipy > output/laplace_kernel_NAME.txt
```
where NAME is a descriptive name for the system you are running on / compilation settings used.

# C Tests
For now, mostly because I'm not really sure how to, I've yet to integrate the C++ code with the python system. Instead, simply compile the C code (e.g. using GCC):
```bash
g++ -std=c++11 -O3 -march=native -fopenmp c_laplace_kernel.cpp -o test.exe
```
or using ICC:
```bash
icc -std=c++11 -O3 -xHost -qopenmp c_laplace_kernel.cpp -o test.exe
```
Please use the .exe extension since its gitignored. Then simply run:
```bash
./test.exe > output/c_laplace_kernel_NAME.txt
```
where again, NAME is a descriptive name for the system you are running on.  For my own reference, the results in the output folder now were created on my Simons workstation using the commands:
```bash
source activate idp_new
module load gcc/8.2.0
pip install .
ipython drivers/laplace_kernel_tests.ipy 2>&1 | tee output/laplace_kernel_stein_workstation_gfortran.txt
pip uninstall pybie2d_speed_tests
module unload gcc/8.2.0
module load intel/compiler/2017-4
python setup.py build --fcompiler=intelem
pip install .
ipython drivers/laplace_kernel_tests.ipy 2>&1 | tee output/laplace_kernel_stein_workstation_intel.txt
source deactivate
cd src
icc -std=c++11 -O3 -xHost -qopenmp c_laplace_kernel.cpp -o test.exe
./test.exe 2>&1 | tee ../output/c_laplace_kernel_stein_workstation_icc.txt
module unload intel/compiler/compiler/2017-4
module load gcc/8.2.0
g++ -O3 -march=native -fopenmp c_laplace_kernel.cpp -o test.exe
./test.exe 2>&1 | tee ../output/c_laplace_kernel_stein_workstation_gcc.txt
```


