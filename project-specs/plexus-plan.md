# plexus: plan (house tier)

Approved by Fran (2026-10-15), with the four recommendations of "Open
for Fran" below:
- the names `Plexus`, `VerbumLiberum` and `Ansa`;
- free-group words stay inside plexus until a second library needs them;
- T4's exact dilatation is deferred;
- the Dynnikov disk convention is decided by the exhaustive Artin
  cross-check (oracle 3), not in advance.

Ledger: parcum …11MKY; tasks T1–T6 are opera …PZEDJ3, …T4QB, …SSCX,
…SV30N1, …QTN, …W4T3.

A house library for braid groups: braid words, Artin's action on the
free group (the exact word problem), the Dynnikov action on loops (the
piecewise-linear, depth-keeping action), reduced Burau, and entropy.
Its first consumers are knotapel proposals N1 and N2
(`knotapel/demo-proposals.md`; ledger …AKY6Z7, …4291V2), then audit P3
(…S0E97V) and proposal N3 (…YRF7DKP).

## Purpose

D127 showed that the values of the group catalogs compute no better
than their height. The proposals that followed ask whether a braid's
STRUCTURE computes when it is kept as depth, with a nonlinearity at
every crossing, instead of being multiplied out into one value. That
needs an exact braid toolkit. None exists in `include/`: D66 and D106
each built braid code inside the demo, which the knotapel rule now
forbids ("general number types and algorithms never live in knotapel/
again").

Braids are general-purpose (topology, mixing dynamics, word problems),
so this is a house library. knotapel consumes it.

What v1 must provide:
- **braid words**: letters, inverse, concatenation, free reduction,
  permutation, exponent sum;
- **Artin's action on the free group F_n**: faithful (Artin's theorem),
  so it decides braid equality exactly. It is the library's own oracle,
  and it also gives the knot-group presentation of a closed braid,
  which N3 needs;
- **the Dynnikov action on loops**: integer coordinates, each
  generator a piecewise-linear update built from max(0, x) and
  min(0, x), i.e. one ReLU layer per crossing; plus the intersection
  number L(u), whose growth gives the entropy;
- **reduced Burau** over Z[t, t^-1] and at t = -1: the linear,
  collapsed representation, which is N2's main null and D106's
  machinery rebuilt from house code;
- **entropy**: the Burau spectral-radius lower bound (exact, as an
  algebraic number) and the Dynnikov growth of L(u).

## Sources

Primary source for the Dynnikov update rules, read 2026-10-15:
- J.-L. Thiffeault, "Braids of entangled particle trajectories",
  Chaos 20, 017516 (2010), arXiv:0906.3647, §III B, equations (3)–(15):
  - the coordinates u = (a_1 … a_{n-2}, b_1 … b_{n-2}) from crossing
    numbers (eq. 3–4);
  - L(u) (eq. 5), with b_0 and b_{n-1} (eq. 6);
  - f+ = max(f, 0), f- = min(f, 0) (eq. 7);
  - sigma_i for i = 2 … n-2 via c_{i-1} (eq. 8–9);
  - sigma_1 and sigma_{n-1} (eq. 10–11);
  - the inverses via d_{i-1} (eq. 12–15);
  - footnote 1: the numbering of Hall and Yurttaş [35], the notation of
    Moussafir [33], and sigma_i a CLOCKWISE interchange;
  - a Matlab implementation in its appendix A.2 (`loopsigma`,
    `loopinter`): a second transcription to check against;
  - a worked example (Fig. 8): n = 5, u = (-1, 1, -2, 0, -1, 0),
    L(u) = 12;
  - entropy of sigma_1 sigma_2^-1 (n = 3): log((3 + √5)/2);
  - entropy of sigma_3^-1 sigma_2^-1 sigma_3^-1 sigma_2 sigma_1 (n = 4):
    ≈ 0.83144 (Fig. 10).
- Background, to read in T2: Dynnikov (2002); Dehornoy, Dynnikov,
  Rolfsen and Wiest, "Ordering Braids" (the word problem through
  Dynnikov coordinates); Hall and Yurttaş, "Intersections of
  multicurves from Dynnikov coordinates" (arXiv:1711.00895) and
  "Dynnikov and train track transition matrices of pseudo-Anosov
  braids" (arXiv:1402.4378); Thiffeault, "Braids and Dynamics"
  (Springer 2022), appendix "Derivation of Dynnikov update rules".

**Conventions to fix in T2, from the sources, and record in the
header:**
- the direction of sigma_i (Thiffeault: clockwise; Birman:
  counterclockwise). Flipping it exchanges a generator's rule with its
  inverse's;
