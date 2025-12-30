# Systema Paradigmata

Date: 2025-12-29
Status: Draft
Related: unified-state-paradigm.md, systema-paradigmata-brainstorm.md
Depends On: unified-state-paradigm (this builds on top of it)

## Summary

Systema Paradigmata defines the OS-like cross-widget paradigms that enable rhubarb to function as a universal computing environment. The core insight is that computers are fundamentally machines for transforming strings — a filetree of strings goes in, gets parsed into a graph (the "AST" or "data tensor"), and projects out as a new filetree of strings. Different "coordinate systems" on the same underlying graph produce different outputs: C code, JavaScript, documentation, visualizations.

This spec defines the paradigms that enable users to navigate, manipulate, and project these hyper-dimensional graphs.

---

## Core Concepts

### The Graph as Intermediate Representation

```
Filetree of Strings  ─→  Graph Database  ─→  Filetree of Strings
    (input)               (tensor/AST)           (output)

Examples:
  .md files        →     Entity Graph    →     HTML website
  C source code    →     Entity Graph    →     Documentation
  User intent      →     Entity Graph    →     Executable program
```

The graph is not just data storage — it is the canonical representation of meaning. File formats are merely "coordinate systems" that project views onto this underlying semantic structure.

### Moderate Realist Taxonomy

Every node in the graph belongs to a real, finite genus within a carefully designed taxonomy. This is not nominalist (arbitrary labels) but moderate realist (real categories of being). The taxonomy enables:

1. **Schema Validation**: Each genus defines required properties/relationships
2. **Inference Rules**: Genus-specific rules can derive missing information
3. **Apophatic Detection**: Rules can identify nodes that SHOULD exist but don't
4. **Type-Safe Transformations**: Tag transformation preserves semantic validity

---

## OS Paradigms

### 1. Inbox (Capsa Intrandi)

**Purpose**: Universal system-to-user communication channel

The Inbox is a widget that queues tasks requiring human attention. All asynchronous system events that need user input flow through this single paradigm.

**Task Sources**:
| Source | Example |
|--------|---------|
| Apophatic nodes | "Function X needs a return type" |
| External file changes | "spec.md was modified outside rhubarb" |
| Scheduled reminders | "Review quarterly goals" |
| Actor requests | "Workflow paused: approve deployment?" |
| Integrity violations | "Entity A references deleted Entity B" |
| Import/export results | "Export to PDF completed" |

**Entity Structure**:
```
Entitas: "InboxTask::abc123"
├─ genus: "InboxTask"
├─ proprietates:
│   ├─ task_type: "apophatic_node" | "file_change" | "reminder" | ...
│   ├─ status: "pending" | "accepted" | "ignored" | "deferred"
│   ├─ priority: "0-10"
│   ├─ detected_time: "1703836800000"
│   ├─ deadline: "1703923200000" (optional)
│   └─ description: "Function 'foo' needs return type"
├─ relationes:
│   ├─ "affects" → [affected entities]
│   └─ "from_workflow" → "Workflow::xyz" (if from state machine)
└─ notae: ["#inbox", "#needs_review"]
```

**User as Actor**: The user is explicitly modeled as an actor in workflows. When a state machine requires human input, it creates an InboxTask and suspends until the user responds. This enables sagas and long-running workflows that span days or weeks.

---

### 2. Commands (Imperata)

**Purpose**: User-to-system action invocation

Commands are the primary way users cause mutations in the system. Commands are:
- **Globally registered**: All widgets register commands in RegistrumCommandi
- **Introspectable**: Commands can be queried, listed, and validated
- **State-aware**: Available commands depend on current context/state machines
- **Polymorphic**: Can trigger state machines, create Inbox tasks, start pipelines

