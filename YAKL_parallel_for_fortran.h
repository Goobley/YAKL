
#pragma once



namespace fortran {

  template <class T> constexpr T fastmod(T a , T b) {
    return a < b ? a : a-b*(a/b);
  }



  class LBnd {
  public:
    int l, u, s;
    LBnd(int u) {
      this->l = 1;
      this->u = u;
      this->s = 1;
    }
    LBnd(int l, int u) {
      this->l = l;
      this->u = u;
      this->s = 1;
      if (u < l) yakl_throw("ERROR: cannot specify an upper bound < lower bound");
    }
    LBnd(int l, int u, int s) {
      this->l = l;
      this->u = u;
      this->s = s;
      if (s < 1) yakl_throw("ERROR: negative strides not yet supported.");
    }
  };



  // Bounds with simple set to true have no lower bounds or strides specified
  // This saves on register space on accelerators
  template <int N, bool simple = false> class Bounds;



  template<> class Bounds<8,false> {
  public:
    index_t nIter;
    int     lbounds[8];
    index_t dims[8];
    index_t strides[8];
    Bounds( LBnd const &b0 , LBnd const &b1 , LBnd const &b2 , LBnd const &b3 , LBnd const &b4 , LBnd const &b5 , LBnd const &b6 , LBnd const &b7 ) {
      lbounds[0] = b0.l;   strides[0] =  b0.s;   dims[0] = ( b0.u - b0.l + 1 ) / b0.s;
      lbounds[1] = b1.l;   strides[1] =  b1.s;   dims[1] = ( b1.u - b1.l + 1 ) / b1.s;
      lbounds[2] = b2.l;   strides[2] =  b2.s;   dims[2] = ( b2.u - b2.l + 1 ) / b2.s;
      lbounds[3] = b3.l;   strides[3] =  b3.s;   dims[3] = ( b3.u - b3.l + 1 ) / b3.s;
      lbounds[4] = b4.l;   strides[4] =  b4.s;   dims[4] = ( b4.u - b4.l + 1 ) / b4.s;
      lbounds[5] = b5.l;   strides[5] =  b5.s;   dims[5] = ( b5.u - b5.l + 1 ) / b5.s;
      lbounds[6] = b6.l;   strides[6] =  b6.s;   dims[6] = ( b6.u - b6.l + 1 ) / b6.s;
      lbounds[7] = b7.l;   strides[7] =  b7.s;   dims[7] = ( b7.u - b7.l + 1 ) / b7.s;
      nIter = 1;
      for (int i=0; i<8; i++) { nIter *= dims[i]; }
    }
    YAKL_INLINE void unpackIndices( index_t iGlob , int indices[8] ) const {
      // Compute base indices
      index_t fac   ; indices[7] = fastmod( (iGlob    ) , dims[7] );
      fac  = dims[7]; indices[6] = fastmod( (iGlob/fac) , dims[6] );
      fac *= dims[6]; indices[5] = fastmod( (iGlob/fac) , dims[5] );
      fac *= dims[5]; indices[4] = fastmod( (iGlob/fac) , dims[4] );
      fac *= dims[4]; indices[3] = fastmod( (iGlob/fac) , dims[3] );
      fac *= dims[3]; indices[2] = fastmod( (iGlob/fac) , dims[2] );
      fac *= dims[2]; indices[1] = fastmod( (iGlob/fac) , dims[1] );
      fac *= dims[1]; indices[0] =          (iGlob/fac)            ;
      // Apply strides and lower bounds
      indices[0] = indices[0]*strides[0] + lbounds[0];
      indices[1] = indices[1]*strides[1] + lbounds[1];
      indices[2] = indices[2]*strides[2] + lbounds[2];
      indices[3] = indices[3]*strides[3] + lbounds[3];
      indices[4] = indices[4]*strides[4] + lbounds[4];
      indices[5] = indices[5]*strides[5] + lbounds[5];
      indices[6] = indices[6]*strides[6] + lbounds[6];
      indices[7] = indices[7]*strides[7] + lbounds[7];
    }
  };

