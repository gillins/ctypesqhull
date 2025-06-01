# ctypesqhull
Library callable from ctypes that calculates area from points using QHull

## Introduction

This library is intented to make it easier to do convex hull calculations
from inside Numba. Without Numba, you might as well use `scipy.spatial.ConvexHull`.

However, for situations where you are looping a lot, this can be slow. If you are using
Numba for you loops you can call C functions via `ctypes` (https://numba.pydata.org/numba-doc/0.15.1/interface_c.html#ctypes). Calling the qhull functions
directly is not possible as the qhull data structures themselves must be accessed.

This library exports a function which can be easily called from `ctypes`. 

## How to build

Using Linux with QHull installed from your distro's package manager:

```
gcc -shared -fPIC -O2 -o ctypesqhull.so ctypesqhull.c -l qhull_r
```

On Linux with Conda:
```
gcc -shared -fPIC -O2 -I $CONDA_PREFIX/include -L $CONDA_PREFIX/lib -o ctypesqhull.so ctypesqhull.c -l qhull_r -Wl,-rpath $CONDA_PREFIX/lib
```

This has not been tested on OSX or Windows, likely more complex steps will be required.

## How to use

Load the `ctypesqhull.so` library and declare the function like this (you may need to modify to have a full path to `ctypesqhull.so`):

```
import ctypes

lib = ctypes.CDLL('./ctypesqhull.so')
func = lib.ctypesqhull
func.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_int]
func.restype = ctypes.c_double
```

Then create a numpy array of shape `numpoints`,`ndims` like this:

```
import numpy
data = numpy.array([[0,0,0], [1,0,0], [0,1,0], [0,0,1]], dtype=float)
```

It is essential that the numpy array is contiguous (ie no strides), in C axis order and `dtype` must be `numpy.float64`. The following `assert`s
can confirm this:

```
assert data.data.c_contiguous
assert data.dtype == numpy.float64
```

Then call the `ctypes` function like this:

```
numpoints, dim = data.shape
area = func(indata.ctypes.data, numpoints, dim)
```

Note that this will work inside Numba `@jit`ed functions as long as the `func` is a global variable in your script.
