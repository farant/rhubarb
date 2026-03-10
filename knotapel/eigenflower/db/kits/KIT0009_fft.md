---
id: KIT0009
title: "FFT library (Accelerate vDSP wrapper)"
status: planned
tier: 1
input: "Real or complex sequence"
output: "Frequency-domain coefficients"
depends_on: []
effort: small
source: []
applicable_to: [G0004]
related: [KIT0010, KIT0004]
---

## Description

C89 wrapper around Apple Accelerate's vDSP FFT routines. Multiple higher-tier KITs need frequency-domain operations: spectral reasoning operators, scattering transforms, and potentially the kernel ridge regression solver (for fast kernel evaluation via FFT).

## Operations needed

- Forward FFT (real to complex)
- Inverse FFT (complex to real)
- Power spectrum
- Convolution via FFT (multiply in frequency domain)

## Why wrap vDSP

vDSP uses the M2's NEON/AMX hardware for vectorized FFT. Already available on macOS, no dependencies. The vDSP API uses a split-complex format that is non-obvious -- the wrapper provides a simpler interface.

## API Sketch

```c
vacuum fft_directum(duplex *input, duplex *re_out, duplex *im_out, longus n);
vacuum fft_inversum(duplex *re_in, duplex *im_in, duplex *output, longus n);
vacuum fft_convolutio(duplex *a, duplex *b, duplex *out, longus n);
```

~100 LOC wrapper + test suite.
