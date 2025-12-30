# Canvas File Explorer v2 - Implementation Spec

Date: 2025-12-29
Status: Draft
Based on: Codebase exploration + interview brainstorm

## Executive Summary

Build a new widget (TabulaExplorator) that combines arx_caeli's spatial canvas with filesystem browsing. Files from mounted host folders appear as draggable icons alongside notes and folder links.

**Key Decision**: Build new widget rather than extend arx_caeli, but reuse patterns and potentially share code.

---

## Part 1: Existing Infrastructure Analysis

### 1.1 What arx_caeli Already Does

| Feature | Implementation | Reusable? |
|---------|---------------|-----------|
| Spatial positioning | Carta struct with x,y in pixels | Yes - same pattern |
| Z-ordering | z_index field, sorted hit testing | Yes |
| Drag and drop | Full state machine with ghost | Yes - copy pattern |
| Folder navigation | `>` prefix → portal_ad relationship | Adapt for canvas links |
| Persistence | Entity repository with relationships | Yes |
| Debounced save | 2-second timer, immundum flag | Yes |
| Link detection | `#link-name` in text | Yes |
| Text editing | Insert mode with cursor | Maybe for notes |

**Limitations of arx_caeli:**
- 64 card limit (no pagination)
- No filesystem integration
- No file type icons
- Cards only (no differentiated item types)

### 1.2 Filesystem Libraries Available

| Library | Purpose | Key Functions |
|---------|---------|---------------|
| `iter_directoria.h` | Directory listing | `directorium_iterator_*`, `directorium_ambulare()` |
| `via.h` | Path manipulation | `via_extensio()`, `via_iungere()`, `via_nomen()` |
| `filum.h` | File I/O & metadata | `filum_status()`, `filum_existit()` |

**What's Missing:**
- FSEvents watching (would need new code)
- Mount point enumeration
- File type → icon mapping

### 1.3 Rendering Capabilities

| Capability | Source | Notes |
|------------|--------|-------|
| 256 glyphs | fons_6x8.h | Include symbols usable for icons |
| Scaled text | tabula_pixelorum_pingere_characterem_scalatum | Any integer scale |
| Shapes | delineare.h | Rectangles, rounded corners, fills |
| 38 patterns | delineare.c | Hatching, dots, etc. for visual variety |
| Colors | color.h + themes | Full RGBA with palette |

### 1.4 Interaction Patterns

From arx_caeli (lib/arx_caeli.c):

**Coordinate Systems:**
- Widget position in characters
- Item position in pixels (widget-relative)
- Mouse events in screen pixels
- Conversion: `char_lat = 6 * scala`, `char_alt = 8 * scala`

**Drag State Machine:**
```c
b32 trahens;                 /* Currently dragging */
i32 trahere_origin_x/y;      /* Original position */
i32 trahere_offset_px_x/y;   /* Click offset within item */
i32 trahere_grid_x/y;        /* Clamped ghost position */
i32 trahere_hover_folder;    /* Drop target detection */
```

**Hit Testing:**
- Sort by z_index descending
- Test from top down (topmost first)
- Bounding box check in pixels

---

## Part 2: Data Model

### 2.1 Entity Structure

```
Canvas Entity
├─ genus: "TabulaCanvas"
├─ proprietates:
│   ├─ address: "/work/projects/rhubarb"
│   ├─ titulus: "Rhubarb Project"
│   ├─ pagina_currens: "0"
│   ├─ paginae_numerus: "1"
│   └─ status: "active"
├─ relationes:
│   ├─ "continet" → [CanvasItem entities]
│   └─ "mount" → [Mount entities]
└─ notae: ["#TabulaCanvas"]

CanvasItem Entity
├─ genus: "TabulaItem"
├─ proprietates:
│   ├─ x: "150"           (pixels, widget-relative)
│   ├─ y: "200"
│   ├─ z_index: "5"
│   ├─ pagina: "0"
│   ├─ item_genus: "file" | "note" | "canvas_link"
│   └─ (type-specific properties below)
├─ relationes:
│   ├─ "in_canvas" → Canvas entity
│   └─ "references" → FileRef | Canvas (depending on type)
└─ notae: []

FileRef Entity (represents a file from host filesystem)
├─ genus: "FileRef"
├─ proprietates:
│   ├─ host_path: "/Users/fran/doc.md"
│   ├─ filename: "doc.md"
│   ├─ extensio: "md"
│   ├─ est_directorium: "false"
│   ├─ mensura: "4096"
│   └─ icon_glyph: "D"    (glyph character for icon)
├─ relationes:
│   └─ "from_mount" → Mount entity
└─ notae: []

Mount Entity
├─ genus: "TabulaMount"
├─ proprietates:
│   ├─ host_path: "/Users/fran/Documents"
│   ├─ titulus: "Documents"
│   └─ activum: "true"
├─ relationes:
│   └─ "mounted_in" → Canvas entity
└─ notae: []
```

