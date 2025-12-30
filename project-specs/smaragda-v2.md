# Smaragda v2 - Implementation Specification

Date: 2025-12-30
Status: Draft
Based on: smaragda.md, codebase exploration
Etymology: Greek σμάραγδος = "emerald" — something precious revealed through process

---

## Executive Summary

This document provides implementation-specific details for Smaragda, grounded in the existing rhubarb codebase. Smaragda is an AI-driven simulation sandbox that uses LLM actors to wargame systems and surface "complications" — emergent edge cases that couldn't be anticipated analytically.

---

## Codebase Integration Points

### Existing Infrastructure to Leverage

| Component | Location | Use in Smaragda |
|-----------|----------|-----------------|
| **STML Parser** | `stml.h/c` | Parse state machine definitions |
| **Entity Repository** | `entitas_repositorium.h/impl.c` | Store sessions, actors, complications |
| **Message Bus** | `cursus.h/c` | Actor-to-actor communication |
| **Timer System** | `tempestivum.h/c` | Auto-advance turns, timeouts |
| **Sputnik Interpreter** | `sputnik_interpres.h/c` | Guard expressions, conditions |
| **Event Sourcing** | `persistentia.h/nuntium.c` | Trace logging, replay |
| **Arena Memory** | `piscina.h/c` | Per-turn temporary allocations |
| **String Interning** | `internamentum.h/c` | Efficient string comparison |
| **Widget System** | `widget.h/c` | Simulation UI |

### New Files to Create

```
include/
├── smaragda.h              # Core types: Session, Turn, Complication
├── smaragda_actor.h        # Actor definition, persona, LLM prompting
├── smaragda_machina.h      # State machine parsing from STML
├── smaragda_trace.h        # Event trace logging and formatting
└── smaragda_llm.h          # LLM interface abstraction

lib/
├── smaragda.c              # Session lifecycle, turn execution
├── smaragda_actor.c        # Actor state, prompt generation
├── smaragda_machina.c      # STML → MachinaDef conversion
├── smaragda_trace.c        # Trace storage and querying
└── smaragda_llm.c          # LLM API calls, response parsing

probationes/
├── probatio_smaragda.c           # Core simulation tests
├── probatio_smaragda_machina.c   # State machine parsing tests
└── probatio_smaragda_actor.c     # Actor/prompt tests
```

---

## Data Structures

### Session Entity

```c
/*
 * Entitas genus: "SmaragdaSession"
 *
 * Stored as entity in repository with properties:
 */
nomen structura {
    chorda*     id;                    /* Entity ID (UUIDv7) */
    chorda*     titulus;               /* Human-readable name */

    /* Session state */
    SmaragdaModus   modus;             /* turn_based | accelerated */
    s32             turnus_currens;    /* Current turn number */
    s64             tempus_simulatum;  /* Simulated time (ms since epoch) */

    /* Cost tracking */
    f64             sumptus_usus;      /* USD spent so far */
    f64             sumptus_limes;     /* Budget limit */

    /* LLM tuning */
    SmaragdaBias    bias;              /* helpful | realistic | adversarial */

    /* Persistence pointers */
    Persistentia*   persistentia_trace; /* In-memory event log for this session */

} SmaragdaSession;

nomen enumeratio {
    SMARAGDA_MODUS_TURNUS     = I,     /* Turn-based, user advances */
    SMARAGDA_MODUS_ACCELERATUS = II    /* Auto-advance */
} SmaragdaModus;

nomen enumeratio {
    SMARAGDA_BIAS_AUXILIARIS   = I,    /* Helpful - things go smoothly */
    SMARAGDA_BIAS_REALISTICUS  = II,   /* Realistic - probabilistic */
    SMARAGDA_BIAS_ADVERSARIUS  = III   /* Adversarial - Murphy's law */
} SmaragdaBias;
```

