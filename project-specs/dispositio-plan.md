# dispositio — plan (pure layout in cells; track (b) B2 of ludus_tessera)

Written 2026-10-03. Prompted by Fran: "should we write some kind of
yoga type (or other) layout constraint solver … as a pure utility
library?" Answer recorded: yes, a pure library, shaped like **Clay**
(Nic Barker, single-header C; a deliberate subset of flexbox), not
Yoga (the CSS spec in ~10k lines of C++) and not Cassowary (float
simplex; ambiguous/unsatisfiable layouts that cannot say why).
Oracle: `../clay` @ **e6cc369** (zlib/libpng licence; reference only,
nothing vendored).

## I. Decisions (Fran, 2026-10-03)

1. **A pure library, `dispositio`** (`include/dispositio.h`,
   `lib/dispositio.c`; Cicero's canon of ARRANGEMENT). It knows nothing
   of componens: `componere` calls it to fill `fines` - as tessellatio
   does not know tessera.
2. **Integer, in CELLS.** Layout happens in cells; pixels are cells ×
   Modulus at the end. Cell alignment (B6b: a terminal reports cell
   centres only) holds BY CONSTRUCTION, no snapping pass. The window
   target uses the same 6×8 cell, so one layout serves both.
3. **Clay's model, v1 subset:** per node a direction (row | column);
   per axis a sizing rule FIT | GROW | FIXED | PERCENT with min/max;
   padding; child gap; alignment (start/centre/end) per axis; clip per
   axis (children not compressed, overflow hidden). Text: a node may
   carry text measured by a callback (single line, no wrapping).
4. **Clay as the oracle**, differentially: a small C99 harness (built
   relaxed, outside the C89 build, against `../clay`) lays out the
   same trees; results compared after rounding. Divergences are named
   in the plan, never silent.

## II. The algorithm (Clay e6cc369, read)

- **Fit, leaves up** (`Clay__CloseElement`, clay.h:1867): along the
  direction a node's size = padding + Σ children + gaps; across it =
  padding + max child. A min-size is accumulated the same way (clip
  axes contribute none). Then clamp to the node's own min/max;
  PERCENT nodes start at 0.
- **Grow and shrink, root down per axis**
  (`Clay__SizeContainersAlongAxis`, clay.h:2281): PERCENT children get
  `(parent − padding − gaps) × percent`. Along the axis, the room left
  = parent − padding − content. Room > 0: GROW children only, smallest
  first - grow the smallest up to the next smallest, then split evenly,
  each capped at max (EQUALISING, no weights). Room < 0: resizable
  children (not FIXED/PERCENT), largest first down to the next largest,
  floored at their min - unless the parent clips that axis (then
  nothing is compressed). Across the axis: GROW children fill the inner
  size (clamped), others keep their fit size.
- **Positions, root down** (`Clay__CalculateFinalLayout`, clay.h:2573):
  children placed along the axis with gaps, the free space distributed
  by the parent's alignment; across the axis each child aligned
  within the inner size.

**Our divergence (named): integer cells.** Clay distributes floats; we
distribute whole cells. Rule for the remainder of an even split:
the leftover cells go one each to the EARLIEST children in tree order
(deterministic, documented); the oracle comparison allows exactly
that difference (sizes summing to the same total, each within 1 cell
of Clay's).

## III. API sketch (names settle in D1)

```c
nomen enumeratio { DISPOSITIO_LINEA = 0, DISPOSITIO_COLUMNA }
    DispositioDirectio;                       /* row | column */
nomen enumeratio { DISPOSITIO_APTA = 0, DISPOSITIO_CRESCENS,
    DISPOSITIO_FIXA, DISPOSITIO_PARS } DispositioGenus; /* fit grow fixed % */
nomen enumeratio { DISPOSITIO_INITIUM = 0, DISPOSITIO_MEDIUM,
    DISPOSITIO_FINIS } DispositioAllineatio;

nomen structura {
    DispositioGenus genus;
                s32 valor;      /* FIXA: cellulae; PARS: centesimae */
                s32 minimum;
                s32 maximum;    /* 0 = sine fine */
} DispositioMensura;

nomen structura {
     DispositioMensura latitudo, altitudo;
    DispositioDirectio directio;
                   s32 interior[IV];  /* padding: sinister dexter
                                       * superior inferior */
                   s32 intervallum;   /* gap */
  DispositioAllineatio allineatio_x, allineatio_y;
                   b32 praecidere_x, praecidere_y;   /* clip */
                chorda textus;        /* mensura per mensorem */
} DispositioForma;

Dispositio* dispositio_creare (Piscina* p);
s32  dispositio_addere (Dispositio* d, s32 parens,
                        constans DispositioForma* forma); /* -> index */
vacuum dispositio_computare (Dispositio* d, s32 latitudo, s32 altitudo,
                             DispositioMensor mensor, vacuum* ctx);
Fines dispositio_fines (constans Dispositio* d, s32 index); /* cellulae */
```

`DispositioMensor` = text width in cells (the Modulus measurer of the
scope: a wide rune 2 in the terminal, 1 glyph in the window - D4 of
tessellatio).

## IV. Tasks

**D0 — the oracle.** `tools/dispositio_oraculum.c` (C99, relaxed, like
vendor objects) + script: reads a tree in a small STML form, lays it
out with Clay @ e6cc369, prints one rectangle per node. Red: the
script refuses without `../clay` at the pinned commit (exit 2, named).

**D1 — fit + fixed + percent + positions.** No grow/shrink yet. Hand
tables (a status bar row, a sidebar column, nested padding/gap,
alignment centre/end) AND the same trees through the oracle. Plants:
gap counted n instead of n−1 times; padding added once instead of
both sides.

**D2 — grow and shrink.** Clay's equalising, integer remainder rule
(II). Vectors: equal split, uneven start sizes, max caps, min floors,
clip axis untouched; oracle within the named tolerance. Plants: grow
by weight-of-current-size (not equalising); remainder to the LAST
child.

**D3 — text.** FIT width from the measurer, height one line; a wide
rune measured 2 by a terminal measurer. Plant: byte length instead of
the measurer.

**D4 — RELATIO** (phase log; the ludus_tessera plan's B2 closed).

## V. Gates

D0: radix (tools), generata if a snippet appears. D1-D3: radix (new
lib), aedilis. D4: briar (corpus library added - MUTATIONES line IN
THE COMMIT that adds the lib, i.e. D1), generata.

## AUDIENDA

- Clay's text elements, aspect ratio, floating and scroll offsets were
  NOT read in detail - out of v1.
- Whether the oracle can express our integer inputs exactly (Clay
  takes floats; integer inputs should round-trip) - D0.
- The exact position pass (`Clay__CalculateFinalLayout`) was located,
  not read line by line - D1 reads it.