  template<> class Bounds<8,true> {
  public:
    index_t nIter;
    index_t dims[8];
    Bounds( index_t b0 , index_t b1 , index_t b2 , index_t b3 , index_t b4 , index_t b5 , index_t b6 , index_t b7 ) {
      dims[0] = b0;
      dims[1] = b1;
      dims[2] = b2;
      dims[3] = b3;
      dims[4] = b4;
      dims[5] = b5;
      dims[6] = b6;
      dims[7] = b7;
      nIter = 1;
      for (int i=0; i<8; i++) { nIter *= dims[i]; }
    }
    YAKL_INLINE void unpackIndices( index_t iGlob , int indices[8] ) const {
      // Compute base indices
      index_t fac   ; indices[7] = fastmod( (iGlob    ) , dims[7] ) + 1;
      fac  = dims[7]; indices[6] = fastmod( (iGlob/fac) , dims[6] ) + 1;
      fac *= dims[6]; indices[5] = fastmod( (iGlob/fac) , dims[5] ) + 1;
      fac *= dims[5]; indices[4] = fastmod( (iGlob/fac) , dims[4] ) + 1;
      fac *= dims[4]; indices[3] = fastmod( (iGlob/fac) , dims[3] ) + 1;
      fac *= dims[3]; indices[2] = fastmod( (iGlob/fac) , dims[2] ) + 1;
      fac *= dims[2]; indices[1] = fastmod( (iGlob/fac) , dims[1] ) + 1;
      fac *= dims[1]; indices[0] =          (iGlob/fac)             + 1;
    }
  };



  template<> class Bounds<7,false> {
  public:
    index_t nIter;
    int     lbounds[7];
    index_t dims[7];
    index_t strides[7];
    Bounds( LBnd const &b0 , LBnd const &b1 , LBnd const &b2 , LBnd const &b3 , LBnd const &b4 , LBnd const &b5 , LBnd const &b6 ) {
      lbounds[0] = b0.l;   strides[0] =  b0.s;   dims[0] = ( b0.u - b0.l + 1 ) / b0.s;
      lbounds[1] = b1.l;   strides[1] =  b1.s;   dims[1] = ( b1.u - b1.l + 1 ) / b1.s;
      lbounds[2] = b2.l;   strides[2] =  b2.s;   dims[2] = ( b2.u - b2.l + 1 ) / b2.s;
      lbounds[3] = b3.l;   strides[3] =  b3.s;   dims[3] = ( b3.u - b3.l + 1 ) / b3.s;
      lbounds[4] = b4.l;   strides[4] =  b4.s;   dims[4] = ( b4.u - b4.l + 1 ) / b4.s;
      lbounds[5] = b5.l;   strides[5] =  b5.s;   dims[5] = ( b5.u - b5.l + 1 ) / b5.s;
      lbounds[6] = b6.l;   strides[6] =  b6.s;   dims[6] = ( b6.u - b6.l + 1 ) / b6.s;
      nIter = 1;
      for (int i=0; i<7; i++) { nIter *= dims[i]; }
    }
    YAKL_INLINE void unpackIndices( index_t iGlob , int indices[7] ) const {
      // Compute base indices
      index_t fac   ; indices[6] = fastmod( (iGlob    ) , dims[6] );
      fac  = dims[6]; indices[5] = fastmod( (iGlob/fac) , dims[5] );
      fac *= dims[5]; indices[4] = fastmod( (iGlob/fac) , dims[4] );
      fac *= dims[4]; indices[3] = fastmod( (iGlob/fac) , dims[3] );
      fac *= dims[3]; indices[2] = fastmod( (iGlob/fac) , dims[2] );
      fac *= dims[2]; indices[1] = fastmod( (iGlob/fac) , dims[1] );
      fac *= dims[1]; indices[0] =          (iGlob/fac)            ;
      // Apply strides and lower bounds
      indices[0] = indices[0]*strides[0] + lbounds[0];
      indices[1] = indices[1]*strides[1] + lbounds[1];
      indices[2] = indices[2]*strides[2] + lbounds[2];
      indices[3] = indices[3]*strides[3] + lbounds[3];
      indices[4] = indices[4]*strides[4] + lbounds[4];
      indices[5] = indices[5]*strides[5] + lbounds[5];
      indices[6] = indices[6]*strides[6] + lbounds[6];
    }
  };

