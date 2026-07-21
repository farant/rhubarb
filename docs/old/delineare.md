# DELINEARE: 2D Graphics Drawing Library Documentation

Another one? You want to draw shapes now? Fine. Listen, if you just read the FENESTRA docs, this is the obvious next step. You've got pixels, now you want lines and circles and all that. This is DELINEARE.

**delineare** (verb, first conjugation) = to draw, to trace out, to sketch. From "linea" (line). You're literally "de-lining" - drawing with lines.

## What This Library Does

DELINEARE draws 2D shapes to a pixel buffer. It sits on top of FENESTRA - you create a window with FENESTRA, get a pixel buffer, then use DELINEARE to draw on it.

Shapes you can draw:
- Lines (straight, horizontal, vertical)
- Rectangles (outline or filled)
- Circles (outline or filled)
- Triangles (outline or filled)
- Ellipses (outline or filled)
- Polygons (arbitrary number of sides, outline or filled)

You also get:
- Clipping rectangles (draw only inside a region)
- Pattern fills (38 built-in patterns from classic Mac)
- XOR mode for old-school effects
- Grid drawing for quick debug visuals

Everything uses classic algorithms - Bresenham for lines, midpoint for circles/ellipses, scanline for filling. These are the algorithms from the 1960s-80s that actually work. No GPU, no shaders, just math and pixels.

## Basic Setup

You need a drawing context. The context holds state - what you're drawing to, clip rectangle, current pattern, drawing mode.

```c
#include "delineare.h"
#include "fenestra.h"
#include "piscina.h"

integer principale(vacuum)
{
    Piscina* piscina;
    FenestraConfiguratio config;
    Fenestra* fenestra;
    TabulaPixelorum* tabula;
    ContextusDelineandi* ctx;

    piscina = piscina_generare_dynamicum("app", M * II);
    si (!piscina) redde I;

    /* Create window - covered in FENESTRA docs */
    config.titulus = "Drawing Test";
    config.x = C;
    config.y = C;
    config.latitudo = DCCC;
    config.altitudo = DC;
    config.vexilla = FENESTRA_ORDINARIA;

    fenestra = fenestra_creare(piscina, &config);
    tabula = fenestra_creare_tabulam_pixelorum(piscina, fenestra, CDLXXX);

    /* Create drawing context */
    ctx = delineare_creare_contextum(piscina, tabula);
    si (!ctx)
    {
        fenestra_destruere(fenestra);
        piscina_destruere(piscina);
        redde I;
    }

    /* Main loop */
    dum (!fenestra_debet_claudere(fenestra))
    {
        fenestra_perscrutari_eventus(fenestra);

        /* Clear to black */
        delineare_vacare(ctx, RGB(ZEPHYRUM, ZEPHYRUM, ZEPHYRUM));

        /* Draw stuff */
        delineare_circulum(ctx, CC, CC, L, RGB(CCLV, ZEPHYRUM, ZEPHYRUM));

        fenestra_praesentare_pixela(fenestra, tabula);
        fenestra_dormire(16666);
    }

    fenestra_destruere(fenestra);
    piscina_destruere(piscina);

    redde ZEPHYRUM;
}
```

New Latin vocabulary:
- **contextus** = context (literally "woven together")
- **delineare** = to draw/sketch
- **vacare** = to empty/clear

This creates a drawing context attached to your pixel buffer, clears the buffer to black each frame, and draws a red circle.

### What's ContextusDelineandi?

It's a struct that holds drawing state:

```c
nomen structura {
    TabulaPixelorum* tabula;      /* Pixel buffer */

    /* Clip rectangle */
    i32 praecisio_x;
    i32 praecisio_y;
    i32 praecisio_latitudo;
    i32 praecisio_altitudo;

    /* Current pattern (8x8 bitmap) */
    i8 exemplar[VIII];

    /* Drawing mode */
    modus_delineandi_t modus;

    /* Pattern offset for tiling */
    i32 exemplar_offset_x;
    i32 exemplar_offset_y;
} ContextusDelineandi;
```

**praecisio** = clipping/cutting off. **exemplar** = pattern/example. **modus** = mode/manner.

The context starts with no clipping (draws to entire buffer), solid fill mode, and an all-white pattern. You can change these.

## Drawing Lines

Lines are fundamental. Everything else is made of lines.

### Horizontal and Vertical Lines

```c
delineare_lineam_horizontalem(ctx, X, CXL, L, RGB(CCLV, CCLV, CCLV));
delineare_lineam_verticalem(ctx, C, XX, CXX, RGB(CCLV, CCLV, CCLV));
```

**lineam** = line (accusative of "linea"). **horizontalem** = horizontal. **verticalem** = vertical.

First one draws a white horizontal line from x=10 to x=140 at y=50. Second draws a white vertical line from y=20 to y=120 at x=100.

These are optimized - they're just loops setting pixels. Faster than general line drawing.

### General Lines (Bresenham)

```c
delineare_lineam(ctx, X, X, CXC, CXC, RGB(CCLV, ZEPHYRUM, ZEPHYRUM));
```

