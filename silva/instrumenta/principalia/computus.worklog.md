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
