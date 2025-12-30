# Systema Paradigmata v2 - Implementation Spec

Date: 2025-12-29
Status: Draft
Based on: Codebase exploration + interview brainstorm
Depends On: unified-state-paradigm (builds on top)

---

## Executive Summary

Systema Paradigmata defines OS-like cross-widget paradigms that enable rhubarb to function as a universal computing environment. The core insight: computers transform filetrees of strings → graph (AST/tensor) → filetrees of strings. Different "coordinate systems" project views onto the same underlying semantic graph.

**First Milestone**: Command Palette with global command registry, fuzzy search, and keyboard navigation.

---

## Part 1: Current State Analysis

### 1.1 Existing Command System (registrum_commandi.h/c)

**Current Architecture:**
```c
nomen structura Command {
    character titulus[XXXII];     /* Command name */
    FunctioCommand executare;     /* Function pointer */
    vacuum* datum;                /* Optional context */
} Command;

nomen structura RegistrumCommandi {
    TabulaDispersa* tabula;       /* name → Command* */
    Piscina* piscina;
} RegistrumCommandi;
```

**Registration Pattern (from libro_paginarum.c, concha.c):**
```c
registrum_commandi_registrare(ctx->reg_commandi, "rename", _libro_command_rename, ctx);
registrum_commandi_registrare(ctx->reg_commandi, "goto", _libro_command_goto, ctx);
registrum_commandi_registrare(reg_commandi, "date", _concha_command_date, NIHIL);
```

**Acme-Style Invocation (pagina.c → layout.c):**
1. `pagina_obtinere_regio_ad_punctum()` scans for `$` prefix
2. Extracts command name (stops at whitespace)
3. Builds `ContextusCommandi` with pagina, position, piscina
4. `registrum_commandi_executare()` looks up and calls handler

**Current Gaps:**
| Gap | Impact |
|-----|--------|
| No enumeration API | Cannot list all commands for palette |
| No argument parsing | Each command parses manually from grid text |
| No metadata | No descriptions, categories, key bindings |
| No schema validation | Argument errors discovered at runtime |
| Single invocation mode | Only Acme-style, no palette/programmatic |

### 1.2 Existing Fuzzy Search (quaerere.h/c)

**Capabilities:**
- Subsequence-based matching (fzf-inspired)
- Scoring: consecutive (16pts), word boundary (8pts), case match (4pts)
- Smart case: case-insensitive unless query has uppercase
- Batch filtering with sort: `quaerere_filtrare()`

**API for Palette:**
```c
/* Filter all commands by user query */
QuaerereFiltrumFructus results = quaerere_filtrare(
    query_chorda,
    command_names_array,      /* chorda* array */
    num_commands,
    piscina
);
/* results.elementa = sorted matches, results.puncta = scores */
```

**Word Boundary Detection:**
- Separators: `/`, `_`, `-`, `.`, ` `, `\`
- CamelCase transitions
- String start

Perfect for matching hierarchical command names like `canvas:navigate`.

### 1.3 Widget Event System

**Event Flow:**
```
Fenestra (OS events)
    ↓
Schirmata (global shortcuts: ctrl-a prefix)
    ↓
ManagerWidget (focus routing, TAB cycling)
    ↓
Focused Widget (tractare_eventum)
    ↓
Returns b32: VERUM (consumed) / FALSUM (pass through)
```

**Schirmata Global Shortcut Pattern (schirmata.c lines 1234-1317):**
```c
/* ctrl-a activates prefix mode */
si (clavis == 'a' && (modificantes & MOD_IMPERIUM)) {
    praefixum_activum = VERUM;
    tempus_praefixum = tempus_currens;
    redde VERUM;  /* Consume event */
}

/* In prefix mode, check for command keys */
si (praefixum_activum) {
    si (tempus_currens - tempus_praefixum > PRAEFIXUM_TIMEOUT) {
        praefixum_activum = FALSUM;  /* Timeout after 2 seconds */
    } alioquin si (clavis == 'n') {
        _schirmata_proxima(schirmata);
        praefixum_activum = FALSUM;
        redde VERUM;
    }
    /* ... more commands ... */
}
```

**Modal Widget Substitution Pattern:**
```c
/* To show modal/popup: */
manager->widgets[index].datum = popup_datum;
manager->widgets[index].reddere = popup_reddere;
manager->widgets[index].tractare_eventum = popup_tractare;
manager_widget_ponere_focus(manager, index);

