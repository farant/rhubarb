# Knotapel Demo 01: Kauffman Bracket

## What This Tests

The atomic operation of the knotapel pipeline: can we compute exact knot
invariants from planar diagrams using nothing but integer arithmetic?

## Hypothesis

The Kauffman bracket (and thus the Jones polynomial) can be computed exactly
for small knots via the state-sum model, using Laurent polynomials with integer
coefficients. No floating point. Different knots produce different brackets.

## Results

All 9 tests pass:

- **Laurent polynomial arithmetic** works correctly (addition, multiplication,
  trimming, equality)
- **Trefoil knot** bracket matches Kauffman's known value (-A^{-5} - A^3 + A^7
  for left-handed; our PD code gives the mirror of the standard right-handed)
- **Figure-eight knot** bracket is correct (A^8 - A^4 + 1 - A^{-4} + A^{-8})
  and palindromic as expected for an amphichiral knot
- **Hopf link** bracket is correct (-A^4 - A^{-4})
- **Knot discrimination** works: trefoil and figure-eight produce different brackets
- **f-polynomial** (normalized bracket) correct for both trefoil and figure-eight

## What We Learned

1. **PD notation is subtle.** The counter-clockwise labeling convention
   matters for chirality. Our "right-handed trefoil" PD code actually gives
   the left-handed bracket. Both are valid -- mirror image bracket is A -> A^{-1}.

2. **Loop counting is the hard part.** Getting the state-sum right requires
   carefully tracking how arcs pair at each crossing in each resolution, then
   tracing cycles through the global arc connectivity. First attempt was wrong.

3. **Integer Laurent polynomials work beautifully.** No floating point needed.
   All computations exact. Important for the knotapel claim that crossing
   matrices produce exact invariants.

4. **The code is small.** ~500 lines of C89 for complete Kauffman bracket
   computation including full polynomial algebra. Zero dependencies.

## Building

    gcc -std=c89 -Wall -Wextra -pedantic -o demo01 main.c
    ./demo01

## What's Next

- **Demo 02:** Matrix approach -- convert each crossing to a Temperley-Lieb
  matrix, multiply in sequence, take trace. Compare against state-sum results.
  This is the key transition: state-sum is O(2^n), matrix product is O(n).
- **Demo 03:** Yang-Baxter validation -- verify crossing matrices satisfy
  the braid relation R12 R23 R12 = R23 R12 R23.
- **Demo 04:** Taxonomy to braid word -- the compiler step.

## Framework Connection

This demo tests the foundation beneath the Compilation Pipeline principle:
can we go from knot topology to exact polynomial invariants? The state-sum
approach is the brute-force oracle. The next demos will show the same
computation done via matrix multiplication (the approach that scales and
connects to the tangle-as-computation thesis).
