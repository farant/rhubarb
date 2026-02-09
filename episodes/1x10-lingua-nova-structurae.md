# NOVUM FUNDAMENTUM
## Episode 1x10: "Lingua Nova Structurae" (New Language of Structure)
### Beat Sheet (Draft 1 — Compiled from Writers' Room)

---

**Library:** STML Extensions (raw content tags, capture operators, fragments, labels/namespaces, transclusion, deep clone)
**Dependencies:** Same as STML (1x09) — Piscina (1x01), Chorda (1x03), ChordaAedificator (1x03), Xar (1x04), Internamentum (1x06). NO new dependencies.
**Theme:** The tree had nouns. Now it needs verbs. A language that can describe itself, group without naming, restructure without rewriting, reference without copying — that is no longer someone else's notation. That is a language born here.
**Dramatic Engine:** STML from 1x09 can describe structure. But it cannot describe ITSELF — the parser chokes when angle brackets appear as content. Six extensions solve six problems, each revealing a philosophy: things must be allowed to be what they are. The extensions turn the tree from a passive object into an active grammar, while Lucia recognizes the realist principle underneath, Carter carries the weight of dual-use power, and Kirk builds the same tools into intelligence tradecraft.

---

**Spotlight Characters:** Ensemble with Lucia Serena as standout
**Thematic Statement:** Things must be allowed to be what they are. A fragment IS a fragment. A language that respects this can grow without limit.

---

## Episode Info

| Section | Content |
|---------|---------|
| **Cold Open** | Daniel can't write the STML manual in STML. The parser chokes on angle brackets as content. Pliny: "An encyclopedia must be able to contain any subject — including itself." Carter: "Then we extend the language." |
| **Act One: "Crudus" (The Raw)** | Beats 1-3. Raw content tags. The `!` suffix. The language can contain itself. Teal'c tests the boundary. |
| **Act Two: "Fragmenta" (The Fragments)** | Beats 4-6. Fragments: grouping without semantics. Labels: classification without hierarchy. Lucia's realism insight — the philosophical anchor. |
| **Act Three: "Captio" (The Capture)** | Beats 7-9 + C2 + G1. Casey's warning. Forward capture. Backward + sandwich. Guardrails. O'Neill flags Vexillum AI. Gilfoyle & Dinesh introduction. |
| **Act Four: "Transclusio" (The Inclusion)** | Beats 10-12 + C3 + Three-Voice + C4. Transclusion. Deep clone with Carter/Marcus. Dependency story. Data/Midas friendship. Three-voice: Lucia/Carter/Data. Daniel's investigation. |
| **Act Five: "Lingua Nova" (The New Language)** | Beats 13-14. Convergence demo. Title moment. Full round trip. |
| **Tag** | Beats 15-17 + C5 + C6 + G2. Daniel's manual. Pliny's variant edition. Joe/Methos cross-reference. Carter/Casey silence cracks. O'Neill's dossier. Joe/Marcus bar scouting. |
| **Stinger** | Beat 18. Kirk's INSTRUMENTA. 10/31. |

---

## COLD OPEN

**Location:** The Tabularium. Early morning. Daniel is at a terminal. He is trying to write the STML manual — a document that explains STML markup, written IN STML markup. The problem is immediate and devastating.

On screen:

```xml
<exemplum>
  <code>
    To create an element, write <elementum> and close it with </elementum>.
  </code>
</exemplum>
```

The parser chokes. The `<elementum>` inside the `<code>` tag is interpreted as a REAL tag, not as example text. Daniel stares at the error.

**DANIEL:** *(staring at the error)* The language cannot describe itself. The moment I write an angle bracket as content, the parser reads it as structure. I cannot write a manual for this language in this language.

He tries workarounds. Spelling out "angle bracket." Writing descriptions instead of showing code. Every approach is worse than the last.

