# Scientia V2 - Implementation-Specific Specification

Date: 2026-01-02
Status: Draft
Based on: scientia.md, scientia-brainstorm.md, scientia-simulation-brainstorm.md, codebase exploration

---

## Executive Summary

This document grounds the Scientia encyclopedia/education system in the existing rhubarb codebase. Based on exploration, we have strong foundations (STML parsing, librarium reader patterns, navigator DAG traversal, arx_caeli spatial navigation) but critical gaps (no card rendering, no skill tree widget, no quiz system).

**Key Finding**: The codebase already implements the three hardest patterns Scientia needs: byte-offset content addressing (librarium_lector), DAG navigation with history (navigator_entitatum), and spatial room navigation (arx_caeli). Implementation is primarily composition of existing patterns.

---

## Codebase Analysis Summary

### What Exists and Works

| Component | Status | Location |
|-----------|--------|----------|
| Content reader with addressing | Complete | `include/librarium_lector.h`, `lib/librarium_lector.c` |
| Byte offset pagination | Complete | `include/paginarium.h`, `lib/paginarium.c` |
| Sentence-level pagination | Complete | `include/sententia_paginarium.h` |
| STML parsing (tree structure) | Complete | `include/stml.h`, `lib/stml.c` |
| Entity system | Complete | `include/entitas.h`, `lib/entitas_repositorium_impl.c` |
| DAG navigation with history | Complete | `include/navigator_entitatum.h`, `lib/navigator_entitatum.c` |
| Spatial room navigation | Complete | `include/arx_caeli.h`, `lib/arx_caeli.c` |
| Split view widget manager | Complete | `include/widget.h`, `lib/widget.c` |
| Progress persistence | Complete | Via entity repository |
| Two-column 30/70 layout | Complete | `lib/navigator_entitatum.c:2476-2482` |

### Critical Gaps

| Component | Status | Impact |
|-----------|--------|--------|
| Science content STML schema | Missing | No structure for science definitions |
| Skill tree widget | Missing | No DAG visualization for skills |
| Card rendering | Missing | No game card display |
| Quiz system | Missing | No spaced repetition |
| Gloss display widget | Missing | No commentary overlay |
| Print/PDF export | Missing | Blocked by Scriptoria sphere |
| Paragraph:sentence addressing | Missing | Content is byte-offset only |

---

## Architecture Decision: Content Addressing

**Problem**: Librarium uses byte offsets, but Scientia needs semantic paragraph:sentence addressing.

**Solution**: Layer semantic addressing on top of byte offsets:

```c
/* Proposed addressing structure */
nomen structura {
    chorda*  scientia_id;      /* Science identifier */
    chorda*  caput_id;         /* Chapter ID */
    i32      paragraphus;      /* Paragraph index (0-based) */
    i32      sententia;        /* Sentence index within paragraph */
    i64      offset_byte;      /* Cached byte offset for display */
    chorda*  hash_contentus;   /* Content hash for version tracking */
} InscriptioScientiae;

/* Multiple addressing modes per the brainstorm */
nomen enumeratio {
    INSCRIPTIO_MODUS_NUMERICUS,    /* paragraph:sentence indices */
    INSCRIPTIO_MODUS_HASH,         /* Content hash lookup */
    INSCRIPTIO_MODUS_SEMANTICUS    /* Paragraph UUID */
} InscriptioModus;
```

**Rationale**: Content can shift during editing. Versioned addresses with hash fallback enable gloss migration when underlying text changes.

---

## Entity Patterns

### Following Existing Conventions

Based on `navigator_entitatum.c` and `entitas_repositorium_impl.c`:

```c
/* Create science entity */
Entitas* scientia = repo->entitas_scaffoldare(
    repo->datum, "Scientia", "physics-classical");

/* Set properties */
repo->proprietas_ponere(repo->datum, scientia,
    "nomen", "Physica Classica");
repo->proprietas_ponere(repo->datum, scientia,
    "categoria", "naturalis");
repo->proprietas_ponere(repo->datum, scientia,
    "status", "PUBLICATUS");

/* Add relationships */
repo->relatio_addere(repo->datum, scientia,
    "continet", chapter_entity_id);
repo->relatio_addere(repo->datum, scientia,
    "requirit", prerequisite_science_id);

/* Add tags for fast queries */
repo->nota_addere(repo->datum, scientia, "#scientia");
repo->nota_addere(repo->datum, scientia, "#categoria::naturalis");
```

### Query Patterns

