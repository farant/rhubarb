# Vocabularium — Latin vocabulary study app (spec)

2026-08-09. Approved design from brainstorm. A real vitrea app Fran uses
daily while reading Latin, doubling as the first genuine silex/silicetum
long-runner. Two deliverables, deliberately entangled:

1. **The app** — capture dictionary-shaped vocabulary per book/chapter
   while reading; drill it with flashcards.
2. **The silex harvest** — the app is scaffolded by `silex novum` and
   every place novum falls short of a vitrea+volumen app is hand-carried
   in `aedificare.sh` and recorded as a measured gap on desideratum
   **01KZ6CSBQN**.

## Rulings from the interview

| Fork | Ruling |
|---|---|
| Study mode | Capture + flashcard drill (no SRS in v1; grows on pull) |
| Entry shape | Dictionary-shaped at capture: lemma, formae, genus, significatio, exemplum? |
| Book model | Multiple books with chapters; sticky current position |
| UI paradigm | Purpose-built keyboard-first screens (forum-style), NOT mensa |
| Data store | One volumen, events + fold (mensa's proven pattern) |
| Surface | Mac only v1 (vitrea servus later on pull) |
| Drill | Scoped deck + wrong-recycle; Latin→gloss default, reverse toggle |
| Name | vocabularium |
| Build approach | Approach 1: app-first, hand-carried build, silex gaps measured |

## Placement

- Project: `~/Documents/projects/silicetum/vocabularium/` — long-runner
  class (accumulates version skew for future `renovare` dogfood).
  Genuinely outside git; the project's own `vocabularium.volumen` (silex
  VCS document) is the history.
- **Data volumen: `~/.rhubarb/vocabularium.volumen`** (mensa state
  convention). It MUST NOT live in the project directory: silex document
  identity = lone `*.volumen` in directory wins, and the project root
  already holds the VCS document — two volumina is a loud refusal.
- Interim backup habit: `cp ~/.rhubarb/vocabularium.volumen` somewhere
  periodically (silicetum protocol: losses are lessons, but study data is
  real).

## Data model — events + fold

Truth = volumen acta (JSON datum); state = fold at launch in JS. An
entity's id is **the seq of its birth actum** — no separate ID scheme.
Chapters are implicit positive integers, not entities. Timestamps come
free (volumen stamps momentum in SQL).

Event genera:

| genus | datum (JSON) | fold semantics |
|---|---|---|
| `liber-conditus` | `{titulus}` | new book, id = seq |
| `locus-positus` | `{liber, capitulum}` | sticky position; last one wins |
| `verbum-conditum` | `{liber, capitulum, lemma, formae, genus, significatio, exemplum?}` | new word, id = seq; all fields free text (notebook, not schema) |
| `verbum-emendatum` | `{verbum, lemma, formae, genus, significatio, exemplum?}` | full field replacement on word `verbum`; position (liber/capitulum) NOT editable in v1 — delete and re-add |
| `verbum-remotum` | `{verbum}` | word disappears from fold (acta history keeps it) |
| `responsum-datum` | `{verbum, directio, rectum}` | one actum PER CARD FLIP (quit mid-drill loses nothing); per-word tallies fold from these |

`directio` ∈ `"la-en"` (Latin→gloss) / `"en-la"`. `rectum` boolean.

Field meanings (all free text, entered at capture):
- `lemma` — dictionary headword (`canis`)
- `formae` — principal parts / genitive (`canis, canis`)
- `genus` — gender / part of speech, free (`m.`, `v. 3`, `adv.`)
- `significatio` — gloss (`dog`)
- `exemplum` — optional sentence from the book

## Architecture — C = event pipe, JS = brain

The mensa-proven split. C side is thin and generic; all fold, deck, and
UI logic in JS (a few thousand words folds instantly client-side).

**C (`fontes/vocabularium.c`):** piscina + fenestra + vitrea + capsula +
internuntius. Handlers (JSON-RPC over internuntius):
- `acta_legere {post_seq}` → array of `{seq, momentum, genus, datum}` via
  `volumen_acta_legere`
- `actum_appendere {genus, datum}` → `{seq}` via
  `volumen_actum_appendere`; 0 = vitium, error message surfaced from
  `volumen_error`

