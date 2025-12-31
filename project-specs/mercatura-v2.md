# Mercatura V2 - Implementation Specification

Date: 2025-12-30
Status: Draft
Based on: mercatura.md, codebase exploration
Related: lapideus-v2.md, salve.md

---

## Codebase Analysis Summary

### What Exists (Strong Foundation)

| Component | Status | Location |
|-----------|--------|----------|
| Entity system | Complete | `entitas.h/c`, `entitas_repositorium.h/c` |
| Event sourcing | Complete | `persistentia.h/c` |
| Widget framework | Complete | `widget.h/c`, `registrum_widget.h/c` |
| Hierarchical navigator | Complete | `navigator_entitatum.h/c` |
| Message bus | Complete | `cursus.h/c` |
| STML parser | Complete | `stml.h/c` |
| Sputnik scripting | Complete | `sputnik_interpres.h/c` |
| String interning | Complete | `internamentum.h/c` |
| Serialization | Complete | `nuntium.h/c` |
| Fuzzy search | Complete | `quaerere.h/c` |

### What Needs Building

| Component | Priority | Complexity |
|-----------|----------|------------|
| Place/Location entities | V1 | Low |
| Address handling | V1 | Low |
| Business entity type | V1 | Low |
| Town hub widget | V1 | Medium |
| Category taxonomy | V1 | Medium |
| Money type handling | V2 | Low |
| Transaction entities | V2 | Medium |
| Double-entry ledger | V2 | High |
| Tax compliance | V3 | High |
| Salve integration | V3 | High |

---

## Entity Design (Following Existing Patterns)

### Pattern Reference

From `entitas.h` and `entitas_repositorium.h`:
- Use `entitas_scaffoldare()` for deterministic UUIDv5 IDs
- Properties stored as strings, parsed via typed accessors
- Use `typus_semanticus` for domain types (e.g., "Currency::USD")
- Use notae for status/categories with prefix conventions
- Relationships are first-class with their own UUIDv7 IDs

### Place Entity (Locus)

```c
/* Creation */
Entitas* locus = repo->entitas_scaffoldare(repo, "Locus", "concord-nh");

/* Proprietates */
repo->proprietas_ponere(repo, locus, "nomen", "Concord");
repo->proprietas_ponere(repo, locus, "status_civitatis", "NH");
repo->proprietas_ponere(repo, locus, "codex_postalis", "03301");
repo->proprietas_ponere(repo, locus, "latitudo", "43.2081");   /* TYPUS_F64 */
repo->proprietas_ponere(repo, locus, "longitudo", "-71.5376"); /* TYPUS_F64 */
repo->proprietas_ponere(repo, locus, "genus_loci", "civitas"); /* town/city/state */

/* Relationes - hierarchy */
repo->relatio_addere(repo, locus, "pars_de", nh_state_id);

/* Notae */
repo->nota_addere(repo, locus, "#locus");
repo->nota_addere(repo, locus, "#civitas");
repo->nota_addere(repo, locus, "#status::NH");
```

### State Entity (for US hierarchy)

```c
Entitas* status = repo->entitas_scaffoldare(repo, "Locus::Status", "nh");

repo->proprietas_ponere(repo, status, "nomen", "New Hampshire");
repo->proprietas_ponere(repo, status, "abbreviatio", "NH");
repo->proprietas_ponere(repo, status, "genus_loci", "status");

repo->nota_addere(repo, status, "#locus");
repo->nota_addere(repo, status, "#status");
```

### Business Entity (Negotium)

