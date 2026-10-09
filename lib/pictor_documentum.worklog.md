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
