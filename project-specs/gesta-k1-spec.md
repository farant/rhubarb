# GESTA K1 + TABULARIUM — SPEC v1

Date: 2026-07-14. Status: v1 (pre-explorer; v2 lands after codebase
exploration per house ritual). Sources: tabularium-visio.md,
tabularium-gradus.md, gesta-k1-interrogatio.md (22 Q&A, all
decisions), ../bun-libraries/smaragda.ts + libraries.ts (TS oracle).

**NOMINA SEALED**: **gesta** = the general event-sourcing/entity
kernel (C89 library, gesta/ subsystem). **tabularium** = the first
tenant/instance (the monorepo's issue/doc/decision store) and its
MCP server namespace (mcp__tabularii__*). Res Gestae = the event log
itself.

## I. QUID SIT

K1 = the kernel's first milestone + its first consumer, shipped
together: append-only event log (tessellae) over scrinium/SQLite,
materialized entity state (res) via the native default reducer,
data-defined genus engine (genera themselves defined BY events),
generic link events, FTS5 search over materialized state, an MCP
server, a committed JSONL projection for git, and a one-session
harvest of the currently-open prose ledgers.

Independently valuable (house law): even if nothing else ever lands,
the monorepo gains a queryable issue/park/decision store with
full-text search and anchored notes, plus a general C89 event-
sourcing kernel reusable by future applications.

## II. DECISUS (from the interrogatio; numbers = its questions)

1. (Q1) Event log = ONE universal table; payload = one JSON column
   (house json lib); promoted real columns: id, res_id, genus_eventus,
   actor, origo, branch_id, seq. nuntium stays out.
2. (Q2) entitas_repositorium untouched; convergence = named pull
   post-K1.
3. (Q3+Q14) tabularium.db UNTRACKED; committed **annales JSONL**,
   appended one line per event AT WRITE TIME (O(1) — the log is
   append-only). Refold-from-JSONL == db is a standing bar.
4. (Q4) Surfaces: C library + MCP day one. **NO CLI twin.** Cold
   write path = #! colloquium script (Q9).
5. (Q5) Genus engine data-defined day one; tenant genus-lite = a
   seed script.
6. (Q6) Reducer identity is DATA in the genus definition:
   "ordinarius" = native default; K3 adds machinula resolution by
   source-digest. Fold version = reducer digest (visio §IV literal).
7. (Q7) Links = generic (res_a, verbum, res_b) events + a links
   fold. No health/constraints (K2).
8. (Q8) FTS indexes MATERIALIZED STATE only (titulus/corpus/tags),
   dirty-queue maintained. History via per-res timeline.
9. (Q9) Claude = autonomous scribe (no per-note confirmation;
   review-by-query). Colloquium #! fallback when resident down.
10. (Q10) Tenant genera + lifecycles (seed): quaestio (issue)
    apertum→laborans→clausum|relictum; parcum parcatum→tractum→
    clausum; decretum (stateless); nota (stateless); desideratum
    apertum→impletum.
11. (Q11) Tags free-form + seed vocabulary + census gardening query.
12. (Q12) Anchors: (genus_ancorae, target) + write-time sigillum
    content hash; read-time resolution via legati; failure =
    CAUTIO-style stale line + similitudo suggestions; standing
    unresolved-anchors query. Never blocks, never silently rebinds.
13. (Q13) Birth content = harvest of OPEN items only (~50-100
    entries from phase-log tails, MEMORY.md boards, worklog
    asperitates).
14. (Q15+Q21) Layout: gesta/ subsystem (fontes/, probationes/,
    phase-log.md, MCP server code, seed + fallback scripts).
    Instance data: tabularium.db untracked at root (or gesta/ —
    v2 explorers advise), annales JSONL committed.
15. (Q17) **Genera are events too**: definitio-generis/
    emendatio-generis events in the same log; the genera table is a
    fold; refold-from-empty rebuilds EVERYTHING including schema.
    (Deliberate upgrade over TS rows.)
16. (Q18) actor ∈ {fran, claude, machina} + free origo string
    (session id / hook / script path).
17. (Q19) MCP = five tools (§V). tabularium = second resident beside
    legatus; LEGATUSD convergence uncoupled.
18. (Q20) TS oracle = hand-derived goldens ('TS: smaragda.ts:NNN'
    comments); no bun in the build; divergences recorded.
19. (Q21) Retention K1 = keep everything.
20. (Q22) Sequencing: after spec-v2, K1 is the standing work
    (~4 chunks).

## III. ARCHITECTURA

**Pipeline**: scribere → tessella row + annales line → dirty marks →
folds advance by HWM watermark → res/nexus/genera tables + FTS
current → quaerere reads folds; res timeline reads the log.

**The log (tessellae)**: id = ULID (scrinium_ulid; creation-ordered
record identity per gradus §III resolution); seq = AUTOINCREMENT
(the HWM axis — monotonic across deletes); res_id ULID; genus_eventus
(creatio | mutatio | nota | status | nexus | denexus |
definitio-generis | emendatio-generis — v2 may refine); datum JSON;
actor; origo; branch_id (present, dormant, '' default).

**Folds = consumers with watermarks** (libraries.ts HWM insight):
each fold row in a consumptores table (titulus → last seq).
K1 folds: ① genera (from definitio events — BOOTSTRAP ORDER: this
fold advances first within any batch so later events validate
against fresh genera); ② res (default reducer: creatio sets genus/
titulus/initial datum; mutatio merges attribute JSON; status sets
status if the genus machine allows — violations recorded not blocked
[Hybrid note: v2 explorers check TS defaultReducer merge semantics
precisely]; nota appends to the notes stream — materialized as
corpus-visible text); ③ nexus (links table); ④ fts-dirty (dirty
queue → FTS5 upsert); ⑤ annales is NOT a fold — it appends at write
time in the same transaction (DECISUS 3).

**Refold**: drop all fold tables + watermarks, replay tessellae from
seq 0. Bar: byte/row-identical to standing tables. Second bar: fresh
db built from annales JSONL == the live db.

**Genus engine**: genus definition datum = { titulus, status_initialis,
machina: [(ex, ad)...], reducer: "ordinarius", schema_notae? }.
Validation posture K1 = record-don't-block (a status event outside
the machine emits a violation NOTE on the res, mirrors examen's
judgment-not-gate philosophy). Strictness = future knob.

