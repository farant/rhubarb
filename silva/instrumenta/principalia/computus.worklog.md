# computus — worklog

## 2026-09-02 — birth (RP §11)

The performance spec's first step: a tool that reports where a
parse's memory goes, so the four Part II constants can be measured
by the project rather than by the spec's author. `silva_computus_metiri`
lexes in one fresh arena and parses (closure inside) in another, both
with a fixed 1 MiB initial block, and reads piscina's counters
afterwards; piscina gained block and allocation counts for it. The
CLI wraps it with aedilis for the closure (`-nudum` skips it).

Numbers on this toolchain with closures: lib/stml.c 176 ms, 436 MB
handed out, 606 MB committed, 1.14 M allocations — the spec's
"allocator-bound" diagnosis in one line. The ratchet is visible in the
block sizes of the pinned corpus (1, 2, 3, 6 MB...).

Pin: `probatio_silva_computus` over four fixa/roundtrip files with
`-nudum`, golden `fixa/computus/basis.tsv`, exact on every
deterministic column, times printed only. Seen red first (no golden),
then written, then green twice. Regenerate with `COMPUTUS_SCRIBERE=1`
and say why in the commit — after the constants land, the golden is
SUPPOSED to change, and the diff is the measurement.

Two things learned: allocation counts depend on the title string's
length (the CLI with a path reported 21437 for base64.c, the probatio
with the bare name 21433), so the probatio pins bare names; and examen
accepted two signedness conversions clang rejects (`idx = -I` into an
i32, unsigned into s32), the class already filed as 01M1GB2MMJ.

## 2026-09-02 (later) — the optimization bench

Three tools so that each lever is an afternoon with a number: `-iter N`
on computus plus `silva/sample.sh` (macOS `sample` over a parse loop;
leaf table and folded stacks), `silva/aequivalentia.sh -basis|-conferre`
(the byte-identity differential over lib/*.c, headers pinned to the
baseline commit's text on disk under a trap while the newly built
binary runs), and phase clocks on SilvaParsura (lex / expansion / GLR /
commit, read by computus). First reading on stml.c at -O2: GLR 36.5 of
54 ms, lexing 6, expansion 4.4, commit 4.7. Combined with the sampler
(Xar and arena operations ~48% of leaf samples) the picture is: the
GLR loop's container traffic, not the automaton, is the cost.

## 2026-09-02 (evening) — three levers, one method

Each: min-of-7 computus before, one change, tests, min-of-7 after,
sample.sh, aequivalentia.sh -conferre, golden regenerated only when
allocation counts were meant to move, three shadow gates, commit.

- Arena peak walk -> running total: 53.2 -> 49.0 ms.
- xar_locare loop -> bit scan: 50.3 -> 43.1 ms (json 13.4 -> 11.8).
- GLR per-step scratch Xars reused: 45.3 -> 39.8 ms, allocations
  -20%, handed out 150 -> 106 MB. The surprise was the memory: each
  recreated Xar re-grew its segments from 4 every step, so the reuse
  also keeps the largest step's capacity.

Lessons: token anchors must include the comments inside a block
(extract the exact text instead); measure on a quiet CPU or the
number is noise; the basis of aequivalentia.sh should be refreshed
after every landing so "mutatae ipsae" stays empty.

## 2026-09-02 (late) — the table side

Two changes on the cooked-table side of the GLR, same proof kit.
Validation per parse was a 14-million-iteration pass over the
committed table on every GLR creation; it moved to the suite (which
had validated the skeleton and the small grammar but never the C89
table itself — fixed). Then the dense indices: the cook now writes a
state-by-terminal and a state-by-nonterminal grid next to the packed
rows, computed from the same rows as they are emitted; the engine
uses them when present and scans otherwise, and the validator checks
every cell. 38.2 -> 33.6 ms on stml.c. The pythonica transaction
wrote all four files and then refused its own judgment because the
not-yet-regenerated table lacked the new fields — the right order of
failure, and a reminder that a contract change and its regeneration
are one step.
