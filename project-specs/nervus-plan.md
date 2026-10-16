# nervus: plan (house tier)

Approved by Fran (2026-10-16), with the five recommendations of "Open
for Fran" below: the name `nervus`; biases from the start; Magnus
coefficients; composition builds a new network; a ReLU is "on" iff
its argument is > 0.

Ledger: parcum …XWKKK; tasks T1–T5 are opera …R9MFT, …GCS66Q,
…HWYE8, …CX2DC, …F75N. N6 (…N84R01) is blocked by this parcum.

A house library for EXACT piecewise-linear networks: straight-line
programs of affine forms and ReLUs over integers, evaluated exactly,
with their activation patterns and the linear (affine) map of each
pattern. No floats, no training. Its first consumer is knotapel
proposal N6 (ledger …N84R01), as demo D129; then N2 (…4291V2), N4
(…ZPRDMSM) and the exact dilatation in plexus (…VFF141H).

## Purpose

D128 showed that a braid word is an exact ReLU network: Thiffeault's
Dynnikov update, written as AFFINE and RELU instructions with integer
coefficients and no biases. The interpreter for those programs lives
in the D128 demo, by decision (plan.md, "Open for Fran" 3: scaffolding
until a second consumer). N6 is that second consumer, and three more
are queued:
- **N6** counts the linear regions of w^m: it needs evaluation that
  returns the activation pattern, and pattern keys for counting.
- **plexus v2** (exact dilatation, Hall and Yurttas): needs the
  linear map of a fixed pattern as an integer matrix (D128 Part D's
  forced pass, done properly).
- **N2** searches braid words that compute Boolean functions: needs
  fast exact evaluation of many networks.
- **N4** compiles a logical graph into weights: needs a network as
  DATA that a compiler can emit, and biases (a gate is a thresholded
  affine form: AND(x, y) = relu(x + y - 1)).

Nothing in the network is braid-specific, so the network lives in a
generic library and plexus EMITS its generator programs into it. The
name `rete` is taken (POSIX sockets, include/rete.h). Proposed:
`nervus` (nervi: nerves; prefix checked free). `flexura` (a bend) is
also free.

## What D128 settled (design inputs)

- Two instructions suffice: AFFINE dst = sum c_k r_k (+ b) and RELU
  dst = max(0, sum c_k r_k (+ b)); min(0, x) = x - relu(x).
- Registers: inputs first, then one fresh register per instruction
  (static single assignment); outputs name registers. Depth, ReLU
  depth and width are properties of the program.
- An activation pattern is one bit per ReLU (argument > 0). At 0 both
  pieces give 0, so ties are harmless for the value but the pattern
  convention must be stated (D128: > 0).
- A fixed pattern makes the network AFFINE: x -> M x + c, M and c
  integer. Without biases c = 0. D128 built M by running the forced
  pattern on basis vectors; the library should build it symbolically.
- Arithmetic: Magnus keeps values that fit in s64 inside the struct,
  without allocation, so Magnus everywhere costs little when values
  are small and stays exact when they grow (braid orbits grow like
  lambda^m). No separate s64 path is needed.

## API sketch (include/nervus.h)

```c
/* a network: inputs, instructions in order, outputs */
nomen structura Nervus Nervus;

/* building */
Nervus* nervus_creare (i32 introitus, Piscina* piscina);
s32     nervus_affinis (Nervus* n, i32 termini, const s32* fontes,
                        const Magnus* coefficientes, Magnus constans);
s32     nervus_relu    (Nervus* n, i32 termini, const s32* fontes,
                        const Magnus* coefficientes, Magnus constans);
void    nervus_exitus  (Nervus* n, i32 numerus, const s32* registra);

/* composition: outputs of a feed inputs of b (same widths) */
b32     nervus_componere (const Nervus* a, const Nervus* b,
                          Piscina* piscina, Nervus** exitus);

/* exact evaluation; forma (optional) receives one bit per ReLU */
b32     nervus_aestimare (const Nervus* n, const Magnus* introitus,
                          Magnus* exitus, NervusForma* forma,
                          Piscina* piscina);

/* the affine map of a pattern: x -> M x + c, exactly */
b32     nervus_forma_affinis (const Nervus* n, const NervusForma* f,
                              Piscina* piscina, Matrix* m,
                              Magnus** c);

/* patterns as keys (for tabula_dispersa), equality, popcount */
chorda  nervus_forma_clavis (const NervusForma* f, Piscina* piscina);

/* shape: instructions, ReLUs, depth, ReLU depth, width */
void    nervus_figura (const Nervus* n, NervusFigura* figura);
```

