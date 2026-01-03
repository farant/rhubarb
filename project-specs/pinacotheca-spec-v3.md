# Pinacotheca - Implementation Spec (v3)

## Overview

This v3 spec incorporates findings from the implementation simulation and follow-up interview. Key changes from v2:
- New "elementa" immediate-mode GUI component system
- TYPUS_BLOBUM with automatic compression for binary data
- Session entity pattern for cross-widget communication
- quaerere_cum_genere for entity repository

## New Prerequisites

### 1. Elementa - GUI Component Module

Immediate-mode GUI component system.

**Components:**
- Slider (cursor_slider)
- Text input (campus_textus)
- Button (bottone)
- Checkbox/toggle (capsa_optandi)
- Palette grid (graticula_colorum)

**API Pattern (immediate-mode, functional style):**

```c
/* include/elementa.h */

/* ================================================================
 * Slider
 * ================================================================ */

nomen structura {
    i32 valor;
    b32 mutatum;     /* VERUM if value changed this frame */
    b32 hover;
    b32 dragging;
} FructusSlider;

FructusSlider
elementa_slider(
    TabulaPixelorum* tabula,
    Thema*           thema,
    i32              x,           /* Character coordinates */
    i32              y,
    i32              latitudo,    /* Width in characters */
    i32              valor,       /* Current value */
    i32              valor_min,
    i32              valor_max,
    Eventum*         eventum,     /* Current frame's event, or NIHIL */
    f32              scala);

/* ================================================================
 * Text Input
 * ================================================================ */

nomen structura {
    chorda* textus;
    i32     cursor;
    b32     mutatum;
    b32     focused;
} FructusCampusTextus;

FructusCampusTextus
elementa_campus_textus(
    TabulaPixelorum* tabula,
    Thema*           thema,
    i32              x,
    i32              y,
    i32              latitudo,
    chorda*          textus,
    i32              cursor,
    b32              focused,
    Eventum*         eventum,
    Piscina*         piscina,     /* For new text allocation */
    f32              scala);

/* ================================================================
 * Button
 * ================================================================ */

nomen structura {
    b32 clicked;
    b32 hover;
} FructusBottone;

FructusBottone
elementa_bottone(
    TabulaPixelorum* tabula,
    Thema*           thema,
    i32              x,
    i32              y,
    chorda*          label,
    Eventum*         eventum,
    f32              scala);

/* ================================================================
 * Checkbox
 * ================================================================ */

nomen structura {
    b32 valor;
    b32 mutatum;
    b32 hover;
} FructusCapsaOptandi;

FructusCapsaOptandi
elementa_capsa_optandi(
    TabulaPixelorum* tabula,
    Thema*           thema,
    i32              x,
    i32              y,
    chorda*          label,
    b32              valor,
    Eventum*         eventum,
    f32              scala);

/* ================================================================
 * Palette Grid (2x8 toggleable colors)
 * ================================================================ */

nomen structura {
    b32 colores[XVI];    /* Which colors are enabled */
    b32 mutatum;         /* VERUM if any color toggled */
    i32 toggled_index;   /* Which color was toggled, -1 if none */
} FructusGraticulaColorum;

FructusGraticulaColorum
elementa_graticula_colorum(
    TabulaPixelorum* tabula,
    Thema*           thema,
    i32              x,
    i32              y,
    const b32*       colores,     /* Current state array[16] */
    Eventum*         eventum,
    f32              scala);
```

**Usage Example:**
```c
/* In dialog rendering/event loop */
FructusSlider fr = elementa_slider(
    tabula, thema,
    10, 5, 30,           /* x, y, width */
    datum->breakpoint_1, /* current value */
    0, 255,              /* min, max */
    eventum, scala);

si (fr.mutatum)
{
    datum->breakpoint_1 = fr.valor;
    _trigger_redither(datum);
}
```

### 2. Entity System Extensions

**TYPUS_BLOBUM with automatic compression:**

