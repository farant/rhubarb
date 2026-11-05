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