  template<> class Bounds<7,true> {
  public:
    index_t nIter;
    index_t dims[7];
    Bounds( index_t b0 , index_t b1 , index_t b2 , index_t b3 , index_t b4 , index_t b5 , index_t b6 ) {
      dims[0] = b0;
      dims[1] = b1;
      dims[2] = b2;
      dims[3] = b3;
      dims[4] = b4;
      dims[5] = b5;
      dims[6] = b6;
      nIter = 1;
      for (int i=0; i<7; i++) { nIter *= dims[i]; }
    }
    YAKL_INLINE void unpackIndices( index_t iGlob , int indices[7] ) const {
      // Compute base indices
      index_t fac   ; indices[6] = fastmod( (iGlob    ) , dims[6] ) + 1;
      fac  = dims[6]; indices[5] = fastmod( (iGlob/fac) , dims[5] ) + 1;
      fac *= dims[5]; indices[4] = fastmod( (iGlob/fac) , dims[4] ) + 1;
      fac *= dims[4]; indices[3] = fastmod( (iGlob/fac) , dims[3] ) + 1;
      fac *= dims[3]; indices[2] = fastmod( (iGlob/fac) , dims[2] ) + 1;
      fac *= dims[2]; indices[1] = fastmod( (iGlob/fac) , dims[1] ) + 1;
      fac *= dims[1]; indices[0] =          (iGlob/fac)             + 1;
    }
  };



  template<> class Bounds<6,false> {
  public:
    index_t nIter;
    int     lbounds[6];
    index_t dims[6];
    index_t strides[6];
    Bounds( LBnd const &b0 , LBnd const &b1 , LBnd const &b2 , LBnd const &b3 , LBnd const &b4 , LBnd const &b5 ) {
      lbounds[0] = b0.l;   strides[0] =  b0.s;   dims[0] = ( b0.u - b0.l + 1 ) / b0.s;
      lbounds[1] = b1.l;   strides[1] =  b1.s;   dims[1] = ( b1.u - b1.l + 1 ) / b1.s;
      lbounds[2] = b2.l;   strides[2] =  b2.s;   dims[2] = ( b2.u - b2.l + 1 ) / b2.s;
      lbounds[3] = b3.l;   strides[3] =  b3.s;   dims[3] = ( b3.u - b3.l + 1 ) / b3.s;
      lbounds[4] = b4.l;   strides[4] =  b4.s;   dims[4] = ( b4.u - b4.l + 1 ) / b4.s;
      lbounds[5] = b5.l;   strides[5] =  b5.s;   dims[5] = ( b5.u - b5.l + 1 ) / b5.s;
      nIter = 1;
      for (int i=0; i<6; i++) { nIter *= dims[i]; }
    }
    YAKL_INLINE void unpackIndices( index_t iGlob , int indices[6] ) const {
      // Compute base indices
      index_t fac   ; indices[5] = fastmod( (iGlob    ) , dims[5] );
      fac  = dims[5]; indices[4] = fastmod( (iGlob/fac) , dims[4] );
      fac *= dims[4]; indices[3] = fastmod( (iGlob/fac) , dims[3] );
      fac *= dims[3]; indices[2] = fastmod( (iGlob/fac) , dims[2] );
      fac *= dims[2]; indices[1] = fastmod( (iGlob/fac) , dims[1] );
      fac *= dims[1]; indices[0] =          (iGlob/fac)            ;
      // Apply strides and lower bounds
      indices[0] = indices[0]*strides[0] + lbounds[0];
      indices[1] = indices[1]*strides[1] + lbounds[1];
      indices[2] = indices[2]*strides[2] + lbounds[2];
      indices[3] = indices[3]*strides[3] + lbounds[3];
      indices[4] = indices[4]*strides[4] + lbounds[4];
      indices[5] = indices[5]*strides[5] + lbounds[5];
    }
  };

  template<> class Bounds<6,true> {
  public:
    index_t nIter;
    index_t dims[6];
    Bounds( index_t b0 , index_t b1 , index_t b2 , index_t b3 , index_t b4 , index_t b5 ) {
      dims[0] = b0;
      dims[1] = b1;
      dims[2] = b2;
      dims[3] = b3;
      dims[4] = b4;
      dims[5] = b5;
      nIter = 1;
      for (int i=0; i<6; i++) { nIter *= dims[i]; }
    }
    YAKL_INLINE void unpackIndices( index_t iGlob , int indices[6] ) const {
      // Compute base indices
      index_t fac   ; indices[5] = fastmod( (iGlob    ) , dims[5] ) + 1;
      fac  = dims[5]; indices[4] = fastmod( (iGlob/fac) , dims[4] ) + 1;
      fac *= dims[4]; indices[3] = fastmod( (iGlob/fac) , dims[3] ) + 1;
      fac *= dims[3]; indices[2] = fastmod( (iGlob/fac) , dims[2] ) + 1;
      fac *= dims[2]; indices[1] = fastmod( (iGlob/fac) , dims[1] ) + 1;
      fac *= dims[1]; indices[0] =          (iGlob/fac)             + 1;
    }
  };



