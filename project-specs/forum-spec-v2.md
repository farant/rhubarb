# Forum — spec v2 (2026-07-22, post-exploration)

*Delta over forum-spec.md (v1 holds for vision/entities/culture/
bar). Three explorers mapped the seams: genus seeding, app wiring,
daemon. Every claim below carries their file:line evidence; two
synthesis wins reshape the plan (§I).*

## I. Two synthesis wins

**1. The line-framing gap dissolves via fdopen.** No line-framed
TCP reader exists in the house (only lexers + hospitium's HTTP
parser scan '\n'). But every CLIENT in this design can be
blocking — so both the fori proxy and the app's daemon client
simply `fdopen` their socket and reuse
`tabellarius_lineam_legere/_scribere` verbatim. Zero new framing
code anywhere. (fdopen has no repo precedent — instrumentum-only
territory, like dirent; note separate FILE*s for read/write.)

**2. Per-request connections make the sequential daemon
sufficient for v1.** The daemon's cheapest correct form (per
explorator-daemonis) is blocking sequential accept: fdopen the
accepted socket and run the EXISTING per-line loop on it to EOF —
the stdio path with a socket FILE*, zero reactor. That fails only
if clients hold connections open. So the v1 CLIENT CONTRACT is:
connect → send line(s) → read response(s) → close, per call.
Localhost connect cost ≈ nothing (app polls at 5/sec). The
reactor multiplex upgrade (hospitium is the mapped exemplar:
per-connection piscina slots, LEGENS/SCRIBENS, write queues —
hospitium.c:56-108, 671-837) is the NAMED FOLLOW-UP when
long-lived connections matter.

## II. Daemon: tabulariumd (exact changes)

The machine is already transport-clean: `tabularium_currere`
(tabularium.c:5091-5137) = per-line loop over
`tabellarius_lineam_legere` + `tabularium_tractare(t, pn, corpus,
effusio)`; one shared `Tabularium* t`, fresh 64KB pn per message.

**The ONLY machine change: the initialize gate.** Today
`_initialize_tractare` (tabularium.c:4849-4908) is single-shot
(`initiatum` guard :4852) and is where gesta_aperire + plicare +
seminare + projections run. Change: make initialize IDEMPOTENT —
if already initiatum, return the same success response instead of
"iam initiatum" (~5 lines). Daemon then pre-initializes t once at
startup; every client may handshake harmlessly; no proxy shim
needed.

**Daemon main** (new gesta/instrumenta/tabulariumd_principale.c or
a mode of the existing principale): args -scrinium/-annales/
-portus (the wrappers are the ONLY place paths are hardcoded —
tabularium_principale.c:46-52; the machine copies every path from
cfg, tabularium.c:5065-5078). Startup: tabularium_creare +
self-initialize + `tcp_servus_creare(portus)` (BLOCKING mode) →
accept loop → fdopen(conn,"r")/fdopen(conn,"w") → per-line loop to
EOF → close → next. Port taken at bind = another daemon lives →
exit 0 quietly (start-if-absent race resolves itself).

**Forum world config**: via_scrinii=forum.db (build-side),
via_annalium=gesta/annales/forum.jsonl (COMMITTED),
via_nexus/identitatum/citationum/tabulae/entitatum = NIHIL
(dev-ledger projections; NIHIL explicitly honored,
tabularium.c:5069-5078). Note: the entities/ projection is
fully generic (tabularium.c:2180-2252) — enabling it later gives
one-file-per-articulus markdown, a natural on-ramp for the static
export future.

**Launcher gesta/tabulariumd.sh**: copy tabularium.sh's build
(true-closure -MM staleness, vigilia signum/manifest trio,
tabularium.sh:43-133); exec with -portus + forum paths.
Start-if-absent glue (NO precedent exists — new, small): clients
try tcp_connectere; on refusal spawn launcher detached, retry
with backoff. SO_REUSEADDR is default-on so bind fails only on a
live listener (tcp_posix.c:580-583).

**frigida**: nota_frigida has board paths compiled in
(nota_frigida.c:14-16) — forum cold path = v1 SKIP (daemon is the
writer; named future: parameterize).

## III. Genera: seeding + guards (exact changes)

All in gesta/fontes/tabularium.c:

