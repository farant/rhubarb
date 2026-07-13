# silva-lsp (LEGATUS) — simulatio

2026-07-13. Pretend-implementation per the house technique (sim-8 in
the officina lineage). Inputs: silva-lsp-fundamenta.md (34 facts),
silva-lsp-interrogatio.md (16 DECISUS). Simulated ~2h50m across:
tabellarius → legatus skeleton → diagnostics lane → ranges → hover →
documentSymbol → syntax errors → packaging → golden harness.
Complications in the four-part schema (discovered-while /
consists-in / consequences / handled-by). Grounding greps/reads were
REAL (json.h, praeparator.h, silva.h exports, hook script).

**VERDICT: zero reversals against the interrogatio DECISUS.** Two
scope-shape updates (C4: extent promotion becomes v0-REQUIRED; C13:
the syntax-error "timeboxed attempt" looks genuinely cheap), one OPEN
question for Fran (C5: type renderer home), one honest correction to
the fundamenta (its "no spatial index needed for v0" claim dies at
hover — but the index is small and per-document).

---

## C1 — JSON-RPC id is integer-OR-string

- **Discovered while**: sketching the tabellarius envelope.
- **Consists in**: responses MUST echo the request id with its
  original type; converting to int corrupts string ids.
- **Consequences**: envelope cannot store id as s64.
- **Handled by**: id = `JsonValor*` passed through verbatim
  (json.c's tagged union distinguishes JSON_INTEGER/JSON_CHORDA —
  Fact 3 of fundamenta pays off). Batch arrays (JSON-RPC 2.0
  feature LSP never uses): reject with InvalidRequest, named
  non-goal.

## C2 — URI ↔ via mapping is real work nobody owns yet

