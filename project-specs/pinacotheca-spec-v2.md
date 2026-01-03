# Pinacotheca - Implementation Spec (v2)

## Overview

This spec details the implementation of the Pinacotheca image gallery and dithering pipeline, based on codebase exploration and the interview brainstorm.

## Entity Storage

### Binary Blob Support

The current entity system stores all properties as `chorda*` (interned strings). Binary image data requires an extension.

**Recommended Approach: Base64 Encoding**

Use the existing `base64.h` library to encode/decode binary data:

```c
/* include/vultus_imago.h */

/* Salvare imaginem (compress + encode) */
b32
vultus_imago_salvare(
    EntitasRepositorium* repo,
    chorda*              titulus,
    const i8*            indices,    /* Palette index per pixel */
    i32                  latitudo,
    i32                  altitudo,
    Piscina*             piscina);

/* Caricare imaginem (decode + decompress) */
b32
vultus_imago_caricare(
    EntitasRepositorium* repo,
    chorda*              titulus,
    i8**                 indices,    /* Output: palette indices */
    i32*                 latitudo,
    i32*                 altitudo,
    Piscina*             piscina);
```

**Storage Pipeline:**
```
Raw indices (1 byte/pixel)
    ↓ flatura_deflare()
Compressed bytes
    ↓ base64_encode()
Base64 chorda* → entity property
```

**Retrieval Pipeline:**
```
Entity property chorda*
    ↓ base64_decode()
Compressed bytes
    ↓ flatura_inflare()
Raw indices (1 byte/pixel)
```

### Entity Structure

```c
/* Genus: Vultus::Imago */
/* Properties: */
/*   titulus: chorda - unique name (used for $image command) */
/*   latitudo: s32 - width in pixels */
/*   altitudo: s32 - height in pixels */
/*   datum: chorda - base64(deflate(palette_indices)) */
/*   creatum: tempus - creation timestamp */
```

**Creation Example:**
```c
Entitas* imago = repo->entitas_scaffoldare(
    repo->datum,
    "Vultus::Imago",
    titulus_slug);

repo->proprietas_ponere(repo->datum, imago, "titulus", titulus);
repo->proprietas_ponere(repo->datum, imago, "latitudo", "360");
repo->proprietas_ponere(repo->datum, imago, "altitudo", "448");
repo->proprietas_ponere(repo->datum, imago, "datum", encoded_data);
```

## Platform Libraries

### Image Loading (imago_platform.h)

Wrap stb_image.h behind platform API:

```c
/* include/imago_platform.h */

nomen structura {
    i8* pixela;       /* RGBA8888 data */
    i32 latitudo;
    i32 altitudo;
    i32 canales;      /* Typically 4 for RGBA */
} ImagoPrima;

/* Caricare ex file (PNG, JPEG, etc.) */
ImagoPrima*
imago_caricare_ex_file(
    const char* via,
    Piscina*    piscina);

/* Caricare ex memoria */
ImagoPrima*
imago_caricare_ex_memoria(
    const i8* datum,
    i32       mensura,
    Piscina*  piscina);

/* Liberare (if not using arena) */
vacuum
imago_liberare(ImagoPrima* imago);
```

**Implementation (lib/imago_platform.c):**
```c
#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_THREAD_LOCALS    /* C89 compatibility */
#include "stb_image.h"

ImagoPrima*
imago_caricare_ex_file(const char* via, Piscina* piscina)
{
    integer latitudo, altitudo, canales;
    i8* datum = (i8*)stbi_load(via, &latitudo, &altitudo, &canales, 4);

    si (!datum) redde NIHIL;

    ImagoPrima* imago = piscina_allocare(piscina, magnitudo(ImagoPrima));
    imago->latitudo = (i32)latitudo;
    imago->altitudo = (i32)altitudo;
    imago->canales = 4;

    /* Copy to arena so stb can free its allocation */
    i32 mensura = imago->latitudo * imago->altitudo * 4;
    imago->pixela = piscina_allocare(piscina, (u32)mensura);
    memcpy(imago->pixela, datum, (u32)mensura);

    stbi_image_free(datum);
    redde imago;
}
```

### Clipboard (clipboard_platform.h)

