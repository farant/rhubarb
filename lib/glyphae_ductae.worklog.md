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

## 2026-10-07 — v2: symbols (Fran: "symbol extras would be good")

Which symbols: a CENSUS, not a guess. Claude Code's binary (Bun) keeps
its JS strings partly as UTF-16 and as `\uXXXX` escapes; raw byte
counts of single symbols were noise (any 2-byte pair appears hundreds
of times in a 236 MB binary). The escape census was the clean signal:
`— … → ⚠ • ← ✓ ↑ ↓ › ≥ ≤ ✗ ⎿ ◐ ...` plus the UI set (⏺ ✻ ✶ ✳ ✢ ✽ ● ❯
⏵ ⏸ ⌘ ⏎ ...). 57 symbols, all width 1 in our runae tables.

Design: hand-drawn 6x8 pictures (body in columns 1-5, rows 0-6 like
fons_6x8), stored as ASCII rows in a table sorted by rune (binary
search), drawn into the same mask, nearest-neighbour scaled for other
cell sizes. API unchanged - `glyphae_ductae_est` simply recognises
more runes; header doc lists the v2 range. Symbols are TEXT for the
contrast floor (only U+2500-259F are exempt). `– — …` used to reach
the font mapped to '-' and '.' ("Thinking." for "Thinking…"); now
drawn. Specimen rendered headless and looked at; redrawn after the
first look: ⚠ (read as a rook -> filled triangle with the mark cut
out) and ⇧ (mushroom -> outlined arrow). Tests: two exact pictures,
2x scaling, every census symbol recognised and non-empty, neighbours
rejected; four plants caught (table order, scale, recognition, search).
The v1 boundary assertion 0x25A0 moved to 0x25A2 (■ is now a symbol).

### The symbol table generator (symbola.py; output pasted into
`symbola[]` - edit the pictures here, then regenerate)

