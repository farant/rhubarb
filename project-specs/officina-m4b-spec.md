# OFFICINA M4b — SPEC: SESSIO + COLLOQUIUM (the REPL + #! runner)

2026-07-11. Sealed by the proven arc: exploratores
(officina-m4b-fundamenta.md, 17 facts) → simulatio-7
(officina-simulatio-7.md, C1-C12, zero design reversals) →
interrogatio (officina-m4b-interrogatio.md, 15 DECISUS). Grounding
preceded this spec (the fundamenta explorers + sim probes ARE the
codebase-exploration pass); line cites live in those documents.

Bar (spec-v2 §XI, made concrete by DECISUS Q12): §V below.

## I. The design in one paragraph

The session is a C document (one growing TU, the liberal
top-level-statement dialect the grammar already accepts) plus an
execution world that is a CACHE of replay(document). Determinism is
the load-bearing wall: it licenses rebuild-as-recovery,
changed-history comparison, and the future fast path. A turn is
read (brace-balance continuation) → parsed (with `;` repair-echo) →
wrapped (statement turns → `vacuum turnus_N(vacuum){…}`) → judged
(examen; ACCEPTANCE = whole-document verdict, DISPLAY = new-range +
socius links) → and only a GREEN turn enters the document; then the
world is torn down, rebuilt, and replayed with per-turn acta
(captured effects) compared for changed-history notices. Every
effect crosses the builtin bridge, so session mode refuses the
nondeterministic families by name (writes, tempus/fortuna, stdin) —
the interceptor seam ships now, the umbra overlay arrives on pull.
sessio is the UI-free engine; colloquium is the line surface AND
the #! batch runner (refusals off, judgment on). The document saves
as itself (`:serva`) or exports as strict C89 with a synthesized
principale (`-strictum`).

## II. DECISUS (binding)

From the interview (all 15, see interrogatio for reasoning):
1. Repair + echo (any single-elementum `;` repair, echoed).
2. Declaration echo: terse `nomen : typus` from semantica.
3. Printer v0: scalars `value : type` host-side + char* preview
   (address + bounded NUL string from VM memory); aggregates
   refused by name; chorda = named fast-follow.
4. Changed history: quiet notice `turnus k: effusio mutata`;
   `:effusio k` shows old/new.
5. ALWAYS-REBUILD v0 (one path; fast path = first named
   optimization, from measurement).
6. Startup: `colloquium -plagulae a,b,c`; bare = builtins-only
   world. Whole-monorepo prelower = named fast-follow.
7. Session-mode refusals: file writes + tempus/fortuna + stdin
   reads, by name, at the builtin bridge.
8. Umbra (write overlay + :committe) OUT of v0 — first named
   post-v0 pull, consumer of the acta seam (sim C11, blessed).
9. `:serva` = liberal document (round-trips); `:serva -strictum` =
   wrapped + synthesized principale (clang-compilable export).
10. #! rule: principale if present, else top-level statement
    schedule in order. `:aperi` = load document, replay, continue.
11. History = the document (`:monstra`); no recall, no dotfiles.
12. Bar = transcript golden + #! script + persistence test (§V).
13. Names: engine **sessio**, binary **colloquium**, trace **acta**,
    wrapper prefix **turnus_N** (+ collision shift), overlay
    **umbra** (reserved, unbuilt).
14. #! owner: colloquium batch mode; script mode = refusals OFF,
    judgment ON.
15. Books officina-side; silva pointer only if silva fontes touched
    (expected: not).

Pre-interview seals (design conversation + sim): replay-document
model; green invariant (acceptance = whole-document verdict, C2);
judge-the-WRAPPED-text (C3 — statement-context legality free);
judge-before-teardown (C8); always-reparse (no systema chaining /
oracle carry-forward / tag seam in v0); engine/surface split with
four named consumers (colloquium, #!, workbench, vindex-eval).

Spec-time decisions (flagged in interrogatio, Fran may veto at
review): #! argv v0 = DROP (argc=0/argv=NULL; argv-wrapper
synthesis parked); stderr captured via ansae[2], shown after
stdout.

