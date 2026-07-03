# Silva — Simulatio ⑥: Layer-0 reconstruction (Phase 5 Chunk B)

*2026-07-02. Mid-phase simulation against SHIPPED code (the simulatio ⑤
technique, now standard practice per silva/CLAUDE.md CULTURA). Subject:
the expansion boundary + directive re-insertion — the genuinely new
design work of Phase 5, which no ancestor ever did (arbor2/v1 only
roundtripped by NOT expanding). Method: pretend-implement
`silva_scribere_fontem`'s reconstruction path and the `_lexema_scribere`
boundary arm, verifying every assumption against the real sources
(silva_expandere.c, silva_token.h, the corpus files). Every claim below
carries a line reference — the Xar-lore lesson applied.*

Bar being designed toward: **cursor.c byte-identical with latina.h
praebere'd** (M1 §6), plus latina.h reconstructing as its own fons,
plus the fuzz oracle extension (any bytes → parse → emit == input).

---

## The update stream

**T+5 — Reading the capture machinery before writing anything.**
`_directivam_capere` (silva_expandere.c:284) appends a token SLICE per
captured line into directivae_out. Slices come from `_lamina_capere` —
original stream tokens, trivia intact. `_lineam_finire` (:239) ends a
logical line at the next `initium_lineae` token or EOF — so
splice-continued directive lines are ONE slice, and the terminating
newline rides the last token's spatia_post INSIDE the slice. First
good news: emitting a captured slice through the Chunk A per-token
primitive reproduces the line's bytes exactly, indentation before `#`
included (it's the CANCELLUM token's spatia_ante).

**T+10 — Mapping every consumed-directive path in the walker.**
`_fluxum_processare` (:1069): #define/#undef captured AFTER successful
processing (:1103) — malformed ones FALL TO RELIQUA uncaptured (:1108),
i.e. their bytes travel the TREE path. #include captured BEFORE
processing (:1112) — always, even unknown/malformed (the "linea capta,
nihil insertum" comment at :863 confirms). Region directives captured
at :604, #endif at :778. Orphan #elif/#else/#endif, bare #, unknown
directives → reliqua (:1127-1128, byte-conservative). Every consumed
byte is captured; every non-consumed byte stays in the stream. No
byte has two owners. This is C3 — the attachment rules partition the
file exactly, for free.

**T+15 — The untaken arms. Third byte source found.**
`_regionem_processare` (:759): a non-taken arm's interior is captured
as `ramus->lexemata_cruda` — a raw slice, NOT in reliqua, NOT in
directivae. Nested regions inside untaken arms are depth-counted only
(:713-749), so the raw slice carries their bytes too — single
coverage. Reconstruction therefore has THREE sources: tree tokens,
directive-line slices, cruda slices. They interleave by source
position. (C2.)

**T+20 — First design sketch of `_fontem_reconstruere`.**
Collect "reinserenda" for the target fons: directivae entries whose
first token's fons_index == target, PLUS cruda slices from the region
tree (recursive walk of exp->regiones, filtering by fons). Each has a
byte offset (first token). Both collections are already in source
order per fons; a merge (or one qsort over the small combined list)
gives a single ordered queue. Then the tree walk emits units — before
each unit whose layer-0 offset is o, flush every reinserendum with
offset < o; at the end flush the rest, then EOF tail trivia. The
tree walk stays structural; POSITION is consulted only for the flush
cursor — exactly the INTENTIO's file-level allowance.

**T+25 — Includes: one directivae stream for ALL fontes.**
`_includendum_processare` recurses with the SAME reliqua+directivae
(:903) — latina.h's 388 #define lines land in the same directivae_out
as cursor.c's 3 #include lines. Without a fons filter, cursor.c's
reconstruction would embed latina's entire body. The filter (first
token fons_index) is mandatory, and sufficient: capture order follows
consumption order, so the per-fons subsequence is source-ordered.
(C1.) Included tokens in the TREE are foreign to the target — skipped;
the #include line flushes into exactly the gap they leave (the splice
sat where the line was).