### 2.2 In-Memory Structures

```c
/* Single item on canvas */
nomen structura TabulaItem {
    chorda*         entitas_id;      /* Entity ID for persistence */
    i32             x, y;            /* Position in pixels */
    i32             z_index;
    i32             pagina;          /* Which page (0-indexed) */
    TabulaItemGenus genus;           /* file, note, canvas_link */

    /* Computed for rendering */
    i32             icon_latitudo;   /* Icon width in pixels */
    i32             icon_altitudo;   /* Icon height in pixels */

    /* Type-specific data */
    unio {
        TabulaItemFile       file;
        TabulaItemNote       note;
        TabulaItemCanvasLink canvas_link;
    } datum;
} TabulaItem;

nomen enumeratio {
    TABULA_ITEM_FILE,
    TABULA_ITEM_NOTE,
    TABULA_ITEM_CANVAS_LINK
} TabulaItemGenus;

nomen structura TabulaItemFile {
    chorda*  host_path;
    chorda*  filename;
    chorda*  extensio;
    b32      est_directorium;
    i64      mensura;
    character icon_glyph;            /* Single character for icon */
} TabulaItemFile;

nomen structura TabulaItemNote {
    chorda*  textus;
    i32      latitudo_chars;         /* Text width */
    i32      altitudo_chars;         /* Text height */
} TabulaItemNote;

nomen structura TabulaItemCanvasLink {
    chorda*  destination_address;    /* e.g., "/work/projects" */
    chorda*  titulus;                /* Display name */
} TabulaItemCanvasLink;
```

### 2.3 Widget Structure

```c
nomen structura TabulaExplorator {
    ContextusWidget*    ctx;
    Entitas*            canvas_entitas;
    chorda*             canvas_address;

    /* Items */
    TabulaItem          items[TABULA_ITEMS_MAXIMUS];  /* 128 items */
    i32                 numerus_itemorum;
    chorda*             entitas_ids[TABULA_ITEMS_MAXIMUS];

    /* Mounts */
    TabulaMount         mounts[TABULA_MOUNTS_MAXIMUS];  /* 8 mounts */
    i32                 numerus_montium;

    /* Pagination */
    i32                 pagina_currens;
    i32                 paginae_numerus;
    i32                 items_per_pagina;              /* Computed from size */

    /* Selection */
    i32                 index_selecta;                 /* -1 if none */
    TabulaModus         modus;

    /* Drag state (copied from arx_caeli pattern) */
    b32                 trahens;
    i32                 trahere_origin_x, trahere_origin_y;
    i32                 trahere_offset_px_x, trahere_offset_px_y;
    i32                 trahere_grid_x, trahere_grid_y;
    b32                 trahere_validum;
    i32                 trahere_hover_target;          /* Drop zone or folder */

    /* Dirty tracking */
    b32                 immundum;
    f64                 tempus_ultimae_mutationis;

    /* Navigation history */
    chorda*             historia[TABULA_HISTORIA_MAXIMUS];  /* 16 levels */
    i32                 historia_index;

    /* Layout cache */
    i32                 widget_x, widget_y;
    i32                 latitudo, altitudo;
    i32                 scala;
} TabulaExplorator;

nomen enumeratio {
    TABULA_MODUS_NORMALIS,
    TABULA_MODUS_SELECTA,
    TABULA_MODUS_BROWSING      /* Inside a mounted folder */
} TabulaModus;

nomen structura TabulaMount {
    chorda*  entitas_id;
    chorda*  host_path;
    chorda*  titulus;
    b32      activum;
    chorda*  current_subpath;      /* If browsing inside mount */
} TabulaMount;
```

---

## Part 3: Icon System

### 3.1 File Type to Glyph Mapping

Use existing 6x8 font glyphs. Define mapping in new header:

