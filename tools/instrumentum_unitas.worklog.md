# instrumentum_unitas.worklog.md

## 2026-11-07 - natus: percursus built as one translation unit (budget item 10/11b)

Why: the parse-side census (silva/fontes/silva_parsare.worklog.md, item 9)
found xar accessors at 20% of self time - call overhead across .o files on
an O(1) path. ThinLTO recovered 337.5 -> 285.4 G; a unity build through the
knotapel snapshot tool (conflator) got 290.3 G in plain C89 (Fran: the more
robust route; first step of the "webpack for C" wish 01M510B2).

Shape: sibling of tools/instrumentum_nectere.sh with the same contract (the
nexus-instrumentorum gate checks it for percursus: fresh call does not
relink, touched source relinks, 8 concurrent fresh/stale calls all succeed
with no temporaries left). Closure from `bin/aedilis <root> --partes`,
cached in `<bin>.unitas/partes.tsv`, so the freshness check is one `find`
over the closure + vexilla + caller + helper + conflator sources. A stale
build takes a mkdir lock (dead owner broken via pid), runs aedilis, builds
the conflator, refreshes silva's nexus, writes the statics table
(symbola_tabulam_scribere, factored out of corpus_infixum.sh - corpus.symbola.tsv
byte-identical after the refactor), conflates; compiles outside the lock
into `<bin>.nexus.<pid>`, then mv. The conflated file stays in the cache
(handy to read; #line points back to the real files).

percursus.sh: unity by default; SILVA_UNITAS=0 = the old object build into
a SEPARATE binary (build/percursus_obiecta) so neither mode can run the
other's binary. Cold unity build ~11 s single core; fresh call 0.02 s.
percursus links 33 sources now (true closure) instead of every
silva/fontes/*.c.

Gotchas found on the way:
- clang -g compile+link in one step writes `<out>.dSYM` next to the
  temporary output; left behind 10 per concurrent run until moved with the
  binary (instrumentum_nectere already did this - copied).
- percursus' closure has NO duplicate statics (the silva amalgamator refuses
  them, S41), so building with an EMPTY statics table compiles clean and the
  report is identical. The gate counts renames (#undef >= 100) instead.
- silva.planta restores the planted file's mtime, so a binary built under a
  plant looks fresh afterwards: plant U4 first "survived" and left a
  plant-built binary installed. The gate now deletes the unity binary, its
  cache and the object binary and always builds from source (memory
  plant-restore-mtime already said this for fumus scripts).

Gate `unitas-percursus` (silva/unitas_probare.sh, ~30 s): both modes build;
the conflated file is real (#line for >= 30 files incl. xar.c and
silva_glr.c, renames applied, binary not older); touching a CLOSURE file
(silva_glr.c, mtime restored afterwards) relinks the unity binary;
`percursus -semantica lib` gives the same report and exit code both ways.
Plants 5/5 red: default back to objects; closure dropped from the freshness
check; silva/fontes missing from the roots; empty statics table; comparison
against the wrong output (by hand - planta's anchor lookup did not find
shell words in that line).

Measurement (whole repo, 2 interleaved runs, report identical): unity
290.9/290.6 G, objects 337.0/337.2 G (-13.7%).

Open: other silva tools (examen, formator, the LSP side) could opt in the
same way; each needs its own cache dir, and two tools' stale builds would
refresh silva's nexus concurrently (outside each other's lock).

## 2026-11-07 - rebuild bookkeeping trimmed (item 12): smaller win than estimated

Step timings of a stale rebuild (11.1 s): aedilis closure 0.40, conflator
freshness 0.15, nexus refresh 1.46 (with nothing changed), statics table
2.4 (awk + sort over all 520 k 'sedes' rows of the 200 MB nexus.tsv),
conflation 0.09, clang -O2 -g 6.6 (frontend 0.2 s; -O0 whole build 0.86 s;
the rest is the optimizer + codegen, instruction selection 14%,
InstCombine 8.5%, inliner 5.7%, GVN 4.8%...).

Two changes:
- statics table filtered to the closure (symbola_clausurae_scribere in
  corpus_infixum.sh): conflated percursus byte-identical to the full-table
  conflation; closure rows identical (6,427). But the floor is one pass of
  BSD grep over 200 MB (~1.3 s), so 2.4 -> ~1.6 s. `grep -F -f` with 72
  patterns under BSD grep took 1 min 44 s (zsh's `grep` is ugrep - my first
  timing, 0.1 s, came from there; scripts get /usr/bin/grep). awk alone
  over the file 4.3 s, grep -E alternation 3.0 s.
- nexus refresh skipped unless a closure file is strictly newer than
  build/nexus.tsv. Only helps rebuilds NOT caused by a closure edit (flags,
  helper, wrapper): an edited closure file genuinely needs the refresh.
  Caveat in the code: a file restored with an OLD mtime (cp -p, planta)
  after the nexus captured the plant is not seen.

Measured end to end: wrapper touched 11.2 -> 10.3 s; closure file edited
10.6 s. Tracing with a perl-timestamp PS4 adds ~1.4 s to the 72-path loop
(looked like a slow conflator build at first).

Real levers left: clang (chunked unity in parallel, or -O0 dev builds) and
the index format (nexus.sh writing its 'sedes' rows - 37 MB - or per-file
statics, so the table need not scan 200 MB).

## 2026-11-09 — experiment: internalize the unity (every function static)

Question: does "tree shaking" (internal linkage for every closure function
except main) pay in plain C89 the way LTO internalization did (290.3 ->
288.2 G)? Experiment only, nothing committed: scratch copy of
build/percursus.unitas/percursus.c with `static ` prefixed to the 623
header prototypes the nexus lists (functio, depth 0, not already static;
return type on the line above the name in house style, same line when the
name column is > 1). C89 linkage rule does the rest: a later plain
declaration/definition inherits the internal linkage of a prior `static`
one, so the .c definitions stay untouched. Needs -Wno-unused-function
(dead functions are now static). Compiled clean first try.

- compile -c: 6.65 -> 5.64 s (-15%, 3 runs each, very stable)
- __text: 521580 -> 394308 bytes (-24%); defined functions 968 -> 356
- runtime, percursus -semantica (whole repo, 2 interleaved runs):
  290.4/290.3 G -> 292.9/292.4 G (+0.8%), wall ~1% slower; lib corpus
  46.31 -> 46.43 G (+0.26%). Reports identical (1525 / 201 lines).

So plain clang -O2 does NOT reproduce LTO's gain: the "last call to a
static" inline bonus pulls single-caller functions into their callers
wholesale, and here that costs a little. The compile-time and size win
is real. Unexplained yet: which inlinings cost the 2.3 G.

## 2026-11-09 — experiment 2: static only for DEAD functions

Dead list from the linker itself, not a call-graph guess: link the unity
with -Wl,-dead_strip; externally visible functions present without it but
absent with it = unreachable from main (address-taken counts as reachable;
"dead" also covers functions already inlined at every site). 625 external
functions, only 103 survive dead_strip; 522 dead, every one has a header
prototype. `static` prefixed to those 522 only.

- compile -c: ~6.55 -> ~5.3 s (-19%; all-static was 5.7 s)
- __text 396824 (all-static 394308, base 521580); 382 functions
- runtime whole repo (5 interleaved rounds): base 290.57-290.85 G,
  dead-only 291.81-291.90 G (one outlier 291.11) = +0.4%; all-static +0.8%.
  Report identical. Wall time drifts seconds between rounds; useless here.

Why live code still changes (per-function size diff, dead-stripped base vs
dead-only): removing dead CALLERS turns multi-caller static helpers into
single-caller ones, and clang's last-call-to-static bonus then inlines cold
helpers into hot functions: _allocare_segmentum_xar into xar_addere
(312 -> 864 bytes), silva_annotationes_ex_commentis 848 -> 5612,
_fistula_interna (silva_parsare) 460 -> 6084. Same mechanism as
all-static, smaller dose. Apple clang does not expose the bonus
(-mllvm -inline-last-call-to-static-bonus unknown). Dropping dead bodies at
source level would do the same thing (the helpers lose the same callers).
PGO is the natural fix: profile counts mark those call sites cold.

Implementation sketch if adopted: always link the unity with -dead_strip;
next build's dead list = header-prototyped functions absent from the
previous binary (cache it in <bin>.unitas/). A stale list is always SAFE:
static never changes behavior in a single TU, only optimization.

## 2026-11-09 — experiment 3: profile-guided optimization (clang PGO)

llvm-profdata ships with Xcode (xcrun llvm-profdata) - no outside install.
Recipe: clang <flags> -fprofile-instr-generate unity.c -> run with
LLVM_PROFILE_FILE=<x>.profraw -> xcrun llvm-profdata merge -o x.profdata ->
clang <flags> -fprofile-instr-use=x.profdata. No new warnings under
-Werror (same source, so no stale-profile warnings).

Training: percursus -semantica lib (instrumented: ~10 s, 107 G).
Evaluation: whole repo (lib is ~46 of the ~290 G, so partly held out).

| variant                    | compile -c | __text | whole repo      |
| base unity                 | ~6.55 s    | 522 KB | 290.55-290.85 G |
| dead-only static           | ~5.3 s     | 397 KB | 291.76-291.85 G |
| base unity + PGO           | 15.4 s     | 587 KB | 256.90-257.73 G |
| dead-only static + PGO     | 9.3-11.9 s | 516 KB | 256.41-257.10 G |

PGO = -11.5% instructions, reports identical (1525 lines). Cumulative vs
separate objects (337.1 G): -24%. With PGO the dead-only variant is no
longer worse - slightly better, and compiles ~5 s faster than base+PGO.
xar_addere: 312 (base) / 864 (dead-only) / 612 (dead-only+PGO) bytes -
the profile undoes part of the cold-helper inlining. Wall times useless
this session (load average ~5; same binary 26-37 s), instructions are the
measure. Cost: PGO compile is 1.5-2.5x slower, plus an instrumented build
and a training run - an installed-build step, not an edit-loop step.
Open: profile staleness policy (clang drops the profile per function whose
CFG hash changed; when to re-train), and whether one profile can serve
several tools sharing silva code.

## 2026-11-09 — interiora built into the driver (item 13 step 2)

The driver now does the dead-only variant by itself:

- link always carries `-Wl,-dead_strip`;
- after the link, `mortua.txt` = candidates with no text symbol (T or t)
  in `nm -U` of the new binary. Counting `t` matters: a function made
  static last time and still alive shows up as a local symbol and must
  leave the list. Candidates = external functions with a prototype in a
  closure header (position from `selecta -sedes`, name from the nexus
  `sedes functio` row inside the declaration's line range);
- the NEXT rebuild turns the candidates named in mortua.txt into
  `interiora.tsv` and passes it to `conflator -i`, adding
  `-Wno-unused-function` only then.

So a cold build (no mortua.txt) is the plain unity file, byte-identical
to before, and the first rebuild after it is the dead-only one, which is
byte-identical to experiment 2's file (522 positions). The list is stable:
the dead-only binary yields the same 522. A stale list is safe in both
directions (static never changes behavior in one TU), so the list is NOT
a staleness input and nothing rebuilds because it changed.

Position cache: `<bin>.unitas/sedes/<header>.tsv`, first line
`# <md5 header> <md5 selecta.c>`. Content keys, not mtimes, on purpose:
silva.planta and `cp -p` restore old mtimes, and a cache written during
a plant would otherwise look fresh afterwards. `md5 -r` over 40 files is
~10 ms. Stale headers are re-scanned in parallel (one selecta each);
selecta is built on demand (INSTRUMENTUM_NECTERE_SOLUM=1, 0.06 s when
fresh) and only when some header is stale. A header selecta cannot parse
(exit 2) is skipped with a message and not cached: fewer interiora, still
correct.

The nexus `sedes` rows of the closure are now grepped once
(`sedes_clausurae_scribere` in corpus_infixum.sh) and read twice (statics
table, candidate join) instead of a second 1.3 s pass.

Measured: stale rebuild 10.3-10.6 -> 9.65 s (two runs, 9.70/9.64); the
cold build is 12.65 s once (selecta freshness check + 39 scans).
`__text` 396824 as in the experiment. Runtime cost stays the measured
+0.4% until the profile step.

`UNITAS_INTERIORA=0` skips applying the table (mortua.txt is still
refreshed) - for comparisons.

Gate (silva/unitas_probare.sh) step V: cold build is plain and writes
>= 100 dead names; the IV rebuild applies them (`^static ` lines in the
conflated file 0 -> 522); a position cache with a foreign key and a bogus
row is recomputed, not trusted; III now compares three binaries (plain
unity kept from the cold build, dead-only unity, objects). Plants, each
red then green: mortua.txt never applied; link without -dead_strip
(mortua too short - 2 checks red); cache key ignored.

Not covered: selecta exit-2 path (no header fails today), and
UNITAS_INTERIORA=0.
