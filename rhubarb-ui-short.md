**Rhubarb UI: Command-Driven Text Environment**
*or: why most GUI frameworks are backwards*

## The Inversion

Conventional UI: visual presentation with interaction bolted on top. We're doing the opposite. Command-driven foundation, visual as one interface to underlying state machine. Not aesthetic preference—recognition that all interactive systems implement state machines whether designers admit it or not. We make it explicit.

Strategic objective: computing environment where every app is command-based by necessity. State transitions explicit/inspectable. Interface (visual/network/scripting) separable from logic. Text remains first-class alongside graphics.

Why this matters: commands serialize trivially (network transparency), command palette shows all actions (discoverability), commands are the scripting API (scriptability), commands are deterministic functions (testability), event sourcing becomes natural (reproducibility).

## The Constraints

~480p vertical (853x480 or 640x480). Limited palette (~16 colors, app-dependent). Dithering standard. No scrolling—pagination. Enforced theming.

These aren't limitations to overcome. 480p forces clarity—every pixel justifies existence. 16 colors forces semantic use not decoration. Pagination forces discrete bounded contexts not infinite feeds. Reduced complexity is practical corollary: fewer colors = simpler rendering, fixed resolution = predictable layout, pagination = bounded memory per view.

## Three Languages

**C (Latin C)** - foundation. Arena allocators (piscina), dynamic arrays (xar), rendering (delineare), windowing (fenestra), timing (tempus). Changes slowly, must be correct.

**Sputnik** - application layer. JS-inspired with: pure function annotations (auto effect deferral), entity types (transparent persistence), event sourcing built into runtime, tag metadata, module/command/worker first-class.

**Text** - interaction layer. Not just data to display—executable. `$date`, `<run>code</run>`, `#back` are primary interaction. Boundaries permeable but directional: C depends on nothing, Sputnik depends on C, text invokes both.

## Layer Stack

