# NOVUM FUNDAMENTUM — 1x10 "Lingua Nova Structurae"
## GENRE FAN PITCH — STML Extensions (Raw Content, Capture Operators, Fragments, Labels, Transclusion, Deep Clone)

*Six coffees. One conspiracy board. Red string EVERYWHERE. Because this episode doesn't just add features to a markup language — it takes the DOM tree from 1x09 and gives it VERBS. Last episode was a noun. "The tree exists." This episode is a sentence. "The tree ACTS." Every franchise we love has a version of this transition — from passive structure to active structure. From having a constitution to AMENDING the constitution. From building a ship to giving it a tractor beam. I've been up since three AM drawing parallels on graph paper and I want to SHARE.*

---

## 1. THE CONCEPTUAL LEAP — From Passive Structure to Active Grammar

### What 1x09 Did

1x09 gave Nova Roma the DOM tree. A thing that IS. Parse text into a tree. Navigate the tree. Serialize it back. The tree is an OBJECT. A noun. Beautiful. Complete. Static.

But the real world isn't static. Documents change. Grow. Reference each other. Contain things that shouldn't be parsed. Include things from elsewhere. Get restructured without being rewritten. The tree needs to DO things.

### What 1x10 Does

Every feature in 1x10 is a VERB for the tree:

| Feature | What It Does | What It IS |
|---|---|---|
| **Raw Content Tags** | Protect content from the parser | **SANCTUARY** |
| **Capture Operators** | Restructure the tree by claiming siblings | **JURISDICTION** |
| **Fragments** | Group without semantic weight | **SCAFFOLDING** |
| **Labels/Namespaces** | Classify nodes along multiple axes | **IDENTITY** |
| **Transclusion** | Reference content across the document | **WORMHOLES** |
| **Deep Clone** | Perfect copy of a subtree | **REPRODUCTION** |

1x09's tree could describe the world. 1x10's tree can CHANGE the world. That's the dramatic engine. And every franchise has been here before.

---

## 2. RAW CONTENT TAGS — THE NOX INVISIBILITY CLOAK

### The API

```c
StmlNodus* stml_elementum_crudum_creare(Piscina*, InternamentumChorda*, constans character*);
/* <code!>This <is> not parsed. The < and > are literal.</code> */
```

The `!` suffix. Raw content. The parser sees the opening tag, reads the exclamation mark, and STOPS PARSING until it hits the matching close tag. Everything inside is literal text. Angle brackets are just characters. No tags. No nesting. No interpretation.

### The Franchise Resonance: The Nox ("The Nox," SG-1 S1E7)

The Nox can make things invisible. Not cloaked — genuinely INVISIBLE. The thing still exists, it still occupies space, but the instruments of detection cannot see it. The Nox don't destroy the thing. They don't move it. They render the thing UNPARSEABLE to the observer.

Raw content tags are Nox invisibility for the parser. The content is there. It occupies space in the document. But the parser's instruments — its angle-bracket detection, its tag-matching, its nesting logic — CANNOT SEE IT. The content exists in a protected zone where the parser's rules don't apply.

```xml
<code!>
  if (x < y && y > z) { printf("<hello>"); }
</code>
```

The `<` and `>` and `<hello>` inside the raw tag are NOT markup. They're literal characters. The parser walks past them the way you walk past an invisible Nox city — it's right there, but you can't interact with it.

**The Deeper Cut — Diplomatic Immunity (DS9 "The Maquis"):** On Deep Space Nine, diplomats have immunity from station security. Bajoran law can't touch them. They exist IN the station but are not SUBJECT to the station's rules. The raw content tag is diplomatic immunity for text — the text exists in the document but is not subject to the parser's rules. It carries a flag (`crudus = VERUM`) that says "I am here, but you cannot parse me."

**The Dark Mirror — Kirk's Redactions:** Kirk's intelligence files contain redacted sections. Blacked out. Unparseable. But Kirk's redactions HIDE information. Raw content tags PROTECT information. Same mechanism — a zone the parser can't enter — opposite intent. Pliny uses raw tags to protect Latin poetry that contains angle brackets. Kirk uses redaction to hide truth. The tool is morally neutral. The user defines the purpose.

**The Pliny Resonance:** Pliny's Naturalis Historia quotes sources verbatim — including contradictions, errors, and claims about unicorns. He PRESERVES the source text without interpreting it. The raw content tag is Pliny's methodology: include the original, don't parse it, let the reader decide.

---

## 3. CAPTURE OPERATORS — TOK'RA SYMBIOSIS, NOT GOULD PARASITISM

### The API

```c
STML_CAPTIO_ANTE    = I,    /* forward <tag (> — captures following siblings */
STML_CAPTIO_RETRO   = II,   /* backward <) tag> — captures preceding siblings */
STML_CAPTIO_FARCIMEN = III   /* sandwich <= tag => — captures surrounding siblings */
```

