# Salve - Control Plane Implementation Specification (V2)

Date: 2025-12-29
Status: Draft
Related: salve.md, systema-paradigmata.md, unified-state-paradigm.md
Etymology: Latin "salve" = "be well" / greeting of wellness

---

## Part 1: Current State Analysis

### 1.1 Timer Infrastructure

**tempus.h/tempus.c** - Frame-level timing
```c
// Provides frame clock, delta time calculation
// Used for animation, physics - NOT for cron/scheduling
tempus_nunc()        // Current frame time
tempus_delta()       // Time since last frame
```

**tempestivum.h/tempestivum.c** - Entity-based timers
```c
// Timers stored as entities with message bus integration
nomen structura Tempestivum {
    i64         exspiratio_ms;      // When timer fires
    chorda*     mittens_genus;      // Message type to send
    vacuum*     payload;            // Data to include
    b32         repetens;           // One-shot vs repeating
    i64         intervallum_ms;     // Repeat interval
} Tempestivum;

// Integration with cursus message bus
// Timer expiration sends message via cursus_mittere()
```

**Gap Analysis**:
- No cron expression parsing (e.g., "0 9 * * MON")
- No absolute time scheduling (only relative intervals)
- No calendar-aware scheduling (weekdays, holidays)
- Timers are relative to frame time, not wall clock

**What We Need**:
- Cron expression parser
- Wall clock time integration
- Persistent scheduled events that survive restart

### 1.2 STML Parser

**stml.h/stml.c** - Full STML parsing capability
```c
// Already supports:
// - Nested elements with arbitrary depth
// - Attributes on elements
// - Text content
// - CSS selectors via selectio.h

StmlElementum* stml_parsare(chorda* stml_fons);
StmlElementum* stml_quaerere(StmlElementum* radix, chorda* selector);
chorda* stml_attributum(StmlElementum* elem, chorda* nomen);
Xar* stml_liberi(StmlElementum* elem);
```

**Ready for**: State machine definitions can be parsed from STML directly.

### 1.3 Message Bus & Request-Response

**cursus.h** - Message bus
```c
// mittens_id field enables request-response patterns
nomen structura Nuntius {
    chorda*     genus;          // Message type
    chorda*     mittens_id;     // Sender ID for replies
    vacuum*     payload;        // Message data
    i64         timestamp;      // When sent
} Nuntius;

cursus_mittere(Cursus* c, Nuntius* n);
cursus_subscribere(Cursus* c, chorda* genus, FunctioNuntius handler);
```

**Actor System** (actor.h):
```c
// Actors have identity, can receive messages
// mittens_id allows "reply to sender" pattern
```

**Implication**: InboxTask can use mittens_id to route responses back to the originating workflow instance.

### 1.4 Entity Lifecycle

**entitas_repositorium.h/entitas_repositorium.c**:
```c
// Two creation patterns:
Entitas* entitas_creare(Repo* r);  // Random UUID
Entitas* entitas_scaffoldare(Repo* r, chorda* namespace, chorda* nomen);  // Deterministic UUIDv5

// Type linking via "est" relationship:
entitas_adnectere(e, "est", genus_entity);

// Event sourcing:
// All mutations logged as events
// Full replay capability for reconstruction
```

**For Salve**:
- Workflow instances use deterministic IDs: `scaffoldare("WorkflowInstance", "weekly_review_fran")`
- State machines linked via "est" to MachinaStatus genus
- All state transitions logged as events

### 1.5 Sputnik Scripting

**sputnik_interpres.h/sputnik_interpres.c**:
```c
// Tree-walking interpreter with REPO global
// Can evaluate expressions, call entity methods

// Available in scripts:
REPO.get("Entity::id")           // Fetch entity
entity.prop("property_name")      // Get property
entity.rel("relationship_type")   // Get related entities

// Useful for transition conditions:
// <guard>REPO.get(instance).prop("review_count") > 3</guard>
```