macOS clipboard access for image paste:

```c
/* include/clipboard_platform.h */

/* Probare si clipboard habet imaginem */
b32
clipboard_habet_imaginem(vacuum);

/* Capere imaginem ex clipboard */
ImagoPrima*
clipboard_capere_imaginem(Piscina* piscina);

/* Future: text clipboard */
/* chorda* clipboard_capere_textum(Piscina* piscina); */
/* vacuum clipboard_ponere_textum(chorda* textus); */
```

**Implementation (lib/clipboard_platform_macos.m):**
```objc
#import <Cocoa/Cocoa.h>
#include "clipboard_platform.h"
#include "piscina.h"

b32
clipboard_habet_imaginem(void)
{
    NSPasteboard* pb = [NSPasteboard generalPasteboard];
    NSArray* types = [pb types];
    return [types containsObject:NSPasteboardTypeTIFF] ||
           [types containsObject:NSPasteboardTypePNG];
}

ImagoPrima*
clipboard_capere_imaginem(Piscina* piscina)
{
    NSPasteboard* pb = [NSPasteboard generalPasteboard];
    NSData* data = [pb dataForType:NSPasteboardTypeTIFF];
    if (!data) data = [pb dataForType:NSPasteboardTypePNG];
    if (!data) return NULL;

    NSBitmapImageRep* rep = [[NSBitmapImageRep alloc] initWithData:data];
    if (!rep) return NULL;

    NSInteger w = [rep pixelsWide];
    NSInteger h = [rep pixelsHigh];

    ImagoPrima* imago = piscina_allocare(piscina, sizeof(ImagoPrima));
    imago->latitudo = (i32)w;
    imago->altitudo = (i32)h;
    imago->canales = 4;

    i32 mensura = imago->latitudo * imago->altitudo * 4;
    imago->pixela = piscina_allocare(piscina, (u32)mensura);

    /* Convert to RGBA */
    for (NSInteger y = 0; y < h; y++) {
        for (NSInteger x = 0; x < w; x++) {
            NSColor* c = [rep colorAtX:x y:y];
            i32 idx = (i32)((y * w + x) * 4);
            imago->pixela[idx + 0] = (i8)([c redComponent] * 255);
            imago->pixela[idx + 1] = (i8)([c greenComponent] * 255);
            imago->pixela[idx + 2] = (i8)([c blueComponent] * 255);
            imago->pixela[idx + 3] = (i8)([c alphaComponent] * 255);
        }
    }

    return imago;
}
```

## Dithering Algorithm

### Core Implementation (dithering.h / dithering.c)

Port the Atkinson dithering from aquinas-dithering-tool.html:

```c
/* include/dithering.h */

/* Aquinas palette - 16 colors */
externus const u32 AQUINAS_PALETTE[XVI];

/* Palette presets */
nomen enumeratio {
    PRAEPARATIO_OMNES,      /* All 16 colors */
    PRAEPARATIO_NULLAE,     /* None */
    PRAEPARATIO_GRISEA,     /* Grayscale: 0,1,3,4,5,15 */
    PRAEPARATIO_CALIDA,     /* Warm: 0,5,6,9,10,11,15 */
    PRAEPARATIO_FRIGIDA,    /* Cool: 0,1,5,7,12,13,14 */
    PRAEPARATIO_MONO        /* Mono: 0,5 */
} PraeparatioPalettae;

/* Dithering status (current slider values) */
nomen structura {
    b32 colores_activi[XVI];  /* Which colors are enabled */
    i32 punctum_nigrum[III];  /* Breakpoint sliders (0-255) */
    f32 scala;                /* Zoom level (1.0 = fit to output) */
    i32 offset_x;             /* Pan offset */
    i32 offset_y;
} StatusDithering;

/* Output from dithering */
nomen structura {
    i8* indices;              /* Palette index per pixel */
    i32 latitudo;
    i32 altitudo;
} ImagoDithered;

/* Initialize default status */
vacuum
dithering_status_initare(StatusDithering* status);

/* Apply preset */
vacuum
dithering_applicare_praeparationem(
    StatusDithering*     status,
    PraeparatioPalettae  praeparatio);

/* Perform dithering (called on every parameter change) */
ImagoDithered*
dithering_processare(
    const ImagoPrima*    origo,
    const StatusDithering* status,
    i32                  output_latitudo,
    i32                  output_altitudo,
    Piscina*             piscina);
```

