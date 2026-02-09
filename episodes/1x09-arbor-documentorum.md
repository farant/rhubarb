# NOVUM FUNDAMENTUM
## Episode 1x09: "Arbor Documentorum" (Tree of Documents)
### Beat Sheet (Draft 1 — Compiled from Writers' Room)

---

**Library:** STML (Structured Text Markup Language — XML superset with DOM tree)
**Dependencies:** Piscina (1x01), Chorda (1x03), ChordaAedificator (1x03), Xar (1x04), Internamentum (1x06)
**Theme:** Flat is dead. The world has natural structure — families contain children, books contain chapters, knowledge contains categories. A document that knows its own shape can be understood by anyone, even centuries later.
**Dramatic Engine:** Pliny's encyclopedia exists as flat text — thirty-seven books of observations, cross-referenced only inside one man's head. The census is lines in a file. The Watcher Chronicles are unstructured pages. The data has outgrown its format. STML gives documents the ability to describe their own structure: markup, parsing, querying, navigation, serialization. The tree is born — and with it, a word: *stirps*.

---

**Spotlight Characters:** Daniel Jackson & Pliny the Elder (co-spotlight)
**Thematic Statement:** The structure was always in the data. The parser only reveals what was already there.

---

## Episode Info

| Section | Content |
|---------|---------|
| **Cold Open** | Pliny alone at his desk. Three flat files. Cross-referencing by hand on wax. Stylus slips. "Arbor documentorum." |
| **Act One: "Radix" (The Root)** | Beats 1-3. Pliny's presentation. Daniel's first markup. The parser. |
| **Act Two: "Rami" (The Branches)** | Beats 4-6. Node types. Querying the tree. Attributes and booleans. |
| **Act Three: "Navigatio" (Navigation)** | Beats 7-9. Tree navigation. Building the encyclopedia. Serialization — the round trip. |
| **Act Four: "Liberi et Parentes" (Children and Parents)** | Beats 10-13 + Three-Voice. Comments. Construction API. Daniel's "stirps." Dependency chain. |
| **Act Five: "Arbor Vivens" (The Living Tree)** | Beats 14-17. Pliny's encyclopedia — the real test. Pliny sees his mind externalized. Title moment. Full serialization. |
| **Tag** | Beats 18-19 + C3-C4. Pliny alone at terminal. Daniel/Lucia "colleague." Joe/Methos reunion. Casey/Carter forward. |
| **Stinger** | Beat 20. Kirk's TAXONOMIA. |

---

## COLD OPEN

**Location:** Pliny's villa. Morning. Pliny is at his desk with three documents open on screen — his encyclopedia entry on Vesuvian minerals, the census record for district seven, and Joe's Chronicle entry on Duncan MacLeod. He is trying to cross-reference them.

The problem is visible before anyone speaks. Each document is a flat text file — lines of characters. No headers, no sections, no categories. Finding where "Vesuvian obsidian" is mentioned in the census requires reading the entire census file. Finding whether Joe's Chronicle mentions the same geological period requires reading the entire Chronicle.

Pliny has developed his own system — handwritten marginalia on a separate wax tablet, his *commentarii*, the intermediate working notes he has used since Rome. The tablet is covered in his meticulous hand: "Obsidianus — census lin. 2847, encyclopedia lin. 491, Chronica lin. 318."

He adds another entry. His stylus slips. The wax smears. Three hours of cross-referencing, gone.

**PLINY:** *(staring at the smeared wax)* The text has structure. I can see it. A mineral entry contains: name, location, properties, uses, historical observations. The census entry contains: name, district, family, occupation. These are not lists. They are... categories within categories. Branches of a tree.

He picks up a scroll — volume twelve of the Naturalis Historia, the one about trees. Opens it to a diagram: a tree, labeled with genus and species. Roots, trunk, branches, leaves.

**PLINY:** A tree. Each branch contains its children. Each child knows its parent. The root contains everything.

He sets the scroll next to the screen showing the flat text file. The juxtaposition is devastating — the Roman polymath drew the data structure two thousand years ago but the computer doesn't know it.

**PLINY:** *(to himself, writing on a fresh tablet)* Arbor documentorum. The tree of documents.

SMASH TO MAIN TITLES.

**Who:** Pliny alone.
**Timing:** 90 seconds.

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

**PLINY:** Precisely. When I wrote the Naturalis Historia, volume one was the index — a separate document describing the structure of the other thirty-six. What if the structure were *inside* the text? What if every document carried its own tree?

**DANIEL:** *(the linguist, catching fire)* A language for describing structure. Not the content itself — the *shape* of the content.

**Who:** Pliny, Carter, Daniel, full team.

### Beat 2: The First Markup — Daniel's Creation

**Location:** The Tabularium. Daniel goes to the board. This is his moment — the linguist creating notation for structure.

**DANIEL:** We need markers. Tags. A way to say "this section begins here" and "this section ends here." Like quotation marks, but for structure.

He writes:

```xml
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

**MARCUS:** *(standing, studying the structure)* The indentation. Each level deeper is indented further. Like the steps of a building — you can see the structure from the outside. *(his face changes — recognition)* This is a stemma. Every noble house in Rome has one — painted lines connecting ancestral portraits on the atrium wall. Parent above, children below. You have built a stemma for documents.

**PLINY:** *(reading the markup, his face changing)* I can read this. Not because I understand the machine. Because the structure is the structure of the census. The machine is speaking my language, not the reverse.

**O'NEILL:** *(from the back of the room, arms crossed)* Who decides what nests inside what?

**MARCUS:** *(immediately)* The data decides. An arch has load-bearing constraints. You cannot put the keystone where the foundation goes. The structure has a nature.

**DANIEL:** *(nodding)* The data determines its own hierarchy. Region CONTAINS families. Families CONTAIN members. The nesting follows the relationships that already exist.

**O'NEILL:** *(satisfied — the same concern he raised in 1x05 with "Who chose the math?", the same answer)* Fair enough.

**CARTER:** What Daniel just wrote is called markup. The angle brackets — `<regio>` and `</regio>` — mark the beginning and end of a region. Everything between them is *inside* that region. Nested markup creates a tree.

**DANIEL:** *(writing the term)* Structured Text Markup Language. STML.

**LUCIA:** *(studying the Latin tag names)* You named the tags in Latin. Not translated — *native*. `<regio>`, `<familia>`, `<membrum>`. These are our words describing our data.

**DANIEL:** *(a small, significant beat)* Yes. The language should belong to the people who use it.

**LUCIA:** *(reading the complete markup aloud, carefully)* This is not a foreign language describing Roman data. This is a Roman document.

*This is Daniel's first step from translator to linguistic creator. He doesn't translate "region" to "regio" — he writes `<regio>` because that's what it IS. The tag names aren't translations. They're native Latin technical vocabulary.*

Daniel introduces the node terminology:

**DANIEL:** Each point in the tree — each junction where branches meet — we call a *nodus*. A knot.

**LUCIA:** Where threads meet and bind.

The term is established. *Nodus* is the technical word for individual nodes in the tree. It enters the vocabulary naturally — a Roman engineer will adopt it before the act is out.

**Who:** Daniel, Marcus, Pliny, Lucia, O'Neill, Carter, full team.

### Beat 3: The Parser — Reading the Tree

**Location:** The Tabularium. Carter takes the code to the terminal.

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

She shows the error handling. Teal'c leans forward — the quality engineer takes the keyboard.

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

**TEAL'C:** *(having deliberately constructed the malformed input)* The document was improperly formed. The parser identified the precise error and the location of the error.

**CARTER:** If you open a tag and don't close it, the parser tells you. Line number, column, what went wrong. The tree enforces its own rules.

**TEAL'C:** *(a single observation, delivered with weight)* Structure requires discipline. A structure that enforces its own discipline is trustworthy.

**PLINY:** *(delighted)* The document corrects the author. Like a good editor.

**ACT BREAK — The tree is proposed, the first markup is written, the parser reads it.**

**Who:** Carter, Data, Teal'c, Pliny.

---

## ACT TWO: "Rami" (The Branches)

*Node types. Querying the tree. Attributes. The tree's vocabulary crystallizes.*

### Beat 4: Node Types — The Vocabulary of Trees

**Location:** The Tabularium. Carter at the board.

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
/* StmlNodus — nodus in arbore STML */
/* genus    — what type of node */
/* titulus  — tag name (for elements) */
/* valor   — text content */
/* attributa — Xar de StmlAttributum */
/* liberi   — Xar de StmlNodus* (children) */
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

**Who:** Carter, Marcus, Data.

### Beat 5: Querying — Casey's First Code Scene

**Location:** The Tabularium. Casey takes the keyboard. His first code scene — he's been watching, learning the API.

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
imprimere("Liber: %.*s\n",
    (integer)titulis->longitudo, titulis->data);
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

**Who:** Casey, Pliny, Daniel.

### Beat 6: Attributes — The Labels on the Branches

**Location:** The Tabularium. Daniel demonstrates attributes.

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

The markup on screen:

```xml
<observatio classis="minerale" periodus="ante Vesuvium"
            fons="Plinius Maior" verificatum>
  <nomen>Obsidianus</nomen>
  <locus>Stabiae</locus>
