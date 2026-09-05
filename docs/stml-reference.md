# STML: reference

Ordered for lookup, not for learning. The tutorial
(`docs/stml-tutorial.md`) teaches; this document states. Every
construct, every rule, every refusal, as implemented in `lib/stml.c`,
`lib/stml_macros.c`, `lib/stml_html.c`, `lib/canon.c` and
`lib/selectio.c` on 2026-09-05. Where the specs and the code
disagree, the code was measured and is what is written here.

Contents

1. Layers and tools
2. Lexical grammar
3. Document structure and node kinds
4. Elements and closing
5. Attributes
6. Text, entities, and the four text kinds
7. Whitespace: the trivia model and the accessors
8. Captures
9. Fragments and transclusion
10. Dotted tags, augmentation, and the three name spaces
11. Comments, processing instructions, DOCTYPE
12. Parser leniency and `stml_strictum`
13. The writer: fidelity and pretty
14. The chain: legere, expandere, distribuere, emit
15. Templates
16. COMMUTATIO
17. Slot projections and PER inside bodies
18. DISTRIBUTIO
19. EXEMPLAR, captures, TRANSPARENTIA, PER
20. CATENA
21. DIRIBITIO
22. SINE
23. INDAGO
24. Expansion vitia (I to XXXI)
25. HTML emission (`vertere`)
26. Canon
27. Selectors
28. Command-line tools
29. C API map
30. Reserved, dead, and absent forms
31. Traps

---

## 1. Layers and tools

STML is one syntax carrying three layers, each a pure function from
a tree to a tree:

| layer | what it does | engine | tool verb |
|---|---|---|---|
| document | text ⇄ tree, byte-faithful | `lib/stml.c` | `stml formare` |
| expansion | templates filled, patterns run, lists distributed | `lib/stml_macros.c` | `stml expandere` |
| emission | tree → HTML | `lib/stml_html.c` | `stml vertere` |

Beside them: **canon** (`lib/canon.c`, `bin/canon_examen`) judges a
document against a schema written in STML; **selectio**
(`lib/selectio.c`) matches CSS-style selectors against a tree.

House law that shapes everything below (from `project-specs/stml-visio.md` §8):
files are truth and every other view is a projection; there is no
expression language in the data plane; every reference failure is
loud; non-pretty writing is byte-exact; reservations are written
down even when unbuilt.

---

## 2. Lexical grammar

### 2.1 Characters

| class | characters |
|---|---|
| name start | `A-Z a-z _ :` |
| name continue | name start plus `0-9 - .` |
| tag name | one optional leading `.`, then name start, then name continue; or a run of `*` alone (`*`, `**`) |
| attribute name | name start, then name continue; never a leading `.`; may begin with `$` when followed by a name start (pattern capture, §19) |
| attribute-element name | `@` + attribute name + `=` (§5.3) |
| fragment id | optional leading `@`, then `A-Z a-z 0-9 _ -` |
| bare attribute value | `A-Z a-z 0-9 _ -` |
| whitespace | space, tab, LF, CR |

`:` in names is accepted for XML compatibility and carries no
meaning. A tag name starting illegally (`<9x>`, `<..x>`, `<!x>`) is
`STML_ERROR_SYNTAXIS`.

### 2.2 Token forms

The lexer dispatches on the bytes after `<`, in this order:

| form | token | section |
|---|---|---|
| `<<...>>` | transclusion | §9.2 |
| `<!--...-->` | comment | §11 |
| `<?...?>` | processing instruction | §11 |
| `<!D...>` | DOCTYPE (uppercase `D` required) | §11 |
| `</#>` | fragment close | §9.1 |
| `</%>` | augmentation close | §10.2 |
| `<#...` | fragment open, `<#>` `<#id>` `<#id/>` `<#id (>` | §9.1 |
| `<% &k;>` | augmentation open | §10.2 |
| `<(>` `<((>` | anonymous capturing fragment | §8.4 |
| `<>` | anonymous fragment (sugar for `<#>`) | §9.1 |
| `<tag ...>` `<tag/>` `</tag>` `</>` `<tag (>` `<) tag>` `<= tag =>` `<tag!>` `<tag\>` `<@a=>` | element family | §4, §5.3, §6, §8 |
| anything else | text | §6 |

