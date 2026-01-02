# Ratio - Truth Graph System

## Overview

Ratio is a fundamental data type and UI system for working with directed acyclic graphs of truth claims. It enables users to model, visualize, navigate, and reason about interconnected propositions with explicit logical relationships.

The name comes from Latin "ratio" meaning reason/reasoning.

## Core Concepts

### Truth Claims (Propositio)

A truth claim is an entity representing a proposition that can be true or false. Claims are:

- **Freeform text** in most cases (natural language statements)
- **Expandable** with richer content (explanation, diagrams, sub-graph visualizations)
- **Connected** to other claims via typed logical relationships
- **Annotated** with metadata (author, timestamp, confidence, evidence, counter-evidence)
- **Linkable** to external sources (deep links to paragraphs/sentences, intelligence database, encyclopedia, personal notes)

### Edge Types (Built-in Library)

A finite, non-user-extensible set of logical relationship types:

**Core types (v1):**
- **Supports/Depends** - "This claim requires that claim to be true"
- **Contradicts** - "These claims cannot both be true"
- **Evidence** - "This observation/fact supports that claim"

**Future types:**
- Necessarily implies (deductive)
- Probabilistically supports (inductive)
- Is a special case of
- Undermines
- Analogous to

Edges have standard entity metadata (creator, timestamp) with optional additional properties (strength, confidence).

### Subgraphs

The primitive unit is the **subgraph** - a connected cluster of related claims. Subgraphs are themselves entities that reference their constituent claim entities via relationships.

### Belief States

Users can mark claims as:
- **True** (accepted)
- **False** (rejected)
- With optional **confidence level**
- With optional **reasoning trace** (why they accept/reject)

This creates a personal "belief overlay" on any graph, enabling:
- Seeing which claims you currently accept
- Viewing claims you've rejected with your reasoning
- **Diffing** your belief state against another person's
- Tracking how your beliefs evolved over time (via event sourcing)

## Data Model

### Entity Types

```
Genus: Ratio::Propositio
  - textus: chorda (the proposition text)
  - explicatio: chorda? (optional expanded explanation)
  - fiducia: f64? (optional confidence 0.0-1.0)
  - status: enum (verum, falsum, incertum)

Genus: Ratio::Subgraphum
  - titulus: chorda
  - descriptio: chorda?

Genus: Ratio::Pagina
  - titulus: chorda
  - positiones: array of {propositio_id, x, y}
```

### Relationship Types

```
Relatio: ratio_sustinet (A supports B)
  - fortitudo: f64? (strength of support)

Relatio: ratio_contradicit (A contradicts B)

Relatio: ratio_probat (A is evidence for B)
  - fortitudo: f64?

Relatio: ratio_continet (Subgraph contains Propositio)

Relatio: ratio_in_pagina (Propositio placed on Pagina at position)
```

## UI Design

### Navigation Model

**Paginated infinite canvas** with N/S/E/W navigation between pages:
- Each page is 640x720 (or half-panel width)
- No scrolling - if content extends, navigate to adjacent page
- Breadcrumb trail for backtracking
- Drill-down into subgraphs
- Search available but not primary navigation mode

### Layout

**Manual positioning** by default (author places claims spatially) with optional algorithmic auto-layout heuristics. Position is purely organizational in v1 (no semantic meaning to spatial arrangement).

### View Modes

Multiple visualization modes for the same data:
- **Graph view** - nodes and edges on paginated canvas
- **Memory palace** - Zelda-style spatial navigation
- **Node-wire editor** - traditional box-and-line editing
- **Outline/tree view** - hierarchical collapsible structure

### Facing Panel Pattern

When a claim is selected, the facing panel shows:
- Claim detail (text, explanation)
- Dependencies (what this claim depends on)
- Dependents (what depends on this claim)
- Evidence links
- History/versioning info

### Authoring

Multiple ways to create claims:
- Natural language entry (parsed into structure)
- Structured forms (fill in fields)
- Visual graph editor (draw nodes, drag to connect)
- STML markup

Content style is Acme + HyperCard: plain text with potential embedded commands, inline links to bring up content in current or facing panel.

## Collaboration Model

### Federated via Internet System

- Graphs sync via daily batch like other Lapideus content
- No real-time collaborative editing over internet
- Possible local-network real-time for in-person use

### Async Group Workflow

For groups discussing an issue:
- Each person can have their own fork
- Changes appear in federated batch updates
- Multiple notification methods (Inbox, recent changes feed, on-graph indicators)
- **Tool for clarity about disagreements** rather than comment-section debates

### Annotation Without Modification

When viewing someone else's graph:
- **Local annotations** - add your own notes, only you see them
- **Fork and modify** - create your own copy to edit
- **Structured evidence addition** - append-only, no free-text "comments"

Key principle: **Express disagreement through graph structure mutation** (create counter-claim with contradicts edge) rather than prose comments where people can be rude.

## Integration Points

### Scientia

Ratio is a lower-level data type that Scientia uses for philosophical content. Sciences can embed truth graphs for:
- Laying out the logical foundations of a discipline
- Exploring philosophical implications
- Structured argumentation

### Intelligence Database

Bidirectional integration:
- Claims can link to intelligence entities as evidence
- Intelligence facts can generate claim nodes
- Example: "Event X happened on date Y" becomes a claim node

### Inbox

Optional user-configurable integration:
- Unresolved contradictions can surface as tasks
- Incomplete arguments flagged for attention
- Disputed claims requiring review

### Smaragda (Future)

Potential for "what-if" analysis:
- Toggle claims and see downstream effects
- Stress-test arguments
- Requires more prototype iteration to validate

## Linting and Validation

### Algorithmic Linting

Conservative approach - **better no linting than false positives**:
- Structural issues (circular dependencies, orphaned nodes)
- Disconnected subgraphs
- Only flag clear wins

### LLM-Assisted (Optional)

Available but treated with skepticism:
- LLMs can have same biases as humans
- Graph structure may be more reliable than AI judgment
- User-invoked, not automatic

## History and Versioning

Since everything is event-sourced:
- View claim as of any past date
- See all edits to a claim/argument
- Diff between versions
- Audit log of who changed what when

UI for this is a design challenge but access to all history should be possible.

## First Milestone

**Visual graph** - the minimum slice to prove the concept:
- Render claims as nodes on paginated canvas
- Navigate with N/S/E/W, drill-down, breadcrumbs
- Basic edge rendering (support, contradict, evidence)
- Select claim to see detail in facing panel

## Open Questions / Risks

### UX Complexity

Primary concern is **high surface area** - many pieces of information to expose within pagination constraints. Not that any individual piece is hard, but the aggregate.

Mitigation: Many UI problems overlap with other Lapideus widgets, solutions can be reused.

### Edge Type Library

Need to finalize the built-in set of relationship types. Should be:
- Rich enough to express real reasoning
- Finite and non-extensible (prevent abuse/confusion)
- Grounded in actual logical/epistemological distinctions

### Confidence/Strength Metadata

TBD whether edges need strength/confidence properties beyond type. May emerge from usage.

## Related Specs

- unified-state-paradigm-brainstorm.md (entity/actor model)
- scientia-brainstorm.md (references syllogism DAG)
- scientia-simulation-brainstorm.md (syllogism DAG as core affordance)
- systema-paradigmata-brainstorm.md (Inbox, apophatic nodes)
- iustitia-brainstorm.md (append-only evidence, public record)
