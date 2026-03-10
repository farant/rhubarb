---
id: PT0014
title: "Scattering transforms for training-free feature extraction"
status: promoted_to_kit
promoted_to: KIT0010
source: ["Hilbert Space paper (Kiruluta et al., 2025)"]
applicable_to: []
related: [PT0012, PT0013]
---

Cascading wavelet transforms with modulus nonlinearities produces translation-invariant, deformation-stable features WITHOUT any learned parameters. Achieved 98.6% on CUReT texture classification with just a linear SVM on top. Competitive with deep CNNs on TIMIT speech recognition with far fewer parameters.

This is a feature extraction method that costs zero training time. The scattering transform naturally operates at the edge of chaos -- it captures structure at all scales while being stable to perturbations.

For eigenflower: use scattering as a fixed feature extractor for text, then apply kernel methods for the reasoning layer. No training needed for the feature extraction step. The Representer Theorem then handles the reasoning operators.

Implementation: the scattering transform is a fixed cascade of convolutions with modulus nonlinearities. Well-defined algorithm. Would need C89 port (Kymatio exists for Python).

Could be combined with complexity scoring (Edge of Chaos) to ensure the scattering representations are at the right complexity level.