Sigils glued to the tag name: `!` (raw), `\` (multiline), `!\` (both,
that order only; `\!` is a syntax error). Capture parens follow the
name and attributes, glued (`<tag(>`) or after a space
(`<tag attr="v" (>`).

### 2.3 Attribute values

| form | example | value stored |
|---|---|---|
| double-quoted | `a="x y"` | the bytes between the quotes, raw |
| bare | `a=xy` | the run of `A-Z a-z 0-9 _ -` |
| boolean | `a` | the interned string `true` |
| empty | `a=""` | empty value (see §5.4) |

Single-quoted values (`a='x'`) are refused: `STML_ERROR_ATTRIBUTUM`.
A `"` cannot occur inside an inline value; use an attribute-element
(§5.3). Whitespace around `=` is normalized away. Newlines between
attributes are allowed and kept as tag-interior trivia (§7.5).

### 2.4 Input canonicalization

CRLF is converted to LF once, over the whole input, before lexing.
`StmlResultus.crlf_canonicalizatum` reports that it happened. A lone
`\r` not followed by `\n` is content. `\r` is therefore not
representable in STML text; consumers needing it encode it
out-of-band.

---

## 3. Document structure and node kinds

A parse yields a **document node** (`STML_NODUS_DOCUMENTUM`) whose
children are the top-level nodes in file order. `elementum_radix` is
the first element child. Node kinds:

| `StmlNodusGenus` | produced by |
|---|---|
| `DOCUMENTUM` (I) | the parse |
| `ELEMENTUM` (II) | elements, and also fragments, augmentations, attribute-elements (see §3.1) |
| `TEXTUS` (III) | text runs |
| `COMMENTUM` (IV) | `<!-- -->` |
| `PROCESSIO` (V) | `<?...?>` |
| `DOCTYPE` (VI) | `<!DOCTYPE ...>` |
| `TRANSCLUSIO` (VII) | `<<...>>` |

### 3.1 Elements that are not elements

Three constructs carry `genus == ELEMENTUM` with a marker field.
Consumers must test the field, not the title string:

| construct | marker | `titulus` |
|---|---|---|
| fragment | `fragmentum == VERUM`, `fragmentum_id` | `#` |
| augmentation | `augmentum_clavis != NIHIL` | `%` |
| attribute-element | `attributum_titulus != NIHIL` | `@` |

`crudus`, `multilinea`, `captio_directio`, `clausura_anonyma` mark
real elements.

### 3.2 Positions

`linea` (1-based line of the node's start) and
`positus_initium`/`positus_finis` (byte extent `[initium, finis)`
from the opening tag through the closing tag) are parse metadata.
The writer never reads them, so they cannot leak into output. Both
are zero on nodes built by hand or produced by expansion (§15.7).

---

## 4. Elements and closing

| form | meaning |
|---|---|
| `<t>...</t>` | element with children |
| `<t/>` | empty element |
| `<t>...</>` | anonymous close: closes the nearest open element; the authored form is preserved on write |
| `<t (>` etc. | capture forms, no closing tag (§8) |

Rules:

- A named close must match the open tag exactly:
  `STML_ERROR_TAG_IMPROPRIE` otherwise. A stray `</>` with nothing
  open is the same error.
- Unterminated is an error for all five construct kinds (element,
  raw, comment, PI, DOCTYPE): `STML_ERROR_TAG_NON_CLAUSUM`.
- `</>` is legal on elements and fragments and on `\` elements. Raw
  (`!`, `!\`) elements require the named close; `</>` inside raw
  content is text.
- The writer collapses an empty pair to the self-closing form:
  `<a></a>` writes as `<a/>`.

---

## 5. Attributes

### 5.1 Inline attributes

`StmlAttributum {titulus, valor, spatia_ante}`; both strings
interned; values raw both ways (no entity processing). Duplicate
names both survive the parse; `stml_attributum_capere` returns the
first (`stml_strictum` reports the duplicate, §12).

### 5.2 Boolean attributes

A bare name stores the interned value `true`. `<x a>` and
`<x a="true">` produce identical trees; the distinction is lost at
parse time. `stml_attributum_habet` answers presence.

### 5.3 Attribute-elements `<@name=>...</>`

Any element may spell an attribute as a child element. The tag is
`@` + name + `=`; the value is the element's content.

**Position law**, judged after captures have been processed:

| position | binding | allowed content |
|---|---|---|
| a prefix of the parent's child list (before any ordinary child) | the parent's attribute | text only; element children are a vitium |
| immediately following a template call `<<#@f>>` (a maximal run) | the call's argument | anything, including subtrees (§15.3) |
| anywhere else | refused: `elementum attributi extra praefixum liberorum nec post vocationem templi (par. 6.3)` | |

A same-line space before the first attribute-element makes a text
node, so the attribute-element is no longer a prefix; write
`<x><@a=>v</>` with nothing between.

The same name given both inline and as an element is the
duplicate-attribute vitium. `!` and `\` are refused on
attribute-elements. Capture arity counts an attribute-element as an
ordinary child.

Fidelity: the attribute-element stays a child node with the marker;
it is never folded into `attributa`. `stml_attributum_capere` looks
in `attributa` first, then scans the child prefix. The two spellings
are equivalent in meaning, not bytes: inline is raw, element content
follows text law (entities decoded).

### 5.4 Empty and absent

| spelling | meaning |
|---|---|
| `a=""` | present, empty value (`valor == NIHIL`) |
| `<@a=/>` or `<@a=></>` | **tombstone**: explicitly absent. `capere` returns `NIHIL`; canon judges it missing (but still checks the name); attribute inheritance (§18) is blocked by it |
| nothing | absent |

The pretty writer prints both `a=""` and `a="true"` as the bare
`a`; on re-read a bare attribute is `true`. An empty-string value
therefore does not survive formatting. Consumers that must
distinguish empty from `true` (HTML's `alt=""`) read the tree, not
the reformatted file.

### 5.5 Labels

`labels="a b c"` (or `class=`) is a space-separated label set with an
API: `stml_titulum_habet`, `stml_titulos_capere`,
`stml_titulum_addere`/`_removere`/`_commutare`. Selectors' `.x`
matches it (§27). No namespaces (`ui::button`) in C.

---

## 6. Text, entities, and the four text kinds

### 6.1 Entities

Decoded in **text only**: `&lt;` `&gt;` `&amp;` `&quot;` `&apos;`.
Any other `&...;` (named, `&#10;`, `&#x41;`) is literal text. A lone
`&` is literal. The writer escapes `<`, `>`, `&` in text and nothing
else; `"` is written bare. Attribute values are raw in both
directions. Raw content (`!`) is untouched in both directions.

Consequences: `&nbsp;` and `&amp;nbsp;` produce the same tree; a
bare `<` in text is a syntax error (`&lt;` is required); `&@x;`
outside a template body round-trips as `&amp;@x;`.

### 6.2 The kind ladder

| form | line breaks | tags and entities | flag on node |
|---|---|---|---|
| `<t>` | soft (one space) | live | none |
| `<t\>` | content, dedented at parse | live; child elements refused (reserved) | `multilinea`, `indentatio` |
| `<t!>` | content | dead | `crudus` |
| `<t!\>` | content, dedented | dead | both |

`\` semantics: the first newline after the tag is clipped; the
common indentation of the remaining non-empty lines is removed and
stored in `indentatio`; relative indentation survives; content on
the tag line itself is legal and excluded from the dedent
computation; the trailing newline-bearing whitespace exits entirely,
so `x</>`, `x\n</>` and `x\n  </>` have the same value. Dedent
compares bytes, not columns (a tab and a space share no prefix).

Refusals, `STML_ERROR_MULTILINEA` with the cause in `result.error`:

| cause | message |
|---|---|
| interior line that is whitespace but not empty | `linea interior spatio sola nec vacua in '<tag\>' (reassemblatio praefixi ambigua fieret)` |
| child element inside | `liberi inlineares in '<tag\>' v2 reservati (textus solus licet)` |
| capture parens on a `\` tag | named refusal |
| `\!` order | `STML_ERROR_SYNTAXIS` |

With attributes the marker sits between name and attributes:
`<t\ a="v">`.

Raw semantics: the lexer enters raw mode at `<t!>` and scans for
`</t`; nothing inside is a tag, entity, fragment or capture. A raw
element with a capture (§8.3) does not enter raw mode.

---

## 7. Whitespace: the trivia model and the accessors

### 7.1 The rule

A whitespace run that contains a newline is **layout** (trivia); a
whitespace run on one line is **content**. Newline-bearing
whitespace-only runs produce no node; same-line whitespace-only runs
are text nodes (`<sep>   </sep>` keeps its value). A text run's
leading or trailing edge leaves the value iff it contains a newline.

### 7.2 Ownership

Every trivia byte has one owner:

| field | on | holds |
|---|---|---|
| `spatia_ante` | any node | whitespace before its first byte |
| `spatia_post` | any node | whitespace after its last byte, through the first newline inclusive |
| `spatia_clausurae` | element | whitespace between the last child and the closing tag; on the document, the file tail |
| `spatia_intra_tagum` | element | whitespace before `>` or `/>` |
| `spatia_ante` | attribute | whitespace before this attribute (NIHIL = one space) |

Between two nodes the run splits at the first newline inclusive:
that much is the prior's `post`, the rest the next's `ante`. After
an open tag the first child takes it all. After a capture-form open
tag, same-line whitespace belongs to the captor's `post` and is
never content.

### 7.3 Accessors

| function | returns |
|---|---|
| `stml_textus_valor` | the meaning: concatenated text values, no trivia, format-stable |
| `stml_textus_fluxus` | the prose reading: children's text joined with one space wherever a newline-bearing boundary sat; edge trivia contributes nothing; consecutive soft boundaries merge; same-line runs stay literal; `\` and `!` content returned untouched |
| `stml_textus_internus` | the bytes: exact reassembly of ante + value + clausurae + post, recursively (layout-sensitive by design; the migration bridge) |
| `stml_textus_normalizatus` | `internus` then dedent and trim (the pre-model reading; still what hand-written STML readers want) |

### 7.4 Comments

Comments stay nodes (content with structure), carry trivia like any
node, and are transparent to capture counting (§8.2).

### 7.5 Tag-interior trivia

Modeled and replayed byte-exact by the fidelity writer; regenerated
by the pretty writer. Whitespace around `=` is the one normalized
exception.

---

## 8. Captures

### 8.1 Forms

| form | direction | takes |
|---|---|---|
| `<t (>` `<t((>` `<t(((>` | forward (`STML_CAPTIO_ANTE`) | the next N siblings as children |
| `<) t>` `<)) t>` | backward (`RETRO`) | the previous N siblings |
| `<= t =>` | sandwich (`FARCIMEN`) | one previous and one next |

`captio_directio` and `captio_numerus` record the authored form.
Captures are resolved during parsing (`_processare_captiones`);
children come out in document order for all three directions, so a
consumer walking `liberi` sees plain nesting. Chained forward
captures work (`<a(> <b(> x`).

### 8.2 Counting

Siblings are counted after the trivia model: newline-bearing
whitespace is not a node and comments are skipped (but preserved in
place). A same-line space after `(>` is a separator. Attribute-
elements count as children. Same-line corners for multi-paren and
backward captures are not addressed (M4, reserved).

### 8.3 Raw line capture `<t! (>`

A raw tag with capture parens takes the rest of the line, raw, up to
but excluding the newline, leading and trailing spaces kept. It has
no closing tag. `((>` is accepted and still captures one line. The
newline becomes the captor's `spatia_post`.

### 8.4 Capturing fragments

`<#id (>`, `<# (>`, and the sugar `<(>` capture forward like
elements. Backward and sandwich forms do not exist for fragments.
The writer normalizes `<(>` to `<#(>`.

### 8.5 Pretty and captures

The pretty writer re-derives capture form from the tree (§13.2).
Authored bytes are not preserved through pretty for captures; the
fixed point is the law.

---

## 9. Fragments and transclusion

### 9.1 Fragments

| form | meaning |
|---|---|
| `<#>...</#>` | anonymous fragment |
| `<>...</>` | same (sugar; writer emits `<#>...</#>`) |
| `<#id>...</#>` | named fragment; `fragmentum_id = "id"` |
| `<#id/>` `<#/>` | empty |
| `<#id a="v">` | fragments take attributes (used for slot declarations, §15) |

The closing tag never carries the id: `</#nomen>` is not a closer,
and the parser will preserve it as garbage without complaint. `</>`
also closes a fragment.

Semantics: a fragment groups children without an element of its own.
**Anonymous fragments under a named parent are DISTRIBUTIO items**
(§18); an anonymous fragment beside a named sibling is refused when
the chain runs. Anonymous fragments at document level or inside a
named fragment pass through.

Template-space fragments (`<#@id>`) are §15.

### 9.2 Transclusion `<<...>>`

`<<selector>>` yields a `TRANSCLUSIO` node whose `valor` is the
interior. The scan skips quoted spans, so `>>` inside quotes is safe.
Round-trips byte-exact.

**Resolution is the consumer's.** The library carries the node and
resolves nothing. Known consumers: silva's arbor (shared-lexeme
identity, `<<#lexN>>`), canon (judges through content transclusions
transparently, refuses orphans and cycles), the macro engine (only
`<<#@...>>`, §15). `vertere` refuses any transclusion. There is no
selector-based resolver in C.

Transclusion is aliasing: one object, many windows. Instantiation
(a fresh copy per site) is the template call.

---

## 10. Dotted tags, augmentation, and the three name spaces

### 10.1 Name spaces

| sigil | space | lives in | example |
|---|---|---|---|
| `#x` | this document | fragment ids, transclusions, anchors | `<#intro>`, `<<#intro>>` |
| `&x;` | the world: an individual | attribute values | `<canis nomen="&laika;">` |
| `.x` | kinds (vocabulary) | attribute values and tag names | `genus=".canis"`, `<.species>` |
| `&@x;` | template slot | template bodies only | §15 |
| `$x` | pattern capture | pattern bodies only | §19 |

`&x;` needs the `;` in every position; that is what keeps `AT&T`
literal. Canon judges `&x;` and `.x` (§26.6).

### 10.2 Dotted tags `<.species>`

One leading dot is part of the tag name. Attributes never take a
leading dot. Consequences: selectors read a leading `.` as a class,
so dotted elements are not selector-addressable; canon's `titulus`
genus mirrors the rule.

### 10.3 Augmentation `<% &clavis;> ... </%>`

Adds content to a library individual named by the sigiled key. Node
is `ELEMENTUM` with `augmentum_clavis` set to the key verbatim
(including `&` and `;`). Strict at birth: `<%>`, `<% laika>`,
`<% .canis>`, `<% &;>`, `</%x>` are syntax errors; unclosed is
`TAG_NON_CLAUSUM`. No attributes on `<%`. One lawful verb: append
(additive only; canon refuses augmentation of single-valued members,
§26.7).

---

## 11. Comments, processing instructions, DOCTYPE

| form | node | notes |
|---|---|---|
| `<!-- ... -->` | `COMMENTUM` | content raw; extra dashes are content; unterminated = `TAG_NON_CLAUSUM` |
| `<?xml ...?>` | `PROCESSIO` | interior raw; unterminated = `TAG_NON_CLAUSUM`; `vertere` refuses it |
| `<!DOCTYPE ...>` | `DOCTYPE` | the `D` must be uppercase; `<!doctype ...>` is `STML_ERROR_SYNTAXIS`; `vertere` ignores it and writes its own |

Any other `<!x` is a syntax error.

---

## 12. Parser leniency and `stml_strictum`

The parser is deliberately lenient; `stml_strictum(radix, piscina)`
returns the well-formedness faults a caller may choose to enforce:

| `StmlStrictumGenus` | message | why it matters |
|---|---|---|
| `ATTRIBUTUM_DUPLICATUM` | `attributum idem bis (primum vincit in lectione, ...)` | the file says one value, every reader uses the other |
| `RADICES_PLURES` | `elementa radicis plura - reliqua tacite ...` | everything after the first root is ignored by `elementum_radix` readers |
| `TEXTUS_EXTRA_RADICEM` | `textus extra elementum radicis` | |
| `TITULUS_VACUUS` | `elementum sine nomine` | `< >` parses as an unnamed element |

Accepted on purpose, not faults: bare attribute values, unknown
entities, boolean attributes, anonymous fragments.

Parse statuses (`StmlStatus`): `SUCCESSUS`, `ERROR_MEMORIA`,
`ERROR_SYNTAXIS`, `ERROR_TAG_NON_CLAUSUM`, `ERROR_TAG_IMPROPRIE`,
`ERROR_ATTRIBUTUM`, `ERROR_VACUUM_INPUT`, `ERROR_CAPTIO`,
`ERROR_MULTILINEA`. The first error wins; later ones are usually a
cascade. Only `MULTILINEA` and the attribute-element position law
fill `result.error` with a cause; the `stml` tool prints `vitium`
for the rest.

---

## 13. The writer: fidelity and pretty

`stml_scribere(nodus, piscina, pulchrum)`.

### 13.1 Fidelity (`pulchrum = FALSUM`)

Reassembles trivia verbatim: `scribere(legere(x)) == x` over LF
input, with named exceptions (capture forms re-derived, empty pairs
collapsed, `<(>` and `<>` normalized, bare values quoted). The
fidelity contract is stated file-wide but gated only on snippets
and the pretty corpus; ticket `01M16XGV45` lists the known
divergences.

### 13.2 Pretty (`pulchrum = VERUM`)

Regenerates all layout from structure. Guaranteed by two gates:
fixed point (`pretty(read(pretty(x))) == pretty(x)`) and tree
equality modulo trivia and form memory. Rules:

- **One layout per element**: any text child → inline; else block,
  one child per line, indented two spaces.
- **Capture collapse**: a single-child element takes capture form
  (`<a(> <b(> text`); links pack greedily within the width; a line
  break indents one step per line; a block terminal opens on a fresh
  line one step deeper.
- **Multi-captee**: two or three plain element children within an
  estimated 12 lines take `((>`/`(((>` with the children stacked on
  following lines and no closer.
- **Close tags** exist only where block form remains (4+ children,
  over-height, mixed content, exotic children). `</>` when the
  element is under 30 lines, the named close beyond; raw keeps its
  name.
- **Width**: 72 columns, with a floor of indent + 40.
- **Flow text** is always re-wrapped; break points are single spaces
  and newline runs; over-budget words overflow. Hanging fill: text
  starts on the tag line and continuation hangs under its first
  column when the first word fits and at least 40 columns remain,
  else vertical fill at +1.
- **Blank lines** between block siblings are kept, clamped to at
  most two; leading blanks of the file drop.
- **Multiline attributes**: a block-position tag that does not fit
  breaks to one attribute per line, names right-aligned into an `=`
  column after the tag name; `>` and `/>` glue to the last value;
  capture closers take their own aligned line. Values never break.
- **`\` blocks** are re-indented to nesting depth, interior
  untouched; raw is never touched.
- **Exotic children** (raw, multiline, fragments, backward and
  sandwich captures, multi-paren authored captures, comments among
  captees, attribute-elements) keep their authored forms.
- Never scrape pretty STML with a regex that crosses the tag-name
  to attribute seam; multiline attributes make it a lie.

### 13.3 Sedes

`stml_scribere_sedibus` fills a side table of `{nodus, initium,
finis}` byte extents in the emitted string, elements only, post-order.

---

## 14. The chain: legere, expandere, distribuere, emit

```
resultus    = stml_legere(textus, piscina, intern);
expansio    = stml_expandere(resultus.radix, piscina, intern);
distributio = stml_distribuere(expansio.radix_expansa, piscina, intern);
/* emit distributio.radix_distributa */
```

Laws (each measured):

1. **One internamentum** for the whole chain. Ids meet at a pointer
   compare; a foreign intern makes every call `FRAGMENTUM_IGNOTUM`.
2. **Gate on `successus`, never on the root pointer.** Some vitia are
   soft (the walk continues, the root is non-NIHIL and wrong).
3. **Feed the DOCUMENTUM root**, not `elementum_radix`, or
   document-level nodes are lost.
4. `elementum_radix` is stale after expansion; re-find it.
5. **Distribution is not optional in practice**: it changes
   cardinality and attribute presence; skipping it emits a different
   document from the one canon judged. It is identity on documents
   without anonymous fragments.
6. All stages share the piscina; nothing is freed individually.

`stml formare` runs none of this (it formats the source, templates
intact). `stml expandere` and `stml vertere` run all of it.

---

## 15. Templates

### 15.1 Definition

```stml
<#@card n="@n" m="@m?" corpus="@corpus">
  <div data-n="&@n;"> ... &@corpus; ... </div>
</#>
```

- The id begins with `@`; the `@` is part of the interned id.
  Fragments without `@` are content fragments and the engine never
  touches them.
- Slots are **declared** on the opening tag: an attribute whose value
  is `@name` declares slot `name`. The attribute name is irrelevant
  to the call; house convention mirrors them (`n="@n"`).
- `@name?` declares an optional slot.
- `&@name;` references a slot anywhere in the body: whole attribute
  value, interpolated in a value (`via="&@basis;/x.c"`), interpolated
  in text, or as a whole text child (the splice position).
- A nested template definition inside a body is opaque (quoted);
  content fragments inside a body are ordinary content.
- A body may call templates defined earlier, including, under §17.4,
  itself.

### 15.2 Call

```stml
<<#@card n="silva" m="alta">>
<<#@card n="silva">><@corpus=><p>x</p></><@m=>alta</>
```

Inline arguments are scalars. Block arguments are attribute-elements
immediately following the call (a maximal run); their value is their
content. The two spellings are equivalent for scalars.

### 15.3 Argument classification

| block argument content | classified as |
|---|---|
| nothing (`<@m=/>`) | tombstone: consumed, absent |
| exactly one text child | scalar, same as inline |
| exactly one text child that is a whole `&@x;` naming a subtree argument of the caller | that subtree, threaded (forest passes across strata) |
| anything else | subtree: children expanded in the caller's context, stored as a forest |

### 15.4 Fill

- Whole-text `&@n;` in child position naming a subtree splices fresh
  clones of the forest.
- Any string position (interpolated text, attribute value,
  transclusion interior) naming a subtree is vitium VII
  `ARGUMENTUM_ARBOREUM` (soft: the literal stays, `successus` falls
  at the end).
- A scalar in a position that needs a tree (a `tag=` or pattern arm,
  PER over it) is vitium XXX `ARGUMENTUM_SCALARE`. The empty scalar
  is the empty forest and never this vitium.

### 15.5 Strata

Document order. A call resolves only to definitions collected
earlier; a body may only call definitions earlier than its own
(`tectum`). Forward reference is II `FRAGMENTUM_POSTERIUS`. Nested
fills recurse; `stratum` is the fill depth. Termination is by
construction.

### 15.6 Judgments at collection vs fill

Collection (definition read): undeclared reference in a body (VI
`LOCULUS_IGNOTUS`), duplicate ids (III), malformed COMMUTATIO (IX),
optional slot used outside a narrowing arm (XI), malformed
projection spelling (XXVI). Fill (call): unknown id (I), unfilled
required slot (IV), superfluous argument (V), duplicate argument
(VIII), no arm matched (X).

### 15.7 Result

`StmlExpansioResultus {successus, radix_expansa, tabula_expansionum,
vitium, linea, fragmentum, loculus}`. The expanded tree is new; the
original is untouched and remains queryable. `tabula_expansionum` is
a side table of `StmlExpansioNota {nodus, fragmentum_id, vocatio,
stratum}` in splice order. Definitions are dropped from the expanded
tree; content transclusions pass through. Clones carry `linea` but
zero `positus` (the extent-provenance obligation, unbuilt).

---

## 16. COMMUTATIO

```stml
<COMMUTATIO de="&@m;">
  <CASUS est="alta">...</CASUS>
  <CASUS nihil>...</CASUS>
  <CASUS non-nihil>...</CASUS>
  <CASUS tag="cap">...</CASUS>
  <CASUS><EST><EXEMPLAR>pattern</EXEMPLAR></EST>...</CASUS>
  <ORDINARIUS>...</ORDINARIUS>
</COMMUTATIO>
```

Only inside template bodies; at document level it passes through
untouched. `de=` must be exactly one whole `&@x;` reference to a
declared slot (it is exempt from narrowing). Arms are tried in order;
first match wins; `ORDINARIUS` must be last. COMMUTATIO, CASUS,
ORDINARIUS and EST never reach the expanded tree.

| arm | fits when | establishes presence of `de` |
|---|---|---|
| `est="literal"` | scalar equals the literal (no `&@` allowed in `est`) | yes |
| `nihil` | slot absent (not given, or tombstoned) | no |
| `non-nihil` | slot present, scalar or subtree | yes |
| `tag="t"` | subtree whose root element is `t` | yes |
| `<EST><EXEMPLAR>P</EXEMPLAR></EST>` | subtree matches pattern P, anchored at its root (§19); P is filled with the current arguments first; captures join the arm's arguments | yes |
| `ORDINARIUS` | always | no |

Rules: `est` against a subtree is VII; `tag`/pattern against a
scalar with text is XXX; an optional slot referenced outside an arm
that establishes presence is XI at collection; nothing matched and no
`ORDINARIUS` is X `CASUS_NULLUS`; comments between arms are skipped;
any other child is IX. `<EST>` holds exactly one attribute-less
EXEMPLAR with a one-element body. A capture name already in the
argument table is XXII; a capture naming a declared slot is refused
at collection.

---

## 17. Slot projections and PER inside bodies

### 17.1 Projections `&@n.a.b;`

On a subtree argument `n` whose single root is an element, each path
segment names a **child element** of the element reached so far and
yields its **content**: text only → scalar; otherwise the child
forest. Never the wrapper itself; choosing among same-named siblings
is a pattern's job. No implicit element-to-text rule.

`&@n.a!;` reads the bytes of text and raw leaves concatenated; `!` is
allowed on any projection as an assertion (structure under it is
VII). `!` over an empty element yields no bytes.

| vitium | when |
|---|---|
| XXVI `PROIECTIO_MALFORMATA` | empty segment (`a..b`, `.b`, `a.`), or `.`/`!` in a slot declaration; spread forms `&@...x;` land here (dead) |
| XXVII `PROIECTIO_ABSENS` | the current forest has no element root (scalar with text, or empty), or a segment names no child; a present but empty wrapper is the empty scalar, not absent |
| XXVIII `PROIECTIO_AMBIGUA` | several element roots, or several children with the segment's name |

Also legal as `de="@n.a"` on PER (below). `de="@arg"` on EXEMPLAR
inside a body is **not built** (XIII).

### 17.2 PER over a forest

```stml
<PER de="@n.items" voca="#@item"/>
<PER de="@n.items" ut="i">...&@i;...</PER>
```

Rows are the **elements** of the projected forest (text and comments
are not rows). `voca=` calls the named template once per row,
filling its sole required subtree slot (zero or several required
subtree slots = XXIX `PER_DELEGATIO_AMBIGUA`); a fresh argument
table, the caller's scope does not leak. `ut=` fills the inline body
within the same instantiation: the outer scope stays visible, the
row name is local (outside the body it is VI), a row name shadowing
a declared slot is XIII. An empty wrapper is a source with zero rows.
`!` in `de=` is XIII (the source is a forest, not bytes). `voca=` +
body, `ut=` without body, `de=` without a forest: XIII.

### 17.3 Self-calls

A call (or `voca=`) naming the definition currently being filled is
admitted iff every subtree argument of the call is a strict
descendant (parent-pointer containment) of some subtree argument of
the current instantiation, and at least one such argument exists.
Otherwise XXXI `RECURSIO_NON_DESCENDENS`. PER delegation to the
current template is checked per row. Mutual recursion stays illegal.

---

## 18. DISTRIBUTIO

Second content projection, `stml_distribuere`, run after expansion.

A **wrapper** is a named element whose children, after the
attribute-element prefix, are all anonymous fragments (`<>`, `<#>`,
`<(>` capturing), at least one. It dissolves into one copy per item:

- wrapper attributes (inline and prefix attribute-elements) carry
  down to every item;
- an item's own attribute of the same name (inline or prefix
  attribute-element) wins; the wrapper's is suppressed;
- an item tombstone `<@x=/>` deletes the inherited attribute and
  stays in the output;
- items are forests; an empty item is an empty element;
- nested anonymous lists flatten into the same type;
- comments among items pass through in place;
- distributed elements take the item's trivia and line, the
  wrapper's type and attributes; the item's `</>` flag wins.

Refused: `distributio MIXTA (involucrum 'x')` when anonymous
fragments are mixed with named children or with text. Same-line
spaces between items are text. Anonymous fragments with no named
parent pass through untouched.

Result: `StmlDistributioResultus {successus, radix_distributa,
vitium (BENE/MIXTA/MEMORIA), linea, titulus}`; a new tree.

---

## 19. EXEMPLAR, captures, TRANSPARENTIA, PER

### 19.1 EXEMPLAR

```stml
<EXEMPLAR [de="$prior"] [modus="omnia|unum|primum|optional"] output="$name" [ancorata] [radix="fontis"]>
  one element
</EXEMPLAR>
```

Document level (or as a link, §20; or in `<EST>`, §16/§21). Resolved
during the one expansion walk, interleaved: it sees only content
**above** itself in document order, in the expanded view. The
result is a **relation**: a set of rows, each a binding environment
(captures → values) plus its root. Rows are tuples: a stage applied
to a prior relation inherits every capture of the incoming row and
adds its own.

| attribute | meaning |
|---|---|
| `de=` absent | scope = the document above |
| `de="$x"` | scope = each row of the prior relation `$x` (per-row application, rows extended) |
| `output="$x"` | required; names the relation; single assignment; must be consumed below (XVIII) |
| `modus` | `omnia` (default, all matches), `unum` (exactly one, else XVI), `primum` (first; zero fine), `optional` (zero or one, else XVII) |
| `ancorata` | try only at the scope's root; absent = floating, tried at every subtree root |
| `radix="fontis"` | output rows keep the incoming row's root instead of narrowing to the match (needs `de=`; else XIII) |

Floating bodies must be single-rooted (XV `CORPUS_SILVESTRE`).

### 19.2 Captures

| spelling | binds |
|---|---|
| `attr="$x"` | the attribute value (scalar); `a=""` never satisfies it |
| `<t>$x</t>` | the whole text (scalar) |
| `<t $x/>` (bare attribute) | the node (forest of one) |
| repeated `$x` | equality constraint; forests compared byte-wise |

Every row implicitly binds its root. Capturing a name the row already
carries is XXII `CAPTURA_COLLISA`.

### 19.3 Matching (loose)

- Element: tag equal (interned pointer); written attributes required
  with equal literal or capture; omitted attributes free. Only inline
  attributes of the candidate are consulted.
- Children: ordered subsequence, greedy, no backtracking; each
  pattern child takes the first matching candidate child; omitted
  children free.
- Text: equality; a whole-text `$x` captures.
- `<*/>` any one element. `<**>` with exactly one element child:
  hoisted out of the sibling subsequence, "somewhere below me",
  bound to the first compatible descendant in document order (no
  enumeration; equality conflicts walk on). Any attribute on `<**>`
  or a wrong child count is XXV, at setup. Strict mode never matches
  `<**>`.
- Effective children (both sides): content fragments dissolve,
  `<<#lexN>>` content aliases resolve to their fragment's content,
  transclusions otherwise skipped, attribute-elements skipped,
  TRANSPARENTIA tags and `@`-space skipped whole.
- A pattern containing `&@n;` in a positive stage is instantiated per
  incoming row (the bridge; §19.5); with no incoming rows it is XIII.

### 19.4 TRANSPARENTIA

```stml
<TRANSPARENTIA tags="expansio pasta ante post" attributa="linea columna"/>
```

Document level, strata-scoped. Matching does not see into the listed
tags (subtree blindness: listing a token wrapper prunes everything
inside it) and ignores the listed attributes on both sides. Missing
or empty `tags=`, or a present but empty `attributa=`, is XIX.
Affects matching only, never emission or canon.

### 19.5 PER

```stml
<PER congruentia="$x">...&@capture;...</PER>
<PER congruentia="$x" voca="#@f"/>
```

Instantiates the body (or calls the template) once per row in
document order; captures arrive as the argument map: scalars fill
strings, node captures splice as clones. Zero rows contribute
nothing. A reference to a capture the row does not carry is left as
literal text (measured; not loud). The relation is consumed by this.

### 19.6 Invariant

No EXEMPLAR, PER, TRANSPARENTIA, CATENA, DIRIBITIO, SINE, EST or
INDAGO survives into a resolved document. A pattern without a
consumer is dead code (XVIII, soft).

---

## 20. CATENA

```stml
<CATENA output="$x" [de="$prior"]>
  <(><EXEMPLAR>...</EXEMPLAR>
  <(><EXEMPLAR radix="fontis">...</EXEMPLAR>
  <(><SINE>...</SINE>
  <(><DIRIBITIO>...</DIRIBITIO>
  <(><CATENA>...</CATENA>
</CATENA>
```

Stages thread implicitly: each link's scope is the previous link's
output; the tail is the chain's output. `output=` and `de=` live on
the wrapper only. Links carry no `de=`/`output=` (a link `output=` is
the reserved tee, XX) but may carry `modus=`; the wrapper carries no
`modus=`/`ancorata`. `<(>` before a link is permitted decoration (an
anonymous capturing fragment; fragments dissolve). Nested bare
chains flatten; a nested CATENA with attributes is XX. Links may be
macro calls that expand to a link (literal links are used raw,
macro-produced links get instantiation semantics). Purity law: every
link is relation → relation; effects consume the tail elsewhere.
Malformations are XX `CATENA_MALFORMATA`.

---

## 21. DIRIBITIO

```stml
<DIRIBITIO de="$x" output="$y">
  <CASUS [angustans]>
    <EST>condition (EXEMPLAR | CATENA | DIRIBITIO, or a template expanding to one)</EST>
    one pure command
  </CASUS>
  <ORDINARIUS>one pure command</ORDINARIUS>
</DIRIBITIO>
```

Set-level guarded dispatch over the incoming relation: the first
CASUS whose condition yields a non-empty relation is taken; its arm
receives the **original** incoming relation, or, with `angustans`,
the condition's output. `ORDINARIUS` is optional and must be last;
no arm taken = empty relation out. Condition roots are bare (no
`output=`/`de=`/`modus=`; `ancorata` allowed). Works at document
level, as a link, and recursively inside EST. Malformations are XXI
`DIRIBITIO_MALFORMATA` (including `ORDINARIUS angustans`).

Identity idiom: `<EXEMPLAR ancorata><*/></EXEMPLAR>` as an arm maps
each row to itself, so "report the offenders" is a condition plus
the identity.

---

## 22. SINE

```stml
<(><SINE>one pattern</SINE>
```

A chain link that **filters**: for each incoming row the body (with
`&@n;` filled from the row) is searched, floating, inside the row's
root; if anything matches the row is dropped, else it passes
unchanged. It asks existence only. Rules: no engine attributes on
SINE, body exactly one pattern, and a source relation must exist (a
SINE first in a document chain without `de=` is XXIII; document-wide
absence is a named door). After a narrowing stage SINE searches the
narrowed root; keep the scope element as the pattern root when the
absence lives beside the match.

---

## 23. INDAGO

`<INDAGO/>` at document level (bare; any attribute or non-comment
child is XXIV) makes every command consumed **after** it leave a
trace comment at its site:

```
<!--indago CATENA $sine-recensione (radix); 1 EXEMPLAR bibliotheca radix->1; 2 EXEMPLAR liber 1->3 (capturae: id a); 3 SINE recensio 3->1 (necati 2); exitus 1-->
<!--indago DIRIBITIO $verdictum (de $libri 3): CASUS 1 conditio 0 praeteritum; ORDINARIUS SUMPTUM; bracchium; EXEMPLAR * 3->3; exitus 3-->
<!--indago PER $verdictum: ordines 3-->
```

Rows in and out per stage, capture names, SINE kills, arm verdicts
(`SUMPTUM`/`praeteritum`, `bracchium angustatum`), PER row counts.
Conditions report verdict counts only. Comments are invisible to the
semantics they describe. Zero behaviour change without the element.

---

## 24. Expansion vitia

`StmlExpansioVitium`, first found wins; the `stml` tool prints the
name plus `(fragmentum '@id')` and `(loculus 'x')` where known. Soft
vitia (VII, XVIII) let the walk finish and fail `successus` at the
end.

| n | name | when |
|---|---|---|
| I | `FRAGMENTUM_IGNOTUM` | call to an undefined template |
| II | `FRAGMENTUM_POSTERIUS` | call to a template defined later |
| III | `FRAGMENTUM_GEMINUM` | two definitions with one id |
| IV | `LOCULUS_NON_IMPLETUS` | required slot not filled (tombstone counts as unfilled) |
| V | `ARGUMENTUM_SUPERFLUUM` | argument naming no declared slot |
| VI | `LOCULUS_IGNOTUS` | body references an undeclared slot |
| VII | `ARGUMENTUM_ARBOREUM` | subtree in a string position (soft) |
| VIII | `ARGUMENTUM_GEMINUM` | same argument twice |
| IX | `COMMUTATIO_MALFORMATA` | bad `de=`, `est` with a reference, non-CASUS child, ORDINARIUS not last, bad arm shape |
| X | `CASUS_NULLUS` | no arm matched, no ORDINARIUS |
| XI | `LOCULUS_NON_ANGUSTATUS` | optional slot used outside a presence-establishing arm |
| XII | `EXEMPLAR_SINE_EXITU` | EXEMPLAR without `output=` or without the `$` |
| XIII | `EXEMPLAR_MALFORMATUM` | unknown modus, duplicate output, `de=`/`congruentia=` without `$`, bad `voca=`, EXEMPLAR/CATENA/DIRIBITIO inside a body, bad `radix=`, `&@n;` bridge with no rows, PER form errors |
| XIV | `SCOPUS_IGNOTUS` | `de=`/`congruentia=` names no prior relation |
| XV | `CORPUS_SILVESTRE` | forest body under floating application |
| XVI | `UNUM_VIOLATUM` | `modus="unum"` with 0 or ≥2 |
| XVII | `OPTIONAL_MULTIPLEX` | `modus="optional"` with ≥2 |
| XVIII | `EXITUS_NON_CONSUMPTUS` | an output nothing consumes (soft) |
| XIX | `TRANSPARENTIA_MALFORMATA` | missing/empty `tags=`, empty `attributa=` |
| XX | `CATENA_MALFORMATA` | see §20 |
| XXI | `DIRIBITIO_MALFORMATA` | see §21 |
| XXII | `CAPTURA_COLLISA` | a capture name repeated across stages or arms |
| XXIII | `SINE_MALFORMATUM` | see §22 |
| XXIV | `INDAGO_MALFORMATUM` | INDAGO not bare |
| XXV | `DESCENSUS_MALFORMATUS` | `<**>` with attributes or not exactly one element child |
| XXVI | `PROIECTIO_MALFORMATA` | bad projection spelling |
| XXVII | `PROIECTIO_ABSENS` | no element root or no such child |
| XXVIII | `PROIECTIO_AMBIGUA` | several roots or several such children |
| XXIX | `PER_DELEGATIO_AMBIGUA` | delegated template has zero or several required subtree slots |
| XXX | `ARGUMENTUM_SCALARE` | scalar with text where a tree is needed |
| XXXI | `RECURSIO_NON_DESCENDENS` | self-call whose arguments do not strictly descend |

---

## 25. HTML emission (`vertere`)

`stml_html_vertere(radix_documenti, piscina)` on a tree **after** the
chain; `stml_html_vertere_liberos(parens, piscina, litteralis)` for a
fragment without doctype. No tag vocabulary; the tags are the
author's. Nothing partial: on refusal the output is empty.

| construct | emission |
|---|---|
| element | `<t attrs>...</t>`; void set (`area base br col embed hr img input link meta param source track wbr`) written bare, children or a close on a void element refused (`VACUUM_CUM_LIBERIS`); everything else always paired, never `<t/>` |
| text | entities decoded in the tree, escaped on output: `&` `<` `>` |
| inline attribute | raw bytes as written |
| attribute-element (prefix) | decoded, escaped `&` and `"` |
| tombstone | omitted |
| boolean `true` | bare name if in the HTML boolean set (`disabled checked required readonly selected multiple autofocus hidden async defer novalidate open reversed loop muted controls default ismap itemscope nomodule playsinline`); else `ATTRIBUTUM_VERUM` (use `<@href=>true</>`) |
| `a=""` | `a=""` (kept; `alt=""` is load-bearing) |
| comment | `<!--` + raw value + `-->` |
| DOCTYPE in source | ignored; `<!DOCTYPE html>` is prepended by the emitter |
| `<script!>` `<style!>` | content verbatim |
| any other `<t!>` | content escaped as literal text |
| `<script>`/`<style>` without `!` | `CRUDUS_DEEST` |
| `<crudum!>bytes</crudum>` | bytes spliced with no tag (only this name, only raw) |
| `<t\>` | value emitted; `indentatio` ignored |
| fragment | dissolved |
| captures, `</>` | no trace; children in document order, real close tags |
| whitespace | trivia (`spatia_ante/post/clausurae`) emitted, so `x y` stays `x y` |
| `&word;` or `&#...;` in text | `ENS_AMBIGUUM` (type the character); does not apply inside raw blocks, so `<code!>&nbsp;</code>` shows literal `&nbsp;` |
| `<<...>>` | `TRANSCLUSIO` |
| `<% &k;>` | `AUGMENTATIO` |
| `<?...?>` | `PROCESSIO` |
| `<.x>` | `TITULUS_PUNCTATUS` |
| nameless element | `TITULUS_NIHIL` |
| attribute-element outside a prefix (post-expansion) | `ATTRIBUTUM_SOLUTUM` |
| repeated singleton (`<body>` twice) | emitted as written; the emitter never judges HTML validity |

`litteralis = VERUM` (parser-generated trees, e.g. from md): every
`&` in text is escaped, no ambiguity refusal, inline attributes
escaped too. Known hazard: `</script` inside script raw content ends
the element in the browser; not refused.

---

## 26. Canon

### 26.1 Files and registry

A canon is an STML file with root `<canon dialectus= versio=>`.
`canones.registrum` (repository root) maps documents to canons by
two keys, root element winning over extension:

```
.genera        natura/natura.canon
<aedilis>      aedilis.canon
<canon>        canon.canon
.canon         canon.canon
<parsura>      silva/c89.canon
```

A document may also carry an **inline canon** as the first element
child of its root (`<r><canon>...</canon>...</r>`). Both judge: the
registry canon is the floor, the inline canon can only add, and the
inline canon is itself judged against `canon.canon`. `canon` as first
child of a root is reserved across dialects; elsewhere it is ordinary
vocabulary. `canon.canon` describes the canon language and judges
itself.

### 26.2 Elements of a canon

```stml
<elementum nomen="x" [intra="parent"] [radix="verum"] [textus="verum|nomen|numerus|veritas|dies|compositum"] [nota=]>
  <attributum nomen="a" [genus=] [necessarium="verum"] [ordinarius=] [fractio="verum"] [minimum=] [maximum=] [nota=]>
    <optio>...</optio>
  </attributum>
  <liberum nomen="c" [minimum=] [maximum=] [nota=]/>
  <glossa lingua="la">...</glossa>
</elementum>
<unicitas nomen= attributum= super="e1 e2" [intra=]/>
<citatio nomen= attributum= ad="e1 e2/attr" [super="e or parens/e"] [intra=]/>
<claves-externae fons="natura"><clavis genus="canis">&laika;</clavis></claves-externae>
```

- `intra=` scopes a definition to a parent (`parens/nomen` key; most
  specific wins). Repeated element names are legal with different
  `intra=`.
- `textus=` absent forbids non-whitespace text; `verum` allows any;
  a genus name requires non-empty text to conform (`TEXTUS_MALUS`).
- Cardinality is an unordered bag with min/max, counted per child
  name.
- `ordinarius=` states a default (must conform to the genus, else the
  canon fails to load); canon says, generated readers apply.
- `minimum`/`maximum` on `attributum` bound a `numerus` value
  (inclusive, digit-string comparison, `fractio="verum"` allows a
  decimal point); on `liberum` they are cardinality.
- `unicitas` needs both `attributum=` and `super=` (an empty `super`
  judges nothing); `intra=` makes it per-instance.
- `citatio`: the citing attribute's value must equal a key declared
  on one of the `ad=` elements (several = subsumption); `super=`
  restricts citing elements, `parens/titulus` paths allowed; `intra=`
  scopes per instance; `ad="*/nomen"` is the star form. Citations
  match by exact element title, no inheritance; several citations on
  one attribute are conjunctive.
- Naming an attribute that exists nowhere in `unicitas`/`citatio` is
  a load-time refusal.

### 26.3 Value genera

| genus | accepts |
|---|---|
| `textus` (default) | anything |
| `nomen` | `[A-Za-z0-9_*]` |
| `numerus` | digits with optional sign; decimal point if `fractio` |
| `veritas` | `verum` or `falsum` only (a bare STML attribute is `true` and does **not** conform; use `electio` with one `<optio>true</optio>`) |
| `dies` | `yyyy`, `yyyy-mm`, `yyyy-mm-dd` |
| `electio` | one of the `<optio>` values |
| `compositum` | `nomen` plus `-` |
| `identitas` | `&x;` (an individual's proper name, declaration or reference, verbatim keys) |
| `referentia` | `&x;` or `.x` (bare = vitium) |
| `titulus` | `compositum` with one optional leading `.` (element and child names) |

### 26.4 Judgment

`canon_iudicare(canon, radix, piscina)` judges the document as
written; `canon_iudicare_expansum` expands first (templates filled,
commands executed) and judges the sense; `canon_iudicare_distributum`
expands and distributes. The intern passed must be the tree's own.
Multi-root documents are judged whole (fragments cross roots); the
root check, inline canon, and key scopes use the first root.

What canon sees through: content transclusions (judged at the point
of use, cycles refused), fragments, augmentation. What it does not
judge: template definition bodies (quoted), call arguments,
attribute-elements bound to a call, well-formed CAPS constructs
(invisible to every vocabulary; their form is judged separately as
`MACHINAE_MALFORMATUM`).

### 26.5 Vitia

| n | `CanonVitiumGenus` | message |
|---|---|---|
| I | `ELEMENTUM_IGNOTUM` | `elementum extra canonem` (+ `an X?` suggestion) |
| II | `ATTRIBUTUM_IGNOTUM` | `attributum extra canonem` |
| III | `ATTRIBUTUM_DEEST` | `attributum necessarium deest` |
| IV | `VALOR_MALUS` | `valor generi attributi non congruit` |
| V | `LIBERUM_ILLICITUM` | `liberum hic non licet` |
| VI | `LIBERI_PAUCI` | `liberi pauciores quam minimum` |
| VII | `LIBERI_MULTI` | `liberi plures quam maximum` |
| VIII | `TEXTUS_ILLICITUS` | `textus ubi non licet` |
| IX | `NOMEN_BIS` | `nomen bis in spatio unico` |
| X | `RADIX_MALA` | `elementum radicis aliud quam canon poscit` |
| XI | `TEXTUS_MALUS` | `textus generi elementi non congruit` |
| XII | `CITATIO_IRRITA` | `citatio non resoluta (clavis absens in scopo)` |
| XIII | `VOCABULUM_IGNOTUM` | `vocabulum extra petitum citationis` |
| XIV | `VALOR_EXTRA_FINES` | `valor extra fines declaratos` |
| XV | `AUGMENTUM_PUGNANS` | augmentation of a single-valued member |
| XVI | `CLAVIS_COLLISA` | a library key minted again |
| XVII | `CITATIO_ALIENA` | key exists but of a genus outside `ad=` |
| XVIII | `TRANSCLUSIO_IRRITA` | transclusion to a missing fragment |
| XIX | `FRAGMENTUM_GEMINUM` | fragment defined twice |
| XX | `TRANSCLUSIO_CIRCULARIS` | fragment transcluding itself |
| XXI | `EXPANSIO_FRACTA` | expansion failed (carries the engine vitium number) |
| XXII | `DISTRIBUTIO_FRACTA` | distribution failed |
| XXIII | `MACHINAE_MALFORMATUM` | a CAPS construct malformed |

### 26.6 Signa

`&nomen;` = an individual (declared with `nomen="&x;"` on an
`identitas` attribute, cited anywhere a `referentia` attribute
allows); `.genus` = a kind, checked against the citation's `ad=`
vocabulary; bare = literal. `claves-externae` embeds a library's
keys so any document can cite them; minting an existing key is XVI;
`<% &x;>` augments one.

### 26.7 Deliberate limits

No expression language, ever. No co-occurrence constraints ("if
`genus=electio` then at least one `optio`"), no cross-attribute
comparisons, no ordered content models, no inheritance in citations.
Those live in the reading program, and a hand-written test is the
only guard for them.

---

## 27. Selectors

`lib/selectio.c`, used by `stml_quaerere`, `stml_quaerere_omnes`,
`stml_proximus_maior`.

| supported | example |
|---|---|
| type | `liber` |
| id | `#x` (attribute `id`) |
| class | `.x` (matches `labels`, or `class`) |
| attribute | `[a]` `[a=v]` `[a^=v]` `[a$=v]` `[a*=v]` `[a~=v]` |
| pseudo-classes | `:first-child` `:last-child` `:empty` |
| combinators | descendant (space), child `>`, adjacent `+`, general sibling `~` |
| compound | `div.x#y[a=v]` |

Not supported: `:not()`, `:nth-child()`, `:root`, label namespaces
`.ns::`, `##` id-prefix selectors (all TS-era features). A leading
`.` in a selector is always a class, so `<.species>` elements cannot
be selected by type.

---

## 28. Command-line tools

### 28.1 `stml`

```
stml formare   <via.stml> [-ad <exitus>] [-probare]
stml expandere <via.stml> [-ad <exitus>]
stml vertere   <via.stml> [-ad <exitus>]
```

| verb | does | output |
|---|---|---|
| `formare` | parse, pretty-write; **no** expansion | STML |
| `expandere` | parse, expand, distribute, pretty-write; idempotent | STML |
| `vertere` | the chain, then HTML | HTML with doctype |

Exit: 0 emitted or conforming; 1 `-probare` found divergence
(`'x.stml' a forma pulchra divergit`); 2 refusal, always with a
message on stderr (`via:linea:columna: causa` for parse errors,
`via:linea: NAME (fragmentum '@x') (loculus 'y')` for expansion
vitia, `via: NAME ('detail')` for HTML refusals). `-ad` naming the
input path is refused. Nothing is written on refusal. A CRLF file
formats to LF and is reported divergent once.

Build: `./tools/stml_struere.sh` → `bin/stml`, installed to
`~/.bin/stml` (remove before copying; macOS caches signatures).

### 28.2 `canon_examen`

```
bin/canon_examen <plagula> ...             canon by registry
bin/canon_examen -canon X.canon <plagula>  explicit canon
bin/canon_examen -machina ...              TSV: via linea columna elementum campus nuntius fons
bin/canon_examen -index <canon>            the canon's inventory as TSV
```

Human form: `via:linea: <elementum> nuntius: detail [fons]`, where
`fons` is `registrum path`, `infixus`, or `infixus-ipse`. Exit 0
clean, 1 vitia (including unparsable input), 2 nothing judged (an
unregistered dialect never passes silently). `canon_examen X.canon`
judges X as a document; to test a canon as a canon use `-canon`.
`bin/canon_coquere` generates typed C readers from a canon.

### 28.3 Elsewhere

`silex iudicare <via>` judges STML against the bundled canon from
outside the repository. `./md/html.sh <x.md>` runs the markdown
projection through the md template and `vertere`. `./silva/arbor.sh`
projects C into STML. pythonica: `silva.expandere(text|path)`
returns the expansion result with named vitia.

---

## 29. C API map

All functions take a `Piscina*` for allocation and an
`InternamentumChorda*` where strings are interned; all strings are
`chorda` (length-prefixed, not NUL-terminated).

| header | area | key functions |
|---|---|---|
| `stml.h` | parse | `stml_legere`, `stml_legere_ex_literis` → `StmlResultus {successus, radix, elementum_radix, status, linea_erroris, columna_erroris, error, crlf_canonicalizatum}` |
| | lex only | `stml_lexemata_colligere` (total token stream with byte extents, no validation) |
| | query | `stml_invenire_liberum`, `stml_invenire_omnes_liberos`, `stml_attributum_capere`, `stml_attributum_habet`, `stml_numerus_liberorum`, `stml_liberum_ad_indicem` |
| | text | `stml_textus_valor`, `stml_textus_fluxus`, `stml_textus_internus`, `stml_textus_normalizatus` |
| | navigate | `stml_frater_proximus`, `stml_frater_prior`, `stml_primus_liberum`, `stml_ultimus_liberum`, `stml_fratres`, `stml_maiores`, `stml_proximus_maior`, `stml_index_inter_fratres` |
| | build | `stml_elementum_creare`, `stml_elementum_crudum_creare`, `stml_transclusionem_creare`, `stml_textum_creare[_ex_chorda]`, `stml_commentum_creare`, `stml_attributum_addere[_chorda]`, `stml_attributum_boolean_addere`, `stml_liberum_addere`, `stml_textum_addere` |
| | mutate | `stml_praeponere`, `stml_inserere_ante/post`, `stml_removere`, `stml_vacare_liberos`, `stml_substituere`, `stml_duplicare`, `stml_duplicare_superficialiter` (leaves `liberi` NIHIL) |
| | labels | `stml_titulum_habet/addere/removere/commutare`, `stml_titulos_capere/numerus` |
| | write | `stml_scribere`, `stml_scribere_ad_aedificator`, `stml_scribere_sedibus` |
| | judge | `stml_strictum`, `stml_strictum_nuntius` |
| `stml_macros.h` | expand | `stml_expandere` → `StmlExpansioResultus`; `stml_distribuere` → `StmlDistributioResultus`; `stml_congruere_strictum` (the lockstep matcher) |
| `canon.h` | schema | `canon_legere`, `canon_ex_nodo`, `canon_infixum_invenire`, `canon_iudicare[_expansum|_distributum]`, `canon_nuntius`, `canon_registrum_quaerere[_radice]` |
| `stml_html.h` | HTML | `stml_html_vertere`, `stml_html_vertere_liberos`, `stml_html_vitium_titulus` |
| `selectio.h` | selectors | `selectio_legere[_ex_literis]`, `selectio_congruit`, `selectio_invenire_primum/omnes`, `stml_quaerere[_omnes]` |

Silva vendors its own copy of the STML library
(`silva/amalgama/silva.h`); a new `StmlNodus` field must be mirrored
there by hand.

---

## 30. Reserved, dead, and absent forms

From the sigil registry (`stml-visio.md` §2), so that nobody re-invents them:

| form | status |
|---|---|
| `<<x \| transform>>` pipe views | reserved, generated projections only |
| `&@...children;` spreads | dead; refused |
| `<TEXTUS>` flattening verb | reserved |
| `de="@arg"` on EXEMPLAR inside bodies | reserved (XIII today) |
| `strictum` flag, per-node strictness escape | reserved |
| `? * +` occurrence suffixes, `minime=`/`maxime=` ranges | reserved |
| sibling-run floating bodies | reserved |
| CATENA tee (`output=` on a link) | reserved, loud |
| aggregates (`numerus/summa/minimum/maximum`) | reserved as decreed projections |
| `%` mutation verbs on matches | reserved; blocked on extent provenance |
| `<VITIUM nuntius=>` user vitium | reserved |
| child elements inside `<t\>` | reserved (v2) |
| `##prefix` selectors, `uuid`/`nid` autogeneration, `.ns::label` | TS only, absent in C |
| `:`/`::` parameter syntax, `<{> <}>` infix XML, `$:` positional args, `@x` interpolation in data | dead |
| `#` as world-entity sigil | dead (migrated to `&x;`) |
| `<!switch>` | never; CAPS tags are the builtin grammar |

---

## 31. Traps

- `</#nomen>` is silently not a closer.
- `<!doctype html>` lowercase is a syntax error; `vertere` writes the
  doctype itself, so never write one.
- `<a (> <@m=>x</>` is legal (position judged after captures); a
  same-line space before an attribute-element is text and breaks the
  prefix.
- `&nbsp;` ≡ `&amp;nbsp;` in the tree; `vertere` refuses both.
- `a=""` becomes `a` (= `true`) under `formare`.
- An anonymous fragment beside a named sibling is a distribution
  error, not a group.
- Same-line spaces between `<>` items are text → `MIXTA`.
- `<**>` as a pattern body root yields zero rows; put it inside an
  element. `<**>` binds one descendant, never all; enumerate with
  floating application.
- After a narrowing stage, `SINE` searches the narrowed subtree.
- Rows are tuples: re-capturing an inherited name is `CAPTURA_COLLISA`.
- In a PER body, `&@x;` for a capture the row lacks stays literal.
- `ORDINARIUS` and `nihil` arms do not prove presence; an optional
  slot referenced there is refused at definition time.
- A tombstone and an inline argument of one name is
  `ARGUMENTUM_GEMINUM`; a tombstone on a required slot is
  `LOCULUS_NON_IMPLETUS`.
- Template calls address slot names (`@n`), never the declaring
  attribute name.
- `stml_expandere` zeroes `positus` on every clone; nothing that
  writes may ride pattern matches yet.
- `stml_duplicare_superficialiter` leaves `liberi` NIHIL; create the
  Xar before appending.
- `stml_attributum_capere` returns the first of duplicate attributes
  and returns decoded text for attribute-elements but raw bytes for
  inline values; the HTML emitter avoids it for that reason.
- `<*/>` inside a C block comment terminates the comment.
- Regex over pretty STML must not cross the tag-name to attribute seam.
- Gate the chain on `successus`, never on the root pointer.
