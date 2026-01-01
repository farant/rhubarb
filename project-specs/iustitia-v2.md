# Iustitia V2 - Implementation-Specific Specification

Date: 2025-12-31
Status: Draft
Based on: iustitia.md, codebase exploration

---

## Executive Summary

This document grounds the Iustitia governance system in the existing rhubarb codebase. Based on exploration, we have strong foundations (entity system, event sourcing, SHA-256) but critical gaps (no actor tracking in events, no license keys, no Merkle chaining).

**Key Finding**: The event sourcing foundation is perfect for justice workflows, but requires augmentation with actor accountability before building Iustitia.

---

## Codebase Analysis Summary

### What Exists and Works

| Component | Status | Location |
|-----------|--------|----------|
| Entity system (Entitas) | Complete | `include/entitas.h`, `lib/entitas_repositorium_impl.c` |
| Event sourcing | Complete (no actor) | `include/persistentia.h`, `lib/persistentia_nuntium.c` |
| SHA-256 hashing | Complete | `include/friatio.h`, `lib/friatio.c` |
| UUIDv7 (time-ordered) | Complete | `include/uuid.h`, `lib/uuid.c` |
| Message bus (Cursus) | Complete | `include/cursus.h` |
| Widget system | Complete | `include/widget.h` |
| List widget patterns | Complete | `lib/librarium_visus.c` (2663 lines) |

### Critical Gaps

| Component | Status | Impact |
|-----------|--------|--------|
| Actor in events | Missing | Cannot track WHO made changes |
| License keys | Missing | No identity anchor |
| Merkle chaining | Missing | Events not tamper-evident |
| Asymmetric signing | Missing | Cannot verify authority |
| Salve (workflows) | Spec only | Rehabilitation tracking blocked |
| Internet ownership | Missing | No authority structure |

---

## Architecture Decision: Actor Tracking

**Problem**: Current `Eventum` structure has no actor_id or timestamp:

```c
/* Current (from persistentia.h) */
nomen structura {
    EventusGenus genus;
    chorda*      entitas_id;
    chorda*      entitas_genus;
    unio { ... } datum;
} Eventum;
```

**Solution**: Augment Eventum before building Iustitia:

```c
/* Proposed augmentation */
nomen structura {
    EventusGenus genus;
    chorda*      actor_id;           /* NEW: License key who made change */
    s64          tempus_creatus;     /* NEW: When change was made */
    chorda*      entitas_id;
    chorda*      entitas_genus;
    unio { ... } datum;
} Eventum;
```

**Rationale**: Iustitia requires accountability at the protocol level. Every action must be traceable to a license key. This is a breaking change but the project is young enough to absorb it.

---

## Entity Patterns

### Following Existing Conventions

Based on `entitas_repositorium_impl.c`, entities use:

```c
/* Create entity with deterministic ID */
Entitas* querela = repo->entitas_scaffoldare(
    repo->datum, "Querela", "complaint-2025-001");

/* Set properties */
repo->proprietas_ponere(repo->datum, querela,
    "submissor_clavis", "LAPIDEUS-XXXX-XXXX");
repo->proprietas_ponere(repo->datum, querela,
    "accusatus_clavis", "LAPIDEUS-YYYY-YYYY");
repo->proprietas_ponere(repo->datum, querela,
    "descriptio", "Refused delivery of paid goods");
repo->proprietas_ponere(repo->datum, querela,
    "status", "APERTA");

/* Add relationships */
repo->relatio_addere(repo->datum, querela,
    "submisit", submitter_entitas_id);
repo->relatio_addere(repo->datum, querela,
    "accusat", accused_entitas_id);

/* Add tags for fast queries */
repo->nota_addere(repo->datum, querela, "#querela");
repo->nota_addere(repo->datum, querela, "#status::aperta");
```

### Query Patterns

```c
/* Find all open complaints */
Xar* apertae = repo->quaerere_cum_praefixo_notae(
    repo->datum, "#status::aperta");

/* Find complaints against specific party */
Xar* contra_eum = repo->capere_entitates_relatae(
    repo->datum, accused_entity,
    chorda_internare_ex_literis(intern, "accusat"));

/* Pattern detection: count per accused */
/* Iterate results and group by accusatus_clavis property */
```

---

## Entity Definitions

### Querela (Complaint)

```c
genus = "Querela"

proprietates:
    submissor_clavis    /* License key of complainant */
    accusatus_clavis    /* License key of accused */
    descriptio          /* Free text description */
    status              /* APERTA | SUB_IUDITIO | RESOLUTA | REIECTA */
    data_creationis     /* Timestamp (s64) */

relationes:
    submisit → Persona entity (complainant)
    accusat → Persona entity (accused)
    refert → Causa entity (if escalated to case)

notae:
    #querela
    #status::aperta | #status::sub_iuditio | #status::resoluta | #status::reiecta
```

### Licentia (License Key)

