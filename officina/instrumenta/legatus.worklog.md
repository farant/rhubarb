# legatus.worklog.md

## 2026-07-13 — birth (chunks B–D, one day)

Design notes that will matter to future maintenance:

- **Hover is USE-SITES ONLY.** nexus symbolorum records symbols at
  resolution sites (M4b sessio find) — hover on a DECLARED name
  returns null by construction. Path back: silva-side declarator-name
  binding export. Do not "fix" this in legatus by string matching.
- **C11 ordering is load-bearing** (_didsave_tractare): document
  trees point into the capita piscina (praebere'd header text), so
  the teardown order is trees → praeparatio+capita → re-analyse.
  Reordering = use-after-free on the next hover. The capita piscina
  is deliberately owned per-Praeparatio (rebuilds stay bounded);
  exclusiones stay in perennis (loaded once).
- **Excluded documents are never stored** — didOpen publishes empty
  and returns before _documentum_parare. The rebuild loop therefore
  never needs exclusion checks.
- **Fixtures must be plain C89 or carry includes** (chunk B find):
  latina-dialect text without #include is judged INFRA
  ("typus nominatus ignotus" clemency) → publishes NOTHING. The
  engine is right; the differential is `./silva/examen.sh -machina`.
- **utf-16 remap** is per-line and two-directional; positions in
  utf-8 mode are byte-exact (silva columns ARE bytes). Corpus is
  BMP-only but the remap handles surrogates anyway.
- Named v0 limits: documentSymbol lacks struct/enum tags (symbol
  table has no tag rows) and does not dedupe decl+def pairs;
  cross-file diagnostics dropped with stderr count (path back:
  group-by-via multi-publish); didClose of .h does not rebuild
  (didSave only).

## 2026-07-13 — first live reaches (shakedown, resumed session)

First contact through the agent's native LSP tool: documentSymbol
(lib/piscina.c 23 rows; include/piscina.h 17 rows — the .h lane
pays off immediately, headers were never judged per-edit before),
hover use-sites (`piscina : structura Piscina*`, `mensura :
insignatus longus`). Rough edges AT THE REACH:

- **ASPERITAS #1 — hover on a function gives the bare name**
  (`_allocare_interna`, no signature). "What are the parameters"
  is THE agent hover question. TypusC89.datum.functio carries
  reditus + parametra + numerus — renderable. Options: extend
  silva_c89_typum_scribere (function arm), or compose in legatus
  from parts. TOP desideratum, demand = day one.
- **FINDING — the client IGNORES capability negotiation for
  tool-invoked requests**: goToDefinition was sent although we
  never advertised definitionProvider. R7's polite −32601 is
  LOAD-BEARING, not theoretical — and it surfaces verbatim in the
  tool output ("methodus ignota", Latin error through the whole
  chain). Also: this is a live DEMAND SIGNAL for definition
  (v0.1 pull counter: 1).
- Hover on declared names (null, use-sites park) will bite most
  when reading HEADERS — decl-heavy files. Same park, sharper
  pull.
- Shakedown watch: double-push (hook + legatus) on first real .c
  edit — expected differential noise, retire hook at bar.
- **SHAKEDOWN FINDING — binary upgrade path**: killing the running
  server externally (SIGTERM) to force a rebuilt binary left the
  client WEDGED: "Cannot send request … server is running" with NO
  process alive (restartOnCrash evidently arms only on failures the
  client itself observes, or is slow). Recovery = /reload-plugins
  (user-side) or session restart. LESSON: upgrade a live legatus
  via /reload-plugins, never kill-and-pray. Prebuild first
  regardless (startupTimeout). CONFIRMED: /reload-plugins cleared
  the wedge and respawned the new binary cleanly.
- **ASPERITAS #1 RESOLVED same session**: function-signature hover
  live — `_allocare_interna : vacuum*(structura Piscina*,
  insignatus longus, insignatus longus, integer)`. Renderer
  extended SILVA-SIDE (TYPUS_C89_FUNCTIO arm in
  _typum_scribere_intus: K&R = empty parens, prototyped-zero =
  "(vacuum)", variadic = ", ..."; pointer-to-function composes via
  the monstrator arm for free); amalgam VERIFICATUM; suites 12/12 +
  30/30 (sessio goldens unaffected — function-decl echo was never
  pinned); probatio pins `probatio_functio : integer(integer)`.
  First full dogfood loop closed: reach → asperitas-in-worklog →
  silva fix → gates → live, within the hour.