```c
/* include/tabula_icones.h */

nomen structura TabulaIconMapping {
    constans character* extensio;    /* "md", "c", "txt", etc. */
    character           glyph;       /* Font glyph character */
    i32                 color;       /* Palette index or RGB */
} TabulaIconMapping;

/* Default mappings */
interior constans TabulaIconMapping TABULA_ICON_MAPPINGS[] = {
    /* Documents */
    { "md",   0x0E, PALETTE_DOCUMENT },     /* Document glyph */
    { "txt",  0x0E, PALETTE_DOCUMENT },
    { "pdf",  0x0E, PALETTE_RED },

    /* Code */
    { "c",    0x7B, PALETTE_CODE },         /* { brace */
    { "h",    0x7B, PALETTE_CODE },
    { "js",   0x7B, PALETTE_YELLOW },
    { "py",   0x7B, PALETTE_BLUE },

    /* Images */
    { "png",  0x0F, PALETTE_IMAGE },        /* Image glyph */
    { "jpg",  0x0F, PALETTE_IMAGE },

    /* Audio */
    { "mp3",  0x0D, PALETTE_AUDIO },        /* Music note */
    { "wav",  0x0D, PALETTE_AUDIO },

    /* Default */
    { NIHIL,  0x07, PALETTE_DEFAULT }       /* Generic file */
};

/* Directory always uses folder glyph */
#define TABULA_ICON_FOLDER     0x1E         /* Folder glyph */
#define TABULA_ICON_FOLDER_OPEN 0x1F        /* Open folder */

/* Lookup function */
character tabula_icon_glyph_pro_extensio(constans character* ext);
i32 tabula_icon_color_pro_extensio(constans character* ext);
```

### 3.2 Icon Rendering

```c
/* Icon layout constants */
#define TABULA_ICON_SCALA       III         /* 3x scale = 18x24 pixels */
#define TABULA_ICON_PADDING     IV          /* 4 pixels padding */
#define TABULA_LABEL_LINEA      II          /* 2 lines for filename */
#define TABULA_ICON_LATITUDO    (VI * TABULA_ICON_SCALA + TABULA_ICON_PADDING * II)
#define TABULA_ICON_ALTITUDO    (VIII * TABULA_ICON_SCALA + VIII * TABULA_LABEL_LINEA + TABULA_ICON_PADDING * II)

/* Render single icon */
vacuum _tabula_reddere_item(
    TabulaExplorator* tab,
    TabulaItem* item,
    TabulaPixelorum* tabula,
    i32 widget_px_x, i32 widget_px_y,
    b32 selecta)
{
    i32 px_x = widget_px_x + item->x;
    i32 px_y = widget_px_y + item->y;

    /* Background (highlight if selected) */
    si (selecta) {
        delineare_rectangulum_plenum(tabula,
            px_x, px_y,
            TABULA_ICON_LATITUDO, TABULA_ICON_ALTITUDO,
            COLOR_SELECTION);
    }

    /* Icon glyph (centered, scaled) */
    character glyph = item->datum.file.icon_glyph;
    i32 glyph_x = px_x + (TABULA_ICON_LATITUDO - VI * TABULA_ICON_SCALA) / II;
    i32 glyph_y = px_y + TABULA_ICON_PADDING;

    tabula_pixelorum_pingere_characterem_scalatum(
        tabula, glyph_x, glyph_y,
        glyph,
        tabula_icon_color_pro_extensio(item->datum.file.extensio),
        TABULA_ICON_SCALA);

    /* Filename label (truncated, below icon) */
    i32 label_y = glyph_y + VIII * TABULA_ICON_SCALA + TABULA_ICON_PADDING;
    _tabula_reddere_label(tabula, px_x, label_y,
        item->datum.file.filename, TABULA_ICON_LATITUDO);
}
```

---

## Part 4: Filesystem Integration

### 4.1 Directory Listing