/* To close modal: restore original widget */
```

**Text Input Pattern (from librarium_visus):**
```c
/* Accumulate typed characters in Xar */
si (eventus->datum.clavis.typus >= ' ' && eventus->datum.clavis.typus <= '~') {
    *(character*)xar_addere(query_chars) = eventus->datum.clavis.typus;
}
si (eventus->datum.clavis.clavis == CLAVIS_RETRORSUM) {
    si (xar_numerus(query_chars) > 0) {
        xar_removere_ultimus(query_chars);
    }
}
```

### 1.4 STML Parser (stml.h/c)

**Node Structure:**
```c
nomen structura StmlNodus {
    StmlNodusGenus    genus;          /* ELEMENTUM, TEXTUS, etc. */
    chorda*           titulus;        /* Tag name (interned) */
    chorda*           valor;          /* Text content */
    Xar*              attributa;      /* StmlAttributum array */
    Xar*              liberi;         /* Child nodes */
    StmlNodus*        parens;
    /* ... capture operators, fragments ... */
} StmlNodus;
```

**Query API:**
```c
StmlNodus* stml_invenire_liberum(parent, "tagname");
Xar* stml_invenire_omnes_liberos(parent, "tagname", piscina);
chorda* stml_attributum_capere(node, "attrname");
b32 stml_attributum_habet(node, "attrname");
chorda stml_textus_internus(node, piscina);

/* CSS-like selectors via selectio.h */
StmlNodus* stml_quaerere(root, "arg[required]", piscina, intern);
Xar* stml_quaerere_omnes(root, ".error", piscina, intern);
```

**No Built-in Schema Validation:** Parser handles syntax only. Application layer must validate semantics.

### 1.5 Entity Repository Patterns

**Genus Hierarchy:**
```
Genus::Genus (root type)
├── contains → Genus::Command
│   ├── contains → Genus::Command::File
│   │   ├── contains → Genus::Command::File::Open
│   │   └── contains → Genus::Command::File::Save
│   └── contains → Genus::Command::Edit
└── contains → Genus::ProprietasDefinitio
```

**Scaffolding (deterministic IDs via UUIDv5):**
```c
/* Same (genus, title) → same entity every time */
Entitas* cmd = repo->entitas_scaffoldare(repo->datum, "Command", "canvas:navigate");
/* Idempotent - safe to call multiple times */
```

**Tag-Based Queries:**
```c
/* Find all commands */
Xar* commands = quaerere_cum_praefixo_notae(repo, "#command::");

/* Find by exact tag */
Xar* file_cmds = quaerere_cum_nota(repo, "#command::file");
```

**Relationship Navigation:**
```c
/* Forward: what does this contain? */
Xar* children = capere_entitates_relatae(repo, parent, "contains");

/* Backward: what contains this? */
Xar* parents = capere_relationes_ad(repo, child_id);
```

---

## Part 2: Target Architecture

### 2.1 Enhanced Command Registry

**New Command Structure:**
```c
nomen structura Imperium {
    chorda*             nomen;              /* "canvas:navigate" */
    chorda*             descriptio;         /* Human-readable description */
    chorda*             categoria;          /* "navigation", "file", etc. */
    StmlNodus*          schema_argumentorum;/* Parsed STML schema */
    ImperiumOutputModus output_modus;       /* How output is handled */
    FunctioImperium     executare;          /* Handler function */
    vacuum*             datum;              /* Registered context */
    chorda*             clavis_velox;       /* Key binding "ctrl+o" */
} Imperium;

nomen enumeratio {
    IMPERIUM_OUTPUT_MUTATIO  = 0,  /* Modifies graph, UI reflects */
    IMPERIUM_OUTPUT_INLINE   = 1,  /* Renders at command location */
    IMPERIUM_OUTPUT_WIDGET   = 2   /* Spawns new widget */
} ImperiumOutputModus;
```

**New Registration API:**
```c
vacuum
registrum_commandi_registrare_plenum(
    RegistrumCommandi*    reg,
    constans character*   nomen,
    constans character*   descriptio,
    constans character*   categoria,
    constans character*   schema_stml,      /* NIHIL for no args */
    ImperiumOutputModus   output_modus,
    FunctioImperium       executare,
    vacuum*               datum,
    constans character*   clavis_velox);    /* NIHIL for no binding */
