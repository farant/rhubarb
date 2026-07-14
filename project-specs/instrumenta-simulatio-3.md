# INSTRUMENTA-SIMULATIO-3 — MCP LEGATI (name-based pull server, one binary)

2026-07-14. Pretend-build of MCP LEGATI against the design pinned in
the post-outgoingCalls conversation: **one binary** (legatus grows a
`-mcp` mode), **resident per session** (harness-managed stdio
lifecycle), **lazy revalidation** (every answer checks its watch
before speaking), v0 tool surface = diagnostica / symbolum /
vocantes / vocata. ~2¾ simulated hours, four chunks.

Real-code probes during the sim: tabellarius framing is exactly two
functions (epistulam_legere :55 fixed-256B header lines + fread body;
epistulam_scribere :143); legatus_currere main loop = per-message
piscina → read → parse → dispatch (legatus.c:4547-4574);
_indicem_onerare loads build/nexus.tsv (48.2 MB, 719,729 rows) once
into piscina_indicis at initialize (:689); _superpositio_recipere is
a pure row callback fed by the same nexus_ordines unit as the sweep
(:777); _extenta_viae's closed-file path already does standalone
praeparator_analysare then THROWS AWAY diagnostics and rows (:3369);
_evadere_json escapes \n \r \t " \ (chorda_aedificator.c) but ONLY
compact serialization avoids raw newlines (pretty mode emits them,
json.c:1625); legatus.sh is stderr-only and ends `exec "$BIN" "$@"`
(args flow through free); LSP radix comes from initialize rootUri —
MCP initialize HAS NO rootUri.

HEADLINE: the design SURVIVED — no pinned decision reversed. The
sim's crown find is a SIMPLIFICATION: **MCP mode is document-free,
which declaws the praeparatio-rebuild trap** (the C11 stale-tree
ordering that haunted sessio/LSP) — no long-lived tree pointers
exist, so the scariest cache in the lattice becomes safe to rebuild.
Second find: the revalidation lattice collapses into a three-tier
EPOCH LAW with strictly nested lifetimes. Third: one new primitive
(`_recensere`, the on-demand judge) feeds three consumers per parse
— diagnostics, overlay rows, extents — and can harvest signature
strings at judge time, turning the extents cache into symbol cards.
Cost honesty: ~1,000 new lines land in legatus.c (→ ~5,600) — the
unit-split question is now REAL and goes to the interview.

## Complications (four-part schema)

**C1 — newline framing is a SERIALIZATION LAW, not just a framing
function.**
- discovered-while: writing tabellarius_lineam_scribere; asked "what
  guarantees the body has no raw '\n'?"
- consists-in: MCP stdio = one JSON message per line. Our compact
  serializer (json_scribere) never emits raw newlines and
  _evadere_json escapes \n \r \t in string values — but
  json_scribere_pulchrum DOES emit raw newlines. One pretty-print
  call anywhere in the MCP response path corrupts the channel.
- consequences: framing correctness depends on a global discipline,
  not a local function.
- handled-by: tabellarius_lineam_scribere ASSERTS no raw '\n' in
  corpus (walk once, ~free at these sizes) and refuses with stderr
  clamor rather than corrupting the stream. Golden: a diagnostic
  whose message contains a tab/newline round-trips escaped.

**C2 — MCP initialize has no rootUri: where does radix come from?**
- discovered-while: writing _mcp_initialize; LSP mode gets radix
  from params.rootUri, MCP initialize params carry only
  protocolVersion/capabilities/clientInfo.
- consists-in: every fopen, index load, and via normalization needs
  the absolute repo root; getcwd(3) would be a NEW POSIX call
  (seam growth for one string the launcher already knows).
- consequences: without it, MCP mode cannot even load the index.
- handled-by: legatus.sh appends `-radix "$RADIX_DIR"` to the exec
  args always (LSP mode ignores it — rootUri wins); principale
  passes it through LegatusConfiguratio. Zero new POSIX. The
  .mcp.json entry is just `./officina/legatus.sh -mcp`.

**C3 — overlay-without-document: the entry point is entangled with
the document lifecycle, but the callback is not.**
- discovered-while: designing per-via revalidation; today overlay
  rows are only produced inside the didOpen/didChange analysis path.