Three capture directions. Forward reaches out and claims everything after it. Backward reaches out and claims everything before it. Sandwich reaches in both directions and wraps.

### The Franchise Resonance: Tok'ra vs. Goa'uld (SG-1)

This is the CRITICAL distinction. And the show hands it to us.

The Goa'uld take hosts AGAINST THEIR WILL. The parasite enters the body, wraps around the spinal column, suppresses the host consciousness, and assumes total control. The original person is still there — trapped, silenced, overwritten. This is Borg assimilation applied to individual bodies. The host's structure is DESTROYED to serve the parasite's purpose.

The Tok'ra share hosts VOLUNTARILY. The symbiote enters with consent. Both consciousnesses remain active. They cooperate. The host retains identity, autonomy, the ability to speak and act independently. The relationship is symbiotic — the host gains longevity and healing, the symbiote gains a body and connection. Structure is PRESERVED while a new layer of organization is added.

**Capture operators are Tok'ra symbiosis.**

Forward capture (`<wrapper (>`) doesn't destroy the siblings it captures. They're still there. Still themselves. Still carrying their attributes, their children, their content. The capture operator WRAPS them — adds a new parent — without destroying their existing structure. The siblings gain a new layer of organization but retain their individual identity.

```xml
<!-- Before capture -->
<chapter>
  <section>Introduction</section>
  <wrapper (>
  <section>Body 1</section>
  <section>Body 2</section>
  <section>Body 3</section>
</chapter>

<!-- After capture resolution -->
<chapter>
  <section>Introduction</section>
  <wrapper>
    <section>Body 1</section>
    <section>Body 2</section>
    <section>Body 3</section>
  </wrapper>
</chapter>
```

The three sections still exist. They're still sections. They still contain their text. They've gained a parent (`wrapper`) but haven't lost their nature. Tok'ra, not Goa'uld.

**Backward capture is Tok'ra retroactive:**

```xml
<chapter>
  <section>Preamble 1</section>
  <section>Preamble 2</section>
  <) preamble>
  <section>Main content</section>
</chapter>
```

The backward capture reaches behind it and wraps the preceding siblings. Retroactive adoption. The sections didn't know they were going to be captured. But the capture doesn't harm them — it RECOGNIZES a relationship that was already implicit. Those sections WERE a preamble. The capture operator makes that relationship formal.

**Sandwich capture is the most SG-1 thing in the entire STML spec:**

```xml
<chapter>
  <section>Before</section>
  <= wrapper =>
  <section>Inside 1</section>
  <section>Inside 2</section>
  <= wrapper =>
  <section>After</section>
</chapter>
```

The sandwich operator marks territory — "from HERE to HERE." Like the Stargate establishing a wormhole with a kawoosh: the entrance and exit are marked, everything between them is connected, and what's inside is TRANSPORTED into a new context (wrapped in a new parent). The content isn't destroyed by the transit. It arrives intact.

