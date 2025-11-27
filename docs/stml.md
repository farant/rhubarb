# STML - Structured Text Markup Language

*Look, you want to parse XML-like stuff? Of course you do. Everyone does. And then they go download some 50,000 line dependency with seventeen transitive dependencies and wonder why their build takes four hours and breaks every third Tuesday.*

*This ain't that.*

STML is an XML superset parser written in C89. It handles everything XML does, plus a few things XML doesn't. It's got boolean attributes like HTML5. It's got raw content tags so you don't have to escape everything. It's got these things called capture operators that... look, we'll get to those.

The whole thing runs on arena allocation (Piscina) and string interning (InternamentumChorda). No malloc soup. No fragmented heap. You parse a document, you're done, you dump the arena, memory's gone. Clean.


## Dependencies

Before we go any further: this library depends on other libraries from this codebase. I'm not gonna explain them in detail here - that's what *their* documentation is for - but you need to know they exist:

| Dependency | Latin | What It Does |
|------------|-------|--------------|
| `piscina.h` | "pool" | Arena memory allocator. All STML allocations come from here. |
| `chorda.h` | "string" | String type. Length-prefixed, not null-terminated internally. |
| `internamentum.h` | "interning" | String interning. Same string = same pointer. |
| `xar.h` | "array" | Dynamic array. Used for children, attributes, etc. |
| `latina.h` | - | Latin keyword macros (`si` for `if`, `redde` for `return`, etc.) |

Yeah. The code is in Latin. `si` means `if`. `redde` means `return`. `dum` means `while`. You'll get used to it. Or you won't. Either way, it's what we're working with.


## Quick Start

Here's the bare minimum to parse something:

```c
#include "stml.h"

s32 principale(vacuum)
{
    Piscina*             piscina;
    InternamentumChorda* intern;
    StmlResultus         res;

    /* Create memory pool (4096 bytes initial) */
    piscina = piscina_generare_dynamicum("mea_piscina", MMMMXCVI);
    si (!piscina)
    {
        redde I;
    }

    /* Create string interner */
    intern = internamentum_creare(piscina);
    si (!intern)
    {
        piscina_destruere(piscina);
        redde I;
    }

    /* Parse some STML */
    res = stml_legere_ex_literis("<greeting>Hello</greeting>", piscina, intern);

    si (res.successus)
    {
        /* res.elementum_radix is your root element */
        chorda textus = stml_textus_internus(res.elementum_radix, piscina);
        /* textus now contains "Hello" */
    }

    /* Clean up everything at once */
    piscina_destruere(piscina);

    redde ZEPHYRUM;
}
```

That's it. Pool in, pool out. Everything allocated during parsing lives in that pool. When the pool goes, it all goes.


## The Node Structure

Every piece of an STML document is a node. Here's what that looks like:

```c
nomen structura StmlNodus
{
    StmlNodusGenus           genus;           /* What kind of node */
    chorda*                  titulus;         /* Tag name (elements) */
    chorda*                  valor;           /* Text content / comment / etc */
    Xar*                     attributa;       /* Array of StmlAttributum */
    Xar*                     liberi;          /* Array of StmlNodus* (children) */
    structura StmlNodus*     parens;          /* Parent node */

    /* STML extensions */
    b32                      crudus;          /* Is this a raw tag? */
    StmlCaptioDirectio       captio_directio; /* Capture operator direction */
    i32                      captio_numerus;  /* How many parens in capture */
    b32                      fragmentum;      /* Is this a fragment? */
    chorda*                  fragmentum_id;   /* Fragment ID (or NIHIL) */
};
```

Let's break that down:

**genus** - The node type. One of:
- `STML_NODUS_DOCUMENTUM` (I) - Root document node
- `STML_NODUS_ELEMENTUM` (II) - Regular element `<tag>`
- `STML_NODUS_TEXTUS` (III) - Text content
- `STML_NODUS_COMMENTUM` (IV) - Comment `<!-- -->`
- `STML_NODUS_PROCESSIO` (V) - Processing instruction `<?xml ?>`
- `STML_NODUS_DOCTYPE` (VI) - DOCTYPE declaration
- `STML_NODUS_TRANSCLUSIO` (VII) - Transclusion `<<selector>>`

**titulus** - For elements, this is the tag name. Pointer to an interned string, so you can compare with `==` if you want (but there are helper functions).

**valor** - For text nodes, this is the text. For comments, the comment content. For transclusions, the selector. You get the idea.

**attributa** - A `Xar` (dynamic array) of `StmlAttributum` structs. Each attribute has a `titulus` (name) and `valor` (value). Both are interned strings.

**liberi** - Children. A `Xar` of `StmlNodus*`. Elements and the document node have children. Text nodes don't.

**parens** - Parent pointer. Goes the other way up the tree. The root document node has `NIHIL` here.


## Node Types In Detail