Draws a red diagonal line from (10, 10) to (190, 190).

This uses Bresenham's line algorithm. You don't need to know how it works, but if you're curious: it's integer-only, never uses division or floating point, and picks the best pixels to approximate a line. Been around since 1962, still the standard.

The algorithm handles all eight octants (every direction). Steep lines, shallow lines, negative slopes - all work.

## Drawing Rectangles

Rectangles come in two flavors: outline and filled.

### Rectangle Outline

```c
delineare_rectangulum(ctx, L, L, C, C, RGB(ZEPHYRUM, CCLV, ZEPHYRUM));
```

**rectangulum** = rectangle (literally "right-angled"). Draws a green rectangle outline at (50, 50) with width 100 and height 100.

This just draws four lines - top, bottom, left, right. Corner pixels get drawn twice, but that doesn't matter.

### Filled Rectangle

```c
delineare_rectangulum_plenum(ctx, CC, CC, C, L, RGB(ZEPHYRUM, ZEPHYRUM, CCLV));
```

**plenum** = full/filled. Draws a blue filled rectangle at (200, 200), width 100, height 50.

This is a nested loop - for each row, set every pixel in that row. Simple but effective. For filling large areas quickly, use this instead of setting individual pixels.

## Drawing Circles

Circles use the midpoint circle algorithm. No trig, no floating point, pure integer math.

### Circle Outline

```c
delineare_circulum(ctx, CC, CC, L, RGB(CCLV, CCLV, ZEPHYRUM));
```

**circulum** = circle. Draws a yellow circle outline at center (200, 200) with radius 50.

The algorithm draws 8 points at a time - using symmetry, one calculated point gives you 8 pixels (4 quadrants, 2 reflections each). It's elegant.

### Filled Circle

```c
delineare_circulum_plenum(ctx, CCC, CC, XXX, RGB(CCLV, ZEPHYRUM, CCLV));
```

Draws a magenta filled circle at (300, 200), radius 30.

Instead of drawing 8 points, it draws 4 horizontal lines per calculated point. Fills from the center outward.

## Drawing Triangles

Triangles need three vertices. You specify them as three (x, y) pairs.

### Triangle Outline

```c
delineare_triangulum(ctx, C, C, CC, X, L, CL, RGB(CCLV, CXXVIII, ZEPHYRUM));
```

**triangulum** = triangle. Draws an orange triangle with vertices at (100, 100), (200, 10), (50, 150).

Three lines connecting three points. Nothing fancy.

### Filled Triangle

```c
delineare_triangulum_plenum(ctx, CC, CC, CCC, CC, CCL, CCC,
                              RGB(CXXVIII, ZEPHYRUM, CXXVIII));
```

Draws a purple filled triangle with vertices at (200, 200), (300, 200), (250, 300).

This uses scanline filling. The algorithm:
1. Sort vertices by Y coordinate (top to bottom)
2. For each horizontal scanline, find where it intersects the triangle edges
3. Draw a horizontal line between those intersections

Works for any triangle - acute, obtuse, right-angled, whatever.

## Drawing Ellipses

Ellipses are like circles but squashed. Two radii instead of one.

### Ellipse Outline

```c
delineare_ellipsin(ctx, CC, CC, C, L, RGB(ZEPHYRUM, CCLV, CCLV));
```

**ellipsin** = ellipse (accusative). Draws a cyan ellipse at (200, 200) with horizontal radius 100 and vertical radius 50.

Uses the midpoint ellipse algorithm - similar to circles but handles different X and Y radii. Still integer-only, still fast.

### Filled Ellipse

```c
delineare_ellipsin_plenam(ctx, CCC, CL, L, XXX, RGB(CCLV, CCLV, CCLV));
```

**plenam** = filled (accusative feminine). Draws a white filled ellipse at (300, 150), horizontal radius 50, vertical radius 30.

Same idea as filled circles - draw horizontal lines instead of points.

## Drawing Polygons

Polygons are arbitrary shapes with any number of sides. You pass an array of vertices.

### Polygon Outline

```c
i32 puncta[] = {
    C, C,         /* Vertex 0: (100, 100) */
    CC, L,        /* Vertex 1: (200, 50) */
    CCC, C,       /* Vertex 2: (300, 100) */
    CCL, CC,      /* Vertex 3: (250, 200) */
    CL, CC        /* Vertex 4: (150, 200) */
};

delineare_polygonum(ctx, puncta, V, RGB(CCLV, CLXV, ZEPHYRUM));
```

**polygonum** = polygon. **puncta** = points (plural of "punctum"). **V** = 5 (five vertices).

The array is x, y, x, y, x, y... Each pair is one vertex. The library draws lines connecting consecutive vertices and closes the shape (last vertex connects to first).

This draws a pentagon in gold color.

### Filled Polygon

```c
i32 hexagonum[] = {
    CC, C,
    CCC, CL,
    CCC, CCL,
    CC, CCC,
    C, CCL,
    C, CL
};

delineare_polygonum_plenum(ctx, hexagonum, VI, RGB(CXXVIII, CXXVIII, CXXVIII));
```

