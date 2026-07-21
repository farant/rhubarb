# XML - Bibliotheca XML Generalis

*You want XML? Fine. Here's XML.*

*Not STML with its fancy capture operators and raw tags and fragments. Not some bloated DOM implementation with XPath and XSLT and seventeen layers of abstraction. Just... XML. The thing you actually need when you're reading a config file or talking to some API from 2003.*

This is a minimal XML parser and serializer. It parses well-formed XML into a tree, lets you query and build trees, and serializes them back out. That's it. No namespaces. No DTD validation. No CDATA sections. If you need those, you're in the wrong place.


## What This Library Does

- Parses XML into a DOM-style tree
- Creates and modifies XML trees programmatically
- Serializes trees back to XML strings (compact or pretty-printed)
- Handles the five standard entity references (`&lt;`, `&gt;`, `&amp;`, `&quot;`, `&apos;`)
- Skips XML declarations (`<?xml ...?>`) and comments

## What This Library Does NOT Do

- Namespaces
- DTD validation or entity definitions
- CDATA sections
- XPath
- Schema validation
- Anything complicated


## Dependencies

| Dependency | Latin | What It Does |
|------------|-------|--------------|
| `piscina.h` | "pool" | Arena memory allocator |
| `chorda.h` | "string" | String type |
| `chorda_aedificator.h` | "string builder" | For serialization |
| `internamentum.h` | "interning" | String deduplication |
| `xar.h` | "array" | Dynamic arrays |
| `latina.h` | - | Latin keyword macros |


## Quick Start

```c
#include "xml.h"

s32 principale(vacuum)
{
    Piscina*             piscina;
    InternamentumChorda* intern;
    XmlResultus          res;

    piscina = piscina_generare_dynamicum("mea_piscina", MMMMXCVI);
    intern = internamentum_creare(piscina);

    /* Parse some XML */
    res = xml_legere_ex_literis(
        "<book id=\"123\">"
        "  <title>The C Programming Language</title>"
        "  <author>Kernighan &amp; Ritchie</author>"
        "</book>",
        piscina, intern);

    si (res.successus)
    {
        chorda* id = xml_attributum_capere(res.radix, "id");
        XmlNodus* title = xml_invenire_liberum(res.radix, "title");
        chorda text = xml_textus_internus(title, piscina);

        /* id->datum = "123" */
        /* text.datum = "The C Programming Language" */
    }

    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
```


## Node Types

XML trees have three kinds of nodes:

| Type | Value | Description |
|------|-------|-------------|
| `XML_NODUS_ELEMENTUM` | I | Element: `<tag>...</tag>` |
| `XML_NODUS_TEXTUS` | II | Text content |
| `XML_NODUS_COMMENTUM` | III | Comment: `<!-- ... -->` |

That's it. No document node. No processing instruction nodes. When you parse, you get back the root *element* directly.


## The Node Structure

```c
nomen structura XmlNodus
{
    XmlNodusGenus        genus;       /* Node type */
    chorda*              titulus;     /* Tag name (elements only) */
    chorda*              valor;       /* Text content */
    Xar*                 attributa;   /* Array of XmlAttributum */
    Xar*                 liberi;      /* Array of XmlNodus* children */
    structura XmlNodus*  parens;      /* Parent pointer */
};
```

For **elements**: `titulus` is the tag name, `attributa` and `liberi` are populated.

For **text nodes**: `valor` is the text content. Everything else is `NIHIL`.

For **comments**: `valor` is the comment content.


## Parsing

Two functions, same result:

```c
/* From a chorda */
XmlResultus
xml_legere(
    chorda               input,
    Piscina*             piscina,
    InternamentumChorda* intern);

/* From a C string */
XmlResultus
xml_legere_ex_literis(
    constans character*  cstr,
    Piscina*             piscina,
    InternamentumChorda* intern);
```

### Parse Result

```c
nomen structura
{
    b32        successus;         /* Did it work? */
    XmlNodus*  radix;             /* Root element */
    XmlStatus  status;            /* Error code */
    i32        linea_erroris;     /* Error line */
    i32        columna_erroris;   /* Error column */
    chorda     error;             /* Error message */
} XmlResultus;
```

Always check `successus` first:

```c
XmlResultus res = xml_legere_ex_literis(input, piscina, intern);

si (!res.successus)
{
    imprimere("Error at line %d, column %d\n",
              res.linea_erroris, res.columna_erroris);
    redde;
}

/* Now safe to use res.radix */
```

### Error Codes