## III. Architecture

**sessio** (officina/instrumenta/sessio.{h,c} — vindex_onerator
precedent; consumes silva + officina THROUGH AMALGAMS ONLY):

- **Documentum**: display text (as typed; the artifact) + turn
  records {intervallum linearum, genus turni (declaratio/sententia/
  definitio/expressio), acta capta}. Perennial piscina.
- **Textus demissus**: derived by the wrap transform (sententia
  turns → `vacuum turnus_N(vacuum){…}`; declarationes/definitiones
  verbatim) + per-turn LINE-OFFSET TABLE mapping judged positions →
  display positions (also error-node spans — syntax errors carry no
  positions through examen; engine reads tree spans and maps).
- **Turn pipeline** (the sim's final form): lege (continuation by
  brace/paren balance + unterminated-comment lexeme) → parsa/repara
  (C1: parse-as-is; if new-range syntax errors and `;`-retry
  cleans, accept + echo) → involve+iudica (examen recipe on wrapped
  text; bis-analysis per parse) → verdictum documenti totius (any
  non-provisional violatio anywhere REJECTS; display = new-range +
  out-of-range summarized "turnus k sub hac definitione aegrotat"
  w/ socius) → demolire+aedifica+replica (below) → cape/imprime →
  appende → prompt.
- **Generations**: piscina_bibliothecae (library moduli lowered
  once at startup from -plagulae; document text; acta; session
  bookkeeping) + piscina_generationis (document tree, semantica,
  document modulus, conexio, machinula, planae) destroyed EVERY
  turn (always-rebuild). _generationem_demoliri order RIGID: ansae
  sweep (fclose host handles >2) → regio_destruere (fixed base must
  be free before next generare) → piscina destroy.
- **Replay**: run the turn schedule (turnus_N wrappers + principale
  never auto-run in session mode) via machinula_currere in document
  order; capture per turn (acta: stdout bytes via ansae[1], stderr
  via ansae[2]); memcmp vs stored acta → quiet changed-history
  notices. Replay HALT at old turn k → reject the new turn, rebuild
  the OLD document (correct by determinism; rare double rebuild).
- **Acta v0** = per-turn captured streams + the refusal events.
  Design the record layout as if it serializes (acta-visio rule);
  serialization = pull.
- **Printer**: host-side; MachinulaExitus.codex + semantica static
  type → `valor : typus`; char* preview reads VM memory (region
  host-addressable, bounded, non-printable escaped); aggregates
  refused by name (interpretare precedent).

**colloquium** (instrumenta/principalia/colloquium.c):
- Interactive: fgets loop (cooked mode; works piped), prompt `> ` /
  continuation `.. `; `:` commands v0: monstra, serva [-strictum],
  aperi, effusio k, exi.
- Batch (#!): argv[1] = file → blank first line IF `#!` (spaces,
  keep `\n` — positions stay true), parse whole file as document,
  judge (REICE → diagnostics to stderr, exit 1), run per DECISUS
  10. Refusals OFF, judgment ON. argv dropped v0.

**Machinula touches** (small, named):
- printf → m->ansae[1] (the one-line semantic correction,
  machinula.c:888); V4 sweep for other hardcoded stdout (puts,
  putchar, perror...).
- Session-mode flags on Machinula (or creare options): refusal
  families → clean halt "aedilis <nomen> in sessione recusatus —
  dilatio nominata".
- V3: F64 return through MachinulaExitus.codex — verify; if bits
  don't round-trip, add fructus channel or medulla-side print for
  floats only.

## IV. Chunks

**Chunk A — sessio core (no world).** Documentum + turn intake
(repair C1) + wrap transform + offset mapping (C3) + judge
integration (acceptance/display C2, socius links). Resolve V1
(break/continue-outside-loop coverage in semantica — wrapped
judging relies on existing statement checks). Vectis A: probatio
suite — classification, repair-echo, position round-trip (judged →
display), whole-document verdict scenarios incl. the
redefinition-rejects-at-old-line case; no machinula needed.

