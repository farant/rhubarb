# aedilis — spec v1 (2026-07-20)

*Status: post-interview, pre-exploration. v2 adds codebase-specific
planning after explorer reports. Interview raw:
project-specs/aedilis-interview.md. Park: 01KXJ2HV (tractum).*

## I. Purpose

Aedilis (the magistrate of public works) is the build layer's first
rung: point it at a C file and it DERIVES the file's true dependency
closure — which objects, which flags, which frameworks — and emits
(a) a manifest recording that truth with per-entry provenance and
(b) a targeted bash script that builds exactly that, saved or
executed. What cannot be derived statically arrives through three
channels: aedilis.stml (repo-wide facts), `/* aedilis: ... */`
annotations (site-local facts), and arguments (invocation facts).

It replaces no existing build path in this arc. It exists so that
dependency truth stops being folklore quadruplicated in hand lists
(gesta RADIX_FONTES, amalgamare.sh ×3, colloquium -plagulae,
compile_tests' link-everything).

## II. Decisions of record (interview, 15)

1. Arc = closure engine + script emitter. Amalgam emitter (01KY0XRBEX)
   and further emitters ride the same engine later.
2. **Silva-primary derivation** from day one; see §III for the
   directive-extraction/own-resolution split.
3. **C89 tool**: tools/aedilis.c → bin/aedilis (built by existing
   compile_tools.sh — no self-hosting paradox).
4. **Manifest → script**: manifest is the artifact of record
   (STML, per-entry provenance: derivatum | fabrica | aedilis.stml |
   annotatio | oraculum); script generated from it.
5. Graph access: **link the silva amalgam**, parse in-process,
   **persistent incremental include-graph cache** (per-file mtime
   invalidation; first run seconds, steady-state ms).
6. **clang -MM = gate + permanent minority lane**: differentia-
   clausurae ships in-arc; corpus-wide CONSENSUS required before
   trust; -MM permanently covers .m files and loud fallback.
7. Manifests in **STML** (lib/stml.c emits + parses natively).
8. Config = **aedilis.stml** (new, aedilis-owned, committed, same
   grammar as manifests). fabrica.tsv remains the excubitor's manual
   ledger (absorption = later milestone).
9. First-arc consumers: **standalone aedilis <file>** + **gesta
   RADIX_FONTES regenerated** + **colloquium/interpretare -plagulae
   derived**.
10. Proof bar: **whole-corpus gate** — targeted scripts for every
    probatio, all run green, plus differentia consensus.
11. Invocation: artifacts to build/aedilis/<basis>/ (gitignored);
    flags -currere, -scribere <via>, -machina/-posix.
12. compile_tests.sh untouched this arc.
13. Annotations: `/* aedilis: obiectum <via> */`,
    `/* aedilis: vexillum <flag> */` — read from aedilis's own silva
    parse (no second scanner; generare: stays separate — it executes,
    aedilis declares).
14. Script default reuses build/ .o cache; **-solitarius** emits the
    hermetic compile-everything variant. Both from one manifest.
15. **Refuse loud** on underivable closure (no manifest, no script,
    named cause). Precision or silence.

## III. The load-bearing refinement (post-interview)

Silva-primary does NOT mean inheriting silva's include RESOLVER.
Aedilis uses silva to parse each file and extract the include
DIRECTIVES (and aedilis: comments) as data, then does its OWN
resolution: includer-relative join via via_iungere +
**via_normalizare** (".." handled — the library exists), then the
-I list from aedilis.stml. Consequences:
- Silva's known resolver gaps (".." bug 01KY0T6T64, -I/praebita
  parity 01KXS2ETAE) are NOT prerequisites — decoupled entirely.
  (They remain worth fixing for examen's own consumers.)
- The differentia vs -MM validates aedilis's WHOLE chain
  (extraction + resolution), which is the thing that matters.
- VERIFY in v2: silva exposes raw #include directives pre-evaluation
  (superset semantics like -MM — both #ifdef arms). House
  file-per-platform convention makes this nearly moot, but the
  guarantee must be stated, not assumed.

## IV. The algorithm

1. Seed: target .c (plus any aedilis: annotations in it).
2. Parse (silva, cache-aware) → include directives → resolve
   (§III) → house headers into the pending set; system headers
   recorded, dropped from closure; vendor/ paths → exclusa-style
   listing.
3. Header → object by convention: include/X.h → lib/X.c, with
   platform variants probed (X.c, X_posix.c, X_<machina>.m/.c) and
   selected by -machina/-posix; irregulars from aedilis.stml (and,
   read-only, fabrica.tsv's obiectum species where applicable).
4. Discovered .c/.m files: parse in turn (silva; .m via the -MM
   minority lane) → more headers → FIXPOINT until stable.
5. Attach flags: base flags from config; per-object link needs
   (frameworks) from aedilis.stml's object→nexus mapping; annotation
   vexilla; vendor objects with vendor flags.
6. Emit manifest (STML: entries with provenance, ordered), then
   script(s) from manifest: cache-reusing (default) — per-object
   mtime compile-if-stale then link; -solitarius — hermetic full
   compile. Bash 3.2-safe, exit-code gated, ANSI-free output.

## V. differentia-clausurae

`bin/aedilis -differentia <file>` (or a wrapper script): compute the
closure via silva-lane and via clang -MM; bucket per entry —
CONSENSUS / NOS SOLI / ORACULUM SOLUM — repo-wide sweep mode for the
corpus gate. Overcoverage (NOS SOLI) and undercoverage (ORACULUM
SOLUM) both visible; the seal requires clean consensus modulo
understood classes (system headers, .m attribution).

## VI. Consumers (this arc)

- Standalone: the §IV flow, human-facing.
- gesta: a mode/flag that emits the RADIX_FONTES list for
  gesta/compile_probationes.sh consumption (or regenerates the
  script's list section) — proving subtree + sqlite vendor coupling.
- Bench: colloquium.sh/interpretare.sh gain optional auto-derivation
  of -plagulae via bin/aedilis when the caller doesn't pass one.

## VII. Phases (sketch — v2 firms)

- **0 — spike**: prove the core mechanism: a tools/ C program links
  the silva amalgam, parses one file, prints its include directives
  + comments; stml write/read round-trip; via_normalizare behavior
  pinned with specimens.
- **A — engine**: resolution + convention mapping + channels +
  fixpoint + graph cache + manifest emission; probatio with fixture
  specimens (platform variant, annotation, vendor, refusal case).
- **B — emitters + gate**: script generation (both postures);
  differentia-clausurae; the whole-corpus run.
- **C — consumers + seal**: gesta regeneration, -plagulae
  integration, worklog, board.

## VIII. Open questions for v2 exploration

1. Silva amalgam API surface: parse entry point(s), how include
   directives and comments are reachable from the parse result,
   piscina conventions, pre-evaluation directive guarantee (§III),
   per-file parse cost (exemplars: legatus, amalgamator.c,
   examen).
2. stml lib: WRITE api shape (builder? serializer?), read API.
3. House argv pattern: lib/argumenta.c usage exemplars.
4. Graph-cache file conventions: nexus.tsv format/versioning/mtime
   idioms (vigilia's builder as exemplar); where the cache lives
   (build/aedilis/?).
5. colloquium.sh/interpretare.sh -plagulae plumbing points; gesta
   compile_probationes.sh structure for list regeneration.
6. fabrica.tsv consumption: which species aedilis reads (obiectum,
   praefixum) and how excubitor's reader parses them (reuse?).
7. How the -MM minority lane is invoked from C (system() + temp
   file in build/aedilis/), and .m edge attribution in the cache.
8. aedilis.stml schema draft: -I list, object→nexus (framework)
   mapping, vendor flag sets, platform default.
