# temperley and the TL bracket in laqueus: plan (D136 T4, step 1)

Status: v1 APPROVED by Fran (2026-10-30: API, library split, name,
strand cap); design review next, before any code.

Agreed with Fran (2026-10-30), before this plan was written: T4 goes
**promote first, then a speed pass, then track A** (Jones statistics on
random braid closures). This plan is the first step only. The speed pass
gets its own plan with registered predictions; track A gets its own
plan, after a literature check.

## Purpose

D136 showed that the Kauffman bracket of a braid closure through
Temperley–Lieb standard modules is exact, fast where the vertex model is
slow (12 × 801 in 78 CPU-minutes against a projected month), and slower
where the vertex model is cheap (short braids with many strands). The
demo is frozen; this plan moves the method into house libraries with
tests, so that later work (the speed pass, track A, and §12.3.2–3 of
the brainstorm: colored Jones through Jones–Wenzl projectors, TL at
roots of unity) builds on a tested function instead of a demo.

## Governing rules

1. **Exact only**, as in contractio-plan.md rule 1: reconstruction is
   driven by the proven Tait-graph bound (`laqueus_modulus_vertex_limes`),
   never by "the answer stopped changing".
2. **Same algorithm as the demo first.** The library reproduces D136's
   evaluator (two buffers, row gather, letters last to first, Δ_j
   weights, /δ). Speed changes come later, each against this baseline.
3. **No globals.** The demo cached modules and buffers in statics; the
   library keeps everything in the structure, memory from the caller's
   piscina, with a byte cap like the vertex model's `octeti_maximi`.
4. **Same contract as the vertex model** wherever both apply: the same
   points (A = i·ω²), return codes (INTERPOLATIO_UTILE / _INUTILE /
   _ERRATUM), interpolation settings and bound, so the two routes are
   interchangeable and comparable at every point.

## Proposed API

### New library `temperley` (TL_n and its standard modules)

Pure algebra; no knots, no primes. Separate from `laqueus` because the
modules serve more than the bracket (Jones–Wenzl projectors, roots of
unity) and because their tests (D136's R2 and R3) stand alone.

```c
/* temperley.h - algebra Temperley-Lieb TL_n: moduli standard W_{n,j}
 * Status nexus: n puncta, paria non transeuntia, j defectus (puncta
 * sine pari), nullus defectus intra par inclusus. Dimensio
 * d = C(n, (n-j)/2) - C(n, (n-j)/2 - 1); summa d^2 super j = Catalan(n). */

nomen structura TemperleyModulus TemperleyModulus;   /* opacum */

/* W_{fila, defectus}; NIHIL si fila > TEMPERLEY_FILA_MAXIMA (XVI),
 * paritas fila - defectus impar, aut defectus > fila */
TemperleyModulus*
temperley_modulus_creare (
         i32  fila,
         i32  defectus,
    Piscina*  piscina);

i32  temperley_modulus_dimensio (constans TemperleyModulus* m);

/* e_i (1 <= i < fila) in statum s (0 <= s < d):
 *   genus TEMPERLEY_NIHIL (e_i s = 0), TEMPERLEY_UNUM (e_i s = imago),
 *   TEMPERLEY_DELTA (e_i s = delta * imago, ansa clausa) */
s32
temperley_actio (
    constans TemperleyModulus* m,
                          i32  i,
                          i32  status,
                          i32* imago);

/* "(())|" et similia - ad probationes et ad errores legendos */
chorda
temperley_status_ad_chordam (
    constans TemperleyModulus* m,
                          i32  status,
                     Piscina*  piscina);
```

### In `laqueus`: the bracket of a braid closure through TL

Mirrors `laqueus_modulus_vertex_*` one for one.

```c
nomen structura LaqueusModulusTemperley LaqueusModulusTemperley;  /* opacum */

/* structura semel: moduli W_{n,j}, duo alvei (2 x max d^2 i32), codex
 * PD (pro limite et fenestra). Verbum ut laqueus_pd_ex_plexu; NIHIL si
 * verbum refutatur, fila > XVI, aut alvei > octeti_maximi. */
LaqueusModulusTemperley*
laqueus_modulus_temperley_creare (
    constans s32* verbum,
             i32  longitudo,
             i32  fila,
             i64  octeti_maximi,
         Piscina* piscina);

/* contractus idem ac laqueus_modulus_vertex_aestimare */
s32
laqueus_modulus_temperley_aestimare (
    LaqueusModulusTemperley* mt,
                        i32  omega,
                        i32  p,
                        i32* punctum,
                        i32* valor);

/* uncinus exactus per interpolatio (fenestra, passus, limes ut vertex) */
b32
laqueus_modulus_temperley_uncinus (
    LaqueusModulusTemperley* mt,
                    Piscina* piscina,
                 Polynomium* exitus);

/* sumptus per punctum praedictus: longitudo x summa d_j^2 */
i64
laqueus_modulus_temperley_sumptus (
    constans LaqueusModulusTemperley* mt);
```

And one entry point that picks the cheaper route per braid:

```c
nomen enumeratio {
    LAQUEUS_VIA_VERTEX = I,
    LAQUEUS_VIA_TEMPERLEY = II
} LaqueusVia;

/* uncinus (et Jones) clausurae tricae: via vilior per sumptum
 * praedictum (vertex: multiplicationes contractionis; TL: longitudo x
 * Catalan(fila)); via NIHIL licet, aliter via electa. FALSUM si verbum
 * refutatur aut neutra via intra octeti_maximi. */
b32
laqueus_uncinus_ex_plexu (
    constans s32*  verbum,
             i32   longitudo,
             i32   fila,
             i64   octeti_maximi,
         Piscina*  piscina,
      Polynomium*  exitus,
      LaqueusVia*  via);

b32
laqueus_jones_ex_plexu ( ...same arguments... );
```

The cost comparison uses D136's measured constants (vertex ~1.85 ns per
contraction multiplication, `build/nodi_magni/coniectura.out`; TL
~1.5 ns per c × Catalan(n), findings Result 2). Crossover examples it
must reproduce: vertex at 12 × 101 and 12 × 201, TL at 8 × 201 and at
every 12 × 401 and longer (findings Result 3).

