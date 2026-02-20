## Demo 04: Spin Chain

- **Status**: COMPLETE
- **File**: `demo_04_spin_chain/main.c` (~587 lines)
- **Tests**: 12/12 pass (4 parts: A=2, B=1, C=4, D=5)
- **Depends on**: Demo 03 (Laurent polynomial ring, state-sum oracle, Kauffman bracket)
- **Feeds into**: Any demo needing tensor product R-matrix representation or Yang-Baxter verification

### Headline
Computes the Kauffman bracket via the tensor product (spin chain) representation using quantum trace, unifying Demo 03's separate standard-module traces into a single 8D computation.

### Key Results
- TL generator U on V tensor V (4x4) satisfies U^2 = delta*U and tr(U) = delta
- R-matrix R = A*U + A^{-1}*I satisfies R*R^{-1} = I
- Yang-Baxter equation R1*R2*R1 = R2*R1*R2 verified in 8D (and for inverses)
- **Quantum trace formula discovered**: bracket(beta) = tr_q(rho(beta)) / (A^2 + A^{-2}), where tr_q = tr(K^{tensor 3} * M) with K = diag(A^2, A^{-2})
- Tested on 5 braids: identity (3-unlink), trefoil, figure-eight, mirror trefoil, single crossing — all match state-sum oracle

### Theorems/Conjectures
- Schur-Weyl duality decomposition: V^{tensor 3} = V_{3/2} tensor W_3 + V_{1/2} tensor W_1 (dim 4x1 + dim 2x2 = 8)
- Quantum trace with K = diag(q, q^{-1}) weights each component by its quantum dimension: qdim(V_{1/2}) = [2]_q = -delta, qdim(V_{3/2}) = [4]_q = A^6+A^2+A^{-2}+A^{-6}
- Dividing by -delta recovers exactly Demo 03's formula: bracket = 1*tr_{W_1} + (A^{-4}+A^4)*tr_{W_3}

### Code Assets
- `Poly` Laurent polynomial ring (reused from Demo 03, MAX_TERMS=120)
- `Mat` NxN polynomial matrix (up to 8x8) with: zero, identity, multiply, scale, add, trace, equality, tensor product, quantum trace, print
- `mat_tensor()` — general Kronecker product of polynomial matrices
- `mat_qtrace()` — quantum trace with diagonal weights
- `braid_product()` — maps braid word to matrix product in 8D representation
- State-sum oracle (union-find based, reused from Demo 03)

### Literature Touched
- Kauffman bracket via R-matrix / quantum group approach
- Schur-Weyl duality for U_q(sl_2)
- Yang-Baxter equation
- Quantum trace (Reshetikhin-Turaev style)
