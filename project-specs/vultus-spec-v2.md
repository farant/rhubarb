# Vultus - Visual Identity & Design System (v2 Implementation Spec)

## Overview

Vultus defines the visual identity and design constraints for Lapideus. This v2 spec adds detailed implementation planning based on codebase exploration.

## Core Constraints (Codified)

### Display Resolution
- **Canvas**: 720x480 pixels (16:9 aspect ratio)
- **Scaling**: Stretch to fit window via `fenestra_praesentare_pixela()`
- **Buffer**: `TabulaPixelorum` with RGBA8888 format (`i32* pixela`)

### The Aquinas Palette (16 Colors)

From `thema.c`, the palette uses 6-bit values scaled to 8-bit:

| Index | Name | 6-bit RGB | Scaled 8-bit | Usage |
|-------|------|-----------|--------------|-------|
| 0 | PALETTE_BLACK | 0x00,0x00,0x00 | #000000 | Pure black |
| 1 | PALETTE_DARK_GRAY | 0x0D,0x11,0x09 | #232309 | Primary text (COLOR_TEXT) |
| 2 | PALETTE_MEDIUM_DARK_GRAY | 0x1F,0x17,0x1F | #7C5C7C | Selection, borders |
| 3 | PALETTE_MEDIUM_GRAY | 0x32,0x2E,0x28 | #C8B8A0 | Comments, status |
| 4 | PALETTE_LIGHT_GRAY | 0x3A,0x36,0x30 | #E8D8C0 | Operators |
| 5 | PALETTE_WHITE | 0x3F,0x3C,0x3A | #FCF0E8 | Bright text |
| 6 | PALETTE_DARK_RED | 0x20,0x08,0x08 | #802020 | Keywords |
| 7 | PALETTE_BLUE | 0x18,0x2F,0x3F | #60BCFC | Steel blue |
| 8 | PALETTE_BRIGHT_PINK | 0x3F,0x06,0x1C | #FC1870 | Errors, properties |
| 9 | PALETTE_DARK_GOLD | 0x29,0x1A,0x00 | #A46800 | Strings |
| 10 | PALETTE_MEDIUM_GOLD | 0x3A,0x2D,0x00 | #E8B400 | Numbers, warnings |
| 11 | PALETTE_BRIGHT_GOLD | 0x3F,0x36,0x10 | #FCD840 | Cursor, commands |
| 12 | PALETTE_DARK_LEAF | 0x10,0x20,0x08 | #408020 | Active borders, links |
| 13 | PALETTE_MEDIUM_LEAF | 0x20,0x30,0x10 | #80C040 | Medium green |
| 14 | PALETTE_BRIGHT_LEAF | 0x28,0x3C,0x18 | #A0F060 | Accent, success |
| 15 | PALETTE_WARM_GRAY | 0x2D,0x25,0x1F | #B4947C | Background |

**Scaling formula**: `rgb_8bit = (rgb_6bit * 255) / 63`

### Semantic Color Mapping

From `thema.h`, semantic names map to palette indices:

```c
COLOR_BACKGROUND       -> PALETTE_WARM_GRAY (15)
COLOR_TEXT            -> PALETTE_DARK_GRAY (1)
COLOR_TEXT_DIM        -> PALETTE_MEDIUM_DARK_GRAY (2)
COLOR_CURSOR          -> PALETTE_BRIGHT_GOLD (11)
COLOR_SELECTION       -> PALETTE_MEDIUM_DARK_GRAY (2)
COLOR_BORDER          -> PALETTE_MEDIUM_DARK_GRAY (2)
COLOR_BORDER_ACTIVE   -> PALETTE_DARK_LEAF (12)
COLOR_ACCENT_PRIMARY  -> PALETTE_BRIGHT_LEAF (14)
COLOR_ACCENT_SECONDARY-> PALETTE_BRIGHT_GOLD (11)
COLOR_ERROR           -> PALETTE_BRIGHT_PINK (8)
COLOR_WARNING         -> PALETTE_MEDIUM_GOLD (10)
COLOR_SUCCESS         -> PALETTE_BRIGHT_LEAF (14)
```

