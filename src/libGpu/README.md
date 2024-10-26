# Da rules
    
In order to not lose sanity when working with C++/CUDA functions, the following 
rules are hereby established:
- When creating a CUDA source file (`.cu`), also create 2 header files for it:
  - regular `.hpp` header. It should contain definitions for the public interface of your CUDA module. In other words, any function you wish to call from 
  some other part of the code to use your CUDA module, goes here
  - a `.cuh` header, containing definitions for your CUDA kernels (functions with the `__global__` signature). Those headers are NOT to be included in regular C/C++ source files/headers
- functions in the `.hpp` header should be in the `gpu` namespace
- functions in the `.cuh` header should be in the `cuda` namespace

So, as an example:

//example.hpp
```cpp
namespace gpu {
    int add(int a, int b);
}
```
//example.cuh
```cpp
namespace cuda {
    __global__ void add(int a, int b, int* sum);
}
```

//example.cu
```cpp

__global__ void cuda::add(int* a, int*b, int* sum) {
    *sum = *a + *b;
}

int gpu::add(int a, int b){
    // call the cuda::add function here
}

```
