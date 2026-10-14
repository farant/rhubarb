# scriba_actiones worklog

## 2026-10-05 — S1c: the page's key action

One action, `pagina.clavis`: committed text and named keys -> vim on
the WORKING sheet (the app gesture in Motus, S1a); vim state between
keys in the ephemeral insula; the document only through the gesture
flush (scriba_documentum / historia). Design agreed with Fran before
code (plan S1c design).

**Input - one letter, not two.** Both real sources send a key-down AND
a committed text event (same timestamp) for a printable key (fenestra
`_clavem_impellere`; rivus likewise). Printables come ONLY from
`EVENTUS_TEXTUS` (committed); named keys (Esc Enter Tab Backspace
Delete arrows Home End) and Ctrl-R from key-downs; printable key-downs
are ignored. The legacy `pagina.c` reads typed characters from the
key's `producta`, which since eventus S3a is set only under modifiers -
it predates that change. `manus_ludus_scribere` (new) sends both, so
tests meet the real double-input.

**Saving (Fran).** Normal/visual: a change is complete when its key is,
flushed at once. Insert: only after `SCRIBA_QUIES_MS` (~1 s) without a
key - the dispatcher's quiet flush. Esc flushes and closes the insert.
First chunk of an insert = plain act, later chunks joined (historia
S1b) - one `u` undoes the whole insert. `u` (normal, no pending key)
and Ctrl-R go to historia; the working sheet is then re-copied from the
projection. `fd` uses `Eventus.tempus`.

**Findings while testing:**
- *This house vim is not vim.* No `y` (yank) - `dd` fills the
  clipboard; `g` alone = top, `G` = end of sheet; Esc does not move
  the cursor back (2D sheet). My first test assumed `yy`, `gg`, col 4.
- *Entering insert mode sets the line's sticky indentation* (-1 -> 0):
  real state, a legitimate act even when the text is unchanged.
- *'\0' from pull-left.* `tabula_trahere_sinistram` writes '\0' into
  the last column (every other path writes ' '). Measured (`ifd`, with
  and without): without normalisation the act carries `textus="foo
  ...\0"` and the sheet holds one NUL. The action now turns '\0' into
  ' ' after every key (`albare`) - the same emptiness for tabula and
  vim, no invisible bytes in documents (decided here; veto-able).
- *Empty attribute values* are indistinguishable from absent through
  `insula_attributum` (STML `a=""` has a NIHIL value) - but the pretty
  writer prints `a=""` as bare `a`, re-read as "true". Never written:
  `textum_ponere` REMOVES on empty. The plant writing "" survived an
  `insula_attributum` check; the test now searches the WRITTEN insula.