### Typography

- **Font**: 6x8 pixel monospace bitmap (`fons_6x8.h`)
- **Sizes**: 1x (6x8), 2x (12x16), optionally 3x/4x
- **Storage**: 256-char lookup, 8 bytes per glyph (bit 7 = leftmost pixel)
- **Rendering**: `tabula_pixelorum_pingere_chordam()`, `_scalatam()` variants

## Dithering Pipeline Implementation

### New Files

```
include/dithering.h         # Dithering API
lib/dithering.c             # Implementation
probationes/probatio_dithering.c
```

### Core Structures

```c
/* dithering.h */

nomen enumeratio {
    DITHERING_FLOYD_STEINBERG,
    DITHERING_ATKINSON,
    DITHERING_BAYER_4X4,
    DITHERING_BAYER_8X8,
    DITHERING_NULLUM           /* No dithering, nearest color */
} ModusDithering;

nomen structura {
    i8* pixela;               /* RGB input (3 bytes per pixel) */
    i32 latitudo;
    i32 altitudo;
} ImaginisInput;

nomen structura {
    i8* indices;              /* Palette indices (1 byte per pixel) */
    i32 latitudo;
    i32 altitudo;
} ImaginisOutput;

/* Main API */
ImaginisOutput* dithering_applicare(
    Piscina*        piscina,
    ImaginisInput*  input,
    ModusDithering  modus);

/* Render dithered image to pixel buffer */
vacuum dithering_reddere(
    TabulaPixelorum* tabula,
    ImaginisOutput*  imago,
    i32              x,
    i32              y,
    i32              scala);
```

### Floyd-Steinberg Implementation

Based on `dithering-example.js`, adapted for 16-color palette:

```c
/* dithering.c */

/* Error diffusion kernel (Floyd-Steinberg) */
hic_manens constans i32 fs_offset_x[] = { 1,  2, -1, 0, 1, 0 };
hic_manens constans i32 fs_offset_y[] = { 0,  0,  1, 1, 1, 2 };
hic_manens constans f32 fs_weights[]  = { 7.0f/16, 1.0f/16, 3.0f/16,
                                          5.0f/16, 1.0f/16, 1.0f/16 };

/* Find nearest palette color using Euclidean distance in RGB space */
hic_manens i32
_invenire_proximum_colorem(i32 r, i32 g, i32 b)
{
    i32 optimus_index = 0;
    i32 minima_distantia = INT_MAX;

    per (i32 i = 0; i < XVI; i++)
    {
        Color c = color_ex_palette(i);
        i32 dr = r - (i32)(i8)c.r;
        i32 dg = g - (i32)(i8)c.g;
        i32 db = b - (i32)(i8)c.b;
        i32 dist = dr*dr + dg*dg + db*db;

        si (dist < minima_distantia)
        {
            minima_distantia = dist;
            optimus_index = i;
        }
    }
    redde optimus_index;
}

/* Floyd-Steinberg with 3-row sliding window */
ImaginisOutput*
dithering_floyd_steinberg(Piscina* piscina, ImaginisInput* input)
{
    i32 lat = input->latitudo;
    i32 alt = input->altitudo;

    ImaginisOutput* output = piscina_allocare(piscina, sizeof(ImaginisOutput));
    output->indices = piscina_allocare(piscina, (magnitudo)(lat * alt));
    output->latitudo = lat;
    output->altitudo = alt;

    /* Error accumulation buffers (3 rows, RGB per pixel) */
    f32* error_rows[III];
    per (i32 i = 0; i < III; i++)
        error_rows[i] = piscina_allocare(piscina,
            (magnitudo)(lat * III * sizeof(f32)));

    per (i32 y = 0; y < alt; y++)
    {
        per (i32 x = 0; x < lat; x++)
        {
            i32 idx = (y * lat + x) * III;

            /* Get pixel + accumulated error */
            f32 r = (f32)(i8)input->pixela[idx + 0] + error_rows[0][x * III + 0];
            f32 g = (f32)(i8)input->pixela[idx + 1] + error_rows[0][x * III + 1];
            f32 b = (f32)(i8)input->pixela[idx + 2] + error_rows[0][x * III + 2];

            /* Clamp */
            r = r < 0 ? 0 : (r > 255 ? 255 : r);
            g = g < 0 ? 0 : (g > 255 ? 255 : g);
            b = b < 0 ? 0 : (b > 255 ? 255 : b);

            /* Find nearest palette color */
            i32 palette_idx = _invenire_proximum_colorem((i32)r, (i32)g, (i32)b);
            output->indices[y * lat + x] = (i8)palette_idx;

            /* Calculate error */
            Color chosen = color_ex_palette(palette_idx);
            f32 err_r = r - (f32)(i8)chosen.r;
            f32 err_g = g - (f32)(i8)chosen.g;
            f32 err_b = b - (f32)(i8)chosen.b;

            /* Distribute error to neighbors */
            per (i32 k = 0; k < VI; k++)
            {
                i32 nx = x + fs_offset_x[k];
                i32 ny = fs_offset_y[k];  /* Relative to current row */

                si (nx >= 0 && nx < lat && ny < III)
                {
                    error_rows[ny][nx * III + 0] += err_r * fs_weights[k];
                    error_rows[ny][nx * III + 1] += err_g * fs_weights[k];
                    error_rows[ny][nx * III + 2] += err_b * fs_weights[k];
                }
            }
        }

        /* Shift error rows (row 0 is done, row 1 becomes row 0, etc.) */
        f32* temp = error_rows[0];
        error_rows[0] = error_rows[1];
        error_rows[1] = error_rows[2];
        error_rows[2] = temp;

        /* Clear the new row 2 */
        per (i32 i = 0; i < lat * III; i++)
            error_rows[2][i] = 0.0f;
    }

    redde output;
}
```

