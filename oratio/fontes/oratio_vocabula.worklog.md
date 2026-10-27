# oratio_vocabula worklog

## 2026-10-21 — vocabulary tables taken as const

`oratio_vocabula_creare` and `oratio_vocabula_creare_anglice` now take
`constans OratioVocabulariumLa*` / `constans OratioVocabulariumEn*`, and
the struct fields are const. Nothing in the library writes through them:
every callee (`_la_quaerere`, `_la_glossarium`, `_en_analysare`,
`_lemma_analysis`) already took const. The glossary is attached BEFORE a
vocabula is created (`oratio_vocabularium_la_glossarium_ponere` on the
non-const table). A test suite can therefore load the 26 MiB Latin table +
glossary + Moby once as a shared fixture, and each section builds its own
vocabula from it (credo v2, probatio_oratio_vocabula).

`oratio_vocabula_symbola` keeps no pointers into `nexus_tsv` (verb, first
path and identifier are copied into vc->piscina). So calling it once per
line-aligned chunk gives the same result as one call over the whole file.
The test suite relies on that to stream build/nexus.tsv (172 MB and
growing). The one per-call cost is the `tractatae` filter table, rebuilt
in vc->piscina on each call (~a few hundred KB).