Tests (probatio_scriba_actiones, through a real dispatcher with
scriba's canons and owners): 7 sections. Plants, all compiling and
caught: printable key-downs handled (letters doubled); no debounce;
never joined; empty values written; fd on time 0; no '\0' whitening.

v1 limits: bytes >= 0x80 in text are ignored (byte sheet); `capsa`
lines split on '\n' (a cell holding '\n' would split - vim never
writes one); per-key strings allocate from the document's arena
(as pictor does).

## 2026-10-05 — scriba on a branch (insula-rami-plan R4)

`ScribaActiones.ramus` and `ScribaCompositio.ramus` (unset = the given
repo's root; `scriba_actiones_initiare` zeroes, the one hand-built
composition in tests now `memset`s - the R3 lesson). Every helper in
this file takes `constans InsulaRamus*`; the action resolves its branch
once; the gesture flush reaches the repo through `ramus->repo` (the
effusor keeps the framework's fixed signature). Canon roots renamed
`scriba` (+ `id`). Standalone unchanged: root 203/203, ludus_tessera
6/6, the scriba tests also under aedilis's build.

Proof (probatio_scriba_ramus): two scribae mounted in one store, real
canons and owners on both, documents in ONE volume (namespaces s1/s2,
R2), a dispatcher and a hand each. Typing in one changes only its
branch, its document and its tree; undo is independent; the root holds
no scriba state. Plants: action ignoring the branch; canon root back to
`ephemera`; componere ignoring the branch - the last SURVIVED until the
test looked at the composed tree (cursor point, status line).

**Finding (open question for Fran):** the undo position is not
persisted. `historia_aperire` reopens at the END of the log (cursor =
finis); undo only moves an in-memory cursor (historia's header says so;
pictor has always done this). So in scriba: undo, quit, reopen - the
undone text is back. The test asserts today's behaviour with a note.

## 2026-10-08 - S2b-3: a view on a shared page, and when it is stale

A scriba view now edits a page document that other views share. The
working sheet (`laboris`) is the view's private copy; it is stale when
the document's history moved without it. `cursor_laboris` records the
history cursor at the moment laboris == projection: set by
`laboris_reficere` and ALSO right after this view's own commit
(`gestum_effundere`). The second one matters: an idle commit
(SCRIBA_QUIES_MS) fires mid-typing; without updating cursor_laboris the
next pulse sees "document moved", re-copies the projection over the
sheet and erases the letters typed since the commit. A plant that
dropped that line first SURVIVED (Esc-terminated typing hides it);
the test now types, pulses the dispatcher past the idle time, types
more, pulses vicus, and checks the later letter survived.
Page navigation (Ctrl+Shift+Left/Right) lives in the key handler next
to Ctrl-R because cursor/mode are owned by pagina.clavis; it flushes
the gesture first, and resizes laboris only if the new page's
dimensions differ.

## 2026-10-08 - S3a: a click places the cursor

`pagina.clavis` now also takes EVENTUS_MUS_DEPRESSUS (left button):
the cell comes from `destinatio_ad_locale(nodus, screen point)` divided
by the cell size (node width / sheet width - the handler does not know
CELLULA_LATITUDO). Like vim, a click ends the insert run for undo: the
gesture is flushed and `insertio_commissa` reset, so the next typed
text is a NEW undo unit (plant caught: without the reset `u` removed
the text before the click too). Insert mode is kept; visual returns to
normal with the selection cleared. House vim has only line-visual
(`V`); `v` does nothing - a test that used `v` failed for that reason.

## 2026-10-08 - Ctrl-[ = Esc (Fran)

`fd` already left insert mode (lib/vim.c, 300 ms; tested in
probatio_scriba_actiones V). Ctrl-[ is new: with Ctrl held fenestra
sends no text event, only DEPRESSUS with `runa = '['` (key without
modifiers) and `producta = 0x1B` (what the key produces). The handler
accepts either, so a layout where `[` sits on another key still works;
it maps to VIM_CLAVIS_ESCAPE through `clavem_tractare` (flush and all).
Three plants caught (whole branch, each field alone).

## 2026-10-08 - S3b-1: clicking a command

The click path is now: cell under the click (`cellulam_ictam`), then
either `iussum_exsequi` (known token there) or `cursorem_ponere`. The
substitution runs as a gesture mutator on the working sheet
(delete the token's cells, `tabula_inserere_spatium` + write the text,
`albare`), and `cursorem_ponere` afterwards flushes - so the whole
command is ONE commit and `u` brings the token back. Errors and
verb failures return before any change. Colouring lives in
scriba_figurae: after a line is drawn, `iussum_proximum` finds known
tokens and redraws those glyphs in COLOR_ACCENT_PRIMARY (skipped on
selected lines, and on lines without a `$`).

## 2026-10-09 - S3b-2: messages

`nuntius` is cleared at the TOP of `scriba_pagina_clavis` for any key
press, committed text or mouse press - before anything else, because a
click on a command may set a new message in the same event. Only
written when present (one attribute read per event otherwise).
`chorda_nulla()` is not declared in chorda.h (dispensator gets it from
elsewhere) - the examen caught the implicit declaration; a local empty
chorda does the job.

## 2026-10-09 - S3d-1: links

`paginam_mutare` became `paginam_ponere(novum, linea, columna)` + the
+-1 step, so links reuse the page switch and can land the cursor on a
tag. Tag cycle searches OTHER pages only: including the viewed page
(as the last stop of the wrap) made a tag present only here "found",
so the user got no message - caught by the first test run. Pages are
opened lazily by the search (scriba_liber_pagina), committed text
only; the click flushes the pending gesture first, so the viewed
page's own edits are in. Links need the page book (standalone scriba:
no colour, no follow).

## 2026-10-09 - Shift+click on a #tag goes backwards (Fran)

nexum_sequi takes `retro`: the page search runs (i + n - k) % n instead
of (i + k) % n, still OTHER pages only and still landing on the first
occurrence of the tag on that page (page-level cycle both ways, so
next-then-prev returns to the same page). Both click paths (focused,
and vicus's one-click ictus_primus hook) pass the event through
scriba_pagina_clavis, so one call site reads MOD_SHIFT. #next/#prev
ignore Shift (not asked). manus_ludus_premere_ad carries no modifiers;
probatio_vicus_nexus VII builds the Shift events by hand.

## 2026-10-13 - body verbs: text goes after '</>'

iussum_exsequi / substitutio_mutator: for a body command the effect's
text is inserted at (linea_clausurae, post_clausuram), not after the
head; a consuming body verb deletes only the head ($verb<>) - if '</>'
is on the same line it shifts left by the head's length, handled. The
cursor goes after the inserted text on the closer's line. Tested in
probatio_vicus_iussa section "corpus" (two throwaway verbs, eco and
ecoc; clicking inside the body runs nothing).

## 2026-10-13 - deferred commands (effectus.differre) for $question

Fran wants $question synchronous with a "loading..." message. A
blocking call cannot paint, so a verb may DEFER: its first call returns
differre + nuntius; iussum_exsequi shows the message and stores the
command (sa->dilatum = I, copy of the Iussum). scriba_reficere (the
pane pulse) calls scriba_iussum_dilatum_currere: stage I -> II returns
VERUM (repaint: the message is now on screen), stage II relocates the
command on the working sheet (same line, start column and verb, else
"<verb>: evanuit"), runs the verb again with iussum->iterum, applies
the effect and commits with scriba_documentum_committere - ONE change,
'u' restores all. Writes to 'nuntius' (owner pagina.clavis) are made
under that scriptor since the pulse runs outside any action.

effectum_applicare is now shared by the click path and the deferred
path: optional head replacement (effectus.caput, body verbs), then
text whose '\n' segments become NEW lines below the insertion line.
tabula_inserere_lineam refuses to push a non-blank last line off the
page, so an answer can only consume trailing blank lines - extra
segments are dropped, user text is never lost. Test: probatio_vicus_
iussa "dilatum" (verb 'mora': message only, first pulse nothing,
second pulse head + two lines, 'u' restores, deleted before the pulse
-> evanuit).

## 2026-10-13 — idle Esc passes through (A6d-2)

In normal mode with nothing pending (no clavis_praecedens, no 'fd'
wait) Esc is now returned unconsumed, next to the existing "Tab in
normal mode -> dispatcher" rule. The dispatcher uses it to leave a
nested scriba (calendar notes -> list); for top-level panes nothing
acts on it (vicus has no Esc handling; dimissio only with an open
popover). Esc still cancels pending commands, visual and insert mode.

## 2026-10-14 — running commands: IussumOpus (quaestio-rivus T7)

A verb can now return `effectus->opus` (`include/iussum.h`): scriba owns
everything around it, so any long command gets the same behaviour.
- Start (stage II of a deferred command): snapshot of `laboris`
  (cells + indentation, in the doc's piscina), the relocated Iussum,
  the final head and `consumit` are kept; a per-pulse piscina
  (`piscina_operis`, vacated each pulse, destroyed at the end).
- `scriba_opus_pulsare` runs in `scriba_reficere` BEFORE the deferred
  command: the frame that starts the command only paints its status,
  the first text appears on the next frame (deterministic, tested:
  pulsus == 0 right after the start).
- Each pulse with new text: restore snapshot, `effectum_applicare` with
  NO head. FINITUM / Esc: restore, apply with the head, ONE
  `scriba_documentum_committere` (one undo). IRRITUM (plan R7): restore
  only, status only, nothing committed.
- Lock: while `sa->opus`, `scriba_pagina_clavis` swallows every event
  (before `nuntium_tollere`, so the status survives); Esc / Ctrl-[
  calls `abrumpere` then closes as FINITUM. `scriba_iussum_ad_punctum`
  is FALSE while running (vicus' first-click path). Page fill =
  `COLOR_SUPERFICIES` while running (`lib/scriba_figurae.c`).
- Status writes go under owner 'pagina.clavis' exactly like the
  deferred path (domini refuse silently otherwise).
- Test lessons: (1) the color probe first sampled columns 30-40, which
  reach past the page onto the desk (always desk gray) - sample inside
  the page; (2) clicking column 0 of a line holding `$lente` re-runs
  the command - prove "unlocked" by typing on an empty line; (3) PLANT
  stayed green: finish WITHOUT restoring the snapshot still showed the
  final text on line XIII (the old "a b" was pushed to XIV) - added
  "nothing left on XIV". Plants: lock removed, restore removed, fill
  unconditional - all red.
