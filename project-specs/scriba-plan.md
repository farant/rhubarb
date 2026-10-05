# scriba — plan (track (c) of ludus_tessera: the first insula-native widget)

*Written 2026-10-05, from Fran's interview at the start of track (c)
(`project-specs/ludus-tessera-plan.md` §V). The question: how does a
vim text page live on the ludus model - durable document, ephemeral
cursor, actions, figura - in both targets?*

## I. Decisions (Fran, 2026-10-05)

1. **pagina first**, schirmata after (once there is something to
   switch between).
2. **The text lives in an act log, like pictor's strokes.** The
   document is a volumen of acts; the grid is its projection; undo and
   redo are a cursor in the log with branches (`<ramus ab/>`), the
   mechanism pictor already has. vim's own undo stack
   (`VimUndoAcervus`: 32 line operations, character edits not undoable,
   `include/vim.h:84`) is not used by this widget.
3. **One act = the EFFECT of one vim change**, not its keys: when a
   change finishes (Esc ends an insert; `dd`, `p`, `x` complete), the
   grid before and after are diffed and the replaced lines recorded.
   Replay splices lines and never runs vim, so old documents stay
   readable whatever vim.c becomes.
4. **A fixed sheet.** The page is a fixed grid (68×56 by default,
   `TABULA_*_DEFALTA`, `include/tabula_characterum.h:40`), its size in
   the durable insula, like pictor's 320×200 canvas: the viewport is
   responsive, the text is not.
5. **No colouring in v1** (coloratio is a later figura concern).
6. **A new small app, `scriba`** (the scribe, beside pictor the
   painter): page + status row, both targets, like pictor.