**The Borg Counter-Example:** The Borg don't capture. They ASSIMILATE. The distinction is everything. When the Borg assimilate a ship, the ship is gone. Its structure is dissolved into the Collective. There's no `stml_removere` that can undo it. Capture operators are reversible — `stml_removere(wrapper)` would release the captured siblings back to their original parent. Try de-assimilating a Borg drone. Picard needed the entire Enterprise crew and a divine intervention (Q's timeline reset) to come back from assimilation. Capture operators come with an undo button because the captured nodes were never destroyed.

**The Marcus Resonance:** Roman law had a concept called *adrogatio* — the formal adoption of one family into another. The adopted family didn't cease to exist. They gained a new *paterfamilias* but retained their name, their property, their identity. The adoption was a legal CAPTURE — a restructuring of family authority without destroying the captured family's nature. Marcus would recognize capture operators immediately.

---

## 4. FRAGMENTS — DATA'S DREAM PROGRAM

### The API

```c
b32 fragmentum;       /* Is this a fragment? <#> or <#id> */
chorda* fragmentum_id; /* Fragment ID (NIHIL for anonymous) */
/* Tokens: STML_TOKEN_FRAGMENTUM_APERIRE, _CLAUDERE, _AUTO */
```

Fragments are containers without semantic weight. `<#>content</#>` groups children together but the `<#>` node itself doesn't mean anything. It's not an element. It has no tag name. It's invisible scaffolding — a way to group things for organizational purposes without asserting that the group is a THING.

### The Franchise Resonance: Data's Dream Program (TNG "Birthright Part I," "Phantasms")

When Data activates his dream program, he experiences imagery that has NO SEMANTIC WEIGHT in his waking consciousness. The images are there. They affect him. They group together into dream sequences. But they don't MEAN anything in the way that his positronic pathways mean things. They're... fragments. Groupings without assertion. Organization without ontology.

Data's dream program is a fragment container for his consciousness. The dreams aren't commands. They're not subroutines. They're not data entries in his positronic net. They're a grouping mechanism that allows him to process adjacent concepts without asserting relationships between them.

```xml
<chapter>
  <#>
    <section>These two sections</section>
    <section>are grouped together</section>
  </#>
  <section>but the group has no name or meaning</section>
</chapter>
```

The fragment groups the two sections. But the fragment itself contributes nothing to the document's semantics. If you serialize the document and strip fragments, the content is unchanged. The sections still exist. They just lose their invisible grouping.

**The Daniel Ascending Parallel (SG-1 "Meridian" / "Fallen"):** When Daniel ascends, he exists without physical body. He's PRESENT but not CORPOREAL. He can observe, influence, even communicate — but he has no weight in the material world. He's a fragment: identity without semantic mass. When he de-ascends, the fragment dissolves and Daniel becomes a "real" element again — a physical being with tag name, attributes, children (relationships).

Fragments are the ascended state of DOM nodes. They exist. They organize. They contribute nothing to the tree's formal meaning. They're ghosts that hold things together.

**The Holodeck Echo (TNG):** The holodeck creates environments that have no existence outside the holodeck. The rooms, the characters, the objects — they're groupings of photons and force fields. They ORGANIZE the user's experience but they aren't REAL in the way the ship is real. A holodeck character has identity (`fragmentum_id`) — you can ask for "Moriarty" — but the character has no weight outside the simulation. Fragments are holodeck for the DOM tree.

**The Casey Craft Parallel:** A carpenter uses a jig — a temporary frame that holds pieces in alignment while the glue dries. The jig is essential to the work. Without it, the pieces won't align. But the jig is removed from the finished piece. It was never PART of the thing being built. It was scaffolding. Fragments are digital jigs. Casey would understand this immediately and nod once.

---

## 5. LABELS AND NAMESPACES — THE ROMAN TRIA NOMINA

### The API

```c
b32 stml_titulum_habet(StmlNodus*, constans character*);  /* has label? */
b32 stml_titulum_addere(StmlNodus*, Piscina*, InternamentumChorda*, constans character*);
b32 stml_titulum_removere(StmlNodus*, Piscina*, InternamentumChorda*, constans character*);
b32 stml_titulum_commutare(StmlNodus*, Piscina*, InternamentumChorda*, constans character*);
Xar* stml_titulos_capere(StmlNodus*, Piscina*);
/* Stored as: labels="foo bar baz" — space-separated, multi-axis classification */
```

Labels are multi-axis classification. A single node can have multiple labels — multiple classifications that are orthogonal to each other. A person can be both "citizen" and "soldier" and "father." The labels don't conflict because they describe different axes of identity.

### The Franchise Resonance: The Roman Tria Nomina

Every Roman citizen has three names: **praenomen** (personal name), **nomen** (family/clan name), **cognomen** (branch/achievement name). Gaius Julius Caesar. Three classification axes for one person:

- Gaius = personal identity (like a first name)
- Julius = clan membership (like a family name)
- Caesar = branch/distinction (like a nickname that became a title)

Each name places the person in a different system. "Gaius" tells you he's an individual. "Julius" tells you he's part of the gens Julia. "Caesar" tells you he's from the branch that would rule the world.

Labels work the same way. A node can be:

```xml
<persona labels="civis senator pater veteranus">Marcus Aurelius</persona>
```

Four labels. Four orthogonal classification axes. Citizen (legal status). Senator (political role). Father (family role). Veteran (military history). All true simultaneously. All queryable independently:

```c
stml_titulum_habet(persona, "senator");  /* VERUM */
stml_titulum_habet(persona, "veteranus");  /* VERUM */
```

**The Starfleet Resonance:** In Star Trek, every officer has multiple classifications: rank (Lieutenant), department (Engineering), species (Human), posting (USS Enterprise), clearance level (Alpha-2), specialization (Warp Theory). Seven orthogonal axes. Each one answers a different query. "Give me all Engineering officers" is one label search. "Give me all Lieutenants" is another. Labels are Starfleet's personnel database made formal.

**The Deep Cut — Klingon Honor Labels:** In Klingon society, a warrior carries accumulated labels of honor (or dishonor). The House they belong to. The battles they've fought. The blood debts they owe. These are additive — `stml_titulum_addere`. A label of dishonor can be revoked by the High Council — `stml_titulum_removere`. Worf's discommendation was a `stml_titulum_removere("honoratus")`. His restoration was a `stml_titulum_addere("honoratus")`.

**Toggle as Dramatic Device:** `stml_titulum_commutare` — toggle a label. The label is either there or not. Binary. This is the SG-1 "friend or foe" question: when Daniel was ascended, was he human? `commutare("humanus")` — toggle. Human, not-human, human again. The label toggles. The person endures.

**The Label/Namespace Duality as Nominalism/Realism:** Labels CLASSIFY. Namespaces SITUATE. A label says "what kind of thing are you?" A namespace says "in whose context do you exist?" The realist claims both matter — a thing has a nature (labels) AND a context (namespace). The nominalist claims the context determines the nature — the namespace IS the label. STML gives you both, independently. The code is moderate realist by design.

---

## 6. TRANSCLUSION — STARGATE ADDRESSES

### The API

```c
STML_NODUS_TRANSCLUSIO = VII   /* <<selector>> */
STML_TOKEN_TRANSCLUSIO = XVI   /* <<selector>> token */
```

Transclusion. A node that says "insert the content found at THIS LOCATION" without copying it. The content stays where it is. The transclusion is a REFERENCE — a pointer, a wormhole, a gate address. Multiple documents can transclude the same source. The source is canonical. The transclusions are views.

### The Franchise Resonance: The Stargate Itself

The Stargate doesn't MOVE things. It creates a CONNECTION between two locations. Step through the gate on Earth, step out on Abydos. You didn't copy yourself. You didn't destroy Earth. You created a temporary, traversable link between two points.

Transclusion is a Stargate for documents.

```xml
<encyclopedia>
  <liber titulus="De Mineralibus">
    <observatio id="obsidianus">
      <nomen>Obsidianus</nomen>
      <locus>Stabiae</locus>
    </observatio>
  </liber>
  <liber titulus="De Usu Medicinae">
    <<#obsidianus>>  <!-- transclusion: show the obsidian entry HERE -->
  </liber>
</encyclopedia>
```

The obsidian entry lives in Book of Minerals. Book of Medicine REFERENCES it. The entry isn't copied. It isn't moved. A wormhole connects the two locations. Edit the original, and every transclusion sees the change — because they're all looking at the same canonical source through their own private Stargates.

**The Seven-Chevron Model:** A Stargate address is a selector — a sequence of symbols that uniquely identifies a destination. A transclusion selector (`<<#obsidianus>>`) is a seven-chevron address for content. It uniquely identifies a node in the document tree and creates a traversable link to it.

**The Anti-Borg Principle (Deepened from 1x09):** The Borg COPY. They take the original and absorb it into the Collective. After assimilation, there is no original — only the copy inside the Collective. Transclusion is the opposite: the original remains sovereign. The transclusion is a VIEW, not a copy. Delete the transclusion, and the original is untouched. Delete the original, and the transclusions break — they don't pretend the content still exists. Honest references.

This is the Internamentum principle (1x06) scaled up. Internamentum gave us canonical strings — one copy, many references. Transclusion gives us canonical STRUCTURES — one subtree, many views. The episode explicitly extends 1x06's thesis to a larger scale.

**The Ancients' Knowledge Base:** The Ancients didn't store knowledge in individual repositories. They stored it in a NETWORK of repositories connected by gates. Each repository could reference knowledge in any other. The knowledge wasn't duplicated — it was TRANSCLUDED. When O'Neill downloaded "all the knowledge," he got the full repository but not the network. He had the nodes but not the links. Transclusion IS the links.

**The Library of Alexandria Echo (from Methos):** The Library's tragedy wasn't just lost scrolls. It was lost REFERENCES. Scholars across the ancient world cited the Library's holdings. When the Library burned, the citations broke. Every `<<Library:scroll_4729>>` became a dead link. Transclusion with functioning infrastructure is what the Library should have had — and what Nova Roma is building. Methos might not say this aloud. But Carter would see it in his face.

---

## 7. DEEP CLONE — RIKER'S TRANSPORTER DUPLICATE

### The API

```c
StmlNodus* stml_duplicare(StmlNodus*, Piscina*, InternamentumChorda*);          /* deep clone */
StmlNodus* stml_duplicare_superficialiter(StmlNodus*, Piscina*, InternamentumChorda*); /* shallow */
```

Deep clone: perfect copy of a node and ALL its descendants. Every child, every attribute, every text node — recursively duplicated. The clone is independent. Changes to the clone don't affect the original. Changes to the original don't affect the clone.

Shallow clone: copy the node itself but NOT its children. The shell without the interior. The title page without the book.

### The Franchise Resonance: Thomas Riker (TNG "Second Chances")

A transporter accident created two Rikers. Identical. Both real. Both carrying the same memories, the same personality, the same commission. A deep clone. Not a copy in any diminished sense — a PERFECT DUPLICATE.

`stml_duplicare` is the transporter accident for DOM nodes. The clone is not lesser. It's not a reference (that's transclusion). It's not a view. It's an independent, fully realized copy that exists separately from the original. Edit the clone, the original doesn't change. Edit the original, the clone doesn't change. Two Rikers, each living their own life.