```c
/* Find all sciences */
Xar* scientiae = repo->quaerere_cum_praefixo_notae(
    repo->datum, "#scientia");

/* Find prerequisites (DAG traversal) */
Xar* requisita = repo->capere_entitates_relatae(
    repo->datum, scientia_entity,
    chorda_internare_ex_literis(intern, "requirit"));
```

---

## Entity Definitions

### Scientia (Science)

```c
genus = "Scientia"

proprietates:
    nomen               /* Display name */
    categoria           /* naturalis | humana | divina */
    descriptio          /* Brief description */
    status              /* SCHEMA | IN_PROGRESSU | PUBLICATUS */

relationes:
    continet → Caput entities (chapters)
    requirit → Scientia entities (prerequisites)
    habet_artem → Ars entities (skill tree)
    habet_chartas → Charta entities (cards)

notae:
    #scientia
    #categoria::naturalis | #categoria::humana | #categoria::divina
```

### Caput (Chapter)

```c
genus = "Caput"

proprietates:
    titulus             /* Chapter title */
    ordo                /* Display order (0-indexed) */
    via_contentus       /* Path to STML content file */

relationes:
    in_scientia → Scientia entity (parent)
    continet → Paragraphus entities

notae:
    #caput
    #scientia::<science_id>
```

### Paragraphus (Paragraph)

```c
genus = "Paragraphus"

proprietates:
    ordo                /* Order within chapter */
    offset_initium      /* Byte offset start */
    offset_finis        /* Byte offset end */
    hash_contentus      /* SHA-256 of content for versioning */
    uuid                /* Stable identifier across edits */

relationes:
    in_capite → Caput entity (parent)
    habet_glossam → Glossa entities

notae:
    #paragraphus
    #caput::<chapter_id>
```

### Ars (Skill)

```c
genus = "Ars"

proprietates:
    nomen               /* Skill name */
    descriptio          /* What this skill represents */
    gradus              /* Difficulty tier (I, II, III, etc.) */
    status              /* CLAUSA | APERTA | ACQUISITA */

relationes:
    in_scientia → Scientia entity
    requirit → Ars entities (prerequisites - DAG)
    aperit → Ars entities (unlocks)
    docetur_in → Paragraphus entities (where taught)

notae:
    #ars
    #scientia::<science_id>
    #gradus::I | #gradus::II | etc.
```

### Charta (Card)

```c
genus = "Charta"

proprietates:
    nomen               /* Card name */
    genus_chartae       /* INSTRUMENTUM | MATERIA | PERSONA | LOCUS | ARS */
    descriptio          /* Card description */
    effectus            /* Effect text (rules) */
    notae_hierachicae   /* Hierarchical tags: "Lignum>Quercus" */

relationes:
    in_scientia → Scientia entity
    requirit_artem → Ars entity (skill required to use)
    agit_in → Charta entities (what this affects)

notae:
    #charta
    #genus::instrumentum | #genus::materia | etc.
    #scientia::<science_id>
```

### Glossa (Commentary)

```c
genus = "Glossa"

proprietates:
    contentus           /* Gloss text */
    auctor_clavis       /* License key of author */
    data_creationis     /* Timestamp */
    typus               /* EXPLICATIO | OBIECTIO | EXEMPLUM | CITATIO */

relationes:
    glossat → Paragraphus entity (what it comments on)
    respondet → Glossa entity (reply to another gloss)

notae:
    #glossa
    #typus::explicatio | #typus::obiectio | etc.
```

---

## Widget Implementation

### ScientiaLectorVisus (Science Reader)

Based on `librarium_lector.c` patterns (~1500 lines estimated):

```c
nomen enumeratio {
    SCIENTIA_MODUS_INDEX,         /* Science list */
    SCIENTIA_MODUS_TOC,           /* Chapter table of contents */
    SCIENTIA_MODUS_LECTIO,        /* Reading mode */
    SCIENTIA_MODUS_LECTIO_GLOSSA  /* Reading with gloss panel */
} ScientiaLectorModus;

nomen structura {
    ContextusWidget* ctx;
    EntitasRepositorium* repo;

    /* Current science/chapter */
    Entitas* scientia_currens;
    Entitas* caput_currens;

    /* Content addressing */
    SententiaPaginariumResultus* sententia_paginarium;
    Xar* paragraphi;              /* Paragraphus entities */
    i32 paragraphus_currens;
    i32 sententia_currens;

    /* Display state */
    ScientiaLectorModus modus;
    i32 pagina_currens;
    i32 selectio;

    /* Gloss panel (30/70 split like navigator) */
    Xar* glossae_currentes;       /* Glosses for current paragraph */
    i32 glossa_selecta;
} ScientiaLectorVisus;
```