### Document Node (STML_NODUS_DOCUMENTUM)

When you parse something, you get back an `StmlResultus` with two important pointers:

```c
nomen structura
{
    b32         successus;        /* Did parsing succeed? */
    StmlNodus*  radix;            /* Root DOCUMENT node */
    StmlNodus*  elementum_radix;  /* First ELEMENT child */
    StmlStatus  status;           /* Error code if failed */
    i32         linea_erroris;    /* Error line number */
    i32         columna_erroris;  /* Error column */
    chorda      error;            /* Error message */
} StmlResultus;
```

The `radix` is the document node - it's a container that holds everything. The `elementum_radix` is a convenience pointer to the first actual element, because 90% of the time that's what you want.

Why have both? Because a document might have comments or processing instructions before the first element:

```xml
<?xml version="1.0"?>
<!-- A comment -->
<root>...</root>
```

The document node's children would be: PI, comment, element. The `elementum_radix` skips straight to `<root>`.


### Element Node (STML_NODUS_ELEMENTUM)

Your basic tag. Has a name, can have attributes, can have children.

```xml
<div class="container">
    <p>Text here</p>
</div>
```

The `<div>` is an element. Its `titulus` is "div". It has one attribute (class="container"). It has one child (the `<p>` element).


### Text Node (STML_NODUS_TEXTUS)

Just text. No children. No attributes. The `valor` field holds the content.

```xml
<p>Hello world</p>
```

That `<p>` element has one child: a text node with `valor` = "Hello world".

**Important**: STML does smart whitespace handling. Leading/trailing whitespace gets trimmed, multiple spaces collapse. If you don't want that, use raw tags (more on that later).


### Comment Node (STML_NODUS_COMMENTUM)

```xml
<!-- This is a comment -->
```

The `valor` field holds " This is a comment " (the stuff between `<!--` and `-->`).


### Processing Instruction (STML_NODUS_PROCESSIO)

```xml
<?xml version="1.0" encoding="UTF-8"?>
```

The `valor` field holds "xml version=\"1.0\" encoding=\"UTF-8\"".


### DOCTYPE (STML_NODUS_DOCTYPE)

```xml
<!DOCTYPE html>
```

The `valor` field holds "html".


### Transclusion (STML_NODUS_TRANSCLUSIO)

This is an STML extension. More on it later in the document.

```xml
<<#sidebar>>
```

The `valor` field holds "#sidebar".


## Parsing

Two functions to parse STML:

```c
/* From a chorda (length-prefixed string) */
StmlResultus
stml_legere(
    chorda               input,
    Piscina*             piscina,
    InternamentumChorda* intern);

/* From a C string (null-terminated) */
StmlResultus
stml_legere_ex_literis(
    constans character*  cstr,
    Piscina*             piscina,
    InternamentumChorda* intern);
```

The second one is usually more convenient. Same result either way.

Always check `res.successus` before using the result:

```c
StmlResultus res;

res = stml_legere_ex_literis("<root><child/></root>", piscina, intern);

si (!res.successus)
{
    imprimere("Parse error at line %d, column %d: %s\n",
              res.linea_erroris,
              res.columna_erroris,
              res.error.datum ? res.error.datum : "unknown");
    redde I;
}

/* Now safe to use res.radix and res.elementum_radix */
```

### Error Codes

If parsing fails, `res.status` tells you why:

| Status | Value | Meaning |
|--------|-------|---------|
| `STML_SUCCESSUS` | 0 | No error |
| `STML_ERROR_MEMORIA` | I | Out of memory |
| `STML_ERROR_SYNTAXIS` | II | General syntax error |
| `STML_ERROR_TAG_NON_CLAUSUM` | III | Unclosed tag at EOF |
| `STML_ERROR_TAG_IMPROPRIE` | IV | Mismatched close tag |
| `STML_ERROR_ATTRIBUTUM` | V | Malformed attribute |
| `STML_ERROR_VACUUM_INPUT` | VI | Empty input |
| `STML_ERROR_CAPTIO` | VII | Capture operator error |


## Attributes

### Reading Attributes

Two ways to check for attributes:

```c
/* Get the value (returns NIHIL if not found) */
chorda* valor = stml_attributum_capere(nodus, "id");

si (valor)
{
    /* Use valor->datum, valor->mensura */
}

/* Just check if it exists (good for boolean attrs) */
si (stml_attributum_habet(nodus, "disabled"))
{
    /* It's there */
}
```

### Boolean Attributes

STML supports HTML5-style boolean attributes:

```xml
<button disabled type="submit">Click</button>
```

That `disabled` has no value. When you call `stml_attributum_capere(nodus, "disabled")`, you get back a pointer to the interned string "true".

The `stml_attributum_habet` function is cleaner for this:

```c
si (stml_attributum_habet(button, "disabled"))
{
    /* Button is disabled */
}
```