- which disk: n punctures in D_n, or the central n punctures of
  D_{n+2} (braidlab's choice for its equality test). On closed loops in
  D_n the full twist Delta^2 (a boundary twist) may act trivially, so
  the loop action on D_n alone may not see it. T2 decides this
  EXHAUSTIVELY against the Artin action, rather than from memory.

## API sketch (include/plexus.h)

Values live in the caller's piscina, like `polynomium` and `radices`.
Coordinates are `Magnus` (already s64 first, big only when needed), so
growth never overflows silently.

```c
nomen structura {
    i32  fila;         /* n, numerus filorum (>= 2) */
    i32  longitudo;
    s32* litterae;     /* +i = sigma_i, -i = sigma_i^-1, 1 <= i < fila */
} Plexus;

nomen structura {      /* verbum gregis liberi F_n */
    i32  longitudo;
    s32* litterae;     /* +j = x_j, -j = x_j^-1, 1 <= j <= n */
} VerbumLiberum;

nomen structura {      /* ansa: coordinatae Dynnikov */
    i32     fila;
    Magnus* a;         /* fila - 2 */
    Magnus* b;         /* fila - 2 */
} Ansa;

/* verba */
b32 plexus_creare (i32 fila, constans s32* litterae, i32 numerus,
                   Piscina* piscina, Plexus* exitus);   /* FALSUM: littera extra 1..fila-1 */
Plexus plexus_inversum (Plexus p, Piscina* piscina);
Plexus plexus_iungere (Plexus p, Plexus q, Piscina* piscina);
Plexus plexus_reducere (Plexus p, Piscina* piscina);    /* sigma sigma^-1 delentur */
s64    plexus_summa_exponentium (Plexus p);
vacuum plexus_permutatio (Plexus p, i32* imago);        /* imago[fila] */

/* actio Artin super F_n: fidelis, ergo aequalitas exacta */
VerbumLiberum plexus_artin (Plexus p, i32 j, Piscina* piscina); /* imago x_j */
b32           plexus_aequalis (Plexus p, Plexus q, Piscina* piscina);

/* actio Dynnikov */
Ansa ansa_ex_coordinatis (i32 fila, constans Magnus* a,
                          constans Magnus* b, Piscina* piscina);
Ansa ansa_agere (Ansa u, Plexus p, Piscina* piscina);   /* ansa post plexum */
Magnus ansa_sectiones (Ansa u, Piscina* piscina);       /* L(u), eq. 5-6 */
b32  ansae_aequales (Ansa u, Ansa v);

/* Burau reductum */
b32 plexus_burau (Plexus p, Piscina* piscina, Matrix** exitus);   /* Z[t, t^-1] */
b32 plexus_burau_minus_uno (Plexus p, Piscina* piscina, Matrix** exitus); /* Z */

/* entropia */
b32 plexus_radius_burau (Plexus p, Piscina* piscina, RadixRealis** rho);
    /* radius spectralis Burau(-1): limes inferior dilatationis */
```

The names are a sketch for Fran. One open choice: `Ansa` ("loop",
"handle") for a Dynnikov vector.

## Exactness

- Every value is an integer (`Magnus`), every Burau entry a Laurent
  polynomial (`polynomium`), every eigenvalue an exact real algebraic
  number (`radices`: the characteristic polynomial det(tI - M) via
  `matrix_determinans` over Z[t, t^-1], then real root isolation).
- No float anywhere in the library. Entropy values are printed from
  exact algebraic numbers; growth rates measured from L(u) are
  sequences of exact integers, and their logarithm is computed only
  for display.
- No silent caps. The free-group words of the Artin action grow
  exponentially with braid length; any limit is a parameter, and
  reaching it returns FALSUM (never a truncated answer).

## Oracles (each independent of the code it checks)

1. **Artin against the braid relations**: sigma_i sigma_{i+1} sigma_i
   = sigma_{i+1} sigma_i sigma_{i+1}, far commutation, sigma
   sigma^-1 = 1, all as equal automorphisms; the full twist Delta^2
   central and nontrivial; the permutation a homomorphism.
2. **Dynnikov against the source**: Fig. 8's vector gives L = 12; every
   update rule checked against the appendix's Matlab transcription on
   random integer vectors (signs mixed, values up to 2^70).
3. **Dynnikov against Artin, exhaustively**: for every pair of braid
   words up to length ℓ on n = 3 and 4 (ℓ set by the run time),
   equal action on the chosen loop family ⟺ `plexus_aequalis`. Both
   directions; this settles the D_n / D_{n+2} question.
4. **Burau against Artin**: Burau is a homomorphism (checked on all
   short words), and for n = 3 it is faithful (recalled: verify), so on
   B_3 equal Burau ⟺ equal braid.
5. **Burau against the knot table**: the Alexander polynomial of a
   closed braid from Burau, Δ(t) ≐ det(I - B(β)) (1 - t)/(1 - t^n), for
   sigma_1^3 (3_1), (sigma_1 sigma_2^-1)^2 (4_1), sigma_1^5 (5_1),
   compared with `tabula_nodorum`'s Alexander polynomials (computed by
   `laqueus` from KnotInfo's PD codes: a fully independent route).
6. **Entropy**: Burau(-1)'s spectral radius for sigma_1 sigma_2^-1 is
   exactly (3 + √5)/2 (the root of t^2 - 3t + 1); Dynnikov growth of
   L(u) matches it; Burau's bound never exceeds the Dynnikov growth on
   any word tested (an inequality that holds by theory); periodic
   braids (powers of Delta, sigma_1 sigma_2 on 3 strands) give linear,
   not exponential, growth; the 4-strand example lands near 0.83144.

## Tasks

Each task follows the house cycle: implement; tests red, then green;
planted faults (`silva.planta`); worklog; commit behind the gates; stop
for Fran's go-ahead.

- **T1. Words and the Artin action.** `Plexus`, `VerbumLiberum`, words,
  free reduction, permutation, exponent sum, `plexus_artin`,
  `plexus_aequalis`. Credo: oracle 1; refusals (bad letters, fila < 2,
  word limit reached).
  *(2026-10-15, done. Names without proper nouns, since the Latin lint
  rejects them: `plexus_imago_libera` (the Artin action) and
  `plexus_aequales`; also `plexus_ex_chorda`/`plexus_ad_chordam` and
  `plexus_dimidia_torsio` (Delta). 10 credo sections, 882 checks. The
  main oracle is the action computed a second way in the test, by
  substitution from the right: 1,203 images agree. Also: the product
  x_1 ... x_n is fixed; each image is conjugate to the generator the
  permutation names; braid relations; Delta^2 central and nontrivial;
  Delta s_i Delta^-1 = s_{n-i}; rewritten words stay equal. Word growth
  under (s1 s2^-1)^m tends to (3 + sqrt 5)/2, the dilatation (oracle 6
  in another form). The image pool is compacted (long words leave
  garbage). venenum clean. 15 plants, 14 red, 1 equivalent mutant; two
  tests strengthened after a plant survived (compaction, running-total
  limit).)*
- **T2. The Dynnikov action.** Read the sources first and record the
  conventions (direction, disk) in the header. `Ansa`, the update rules
  (8)–(15), `ansa_sectiones`. Credo: oracles 2 and 3.
  *(2026-10-15, done. The paper's equations and its Matlab appendix
  agree line by line; the test's oracle re-types the Matlab in s64.
  Convention: the paper's (clockwise sigma_i, left to right), which
  does not affect equality. Disk decided by oracle 3: on D_{n+2} the
  loops E_1 .. E_{n+1} partition every word up to length 6/5/4 on
  B_3/B_4/B_5 exactly as Artin does (577, 1,579, 1,143 classes); on D_n
  Delta^2 is invisible. Added: `ansa_circa` (E_k), `ansa_ad_chordam`,
  `plexus_in_maiore` (embedding), `plexus_aequales_ansis` (equality on
  D_{n+2}). Growth matches (3 + sqrt 5)/2 and the paper's 0.83144.
  20 credo sections, 1,000 checks; sanitizers clean. 16 plants, 15 red,
  1 equivalent within the tested range (E_{n+1} redundant); three tests
  strengthened after survivors.)*
- **T1 review (2026-10-15, done between T2 and T3).** An independent
  reviewer with two Python oracles (Artin, and Burau over Z[t, t^-1])
  found 0 disagreements over 1.83 billion equality verdicts and 345,888
  images, and 3 M and 8 L findings, all fixed: the compaction cliff
  above ~8,192 strands, prefilter-masked test gaps, untested bounds,
  `plexus_validus`, `plexus_imagines_liberae`, and the action
  directions stated and tested. The 11 surviving mutants are now red.
  This does not replace T5, the full review after T4.
- **T3. Reduced Burau.** Over Z[t, t^-1] and at t = -1; characteristic
  polynomial; `plexus_radius_burau`. Credo: oracles 4 and 5, and the
  exact (3 + √5)/2.
  *(2026-10-15, done. Proper names went into the glossary as
  ignotum-permissum entries with a "nomen proprium" note (Fran):
  `plexus_burau`, `plexus_burau_minus_uno`, `plexus_alexander`, and
  `plexus_burau_radix_maxima`, which is max(1, max |real eigenvalue|)
  of B(-1), renamed from `plexus_radius_burau` because the real roots
  give a lower bound, not the spectral radius. Burau partitions B_3
  words exactly as Artin does, and B_4 up to length 5 too. The
  Alexander polynomials of eight closures (3_1 to 7_1) match KnotInfo
  via the knot table, and Markov invariance holds. (3 + sqrt 5)/2 is
  exact, also with negative eigenvalues. The B_3 radix matches loop
  growth within 1%. Limitation: for the paper's 4-strand braid the real
  eigenvalues give 1. 33 sections, 1,101 checks, sanitizers clean.
  13 plants: 12 red, 1 equivalent in practice.)*
- **T3b (2026-10-16, done; after the D128 review): an exact Burau
  certificate at roots of unity.** `plexus_burau_radius_maior(p, ordo)`
  combines Kolev (cited) with Kronecker, applied to the realified
  integer matrix of B(zeta). It certifies positive entropy where
  Burau(-1) is blind (s1 s3^-1 s2 at order 3). Zero-entropy braids are
  refused at orders 2..6, and certification always implies loop growth.
  40 sections, 1,171 checks; 5 plants red.
- **T4. Entropy by growth.** L(u) under iteration, exact; growth table;
  credo: oracle 6. *Sketch for Fran first:* the exact dilatation (the
  stabilised piecewise-linear branch gives an integer matrix whose
  largest eigenvalue is the dilatation: Hall and Yurttaş,
  arXiv:1402.4378). This is optional for N1, and a bigger piece.
  *(2026-10-15, done. `plexus_incrementum` returns exact L(u . p^m)
  using two alternating scratch pools. Oracle 6: the ratio tends to
  (3 + sqrt 5)/2 and to the paper's 2.2966; the twist grows linearly,
  2 + 2m; the periodic braid stays constant; the Burau(-1) radix never
  exceeds the growth on 40 random braids on B_3..B_5 (exact fraction
  check). Loop growth is the primary entropy tool, because Burau's real
  eigenvalues give only 1 for some pseudo-Anosov braids. The exact
  dilatation stays deferred. 35 sections, 1,118 checks; 4 plants
  red.)*
- **T5. Review**: an independent reviewer reimplements the update rules
  and the Artin action in Python straight from the sources (not from
  our code) and compares on random and exhaustive inputs; then fixes.
  *(2026-10-15, done. An independent reviewer with oracles from the
  papers (Dynnikov equations, Artin, textbook Burau, Fox calculus,
  Sturm sequences) found 0 disagreements, and H 0, M 3, L 6, N 5. All
  were fixed:
  - plexus_aequales_ansis memory O(n), with the prefilters;
  - loop-pool compaction checked every letter;
  - Burau's scratch matrix in its own pool;
  - `ansa_valida`;
  - `plexus_apex_proprius` for testing the memory claims;
  - the missing guard and boundary tests, and corrected wording.
  39 sections, 1,154 checks; 13 plants, 11 red, 2 equivalent.)*
- **T6. N1 as a demo** (D128), with its own plan for Fran's approval:
  the per-generator ReLU-layer form and operation counts; growth and
  entropy over all braid words up to length ℓ; periodic against
  pseudo-Anosov; comparison with D106's numbers. N2 (D129) then gets
  its own plan.

## Gates

- `radix` (the new lib and its probatio); `silva` (examen, Latin lint:
  new words may need the glossary, e.g. a proper name like Dynnikov
  stays out of identifiers); `venenum` (add plexus to the math-tier
  suite, as cellae was); `oratio` if the glossary is touched.
- New lib file: `tools/compile_tests_fontes_generare.sh` output
  regenerated in the same commit.

## Out of scope (v1)

- Closure to a PD code and Markov moves (P3 needs them; a later slice).
- Train tracks / Bestvina–Handel; the Garside normal form; the
  Dehornoy order (beyond what the word problem needs).
- Floats of any kind, including the "projectivized" real coordinates
  Thiffeault mentions.
- A general logic-to-layers compiler (proposal N4): a separate design
  conversation. The Dynnikov layers would be a natural first program
  for it.

## Open for Fran

1. The API sketch and names (`Plexus`, `VerbumLiberum`, `Ansa`).
2. Free-group words inside plexus (proposed: yes, until a second
   library needs them; the PULL rule).
3. T4's exact dilatation: in v1, or deferred until a demo needs it
   (proposed: deferred; the Burau bound plus exact growth suffices for
   N1).
4. The Dynnikov convention: proposed to follow Thiffeault's equations
   as printed, with the disk chosen by oracle 3 rather than in advance.
