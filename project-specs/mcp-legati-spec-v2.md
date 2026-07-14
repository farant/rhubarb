# MCP LEGATI — SPEC v2 (codebase-verified)

2026-07-14. Supersedes mcp-legati-spec.md (v1): every §X open fact
verified by three explorers against the tree; v1's design stands
with THREE corrections (marked ⚠ below). This file is
self-contained — implementation reads this one.

Line references are to the tree as of 2026-07-14 (post-outgoingCalls,
uncommitted); they drift after the next commit — trust names over
numbers.

## I. Quid sit

LEGATI: name-based, pull-based MCP stdio server; same binary as
legatus (`-mcp` mode); resident per session; lazy revalidation.
Complement to the positional push LSP — the demand record is the
LSP shakedown's two client-side constraints (no diagnostics pull;
didOpen scoped to session-edited files).

## II. DECISUS (interview, all twelve pinned)

1. ONE BINARY, v0 IN-FILE (banner sections; split at LEGATUSD).
2. PURE LATIN everywhere (names, descriptions, results, CAUTIO).
3. diagnostica = EXACT EXAMEN PARITY (§VI.1 — the verified format).
4. FIXED CAPS + explicit "et alia N" tails; verbosum = park.
5. initialize `instructions` = Latin tool-choice doctrine + the
   freshness promise; drafted at ship, reviewed at seal.
6. LSP INDEX-RELOAD BACKPORT in chunk 2 (bar: LSP suite green).
7. Name "legati" → mcp__legati__{diagnostica,symbolum,vocantes,
   vocata}.
