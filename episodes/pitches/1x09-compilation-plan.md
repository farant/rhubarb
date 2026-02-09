# NOVUM FUNDAMENTUM
## Episode 1x09: "Arbor Documentorum" (Tree of Documents)
### COMPILATION PLAN — Decisions Document

---

## A. STRUCTURAL DECISIONS

### Beat Count and Act Structure

**Recommendation: ~21 beats** (Cold Open + 5 Acts + Tag + Stinger, with 4 C-Plot beats interwoven).

The A-plot writer's act structure is the backbone. Five acts with clear names and emotional progression:

| Section | Beats | Content |
|---------|-------|---------|
| Cold Open | 1 | Pliny's flat-file frustration. Wax smears. "Arbor documentorum." |
| Act One: "Radix" (The Root) | Beats 1-3 | Pliny's presentation. Daniel's first markup. The parser. |
| Act Two: "Rami" (The Branches) | Beats 4-6 | Node types. Querying the tree. Attributes / boolean attributes. |
| Act Three: "Navigatio" (Navigation) | Beats 7-9 | Tree navigation. Building the encyclopedia. Serialization — the round trip. |
| Act Four: "Liberi et Parentes" (Children and Parents) | Beats 10-13 | Comments. Full construction API. Daniel's term. The dependency chain. |
| Act Five: "Arbor Vivens" (The Living Tree) | Beats 14-17 | Pliny's encyclopedia. Pliny sees his mind externalized. Title moment. Full serialization. |
| Tag | Beats 18-19 | Pliny alone at the terminal. Casey/Carter forward. |
| Stinger | Beat 20 | Kirk's stinger. |
| C-Plot | Beats C1-C4 | Carter's emotional-state experiment. Joe/Pliny structured chronicles. Daniel's STML dossier. Joe/Methos reunion. |

---

## B. CONFLICT RESOLUTION

### Conflict 1: Daniel's Coined Term — "Nodus" vs. "Stirps"

**A-plot pitch:** Daniel coins "nodus" (node/knot). Roman engineer uses it naturally. "So the census document — the radix nodus contains the regio nodi..."

**Character pitch:** Daniel coins "stirps" (rootstock/family tree/lineage). Pliny supplies the word through the genealogy parallel. "My encyclopedia is a *stirps*." Lucia validates: "You did not translate. You reminded him of something he already knew."

**Historian pitch:** Both terms documented — "nodus" as knot/junction, "stirps" as lineage/stock. Both have rich classical resonance.

**DECISION: Use STIRPS as the character-level coinage and NODUS as the technical term. They are complementary, not competing.**

Reasoning:

1. **"Stirps" is the BIG word — the one Daniel coins through collaboration with Pliny/Lucia.** It's the conceptual breakthrough: not "tree as data structure" but "tree as genealogy, as natural hierarchy that Romans already understand." The stirps scene is Daniel's Turning Point 2 — the linguist who creates rather than translates. This is character work. It stays.

2. **"Nodus" is the TECHNICAL word — it's already baked into the code (`StmlNodus`).** It refers to the individual node, not the whole tree structure. The A-plot writer's Roman engineer scene ("the radix nodus contains the regio nodi") is excellent — it shows the technical vocabulary being adopted. This scene stays too.

3. **The two words serve different functions.** "Stirps" describes the WHOLE — the document-as-tree, the genealogical metaphor, the concept. "Nodus" describes the PART — each individual junction in the tree. A document is a stirps made of nodi. Marcus: "Show me the stirps of the census document" (the whole tree). Roman engineer: "The radix nodus contains the regio nodi" (the individual nodes). Both moments survive. Both are Daniel's contribution, at different scales.

4. **Daniel's arc is RICHER with both.** He starts with "nodus" (the technical term, a translation/repurposing), then arrives at "stirps" (the conceptual breakthrough, through Pliny's contribution). The scene order: Beat 2 establishes "nodus" as the term for individual nodes during the first markup. Beat 12 is the stirps revelation — deeper, more conceptual, growing from Pliny's genealogy insight.

5. **The episode title stays "Arbor Documentorum" — neither stirps nor nodus.** "Arbor" is the generic/botanical word. "Stirps" is the lived, genealogical word the Romans adopt. The tension between them IS the episode's linguistic drama: Daniel starts with "arbor" (the computer science concept), arrives at "stirps" (the Roman concept). The title represents where they START; the vocabulary represents where they END.

### Conflict 2: Pliny's Encyclopedia-as-Tree Revelation — Which Version?

**A-plot pitch:** Pliny sees his encyclopedia parsed into STML, queries it, is delighted. "That query would have taken me three hours with flat files. You found it in two lines." Emotional crescendo at Beat 14-16 (title moment): "The document is its own map."

**Character pitch:** Full revelation scene. Carter and Daniel encode Volume 7 as a demonstration. Pliny: "That is my book THE WAY I SEE IT. In my mind." His voice breaks. "No one has ever seen it. Until now." Then Casey: "Now it doesn't have to be in your head." Montage of the encyclopedia being structured.

**DECISION: Use the CHARACTER writer's revelation scene as the emotional payload, placed in Act 5 (Beat 14-15 area). Use the A-plot writer's querying/navigation scenes in Acts 2-3 as the technical buildup.**

Reasoning:

1. The character writer's version is the emotional core: "No one has ever seen it. Until now." This is the episode's equivalent of Casey's "Not a defect — a property of the place" in 1x08. It cannot be cut.