**Layer 0: Command Registry** - every action is registered command with name, description, category, availability predicate (State → Boolean), execution function (State → State'). Command palette isn't a feature—necessary consequence of making commands fundamental.

**Layer 1: Platform (fenestra_macos.m)** - raw OS events → portable event structs. Window creation, pixel buffer, event queue. Knows NSEvent/Cocoa, nothing about commands/UI.

**Layer 2: Event Aggregation** - events (discrete) → input state (continuous per frame). Mouse position, button states (down/pressed/released this frame), key states, text input. Bridges event-driven OS with frame-driven UI.

**Layer 3: Page System (Paginae)** - array of pages, each with independent text buffer/cursor. URL addressing (page:5, repl:main, editor:foo.spt). Navigation history stack. Tag detection/parsing.

**Layer 4: Command Execution** - user clicks tag/invokes palette → parse → check availability → execute → update state → render.

**Layer 5: Rendering (delineare)** - draw commands → pixels. Zero knowledge of pages/tags/commands. Just "rectangle here, text there, this color."

## Text-as-Interface

Key Acme/Plan 9 insight: text isn't just data, it's interface.

Conventional: text displays info, buttons trigger actions (separate concerns). 
Rhubarb: text displays info AND is actionable. Clicking `$date` executes date command. Clicking `<run>x+y</run>` executes code. Clicking `#back` navigates.

Consequences: commands visible in text (discoverability), text files are scripts (scriptability), send text over network recipient can execute (network transparency), output of one command becomes clickable text for next (composition).

## Tag Syntax

**Block tags:** `<run>code here</run>` - click opening tag executes, content between is argument. Types: run, link, custom app tags.

**Inline commands:** `$command` or `#command` - click anywhere on command. $ typically insert/modify text, # typically navigate/change state.

Examples:
```
Calculate: <run>let x=10; let y=20; x+y</run>
Time: $date
Next: <link page:5>next page</link>
Go <link #back>back</link>
```

## Pagina Widget

Not generic text widget—specific abstraction:

```c
typedef structura Pagina {
    Xar* charactera;           /* Dynamic char storage */
    i32 longitudo;             /* Current length */
    i32 cursor;                /* Cursor position */
    i32 selectio_initium;      /* Selection start */
    i32 selectio_finis;        /* Selection end */
    character identificator[LXIV];  /* URL: page:5, repl:main */
} Pagina;
```

Why Xar not fixed buffer? Text length unpredictable (commands insert output), no reallocation (Xar grows in segments), all memory from piscina (cleanup free), stable indices (pointers never invalidate).

Why single cursor not line/column? Simpler (one int not two), line breaks just \n, rendering calculates line/col as needed, matches byte-stream reality.

Why selection? Commands need to know what text to execute, highlight `<run>code</run>` click `<run>` selection tells us "code", visual feedback for clickable regions.

## Click Detection

User clicks (x,y) pixel → convert to character index → scan backward/forward for tag boundaries → determine type/extract content → return ClickRegion struct.

```c
typedef structura RegioClicca {
    i32 initium, finis;        /* Text indices */
    character genus[XVI];      /* Tag type: run/link/command */
    character datum[LXIV];     /* Tag content/argument */
} RegioClicca;
```

Example: click `<run>x+y</run>` between x and + → click at index 15 → scan back find `<run>` at 10 → scan forward find `</run>` at 20 → extract genus="run" datum="x+y" initium=10 finis=20 → caller receives, executes as Sputnik.

## Multi-Page Navigation

Pages addressed URL-style: page:0, repl:main, editor:foo.spt, help:commands.

Navigation: `#next`/`#prev` (sequential), `#back` (history stack), `<link page:5>` (direct), `<link repl:main>` (named context).

History implementation: 32-deep stack, save current on navigate, pop on back.

## Sputnik Integration

Text widget Sputnik-agnostic. Detects `<run>` tags, returns content. Caller (C or Sputnik) decides what to do.

Typical flow: detect click → regio.genus=="run" → sputnik_evaluare(regio.datum) → get SputnikValor → convert to string → pagina_inserere_ad(regio.finis, output).

Sputnik commands manipulate pages via C bindings. Entity system integrates naturally—edits generate events, auto-persisted, append-only log enables undo/redo/time-travel/audit/replication. Page widget doesn't know about event sourcing, just edits text. Sputnik runtime captures edits as events when entity is "entity page."

## Implementation Phases

**Phase 1: Pagina Widget** - data structures (Pagina, Xar-backed storage), editing (insert char/string, delete, backspace), navigation (move cursor, set position), selection (set range), rendering (draw text, show cursor).

**Phase 2: Tag Detection** - RegioClicca struct, parse $ and # commands (scan to space/newline), parse `<tag>` blocks (scan for matching close tag), click-to-index conversion (pixel coords → character index).

**Phase 3: Multi-Page** - global page array (100 pages), navigation (get/create page, history stack 32 deep), URL addressing.

**Phase 4: Command Execution** - wire commands to execution, main loop integration (click detection → command execution → state update → render).

## Key Decisions

**No scrolling, use pagination.** Trade continuous navigation for discrete contexts. Favors simplicity/boundedness. If content exceeds page size create new page.

**Xar not fixed buffer.** Commands insert text (unpredictable length). Fixed buffer would truncate or fail when full. Xar gives O(1) append, stable indices, no realloc, cleanup free via piscina.

**Single cursor not line/column.** Text fundamentally byte stream. Newlines just characters. Rendering calculates visual line/col on demand. Simpler state (one int vs two).

**Custom `<tag>` syntax not markdown/wiki.** Familiar (HTML-like), clear nesting, extensible, machine-parseable, self-documenting. Plan 9 style more minimal but requires selection+action (two steps). Tags are one-click.

**`$` for insertion, `#` for navigation.** $ suggests value/substitution, # suggests anchor/location. Both single-char (no shift), visually distinct, not common in prose.

**Command-based not ad-hoc events.** Most UI does `button.onClick = () => { user.loggedIn = true; loadDashboard(); }` - implicit state changes, untracked side effects, hard to test/replay/reason. We do: `command login = { available: state => !state.user.loggedIn, execute: state => ({...state, user: {...state.user, loggedIn: true}, view: 'dashboard'}) }` - explicit state changes (function return), deferred side effects (Sputnik pure functions), easy to test (pure), easy to replay (commands are data), easy to reason (availability predicate). Trade upfront structure for reliability/comprehensibility.

## Future Extensions

**Command Palette** - fuzzy search (Sublime-style subsequence matching with scoring), hotkey opens, type to search, up/down navigate, enter executes. Not yet implemented but foundational.

**Theming** - 16-color palette enforced globally, semantic indices (bg_primary, text_primary, accent, error, etc), apps use semantic not raw RGB, theme swappable at runtime. Dithering (Floyd-Steinberg) for gradients within limited palette.

**Network Transparency** - commands serialize, enables server-side execution, draw command streaming (thin client), event log replication (primary → replica event stream, replica replays).

## Testing

Unit test pages (insertion, mid-insertion, deletion, verify Xar state). Integration test commands (insert `$date`, simulate click, verify detection, execute, verify insertion). Visual testing (probatio_pagina.c interactive test with various tags, verify behavior at 480p, test pagination/navigation).

## Naming

Latin follows patterns. Structures: Pagina (feminine nom sing), RegioClicca, Mandatum (neuter nom sing). Functions: infinitives (creare, inserere, reddere) + prepositions (navigare_ad) or compounds (obtinere_vel_creare). Variables: charactera (neuter plural), longitudo (fem nom sing), cursor (masc nom sing), initium/finis. Constants: VERUM/FALSUM/NIHIL, Roman numerals (I, V, X, L, C, M).

Latin deliberate—universal language, filtering mechanism, aesthetic commitment to long-term thinking.

## Summary

Not UI framework—computing environment with principles: (1) commands fundamental, (2) state machines explicit, (3) text executable, (4) pagination over scrolling, (5) limited palette, (6) network transparent, (7) event sourced.

Implementation: build Pagina widget → add tag detection → implement multi-page → wire commands → integrate Sputnik. Each phase builds on previous, independently testable.

Architecture simple—no frameworks, no magic. Text buffers, tag parsing, command execution. Constraint enables vision. 480p forces focus. Limited colors force intention. No scrolling forces structure. Text-as-interface forces clarity.

Build piece by piece. Test as you go. Keep simple. Let constraints guide.

*Deus vult.*