**Command Structure**:
```
Entitas: "Imperium::canvas:navigate"
├─ genus: "Imperium"
├─ proprietates:
│   ├─ nomen: "canvas:navigate"
│   ├─ descriptio: "Navigate to a canvas by address"
│   ├─ schema_argumentorum: "<arg name='address' type='chorda' required='true'/>"
│   ├─ output_mode: "graph_mutation" | "inline_render" | "widget_spawn"
│   └─ available_when: "widget:TabulaExplorator:visible"
├─ relationes:
│   └─ "registered_by" → "Widget::TabulaExplorator"
└─ notae: ["#command", "#navigation"]
```

**Invocation Methods**:

1. **Acme Inline**: `$canvas:navigate /work/projects/rhubarb`
   - Prefix sigil `$` marks executable text
   - Arguments follow command name
   - Rich arguments in STML: `$export <format>pdf</format><quality>high</quality>`

2. **Command Palette**: Fuzzy-searchable popup (first milestone)
   - Keyboard navigation (arrows + enter)
   - Shows command description and argument hints
   - Filters by current context

**Argument Validation**:
- Simple arguments: scalar types (chorda, integer, boolean)
- Complex arguments: STML schema with type constraints
- Entity arguments: "any entity of genus X"

**Output Modes**:
| Mode | Behavior |
|------|----------|
| graph_mutation | Command modifies entities; UI reflects via reactivity |
| inline_render | Command output replaces/appears near command text |
| widget_spawn | Command output opens in new widget/pane |

---

### 3. State Machines (Machinae Status)

**Purpose**: First-class workflows with persistence and human-in-the-loop

State machines are entities that define states, transitions, and guards. They enable:
- Long-running workflows that pause for user input
- Event-driven automation
- Complex multi-step processes (sagas)

**State Machine Definition**:
```
Entitas: "MachinaStatus::deployment_flow"
├─ genus: "MachinaStatus"
├─ proprietates:
│   ├─ nomen: "deployment_flow"
│   └─ status_initialis: "draft"
├─ relationes:
│   ├─ "habet_statum" → ["Status::draft", "Status::review", ...]
│   └─ "habet_transitionem" → ["Transitio::submit", ...]
└─ notae: ["#workflow", "#deployment"]

Entitas: "Status::draft"
├─ genus: "Status"
├─ proprietates:
│   └─ nomen: "draft"
├─ relationes:
│   └─ "in_machina" → "MachinaStatus::deployment_flow"

Entitas: "Transitio::submit"
├─ genus: "Transitio"
├─ proprietates:
│   ├─ ab_statu: "draft"
│   ├─ ad_statum: "review"
│   ├─ imperium: "deployment:submit"  /* Creates command */
│   └─ custos: "<guard>has_tests</guard>"  /* Condition */
```

**Running Instance**:
```
Entitas: "InstanceMachinae::deploy_v2.1"
├─ genus: "InstanceMachinae"
├─ proprietates:
│   ├─ status_currens: "review"
│   └─ suspensa: "true"
├─ relationes:
│   ├─ "machina" → "MachinaStatus::deployment_flow"
│   └─ "pendet_ab" → "InboxTask::awaiting_approval"
```

**Command Integration**: Available commands are derived from valid transitions from current state. If you're in "review" state with transitions to "approved" and "rejected", commands `deployment:approve` and `deployment:reject` become available.

---

### 4. Node-Based Visual Programming (Programmatio Nodorum)

**Purpose**: Visual composition of transformations, queries, and workflows

A graph/node UI paradigm (like Unreal Blueprints, music apps, 3D material editors) for:
- State machine design
- Data flow pipelines
- Query composition
- Template/projection composition

**Node Types**:
| Type | Purpose |
|------|---------|
| Source | Entry point (entity query, file input, event trigger) |
| Transform | Modify data (map, filter, parse, format) |
| Sink | Output (write file, mutate entity, render) |
| Control | Flow control (branch, merge, loop) |
| Human | Pause for user input (creates InboxTask) |

Nodes are entities; connections are relationships. The visual UI is one "projection" of underlying graph data.

---

### 5. Projections (Proiectiones)

**Purpose**: Generate output filetrees from graph state

A projection defines how to transform graph entities into output strings/files. This is the inverse of parsing.