**Data and Lore:** Lore is a deep clone of Data — same positronic architecture, same neural pathways, same base programming. But Lore diverged. The clone evolved independently. Changed. Became something the original wasn't. Deep clone creates that possibility — the copy starts identical but is free to diverge.

**The Shallow Clone — The Briefing Report:** `stml_duplicare_superficialiter` copies the node without its children. In Trek terms: the mission briefing vs. the mission log. The briefing has the structure — "USS Enterprise, NCC-1701-D, Mission to Sector 001" — but none of the details. The children (the actual log entries, the sensor readings, the crew reports) are missing. It's a table of contents without the book.

Shallow clone is what you send up the chain of command. The Captain gets the full report (deep clone). The Admiral gets the summary (shallow clone). The Commander-in-Chief gets the heading (the node itself, no clone at all).

**The Replicator Echo (SG-1/SGA):** Replicators replicate — deep clone with a twist. Each replicator block is a copy of the pattern, but the pattern is self-modifying. The copies EVOLVE. This is the danger of deep clone without constraints: the copies can become something you didn't intend. On Nova Roma, deep clone is controlled — you choose WHEN to clone and WHAT to clone. The Replicators had no such restraint. They cloned everything, always, forever. The tree grew without a gardener.

**Template Instantiation:** Deep clone is how you use templates. Create a template node — a perfect `<observatio>` structure with all the right sub-elements. Then `stml_duplicare` it every time you need a new observation. Fill in the details. The template is the Form (in the philosophical sense). The clone is the instantiation. Plato would recognize this: the Ideal Observation exists as a template. Every actual observation is a copy — perfect in structure, particular in content.

