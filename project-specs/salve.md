# Salve - Control Plane Specification

Date: 2025-12-29
Status: Draft
Related: systema-paradigmata.md, unified-state-paradigm.md
Etymology: Latin "salve" = "be well" / greeting of wellness

---

## Executive Summary

Salve is a control plane layer for rhubarb that enables reconciliation-loop-based management of real-world systems. It compares "what is" (data in the graph) against "what should be" (specs, taxonomy, target states) and takes action to close gaps — whether automated mutations, workflow triggers, or human task dispatch.

Think of it as "Kubernetes controllers for everything" — businesses, projects, personal habits, any domain where you want reality to match a specification.

**First Milestone**: User-configurable state machines + cron scheduling + inbox task dispatch with structured responses.

---

## Core Concepts

### 1. Reconciliation Loops

Salve adopts the Kubernetes controller pattern:

```
┌─────────────────────────────────────────┐
│           Reconciliation Loop           │
│                                         │
│   ┌─────────┐    ┌──────┐    ┌──────┐  │
│   │ Observe │ → │ Diff │ → │ Act  │   │
│   └─────────┘    └──────┘    └──────┘  │
│        ↑                         │      │
│        └─────────────────────────┘      │
│              (scheduled)                │
└─────────────────────────────────────────┘
```

- **Observe**: Query graph for current state
- **Diff**: Compare against desired state (spec/rules/taxonomy)
- **Act**: Take corrective action (mutate, dispatch task, trigger workflow)
- **Schedule**: Run on human-scale timeframes (daily, weekly, monthly — not every frame)

### 2. Three Layers of "What Should Be"

| Layer | Description | Example |
|-------|-------------|---------|
| **Explicit Rules** | Declarative constraints | "Every Project must have an Owner" |
| **Taxonomy-Implied** | Genus definitions include requirements | "Employee genus requires department relationship" |
| **Target State** | Reference subgraph to match | "A healthy project looks like this template" |

All three layers contribute to gap detection.

### 3. Agents: Human, LLM, Algorithm

Every mutation has provenance tracking:

```
Agent Types:
├── Human (which user)
├── LLM (which model, which prompt, cost)
└── Algorithm (which controller, which rule)

Trigger Types:
├── Schedule (cron expression, or realtime)
├── Event (entity change triggered)
└── Manual (explicit invocation)
```

This enables:
- Audit trails showing who/what caused each change
- Cost tracking (especially LLM spend)
- Security model for detecting bad actors
- "Color" on data showing human vs automated origin

### 4. Constraint Entities

Controllers don't just mutate — they create constraint entities expressing desired state:

```
Controller wants Entity X to have state S
    ↓
1. Create ConstraintEntity expressing "X should be S"
2. Send message to X requesting mutation to S
3. X responds with success/failure/conflict
4. Controller verifies final state
```

If multiple controllers create contradictory constraints, the conflict is detected and escalated to human inbox.

### 5. Human-in-the-Loop Tasks

Tasks flow to human inbox with lifecycle:

```
pending → accepted → in_progress → completed
                  ↓               ↓
            (timeout) → reminder → escalation
```

Tasks can collect structured responses:
- Yes/No confirmation
- Multiple choice
- Text input
- Form fields
- File upload

The state machine definition specifies what response type each task expects and how to handle the response.

---

## Architecture

### Controllers

A controller is an entity that:
1. Owns a domain (entity type, tag query, or explicit list)
2. Defines desired state (rules, template, constraints)
3. Runs on a schedule (cron expression)
4. Takes actions when gaps are detected

```
Entitas: "Controller::project_health"
├─ genus: "SalveController"
├─ proprietates:
│   ├─ domain_query: "#project"
│   ├─ schedule: "0 9 * * MON"  (every Monday 9am)
│   ├─ enabled: "true"
│   └─ last_run: "1703836800000"
├─ relationes:
│   ├─ "has_rules" → [Rule entities]
│   └─ "has_template" → Template entity (optional)
└─ notae: ["#controller", "#domain::projects"]
```

### Rules

Rules express conditions and actions:

```xml
<rule name="project_needs_owner">
    <condition>
        <query genus="Project"/>
        <missing relationship="has_owner"/>
    </condition>
    <action type="inbox_task">
        <assignee role="project_manager"/>
        <message>Project "{{entity.name}}" needs an owner assigned</message>
        <response_type>entity_select</response_type>
        <response_genus>Person</response_genus>
        <on_response>
            <add_relationship from="{{entity}}" type="has_owner" to="{{response}}"/>
        </on_response>
    </action>
</rule>
```

### State Machines