```c
/* Scan mounted folder and create file items */
vacuum _tabula_scan_mount(
    TabulaExplorator* tab,
    TabulaMount* mount,
    constans character* subpath)
{
    character via_plena[MMMMXCVI];

    /* Build full path */
    si (subpath && subpath[0] != '\0') {
        snprintf(via_plena, MMMMXCVI, "%s/%s",
            mount->host_path->datum, subpath);
    } alioquin {
        memcpy(via_plena, mount->host_path->datum, mount->host_path->mensura);
        via_plena[mount->host_path->mensura] = '\0';
    }

    /* Open directory */
    DirectoriumIterator* iter = directorium_iterator_aperire(via_plena);
    si (!iter) redde;

    DirectoriumIntroitus introitus;
    i32 item_x = TABULA_ICON_PADDING;
    i32 item_y = TABULA_HEADER_ALTITUDO;  /* Below header */
    i32 column = 0;

    dum (directorium_iterator_proximum(iter, &introitus)) {
        /* Skip hidden files */
        si (introitus.titulus.datum[0] == '.') perge;

        /* Create item */
        TabulaItem* item = &tab->items[tab->numerus_itemorum];
        item->genus = TABULA_ITEM_FILE;
        item->x = item_x;
        item->y = item_y;
        item->z_index = tab->numerus_itemorum;
        item->pagina = tab->pagina_currens;

        /* Copy file info */
        item->datum.file.host_path = chorda_internare(tab->ctx->intern,
            chorda_ex_literis(via_plena, tab->ctx->piscina));
        item->datum.file.filename = chorda_internare(tab->ctx->intern,
            introitus.titulus);
        item->datum.file.est_directorium = (introitus.genus == INTROITUS_DIRECTORIUM);
        item->datum.file.mensura = introitus.mensura;

        /* Determine icon */
        si (item->datum.file.est_directorium) {
            item->datum.file.icon_glyph = TABULA_ICON_FOLDER;
            item->datum.file.extensio = NIHIL;
        } alioquin {
            chorda ext = via_extensio(introitus.titulus, tab->ctx->piscina);
            item->datum.file.extensio = chorda_internare(tab->ctx->intern, ext);
            item->datum.file.icon_glyph = tabula_icon_glyph_pro_extensio(
                item->datum.file.extensio->datum);
        }

        tab->numerus_itemorum++;

        /* Grid layout */
        column++;
        item_x += TABULA_ICON_LATITUDO + TABULA_ICON_SPACING;
        si (item_x + TABULA_ICON_LATITUDO > tab->latitudo * VI * tab->scala) {
            item_x = TABULA_ICON_PADDING;
            item_y += TABULA_ICON_ALTITUDO + TABULA_ICON_SPACING;
            column = 0;
        }

        /* Check page overflow */
        si (item_y + TABULA_ICON_ALTITUDO > tab->altitudo * VIII * tab->scala - TABULA_FOOTER_ALTITUDO) {
            tab->paginae_numerus++;
            item_x = TABULA_ICON_PADDING;
            item_y = TABULA_HEADER_ALTITUDO;
            /* Items on next page get incremented pagina */
        }

        si (tab->numerus_itemorum >= TABULA_ITEMS_MAXIMUS) frange;
    }

    directorium_iterator_claudere(iter);
}
```

### 4.2 File Opening (Launcher)

```c
/* Open file in system application */
vacuum tabula_aperire_in_systemate(constans character* via)
{
    character command[MMMMXCVI];
    snprintf(command, MMMMXCVI, "open \"%s\"", via);
    system(command);  /* macOS 'open' command */
}

/* Future: Internal viewer dispatch */
vacuum tabula_aperire_interne(TabulaExplorator* tab, TabulaItem* item)
{
    constans character* ext = item->datum.file.extensio->datum;

    /* Dispatch to appropriate widget */
    si (strcmp(ext, "md") == 0 || strcmp(ext, "txt") == 0) {
        /* Open in Pagina/LibroPaginarum */
        tab->ctx->commutare_widget(tab->ctx->schirmata_datum,
            "pagina", item->datum.file.host_path->datum);
    }
    alioquin {
        /* Fall back to system */
        tabula_aperire_in_systemate(item->datum.file.host_path->datum);
    }
}
```

---

## Part 5: Pagination

### 5.1 Page Layout

```
┌─────────────────────────────────────────────────┐
│ < Back    /work/projects/rhubarb    [Mount...]  │ ← Header (40px)
├─────────────────────────────────────────────────┤
│                                                 │
│  [icon]  [icon]  [icon]  [icon]  [icon]        │
│  file1   file2   file3   file4   file5         │
│                                                 │
│  [icon]  [icon]  [icon]  [icon]  [icon]        │
│  file6   file7   file8   file9   file10        │
│                                                 │
│                 ... more rows ...               │
│                                                 │
├─────────────────────────────────────────────────┤
│ [< Prev]  Page 1 of 3  [Next >]   [+ New Note] │ ← Footer (32px)
│ [Drop Zone: Prev Page] [Drop Zone: Next Page]  │
└─────────────────────────────────────────────────┘
```