---

## 8. THREE-VOICE CANDIDATE

### The Natural Rotation

| Episode | Three Voices | Algorithm | Lens |
|---------|-------------|-----------|------|
| 1x05 | Pliny / Data / Casey | Hashing | Fingerprint of knowledge |
| 1x06 | Methos / Data / Casey | Interning | Canonical copy, one for all |
| 1x07 | Joe / Pliny / Data | Persistence | Writing to survive |
| 1x08 | Casey / Methos / Data | Time as biography / density / gap |
| 1x09 | Daniel / Pliny / Data | Structured markup | Tree as language / genealogy / guarantee |
| **1x10** | **Lucia / Carter / Data** | **Extensions** | **Grammar / Power / Classification** |

### Why Lucia / Carter / Data

**Lucia** has been the linguistic conscience of Nova Roma since 1x03. She pushed Daniel from translation to creation. She named things. She criticized things that were "Latin words but not Latin thinking." STML extensions give her language GRAMMAR — the rules that let the language DO things, not just describe things. Fragments are pronouns (pointing without naming). Labels are adjectives (describing without limiting). Capture operators are verbs (acting on the tree). Transclusion is quotation (including without copying). Lucia would see this immediately: "You have given the language its grammar."

**Carter** is the builder. She wrote the parser. She drew the dependency tree. The extensions are HER work — she proposed them in the 1x09 closing. But Carter's relationship to the extensions is about POWER. Raw content tags protect. Capture operators restructure. Deep clone reproduces. These are powerful operations, and Carter is increasingly aware that power requires responsibility. The Midas temporal data she's carrying, the secrets she's keeping — the extensions give her tools that make her MORE capable and MORE burdened.

**Data** is the formal analyst. He sees each extension as a formal property of the tree. Raw content = zones of unparsed text (opaque subtrees). Capture = tree restructuring operations (reparenting). Fragments = semantically null nodes (grouping without assertion). Labels = multi-attribute classification (set theory). Transclusion = referential integrity (pointer semantics). Deep clone = structural reproduction (recursive copy). Data would state the formal property, then note: "Each extension adds an operation that the previous tree lacked. The extensions do not change the tree's nature. They expand its vocabulary."

### The Three Voices

**LUCIA:** "In Latin, we have six cases. Nominative, genitive, dative, accusative, ablative, vocative. Each one tells you what a word DOES in a sentence — not what it IS, but what it's DOING. Your extensions are cases for the tree. The raw tag is the ablative — separated, protected, instrumental. The capture operator is the accusative — reaching out, acting on, claiming. The fragment is the vocative — addressing, calling, without stating. The label is the nominative — naming, classifying, identifying. You did not add features. You added grammar. The tree can conjugate."

**CARTER:** "Every extension is a tool. And every tool has a dual use. Raw content protects poetry from the parser. It also hides information from the parser. Capture operators restructure a document for clarity. They also restructure a document for control. Deep clone preserves a template. It also creates an army. *(beat, looking at her hands)* The extensions make the tree more powerful. Powerful tools require honest hands."

**DATA:** "I observe a pattern in these extensions. Each one relaxes a constraint of the original tree while preserving a deeper invariant. Raw content relaxes the parsing constraint but preserves containment. Capture operators relax the nesting constraint but preserve parentage. Fragments relax the naming constraint but preserve grouping. Labels relax the single-classification constraint but preserve queryability. Transclusion relaxes the locality constraint but preserves referential integrity. Deep clone relaxes the uniqueness constraint but preserves structural fidelity. The extensions are not exceptions to the tree's rules. They are the tree's rules, stated more precisely."