**hexagonum** = hexagon. **VI** = 6. Draws a gray filled hexagon.

This uses scanline filling like triangles:
1. For each horizontal scanline
2. Find all intersections with polygon edges
3. Sort the intersections
4. Fill between pairs of intersections (1-2, 3-4, 5-6, etc.)

Works for convex and concave polygons. The algorithm is robust.

The implementation limits intersections to 256 per scanline. If you have a polygon so complex it needs more than 256 intersections on one scanline, you're doing something very wrong or very right, and either way you're on your own.

## Clipping

Clipping restricts drawing to a rectangular region. Anything outside gets discarded.

### Setting Clip Rectangle

```c
delineare_ponere_praecisionem(ctx, C, C, CC, CC);
```

**ponere** = to set/place. **praecisionem** = clipping (accusative of "praecisio").

This sets a clip rectangle at (100, 100) with width 200 and height 200. Now all drawing happens only inside that rectangle.

```c
/* Only the part inside (100,100,200,200) appears */
delineare_circulum(ctx, CC, CC, C, RGB(CCLV, ZEPHYRUM, ZEPHYRUM));
```

This circle is centered at (200, 200) with radius 100. Without clipping, you'd see the whole circle. With clipping, you see only the portion inside the clip rectangle.

### Removing Clipping

```c
delineare_tollere_praecisionem(ctx);
```

**tollere** = to lift/remove. Resets clipping to the full buffer.

### Why Use Clipping?

For UI panels, scrollable areas, split screens - anywhere you want to constrain drawing to a region. It's faster than manually checking bounds in your drawing code.

The library checks clipping on every pixel, so there's a small cost. But it's just a few comparisons per pixel, negligible compared to the rest of the work.

## Drawing Modes

DELINEARE supports three drawing modes: solid, pattern, and XOR.

### Solid Mode (Default)

```c
delineare_ponere_modum(ctx, MODUS_SOLIDUS);
```

**modus** = mode. **solidus** = solid.

In solid mode, pixels are set to the exact color you specify. This is the default.

### Pattern Mode

```c
delineare_ponere_modum(ctx, MODUS_EXEMPLAR);
```

**exemplar** = pattern.

In pattern mode, shapes are filled with an 8x8 repeating pattern instead of solid color. The pattern is a bitmap - each bit determines whether a pixel gets the color or not.

The pattern is an 8-byte array, one byte per row. Each bit in the byte represents a pixel. Bit 7 (MSB) is the leftmost pixel, bit 0 (LSB) is rightmost.

#### Using Built-In Patterns

The library has 38 built-in patterns from classic Mac:

```c
delineare_ponere_exemplar_internum(ctx, EXEMPLAR_TESSELLATUM);
delineare_ponere_modum(ctx, MODUS_EXEMPLAR);
delineare_rectangulum_plenum(ctx, C, C, CC, CC, RGB(CCLV, ZEPHYRUM, ZEPHYRUM));
```

**tessellatum** = checkerboard (from "tessella" = small tile). This fills a rectangle with a checkerboard pattern in red.

Built-in patterns:
- `EXEMPLAR_SOLIDUS` - solid fill (all bits set)
- `EXEMPLAR_TESSELLATUM` - checkerboard
- `EXEMPLAR_LINEAE_HORIZONTALES` - horizontal lines
- `EXEMPLAR_LINEAE_VERTICALES` - vertical lines
- `EXEMPLAR_DIAGONALIS` - diagonal lines
- `EXEMPLAR_LATERICIUM` - brick pattern
- `EXEMPLAR_PUNCTA_DISPERSA` - scattered dots
- `EXEMPLAR_ALBUS` - white/empty (no bits set)

And 30 more. See the enum in the header for all of them. These are based on the 38 standard patterns from the original Macintosh (1984).

#### Custom Patterns

```c
constans i8 meum_exemplar[VIII] = {
    0xFF,  /* Row 0: ████████ */
    0x81,  /* Row 1: █      █ */
    0x81,  /* Row 2: █      █ */
    0x81,  /* Row 3: █      █ */
    0x81,  /* Row 4: █      █ */
    0x81,  /* Row 5: █      █ */
    0x81,  /* Row 6: █      █ */
    0xFF   /* Row 7: ████████ */
};

delineare_ponere_exemplar(ctx, meum_exemplar);
delineare_ponere_modum(ctx, MODUS_EXEMPLAR);
delineare_circulum_plenum(ctx, CC, CC, L, RGB(ZEPHYRUM, CCLV, ZEPHYRUM));
```

This defines a pattern that's a hollow square and fills a circle with it. The pattern tiles - repeats every 8 pixels horizontally and vertically.

#### Pattern Offset

By default, patterns align to (0, 0). You can offset them:

```c
delineare_ponere_offset_exemplaris(ctx, IV, IV);
```

**offset** is an English loanword here. This shifts the pattern 4 pixels right and 4 pixels down. Useful for aligning patterns when drawing adjacent shapes.