### Image Loading (New)

Need to add PNG/JPEG loading. Minimal approach:

```c
/* imago.h */

nomen structura {
    i8* pixela;           /* RGB or RGBA */
    i32 latitudo;
    i32 altitudo;
    i32 canales;          /* 3 = RGB, 4 = RGBA */
} Imago;

/* Load from file */
Imago* imago_caricare_ex_archivo(Piscina* piscina, constans character* via);

/* Load from memory (for clipboard) */
Imago* imago_caricare_ex_memoria(Piscina* piscina, constans i8* data, magnitudo longitudo);

/* Free (if not using piscina) */
vacuum imago_liberare(Imago* imago);
```

Options for implementation:
1. **stb_image.h** - Single-header, easy to integrate, C89 compatible
2. **Hand-written PNG decoder** - More control, no dependencies
3. **Platform APIs** - NSImage on macOS (requires Objective-C bridge)

Recommendation: Use stb_image.h for v1, potentially replace later.

## Paint App Widget

### New Files

```
include/pictor.h            # Paint app header
lib/pictor.c                # Implementation
lib/pictor_instrumenta.c    # Drawing tools
```

### Core Structure

```c
/* pictor.h */

nomen enumeratio {
    INSTRUMENTUM_PENICULUS,       /* Pencil/brush */
    INSTRUMENTUM_PLENUM,          /* Fill bucket */
    INSTRUMENTUM_LINEA,           /* Line tool */
    INSTRUMENTUM_RECTANGULUM,     /* Rectangle */
    INSTRUMENTUM_ELLIPSIS,        /* Ellipse */
    INSTRUMENTUM_SELECTIO,        /* Selection rectangle */
    INSTRUMENTUM_TEXTUS,          /* Text tool */
    INSTRUMENTUM_PIPETTA          /* Color picker */
} InstrumentumPictoris;

nomen structura {
    i8* pixela;                   /* Palette indices, not RGB */
    i32 latitudo;
    i32 altitudo;
} StratumPictoris;               /* Single layer */

nomen structura {
    ContextusWidget*      ctx;
    Piscina*              piscina;

    /* Canvas */
    StratumPictoris*      strata[XVI];   /* Up to 16 layers */
    i32                   numerus_stratorum;
    i32                   stratum_currens;

    /* Tool state */
    InstrumentumPictoris  instrumentum;
    i8                    color_primarius;    /* Palette index */
    i8                    color_secundarius;  /* Palette index */
    i32                   magnitudo_peniculi; /* Brush size */

    /* Viewport (for pan/zoom within canvas) */
    i32                   visio_x;
    i32                   visio_y;
    i32                   scala_visio;        /* 1, 2, 4, 8... */

    /* Selection */
    i32                   selectio_x, selectio_y;
    i32                   selectio_lat, selectio_alt;
    b32                   habet_selectionem;

    /* Undo history */
    StratumPictoris*      historia[XXXII];
    i32                   historia_index;
    i32                   historia_numerus;

    /* Drawing state */
    b32                   delineans;          /* Currently drawing */
    i32                   cursor_x, cursor_y;

    /* Widget bounds (saved during render) */
    i32                   widget_x, widget_y;
    i32                   widget_lat, widget_alt;
    i32                   widget_scala;
} Pictor;

/* Lifecycle */
Pictor* pictor_creare(ContextusWidget* ctx);
vacuum  pictor_destruere(Pictor* pictor);

/* Rendering (widget interface) */
vacuum pictor_reddere(
    Pictor*          pictor,
    TabulaPixelorum* tabula,
    i32 x, i32 y, i32 lat, i32 alt,
    i32 scala, b32 focused);

/* Events */
b32 pictor_tractare_eventum(Pictor* pictor, constans Eventus* eventus);

/* File I/O */
vacuum pictor_salvare(Pictor* pictor, constans character* via);
vacuum pictor_caricare(Pictor* pictor, constans character* via);

/* Import dithered image */
vacuum pictor_importare_imaginem(Pictor* pictor, constans character* via);
```