### Setting Attributes

When building trees programmatically:

```c
StmlNodus* elem = stml_elementum_creare(piscina, intern, "button");

/* Regular attribute */
stml_attributum_addere(elem, piscina, intern, "type", "submit");

/* Boolean attribute */
stml_attributum_boolean_addere(elem, piscina, intern, "disabled");

/* Attribute with chorda value (not C string) */
stml_attributum_addere_chorda(elem, piscina, intern, "data", some_chorda);
```


## Querying the Tree

### Finding Children

```c
/* Find first child with tag name */
StmlNodus* child = stml_invenire_liberum(parent, "item");

/* Find ALL children with tag name */
Xar* items = stml_invenire_omnes_liberos(parent, "item", piscina);

per (i32 i = ZEPHYRUM; i < xar_numerus(items); i++)
{
    StmlNodus** slot = xar_obtinere(items, i);
    StmlNodus* item = *slot;
    /* Do something with item */
}
```

### Counting and Indexing

```c
/* How many children? */
i32 count = stml_numerus_liberorum(parent);

/* Get child at index */
StmlNodus* first = stml_liberum_ad_indicem(parent, ZEPHYRUM);
StmlNodus* third = stml_liberum_ad_indicem(parent, II);
```

### Getting Text Content

```c
/* Get all text inside an element (concatenated) */
chorda text = stml_textus_internus(element, piscina);
```

This walks the tree and concatenates all text nodes. For:

```xml
<p>Hello <b>world</b>!</p>
```

`stml_textus_internus` on the `<p>` returns "Hello world!".


## Tree Navigation

STML gives you the full DOM-style navigation API:

### Siblings

```c
/* Next sibling (or NIHIL) */
StmlNodus* next = stml_frater_proximus(nodus);

/* Previous sibling (or NIHIL) */
StmlNodus* prev = stml_frater_prior(nodus);

/* All siblings except self */
Xar* siblings = stml_fratres(nodus, piscina);

/* Where am I among my siblings? (-1 if no parent) */
s32 index = stml_index_inter_fratres(nodus);
```

### Children (shortcuts)

```c
/* First child */
StmlNodus* first = stml_primus_liberum(parent);

/* Last child */
StmlNodus* last = stml_ultimus_liberum(parent);
```

### Ancestors

```c
/* Get all ancestors (parent, grandparent, ..., root) */
Xar* ancestors = stml_maiores(nodus, piscina);

/* Find closest ancestor matching a selector */
StmlNodus* form = stml_proximus_maior(nodus, "form", piscina, intern);
```

The `stml_proximus_maior` function walks up the tree until it finds an element matching the selector. It includes the starting node in the search, so if `nodus` itself matches, it returns `nodus`.


## Tree Manipulation

### Adding Children

```c
/* Append child (at end) */
stml_liberum_addere(parent, child);
/* or */
stml_appendere(parent, child);  /* Same thing, alias */

/* Prepend child (at beginning) */
stml_praeponere(parent, child, piscina);
```

### Inserting

```c
/* Insert before a node */
stml_inserere_ante(existing_node, new_node, piscina);

/* Insert after a node */
stml_inserere_post(existing_node, new_node, piscina);
```

### Removing

```c
/* Remove node from its parent */
stml_removere(node, piscina);

/* Remove all children from a node */
stml_vacare_liberos(parent);

/* Replace one node with another */
stml_substituere(old_node, new_node, piscina);
```

**Note**: "Removing" a node doesn't free memory. The node still exists in the pool. It's just no longer part of the tree. This is fine - when you destroy the pool, everything goes.


## Cloning

```c
/* Deep clone (copies everything, including children) */
StmlNodus* copy = stml_duplicare(original, piscina, intern);

/* Shallow clone (just the node, no children) */
StmlNodus* shallow = stml_duplicare_superficialiter(original, piscina, intern);
```


## Labels (CSS Classes)

STML has built-in support for space-separated label lists, like CSS classes:

```xml
<div labels="card highlight featured">...</div>
```

Or for HTML compatibility:

```xml
<div class="card highlight featured">...</div>
```

Both work. The API:

```c
/* Check if node has a label */
si (stml_titulum_habet(nodus, "highlight"))
{
    /* It's highlighted */
}

/* How many labels? */
i32 count = stml_titulos_numerus(nodus);

/* Get all labels as array */
Xar* labels = stml_titulos_capere(nodus, piscina);

/* Add a label (returns FALSUM if already exists) */
stml_titulum_addere(nodus, piscina, intern, "new-label");

/* Remove a label (returns FALSUM if not found) */
stml_titulum_removere(nodus, piscina, intern, "old-label");

/* Toggle (add if missing, remove if present) */
/* Returns VERUM if now has it, FALSUM if now doesn't */
b32 now_has_it = stml_titulum_commutare(nodus, piscina, intern, "active");
```


