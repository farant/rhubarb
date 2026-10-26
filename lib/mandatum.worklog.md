# mandatum.worklog.md

## 2026-09-04 — natus (ludus T2)

Six draw primitives + coetus spans (`magnitudo_arboris`) in a flat
xar; STML round-trip; structural equality. Design: pictor-spec.md §3.2,
ludus-brainstorm.md §XV. Coetus carries clip/translate/integer-scale as
PROPERTIES — no push/pop commands exist, so mismatch is structurally
impossible. No id/actio/partes on any primitive: those live on the
componens (T3); coetus carries only an optional `provenientia` for the
inspector, which targeting never reads. Serialization internals take
`constans Mandata*` (a `-Wcast-qual` lesson from drafting the plan).
Probatio: 16 assertions incl. a nested coetus round-trip and a planted
inequality. Examen ACCIPE on both files at birth.


## 2026-09-05 — coordinatae signatae (ludus P3 T1)

`Punctum` and `Fines` are `s32` now, and everything that carries
them follows: `componens` (fines, translatio), `destinatio`,
`manus_ludus`, `motus` (pan), `derivare` (the last click), and the
three signatures that took coordinates as `i32`
(`mandata_textus`, `mandata_coetus_incipere`,
`componens_ponere_transformatio`). Done first in Plan 2, before any
consumer of negative pan exists, so nothing built in P3 inherits
the unsigned wart.

What rippled and how: the two serializers each gained an `s32`
twin for their attribute reader and writer (`attributum_s32`,
`attributum_signatum`) and the six coordinate sites moved to
them; counts, scale, colour, thickness stay `i32` on the
`attributum_i32`/`_numericum` pair. The local `v` of both `puncta`
readers is `s32`. `Eventus.datum.mus` stays `i32` at the platform
boundary and is cast once in `destinatio_geometrica`, once in
`derivare`, and once in the hand's `mus` helper. `chorda_ut_s32`
is `strtol`, so `"-40"` parses.

The negative guard in `destinatio.c`'s `invenire` is GONE: it was
a workaround for unsigned child space (a negative axis could not
be a legitimate child), and with signed fines a child at
(-20,-20) is legitimate. The probatio pins one: `umbra` inside the
pannus, hit through the pannus's translation. `motus`'s probatio
flushes a negative pan and reads `"-40"` back.

Gate: the eight ludus suites (mandatum, componens, motus,
destinatio, manus_ludus, dispensator, pictor_toy, eventus_stml);
the replay exemplar did not change. Planted fault: the plan said
"drop the translation term"; dropping the X term stayed GREEN —
every translation in the probationes is vertical (0,50), so the x
term is always zero and the plant was mute. Dropping the Y term
went red at the tabula-through-pannus assertion (line 148), green
on revert. A horizontal translation case is a cheap addition for
T2's probatio; noted there. Examen ACCIPE
on all twelve touched files; formator 0.

## 2026-10-23 — text and image source are copied (A2b closed in the library)

`mandata_textus` and `mandata_imago` stored the caller's chorda BY
REFERENCE, while `mandata_linea`/`mandata_polygonum` already copied
their points (`puncta_copiare`). Every caller therefore had to keep the
text alive until rasterization (kalendarium rule A2b), and it bit twice:
10-13 weekday letters pointing at a stack buffer drew as random glyphs;
10-23 month feast titles pointed into projection-cache entries that the
LRU evicted before rasterization (garbage on every mouse move). Both
were fixed at the call site and the rule written down - a discipline
that the next caller can forget.

Now both copy into `m->piscina` with `chorda_transcribere` (empty ->
empty). The draw list owns everything it holds. Cost: the text bytes,
once per command per frame; both hosts (ludus_fenestra,
ludus_tessera) vacate the frame pool and create a fresh Mandata every
frame, so nothing accumulates. Call sites that already copy now copy
twice - harmless, left in place.

Test: probatio_mandatum `possessio` - queue text and an image source
from local buffers, overwrite the buffers, the commands still read the
originals at other addresses. Plants: each copy removed -> red. Full
root suite 371/371, ludus_tessera 8/8, tessera 15/15.
