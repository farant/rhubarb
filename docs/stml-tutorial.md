# STML: a tutorial

This is a tutorial for someone who has never written a markup
document before. If you know HTML you can skim the first chapters,
but do not skip them: STML looks like HTML and then quietly does
several things differently.

STML is three things stacked on top of each other:

1. A way of writing a **document** as plain text, so that both a
   person and a program can read it. This is the part that resembles
   HTML and XML.
2. A small **template** language for generating documents from
   other documents without repeating yourself.
3. A small **pattern** language for asking a document questions
   ("which books have no review?") and turning the answers into a
   new document.

The whole thing is written in C89 and lives in this repository. The
library is `lib/stml.c` with its header `include/stml.h`; the
templates and patterns live in `lib/stml_macros.c`. You do not need
to read any of that to follow this tutorial.

Every example below was run through the real tool before it was
pasted here. Run them yourself. STML is best learned by typing a
document, running it, and looking at what came out.

## What you need

- The `stml` command. Build and install it with
  `./tools/stml_struere.sh`; it lands in `~/.bin/stml`.
- A text editor.
- For the last chapter, `bin/canon_examen`, built by
  `./tools/canon_struere.sh`.

STML files end in `.stml`. Everything in this repository is named in
Latin, so tag names in the examples are Latin words. Where it helps,
the English is given in parentheses the first time.

---

## 1. A document is a tree

Here is a small STML document. Save it as `liber.stml`:

```stml
<liber>
  <titulus>De Agri Cultura</titulus>
  <auctor>Cato</auctor>
  <pagina numerus="1">Est interdum praestare mercaturis rem quaerere.</pagina>
  <pagina numerus="2">Maiores nostri sic habuerunt.</pagina>
</liber>
```

Read it from the outside in. `<liber>` (book) opens something and
`</liber>` closes it. Between them are four smaller things: a title,
an author, and two pages. Each of those is opened and closed the
same way. Inside the pages there is plain text.

The words for the parts:

| you see | it is called | Latin, as the code calls it |
|---|---|---|
| `<liber>` ... `</liber>` | an **element** | elementum |
| `liber` | the element's **tag name** | titulus |
| `<liber>` | the **opening tag** | |
| `</liber>` | the **closing tag** | |
| `numerus="1"` | an **attribute**: a name and a value on the opening tag | attributum |
| `Cato` | **text** | textus |
| the four things inside `liber` | its **children** | liberi |
| `liber`, for `titulus` | the **parent** | parens |
| `liber`, for the whole file | the **root** | radix |

Because every element sits inside exactly one parent, the document
is a tree. `liber` is the trunk; `titulus`, `auctor`, and the two
`pagina` elements are branches; the text is the leaves.

Three rules keep it a tree:

- Everything is inside one root element.
- Elements nest. You close the inner one before the outer one.
  `<a><b></a></b>` is wrong.
- An element with nothing inside can be written short:
  `<pagina numerus="3"/>` means the same as
  `<pagina numerus="3"></pagina>`.

You can leave notes for yourself with a comment:

```stml
<!-- this is a comment -->
```

A comment is kept as part of the document. Tools that print the
document print the comment too.

Tag names start with a letter or `_` and continue with letters,
digits, `_`, `-`, `.`, or `:`. Kebab names like `definitio-functionis`
are the house style.

---

## 2. Meet the tool

The `stml` command has three verbs. The first one, `formare`
(to shape), reads a document and prints it back tidied up. Run it on
the book:

```
stml formare liber.stml
```

You get this:

```stml
<liber>
  <titulus(> De Agri Cultura
  <auctor(> Cato
  <pagina numerus="1" (> Est interdum praestare mercaturis rem quaerere.
  <pagina numerus="2" (> Maiores nostri sic habuerunt.
</>
```

Two things look strange, and you need to be able to read them before
going on, because the formatter uses them everywhere.

**`(>` on an opening tag.** The `(` is a **capture**: it says "the
next thing on this line is my child, and I have no closing tag".
So `<titulus(> De Agri Cultura` means exactly
`<titulus>De Agri Cultura</titulus>`. The space after `(>` is a
separator, not part of the text. Chapter 5 explains captures fully;
for now, read `(>` as "and here is my one child".

**`</>` with no name.** This closes the nearest element that is still
open. Here it closes `liber`. The formatter writes `</>` when the
element is short enough that you can see what is being closed; it
writes the full name for long elements.

Here is a slightly bigger document, with a comment and a nested
chapter:

```stml
<liber>
  <!-- the front matter -->
  <titulus>De Agri Cultura</titulus>
  <caput numerus="1">
    <titulus>Praefatio</titulus>
    <pagina numerus="1">Est interdum praestare mercaturis rem quaerere.</pagina>
    <pagina numerus="2">Maiores nostri sic habuerunt.</pagina>
  </caput>
</liber>
```

Formatted:

```stml
<liber>
  <!-- the front matter -->
  <titulus(> De Agri Cultura
  <caput numerus="1" (((>
    <titulus(> Praefatio
    <pagina numerus="1" (> Est interdum praestare mercaturis rem
                           quaerere.
    <pagina numerus="2" (> Maiores nostri sic habuerunt.
</>
```

`(((>` with three parens means "the next three things are my
children". A long line of text is wrapped and hung under its first
word. The comment survived. Nothing about the meaning changed; only
the layout did.

The formatter is a tool you will use constantly, for two reasons.
It shows you what the parser understood, and it gives every file in
the repository the same shape.

### The three verbs

| verb | what it does |
|---|---|
| `stml formare <file>` | print the document tidied (nothing expanded) |
| `stml expandere <file>` | fill in templates and run patterns (chapters 7 to 12) |
| `stml vertere <file>` | turn the document into HTML (chapter 10) |

Each prints to standard output. Add `-ad out.stml` to write a file
instead (it refuses to overwrite its own input). `formare -probare`
prints nothing and exits 1 if the file is not already in its tidy
form, which is how a script checks a whole directory.

