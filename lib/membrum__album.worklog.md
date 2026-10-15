# album worklog

## 2026-10-14 - U1: the paginated index, extracted from the calendar

nomenclator-plan U1. Header approved by Fran as `catalogus`; the name
collided at LINK time with the existing public `catalogus_*` functions
of `cellae.h` (math library: catalogus_numerus, catalogus_clausura,
...), so Fran picked `album` (the Roman white board listing names,
album senatorium). Only canon.c has an internal static `album_solum` -
no symbol clash. Lesson: grep `<name>_` across include/ and lib/ before
proposing a library name, not just `ls include/<name>.h`.

The page rules are the calendar's (A6b pagination, the repeated group
heading and heading chains of 2026-10-14) with one generalization: the
calendar recognised headings by id prefix ('g', 'c', 'p<k>' own items,
'p' does not); album rows declare their kind - ALBUM_RES,
ALBUM_TITULUS (owns the items after it, repeated on a split page,
its height reserved), ALBUM_FASCIA (heading that owns nothing, never
repeated). Pure: no island, no layout, no drawing; pages computed
lazily and dropped on every album_addere.

album_pagina reports the repeated heading as an INDEX (-1 = none); the
host decides its id (the calendar keeps "<id>-c").

Ids: `chorda_ex_literis(id, piscina)` already copies into the pool -
my first version wrapped it in chorda_transcribere (a second copy); a
plant removing the "copy" stayed green, which is how that showed up.

Tests: probatio_album (credo v2, 4 sections: paginae, titulus_ultimus,
iteratio, res). Plants: reservation dropped; band repeated; chain of
one row only; preferred page ignored - all red.

## 2026-10-14 - files renamed to the `membrum__` kind prefix

Fran: UI component libraries get a kind prefix so they are easy to
scan (house convention …PTMJ2: double-underscore kind prefixes on FILE
names). album.h/.c, its worklog and its test became membrum__album.*;
functions and types stay `album_*` / `Album`; guard MEMBRUM__ALBUM_H
(a double underscore INSIDE an identifier is not reserved in C89 -
only a leading `__` or `_X` is). First `__` file in include/; aedilis
(`<aedilis corpus=...>`), compile_tests_fontes_generare, the formatter
and the Latin lint all accepted it. Stale build dirs of the old name
removed (build/aedilis/probatio_album, fabrica area) - see memory
renamed-lib-stale-objects.