- consists-in: _superpositio_recipere is a pure callback (row in,
  index insert) fed by the same nexus_ordines unit as the sweep —
  nothing about it needs a LegatusDocumentum. Meanwhile
  _extenta_viae's closed-file path ALREADY runs the full standalone
  parse and then discards diagnostics and rows it could have kept.
- consequences: the machinery exists but is split across two homes;
  building revalidation as a third copy would be the newest_header
  mistake all over again.
- handled-by: NEW PRIMITIVE `_recensere(l, via)` = read from disk +
  praeparator_analysare + necare old rows + emit overlay rows +
  refresh extents (+ harvest symbol cards, C9) + record via→mtime,
  trees destroyed before return. _extenta_viae's closed-file branch
  REFACTORS ONTO IT (net code shrinks); diagnostica renders its
  diagnostic walk. One judge, three consumers, one home.

**C4 — lazy re-judge invalidates the rows you are holding: the
two-pass lookup.**
- discovered-while: symbolum(titulus) — you can't know WHICH viae to
  revalidate until you've looked up the titulus; but revalidating
  kills and replaces the very chain rows the lookup returned.
- consists-in: lookup → discover involved viae → any dirty? →
  _recensere them → the old LegatusOrdo chain now contains mortuus
  rows and fresh rows it doesn't know about.
- consequences: answering from the pre-revalidation chain returns
  stale lines (the exact silent-empty failure today's outgoingCalls
  probe hit).
- handled-by: TWO-PASS LAW: pass 1 collects involved viae + dirty
  check; if anything was re-judged, RE-RUN the lookup (chains are
  cheap hash walks); pass 2 answers. Bounded at two passes because
  _recensere records the mtime it judged — a file can't be dirty
  twice in one request. Every name-based tool gets the same shape.

**C5 — SIMPLIFICATION: document-free mode declaws the praeparatio
rebuild.**
- discovered-while: writing the cache table and reaching the scary
  row — "praeparatio rebuild frees piscina_capitum while X holds
  pointers into it"; enumerated X.
- consists-in: in LSP mode X = every open document's parse tree
  (hence _didsave_tractare's rigid three-step C11 dance). In MCP
  mode there ARE no documents: _recensere trees are ephemeral
  (destroyed before return), extents/symbol-cards are transcribed
  into piscina_indicis, index rows likewise. NOTHING outlives a
  request that points into piscina_capitum.
- consequences: praeparatio rebuild in MCP mode = destroy + rebuild,
  no ordering ceremony. The lattice's scariest invalidation is its
  safest.
- handled-by: nothing to handle — record WHY it is safe (this
  paragraph) so a future documents-in-MCP feature knows what it
  re-arms.

**C6 — the EPOCH LAW: three nested cache tiers, invalidation kills
downward.**
- discovered-while: ordering the per-request checks; sibling caches
  with independent invalidation rules produced a 3×3 case matrix
  nobody wants to test.
- consists-in: declare lifetimes NESTED BY POLICY:
  TIER 1 praeparatio (capita) ⊃ TIER 2 index (tsv + overlays +
  extents/cards + via-mtime table, all in piscina_indicis) ⊃
  TIER 3 per-via freshness. Checks per tools/call, in order:
  ① any praeparatio caput mtime ≥ praeparatio timestamp → rebuild
  praeparatio AND reload index (tier 1 kills tier 2);
  ② build/nexus.tsv mtime changed → reload index (piscina_indicis
  destroyed wholesale — overlays, cards, mtime table die together,
  correctly, because they were derived under the old index);
  ③ involved-via mtime ≥ recorded → _recensere that via.
- consequences: the case matrix collapses to a line; the invariant
  is auditable ("nothing in tier N+1 survives a tier-N event").
- handled-by: one function `_aetates_custodire(l, ...)` runs ①② at
  request entry (~50 stats, <1ms); ③ runs inside lookups (C4).

**C7 — index reload is cheap enough to be honest.**
- discovered-while: costing tier-2 events. 48.2 MB / 719,729 rows;
  spawn-time load is the same code and feels ~1s.
- consists-in: post-commit hook regenerates the tsv (branch switches
  and pulls change MANY files — overlays alone cannot cover that);
  the LSP resident today just goes quietly stale after commits.
- consequences: without reload, MCP answers after a commit are wrong
  in the worst way (confidently stale). With it, the first request
  after a commit pays ~1s.
- handled-by: reload on tsv-mtime change, wholesale piscina swap
  (nothing outside piscina_indicis points in — verified). The ~1s
  is disclosed in the result ("index renovatus post commissionem").
  PARK (named): backport the same reload trigger to LSP mode —
  today's resident keeps a pre-commit index forever.

**C8 — mtime polarity: the vigilia and the revalidator want OPPOSITE
comparisons.**
- discovered-while: reusing praeparator_tempus_plagulae; the vigilia
  uses strict > (coeval binary = fresh, avoids false nags on
  edit+rebuild-same-second — the lazy-spawn find).
- consists-in: for CACHE freshness the failure directions invert: a
  false-fresh answer is a lie, a false-stale re-judge costs ~100ms.
  Edit + ask within the same second with equal mtimes must re-judge.
- consequences: one shared helper with two polarities = a foot-gun
  waiting for a refactor to "clean up the inconsistency."
- handled-by: revalidation compares ≥ (coeval = suspect) and the
  comparison sites carry a comment naming the OTHER polarity and
  why. Worst case: one spurious re-judge per same-second window.
  (Nanosecond mtimes via st_mtimespec = named improvement in the
  posix seam file if this ever measures as churn.)

**C9 — symbolum needs a live sem for signatures; harvest at judge
time instead.**
- discovered-while: symbolum(titulus) wants the hover-style
  signature; hover renders from an OPEN document's semantica.
  Closed-file signature = re-parse per question (~100-300ms) for a
  string that was available FREE during the last _recensere.
- consists-in: extents entries grow into SYMBOL CARDS:
  {titulus, linea_a, linea_b, signatura (rendered string), genus}
  harvested for depth-0 symbols during every _recensere, stored in
  piscina_indicis (dies with tier 2 — correct).
- consequences: symbolum answers from cache in O(lookup); first
  question about a never-judged file pays one _recensere (which
  also freshens everything else about that file).
- handled-by: card harvest inside _recensere; macro branch reads the
  #define line + vista corpus extents on demand (v0.2 machinery,
  already document-free).