### Key Bindings (following rhubarb conventions)

```
j/k         Navigate items / sentences
l           Enter (select science, open chapter, view gloss)
h           Back (to chapter list, to science list)
Tab         Toggle gloss panel (30/70 split)
g           Go to gloss for current paragraph
/           Search within science
1-9         Jump to chapter by number
```

### ArsVisus (Skill Tree Widget)

Based on `navigator_entitatum.c` DAG patterns (~1200 lines estimated):

```c
nomen enumeratio {
    ARS_MODUS_ARBOR,              /* Tree overview */
    ARS_MODUS_FOCUS,              /* Single skill focus */
    ARS_MODUS_VIA                 /* Path view (learning sequence) */
} ArsVisusModus;

nomen structura {
    ContextusWidget* ctx;
    EntitasRepositorium* repo;

    /* Current state */
    Entitas* scientia;
    Entitas* ars_currens;
    ArsVisusModus modus;

    /* DAG navigation (like navigator_entitatum) */
    Xar* via;                     /* ItemHistoriae stack */
    Xar* artes_visibiles;         /* Current view's skills */

    /* Pagination (following navigator pattern) */
    i32 pagina_currens;
    i32 items_per_pagina;
    i32 selectio;

    /* Layout (Zelda 1 room style per brainstorm) */
    i32 camera_x;                 /* Current "room" offset */
    i32 camera_y;
} ArsVisus;
```

### DAG Visualization Strategy

Per the brainstorm, three visualization modes:

1. **Folder-style** (like navigator_entitatum)
   - Click skill to see its prerequisites/unlocks
   - 30/70 split: parent skills | current skill details

2. **Zelda 1 room-based** (like arx_caeli)
   - Each "room" shows skills at a particular depth tier
   - Navigate N/S/E/W to adjacent tiers
   - Use arx_caeli navigation patterns

3. **Memory palace** (spatial mode)
   - Full arx_caeli integration
   - Skills as positioned cards in rooms
   - Build your own mental map

```c
nomen enumeratio {
    ARS_VISUS_FOLDER,             /* Navigator-style drilling */
    ARS_VISUS_ZELDA,              /* Room-based tier navigation */
    ARS_VISUS_PALATIUM            /* Full spatial memory palace */
} ArsVisusTipus;
```

---

## STML Content Schema

### Science Definition Format

Following `librarium.stml` patterns:

```xml
<scientia id="physics-classical" categoria="naturalis">
  <nomen>Physica Classica</nomen>
  <descriptio>Studium motus et energiae...</descriptio>

  <curriculum>
    <caput numero="1" id="principia-motus">
      <titulus>Principia Motus</titulus>

      <sectio>
        <paragraphus uuid="p-001">
          Corpus omne perseverare in statu suo quiescendi
          vel movendi uniformiter in directum, nisi quatenus
          illud a viribus impressis cogitur statum suum mutare.
        </paragraphus>

        <paragraphus uuid="p-002">
          Mutationem motus proportionalem esse vi motrici
          impressae, et fieri secundum lineam rectam qua
          vis illa imprimitur.
        </paragraphus>
      </sectio>
    </caput>
  </curriculum>

  <artes>
    <ars id="lex-prima" gradus="I">
      <nomen>Lex Prima</nomen>
      <descriptio>Comprehensio inertiae</descriptio>
      <docetur_in>#p-001</docetur_in>
    </ars>

    <ars id="lex-secunda" gradus="II">
      <nomen>Lex Secunda</nomen>
      <descriptio>F=ma</descriptio>
      <requirit>#lex-prima</requirit>
      <docetur_in>#p-002</docetur_in>
    </ars>
  </artes>

  <chartae>
    <charta genus="instrumentum" id="libra">
      <nomen>Libra</nomen>
      <notae>Instrumentum>Mensura>Massa</notae>
      <effectus>Mensurat massam materiae</effectus>
    </charta>
  </chartae>
</scientia>
```

### Content File Organization

```
data/
  scientiae/
    physics-classical/
      scientia.stml          /* Main definition */
      curriculum/
        01-principia-motus.stml
        02-dynamica.stml
      artes.stml             /* Skill tree definition */
      chartae.stml           /* Card definitions */
      glossae/
        community.stml       /* Community glosses */
```