**For Salve**: Sputnik can evaluate transition guards and on_response conditions.

---

## Part 2: Target Architecture

### 2.1 Core Types

```c
/* include/salve.h */

/* ============================================
 * Cron Expression
 * ============================================ */
nomen structura CronExpressio {
    i8      minuta;         // 0-59, -1 = any
    i8      hora;           // 0-23, -1 = any
    i8      dies_mensis;    // 1-31, -1 = any
    i8      mensis;         // 1-12, -1 = any
    i8      dies_hebdomadis;// 0-6 (Sun=0), -1 = any
} CronExpressio;

/* ============================================
 * State Machine Definition (parsed from STML)
 * ============================================ */
nomen structura MachinaStatusDef {
    chorda*             nomen;
    chorda*             genus_instanti;     // Genus for instances
    Xar*                status_list;        // List of StatusDef*
    chorda*             status_initialis;
    TabulaDispersa*     status_map;         // name -> StatusDef*
} MachinaStatusDef;

nomen structura StatusDef {
    chorda*             nomen;
    b32                 initialis;
    CronExpressio*      horarium;           // Optional schedule
    Xar*                on_enter_actions;   // List of ActioDef*
    Xar*                on_exit_actions;
    Xar*                on_response_actions;
    Xar*                on_schedule_actions;
    Xar*                transitiones;       // List of TransitioDef*
    i64                 timeout_ms;         // Optional timeout
    Xar*                on_timeout_actions;
} StatusDef;

nomen structura TransitioDef {
    chorda*             ad_statum;          // Target state name
    chorda*             custos;             // Guard expression (Sputnik)
    chorda*             imperium;           // Command to generate
} TransitioDef;

nomen structura ActioDef {
    ActioGenus          genus;
    vacuum*             payload;            // Type-specific data
} ActioDef;

nomen enumeratio {
    ACTIO_TRANSITION        = 0,
    ACTIO_INBOX_TASK        = 1,
    ACTIO_SET_PROPERTY      = 2,
    ACTIO_SCHEDULE_DELAY    = 3,
    ACTIO_EXTERNAL_CALL     = 4
} ActioGenus;

/* ============================================
 * Inbox Task Definition
 * ============================================ */
nomen structura InboxTaskDef {
    chorda*             nuntius;            // Message to display
    InboxResponseGenus  response_genus;
    Xar*                optiones;           // For choice types
    i64                 timeout_ms;
    Xar*                on_timeout_actions;
} InboxTaskDef;

nomen enumeratio {
    INBOX_CONFIRMATION      = 0,    // Yes/No
    INBOX_CHOICE            = 1,    // Single select
    INBOX_MULTI_CHOICE      = 2,    // Multi select
    INBOX_TEXT              = 3,    // Free text
    INBOX_FORM              = 4,    // Multiple fields
    INBOX_ENTITY_SELECT     = 5,    // Entity picker
    INBOX_ACTION_REPORT     = 6     // Did you do X?
} InboxResponseGenus;

/* ============================================
 * Workflow Instance (Runtime State)
 * ============================================ */
nomen structura InstanceMachinae {
    chorda*             id;
    MachinaStatusDef*   definitio;
    chorda*             status_currens;
    i64                 ingressus_status_at;    // When entered current state
    TabulaDispersa*     proprietates;           // Instance properties
    chorda*             pendet_ab_task_id;      // Waiting for this task
    b32                 suspensa;
} InstanceMachinae;

/* ============================================
 * Inbox Task Instance
 * ============================================ */
nomen structura InboxTask {
    chorda*             id;
    chorda*             workflow_instance_id;   // Who created this
    chorda*             nuntius;
    InboxResponseGenus  response_genus;
    Xar*                optiones;
    InboxTaskStatus     status;
    i64                 creata_at;
    i64                 deadline;               // Optional
    TabulaDispersa*     response;               // User's response
} InboxTask;

nomen enumeratio {
    TASK_PENDING        = 0,
    TASK_ACCEPTED       = 1,
    TASK_IN_PROGRESS    = 2,
    TASK_COMPLETED      = 3,
    TASK_TIMED_OUT      = 4,
    TASK_IGNORED        = 5
} InboxTaskStatus;
```

