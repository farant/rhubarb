# 1x09 A-PLOT / TECHNICAL PITCH
## "Arbor Documentorum" (Tree of Documents)

**Library:** STML (Structured Text Markup Language — XML superset with DOM tree)
**Dependencies:** Piscina (1x01), Chorda (1x03), ChordaAedificator (1x03), Xar (1x04), Internamentum (1x06)
**Theme:** Flat is dead. Hierarchy is how the world actually organizes itself. A document that describes its own structure is a document that can be understood by anyone — even centuries later.
**Dramatic Engine:** The data has outgrown flat files. Pliny's encyclopedia has 37 books, each with chapters, each with sections, each with observations. The census has regions, districts, families, members. Joe's Watcher Chronicles have subjects, timelines, entries. All of this is stored as flat text — lines in a file. The structure exists in the *reader's head*, not in the *document itself*. STML makes the implicit structure explicit. The tree is born.

---

## COLD OPEN

**Location:** Pliny's villa. Morning. Pliny is at his desk with three documents open on screen — his encyclopedia entry on Vesuvian minerals, the census record for district seven, and Joe's Chronicle entry on Duncan MacLeod. He is trying to cross-reference them.

The problem is visible before anyone speaks. Each document is a flat text file — lines of characters. No headers, no sections, no categories. Finding where "Vesuvian obsidian" is mentioned in the census requires reading the entire census file. Finding whether Joe's Chronicle mentions the same geological period requires reading the entire Chronicle.

Pliny has developed his own system — handwritten marginalia on a separate wax tablet, noting line numbers. A human index layered on top of a machine that has no index. The tablet is covered in his meticulous hand: "Obsidianus — census lin. 2847, encyclopedia lin. 491, Chronica lin. 318."

He adds another entry. His stylus slips. The wax smears. Three hours of cross-referencing, gone.

**PLINY:** *(staring at the smeared wax)* The text has structure. I can see it. A mineral entry contains: name, location, properties, uses, historical observations. The census entry contains: name, district, family, occupation. These are not lists. They are... categories within categories. Branches of a tree.

He picks up a scroll — volume twelve of the Naturalis Historia, the one about trees. Opens it to a diagram: a tree, labeled with genus and species. Roots, trunk, branches, leaves.

**PLINY:** A tree. Each branch contains its children. Each child knows its parent. The root contains everything.

He sets the scroll next to the screen showing the flat text file. The juxtaposition is devastating — the Roman polymath drew the data structure two thousand years ago but the computer doesn't know it.

**PLINY:** *(to himself, writing on a fresh tablet)* Arbor documentorum. The tree of documents.

SMASH TO MAIN TITLES.

---

## ACT ONE: "Radix" (The Root)

*The problem crystallizes. Flat files fail. The tree is proposed. The first markup is written.*

### Beat 1: Pliny's Presentation to the Team

**Location:** The Tabularium. Morning session. Pliny has brought his scroll, his smeared wax tablet, and his frustration.

**PLINY:** I have been indexing your flat files by hand. Line numbers. Cross-references. Marginalia on wax — the same method I used in Rome for thirty years. *(beat)* The wax smeared this morning. Three hours of work. But that is not the problem. The problem is that I should not need to index your files at all. The files should index themselves.

He opens three documents on the board:

```
Census — Regio VII:
Gaius Aurelius Ferrarius, faber, familia 4
  Aurelia Tertia, uxor
  Marcus Aurelius Minor, filius, aetas 12
  Gaius Aurelius Parvus, filius, aetas 7
Lucius Valerius Priscus, senator, familia 3
  Valeria Prima, uxor
  Lucius Valerius Secundus, filius, aetas 19
```

**PLINY:** This is flat text. But it is NOT flat data. Gaius has a family. His family has members. Each member has properties — name, role, age. This is a tree. Region contains families. Families contain members. Members have attributes. But the file does not know this. The file sees lines. I see a tree because I am a scholar. The machine sees characters because it is a machine.