</observatio>
```

**LUCIA:** *(reading the markup aloud, carefully)* I can read this. A Roman citizen who has never seen a machine could read this. The structure is Latin. The logic is Latin. The content is Latin. This is not a foreign language describing Roman data. This is a Roman document that happens to be readable by a machine.

A brief moment — Daniel and Lucia both reach for the same stylus on the work table to annotate a tag name. Their hands touch. Lucia doesn't pull away immediately. Daniel notices. They both go back to work. Three seconds. Nobody comments.

**ACT BREAK.**

**Who:** Daniel, Pliny, Lucia.

---

### Beat C1 (Act 2-3): Carter's Emotional-State Experiment

**Location:** Carter's workstation. Alone. She opens her temporal data — the Midas frequency from 1x08 (0.0037 Hz, confirmed sinusoidal). Now she has STML. She structures her observations:

```xml
<mensura tempus="47832.441892">
  <frequentia valor="0.00371" amplitudo="0.0041"/>
  <observatio>Midas in horto. Tranquillus. Cum herbis.</observatio>
</mensura>
<mensura tempus="48102.556723">
  <frequentia valor="0.00369" amplitudo="0.0043"/>
  <observatio>Midas in foro. Ridens. Cum Marco loquens.</observatio>
</mensura>
<mensura tempus="48814.339201">
  <frequentia valor="0.00382" amplitudo="0.0058"/>
  <observatio>Midas post disputationem de aquaeductu. Agitatus.</observatio>
</mensura>
```

The frequency shifts slightly when Midas is agitated. The amplitude increases. When he's calm, it's stable. When he's animated, it rises. Not dramatically — the difference is in the fourth decimal place. But it's there. The "heartbeat" accelerates under stress.

Carter saves the file. Structured now — not the flat text she started with in 1x05, but STML. Each measurement timestamped (Tempus), each observation tagged (STML structure), each correlation preserved.

**CARTER:** *(to herself, very quiet)* It responds to his emotional state. It's not just alive. It's connected to him.

She closes the file. The burden deepens.

**Who:** Carter alone.
**Timing:** 45 seconds.

---

## ACT THREE: "Navigatio" (Navigation)

*Walking the tree. Parent and child. Siblings. Building the encyclopedia. The round trip.*

### Beat 7: Tree Navigation — Walking the Branches

**Location:** The Tabularium. Carter demonstrates navigation.

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

**MARCUS:** *(the engineer, instantly)* A road network. Every intersection knows which roads lead forward, which lead back, which lead to the left and right, and which road brought you here. You can navigate without a map because every nodus IS a map.

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

**Who:** Carter, Marcus, Pliny, Data.

### Beat 8: Building the Encyclopedia — Pliny and Casey Together

**Location:** The Tabularium. Pliny and Casey build together.

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

A beat. Nobody comments. But the room feels it — the eighty-year-old mortal, surrounded by immortals and an android, building something that will outlive him.

**Who:** Pliny, Casey, Carter.

---

### Beat C2 (Act 3): Joe/Pliny Structured Chronicles

**Location:** Pliny's study. Joe and Pliny are converting Watcher records into structured format. Joe is dictating. Pliny is organizing.

**JOE:** Subject: Duncan MacLeod. First observation: 1625. Location: Scottish Highlands. Observer designation: W-4419.

**PLINY:** *(writing in the STML structure, which he's taken to with the passion of a man who has been organizing information for eighty years)* Wait. Your observer. W-4419. This observer also documented three other subjects in the same period?

**JOE:** How did you know that?

**PLINY:** *(pointing to the tree on screen)* I cross-referenced. Your flat records listed the observer number in each entry but never connected them. In the tree, I grouped observations by observer. W-4419 was prolific — four subjects across Scotland and Northern England between 1620 and 1640. *(beat)* He was a good Watcher.

**JOE:** *(staring at the structured data)* I've been reading these records for thirty years. I never saw that connection.

**PLINY:** You didn't have a tree.

**Who:** Joe, Pliny.
**Timing:** 45 seconds.

---

### Beat 9: Serialization — The Round Trip

**Location:** The Tabularium. Casey demonstrates the serializer.

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

**PLINY:** *(reading the output)* This is beautiful. I can read it as a scholar. The machine can read it as a tree. The same document serves both purposes.

**MARCUS:** *(the engineer's observation)* And notice — the serialized output is valid input. You can save this to disk with Filum, reload it with `stml_legere`, and the tree is restored. *(beat)* This is how we should have been saving data since the beginning.

**ACT BREAK.**

**Who:** Casey, Pliny, Marcus.

---

## ACT FOUR: "Liberi et Parentes" (Children and Parents)

*Comments, construction, "stirps," the dependency chain, three-voice.*

### Beat 10: Comments — The Builder's Marks

**Location:** The Tabularium. Daniel introduces comments.

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

**Who:** Daniel, Pliny.

### Beat 11: The Full Construction API

**Location:** The Tabularium. Carter builds a complex document live.

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

**Who:** Carter, Marcus.

### Midas: The Persian Archives (30 seconds)

**Location:** The Tabularium. Midas wanders through — cheerful, carrying scrolls from the storeroom. He pauses near the tree diagram on the board.

**MIDAS:** *(looking at the tree structure, smiling)* Trees. When Cyrus took Babylon, his scribes found the filing system of the Babylonian priests. Clay tablets organized into categories — categories within categories. Cylinder seals marking each branch. *(beat)* When the empire fell, the filing system survived. The Persians adopted it. The scribes barely noticed the change of empire.

He wanders out. Carter, from across the room, catches a detail — her expression shifts briefly. The clay tablets he mentions. A collection that disappeared.

Thirty seconds. The comedy of a man casually dropping two-thousand-five-hundred-year-old archival history because he was THERE. The temporal plant: the tablets are in a museum Carter knows. Or they were.

**Who:** Midas (brief, 30 seconds). Carter notices.

### Beat 12: Daniel's Term — "Stirps"

**Location:** The Tabularium. A scene that carries enormous weight.

Daniel is explaining the tree structure to a group of Roman engineers, Lucia alongside him. He has established "nodus" as the technical term (Beat 2). But Lucia pushes further.

**LUCIA:** *(reading Daniel's diagram)* Nodus. Radix. Liberi. Parens. These are Latin words. They are not Latin *thinking*. You have mapped your language onto ours like a mosaic on foreign stone. A Roman reads "nodus" and thinks of a knot — a complication, something tangled. You want them to think of a branch point. These are not the same idea.

**DANIEL:** *(after Lucia's critique, pacing)* The problem isn't finding Latin words for tree concepts. The problem is that I'm starting from the tree and looking for Latin. I need to start from Latin and find the tree.

**PLINY:** *(who has been listening)* A tree? My encyclopedia is not a tree. It is organized by subject.

**DANIEL:** But within each subject — your entry on olive trees. It has a heading. Under the heading, sections. Under sections, observations. Under observations, specific details. That's not flat. That's nested. Parents contain children.

**PLINY:** *(the scholar's mind catching fire)* Stirps! The genealogy. The *gens* contains *familiae*. The *familia* contains *domus*. The *domus* contains persons. *(standing)* My encyclopedia is a *stirps*. Volume is the *gens*. Chapter is the *familia*. Entry is the person.

**DANIEL:** *(stunned — Pliny just did his work for him)* Stirps. The rootstock. The family tree.

**LUCIA:** *(quietly, to Daniel)* You did not translate. You reminded him of something he already knew.

A Roman engineer, one of Marcus's apprentices, uses it immediately:

**ROMAN ENGINEER:** So the census document — the radix nodus contains the regio nodi, and each regio nodus contains familia nodi. *(beat)* Show me the stirps of the census document.

Daniel blinks. The Roman just used BOTH terms naturally — "nodus" for the technical part, "stirps" for the whole tree. Not as translations. As vocabulary. The words belong to them now.

**DANIEL:** *(to Lucia, quiet)* He used it.

**LUCIA:** *(smiling)* Of course he did. They're good words. They're the right words.

*This is Daniel's Turning Point 2 from the season outline: Daniel coins terms that the Romans adopt naturally. "Nodus" is the technical word (the part). "Stirps" is the conceptual word (the whole). The episode title is "Arbor Documentorum" — where they started. "Stirps" is where they end.*

**Who:** Daniel, Lucia, Pliny, Roman Engineer.

### Beat 13: The Dependency Chain — Everything Comes Together

**Location:** The Tabularium. Carter writes on the board.

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
  +-- STML (1x09) ← uses Chorda, Xar, Internamentum
```