2. The A-plot writer's querying scenes (Beat 5 — "That query would have taken me three hours") are the TECHNICAL buildup that makes the emotional payoff land. Pliny first experiences the utility (querying is faster), THEN experiences the existential revelation (the tree externalizes his mind). The arc is: frustration (cold open) → utility (Act 2) → wonder (Act 5).

3. **Cut the montage.** The character writer's working montage (Pliny dictates, Daniel structures, Lucia verifies, Carter types) is beautiful but the episode is already dense. Replace with a SINGLE beat: Pliny and Casey building one entry together (the A-plot's Beat 8 — building the encyclopedia tree node by node). This is the montage compressed into one scene.

4. The character writer's "The stirps survives the gardener" line — KEEP. Place it at the end of the revelation scene. This is Pliny's mortality thread at its sharpest.

### Conflict 3: Kirk Stinger Word — TAXONOMIA vs. HIERARCHIA vs. ARBOR

**A-plot pitch:** TAXONOMIA. Kirk builds a classification tree of Nova Roma's organizational structure. "Everything organizes itself eventually. You just have to watch long enough." 9/31.

**Threads pitch:** HIERARCHIA. Kirk builds a compartmentalized org chart — "a tree with no root." Cells, layers, nobody below the second level knows anyone above it. "They think they're building a civilization. They're building my intelligence network's architecture for me." 9/31.

**Genre pitch:** ARBOR. Kirk reorganizes his intelligence from flat lists to a wall-tree. Personnel → Military/Civilian/Anomalous. Infrastructure → Gate Telemetry/Temporal/Comms. "A list tells you WHAT you know. A tree tells you HOW it connects." 9/31.

**DECISION: Use TAXONOMIA (A-plot) as the stinger word, but incorporate the threads writer's compartmentalized-network concept and the genre fan's wall-tree visual.**

Reasoning:

1. **TAXONOMIA is the strongest word** because it carries the dark-mirror inversion most sharply. The team builds a tree to REVEAL natural structure (realist). Kirk builds a taxonomy to IMPOSE artificial structure (nominalist classification for intelligence purposes). "Taxonomy" implies classification-as-weapon — sorting people into threat categories.

2. **HIERARCHIA** is too on-the-nose. The episode is about hierarchy. Kirk should shadow the concept with a different word, consistent with the TABULARIUM/LEXICON/ARCHIVUM/CHRONOGRAPHIA pattern where Kirk uses DIFFERENT terminology than the team.

3. **ARBOR** matches the episode title, which breaks the pattern. Kirk has always used his own vocabulary. TAXONOMIA shadows "tree" without naming it.

4. **From the threads writer:** The "tree with no root" concept is excellent and should stay. Kirk's organization is deliberately rootless — compartmentalized, resilient, no single point of failure. This is the nominalist tree: structure without essence. Include the aide/cell-restructuring dialogue.

5. **From the genre fan:** The wall-tree visual (replacing the whiteboard) is the right escalation. Kirk's intelligence grows from flat lists (1x05-08) to a tree structure (1x09). The visual progression mirrors Nova Roma's progression.

6. **Final stinger structure:**
   - Kirk's monitoring station. The wall has been reorganized — no longer lists. A TREE. Floor-to-ceiling.
   - Aide enters with the Seattle cell restructuring report. "Three layers. Nobody below the second level knows anyone above it."
   - Kirk: "If they capture any branch, the root survives. If they capture the root — there is no root."
   - Kirk circles the ANOMALOUS branch on the wall tree: MACLEOD, D. / CASEY / METHOS. Three unknowns.
   - Writes on whiteboard: **9 / 31**
   - Below CHRONOGRAPHIA: **TAXONOMIA**
   - "Everything organizes itself eventually. You just have to watch long enough."
   - SMASH TO BLACK.

### Conflict 4: Joe/Methos Reunion — Location, Tone, and Alexandria Content

**Character pitch:** Outdoors. Stone wall at night. Stars. Joe approaches Methos. "We should probably talk." / "We really shouldn't." Joe reveals he wrote Adam Pierson's death record. Methos reveals the black stone in Alexandria — a stone that "whispered," that gave instructions, that drove the impulse to catalog. "Sometimes it's human genius. Sometimes it isn't." Joe, alone after: "Subject appears concerned..." Crosses out "Subject," writes "Methos."

**Threads pitch:** Indoors. Joe's temporary quarters. Methos appears unannounced. "Joe." / "Methos." Extended conversation: Adam Pierson's Watcher file still open. Methos's Alexandria cataloguer parable ("the happiest man in Alexandria — he found the thing that didn't end"). Methos gives implicit permission for Daniel's investigation: "He'll find what I found. But he'll understand it better than I did." 120-150 seconds.

**DECISION: Use the THREADS writer's version as the primary structure, but incorporate the character writer's black stone reveal and Joe's journal entry.**

Reasoning:

1. **Setting: INDOORS (threads writer).** Joe's quarters are more intimate than outdoors. The scene needs to feel private, enclosed, wine on a table. The stone wall / stars setting is cinematic but fights the intimacy. Save outdoor Methos scenes for later.

2. **Opening: THREADS writer's version is crisper.** Methos appears without announcement. "Joe." / "Methos." No preamble. These two don't need setup — three episodes of deferred tension is the setup.

