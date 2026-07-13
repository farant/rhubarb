# silva-lsp — fundamenta (on-ramp grounding)

2026-07-13. Compiled from four parallel explorer reports (superficies /
daemon / positio / cliens), launched per the ORDO PROXIMUS addendum in
officina/phase-log.md after Fran chose silva-lsp over M5 aarch64.
Chosen rationale (a design input, not color): **the agent uses C89
judgment constantly — daily dogfooding is the polish engine.** Books
silva-side (M4a precedent); this document is the spec's ground truth.

---

## THE REFRAMES (read these before anything else)

**REFRAME 1 — the value proposition has moved.** Claude Code's LSP
diagnostics channel is EDIT-SCOPED: diagnostics are pushed after the
agent edits a file. That is the *same trigger surface the
examen-custos hook already owns* (0.46s, silent-when-clean, live
today). Docs show no push-on-read and no project-wide sweep. So a
diagnostics-only silva-lsp largely DUPLICATES an existing channel.
What an LSP server genuinely adds:
  (a) the built-in **LSP tool** — definition / references / hover /
      document symbols / workspace symbols — backed by silva's real
      C89 understanding instead of clangd's wrong-on-Latin guesses
      (tool confirmed present in the main-session harness);
  (b) consolidation: one warm daemon could serve the hook's job too,
      retiring a per-edit process spawn;
  (c) killing clangd noise — but that is FREE (Reframe 2).

**REFRAME 2 — clangd is running against us right now, and disabling
it costs nothing.** The `clangd-lsp` plugin (official marketplace) is
ENABLED globally on this machine; Claude Code pushes clangd
diagnostics on every .c edit ON TOP of the examen hook. One line in
repo `.claude/settings.json` — `"clangd-lsp@claude-plugins-official":
false` — silences it project-wide, no code written. Caveat: the
examen hook was deliberately set up as a live differential vs clangd;
disabling ends that calibration channel (arguably job-done post-M4a
vectis). FRAN'S CALL; not yet done.

**REFRAME 3 — the warm daemon's hoped-for amortization is mostly
blocked as-is: ~1.4×, not ~7×.** The warm Praeparatio caches header
TEXT, never header LEXEMES. `_contextum_applicare`
(silva/fontes/silva_parsare.c:1256–1268) eagerly re-lexes ALL 184
repo headers (1.63 MB) into the ephemeral expansio on EVERY parse
(silva_expandere.c:1038), whether or not the file includes them.
Measured: of the ~0.15s cold floor, only ~0.06s (process + sweep +
systema parse) is amortizable; ~0.10s header re-lex rides on every
analysis. Capturing the real win = warm a pre-lexed includendum set
in the contextus — a change to `_contextum_applicare` / the contextus
data model, NOT a config flag. This is THE central engine design
decision of the milestone, and it plausibly IS the "fontes-side
loaders migrate on next touch" work arriving on schedule.