### Rendering Implementation

```c
/* pictor.c */

vacuum
pictor_reddere(
    Pictor*          pictor,
    TabulaPixelorum* tabula,
    i32 x, i32 y, i32 lat, i32 alt,
    i32 scala, b32 focused)
{
    /* Save widget bounds */
    pictor->widget_x = x;
    pictor->widget_y = y;
    pictor->widget_lat = lat;
    pictor->widget_alt = alt;
    pictor->widget_scala = scala;

    i32 char_lat = VI * scala;
    i32 char_alt = VIII * scala;
    i32 px_x = x * char_lat;
    i32 px_y = y * char_alt;
    i32 px_lat = lat * char_lat;
    i32 px_alt = alt * char_alt;

    ContextusDelineandi* ctx = delineare_creare_contextum(
        pictor->piscina, tabula);

    /* Background */
    delineare_rectangulum_plenum(ctx, px_x, px_y, px_lat, px_alt,
        color_ad_pixelum(thema_color(COLOR_BACKGROUND)));

    /* Border */
    u32 color_border = focused
        ? color_ad_pixelum(thema_color(COLOR_BORDER_ACTIVE))
        : color_ad_pixelum(thema_color(COLOR_BORDER));
    delineare_rectangulum(ctx, px_x, px_y, px_lat, px_alt, color_border);

    /* Canvas area (inside border) */
    i32 canvas_x = px_x + char_lat;
    i32 canvas_y = px_y + char_alt;
    i32 canvas_lat = px_lat - char_lat * II;
    i32 canvas_alt = px_alt - char_alt * III;  /* Room for toolbar */

    /* Render layers bottom-up */
    per (i32 s = 0; s < pictor->numerus_stratorum; s++)
    {
        StratumPictoris* stratum = pictor->strata[s];
        si (!stratum) perge;

        _reddere_stratum(tabula, stratum,
            canvas_x, canvas_y, canvas_lat, canvas_alt,
            pictor->visio_x, pictor->visio_y,
            pictor->scala_visio);
    }

    /* Selection rectangle */
    si (pictor->habet_selectionem)
    {
        _reddere_selectionem(ctx, pictor, canvas_x, canvas_y);
    }

    /* Toolbar at bottom */
    _reddere_toolbar(pictor, tabula, ctx,
        px_x, px_y + px_alt - char_alt * II,
        px_lat, char_alt * II, scala);

    /* Cursor preview */
    si (focused)
    {
        _reddere_cursor(ctx, pictor, canvas_x, canvas_y);
    }
}

hic_manens vacuum
_reddere_stratum(
    TabulaPixelorum*  tabula,
    StratumPictoris*  stratum,
    i32 dest_x, i32 dest_y,
    i32 dest_lat, i32 dest_alt,
    i32 src_x, i32 src_y,
    i32 zoom)
{
    per (i32 sy = 0; sy < dest_alt / zoom; sy++)
    {
        per (i32 sx = 0; sx < dest_lat / zoom; sx++)
        {
            i32 canvas_x = src_x + sx;
            i32 canvas_y = src_y + sy;

            si (canvas_x < 0 || canvas_x >= stratum->latitudo) perge;
            si (canvas_y < 0 || canvas_y >= stratum->altitudo) perge;

            i8 palette_idx = stratum->pixela[canvas_y * stratum->latitudo + canvas_x];
            si (palette_idx < 0) perge;  /* Transparent */

            Color color = color_ex_palette(palette_idx);
            i32 pixel = color_ad_pixelum(color);

            /* Draw zoomed pixel */
            per (i32 zy = 0; zy < zoom; zy++)
            {
                per (i32 zx = 0; zx < zoom; zx++)
                {
                    i32 dx = dest_x + sx * zoom + zx;
                    i32 dy = dest_y + sy * zoom + zy;
                    si (dx >= 0 && dx < tabula->latitudo &&
                        dy >= 0 && dy < tabula->altitudo)
                    {
                        tabula->pixela[dy * tabula->latitudo + dx] = pixel;
                    }
                }
            }
        }
    }
}
```

