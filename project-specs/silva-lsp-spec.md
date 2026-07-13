# LEGATUS — silva-lsp spec (v1)

2026-07-13. Inputs, in authority order: silva-lsp-interrogatio.md
(17 DECISUS — governs), silva-lsp-simulatio.md (C1–C15 — shapes),
silva-lsp-fundamenta.md (34 facts + measurements — grounds).
Books officina-side (officina/phase-log.md). Never implement a chunk
from a compaction summary alone — re-read this spec's section first.

---

## I. QUID SIT

**legatus** — the ambassador: an LSP 3.17 server speaking JSON-RPC
over stdio, serving silva's own C89 judgment and understanding to
Claude Code, replacing clangd for this repo. **The scope principle
(Q1): legatus's surface is what the agent actually reaches for,
staged by real cost.**

v0 surface: **diagnostics push + hover + documentSymbol** (the
single-file operations off the warm parse). v0.1 by name:
definition/references/workspaceSymbol (cross-file index; fifth
exploration first — nexus symbolorum = likely substrate).

Components:
- **lib/tabellarius** — the courier: JSON-RPC framing ∥ envelope,
  a pure monorepo library over lib/json (future consumer: MCP
  server).
- **officina/instrumenta/legatus.{c,h}** — the engine:
  `legatus_currere(FILE* intra, FILE* extra, LegatusConfiguratio*)`;
  plus **officina/instrumenta/principalia/legatus.c** (thin main) and
  **./officina/legatus.sh** (colloquium-pattern incremental build +
  exec).
- **Silva enablers (chunk 0)** — three public-API additions in ONE
  amalgam ceremony (§IV).