```

**Backward-Compatible Wrapper:**
```c
/* Existing registrare() calls continue to work */
vacuum
registrum_commandi_registrare(
    RegistrumCommandi*    reg,
    constans character*   nomen,
    FunctioCommand        executare,        /* Old signature */
    vacuum*               datum)
{
    /* Convert to new format with defaults */
    registrum_commandi_registrare_plenum(
        reg, nomen, NIHIL, "general", NIHIL,
        IMPERIUM_OUTPUT_MUTATIO,
        (FunctioImperium)executare, datum, NIHIL);
}
```

**Enumeration API:**
```c
nomen structura ImperiumIterator {
    TabulaIterator iter;
} ImperiumIterator;

ImperiumIterator
registrum_commandi_iterator_initium(RegistrumCommandi* reg);

b32
registrum_commandi_iterator_proximum(
    ImperiumIterator* iter,
    Imperium**        imperium_out);

/* Convenience: get all command names as chorda array */
Xar*
registrum_commandi_nomina(RegistrumCommandi* reg, Piscina* piscina);
```

### 2.2 Argument Schema Definition

**STML Schema Format:**
```xml
<schema>
    <arg name="address" type="chorda" required="true">
        Canvas address to navigate to
    </arg>
    <arg name="animate" type="b32" default="true">
        Animate the navigation transition
    </arg>
    <arg name="speed" type="f32" default="1.0" min="0.1" max="5.0">
        Animation speed multiplier
    </arg>
</schema>
```

**Schema Processing:**
```c
nomen structura ArgumentumDefinitio {
    chorda*    nomen;
    chorda*    typus;       /* "chorda", "i32", "f32", "b32" */
    b32        requiritur;
    chorda*    valor_defectus;
    chorda*    minimum;     /* For numeric types */
    chorda*    maximum;
    chorda*    descriptio;
} ArgumentumDefinitio;

/* Parse schema STML into structured array */
Xar*
imperium_schema_legere(StmlNodus* schema_nodus, Piscina* piscina);

/* Validate arguments against schema */
b32
imperium_argumenta_validare(
    Xar*               schema,      /* ArgumentumDefinitio array */
    TabulaDispersa*    argumenta,   /* Provided arguments */
    chorda*            error_out,   /* Error message if invalid */
    Piscina*           piscina);