### Parsing into Entities

```c
/* Load science from STML */
StmlResultus res = stml_legere_ex_via(via_scientia, piscina, intern);

si (res.successus)
{
    StmlNodus* radix = res.elementum_radix;
    chorda* id = stml_attributum_capere(radix, "id");

    /* Create science entity */
    Entitas* scientia = repo->entitas_scaffoldare(
        repo->datum, "Scientia", id->data);

    /* Process chapters */
    StmlNodus* curriculum = stml_invenire_liberum(radix, "curriculum");
    Xar* capitula = stml_invenire_omnes_liberos(curriculum, "caput", piscina);

    per (i32 i = 0; i < xar_numerus(capitula); i++)
    {
        StmlNodus* caput = xar_obtinere(capitula, i);
        _processare_caput(repo, scientia, caput, i);
    }

    /* Process skills */
    StmlNodus* artes = stml_invenire_liberum(radix, "artes");
    _processare_artes(repo, scientia, artes);

    /* Process cards */
    StmlNodus* chartae = stml_invenire_liberum(radix, "chartae");
    _processare_chartas(repo, scientia, chartae);
}
```

---

## Split View Integration

### Gloss Display (30/70 Split)

Following navigator_entitatum pattern exactly:

```c
/* From navigator_entitatum.c:2476-2482 */
latitudo_sinistra = (textus_latitudo * III) / X;  /* 30% for glosses */
latitudo_media = textus_latitudo - latitudo_sinistra;  /* 70% for content */
```

**Sync behavior** (per brainstorm):
- When paragraph changes in content pane, gloss pane updates
- When gloss selected, content highlights source paragraph
- Use `_reddere_columnam_entitatis()` pattern for dimmed context

### Memory Palace Mode

Full arx_caeli integration:

```c
/* Reuse arx_caeli patterns for spatial navigation */
nomen structura {
    ArcCaeli* arc_caeli;          /* Embed full arx_caeli */

    /* Science-specific overlays */
    Entitas* scientia;
    b32 modus_constructio;        /* Building mode vs. navigation */

    /* Link cards to science content */
    TabulaDispersa* carta_ad_ars; /* Card ID → Ars entity */
} PalatiumMemoriae;
```

---

## Implementation Phases

### Phase 1: Science Reader (V1 Milestone)

**Files to create:**

| File | Purpose | Est. Lines |
|------|---------|------------|
| `include/scientia_lector.h` | Core types | 150 |
| `lib/scientia_lector.c` | Reader implementation | 800 |
| `lib/scientia_lector_visus.c` | Display widget | 1200 |
| `probationes/probatio_scientia.c` | Unit tests | 400 |

**Total Phase 1**: ~2550 lines new C89 code

**Success criteria:**
- Load science from STML file
- Navigate chapters via TOC
- Read content with paragraph:sentence addressing
- Save/restore reading progress
- Basic gloss display (30/70 split)

### Phase 2: Skill Tree

**Dependencies**: Phase 1 complete

**Files:**

| File | Purpose | Est. Lines |
|------|---------|------------|
| `include/ars_visus.h` | Skill tree widget types | 100 |
| `lib/ars_visus.c` | DAG navigation widget | 1000 |
| `probationes/probatio_ars.c` | Tests | 300 |

**Total Phase 2**: ~1400 lines

### Phase 3: Card System

**Dependencies**: Phase 2 complete

**Files:**

| File | Purpose | Est. Lines |
|------|---------|------------|
| `include/charta.h` | Card types & tag system | 150 |
| `lib/charta.c` | Card mechanics engine | 600 |
| `lib/charta_visus.c` | Card display widget | 800 |
| `probationes/probatio_charta.c` | Tests | 400 |

**Total Phase 3**: ~1950 lines

### Phase 4: Quiz System

**Dependencies**: Phase 3 complete

**Files:**

| File | Purpose | Est. Lines |
|------|---------|------------|
| `include/probatio_scientia.h` | Quiz/test types | 100 |
| `lib/probatio_scientia.c` | Spaced repetition logic | 500 |
| `lib/probatio_visus.c` | Quiz widget | 600 |

**Total Phase 4**: ~1200 lines

### Phase 5: Memory Palace

**Dependencies**: Phase 2 complete, arx_caeli

**Files:**

| File | Purpose | Est. Lines |
|------|---------|------------|
| `include/palatium_memoriae.h` | Memory palace types | 100 |
| `lib/palatium_memoriae.c` | Spatial skill navigation | 800 |

