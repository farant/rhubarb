---
id: PT0013
title: "Spectral reasoning via modulation of frequency components"
status: untested
source: ["Hilbert Space paper (Kiruluta et al., 2025)"]
applicable_to: []
related: [PT0012, PT0014]
---

Represent entities as spectral expansions f_x = sum f_k^(x) * phi_k. A reasoning operator R acts as sparse modulation of frequency components: R[f_x] = sum gamma_k^(r) * f_k^(x) * phi_k. Word analogy "king - man + woman = queen" becomes spectral shift.

Syllogistic operations can be implemented as element-wise multiplication of spectral coefficients. This is extremely efficient via FFT. Each syllogism type has its own modulation pattern (set of gamma_k values).

For eigenflower: the spectral decomposition reveals which aspects of meaning each syllogism operates on. Different frequency components correspond to different features of the entity representations. Low frequencies = broad categorical membership, high frequencies = fine distinctions.

Learn parameterized family of operators {T_theta^(r)} by minimizing sum ||T_theta^(R_i) f_{x_i} - f_{y_i}||^2 + lambda * R(theta). Operators can be linear maps, convolutional filters in basis domain, or neural networks mapping spectral coefficients.

Fourier spectral filtering defines a diagonal operator in Fourier domain = convolution in spatial domain. Very few parameters per "layer." Learnable soft-thresholding provides tunable band-pass filtering.
