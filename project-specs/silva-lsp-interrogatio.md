# silva-lsp — interrogatio (raw Q&A)

2026-07-13. Post-fundamenta interview (16 questions, 4 batches).
Precedent: officina-m4b-interrogatio.md. Every recommendation
accepted; two answers carried steering beyond the options (Q1 scope
principle, Q6 reuse seam). Spec follows AFTER simulatio (Q16).

Fact established mid-interview: the Claude Code agent harness's LSP
tool exposes goToDefinition, findReferences, hover, documentSymbol,
workspaceSymbol, goToImplementation, prepareCallHierarchy,
incomingCalls, outgoingCalls (schema inspected live). Hover IS in the
agent's reach.

---

## Q1 — v0 center of gravity

Context: LSP diagnostics are edit-scoped = the hook's existing
trigger surface; hover/docSymbol are single-file ops off the warm
parse; definition/references need a cross-file index (unexplored).

**FRAN (verbatim):** "it's stuff that you would use as a claude code
agent. so i think diagnostics for sure, probably not hover? i'm not
sure if that's something you have access to. so whatever you would
have access to and use within an lsp server while working."

**Resolution (Q5, after LSP-tool schema check): DECISUS — v0 =
diagnostics + hover + documentSymbol.** Hover is in the agent's
surface and is the "what type/signature is this" question (today an
emitte/interpretare shell round-trip); docSymbol is nearly free off
the same parse. definition/references/workspaceSymbol = **named
v0.1** behind a fifth exploration (cross-file index design; nexus
symbolorum = likely substrate). **The scope principle is now
recorded: legatus's surface is defined by what the agent actually
reaches for, staged by real cost.**

## Q2 — clangd-lsp disposition

**DECISUS: disable now** (`"clangd-lsp@claude-plugins-official":
false` in repo .claude/settings.json — the typescript-lsp:false line
proves the mechanism). Noise gone immediately; bench covers
navigation in the gap; the examen-vs-clangd live differential is
declared DONE (M4a vectis finished that calibration). Executed
in-session 2026-07-13.

## Q3 — R1 (warm pre-lexed headers)

**DECISUS: park by name.** v0 ships at ~0.15s/analysis (3× faster
than the hook; agent didChange is per-edit, not per-keystroke). The
park's named pulls: (a) the fontes-loader migration "next touch"
(R1 IS that work arriving), (b) felt latency in daily driving.
Path back: pre-lexed includendum set in contextus;
_contextum_applicare is the site (silva_parsare.c:1256).

## Q4 — syntax-error positions (0:0 today)

**DECISUS: timeboxed attempt.** v0 attempts first-error-node-token
position via the radix chain (GLR breaking-token index exists); if it
becomes real parser surgery, fall back to degenerate line-1
whole-file range and the parser-side positioning task becomes a named
park. Escape hatch keeps the milestone honest.

## Q5 — scope confirmation (see Q1 resolution)

**DECISUS: Diag + hover + docSymbol**, definition/references = v0.1.

## Q6 — nomen ceremony

**DECISUS: LEGATUS.** The legate/ambassador — speaks a foreign
protocol (JSON-RPC) on silva's behalf to a foreign power (the
editor/agent harness). Binary `legatus`; launcher per Q7's home.
Rejected: interpres (spirit-collision with interpretare.sh), praeco
(a herald doesn't answer questions; legatus does).

## Q7 — domus (where the code lives)

**FRAN (verbatim):** "officina-side sounds great, the only thing that
i would consider is if it's worth parking mcp json-rcp or whatever
code somewhere else so that it is reusable later for other
applications"

**DECISUS: officina-side.** Code in officina/instrumenta (praeparator
neighbor), launcher ./officina/legatus.sh. Dependency-honest (amalgam
+ praeparator + lib/json = vindex/colloquium profile). **Books move
officina-side** (the silva phase-log GRADUS I entry stands as
history; correction recorded forward, appended never edited). The
reuse instinct became Q8.

## Q8 — the JSON-RPC seam (follow-up to Q7)

**DECISUS: lib/ now, pure API — nomen TABELLARIUS** (the Roman
courier who carries epistulae). A real monorepo library with its own
credo probatio: pure chorda-in/chorda-out, zero LSP knowledge,
**framing and envelope as separate composable pieces** (LSP =
Content-Length framing; a future MCP server reuses the envelope with
different framing). Precedent: similitudo went straight to lib/.

## Q9 — crash policy (OOM = exire(1))

**DECISUS: accept-and-respawn.** The client restarts it
(restartOnCrash defaults true; CC 2.1.207 ≥ 2.1.205 floor). Warm
rebuild ~0.05s on respawn. No silva allocator changes. Documented
policy, not an accident.

## Q10 — header invalidation

**DECISUS: rebuild on .h save.** didSave/didClose of any .h → full
Praeparatio rebuild (~0.05s) → re-judge open documents. Unsaved .h
buffer edits judge that buffer but do NOT update the context other
files see — documented divergence (matches compiler world-view; the
sweep reads disk).

## Q11 — examen-custos hook fate

**DECISUS: coexist → retire at bar.** Both channels run during
legatus's shakedown (live differential against the proven hook —
catches LSP delivery bugs); at the daily-driver bar the hook is
disabled, script KEPT for headless/CI contexts (censor-in-gates
precedent). Double-push accepted as temporary shakedown cost.