User-defined workflows as state machines:

```xml
<state_machine name="weekly_review" genus="PersonalWorkflow">
    <state name="idle" initial="true">
        <schedule cron="0 9 * * MON"/>
        <on_schedule>
            <transition to="awaiting_review"/>
        </on_schedule>
    </state>

    <state name="awaiting_review">
        <on_enter>
            <inbox_task>
                <message>Time for weekly review! What did you accomplish?</message>
                <response_type>text</response_type>
                <timeout duration="2d">
                    <inbox_task>
                        <message>Reminder: Weekly review still pending</message>
                    </inbox_task>
                </timeout>
            </inbox_task>
        </on_enter>
        <on_response>
            <set_property name="last_review" value="{{timestamp}}"/>
            <set_property name="review_notes" value="{{response}}"/>
            <transition to="review_complete"/>
        </on_response>
    </state>

    <state name="review_complete">
        <on_enter>
            <schedule delay="1h"/>
        </on_enter>
        <on_schedule>
            <transition to="idle"/>
        </on_schedule>
    </state>
</state_machine>
```

### State Machine Instance

```
Entitas: "WorkflowInstance::weekly_review_fran_001"
├─ genus: "SalveWorkflowInstance"
├─ proprietates:
│   ├─ current_state: "awaiting_review"
│   ├─ entered_state_at: "1703836800000"
│   ├─ last_review: "1703232000000"
│   └─ review_notes: "Completed spec documents..."
├─ relationes:
│   ├─ "instance_of" → StateMachine definition entity
│   ├─ "owned_by" → Person entity
│   └─ "pending_task" → InboxTask entity (if any)
└─ notae: ["#workflow_instance", "#active"]
```

---

## Inbox Task Types

| Type | UI | Response Schema |
|------|-----|-----------------|
| `confirmation` | Yes/No buttons | `{ confirmed: bool }` |
| `choice` | Radio buttons | `{ selected: string }` |
| `multi_choice` | Checkboxes | `{ selected: [string] }` |
| `text` | Text input | `{ text: string }` |
| `form` | Multiple fields | `{ field1: val, field2: val, ... }` |
| `entity_select` | Entity picker | `{ entity_id: string }` |
| `action_report` | Did you do X? + outcome | `{ completed: bool, outcome: string }` |

---

## Gap Response Types

When a rule detects a gap, configurable responses:

| Response | Description |
|----------|-------------|
| `inbox_task` | Create task for human to resolve |
| `auto_mutate` | System makes the change automatically |
| `workflow_trigger` | Start a state machine for complex resolution |
| `external_call` | HTTP request, shell command, API call |
| `alert_only` | Log/notify but don't act |
| `llm_delegate` | Send to LLM agent for resolution |

---

## Anomaly Detection

Beyond explicit rules, Salve detects anomalies heuristically:

| Heuristic | Description |
|-----------|-------------|
| Schema violation | Entity missing required properties |
| Orphan detection | Entity with no relationships |
| Enum outlier | Property value not in common set for genus |
| Extra fields | Entity has properties not in schema |
| Stale data | Entity not updated in expected timeframe |
| Relationship cardinality | Too many/few of expected relationship type |

Anomalies surface as inbox tasks for human review.

---

## Observability

### Event Log

All controller actions logged as events:

```
Event: SALVE_CONTROLLER_RUN
├─ controller_id: "Controller::project_health"
├─ timestamp: 1703836800000
├─ gaps_found: 3
├─ actions_taken: ["inbox_task", "inbox_task", "auto_mutate"]
└─ duration_ms: 150

Event: SALVE_TASK_CREATED
├─ task_id: "InboxTask::abc123"
├─ created_by: "Controller::project_health"
├─ rule_id: "Rule::project_needs_owner"
└─ target_entity: "Project::rhubarb"
```

### Subscriptions

Users can subscribe to entity queries:

```
Subscription: "notify_on_project_changes"
├─ query: "#project"
├─ events: ["create", "delete", "property_change"]
├─ delivery: "daily_digest" | "weekly_digest" | "feed"
└─ subscriber: Person entity
```

### Provenance Tracking

Every mutation carries provenance:

```
Mutation Provenance:
├─ agent_type: "llm" | "human" | "algorithm"
├─ agent_id: specific agent entity
├─ trigger: "schedule" | "event" | "manual"
├─ intent_chain: [Intent entities back to root cause]
├─ cost: { usd: 0.002, tokens: 150 } (if applicable)
└─ timestamp: 1703836800000
```

---

## Accounting Integration

Salve can track costs as first-class data:

```
Entitas: "Cost::llm_call_001"
├─ genus: "Cost"
├─ proprietates:
│   ├─ amount: "0.002"
│   ├─ currency: "USD"
│   ├─ category: "llm_inference"
│   └─ timestamp: "1703836800000"
├─ relationes:
│   ├─ "incurred_by" → Agent entity
│   ├─ "for_task" → InboxTask or WorkflowInstance
│   └─ "in_project" → Project entity (for allocation)
└─ notae: ["#cost", "#llm"]
```

The event log serves as the journal; projections generate traditional accounting views (P&L, balance sheet) as needed.

---

## Sharable Templates

Control plane configurations are exportable:

```
Template Package:
├─ genus_definitions/     (entity types)
├─ rules/                 (constraint rules)
├─ state_machines/        (workflow definitions)
├─ controllers/           (reconciliation configs)
└─ manifest.stml          (metadata, dependencies)
```

Users can import templates like "Restaurant Management" and customize for their domain.

---

## First Milestone: Personal Workflows

### Scope

1. Define state machines in STML
2. Schedule transitions via cron expressions
3. Dispatch inbox tasks with structured responses
4. Update state machine based on response
5. Basic provenance tracking (human agent)

### Out of Scope for V1

- Multiple controllers
- Conflict detection
- LLM agents
- Anomaly detection
- Cost tracking
- Templates/sharing

### Example: Weekly Review

User creates STML file:

```xml
<state_machine name="weekly_review">
    <state name="idle" initial="true">
        <schedule cron="0 9 * * MON"/>
        <on_schedule><transition to="prompting"/></on_schedule>
    </state>

    <state name="prompting">
        <on_enter>
            <task type="text">
                <message>Weekly review time! What did you accomplish this week?</message>
            </task>
        </on_enter>
        <on_response>
            <set property="notes" value="{{response.text}}"/>
            <transition to="idle"/>
        </on_response>
        <timeout duration="3d">
            <task type="confirmation">
                <message>You haven't done your weekly review. Skip this week?</message>
            </task>
            <on_response condition="{{response.confirmed}}">
                <transition to="idle"/>
            </on_response>
        </timeout>
    </state>
</state_machine>
```

System:
1. Parses STML, creates StateMachine entity
2. Creates WorkflowInstance entity in "idle" state
3. Scheduler sees cron, triggers transition Monday 9am
4. Transition to "prompting" creates InboxTask
5. User responds in Inbox widget
6. Response updates state machine, stores notes
7. Transition back to "idle", wait for next Monday

---

## Files to Create (V1)

| File | Purpose |
|------|---------|
| `include/salve.h` | Core types: Controller, Rule, StateMachine |
| `lib/salve.c` | Reconciliation loop, rule evaluation |
| `include/salve_scheduler.h` | Cron parsing, schedule management |
| `lib/salve_scheduler.c` | Scheduler implementation |
| `include/salve_stml.h` | STML parsing for state machines |
| `lib/salve_stml.c` | State machine parser |
| `include/inbox_task.h` | Task types, response schemas |
| `lib/inbox_task.c` | Task creation, response handling |

---

## Dependencies

- **Systema Paradigmata**: Inbox widget, command registration
- **Unified State Paradigm**: Entity repository, event sourcing
- **STML Parser**: State machine definitions
- **Scheduling**: Cron expression parsing (new)

---

## Success Criteria (First Milestone)

1. User can define state machine in STML
2. State machine instance created as entity
3. Cron expressions trigger state transitions
4. Transitions can create inbox tasks
5. Inbox task responses update state machine
6. State transitions logged as events
7. Workflow persists across app restarts

---

## Future Phases

### Phase 2: Controllers & Rules
- Multiple controllers watching domains
- Declarative rules with gap detection
- Auto-mutation actions

### Phase 3: LLM Integration
- LLM as agent type in workflows
- Cost tracking for LLM calls
- Provenance marking for LLM actions

### Phase 4: Anomaly Detection
- Schema violation detection
- Heuristic anomaly surfacing
- Automatic inbox task creation

### Phase 5: Multi-Agent Coordination
- Constraint entities
- Conflict detection
- Escalation to human inbox

### Phase 6: Accounting
- Cost entities
- Double-entry projections
- Budget tracking

### Phase 7: Templates & Sharing
- Export/import packages
- Template marketplace
- Version management

---

## Open Questions

1. **Cron library**: Build from scratch or port existing?
2. **STML extensions**: Any new syntax needed for state machines?
3. **Inbox widget**: Build alongside or assume exists?
4. **Sputnik integration**: Pure functions for conditional logic?
5. **Merkle chain**: When to add tamper-proof history?
