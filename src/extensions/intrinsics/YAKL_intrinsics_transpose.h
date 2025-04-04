
#pragma once
// Included by YAKL_intrinsics.h

namespace yakl {
  namespace intrinsics {

    template <class T, int myMem>
    inline Array<T,2,myMem,styleC> transpose(Array<T,2,myMem,styleC> const &in ) {
      #ifdef KOKKOS_ENABLE_DEBUG
        if (!allocated(in)) Kokkos::abort("ERROR: Calling transpose on unallocated array");
      #endif
      auto d0 = size(in,0);
      auto d1 = size(in,1);
      if constexpr (myMem == memHost) {
        auto out = in.createHostCopy().template reshape<2>( { d1 , d0 } );
        for (int i=0; i < d0; i++) {
          for (int j=0; j < d1; j++) {
            out(j,i) = in(i,j);
          }
        }
        return out;
      } else {
        auto out = in.createDeviceCopy().template reshape<2>( { d1 , d0 } );
        c::parallel_for( YAKL_AUTO_LABEL() , c::Bounds<2>(d0,d1) , KOKKOS_LAMBDA (int i, int j) {
          out(j,i) = in(i,j);
        });
        return out;
      }
    }

    template <class T, int myMem>
    inline Array<T,2,myMem,styleFortran> transpose(Array<T,2,myMem,styleFortran> const &in ) {
      #ifdef KOKKOS_ENABLE_DEBUG
        if (!allocated(in)) Kokkos::abort("ERROR: Calling transpose on unallocated array");
      #endif
      auto l1 = lbound(in,1);
      auto l2 = lbound(in,2);
      auto u1 = ubound(in,1);
      auto u2 = ubound(in,2);
      if constexpr (myMem == memHost) {
        auto out = in.createHostCopy().template reshape<2>( { {l2,u2} , {l1,u1} } );
        for (int i=l1; i <= u1; i++) {
          for (int j=l2; j <= u2; j++) {
            out(j,i) = in(i,j);
          }
        }
        return out;
      } else {
        auto out = in.createDeviceCopy().template reshape<2>( { {l2,u2} , {l1,u1} } );
        fortran::parallel_for( YAKL_AUTO_LABEL() , fortran::Bounds<2>({l1,u1},{l2,u2}) , KOKKOS_LAMBDA (int i, int j) {
          out(j,i) = in(i,j);
        });
        return out;
      }
    }

    template <size_t n1, size_t n2, class T>
    KOKKOS_INLINE_FUNCTION SArray<T,2,n2,n1> transpose(SArray<T,2,n1,n2> const &a) {
      SArray<T,2,n2,n1> ret;
      for (int j=0; j < n1; j++) {
        for (int i=0; i < n2; i++) {
          ret(j,i) = a(i,j);
        }
      }
      return ret;
    }

    template <class B1, class B2, class T>
    KOKKOS_INLINE_FUNCTION FSArray<T,2,B1,B2> transpose(FSArray<T,2,B1,B2> const &a) {
      FSArray<T,2,B2,B1> ret;
      for (int j=B1::lower(); j <= B1::upper(); j++) {
        for (int i=B2::lower(); i <= B2::upper(); i++) {
          ret(j,i) = a(i,j);
        }
      }
      return ret;
    }

  }
}