```c
Entitas* negotium = repo->entitas_scaffoldare(repo, "Negotium", "smiths-woodcraft");

/* Core identity */
repo->proprietas_ponere(repo, negotium, "nomen_legale", "Smith's Woodcraft LLC");
repo->proprietas_ponere(repo, negotium, "nomen_possessoris", "John Smith");

/* Address (structured) */
repo->proprietas_ponere(repo, negotium, "adlocutio_vicus", "123 Main Street");
repo->proprietas_ponere(repo, negotium, "adlocutio_civitas", "Concord");
repo->proprietas_ponere(repo, negotium, "adlocutio_status", "NH");
repo->proprietas_ponere(repo, negotium, "adlocutio_codex", "03301");

/* Contact */
repo->proprietas_ponere(repo, negotium, "telephonium", "603-555-1234");
repo->proprietas_ponere(repo, negotium, "inscriptio_electronica", "john@smithswoodcraft.com");

/* Hours (JSON-like or structured) */
repo->proprietas_ponere(repo, negotium, "horae", "Mon-Fri 9-5, Sat 10-2");

/* Relationes */
repo->relatio_addere(repo, negotium, "locatum_in", concord_id);
repo->relatio_addere(repo, negotium, "membrum_de", guild_id); /* optional */

/* Notae - categories use prefix convention */
repo->nota_addere(repo, negotium, "#negotium");
repo->nota_addere(repo, negotium, "#categoria::Ars::Lignaria"); /* Craft::Woodworking */
repo->nota_addere(repo, negotium, "#categoria::Fabricatio");
repo->nota_addere(repo, negotium, "#status::activum");
```

### Category Entity (Categoria)

For Yahoo-style browsable hierarchy:

```c
Entitas* cat = repo->entitas_scaffoldare(repo, "Categoria", "ars-lignaria");

repo->proprietas_ponere(repo, cat, "nomen", "Woodworking");
repo->proprietas_ponere(repo, cat, "nomen_latinum", "Ars Lignaria");
repo->proprietas_ponere(repo, cat, "descriptio", "Crafting with wood");
repo->proprietas_ponere(repo, cat, "ordo", "10"); /* sort order */

/* Hierarchy */
repo->relatio_addere(repo, cat, "pars_de", craft_category_id);

repo->nota_addere(repo, cat, "#categoria");
repo->nota_addere(repo, cat, "#radix"); /* if top-level */
```

---

## Widget Implementation

### Approach: Adapt NavigatorEntitatum

Based on exploration, `navigator_entitatum.c` (85K lines) provides the best foundation:
- Two-column hierarchical navigation
- Forward/back history stack
- Focus and selection handling
- Status persistence

### New Widget: DirectoriumMercatura

```c
/* include/directorium_mercatura.h */

typedef enum {
    MODUS_LOCI,       /* Browsing towns */
    MODUS_CATEGORIAE, /* Browsing categories */
    MODUS_NEGOTIORUM, /* Viewing business list */
    MODUS_SINGULARIS  /* Single business detail */
} ModusDirectorii;

typedef struct {
    /* Navigation state */
    ModusDirectorii modus;
    chorda* locus_selectus_id;      /* Current town */
    chorda* categoria_selecta_id;   /* Current category */
    chorda* negotium_selectum_id;   /* Selected business */

    /* History for back navigation */
    Xar* historia;  /* Stack of previous states */

    /* Cached lists */
    Xar* loci;      /* Towns in current view */
    Xar* categoriae; /* Categories in current view */
    Xar* negotia;   /* Businesses matching filters */

    /* Selection */
    integer index_selectus;
    integer offset_visibilis;

    /* Dependencies */
    EntitasRepositorium* repo;
    InternamentumChorda* intern;
    Piscina* piscina;
} DirectoriumMercatura;

/* Lifecycle */
DirectoriumMercatura* directorium_mercatura_creare(ContextusWidget* ctx);
vacuum directorium_mercatura_destruere(DirectoriumMercatura* dir);

/* Widget interface */
vacuum directorium_mercatura_reddere(
    DirectoriumMercatura* dir,
    integer x, integer y,
    integer latitudo, integer altitudo,
    b32 habet_focum
);

b32 directorium_mercatura_tractare_eventum(
    DirectoriumMercatura* dir,
    Eventum* eventum
);

/* Persistence */
vacuum directorium_mercatura_salvare_statum(DirectoriumMercatura* dir);
vacuum directorium_mercatura_restituere_statum(DirectoriumMercatura* dir);
```

### Navigation Flow

```
[States List]     →l→     [Towns in State]     →l→     [Categories]
     ↓h                         ↓h                        ↓h
  (back)                     (back)                    (back)
                                                          ↓l
                                                   [Businesses]
                                                          ↓l
                                                   [Business Detail]
```

### Key Bindings (following existing patterns)

| Key | Action |
|-----|--------|
| j/k | Move selection up/down |
| l | Enter selected item (drill down) |
| h | Go back in history |
| / | Start search |
| Enter | View details / take action |
| q | Exit widget |

