# NOVUM FUNDAMENTUM
## Episode 1x05: "Tabularium Novum" (The New Archive)
### Historian Pitch (Draft 1)

---

## WHY THIS EPISODE MATTERS HISTORICALLY

This is the episode where the show earns its claim that Nova Roma is a *real* civilization, not a costume drama. Hash tables and hash functions are the technological backbone of modern information retrieval, but the *problem* they solve — "I have ten thousand things; how do I find the one I need?" — is the defining challenge of every literate civilization since Sumer. The Romans had their own answers. They were brilliant. And they were insufficient.

Pliny the Elder, the character who spotlights this episode, spent his entire life trying to solve this problem with brute force and human memory. His *Naturalis Historia* — 37 books, approximately 2,500 chapters, 20,000 facts cited from 473 authors — was the largest single-authored work to survive from antiquity. Book I is nothing but a table of contents and source index for the other 36. He invented the encyclopedia format. He *was* a hash table, running on wine and determination, and he died because he couldn't stop trying to record and rescue what he knew.

This episode should feel like watching a man who has been searching for something his whole life — across eighty years of obsessive documentation — finally find it.

---

## PART 1: THE REAL TABULARIUM — WHAT THE AUDIENCE SHOULD SEE

### The Building

The Tabularium was built in 78 BC by Quintus Lutatius Catulus on the southeast slope of the Capitoline Hill, directly overlooking the Roman Forum. It was constructed following the catastrophic fire of 83 BC that ravaged the Capitoline during Sulla's civil wars, destroying the Temple of Jupiter Optimus Maximus and, crucially, centuries of state records stored in nearby temples.

Key physical details for production design:
- **The great corridor**: 67 meters (220 feet) long, raised 15 meters above the forum on a massive substructure. Eleven large arches along the facade. This was *monumental* architecture dedicated to *paperwork*. Rome built a palace for its filing cabinets.
- **The arcade level**: Where the actual archives were kept. Rooms opening off the central corridor, each dedicated to a category of record.
- **The view**: From the Tabularium's windows, you could see the entire Forum below. The archive literally *overlooked* the life it documented. This is a visual metaphor the show should use — Pliny looking down from the archive at the people whose names fill the records.

### What Was Stored There

The Tabularium held the *tabulae publicae* — public records inscribed on bronze tablets:
- **Leges**: Laws passed by the assemblies
- **Senatus consulta**: Decrees of the Senate
- **Foedera**: Treaties with foreign nations
- **Tabulae censoriae**: Census records (direct connection to 1x04)
- **Financial records**: State accounts, tax records, public contracts

The name itself comes from *tabula* (tablet, board, writing surface) — the same root as our "tabula dispersa." This is not coincidence. The writers should lean into it. When Pliny says "Tabularium Novum," he is explicitly saying: "We are building the new version of the building I remember."

### How Romans Actually Filed Things

This is where the historical reality gets dramatically useful:

**Scrinia**: Cylindrical containers (think large scroll cases) for transporting and storing groups of papyrus rolls. Organized by topic. A scrinium of laws, a scrinium of treaties, a scrinium of financial records. Each scrinium essentially a *bucket* — a container that holds related documents.

**Armarium**: A cabinet with individual compartments — *loculi* (little places) or *nidi* (nests) — each holding a specific scroll or group of scrolls. The armarium is the closest Roman equivalent to an array of slots. Each compartment has a position. You know where to look because you know the system.

**Pinax/Titulus**: A small tag (usually parchment or wood) attached to the protruding end of a scroll, identifying its contents. When scrolls were stored in a scrinium or armarium, the pinax stuck out so you could read it without unrolling the scroll. This is *metadata*. The pinax is the key; the scroll is the value. The Romans had key-value storage.

**The problem**: The pinax only works if you know which armarium to look in. And the armarium only works if you remember which scrinium it's in. And the scrinium only works if someone maintained the system. This is *linear search* with human memory as the index. It's O(n) at best, O(despair) when the old librarian dies and nobody remembers where things were put.

