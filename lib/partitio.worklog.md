# partitio worklog

## 2026-09-14 — T35 a, the algebra

Born for oratio T35 (the partition lattice; spec `oratio-spec.md` §2
decisions 49–53, §7 "Design — T35"; plan
`project-specs/partitio-reticulum-plan.md`). Ported from knotapel's
`knotapel/raqiya/scrutinium.h`, not vendored.

**What changed from scrutinium, and why.**

- *Canonical numbering by first appearance* is the one invariant the
  library rests on. Every constructor and the meet go through one
  canonicalizer (`_partitio_ex_clavibus`: row keys as bytes into a
  chorda-keyed `tabula_dispersa`). Two consequences: equal partitions
  have byte-identical `grex` arrays, and every group has a
  representative row `primus[g]`, so a partition carries no titles.
- *Refinement is O(n) with no allocation*: a refines b iff every row has
  the same b-group as its a-group's representative. Scrutinium walks
  groups × rows.
- *Keys are copied as bytes* (`memcpy` into an `i8` array) instead of
  written into `i32`/`i64` arrays from piscina, so nothing depends on
  the arena's alignment.
- *Join* is union-find over rows, each row joined to its representative
  in a and in b; the roots are renumbered by the same canonicalizer.
- *Duplicates* come from the refinement matrix itself (refines both
  ways ⇔ equal, antisymmetry on set partitions) — no second comparison
  path.
- *Width by Dilworth*: unique partitions minus a maximum matching over
  the strict order (Kuhn's augmenting paths). Refinement is transitive,
  so the minimum chain cover equals that difference. Scrutinium's brute
  force is capped at k ≤ 20; the LIS census rows carry 31 columns. The
  probatio keeps the brute force as the ORACLE for k ≤ 12.
- *Covering relation* (`tegit`) added so a report prints the nesting
  diagram, not every implied pair. Altitude = longest strict chain
  among uniques, by k rounds of relaxation.
- Dropped by name: V / diamond flags, complementary pairs, pair
  histograms, edge lists, graph analyser, the ring vtable.

**Bit me: `duplex` is a latina.h macro** (`#define duplex double`). The
field planned as `duplex` expanded to `i32* double;`, the typedef failed,
and examen reported only "typus nominatus ignotus: PartitioReticulum" at
the function declaration — the real error is one struct member away
from the message. Renamed `geminus` (twin: the index of the first equal
partition) in the header, library, probatio, spec and plan. When examen
says a freshly declared typedef is unknown, grep its members against
latina.h first.

**Three process finds.**

- The source-list generator (`tools/compile_tests_fontes_generare.sh`)
  derives closures from what exists on disk: run before the library
  file exists, it leaves `lib/partitio.c` out and the green run fails at
  LINK with every `partitio_*` symbol missing. Run it again once the
  library exists; `-probare` confirms freshness.
- `./oratio/vocabula.sh -nova` counts only git-TRACKED files, so on new
  untracked files it reports "NOVA 0" without having seen them. Check a
  new file's identifier words with `./oratio/quaere.sh` before the
  commit stages it (the pre-commit lint then sees them).
- It did: the first commit was refused by the pre-commit lint for the
  local array `grex13` — a digit suffix makes an unknown WORD (same trap
  as `lecta4` in oratio T33). Renamed `infimi` (the expected meet); a
  name short enough that the declaration block still aligns within 72.

**Gate.** `probationes/probatio_partitio.c`: canonical numbering on
`{7,3,7,9}`; cells as values; a hand family over four rows (discrete,
`{0,0,1,1}`, `{0,1,0,1}`, `{0,0,0,1}`, one group, and a relabelled copy
of the second) with exact duplicates, six covering edges, altitude 2,
width 3; refusal of unequal row counts; laws over 200 seeded random
families of 2–12 partitions of seven rows (meet refines both, both
refine the join, idempotence, commutation, refinement a partial order,
matrix agrees with `partitio_subtilior`, Dilworth width equals brute
force) — 10,623 ordered pairs checked on the first green run.

**Planted fault** (`silva.planta`, gate `radix` filtered `partitio`): the
canonicalizer's found-key branch `p->grex[i] = *(i32*)valor;` replaced by
`p->grex[i] = i;` (a repeated key gets its row index instead of its
group). RED at the first hand case — `p->grex[II] == ZEPHYRUM` at
`probatio_partitio.c:126` — then GREEN after the automatic revert.

## 2026-09-14 — T35 b, the judgment

`include/partitio_aestimatio.h`, `lib/partitio_aestimatio.c`: held-out
scoring and the greedy chain, exactly to the plan's definitions (spec §7
JUDGMENT plus the named additions `suffragia`, `numeri`, `initium`, and
the base tie rule).

