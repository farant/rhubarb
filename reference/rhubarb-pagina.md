# Rhubarb Pagina: Fixed-Size Text Editor
*or: why bounded contexts enable simpler code*

## The Grid

Computing's history: pixel-perfect positioning, responsive layouts, infinite scrolling. We reject all three.

**Grid-based layout** - 6×8 monospace font on 853×480 screen = 142 characters × 60 lines. Everything snaps to character grid. UI widgets don't float at arbitrary pixel coordinates—they occupy grid cells. Not aesthetic preference—recognition that monospace text on fixed resolution creates natural coordinate system.

Why this matters: predictable layout (no reflow), simple positioning (x=column, y=row), stable pagination (line 55 always line 55), no responsive complexity (one target resolution).

## Fixed Dimensions

Codex view occupies **fixed screen region** - roughly 50% vertical split. Two codex views side-by-side. With borders/margins: ~70 characters × 55 lines per view.

**Page capacity = exactly view capacity** - 70 × 55 = 3,850 characters. Not 10K "to be safe", not dynamic based on content. Exactly what fits on screen.

Consequences: page boundary deterministic (like physical book), no "resize window destroys pagination", content either fits or doesn't (if doesn't, create new page), buffer size known at compile time.

## Why Simple Buffer

Explored alternatives:

**Xar (dynamic array)** - grows in segments, no reallocation, stable indices. Rejected: page is bounded, don't need growth. Indirection unnecessary.

**Rope (tree of strings)** - O(log n) operations. Rejected: complex in C89, overkill for 3,850 chars, harder to debug.

**Gap buffer** - array with gap at cursor, O(1) insert. Considered seriously: efficient for typing (most common operation), simple (just character array + two indices tracking gap). Rejected: 3,850 chars small enough that O(n) insert negligible on modern CPU, gap buffer adds complexity (render as two segments, translate logical→physical position), not worth trade.

**Simple fixed array** - `character buffer[3850]`, cursor is index, insert shifts tail. Chosen: simplest possible, O(n) insert where n≤3850 is microseconds (moving 2K chars = nothing), direct indexing (no translation), trivial to render (one loop), matches "bounded context" philosophy.

## Command Output Model

Critical realization: Sputnik commands return **structured objects**, not raw text dumps.

Powershell model: scripts generate objects, UI decides representation. Command returns large array? UI shows: `[1, 2, 3, ... 47 more] <link object:abc123>inspect</link>`. Command returns object? UI shows: `{x: 10, y: 20, ...}` or `<link object:def456>Result</link>`.

Never inserting 50KB text mid-page. Inserting: short representations (few hundred chars), links to detailed views, structured summaries.

This eliminates "command output insertion" as performance concern. Makes simple buffer obviously sufficient.

## Output Panes

Codex page ≠ output display. Executing code in codex pane → output appears elsewhere.

**Output destinations:**
- Other panes/viewports (not inline in codex)
- Entity inspector views
- Visual renderings
- Graph visualizations
- Maybe small acknowledgment in codex ("✓ Function defined")

Codex view shows ~50% screen, other 50% for output/inspection/visualization. Or multiple virtual desktops (tmux-style). Execution context separate from display context.

## Ephemeral vs Persistent

**Pages are persistent** (saved to graph DB), **not ephemeral** (unlike pure REPL scratch).

Can address by number (`page:5`) or name (`page:repl`, `page:notes`). Pages contain history of interactions, experiments, notes. Not "throwaway input buffer."

**But pages ≠ source of truth** for Sputnik modules.

Lisp REPL / Smalltalk image model:
1. Type in page: `function calculateTax(x) { return x * 0.2; }`
2. Execute (click `<run>` tag)
3. Function exists **globally in Sputnik runtime**
4. Delete text from page
5. Function still exists (can call from anywhere)

State lives in: Sputnik runtime (module definitions, entities, functions), event log (all changes persisted), NOT in page text.

**Exception: bidirectional tags** - `<module function="foo">code here</module>` creates two-way binding. Editing tag updates module, module changes update tag. These pages ARE source of truth for specific entities.

## Entity Graph Foundation

Not building isolated "text editor" or "page manager." Building interaction surface for **event-sourced entity graph database**.

