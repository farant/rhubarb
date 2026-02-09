# NOVUM FUNDAMENTUM 1x09 — CHARACTER / FLASHBACK PITCH
## "Arbor Documentorum" (Tree of Documents)
### Library: STML (Structured Text Markup Language — DOM tree, XML superset)

---

## 1. DANIEL'S LINGUISTIC CREATION MOMENT

### The Problem Daniel Faces

Daniel has been translating. Since 1x03, Lucia has pushed him from "putting your thoughts in our words" toward something else. In 1x09, the team needs to describe what a tree of documents IS — not translate "XML" or "DOM" but CREATE a Latin concept that captures hierarchical structure, parent-child relationships, nested containment.

Daniel's first instinct is translation:

**DANIEL:** *(at the board, brainstorming)* "Arbor" for tree. "Nodus" for node. "Radix" for root. "Liberi" for children. "Parens" for parent.

**LUCIA:** *(reading his list, unimpressed)* These are Latin words. They are not Latin *thinking*. You have mapped your language onto ours like a mosaic on foreign stone. A Roman reads "nodus" and thinks of a knot — a complication, something tangled. You want them to think of a branch point. These are not the same idea.

**DANIEL:** They'll learn the technical meaning.

**LUCIA:** They will learn to think like you. Is that your intention? To replace their conceptual landscape with yours?

This is the Turning Point 2 beat from the character arc: "He coins a term that the Romans adopt naturally — it enters their daily vocabulary."

### The Coinage: "Stirps"

The term Daniel coins is **stirps** — a word that already exists in Latin but that he repurposes. In classical Latin, *stirps* means "root stock, lineage, family tree." A Roman hearing *stirps* thinks of genealogy — parent, child, descendant. The family tree IS a tree. The word already contains the data structure.

The scene where it happens:

**DANIEL:** *(after Lucia's critique, pacing)* The problem isn't finding Latin words for tree concepts. The problem is that I'm starting from the tree and looking for Latin. I need to start from Latin and find the tree.

**PLINY:** *(who has been listening)* A tree? My encyclopedia is not a tree. It is organized by subject.

**DANIEL:** But within each subject — your entry on, say, olive trees. It has a heading. Under the heading, sections. Under sections, observations. Under observations, specific details. That's not flat. That's nested. Parents contain children.

**PLINY:** *(the scholar's mind catching fire)* Stirps! The genealogy. The *gens* contains *familiae*. The *familia* contains *domus*. The *domus* contains persons. *(standing)* My encyclopedia is a *stirps*. Volume is the *gens*. Chapter is the *familia*. Entry is the person.

**DANIEL:** *(stunned — Pliny just did his work for him)* Stirps. The family tree.

**LUCIA:** *(quietly, to Daniel)* You did not translate. You reminded him of something he already knew.

This is the difference between translation and linguistic creation. Daniel didn't impose an English concept; he found the Latin concept that was already waiting. "Stirps" enters Roman vocabulary naturally because it was already there — they just hadn't applied it to documents yet.

### How "Stirps" Enters Daily Use

By the end of the episode, multiple Romans use "stirps" unprompted:

- **MARCUS:** "Show me the stirps of the census document." (Not "show me the tree" — *stirps*, because he thinks in genealogy.)
- A **Roman clerk**, filing Pliny's encyclopedia entries: "This observation belongs under which stirps?" (Using it like "category" — which it is, but with hierarchy built in.)
- **PLINY** himself, correcting a filing error: "No — the entry on volcanic glass goes under Mineralia, not Geographia. Follow the stirps."

Daniel hears this. The audience sees him hear it. He doesn't celebrate. He just... nods. A small, private recognition: a word he midwifed is alive. It belongs to them now.

**LUCIA:** *(finding him later)* You heard them.

**DANIEL:** *(nodding)* They're using it.

**LUCIA:** Not using it. Speaking it. There is a difference. When you use a word, it belongs to the language you learned it from. When you speak it, it belongs to you.

---

## 2. PLINY SEES HIS ENCYCLOPEDIA AS STRUCTURED MARKUP

### The Emotional Setup

Pliny's encyclopedia is the data that makes this episode personal. Forty volumes. Eighty years. 2,493 chapters. Organized by Pliny's own system — books, sections, observations. But the organization has always lived in Pliny's HEAD. Other people can't navigate it without asking him (established in 1x05, Beat 2: "Then it takes longer").

In 1x07, the encyclopedia was saved to disk as flat text — preserved but not *structured*. Each entry is a string. The relationships between entries (this observation belongs to this chapter of this volume on this subject) are implicit, not encoded.

### The Revelation Scene

Carter and Daniel build a demonstration. They take Volume 7 of Pliny's encyclopedia — the volume on metals and mining — and encode it in STML:

```xml
<liber titulus="De Metallis et Fossilibus" numerus="VII">
  <sectio titulus="De Auro">
    <observatio locus="Hispania" annus="XLII">
      Aurum in Hispaniae fluminibus...
    </observatio>
    <observatio locus="Nova Roma" annus="LXXX">
      Aurum nondum inventum in Nova Roma...
    </observatio>
  </sectio>
  <sectio titulus="De Argento">
    ...
  </sectio>
</liber>
```

They display the tree on screen. Pliny's entries. Pliny's chapter titles. Pliny's observations. But now VISIBLE as a tree — the parent-child relationships rendered graphically, the nesting explicit, the structure that was always in his mind now externalized.

**PLINY:** *(staring at the screen for a long beat)* That is... my book.

**CARTER:** Volume seven. Metals and mining.

**PLINY:** No. You don't understand. That is my book THE WAY I SEE IT. In my mind. *(he touches the screen the way he touched scrolls in 1x05)* When I think about my encyclopedia, this is what I see. Volumes containing sections. Sections containing observations. Observations containing details. A stirps. A living tree. I have carried this structure in my memory for sixty years. *(his voice breaks very slightly)* No one has ever seen it. Until now.

This is the emotional payload of the episode. Pliny has been the world's most isolated genius — an encyclopedia in a man's body, organized by a system only he can navigate. The DOM tree doesn't just digitize his work. It EXTERNALIZES his mind. For the first time in eighty years, someone else can see the architecture of his thought.

**CASEY:** *(quietly, from behind)* Now it doesn't have to be in your head.

**PLINY:** *(turning to Casey — the man who told him about "the God who remembers everything" in 1x05)* For sixty years, if I had died, the organization would have died with me. The words would survive — I made sure of that. But the *shape* of the knowledge... *(he looks back at the tree on screen)* ...the shape would have been lost. The scrolls would remain. The index in my mind would not.

**CARTER:** Now the shape is on disk. Preserved. Searchable. Navigable by anyone.

**PLINY:** *(sitting down, suddenly exhausted by relief)* The stirps survives the gardener.

### The Montage

A working montage. Pliny dictates, Daniel translates/structures, Lucia verifies the Latin, Carter types. Volume by volume. The tree grows on screen. Pliny watches his life's work become navigable — not just indexed (1x05) but STRUCTURED. The difference between an index (flat, one-dimensional, lookup) and a tree (hierarchical, multi-dimensional, navigable) mirrors the difference between the hash table and the DOM.

---

## 3. DANIEL / LUCIA DYNAMIC

### The Intellectual Equals

Daniel and Lucia are doing the hardest work in the room this episode. Carter builds the parser. Casey and Marcus handle the engineering. But Daniel and Lucia have the CONCEPTUAL problem: how do you make structured markup make sense to Romans?

Their scenes should crackle with intellectual tension:

**LUCIA:** *(reading STML syntax)* An opening tag, a closing tag, content between. This is merely punctuation for prose.

**DANIEL:** It's more than that. The tags name the relationship. `<sectio>` isn't a label — it's a declaration. "Everything inside me is part of this section."

**LUCIA:** *(testing him)* Then the tag is a boundary. Like a wall around a garden. The wall doesn't describe the garden. It declares where the garden begins and ends.

**DANIEL:** *(excited)* Yes! That's exactly —

**LUCIA:** But a wall without a gate is a prison. Your tags open and close. Where is the gate?

**DANIEL:** The closing tag IS the gate. `</sectio>` is the exit.

**LUCIA:** *(smiling — the first genuine smile she's given him)* Then you have not built punctuation. You have built architecture.

### The Unspoken Tension

Physical proximity: Daniel and Lucia spend the entire episode side by side at the work table, debating structure, testing words, building the vocabulary of the new system. There's a scene where they both reach for the same stylus. Their hands touch. Lucia doesn't pull away immediately. Daniel notices. They both go back to work.

Later:

**LUCIA:** *(end of a long work session, late)* You have improved.

**DANIEL:** *(surprised)* At what?

**LUCIA:** At listening. When you arrived, you heard Latin. Now you hear the thought inside the Latin. *(beat)* That is the difference between a translator and a... *(she searches for the word)*

**DANIEL:** A what?

**LUCIA:** *(very quiet)* A colleague.

She leaves. Daniel watches her go. The word "colleague" is loaded — it's the most Lucia has ever acknowledged their intellectual partnership. It's also the least romantic word she could have chosen, which makes it MORE charged. "Colleague" from Lucia Serena is a declaration of equality. From a woman who doesn't consider many people her equal.

---

## 4. CASEY / PLINY FAITH CONVERSATION — The Patronage Scene

### Setup

This scene is specified in the season outline. It's the next escalation in the faith thread — from the crucifix question (1x05: "A man who died for everyone"), to the rosary counting (1x08: "Reminders. That I'm not in charge."), to systematic theology (1x09: Pliny begins asking structured questions).

### The Scene

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

**PLINY:** *(nodding, satisfied — the observation has been classified)* I understand patronage. A client does not approach the emperor directly. He approaches a patron who has the emperor's ear. The patron intercedes. The emperor may or may not grant the request, but the patron has standing to ask.

**CASEY:** Mary has standing.

**PLINY:** Because of her relationship to your God.

**CASEY:** Because she's his mother.

**PLINY:** *(sitting back — this is a significant datum)* The patron is the God's MOTHER. *(he writes in his notebook)* That is... a considerably powerful patron.

**CASEY:** *(small smile)* Yeah.

**PLINY:** *(writing, almost to himself)* "Patrona Dei — mater ipsius. Intercessio non est adoratio sed petitio per auctoritatem." *(The patron of God — his own mother. Intercession is not worship but petition through authority.)*

He closes the notebook. Looks at Casey.

**PLINY:** I have six more questions. They are more difficult.

**CASEY:** This is going to take more than one evening.

**PLINY:** *(the scholar who has been waiting eighty years for interesting answers)* I have been waiting eighty years for answers that have a structure I can study. Your faith has structure. It has a stirps. *(beat — he just used the episode's new word to describe Casey's religion)* I don't have to believe in it to study it.

**CASEY:** *(genuinely moved)* That's the best thing anyone's said about it in a long time.

### Why This Scene Works

The Roman social structure — patronage — makes Catholicism's intercessory prayer comprehensible in a way modern secular culture can't access. Modern people hear "praying to Mary" and think it's polytheism or idol worship. Romans hear it and immediately understand: you approach power through intermediaries. The patron system is the operating system of Roman society. Casey has struggled to explain this for two hundred years. Pliny gets it in thirty seconds because his civilization was built on the same architecture.

The faith thread deepens without becoming a subplot. Pliny is cataloging, not converting. His notebook is an *adversaria* entry, not a prayer. But the intellectual respect is genuine, and his use of "stirps" to describe Casey's faith — the tree structure of theology — is the moment when the technical and spiritual threads converge without either dominating.

---

## 5. ENSEMBLE BEAT TABLE

| Character | Role This Episode | Key Scene(s) | Arc Movement |
|-----------|------------------|--------------|--------------|
| **Daniel** | Co-spotlight. Linguistic creation. | Stirps coinage, Lucia partnership, tag-as-architecture | Translator → Language Architect (Turning Point 2) |
| **Pliny** | Co-spotlight. Emotional core. | Encyclopedia-as-tree revelation, patronage conversation | Isolated Scholar → Connected Mentor (his mind externalized) |
| **Carter** | Builder, engineer. Builds the parser. | DOM demonstration, dependency chain, C-plot temporal | Builder Under Strain (Midas secret compounds) |
| **Marcus** | Roman engineer. Structure as architecture. | "Show me the stirps," nesting = load-bearing walls | Native Tech Lead (structural intuition) |
| **Casey** | Faith thread + background craft. | Patronage scene with Pliny | Hermit → Team Member (spiritual mentor now) |
| **Lucia** | Co-architect with Daniel. | Wall/gate metaphor, "colleague" scene | Philosopher → Curriculum Architect |
| **Teal'c** | Tester. Tests nesting, malformed input. | Test suite + nested-depth observation | Quality Engineer (structure requires discipline) |
| **Data** | Precise analyst. Tree traversal. | Three-voice candidate, "tree of ancestry" | Team Member (his own ancestry is a tree) |
| **O'Neill** | User advocate. The clarity question. | "Who decides what nests inside what?" | Guardian of Common Sense |
| **Methos** | One speech, perfectly placed. | Trees of knowledge — "every one I've seen was eventually cut down" | Reluctant Participant (sardonic) |
| **Joe** | Working session with Pliny. Chronicle structure. | Joe's Watcher records AS a tree | Chronicler (his data needs structure too) |
| **Midas** | Brief comedy, temporal plant. | 30 sec — a story about Persian royal archives | The Unknowing Anomaly |
| **Duncan** | Background. | O'Neill brief + observation about Casey/Pliny | Builder (watching community form) |

---

## 6. THREE-VOICE ALGORITHM MOMENT — CANDIDATES

### Candidate A: "The Tree of Ancestry" (Recommended)

**PLINY:** *(looking at his encyclopedia tree on screen)* Every stirps tells you where something came from. Volume seven contains the section on gold. The section on gold contains the observation from Hispania. If you want the observation, you must first know the volume, then the section, then the entry. *(beat)* Knowledge is not flat. It lives in families. It has parents.

**DATA:** *(precise)* A tree is an acyclic directed graph with exactly one root and a unique path between any two nodes. Every node has exactly one parent — except the root, which has none. *(beat)* I find this constraint significant. In all my operational memory — every fact, every protocol, every experience — there is one root node. One origin. Every other memory branches from it. *(quieter)* I was activated on stardate 43205.6. That is my root. Everything I know descends from it. Every conversation, every observation, every friendship. *(a pause that is longer than Data's usual pauses)* When Commander Maddox questioned whether I was a person, he was questioning whether my tree had a root — whether there was a genuine origin, or merely a first instruction.

**DANIEL:** *(who has been listening, the linguist hearing something deeper)* Every language is a tree. Proto-Indo-European is the root. Latin, Greek, Sanskrit — branches. French, Spanish, Italian — children of Latin. Children inherit from parents but they are not their parents. French is not Latin. But French *remembers* Latin. *(beat)* When I coin a word here — when "stirps" enters Roman vocabulary for documents — I'm grafting a new branch onto an ancient tree. The tree doesn't reject it. It grows to accommodate it. That's what living languages do. That's what living trees do.

**Architecture:** Pliny sees the tree as genealogy of knowledge (everything has lineage). Data sees the tree as the structure of memory itself (one root, everything descends). Daniel sees the tree as the structure of linguistic inheritance (languages are trees that grow). Three lenses on the same data structure — family, memory, language.

### Candidate B: "The Tree and the Forest"

A simpler version focusing on containment: Pliny sees a book containing chapters, Data sees a formal hierarchy, Casey sees nested craftsmanship (a cabinet contains drawers, drawers contain tools, tools contain purposes). This version has Casey instead of Daniel as the third voice. Less thematically tight — recommend Candidate A.

---

## 7. JOE / METHOS — FIRST PRIVATE CONVERSATION

### Setup

This has been deferred from 1x07 (the 3-second eye contact at dinner) and 1x08 (separate orbits). It is DUE this episode or next. I recommend placing it in the TAG — late evening, after the team has dispersed.

### The Scene

**Location:** A corner of Nova Roma. Night. Stars. Methos is sitting on a stone wall, wine in hand. Joe finds him. Not by accident. Joe has been waiting for the right moment.

**JOE:** We should probably talk.

**METHOS:** *(not looking up)* We really shouldn't.

**JOE:** *(sitting down anyway — the cane, the grunt, the bartender who has earned the right to sit wherever he wants)* Adam Pierson died eight years ago. Car accident in Vancouver. Very convincing. *(beat)* I wrote the Chronicle entry myself.

**METHOS:** *(still not looking)* How was the funeral?

**JOE:** Short. Not many mourners. The Watchers closed the file.

**METHOS:** *(finally turning)* And then you came here.

**JOE:** And then I came here. *(beat)* Five thousand years, Methos. Twenty-three known aliases, probably two hundred unknown ones. The longest Chronicle in the Archive and most of it is blank pages and question marks.

**METHOS:** I prefer it that way.

**JOE:** I know you do. *(long pause)* I'm not going to write about you.

Methos looks at him sharply. This is unexpected. Joe Dawson — the man who has devoted his life to the record, who told Pliny "the observation survives the observer" — is offering NOT to observe.

**JOE:** I'm not a Watcher anymore. Not really. There's no one to file a report with. *(beat)* And even if there were, you've been... *(searching for the word)* ...decent. Since we got here. Contributing. Writing things down, for God's sake. *(beat)* The Methos I watched for fifteen years wouldn't have written a single word for posterity. He ran FROM records.

**METHOS:** *(very quiet)* People change.

**JOE:** In five thousand years?

**METHOS:** *(the smallest smile — a humor that spans millennia)* Eventually.

Beat. Joe produces a flask. Pours for both.

**JOE:** One question. That's all I'm asking. One, and then we can go back to pretending we don't have history.

**METHOS:** *(drinking)* One.

**JOE:** Alexandria. Daniel told me what you said. The sub-chamber. The priestess. The thing you stopped yourself from saying. *(beat)* What was down there?

Methos is very still. The wine in his hand doesn't move.

**METHOS:** *(after a long silence)* Something old. Older than the Library. Older than Egypt. *(beat)* There was a stone. Black. Dense. The priests said it whispered. They said it gave them instructions — how to organize, how to preserve, how to... grow. *(his voice careful)* The Library wasn't built because someone had a good idea. It was built because the stone told them to.

**JOE:** *(the Watcher, filing everything)* Told them.

**METHOS:** In a way. Not words. Patterns. Impulses. The urge to collect, to catalog, to make everything findable. *(he looks at the Tabularium across the plaza)* Sound familiar?

Joe follows his gaze. The Tabularium. The hash table. The encyclopedia. The filing systems. The impulse to organize.

**JOE:** You're saying...

**METHOS:** I'm saying I've seen this impulse before. In Alexandria. In places older than Alexandria. The drive to structure knowledge, to build trees of information, to make the world navigable. *(beat)* Sometimes it's human genius. Sometimes it isn't.

He stands. Sets down the cup.

**METHOS:** That was your one question. Good night, Joseph.

He walks away. Joe sits alone. Staring at the Tabularium. The trees of Pliny's encyclopedia glowing through the window.

**JOE:** *(to himself, writing in his journal)* "Subject appears concerned that the Nova Roma project echoes a pattern he has observed before. Further investigation required."

He pauses. Crosses out "Subject." Writes: "Methos."

The Watcher who promised not to watch. Already watching.

### Why This Placement Works

The Joe/Methos scene lands harder in 1x09 than it would have in 1x07 or 1x08 because the EPISODE ITSELF is about trees of knowledge. Methos's warning — that the drive to structure knowledge isn't always human in origin — recontextualizes the entire A-plot. The team has been building a DOM tree. Methos has seen this pattern before, in Alexandria, driven by something that wasn't human. The tree is the recurring motif: the tree of documents, the tree of lineage (stirps), the tree of faith (Pliny's patronage stirps), and now — Methos's tree of suspicion. Something planted these seeds before.

---

## 8. OPEN QUESTIONS FOR COMPILATION

1. **Daniel's "stirps" vs. "arbor" vs. "nodus"** — I've pitched "stirps" as the LIVING term Daniel coins. But the episode title uses "arbor" ("Arbor Documentorum"). Should the title reflect the final vocabulary? Or is the tension between the episode title (arbor = generic tree) and Daniel's specific coinage (stirps = family tree / lineage tree) part of the drama? The episode could dramatize the REPLACEMENT — Daniel starts with "arbor" and through Pliny/Lucia's influence arrives at "stirps."

2. **Pliny sees his encyclopedia as a tree — how much screen time?** I've pitched a full revelation scene plus montage. If the A-plot needs more room, the montage can compress. But the revelation scene (Pliny seeing his mind externalized) should not be cut — it's the emotional core.

3. **Casey/Pliny patronage scene — how explicit?** The season outline specifies the Mary/patronage beat. I've written it at full length with Pliny taking notes. This could be shortened if it crowds the episode, but I think the "Patrona Dei" notebook moment is too good to lose — it's Pliny doing what Pliny does (cataloging everything, including God).

4. **Joe/Methos scene — 1x09 or 1x10?** I've argued for 1x09 because the tree motif connects to Methos's Alexandria warning. But if the episode is overfull, this can be pushed to 1x10 without damage. The 1x07 eye contact and 1x08 separate-orbits build enough tension that either episode works for the payoff.

5. **Three-voice: Daniel or Casey as third voice?** I've pitched Daniel (Candidate A — language-as-tree) because his Turning Point 2 is THIS episode and the three-voice should serve the spotlight characters. But the 1x05-1x08 pattern has been {Pliny/Data/Casey, Methos/Data/Casey, Joe/Pliny/Data, Casey/Methos/Data}. Putting Daniel in would be a first. Is that a feature (the rotation is expanding) or a problem (breaking pattern)?

6. **Daniel/Lucia romantic tension — how far this episode?** The hand-touching-on-stylus beat is calibrated to be noticeable but deniable. The "colleague" scene is the real confession. Is this enough for 1x09, or does the compilation plan want more/less?

7. **O'Neill's structural question** — "Who decides what nests inside what?" parallels his 1x05 "Who chose the math?" I've kept it as a one-beat observation. Should it be expanded into a scene?

8. **Midas comedy beat** — What's the 30-second Midas drop-in? I suggest a story about Persian royal archives where Cyrus had scribes who organized documents into clay tablet trees — "They filed everything. When Babylon fell, the filing system survived. The Persians adopted it. The scribes barely noticed the change of empire." A man casually dropping 2,500-year-old archival history because he was THERE. The temporal plant: the clay tablets he mentions are in a museum that Carter knows doesn't have them anymore — they disappeared. Near Midas?

---

*Character / Flashback Pitch for 1x09 "Arbor Documentorum"*
*Writer: Character Writer*
*Status: Complete*
