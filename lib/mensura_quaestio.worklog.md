# mensura_quaestio worklog

## 2026-10-15 — empty results name their cause (cursus_quaerere_similes)

**Friction.** A query whose title filter matched nothing printed nothing and
exited 0 - identical to "this job has never failed". Bit three times in one
session: `historia titulus=probatio_tcp_servus` (the job is
`probationes_radicis/probatio_tcp_servus`), `tendentia titulus=...` (same),
`limes=5` used as a row limit (it is lentescentes' per-mille threshold; rows
are `n=`). An agent checking "is this flaky?" would have concluded "never
failed" from silence.

**Library.** `cursus_quaerere_similes(index, filtrum, maximi, &noti, p)`:
`noti` counts runs matching the TITLE filter alone (titulus/praefixum; status,
post, nota, arbor, ramus ignored) - so "known title, other filters excluded
it" is told apart from "never recorded". If noti == 0: distinct titles that
contain the query, or whose last segment (after `/`, `#`, `:`) equals the
query's, ranked by run count then title, capped at `maximi`. NIHIL when there
is no title filter. Tests: section XII of probatio_mensura_quaestio (bare
name -> 3 similar, ranking + cap, segment-only match where the query is NOT
a substring, known-but-filtered, nothing similar, no filter). Plants: drop
the segment rule (16/17), count noti with the full filter (15/17).

**mensor.** One hook at `_quaestio`'s common exit (`ordines` empty ->
`_nihil_congruit_nominare`), on STDERR and in HUMAN mode only - `-machina`
stays silent and the exit code is unchanged. First attempt printed in both
modes; mensor_cursus_fumus XI refused it (`sectiones titulus=nemo -machina
2>&1` must be empty - consumers often merge stderr). XI now also asserts the
human hint (`titulus=xi` -> `simillimi: porta:xi (n=1)`, the ':' segment
rule); plant (hook removed) -> XI red. `ordines` now starts
NIHIL (it was read at the exit; not every branch assigns it). `mensor
quaestio` without a name, or with an unknown one, prints a usage table: each
query's question, the parameters (`n=` vs `limes=`), and the title shapes.
Spec analytica §V gained worked examples ("how to ask").