Exit codes: 0 means it worked, 1 means `-probare` found differences,
2 means the tool refused. A refusal always prints a message with the
file name, and where it can, a line and column.

### When it refuses

Save this and format it:

```stml
<r><p>a < b</p></r>
```

```
stml formare k.stml: k.stml:1:12: vitium
```

A bare `<` inside text is not allowed, because `<` is how a tag
starts. Chapter 4 shows how to write it. Notice the message: line 1,
column 12, and the word `vitium` (fault). Parser refusals are terse;
the tool tells you where, not always why. Template and pattern
refusals, which come later, are named.

---

## 3. Attributes

An attribute is a name and a value on an opening tag. There are
three ways to write one:

```stml
<pagina numerus=3 lingua="la" secreta>
  Text of the page.
</pagina>
```

- `lingua="la"` is the normal form: the value in double quotes.
- `numerus=3` is a bare value. Only letters, digits, `-` and `_` may
  appear in a bare value; anything else needs quotes.
- `secreta` alone is a **boolean** attribute. It means the attribute
  is present, with the value "true".

The formatter normalizes the first two:

```stml
<pagina numerus="3" lingua="la" secreta>Text of the page.</>
```

Single quotes are **not** allowed as value delimiters. `<x a='b'/>`
is a parse error. Only `"` delimits a value.

That raises a question: how do you put a `"` inside a value? You
cannot, inline. Instead an attribute can be written as an element,
with the `@` sign and an `=`:

```stml
<pagina numerus="3">
  <@nota=>He said "salve" and left.</>
  Text of the page.
</pagina>
```

`<@nota=>...</>` is an **attribute-element**. It sets the attribute
`nota` on `pagina` to the text inside it, and that text may contain
anything text may contain, including quotes. Programs reading the
document see `nota` as an ordinary attribute.

There is one rule: attribute-elements must come **first** among the
children, before any ordinary child. Put one after some text and the
parser refuses:

```
t11.stml:3:1: elementum attributi extra praefixum liberorum nec post vocationem templi (par. 6.3)
```

