# antiphonale worklog

## 2026-11-04 — P6: the pane (no sound yet)

**Model: aetherophonum, not chronica.** chronica carries semitae,
history, people/places; aetherophonum is the small pane with custom
drawing and a pane-specific port injected at mount — the same shape we
need (P7 wires AntiphonalePorta to the vicus sound port's carmen slots).
Mounted through the sound context (`soni`) for the same reason.

**Where things happen.** File reading and version swaps in the pulse
(`antiphonale_pulsare`, woken by the specula's fossa); layout lazily in
the compositor (`compositio_currens`: recomposed when pane width/height
or spatium changes, in its own piscina so relayout never grows the
version); the figure only reads and emits Mandata (figura.h <purus/>).
A version = one piscina (text + GabcCantus); the old one is destroyed
only after the new one is built (plant: destroying first loses the
drawing on a bad save). Byte-identical re-reads keep the version (no
spurious redraw).

**Banner** (Review Focus 1): "L:C: causa" from the first VITIUM,
"empty file", "missing: <path>"; drawn over the page top in COLOR_ERROR;
the next good save clears it without a keypress.

**specula needed NOTE_ATTRIB.** Truncating a file to zero bytes
(fopen "w" with no write) raises neither NOTE_WRITE nor NOTE_EXTEND on
macOS, so "empty file" was never seen. Added NOTE_ATTRIB to
specula_macos.c, with two specula tests: truncation is seen, and plain
reading (atime) raises nothing — so no reader wakes itself in a loop.

**Keys.** membrum__praecepta gained "Left", "Right", "PageUp",
"PageDown" forms (no modifiers) — PageUp/PageDown here, Left/Right for
P7's seek. j/k too. Zoom + (or =) and -: the first syllable on the
current page is found before the relayout and its new page becomes the
current page.

**$antiphonale(path)** is its own iussum over vicus_acervo_mutare (the
front antiphonale pane takes the new path; none: mounts one). "~/"
expands to HOME — the vicus test points HOME at its scratch area so the
typed path fits the 80-column scriba line.

**Not yet:** sound (P7); the pane does not pulse in the background
(vivit_in_fundo FALSE) — a save while another pane is in front shows on
return.

## 2026-11-04 — plain background inside the border (Fran)

figura_mensam_pingere fills the whole pane with the focus texture when
focused; the pane drew straight on it, so the whole background was
textured. Now, as navigator and chronica: a COLOR_BACKGROUND rectangle
inside a 6 x 8 border (ANTIPHONALE_LIMBUS_X/Y), and the page, banner,
bottom line and arrow hit areas all live inside it; the layout width and
height shrink by the border. Test `_fundus_intra_limbum` (red before).

## 2026-11-05 — P7: playback in the pane

Space (or the ▶/■ button in the bottom line) plays from pulse 0 or from
the selected syllable and stops; a click selects a syllable when stopped
and seeks to its first link row when playing; Left/Right go to the
previous/next syllable that has notes. Every pulse while playing (mora
XXXIII) reads the port's pulse, lights the syllable of the last link row
whose start <= pulse (binary search; a breath between incisa keeps the
previous syllable lit), and turns the page to it — also after a zoom or
a swap moved it (Review Focus 5). A good save while playing calls
incipere(new, P) BEFORE the old version is destroyed; a version shorter
than P, or not playable, stops; a bad save keeps playing the old one
(G18). A chant with no notes draws, and Space says "not playable: <why>"
in the banner (Review Focus 2). The highlight leads the sound by the
audio device latency (cantor's pulse is the mixer clock) — accepted v1.

**The pane must not know carmen.h.** First cut built the Carmen in the
pane (carmina_legere) and ludus_tessera — which links vicus — failed on
mixtura/sonus/tibia: carmen.c pulls the sound engine. vicus_sonus.h says
vicus knows no sound. So the pane only runs the converter (gabc_carmen:
text + link table, pure) and hands the carmen TEXT to the port; the
port (vicus_sonus_porta.c, sound tier) parses it into its own piscina
and frees the previous one after cantor holds the new. P5's slot changed
from `constans Carmen*` to `chorda` accordingly.

**Closing the pane while playing** leaves the sound running until it
ends or vicus mutes — vicus has no unmount hook for panes yet.

**Editing lesson:** a whitespace-tolerant regex replace (indentation as
`\s+`) swallowed the newline before each replaced block and joined ~26
line pairs; caught by the formatter's long-line report, split by a
script restricted to the added lines. Exact replacements from now on.

## 2026-11-06 — final review: watcher retry leaked while the folder was missing

`specula_creare` allocates the Specula and both path strings in the
arena it is given BEFORE it can fail (parent directory missing). The
pane passed `m->piscina` (lives as long as vicus) and, with no watcher,
retried on every pulse at MORA_SINE_SPECULA = 16 ms: ~40 MB/hour for an
80-char path plus a 60 Hz wake, for `$antiphonale(~/typo/x.gabc)` or a
branch switch that removes the folder. Fix: the watcher gets its own
arena (`piscina_speculae`); `speculam_aperire` destroys it when
`specula_creare` fails, `speculam_claudere` destroys it on path change
(also ends the small per-path-change leak in `viam_ponere`).
MORA_SINE_SPECULA is now 250 ms — the same interval is the re-read
cadence on platforms without a watcher (Android), still live enough.
Test: section "directorium absens" (200 pulses, `piscina_summa_usus`
unchanged; mora >= 250; folder + file created -> picked up, mora -1).

## 2026-11-06 — review minors 1-4: banners, selection after a swap, click past the end

- A live swap into a version with no notes stopped the sound silently;
  now it sets "not playable: <reason>" like Space does, and keeps the
  place (electa = the lit syllable).
- `incipere` refused (vicus refuses only when muted or without an audio
  device) used to do nothing visible: banner VEXILLUM_MUTUM ("can't
  play: sound muted or unavailable"), both on Space and on a swap
  mid-play; the next successful Space clears that banner (only that
  one — a parse-error banner stays).
- Selection after a swap keeps its INDEX, not its musical position:
  the common edit reshapes a melody (Jesus melisma) and keeps the
  syllables, so the index stays right while the pulse would land inside
  an earlier, longer melisma. Playback still maps by pulse (G18 —
  sound is time). A shorter version clamps the selection to the last
  syllable (before: it vanished and Space played from the top).
- Click on a syllable after the last note, then Space: starts at the
  last note (`nexus_syllabae(..., -I)`), not at the beginning.
Test: section "vexilla et electa" (6 checks red before).

## 2026-11-06 - review minor VIII: tests for RF1, RF2, RF5

Tests only, no library change. `probatio_antiphonale_latus`:

- **RF1 (rename-save):** `_servatum_probare` now saves the way editors
  do - write `<via>.tmp`, `filum_movere` it over the chant - twice (good,
  then VITIUM), then writes in place. The last step is the real check:
  the watcher must follow the NEW inode after the rename, not the
  unlinked old one. Plant (no close/re-open on NOTE_DELETE|NOTE_RENAME
  in specula_macos.c) turns all three rename steps red.
- **RF2 (converter refuses):** with the pane's ordinary form (strictus
  FALSUM) `gabc_carmen_scribere` refuses a chant that HAS notes only on
  memory failure - no file can reach "the converter refused it". The
  reachable case (no notes) now asserts the reason text "not playable:
  no notes". After a live swap to a no-notes version, the syllable where
  sound stopped stays SELECTED (drawn lit), same as a Space-stop - by
  design since minor III, not a stale highlight.
- **RF5 (resize / swap while playing):** helper `_superficiem_ponere`
  writes superficies_latitudo/_altitudo as scriptor "dispensator" (as
  navigator's test does). A taller pane, a smaller pane, and a swap that
  removes the (z) breaks (same times, q25 on an earlier page) all keep
  q25 lit on the VISIBLE page. Plant "page follows only when the lit
  syllable changes" turns the smaller-pane step red.

Gotcha: the "pagina" attribute can sit PAST the last page after a
relayout (taller pane -> fewer pages). Nothing breaks - pagina_currens
clamps it for drawing, page-follow, and PageUp/PageDown - but tests
must read the visible page ("k/N" in the bottom line, `_pagina_visa`),
not the raw attribute.
