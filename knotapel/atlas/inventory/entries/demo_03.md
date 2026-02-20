## Demo 03: TL Module & Braid Representation

- **Status**: COMPLETE
- **File**: `demo_03/main.c` (~603 lines)
- **Tests**: 8/8 pass (4 parts: TL relations, braid inverses, Yang-Baxter, full bracket)
- **Depends on**: Demo 01 (Kauffman bracket state-sum oracle), Demo 02 (braid representation concepts)
- **Feeds into**: Demo 04 (spin chain / tensor product representation)

### Headline
Constructs the correct TL_3 standard module W_1 and braid representation, verifying TL algebra relations, braid group inverses, Yang-Baxter equation, and full Kauffman bracket computation via both standard modules W_1 (dim 2) and W_3 (dim 1).

### Key Results
- TL_3 standard module W_1 (1-defect, dim 2) with basis: e_a = (cap12, free3), e_b = (cap23, free1)
- U_1 = [[delta, 1], [0, 0]], U_2 = [[0, 0], [1, delta]] — satisfy U^2 = delta*U and U1*U2*U1 = U1, U2*U1*U2 = U2
- Braid representation: g_i = A*U_i + A^{-1}*I (NOT the Hecke algebra form — this is the Kauffman bracket form)
- Inverse: g_i^{-1} = A^{-1}*U_i + A*I — works because (A^2 + A^{-2} + delta) = 0
- W_3 (3-defect, dim 1): U_i acts as 0, so g_i = A^{-1} (scalar), g_i^{-1} = A (scalar)
- Full bracket formula: <L> = 1*tr_{W_1}(rho(b)) + (A^{-4} + A^4)*tr_{W_3}(rho(b))
- Verified against state-sum oracle for: figure-eight, trefoil, identity (3-component unlink), mirror trefoil, single crossing

### Theorems/Conjectures
- Extended commentary in header works through the algebra showing g_i = A*I + A^{-1}*U_i does NOT satisfy the Hecke relation g - g^{-1} = (A - A^{-1})*I, but IS invertible in the TL algebra
- Key identity: (A*U + A^{-1}*I)(A^{-1}*U + A*I) = I follows from delta + A^2 + A^{-2} = 0
- Lengthy addendum explores why W_0 doesn't exist for TL_3 (parity: n=3 odd, so defects must be odd: 1 or 3)
- Addendum attempts to find constant coefficients c_0, c_1 for Markov trace as weighted ordinary traces — discovers this doesn't work (Markov trace is more subtle)

### Code Assets
- **Laurent polynomial ring** (`Poly`): zero, monomial, trim, add, multiply, equality, print — reused across later demos
- **2x2 matrix algebra** (`Mat2`): identity, multiply, scale, add, trace, equality, print — over Laurent polynomials
- **4x4 matrix algebra** (`Mat4`): zero, multiply, equality, tensor product — defined but tensor Yang-Baxter deferred to Demo 04
- **Union-find state-sum oracle** (`braid_bracket`): enumerates all 2^n smoothings, counts loops via union-find — serves as ground-truth verifier
- **Braid struct**: word array + length + strand count

### Literature Touched
- Temperley-Lieb algebra standard modules (W_k for TL_n)
- Kauffman bracket skein relation (A*<0-smoothing> + A^{-1}*<1-smoothing>)
- Jones/Kauffman braid representation vs Hecke algebra representation
- Quantum dimensions of TL modules
- Dimension formula: dim(W_k) = C(n, (n-k)/2) - C(n, (n-k)/2 - 1)

### Open Questions
- The addendum's attempt to express the Markov trace as a constant-coefficient weighted sum of ordinary module traces fails — the Markov trace requires a more sophisticated construction (possibly involving conditional expectations / Jones basic construction)
- 4x4 tensor product Yang-Baxter verification explicitly deferred to Demo 04