**Implementation (lib/dithering.c):**

```c
#include "dithering.h"
#include "latina.h"

/* Aquinas palette (from thema.c) */
constans u32 AQUINAS_PALETTE[XVI] = {
    0x000000,  /* 0: Black */
    0x344424,  /* 1: Dark Gray */
    0x7C5C7C,  /* 2: Mauve */
    0xC8B8A0,  /* 3: Medium Gray */
    0xE8D8C0,  /* 4: Light Gray */
    0xFCF0E8,  /* 5: White */
    0x802020,  /* 6: Dark Red */
    0x60BCFC,  /* 7: Blue */
    0xFC1870,  /* 8: Pink */
    0xA46800,  /* 9: Dark Gold */
    0xE8B400,  /* 10: Medium Gold */
    0xFCD840,  /* 11: Bright Gold */
    0x408020,  /* 12: Dark Leaf */
    0x80C040,  /* 13: Medium Leaf */
    0xA0F060,  /* 14: Bright Leaf */
    0xB4947C   /* 15: Warm Gray */
};

/* Atkinson error diffusion offsets */
hic_manens const i32 ATKINSON_OFFSETS[VI][II] = {
    {1, 0}, {2, 0},
    {-1, 1}, {0, 1}, {1, 1},
    {0, 2}
};

/* Find nearest color in active palette */
hic_manens i32
_invenire_proximum_colorem(
    i32                   r,
    i32                   g,
    i32                   b,
    const StatusDithering* status)
{
    i32 optimus_index = 0;
    i32 optima_distantia = 0x7FFFFFFF;

    per (i32 i = 0; i < XVI; i++)
    {
        si (!status->colores_activi[i]) perge;

        u32 color = AQUINAS_PALETTE[i];
        i32 pr = (i32)((color >> 16) & 0xFF);
        i32 pg = (i32)((color >> 8) & 0xFF);
        i32 pb = (i32)(color & 0xFF);

        i32 dr = r - pr;
        i32 dg = g - pg;
        i32 db = b - pb;
        i32 dist = dr * dr + dg * dg + db * db;

        si (dist < optima_distantia)
        {
            optima_distantia = dist;
            optimus_index = i;
        }
    }

    redde optimus_index;
}

ImagoDithered*
dithering_processare(
    const ImagoPrima*      origo,
    const StatusDithering* status,
    i32                    output_latitudo,
    i32                    output_altitudo,
    Piscina*               piscina)
{
    /* Allocate output */
    ImagoDithered* output = piscina_allocare(piscina, magnitudo(ImagoDithered));
    output->latitudo = output_latitudo;
    output->altitudo = output_altitudo;
    output->indices = piscina_allocare(piscina,
        (u32)(output_latitudo * output_altitudo));

    /* Allocate float buffers for error diffusion */
    i32 num_pixels = output_latitudo * output_altitudo;
    f32* buffer_r = piscina_allocare(piscina, (u32)(num_pixels * magnitudo(f32)));
    f32* buffer_g = piscina_allocare(piscina, (u32)(num_pixels * magnitudo(f32)));
    f32* buffer_b = piscina_allocare(piscina, (u32)(num_pixels * magnitudo(f32)));

    /* Sample and scale source image into buffers */
    /* Apply zoom and pan offsets */
    f32 scale_x = (f32)origo->latitudo / ((f32)output_latitudo * status->scala);
    f32 scale_y = (f32)origo->altitudo / ((f32)output_altitudo * status->scala);

    per (i32 y = 0; y < output_altitudo; y++)
    {
        per (i32 x = 0; x < output_latitudo; x++)
        {
            /* Calculate source position with centering */
            f32 center_x = (f32)origo->latitudo / 2.0f;
            f32 center_y = (f32)origo->altitudo / 2.0f;
            f32 src_x = center_x + ((f32)x - (f32)output_latitudo / 2.0f) * scale_x;
            f32 src_y = center_y + ((f32)y - (f32)output_altitudo / 2.0f) * scale_y;

            i32 sx = (i32)src_x;
            i32 sy = (i32)src_y;

            /* Clamp to bounds */
            si (sx < 0) sx = 0;
            si (sy < 0) sy = 0;
            si (sx >= origo->latitudo) sx = origo->latitudo - 1;
            si (sy >= origo->altitudo) sy = origo->altitudo - 1;

            i32 src_idx = (sy * origo->latitudo + sx) * 4;
            i32 dst_idx = y * output_latitudo + x;

            buffer_r[dst_idx] = (f32)(u8)origo->pixela[src_idx + 0];
            buffer_g[dst_idx] = (f32)(u8)origo->pixela[src_idx + 1];
            buffer_b[dst_idx] = (f32)(u8)origo->pixela[src_idx + 2];
        }
    }

    /* Atkinson dithering */
    per (i32 y = 0; y < output_altitudo; y++)
    {
        per (i32 x = 0; x < output_latitudo; x++)
        {
            i32 idx = y * output_latitudo + x;

            i32 old_r = (i32)buffer_r[idx];
            i32 old_g = (i32)buffer_g[idx];
            i32 old_b = (i32)buffer_b[idx];

            /* Clamp */
            si (old_r < 0) old_r = 0;
            si (old_r > 255) old_r = 255;
            si (old_g < 0) old_g = 0;
            si (old_g > 255) old_g = 255;
            si (old_b < 0) old_b = 0;
            si (old_b > 255) old_b = 255;

            /* Find nearest */
            i32 new_idx = _invenire_proximum_colorem(old_r, old_g, old_b, status);
            output->indices[idx] = (i8)new_idx;

            /* Get new color RGB */
            u32 new_color = AQUINAS_PALETTE[new_idx];
            i32 new_r = (i32)((new_color >> 16) & 0xFF);
            i32 new_g = (i32)((new_color >> 8) & 0xFF);
            i32 new_b = (i32)(new_color & 0xFF);

            /* Calculate error (Atkinson divides by 8) */
            f32 err_r = (f32)(old_r - new_r) / 8.0f;
            f32 err_g = (f32)(old_g - new_g) / 8.0f;
            f32 err_b = (f32)(old_b - new_b) / 8.0f;

            /* Diffuse error to neighbors */
            per (i32 i = 0; i < VI; i++)
            {
                i32 nx = x + ATKINSON_OFFSETS[i][0];
                i32 ny = y + ATKINSON_OFFSETS[i][1];

                si (nx >= 0 && nx < output_latitudo &&
                    ny >= 0 && ny < output_altitudo)
                {
                    i32 n_idx = ny * output_latitudo + nx;
                    buffer_r[n_idx] += err_r;
                    buffer_g[n_idx] += err_g;
                    buffer_b[n_idx] += err_b;
                }
            }
        }
    }

    redde output;
}
```