  template<> class Bounds<5,false> {
  public:
    index_t nIter;
    int     lbounds[5];
    index_t dims[5];
    index_t strides[5];
    Bounds( LBnd const &b0 , LBnd const &b1 , LBnd const &b2 , LBnd const &b3 , LBnd const &b4 ) {
      lbounds[0] = b0.l;   strides[0] =  b0.s;   dims[0] = ( b0.u - b0.l + 1 ) / b0.s;
      lbounds[1] = b1.l;   strides[1] =  b1.s;   dims[1] = ( b1.u - b1.l + 1 ) / b1.s;
      lbounds[2] = b2.l;   strides[2] =  b2.s;   dims[2] = ( b2.u - b2.l + 1 ) / b2.s;
      lbounds[3] = b3.l;   strides[3] =  b3.s;   dims[3] = ( b3.u - b3.l + 1 ) / b3.s;
      lbounds[4] = b4.l;   strides[4] =  b4.s;   dims[4] = ( b4.u - b4.l + 1 ) / b4.s;
      nIter = 1;
      for (int i=0; i<5; i++) { nIter *= dims[i]; }
    }
    YAKL_INLINE void unpackIndices( index_t iGlob , int indices[5] ) const {
      // Compute base indices
      index_t fac   ; indices[4] = fastmod( (iGlob    ) , dims[4] );
      fac  = dims[4]; indices[3] = fastmod( (iGlob/fac) , dims[3] );
      fac *= dims[3]; indices[2] = fastmod( (iGlob/fac) , dims[2] );
      fac *= dims[2]; indices[1] = fastmod( (iGlob/fac) , dims[1] );
      fac *= dims[1]; indices[0] =          (iGlob/fac)            ;
      // Apply strides and lower bounds
      indices[0] = indices[0]*strides[0] + lbounds[0];
      indices[1] = indices[1]*strides[1] + lbounds[1];
      indices[2] = indices[2]*strides[2] + lbounds[2];
      indices[3] = indices[3]*strides[3] + lbounds[3];
      indices[4] = indices[4]*strides[4] + lbounds[4];
    }
  };

  template<> class Bounds<5,true> {
  public:
    index_t nIter;
    index_t dims[5];
    Bounds( index_t b0 , index_t b1 , index_t b2 , index_t b3 , index_t b4 ) {
      dims[0] = b0;
      dims[1] = b1;
      dims[2] = b2;
      dims[3] = b3;
      dims[4] = b4;
      nIter = 1;
      for (int i=0; i<5; i++) { nIter *= dims[i]; }
    }
    YAKL_INLINE void unpackIndices( index_t iGlob , int indices[5] ) const {
      // Compute base indices
      index_t fac   ; indices[4] = fastmod( (iGlob    ) , dims[4] ) + 1;
      fac  = dims[4]; indices[3] = fastmod( (iGlob/fac) , dims[3] ) + 1;
      fac *= dims[3]; indices[2] = fastmod( (iGlob/fac) , dims[2] ) + 1;
      fac *= dims[2]; indices[1] = fastmod( (iGlob/fac) , dims[1] ) + 1;
      fac *= dims[1]; indices[0] =          (iGlob/fac)             + 1;
    }
  };



  template<> class Bounds<4,false> {
  public:
    index_t nIter;
    int     lbounds[4];
    index_t dims[4];
    index_t strides[4];
    Bounds( LBnd const &b0 , LBnd const &b1 , LBnd const &b2 , LBnd const &b3 ) {
      lbounds[0] = b0.l;   strides[0] =  b0.s;   dims[0] = ( b0.u - b0.l + 1 ) / b0.s;
      lbounds[1] = b1.l;   strides[1] =  b1.s;   dims[1] = ( b1.u - b1.l + 1 ) / b1.s;
      lbounds[2] = b2.l;   strides[2] =  b2.s;   dims[2] = ( b2.u - b2.l + 1 ) / b2.s;
      lbounds[3] = b3.l;   strides[3] =  b3.s;   dims[3] = ( b3.u - b3.l + 1 ) / b3.s;
      nIter = 1;
      for (int i=0; i<4; i++) { nIter *= dims[i]; }
    }
    YAKL_INLINE void unpackIndices( index_t iGlob , int indices[4] ) const {
      // Compute base indices
      index_t fac   ; indices[3] = fastmod( (iGlob    ) , dims[3] );
      fac  = dims[3]; indices[2] = fastmod( (iGlob/fac) , dims[2] );
      fac *= dims[2]; indices[1] = fastmod( (iGlob/fac) , dims[1] );
      fac *= dims[1]; indices[0] =          (iGlob/fac)            ;
      // Apply strides and lower bounds
      indices[0] = indices[0]*strides[0] + lbounds[0];
      indices[1] = indices[1]*strides[1] + lbounds[1];
      indices[2] = indices[2]*strides[2] + lbounds[2];
      indices[3] = indices[3]*strides[3] + lbounds[3];
    }
  };

