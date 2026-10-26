# pensa K5 — Reminders edit view Implementation Plan

> **For agentic workers:** this repo's standing method (Fran): tasks are
> executed INLINE in the main session, one task per turn, Fran approves
> each before the next; no subagents. Steps use checkbox (`- [ ]`)
> syntax for tracking.

**Goal:** edit the checklist template inside the Reminders view: `e`
(or an Edit button) splits the view — the live read view on top, an
embedded scriba on the `pensa` book below — and parse results update
as you type.

**Architecture:** the same machinery as the day notes. A second
`Charta` (membrum__charta) in the calendar's figure context, mounted at
mount time on book key `pensa` (nested scriba `<id>_pensa`), composed
only in Reminders edit mode into a bottom-half slot `forma`. A status
field `monita_modus` (`lectio` | `editio`) holds the mode. Entering the
editor = click or Tab (dispatcher level rules, as for the notes);
leaving = scriba takes the first Esc, the second returns focus to the
list, a third Esc (list) leaves edit mode, another goes to the month.
While the editor has focus the calendar ignores keys. Typing reaches
the book through the existing gesture hand-off; the pane's pulse
refreshes both editors.

**Tech Stack:** C89 in Latin; `membrum__charta`, `scriba`, `pensa`,
credo v2.

**Spec:** `project-specs/pensa-spec.md` §V.2 (Edit view) and AUDIENDA
(exit key); decisions with Fran 2026-10-23: split view (read view on
top, editor below); focus by click/Tab like the day notes (`e` does
not jump into the editor — a dispatcher focus-request feature is a
later, separate task).

## Global Constraints

- Same C89/Latin rules as K1–K4; new lint words must be real Latin.
- No change outside the calendar (kalendarium.c/.h, its manifest,
  tests). The dispatcher is not touched.
- A calendar without a volume: no editor (Charta not mounted); `e`
  still switches the mode, the slot stays empty, nothing crashes.
- Day-notes behaviour and every existing test stay unchanged.
- Commit only through `silva.commissio`, only on Fran's word.

## Header text (approved with the plan)

`include/kalendarium.h` — no signature changes. After VISUS MONITORUM:

```c
 * EDITIO MONITORUM (pensa K5; campus monita_modus lectio|editio): e
 * aut globulus capitis "edere" ("Edit", solum in lectione) visum
 * dividit - supra lectio (ordines ut in K4, paginae dimidiae), infra
 * locus "forma" cum scriba nidificata "<id>_pensa" in libro "pensa"
 * (Charta, ut notae diei). In scribam ictu aut Tab; ibi claves scribae
 * sunt (calendarium nihil agit); Esc primum scribae, secundum ad
 * indicem, tertium (in indice) editionem claudit, deinde ad mensem.
 * Lectio formam in omni pictura legit: vitia et "next:" dum scribitur
 * mutantur. Visus mutatus (globulo) lectionem restituit.
```

`apps/calendarium/organum.stml` — new field:

```xml
    <campus nomen="monita_modus" typus="electio" vita="transiens"
            absens="lectio" scriptor="calendarium.agere"
            nota="visus monitorum (K5): lectio sola aut editio (scriba
                  libri pensa infra, lectio supra)">
      <optio>lectio</optio>
      <optio>editio</optio>
    </campus>
```

then `./tools/organa_infigere.sh`.

## Review Focus

1. **Keys typed in the editor must not run calendar commands** (`e`,
   `[`, Esc chain): pinned in Task 2.
2. **Leaving the view while editing** (bar button to Month and back):
   returns in read mode, no stray editor. Pinned in Task 1.
3. **No volume**: `e` does not crash; slot empty. Pinned in Task 1.
4. **Typing a broken heading** shows its error in the read view on the
   next frame (live). Pinned in Task 2.
5. **Day notes unaffected** (both editors in one context): the
   existing `notae` sections stay green; the key-ignore check covers
   both editors. Pinned by the existing suite + a Task 2 plant.

---

### Task 1: edit mode, split layout, editor mounted and composed

**Files:** manifest (+ organa_infixa), `include/kalendarium.h`,
`lib/kalendarium.c`, `probationes/probatio_kalendarium.c` (section
`monita_editio`).

- [ ] **Step 1: Failing test** (480x480, MONITA_EXEMPLI):
  - `e` -> `monita_modus` "editio"; components `forma` exists, its top
    is below the last visible list row; rows still start `"m h0"`.
  - nested scriba composed: a click inside `forma` (ictus_ad at
    `f.x + XL, f.y + XXX`) makes `motus->spatium_intimum` contain
    `"_pensa"`.
  - Esc (focus back) -> Esc on the list -> "lectio", `forma` gone;
    Esc -> visus "mensis".
  - Edit button: back to Reminders, click `#edere` -> "editio";
    click `visus-mensis` then `visus-monita` -> "lectio".
  - no volume (`mundum_struere(x, p, "monita")` without volume): `e`
    -> "editio", no crash, `forma` exists, no nested scriba.
  - empty book text: "No checklist yet. Press e to write one."
  Run — Expected: FAIL (no `e` command).

