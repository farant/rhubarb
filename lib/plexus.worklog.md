# plexus worklog

## 2026-10-15 — T1: words and the Artin action

Plan: `project-specs/plexus-plan.md` (approved by Fran 2026-10-15).

**Names.** The plan's sketch called the free-group action
`plexus_artin`. The Latin lint does not know `artin` or `burau`
(proper names are not words), so identifiers avoid them: the action is
`plexus_imago_libera` (the image of a free generator). Proper names
stay in comments. T3's Burau functions need the same treatment.

**Convention.** phi_p = phi_{a_1} o ... o phi_{a_m}, so phi_{pq} =
phi_p o phi_q. The library computes it left to right by keeping the
images W_k of all generators and updating two of them per letter:
- sigma_i:    W_i' = W_i W_{i+1} W_i^-1,  W_{i+1}' = W_i
- sigma_i^-1: W_i' = W_{i+1},  W_{i+1}' = W_{i+1}^-1 W_i W_{i+1}

The test's oracle applies the same automorphism the other way, by
substituting letters from the right (phi_{a_m} first). 1,203 images
agree. With this convention Delta^2 acts as conjugation by
x_1 x_2 ... x_n (checked on B_3 for Delta^2 and Delta^4).

**Equality.** `plexus_aequales` reduces r = p q^-1 and tests two exact
necessary invariants first: the exponent sum (a homomorphism to Z) and
the permutation. Only then does it compute phi_r, which is faithful
(Artin), so the answer is exact either way.

**Growth.** Free-word length under (sigma_1 sigma_2^-1)^m is 3, 7, 17,
43, 111, 289, ... (observed: a_{m+1} = 3 a_m - a_{m-1} - 1), so the
ratio tends to (3 + sqrt 5)/2. That is the braid's dilatation
(Thiffeault 2010, Sect. III B): word growth under a pseudo-Anosov
automorphism has the same rate. The test checks the ratio within ~0.5%
exactly in integers (|a_{m+1}^2 - 3 a_{m+1} a_m + a_m^2| < a_m^2/100,
m = 8..12). m = 20 exceeds a limit of 10^8 letters: the Artin action
is exact but exponential, so it is not the tool for long braids. That
is the Dynnikov action's job (T2).

**Memory.** Each letter allocates new words and leaves the old ones as
garbage, so a long word whose images stay small (e.g. (s1 s1^-1)^50000)
used to grow the pool without bound. The image pool is compacted (live
words copied to a fresh pool, the old one destroyed) whenever its use
exceeds 4x the live words + 64 KB. `plexus_aequales` uses the caller's
pool as scratch with notare/reficere, and leaves it as it found it
(tested).

**Plants (silva.planta, 15).** 14 red. One survivor, an equivalent
mutant: `plexus_ex_chorda` accepting a letter equal to `fila` changes
nothing, because `plexus_creare` refuses the letter anyway. Two first
versions of tests were too weak, both found by plants:
- compaction was never triggered, so a compaction that copied the wrong
  image survived. Added the "contractio" section: a long trivial word
  forces compaction, and (s1 s2)^900 = (Delta^2)^300 must give exactly
  u^300 x_k u^-300 (built by the test's own reducer);
- the running-total limit was masked by the per-product limit. Added a
  test with the limit exactly one below the true total.

My own first guesses that the tests caught: (sigma_1 sigma_2^-1)^20
fits in 10^8 letters (it does not: about 2 x 10^8); the commutator
s1 s2 s1^-1 s2^-1 grows linearly (it is pseudo-Anosov, exponential);
the images of Delta^2m all have length 2*3m + 1 (x_3's is shorter
after free reduction).

venenum: probatio_plexus clean under ASan/UBSan (plexus added to the
math-tier suites).
