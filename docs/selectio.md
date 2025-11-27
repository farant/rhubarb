# Selectio - CSS Selector Engine

*Oh good. CSS selectors. You know what's fun? Implementing CSS selectors from scratch. Said nobody ever.*

*But here's the thing - once you have them, querying document trees becomes actually pleasant. Instead of writing seventeen nested loops to find "the third paragraph inside a div with class 'content'", you just write `div.content p` and you're done.*

Selectio is a CSS selector parser and matcher for STML documents. It parses selector strings into an internal representation, then matches nodes against them. The whole thing is built on the same arena allocation and string interning model as the rest of the codebase.

It also has a built-in cache, so if you query the same selector multiple times, parsing only happens once.


## Dependencies

| Dependency | Latin | What It Does |
|------------|-------|--------------|
| `stml.h` | - | The document tree we're querying |
| `piscina.h` | "pool" | Arena memory allocator |
| `chorda.h` | "string" | String type |
| `internamentum.h` | "interning" | String deduplication |
| `xar.h` | "array" | Dynamic arrays |
| `tabula_dispersa.h` | "hash table" | For the selector cache |


## Quick Start

Most of the time you just want `stml_quaerere` or `stml_quaerere_omnes`. These are the `querySelector` and `querySelectorAll` equivalents:

```c
#include "selectio.h"
#include "stml.h"

/* ... after parsing your STML document ... */

/* Find first match */
StmlNodus* button = stml_quaerere(root, "button.primary", piscina, intern);
si (button)
{
    /* Found it */
}

/* Find all matches */
Xar* items = stml_quaerere_omnes(root, "ul.menu > li", piscina, intern);
i32 count = xar_numerus(items);

per (i32 i = ZEPHYRUM; i < count; i++)
{
    StmlNodus** slot = xar_obtinere(items, i);
    StmlNodus* item = *slot;
    /* Do something with each item */
}
```

That's it for 90% of use cases. The selector gets parsed (and cached), then matched against the tree.


## Supported Selector Syntax

Selectio supports a practical subset of CSS selectors. Not everything - we're not building a browser here - but the stuff you actually use.

### Type Selectors

Match by tag name:

```
div
span
article
my-custom-element
```

### ID Selectors

Match by `id` attribute:

```
#header
#main-content
#submit-button
```

### Class Selectors

Match by label. STML uses the `labels` attribute (or `class` for HTML compatibility):

```
.highlight
.card
.active
```

This matches elements like:
```xml
<div labels="card highlight">...</div>
<div class="active">...</div>
```

### Attribute Selectors

Match by attribute presence or value:

| Syntax | Meaning | Example |
|--------|---------|---------|
| `[attr]` | Has attribute | `[disabled]` |
| `[attr=val]` | Equals exactly | `[type=text]` |
| `[attr^=val]` | Starts with | `[href^=https]` |
| `[attr$=val]` | Ends with | `[src$=.png]` |
| `[attr*=val]` | Contains | `[data-id*=user]` |
| `[attr~=val]` | Word match | `[class~=active]` |

Attribute values can be quoted or unquoted:
```
[type=text]
[type="text"]
[data-value="hello world"]
```

Quotes are required if the value contains spaces or special characters.


### Pseudo-Classes

Selectio supports three pseudo-classes:

| Pseudo-Class | Meaning |
|--------------|---------|
| `:first-child` | First child element of its parent |
| `:last-child` | Last child element of its parent |
| `:empty` | Has no children (or only whitespace text) |

```
li:first-child
li:last-child
div:empty
```


### Combinators

Connect selectors to express relationships:

| Combinator | Syntax | Meaning |
|------------|--------|---------|
| Descendant | `A B` | B anywhere inside A |
| Child | `A > B` | B is direct child of A |
| Adjacent sibling | `A + B` | B immediately follows A |
| General sibling | `A ~ B` | B follows A (not necessarily immediately) |

