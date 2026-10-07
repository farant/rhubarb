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
