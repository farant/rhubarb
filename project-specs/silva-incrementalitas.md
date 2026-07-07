# Silva — incrementalitas: the foreclosure check

*2026-07-06. Recensio-positionis item #7 ("no-pull tier"): verify IN WRITING
that nothing in the current data model forecloses incremental reparse,
before more consumers calcify position semantics. This is analysis, not a
design for building incrementality — nobody has asked for it. The
predecessors died of decisions that looked reasonable until a feature needed
the door they had quietly closed; this page keeps the door on record.*

## I. The question

Silva's per-segment architecture is accidentally incrementality-shaped: the
driver slices the stream into independent segments (statements /
brace-terminated definitions), each parsed by its own GLR run with its own
recovery. Tree-sitter's incremental trick is exactly "an edit invalidates
only the nodes it touches" — for silva the natural unit would be the
segment. The feared foreclosure: every token carries an ABSOLUTE
`byte_offset` (and `linea`), an edit shifts every downstream offset, and if
anything load-bearing assumes offsets never move, incremental reparse is
architecturally dead.

## II. Census of position consumers (verified against source, 2026-07-06)

Everything that reads `byte_offset` / `linea`, and whether it compares
positions ACROSS anything that an edit would desynchronize:

| consumer | use | cross-parse? |
|---|---|---|
| silva_scribere.c (11 sites) | reinserenda merge order; emitted-range dedup (`emissum_usque`); extent containment | no — all within one parse snapshot |
| silva_parsare.c weave (8) | conditional-region extents + containment | no |
| silva_expandere.c (3) | function-like adjacency (`paren == titulus+len`); ramus corpus extents | no; adjacency is same-fons relative |
| silva_c89_oraculum.c (8) | positional typedef visibility (`situs`) | no — oracle filled per parse (but see §V) |
| vistas / saltuarius | `linea` for TOC jumps; ramus byte extents (public API) | rows rebuilt per Tab; LRU invalidates per file change |
| journal (`positus`) | stream-INDEX (token count), not bytes | per parse |

**Finding: every position comparison is intra-parse.** Nothing persists
offsets across parses except hosts that copy `linea` into their own UI rows
— and saltuarius rebuilds those on open/Tab. There is no "offset database"
anywhere.

## III. What segment-level incrementality would actually require

Sketch (for the record, not for building): edit arrives with (fons, range,
delta) →

1. **Re-lex the edited fons** (lexing is the cheap stage: ~0.34 ms/KB), or
   splice-lex the edited window (harder, not required at C-file scale).
