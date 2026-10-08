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


## 2026-10-07 — Cmd keys never reach the program (D6c follow-up)

Fran pasted with Cmd+V into Claude Code and got `v` + text. Our window
loop queues EVERY key event and then hands it to AppKit, which runs the
menu's Paste (now a GLUTINATA event); Claude Code enables kitty keys, so
the queued Cmd+v went out as super+v and was typed as `v`. Ghostty never
forwards a key its keybinding consumed. Suppressing menu-handled keys in
fenestra_macos would break lib/schirmata.c (it imports images on the
Cmd+V KEY event), so terminale drops super key events itself - the
macOS terminal convention (Terminal.app, iTerm), and what the legacy
encoder already did; only the kitty path leaked. Divergence from
Ghostty: it can send unbound super keys under kitty; we never do.

Also seen in that session: box-drawing characters render as tofu - the
6x8 font has no glyphs for them (fonts come after the emulator).

The first D6c commit failed the aedilis gate with "scripsit extra
vestigium: bin/terminale": launching ./apps/terminale/terminale.sh
during a gate rewrites bin/terminale. Not code - rerun passed.

## 2026-10-07 — minimum contrast and faint text

Fran's screenshot (Claude Code in terminale): status lines in #999999
and similar grays nearly vanish on the warm theme background - programs
choose colours for a DARK background. Ghostty's answer is
`minimum-contrast` (WCAG ratio; below it the shader SNAPS text to black
or white). Fran chose ratio 3.0 and a MINIMAL nudge instead: toward the
extreme (black/white) that can contrast more, binary search for the
smallest step reaching 3.0, then step up past rounding. Hues survive (a
light blue path stays blue, cyan stays teal); dim stays dimmer than
normal text.

Faint (SGR 2) was documented in terminale.h but never drawn; now the
text blends 50% toward its background (Ghostty faint-opacity), then the
contrast floor applies. Background for the check = the cell's own
background if it has one, else the page background (theme or OSC 11).
An untouched default colour stays a THEMA token (live theme).

Side effect on tests: section I's exact palette assertions (xterm red
CD0000, bright red, gray 0x80) fall below 3.0 on the warm theme, so
those cells now sit on a white background (all >= 3.9 there); section
VII's OSC colours became light text on the dark OSC 11 background. Both
still assert exact values - the floor simply has nothing to do there.

Visual check: before/after renders of Claude-Code-like lines (headless
PNG, ratio 1.0 vs 3.0) - gray 250, yellow, cyan and #999999 unreadable
before, all readable after.

Glossary: contrastus, legibilis, opacitas.


## 2026-10-07 — D7c: decorations and reverse screen (vttest walk)

vttest's rendition screen showed "underline" identical to "vanilla":
terminale drew only colours, inverse, faint and conceal although the
core stores every SGR attribute. Now (window only):
- underline in all five forms at the bottom pixel row: double = rows
  ch-1 and ch-3, curly = 2-px runs alternating rows ch-1/ch-2, dotted =
  every other pixel, dashed = 3 on / 3 off. Patterns take their phase
  from ABSOLUTE x (`lineam_formatam(..., periodus, plenum, phasis)`), so
  a wave runs on across cells. First draft shifted the curly upper row
  by passing x0+2, which moved the pixels instead of the phase - the
  test caught it; phase is now its own argument.
- underline colour = SGR 58 if set, else the text colour after the
  contrast floor; strike (row ch/2) and overline (row 0) use the text
  colour.
- bold: the 6x8 font has one weight, so the glyph is drawn again one
  pixel right (plus the existing bright palette for 0-7). Its spill
  into the next cell's first column is painted over when that cell has
  its own background - harmless at this size.
- concealed cells (SGR 8) get no decorations either: an underline
  would give away the hidden text's length.
- `TerminaleApplicatio.ornamenta_pixelorum` (Fran approved; default
  VERUM, the twin sets FALSUM): tessellatio turns a horizontal mandate
  line into box-drawing cells, so in the twin an underline would erase
  its letter. Twin decorations would need SGR on tessera cells - parked.
- DECSCNM ?5: `littera_nativa` / `fundus_nativus` swap when
  `aemulator_modi().schirmus_inversus` (Ghostty swaps only the
  defaults; explicit colours and the cursor stay). The surface fill now
  happens whenever the native background is not the theme background
  (was: only after OSC 11), otherwise the window's own theme fill would
  show through.
Parked: italic (mandates cannot shear text), blink (needs a redraw
timer). Tests IX/X; eight plants caught.
Lint note: the local `ima` (bottom row) was a new word; a glossary
entry `imus` turned the oratio oracle gate red (subject precision) -
the glossary is oratio's FIRST lexical source and `imus` is also
"we go" (eo). Renamed to `infima` (WORDS knows it) instead; a house
gloss must never shadow a real Latin form.