## Modal Dialog System

### Architecture

Following the "Post-Render Overlay" pattern from codebase exploration:

```c
/* include/dialogus.h */

/* Forward declaration */
nomen structura Dialogus;

/* Render callback */
nomen vacuum (*FunctioDialogusReddere)(
    Dialogus*        dialogus,
    TabulaPixelorum* tabula,
    f32              scala);

/* Event callback */
nomen b32 (*FunctioDialogusTractare)(
    Dialogus*        dialogus,
    Eventum*         eventum);

/* Dismiss callback */
nomen vacuum (*FunctioDialogusDimittere)(
    vacuum*          datum,
    b32              salvatum);   /* VERUM if Salvare, FALSUM if Abicere */

nomen structura {
    /* Rendering */
    FunctioDialogusReddere  reddere;
    FunctioDialogusTractare tractare_eventum;

    /* Lifecycle */
    FunctioDialogusDimittere on_dismiss;
    vacuum*                  datum_dismiss;

    /* Layout (character coordinates) */
    i32 x;
    i32 y;
    i32 latitudo;
    i32 altitudo;

    /* Dialog-specific data */
    vacuum* datum;
    Piscina* piscina;
} Dialogus;

/* Create dialog (generic) */
Dialogus*
dialogus_creare(
    i32       latitudo,
    i32       altitudo,
    vacuum*   datum,
    FunctioDialogusReddere  reddere,
    FunctioDialogusTractare tractare,
    Piscina*  piscina);

/* Center dialog in panel */
vacuum
dialogus_centrare(Dialogus* d, i32 panel_latitudo, i32 panel_altitudo);

/* Set dismiss callback */
vacuum
dialogus_ponere_dismiss(
    Dialogus*               d,
    FunctioDialogusDimittere callback,
    vacuum*                  datum);
```

