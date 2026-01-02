# Scientia - Encyclopedia/Education Specification

Date: 2025-12-31
Status: Draft
Related: mercatura.md, iustitia.md, salve.md, smaragda.md, lapideus.md
Etymology: Latin "scientia" = "knowledge, science" — the systematic understanding of any domain

---

## Executive Summary

Scientia is the knowledge sphere of Lapideus, providing a structured encyclopedia of all sciences — from classical liberal arts to practical trades — with integrated gamification through a printable card game system. Unlike modern education's separation of theory and practice, Scientia unifies applied technique and philosophical reflection within each science, creating a complete learning path from curiosity to mastery.

**Core Mission**: Make all human knowledge accessible, structured, and engaging through a combination of canonical texts, skill trees, and card-based learning games.

**Key Differentiators:**
- Unified sciences: Every field includes both techne (practice) and philosophia (contemplation)
- LCG-style gamification: Printable card decks that function as state machines for learning
- Federated content: Encyclopedia assembled from internets you belong to
- Canonical books: One definitive text per science, improved across centuries
- Gloss culture: Commentary system addressed to specific paragraphs/sentences
- Roguelike generation: Procedurally generated learning adventures
- Catholic philosophical grounding: Contemplation and sharing fruits thereof

**First Milestone**: Single complete science demo OR card system prototype, potentially with lisp-syntax math curriculum.

---

## Core Philosophy

### Reunifying Techne and Episteme

The Greeks made a mistake: they separated craft (techne) from knowledge (episteme), treating manual work as beneath philosophical consideration. This created:
- "Theoretical" education disconnected from doing
- "Vocational" training disconnected from understanding
- Lost wisdom that emerges from reflective practice

Scientia corrects this by requiring every science to have both:
- **Applied dimension**: Techniques, tools, materials, projects, skills
- **Philosophical dimension**: Patterns observed through practice, theological/mystical connections, truth discerned through experience

### The Tree of Sciences

Not a hierarchy of prestige but a taxonomy of domains:
- **Classical branches**: Trivium (Grammar, Logic, Rhetoric), Quadrivium (Arithmetic, Geometry, Music, Astronomy)
- **Trade branches**: Every occupation is a science (plumbing, woodworking, selling Christmas trees)
- **Multiple views**: Same content browsable by classical structure, life domains, or practical goals

Every node in the tree follows the same structural pattern, making navigation intuitive across domains.

### Contemplation and Its Fruits

Following Aquinas: the Christian life is contemplation and sharing the fruits of contemplation. Following Bonaventure: every science can be a road to God. The philosophical dimension of each science is explicitly Catholic, drawing connections between practice and truth.

---

## Architecture Overview

### Content Sources

```
┌─────────────────────────────────────────────────────────────────┐
│                         SCIENTIA                                 │
│                                                                  │
│  ┌────────────────────────────────────────────────────────────┐ │
│  │                    Your Internets                           │ │
│  │                                                              │ │
│  │   Family ──┐    Guild ──┐    Aggregator ──┐                 │ │
│  │   Internet │    Net     │    (Catholic    │                 │ │
│  │            │            │    Encyclopedia)│                 │ │
│  │            └────────────┴─────────────────┘                 │ │
│  │                         │                                    │ │
│  │                         ↓                                    │ │
│  │              ┌──────────────────┐                           │ │
│  │              │   Merged View    │                           │ │
│  │              │   (configurable  │                           │ │
│  │              │    filtering)    │                           │ │
│  │              └──────────────────┘                           │ │
│  └────────────────────────────────────────────────────────────┘ │
│                                                                  │
│  User can:                                                       │
│  - Include/exclude internets per domain                         │
│  - Filter out specific contributors                             │
│  - Cherry-pick paragraphs                                       │
│  - Add personal content to their view                           │
└─────────────────────────────────────────────────────────────────┘
```

### Content Structure