## Creating Nodes Programmatically

You don't have to parse strings. You can build trees by hand:

```c
/* Create an element */
StmlNodus* div = stml_elementum_creare(piscina, intern, "div");

/* Create a text node */
StmlNodus* text = stml_textum_creare(piscina, intern, "Hello world");

/* Or from a chorda */
StmlNodus* text2 = stml_textum_creare_ex_chorda(piscina, intern, some_chorda);

/* Create a comment */
StmlNodus* comment = stml_commentum_creare(piscina, intern, " TODO: fix this ");

/* Create a raw content element */
StmlNodus* code = stml_elementum_crudum_creare(piscina, intern, "code");

/* Build the tree */
stml_liberum_addere(div, text);
stml_attributum_addere(div, piscina, intern, "class", "greeting");
```

There's also a convenience function:

```c
/* Add text directly to an element */
stml_textum_addere(parent, piscina, intern, "Some text");
```


## Serialization

Turn your tree back into a string:

```c
/* Compact output */
chorda output = stml_scribere(nodus, piscina, FALSUM);

/* Pretty-printed with indentation */
chorda pretty = stml_scribere(nodus, piscina, VERUM);
```

The `pulchrum` parameter (second boolean): `VERUM` for pretty, `FALSUM` for compact.

There's also a lower-level function that writes to a `ChordaAedificator`:

```c
ChordaAedificator* builder = chorda_aedificator_creare(piscina, CCLVI);

stml_scribere_ad_aedificator(nodus, builder, FALSUM, ZEPHYRUM);

chorda result = chorda_aedificator_consumere(builder);
```


---

# STML Extensions

Okay, now we get to the good stuff. The things that make STML not just "XML but in Latin."


## Raw Content Tags

*You know what I hate? Entity escaping. You want to put some code in your document and suddenly everything is `&lt;` this and `&amp;` that and you can't read anything anymore.*

Raw tags solve this. Add a `!` after the tag name:

```xml
<code!>
function example() {
    if (x < y && z > w) {
        return true;
    }
}
</code>
```

Everything inside is literal. The `<`, `>`, and `&` are just characters. No entity parsing. No child elements. Just raw text until the closing tag.

When you serialize a raw tag, it comes back with the `!`:

```c
res = stml_legere_ex_literis("<code!><div>&amp;</div></code>", piscina, intern);

/* The element has crudus == VERUM */
si (res.elementum_radix->crudus)
{
    /* It's a raw tag */
}

/* Serialize it back */
chorda output = stml_scribere(res.elementum_radix, piscina, FALSUM);
/* output is "<code!><div>&amp;</div></code>" */
```

The content is preserved exactly. That `&amp;` stays `&amp;`, not `&`.


## Capture Operators

*Okay. This is where it gets weird. Stay with me.*

Capture operators let you reparent sibling nodes without writing deeply nested markup. There are three kinds:

### Forward Capture: `<tag (>`

Captures nodes that come *after* it:

```xml
<root>
    <wrapper (>
    <item/>
    <item/>
</root>
```

Parses as if you wrote:

```xml
<root>
    <wrapper>
        <item/>
        <item/>
    </wrapper>
</root>
```

The `(>` says "capture my following siblings as my children."

You can use multiple parens for nested capture:

```xml
<root>
    <outer ((>
    <inner (>
    <content/>
</root>
```

Two parens = capture two levels deep.


### Backward Capture: `<) tag>`

Captures nodes that come *before* it:

```xml
<root>
    <item/>
    <item/>
    <) wrapper>
</root>
```

Parses as:

```xml
<root>
    <wrapper>
        <item/>
        <item/>
    </wrapper>
</root>
```

Same idea, opposite direction. The `<)` at the front means "capture my preceding siblings."


### Sandwich Capture: `<= tag =>`

Captures nodes on *both* sides:

```xml
<root>
    <before/>
    <= wrapper =>
    <after/>
</root>
```

Parses as:

```xml
<root>
    <wrapper>
        <before/>
        <after/>
    </wrapper>
</root>
```

### Why Would You Use This?

Good question. Here's a real use case:

Without capture operators:
```xml
<article>
    <section class="prose">
        <p>First paragraph.</p>
        <p>Second paragraph.</p>
        <p>Third paragraph.</p>
    </section>
</article>
```

With capture operators:
```xml
<article>
    <section class="prose" (>
    <p>First paragraph.</p>
    <p>Second paragraph.</p>
    <p>Third paragraph.</p>
</article>
```

Less nesting. Flatter structure. Easier to read when you have lots of content.

The node's `captio_directio` field tells you which kind:
- `STML_CAPTIO_NIHIL` (0) - No capture
- `STML_CAPTIO_ANTE` (I) - Forward `(>`
- `STML_CAPTIO_RETRO` (II) - Backward `<)`
- `STML_CAPTIO_FARCIMEN` (III) - Sandwich `<= =>`