## Tests

- `probatio_temperley.c` (D136's R2 and R3, now permanent):
  - dimensions equal the formula and Σ d² = Catalan(n), n ≤ 16;
  - every state valid (non-crossing, no enclosed defect);
  - e_i² = δe_i, e_i e_{i±1} e_i = e_i, far commutation, on every basis
    state, n ≤ 12 (n ≤ 16 if the suite stays under a few seconds);
  - refusals: fila 0 or > XVI, wrong parity, defectus > fila.
- `probatio_laqueus.c` additions:
  - TL bracket = state sum (c ≤ 24) on random braids, n ≤ 8;
  - TL bracket = vertex bracket on random braids up to c ≈ 100;
  - TL value = vertex value at points of two primes (no interpolation);
  - known knots: 3_1, 4_1, T(3,4), T(4,5) against the table;
  - a few lines of D136's `brackets.txt` (cheap sizes, e.g. 3 × 50,
    8 × 101, 8 × 201) embedded as strings: the frozen demo as oracle;
  - the dispatcher's choices on the crossover examples above;
  - refusals: links (multi-component closures), letter out of range,
    buffers over the byte cap.
- Plants (tool reliability doctrine: every gate born red by a compiling
  plant): case (c) of the action → 0; Δ recurrence with +; /δ dropped;
  σ⁻¹ given σ's image; enclosed defects admitted. Each must turn a
  named test red.

## Tasks

- **T0. Approve this plan.** Fran on the API and the library split;
  then a design review (subagent, before any code), folded in as v2.
- **T1. `temperley`.** Header, source, probatio, glossary entries
  (`temperley`, `lieb`, any new word the lint flags), plants.
- **T2. `laqueus_modulus_temperley_*`.** Creare, aestimare, uncinus,
  sumptus; probatio additions except the dispatcher; plants. The Tait
  bound and the Kauffman window are today reachable only through the
  vertex structure (`laqueus_modulus_vertex_limes`, which builds the
  whole network); T2 factors them into an interior helper on the PD
  code that both routes call, with the vertex model's results
  unchanged (its probatio must stay byte-identical).
- **T3. `laqueus_uncinus_ex_plexu` / `_jones_ex_plexu`.** The cost
  model and the dispatcher; probatio; `lib/laqueus.worklog.md` entry.
- **T4. Review.** Independent reviewer, own oracle (recensor-d136's
  regular-representation oracle can be reused as the reference), not
  building in the live tree.

## Questions for Fran (answered 2026-10-30: all three as recommended)

1. **The library's name.** → `temperley`. `temperley` (proper name, like `jones` and
   `alexander` in laqueus identifiers; glossary entries `temperley`,
   `lieb`) is the plain choice. A Latin alternative would describe the
   objects, e.g. `copulae` (pairings), but nobody would find the
   Temperley–Lieb algebra under it.
2. **Separate library or inside `laqueus`?** → separate. (Inside would
   be simpler now, but laqueus.c is already 3,244 lines.)
3. **Fila maximum XVI.** → yes. D136 measured up to 16 (35 million states,
   106 MB of buffers per point). Higher n is possible in principle but
   untested; the cap keeps the contract honest.

## AUDIENDA

- The dispatcher's constants are from one machine and one load; the
  choice can be wrong near the crossover, which costs time, never
  correctness (both routes are exact).
- `brackets.txt` lines as test oracle: they were computed by the demo
  and certified by the T3 reviewer's independent oracle, so they are
  not self-referential, but the library will share the demo's
  algorithm.
- Nothing here has been measured yet for the library itself; the demo's
  numbers apply only insofar as the code is the same.