```
Science: "Scientia Lignarii" (Woodworking)
│
├─ Canonical Book (addressable by paragraph:sentence)
│   ├─ 1. History
│   ├─ 2. Principles
│   ├─ 3. Techniques
│   ├─ 4. Tools
│   ├─ 5. Materials
│   ├─ 6. Projects
│   ├─ 7. Complications (Smaragda-surfaced edge cases)
│   ├─ 8. Masters (Notable practitioners)
│   └─ 9. Philosophy (Contemplative insights)
│
├─ Skill Tree (progression nodes)
│   ├─ Fundamentals → Intermediate → Advanced → Master
│   └─ Branching specializations
│
├─ Card Deck
│   ├─ Tools cards
│   ├─ Resources cards
│   ├─ Skills cards
│   ├─ People cards
│   ├─ Location cards
│   └─ Scenario cards
│
├─ Simulations (Sputnik scripts / hardcoded modes)
│
├─ Glosses (commentary from your internets)
│
└─ Assets
    ├─ Printable cards (poker size)
    ├─ Map pages (hex/overworld)
    └─ 3D print STLs (figurines)
```

---

## Core Concepts

### 1. Standard Taxonomy of Sciences

Every science follows the same category structure:

| Category | Description | Integration |
|----------|-------------|-------------|
| History | How this science developed | Context for current practice |
| Principles | Fundamental concepts | Prerequisites in skill tree |
| Techniques | Methods and procedures | Skills to acquire |
| Tools | Instruments used | Equipment cards |
| Materials | Substances worked with | Resource cards |
| Projects | Things to build/do | Skill tree milestones |
| Complications | Emergent difficulties | Smaragda scenarios |
| Masters | Notable practitioners | People cards, inspiration |
| Philosophy | Contemplative insights | Catholic mystical connections |

### 2. Canonical Books

One definitive text per science, addressable like the Bible:

```
Reference: "Scientia.Lignarii:3.7"
           (Woodworking, paragraph 3, sentence 7)

Properties:
├─ Improved across centuries (long-term investment)
├─ Readable start to finish
├─ Printable or purchasable as physical book
├─ 16-color aesthetic, monospace font
├─ 640x720 two-panel widget display
└─ Version + content-following addressing
```

**Authority hierarchy:**
1. Content producers (authors)
2. Internet owners (curators)
3. Consumer (can remix, cherry-pick, add personal content)

### 3. Gloss Culture

Marginalia system replacing forums:

```
Gloss Entity:
├─ target: Reference to specific paragraph:sentence
├─ author: License key
├─ content: Commentary text
├─ replies: Child glosses (threaded)
└─ source_internet: Where gloss originated
```

Glosses:
- Attach to specific text locations (not floating comments)
- Federated from your internets
- Layer depth (glosses on glosses)
- The closest thing to "discussion" in Lapideus

### 4. Skill Trees

Progression tracking for each science:

```
Skill Tree Node:
├─ id: Unique identifier
├─ name: Skill name
├─ description: What you learn/can do
├─ prerequisites: Required prior skills
├─ validation_modes: [
│   "self_attestation",
│   "project_completion",
│   "assessment",
│   "external_validation"
│ ]
├─ unlocks: Cards, content, badges
└─ estimated_effort: Time/projects to acquire
```

**Visualization:**
- Dedicated tree widget (no scrolling, only pagination)
- Fixed chunks of nodes per page
- Navigate by clicking nodes (like folders)
- Breadcrumb trail for context

### 5. Card Deck System (LCG Model)

Printable poker-size cards functioning as learning state machines:

**Card Entity Types:**
| Type | Description | Examples |
|------|-------------|----------|
| Tools | Instruments used | Saw, Plane, Chisel |
| Resources | Materials/supplies | Oak, Pine, Glue |
| Skills | Abilities to deploy | Dovetail Joint, French Polish |
| People | Characters/mentors | Master Craftsman, Customer |
| Locations | Places for encounters | Workshop, Lumber Yard |
| Scenarios | Situations to navigate | Rush Order, Warped Board |

**Game Modes:**
- Quiz mode: Test knowledge from cards
- Beat monsters mode: Overcome challenges
- Sandbox mode: Free deck-building, Scribblenauts-style
- Scenario mode: Smaragda-style simulations