| Status | Value | Meaning |
|--------|-------|---------|
| `XML_SUCCESSUS` | 0 | No error |
| `XML_ERROR_MEMORIA` | I | Out of memory |
| `XML_ERROR_SYNTAXIS` | II | General syntax error |
| `XML_ERROR_TAG_NON_CLAUSUM` | III | Unclosed tag |
| `XML_ERROR_TAG_IMPROPRIE` | IV | Mismatched close tag |
| `XML_ERROR_ATTRIBUTUM` | V | Malformed attribute |
| `XML_ERROR_EFFUGIUM` | VI | Invalid entity reference |
| `XML_ERROR_VACUUM_INPUT` | VII | Empty input |


### What Gets Skipped

The parser ignores:
- XML declarations: `<?xml version="1.0"?>`
- Comments: `<!-- anything -->`

These are skipped silently. If you need to preserve them, use STML instead.


## Entity References

The parser handles the five predefined XML entities:

| Entity | Character |
|--------|-----------|
| `&lt;` | `<` |
| `&gt;` | `>` |
| `&amp;` | `&` |
| `&quot;` | `"` |
| `&apos;` | `'` |

In text content and attribute values, these are decoded automatically:

```c
res = xml_legere_ex_literis("<data>&lt;tag&gt;</data>", piscina, intern);
text = xml_textus_internus(res.radix, piscina);
/* text.datum = "<tag>" */
```

Unknown entities are passed through as-is. No custom entity support.


## Querying

### Finding Children

```c
/* Find first child with tag name */
XmlNodus* child = xml_invenire_liberum(parent, "item");

/* Find all children with tag name */
Xar* items = xml_invenire_omnes_liberos(parent, "item", piscina);
```

These search **direct children only**, not descendants.


### Counting and Indexing

```c
/* How many children? */
i32 count = xml_numerus_liberorum(parent);

/* Get child at index */
XmlNodus* first = xml_liberum_ad_indicem(parent, ZEPHYRUM);
```


### Attributes

```c
/* Get attribute value (or NIHIL if not found) */
chorda* value = xml_attributum_capere(element, "id");

/* Check if attribute exists */
si (xml_attributum_habet(element, "disabled"))
{
    /* It's there */
}
```


### Text Content

```c
/* Get concatenated text of all text children */
chorda text = xml_textus_internus(element, piscina);
```

This only concatenates immediate text children, not text from nested elements.


## Building Trees

### Creating Nodes

```c
/* Create element */
XmlNodus* div = xml_elementum_creare(piscina, intern, "div");

/* Create text node from C string */
XmlNodus* text = xml_textum_creare(piscina, intern, "Hello world");

/* Create text node from chorda */
XmlNodus* text2 = xml_textum_creare_ex_chorda(piscina, intern, some_chorda);

/* Create comment */
XmlNodus* comment = xml_commentum_creare(piscina, intern, " TODO ");
```


### Adding Attributes

```c
/* Add attribute with C string value */
xml_attributum_addere(element, piscina, intern, "id", "123");

/* Add attribute with chorda value */
xml_attributum_addere_chorda(element, piscina, intern, "data", some_chorda);
```


### Building the Tree

```c
/* Add child to element */
xml_liberum_addere(parent, child);

/* Convenience: create and add text in one step */
xml_textum_addere(parent, piscina, intern, "Some text");
```

Example - building a complete tree:

```c
XmlNodus* root = xml_elementum_creare(piscina, intern, "book");
xml_attributum_addere(root, piscina, intern, "isbn", "978-0131103627");

XmlNodus* title = xml_elementum_creare(piscina, intern, "title");
xml_textum_addere(title, piscina, intern, "The C Programming Language");
xml_liberum_addere(root, title);

XmlNodus* author = xml_elementum_creare(piscina, intern, "author");
xml_textum_addere(author, piscina, intern, "Brian W. Kernighan");
xml_liberum_addere(root, author);

/* root is now a complete tree */
```


## Serialization

```c
/* Compact output (no whitespace) */
chorda output = xml_scribere(node, piscina, FALSUM);

/* Pretty-printed (indented, newlines) */
chorda pretty = xml_scribere(node, piscina, VERUM);
```

The `pulchrum` parameter: `VERUM` for pretty, `FALSUM` for compact.

### Escaping

The serializer automatically escapes special characters:

- `<` becomes `&lt;`
- `>` becomes `&gt;`
- `&` becomes `&amp;`
- `"` in attributes becomes `&quot;`

You don't need to pre-escape anything.


### Pretty Printing

With `pulchrum == VERUM`:

```xml
<root>
  <item id="1"/>
  <item id="2"/>
</root>
```

With `pulchrum == FALSUM`:

```xml
<root><item id="1"/><item id="2"/></root>
```

Elements with only text content stay on one line:

```xml
<title>Hello World</title>
```

Not:

```xml
<title>
  Hello World
</title>
```