("attribute-element outside the prefix of the children and not after
a template call".) The rule exists so that the tool never has to
guess which element an attribute-element belongs to.

An empty attribute-element, `<@nota=/>`, means "this attribute is
explicitly absent". That looks pointless now; it matters in chapter 9,
where attributes flow down from a parent and you sometimes need to
say "not this one".

Attribute names follow the same rules as tag names, except that they
may not start with a dot.

---

## 4. Text: what is content and what is layout

### Special characters

Because `<` starts a tag and `&` starts an entity, text needs a way
to say those characters literally. STML understands five
**entities** in text:

| you write | you mean |
|---|---|
| `&lt;` | `<` |
| `&gt;` | `>` |
| `&amp;` | `&` |
| `&quot;` | `"` |
| `&apos;` | `'` |

That is the whole list. Any other `&word;` is not an entity and is
kept as the literal characters. A lone `&` is also fine. Try this:

```stml
<p>1 &lt; 2 &amp; 3 &gt; 2. AT&T. "quoted". &nbsp; stays literal.</p>
```

```stml
<p(> 1 &lt; 2 &amp; 3 &gt; 2. AT&amp;T. "quoted". &amp;nbsp; stays
     literal.
```

Three things to notice. The writer only ever produces `&lt;`, `&gt;`
and `&amp;`; a `"` is written bare. `AT&T` came back as `AT&amp;T`,
which reads back as the same `AT&T`. And `&nbsp;`, which is not one
of the five, came back as `&amp;nbsp;`: the parser had read it as the
literal six characters, and the writer escaped the `&` so that the
next reader sees the same six characters. If you want a non-breaking
space, type the actual character.

Entities are decoded only in text. Attribute values are taken
exactly as written, both when reading and when writing.

### Whitespace

Look again at the output above. The text was on one line in the
input and on two lines in the output, and the meaning did not
change. STML has one rule for this:

> Whitespace that contains a newline is **layout**. Whitespace on
> a single line is **content**.

So the newline and indentation between `<liber>` and `<titulus>` are
not part of the document's meaning; they are how it was laid out on
the page. But the space in `<p>salve <b>munde</b></p>` is content,
because it sits on one line between two pieces of text, and the
formatter will never remove it.

Inside ordinary text, a line break counts as a single space. The
formatter is therefore free to re-wrap prose to fit its width:

```stml
<pagina>
  Quae maxime laudabatur, ea erat: bonum agricolam bonumque
  colonum. Amplissime laudari existimabatur qui ita laudabatur.
</pagina>
```

```stml
<pagina(> Quae maxime laudabatur, ea erat: bonum agricolam bonumque
          colonum. Amplissime laudari existimabatur qui ita laudabatur.
```

This is the right default for prose. It is the wrong default for
code and poetry, where line breaks mean something. For those you
declare a different **kind** of text on the tag.

### Raw text: `<tag!>`

A `!` glued to the tag name makes the content **raw**: no tags are
recognised inside it, no entities are decoded, nothing is touched.

```stml
<code!>if (x < 10 && y) {}</code>
```

The `<` and `&&` are just characters. Raw content ends only at the
matching named closing tag, `</code>`; `</>` inside it is just text.

### Multi-line text: `<tag\>`

A `\` glued to the tag name says that line breaks are content. The
common indentation is removed, so you can indent the block to match
the file, and the relative indentation inside it survives:

```stml
<poema\>
  arma virumque cano
    Troiae qui primus
</>
```

The text of this element is `arma virumque cano\n  Troiae qui primus`.
The formatter re-indents the block to wherever it sits, without
touching its insides.

Two things are refused, each with a message that says why. An
interior line that is only spaces:

```
p.stml:1:1: linea interior spatio sola nec vacua in '<tag\>' (reassemblatio praefixi ambigua fieret)
```

And child elements inside a `\` block, which are reserved for later:

```
q.stml:1:1: liberi inlineares in '<tag\>' v2 reservati (textus solus licet)
```

Raw and multi-line combine as `<code!\>`, in that order only:

```stml
<code!\>
    int main(void) {
        return 0;
    }
</code>
```

### The four kinds, side by side

| form | line breaks are | tags and entities inside | formatter may |
|---|---|---|---|
| `<tag>` | a space | recognised | re-wrap the text |
| `<tag\>` | content | recognised (no child elements yet) | re-indent the block |
| `<tag!>` | content | not recognised | touch nothing |
| `<tag!\>` | content | not recognised | re-indent the block |

The kind is declared on the tag, so every tool that reads the
document agrees about what the line breaks mean. That is what makes
the formatter safe to run on any file.

---

## 5. Captures

You have seen `(>` in the formatter's output. Now use it yourself.

A **capture** lets an element take its neighbours as children
without wrapping them in a closing tag. There are three directions.

**Forward**, `<tag (>`: the next sibling becomes a child. Each extra
paren takes one more sibling.

**Backward**, `<) tag>`: the previous sibling becomes a child.
`<)) tag>` takes two.

**Sandwich**, `<= tag =>`: one sibling on each side.

Here is a page written with all three. The `vertere` verb (chapter
10) writes plain nested HTML, so it is the clearest way to see what
tree the captures built:

```stml
<html>
<section (>
<h1>Titulus</h1>
<p>Not captured.</p>
<ul (((>
<li>unum</li>
<li>duo</li>
<li>tria</li>
<p>After the list.</p>
<h2>A</h2>
<p>B</p>
<)) article>
<em>ante</em>
<= span =>
<b>post</b>
</html>
```

```
stml vertere u1b.stml
```

```html
<!DOCTYPE html>
<html>
<section><h1>Titulus</h1>
</section>
<p>Not captured.</p>
<ul><li>unum</li>
<li>duo</li>
<li>tria</li>
</ul>
<p>After the list.</p>
<article><h2>A</h2>
<p>B</p>
</article>
<span><em>ante</em>
<b>post</b>
</span>
</html>
```

`section (>` took one sibling, the `h1`. `ul (((>` took three. The
`article` came after its two children and pulled them in with `))`.
The `span` took one on each side. Comments do not count as siblings
for capturing.

There is also a space rule you met in chapter 2: after `(>`, a space
on the same line is a separator, not text. `<a (> <b/>` captures `b`,
not a lone space.

### Raw line capture: `<tag! (>`

A raw tag with a single forward capture takes **the rest of the
line** as raw text. No tags, no entities, no quotes needed, up to the
end of the line. The repository's build description uses this for
compiler flags and paths:

```stml
<aedilis>
  <inclusa>
    <via (>include
    <via (>probationes
  </inclusa>
  <vexilla>
    <vexillum! (>-Wall
    <vexillum! (>-Wsign-conversion
  </vexilla>
</aedilis>
```

(That is a cut-down copy of `aedilis.stml` in the repository root.)
`<via (>include` is an ordinary forward capture of a text; the text
runs to the end of the line. `<vexillum! (>-Wall` is the raw form:
`<` and `&` on the rest of that line would be taken literally.

### The formatter and captures

The formatter uses capture forms whenever they make a file shorter
and still readable: a chain of single children becomes
`<a(> <b(> <c(> text`, two or three children become `((>` or `(((>`
stacked on the following lines. When you read a formatted file, read
`(` as "child follows". Captures change how a file is written, never
what it means, and the formatter is allowed to change one form into
the other.

---

## 6. Fragments, references, and three kinds of name

### Fragments

A **fragment** is a bag of children with no element of its own:

```stml
<#nav>
  <a>Home</a>
  <a>Books</a>
</#>
```

`<#nav>` opens a fragment named `nav`; `</#>` closes it (the closing
tag never repeats the name). A program reading the document sees the
two `a` elements grouped, but there is no `nav` element in the tree.

Fragments exist so that a document can name a piece of itself and
point at it from elsewhere:

```stml
<<#nav>>
```

`<<#nav>>` is a **transclusion**: "the content of `nav` goes here".
It is a pointer, not a copy. STML records the pointer and leaves
resolving it to whatever program reads the document, because
different programs want different things from a pointer. The `stml`
tool keeps it as written; `vertere` refuses it, since HTML has no
such thing:

```
stml vertere l.stml: l.stml: TRANSCLUSIO ('#nemo')
```

You will not use transclusion much while learning. What you will use
is its cousin with arguments, the template, in chapter 7.

### Anonymous fragments are not for grouping

A fragment can have no name: `<#>...</#>`, or the shorter `<>...</>`,
or `<(>` which is an anonymous fragment that captures the next
sibling. These look like a handy way to group things. **They are
not.** Under a named parent, an anonymous fragment means "repeat the
parent for each of these" (chapter 9), and mixing one with ordinary
siblings is refused:

```stml
<r><a/><(><i>x</i></r>
```

```
distributio MIXTA (involucrum 'r')
```

Use a named fragment or a real element to group.

### Three kinds of name

STML reserves three symbols for names that point at something.
You will meet them in the repository's documents; you do not need
them for this tutorial.

- `#nav`: a name inside **this document** (fragments and, in
  attributes, anchors).
- `&laika;`: a name for an **individual** in the world, written in an
  attribute value: `<canis nomen="&laika;">`. The `;` is what makes
  it safe in prose: `AT&T` has no `;` and stays literal.
- `.species`: a name for a **kind**, either as a reference in an
  attribute or as a tag that mints a kind: `<.species>`. A tag may
  start with one dot; an attribute name may not.

There is also `<% &laika;> ... </%>`, which adds content to an
individual defined in a library. These belong to the canon and
natura layers, which have their own documentation.

---

## 7. Templates

Now the second layer. Suppose a page shows the same little card many
times:

```stml
<div class="card" data-n="silva">
  <h1>silva</h1>
  <p>parsator</p>
</div>
```

You would rather write the shape once and the content many times.
A **template** is a fragment whose name starts with `@` and whose
opening tag declares **slots**:

```stml
<#@card n="@n" corpus="@corpus">
  <div class="card" data-n="&@n;">
    <h1>&@n;</h1>
    &@corpus;
  </div>
</#>
```

Read the opening tag as a list of slots. `n="@n"` declares a slot
called `n`; `corpus="@corpus"` declares one called `corpus`. The
house convention is to give the attribute and the slot the same name,
as here, so you never have to remember which is which. (Strictly it
is the part after the `@` that names the slot.)

Inside the body, `&@n;` means "put the value of slot `n` here". It
works inside an attribute value, inside text, and on its own as a
whole child.

A **call** looks like a transclusion with attributes:

```stml
<<#@card n="silva">><@corpus=><p>parsator</p><p>C89</p></>
```

`n="silva"` fills slot `n` with the text `silva`. The `<@corpus=>`
right after the call is an attribute-element again, this time meaning
"argument for slot `corpus`", and its value is everything inside it,
elements included. So a slot can hold a whole subtree, not just a
string. A call consumes every attribute-element that immediately
follows it.

Run it with `expandere`:

```stml
<r>
<#@card n="@n" corpus="@corpus">
  <div class="card" data-n="&@n;">
    <h1>&@n;</h1>
    &@corpus;
  </div>
</#>
<<#@card n="silva">><@corpus=><p>parsator</p><p>C89</p></>
<<#@card n="natura">><@corpus=>solum textus</>
</r>
```

```stml
<r>
  <div class="card" data-n="silva" (((>
    <h1(> silva
    <p(> parsator
    <p(> C89

  <div class="card" data-n="natura"><h1>natura</h1>solum textus</>
</>
```

The definition is gone from the output; only the filled-in cards
remain. The file on disk keeps the template. Expansion is something a
reader does, never something that rewrites your file.

### The rules, and what breaks them

Every mistake is refused with a name. These are the ones you will
make first.

**Define before you call.** A call may only use a template defined
earlier in the file:

```
t12.stml:2: FRAGMENTUM_POSTERIUS (fragmentum '@card')
```

**Arguments name slots, not attributes.** With
`<#@card nomen="@n">`, calling `<<#@card nomen="x">>` fails, because
the slot is `n`:

```
u2.stml:3: ARGUMENTUM_SUPERFLUUM (fragmentum '@card') (loculus 'nomen')
```

Mirror the names (`nomen="@nomen"`) and the mistake cannot happen.

**Fill every slot.** A call that leaves a declared slot empty:

```
u4.stml:4: LOCULUS_NON_IMPLETUS (fragmentum '@card') (loculus 'n')
```

**One value per slot.** Giving `m` both inline and as
`<@m=>...</>` is `ARGUMENTUM_GEMINUM`.

**A subtree cannot go into a string.** If `corpus` holds elements,
writing `title="&@corpus;"` is `ARGUMENTUM_ARBOREUM`. A subtree may
only be spliced where children go.

**Optional slots.** Declare a slot as `m="@m?"` and a call may omit
it. But the body may then only mention `&@m;` inside a branch that
has checked it is present, which is the next chapter.

**Explicit absence.** `<@m=/>` (an empty argument) says "I am not
giving `m`". For a required slot that is `LOCULUS_NON_IMPLETUS`; for
an optional one it is the same as leaving it out.

---

## 8. Choosing inside a template

### `COMMUTATIO`: pick a branch by a value

Templates cannot compute, on purpose: the meaning of a document has
to be visible on the page. What they can do is **choose** between
branches written out in full. The tags that do this are in CAPITAL
letters, so that you can always tell a built-in from your own tags.

```stml
<r>
<#@card n="@n" m="@m?" corpus="@corpus">
  <div class="card" data-n="&@n;">
    <COMMUTATIO de="&@m;">
      <CASUS est="alta"><h1>&@n; (&@m;)</h1></CASUS>
      <CASUS nihil><h3>&@n;</h3></CASUS>
      <CASUS non-nihil><h2>&@n; (&@m;)</h2></CASUS>
    </COMMUTATIO>
    &@corpus;
  </div>
</#>
<<#@card n="silva" m="alta">><@corpus=><p>parsator</p><p>C89</p></>
<<#@card n="natura">><@corpus=>solum textus</>
<<#@card n="pipe">><@corpus=>x</><@m=>via-bloco</>
<<#@card n="tomb">><@corpus=>y</><@m=/>
</r>
```

```stml
<r>
  <div class="card" data-n="silva" (((>
    <h1(> silva (alta)
    <p(> parsator
    <p(> C89

  <div class="card" data-n="natura"><h3>natura</h3>solum textus</>

  <div class="card" data-n="pipe"><h2>pipe (via-bloco)</h2>x</>

  <div class="card" data-n="tomb"><h3>tomb</h3>y</>
</>
```

`COMMUTATIO de="&@m;"` (switch on `m`) tries its `CASUS` (case)
children in order and takes the first that fits. A case can test
three things and nothing else:

- `est="alta"`: the value is exactly this text.
- `nihil`: the slot was not given.
- `non-nihil`: the slot was given, whatever its value.

`ORDINARIUS` (default) is a case that always fits; it must be last.
If nothing fits and there is no default, that is `CASUS_NULLUS`.
Notice that `<@m=/>` counted as "not given" and reached the `nihil`
branch.

Why only literals and presence, never `greater than`? Because a
schema (chapter 13) can then check every branch of every template
before anything runs. Add expressions and that becomes impossible.

An optional slot may only be used inside a case that proved it is
present: `est` and `non-nihil` prove it, `nihil` and `ORDINARIUS` do
not. Write `&@m;` inside `ORDINARIUS` and the template is refused at
definition time with `LOCULUS_NON_ANGUSTATUS`. That sounds fussy.
It is what guarantees a filled template never silently contains an
empty hole.

### Reaching into a subtree argument

When a slot holds a subtree, the body can look inside it.

```stml
<r>
<#@h n="@n">
  <COMMUTATIO de="&@n;">
    <CASUS><EST><EXEMPLAR><cap><gradus>1</gradus><t>$t</t></cap></EXEMPLAR></EST><h1 data-t="&@t;">&@n.corpus;</h1></CASUS>
    <CASUS tag="cap"><h2>&@n.corpus;</h2> [&@n.gradus;] {&@n.codex!;}</CASUS>
    <CASUS tag="lista"><ul><PER de="@n.items" ut="i"><li>&@i;</li></PER></ul></CASUS>
    <ORDINARIUS><em>?</em></ORDINARIUS>
  </COMMUTATIO>
</#>
<<#@h>><@n=><cap><gradus>1</gradus><t>Titulus</t><corpus>Salve <b>munde</b></corpus><codex!>a<b</codex></cap></>
<<#@h>><@n=><cap><gradus>2</gradus><t>Alter</t><corpus>secundus</corpus><codex!>x&y</codex></cap></>
<<#@h>><@n=><lista><items><i>1</i><i>2</i></items></lista></>
<<#@h>><@n=><alius/></>
</r>
```

```stml
<r><h1 data-t="Titulus">Salve <b>munde</b></h1><h2>secundus</h2> [2] {x&amp;y}<ul><li><i>1</i></li><li><i>2</i></li></ul><em>?</em></>
```

Four new pieces, one per case:

- **`<CASUS tag="cap">`** fits when the argument's root element is
  a `cap`. It is a test on the tag name only.
- **`<CASUS><EST><EXEMPLAR>...</EXEMPLAR></EST>arm</CASUS>`** fits
  when the argument matches a **pattern**. Patterns are chapter 11;
  here the pattern says "a `cap` whose `gradus` says 1, and whose
  `t` text I want to keep as `$t`". The captured `$t` is then usable
  in the arm as `&@t;`.
- **`&@n.corpus;`** is a **slot projection**: the content of the
  `corpus` child of the argument. If that child holds only text you
  get the text; otherwise you get its children. `&@n.gradus;` gave
  the text `2`. Paths chain, `&@n.a.b;`, one child name per step.
- **`&@n.codex!;`** with `!` reads the bytes of a raw child. Without
  the `!`, raw content is still an element and would be refused in a
  text position.
- **`<PER de="@n.items" ut="i">...</PER>`** repeats its body once
  for each element inside `items`, calling the current one `i`.

`PER` can also hand each element to another template instead of an
inline body: `<PER de="@n.items" voca="#@item"/>` calls `#@item`
once per element, filling its single subtree slot. This is how a
template walks a tree: it calls itself on each child. The tool only
allows such a self-call when the argument is strictly inside the
current one, so the walk must end:

```
s.stml:2: RECURSIO_NON_DESCENDENS (fragmentum '@f') (loculus 'n')
```

The markdown-to-HTML program in `md/html/md-html.stml` is one
template of about forty cases built exactly this way. Read it once
you have finished this tutorial; it is the largest real example in
the repository.

---

## 9. Lists without repeating yourself

The third layer of expansion is `DISTRIBUTIO`, and it needs no
template at all. Write a wrapper element whose children are all
anonymous fragments:

```stml
<r>
<ul class="lista" data-x="1">
  <> alpha </>
  <><@data-x=/> beta </>
  <>
    <> gamma-nested </>
    <> delta-nested </>
  </>
  <><@data-x=>3</> eps </>
  <><@class=>own</> zeta </>
</ul>
<nota t="a"><>emere lac</><>ianuam</></>
</r>
```

```stml
<r>
  <ul class="lista" data-x="1"> alpha </>
  <ul class="lista"><@data-x=/> beta </>

  <ul class="lista" data-x="1"> gamma-nested </>
  <ul class="lista" data-x="1"> delta-nested </>
  <ul class="lista"><@data-x=>3</> eps </>
  <ul data-x="1"><@class=>own</> zeta </>
  <nota t="a" (> emere lac
  <nota t="a" (> ianuam
</>
```

The wrapper dissolves and each item becomes a copy of it. The rules:

- The wrapper's attributes flow down to every item.
- An item's own attribute wins over the wrapper's. `<@class=>own</>`
  replaced the class; `<@data-x=>3</>` replaced the number.
- `<@data-x=/>`, the explicit absence from chapter 3, **deletes** the
  inherited attribute, and stays in the output so that the deletion
  is visible.
- Nested anonymous lists flatten into the same kind.

The same works in HTML with a real `li`, which chapter 10 shows.

Two traps, both refused as `distributio MIXTA`:

- **Spaces between items on one line are text.** `<> a </> <> b </>`
  has a text node between the items, and text is not an item. Put
  each item on its own line or glue them: `<>a</><>b</>`.
- **An anonymous fragment beside a named sibling.** See chapter 6.

---

## 10. Making a web page

`stml vertere` (to turn) writes HTML. The idea is simple: you write
HTML's tags in STML's syntax, and the tool handles the few places
where the two syntaxes disagree. It has no idea what `div` or `h1`
mean; the tags are yours.

```stml
<html lang="la">
<head>
  <meta charset="utf-8"/>
  <title>Instrumenta</title>
  <style!>
    body  { font: 14px/1.5 ui-monospace, monospace; }
    .card { border: 1px solid #ddd; padding: 1rem; }
  </style>
</head>
<body>
  <h1>Instrumenta</h1>

  <#@card nomen="@nomen" munus="@munus">
    <div class="card">
      <h3>&@nomen;</h3>
      <p>&@munus;</p>
    </div>
  </#>

  <<#@card nomen="silva"  munus="parsator C89">>
  <<#@card nomen="natura" munus="genera">>

  <ul>
    <li><>unum</><>duo</><>tria</></li>
  </ul>
  <input type="checkbox" checked/>
</body>
</html>
```

```html
<!DOCTYPE html>
<html lang="la">
<head>
  <meta charset="utf-8">
  <title>Instrumenta</title>
  <style>
    body  { font: 14px/1.5 ui-monospace, monospace; }
    .card { border: 1px solid #ddd; padding: 1rem; }
  </style>
</head>
<body>
  <h1>Instrumenta</h1>

    <div class="card">
      <h3>silva</h3>
      <p>parsator C89</p>
    </div>

    <div class="card">
      <h3>natura</h3>
      <p>genera</p>
    </div>

  <ul><li>unum</li><li>duo</li><li>tria</li>  </ul>
  <input type="checkbox" checked>
</body>
</html>
```

Everything from chapters 7 to 9 ran first (templates filled, the
`li` list distributed), then the HTML was written. What `vertere`
does on the way:

- Writes `<!DOCTYPE html>` itself. Your file carries no doctype.
- Closes elements the HTML way: `br`, `img`, `meta`, `input` and the
  other void elements are written bare; everything else gets a real
  closing tag, never `<div/>`, which a browser would read as an
  unclosed `div`.
- Keeps the spaces between elements, so `x y` stays `x y`.
- Escapes text (`&`, `<`, `>`) and leaves inline attribute values as
  you wrote them.
- Copies `<script!>` and `<style!>` content byte for byte, because a
  browser reads those raw. Any other raw tag, such as `<pre!>`, is
  meant as literal text and is escaped: `<pre!>a < b</pre>` becomes
  `<pre>a &lt; b</pre>`.
- Judges nothing about HTML validity. Two `<body>` elements are
  written as two `<body>` elements.

And what it refuses, each with a name:

| you wrote | refusal | do this instead |
|---|---|---|
| `<script>` or `<style>` without `!` | `CRUDUS_DEEST` | add the `!`; without it the code was parsed as markup |
| `&nbsp;`, `&#10;`, any `&word;` in text | `ENS_AMBIGUUM` | type the character itself |
| `<a href>` or `href="true"` on a non-boolean attribute | `ATTRIBUTUM_VERUM` | give a real value, or `<@href=>true</>` |
| `<<#x>>` | `TRANSCLUSIO` | use a template call |
| `<.x>`, `<% &k;>`, `<?...?>` | named refusals | these have no HTML meaning |

The `&nbsp;` refusal deserves a word. `&nbsp;` and `&amp;nbsp;` parse
to the same six characters (chapter 4), so the tool cannot know
whether you meant a space or the literal text. Rather than guess, it
refuses. UTF-8 characters are always fine.

One escape hatch exists for real HTML you already have: content
inside `<crudum!>...</crudum>` is copied out with no tag around it.
The markdown converter uses it for HTML blocks.

---

## 11. Asking a document questions

The third layer. A **pattern** is an element written the way you
would write the thing you are looking for, with `$names` where you
want to keep what you found.

```stml
<bibliotheca>
  <liber id="cato" auctor="Cato"><titulus>De Agri Cultura</titulus></liber>
  <liber id="varro" auctor="Varro"><titulus>De Re Rustica</titulus></liber>
  <liber id="columella" auctor="Columella"><titulus>De Re Rustica</titulus></liber>
  <recensio de="cato">Utilis.</recensio>
  <recensio de="varro">Longa.</recensio>
</bibliotheca>

<EXEMPLAR output="$libri"><liber auctor="$a"><titulus>$t</titulus></liber></EXEMPLAR>

<index>
  <PER congruentia="$libri"><res>&@a;: &@t;</res></PER>
</index>
```

```stml
<bibliotheca>
  <liber id="cato" auctor="Cato" (> <titulus(> De Agri Cultura
  <liber id="varro" auctor="Varro" (> <titulus(> De Re Rustica
  <liber id="columella" auctor="Columella" (> <titulus(> De Re Rustica
  <recensio de="cato" (> Utilis.
  <recensio de="varro" (> Longa.
</>

<index(((>
  <res(> Cato: De Agri Cultura
  <res(> Varro: De Re Rustica
  <res(> Columella: De Re Rustica
```

Read `EXEMPLAR` (pattern) as a question. "Find every `liber` that has
an `auctor` attribute, call its value `$a`, and that has a `titulus`
child whose text I will call `$t`." The answer is a table of
**rows**, one per match, each row holding the captures. The table is
named by `output="$libri"`.

`PER congruentia="$libri"` (for each match) then writes its body once
per row. The captures arrive as if they were template arguments, so
inside `PER` you write `&@a;` just as in a template. There is no new
way to refer to things: `$x` catches on the pattern side, `&@x;`
uses on the writing side.

The matching rules, in one breath:

- What you write is required; what you leave out is free. The
  pattern above says nothing about `id`, so books with or without an
  `id` match.
- Children must appear in the same order, but other children may
  sit between them.
- `$a` in a value position keeps the value. `$t` as the whole text of
  an element keeps the text. `$x` written as a bare attribute,
  `<liber $x/>`, keeps the whole element.
- The same `$name` twice means "these must be equal".
- `<*/>` matches any one element. `<**>...</**>` around an element
  means "this appears somewhere below, not necessarily as a direct
  child".
- The pattern is tried at every element of the document, unless you
  say `ancorata` (anchored), which tries it only at the root of what
  it was given.

A pattern lives at the top level of a document and only sees what
came **before** it in the file. That, and the rule that a pattern's
output must be used further down, are what keep the whole thing
from ever looping.

### How many

`modus` (mode) says how many matches you expect:

| `modus=` | meaning | if wrong |
|---|---|---|
| absent, or `omnia` | all of them, in document order | never wrong; zero rows is fine |
| `unum` | exactly one | `UNUM_VIOLATUM` |
| `primum` | the first | zero is fine |
| `optional` | zero or one | `OPTIONAL_MULTIPLEX` |

```stml
<EXEMPLAR modus="unum" output="$primus"><liber id="cato" auctor="$a"/></EXEMPLAR>
<EXEMPLAR output="$rustica"><liber auctor="$a"><**><titulus>De Re Rustica</titulus></**></liber></EXEMPLAR>
```

run over the two-book library gives one row for `$primus` and, via
`<**>`, the one book whose title is De Re Rustica somewhere inside.

### Two refusals you will meet

A pattern nobody uses:

```
j.stml:1: EXITUS_NON_CONSUMPTUS (loculus '$u')
```

A `unum` that found none or several:

```
i.stml:1: UNUM_VIOLATUM (loculus '$u')
```

### Telling a pattern what to ignore

Some documents carry bookkeeping elements that a pattern should not
look inside, such as the position and origin wrappers in the
projected C syntax trees. `TRANSPARENTIA` (transparency) declares
them once at the top of the file:

```stml
<TRANSPARENTIA tags="expansio pasta stringificatio" attributa="linea columna"/>
```

Patterns then see through those tags and ignore those attributes.
Without this, a search for the identifier `NULL` in a C tree would
also match the `NULL` spelled inside the token's own expansion
record.

---

## 12. Chains, absences, and decisions

### `CATENA`: one question after another

Questions compose. Put patterns in a `CATENA` (chain) and each one
runs over the rows the previous one produced:

```stml
<bibliotheca>
  <liber id="cato" auctor="Cato"><titulus>De Agri Cultura</titulus></liber>
  <liber id="varro" auctor="Varro"><titulus>De Re Rustica</titulus></liber>
  <liber id="columella" auctor="Columella"><titulus>De Re Rustica</titulus></liber>
  <recensio de="cato">Utilis.</recensio>
  <recensio de="varro">Longa.</recensio>
</bibliotheca>

<INDAGO/>
<CATENA output="$sine-recensione">
  <(><EXEMPLAR><bibliotheca/></EXEMPLAR>
  <(><EXEMPLAR radix="fontis"><liber id="$id" auctor="$a"/></EXEMPLAR>
  <(><SINE><recensio de="&@id;"/></SINE>
</CATENA>

<relatum>
  <PER congruentia="$sine-recensione"><desideratur auctor="&@a;" liber="&@id;"/></PER>
</relatum>
```

```stml
<bibliotheca>
  <liber id="cato" auctor="Cato" (> <titulus(> De Agri Cultura
  <liber id="varro" auctor="Varro" (> <titulus(> De Re Rustica
  <liber id="columella" auctor="Columella" (> <titulus(> De Re Rustica
  <recensio de="cato" (> Utilis.
  <recensio de="varro" (> Longa.
</>
<!--indago CATENA $sine-recensione (radix); 1 EXEMPLAR bibliotheca radix->1; 2 EXEMPLAR liber 1->3 (capturae: id a); 3 SINE recensio 3->1 (necati 2); exitus 1-->

<relatum>
  <desideratur auctor="Columella" liber="columella"/>
  <!--indago PER $sine-recensione: ordines 1-->
</>
```

The question was "which books have no review?", and the answer is
Columella. Step by step:

1. Find the library. One row.
2. Inside that row, find every book, keeping its `id` and author.
   Three rows. `radix="fontis"` says "keep the library as the row's
   root, do not narrow to the book", which matters for the next step.
3. `SINE` (without) **removes** every row for which its pattern finds
   something inside the row's root. `&@id;` is filled in per row, so
   for the Cato row it looks for `<recensio de="cato"/>`, finds one,
   and drops the row. Only Columella survives.

The `<(>` in front of each link is just decoration that keeps the
lines short; the chain would read the same without it.

Every capture a row already has stays with it. That is why the final
report can print both `&@a;` and `&@id;`. It also means a later
pattern may not re-capture a name the row already carries; that is
refused as `CAPTURA_COLLISA`.

**`INDAGO`** (investigation) at the top of the file made the tool
leave a comment at every step saying what it did: how many rows came
in, how many went out, what was captured, what `SINE` killed. Remove
the `<INDAGO/>` line and the comments disappear. When a chain gives
you the wrong answer, this is the first thing to reach for.

### `DIRIBITIO`: decide by a condition

`DIRIBITIO` (the sorting of ballots) chooses between chains. Each
`CASUS` has a condition in `<EST>` and an arm; the first case whose
condition finds anything wins, and the arm runs. `ORDINARIUS` runs if
none did.

```stml
<INDAGO/>
<bibliotheca>
  <liber id="cato" auctor="Cato" annus="-160"/>
  <liber id="varro" auctor="Varro" annus="-37"/>
  <liber id="columella" auctor="Columella" annus="65"/>
</bibliotheca>

<EXEMPLAR output="$libri"><liber id="$id"/></EXEMPLAR>

<DIRIBITIO de="$libri" output="$verdictum">
  <CASUS angustans>
    <EST><EXEMPLAR><liber auctor="Cato"/></EXEMPLAR></EST>
    <EXEMPLAR ancorata><liber auctor="$a"/></EXEMPLAR>
  </CASUS>
  <ORDINARIUS><EXEMPLAR ancorata><*/></EXEMPLAR></ORDINARIUS>
</DIRIBITIO>

<relatum><PER congruentia="$verdictum"><v id="&@id;" auctor="&@a;"/></PER></relatum>
```

```stml
<!--indago EXEMPLAR $libri (radix): ordines 3 (capturae: id)-->
<!--indago DIRIBITIO $verdictum (de $libri 3): CASUS 1 conditio 1 SUMPTUM; bracchium angustatum; EXEMPLAR liber 1->1; exitus 1-->

<relatum>
  <v id="cato" auctor="Cato"/>
  <!--indago PER $verdictum: ordines 1-->
</>
```

Three books came in. The condition "is there a book by Cato?" found
one row, so the first case was taken. `angustans` (narrowing) says
the arm receives only the rows that satisfied the condition, rather
than all three; without it the arm would see all the incoming rows.
The arm is a pattern anchored at the row's root that captures the
author.

Change the condition to look for an author called `Nemo` and the
default runs instead:

```stml
<!--indago DIRIBITIO $verdictum (de $libri 3): CASUS 1 conditio 0 praeteritum; ORDINARIUS SUMPTUM; bracchium; EXEMPLAR * 3->3; exitus 3-->

<relatum>
  <v id="cato" auctor="&@a;"/>
  <v id="varro" auctor="&@a;"/>
  <v id="columella" auctor="&@a;"/>
  <!--indago PER $verdictum: ordines 3-->
</>
```

`<EXEMPLAR ancorata><*/></EXEMPLAR>` is the identity: every row maps
to itself. Two things to learn from the second output. The
`ORDINARIUS` arm captured nothing, so `&@a;` in the report had
nothing to fill it and was left as literal text; a report should only
mention captures every arm provides. And the trace tells you exactly
which arm ran, which is what `INDAGO` is for.

### A real one

The repository lints its own C code this way. Silva, the C parser,
projects every source file into STML, and this document, from
`silva/probationes/fixa/exemplaria/inutilis_localis.stml`, finds
variables that are declared and never used:

```stml
<TRANSPARENTIA tags="expansio pasta stringificatio api extentum ante post regio-cruda" attributa="f b linea columna def-f def-l def-c"/>
<CATENA output="$inutilia">
  <(><EXEMPLAR><definitio-functionis/></EXEMPLAR>
  <(><EXEMPLAR radix="fontis"><declaratio><declaratores><**><declarator-titulus><tok_titulus><lex-identificator $n/></tok_titulus></declarator-titulus></**></declaratores></declaratio></EXEMPLAR>
  <(><SINE><folium-identificator><tok_valor>&@n;</tok_valor></folium-identificator></SINE>
</CATENA>
<relatum lint="variabilis-inutilis">
  <PER congruentia="$inutilia"><situs>&@n;</situs></PER>
</relatum>
```

Same three moves as the library: find each function, find each
declared name inside it (keeping the function as the row's root),
drop the rows where that name is used somewhere in the function.
Its answers are checked, file by file, against the C analyzer that
does the same job in C, and they agree exactly.

---

## 13. Contracts

A document format is only useful if the documents obey it. A
**canon** is a description of a format, written in STML, that the
tool `canon_examen` uses to judge documents. It says which elements
exist, which attributes they take and of what kind, which children
they may have and how many, and which values must be unique or must
refer to something.

A document can carry its own canon as the first child of its root.
Here is a book format with a few deliberate mistakes in the content:

```stml
<liber>
  <canon dialectus="liber" versio="1">
    <elementum nomen="liber" radix="verum">
      <liberum nomen="caput" minimum="1" maximum="1"/>
      <liberum nomen="pagina"/>
    </elementum>
    <elementum nomen="caput" textus="verum">
      <attributum nomen="lingua" genus="electio" necessarium="verum">
        <optio>la</optio><optio>en</optio>
      </attributum>
    </elementum>
    <elementum nomen="pagina" textus="verum">
      <attributum nomen="numerus" genus="numerus" minimum="1" maximum="999"/>
      <attributum nomen="ad" genus="nomen"/>
    </elementum>
    <unicitas nomen="paginae" attributum="numerus" super="pagina"/>
    <citatio nomen="renvoi" attributum="ad" ad="pagina/numerus"/>
  </canon>
  <caput lingua="la">Titulus</caput>
  <pagina numerus="1">prima</pagina>
  <pagina numerus="2" ad="1">secunda</pagina>
  <pagina numerus="2" ad="9" color="ruber">tertia</pagina>
  <pagina numerus="1000">quarta</pagina>
  <pagna numerus="5"/>
</liber>
```

```
bin/canon_examen cn.stml
cn.stml:22: <pagina> attributum extra canonem: color [infixus]
cn.stml:23: <pagina> valor extra fines declaratos: numerus [infixus]
cn.stml:24: <liber> liberum hic non licet: pagna [infixus]
cn.stml:24: <pagna> elementum extra canonem: an pagina? [infixus]
cn.stml:22: <pagina> nomen bis in spatio unico: 2 [infixus]
cn.stml:22: <pagina> citatio non resoluta (clavis absens in scopo): 9 [infixus]
canon_examen: plagulae 1 / VITIA 6
```

Every mistake was found and named: an unknown attribute, a number
out of range, an unknown element (with a guess at what was meant),
a duplicated page number, and a reference to a page that does not
exist. Exit code 1 means faults were found; 0 means clean; 2 means
nothing was judged at all, which is treated as a failure so that a
missing canon can never pass silently.

The vocabulary of a canon:

- `<elementum nomen="x">` declares an element; `radix="verum"` marks
  the root; `textus="verum"` allows text inside.
- `<attributum nomen="a" genus="...">` declares an attribute. Kinds:
  `textus` (anything), `nomen` (an identifier), `numerus`, `veritas`
  (`verum`/`falsum`), `dies` (a date), `electio` (one of the listed
  `<optio>`), `compositum` (a kebab name). `necessarium="verum"`
  requires it; `minimum`/`maximum` bound a number.
- `<liberum nomen="c" minimum= maximum=>` declares an allowed child
  and how many.
- `<unicitas attributum= super=>` says a value must be unique across
  the named elements.
- `<citatio attributum= ad="elem/attr">` says a value must equal some
  declared key elsewhere in the document.

There is no expression language in a canon and there never will be.
Anything a canon cannot say (such as "this attribute implies that
one") is checked by the program that reads the format.

Most formats in the repository keep their canon in a separate
`.canon` file, listed in `canones.registrum` at the root, keyed by
the document's root element. `canon.canon` describes the canon
language itself and is used to judge every other canon, including
itself.

---

## 14. Where to go next

- `docs/stml-reference.md`: every construct, every refusal, every
  rule, ordered for lookup rather than for learning.
- `include/stml.h`: the C API for reading, walking, building and
  writing documents. `include/stml_macros.h` for expansion,
  `include/canon.h` for judging, `include/stml_html.h` for HTML.
- `project-specs/stml-visio.md`: why STML is shaped the way it is,
  including the table of every symbol it reserves and what became of
  older ideas.
- The specs behind each layer: `stml-trivia-spec.md` (text and
  whitespace), `stml-macros-spec.md` (templates),
  `stml-exemplaria-spec.md` (patterns), `stml-instrumentum-spec.md`
  (the tool and HTML), `md-arbor-spec.md` section 6 (slot
  projections and `PER` over subtrees).
- `md/html/md-html.stml`: the largest real template program.
- `silva/probationes/fixa/exemplaria/`: the real lint documents.

`docs/old/stml.md` is an earlier introduction. It predates most of
what this tutorial covers and is kept for history only.