**Key Properties:**
- Free mixing across sciences
- Unlock new cards by completing scenarios
- Minimal sync burden (just track unlocks)
- Physical deck is primary, digital tracks progress

### 6. Roguelike Generation

Procedurally generated learning adventures:

```
Generated Adventure:
├─ seed: Current skills, studying sciences, random factors
├─ overworld: Hex map or page-based (Jaws of the Lion style)
├─ encounters: Drawn from card deck
├─ progression: Unlock cards, reach locations
└─ printable: Physical book/map pages
```

**Includes:**
- Hex maps for tactical encounters
- 3D printable figurines (bundled with content)
- Ringbound book format option
- Cottage business opportunity: printing services

---

## Gamification Philosophy

### LCG Principles Applied

Inspired by Marvel Champions solo card game:
- **Solo-friendly**: No opponent required
- **Modular**: Mix and match components
- **Extensible**: New cards expand possibilities
- **State machine**: Card rules guide play

### Card Game Challenge

The core design problem: create a generic game engine that:
- Works with cards from any science
- Simulates meaningful scenarios
- Remains engaging as a game
- Teaches through play

Reference games:
- **Distant Skies / Earthborne Rangers**: Overworld exploration
- **Settlers of Catan**: Procedural map generation
- **Gloomhaven: Jaws of the Lion**: Ringbound map book

---

## Math Curriculum (Foundational Layer)

### Lisp-Syntax Math

A new approach to mathematical notation:

**Principles:**
- S-expressions for clear order of operations
- Kebab-case named operators (not symbols)
- Graph theory as foundational framework
- Macros for describing intent

**Example:**
```
Traditional: (a + b) × c ÷ d
Lisp-style:  (divide (multiply (add a b) c) d)

With named operators:
(divide-by d
  (multiply-together
    (sum-of a b)
    c))
```

**Goals:**
- Remove parsing barrier of weird syntax
- Make advanced math accessible
- Unify concepts through graph theory
- Enable macro definitions for common patterns

This system is:
- The mathematics curriculum within Scientia
- A notation used across all sciences for math content
- A foundational computational layer

---

## Curriculum System

Curated learning paths:

```
Curriculum Entity:
├─ title: "Start a Bakery"
├─ author: Creator's license key
├─ sciences: Referenced sciences in order
├─ content_selections: Specific chapters/sections
├─ activities: Exercises, projects, assessments
├─ schedule: Salve integration for pacing
├─ target_audience: Age considerations, prerequisites
└─ printable: Homeschool-ready output
```

**Curriculum Types:**
- Science sequence (study A, then B, then C)
- Goal-oriented (content from multiple sciences for outcome)
- Scheduled study (Salve-integrated timing)
- Homeschool packages (printable activities for parents)

---

## Integration with Other Spheres

### Mercatura (Commerce)

- Skill progression surfaces badges on business profiles
- Sciences map to marketplace categories
- Learning paths lead to earning paths
- Physical books/cards purchasable via marketplace
- Printing services as cottage business opportunity

### Iustitia (Justice)

- Rehabilitation may require completing specific courses
- Authority can assign content with mandatory assessment
- Completion tracking for sentence fulfillment

### Salve (Control Plane)

- Curriculums integrate with scheduling
- Study sessions as managed workflows
- Progress tracking and reminders
- Complication terminology shared with Smaragda

### Smaragda (Simulation)

- Complications category populated by simulation findings
- Card scenarios mirror Smaragda scenarios
- "What would happen if..." for learning

### Lapideus (Network)

- Content federated from internets
- Daily sync for encyclopedia updates
- Merged views with configurable filtering

---

## Widget Implementations

### Book Reader Widget

```
Features:
├─ Paginated display (no scrolling)
├─ Paragraph:sentence addressing
├─ Gloss margin display
├─ 16-color, monospace aesthetic
├─ Chapter navigation
└─ Cross-reference links
```

### Skill Tree Widget

```
Features:
├─ Fixed-chunk pagination
├─ Click-to-drill navigation
├─ Breadcrumb trail
├─ Progress indicators
├─ Unlock notifications
└─ Auto-layout within chunks
```

### Card Viewer Widget

