# psalterium Plan — the shared chant view, the embedded psalter, the `$psalterium` pane

**Ledger:** parcum "psalmodia" (01M6V5H5V1EGK1V37TP5M6BFN1); one opus
per task, filed at T1.

**STATUS: DRAFT 2026-11-14** — awaiting Fran's review.

> **For agentic workers:** REQUIRED SUB-SKILL: Use
> superpowers:executing-plans (inline, per house convention, no
> subagents) to implement this plan task by task, ONE TASK PER TURN
> with Fran's approval. Every task OPENS with a measurement that may
> re-slice it; a re-slice is shown to Fran before work continues.
> Steps use checkbox (`- [ ]`) syntax. Each task ends with an
> "Executed" note.

**Goal:** `$psalterium` opens a vicus pane listing the 150 psalms;
Enter shows the chosen psalm in square notation on a psalm tone
(default 8 G), `t`/`T` change the tone, `[`/`]` step psalms, Space
sings it.

**Spec:** `project-specs/psalterium-spec.md` (D1-D8, §II shared view,
§III pane, §IV embedded texts, §V tests, §VI-VII non/anti-goals).

**Architecture.**
- `gabc_visus` (new, from `lib/antiphonale.c`): one chant shown from
  gabc text in memory — version swap, layout, paging, zoom, syllable
  selection, play-along, drawing, footer, banner. Each pane keeps its
  OWN key table and click dispatch and calls `gabc_visus_*` actions,
  so the antiphonale's command names and node ids stay byte-identical.
- `antiphonale` shrinks to the file shell: path, watcher, reading,
  "missing:"/"empty file" banners.
- `psalterium_infixum` (GENERATUM): the 150 texts + LICENTIA in the
  binary.
- `psalterium` (new pane): index (album rows) and psalm view (fitter →
  gabc → `gabc_visus`), registered in `lib/vicus_applicatio.c` beside
  the antiphonale.

## Global Constraints

- C89 with latina.h; Latin identifiers, comments, messages; latina.h
  macros and Roman numerals are forbidden identifiers.
- `chorda` NOT NUL-terminated; `i32`/`i64` UNSIGNED — "none" is `s32`
  -1 or NIHIL.
- **G18 kept:** the antiphonale's live-edit swap keeps playback at the
  same pulse; the psalter's tone/psalm change STOPS it (spec III.2).
- **`probatio_antiphonale_latus` passes unchanged**, and
  `AntiphonaleMontatio` / `AntiphonalePorta` keep their public shape
  (spec II guard). A needed test edit is a finding for Fran.
- No temporary files; no `musica/psalmi/` read at runtime; pages,
  never scrolling (spec VII).
- Every key action has a mouse path (house rule).
- State only through `apps/psalterium/organum.stml` fields; texts and
  fitted chant live in mount memory.
- New C files through `./silva/scribe.sh`; aedilis tag in each header;
  `git add -N` new files, then `./tools/compile_tests_fontes_generare.sh`.
- Every new test section born red by a plant that COMPILES
  (`silva.planta`), recorded in the Executed note and the worklog.
- Commit through `silva.commissio` with the gates psalmodia plan-2
  used plus GENERATUM; tree frozen while gates run; touched vicus
  source → rebuild the tool before commissio.
- **Headers reviewed by Fran before implementation:** T1 and T3 STOP.

## Review Focus

Five conditions the spec implies but does not test; each pinned by a
test in the named task.

1. **Bad stored state** — a route or saved state with `psalmus` 0,
   200 or absent while `visus=psalmus`, or a `signum` the table no
   longer has: index (or 8 G), no crash, no banner. (T6
   `_status_malus_probare`.)
2. **Tiny pane** — height for one row or less, width too narrow for a
   staff: index still pages (capacity ≥ 1 row), psalm view shows the
   existing "pane too narrow" text. (T5 `_index_probare`, T6.)
3. **Typed numbers** — "0", "151", "1 5 1", a digit then a pause and
   another, Backspace: never selects outside 1-150; a number that
   would exceed 150 restarts from the new digit. (T5
   `_numeri_probare`.)
