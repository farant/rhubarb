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