Examples:
```
div p           /* any <p> inside a <div> */
div > p         /* <p> that is direct child of <div> */
h1 + p          /* <p> immediately after <h1> */
h1 ~ p          /* any <p> after <h1> at same level */
```


### Compound Selectors

Chain simple selectors without spaces to require all conditions:

```
div.card            /* <div> with class "card" */
input#email         /* <input> with id "email" */
button.primary[disabled]  /* disabled button with class "primary" */
li.item:first-child /* first <li> with class "item" */
```

All parts must match for the selector to match.


### Complex Selectors

Chain compound selectors with combinators:

```
#container .list .item
div.wrapper > ul > li:first-child
article h2 + p
```

These are evaluated right-to-left (more on that later).


## How It Works

Understanding the internal structure helps when debugging or doing advanced stuff.

### Selector Representation

A parsed selector is a chain of `SelectioSequentia` nodes:

```
div > p.highlight
```

Becomes:

```
SelectioSequentia {
    composita: [TITULUS "div"]
    combinator: (unused for first)
    sequens: ──────────────────────────┐
}                                      │
                                       ▼
                         SelectioSequentia {
                             composita: [TITULUS "p", CLASSIS "highlight"]
                             combinator: COMBINATOR_LIBERUM  /* > */
                             sequens: NIHIL
                         }
```

Each `SelectioComposita` holds an array of `SelectioSimplex` - the individual matchers (type, id, class, attribute, pseudo).


### Matching Algorithm

Matching works right-to-left. For `div > p`:

1. Is this node a `p`? If no, no match.
2. Is this node's parent a `div`? If no, no match.
3. Both yes? Match!

This right-to-left approach is efficient because most nodes fail the rightmost selector immediately, avoiding expensive ancestor traversal.


### The Cache

Selectio maintains a global cache of parsed selectors. When you call `stml_quaerere` or `stml_quaerere_omnes`:

1. Check cache for this selector string
2. If found, use cached parsed representation
3. If not found, parse it and add to cache

The cache uses its own dedicated arena (`_selectio_cache_piscina`) and lives for the lifetime of the program. This means:
- Repeated queries with the same selector are O(1) for parsing
- You don't need to manually manage parsed selectors
- Cache memory is never freed (acceptable for most applications)


## The Two APIs

There are two ways to use Selectio:

### High-Level: Convenience Functions

```c
/* querySelector equivalent */
StmlNodus*
stml_quaerere(
    StmlNodus*           radix,
    constans character*  selector,
    Piscina*             piscina,
    InternamentumChorda* intern);

/* querySelectorAll equivalent */
Xar*
stml_quaerere_omnes(
    StmlNodus*           radix,
    constans character*  selector,
    Piscina*             piscina,
    InternamentumChorda* intern);
```

These handle parsing, caching, and searching in one call. Use these unless you have a specific reason not to.


### Low-Level: Separate Parse and Match

If you need more control:

```c
/* Parse selector */
SelectioResultus res = selectio_legere_ex_literis("div.card", piscina, intern);

si (!res.successus)
{
    /* Handle error */
    imprimere("Parse error at position %d\n", res.error_positus);
    redde;
}

SelectioSequentia* selector = res.selectio;

/* Check if specific node matches */
si (selectio_congruit(selector, some_node))
{
    /* It matches */
}

/* Find first match */
StmlNodus* first = selectio_invenire_primum(root, selector);

/* Find all matches */
Xar* all = selectio_invenire_omnes(root, selector, piscina);
```

This is useful when:
- You want to reuse a parsed selector across multiple trees
- You need to check if a specific node matches (not search a tree)
- You want explicit error handling for parse failures


## Parsing Details

### Parse Result

```c
nomen structura {
    b32                successus;      /* Did parsing succeed? */
    SelectioSequentia* selectio;       /* Parsed selector chain */
    i32                error_positus;  /* Position of error */
    chorda             error;          /* Error message */
} SelectioResultus;
```