And `captio_numerus` tells you how many parens (for nested captures).


## Fragments

Fragments are invisible grouping containers. Like React's `<>...</>` or `<Fragment>`.

```xml
<#>
    <item/>
    <item/>
</#>
```

That's an anonymous fragment. It groups the items but doesn't add an element to the output.

Named fragments have an ID:

```xml
<#sidebar>
    <nav>...</nav>
</#>
```

Self-closing fragments:

```xml
<#placeholder/>
```

On the node, `fragmentum == VERUM` for fragments. `fragmentum_id` is the ID (or `NIHIL` for anonymous).

The closing tag is always `</#>`, regardless of whether the opening had an ID.


## Transclusion

Transclusion is content inclusion by reference:

```xml
<page>
    <#header>
        <h1>My Site</h1>
    </#>

    <main>
        Content here
    </main>

    <footer>
        <<#header>>
    </footer>
</page>
```

The `<<#header>>` says "insert the content of the fragment with ID 'header' here."

This is just a *reference* at parse time. STML doesn't actually do the substitution - that's for your application layer. The parser just gives you a node of type `STML_NODUS_TRANSCLUSIO` with the selector in `valor`.

Selectors can be anything:
- `<<#id>>` - by fragment ID
- `<<.class>>` - by class/label
- `<<tag>>` - by tag name
- `<<complex > selector>>` - whatever CSS-like thing you want

The parser doesn't interpret the selector. It just stores it. Your code decides what to do with it.


---

# API Reference

## Parsing

### stml_legere

```c
StmlResultus
stml_legere(
    chorda               input,
    Piscina*             piscina,
    InternamentumChorda* intern);
```

Parse STML from a `chorda` (length-prefixed string).

**Parameters:**
- `input` - The STML source text
- `piscina` - Memory pool for all allocations
- `intern` - String interner for deduplication

**Returns:** `StmlResultus` with parse results


### stml_legere_ex_literis

```c
StmlResultus
stml_legere_ex_literis(
    constans character*  cstr,
    Piscina*             piscina,
    InternamentumChorda* intern);
```

Parse STML from a null-terminated C string.

**Parameters:**
- `cstr` - The STML source text (null-terminated)
- `piscina` - Memory pool
- `intern` - String interner

**Returns:** `StmlResultus` with parse results


---

## Querying

### stml_invenire_liberum

```c
StmlNodus*
stml_invenire_liberum(
    StmlNodus*           nodus,
    constans character*  titulus);
```

Find the first child element with the given tag name.

**Parameters:**
- `nodus` - Parent node to search
- `titulus` - Tag name to find

**Returns:** First matching child, or `NIHIL` if not found


### stml_invenire_omnes_liberos

```c
Xar*
stml_invenire_omnes_liberos(
    StmlNodus*           nodus,
    constans character*  titulus,
    Piscina*             piscina);
```

Find all children with the given tag name.

**Parameters:**
- `nodus` - Parent node to search
- `titulus` - Tag name to find
- `piscina` - Pool for allocating result array

**Returns:** `Xar` of `StmlNodus*` pointers


### stml_attributum_capere

```c
chorda*
stml_attributum_capere(
    StmlNodus*           nodus,
    constans character*  titulus);
```

Get an attribute value by name.

**Parameters:**
- `nodus` - Element node
- `titulus` - Attribute name

**Returns:** Pointer to value string, or `NIHIL` if not found


### stml_attributum_habet

```c
b32
stml_attributum_habet(
    StmlNodus*           nodus,
    constans character*  titulus);
```

Check if node has an attribute.

**Parameters:**
- `nodus` - Element node
- `titulus` - Attribute name

**Returns:** `VERUM` if present, `FALSUM` otherwise


### stml_textus_internus

```c
chorda
stml_textus_internus(
    StmlNodus* nodus,
    Piscina*   piscina);
```

Get concatenated text content of node and all descendants.

**Parameters:**
- `nodus` - Node to extract text from
- `piscina` - Pool for result string

**Returns:** Concatenated text as `chorda`


### stml_numerus_liberorum

```c
i32
stml_numerus_liberorum(
    StmlNodus* nodus);
```

Get number of child nodes.

**Parameters:**
- `nodus` - Parent node

**Returns:** Child count (0 if no children)


### stml_liberum_ad_indicem

```c
StmlNodus*
stml_liberum_ad_indicem(
    StmlNodus* nodus,
    i32        index);
```

Get child at specific index.

**Parameters:**
- `nodus` - Parent node
- `index` - Zero-based index

**Returns:** Child node, or `NIHIL` if out of bounds


---

## Navigation

### stml_index_inter_fratres

```c
s32
stml_index_inter_fratres(
    StmlNodus* nodus);
```

Get index of node among its siblings.

**Parameters:**
- `nodus` - Node to find