### 2.2 Cron Parser API

```c
/* include/salve_cron.h */

// Parse cron expression string
// Returns NULL on parse error
CronExpressio* cron_parsare(chorda* expressio);

// Check if cron matches given timestamp
b32 cron_congruit(CronExpressio* cron, i64 timestamp_ms);

// Calculate next occurrence after given timestamp
i64 cron_proximum(CronExpressio* cron, i64 post_timestamp_ms);

// Format cron back to string (for debugging)
chorda* cron_forma(CronExpressio* cron, Piscina* p);
```

### 2.3 State Machine Parser API

```c
/* include/salve_stml.h */

// Parse state machine definition from STML
MachinaStatusDef* machina_parsare(StmlElementum* radix, Piscina* p);

// Parse individual components
StatusDef* status_parsare(StmlElementum* elem, Piscina* p);
TransitioDef* transitio_parsare(StmlElementum* elem, Piscina* p);
ActioDef* actio_parsare(StmlElementum* elem, Piscina* p);
InboxTaskDef* inbox_task_def_parsare(StmlElementum* elem, Piscina* p);

// Validation
b32 machina_validare(MachinaStatusDef* def, chorda** error_msg);
```

### 2.4 Workflow Engine API

```c
/* include/salve_engine.h */

// Engine manages all running instances
nomen structura SalveEngine {
    Repo*               repositorium;
    Cursus*             cursus;             // Message bus
    TabulaDispersa*     definitiones;       // name -> MachinaStatusDef*
    TabulaDispersa*     instantiae;         // id -> InstanceMachinae*
    Xar*                scheduled_events;   // Pending cron triggers
    SputnikInterpres*   interpres;          // For guard evaluation
} SalveEngine;

// Lifecycle
SalveEngine* salve_creare(Repo* r, Cursus* c);
vacuum salve_destruere(SalveEngine* e);

// Definition management
vacuum salve_definitionem_registrare(SalveEngine* e, MachinaStatusDef* def);
MachinaStatusDef* salve_definitionem_invenire(SalveEngine* e, chorda* nomen);

// Instance management
InstanceMachinae* salve_instantiam_creare(SalveEngine* e, chorda* def_nomen, chorda* owner_id);
InstanceMachinae* salve_instantiam_invenire(SalveEngine* e, chorda* instance_id);
vacuum salve_instantiam_destruere(SalveEngine* e, chorda* instance_id);

// State transitions
b32 salve_transire(SalveEngine* e, chorda* instance_id, chorda* ad_statum);

// Tick - called periodically (e.g., every minute)
vacuum salve_tick(SalveEngine* e, i64 nunc_ms);

// Task response handling
vacuum salve_task_respondere(SalveEngine* e, chorda* task_id, TabulaDispersa* response);
```

### 2.5 Message Types

```c
/* Messages sent via cursus */

// State machine triggers
#define SALVE_MSG_SCHEDULE_TRIGGER  "salve:schedule_trigger"
#define SALVE_MSG_TASK_RESPONSE     "salve:task_response"
#define SALVE_MSG_TRANSITION        "salve:transition"
#define SALVE_MSG_TIMEOUT           "salve:timeout"

// Inbox task lifecycle
#define SALVE_MSG_TASK_CREATED      "salve:task_created"
#define SALVE_MSG_TASK_ACCEPTED     "salve:task_accepted"
#define SALVE_MSG_TASK_COMPLETED    "salve:task_completed"

// Events for observability
#define SALVE_EVENT_STATE_ENTERED   "salve:state_entered"
#define SALVE_EVENT_STATE_EXITED    "salve:state_exited"
#define SALVE_EVENT_ACTION_EXECUTED "salve:action_executed"
```

