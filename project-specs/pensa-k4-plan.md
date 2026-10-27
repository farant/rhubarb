# pensa K4 — Reminders tab, read view Implementation Plan

> **For agentic workers:** this repo's standing method (Fran): tasks are
> executed INLINE in the main session, one task per turn, Fran approves
> each before the next; no subagents. Steps use checkbox (`- [ ]`)
> syntax for tracking.

**Goal:** a new calendar view, Reminders (`visus = monita`), after
Agenda: the daily checklist template rendered read-only — each schedule
heading as typed with a dim "today" / "next: Sat 24 Oct" note, its
items beneath, parse errors in place — paginated like the day and
agenda views.

**Architecture:** a fifth view bit `VISUS_MONITORUM`. The composer
gives it the existing index layout (`indicem_disponere`: header with
page arrows, album rows of the current page); `ordines_indicis` gains a
monita branch that turns the parsed form (`pensa_formam_legere` over
the `pensa` book) into album rows; a new painter `monita_pingere`
draws them. Rows are not selectable in K4 (nothing to act on until
K5's edit view). The bar gains a "Reminders" segment that becomes a
bell pictogram when the bar is too narrow for the word.

**Tech Stack:** C89 in Latin; `pensa`, `recursus`, `membrum__album`,
`figura`, credo v2.

**Spec:** `project-specs/pensa-spec.md` §V.2; decisions with Fran
2026-10-23 in chat: (1c) bar shows the word "Reminders" when it fits,
else a bell pictogram; (2) headingless every-day lines show under a DIM
"Every day" heading (not typed, so dim); (3) no ticks in the read view.

## Global Constraints

- Same C89/Latin rules as K1–K3: no Roman-numeral identifiers, `i32`
  unsigned (`s32` for -1), `chorda` not NUL-terminated.
- Text to `mandata_textus` is copied by mandata itself since d97e5865;
  no special care needed, but keep the house habit of building strings
  in `md->piscina`.
- Calendar without a volume: Reminders shows "No checklist yet." (no
  crash, no pensa calls).
- Every existing calendar test stays green unchanged except where the
  bar gains a segment (Agenda is no longer the LAST segment).
- New lint words must be real Latin (K3 lesson: `signabile` -> renamed).
- Commit only through `silva.commissio`, only on Fran's word.

## Header text (approved with the plan)

`include/kalendarium.h` — no signature changes. New paragraph after
INDEX COTIDIANUS:

```c
 * VISUS MONITORUM (pensa K4; visus="monita", post agendam): forma
 * indicis cotidiani (liber "pensa" voluminis) lectioni sola. Titulus
 * "Reminders"; caput cum sagittis paginae; ordines (album, paginae ut
 * in die et agenda): fascia "m" ("Daily checklist"), pro grege k
 * titulus "h<k>" (PARTES_INDEX; schedula ut scripta, aut "Every day"
 * obscurum pro lineis sine capite; ad dextrum obscure "today", "next:
 * Sat 24 Oct" (recursus_proximus) aut "no more dates"; in pagina nova
 * "h<k>-c"), vitium "v<k>" (schedula non lecta: nuntius recursus
 * involutus, colore erroris) et res "u<k>-<j>" (PARTES_ITEM, non
 * eligendae); sine forma "nullum" ("No checklist yet."). Claves: [ ]
 * paginae, Esc ad mensem. Globulus barrae "visus-monita": "Reminders"
 * si barra capit, aliter campana (III cellulae).
```

`apps/calendarium/organum.stml` — `visus` gains `<optio>monita</optio>`
(after agenda); `<cognomen nomen="monita" semita="calendarium/monita"/>`;
then `./tools/organa_infigere.sh`.

## Review Focus

1. **Narrow pane**: at 280 px the bar must not overflow — bell, 3
   columns. Pinned in Task 4.
2. **No volume / no book / empty book**: "No checklist yet.", no crash.
   Pinned in Task 2.
3. **Long error message**: wraps inside its row; the row's height is
   computed by the SAME wrap function the painter uses. Pinned in
   Task 3 (all wrapped lines inside the row's bounds).
4. **Group spilling across pages**: heading repeats as "h<k>-c";
   `[`/`]` turn pages without trying to select. Pinned in Task 2.
5. **One-off date in the past**: "no more dates", not a crash or a
   wrong "next". Pinned in Task 3.

---

### Task 1: the view itself (manifest, bar word, title, route, keys)

**Files:** `lib/kalendarium.c`, `include/kalendarium.h` (header text),
`apps/calendarium/organum.stml` (+ `lib/organa_infixa.c` regenerated),
`probationes/probatio_kalendarium.c` (section `monita_visus`).

- [ ] **Step 1: Failing test**

```c
/* K4: globulus Reminders visum monita aperit; titulus, locus, semita;
 * Esc ad mensem; argumentum "monita" (cognomen) */
interior vacuum
_probare_monita_visum (
    CredoContextus* c)
{
     Mundus* x;
    Mandata* md;
      Fines  f;

    x = (Mundus*)c->datum;
    thema_initiare();
    x->latitudo  = CDLXXX;
    x->altitudo  = CDLXXX;
    CREDO_NECESSE(agendam_parare(x, c));

    CREDO_GRADUS("globulus Reminders");
    CREDO_VERUM(ictus_in(x, "visus-monita", c->piscina));
    CREDO_VERUM(campus_est(x, "visus", "monita"));
    md = reddere(x, c->piscina);
    f  = fines_componentis(x, "titulus", c->piscina);
    CREDO_VERUM(textus_in_continet(md, f, "Reminders"));

    CREDO_GRADUS("locus et titulus loci");
    CREDO_CHORDA_AEQUALIS_LITERIS(kalendarium_locus(&x->m, c->piscina),
        "calendarium/monita");
    CREDO_CHORDA_AEQUALIS_LITERIS(kalendarium_locus_titulus(&x->m,
        c->piscina), "Reminders");

    CREDO_GRADUS("Esc ad mensem");
    effugium(x);
    CREDO_VERUM(campus_est(x, "visus", "mensis"));

    CREDO_GRADUS("argumentum monita");
    CREDO_VERUM(kalendarium_argumentum_ponere(&x->m, "monita"));
    CREDO_VERUM(campus_est(x, "visus", "monita"));
}
```

(`effugium` = existing Esc helper; if absent use
`clavis_specialis(x, CLAVIS_EFFUGIUM, XXVII, XXVII, ZEPHYRUM)`.)

Run — Expected: FAIL (no `visus-monita` component).

- [ ] **Step 2: Implementation** (`lib/kalendarium.c`)

1. `#define VISUS_MONITORUM  XVI` after VISUS_HEBDOMADIS; `visus_monitorum(m)`
   beside `visus_agendae`; `visus_currens` returns it first.
2. `VISUS_BARRA` gains `| VISUS_MONITORUM`; the four `calendarium.visus_*`
   rows gain `| VISUS_MONITORUM`; page commands and `ad_mensem` gain it.
3. Command `{ "calendarium.visus_monitorum", "Reminders", NIHIL,
   VISUS_MENSIS | VISUS_INDICIS | VISUS_HEBDOMADIS | VISUS_MONITORUM,
   ZEPHYRUM, fac_visum_monitorum }` (`visum_ponere(cm->m, "monita")`).
4. GLOBULI row after Agenda: `{ "visus-monita", "Reminders", VISUS_BARRA,
   VISUS_MONITORUM, VERUM, "calendarium.visus_monitorum" x3, FALSUM }`;
   page-arrow rows' `visus` gain `| VISUS_MONITORUM`.
5. `figura_globuli`: segment kind ULTIMUM for `VISUS_MONITORUM` (Agenda
   becomes MEDIUM).
6. Composer: `si (visus_agendae(m) || visus_diei(m) || visus_monitorum(m))
   indicem_disponere(...)`; in `indicem_disponere` the "nova" header
   button only when `visus_currens(m) & VISUS_INDICIS`.
7. `figura_tituli`: `si (visus_monitorum(...))` -> "Reminders" in
   FONS_TITULI (like the week title).
8. Root figure (`figura_tabulae`): `si (visus_monitorum(...))
   { monita_pingere(...); redde; }` — Task 1 adds `monita_pingere` as
   an empty function (body filled in Task 3).
9. `visus_loci`: add "monita"; `kalendarium_locus`: "calendarium/monita";
   `kalendarium_locus_titulus`: "Reminders".
10. Header text, manifest option + cognomen, organa_infigere.

- [ ] **Step 3: Run** both calendar suites — Expected: PASS. If an old
  test asserted Agenda's segment kind or the bar's exact width, update
  it (record the ruling).

- [ ] **Step 4: Plants**: drop the GLOBULI row (red: click);
  `visus_loci` without "monita" (red: locus).

---

### Task 2: rows (album) and paging

**Files:** `lib/kalendarium.c` (`ordines_indicis` monita branch, wrap
helper), `probationes/probatio_kalendarium.c` (fixture + sections
`monita_ordines`, `monita_absentia`, `monita_paginae`).

Fixture (today = Wed 14 Oct 2026):

```c
hic_manens constans character MONITA_EXEMPLI[] =
    "vitamins\nmorning offering\n\nwednesdays\n  mass\n\nweekends\n"
    "  mow the lawn\n\nevery other monday\n  call mom\n\n2026-10-01\n"
    "  pay rent";
```

Groups: 0 every day (implicit), 1 wednesdays, 2 weekends, 3 error, 4
once (past).

- [ ] **Step 1: Failing tests**
- `monita_ordines` (480x480): ids in tree order (helper `monita_esse`,
  prefixes `m`, `h`, `v`, `u`, `nullum`):
  `"m h0 u0-0 u0-1 h1 u1-0 h2 u2-0 h3 v3 u3-0 h4 u4-0"`.
- `monita_absentia`: no book -> `"m nullum"`; no volume (plain
  `mundum_struere` without volume) -> `"m nullum"`.
- `monita_paginae` (default 280x256): every-day group of 15 items
  (`"i0\ni1\n...i14"`): page 1 starts `"m h0 u0-0"`; `]` -> `pagina`
  "1", page 2 starts with `"h0-c"`; `[` back to "".

- [ ] **Step 2: Implementation**

```c
/* K4: textus in lineas columnarum n (ad spatia; verbum longius
 * frangitur); Xar de chorda (frusta textus ipsius) */
interior Xar*
lineas_involvere (chorda textus, s32 columnae, Piscina* p);
```

`ordines_indicis`: before the day branch,

```c
    si (visus_monitorum(m))
    {
        monita_in_album(m, l, p);
        redde l;
    }
```

`monita_in_album`: form from `pensa_textum_legere(ctx->volumen, ...)`
(no volume -> no form); `"m"` ALBUM_FASCIA II; no groups -> `"nullum"`
ALBUM_RES II res 0; per group k: `"h<k>"` ALBUM_TITULUS II; if
`vitium.mensura`: `"v<k>"` ALBUM_RES, altitudo = lines of
`lineas_involvere(vitium, columnae_textus(m))` + I, res 0; per item
`"u<k>-<j>"` ALBUM_RES II res 0. `columnae_textus(m)` = surface width
/ KALENDARIUM_LITTERA - II (margins) - IV (indent) — the painter uses
the SAME function.

- [ ] **Step 3: Run** — PASS. **Step 4: Plants**: skip the error row
  (red: ids); fascia as TITULUS (red: page 2 starts "m-c").

---

### Task 3: drawing + screenshot

**Files:** `lib/kalendarium.c` (`monita_pingere`), tests section
`monita_figurae`.

- [ ] **Step 1: Failing test** (480x480, MONITA_EXEMPLI):
- `"m"`: fascia_in + "Daily checklist".
- `"h0"`: "Every day" in COLOR_TEXT_DIM; "today" in the row.
- `"h1"`: "wednesdays" COLOR_TEXT; "today".
- `"h2"`: "weekends"; "next: Sat 17 Oct".
- `"h3"`: no note; `"v3"`: text contains "from which Monday", colour
  COLOR_ERROR, every text command of the error inside `v3`'s bounds.
- `"h4"`: "no more dates".
- `"u2-0"`: "mow the lawn" COLOR_TEXT.
- no book: `"nullum"` contains "No checklist yet.".

- [ ] **Step 2: Implementation** — `monita_pingere(contextus, md, c)`
  loops the children: band (figura_fasciam_pingere + bold "Daily
  checklist"); heading (bold text at x+VI; note right-aligned dim,
  computed: recursus NIHIL -> none; `recursus_continet(r, hodie)` ->
  "today"; `recursus_proximus(r, hodie + 1, &d)` -> "next: " + "Sat 17
  Oct" (fasti_nomen_diei_hebdomadis / fasti_nomen_mensis abbreviated);
  else "no more dates"; heading text truncated before the note);
  error (lines of `lineas_involvere`, one per KALENDARIUM_LINEA, x+XXII,
  COLOR_ERROR); item (x+XXII, COLOR_TEXT, separator line below like
  agenda rows); `"nullum"` dim text. Repeated heading `"h<k>-c"` draws
  as its heading.

- [ ] **Step 3: Run** — PASS. **Step 4: Plants**: drop the
  `recursus_continet` -> "today" branch (red: h1 shows "next: Wed 21
  Oct"); error colour COLOR_TEXT (red: v3 colour); implicit heading in
  COLOR_TEXT (red: h0 dim).
- [ ] **Step 5: Screenshot** (throwaway probe, vicus 960x600, fixed
  clock): Reminders with MONITA_EXEMPLI; Read the PNG; show Fran.

---

### Task 4: bar — word or bell

**Files:** `lib/kalendarium.c` (`globulos_disponere`, `figura_globuli`,
`campana` pictogram), tests section `monita_globulus`.

- [ ] **Step 1: Failing test**: at 480 px `visus-monita` is
  `(strlen("Reminders") + II) * KALENDARIUM_LITTERA` wide and contains
  "Reminders"; at 280 px (default) it is `III * KALENDARIUM_LITTERA`
  wide, contains no text, and has pictogram rectangles inside; and the
  bar's rightmost button ends inside the surface (no overflow).
- [ ] **Step 2**: (the width rule moved to Task 1 - ruling: the word
  alone overflowed narrow panes and broke old layout tests;
  `barra_verbum_capit` sums the visible bar buttons' word widths + left
  gaps against the bar's columns and gives `visus-monita` width III
  with empty title when they do not fit.) `figura_globuli`: segment
  with empty text, then `campanam_pingere` (pattern like `placenta`,
  centred).
- [ ] **Step 3: Run**; **Step 4: Plant**: always the word (red: 280
  width).

---

### Task 5: vicus end-to-end, worklog, spec, commit

- vicus section `monita`: pane on 2026-10, book "vitamins\n\nweekends\n
  mow the lawn", click `#visus-monita` -> visus monita, `#h1` exists;
  plant: GLOBULI row dropped.
- Worklog (`lib/kalendarium.worklog.md`): view bit, rows, wrap helper
  shared by album and painter, bell rule, why no selection.
- Spec §VII K4 DONE; lint; commit on Fran's word.