  template<> class Bounds<4,true> {
  public:
    index_t nIter;
    index_t dims[4];
    Bounds( index_t b0 , index_t b1 , index_t b2 , index_t b3 ) {
      dims[0] = b0;
      dims[1] = b1;
      dims[2] = b2;
      dims[3] = b3;
      nIter = 1;
      for (int i=0; i<4; i++) { nIter *= dims[i]; }
    }
    YAKL_INLINE void unpackIndices( index_t iGlob , int indices[4] ) const {
      // Compute base indices
      index_t fac   ; indices[3] = fastmod( (iGlob    ) , dims[3] ) + 1;
      fac  = dims[3]; indices[2] = fastmod( (iGlob/fac) , dims[2] ) + 1;
      fac *= dims[2]; indices[1] = fastmod( (iGlob/fac) , dims[1] ) + 1;
      fac *= dims[1]; indices[0] =          (iGlob/fac)             + 1;
    }
  };



  template<> class Bounds<3,false> {
  public:
    index_t nIter;
    int     lbounds[3];
    index_t dims[3];
    index_t strides[3];
    Bounds( LBnd const &b0 , LBnd const &b1 , LBnd const &b2 ) {
      lbounds[0] = b0.l;   strides[0] =  b0.s;   dims[0] = ( b0.u - b0.l + 1 ) / b0.s;
      lbounds[1] = b1.l;   strides[1] =  b1.s;   dims[1] = ( b1.u - b1.l + 1 ) / b1.s;
      lbounds[2] = b2.l;   strides[2] =  b2.s;   dims[2] = ( b2.u - b2.l + 1 ) / b2.s;
      nIter = 1;
      for (int i=0; i<3; i++) { nIter *= dims[i]; }
    }
    YAKL_INLINE void unpackIndices( index_t iGlob , int indices[3] ) const {
      // Compute base indices
      index_t fac   ; indices[2] = fastmod( (iGlob    ) , dims[2] );
      fac  = dims[2]; indices[1] = fastmod( (iGlob/fac) , dims[1] );
      fac *= dims[1]; indices[0] =          (iGlob/fac)            ;
      // Apply strides and lower bounds
      indices[0] = indices[0]*strides[0] + lbounds[0];
      indices[1] = indices[1]*strides[1] + lbounds[1];
      indices[2] = indices[2]*strides[2] + lbounds[2];
    }
  };

  template<> class Bounds<3,true> {
  public:
    index_t nIter;
    index_t dims[3];
    Bounds( index_t b0 , index_t b1 , index_t b2 ) {
      dims[0] = b0;
      dims[1] = b1;
      dims[2] = b2;
      nIter = 1;
      for (int i=0; i<3; i++) { nIter *= dims[i]; }
    }
    YAKL_INLINE void unpackIndices( index_t iGlob , int indices[3] ) const {
      // Compute base indices
      index_t fac   ; indices[2] = fastmod( (iGlob    ) , dims[2] ) + 1;
      fac  = dims[2]; indices[1] = fastmod( (iGlob/fac) , dims[1] ) + 1;
      fac *= dims[1]; indices[0] =          (iGlob/fac)             + 1;
    }
  };



