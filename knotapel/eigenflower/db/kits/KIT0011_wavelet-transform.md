---
id: KIT0011
title: "Wavelet transform library"
status: planned
tier: 1
input: "Signal, wavelet type, scale parameters"
output: "Wavelet coefficients at all scales"
depends_on: [KIT0009]
effort: medium
source: []
applicable_to: [G0010]
related: [KIT0010]
---

## Description

Discrete wavelet transform implementation for C89. The scattering transform (KIT0010) is built on cascading wavelet convolutions, so this is a prerequisite.

## Operations needed

- Forward DWT (decompose signal into wavelet coefficients at multiple scales)
- Inverse DWT (reconstruct signal from coefficients)
- Wavelet function generation (Morlet, Mexican hat, Haar)
- Convolution with wavelet at specified scale

Can use FFT-based convolution (via KIT0009) for efficiency at large scales, or direct convolution for small scales.

## Wavelet types

- **Morlet**: good for frequency analysis, standard in scattering transforms
- **Haar**: simplest, good for discrete/binary signals (potentially useful for Boolean tensor features)
- **Mexican hat**: good for edge detection in 1D signals

~400 LOC including multiple wavelet types.
