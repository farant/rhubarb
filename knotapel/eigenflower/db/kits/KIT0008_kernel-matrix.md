---
id: KIT0008
title: "Kernel matrix constructor"
status: planned
tier: 1
input: "Data points, kernel function choice, kernel parameters"
output: "Gram matrix K[i,j] = k(x_i, x_j)"
depends_on: []
effort: small
source: ["Hilbert Space paper (Kiruluta et al., 2025)"]
applicable_to: [G0005]
related: [KIT0004, KIT0007]
---

## Description

Constructs the kernel (Gram) matrix from a set of data points and a kernel function. This is the input to kernel ridge regression (KIT0004).

## Kernel functions needed

- **RBF (Gaussian)**: k(x, y) = exp(-||x - y||^2 / (2 * sigma^2)). Universal approximator.
- **Polynomial**: k(x, y) = (x^T y + c)^d. Natural for logical structure (degree corresponds to interaction order).
- **Linear**: k(x, y) = x^T y. Simplest case, equivalent to ordinary least squares.

## Properties

- K is always symmetric positive semi-definite
- K + lambda*I is always symmetric positive definite (for lambda > 0), so Cholesky is safe
- For n = 3,500 (Bible + Summa entities), K is 3,500 x 3,500 = ~98 MB at double precision. Fits easily in memory.
- Construction is O(n^2 * d) where d is embedding dimension

## API Sketch

```c
nomen vacuum (*functio_nuclei)(duplex *x, duplex *y, longus dim, vacuum *param);
vacuum nucleum_construere(duplex *X, longus n, longus dim, functio_nuclei k, vacuum *param, duplex *K);
```

~150 LOC + kernel functions.