**T+30 — The boundary arm. Multi-generation chains checked.**
`#define A B` / `#define B 3` / `A;` — the final token's one-hop
invocatio is the INTERMEDIATE gen-1 token, not the source. But
`silva_token_radix` (silva_token.h:311) follows invocatio/sinister/
primus to the layer-0 token — the source `A`, with its real trivia.
So the boundary arm is: origo != FONS → radix = silva_token_radix();
if radix is target-fons FONS, emit radix ONCE per run (dedup state =
last radix pointer, reset on any FONS token). Multi-token bodies
(`#define DUO 1 2`) produce runs sharing one radix even when the
tokens land in DIFFERENT tree slots — tree order is stream order
(slot-order validation, Chunk A), so the run stays consecutive.
Adjacent same-macro invocations (`A A`) have DISTINCT radix tokens —
dedup by pointer, not by name, handles it. (C4.)

**T+35 — What radix does NOT cover: pasta and stringificatio.**
PASTA origo has no invocatio (silva_token.h:214-219) — radix follows
`sinister`, which for a body-token operand is a DEF-SITE token: the
chain can land in the DEFINING file. Same class for CHORDA (primus)
and API (synthetic fons). The boundary arm must check the WHOLE chain:
walking radix, if any hop is PASTA/CHORDA or ends at API → fractura
clara for M1 (named deferral, unchanged from the INTENTIO). Detection
is cheap — silva_token_radix already walks; a variant that reports the
chain's genera costs a loop.

**T+40 — Function-like extents: found the recording site, checked
whether anything forces it.** `_argumenta_colligere` (:1498) consumes
name..`)` from the CURRENT stream — at layer 0 that stream holds FONS
tokens, and the collection loop knows the exact extent when parens
balance. Recording `{name-token → slice}` (only when name origo ==
FONS) is ~30 lines: an Xar on SilvaExpansio + one lookup in the
boundary arm (emit slice instead of single radix token). Then I
grepped the corpus: preprocessor_test.c DEFINES `ADD(a,b)` but never
calls it; no corpus file self-defines-and-uses a function-like macro.
Nothing forces extents. cursor.c + latina need object-like only.
(C5 — decision for Fran: build now while the site is open, or keep
the fail-loud arm.)