3. **The Adam Pierson file: COMBINE.** Use the character writer's detail that Joe wrote Adam Pierson's obituary himself, AND the threads writer's beat that the file is still open ("Because closing a file means the story is over").

4. **The Alexandria cataloguer parable: KEEP (threads).** "The happiest man in Alexandria. He'd found the thing that didn't end — the work." This is Methos at his narrative best. A story that describes Joe, Pliny, and the A-plot simultaneously.

5. **The black stone reveal: MODIFIED (character).** Include the character writer's revelation — the stone in the sub-chamber, the whispers, the impulse to organize — but make it BRIEFER and less explicit than the character writer's version. Methos should NOT describe the stone in detail. He should describe the IMPULSE it created: "The drive to structure knowledge, to build trees of information, to make the world navigable. Sometimes it's human genius. Sometimes it isn't." This is more unsettling because it's vague. The audience fills in the blanks.

6. **The implicit authorization: KEEP (threads).** "He'll find what I found. But he'll understand it better than I did." Methos trusting Daniel's investigation. The green light, given implicitly.

7. **Joe's journal entry: KEEP (character).** Crosses out "Subject." Writes "Methos." The Watcher who promised not to watch, already watching. This is the scene's coda.

8. **Timing: 120 seconds.** This is a big scene. The longest C-plot beat in the season. It's earned — three episodes of deferral. But trim it from the threads writer's 150 seconds. Cut the "people change" / "in five thousand years?" / "eventually" exchange — it's charming but the scene doesn't need it.

### Conflict 5: Casey/Pliny Faith Scene — Scope and Placement

**Character pitch:** Full scene at Pliny's villa. Evening. Wine. Pliny has a page of numbered questions. Seven questions prepared, the seventh is brief: the Mary/patronage parallel. "Patrona Dei — mater ipsius." Pliny writes it in his adversaria. "I have six more questions." / "This is going to take more than one evening." Full length.

**Threads pitch:** Workshop setting. Late in the day. Pliny has a wax tablet with systematic questions. Same patronage parallel. Adds the Salii comparison (repetitive devotional practice parallel). Pliny: "Perhaps it should [involve dancing]." — first religious humor. "Fourteen more questions." Extends the encounter with rosary/beads detail.

**DECISION: Use the CHARACTER writer's scene as the primary version, in Pliny's villa (evening), but incorporate the threads writer's Salii comparison and humor beat.**

Reasoning:

1. **Location: Pliny's villa (character writer).** The faith conversations work better in Pliny's personal space. The villa is where Pliny is a scholar, not a team member. The workshop setting (threads) would make this feel like work; the villa makes it feel like intellectual friendship.

2. **Question count: SEVEN (character writer), not fourteen (threads writer).** Seven questions with "six more" implies focused, manageable follow-up. Fourteen risks making Pliny seem obsessive rather than systematic. Seven is the scholarly number anyway.

3. **The Salii comparison: INCLUDE (threads writer).** Pliny comparing the rosary to the Salii priests' counting dance is perfect Pliny — he compares, he doesn't evaluate. And "Perhaps it should [involve dancing]" is the first joke Pliny has made about religion. That's an escalation. Include it AFTER the patronage parallel, as a lighter coda.

4. **"Patrona Dei" notebook moment: KEEP (character writer).** Pliny writing in Latin in his adversaria: "Patrona Dei — mater ipsius. Intercessio non est adoratio sed petitio per auctoritatem." This is too good to lose. The scholar cataloging God.

5. **"Your faith has a stirps": KEEP (character writer).** Pliny using the episode's new word to describe Casey's religion. The technical and spiritual threads converge. This only works if the stirps coinage has already happened (it will — in Act 4, before this scene).

6. **Timing: 90-120 seconds.** The season outline specifies this beat. It earns its length. But trim the threads writer's extended rosary discussion — the rosary has been handled in 1x07 and 1x08; this scene is about the NEXT step (systematic theology, patronage).

### Conflict 6: Three-Voice Algorithm Moment — Participants and Content

**A-plot pitch:** Not specified directly, but Daniel's linguistic creation and Pliny's encyclopedia are the episode's emotional poles.

**Character pitch (Candidate A):** Pliny / Data / Daniel. "The Tree of Ancestry." Pliny sees tree as genealogy of knowledge. Data sees tree as structure of memory (one root, everything descends; "Measure of a Man" resonance). Daniel sees tree as structure of linguistic inheritance (proto-languages, branching). Three lenses: family, memory, language. RECOMMENDED.

**Character pitch (Candidate B):** "The Tree and the Forest" — Pliny/Data/Casey. Less thematic fit.

**Genre pitch:** Daniel / Pliny / Data. Daniel as grammar, Pliny as taxonomy, Data as formal guarantee ("single parentage means unambiguous authority"). Three independent proofs of realism.

**DECISION: Use Daniel / Pliny / Data (both character and genre writers agree on the trio). Use the CHARACTER writer's Candidate A content as the primary speeches, augmented with the genre writer's Data speech about single parentage.**

Reasoning:

1. **Daniel MUST be in the three-voice moment.** His Turning Point 2 is THIS episode. The three-voice should serve the spotlight characters. The 1x05-1x08 pattern has been {Pliny/Data/Casey, Methos/Data/Casey, Joe/Pliny/Data, Casey/Methos/Data}. Daniel entering the rotation is a feature — it means the three-voice is expanding, which tracks with the ensemble growing. Casey can rest here; he had the last two.

