# Graph-Based Entity Navigator
*or: why filesystems are the wrong abstraction for knowledge*

## The Problem Space

Traditional file explorers assume tree structure. One parent, multiple children, strict hierarchy. This is a lie we tell ourselves about data. Reality: everything relates to everything else. Notes reference projects. Projects contain notes. Documents tagged multiple ways. Same entity reachable through different paths.

Filesystems force us to choose ONE canonical location. Wrong abstraction. Data is a graph, not a tree.

## Core Insight

**Every entity is both container and data.**

Not "folders OR files". Every entity has:
- Relationships (edges to other entities - navigable)
- Properties (key-value pairs - displayable)

A "Page" entity might show:
```
RELATIONSHIPS:
  → children/ (5)
  → tags/ (3)
  → linked-from/ (12)

PROPERTIES:
  id: abc-123
  type: Page
  name: "notes"
  content: [multi-line text box]
```

Press 'l' on "children/" - navigate into that relationship, see list of child entities. Press 'l' on "content" - open editor for that property value. Same navigation metaphor, different semantic meaning.

## Navigation Model

**Linear selection through heterogeneous list.**

j/k moves through items. Each item can be:
- Relationship (→ children/ (5)) - renders as one line
- Simple property (id: abc-123) - renders as one line
- Complex property (content: [paragraph]) - renders as multi-line box

But navigation treats ALL as single items. Pressing 'j' on a 5-line property box highlights the WHOLE box and moves past it. Visual complexity doesn't equal navigation complexity.

**Why this works:**
- Consistent mental model (up/down through list)
- Visual richness where needed (multi-line text, arrays, etc)
- No mode switching (don't need "enter property mode" vs "navigate mode")

## Pagination, Not Scrolling

60 items, screen shows 30? Display items 0-29. Visual indicator at bottom (`───┬───`). Press 'j' at item 29 → show items 30-59. Discrete page flip.

**Rationale:**
- Bounded contexts (see 30 items, not infinite stream)
- Predictable (item 15 always in same screen position)
- Simpler (no scroll offset state, no smooth scrolling logic)
- Philosophy match (everything else is paginated)

Could show subtle indicator: `...` or thicker line when more items exist up/down.

## Path History Is Literal Traversal

```
a → b → a → b → a
```

Five entries in stack. Not "smart dedupe". Not "you're already viewing a". Press 'h' five times to rewind.

**Why:**
- Simple (no cycle detection, no special cases)
- Predictable (back always goes where you came from)
- True to navigation (you DID go a→b→a, history reflects that)
- Enables exploration (spiral through graph however you want)

Breadcrumb shows: `root → pages → notes → tags → important → notes`

Yes, "notes" appears twice. That's the POINT - shows your path, not the canonical structure (there isn't one).

## Graph Structure vs Widget State

**Separation of concerns:**

The graph stores entities and relationships (data layer). The widget tracks current view (UI layer).

```c
/* Widget doesn't own data - queries through callbacks */
nomen structura {
    Entitas* (*obtinere_entitatem)(character* id, vacuum* ctx);
    i32 (*obtinere_relationes)(character* ent_id, character* rel_name,
                                Entitas* out[], i32 max, vacuum* ctx);
    vacuum* contextus_data;
} EntitasProvidor;

/* Widget state */
nomen structura {
    EntitasProvidor* providor;
    character via[XXXII][LXIV];  /* Stack of entity IDs */
    i32 profunditas_viae;
    character entitas_currens[LXIV];
    i32 selectio;  /* Which item highlighted */
    i32 pagina;    /* Which page of items */
} NavigatorEntitatum;
```

**Why callbacks:**
- Widget doesn't care HOW entities are stored
- Data layer can be in-memory, sqlite, network, whatever
- Can swap implementations (mock data for testing, real DB for production)
- Clean dependency (widget depends on interface, not implementation)

## Bidirectional Relationships

Graph isn't append-only traversal. Can go a→b, then from b explore back to a, then to b again.

**Implementation choice:** Store edges explicitly or compute inverses on query?

Option 1: If A has relationship "child→B", also store B has "parent→A"
- Faster queries (no scan)
- More storage (duplicate edges)
- Consistency risk (must update both)

Option 2: Single edge A→B, compute inverse with "find all entities where relationship X points to B"
- Less storage
- Slower queries (linear scan)
- Always consistent (single source of truth)

Start with Option 2. Premature optimization is the root of all evil. Linear scan fine for <10K entities. Can optimize later with reverse indices if needed.

## Root Entities and Entry Points

No single "root". Multiple roots with flags:
- `root:system` - system entities, config, etc
- `root:pages` - user pages/notes
- `root:recent` - recently accessed entities
- `root:search` - search results view

Each root is just an entity with `is_root = VERUM` flag. Navigator can show all roots initially, or default to one.

## String Interning Layer

