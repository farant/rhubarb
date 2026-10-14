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

## 2026-10-14 - retention: diurna_purgare (analytica T4 2c)

`diurna_purgare(d, &regula, nunc_ms, siccum, &relatio)`. "now" is a
parameter, so retention tests are deterministic ("now + 40 days").
Per KEY a needed form: pinned or last-of-its-title (ultimum_servare)
-> raw forever; failed keys use their own (longer) tiers; else age vs
crudum_ms / compressum_ms. Per BODY the freshest form any key needs
(CRUDUM < COMPRESSUM < DELETUM as an enum order). Actions only move
forward - a compressed body is never re-expanded (legere inflates),
so a later pin only stops further steps. Compression: gzip (flatura)
to .gz via temp + rename, then the raw file goes. Deleted bodies keep
their key's line until `oblivio_ms` after deletion; compaction then
drops the key (legere: IGNOTUM - the run record still has the hash
and the note).

One lock (index.sera, flock) for EVERY writer: ponere now holds it
across "body exists? / write / append" and figere across "key exists?
/ append"; otherwise a purge could delete a body between ponere's
existence check and its index line (a line pointing at nothing), or
compaction could drop a key figere just checked. Compaction writes the
live state (last positum per key, fixum, then body-form lines AFTER all
positum lines because the fold reads in order) to a temp file and
renames it. Not tested: real concurrency (two processes racing) - the
lock is structural, its absence would not show in a single-process
suite.

Two tests strengthened BY REASONING before planting: XII first had the
fresher (failed) key second, so "last key wins" would have passed as
"freshest wins" - now the fresher key comes first; and the compressed
read path finally has a test (VIII: legere after compression).

Policy is data: tools/analytica_retentio.stml (sana 7/30, fracta
30/90, ultimum servare, oblivio 365 - interview Q8). `bin/mensor
purgare [-siccum] [-machina] [-regula via]` (else MENSOR_RETENTIO,
else tools/analytica_retentio.stml from cwd); every element is
required - a missing one is refused by name, exit 2 (no compiled-in
defaults). Not yet: the opportunistic once-a-day trigger after a fold
(spec IV) - belongs with T6's adoption.
Plants VIII red (pin ignored, failed tiers ignored, last key wins, first-of-title kept, dry run deletes, oblivio ignored, servare inverted, missing element silently accepted).
