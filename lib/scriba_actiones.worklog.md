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