### 5.2 Page Navigation

```c
vacuum tabula_pagina_proxima(TabulaExplorator* tab)
{
    si (tab->pagina_currens < tab->paginae_numerus - 1) {
        tab->pagina_currens++;
        tab->index_selecta = NIHIL_SELECTA;
    }
}

vacuum tabula_pagina_prior(TabulaExplorator* tab)
{
    si (tab->pagina_currens > 0) {
        tab->pagina_currens--;
        tab->index_selecta = NIHIL_SELECTA;
    }
}

/* Drop zone hit testing */
TabulaDropZone _tabula_drop_zone_ad_punctum(TabulaExplorator* tab, i32 px_x, i32 px_y)
{
    i32 footer_y = (tab->altitudo * VIII * tab->scala) - TABULA_FOOTER_ALTITUDO;

    si (px_y >= footer_y) {
        si (px_x < tab->latitudo * VI * tab->scala / II) {
            redde TABULA_DROP_PREV_PAGE;
        } alioquin {
            redde TABULA_DROP_NEXT_PAGE;
        }
    }

    redde TABULA_DROP_NONE;
}
```

### 5.3 Cross-Page Item Move

```c
vacuum _tabula_movere_item_ad_paginam(TabulaExplorator* tab, i32 item_index, i32 pagina_dest)
{
    TabulaItem* item = &tab->items[item_index];

    /* Update item's page */
    item->pagina = pagina_dest;

    /* Reset position to first available slot on target page */
    item->x = TABULA_ICON_PADDING;
    item->y = TABULA_HEADER_ALTITUDO;

    /* Find non-overlapping position (simple: just stack at origin) */
    /* Future: smarter layout */

    tabula_marcare_immundum(tab);
}
```

---

## Part 6: Event Handling

### 6.1 Mouse Events

