# silva_conflatio.worklog.md

## 2026-10-07 — natus: extracted from briar_amalgama

Why: knotapel demos are going to import house libraries (laqueus and its
closure), and Fran wants each finished demo frozen as ONE compilable file
(`demo-snapshot.c`) so its findings stay reproducible while the libraries
keep evolving. The house had two amalgamation mechanisms:

- `silva_amalgama` (+ `tools/amalgama_ligare.sh`, the "generic linker"):
  duplicate statics across files are REFUSED (S41), never renamed. On the
  laqueus closure it refuses exactly the 10 collisions (`_apex_notare`
  magnus/matrix/polynomium, `_summa`, `_servare`, `_officinae_*`,
  `_est_spatium`, `_sine_spatiis`).
- `briar -amalgama` (briar_amalgama.c): already the single-file escape
  hatch, with every static renamed per file (`#define s s_<stirps>` /
  `#undef`, lists from `corpus.symbola.tsv`), headers in dependency
  post-order (postulata_posix.h first), local includes blanked (line
  numbers kept), `#line 1 "via"` per file. Tied to thistle (fabrica
  fructus, silex corpus, library members).

Decision (Fran, 10-07): extract briar's GENERIC part into one shared
mechanism in silva/instrumenta, rather than adding a second renaming
implementation to silva_amalgama ("single cases that should be classes").

What moved (pure: plagulae + statics table in, text out):
inclusio_localis, caput_est / fons_est (client roots now a context field,
not SILEX_RADICES_CLIENTIUM), statics-table parsing (from TEXT, not a
SilexFons), stirps, header ordering, source ordering, plain emission
(#line + blanked includes), renamed emission (one function for both lib
sources — suffix = stem, macros #undef only — and briar members — suffix =
member title, genus empty so everything is #define'd).

What stayed in briar: members and generated region files, the banner
(briar's flags), vitrea/vendor/.m refusals, the overwrite guard.
`briar_amalgama_inclusio_localis` is a one-line wrapper (API kept).
briar_amalgama.c 1013 → 542 lines.

Gate: briar's own amalgam golden (`fixa/amalgama/gamma.c`, byte for byte)
plus its structural real-corpus and member checks. Proven live: a
one-character plant in conflatio's comment text turns the golden red, so
briar really runs through the new path. No briar behaviour change → no
MUTATIONES line.

Unit test `silva/probationes/probatio_silva_conflatio.c` (32 checks): local
include syntax (spaces, system includes, `.c`, comments, too-short names,
`#define include`), vendored header/source tests incl. a client root,
statics table filtering (non-sources and other dirs dropped, malformed
lines skipped), header order (postulata first, post-order, client-root
resolution: `"m.h"` → `materia/fontes/m.h`), source order (twins in header
order, then the rest in closure order, the program root never), and exact
emitted text for all three emission shapes.

Plants (all compile): macros not skipped; postulata not first; twins not
placed after headers; local includes not blanked; client roots not
searched; table without the source filter; pre-order instead of post-order
— all red. My FIRST client-root plant (`r < ZEPHYRUM - I`) "survived"
because it read `radices[-1]` (undefined memory) instead of switching the
search off — a broken plant, not a test gap; the correct plant
(`FALSUM && ...`) is red. Lesson: a plant that changes a loop bound can
turn into UB rather than the fault you meant.

Build lists: silva's suite compiles every instrumenta/*.c (nothing to
add); briar names it in compile_probationes.sh (compile step + header
watch) and tools/briar_struere.sh (fingerprint), next to silva_lexicon.c
— briar's link globs briar/build/*.o.

Next consumer: principalia/conflator.c + knotapel/archive.sh (step 2).
