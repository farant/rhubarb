# pictor_documentum.worklog.md

## 2026-09-05 — natus (ludus P3 T4)

The document is its stroke log. Truth is the acta table of a
`volumen` (append-only); the bitmap is a projection: nearest
checkpoint at or below the cursor, then the live acta after it.
Undo and redo move a cursor in memory and never write; a NEW stroke
after an undo appends `<ramus ab="seq"/>` first, and replay treats
the acta between `ab` and the ramus as dead. So the log never
truncates and history never lies, which is what makes tier 8
(`pictor_documentum_verificare`: reproject from nothing, compare
sigilla) meaningful. Checkpoint every `intervallum` acta: the raw
RGBA bytes go in as a massa (content-addressed by SHA-256), and the
plagula `checkpoint/<seq>` maps the seq to the massa's hex. The
plagula `documentum` carries the dimensions and interval so
`pictor_documentum_aperire` can rebuild from a volumen alone.

Acta v1: `<ictus instrumentum color magnitudo><punctum x y/>…
</ictus>` — a filled square of side `magnitudo` at every point and
a one-pixel line between consecutive points; `color` is a palette
index. `<ramus>` paints nothing. Every other genus the spec lists
(impletio, figura, insertio, stratum, paletta, selectio_commissa) is
ignored by v1 with no diagnostic; P5 gives each a handler. The
`volumen-creatum` actum the store writes at creation is skipped.

The store is not ours alone — the first run said so. `volumen`
appends its OWN acta: `volumen-creatum` at creation and a
`plagula-condita` for every `volumen_plagulam_condere`, each with a
massa of the plagula's content. So a document's sequence numbers
are not contiguous, the first stroke is not seq 1, and the draft's
undo cursor stepped onto plagula acta. Three consequences landed:
the live-acta reader keeps `ictus` only (and honours `ramus`); the
checkpoint cadence counts LIVE STROKES (`numerus_vivorum`), not seq
modulo interval; and `proicere_ad` finds its base by enumerating
the `checkpoint/<seq>` plagulae and taking the largest seq that is
≤ the cursor AND live after rami — which also closes the
dead-checkpoint hole the plan named. The probatio uses the seqs the
store returns, never literals.

Facts worth keeping: there is no `chorda_ex_s64` — sequence numbers
format through `chorda_ex_f64` with zero decimals, as event
timestamps do. Massae hold zero bytes intact (the projection is full
of them; the checkpoint round trip proves it — the plan's AUDIENDA
question, answered). `volumen_summa_massarum` counts plagula
contents too, so it is never "one per checkpoint".

Measured (spec §10): 200 strokes appended and applied in 26 ms,
0.13 ms per actum — volumen's write rate is not the editor's
bottleneck; one undo at interval 2 in 1 ms. At interval 64 an undo
replays at most 63 strokes on top of a checkpoint: about 8 ms by
the same rate, well under a frame. The interval stays 64 in the
binary.

Gate: `probatio_pictor_documentum` (46). Planted faults: ramus
ignored in the live reader — the incremental projection stayed
RIGHT (the dead stroke is never re-applied to the live bitmap) and
tier 8's `verificare` went red, because the replay differed: that
is the tier earning its keep; the checkpoint plagula never written
(massa stored, early return inverted) — red at the
`checkpoint/<q2>` lookup. Both green on revert. A plant that
deleted the only call to `checkpoint_condere` was refused before
running: `-Wunused-function`. Examen ACCIPE; the latina guard caught `ordinarius` used as
a parameter name (it is `default`) before clang did.

## 2026-10-05 — the golden before the cut (scriba-plan H0)

Before extracting the log engine into `historia`, today's behaviour is
frozen as bytes: `probationes/probatio_pictor_documentum_aurum.c`
scripts a 32×16 document at interval 2 (five strokes = checkpoints at
seq 4 and 7; three undos; two strokes on a branch = `<ramus ab="4"/>`
and a checkpoint at 12; redo refused; undo + redo; reopen) and dumps
the states (cursor, end, live count, sigillum), every act without its
timestamp, every plagula with origin, hash and content, and the three
totals into `probationes/fixa/pictor_documentum/aurum.txt` (59 lines,
identical over three runs). `PICTOR_DOCUMENTUM_AURUM_SCRIBERE=1`
rewrites it.