```c
/* In entitas.h - add to TypusLiteralis enum */
TYPUS_BLOBUM = VII,  /* Binary blob, auto-compressed with flatura */

/* New API functions */
b32
entitas_proprietas_ponere_blobum(
    Entitas*     entitas,
    chorda*      clavis,
    const i8*    datum,
    i32          mensura);
/* Internally: compress with flatura, store compressed bytes */

b32
entitas_proprietas_capere_blobum(
    Entitas*     entitas,
    chorda*      clavis,
    i8**         datum,
    i32*         mensura,
    Piscina*     piscina);
/* Internally: decompress with flatura, allocate from piscina */
```

**Genus querying:**

```c
/* In entitas_repositorium.h - add to vtable */

/* Query exact genus match */
Xar* (*quaerere_cum_genere)(
    vacuum*         datum,
    chorda*         genus);

/* Query genus prefix (returns all child genera too) */
Xar* (*quaerere_cum_praefixo_generis)(
    vacuum*         datum,
    chorda*         praefixum_generis);

/* Example usage: */
/* quaerere_cum_genere("Vultus::Imago") - exact match */
/* quaerere_cum_praefixo_generis("Vultus") - returns Vultus::Imago, Vultus::Pictor, etc. */
```

### 3. Session Entity Pattern

Cross-widget state sharing via entity system:

```c
/* Genus: Sessio::Importatio */
/* Properties: */
/*   status: chorda - "pending" | "active" | "completed" | "cancelled" */
/*   via_originis: chorda - source file path or "clipboard" */
/*   colores_activi: blobum - boolean array[16] */
/*   punctum_1: s32 - breakpoint slider 1 */
/*   punctum_2: s32 - breakpoint slider 2 */
/*   punctum_3: s32 - breakpoint slider 3 */
/*   zoom: f64 - zoom level */
/*   pan_x: s32 - pan offset x */
/*   pan_y: s32 - pan offset y */
/*   titulus: chorda - proposed image name */
/*   output: blobum - current dithered output (for preview) */

/* Both dialog and preview widgets observe this entity */
/* Dialog writes changes, preview reads and renders */
```

## Updated Implementation Order

### Phase 1: Entity Extensions
1. Add TYPUS_BLOBUM to entity system
2. Implement automatic flatura compression/decompression
3. Add quaerere_cum_genere and quaerere_cum_praefixo_generis

### Phase 2: Elementa GUI Components
4. Create include/elementa.h with API
5. Implement slider
6. Implement button
7. Implement checkbox
8. Implement text input
9. Implement palette grid

### Phase 3: Platform Libraries
10. imago_platform.h/.c - Image loading (stb_image wrapper)
11. clipboard_platform.h/.m - macOS clipboard access

### Phase 4: Dithering Core
12. dithering.h/.c - Atkinson algorithm
13. vultus_imago.h/.c - Image entity storage helpers

### Phase 5: Dialog System
14. dialogus.h/.c - Modal dialog framework

### Phase 6: Import Dialog
15. dialogus_dithering.h/.c - Using elementa components
16. Session entity integration

### Phase 7: Pinacotheca Widget
17. pinacotheca_visus.h/.c - Browser and view modes
18. Preview mode for import session

### Phase 8: Integration
19. Schirmata dialog support
20. Command registration
21. Cmd+V clipboard detection

## File Structure

### New Files

```
include/elementa.h                 # GUI component API
lib/elementa.c                     # All component implementations

include/imago_platform.h           # Image loading API
lib/imago_platform.c               # stb_image wrapper
vendor/stb_image.h                 # Single-header library

include/clipboard_platform.h       # Clipboard API
lib/clipboard_platform_macos.m     # macOS implementation

include/dithering.h                # Dithering algorithm
lib/dithering.c                    # Implementation

include/dialogus.h                 # Modal dialog system
lib/dialogus.c                     # Implementation

include/dialogus_dithering.h       # Import dialog
lib/dialogus_dithering.c           # Implementation

include/vultus_imago.h             # Image entity helpers
lib/vultus_imago.c                 # Implementation

include/pinacotheca_visus.h        # Gallery widget
lib/pinacotheca_visus.c            # Implementation
```

