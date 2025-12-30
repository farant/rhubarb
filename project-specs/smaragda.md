# Smaragda - AI Simulation Sandbox Specification

Date: 2025-12-30
Status: Draft
Related: salve.md, systema-paradigmata.md, unified-state-paradigm.md
Etymology: Greek σμάραγδος (smaragdos) = "emerald" — something precious revealed through process

---

## Executive Summary

Smaragda is an AI-driven simulation sandbox for wargaming systems, business plans, and state machines. It uses LLM actors to role-play through scenarios, surfacing "complications" — emergent edge cases that couldn't be anticipated analytically.

The core insight: real-world plans fail because of computational irreducibility. You can't predict everything that will go wrong from first principles. Smaragda accelerates the discovery of these failure modes by running AI-powered wargames against your systems, letting you iterate on your mental model before deploying to reality.

Think of it as: Dwarf Fortress + D&D + VisiCalc — emergent complexity from rules, narrative improvisation with AI actors, and accessibility that makes scenario exploration trivially fast.

**First Milestone**: Simple state machine testing — load a state machine (user-authored or LLM-generated), have LLM actors generate interesting inputs, surface complications.

---

## Core Concepts

### 1. The Amateur-to-Expert Model Pipeline

```
Amateur Model → Artifact (state machine, rules) → Simulation → Complications
      ↑                                                              │
      └──────────────── Refined Understanding ←──────────────────────┘
```

Users start with incomplete mental models. They encode what they know into artifacts (state machines, workflow rules, actor definitions). Simulation reveals what they don't know they don't know. This feedback loop transforms amateur models into expert models through accelerated iteration.

### 2. Wargaming with AI Actors

Rather than having the LLM predict aggregate outcomes ("will my business succeed?"), actors operate at a lower level of detail:

- LLM role-plays as individual actors (employee, customer, supplier)
- Each actor "shows their work" — step-by-step reasoning
- The reasoning process itself surfaces missing pieces: "Now I need to enter the code... wait, I don't have the code"

The state machines execute deterministically; the LLM provides creative adversarial input at decision points.

### 3. Complications as Primary Output

A **complication** is an emergent situation that:
- Couldn't be predicted from analyzing the rules alone
- Represents a gap in the model or plan
- Manifests as: resource exhaustion, missing information, blocked workflows, user frustration

Examples:
- "This person was in a scenario where they had run out of X"
- "Customers were irritated because product couldn't do this"
- "We needed to do XYZ and there was no path or resources to do so"

Complications are saved as explicit test cases for regression testing.

### 4. LLM as DM (Dungeon Master)

The LLM manages simulation pacing and external reality:

| Role | Description |
|------|-------------|
| **Actor Simulation** | Role-play as entities with configurable information access |
| **Scenario Generation** | Create plausible disruptions and events |
| **External Improvisation** | Generate responses for out-of-scope interactions |
| **Time Management** | Discretion over turn duration and pacing |
| **Parameter Suggestion** | Propose interesting variations to explore |

### 5. Turn-Based with Automation Gradient

Default mode is **turn-based**:
- User controls when to advance
- Can modify anything between turns
- Can steer simulation with guidance

Gradient toward automation:
- Semi-auto: auto-advance mundane steps, pause on interesting events
- Fully accelerated: run many turns quickly, surface complications

---

## Architecture

### Simulation Sandbox

```
┌─────────────────────────────────────────────────────────────┐
│                    Smaragda Sandbox                         │
│                                                             │
│  ┌─────────────┐    ┌──────────────┐    ┌───────────────┐  │
│  │   Actors    │ ←→ │ State Machines│ ←→ │ LLM Interface │  │
│  └─────────────┘    └──────────────┘    └───────────────┘  │
│         ↓                  ↓                    ↓          │
│  ┌─────────────────────────────────────────────────────┐   │
│  │              Simulation Event Log                    │   │
│  │  (full traces, actor reasoning, complications)       │   │
│  └─────────────────────────────────────────────────────┘   │
│                           ↓                                 │
│  ┌─────────────────────────────────────────────────────┐   │
│  │              Graph Arena (sandbox state)             │   │
│  │         Read-only access to real graph               │   │
│  └─────────────────────────────────────────────────────┘   │
└─────────────────────────────────────────────────────────────┘
```

### Simulation Session

```
Entitas: "SimulationSession::wool_business_v3"
├─ genus: "SmaragdaSession"
├─ proprietates:
│   ├─ name: "Wool Business Exploration"
│   ├─ created_at: "1703923200000"
│   ├─ mode: "turn_based" | "accelerated"
│   ├─ current_turn: "47"
│   ├─ simulated_time: "2025-06-15T14:30:00"
│   ├─ budget_used: "2.34"
│   ├─ budget_limit: "10.00"
│   └─ adversarial_bias: "mild"
├─ relationes:
│   ├─ "has_actors" → [Actor entities]
│   ├─ "has_state_machines" → [StateMachine entities]
│   ├─ "has_complications" → [Complication entities]
│   ├─ "forked_from" → Snapshot entity (optional)
│   └─ "parent_session" → SimulationSession (if branched)
└─ notae: ["#simulation", "#active"]
```

