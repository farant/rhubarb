# probationes/fixa/quadrans — provenance

Test photographs for the `quadrans` plan (`project-specs/quadrans-plan.md`,
decision D6), supplied by Fran on 2026-09-30 from his own files. Both
depict artworks long in the public domain; where each PHOTOGRAPH came from
(museum open-access or otherwise) was not recorded — to confirm before
any use beyond tests.

| file | subject | derived from (Fran's original) | reduction |
|---|---|---|---|
| `assumptio.jpg` (571×800, 200,470 bytes) | the Assumption of the Blessed Virgin Mary — Baroque painting: Our Lady on the crescent moon, crown of stars, angels and cherubs | `assumption.jpeg`, 1044×1462, SHA-256 `251b46a41aed94169b06181c92e782313d851eb69d9a76f53269db39634fb140` | `sips -Z 800 -s formatOptions 85` |
| `christus_sculptus.jpg` (800×729, 174,406 bytes) | bust of Christ, painted terracotta (Renaissance), before a dark grey gradient | `jesus sculpture.jpg`, 2342×2136, SHA-256 `2520a457549cbd5f09e716d2e1b4c7ea225d63fbe6d4d1d0272e3bc2eadf801b` | same |

SHA-256 of the committed files:

    bed7087895661c7b72ed0cb1e28e890c314422b622bca18bc2e3785f6f7ed4a5  assumptio.jpg
    ffc9d2106c03bc7fedb6d036fb885c66e7f86e6bd98eb807e31eadb038295dc7  christus_sculptus.jpg

Why these two: the painting is saturated colour, a smooth radiant glow and
fine detail (colour fidelity, D2's extreme-vs-mean measurement); the bust
has a large smooth dark gradient (banding under the 256-colour path, D4,
and what dithering is for) and fine texture (curls, flaking paint).
Originals are reduced to a long side of 800 px because every consumer
pre-scales far below that and committed fixtures stay in history forever.