**Anchors**: stored in res datum as [{genus_ancorae: symbolum|via|
commissio, scopus: "chorda_secare", sigillum: hex}...]. Resolution
read-time only (legati symbolum / file stat / git). Staleness =
resolved-but-hash-differs (content moved on) vs unresolved (target
gone) — both render CAUTIO lines, unresolved adds similitudo
suggestions.

## IV. SCHEMA v1 (sketch — v2 explorers refine DDL)

```
tessellae(seq INTEGER PRIMARY KEY AUTOINCREMENT, id TEXT UNIQUE,
  res_id TEXT, genus_eventus TEXT, datum TEXT/*JSON*/,
  actor TEXT, origo TEXT, branch_id TEXT DEFAULT '')
consumptores(titulus TEXT PRIMARY KEY, seq INTEGER)
genera(titulus TEXT PRIMARY KEY, datum TEXT/*JSON*/)          -- fold
res(res_id TEXT PRIMARY KEY, genus TEXT, titulus TEXT,
  status TEXT, datum TEXT/*JSON: attrs+tags+anchors+notes*/,
  creatum TEXT, mutatum TEXT)                                  -- fold
nexus(res_a TEXT, verbum TEXT, res_b TEXT)                     -- fold
res_fts(FTS5: titulus, corpus, tags)                           -- fold
sordidae(res_id)                                               -- dirty queue
```

## V. MCP — mcp__tabularii__* (legati playbook: few tools, capped
text answers, CAUTIO channel, doctrine in instructions field)

- **addere** {genus, titulus, corpus?, tags?, ancorae?, nexus?} →
  creatio (+optional immediate link events); returns res_id + echo.
- **gerere** {res, actus, ...} → append one event: nota{textus} |
  status{novus} | nexus{verbum, alterum} | mutatio{clavis:valor} |
  denexus. (One tool, actus-dispatched — keeps surface small.)
- **quaerere** {textus?, genus?, tag?, status?} → FTS + filters,
  capped list with res_id/titulus/status/tags per row.
- **res** {res|titulus} → full state + anchors (resolved, with
  CAUTIO lines) + timeline (capped), similitudo on miss.
- **census** {} → counts by genus × status + tag census + unresolved
  anchor count.