### Rendering Layout

```
┌─────────────────────────────────────────────────────────────────┐
│ Concord, NH > Craft > Woodworking                    [12 items] │
├─────────────────────────────────────────────────────────────────┤
│                                                                  │
│   Smith's Woodcraft LLC                                         │
│   123 Main Street                                               │
│   603-555-1234                                                  │
│ ► Mon-Fri 9-5, Sat 10-2                        [Woodworking]   │
│                                                                  │
│   Johnson's Furniture                                           │
│   456 Oak Avenue                                                │
│   603-555-5678                                                  │
│   Mon-Sat 10-6                                 [Furniture]      │
│                                                                  │
│   ...                                                           │
│                                                                  │
├─────────────────────────────────────────────────────────────────┤
│ j/k:navigate  l:enter  h:back  /:search                        │
└─────────────────────────────────────────────────────────────────┘
```

---

## Data Population

### US Geographic Data

Need to bootstrap with US location data:

```c
/* locus_bootstrap.h */

/* Populate all 50 states + DC */
vacuum locus_bootstrap_status(EntitasRepositorium* repo, InternamentumChorda* intern);

/* Populate major cities/towns (start with ~500, expand to ~20,000) */
vacuum locus_bootstrap_civitates(EntitasRepositorium* repo, InternamentumChorda* intern);

/* Data source: Census Bureau place names + coordinates */
```

### Category Taxonomy

Start with high-level categories:

```
Categoria (Top Level)
├── Cibus (Food & Dining)
│   ├── Caupona (Restaurant)
│   ├── Pistrina (Bakery)
│   └── Taberna Potus (Cafe/Bar)
├── Servitia (Services)
│   ├── Faber Ferrarius (Plumber)
│   ├── Electricus (Electrician)
│   └── Hortulanus (Landscaping)
├── Mercatura (Retail)
│   ├── Vestimenta (Clothing)
│   ├── Ferramenta (Hardware)
│   └── Libri (Books)
├── Artes (Crafts)
│   ├── Lignaria (Woodworking)
│   ├── Textilia (Textiles)
│   └── Ceramica (Pottery)
└── Professiones (Professional)
    ├── Advocatus (Legal)
    ├── Medicus (Medical)
    └── Computatoria (Technology)
```

---

## Money Handling

### Design Decision: Integer Cents

Store all monetary values as `s64` representing smallest currency unit:

```c
/* In proprietas: */
repo->proprietas_ponere(repo, txn, "pretium", "3500"); /* $35.00 as cents */

/* Semantic type indicates currency */
/* Use typed accessor */
s64 pretium_centesimi;
entitas_proprietas_capere_s64(txn, "pretium", &pretium_centesimi);

/* For display: */
/* pretium_centesimi / 100 = dollars, pretium_centesimi % 100 = cents */
```

### Future: Money Type Helper

```c
/* pecunia.h - money handling utilities */

typedef struct {
    s64 quantitas;     /* Amount in smallest unit */
    chorda* currency;  /* "USD", "EUR", etc. */
    u8 decimales;      /* Decimal places (2 for USD) */
} Pecunia;

/* Parse from string: "35.00" → 3500 cents */
Pecunia pecunia_ex_chorda(chorda* valor, chorda* currency);

/* Format for display: 3500 → "$35.00" */
chorda* pecunia_ad_chordam(Pecunia* p, Piscina* pool);

/* Arithmetic (returns new Pecunia, no mutation) */
Pecunia pecunia_addere(Pecunia a, Pecunia b);
Pecunia pecunia_subtrahere(Pecunia a, Pecunia b);
```

---

## Files to Create

### Phase 1: Local Business Directory (First Milestone)

| File | Purpose | ~Lines |
|------|---------|--------|
| `include/locus.h` | Place entity types and queries | 150 |
| `lib/locus.c` | Place creation, hierarchy, queries | 400 |
| `include/locus_bootstrap.h` | US geographic data loading | 50 |
| `lib/locus_bootstrap.c` | State/city data population | 800 |
| `include/negotium.h` | Business entity types | 150 |
| `lib/negotium.c` | Business CRUD operations | 400 |
| `include/categoria.h` | Category taxonomy | 100 |
| `lib/categoria.c` | Category hierarchy and queries | 300 |
| `include/directorium_mercatura.h` | Directory widget header | 200 |
| `lib/directorium_mercatura.c` | Directory widget implementation | 1500 |
| `probationes/probatio_locus.c` | Place tests | 300 |
| `probationes/probatio_negotium.c` | Business tests | 300 |
| `probationes/probatio_directorium.c` | Widget tests | 400 |

