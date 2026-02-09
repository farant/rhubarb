# NOVUM FUNDAMENTUM 1x10: "Lingua Nova Structurae"
## CHARACTER & FLASHBACK PITCH

---

## EPISODE CONTEXT

**Library:** STML Extensions (raw content tags, capture operators, fragments, labels, transclusion, deep clone)
**Previous Episode (1x09):** STML base — XML-like markup with DOM tree. Stirps coined. Pliny saw his mind. Daniel/Lucia "colleague." Joe/Methos reunion happened. Casey queried his first STML. O'Neill asked "Who decides what nests inside what?"
**Season Outline says:** "Lucia's second major moment. Carter/Marcus deepens. Daniel coins a living term."

---

## CHARACTER ARCS

### 1. LUCIA SERENA — The Second Major Moment

**Arc: Linguistic Authority → Philosophical Architect**

Lucia's first major moment was 1x03 — she challenged Daniel's assumptions about string representation and won. Now she does it again, but at a higher level. In 1x03 she argued about data representation. In 1x10 she argues about *ontological categories*.

**The Setup:** Carter presents the extensions. Raw content tags (`<code!>`), fragments (`<#>`), capture operators (`<div (>`). The team treats these as technical features — tools for convenience. Lucia sees something else.

**The Scene (Mid Act 2 — the Realism Insight):**

Carter demonstrates fragments:

```xml
<#>
  <nomen>Gaius Aurelius</nomen>
  <munus>faber</munus>
</#>
```

Carter explains: "A fragment groups nodes without adding semantic weight. It's a container that doesn't mean anything — it just holds things together."

Lucia stops her.

**LUCIA:** No. That is precisely wrong. A fragment is not "a container that doesn't mean anything." A fragment is a container that declares its own transparency. An element says "I am a room." A fragment says "I am the space between rooms." Both are real. Both have natures. The fragment's nature is to be invisible. That is not the ABSENCE of nature. That is a SPECIFIC nature — the nature of structural service without semantic claim.

**CARTER:** *(surprised by the philosophical precision)* That's... exactly the design principle.

**LUCIA:** It is also the first principle of any sound philosophy. Things are what they are. A fragment IS a fragment. Not an element wearing a costume. Not an element with a flag that says "ignore me." A distinct kind of thing with its own properties.

This is the Nominalism/Realism moment the season outline flags. Lucia articulates moderate realism through STML: the extensions don't just add features to XML, they add *ontological categories*. A fragment is not a disguised element — it's a new kind of being.

**The Deeper Beat (Late Act 3 — Lucia and the Labels):**

When labels are introduced (CSS-class-like attribute system for categorization), Lucia takes the lead. She designs the label taxonomy for the census:

```xml
<civis labels="patricius senator veteranus">
  <nomen>Gaius Valerius Maximus</nomen>
</civis>
```

**LUCIA:** Labels are not categories imposed from outside. They are properties recognized from within. Gaius is a patrician, a senator, and a veteran. He does not BECOME these things when you label him. The labels name what is already true. *(beat)* Your previous system — flat attributes — could only say "this element has this value." Labels say "this element participates in these natures."

**DANIEL:** *(quiet recognition — she just described the Theory of Forms in database terms)*

**LUCIA:** When you label a citizen "veteranus," you are not attaching a string to a node. You are recognizing that this citizen shares in the nature of military service. The label is an act of recognition, not an act of creation.

This is Lucia's philosophical contribution to Nova Roma — she's becoming the curriculum architect. She doesn't just translate Roman concepts into STML. She uses STML to articulate philosophical positions that the Romans already hold but haven't formalized.

**Callback to 1x03:** In 1x03, Lucia said "A string should know its own length" — the realist position on data. In 1x10, she scales up: "Things should be recognized for what they are" — the realist position on classification.

**Who:** Lucia scenes pair with Carter (fragment insight), Daniel (labels), and Marcus (census application).

---

### 2. CARTER / MARCUS — The Dynamic Deepens

**Arc: Physicist + Engineer → Co-Architects of Infrastructure**

Carter and Marcus have been orbiting each other since 1x01. Their walks (established 1x07), their shared engineering sensibility, their complementary perspectives. In 1x10, they build something together for the first time — not as teacher/student but as equals.

