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