---

## Part 3: Entity Mapping

### 3.1 State Machine Definition Entity

```
Entitas: "MachinaStatus::weekly_review"
├─ genus: "MachinaStatus"
├─ proprietates:
│   ├─ nomen: "weekly_review"
│   ├─ status_initialis: "idle"
│   └─ stml_fons: "<state_machine>...</state_machine>"
├─ relationes:
│   ├─ "habet_statum" → ["Status::idle", "Status::prompting"]
│   └─ "creata_ab" → Person entity
└─ notae: ["#workflow_definition", "#personal"]
```

### 3.2 State Definition Entity

```
Entitas: "Status::weekly_review::idle"
├─ genus: "Status"
├─ proprietates:
│   ├─ nomen: "idle"
│   ├─ initialis: "true"
│   └─ cron_expressio: "0 9 * * MON"
├─ relationes:
│   └─ "in_machina" → "MachinaStatus::weekly_review"
└─ notae: ["#state"]
```

### 3.3 Workflow Instance Entity

```
Entitas: "InstanceMachinae::weekly_review_fran_001"
├─ genus: "InstanceMachinae"
├─ proprietates:
│   ├─ status_currens: "prompting"
│   ├─ ingressus_status_at: "1703836800000"
│   ├─ suspensa: "true"
│   ├─ notes: ""
│   └─ last_review: "1703232000000"
├─ relationes:
│   ├─ "definitio" → "MachinaStatus::weekly_review"
│   ├─ "dominus" → Person entity
│   └─ "pendet_ab" → "InboxTask::abc123"
└─ notae: ["#workflow_instance", "#active"]
```

### 3.4 Inbox Task Entity

```
Entitas: "InboxTask::abc123"
├─ genus: "InboxTask"
├─ proprietates:
│   ├─ nuntius: "Weekly review time! What did you accomplish?"
│   ├─ response_genus: "text"
│   ├─ status: "pending"
│   ├─ creata_at: "1703836800000"
│   └─ deadline: "1704096000000"
├─ relationes:
│   ├─ "ab_workflow" → "InstanceMachinae::weekly_review_fran_001"
│   └─ "assignata_ad" → Person entity
└─ notae: ["#inbox", "#pending"]
```

---

## Part 4: STML State Machine Syntax

### 4.1 Complete Grammar

```xml
<state_machine name="NAME" genus="OPTIONAL_GENUS">

    <state name="STATE_NAME" initial="true|false">

        <!-- Optional: Cron schedule for this state -->
        <schedule cron="CRON_EXPRESSION"/>

        <!-- Optional: Delay before auto-transition -->
        <schedule delay="DURATION"/>

        <!-- Actions on entering state -->
        <on_enter>
            <transition to="STATE_NAME"/>
            <set property="PROP_NAME" value="VALUE"/>
            <task type="TASK_TYPE">
                <message>TEXT with {{variables}}</message>
                <option value="VAL">Label</option>
                <timeout duration="DURATION">
                    <!-- Nested actions -->
                </timeout>
            </task>
        </on_enter>

        <!-- Actions on receiving task response -->
        <on_response>
            <set property="PROP_NAME" value="{{response.field}}"/>
            <transition to="STATE_NAME"/>
        </on_response>

        <!-- Conditional response handling -->
        <on_response condition="SPUTNIK_EXPRESSION">
            <!-- Actions if condition true -->
        </on_response>

        <!-- Actions when schedule triggers -->
        <on_schedule>
            <transition to="STATE_NAME"/>
        </on_schedule>

        <!-- Actions on exiting state -->
        <on_exit>
            <!-- Actions -->
        </on_exit>

        <!-- Timeout handling -->
        <timeout duration="DURATION">
            <task type="confirmation">
                <message>Reminder message</message>
            </task>
        </timeout>

    </state>

</state_machine>
```

### 4.2 Task Types