```

### 2.3 Command Palette Widget

**Widget Structure:**
```c
nomen structura PalettaImperorum {
    ContextusWidget*    ctx;
    RegistrumCommandi*  registrum;

    /* Input state */
    Xar*                query_chars;       /* Xar<character> */
    i32                 cursor_positio;

    /* Results state */
    Xar*                resultata;         /* Xar<Imperium*> filtered */
    Xar*                puncta;            /* Xar<s32> scores */
    i32                 index_selecta;     /* Highlighted result */
    i32                 offset_visibilis;  /* Scroll offset */

    /* Layout */
    i32                 maximus_visibilis; /* Max results shown (8?) */
    i32                 latitudo_inputis;  /* Input field width */

    /* Lifecycle */
    b32                 visibilis;
    FunctioClausura     post_selectionem;  /* Callback after selection */
    vacuum*             post_datum;
} PalettaImperorum;
```

**Event Handling:**
```c
hic_manens b32
_paletta_tractare_eventum(vacuum* datum, constans Eventus* eventus)
{
    PalettaImperorum* paletta = (PalettaImperorum*)datum;

    si (eventus->genus != EVENTUS_CLAVIS_DEPRESSUS) {
        redde FALSUM;
    }

    clavis_t clavis = eventus->datum.clavis.clavis;
    character typus = eventus->datum.clavis.typus;

    /* Escape: close palette */
    si (clavis == CLAVIS_EFFUGIUM) {
        _paletta_claudere(paletta);
        redde VERUM;
    }

    /* Enter: execute selected command */
    si (clavis == CLAVIS_REDITUS) {
        si (paletta->index_selecta >= 0 &&
            paletta->index_selecta < (i32)xar_numerus(paletta->resultata)) {
            Imperium* imp = *(Imperium**)xar_obtinere(
                paletta->resultata, (u32)paletta->index_selecta);
            _paletta_executare(paletta, imp);
        }
        redde VERUM;
    }

    /* Arrow up: previous result */
    si (clavis == CLAVIS_SURSUM) {
        si (paletta->index_selecta > 0) {
            paletta->index_selecta--;
            _paletta_adjustare_scroll(paletta);
        }
        redde VERUM;
    }

    /* Arrow down: next result */
    si (clavis == CLAVIS_DEORSUM) {
        si (paletta->index_selecta < (i32)xar_numerus(paletta->resultata) - 1) {
            paletta->index_selecta++;
            _paletta_adjustare_scroll(paletta);
        }
        redde VERUM;
    }

    /* Backspace: delete character */
    si (clavis == CLAVIS_RETRORSUM) {
        si (xar_numerus(paletta->query_chars) > 0) {
            xar_removere(paletta->query_chars,
                         xar_numerus(paletta->query_chars) - 1);
            _paletta_filtrare(paletta);
        }
        redde VERUM;
    }

    /* Printable character: add to query */
    si (typus >= ' ' && typus <= '~') {
        *(character*)xar_addere(paletta->query_chars) = typus;
        _paletta_filtrare(paletta);
        redde VERUM;
    }

    redde FALSUM;
}
```

**Filtering Logic:**
```c
hic_manens vacuum
_paletta_filtrare(PalettaImperorum* paletta)
{
    /* Build query chorda from accumulated chars */
    chorda query;
    query.datum = (i8*)xar_obtinere_primum(paletta->query_chars);
    query.mensura = (i32)xar_numerus(paletta->query_chars);

    /* Get all command names */
    Xar* nomina = registrum_commandi_nomina(
        paletta->registrum, paletta->ctx->piscina);

    /* Build chorda array for fuzzy search */
    chorda* candidati = (chorda*)piscina_capere(
        paletta->ctx->piscina, xar_numerus(nomina) * sizeof(chorda));

    per (u32 i = 0; i < xar_numerus(nomina); i++) {
        candidati[i] = *(chorda*)xar_obtinere(nomina, i);
    }

    /* Filter with fuzzy search */
    QuaerereFiltrumFructus fructus = quaerere_filtrare(
        query, candidati, (i32)xar_numerus(nomina), paletta->ctx->piscina);

    /* Convert names back to Imperium pointers */
    xar_vacuare(paletta->resultata);
    xar_vacuare(paletta->puncta);

    per (i32 i = 0; i < fructus.numerus; i++) {
        Imperium* imp = registrum_commandi_capere(
            paletta->registrum, fructus.elementa[i].datum);
        *(Imperium**)xar_addere(paletta->resultata) = imp;
        *(s32*)xar_addere(paletta->puncta) = fructus.puncta[i];
    }

    /* Reset selection to first result */
    paletta->index_selecta = (fructus.numerus > 0) ? 0 : -1;
    paletta->offset_visibilis = 0;
}
```

**Rendering:**
```c
hic_manens vacuum
_paletta_reddere(vacuum* datum, Tabula* tabula)
{
    PalettaImperorum* paletta = (PalettaImperorum*)datum;

    /* Calculate centered position */
    i32 latitudo = 40;  /* Characters wide */
    i32 altitudo = paletta->maximus_visibilis + 3;  /* Results + input + border */
    i32 x = (tabula->latitudo - latitudo) / 2;
    i32 y = 4;  /* Near top of screen */

    /* Draw border/background */
    _paletta_reddere_scatula(tabula, x, y, latitudo, altitudo);

    /* Draw input field with query */
    _paletta_reddere_input(tabula, paletta, x + 1, y + 1, latitudo - 2);

    /* Draw results */
    i32 y_resultatum = y + 2;
    i32 visibilis_finis = paletta->offset_visibilis + paletta->maximus_visibilis;

    per (i32 i = paletta->offset_visibilis;
         i < visibilis_finis && i < (i32)xar_numerus(paletta->resultata);
         i++) {
        Imperium* imp = *(Imperium**)xar_obtinere(paletta->resultata, (u32)i);
        b32 selecta = (i == paletta->index_selecta);

        _paletta_reddere_resultatum(
            tabula, imp, x + 1, y_resultatum, latitudo - 2, selecta);
        y_resultatum++;
    }
}
```

### 2.4 Global Keyboard Activation

**Integration in Schirmata:**
```c
/* In schirmata_tractare_eventum, before prefix mode check */