  template<> class Bounds<2,false> {
  public:
    index_t nIter;
    int     lbounds[2];
    index_t dims[2];
    index_t strides[2];
    Bounds( LBnd const &b0 , LBnd const &b1 ) {
      lbounds[0] = b0.l;   strides[0] =  b0.s;   dims[0] = ( b0.u - b0.l + 1 ) / b0.s;
      lbounds[1] = b1.l;   strides[1] =  b1.s;   dims[1] = ( b1.u - b1.l + 1 ) / b1.s;
      nIter = 1;
      for (int i=0; i<2; i++) { nIter *= dims[i]; }
    }
    YAKL_INLINE void unpackIndices( index_t iGlob , int indices[2] ) const {
      // Compute base indices
      indices[1] = fastmod( (iGlob        ) , dims[1] );
      indices[0] =          (iGlob/dims[1])            ;
      // Apply strides and lower bounds
      indices[0] = indices[0]*strides[0] + lbounds[0];
      indices[1] = indices[1]*strides[1] + lbounds[1];
    }
  };

  template<> class Bounds<2,true> {
  public:
    index_t nIter;
    int     lbounds[2];
    index_t dims[2];
    index_t strides[2];
    Bounds( index_t b0 , index_t b1 ) {
      dims[0] = b0;
      dims[1] = b1;
      nIter = 1;
      for (int i=0; i<2; i++) { nIter *= dims[i]; }
    }
    YAKL_INLINE void unpackIndices( index_t iGlob , int indices[2] ) const {
      // Compute base indices
      indices[1] = fastmod( (iGlob        ) , dims[1] ) + 1;
      indices[0] =          (iGlob/dims[1])             + 1;
    }
  };



  template<> class Bounds<1,false> {
  public:
    index_t nIter;
    int     lbounds[1];
    index_t dims[1];
    index_t strides[1];
    Bounds( LBnd const &b0 ) {
      lbounds[0] = b0.l;   strides[0] =  b0.s;   dims[0] = ( b0.u - b0.l + 1 ) / b0.s;
      nIter = dims[0];
    }
    YAKL_INLINE void unpackIndices( index_t iGlob , int indices[1] ) const {
      // Compute base indices
      indices[0] = iGlob;
      // Apply strides and lower bounds
      indices[0] = indices[0]*strides[0] + lbounds[0];
    }
  };

  template<> class Bounds<1,true> {
  public:
    index_t nIter;
    index_t dims[1];
    Bounds( index_t b0 ) {
      dims[0] = b0;
      nIter = dims[0];
    }
    YAKL_INLINE void unpackIndices( index_t iGlob , int indices[1] ) const {
      // Compute base indices
      indices[0] = iGlob + 1;
    }
  };



  template <int N> using SimpleBounds = Bounds<N,true>;



  template <class F, bool simple> YAKL_INLINE void callFunctor(F const &f , Bounds<1,simple> const &bnd , int const i ) {
    int ind[1];
    bnd.unpackIndices( i , ind );
    f(ind[0]);
  }
  template <class F, bool simple> YAKL_INLINE void callFunctor(F const &f , Bounds<2,simple> const &bnd , int const i ) {
    int ind[2];
    bnd.unpackIndices( i , ind );
    f(ind[0],ind[1]);
  }
  template <class F, bool simple> YAKL_INLINE void callFunctor(F const &f , Bounds<3,simple> const &bnd , int const i ) {
    int ind[3];
    bnd.unpackIndices( i , ind );
    f(ind[0],ind[1],ind[2]);
  }
  template <class F, bool simple> YAKL_INLINE void callFunctor(F const &f , Bounds<4,simple> const &bnd , int const i ) {
    int ind[4];
    bnd.unpackIndices( i , ind );
    f(ind[0],ind[1],ind[2],ind[3]);
  }
  template <class F, bool simple> YAKL_INLINE void callFunctor(F const &f , Bounds<5,simple> const &bnd , int const i ) {
    int ind[5];
    bnd.unpackIndices( i , ind );
    f(ind[0],ind[1],ind[2],ind[3],ind[4]);
  }
  template <class F, bool simple> YAKL_INLINE void callFunctor(F const &f , Bounds<6,simple> const &bnd , int const i ) {
    int ind[6];
    bnd.unpackIndices( i , ind );
    f(ind[0],ind[1],ind[2],ind[3],ind[4],ind[5]);
  }
  template <class F, bool simple> YAKL_INLINE void callFunctor(F const &f , Bounds<7,simple> const &bnd , int const i ) {
    int ind[7];
    bnd.unpackIndices( i , ind );
    f(ind[0],ind[1],ind[2],ind[3],ind[4],ind[5],ind[6]);
  }
  template <class F, bool simple> YAKL_INLINE void callFunctor(F const &f , Bounds<8,simple> const &bnd , int const i ) {
    int ind[8];
    bnd.unpackIndices( i , ind );
    f(ind[0],ind[1],ind[2],ind[3],ind[4],ind[5],ind[6],ind[7]);
  }



