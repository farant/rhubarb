# fabrica — spec v1 (aedificatio domus: artificia, ingressus, iudex)

*2026-09-29. Written from the interview of the same day
(`fabrica-interview.md`, Q1–Q32, Fran's answers) and the ledger park
01KZYN4VPZ (2026-08-13/25). Question that prompted it: after a day
of tool-debt fixes, Fran asked about test run times and whether the
build/test organization was "conceptually messy even if it's
working". The measured answer: the GATE layer is coherent; the
PRODUCING layer is not — every script both decides and acts, each in
its own way. This spec is v1: design from the conversation. v2 adds
the codebase pass (file:line, measured commands) before any plan.*

*Discipline note: PULL, not checklist. Every capability here
graduates when a consumer needs it. Granular inputs and comment
opt-ins in particular are DEFERRED until a real case earns them
(Q22) — the model leaves room; nothing is built for them in slice 1.*

## I. Why — what was measured

| finding (2026-09-29) | cost / consequence |
|---|---|
| 15 `build/` dirs, 1,737 objects, 5.5 GB | `piscina.o`, `chorda.o` each compiled 24× |
| `frigida_probare.sh`: 14 suites, serial | ~14 min cold (briar 248 s, radix 153 s, silva 147 s) |
| `generata` 381 s, no stage timings | owed by most `include/` edits |
| `porta_vetustatis` run twice in stage VI | 31 of 42 s (fixed same day) |
| ≥6 notions of "stale" | mtime (`newest_header`, ~50 `find`s), existence (`silex_struere`), none (`bin/manus`, 6 weeks), per-stage content compare (generata I–VI), natura exit 3, content-hash keys/receipts (briar, gates) |
| stale installed binaries | `bin/manus` (6 weeks, wrong hint), spectator (fumus XII false alarm) |
| manual rebake ritual | `registrum` → `briar_struere` → `spectator_struere`, ~15× in one session |

The common shape (the park's words): **"successful build, stale
artifact, no error."** One concept is missing: an artifact that knows
what it was built from.

## II. Model

- **Artificium** (artifact): a produced file — in `build/`, committed
  (amalgams, generated sources), or installed outside the repo
  (`~/.bin/briar`) (Q11).
- **Ingressus** (input): anything with an IDENTITY and a DIGEST
  (SHA-256, `sigillum.h`). Input KINDS are modular (§IV).
- **Actio** (action): `{ingressus} → {exitus}` through a typed KIND
  (Q2) with named parameters — compile, link, generate, run a test,
  run a script (fixed arguments), install; later `fenestra` (GUI,
  exclusive, Q32). No templating, no expressions (Canon).
- **Involucrum** (envelope): what the layer does around EVERY action —
  timing, captured logs + last verdict line, exit contract 0/1/2,
  deadline, lock, declared outputs verified to exist then digested,
  record written (Q2, pre-interview).
- **Memoria** (record): "output O was built from inputs with digests
  D" — sqlite in `build/` (Q8), local and disposable.
- **Provenientia**: how an artifact's origin is known WITHOUT a
  record — modular per artifact (Q3, Q17): regenerate-and-compare
  (committed files) or self-report (installed binaries, §VI).
- **Stale** = today's input digests ≠ the recorded (or reported)
  ones. Never mtime. Never "exists".

Layering (Q12): **aedilis** stays the pure dependency oracle
(closures, manifests). **fabrica** is a new library above it:
declarations, queries, planner, records, envelope. The planner is
PURE (graph + digests → plan); a thin executor acts ("the engine
judges, a thin layer acts" — park 01KZYN4VPZ).

Name (Q20): **fabrica** (`lib/fabrica.c`, `include/fabrica.h`,
`bin/fabrica`). briar's partition module keeps `briar_fabrica`; the
prefix disambiguates.

## III. Declarations

Per subsystem (Q13): `silva/fabrica.stml`, `briar/fabrica.stml`, …
next to the code they build; a root `fabrica.stml` lists the
subsystems. STML, judged by a canon dialect like every house format.
Sketch (element and attribute names are placeholders — Fran names):

```xml
<actio nomen="manus" genus="installatio">
  <ingressus genus="manifestum" radix="tools/manus_instrumentum.c"/>
  <exitus via="bin/manus" provenientia="relatio"/>
</actio>
<actio nomen="amalgama_silva" genus="generator">
  <ingressus genus="manifestum" via="silva/amalgama/fontes_generata.h"/>
  <exitus via="silva/amalgama/silva.c" commissum="verum"
          provenientia="regeneratio"/>
</actio>
```

Sources feed declarations through comments (pre-interview) by
**pull** (Q5): a comment states a PROPERTY; a declaration's query
selects by property; sources never name actions. A property no query
selects is **warned** (Q6). The vocabulary is **deferred** (Q22).

## IV. Inputs

| kind | digest of | slice |
|---|---|---|
| file | bytes | 1 |
| manifest (aedilis) | ordered digests of the closure's files | 1 |
| flags / tool | flag string; a tool binary's own digest | 1 |
| literal query (`bin/fabrica quaere '…'`) | ordered digests of the result set | 4 |
| silva / stml selector view | canonical text of the matched nodes | on need |

**Views are per CONSUMER, not per file.** The amalgam reads header
comments; a compile may not. Bytes are the default; a narrower view
is an explicit opt-in, and only where digest cost ≪ action cost (a
six-minute gate, not a 50 ms compile). A too-narrow view is a silent
stale build — the one failure this layer exists to prevent.

**Knowing an action's inputs — three tiers (Q7, Q9, Q29–Q30):**

1. **aedilis-generated builds**: inputs straight from the manifest
   (`bin/aedilis tools/x.c` already writes it).
2. **Hand scripts, the query idiom**: file lists come ONLY from
   `X="$(bin/fabrica quaere '<literal>')"`. A crusta lint enforces:
   the argument is a literal; every compiled/read path comes from such
   a variable or is itself a literal. The fabrica evaluates the SAME
   literal query, so script and judge cannot disagree.
3. **Anything else**: refused with the line named (Q9: "see how it
   works in practice and adapt"), or given an explicit declared input
   while it waits for migration.

A bootstrap script builds `bin/fabrica` first (Go's `make.bash`
pattern: bash holds sequence, never knowledge).

Query target (Q14): OPEN. Module-owned indices that are themselves
artifacts are the candidate; decided by the first real queries.

## V. The judge — slice 1a

Read-only. Builds nothing (Q4).

**Families registered (Q21):** installed binaries (`~/.bin/briar`,
`briar-spectator`, `bin/manus`, `bin/aedilis`, `bin/natura`,
silex …), committed amalgams (silva, tessera, officina), generated
sources (aedilis launcher snippets, the C89 lexicon, the latina.h
numeral section, capsulae).

**Provenance per family (Q17):**

| family | provenance | cost |
|---|---|---|
| installed binary | `-provenientia` self-report (§VI) vs today's input digest | ~ms |
| committed amalgam / generated source | regenerate into scratch, compare bytes | 0.2–4 s warm |
| neither available | `IGNOTUM: sine provenientia` — never "current" | — |

**Two modes (Q25):** `celer` (self-reports, digests, records; no
regeneration; target < 2 s) for session start and hooks — regenerable
artifacts are reported `non iudicatum (celer)`, never "current";
`plenus` (regenerate and compare) for commissio and on demand.

**Output:** one line per non-current artifact with the reason (which
inputs changed, or how many lines differ), then **the healing plan in
dependency order** (Q16) — e.g. objects before briar before fumus —
as runnable commands. **Orphans (Q24):** outputs under `build/` no
declaration produces, with sizes; never deleted by the judge. Exit
0 all current · 1 something stale or unknown · 2 nothing judged
(house contract).

**Surfaces (Q19, proposed — Fran trims):** `bin/fabrica` CLI;
session-start hook (celer); `silva.commissio` (plenus, names stale
artifacts the change affects); fumus scripts refuse a stale installed
binary; later a legati/tabularii tool.

## VI. Self-report convention — `-provenientia` (Q18, Q28)

Every installable house binary answers `-provenientia` with a fixed
format, its digest written in at build time by the install action
(the pattern `briar_aedificatio_fontes` already uses for `-versio`):

```
provenientia 1
artificium bin/manus
ingressus <SHA-256 of the action's inputs>
commissum <hash>[ SORDIDUM]
```

The judge recomputes `ingressus` from the declaration and today's
tree; unequal = stale, naming the changed inputs when records allow.

## VII. Executor and envelope — slice 1b (Q26)

A thin executor runs the healing plan inside the envelope (§II):
timing recorded per action (the stage-timing table the tool-debt work
wanted, for free), logs in one schema under `build/`, exit contract,
deadlines via `processus_exsequi`, outputs verified and digested,
records written. The rebake ritual becomes one command.

## VIII. Records

sqlite in `build/fabrica.db` (Q8, vendor sqlite): per action run —
input digests, output digests, duration, exit, verdict line, time.
History kept (timing trends). Losing it costs a rebuild or a
provenance pass, never correctness.

## IX. Parallelism

Separate processes, never threads (house law). Two actions conflict
iff declared outputs or scratch directories overlap (Q10); otherwise
parallel up to a process limit. `fenestra` actions are exclusive of
each other and run only when explicitly asked (Q32).

## X. Gates as actions, and migration

Gates become actions (Q1): a verdict is an output; receipts become
records; "owed gates" becomes "stale actions downstream of the diff".
Sequenced AFTER test runners (Q27). Existing bespoke checks
(`porta_vetustatis`, generata I–VI, `newest_header`, natura exit 3)
become **oracles** — a gate asserts the fabrica's verdict agrees —
and are deleted once agreement holds on real history (Q15).

## XI. Slices

| slice | delivers | oracle |
|---|---|---|
| 1a judge | declarations for three families, provenance, two modes, plan, orphans, `-provenientia` on installables | generata I–VI, porta_vetustatis, briar `-versio` |
| 1b executor | envelope, timings, records; rebake = one command | the `*_struere.sh` scripts it replaces |
| 2 test runners | one subsystem's runner on the fabrica; then the shared object store in `build/aedilis/obiecta/` on digests (Q27, Q31) | the old runner, byte for byte |
| 3 gates as actions | receipts → records; owed gates → stale actions; parallel gates | `portae_debitae` |
| 4 query idiom | `bin/fabrica quaere`, crusta lint, script migration (tier 2) | hand lists / snippets they replace |
| on need | granular views, comment opt-ins (Q22), briar convergence (Q23) | — |

## XII. Decisions (Fran, 2026-09-29)

Q1 gates become actions · Q2 typed kinds · Q3 provenance modular ·
Q4 judge first · Q5 pull · Q6 warn orphans · Q7 static derivation ·
Q8 sqlite in build/ · Q9 refuse unresolved (adapt in practice) · Q10
parallel from outputs · Q11 installs covered · Q12 atop aedilis · Q13
per subsystem · Q14 query target OPEN · Q15 oracle then delete · Q16
ordered plan · Q17 provenance, never assumed · Q18 self-report · Q20
name kept · Q21 three families · Q22 vocabulary deferred · Q23 briar
later · Q24 report orphans · Q25 two modes · Q26 executor in 1b · Q27
test runners next · Q28 `-provenientia` · Q30 three tiers · Q31
digests in `build/aedilis/obiecta/` · Q32 exclusive `fenestra` kind.

## AUDIENDA — not verified (v2's job)

1. The aedilis manifest format: does it hold every input an install
   needs (flags, generated sources, the corpus that briar embeds)?
2. Determinism of every regenerable family (amalgams measured
   byte-identical; lexicon, snippets, numerals, capsulae not).
3. Whether `celer` < 2 s is reachable (count of installables × cost
   of `-provenientia`, digests of their closures).
4. How the install action writes the digest INTO a binary without the
   digest depending on itself (briar's `fontes` hash is the model —
   measure how it is computed and linked).
5. vendor sqlite under house flags (silex links it; compile rules).
6. crusta's ability to find literal `bin/fabrica quaere` calls and
   variable provenance of paths (tier 2's lint) — measure on a real
   script.
7. Name collisions: the old `fabrica.tsv` named in the aedilis
   interview; `briar_fabrica`; any `fabrica` identifiers in lib/.
8. What `portae_debitae` and gate receipts look like internally, for
   slice 3's mapping.