### XOR Mode

```c
delineare_ponere_modum(ctx, MODUS_XOR);
```

In XOR mode, the color is XORed with the existing pixel value instead of overwriting it.

```c
/* Draw a red rectangle */
delineare_ponere_modum(ctx, MODUS_SOLIDUS);
delineare_rectangulum_plenum(ctx, C, C, CC, CC, RGB(CCLV, ZEPHYRUM, ZEPHYRUM));

/* XOR with white - inverts colors */
delineare_ponere_modum(ctx, MODUS_XOR);
delineare_circulum_plenum(ctx, CL, CL, L, RGB(CCLV, CCLV, CCLV));
```

The circle XORs with the rectangle. Where they overlap, the red inverts to cyan. Draw the circle again and the original color returns - that's the magic of XOR.

XOR mode was huge in the 80s and 90s for UI effects - rubber-band selection boxes, drag outlines, etc. Draw something with XOR, move it, draw it again to erase, draw at new location. No need to save/restore the background.

These days it's less common because we have double buffering and can just redraw everything. But it's still useful for certain effects and it's computationally cheap.

## Utility Functions

### Clearing the Buffer

```c
delineare_vacare(ctx, RGB(CCLV, CCLV, CCLV));
```

Fills the entire buffer with white. This temporarily disables clipping, fills the buffer, then restores clipping.

You could achieve the same with `delineare_rectangulum_plenum()` and manually disabling clipping, but this is more convenient.

### Drawing a Grid

```c
delineare_cratem(ctx, XX, RGB(CXXVIII, CXXVIII, CXXVIII));
```

**cratem** = grid (accusative of "cratis" = wicker/lattice). Draws a gray grid with 20-pixel spacing.

Draws vertical lines every 20 pixels and horizontal lines every 20 pixels. Useful for debug visuals, graph paper effects, alignment guides.

### Drawing Individual Pixels

```c
delineare_pixelum(ctx, C, C, RGB(CCLV, ZEPHYRUM, ZEPHYRUM));
```

**pixelum** = pixel. Sets a single pixel at (100, 100) to red.

You can do this, but it's usually slower than using FENESTRA's `tabula_pixelorum_ponere_pixelum()` directly. This function goes through the context (respects clipping, patterns, drawing mode), whereas the FENESTRA function is just a direct write.

Use this when you want clipping or pattern fills for individual pixels. Otherwise use the FENESTRA function.

## A Complete Example

Here's a program that draws various shapes with different modes:

```c
#include "delineare.h"
#include "fenestra.h"
#include "piscina.h"

integer principale(vacuum)
{
    Piscina* piscina;
    FenestraConfiguratio config;
    Fenestra* fenestra;
    TabulaPixelorum* tabula;
    ContextusDelineandi* ctx;

    piscina = piscina_generare_dynamicum("shapes", M * II);
    si (!piscina) redde I;

    config.titulus = "Shape Demo";
    config.x = C;
    config.y = C;
    config.latitudo = DCCC;
    config.altitudo = DC;
    config.vexilla = FENESTRA_ORDINARIA;

    fenestra = fenestra_creare(piscina, &config);
    si (!fenestra)
    {
        piscina_destruere(piscina);
        redde I;
    }

    tabula = fenestra_creare_tabulam_pixelorum(piscina, fenestra, CDLXXX);
    si (!tabula)
    {
        fenestra_destruere(fenestra);
        piscina_destruere(piscina);
        redde I;
    }

    ctx = delineare_creare_contextum(piscina, tabula);
    si (!ctx)
    {
        fenestra_destruere(fenestra);
        piscina_destruere(piscina);
        redde I;
    }

    dum (!fenestra_debet_claudere(fenestra))
    {
        fenestra_perscrutari_eventus(fenestra);

        /* Clear to dark gray */
        delineare_vacare(ctx, RGB(LX, LX, LX));

        /* Solid shapes */
        delineare_ponere_modum(ctx, MODUS_SOLIDUS);
        delineare_rectangulum_plenum(ctx, XX, XX, C, C, RGB(CCLV, ZEPHYRUM, ZEPHYRUM));
        delineare_circulum_plenum(ctx, CL, LXX, XL, RGB(ZEPHYRUM, CCLV, ZEPHYRUM));
        delineare_triangulum_plenum(ctx, CCL, XX, CCC, CXX, CC, CXX,
                                      RGB(ZEPHYRUM, ZEPHYRUM, CCLV));

        /* Pattern fill */
        delineare_ponere_exemplar_internum(ctx, EXEMPLAR_TESSELLATUM);
        delineare_ponere_modum(ctx, MODUS_EXEMPLAR);
        delineare_rectangulum_plenum(ctx, XX, CCL, C, C, RGB(CCLV, CCLV, ZEPHYRUM));

        /* Outlined shapes */
        delineare_ponere_modum(ctx, MODUS_SOLIDUS);
        delineare_rectangulum(ctx, CCC, XX, CL, CL, RGB(CCLV, CCLV, CCLV));
        delineare_circulum(ctx, CCCLXXV, LXX, XL, RGB(CCLV, CCLV, CCLV));

        /* Draw grid in clipped region */
        delineare_ponere_praecisionem(ctx, CCC, CCL, CCL, CCL);
        delineare_cratem(ctx, XX, RGB(C, C, C));
        delineare_tollere_praecisionem(ctx);

        fenestra_praesentare_pixela(fenestra, tabula);
        fenestra_dormire(16666);
    }

    fenestra_destruere(fenestra);
    piscina_destruere(piscina);

    redde ZEPHYRUM;
}
```

