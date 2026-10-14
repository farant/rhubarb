# diurna worklog

## 2026-10-13 - born (analytica T4 step 2a)

Fran: "metrics and logs are a different thing (this might not be the
only application for them long term)". So log bodies are a library of
their own, not volumen massae (which are permanent by design - retention
must delete) and not thesaurus (generation-based purge and
verify-on-read are build-cache semantics). Model: Loki, not
Elasticsearch - a few low-cardinality labels (titulus, fractum), the
run id as KEY never as label, bodies as plain files. Spec:
project-specs/analytica-spec.md III-half and IV.

Layout under a root: corpora/II/LXII (SHA-256 of the stored bytes;
written to a pid-named temp then renamed, never rewritten when present
raw or .gz), index.tsv append-only (positum / fixum / compressum /
deletum, filum_appendere_firmiter). Body FIRST, index line SECOND: a
line never points at a missing body. Key is (clavis, seq) - seq 0 = a
run's whole body; resident/server streams later reuse it with seq 1..n.
Per-key decisions, per-body storage: compressum/deletum lines are
keyed by sigillum, and a positum after a deletum of the same body means
ponere rewrote it (fold resets it to CRUDUM). Reads fold the whole index
(hash by key + by body) - fine because purgare (next step) compacts it.
legere verifies the sha: a changed body is CORRUPTUM, never returned.

Traps hit: i32 is UNSIGNED - a backward `k >= 0` loop never ends and
segfaulted on xar_obtinere(NULL) (s32 now, commented); strtoll is not
C89 (strtol into longus, 64-bit on LP64); angle-bracket paths in
comments evoke STML tags (examen 'annotatio malformata') - rewritten
without brackets; a regex rename of `gz` also rewrote the ".gz" suffix
literals - restored by exact edit (exact edits only, again).

Not yet tested: the compressed read branch (nothing writes .gz until
purgare). probatio_diurna 7 sections, mixed order green; plants VI red
(sigil not of content, tail not cut, impure title accepted, sha check
off, pin not applied, momentum from the wrong field).