- [ ] **Step 2: Implementation**
  1. `KalendariumFigurae` gains `Charta pensorum; /* K5: editor libri
     "pensa" */`. `pensorum_montare(m, volumen, id, systemata)` beside
     `notas_montare`: id `"<id>_pensa"`, key `"pensa"`, size from the
     same slot arithmetic as the notes (`notarum_locus`); called right
     after `notas_montare` in `kalendarium_montare`.
  2. `editio_monitorum(m)`: visus monita AND `monita_modus` == editio.
  3. Commands: `{ "calendarium.monita_edere", "Edit checklist", "e",
     VISUS_MONITORUM, ZEPHYRUM, fac_monita_edere }` (writes editio);
     `{ "calendarium.monita_effugere", "Back", "Esc", VISUS_MONITORUM,
     ZEPHYRUM, fac_monita_effugere }` (editio -> lectio, else visus
     mensis); `ad_mensem` loses VISUS_MONITORUM (one Esc per view).
     `fac_visum_monitorum` writes visus monita + modus lectio (three
     fields in one write: `GrexCamporum`).
  4. GLOBULI header row `{ "edere", "Edit", VISUS_MONITORUM, ZEPHYRUM,
     FALSUM, "calendarium.monita_edere" x3, VERUM }`; placed by
     `indicem_disponere` only in monita lectio.
  5. Layout: `indicem_disponere` splits like the day view (PARS L top
     region + `forma` slot bottom, PARTES_NULLUM) when
     `editio_monitorum(m)`; `capacitas_indicis` halves the body in the
     same case.
  6. Composer: `pensorum_componere` (like `notas_componere`, slot
     `forma`) when `editio_monitorum(m)`; root figure draws the
     shadow (`charta_umbram_pingere`) for `forma` before the rows.
  7. `nullum` text: "No checklist yet. Press e to write one."
  8. Header text; manifest field; organa_infigere.
- [ ] **Step 3: Run** both calendar suites — PASS.
- [ ] **Step 4: Plants**: split without `editio` check (red: lectio
  shows forma); `fac_visum_monitorum` without the modus reset (red:
  back in editio); Esc command still `ad_mensem` in monita (red: Esc
  leaves to month while editing).

---

### Task 2: typing, key isolation, Tab/Esc, live re-parse

**Files:** `lib/kalendarium.c` (key ignore, gesture hand-off, pulse),
tests section `monita_scribere`.

- [ ] **Step 1: Failing test** (kalendarium suite, pattern of
  `_probare_notas`): MONITA_EXEMPLI; `kalendarium_gestum_ponere`;
  e; click in `forma`; in scriba normal mode `G` (last line), `o`
  (open a line), type `"every fortnight"`, Enter, `"  water plants"`,
  Esc - a heading WITH an indented child (a childless line would be an
  every-day item and never parsed); flush the gesture;
  `pensa_textum_legere` contains "every fortnight"; recompose: the new
  group's heading `h5` and its error row `v5` exist (live re-parse;
  "every fortnight" is not in the grammar). Keys ignored while focused: before Esc, `[`
  typed as text, `pagina` unchanged, `monita_modus` still editio.
  Second Esc: focus leaves (`_pensa` not in spatium_intimum), still
  editio. Tab -> focus back in `_pensa`; Tab -> out.
- [ ] **Step 2: Implementation**: (key ignore for the pensa editor
  moved to Task 1 - the Esc chain needs it; ruling in the ledger);
  `kalendarium_gestum_ponere`
  hands the gesture to BOTH chartas (only the focused one types);
  `kalendarium_reficere` refreshes both.
- [ ] **Step 3: Run**; **Step 4: Plants**: key ignore without
  pensorum (red: `[` turns the page / `e` re-enters); gesture to notes
  only (red: text never reaches the book); reficere without pensorum
  (only if a test observes it - else record green and drop).

---

### Task 3: vicus end-to-end, screenshot, docs, commit

- vicus section `monita_editio` (app 960): Reminders -> `#edere` ->
  click in `#forma` -> type -> Esc -> flush -> book has the text; the
  read rows show it. Plant: gesture hand-off to notes only.
- Screenshot (throwaway probe): split view with a broken heading being
  typed (error visible above the editor).
- Worklog; spec §VII K5 DONE, AUDIENDA exit key resolved (click/Tab in;
  scriba Esc, then list, then mode, then month); §VIII parked: focus
  request from actions (dispatcher), so `e` can land in the editor.
- Lint; commit on Fran's word.
