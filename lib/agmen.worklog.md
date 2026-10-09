# agmen worklog

## 2026-10-09 - slice 1: modular kernels, NEON + scalar reference

API approved by Fran (agmen.h sketch): array-level kernels only (no
public register types), modulus handle `AgmenModulus` (Montgomery
constants computed once - the extensio lesson: per-call setup cost
belongs in a handle from day one), odd p < 2^31, backend by compiler
macro (`__ARM_NEON && __aarch64__`, escape `-DAGMEN_SCALARIS`), not by
aedilis OS variant (ISA is a different axis from OS; NEON is mandatory
in AArch64, so no runtime dispatch).

**Montgomery fits in 64 bits.** R = 2^32, p < 2^31: T + q p < 2 p R <
2^64, so redc needs no 128-bit product (C89). a b mod p = redc(redc(a
b) r2); multiplica_adde converts c once (redc(c r2) = c R); productum
sums redc(a b) (< 2^31 each, u64 lanes via vpadalq_u32) and corrects
by R once at the end.

**NEON lane bookkeeping.** vmull_u32(low) gives lanes 0-1, vmull_high_u32
lanes 2-3; reinterpret as u32x4 and vuzp1q_u32 = low halves (T mod R),
vuzp2q_u32 = high halves ((T + q p) / R). Plant A3 (uzp2 for uzp1) is
red.

**The scalar reference must be protected from the vectorizer.** clang -O2
auto-vectorized even the Montgomery tail loops (-Rpass=loop-vectorize);
`_Pragma("clang loop vectorize(disable) interleave(disable)")` (guarded
by __clang__; accepted under -std=c89 -pedantic) on every reference loop
and every NEON tail. Check: -Rpass reports 0 vectorized loops in agmen.c.

**Scalar build must not share object files.** Testing -DAGMEN_SCALARIS
via a copied aedilis struere.sh first reused build/aedilis/obiecta's
NEON agmen.o (mtime cache) - "via NEON: adest" in the scalar build was
the tell. Worse, a rebuild there would have left a SCALAR agmen.o for
every later build. Redirect OBIECTA_DIR as well as EXITUS_DIR.

**New library = regenerate test source lists.** compile_tests.sh failed
to link (agmen.c absent) until `./tools/compile_tests_fontes_generare.sh`.