This draws:
- Red, green, and blue filled shapes (rectangle, circle, triangle)
- Yellow rectangle with checkerboard pattern
- White outlined rectangle and circle
- Grid in the bottom-right corner (clipped region)

Compile it, run it, look at it. That's how you learn.

## Performance Considerations

### Filled vs Outlined

Filled shapes are slower than outlines - they draw more pixels. A filled circle with radius 100 draws about 31,000 pixels. The outline draws about 628 pixels.

For large shapes, filled operations take noticeably longer. If you're drawing hundreds of shapes per frame, consider using outlines or smaller shapes.

### Pattern Mode Cost

Pattern mode adds a bitwise test per pixel. For each pixel:
1. Calculate position in 8x8 pattern
2. Check if bit is set
3. Only write pixel if bit is set

This is faster than solid mode for sparse patterns (few set bits) because you're writing fewer pixels. For dense patterns (many set bits), it's slower because of the extra checks.

### XOR Mode Cost

XOR mode adds one read and one XOR operation per pixel. Solid mode is just a write. The difference is small but measurable.

If you're XORing large areas every frame, you might notice the cost. For occasional XOR effects (UI feedback, selection boxes), it's negligible.

### Clipping Cost

Clipping adds 4 comparisons per pixel (x < min, x >= max, y < min, y >= max). Modern CPUs handle this easily. The cost is tiny compared to memory access.

If you're paranoid about performance, disable clipping when you don't need it. But honestly, don't worry about it unless profiling shows it's a problem.

### Algorithm Complexity

All the primitive algorithms are roughly O(perimeter) for outlines and O(area) for fills:

- Line: O(length)
- Rectangle outline: O(2×width + 2×height)
- Rectangle fill: O(width × height)
- Circle outline: O(radius)
- Circle fill: O(radius²)
- Triangle fill: O(height × average_width)
- Polygon fill: O(height × edges)

The implementations are tight loops with minimal overhead. Bresenham's algorithm uses only integer addition and subtraction - no multiplication or division in the inner loop.

For most use cases, you can draw hundreds of shapes per frame at 60 FPS on a modern CPU.

## Context State Management

The context holds state. This is convenient but can bite you if you forget to reset things.

### Resetting the Context

```c
delineare_restituere_contextum(ctx);
```

**restituere** = to restore. Resets all context state to defaults:
- Clipping set to full buffer
- Drawing mode set to solid
- Pattern set to all-white
- Pattern offset set to (0, 0)

Use this when you want a clean slate. Otherwise you might wonder why your shapes are clipped or patterned when you didn't ask for that.

### State to Track

Things that persist in the context:
- Clip rectangle
- Drawing mode
- Current pattern
- Pattern offset

The color is NOT stored in the context - you pass it to each drawing function. Same with coordinates. The context only holds persistent state that affects how drawing happens.

---

# API Reference

Every function, detailed. You know the drill.

## Context Management

### delineare_creare_contextum

```c
ContextusDelineandi* delineare_creare_contextum(
    Piscina* piscina,
    TabulaPixelorum* tabula
);
```

Creates a drawing context.

**Parameters:**
- `piscina` - Memory pool for allocation
- `tabula` - Pixel buffer to draw to

**Returns:** New context, or `NIHIL` on error

**Notes:** Context starts with no clipping, solid mode, white pattern.

### delineare_restituere_contextum

```c
vacuum delineare_restituere_contextum(
    ContextusDelineandi* ctx
);
```

Resets context to default state.

**Parameters:**
- `ctx` - Context to reset

**Notes:** Clears clipping, resets to solid mode, resets pattern.

## Drawing Mode

### delineare_ponere_modum

```c
vacuum delineare_ponere_modum(
    ContextusDelineandi* ctx,
    modus_delineandi_t modus
);
```

Sets the drawing mode.

**Parameters:**
- `ctx` - Drawing context
- `modus` - Mode: `MODUS_SOLIDUS`, `MODUS_EXEMPLAR`, or `MODUS_XOR`

## Pattern Management

### delineare_ponere_exemplar

```c
vacuum delineare_ponere_exemplar(
    ContextusDelineandi* ctx,
    constans i8 exemplar[VIII]
);
```

Sets a custom pattern.

**Parameters:**
- `ctx` - Drawing context
- `exemplar` - 8-byte pattern (one byte per row)

**Notes:** Each byte is a row. Bit 7 = leftmost pixel, bit 0 = rightmost.