**CARTER:** *(nodding — she's been waiting for this since the 1x08 closing)* The data describes itself. Or it should.

**PLINY:** Precisely. When I wrote the Naturalis Historia, volume one was the index. But the index was not part of the text — it was a separate document describing the structure of the other thirty-six. What if the structure were *inside* the text? What if every document carried its own tree?

**DANIEL:** *(the linguist, catching fire)* A language for describing structure. Not the content itself — the *shape* of the content.

### Beat 2: The First Markup

Daniel goes to the board. This is his moment — the linguist creating notation for structure.

**DANIEL:** We need markers. Tags. A way to say "this section begins here" and "this section ends here." Like quotation marks, but for structure.

He writes:

```
<regio nomen="VII">
  <familia>
    <nomen>Gaius Aurelius Ferrarius</nomen>
    <munus>faber</munus>
    <membrum>
      <nomen>Aurelia Tertia</nomen>
      <relatio>uxor</relatio>
    </membrum>
    <membrum>
      <nomen>Marcus Aurelius Minor</nomen>
      <relatio>filius</relatio>
      <aetas>12</aetas>
    </membrum>
  </familia>
</regio>
```

Silence. The team reads it. Then:

**MARCUS:** *(standing, studying the structure)* The indentation. Each level deeper is indented further. Like the steps of a building — you can see the structure from the outside.

**PLINY:** *(reading the markup, his face changing)* I can read this. Not because I understand the machine. Because the structure is the structure of the census. The machine is speaking my language, not the reverse.

**CARTER:** What Daniel just wrote is called markup. The angle brackets — `<regio>` and `</regio>` — mark the beginning and end of a region. Everything between them is *inside* that region. Nested markup creates a tree.

**DANIEL:** *(writing the term)* Structured Text Markup Language. STML.

**LUCIA:** *(studying the Latin tag names)* You named the tags in Latin. Not translated — *native*. `<regio>`, `<familia>`, `<membrum>`. These are our words describing our data.

**DANIEL:** *(a small, significant beat)* Yes. The language should belong to the people who use it.

*This is Daniel's turning point from translator to linguistic creator. He doesn't translate "region" to "regio" — he writes `<regio>` because that's what it IS. The tag names aren't translations. They're native Latin technical vocabulary that Daniel midwifes into existence.*

### Beat 3: The Parser — Reading the Tree

Carter takes the code to the terminal:

```c
/* Legere STML ex literis */
StmlResultus res = stml_legere_ex_literis(
    "<regio nomen=\"VII\">"
    "  <familia>"
    "    <nomen>Gaius Aurelius</nomen>"
    "  </familia>"
    "</regio>",
    piscina,
    intern);

si (res.successus)
{
    StmlNodus* radix = res.radix;
    imprimere("Genus: %d\n", radix->genus);
    /* STML_NODUS_DOCUMENTUM */
}
```

**CARTER:** `stml_legere_ex_literis` — read STML from text. It takes the markup, the arena for memory, and the interning table for string deduplication. It returns a result — success or failure, and if successful, the root of the tree.

**DATA:** *(precise)* The root node is of type `STML_NODUS_DOCUMENTUM` — document. It is the trunk of the tree. All content exists as branches beneath it.

**CARTER:** The parser reads the angle brackets. Every `<tag>` opens a node. Every `</tag>` closes it. Everything between them is content — either text or more nodes. The nesting creates the tree.

She shows the error handling:

```c
StmlResultus res = stml_legere_ex_literis(
    "<regio><familia></regio>",   /* missing </familia> */
    piscina, intern);

si (!res.successus)
{
    imprimere("Error: %.*s (linea %d)\n",
        (integer)res.error.longitudo,
        res.error.data,
        res.linea_erroris);
    /* "Tag non clausum: familia (linea 1)" */
}
```

**CARTER:** If you open a tag and don't close it, the parser tells you. Line number, column, what went wrong. The tree enforces its own rules.

**PLINY:** *(delighted)* The document corrects the author. Like a good editor.

**ACT BREAK — The tree is proposed, the first markup is written, the parser reads it.**

---

## ACT TWO: "Rami" (The Branches)

*Querying the tree. Finding things. The encyclopedia gets structured. Pliny's encyclopedia as the test case.*

### Beat 4: Node Types — The Vocabulary of Trees

Carter at the board:

```
Genera Nodorum — Node Types:

STML_NODUS_DOCUMENTUM  — The root. The trunk.
STML_NODUS_ELEMENTUM   — A branch. Has a name, attributes, children.
STML_NODUS_TEXTUS      — A leaf. Pure text content.
STML_NODUS_COMMENTUM   — A note to the reader. Not part of the data.
```

**CARTER:** Four types. The document is the trunk — one per tree. Elements are branches — they have names, they can have attributes, they can contain other elements or text. Text nodes are leaves — the actual content, the words on the page. Comments are notes — the parser reads them but they're not part of the data.

**MARCUS:** *(immediately)* An element is a room. It has a name — "kitchen," "atrium." It has properties — size, materials. It contains things — furniture, people, other rooms. Text is the thing you actually store in the room. Comments are the builder's marks on the stone — useful for the builder, invisible to the occupant.

**CARTER:** *(nodding)* That's exactly right.

On screen, the StmlNodus structure:

```c
/* StmlNodus — a node in the tree */
/* genus    — what type of node */
/* titulus  — tag name (for elements) */
/* valor    — text content */
/* attributa — Xar of StmlAttributum */
/* liberi   — Xar of StmlNodus* (children) */
/* parens   — pointer back to parent */
```

**DATA:** Each node knows its type, its name, its content, its attributes, its children, and its parent. The tree is fully navigable in both directions — from root to leaf and from leaf to root.

**CARTER:** And notice what this node contains: a Xar for children, a Xar for attributes, interned strings for names and values, all allocated from a Piscina. *(she writes the dependency chain on the board)*

```
STML depends on:
  Piscina (1x01)     — memory allocation
  Chorda (1x03)      — string representation
  ChordaAedificator  — building strings
  Xar (1x04)         — dynamic arrays (children, attributes)
  Internamentum (1x06) — string deduplication
```

**CARTER:** Five libraries. Five episodes. This is the first library that uses *everything* we've built. The tree grows from all the roots.

### Beat 5: Querying — Finding Things in the Tree

Casey takes the keyboard. His first code scene — he's been watching, learning the API.

```c
/* Pliny's encyclopedia, first entry */
StmlResultus res = stml_legere_ex_literis(
    "<encyclopedia>"
    "  <liber numerus=\"XII\" titulus=\"Arbores\">"
    "    <capitulum numerus=\"I\">"
    "      <observatio>"
    "        <nomen>Quercus Robur</nomen>"
    "        <locus>Gallia</locus>"
    "        <proprietates>Lignum durum, cortex crassus</proprietates>"
    "      </observatio>"
    "      <observatio>"
    "        <nomen>Olea Europaea</nomen>"
    "        <locus>Italia</locus>"
    "        <proprietates>Fructus oleosus, lignum leve</proprietates>"
    "      </observatio>"
    "    </capitulum>"
    "  </liber>"
    "</encyclopedia>",
    piscina, intern);

/* Invenire librum de arboribus */
StmlNodus* liber = stml_invenire_liberum(
    res.elementum_radix, "liber");

/* Capere titulum */
chorda* titulis = stml_attributum_capere(liber, "titulus");
imprimere("Liber: %.*s\n", (integer)titulis->longitudo, titulis->data);
/* "Liber: Arbores" */
```

**CASEY:** `stml_invenire_liberum` — find the first child with this tag name. Give me the first `<liber>` inside the encyclopedia. Then `stml_attributum_capere` — get the value of the "titulus" attribute. One call to find the branch. One call to read its label.

**PLINY:** *(leaning forward, eyes bright)* And if I want all the observations in the first chapter?

```c
/* Invenire capitulum */
StmlNodus* capitulum = stml_invenire_liberum(liber, "capitulum");

/* Invenire omnes observationes */
Xar* observationes = stml_invenire_omnes_liberos(
    capitulum, "observatio", piscina);

i32 n = xar_longitudo(observationes);
imprimere("Observationes: %d\n", n);
/* "Observationes: 2" */

/* Legere primam observationem */
StmlNodus* prima = *(StmlNodus**)xar_capere(observationes, 0);
StmlNodus* nomen_nodus = stml_invenire_liberum(prima, "nomen");
chorda textus = stml_textus_internus(nomen_nodus, piscina);
imprimere("Prima observatio: %.*s\n",
    (integer)textus.longitudo, textus.data);
/* "Prima observatio: Quercus Robur" */
```

**CASEY:** `stml_invenire_omnes_liberos` — find ALL children with this tag name. Every observation in the chapter, returned as a Xar. Then walk the array — `stml_textus_internus` gets the text content.

**PLINY:** *(standing, almost trembling)* That query would have taken me three hours with flat files. You found it in two lines.

**CASEY:** The tree already knows its own structure. We're not searching — we're navigating. Following branches.

**PLINY:** *(to Daniel, intense)* Do you understand what this means for the Naturalis Historia? Thirty-seven volumes. Twenty thousand observations. I could ask the machine: "Show me every observation about volcanic minerals in books thirty-three through thirty-seven" — and it would know. Not because someone indexed it. Because the structure IS the index.

### Beat 6: Attributes — The Labels on the Branches

Daniel demonstrates attributes:

```c
/* Attributa — metadata about an element */
StmlNodus* observatio = stml_elementum_creare(
    piscina, intern, "observatio");

stml_attributum_addere(observatio, piscina, intern,
    "classis", "minerale");
stml_attributum_addere(observatio, piscina, intern,
    "periodus", "ante Vesuvium");
stml_attributum_addere(observatio, piscina, intern,
    "fons", "Plinius Maior");

/* Boolean attributa — no value needed */
stml_attributum_boolean_addere(observatio, piscina, intern,
    "verificatum");
```

**DANIEL:** Attributes are metadata — data *about* the data. The observation is about a mineral. Its class is "minerale." Its period is "ante Vesuvium." Its source is Pliny the Elder. And the boolean attribute `verificatum` — verified — says this observation has been confirmed. No value needed. The tag's presence IS the value.

**PLINY:** *(studying the boolean attribute)* `verificatum`. Verified. A single word that certifies the observation. Like a seal on a document — the seal says nothing, but its presence says everything.

**DANIEL:** And STML supports this natively. In standard XML, you'd need `verificatum="true"`. In STML, just the word `verificatum` is enough. The language knows what a boolean attribute means.

```stml
<observatio classis="minerale" periodus="ante Vesuvium"
            fons="Plinius Maior" verificatum>
  <nomen>Obsidianus</nomen>
  <locus>Stabiae</locus>
</observatio>
```

**LUCIA:** *(reading the markup aloud, carefully)* I can read this. A Roman citizen who has never seen a machine could read this. The structure is Latin. The logic is Latin. The content is Latin. This is not a foreign language describing Roman data. This is a Roman document that happens to be readable by a machine.

**ACT BREAK.**

---

## ACT THREE: "Navigatio" (Navigation)

*Walking the tree. Parent and child. Siblings. The tree as a living structure you can move through.*

### Beat 7: Tree Navigation — Walking the Branches

Carter demonstrates navigation:

```c
/* Navigating the tree */
StmlNodus* radix = res.radix;

/* Primus liberum — first child */
StmlNodus* primus = stml_primus_liberum(radix);

/* Ultimus liberum — last child */
StmlNodus* ultimus = stml_ultimus_liberum(radix);

/* Frater proximus — next sibling */
StmlNodus* proximus = stml_frater_proximus(primus);

/* Frater prior — previous sibling */
StmlNodus* prior = stml_frater_prior(proximus);

/* Parens — back up to parent */
StmlNodus* parens_nodus = primus->parens;
```

**CARTER:** You can walk the tree in every direction. Down to the first child. Down to the last child. Sideways to the next sibling. Sideways to the previous sibling. Up to the parent. The tree is fully navigable.

**MARCUS:** *(the engineer, instantly)* A road network. Every intersection knows which roads lead forward, which lead back, which lead to the left and right, and which road brought you here. You can navigate without a map because every node IS a map.

**CARTER:** Let me show you something more powerful.

```c
/* Maiores — all ancestors from leaf to root */
Xar* maiores = stml_maiores(observatio_nodus, piscina);

/* Walking ancestors: observatio -> capitulum -> liber -> encyclopedia -> documentum */
per (i32 i = 0; i < xar_longitudo(maiores); i++)
{
    StmlNodus* maior = *(StmlNodus**)xar_capere(maiores, i);
    si (maior->titulus)
    {
        imprimere("  Maior %d: %.*s\n", i,
            (integer)maior->titulus->longitudo,
            maior->titulus->data);
    }
}
```

```
  Maior 0: capitulum
  Maior 1: liber
  Maior 2: encyclopedia
```

**CARTER:** `stml_maiores` — ancestors. From any node, you can ask: "Where am I?" The observation knows it's in a chapter, which is in a book, which is in the encyclopedia, which is in the document. The path from leaf to root tells you the *context* of any piece of data.

**PLINY:** *(stunned)* The observation knows where it lives. Not just what it says — where it belongs. The context is encoded in the structure itself.

**DATA:** *(completing the thought)* In a flat file, context is lost. An observation about obsidian is indistinguishable from an observation about olive trees unless you read the surrounding text. In a tree, the observation carries its ancestry. It knows it is in Book XXXVI, Chapter 67, under "Volcanic Minerals." The context is not interpretation. It is structure.

### Beat 8: Tree Manipulation — Building the Encyclopedia

Pliny and Carter build together:

```c
/* Creare the encyclopedia structure */
StmlNodus* encyclopedia = stml_elementum_creare(
    piscina, intern, "encyclopedia");

stml_attributum_addere(encyclopedia, piscina, intern,
    "auctor", "Gaius Plinius Secundus");
stml_attributum_addere(encyclopedia, piscina, intern,
    "titulus", "Naturalis Historia Nova");

/* Creare liber XII — De Arboribus */
StmlNodus* liber = stml_elementum_creare(
    piscina, intern, "liber");
stml_attributum_addere(liber, piscina, intern,
    "numerus", "XII");
stml_attributum_addere(liber, piscina, intern,
    "titulus", "De Arboribus");

/* Addere liberum — attach book to encyclopedia */
stml_liberum_addere(encyclopedia, liber);

/* Creare capitulum */
StmlNodus* cap = stml_elementum_creare(
    piscina, intern, "capitulum");
stml_attributum_addere(cap, piscina, intern,
    "numerus", "I");
stml_liberum_addere(liber, cap);

/* Creare observatio */
StmlNodus* obs = stml_elementum_creare(
    piscina, intern, "observatio");
stml_attributum_addere(obs, piscina, intern,
    "classis", "arbor");
stml_liberum_addere(cap, obs);

/* Addere text content */
stml_textum_addere(obs, piscina, intern,
    "Quercus robur — lignum durissimum, cortex crassus...");
```

**PLINY:** *(watching the tree build, node by node)* `stml_elementum_creare` — create a branch. `stml_liberum_addere` — attach it to its parent. `stml_textum_addere` — place the leaf. *(beat)* I am rebuilding the Naturalis Historia. But this time, the structure is inside the document, not inside my head.

**CARTER:** And because it's in the document, anyone can navigate it. Not just you.

**PLINY:** *(quietly, the weight of mortality in his voice)* Not just me. When I am gone.

A beat. Nobody comments. But the room feels it — the 80-year-old mortal, surrounded by immortals and an android, building something that will outlive him.

### Beat 9: Serialization — The Round Trip

Casey demonstrates the serializer:

```c
/* Scribere — serialize the tree back to text */
chorda output = stml_scribere(encyclopedia, piscina, VERUM);
/* VERUM = pretty-print with indentation */

imprimere("%.*s\n", (integer)output.longitudo, output.data);
```

Output:

```xml
<encyclopedia auctor="Gaius Plinius Secundus"
              titulus="Naturalis Historia Nova">
  <liber numerus="XII" titulus="De Arboribus">
    <capitulum numerus="I">
      <observatio classis="arbor">
        Quercus robur — lignum durissimum, cortex crassus...
      </observatio>
    </capitulum>
  </liber>
</encyclopedia>
```

**CASEY:** `stml_scribere` — write. The tree becomes text. The text can become a tree again. Parse, query, modify, serialize. The round trip is complete.

**CASEY:** And the `pulchrum` parameter — "beautiful" — controls pretty-printing. With it on, you get indentation, line breaks, human-readable structure. With it off, you get compact output for storage.

**PLINY:** *(reading the output)* This is beautiful. I can read it as a scholar. The machine can read it as a tree. The same document serves both purposes.

**MARCUS:** *(the engineer's observation)* And notice — the serialized output is valid input. You can save this to disk with Filum, reload it with stml_legere, and the tree is restored. *(beat)* This is how we should have been saving data since the beginning.

**ACT BREAK.**

---

## ACT FOUR: "Liberi et Parentes" (Children and Parents)

*Comments and construction. The dependency chain alive in one library. Daniel's term enters the language.*

### Beat 10: Comments — The Builder's Marks

Daniel introduces comments:

```c
/* Creare commentum */
StmlNodus* nota = stml_commentum_creare(
    piscina, intern,
    "Haec observatio verificanda est ab alio auctore");

stml_inserere_ante(obs, nota, piscina);
```

The markup now contains:

```xml
<!-- Haec observatio verificanda est ab alio auctore -->
<observatio classis="arbor">
  Quercus robur — lignum durissimum...
</observatio>
```

**DANIEL:** Comments. `<!-- text -->`. The parser reads them but they're not part of the data tree — they're notes for the human reader. The builder's marks that Marcus mentioned — visible in the source, invisible in the structure.

**PLINY:** *(immediately)* Scholia. Marginal notes. Every great manuscript has them — the copyist's commentary alongside the text. The content says "this is the observation." The scholion says "this observation needs verification." *(beat)* You have given my documents the capacity for self-annotation.

**DANIEL:** And unlike your wax tablet marginalia, these survive. They're inside the document. They persist with the text.

### Beat 11: The Full Construction API

Carter builds a complex document live, demonstrating the manipulation functions:

```c
/* Praeponere — prepend a child (add to beginning) */
StmlNodus* praefatio = stml_elementum_creare(
    piscina, intern, "praefatio");
stml_textum_addere(praefatio, piscina, intern,
    "Naturalis Historia Nova — Gaius Plinius Secundus");
stml_praeponere(encyclopedia, praefatio, piscina);

/* Inserere post — insert after a specific node */
StmlNodus* index = stml_elementum_creare(
    piscina, intern, "index");
stml_inserere_post(praefatio, index, piscina);

/* Numerus liberorum — count children */
i32 n = stml_numerus_liberorum(encyclopedia);
imprimere("Liberi encyclopediae: %d\n", n);
/* praefatio + index + liber XII = 3 */

/* Liberum ad indicem — get child by index */
StmlNodus* secundum = stml_liberum_ad_indicem(encyclopedia, 1);
/* secundum = index */
```

**CARTER:** The full construction API. Prepend, insert before, insert after, remove, replace. You can build the tree from scratch or restructure an existing one. Add a preface before book one. Insert an index between the preface and the first book. Count children. Get a child by position.

**MARCUS:** *(the builder)* This is not mere storage. This is architecture. You can remodel a document the way you remodel a building — add rooms, move walls, insert a corridor between the atrium and the triclinium. The structure is not fixed at creation. It can evolve.

### Beat 12: Daniel's Term — "Nodus"

A small scene that carries enormous weight.

Daniel is explaining the tree to a group of Roman engineers, Lucia translating alongside him.

**DANIEL:** Each point in the tree — each branch, each leaf, each junction — we call a "node." In Latin...

He pauses. Looks at Lucia.

**LUCIA:** *Nodus*. A knot. Where threads meet and bind.

**DANIEL:** *(to the Romans)* A nodus. Every element in the tree is a nodus. The document is a nodus. Each chapter is a nodus. Each observation is a nodus. They connect to each other the way knots connect threads in a net.

A Roman engineer, one of Marcus's apprentices, nods slowly. Then uses it:

**ROMAN ENGINEER:** So the census document — the radix nodus contains the regio nodi, and each regio nodus contains familia nodi?

Daniel blinks. The Roman just used his terminology naturally. Not as a translation — as vocabulary. The word belongs to them now.

*This is the turning point from the season outline: Daniel coins a term that the Romans adopt naturally. "Nodus" enters Nova Roma's daily vocabulary. He doesn't translate. He creates.*

**DANIEL:** *(to Lucia, quiet)* He used it.

**LUCIA:** *(smiling)* Of course he did. It's a good word. It's the right word.

### Beat 13: The Dependency Chain — Everything Comes Together

Carter writes on the board:

```
STML — Bibliotheca Arboris

Dependencies:
  Piscina (1x01)       — every node allocated from the arena
  Chorda (1x03)        — every string is a chorda
  ChordaAedificator    — serializer builds output string
  Xar (1x04)           — children and attributes are dynamic arrays
  Internamentum (1x06) — all tag names and attribute values interned

Not used:
  Filum (1x07)         — STML works in memory; saving to disk is separate
  Tempus (1x08)        — no time dependency
```

**CARTER:** This is the first library that uses everything except file I/O and time. Piscina allocates the memory. Chorda represents every string. ChordaAedificator builds the serialized output. Xar holds the children arrays and attribute lists. Internamentum ensures that every tag name — every `<observatio>`, every `<liber>`, every `<capitulum>` — exists once in memory and is compared by pointer.

```
Dependency tree (updated):

Piscina (1x01)
  +-- Credo (1x02)
  +-- Chorda (1x03)
  |     +-- ChordaAedificator (1x03)
  |     +-- Xar (1x04)
  |     +-- TabulaDispersa (1x05)
  |     |     +-- Internamentum (1x06)
  |     +-- Filum (1x07)
  +-- Tempus (1x08)
  +-- STML (1x09) ← uses Chorda, Xar, Internamentum  <--
```

**CARTER:** STML reconnects to the hash table chain — it needs Internamentum for interning, which needs Tabula Dispersa, which needs Chorda. It also needs Xar independently. The dependency tree is getting complex. Five libraries flowing into one.

**DATA:** *(precisely)* The dependency tree of our libraries IS a tree. STML is a tree that describes trees, built from libraries that form a tree. *(beat)* The recursion is not accidental.

**ACT BREAK.**

---

## ACT FIVE: "Arbor Vivens" (The Living Tree)

*Pliny's encyclopedia in STML. The full demo. The title moment.*

### Beat 14: Pliny's Encyclopedia — The Real Test

Pliny and Casey work together. The encyclopedia's first three entries, live, on screen:

```c
/* Naturalis Historia Nova — in arbore */
constans character* encyclopedia_stml =
    "<encyclopedia auctor=\"Gaius Plinius Secundus\""
    "             titulus=\"Naturalis Historia Nova\">"
    "  <liber numerus=\"XXXVI\" titulus=\"De Lapidibus\">"
    "    <capitulum numerus=\"LXVII\">"
    "      <observatio classis=\"minerale\" verificatum>"
    "        <nomen>Obsidianus</nomen>"
    "        <locus>Stabiae, Campania</locus>"
    "        <proprietates>Lapis niger, vitrus naturalis,"
    "          acies acutissima</proprietates>"
    "        <usus>Specula, cultri chirurgici</usus>"
    "        <nota>Primus inventus ab Obsio in Aethiopia</nota>"
    "      </observatio>"
    "    </capitulum>"
    "  </liber>"
    "</encyclopedia>";

StmlResultus res = stml_legere_ex_literis(
    encyclopedia_stml, piscina, intern);
```

**PLINY:** *(watching the parse succeed)* Read it. Show me the obsidian entry.

```c
StmlNodus* enc = res.elementum_radix;
StmlNodus* liber = stml_invenire_liberum(enc, "liber");
StmlNodus* cap = stml_invenire_liberum(liber, "capitulum");
StmlNodus* obs = stml_invenire_liberum(cap, "observatio");

/* Verificare — is it verified? */
b32 verificatum = stml_attributum_habet(obs, "verificatum");
imprimere("Verificatum: %s\n",
    verificatum ? "ITA" : "MINIME");

/* Capere nomen */
StmlNodus* nomen_nodus = stml_invenire_liberum(obs, "nomen");
chorda textus_nominis = stml_textus_internus(nomen_nodus, piscina);
imprimere("Minerale: %.*s\n",
    (integer)textus_nominis.longitudo,
    textus_nominis.data);
```

```
Verificatum: ITA
Minerale: Obsidianus
```

**PLINY:** *(very quietly)* It knows. The document knows that obsidian is verified. It knows the name. It knows the location. It knows the uses. And I did not build an index. The structure IS the index.

**CASEY:** *(nodding)* You asked for this in the cold open. The files should index themselves. They do now.

### Beat 15: Cloning — The Duplicate

A short but important scene:

```c
/* Duplicare — deep clone */
StmlNodus* copia = stml_duplicare(obs, piscina, intern);

/* The clone is independent — modifying it does not affect the original */
stml_attributum_addere(copia, piscina, intern,
    "copia", "verum");

/* Original unchanged */
b32 habet = stml_attributum_habet(obs, "copia");
/* habet = FALSUM — original does not have the attribute */
```

**CARTER:** `stml_duplicare` — deep clone. A complete copy of the node and all its children. The copy is independent — changes to the copy don't affect the original. Like making a wax impression of a signet ring — the impression is a perfect copy, but carving the impression doesn't damage the ring.

**PLINY:** *(the scholar's instinct)* For variant editions. When two manuscripts disagree, you clone the passage and annotate both versions. The original is preserved. The variant is recorded. Neither destroys the other.

### Beat 16: The Title Moment — "Arbor Documentorum"

**Location:** The Tabularium. Late afternoon. The team gathered. The full STML API on the board.

Carter writes the dependency tree — the updated one, with STML reconnecting to the hash table chain.

**CARTER:** Nine episodes. Nine libraries. STML is the first library that uses almost everything we've built — arena, strings, string builder, dynamic arrays, and string interning. Five libraries from five episodes flowing into one.

Pliny stands. He looks at the board. Then at the screen, where his encyclopedia entry sits in its tree structure — parseable, queryable, navigable.

**PLINY:** I have been a scholar for sixty years and a ghost for eighty more. I have organized knowledge in every way that ink and papyrus allow — indexes, cross-references, tables of contents, marginalia. Every method has the same limitation: the organization lives in my head or in a separate document. The knowledge and its structure are separate. *(beat)* This is the first time the structure lives inside the knowledge itself. The document is its own map.

He writes on the board:

**ARBOR DOCUMENTORUM**

**PLINY:** The tree of documents. Not a tree planted in the ground — a tree that grows inside the text. The branches are the structure. The leaves are the words. And the roots — *(he points to the dependency chain)* — the roots are everything you built before.

**DANIEL:** *(looking at the Latin words on the board, the words he helped create — nodus, radix, attributum, liberum, parens)* We built a language. Not a translation of English into Latin. A language that was born in Latin, for Latin speakers, describing the structure of Latin knowledge. *(beat)* And they're already using it.

He gestures toward the corridor, where Roman engineers are discussing census markup. The words nodus, radix, elementum float in from the hall, used casually, owned.

**Who:** Carter, Pliny, Daniel, full team.

### Beat 17: The Full Serialization

Casey runs the final demo — serialize the complete encyclopedia fragment:

```c
chorda output = stml_scribere(
    res.radix, piscina, VERUM);

/* Save to disk using Filum (1x07) */
filum_scribere_totum(
    "naturalis_historia_nova.stml",
    output,
    piscina);

/* Read back and verify */
chorda lecta = filum_legere_totum(
    "naturalis_historia_nova.stml",
    piscina);

StmlResultus res2 = stml_legere(
    lecta, piscina, intern);

/* Compare: original tree == reconstructed tree */
```

**CASEY:** The round trip. Parse to tree. Query the tree. Modify the tree. Serialize back to text. Save to disk with Filum. Read back from disk. Parse again. The tree is restored. *(beat)* Flat files are wax tablets — temporary, fragile, formless. Structured documents are stone inscriptions — permanent, organized, self-describing.

**PLINY:** *(completing the metaphor from 1x07)* Marcus said it in the last episode. We are becoming a city. *(beat)* This is the archive. The tabularium was wax. The arbor is stone.

---

## TAG

### Beat 18: Pliny at the Terminal — Alone

**Location:** Pliny's villa. Late evening. Pliny alone at the terminal. He's entering his encyclopedia — slowly, carefully, the way a scholar enters data. Tag by tag. Observation by observation.

On screen:

```xml
<encyclopedia auctor="Gaius Plinius Secundus"
              titulus="Naturalis Historia Nova">
  <!-- Liber I — Index Generalis -->
  <liber numerus="I" titulus="Index Generalis">
    <!-- To be structured from existing scrolls -->
  </liber>

  <!-- Liber II — De Mundo et Elementis -->
  <liber numerus="II" titulus="De Mundo et Elementis">
    <capitulum numerus="I" titulus="De Caelo">
      <observatio classis="astronomia" verificatum>
        <nomen>Stellae Fixae</nomen>
        <nota>Eaedem in utroque caelo — Terra et Nova Roma</nota>
      </observatio>
    </capitulum>
  </liber>
</encyclopedia>
```

He pauses at the second observation. Types:

```xml
      <observatio classis="astronomia" verificatum>
        <nomen>Caelum Novum</nomen>
        <nota>Constellationes ignotae. Caelum non est
          caelum Romae. Sed stellae fixae eaedem sunt.
          Quomodo?</nota>
      </observatio>
```

*The new sky. Unknown constellations. The sky is not Rome's sky. But the fixed stars are the same. How?*

Pliny the scholar, eighty years dead, asking questions he'll never answer, encoding them in a structure that will preserve the question for anyone who comes after.

He saves the file. The timestamp from Tempus (1x08) marks the moment.

**Who:** Pliny alone.
**Timing:** 60 seconds.

### Beat 19: Casey and Carter — Forward

**Location:** The workshop. Night. Casey and Carter, the closing pair.

**CARTER:** The encyclopedia is going to grow. The census is going to grow. Joe's Chronicles. Daniel's linguistic records. Every document needs structure. *(beat)* But right now, every document has to be structured by hand. Tag by tag.

**CASEY:** You want to extend the language.

**CARTER:** Not extend — augment. Fragments, so you can group things without adding semantic weight. Raw content tags, so you can embed code without the parser trying to read angle brackets. Capture operators, so you can restructure a document's tree without rewriting the source.

**CASEY:** *(cautious)* Those are powerful features. Powerful features are dangerous features.

**CARTER:** They're also necessary features. Pliny needs fragments for his variant editions. Daniel needs raw content for embedding Latin poetry with angle-bracket notation. The census engineers need a way to annotate flat data without restructuring the whole file.

**CASEY:** *(after a beat)* One step at a time. The tree works. The tree is solid. We can extend it when we need to. *(he looks at the screen)* Right now, for the first time, our data knows what it is.

**Who:** Casey, Carter.
**Timing:** 45 seconds.

*FORWARD NOTE: This seeds 1x10, where STML gets extended with capture operators, fragments, and other STML-specific features. 1x09 is the foundation — standard XML-like markup with DOM tree. 1x10 is the innovation.*

---

## STINGER

### Beat 20: Kirk's TAXONOMIA

**Location:** Earth. Night. Kirk's monitoring station. A new screen on the wall — not just text feeds and timestamps anymore. A tree structure.

Kirk has been watching Nova Roma's data transmissions. He's noticed the shift from flat text to structured markup. On screen: an intercepted STML document — the census fragment, captured through gate telemetry.

Kirk studies the tree structure. He doesn't read the content. He reads the *shape*. How many regions? How many families? How deep the hierarchy? The tree's structure tells him about Nova Roma's organization without needing to understand a word of Latin.

He opens a new file on his own system. Not markup — a classification tree. His organizational chart of Nova Roma's power structure:

```
NOVA ROMA — ORGANIZATIONAL TAXONOMY

Carter, S. — Technical Lead
  Dependency: everything
  Risk: highest

O'Neill, J. — Security / Strategy
  Dependency: Carter
  Risk: high (military response)

Data — Unknown Classification
  Dependency: unknown
  Risk: UNKNOWN (priority)

MacLeod, D. — Immortal / Defense
  Dependency: Casey, Joe
  Risk: medium (physical)

Midas — ?
  Classification: PENDING
  Temporal profile: ACTIVE
  Risk: ???
```

He's using their own innovation against them. They built a tree to organize knowledge. Kirk built a tree to organize targets.

He writes on the whiteboard: **9 / 31**

Below CHRONOGRAPHIA: **TAXONOMIA**

Kirk stinger progression: TABULARIUM (1x05) → LEXICON (1x06) → ARCHIVUM (1x07) → CHRONOGRAPHIA (1x08) → TAXONOMIA (1x09). The vocabulary mirrors the team's own tools: indexing → standardizing → persisting → timing → *classifying*. The team built a tree to organize their data. Kirk built a tree to organize them.

**KIRK:** *(studying the tree, very quiet)* Everything organizes itself eventually. You just have to watch long enough.

SMASH TO BLACK.

END OF EPISODE.

---

## ON-SCREEN CODE MOMENTS

### Moment 1: Pliny's Flat File Frustration (Cold Open)
The smeared wax tablet of cross-references. Line numbers. Three hours of manual indexing, erased. The problem before the solution.

### Moment 2: Daniel's First Markup (Act 1)
```xml
<regio nomen="VII">
  <familia>
    <nomen>Gaius Aurelius Ferrarius</nomen>
```
The moment markup is born. Latin tags. Native vocabulary. Not a translation.

### Moment 3: The Parser (Act 1)
```c
StmlResultus res = stml_legere_ex_literis(text, piscina, intern);
```
Text becomes tree. The parser reads angle brackets and builds structure.

### Moment 4: Querying — Find and Navigate (Act 2)
```c
StmlNodus* liber = stml_invenire_liberum(enc, "liber");
chorda* titulis = stml_attributum_capere(liber, "titulus");
```
Three hours of manual searching reduced to two function calls.

### Moment 5: Find All — The Power Query (Act 2)
```c
Xar* observationes = stml_invenire_omnes_liberos(
    capitulum, "observatio", piscina);
```
All observations in a chapter. One call. A Xar of results.

### Moment 6: Constructing the Tree (Act 3)
```c
StmlNodus* encyclopedia = stml_elementum_creare(piscina, intern, "encyclopedia");
stml_liberum_addere(encyclopedia, liber);
```
Building the tree node by node. Create, attach, grow.

### Moment 7: Navigation — Ancestors (Act 3)
```c
Xar* maiores = stml_maiores(observatio_nodus, piscina);
```
Any node knows its entire ancestry. Context is structure.

### Moment 8: The Round Trip (Act 5)
```c
chorda output = stml_scribere(res.radix, piscina, VERUM);
filum_scribere_totum("naturalis_historia_nova.stml", output, piscina);
```
Parse → query → modify → serialize → save → read → parse again. The tree survives the round trip.

### Moment 9: Boolean Attribute (Act 2)
```xml
<observatio classis="minerale" verificatum>
```
One word — `verificatum` — certifies the observation. The presence of the tag IS the value.

### Moment 10: Pliny's New Sky (Tag)
```xml
<nota>Constellationes ignotae. Caelum non est caelum Romae.
  Sed stellae fixae eaedem sunt. Quomodo?</nota>
```
A question encoded in structure, preserved for anyone who comes after. The scholar's eternal act.

---

## FEATURE-TO-DRAMATIC-MOMENT MAP

| STML Feature | API Function(s) | Dramatic Moment | Beat |
|---|---|---|---|
| **Parsing** | `stml_legere_ex_literis()` | Text becomes tree. The first parse. | 3 |
| **Error handling** | `StmlResultus.successus`, `.error`, `.linea_erroris` | "The document corrects the author" | 3 |
| **Node types** | `STML_NODUS_*` enum | Four types = the vocabulary of trees | 4 |
| **Node structure** | `StmlNodus` fields | Every node knows type, name, children, parent | 4 |
| **Find child** | `stml_invenire_liberum()` | Pliny's query in two calls vs. three hours | 5 |
| **Find all children** | `stml_invenire_omnes_liberos()` | "All observations in chapter one" | 5 |
| **Get attribute** | `stml_attributum_capere()` | Reading the label on the branch | 5 |
| **Get text** | `stml_textus_internus()` | "Prima observatio: Quercus Robur" | 5 |
| **Boolean attributes** | `stml_attributum_boolean_addere()`, `stml_attributum_habet()` | `verificatum` — the seal on the document | 6 |
| **Add attribute** | `stml_attributum_addere()` | Metadata about metadata | 6, 8 |
| **First/last child** | `stml_primus_liberum()`, `stml_ultimus_liberum()` | Tree navigation directions | 7 |
| **Siblings** | `stml_frater_proximus()`, `stml_frater_prior()` | Sideways navigation | 7 |
| **Parent pointer** | `nodus->parens` | "The road that brought you here" | 7 |
| **Ancestors** | `stml_maiores()` | The observation knows where it belongs — context as structure | 7 |
| **Create element** | `stml_elementum_creare()` | Building the encyclopedia node by node | 8 |
| **Add child** | `stml_liberum_addere()` | Attaching branches to the tree | 8 |
| **Add text** | `stml_textum_addere()` | Placing the leaf | 8 |
| **Comments** | `stml_commentum_creare()` | Scholia — the builder's marks | 10 |
| **Insert before/after** | `stml_inserere_ante()`, `stml_inserere_post()` | Document remodeling | 11 |
| **Prepend** | `stml_praeponere()` | Adding the preface | 11 |
| **Count children** | `stml_numerus_liberorum()` | Counting the branches | 11 |
| **Child by index** | `stml_liberum_ad_indicem()` | Positional access | 11 |
| **Deep clone** | `stml_duplicare()` | Variant editions — copy without destroying | 15 |
| **Serialize** | `stml_scribere()` | The round trip — tree back to text | 9, 17 |
| **Pretty-printing** | `stml_scribere(..., VERUM)` | "This is beautiful" | 9 |

**Features saved for 1x10:** Raw content tags (`!` suffix), capture operators (`(>`, `<)`, `<=...=>`), fragments (`<#>`), transclusion (`<<selector>>`), labels/namespace support, UUID/NanoID generation. These are STML-specific innovations beyond standard XML-like markup.

---

## DEPENDENCY CHAIN — THE TREE OF TREES

```
Episode 1x01: Piscina ............ memory allocation
    |
Episode 1x02: Credo .............. testing
    |
Episode 1x03: Chorda ............. strings
    |         ChordaAedificator .. string builder
    |
Episode 1x04: Xar ............... dynamic arrays
    |
Episode 1x05: TabulaDispersa .... hash table
    |         Friatio ............ hash functions
    |
Episode 1x06: Internamentum ..... string interning
    |
Episode 1x07: Filum ............. file I/O
    |
Episode 1x08: Tempus ............ time management
    |
Episode 1x09: STML .............. structured markup  ← HERE
    Uses: Piscina, Chorda, ChordaAedificator, Xar, Internamentum
    Does NOT use: Filum (I/O is separate), Tempus (no time dependency)

This is the CONVERGENCE episode — the first library that pulls from
five different libraries across five different episodes. The dependency
tree itself becomes a theme.
```

---

## KEY DIALOGUE SAMPLES

**PLINY (Cold Open):** "The text has structure. I can see it. A mineral entry contains: name, location, properties, uses, historical observations. These are not lists. They are categories within categories. Branches of a tree."

**DANIEL (Beat 2):** "We need markers. Tags. A way to say 'this section begins here' and 'this section ends here.' Like quotation marks, but for structure."

**LUCIA (Beat 6):** "I can read this. A Roman citizen who has never seen a machine could read this. The structure is Latin. The logic is Latin. This is not a foreign language describing Roman data. This is a Roman document that happens to be readable by a machine."

**PLINY (Beat 5):** "That query would have taken me three hours with flat files. You found it in two lines."

**DATA (Beat 7):** "In a flat file, context is lost. An observation about obsidian is indistinguishable from an observation about olive trees unless you read the surrounding text. In a tree, the observation carries its ancestry."

**PLINY (Beat 8):** "Not just me. When I am gone." *(the mortality beat)*

**ROMAN ENGINEER (Beat 12):** "So the census document — the radix nodus contains the regio nodi, and each regio nodus contains familia nodi?" *(Daniel's term, adopted)*

**DATA (Beat 13):** "The dependency tree of our libraries IS a tree. STML is a tree that describes trees, built from libraries that form a tree. The recursion is not accidental."

**PLINY (Title Moment):** "This is the first time the structure lives inside the knowledge itself. The document is its own map."

**KIRK (Stinger):** "Everything organizes itself eventually. You just have to watch long enough."
