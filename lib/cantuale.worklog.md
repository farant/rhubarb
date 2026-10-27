# cantuale worklog

## 2026-10-22 - the jingle book (S4, before the vicus host)

Fran asked for a library of jingles so a host can "pick a random one of
the kind that's wanted". `cantuale` (medieval Latin: a choir's book of
chants) reads named melodies tagged by occasion (`permissio`, `finis`,
`vitium`) and mood (`laetus`, `gravis`) from STML, and picks one at
random per tag.

**Judge the whole book at load.** Every text goes through `cantus_legere`,
every `@preset` through `cantus_praeparatum_sonitus`, notes <= 32. One
bad jingle fails the load with the jingle's title and an offset into ITS
text (`'cadentia' 9: longitudo non est potentia II`). A notification that
only fails when it fires is the failure mode this avoids. For an unknown
preset the offset points at the `@`, not at the note (`cantus_iussa_parare`
points at the note): in a book you fix the name.

**32 notes is a host constraint, not a musical one.** `cantus_iussa_parare`
claims a voice per note up front, and at equal priority the rector steals
the OLDEST voice, i.e. an unplayed note of the same jingle. A host with a
fixed mixer (vicus) can size it once: 32 voices covers any book jingle.

**Raw text.** MML uses `<`/`>` for octaves, so the text lives in a raw
element `<textus!>...</textus>`. Raw content is VERBATIM: `&lt;` would
reach the MML reader as four characters (caught while writing the seed
book). Only `</textus` followed by `>`, `!` or a space ends it, which
MML never contains.

**No repeats.** `cantuale_eligere(..., vitanda)` skips the previous pick
only when it is a candidate IN THIS BOOK and another exists. A first
draft checked only "vitanda has the tag": a pointer from another book
would then never match while the count was still reduced, and the walk
could run off the end. Now the skip flag is set by finding the pointer
among the candidates.

**Embedded, like sancti.canon.** `cantuale.stml` (repo root) ->
`tools/cantuale_infigere.sh` -> `lib/cantuale_infixum.c` (GENERATUM, a
byte array), generator action `cantuale_infixum` in aedificatio.stml.
The suite's `ordinarium` section loads the shipped book and requires >= 2
jingles per occasion (so no-repeat always has an alternative). A broken
jingle in the book turns the suite red (planted by hand: `l3` in
`cadentia`, regenerated, red, restored, byte-identical). The book is
written in the formatter's house form (`</>` closers).

Seed book: 12 jingles. permissio ends unresolved (a question), finis
returns to the tonic, vitium is low / minor / chromatic. Values are
first drafts for Fran's ears.

`auditio`: `-cantuale` lists, `-genus G [seed]` picks (clock seed when
none given; 80 seeds over four `finis` jingles gave 19/21/19/21),
`-cantilena T` plays by name.

Suite: 6 sections, green on the first implementation run; plants 9/9
red then green, plus the book plant.

## 2026-10-22 - cantionem_legere: one jingle outside the book (praeco cantus)

For `praeco cantus` (Claude-sent jingles, wish …RJSBFQ) a single MML text
must pass exactly the checks a book entry passes. The checks that ran
after `stml_textus_valor` (cantus_legere, 0 notes, > XXXII notes, unknown
'@' preset at the '@') moved into `_cantilenam_iudicare`, called by both
`_cantionem_legere` (book) and the new public `cantionem_legere`. Suite
section `cantio` asserts the SAME causa string for the same text both ways.
`cantionem_legere` does not copy the text: notes' `praeparatum` chordae
point into it, so it must outlive the Cantio (tintinnabulum copies first).
A trailing unknown preset with no note after it ("c @nullum") is accepted -
in the book too: nothing resolves it, nothing plays it.
