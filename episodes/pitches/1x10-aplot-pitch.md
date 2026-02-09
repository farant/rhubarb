# 1x10 A-PLOT PITCH: "Lingua Nova Structurae" (New Language of Structure)
## A-Plot / Technical Writer

---

## PREMISE

1x09 gave Nova Roma the tree. Documents know their own shape. But they don't yet know how to handle the REAL world — messy content, provisional structure, references to themselves. 1x10 is where STML stops being XML-that-speaks-Latin and becomes something new. The episode takes six features that individually seem like convenience tools and reveals them as a philosophy: a markup language should be able to describe ANY document, including documents that are still being written, documents that reference themselves, and documents that contain other languages.

The seed was planted in 1x09 Beat 19. Carter listed the features: "Fragments, so you can group things without adding semantic weight. Raw content tags, so you can embed code without the parser trying to read angle brackets. Capture operators, so you can restructure a document's tree without rewriting the source." Casey's response: "Those are powerful features. Powerful features are dangerous features." This episode proves both of them right.

---

## THE PROBLEM (Cold Open)

**Location:** The Tabularium. Early morning. Daniel is at a terminal. He is trying to document the STML language USING STML. Specifically, he's trying to write a manual that includes examples of STML markup.

The problem is immediate and devastating:

```xml
<exemplum>
  <code>
    To create an element, write <elementum> and close it with </elementum>.
  </code>
</exemplum>
```

The parser chokes. Of course it does. The `<elementum>` inside the `<code>` tag is interpreted as a REAL tag, not as example text. Daniel is trying to write ABOUT a language IN that language, and the language can't contain itself.

**DANIEL:** *(staring at the error)* The language cannot describe itself. The moment I write an angle bracket as content, the parser reads it as structure. I cannot write a manual for this language in this language.

**PLINY:** *(arriving, reading the error)* Your document about documents is defeated by its own notation. *(beat)* That is... actually a significant problem. An encyclopedia must be able to contain any subject — including itself.

Daniel tries workarounds. Spelling out "angle bracket." Writing descriptions instead of showing code. Every approach is worse.