4. **Leaving while singing** — Esc to the index while playing stops
   the sound (no invisible playback without a stop key); `]` while
   playing stops it. (T6 `_sonus_probare`.)
5. **Memory across re-fits** — 200 tone changes and 200 psalm steps:
   `piscina_summa_usus` of the mount arena flat after the first
   cycle (each fit in its own arena, old one destroyed after the new
   is built). (T6 `_memoria_probare`.)

---

## Task T1: The header `gabc_visus.h` (STOP: Fran reviews)

**Files:** Create `include/gabc_visus.h`, `lib/gabc_visus.worklog.md`.

**Interfaces:** Produces (exact names; T2 implements):
- `GabcVisusPorta` — the four sound slots + ctx moved verbatim from
  `AntiphonalePorta`; `antiphonale.h` keeps
  `nomen GabcVisusPorta AntiphonalePorta;`.
- `GabcVisus` (opaque) created by
  `GabcVisus* gabc_visus_creare(Piscina*, InsulaRamus* ramus,
  constans character* actio, constans character* radix_id,
  constans GabcVisusPorta* porta, i32 latitudo, i32 altitudo,
  i32 margo_supra)` — `actio` is the scriptor for the ramus fields
  `pagina` and `spatium` and the action of its footer nodes;
  `radix_id` is the root node whose clicks select syllables;
  `margo_supra` = pixels the shell reserves above the page (0 for the
  antiphonale).
- `b32 gabc_visus_textum_ponere(GabcVisus*, chorda textus,
  b32 continuare)` — parse; good → new version (continuare VERUM =
  G18 carry-over, FALSUM = stop); identical text → nothing; bad →
  last good kept + banner "linea:col: causa". VERUM if anything to
  redraw.
- `vacuum gabc_visus_vacuare(GabcVisus*)` — drop version, stop sound,
  selection -1.
- `vacuum gabc_visus_vexillum_ponere(GabcVisus*, constans character*)`
  ("" clears) and `constans character* gabc_visus_vexillum(...)`.
- Actions (each returns b32 "consumed"): `gabc_visus_canere`,
  `gabc_visus_syllabam_movere(v, s32 directio)`,
  `gabc_visus_paginam_movere(v, s32 gradus)`,
  `gabc_visus_spatium_movere(v, s32 gradus)`,
  `gabc_visus_paginam_ponere(v, s32 pagina)`, `gabc_visus_sistere`.
- `b32 gabc_visus_premere(GabcVisus*, Componens* nodus,
  constans Eventus* ev)` — FALSUM if the node is not one of its own.
- `vacuum gabc_visus_componere(GabcVisus*, Componens* radix,
  Piscina*, InternamentumChorda*)` — adds `sagitta_prior`,
  `sagitta_proxima`, `globulus_canere`.
- `vacuum gabc_visus_pingere(GabcVisus*, Mandata*, Fines totum,
  b32 focatum, chorda titulus, constans character* nuntius_vacuus)`.
- `b32 gabc_visus_pulsare(GabcVisus*)`, `Mora gabc_visus_mora(...)`
  (`MORA_SONANS` while playing, else -1), `b32 gabc_visus_sonat(...)`,
  `chorda gabc_visus_titulus(...)` (gabc `name:`, empty if none).

- [ ] **Step 1: Measure.** List every function and static in
  `lib/antiphonale.c` and mark it VIEW or FILE; anything unclear goes
  in the worklog as a question. Confirm no other file includes
  internals (`bin/natura quaere antiphonale`).
- [ ] **Step 2: Draft the header** — head comment: what moved and why
  (spec D4), G18 vs `continuare`, the ramus fields it owns, USUS for
  both shells.
- [ ] **Step 3:** `./silva/examen.sh`, `./silva/formator.sh -vitia`,
  `./oratio/vocabula.sh -nova`; header compiles alone.
- [ ] **Step 4: STOP — show Fran the header.** Commit after approval.