```xml
<!-- Confirmation (Yes/No) -->
<task type="confirmation">
    <message>Do you approve this?</message>
</task>

<!-- Single choice -->
<task type="choice">
    <message>Select priority:</message>
    <option value="high">High Priority</option>
    <option value="medium">Medium Priority</option>
    <option value="low">Low Priority</option>
</task>

<!-- Multiple choice -->
<task type="multi_choice">
    <message>Select tags:</message>
    <option value="urgent">Urgent</option>
    <option value="important">Important</option>
    <option value="blocking">Blocking</option>
</task>

<!-- Text input -->
<task type="text">
    <message>What did you accomplish?</message>
</task>

<!-- Form with multiple fields -->
<task type="form">
    <message>Fill out review:</message>
    <field name="summary" type="text" required="true"/>
    <field name="rating" type="number" min="1" max="5"/>
    <field name="continue" type="boolean"/>
</task>

<!-- Entity selection -->
<task type="entity_select" genus="Person">
    <message>Who should own this project?</message>
</task>
```

### 4.3 Duration Format

```
1h      = 1 hour
30m     = 30 minutes
2d      = 2 days
1w      = 1 week
```

### 4.4 Variable Interpolation

```
{{timestamp}}           - Current timestamp (ms)
{{date}}                - Current date (ISO format)
{{instance.prop}}       - Instance property
{{response.text}}       - Task response text field
{{response.confirmed}}  - Task response confirmation
{{response.selected}}   - Task response selection(s)
```

---

## Part 5: Implementation Plan

### Phase 1: Cron Parser (Foundation)

**Files to create**:
| File | Purpose |
|------|---------|
| `include/salve_cron.h` | Cron expression type and API |
| `lib/salve_cron.c` | Cron parsing and matching |
| `tests/probatio_salve_cron.c` | Unit tests |

**Implementation**:
1. Parse cron expression format: `minute hour day_of_month month day_of_week`
2. Support wildcards (`*`), specific values, ranges (`1-5`), steps (`*/15`)
3. Match against given timestamp
4. Calculate next occurrence

**Test cases**:
- "0 9 * * MON" → Every Monday 9am
- "*/15 * * * *" → Every 15 minutes
- "0 0 1 * *" → First of every month

### Phase 2: State Machine Parser

**Files to create**:
| File | Purpose |
|------|---------|
| `include/salve_stml.h` | State machine parser API |
| `lib/salve_stml.c` | STML → MachinaStatusDef |
| `tests/probatio_salve_stml.c` | Parser tests |

**Implementation**:
1. Use existing stml.h to parse STML document
2. Extract state machine structure via selectors
3. Build MachinaStatusDef with all states, transitions, actions
4. Validate completeness (initial state exists, all transition targets valid)

### Phase 3: Workflow Engine Core

**Files to create**:
| File | Purpose |
|------|---------|
| `include/salve.h` | Core types |
| `include/salve_engine.h` | Engine API |
| `lib/salve_engine.c` | Engine implementation |
| `tests/probatio_salve_engine.c` | Engine tests |

**Implementation**:
1. Engine holds definitions and active instances
2. `salve_instantiam_creare` starts new workflow in initial state
3. `salve_transire` executes transition, runs on_exit → on_enter
4. `salve_tick` checks cron schedules, triggers timeouts

### Phase 4: Inbox Task Integration

**Files to create**:
| File | Purpose |
|------|---------|
| `include/inbox_task.h` | Task types |
| `lib/inbox_task.c` | Task creation, response handling |

**Implementation**:
1. Create InboxTask entity from InboxTaskDef
2. Task carries workflow instance ID for response routing
3. On response: validate against expected type, route to engine
4. Engine processes response, may trigger transition

### Phase 5: Entity Persistence

**Files to modify**:
| File | Change |
|------|--------|
| `lib/entitas_repositorium.c` | Add MachinaStatus, InstanceMachinae genus support |

**Implementation**:
1. Register new genera in taxonomy
2. Workflow instances persist as entities
3. On restart: load all InstanceMachinae entities, rebuild engine state
4. Scheduled events recalculated from current state + cron expressions