## 2026-10-07 — D7c: drawn glyphs (glyphae_ductae)

Box drawing U+2500-257F, blocks U+2580-259F and braille U+2800-28FF no
longer go to the font (tofu): when `ornamenta_pixelorum` is on and the
cell's grapheme is ONE such rune, `larvam_pingere` asks glyphae_ductae
for the opacity mask at cell size and emits one rectangle per run of
equal opacity per row (full = text colour, shade = text blended into the
cell's background). No synthetic bold on these (the extra pixel would
smear lines); decorations still apply. Under the cursor the mask is drawn
in the background colour, as text is. The twin keeps the rune as text
(the host terminal has a real font).

Contrast: Ghostty's `noMinContrast` exempts box drawing, block elements,
legacy computing and powerline from minimum contrast (renderer/cell.zig)
- the colour of a block IS the picture (logo blocks, colour bars).
`litteram_legibilem` gained `graphica`: faint still applies, the
contrast floor does not, for U+2500-259F; braille is NOT exempt
(Ghostty neither). Found red-first: a red `█` came out nudged.

Multi-rune graphemes fall back to text (guard in `runa_ducta`) - not
reachable today: the core drops zero-width marks (graphemes = v2), so
the test I wrote for it could not pass and was removed; the guard stays
for v2. Specimen (Claude Code box, light/double/heavy grids, logo,
shades, eighths, braille, diagonals, dashes, a tmux border) rendered
headless through tools/aemulator_vttest.c and looked at. Six plants
caught (no rune, shade unmixed, cursor as text, contrast on graphics,
twin ignored, runs unmerged).


## 2026-10-07 — install stage (aemulator-plan D8 follow-up)

Fran approved: fabrica action `terminale` (tools/terminale_struere.sh,
the mensor_ui pattern: aedilis -> provenance -> struere -> rm + cp into
bin/terminale) and `institutio_terminale` (tools/instituere.sh ->
~/.bin/terminale, the briar pattern). The app answers `-provenientia`
(fabrica's `relatio`: the binary reports the digest it was built
from), so `bin/fabrica iudicare` can tell a stale install.
apps/terminale/terminale.sh now builds through the same script - once
terminale.c names the provenance file a build that skips writing it
fails. INSTALL ONLY FROM MAIN (after the merge): `./tools/instituere.sh
bin/terminale` or `bin/fabrica sanare installata`. The twin stays
dev-only. terminale is self-contained at run time (font and theme
compiled in, only $SHELL read), so the ~/.bin copy runs anywhere.


## 2026-10-07 — park 011 step 1: contrast cache (measured)

Measured first (tools/aemulator_vttest.c gained `metire N`,
`ornamenta 0|1` and `MAGNITUDO=CxL`): btop at Fran's window size
(160x75 cells) costs ~17 ms per FULL frame at light load - the whole
60 Hz budget; drawn glyphs on/off made no difference. `sample`: raster
62% (pixel-at-a-time fills), contrast floor 22% (`pow()` per channel
inside a 16-step bisection, every cell every frame), glyph masks 6%.

Fix (the function is pure): `canalis_linearis` reads a 256-entry table
filled once with the SAME formula (identical doubles), and
`contrastum_curare` sits in front of `contrastum_computare` with a
256-slot direct-mapped cache keyed by the full (text, background)
pair. Proof of identity: a deterministic colour screen (4 backgrounds
x 52 colours, plain + faint, truecolour, drawn glyphs; under the 512
style cap) renders byte-identical PNGs before/after. A/B under load
(driver built against old vs new terminale.c, 200 frames): ~20 ms ->
~16 ms per frame, CPU time likewise.

The cache test (section XII, 16 text x 16 background colours) first
let two plants through: a key comparing only text, or only
background. The first hash (XOR of bytes) made same-background
collisions IMPOSSIBLE - only colours whose bytes XOR alike collide,
e.g. (r,g,b)/(b,g,r) - so the test could never see them, and real
palettes collided more than they should. Multiplicative mix (top
byte); the test now asserts the nudge DIRECTION too (a correct nudge
moves each channel toward 0 or toward 255 from the cell's own colour),
and its comment records the computed collision counts (8 same
background, 12 same text) so a hash change knows to rerun the plants.
Four plants caught.

Found on the way (not this step's work): the core keeps at most 512
styles per screen (STILI_MAXIMI) - beyond that, new styles fall back to
the default (Ghostty grows page style capacity); and interning a style
is a linear scan over up to 512 entries per SGR. Filed as park 012.