**Entity Representation:**
```
Entitas: "SmaragdaSession::wool_business_v3"
├─ genus: "SmaragdaSession"
├─ proprietates:
│   ├─ titulus: "Wool Business Exploration"
│   ├─ modus: "turnus"
│   ├─ turnus_currens: "47"
│   ├─ tempus_simulatum: "1750000000000"
│   ├─ sumptus_usus: "2.34"
│   ├─ sumptus_limes: "10.00"
│   └─ bias: "adversarius"
├─ relationes:
│   ├─ "habet_actorem" → [SmaragdaActor entities]
│   ├─ "habet_machinam" → [SmaragdaMachina entities]
│   ├─ "habet_complicationem" → [SmaragdaComplicatio entities]
│   └─ "bifurcatum_ex" → SmaragdaSnapshot (optional)
└─ notae: ["#simulatio", "#activa"]
```

### Actor Entity

```c
/*
 * Entitas genus: "SmaragdaActor"
 *
 * Actors are entities that the LLM role-plays as during simulation.
 */
nomen structura {
    chorda*     id;
    chorda*     munus;                 /* Role: "customer", "employee", etc. */
    chorda*     persona;               /* Persona description for LLM */

    SmaragdaScopus  scopus_informationis;  /* What actor can see */
    chorda*         quaestio_visibilitatis; /* Entity query for visibility */

    chorda*     model_llm;             /* "haiku" | "sonnet" | "opus" */

} SmaragdaActor;

nomen enumeratio {
    SMARAGDA_SCOPUS_REALISTICUS = I,   /* Only what actor would know */
    SMARAGDA_SCOPUS_DEI         = II,  /* God view - sees everything */
    SMARAGDA_SCOPUS_PROPRIUS    = III  /* Custom query-based visibility */
} SmaragdaScopus;
```

**Entity Representation:**
```
Entitas: "SmaragdaActor::customer_impatient_01"
├─ genus: "SmaragdaActor"
├─ proprietates:
│   ├─ munus: "customer"
│   ├─ persona: "Impatient, price-sensitive, frequent complainer"
│   ├─ scopus_informationis: "realisticus"
│   └─ model_llm: "haiku"
├─ relationes:
│   ├─ "in_sessione" → SmaragdaSession
│   ├─ "sequitur_machinam" → SmaragdaMachina (optional)
│   └─ "habet_interactiones" → [SmaragdaEventum entities]
└─ notae: ["#actor", "#persona::impatiens"]
```

### Complication Entity

```c
/*
 * Entitas genus: "SmaragdaComplicatio"
 *
 * A discovered edge case or emergent problem.
 */
nomen structura {
    chorda*     id;
    chorda*     summarium;             /* Brief description */
    chorda*     narratio;              /* LLM-generated explanation */

    s32         turnus_inventus;       /* Turn when discovered */
    chorda*     categoria;             /* missing_info, blocked, frustrated, etc. */

    b32         salvatus_ut_test;      /* Promoted to regression test? */

} SmaragdaComplicatio;
```

**Entity Representation:**
```
Entitas: "SmaragdaComplicatio::resource_exhausted_001"
├─ genus: "SmaragdaComplicatio"
├─ proprietates:
│   ├─ summarium: "Employee needed access code but none sent"
│   ├─ narratio: "[LLM explanation of what went wrong...]"
│   ├─ turnus_inventus: "23"
│   ├─ categoria: "informatio_deest"
│   └─ salvatus_ut_test: "verum"
├─ relationes:
│   ├─ "inventum_in" → SmaragdaSession
│   ├─ "actores_implicati" → [SmaragdaActor entities]
│   └─ "vestigium_eventorum" → [SmaragdaEventum entities in sequence]
└─ notae: ["#complicatio", "#categoria::informatio_deest"]
```

### Trace Event Entity