### Phase 6: Sputnik Integration

**Files to modify**:
| File | Change |
|------|--------|
| `lib/salve_engine.c` | Use Sputnik for guard evaluation |

**Implementation**:
1. Inject workflow instance properties into Sputnik context
2. Evaluate guard expressions before transitions
3. Evaluate on_response conditions
4. Handle expression errors gracefully

---

## Part 6: Scheduler Integration

### 6.1 Wall Clock Timer

The existing tempestivum system uses frame-relative timing. Salve needs wall clock time.

**Approach**: Create a periodic tick that:
1. Gets current wall clock time (`time(NULL)`)
2. Checks all active cron schedules
3. Fires messages for any that match

```c
// Called by main loop, e.g., every 60 seconds
vacuum salve_tick(SalveEngine* e, i64 nunc_ms) {
    // Check cron schedules
    per (i32 i = 0; i < xar_longitudo(e->scheduled_events); i++) {
        ScheduledEvent* ev = xar_accipere(e->scheduled_events, i);
        si (cron_congruit(ev->cron, nunc_ms)) {
            // Fire schedule trigger
            Nuntius n = {
                .genus = SALVE_MSG_SCHEDULE_TRIGGER,
                .payload = ev->instance_id
            };
            cursus_mittere(e->cursus, &n);
        }
    }

    // Check timeouts
    per (TabulaDispersaIter it = ...) {
        InstanceMachinae* inst = ...;
        StatusDef* status = machina_statum_invenire(inst->definitio, inst->status_currens);
        si (status->timeout_ms > 0) {
            i64 elapsed = nunc_ms - inst->ingressus_status_at;
            si (elapsed >= status->timeout_ms) {
                salve_timeout_tractare(e, inst);
            }
        }
    }
}
```

### 6.2 Persistence Across Restart

On startup:
1. Load all InstanceMachinae entities from repository
2. Reconstruct in-memory InstanceMachinae from entity properties
3. Rebuild scheduled_events list from current states
4. Calculate next cron occurrence for each

```c
vacuum salve_restaurare(SalveEngine* e) {
    // Query all workflow instances
    Xar* instances = repo_quaerere(e->repositorium, "#workflow_instance");

    per (i32 i = 0; i < xar_longitudo(instances); i++) {
        Entitas* ent = xar_accipere(instances, i);
        InstanceMachinae* inst = instance_ab_entitate(ent);
        tabula_dispersa_inserere(e->instantiae, inst->id, inst);

        // Rebuild schedule if current state has cron
        StatusDef* status = machina_statum_invenire(inst->definitio, inst->status_currens);
        si (status->horarium != NIHIL) {
            ScheduledEvent* ev = ...;
            ev->cron = status->horarium;
            ev->instance_id = inst->id;
            ev->next_fire = cron_proximum(ev->cron, tempus_nunc());
            xar_adiungere(e->scheduled_events, ev);
        }
    }
}
```

---

## Part 7: Example Workflow

### 7.1 Weekly Review State Machine

```xml
<state_machine name="weekly_review">

    <state name="idle" initial="true">
        <schedule cron="0 9 * * MON"/>
        <on_schedule>
            <transition to="prompting"/>
        </on_schedule>
    </state>

    <state name="prompting">
        <on_enter>
            <task type="text">
                <message>Weekly review time! What did you accomplish this week?</message>
                <timeout duration="3d">
                    <task type="confirmation">
                        <message>You haven't done your weekly review. Skip this week?</message>
                    </task>
                </timeout>
            </task>
        </on_enter>

        <on_response>
            <set property="notes" value="{{response.text}}"/>
            <set property="last_review" value="{{timestamp}}"/>
            <transition to="idle"/>
        </on_response>

        <on_response condition="{{response.confirmed}}">
            <set property="skipped_count" value="{{instance.skipped_count + 1}}"/>
            <transition to="idle"/>
        </on_response>
    </state>

</state_machine>
```