### delineare_ponere_exemplar_internum

```c
vacuum delineare_ponere_exemplar_internum(
    ContextusDelineandi* ctx,
    i32 exemplar_id
);
```

Sets a built-in pattern.

**Parameters:**
- `ctx` - Drawing context
- `exemplar_id` - Pattern ID (0-37)

**Notes:** See `exemplar_internum_t` enum for available patterns.

### delineare_ponere_offset_exemplaris

```c
vacuum delineare_ponere_offset_exemplaris(
    ContextusDelineandi* ctx,
    i32 x,
    i32 y
);
```

Sets pattern offset.

**Parameters:**
- `ctx` - Drawing context
- `x` - Horizontal offset
- `y` - Vertical offset

**Notes:** Shifts where the pattern starts. Useful for aligning tiled patterns.

## Clipping

### delineare_ponere_praecisionem

```c
vacuum delineare_ponere_praecisionem(
    ContextusDelineandi* ctx,
    i32 x,
    i32 y,
    i32 latitudo,
    i32 altitudo
);
```

Sets clipping rectangle.

**Parameters:**
- `ctx` - Drawing context
- `x` - Clip rectangle X
- `y` - Clip rectangle Y
- `latitudo` - Clip rectangle width
- `altitudo` - Clip rectangle height

**Notes:** Drawing outside this rectangle is ignored. Automatically clamped to buffer bounds.

### delineare_tollere_praecisionem

```c
vacuum delineare_tollere_praecisionem(
    ContextusDelineandi* ctx
);
```

Removes clipping.

**Parameters:**
- `ctx` - Drawing context

**Notes:** Resets clipping to full buffer.

## Primitives - Pixel

### delineare_pixelum

```c
vacuum delineare_pixelum(
    ContextusDelineandi* ctx,
    i32 x,
    i32 y,
    i32 color
);
```

Draws a single pixel.

**Parameters:**
- `ctx` - Drawing context
- `x` - X coordinate
- `y` - Y coordinate
- `color` - RGBA8888 color

**Notes:** Respects clipping, pattern, and drawing mode.

## Primitives - Lines

### delineare_lineam

```c
vacuum delineare_lineam(
    ContextusDelineandi* ctx,
    i32 x0,
    i32 y0,
    i32 x1,
    i32 y1,
    i32 color
);
```

Draws a line.

**Parameters:**
- `ctx` - Drawing context
- `x0` - Start X
- `y0` - Start Y
- `x1` - End X
- `y1` - End Y
- `color` - RGBA8888 color

**Notes:** Uses Bresenham's algorithm.

### delineare_lineam_horizontalem

```c
vacuum delineare_lineam_horizontalem(
    ContextusDelineandi* ctx,
    i32 x0,
    i32 x1,
    i32 y,
    i32 color
);
```

Draws a horizontal line.

**Parameters:**
- `ctx` - Drawing context
- `x0` - Start X
- `x1` - End X
- `y` - Y coordinate
- `color` - RGBA8888 color

**Notes:** Optimized for horizontal lines. x0 and x1 can be in any order.

### delineare_lineam_verticalem

```c
vacuum delineare_lineam_verticalem(
    ContextusDelineandi* ctx,
    i32 x,
    i32 y0,
    i32 y1,
    i32 color
);
```

Draws a vertical line.

**Parameters:**
- `ctx` - Drawing context
- `x` - X coordinate
- `y0` - Start Y
- `y1` - End Y
- `color` - RGBA8888 color

**Notes:** Optimized for vertical lines. y0 and y1 can be in any order.

## Primitives - Rectangles

### delineare_rectangulum

```c
vacuum delineare_rectangulum(
    ContextusDelineandi* ctx,
    i32 x,
    i32 y,
    i32 latitudo,
    i32 altitudo,
    i32 color
);
```

Draws a rectangle outline.

**Parameters:**
- `ctx` - Drawing context
- `x` - Top-left X
- `y` - Top-left Y
- `latitudo` - Width
- `altitudo` - Height
- `color` - RGBA8888 color

### delineare_rectangulum_plenum

```c
vacuum delineare_rectangulum_plenum(
    ContextusDelineandi* ctx,
    i32 x,
    i32 y,
    i32 latitudo,
    i32 altitudo,
    i32 color
);
```

Draws a filled rectangle.

**Parameters:**
- `ctx` - Drawing context
- `x` - Top-left X
- `y` - Top-left Y
- `latitudo` - Width
- `altitudo` - Height
- `color` - RGBA8888 color

## Primitives - Circles

### delineare_circulum

```c
vacuum delineare_circulum(
    ContextusDelineandi* ctx,
    i32 centrum_x,
    i32 centrum_y,
    i32 radius,
    i32 color
);
```

Draws a circle outline.

**Parameters:**
- `ctx` - Drawing context
- `centrum_x` - Center X
- `centrum_y` - Center Y
- `radius` - Radius
- `color` - RGBA8888 color

**Notes:** Uses midpoint circle algorithm.

### delineare_circulum_plenum