---

## Task T2: Extract `gabc_visus`; antiphonale becomes the file shell

**Files:** Create `lib/gabc_visus.c`, `probationes/probatio_gabc_visus.c`;
modify `lib/antiphonale.c`, `include/antiphonale.h`.

**Interfaces:** Consumes T1. Produces nothing new.

- [ ] **Step 1: Measure.** Run `./compile_tests.sh antiphonale` on the
  untouched tree; record the pass count (the guard number).
- [ ] **Step 2: Write the failing tests** in `probatio_gabc_visus.c`:
  `_textus_probare` — `gabc_visus_textum_ponere` with
  `"name: A;\n%%\n(c4) A(g)men.(h) (::)"` → title "A", at least one
  page, no banner; same text again → FALSUM;
  `"%%\n(c4) A(g"` → banner starting with a line:col, version kept
  (title still "A"). `_continuare_probare` — with a fake porta
  playing at pulse 100: new text + continuare VERUM → porta.incipere
  called with pulse 100; new text + continuare FALSUM → porta.sistere
  called, `gabc_visus_sonat` FALSUM.
- [ ] **Step 3: Run** `./compile_tests.sh gabc_visus` → FAIL (no
  `lib/gabc_visus.c`).
- [ ] **Step 4: Implement** — move the VIEW code; `antiphonale.c`
  keeps FILE code and its own `TABULA_PRAECEPTORUM` (same names,
  keys, titles) whose functions call `gabc_visus_*`; its `relegere`
  calls `gabc_visus_textum_ponere(v, textus, VERUM)`.
- [ ] **Step 5: Run** `./compile_tests.sh gabc_visus` and
  `./compile_tests.sh antiphonale` → PASS, antiphonale count = Step 1
  number, `git diff --stat probationes/probatio_antiphonale_latus.c`
  empty. Plant: `continuare` ignored (always carries over) →
  `_continuare_probare` red. Restore.
- [ ] **Step 6:** rebuild vicus, open a `.gabc` with `$antiphonale`,
  edit it live, play — by eye/ear once; worklog. Commit.

---

## Task T3: Headers `psalterium.h`, `psalterium_infixum.h`, the manifest (STOP: Fran reviews)

**Files:** Create `include/psalterium.h`, `include/psalterium_infixum.h`,
`apps/psalterium/organum.stml`, `lib/psalterium.worklog.md`.

**Interfaces:** Produces (T4-T7 implement):
- `chorda psalterium_infixum_psalmus(i32 numerus, Piscina*)` — copy of
  psalm 1-150's file text; mensura 0 outside 1-150.
  `chorda psalterium_infixum_licentia(Piscina*)`.
- Pure helpers in `psalterium.h`:
  `chorda psalterium_incipit(chorda textus, Piscina*)` — first verse
  up to the first `*` or `†`, trailing `,:;` and spaces removed;
  `i32 psalterium_signa_numerus(constans PsalmodiaTabula*)`;
  `chorda psalterium_signum(constans PsalmodiaTabula*, i32 index,
  Piscina*)` — the D8 cycle's label ("8 G", "peregrinus");
  `s32 psalterium_signi_index(constans PsalmodiaTabula*, chorda
  signum)` (-1 unknown);
  `b32 psalterium_argumentum_legere(chorda argumentum, Piscina*
  scheda, s32* psalmus, chorda* signum, ChordaAedificator* causa)` —
  argument forms "" (index: psalmus -1), "109", "109,8 G"; errors per
  spec III.3 with the exact texts there.
- `PsalteriumMontatio` mirroring `AntiphonaleMontatio` (porta =
  `GabcVisusPorta`), and `psalterium_montare`, `_componere`,
  `_argumentum_ponere`, `_pulsare`, `_mora`, `_titulus` with the
  antiphonale's signatures.
