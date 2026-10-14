# analytica - run history for every job (spec)

Fran 2026-10-13 (vision …QB5SQ): a repo analytics layer - every script or
logical job has a history (how often, how long, outcome, commit, when it
last ran), standard logs with exit codes and rotation; eventually every
script carries the chassis and a script without it is a lint failure,
adopted gradually while the API settles. Census:
`analytica-census.md`; decisions: `analytica-interview.md`.

## I. Goal and acceptance

One run record, one home, one query surface. **Acceptance = the six
questions actually asked during cursores ad gradus, each answered by a
named query** (census §II):
1. where did a job's time go (nested steps);
2. which upstream generators re-run on every judge;
3. did this commit make X slower;
4. when did frigida last pass, in which tree and branch;
5. which scripts never run;
6. what did X print the last time it failed.

## II. The run record (D1)

A run (*cursus*) = anything with a start, an end and an outcome. Fields:
`id`, `parens`, `sessio`, `titulus` (Q1: chassis title, else root-relative
script path; fabrica actions keep their titles; gates `porta:<nomen>`,
commit phases `phasis:<nomen>`), `argv` (recorded, not identity),
`initium` (wall clock, ms), `duratio_ms` (monotonic), `exitus` (code),
`status` (transiit | fractus | interruptus), `vocator` (Q5: homo |
commissio | uncus | fabrica | frigida | agens), `commissum`, `sordidum`,
`ramus`, `arbor` (Q6 + Fran: computed once per top-level session,
inherited by environment), `machina`, rusage (max RSS, user/sys CPU),
`acta` (log body reference + size).

Two journal lines per run - **initium** at start, **finis** at end - so
a run that never finished (killed, hung, crashed shell) is visible as
`interruptus` instead of missing. Journal first, as mensura already does:
one atomic append per line (under PIPE_BUF), folded into the volume
later.

Home (Q2): the mensura volume `~/.rhubarb/mensurae.volumen`, shared by
every tree. The run API lives in lib/mensura (runs are a layer of
measurement, not a new library); measurements inside a run attach to it
via `parens` as today (Q4).

## III. Producers (D2, Q3)

- `tools/cursus.sh` (chassis v1, Q10): a sourced prologue - opens the
  run (initium), sets `trap EXIT` to write finis with the exit code,
  captures stdout/stderr to a temp file OUTSIDE the tree (lesson of the
  entry points: never write into the tree while things run) and stores
  it at the end. Identity and version from the annotation
  `# <cursus titulus="..." versio="I"/>` in the script header.
- `bin/mensor cursus incipere|finire ...` - the C side of the prologue
  (and of anything else that wants to record a run).
- silva.py `_tempus_notare` and `porta()` also emit run records
  (`porta:<g>`, `phasis:<x>`); tempora.tsv stays for now as a cache.
- bin/fabrica actions also emit run records (parens from the
  environment), alongside fabrica's own `cursus` table (which its
  estimates need).

## III½. Three kinds of record (Fran 2026-10-13)

Fran: "a clear distinction between logs and metrics ... elasticsearch
type stuff for logs and prometheusdb type stuff for metrics". The field
splits three ways (OpenTelemetry's spans / logs / metrics), each with
its own engine, and so do we - ONE STORE PER KIND, joined only by the
run id:

| kind | ours | model | unit | retention | hard rule |
|---|---|---|---|---|---|
| spans (traces) | `cursus` in the mensura volume | Jaeger / Tempo / OTel | id, parens, sessio (= trace id), titulus, start, duration, status, attributes | metadata forever (cheap) | children carry the parent id |
| logs | `diurna` (new library, §IV) | Loki, NOT Elasticsearch | a body (run output) or, later, a stream of chunks | by age: full -> gzip -> deleted; failures, last-per-job, pins kept longer | index only low-cardinality labels; the run id is the KEY, never a label; search = scan bodies inside a label/time window |
| metrics | none yet - DERIVED from spans at query time (durations by commit, failure counts, regenerations) | Prometheus / RRD | sampled number + labels | downsampled (lossy on purpose) | label cardinality low |

`mensura` measurements stay what mensura.h says they are: discrete,
lossless, attached to a run (span events), NOT a time series. A real
metrics store is the parked Metrum (…AMZVFN) and becomes due with the
first continuously SAMPLED series (a resident's RSS every N s, disk use
over months) - not before.

## IV. Logs and retention (D3, Q7, Q8, Q11)

- DECIDED 2026-10-13 (Fran: "metrics and logs are a different thing
  ... a solution tailored to their use case"): bodies do NOT go in the
  volume - volumen massae are permanent by design and retention must
  delete. A log library `diurna` (acta diurna) owns them: files under a
  root (`corpora/<2>/<62>` SHA-256, atomic write, dedup; `.gz` after
  compression), an append-only `index.tsv` of `positum / fixum /
  compressum / deletum` lines keyed `(clavis, seq)` - seq 0 for a
  run's whole body; streams of chunks (residents, servers) later reuse
  the same key without a format change. Retention decides per KEY,
  storage is per BODY: a deduplicated body is compressed or deleted
  only when no key still needs it fresher. A deleted body keeps its
  index line ("deleted on <date>", not "not found"). Compaction of the
  index under a file lock in purgare. Policy is a struct the caller
  fills (analytica: from STML); diurna knows no mensura and no volume.
- (superseded) Bodies stored as volume files over content-addressed
  `massae` (SHA-256 dedup: identical quiet logs = one blob). One
  function stores a body - the SEAM where redaction plugs in later; the
  chassis never writes the environment wholesale.
- Size cap per body (8 MB, tail kept).
- Retention as data (one STML file): metadata forever; bodies full 7 d,
  gzip (flatura) to 30 d, then deleted; failures full 30 d, gzip to 90 d;
  the last body of every job forever; pinned bodies (run id named in the
  ledger or a worklog) forever.
- `bin/mensor purgare [-siccum]` applies it, opportunistically at most
  once a day after a journal fold, itself a chassis run.

## V. Queries (D4, Q9)

Named queries with parameters, in C, no query language:
`bin/mensor quaestio <nomen> par=valor ... [-machina]` - `ultimum`
(last run of a job), `historia` (runs of a job), `tendentia` (duration
by commit), `numquam` (jobs in the coverage universe without runs),
`fracta` (failing runs), `acta` (a run's log), `filii` (a run's tree of
steps), `discus` (body bytes by job). pythonica: `silva.cursus(nomen,
**par)` returning rows (thin wrapper).

## VI. Not in this slice

The lint and its debt table (phase III of the vision); the
`processus_exsequi` / `silva._curre` chokepoint harness beyond what III
lists; redaction; dashboards; folding credo verdicts and test_logs.

## VII. Done means

The six questions answered by named queries on real data (a test each);
chassis v1 on the pilot (the four converted entry points, frigida, one
hook, one gate script) with a lint stage that requires adopters to carry
the current version; retention applied with a dry run shown; fold and
cleanup measured (the instrument must not slow what it measures).

## AUDIENDA (not verified)

- volumen's file API for many small bodies (one transaction per body)
  under concurrent writers - not measured.
- Whether every journal writer today is folded (`mensor condere`) - the
  census could not confirm it per runner.
- rusage availability in bash (`times` vs a C wrapper) - to settle in T2.