## Q12 — judge .h directly?

**DECISUS: yes.** New coverage the hook never had (hook skips .h
"judged via includers" — headers currently get NO per-edit judgment).
Corpus evidence exists: percursus judges .c+.h (750 files) with the
verdict column. Headers parse with systema + praebere'd siblings.

## Q13 — exclusion discipline

**DECISUS: same list, publish empty.** legatus reads the same
exclusiones.txt + path filters as the hook (scratchpad/build/fixa/
amalgama) and publishes an EMPTY diagnostics array for excluded files
— silence discipline (M4a lesson: known-dialect noise teaches
ignoring everything), one shared source of truth, and the amalgama
exclusion dodges the 3.8s 15k-line analysis.

## Q14 — positionEncoding

**DECISUS: offer utf-8 + utf-16 fallback.** Advertise utf-8
(silva byte columns = zero conversion); if the client only offers
utf-16 (unknown until first live initialize — named observation
point), per-line byte→code-unit remap (small: corpus surveyed
surrogate-free, 51 files non-ASCII all BMP). Both paths in v0;
conformant either way.

## Q15 — the seal

**DECISUS: formal + daily-driver (M3/M4 precedent).**
- FORMAL: golden JSON-RPC transcripts — credo probatio drives the
  legatus binary over pipes (initialize→didOpen→publishDiagnostics;
  hover; documentSymbol; syntax-error scenario; exclusion scenario;
  shutdown/exit), byte-asserted.
- DAILY-DRIVER: registered via --plugin-dir, real agent sessions,
  hover/docSymbol reached organically, hook retired at this bar.

## Q16 — process

**DECISUS: simulatio first.** silva-lsp-simulatio (pretend
implementation: transport loop, position→node index, hover
rendering, registration handshake) → complications C1-Cn to
project-specs/silva-lsp-simulatio.md → THEN the spec (from interview
+ fundamenta + sim). Every prior sim paid.

---

## DECISUS COMPENDIUM

| # | res | decisum |
|---|-----|---------|
| 1/5 | scope | v0 = diagnostica + hover + documentSymbol; def/refs = v0.1 post-exploratione quinta; superficies = quod agens re vera adhibet |
| 2 | clangd | inactivum NUNC (repo settings); differentialis DONE |
| 3 | R1 | parcatum nominatim (pull: migratio fontes-oneratorum / latentia sentita) |
| 4 | syntax pos | conatus timeboxed; effugium = extensio degenerata + parca |
| 6 | nomen | LEGATUS |
| 7 | domus | officina-latere; libri officina-latere |
| 8 | rpc | lib/tabellarius natus purus (framing ∥ envelope) |
| 9 | ruina | accipe-et-resuscita (restartOnCrash) |
| 10 | invalidatio | reaedifica in .h servato (~0.05s) |
| 11 | uncus | coexsistere → dimittere ad barram (scriptum manet) |
| 12 | .h | iudica directe (novum coverage) |
| 13 | exclusiones | eadem lista, vacuum publica (silentium) |
| 14 | encoding | utf-8 oblatum + utf-16 fallback |
| 15 | sigillum | transcripta aurea formalia + agitatio cotidiana |
| 16 | processus | simulatio → spec |