**C10 — result text needs caps, and the no-silent-caps law applies.**
- discovered-while: vocantes(piscina_allocare) — thousands of uses;
  a tool result is agent context, tokens are the budget.
- consists-in: uncapped answers blow context; silently capped
  answers read as complete (the excubitor lesson).
- consequences: every list-shaped answer needs a cap AND a visible
  remainder note.
- handled-by: nexus-style grouped-by-file output, per-file and total
  caps, explicit "et alia N in M plagulis" tail lines. Numbers are
  v0 constants; a verbosum knob is a named park.

**C11 — legatus.c crosses ~5,600 lines: the unit-split question is
now real.**
- discovered-while: tallying the sim's line budget (~150 dispatch +
  ~150 _recensere + ~200 lattice + ~400 tools/schemas + ~100
  framing/probatio glue).
- consists-in: one file holding two protocol surfaces + index +
  caches + judge plumbing. interior functions make extraction
  nontrivial (today's outgoingCalls reused SIX of them).
- consequences: either accept the growth for v0 or split first
  (index+caches unit / protocol surfaces unit / tools unit) and eat
  the header ceremony + build-script rows + excubitor fabrica rows.
- handled-by: INTERVIEW. Sim's lean: v0 in-file with banner
  sections (ship the value, split at the LEGATUSD refactor where a
  daemon forces the seam anyway) — but it is a taste call and
  Fran reads all the code.

**C12 — what the sim cannot test: the harness side.**
- discovered-while: throughout; every "does Claude Code…" question.
- consists-in: unknowables until contact: whether tools/list is
  fetched once or per-turn; how tool descriptions render in MY
  context (token weight of schemas); /mcp reconnect behavior vs
  the vigilia nag; spawn cwd and env for project-scope .mcp.json;
  whether initialize `instructions` actually lands in agent context;
  approval-prompt UX for the new server.
- consequences: first-contact shakedown list, exactly like the LSP
  shakedown (which took three sessions to map didOpen scoping).
- handled-by: written as the SHAKEDOWN list in the chunk plan; the
  vigilia + stderr channel give live introspection from minute one.

## The five-minute log (compressed where uneventful)

**CHUNK A — framing + handshake (T+0:00–0:40)**
- T+0:05 tabellarius recon: the whole framing seam is two functions.
  Decide: no mode enum, two NEW functions (lineam_legere/scribere) —
  framing is the caller's choice per call site, no library state.
- T+0:10 lineam_legere: fgetc loop into ChordaAedificator (unbounded
  — the 256B fixed buffer is a header-line artifact, bodies need
  growth), strip trailing \r defensively, EOF mid-line = finitus,
  skip blank lines (spec says they shouldn't exist; be liberal).
- T+0:15 lineam_scribere: body + '\n' + fflush. The "what forbids
  embedded newlines" question → C1 (assert + refuse).
- T+0:20 legatus_currere fork: cfg.modus_mcp → read via lineam,
  dispatch via _mcp_tractare. principale parses -mcp and -radix →
  C2 (launcher passes -radix always). legatus.sh: ZERO changes
  beyond that one appended arg; stdout hygiene already guaranteed
  (stderr-only + exec).
- T+0:25 _mcp_tractare skeleton: initialize / notifications/
  initialized (ignore) / ping / tools/list / tools/call / -32601.
  Response envelopes reuse tabellarius_responsum (envelope layer is
  framing-agnostic — the split earns its keep).
- T+0:30 initialize result: protocolVersion echo policy (echo the
  client's if known shape, else ours), capabilities {tools:{}},
  serverInfo legati/0.1, `instructions` field = a paragraph of bench
  doctrine injected into the client — free doctrine channel, write
  it at ship time.
- T+0:35 MCP initialize must also do LSP-initialize's setup work
  (radix, praeparatio, index, vigilia). Same code path, one flag.
  Vigilia nag goes into every tool result as a CAUTIO line (not a
  diagnostic — MCP has no diagnostics channel).
- T+0:40 chunk A ends. Budget: tabellarius +~40, legatus +~150.

**CHUNK B — tools/call plumbing + diagnostica (T+0:45–1:20)**
- T+0:45 tools/list: 4 tool literals with inputSchema
  (-Wno-overlength-strings already on). Descriptions: language →
  interview (sim assumes Latin name + one-line Latin description
  with an English gloss clause).
- T+0:50 tools/call: params.name dispatch; arguments object;
  result {content:[{type:"text",text}]}; isError=true for tool-level
  failures (via not found), JSON-RPC error for protocol-level.
- T+0:55 diagnostica(via): normalize via (absolute → strip radix;
  "./" strip; reject outside-radix), then _recensere + render the
  diagnostic walk as examen-style text (verdict line + positioned
  rows, INFRA suppressed like the LSP publish).
- T+1:00 realized: diagnostica IS warm examen — output format parity
  with examen.sh is a cheap consistency win (agent already reads
  that shape). → interview confirm.
- T+1:05 the three-consumers find lands (C3): _recensere feeds
  diagnostics + overlay + extents in ONE parse; _extenta_viae's
  closed-file branch refactors onto it, net shrink.
- T+1:10 piscina audit of _recensere: effimera per judge (8MB, the
  _extenta_viae precedent), rows/cards transcribed into
  piscina_indicis, trees destroyed → C5 falls out (document-free =
  no capitum pointers survive).
- T+1:15 probatio shape: new probatio_officina_legati.c, newline
  variants of _scribe/_lege, goldens: handshake, list, diagnostica
  on a phantasma with a known error + a message containing '\t'
  (C1 bar).
- T+1:20 chunk B ends.

**CHUNK C — the revalidation lattice (T+1:25–2:00)**
- T+1:25 wrote the cache inventory table; the praeparatio row is the
  scary one → enumerated pointer-holders → C5 (already banked).
- T+1:30 who-checks-when: request-entry checks vs per-lookup checks.
  You can't know involved viae until you look up the titulus → C4
  two-pass law.
- T+1:35 via-mtime table (via → last-judged mtime) lives in
  piscina_indicis ON PURPOSE (dies with tier 2). Polarity question
  → C8 (≥ here, > in vigilia, comment both sites).
- T+1:40 tsv reload: stat per request; on change destroy
  piscina_indicis wholesale + _indicem_onerare fresh. Enumerated
  outside pointer-holders: none in MCP mode. ~1s disclosed → C7.
  LSP backport = named park.
- T+1:45 ordering collapsed into the EPOCH LAW (C6): tier 1 ⊃ tier 2
  ⊃ tier 3; one _aetates_custodire at request entry (~50 stats),
  tier 3 inside lookups.
- T+1:50 failure honesty: _recensere on a syntax-broken dirty file →
  old rows necati, no fresh rows; answers carry "plagula X non
  analysabilis" CAUTIO instead of silence. (Fail-open-with-words vs
  today's fail-closed-silent.)
- T+1:55 cost budget per call: <1ms stats typical; +100-300ms per
  dirty via; +~1s on commit boundary. All disclosed when paid.
- T+2:00 chunk C ends. ~200 lines + _recensere ~150.

**CHUNK D — the name-based tools (T+2:05–2:45)**
- T+2:05 vocata(titulus): body-def policy → defining via →
  tier-3 check → extents + the outgoingCalls walk, name-first (no
  item, no range, no prepare). Output: grouped by callee with
  via:linea, nexus-style.
- T+2:10 vocantes(titulus): incoming walk; revalidate only DIRTY
  using-viae (bounded by the existing 20-file analysis ceiling).
- T+2:15 symbolum(titulus): sedes list (body-def first) + genus +
  signatura from the SYMBOL CARDS (C9) + per-file use counts +
  macro branch (#define line + corpus extents). Composite answer,
  one call.
- T+2:20 similitudo fallback on symbolum miss (the vicinitas
  pattern, flat acies) — comment words and typos get directions,
  same as hover.
- T+2:25 caps + tail lines everywhere (C10).
- T+2:30 probatio: staleness scenario (write phantasma, judge, edit
  it, re-ask → fresh lines), tsv-swap scenario (rewrite tsv → reload
  fires, overlays die), utime tricks per the vigilia probatio
  precedent.
- T+2:35 .mcp.json + registration flow + the SHAKEDOWN list (C12).
- T+2:40 line-count tally → C11 (unit split → interview).
- T+2:45 END.

## Scope drops & simplifications banked

- NO getcwd / new POSIX (C2 launcher arg).
- NO watcher thread, NO fsevents — lazy revalidation only.
- NO document layer in MCP mode (C5 — and the trap stays declawed
  only while this holds; recorded).
- NO excubitor-verdict tool in v0 (bash tool exists; vigilia BFS
  generalization = v0.1 candidate).
- NO fuzzy-search tool (symbolum's similitudo fallback covers the
  need; nexus -similis exists for the bench).
- Envelope layer (tabellarius_responsum/errorem) reused UNCHANGED —
  only framing forks.

## Revised chunk plan (real implementation)

| chunk | contents | bar |
|---|---|---|
| 1 | tabellarius newline mode + -mcp/-radix + handshake + tools/list + ping | golden: handshake + list over newline framing; C1 escape golden |
| 2 | _recensere primitive (three consumers + cards) + epoch lattice (_aetates_custodire, via-mtime table, tsv reload) + _extenta_viae refactor onto _recensere | staleness goldens: dirty-via re-judge, tsv-swap reload, LSP suite STILL GREEN (the refactor touches shared code) |
| 3 | diagnostica + symbolum + vocantes + vocata + caps + similitudo fallback + .mcp.json + instructions | tool goldens ×4 + live shakedown in-session (C12 list) |

Estimate: 3 chunks, comparable to the excubitor arc's day. The LSP
suite staying green through chunk 2 is the load-bearing bar — the
refactor onto _recensere touches the LSP's own closed-file path.

## Interview seeds

1. C11 — split legatus.c into units now, or v0 in-file and split at
   LEGATUSD? (sim leans in-file; Fran reads all the code, his call.)
2. Tool descriptions + result text language: pure Latin, or Latin
   with English gloss? (agent-facing text; the agent reads both.)
3. diagnostica output parity with examen.sh — exact same verdict
   format, or MCP-flavored?
4. Caps defaults (per-file / total) and whether a verbosum knob is
   v0 or park.
5. The `instructions` doctrine paragraph — who drafts it, what goes
   in (bench symptom table pointer? tool-choice guidance?).
6. LSP index-reload backport (C7 park) — same arc or later?
7. Server name in .mcp.json ("legati"?) — tool names become
   mcp__legati__symbolum etc.
8. protocolVersion pin — hardcode latest known, or echo client's?
   (sim: echo if parseable, else ours; low stakes, confirm.)