```python
# symbola 6x8 (columnae 1-5 corpus, ordines 0-6; ordo 7 descensor)
B='......'
S={}
def d(cp, *rows):
    rows=list(rows)+[B]*(8-len(rows))
    assert len(rows)==8 and all(len(r)==6 for r in rows), hex(cp)
    S[cp]=rows
# sagittae
d(0x2190, B,B,'..#...','.#####','..#...')
d(0x2191, '...#..','..###.','.#.#.#','...#..','...#..','...#..','...#..')
d(0x2192, B,B,'....#.','.#####','....#.')
d(0x2193, '...#..','...#..','...#..','...#..','.#.#.#','..###.','...#..')
d(0x21B5, B,'.....#','.....#','..#..#','.#####','..#...')
d(0x21E7, '...#..','..#.#.','.#...#','.##.##','..#.#.','..#.#.','..###.')
# notae
d(0x2713, B,B,'.....#','....#.','.#.#..','..#...')
d(0x2714, B,'.....#','....##','.#.##.','.###..','..#...')
d(0x2715, B,'.#...#','..#.#.','...#..','..#.#.','.#...#')
d(0x2716, B,'.#...#','.##.##','..###.','.##.##','.#...#')
d(0x2717, B,'.#...#','..#.#.','...#..','..#.#.','.#...#')
d(0x2718, B,'.#...#','.##.##','..###.','.##.##','.#...#')
# circuli et puncta
d(0x2022, B,B,'...#..','..###.','...#..')
d(0x2219, B,B,B,'..##..','..##..')
d(0x25CF, B,'..###.','.#####','.#####','.#####','..###.')
d(0x23FA, B,'..###.','.#####','.#####','.#####','..###.')
d(0x25CB, B,'..###.','.#...#','.#...#','.#...#','..###.')
d(0x25C9, B,'..###.','.#...#','.#.#.#','.#...#','..###.')
d(0x25EF, '..###.','.#...#','.#...#','.#...#','.#...#','.#...#','..###.')
d(0x2B24, '..###.','.#####','.#####','.#####','.#####','.#####','..###.')
d(0x25D0, B,'..###.','.##..#','.##..#','.##..#','..###.')
d(0x25D1, B,'..###.','.#..##','.#..##','.#..##','..###.')
d(0x25D2, B,'..###.','.#...#','.#####','.#####','..###.')
d(0x25D3, B,'..###.','.#####','.#####','.#...#','..###.')
# stellae (Claude Code: · ✢ ✳ ✶ ✻ ✽)
d(0x2722, B,'...#..','...#..','.##.##','...#..','...#..')
d(0x2733, B,'.#.#.#','..###.','.#####','..###.','.#.#.#')
d(0x2736, B,'...#..','.#####','..###.','.#####','...#..')
d(0x273B, B,'.#.#.#','..#.#.','.##.##','..#.#.','.#.#.#')
d(0x273D, B,'..#.#.','.##.##','...#..','.##.##','..#.#.')
d(0x2726, '...#..','...#..','..###.','.#####','..###.','...#..','...#..')
d(0x2605, '...#..','...#..','.#####','..###.','..#.#.','.#...#')
# anguli
d(0x203A, B,B,'..#...','...#..','..#...')
d(0x276F, B,'.##...','..##..','...##.','..##..','.##...')
# media et figurae
d(0x23F5, B,'..#...','..##..','..###.','..##..','..#...')
d(0x23F8, B,'.##.##','.##.##','.##.##','.##.##','.##.##')
d(0x25B6, '.#....','.##...','.###..','.####.','.###..','.##...','.#....')
d(0x25B8, B,B,'..#...','..##..','..#...')
d(0x25B2, B,'...#..','..###.','.#####')
d(0x25BC, B,B,'.#####','..###.','...#..')
d(0x25A0, B,'.#####','.#####','.#####','.#####','.#####')
d(0x25A1, B,'.#####','.#...#','.#...#','.#...#','.#####')
d(0x25AA, B,B,'..###.','..###.','..###.')
# claves macOS
d(0x2318, B,'.##.##','.#####','..#.#.','.#####','.##.##')
d(0x2325, B,B,'.##.##','...#..','....##')
d(0x23CE, B,'.....#','.....#','..#..#','.#####','..#...')
# varia
d(0x26A0, '...#..','..###.','..#.#.','.##.##','.#####','.##.##','.#####')
d(0x23BF, '..#...','..#...','..#...','..####')
d(0x29C9, B,'.###..','.#.###','.###.#','...#.#','...###')
d(0x22EE, B,'...#..',B,'...#..',B,'...#..')
d(0x2610, '.#####','.#...#','.#...#','.#...#','.#...#','.#...#','.#####')
d(0x2612, B,'.#####','.##.##','.#.#.#','.##.##','.#####')
d(0x2264, '....#.','...#..','..#...','...#..','....#.',B,'..###.')
d(0x2265, '..#...','...#..','....#.','...#..','..#...',B,'..###.')
d(0x2261, B,'.#####',B,'.#####',B,'.#####')
# punctuatio (fons.c ad '-' '.' vertebat)
d(0x2013, B,B,B,'.####.')
d(0x2014, B,B,B,'######')
d(0x2026, B,B,B,B,B,B,'.#.#.#')
import unicodedata, sys
out=[]
for cp in sorted(S):
    nm=unicodedata.name(chr(cp))
    nm=nm.lower()
    while len(('    { 0x%04X,   /* %s %s */' % (cp, chr(cp), nm)).encode())>72: nm=nm.rsplit(' ',1)[0]
    out.append('    { 0x%04X,   /* %s %s */\n' % (cp, chr(cp), nm))
    for i,r in enumerate(S[cp]):
        out.append('      { "%s" }%s\n' % (r, ',' if i<7 else ' },') if False else '')
    out.append('        { ' + ', '.join('"%s"'%r for r in S[cp][:4]) + ',\n')
    out.append('          ' + ', '.join('"%s"'%r for r in S[cp][4:]) + ' } },\n')
sys.stdout.write(''.join(out))
print('/* numerus %d */' % len(S), file=sys.stderr)
# specimen sheet for review
with open('/private/tmp/claude-501/-Users-francisarant-Documents-projects-rhubarb-secunda/134c8417-d34b-49a2-b8c8-3d3b81913875/scratchpad/symbola_specimen.txt','w') as f:
    cps=sorted(S)
    for i in range(0,len(cps),10):
        grp=cps[i:i+10]
        for r in range(8): f.write('  '.join(S[c][r] for c in grp)+'\n')
        f.write('  '.join(('U+%04X'%c)[2:].ljust(6) for c in grp)+'\n\n')
```