/* Ctrl+K or Ctrl+P: open command palette */
si ((clavis == 'k' || clavis == 'p') && (modificantes & MOD_IMPERIUM)) {
    _schirmata_aperire_palettam(schirmata);
    redde VERUM;
}

/* ... existing prefix mode handling ... */
```

**Palette Activation:**
```c
hic_manens vacuum
_schirmata_aperire_palettam(Schirmata* schirmata)
{
    Schirma* schirma = &schirmata->schirmae[schirmata->index_currens];

    /* Store current widget state for restoration */
    schirmata->paletta_praecedens_focus = schirma->manager->focus_index;
    schirmata->paletta_praecedens_widget =
        schirma->manager->widgets[SCHIRMA_WIDGET_PALETTA];

    /* Substitute palette widget */
    schirma->manager->widgets[SCHIRMA_WIDGET_PALETTA].datum =
        &schirmata->paletta;
    schirma->manager->widgets[SCHIRMA_WIDGET_PALETTA].reddere =
        _paletta_reddere;
    schirma->manager->widgets[SCHIRMA_WIDGET_PALETTA].tractare_eventum =
        _paletta_tractare_eventum;

    /* Give focus to palette */
    manager_widget_ponere_focus(schirma->manager, SCHIRMA_WIDGET_PALETTA);

    /* Initialize palette state */
    xar_vacuare(schirmata->paletta.query_chars);
    _paletta_filtrare(&schirmata->paletta);
    schirmata->paletta.visibilis = VERUM;
}
```

---

## Part 3: Command as Entity Pattern

For the full Systema Paradigmata vision, commands can also be stored as entities:

### 3.1 Command Entity Structure

```
Entitas: "Imperium::canvas:navigate"
├─ genus: "Imperium"
├─ proprietates:
│   ├─ nomen: "canvas:navigate"
│   ├─ descriptio: "Navigate to a canvas by address"
│   ├─ categoria: "navigation"
│   ├─ output_modus: "mutatio"
│   ├─ clavis_velox: "ctrl+g"
│   └─ schema: "<schema><arg name='address' type='chorda' required='true'/></schema>"
├─ relationes:
│   └─ "in_categoria" → "Categoria::navigation"
└─ notae: ["#command", "#command::navigation", "#builtin"]
```

### 3.2 Dynamic Command Loading

```c
/* Load commands from entity repository */
vacuum
registrum_commandi_carcare_ex_repositorio(
    RegistrumCommandi*   reg,
    EntitasRepositorium* repo,
    Piscina*             piscina)
{
    /* Find all command entities */
    Xar* imperia = quaerere_cum_nota(repo, "#command");

    per (u32 i = 0; i < xar_numerus(imperia); i++) {
        Entitas* ent = *(Entitas**)xar_obtinere(imperia, i);

        /* Extract properties */
        chorda* nomen = entitas_proprietas_capere(ent, "nomen");
        chorda* descriptio = entitas_proprietas_capere(ent, "descriptio");
        /* ... etc ... */

        /* Register (handler must be provided separately) */
        FunctioImperium handler = _capere_handler_pro_nomine(nomen);
        si (handler != NIHIL) {
            registrum_commandi_registrare_plenum(
                reg, nomen->datum,
                descriptio ? descriptio->datum : NIHIL,
                /* ... */);
        }
    }
}
```

### 3.3 State Machine Integration (Future)

Commands become transitions in state machines:

```
MachinaStatus: "deployment_flow"
├─ Status: "draft"
│   └─ Transitio: "submit"
│       └─ imperium → "deployment:submit" (valid from this state)
├─ Status: "review"
│   ├─ Transitio: "approve"
│   │   └─ imperium → "deployment:approve"
│   └─ Transitio: "reject"
│       └─ imperium → "deployment:reject"
```

When user is in "draft" state, only `deployment:submit` appears as available command.

---

## Part 4: Files to Create/Modify

### 4.1 New Files

| File | Purpose |
|------|---------|
| `include/imperium.h` | Enhanced command structure, schema types |
| `lib/imperium.c` | Schema parsing, argument validation |
| `include/paletta_imperorum.h` | Command palette widget header |
| `lib/paletta_imperorum.c` | Palette widget implementation |

### 4.2 Files to Modify

| File | Changes |
|------|---------|
| `include/registrum_commandi.h` | Add Imperium struct, enumeration API, new registration |
| `lib/registrum_commandi.c` | Implement enumeration, store enhanced metadata |
| `include/schirmata.h` | Add palette state, activation key constant |
| `lib/schirmata.c` | Ctrl+K handler, palette activation/deactivation |
| `lib/libro_paginarum.c` | Update command registrations with metadata |
| `lib/concha.c` | Update command registrations with metadata |
| `lib/calendario_visus.c` | Update command registrations with metadata |

---

## Part 5: Migration Plan

### Phase 1: Enhanced Registry (Foundation)

1. Add `Imperium` struct to `registrum_commandi.h`
2. Add `registrum_commandi_registrare_plenum()`
3. Add iterator API for enumeration
4. Keep existing `registrum_commandi_registrare()` as wrapper
5. Update existing command registrations to use new API (optional metadata)

**Test:** Enumerate all commands, verify metadata accessible.

### Phase 2: Command Palette Widget

1. Create `paletta_imperorum.h/c`
2. Implement filtering with `quaerere_filtrare()`
3. Implement keyboard navigation
4. Implement rendering
5. Add Ctrl+K activation in schirmata

**Test:** Open palette, type to filter, navigate with arrows, execute command.

### Phase 3: Argument Schemas

1. Create `imperium.h/c` for schema handling
2. Define STML schema format
3. Implement `imperium_schema_legere()`
4. Implement `imperium_argumenta_validare()`
5. Add schema to key commands (goto, rename, etc.)

**Test:** Command with required arg shows error if missing.

### Phase 4: Key Bindings

1. Add `clavis_velox` to Imperium
2. Parse key binding strings ("ctrl+o", "alt+s", etc.)
3. Build binding→command map at registration
4. Check bindings in schirmata before palette

**Test:** Ctrl+O opens file (if bound), shows in palette hint.

---

## Part 6: Detailed API Additions

### 6.1 registrum_commandi.h Additions

```c
/* --- New Types --- */