8. protocolVersion: echo client's if date-shaped, else our pin.
9. Stale self: answer + CAUTIO ("LEGATI IPSE STALUS — /mcp
   reconnect renovat"); never refuse.
10. POSIX server-wide OFF; dual-praeparatio posix:true = park.
11. symbolum = LEAN CARD; hierarchy stays in vocantes/vocata.
12. goToImplementation rider in chunk 1 (client ops 9/9).

## III. Framing + handshake (verified seams)

- tabellarius framing is exactly TWO functions today
  (tabellarius_epistulam_legere lib/tabellarius.c:55,
  _scribere :143). Add two NEW ones, no mode state:
  `tabellarius_lineam_legere(FILE*, Piscina*, b32* finitus)` —
  unbounded via ChordaAedificator, strip \r, skip blank lines,
  EOF mid-line = finitus; `tabellarius_lineam_scribere(FILE*,
  chorda)` — body + '\n' + fflush, ASSERTS no raw '\n' (refuse +
  stderr clamor, never corrupt the channel).
- SERIALIZATION LAW: compact json_scribere ONLY on the MCP path.
  VERIFIED: _evadere_json (chorda_aedificator.c) escapes \n \r \t
  " \\; pretty mode emits raw newlines (json.c:1625) and is
  forbidden here. Golden: diagnostic message containing '\t'
  round-trips escaped.
- Envelope layer (tabellarius_responsum/errorem) unchanged.
- RADIX: MCP initialize has no rootUri. legatus.sh appends
  `-radix "$RADIX_DIR"`; principale → LegatusConfiguratio (LSP's
  rootUri wins when present; _radicem_statuere at legatus.c:3984
  already prefers cfg->radix). Zero new POSIX.
- MCP methods: initialize / notifications/initialized (ignore) /
  ping / tools/list / tools/call; unknown → -32601.
- ⚠ CORRECTION 1 (encoding): MCP needs NO position-encoding
  negotiation — results are text; linea/columna print as 1-based
  numbers. _codificationem_negotiari and the UTF-16 remap are
  LSP-only plumbing; skip entirely.

## IV. MCP initialize (parity with verified LSP order)

LSP _initialize_tractare order (legatus.c:4311-4396): guard →
_radicem_statuere → codification → posix option →
_praeparationem_struere → _exclusiones_onerare → _indicem_onerare
→ _vigiliam_construere → initiatum → respond.

MCP initialize runs the SAME sequence minus codification; posix
comes from config (OFF in v0), cum_latina stays 0 (PINNED at
legatus.c:3847 — macro-provenance collapse; MCP inherits). Respond
{protocolVersion, capabilities:{tools:{}}, serverInfo:
{name:"legati", version:"0"}, instructions:<doctrine>}.

## V. The revalidation lattice (epoch law) — verified inventory

Three tiers, nested by policy; invalidation kills downward.

**TIER 1 — praeparatio.** VERIFIED inventory (praeparator.c): the
praeparatio holds ① silva/fontes/systema_c89.h (always),
② systema_posix.h (iff cum_posix), ③ include/latina.h (iff
cum_latina — off), ④ EVERY basename-unique .h under radix
(recursive walk skipping dotfiles + build/.git/results/
node_modules; first-wins dedup; basename→path map p->viae_capitum).
⚠ CORRECTION 2 (sweep design): the sim's "~40 stats" was wrong —
the capita set is the whole .h tree (hundreds). The tier-1 check
therefore stats the KNOWN list (p->viae_capitum values + the
selected system headers) — a few hundred stat() calls ≈ 1-2ms —
and does NOT re-walk directories per request. NEW-header discovery
rides on judge-miss: when a _recensere hits an unresolvable
include, trigger tier-1 rebuild once. (Requires praeparator to
EXPOSE the capita list — small praeparator.h addition, or an
iterator.) VERIFIED asymmetry: nothing re-stats capita today; the
LSP relies on didSave rebuild (legatus.c:3863) — tier-1 sweeping
is genuinely new behavior, MCP-mode only in v0.
Tier-1 event → _praeparationem_struere + index reload. SAFE
because MCP mode is document-free (no trees into piscina_capitum;
_recensere trees are ephemeral). Recorded: documents-in-MCP would
re-arm the C11 trap.

**TIER 2 — index** (piscina_indicis: tsv rows + overlays + symbol
cards + via-mtime table). Check: build/nexus.tsv mtime changed →
destroy piscina_indicis + _indicem_onerare fresh (48 MB / ~720k
rows ≈ 1s, disclosed: "index renovatus post commissionem").
VERIFIED: _indicem_onerare (legatus.c:688) creates its own piscina
and all tables — re-runnable after a destroy; nothing outside
points in. Backport the same trigger to LSP mode (chunk 2).

**TIER 3 — per-via** (via → last-judged mtime, lives in
piscina_indicis on purpose). Checked inside lookups; dirty →
_recensere(via). POLARITY: ≥ here (coeval = suspect); vigilia
keeps > (coeval = fresh); both sites comment the other.

`_aetates_custodire` runs tiers 1-2 at request entry; TWO-PASS LAW
for lookups (collect involved viae → judge dirty ones → re-run the
lookup; bounded because judged mtimes are recorded).

## VI. _recensere — the on-demand judge (verified mechanics)

Mirrors the VERIFIED overlay tail of _analysare_et_publicare
(legatus.c:1175-1189): ① _ordines_plagulae_necare(l, via_sine)
(tombstones, :1184) → ② nexus_ordines_fundere(parsura, sem,
effimera_scratch, _superpositio_recipere, l) (nexus_ordines.h:51 —
same unit as the offline sweep, divergence impossible by
construction; emitted rows land in piscina_indicis inside the
callback, scratch piscina only feeds the walk) → ③ extents refresh
(today: tabula_dispersa_delere, :1187; _recensere instead REBUILDS
the entry + harvests cards) → ④ record via→mtime.

Inputs from one praeparator_analysare (praeparator.h:74; the
detached shape already lives at legatus.c:3390-3434): sem +
parsura; fons text read via praeparator_plagulam_legere into the
request piscina; trees in an ephemeral SilvaPiscina destroyed
before return. Syntax-broken file → old rows stay necati, answers
carry "plagula X non analysabilis" CAUTIO.

SYMBOL CARDS (harvest during ③): iterate depth-0 symbols (the
_extenta_ex_semantica loop shape, legatus.c:3278) and render
signatures with **silva_c89_typum_scribere(TypusC89*, buffer,
CCLVI)** (silva.h:1395 — VERIFIED: depends on the symbol's typus
ALONE; returns 0 = irreddibilis → name-only fallback, hover
precedent legatus.c:2671). Card = {titulus, linea_a/b, signatura,
genus} in piscina_indicis. ⚠ CORRECTION 3 (smaller refactor than
planned): no lookup re-typing needed for v0 — cards come from the
symbol-table ITERATION, not byte lookups; the only extraction is
the inline hover render (legatus.c:2653-2704) into a helper taking
SemanticaSymbolum*. (_invenire_ad_byte re-typing to
(parsura, sem) is deferred until MCP ever answers positionally.)

_extenta_viae's closed-file branch (legatus.c:3369) refactors onto
_recensere — it currently parses then DISCARDS diagnostics and
rows. This refactor is why the LSP suite is chunk 2's bar.

## VII. Tool surface (v0, verified formats)

Common: Latin text results, grouped nexus-style, capped with
explicit tails; CAUTIO lines (self-stale / index-reload / via non
analysabilis / graph-age). isError=true for tool-level failures;
JSON-RPC errors for protocol failures. Via normalization: absolute
→ strip radix; "./" strip; outside radicem → error.

**1. diagnostica {via}** — _recensere + EXACT examen human format
(VERIFIED from silva/instrumenta/principalia/examen.c):
- syntax line first if parse errors:
  `<via>: [violatio] nodi erroris (syntaxis) <n>`
- per-diagnostic rows `<via>:<linea>:<columna>: [<severitas>]
  <causa>[ (provisionale)]` — severitas word violatio/suspectum/
  domesticum/infra; ALL tiers printed INCLUDING infra (examen
  parity; note: the LSP publish suppresses INFRA and drops
  cross-file rows — diagnostica does NEITHER, its rows carry via);
- verdict line `verdictum: ACCIPE|REICE  (violationes N, suspecta
  N, domestica N, infra N; provisionalia N)` — REICE on any
  non-provisional VIOLATIO or any parse error.
Walk API (VERIFIED silva.h:1357): silva_c89_diagnostica_numerus /
silva_c89_diagnosticum_per_indicem; SemanticaDiagnosticum fields
(silva.h:1295): causa/codex/severitas/provisionale/via (SilvaChorda,
%.*s)/linea/columna/longitudo/socius (unused today — candidate
"socius:" suffix line = named park).

**2. symbolum {titulus}** — lean card: sedes list (body-def
policy: .c primary → .c secondary → any sedes), genus, signatura
(from cards), uses-per-file counts; macro branch VERIFIED
document-free: resolution via the workspace macro index
(_sedes_macronis legatus.c:2027), #define line read from DISK
(_lineam_definitionis_legere :2054 — open-doc branch is only a
freshness shortcut), multi-line corpus extents need the requesting
parsura->expansio → in MCP the DEFINING file's _recensere parsura
serves, else line + " ..." degradation (hover precedent).
similitudo top-3 fallback on miss (vicinitas pattern, flat acies).

**3. vocantes {titulus}** — incoming by name: usus-chain walk,
revalidate DIRTY using-viae only, 20-file analysis ceiling
(LEGATUS_PLAGULAE_VOCANTIUM_MAXIMAE), grouped by caller.

**4. vocata {titulus}** — outgoing by name: body-def via → tier-3
→ extent + usus-in-extent walk (outgoingCalls internals; no
item/range/prepare), grouped by callee with fromRange lineae.

## VIII. Registration

.mcp.json (root, committed):
`{"mcpServers":{"legati":{"command":"./officina/legatus.sh",
"args":["-mcp"]}}}`. Launcher appends -radix; stdout hygiene
verified (stderr-only chatter + exec).

## IX. Chunks + bars

| chunk | contents | bar |
|---|---|---|
| 1 | tabellarius lineam ×2; -mcp/-radix; handshake+ping+tools/list; goToImplementation rider | newline-framing goldens + '\t'-escape golden; LSP 9/9 golden |
| 2 | signature-render extraction; _recensere (necare→fundere→extents+cards→mtime); _aetates_custodire (tier-1 known-list sweep + judge-miss discovery, tier-2 reload BOTH modes); _extenta_viae refactor | staleness goldens (dirty-via, tsv-swap, utime tricks); **LSP suite green**; capita-list exposure in praeparator.h |
| 3 | four tools + caps + similitudo fallback + doctrine + .mcp.json | tool goldens ×4 (incl. examen-parity diff on a clean file vs examen.sh output); live shakedown |

## X. Parks (named)

verbosum knob; dual-praeparatio posix:true; excubitor-verdict tool;
socius line in diagnostica; sonda una-ictus; positional-lookup
detachment (_invenire_ad_byte re-typing); LEGATUSD (socket, shared
world, unit split); nanosecond mtimes.

## XI. First-contact shakedown (unknowable until live)

tools/list cadence; schema token weight; instructions delivery;
/mcp reconnect vs vigilia nag; spawn cwd/env; approval UX.
