# Ratio - Truth Graph System (v2 Implementation Spec)

## Overview

Ratio is a fundamental data type and UI system for working with directed acyclic graphs of truth claims. This v2 spec adds detailed implementation planning based on codebase exploration.

## Entity Model

### New Genus Types

Following the existing pattern where genus types are implicit (no registration needed), Ratio introduces:

```
Genus::Ratio                        # Root for all Ratio types
Genus::Ratio::Propositio            # A truth claim
Genus::Ratio::Subgraphum            # A named collection of claims
Genus::Ratio::Pagina                # A canvas page with positioned claims
Genus::Ratio::StatusCredendi        # A user's belief state overlay
```

**Creating entities (follows existing pattern):**
```c
/* Auto-scaffolds entire genus hierarchy */
Entitas* prop = repo->entitas_scaffoldare(
    repo->datum,
    "Ratio::Propositio",
    "claim-unique-slug");

/* Properties for Propositio */
repo->proprietas_ponere(repo->datum, prop, "textus", "The proposition text");
repo->proprietas_ponere(repo->datum, prop, "explicatio", "Optional longer explanation");
repo->proprietas_ponere(repo->datum, prop, "status", "verum");  /* verum/falsum/incertum */
repo->proprietas_ponere(repo->datum, prop, "fiducia", "0.85");  /* Optional confidence */
```

### New Relationship Types

No registration needed - just use the relationship genus string:

```
ratio_sustinet      # A supports/is premise for B (deductive)
ratio_contradicit   # A contradicts B (mutual exclusion)
ratio_probat        # A is evidence for B (inductive/empirical)
ratio_continet      # Subgraph contains Propositio
ratio_in_pagina     # Propositio placed on Pagina
```

**Creating relationships:**
```c
/* Support relationship */
Relatio* rel = repo->relatio_addere(
    repo->datum,
    premise,
    "ratio_sustinet",
    conclusion->id);

/* With strength metadata (store as property on relationship entity) */
/* Note: relationships themselves can be treated as entities for metadata */
```

### Pagina (Canvas Page) Structure

Following arx_caeli's Carta pattern:

```c
/* Pagina entity properties */
repo->proprietas_ponere(repo->datum, pagina, "titulus", "Page Title");

/* Position claims on page via relationship with properties */
Relatio* placement = repo->relatio_addere(
    repo->datum,
    pagina,
    "ratio_in_pagina",
    propositio->id);

/* Store position as properties on a placement entity */
Entitas* pos = repo->entitas_scaffoldare(repo->datum, "Ratio::Positio", uuid);
repo->proprietas_ponere(repo->datum, pos, "x", "120");
repo->proprietas_ponere(repo->datum, pos, "y", "80");
repo->proprietas_ponere(repo->datum, pos, "z_index", "5");
repo->relatio_addere(repo->datum, pos, "positio_pro", propositio->id);
repo->relatio_addere(repo->datum, pos, "in_pagina", pagina->id);
```

## UI Architecture

### Widget Structure

Ratio will be a **singleton widget** (like arx_caeli, biblia_visus) managed by schirmata:

```c
/* In schirmata.h - add to Schirmata struct */
nomen structura {
    /* ... existing fields ... */
    RatioVisus* ratio_visus;        /* Singleton instance */
    b32         modus_ratio_visus;  /* Per-screen mode flag */
} Schirmata;

/* In schirma (per-screen state) */
nomen structura {
    /* ... existing fields ... */
    b32 modus_ratio_visus;
    /* Saved state when switching away */
    chorda* ratio_pagina_id;
    i32     ratio_selectio;
} Schirma;
```

### Split-Pane Coordination

Following the arx_caeli link_callback pattern:

```c
/* ratio_visus.h */
nomen vacuum (*FunctioRatioDetailCallback)(
    vacuum*             datum,
    constans character* propositio_id);

structura RatioVisus {
    ContextusWidget*          ctx;
    Piscina*                  piscina;
    InternamentumChorda*      intern;
    EntitasRepositorium*      repo;

    /* Current state */
    chorda*                   pagina_currens_id;
    Xar*                      propositiones;      /* Visible claims */
    i32                       selectio;           /* Selected index */
    i32                       pagina_x;           /* Canvas page coords */
    i32                       pagina_y;

    /* Navigation history (like libro_paginarum) */
    chorda*                   historia[XXXII];
    i32                       historia_numerus;

    /* Facing panel callback */
    FunctioRatioDetailCallback detail_callback;
    vacuum*                    detail_datum;
};

/* When claim selected, notify facing panel */
vacuum ratio_visus_selectionem_mutare(RatioVisus* rv, i32 index)
{
    rv->selectio = index;
    si (rv->detail_callback && index >= 0)
    {
        Propositio* p = xar_capere(rv->propositiones, index);
        rv->detail_callback(rv->detail_datum, p->id->data);
    }
}
```

### Schirmata Integration

```c
/* In schirmata.c - initialization */
schirmata->ratio_visus = ratio_visus_creare(ctx);
si (schirmata->ratio_visus)
{
    ratio_visus_ponere_detail_callback(
        schirmata->ratio_visus,
        _ratio_detail_callback,
        schirmata);
}

/* Callback updates libro with claim details */
hic_manens vacuum
_ratio_detail_callback(vacuum* datum, constans character* prop_id)
{
    Schirmata* schirmata = (Schirmata*)datum;

    /* Load claim entity */
    chorda* id = chorda_internare_ex_literis(schirmata->ctx->intern, prop_id);
    Entitas* prop = schirmata->ctx->repo->capere_entitatem(
        schirmata->ctx->repo->datum, id);

    si (prop)
    {
        /* Format claim details for libro display */
        chorda* textus = entitas_proprietas_capere(prop,
            chorda_internare_ex_literis(schirmata->ctx->intern, "textus"));

        /* Navigate libro to show formatted details */
        /* Could create a dedicated "claim detail" page template */
        libro_navigare_ad_nomen(schirmata->libro, prop_id);
    }
}
```

## Visual Rendering

### Claim Node Rendering

Following navigator_entitatum's visual patterns:

```c
/* Node visual structure */
#define RATIO_NODE_PADDING_X    II      /* 2 chars */
#define RATIO_NODE_PADDING_Y    I       /* 1 line */
#define RATIO_NODE_MIN_WIDTH    XX      /* 20 chars */
#define RATIO_NODE_MAX_WIDTH    L       /* 50 chars */

/* Color scheme following existing palette */
#define RATIO_COLOR_NODE_BG         PALETTE_DARK_STONE
#define RATIO_COLOR_NODE_BORDER     PALETTE_MEDIUM_GOLD
#define RATIO_COLOR_NODE_SELECTED   COLOR_ACCENT_PRIMARY
#define RATIO_COLOR_EDGE_SUSTINET   PALETTE_DARK_LEAF     /* Green for support */
#define RATIO_COLOR_EDGE_CONTRADICIT PALETTE_DARK_ROSE    /* Red for contradiction */
#define RATIO_COLOR_EDGE_PROBAT     PALETTE_MEDIUM_GOLD   /* Gold for evidence */

/* Render single node */
hic_manens vacuum
_ratio_reddere_propositio(
    TabulaPixelorum*    tabula,
    Thema*              thema,
    Propositio*         prop,
    i32                 x,
    i32                 y,
    b32                 selectum,
    f32                 scala)
{
    /* Calculate text bounds with wrapping */
    i32 latitudo = _calculare_latitudo_textus(prop->textus, RATIO_NODE_MAX_WIDTH);
    i32 altitudo = _calculare_altitudo_textus(prop->textus, latitudo);

    /* Draw rounded rect background */
    u32 color_bg = selectum ? RATIO_COLOR_NODE_SELECTED : RATIO_COLOR_NODE_BG;
    delineare_rectangulum_rotundatum(
        tabula,
        x * VI * scala,
        y * VIII * scala,
        (latitudo + RATIO_NODE_PADDING_X * II) * VI * scala,
        (altitudo + RATIO_NODE_PADDING_Y * II) * VIII * scala,
        color_bg,
        II);  /* corner radius */

    /* Draw text */
    tabula_pixelorum_pingere_chordam(
        tabula,
        (x + RATIO_NODE_PADDING_X) * VI * scala,
        (y + RATIO_NODE_PADDING_Y) * VIII * scala,
        prop->textus,
        thema_capere_colorem(thema, selectum ? COLOR_TEXT_INVERSE : COLOR_TEXT_PRIMARY),
        scala);
}
```