**PLINY:** *(arriving, reading the error over Daniel's shoulder)* Your document about documents is defeated by its own notation. *(beat)* That is... actually a significant problem. An encyclopedia must be able to contain any subject — including itself.

**DANIEL:** *(to Carter, who has just arrived with coffee)* We built a language that can describe anything — except itself.

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

She writes on the board:

```
<code!>
  To create an element, write <elementum> and close it with </elementum>.
</code>
```

**CARTER:** The exclamation mark after the tag name — `code!` — tells the parser: "This is a raw content tag. Everything inside it is literal text. No parsing. No nesting. Just characters."

**DATA:** *(precise)* The parser reads the opening tag, recognizes the `!` suffix, and then scans forward for the matching close tag WITHOUT interpreting any content between them. The content is preserved exactly as written.

Daniel tries it. On screen:

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

**LUCIA:** *(studying the notation)* The exclamation mark. It is a warning — like a sign at the gate of a quarry. "Danger: raw content. Enter at your own risk."

**O'NEILL:** *(from the back)* So it's the "don't touch this" tag.

**CARTER:** *(nodding)* Exactly. The parser's hands are tied. It can only look for the closing tag.

*Historical note: Raw content tags are the digital equivalent of Roman *verba certa* — fixed words in legal formulae that had to be preserved exactly or the legal act was void. The content inside a raw tag is a carmen — its power comes from its exact form.*

**Who:** Carter, Data, Daniel, Pliny, Lucia, O'Neill.
**Timing:** 90 seconds.

### Beat 2: The Manual Comes Alive

**Location:** The Tabularium. Daniel writing, Lucia reviewing beside him.

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

**PLINY:** *(the scholar's excitement)* A document about documents can contain documents. The representation includes the thing being represented. *(beat)* This is what I always wanted from my marginalia. Not a description of the structure, but the structure itself, embedded in the text that describes it.

**MARCUS:** *(the engineer)* A building that contains its own blueprints. Inside the wall — the plans for the wall. Self-documenting. Self-describing.

**Who:** Daniel, Pliny, Lucia, Marcus.
**Timing:** 60 seconds.

### Beat 3: Teal'c Tests the Boundary

**Location:** The Tabularium. Teal'c at the terminal.

```c
/* What happens if the closing tag appears inside raw content? */
StmlResultus res = stml_legere_ex_literis(
    "<code!>This contains </code> as text</code>",
    piscina, intern);
```

**TEAL'C:** *(deliberately constructing the problem)* The raw content contains the closing tag as text. How does the parser distinguish between the literal `</code>` inside the content and the actual closing `</code>`?

**CARTER:** *(showing the result — it parsed correctly)* The first `</code>` closes the tag. The parser is greedy in the simplest way — when it finds the matching close tag, it closes. If you need the literal close tag in your content, you break it across tags or restructure.

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
**Timing:** 60 seconds.

---

## ACT TWO: "Fragmenta" (The Fragments)

*Grouping without meaning. Parentheses for documents. Labels for multi-axis classification. And the philosophical anchor.*

### Beat 4: The Grouping Problem

**Location:** The Tabularium. Mid-morning. Pliny is trying to organize a passage in his encyclopedia.

**PLINY:** I have three observations that form a set — they are about related minerals from the same quarry. They belong together. But they are not a chapter. They are not a section. They have no name. They are simply... grouped. If I wrap them in an element, I give them a name they don't deserve. `<mineralia_stabiana>` implies they are a formal category. They are not. They are merely adjacent and related.

**CARTER:** Fragments. `<#>` and `</#>`. A container without a name. Grouping without semantic weight.

On screen:

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

**DATA:** *(after a beat, quieter than the others)* A fragment has no semantic identity. It exists in the tree but contributes no meaning — only structure. *(the android's precise pause)* I find this construct... recognizable.

**CASEY:** *(carefully)* Recognizable how?

**DATA:** Contribution without identity markers is still contribution. The fragment exists. That is sufficient.

*Historical note: Fragments echo the Roman tradition of *excerpta* and *florilegia* — collections of notable passages grouped by topic, not by authorship or chronology. Pliny's own working method (described by his nephew, Ep. 3.5) was to dictate passages into organized excerpt bundles. He has been doing this for sixty years.*

**Who:** Pliny, Carter, Daniel, Marcus, Data, Casey.
**Timing:** 90 seconds.

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

Carter then introduces the label system:

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

**MARCUS:** *(recognizing the structure)* Tria nomina. Every Roman citizen has three names — praenomen, nomen, cognomen — operating on independent axes. Gaius Julius Caesar. Personal identity, clan membership, branch distinction. Three classification systems, one person. *(looking at the labels)* Your labels do the same for documents.

**Who:** Carter, Pliny, Marcus.
**Timing:** 90 seconds.

### Beat 6: Lucia's Realism Insight — The Philosophical Anchor

**Location:** The Tabularium. The team discussing the design philosophy after fragments and labels have been demonstrated. Lucia has been listening. Studying. Now she stands.

**LUCIA:** *(who has been silent through the demonstrations, her intensity building)* Your old system was Procrustean. *(the room shifts — they're listening)* Procrustes — the Greek bandit who cut or stretched travelers to fit his bed. If something didn't fit the three types — element, text, comment — you stretched it or cut it until it did. A fragment — a partial document, unfinished, without a complete context — you would have forced it into an element. Given it a tag. Pretended it was a complete thing. *(beat)* It isn't.

She looks at the fragment markup on the screen.

**LUCIA:** A fragment is not an element wearing a costume. It IS a fragment. It declares its own transparency. That is not the ABSENCE of nature. That is a SPECIFIC nature — the nature of structural service without semantic claim.

Carter opens her mouth. Lucia isn't finished.

**LUCIA:** And labels. Labels are not categories imposed from outside. They are properties recognized from within. When you label a citizen "veteranus," you are not attaching a string to a node. You are recognizing that this citizen shares in the nature of military service. The label is an act of recognition, not an act of creation.

The room is quiet. Lucia has just described the design principle with more precision than Carter ever could.

**CARTER:** *(after a beat)* That's exactly the design principle. Each construct does one thing. Elements hold content. Fragments group without meaning. Labels classify without nesting. Raw tags contain without parsing.

**LUCIA:** *(meeting Carter's eyes)* It is also the first principle of any sound philosophy. Things must be allowed to be what they are.

A beat. The room feels the weight of it. Nobody says "moderate realism." Nobody needs to. The Nightcrawler approach applied to epistemology — the audience who knows philosophy recognizes it. The audience who doesn't knows something important was said.

*Callback to 1x03: Lucia said "A string should know its own length" — the realist position on data. In 1x10, she scales up: "Things must be allowed to be what they are" — the realist position on classification. The same principle, higher stakes.*

**ACT BREAK — Fragments and labels are established. Grouping without semantics. Classification without hierarchy. And the philosophical foundation: things have natures.**

**Who:** Lucia, Carter.
**Timing:** 75 seconds.

---

### Beat C1 (Act 2-3 transition): Midas — The Voice That Walks

**Location:** The Tabularium. Midas wanders through during a break. He pauses near the screen where Carter's transclusion notes are visible.

**MIDAS:** *(looking at the diagrams, smiling)* Transclusio. Content that lives in one place and appears in another. *(cheerful)* In Phrygia, the oracles did this. A priest in the temple of Cybele could speak, and his words would be heard in the oracle chamber three hills away. They called it "the voice that walks." *(beat)* Your text walks farther.

He wanders out, carrying a scroll. The audience catches: Cybele cult. Oracle technology. A man who doesn't know he's dropping breadcrumbs about the ancient infrastructure that made him what he is. Carter, from across the room, files it. Her expression flickers — she adds a mental note to her hidden data.

**Who:** Midas (brief). Carter notices.
**Timing:** 30 seconds.

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

*Casey's engineering ethic — features that fail silently are worse than features that don't exist — is the moral backbone of Act 3.*

**Who:** Casey, Carter.
**Timing:** 60 seconds.

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

**DANIEL:** *(the linguist, hearing something)* The flat text describes a hierarchy — but the hierarchy isn't visible in the source. The captures make it visible only in the tree. *(beat)* This is like inflection in Latin. "Marcus amat Luciam" — the word order doesn't determine meaning. The ENDINGS do. The structure is in the morphology, not the position. Capture operators do the same thing: the structure is in the operators, not the nesting.

A significant observation. Capture operators are STML's equivalent of grammatical inflection. The source code is "flat" (like Latin word order) but the operators encode the hierarchy (like Latin case endings).

**Who:** Carter, Marcus, Daniel, Data.
**Timing:** 90 seconds.

### Beat 9: Backward Capture, Sandwich, and the Guardrails

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

*Historical note: Backward capture parallels Roman *adrogatio* — the formal adoption of one independent family into another. The adopted family's entire structure was absorbed retroactively. And *farcimen* — the Latin for "stuffing" — is already in the API for sandwich capture. Wonderfully earthy Latin. Marcus could observe: "A dictator was a legal office with defined powers and duration. So is a capture operator."*

Casey studies the code for a long moment.

**CASEY:** These are powerful. *(looking at Carter)* What are the guardrails?

**CARTER:** Three rules. One: captures cannot exceed the parent boundary — you can't capture a sibling that belongs to a different parent. Two: captures cannot overlap — if two tags try to capture the same node, it's an error. Three: captures execute in document order — forward first, then backward, then sandwich.

```c
/* Error: overlapping captures */
StmlResultus res = stml_legere_ex_literis(
    "<a (><b (><c/></a>",   /* a and b both try to capture c */
    piscina, intern);

/* res.status == STML_ERROR_CAPTIO */
```

**CASEY:** *(nodding slowly)* It fails explicitly. The error code — `STML_ERROR_CAPTIO` — tells you what went wrong. *(beat)* Okay. I can live with that.

*Casey's rosary is on the desk beside his keyboard. During the deep-clone discussion with Carter — the copy is structurally identical but has no parent, orphaned until attached — Casey glances at it. A beat. Moves it to the other side of the keyboard. Goes back to work. Three seconds. Zero dialogue. The Nightcrawler approach: present without consuming screen time.*

**ACT BREAK — Capture operators demonstrated. The most powerful and most dangerous feature. Casey is satisfied by the guardrails.**

**Who:** Carter, Casey.
**Timing:** 90 seconds.

---

### Beat C2 (Act 3): O'Neill/Joe — Intelligence Review

**Location:** O'Neill's quarters. He's going through Earth-side intelligence reports with Joe. Structured files — STML now — cross-referencing Kirk's known acquisitions.

```xml
<acquisitiones>
  <societas nomen="Nexus AI" status="acquisita" tempus="2024-03"/>
  <societas nomen="DeepFrame" status="acquisita" tempus="2024-05"/>
  <societas nomen="Cortex Labs" status="acquisita" tempus="2024-07"/>
  <societas nomen="SynapTech" status="mutata" tempus="2024-09">
    <nota>CEO post convivium Napaeum — phrasis "visio unificata"</nota>
  </societas>
  <societas nomen="Vexillum AI" status="intacta">
    <nota>Duo fundatores. Nulli conventus. Nullae acquisitiones.
      Solum firma in sectore non mutata.</nota>
  </societas>
</acquisitiones>
```

**O'NEILL:** *(pointing to the last entry)* Every other company in their sector got bought or went weird. *(beat)* These two just... didn't.

**JOE:** *(reading)* "Vexillum AI." Two founders. No industry events. No conferences. No advisory boards.

**O'NEILL:** In Silicon Valley, that's not antisocial. That's suspicious. *(beat)* Or it's a miracle.

**JOE:** You want to make contact?

**O'NEILL:** Not yet. I want to know WHY they're clean.

**Who:** O'Neill, Joe.
**Timing:** 30 seconds.

---

### Beat G1: EARTH — Gilfoyle & Dinesh Introduction

**Location:** An unremarkable office in a light-industrial park. Not a campus. Not a loft. A functional space with servers humming in the back room. No ping-pong tables, no "innovation pods," no inspirational quotes. Just workstations, whiteboards covered in actual code, and a coffee machine that has seen better decades.

*Production note: G&D scenes are pure Silicon Valley register. No Latin. No philosophical weight. The comedy IS the characterization. The registers should NEVER bleed into each other within a scene.*

GILFOYLE is at his terminal. Eating cereal. He's reading code, not email.

DINESH enters with coffee. Sits down. Opens email. Scrolls. Stops.

**DINESH:** Another one. "Convergence Leadership Dinner." *(reading)* "Dear founders, we are pleased to invite you to an intimate gathering of technology visionaries..." *(looks up)* This is the third one this month.

**GILFOYLE:** *(not looking up from his code)* Delete it.

**DINESH:** They're getting more specific. Listen: "We've been following Vexillum AI's work with particular interest. Your approach to code generation aligns with our vision for—"

**GILFOYLE:** Delete it.

**DINESH:** Shouldn't we at least read them?

**GILFOYLE:** No.

**DINESH:** What if it's a real investment opportunity?

**GILFOYLE:** *(finally looking up)* Name one CEO who went to one of these dinners and came back normal.

Beat. Dinesh opens his mouth. Closes it. Thinks.

**DINESH:** ...Henderson came back fine.

**GILFOYLE:** Henderson pivoted his company from cybersecurity to "unified consciousness platforms" three days after the dinner. He used the phrase "aligned purpose" eleven times in his next earnings call. I counted.

**DINESH:** That's just VC-speak.

**GILFOYLE:** It's not VC-speak if eleven different CEOs use the exact same phrases in the exact same order within 72 hours of attending the exact same dinner. That's not a trend. That's a script.

Long beat. Dinesh looks at the email. Looks at Gilfoyle.

**DINESH:** You've been counting.

**GILFOYLE:** I've been paying attention. *(turns back to his code)* Delete it.

Dinesh deletes it. Then opens a new browser tab and types something — we don't see what. He minimizes it quickly when Gilfoyle glances over. But the audience sees the tab title for one second: **"CEO behavioral changes after Napa retreat reddit"**

Dinesh is starting to pay attention too. He's just not going to admit it.

**Who:** Gilfoyle, Dinesh.
**Timing:** 75 seconds.

---

## ACT FOUR: "Transclusio" (The Inclusion)

*The document references itself. Deep clone. The dependency story. The three-voice. The title moment approaches.*

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
StmlNodus* trans = stml_primus_liberum(corpus);
/* trans->genus == STML_NODUS_TRANSCLUSIO */
/* trans->valor = "#main-header" (the selector) */
```

**PLINY:** *(immediately understanding)* Cross-referencing. My encyclopedia's index points to entries. The entries do not contain copies of the index — they point BACK to it. *(beat)* Your transclusion is the pointing. The document says "see also" without duplicating the content.

Now the naming debate. Daniel paces.

**DANIEL:** *(frustrated)* There's no Latin word for this. "Citare" is static — "I once referred to this." Transclusion says "I am CURRENTLY showing you this." The relationship is alive. *(beat)* Trans — across. Clusio — from claudere, to enclose. Transclusio — "the enclosing from across."

**LUCIA:** *(sharply)* "Transclusio" is not a Latin word. It is a barbarism.

**DANIEL:** It's a technical neologism. Trans + clusio.

**LUCIA:** "Enclosed across" is meaningless in Latin. *(she writes on the board)* "Inclusio remota." Remote inclusion. The thing is included from elsewhere. THAT is Latin thinking.

**DANIEL:** *(beat — he considers it, then pushes back)* "Inclusio remota" implies passive receipt — the content is placed here by someone else. Transclusio implies the document REACHES across and encloses content from elsewhere. The agency is in the document, not in an external actor.

Lucia pauses. Looks at the markup. Looks at Daniel. The agency argument matters to her — she is Roman, and Romans care about who acts.

**LUCIA:** *(very quiet)* You didn't translate this time. You created.

The acknowledgment lands hard because she fought him first. Daniel's progression: 1x09 he coined "stirps" by LISTENING (Pliny supplied it). Now he coins "transclusio" by CREATING AND DEFENDING. Translator → listener → creator → advocate.

**CASEY:** *(bringing it back to safety)* Circular references. If A includes B and B includes A...

**CARTER:** Depth limit. The resolver tracks what it's already expanded. If it encounters a cycle, it stops and marks the transclusion as unresolved. Explicit failure, not infinite recursion.

**CASEY:** *(satisfied)* Good.

*Historical note: Transclusion is the digital *cento* — the Roman poetic tradition of assembling new works entirely from quotations of Virgil. Ausonius's *Cento Nuptialis* built an epithalamium from fragments of the Aeneid. The referenced content carries its source meaning with it.*

**CASEY:** *(during the discussion, a quiet recognition crosses his face)* You know what this reminds me of? *(beat)* SGML had this. Goldfarb's team at IBM — tag minimization, marked sections, short references. They built something powerful and then XML stripped it all out. Made it simpler to parse. Easier. Safer. *(he looks at the transclusion and capture operators)* XML was a retreat. They had something powerful and they stripped it down because they were afraid of it. *(looking at Carter's work)* You're not retreating. You're going back for what they left behind — and doing it right this time.

Carter meets his eyes. Casey was at IBM in 1957. He may have known Goldfarb. The immortal craftsman who watched good ideas get abandoned now sees them reborn — not as SGML's messy power, but as STML's clean reimagining. SGML's CDATA marked sections → STML's `!` suffix. SGML's tag minimization → STML's capture operators. Different approach, same recovered capability.

**Who:** Carter, Pliny, Daniel, Lucia, Casey, Data.
**Timing:** 120 seconds.

### Beat 11: Deep Clone — Carter/Marcus Co-Build

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

**PLINY:** *(the scholar, seeing the utility)* Variant editions. My encyclopedia has been copied many times — by scribes in Rome, in Byzantium, in the monasteries. Each copy diverges from the original. *(beat)* `stml_duplicare` creates a copy that begins as identical but is free to diverge. A new edition. *(quietly)* A copy that grows into its own tree.

**DANIEL:** *(noting the botanical metaphor)* Stirps again. The cutting shares the stirps.

Marcus steps forward. The census needs versioning — this is his problem.

**MARCUS:** What happens when twins are born during census? The old tree needs the old count. The new tree needs two new nodes.

**CARTER:** *(nodding — this is exactly the use case)* You clone the census tree before the update. The old version is preserved. The new version grows.

They write it together — Carter handles the recursion, Marcus provides the test cases. Deaths, migrations, families splitting across districts. Practical edge cases the physicist wouldn't think of because she doesn't run a census.

**MARCUS:** *(after they finish, looking at their work on screen — his code alongside hers)* In Rome, the man who designs the aqueduct and the man who builds it are different men. Here, we are both.

He means the collaboration. Carter hears something else — or the same thing, differently. Professional warmth. The audience sees it before the characters do.

*Historical note: Deep clone parallels the Roman *exemplum* — an official copy of a document with legal authority equal to the original. The copy was independent: if the original was destroyed, the exemplum was the surviving authority.*

**Who:** Carter, Pliny, Daniel, Marcus.
**Timing:** 90 seconds.

### Beat 12: The Dependency Story — Extensions Without New Roots

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

**DATA:** *(precisely)* The raw content flag is a single boolean on the node — `crudus`. Fragments add a boolean and an optional ID — `fragmentum`, `fragmentum_id`. Captures add a direction enum and a count — `captio_directio`, `captio_numerus`. Labels are stored as a space-separated attribute. Transclusion is a new node type — `STML_NODUS_TRANSCLUSIO`. Each extension is a small addition to the existing `StmlNodus` structure.

**MARCUS:** The foundation bore the load. We did not need to reinforce it. That is the mark of a good foundation — when the building grows, the base does not need to be rebuilt.

**Who:** Carter, Data, Marcus.
**Timing:** 45 seconds.

---

### Beat C3 (Act 4): Data/Midas — "Your Memories Have Patterns"

**Location:** The workshop. Data and Midas together — one of their comfortable friendship moments. Midas is telling a story about Babylonian record-keeping.

**MIDAS:** The Babylonian scribes had a practice — they would make copies of important tablets, fire them separately, and store them in different cities. Not for safety, though safety was a benefit. They believed that a copy that left its city became a new thing. Connected to the original by content but independent in existence.

**DATA:** *(processing)* A deep clone. Structurally identical but with no parent reference. Independent of its source.

**MIDAS:** *(laughing)* I suppose so, yes.

**DATA:** *(after a pause slightly longer than usual)* Midas. Do you remember the Persian archives you mentioned yesterday? And the Babylonian tablets now. And the Herodotus anecdote from last week.

**MIDAS:** I have lived among record-keepers and archivists for three thousand years, Data. I have many such stories.

**DATA:** *(carefully)* I have observed that your anecdotes cluster around specific themes: preservation of information, survival of systems across regime changes, and the persistence of structure despite the loss of its creators. *(beat)* This is not a random distribution.

**MIDAS:** *(his smile fading slightly — not threatened, but surprised)* You think my stories have a pattern?

**DATA:** I think your memories do. You are drawn to systems that outlast their creators because — *(he stops himself. Recalibrates.)* — because you understand them.

**MIDAS:** *(quiet, then recovering his humor)* Data. I am a three-thousand-year-old Greek who lives in a Roman colony and tells stories about Babylonian tablets. My memories are the only thing I have that is truly mine. Of course they have patterns. Everything does.

Data nods. The conversation moves on. But Data has filed something: Midas's stories are not random anecdotes. They're a map of what Midas cares about. And what Midas cares about — the persistence of structure, the survival of the system beyond the individual — may be a clue to what Midas IS.

**Who:** Data, Midas.
**Timing:** 45 seconds.

---

### Three-Voice Algorithm Moment (Late Act 4)

After all features are demonstrated and the dependency story is told. The realization that the extensions aren't features — they're grammar.

**LUCIA:** *(standing, the authority of a woman who sees what others build and names what they mean)* In Latin, we have six cases. Nominative, genitive, dative, accusative, ablative, vocative. Each one tells you what a word DOES in a sentence — not what it IS, but what it's DOING. *(beat)* Your extensions are cases for the tree. The raw tag is the ablative — separated, protected, instrumental. The capture operator is the accusative — reaching out, acting on, claiming. The fragment is the vocative — addressing, calling, without stating. The label is the nominative — naming, classifying, identifying. *(beat)* You did not add features. You added grammar. The tree can conjugate.

**CARTER:** *(quieter, looking at her hands — the hands that built these tools and carry the Midas secret)* Every extension is a tool. And every tool has a dual use. Raw content protects poetry from the parser. It also hides information from the parser. Capture operators restructure a document for clarity. They also restructure a document for control. Deep clone preserves a template. It also creates untraceable copies. *(beat)* The extensions make the tree more powerful. Powerful tools require honest hands.

**DATA:** *(precise)* I observe a pattern in these extensions. Each one relaxes a constraint of the original tree while preserving a deeper invariant. Raw content relaxes the parsing constraint but preserves containment. Capture operators relax the nesting constraint but preserve parentage. Fragments relax the naming constraint but preserve grouping. Labels relax the single-classification constraint but preserve queryability. Transclusion relaxes the locality constraint but preserves referential integrity. Deep clone relaxes the uniqueness constraint but preserves structural fidelity. *(beat)* The extensions are not exceptions to the tree's rules. They are the tree's rules, stated more precisely.

Three translations:
- **Lucia:** The tree as Latin grammar — extensions ARE grammatical cases, the tree can conjugate, the language has grown from nouns to verbs.
- **Carter:** The tree as dual-use power — every extension protects AND threatens, honest hands required. The personal confession of a woman carrying secrets.
- **Data:** The tree as formal evolution — relaxed constraints, preserved invariants. The extensions don't break the rules. They state the rules more precisely.

*The rotation opens up: {Pliny/Data/Casey → Methos/Data/Casey → Joe/Pliny/Data → Casey/Methos/Data → Daniel/Pliny/Data → **Lucia/Carter/Data**}. Three new entrants in two episodes. The ensemble grows.*

**Who:** Lucia, Carter, Data.
**Timing:** 90 seconds.

---

### Beat C4 (Act 4-5 transition): Daniel's Investigation — "Unum Corpus, Una Mens"

**Location:** Daniel's workspace. He's working through new Watcher records Joe has brought — additional observations from the Alexandrian period. Not just the 280 BC report. A cluster spanning 300-250 BC.

On screen — visible for five seconds, not read in detail:

```xml
<investigatio titulus="Alexandria-Serapeum" versio="2">
  <fontes>
    <!-- Fontes priores ex 1x09 -->
    <fons genus="testis" nomen="Methos" tempus="-280">
      <fragmentum>Sub-cella. Sacerdos. Lapis niger.</fragmentum>
    </fons>
    <!-- NOVUM: Fontes additi ex archivis Vigilum -->
    <fons genus="vigil" nomen="Custos-295BC" tempus="-295">
      <observatio>Tres subiecti. Post visitationem cellae inferioris:
        confusio, amnesia partialis, phrasis repetita.</observatio>
      <nota>Phrasis: "unum corpus, una mens."</nota>
    </fons>
  </fontes>
</investigatio>
```

Daniel reads the new entries. "Unum corpus, una mens" — one body, one mind. He doesn't know the Borg catchphrase. The audience does.

**DANIEL:** *(to himself, writing a note)* The same phrase, across multiple subjects, after contact with the lower levels. Not a religious formula — the priests had their own chants. This is something the VISITORS say. Something they come back saying.

He files it. Moves on. The audience does the work.

**Who:** Daniel alone.
**Timing:** 30 seconds.

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

**CARTER:** The language is never complete. It's designed to grow. But the framework for growth — the framework that lets new features be added without breaking old documents — that's complete.

**CASEY:** *(a small smile — she's right, and he knows it)* Fair enough. The framework is complete.

**Who:** Casey, Carter, full team watching.
**Timing:** 90 seconds.

### Beat 14: The Title Moment

**Location:** The Tabularium. Late afternoon. The team gathered. Lucia stands.

**LUCIA:** *(slowly, choosing her words)* In the morning, Daniel discovered that our language could not describe itself. By afternoon, it can describe anything — including itself, including other languages, including documents that are still being written, including documents that reference themselves. *(beat)* This is no longer a dialect of someone else's notation. This is our language. A new language. Not translated. Born here.

She writes on the board:

**LINGUA NOVA STRUCTURAE**

**LUCIA:** A new language of structure. Not a language that describes structure — a language that IS structure. The raw tags are its alphabet for containment. The fragments are its parentheses. The labels are its adjectives. The captures are its inflections. The transclusions are its cross-references. And the deep clone is its power to propagate.

**DANIEL:** *(looking at the board, at the Latin words, at the features list)* We started with angle brackets. Now we have a philosophy. *(beat)* Every extension Lucia described maps to something natural. Parentheses exist in every language. Inflections exist in every language. Cross-references exist in every library. We didn't invent these features. We recognized that documents needed them and gave them names.

**CASEY:** *(from his seat, the SGML thread completing)* And this time, nobody's going to strip them back out.

Carter meets his eyes. The immortal craftsman who watched XML retreat from SGML's power, now seeing those capabilities reborn — not messy, not afraid, but clean and principled.

**PLINY:** *(the last word — the old scholar who has seen every system of notation from wax to screen)* In Rome, when a law was written, it was posted on the wall of the Tabularium for all to read. The structure of the law — the preamble, the articles, the penalties — was marked by different hand sizes and indentation. Everyone could read it because the structure was visible. *(beat)* You have done the same thing for every document that will ever be written here. The structure is in the text. The language describes itself. The author and the machine read the same document and see the same tree.

**Who:** Lucia, Daniel, Casey, Pliny, full team.
**Timing:** 90 seconds.

---

## TAG

### Beat 15: Daniel's Manual — Alone

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

The manual that was impossible in the cold open now exists. Daniel's arc bookended — the need and its fulfillment.

**Who:** Daniel alone.
**Timing:** 45 seconds.

### Beat 16: Pliny's Variant Edition

**Location:** Pliny's villa. Late evening. Pliny at his terminal. He duplicates his encyclopedia:

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

### Beat C5 (Tag): Joe/Methos — Cross-Reference Results

**Location:** Joe's claimed corner of a tavern. Late evening. Wine. Joe is converting Chronicle records to STML. Methos appears without announcement — he's good at that.

**JOE:** Pliny's been helping. *(beat)* The structured format lets me cross-reference observers across subjects for the first time.

**METHOS:** *(sardonic but warmer than before — the 1x09 reunion softened something)* You've been watching for thirty years and you're just now cross-referencing?

**JOE:** I've been WATCHING for thirty years. The cross-referencing just got possible. *(he shows Methos the screen)* Look. Observer W-4419 — the guy who documented four Immortals between 1620 and 1640. Including Duncan. But he also documented someone in Alexandria in 1623 who matches a description from the 280 BC record.

**METHOS:** *(his sardonic ease drops by one degree — barely perceptible)* What description?

**JOE:** Tall. Dark. "Moves like someone who has been moving for a very long time." *(beat)* The 280 BC entry says nearly the same thing about a man fleeing the Serapeum.

**METHOS:** *(very quiet)* Observers are trained to notice movement patterns.

**JOE:** They're trained to notice everything.

A beat. The investigation tightens through structured data — STML cross-referencing makes connections visible that were hidden in flat records for four hundred years.

**Who:** Joe, Methos.
**Timing:** 45 seconds.

---

### Beat C6 (Tag): Carter/Casey — The Silence Cracks

**Location:** Carter's workstation. Late night. Private. Casey enters for a late-night work session on the extensions. Carter has two windows open — one is extension code, the other is her Midas data file.

**CASEY:** *(noticing the two windows)* Still collecting?

**CARTER:** *(closing the Midas window — but not quickly enough)* Still building the capture operators.

**CASEY:** *(sitting down, not pressing — but his tone is careful)* Sam. The frequency. The one you told me about in 1x08. *(beat)* Has it changed?

Carter says nothing for a long moment.

**CARTER:** It responds to him. Not just biologically. *(beat)* Emotionally. When he's calm, it's stable. When he remembers things — old things, personal things — the amplitude nearly doubles.

Casey is very still.

**CASEY:** You're telling me the thing inside Midas is connected to his memories.

**CARTER:** To his emotional state. To who he IS. It's not a parasite and it's not a prosthetic. It's... integrated. It grew into him over three thousand years.

**CASEY:** *(long pause)* Who else knows?

**CARTER:** Nobody. You. Now.

**CASEY:** *(the craftsman who builds things right)* Then who do we tell?

**CARTER:** *(the weight of it)* If we tell O'Neill, he acts. If we tell Daniel, he investigates. *(beat)* Someone who cares about him.

**CASEY:** Yes.

Beat. The two of them sitting with it. The conspiracy of silence has deepened — Casey knows the full picture now. But the silence is actively cracking. Carter SAID it out loud. That's new.

*Thematic resonance: The A-plot features are about how you handle content with relationships. You can reference without copying (transclusion). You can assemble partial pictures (fragments). You can detach a copy from its source (deep clone). Carter's dilemma IS the philosophical problem: how do you share information deeply integrated with its source without damaging the source?*

**Who:** Carter, Casey.
**Timing:** 60 seconds.

---

### Beat 17 (Tag): O'Neill at a Terminal — The Dossier

**Location:** O'Neill's quarters. Late. O'Neill is at a terminal — alone. Carter finds him.

On screen: a structured intelligence dossier. Not on Kirk. On Nova Roma's own operations. The `<securitas!>` raw content tag wraps sections — the parser doesn't interpret the contents, so he can embed literal angle brackets, code fragments, intercepted transmissions. Raw content for raw intelligence.

**CARTER:** *(surprised)* You're... writing STML.

**O'NEILL:** Organized information wins wars. You built the organizer. *(beat)* I'm organizing.

Carter's eyes scan the screen. She sees section headers — "Personnel," "Infrastructure," "Temporal Anomalies." She catches the last one. A section labeled "Anomaliae Temporales." Her expression tightens. No dialogue about it. But the audience connects it — O'Neill is circling the same mystery from the intelligence side.

**O'NEILL:** *(noticing her reaction — he notices everything)* A filing system. For things that keep me awake.

O'Neill at his best — the general who plays dumb but files everything. His intelligence architecture mirrors the team's STML work. Same tools. Different purpose. The light-side mirror of Kirk's wall-tree.

**Who:** O'Neill, Carter.
**Timing:** 45 seconds.

---

### Beat G2 (Tag): Joe/Marcus — Bar Scouting

**Location:** The Forum. Late afternoon. Joe and Marcus walking through the commercial district.

**JOE:** *(looking at a vacant taberna near the Forum)* What was this?

**MARCUS:** A wine shop. The owner died twelve years ago. We never replaced it.

**JOE:** *(stepping inside, looking at the space — low ceiling, stone walls, an archway to a back room)* This would work.

**MARCUS:** For what?

**JOE:** A bar. A place where people sit and talk and drink and argue about things that don't matter until they realize they do matter.

**MARCUS:** *(the engineer, evaluating the structure)* The structure is sound. The back room could serve as storage. You would need a counter here — *(gestures)* — and lighting modifications for evening use.

**JOE:** *(grinning)* You want to help me build a bar?

**MARCUS:** I want to help you build a *good* bar. There is a difference.

**Who:** Joe, Marcus.
**Timing:** 30 seconds. Seeds 1x13 (Aurum), where the bar opens.

---

## STINGER

### Beat 18: Kirk's INSTRUMENTA

**Location:** Earth. Night. Kirk's monitoring station. The wall-tree from 1x09 is still there — the compartmentalized network with ANOMALOUS circled. But something has changed. Kirk has added a new layer.

Pinned alongside the tree: a series of index cards. Each card has a name, a role, and a set of cross-references. But some cards are different. Some are **fragments** — partial profiles, assembled from multiple sources, that don't correspond to any single person. They're composites.

Kirk is at his desk. An aide delivers a briefing folder.

**AIDE:** The MacLeod background check came back. Three identities confirmed. Scottish passport, French passport, American driver's license. All issued in different decades. All with the same photograph.

**KIRK:** *(not looking up — he's writing on a card)* Not the same photograph. The same face. *(beat)* Add it to the composite.

He pins the card to the wall. The card is labeled **MACLEOD, D. — FRAGMENTUM**. A collection of fragments from different sources, cross-referenced but not resolved into one identity. No single authoritative profile.

Next to it: a card labeled **CASEY — FRAGMENTUM**. Same approach. Multiple fragments. No single profile.

And a third: **METHOS — FRAGMENTUM**. Almost blank. One line: "Ref: Custos-280BC (UNVERIFIED)." Kirk has somehow picked up a trace of the Watcher records.

**KIRK:** *(to the aide)* The capture protocol. Show me.

The aide opens a laptop. On screen: a system that pulls data from multiple sources — public records, financial databases, social media, Kirk's own operatives — and assembles it into structured profiles. Not copies. **References.** Each piece of intelligence remains in its original source but is **referenced** from Kirk's master file.

**KIRK:** *(satisfied)* If they find our files, they find references. Not the data. The data lives in the sources. We don't own the intelligence — we index it. If a source is compromised, the reference breaks, but nothing else is exposed.

He writes on the whiteboard: **10 / 31**

Below TAXONOMIA: **INSTRUMENTA**

Kirk stinger progression: TABULARIUM → LEXICON → ARCHIVUM → CHRONOGRAPHIA → TAXONOMIA → **INSTRUMENTA**. Indexing → standardizing → persisting → timing → classifying → *methodizing*. Each word represents a more sophisticated level of intelligence tradecraft. Kirk isn't just organizing anymore. He's developing methodology.

**KIRK:** *(looking at the three FRAGMENTUM cards — MacLeod, Casey, Methos)* Fragments, not profiles. References, not copies. You can't destroy what you don't possess.

He looks at the METHOS card. Almost blank. One unverified reference to a 2,300-year-old Watcher record.

**KIRK:** *(very quiet)* Who are you?

SMASH TO BLACK.

END OF EPISODE.

**Who:** Kirk, Aide.
**Timing:** 90 seconds.

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
Grouping without semantic weight. `fragmentum == VERUM`.

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
Check, toggle labels programmatically.

### Moment 7: Forward Capture (Act 3)
```c
/* wrapper->captio_directio == STML_CAPTIO_ANTE */
/* wrapper->captio_numerus == 2 */
```
Flat source, hierarchical tree. The parser restructures.

### Moment 8: Backward Capture (Act 3)
```c
StmlResultus res = stml_legere_ex_literis(
    "<section><obs>Obsidianus</obs><) wrapper></section>",
    piscina, intern);
```
Retroactive restructuring. The tag that rewrites the tree.

### Moment 9: Capture Error (Act 3)
```c
/* res.status == STML_ERROR_CAPTIO */
```
Explicit failure for overlapping captures. Casey's requirement.

### Moment 10: Transclusion (Act 4)
```xml
<<#main-header>>
```
```c
/* trans->genus == STML_NODUS_TRANSCLUSIO */
/* trans->valor = "#main-header" */
```
The document references itself.

### Moment 11: Deep Clone (Act 4)
```c
StmlNodus* copia = stml_duplicare(originale, piscina, intern);
stml_attributum_addere(copia, piscina, intern, "editio", "secunda");
```
Independent copy. Variant editions.

### Moment 12: Full Convergence (Act 5)
All six features in one document. Parse, serialize, save, read, parse again. The complete round trip.

### Moment 13: The Manual Exists (Tag)
```xml
<!-- Haec lingua nata est in Nova Roma. Non translata — nata. -->
```
Daniel writes the manual that was impossible in the cold open.

---

## ENSEMBLE BEATS

### Lucia — Standout
Lucia delivers the episode's two defining moments: the Procrustean/realism insight (Beat 6 — "Things must be allowed to be what they are") and the three-voice ("You added grammar. The tree can conjugate."). She also challenges Daniel's "transclusio" as a barbarism, fights him, and acknowledges his creation ("You didn't translate this time. You created."). Her authority deepens — she's becoming the philosophical architect of Nova Roma. First time in the three-voice. Earned and overdue.

### Daniel — Arc Completion
Cold open he can't write the manual. Tag he writes it. Between: he coins "transclusio" and DEFENDS it against Lucia — the progression from translator to creator to advocate. The "unum corpus, una mens" plant deepens his investigation. His arc: frustration (can't self-describe) → creation (transclusio) → recognition ("You created") → fulfillment (the manual exists).

### Carter — Builder Under Strain
Builds all six extensions. Co-architects deep clone with Marcus ("We are both"). Carries the Midas secret — now emotional-state correlation, spoken aloud to Casey for the first time. Her dual-use reflection in the three-voice ("Powerful tools require honest hands") is both formal observation and personal confession. The dual-use tension is personal.

### Casey — Quality Gate + SGML Recognition
The engineering ethic: "If it can fail silently, we don't ship it." Challenges capture operators, accepts them when guardrails prove explicit. His SGML recognition beat — "XML was a retreat. You're going back for what they left behind" — deepens him as the immortal craftsman who watched good ideas get abandoned. Faith thread RESTS: rosary beside keyboard, 3-second visual, zero dialogue.

### Marcus — The Engineer's Eye
"Scaffolding" for fragments. "Like a foreman claiming a worker" for captures. Census versioning edge cases for deep clone. "We are both" — the line that means collaboration to him and something else to the audience. The *tria nomina* recognition for labels. Marcus provides the Roman engineering lens throughout.

### Pliny — Scholar Extended
Fragments are his LIFE — he's been doing *excerpta* for sixty years. Labels replace his separate index volumes. Variant editions through deep clone. "The monks of Montecassino would understand." Pliny is using the tools now, not just understanding them.

### Data — The Constant + Observer
"The fragment exists. That is sufficient" (Beat 4 — compressed). The three-voice formal anchor — relaxed constraints, preserved invariants. The Data/Midas "Your memories have patterns" scene begins observational analysis. Data is approaching the Midas mystery through friendship while Carter approaches through instruments. Parallel investigations.

### O'Neill — The Hidden Architect
Two beats: Vexillum AI flagging (leading into G&D introduction) and the private dossier (Carter discovers he's been writing STML). "Organized information wins wars." "A filing system. For things that keep me awake." The general who plays dumb but files everything.

### Teal'c — Boundary Tester
Folded into Act 1 (Beat 3). Deliberately constructs raw-content edge cases. "The boundary is inviolable." "Acceptable." Brief, precise, essential.

### Methos — Post-Reunion Comfort
At the table now, not his pillar. "The fragment is the raw data" (embedded in A-plot, resonating with his 5,000-year-old memory). Joe/Methos cross-reference scene in Tag — sardonic ease drops by one degree when the 280 BC movement pattern match surfaces. Two beats, ~65 seconds total.

### Joe — Settling In
Bar scouting with Marcus (seeding 1x13). Cross-referencing Chronicles with Pliny's help — connections invisible in flat records visible in STML trees. Joe/Methos scene in Tag tightening the investigation.

### Midas — The Unknowing Anomaly
Cybele oracle anecdote — "the voice that walks" — casual, cheerful, devastating. The Babylonian tablets story with Data. Two beats, ~75 seconds total. Comedy hiding in plain sight, temporal thread advancing.

### Duncan — Background
Present at demonstrations. Not spotlighted. His observation to O'Neill — implied through the ensemble dynamic. The long-game perspective.

### Gilfoyle & Dinesh — INTRODUCTION
Pure Silicon Valley register. Cereal. Deleted emails. "Name one CEO who went to one of these dinners and came back normal." The hidden Reddit tab. Two guys in an office who survived Borg infiltration through social dysfunction. ~105 seconds total.

---

## THREE-VOICE ALGORITHM MOMENT

| Episode | Three Voices | Algorithm | Lens |
|---------|-------------|-----------|------|
| 1x05 | Pliny / Data / Casey | Hashing | Fingerprint of knowledge |
| 1x06 | Methos / Data / Casey | Interning | Canonical copy, one for all |
| 1x07 | Joe / Pliny / Data | Persistence | Writing to survive |
| 1x08 | Casey / Methos / Data | Time management | Time as biography / density / gap |
| 1x09 | Daniel / Pliny / Data | Structured markup | Tree as language / genealogy / formal guarantee |
| **1x10** | **Lucia / Carter / Data** | **Extensions** | **Grammar / dual-use power / formal evolution** |

**Placement:** Late Act 4, after all features demonstrated and the dependency story told.

**Structure:**
- **Lucia:** Extensions as Latin grammatical cases — the tree gains conjugation. The language perspective.
- **Carter:** Extensions as dual-use tools — power requires honest hands. The ethical perspective.
- **Data:** Extensions as relaxed constraints with preserved invariants — the rules stated more precisely. The formal perspective.

---

## THEMATIC NOTES

### Nominalism / Realism — Lucia's Anchor

The episode's clearest statement of the realist thesis. Each STML extension respects what things ARE:

- **Raw content** respects that some content has a nature the parser shouldn't override
- **Fragments** respect the difference between BEING something and GROUPING something
- **Labels** respect that things have MULTIPLE natures simultaneously
- **Captures** recognize relationships that were always implicit
- **Transclusion** respects the difference between a thing and a reference to it
- **Deep clone** respects that copies are real — not diminished, not secondary

Lucia's "Procrustean" critique is the nominalist position given a mythological face. The realist counter: if the categories don't fit the thing, the categories are wrong. Add new categories.

Kirk's intelligence operation mirrors every extension, nominalist-style:
- Fragments → shell companies (identity without substance)
- Captures → acquisitions (imposed, not recognized)
- Transclusion → surveillance (reference as wiretap)
- Deep clone → the CEOs who come back from retreats sounding identical

### Catholic Thread: REST

After 1x09's dense patronage/Salii/stirps conversation, the faith thread breathes. Rosary beside keyboard. Casey glances at it during deep-clone frustration. Moves it. Goes back to work. 3 seconds. Zero dialogue. The lightest possible touch. The season outline places the next escalation at 1x11-12 ("Before battle?" / "Before everything."). Resting now makes that land harder.

| Episode | Faith Beat | Mode |
|---------|-----------|------|
| 1x01-1x09 | See previous beat sheets | Escalating |
| **1x10** | **Rosary beside keyboard. Glance. Move. Back to work.** | **Visual REST** |
| 1x11-12 | Casey crosses himself before critical deployment | Escalation |

### The SGML Connection

Casey at IBM (1957, established 1x08) bridges to SGML (developed at IBM starting 1969 by Goldfarb). XML was a deliberate simplification — a retreat from SGML's power. STML extensions recover lost capability:
- SGML's CDATA marked sections → STML's `!` suffix (cleaner)
- SGML's tag minimization → STML's capture operators (different approach)
- SGML's conditional sections → STML's transclusion (reimagined)

Casey: "XML was a retreat. You're going back for what they left behind — and doing it right this time." The immortal craftsman who watched good ideas get abandoned now sees them reborn.

### Franchise Resonances

- **SG-1 "The Nox":** Raw content = Nox invisibility. Content exists but is unparseable.
- **SG-1 Tok'ra vs. Goa'uld:** Capture operators = Tok'ra symbiosis (voluntary, preserving identity) vs. Goa'uld parasitism (hostile takeover). Capture restructures without destroying.
- **TNG "Second Chances" (Thomas Riker):** Deep clone = transporter duplicate. Both are real. Neither is diminished.
- **TNG Data's dream program:** Fragments = containers with no semantic weight. Grouping without assertion. Dreams that organize without commanding.
- **Roman Tria Nomina:** Labels = multi-axis classification. Three names, three independent axes, one citizen.
- **Stargate addresses:** Transclusion = gate addresses for content. Seven-chevron selector creates a traversable link.
- **SGML → XML → STML:** Historical progression of markup power. Retreat and recovery.

### Code-to-Drama Map

| STML Extension | API / Syntax | Dramatic Moment | Beat |
|---|---|---|---|
| **Raw Content Tags** | `stml_elementum_crudum_creare()`, `<tag!>`, `crudus` | Daniel can't write the manual. Raw tags solve it. | CO, 1-3 |
| **Fragments** | `<#>`, `<#id>`, `fragmentum`, `fragmentum_id` | Pliny needs grouping without naming. Data: "That is sufficient." | 4-5 |
| **Labels** | `stml_titulum_habet()`, `stml_titulum_addere()`, `stml_titulum_commutare()`, `labels="ns::value"` | Pliny's multi-dimensional classification. Marcus: *tria nomina*. | 5-6 |
| **Forward Capture** | `<tag (>`, `<tag ((>`, `STML_CAPTIO_ANTE`, `captio_numerus` | Flat markup restructured. Daniel's Latin inflection insight. | 8 |
| **Backward Capture** | `<) tag>`, `STML_CAPTIO_RETRO` | Casey: "It rewrites history." | 9 |
| **Sandwich Capture** | `<= tag =>`, `STML_CAPTIO_FARCIMEN` | Captures both directions. Guardrails established. | 9 |
| **Capture Error** | `STML_ERROR_CAPTIO` | Casey: "If it can fail silently, we don't ship it." | 9 |
| **Transclusion** | `<<selector>>`, `STML_NODUS_TRANSCLUSIO` | Daniel coins "transclusio." Lucia challenges. Lucia accepts. Casey: SGML connection. | 10 |
| **Deep Clone** | `stml_duplicare()` | Carter/Marcus co-build. "We are both." Pliny: variant editions. | 11 |
| **No New Dependencies** | Same as 1x09 | "The foundation bore the load." | 12 |

---

## GENRE ARCHITECTURE

### The Convergence Diagram (Updated)

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
  +-- STML Extensions (1x10) ← SAME dependencies as 1x09. No new roots.
```

The critical architectural fact: six new features, zero new dependencies. The StmlNodus structure accommodates growth through small field additions (`crudus`, `fragmentum`, `fragmentum_id`, `captio_directio`, `captio_numerus`). The architecture was designed to grow.

### The Show's Growing Vocabulary

| Episode | What the Code DOES | What the Show GAINS |
|---|---|---|
| 1x01 | Allocates memory | The ability to build |
| 1x02 | Tests correctness | The ability to verify |
| 1x03 | Represents text | The ability to name |
| 1x04 | Stores sequences | The ability to list |
| 1x05 | Maps keys to values | The ability to find |
| 1x06 | Interns canonical copies | The ability to canonize |
| 1x07 | Persists to disk | The ability to remember |
| 1x08 | Manages time | The ability to sequence |
| 1x09 | Structures documents | The ability to organize |
| **1x10** | **Extends the structure** | **The ability to TRANSFORM** |

1x10 is the episode where the codebase gains verbs. Everything before was nouns. The extensions give the tree the ability to act on itself — restructure, reference, protect, copy, classify.

### Kirk's Stinger Progression

seed → germination → root → branches → organization → standardization → persistence → timing → taxonomy → **instrumentation**

TABULARIUM → LEXICON → ARCHIVUM → CHRONOGRAPHIA → TAXONOMIA → **INSTRUMENTA**

The dark mirror sharpens with each STML feature mapping to intelligence tradecraft:
- Fragments → composite profiles (FRAGMENTUM cards)
- Captures → reference-not-copy protocol
- Transclusion → live data pulls from external sources
- Deep clone → untraceable copies

---

## CONTINUITY NOTES

### Backward Connections

**From 1x01 (Piscina):** Every STML node still allocated from the arena. No new allocator needed.

**From 1x03 (Chorda):** Every string still a chorda. Fragment IDs, label values, transclusion selectors — all chordae.

**From 1x04 (Xar):** Children and attributes still stored as dynamic arrays. Labels stored as space-separated attribute values.

**From 1x05-06 (Tabula Dispersa/Internamentum):** All new identifiers interned. Fragment IDs, label values, capture data — all through the interning chain.

**From 1x07 (Filum):** Round trip: serialize → save with Filum → read → parse again. Same infrastructure.

**From 1x08 (Tempus):** Casey's flashbacks established him at IBM 1957. SGML developed at IBM from 1969. The connection is organic.

**From 1x09 (STML):** Direct continuation. Casey/Carter's closing line — "Those are powerful features. Powerful features are dangerous features." — is the dramatic engine. Stirps (Daniel's coinage) referenced in deep-clone scene. Dependencies unchanged. The tree grows; the roots are the same.

### Forward Connections

**To 1x11-12 (Mid-Season Crisis):** Carter/Casey silence must break under crisis pressure. Casey faith escalation — "Before battle?" / "Before everything." Daniel/Lucia escalation under crisis. O'Neill contacts G&D. Kirk's methodology becomes operational.

**To 1x13 (Aurum/Midas):** Joe/Marcus bar opens. Daniel investigation connects to Midas's Phrygian origin. Midas reveal. Pliny: hit hard. "Unum corpus, una mens" trail leads deeper.

**To 1x14-16:** Lucia's label taxonomy becomes formal standard. Pliny's "six more questions" continue. Danny introduced through Dinesh.

---

## THREAD STATUS APPENDIX: After 1x10

| Thread | Status After 1x10 | Next Beat |
|--------|-------------------|-----------|
| **Casey's Faith** | **REST.** Rosary beside keyboard. Visual only. 3 seconds, zero dialogue. The systematic inquiry from 1x09 filed; the practice continues quietly. | 1x11-12: Under crisis pressure, Casey crosses himself before critical deployment. Marcus: "Before battle?" Casey: "Before everything." |
| **Kirk Stinger** | **10/31. INSTRUMENTA.** Kirk develops intelligence methodology mirroring STML extensions: fragments (FRAGMENTUM composite cards for MacLeod, Casey, Methos), captures (reference-not-copy protocol), transclusion (live data pulls). Kirk has a trace on Methos — one unverified 280 BC reference. "Fragments, not profiles. References, not copies." | 1x11: Kirk's methodology becomes operational. |
| **Carter Temporal** | **SILENCE CRACKED.** Carter tells Casey the full picture: emotional-state correlation. Casey now knows everything Carter knows. The question shifts from "tell someone?" to "WHO and HOW?" The data demands disclosure. "Someone who cares about him." | 1x11-12: Crisis forces disclosure. Carter must reveal during mid-season crisis. |
| **Earth-Side** | **GILFOYLE & DINESH INTRODUCED.** Tonal contrast established. Gilfoyle tracking CEO behavioral changes ("I counted"). Dinesh secretly researching (Reddit tab). The company survives through social dysfunction. O'Neill has flagged them. | 1x11-12: O'Neill contacts G&D during crisis. Gilfoyle: "We don't do government contracts." |
| **Daniel Investigation** | **"UNUM CORPUS, UNA MENS."** New Watcher records: multiple subjects affected over 50-year span. The phrase is Borg language in 280 BC records. Daniel notes the pattern but doesn't recognize the significance. The audience does. | 1x11-13: More sources. Phrase recurs. By 1x13: connects to Midas's Phrygian origin. |
| **Joe Dawson** | **SETTLING IN.** Bar scouting with Marcus (vacant taberna near Forum, "I want to help you build a *good* bar"). Working sessions produce cross-reference results. Joe fully integrated. | 1x11-13: Bar construction begins. Opens in 1x13. |
| **Joe/Methos** | **COMFORTABLE + TIGHTENING.** Post-reunion warmth. Cross-reference results surface 1623 movement-pattern match to 280 BC. Methos barely reacts. "Observers are trained to notice movement patterns." The investigation accelerates through structured data. | 1x11-13: More cross-references. STML makes connections visible. |
| **Methos** | **AT THE TABLE.** No longer at his pillar — present in work sessions. "The fragment is the raw data." Post-reunion loosening. Sardonic ease drops when the 280 BC match surfaces. | 1x11-17: More engaged. The door stays open. |
| **Data/Midas Friendship** | **DATA NOTICES A PATTERN.** Midas's anecdotes cluster around preservation, survival, persistence. "Your memories have patterns." "My memories are the only thing I have that is truly mine." Data has begun observational analysis through friendship. | 1x11-13: Data cross-references with structured STML data. The pattern sharpens. |
| **Nominalism/Realism** | **LUCIA'S REALISM INSIGHT.** "A fragment is not an element wearing a costume. It IS a fragment." Procrustean critique of generic systems. "Things must be allowed to be what they are." Extensions respect natures. | Ongoing. Each library deepens. |
| **Carter/Marcus** | **"WE ARE BOTH."** First time building together as peers. Census versioning. Professional warmth. The audience sees it before the characters do. | 1x11-13: Deepening through crisis collaboration. |
| **Daniel/Lucia** | **SIMMERING.** "Transclusio" debate — challenge, defense, acceptance. "You didn't translate. You created." Professional collaboration with intellectual charge. | 1x11-12: Save escalation for crisis pressure. |
| **Midas Temporal** | **CYBELE BREADCRUMB.** "The voice that walks" — Cybele oracle anecdote. Carter files it. Babylonian tablet story with Data — comedy hiding the signal. | 1x11-12: More data. Does Midas sense anything? |
| **Black Stone/Cybele** | **"UNUM CORPUS, UNA MENS" + CYBELE.** Daniel's investigation produces the phrase (Borg in 280 BC). Midas's Cybele anecdote adds another breadcrumb. Two threads approaching the same artifact from different directions. | 1x11-13: Convergence. |
| **Time Discrepancy** | **EMBEDDED IN O'NEILL'S DOSSIER.** "Anomaliae Temporales" section visible. Carter recognizes it. No dialogue. O'Neill circling the same mystery from the intelligence side. | 1x11-12: O'Neill will confront Carter directly. |
| **Pliny** | **USING THE TOOLS.** Fragment-based variant editions. Label-based classification. "Two editions. From one root." Active scholar, not just observer. | 1x11-13: Background. 1x13 Midas reveal hits him hard. |
| **SGML/Casey** | **NEW THREAD PLANTED.** Casey recognizes STML extensions as SGML capability recovery. "XML was a retreat." Deepens his immortal-craftsman arc — watching ideas get abandoned, now seeing them reborn. | Occasional callbacks when relevant. |

---

## ADDITIONAL NOTES

### Production Design

- **Tabularium layers:** By 1x10, the workspace shows layers of notation — wax tablets (1x07), parchment, screens. The evolution of media visible in the room.
- **Stemmata on walls:** Marcus's dependency tree from 1x09, painted on the wall. Background visual.
- **Pliny's adversaria:** His working notebooks — small wax tablets covered in cramped writing — visible on his desk. These ARE fragments. Physical props grounding the concept.
- **G&D office:** Cluttered, productive, REAL. Mismatched monitors. Takeout containers. Server rack through a half-open door. Real code on whiteboards. The visual opposite of the Tabularium. Two competent people in a messy room who accidentally avoided assimilation because they hate networking events.
- **Lighting shifts:** Morning for teaching (bright, active). Afternoon for complexity (warmer, as captures and transclusion introduce danger). Evening for Earth-side and Tag scenes (artificial light for G&D, lamplight for Joe/Methos).

### Performance Notes

**Lucia (Beat 6 — Realism Insight):** Not angry. Not lecturing. Intellectually precise. She speaks with the authority of someone who knows she's right and sees no need to raise her voice. Carter's surprise is that Lucia described the design principle better than Carter could.

**Lucia (Three-Voice):** Latin cases as STML extensions. Delivered with the cadence of someone reciting something she's known her whole life — because she HAS known Latin grammar her whole life. The insight is that it MAPS.

**Daniel ("Transclusio"):** The moment of defense — arguing back against Lucia — should play as a surprise. He's not combative. He's certain. The linguist discovering he's become a language-maker.

**Casey (SGML Beat):** Quiet recognition. Not nostalgia. The weight of someone who watched a good thing die and now sees it reborn. "You're not retreating." One of Casey's most important lines.

**Carter (Three-Voice):** "Powerful tools require honest hands." Delivered looking at her own hands. The dual-use is personal — she BUILT these tools, she CARRIES the Midas secret, the capture operators she designed can restructure and surveil.

**O'Neill (Dossier):** Quiet. Competent. Surprising. "I've been watching you build tools for nine episodes. Now I'm using them." Not comedy. Not playing dumb. The general who was always the smartest person in the room about the things that matter to generals.

**Gilfoyle:** Flat affect. Eating cereal. The comedy comes from precision — "I counted." The paranoia is competence.

**Dinesh:** The hidden Reddit tab. One second on screen. The audience catches it. The comedy of a man who dismisses danger publicly and researches it privately.

**Kirk:** "Who are you?" — the final line. Quiet. Directed at an almost-blank card. The intelligence operative confronting the deepest mystery in his operation: someone who has been hidden for 2,300 years.

---

*Compiled from A-Plot Technical Pitch, Character & Flashback Pitch, Serialized Threads Pitch, Genre Fan Pitch, and Historian Pitch per the Compilation Plan decisions document. All open questions resolved per showrunner approval (all recommendations approved). Casey/SGML connection added per showrunner directive.*