**One place for the retention law.** Training counts are totals minus
the held-out fold, computed by ONE helper, `_disciplina(totalis,
in_sorte)`, shared by the base answer and the group votes. This is not
style: a leak planted only in the votes would leave grid III green (the
base is still right, and a tied group votes the base), so the plant
would prove nothing. One law point makes the circularity plant mean
what it says.

**Rules as built.** Base = gold group with the most training rows, a tie
to the lowest gold index. A group votes only with at least `limen`
training rows, for its strict training majority; a tie votes the base. A
group without a vote answers the base, and its rows in the fold count
as `inaestimati`; `inaestimabilis` = `2 * inaestimati > ordines`. The
chain evaluates candidates inside `piscina_notare`/`piscina_reficere`,
keeping only rows right and a "some fold fell" flag in arrays allocated
before the mark, then recomputes the accepted meet outside the scratch
region. Candidates are ranked by rows right; if the best fails the
minimum gain, all the rest do too, so the chain stops there.

**Numerals.** `latina.h` defines a subset of Roman numerals: `D` and
`DCCL` exist, `DCLIII` and `DCCCLXXXIV` do not. Examen reports an
undefined numeral as "identificator ignotus in expressione". The
probatio uses the literals `653` and `884` for those two purities.

**Gate.** `probationes/probatio_partitio_aestimatio.c`, three grids with
every number derived in a comment; all matched on the first green run.
Grid I (interaction): each single column scores 9 of 12, their meet 12,
and the greedy chain takes ZERO steps with or without the veto — the
interaction is invisible to a greedy chain, which is why the instrument
prints pairs. Grid II (veto): the column gains +6 pooled but fold C falls
7 → 5; the vetoed chain refuses it, the pooled chain takes it. Grid III
(ties): votes O, S, S across the folds, one mutable group, both tie
rules exercised.

**Planted fault** (`silva.planta`, gate `radix` filtered
`partitio_aestimatio`): `_disciplina` returned `totalis` (the held-out
fold leaks into training). RED — and FIRST not in grid III as the plan
predicted but in grid I's `limen III` case: inflated training counts
(2 → 3) clear the vote threshold, so every fold scores 4 instead of 3
with no fallback rows. Also red: the fold-as-candidate case
(`inaestimati`, `inaestimabilis`) and grid III (rows right, base,
mutability, votes). Grid II stayed green — its votes do not depend on
the held-out fold's own rows. GREEN after the revert. Lesson for census
work: a leak shows first where a group's training count sits right at
the threshold.

## 2026-09-14 — T36 a, captives, the profile, the seed

Spec §2 decisions 55–56, §7 "Design — T36"; plan
`project-specs/partitio-reticulum-t36-plan.md`. Appended fields only,
nothing renamed.

**Captive groups (decision 55).** A group whose rows all lie in one
fold can never be judged by leaving that fold out. The half-of-rows
rule missed the case decision 53 names (a synthetic verse/prose column
over the real contests: 351 fallback rows of 1,607, no flag, the verse
group voting in five corpora and answering no row). Now `captivus[g]`
holds the fold, and a captive group with at least `limen` rows flags
the column. Grid IV: eleven rows, flagged at threshold two, not at
threshold four — the threshold is tested, not just the presence.

**The profile (decision 56) — a tie is OPEN.** The spec text said
"ordinatus: every fold votes and all votes agree". A group whose
training rows tie votes the base by rule, so that vote is the base's,
not the group's; calling it ordered would dress the base rate as a
bucket. Built: COACTUS = every fold votes with `margo == disciplina`;
ORDINATUS = every fold votes with `margo > 0` and one gold; APERTUS
otherwise, and a mutable group is apertus too (`mutabilis[g]` stays
its own flag so the flip is visible). `margo` is the largest training
count minus the second; the second is tracked beside the maximum in
the same loop (a value equal to the maximum sets the second to it, so
a tie is margin 0 without a special case).

**Options struct for the chain.** `PartitioCatenaOptiones {limen,
lucrum_minimum, sortes_vetant, semen}` replaces three loose
parameters — before the weight table becomes a caller, while the only
callers are the probatio and the instrument. `semen` is the step-0
partition, kept on the catena; a column already inside the seed meets
to the same partition and the gain floor refuses it, so the library
needs no "used" flags for the seed's columns. The instrument's two
`c->una` fallbacks became `catena->semen`.

**Gate.** Grid IV and every profile matched on the first green run.
**Planted fault** (`silva.planta`, gate `radix` filtered
`partitio_aestimatio`): the captive test inverted (`in_sorte !=
totales[g]`). RED — and FIRST in grid I's one-group case
(`probatio_partitio_aestimatio.c:141`, `inaestimabilis`), not in grid
IV: with the test inverted every group spread over several folds
becomes "captive", so the first column judged is the first to flag.
GREEN after the revert. Same lesson as the T35 b plant: a fault lands
where the data first crosses it, not where the test for it was written.