plexus adds one function:

```c
/* the Dynnikov action of p as a network on 2n - 4 coordinates */
b32 plexus_nervus (Plexus p, Piscina* piscina, Nervus** exitus);
```

Names, argument order and whether `Nervus` is opaque are open (see
"Open for Fran").

## Exactness

All values and coefficients are Magnus; patterns are sign data. Every
function is exact or refuses (b32 FALSUM, nothing written). The affine
map of a pattern is computed by propagating affine forms through the
program (each register an affine form in the inputs; a ReLU forced on
copies its form, forced off gives 0), not by sampling.

## Oracles (each independent of the code it checks)

1. A Python evaluator written from the definition (not from
   nervus.c): random programs, random inputs, values and patterns
   compared, including 2^200-scaled inputs.
2. The affine map: for random programs and patterns, M x + c equals
   the forced evaluation on random x (the forced evaluation is a
   separate, simpler code path), and on inputs that realize the
   pattern it equals ordinary evaluation.
3. plexus_nervus against ansa_agere on random words and vectors,
   n = 3..8 (D128 Part A's anchor, moved into the library tests), and
   against D128's hand counts (2 / 5 ReLUs per letter, ReLU depth 2).
4. Composition: nervus(p) composed with nervus(q) = nervus(pq).

## Tasks

1. **T1: the core.** Types, building, exact evaluation with patterns,
   shape statistics; refusal on malformed programs (register out of
   range, used before defined). Tests (credo v2 sections) with oracle
   1. Plants.
2. **T2: patterns.** Pattern keys, the affine map of a pattern,
   composition. Tests with oracles 2 and 4.
3. **T3: plexus_nervus.** plexus emits each generator's program
   (Thiffeault eq. 8-15, f- = f - relu(f)) and composes a word. D128's
   anchor becomes a plexus test (oracle 3). D128's own code is not
   changed (frozen).
4. **T4: independent review** with its own oracle, then fixes.
5. **T5: N6 as demo D129**, with its own plan first (sketch below).

## N6 / D129 sketch (its own plan settles these)

- **Question.** Does a braid's entropy set how fast w^m carves its
  input space into linear regions?
- **What is counted.** Distinct activation patterns of w^m over the
  integer box [-R, R]^(2n-4) (every integer vector is a lamination),
  for m = 1..M, per braid of the D128 census, by class P / Z / E and
  against the rate. This is a LATTICE count, exact, and capped by the
  box size, so R must grow with m (or a projective slice be used); the
  plan says how. Polyhedral regions are a different object (they need
  exact linear programming) and are out of scope; the findings will
  say which object was counted.
- **Hypothesis to register.** E: exponential growth in m at a rate
  tied to lambda; P: bounded; Z: polynomial. A tension to test: D128
  Part D found each orbit settles into 2-3 regions within 5 passes,
  so pattern sequences are short transients plus cycles, and the
  count may saturate even for E.
- **Nulls.** The same words through Burau (one region by
  construction); random bias-free PL networks of the same shape and
  coefficient range; shuffled words.
- **Background to read first:** Montufar, Pascanu, Cho and Bengio, "On
  the number of linear regions of deep neural networks", NeurIPS 2014.

## Gates

`./compile_tests.sh nervus` and `./compile_tests.sh plexus`; the
venenum sanitizer suites for both (closure: magnus, matrix and what
they pull in); the Latin identifier lint; examen.

## Out of scope (v1)

- Polyhedral region counting and exact linear programming.
- Floating point, training, gradients.
- Graph-level optimization (merging commuting letters into one layer).

## Open for Fran

1. **The name:** `nervus` (proposed) or `flexura`, or another.
2. **Biases:** included from the start (proposed: N4 needs them, and
   braid networks simply pass zero), or added when N4 comes.
3. **Coefficients as Magnus** (proposed: general, and the fast path
   makes small ones cheap) or as s64.
4. **Composition materializes a new network** (proposed: simple, and
   w^m for N6 is evaluated by repeated passes, not by building w^m),
   or is lazy.
5. **The pattern convention:** a ReLU is "on" iff its argument is > 0
   (D128's convention, proposed), stated in the header.