## Screen Overlay Layers

### Integration with Schirmata

```c
/* In schirmata.h - add overlay support */

nomen structura {
    /* ... existing fields ... */

    /* Per-screen overlay layers */
    StratumPictoris* stratum_fundus;     /* Background layer */
    StratumPictoris* stratum_superficies; /* Foreground layer */
    b32              modus_pictoris;      /* Edit mode active */
} Schirma;

/* In schirmata.c */

vacuum
schirmata_reddere(Schirmata* schirmata, TabulaPixelorum* tabula)
{
    Schirma* schirma = &schirmata->schirmae[schirmata->index_currens];

    /* 1. Render background overlay (if exists) */
    si (schirma->stratum_fundus)
    {
        _reddere_stratum_overlay(tabula, schirma->stratum_fundus, 0, 0);
    }

    /* 2. Render widgets normally */
    manager_widget_reddere(schirmata->manager, tabula, schirmata->scala);

    /* 3. Render foreground overlay (if exists) */
    si (schirma->stratum_superficies)
    {
        _reddere_stratum_overlay(tabula, schirma->stratum_superficies, 0, 0);
    }

    /* 4. If in edit mode, show edit UI */
    si (schirma->modus_pictoris)
    {
        _reddere_edit_overlay_ui(schirmata, tabula);
    }
}
```

## 3D Software Rasterizer

### New Files

```
include/rasterizator.h      # 3D rendering API
lib/rasterizator.c          # Core rasterizer
lib/rasterizator_math.c     # Vector/matrix math
include/modellum.h          # 3D model structures
lib/modellum.c              # Model loading (OBJ)
```

### Core Structures