## 2026-10-08 - v3: Greek and maths (Fran's tofu list)

Fran saw tofu in terminale for `cos 2π/19`, `a = 10¹⁰⁰⁰`, `|α| ≫ 1`,
`∛2`, `√`. fons_6x8 has no Greek (fons.c even said so, with pi as the
example). Split:

- **Lookalikes go through fons.c** (Α Β Ε Ζ Η Ι Κ Μ Ν Ο Ρ Τ Υ Χ and ο ->
  the Latin glyph): they then work in EVERY app (scriba, pictor), not
  only terminale.
- **Drawn here** (terminale asks glyphae_ductae BEFORE fons, so these
  win): 24 lowercase, 10 capitals unlike Latin (Γ Δ Θ Λ Ξ Π Σ Φ Ψ Ω),
  superscript 0-9 + - n, subscript 0-9, √ ∛ ≪ ≫. 61 new, 118 total.
  ² ³ ¹ were mapped by fons.c to FULL-SIZE digits (10¹⁰⁰⁰ read as
  1010000); the drawn small digits now take precedence in terminale.
  Superscripts = a 3x5 digit in rows 0-4 (cols 1-3), subscripts the
  same in rows 3-7.
- Lowercase sits on fons_6x8's metrics: x-height rows 2-6, ascenders
  from row 0, descender row 7 (read off `o p d y` in fons_6x8.h).

Limit: scriba/pictor draw text through fons only, so a drawn glyph
(π, α...) is still tofu there - moving the symbol fallback into
fenestra_textus is a later, separate change.

Specimen through the real terminale raster: tools/aemulator_vttest
with `/bin/cat` as the child and one `mitte` line of UTF-8 (the tty
echo puts it on screen), `MAGNITUDO=48x10`, PNG upscaled with sips for
viewing. Plants (6, all named): pi picture, a subscript drawn high, a
missing entry, broken table order, two fons mappings.

### The symbol table generator, v3 (supersedes the v2 block above)