**Returns:** Zero-based index, or -1 if no parent


### stml_frater_proximus

```c
StmlNodus*
stml_frater_proximus(
    StmlNodus* nodus);
```

Get next sibling.

**Parameters:**
- `nodus` - Current node

**Returns:** Next sibling, or `NIHIL`


### stml_frater_prior

```c
StmlNodus*
stml_frater_prior(
    StmlNodus* nodus);
```

Get previous sibling.

**Parameters:**
- `nodus` - Current node

**Returns:** Previous sibling, or `NIHIL`


### stml_primus_liberum

```c
StmlNodus*
stml_primus_liberum(
    StmlNodus* nodus);
```

Get first child.

**Parameters:**
- `nodus` - Parent node

**Returns:** First child, or `NIHIL`


### stml_ultimus_liberum

```c
StmlNodus*
stml_ultimus_liberum(
    StmlNodus* nodus);
```

Get last child.

**Parameters:**
- `nodus` - Parent node

**Returns:** Last child, or `NIHIL`


### stml_fratres

```c
Xar*
stml_fratres(
    StmlNodus* nodus,
    Piscina*   piscina);
```

Get all siblings (excluding self).

**Parameters:**
- `nodus` - Current node
- `piscina` - Pool for result array

**Returns:** `Xar` of `StmlNodus*`


### stml_maiores

```c
Xar*
stml_maiores(
    StmlNodus* nodus,
    Piscina*   piscina);
```

Get all ancestors (parent to root).

**Parameters:**
- `nodus` - Starting node
- `piscina` - Pool for result array

**Returns:** `Xar` of `StmlNodus*` from parent to root


### stml_proximus_maior

```c
StmlNodus*
stml_proximus_maior(
    StmlNodus*           nodus,
    constans character*  selector,
    Piscina*             piscina,
    InternamentumChorda* intern);
```

Find closest ancestor matching selector (includes self).

**Parameters:**
- `nodus` - Starting node
- `selector` - Tag name or selector to match
- `piscina` - Pool for any allocations
- `intern` - String interner

**Returns:** Matching ancestor, or `NIHIL`


---

## Construction

### stml_elementum_creare

```c
StmlNodus*
stml_elementum_creare(
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  titulus);
```

Create a new element node.

**Parameters:**
- `piscina` - Memory pool
- `intern` - String interner
- `titulus` - Tag name

**Returns:** New element node


### stml_elementum_crudum_creare

```c
StmlNodus*
stml_elementum_crudum_creare(
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  titulus);
```

Create a raw content element (the `!` kind).

**Parameters:**
- `piscina` - Memory pool
- `intern` - String interner
- `titulus` - Tag name

**Returns:** New raw element with `crudus == VERUM`


### stml_textum_creare

```c
StmlNodus*
stml_textum_creare(
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  textus);
```

Create a text node from C string.

**Parameters:**
- `piscina` - Memory pool
- `intern` - String interner
- `textus` - Text content

**Returns:** New text node


### stml_textum_creare_ex_chorda

```c
StmlNodus*
stml_textum_creare_ex_chorda(
    Piscina*             piscina,
    InternamentumChorda* intern,
    chorda               textus);
```

Create a text node from `chorda`.

**Parameters:**
- `piscina` - Memory pool
- `intern` - String interner
- `textus` - Text content as `chorda`

**Returns:** New text node


### stml_commentum_creare

```c
StmlNodus*
stml_commentum_creare(
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  textus);
```

Create a comment node.

**Parameters:**
- `piscina` - Memory pool
- `intern` - String interner
- `textus` - Comment content

**Returns:** New comment node


### stml_attributum_addere

```c
b32
stml_attributum_addere(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  titulus,
    constans character*  valor);
```

Add attribute to element.

**Parameters:**
- `nodus` - Element node
- `piscina` - Memory pool
- `intern` - String interner
- `titulus` - Attribute name
- `valor` - Attribute value

**Returns:** `VERUM` on success


### stml_attributum_boolean_addere

```c
b32
stml_attributum_boolean_addere(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  titulus);
```

Add boolean attribute (value becomes "true").

**Parameters:**
- `nodus` - Element node
- `piscina` - Memory pool
- `intern` - String interner
- `titulus` - Attribute name

**Returns:** `VERUM` on success


### stml_attributum_addere_chorda

```c
b32
stml_attributum_addere_chorda(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  titulus,
    chorda               valor);
```

Add attribute with `chorda` value.

**Parameters:**
- `nodus` - Element node
- `piscina` - Memory pool
- `intern` - String interner
- `titulus` - Attribute name
- `valor` - Attribute value as `chorda`

**Returns:** `VERUM` on success


### stml_liberum_addere

```c
b32
stml_liberum_addere(
    StmlNodus* parens,
    StmlNodus* liberum);
```

Append child to element.