```c
/* rasterizator.h */

nomen structura { f32 x, y, z; } Vec3;
nomen structura { f32 x, y, z, w; } Vec4;
nomen structura { f32 m[IV][IV]; } Mat4;

nomen structura {
    Vec3 positio;
    Vec3 normalis;
    f32  u, v;              /* Texture coords */
} Vertex;

nomen structura {
    i32 v[III];             /* Vertex indices */
} Facies;

nomen structura {
    Vertex*  vertices;
    i32      numerus_verticum;
    Facies*  facies;
    i32      numerus_facierum;
    i8*      textura;       /* Palette indices, or NULL for flat shading */
    i32      textura_lat;
    i32      textura_alt;
} Modellum;

nomen enumeratio {
    MODUS_LINEA_ARTIS,      /* Wireframe + cell shading */
    MODUS_SOLIDUS,          /* Flat shaded polygons */
    MODUS_TEXTURATUS,       /* With textures */
    MODUS_PS1               /* PS1 style with affine warping */
} ModusReddendi;

nomen structura {
    TabulaPixelorum* tabula;
    f32*             z_buffer;      /* Depth buffer */
    i32              latitudo;
    i32              altitudo;

    /* Camera */
    Vec3             camera_positio;
    Vec3             camera_directio;
    f32              fov;

    /* Lighting (simple directional) */
    Vec3             lux_directio;

    /* Render mode */
    ModusReddendi    modus;

    /* Cell shading colors (palette indices) */
    i8               color_lux;     /* Lit color */
    i8               color_umbra;   /* Shadow color */

    /* Dithering for gradients */
    b32              dithering_activum;
} ContextusRasterizatoris;

/* API */
ContextusRasterizatoris* rasterizator_creare(
    Piscina* piscina,
    TabulaPixelorum* tabula);

vacuum rasterizator_vacare(ContextusRasterizatoris* ctx);

vacuum rasterizator_reddere_modellum(
    ContextusRasterizatoris* ctx,
    Modellum*                modellum,
    Mat4*                    transformatio);
```

### Triangle Rasterization

```c
/* rasterizator.c */

/* Scanline triangle rasterization */
hic_manens vacuum
_reddere_triangulum(
    ContextusRasterizatoris* ctx,
    Vec4* v0, Vec4* v1, Vec4* v2,
    f32 lum0, f32 lum1, f32 lum2)  /* Per-vertex lighting */
{
    /* Sort vertices by Y */
    si (v0->y > v1->y) { Vec4* t = v0; v0 = v1; v1 = t; }
    si (v0->y > v2->y) { Vec4* t = v0; v0 = v2; v2 = t; }
    si (v1->y > v2->y) { Vec4* t = v1; v1 = v2; v2 = t; }

    i32 y0 = (i32)v0->y;
    i32 y1 = (i32)v1->y;
    i32 y2 = (i32)v2->y;

    /* Scanline rasterization */
    per (i32 y = y0; y <= y2; y++)
    {
        si (y < 0 || y >= ctx->altitudo) perge;

        /* Calculate X bounds for this scanline */
        f32 x_left, x_right, z_left, z_right, lum_left, lum_right;
        _calcular_scanline_bounds(v0, v1, v2, y,
            &x_left, &x_right, &z_left, &z_right,
            &lum_left, &lum_right, lum0, lum1, lum2);

        si (x_left > x_right)
        {
            f32 t;
            t = x_left; x_left = x_right; x_right = t;
            t = z_left; z_left = z_right; z_right = t;
            t = lum_left; lum_left = lum_right; lum_right = t;
        }

        /* Render scanline */
        per (i32 x = (i32)x_left; x <= (i32)x_right; x++)
        {
            si (x < 0 || x >= ctx->latitudo) perge;

            /* Interpolate Z and lighting */
            f32 t = (x_right - x_left) > 0.001f
                ? ((f32)x - x_left) / (x_right - x_left)
                : 0.0f;
            f32 z = z_left + t * (z_right - z_left);
            f32 lum = lum_left + t * (lum_right - lum_left);

            /* Z-buffer test */
            i32 idx = y * ctx->latitudo + x;
            si (z < ctx->z_buffer[idx])
            {
                ctx->z_buffer[idx] = z;

                /* Cell shading: threshold lighting */
                i8 color_idx;
                si (lum > 0.5f)
                    color_idx = ctx->color_lux;
                alioquin si (ctx->dithering_activum && lum > 0.3f)
                {
                    /* Dither between light and shadow */
                    i32 pattern = (x + y) % II;
                    color_idx = pattern ? ctx->color_lux : ctx->color_umbra;
                }
                alioquin
                    color_idx = ctx->color_umbra;

                Color c = color_ex_palette(color_idx);
                ctx->tabula->pixela[idx] = color_ad_pixelum(c);
            }
        }
    }
}
```