**Tag Transformation Model**:
1. Start with high-level semantic tags (your domain entities)
2. Apply transformation rules that progressively lower abstraction
3. Rules unfold tags into more specific tags (like keyhole optimization)
4. Final layer outputs raw strings in target format

**Example**: Graph → C Code
```
<function name="foo" returns="int">
  <param name="x" type="int"/>
  <body>
    <return><add><var>x</var><literal>1</literal></add></return>
  </body>
</function>

↓ transformation rules ↓

int foo(int x) {
    return x + 1;
}
```

**Apophatic Detection During Projection**:
When transforming to a target format, rules may discover missing information:
- C projection needs return type → not missing
- JavaScript projection needs async/await annotation → MISSING → InboxTask created

**Projection Entity**:
```
Entitas: "Proiectio::c_source"
├─ genus: "Proiectio"
├─ proprietates:
│   ├─ target_format: "c"
│   └─ output_path: "src/"
├─ relationes:
│   └─ "habet_regulas" → [TransformationRule entities]
```

---

### 6. Completeness Rules (Regulae Integritatis)

**Purpose**: Pattern-match subgraphs and detect/infer missing nodes

Rules are Prolog-like declarations that:
1. Match patterns in the graph
2. Assert what else must exist (schema-like)
3. Infer and populate missing information when deterministic

**Rule Structure**:
```
Entitas: "Regula::function_needs_return"
├─ genus: "RegulaIntegritatis"
├─ proprietates:
│   ├─ pattern: "<match genus='Function'><missing property='return_type'/></match>"
│   ├─ action: "flag_apophatic"
│   └─ descriptio: "Every function must have a return type"
├─ relationes:
│   └─ "applies_to_genus" → "Genus::Function"

Entitas: "Regula::truck_has_wheels"
├─ genus: "RegulaIntegritatis"
├─ proprietates:
│   ├─ pattern: "<match genus='Truck' model='F150'/>"
│   ├─ action: "infer"
│   └─ inference: "<set property='wheel_count'>4</set>"
```

**Interaction with Inbox**:
- `flag_apophatic`: Creates InboxTask for user to resolve
- `infer`: Automatically populates the missing data
- `suggest`: Creates InboxTask with proposed value for confirmation

---

### 7. Scheduling (Horarium)

**Purpose**: Time-based triggers for workflows and tasks

**Capabilities**:
| Type | Example |
|------|---------|
| Cron-style | "Run backup every night at 2am" |
| Deadline-aware | "This task must complete by Friday" |
| Event-triggered | "On sunrise, update light theme" |
| Calendar-integrated | "Before meeting, prepare notes" |

**Scheduler Entity**:
```
Entitas: "Horarium::daily_backup"
├─ genus: "Horarium"
├─ proprietates:
│   ├─ schedule: "0 2 * * *"  /* cron syntax */
│   ├─ next_run: "1703908800000"
│   └─ enabled: "true"
├─ relationes:
│   └─ "triggers" → "MachinaStatus::backup_workflow"
```

---

### 8. Networking & Sync (Retia)

**Purpose**: Federated private/public networks sharing graph data

**Model**: Each rhubarb instance is both browser and server. Users belong to multiple "internets" (like Discord servers) that can federate.

**Key Concepts**:
- **Daily batch publishing**: Changes queue until daily sync
- **Forkable data**: Like git, users can fork subgraphs
- **Permission scoping**: Different access levels per network
- **Change aggregation**: Browse all changes across connected internets

**Entity for Network Membership**:
```
Entitas: "Retis::catholic_scholars"
├─ genus: "Retis"
├─ proprietates:
│   ├─ nomen: "Catholic Scholars Network"
│   ├─ sync_frequency: "daily"
│   └─ last_sync: "1703836800000"
├─ relationes:
│   └─ "contains_shared" → [shared entity IDs]
```

---

## First Milestone: Command Palette

### Scope

Implement a functional command palette that demonstrates the command paradigm:

1. Global command registry (enhance existing RegistrumCommandi)
2. Command introspection API (list commands, get schema)
3. Argument validation (basic scalar types + STML)
4. Palette widget with fuzzy search
5. Keyboard navigation (up/down/enter/escape)
6. Command execution with output handling

### Files to Create

| File | Purpose |
|------|---------|
| `include/imperium.h` | Command entity structure and registration |
| `lib/imperium.c` | Command validation, execution |
| `include/paletta_imperorum.h` | Palette widget header |
| `lib/paletta_imperorum.c` | Palette widget implementation |

### Files to Modify

| File | Change |
|------|--------|
| `include/registrum_commandi.h` | Extend for new command structure |
| `lib/registrum_commandi.c` | Command introspection, schema validation |
| `lib/schirmata.c` | Keyboard binding to open palette |

### Command Entity Structure

```c
nomen structura Imperium {
    chorda*         nomen;           /* "canvas:navigate" */
    chorda*         descriptio;      /* Human-readable description */
    chorda*         schema_argumentorum;  /* STML schema for args */
    ImperiumOutputMode  output_mode;
    FunctioImperium     executio;    /* Function pointer */
    chorda*         available_when;  /* Context expression */
} Imperium;

nomen enumeratio {
    IMPERIUM_OUTPUT_MUTATIO     = 0,  /* Mutates graph */
    IMPERIUM_OUTPUT_INLINE      = 1,  /* Renders inline */
    IMPERIUM_OUTPUT_WIDGET      = 2   /* Spawns widget */
} ImperiumOutputMode;

nomen FunctioImperium = vacuum (*)(ContextusWidget* ctx, TabulaDispersa* argumenta);
```

### Palette Widget Structure

```c
nomen structura PalettaImperorum {
    ContextusWidget*    ctx;
    chorda*             quaerens;        /* Current search text */
    Xar*                resultata;       /* Filtered command list */
    i32                 index_selecta;   /* Currently highlighted */
    b32                 visibilis;       /* Is palette open? */

    /* Input state */
    i32                 cursor_positio;

    /* Layout */
    i32                 x, y;
    i32                 latitudo, altitudo;
    i32                 maximus_resultata;  /* Max visible results */
} PalettaImperorum;
```

### Fuzzy Search

Use existing `fuzzy_search.h` library for matching command names against user input.

### Success Criteria

1. Palette opens on keyboard shortcut (Cmd+K or similar)
2. Typing filters commands by fuzzy match
3. Arrow keys navigate results
4. Enter executes selected command
5. Escape closes palette
6. Commands from all widgets appear in palette
7. Invalid arguments show error before execution
8. Command output handled according to mode

---

## Future Phases

### Phase 2: Inbox Widget
- Universal task queue
- Task entities with status, priority, deadline
- Integration with state machines (pause/resume)
- Aggregated view of pending tasks

### Phase 3: State Machine Framework
- State/Transition entities
- Running instance tracking
- Command generation from valid transitions
- Visual editor (node-based)

### Phase 4: Rule Engine
- Completeness rule entities
- Pattern matching on subgraphs
- Apophatic node detection
- Automatic inference

### Phase 5: Projections
- Tag transformation framework
- C/JavaScript/Markdown generators
- Apophatic detection during projection
- Bidirectional sync (future)

### Phase 6: Networking
- Federated network model
- Daily batch sync
- Permission model
- Change aggregation UI

---

## Open Questions

1. **Keyboard shortcut for palette** - Cmd+K? Cmd+P? Configurable?
2. **Command namespacing** - `widget:command` or flat namespace?
3. **STML schema format** - Need to finalize argument schema syntax
4. **State-aware command filtering** - How complex can `available_when` expressions be?
5. **Command history** - Should command invocations be event-sourced?

---

## Success Criteria (Full System)

1. User can invoke any system action through command palette OR Acme inline
2. Long-running workflows pause for user input via Inbox
3. Missing information detected and surfaced automatically
4. Graph can project to multiple output formats
5. Multiple rhubarb instances can sync via federated networks
6. Scheduled tasks run reliably on configured schedules
7. Visual node editor available for composing transformations