---

## 9. GILFOYLE/DINESH REGISTER — THE TONAL BOUNDARY

### The Silicon Valley Coexistence

1x10 is the INTRODUCTION of Gilfoyle and Dinesh per the season outline. Earth-side C-plot. This means the episode has to manage two registers simultaneously: the Nova Roma dramatic register (Latin, philosophical, slow-burn craft) and the Silicon Valley comedy register (sarcastic, technical, fast banter).

**The Tonal Boundary Rule:** The registers should NEVER bleed into each other within a scene. A Gilfoyle/Dinesh scene is pure Silicon Valley. A Carter/Lucia scene is pure Nova Roma. The comedy doesn't undercut the drama. The drama doesn't make the comedy feel cheap. They're parallel frequencies — same themes, different modes.

**How the features map across registers:**

| STML Feature | Nova Roma Register | Silicon Valley Register |
|---|---|---|
| Raw Content | Pliny protecting poetry from the parser | Gilfoyle deleting unread investor emails ("I am not parsing that") |
| Capture Operators | Lucia's grammatical insight about accusative case | A tech giant acquiring competitors ("forward capture") |
| Fragments | Data's formal analysis of semantically null nodes | Big Head's project: exists, does nothing, somehow works |
| Labels | Marcus recognizing tria nomina | The retreat attendees who all use the same vocabulary |
| Transclusion | Carter building canonical references | Dinesh quoting the same subreddit to Danny |
| Deep Clone | Pliny's template for encyclopedia entries | CEOs who came back from retreats sounding identical ("deep cloned") |

**The KEY G&D moment for 1x10:** Per the season outline, O'Neill flags G&D's company as the one AI firm that hasn't been acquired. Cut to their office: Gilfoyle deleting an investor email. Dinesh: "Shouldn't we at least read those?" Gilfoyle: "No."

**The Genre Fan's Reading:** Gilfoyle is performing RAW CONTENT. He's marking the investor emails as `<spam!>`. He won't parse them. Won't interpret them. Won't let them restructure his tree. He is the human embodiment of the `!` suffix — content that refuses to be parsed.

And Dinesh is the transclusion node — he's always REFERENCING other people's content. His friendship with Danny, his X-Files subreddit, his habit of forwarding articles. Dinesh is `<<external_source>>`. He includes content from elsewhere. Gilfoyle is `<firewall!>`. He refuses content from anywhere.

Between them, they've been accidentally performing the two most fundamental operations in information security: include what's trusted, reject what's not. And they didn't even know they were doing it. This is why they survived. Not through intelligence (though they have it). Through temperament. Gilfoyle's paranoia and Dinesh's social connections — raw content rejection and trusted transclusion — are the antibodies the tech industry needed.

---

## 10. CODE-TO-GENRE MAP — EVERY EXTENSION, EVERY RESONANCE

| STML Feature | API | Franchise Resonance | Dramatic Moment |
|---|---|---|---|
| **Raw Content** | `stml_elementum_crudum_creare()`, `<tag!>` | Nox invisibility / Diplomatic immunity / Pliny's verbatim sources | Content protected from parser. Poetry with angle brackets. Casey: "Some things shouldn't be interpreted." |
| **Forward Capture** | `STML_CAPTIO_ANTE`, `<tag (>` | Tok'ra symbiosis — voluntary host-taking that preserves identity | Restructuring a document by wrapping following content. Roman *adrogatio* adoption parallel |
| **Backward Capture** | `STML_CAPTIO_RETRO`, `<) tag>` | Retroactive Tok'ra — recognizing a relationship that was always implicit | Wrapping preceding content. "Those sections WERE a preamble" |
| **Sandwich Capture** | `STML_CAPTIO_FARCIMEN`, `<= tag =>` | Stargate kawoosh — marking entry and exit, connecting everything between | Territory-marking. "From HERE to HERE" |
| **Fragments** | `<#>`, `<#id>`, `fragmentum` | Data's dream program / Daniel ascending / Holodeck / Carpenter's jig | Grouping without meaning. Scaffolding. Identity without weight |
| **Labels** | `stml_titulum_*()`, `labels="..."` | Roman tria nomina / Starfleet multi-axis classification / Klingon honor | Multi-axis identity. `commutare` as dramatic toggle. |
| **Transclusion** | `<<selector>>`, `STML_NODUS_TRANSCLUSIO` | Stargate addresses / Library of Alexandria references / Ancient knowledge network | Content referenced, not copied. Anti-Borg. Canonical structure |
| **Deep Clone** | `stml_duplicare()` | Thomas Riker / Data & Lore / Replicators | Perfect independent copy. Template instantiation. Platonic Forms |
| **Shallow Clone** | `stml_duplicare_superficialiter()` | The briefing report — structure without detail | Summary. What the Admiral sees vs. what the Captain sees |