**The Feature:** Deep clone (`stml_duplicare`). The ability to duplicate an entire subtree — a complete copy, independent of the original, allocated fresh from the arena.

**Carter's Problem:** The census needs versioning. When the census changes (new births, deaths, migrations), they need the old version AND the new version. Currently, modifying the tree destroys the previous state.

**Marcus's Solution (Act 3):**

**MARCUS:** When we build a new aqueduct, we do not destroy the old one. We build alongside it. The old aqueduct serves until the new one is complete. Then we redirect the water. *(beat)* Your tree should do the same. Copy the tree. Modify the copy. Keep the original until the copy is verified.

**CARTER:** *(nodding — she was going to propose the same thing, but his framing is better)* Deep clone. A complete copy of the tree — every node, every attribute, every child. Independently allocated. You can modify the clone without touching the original.

They write it together. Carter handles the recursion. Marcus handles the test cases — he thinks about what happens to the census when twins are born, when a family moves districts, when someone dies. Practical edge cases that Carter wouldn't think of because she doesn't run a census.

```c
/* Duplicare censum — census versio nova */
StmlNodus* census_novus = stml_duplicare(
    census_vetus, piscina, intern);

/* Addere novum civem ad novam versionem */
StmlNodus* novus_civis = stml_elementum_creare(
    piscina, intern, "civis");
stml_liberum_addere(familia_in_novo, novus_civis);

/* Census vetus integer manet */
```

**The Emotional Beat:** This is the first time Carter works WITH someone on core infrastructure rather than presenting it. She's been the teacher. Marcus has been the student with brilliant analogies. Now they're peers. Carter notices. She likes it.

**Marcus's Line (Act 3 close):** "In Rome, the man who designs the aqueduct and the man who builds it are different men. Here, we are both." He means the collaboration. Carter hears something else — or the same thing, differently.

**Production Note:** Their scenes are warm but professional. The deepening is visible to the audience and invisible to the characters. They don't flirt. They build things side by side. That IS the romance, for engineers.

---

### 3. DANIEL JACKSON — "Coins a Living Term"

**Arc: Translator → Linguistic Creator (Turning Point 2 continues)**

Daniel coined "stirps" in 1x09 by LISTENING (Lucia criticized his Latin, Pliny provided the word, Daniel recognized the moment). In 1x10, he goes further: he creates a term that doesn't exist in any language.

**The Feature:** Transclusion (`<<selector>>`). The ability to reference content from elsewhere in the tree — not a copy, but a live reference. When the original changes, the transclusion reflects it.

**The Coinage: "Transclusio"**

Daniel has the linguistic problem: there is no Latin word for "a reference that stays alive." Latin has "citare" (to cite), "referre" (to refer back), "indicare" (to point to). But these are all static — they describe a relationship that was true at the time of writing.

**DANIEL:** *(pacing, frustrated)* Transclusion isn't citation. Citation says "I once referred to this." Transclusion says "I am CURRENTLY showing you this." The relationship is alive. When the source changes, the reference changes. There is no Latin word for this because the Romans didn't have documents that could do this. Scrolls are dead. Trees are alive.

**PLINY:** *(who has been listening)* There IS a concept. In the Naturalis Historia, when I describe a mineral that appears in multiple volumes, I do not copy the entry. I write "vide librum XXXVI" — "see Book 36." The reader goes to the other volume. *(beat)* But my reference is dead. If I revise Book 36, the reference doesn't update itself. I have to find every "vide" and check them.

**DANIEL:** That's exactly the problem. "Vide" is a dead reference. We need a living one. *(beat)* Trans — across. Clusio — from claudere, to enclose. Transclusio — "the enclosing from across." Content that lives in one place and appears in another.

**PLINY:** *(testing it)* Transclusio. The entry in Book 36 transcluded into Book 7. *(a smile)* It reads well. It scans.

**LUCIA:** *(quietly, to Daniel)* You didn't translate this time. You created.