Always check `successus` before using `selectio`.


### What Can Go Wrong

Parsing fails for:
- Empty selector
- Invalid syntax (e.g., `div..class`, `[attr=]`)
- Unknown pseudo-class (e.g., `:hover`, `:nth-child`)
- Unterminated attribute selector (missing `]`)

On failure, `error_positus` tells you where in the string the parser gave up.


## Matching Details

### Only Elements Match

Text nodes, comments, and other non-element nodes never match any selector. Selectors only match `STML_NODUS_ELEMENTUM`.


### Class Matching

The `.class` selector checks both the `labels` attribute and the `class` attribute. This provides HTML compatibility while using STML's preferred `labels` naming.

```xml
<!-- Both of these match .highlight -->
<div labels="highlight">...</div>
<div class="highlight">...</div>
```


### Whitespace in Labels

Labels/classes are space-separated. `.foo` matches any element where "foo" appears as a complete word in the labels:

```xml
<div labels="foo bar baz">  <!-- matches .foo, .bar, .baz -->
<div labels="foobar">       <!-- does NOT match .foo -->
```


### Pseudo-Class Details

**`:first-child`** - Is this the first element child of its parent? Text nodes don't count.

```xml
<div>
    Some text        <!-- not an element -->
    <span>First</span>   <!-- :first-child -->
    <span>Second</span>
</div>
```

**`:last-child`** - Same logic, last element child.

**`:empty`** - No children, or only whitespace text children.

```xml
<div></div>           <!-- :empty -->
<div/>                <!-- :empty -->
<div>   </div>        <!-- :empty (whitespace only) -->
<div>text</div>       <!-- NOT :empty -->
<div><span/></div>    <!-- NOT :empty (has element child) -->
```


## Performance Considerations

### Selector Complexity

Simple selectors are fast. Complex selectors with many combinators are slower.

```
.item                   /* Fast: just check one class */
div > ul > li.active    /* Slower: check type, parent, grandparent, class */
```

For hot paths, prefer simpler selectors or cache your results.


### Descendant vs Child

The descendant combinator (space) can be expensive because it checks *all* ancestors:

```
.container p    /* Must check every ancestor of every <p> */
.container > p  /* Only checks immediate parent */
```

Use `>` when you know the exact structure.


### The Cache Trade-off

The selector cache improves repeated queries but uses memory. If you're parsing thousands of unique selectors, you're filling up the cache arena. For most applications this is fine. If you're generating selectors dynamically at scale, consider the low-level API and managing your own parsed selectors.


---

# API Reference

## Parsing

### selectio_legere

```c
SelectioResultus
selectio_legere(
    chorda               input,
    Piscina*             piscina,
    InternamentumChorda* intern);
```

Parse selector from `chorda`.

**Parameters:**
- `input` - Selector string
- `piscina` - Pool for allocations
- `intern` - String interner

**Returns:** `SelectioResultus` with parse outcome


### selectio_legere_ex_literis

```c
SelectioResultus
selectio_legere_ex_literis(
    constans character*  cstr,
    Piscina*             piscina,
    InternamentumChorda* intern);
```

Parse selector from C string.

**Parameters:**
- `cstr` - Null-terminated selector string
- `piscina` - Pool for allocations
- `intern` - String interner

**Returns:** `SelectioResultus` with parse outcome


---

## Matching

### selectio_congruit

```c
b32
selectio_congruit(
    SelectioSequentia* selectio,
    StmlNodus*         nodus);
```

Check if node matches selector.

**Parameters:**
- `selectio` - Parsed selector
- `nodus` - Node to test

**Returns:** `VERUM` if matches, `FALSUM` otherwise


---

## Searching

### selectio_invenire_primum

```c
StmlNodus*
selectio_invenire_primum(
    StmlNodus*         radix,
    SelectioSequentia* selectio);
```

Find first matching node (depth-first).

