
#ifndef _YAKL_H_
#define _YAKL_H_

#include <iostream>
#include <algorithm>
#include <vector>
#include <limits>
#include "BuddyAllocator.h"

#ifdef __USE_CUDA__
  #define YAKL_LAMBDA [=] __device__
  #define YAKL_INLINE inline __host__ __device__
  #include <cub/cub.cuh>
#elif defined(__USE_HIP__)
  #define YAKL_LAMBDA [=] __host__ __device__
  #define YAKL_INLINE inline __host__ __device__
  #include "hip/hip_runtime.h"
  #include <hipcub/hipcub.hpp>
#else
  #define YAKL_LAMBDA [&]
  #define YAKL_INLINE inline
#endif

#ifdef _OPENMP45
#include <omp.h>
#endif

#ifdef _OPENACC
#include "openacc.h"
#endif


namespace yakl {

  // Memory space specifiers for YAKL Arrays
  int constexpr memDevice = 1;
  int constexpr memHost   = 2;
  #if defined(__USE_CUDA__) || defined(__USE_HIP__)
    int constexpr memDefault = memDevice;
  #else
    int constexpr memDefault = memHost;
  #endif

  int constexpr COLON = std::numeric_limits<int>::min();
  int constexpr NOSPEC = std::numeric_limits<int>::min()+1;


  // Size of the buffer to hold large functors for the CUDA backend to avoid exceeding the max stack frame
  int constexpr functorBufSize = 1024*128;
  // Buffer to hold large functors for the CUDA backend to avoid exceeding the max stack frame
  extern void *functorBuffer;


  // Pool allocator object
  extern BuddyAllocator pool;

  // YAKL allocator and deallocator
  extern std::function<void *( size_t )> yaklAllocDevice;
  extern std::function<void ( void * )>  yaklFreeDevice;

  // YAKL allocator and deallocator
  extern std::function<void *( size_t )> yaklAllocHost;
  extern std::function<void ( void * )>  yaklFreeHost;


  template <int L, int U> class bnd {
  public:
    bnd() = delete;
    static constexpr int l() { return L; }
    static constexpr int u() { return U; }
  };


  // Block the CPU code until the device code and data transfers are all completed
  inline void fence() {
    #ifdef __USE_CUDA__
      cudaDeviceSynchronize();
    #endif
    #ifdef __USE_HIP__
      hipDeviceSynchronize();
    #endif
  }


#include "YAKL_init.h"


  inline void finalize() {
    pool = BuddyAllocator();
    #if defined(__USE_CUDA__)
      cudaFree(functorBuffer);
    #endif
  }


#include "YAKL_unpack.h"


#include "YAKL_parallel_for_c.h"


#include "YAKL_parallel_for_fortran.h"


#include "YAKL_reductions.h"


#include "YAKL_atomics.h"


}


#endif