7. **The clipboard is ephemeral insula state**, app-level, written by
   the page's actions, recorded and replayed like any state. Not the OS
   pasteboard (a later, explicit command, like vim's `"+`).
8. **Extract pictor's log engine and migrate pictor onto it.** It is
   generic; only the projection is pixels (below). scriba is the second
   client.

9. **An insert in progress is saved DEBOUNCED** (Fran, 2026-10-05,
   before S1): typing changes only the working sheet (gesture tier);
   after ~1 s without a key (its own setting, not the 300 ms
   `quies_ms`) the working sheet is committed as an act - a long insert
   is saved in chunks, a crash loses at most the last pause; Esc
   commits at once; quit commits what is pending before exiting.
10. **One `u` undoes the WHOLE insert** (vim's meaning), however many
    chunks were saved: chunks after the first are marked as joined to
    the previous act, and historia's undo/redo step over a joined
    group (a small, general historia addition).

## II. What exists (read 2026-10-05)

- **pictor_documentum** (`lib/pictor_documentum.c`, 662 lines). The
  generic part: `acta_viva` (:157, the live acts in a range, skipping
  branches' dead spans), `seq_vivum` (:215), `checkpoint_proximus`
  (:237, plagula `checkpoint/<seq>` → massa by sigillum),
  `proicere_ad` (:279), `checkpoint_condere` (:340, tag
  `"pictor:checkpoint"`), `_actum` (:509, appends a `<ramus>` first
  when the cursor is behind the end), `_revocare` (:553), `_reficere`
  (:576), `_verificare` (:636, reproject from nothing and compare
  sigilla). The pictor part: the pixel buffer (`mensura_pixelorum`,
  `vacare_albam` :62), `ictum_applicare` (:76), `actum_applicare`
  (:132, the act dispatch), and the `documentum` plagula holding the
  dimensions (`_aperire` :448).
- **The projection is a flat byte buffer of fixed size** (the
  checkpoint memcpy's `mensura_pixelorum` bytes, :312). A text grid is
  one too: `latitudo × altitudo` cells + `altitudo` s32 sticky
  indentations (`TabulaCharacterum`, `tabula_characterum.h:63`).
- **vim** (`lib/vim.c` 2101 lines, `include/vim.h`): a pure
  interpreter over a `TabulaCharacterum*`, `VimStatus` in → out.
  Mutates the grid in place; `mutatus` per key; `fd` escape keeps
  `f64 tempus_f` (:120); clipboard `VimClipboard` fixed 32 × 68
  (:70).
- **Motus** (`include/motus.h`): the gesture tier - "painting per
  event, writing at rest"; pictor's stroke in progress lives in
  `ictus_pendens` and leaves only through a durable write on mouse-up
  (`lib/pictor_actiones.c:150`). An insert session is the same shape:
  Esc is the mouse-up. Motus's fields are pictor-shaped (points, pan,
  zoom).
- **The legacy page** (`include/pagina.h`, concha): `Pagina` =
  grid + coloratio + VimStatus + blink, drawn with `fenestra.h`. Stays
  as is (concha = LEGACY, ludus-tessera plan §II).

## III. Track H — the log engine (extraction)

**H0 — the oracle before the cut.** A golden volume written by TODAY's
pictor_documentum (strokes, an undo, a branch, enough acts to cross two
checkpoints): every plagula key and value, every massa hash, the final
sigillum, committed as a fixture. After H2 the same script must produce
byte-identical output. Red first: the fixture check runs (and passes)
on today's code; a plant changing the checkpoint tag must fail it.

H0 as built: `probatio_pictor_documentum_aurum` + `fixa/pictor_documentum/
aurum.txt` (59 lines, deterministic). Two plants - the checkpoint tag
renamed; a dead checkpoint admitted as base - compile, pass the old
suite, and are caught only by the golden (worklog).

**H1 — the engine library, `historia`** (name: Fran, 2026-10-05). The log,
branches, checkpoints, cursor, undo/redo, verification; the projection
through a small vtable:

```c
nomen structura {
    vacuum (*vacare)    (vacuum* ctx, i8* memoria);
       b32 (*applicare) (vacuum* ctx, i8* memoria, chorda actum);
} HistoriaProiectio;      /* memoria: mensura bytes, owned by client */
```

plus the checkpoint tag and interval from the client. Tests with a toy
projection (bytes as counters), independent of pictor: branch after
undo, a dead span never replayed, checkpoint chosen only if live, redo
after a branch refused, verify catches a corrupted massa. Plants: no
`<ramus>` after undo; a checkpoint inside a dead span used; replay
from the checkpoint AND its acts (double-applied).

H1 as built: `include/historia.h`, `lib/historia.c` (pictor's engine,
line for line); the projection is `{memoria, mensura, vacare,
applicare, ctx}` - callbacks get the context only, the client owns its
memory. `verificare`'s meaning sharpened (signed divergence, not later
scribbles; worklog). 12 sections with a call-counting toy; four plants.

**H2 — pictor_documentum as a client.** API unchanged
(`include/pictor_documentum.h`); H0's fixture byte-identical; pictor's
suites green (documentum, actiones, figurae, replay). briar MUTATIONES
(new corpus library) in THIS commit.

H2 as built: pictor_documentum keeps the pixel half (662 → 388
lines); the struct lost cursor/finis/numerus_vivorum/sigillum
(accessor `pictor_documentum_numerus_vivorum`); H0 golden byte-identical
first run; H0's plants re-aimed and caught at the same lines; the
ludus_tessera hand list gained `historia`. **Track H done.**

## IV. Track S — scriba

**S0 — the text document.** `scriba_documentum` over the engine: the
projection = cells + indentations; the `documentum` plagula holds the
sheet size. Acts v1:

```
<mutatio linea="3" deletae="1">
  <linea indentatio="4">new text of line 3</linea>
  <linea indentatio="-1">an inserted line 4</linea>
</mutatio>
```

and the diff that produces one from two grids (the smallest line range
that differs; a sheet has fixed height, so a splice keeps the line
count - inserted lines push the tail off the sheet, as vim does on a
full grid: S0 reads what `o` does at the last line). Tests: diff →
apply round-trips on hand vectors (one char, a line insert, a line
delete, the last line, an empty diff = no act); undo/redo through the
engine. Plants: off-by-one range end; indentation not carried.

S0 as built: `include/scriba_documentum.h`, `lib/scriba_documentum.c`.
The diff is generic (trim trailing BLANK lines, common prefix/suffix,
splice + pad), so vim's full-sheet behaviour does not matter to it.
BLANK = all ' ' + indentation -1 (`tabula_initiare` fills spaces - its
header says '\0'); '\0' is content. Lines live in an attribute
(`textus`, escapes `\\ \0 \t \1 \q \xHH`). 77 asserts; four plants.

**S1 split (Fran, 2026-10-05)** after decisions 9-10:

- **S1a - a generic gesture slot in Motus (framework).** Motus gains
  `gestus` (opaque app state of a gesture in progress), its own
  last-change time and quiet interval, and a flush callback;
  `dispensator_tractare` flushes it when quiet (beside the pan/zoom
  flush, on event time - replay-deterministic); new
  `dispensator_finire` flushes everything pending and both glues call
  it before exiting (today nothing flushes at exit). Toy-app tests.
  S1a as built: `MotusGestus gestus` in Motus (status, effusor, ctx,
  quies_ms, tempus, sordidus); `mutare_gestum`, `motus_gestus_quies`,
  `motus_gestum_effundere` (writer "gestus"); `dispensator_finire`;
  both glues call it after their loop. Root 198/198; five plants. The
  glues' call is not headless-testable (S3 shows it).
- **S1b - joined acts in historia.** `historia_actum_coniunctum`: an
  act marked as joined to the previous one (a historia-owned marker,
  like `<ramus>`); undo/redo step over a joined group. H0 golden must
  stay byte-identical.
  S1b as built: `historia_actum_coniunctum` (marker `<coniunctio/>`
  before the act); flags parallel to live acts, cut with them by a
  `<ramus>`; undo/redo over a group. H0 golden byte-identical; four
  plants (one needed a second act after a branch to be seen).
- **S1c - scriba state and keys** (the original S1 below): the working
  sheet lives in the gesture slot; first chunk of an insert = an act,
  later chunks joined; Esc flushes at once.

**S1 — state and actions.** The ephemeral canon: cursor line/column,
mode, visual selection, pending key, the `fd` flag, the clipboard
(yanked lines). The key action rebuilds `VimStatus` from state, runs
vim on the WORKING grid, writes state back; at a change's end the
diff becomes an act. `u` / Ctrl-R in normal mode go to the engine, not
vim. `fd`'s time comes from `Eventus.tempus` (the one-clock rule).
OPEN for S1 (read first, ask if unclear): where the working grid of an
unfinished insert lives - Motus is the right TIER, but its fields are
pictor's (generic payload slot vs a scriba motus). Tests: a key script
→ document + state; undo after an insert removes the whole insert;
yank in one page → paste replays. Plants: change boundary per key (an
insert becomes N acts); clipboard outside the canon (refused silently
- assert it).

**S2 — figura and layout.** Grid → mandata (one text per line, the
cursor cell, the visual selection, the cut-off mark from the legacy
page); `componere` with dispositio: viewport (GROW, clipped, the
sheet inside on the desk, `PARTES_PROSPECTUS`) over a status row
(mode, line:column). Tests: figurae on hand states; the tree golden.

**S3 — the app.** `lib/scriba_applicatio` + `apps/scriba/scriba.c`
(window) + `scriba_terminalis.c` (+ `.sh`), a volumen by path (as
pictor's `pictor_volumen_aperire`). Headless proof: a key session
recorded in the window replays through codificator → rivus → glue to
the same sigillum (as pictor's A4 proof). Fran's look in both targets.

**S4 — RELATIO** (phase log; the ludus-tessera plan's §V updated;
schirmata's tasks written then).

## V. Gates

H0: radix. H1: radix (new lib), aedilis, briar (MUTATIONES). H2:
radix, ludus_tessera, aedilis, briar. S0-S2: radix, aedilis, briar.
S3: + ludus_tessera (the terminal main), generata if a snippet appears.

## AUDIENDA

- vim on a FULL sheet (`o` at the last line, a paste that overflows):
  what it does to the tail was not read - S0.
- Whether `TabulaCharacterum`'s barrier/push logic can change lines
  outside the cursor's (it can push content forward) - the diff must
  find the true range, not assume the cursor line; S0 vectors cover it.
- Motus's genericity (S1): `ictus_pendens` is a Xar of Punctum; the
  `<quies/>` effusion writes pan/zoom only.
- vim's fixed buffers (`VIM_CLIPBOARD_LINEA_MAXIMA` = 68) against a
  sheet wider than 68 - S1 reads; a sheet wider than 68 may be
  refused in v1.
- The legacy page's cut-off mark and blink: read at S2, not now.
