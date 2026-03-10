---
id: KIT0007
title: "Matrix operations library (Accelerate wrapper)"
status: planned
tier: 1
input: "n x n matrix A, vector/matrix b"
output: "Solution x to Ax = b, determinant, inverse, multiply"
depends_on: []
effort: small
source: []
applicable_to: [G0004, G0005]
related: [KIT0004, KIT0005, KIT0006, KIT0009]
---

## Description

C89 wrapper around Apple Accelerate's LAPACK and BLAS routines for the matrix operations eigenflower needs. This is the foundational math library that most higher-tier KITs depend on.

## Operations needed

- Matrix multiply: cblas_dgemm
- Matrix-vector multiply: cblas_dgemv
- LU decomposition + solve: dgesv_
- Cholesky decomposition + solve: dpotrf_ + dpotrs_ (for symmetric positive definite, which K + lambda*I always is)
- Determinant (from LU pivots)
- Gram-Schmidt orthogonalization (for CVP)
- Identity matrix construction
- Matrix copy, transpose, scale

## Why wrap Accelerate

Apple Accelerate is already on every Mac, uses the AMX coprocessor on M2 for hardware-accelerated matrix ops, and has a C API. No dependencies to install. The wrapper provides a clean C89 API that hides the Fortran-style LAPACK calling conventions.

## API Sketch

```c
vacuum mat_multiplica(duplex *A, duplex *B, duplex *C, longus m, longus n, longus k);
integer mat_solve(duplex *A, duplex *b, longus n);
integer mat_cholesky_solve(duplex *A, duplex *b, longus n);
vacuum mat_gram_schmidt(duplex *basis, duplex *ortho, longus n, longus dim);
```

~200 LOC. Straightforward wrapping.