Main loop: `fenestra_expectare_eventus`, drain bridge TO EMPTY each tick
(`vitrea_obtinere_nuntium` — TRACTA STATIM contract), INTERITUS →
`vitrea_recargare`. Volumen opened with `volumen_aperire_aut_creare` at
launch; open failure = loud refusal, never silent create-elsewhere.

**JS:**
- `assets/logica.js` — PURE functions, no DOM, no bridge: fold(acta) →
  status; deck building per scope; shuffle (Fisher–Yates); recycle rule;
  infirma selection. This is the testable core.
- `assets/vocabularium.js` — bridge + DOM + views; on launch calls
  `acta_legere {post_seq: 0}`, folds, renders; every mutation =
  `actum_appendere` then applies the same event to the local fold (no
  re-read).
- `assets/index.html` — shell + view containers.

## UI — three keyboard-first views (Latin labels)

View switching: header buttons + `Cmd+1/2/3`.

**Captura (default).** Sticky position header `Liber • Capitulum N`
(click → picker: choose liber or create one by title, set chapter
number → emits `locus-positus`). Entry form: lemma / formae / genus /
significatio / exemplum. Tab between fields; Enter in any field commits
(lemma + significatio required, rest optional), emits `verbum-conditum`
at the current position, clears form, refocuses lemma. Below: this
chapter's entries, newest first.

**Index.** Books → chapters → words. Per row: edit (inline form →
`verbum-emendatum`) and delete (→ `verbum-remotum`, with confirm).
Counts per chapter.

**Exercitium.** Setup: scope (capitulum praesens / liber / omnia /
infirma) + direction toggle (default la-en). Deck = shuffled matching
words. Card front: la-en shows lemma alone; en-la shows significatio
alone. Back: all fields. Keys: Space = flip; after flip Y/→ = rectum,
N/← = pravum (each emits `responsum-datum` immediately); Esc = exit.
Pravum cards recycle to deck end until answered rectum once this
session. End screen: summa / recta prima vice / iterationes.

**Infirma definition (exact):** words whose MOST RECENT responsum (any
direction) has `rectum: false`. Never-drilled words are not infirma.

## Build — `aedificare.sh` = generated + hand-carried tiers

Scaffold via `silex novum` against the rhubarb fabrica (SILEX_FABRICA or
-f). Then hand-extend the generated `aedificare.sh`. Known tiers novum
cannot emit today — **each becomes a numbered nota/ictus on
01KZ6CSBQN**:

1. **Vendor tier** — `vendor/sqlite3.c` compiled with relaxed flags
   (-O2, no -Werror battery) + the 7 `SQLITE_*` defines, copied from
   rhubarb `compile_tests.sh`.
2. **Platform `.m` sources** — `lib/vitrea_macos.m`,
   `lib/fenestra_macos.m` (+ whatever the closure walk shows, e.g.
   clipboard), compiled as ObjC, linked with the needed frameworks
   (Cocoa, WebKit) — the aedilis convention silex's textual closure
   does not know.
3. **Capsula asset step** — vendor `capsula_generare`'s source into the
   scaffold (`instrumenta/`), build it FIRST in `aedificare.sh`, run it
   on `assets/vocabularium.toml` every build (CAPSULA ANTE OMNIA — the
   forum lesson: a stale capsula is a silent 'my edit did nothing').
4. **Anything else the walk reveals** — recorded as found.

Also carried from forum.sh: `rm` before `cp` when installing the binary
(macOS SIGKILLs an overwritten signed inode).

## Testing

- `probationes/probatio_vocabularium.c` (credo) — event round-trip
  against a temp volumen in build/: append each genus, read back, assert
  seq/genus/datum; the two handler paths exercised at the C level.
- `probationes/probatio_logicae.js` (osascript, DOM-free — exemplar
  probatio_fori.js, with its four known ways of lying green in mind) —
  fold from a scripted acta sequence (books, position, words, emendatum,
  remotum, responsa); deck per scope incl. infirma; recycle behavior;
  front/back content per direction.
- `probare.sh` runs both.

## Out of scope (v1, all on-pull later)

Spaced repetition scheduling; phone surface (vitrea servus); mensa
board; search; import/export; morphology validation; audio; stats
dashboards beyond the end-of-drill tally.

## Record-keeping

- Worklog inside the project: `vocabularium.worklog.md` (silicetum is
  outside git; the VCS volumen carries it).
- Silex gaps → tabularium (notas/ictus on 01KZ6CSBQN) as found, not
  batched at the end.