2. **Pliny MUST be in it.** His encyclopedia-as-tree is the episode's emotional core. He's the co-spotlight.

3. **Data is the constant.** He's been in every three-voice moment. He's the formal anchor.

4. **Content: MERGED.**
   - **Pliny:** The character writer's genealogy-of-knowledge speech. "Every stirps tells you where something came from... Knowledge is not flat. It lives in families. It has parents." Short, scholarly, emotional.
   - **Data:** The genre writer's formal-guarantee speech, with the character writer's "Measure of a Man" resonance layered in. "Every node has exactly one parent. This constraint means authority is always resolvable... When Commander Maddox questioned whether I was a person, he was questioning whether my tree had a root — whether there was a genuine origin, or merely a first instruction." The genre writer's mathematical precision + the character writer's existential weight.
   - **Daniel:** The character writer's linguistic-inheritance speech. "Every language is a tree. Proto-Indo-European is the root... When I coin a word here — when 'stirps' enters Roman vocabulary — I'm grafting a new branch onto an ancient tree."

5. **Placement:** Late Act 4, after Daniel's stirps coinage (Beat 12) and the dependency-chain discussion (Beat 13). The three-voice grows from the realization that trees are everywhere — in knowledge, in memory, in language.

### Conflict 7: Daniel/Lucia Romantic Tension — How Far?

**Character pitch:** Hand-touching-on-stylus beat. "Colleague" scene at end of long work session. Calibrated to be noticeable but deniable.

**A-plot pitch:** Daniel and Lucia collaborate on the first markup. "The language should belong to the people who use it." Intellectual partnership, no overt tension.

**DECISION: Include the CHARACTER writer's "colleague" scene and the stylus moment, but TRIM. The A-plot scenes carry the intellectual dynamic; the character writer adds the personal undertone.**

Reasoning:

1. The "colleague" scene is excellent: "You have improved... at listening." The word "colleague" from Lucia is a declaration of equality. Include in the Tag area, after the main work is done.

2. The hand-touching-on-stylus: INCLUDE as a brief moment during an Act 2-3 teaching scene. Not a scene — a BEAT within a scene. Three seconds. Nobody comments.

3. Do NOT expand beyond this. The Daniel/Lucia dynamic should simmer, not boil. The character writer calibrated it correctly — "noticeable but deniable." Save escalation for 1x11-12.

### Conflict 8: Cold Open — Pliny Alone or Pliny with Context?

**A-plot pitch:** Pliny at his desk. Three documents open. Cross-referencing problem visible. Wax tablet marginalia. Stylus slips. Three hours of work, smeared. Pliny sees the tree in his own data. "Arbor documentorum." SMASH TO MAIN TITLES.