**Core system:**
- Everything is entity (type + properties + relationships)
- All mutations are events (append-only log)
- Multiple views over same graph

**Entity types:**
- Page (text content, cursor state, tags)
- Module (Sputnik code modules)
- Function (function definitions)
- Note, Task, File, etc.

**Pages as entities:**
- Page text contains tags
- Tags create relationships (Page ↔ Function, Page ↔ Page)
- Executing code = generating events that modify entity graph
- Page history = events that modified that Page entity

**Navigation = graph queries:**
- "Give me Page entity with id=5"
- "Give me Page entity with name='repl'"
- "Give me all Pages linked from current Page"
- "Give me timeline of events for Page:5"

**Views = query results + rendering:**
- File tree = hierarchical view over Page entities
- Timeline = chronological events
- Graph view = entity relationships visualized
- Tag search = Pages matching tag query
- Command palette = fuzzy search all entities

**No separate "Codex" data structure** - graph DB manages Page entities (among all other entities). "Multi-page navigation" = querying graph. "Page relationships" = graph edges. "History" = event log.

## Pagina Data Structure

Single page text buffer. Will eventually be entity, for now just struct.

```c
#define PAGINA_LATITUDO_CHARACTERA 70   /* Chars per line */
#define PAGINA_ALTITUDO_LINEA 55        /* Lines per page */
#define PAGINA_CAPACITAS (PAGINA_LATITUDO_CHARACTERA * PAGINA_ALTITUDO_LINEA)  /* 3,850 */

typedef structura Pagina {
    character buffer[PAGINA_CAPACITAS];  /* Fixed text buffer */
    i32 longitudo;                       /* Current text length */
    i32 cursor;                          /* Cursor byte offset */
    i32 selectio_initium;                /* Selection start (-1 if none) */
    i32 selectio_finis;                  /* Selection end */
    character identificator[LXIV];       /* "page:5" or "page:repl" */
} Pagina;
```

**Why cursor is byte offset not line/column:**
- Text fundamentally byte stream
- Newlines just `\n` characters
- Simpler state (one int vs two)
- Rendering calculates visual line/col on demand

**Why selection as range:**
- Tags need highlighting (show `<run>code</run>` is clickable)
- Commands need context (what text to execute)
- Visual feedback for user actions

## Rendering Model

**Pagina struct = pure data** (no rendering info). Rendering function takes position/size parameters.

```c
vacuum pagina_reddere(
    TabulaPixelorum* tabula,
    Pagina* pagina,
    i32 x,              /* Grid origin column */
    i32 y,              /* Grid origin row */
    i32 latitudo,       /* Width in chars */
    i32 altitudo,       /* Height in chars */
    i32 scala);         /* Font scale (1=6x8, 2=12x16) */
```

**Line breaking calculated during render:**
- Walk buffer character-by-character
- Track current x position
- Break to next line when:
  1. Hit `\n` (explicit newline), OR
  2. Current x + next char exceeds width (word wrap or char wrap)
- No "line array" stored—computed fresh each frame

**Why compute per frame:**
- Immediate mode GUI philosophy (rebuild each frame)
- Text changes rarely vs renders (type once, render 60fps until next edit)
- Line breaks deterministic (same text + width = same breaks)
- Simpler (no "invalidate cached line breaks" logic)

**Cursor rendering:**
- Convert byte offset to (col, row) during line break calculation
- Draw filled rectangle at (x + col*6*scala, y + row*8*scala)
- Blink animation using tempus (30 frames on, 30 frames off, like aquinas)
- Stays solid while typing (reset blink counter on edit)

**Selection rendering:**
- Convert selectio_initium/finis to (col1,row1) and (col2,row2)
- Draw filled rectangles for highlighted regions (handle multi-line)
- Draw text over highlight with different color

## Font Metrics

Monospace 6×8 font confirmed in fenestra.h:

```c
/* Available functions: */
i32 fons_latitudo_chordae(chorda textus, i32 scala);  /* String width in pixels */
i32 fons_altitudo_chordae(i32 scala);                  /* Text height in pixels */
```

**Character dimensions:**
- Width: 6 pixels × scala
- Height: 8 pixels × scala