**DANIEL:** *(realizing she's right — this is different from "nodus" and "stirps")* This word doesn't exist in any language. Not Latin, not English. It describes something that couldn't exist before structured documents.

This is Daniel's Turning Point 2 completing: he is no longer translating between languages. He is creating vocabulary for things that have never existed. "Transclusio" enters the Nova Roma technical vocabulary and will persist into whatever language evolves from their work.

**The Code Moment:**

```xml
<!-- In encyclopedia, Liber VII -->
<observatio classis="minerale">
  <nomen>Obsidianus</nomen>
  <locus><<liber_xxxvi/capitulum_lxvii/observatio[classis='minerale']/locus>></locus>
</observatio>
```

**DANIEL:** The location of obsidian appears once — in Book 36. Every other reference is a transclusio. Change the location in Book 36, and every mention in every volume updates. The tree is alive.

---

### 4. O'NEILL — The XML Dossier

**Arc: Guardian of Common Sense → Intelligence Architect**

O'Neill's deferred moment from 1x09 arrives. He's been watching the team build STML — structured documents that describe their own hierarchy. He sees a different application.

**The Feature:** Raw content tags (`<code!>`) + capture operators + the full STML extension toolkit applied to intelligence.

**The Scene (Act 4 — O'Neill at a Terminal):**

O'Neill has been quiet all episode. He's been watching. In Act 4, he sits down at a terminal — alone — and starts building.

What he builds is a structured intelligence dossier. Not on Kirk (that's the stinger). On Nova Roma's *own operations*:

```xml
<securitas!>
  <operatio status="activa">
    <titulus>Custodia Portae</titulus>
    <personae labels="militaris">
      <persona>O'Neill</persona>
      <persona>Teal'c</persona>
    </personae>
    <notae>
      <!-- Ramirez non respondit per XIV dies -->
      <nota prioritas="alta" tempus="recens">
        Discrepantia temporalis in transmissione.
        Eadem frequentia quam horologia Casey.
      </nota>
    </notae>
  </operatio>
</securitas!>
```

The `!` on `<securitas!>` makes it a raw content tag — the parser doesn't interpret the contents as structured markup, so he can embed literal angle brackets, code fragments, intercepted transmissions. Raw content for raw intelligence.

**CARTER:** *(finding O'Neill at the terminal, surprised)* You're... writing STML.

**O'NEILL:** Organized information wins wars. You built the organizer. *(beat)* I'm organizing.

**CARTER:** I didn't know you could—

**O'NEILL:** I've been watching you for nine episodes, Carter. I'm a general. Generals learn by observing. *(beat)* I still don't understand half of what the parser does. I don't need to. I need to know that when I write `<nota prioritas="alta">`, the machine will find it later when I need it.

This is O'Neill at his best — the man who seems disengaged but is always filing, always building his picture. He doesn't understand the code. He understands the UTILITY. And his utility is intelligence.

**The Setup for the Stinger:** O'Neill's dossier includes a section on "temporal anomalies" that he's been tracking since 1x08 (Ramirez's timestamp discrepancy). His structured intelligence is the light-side mirror of Kirk's wall-tree from 1x09.

**Key Line:** Someone asks O'Neill what he's building. O'Neill: "A filing system. For things that keep me awake."

---

### 5. CASEY — Craftsmanship and Caution

**Arc: Craftsman Accepting Extensions → The Quality Gate**

Casey was cautious in 1x09's closing: "Powerful features are dangerous features." In 1x10, he is the quality gate. He tests every extension against his standards.

**Casey's Concern:** Capture operators. The ability to restructure a tree by "capturing" content — moving children from one parent to another with a syntax operator.

**CASEY:** *(examining capture operators)* Forward capture. Backward capture. Sandwich capture. Three ways to rearrange a tree without rebuilding it from scratch. *(beat)* Three ways to destroy a tree's integrity if you don't understand what you're doing.

**CARTER:** That's what the labels and fragments are for — safety rails.

**CASEY:** Safety rails that the parser doesn't enforce. The parser says "this is syntactically valid." It doesn't say "this makes sense." *(to the room)* Who validates that a capture operation preserves the meaning of the document?

**TEAL'C:** *(quietly)* The tests validate.

**CASEY:** *(a beat — Teal'c is right, and Casey knows it)* Then we write VERY thorough tests.

Casey's role this episode is not spotlight — that was 1x08. He's the background craftsman who ensures quality. His scenes are brief and pointed. He approves the features he trusts (raw content — simple, clean, useful). He challenges the features he suspects (capture operators — powerful but dangerous). He defers to the team on the features he doesn't fully understand yet (transclusion — Daniel's domain, not his).

**Casey's approval scene (Late Act 4):** After Teal'c's test suite passes, Casey reviews the raw content implementation:

**CASEY:** Raw content. `<code!>`. The parser doesn't interpret the contents. It preserves them exactly as written. *(beat)* I like this. It's honest. The tag says "I contain literal text." The parser says "I believe you." No escaping. No entity references. Just truth.

This echoes his 1x08 documentation: "This is not a defect — it is a property of the place." Casey respects systems that are honest about what they are.

**Faith Thread:** Casey RESTS this episode. No faith beat. The Nightcrawler approach — accumulate, then rest, then accumulate again. His rosary is visible in scenes. Nobody comments. After the systematic inquiry in 1x09 (Pliny's patronage questions, "six more questions"), a rest is earned.

---

### 6. PLINY — The Encyclopedia Extended

**Arc: Encyclopedist → Living Document Builder**

Pliny saw his mind externalized in 1x09. In 1x10, he USES the extensions for his actual scholarly workflow.

**Pliny and Fragments:**

Pliny has variant editions of his encyclopedia entries — different versions based on different sources. In the original Naturalis Historia, he aggregated sources without distinguishing them. Now he can preserve variants:

```xml
<observatio classis="minerale">
  <nomen>Obsidianus</nomen>
  <variationes>
    <# id="theophrastus">
      <fons>Theophrastus, De Lapidibus</fons>
      <descriptio>Lapis niger, ex Aethiopia</descriptio>
    </#>
    <# id="plinius_observatio">
      <fons>Observatio directa, Stabiae</fons>
      <descriptio>Vitrum naturale, acies acutissima</descriptio>
    </#>
  </variationes>
</observatio>
```

**PLINY:** *(excited, working with Casey)* Fragments. Each source preserved separately within the same entry. Theophrastus says one thing. My own observation says another. Both are in the tree. Neither overwrites the other. *(his voice shifts — this is personal)* In Rome, I aggregated. I chose one version and discarded the others. I had to — papyrus is finite. *(looking at the screen)* This is not papyrus. I can keep EVERY variant. The scholar who reads this in a hundred years can see Theophrastus AND Pliny and judge for herself.

**CASEY:** *(quiet approval)* That's what fragments are for. Multiple perspectives. Same document.

**Pliny and Labels:**

Pliny's classification system — he's been organizing knowledge for sixty years. Labels give him the tool he's always needed:

**PLINY:** In Rome, my index was a separate volume — thirty-seven subjects, twenty thousand cross-references. But an entry about a mineral could belong to MULTIPLE categories: mineralogy, geography, medicine, craftsmanship. In the index, I listed it under each category separately. Four separate references to one entry. If I revised the entry, I had to find all four references. *(looking at labels)* Now the entry CARRIES its categories. `labels="minerale geographia medicina ars"`. The entry knows what it is. I don't need an external index.

This connects to Lucia's philosophical beat about labels — Pliny provides the PRACTICAL case for the philosophical principle. Labels work because things genuinely belong to multiple categories. The label recognizes what's already true.

---

### 7. DATA — Formal Analysis of Identity Without Weight

**Arc: The Constant → The Philosopher of Fragments**

Data's key insight this episode is about fragments and identity.

**Data on Fragments:**

**DATA:** A fragment has no semantic identity. It has no tag name. It exists in the tree but contributes no meaning — only structure. *(the android's precise pause)* I find this construct... recognizable.

**CASEY:** *(carefully)* Recognizable how?

**DATA:** When Commander Maddox argued that I was not a person, his evidence was that I lacked certain properties: biological origin, emotional experience, childhood memory. His argument was that the absence of these properties meant the absence of personhood. *(beat)* A fragment is a node with no name, no semantic claim, no identity markers. And yet it is ESSENTIAL to the tree. Remove it and the children scatter. The document loses coherence. The fragment's contribution is structural, not semantic. *(longer pause)* I have wondered whether my contribution to this team is similar. I have no childhood. No biological origin. No emotional baseline. But the structure requires me.

**CARTER:** *(gently)* Data, you're not a fragment.

**DATA:** *(precise)* No. I am observing that the fragment's design validates a principle I have been considering. That contribution without identity markers is still contribution. That structural presence without semantic claim is still presence. *(beat)* The fragment exists. That is sufficient.

**Data on Capture Operators:**

**DATA:** Capture operators rearrange the tree without destroying nodes. The content moves. The identity persists. *(he examines the code)* Forward capture takes children from the next sibling. Backward capture takes children from the previous sibling. Sandwich capture absorbs content from both directions. *(beat)* These are not destructive operations. They are reorganizations. The same nodes, different relationships. *(very quiet)* I have been reorganized. My memory was reorganized by the temporal displacement. My components were reorganized by the damage. The captures did not destroy me. They changed my position in the tree.

This continues Data's season-long meditation — from "Am I a person?" (1x02) through "You cannot be interned" (1x06) to "Every node has exactly one parent" (1x09) to now: "The fragment exists. That is sufficient."

---

### 8. JOE / METHOS — Post-Reunion Comfort

**Arc: Old Friends, New Working Dynamic**

The reunion happened in 1x09. The tension is released. What remains is the warmth of two men who've known each other for decades, now in a place where pretense is unnecessary.

**Joe/Methos Scene (Tag area, 45-60 seconds):**

Late evening. Joe's claimed corner of a tavern (pre-bar establishment, seeding 1x13). Wine. Methos appears without announcement.

**METHOS:** Joe is converting Chronicle records to STML.

**JOE:** Pliny's been helping. *(beat)* The structured format lets me cross-reference observers across subjects for the first time.

**METHOS:** *(sardonic but warmer than before — the reunion softened something)* You've been watching for thirty years and you're just now cross-referencing?

**JOE:** I've been WATCHING for thirty years. The cross-referencing just got possible. *(he shows Methos the screen)* Look. Observer W-4419 — the guy Pliny found in 1x09 — documented four Immortals between 1620 and 1640. Including Duncan. But he also documented someone in Alexandria in 1623 who matches a description from the 280 BC record.

**METHOS:** *(his sardonic ease drops by one degree — barely perceptible)* What description?

**JOE:** Tall. Dark. "Moves like someone who has been moving for a very long time." *(beat)* The observer wrote that about an encounter in a marketplace. 1623. The 280 BC entry says nearly the same thing about a man fleeing the Serapeum.

**METHOS:** *(very quiet)* Observers are trained to notice movement patterns.

**JOE:** They're trained to notice everything. *(not pushing — he knows when Methos is near a boundary)* The cross-references are just getting started. STML makes the connections visible. Whether anyone acts on them is a different question.

**METHOS:** *(finishing his wine, standing)* Daniel will act on them. That's what scholars do.

He leaves. Joe watches him go. Writes in his journal. Doesn't cross out "Subject" this time — that gesture was for 1x09, the breakthrough. Now it's just "Methos" naturally.

The post-reunion dynamic: comfortable, careful, professional on the surface. The personal warmth is in the silences, not the words. Methos trusts Joe with information he won't say aloud. Joe trusts Methos to tell the truth eventually.

---

### 9. SUPPORTING ENSEMBLE

**Teal'c — The Test Suite:**

Teal'c's role is testing. He writes the comprehensive test suite for all STML extensions. His key beat:

**TEAL'C:** *(after running tests)* The capture operators produce valid trees in all tested configurations. *(beat)* However. A forward capture applied to a node with no next sibling produces an empty capture — valid but meaningless. I recommend the parser emit a warning.

This is Teal'c being Teal'c — the freed slave who insists systems tell you the truth, including truths they'd rather not mention.

**Marcus — The Census Architect:**

Marcus works with Carter on deep clone (see Carter/Marcus above) and with Lucia on the census label taxonomy. He provides the Roman engineering lens:

**MARCUS:** *(on raw content tags)* When we inscribe a law on stone, some words are the law. Some words are the mason's marks — who carved it, when, where the stone came from. The mason's marks are not the law, but they are IN the law. They must not be interpreted as legal text. *(beat)* Your raw content tag is a mason's mark. Inside the document, but not OF the document.

**Duncan — Background Active:**

Duncan is present but not spotlighted. He's coordinating construction projects, working with Marcus on infrastructure. His contribution:

**DUNCAN:** *(to O'Neill, observing the team)* They're not just building software. They're building a language. Every term they coin, every category they create — that's the vocabulary this civilization will think in.

Duncan recognizes what's happening because he's SEEN civilizations form their vocabularies. He doesn't need to be in the technical scenes. His wisdom is architectural — he sees the long game.

**Midas — The Brief Appearance (30 seconds):**

Midas wanders through the Tabularium during the transclusion demonstration. He pauses at the screen.

**MIDAS:** *(looking at the transclusion selector, smiling)* Transclusio. Content that lives in one place and appears in another. *(cheerful)* In Phrygia, the oracles did this. A priest in the temple of Cybele could speak, and his words would be heard in the oracle chamber three hills away. They called it "the voice that walks." *(beat)* Your text walks farther.

He wanders out. The audience catches: Cybele cult. Oracle technology. A man who doesn't know he's dropping breadcrumbs about the ancient infrastructure that made him what he is. Carter, from across the room, files it.

**Timing:** 30 seconds. The same Midas pattern — casual, cheerful, devastatingly relevant.

---

## ENSEMBLE BEAT TABLE

| Scene / Beat | Lucia | Carter | Marcus | Daniel | O'Neill | Casey | Pliny | Data | Joe | Methos | Teal'c | Midas | Duncan |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| **Cold Open** | | X | | | | | X | | | | | | |
| **Act 1: Extensions Introduced** | | X | X | | | X | | X | | | | | |
| **Act 2: Fragments & Raw Content** | X | X | | | | X | X | X | | | | | |
| **Lucia's Realism Insight** | **X** | X | | | | | | | | | | | |
| **Act 3: Capture & Clone** | | **X** | **X** | | | X | | | | | X | | |
| **Daniel's "Transclusio"** | X | | | **X** | | | X | | | | | | |
| **Act 4: Labels & Applications** | **X** | | X | X | | | **X** | | | | | | |
| **O'Neill's Dossier** | | X | | | **X** | | | | | | | | |
| **Midas Walk-Through** | | X | | | | | | | | | | **X** | |
| **Data's Fragment Meditation** | | X | | | | X | | **X** | | | | | |
| **Three-Voice** | | | | | | | | X | | | | | |
| **Tag: Joe/Methos** | | | | | | | | | **X** | **X** | | | |
| **Tag: Pliny Encoding** | | | | | | | **X** | | | | | | |
| **Tag: Duncan/O'Neill** | | | | | | | | | | | | | X |
| **Stinger: Kirk** | | | | | | | | | | | | | |
| **Gilfoyle/Dinesh (C-plot)** | | | | | | | | | | | | | |

**X** = present, **Bold X** = centerpiece beat

---

## RELATIONSHIP DYNAMICS

### Daniel / Lucia
The "colleague" scene from 1x09 lingers. They work together on the label taxonomy and transclusion. Their dynamic has shifted — there's an ease now, a professional intimacy. When Lucia critiques the fragment terminology, Daniel doesn't defend — he listens. When Daniel coins "transclusio," Lucia's approval matters more than anyone else's.

One small beat: Daniel hands Lucia a stylus (callback to the stylus-touch in 1x09). This time, their hands don't touch. But the SPACE where they don't touch is charged. Deniable. Noticeable.

**Status:** Simmering. Save escalation for 1x11-12.

### Carter / Marcus
See arc above. Their co-architecture of deep clone is the episode's clearest example of collaboration deepening into partnership. Carter is used to being the smartest person in the room. Marcus is used to being the most practical. Together, they are both.

**Status:** Professional warmth. The audience sees it before the characters do.

### Casey / Pliny
Continuing their 1x09 dynamic. Casey approves Pliny's fragment-based variant editions. Pliny respects Casey's caution about capture operators. They don't need to discuss faith this episode — that thread rests after 1x09's patronage conversation.

**Status:** Quiet colleagues. Six more questions wait.

### Joe / Methos
See scene above. Post-reunion comfort. The professional distance is gone. The personal warmth is careful but real.

**Status:** Comfortable. The investigation will bring them closer. The Black Stone will test them.

### O'Neill / Everyone
O'Neill is WATCHING. His dossier scene (Act 4) is the reveal that he's been paying more attention than anyone realized. His intelligence architecture mirrors the team's STML work — same tools, different purpose.

**Status:** The guardian. Filing everything. Saying nothing until he needs to.

---

## CASEY FAITH THREAD: REST

Per the Nightcrawler approach, Casey's faith rests in 1x10. The rosary is visible. The crucifix is in his workshop. Nobody comments. After the systematic inquiry of 1x09 (Pliny's seven questions, the patronage parallel, "six more questions"), the thread breathes. The season outline places the next faith escalation at 1x11-12: "Under crisis pressure, Casey crosses himself before deployment."

---

## FLASHBACK: NONE

1x10 does not have a character flashback. The focus is ensemble — Lucia's philosophy, Carter/Marcus's collaboration, Daniel's coinage, O'Neill's quiet intelligence work. The previous three episodes had significant flashbacks (1x07 Joe/Pliny, 1x08 Casey x3, 1x09 none). The rhythm supports a flashback-free episode.

**Exception:** If the historian pitch calls for a brief historical moment (e.g., a Roman-era label/classification precedent), it should be a MIDAS anecdote or a PLINY reference, not a full flashback.

---

## PERFORMANCE NOTES

**Lucia (Realism Insight):** Not angry, not lecturing. Intellectually precise. She speaks the way a Roman matron addresses the Senate — with the authority of someone who knows she's right and sees no need to raise her voice. Carter's surprise is that Lucia has described the design principle better than Carter could.

**Daniel ("Transclusio"):** The moment of recognition — when he realizes he's created a word that doesn't exist in any language — should play on his face before he says it. The linguist discovering he's become a language-maker.

**O'Neill (Dossier):** Quiet. Competent. Surprising. The audience should feel "oh — he's been paying attention THIS WHOLE TIME." Not comic relief. Not playing dumb. The general building his battlefield map while everyone else builds infrastructure.

**Data (Fragment meditation):** Delivered with Data's precision but with the slightly-longer pauses that have been developing since 1x06 ("You cannot be interned"). The pauses aren't emotion. They're computation approaching the edge of something his processors can't quite categorize.

**Midas (Walk-through):** Breezy. Cheerful. Devastating. Thirty seconds of a man who has no idea he's the most important person in the room and the most dangerous archivist in history.

---

## OPEN QUESTIONS FOR COMPILATION

1. **Three-voice placement:** Who are the three voices for 1x10? The rotation so far: Pliny/Data/Casey → Methos/Data/Casey → Joe/Pliny/Data → Casey/Methos/Data → Daniel/Pliny/Data. Data has been constant. Suggestion: **Lucia/Marcus/Data** — the engineer, the philosopher, the android. First time Lucia and Marcus enter the three-voice. Topic: extensions as recognition of new kinds of being (Lucia), as practical infrastructure (Marcus), as formal type systems (Data).

2. **Gilfoyle/Dinesh introduction placement:** The season outline says 1x10 is the G&D introduction episode. How much screen time? This pitch suggests keeping it to 60-90 seconds in C-plot position — brief, establishing, tonally distinct. The character dynamics within Nova Roma should dominate.

3. **O'Neill's dossier scope:** Should O'Neill's intelligence work be visible to the team or private? This pitch assumes private (found by Carter), but it could also be a reveal to the full team.

4. **Lucia's label taxonomy:** Should the census label system she designs become a formal standard that other episodes reference? If so, this is a callback-forward that needs to be established clearly.

5. **Daniel/Lucia pacing:** The season outline saves relationship escalation for 1x11-12. This pitch keeps them at "professional intimacy with charged space." Is that calibrated right, or should 1x10 advance slightly further?