**CARTER:** STML reconnects to the hash table chain — it needs Internamentum for interning, which needs Tabula Dispersa, which needs Chorda. It also needs Xar independently. Five libraries from five episodes flowing into one.

**DATA:** *(precisely)* The dependency tree of our libraries IS a tree. STML is a tree that describes trees, built from libraries that form a tree. *(beat)* The recursion is not accidental.

**ACT BREAK.**

**Who:** Carter, Data.

### Three-Voice Algorithm Moment (Late Act 4)

Grows organically from the dependency-chain discussion. The realization that trees are everywhere — in knowledge, in memory, in language.

**DANIEL:** *(who has been standing at the board where he wrote "stirps," the linguist hearing something deeper)* Every language is a tree. Proto-Indo-European is the root. Latin, Greek, Sanskrit — branches. French, Spanish, Italian — children of Latin. Children inherit from parents but they are not their parents. French is not Latin. But French *remembers* Latin. *(beat)* When I coin a word here — when "stirps" enters Roman vocabulary for documents — I'm grafting a new branch onto an ancient tree. The tree doesn't reject it. It grows to accommodate it. That's what living languages do. That's what living trees do.

**PLINY:** *(looking at his encyclopedia tree on screen)* Every stirps tells you where something came from. Volume seven contains the section on gold. The section on gold contains the observation from Hispania. If you want the observation, you must first know the volume, then the section, then the entry. *(beat)* Knowledge is not flat. It lives in families. It has parents.