### Integration with Schirmata

```c
/* In schirmata.h */
nomen structura {
    /* ... existing fields ... */
    Dialogus* dialogus_sinister;   /* Modal over left panel */
    Dialogus* dialogus_dexter;     /* Modal over right panel */
} Schirmata;

/* In schirmata.c - rendering */
vacuum
schirmata_reddere(Schirmata* schirmata, TabulaPixelorum* tabula, f32 scala)
{
    /* Render widgets */
    manager_widget_reddere(schirmata->manager, tabula, scala);

    /* Render tab bar */
    _reddere_tabulam_schirmarum(schirmata, tabula, scala);

    /* Render modals on top */
    si (schirmata->dialogus_sinister)
    {
        /* Optional: dim left panel */
        _reddere_obscurationem(tabula, 0, 0, LXXI, LX, scala);
        schirmata->dialogus_sinister->reddere(
            schirmata->dialogus_sinister, tabula, scala);
    }
    si (schirmata->dialogus_dexter)
    {
        _reddere_obscurationem(tabula, LXXI, 0, LXXI, LX, scala);
        schirmata->dialogus_dexter->reddere(
            schirmata->dialogus_dexter, tabula, scala);
    }
}

/* In schirmata.c - event handling */
b32
schirmata_tractare_eventum(Schirmata* schirmata, Eventum* eventum)
{
    /* Modal captures input for its panel */
    si (schirmata->dialogus_sinister &&
        eventum->click_x < LXXI * VI)  /* Left panel click */
    {
        redde schirmata->dialogus_sinister->tractare_eventum(
            schirmata->dialogus_sinister, eventum);
    }

    si (schirmata->dialogus_dexter &&
        eventum->click_x >= LXXI * VI)  /* Right panel click */
    {
        redde schirmata->dialogus_dexter->tractare_eventum(
            schirmata->dialogus_dexter, eventum);
    }

    /* Normal event routing... */
}
```

## Import Dialog (Dithering Dialog)

### Structure

```c
/* include/dialogus_dithering.h */

nomen structura {
    /* Source image */
    ImagoPrima* origo;

    /* Dithering state (live-updated) */
    StatusDithering status;

    /* Current output (regenerated on changes) */
    ImagoDithered* output;

    /* UI State */
    chorda* titulus;              /* Image name (editable) */
    i32     cursor_titulus;       /* Text cursor position */
    b32     titulus_focused;      /* Is name field focused? */

    /* Slider states */
    i32 slider_hover;             /* -1 if none, 0-3 for sliders */
    b32 slider_dragging;

    /* Preview state */
    f32 zoom;
    i32 pan_x;
    i32 pan_y;
    b32 panning;

    /* References */
    Piscina* piscina;
    InternamentumChorda* intern;
    EntitasRepositorium* repo;

    /* Callback when done */
    FunctioDialogusDimittere on_complete;
    vacuum* datum_complete;
} DialogusDitheringDatum;

/* Create import dialog for file */
Dialogus*
dialogus_dithering_creare_ex_file(
    const char*           via,
    EntitasRepositorium*  repo,
    InternamentumChorda*  intern,
    Piscina*              piscina);

/* Create import dialog for clipboard */
Dialogus*
dialogus_dithering_creare_ex_clipboard(
    EntitasRepositorium*  repo,
    InternamentumChorda*  intern,
    Piscina*              piscina);
```

### Dialog Layout (Left Panel)