**Chunk B — world lifecycle + replay + capture.** Generations +
demoliri order (C6/C8) + rebuild + replay schedule + the
printf→ansae[1] correction + per-turn capture + refusal flags +
acta records + changed-history compare. Resolve V2 (arena
co-allocation), V4 (stdout sweep). Vectis B: probatio driving
multi-turn scenarios через engine API (build/replay/reject/rebuild-
old); THE PERSISTENCE REGRESSION TEST (write-N/read-N+1 global +
malloc'd block — mundus's gap) lands here.

**Chunk C — printer + interactive surface.** Host-side printer (V3
float verify) + decl echo + colloquium loop (continuation, `:`
commands, diagnostics display through the offset map). Vectis C:
golden-transcript suite v1 (scripted input through the ENGINE API,
full output byte-compared — includes refusal messages, repair
echo, changed-history notice).

**Chunk D — #! batch path + seal.** Shebang blanking + C12 rule +
refusals-off script mode + :aperi/:serva/-strictum emitters. Vectis
D = the M4b BAR (§V). RELATIO MILLIARII with full phase-log audit
(house rule) + instrument debrief.

## V. The M4b bar (DECISUS Q12)

1. **Golden transcripts**: probatio_officina_sessio drives scripted
   sessions through the engine API, byte-comparing full output —
   scenarios MUST include: decl/statement/expression turns; repair
   echo; a REICE'd turn (absent from document); a redefinition that
   rejects via an OLD turn's line; a redefinition that lands with a
   changed-history notice; each refusal family; :serva/:aperi
   round-trip; -strictum export compiles under the house clang
   flags.
2. **A #! script driving ≥2 monorepo libraries runs green** via
   colloquium batch (with real writes — refusals off).
3. **The globals persistence regression test ships** (write-N/
   read-N+1, global + heap).
4. All existing suites stay green; amalgam gates hold.

## VI. Verify-list (from sim-7, resolved in named chunks)

V1 (chunk A): statement-context checks present in semantica for
break/continue? V2 (B): demissio/conexio arena co-allocation
supports the generational split. V3 (C): F64 through
MachinulaExitus.codex. V4 (B): hardcoded-stdout sweep. V5 (only if
ever needed): per-machinula builtin-table mutability — mode-flag
branch chosen instead; V5 likely never fires.

## VII. Parks (named, with pulls)

- **umbra + :committe** (write-overlay + seal ceremony) — pull:
  first real want of file-writing turns; consumer of the acta seam;
  the stdio-over-buffers cost is the measured price (sim C11).
- **Fast path** (statement turns against live world) — pull: felt
  latency; equivalence testable via the acta compare.
- **Snapshot/restore globalia** (halted fast-path recovery; the
  acta snapshot primitive) — rides the fast path.
- **chorda printing** (aggregate return / %reditus path) — pull:
  first session that wants it (soon; named fast-follow).
- **argv-array wrapper for #!** — pull: first script needing args.
- **Record-on-first-contact** (stdin/time/reads → deterministic
  replay) — pull: refusals felt as friction; acta-visio machinery.
- **Whole-monorepo startup + moduli cache reuse** — pull: -plagulae
  friction.
- **Workbench** (tessera: document+console+stepper panes over one
  world) and **vindex expression evaluation** (needs VARIABILIA for
  frame-locals) — the named engine consumers 3 and 4.
- **Fork-validation** (test-build new world in a child) — pull:
  double-rebuild cost felt in practice.
- **Line editor + history recall** — rides the workbench.

## VIII. Books + discipline

Books officina-side (officina/phase-log.md: INTENTIO per chunk,
RELATIO per chunk, RELATIO MILLIARII + audit at seal). Instrument
debrief per RELATIO (house rule). Every new public officina.h row =
hospes call, same change. Latin everywhere in code; turnus_N
collision check before wrap generation. Fran's commits seal chunks.