```c
/*
 * Entitas genus: "SmaragdaEventum"
 *
 * A single event in the simulation trace.
 */
nomen structura {
    chorda*     id;
    s32         turnus;
    s64         tempus_simulatum;

    chorda*     actor_id;              /* Which actor acted */
    chorda*     actio_genus;           /* Type of action */
    chorda*     cogitatio;             /* "Show your work" reasoning */
    chorda*     exitus;                /* Outcome/result */

    b32         complicatio_inventa;   /* Did this surface a complication? */

} SmaragdaEventum;
```

---

## State Machine Integration

### Parsing State Machines from STML

Leverage the existing STML parser. State machines are defined in STML format:

```xml
<machina_status nomen="support_ticket">
    <status nomen="aperta" initialis="verum">
        <cum_eventu nomen="assignare">
            <transitio ad="assignata"/>
        </cum_eventu>
    </status>

    <status nomen="assignata">
        <cum_eventu nomen="resolvere">
            <transitio ad="resoluta"/>
        </cum_eventu>
        <cum_eventu nomen="escalare">
            <transitio ad="escalata"/>
        </cum_eventu>
    </status>

    <status nomen="resoluta" finalis="verum"/>
    <status nomen="escalata"/>
</machina_status>
```

### MachinaDef Structure

```c
/*
 * Parsed state machine definition (in-memory, not persisted as entity)
 */
nomen structura {
    chorda*         nomen;
    StatusDef*      status_initialis;
    Xar*            status_omnes;      /* Xar<StatusDef*> */
    TabulaDispersa* status_per_nomen;  /* nomen → StatusDef* */
} MachinaDef;

nomen structura {
    chorda*     nomen;
    b32         initialis;
    b32         finalis;
    Xar*        eventus_handlers;      /* Xar<EventusHandler*> */
    Xar*        actiones_in_introitu;  /* Xar<Actio*> */
    Xar*        actiones_in_exitu;     /* Xar<Actio*> */
} StatusDef;

nomen structura {
    chorda*     nomen_eventus;
    chorda*     conditio;              /* Sputnik expression (optional) */
    chorda*     status_destinatio;
    Xar*        actiones;              /* Xar<Actio*> */
} EventusHandler;
```

### Parsing Function

```c
/*
 * Parse STML node into MachinaDef.
 * Uses existing stml_legere() then walks the tree.
 */
MachinaDef* smaragda_machina_legere(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern
);

/*
 * Validate machine definition:
 * - Has initial state
 * - All transition targets exist
 * - No orphan states (unless explicitly final)
 */
SmaragdaStatus smaragda_machina_validare(MachinaDef* machina);
```

---

## Simulation Engine

### Session Lifecycle

```c
/*
 * Create new simulation session.
 * Creates SmaragdaSession entity in repository.
 * Creates in-memory persistentia for trace events.
 */
SmaragdaSession* smaragda_sessio_creare(
    EntitasRepositorium* repo,
    Piscina*             piscina,
    InternamentumChorda* intern,
    chorda*              titulus
);

/*
 * Load existing session from repository.
 * Reconstructs trace from stored events.
 */
SmaragdaSession* smaragda_sessio_aperire(
    EntitasRepositorium* repo,
    Piscina*             piscina,
    InternamentumChorda* intern,
    chorda*              sessio_id
);

/*
 * Add actor to session.
 * Creates SmaragdaActor entity linked to session.
 */
SmaragdaActor* smaragda_actor_addere(
    SmaragdaSession*     sessio,
    chorda*              munus,
    chorda*              persona,
    SmaragdaScopus       scopus,
    chorda*              model_llm
);

/*
 * Add state machine to session.
 * Parses STML and links to session.
 */
SmaragdaMachina* smaragda_machina_addere(
    SmaragdaSession*     sessio,
    chorda*              stml_contentus
);
```

### Turn Execution