```c
interior b32
_tabula_tractare_mus(TabulaExplorator* tab, constans Eventus* eventus)
{
    i32 char_lat = VI * tab->scala;
    i32 char_alt = VIII * tab->scala;

    /* Convert to widget-relative pixels */
    i32 px_x = eventus->datum.mus.x - (tab->widget_x * char_lat);
    i32 px_y = eventus->datum.mus.y - (tab->widget_y * char_alt);

    commutatio (eventus->genus) {

    casus EVENTUS_MUS_DEPRESSUS: {
        /* Check header buttons */
        si (_tabula_header_click(tab, px_x, px_y)) {
            redde VERUM;
        }

        /* Check footer/drop zones */
        si (_tabula_footer_click(tab, px_x, px_y)) {
            redde VERUM;
        }

        /* Hit test items (current page only) */
        i32 hit = _tabula_invenire_item_ad_punctum(tab, px_x, px_y);

        si (hit != NIHIL_SELECTA) {
            tab->index_selecta = hit;
            tab->modus = TABULA_MODUS_SELECTA;

            /* Start drag */
            TabulaItem* item = &tab->items[hit];
            tab->trahens = VERUM;
            tab->trahere_origin_x = item->x;
            tab->trahere_origin_y = item->y;
            tab->trahere_offset_px_x = px_x - item->x;
            tab->trahere_offset_px_y = px_y - item->y;
            tab->trahere_grid_x = item->x;
            tab->trahere_grid_y = item->y;

            /* Bring to front */
            item->z_index = _tabula_max_z_index(tab) + 1;
        } alioquin {
            /* Click on empty - deselect */
            tab->index_selecta = NIHIL_SELECTA;
            tab->modus = TABULA_MODUS_NORMALIS;
        }
        redde VERUM;
    }

    casus EVENTUS_MUS_MOTUS: {
        si (tab->trahens && tab->index_selecta != NIHIL_SELECTA) {
            TabulaItem* item = &tab->items[tab->index_selecta];

            /* Calculate ghost position with clamping */
            i32 ghost_x = px_x - tab->trahere_offset_px_x;
            i32 ghost_y = px_y - tab->trahere_offset_px_y;

            /* Clamp to content area */
            i32 max_x = (tab->latitudo * char_lat) - TABULA_ICON_LATITUDO;
            i32 max_y = (tab->altitudo * char_alt) - TABULA_FOOTER_ALTITUDO - TABULA_ICON_ALTITUDO;

            si (ghost_x < 0) ghost_x = 0;
            si (ghost_y < TABULA_HEADER_ALTITUDO) ghost_y = TABULA_HEADER_ALTITUDO;
            si (ghost_x > max_x) ghost_x = max_x;
            si (ghost_y > max_y) ghost_y = max_y;

            tab->trahere_grid_x = ghost_x;
            tab->trahere_grid_y = ghost_y;

            /* Check drop zone hover */
            tab->trahere_hover_target = _tabula_drop_zone_ad_punctum(tab, px_x, px_y);
        }
        redde VERUM;
    }

    casus EVENTUS_MUS_LIBERATUS: {
        si (tab->trahens && tab->index_selecta != NIHIL_SELECTA) {
            /* Check drop zone */
            si (tab->trahere_hover_target == TABULA_DROP_PREV_PAGE) {
                _tabula_movere_item_ad_paginam(tab, tab->index_selecta, tab->pagina_currens - 1);
            }
            alioquin si (tab->trahere_hover_target == TABULA_DROP_NEXT_PAGE) {
                _tabula_movere_item_ad_paginam(tab, tab->index_selecta, tab->pagina_currens + 1);
            }
            alioquin {
                /* Commit position change */
                TabulaItem* item = &tab->items[tab->index_selecta];
                item->x = tab->trahere_grid_x;
                item->y = tab->trahere_grid_y;
                tabula_marcare_immundum(tab);
            }

            tab->trahens = FALSUM;
            tab->trahere_hover_target = TABULA_DROP_NONE;
        }
        redde VERUM;
    }

    casus EVENTUS_MUS_DUPLEX: {
        i32 hit = _tabula_invenire_item_ad_punctum(tab, px_x, px_y);

        si (hit != NIHIL_SELECTA) {
            TabulaItem* item = &tab->items[hit];

            si (item->genus == TABULA_ITEM_FILE) {
                si (item->datum.file.est_directorium) {
                    /* Navigate into directory */
                    _tabula_navigare_in_directorium(tab, item);
                } alioquin {
                    /* Open file */
                    tabula_aperire_interne(tab, item);
                }
            }
            alioquin si (item->genus == TABULA_ITEM_CANVAS_LINK) {
                /* Navigate to linked canvas */
                tabula_navigare_ad(tab, item->datum.canvas_link.destination_address);
            }
        } alioquin {
            /* Double-click empty: create new note (future) */
        }
        redde VERUM;
    }

    }

    redde FALSUM;
}
```

---

## Part 7: Persistence

### 7.1 Save Operation

```c
vacuum tabula_salvare(TabulaExplorator* tab)
{
    EntitasRepositorium* repo = tab->ctx->repo;

    /* Save canvas entity */
    Entitas* canvas = tab->canvas_entitas;
    character buffer[XXXII];

    snprintf(buffer, XXXII, "%d", tab->pagina_currens);
    repo->proprietas_ponere(repo->datum, canvas, "pagina_currens", buffer);

    snprintf(buffer, XXXII, "%d", tab->paginae_numerus);
    repo->proprietas_ponere(repo->datum, canvas, "paginae_numerus", buffer);

    /* Save each item */
    per (i32 i = 0; i < tab->numerus_itemorum; i++) {
        TabulaItem* item = &tab->items[i];

        /* Get or create entity */
        Entitas* entitas;
        si (tab->entitas_ids[i]) {
            entitas = repo->capere_entitatem(repo->datum, tab->entitas_ids[i]);
        } alioquin {
            entitas = repo->entitas_creare(repo->datum, "TabulaItem");
            tab->entitas_ids[i] = entitas->id;

            /* Create containment relationship */
            repo->relatio_addere(repo->datum, canvas, "continet", entitas->id);
        }

        /* Save item properties */
        snprintf(buffer, XXXII, "%d", item->x);
        repo->proprietas_ponere(repo->datum, entitas, "x", buffer);

        snprintf(buffer, XXXII, "%d", item->y);
        repo->proprietas_ponere(repo->datum, entitas, "y", buffer);

        snprintf(buffer, XXXII, "%d", item->z_index);
        repo->proprietas_ponere(repo->datum, entitas, "z_index", buffer);

        snprintf(buffer, XXXII, "%d", item->pagina);
        repo->proprietas_ponere(repo->datum, entitas, "pagina", buffer);

        /* Type-specific properties */
        si (item->genus == TABULA_ITEM_FILE) {
            repo->proprietas_ponere(repo->datum, entitas, "item_genus", "file");
            repo->proprietas_ponere(repo->datum, entitas, "host_path",
                item->datum.file.host_path->datum);
            /* FileRef entity handled separately */
        }
        /* ... other types ... */
    }

    tab->immundum = FALSUM;
}
```