### Simulation Actor

```
Entitas: "SimActor::customer_impatient_01"
├─ genus: "SmaragdaActor"
├─ proprietates:
│   ├─ role: "customer"
│   ├─ persona: "Impatient, price-sensitive, frequent complainer"
│   ├─ information_scope: "realistic" | "god_view" | "custom"
│   ├─ visible_entities: ["Product::*", "Order::self"]
│   └─ llm_model: "haiku"
├─ relationes:
│   ├─ "in_session" → SimulationSession
│   ├─ "follows_state_machine" → StateMachine (optional)
│   └─ "has_interactions" → [SimEvent entities]
└─ notae: ["#sim_actor", "#persona::impatient"]
```

### Complication

```
Entitas: "Complication::resource_exhausted_001"
├─ genus: "SmaragdaComplication"
├─ proprietates:
│   ├─ summary: "Employee needed access code but no one had sent it"
│   ├─ discovered_at_turn: "23"
│   ├─ severity: "blocking"
│   ├─ category: "missing_information"
│   └─ narrative: "[LLM-generated explanation of what went wrong]"
├─ relationes:
│   ├─ "found_in" → SimulationSession
│   ├─ "involved_actors" → [SimActor entities]
│   ├─ "event_trace" → [SimEvent entities in sequence]
│   └─ "saved_as_test" → TestCase entity (if promoted)
└─ notae: ["#complication", "#severity::blocking"]
```

---

## Simulation Modes

### Mode 1: State Machine Testing

Test Salve-style state machines before deployment:

```
Input:
├─ State machine definition (STML or LLM-generated)
├─ Actor definitions (roles + optional personas)
└─ Optional: starting state snapshot

Process:
1. LLM generates interesting inputs at each decision point
2. State machine executes deterministically
3. LLM "shows work" for each actor action
4. System logs full trace
5. Complications surfaced when actors get stuck/frustrated

Output:
├─ Full event trace with reasoning
├─ List of complications found
└─ Coverage metrics (states/transitions exercised)
```

### Mode 2: Business Wargaming

Explore hypothetical ventures:

```
Input:
├─ Text description: "I want to start a wool clothing business in NH"
└─ Optional: constraints, initial capital, timeline

Process:
1. LLM bootstraps: suggests actors (suppliers, customers, employees)
2. LLM generates initial state machines/rules
3. User refines or accepts
4. Simulation runs through months/years of operation
5. LLM introduces realistic disruptions
6. Complications reveal what user didn't think of

Output:
├─ Refined business model artifacts
├─ Known risks and edge cases
└─ Test cases for future iterations
```

### Mode 3: Interactive Exploration

Turn-based wargaming with user as participant:

```
Input:
├─ Any of the above setups
└─ User preference for involvement level

Process:
1. User advances turns manually
2. Can inject "what if" scenarios at any point
3. Can modify rules/actors mid-simulation
4. Can branch to explore alternatives
5. LLM provides commentary and suggestions

Output:
├─ Deepened understanding of system dynamics
├─ Saved branches for comparison
└─ Exportable artifacts (rules, test cases)
```

---

## LLM Integration

### Actor Simulation Prompt Structure

```
You are role-playing as [ACTOR_ROLE] in a simulation.

Your persona: [PERSONA_DESCRIPTION]

Current situation:
[STATE_MACHINE_CURRENT_STATE]
[RELEVANT_CONTEXT_FROM_GRAPH]

You can see:
[SCOPED_VISIBILITY_INFO]

The system is asking you to: [CURRENT_DECISION_POINT]

Show your work: Think through step-by-step how you would respond.
Note any information you need but don't have.
Make your decision and provide your input to the system.
```

### Adversarial Tuning

Configurable per session:

| Level | Description | Use Case |
|-------|-------------|----------|
| Helpful | Things go smoothly, few disruptions | Baseline testing |
| Realistic | Probabilistic issues based on domain | Normal exploration |
| Mildly adversarial | Murphy's law tendencies | Stress testing |
| Hostile | Active attempts to break system | Security/robustness |

### Cost Management

| Strategy | Description |
|----------|-------------|
| Budget per run | Set max spend, simulation stops when exhausted |
| Tiered models | Haiku for routine decisions, Sonnet for complex ones |
| Caching | Similar actor decisions can be cached |
| Batch | Group independent decisions for efficiency |

---

## Visualization (V1: Text-Based)

### Timeline View

```
Turn 23 | 2025-06-15 14:30
├─ [Customer::impatient_01] Tried to check order status
│   └─ "I'm going to the website... looking for my order number..."
│   └─ ⚠️ COMPLICATION: No order confirmation was ever sent
├─ [Employee::warehouse_01] Processing shipment
│   └─ Routine: Picked and packed order #4521
└─ [System] State transition: order_status → shipped

Turn 24 | 2025-06-15 15:00
├─ [Customer::impatient_01] Called support line
│   └─ "I can't find my order! Where is it?"
...
```