**Character pitch:** No explicit cold open proposed (starts with Daniel's linguistic problem).

**Threads pitch:** No explicit cold open.

**Historian pitch:** Provides grounding for Pliny's working method (commentarii, excerpting, the lector system).

**DECISION: Use the A-PLOT writer's cold open entirely. It's perfect.**

Reasoning:

1. The cold open establishes the PROBLEM (flat files can't express structure) and the PERSON (Pliny, the episode's emotional anchor) in a single scene. The smeared wax tablet is the visual hook.

2. The historian's detail about Pliny's working method (lector, commentarii, excerpting) enriches the scene without changing it. Add one detail: Pliny's wax tablet as his *commentarii* — the intermediate working notes. The audience sees: this man has been indexing knowledge by hand for decades.

3. "Arbor documentorum" as Pliny's own phrase, spoken to himself before the team ever uses it — this is powerful. The title comes from the character who needs it most.

### Conflict 9: O'Neill's Role — Scene or Observation?

**Character pitch:** O'Neill asks "Who decides what nests inside what?" — paralleling his 1x05 "Who chose the math?"

**Threads pitch:** O'Neill building his own XML dossier for intelligence work. Three timestamp irregularities.

**A-plot pitch:** O'Neill not prominently featured.

**DECISION: INCLUDE O'Neill's "Who decides what nests inside what?" as a ONE-BEAT observation during the A-plot, NOT as a separate scene. DEFER O'Neill's XML dossier to 1x10.**

Reasoning:

1. The episode is already dense with Daniel/Pliny/Joe/Methos/Carter C-plot scenes. Adding a full O'Neill scene risks overload.

2. But the question is essential — it parallels "Who chose the math?" and it's the legitimate political question about hierarchical structure. Include it during the first markup scene (Beat 2) or the node types scene (Beat 4). O'Neill asks, Marcus answers (architecture has natural load-bearing constraints), Daniel adds (the data determines its own structure). Done in 30 seconds.

3. The threads writer's O'Neill dossier is a good idea but 1x09 has too many moving parts. Defer. O'Neill learning XML from the A-plot and applying it to intelligence is a natural 1x10 beat.

### Conflict 10: Midas Comedy Beat

**Character pitch:** Story about Persian royal archives. Cyrus had scribes who organized documents into clay tablet trees. "When Babylon fell, the filing system survived." Temporal plant: clay tablets now missing from a museum Carter knows about.

**Threads pitch:** No specific Midas beat.

**A-plot pitch:** No specific Midas beat.

**DECISION: INCLUDE the character writer's Persian archives story, compressed to 30 seconds. Place during the Act 3-4 transition (dependency chain or construction API scenes).**

Reasoning:

1. The Midas comedy beats have been running since 1x05 (Herodotus sundial in 1x08). The pattern is: Midas drops a 30-second historical anecdote that is simultaneously funny, historically grounded, and a temporal anomaly plant hiding in plain sight.

2. The Persian archives story works: clay tablet trees = filing system that survives regime change = the persistence of hierarchical organization. Midas was THERE. The temporal plant: Carter recognizing that the tablets he mentions disappeared. Brief glance. Nobody follows up.

3. Keep to 30 seconds. Midas wanders through, tells the story, wanders out. Same rhythm as the Herodotus sundial in 1x08.

### Conflict 11: Carter's C-Plot — Emotional-State Experiment

**Threads pitch:** Carter logs Midas's temporal signature correlated with emotional states. Frequency shifts slightly when agitated (fourth decimal place). Amplitude increases. XML-structured data logs. "It responds to his emotional state. It's not just alive. It's connected to him."

**Other pitches:** No competing version.

**DECISION: INCLUDE. Place as C-plot Beat C1 (Act 2-3 area). 45 seconds.**

Reasoning:

1. The thread appendix from 1x08 explicitly asks: "Does the frequency shift with Midas's emotional state?" This is the answer. Yes. The fourth decimal place shifts. Amplitude increases under stress.

2. The threads writer's XML format for the logs is correct — Carter uses the A-plot's new tool for her private investigation. This is perfect meta-level storytelling: the tool that structures Pliny's encyclopedia also structures Carter's secret.

3. The risk of "too many revelations" is real, but this is a tiny C-plot beat. 45 seconds. Carter alone. One whispered line. It deepens the Midas thread without commanding attention.

### Conflict 12: Teal'c's Role — Test Scene Structure

**Character pitch:** Teal'c tests nesting, malformed input. "Quality Engineer."

**A-plot pitch:** Error handling shown during the parser demonstration (Beat 3). "If you open a tag and don't close it, the parser tells you."

**DECISION: FOLD Teal'c's testing into the A-plot's error handling beat (Beat 3). Teal'c runs the malformed input test. "Tag non clausum: familia." Teal'c: brief observation about structure requiring discipline. Not a separate scene.**

Reasoning:

1. Teal'c had a full test-suite centerpiece in 1x08 (22 VERITAS). Giving him another full test scene in 1x09 would be repetitive. Instead, a single testing beat within the parser demonstration — Teal'c feeds malformed markup, the parser catches it, Teal'c approves. 15-20 seconds.

2. Save a Teal'c-centric testing scene for a later episode where the test logic is more dramatically interesting (1x10 with STML extensions, or 1x11 with networking).

---

## C. BEAT SHEET ORDER (Final)

| Beat | Content | Act | Source(s) |
|------|---------|-----|-----------|
| Cold Open | Pliny at his desk. Three flat files. Wax tablet marginalia. Stylus slips. "Arbor documentorum." | Pre-titles | A-plot |
| 1 | Pliny's presentation to the team. "The files should index themselves." Census data as flat text vs. tree. Carter: "The data describes itself. Or it should." | Act 1 | A-plot |
| 2 | Daniel's first markup. `<regio>`, `<familia>`, `<nomen>`. Latin tags. "The language should belong to the people who use it." O'Neill: "Who decides what nests inside what?" Marcus sees the stemma. Lucia: "This is not a foreign language describing Roman data. This is a Roman document." **"Nodus"** established as the technical term for individual nodes. | Act 1 | A-plot + Character + Historian |
| 3 | The parser. `stml_legere_ex_literis`. Error handling — Teal'c feeds malformed input. "The document corrects the author." **ACT BREAK.** | Act 1 | A-plot + Character (Teal'c) |
| 4 | Node types — the vocabulary of trees. Four types: document, element, text, comment. Marcus: "An element is a room." Carter: dependency chain on board (5 libraries from 5 episodes). | Act 2 | A-plot |
| 5 | Querying. Casey at the keyboard — his first code scene. `stml_invenire_liberum`, `stml_invenire_omnes_liberos`. Pliny: "That query would have taken me three hours." Casey: "We're not searching — we're navigating." | Act 2 | A-plot |
| 6 | Attributes. Daniel demonstrates. Boolean `verificatum`. Pliny: "A single word that certifies the observation. Like a seal on a document." Lucia reads the markup aloud: "A Roman citizen who has never seen a machine could read this." **ACT BREAK.** | Act 2 | A-plot |
| C1 | Carter's emotional-state experiment. XML-structured temporal logs. Frequency shifts with Midas's emotional state. "It's not just alive. It's connected to him." | (Act 2-3) | Threads |
| 7 | Tree navigation. First child, last child, siblings, parent, ancestors. Marcus: "A road network." Carter: `stml_maiores` — any node knows its ancestry. Pliny: "The observation knows where it lives." Data: "The context is not interpretation. It is structure." | Act 3 | A-plot |
| 8 | Building the encyclopedia. Pliny and Casey build together. `stml_elementum_creare`, `stml_liberum_addere`, `stml_textum_addere`. Pliny: "Not just me. When I am gone." The mortality beat. | Act 3 | A-plot + Character |
| C2 | Joe/Pliny structured chronicles working session. Joe dictates Watcher records. Pliny cross-references: "W-4419 was prolific — four subjects across Scotland." Joe: "I've been reading these records for thirty years. I never saw that connection." Pliny: "You didn't have a tree." | (Act 3) | Threads |
| 9 | Serialization — the round trip. Casey: `stml_scribere`. Pretty-printing. "The tree becomes text. The text can become a tree again." Marcus: "The serialized output is valid input." **ACT BREAK.** | Act 3 | A-plot |
| 10 | Comments — the builder's marks. Daniel: `<!-- text -->`. Pliny: "Scholia. Marginal notes." Comments persist inside the document. | Act 4 | A-plot |
| 11 | Full construction API. Prepend, insert before/after, count children, get child by index. Marcus: "This is not mere storage. This is architecture. You can remodel a document the way you remodel a building." | Act 4 | A-plot |
| Midas | Midas's 30-second drop-in. Persian royal archives. Clay tablet trees. Filing system survives regime change. Carter notices a detail. Midas wanders out. | (Act 4) | Character |
| 12 | Daniel's term — "Stirps." The COINAGE scene. Daniel starts with "nodus" (established in Beat 2) but Lucia pushes: "These are Latin words. They are not Latin *thinking*." Pliny: "My encyclopedia is a *stirps*. Volume is the *gens*. Chapter is the *familia*." Lucia: "You did not translate. You reminded him of something he already knew." Roman engineer uses "stirps" naturally. Daniel hears it. | Act 4 | Character + A-plot |
| 13 | The dependency chain. Carter writes the updated tree on the board. Five libraries flowing into one. Data: "STML is a tree that describes trees, built from libraries that form a tree. The recursion is not accidental." **ACT BREAK.** | Act 4 | A-plot |
| 3-Voice | Daniel / Pliny / Data. Pliny: tree as genealogy of knowledge. Data: tree as structure of memory + single parentage guarantee + "Measure of a Man" resonance. Daniel: tree as linguistic inheritance — grafting a new branch onto an ancient tree. | (Late Act 4) | Character + Genre |
| 14 | Pliny's encyclopedia — the real test. The first three entries parsed, queried, displayed. Boolean `verificatum`. "The document knows that obsidian is verified." Casey: "You asked for this in the cold open. The files should index themselves. They do now." | Act 5 | A-plot |
| 15 | Pliny sees his mind externalized. Character writer's revelation scene. "That is my book THE WAY I SEE IT. In my mind." Voice breaks. "No one has ever seen it. Until now." Casey: "Now it doesn't have to be in your head." Pliny: "The stirps survives the gardener." | Act 5 | Character (primary) + A-plot |
| 16 | The title moment. Pliny writes ARBOR DOCUMENTORUM on the board. Daniel: "We built a language. Not a translation." Roman engineers in the corridor using nodus, radix, stirps. | Act 5 | A-plot |
| 17 | Full serialization. Round trip: parse → query → serialize → save with Filum → read back → parse again. Casey: "Flat files are wax tablets. Structured documents are stone inscriptions." Pliny: "The arbor is stone." | Act 5 | A-plot |
| C3 | Daniel's STML investigation dossier. Cross-referencing Methos fragments, Watcher records, archaeological evidence. Pliny sees the methodology: "Three convergent sources for a single event. That is strong evidence." Brief — 60 seconds. Show the XML on screen briefly, not in detail. | (Act 4-5) | Threads |
| 18 | Tag: Pliny alone at the terminal. Evening. Entering his encyclopedia tag by tag. The new sky observation: "Constellationes ignotae. Caelum non est caelum Romae. Sed stellae fixae eaedem sunt. Quomodo?" A question preserved for anyone who comes after. | Tag | A-plot |
| 18b | Tag: Daniel/Lucia — the "colleague" scene. End of long work session. "You have improved... at listening." Lucia: "A colleague." She leaves. Daniel watches her go. | Tag | Character |
| C4 | Joe/Methos reunion. Joe's quarters. Late night. Wine. Adam Pierson's file still open. Alexandria cataloguer parable. The black stone reveal (brief, vague — the impulse, not the detail). "He'll find what I found. But he'll understand it better." Joe's journal: crosses out "Subject," writes "Methos." 120 seconds. | Tag | Character + Threads (compiled) |
| 19 | Tag: Casey/Carter forward. "The encyclopedia is going to grow." Carter wants to extend the language (fragments, raw content, capture operators). Casey: "One step at a time. The tree works." Seeds 1x10. | Tag | A-plot |
| 20 | Stinger: Kirk's TAXONOMIA. Wall-tree replaces flat lists. Aide reports on Seattle cell restructuring. "Three layers. No one below the second level knows anyone above it." Kirk: "If they capture the root — there is no root." Circles ANOMALOUS branch (MacLeod, Casey, Methos). 9/31. TAXONOMIA. "Everything organizes itself eventually. You just have to watch long enough." | Stinger | A-plot + Threads + Genre (compiled) |

