# praeconium_cantus worklog

## 2026-10-22 - praeco cantus step C: the agent-side command

`praeconium_cantum_parare(textus, intactus, e, piscina, vitium)` is the only
logic: length cap + `cantionem_legere` (the book's rule) BEFORE any field
is set, then eventus "cantus", genus "intactus"|"", nuntius = text. Its own
lib, not lib/praeconium.c, so colloquia/admonitiones readers of
praeconium.h do not pull in cantuale/cantus/sonitus headers.

Why the cap: `praeconium_scribere` TRUNCATES nuntius at
PRAECONIUM_NUNTIUS_MAXIMUS (CC) - a cut jingle would play wrong in vicus
with no error anywhere. The #define moved from lib/praeconium.c into
praeconium.h so the cap has one definition; its comment documents the
'cantus' event format vicus reads (two string literals in
vicus_applicatio.c, on purpose: vicus must not link cantuale).

tools/praeco.c (glue, untested like `status`): `ambitus()` now fills
tempus/panellus/servitor/via for both status and cantus; `cantus`
[-intactus] <mml> | -titulus T | -genus G (book via cantuale_ordinarium,
-genus seeded from the clock, prints the chosen title); error = causa +
text + caret, exit 1, NOTHING written; `cantuale` lists the book.
Checked by hand against RHUBARB_COLLOQUIA=<tmp>: 4 good calls -> exactly
4 lines, 7 bad calls -> exit 1 and no line.

Suite probatio_praeconium_cantus (4 sections, 34 asserts, incl. a
write/read round trip through JSON with '#', '<', '>'); plants 8/8 red.