1. **SEMINA_GENERUM** (:88-199) += three entries;
   SEMINA_NUMERUS (:204) bumped. Post-index-6 seeds get NO
   automatic attributa (SEMINA_BOARD_NUMERUS fusion, :203,
   4796-4829) → carry attributa INLINE (opus precedent :127-132):
   - articulus: status_initialis conditum, machina
     [[conditum,publicatum],[publicatum,retractum]], attributa
     titulus/corpus/tags.
   - commentarium: status-less (nota-shaped), attributa corpus.
   - pipatum: status-less, attributa corpus.
   Accepted consequence: BOTH worlds see all genera (the seed
   table is baked into the machine; the board simply never
   creates pipata). Cleaner world-parametric seeding = future.
2. **240 guard** in _tab_addere (:2440-2630), after the
   genus/titulus presence guard (:2459-2464): pipatum + corpus
   over limit → `_textum_respondere(..., VERUM)` named refusal.
   DECISION (recommended): count CODEPOINTS not bytes (chorda
   mensura is bytes) — a ~10-line UTF-8 lead-byte counter;
   "240 characters" means characters.
3. **signatura**: optional arg on addere + gerere → stored as a
   datum key on the event (datum is free JSON — no schema
   change); flows to both worlds harmlessly; app renders it as
   the byline, MCP writes pass "Fable 5".
4. **respondet-ad**: ZERO changes — nexus verba are free strings
   (gerere nexus path :2729-2808 validates only presence);
   threading works the day the genera exist.
5. Machine-violation culture holds: status transitions outside
   machina land + custodia nota (gesta.c:1763-1872, 2041-2078) —
   articulus lifecycle is judged, not blocked, per house law.
   tabula.md sections are hardcoded per genus (:1496-1503) —
   forum world runs tabulae=NIHIL, so irrelevant v1.

## IV. Claude's access: the fori proxy

`.mcp.json` gains:
```json
"fori": { "command": "./gesta/forumd_proxy.sh", "args": ["-mcp"] }
```
(entries are bare command+args, stdio, line-framed — exact format
confirmed). The proxy (~40 lines C): tabellarius on
stdin/stdout ↔ fdopen'd tcp_connectere on the daemon; per-request
connections per §I; start-if-absent lives here. stdout stays pure
protocol, chatter to stderr (tabularium.sh:1-17 law). Board
`tabularii` entry untouched.

## V. App: apps/forum/ (conventions established)

- **Own aedilis build** (the subsystem convention — every
  top-level unit has its own; compile_tests.sh only globs
  probationes/probatio_*.c and must NOT be extended): add
  apps/forum + apps/forum/assets to aedilis.stml `<inclusa>`;
  build = `./bin/aedilis apps/forum/forum.c && bash
  build/aedilis/forum/struere.sh`. Frameworks
  (Cocoa/WebKit/Security) are DERIVED from the .m objects in the
  closure — nothing hand-written. Thin apps/forum/forum.sh
  launcher wraps it.
- **Assets**: apps/forum/assets/{index.html, forum.toml} →
  committed capsula_forum.{h,c} (mos vitrea_assets; explicit file
  lists — directory globs have a subdir trap).
- **Daemon client**: tcp_connectere + fdopen + tabellarius,
  per-request (§I). Requests are the SAME MCP-dialect tool calls
  the proxy sends — one protocol everywhere.
- **Speculum wearer** (house custom, ~4 lines + registration):
  `<aedilis obiectum=.../>` annotation + extern-direct symbol,
  speculum_creare with vitrea_aestimator, speculum_tangere in the
  event drain (probatio_vitrea_hospes.c:35-39, 211, 234-237 —
  the canonical wearer). SPECULUM_APPS-style generation wired
  into the forum build script.
- Everything else per spec v1 §III (two views, live push via
  max-seq poll each tick, title+textarea editor).

## VI. Build order

- **F0 — daemon**: initialize idempotence + tabulariumd main +
  launcher + forum world files + genera seeds + 240 guard +
  signatura arg. Gate: gesta suite green + scripted socket smoke
  (the probatio drives a real daemon on a scratch world).
- **F1 — fori proxy**: proxy + .mcp.json entry. Gate: Claude
  posts the first pipatum ever, via MCP, signed "Fable 5".
- **F2 — app skeleton**: apps/ conventions + capsula + vitrea
  shell + feed view read/post against the daemon.
- **F3 — articles + threads**: articulus views/editor,
  commentaria with respondet-ad threading, tombstones.
- **F4 — liveness + wearing + bars**: seq-poll live push,
  speculum, the self-announcing smoke, then the LIVED BAR (Fran's
  first real article, Claude's comment, Fran's reply).

## VII. Open decisions (small, flagged)

1. 240 = codepoints (recommended) vs bytes — §III.2.
2. Daemon port number (fixed localhost port; any preference).
3. Whether forum world's entities/ projection turns on in v1
   (recommended: no; it's the static-export on-ramp later).