Registration: .mcp.json at root (legati precedent); gesta/
tabularium.sh -mcp launcher. Second resident ratified. Doctrine
must state the scribe contract (autonomous filing) so future
sessions inherit the workflow.

**Cold path**: gesta/scriptum/nota_frigida.c — #! colloquium script
linking gesta+scrinium: appends an event (and annales line) without
the resident. Officina charter dogfood.

## VI. TENANT SEED (tabularium)

Seed = a colloquium script (or C seed unit) that APPENDS EVENTS
(definitio-generis ×5 + tag-seed notes) — no schema migrations
beyond gesta core (smaragda's own TODO-regret honored: core minimal,
tenant composable). Genera per DECISUS 10. Tag seed: lib names +
standing vocabulary (vectis, lsp, mcp, perf, corpus-law, ...).

Harvest session (chunk D): Claude reads phase-log tails + MEMORY.md
boards + worklog asperitates, files open items with anchors +
origo="messis-2026-07". Cross-check bar below.

## VII. BARRAE (K1 seal)

1. Refold-from-log == standing folds (row-identical); refold from
   annales JSONL == db.
2. TS-golden credo suite green (reducer semantics, HWM advance,
   genus fold bootstrap order, link fold; 'TS: smaragda.ts:NNN'
   comments at each hand-derived expectation).
3. FTS finds planted notes by prose, by tag, by anchored-symbol
   name; census matches.
4. Anchor demo: rename a fixture symbol → res view shows CAUTIO +
   similitudo suggestion; unresolved-anchors query lists it.
5. THE LEDGER BAR (gradus): "what's outstanding for legati?"
   answered by ONE quaerere/census call, cross-checked against the
   hand-compiled 2026-07-14 ledger.
6. Suites: gesta probationes green under house flags; existing
   suites untouched; excubitor/fabrica rows for the new pool.
7. Live shakedown: resident registered, five tools exercised in a
   real session; scribe files a real debrief note autonomously.

## VIII. CHUNKS (~4, officina cadence: INTENTIO → build → RELATIO)

- **A — gesta core**: schema migration, tessella append + annales
  line (one transaction), genera fold + bootstrap order, res fold
  w/ default reducer, refold; TS-golden suite.
- **B — folds infra**: HWM consumptores, nexus fold, FTS5 +
  dirty queue, census queries, anchors (sigillum hash at write,
  resolution walk at read w/ legati... NB: read-time resolution
  lives in the MCP layer where legati is reachable — kernel stores,
  server resolves; v2 pins the seam).
- **C — surfaces**: MCP server (tabellarius framing, five tools,
  doctrine, CAUTIO channel), .mcp.json, launcher, colloquium cold
  script, tenant seed.
- **D — harvest + seal**: open-items harvest, ledger bar, live
  shakedown, RELATIO + debrief.

## IX. DEPARTURES FROM TS (recorded upgrades)

- Genera defined via EVENTS, not rows (total refold).
- Anchors carry write-time content hashes; resolution is read-time
  through a live index (legati) — smaragda's slug anchors upgraded.
- Committed JSONL projection (git carries history).
- actor + origo split.
- No sync/HTTP; single machine, single writer (WAL, busy_timeout).
- Latin surface; C89 under house flags.

## X. OPEN FOR SPEC-V2 (explorer questions)

1. scrinium API sufficiency: transactions (BEGIN/COMMIT surface?),
   FTS5 wrapper needs, prepared-statement reuse, anything K1 must
   add to scrinium itself.
2. House json lib exact surface (name, read/write, escaping) —
   JSONL emission + datum parsing both ride it.
3. Legatus -mcp pattern extraction: what's copyable (tabellarius
   framing, dispatch, doctrine, launcher, fabrica/excubitor rows,
   vigilia?) vs legatus-specific; cost of the second resident.
4. TS semantics distillation for goldens: defaultReducer merge rules
   (deep vs shallow, deletes, arrays), tessella column exactness,
   HWM/dirty-queue mechanics, FTS config — with line references.
5. Colloquium #! + -plagulae mechanics for the cold script (which
   libs link; does gesta amalgamate or stay units at K1?).
6. Anchor resolution seam: MCP-layer resolution calling legati as a
   CLIENT (resident-to-resident?) vs reading nexus.tsv directly vs
   shelling nexus.sh — pick the honest v1.
7. db + annales file placement final call.