**T+45 — Tracing cursor.c end-to-end on paper.**
cursor.c: 3 #include lines (cursor.h, color.h, thema.h), then pure
latina-flavored C89. cursor.h is a GUARDED file (#ifndef CURSOR_H)
including latina.h + fenestra.h. Praebere closure for the M1 test:
cursor.h, color.h, latina.h (all in the corpus); thema.h/fenestra.h
stay unknown → learning mode → lines captured, nothing spliced —
reconstruction unaffected. With latina loaded, the body's si/redde/
per/structura EXPAND: the tree is full of EXPANSIO tokens whose
radices are cursor.c FONS tokens. Reconstruction of fons 0: tree walk
(skip foreign cursor.h/latina.h/color.h tokens; radix+dedup for
expansions; FONS tokens verbatim) + 3 #include lines flushed by
offset + EOF tail. Traced the first function by hand: byte-exact.
The guard path (`_plagulam_processare` :1179-1186, "tres directivae
captae") captures cursor.h's #ifndef line, its #define via the
walker, and the #endif line — cursor.h reconstructs too.

**T+50 — latina.h as its own fons: found a real hole.**
latina.h is nothing but directives + comments — after processing, its
tree contribution is ZERO tokens; everything lives in directive
slices. Fine. But its EOF token — carrying the file's TAIL bytes as
spatia_ante — is DROPPED at :1131 (servare_eof FALSUM for included
files). Reconstructing latina.h would lose everything after the last
directive's newline. The token still EXISTS: exp->includenda's
`incl->lexemata` retains the full lexed stream, EOF included. So
fontem, when target != the stream's own fons, fetches the included
file's EOF from the includenda table. No expander change needed —
just a lookup. (C6.)

**T+55 — Guard corner: custos predefined BEFORE first processing.**
`_plagulam_processare` :1172: if the guard macro is already defined
when a guarded file is FIRST processed (deliberate API pre-definition
— re-includes never reach this, they short-circuit at :887), the path
processes only [post-#endif..n) and captures NOTHING — the guard
lines AND the whole interior vanish from both streams. Reconstruction
hole for that fons in that configuration. Rare, deliberate, and
detectable. Candidates: capture-and-record in that path (needs a
cruda-style record for the skipped interior), or document + fail-loud
when fontem targets such a fons. (C7 — decision for Fran; my
recommendation is defer with a loud fracture, it needs API abuse to
trigger.)

**T+60 — Fuzz shapes, one by one against the walker.**
- est_imperfecta region (EOF before #endif): taken-arm tokens → tree;
  arm lines captured; missing #endif = missing nothing (no bytes
  existed). Covered.
- Stray mid-line `#`: not initium_lineae → reliqua → tree. Covered.
- Malformed #define: reliqua → tree (UNcaptured — the asymmetry with
  #include is real but each path is single-coverage; C8, test-only).
- `#include` with garbage operand: captured + nothing inserted →
  re-emitted verbatim. Covered.
- Orphan #endif / #else: reliqua → tree. Covered.
- `#if` with unevaluable expr: falsum arm → cruda slice. Covered.
- Directive containing STRING_IMPERFECTUM: slice verbatim. Covered.
The fuzz oracle (parse→emit==input over 400 specimens) should pass
by construction once reinserenda lands — every consumed byte has
exactly one re-emission source.

**T+70 — The scribere-side shape, final.**
`SilvaParsura` gains `directivae` (driver passes the out-param it
currently NILs at silva_parsare.c:323). `_lexema_scribere` grows the
boundary arm (radix walk + chain-genus check + dedup). fontem grows
`_reinserenda_colligere` (directivae filter + region walk + sort) and
the flush cursor. The Chunk A fail-loud test (`#define T int\nT x;`)
flips to a fidelity assertion. New fixtures: preprocessor_test.c
shape (object-like use + regions + #pragma passthrough + unknown
include), cursor.c closure bar, latina.h fons reconstruction, the
fuzz shapes above. Estimated new code: ~120 lines scribere, ~10
driver, ~30 expander IF extents approved, plus probatio.

**T+75 — Re-checked one nagging ordering assumption.** The flush
cursor compares TARGET-fons offsets only. Foreign tree tokens have no
target offset — they don't advance the cursor; the #include line
flushes when the next TARGET unit arrives or at end-of-walk. Two
consecutive #includes: both lines sit in the queue in offset order,
both flush together before the next target token. Correct placement,
no special case.

---

## Complexity register (four-part schema)

- **C1 — One directivae stream for all fontes.** Discovered-while:
  reading the include recursion (:903). Consists-in: captured lines
  from every processed file interleave in one Xar, consumption order.
  Consequences: unfiltered re-insertion embeds included files' bodies
  into the including file. Handled-by: filter by first-token
  fons_index; per-fons subsequence is source-ordered by construction.
- **C2 — Untaken arms are a third byte source.** Discovered-while:
  tracing #if reconstruction. Consists-in: non-taken interiors exist
  ONLY as ramus->lexemata_cruda on the region tree; nested regions
  ride inside the slice. Consequences: fontem must walk exp->regiones
  collecting cruda per fons. Handled-by: reinserenda = directive
  slices + cruda slices, one offset-ordered queue.
- **C3 — Attachment rules partition bytes exactly.** Discovered-while:
  hunting double-coverage at directive boundaries. Consists-in: a
  directive line's newline lives in its own last token's spatia_post;
  neighbors' trivia never cross the boundary; BOF/EOF trivia attach
  inward. Consequences: tree + directivae + cruda is an exact
  partition — no gaps, no overlaps. Handled-by: nothing to build;
  asserted by the fidelity oracle.
- **C4 — Boundary arm = radix + consecutive-run dedup.**
  Discovered-while: tracing multi-generation expansion. Consists-in:
  one-hop invocatio can point at intermediate tokens;
  silva_token_radix reaches layer 0; runs share a radix pointer,
  adjacent invocations don't. Consequences: dedup keyed on radix
  POINTER (reset on FONS token) emits each invocation exactly once.
  Handled-by: _lexema_scribere boundary arm; tree order = stream
  order (Chunk A slot-order validation) keeps runs consecutive.
- **C5 — Function-like extents cheap; nothing forces them.**
  Discovered-while: checking _argumenta_colligere (:1498) + corpus
  grep (ADD defined, never used). Consists-in: layer-0 invocation
  bytes (args' trivia included) exist only pre-expansion; the
  collection loop knows the extent; recording {name-token → slice}
  when name is FONS costs ~30 lines. Consequences: without it,
  function-like expansion stays a loud fracture; with it, fidelity
  extends. Handled-by: FRAN DECIDES — build now vs. keep fail-loud
  until forced.
- **C6 — Included files' EOF dropped; tail bytes retrievable.**
  Discovered-while: latina.h self-reconstruction trace. Consists-in:
  :1131 drops included EOFs; their spatia_ante carry file tails; the
  tokens survive in exp->includenda. Consequences: reconstructing an
  included fons must fetch its EOF from the includenda table.
  Handled-by: lookup in fontem when target fons != stream fons.
- **C7 — Guard file with predefined custos skips uncaptured.**
  Discovered-while: reading :1172. Consists-in: first-processing of a
  guarded file whose custos is already defined captures nothing and
  skips the interior; only API pre-definition triggers it.
  Consequences: reconstruction hole for that fons in that
  configuration. Handled-by: FRAN DECIDES — recommended: defer, with
  fontem fracturing LOUDLY if it targets such a fons (detectable:
  est_custos fons with zero captured lines).
- **C8 — Malformed-directive asymmetry.** Discovered-while: walker
  dispatch read. Consists-in: malformed #define → tree path;
  malformed #include → captured path. Consequences: none for
  coverage (each single-owner); tests must exercise both.
  Handled-by: fuzz fixtures.

## Verdict

The design survives contact with the real code — no architecture
change needed, and two of the INTENTIO's open mechanics turned out
ALREADY BUILT (directive capture including guard lines; cruda slices).
Chunk B build order: (1) driver captures directivae; (2) boundary arm
(radix + chain check + dedup); (3) reinserenda collection + flush
cursor + included-EOF lookup; (4) extents if approved; (5) fixtures:
flip the Chunk A fail-loud test, preprocessor_test-shape, cursor.c
closure bar, latina.h fons, fuzz shapes, corpus 78/78.

## Open questions — RESOLVED (Fran, 2026-07-02)

1. **C5 — BUILD NOW.** Function-like extents land in Chunk B (~30
   expander lines: record {name-token → extent slice} at layer-0
   collection in _argumenta_colligere; boundary arm emits the slice).
   Closes the function-like deferral while the site is open.
2. **C7 — FIX, via the untaken-arm model.** Fran: bytes in guards
   must never be lost from byte-for-byte reconstruction, but the
   skipped interior needs a concept of being "out of scope." That IS
   the untaken-arm model: when the custos is already defined at first
   processing, DON'T apply guard transparency — route the file
   through the normal region walker. #ifndef evaluates false, the
   interior becomes lexemata_cruda (out of scope but retained), the
   #ifndef/#endif lines are captured normally. ~3-line fix in
   _plagulam_processare's :1172 branch; reconstruction rides the
   standard reinserenda path with zero new scribere code; queries see
   an honest est_sumptum=FALSUM arm.
