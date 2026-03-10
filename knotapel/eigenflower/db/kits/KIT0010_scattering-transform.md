---
id: KIT0010
title: "Scattering transform feature extractor"
status: planned
tier: 2
input: "Signal (text as byte sequence or embedding sequence)"
output: "Scattering coefficients (fixed, no training parameters)"
depends_on: [KIT0009, KIT0011]
effort: medium
source: ["Hilbert Space paper (Kiruluta et al., 2025)"]
migrated_from: PT0014
applicable_to: [G0010]
related: [KIT0004, KIT0002]
---

## Description

Cascading wavelet transforms with modulus nonlinearities produces translation-invariant, deformation-stable features WITHOUT any learned parameters. Achieved 98.6% on CUReT texture classification with just a linear SVM on top. Competitive with deep CNNs on TIMIT speech recognition with far fewer parameters.

This is a feature extraction method that costs zero training time. The scattering transform naturally operates at the edge of chaos -- it captures structure at all scales while being stable to perturbations.

For eigenflower: use scattering as a fixed feature extractor for text, then apply kernel methods for the reasoning layer. No training needed for the feature extraction step.

## Algorithm

1. Convolve input with wavelet at scale j1, apply modulus: |x * psi_{j1}|
2. Convolve result with wavelet at scale j2 > j1, apply modulus: ||x * psi_{j1}| * psi_{j2}|
3. Average each layer: S_0 = x * phi, S_1 = |x * psi_j| * phi, S_2 = ||x * psi_{j1}| * psi_{j2}| * phi
4. Concatenate all S_0, S_1, S_2 coefficients as features

Kymatio exists in Python -- would need C89 port. ~400 LOC.