```
Features:
├─ Collection browser
├─ Deck builder
├─ Print layout generator
├─ Unlock status display
└─ Game mode launcher
```

### Simulation Widget (TBD)

Library of hardcoded simulation modes that Sputnik scripts can configure and compose.

---

## Timing and Sync

### Daily Batching

Following Lapideus model:
- New glosses arrive with daily sync
- Book updates are version-tracked
- Card unlocks sync with progress

### Print-Physical Bridge

- Cards designed for home printing
- Minimal sync burden (just unlock tracking)
- Physical deck is offline-complete
- Digital enhances but doesn't require

---

## First Milestone

### Option A: Single Science Demo

Complete implementation of one science:
- Full canonical book with addressing
- Skill tree with progression
- Card deck (all types)
- Basic game mode
- Gloss system working

### Option B: Card System Prototype

The generic card game engine:
- Entity card types implemented
- At least one game mode playable
- Print layout working
- Unlock system functional

### Potential Tie-In: Math Curriculum

Develop alongside either milestone:
- Lisp-syntax math notation
- Graph theory foundations
- Test with mathematics as first science

### Success Criteria

1. Navigate tree of sciences
2. Read book content with paragraph addressing
3. View skill tree for a science
4. Print playable card deck
5. Play at least one game mode
6. Unlock cards through play
7. See gloss commentary
8. Content merges from multiple internets
9. Homeschool-usable output

---

## Future Phases

### Phase 2: Multi-Science

- Tree navigation across all sciences
- Cross-science card mixing
- Curriculum builder

### Phase 3: Roguelike Adventures

- Procedural generation engine
- Hex map system
- 3D printable figurines

### Phase 4: Community Content

- Science authoring tools
- Gloss contribution flow
- Card design interface

### Phase 5: Validation Infrastructure

- Assessment engine
- External validation workflow
- Guild certification integration
- City-state community center support

---

## Dependencies

### Existing (from rhubarb)

- `entitas_repositorium.h/c` - Entity storage
- `persistentia.h/c` - Event log persistence
- `stml.h/c` - Markup parsing
- `sputnik.h/c` - Scripting for simulations
- Widget system for UI

### Required from Other Specs

- **Lapideus**: Federation, sync, internet ownership
- **Mercatura**: Purchasing books/printing services
- **Iustitia**: Rehabilitation course tracking
- **Salve**: Curriculum scheduling
- **Smaragda**: Complication generation

### New Required

- Book reader widget with addressing
- Skill tree widget
- Card game engine
- Print layout generator
- Lisp math parser (for curriculum)

---

## Open Questions

1. **Game design**: What's the core card game loop that's generic enough for any cards but engaging?
2. **Addressing stability**: Exact mechanism for paragraph:sentence addressing across edits?
3. **Generation seeds**: Full specification for roguelike procedural generation?
4. **Simulation library**: What hardcoded simulation modes are needed?
5. **Gloss threading**: Maximum depth? Display constraints?
6. **3D assets**: STL format constraints? Level of detail?
7. **Math notation**: Full specification of lisp-syntax math system?

---

## Appendix: Example Sciences (Top-Level)

### Classical Branch

| Science | Latin | Description |
|---------|-------|-------------|
| Grammar | Grammatica | Structure of language |
| Logic | Logica | Correct reasoning |
| Rhetoric | Rhetorica | Persuasive communication |
| Arithmetic | Arithmetica | Number and computation |
| Geometry | Geometria | Space and shape |
| Music | Musica | Harmony and proportion |
| Astronomy | Astronomia | Celestial patterns |

### Trade Branch (Examples)

| Science | Latin | Description |
|---------|-------|-------------|
| Woodworking | Lignaria | Working with wood |
| Plumbing | Aquaeductus | Water systems |
| Baking | Pistoria | Bread and pastry |
| Accounting | Rationaria | Financial records |
| Gardening | Horticultura | Growing plants |
| Christmas Trees | Arbores Natalis | Growing and selling |

Each follows the same taxonomy: History, Principles, Techniques, Tools, Materials, Projects, Complications, Masters, Philosophy.