**Observation:** Massive string duplication.
- Entity IDs appear in every relationship pointing to that entity
- Property keys ("name", "content", "created") on every entity
- Type names ("Page", "Module") everywhere
- Relationship names ("children", "tags")

**Solution:** Intern strings globally.

```c
nomen structura {
    TabulaDispersa* tabula;  /* chorda -> chorda* (canonical) */
    Piscina* piscina;
} InternamentumChorda;

chorda* chorda_internare(InternamentumChorda* intern, chorda s);
```

Store each unique string once. Everywhere else stores `chorda*` pointer to canonical copy.

**Benefits:**
- Memory savings (one "children" instead of 1000)
- Fast comparison (pointer equality: `a == b` instead of `chorda_aequalis(a, b)`)
- Cache friendly (pointers small, canonical strings hot)

**Cost:**
- Intern table overhead (hash table for lookups)
- Lifetime coupling (interned strings live until table cleared)

Acceptable tradeoff. Everything allocated from piscina anyway - cleanup is arena reset.

## Data Structures: What We Have

**chorda** (lib/chorda.c):
- Length-prefixed string (i32 mensura + i8* datum)
- No null terminator
- Rich operations: split, concat, trim, hash, compare
- Allocates from piscina

**TabulaDispersa** (lib/tabula_dispersa.c):
- Hash table with open addressing
- chorda keys, void* values
- Iterator support
- FNV-1a hash
- Auto-resizing, tombstone tracking

**Piscina** (arena allocator):
- Bump pointer allocation
- No individual free
- Reset entire arena at once
- Fast, simple, leak-proof

**These three primitives solve entity storage:**
- Intern strings with TabulaDispersa<chorda, chorda*>
- Store entities with TabulaDispersa<chorda, Entitas*>
- Allocate everything from Piscina
- No manual memory management

## Entity Structure (Proposed)

```c
nomen structura {
    chorda* id;      /* Interned */
    chorda* genus;   /* Interned type: "Page", "Module" */

    /* Properties */
    nomen structura {
        chorda* clavis;  /* Interned: "name", "content" */
        chorda* valor;   /* NOT interned (unique values) */
        GenusProprietatis genus;  /* TEXTUS_BREVIS, TEXTUS_LONGUS, etc */
        i32 altitudo_redditionis;  /* Lines to render */
    } Proprietas;

    Proprietas* proprietates;
    i32 numerus_proprietatum;

    /* Relationships */
    nomen structura {
        chorda* nomen;       /* Interned: "children", "tags" */
        chorda** ids;        /* Array of interned entity IDs */
        i32 numerus;
    } Relatio;

    Relatio* relationes;
    i32 numerus_relationum;
} Entitas;
```

**Property types matter for rendering:**
- TEXTUS_BREVIS: `name: "foo"` (one line)
- TEXTUS_LONGUS: Box with paragraph
- NUMERUS: `count: 42`
- ARRAY: `tags: [#foo, #bar, #baz]`
- TEMPUS: `created: 2025-01-15 10:30`

Widget renders each differently but treats all as single navigation item.

## Search Palette

Shift+Space opens fuzzy search overlay:

```
════════════════════════════════════════
Search: proj█
────────────────────────────────────────
  project-x (Page)
  project-notes (Page)
  ProjectManager (Module)
════════════════════════════════════════
```

Type to filter, j/k to select, Enter to jump.

**Jump behavior:** Clear current path stack, start fresh at selected entity? Or append to current path?

Probably clear. Search is "teleport to entity" not "navigate from here to there". User's mental model: "find thing, start exploring from there".

**Search implementation:**
- Substring matching (simple, fast enough)
- Search entity IDs, type, property values
- Fuzzy matching nice-to-have (Levenshtein distance, subsequence scoring)
- Start with simple substring, upgrade if needed

## Mock-First vs Data-First

**Question:** Build widget with fake data first, or build entity system first?

**Mock-first approach:**
```c
Entitas entidades_prueba[V];  /* 5 test entities */
/* Hardcode some circular relationships */
/* Build widget against this */
```

**Benefits:**
- Fast iteration on UI/UX
- Discover what data layer needs
- No premature abstraction

**Data-first approach:**
- Build InternamentumChorda
- Build GraphusEntitatum
- Build query functions
- Then build widget

**Benefits:**
- Real constraints early
- No throwaway code
- Forces API design

**Decision:** Mock first, but use REAL data structures.