**Estimated V1 total: ~5,000 lines**

### Phase 2: Want Ads and Transactions

| File | Purpose | ~Lines |
|------|---------|--------|
| `include/desiderium.h` | Want ad entity types | 150 |
| `lib/desiderium.c` | Want ad operations | 500 |
| `include/transactio.h` | Transaction entity types | 200 |
| `lib/transactio.c` | Transaction workflow | 600 |
| `include/pecunia.h` | Money handling utilities | 100 |
| `lib/pecunia.c` | Money parsing/formatting | 300 |

### Phase 3: ERP Features

| File | Purpose | ~Lines |
|------|---------|--------|
| `include/inventarium.h` | Inventory management | 200 |
| `lib/inventarium.c` | Stock tracking | 600 |
| `include/liber_rationum.h` | Accounting/ledger | 300 |
| `lib/liber_rationum.c` | Double-entry bookkeeping | 1000 |
| `include/tributum.h` | Tax compliance | 200 |
| `lib/tributum.c` | Tax calculations | 800 |

---

## Integration Points

### With Existing Systems

| System | Integration |
|--------|-------------|
| `entitas_repositorium` | All entities stored via repository interface |
| `persistentia` | Event sourcing for audit trail |
| `cursus` | Message bus for async operations |
| `schirmata` | Multi-screen support for different views |
| `registrum_widget` | Register DirectoriumMercatura as widget type |
| `quaerere` | Fuzzy search for businesses/categories |

### With Salve (Future)

When Salve is implemented:
- Business operations as state machines
- Order workflow: pending → accepted → paid → shipped → delivered
- Inventory alerts as scheduled tasks
- Tax deadlines as calendar reminders

### With Lapideus (Future)

When Lapideus networking is implemented:
- Town hubs sync between internets
- Business presence across multiple local internets
- Daily batch updates for directory changes

---

## Query Patterns

### Find all businesses in a town

```c
Xar* negotia = repo->capere_entitates_relatae(repo, locus_id, "locatum_in");
```

### Find businesses by category

```c
Xar* negotia = repo->quaerere_cum_nota(repo, "#categoria::Ars::Lignaria");
```

### Find businesses in town AND category

```c
/* First get by location */
Xar* in_loco = repo->capere_entitates_relatae(repo, locus_id, "locatum_in");

/* Then filter by category tag */
Xar* filtered = xar_creare(sizeof(Entitas*));
for (integer i = 0; i < xar_longitudo(in_loco); i++) {
    Entitas* n = *(Entitas**)xar_capere(in_loco, i);
    if (entitas_nota_cum_praefixo_habet(n, "#categoria::Ars::Lignaria")) {
        xar_addere(filtered, &n);
    }
}
```

### Search businesses by name

```c
Xar* results = repo->quaerere_textum(repo, "smith");
/* Filter to only Negotium genus */
```

---

## Success Criteria (V1)

1. Can bootstrap US state and sample town data
2. Can create Business entities with full address
3. Category taxonomy is browsable hierarchically
4. DirectoriumMercatura widget renders correctly
5. Navigation (j/k/h/l) works smoothly
6. Search finds businesses by name
7. Widget state persists across app restarts
8. Family can add test businesses and browse them

---

## Open Implementation Questions

1. **Location data source**: Where to get accurate US city/town data with coordinates?
2. **Category taxonomy**: Start minimal or comprehensive? Latin or English names?
3. **Hours format**: Structured (JSON) or free-form text?
4. **Map view**: Defer entirely or stub with placeholder?
5. **golden.earth bridge**: How does web export work technically?

---

## Dependencies on Other Specs

| Spec | Dependency |
|------|------------|
| Salve | Order workflows, business control planes |
| Lapideus | Network sync, identity/licensing |
| Scientia | Trade knowledge integration |
| Smaragda | Dispute resolution design |

Mercatura V1 (local business directory) can proceed independently.