```c
/*
 * Execute one turn of simulation.
 *
 * 1. Determine which actors can act this turn
 * 2. For each actor:
 *    a. Build prompt with context (state machine state, visible entities)
 *    b. Call LLM for "show your work" reasoning
 *    c. Parse LLM response for action/input
 *    d. Execute action against state machine
 *    e. Log trace event
 *    f. Check for complications
 * 3. Advance simulated time
 * 4. Increment turn counter
 */
SmaragdaTurnusResultatus smaragda_turnum_exsequi(
    SmaragdaSession*     sessio,
    SmaragdaLLMInterface* llm,
    Piscina*             piscina_temporaria
);

nomen structura {
    s32                 turnus;
    Xar*                eventus;       /* Xar<SmaragdaEventum*> */
    Xar*                complicationes; /* Xar<SmaragdaComplicatio*> newly found */
    f64                 sumptus_turni; /* LLM cost this turn */
    b32                 terminatus;    /* Did simulation reach end state? */
} SmaragdaTurnusResultatus;
```

### LLM Interface

```c
/*
 * Abstract LLM interface for actor simulation.
 * Allows swapping backends (Anthropic, OpenAI, local, mock).
 */
nomen structura {
    void* datum;

    /* Generate actor response given prompt */
    SmaragdaLLMResponse (*generare)(
        void*       datum,
        chorda*     systema_prompt,
        chorda*     user_prompt,
        chorda*     model,
        Piscina*    piscina
    );

} SmaragdaLLMInterface;

nomen structura {
    chorda*     cogitatio;     /* "Show your work" reasoning */
    chorda*     actio;         /* Parsed action/input */
    f64         sumptus;       /* Cost in USD */
    s32         tokens_input;
    s32         tokens_output;
    b32         successus;
    chorda*     error;
} SmaragdaLLMResponse;
```

### Prompt Generation

```c
/*
 * Build prompt for actor given current simulation state.
 */
chorda* smaragda_prompt_aedificare(
    SmaragdaSession*     sessio,
    SmaragdaActor*       actor,
    MachinaDef*          machina,
    chorda*              status_currens,
    Piscina*             piscina,
    InternamentumChorda* intern
);
```

**Prompt Template:**
```
You are role-playing as [MUNUS] in a simulation.

Your persona: [PERSONA]

Current state machine: [MACHINA_NOMEN]
Current state: [STATUS_CURRENS]
Available transitions: [TRANSITIONES]

What you can see:
[VISIBLE_ENTITIES_FROM_SCOPUS_QUERY]

Recent events:
[LAST_N_TRACE_EVENTS]

---

Show your work: Think through step-by-step how you would respond in this situation.
Note any information you need but don't have.
Then state your action clearly.

Format your response as:
THINKING: [your reasoning]
ACTION: [the action you take, matching one of the available transitions]
```

---

## Complication Detection

### Heuristics

```c
/*
 * Analyze actor reasoning for complications.
 */
SmaragdaComplicatio* smaragda_complicationem_detegere(
    SmaragdaEventum*     eventum,
    SmaragdaSession*     sessio,
    Piscina*             piscina
);
```

**Detection Patterns:**

| Pattern | Detected How |
|---------|--------------|
| Missing information | LLM mentions needing something it doesn't have |
| Blocked workflow | State machine reaches state with no valid transitions |
| Actor frustration | LLM expresses frustration in persona |
| Resource exhaustion | Property values indicate depletion |
| Timeout/delay | Simulated time exceeds expected bounds |
| Invalid action | LLM tries action not in transition list |

### Complication Storage

Complications are entities linked to the session:

```c
SmaragdaComplicatio* smaragda_complicationem_salvare(
    SmaragdaSession*     sessio,
    chorda*              summarium,
    chorda*              narratio,
    chorda*              categoria,
    Xar*                 actores_implicati,
    Xar*                 vestigium_eventorum
);
```

---

## Graph Arena (Sandbox Isolation)

### In-Memory Fork

For simulations that need graph context but shouldn't affect real data:

```c
/*
 * Create isolated graph arena for simulation.
 * Uses in-memory persistentia, optionally seeded from snapshot.
 */
EntitasRepositorium* smaragda_arena_creare(
    Piscina*             piscina,
    InternamentumChorda* intern,
    EntitasRepositorium* repo_fons,      /* Source to copy from (optional) */
    chorda*              quaestio_fons   /* What to copy: "#project::x" etc */
);
```

**Implementation:**
1. Create `persistentia_memoria_creare()` for the arena
2. Create new `EntitasRepositorium` backed by that persistentia
3. If `repo_fons` provided:
   - Query entities matching `quaestio_fons`
   - Replay their creation events into arena
   - Deep copy relationships

### Read-Only Access to Real Graph

```c
/*
 * Query real graph without mutation capability.
 * Used when simulation needs context but shouldn't write.
 */
Xar* smaragda_quaerere_legere_solum(
    SmaragdaSession*     sessio,
    EntitasRepositorium* repo_verus,
    chorda*              quaestio
);
```

---

## Trace Storage

### Using Event Sourcing

Each simulation session has its own in-memory event log:

```c
/*
 * Simulation trace is stored as events in session's persistentia.
 * This allows full replay and querying.
 */

/* Event types for simulation (extend EventusGenus) */
nomen enumeratio {
    SMARAGDA_EVENTUS_TURNUS_INCIPIT   = 100,
    SMARAGDA_EVENTUS_ACTOR_COGITAT    = 101,
    SMARAGDA_EVENTUS_ACTOR_AGIT       = 102,
    SMARAGDA_EVENTUS_TRANSITIO        = 103,
    SMARAGDA_EVENTUS_COMPLICATIO      = 104,
    SMARAGDA_EVENTUS_TURNUS_FINIT     = 105
} SmaragdaEventusGenus;
```

### Trace Formatting

```c
/*
 * Format trace for display.
 */
chorda* smaragda_trace_formare(
    SmaragdaSession*     sessio,
    s32                  turnus_ab,
    s32                  turnus_ad,
    Piscina*             piscina
);
```

**Output Format:**
```
Turn 23 | 2025-06-15 14:30
├─ [customer::impatient_01] Tried to check order status
│   └─ THINKING: "I'm going to the website... looking for my order..."
│   └─ ⚠️ COMPLICATION: No order confirmation was ever sent
│   └─ ACTION: call_support
├─ [employee::warehouse_01] Processing shipment
│   └─ THINKING: "Picking order #4521 from shelf B3..."
│   └─ ACTION: mark_shipped
└─ State transition: order_status → shipped
```

---

## UI Integration

### Widget for Simulation

```c
/*
 * Smaragda widget for viewing/controlling simulations.
 * Registered in RegistrumWidget.
 */
b32 smaragda_widget_factory(void* dom, void* nodus);
void smaragda_widget_init(ContextusWidget* ctx);
```

**Widget Modes:**

| Mode | Description |
|------|-------------|
| Session list | Show all sessions, create new |
| Turn-by-turn | Manual advancement, full trace visible |
| Accelerated | Auto-run with complications highlighted |
| Complication review | Browse/filter found complications |
| Actor editor | Define/edit actor personas |

### Command Registration

```c
/*
 * Commands for simulation control.
 */
registrum_commandi_registrare(reg, "smaragda:nova", smaragda_cmd_nova);
registrum_commandi_registrare(reg, "smaragda:aperire", smaragda_cmd_aperire);
registrum_commandi_registrare(reg, "smaragda:turnus", smaragda_cmd_turnus);
registrum_commandi_registrare(reg, "smaragda:currere", smaragda_cmd_currere);
registrum_commandi_registrare(reg, "smaragda:bifurcare", smaragda_cmd_bifurcare);
registrum_commandi_registrare(reg, "smaragda:salvare_test", smaragda_cmd_salvare_test);
```

