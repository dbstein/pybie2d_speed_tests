# pyBIE2D: Speed Tests
 ---
This is a submoddule meant for rigorously benchmarking several representative boundary integral related tasks computational tasks, with the goal of choosing how to develop the bulk of the toolkit. These tasks should be simple enough that they can be implemented rapidly and efffectively using a variety of different methods, but representative enough of the overall required tasks that we can be confident in our decisions regarding development. An alternative choice might be to allow the user to choose from among a variety of computational backends.

## Tasks

1. Laplace Kernel Evaluation
2. On-the-fly compuataion of local evaluation at many points near to a panel quadrature

## Methods

1. Basic numpy implementations
2. Implementations based on numexpr
3. Implementations based on numba
4. Compiled fortran
5. Compiled C

## Notes

The speed of the function calls will depend on the users machine and underlying python/scipy/numpy installation. What is our goal? Do we want code that will run fast on *any machine*?  Do we want to write the simplest possible code that will run fast on *most peoples machines*?  Are we okay with saying *for optimal performance you must install the Intel Distribution for Python*?

## Running the speed tests

# Python and Fortran tests
To build and install the python and Fortran code using your default fortran compiler, simply execute:

```bash
pip install .
```

To build and install using an alternative Fortran compiler, execute, e.g.:
```bash
python setup.py build --fcompiler=intelem
pip install .
```

To run the tests, simply execute:
```bash
ipython drivers/laplace_kernel_tests.ipy > output/laplace_kernel_NAME.txt
```
where NAME is a descriptive name for the system you are running on.

# C Tests
For now, mostly because I'm not really sure how to, I've yet to integrate the C++ code with the python system. Instead, simply compile the C code (e.g. using GCC):
```bash
g++ -std=c++11 -O3 -march=native -fopenmp c_laplace_kernel.cpp -o test -lm
```
or using ICC:
```bash
icc -std=c++11 -O3 -march=native -fopenmp c_laplace_kernel.cpp -o test -lm
```
Then simply run:
```bash
./test > output/c_laplace_kernel_NAME.txt
```
where again, NAME is a descriptive name for the system you are running on.  For my own reference, the results in the output folder now were created on my Simons workstation using the commands:
```bash
source activate idp_3
python setup.py build --fcompiler=intelem
ipython drivers/laplace_kernel_tests.ipy > output/laplace_kernel_stein_workstation_12_cores_intel_numpy.txt
pip uninstall pyBIE2D_Speed_Tests
source deactivate
module load gcc/7.3.0
module load python
python setup.py build --fcompiler=intelem
ipython drivers/laplace_kernel_tests.ipy > output/laplace_kernel_stein_workstation_12_cores_module_numpy.txt
g++ -std=c++11 -O3 -march=native -fopenmp c_laplace_kernel.cpp -o test -lm
./test > output/c_laplace_kernel_stein_workstation_12_cores_gcc.txt
icc -std=c++11 -O3 -xHost -qopenmp c_laplace_kernel.cpp -o test -lm
./test > output/c_laplace_kernel_stein_workstation_12_cores_icc.txt
```