### Complication Summary

```
Session: wool_business_v3
Turns: 47 | Simulated: 6 months | Cost: $2.34

COMPLICATIONS FOUND (7):
━━━━━━━━━━━━━━━━━━━━━━━
1. [Turn 23] Missing order confirmation emails
   → Customer couldn't track order, called support

2. [Turn 31] Supplier lead time not accounted for
   → Ran out of raw materials during peak season

3. [Turn 38] No escalation path for quality issues
   → Defective batch shipped, no process to recall
...

[Save as test cases] [Export report] [Continue simulation]
```

---

## Relationship to Other Systems

### Salve Integration

- Smaragda tests Salve state machines before deployment
- Complications become inbox tasks for rule refinement
- Validated rules can be promoted to live controllers

### Systema Paradigmata

- Simulation sessions are widget-based (persist/restore)
- Commands for controlling simulation (advance, branch, etc.)
- Timeline view as inbox-style widget

### Sputnik

- Pure functions for deterministic state transition logic
- LLM handles creative parts, Sputnik handles calculations

### Graph/Entity System

- Sandbox has its own "graph arena"
- Read-only access to real graph for context
- Results can be explicitly promoted to real graph

---

## First Milestone: Simple State Machine Testing

### Scope

1. Load state machine definition (STML)
2. Define actors with personas
3. Run turn-by-turn simulation
4. LLM generates inputs at decision points
5. Full trace logging with "show your work"
6. Surface complications
7. Save complications as test cases

### Out of Scope for V1

- Business wargaming from text description
- JRPG-style visualization
- Concurrent parallel simulations
- Salve-triggered scheduled simulations
- Automatic rule generation from complications

### Example Session

User creates STML state machine for customer support flow:

```xml
<state_machine name="support_ticket">
    <state name="open" initial="true">
        <on_event name="assign">
            <transition to="assigned"/>
        </on_event>
    </state>
    <state name="assigned">
        <on_event name="resolve">
            <transition to="resolved"/>
        </on_event>
        <on_event name="escalate">
            <transition to="escalated"/>
        </on_event>
    </state>
    <!-- ... -->
</state_machine>
```

User defines actors:
- `customer_frustrated`: "Has been waiting 3 days, increasingly angry"
- `support_agent_new`: "First week on the job, unsure of processes"
- `support_agent_senior`: "Experienced but overloaded with tickets"

System runs simulation:
1. Customer submits ticket (LLM generates realistic complaint)
2. New agent receives ticket (LLM shows uncertainty)
3. Various paths explored based on LLM decisions

Complications found:
- "New agent didn't know escalation criteria"
- "No SLA tracking - customer waited 5 days without response"
- "Resolution marked complete but customer still had issue"

---

## Files to Create (V1)

| File | Purpose |
|------|---------|
| `include/smaragda.h` | Core types: Session, Actor, Complication |
| `lib/smaragda.c` | Session management, turn execution |
| `include/smaragda_actor.h` | Actor definition, persona handling |
| `lib/smaragda_actor.c` | Actor state, LLM prompt generation |
| `include/smaragda_trace.h` | Event logging, trace formatting |
| `lib/smaragda_trace.c` | Trace storage and retrieval |
| `include/smaragda_llm.h` | LLM interface abstraction |
| `lib/smaragda_llm.c` | Prompt construction, response parsing |

---

## Dependencies

- **Salve**: State machine definitions and execution
- **STML Parser**: Reading state machine definitions
- **Entity System**: Persistence for sessions and complications
- **LLM Interface**: API calls for actor simulation

---

## Success Criteria (First Milestone)

1. User can load a state machine from STML
2. User can define actors with personas
3. Turn-by-turn advancement works
4. LLM generates plausible actor inputs
5. Full reasoning traces are saved
6. Complications are detected and surfaced
7. Complications can be saved as test cases
8. Session state persists across app restarts

---

## Future Phases

### Phase 2: Business Wargaming
- LLM bootstraps actors and rules from description
- Extended time simulation (months/years)
- Domain knowledge integration

### Phase 3: Parallel Exploration
- Concurrent simulation runs
- Branching and comparison
- Parameter sweep with LLM suggestions

### Phase 4: Visual Modes
- JRPG-style visualization (if asset system exists)
- Graph view of entity relationships
- Interactive timeline scrubbing

### Phase 5: Salve Integration
- Automatic rule suggestions from complications
- Scheduled drift detection simulations
- CI-style validation before deployment

---

## Open Questions

1. **LLM interface**: How do we abstract the LLM calls for future flexibility?
2. **Trace storage**: Event log format — reuse existing event system or specialized?
3. **Graph arena**: Implementation of sandbox graph isolation
4. **Turn semantics**: Flexible turn definition needs clear API
5. **Cost tracking**: Integration with accounting system from Salve