## Pixel Editor Widget

Separate from paint app, focused on sprite/tile/animation:

```c
/* editor_pixelorum.h */

nomen structura {
    i32 latitudo;
    i32 altitudo;
    i32 numerus_framium;
    i8* framia;              /* [frame][y][x] palette indices */
    i32 duratio_ms;          /* Animation frame duration */
} Animatio;

nomen structura {
    ContextusWidget*  ctx;
    Piscina*          piscina;

    /* Current sprite/animation */
    Animatio*         animatio;
    i32               framium_currens;

    /* Tools (similar to Pictor) */
    InstrumentumPictoris instrumentum;
    i8                color_primarius;
    i8                color_secundarius;

    /* Onion skinning */
    b32               onion_skin_activum;
    i32               onion_skin_framia;

    /* Grid overlay */
    b32               grid_activum;
    i32               grid_magnitudo;

    /* Animation playback */
    b32               ludens;
    i64               tempus_ultimi_framii;
} EditorPixelorum;
```

## File Format

### Vultus Image Format (.vim)

Simple binary format for palette-indexed images:

```c
/* File header */
structura VimHeader {
    i8  magica[IV];          /* "VIM\0" */
    i16 versio;              /* 1 */
    i16 latitudo;
    i16 altitudo;
    i16 numerus_stratorum;
    i16 numerus_framium;     /* For animation */
    i16 duratio_ms;          /* Frame duration */
    i8  reservatum[XVI];
};

/* Per-layer data follows: latitudo * altitudo bytes of palette indices */
/* -1 = transparent */
```

## Implementation Order

### Phase 1: Dithering Pipeline (First Milestone)

1. Add stb_image.h for image loading
2. Implement `dithering.h/.c` with Floyd-Steinberg
3. Create simple viewer widget to test
4. Add clipboard support (macOS pasteboard API)

### Phase 2: Paint App Basics

1. Implement `pictor.h/.c` core structure
2. Basic tools: pencil, fill, line
3. Single layer support
4. File save/load (.vim format)

### Phase 3: Screen Overlays

1. Add overlay fields to Schirma
2. Implement toggle into edit mode
3. Persistence with schirmata state

### Phase 4: Pixel Editor

1. Implement `editor_pixelorum.h/.c`
2. Animation timeline
3. Onion skinning
4. Tile mode (repeating preview)

### Phase 5: 3D Rasterizer

1. Implement `rasterizator.h/.c` core
2. OBJ model loading
3. Cell shading with two colors
4. Dithered lighting option

### Phase 6: 3D Diorama Tool

1. Scene composition widget
2. Model positioning/rotation UI
3. Camera controls
4. Export to .vim bitmap

## Open Questions

1. **stb_image licensing** - Public domain, should be fine
2. **Clipboard format** - PNG data or raw bitmap?
3. **3D model import validation** - How to enforce poly limits?
4. **Distressed borders** - Pre-made masks or procedural?
5. **HyperCard behaviors** - Full Sputnik or simpler command binding?

## Related Files

- `thema.c` - Aquinas palette definition (lines 1-100)
- `delineare.h/.c` - Drawing primitives (existing patterns/dithering)
- `fons_6x8.h` - Bitmap font
- `fenestra.h` - TabulaPixelorum and window API
- `widget.h/.c` - Widget registration and rendering lifecycle
- `dithering-example.js` - Reference Floyd-Steinberg implementation