**Parameters:**
- `parens` - Parent element
- `liberum` - Child node to add

**Returns:** `VERUM` on success


### stml_textum_addere

```c
b32
stml_textum_addere(
    StmlNodus*           parens,
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  textus);
```

Create and append text node in one step.

**Parameters:**
- `parens` - Parent element
- `piscina` - Memory pool
- `intern` - String interner
- `textus` - Text content

**Returns:** `VERUM` on success


---

## Serialization

### stml_scribere

```c
chorda
stml_scribere(
    StmlNodus* nodus,
    Piscina*   piscina,
    b32        pulchrum);
```

Serialize node to string.

**Parameters:**
- `nodus` - Node to serialize
- `piscina` - Pool for result string
- `pulchrum` - `VERUM` for pretty-print, `FALSUM` for compact

**Returns:** Serialized STML as `chorda`


### stml_scribere_ad_aedificator

```c
b32
stml_scribere_ad_aedificator(
    StmlNodus*          nodus,
    ChordaAedificator*  aedificator,
    b32                 pulchrum,
    i32                 indentatio);
```

Serialize node to existing string builder.

**Parameters:**
- `nodus` - Node to serialize
- `aedificator` - String builder to write to
- `pulchrum` - Pretty-print flag
- `indentatio` - Current indentation level

**Returns:** `VERUM` on success


---

## Labels

### stml_titulum_habet

```c
b32
stml_titulum_habet(
    StmlNodus*          nodus,
    constans character* titulum);
```

Check if node has label.

**Parameters:**
- `nodus` - Element node
- `titulum` - Label to check

**Returns:** `VERUM` if present


### stml_titulos_numerus

```c
i32
stml_titulos_numerus(
    StmlNodus* nodus);
```

Count labels on node.

**Parameters:**
- `nodus` - Element node

**Returns:** Number of labels (0 if none)


### stml_titulos_capere

```c
Xar*
stml_titulos_capere(
    StmlNodus* nodus,
    Piscina*   piscina);
```

Get all labels as array.

**Parameters:**
- `nodus` - Element node
- `piscina` - Pool for result

**Returns:** `Xar` of `chorda`


### stml_titulum_addere

```c
b32
stml_titulum_addere(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  titulum);
```

Add label to node.

**Parameters:**
- `nodus` - Element node
- `piscina` - Memory pool
- `intern` - String interner
- `titulum` - Label to add

**Returns:** `VERUM` if added, `FALSUM` if already existed


### stml_titulum_removere

```c
b32
stml_titulum_removere(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  titulum);
```

Remove label from node.

**Parameters:**
- `nodus` - Element node
- `piscina` - Memory pool
- `intern` - String interner
- `titulum` - Label to remove

**Returns:** `VERUM` if removed, `FALSUM` if not found


### stml_titulum_commutare

```c
b32
stml_titulum_commutare(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  titulum);
```

Toggle label on node.

**Parameters:**
- `nodus` - Element node
- `piscina` - Memory pool
- `intern` - String interner
- `titulum` - Label to toggle

**Returns:** `VERUM` if node now has label, `FALSUM` if now doesn't


---

## Manipulation

### stml_praeponere

```c
b32
stml_praeponere(
    StmlNodus* parens,
    StmlNodus* liberum,
    Piscina*   piscina);
```

Prepend child to element.

**Parameters:**
- `parens` - Parent element
- `liberum` - Child to prepend
- `piscina` - Memory pool

**Returns:** `VERUM` on success


### stml_inserere_ante

```c
b32
stml_inserere_ante(
    StmlNodus* nodus,
    StmlNodus* novum,
    Piscina*   piscina);
```

Insert node before another.

**Parameters:**
- `nodus` - Reference node
- `novum` - Node to insert
- `piscina` - Memory pool

**Returns:** `VERUM` on success


### stml_inserere_post

```c
b32
stml_inserere_post(
    StmlNodus* nodus,
    StmlNodus* novum,
    Piscina*   piscina);
```

Insert node after another.

**Parameters:**
- `nodus` - Reference node
- `novum` - Node to insert
- `piscina` - Memory pool

**Returns:** `VERUM` on success


### stml_removere

```c
b32
stml_removere(
    StmlNodus* nodus,
    Piscina*   piscina);
```

Remove node from parent.

**Parameters:**
- `nodus` - Node to remove
- `piscina` - Memory pool

**Returns:** `VERUM` on success


### stml_vacare_liberos

```c
vacuum
stml_vacare_liberos(
    StmlNodus* nodus);
```

Remove all children.

**Parameters:**
- `nodus` - Parent node


### stml_substituere

```c
b32
stml_substituere(
    StmlNodus* vetus,
    StmlNodus* novum,
    Piscina*   piscina);
```

Replace one node with another.

**Parameters:**
- `vetus` - Node to replace
- `novum` - Replacement node
- `piscina` - Memory pool