### 7.2 Debounced Save

```c
#define TABULA_DEBOUNCE_TEMPUS 2.0  /* 2 seconds */

vacuum tabula_salvare_si_immundum(TabulaExplorator* tab)
{
    si (!tab->immundum) redde;

    f64 tempus_nunc = tempus_secundae();
    si (tempus_nunc - tab->tempus_ultimae_mutationis >= TABULA_DEBOUNCE_TEMPUS) {
        tabula_salvare(tab);
    }
}

vacuum tabula_marcare_immundum(TabulaExplorator* tab)
{
    tab->immundum = VERUM;
    tab->tempus_ultimae_mutationis = tempus_secundae();
}
```

---

## Part 8: Command Integration

### 8.1 Register Commands

```c
vacuum tabula_explorator_init(ContextusWidget* ctx)
{
    registrum_commandi_registrare(ctx->reg_commandi, "canvas",
        _tabula_command_canvas, ctx);
    registrum_commandi_registrare(ctx->reg_commandi, "mount",
        _tabula_command_mount, ctx);
}

interior b32 _tabula_command_canvas(ContextusCommandi* ctx)
{
    ContextusWidget* wctx = (ContextusWidget*)ctx->datum_registratus;

    /* Parse address argument */
    character argumentum[CXXVIII];
    /* ... extract from text after command ... */

    /* Switch to canvas widget with address */
    wctx->commutare_widget(wctx->schirmata_datum, "tabula", argumentum);

    redde VERUM;
}

interior b32 _tabula_command_mount(ContextusCommandi* ctx)
{
    /* Parse path argument */
    /* Add mount to current canvas */
    /* Trigger rescan */
    redde VERUM;
}
```

---

## Part 9: Files to Create

| File | Purpose | Lines (est) |
|------|---------|-------------|
| `include/tabula_explorator.h` | Widget header, structs, API | 200 |
| `lib/tabula_explorator.c` | Main widget implementation | 1500 |
| `include/tabula_icones.h` | Icon mapping definitions | 100 |
| `lib/tabula_icones.c` | Icon lookup functions | 50 |

## Part 10: Files to Modify

| File | Change |
|------|--------|
| `include/registrum_widget.h` | Add TabulaExplorator factory |
| `lib/registrum_widget.c` | Register factory |
| `lib/layout.c` | Add "tabula" tag support |
| `include/schirmata.h` | Add tabula mode (optional) |
| `lib/schirmata.c` | Mode switching |
| `lib/concha.c` | Call tabula_explorator_init |

---

## Part 11: Migration Path

### Phase 1: Basic Canvas (First Milestone)
1. Create TabulaExplorator widget with item rendering
2. Implement single mount, display as icons
3. Click to select, drag to reposition
4. Double-click directory to navigate
5. Double-click file to open in system app
6. Persist item positions to entity repository

### Phase 2: Full Navigation
1. Canvas-to-canvas links
2. Navigation history (back button)
3. Canvas address bar
4. Multiple mounts per canvas

### Phase 3: Pagination
1. Page count calculation based on items
2. Prev/next page buttons
3. Drop zones for cross-page moves
4. Page indicator

### Phase 4: Notes
1. Text note item type
2. Double-click empty to create
3. Text editing (adapt from arx_caeli)

### Phase 5: Annotations & Polish
1. File annotations (layered model)
2. Context menu
3. Keyboard shortcuts
4. Visual refinements

---

## Part 12: Success Criteria (First Milestone)

1. ✓ Widget renders in schirmata layout
2. ✓ Can mount a folder via command or UI
3. ✓ Folder contents display as icons with filename labels
4. ✓ Icons reflect file type (by extension)
5. ✓ Click to select (visual highlight)
6. ✓ Drag to reposition (with ghost)
7. ✓ Positions persist across app restart
8. ✓ Double-click directory navigates into it
9. ✓ Double-click file opens in macOS app
10. ✓ Back button returns to parent/previous