**REFRAME 4 — the JSON problem does not exist.** lib/json.c is
production-complete: 1859 lines, parse + serialize + builder + pretty
printer, 553-line credo suite, correct escaping of arbitrary
diagnostic text (`"` `\` `\n` control bytes → \uXXXX; UTF-8
passthrough), distinct s64/f64 genera (JSON-RPC ids round-trip as
integers), error position on parse failure. Use as-is; do not
hand-roll a second JSON path. nuntium: confirmed irrelevant (binary).

---

## I. PROTOCOL SURFACE (superficies)

1. **Framing** (LSP 3.17): `Content-Length: N\r\n` (+ optional
   headers) `\r\n` then EXACTLY N bytes UTF-8 body. Only
   Content-Length mandatory.
2. **Requests needing real responses: two.** `initialize` →
   InitializeResult; `shutdown` → null. EVERYTHING else unimplemented
   → error `-32601` MethodNotFound. Requests before initialize →
   `-32002` ServerNotInitialized.
3. **`$/` rule**: `$/`-requests → -32601; `$/`-notifications
   ($/cancelRequest, $/setTrace) → silently drop. Unknown
   notifications → drop, never respond.
4. **Notifications acted on**: didOpen (parse→publish), didChange
   (Full sync: replace text→reparse→publish), didClose (drop doc +
   publish EMPTY diagnostics to clear squiggles), didSave (optional).
   `initialized` → no-op.
5. **publishDiagnostics** is a server→client notification, needs NO
   capability declaration. 0-based line/character. severity 1=Error
   2=Warning 3=Info 4=Hint.
6. **Minimal capabilities**:
   `{"capabilities":{"textDocumentSync":1,"positionEncoding":"utf-8"},
     "serverInfo":{"name":"silva-lsp","version":"0"}}`
   (int shorthand 1 = Full; object form `{"openClose":true,
   "change":1,"save":true}` if didSave wanted. None=0 Full=1
   Incremental=2.)
7. **Exit semantics**: shutdown (respond null, do NOT exit) → exit
   notification → terminate; exit 0 iff shutdown seen, else 1.
8. **Single-threaded synchronous is compliant**: fully handle one
   message (including its publishDiagnostics) before reading the
   next. No threading in v0.
9. **positionEncoding**: LSP default utf-16 is MANDATORY for clients;
   utf-8 is opportunistic (`general.positionEncodings`). Silva columns
   are 1-based BYTE offsets (silva_lexema.c:161–173: columna++ per
   byte) → **utf-8 = zero-conversion** (just −1/−1); utf-16 fallback
   needs a per-line byte→code-unit rescan. Corpus survey: 51 files
   contain non-ASCII (§ ½ × ² ± in comments), ALL BMP, no surrogate
   pairs — the fallback is simple arithmetic. Whether Claude Code
   offers utf-8: UNKNOWN until first live initialize.
10. **Full sync sizing**: largest hand-edited file ~191 KB
    (silva_c89_semantica.c) — fine. Multi-MB generated files
    (capsula_libri 12.8 MB) are the only stress and aren't live-edited.
11. json.c hygiene notes: per-message piscina reset is the natural
    allocator discipline; `iscntrl((signed char))` UB-ish for ≥0x80
    (benign on macOS, cast if touched); \uXXXX surrogate decode on
    parse unverified (low impact — clients send raw UTF-8).

## II. ENGINE + WARM DAEMON (daemon)

12. **praeparator_analysare IS the diagnostics recipe** — byte-
    identical in shape to examen.c's analysis phase (praeparator.c:
    289–330 ≡ examen.c:329–356; both descend from sessio.c). No
    divergence that changes diagnostics in the default path. Touches
    NO machinula/regio/conexio/demissio.
13. **A judge-only LSP needs praeparator + amalgam diagnostic API,
    NOT sessio.** UNA SESSIO EXSECUTANS binds only executing code
    (fixed-base regio); a daemon that never executes is unconstrained.
14. **Build on the AMALGAM (vindex pattern) — decided, not open.**
    praeparator includes silva.h, which exists only in
    silva/amalgama/; linking it into a fontes-style build (examen's)
    double-defines the silva public API. Cost: silva-lsp inherits the
    amalgamare.sh regeneration dependency, same as vindex/colloquium.
15. **Ephemeral-piscina-per-analysis is the proven pattern**:
    vindex_onerator.c:110–138 = the template (8 MB "vindex_arbor"
    ephemeral per file, praeparator_analysare, destroy). sessio.c ×9
    sites. Daemon didChange loop = create → analysare → walk
    diagnostica → destroy. Zero new machinery.
16. **Diagnostic walk API (amalgam, silva.h:1275–1339)**:
    silva_c89_diagnostica_numerus / _diagnosticum_per_indicem;
    SemanticaDiagnosticum {nodus, causa, codex, severitas,
    provisionale, via, linea, columna, socius}.
17. **Piscinae grow, not reserve** (initial chunk + malloc-on-demand,
    demand-paged): warm 64 MB praeparator / ephemeral 8 MB house norm.
18. **OOM aborts the process**: silva's allocator is fatalis=VERUM →
    exire(I) on malloc failure (silva.c:4657–4689). No soft-fail. A
    daemon dies on pathological input. Mitigation candidates:
    accept-and-respawn (Claude Code plugin `restartOnCrash` defaults
    TRUE — the client already does this), or a non-fatal allocate
    variant (doesn't exist). Needs a DECISUS.
19. **Header invalidation is coarse and cheap**: no single-header
    replace API (praebenda append-only; tabula last-wins would help
    but arena text never freed). Honest v0: any repo-.h change →
    rebuild the whole Praeparatio at ~0.04–0.06s. Systema headers
    never change. Awkward named case: didChange on an unsaved .h
    buffer (buffer vs on-disk text for the sweep).
20. **Measurements** (−O0, warm cache, 2 runs):
    | scenario | result |
    |---|---|
    | cold floor (tiny file, repo ctx) | ~0.15s |
    | …decomposed | 0.02 process+malloc · 0.03 sweep · **0.10 header re-lex (PER ANALYSIS)** · 0.01 systema |
    | lib/piscina.c (523 l) | 0.157s |
    | silva_parsare.c (1375 l) | 0.206s |
    | sessio.c (3369 l) | 0.302s |
    | warm daemon, recipe unchanged | ≈0.10s + file work (~1.4×) |
    | warm daemon + pre-lexed headers | ≈file work only (the real win; needs R1) |
    | Praeparatio full rebuild | ~0.05s |

## III. POSITIONS → RANGES (positio)

21. **Diagnostics carry a POINT but retain the nodus.**
    SemanticaDiagnosticum: via/linea/columna 1-based, 0=unknown; plus
    nodus + socius pointers (the range re-derivation handles).
22. **THE ~2-LINE RANGE**: at silva_c89_semantica.c:317–341 position
    is materialized from `radix = silva_token_radix(_lexema_primum
    (nodus))` — and `radix->longitudo` (token byte width) is in hand
    and DISCARDED. One struct field + one assignment ⇒ every semantic
    diagnostic gets an honest (linea,columna)..(+longitudo) range.
    First-token width is the RIGHT range for the dominant classes
    (undeclared id, redecl, bad specifier), not a compromise.
23. **Macro honesty already works**: diagnostic on `si (...)` lands at
    the source `si` keyword via the origo-root chain (EXPANSIO→
    invocatio walk, silva_token.c:246–279) — not the synthetic `if`
    (byte_offset −1), not 0:0. Both semantica and sessio's extent
    walker already do this.
24. **_extensionem_valoris promotion is a MOVE, not a port**
    (sessio.c:1694–1773): whole-subtree [min,max) byte extent,
    radix-resolved, fons_princeps-filtered (that filter is DESIRABLE
    for LSP — clamps macro-spanning nodes to the current file).
    Written 100% against silva's public API. Only real work: it's
    byte-only; LSP wants a line/column variant. Note: whole-NODE
    ranges are often WORSE UX than first-token (a bad function def
    would underline the whole function).
25. **SYNTAX ERRORS HAVE NO POSITION — the biggest UX gap.** examen.c
    emits parse failures hardcoded 0:0 (:363–377); the semantica
    diagnostic path never sees them. Mid-edit broken syntax is the
    MOST COMMON live-editor state. Real positions exist parser-side
    (SilvaGLR.positio = breaking token index; commissio error nodes
    carry segment tokens) — a separate, parser-side task the on-ramp
    had not scoped. v0 must at minimum decide its honest fallback
    (whole-file? line 1? first error-node token?).
26. **Conversions**: LSP 0-based vs silva 1-based → −1 on line AND
    column (two mandatory off-by-ones). Tabs: silva counts 1 column
    per byte — matches LSP default tab-as-one-character. 0/0 means
    "no range", never "line 0". API-injected radices (genus API,
    byte_offset −1) correctly yield 0/0.
27. Closed questions: quaestio is pattern-matching only (no spatial
    query — irrelevant); no position→token index exists anywhere
    (unneeded for v0 if ranges are fixed at the source);
    _lexema_primum is static/unexported and _lexema_ultimum doesn't
    exist (fix ranges at the source instead of re-walking downstream).

## IV. CLIENT INTEGRATION (cliens)

28. **Registration = plugin `.lsp.json`**: `{"silva-lsp": {"command":
    ..., "args": [...], "extensionToLanguage": {".c":"c",".h":"c"}}}`.
    Optional: transport(stdio default), env, initializationOptions,
    settings, startupTimeout, shutdownTimeout, restartOnCrash
    (default true), maxRestarts, **diagnostics:false** (keeps
    navigation, suppresses diagnostic injection — a real knob for us).
    restartOnCrash/shutdownTimeout need CC ≥2.1.205 (box: 2.1.207).
29. **First-registered-wins, not replacement**: two enabled servers
    claiming .c = unspecified race. Concrete move: disable clangd-lsp
    (`enabledPlugins` false in repo .claude/settings.json — the
    typescript-lsp:false line proves the mechanism) + register
    silva-lsp. There is NO built-in default C server to "replace";
    clangd is itself a plugin.
30. **Two consumption channels**: (a) automatic edit-scoped
    diagnostics push (agent sees them same-turn; user via Ctrl+O);
    (b) the callable LSP tool (definition/references/hover/symbols/
    call-hierarchy) — inactive until a code-intelligence plugin
    covers the language. Confirmed present as a deferred tool in the
    main session harness.
31. **Dev loading without publishing**: `--plugin-dir <path>`, or
    local marketplace (`/plugin marketplace add ./dir`). Plugin files
    are CACHED on install — command must be in PATH or
    ${CLAUDE_PLUGIN_ROOT}-relative; a script cd-ing into the repo is
    brittle under marketplace install (fine under --plugin-dir dev).
    `/reload-plugins` applies without restart.
32. **Debugging the handshake**: `claude --debug` prints LSP skip
    reasons + negotiation; /plugin Errors tab; Ctrl+O shows received
    diagnostics. Harness-free golden transcripts: scripted
    Content-Length JSON-RPC over stdio (initialize → didOpen → expect
    publishDiagnostics; assert bytes) — the house golden-transcript
    pattern (M4b sessio suite) transfers directly. pytest-lsp/
    lsp-devtools exist if an external harness is ever wanted.
33. **Current baseline**: compile_flags.txt (repo root) = full house
    flags + all -I roots (clangd's input; no .clangd file; no LSP
    keys in repo settings). examen-custos hook: PostToolUse
    Write|Edit, .c only (.h SKIPPED — judged via includers),
    excludes scratchpad/build/fixa/amalgama + pinned exclusiones.txt,
    runs examen.sh -machina, drops infra tier, caps 10 rows,
    silent-when-clean, measured 0.46–0.47s.
34. Severity mapping sketch (for the spec): violatio→1 Error,
    suspectum→2 Warning, domesticum→3 Info, infra→drop (hook
    precedent) or 4 Hint; source:"silva"; code:ExamenCodex;
    provisionale → message suffix (hook precedent).

## NAMED UNKNOWNS / RISKS (consolidated)

- **R1 (dominant, engine)**: header re-lex not amortized (~0.10s per
  analysis). The real daemon win needs a warm pre-lexed includendum
  set in contextus — silva-side data-model change; THE design
  decision. (Fact: text is cached append-only; lexemes rebuilt per
  parse.)
- **R2 (daemon lifecycle)**: OOM → exire(I) kills the daemon. Lean:
  accept-and-respawn (client restartOnCrash default true). Needs
  DECISUS.
- **R3 (invalidation)**: coarse full-rebuild on .h change (~0.05s,
  acceptable); unsaved-.h-buffer case named.
- **R4 (positions)**: does Claude Code's client offer utf-8
  positionEncoding? Observe first live initialize. utf-16 fallback =
  per-line rescan, surrogate-free for this corpus.
- **R5 (scope)**: syntax-error positions absent (0:0) — parser-side
  task, most common live-edit state. Scope decision needed.
- **R6 (value)**: if v0 is diagnostics-only, it duplicates the hook's
  trigger surface. What does the hook do afterward — retire, or stay
  as belt-and-suspenders? If navigation is in scope, what serves
  definition/references — nexus's batch index is disposable-TSV, NOT
  a warm silva query; M1a's nexus symbolorum export (use-site→symbol,
  per-parsura) is the likely substrate but was NOT explored (fifth
  subject, post-fork-scope).
- **R7 (registration)**: `strict:false` on marketplace LSP entries
  undocumented; whether Claude Code sends navigation requests to a
  server that advertises no such capabilities should be handled by
  negotiation, but v0 must answer any surprise request -32601, never
  hang.

## OPEN QUESTIONS FOR FRAN (pre-simulatio)

1. **Center of gravity**: diagnostics-only v0 (consolidate the hook's
   job into a warm daemon) vs diagnostics + navigation/hover (the LSP
   tool becomes the agent's warm bench — hover = M4b type renderer's
   natural consumer; the dogfooding rationale cuts this way)?
2. **Disable clangd-lsp now?** Free win, one settings line; ends the
   deliberate examen-vs-clangd live differential.
3. **R1 in scope?** Warm pre-lexed headers (silva-side contextus
   change, the fontes-loader "next touch" arriving) vs ship v0 at
   ~0.15s/analysis and park it (0.15s is already 3× faster than the
   hook's 0.46s process spawn).
4. **Hook disposition** if silva-lsp ships diagnostics: retire /
   coexist / hook becomes the fallback for headless contexts?
5. **Syntax-error positions (R5)**: v0 fallback choice, and whether
   the parser-side positioning task joins this milestone or is a
   named park.
6. Naming ceremony: the daemon's Latin name (the "silva-lsp" label is
   the milestone, not necessarily the binary).
