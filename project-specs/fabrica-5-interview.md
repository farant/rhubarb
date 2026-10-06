# fabrica slice 5 - interview: the gate rollout

Ledger: desideratum …J6HF ("the remaining gates as `iudicium`
actions, after toml"); park …2VP7 (shadow passes) waits on it. Built on
fabrica spec 3 (`project-specs/fabrica-spec-3.md`, toml pilot, §XIV as
built) and effectus slices 1-3 (chain roots, `<argumenta>`, the oracle).

## 0. Data (2026-10-06, before any question)

**Where gate time goes** (`build/portae/tempora.tsv`, 2026-10-02 ->
10-06, 91 commits): 7.7 h of gate runs.

| gate | runs | total s | mean s |
|---|---:|---:|---:|
| pythonica | 18 | 6,357 | 353 |
| fabrica | 29 | 5,268 | 182 |
| aedilis | 21 | 3,626 | 173 |
| toml | 60 (+19 reused) | 2,408 | 40 |
| radix | 25 | 1,873 | 75 |
| generata | 13 | 1,730 | 133 |
| oratio | 13 | 1,324 | 102 |
| iudicium-fumus | 21 | 870 | 41 |
| crusta | 29 | 861 | 30 |

**The migrated gate is reused only 24% of the time** (toml: 60 real
runs, 19 reuses). Why a verdict re-ran is NOT recorded (`build/fabrica.db`
`cursus` keeps SANATUM rows with no cause; no IUDICIUM rows for
`porta_toml`) - so the reuse rate cannot be explained from history. One
cause is visible today: `bin/fabrica iudicare build/fabrica/verdicta/
toml.txt` -> `STALUM ... lectio transitus mutata: include` - the trace
holds a LISTING of `include/`, and today's merge added `matrix.h`,
`anulus.h`, ...; toml uses none of them. Since 10-02, 20 of 189 commits
added or removed a header in `include/` and 15 touched
`pythonica/silva.py` (a declared input of every verdict, spec 3 §XIV
"coarseness left on purpose"): roughly one commit in five voids EVERY
migrated verdict regardless of what it touched.

**Runner readiness** (effectus summary of each runner, today):

| gate | runner | processes | nulla | ignotum | work happens in |
|---|---|---:|---:|---:|---|
| pythonica | pythonica/probare.sh | 1 | 0 | 0 | Python (`probatio_silva.py`) - neither the C read ledger nor effectus sees it |
| fabrica | tools/fabrica_oraculum.sh | 17 | 92 | 15 | bash tree + C |
| aedilis | tools/aedilis_porta.sh | 1 | 0 | 0 | C (traced) |
| radix | compile_tests.sh | 15 | 87 | 49 | links all lib/*.c: key moves almost always |
| generata | tools/generata_probare.sh | 16 | 89 | 13 | bash tree + C |
| oratio | oratio/compile_probationes.sh | 8 | 75 | 12 | bash + C |

Most unresolved sites are `argumentum` (142 in fabrica's tree) - what
in-chain derivation with a declared argv binds (effectus-3 T5).

Correction of the obvious plan ("migrate the expensive gates in cost
order"): the coarse inputs every verdict shares would void most of the
new passes; and the most expensive gate's work is invisible to both
input sources.

## Questions

**DECIDED (Fran 2026-10-06): every recommendation (a), Q1-Q6.** On Q3
Fran added: Python in pythonica is unavoidable but is meant to be a THIN
wrapper over house/C functionality - whatever makes sense to fold into
C should move there; instrumenting the Python (audit hook) is right.

**Q1. Fix the shared coarseness first?** (a) Recommended: one task
before any migration - log WHY every verdict re-runs (the judge's
MUTATUM line into `cursus.causa`), then remove the two known voiders:
the `include/` listing (digest the names actually looked up, or the
listing per directory the closure includes from) and `silva.py` as an
input (split porta()'s machinery from the rest, or declare only the
functions a verdict depends on). Measure toml's reuse rate before/after.
(b) Migrate first, fix later. (c) Only add the logging now.

**Q2. Order of gates.** (a) Recommended: aedilis first (C, traced,
runner clean - closest to toml), then fabrica and generata (big bash
trees: the effectus work pays here), oratio; radix LAST or never (its
key moves almost every commit); pythonica separately (Q3). (b) Strict
cost order. (c) All at once as a batch job.

**Q3. pythonica (the most expensive gate).** (a) Recommended: a Python
read ledger - `sys.addaudithook` records every `open` in the test
process into the same trace format; subprocesses it spawns are house
binaries (traced) or scripts (effectus). Its own design step, after
Q2's first gates. (b) Declare coarsely (all of pythonica/ + every tool
it spawns). (c) Leave pythonica on tree-seal receipts.

**Q4. Per-gate checklist additions.** (a) Recommended: toml's checklist
(spec 3 §V: raw IO -> filum, getenv -> lectiones, bash-written inputs ->
actions, `porta_<g>` declaration, trace paths, plants) PLUS: the runner
as an effectus chain root with `<argumenta>` (pythonica cross-check);
`./crusta/effectus_oraculum.sh -domus <runner>` non tecta 0 at
migration (a tool step, not a gate - C14); census reports in-chain and
standalone unresolved separately. (b) toml's checklist only.

**Q5. How the batch runs.** (a) Recommended: one expeditio row per gate
(like W0ZBW / 'migratio ad sors'), each row = one commit with its
gate's plants; the shared-coarseness task (Q1) is a normal plan task
before the batch. (b) A plan with one task per gate.

**Q6. Shadow passes (park …2VP7).** (a) Recommended: after the batch -
it multiplies reuse only once passes survive ordinary commits. (b) Now.