```python
# symbola 6x8 (columnae 1-5 corpus, ordines 0-6; ordo 7 descensor)
B='......'
S={}
def d(cp, *rows):
    rows=list(rows)+[B]*(8-len(rows))
    assert len(rows)==8 and all(len(r)==6 for r in rows), hex(cp)
    S[cp]=rows
# sagittae
d(0x2190, B,B,'..#...','.#####','..#...')
d(0x2191, '...#..','..###.','.#.#.#','...#..','...#..','...#..','...#..')
d(0x2192, B,B,'....#.','.#####','....#.')
d(0x2193, '...#..','...#..','...#..','...#..','.#.#.#','..###.','...#..')
d(0x21B5, B,'.....#','.....#','..#..#','.#####','..#...')
d(0x21E7, '...#..','..#.#.','.#...#','.##.##','..#.#.','..#.#.','..###.')
# notae
d(0x2713, B,B,'.....#','....#.','.#.#..','..#...')
d(0x2714, B,'.....#','....##','.#.##.','.###..','..#...')
d(0x2715, B,'.#...#','..#.#.','...#..','..#.#.','.#...#')
d(0x2716, B,'.#...#','.##.##','..###.','.##.##','.#...#')
d(0x2717, B,'.#...#','..#.#.','...#..','..#.#.','.#...#')
d(0x2718, B,'.#...#','.##.##','..###.','.##.##','.#...#')
# circuli et puncta
d(0x2022, B,B,'...#..','..###.','...#..')
d(0x2219, B,B,B,'..##..','..##..')
d(0x25CF, B,'..###.','.#####','.#####','.#####','..###.')
d(0x23FA, B,'..###.','.#####','.#####','.#####','..###.')
d(0x25CB, B,'..###.','.#...#','.#...#','.#...#','..###.')
d(0x25C9, B,'..###.','.#...#','.#.#.#','.#...#','..###.')
d(0x25EF, '..###.','.#...#','.#...#','.#...#','.#...#','.#...#','..###.')
d(0x2B24, '..###.','.#####','.#####','.#####','.#####','.#####','..###.')
d(0x25D0, B,'..###.','.##..#','.##..#','.##..#','..###.')
d(0x25D1, B,'..###.','.#..##','.#..##','.#..##','..###.')
d(0x25D2, B,'..###.','.#...#','.#####','.#####','..###.')
d(0x25D3, B,'..###.','.#####','.#####','.#...#','..###.')
# stellae (Claude Code: · ✢ ✳ ✶ ✻ ✽)
d(0x2722, B,'...#..','...#..','.##.##','...#..','...#..')
d(0x2733, B,'.#.#.#','..###.','.#####','..###.','.#.#.#')
d(0x2736, B,'...#..','.#####','..###.','.#####','...#..')
d(0x273B, B,'.#.#.#','..#.#.','.##.##','..#.#.','.#.#.#')
d(0x273D, B,'..#.#.','.##.##','...#..','.##.##','..#.#.')
d(0x2726, '...#..','...#..','..###.','.#####','..###.','...#..','...#..')
d(0x2605, '...#..','...#..','.#####','..###.','..#.#.','.#...#')
# anguli
d(0x203A, B,B,'..#...','...#..','..#...')
d(0x276F, B,'.##...','..##..','...##.','..##..','.##...')
# media et figurae
d(0x23F5, B,'..#...','..##..','..###.','..##..','..#...')
d(0x23F8, B,'.##.##','.##.##','.##.##','.##.##','.##.##')
d(0x25B6, '.#....','.##...','.###..','.####.','.###..','.##...','.#....')
d(0x25B8, B,B,'..#...','..##..','..#...')
d(0x25B2, B,'...#..','..###.','.#####')
d(0x25BC, B,B,'.#####','..###.','...#..')
d(0x25A0, B,'.#####','.#####','.#####','.#####','.#####')
d(0x25A1, B,'.#####','.#...#','.#...#','.#...#','.#####')
d(0x25AA, B,B,'..###.','..###.','..###.')
# claves macOS
d(0x2318, B,'.##.##','.#####','..#.#.','.#####','.##.##')
d(0x2325, B,B,'.##.##','...#..','....##')
d(0x23CE, B,'.....#','.....#','..#..#','.#####','..#...')
# varia
d(0x26A0, '...#..','..###.','..#.#.','.##.##','.#####','.##.##','.#####')
d(0x23BF, '..#...','..#...','..#...','..####')
d(0x29C9, B,'.###..','.#.###','.###.#','...#.#','...###')
d(0x22EE, B,'...#..',B,'...#..',B,'...#..')
d(0x2610, '.#####','.#...#','.#...#','.#...#','.#...#','.#...#','.#####')
d(0x2612, B,'.#####','.##.##','.#.#.#','.##.##','.#####')
d(0x2264, '....#.','...#..','..#...','...#..','....#.',B,'..###.')
d(0x2265, '..#...','...#..','....#.','...#..','..#...',B,'..###.')
d(0x2261, B,'.#####',B,'.#####',B,'.#####')
# punctuatio (fons.c ad '-' '.' vertebat)
d(0x2013, B,B,B,'.####.')
d(0x2014, B,B,B,'######')
d(0x2026, B,B,B,B,B,B,'.#.#.#')
# --- v3 (2026-10-08): Graeca et mathematica (Franus: tofu in terminale) ---
# Graeca 6x8 (columnae 1-5; minusculae ordines 2-6, ascensus 0, descensus 7)
# minusculae
d(0x03B1, B,B,'..##.#','.#..#.','.#..#.','.#..#.','..##.#')            # α
d(0x03B2, '..##..','.#..#.','.#.#..','.#..#.','.#...#','.#...#','.####.','.#....')  # β
d(0x03B3, B,B,'.#...#','.#...#','..#.#.','..#.#.','...#..','...#..')  # γ
d(0x03B4, '..##..','.#....','..#...','..##..','.#..#.','.#..#.','..##..')  # δ
d(0x03B5, B,B,'..###.','.#....','..##..','.#....','..###.')            # ε
d(0x03B6, '.####.','...#..','..#...','.#....','.#....','..###.','.....#','....#.')  # ζ
d(0x03B7, B,B,'.#.##.','.##..#','.#...#','.#...#','.#...#','.....#')  # η
d(0x03B8, '..##..','.#..#.','.#..#.','.####.','.#..#.','.#..#.','..##..')  # θ
d(0x03B9, B,B,'..#...','..#...','..#...','..#..#','...##.')            # ι
d(0x03BA, B,B,'.#..#.','.#.#..','.##...','.#.#..','.#..#.')            # κ
d(0x03BB, '.#....','..#...','..#...','...#..','..#.#.','.#...#','.#...#')  # λ
d(0x03BC, B,B,'.#...#','.#...#','.#...#','.#..##','.###.#','.#....')  # μ
d(0x03BD, B,B,'.#...#','.#...#','..#..#','..#.#.','...#..')            # ν
d(0x03BE, '.####.','..#...','...##.','..#...','.#....','..###.','.....#','...##.')  # ξ
d(0x03C0, B,B,'.#####','..#.#.','..#.#.','..#.#.','..#..#')            # π
d(0x03C1, B,B,'..###.','.#...#','.#...#','.##..#','.#.##.','.#....')  # ρ
d(0x03C2, B,B,'..###.','.#....','.#....','..##..','....#.','...#..')  # ς
d(0x03C3, B,B,'..####','.#..#.','.#...#','.#...#','..###.')            # σ
d(0x03C4, B,B,'.#####','...#..','...#..','...#..','....##')            # τ
d(0x03C5, B,B,'.#..#.','.#...#','.#...#','.#...#','..###.')            # υ
d(0x03C6, B,'...#..','.#.##.','.#.#.#','.#.#.#','..###.','...#..','...#..')  # φ
d(0x03C7, B,B,'.#...#','..#.#.','...#..','..#.#.','.#...#')            # χ
d(0x03C8, B,'...#..','.#.#.#','.#.#.#','.#.#.#','..###.','...#..','...#..')  # ψ
d(0x03C9, B,B,'..#.#.','.#...#','.#.#.#','.#.#.#','..#.#.')            # ω
# maiusculae quae Latinis dissimiles
d(0x0393, '.#####','.#....','.#....','.#....','.#....','.#....','.#....')  # Γ
d(0x0394, '...#..','...#..','..#.#.','..#.#.','.#...#','.#...#','.#####')  # Δ
d(0x0398, '..###.','.#...#','.#...#','.#####','.#...#','.#...#','..###.')  # Θ
d(0x039B, '...#..','...#..','..#.#.','..#.#.','.#...#','.#...#','.#...#')  # Λ
d(0x039E, '.#####',B,B,'..###.',B,B,'.#####')                        # Ξ
d(0x03A0, '.#####','.#...#','.#...#','.#...#','.#...#','.#...#','.#...#')  # Π
d(0x03A3, '.#####','.#....','..#...','...#..','..#...','.#....','.#####')  # Σ
d(0x03A6, '...#..','..###.','.#.#.#','.#.#.#','.#.#.#','..###.','...#..')  # Φ
d(0x03A8, '.#.#.#','.#.#.#','.#.#.#','..###.','...#..','...#..','...#..')  # Ψ
d(0x03A9, '..###.','.#...#','.#...#','.#...#','..#.#.','..#.#.','.##.##')  # Ω
# similes Latinis -> fons.c (non hic)
SIMILES={0x0391:'A',0x0392:'B',0x0395:'E',0x0396:'Z',0x0397:'H',0x0399:'I',0x039A:'K',
 0x039C:'M',0x039D:'N',0x039F:'O',0x03A1:'P',0x03A4:'T',0x03A5:'Y',0x03A7:'X',0x03BF:'o'}
# --- mathematica (Franus 2026-10-08: 10¹⁰⁰⁰, |α| ≫ 1, ∛2, √) ---
PARVI={'0':['###','#.#','#.#','#.#','###'],'1':['.#.','##.','.#.','.#.','###'],
 '2':['###','..#','###','#..','###'],'3':['###','..#','.##','..#','###'],
 '4':['#.#','#.#','###','..#','..#'],'5':['###','#..','###','..#','###'],
 '6':['###','#..','###','#.#','###'],'7':['###','..#','.#.','.#.','.#.'],
 '8':['###','#.#','###','#.#','###'],'9':['###','#.#','###','..#','###'],
 '+':['...','.#.','###','.#.','...'],'-':['...','...','###','...','...'],
 'n':['...','##.','#.#','#.#','#.#']}
def parvus(ch, summus):
    rows=[B]*8
    off=0 if summus else 3
    for i,r in enumerate(PARVI[ch]): rows[off+i]='.'+r+'..'
    return rows
SUPRA={'0':0x2070,'1':0x00B9,'2':0x00B2,'3':0x00B3,'4':0x2074,'5':0x2075,'6':0x2076,
 '7':0x2077,'8':0x2078,'9':0x2079,'+':0x207A,'-':0x207B,'n':0x207F}
for ch,cp in SUPRA.items(): d(cp,*parvus(ch,True))
for k in range(10): d(0x2080+k,*parvus(str(k),False))
d(0x221A, '.....#','.....#','....#.','....#.','.#.#..','..##..','...#..')  # √
d(0x221B, '.##..#','..#..#','.##.#.','....#.','.#.#..','..##..','...#..')  # ∛
d(0x226A, B,'...#.#','..#.#.','.#.#..','..#.#.','...#.#')                  # ≪
d(0x226B, B,'.#.#..','..#.#.','...#.#','..#.#.','.#.#..')                  # ≫
import unicodedata, sys
out=[]
for cp in sorted(S):
    nm=unicodedata.name(chr(cp))
    nm=nm.lower()
    while len(('    { 0x%04X,   /* %s %s */' % (cp, chr(cp), nm)).encode())>72: nm=nm.rsplit(' ',1)[0]
    out.append('    { 0x%04X,   /* %s %s */\n' % (cp, chr(cp), nm))
    for i,r in enumerate(S[cp]):
        out.append('      { "%s" }%s\n' % (r, ',' if i<7 else ' },') if False else '')
    out.append('        { ' + ', '.join('"%s"'%r for r in S[cp][:4]) + ',\n')
    out.append('          ' + ', '.join('"%s"'%r for r in S[cp][4:]) + ' } },\n')
sys.stdout.write(''.join(out))
print('/* numerus %d */' % len(S), file=sys.stderr)
# specimen sheet for review
with open('/private/tmp/claude-501/-Users-francisarant-Documents-projects-rhubarb-secunda/134c8417-d34b-49a2-b8c8-3d3b81913875/scratchpad/symbola_specimen_v3.txt','w') as f:
    cps=sorted(S)
    for i in range(0,len(cps),10):
        grp=cps[i:i+10]
        for r in range(8): f.write('  '.join(S[c][r] for c in grp)+'\n')
        f.write('  '.join(('U+%04X'%c)[2:].ljust(6) for c in grp)+'\n\n')
```