- **officina/legatus-plugin/** — Claude Code plugin dir
  (.claude-plugin/plugin.json + .lsp.json), dev-loaded via
  --plugin-dir.

Non-goals v0 (named, with paths back): definition/references
(v0.1); R1 warm pre-lexed headers (park; pull = fontes-loader
migration or felt latency; site: _contextum_applicare,
silva_parsare.c:1256); cross-file diagnostic publish (C3 path back:
group-by-via); incremental sync; marketplace-portable packaging;
window/logMessage channel; JSON-RPC batches.

## II. SEALED DECISIONS (compendium — full text in interrogatio)

Scope = diag+hover+docSymbol (Q1/Q5) · clangd-lsp disabled NOW,
differential DONE (Q2) · R1 parked (Q3) · syntax positions attempted,
fallback intact (Q4, upgraded by sim C13) · nomen LEGATUS (Q6) ·
domus officina-side (Q7) · lib/tabellarius born pure (Q8) · crash =
accept-and-respawn via restartOnCrash (Q9) · invalidation = rebuild
on .h save (Q10) · hook coexists → retires at daily-driver bar,
script kept for headless (Q11) · .h judged directly (Q12) ·
exclusions = same list, publish empty (Q13) · offer utf-8 + utf-16
fallback (Q14) · seal = formal golden transcripts + daily-driver
(Q15) · type renderer promoted SILVA-SIDE (Q17/C5).

## III. TABELLARIUS (chunk A)

Pure library, no LSP knowledge. Two independent layers:

**Framing** (LSP transport = Content-Length):
- `chorda tabellarius_epistulam_legere(FILE* fluxus, Piscina* p,
  b32* finis_out)` — reads header lines (ASCII, \r\n) to the blank
  line, parses Content-Length, freads EXACTLY N bytes. finis_out =
  EOF/malformed-stream flag. Unknown headers skipped.
- `vacuum tabellarius_epistulam_scribere(FILE* fluxus, chorda
  corpus)` — emits `Content-Length: N\r\n\r\n` + bytes + fflush.

**Envelope** (JSON-RPC 2.0):
- `TabellariusNuntius` {genus: PETITIO/NUNTIATIO/RESPONSUM/PRAVUM;
  id: JsonValor* (verbatim — C1: int-or-string echoed exactly);
  methodus: chorda; params: JsonValor*}.
- `tabellarius_nuntium_legere(chorda corpus, Piscina*) →
  TabellariusNuntius` (parse via json_legere; jsonrpc field checked;
  batch arrays → PRAVUM).
- Builders: `tabellarius_responsum(Piscina*, JsonValor* id,
  JsonValor* resultatum) → chorda`,
  `tabellarius_errorem(Piscina*, JsonValor* id, s64 codex, constans
  character* nuntius) → chorda`,
  `tabellarius_nuntiationem(Piscina*, constans character* methodus,
  JsonValor* params) → chorda`.
Error codes as named constants: PARSE_ERROR −32700, INVALID_REQUEST
−32600, METHOD_NOT_FOUND −32601, SERVER_NOT_INITIALIZED −32002.

Bar A: probationes/probatio_tabellarius.c (root suite) — framing
round-trips (incl. split reads via tmpfile), envelope parse/build
golden strings, id-type echo (int AND string), batch rejection,
malformed-stream flag.

## IV. SILVA ENABLERS (chunk 0 — one fontes touch, one amalgam ceremony)

All three are public-API additions; legatus consumes ONLY public
silva API afterward.

1. **Diagnostic extent**: `unsigned int longitudo` on
   SemanticaDiagnosticum (0 = unknown); assigned from
   radix->longitudo at the single materialization site
   (silva_c89_semantica.c:328 vicinity) where radix is already in
   hand. examen -machina TSV UNCHANGED (column stability for the
   differential vectis).
2. **Node extent**: promote sessio's _extensionem_valoris (pure move
   — written against public API; sessio.c:1694–1773) as
   `silva_nodus_extensionem(nodus, fons_index, s32* min, s32* max)`
   (byte form) + a linea/columna variant (track radix->linea/columna
   min/max). fons_princeps filter stays (desirable: clamps
   macro-spanning nodes to the current file).
3. **Type renderer**: promote sessio's _typum_scribere family as
   `silva_c89_typum_scribere(SilvaPiscina*, constans TypusC89*,
   SilvaChorda titulus) → SilvaChorda` — Latin C-style declaration
   rendering with optional declarator name. NOTE: sessio's version
   writes via officina-side chorda_aedificator; the promotion
   re-expresses against silva-internal string building (NOT a pure
   move — the one line-item of real porting in chunk 0).

**sessio migrates in the same pass**: deletes its private extent
walker and renderer, consumes the public rows (praeparator-migration
pattern; behavior-preserving).

Gates (all must hold in one run): silva suite · amalgamare.sh
VERIFICATUM (silva.h hand-extended with the three rows) · officina
suite 11/11 (sessio parity = the migration proof) · examen_vectis
-corpus TENET (TSV unchanged proof) · censor.

## V. LEGATUS ENGINE (chunk B)

**Loop**: single-threaded; read message → dispatch → fully handle
(including any publishDiagnostics) → next. Requests before
initialize → −32002. Unknown requests → −32601. `$/` requests →
−32601; `$/` and unknown notifications → silently dropped.
shutdown → respond null, mark; exit → terminate (0 iff shutdown
seen). **stdin EOF → exit 1** (client died). STDOUT IS
PROTOCOL-PURE — all logging to stderr (C10).

**initialize** (C8): radix from rootUri (fallback workspaceFolders
[0], fallback cwd); scan general.positionEncodings for "utf-8" →
mode; build Praeparatio (cum_posix from initializationOptions
{"posix": bool}, default FALSUM — hook/examen ISO parity;
cum_latina=FALSUM; sine_capitibus=FALSUM). Response:
```json
{"capabilities":{
   "textDocumentSync":{"openClose":true,"change":1,"save":true},
   "positionEncoding":"utf-8|utf-16",
   "hoverProvider":true,
   "documentSymbolProvider":true},
 "serverInfo":{"name":"legatus","version":"0"}}
```
(object-form sync: didSave needed for the .h lane, Q10.)

**URI ↔ via (C2)**: uri_ad_viam (strip file://, percent-decode,
radix-relativize to `./…` form — byte-matches exclusiones.txt rows)
and viam_ad_uriam (inverse + percent-encode). Internal canonical via
= the repo-relative form. Files outside radix → treated as excluded.

**Document store (C7)**: tabula_dispersa via→Documentum*; Documentum
{via, versio, piscina_versionis, textus, tabula_linearum,
index_extentorum, parsura, semantica, effimera(SilvaPiscina)}. Every
didChange/didOpen version: NEW piscina_versionis holds text + line
table + token index; NEW effimera holds trees (praeparator contract:
fons must outlive trees — both die together on the NEXT version or
didClose). Bounded: open docs × 1 live version.

**Analysis lane** (didOpen/didChange; .c AND .h — Q12):
1. Excluded (exclusiones.txt ∪ scratchpad/build/fixa/amalgama ∪
   outside-radix — C12)? → publish EMPTY, done.
2. praeparator_analysare(via, buffer text). Apparatus-broken (NULL
   sem) → publish empty + stderr note.
3. parsura->numerus_errorum > 0 → syntax lane (§VII).
4. Walk diagnostica: keep via == document via ONLY (C3; dropped
   count → stderr); drop EXAMEN_INFRA (hook parity, Q13-discipline);
   map severitas violatio→1 suspectum→2 domesticum→3; code = codex
   int; source = "silva"; message = causa (+ " (provisionale)");
   range = linea/columna/longitudo converted (§VI). linea==0 →
   degenerate {0,0}–{0,1} + "(positio ignota)" (C15).
5. Publish with document version echoed. didClose → publish empty +
   drop Documentum.
   didChange payload: LAST element of contentChanges, text field
   (C9 — we advertised Full; do not "fix" into incremental).

**.h save lane (Q10, C11 — rigid order, stale-object trap)**:
didSave of any .h under radix → ① destroy ALL open documents'
version piscinas/effimerae (trees die first — they point into ctx
arena) → ② praeparator_destruere + praeparator_praeparare (~0.05s)
→ ③ re-analyse + re-publish every open document from its stored
text. Single-threaded = atomic to the client.

Bar B (golden v0): initialize/capabilities · didOpen clean → empty ·
didOpen violatio fixture → asserted rows · excluded file → empty ·
unknown method → −32601 · shutdown/exit · EOF → exit 1.

## VI. POSITIONS, HOVER, DOCUMENTSYMBOL (chunk C)

**Conversions**: silva 1-based bytes → LSP 0-based; utf-8 mode =
subtract 1 (line AND column); end.character = columna−1+longitudo.
utf-16 mode: per-line byte→code-unit rescan via tabula_linearum
(line-start offsets over the version text; corpus is surrogate-free
— fundamenta survey — so the remap is arithmetic, but write it
correctly for astral anyway: +2 for 4-byte UTF-8 sequences).

**Position index** (built per version, in piscina_versionis):
tabula_linearum + index_extentorum = sorted {byte_a, byte_b, token*}
over parsura->lexemata, radix-resolved, filtered fons_princeps &&
byte_offset ≥ 0. position → byte (line table) → token (binary
search).

**Hover** (C4): byte offset → tree descent by
silva_nodus_extensionem from root elementa (child whose [min,max)
contains offset; recurse to leaf) → try silva_c89_symbolum_nodi →
render `titulus : typus` via silva_c89_typum_scribere (colloquium's
echo format — one house voice); else silva_c89_typus_expressionis on
the covering expression node → `typus` alone. Nothing at position →
null result (legal). contents = plaintext.

**documentSymbol** (C14): SAFE PATH = root elementa walk filtered
fons_princeps: per element, name (probe machinery precedent from
sessio's decl echo) + range via silva_nodus_extensionem +
SymbolKind {functio→12, structura-tag→23, enum→10, typedef→5,
variabilis→13}. AT BUILD: verify whether SemanticaSymbolum carries
its declaring nodus (tail fields unverified in sim) — if yes, the
symbol-table walk (profunditas==0 && !ex_systemate) is the cleaner
source; decide by inspection, record in worklog. Flat
SymbolInformation[] v0 (hierarchical DocumentSymbol[] = named
non-goal).

Bar C: hover known-answer transcripts (identifier → `s : chorda*`
style, expression, whitespace-null) · documentSymbol known-answer on
a fixture · range assertions with longitudo · utf-16 fixture (a line
with `§`).

## VII. SYNTAX ERRORS (chunk D; sim C13 — attempt expected to land)

numerus_errorum > 0 → walk root elementa for
SILVA_C89_GENUS_ERROR (53): per error node, first origo-root token
with byte_offset ≥ 0 → position; extent via silva_nodus_extensionem
→ range; severity Error; message "syntaxis fracta" + count.
No qualifying token (all-synthetic/API radices) → FALLBACK (Q4,
intact): one diagnostic, degenerate range at {0,0}, whole-file
message. Semantica rows still published alongside (bis-analysis
proceeds on recovered trees — same as examen today).

## VIII. PACKAGING + REGISTRATION (chunk D)

officina/legatus-plugin/: `.claude-plugin/plugin.json` (name
"legatus") + `.lsp.json`:
```json
{"legatus": {
   "command": "${CLAUDE_PLUGIN_ROOT}/legatus-exec.sh",
   "extensionToLanguage": {".c":"c", ".h":"c"},
   "startupTimeout": 30000,
   "restartOnCrash": true, "maxRestarts": 5,
   "initializationOptions": {"posix": false}}}
```
legatus-exec.sh execs ../legatus.sh (in-repo relative — valid under
--plugin-dir; marketplace-cache portability = named non-goal v0).
Launcher = colloquium.sh pattern (incremental objects, chatter →
stderr, exec binary). Document the one-time prebuild + the
startupTimeout raise (C10/C14). Registration steps recorded in the
plugin README: `claude --plugin-dir …` or repo extraKnownMarketplaces
later; clangd-lsp already disabled (Q2, done 2026-07-13). Verify
handshake via `claude --debug` + Ctrl+O.

## IX. CHUNKS + BARS

- **0 — silva enablers**: §IV. Bar: the five gates in one run;
  sessio parity (officina suite) is the migration proof.
- **A — tabellarius**: §III. Bar: probatio_tabellarius golden
  strings + framing round-trips.
- **B — engine core + diagnostics lane**: §V. Bar: golden v0
  transcript set (in-process engine over pipe FILE*s — C6, sessio
  precedent; probatio_officina_legatus.c).
- **C — positions/hover/docSymbol**: §VI. Bar: known-answer
  transcripts incl. utf-16 fixture.
- **D — syntax + .h lane + packaging + SEAL**: §VII/§VIII + full
  golden suite (all §V/§VI/§VII scenarios + .h-save rebuild
  scenario + $/-drop + encoding negotiation both ways) + THE
  DAILY-DRIVER BAR: registered via --plugin-dir in real sessions;
  hover/documentSymbol reached organically; shakedown differential
  (hook + legatus both live: same edit → same rows) held for a
  session, THEN hook disabled (script kept — Q11). Instrument
  debrief in the RELATIO (adhibita/fructus/asperitates/desiderata).

## X. RISKS / OBSERVATION POINTS

- Does Claude Code's client offer utf-8 positionEncodings? First
  live initialize answers; both paths shipped regardless (Q14).
- SemanticaSymbolum nodus field — verify at chunk C (C14).
- Renderer promotion = re-expression against silva internals (the
  only non-mechanical part of chunk 0).
- startupTimeout vs cold build — prebuild discipline + 30s margin.
- LSP-tool traffic against minimal capabilities: negotiation should
  gate it; any surprise request answers −32601, never hangs (R7).

## XI. V0.1 BY NAME (the pull list)

definition/references/workspaceSymbol (fifth exploration: cross-file
index; nexus symbolorum substrate; per-URI group publish C3 rides
along) · R1 warm pre-lexed headers · hierarchical DocumentSymbol ·
incremental sync · marketplace packaging · MCP server as
tabellarius's second consumer.
