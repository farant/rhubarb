---
id: PT0071
title: "Fourier spectral filtering as lightweight attention alternative"
status: untested
source: ["Hilbert Space paper (Kiruluta et al., 2025)"]
applicable_to: []
related: [PT0013, PT0041]
---

Apply learnable multiplicative weights gamma_k to Fourier coefficients of the input sequence. This defines a diagonal operator in Fourier domain = convolution in spatial domain. Extremely efficient via FFT. Very few parameters per "layer" (one gamma_k per frequency component).

Learnable spectral models with differentiable soft-thresholding: f_k^out = sigma_{theta_k}(|f_k|) * f_k where sigma_theta(z) = z / (z + theta). This is a tunable band-pass filter that adapts per frequency component.

For eigenflower: an extremely lightweight alternative to attention. Each "attention layer" is just a set of learnable frequency weights. Combined with FFT (O(n log n)), this could be faster than both standard attention (O(n^2)) and Orchid convolution (O(n log n) with higher constant).

The tradeoff: simpler than Orchid (no data-dependent conditioning), which means less expressiveness. May work well for the fluency layer's simpler tasks (parsing, generation) but not for complex reasoning.

Apple Accelerate vDSP provides optimized FFT routines directly usable from C89.