### 7.2 Execution Flow

```
Monday 9:00 AM
├── salve_tick() detects cron match
├── Sends SALVE_MSG_SCHEDULE_TRIGGER to instance
├── Engine receives, executes on_schedule actions
├── Transition: idle → prompting
│   ├── on_exit(idle) - none
│   ├── State changes to "prompting"
│   ├── on_enter(prompting):
│   │   └── Create InboxTask with text prompt
│   └── Instance marked suspensa=true, pendet_ab=task_id
└── Event logged: SALVE_EVENT_STATE_ENTERED

User responds Tuesday
├── UI calls salve_task_respondere(task_id, {text: "Finished spec..."})
├── Engine finds instance via task's workflow_instance_id
├── Executes on_response actions:
│   ├── Set property "notes" = "Finished spec..."
│   ├── Set property "last_review" = current timestamp
│   └── Transition: prompting → idle
├── Instance marked suspensa=false
└── Event logged: SALVE_EVENT_STATE_ENTERED (idle)

Following Monday
└── Cycle repeats
```

---

## Part 8: Files Summary

### New Files

| File | Purpose |
|------|---------|
| `include/salve.h` | Core types: MachinaStatusDef, StatusDef, InstanceMachinae, etc. |
| `include/salve_cron.h` | Cron expression parsing API |
| `include/salve_stml.h` | State machine STML parser API |
| `include/salve_engine.h` | Workflow engine API |
| `include/inbox_task.h` | Inbox task types and API |
| `lib/salve_cron.c` | Cron implementation |
| `lib/salve_stml.c` | STML parser implementation |
| `lib/salve_engine.c` | Engine implementation |
| `lib/inbox_task.c` | Task creation and response |
| `tests/probatio_salve_cron.c` | Cron tests |
| `tests/probatio_salve_stml.c` | Parser tests |
| `tests/probatio_salve_engine.c` | Engine tests |

### Modified Files

| File | Change |
|------|--------|
| `lib/entitas_repositorium.c` | Register new genera |
| `lib/schirmata.c` | Integrate salve_tick into main loop |
| `include/genera.h` | Add genus constants |

---

## Part 9: Success Criteria (First Milestone)

1. **Cron parsing**: Parse standard cron expressions, calculate next occurrence
2. **STML parsing**: Parse state machine definitions from STML
3. **Instance creation**: Create workflow instance from definition
4. **State transitions**: Execute transitions with on_enter/on_exit actions
5. **Inbox tasks**: Create tasks, receive responses, route to engine
6. **Schedule triggers**: Cron expressions fire at correct times
7. **Timeouts**: State timeouts create reminder tasks
8. **Persistence**: Instances survive restart, schedules recalculated
9. **Event logging**: All state changes logged as events
10. **Sputnik guards**: Transition conditions evaluated via Sputnik

---

## Part 10: Open Questions Resolved

1. **Cron library**: Build from scratch - simple enough, avoids dependencies
2. **STML extensions**: No new syntax needed - existing STML sufficient
3. **Inbox widget**: Build alongside - InboxTask entity + minimal UI
4. **Sputnik integration**: Use existing interpreter with REPO + instance context
5. **Merkle chain**: Defer to later phase - event log sufficient for V1

---

## Dependencies

| Component | Required By |
|-----------|-------------|
| stml.h/stml.c | salve_stml.c (STML parsing) |
| cursus.h/cursus.c | salve_engine.c (message bus) |
| entitas_repositorium.h/c | salve_engine.c (entity storage) |
| sputnik_interpres.h/c | salve_engine.c (guard evaluation) |
| tempus.h/tempus.c | salve_engine.c (timing) |
| tabula_dispersa.h/c | All (hash maps) |
| xar.h/xar.c | All (dynamic arrays) |
| chorda.h/chorda.c | All (strings) |
| piscina.h/piscina.c | All (memory) |