```
┌─────────────────────────────────────────┐
│ Nomen: [imago_20260102_1430____]        │
├─────────────────────────────────────────┤
│ [All] [Grays] [Warm] [Cool] [Mono]      │
│                                         │
│ ■ ■ ■ ■ ■ ■ ■ ■   (2x8 palette grid)   │
│ ■ ■ ■ ■ ■ ■ ■ ■   (click to toggle)    │
├─────────────────────────────────────────┤
│ Punctum I:   [====●==========] 64       │
│ Punctum II:  [========●======] 128      │
│ Punctum III: [============●==] 192      │
├─────────────────────────────────────────┤
│ Amplificatio: [●=============] 1.0      │
├─────────────────────────────────────────┤
│        [Salvare]     [Abicere]          │
└─────────────────────────────────────────┘
```

### Preview (Right Panel)

```
┌─────────────────────────────────────────┐
│                                         │
│                                         │
│           ┌─────────────────┐           │
│           │                 │           │
│           │  Dithered       │           │
│           │  Preview        │           │
│           │  (drag to pan)  │           │
│           │                 │           │
│           └─────────────────┘           │
│                                         │
│                                         │
└─────────────────────────────────────────┘
```

## Slider Widget

### Implementation

```c
/* include/cursor_slider.h */

nomen structura {
    i32 x;
    i32 y;
    i32 latitudo;        /* In characters */
    i32 valor_min;
    i32 valor_max;
    i32 valor;           /* Current value */
    b32 hover;
    b32 dragging;
} CursorSlider;

/* Render slider */
vacuum
cursor_slider_reddere(
    const CursorSlider* slider,
    TabulaPixelorum*    tabula,
    Thema*              thema,
    f32                 scala);

/* Handle mouse event */
b32
cursor_slider_tractare_eventum(
    CursorSlider* slider,
    Eventum*      eventum,
    f32           scala);
```

**Rendering:**
```c
vacuum
cursor_slider_reddere(
    const CursorSlider* slider,
    TabulaPixelorum*    tabula,
    Thema*              thema,
    f32                 scala)
{
    i32 char_w = VI * (i32)scala;
    i32 char_h = VIII * (i32)scala;

    i32 px = slider->x * char_w;
    i32 py = slider->y * char_h;
    i32 pw = slider->latitudo * char_w;
    i32 ph = char_h;

    /* Draw track */
    u32 track_color = slider->hover ?
        PALETTE_MEDIUM_GRAY : PALETTE_DARK_GRAY;
    delineare_rectangulum_plenum(tabula, px, py + ph/4, pw, ph/2, track_color);

    /* Draw handle */
    f32 ratio = (f32)(slider->valor - slider->valor_min) /
                (f32)(slider->valor_max - slider->valor_min);
    i32 handle_x = px + (i32)(ratio * (f32)(pw - char_w));

    u32 handle_color = slider->dragging ?
        PALETTE_BRIGHT_GOLD : PALETTE_MEDIUM_GOLD;
    delineare_rectangulum_plenum(tabula, handle_x, py, char_w, ph, handle_color);
}
```

## Pinacotheca Widget

### Structure

```c
/* include/pinacotheca_visus.h */

nomen enumeratio {
    PINACOTHECA_MODUS_BROWSER,    /* Split list + preview */
    PINACOTHECA_MODUS_VIEW        /* Full image view */
} PinacothecaModus;

nomen structura {
    ContextusWidget*      ctx;
    Piscina*              piscina;
    InternamentumChorda*  intern;
    EntitasRepositorium*  repo;

    /* Mode */
    PinacothecaModus      modus;

    /* List state */
    Xar*                  imagines;      /* Array of chorda* (tituli) */
    i32                   selectio;      /* Selected index */
    i32                   scroll_offset; /* First visible index */

    /* Current image (decoded for display) */
    chorda*               titulus_currens;
    ImagoDithered*        imago_currens;

    /* Delete confirmation */
    b32                   confirmare_deletionem;

    /* Deep link callback */
    FunctioCommutareWidget commutare_widget;
    vacuum*                schirmata_datum;
} PinacothecaVisus;

/* Create widget */
PinacothecaVisus*
pinacotheca_visus_creare(ContextusWidget* ctx);

/* Navigate to specific image */
vacuum
pinacotheca_visus_navigare_ad(PinacothecaVisus* pv, const char* titulus);

/* Reload image list */
vacuum
pinacotheca_visus_reficere_indicem(PinacothecaVisus* pv);
```

