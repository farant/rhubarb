# Portae debitae — spec v1 (which gates does a change owe?)

2026-09-24 · decided with Fran in conversation (decretum …SQ2Q4C), for
desideratum …S4D7AZ; builds on inventarium v1 (`inventarium-spec.md`) and
its first inventory 'suitae probationum' (…EH76CR).

## 1. What it is

Given the files a change touches, name every gate (`PORTAE` entry) that
owes a run, **each with its reason**:

    gesta   compiles gesta/fontes/tabularium.c
            (gesta/probationes/probatio_tabularium.c)
    radix   runs gesta/instrumenta/tabulariumd_principale.c, whose
            closure holds gesta/fontes/tabularium.c
            (probationes/probatio_cliens_tabularii.c)

Motivating case: `dcd516c7` changed `gesta/fontes/tabularium.c`, was
gated on `gesta` only, and broke `probatio_cliens_tabularii` in the root
suite, whose test starts the daemon inside `system(...)`. Nothing in the
house could answer "I changed X; which suites do I owe?" except memory.

## 2. Coverage has three sources

Coverage is **derived on demand, never stored per file** (thousands of
rows, stale on every commit). The inventory holds only what the graph
cannot see.

**a. Compile coverage (derived).** A compiled suite's tests are the
runner's own glob, `<dir>/probatio_*.c`, with `<dir>` taken from
`SUITAE` in `pythonica/silva.py`. Each test's closure comes from
`bin/aedilis <test> --partes` (`O` sources, `C` headers, `V` vendor). A
suite owes a run when a changed file is in any of its tests' closures.
Measured 2026-09-24: all 392 test files resolve, 11 s with 4 parallel
workers, zero failures; no cache in v1.

**b. Runtime coverage (lens `currit binaria`, textus).** What a suite's
tests *run*: a comma list of paths. A `.c` path is an entry point — its
aedilis closure counts as covered; any other path (a script) counts
literally. Example: the `compile_tests.sh` row holds
`gesta/instrumenta/tabulariumd_principale.c, gesta/tabulariumd.sh`.
Proposed by a scan of the tests' `system(...)` / `processus_*` string
arguments (~12 real edges today; most matched strings are fixture data),
confirmed by hand. `fons` records which.

**c. Path coverage (lens `tegit viae`, textus).** Comma list of path
patterns (`fnmatch`, `**` = any depth) for inputs that are not C: the
17 of 31 gates that are fumus scripts, `pythonica/**` for the pythonica
gate, `lib/**` for `amalgamata`, fixtures, canons. By hand.

The old prose lens `tegit` is superseded by (b) + (c); it stays until a
lens-removal event exists (out of scope).

## 3. Reading the inventory from Python

A new read-only verb on the cold path, beside `-res` and `-mappa`:

    ./gesta/frigida.sh -inventarium <res>

prints one TSV line per cell, `ordo \t lens \t genus \t valor` (rows in
table order, then lenses in table order; empty `valor` for an absent
cell is NOT printed). It reuses the engine's own fold: Python never
re-folds ledger events. `silva.inventarium(res)` parses it through the
existing substitutable `_frigida_imperium` hook (tests fake it).

## 4. Surfaces

**`silva.portae_debitae(viae)`** → list of `Debitum(porta, causa,
manu)`, deterministic order (PORTAE order). `causa` names the path
chain; `manu` = the row's `manu tantum` is `ita` (a GUI script: owed,
but a person runs it). Files no gate covers come back separately as
`intecta` (information, never an error — docs are expected there; a new
`.c` that no suite reaches is a finding worth printing).

**`tools/portae_debitae.sh [viae…]`** — thin CLI; without arguments it
takes the working tree's changed and staged files. Prints the gates, a
reason per gate, manual gates, and uncovered files. Exit 0 always when
it judged; 2 = nothing judged (inventory unreadable).

**`silva.commissio`** — computes the owed gates for `viae` and **runs
them automatically**: effective gates = requested ∪ owed. Each gate it
adds is printed with its reason ("porta debita addita: radix — …"), so
the warning and the run are one line. Manual (GUI) gates are printed as
owed-by-hand, never run. Escape: `sine_debitis='<cause>'` (non-empty
cause required, printed) — the analogue of `--no-verify`.
`commissio_umbra` gets the same computation.

No refusal mode in v1 (Fran: warn first, measure false positives).

## 5. Acceptance

Replay `dcd516c7`: `portae_debitae(['gesta/fontes/tabularium.c',
'gesta/gesta.worklog.md', 'gesta/probationes/probatio_tabularium.c'])`
must contain `gesta` AND `radix`, radix's reason naming
`tabulariumd_principale.c`. Born red: before the `currit binaria` cell
exists, radix is absent.

## 6. Out of scope v1

A closure cache · refusal mode · removing the prose `tegit` lens ·
generated-source edges beyond what a glob in `tegit viae` states ·
automatic switch from `commissio` to `commissio_umbra` when the owed
set is long (v1 runs the owed gates in whichever the caller chose, and
prints the owed set before running).
