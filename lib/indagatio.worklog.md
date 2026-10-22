# indagatio worklog

## 2026-10-21 — S1: library on fixtures (genera-datorum D5)

Spec `project-specs/indagatio.md`; S0 measured Fran's volume (counts
only): 61 agenda items, 34 contacts, 79 day notebooks, 35 contact-note
notebooks, 36 common-notebook pages (74.5 KB, the bulk of the text);
whole index from nothing ~40 ms (replaying notebook histories), FTS5
insert ~2 ms, queries 0.01-1.4 ms. So: in memory, built on the FIRST
search, no file.

**Deviations from the spec, approved with the header (Fran 10-21):**
a key may yield SEVERAL rows (a day notebook keys on the date - a
spatium key cannot contain '/' - and its rows are the pages); the
declaration carries only the cluster label (`<quaerendum genus>`),
`computare` builds the full route; `enumerare` lives in conspectus
(parare refuses a searchable declaration without it); indagatio finds
the searchable kinds in the registry itself.

**scrinium binds an EMPTY chorda as SQL NULL.** Two bugs from one
cause: `contextus TEXT NOT NULL` refused every row with an empty
context, and `?2 = ''` (no cluster filter) was never true, so every
all-kinds query returned nothing. Fixed with nullable display columns
and `coalesce(?2, '') = ''`. Any house SQL comparing a bound chorda to
'' has the same trap.

**Plain words:** split on ASCII whitespace; a term is kept if it has an
ASCII letter/digit or any non-ASCII byte; each term is quoted (`"` ->
`""`) with a prefix star. 2000 random strings over `"*()-:^+.,'` +
letters + é + em dash never produce a MATCH error - including a term
of a lone em dash (`"—"*`: FTS5 tokenizes it to nothing, no error).
No Unicode letter classifier exists in the house (wish …K688S).

**Memory test, three false starts** (all fixture/measurement problems,
not library leaks): (1) agenda renames grow agenda's own history ~2 KiB
per act, which buried the signal; (2) comparing with/without searches
doubled the noise (5.3 MiB swing); (3) a fixture `computare` that read
ALL acts of its key into the entry's arena made the cache grow with the
data. Final fixture kind `titulus` keeps only the last act (scratch
arena for the read). 3000 append+search cycles: 0.45-1.8 MiB; plant
"old dirty-key arena not destroyed" +13 MiB; bound 5 MiB.

**An unused arena hides a leak.** The drain used to create a work arena
it only needed for whole-kind rewrites; the plant "work arena not
destroyed" stayed GREEN because macOS footprint counts touched pages
only and the single-key path never touched it. Fixed in the code: the
whole-kind rewrite (`genus_scribere`) now owns its scratch, created and
destroyed in the same function. Also fixed: if creating the new
dirty-key arena failed, the drain dropped the old one (keys lost and
leaked) - it now keeps it.

Plants (silva.planta, all red at their own assertions): 11 - old dirty
arena kept, observer ignores keys, observer ignores "all", old rows not
deleted, version not bumped, quotes not doubled, no prefix star,
highlight flag inverted, clusters not sorted, page ignored,
verification blind. Not plantable: `genus_scribere`'s scratch not
destroyed (whole-kind rewrites are rare; paired in one function).

## 2026-10-21 — S2: the house's searchable kinds (lib indagatio_genera)

Four kinds in one library, `indagatio_genera` (mos `conspectus_dies`:
declarations `lib/conspectus_{res,notae,liber,personae}.stml` infixed,
one `indagatio_genera_registrare`). Not inside agenda/personae/
scriba_liber: their consumers would all link sqlite through indagatio.

| conspectus | key (typus) | sources | rows |
|---|---|---|---|
| res | item id (nomen) | agenda per-rem | one |
| notae | date (dies) | spatium dies/{clavis} | one per non-empty page |
| liber | page name (textus: names may hold '-') | spatium paginae/{clavis} | one |
| personae | contact id (nomen) | personae per-rem + spatium persona/{clavis} | one |

**Read-only notebooks.** `scriba_liber_aperire*` WRITE page "1" when a
book has no index, so a pure `computare` cannot use them. Two new
read-only readers in scriba_liber (`nomina_legere`, `paginam_legere`;
praefixum NIHIL = the common book's old layout `scriba/paginae` +
`paginae/<n>`); `lineas_legere` is rebuilt on the same internals,
behaviour unchanged. Test X asserts acts and plagulae counts are
unchanged after every read.

**Common notebook keyed per page.** Typing on one page rewrites one row
(counted in the test). Pages are only appended (no delete/reorder API),
so "page n" (index position) needs no fons on the index plagula. If
reorder or delete ever arrives, `liber` needs a fons on `scriba/paginae`
(irritat omnia) - the plagula's via is exactly "scriba/paginae", which
a spatium fons "paginae/{clavis}" never sees.

**Day notebooks keyed per date**, rows per page: adding page 2 must
retitle page 1 ("page 1 of 2"), which per-page keys could not do
without the index.

**Contacts:** personae has no per-item codec (agenda has
`agenda_rem_codificare`), so the per-rem fingerprint is private: every
field shown or searched, '\x1f'-separated. Phones also enter the body
as digits-only suffixes from each digit group on ("+1 (555) 123-4567"
-> "15551234567 5551234567 1234567"), so a number typed from any group
boundary, without punctuation, prefix-matches; "2345" (mid-group) does
not.

**Routes** are what S3 must make real or check: `scriba/<nomen>` does
not exist (scriba has no routes); `calendarium/agenda?res_electa=` -
the manifest says res_electa is for the day view. Day-note hits open
the day (no route field for the notes page).

Forced deviation: the approved header's examples had the wrong
weekdays ("Tue 21 Oct 2026", "Wed 1 Oct 2026"); 21 Oct 2026 is a
Wednesday, 1 Oct a Thursday - corrected in the comment, tests compute
them.

Plants (silva.planta, all red at their own assertions): 15 - done date
from the assigned day, item route inverted, item keys always the first
item, "page n of x" never, day enumeration blind, page title not
trimmed, page numbered from 0, phone digits from two groups only,
contact notes inverted, contact fingerprint without surname, books
without a volume, deleted contact kept (caught only by verificare);
scriba_liber: common-book path wrong, maximum ignored, bad index
accepted.
