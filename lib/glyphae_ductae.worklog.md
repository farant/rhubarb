# glyphae_ductae worklog

## 2026-10-07 — v1: box drawing, blocks, braille as masks (aemulator-plan D7c)

Why: terminale's 6x8 font has 256 slots (Latin-1 fills the top half),
so box drawing (128), blocks (32) and braille (256) showed as tofu -
Claude Code's rounded boxes, tmux borders, btop graphs. Ghostty draws
these procedurally at cell size (src/font/sprite/draw), so lines meet
exactly across cells; we port that. Fran approved the API: pure, no
allocation, output = an OPACITY MASK (one byte per pixel, 0..255, row
order) that the caller turns into mandates or pixels.

Ported, with Ghostty's names in the code:
- `lineas_ducere` = linesChar: four arms, style none/light/heavy/double;
  the arm ends in the centre follow Ghostty's up_bottom / down_top /
  left_right / right_left rules exactly (double-line junctions too).
- `interruptas_*` = dashHorizontal / dashVertical: N dashes, gap
  min(desired, size / 2N), remainder distributed into dashes; when the
  cell is narrower than 2N pixels Ghostty draws a solid light line -
  so at 6 px wide `┈` (4 dashes) IS `─`. Faithful, pinned in tests.
- arcs `╭╮╯╰`: vertical line, cubic with control points at a quarter
  radius (r = min(w, h) / 2), horizontal line; diagonals `╱╲╳` with the
  half-pixel overshoot. Ghostty strokes these anti-aliased; v1 is
  binary: a pixel is on when its CENTRE lies STRICTLY within half the
  thickness of the path. `<=` doubled pixels at exact ties (`╳` at
  12x16 had a 4-wide centre) - strict pinned by a 12x16 check.
- blocks = block.zig: eighths rounded half-up, quadrants via
  Fraction.min/max at one half, shades `░▒▓` as opacity 0x40/0x80/0xC0
  (terminale blends text into background - better than a checker at
  6x8).
- braille = braille.zig's distribution of spare pixels (dot width,
  margins, spacing): at 6x8 dots are 1 px at columns 1, 4 and rows
  0, 2, 4, 6.
Thickness: Ghostty uses the font's underline thickness; we derive
light = max(1, (h + 8) / 16) (1 px up to 23 px tall), heavy = 2x.

Tests are ASCII pictures (`.` `#` `1` `2` `3`), hand-derived from
Ghostty's arithmetic at 6x8; arcs/diagonals were looked at (6x8 and
12x16) and then pinned. Table-wide laws: every line character's four
edges match empty or a pure line's edge (continuity across cells) at
five sizes, and nothing is written past w*h at sizes down to 1x1. Ten
plants caught (table entry, centres, double junction, dash remainder,
braille bit, shade value, clipping, threshold, heavy centre, arc
direction).

Lint: new words were renamed, not glossed (`ima` -> `infima`: a gloss
for `imus` breaks the oratio oracle - "we go"; `lineolas` ->
`interruptas`; `braille_*` -> `puncta_*`; `ax..by` -> `x0..y1`;
`alignatum` -> `marginale`). A NEW untracked file is invisible to
`vocabula.sh -nova` until `git add -N` - the commit would have caught
it, my hand check did not.

### The arm table generator (run from the repo root; output pasted
into `brachia_capsarum`, comment `s d i l` = up right down left)

```python
import re
src=open('../ghostty/src/font/sprite/draw/box.zig').read()
body=src[src.index('pub fn draw2500_257F'):src.index('pub fn linesChar')]
pairs=re.findall(r"// '(.)'\s*\n\s*0x([0-9a-fA-F]{4}) => (.*?)(?=\n\s*// '|\n\s*else =>)", body, re.S)
STY={'none':0,'light':1,'heavy':2,'double':3}
rom=['ZEPHYRUM','I','II','III']
out=[]; n=0
for ch,cp,expr in pairs:
    cp=int(cp,16); n+=1
    m=re.match(r"linesChar\(metrics, canvas, \.\{(.*?)\}\)", expr.strip())
    if m:
        arms=dict(up=0,right=0,down=0,left=0)
        for k,v in re.findall(r"\.(\w+) = \.(\w+)", m.group(1)):
            arms[k]=STY[v]
        v=arms['up']|arms['right']<<2|arms['down']<<4|arms['left']<<6
        out.append((cp,ch,'0x%02X'%v, 's%d d%d i%d l%d'%(arms['up'],arms['right'],arms['down'],arms['left'])))
    else:
        out.append((cp,ch,'0x00', expr.strip().split('(')[0].replace('try ','')))
assert n==128, n
assert [c for c,_,_,_ in out]==list(range(0x2500,0x2580))
for cp,ch,v,c in out:
    print('    %s,   /* %04X %s %s */' % (v, cp, ch, c))
```
