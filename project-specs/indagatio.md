# indagatio — search across every kind of data (genera-datorum D5)

Status: decided with Fran 2026-10-21 (§X). Builds on R5 / D4
(`proiectiones.md`, lib `conspectus`). Latin names are proposals
until the header text is approved.

One search pane on the right of vicus. You type plain words; hits
from every searchable kind come back CLUSTERED by kind (agenda,
notebooks, contacts, ...), each hit presenting itself with a title, a
context line and a snippet, and Enter opens it. A search is a route
(`indagatio?q=rhubarb`), so it is linkable and lives in the pane's
history. Kinds OPT IN by declaration; nobody writes indexing code.

## I. Doctrine

1. **Index current state, never acts.** A notebook line typed in 200
   edits must be found once, an undone item never, a renamed item only
   under its new title. The text indexed for an item is what a pure
   function of the folded documents says NOW - a view model (R5).
2. **Opt-in by declaration.** A kind is searchable because its
   declaration says so (`<quaerendum>`, §IV). The kind's author writes
   one pure function "key -> presentation + text"; maintaining the
   index is the library's job. Later (D6) a category default may turn
   opt-in into opt-out; secrets are never indexable.
3. **The index is DERIVED.** Rebuildable from truth at any time, never
   backed up, never inside the volume (R5 decision X.3). First built
   in memory at open; a sibling file only when a measurement asks.
4. **Lazy, like view models.** An act only marks index rows dirty;
   rows are rewritten when someone searches (or at an idle pulse).
   Typing costs nothing extra; a search catches up first, so it is
   never stale.
5. **A search is a live query.** A link reproduces the QUERY, not the
   results of the day it was made (Fran 2026-10-21). Frozen "as of"
   links wait for `@seq` routes (semitae §VI-d, parked).
6. **Plain words** (Fran 2026-10-21). FTS5 operators are hidden; the
   query a person types can never be a syntax error.

## II. What exists today (cited)

- **FTS5 is compiled into the apps and tests:** `aedilis.stml:90`
  (`-DSQLITE_ENABLE_FTS5`), `compile_tests.sh:51`,
  `ludus_tessera/compile_probationes.sh:268`.
- **House prior art - the ledger:** `gesta/fontes/gesta.c:52` (`res_fts
  USING fts5(res_id UNINDEXED, titulus, corpus, notae,
  tokenize='unicode61 remove_diacritics 2')`), a `sordidae` table of
  dirty ids drained lazily before every query (`gesta_fts_exhaurire`,
  `gesta.c:3926`), ranking by `bm25` (`gesta.c:4103`). Its lesson:
  an invalid MATCH once returned "nothing found" silently (vitium
  01M350VMNF, comment at `gesta.c:4114`) - hence §I.6. Its term
  splitter (`tabularium.c:5190`) is ASCII-only - not copied.
- **In-memory database:** `scrinium_aperire` passes the path to
  `sqlite3_open` unchanged (`lib/scrinium.c:41`), so `":memory:"`
  works.
- **View models and invalidation (D4):** lib `conspectus`
  (`include/conspectus.h`): declarations in STML beside the C, a seq
  cursor, routing of acts by SPACE prefix, `per-rem` diffing of keys
  AND fingerprint per item (catches renames), one host-owned registry
  per volume (D4c: vicus `ContextusKalendarii`).
- **Kinds that would opt in first:**
  - agenda items: `agenda_res` / `agenda_rem_codificare`
    (`include/agenda.h:181,189`) - title, notes, rubrics, day.
  - notebooks: the common vicus notebook (`scriba/paginae`, pages
    `paginae/<n>`) and day notebooks (`dies/<date>/paginae/<n>`)
    (`include/scriba_liber.h:36-47`); page text via the page document;
    `scriba_liber_lineas_legere` reads a book's lines read-only
    (`:93`).
  - contacts: `Persona` (`include/personae.h:59-75`) - names, title,
    rubrics, phones, emails, birthday; their NOTES are a scriba
    notebook in space `persona/<id>` (`include/nomenclator.h:52`,
    `lib/nomenclator.c:44`, P5b), the same mechanism as day notebooks.
  - saints (reference data): `sancti_congruentes` (`include/sancti.h:
    205`) is the saints pane's own fuzzy NAME search (fzf-style
    subsequence), a different tool from full-text.
- **The right-hand pane cycle:** `cyclus_dexter` = notifications,
  sessions, agenda (`lib/vicus_applicatio.c:2902`).
- **Routes:** semitae (`project-specs/semitae-spec.md`): status values
  become query keys (§IV), percent-escaped; the universal relation
  `quaestio` = "search within this widget", parameter `q`, Cmd+F
  (§VI-b, §IX).
