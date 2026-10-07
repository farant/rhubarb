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