- Manifest fields exactly spec III.4 — **one ruling:** no index page
  field; the index page follows `electus` (as lector's library does),
  `pagina` belongs to the psalm view (`gabc_visus` writes it).
  Cognomina `psalterium/index`, `psalterium/psalmus/{}`.

- [ ] **Step 1: Measure.** How lector's `visus` + `<cognomina>` restore
  a route (trace one restore in `lib/semita.c`) — closes spec AUDIENDA
  1; record in the worklog.
- [ ] **Step 2: Draft** the two headers and the manifest; header
  comment of `psalterium.h` states keys and mouse paths of both views
  (index: Up/Down j/k, PageUp/PageDown, Enter, digits, Esc closes;
  psalm: the view's keys + `t`/`T`, `[`/`]`, Esc; a header line above
  the page with clickable "≡", "‹", "›", tone label; footer title
  "Psalmus 109 · 8 G").
- [ ] **Step 3:** examen, formator, vocabula; `./tools/organa_infigere.sh`
  accepts the manifest.
- [ ] **Step 4: STOP — show Fran.** Commit after approval.

---

## Task T4: The embedded psalter; the Gloria fit

**Files:** Create `tools/psalterium_infigere.sh`,
`lib/psalterium_infixum.c` (GENERATUM); modify `aedificatio.stml`
(generator action beside `toni_psalmorum_infixum`),
`probationes/probatio_psalmodia.c`.

**Interfaces:** Consumes T3 `psalterium_infixum_*`.

- [ ] **Step 1: Measure.** Size of the generated C file and the vicus
  link time before/after (closes AUDIENDA 2).
- [ ] **Step 2: Write the failing tests** in `probatio_psalmodia.c`:
  `_infixum_probare` — for N in 1..150 the embedded text equals
  `musica/psalmi/N.txt` byte for byte; N 0 and 151 → mensura 0;
  licence starts "MIT License". `_gloria_probare` — every psalm ×
  every D8 ending fitted with `sine_gloria` FALSUM: 0 warnings total.
- [ ] **Step 3: Run** `./compile_tests.sh psalmodia` → FAIL.
- [ ] **Step 4: Implement** — generator as `toni_psalmorum_infigere.sh`
  (one byte array + offset table of 151 entries); register the action.
- [ ] **Step 5: Run** → PASS. Plant: one byte of 104.txt changed in
  the generated copy → `_infixum_probare` red. Restore; GENERATUM
  gate green. Commit.

---

## Task T5: Pure helpers and the index view

**Files:** Create `lib/psalterium.c`, `probationes/probatio_psalterium_latus.c`.

**Interfaces:** Consumes T2 `gabc_visus_*` (not yet used for drawing
in this task), T3, T4.

- [ ] **Step 1: Write the failing tests:** `_incipit_probare` —
  Ps 109 → "Dixit Dóminus Dómino meo"; Ps 1 stops before "†".
  `_signa_probare` — count 33; index of "8 G" ≥ 0; label(index("8
  G")) == "8 G"; every label resolves via `psalmodia_signum_invenire`.
  `_argumentum_probare` — every row of spec III.3 (exact error text).
  `_index_probare` — mounted at default size: 150 rows, row for 109
  reads "109  Dixit Dóminus Dómino meo"; PageDown moves `electus` by
  one page; at height 20 px capacity is 1 row and paging still works.
  `_numeri_probare` — Review Focus 3.
- [ ] **Step 2: Run** `./compile_tests.sh psalterium` → FAIL.
- [ ] **Step 3: Implement** helpers, mount, index componere/figura
  (album rows `r<n>` clickable, footer arrows, "Divinum Officium · MIT"
  dim in the footer), index key table.
- [ ] **Step 4: Run** → PASS. Plant: incipit cuts only at `*` →
  `_incipit_probare` red. Restore. Commit.

---

## Task T6: The psalm view

**Files:** Modify `lib/psalterium.c`, `probationes/probatio_psalterium_latus.c`.

**Interfaces:** Consumes T2, T5.

- [ ] **Step 1: Write the failing tests:** `_psalmus_probare` — Enter
  on 109 → `visus=psalmus`, `psalmus=109`, fitted text has
  `name: Psalmus 109;` and `annotation: 8 G;`; `t` → the next label
  in table order, `T` back, wrap at both ends; `]` on 150 and `[` on
  1 change nothing; tone change keeps `pagina` (clamped), psalm change
  sets 0; Esc → index with `electus` = 109. `_sonus_probare` — fake
  porta: Space plays; `t`, `]` and Esc each stop it (Review Focus 4).
  `_status_malus_probare` — Review Focus 1. `_memoria_probare` —
  Review Focus 5. `_semita_probare` — store the route of Ps 109 on
  2 A, remount from it: same psalm, same tone. Clicks on "≡", "‹", "›"
  and the tone label do what their keys do.
- [ ] **Step 2: Run** → FAIL.
- [ ] **Step 3: Implement** — fit in a fresh arena per change
  (`psalmodia_aptare` with `titulus` "Psalmus N", Gloria on), write
  gabc, `gabc_visus_textum_ponere(v, gabc, FALSUM)`, destroy the old
  fit arena after; first fitter warning → `gabc_visus_vexillum_ponere`.
- [ ] **Step 4: Run** → PASS. Plant: tone change passes `continuare`
  VERUM → `_sonus_probare` red. Restore. Commit.

---

## Task T7: `$psalterium` in vicus; fumus

**Files:** Modify `lib/vicus_applicatio.c`; regenerate
`lib/organa_infixa.c`; modify `tools/auditio_fumus.sh` only if a CLI
check is wanted (not planned).

**Interfaces:** Consumes T3/T5/T6 pane functions.

- [ ] **Step 1: Write the failing test** — in the vicus test that
  covers `$antiphonale` (find it: `bin/natura quaere antiphonale_iussum`),
  add `$psalterium(109, 8 G)` → a psalterium pane in front on 109 / 8
  G; `$psalterium(151)` → error text of III.3; a second
  `$psalterium(110)` reuses the pane.
- [ ] **Step 2: Run** → FAIL.
- [ ] **Step 3: Implement** — `vicus_genus_addere("psalterium", ...)`
  with the antiphonale's porta pattern (`PortaAntiphonae`, one singer);
  `iussum_registrare("psalterium", ...)` whose handler validates with
  `psalterium_argumentum_legere`, joins arguments as "109,8 G" and
  calls `vicus_acervo_mutare`.
- [ ] **Step 4: Run** → PASS; plant (handler ignores the second
  argument) → red; restore.
- [ ] **Step 5: Fumus** — rebuild vicus, `-imago` screenshots of the
  index and of Ps 109 on 8 G, look at both; listen once. Worklog.
  Commit.

---

## Executed notes

**T4 (2026-11-14).** Generator + GENERATUM `lib/psalterium_infixum.c`
(1.65 MB; offsets `_initia[151]`), action `psalterium_infixum` (psalms
as one `plagulae` input). Ruling: `_gloria_probare` already existed,
the new section is `_psalterium_gloria_probare` - cost if wrong: a
rename. Measurements closed spec AUDIENDA 2 (link 0.12 s unchanged,
binary +265 KB). Red by link failure, then three plants red (Ps 104
byte; Gloria unaccented; Gloria omitted), restored green. Gloria fit:
0 warnings over 150 x 33 on first run.

**T5 (2026-11-14).** `lib/psalterium.c` (helpers, mount, index view)
and `probationes/probatio_psalterium_latus.c` (5 sections, 92 checks).
Red at link before the lib; all green on first run, so five plants,
one per section, each red there (incipit at '*' only, the planned one).
Rulings: digits/Backspace handled before the key table, not as ten
table rows - cost if wrong: they don't show in a command list; any
movement clears typed digits - cost: none known; Enter/click and
`psalterium_argumentum_ponere` write the route in T5, the fit is T6
(`psalterium_semitam_secuta` stub) - cost: T6 fills the stub;
`$psalterium(, 8 G)` (tone without number) = "psalm number 1-150"
error - cost if wrong: one branch.

**T6 (2026-11-14).** Psalm view in `lib/psalterium.c` (fit per change
in its own arena, gabc to the view with `continuare` FALSUM, header
"≡ ‹ › 8 G", footer "Psalmus N · tone"); test file +5 sections (197
checks, 10/10). Red before (4 sections; memory trivially green), the
plan's plant (continuare VERUM) red in `_sonus_probare`, six more
plants red. Rulings: `lib/gabc_visus.c` touched (outside the task's
files) - `numerum_scribere` copied the scriptor into the mount arena on
every page write, which Review Focus 5 forbids; cost if wrong: none
for the antiphonale (its suites green). A fitter warning's banner
covers the header text, the header nodes still answer clicks - cost
if wrong: no header while a warning shows (none today). The same
(psalm, tone) is not re-fitted, so a restore keeps playing - cost if
wrong: a stale fit if the embedded texts changed at runtime
(impossible).

**T7 (2026-11-14).** `$psalterium` registered in `lib/vicus_applicatio.c`
(genus with routes, shared singer, `semitam_secuta`); new
`probationes/probatio_vicus_psalterium.c` (command, reuse, both
errors, index; screenshots). Rulings: a NEW vicus test file instead
of adding to `probatio_vicus_antiphonale.c` (kept untouched as the T2
guard) - cost if wrong: one more file; glue written before its test,
red shown by two plants (second argument ignored - the plan's - and
command unregistered) - cost: the order slipped once; screenshots
from a headless test frame, not `vicus -fumus -imago` (no way to run
a command there) - cost if wrong: none seen, same drawing path;
header psalm arrows "«" "»" instead of "‹" "›" (font lacks them) -
cost: a glyph choice for Fran. Listening: not done by Claude (no
ears) - offered to Fran.

**Final review (2026-11-14).** One fresh reviewer (Opus) over
0c6c96e84..951671415: "with fixes", no Critical. Fixed in one pass,
each test red first, then green; plants restored:
- Final: fixed Important 1 (a tone given with `$psalterium(N, tone)`
  came back on every restart: the argument was the pane's identity,
  re-applied at mount after the user's later tone) -
  `f->argumentum_transiens = VERUM`; vicus test
  `_restitutio_probare` (restart on the same volume keeps the chosen
  tone) RED->GREEN; plant (FALSUM) red.
- Final: fixed Important 2 (the "can't play" / muted banner hid the
  psalm header) - gabc_visus draws the banner at `y_paginae`, below
  the wrapper's margin (antiphonale margin 0: unchanged), header no
  longer suppressed; `_recensio_probare` RED->GREEN; plant red.
- Final: fixed Minor 1, regraded Important (spec III.2 "page 0 for a
  new psalm"): history to another psalm and Esc-then-`$psalterium`
  kept the old page - `semitam_secuta` clears `pagina` when the psalm
  differs from the fitted one, Esc clears it; RED->GREEN; two plants
  red.
- Final: fixed Minor 3, regraded Important (Review Focus 2's psalm
  half was promised in T6, untested): narrow psalm view shows "pane
  too narrow"; pins existing behaviour, plant on the message red.
- Final: minor (deferred): Review Focus 5's test measures the mount
  arena only; a leaked fit or version arena (dynamic, outside it)
  would pass. Code destroys both.
- Final: minor (deferred): `_aptare` records the fitted pair even if
  the view rejected the gabc (unreachable today).
- Final: minor (deferred): Backspace in the index with no digits is
  consumed (harmless unless vicus binds it).
- Final: Ruling: declined-to-judge items stand as recorded by the
  reviewer - sound on after a non-Esc close, per-mount arenas never
  freed, stale playing icon after another pane takes the singer (all
  house-wide / inherited from the antiphonale, no unmount hook);
  height-20 psalm view pages overflowing systems (gabc_compositio,
  shared); no digit timeout and Esc-with-digits closes (spec); extreme
  narrow overflow (cosmetic); insula arena growth (vicus-wide);
  harness quirk (ledger note) - cost if wrong: each is a separate
  follow-up, none a regression from this branch.
