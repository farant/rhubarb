# Rhubarb UI System Design
*Principia et Implementatio*

## I. On the Strategic Vision

### The Nature of the System

We are not building a conventional GUI framework. The distinction is fundamental: conventional UI systems treat interaction as a secondary concern layered atop visual presentation. Rhubarb inverts this - the system is command-driven at its foundation, with visual presentation serving as one interface to an underlying command and state machine architecture.

This is not mere aesthetic preference. The decision stems from a recognition that all interactive systems, whether their designers acknowledge it or not, implement state machines with valid transitions. Most hide this reality behind ad-hoc event handlers and mutable objects. We make it explicit and foundational.

The strategic objective: Build a computing environment where:
- Every application is command-based by necessity, not choice
- State transitions are explicit and inspectable
- The interface (visual, network, scripting) is separable from the logic
- Text remains a first-class interaction medium alongside graphical elements

This has profound implications. A command-based architecture enables:
- Network transparency (commands serialize trivially)
- Discoverability (command palette shows all available actions)
- Scriptability (commands are the scripting API)
- Testability (commands are deterministic functions of state)
- Reproducibility (event sourcing becomes natural)

### The Constraint as Liberator

The system operates under deliberate constraints:
- ~480p vertical resolution (853x480 or 640x480)
- Limited color palette (~16 colors, application-dependent)
- Dithering as standard visual technique
- No scrolling - pagination instead
- Enforced theming and visual consistency

These are not limitations to overcome but principles that shape the design space. A 480p constraint forces clarity - every pixel must justify its existence. A 16-color palette forces thoughtful use of color as semantic signifier, not decoration. Pagination over scrolling forces discrete, bounded contexts rather than infinite feeds.

The aesthetic constraint has a practical corollary: reduced complexity. Fewer colors means simpler rendering. Fixed resolution means predictable layout. Pagination means bounded memory per view.

### The Three Languages

The system speaks three languages at different levels:

1. **C (Latin C)** - The foundation. Arena allocators (piscina), dynamic arrays (xar), core rendering (delineare), windowing (fenestra), timing (tempus). This layer changes slowly and must be correct.

2. **Sputnik** - The application layer. A JavaScript-inspired scripting language with:
   - Pure function annotations with automatic effect deferral
   - Entity types with transparent persistence
   - Event sourcing built into the runtime
   - Tag-based metadata system
   - Module, command, and worker first-class types

3. **Text** - The interaction layer. Text is not just data to display - it is executable. Commands written in text (`$date`, `<run>code</run>`, `#back`) are the primary interaction model.

The boundary between layers is permeable but deliberate. Sputnik can call C through bindings. Text can invoke Sputnik or C commands. But dependencies flow in one direction: C depends on nothing, Sputnik depends on C, text invokes both.

## II. On the Architecture

### The Layer Stack

The system decomposes into distinct layers, each with clear responsibilities:

**Layer 0: Command Registry**
The foundation. Every action in the system is a registered command with:
- Name (string identifier)
- Description (human-readable)
- Category (for organization)
- Availability predicate (f: State → Boolean)
- Execution function (f: State → State')

Commands are first-class. The command palette is not a feature - it is a necessary consequence of making commands the fundamental abstraction.

**Layer 1: Platform (fenestra_macos.m)**
Raw OS events translated into portable event structs. Window creation, pixel buffer management, event queue. This layer knows about NSEvent and Cocoa but nothing about commands or UI elements.

**Layer 2: Event Aggregation**
Events (discrete occurrences) aggregated into input state (continuous state per frame). Mouse position, button states (down this frame? pressed this frame? released this frame?), key states, text input. This bridges event-driven OS layer with frame-driven UI layer.

**Layer 3: Page System (Paginae)**
Multi-page text editor with:
- Array of pages, each with independent text buffer and cursor
- URL-based addressing (page:5, repl:main, editor:foo.spt)
- Navigation history stack
- Tag detection and parsing

**Layer 4: Command Execution**
When user clicks a tag or invokes command palette:
- Parse tag/command
- Check if command available in current state
- Execute command
- Update state
- Render new state

**Layer 5: Rendering (delineare)**
Draw commands → pixels. Knows nothing about pages, tags, or commands. Just: "draw rectangle here, text there, this color."

### The Text-as-Interface Model

The key insight from Acme and Plan 9: text is not just data, it is interface.

In conventional systems:
- Text displays information
- Buttons trigger actions
- These are separate concerns

In Rhubarb:
- Text displays information
- Text *is* actionable
- Clicking `$date` executes the date command
- Clicking `<run>x + y</run>` executes the code
- Clicking `#back` navigates history

This unification has consequences:

**For discoverability:** Commands are visible in text. You see `$date` and learn the date command exists.

**For scriptability:** Text files *are* scripts. Save page to file, load it later, click commands - they execute.

**For network transparency:** Send text over network, recipient can execute it. The text itself encodes the interface.

**For composition:** Commands compose through text. The output of one command becomes clickable text for the next.

### Tag Syntax

We adopt a simple tag syntax:

**Block tags:** `<run>code here</run>`
- Click opening tag to execute
- Content between tags is the argument
- Types: `<run>`, `<link>`, custom application tags

**Inline commands:** `$command` or `#command`
- Click anywhere on the command
- $ commands typically insert/modify text
- # commands typically navigate or change state

**Examples:**
```
Calculate something:
<run>
let x = 10
let y = 20
x + y
</run>

Current time: $date

Navigate to <link page:5>next page</link>

Go <link #back>back</link>
```

### The Pagina Widget

At the core is the page (Pagina) - not a generic text widget, but a specific abstraction:

```c
typedef structura Pagina {
    Xar* charactera;           /* Dynamic character storage */
    i32 longitudo;             /* Current text length */
    i32 cursor;                /* Cursor position (index into charactera) */
    i32 selectio_initium;      /* Selection start (for highlighting) */
    i32 selectio_finis;        /* Selection end */
    character identificator[LXIV];  /* URL: "page:5", "repl:main" */
} Pagina;
```

Why Xar (dynamic array) instead of fixed buffer?
- Text length is unpredictable (commands insert output)
- No reallocation - Xar grows in segments
- All memory from piscina - cleanup is free
- Stable indices - pointers never invalidate

Why single cursor instead of line/column?
- Simpler model - one integer, not two
- Line breaks are just \n characters
- Rendering calculates line/col as needed
- Matches underlying text-as-byte-stream reality

Why selection instead of just cursor?
- Commands need to know what text to execute
- Highlight `<run>code</run>`, click `<run>`, selection tells us "code"
- Visual feedback for clickable regions

### Click Detection

When user clicks at pixel (x, y):

1. Convert pixel coordinates to character index in text
2. Scan backwards/forwards from index to find tag boundaries
3. Determine tag type and extract content
4. Return ClickRegion struct

```c
typedef structura RegioClicca {
    i32 initium;               /* Start index in text */
    i32 finis;                 /* End index in text */
    character genus[XVI];      /* Tag type: "run", "link", "command" */
    character datum[LXIV];     /* Tag content/argument */
} RegioClicca;
```

Example: User clicks on `<run>x + y</run>` between 'x' and '+'

1. Click at character index 15 (the 'x')
2. Scan backward, find `<run>` at index 10
3. Scan forward, find `</run>` at index 20
4. Extract: genus="run", datum="x + y", initium=10, finis=20
5. Return RegioClicca

Caller receives this and decides: "genus is 'run', so execute datum as Sputnik code"

### Multi-Page Navigation

Pages are addressed by URL-like identifiers:
- `page:0`, `page:1`, `page:2` - numbered pages
- `repl:main` - REPL session named "main"
- `editor:foo.spt` - editing file foo.spt
- `help:commands` - help page about commands

Navigation commands:
- `#next` / `#prev` - sequential navigation through numbered pages
- `#back` - history stack navigation
- `<link page:5>` - direct navigation to specific page
- `<link repl:main>` - navigate to named context

History stack implementation:
```c
hic_manens character* historia_paginae[XXXII];  /* 32 deep */
hic_manens i32 positio_historia;

vacuum navigare_ad(character* identificator) {
    /* Save current page to history */
    historia_paginae[positio_historia++] = pagina_currens->identificator;

    /* Navigate to new page (create if doesn't exist) */
    pagina_currens = obtinere_vel_creare_paginam(identificator);
}

vacuum navigare_retro(vacuum) {
    si (positio_historia > 0) {
        character* prior = historia_paginae[--positio_historia];
        pagina_currens = obtinere_paginam(prior);
    }
}
```

### Integration with Sputnik

The text widget is Sputnik-agnostic. It detects `<run>` tags and returns their content. The caller (written in C or Sputnik) decides what to do.

Typical integration:
```c
/* Main loop */
dum (currens) {
    fenestra_perscrutari_eventus(fenestra);

    dum (fenestra_obtinere_eventus(fenestra, &eventus)) {
        si (eventus.genus == EVENTUS_MUSCULI_DEPRESSUS) {
            RegioClicca* regio = pagina_detectare_click(
                pagina_currens,
                eventus.datum.musculus.x,
                eventus.datum.musculus.y
            );

            si (regio && chorda_aequalis(regio->genus, "run")) {
                /* Execute Sputnik code */
                SputnikValor resultatum = sputnik_evaluare(regio->datum);

                /* Insert result after </run> tag */
                chorda* output = sputnik_valor_ad_chordam(resultatum);
                pagina_inserere_ad(pagina_currens, regio->finis, output);
            }
        }
    }

    pagina_reddere(pagina_currens, tabula);
    fenestra_praesentare_pixela(fenestra, tabula);
}
```

Sputnik commands can manipulate pages:
```javascript
// Sputnik code
command navigateToPage = {
    execute: (pageId) => {
        // Calls C function via binding
        navigare_ad(pageId);
    }
}

// Tag in text: <link page:5>
// When clicked, Sputnik receives pageId="page:5"
// Command executes, C function navigates
```

### Event Sourcing and Entity System

Sputnik's entity type integrates naturally with page model:

```javascript
// Sputnik
entity Note {
    title: "",
    content: "",
    tags: []
}

// Create entity - generates event, auto-persisted
let note = Note.create({
    title: "Design Notes",
    content: "Multi-page text editor...",
    tags: ["#design", "#ui"]
});

// Edit entity - generates update event
note.title = "Rhubarb Design Notes";  // Auto-persisted
```

Events stored in append-only log. Page state reconstructed by replaying events. This gives:
- Undo/redo (replay events up to point N)
- Time travel debugging (reconstruct any historical state)
- Audit trail (who changed what when)
- Replication (send events to other machines)

The page widget doesn't know about event sourcing. It just edits text. The Sputnik runtime captures edits as events when the entity is an "entity page."

## III. On Implementation

### Phase 1: Foundation (Pagina Widget)

Build the minimal text editor:

**Data structures:**
```c
/* In campustextus.h (or pagina.h) */

typedef structura Pagina {
    Xar* charactera;
    i32 longitudo;
    i32 cursor;
    i32 selectio_initium;
    i32 selectio_finis;
    character identificator[LXIV];
} Pagina;

/* Creation */
Pagina* pagina_creare(Piscina* piscina, character* identificator);

/* Editing */
vacuum pagina_inserere_characterem(Pagina* p, character c);
vacuum pagina_inserere_chordam(Pagina* p, chorda* s);
vacuum pagina_delere_characterem(Pagina* p);  /* Delete at cursor */
vacuum pagina_retro_delere(Pagina* p);        /* Backspace */

/* Navigation */
vacuum pagina_movere_cursor(Pagina* p, i32 delta);
vacuum pagina_ponere_cursor(Pagina* p, i32 positio);

/* Selection */
vacuum pagina_ponere_selectionem(Pagina* p, i32 initium, i32 finis);

/* Rendering */
vacuum pagina_reddere(Pagina* p, TabulaPixelorum* tabula, i32 x, i32 y);
```

**Implementation notes:**

Character storage via Xar:
```c
Pagina* pagina_creare(Piscina* piscina, character* identificator) {
    Pagina* p = piscina_allocare(piscina, sizeof(Pagina));
    p->charactera = xar_creare(piscina, sizeof(character));
    p->longitudo = 0;
    p->cursor = 0;
    p->selectio_initium = 0;
    p->selectio_finis = 0;
    chorda_copiare(p->identificator, identificator, LXIV);
    redde p;
}
```

Insertion with cursor advancement:
```c
vacuum pagina_inserere_characterem(Pagina* p, character c) {
    character* locus;

    /* Add character to xar */
    locus = xar_addere(p->charactera);

    /* Shift characters right if inserting mid-text */
    si (p->cursor < p->longitudo) {
        i32 i;
        per (i = p->longitudo; i > p->cursor; i--) {
            character* dest = xar_obtinere(p->charactera, i);
            character* src = xar_obtinere(p->charactera, i - 1);
            *dest = *src;
        }
    }

    /* Insert new character */
    locus = xar_obtinere(p->charactera, p->cursor);
    *locus = c;

    p->cursor++;
    p->longitudo++;
}
```

Rendering (simple version, line wrapping handled later):
```c
vacuum pagina_reddere(Pagina* p, TabulaPixelorum* tabula, i32 x, i32 y) {
    i32 cursor_x = x;
    i32 cursor_y = y;
    i32 i;

    per (i = 0; i < p->longitudo; i++) {
        character* c = xar_obtinere(p->charactera, i);

        si (*c == '\n') {
            cursor_x = x;
            cursor_y += 10;  /* Line height */
        } alioquin {
            /* Draw character */
            tabula_pixelorum_pingere_characterem(
                tabula, cursor_x, cursor_y, *c,
                RGB(CCLV, CCLV, CCLV)
            );
            cursor_x += 6;  /* Character width */

            /* Draw cursor if at this position */
            si (i == p->cursor) {
                delineare_lineam_verticalem(ctx, cursor_x, cursor_y, 8,
                    RGB(CCLV, CC, 0));
            }
        }
    }
}
```

### Phase 2: Tag Detection

Add tag parsing without execution:

```c
typedef structura RegioClicca {
    i32 initium;
    i32 finis;
    character genus[XVI];
    character datum[LXIV];
    b32 validum;
} RegioClicca;

RegioClicca pagina_detectare_click(Pagina* p, i32 x_pixel, i32 y_pixel);
b32 invenire_signum_ad(Pagina* p, i32 index, RegioClicca* regio);
```

Tag detection algorithm:
```c
b32 invenire_signum_ad(Pagina* p, i32 index, RegioClicca* regio) {
    character* c;
    i32 i;

    /* Check for $ or # commands */
    c = xar_obtinere(p->charactera, index);
    si (*c == '$' || *c == '#') {
        /* Scan forward to end of command (space or newline) */
        regio->initium = index;
        regio->finis = index + 1;

        dum (regio->finis < p->longitudo) {
            c = xar_obtinere(p->charactera, regio->finis);
            si (*c == ' ' || *c == '\n') {
                frange;
            }
            regio->finis++;
        }

        /* Extract command name */
        i32 len = regio->finis - regio->initium;
        si (len > LXIII) len = LXIII;
        per (i = 0; i < len; i++) {
            c = xar_obtinere(p->charactera, regio->initium + i);
            regio->datum[i] = *c;
        }
        regio->datum[len] = '\0';

        chorda_copiare(regio->genus, "command", XVI);
        regio->validum = VERUM;
        redde VERUM;
    }

    /* Check for <tag> blocks */
    /* Scan backward to find < */
    i = index;
    dum (i >= 0) {
        c = xar_obtinere(p->charactera, i);
        si (*c == '<') {
            /* Found potential tag start */
            /* Extract tag name */
            /* Find matching closing tag */
            /* Extract content between tags */
            /* ... implementation ... */
            redde VERUM;
        }
        si (*c == '>') {
            /* We're inside a tag, keep scanning */
            i--;
            perge;
        }
        i--;
    }

    regio->validum = FALSUM;
    redde FALSUM;
}
```

Click to index conversion:
```c
RegioClicca pagina_detectare_click(Pagina* p, i32 x_pixel, i32 y_pixel) {
    RegioClicca regio = {0};
    i32 cursor_x = 0;
    i32 cursor_y = 0;
    i32 i;

    /* Find character index at click position */
    per (i = 0; i < p->longitudo; i++) {
        character* c = xar_obtinere(p->charactera, i);

        si (*c == '\n') {
            cursor_x = 0;
            cursor_y += 10;
        } alioquin {
            si (y_pixel >= cursor_y && y_pixel < cursor_y + 8 &&
                x_pixel >= cursor_x && x_pixel < cursor_x + 6) {
                /* Clicked on this character */
                invenire_signum_ad(p, i, &regio);
                redde regio;
            }
            cursor_x += 6;
        }
    }

    redde regio;
}
```

### Phase 3: Multi-Page Management

Global page array and navigation:

```c
/* In pagina.c */
hic_manens Pagina* g_paginae[C];  /* 100 pages */
hic_manens i32 g_numerus_paginae = 0;
hic_manens Pagina* g_pagina_currens = NIHIL;

/* History */
hic_manens character* g_historia[XXXII];
hic_manens i32 g_positio_historia = 0;

Pagina* obtinere_paginam(character* identificator);
Pagina* obtinere_vel_creare_paginam(character* identificator);
vacuum navigare_ad(character* identificator);
vacuum navigare_retro(vacuum);
vacuum navigare_proxima(vacuum);
vacuum navigare_prior(vacuum);
```

Implementation:
```c
Pagina* obtinere_vel_creare_paginam(character* identificator) {
    Pagina* p;
    i32 i;

    /* Search for existing page */
    per (i = 0; i < g_numerus_paginae; i++) {
        si (chorda_aequalis(g_paginae[i]->identificator, identificator)) {
            redde g_paginae[i];
        }
    }

    /* Create new page */
    si (g_numerus_paginae >= C) {
        redde NIHIL;  /* No more pages */
    }

    p = pagina_creare(g_piscina_globalis, identificator);
    g_paginae[g_numerus_paginae++] = p;
    redde p;
}

vacuum navigare_ad(character* identificator) {
    Pagina* nova;

    /* Save current page to history */
    si (g_pagina_currens != NIHIL) {
        g_historia[g_positio_historia++] = g_pagina_currens->identificator;
        si (g_positio_historia >= XXXII) {
            g_positio_historia = XXXII - 1;  /* Wrap */
        }
    }

    /* Navigate */
    nova = obtinere_vel_creare_paginam(identificator);
    g_pagina_currens = nova;
}

vacuum navigare_retro(vacuum) {
    si (g_positio_historia > 0) {
        character* identificator = g_historia[--g_positio_historia];
        g_pagina_currens = obtinere_paginam(identificator);
    }
}
```

### Phase 4: Command Execution

Wire up commands to actual execution:

```c
vacuum exsequi_mandatum(RegioClicca* regio) {
    si (chorda_aequalis(regio->genus, "command")) {
        si (chorda_incipit_cum(regio->datum, "$date")) {
            /* Insert date */
            character buffer[XXXII];
            obtinere_tempus_chordam(buffer, XXXII);
            pagina_inserere_chordam(g_pagina_currens, buffer);
        }
        alioquin si (chorda_aequalis(regio->datum, "#back")) {
            navigare_retro();
        }
        alioquin si (chorda_aequalis(regio->datum, "#next")) {
            navigare_proxima();
        }
        /* ... more commands ... */
    }
    alioquin si (chorda_aequalis(regio->genus, "run")) {
        /* Execute Sputnik code */
        SputnikValor resultatum = sputnik_evaluare(regio->datum);

        /* Insert result after closing tag */
        chorda* output = sputnik_valor_ad_chordam(resultatum);
        pagina_inserere_ad(g_pagina_currens, regio->finis, "\n");
        pagina_inserere_ad(g_pagina_currens, regio->finis + 1, output);
    }
    alioquin si (chorda_aequalis(regio->genus, "link")) {
        /* Navigate to linked page */
        navigare_ad(regio->datum);
    }
}
```

Main loop integration:
```c
/* In main() */
dum (currens) {
    fenestra_perscrutari_eventus(fenestra);

    dum (fenestra_obtinere_eventus(fenestra, &eventus)) {
        commutatio (eventus.genus) {
            casus EVENTUS_MUSCULI_DEPRESSUS: {
                RegioClicca regio = pagina_detectare_click(
                    g_pagina_currens,
                    eventus.datum.musculus.x,
                    eventus.datum.musculus.y
                );

                si (regio.validum) {
                    exsequi_mandatum(&regio);
                }
                frange;
            }

            casus EVENTUS_CLAVIS_DEPRESSUS: {
                character c = eventus.datum.clavis.character;
                pagina_inserere_characterem(g_pagina_currens, c);
                frange;
            }
        }
    }

    delineare_vacare(ctx, RGB(0, 0, 0));
    pagina_reddere(g_pagina_currens, tabula, 10, 10);
    fenestra_praesentare_pixela(fenestra, tabula);
}
```

## IV. On Tradeoffs and Decisions

### Why No Scrolling?

**Argument for scrolling:**
- Users expect it
- Smooth navigation through long content
- No artificial boundaries

**Argument against (chosen):**
- Violates bounded context principle
- Infinite scrolling encourages unbounded growth
- Pagination forces chunking - better for comprehension
- Simpler implementation (no viewport offset state)
- Memory bounded (each page is PAGE_SIZE max)

**Decision:** Pagination. If content exceeds page size, create new page. Navigation via #next/#prev or direct links.

This trades continuous navigation for discrete contexts. The tradeoff favors simplicity and boundedness.

### Why Xar Instead of Fixed Buffer?

**Argument for fixed buffer:**
- aquinas pattern (proven in practice)
- Simple - just character array[PAGE_SIZE]
- Predictable memory usage
- No dynamic allocation concerns

**Argument for Xar (chosen):**
- Commands insert text (e.g., $date inserts timestamp)
- Code execution inserts output (unpredictable length)
- Page size should match content, not arbitrary limit
- Xar provides O(1) append, stable indices, no reallocation
- All from piscina - cleanup is free

**Decision:** Xar. The dynamic nature of command execution makes fixed buffers impractical. We'd have to truncate output or fail commands when buffer full.

### Why Single Cursor Instead of Line/Column?

**Argument for line/column:**
- aquinas pattern
- Matches how users think ("line 5, column 10")
- Easier for some operations (move up/down)

**Argument for single index (chosen):**
- Text is fundamentally a byte stream
- Newlines are just characters
- Rendering calculates visual line/col on demand
- Simpler state (one integer vs two)
- Matches Xar's indexed access model

**Decision:** Single cursor index. Convert to line/col when rendering or for specific operations. This matches the underlying data model more directly.

### Tag Syntax Choice

**Alternatives considered:**
1. Markdown-style: `[text](command:arg)`
2. Wiki-style: `[[command:arg|text]]`
3. Plan 9 style: highlight text, right-click, execute
4. Custom: `<tag>content</tag>`

**Decision:** Custom XML-like tags. Reasoning:
- Familiar to anyone who's seen HTML
- Clear nesting (opening/closing tags)
- Extensible (arbitrary tag names)
- Machine-parseable (no ambiguity)
- Self-documenting (tag name indicates purpose)

Plan 9 style is more minimal but requires selection + action (two steps). Tags are one-click.

### Command Prefix Choice

**Alternatives:**
- `$command` (like shell variables)
- `/command` (like IRC/Slack)
- `!command` (like shell history)
- `@command` (like mentions)

**Decision:** `$` for insertion commands, `#` for navigation. Reasoning:
- `$` suggests value/substitution (fits insertion)
- `#` suggests anchor/location (fits navigation)
- Both single-character (no shift key)
- Visually distinct
- Not commonly used in prose

### State Machine vs. Ad-hoc Events

This is the foundational decision. Most UI systems use ad-hoc event handlers:

```javascript
// Typical approach
button.onClick = () => {
    // Mutate global state
    user.loggedIn = true;
    // Trigger side effects
    loadDashboard();
    // Update UI
    button.disabled = true;
}
```

Problems:
- State changes implicit
- Side effects untracked
- Hard to test (global mutations)
- Hard to replay (where's the event log?)
- Hard to reason about (when is button clickable?)

**Our approach:**
```javascript
// Command-based
command login = {
    available: (state) => !state.user.loggedIn,
    execute: (state) => {
        return {
            ...state,
            user: { ...state.user, loggedIn: true },
            view: 'dashboard'
        };
    }
}
```

- State changes explicit (function return)
- Side effects deferred (Sputnik pure functions)
- Easy to test (pure function)
- Easy to replay (commands are data)
- Easy to reason about (availability predicate)

The tradeoff: more upfront structure, less ad-hoc flexibility. We accept this - the structure pays dividends in reliability and comprehensibility.

## V. On Future Extensions

### Command Palette

Not yet implemented, but foundational:

```c
typedef structura PalatiumMandatorum {
    chorda* quaestio;          /* Search query */
    Mandatum** mandata;        /* Filtered commands */
    i32 numerus_mandatorum;
    i32 selectio;              /* Selected index */
} PalatiumMandatorum;

vacuum palatium_monstrare(PalatiumMandatorum* p);
vacuum palatium_quaerere_fuzzy(PalatiumMandatorum* p, chorda* q);
Mandatum* palatium_obtinere_selectum(PalatiumMandatorum* p);
```

Fuzzy search implementation - Sublime Text style substring matching with scoring:
- Match any subsequence of query in command name
- Score by: match position, consecutive characters, word boundaries
- Sort by score, show top N results

Integration:
- Hotkey (Ctrl+P?) opens palette
- Type to search
- Up/down to navigate
- Enter to execute
- Escape to cancel

### Theming System

Constraint: Limited palette enforced across all applications.

```c
typedef structura Thema {
    i32 palette[XVI];          /* 16 colors (RGB values) */

    /* Semantic color indices into palette */
    i32 bg_primarium;          /* 0 = index into palette */
    i32 bg_secundarium;
    i32 textus_primarium;
    i32 textus_secundarium;
    i32 accentus;
    i32 error;
    i32 successus;
    i32 monitio;
} Thema;

externus Thema g_thema_globalis;

i32 obtinere_colorem(i32 index_semanticus);
vacuum ponere_thema(Thema* thema);
```

Applications use semantic indices, not raw RGB. Theme can be swapped at runtime, all UI updates automatically.

Dithering integration:
```c
vacuum delineare_rectangulum_dithered(
    ContextusDelineandi* ctx,
    i32 x, i32 y, i32 w, i32 h,
    i32 color_index_1,
    i32 color_index_2,
    f32 ratio  /* 0.0 = all color1, 1.0 = all color2 */
);
```

Floyd-Steinberg dithering for smooth gradients within limited palette.

### Sputnik Integration Details

The C/Sputnik boundary:

**C calls Sputnik:**
```c
/* Execute Sputnik code, get result */
SputnikValor sputnik_evaluare(character* codex);

/* Convert Sputnik value to string for display */
chorda* sputnik_valor_ad_chordam(SputnikValor v);

/* Register C function callable from Sputnik */
vacuum sputnik_registrare_functionem(
    character* nomen,
    SputnikFunctioNativa fn
);
```

**Sputnik calls C:**
```javascript
// Sputnik code
let result = Native.navigare_ad("page:5");  // Calls C function
let text = Native.pagina_obtinere_textum(0);  // Get page 0 text
```

**Pure function integration:**
```javascript
pure function updateUI(state, action) {
    // Pure logic
    let newState = reducer(state, action);

    // This is impure - automatically deferred
    Native.pagina_inserere_chordam("Result: " + newState.value);

    return newState;  // Pure return
}

// Execute
let {value, effects} = updateUI(state, action);
// value = new state
// effects = [{type: 'native_call', fn: 'pagina_inserere_chordam', args: [...]}]

// Executor decides when to run effects
effects.forEach(e => executeEffect(e));
```

This separates pure computation from side effects. Testing is pure function testing. Effects are data that can be inspected, delayed, batched, or replayed.

### Network Transparency

The command-based architecture enables:

**Server-side execution:**
```
Client → Server: {command: "search", args: {query: "foo"}}
Server → Client: {result: [...], commands: ["view", "edit", "delete"]}
```

**Draw command streaming:**
```
Client → Server: (mouse input, keyboard)
Server → Client: (draw commands)
```

Server renders to draw command buffer, streams to client, client just blits pixels. Thin client model.

**Event log replication:**
```
Primary → Replica: (event stream)
Replica: (replays events, reconstructs state)
```

All state changes are events. Events are serializable. Therefore state is replicable.

## VI. On Testing

### Unit Testing Pages

```c
vacuum probatio_pagina_insertio(vacuum) {
    Piscina* piscina = piscina_generare_dynamicum("test", M);
    Pagina* p = pagina_creare(piscina, "test:0");

    /* Test insertion */
    pagina_inserere_characterem(p, 'a');
    ASSERT(p->longitudo == 1);
    ASSERT(p->cursor == 1);

    pagina_inserere_characterem(p, 'b');
    ASSERT(p->longitudo == 2);

    /* Test mid-insertion */
    pagina_ponere_cursor(p, 1);
    pagina_inserere_characterem(p, 'X');
    ASSERT(p->longitudo == 3);

    /* Verify text is "aXb" */
    character* c0 = xar_obtinere(p->charactera, 0);
    character* c1 = xar_obtinere(p->charactera, 1);
    character* c2 = xar_obtinere(p->charactera, 2);
    ASSERT(*c0 == 'a');
    ASSERT(*c1 == 'X');
    ASSERT(*c2 == 'b');

    piscina_destruere(piscina);
}
```

### Integration Testing Commands

```c
vacuum probatio_mandatum_date(vacuum) {
    Piscina* piscina = piscina_generare_dynamicum("test", M);
    Pagina* p = pagina_creare(piscina, "test:0");

    /* Insert $date command */
    pagina_inserere_chordam(p, "$date");

    /* Simulate click on $date */
    RegioClicca regio = {0};
    invenire_signum_ad(p, 0, &regio);

    ASSERT(regio.validum);
    ASSERT(chorda_aequalis(regio.genus, "command"));
    ASSERT(chorda_aequalis(regio.datum, "$date"));

    /* Execute command */
    i32 longitudo_ante = p->longitudo;
    exsequi_mandatum(&regio);

    /* Verify text was inserted */
    ASSERT(p->longitudo > longitudo_ante);

    piscina_destruere(piscina);
}
```

### Visual Testing

Create probatio_pagina.c - interactive test:
- Shows page with various tags
- Click tags, verify behavior
- Test rendering at 480p
- Verify pagination
- Test multi-page navigation

## VII. On Naming

All Latin names follow established patterns:

**Structures:**
- Pagina (page) - feminine nominative singular
- RegioClicca (click region) - feminine nominative singular
- Mandatum (command) - neuter nominative singular

**Functions:**
- pagina_creare (create page) - infinitive form
- pagina_inserere (insert into page) - infinitive
- pagina_reddere (render page) - infinitive
- navigare_ad (navigate to) - infinitive + preposition
- obtinere_vel_creare (get or create) - compound infinitive

**Variables:**
- charactera (characters) - neuter plural nominative
- longitudo (length) - feminine nominative singular
- cursor (cursor) - masculine nominative singular
- initium (beginning) - neuter nominative singular
- finis (end) - masculine nominative singular

**Constants:**
- VERUM (true) - neuter nominative singular
- FALSUM (false) - neuter nominative singular
- NIHIL (nothing) - neuter nominative singular
- Roman numerals for numbers (I, V, X, L, C, M, etc.)

The language choice is deliberate - Latin as universal language, filtering mechanism, and aesthetic commitment to long-term thinking.

## VIII. Conclusion

This is not a UI framework in the conventional sense. It is a computing environment with principles:

1. **Commands are fundamental** - all actions are commands, commands are first-class, command palette is inevitable
2. **State machines are explicit** - availability predicates, state transitions, no hidden mutations
3. **Text is executable** - tags in text invoke commands, the text itself is the interface
4. **Pagination over scrolling** - bounded contexts, discrete pages, navigation via links
5. **Limited palette** - constraint forces clarity, semantic color use, visual consistency
6. **Network transparent** - commands serialize, state replicates, thin clients possible
7. **Event sourced** - all changes are events, history is queryable, state is reconstructible

The implementation path is clear:
1. Build Pagina widget (text editing, rendering)
2. Add tag detection (parse, detect clicks)
3. Implement multi-page (array, navigation, history)
4. Wire commands (execution, output insertion)
5. Integrate Sputnik (REPL, execution, bindings)

Each phase builds on the previous. Each phase is testable independently. The architecture is simple - no complex frameworks, no magic, just text buffers, tag parsing, and command execution.

The constraint enables the vision. 480p forces focus. Limited colors force intention. No scrolling forces structure. Text-as-interface forces clarity.

Build it piece by piece. Test as you go. Keep it simple. Let the constraints guide you.

*Deus vult.*