Create 5 Entitas structs, manually populate with chorda values (don't intern yet), store in simple array. Widget queries array linearly. No hash table, no interning, just structs.

This tests:
- Widget rendering logic
- Navigation feel
- Property display variants
- Breadcrumb trail
- Pagination math

Once that works, swap array for TabulaDispersa, add interning, add real queries. Widget code mostly unchanged (callback interface same).

## Visual Design Details

**Relationships section:**
- Prefix with `→` or `►`
- Show count: `→ children/ (5)`
- Highlight entire line on selection
- Maybe dim count slightly vs name

**Properties section:**
- Key-value pairs: `name: "foo"`
- Multi-line properties in box:
  ```
  content: ┌─────────────────┐
           │ This is longer  │
           │ text that wraps │
           └─────────────────┘
  ```
- Arrays inline if short: `tags: [#foo, #bar]`
- Arrays as list if long

**Separator between sections:**
- Blank line? Horizontal rule? Subtle color change?
- Keep minimal (every pixel justified at 480p)

**Selection highlight:**
- Inverse video (black on white → white on black)
- Or colored background
- Must be OBVIOUS which item selected

## Property Editing

Press 'l' on property → open editor pane.

**Editor pane shows:**
- Property key: `content`
- Current value (editable text area)
- Buttons/commands: Save, Cancel, Delete Property, History

**History view:**
- Show all past values with timestamps (event sourcing)
- Can revert to previous value
- Can see who changed it (multi-user later)

This is where event sourcing shines. Every property mutation is an event. Editor shows event log.

## Technical Friction Points

**String lifetime management:**
- chorda points into piscina
- If piscina resets, all chorda invalid
- Need careful lifetime boundaries
- Probably: one piscina for session, reset on shutdown

**Property value size:**
- chorda* valor works for most things
- But what about large blobs? Images? Binary data?
- Maybe valor is pointer to separate storage?
- Or valor is chorda for small things, id reference for large?

**Relationship query performance:**
- "Give me all entities with relationship X to entity Y"
- Linear scan O(n * m) where n=entities, m=avg relationships
- For 1000 entities, maybe 10K checks
- Probably fine on modern CPU
- If too slow: build reverse index (relationship → [entities])

**Graph cycles:**
- Can create infinite loops (a→b→c→a)
- Widget doesn't care (path stack is linear)
- But graph queries might loop forever
- Need cycle detection? Or just document "don't do that"?

Start without cycle prevention. Add if becomes problem. YAGNI.

## Testing Strategy

**Phase 1: Mock entities + widget**
- 5 hardcoded entities
- Manual circular relationships
- Test j/k navigation
- Test l entering relationships
- Test h going back
- Test pagination (add 100 mock properties to see paging)
- Visual test in probatio_navigator.c

**Phase 2: Real data layer**
- Build InternamentumChorda
- Build GraphusEntitatum with TabulaDispersa
- Replace array with hash table
- Same widget code, different callbacks

**Phase 3: Persistence**
- Serialize entities to disk
- Load on startup
- Event sourcing log
- But that's later

## Key Decisions Summary

1. **Every entity is container + data** - relationships AND properties, not either/or
2. **Linear navigation through heterogeneous items** - j/k moves through visual items, not semantic types
3. **Pagination over scrolling** - discrete pages, visual indicators, bounded contexts
4. **Literal path history** - stack reflects actual traversal, cycles allowed
5. **Callback interface** - widget queries data, doesn't own it
6. **String interning** - massive deduplication, pointer equality
7. **chorda + TabulaDispersa + Piscina** - primitives already exist, just compose
8. **Mock first with real structures** - test UX early, upgrade to production later
9. **Properties have types** - affects rendering, not navigation
10. **Search is teleport** - clear path, start fresh at result

## Open Questions

- **Default sort order?** Relationships alphabetical? Properties by type then name?
- **Filter views?** "Show only entities of type Page"? Or always show everything?
- **Keyboard shortcuts beyond jklh?** 'g'/'G' for first/last? '/' for search?
- **Visual theme?** Colors for different property types? Relationship icons?
- **Editing relationships?** Can user add/remove edges from navigator? Or read-only?

Defer these. Build basic version. Answers will emerge from using it.

## Implementation Order

1. Create include/entitas.h with Entitas struct
2. Create include/navigator_entitatum.h with widget struct
3. Create probatio_navigator.c with 5 mock entities
4. Implement widget rendering (draw relationships, properties, selection)
5. Implement j/k navigation
6. Implement l/h navigation (enter/back)
7. Implement pagination
8. Test, iterate on feel
9. Build InternamentumChorda
10. Build GraphusEntitatum
11. Replace mock with real data
12. Add search palette
13. Add property editor

Each step builds on previous. Each step testable independently.

## Why This Matters

Not building "file browser with extra features". Building interaction model for knowledge graph. Everything in system is entity. Code modules, notes, commands, history, config - all entities, all navigable.

Traditional filesystems: tree structure imposed on graph data. Square peg, round hole.

This: graph structure for graph data. Navigate naturally. See relationships. Follow connections. Knowledge isn't hierarchical - stop pretending it is.

Constraint: 480p vertical, 70 chars wide. Forces clarity. Every entity must show its essence in limited space. No waste.

Result: fast, keyboard-driven, graph-native navigation. Acme's "text as interface" meets knowledge graphs. Everything clickable, everything navigable, everything connected.

*Build incrementally. Test constantly. Keep simple. The constraint is the vision.*