```c
genus = "Licentia"

proprietates:
    clavis              /* LAPIDEUS-XXXX-XXXX-XXXX format */
    possessor_nomen     /* Real name (required) */
    typus               /* PERSONALIS | NEGOTIUM | POLITICUS */
    status              /* ACTIVA | REVOCATA | SUSPENSA */
    data_emissionis     /* When issued (s64) */

relationes:
    possessa_ab → Persona entity

notae:
    #licentia
    #status::activa | #status::revocata | #status::suspensa
```

### Magistratus (Authority)

```c
genus = "Magistratus"

proprietates:
    potestates          /* Bitmask: CAN_REVOKE | CAN_EJECT | CAN_EXPUNGE */
    data_concessionis   /* When authority granted */

relationes:
    habet_auctoritatem → Licentia entity (who has authority)
    super_interrete → Interrete entity (which internet)

notae:
    #magistratus
    #interrete::<internet_id>
```

---

## Widget Implementation

### QuereleVisus Structure

Based on `librarium_visus.c` patterns (~1200 lines estimated):

```c
nomen enumeratio {
    QUERELE_MODUS_LISTA,      /* List all complaints */
    QUERELE_MODUS_DETTAGLI,   /* Single complaint view */
    QUERELE_MODUS_FORMA       /* Submit new complaint */
} QuereleModus;

nomen enumeratio {
    QUERELE_CATEGORIA_OMNES,       /* All complaints */
    QUERELE_CATEGORIA_PER_ACCUSATUM, /* Grouped by accused */
    QUERELE_CATEGORIA_PER_STATUS   /* Grouped by status */
} QuerelaCategoria;

nomen structura {
    ContextusWidget* ctx;
    EntitasRepositorium* repo;

    /* Data */
    Xar* queriae;              /* All complaints */
    Xar* queriae_filtratae;    /* Filtered view */

    /* Navigation */
    QuereleModus modus;
    QuerelaCategoria categoria;
    chorda* filtrum_accusatus; /* Current group filter */
    s32 index_selecta;
    s32 index_paginae;
    s32 items_per_pagina;

    /* Form state */
    character formulae_accusatus[128];
    character formulae_descriptio[512];
    s32 formulae_cursor;
    b32 in_edit_modo;
} QuereleVisus;
```

### Key Bindings (following rhubarb conventions)

```
j/k     Navigate list up/down
Enter   Select / view details
Escape  Back to list
n       New complaint (enter form mode)
/       Filter by accused
1-3     Switch category (all/by-accused/by-status)
```

### Status Colors (from thema.h)

```c
/* Status indicator colors */
APERTA      → COLOR_WARNING (yellow)
SUB_IUDITIO → COLOR_STATUS_INSERT (blue)
RESOLUTA    → COLOR_SUCCESS (green)
REIECTA     → COLOR_ERROR (red)
```

---

## Implementation Phases

### Phase 0: Prerequisites (Before Iustitia)

**Must complete first:**

1. **Augment Eventum with actor tracking**
   - Add `actor_id` and `tempus_creatus` to Eventum struct
   - Update `persistentia_nuntium.c` serialization
   - Update all `entitas_repositorium` write methods to accept actor
   - ~200 lines changed across 3 files

2. **Create Licentia validation**
   - Format validation: `LAPIDEUS-XXXX-XXXX-XXXX`
   - Basic license entity creation
   - ~150 lines new code

### Phase 1: Complaint Log (V1 Milestone)

**Files to create:**

| File | Purpose | Est. Lines |
|------|---------|------------|
| `include/iustitia.h` | Core types: Querela, status enums | 100 |
| `lib/iustitia.c` | Complaint CRUD operations | 300 |
| `lib/querele_visus.c` | Widget for complaint log | 1200 |
| `probationes/probatio_iustitia.c` | Unit tests | 400 |

**Total Phase 1**: ~2000 lines new C89 code

**Success criteria:**
- Submit complaint (submitter, accused, description)
- View all complaints publicly
- Group by accused party
- Basic status transitions
- Family can use for household dispute logging

### Phase 2: Case Management

**Dependencies**: Phase 1 complete

**New structures:**
- Causa (case entity)
- Arbiter assignment
- Ruling recording
- Basic appeals

**Files:**
- Extend `iustitia.h/c` with Causa
- Create `causa_visus.c` widget
- ~1500 lines additional

### Phase 3: Revocation Blockchain

**Dependencies**: Phase 2 complete, Merkle library

**New libraries required:**

| File | Purpose | Est. Lines |
|------|---------|------------|
| `include/arboris_merkle.h` | Merkle tree types | 80 |
| `lib/arboris_merkle.c` | Merkle tree impl | 400 |
| `include/catena_revocationis.h` | Revocation chain types | 100 |
| `lib/catena_revocationis.c` | Revocation chain impl | 600 |

**Extend persistentia:**
- Add hash chaining to events
- `hash_prior` and `hash_proprium` fields
- ~150 lines changed