### Files to Modify

```
include/entitas.h                  # Add TYPUS_BLOBUM
lib/entitas.c                      # Implement blob property functions
include/entitas_repositorium.h     # Add genus query functions
lib/entitas_repositorium_impl.c    # Implement genus queries
include/schirmata.h                # Add dialogus fields
lib/schirmata.c                    # Dialog rendering/events, Cmd+V
lib/concha.c                       # Register new commands
compile_*.sh                       # Add new source files
```

## Memory Management

### Scratch Piscina Pattern

For operations that produce temporary results (dithering), use a scratch arena:

```c
nomen structura {
    /* ... */
    Piscina* piscina_scratch;   /* Reset before each dither */
} DialogusDitheringDatum;

/* Before dithering */
piscina_reficere(datum->piscina_scratch);

/* Dither allocates from scratch piscina */
datum->output = dithering_processare(..., datum->piscina_scratch);
```

## Cross-Widget Communication

### Session Entity Flow

```
1. User initiates import ($import-image or Cmd+V)
2. Create Sessio::Importatio entity with initial state
3. Dialog widget observes entity, writes changes on user interaction
4. Preview widget observes entity, re-renders when output changes
5. On Salvare: create Vultus::Imago from session, delete session
6. On Abicere: delete session entity
```

### Entity Observation

Widgets poll the session entity each frame (or use actor message system if available):

```c
/* In preview widget render */
Entitas* sessio = repo->capere_entitatem(repo->datum, sessio_id);
si (sessio)
{
    i8* output_data;
    i32 output_len;
    si (entitas_proprietas_capere_blobum(sessio, "output",
            &output_data, &output_len, piscina))
    {
        _render_dithered_preview(output_data, ...);
    }
}
```

## Binary Storage Details

### Image Storage Format

```
Vultus::Imago entity:
  titulus: chorda        # Unique name
  latitudo: s32          # Width in pixels
  altitudo: s32          # Height in pixels
  datum: blobum          # Palette indices (1 byte/pixel, auto-compressed)
  creatum: tempus        # Creation timestamp
```

Storage size estimate:
- 360x448 image = 161,280 bytes raw
- ~40,000 bytes after flatura compression (~75% reduction)
- Stored directly in entity property (no base64)

### Compression Integration

```c
/* In entitas.c */

b32
entitas_proprietas_ponere_blobum(
    Entitas* entitas,
    chorda*  clavis,
    const i8* datum,
    i32      mensura)
{
    /* Compress */
    FlaturaFructus comp = flatura_deflare(
        datum, mensura,
        FLATURA_COMPRESSIO_ORDINARIA,
        entitas->piscina);

    si (comp.status != FLATURA_STATUS_OK)
        redde FALSUM;

    /* Store compressed data with length prefix */
    /* Format: [4 bytes original len][4 bytes compressed len][compressed data] */
    i32 total = VIII + comp.mensura;
    i8* buffer = piscina_allocare(entitas->piscina, (u32)total);
    memcpy(buffer, &mensura, IV);
    memcpy(buffer + IV, &comp.mensura, IV);
    memcpy(buffer + VIII, comp.datum, (u32)comp.mensura);

    /* Store as property */
    Proprietas* prop = _capere_vel_creare_proprietatem(entitas, clavis);
    prop->typus_literalis = TYPUS_BLOBUM;
    prop->valor = chorda_ex_octeti(buffer, total, entitas->piscina);

    redde VERUM;
}
```

## Related Specs

- pinacotheca-spec.md - Original feature spec
- pinacotheca-spec-v2.md - First implementation spec
- pinacotheca-simulation-brainstorm.md - Simulation findings
- vultus-spec.md - Parent design system
- unified-state-paradigm-brainstorm.md - Actor/entity patterns