nomen enumeratio {
    IMPERIUM_OUTPUT_MUTATIO  = 0,
    IMPERIUM_OUTPUT_INLINE   = 1,
    IMPERIUM_OUTPUT_WIDGET   = 2
} ImperiumOutputModus;

nomen structura Imperium {
    chorda*             nomen;
    chorda*             descriptio;
    chorda*             categoria;
    StmlNodus*          schema_argumentorum;
    ImperiumOutputModus output_modus;
    FunctioImperium     executare;
    vacuum*             datum;
    chorda*             clavis_velox;
} Imperium;

nomen b32 (*FunctioImperium)(ContextusCommandi* ctx, TabulaDispersa* argumenta);

/* --- New Registration --- */

vacuum
registrum_commandi_registrare_plenum(
    RegistrumCommandi*    reg,
    constans character*   nomen,
    constans character*   descriptio,
    constans character*   categoria,
    constans character*   schema_stml,
    ImperiumOutputModus   output_modus,
    FunctioImperium       executare,
    vacuum*               datum,
    constans character*   clavis_velox);

/* --- Enumeration --- */

nomen structura ImperiumIterator {
    TabulaIterator iter;
} ImperiumIterator;

ImperiumIterator
registrum_commandi_iterator_initium(RegistrumCommandi* reg);

b32
registrum_commandi_iterator_proximum(
    ImperiumIterator* iter,
    Imperium**        imperium_out);

Xar*
registrum_commandi_nomina(RegistrumCommandi* reg, Piscina* piscina);

/* --- Lookup --- */

Imperium*
registrum_commandi_capere(RegistrumCommandi* reg, constans character* nomen);

Imperium*
registrum_commandi_capere_chorda(RegistrumCommandi* reg, chorda nomen);
```

### 6.2 imperium.h

```c
#ifndef IMPERIUM_H
#define IMPERIUM_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "stml.h"
#include "tabula_dispersa.h"

/* --- Argument Definition --- */