**Total Phase 5**: ~900 lines

### Phase 6: Card Game Modes

**Dependencies**: Phase 3 complete

**New work:**
- "Discrete physics engine" for card interactions
- Multiple game mode implementations
- Deck building interface

**Total Phase 6**: ~2000 lines (estimated)

---

## Critical Path

```
                    ┌─────────────────────┐
                    │ Phase 1: Science    │
                    │ Reader              │
                    └──────────┬──────────┘
                               │
          ┌────────────────────┼────────────────────┐
          │                    │                    │
┌─────────▼─────────┐ ┌────────▼────────┐ ┌────────▼────────┐
│ Phase 2: Skill    │ │ Phase 3: Card   │ │ (Print blocked  │
│ Tree              │ │ System          │ │ by Scriptoria)  │
└─────────┬─────────┘ └────────┬────────┘ └─────────────────┘
          │                    │
          │                    │
┌─────────▼─────────┐ ┌────────▼────────┐
│ Phase 5: Memory   │ │ Phase 4: Quiz   │
│ Palace            │ │ System          │
└─────────┬─────────┘ └────────┬────────┘
          │                    │
          └────────────────────┼────────────────────┐
                               │                    │
                    ┌──────────▼──────────┐        │
                    │ Phase 6: Card       │◄───────┘
                    │ Game Modes          │
                    └─────────────────────┘
```

---

## Effort Estimates

| Phase | New Lines | Changed Lines | Dependencies |
|-------|-----------|---------------|--------------|
| 1 (Science Reader) | 2550 | 0 | None |
| 2 (Skill Tree) | 1400 | 0 | Phase 1 |
| 3 (Card System) | 1950 | 0 | Phase 2 |
| 4 (Quiz System) | 1200 | 0 | Phase 3 |
| 5 (Memory Palace) | 900 | 0 | Phase 2, arx_caeli |
| 6 (Card Games) | 2000 | 0 | Phase 3 |

**Total for full Scientia**: ~10,000 lines

**Note**: Print/PDF export requires Scriptoria sphere (separate effort)

---

## Reusable Patterns from Codebase

### From librarium_lector.c

- Byte offset addressing via `PaginariumLinea`
- Two-mode pagination (page vs sentence)
- Progress persistence via entity repository
- Lazy word-wrap evaluation
- Content caching pattern

### From navigator_entitatum.c

- DAG traversal with history stack (`Xar* via`)
- 30/70 split view rendering
- Item flattening for pagination
- j/k/l/h navigation bindings
- Backlink support (inverse relations)

### From arx_caeli.c

- Spatial card positioning
- Room-based navigation with history
- Drag and collision detection
- Z-ordering for layered display
- Click-to-interact patterns

### From stml.c

- DOM-style tree API
- Fragment/transclusion support
- Attribute and content extraction
- Schema-less structure definition

---

## Open Technical Questions

1. **Paragraph UUID generation**: Use UUIDv7 (time-ordered) or UUIDv5 (content-hash based)?

2. **Gloss sync granularity**: Sync on paragraph change or sentence change?

3. **Card tag hierarchy depth**: How deep should hierarchical tags go? (Wood > Hardwood > Oak)

4. **Quiz algorithm**: SM-2 (Anki) or simpler Leitner box system?

5. **Memory palace persistence**: Store room layouts per-user or shared?

6. **Federation merge strategy**: How to merge glosses from multiple internets?

---

## Success Criteria (V1)

1. ✓ Load science definition from STML
2. ✓ Navigate chapters via TOC
3. ✓ Read content with paragraph tracking
4. ✓ Display glosses in 30/70 split view
5. ✓ Save/restore reading progress
6. ✓ View skill tree for science
7. ✓ Navigate skill DAG with prerequisites
8. ✓ Data persists across restarts

---

## Appendix: Standard Science Categories

Per brainstorm, these are "ubiquitous language" not strict requirements:

| Category | Latin | Description |
|----------|-------|-------------|
| History | Historia | Development of the science |
| Principles | Principia | Core concepts and laws |
| Techniques | Artes | Methods and procedures |
| Tools | Instrumenta | Equipment and apparatus |
| Materials | Materiae | Substances worked with |
| Projects | Opera | Practical applications |
| Complications | Difficultates | Common pitfalls |
| Masters | Magistri | Notable practitioners |
| Philosophy | Philosophia | Theoretical foundations |