---

## 11. NOMINALISM / REALISM — THE GRAMMAR ARGUMENT

### How Extensions Advance the Thesis

**The Realist Position (STML Extensions):**

Each extension RESPECTS the distinction between what a thing IS and what you can DO with it:

- **Raw content** respects that some content HAS A NATURE that the parser shouldn't override. The angle brackets in C code ARE characters, not markup. The raw tag recognizes this nature.
- **Capture operators** respect that restructuring is RE-COGNITION — the sections were always a preamble, the capture operator just makes it formal. The structure was implicit. The operator reveals it. Realist.
- **Fragments** respect the difference between BEING something and GROUPING something. A fragment doesn't pretend to be an element. It's honest about its lack of semantic weight. It's scaffolding that knows it's scaffolding.
- **Labels** respect that things have MULTIPLE NATURES simultaneously. A citizen AND a senator AND a father. The labels are independent because the natures are independent. The realist says: a thing can be many things without contradiction, because its natures are real and don't conflict.
- **Transclusion** respects the difference between a THING and a REFERENCE to a thing. The map is not the territory. The transclusion is not the content. The realist distinguishes between the two; the nominalist says "what's the difference?"
- **Deep clone** respects that copies are REAL — not diminished, not secondary. Thomas Riker is a real person. A deep-cloned node is a real node. The copy has the same nature as the original because nature is inherent, not bestowed by being "first."