### Edge Rendering

```c
/* Edge rendering between nodes */
hic_manens vacuum
_ratio_reddere_relatio(
    TabulaPixelorum*    tabula,
    Propositio*         origo,
    Propositio*         destinatio,
    chorda*             genus,
    f32                 scala)
{
    /* Calculate center points */
    i32 x1 = origo->x + origo->latitudo / II;
    i32 y1 = origo->y + origo->altitudo / II;
    i32 x2 = destinatio->x + destinatio->latitudo / II;
    i32 y2 = destinatio->y + destinatio->altitudo / II;

    /* Select color by relationship type */
    u32 color;
    si (chorda_aequat_literis(genus, "ratio_sustinet"))
        color = RATIO_COLOR_EDGE_SUSTINET;
    alioquin si (chorda_aequat_literis(genus, "ratio_contradicit"))
        color = RATIO_COLOR_EDGE_CONTRADICIT;
    alioquin
        color = RATIO_COLOR_EDGE_PROBAT;

    /* Draw line with arrow */
    delineare_lineam(tabula,
        x1 * VI * scala, y1 * VIII * scala,
        x2 * VI * scala, y2 * VIII * scala,
        color);

    /* Draw arrowhead at destination */
    _ratio_reddere_sagittam(tabula, x2, y2, x1, y1, color, scala);
}
```

## Navigation

### Paginated Canvas (N/S/E/W)

Following the "no scrolling, pagination only" constraint:

```c
/* Canvas page coordinates */
structura RatioVisus {
    /* ... */
    i32 pagina_x;   /* Current page column */
    i32 pagina_y;   /* Current page row */
};

/* Check if content exists in direction */
hic_manens b32
_ratio_habet_paginam(RatioVisus* rv, i32 dx, i32 dy)
{
    i32 nova_x = rv->pagina_x + dx;
    i32 nova_y = rv->pagina_y + dy;

    /* Query if any claims positioned in that page region */
    per (i32 i = 0; i < xar_longitudo(rv->propositiones); i++)
    {
        Propositio* p = xar_capere(rv->propositiones, i);
        i32 px = p->x / PAGINA_LATITUDO;
        i32 py = p->y / PAGINA_ALTITUDO;
        si (px == nova_x && py == nova_y)
            redde VERUM;
    }
    redde FALSUM;
}

/* Navigate to adjacent page */
vacuum ratio_visus_navigare(RatioVisus* rv, i32 dx, i32 dy)
{
    si (_ratio_habet_paginam(rv, dx, dy))
    {
        rv->pagina_x += dx;
        rv->pagina_y += dy;
        rv->selectio = 0;  /* Reset selection */
    }
}

/* Key handling */
casus KEY_LEFT:  ratio_visus_navigare(rv, -1,  0); frange;
casus KEY_RIGHT: ratio_visus_navigare(rv,  1,  0); frange;
casus KEY_UP:    ratio_visus_navigare(rv,  0, -1); frange;
casus KEY_DOWN:  ratio_visus_navigare(rv,  0,  1); frange;
```

### Drill-Down and Breadcrumbs

Following navigator_entitatum's history pattern:

```c
/* History entry */
nomen structura {
    chorda* pagina_id;
    i32     pagina_x;
    i32     pagina_y;
    i32     selectio;
} RatioHistoria;

/* Drill into subgraph */
vacuum ratio_visus_intrare(RatioVisus* rv)
{
    Propositio* sel = _ratio_selectum(rv);
    si (!sel) redde;

    /* Check if this claim has a subgraph */
    Xar* subgraphs = rv->repo->capere_relationes_typi(
        rv->repo->datum, sel->id, "ratio_continet_in");

    si (xar_longitudo(subgraphs) > 0)
    {
        /* Save current state to history */
        RatioHistoria* h = piscina_allocare(rv->piscina, sizeof(RatioHistoria));
        h->pagina_id = rv->pagina_currens_id;
        h->pagina_x = rv->pagina_x;
        h->pagina_y = rv->pagina_y;
        h->selectio = rv->selectio;
        rv->historia[rv->historia_numerus++] = h;

        /* Navigate to subgraph */
        Relatio* rel = xar_capere(subgraphs, 0);
        _ratio_caricare_paginam(rv, rel->origo_id);
    }
}

/* Go back */
vacuum ratio_visus_retro(RatioVisus* rv)
{
    si (rv->historia_numerus > 0)
    {
        RatioHistoria* h = rv->historia[--rv->historia_numerus];
        _ratio_caricare_paginam(rv, h->pagina_id);
        rv->pagina_x = h->pagina_x;
        rv->pagina_y = h->pagina_y;
        rv->selectio = h->selectio;
    }
}
```

## File Structure

### New Files to Create

```
include/ratio_visus.h           # Main widget header
lib/ratio_visus.c               # Widget implementation
lib/ratio_visus_reddere.c       # Rendering (if large)
probationes/probatio_ratio.c    # Tests
```

### Files to Modify

```
include/schirmata.h             # Add RatioVisus* and modus flag
lib/schirmata.c                 # Add initialization, mode switching, callbacks
include/registrum_widget.h      # Register "ratio" widget factory (if needed)
```

## Commands

Following existing command registration pattern:

```c
/* In ratio_visus.c */
vacuum ratio_visus_init(ContextusWidget* ctx)
{
    RegistrumCommandi* reg = ctx->reg_commandi;

    registrum_commandi_registrare(reg, "$ratio-new",
        "Create new claim", _ratio_cmd_novum, ctx);
    registrum_commandi_registrare(reg, "$ratio-link",
        "Link selected claims", _ratio_cmd_ligare, ctx);
    registrum_commandi_registrare(reg, "$ratio-contradict",
        "Mark contradiction", _ratio_cmd_contradicere, ctx);
    registrum_commandi_registrare(reg, "$ratio-evidence",
        "Add evidence link", _ratio_cmd_probare, ctx);
    registrum_commandi_registrare(reg, "$ratio-toggle",
        "Toggle belief state", _ratio_cmd_commutare, ctx);
}
```

## First Milestone: Visual Graph

Minimum implementation for first milestone:

1. **Entity layer**: Create Ratio::Propositio entities with textus property
2. **Relationship layer**: Create ratio_sustinet, ratio_contradicit, ratio_probat relationships
3. **Pagina layer**: Position claims on canvas pages
4. **Render layer**: Draw nodes as boxes, edges as lines with arrows
5. **Navigation**: j/k selection, N/S/E/W page navigation, h/l drill/back
6. **Facing panel**: Show selected claim details in libro

### Implementation Order

1. `ratio_visus.h` - Define structures
2. `ratio_visus.c` - Create/destroy, state management
3. Add to `schirmata.h/.c` - Singleton, mode flag, callback
4. Rendering - Nodes first, then edges
5. Navigation - Selection, then pagination, then history
6. Commands - $ratio-new, then linking commands
7. Tests - Entity creation, relationship queries

## Open Implementation Questions

1. **Z-ordering for edges**: Render edges before nodes (z=0) so nodes appear on top?
2. **Edge routing**: Simple straight lines, or avoid crossing nodes?
3. **Auto-layout algorithm**: Force-directed? Hierarchical? Optional for v1?
4. **Claim detail page template**: Create dedicated libro page format for claims?
5. **Belief state storage**: Separate StatusCredendi entity per user, or property on Propositio?