- **Discovered while**: writing the didOpen handler.
- **Consists in**: LSP speaks `file:///Users/...` with
  percent-encoding; silva speaks repo-relative `./lib/piscina.c`
  (that's what exclusiones.txt rows and diagnostic via echoes use).
  Needed: decode %XX + strip scheme + make radix-relative, and the
  inverse for publishing. lib/via.h has path joins/normalization but
  NO URI codec.
- **Consequences**: every message crosses this boundary twice; a
  mismatch breaks exclusion matching and via filtering silently.
- **Handled by**: small uri_ad_viam / viam_ad_uriam pair INSIDE
  legatus (LSP-domain knowledge stays out of tabellarius); the
  repo-relative form is the canonical internal via, chosen to match
  exclusiones.txt exactly.

## C3 — cross-file diagnostics: analysis of X emits rows with via=header

- **Discovered while**: walking diagnostics into publishDiagnostics.
- **Consists in**: SemanticaDiagnosticum.via is the OFFENDING
  token's root file — judging lib/foo.c can produce rows positioned
  in chorda.h. publishDiagnostics is per-URI; publishing header rows
  under foo.c's URI with header line numbers = lying squiggles.
- **Consequences**: unfiltered, every header hiccup paints the open
  document at wrong positions.
- **Handled by**: v0 publishes only rows with via == the analyzed
  document; dropped rows counted to stderr. PATH BACK NAMED:
  group-by-via multi-publish (LSP allows publishing for any URI) —
  v0.1 candidate alongside definition/references.

## C4 — hover needs token→NODE; extent promotion becomes v0-REQUIRED

- **Discovered while**: hover part 2 — had position→token (sorted
  radix-filtered index over parsura->lexemata, built per version),
  but typationes and symbolum_nodi are keyed by NODUS, and no public
  token→node map exists.
- **Consists in**: the only honest node lookup is tree descent by
  extent: recurse from root elementa into the child whose
  [min,max) byte range contains the target offset — which requires
  the whole-subtree extent utility (sessio's _extensionem_valoris,
  the M4a park).
- **Consequences**: the fundamenta's "no spatial index needed for
  v0" claim is DEAD; the extent promotion moves from
  nice-to-have to v0-required. (It also serves documentSymbol
  ranges and C13 error-node positions — one utility, three
  consumers.)
- **Handled by**: promote as planned (pure move, public-API-only
  code) + line/column variant; per-document-version position index
  = {line-start table (shared with utf-16 fallback), sorted token
  extents}. Both built once per analysis, ~O(tokens).

## C5 — TypusC89→text renderer is sessio-PRIVATE — OPEN: where does it live?

- **Discovered while**: rendering hover contents.
- **Consists in**: the Latin C-style type renderer (_typum_scribere
  family, M4b chunk C) is static inside officina/instrumenta/
  sessio.c. Hover needs exactly it (`s : chorda*`), amalgam grep
  confirms NO public equivalent.
- **Consequences**: copy = the divergent-twins disease praeparator
  just cured; promotion needs a home decision.
- **Handled by**: rule of two is satisfied (sessio + legatus).
  Options: (a) extract to officina/instrumenta shared unit
  (scriptor_typorum.{c,h}, praeparator precedent), (b) promote
  silva-side as public silva_c89_typum_scribere (the eventual home —
  it renders silva's TypusC89). **OPEN FOR FRAN** — sim leans (a)
  now, (b) named future (silva-side promotion is an amalgam ceremony
  mid-milestone).

## C6 — golden transcripts: no subprocess/pipe precedent — dissolved by engine API

- **Discovered while**: sketching the formal bar; grep found no
  popen/fork+pipe pair anywhere in probationes/lib.
- **Consists in**: driving the BINARY over bidirectional pipes from
  a credo probatio = new fork/exec/dup2 machinery.
- **Consequences**: harness cost, flakiness surface.
- **Handled by**: the sessio precedent — legatus is an ENGINE with a
  thin main: `legatus_currere(FILE* intra, FILE* extra, cfg)` in
  officina/instrumenta/legatus.{c,h}; binary = principalia wrapper.
  Probatio drives the engine in-process over tmpfile/fdopen'd pipe
  FILE*s (sessio's ansae pattern). The stdio-binary path is covered
  by the daily-driver bar.

## C7 — document store lifecycle: piscina-per-version

- **Discovered while**: didChange handler; praeparator.h contract
  "fons vivere debet quamdiu arbores" (text must outlive trees).
- **Consists in**: piscinae have no free; replacing text per
  didChange in one arena grows unboundedly over a long session.
- **Consequences**: daemon memory creep.
- **Handled by**: one piscina per document VERSION holding text +
  position index + analysis trees + semantica together; destroyed
  wholesale when the next didChange arrives (or didClose). Bounded:
  live memory = open documents × 1 version each.

## C8 — initialize carries the radix; posix is a config knob

- **Discovered while**: initialize handler.
- **Consists in**: rootUri/workspaceFolders → Praeparatio radix;
  positionEncodings array scan picks utf-8 else utf-16. Judgment
  dialect: hook parity requires ISO default (examen.sh default);
  POSIX-transitive files are pinned exclusions anyway.
- **Handled by**: cum_posix=FALSUM default; `initializationOptions:
  {"posix": true}` named as the .lsp.json knob (plugin config plumbs
  straight through — no code beyond a json_objectum_capere).
  Files outside radix → publish empty (exclusion discipline).

## C9 — didChange payload defensiveness

- **Consists in**: Full sync sends contentChanges as an array; spec
  says whole text = single element.
- **Handled by**: take the LAST element's text; ignore rangeful
  elements (we advertised Full). One `si`, recorded so nobody
  "fixes" it into incremental accidentally.

## C10 — stdout purity + startup cost (C14 merged)

- **Discovered while**: packaging pass.
- **Consists in**: ONE byte of non-protocol stdout corrupts the
  channel; house build scripts chatter (already to stderr, colloquium
  precedent — keep it that way); legatus binary must never
  printf(stdout). Separately: cold compile at first spawn could blow
  startupTimeout.
- **Handled by**: all legatus logging → stderr; launcher
  ./officina/legatus.sh = colloquium.sh-pattern incremental build
  (warm exec ~instant) + documented one-time prebuild; raise
  startupTimeout in .lsp.json as belt-and-suspenders. Plugin dir for
  dev = in-repo directory loaded via --plugin-dir (paths stay valid;
  marketplace-cache portability = named non-goal for v0).

## C11 — .h rebuild ordering: the stale-object trap, pre-avoided

- **Discovered while**: .h didSave handler; the debugging-lessons
  stale-object row itched.
- **Consists in**: open documents' parsurae/semanticae hold pointers
  INTO the warm Praeparatio's ctx piscina (praebenda text). Destroy
  Praeparatio → every open doc's trees dangle.
- **Consequences**: use-after-free on the very next hover.
- **Handled by**: rigid order (demoliri discipline, M4b precedent):
  ① destroy ALL open documents' version piscinas (trees die first)
  ② destroy + rebuild Praeparatio (~0.05s) ③ re-analyse and
  re-publish every open document. Single-threaded loop makes this
  atomic from the client's view.

## C12 — exclusion matching must be byte-faithful to the hook

- **Consists in**: exclusiones.txt rows are `./lib/foo.c` form;
  the hook also hard-filters scratchpad/build/fixa/amalgama.
- **Handled by**: same file, same row format (C2's canonical
  internal via chosen to match), same path filters, shared source of
  truth. Excluded/outside/oversized → publish EMPTY array (clears
  stale squiggles too — didClose does the same).

## C13 — syntax-error positions: the timebox looks like it LANDS

- **Discovered while**: error lane; SILVA_C89_GENUS_ERROR = 53 is a
  PUBLIC genus in the amalgam.
- **Consists in**: parse failures → error nodes IN root elementa;
  first origo-root token with byte_offset ≥ 0 gives an honest
  position; C4's extent walker gives the range. Same machinery, no
  parser surgery visible.
- **Consequences**: the interrogatio's fallback (degenerate line-1
  range) likely never fires; keep it as the code path for
  all-synthetic-token error nodes (API-injected radices).
- **Handled by**: planned as the primary path; fallback intact;
  message carries parsura->numerus_errorum.

## C14 — documentSymbol: does SemanticaSymbolum carry its nodus?

- **Discovered while**: symbol walk sketch; the struct shows
  {genus, titulus, typus, valor, repositio, profunditas,
  ex_systemate, ...} — tail fields unverified, no nodus SEEN.
- **Consists in**: without a node pointer, symbols have no
  POSITION → no LSP range.
- **Consequences**: if absent, the symbol-table walk can't serve
  documentSymbol alone.
- **Handled by**: verify at build; SAFE PATH regardless = root
  elementa walk (extent per element via C4's walker + name via the
  probe machinery sessio proved) filtered fons_princeps +
  profunditas-0 semantics. SymbolKind map: FUNCTIO→12, structura
  tag→23, enum→10, typedef→5(Class, LSP has no typedef kind),
  variable→13. Hover format = colloquium's echo format
  (`titulus : typus`) — the two tools speak one house voice.

## C15 — unknown-position rows still need a legal range

- **Consists in**: LSP Diagnostic REQUIRES a range; rows with
  linea=0 (API-injected origins; pre-C13 syntax errors) have none.
- **Handled by**: degenerate {0,0}-{0,1} + "(positio ignota)"
  message suffix; expected to be rare once C13 lands.

---

## Scope-shape deltas vs interrogatio (no reversals)

1. Extent utility: optional → **v0-REQUIRED** (hover descent,
   docSymbol ranges, error-node ranges — three consumers).
2. Syntax-error attempt: timebox → **likely lands** (public GENUS_
   ERROR + same extent machinery); fallback stays as written.
3. Per-document position index (line table + token extents) joins
   the design — small, per-version, dies with its piscina.

## OPEN for Fran (carried to spec unless answered)

- C5: type renderer home — officina shared unit now (sim's lean) vs
  silva-side public promotion (eventual home, amalgam ceremony).

## Chunk shadow (for the spec, not binding)

A: lib/tabellarius (framing ∥ envelope, credo probatio) →
B: legatus engine core (loop, initialize/lifecycle, document store,
   diagnostics lane + exclusions + URI codec; golden bar v0) →
C: ranges + hover + documentSymbol (silva longitudo fix + extent
   promotion + renderer extraction + position index) →
D: syntax errors + .h lane + utf-16 fallback + packaging + seal
   (golden transcripts full + daily-driver registration).
