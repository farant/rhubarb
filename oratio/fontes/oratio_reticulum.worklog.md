# oratio_reticulum worklog

## 2026-09-14 — T35 d, the instrument

The partition-lattice instrument (spec `oratio-spec.md` §2 decisions
49–53, §7 "Design — T35"; plan `project-specs/partitio-reticulum-plan.md`):
`oratio/fontes/oratio_reticulum.h/.c` does the work, `oratio/instrumenta/
reticulum.c` is a thin main, `./oratio/reticulum.sh` the wrapper (the
`verba.sh` pattern). The library takes the TSV as a chorda, so the
probatio runs it in-process on a committed fixture.

**Compute first, render last.** The chain (inside the partition
library), the end-of-chain probe and the pair table all evaluate
candidates inside `piscina_notare` … `piscina_reficere` scratch regions,
keeping only numbers in arrays allocated before the mark. A
`ChordaAedificator` that grows by allocating from the same piscina must
therefore not exist during computation: if it grew inside a region, the
refit would free its buffer. So every number is computed and stored
first, and the builder is created only for rendering.

**Phases over one context** (`ContextusReticuli`): read (headers by
KIND, width checked per row, rows of other kinds ignored, `#` lines
skipped), judge the options, select rows (`-ubi`), build values and
partitions (`-gradus` → `0 | 1 | plures`), compute, render. The order of
the checks is fixed so a refusal is predictable: header and widths while
reading; then gold exists → gold prefix → folds exist → `-ubi` titles →
`-columnae` titles → `aurum-` among features → `-praeter`/`-gradus`
titles → `-greges`; then no rows after `-ubi`; then a non-numeric
`-gradus` value; then fewer than two folds. Every refusal is a named
cause (`reticulum: …`), exit 2 from the main.

**Features by default** are every header title except `aurum-` columns
and the fold column; `-columnae` replaces the default (in the order
given), `-praeter` removes. Machine rows: `RETICULUM-COLUMNA` in gain
order (ties by feature order), `-SORS` per feature per fold (fold named
by the fold column's value at the group's representative row),
`-CATENA` vetoed then pooled (step 0 = the one-group partition; gain and
falling folds against the previous step), `-PAR` the top twenty by
interaction gain, `-TEGIT`, `-DUPLEX`; each kind with its own `COLUMNAE`
header, printed even when no rows follow.

**One addition to the plan's interface:**
`oratio_reticulum_optiones_initium` (defaults: limen 20, empty lists
from the piscina), used by the main and the probatio alike.

**Numeric flags are decimal.** `-limen` and `-lucrum` parse digits only.
The plan's step 4 wrote `-limen II -lucrum I`, which prints the usage and
exits 2 — the plan text was wrong, not the parser (the other oratio
instruments take decimal numbers too).

**Gate** `oratio/probationes/probatio_oratio_reticulum.c`: fixture
`fixa/reticulum/proba.tsv` (grid II of the judgment probatio as `PROBA`
rows, a comment line, a row of another kind) → machine rows compared
line by line with `fixa/reticulum/exspectata.tsv`, derived by hand in
T35 b; the human report names `cadit C`; a one-fold run refuses; nine
named refusals from inline TSV; `-ubi f=u` keeps 12 rows. Green on the
first run (0.18 s). By hand through the wrapper: the machine output
equals the expected file apart from the path, a refusal exits 2 with its
cause on stderr.

**Planted fault** (`silva.planta`, gate `oratio` filtered `reticulum`):
the row filter `si (!_ordo_manet(c, ordo))` became
`si (FALSUM && !_ordo_manet(c, ordo))` — `-ubi` ignored. RED exactly
where predicted: the `-ubi f=w` run no longer refuses ("nulli ordines
post -ubi", `probatio_oratio_reticulum.c:191` and `:198`) and the
`-ubi f=u` run no longer keeps 12 rows (`:328`). GREEN after the
automatic revert.