Tests: 840 cases (10 moduli incl. odd composites, lengths 0-17 + 64,
1000, 1024, four fill modes, in place, sentinel past the end), three
oracles per element (public, scalar reference, congruentia's '%'); long
dot product (p-1)^2 = 1. Scalar-only build also green (NEON abest). 13
plants red.

**Measurement (tools/agmen_mensura.c, M2, p = 2^31-1, 4096 elements):**

| ns/element | congruentia (%) | scalar Montgomery | plain C (clang-vectorized) | hand NEON |
|---|---|---|---|---|
| multiplica | 2.2-2.8 | 2.9-3.1 | 1.35 | 1.19-1.31 |
| productum internum | 3.6-3.8 | 1.7 | 0.57-0.65 | 0.53-0.57 |

- clang vectorizes the plain-C Montgomery loops itself (width 4) and
  the hand NEON beats it by only 1.0-1.2x. The straightforward
  widening-multiply NEON is what the compiler already generates.
- Scalar Montgomery multiply is SLOWER than '%' on M2 (fast 64-bit
  udiv); its value is only that it vectorizes.
- Real gains are vs today's house path: 1.8-2.2x multiply, ~7x dot
  product.
- Open: the Kyber/Dilithium-style signed Montgomery on vqdmulhq_s32
  (high-half multiply, no widening to 64-bit lanes) is what a compiler
  does not find; that is the remaining case for hand NEON.

## 2026-10-09 - decision: plain C the compiler vectorizes, no intrinsics

Fran chose option 1 after the measurement above: hand NEON beat clang's
own vectorization of the same C by only 1.0-1.2x, so by the agreed rule
("a kernel stays hand-written only if it beats plain C") the intrinsics
go. The public kernels are now plain C loops over the same inlined
Montgomery helpers; the scalar reference keeps its don't-vectorize
pragma as the oracle; `-DAGMEN_SCALARIS` and `agminis_neon_adest` are
gone (nothing left to force or report). API unchanged - the array-level
decision paid off on day one.

**Guard, because vectorization is now an invisible property.** A compiler
update or an innocent edit (a branch, a call that does not inline) can
stop clang vectorizing a kernel; every correctness test would stay
green while speed halves - a silent gate. Gate `agmen-vectorizatio`
(tools/agmen_vectorizatio.sh) compiles lib/agmen.c with the house flags
and -Rpass=loop-vectorize and demands: (1) every loop marked
`/* AGMEN VECTORIZANDA: <nucleus> */` reported vectorized, (2) one
marker per public kernel, (3) NO other loop vectorized (the reference
stays scalar).

**Not yet in the ledger inventory** (worktree): the gate is registered in
silva.py but no inventory row says lib/agmen.c owes it, so commissio
will not add it automatically - name it by hand until the row exists.

**Option 2 kept for a real modular workload** (multimodular determinant,
NTT): signed Montgomery on the high-half multiply vqdmulhq_s32 (Becker,
Hwang, Kannwischer, Yang, Yang, "Neon NTT: Faster Dilithium, Kyber, and
Saber on Cortex-A72 and Apple M1", TCHES 2022) keeps 4 lanes in 32 bits
instead of widening to 64 - the one trick clang does not find. Measure
against the plain-C kernels with tools/agmen_mensura.c.

The removed straightforward NEON (widening multiply, uzp1/uzp2), kept as
the starting point:

```c
/* redc lanarum IV: productum T in duobus dimidiis (lanae 0-1 in imo,
 * 2-3 in summo), effectus < p */
interior uint32x4_t
_redc_neon (
    uint64x2_t imum,
    uint64x2_t summum,
    uint32x4_t p_inversa,
    uint32x4_t p)
{
    /* T mod R: dimidia ima lanarum (uzp1 = lanae pares) */
    uint32x4_t t_imum = vuzp1q_u32(vreinterpretq_u32_u64(imum),
        vreinterpretq_u32_u64(summum));
    uint32x4_t q      = vmulq_u32(t_imum, p_inversa);
    uint64x2_t s_imum = vaddq_u64(imum, vmull_u32(vget_low_u32(q),
        vget_low_u32(p)));
    uint64x2_t s_summ = vaddq_u64(summum, vmull_high_u32(q, p));
    /* (T + q p) / R: dimidia summa (uzp2 = lanae impares) */
    uint32x4_t r      = vuzp2q_u32(vreinterpretq_u32_u64(s_imum),
        vreinterpretq_u32_u64(s_summ));

    redde vsubq_u32(r, vandq_u32(vcgeq_u32(r, p), p));
}

/* a b R^-1 mod p per lanas */
interior uint32x4_t
_redc_producti_neon (
    uint32x4_t a,
    uint32x4_t b,
    uint32x4_t p_inversa,
    uint32x4_t p)
{
    redde _redc_neon(vmull_u32(vget_low_u32(a), vget_low_u32(b)),
        vmull_high_u32(a, b), p_inversa, p);
}

interior i32
_productum_internum_modulo_neon (
    constans          i32*  a,
    constans          i32*  b,
                      i32   numerus,
    constans AgmenModulus*  modulus)
{
    uint32x4_t p      = vdupq_n_u32(modulus->p);
    uint32x4_t pi     = vdupq_n_u32(modulus->p_inversa);
    uint64x2_t cumulus = vdupq_n_u64(ZEPHYRUM);
    i64        summa;
    i32        i      = ZEPHYRUM;

    per (; i + IV <= numerus; i += IV)
    {
        /* termini < 2^31, bini in lanas 64 bitorum additi */
        cumulus = vpadalq_u32(cumulus, _redc_producti_neon(
            vld1q_u32(a + i), vld1q_u32(b + i), pi, p));
    }
    summa = vgetq_lane_u64(cumulus, 0) + vgetq_lane_u64(cumulus, 1);
    AGMEN_NON_VECTORIZARE
    per (; i < numerus; i++)
    {
        summa += _redc((i64)a[i] * (i64)b[i], modulus);
    }
    redde _redc((summa % (i64)modulus->p) * (i64)modulus->r2, modulus);
}
```

## 2026-10-09 - slice 1 as committed

Plants (silva.planta, 13 red): correctness B1-B9 against the agmen
tests (reduction without subtraction, s == p unreduced, a == b given
+p, public tail truncated, c not in Montgomery form, missing final R
correction, Newton 3 steps, even modulus, 2^31 bound); guard C1-C3
against `agmen-vectorizatio` (pragma on a kernel loop, an early exit
`frange` inside a kernel loop - the realistic way to lose vectorization,
reference pragma removed, marker removed). First C1 attempt put the
pragma on the loop line itself and went red for the wrong reason (the
"line after marker is a loop" check) - re-planted with the pragma line
before the marker.

Benchmark, idle M2 (the earlier table ran beside a frigida run; absolute
times roughly halve, ratios hold), p = 2^31-1, 4096 elements, ns/elem:

| | congruentia (%) | scalar Montgomery | agmen (clang-vectorized) |
|---|---|---|---|
| multiplica | 1.22 | 1.30 | 0.50 (2.4x) |
| productum internum | 1.52 | 0.71 | 0.26 (5.8x) |

Lint: `redc` -> `_reducere`, `sorbitor` -> `consumptor`, sentinel ->
`CUSTOS`/`_custodes_ponere`; glossary: montgomery (proper name),
vectorizare (house technical term).

In-place calls (exitus == a) go through clang's runtime overlap check
and may run the scalar remainder loop; correct (tested), speed not
measured.

**Correction (same day): no `_Pragma` macro.** The first commit attempt
failed the `silva` gate: probatio_silva_canon_corpus asserts ZERO
"invocatio vacua" (a macro that expands to zero tokens - a named gap in
silva's canon, kept absent from the real corpus so it stays visible).
`AGMEN_NON_VECTORIZARE` was exactly that (`_Pragma` is consumed; the
non-clang branch was empty) - 5 uses = the 5 counted. Now each reference
loop carries an explicit
`#if defined(__clang__) / #pragma clang loop vectorize(disable)
interleave(disable) / #endif`; silva handles `#pragma` inside function
bodies (census green). Guard plants re-planted on the new form (C1 with
the pragma BEFORE the marker - between marker and loop it trips the
placement check instead, red for the wrong reason, twice now).
Lesson: a new `lib/*.c` owes the `silva` gate for reasons beyond size -
silva's census reads every construct in the corpus.