### Lower-Level Serialization

If you need to serialize to an existing string builder:

```c
b32
xml_scribere_ad_aedificator(
    XmlNodus*          nodus,
    ChordaAedificator* aedificator,
    b32                pulchrum,
    i32                indentatio);
```


## Roundtrip

Parse, serialize, parse again - should give you the same data:

```c
/* Parse */
XmlResultus res1 = xml_legere_ex_literis(original, piscina, intern);

/* Serialize */
chorda serialized = xml_scribere(res1.radix, piscina, FALSUM);

/* Parse again */
XmlResultus res2 = xml_legere(serialized, piscina, intern);

/* res1 and res2 should have equivalent content */
```

Whitespace may not be preserved exactly (the parser normalizes it), but the data will be the same.


---

# API Reference

## Parsing

### xml_legere

```c
XmlResultus
xml_legere(
    chorda               input,
    Piscina*             piscina,
    InternamentumChorda* intern);
```

Parse XML from `chorda`.

**Parameters:**
- `input` - XML source
- `piscina` - Memory pool
- `intern` - String interner

**Returns:** `XmlResultus` with parse outcome


### xml_legere_ex_literis

```c
XmlResultus
xml_legere_ex_literis(
    constans character*  cstr,
    Piscina*             piscina,
    InternamentumChorda* intern);
```

Parse XML from C string.

**Parameters:**
- `cstr` - Null-terminated XML
- `piscina` - Memory pool
- `intern` - String interner

**Returns:** `XmlResultus` with parse outcome


---

## Querying

### xml_invenire_liberum

```c
XmlNodus*
xml_invenire_liberum(
    XmlNodus*           nodus,
    constans character* titulus);
```

Find first child with tag name.

**Parameters:**
- `nodus` - Parent element
- `titulus` - Tag name to find

**Returns:** First matching child, or `NIHIL`


### xml_invenire_omnes_liberos

```c
Xar*
xml_invenire_omnes_liberos(
    XmlNodus*           nodus,
    constans character* titulus,
    Piscina*            piscina);
```

Find all children with tag name.

**Parameters:**
- `nodus` - Parent element
- `titulus` - Tag name to find
- `piscina` - Pool for result array

**Returns:** `Xar` of `XmlNodus*`


### xml_attributum_capere

```c
chorda*
xml_attributum_capere(
    XmlNodus*           nodus,
    constans character* titulus);
```

Get attribute value by name.

**Parameters:**
- `nodus` - Element node
- `titulus` - Attribute name

**Returns:** Pointer to value, or `NIHIL`


### xml_attributum_habet

```c
b32
xml_attributum_habet(
    XmlNodus*           nodus,
    constans character* titulus);
```

Check if attribute exists.

**Parameters:**
- `nodus` - Element node
- `titulus` - Attribute name

**Returns:** `VERUM` if present


### xml_textus_internus

```c
chorda
xml_textus_internus(
    XmlNodus* nodus,
    Piscina*  piscina);
```

Get concatenated text content.

**Parameters:**
- `nodus` - Element node
- `piscina` - Pool for result

**Returns:** Concatenated text


### xml_numerus_liberorum

```c
i32
xml_numerus_liberorum(
    XmlNodus* nodus);
```

Get child count.

**Parameters:**
- `nodus` - Parent node

**Returns:** Number of children


### xml_liberum_ad_indicem

```c
XmlNodus*
xml_liberum_ad_indicem(
    XmlNodus* nodus,
    i32       index);
```

Get child at index.

**Parameters:**
- `nodus` - Parent node
- `index` - Zero-based index

**Returns:** Child node, or `NIHIL`


---

## Construction

### xml_elementum_creare

```c
XmlNodus*
xml_elementum_creare(
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  titulus);
```

Create element node.

**Parameters:**
- `piscina` - Memory pool
- `intern` - String interner
- `titulus` - Tag name

**Returns:** New element


### xml_textum_creare

```c
XmlNodus*
xml_textum_creare(
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  textus);
```

Create text node from C string.

**Parameters:**
- `piscina` - Memory pool
- `intern` - String interner
- `textus` - Text content

**Returns:** New text node


### xml_textum_creare_ex_chorda

```c
XmlNodus*
xml_textum_creare_ex_chorda(
    Piscina*             piscina,
    InternamentumChorda* intern,
    chorda               textus);
```

Create text node from `chorda`.

**Parameters:**
- `piscina` - Memory pool
- `intern` - String interner
- `textus` - Text content

**Returns:** New text node


### xml_commentum_creare

```c
XmlNodus*
xml_commentum_creare(
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  textus);
```

Create comment node.

**Parameters:**
- `piscina` - Memory pool
- `intern` - String interner
- `textus` - Comment content