---

## Integration with Salve

### Pre-Deploy Testing

When Salve is implemented, Smaragda can test Salve state machines:

```c
/*
 * Import Salve state machine for simulation.
 * Same STML format, just different context.
 */
SmaragdaMachina* smaragda_machina_ex_salve(
    SmaragdaSession*     sessio,
    chorda*              salve_machina_id,
    EntitasRepositorium* repo
);
```

### Workflow:
1. Author Salve state machine in STML
2. Import into Smaragda session
3. Define test actors (employee personas, customer types)
4. Run simulation turns
5. Review complications
6. Iterate on state machine
7. Deploy to Salve when satisfied

---

## Implementation Phases

### Phase 1: Core Engine (V1 Milestone)

**Files:**
- `include/smaragda.h` - Session, Actor, Complication types
- `lib/smaragda.c` - Session lifecycle, basic turn execution
- `include/smaragda_machina.h` - State machine types
- `lib/smaragda_machina.c` - STML parsing for state machines
- `probationes/probatio_smaragda.c` - Core tests

**Scope:**
- Create/load sessions
- Define actors with personas
- Parse state machines from STML
- Execute turns (mock LLM responses for testing)
- Log trace events
- Detect basic complications (blocked state, invalid action)

**Lines estimate:** ~1500-2000 lines

### Phase 2: LLM Integration

**Files:**
- `include/smaragda_llm.h` - LLM interface
- `lib/smaragda_llm.c` - Anthropic API integration
- `lib/smaragda_prompt.c` - Prompt building

**Scope:**
- Real LLM calls via HTTP
- Prompt template system
- Response parsing (THINKING/ACTION extraction)
- Cost tracking
- Model selection (haiku/sonnet/opus)

**Lines estimate:** ~800-1000 lines

### Phase 3: Trace & Visualization

**Files:**
- `include/smaragda_trace.h` - Trace types
- `lib/smaragda_trace.c` - Trace storage/formatting
- Widget integration in existing schirmata

**Scope:**
- Full trace storage as events
- Timeline view formatting
- Complication highlighting
- Basic widget for session management

**Lines estimate:** ~600-800 lines

### Phase 4: Advanced Features

**Scope:**
- Graph arena (sandbox isolation)
- Session branching/forking
- Parameter variation (LLM-suggested)
- Concurrent sessions
- Regression test promotion

---

## Dependencies

### Required (Existing)
- `stml.h/c` - STML parsing
- `entitas_repositorium.h/impl.c` - Entity storage
- `persistentia.h`, `persistentia_memoria.c` - Event logging
- `piscina.h/c` - Memory management
- `internamentum.h/c` - String interning
- `chorda.h/c` - String handling
- `xar.h/c` - Dynamic arrays
- `tabula_dispersa.h/c` - Hash tables

### Required (To Create)
- HTTP client for LLM API calls (or shell out to curl)
- JSON parsing for LLM responses

### Optional
- `sputnik_interpres.h/c` - For guard expressions in state machines
- `tempestivum.h/c` - For auto-advance timing in accelerated mode

---

## Success Criteria (V1)

1. ✓ Create simulation session as entity
2. ✓ Define actors with personas
3. ✓ Parse state machine from STML
4. ✓ Execute turns with mock LLM responses
5. ✓ Log full trace as events
6. ✓ Detect blocked-state complications
7. ✓ Persist session across app restarts
8. ✓ Display trace in terminal format

---

## Open Questions

1. **HTTP/LLM calls**: Build HTTP client in C89, or shell out to curl/wget?
2. **JSON parsing**: Build minimal JSON parser, or use structured text format for LLM responses?
3. **Widget integration**: New schirma or integrate into existing?
4. **Test coverage**: How to test LLM-dependent code? Mock interface essential.
5. **Cost limits**: Per-turn, per-session, or global budget enforcement?