**Dramatic use**: Pliny should describe this system to Carter with growing frustration. "I know where everything is. I have always known. But when I am gone, who will know? The system lives in my head. That is not a system. That is a hostage situation."

---

## PART 2: PLINY THE ELDER — THE MAN WHO TRIED TO INDEX THE WORLD

### Historical Pliny

Gaius Plinius Secundus (23-79 AD). Career trajectory: equestrian military officer in Germany, procurator in several provinces, then admiral of the Roman fleet at Misenum. But his real life's work was the *Naturalis Historia*.

### His Work Habits (Source: Pliny the Younger, *Epistulae* III.5)

His nephew's letter to Baebius Macer is one of the most detailed portraits of a working intellectual from antiquity. Key details for characterization:

- **He never stopped working.** The only break was his bath, and even during his bath he had a book read to him or dictated notes.
- **He had a reader and a secretary at all times.** One servant read aloud; another wrote down extracts as Pliny dictated. He traveled with them. He ate with them working beside him.
- **Winter precautions**: He gave his secretary long sleeves and gloves so the man's writing hand wouldn't stiffen in cold weather. He wasn't doing this from kindness — he was doing it to avoid lost productivity. (Though knowing Pliny, it was probably both.)
- **His reading method**: While traveling, he read constantly. He once reproached a friend who had interrupted a reader: "Surely you understood what was being read? We've lost time." He counted wasted minutes.
- **His index system**: Book I of *Naturalis Historia* is a complete table of contents and source list for all 36 subsequent books. Each book begins with its own table of contents. This was revolutionary — no surviving ancient work before Pliny had this structure. He built the index because he needed to *find things in his own work.*
- **The famous quote**: *"Nullum esse librum tam malum, ut non in aliqua parte prodesset"* — "There is no book so bad that some good cannot be got out of it." This is the mind of a man who has decided that ALL knowledge has value and therefore ALL knowledge must be preserved and therefore ALL knowledge must be findable.

### His Death (Source: Pliny the Younger, *Epistulae* VI.16 and VI.20)

On August 24 (or October 24, per recent scholarship), 79 AD, Pliny the Elder was commanding the Roman fleet at Misenum when Vesuvius erupted. His nephew watched the initial eruption from across the Bay of Naples.

What happened:
1. Pliny saw the eruption column. His first instinct was **to observe and record it** — he ordered a light vessel prepared.
2. He received a message from Rectina, wife of Cascus, begging for rescue. She was trapped at the foot of the mountain.
3. He changed course. Launched the full fleet. Sailed *toward* the volcano.
4. At Stabiae, he found his friend Pomponianus with ships already loaded but unable to sail because the wind was blowing inshore. Pliny reassured him, bathed, dined, and went to bed — or pretended to, because his nephew notes his loud snoring could be heard in the corridor.
5. By morning, the courtyard was filling with pumice. The building shook. They decided to flee to the shore. Pliny, who was corpulent and suffered from respiratory problems, could not keep up.
6. He sat down on a sailcloth spread on the ground. Asked for cold water. Twice. The sulphurous fumes overwhelmed him. When dawn returned the next day, they found his body, "looking more like a sleeper than a dead man."

**Dramatic use**: In the show, Pliny is already dead and resurrected on Nova Roma (like all the historical figures). But his memory of dying at Vesuvius should be the ghost beneath the surface of this episode. Pliny watched his world — the libraries, the records, the scrinia full of scrolls — buried under ash. He felt the physical infrastructure of knowledge destroyed. And now he's being offered a system where knowledge can't be lost to fire, flood, or the death of the one person who knew where things were.

The moment when Pliny understands hashing should carry the weight of a man who has been drowning for eighty years and someone has just thrown him a rope.

---

## PART 3: THE FRIATIO METAPHOR — GRINDING AS HASHING

### The Latin

*Friare* is genuine classical Latin: "to rub away, crumble, break into small pieces." It gives us English *friable* (easily crumbled) and, through the diminutive *frivolus* (broken, crumbled, worthless), *frivolous*. The related verb *fricare* (to rub) gives us *friction*.