```c
vacuum delineare_circulum_plenum(
    ContextusDelineandi* ctx,
    i32 centrum_x,
    i32 centrum_y,
    i32 radius,
    i32 color
);
```

Draws a filled circle.

**Parameters:**
- `ctx` - Drawing context
- `centrum_x` - Center X
- `centrum_y` - Center Y
- `radius` - Radius
- `color` - RGBA8888 color

## Primitives - Triangles

### delineare_triangulum

```c
vacuum delineare_triangulum(
    ContextusDelineandi* ctx,
    i32 x0,
    i32 y0,
    i32 x1,
    i32 y1,
    i32 x2,
    i32 y2,
    i32 color
);
```

Draws a triangle outline.

**Parameters:**
- `ctx` - Drawing context
- `x0` - Vertex 0 X
- `y0` - Vertex 0 Y
- `x1` - Vertex 1 X
- `y1` - Vertex 1 Y
- `x2` - Vertex 2 X
- `y2` - Vertex 2 Y
- `color` - RGBA8888 color

### delineare_triangulum_plenum

```c
vacuum delineare_triangulum_plenum(
    ContextusDelineandi* ctx,
    i32 x0,
    i32 y0,
    i32 x1,
    i32 y1,
    i32 x2,
    i32 y2,
    i32 color
);
```

Draws a filled triangle.

**Parameters:**
- `ctx` - Drawing context
- `x0` - Vertex 0 X
- `y0` - Vertex 0 Y
- `x1` - Vertex 1 X
- `y1` - Vertex 1 Y
- `x2` - Vertex 2 X
- `y2` - Vertex 2 Y
- `color` - RGBA8888 color

**Notes:** Uses scanline filling.

## Primitives - Ellipses

### delineare_ellipsin

```c
vacuum delineare_ellipsin(
    ContextusDelineandi* ctx,
    i32 centrum_x,
    i32 centrum_y,
    i32 radius_x,
    i32 radius_y,
    i32 color
);
```

Draws an ellipse outline.

**Parameters:**
- `ctx` - Drawing context
- `centrum_x` - Center X
- `centrum_y` - Center Y
- `radius_x` - Horizontal radius
- `radius_y` - Vertical radius
- `color` - RGBA8888 color

**Notes:** Uses midpoint ellipse algorithm.

### delineare_ellipsin_plenam

```c
vacuum delineare_ellipsin_plenam(
    ContextusDelineandi* ctx,
    i32 centrum_x,
    i32 centrum_y,
    i32 radius_x,
    i32 radius_y,
    i32 color
);
```

Draws a filled ellipse.

**Parameters:**
- `ctx` - Drawing context
- `centrum_x` - Center X
- `centrum_y` - Center Y
- `radius_x` - Horizontal radius
- `radius_y` - Vertical radius
- `color` - RGBA8888 color

## Primitives - Polygons

### delineare_polygonum

```c
vacuum delineare_polygonum(
    ContextusDelineandi* ctx,
    constans i32* puncta,
    i32 numerus_punctorum,
    i32 color
);
```

Draws a polygon outline.

**Parameters:**
- `ctx` - Drawing context
- `puncta` - Array of vertices (x,y pairs)
- `numerus_punctorum` - Number of vertices
- `color` - RGBA8888 color

**Notes:** Array format is `[x0, y0, x1, y1, x2, y2, ...]`. Automatically closes the polygon.

### delineare_polygonum_plenum

```c
vacuum delineare_polygonum_plenum(
    ContextusDelineandi* ctx,
    constans i32* puncta,
    i32 numerus_punctorum,
    i32 color
);
```

Draws a filled polygon.

**Parameters:**
- `ctx` - Drawing context
- `puncta` - Array of vertices (x,y pairs)
- `numerus_punctorum` - Number of vertices
- `color` - RGBA8888 color

**Notes:** Uses scanline filling. Handles convex and concave polygons.

## Utilities

### delineare_vacare

```c
vacuum delineare_vacare(
    ContextusDelineandi* ctx,
    i32 color
);
```

Clears the entire buffer.

**Parameters:**
- `ctx` - Drawing context
- `color` - RGBA8888 color

**Notes:** Temporarily disables clipping, fills buffer, restores clipping.

### delineare_cratem

```c
vacuum delineare_cratem(
    ContextusDelineandi* ctx,
    i32 spatium,
    i32 color
);
```

Draws a grid.

**Parameters:**
- `ctx` - Drawing context
- `spatium` - Spacing between grid lines
- `color` - RGBA8888 color

**Notes:** Draws vertical and horizontal lines at regular intervals.

---

# Frequently Asked Questions

## Why Bresenham?

Because it works and it's fast. Bresenham's algorithm from 1962 is still the standard for line rasterization. It uses only integer addition and subtraction - no floating point, no division, no multiplication in the inner loop.

Modern graphics cards use Bresenham (or variants) internally. It's been optimized to death over 60 years and nothing beats it for what it does.

## Why No Anti-Aliasing?

Anti-aliasing makes pixels fuzzy to create the illusion of smooth edges. It requires partial transparency and blending.

