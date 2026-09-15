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

## 2026-09-14 — T35 e, the first real run

Rows regenerated with `metire.sh x.tsv -lites`: 24,306 LIS rows, seven
headers (EWT dev produced no contests). Filter: `-ubi caput-victoris-idem=1
-ubi relatio-victoris=subiectum,obiectum-verbi -ubi
relatio-victae=subiectum,obiectum-verbi -ubi aurum-lectio=victor,victa`,
`-gradus` on the counts and distances, `-praeter victor,victa,dependens,
caput,clausula` → 1,607 contests, six folds, base 623 permille. Both runs
take under a second.

**Acceptance 1 held**: the recount (first checked against the fixture)
and the instrument agree on all 146 COLUMNA/SORS/CATENA rows.

**What the tool showed, and what it could not.** Verb number is the best
single column (+75), but the zero-tolerance veto refuses it because LLCT
— 78 rows, 76 loser-right — loses 12; the pooled chain takes it and stops
at `accusativi-certi +5 < 20`. The pair table's top entry, `ante ×
numerus-victae` (+79), is a pure interaction the greedy chain cannot
reach. Ad hoc tables (scratchpad, not committed) confirmed the T32 f
verse/prose flip only at depth three (position × loser number × verb
number: CIRCSE 53:30, Perseus 11:24). Both limits — the veto decided by
the smallest lopsided fold, and depth two — are recorded as quaestio
01M2HA2RAF for Fran; the spec's as-built states the numbers.

**Slip in the ad hoc table**: the vote label was cut to four letters
(`vict`), making victor and victa indistinguishable — the counts and the
flip marker carried the reading. Print labels whole.

## 2026-09-14 — T36 b, the search

Spec §2 decision 54, §7 "Design — T36"; plan
`project-specs/partitio-reticulum-t36-plan.md`.

**The k-meet table replaces the pair table.** `-profunditas N` (default
2, refused above 4) judges every subset of 2..N non-constant feature
columns; the increment of a meet is its rows right minus the best rows
right among its (k−1)-sub-meets, so at depth 2 the old pair definition
is unchanged. Row kind `RETICULUM-INFIMUM` supersedes `RETICULUM-PAR`,
which no reader consumed. The scratch-region rule bit harder here than
in T35 d: the sub-meet lookup needs a hash table keyed by the tuple's
bytes, and the table's nodes are allocated from the piscina, so the
insert (and the tuple it points at, and the entry whose index it
returns) must all happen BEFORE `piscina_notare`; only the meet and its
judgment live inside the region. Entries are a flat array sized by
Σ C(m, d) up front, because an xar that grows would move the entries
the hash values point into. Printing takes the first `-prima` per depth
by repeated selection, not a sort: 20 × 31,465 at depth 4 is cheap,
a sort of 31,465 by insertion is not.

**Constants leave the lattice.** A feature with one group after `-ubi`
is the top element: it made the T35 e cover list eleven rows of
`x < caput-victoris-idem`. Constants are judged and printed as COLUMNA
rows, named on one line, and excluded from the lattice, the k-meet
table and the seed. Grid II's `h` is constant, so its expected rows
lost the TEGIT row and the pair row (regenerated by hand, that is the
cause).

**Seed and conditioned candidates.** `-initium t1,t2` starts both
chains from the meet of the named columns (library `semen`); step 0
names them. Each pooled-chain step that loses rows on a fold becomes a
`RETICULUM-CONDICIO` row and a CANDIDATI CONDICIONATI line (decision
54). `cadentes` split into `cadentes-basi` (COLUMNA, INFIMUM) and
`cadentes-gradu` (CATENA). Permille is now computed in 64 bits.

**The parity fixture.** Gold = parity of three binary columns: every
single and every pair has zero information (training ties everywhere,
so every group votes the base), the triple is exact. It exercises depth
three (+12 over the best pair), the seeded chain (from `f × g` the
chain accepts `h`, +12, no fold falls) and, in T36 c, the group rows.
Every expected row was derived by hand and matched on the first green
run.

**Gate** green on the first run: grid II rows regenerated, parity rows
line by line, seeded chain rows, `-greges initium`, five new refusals
(depth above four, unknown / gold / non-feature / constant seed column,
`-greges initium` without a seed).

## 2026-09-14 — T36 c, the evidence rows

Spec §2 decision 56, §7 "Design — T36" EVIDENCE.

**Group rows.** `RETICULUM-GREX` (one per group of the partition
`-greges` names: a column, the vetoed or pooled chain's end, or the
seed) and `RETICULUM-GREX-SORS` (one per group and fold: training rows,
vote, margin, held-out rows, rows right). This is the weight table's raw
material, produced by the census. The partition choice is one helper
(`_greges_eligere`) shared by the human view and the machine rows, so
they cannot drift; a group's title is the values of the partition's
columns at its representative row, seed columns first. Both headers
print on every run, rows only with `-greges` — the T35 d expected files
gained the two header lines for that reason.

**Interleaved folds.** `-sortes-alternae N` makes the fold of the r-th
kept row `r mod N`, with synthetic titles `alterna-0..`; `-sortes` is
then omitted, so no feature is excluded on the fold's account — the
corpus column becomes a FEATURE, which is itself a question (on grid II
the chain takes `thesaurus` at step 2). Pass `-praeter thesaurus` when
that is not the question. The fold column index is −1 and the fold
titles come from one `valores_sortium` array whichever way they were
made.

**A counting slip caught by the tool.** The plan derived the
interleaved-fold numbers for grid II by hand as 22 right of 26; the C
tool said 23 and the independent Python recount said 23. Recounting the
odd rows by index showed `v` has seven of them, not six. The two
implementations agreeing against the hand number is the acceptance
working in miniature; the plan text is corrected in place and says so.

**`lemma-capitis`** (oracle): the LEMMA token of the head's first
reading, appended as LIS column 32 under the header law; present on
every contest row of the three pinned Latin treebanks (2,413 / 4,233 /
1,973). It exists so `-sortes lemma-capitis` can hold out each verb.

**The recount** grew to every kind it can derive from the judgment
(COLUMNA with captives, SORS, CATENA with the seed, CONDICIO, INFIMUM,
GREX, GREX-SORS) and equals both fixtures' expected rows before the real
run. zsh does not split an unquoted `$extra` — `${=extra}` — a flag
string passed as one word made the recount look broken for a minute.

## 2026-09-14 — T36 d, the second run

Rows regenerated (24,306, `lemma-capitis` on every contest row). Runs:
depth three, seeded from position × loser number with group rows, six
interleaved folds, 307 verb lemmas as folds, and the arc contests.
Numbers in the spec's "As built — T36".

**Acceptance 1 held** (184 + 190 + 166 rows equal) — after a zsh
detour: a two-word command kept in a variable (`P="python3 x.py"`)
does not split either, so `$P` is "command not found" and the recount
file is empty; the C side of the diff looked like a recount defect for
one turn. `${=P}`, or do not keep commands in variables.

**The prediction that was wrong.** (2) said the flipping triple would be
among the top meets. It cannot be: a group whose majority flips between
corpora is answered by the other corpora's majority when its own corpus
is held out, so it loses exactly where it flips (increment −40 against
its best pair). The GREX rows show the flip precisely — CIRCSE 53:30 for
the winner, Perseus 11:24 for the loser, both groups APERTUS and mutable
— and the held-out table refuses to credit it. That refusal is the
tool doing its job: the flip is a CONDITIONED bucket (decision 54), not
a universal one. Write predictions from the definition, not from the
in-sample table.

**The bets that held.** With interleaved folds the LLCT veto vanishes
and the chain accepts verb number then the loser's gender: genre, not
fold size. With lemma folds verb number still gains +75: structural,
not lexical. Zero tolerance over 307 tiny folds refuses everything, so
decision 50 is a rule about corpora; with many small folds read the
pooled chain and the per-fold rows.

**Constants and captives on real data.** Two constant columns left the
lattice (altitude 1 → 0, the eleven cover edges gone); six columns each
carry one captive group of two rows (a rare case value living in one
corpus), reported and under the threshold.

**The arc census** (8,406 contests) is the richer table: head direction
+555 held out, vetoed by the three verse-bearing corpora and named as a
conditioned candidate; direction × distance a pure interaction of +692;
width 19.

## 2026-09-14 — T37 a, the instrument's second pass

Spec §7 "Design — T37"; plan `partitio-reticulum-t37-plan.md`. No
library change; nothing reordered.

**Noise floor by fold agreement.** `sortes-lucrantes` on COLUMNA and
INFIMUM rows (folds whose rows right exceed the base) and
`lucrantes-gradu` on CATENA rows (folds above the previous step); the
human lines carry `lucrantes N` beside `cadentes`. The ranking is still
the increment. The count is an integer the doctrine allows where a
confidence interval is not.

**The transparent veto.** `_finem_computare` already evaluated every
remaining column in a scratch region to name the best; it now keeps
every candidate's groups, rows right, gain, and per-fold falls in
arrays allocated before the mark, so the end of each chain can be
printed in full: `RETICULUM-FINIS` rows with a cause (`lucrum` below the
floor, else `cadit`), and the human `recusatae: a cadit X, b cadit Y` /
`ceterae N lucro deficientes`. The pooled chain's rows are always
`lucrum`, by construction — a candidate above the floor would have been
taken.

**The condensed report.** Columns with no gain and no flag collapse to
`sine lucro: …`; meet lists stop at the first non-positive increment
(`nulla`); fold lists longer than eight names become `cadit N ex S`
(column lines, chain lines, the conditioned block, the finis line);
each column with a non-zero gain gets `sortes: A +4 B +4 C -2`, or the
three counts when the folds exceed eight. One scratch array of fold
flags on the context serves every list.

**The self-describing file.** `RETICULUM-OPTIONES` is the first row of
machine output: every flag of the run, lists joined by `,`, conditions
by `;`, absences `-`. The recount prints the same from its own parsed
options, so the acceptance diff covers it.

**Gate** green on the first run: both expected files regenerated by
hand (the FINIS numbers rest on one fact — the meet of a column with a
constant column is the column itself), the recount equal to both, the
human laws (`sortes:` line, `recusatae:`, `ceterae`, `sine lucro`,
`nulla`). Plant: `>=` for `>` in the gaining-fold count, red on the
parity columns (three gaining folds where there are none), green after
the revert.
