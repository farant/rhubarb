
## 2026-10-31 — instrument closures compiled in one --plures call (budget B2 H5)

`cursor_instrumentum_struere` and `cursor_fontes_compilare` started one
`bin/compilator` process per object. Each start costs ~32 ms (process
plus thesaurus and clang-identity setup), so a 21-object cache-hit
closure took 0.68 s before the binary even ran. pythonica's
Prosa/Oratio groups are almost all this wrapper overhead (30 wrapper
calls = 39.3 of 39.4 s, batch H). Both now write a vocationes file
(compile_tests.sh line format: flags + `-c src -o obj`, no spaces in
paths or flags) plus the same mandata.tsv lines in the same order, and
make one `--plures` call. The "binary newer than every object" link
rule is unchanged, and compilator still doesn't rewrite identical
objects. Measured: `md/extenta.sh FONTES.md h1` warm 0.18-0.21 s
(analyst baseline 1.0-1.07 s).

`--plures` keeps going past a failing line (exit 1, clang names the
source). The `|| return 1` after it is load-bearing: without it a warm
wrapper links the PREVIOUS object silently, which before only showed up
cold. New stage VII in tools/instrumenta_nexus_fumus.sh (tegit now
includes cursor_communis.sh): build extenta in a temp dir, a repeat call
links nothing, and a failing compilator returns FALSUM with nothing
linked. Plant (drop `|| return 1`) → FRACTUM.

Gotcha found while writing VII: macOS /bin/bash 3.2 `-nt` compares
whole seconds, so objects and binary written in the same second count
as stale and the next call relinks once (documented in
cursor_instrumentum_struere: "aequalitas secundi = stalum"). The test
lets one call settle before measuring.