2. **Identify dirty segments**: the segment(s) overlapping the edit, PLUS
   the adjacent-previous segment (trivia attaches TRAILING, so a boundary
   edit can move bytes between two segments' ownership).
3. **Validity predicate for downstream segments** — the preprocessor is the
   real dependency, not the offsets, and silva already has the exact
   machinery to express it: the JOURNAL records macro state per stream
   position (point-in-time expansion), and the ORACLE records typedefs per
   position. A downstream segment's old tree remains valid iff (a) the
   macro table at its start is unchanged (journal fingerprint) and (b) the
   typedef set at its start is unchanged (oracle prefix). Edits inside a
   function body typically change neither → everything downstream survives.
4. **Offset rebase for surviving segments**: add delta to byte_offset /
   recompute linea for every token of the edited fons after the edit point.
   Tokens live in stable Xar segments (pointers stable); a rebase walk is
   O(tokens), mechanical, ~50 lines. NOTHING consumes offsets across the
   rebase boundary (§II), so no other structure needs touching.
5. **Re-slice + re-parse dirty segments only**; splice new elementa into
   the radix lista (listas are copy-on-divergence views — splicing is a
   supported shape).

## IV. The verdict, ranked by real constraint

**Offsets are NOT the foreclosure.** They are a mechanical rebase (step 4).
The actual constraints, deepest first:

1. **Arena lifetime granularity (the deep one).** Today ONE piscina per
   parse holds tokens + trees + everything; arenas never free. Keeping old
   segments' trees across a reparse means the old arena must outlive the
   parse that replaced parts of it — repeated edits would chain arenas
   (unbounded growth) unless allocation becomes per-segment (a piscina per
   segment, freed when its segment dies) or generational. This is a MEMORY
   MODEL decision, invisible to the API, and the one place where a future
   refactor should be consulted BEFORE consumers multiply: nothing today
   forecloses per-segment arenas (segments are already parsed
   independently), but nothing prepares for them either.
2. **Whole-file artifacts.** strata (per-generation token streams), the
   region tree, directivae, inclusiones are whole-parse objects. Segment
   reparse must either regenerate them wholesale (cheap — they are
   expansion-stage products and expansion is fast) or accept staleness
   windows. Not foreclosure; bookkeeping.
3. **Token pointer identity.** Structural equality and provenance use
   pointer identity WITHIN a parse; a re-lexed region mints new tokens, so
   spliced trees mix token generations. Fine for emission/queries (all
   reads are value/structure), but any future feature assuming "one parse =
   one token universe" would break — worth remembering, not worth acting.

**Nothing currently forecloses segment-level incrementality.** The path
back exists. The invariants to PRESERVE so it stays open:

- Position comparisons stay intra-parse (the §II property — new code that
  persists offsets across parses must carry a version/parse token).
- Segments stay independent (per-segment GLR state reset — already the
  boundary-finder's discipline).
- The journal/oracle stay position-keyed (they ARE the invalidation
  predicate).
- Any arena refactor consults the per-segment-lifetime question first.

Also for honesty: at current scale incrementality may never be pulled —
whole-file c89 parse runs ~1-4 ms/KB; a 50 KB file is ~100 ms, fine on
save, marginal per-keystroke. The pull, if it comes, comes from an editor
(solarium) typing into large files.

## V. THE FINDING: the positional oracle is fons-blind (real bug, found by
## this census)

The §II verification pass caught a position comparison that IS broken —
not across parses, but across FONTES within one parse:

`silva_c89_oraculum.c` visibility check: `situs > situs_ambigui` compares
the typedef's declaring-token `byte_offset` against the ambiguous use's
`byte_offset`. **Offsets are per-fons; these are different rulers.** A
typedef INCLUDED from a header carries its header-relative offset; if that
offset is larger than the use's main-file offset, the typedef is judged
"declared after use" → wrongly invisible → the fork is RETAINED instead of
resolved, with the EXPRESSION reading canonical (policy pass) — no error,
no badge, no TOC row. Invisible to the zero-error wall by construction.

**Empirically confirmed** (scratchpad probe, 2026-07-06): identical
`#include "pad.h"` + `Meus * m;`; typedef at pad.h offset ~0 → resolves;
typedef after a 3 KB comment in pad.h → wrongly retained.

**Census** (percursus gained the ambigui counter — now a permanent product
number): monorepo baseline 17,607 retained ambigui in 497 plagulae; with
the positional check experimentally disabled: 15,225 in 407. **Delta =
2,382 wrongly-retained readings in ~90 files** (upper bound for the
fons-blind class; same-fons typedef-after-use is test-shape-only in
practice). The remaining 15,225 are honest retention: genuine C ambiguity
+ typedefs silva cannot know (system headers).

Also noted en route: LEXICON-registered files (latinam_addere path) never
register their typedefs at all — lexica feed the macro table only; their
non-directive content (latina.h's own `nomen ... i8;` lines!) is not
parsed into the radix. Under include-driven contexts (percursus -capita,
praebenda) latina.h IS parsed when actually included, so the sweep is
unaffected — but saltuarius's latinam_addere-only nexus never learns
i8/s32/etc. as TYPEDEFS (they resolve as unknown-name retention or X10
kills). Same fix family.

**Fix options** (decision owed, not taken here):
- (a) **Cross-fons ⇒ visible** (small): registration stores situs only for
  fons-princeps tokens; foreign-fons typedefs situs 0. Strictly better
  than today (fixes the 2,382); mis-handles include-after-use, which real
  code never does. Candidate for the same ADDITIONES-class package as the
  table-diagnostics item.
- (b) **Stream-order situs** (principled): registration walks the radix in
  stream order — a monotone counter IS declaration order as the compiler
  sees it; the resolutor needs the ambiguum's stream ordinal (available if
  the pre-commit walk numbers elementa). Correct for every shape incl.
  include-after-use. Slightly more design; natural at the typus/index
  milestone where scope-aware symbol tables arrive anyway.