The choice of *friatio* for "hash function" is linguistically brilliant and dramatically rich, because the Romans had three major industrial applications of grinding, all of which Pliny would know intimately:

### The Millstone (Mola)

The *mola* — the grain mill — was ubiquitous in Roman life. The large commercial *mola asinaria* (donkey mill) consisted of two stones: the fixed *meta* (cone-shaped base) and the rotating *catillus* (hopper stone) that was turned by animal or slave labor. Wheat went in the top; flour came out the bottom. The grain was reduced to its essence. The individual kernel became indistinguishable — you could not reconstruct the kernel from the flour. **This is a one-way function.** Pliny documented grain mills in *Naturalis Historia* Book XVIII.

**Scene possibility**: Pliny takes Carter (and the audience) to a working grain mill. "Watch. The wheat goes in — each kernel unique, each with its own weight, its own shape. It comes out as flour. Every kernel different; every flour the same white powder. You cannot get the kernel back from the flour. But —" (he scoops flour from two different batches) "— if I grind the same wheat twice, I get the same flour."

This is the hash function explained in 30 seconds using Roman technology.

### The Olive Press (Trapetum)

The *trapetum* was a sophisticated machine: a large stone basin (*mortarium*) with hemispherical stone rollers (*orbes*) mounted on a horizontal axle. The genius of the trapetum was that it crushed the olive flesh to release the oil WITHOUT crushing the pit — the pit's bitterness would ruin the oil. The rollers were set at a precise gap to crush the flesh but pass over the stone.

**The connection**: A hash function that preserves the essential character while reducing the input. The olive becomes oil — a transformation that captures what matters (the oil) and discards what doesn't (the flesh). But unlike the millstone, the trapetum is *selective*. It crushes to extract, not to destroy. This is a more sophisticated hash — FNV-1a doesn't just smash the input; the XOR-and-multiply pattern extracts and combines the essential bits of each byte.

### The Mortar and Pestle (Mortarium et Pistillum)

Used for pigments (for painting), medicines (Pliny documents hundreds of pharmacological preparations), and cooking spices. The operator has direct control over the coarseness of the grind. A rough grind for cooking; a fine grind for painting; a powder for medicine.

**The connection**: Different hash functions for different purposes. FNV-1a is the coarse kitchen grind — fast, good enough for everyday use. SHA-256 is the pharmacist's powder — precise, thorough, reliable. CRC32 is the painter's pigment — not for consumption, just for checking that what you have matches what you expected.

### Dialogue Seed

**PLINY:** In my encyclopedia I documented forty-seven varieties of grain, twelve methods of milling, and nine grades of flour produced by the finest mills of Campania. I never imagined that the principle of the millstone would become the principle of the archive.

**CARTER:** The principle is the same. Reduction. You take something complex — a name, a word, a document — and you reduce it to something simple. A number. A position.

**PLINY:** And if two names produce the same number?

**CARTER:** Then you have a collision.

**PLINY:** *(immediately)* Like two olives in the same channel of the trapetum.

**CARTER:** ...Exactly like that.

Pliny gets there fast because he has spent eighty years thinking about how to organize things. He doesn't need the computer science terminology. He has the *concepts* already, wearing Roman clothes.

---

## PART 4: CALLIMACHUS AND THE PINAKES — THE ANCESTOR OF THE HASH TABLE

### The Historical Record

Callimachus of Cyrene (c. 310-240 BC) was a scholar at the Library of Alexandria who created the *Pinakes* (*Tablets*) — a bibliographic survey of the Library's holdings that filled 120 books. Its full title was *Pinakes of Those Who Were Outstanding in Every Branch of Learning, and Their Writings.*

The organizational structure:
1. **Primary division**: Poetry vs. Prose
2. **Secondary division**: Genre (epic, lyric, tragic, comic, etc. for poetry; history, oratory, philosophy, medicine, law, etc. for prose)
3. **Within each genre**: Authors listed alphabetically
4. **Each author entry**: Name, father's name, birthplace, brief biography, nicknames, complete list of works
5. **Each work**: Title, first line (the *incipit* — because many works had similar titles), and number of lines

