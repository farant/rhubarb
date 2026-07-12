# SIMULATIO-7 — M4b REPL (sessio engine, replay-document design)

2026-07-11. Pretend-build of the M4b REPL against the design that
emerged from the fundamenta + design conversation: document-as-truth,
replay-on-shape-change, green invariant, overlay/acta seam, engine
(sessio) + surfaces (line REPL "colloquium" [unsealed] + #! runner;
workbench + vindex-eval as named pulls). ~2¾ simulated hours, four
chunks. Real-code probes during the sim: _aed_printf stdout hardcode
(machinula.c:888) vs ansae[0/1/2] standard-stream slots (:128-130);
builtin table = name→fn rows (:1546); MachinulaExitus.codex = "valor
redditus" (machinula.h:51-55); machinula_creare takes caller Piscina
(machinula.h:61); onerator's three-piscina split (onerator:17-19).

HEADLINE: the replay design SURVIVED — no complication forced a
reversal. The always-reparse simplification HELD (v0 needs NO systema
chaining, NO oracle carry-forward, NO retained prior trees, NO tag
seam — fundamenta fact 2's plumbing gaps all drop out of v0 scope).
One lean REVERSED: overlay v0 tier (C11) — refuse-writes-by-name in
v0, umbra overlay = named v1 pull.

## Complications (four-part schema)

**C1 — classification requires parse; auto-`;` repairs more than
expressions.**
- discovered-while: writing turn intake — classify by last elementum
  genus, but a bare expression is a syntax error until `;` exists.
- consists-in: parse-as-is first; if new-range syntax errors AND
  retry-with-`;` parses clean, accept repaired form. But this also
  silently repairs `int x = 5` (forgotten semicolon) — DWIM vs
  silent-repair policy question.
- consequences: two parses per bare-expression turn (ms — fine); a
  policy decision leaks into UX.
- handled-by: accept any single-elementum repair but ECHO the
  repaired line. Default policy → interview.

**C2 — acceptance ≠ display (the green invariant's mechanical
form).**
- discovered-while: implementing "filter diagnostics to the new
  turn's line range."
- consists-in: a redefinition turn can make an OLD turn's line
  diagnose (arity change fires at turn 3's call site, outside the
  new range) — the display filter would hide the very violation
  that must reject.
- consequences: two separate rules needed, or the invariant is
  unsound.
- handled-by: ACCEPTANCE = whole-document verdict (any
  non-provisional violation anywhere rejects the turn); DISPLAY =
  new-range diagnostics + out-of-range ones summarized as "turnus k
  sub hac definitione aegrotat" with socius cross-link. Historical
  SUSPECTUM re-emissions (implicit calls re-diagnosed every reparse)
  are display-filtered by the same range rule; suspecta never REICE
  so acceptance is unaffected.

**C3 — the two-text problem (display vs lowered document).**
- discovered-while: statement turns judge at top level (liberal
  grammar) but medulla lowers only functions.
- consists-in: engine keeps the DISPLAY document (as typed — the
  artifact) and derives the LOWERED document (statement turns
  wrapped `vacuum __turnus_N(vacuum){…}`) by deterministic
  transform. DECISION: judge the WRAPPED text — statement-context
  legality arrives free (`return 5;` lands in a void wrapper →
  REDDE violation; the top-level-statement policy question mostly
  evaporates).
- consequences: a per-turn line-offset table mapping judged
  positions → display positions; must also cover error-node spans
  (syntax errors carry no positions through examen — fundamenta
  fact 16 — so the engine reads spans from the tree and maps them).
- handled-by: wrapper brace lines OWN lines; offset table per turn;
  one mapping function on the display path. Bounded, testable.

**C4 — the saved session is a DIALECT question.**
- discovered-while: writing :serva.
- consists-in: a display document with top-level statements is not
  strict C89 — it is the liberal dialect only our runner accepts.
  Alternative: :serva emits the WRAPPED form + synthesized
  principale calling turns in order = strict C89, clang-compilable
  (session → real program export). But then save/load isn't a clean
  round-trip of one text.
- consequences: artifact semantics; also defines what #! scripts MAY
  contain.
- handled-by: both emitters are cheap; default + flag → interview.

**C5 (VERIFY) — generational arena split.**
- discovered-while: world lifecycle design (perennial: library
  moduli, document text, trace; generational: document tree +
  semantica + document modulus + conexio + machinula + planae,
  destroyed per rebuild).
- consists-in: machinula_creare takes caller Piscina (verified,
  machinula.h:61); onerator already runs three piscinas (verified,
  onerator:17-19). UNVERIFIED: whether demissio/conexio co-allocate
  anything into a shared arena that would die with the generation.
- consequences: if co-allocation exists, the split needs one more
  piscina parameter somewhere — small, but must be found before
  build.
- handled-by: named verify-item V2 for the spec/chunk A.

**C6 — host FILE* hygiene at teardown.**
- discovered-while: teardown ordering (_generationem_demoliri:
  ansae sweep → regio_destruere → piscina destroy — order RIGID,
  fixed-base regio must be unmapped before next generare).
- consists-in: machinula's ansae[] may hold real host FILE*s; a
  rebuild without fclose leaks fds every shape-turn.
- consequences: long sessions exhaust fds.
- handled-by: engine sweeps ansae[3..] (0/1/2 are the standard
  streams) and fcloses before destroy. Mostly dissolves under the
  future umbra (overlay handles aren't host FILE*s); the sweep
  stays regardless.

**C7 (VERIFY) — value printing host-side via MachinulaExitus.codex.**
- discovered-while: expression-turn printing. codex documented
  "valor redditus" (machinula.h:53) → engine formats HOST-SIDE with
  the SEMANTICA static type — richer than interpretare's
  medulla-side printf (type names; chorda/char* display can read VM
  memory directly, region is host-addressable).
- consists-in: UNVERIFIED: does an F64 return arrive as bits in
  codex (s64)? Aggregates: refuse-by-name (interpretare precedent).
- consequences: if floats don't round-trip through codex, small API
  addition (fructus_out) or fall back to medulla-side print for
  floats only.
- handled-by: verify-item V3; either resolution is small.

**C8 — transactional teardown: judge BEFORE teardown; double
rebuild on replay-divergence.**
- discovered-while: shape-turn sequence. Regio fixed-base means the
  old world must be destroyed before the new one exists; if REPLAY
  of the new document halts at old turn k (redefinition broke
  history at runtime), the old world is gone.
- consists-in: recovery correct by determinism — rebuild the OLD
  document, state identical, guaranteed. Ordering discipline:
  judgment needs no world, so judge-rejections (the common case)
  never touch the world at all.
- consequences: a replay-divergence rejection costs two rebuilds
  (rare; acceptable). Fork-validation (child munmaps inherited
  regio, test-builds new world, parent untouched) = NAMED
  OPTIMIZATION, not v0.
- handled-by: strict operation order in the engine: parse → judge →
  [green?] → demoliri → build → replay → [halt at old turn? →
  rebuild old document, reject turn].

**C9 — dirty world after fast-path halts.**
- discovered-while: statement fast-path (execute just the new
  wrapper against the live world — licensed by determinism).
- consists-in: a halted statement turn has already partially
  mutated the live world; world ≠ replay(document).
- consequences: state corruption if ignored.
- handled-by: dirty flag; next turn rebuilds first (lazy). The
  halted probe's captured partial stdout IS shown (honest). Named
  optimization for later: snapshot globalia extent before fast-path
  turns, restore on halt (the acta snapshot primitive arriving as
  an optimization).

**C10 — the capture seam is ONE LINE of machinula.**
- discovered-while: chunk C, probing the real printf builtin.
- consists-in: _aed_printf hardcodes f.fluxus = stdout
  (machinula.c:888) while ansae[1] already IS the stdout slot
  (:128-130). printf ≡ fprintf(stdout) says it should read
  m->ansae[1] anyway — a semantic CORRECTION, not a feature. Engine
  then sets ansae[1] to a capture stream per turn.
- consequences: per-turn stdout capture (and stderr via ansae[2])
  costs almost nothing; the acta seam v0 = ansae redirection.
- handled-by: the one-line fix + engine-owned capture FILE*
  (tmpfile/funopen behind the platform layer). VERIFY V4: sweep for
  other hardcoded-stdout builtins (puts, putchar, perror).

**C11 — the umbra's real cost is BEING FILE*; v0 tier lean
REVERSED.**
- discovered-while: overlay interception design. Interception
  itself is easy (mode-flag branch in the file family + engine-owned
  path→buffer tabula_dispersa).
- consists-in: the expensive part is stdio SEMANTICS over buffers —
  fseek/ftell/fgets/ungetc/append-mode/partial reads. Reimplementing
  FILE* honestly is the actual build.
- consequences: tier-2 (write overlay) in v0 would dominate the
  milestone's new-code budget for a secondary feature.
- handled-by: v0 = TIER 1, refuse file-writing builtins by name in
  session mode ("scriptio plagularum in sessione — dilatio
  nominata", interpretare-refusal precedent); reads pass through
  live. The SEAM ships (mode flag + builtin-family routing); the
  umbra store = named v1 pull (acta-visio consumer list). This
  REVERSES the earlier tier-2 lean, with the cost now measured.

**C12 — script execution rule (#! runner).**
- discovered-while: chunk D, running the runner through the same
  engine in batch mode (blank shebang → parse → judge → wrap).
- consists-in: what does a script RUN? Rule: principale if present;
  else the top-level-statement schedule in document order — which
  makes C SCRIPTS WITH TOP-LEVEL CODE a real product of the
  document model.
- consequences: pleasant product surface; interacts with C4 (what a
  saved/loadable script may contain).
- handled-by: one rule in the engine's batch path; :aperi (load
  session = replay document, continue interactively) falls out free.

## Verify-list (before/at chunk A of the build)

- V1: does semantica check break/continue-outside-loop at all
  (wrapped judging relies on existing statement checks; REDDE is
  known-covered)?
- V2: demissio/conexio arena co-allocation (C5).
- V3: F64 through MachinulaExitus.codex (C7).
- V4: hardcoded-stdout sweep beyond _aed_printf (C10).
- V5: per-machinula builtin-table mutability (only if a swap-based
  seam ever beats the mode-flag branch — not needed for v0).

## What the sim confirmed / settled

- Replay design: NO reversals. Green invariant got its mechanical
  form (C2). Determinism used three separate times as a correctness
  argument (fast path ≡ replay; rebuild-old recovery; changed-history
  compare) — it is the design's load-bearing wall.
- Always-reparse: v0 silva-side work collapses to the `;` probe
  (C1), the wrap transform + position mapping (C3), and range-based
  display filtering (C2). No systema plumbing, no oracle
  carry-forward, no tag seam.
- New-machinery inventory for the spec: sessio engine (document,
  turns, generations, trace), the wrap transform + offset mapping,
  ansae capture seam (one line + engine stream), refusal flags
  (file-writes, nondeterministic builtins), host-side value printer,
  line surface, batch/#! path. Machinula API touches: printf→ansae[1]
  fix, session-mode flags, (maybe) F64 return channel.
- Turn lifecycle, final form: read (continuation by brace balance) →
  parse/repair (C1) → wrap+judge (C3) → whole-document verdict (C2)
  → [statement & clean world: fast-path execute | else: demoliri →
  rebuild → replay w/ trace compare (C8)] → capture/print (C7/C10)
  → append to document → prompt.

## Open questions surfaced FOR the interview (beyond fundamenta §III)

1. C1 repair policy: echo-and-accept vs strict (require `;`).
2. C4 save dialect: liberal document vs strict-C89 export (wrapped +
   synthesized principale) — which is default?
3. C11 confirmed tier 1 for v0 — bless the reversal?
4. Changed-history surfacing format ("turnus k: effusio mutata") —
   quiet notice, loud warning, or suppressed?
5. Declaration-turn echo: `x : s32`, full declarator echo, or
   silence?
6. Session startup world: -plagulae list (interpretare precedent) vs
   whole-monorepo prelower vs lazy on #include?
7. Names to seal: the engine (sessio?), the REPL binary
   (colloquium?), the overlay (umbra?), the trace (acta/vestigium?),
   __turnus_N wrapper prefix.