  #ifdef __USE_CUDA__
    template <class F, int N, bool simple> __global__ void cudaKernelVal( Bounds<N,simple> bounds , F f ) {
      size_t i = blockIdx.x*blockDim.x + threadIdx.x;
      if (i < bounds.nIter) {
        callFunctor( f , bounds , i );
      }
    }

    template <class F, int N, bool simple> __global__ void cudaKernelRef( Bounds<N,simple> bounds , F const &f ) {
      size_t i = blockIdx.x*blockDim.x + threadIdx.x;
      if (i < bounds.nIter) {
        callFunctor( f , bounds , i );
      }
    }

    template<class F , int N , bool simple , typename std::enable_if< sizeof(F) <= 4000 , int >::type = 0>
    void parallel_for_cuda( Bounds<N,simple> const &bounds , F const &f , int vectorSize = 128 ) {
      cudaKernelVal <<< (unsigned int) (bounds.nIter-1)/vectorSize+1 , vectorSize >>> ( bounds , f );
    }

    template<class F , int N , bool simple , typename std::enable_if< sizeof(F) >= 4001 , int >::type = 0>
    void parallel_for_cuda( Bounds<N,simple> const &bounds , F const &f , int vectorSize = 128 ) {
      F *fp = (F *) functorBuffer;
      cudaMemcpyAsync(fp,&f,sizeof(F),cudaMemcpyHostToDevice);
      cudaKernelRef <<< (unsigned int) (bounds.nIter-1)/vectorSize+1 , vectorSize >>> ( bounds , *fp );
    }
  #endif



  #ifdef __USE_HIP__
    template <class F, int N, bool simple> __global__ void hipKernel( Bounds<N,simple> bounds , F f ) {
      size_t i = blockIdx.x*blockDim.x + threadIdx.x;
      if (i < bounds.nIter) {
        callFunctor( f , bounds , i );
      }
    }

    template<class F, int N, bool simple>
    void parallel_for_hip( Bounds<N,simple> const &bounds , F const &f , int vectorSize = 128 ) {
      hipLaunchKernelGGL( hipKernel , dim3((bounds.nIter-1)/vectorSize+1) , dim3(vectorSize) ,
                          (std::uint32_t) 0 , (hipStream_t) 0 , bounds , f );
    }
  #endif


  template<class F, int N, bool simple>
  void parallel_for_cpu_serial( Bounds<N,simple> const &bounds , F const &f ) {
    for (index_t i = 0 ; i < bounds.nIter ; i++) {
      callFunctor( f , bounds , i );
    }
  }


  template <class F, int N, bool simple>
  inline void parallel_for( Bounds<N,simple> const &bounds , F const &f , int vectorSize = 128 ) {
    #ifdef __USE_CUDA__
      parallel_for_cuda( bounds , f , vectorSize );
    #elif defined(__USE_HIP__)
      parallel_for_hip ( bounds , f , vectorSize );
    #else
      parallel_for_cpu_serial( bounds , f );
    #endif

    #if defined(__AUTO_FENCE__)
      fence();
    #endif
  }



  template <class F, int N, bool simple>
  inline void parallel_for( char const * str , Bounds<N,simple> const &bounds , F const &f, int vectorSize = 128 ) {
    parallel_for( bounds , f , vectorSize );
  }


  template <class F> inline void parallel_for( LBnd &bnd , F const &f , int vectorSize = 128 ) {
    parallel_for( Bounds<1,false>(bnd) , f , vectorSize );
  }


  template <class F> inline void parallel_for( char const * str , LBnd &bnd , F const &f , int vectorSize = 128 ) {
    parallel_for( Bounds<1,false>(bnd) , f , vectorSize );
  }


  template <class F, class T, typename std::enable_if< std::is_integral<T>::value , int >::type = 0 >
  inline void parallel_for( T bnd , F const &f , int vectorSize = 128 ) {
    parallel_for( Bounds<1,true>(bnd) , f , vectorSize );
  }


  template <class F, class T, typename std::enable_if< std::is_integral<T>::value , int >::type = 0 >
  inline void parallel_for( char const * str , T bnd , F const &f , int vectorSize = 128 ) {
    parallel_for( Bounds<1,true>(bnd) , f , vectorSize );
  }



}
