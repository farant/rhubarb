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

## 2026-10-15 — T2: the Dynnikov action on loops

**Source, checked twice.** Thiffeault 2010 (arXiv:0906.3647) prints the
update rules as equations (8)-(15) and again as Matlab in appendix A.2
(`loopsigma`, `loopinter`). I compared the two line by line before
coding: they agree, including the sigma_1 / sigma_{n-1} boundary cases
and all four inverse rules. The library follows the equations; the test
re-types the Matlab listing literally in s64 (`_regula_fontis`,
`_regula_sectionum`) as its oracle, on 2,000 random (vector, word)
pairs and on L(u).

**Conventions.** The source's sigma_i is a CLOCKWISE exchange (Birman's
is counterclockwise), and a word acts on a loop from left to right.
Neither matters for equality: mirroring (sigma_i <-> sigma_i^-1) is an
automorphism and reversal an anti-automorphism of B_n, and both
preserve equality. A wrong rule breaks the braid relations, which the
"ansae relationes" section checks on random vectors (half of them
scaled by 2^70).

**The loop E_k** around punctures k, k+1: crossing numbers nu_k = 2,
all others 0, so a = 0, b_k = 1, b_{k-1} = -1. L(E_k) = 2 for every
n = 3..8. The paper's Fig. 8 vector (n = 5) gives L = 12.

**Which disk: settled exhaustively.** Every braid word up to length 6
on B_3 (5,461 words), 5 on B_4 (9,331), 4 on B_5 (4,681), grouped by
the Artin images and by the images of E_1 .. E_{n+1} on D_{n+2}
(central punctures): the partitions are identical (577, 1,579 and
1,143 classes, no disagreement in either direction). On D_n itself the
loops see fewer classes (378 for B_3 <= 6), and Delta^2 fixes every
E_k on D_n but not on D_{n+2}, as expected: on D_n the full twist is a
boundary twist and closed curves cannot see it. `plexus_aequales_ansis`
therefore works on D_{n+2}.

A plant showed that E_{n+1} is redundant within the tested range: the
family E_1 .. E_n gives the same partitions. The library keeps the full
family (one extra loop, cheap); I have no proof that the smaller family
is faithful in general.

**Growth.** L(u) under (s1 s2^-1)^m grows with ratio -> (3 + sqrt 5)/2
(checked exactly in integers for m = 10..20; beyond m ~ 22 the check's
squares overflow s64, which UBSan caught in the first version of the
test). The paper's 4-strand braid s3^-1 s2^-1 s3^-1 s2 s1 grows with
ratio 2.2966 = e^0.83144 within 0.1% (m = 20..30), matching the
paper's entropy 0.83144. (s1 s2)^3 = Delta^2 leaves L = 2 on D_3.
(s1 s2^-1)^3000 gives a 4,166-bit L (predicted 3000 log2 2.618 =
4,165.2) and its inverse returns E_1 exactly.

**Memory.** `ansa_agere` computes in its own pool and transcribes the
result into the caller's (tested: < 4 KB left behind for a 400-letter
word with 280-bit results). Long words with big coordinates compact the
pool every 256 letters once it exceeds 1 MB. The first compaction test
(1,000 periods, n = 3) never reached the threshold, which a plant
showed; 3,000 periods does.

**Plants (16).** 15 red, 1 equivalent mutant within the tested range
(the E_{n+1} omission above). Survivors of the first run, all fixed by
stronger tests: L without the max in b_0 (only hand vectors tested L:
now the loopinter oracle on random vectors), compaction (threshold not
reached), `ansae_aequales` ignoring b (no test compared loops that
differ only in b).