---

## D. WHAT TO INCLUDE FROM EACH PITCH

### From the A-Plot Writer:
**KEEP:**
- The complete act structure and pacing (the backbone)
- ALL on-screen code moments (10 moments — essential)
- The cold open (Pliny's flat-file frustration, smeared wax)
- Pliny's presentation (Beat 1)
- Daniel's first markup (Beat 2)
- The parser with error handling (Beat 3)
- Node types (Beat 4)
- Querying — Casey's first code scene (Beat 5)
- Attributes and boolean attributes (Beat 6)
- Tree navigation and ancestors (Beat 7)
- Building the encyclopedia (Beat 8)
- Serialization and round trip (Beat 9)
- Comments (Beat 10)
- Full construction API (Beat 11)
- "Nodus" coinage — Roman engineer's adoption (fold into Beat 2 + Beat 12)
- Dependency chain (Beat 13)
- Pliny's encyclopedia — the real test (Beat 14)
- Title moment (Beat 16)
- Full serialization with Filum round trip (Beat 17)
- Tag: Pliny at terminal / new sky (Beat 18)
- Tag: Casey/Carter forward — seeding 1x10 (Beat 19)
- Kirk stinger framework (Beat 20)
- Feature-to-dramatic-moment map (for beat sheet reference)
- Dependency chain diagram

**CUT:**
- Deep clone scene (Beat 15 in A-plot) — a useful API feature but not dramatically necessary. Mention in thematic notes.

### From the Character Writer:
**KEEP:**
- Daniel's "stirps" coinage — the FULL scene with Pliny/Lucia (Beat 12)
- Pliny's revelation scene ("That is my book THE WAY I SEE IT") (Beat 15)
- "The stirps survives the gardener" — Pliny's mortality line
- Casey/Pliny faith scene — patronage parallel, adversaria, "six more questions" (full scene)
- Daniel/Lucia "colleague" scene (Tag)
- Daniel/Lucia stylus touch (brief, within Act 2-3)
- Three-voice Candidate A (Daniel/Pliny/Data — "The Tree of Ancestry")
- Joe/Methos reunion — core structure, black stone reveal, "we should probably talk"
- Ensemble beat table for direction reference
- Midas Persian archives comedy beat (30 seconds)
- Open question 1 resolution: arbor (title) vs. stirps (coinage) — dramatized as progression

**CUT:**
- Daniel/Lucia extended montage (replace with single building scene)
- Pliny working montage (compress)
- "Stirps" replacing "nodus" entirely — both terms survive

### From the Threads Writer:
**KEEP:**
- Casey faith escalation: patronage parallel + Salii comparison + "six/fourteen more questions" (compiled with character writer)
- Carter C-plot: emotional-state experiment (Beat C1)
- Joe/Pliny working session with XML (Beat C2 — "You didn't have a tree")
- Daniel's STML investigation dossier (Beat C3)
- Kirk stinger: compartmentalized network, "tree with no root," nominalist tree concept
- Joe/Methos: core dialogue, cataloguer parable, implicit authorization for Daniel's investigation
- Thread status appendix — include at end of beat sheet
- Thread intersection map
- Nominalism/realism: Lucia states thesis ("A tree that reflects the nature of what it contains is not a classification. It is a recognition.") — fold into a Daniel/Lucia scene
- All backward/forward connection notes

**CUT:**
- Kirk stinger word HIERARCHIA (replaced with TAXONOMIA)
- O'Neill XML dossier (defer to 1x10)
- Daniel/Lucia nominalism/realism as separate scene — fold into existing Daniel/Lucia work scenes
- Beat C4 length (trim from 150 to 120 seconds)

### From the Genre Fan:
**KEEP:**
- Three-voice: Data's single-parentage formal guarantee speech
- DOM tree as governance model (Marcus recognizing stemmata) — fold into Beat 2 and historian notes
- The Watcher Chronicles as DOM tree observation — Joe should recognize the structure (fold into C2)
- Franchise resonances: "The Chase" (common descent), "Darmok" (Daniel parallel), "The Fifth Race" (knowledge without structure), "Measure of a Man" (Data's tree-voice)
- Methos's lost tree (most branches pruned) — if Data has access to Watcher data, seed this
- Kirk's wall-tree replacing flat lists (visual escalation)
- "The Borg overwrite. The tree preserves structure." (thematic note, not dialogue — already used similar in 1x07)
- Serialization as "Measure of a Man" parallel (thematic notes)
- Code-to-genre map (for series bible)

**CUT:**
- Kirk stinger word ARBOR (replaced with TAXONOMIA)
- The full franchise-by-franchise breakdown — for series bible, not beat sheet
- Capture operators analysis (saved for 1x10)
- Extended nominalism/realism philosophical analysis — for thematic notes, not scenes
- The B5/Grey Council parallel — too many franchise references in one episode

### From the Historian:
**KEEP:**
- Pliny's Naturalis Historia structure (37 books, the meta-document of Book I) — brief reference in Beat 1
- Roman stemmata: the ius imaginum, painted family trees on atrium walls. Marcus recognizes STML as stemma — in Beat 2
- The Roman Tabularium's hierarchical classification (regio → tribus → familia → civis) — flat census from 1x04 needs hierarchy
- The scroll-to-codex transition: flat → structured. The codex (from caudex, tree trunk) — historical resonance note
- The Tree of Porphyry: classification as tree, realist artifact — Daniel would know this (brief reference)
- Latin vocabulary audit: arbor, radix, ramus, folium, stirps, stemma, nodus, codex — all verified
- The beautiful accident of *codex* coming from *caudex* (tree trunk) — possible Daniel or Lucia line
- Character knowledge map — Marcus sees stemmata, Pliny sees his encyclopedia, Daniel sees Aristotle through Porphyry, Casey sees Jesse Tree + IBM IMS, Data sees mathematical universality
- Production design: stemmata on Roman house walls (visual rhyme with DOM tree on screen)
- Pliny's compilation method as parse-and-build pipeline — the first compiler
- *Nodus Herculeus* (marriage knot) and *nodus* in rhetoric (plot complication) — for thematic notes

**CUT:**
- Medieval sections (Isidore, Jesse Tree, manuscript stemma) — Daniel/Casey may know these but they're too deep for the episode. For series bible.
- Full timeline check — reference briefly in continuity notes
- Extended production design notes — for production bible, not beat sheet

---

## E. ELEMENTS TO DEFER

### Defer to Later Episodes:
- **O'Neill's XML dossier** — 1x10. O'Neill learning from the A-plot and applying XML to intelligence work.
- **Daniel/Lucia escalation beyond "colleague"** — 1x11-12. The romantic tension simmers.
- **STML extensions** (fragments, raw content, capture operators) — 1x10, explicitly seeded in Beat 19.
- **Methos's full Alexandria story** — Season 1 back half. The stone, the details, the connection to Midas.
- **Data examining Watcher database structure** — 1x10-12. "The tree is incomplete. The root exists. Many branches are absent."
- **Deep clone as dramatic device** — 1x10. Variant editions, manuscript traditions.

### Defer to Series Bible / Thematic Notes:
- The full franchise-by-franchise analysis (genre fan) — series bible
- Medieval tree traditions (Isidore, Jesse Tree, manuscript stemma) (historian) — series bible
- The scroll-to-codex civilizational parallel table (historian) — thematic notes
- The DOM tree as governance model — deep analysis (genre fan) — series bible
- Capture operators as jurisdiction (genre fan) — series bible (seeds 1x10)
- The full code-to-genre map (genre fan) — series bible
- Extended nominalism/realism philosophical framework (genre fan + threads) — thematic notes
- Immortal lineage trees as DOM trees (genre fan) — series bible

---

## F. OPEN QUESTIONS FOR REVIEW

### 1. Joe/Methos Placement — Tag or Separate C-Plot Thread?

I've placed the Joe/Methos reunion as C4 in the Tag area because the scene needs evening/night, wine, and privacy. This makes the Tag section heavy (four beats: Pliny terminal + Daniel/Lucia + Joe/Methos + Casey/Carter). The alternative: move Joe/Methos to Act 3-4 as a C-plot intercut, which distributes the weight better but risks interrupting the A-plot's momentum.

**My recommendation:** Keep it in the Tag. The Tag CAN be heavy because the A-plot resolves in Act 5. After the title moment and full serialization, the audience is ready for character scenes. The four Tag beats descend in scale: Pliny alone (intimate) → Daniel/Lucia (personal) → Joe/Methos (the big C-plot payoff) → Casey/Carter (forward seed). This is a closing cascade, not a pileup.

### 2. Pliny's Revelation vs. Title Moment — Ordering

I've placed the character writer's revelation scene (Beat 15 — "That is my book THE WAY I SEE IT") BEFORE the A-plot's title moment (Beat 16 — Pliny writes ARBOR DOCUMENTORUM on the board). The alternative: reverse them, with the title moment as the climax and the revelation as the emotional coda.

**My recommendation:** Revelation THEN title. The emotional payload (Beat 15) is stronger as the CLIMAX. The title moment (Beat 16) is the resolution — Pliny naming what he's just experienced. The shape is: demonstration (Beat 14) → emotional devastation (Beat 15) → naming (Beat 16) → technical capstone (Beat 17). This follows the 1x08 pattern: Casey's acceptance (Beat 13) → Marcus names the horologium (Beat 18). The naming follows the feeling.

### 3. How Much On-Screen Code for Daniel's Investigation?

The threads writer proposes Daniel's investigation XML on screen in detail. The A-plot already has 10 code moments. Adding Daniel's investigation XML risks code overload.

**My recommendation:** Show Daniel's investigation XML BRIEFLY — a 5-second screen glimpse, not a full code reading. The audience should see that Daniel is using the A-plot's tool for C-plot investigation. The details of his dossier can be dialogue with Pliny ("Three convergent sources for a single event") rather than on-screen code. This respects the code budget while advancing the thread.

### 4. Salii Comparison — Include or Cut?

The threads writer's Salii comparison (Pliny comparing the rosary to the Salii priests' ritual dance) is historically grounded and tonally right. But it extends the faith scene from 90 to 120 seconds and adds a humor beat that may break the scene's scholarly tone.

**My recommendation:** INCLUDE. The humor beat ("Perhaps it should [involve dancing]") is the first time Pliny has joked about religion. That's an escalation. And Pliny comparing devotional practices across cultures IS Pliny — he compares, he catalogs, he finds parallels. The Salii comparison comes AFTER the patronage parallel (the main beat), as a lighter coda. The scene shape: serious (patronage) → lighter (Salii) → closing (six more questions). The humor doesn't break the tone. It ventilates it.

### 5. Carter's Emotional-State Experiment — 1x09 or 1x10?

The threads writer asks whether the emotional-state correlation should be in 1x09 or deferred to 1x10. Risk: too many revelations. Benefit: XML tools make the structured observation possible for the first time.

**My recommendation:** 1x09. The revelation is tiny (fourth decimal place). The C-plot beat is 45 seconds. And the meta-level storytelling is too good to defer: Carter uses the A-plot's tool (STML) for her private investigation. Every episode's C-plot should demonstrate the A-plot's utility in a different context. In 1x08, Casey's wave functions powered Carter's frequency analysis. In 1x09, STML structures Carter's temporal data logs. The pattern is established. Don't break it.

---

*Compilation plan complete. Ready for review.*