Read off the golden: checkpoint keys use the STROKE's seq, and the
volume interleaves its own `plagula-condita` acts in the same seq
space (hence "seqs are not multiples of the interval"); a checkpoint's
content (the massa's hash) equals the document sigillum at that act;
the dead `checkpoint/7` stays in the manifest after the branch.

**The existing suite missed both plants.** A renamed checkpoint tag
(`pictor:checkpoint` → another) and `checkpoint_proximus` admitting a
DEAD checkpoint both compiled and passed `probatio_pictor_documentum`;
only the golden caught them (line 40: the tag; line 25: the sigillum
after undo-past-a-branch, rebuilt from dead checkpoint/7). The second
is the subtle rule H1 must keep: a checkpoint inside a span a branch
abandoned is never a base.

## 2026-10-05 — onto historia (scriba-plan H2)

pictor_documentum is now a client of `historia`: it keeps the pixel
half (the RGBA buffer, white `vacare`, `ictum_applicare`, the
`documentum` manifest) and gives historia a `HistoriaProiectio` whose
memory is `tabula->pixela`, genus "ictus", origin "pictor:checkpoint".
The API is unchanged except that `cursor`, `finis`, `numerus_vivorum`
and `sigillum` left the struct (they live in `doc->historia`); the one
field tests read directly gained an accessor,
`pictor_documentum_numerus_vivorum` (seven test sites). 662 → 388
lines.

**The H0 golden came out byte-identical** on the first run, and the two
H0 plants, re-aimed at their new homes (the tag string in this file;
dead-checkpoint admission in historia.c), are caught at the same golden
lines (40 and 25). The ludus_tessera runner's hand list needed
`historia` (0/5 → 5/5: one missing lib, as B3 with dispositio).

**Side finding (pre-existing, not H2):** `AEDIFICARE_SOLUM=1
./apps/pictor/pictor.sh` does NOT build-only - unlike
`pictor_terminalis.sh`, pictor.sh has no such switch and exec'd the
window app (my mistake to assume; it opened the gitignored
`pictor.volumen` read-only in effect - 29 acts written by the OLD code
opened and rendered by the new one; WAL empty; killed). Its 100% CPU
while idle is `ludus_fenestra_currere`: poll → frame → present with no
wait (since ludus P3), unlike the terminal glue's `quies_ms`.

## 2026-10-05 — pictor on a branch (insula-rami-plan R3)

pictor's state now goes through an `InsulaRamus`: `PictorActiones` and
`PictorCompositio` carry one (an unset branch - repo NIHIL - means the
root of the repo the handler/componere was given, so every existing
construction still works and standalone pictor is unchanged: root
201/201, ludus_tessera 6/6, H0 golden identical). Its canons' root
element is now `pictor` (+ an `id` attribute): `canon_iudicare` checks
the judged element's NAME against the canon root, so the same canon
judges a standalone root and a `<pictor id="p1">` mount - the app's
initial insulae and four tests build `<pictor …>` roots now.

Proof (probatio_pictor_ramus): a host store with two pictor mounts;
pictor's real canons and owners on p1; a real dispatcher. The page
size comes from p1, `p` writes `instrumentum` into p1 only (not the
root, not p2; owner check passes), a stroke's act carries p1's colour.
Plants: actions ignoring the branch; componere ignoring it; the canon
root renamed back to `ephemera` (the branch canon then refuses pictor's
own writes) - all caught.

Not yet on the branch (framework, T2/T3): Motus's pan/zoom flush
(`motus_effundere`) writes to the ROOT ephemeral insula; the dispatcher
writes `focus` and `superficies_*` to the root.

**Gate find (same day): an unset branch must be a ZEROED struct.** The
commit's aedilis gate segfaulted `probatio_pictor_actiones` while
`compile_tests` passed it: the test built `PictorActiones actiones;
actiones.doc = doc;` without zeroing, so the new `ramus` held stack
garbage - "repo NIHIL = root" only holds for a zeroed struct. In one
build the garbage was 0, in aedilis's it was not. Four constructions
now `memset` first (actiones ×2, componentia, figurae, plus my own new
test); both headers say "zero the whole struct". The second build
(aedilis's own closure and flags) is what caught it.

## 2026-10-08 - strokes off the canvas: clipped, not wrapped (Fran)

Fran: dragging off the left or bottom edge drew a line across the canvas
to the opposite edge. Cause: `ictum_applicare` cast the stored (signed)
points straight to `i32` - UNSIGNED in this house - for
`delineare_lineam` / `delineare_rectangulum_plenum`. x = -10 became
~4.3e9; the line was walked toward it (and, in a test, effectively
forever - the first red run hung 9 minutes). Now each segment is
clipped to [0, w) x [0, h) with Cohen-Sutherland in s64 before any cast,
and each brush dot is intersected with the canvas. A drag that leaves
the canvas stops at the edge.

Test lesson: the bug's red state is a HANG, so the off-canvas strokes run
first in a CREDO_NON_PENDET child with its own temporary volume (a
forked child must not write the parent's volume); the pixel checks run
only if that passed. Plants: no clipping and no trivial reject = named
hang; wrong left/bottom edge = pixel checks.

## 2026-10-09 - canvas clears to the theme background (Fran: no yellow)

`vacare_fundo` clears to `thema_color(COLOR_BACKGROUND)` (scriba and
terminale's background) instead of colouring-palette entry
PALETTE_WHITE, which the theme renders as bright yellow. Trap: there
are TWO palettes - `thema_color` (semantic) resolves through
`color_ex_palette`, while stroke colours index the COLOURING palette
(`thema_color_ex_indice_colorationis`); the semantic colour's index
(`thema_palette_index`) read in the colouring palette gave the ink
colour, and the spray test went blank. Use the semantic colour itself.
Migration: the background is not in the acts, but CHECKPOINTS store
rendered pixels - drawings saved before this keep the old yellow
wherever they restore from a checkpoint; new canvases are neutral.
Goldens: aurum.txt (fingerprints only), pictor_prima specimen.

## 2026-10-09 - stroke colours = the Aquinas palette itself

Fran: the colour palette showed repeats ("there should be 16 unique
colours"). The theme's Aquinas palette IS 16 distinct colours
(`palette_aquinas`, `color_ex_palette`); pictor resolved stroke colours
through `thema_color_ex_indice_colorationis`, which is not a palette
but the SYNTAX-highlighting role map (0-12: command, tag, string...;
13+ fall back to the text colour) - so 0/13/14/15 and 7/9 coincided,
and "white" (5) was the gold of role "number". Strokes and swatches now
use `color_ex_palette` (swatches as RGBA, since COLOR_MANDATI_INDEX is
resolved through the role map by the rasterizers). Existing drawings
re-render in the true palette colours. Test: the palette's 16 options
are distinct (probatio_pictor_palette VIII).

## 2026-10-09 - P2 spongia (eraser)

Fran's plan: the eraser paints the canvas background. Named `spongia`
(Romans wiped wet ink off with a sponge); key `e`.

- Log: `<ictus instrumentum="spongia" magnitudo>` with points and NO
  colour - replay paints `thema_color(COLOR_BACKGROUND)`, the same call
  as `vacare_fundo`, so erased areas and a fresh canvas always agree.
  (Checkpoints hold pixels, so a theme change would still show the old
  background wherever a checkpoint restores - same caveat as P0.)
- Size: square of PICTOR_SPONGIAE_LATUS (16) x magnitudo, centred
  `[c - latus/2, c + latus/2)`. A 1x1 eraser (brush's size) is useless,
  so it gets its own base like the spray radius.
- Sweep rule: a square at the first point, then at EVERY Bresenham
  point after the start of each segment. A fast drag gives two points
  far apart; stamping only at points left gaps (plant S2 caught it).
  The preview figure (pictor_figurae.c spongiam_praevidere) follows
  the same rule and the test counts the squares exactly (40..50 = 11),
  so the two copies of the walk cannot drift silently.
- ictum_tractare / ictum_scribere now take the tool's litterae instead
  of a b32 aspergillum; pointer comparison against the static
  litterae_* is deliberate (one tag per tool).

## 2026-10-09 - P3 exemplaria (patterns)

- Rule: a pixel the brush or spray touches gets `color` where the
  pattern bit at its CANVAS (x, y) is set, `color_secundus` where not;
  colour -1 = leave the pixel alone. Pattern 0 = solid. Both new log
  attributes are written only when non-default, so old logs mean what
  they meant.
- Brush = two passes through delineare's existing MODUS_EXEMPLAR:
  foreground with the pattern, background with the INVERTED bytes
  (`~bitus[k]`). Solid strokes stay in MODUS_SOLIDUS, so pre-P3
  drawings replay byte-identically (aurum seals unchanged).
  `penicillum_pingere` is the old brush loop pulled out so it can run
  twice.
- Spray sets single pixels, so each dot asks `atramenti_pixelum`.
- Bug fixed on the way: a brush stroke with foreground "none" (-1)
  called color_ex_palette(-1), reading 3 bytes BEFORE the palette
  array. "none" is now a skipped pass (test IV pins it: plant X4).
- Twin/tessellation: the 4th bar square moved the status text from
  x 84 (= cell 14 exactly) to x 110 -> cell 18 in the terminal twin.
- Fran: pattern palette options use the chosen fg/bg (title
  "exemplar:<n>:<fg>:<bg>", same as the bar square); both none falls
  back to 1-bit text colour so the patterns stay legible.

## 2026-10-09 - P4a brush sizes

- Sizes are PER TOOL now (Fran agreed): ephemeral `magnitudo_penicilli`
  (brush diameter px; options 1 2 3 4 6 8 12 16 32 64 - Fran asked for
  32/64) and `magnitudo_aspergilli` (spray multiplier, UI in P4b). The
  generic `magnitudo` left canon/init/domini. The LOG keeps one
  `magnitudo`, written from the current tool's attribute (eraser: 1).
- Brush = disc (pictor_disci_pixelum: doubled coords from centre,
  a^2 + b^2 <= n^2; n <= 3 full square) swept by `verrere`: per
  segment clip with margin n-1, stamp the clipped start, walk, then
  stamp each point. At n = 1 that is exactly the old
  delineare_lineam + 1x1 rect, including off-canvas strokes.
- TIE RULE: delineare_lineam uses STRICT comparisons (e2 > -dy,
  e2 < dx); my P2 eraser walk used >= / <=. Python brute force over
  120x120 slopes: each side differs on ~16% of slopes, x-side ties only
  on steep lines like (1:2). pictor_lineam_ambulare uses delineare's
  rule so old drawings replay identically. The eraser moved onto it
  too (edge pixels can differ at ties for strokes made since P2 today).
  The aurum seals did NOT pin this (plant M7 survived): test VIII now
  draws 8 lines incl. both tie kinds and compares against
  delineare_lineam itself as the oracle.
- Preview: stamps every max(1, n/4) steps + segment end (a 64 brush
  swept per pixel = ~25k rects/frame); real colour, pattern only on
  release. Name collisions on the way: `magnitudo` (= sizeof macro)
  and `Sigillum` (sigillum.h hash seal type).
- Twin: status text x 136 -> cell 23 (tessellation rounds to NEAREST:
  110 -> 18, 136 -> 23).

## 2026-10-09 - P4b spray sizes

- Fran's options: 1 2 4 8 16 (radius 8..128). Density is LINEAR (Fran
  agreed): 6 x m dots per point and m per 8 ms of dwell. Constant
  density would be 6 x m^2 = 1536 per point at 16, too heavy for the
  live preview (1x1 mandata). m = 1 unchanged.
- The size square follows the current tool: brush "magnitudo:<n>",
  spray "magnitudo:aspergillum:<m>", eraser "magnitudo:spongia:16"
  dimmed with an EMPTY action (clicking opens nothing). It never
  disappears, so the status text does not jump between tools.
- magnitudo.ponere reads the current tool and checks the value against
  that tool's own table; the two tables are duplicated in
  pictor_actiones.c and pictor_componentia.c (tests click the last
  option of each to pin them).
- First draft of section IX never held the spray still, so dwell
  scaling was unpinned; added an 80 ms hold (plant B7).

## 2026-10-09 - pictor-strata L2: layered document

Plan: project-specs/pictor-strata-plan.md. Grammar in the header.
- Layers live in PictorDocumentum.strata[16] (bottom first). Buffers
  are never freed (arena): slots beyond numerus_stratorum keep spare
  buffers - delete moves the deleted buffer there, add reuses it.
- Strokes paint into their layer by pointing doc->tabula at the layer
  buffer around ictum_pingere (every rasteriser reads doc->tabula);
  doc->tabula is the COMPOSITE the rest of the app and every test
  reads. The eraser now writes alpha 0 (transparent) on its layer.
- Compositing runs inside the history's sigillare hook - historia calls
  it exactly when the state settles (after an action, after a replay,
  after vacare), so there is no "remember to composite" call site.
- Old drawings: the old raw checkpoint fails decodificare (no
  "STRATA1 " magic) -> replay. Evidence they look identical: the
  golden test now asserts, at every step, that the COMPOSITE's raw
  hash equals the old seal recorded in the frozen
  probationes/fixa/pictor_documentum/aurum_ante_strata.txt (7/7). The
  byte golden aurum.txt was re-promoted (named format change): only
  seals and checkpoint hashes changed, plus the new "compositum" lines.
- Measured (test build, 468x440, 8 layers): ~29 ms per stroke commit
  (paid on mouse-up), ~43 ms to reopen from an encoded checkpoint.
  Mostly SHA-256 over every layer per stroke; follow-up = per-layer
  cached seals, re-hash only the touched layer (needs PictorStratum
  fields -> header approval).
- Bench gotcha: a fortify trap (EXC_BREAKPOINT in __chk_fail_overflow)
  was my bench's own sprintf into a 128-byte buffer, not L2.