- **Name check:** `indagatio` appears only in an unrelated knotapel
  identifier (`Scr_Indagator`); `quaestio` is taken (`lib/quaestio.c`,
  `lib/vicus_quaestio_*` = asking Claude).

## III. The model

```
documents (agenda, personae, scriba pages, saints corpus)
  | f per kind - pure, declared <quaerendum>        (conspectus)
presentation + text, one per KEY (item id, page path, saint id)
  | rows rewritten lazily when their key goes stale  (indagatio)
FTS5 index: kind, key, title, context, route | text columns
  | plain-word query -> clusters by kind, pages    (indagatio)
results (cached per query and index version)
  | read by
search pane (right-hand cycle), route indagatio?q=...
```

- **Kind** (`genus`): a searchable declaration; its label is the
  cluster heading ("Agenda", "Notebooks", "Contacts", "Saints").
- **Key:** what one hit is - an agenda item, a notebook PAGE (not a
  line: the snippet shows the line), a contact, a saint. A contact's
  notes belong to the contact: ONE hit per contact whose text is its
  fields plus its notebook's lines (sources: `personae` per-rem and
  spatium `persona/{clavis}`); the snippet shows whichever matched.
- **Presentation:** title (a field, or computed: "19 Oct · page 2 of
  3"), context line ("due Tue 21 Oct · #home", "done 1 Oct"), route
  (a template filled with the key). Stored UNINDEXED next to the text,
  so a result list renders without reopening any document.
- **Text:** one or more weighted columns (title, body).

## IV. Declaration

A kind is a conspectus declaration with one more element:

```xml
<conspectus nomen="res-textus" versio="1">
  <clavis typus="id"/>
  <fons documentum="agenda" irritat="per-rem"/>
  <quaerendum genus="Agenda"
              semita="calendarium/dies/{dies}?index.res_electa={clavis}"/>
</conspectus>

<conspectus nomen="pagina-textus" versio="1">
  <clavis typus="via"/>
  <fons spatium="{clavis}"/>
  <quaerendum genus="Notebooks"
              semita="calendarium/dies/{dies}?notae.pagina={pagina}"/>
</conspectus>
```

The C side (beside the STML, as for `dies`): `computare` returns a
fixed struct (proposed `IndagatioRes`: titulus, contextus, corpus,
semita) instead of a free view model, plus ONE new hook,
`enumerare(ctx, piscina) -> Xar of keys` (every item id, every page
path), because the index needs keys nobody has read yet. Route
template fields other than `{clavis}` are filled by `computare`.

*Amended at S1 (header approved by Fran 2026-10-21):* the declaration
carries ONLY the label (`<quaerendum genus="Agenda"/>`); `computare`
returns `IndagatioConspectus` = the ROWS of one key (several allowed:
a day notebook keys on the date - a spatium key cannot contain '/' -
and its rows are the pages, each "page n of x"), each row with its
full route. `enumerare` is set on the conspectus registry
(`conspectus_enumerationem_ponere`; parare refuses a searchable
declaration without it); indagatio finds every searchable kind in the
registry itself, so adding a kind touches nothing in the host.

## V. Keeping the index current

- **What is new in conspectus:** today only CACHED entries go stale.
  The index needs to hear about every key whose sources changed,
  read or not. Proposed: a per-declaration observer (`auditor`)
  that `conspectus_renovare` calls with each key it would make stale
  - `per-rem` already knows the changed ids, `spatium` knows the key
  from the act's space, `omnia` means "all keys" (re-enumerate). No
  second router: acts are mapped to keys in ONE place.
- **indagatio** owns the in-memory FTS5 database, a dirty set (like
  the ledger's `sordidae`) and a version counter. The observer adds
  keys to the dirty set; before a query, dirty keys are recomputed
  through `f` and their rows replaced (or deleted, when `f` says the
  key is gone). Each rewrite bumps the version.
- **Startup:** enumerate every key of every kind, compute, insert -
  measured first (§IX S0).
- **Verification:** rebuild the whole index into a second in-memory
  database and compare row by row (same rule as
  `conspectus_verificare`).
- **Other processes:** same limit as D4c - an open agenda document
  does not see another process's acts; notebooks are read from the
  volume and are not affected. Parked with …RB9Z5.

## VI. Queries

- **Plain words:** split on non-letters with the house Unicode
  classifier (`runae`), drop empty terms, quote each term and add a
  prefix star: `rhub tart` -> `"rhub"* "tart"*` (implicit AND).
  Quoting makes every input valid; an empty query returns nothing,
  not everything.
- **Tokenizer:** `unicode61 remove_diacritics 2`, as the ledger.
  No stemming (the built-in porter stemmer is English-only).
- **Ranking within a kind:** `bm25`, title column weighted above
  body.
- **Cluster order** (Fran 2026-10-21): by each cluster's BEST hit if
  that signal proves meaningful across kinds; otherwise alphabetical
  by cluster label. Risk: bm25 length normalization favours short
  texts (agenda titles) over long pages; S4 decides with real data.
- **Overview and pages:** the overview shows the best N hits per
  cluster with "+ k more"; a cluster page (`&genus=…&pagina=…`) lists
  one kind, paged by the pane height (house rule: pages, not
  scrolling).

## VII. Results and routes

- **Route:** the pane's genus is `indagatio`; its status is
  `q`, `genus` (one cluster or none) and `pagina`:
  `indagatio?q=rhubarb`, `indagatio?q=rhubarb&genus=notae&pagina=2`.
  Linkable, in the pane's back/forward history, restored on restart
  (semitae §VI-e).
- **Results cached** per (query, index version): repainting costs
  nothing; any rewrite of the index invalidates. (Not a conspectus
  entry: the index is not a document source, so a version number is
  the whole dependency.)
- **Enter** opens the hit's route in the main stack.
- **Opening the pane:** Cmd+Shift+F from anywhere in vicus (focus in
  the query field), and the normal right-hand cycle (Fran 2026-10-21).
- **The `quaestio` relation** (Cmd+F, "search within this widget"):
  for a widget without its own search, it opens the global pane
  filtered to that widget's kind (`indagatio?q=…&genus=…`). Saints
  keep their own fuzzy NAME search for Cmd+F in the saints pane, and
  are ALSO indexed in the global search (Fran 2026-10-21).

## VIII. Testing

- Index == rebuild after random sequences of creates, renames, edits,
  deletes, undo/redo, notebook typing (the verification of §V).
- A rename is found under the new title only; an undone item is not
  found; a deleted page disappears.
- Laziness: N acts with no query rewrite no rows; one query rewrites
  only dirty keys (counted).
- Query sanitizer: every byte string produces a valid MATCH (fuzzed);
  operators (`OR`, `NOT`, `-`, `"`, `(`, `*`, `:`) typed by a person
  are plain text.
- Routes round-trip (semitae rule); the pane shows the same results
  for the same route.
- Memory: index and dirty set bounded across long edit sessions
  (warm-up phase, as in conspectus).
- Plants through `silva.planta`, red first.

## IX. Phases

| phase | content |
|---|---|
| **S0** | measure: counts on a scratch copy of Fran's volume (items, contacts, notebook pages, text bytes - counts only, never content); time to build an in-memory FTS5 index of that size on fixture text |
| **S1** | lib `indagatio` + the conspectus observer and `enumerare` hook (header text first); fixture kinds only; lazy maintenance, verification, sanitizer, clustering, pages |
| **S2** | real kinds: agenda items (done ones marked), notebook pages (day and common), contacts (fields + notes notebook) |
| **S3** | the pane in the right-hand cycle, route `indagatio?q=`, Enter opens; Fran's look |
| **S4** | cluster order decided on real data; saints (reference data, indexed once per corpus version); the `quaestio` relation |
| **later, on PULL** | index file beside the volume (startup measured too slow); stemming; communications; "as of" links; D6 category defaults |

## X. Decisions (Fran, 2026-10-21)

1. A search widget in a right-hand pane, across ALL kinds, clustered
   by kind.
2. Each kind presents itself with a title - a field or computed
   ("page n of x").
3. Searches are routes: linkable, live queries.
4. Cluster order by best hit if the signal is good; otherwise
   alphabetical by cluster name.
5. Plain words; FTS5 operators hidden for now.
6. Name: lib and pane genus `indagatio`.
7. Saints: their own fuzzy name search stays (Cmd+F in the saints
   pane) AND they are indexed in the global search.
8. Open with Cmd+Shift+F, and through the normal right-hand cycle.
9. Contacts: names, title, rubrics, emails, phones AND their notes
   notebook are searchable (one hit per contact).
10. Completed agenda items appear, marked done in the context line.

## XI. Open questions for Fran

None at the spec level; header text and Latin names come with S1.

## XII. AUDIENDA

- **Not measured:** startup build time; whether bm25 scores are
  comparable across kinds of very different length.
- **Not verified:** that conspectus can report stale keys for
  entries never read without restructuring `renovare` (§V) - S1
  header work decides.
- **Not checked:** how many notebook pages, items and contacts Fran's
  volume holds (S0, counts only).
- **Not checked:** the route of an agenda item WITHOUT a day (the
  §IV template assumes one; undated items probably open the agenda
  view with the item selected) - S2.
- **Not designed:** search inside pictures (pictor) or blobs (PDF
  text); communications (no such documents yet).
- The other-process limit (§V) is inherited from D4c, not new.