This is a multi-level lookup system. Genre narrows the search space. Alphabetical order within genre narrows further. The incipit distinguishes works with identical titles. **This is a hash function with collision resolution** — the genre is the primary hash, alphabetical order is the probe sequence, and the incipit is the collision-resolution key.

### The Methos Connection

Methos is 5,000 years old. The Library of Alexandria flourished from roughly 300 BC to at least 48 BC (Caesar's fire) and arguably survived in diminished form for centuries after. Methos was there. The season outline (1x06) establishes that Methos was at Alexandria and that his revelation to Daniel about this is a major plot point.

But this is 1x05, one episode before that revelation. The seeds should be planted here:

**Scene possibility**: Daniel is explaining Callimachus's system to the group, using it as a historical precedent for what they're building. Methos is in the room. At the mention of the Pinakes, Methos's expression flickers — barely perceptible, the way a man looks when you mention a place he's been but hasn't told you about.

**DANIEL:** Callimachus organized the Library of Alexandria using a system of categories and sub-categories. Genre, then author, then work. He called it the *Pinakes* — the tablets. One hundred and twenty books just cataloging what was in the library. It was the first —

**METHOS:** *(cutting in, then catching himself)* ...The first systematic bibliography. Yes. I've read about it.

"Read about it." The audience who knows Methos's secret catches the lie. Daniel, who doesn't know yet, files the interruption. One episode later, the truth comes out.

### Why This Matters for the A-Plot

The Pinakes demonstrates that the problem Pliny is facing — "how do I find what I need in a vast collection?" — has been attacked by the greatest minds of the ancient world. Callimachus's solution was hierarchical categorization: narrow the search space by genre, then alphabetize, then verify. It worked. It served Alexandria for centuries.

But it was fragile. It depended on correct categorization (what if a work spans genres?). It depended on alphabetical ordering (what if you know the subject but not the author?). It depended on the physical organization of the library matching the catalog (what if scrolls were reshelved incorrectly?).

The hash table solves all of these problems. Any key. Constant-time lookup. No dependence on physical ordering. The *Pinakes* is the ancestor; the *tabula dispersa* is the descendant that finally gets it right.

---

## PART 5: ROMAN FILING SYSTEMS — VISUAL VOCABULARY

### What Should Appear On Screen

The production designers need to understand what a working Roman archive *looks like*. This is not the Hollywood version (scrolls scattered artistically on a desk). This is institutional infrastructure:

**Scrinia (scroll containers)**:
- Cylindrical buckets, usually wood or leather, roughly 30-40 cm tall
- Each contains 6-10 scrolls standing upright
- The *pinax* tags visible protruding from the top of each scroll
- Organized on shelves, in rows, often color-coded or marked with painted symbols
- Think of them as hash buckets. Each scrinium is a slot in the hash table. The scrolls inside are the entries. The pinax is the key metadata.

**Armarium (scroll cabinet)**:
- A large wooden cabinet with individual compartments (*loculi*)
- Each compartment typically held one scroll or a small group
- Diamond-shaped or square pigeonholes arranged in a grid
- Wall-mounted, floor-to-ceiling in important rooms
- The armarium at its best is a fixed-size array — each position known, each scroll retrievable by position

**Tabulae ceratae (wax tablets)**:
- Wooden frames with recessed wax surfaces
- Written with a bronze or iron *stilus* (stylus)
- Erasable — smooth the wax to rewrite. These are the temporary records: drafts, notes, calculations
- Often hinged in pairs (*diptychon*) or triples (*triptychon*)
- For legal documents, sealed with wax and thread, with witnesses' seals pressed into the wax

**Volumina (scrolls)**:
- Papyrus rolls, typically 6-10 meters long, wound around a wooden rod (*umbilicus*)
- Text written in columns (*paginae*), typically 25-30 lines per column
- The *titulus* (title) or *index* written on the back of the roll and on the attached *pinax* tag
- Reading required unrolling with the right hand and re-rolling with the left — this is *sequential access*

### The Visual Metaphor

The episode should establish Pliny's existing archive — scrinia upon scrinia, armaria along every wall, a lifetime of knowledge stored in a system that only Pliny fully understands — and then show the hash table as the solution to its fundamental limitation.

**PLINY:** *(gesturing at his archive)* Every scroll has a place. I know every place. But this knowledge — *(tapping his temple)* — this is the index. When I die, the archive becomes a labyrinth without a map.

**CARTER:** What if the map was built into the archive itself?

This is the dramatic pivot. Roman filing systems depend on external knowledge (the librarian's memory, the catalog, the organizational system maintained by human discipline). The hash table builds the navigation into the data structure itself. You don't need to know where Pliny filed it. You need the key, and the table tells you where to look.

---

## PART 6: THE CENSUS-TO-ARCHIVE PIPELINE — PROTO-HASHING IN ROMAN BUREAUCRACY

### How the Census Actually Worked

Roman censors (two, elected every five years, serving eighteen months) conducted the census — the *lustrum* — in the Campus Martius. Every Roman citizen was required to appear in person and declare:

1. **Full name** (*praenomen, nomen, cognomen*)
2. **Father's name** (or patron's, if freed)
3. **Tribe** (*tribus*) — one of 35 territorial divisions, hereditary even if you moved
4. **Age**
5. **Property and wealth** — declared under oath
6. **Wife and children**
7. **Military status**

Based on this declaration, the censors assigned each citizen to:
- A **tribus** (tribe) — 35 total, 4 urban + 31 rural
- A **classis** (class) — 5 property classes plus the *proletarii* (those with no property)
- A **centuria** (century) — a voting and military unit within the class

### The Proto-Hash Function

The tribus + classis assignment IS a hash function. Consider:

**Input**: A citizen (complex, unique, irreducible individual)
**Function**: Apply the tribus-classis mapping based on residence and property
**Output**: A position in a known, finite set of categories (35 tribes x 6 classes = 210 possible categories)

To find a citizen in the census records, you would:
1. Know their tribus (narrows to 1/35 of all citizens)
2. Know their classis (narrows further by wealth bracket)
3. Search within that category

This is **not** constant-time lookup — it's still linear search within the bucket. But the bucket-narrowing principle is identical to the hash table. The tribus-classis mapping is the hash function. The storage within each category is the collision chain.

**Dramatic use**: Marcus, the census engineer from 1x04, should recognize this. He's the one who built the Xar census. He understands that the census already has a classification system. The hash table formalizes what the Romans were already doing intuitively.

**MARCUS:** We already group citizens by tribus and classis. If I want to find a man, I find his tribe first, then his class, then I search within the class.

**CARTER:** That's exactly what the hash table does. Except instead of tribe and class, we use a number calculated from the name itself.

**MARCUS:** *(thinking)* So the name tells you where to look. The name IS the address.

**CARTER:** Now you understand friatio.

### The Tabulae Censoriae

The actual census records — *tabulae censoriae* — were kept in the Tabularium. They were among the most important documents in the Roman state, because they determined:
- **Taxation**: Your classis determined your tax rate
- **Military obligation**: Your classis determined your military role
- **Voting weight**: Your centuria determined your vote in the centuriate assembly — and the wealthy classes voted first

The census was not just counting. It was **classification**, **assignment**, and **retrieval** — the three operations of a hash table: insert (register a citizen), store in the correct slot (assign tribus and classis), and retrieve (find the citizen by their classification).

---

## PART 7: LATIN TERMINOLOGY — WHAT'S REAL AND WHAT'S INVENTED

The characters should use Latin naturally. Here's the linguistic audit:

### Genuine Classical Latin
- **Tabula**: Board, writing tablet, table. Universally attested. The root of "Tabularium."
- **Friare**: To crumble, rub away. Attested in classical sources. Parent of *friabilis*.
- **Friatio**: The act of crumbling. Grammatically correct nominal form of *friare*. May not be attested in surviving texts, but the formation is perfectly regular (-tio suffix on verb stem, cf. *oratio* from *orare*, *ratio* from *reri*).
- **Slotus**: Not classical Latin. Invented for the codebase. But *follows* Latin second-declension masculine pattern. Romans would hear it as a loanword and accept it, the way they accepted Greek technical terms.
- **Collisio**: Classical Latin — "a striking together, collision." From *collidere*. Attested in Seneca and later authors. Perfectly correct for hash collisions.
- **Dispersa**: Classical Latin — feminine past participle of *dispergere* (to scatter, spread). "Tabula dispersa" = "scattered table" — a table whose entries are dispersed by the hash function. Grammatically impeccable.
- **Capacitas**: Classical Latin — "capacity, spaciousness." Attested in Pliny himself (*Naturalis Historia*).
- **Clavis**: Classical Latin — "key." Both literal (a physical key) and metaphorical. Used for lookup keys: perfect.
- **Valor**: Late Latin / Medieval Latin for "value, worth." Not classical (classical would be *pretium* or *aestimatio*). But the show is building a technical vocabulary, and *valor* is transparent to any Latin reader.
- **Positus**: Classical Latin — past participle of *ponere* (to place). "Position." Impeccable.
- **Distantia**: Classical Latin — "distance, remoteness." Used in the code for probe distance. Attested in Seneca.
- **Numerus**: Classical Latin — "number." Need I say more.
- **Factor**: Late Latin — "maker, doer." Used in the code for load factor. Classical Latin would use *ratio* (proportion). Either works.

### Latin the Characters Should Use in Dialogue
- **"Friare est reducere"** — "To hash is to reduce." The one-sentence explanation.
- **"Clavis dat locum"** — "The key gives the place." How a hash table works.
- **"Duo in unum locum"** — "Two into one place." What a collision is.
- **"Tabula crescit sed non migrat"** — "The table grows but does not migrate." How resize works with the piscina arena (old slots stay; new slots are added).

### A Pliny-Specific Moment
Pliny should name things. He is the great taxonomist. When he understands the hash table, he should give it its proper Latin classification the way he classified animals, minerals, and medicines:

**PLINY:** *(writing in his notebook)* *Tabula dispersa: instrumentum ordinandi per friationes. Genus: tabulae. Species: dispersa. Clavis ingredit; friatio dat locum; valor invenitur. Collisiones solvuntur per progressum linearem.*

Translation: "Dispersed table: an instrument of ordering through hash functions. Genus: tables. Species: dispersed. The key enters; the hash gives the place; the value is found. Collisions are resolved by linear progression."

He's writing an encyclopedia entry. He can't help himself. He has been writing encyclopedia entries for eighty years.

---

## PART 8: VESUVIUS — THE FIRE BENEATH THE SURFACE

### Should Vesuvius Appear?

Yes. But not as a flashback. Not yet. Vesuvius should be present as *absence* — as the thing Pliny doesn't talk about but which drives everything he does.

Consider: Pliny died at Stabiae trying to rescue people from the eruption. But he also died trying to *observe* it. His nephew's letters to Tacitus make clear that Pliny's first instinct was scientific: he saw the eruption column, wanted to examine it more closely, and ordered a boat. Only when the rescue request came from Rectina did the mission change from observation to salvation.

On Nova Roma, Pliny has been given a second life. He has spent it doing the same thing he always did: documenting, recording, organizing. But the urgency is different. He has *seen* what happens when records are destroyed. Not abstractly — he felt the pumice fall. He breathed the sulphur. He died because the world couldn't be saved by documentation alone, but he died *documenting*.

### How Vesuvius Should Manifest

**Not as dialogue**: Pliny doesn't say "I watched everything burn." He's Roman. He's stoic. He processes grief through work, not words.

**As behavior**: When Pliny sees the hash table working — when he watches Carter demonstrate that a record can be found instantly, that the table grows without losing anything, that the *system* knows where things are without depending on one man's memory — his reaction should be disproportionate. Not joy. *Relief*. The kind of relief that comes with a sob you don't quite release. The audience should feel that something deeper than a data structure has just been understood.

**As one moment**: Late in the episode. Pliny alone in his archive. He's staring at the scrinia. His hand rests on one of them — old, leather, cracked. The camera holds.

**PLINY:** *(to himself, barely audible)* I could not save the scrolls. *(beat)* But I can make sure they are never lost this way again.

That's the only direct reference. It's enough. The audience knows the history. The show trusts them.

### Connection to His Nephew's Account

Pliny the Younger wrote to Tacitus that his uncle's body was found "looking more like a sleeper than a dead man." There's something heartbreaking about this. The man who never stopped working, found at rest. If the show ever does a Vesuvius flashback (perhaps Season 2), this image should be the last thing we see. But for 1x05, the *weight* of that image should be felt without showing it.

---

## PART 9: WHAT A FIRST-CENTURY POLYMATH WOULD THINK ABOUT HASH FUNCTIONS

### He Would Understand Immediately

Pliny was not a philosopher in the Greek sense. He was a *compiler* — a man who organized other people's knowledge into usable form. His entire intellectual framework was:

1. **Collect** everything
2. **Organize** it by subject
3. **Index** it so anyone can find anything
4. **Verify** by cross-referencing sources

He already thought in keys and values. His index in Book I maps topics to locations in the text. His source lists map claims to authorities. He understood that *finding* knowledge is as important as *having* it.

### What Would Surprise Him

The one-way nature of the hash function. In Pliny's world, you can always go from the classification back to the thing classified. If you know something is in Book XIV (wine), you can find it. And from the entry, you can identify the subject.

But *friatio* goes one direction. You can get from "Gaius Plinius Secundus" to position 47, but you cannot get from position 47 back to the name without looking. The information has been *reduced* — like grain to flour. This would fascinate him. He would understand it through the mola metaphor, and he would immediately ask: "What else can you reduce?"

### What Would Move Him

That the system doesn't forget. The hash table with arena allocation (piscina) never frees memory. When the table grows, old entries stay; new space is added. This echoes Pliny's entire philosophy of knowledge: nothing should be discarded. "No book so bad that some good cannot be got out of it." The hash table, like Pliny's encyclopedia, accumulates. It does not curate by destruction.

---

## PART 10: SPECIFIC SCENE PROPOSALS AND HISTORICAL GROUNDING

### Scene: Pliny Gives a Tour of His Archive (Early Act 1)

Before the hash table is introduced, show Pliny's system. The audience needs to see what they're replacing.

Pliny leads Carter through his archive room. Walls of armaria. Stacks of scrinia. Wax tablets in organized piles. Everything has a place — but the place is in Pliny's head.

**PLINY:** *(moving through the room with practiced ease)* Geography is on the east wall. Natural philosophy, west. Medicine, south — organized by ailment, then by remedy, then by source authority. *(pulling a scroll)* See — this is my chapter on maritime creatures. I have documented one hundred and seventy-six species. Each scroll has its tag. Each tag is in my hand.

**CARTER:** What happens when someone else needs to find a scroll?

**PLINY:** *(simply)* They ask me.

**CARTER:** And if you're not here?

Long pause. Pliny puts the scroll back. Very carefully.

**PLINY:** Then it takes longer.

### Scene: The Mola Demonstration (Mid Act 2)

Pliny takes the team to a working grain mill outside the forum. Real Roman technology, on screen. The mola turns. Grain goes in. Flour comes out.

This should be shot as a demonstration, not a metaphor-delivery-device. Let the audience see the technology working. Let them feel the weight of the stones, the rhythm of the grinding, the transformation of material. Then let Pliny make the connection.

Historical accuracy: The mola in Nova Roma should be a *mola asinaria* (donkey-powered mill) or a *mola manuaria* (hand-turned mill), not a waterwheel mill — waterwheels are attested in the Roman world but are a later development and might feel anachronistic for a colony.

### Scene: Pliny's Encyclopedia Entry (Late Act 3 or Tag)

After the hash table is working, Pliny sits at his desk. He opens a fresh wax tablet. He writes an entry for *Naturalis Historia Nova* — his updated encyclopedia for the new world:

*"De Tabulis Dispersis"* — "On Dispersed Tables"

He writes in the same format he used for the original *Naturalis Historia*: subject, definition, properties, notable examples, sources. He is filing the hash table alongside maritime creatures and Campanian flour. It's knowledge. It goes in the encyclopedia.

The camera should linger on his handwriting. Then pull back to show the room — the old archive behind him, the new system on the screen in front of him. The past and the future in one frame, with Pliny at the intersection.

### Scene: The Compass Anomaly (C-Plot)

When the compass spins near Midas (the existing C-plot beat), the historical grounding matters: Roman compasses existed as wind-direction indicators, and the concept of magnetic north was known through lodestone. A Roman watching a compass spin would understand that something is *pulling* the needle — not magic, but a force. Pliny documented lodestones in *Naturalis Historia* Book XXXVI. If Pliny witnesses the compass anomaly, he would reach for his notebook, not for a prayer. He would want to *record* it.

---

## PART 11: PRODUCTION DESIGN NOTES

### Pliny's Archive Room
- Modeled after what we know of the real Tabularium's interior rooms: plastered walls, barrel-vaulted ceiling, good natural light from high windows
- Armaria on three walls (wood, with individual compartments visible)
- Scrinia on the remaining wall and on tables (cylindrical containers, some leather, some wooden)
- A large work table (Roman *mensa*) in the center, covered with wax tablets and loose scrolls
- An *abacus* (Roman counting board) on the table — Pliny uses it for calculations
- A reading stand (*pluteus*) with a scroll currently unrolled — he was in the middle of something when we arrived
- Oil lamps (*lucernae*) — good ones, bronze, multiple wicks. Pliny works at night.

### The Grain Mill
- Located outside the city walls, near the agricultural district
- A *mola asinaria*: hourglass-shaped stone mill turned by a donkey walking in a circle
- The *meta* (fixed conical base) and *catillus* (rotating hopper) visible as two distinct stones
- Grain funnel at the top, flour channel at the bottom
- The sound should be emphasized: the grinding of stone on stone, rhythmic, heavy, ancient

### Pliny's Writing
- He writes on wax tablets for notes (erasable, quick)
- He writes on papyrus for permanent records (the encyclopedia entries)
- His handwriting should be small, dense, and incredibly neat — he fits more words per line than anyone else, because he has more to say

---

## SUMMARY: THE HISTORIAN'S FIVE KEY CONTRIBUTIONS

1. **The Tabularium is not a metaphor — it's a building.** Ground the episode in the physical reality of Roman archive infrastructure. Show scrinia, armaria, pinax tags, wax tablets. Make the audience believe this is how it worked, because it IS how it worked.

2. **Pliny's work habits are the character.** Don't invent Pliny's personality — his nephew already documented it. The man who never stopped reading, who had secretaries in gloves, who counted wasted minutes. This is obsessive documentation as a way of life. The hash table gives him the tool his entire career was reaching for.

3. **Friatio is a real metaphor from Pliny's world.** The millstone, the olive press, the mortar and pestle — these are not analogies imported from modern experience. These are technologies Pliny documented, used, and understood. The hash function explained through Roman industrial technology is not a concession to the setting. It's the most natural explanation possible.

4. **Callimachus's Pinakes is the historical ancestor.** The first library catalog was a proto-hash-table. The show should establish this lineage — not to be educational, but because Methos was THERE, and the audience needs to see him flinch.

5. **Vesuvius is the wound that makes the technology matter.** Pliny isn't excited about hash tables because they're clever. He's devastated by them because they solve the problem that killed him — the fragility of knowledge stored in physical media, organized by human memory, vulnerable to fire and time. Every scroll in his archive is a scroll that almost didn't survive. The hash table means the next Vesuvius doesn't win.

---

*"Nulla dies sine linea."* — Pliny the Elder (via his nephew)
"Not a day without a line."

That was how he lived. That should be how this episode feels.