### Browser Mode Layout

```
┌───────────────────────┬───────────────────────┐
│ Imagines              │                       │
│ ───────────────────── │                       │
│ > imago_20260101_1200 │                       │
│   imago_20260102_0900 │      Preview          │
│   sancta_maria        │      of selected      │
│   stella_maris        │      image            │
│                       │                       │
│                       │                       │
│                       │                       │
├───────────────────────┤                       │
│ [Rename] [Delete]     │                       │
└───────────────────────┴───────────────────────┘
```

## Commands

### Registration

```c
/* In pinacotheca_visus.c */
vacuum
pinacotheca_visus_init(ContextusWidget* ctx)
{
    registrum_commandi_registrare(
        ctx->reg_commandi,
        "import-image",
        _command_import_image,
        ctx);

    registrum_commandi_registrare(
        ctx->reg_commandi,
        "image",
        _command_image,
        ctx);

    registrum_commandi_registrare(
        ctx->reg_commandi,
        "pinacotheca",
        _command_pinacotheca,
        ctx);
}

hic_manens b32
_command_import_image(ContextusCommandi* ctx)
{
    ContextusWidget* widget_ctx = (ContextusWidget*)ctx->datum_registratus;

    /* Extract path from text after command */
    /* ... */

    /* Create and show dithering dialog */
    Dialogus* d = dialogus_dithering_creare_ex_file(
        via,
        widget_ctx->repo,
        widget_ctx->intern,
        widget_ctx->piscina);

    /* Show dialog on left panel */
    schirmata_ostendere_dialogum_sinistrum(
        widget_ctx->schirmata_datum, d);

    /* Switch right panel to pinacotheca */
    widget_ctx->commutare_widget(
        widget_ctx->schirmata_datum,
        "pinacotheca",
        NIHIL);

    redde VERUM;
}
```

## File Structure

### New Files to Create

```
include/imago_platform.h           # Image loading API
lib/imago_platform.c               # stb_image wrapper
lib/stb_image.h                    # Single-header library

include/clipboard_platform.h       # Clipboard API
lib/clipboard_platform_macos.m     # macOS implementation

include/dithering.h                # Dithering algorithm
lib/dithering.c                    # Implementation

include/dialogus.h                 # Modal dialog system
lib/dialogus.c                     # Implementation

include/dialogus_dithering.h       # Import dialog
lib/dialogus_dithering.c           # Implementation

include/cursor_slider.h            # Slider UI widget
lib/cursor_slider.c                # Implementation

include/vultus_imago.h             # Image entity helpers
lib/vultus_imago.c                 # Implementation

include/pinacotheca_visus.h        # Gallery widget
lib/pinacotheca_visus.c            # Implementation
```

### Files to Modify

```
include/schirmata.h                # Add dialogus fields
lib/schirmata.c                    # Dialog rendering/events
lib/concha.c                       # Register new commands
compile_*.sh                       # Add new source files
```

## Implementation Order

### Phase 1: Platform Libraries
1. `imago_platform.h/.c` - Image loading with stb_image
2. `clipboard_platform.h/.m` - macOS clipboard access

### Phase 2: Dithering Core
3. `dithering.h/.c` - Atkinson dithering algorithm
4. `vultus_imago.h/.c` - Entity storage with compression

### Phase 3: UI Primitives
5. `cursor_slider.h/.c` - Slider widget
6. `dialogus.h/.c` - Modal dialog framework

### Phase 4: Import Dialog
7. `dialogus_dithering.h/.c` - Full import dialog

### Phase 5: Gallery Widget
8. `pinacotheca_visus.h/.c` - Browser and view modes

### Phase 6: Integration
9. Modify schirmata for dialog support
10. Register commands
11. Wire up Cmd+V detection

## Open Questions Resolved

1. **Binary storage**: Use base64 + flatura compression
2. **Modal dialogs**: Post-render overlay pattern
3. **Slider input**: Mouse-only, no keyboard
4. **Live updates**: No debounce, immediate re-dither
5. **Output size**: ~448px height, widget aspect ratio
6. **Sort order**: Alphabetical by name