**Positioning math:**
- Cursor x = origin_x + (col * 6 * scala)
- Cursor y = origin_y + (row * 8 * scala)
- Chars per line = width / (6 * scala)
- Lines per page = height / (8 * scala)

All integer arithmetic, no floating point, deterministic.

## Editing Operations

**Insert character at cursor:**
```c
vacuum pagina_inserere_characterem(Pagina* p, character c) {
    si (p->longitudo >= PAGINA_CAPACITAS) redde;  /* Full */

    /* Shift tail right */
    per (i32 i = p->longitudo; i > p->cursor; i--) {
        p->buffer[i] = p->buffer[i-1];
    }

    /* Insert */
    p->buffer[p->cursor] = c;
    p->cursor++;
    p->longitudo++;
}
```

O(n) where n = chars after cursor. For n ≤ 3,850 and modern CPU, negligible.

**Delete character (backspace):**
```c
vacuum pagina_delere_characterem(Pagina* p) {
    si (p->cursor == 0) redde;  /* At start */

    /* Shift tail left */
    per (i32 i = p->cursor - 1; i < p->longitudo - 1; i++) {
        p->buffer[i] = p->buffer[i+1];
    }

    p->cursor--;
    p->longitudo--;
}
```

**Move cursor:**
- Left/right: `cursor++` or `cursor--` (with bounds check)
- Up/down: calculate current row/col, move to same col on prev/next row
- Home/end: move to start/end of current line (find `\n` characters)
- Page up/down: move cursor by ~50 lines (or to start/end if less)

**Selection:**
- Start selection: `selectio_initium = cursor`
- Extend selection: move cursor (selectio_finis = cursor)
- Clear selection: `selectio_initium = selectio_finis = -1`

## Phase 1 Scope

Build single-page text editor. **NOT included:**
- Tag detection/parsing (Phase 2)
- Click-to-index conversion (Phase 2)
- Multi-page array/navigation (graph DB integration)
- Command execution (Phase 4)
- Entity graph integration (later)

**Included:**
- Pagina struct (data)
- Editing primitives (insert, delete, move cursor, selection)
- Rendering (draw text, cursor, selection)
- Keyboard input handling (character input, navigation keys)
- Visual test (probatio_pagina.c)

Build foundation. Make text editing work perfectly. Everything else builds on this.

## Testing Strategy

**Unit tests** (probationes/probatio_pagina_unitas.c):
- Insert at start/middle/end
- Delete at start/middle/end
- Cursor movement (left, right, up, down, home, end)
- Selection (create, extend, delete selected)
- Bounds checking (can't insert when full, can't delete at 0)
- Newline handling (insert \n, cursor up/down across lines)

**Visual test** (probationes/probatio_pagina.c):
- Interactive window
- Type characters, see them appear
- Arrow keys move cursor
- Backspace deletes
- Selection with shift+arrows
- Page boundary visualization (show when approaching 3,850 limit)
- Cursor blink animation
- Multi-line text with newlines

## Key Decisions Summary

**Fixed buffer over dynamic** - page bounded by screen, 3,850 chars exactly. No Xar, no rope, no gap buffer. Simple array.

**Grid layout over pixel positioning** - 142×60 character grid. Widgets snap to grid. Predictable, stable, simple.

**Computed line breaks over stored** - calculate during render. Immediate mode philosophy. No cache invalidation complexity.

**Byte offset cursor over line/column** - simpler state, matches byte stream reality. Rendering translates to visual as needed.

**Objects over text dumps** - Sputnik returns structured data. UI shows summaries/links. Never massive text insertion.

**Entity graph foundation** - pages are entities. No separate Codex struct. Graph DB manages all entities. Multiple views over graph.

**Persistent REPL model** - pages saved, but not source of truth (except bidirectional tags). State in runtime + event log.

**Build simple, integrate later** - Phase 1 just text editor. Entity integration comes after foundation solid.

## Implementation Path

1. Create `include/pagina.h` with struct + function declarations
2. Create `lib/pagina.c` with implementations
3. Editing functions: insert, delete, cursor movement, selection
4. Rendering function: line breaks, text, cursor, selection
5. Visual test: window, input handling, display
6. Iterate until editing feels right

Then Phase 2 (tag detection), then graph DB integration, then command execution.

Foundation first. Build incrementally. Test as you go.

*Deus vult.*
