# KNOTAPEL PROMOTIO — quid ex knotapel in lib/ promovendum sit

*Born 2026-09-04 (Fran's question: "does knotapel have a math
library worth pulling out to rhubarb proper?"). Third of three
surveys taken the same day — see nog-translatio.md and
solarium-comparatio.md for the two external predecessor trees, and
piscina-adoptio.md for what the arena conversion in IV actually
costs and buys. This
one is different in kind: knotapel is INSIDE rhubarb (rhubarb/knotapel),
so nothing is being ported between repositories. The question is
promotion — what earns a place in lib/ under house law.*

*Discipline note: a map of what COULD be promoted, not a work queue.
Rows graduate on PULL. Note that knotapel's own CLAUDE.md exempts it
from the Latin rule ("don't worry about writing things in latin like
the rest of rhubarb, knotapel is kind of a different context") — so
promotion means opting BACK IN, and that cost is real.*

Status vocabulary: **PROMOVENDUM** (general enough for lib/, and
ready) · **EXTRAHENDUM** (worth having, but exists only as
copy-paste across demos — extraction precedes promotion) ·
**MANENDUM** (stays in knotapel: research-specific, or on the way
out).

## I. Quid sit knotapel

Knot theory and topological quantum computing research: Kauffman
bracket, braid words, Temperley-Lieb algebras, Jones-Wenzl
idempotents, DKC, cyclotomic arithmetic, Alexander polynomial, Potts
model. ~112 numbered demos.

139 `.c` files, **4 `.h` files**, 159,689 lines. That ratio is the
whole story of §III.

It also carries its own documentation apparatus — `atlas/` with
`inventory/code-assets.md` (215 catalogued assets with reuse counts),
`floating-point-audit.md`, `demo-index.md`, `theorems.md`. The atlas
is better inventory than most of rhubarb has; this document leans on
it rather than duplicating it.

## II. raqiya — bibliotheca iam extracta

The 4 headers all live in `knotapel/raqiya/`. They are already a
proper library set, and already partly conform to house convention:
tests named `probatio_*` (5,285 lines), a `scrutinium.worklog.md` in
house worklog format, Latin type names inside (`Scr_Signatura`,
`est_catena`, `est_V`, `est_diamas`). Worklog records v1 SHIPPED
2026-02-25.

| header | lines | static fns | what |
|---|---|---|---|
| `scrutinium.h` | 2,936 | 68 | **type-agnostic engine**: partitions, partition lattice (refinement DAG + redundancy detection), pair histograms, edge lists (restrict/intersect/quotient), graph analyzer, degree-sequence comparison. `SCR_WRAP_*` macros for adopting new types. |
| `raqiya.h` | 2,596 | 86 | Z[zeta_8] exact arithmetic + 9 relationship detectors |
| `gauss.h` | 774 | 46 | Z[i] Gaussian integers + 7 detectors |
| `eisenstein.h` | 190 | 20 | Z[omega] Eisenstein integers, exact arithmetic |

**Compiler bar — the cleanest of the three surveys.** Under the full
house flag set (`-std=c89 -pedantic -Wall -Wextra -Wconversion
-Wsign-conversion -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes
-Wwrite-strings`), measured 2026-09-04:

```
probatio_raqiya                     0 warn   0 err
probatio_gauss                      0 warn   0 err
probatio_scrutinium                 9 warn   0 err
probatio_gauss_scrutinium          20 warn   0 err
probatio_eisenstein_scrutinium     18 warn   0 err
probatio_cyc8_scrutinium           17 warn   0 err
```

**All 64 warnings are `-Wunused-function`. Nothing else.** They are
an artifact of the single-header `static` idiom — a consumer calling
12 of scrutinium's 68 functions is warned about the other 56. The
`include/X.h` + `lib/X.c` split under aedilis convention makes every
one of them disappear. The only defect is cured BY the promotion.

(Compare: solarium 6 substantive warnings across 17,134 lines; nog
34 of 56 files failing to compile at all.)

**Cost is the arena, and it varies sharply:**

```
                malloc  free  float
eisenstein.h        0     0      0    <- pure value-type integer arithmetic
gauss.h            15     7      0
raqiya.h           46    30      8
scrutinium.h       68    53      1
```

`chorda` barely applies — these do fixed-size integer arithmetic, not
string work. `Piscina` does: converting scrutinium + raqiya means 114
allocation sites, and deletes 83 `free()` calls outright.

## III. Duplicatio — the real scale of the opportunity

**Only 8 of 139 `.c` files include any local header.** The other 131
are self-contained copy-paste. Measured by counting definitions
across the tree:

```
uf_find    (union-find)   defined in  43 files
cx_add                    defined in  36 files
cx_mul                    defined in  30 files
quat_mul                  defined in  12 files
p_mul      (Laurent poly) defined in  10 files
mat_mul                   defined in   9 files
```

`atlas/inventory/code-assets.md` corroborates independently, with its
own reuse counts over 215 catalogued assets:

```
REUSE  ASSET                                    SIZE
   50  2.2 Union-Find Loop Counter              ~40 lines
   44  1.2 Complex Arithmetic (Cx)             ~100 lines
   30  5.1 NPN Equivalence Classification      ~130 lines
   24  2.12 Braid Word Decoder/Enumerator       ~25 lines
   16  1.3 Cyclotomic Z[zeta_8] (Cyc8)         ~200 lines
   14  6.1 LCG Random Number Generator          ~25 lines
   13  11.1 Group Closure Algorithm             ~80 lines
   13  3.1 Planar Matching Enumeration          ~80 lines
```

Entry 1.1's own note is the honest summary of the whole tree:
*"Copied verbatim between demos (no shared library extraction)."*

This is not necessarily a defect — self-contained demo programs are
a defensible research idiom, and the atlas exists precisely because
the duplication was chosen rather than suffered. But it means the
promotion question has two halves, and this is the larger one.

## IV. PROMOVENDUM

| Asset | Source | Why it belongs in lib/ |
|---|---|---|
| `scrutinium` | `raqiya/scrutinium.h` (2,936) | The prize. Type-agnostic partition/lattice/edge-list/graph engine — nothing in lib/ does this, and it is not knot-specific despite its origin. Already half-Latin. Biggest arena conversion (68 sites); the split kills 64 warnings. |
| `eisenstein` | `raqiya/eisenstein.h` (190) | 190 lines, zero allocation, zero FP, no `math.h`. Pure value-type Z[omega]. Promotion is a translation exercise and nothing else. |
| `gauss` | `raqiya/gauss.h` (774) | Z[i] exact arithmetic, zero FP, 15 allocation sites. General number theory by any standard. |

## V. EXTRAHENDUM

Exists only as copy-paste; extraction must precede promotion.

| Asset | Copies | Size | Note |
|---|---|---|---|
| union-find | **43 definitions** (atlas says 50 demos) | ~40 lines | Most duplicated AND most generic thing in the tree. Belongs in lib/ on its own merits, entirely independent of knot theory. Highest value-per-line in this document. |
| `Cyc8` (Z[zeta_8] exact) | 16 demos | ~200 lines | Already partly subsumed by `raqiya.h`; check for divergence before extracting a second time. |
| LCG RNG | 14 demos | ~25 lines | Trivially general. rhubarb has no RNG in lib/. |
| group closure (`build_closure`) | 13 demos | ~80 lines | Generic BFS closure over a group operation. |
| Laurent polynomial ring (`Poly`) | 8–10 demos | ~120 lines | Integer-coefficient Laurent polynomials with exact division. Atlas notes MAX_TERMS varies between copies (64 early, 120 from D04) — reconcile before extracting. |

## VI. MANENDUM

**`Cx` (complex doubles) — do NOT promote, despite being second-most
duplicated (44 demos).**

knotapel's CLAUDE.md: *"it might be worthwhile to just 100% avoid
floating point, occasionally we have artifacts in results that are
from floating point."* `atlas/inventory/floating-point-audit.md`
documents a deliberate arc away from it in three regimes — pure
integer (D01–D09), FP-native (D10–D28), exact-with-FP-boundary (D29
onward) — and names D100–D105 "the most fully exact demos in the
project," where sign-pattern activations classify integer components
with no trig at all. The audit carries an explicit exactification
roadmap (easy / medium / hard).

Promoting `Cx` to lib/ would install the retreating regime as house
infrastructure. The exact rings are what is ascendant — and they are
exactly what `raqiya/` already extracted.

**`raqiya.h` itself** is the most research-specific of the four
headers (9 detectors tuned to Z[zeta_8], 8 float mentions). It should
follow its dependencies rather than lead them; promote it only if
something outside knotapel wants it.

**The demo bodies** (bracket engines, TL algebra, braid compilers,
DKC search) stay. They are the research, not infrastructure.

## VII. Ordo

1. **`eisenstein`** — 190 lines, no allocation, no FP. Smallest
   possible calibration of what a knotapel→lib promotion costs.
2. **`scrutinium`** — the real prize; do it once §1 has taught the
   translation cost.
3. **`gauss`** — same shape as eisenstein, more of it.
4. **union-find** — ~40 lines against 43 copies. Pure win, no
   dependencies, worth doing regardless of everything above.
5. **`raqiya`** — only on external pull.

## VIII. AUDIENDA

- **Tests not run.** raqiya's 5,285 lines of `probatio_*` compile
  clean under house flags; their pass rate was not checked.
- **The 43 union-find copies were not diffed.** Counted by
  definition-site grep. They may have diverged; the atlas notes
  exactly this hazard for `Poly` (MAX_TERMS 64 vs 120), so assume it
  for the others until checked.
- **Atlas reuse counts are the atlas's own** — parsed from
  code-assets.md "Reused in" lines, not independently verified against
  the demo sources. Where atlas and grep disagree (union-find: atlas
  50, grep 43) both numbers are reported above rather than reconciled.
- **`scrutinium`'s genericity is claimed, not stress-tested here.**
  It carries three type instantiations (Z[i], Z[omega], Z[zeta_8]).
  Whether `SCR_WRAP_*` generalizes to a non-algebraic domain — the
  actual test of lib/ worthiness — is unknown.