This library draws solid pixels. No blending, no transparency (alpha is always 255). Adding anti-aliasing would require:
1. Reading existing pixel values
2. Calculating coverage percentages
3. Blending colors
4. Writing results

That's 3-4x slower than just writing a pixel. For pixel art and retro graphics, you don't want anti-aliasing anyway - you want crisp pixel-perfect shapes.

If you need anti-aliasing, render at higher resolution and scale down. Or use a different library.

## Why Integer Coordinates Only?

Because we're drawing to pixels, which are discrete. Floating-point coordinates add complexity and cost for minimal benefit.

If you need sub-pixel positioning, multiply your coordinates by 16 or 256, do your calculations in that space, then divide when drawing. That's what game engines did in the 90s.

## What About Thick Lines?

The library draws 1-pixel-wide lines. For thick lines, you have options:

1. Draw multiple parallel lines
2. Draw a filled rectangle rotated to the line angle
3. Use Xiaolin Wu's algorithm (requires anti-aliasing support)
4. Live with 1-pixel lines

Most use cases don't need thick lines. For those that do, option 1 or 2 usually works.

## Can I Draw Bezier Curves?

Not directly. The library has primitives - lines, circles, polygons. Bezier curves are built from many short line segments.

To draw a Bezier curve:
1. Sample the curve at regular intervals
2. Draw lines between consecutive points
3. More samples = smoother curve

This is how everyone does it. Even hardware renderers tessellate curves into line segments.

## Why Scanline Filling Instead of Flood Fill?

Flood fill is recursive and unpredictable. Worst case, it can overflow the stack for large areas.

Scanline filling is deterministic. It processes each row once, top to bottom. You know exactly how long it will take based on the shape's bounding box.

For filled shapes with known boundaries (triangles, polygons), scanline filling is always the right choice.

## What's the Deal With Pattern 19 (EXEMPLAR_ALBUS)?

It's all zeros - no bits set. In pattern mode, this means no pixels are drawn. It's effectively transparent.

Why have it? For completeness with the classic Mac patterns. Pattern 19 was white on the Mac (draw nothing). Pattern 0 was black (draw everything).

In practice, you'd use solid mode with a white color instead of pattern mode with pattern 19. But the option is there.

## Why 38 Patterns?

Because that's how many the original Macintosh had. The patterns are from "Inside Macintosh" (1985).

They were designed to give varied textures with only 8×8 bits. Limited by the hardware of the time, but still useful today for retro aesthetics or memory-constrained environments.

You can create custom patterns if you need different ones. The 38 built-in patterns are just convenient defaults.

## Can I Rotate Shapes?

Not directly. The library draws axis-aligned shapes (rectangles with horizontal/vertical sides, circles, etc.).

For rotated shapes:
1. Transform vertex coordinates yourself (2D rotation matrix)
2. Draw the polygon with transformed vertices

This is true for any 2D graphics library. Rotation is a coordinate transform, not a drawing primitive.

## Why No Gradients?

Gradients require interpolating colors across pixels. That means:
1. Color space conversions
2. Per-pixel blending calculations
3. More complex fill logic

This library is about primitives - solid colors, patterns, XOR. Gradients are a higher-level feature.

To fake a gradient, draw many thin rectangles with slightly different colors. It's how gradients were done before hardware acceleration.

## What About Texture Mapping?

Texture mapping (drawing images warped onto shapes) is complex. It requires:
1. Perspective correction
2. Texture coordinate interpolation
3. Sampling and filtering

That's beyond the scope of this library. If you need texture mapping, you're building a 3D rasterizer, which is a whole different project.

For 2D sprites, just blit the image directly. For affine transformations (scale/rotate without perspective), transform the corners and render as a textured polygon using a different library or custom code.

## How Do I Draw Text?

Use FENESTRA's text rendering:

```c
chorda textus = chorda_ex_literis("Hello", piscina);
tabula_pixelorum_pingere_chordam(tabula, x, y, textus, color);
```

DELINEARE is for shapes. FENESTRA handles text. They work together - draw shapes with DELINEARE, overlay text with FENESTRA.

## Can I Mix DELINEARE and Direct Pixel Access?

Yes. DELINEARE just writes to the pixel buffer. You can use FENESTRA's `tabula_pixelorum_ponere_pixelum()` for some pixels and DELINEARE for shapes.

Just remember: DELINEARE respects the context state (clipping, patterns, mode). Direct pixel access bypasses all that. Mix them carefully.

## Why Is There No delineare_destruere()?

Because the context is allocated from a pool. You don't free individual allocations - you destroy the pool.

When you call `piscina_destruere()`, everything allocated from that pool is freed at once. No need for per-object destructors.

This is the pool allocation philosophy. Read the FENESTRA docs or the Piscina docs for more on this.

---

You've now got lines, shapes, patterns, clipping, and XOR mode. Go draw something. And if your shapes look ugly, that's not a bug - it's a feature. Pixel graphics are supposed to look blocky. Embrace it.

