# terminale worklog

App `terminale` (aemulator-plan E-thin, decisions 23-25). Library
`lib/terminale.c` behind the approved `include/terminale.h`.

## 2026-10-06/07 — E2: the library, headless

**A screenshot caught what the tests could not.** The first PNG showed
the SGR 44 background cell as near-white. Decoding the pixels: (255,
238, 0, 0) for an intended (0, 0, 238) - bytes reversed. `mandatum.h`
said an RGBA mandate colour is the literal 0xRRGGBBAA; both rasterizers
(`delineare_mandata` color_ex_mandato, `tessellatio` _color) read it
with `color_ex_pixelum` (frame-buffer packing a<<24|b<<16|g<<8|r), and
the two existing producers (their tests) build it with
`color_ad_pixelum`. So the COMMENT was wrong; fixed the comment, used
color_ad_pixelum here. My first test asserted the literal 0xCD0000FF -
it encoded the same wrong assumption and passed. Pixels are now read
back from the PNG and asserted.

**Key pairing.** Real sources (fenestra, manus_ludus) send key down,
then its text, then key up; the codificator wants (key, text) as one
series. A key press therefore pends in a private context; the next
TEXTUS COMMISSUM/SCRIPTA is encoded with it, anything else flushes it
alone (Enter, Ctrl-C, arrows have no text). Plant: encode each event
alone -> caught ("ls" test).

**No canon needed (yet).** With no canon registered, insula accepts the
dispatcher's focus and surface writes. A vicus mount will need one.

**Scroll was never broken.** E1 read fenestra's empty `scrollWheel:`
view method as "scroll dropped"; scroll is translated earlier, in the
polling path. Stale comment in ludus_fenestra.h led there.

**Byte counts again:** a 96-byte script passed as 89 cut the trailing
cursor move. Count literal bytes with a tool.

## 2026-10-07 — E3: the apps

**Draw only on change.** The window loop pulses the host (no wait),
waits on window events up to 16 ms (0 when a frame is owed), and runs
ludus_quadrum + present only if the pulse reported `mutatum` or an
event arrived. -fumus always draws (it counts frames).

**Nested smoke for the twin.** The tool shell has no TTY, so
`terminale_terminalis -fumus` ran as the CHILD of a scratch runner on
aemulator_hospes; dumping its screen when "linea 3" appeared showed the
exact sample. A dump after exit is empty - the twin draws on the
alternate screen and restores the primary when it leaves. 4 unknown
sequences = the twin's mouse/paste mode requests (phase D).

## 2026-10-07 — D6b: modes, mouse, focus, title, colours

**Window focus events go to the root.** destinatio routes keys, text and
wheel to the focus ("focal") and mouse buttons by position; anything
else (EVENTUS_FOCUS / DEFOCUS from a window) goes to the tree root. Our
root had no action, so DEFOCUS vanished; the root now carries
"terminale.clavis" too (unhandled genera return FALSUM, harmless).

**Wheel decision order** (Ghostty): program tracks the mouse -> wheel
report (our own residue makes whole lines first; the encoder divides by
the cell height again, so the synthetic event carries lines x cell
height); alternate screen + ?1007 -> arrow keys through the encoder
(DECCKM honoured); else the history view.

**Focus report on enable** happens after each pulse: the snapshot's
`focus` turning on sends CSI I or CSI O from the tracked state
(initially focused, as Ghostty's flags). Plant G11 (report every pulse)
is caught by the second frame.

**Title:** the core's titulus effect needs the context BEFORE the host
exists (effect datum) - creation order changed. The window main copies
the title into a stack buffer: a pool copy per change would grow for
the whole session (zsh retitles on every prompt).

**Colours:** an unchanged dynamic colour (live value == theme colour
given at creation) stays a THEMA token so a theme switch still applies;
once a program sets it, RGB. A changed background paints one rectangle
over the whole surface first.