**Returns:** New comment node


### xml_attributum_addere

```c
b32
xml_attributum_addere(
    XmlNodus*            nodus,
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


### xml_attributum_addere_chorda

```c
b32
xml_attributum_addere_chorda(
    XmlNodus*            nodus,
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


### xml_liberum_addere

```c
b32
xml_liberum_addere(
    XmlNodus* parens,
    XmlNodus* liberum);
```

Add child to element.

**Parameters:**
- `parens` - Parent element
- `liberum` - Child to add

**Returns:** `VERUM` on success


### xml_textum_addere

```c
b32
xml_textum_addere(
    XmlNodus*            parens,
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  textus);
```

Create and add text child in one step.

**Parameters:**
- `parens` - Parent element
- `piscina` - Memory pool
- `intern` - String interner
- `textus` - Text content

**Returns:** `VERUM` on success


---

## Serialization

### xml_scribere

```c
chorda
xml_scribere(
    XmlNodus* nodus,
    Piscina*  piscina,
    b32       pulchrum);
```

Serialize node to string.

**Parameters:**
- `nodus` - Node to serialize
- `piscina` - Pool for result
- `pulchrum` - `VERUM` for pretty-print

**Returns:** XML string


### xml_scribere_ad_aedificator

```c
b32
xml_scribere_ad_aedificator(
    XmlNodus*          nodus,
    ChordaAedificator* aedificator,
    b32                pulchrum,
    i32                indentatio);
```

Serialize to existing string builder.

**Parameters:**
- `nodus` - Node to serialize
- `aedificator` - String builder
- `pulchrum` - Pretty-print flag
- `indentatio` - Current indent level

**Returns:** `VERUM` on success


---

# FAQ

## When should I use XML vs STML?

Use **XML** when:
- You're reading/writing standard XML files
- You need to interoperate with other XML tools
- Simplicity is more important than features

Use **STML** when:
- You want boolean attributes (`<button disabled>`)
- You want raw content tags (`<code!>...</code>`)
- You want capture operators or fragments
- You're building HTML-like documents


## Why no namespaces?

Because namespaces are complicated and most people don't need them. If you're parsing `xmlns`-heavy XML, you'll see the namespace prefixes as part of the tag and attribute names. That's usually good enough.

If you need real namespace support, you need a real XML library. This isn't that.


## Why no CDATA?

Same reason. CDATA is another way to include literal text without escaping. This library just uses entity escaping. It's simpler and handles all the same cases.


## Why no DTD validation?

Because DTD is a rabbit hole. You start with "validate against a schema" and end up implementing half of SGML. This library parses well-formed XML. Validity is your problem.


## Why does xml_textus_internus only get direct text children?

Because that's the common case. If you have:

```xml
<p>Hello <b>world</b>!</p>
```

And you call `xml_textus_internus` on the `<p>`, you get "Hello !" - the text nodes that are direct children.

If you want "Hello world!" (including text from nested elements), walk the tree yourself. It's not hard:

```c
interior vacuum
collect_all_text(XmlNodus* nodus, ChordaAedificator* aed)
{
    si (nodus->genus == XML_NODUS_TEXTUS && nodus->valor)
    {
        chorda_aedificator_appendere_chorda(aed, *nodus->valor);
    }
    alioquin si (nodus->genus == XML_NODUS_ELEMENTUM && nodus->liberi)
    {
        i32 i;
        per (i = ZEPHYRUM; i < xml_numerus_liberorum(nodus); i++)
        {
            collect_all_text(xml_liberum_ad_indicem(nodus, i), aed);
        }
    }
}
```


## What happens with unknown entities?

They're passed through as-is. If you have `&foo;` in your XML, it stays `&foo;` after parsing. No error, no transformation.


## Is this thread-safe?

No. Each parser uses its pool and interner. Don't share those across threads. If you need parallel parsing, give each thread its own pool.


## Can I modify a parsed tree?

Yes, but carefully. You can add children with `xml_liberum_addere`. There's no `xml_removere` - if you need tree manipulation, use STML instead, which has a full set of mutation operations.


## Why doesn't XmlResultus have elementum_radix like StmlResultus?

Because XML doesn't have the same concept of "stuff before the root element" that needs to be preserved. The XML parser skips declarations and comments before the root. When you parse, `radix` *is* the root element.

STML is different - it wraps everything in a document node because it preserves processing instructions and might have multiple roots.


## How big can documents be?

Limited by pool memory. The parser doesn't stream - it builds the entire tree in memory. For huge documents (gigabytes), this isn't the right tool. For normal documents (megabytes), you're fine.


---

*That's XML. Simple. Boring. Gets the job done. Next.*