**DANIEL:** *(to Carter, who's just arrived)* We built a language that can describe anything — except itself.

**CARTER:** *(sitting down, already thinking)* Then we extend the language.

SMASH TO MAIN TITLES.

**Who:** Daniel, Pliny, Carter.
**Timing:** 75 seconds.

---

## ACT ONE: "Crudus" (The Raw)

*The first extension. Raw content tags solve the self-description problem. STML can now contain itself.*

### Beat 1: Carter's Proposal — The Exclamation Mark

**Location:** The Tabularium. Morning session. Carter at the board.

**CARTER:** The problem is that the parser treats ALL angle brackets as structure. We need a way to say: "Inside THIS tag, angle brackets are just text. Don't parse them." A raw content tag.

She writes:

```
<code!>
  To create an element, write <elementum> and close it with </elementum>.
</code>
```

**CARTER:** The exclamation mark after the tag name — `code!` — tells the parser: "This is a raw content tag. Everything inside it is literal text. No parsing. No nesting. Just characters."

**DATA:** *(precise)* The parser reads the opening tag, recognizes the `!` suffix, and then scans forward for the matching close tag WITHOUT interpreting any content between them. The content is preserved exactly as written.

Daniel tries it:

```c
/* Elementum crudum — raw content */
StmlNodus* codex = stml_elementum_crudum_creare(
    piscina, intern, "code");

/* The raw tag preserves everything */
StmlResultus res = stml_legere_ex_literis(
    "<code!>To create <elementum>, write <elementum></code>",
    piscina, intern);

StmlNodus* code_nodus = stml_invenire_liberum(
    res.elementum_radix, "code");

/* code_nodus->crudus == VERUM */
/* textus internus = "To create <elementum>, write <elementum>" */
```

**DANIEL:** *(reading the output)* It preserved the angle brackets. The parser saw them as text, not structure.

**PLINY:** *(immediately seeing the implication)* Then the manual can exist. A document about STML, written in STML, containing STML examples that are not parsed. The encyclopedia can contain itself.

**CARTER:** `stml_elementum_crudum_creare` — create a raw content element. The `crudus` field on the node is set to `VERUM`. The serializer knows to write the `!` suffix. The round trip preserves the raw content.

**LUCIA:** *(studying the notation)* The exclamation mark. It is a warning — "Danger: raw content. Enter at your own risk." Like a sign at the gate of a quarry.

**O'NEILL:** *(from the back)* So it's the "don't touch this" tag.

**CARTER:** *(nodding)* Exactly. The parser's hands are tied. It can only look for the closing tag.

**Who:** Carter, Data, Daniel, Pliny, Lucia, O'Neill.

### Beat 2: The Manual Comes Alive

**Location:** The Tabularium. Daniel writing, Lucia reviewing.

Daniel writes the first page of the STML manual — in STML:

```xml
<manual titulus="Ars STML">
  <capitulum numerus="I" titulus="Elementa">
    <sectio titulus="Creare Elementum">
      <paragraphus>
        Elementum creatur cum signis angularibus.
        Exemplum:
      </paragraphus>
      <code!>
        <nomen>Gaius Aurelius</nomen>
        <munus>faber</munus>
      </code>
    </sectio>
  </capitulum>
</manual>
```

**DANIEL:** *(reading the rendered output)* The manual describes how to write markup, and the examples are preserved inside the document. We just taught the language to speak about itself.

**LUCIA:** *(quiet, precise)* Not just speak about itself. *Contain* itself. An STML document can now hold another STML document as content — not as structure, but as subject matter. The container and the contained coexist.

**PLINY:** *(the scholar's excitement)* My encyclopedia entry on trees contained no trees. Only descriptions of trees. Now — a document about documents can contain documents. The representation includes the thing being represented. *(beat)* This is what I always wanted from my marginalia. Not a description of the structure, but the structure itself, embedded in the text that describes it.

**MARCUS:** *(the engineer)* A building that contains its own blueprints. Inside the wall — the plans for the wall. Self-documenting. Self-describing.

**Who:** Daniel, Lucia, Pliny, Marcus.

### Beat 3: Teal'c Tests the Boundary

**Location:** The Tabularium. Teal'c at the terminal.

```c
/* What happens if the closing tag appears inside raw content? */
StmlResultus res = stml_legere_ex_literis(
    "<code!>This contains </code> as text</code>",
    piscina, intern);
```

**TEAL'C:** *(deliberately constructing the problem)* The raw content contains the closing tag as text. How does the parser distinguish between the literal `</code>` inside the content and the actual closing `</code>`?

**CARTER:** *(showing the result — it parsed correctly)* The first `</code>` closes the tag. The parser is greedy in the simplest way — when it finds the matching close tag, it closes. If you need the literal close tag in your content, you use nesting depth or break it across tags.

**TEAL'C:** *(testing further)*

```c
/* Nested raw tags? */
StmlResultus res2 = stml_legere_ex_literis(
    "<outer!>text <inner!>more</inner> end</outer>",
    piscina, intern);
/* The <inner!> is just text — not parsed inside <outer!> */
```

**TEAL'C:** Inside a raw tag, nothing is parsed. Not even other raw tags. *(beat)* This is absolute containment. The boundary is inviolable.

**CARTER:** That's the design. Raw means raw. No exceptions.

**TEAL'C:** *(a single nod)* Acceptable.

**ACT BREAK — Raw content tags solve self-description. The language can now contain itself.**

**Who:** Teal'c, Carter.

---

## ACT TWO: "Fragmenta" (The Fragments)

*Grouping without meaning. Parentheses for documents. And the label system.*

### Beat 4: The Grouping Problem

**Location:** The Tabularium. Pliny is trying to mark a passage in his encyclopedia.

**PLINY:** I have three observations that form a set — they are about related minerals from the same quarry. They belong together. But they are not a chapter. They are not a section. They have no name. They are simply... grouped. If I wrap them in an element, I give them a name they don't deserve. `<mineralia_stabiana>` implies they are a formal category. They are not. They are merely adjacent and related.

**CARTER:** Fragments. `<#>` and `</#>`. A container without a name. Grouping without semantic weight.

```c
StmlResultus res = stml_legere_ex_literis(
    "<capitulum>"
    "  <#>"
    "    <obs>Obsidianus — Stabiae</obs>"
    "    <obs>Pumex — Stabiae</obs>"
    "    <obs>Calcium — Stabiae</obs>"
    "  </#>"
    "  <obs>Ferrum — Hispania</obs>"
    "</capitulum>",
    piscina, intern);

/* The fragment groups the Stabiae minerals */
/* without asserting they are a named category */
StmlNodus* fragmentum = stml_primus_liberum(
    res.elementum_radix);
/* fragmentum->fragmentum == VERUM */
/* fragmentum->fragmentum_id == NIHIL (anonymous) */
```

**PLINY:** *(reading the markup)* `<#>`. No name. No assertion. Just grouping. *(beat)* Like parentheses in an equation. They change the order of operations without adding a new term.

**DANIEL:** *(immediately)* That's exactly right. In mathematics, parentheses are invisible semantically — `(a + b)` is the same value as `a + b`. But they change how you READ the expression. How you interpret the grouping. The fragment does the same thing for documents.

**MARCUS:** *(the engineer)* Scaffolding. The scaffolding holds the stones in place during construction. It has no architectural meaning. When the building is done, the scaffolding can be removed — or left in place for future renovation.

### Beat 5: Named Fragments and Labels

**Location:** The Tabularium. Carter extends the concept.

```c
/* Named fragments — for referencing later */
StmlResultus res = stml_legere_ex_literis(
    "<documento>"
    "  <#introduction>"
    "    <p>Haec est introductio.</p>"
    "  </#>"
    "  <#body>"
    "    <p>Hoc est corpus.</p>"
    "  </#>"
    "</documento>",
    piscina, intern);

StmlNodus* intro = stml_primus_liberum(res.elementum_radix);
/* intro->fragmentum == VERUM */
/* intro->fragmentum_id->data = "introduction" */
```

**CARTER:** Named fragments — `<#introduction>` — give the grouping an identifier without giving it semantic weight. The fragment is still invisible structurally. But it can be found by its ID.

**LUCIA:** *(studying the concept)* A label, not a category. You have named the scaffolding so the builder can find it again, but it remains scaffolding.

Carter then introduces the labels system:

```c
/* Tituli — labels for classification */
StmlNodus* obs = stml_elementum_creare(piscina, intern, "obs");

stml_titulum_addere(obs, piscina, intern, "minerale");
stml_titulum_addere(obs, piscina, intern, "stabiae");
stml_titulum_addere(obs, piscina, intern, "verificatum");

/* Check for label */
b32 est_minerale = stml_titulum_habet(obs, "minerale");
/* VERUM */

/* Count labels */
i32 n = stml_titulos_numerus(obs);
/* 3 */

/* Toggle */
stml_titulum_commutare(obs, piscina, intern, "dubium");
/* Now has: minerale, stabiae, verificatum, dubium */
```

In markup:

```xml
<obs labels="minerale stabiae verificatum">
  <nomen>Obsidianus</nomen>
</obs>
```

**CARTER:** Labels. Classification without hierarchy. An observation can be a mineral AND from Stabiae AND verified, without any of those being a nesting relationship. Labels are flat. Trees are hierarchical. You need both.

**PLINY:** *(immediately grasping the utility)* My encyclopedia uses three indexing systems: by subject, by location, by book number. An observation about obsidian from Stabiae in book thirty-six belongs to all three. With labels — `minerale stabiae liber_XXXVI` — it can be classified in three dimensions simultaneously without being nested three levels deep.

**CARTER:** And with namespaces — `labels="classis::minerale locus::stabiae liber::XXXVI"` — you can organize the labels themselves.

**PLINY:** *(writing, excited)* The `::` separates the system from the classification. `classis::minerale` means "in the classification system called 'classis,' this is 'minerale.'" *(beat)* I have been doing this on paper for sixty years. Separate indexes for separate systems. You have put them all in the same document.

**Who:** Carter, Pliny, Daniel, Lucia, Marcus.

### Beat 6: The Philosophical Moment — Lucia's Insight

**Location:** The Tabularium. Later. The team discussing the design philosophy.

**LUCIA:** *(who has been listening, studying the fragment and label syntax)* Your old system treated all things the same. Attribute, element, text — interchangeable containers. Your new system lets things be what they are. A fragment is not an element wearing a costume. It IS a fragment. A label is not an attribute pretending to be a category. It IS a classification. *(beat)* You have given the language room to grow because you have given its parts their proper nature.

**CARTER:** *(surprised by the philosophical precision)* That's exactly the design principle. Each construct does one thing. Elements hold content. Fragments group without meaning. Labels classify without nesting. Raw tags contain without parsing.

**LUCIA:** It is also the first principle of any sound philosophy. Things must be allowed to be what they are.

A beat. The room feels the weight of it. Lucia just described moderate realism as a software design principle. Nobody calls it that. It's just... true.

**ACT BREAK — Fragments and labels are established. Grouping without semantics. Classification without hierarchy.**

**Who:** Lucia, Carter.

---

## ACT THREE: "Captio" (The Capture)

*The most complex feature. Capture operators restructure the tree during parsing. This is where power becomes dangerous.*

### Beat 7: Casey's Warning — Setting the Stakes

**Location:** The workshop. Casey and Carter, early.

**CASEY:** You said capture operators. That's the one that worries me.

**CARTER:** It should. Capture operators let you restructure the document tree during parsing. Instead of nesting content manually, you write flat content and the parser nests it for you.

**CASEY:** *(leaning back)* So the parser is doing surgery. On itself. While running.

**CARTER:** *(nodding)* Forward capture grabs the next siblings and makes them children. Backward capture grabs previous siblings. Sandwich capture grabs one on each side.

**CASEY:** And if someone writes conflicting captures? Overlapping captures? Captures that reference each other?

**CARTER:** Then the parser reports an error — `STML_ERROR_CAPTIO`. The captures have rules. They can't overlap. They can't capture beyond their parent's boundary. *(beat)* But yes. This is the most dangerous feature we're building.

**CASEY:** *(after a moment)* Show me. If it can fail safely, I'll accept it. If it can fail silently, we don't ship it.

This establishes the dramatic stakes for the capture sequence: power vs. safety. Casey's engineering ethic — features that fail silently are worse than features that don't exist.

**Who:** Casey, Carter.

### Beat 8: Forward Capture — The First Demonstration

**Location:** The Tabularium. Carter demonstrates.

**CARTER:** Forward capture. The `(>` operator. When a tag has `(>`, it captures the next sibling and makes it a child.

She writes flat markup:

```
<section>
  <h1>Mineralia</h1>
  <wrapper (>
  <obs>Obsidianus</obs>
  <obs>Pumex</obs>
  <obs>Calcium</obs>
</section>
```

**CARTER:** Without the capture, `<h1>`, `<wrapper>`, and the three `<obs>` are all siblings — children of `<section>`. With `(>`, the `<wrapper>` reaches forward and captures its next sibling — `<obs>Obsidianus</obs>` — as a child.

The result tree:

```
section
├── h1: "Mineralia"
├── wrapper
│   └── obs: "Obsidianus"    ← captured
├── obs: "Pumex"
└── obs: "Calcium"
```

**MARCUS:** *(studying the tree)* The wrapper reached forward and took ownership. Like a foreman claiming a worker from the general labor pool. "You — you're with me now."

**CARTER:** And with `((>` — double parentheses — it captures TWO siblings:

```
<wrapper ((>
<obs>Obsidianus</obs>
<obs>Pumex</obs>
<obs>Calcium</obs>
```

Result:

```
section
├── h1: "Mineralia"
├── wrapper
│   ├── obs: "Obsidianus"    ← captured
│   └── obs: "Pumex"         ← captured
└── obs: "Calcium"
```

On screen, the code:

```c
/* StmlCaptioDirectio — capture direction */
/* STML_CAPTIO_ANTE = forward capture */
/* node->captio_directio and node->captio_numerus */

StmlResultus res = stml_legere_ex_literis(
    "<section>"
    "  <h1>Mineralia</h1>"
    "  <wrapper ((>"
    "  <obs>Obsidianus</obs>"
    "  <obs>Pumex</obs>"
    "  <obs>Calcium</obs>"
    "</section>",
    piscina, intern);

StmlNodus* wrapper = stml_invenire_liberum(
    res.elementum_radix, "wrapper");
/* wrapper->captio_directio == STML_CAPTIO_ANTE */
/* wrapper->captio_numerus == 2 */
i32 n = stml_numerus_liberorum(wrapper);
/* n == 2: Obsidianus, Pumex */
```

**DATA:** *(precise)* The capture count is stored in the node: `captio_numerus`. Forward capture with `((>` sets the count to two. The parser restructures the tree: the next two siblings become children of the capturing element. The tree in memory is identical to what you would get if you had nested them manually.

**DANIEL:** *(the linguist, hearing something)* The flat text describes a hierarchy — but the hierarchy isn't visible in the source. The captures make it visible only in the tree. *(beat)* This is like inflection in Latin. "Marcus amat Luciam" — the word order doesn't determine meaning. The ENDINGS do. The structure is in the morphology, not the position. Capture operators do the same thing: the structure is in the operators, not the nesting.

This is a significant observation — capture operators are STML's equivalent of grammatical inflection. The source code is "flat" (like Latin word order) but the operators encode the hierarchy (like Latin case endings). The tree is determined by meaning markers, not by physical nesting.

**Who:** Carter, Marcus, Data, Daniel.

### Beat 9: Backward Capture and Sandwich — The Full Toolkit

**Location:** The Tabularium.

**CARTER:** Backward capture — `<)`. The tag reaches BACKWARD and captures the PREVIOUS sibling.

```
<obs>Obsidianus</obs>
<) wrapper>
```

Result: `<obs>Obsidianus</obs>` becomes a child of `<wrapper>`, even though it appeared BEFORE `<wrapper>` in the source.

```c
/* STML_CAPTIO_RETRO = backward capture */
StmlResultus res = stml_legere_ex_literis(
    "<section>"
    "  <obs>Obsidianus</obs>"
    "  <) wrapper>"
    "</section>",
    piscina, intern);

StmlNodus* wrapper = stml_invenire_liberum(
    res.elementum_radix, "wrapper");
/* wrapper->captio_directio == STML_CAPTIO_RETRO */
/* Child: obs "Obsidianus" */
```

**CASEY:** *(studying the backward capture)* It rewrites history. The previous sibling ALREADY existed at the same level. The backward capture retroactively changes its position in the tree.

**CARTER:** Yes. And sandwich capture — `<= wrapper =>` — captures one previous AND one next:

```
<obs>Obsidianus</obs>
<= wrapper =>
<obs>Pumex</obs>
```

Result:

```
wrapper
├── obs: "Obsidianus"    ← captured backward
└── obs: "Pumex"         ← captured forward
```

```c
/* STML_CAPTIO_FARCIMEN = sandwich capture */
/* Captures one previous, one next */
```

**CASEY:** *(after a long beat)* These are powerful. *(looking at Carter)* What are the guardrails?

**CARTER:** Three rules. One: captures cannot exceed the parent boundary — you can't capture a sibling that belongs to a different parent. Two: captures cannot overlap — if two tags try to capture the same node, it's an error. Three: captures execute in document order — forward first, then backward, then sandwich.

```c
/* Error: overlapping captures */
StmlResultus res = stml_legere_ex_literis(
    "<a (><b (><c/></a>",   /* a and b both try to capture c */
    piscina, intern);

/* res.status == STML_ERROR_CAPTIO */
```

**CASEY:** *(nodding slowly)* It fails explicitly. The error code — `STML_ERROR_CAPTIO` — tells you what went wrong. *(beat)* Okay. I can live with that.

**ACT BREAK — Capture operators demonstrated. The most powerful and most dangerous feature. Casey is satisfied by the guardrails.**

**Who:** Carter, Casey, Data.

---

## ACT FOUR: "Transclusio" (The Inclusion)

*The document references itself. Deep clone. The title moment approaches.*

### Beat 10: Transclusion — The Document That Knows Itself

**Location:** The Tabularium. Carter introduces the final major feature.

**CARTER:** Transclusion. A document that references itself. `<<selector>>` — the double angle brackets — says: "Include the content found at this selector, right here."

```xml
<documento>
  <header id="main-header">
    <h1>Naturalis Historia Nova</h1>
    <auctor>Gaius Plinius Secundus</auctor>
  </header>

  <corpus>
    <!-- Include the header here too -->
    <<#main-header>>
  </corpus>
</documento>
```

**CARTER:** `<<#main-header>>` says: "Find the element with `id="main-header"` and include its content here." The header appears in the corpus without being copied.

On screen:

```c
/* STML_NODUS_TRANSCLUSIO — transclusion node */
StmlNodus* trans = stml_invenire_liberum(corpus, "");
/* Or, found via traversal */
/* trans->genus == STML_NODUS_TRANSCLUSIO */
/* trans->valor = "#main-header" (the selector) */
```

**PLINY:** *(immediately understanding)* Cross-referencing. My encyclopedia's index points to entries. The entries do not contain copies of the index — they point BACK to it. *(beat)* Your transclusion is the pointing. The document says "see also" without duplicating the content.

**DATA:** The transclusion node stores the selector as a string. Resolution — finding and inserting the referenced content — is a separate operation. The parser creates the placeholder. The application resolves it.

**CASEY:** *(cautious)* Circular references. If A includes B and B includes A...

**CARTER:** Depth limit. The resolver tracks what it's already expanded. If it encounters a cycle, it stops and marks the transclusion as unresolved. Explicit failure, not infinite recursion.

**CASEY:** *(satisfied)* Good.

**Who:** Carter, Pliny, Data, Casey.

### Beat 11: Deep Clone — Duplication as Independence

**Location:** The Tabularium. Carter demonstrates `stml_duplicare`.

**CARTER:** Sometimes you don't want a reference — you want a COPY. An independent duplicate. `stml_duplicare` — deep clone.

```c
/* Duplicare — deep clone */
StmlNodus* originale = stml_invenire_liberum(
    res.elementum_radix, "liber");

StmlNodus* copia = stml_duplicare(
    originale, piscina, intern);

/* copia is independent — modifying it does not affect originale */
stml_attributum_addere(copia, piscina, intern,
    "editio", "secunda");

/* originale has no "editio" attribute */
/* copia has editio="secunda" */
```

**PLINY:** *(the scholar, seeing the utility)* Variant editions. My encyclopedia has been copied many times — by scribes in Rome, in Byzantium, in the monasteries. Each copy diverges from the original. The scribes add notes, correct errors, sometimes introduce new errors. *(beat)* `stml_duplicare` creates a copy that begins as identical but is free to diverge. A new edition. Independent of the original, but born from it.

**CARTER:** Deep clone copies everything — all children, all attributes, all text. Shallow clone — `stml_duplicare_superficialiter` — copies only the node itself, without its children.

**PLINY:** *(quietly)* A copy that grows into its own tree. Like a cutting from a vine — it shares the rootstock but develops its own branches.

**DANIEL:** *(noting the botanical metaphor)* Stirps again. The cutting shares the stirps.

**Who:** Carter, Pliny, Daniel.

### Beat 12: Dependency Story — Extensions Without New Roots

**Location:** The Tabularium. Carter at the board, updating the dependency diagram.

```
STML Extensions — Episode 1x10
  Same dependencies as STML (1x09):
    Piscina (1x01)       — all nodes still arena-allocated
    Chorda (1x03)        — all strings still chordae
    ChordaAedificator    — serializer still builds output
    Xar (1x04)           — children/attributes still dynamic arrays
    Internamentum (1x06) — all names still interned

  NO new dependencies.
```

**CARTER:** Every new feature — raw content, fragments, labels, captures, transclusion, deep clone — uses the same infrastructure. No new libraries. No new dependencies. The tree grew new branches, but the roots are the same.

**DATA:** *(precisely)* The raw content flag is a single boolean on the node — `crudus`. Fragments add a boolean and an optional ID — `fragmentum`, `fragmentum_id`. Captures add a direction enum and a count — `captio_directio`, `captio_numerus`. Labels are stored as a space-separated attribute. Transclusion is a new node type — `STML_NODUS_TRANSCLUSIO`. Each extension is a small addition to the existing `StmlNodus` structure. The architecture was designed to accommodate growth.

**MARCUS:** *(the engineer)* The foundation bore the load. We did not need to reinforce it. That is the mark of a good foundation — when the building grows, the base does not need to be rebuilt.

**Who:** Carter, Data, Marcus.

### Three-Voice Zone (Late Act 4)

This is the natural place for the three-voice to develop — after all features have been demonstrated, as the team reflects on what they've built. [DEFERRED to character/thread writers to specify speakers and content — the A-plot pitch notes the dramatic placement.]

---

## ACT FIVE: "Lingua Nova" (The New Language)

*The features converge. The full power of STML is demonstrated. The title moment lands.*

### Beat 13: The Convergence — All Features Working Together

**Location:** The Tabularium. Casey runs the full demonstration. A document that uses EVERY new feature:

```xml
<manual titulus="Ars STML" labels="documentatio lingua::stml">

  <#introduction>
    <h1>Introductio</h1>
    <p>STML est lingua structurae...</p>
  </#>

  <capitulum titulus="Elementa Cruda">
    <p>Elementum crudum continet textum sine parsatione:</p>
    <code!>
      <exemplum>Hoc est <elementum> in textu crudo.</exemplum>
    </code>
  </capitulum>

  <capitulum titulus="Captio">
    <p>Captio ante:</p>
    <wrapper (>
    <demonstratio>Haec demonstratio capta est.</demonstratio>
  </capitulum>

  <appendix>
    <<#introduction>>
  </appendix>

</manual>
```

```c
StmlResultus res = stml_legere_ex_literis(
    manual_stml, piscina, intern);

/* Verify raw content */
StmlNodus* code = /* ... navigate to code element ... */;
b32 est_crudus = code->crudus;
/* VERUM — raw content preserved */

/* Verify fragment */
StmlNodus* intro = stml_primus_liberum(res.elementum_radix);
b32 est_fragmentum = intro->fragmentum;
/* VERUM — anonymous fragment */

/* Verify labels */
b32 est_doc = stml_titulum_habet(res.elementum_radix, "documentatio");
/* VERUM */

/* Verify capture */
StmlNodus* wrapper = /* ... navigate to wrapper ... */;
i32 captio_n = wrapper->captio_numerus;
/* 1 — captured one sibling */

/* Verify transclusion */
StmlNodus* appendix = /* ... navigate to appendix ... */;
StmlNodus* trans = stml_primus_liberum(appendix);
/* trans->genus == STML_NODUS_TRANSCLUSIO */
/* trans->valor = "#introduction" */
```

**CASEY:** One document. Six new features. Raw content preserves code examples. Fragments group the introduction without naming it. Labels classify the document across multiple dimensions. Capture operators restructure the tree from flat source. Transclusion references content without copying it. *(beat)* And the round trip works.

```c
chorda output = stml_scribere(res.radix, piscina, VERUM);
imprimere("%.*s\n", (integer)output.longitudo, output.data);
```

The serialized output appears — indented, readable, preserving every feature. The round trip is complete.

**Who:** Casey, full team watching.

### Beat 14: The Title Moment

**Location:** The Tabularium. Late afternoon. The team gathered.

Lucia stands. She has been listening to six features being introduced, has watched the language grow from XML-that-speaks-Latin into something genuinely new.

**LUCIA:** *(slowly, choosing her words)* In the morning, Daniel discovered that our language could not describe itself. By afternoon, it can describe anything — including itself, including other languages, including documents that are still being written, including documents that reference themselves. *(beat)* This is no longer a dialect of someone else's notation. This is our language. A new language. Not translated. Born here.

She writes on the board:

**LINGUA NOVA STRUCTURAE**

**LUCIA:** A new language of structure. Not a language that describes structure — a language that IS structure. The raw tags are its alphabet for containment. The fragments are its parentheses. The labels are its adjectives. The captures are its inflections. The transclusions are its cross-references. And the deep clone is its power to propagate.

**DANIEL:** *(looking at the board, at the Latin words, at the features list)* We started with angle brackets. Now we have a philosophy. *(beat)* Every extension Lucia described maps to something natural. Parentheses exist in every language. Inflections exist in every language. Cross-references exist in every library. We didn't invent these features. We recognized that documents needed them and gave them names.

**PLINY:** *(the last word, the old scholar who has seen every system of notation from wax to screen)* In Rome, when a law was written, it was posted on the wall of the Tabularium for all to read. The structure of the law — the preamble, the articles, the penalties — was marked by different hand sizes and indentation. Everyone could read it because the structure was visible. *(beat)* You have done the same thing for every document that will ever be written here. The structure is in the text. The language describes itself. The author and the machine read the same document and see the same tree.

**Who:** Lucia, Daniel, Pliny, full team.

### Beat 15: Full Serialization — The Complete Round Trip

**Location:** The Tabularium. Casey runs the final test.

```c
/* The complete manual — with all features */
chorda output = stml_scribere(res.radix, piscina, VERUM);

/* Save to disk */
filum_scribere_totum("ars_stml.stml", output, piscina);

/* Read back */
chorda lecta = filum_legere_totum("ars_stml.stml", piscina);
StmlResultus res2 = stml_legere(lecta, piscina, intern);

/* Verify: same structure */
/* Raw tags preserved. Fragments preserved. Labels preserved. */
/* Captures produce the same tree. Transclusions survive. */
```

**CASEY:** Parse. Query. Modify. Serialize. Save. Read. Parse again. Every new feature survives the round trip. Raw content comes back raw. Fragments come back as fragments. Labels come back labeled. Captures produce the same tree. *(beat)* The language is complete.

**CARTER:** *(correcting, gently)* The language is never complete. It's designed to grow. But the foundation for growth — the framework that lets new features be added without breaking old documents — that's complete.

**CASEY:** *(a small smile — she's right, and he knows it)* Fair enough. The framework is complete.

**Who:** Casey, Carter.

---

## TAG

### Beat 16: Daniel's Manual — Alone

**Location:** Daniel's workspace. Late evening. Daniel is writing the STML manual — the one he couldn't write in the cold open. Now he can:

```xml
<manual titulus="Ars STML" labels="documentatio lingua::stml versio::prima">
  <#praefatio>
    <p>Haec ars STML scripta est in STML.</p>
    <p>Lingua se ipsam describit.</p>
  </#>
  <capitulum numerus="I" titulus="Fundamenta">
    <sectio titulus="Quid est STML?">
      <p>STML est lingua structurae textus...</p>
      <code!>
        <elementum attributum="valor">textus</elementum>
      </code>
    </sectio>
  </capitulum>
</manual>
```

He pauses. Types one more line:

```xml
  <!-- Haec lingua nata est in Nova Roma. Non translata — nata. -->
```

*This language was born in Nova Roma. Not translated — born.*

**Who:** Daniel alone.
**Timing:** 45 seconds.

### Beat 17: Pliny's Variant Edition

**Location:** Pliny's villa. Late evening. Pliny duplicates his encyclopedia:

```c
StmlNodus* editio_prima = /* ... his original encyclopedia ... */;

/* Create a variant — for the Roman engineers */
StmlNodus* editio_technica = stml_duplicare(
    editio_prima, piscina, intern);

stml_attributum_addere(editio_technica, piscina, intern,
    "editio", "technica");
stml_attributum_addere(editio_technica, piscina, intern,
    "destinatio", "fabri et architecti");
```

The technical edition. Born from the master. Free to diverge. Pliny starts adding engineering-specific annotations that the general edition doesn't need. Two trees from one stirps.

**PLINY:** *(to himself, typing)* Two editions. From one root. The monks of Montecassino would understand.

**Who:** Pliny alone.
**Timing:** 30 seconds.

---

## FEATURE-TO-DRAMATIC-MOMENT MAP

| STML Extension | API / Syntax | Dramatic Moment | Beat |
|---|---|---|---|
| **Raw Content Tags** | `stml_elementum_crudum_creare()`, `<tag!>`, `nodus->crudus` | Daniel can't write a manual about STML in STML. Raw tags solve it — the language contains itself. | Cold Open, 1-3 |
| **Fragments** | `<#>`, `<#id>`, `nodus->fragmentum`, `nodus->fragmentum_id` | Pliny needs to group without naming. Fragments = parentheses for documents. | 4-5 |
| **Labels** | `stml_titulum_habet()`, `stml_titulum_addere()`, `stml_titulos_numerus()`, `stml_titulum_commutare()`, `labels="ns::value"` | Pliny's multi-dimensional classification. Labels with namespaces replace separate index systems. | 5-6 |
| **Forward Capture** | `<tag (>`, `<tag ((>`, `STML_CAPTIO_ANTE`, `captio_numerus` | Flat markup restructured into hierarchy during parse. Inflection, not nesting. | 8 |
| **Backward Capture** | `<) tag>`, `STML_CAPTIO_RETRO` | The tag that rewrites history — retroactive nesting. | 9 |
| **Sandwich Capture** | `<= tag =>`, `STML_CAPTIO_FARCIMEN` | Captures both directions. Full restructuring power. | 9 |
| **Capture Error** | `STML_ERROR_CAPTIO` | Casey's test: overlapping captures fail explicitly, not silently. | 9 |
| **Transclusion** | `<<selector>>`, `STML_NODUS_TRANSCLUSIO` | Document references itself. Pliny: "See also" without duplication. | 10 |
| **Deep Clone** | `stml_duplicare()` | Pliny's variant editions. A copy that grows into its own tree. | 11 |
| **Shallow Clone** | `stml_duplicare_superficialiter()` | Mentioned alongside deep clone. Copy without children. | 11 |
| **No New Dependencies** | Same as 1x09 | The foundation bore the load. Extensions don't require new roots. | 12 |

---

## ON-SCREEN CODE MOMENTS

### Moment 1: The Self-Description Failure (Cold Open)
```xml
<code>
  To create an element, write <elementum> and close it with </elementum>.
</code>
```
The parser chokes. The problem that motivates everything.

### Moment 2: Raw Content Creation (Act 1)
```c
StmlNodus* codex = stml_elementum_crudum_creare(piscina, intern, "code");
```
The API for creating raw nodes. `crudus` flag.

### Moment 3: Raw Content Parsing (Act 1)
```c
StmlResultus res = stml_legere_ex_literis(
    "<code!>To create <elementum>, write <elementum></code>",
    piscina, intern);
```
The `!` suffix in action. Angle brackets preserved as text.

### Moment 4: Fragment Grouping (Act 2)
```xml
<#>
  <obs>Obsidianus — Stabiae</obs>
  <obs>Pumex — Stabiae</obs>
</#>
```
Grouping without semantic weight.

### Moment 5: Labels with Namespaces (Act 2)
```xml
<obs labels="classis::minerale locus::stabiae liber::XXXVI">
```
Multi-dimensional classification in a single attribute.

### Moment 6: Label API (Act 2)
```c
b32 est_minerale = stml_titulum_habet(obs, "minerale");
stml_titulum_commutare(obs, piscina, intern, "dubium");
```
Check, add, remove, toggle labels programmatically.

### Moment 7: Forward Capture (Act 3)
```
<wrapper ((>
<obs>Obsidianus</obs>
<obs>Pumex</obs>
```
Flat source. Hierarchical tree. The parser restructures.

### Moment 8: Capture Direction Enum (Act 3)
```c
/* wrapper->captio_directio == STML_CAPTIO_ANTE */
/* wrapper->captio_numerus == 2 */
```
The data structure behind capture operators.

### Moment 9: Backward Capture (Act 3)
```c
StmlResultus res = stml_legere_ex_literis(
    "<section><obs>Obsidianus</obs><) wrapper></section>",
    piscina, intern);
```
Retroactive restructuring. The tag that rewrites the tree.

### Moment 10: Capture Error (Act 3)
```c
/* res.status == STML_ERROR_CAPTIO */
```
Explicit failure for overlapping captures. Casey's requirement.

### Moment 11: Transclusion (Act 4)
```xml
<<#main-header>>
```
The document references itself. STML_NODUS_TRANSCLUSIO.

### Moment 12: Deep Clone (Act 4)
```c
StmlNodus* copia = stml_duplicare(originale, piscina, intern);
stml_attributum_addere(copia, piscina, intern, "editio", "secunda");
```
Independent copy. Variant editions.

### Moment 13: Full Convergence (Act 5)
All six features in one document. The complete round trip. Parse, serialize, save, read, parse again.

### Moment 14: The Manual Exists (Tag)
```xml
<!-- Haec lingua nata est in Nova Roma. Non translata — nata. -->
```
Daniel writes the manual that was impossible in the cold open.

---

## DRAMATIC ENGINE SUMMARY

The episode follows a **problem → solution → philosophy** arc:

**Act 1 (Raw Tags):** "The language can't contain itself." → It can now.
**Act 2 (Fragments/Labels):** "We need grouping without meaning and classification without hierarchy." → Both solved.
**Act 3 (Capture Operators):** "Power is dangerous." → Power with guardrails is necessary.
**Act 4 (Transclusion/Clone):** "The document should reference itself." → It can, safely.
**Act 5 (Convergence):** "All features together. The round trip works."
**Title Moment:** "This is no longer someone else's notation. This is our language."

The emotional progression: frustration (can't self-describe) → relief (raw tags) → elegance (fragments/labels) → tension (captures are dangerous) → satisfaction (guardrails work) → awe (all features converge) → ownership (this language was born here).

---

## NOTES FOR OTHER WRITERS

- **Three-voice placement:** Late Act 4 is the natural spot, after all features are demonstrated and the dependency story is told. Suggested voices: any three who haven't been spotlighted recently, but the A-plot pitch defers speaker selection to the character writer.
- **Casey's engineering ethic** — "features that fail silently are worse than features that don't exist" — is the moral backbone of Act 3. He accepts capture operators ONLY because they fail explicitly.
- **Lucia's philosophical moment** (Beat 6) is the episode's thesis statement: "Things must be allowed to be what they are." This is moderate realism as software design. The thread writer will want to track this.
- **Daniel's arc:** Cold open he can't write the manual. Tag he writes it. The episode is bookended by his need and its fulfillment.
- **The language-as-inflection insight** (Beat 8, Daniel's observation about capture operators as Latin case endings) is potentially a three-voice seed — Latin grammar as a model for markup, where meaning is encoded in morphology not position.
- **No new dependencies** is a feature, not a limitation. The architecture was designed to grow. This should land emotionally — Marcus's "the foundation bore the load."