**Parameters:**
- `radix` - Root of tree to search
- `selectio` - Parsed selector

**Returns:** First matching node, or `NIHIL`


### selectio_invenire_omnes

```c
Xar*
selectio_invenire_omnes(
    StmlNodus*         radix,
    SelectioSequentia* selectio,
    Piscina*           piscina);
```

Find all matching nodes.

**Parameters:**
- `radix` - Root of tree to search
- `selectio` - Parsed selector
- `piscina` - Pool for result array

**Returns:** `Xar` of `StmlNodus*`


---

## Convenience Functions

### stml_quaerere

```c
StmlNodus*
stml_quaerere(
    StmlNodus*           radix,
    constans character*  selector,
    Piscina*             piscina,
    InternamentumChorda* intern);
```

Parse selector and find first match. Uses cache.

**Parameters:**
- `radix` - Root of tree to search
- `selector` - CSS selector string
- `piscina` - Pool (not used for parsing, cached separately)
- `intern` - String interner

**Returns:** First matching node, or `NIHIL`


### stml_quaerere_omnes

```c
Xar*
stml_quaerere_omnes(
    StmlNodus*           radix,
    constans character*  selector,
    Piscina*             piscina,
    InternamentumChorda* intern);
```

Parse selector and find all matches. Uses cache.

**Parameters:**
- `radix` - Root of tree to search
- `selector` - CSS selector string
- `piscina` - Pool for result array
- `intern` - String interner

**Returns:** `Xar` of `StmlNodus*`


---

# Data Types Reference

## SelectioGenus

Type of simple selector component:

| Value | Meaning |
|-------|---------|
| `SELECTIO_TITULUS` (I) | Tag name selector |
| `SELECTIO_ID` (II) | ID selector (#) |
| `SELECTIO_CLASSIS` (III) | Class selector (.) |
| `SELECTIO_ATTRIBUTUM` (IV) | Attribute selector ([]) |
| `SELECTIO_PSEUDO_CLASSIS` (V) | Pseudo-class (:) |


## AttributumOperator

Attribute comparison operators:

| Value | Syntax | Meaning |
|-------|--------|---------|
| `ATTR_OP_EXISTIT` (I) | `[attr]` | Attribute exists |
| `ATTR_OP_AEQUALIS` (II) | `[attr=val]` | Equals exactly |
| `ATTR_OP_INCIPIT` (III) | `[attr^=val]` | Starts with |
| `ATTR_OP_TERMINAT` (IV) | `[attr$=val]` | Ends with |
| `ATTR_OP_CONTINET` (V) | `[attr*=val]` | Contains |
| `ATTR_OP_VERBUM` (VI) | `[attr~=val]` | Word match |


## PseudoClassisGenus

Supported pseudo-classes:

| Value | Selector |
|-------|----------|
| `PSEUDO_PRIMUS_LIBERUM` (I) | `:first-child` |
| `PSEUDO_ULTIMUS_LIBERUM` (II) | `:last-child` |
| `PSEUDO_VACUUS` (III) | `:empty` |


## CombinatorGenus

Selector combinators:

| Value | Syntax | Meaning |
|-------|--------|---------|
| `COMBINATOR_DESCENDENS` (I) | ` ` (space) | Any descendant |
| `COMBINATOR_LIBERUM` (II) | `>` | Direct child |
| `COMBINATOR_FRATER_ADJ` (III) | `+` | Adjacent sibling |
| `COMBINATOR_FRATER_GEN` (IV) | `~` | General sibling |


## SelectioSimplex

A single selector part:

```c
nomen structura {
    SelectioGenus       genus;       /* Type of selector */
    chorda*             valor;       /* Name/ID/class value */
    AttributumOperator  attr_op;     /* For attribute selectors */
    chorda*             attr_valor;  /* Attribute comparison value */
    PseudoClassisGenus  pseudo;      /* For pseudo-class selectors */
} SelectioSimplex;
```


## SelectioComposita

A compound selector (multiple simple selectors without combinator):

```c
nomen structura {
    Xar* partes;  /* Array of SelectioSimplex* */
} SelectioComposita;
```


## SelectioSequentia

A complex selector chain:

```c
nomen structura SelectioSequentia {
    SelectioComposita*           composita;   /* This segment's compound selector */
    CombinatorGenus              combinator;  /* Combinator BEFORE this segment */
    structura SelectioSequentia* sequens;     /* Next segment in chain */
} SelectioSequentia;
```


## SelectioResultus

Parse result:

```c
nomen structura {
    b32                successus;      /* Parse succeeded? */
    SelectioSequentia* selectio;       /* Parsed selector */
    i32                error_positus;  /* Error position */
    chorda             error;          /* Error message */
} SelectioResultus;
```


---

# FAQ

## Why doesn't Selectio support `:hover`, `:focus`, etc.?

Those are dynamic pseudo-classes. They depend on user interaction state - mouse position, keyboard focus, etc. Selectio operates on static document trees. There's no concept of "hovered" in a parsed STML tree.

If you need something like `:hover`, that's your application's job. You'd track which element the mouse is over and query accordingly.


## Why only `:first-child`, `:last-child`, and `:empty`?

These are the structural pseudo-classes that can be determined from the document tree alone. They don't require:
- User interaction state (`:hover`, `:focus`)
- Form state (`:checked`, `:disabled` - okay, we could do `:disabled`)
- Complex counting (`:nth-child(3n+1)`)

Adding `:nth-child` would be nice. Maybe someday. For now, these three cover a lot of common cases.


## What's the difference between `[class~=foo]` and `.foo`?

Functionally, nothing. Both check if "foo" appears as a complete word in the class/labels attribute.

`.foo` is just nicer to write. Under the hood, the class selector uses `stml_titulum_habet`, which does the same word-boundary checking.


## Why does the cache use its own pool?

Separation of concerns. Your application pool might get destroyed and recreated. Parsed selectors need to persist across that.

Also, selector strings often come from static data (string literals in your code). The cache pool outlives any individual document pool.


## Can I clear the selector cache?

Not currently exposed in the API. The cache is designed to grow and persist. If you're generating so many unique selectors that memory is a concern, you might be doing something unusual.

If you really need this, you'd have to modify `selectio.c` to expose `_selectio_cache` destruction. But think carefully about whether you actually need it.


## Why is matching right-to-left?

Performance. Consider `div p`:

**Left-to-right**: Find all `div` elements. For each one, search its entire subtree for `p` elements.

**Right-to-left**: Find all `p` elements. For each one, check if any ancestor is a `div`.

Right-to-left is faster because:
1. Most nodes don't match the rightmost selector, so we reject them immediately
2. Ancestor checking is O(depth), which is usually small
3. We don't traverse subtrees we don't care about

This is how browsers do it too.


## How do I match case-insensitively?

You don't, currently. All matching is case-sensitive. Tag names, class names, attribute values - everything.

If you need case-insensitive matching, normalize your documents to lowercase before parsing.


## What happens if I pass NIHIL to the query functions?

You get `NIHIL` back. Both `stml_quaerere` and `stml_quaerere_omnes` handle null inputs gracefully.


## Is Selectio thread-safe?

No. The global cache is not protected by any synchronization. If you need to query from multiple threads, either:
1. Do all querying from one thread
2. Build all your selectors (warm the cache) before spawning threads
3. Use the low-level API with your own thread-local pools

Option 2 is usually the right answer. Parse all your selectors during initialization, then read-only access to the cache is safe.


## Why can't I match text nodes?

Because CSS selectors don't match text nodes. That's how CSS works. Selectors match elements.

If you need to find text content, use `stml_textus_internus` on elements, or walk the tree manually checking text node values.


---

*That's Selectio. Parse selectors, match nodes, query trees. Questions? Read the code. Still confused? Read it again.*