**DATA:** *(precise)* A tree has a property that neither of you has stated but both of you rely on. It is this: every node has exactly one parent. This constraint — single parentage — means the tree can be traversed unambiguously. Any node can reach the root by following parent links. There is one path from any leaf to the root. Never two. Never zero. *(beat)* I find this constraint significant. When Commander Maddox questioned whether I was a person, he was questioning whether my tree had a root — whether there was a genuine origin, or merely a first instruction. *(a pause that is longer than Data's usual pauses)* The tree answers that question. Every node has exactly one parent. Authority is always resolvable. Origin is always traceable.

Three translations:
- **Daniel:** The tree as linguistic inheritance — languages are trees that grow, translation is mapping between trees, coining a word is grafting a branch.
- **Pliny:** The tree as genealogy of knowledge — everything has lineage, knowledge lives in families, the tree is the scholar's native structure.
- **Data:** The tree as formal guarantee — single parentage means unambiguous authority, every query has an answer, the structure itself is a proof. Plus the "Measure of a Man" resonance: the tree provides what Maddox demanded — a traceable origin.

*The architecture: Daniel speaks from LANGUAGE (meaning has structure). Pliny speaks from NATURE (the world has hierarchy). Data speaks from LOGIC (structure has provable properties). All three agree: the tree isn't invented. It's discovered.*

**Who:** Daniel, Pliny, Data.

---

## ACT FIVE: "Arbor Vivens" (The Living Tree)

*Pliny's encyclopedia in STML. The emotional core. The title moment. Full serialization.*

### Beat 14: Pliny's Encyclopedia — The Real Test

**Location:** The Tabularium. Late afternoon. Pliny and Casey work together. The encyclopedia's first entries, live, on screen:

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

**Who:** Pliny, Casey.

### Beat 15: Pliny Sees His Mind Externalized

**Location:** The Tabularium. Carter and Daniel have encoded Volume 7 of Pliny's encyclopedia — metals and mining — as a demonstration. They display the tree on screen. Pliny's entries. Pliny's chapter titles. Pliny's observations. The parent-child relationships rendered graphically, the nesting explicit, the structure that was always in his mind now externalized.

**PLINY:** *(staring at the screen for a long beat)* That is... my book.

**CARTER:** Volume seven. Metals and mining.

**PLINY:** No. You don't understand. That is my book THE WAY I SEE IT. In my mind. *(he touches the screen the way he touched scrolls in 1x05)* When I think about my encyclopedia, this is what I see. Volumes containing sections. Sections containing observations. Observations containing details. A stirps. A living tree. I have carried this structure in my memory for sixty years. *(his voice breaks very slightly)* No one has ever seen it. Until now.

**CASEY:** *(quietly, from behind)* Now it doesn't have to be in your head.

**PLINY:** *(turning to Casey)* For sixty years, if I had died, the organization would have died with me. The words would survive — I made sure of that. But the *shape* of the knowledge... *(he looks back at the tree on screen)* ...the shape would have been lost. The scrolls would remain. The index in my mind would not.

**CARTER:** Now the shape is on disk. Preserved. Searchable. Navigable by anyone.

**PLINY:** *(sitting down, suddenly exhausted by relief)* The stirps survives the gardener.

**Who:** Pliny, Carter, Casey, Daniel.

### Beat 16: The Title Moment

**Location:** The Tabularium. Late afternoon. The team gathered.

Pliny stands. He looks at the board — the dependency tree, the STML API, the markup examples. Then at the screen, where his encyclopedia entry sits in its tree structure — parseable, queryable, navigable.

**PLINY:** I have been a scholar for sixty years and a ghost for eighty more. I have organized knowledge in every way that ink and papyrus allow — indexes, cross-references, tables of contents, marginalia. Every method has the same limitation: the organization lives in my head or in a separate document. The knowledge and its structure are separate. *(beat)* This is the first time the structure lives inside the knowledge itself. The document is its own map.

He writes on the board:

**ARBOR DOCUMENTORUM**

**PLINY:** The tree of documents. Not a tree planted in the ground — a tree that grows inside the text. The branches are the structure. The leaves are the words. And the roots — *(he points to the dependency chain)* — the roots are everything you built before.

**DANIEL:** *(looking at the Latin words on the board, the words he helped create — nodus, radix, stirps, attributum, liberum, parens)* We built a language. Not a translation of English into Latin. A language that was born in Latin, for Latin speakers, describing the structure of Latin knowledge. *(beat)* And they're already using it.

He gestures toward the corridor, where Roman engineers are discussing census markup. The words nodus, radix, stirps float in from the hall, used casually, owned.

**Who:** Pliny, Daniel, Carter, full team.

### Beat 17: Full Serialization — The Round Trip

**Location:** The Tabularium. Casey runs the final demo.

```c
/* Serialize the complete encyclopedia */
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

/* Round trip complete — original tree == reconstructed tree */
```

**CASEY:** The round trip. Parse to tree. Query the tree. Modify the tree. Serialize back to text. Save to disk with Filum. Read back from disk. Parse again. The tree is restored. *(beat)* Flat files are wax tablets — temporary, fragile, formless. Structured documents are stone inscriptions — permanent, organized, self-describing.

**PLINY:** *(completing the metaphor from 1x07)* Marcus said it in the last episode. We are becoming a city. *(beat)* This is the archive. The tabularium was wax. The arbor is stone.

**Who:** Casey, Pliny.

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

### Beat 18b: Daniel/Lucia — The "Colleague" Scene

**Location:** The Tabularium. Late. The team has dispersed. Daniel and Lucia are still at the work table, the remains of a long day's markup spread between them.

**LUCIA:** *(end of a long work session)* You have improved.

**DANIEL:** *(surprised)* At what?

**LUCIA:** At listening. When you arrived, you heard Latin. Now you hear the thought inside the Latin. *(beat)* That is the difference between a translator and a... *(she searches for the word)*

**DANIEL:** A what?

**LUCIA:** *(very quiet)* A colleague.

She gathers her tablets. Leaves. Daniel watches her go. The word "colleague" is loaded — it's the most Lucia has ever acknowledged their intellectual partnership. It's also the least romantic word she could have chosen, which makes it MORE charged. "Colleague" from Lucia Serena is a declaration of equality. From a woman who doesn't consider many people her equal.

**Who:** Daniel, Lucia.
**Timing:** 30 seconds.

---

### Beat C3 (Tag area): Daniel's STML Investigation Dossier

**Location:** Daniel's workspace. He's cross-referencing Methos fragments, Watcher records, and archaeological evidence — his investigation from 1x06-07, now in structured format.

On screen — visible for five seconds, not read in detail:

```xml
<investigatio titulus="Alexandria-Serapeum">
  <fontes>
    <fons genus="testis" nomen="Methos" tempus="-280">
      <fragmentum>Sub-cella. Sacerdos. Lapis niger.</fragmentum>
    </fons>
    <fons genus="vigil" nomen="Custos-280BC" tempus="-280">
      <observatio>Homo descriptioni congruens fugit ex Serapaeo.</observatio>
    </fons>
    <fons genus="archaeologicum" tempus="-250/-200">
      <observatio>Serapaeum inferiores cellae documentatae.</observatio>
    </fons>
  </fontes>
</investigatio>
```

**PLINY:** *(passing by Daniel's workspace, the scholar in him immediately engaged)* Three convergent sources for a single event. That is strong evidence. *(beat, looking at the STML structure)* Your tree. The root is the investigation. The branches are the sources. The connections between branches — those are your cross-references.

**DANIEL:** *(nodding)* The structure grows without losing coherence. When I find more sources, I add more branches.

**Who:** Daniel, Pliny.
**Timing:** 60 seconds.

---

### Beat C4 (Tag area): Joe/Methos Reunion

**Location:** Late night. Joe's temporary quarters — not the bar yet (that's 1x13), just a room near the Forum. Wine on the table. Methos appears without announcement. He's good at appearing without announcement.

**METHOS:** *(standing in the doorway, wine in hand — he brought his own)* Joe.

**JOE:** *(not surprised — he's been expecting this)* Methos.

A beat. Methos sits. They drink.

**JOE:** *(after a long drink)* The Chronicles have you as Adam Pierson. Watcher researcher. Born 1963. Died... well, you didn't die, you just stopped showing up. Your file is still open.

**METHOS:** You could close it.

**JOE:** I could. *(beat)* I haven't.

**METHOS:** Why?

**JOE:** Because closing a file means the story is over. *(beat)* And your story isn't over. It's the longest one we have.

Silence. Methos drinks. When he speaks, the sardonic edge is thinner.

**METHOS:** When I was in Alexandria — the real one, not the nostalgia version — there was a cataloguer. He spent forty years indexing scrolls. By the time he finished, the first scrolls he'd indexed had been recopied and the originals destroyed. He started over. He spent another thirty years. Then the Romans came and he started over again.

**JOE:** What happened to him?

**METHOS:** He died. Old. Still cataloguing. *(beat)* He was the happiest man in Alexandria. He'd found the thing that didn't end.

**JOE:** *(understanding)* The work.

**METHOS:** The work. *(looking at Joe with something approaching directness)* Daniel asked me about Alexandria. About the lower levels. The things I saw there.

**JOE:** I know. I have the Watcher record. 280 BC. An observer noted a man matching your description fleeing the Serapeum. Running. A man who doesn't run from anything was running.

**METHOS:** *(very quiet)* What else does your record say?

**JOE:** That you didn't return to Alexandria for four hundred years. *(beat)* What did you see, Methos?

**METHOS:** *(the five-thousand-year-old man choosing his words like someone disarming a bomb)* Something old. Older than the Library. There was... an impulse. The drive to structure knowledge, to build trees of information, to make the world navigable. *(he looks at the Tabularium across the forum through the window)* Sound familiar?

Joe follows his gaze. The Tabularium. The trees of Pliny's encyclopedia glowing through the window.

**METHOS:** Sometimes it's human genius. Sometimes it isn't. *(beat)* Daniel is asking the right questions. And he has better tools now than I had then. *(very careful)* He'll find what I found. But he'll understand it better than I did. I was a survivor. He's a scholar.

The scene ends. Two men who've been watching from the sidelines, now on the field. The Watcher's file on the world's oldest man is still open. The conversation isn't over. But it's started.

**JOE:** *(alone after Methos leaves, writing in his journal)* "Subject appears concerned that the Nova Roma project echoes a pattern he has observed before. Further investigation required."

He pauses. Crosses out "Subject." Writes: "Methos."

The Watcher who promised not to watch. Already watching.

**Who:** Joe, Methos.
**Timing:** 120 seconds.

---

### Beat 19: Casey/Carter — Forward

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

### Casey/Pliny Faith Scene (Tag area): The Patronage Conversation

**Location:** Pliny's villa. Evening. The work is done for the day. Wine. Pliny has his notebook — the same *adversaria* where he files all his observations. He has a page with numbered questions. Casey sees it.

**CASEY:** *(looking at the list)* You wrote questions.

**PLINY:** *(the naturalist — of course he wrote questions)* Seven so far. I have been composing them since our conversation about memory. *(1x05 callback)* The first six require more time. But the seventh is brief.

**CASEY:** Let's hear it.

**PLINY:** You say there is one God.

**CASEY:** Yes.

**PLINY:** But you pray to a woman. This Mary. *(he gestures at the rosary on Casey's wrist)* The beads — the counting — that prayer is to her.

**CASEY:** I ask her to pray for me.

**PLINY:** *(processing — this is unfamiliar)* You ask... her to pray? To the God? On your behalf?

**CASEY:** Yes.

**PLINY:** So she is an intermediary. Between you and your God. *(his scholar's mind reaching for the parallel)* Like a patron.

Casey stops. He looks at Pliny. The word "patron" has just illuminated something Casey has tried to explain to modern people a hundred times and failed. The Roman *patronus* system — a powerful person who intercedes on behalf of a client — is structurally identical to intercessory prayer. A modern person hears "Why do you pray to Mary?" and thinks it's worship. A Roman hears it and thinks: *of course* you approach the powerful through an intermediary. That's how civilization works.

**CASEY:** *(slowly, genuinely surprised)* That's... actually not a bad way to think about it.

**PLINY:** *(nodding, satisfied)* I understand patronage. A client does not approach the emperor directly. He approaches a patron who has the emperor's ear. The patron intercedes. The emperor may or may not grant the request, but the patron has standing to ask.

**CASEY:** Mary has standing.

**PLINY:** Because of her relationship to your God.

**CASEY:** Because she's his mother.

**PLINY:** *(sitting back)* The patron is the God's MOTHER. *(he writes in his notebook)* That is... a considerably powerful patron.

**CASEY:** *(small smile)* Yeah.

**PLINY:** *(writing, almost to himself)* "Patrona Dei — mater ipsius. Intercessio non est adoratio sed petitio per auctoritatem." *(The patron of God — his own mother. Intercession is not worship but petition through authority.)*

A beat. Pliny looks up from his notebook.

**PLINY:** And the beads — the counting practice. You said "enough" — that was Marcus's question in the last episode. But I want to understand the structure.

**CASEY:** Repetitive prayers. Meditations on the life of her son.

**PLINY:** *(beat — the scholar processing)* A repetitive devotional practice with structured meditation. We have something similar. The Salii — the priests of Mars — perform a ritual dance, stepping and counting, while reciting the Carmen Saliare. The steps are the counting. The words are the meditation. *(beat)* Yours is quieter.

**CASEY:** Mine doesn't involve dancing.

**PLINY:** *(a small smile — the first time Pliny has made anything resembling a joke about religion)* Perhaps it should.

**CASEY:** *(genuine laugh)*

**PLINY:** *(closing the notebook, looking at Casey)* Your faith has a stirps.

He just used the episode's new word to describe Casey's religion. The technical and spiritual threads converge.

**PLINY:** I don't have to believe in it to study it. But it has structure. It has a genealogy of ideas. *(beat)* I have six more questions. They are more difficult.

**CASEY:** This is going to take more than one evening.

**PLINY:** *(the man who waited eighty years in Nova Roma)* I have been waiting eighty years for interesting answers. I can wait a few more evenings.

**Who:** Casey, Pliny.
**Timing:** 90-120 seconds.

---

## STINGER

### Beat 20: Kirk's TAXONOMIA

**Location:** Earth. Night. Kirk's monitoring station. Something has changed. The flat lists on the wall are gone. In their place: a TREE. Floor-to-ceiling. Hand-drawn. A wall-tree.

Kirk has reorganized his intelligence operation. The flat list of names, dates, assets has been restructured into hierarchy:

```
NOVA ROMA
├── PERSONNEL
│   ├── MILITARY (O'Neill, Teal'c, Carter)
│   ├── CIVILIAN (Daniel, Lucia, Marcus, Pliny)
│   └── ANOMALOUS
│       ├── MACLEOD, D.
│       ├── CASEY
│       └── METHOS (ref: ??)
├── INFRASTRUCTURE
│   ├── GATE TELEMETRY
│   ├── TEMPORAL PROFILE (see CHRONOGRAPHIA)
│   └── COMMUNICATIONS INTERCEPTS
└── OPERATIONS
    ├── TABULARIUM — data catalog
    ├── LEXICON — standardized terms
    ├── ARCHIVUM — persistent storage
    ├── CHRONOGRAPHIA — temporal analysis
    └── TAXONOMIA — ???
```

An aide enters with a folder.

**AIDE:** The Seattle cell is restructured. Three layers now, like you asked. Nobody below the second level knows anyone above it.

**KIRK:** *(not looking up from the wall-tree)* Show me.

The aide produces a printed org chart. Kirk holds it next to his wall-tree. They match.

**KIRK:** *(pointing to a node)* If they capture any branch, the root survives. If they capture the root — *(beat)* — there is no root. The root is an abstraction. The tree doesn't have a center. It has a structure.

He circles the ANOMALOUS branch on the wall-tree. Three names. MacLeod, Casey, Methos. Three unknowns.

He writes on the whiteboard: **9 / 31**

Below CHRONOGRAPHIA: **TAXONOMIA**

Kirk stinger progression: TABULARIUM (1x05) → LEXICON (1x06) → ARCHIVUM (1x07) → CHRONOGRAPHIA (1x08) → TAXONOMIA (1x09). The vocabulary mirrors the team's own tools: indexing → standardizing → persisting → timing → *classifying*. The team built a tree to organize knowledge. Kirk built a tree to organize them.

**KIRK:** *(studying the tree, very quiet)* Everything organizes itself eventually. You just have to watch long enough.

SMASH TO BLACK.

END OF EPISODE.

**Who:** Kirk, Aide.

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
Text becomes tree. The first parse. Error handling: `STML_ERROR_TAG_NON_CLAUSUM`.

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

### Moment 6: Boolean Attribute (Act 2)
```xml
<observatio classis="minerale" verificatum>
```
One word — `verificatum` — certifies the observation. The presence of the tag IS the value.

### Moment 7: Navigation — Ancestors (Act 3)
```c
Xar* maiores = stml_maiores(observatio_nodus, piscina);
```
Any node knows its entire ancestry. Context is structure.

### Moment 8: Constructing the Tree (Act 3)
```c
StmlNodus* encyclopedia = stml_elementum_creare(piscina, intern, "encyclopedia");
stml_liberum_addere(encyclopedia, liber);
stml_textum_addere(obs, piscina, intern, "Quercus robur...");
```
Building the tree node by node. Create, attach, grow.

### Moment 9: The Round Trip (Act 3/5)
```c
chorda output = stml_scribere(res.radix, piscina, VERUM);
filum_scribere_totum("naturalis_historia_nova.stml", output, piscina);
```
Parse → query → modify → serialize → save → read → parse again. The tree survives the round trip.

### Moment 10: Construction API (Act 4)
```c
stml_praeponere(encyclopedia, praefatio, piscina);
stml_inserere_post(praefatio, index, piscina);
i32 n = stml_numerus_liberorum(encyclopedia);
StmlNodus* secundum = stml_liberum_ad_indicem(encyclopedia, 1);
```
Prepend, insert, count, access by index. Architecture, not storage.

### Moment 11: Comments as Scholia (Act 4)
```c
StmlNodus* nota = stml_commentum_creare(piscina, intern,
    "Haec observatio verificanda est ab alio auctore");
stml_inserere_ante(obs, nota, piscina);
```
Builder's marks that survive inside the document.

### Moment 12: Pliny's Encyclopedia — The Real Test (Act 5)
```c
b32 verificatum = stml_attributum_habet(obs, "verificatum");
chorda textus_nominis = stml_textus_internus(nomen_nodus, piscina);
```
The document knows its own content. "Verificatum: ITA."

### Moment 13: Pliny's New Sky (Tag)
```xml
<nota>Constellationes ignotae. Caelum non est caelum Romae.
  Sed stellae fixae eaedem sunt. Quomodo?</nota>
```
A question encoded in structure, preserved for anyone who comes after.

---

## ENSEMBLE BEATS

### Daniel — Co-Spotlight
Daniel drives the linguistic creation. His arc: Translator → Language Architect (Turning Point 2). He creates the markup notation (Beat 2), establishes "nodus" as the technical term (Beat 2), arrives at "stirps" through Pliny and Lucia's collaboration (Beat 12), and enters the three-voice for the first time with the language-as-tree speech. The "colleague" scene with Lucia (Beat 18b) is the personal undertone — noticeable but deniable.

### Pliny — Co-Spotlight, Emotional Core
Pliny's arc: Isolated Scholar → Connected Mentor. His encyclopedia IS the test data. Cold open frustration → utility discovery (Beat 5: "three hours in two lines") → emotional revelation (Beat 15: "No one has ever seen it. Until now.") → title moment (Beat 16: "The document is its own map.") → solitary encoding (Beat 18: the new sky). His mortality line — "Not just me. When I am gone" (Beat 8) — and the patronage conversation with Casey make this his richest episode since 1x05.

### Carter — Builder Under Strain
Builds the parser, demonstrates the API, draws the dependency chain. Her C-plot beat (C1 — emotional-state experiment) deepens the Midas burden: structured XML logs show the frequency shifts with emotional state. "It's not just alive. It's connected to him." She carries the Midas data from approximate to precise to *correlated with psychology*.

### Marcus — Structural Intuition
"An element is a room" (Beat 4). "A road network" (Beat 7). "This is not mere storage — this is architecture" (Beat 11). "This is a stemma" (Beat 2). Marcus consistently provides the Roman engineering lens for every concept. His recognition of the stemma — the family tree painted on noble house walls — is the visual bridge between Roman culture and DOM trees.

### Casey — Background Craft + Faith Thread
Casey's first code scene (Beat 5 — querying). The faith conversation with Pliny — the patronage parallel, the Salii comparison, "six more questions," "Your faith has a stirps." Casey is background-active this episode — not spotlighted (that was 1x08), but present and significant.

### Lucia — Co-Architect
"These are Latin words. They are not Latin *thinking*" (Beat 12). The "colleague" scene (Beat 18b). The stylus-touch moment (Beat 6). Lucia pushes Daniel from translation to creation. Her intellectual authority is growing — she's becoming the curriculum architect of Nova Roma.

### Teal'c — Quality Engineer
Folded into the parser scene (Beat 3). Deliberate malformed input. "Structure requires discipline." Brief but precise.

### Data — The Constant
Data's three-voice moment — single parentage as formal guarantee, the "Measure of a Man" resonance ("Was my tree rooted in a genuine origin?"). Present in the teaching scenes, anchoring the formal perspective. The constant in the three-voice rotation.

### O'Neill — The Clarity Question
"Who decides what nests inside what?" (Beat 2) — paralleling 1x05's "Who chose the math?" One beat. Done in 30 seconds. The guardian of common sense.

### Methos — Perfectly Placed
One scene, in the Tag. The Alexandria cataloguer parable ("the happiest man in Alexandria — he'd found the thing that didn't end"). The black stone impulse — brief, vague, unsettling. "He'll find what I found." 120 seconds of the season's most important C-plot scene.

### Joe — Participant
Three appearances: working session with Pliny (C2 — "You didn't have a tree"), the Methos reunion (C4), and through Daniel's dossier (C3, indirectly). Joe is no longer settling in — he's producing structured intelligence.

### Midas — The Unknowing Anomaly
Thirty seconds. The Persian archives story. Clay tablet trees. "When the empire fell, the filing system survived." A plant hiding in plain sight.

### Duncan — Background
Not spotlighted. Present at demonstrations. His relationship with the team is implied through Joe's scenes.

---

## THREE-VOICE ALGORITHM MOMENT

| Episode | Three Voices | Algorithm | Lens |
|---------|-------------|-----------|------|
| 1x05 | Pliny / Data / Casey | Hashing | Fingerprint of knowledge |
| 1x06 | Methos / Data / Casey | Interning | Canonical copy, one for all |
| 1x07 | Joe / Pliny / Data | Persistence | Writing to survive |
| 1x08 | Casey / Methos / Data | Time management | Time as biography / density / gap |
| **1x09** | **Daniel / Pliny / Data** | **Structured markup** | **Tree as language / genealogy / formal guarantee** |

**Placement:** Late Act 4, after Daniel's stirps coinage (Beat 12) and the dependency-chain discussion (Beat 13). The three-voice grows from the realization that trees are everywhere — in knowledge, in memory, in language.

**Structure:**
- **Daniel:** The tree as linguistic inheritance — languages are trees that grow, every word coined is a branch grafted onto an ancient tree.
- **Pliny:** The tree as genealogy of knowledge — knowledge lives in families, has parents, has lineage.
- **Data:** The tree as formal guarantee — single parentage means unambiguous authority, the "Measure of a Man" resonance, origin is always traceable.

---

## THEMATIC NOTES

### Nominalism / Realism — Trees as Realist Structures

The tree is the realist position made into code. When you nest `<fauna><aves><aquila>`, you're asserting that eagles ARE birds that ARE animals. The nesting encodes an ontological truth about the NATURE of the relationship. A flat file is nominalist — all data equivalent, position accidental. A tree is realist — position is meaningful, hierarchy reflects nature.

Lucia states the thesis through Daniel: "A tree that reflects the nature of what it contains is not a classification. It is a recognition." This is folded into the Daniel/Lucia work scenes — the philosophical through-line expressed through the characters who understand it best.

Kirk's rootless tree is the nominalist counter — structure without essence. His compartmentalized network has hierarchy but no natural root. "If they capture the root — there is no root." The nominalist version of organization: structure without nature, designed to resist reality rather than reflect it.

### Catholic Thread — The Patronage Scene

The faith thread escalates to systematic inquiry. Previous beats were *reactive* — someone notices Casey's practice. Now Pliny *initiates*. He comes to Casey with prepared questions. The polymath who has spent eighty years collecting information has added Casey's religion to his research program.

The patronage parallel works because the Roman *patronus* system is structurally identical to intercessory prayer. Modern people hear "praying to Mary" and think it's polytheism. Romans hear it and immediately understand: you approach power through intermediaries. Pliny gets it in thirty seconds because his civilization was built on the same architecture.

The Salii comparison is Pliny being Pliny — he compares, he catalogs, he finds parallels. The humor beat ("Perhaps it should [involve dancing]") is the first time Pliny has joked about religion. An escalation.

"Your faith has a stirps" — the technical and spiritual threads converge without either dominating.

### The Convergence Episode

This is the first library that pulls from five different libraries across five different episodes. The dependency tree itself becomes a theme. Data's line: "STML is a tree that describes trees, built from libraries that form a tree. The recursion is not accidental."

### Franchise Resonances

- **TNG "The Chase":** The progenitor's DNA revealed hidden structure — common descent encoded in biology. STML reveals hidden structure — hierarchy encoded in markup. Same operation, different substrate.
- **SG-1 "The Fifth Race":** O'Neill had all the knowledge and no structure. STML is the structure. The parser IS the index O'Neill lacked.
- **TNG "Darmok":** Daniel as linguist building a language whose structure IS meaning. STML is "Darmok made formal."
- **TNG "The Measure of a Man":** Data's three-voice moment. Single parentage as the answer to Maddox's question: "Does the tree have a genuine root, or merely a first instruction?"
- **Highlander — Watcher Chronicles as DOM tree:** Joe recognizes the tree instantly — it IS his filing system. Subject → eras → entries → observations.

### Code-to-Drama Map

| STML Feature | API Function(s) | Dramatic Moment | Beat |
|---|---|---|---|
| **Parsing** | `stml_legere_ex_literis()` | Text becomes tree. The first parse. | 3 |
| **Error handling** | `StmlResultus.successus`, `.error`, `.linea_erroris` | Teal'c's malformed input. "Structure requires discipline." | 3 |
| **Node types** | `STML_NODUS_*` enum | Four types = the vocabulary of trees | 4 |
| **Find child** | `stml_invenire_liberum()` | Pliny's query in two calls vs. three hours | 5 |
| **Find all children** | `stml_invenire_omnes_liberos()` | "All observations in chapter one" | 5 |
| **Get attribute** | `stml_attributum_capere()` | Reading the label on the branch | 5 |
| **Get text** | `stml_textus_internus()` | "Prima observatio: Quercus Robur" | 5 |
| **Boolean attributes** | `stml_attributum_boolean_addere()`, `stml_attributum_habet()` | `verificatum` — the seal on the document | 6, 14 |
| **First/last child** | `stml_primus_liberum()`, `stml_ultimus_liberum()` | Tree navigation directions | 7 |
| **Siblings** | `stml_frater_proximus()`, `stml_frater_prior()` | Sideways navigation | 7 |
| **Ancestors** | `stml_maiores()` | The observation knows where it belongs | 7 |
| **Create element** | `stml_elementum_creare()` | Building the encyclopedia node by node | 8 |
| **Add child** | `stml_liberum_addere()` | Attaching branches to the tree | 8 |
| **Add text** | `stml_textum_addere()` | Placing the leaf | 8 |
| **Comments** | `stml_commentum_creare()` | Scholia — the builder's marks | 10 |
| **Insert before/after** | `stml_inserere_ante()`, `stml_inserere_post()` | Document remodeling | 11 |
| **Prepend** | `stml_praeponere()` | Adding the preface | 11 |
| **Count children** | `stml_numerus_liberorum()` | Counting the branches | 11 |
| **Child by index** | `stml_liberum_ad_indicem()` | Positional access | 11 |
| **Serialize** | `stml_scribere()` | The round trip — tree back to text | 9, 17 |
| **Pretty-printing** | `stml_scribere(..., VERUM)` | "This is beautiful" | 9 |

**Features saved for 1x10:** Raw content tags (`!` suffix), capture operators (`(>`, `<)`, `<=...=>`), fragments (`<#>`), transclusion (`<<selector>>`), labels/namespace support. These are STML-specific innovations beyond standard XML-like markup.

---

## GENRE ARCHITECTURE

### The Convergence Diagram

```
Piscina (1x01)
  +-- Credo (1x02) — testing
  +-- Chorda (1x03) — strings
  |     +-- ChordaAedificator (1x03)
  |     +-- Xar (1x04) — dynamic arrays
  |     +-- TabulaDispersa (1x05) — hash table
  |     |     +-- Internamentum (1x06) — interning
  |     +-- Filum (1x07) — file I/O
  +-- Tempus (1x08) — time
  +-- STML (1x09) ← uses Piscina, Chorda, ChordaAedificator, Xar, Internamentum
```

This is the first library that uses almost everything built across five episodes. The dependency tree grows wider AND reconnects to the hash table chain. Five roots flowing into one tree.

### Daniel Enters the Three-Voice Rotation

The three-voice pattern expands. Previous rotation: {Pliny/Data/Casey, Methos/Data/Casey, Joe/Pliny/Data, Casey/Methos/Data}. Daniel entering is a feature — the rotation is growing with the ensemble. Casey rests here; he had the last two.

### Kirk's Stinger Progression

seed → germination → root → branches → organization → standardization → persistence → timing → **taxonomy**

TABULARIUM → LEXICON → ARCHIVUM → CHRONOGRAPHIA → **TAXONOMIA**

The dark mirror sharpens: the team builds a tree to give knowledge its natural structure (realist hierarchy). Kirk builds a tree to give his operatives a *designed* structure — compartmentalized, resilient to capture, no natural center. Realist tree vs. nominalist tree. One reflects the structure of reality. The other imposes a structure to resist reality.

---

## CONTINUITY NOTES

### Backward Connections

**From 1x01 (Piscina):** Every STML node allocated from the arena. The foundation literal: `stml_elementum_creare(piscina, intern, ...)`.

**From 1x02 (Credo):** Teal'c's testing methodology — malformed input in Beat 3. Structure requires discipline.

**From 1x03 (Chorda):** Every string in the tree is a chorda. The Chorda/ChordaAedificator dependency is visible in serialization.

**From 1x04 (Xar):** Children and attributes stored as dynamic arrays. The census from 1x04 — flat in Xar — now needs hierarchy.

**From 1x05 (Tabula Dispersa/Internamentum):** All tag names interned via Internamentum. The hash table chain reconnects.

**From 1x07 (Filum):** The round trip: serialize → save with Filum → read back → parse again. Carter saved temporal data to disk in 1x07; now she structures it in STML.

**From 1x08 (Tempus):** Carter's frequency confirmed (0.0037 Hz). Now: emotional-state correlation detected. Casey and Carter's closing line in 1x08 — "Arbor documentorum" — this is that episode.

### Forward Connections

**To 1x10 (STML Extensions):** Casey/Carter's closing line seeds the extension episode. Raw content tags, capture operators, fragments. O'Neill's XML dossier deferred here.

**To 1x11-12 (Mid-Season Crisis):** Daniel/Lucia beyond "colleague." Casey's faith under crisis pressure.

**To 1x13 (Aurum/Midas):** Daniel's STML dossier grows. Methos's implicit authorization accelerates the investigation. The dossier connects to Midas's Phrygian origin.

**To 1x14-16 (Pliny's Questions):** "Six more questions." "This is going to take more than one evening." The patronage scene seeds the longer theological interview.

---

## THREAD STATUS APPENDIX: After 1x09

| Thread | Status After 1x09 | Next Beat |
|--------|-------------------|-----------|
| **Casey's Faith** | **SYSTEMATIC INQUIRY BEGINS.** Pliny approaches Casey with prepared questions. Patronage parallel for Marian devotion. Salii comparison — first religious humor. "Six more questions." "This is going to take more than one evening." "Your faith has a stirps." | 1x10: Rest. 1x11-12: Under crisis pressure, Casey crosses himself before deployment. |
| **Kirk Stinger** | **9/31. TAXONOMIA.** Kirk builds a wall-tree — floor-to-ceiling visual. Compartmentalized network, "tree with no root." Circles ANOMALOUS branch (MacLeod, Casey, Methos). "Everything organizes itself eventually. You just have to watch long enough." | 1x10: Kirk mirrors STML extensions. |
| **Joe Dawson** | **PARTICIPANT.** Joe/Methos private conversation happened. Joe/Pliny working sessions now use STML. Crosses out "Subject," writes "Methos." Producing structured intelligence. | 1x10-12: More sessions. Bar scouting begins. |
| **Joe/Methos** | **REUNION HAPPENED.** Private. Wine. Late night. Honest. The cataloguer parable. Methos gives implicit permission for Daniel's investigation. "He'll find what I found." The professional distance is gone. What remains is personal. | 1x10-12: Comfortable. Collaboration builds. |
| **Joe/Pliny Partnership** | **PRODUCTIVE.** STML transforms their collaboration. Pliny cross-references Watcher records in ways Joe never could. "You didn't have a tree." | 1x10-13: Deepening. By 1x13: the bar opens as second workspace. |
| **Black Stone / Cybele** | **INVESTIGATION ACCELERATED.** Daniel builds structured STML dossier. Three convergent sources cross-referenced. Methos implicitly blesses the investigation. Daniel knows: black stone, Serapeum, Cybele cult, 280 BC. He doesn't know it's a Borg artifact. | 1x10-13: More sources. By 1x13: connects to Midas's Phrygian origin. |
| **Midas Temporal** | **EMOTIONAL CORRELATION DETECTED.** Carter finds frequency shifts with Midas's emotional state (amplitude increases when agitated). Now stored in structured STML. The "heartbeat" is connected to his psychology. | 1x10-12: More data. Does Midas know what he is? |
| **Daniel as Investigator** | **EMPOWERED.** STML tools for structured analysis. Dossier organized, cross-referenced, growing. Methos's implicit authorization. Pliny sees the methodology and admires it. | 1x10-13: Investigation accelerates toward Midas reveal. |
| **Methos's Secret** | **DOOR OPENED.** Didn't reveal what he saw. But acknowledged Daniel is on the right track. Trusting someone else to handle what he couldn't. The secret is shared in the sense that others pursue it with his knowledge. | 1x10-17: Less alone with it. |
| **Carter's Secret Burden** | **STRUCTURED + DEEPER.** Data now in STML — organized, timestamped, with emotional-state correlation. Evidence harder to dismiss. She needs to tell someone. Casey knows frequency but not emotional correlation. | 1x10-12: Silence becomes untenable. |
| **Nominalism / Realism** | **TREES AS REALIST STRUCTURES.** Flat files = nominalist (all data equivalent). Trees = realist (hierarchy reflects nature). Kirk's rootless tree = nominalist counter. Lucia: "A tree that reflects the nature of what it contains is not a classification. It is a recognition." | Ongoing. Each library deepens. |
| **Pliny's Mortality** | **ENGAGED.** "Not just me. When I am gone." (Beat 8). "The stirps survives the gardener" (Beat 15). Pliny is energized — the tree gives him a tool for immortality that doesn't require being immortal. | Ongoing. The tree preserves what Pliny cannot. |
| **Data/Midas Friendship** | **BACKGROUND.** Data present in teaching scenes. Not spotlighted on Midas thread this episode. | 1x10: Data may notice patterns through structured-data lens. |
| **Daniel/Lucia** | **"COLLEAGUE."** The word. The stylus touch. The intellectual partnership deepens. Noticeable but deniable. | 1x11-12: Simmers. Save escalation for later. |
| **Time Discrepancy** | **UNCHANGED from 1x08.** O'Neill tracking. No new beat — the intelligence thread simmers. | 1x10-12: More data points accumulate. |
| **Earth-Side Seed** | **UNCHANGED from 1x08.** "Smart Town" chyron was placed. | 1x10: Gilfoyle/Dinesh introduction per season outline. |

---

## ADDITIONAL NOTES

### Historical Grounding

- **Roman Stemmata:** Painted family trees on atrium walls, connecting wax portrait masks (*imagines*). Pliny NH 35.6: "Stemmata vero lineis discurrebant ad imagines pictas." Marcus recognizes the DOM tree as stemma (Beat 2).
- **Pliny's Compilation Method:** Nephew describes uncle's working method (Ep. 3.5): a reader (*lector*), working notebooks (*commentarii*), categorized excerpts. The parse → tokenize → build pipeline. Pliny was the first compiler.
- **The Codex from Caudex:** The word "codex" (bound book) comes from "caudex" (tree trunk). A bound book is, etymologically, a tree trunk. The physical medium of structured documents carries the tree in its name. Daniel or Lucia could note this.
- **The Tree of Porphyry:** Aristotle's Categories → Porphyry's Isagoge (c. 268 AD) → the first explicit tree diagram for logical classification. Substance → bodily/incorporeal → living/nonliving → sensing/insensing → rational/irrational → HOMO. The realist artifact. Daniel would know this tradition through medieval manuscripts.
- **The Roman Census Hierarchy:** Regio → tribus → familia → civis. Four levels of nesting. A tree. The flat Xar census from 1x04 needs the hierarchy STML provides.

### Production Design

- **Stemmata on Roman house walls** — visual rhyme with DOM tree on screen. Same branching structure. Same parent-child connections. Same visual grammar.
- **Pliny's workspace:** Scrolls in *capsa* (cylindrical scroll-cases). Wax tablet notebooks (*tabulae ceratae*). The contrast: linear scrolls vs. the tree on screen.
- **A physical tree** in the Forum or villa — the visual motif. The team working under it, building tree structure in code.

### The Joe/Methos Scene as Series Architecture

The Joe/Methos private conversation is arguably the most important C-plot scene in Season 1. It releases three episodes of deferred tension. It reframes Methos from sardonic commentator to strategically honest collaborator. It authorizes Daniel's investigation ("He'll find what I found"). The "happiest man in Alexandria" parable is Methos at his storytelling best — it's about Joe, it's about Pliny, it's about the entire STML project. Organizing knowledge is the work that doesn't end.

### Performance Notes

**Pliny (Beat 15 — "That is my book THE WAY I SEE IT"):** The emotional payload of the episode. Voice breaks slightly. He touches the screen the way he touched scrolls in 1x05. Sixty years of structure locked in one man's head, finally externalized.

**Daniel (Beat 12 — "He used it."):** Small. Private. The recognition that a word he midwifed is alive. He doesn't celebrate. He nods.

**Lucia (Beat 18b — "A colleague."):** Very quiet. The word is loaded. She leaves. Daniel watches. The least romantic word she could have chosen, which makes it the most charged.

**Kirk (Beat 20 — "Everything organizes itself eventually."):** Studied. Calm. The man who watches the watchers, learning from what they build.

**Joe (C4 — crosses out "Subject," writes "Methos"):** The Watcher who promised not to watch. Already watching. The pen moving on the journal. The gesture says more than the words.

---

*Compiled from A-Plot Technical Pitch, Character & Flashback Pitch, Serialized Threads Pitch, Genre Fan Pitch, and Historian Pitch per the Compilation Plan decisions document. All open questions resolved per showrunner approval (all recommendations approved).*