**The Nominalist Counter (Kirk's Operations):**

Kirk's intelligence operation mirrors every extension, nominalist-style:

- Raw content → REDACTION. Kirk hides information. The raw tag protects information.
- Capture → ACQUISITION. Kirk's network captures companies forward (buying), backward (retroactive IP claims), and sandwich (surrounding holdouts). Same operator. Imposed, not recognized.
- Fragments → SHELL COMPANIES. Identity without substance. A legal entity that exists to group things without being a thing itself. Kirk's fragment nodes are corporate structures designed to obscure, not organize.
- Labels → COVER IDENTITIES. Kirk labels assets with false classifications. The labels don't reflect nature — they contradict it. A Borg seed labeled "investor."
- Transclusion → SURVEILLANCE. Kirk doesn't copy information. He OBSERVES it in place. Transclusion as wiretap. Same mechanism — reference without duplication — opposite intent.
- Deep clone → BODY SNATCHING. The CEOs who come back from retreats sounding identical. Deep cloned... but the original was overwritten, not preserved. The Borg version of duplication: the copy replaces the original.

**Lucia's Insight (per season outline):** "Your old system treated all things the same. Your new system lets things be what they are. A fragment is not an element wearing a costume. It IS a fragment." This is the moderate realist thesis applied to programming: things have natures, and your tools should respect those natures rather than forcing everything into one undifferentiated category.

---

## 12. THE FRANCHISE DEEP CUTS — EASTER EGGS AND REWATCH REWARDS

### SG-1

- **"Window of Opportunity" (S4E6):** Teal'c learned Latin during the time loops. He's been reading code comments since 1x01. When the extensions introduce more complex syntax (`<tag (>`, `<= tag =>`), Teal'c reads them aloud. Correctly. The team has stopped being surprised.
- **"2010" (S4E16):** The Aschen gave Earth clean energy and universal health — and secretly sterilized the population. Technology that solves problems but hides its true purpose. Raw content tags could hide malicious content — but the parser MARKS them as raw. The tree is honest about what it can't see. The Aschen weren't.
- **"Tin Man" (S1E18):** SG-1's consciousnesses copied into robot bodies. Deep clone. But the copies KNEW they were copies and had to reckon with that. In STML, a deep-cloned node has no flag saying "I am a copy." It's structurally identical. The philosophical question: does the copy know? Data would think about this.

### Star Trek

- **TNG "The Offspring" (S3E16):** Data creates Lal — not a deep clone (Lal has her own architecture) but close. She chooses her own form. She develops beyond Data. Then she dies. The deep clone creates something that CAN surpass its source. The template doesn't limit the copy.
- **TNG "Parallels" (S7E11):** Worf traverses parallel universes — each one a different tree with the same root but different branches. Transclusion across parallel timelines. Each universe REFERENCES the same events but from different branches. The Stargate network as a multi-universe transclusion system.
- **DS9 "Whispers" (S2E14):** O'Brien discovers he's a replicant — a deep clone who doesn't know he's a clone. The entire episode is from the clone's perspective. The audience doesn't know either. Deep clone as unreliable narrator. If a node were deep-cloned and placed in a different tree, would it know?
- **VOY "Author, Author" (S7E20):** The Doctor writes a holonovel. The publisher deep-clones it across the Federation. The copies are identical. But is the Doctor's creative work the same as a data copy? The holographic rights question. For STML: when you `stml_duplicare` Pliny's encyclopedia, is the copy Pliny's work or Nova Roma's infrastructure?

### Highlander

- **The Quickening as Information Transfer:** When an Immortal takes another's head, they receive the Quickening — the defeated Immortal's knowledge, experience, power. This is NOT deep clone (the original is destroyed) and NOT transclusion (the reference is permanent, not dynamic). It's ASSIMILATION — the Borg operation, performed by the heroes. The show has never examined this parallel. STML extensions make it visible: the Highlander universe's central mechanic is structurally identical to the thing Nova Roma is fighting against. Duncan takes heads. The Borg take civilizations. The scale is different. The operation is the same.
- **"Methos" (S3E16-17) — The Adam Pierson Identity:** Methos's Watcher file is a fragment with a false ID. "Adam Pierson" is a `<#adam_pierson>` — a fragment container with an identifier that doesn't correspond to a real element. The fragment groups Methos's modern-era activities without asserting that "Adam Pierson" is a real person. Because he isn't. The fragment is honest — it groups without asserting. Methos's cover story, encoded as data structure.

### Foundation

- **Seldon's Vault as Transclusion:** The Seldon hologram appears at predetermined intervals, delivering prepared messages. It's a pre-recorded transclusion — the content was created once (Seldon's analysis) and transcluded at multiple points in time. The references are one-directional (Seldon can't see the future reactions). The transcluded content is canonical — Seldon said what Seldon said. The audience's interpretation varies. The source doesn't change.
- **The Second Foundation as Capture:** The Second Foundation captures the First Foundation's trajectory — not by assimilating it, but by wrapping it in a larger context that the First Foundation doesn't see. Forward capture: the Second Foundation reaches ahead and claims the trajectory of events that follow its interventions. The First Foundation is captured — restructured within a larger plan — without knowing it.

### Babylon 5

- **The Great Machine on Epsilon III as Raw Content:** Draal and the Great Machine exist inside Epsilon III — present, powerful, but UNPARSEABLE by normal means. You can't scan it. You can't interface with it through standard channels. It's a raw content zone. The content is there. The standard parser can't read it. You need special access (Draal's invitation) to enter the zone.
- **Minbari Soul Theory as Deep Clone:** The Minbari believe souls are reborn — deep cloned from one body to the next. But the clone diverges: each life is different. The soul template (`stml_duplicare`) creates a new life that starts identical but becomes unique. Delenn's transformation is a deep clone that mutated — Minbari template, human modifications, something new that neither species predicted.

---

## 13. THE EPISODE'S GENRE ARCHITECTURE — META-OBSERVATION

### The Show's Growing Vocabulary

Look at the progression:

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

1x10 is the episode where the codebase gains VERBS. Everything before this was nouns — things that exist, things that store, things that persist. The extensions give the tree the ability to act on itself. To restructure. To reference. To protect. To copy. To classify.

In franchise terms: this is the episode where the starship gets weapons, shields, and transporters. Not because it's a warship — because a starship that can only orbit is incomplete. It needs to DO things. The tree is the ship. The extensions are its systems.

### The Kirk Stinger Progression

seed → germination → root → branches → organization → standardization → persistence → timing → taxonomy → **grammar**

TABULARIUM → LEXICON → ARCHIVUM → CHRONOGRAPHIA → TAXONOMIA → **GRAMMATICA**

Kirk mirrors the STML extensions in his intelligence operation. His 10th vocabulary word should be GRAMMATICA — the rules of his intelligence language. Not what he knows (taxonomy). How he OPERATES on what he knows. Forward capture of assets. Backward capture of history. Sandwich capture of territory. Raw redaction of sensitive data. Fragment shell companies. Label cover identities. Transclusion surveillance.

Kirk writes: **10 / 31**

And below: **GRAMMATICA** — "The rules now have rules."

---

*[Falls asleep face-down on the conspiracy board. Red string tangled in hair. The graph paper has a tree drawn on it with six branches labeled NOX, TOK'RA, HOLODECK, TRIA NOMINA, STARGATE, RIKER. At the root of the tree, circled in red: "GRAMMAR = POWER." Post-It note on the monitor: "Extensions aren't features. They're VERBS." Another Post-It: "Gilfoyle IS raw content. Dinesh IS transclusion. They're the STML security model and they don't know it." A third Post-It, underlined twice: "The Quickening is assimilation. Nobody has noticed. NOBODY HAS NOTICED."]*

*The tree doesn't just exist anymore. It acts. It protects. It reaches. It copies. It references. It classifies. Every franchise feature — shields, transporters, wormholes, adoption, honor systems, diplomatic immunity — maps to an STML extension because the extensions formalize operations that civilizations have always performed on structured information.*

*Lingua Nova Structurae. The new language of structure. Not a new language — the same language, now with grammar.*