**Returns:** `VERUM` on success


---

## Cloning

### stml_duplicare

```c
StmlNodus*
stml_duplicare(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern);
```

Deep clone node (including children).

**Parameters:**
- `nodus` - Node to clone
- `piscina` - Pool for new nodes
- `intern` - String interner

**Returns:** Cloned node


### stml_duplicare_superficialiter

```c
StmlNodus*
stml_duplicare_superficialiter(
    StmlNodus*           nodus,
    Piscina*             piscina,
    InternamentumChorda* intern);
```

Shallow clone (node only, no children).

**Parameters:**
- `nodus` - Node to clone
- `piscina` - Pool for new node
- `intern` - String interner

**Returns:** Cloned node without children


---

# FAQ

## Why arena allocation instead of malloc/free?

*Oh boy, here we go.*

Look, malloc/free is fine for some things. But for a parser? You allocate hundreds of little objects - strings, nodes, attribute arrays - and then what? You have to track all of them. Reference counting? Garbage collection? A nightmare.

With arenas: allocate, allocate, allocate. Done? Destroy the arena. One call. Everything gone. No leaks. No use-after-free. No fragmentation.

Also: locality. All your nodes are in contiguous memory. Cache loves that. Cache is your friend.


## Why string interning?

Two reasons:

1. **Memory**: If your document has 500 `<div>` tags, you don't need 500 copies of the string "div". You need one.

2. **Comparison**: When strings are interned, pointer equality means string equality. `a == b` instead of `strcmp(a, b)`. Much faster.


## Why is the closing tag for fragments always `</#>` regardless of the ID?

Design choice. It's simpler. You don't have to match IDs. The parser just looks for `</#>` and closes the most recent open fragment.

Also mirrors how fragments work in JSX: `<>...</>` doesn't have an ID on the close either.


## Why doesn't STML support entity references?

Because raw tags exist. If you need literal `<` or `&`, use a raw tag:

```xml
<code!>if (x < 10 && y > 5)</code>
```

Entities are a pain. They're verbose, hard to read, and easy to mess up. Raw tags are cleaner.

STML *does* unescape the basic entities (`&lt;`, `&gt;`, `&amp;`, `&quot;`, `&apos;`) in regular content when parsing. But you can't define new ones, and raw tags ignore them entirely.


## What's the difference between `radix` and `elementum_radix` in StmlResultus?

`radix` is the document node - the invisible root that contains everything.

`elementum_radix` is the first actual element child of the document.

For:
```xml
<?xml version="1.0"?>
<root>...</root>
```

`radix->liberi` = [PI node, element node]
`elementum_radix` = the element node directly

99% of the time you want `elementum_radix`.


## How do capture operators actually work?

The parser first builds a flat list of siblings, then rearranges them based on capture operators.

Forward capture `<tag (>`: Take all following siblings, make them children of this tag.

Backward capture `<) tag>`: Take all preceding siblings, make them children of this tag.

Sandwich `<= tag =>`: Take siblings from both sides.

The `captio_numerus` field says how many levels deep. `((>` captures two levels (children and grandchildren).


## Why doesn't transclusion actually substitute content?

Separation of concerns. The parser's job is parsing. It gives you a tree with transclusion nodes in it.

*Your* job is deciding what transclusion means in your application. Maybe you look up fragments by ID. Maybe you do something else. The parser doesn't assume.


## Can I modify a parsed tree?

Yes. All the manipulation functions (`stml_liberum_addere`, `stml_removere`, etc.) work on any nodes, whether parsed or created programmatically.

But remember: "removing" doesn't free memory. The node still exists in the pool. That's fine - just don't expect removed nodes to disappear until you destroy the pool.


## What happens if parsing fails?

You get an `StmlResultus` with `successus == FALSUM`. Check `status` for the error type, `linea_erroris` and `columna_erroris` for location, `error` for a message.

The `radix` and `elementum_radix` will be `NIHIL`. Don't use them.


## Is STML thread-safe?

No. Each parser instance uses its own pool and interner. Don't share those across threads. If you need to parse in multiple threads, each thread gets its own pool.


## Can I use STML for HTML?

Mostly. STML is stricter than HTML (tags must close properly), but it handles:
- Boolean attributes: `<input disabled>`
- Unquoted attributes: not supported (use quotes)
- Self-closing tags: `<br/>` works

For messy real-world HTML, you'd want an actual HTML parser that handles tag soup. STML assumes well-formed input.


## What's the maximum document size?

Limited by available memory and the pool size. The pool grows dynamically, but at some point you'll run out. For huge documents, consider streaming parsers instead.

For normal documents (megabytes, not gigabytes), you're fine.


---

*Alright, that's the STML docs. All of 'em. You got parsing, querying, manipulation, serialization, all the extensions, API reference, FAQ. If there's something else you need, ask. But read this first.*