### Phase 4: AI Arbitration

**Dependencies**: Phase 3 complete

**Plugin architecture:**
```c
nomen structura {
    vacuum* datum;

    /* Evaluate a case, return ruling recommendation */
    IudiciumAI (*evaluare_causam)(vacuum* datum, Causa* causa);

    /* Screen complaint for patterns/priority */
    ScreeningResult (*vagliare_querelam)(vacuum* datum, Querela* q);

    /* Detect coordinated attack patterns */
    b32 (*detegere_impetum)(vacuum* datum, Xar* revocationes);
} ArbiterAIPlugin;
```

### Phase 5: Rehabilitation (Requires Salve)

**BLOCKED**: Salve is specification only, zero implementation.

**When Salve exists:**
- Probation tracking via state machines
- Appeal deadline monitoring via cron
- Penitence tracker via daily prompts
- Scientia course completion tracking

**Salve effort estimate**: 2000+ lines before Iustitia can use it

### Phase 6: Full Governance

- Expungement system
- Defederation management
- Aggregator integration
- HRE-style overlapping jurisdictions

---

## File Structure

```
include/
    iustitia.h              /* Core types */
    arboris_merkle.h        /* Merkle tree (Phase 3) */
    catena_revocationis.h   /* Revocation chain (Phase 3) */
    signum_ed25519.h        /* Signing (Phase 3+) */

lib/
    iustitia.c              /* Core implementation */
    querele_visus.c         /* Complaint log widget */
    causa_visus.c           /* Case management widget (Phase 2) */
    arboris_merkle.c        /* Merkle tree (Phase 3) */
    catena_revocationis.c   /* Revocation chain (Phase 3) */
    signum_ed25519.c        /* Ed25519 signing (Phase 3+) */

probationes/
    probatio_iustitia.c
    probatio_arboris_merkle.c
    probatio_catena_revocationis.c
```

---

## Critical Path

```
                    ┌─────────────────────┐
                    │ Augment Eventum     │
                    │ (actor tracking)    │
                    └──────────┬──────────┘
                               │
                    ┌──────────▼──────────┐
                    │ Phase 1: Complaint  │
                    │ Log                 │
                    └──────────┬──────────┘
                               │
          ┌────────────────────┼────────────────────┐
          │                    │                    │
┌─────────▼─────────┐ ┌────────▼────────┐ ┌────────▼────────┐
│ Phase 2: Cases    │ │ Build Merkle    │ │ Build Salve     │
│                   │ │ Library         │ │ (2000+ lines)   │
└─────────┬─────────┘ └────────┬────────┘ └────────┬────────┘
          │                    │                    │
          └────────────────────┼────────────────────┘
                               │
                    ┌──────────▼──────────┐
                    │ Phase 3: Revocation │
                    │ Blockchain          │
                    └──────────┬──────────┘
                               │
                    ┌──────────▼──────────┐
                    │ Phase 4: AI         │
                    │ Arbitration         │
                    └──────────┬──────────┘
                               │
                    ┌──────────▼──────────┐
                    │ Phase 5: Rehab      │
                    │ (needs Salve)       │
                    └──────────┬──────────┘
                               │
                    ┌──────────▼──────────┐
                    │ Phase 6: Full       │
                    │ Governance          │
                    └─────────────────────┘
```

---

## Effort Estimates

| Phase | New Lines | Changed Lines | Dependencies |
|-------|-----------|---------------|--------------|
| 0 (Prerequisites) | 150 | 200 | None |
| 1 (Complaint Log) | 2000 | 0 | Phase 0 |
| 2 (Cases) | 1500 | 100 | Phase 1 |
| 3 (Revocation) | 1200 | 150 | Merkle lib |
| 4 (AI Plugin) | 400 | 0 | Phase 3 |
| 5 (Rehab) | 800 | 0 | Salve (2000+) |
| 6 (Full Gov) | 1500 | 200 | All above |

**Total for full Iustitia**: ~7500 lines + Salve (~2000) = ~9500 lines

---

## Open Technical Questions

1. **Eventum augmentation**: Breaking change to file format. Need migration strategy or accept clean slate?

2. **License key generation**: Use UUIDv5 from holder name + secret? Or separate key generation system?

3. **Merkle tree granularity**: Per-event chaining vs daily batch Merkle roots?

4. **Ed25519 vs HMAC**: Full asymmetric signing or symmetric HMAC with shared secrets for MVP?

5. **Widget split**: One mega-widget (like LibrariumVisus at 2663 lines) or separate widgets for list/detail/form?

---

## Success Criteria (V1)

1. ✓ Submit complaint with minimal fields
2. ✓ View all complaints publicly
3. ✓ Group complaints by accused
4. ✓ Status transitions work
5. ✓ Data persists across restarts
6. ✓ Actor tracking in events works
7. ✓ Family can log household disputes
8. ✓ Pattern detection shows repeat offenders
