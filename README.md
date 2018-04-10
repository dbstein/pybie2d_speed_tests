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