nomen structura ArgumentumDefinitio {
    chorda*    nomen;
    chorda*    typus;          /* "chorda", "i32", "f32", "b32" */
    b32        requiritur;
    chorda*    valor_defectus;
    chorda*    minimum;
    chorda*    maximum;
    chorda*    descriptio;
} ArgumentumDefinitio;

/* --- Schema API --- */

Xar*
imperium_schema_legere(
    StmlNodus*            schema_nodus,
    Piscina*              piscina,
    InternamentumChorda*  intern);

b32
imperium_argumenta_validare(
    Xar*               schema,
    TabulaDispersa*    argumenta,
    chorda*            error_out,
    Piscina*           piscina);

/* --- Argument Parsing --- */

TabulaDispersa*
imperium_argumenta_legere_ex_textu(
    constans character*  textus,
    i32                  longitudo,
    Piscina*             piscina,
    InternamentumChorda* intern);

TabulaDispersa*
imperium_argumenta_legere_ex_stml(
    StmlNodus*           argumenta_nodus,
    Piscina*             piscina,
    InternamentumChorda* intern);

#endif /* IMPERIUM_H */
```

### 6.3 paletta_imperorum.h

```c
#ifndef PALETTA_IMPERORUM_H
#define PALETTA_IMPERORUM_H

#include "latina.h"
#include "widget.h"
#include "registrum_commandi.h"
#include "xar.h"

nomen structura PalettaImperorum {
    ContextusWidget*    ctx;
    RegistrumCommandi*  registrum;

    /* Input */
    Xar*                query_chars;
    i32                 cursor_positio;

    /* Results */
    Xar*                resultata;
    Xar*                puncta;
    i32                 index_selecta;
    i32                 offset_visibilis;

    /* Layout */
    i32                 x, y;
    i32                 latitudo, altitudo;
    i32                 maximus_visibilis;

    /* State */
    b32                 visibilis;

    /* Callbacks */
    vacuum              (*post_executionem)(vacuum* datum, Imperium* imp);
    vacuum              (*post_clausuram)(vacuum* datum);
    vacuum*             callback_datum;
} PalettaImperorum;

/* --- Lifecycle --- */

PalettaImperorum*
paletta_imperorum_creare(
    Piscina*            piscina,
    ContextusWidget*    ctx,
    RegistrumCommandi*  registrum);

vacuum
paletta_imperorum_aperire(PalettaImperorum* paletta);

vacuum
paletta_imperorum_claudere(PalettaImperorum* paletta);

/* --- Widget Interface --- */

vacuum
paletta_imperorum_reddere(vacuum* datum, Tabula* tabula);

b32
paletta_imperorum_tractare_eventum(vacuum* datum, constans Eventus* eventus);

#endif /* PALETTA_IMPERORUM_H */
```

---

## Part 7: Success Criteria

### Milestone 1: Command Palette MVP

1. **Ctrl+K opens palette** centered on screen
2. **Typing filters commands** via fuzzy search
3. **Arrow keys navigate** highlighted selection
4. **Enter executes** selected command
5. **Escape closes** palette
6. **All widgets' commands visible** in single palette
7. **Commands show descriptions** when available

### Future Milestones

2. **Argument validation** before execution
3. **Key binding display** next to command names
4. **State-aware filtering** (only valid commands shown)
5. **Command history** (recent commands at top)
6. **Inline argument input** for commands with required args

---

## Part 8: Open Questions

1. **Palette positioning**: Centered? At cursor? Configurable?
2. **Command namespacing**: Enforce `category:name` or allow flat names?
3. **Argument input UX**: Inline fields in palette? Separate dialog?
4. **Result limit**: Show all matches or cap at N (50? 100?)?
5. **Category filtering**: Allow filtering by category (Ctrl+K then @file)?

---

## Appendix: Key File Locations

| Component | Header | Implementation |
|-----------|--------|----------------|
| Command registry | include/registrum_commandi.h | lib/registrum_commandi.c |
| Fuzzy search | include/quaerere.h | lib/quaerere.c |
| Widget base | include/widget.h | lib/widget.c |
| Schirmata | include/schirmata.h | lib/schirmata.c |
| STML parser | include/stml.h | lib/stml.c |
| Entity repo | include/entitas_repositorium.h | lib/entitas_repositorium_impl.c |
| Hash table | include/tabula_dispersa.h | lib/tabula_dispersa.c |
| Dynamic array | include/xar.h | lib/xar.c |
