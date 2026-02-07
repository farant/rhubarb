# NOVUM FUNDAMENTUM
## Episode 1x05: "Tabularium Novum" (The New Archive)
### Beat Sheet (Draft 1 — Compiled from Writers' Room)

---

## COLD OPEN

**Location:** Pliny's villa. Night. Late. Oil lamps. Pliny is at his desk, surrounded by scrolls and the census report printed from 1x04. He has a question from the senate: a property dispute between two families in district five. He needs the census records of both families.

He begins reading from the top. Entry one. Entry two. Entry three.

He's searching linearly. The way he has always searched. The way you search a scroll — you start at the beginning and you read until you find what you're looking for or reach the end.

By entry three hundred, his eyes are straining. The oil lamp flickers. He rubs his face. Continues.

**CUT TO:** Morning. Pliny is still at the desk. He has found the first family — entry two thousand, one hundred and fourteen. He has not found the second. The pages are marked in his meticulous hand. Hours of reading for one name.

Casey enters with bread and olives. He sees the old man at the desk. The stack of pages. The exhaustion.

**CASEY:** How long have you been searching?

**PLINY:** Since the third watch.

Casey looks at the census printout. Three thousand entries. Sequential. Unsorted.

**CASEY:** When you wrote the Naturalis Historia — all thirty-seven volumes — how did you find things?

**PLINY:** *(surprised by the question)* I wrote an index. Volume one. Every subject listed, every book and chapter referenced. No scholar would write an encyclopedia without an index. That would be —

He stops. Looks at the census report. Three thousand entries. No index.

**PLINY:** That would be what we have done.

**CASEY:** *(quiet)* We built the encyclopedia. We forgot the index.

SMASH TO MAIN TITLES.

---

## ACT ONE: "Index" (The Index)

*The problem crystallizes. The scholar asks the question the engineers missed.*

### Beat 1: Pliny Before the Senate

**Location:** Senate chamber. Morning session. Pliny reports on the property dispute. He has the answer — but it took him all night to find it. The senators are not impressed by the delay.

**SENATOR PRISCUS:** The census was complete within a day. The query took longer than the count?

**PLINY:** The ledger grows beautifully. But it does not answer questions. It merely... contains answers. The difference is significant.

Pliny is too diplomatic to say "your new technology is useless for lookup," but that's what he means. Marcus, in the gallery, hears it. Carter hears it.

**PLINY:** *(to the senate)* When Rome was small, the censor knew every family. He could be asked a name and give you the record from memory. Nova Roma has three thousand citizens. No man's memory is sufficient. We need a method.

### Beat 2: The Workshop — Defining the Problem

**Location:** Pliny's villa. Workshop. The team assembled. Pliny has come from the senate. He is tired but precise.

Before the hash table discussion, Pliny gives Carter a brief tour of his archive room. Walls of armaria — wooden cabinets with individual compartments, each holding scrolls. Stacks of scrinia — cylindrical containers with pinax tags protruding from each scroll. Wax tablets in organized piles. Everything has a place — but the place is in Pliny's head.

**PLINY:** Geography is on the east wall. Natural philosophy, west. Medicine, south — organized by ailment, then by remedy, then by source authority.

**CARTER:** What happens when someone else needs to find a scroll?

**PLINY:** *(simply)* They ask me.

**CARTER:** And if you're not here?

Long pause. Pliny puts a scroll back. Very carefully.

**PLINY:** Then it takes longer.

He turns to the team.

**PLINY:** I need to look up a citizen by name. Not by position in the census. By *name*. "Find me Gaius Aurelius." And the answer must come immediately — not after reading three thousand entries.

Carter nods. She knows what this is. She's been waiting for this moment since 1x04 — the moment the data gets large enough that sequential search becomes unbearable.

**CARTER:** Right now, finding one name in three thousand entries requires checking every entry. On average, you'll check fifteen hundred before you find it. That's called linear search — the time grows with the size of the data.

**MARCUS:** *(standing, as always)* When I inspect a road, I do not walk every mile to find one milestone. I know the section. I go to the section.

**CARTER:** Exactly. What if, instead of searching, you could *calculate* where to look?

Silence. The word "calculate" lands differently for the Romans. Calculation is what you do with numbers. Names are not numbers. How do you calculate where a *name* lives?

**PLINY:** *(leaning forward)* You mean to say — given only a name — you can determine its location? Without searching?

**CARTER:** Given only a name, I can *grind it down* to a number. And that number tells me where to look.

**PLINY:** *(his scholar's mind catching fire)* Grind it down. *Friare.*

### Beat 3: Pliny's Metaphor — The Mill

This is the naming moment. Not Daniel this time — Pliny.

**PLINY:** When the miller takes wheat to the mill, what enters is grain — complex, varied, each kernel unique. What emerges is flour — uniform, reduced, essential. The grain is destroyed in its particularity, but its *substance* is preserved. You can bake bread from flour. You cannot bake bread from intact grain.

He stands. For an eighty-year-old man, he moves with sudden energy.

**PLINY:** You are proposing a mill for names. The name goes in — complex, varied, unique. A number comes out — uniform, reduced, essential. The name is not destroyed, but it is *transformed* into something you can work with. Something you can use to find your way.

**CARTER:** That's exactly right.

**PLINY:** *Friatio.* The grinding. The act of reducing something to its essence without losing what matters.

**DANIEL:** *(from the corner, writing)* *Friatio: actus friandi. Reductio ad essentiam.*

**PLINY:** *(to Carter)* Show me.

### Beat 4: The First Hash

Carter at the keyboard. On screen:

```c
i32 friatum = friatio_fnv1a(nomen_civis);
```

She types a name: "Gaius Aurelius Corvinus." The function runs. A number appears: `1847293651`.

**CARTER:** That's the hash. The *friatum*. The ground-down essence of the name. It's always the same number for the same name. Always.

She types the name again. Same number. Types it with a capital letter changed — different number.

**PLINY:** The same grain through the same mill produces the same flour.

**CARTER:** Every time.

**MARCUS:** And this number tells you where to look?

**CARTER:** *(drawing on the slate)* Imagine a table with sixteen slots. The number is very large — over a billion. But we only need sixteen. So we take the number and reduce it further. *(she writes)* The number, masked to sixteen slots, gives us... slot three.

```c
i32 positus = friatum & (capacitas - I);
```

**CARTER:** Slot three. That's where Gaius Aurelius Corvinus lives. Not "somewhere in three thousand entries." *Slot three.* One step.

**MARCUS:** *(testing)* And if I ask for a different name?

Carter types "Lucia Sempronia." Different friatum. Different slot. Slot eleven.

**MARCUS:** Every name has a home.

**CARTER:** Every name has a home.

### Beat 5: ACT BREAK — The Collision

They build a small table. Sixteen slots. They start inserting names.

Entry one: "Gaius Aurelius" -> slot 3. Success.
Entry two: "Lucia Sempronia" -> slot 11. Success.
Entry three: "Marcus Valerius" -> slot 7. Success.

The team watches. Each name finds its slot. It's elegant. It's fast. Casey is smiling. Data is observing. Teal'c is writing tests.

Entry eight: "Titus Flavius" -> slot 3.

Slot three. But slot three already holds Gaius Aurelius.

The screen shows the conflict. Red text. Two different names, ground through the same mill, producing the same location.

**CARTER:** *(not surprised — she's been waiting for this)* Collision.

**PLINY:** *(the scholar's delight)* Two different grains producing the same flour?

**CARTER:** Not the same flour — the same *bin*. The mill has a finite number of bins. Eventually, two different grains land in the same one.

**MARCUS:** *(the engineer, immediately practical)* Then the bin must accommodate both.

**CARTER:** That's the question. How?

Hard cut. The slate board. Carter has drawn two names pointing to the same slot. A question mark between them.

---

## ACT TWO: "Collisio" (The Collision)

*The hash table's fundamental challenge. Two solutions. The Roman way wins.*

### Beat 6: Two Schools of Thought

Carter explains the two classical approaches to collision:

**CARTER:** *(drawing)* First approach: when two names land in the same slot, you make the slot bigger. You hang a list off slot three — Gaius first, then Titus. When you look for Titus, you go to slot three and walk the list.

She draws it — chains hanging from the table slots.

**CARTER:** Second approach: when slot three is taken, you try slot four. If slot four is taken, you try slot five. You walk forward until you find an empty slot. That's where Titus lives.

She draws this — a flat table, an arrow sliding from slot three to slot four.

**MARCUS:** *(immediately)* The second.

**CARTER:** Why?

**MARCUS:** *(pointing at the first drawing)* This is a table with ropes dangling from it. Every rope is a separate structure. Every rope can be a different length. You cannot survey the table and know its contents at a glance. You must pull every rope.

He points at the second drawing.

**MARCUS:** This is a floor. Everything is on the floor. When you enter the room, you see all of it. Nothing is hidden behind a rope. *(beat)* A Roman census is kept on a flat surface. Tablets laid in a row. The inspector walks the row. He does not climb ropes.

Carter stares at him. He's articulated the argument for open addressing vs. chaining — and he's right, not for the abstract computer science reason, but for the structural one. Cache locality. Memory contiguity. The inspector walking the flat surface.

**CASEY:** *(to Carter, quietly)* He just invented open addressing from first principles.

**CARTER:** He didn't invent it. He *remembered* it. This is how Romans have always organized things.

### Beat 7: Linear Probing

Carter implements Marcus's instinct.

```c
/* Si slotus occupatus est, proba proximum */
positus = (positus + I) & tabula->capacitas_mask;
```

On screen, the insertion of "Titus Flavius": slot 3 is occupied, try slot 4. Slot 4 is empty. Titus goes in slot 4.

**MARCUS:** And when you search for Titus?

**CARTER:** You hash his name. Get slot three. Check — it's Gaius, not Titus. Move to slot four. Check — it's Titus. Found him.

**MARCUS:** You are walking the floor.

**CARTER:** Walking the floor. Never more than a few steps. Usually one.

**DATA:** *(from his corner, precise)* The average number of probes for a successful search in a hash table with load factor alpha is approximately one divided by one minus alpha. At seventy-five percent load — three occupied of every four slots — the average is four probes.

**PLINY:** *(to Data)* You are saying that even when the archive is three-quarters full, the search requires only four steps?

**DATA:** On average, yes.

**PLINY:** I searched three thousand entries for eight hours last night. This method would have found my answer in... four steps?

**DATA:** Approximately.

Pliny sits back. The magnitude of this lands on him physically. Eight hours reduced to four steps. Not incrementally better — categorically different.

### Beat 8: The Tombstone Problem

More entries. The table fills. Then Carter demonstrates deletion.

**CARTER:** A citizen dies. Or moves away. We need to remove their entry. But we can't just empty the slot.

**MARCUS:** Why not?

Carter draws on the slate. Three entries: A in slot 5, B in slot 6 (collided from slot 5), C in slot 7 (collided from slot 5). She erases B.

**CARTER:** If you empty slot six, and then search for C — you hash C, get slot five, find A. Not C. Move to slot six — empty. You stop. You think C doesn't exist. But C is in slot seven, behind the gap you created.

**MARCUS:** *(understanding immediately)* You have broken the road. The inspector walks from milestone five to milestone six, finds nothing, and turns back — never reaching milestone seven.

**CARTER:** Exactly. So we don't empty the slot. We mark it.

She writes on the slate:

```
SLOT_VACUUM    — never used, truly empty
SLOT_OCCUPATUM — contains a citizen
SLOT_DELETUM   — was occupied, now cleared, but the road continues
```

**PLINY:** A tombstone. The slot remembers that someone was here. The search continues past it.

**CARTER:** *Tumulus.* The tombstone slot. The inspector passes it and keeps walking. Only a truly empty slot — one that was *never* used — stops the search.

**MARCUS:** *(quietly, thinking of the census)* The record remembers they were here.

This connects directly to O'Neill's question from 1x04, Beat 15 — "What happens when someone dies?" Pliny answered then: "The dead remain in the census. They are marked, not erased." Now the data structure implements that philosophy. SLOT_DELETUM is the digital tombstone.

### Beat 9: Pliny Asks About the Crucifix

During a lull in the technical work. Pliny is in his element this episode — his encyclopedia is becoming searchable, his life's work gaining a new kind of permanence. He's been watching the team work. He's also been watching Casey.

The crucifix has been on Casey's workbench since 1x02. Pliny has seen it every day. He has not asked. He was waiting until he had the right question.

Pliny pauses near the workbench. He's looking at the crucifix the way he looks at a new specimen — not with suspicion, but with scholarly attention.

**PLINY:** This image. I have seen it on your wall since you arrived. What does it represent?

Casey looks at the crucifix. He's been asked this before — but not by an eighty-year-old Roman polymath who genuinely wants to know the way he wants to know what kind of stone the forum is built from.

**CASEY:** A man who died for everyone.

**PLINY:** *(processing — this is a factual claim)* Willingly?

**CASEY:** Yes.

Pliny writes something down. Not much. A note in the margin of whatever he's working on. The way he writes down the properties of a new mineral — not because he believes in it, but because it has been observed and observation demands record.

He does not ask a follow-up question. Not yet. The polymath has opened a file. He will return to it.

**DIRECTION NOTE:** The writing-down is critical. It signals Pliny is *cataloging*, not reacting emotionally. The crucifix has moved from "decoration" to "datum" in his classification system.

### Beat 10: ACT BREAK — The Load Factor

The table fills. Eight of sixteen slots occupied. Then nine. Ten. Eleven. Twelve.

The collisions increase. What was one or two probes becomes five, six, seven. Marcus watches the `collisiones_totales` counter climb. The `distantia_maxima` — the longest probe distance — reaches eight.

**MARCUS:** The road is becoming crowded. The inspector is walking too far.

**CARTER:** That's the load factor. The ratio of entries to total slots. *(she points at the screen)* We're at seventy-five percent. Three of every four slots filled.

```c
f32 factor = tabula_dispersa_factor_oneris(tabula);
/* 0.75 — the threshold */
```

**MARCUS:** A road at three-quarters capacity is manageable. A road at full capacity is gridlock. Where is the threshold?

**CARTER:** Seventy-five percent. When we cross it, we grow.

**MARCUS:** *(the man who designed growth in 1x04)* How do you grow a flat table?

**CARTER:** You build a bigger table. Double the slots. And then —

She hesitates. Because this is the moment the Tabula Dispersa breaks from the Xar philosophy.

**CARTER:** And then you re-grind every name. Every existing entry gets its *friatum* recalculated against the new table size. They move to new slots.

Silence. Marcus stares at her.

**MARCUS:** Everything moves.

**CARTER:** Everything moves.

**MARCUS:** We spent all of last episode ensuring nothing moves. *Crescentia sine destructione.* And now you tell me the index must destroy itself to grow?

**CARTER:** The *data* doesn't move. The census entries in the Xar are exactly where they were. It's the *index* that restructures. The references change, not the records.

This is the critical distinction — and the dramatic tension of the episode. The Xar (1x04) grows without destruction. The Tabula Dispersa grows *through* reorganization. The index is not the archive. The archive is permanent; the index is rebuilt.

**PLINY:** *(the scholar, understanding perfectly)* When I write a new edition of my encyclopedia, the text does not change. But the index must be rewritten entirely, because the page numbers have shifted. The *knowledge* is stable. The *finding aid* adapts.

**MARCUS:** *(processing)* The road stays. The signs move.

**CARTER:** The road stays. The signs move.

Hard cut. The slate board now shows two tables side by side — the old (16 slots) and the new (32 slots). Arrows connecting names from old positions to new positions. Every name has moved. But every name is still *findable*.

---

## ACT THREE: "Tabularium" (The Archive)

*The system comes together. The test reveals a truth. Casey and Pliny connect.*

### Beat 11: Building the Census Index

The team constructs the real system. The Xar holds the census data (from 1x04). The Tabula Dispersa indexes it by name.

Carter writes the integration code on screen:

```c
/* Creare indicem censualis */
TabulaDispersa* index = tabula_dispersa_creare_chorda(
    piscina, CXXVIII);

/* Per omnes cives ambulare, indicem aedificare */
XarIterator iter = xar_iterator_initium(census);
dum (!xar_iterator_finis(&iter))
{
    Civis* civis = (Civis*)xar_iterator_proximum(&iter);
    tabula_dispersa_inserere(index,
        civis->nomen,
        (vacuum*)civis);
}
```

**CARTER:** The census holds the people. The index holds their locations. Separate structures, separate concerns.

**PLINY:** *(watching the index build)* Volume one of the Naturalis Historia is the index. Thirty-six volumes of content. One volume to find them. *(beat)* You are building my volume one.

The table fills. It grows once — from 128 to 256 slots. The re-hashing completes in a fraction of a second. All three thousand citizens indexed.

**CARTER:** Try it.

Pliny approaches the keyboard. His first time typing in the series. Casey stands beside him, guiding his fingers. An eighty-year-old scholar who has spent decades with a stylus — the hand-eye coordination transfers, but the layout is alien.

**PLINY:** Gaius Aurelius Corvinus.

Casey helps him type it. The query runs:

```c
Civis* civis;
tabula_dispersa_invenire(index,
    nomen_chorda, (vacuum**)&civis);
```

One probe. Slot found. The citizen record appears on screen instantly.

**PLINY:** *(very quietly)* Eight hours. That is what this took me last night.

He types another name. Another instant result. Another. Another.

**PLINY:** *(not looking up from the screen)* This changes everything.

### Beat 12: The Friatio Spectrum

Data approaches the group with an observation. He has been studying the friatio library.

**DATA:** You have implemented multiple hash functions of varying strength. FNV-1a for speed. DJB2 as an alternative. SHA-1 and SHA-256 for cryptographic purposes. CRC32 for error detection. *(beat)* This is a spectrum of grinding. Coarse to fine.

He draws on the slate:

```
Coarse <-------------------------------> Fine
FNV-1a    DJB2    CRC32    SHA-1    SHA-256
 fast      fast    medium   slow     slowest
 32-bit    32-bit  32-bit   160-bit  256-bit
```

**DATA:** FNV-1a grinds quickly. It produces a rough flour — adequate for finding the right bin. SHA-256 grinds slowly but produces a flour so fine that no two different grains have ever been known to produce the same result.

**PLINY:** *(fascinated)* The fineness of the grinding determines the certainty of identification?

**DATA:** Precisely. For a census lookup, coarse grinding is sufficient. For verifying that a document has not been altered — that requires the finest grinding possible.

**PLINY:** In my library, I use a rough index to find the scroll, and then I read the scroll to verify its content. Two levels of certainty. *(to Data)* You are describing the same principle across your entire spectrum.

**DATA:** I am. The coarse hash is the librarian who points you to the shelf. The cryptographic hash is the seal that proves the scroll has not been tampered with.

A moment of recognition between them — the naturalist and the android, both devoted to accurate observation, both compelled to record. Data is the person on Nova Roma most like Pliny in disposition, if not in form.

**PLINY:** *(to Data, with genuine warmth)* I believe our disciplines are complementary.

### Beat 13: Teal'c's Test Failure — The Duplicate Key

Teal'c has been building his test suite all episode. He runs it.

```
probatio_insertio_simplex .......... VERITAS
probatio_insertio_duplex ........... VERITAS
probatio_collisio .................. VERITAS
probatio_deletio_tumulus ........... VERITAS
probatio_crescentia ................ VERITAS
probatio_iterator .................. VERITAS
probatio_factor_oneris ............. VERITAS
probatio_invenire_post_crescentiam . VERITAS
```

Eight tests. Eight VERITAS. Then Teal'c runs one more test.

```
probatio_clavium_duplicatarum ...... FALSITAS
```

Silence.

**TEAL'C:** *(to the room, with the flat gravity of a man reporting an anomaly)* I have a test failure.

Everyone stops.

**TEAL'C:** I entered two different entries with the same key.

**MARCUS:** That should overwrite —

**TEAL'C:** It does. That is the failure.

He explains: Pliny has two entries for the volcanic glass formations in the eastern hills. One from thirty years ago. One from last year. The formations changed. The glass weathered differently than he predicted. Both entries are valuable. Both use the same key: "vitrum volcanicum orientale." When he inserts the second, the first is destroyed.

Pliny, across the room, looks up.

**PLINY:** *(slow)* You are telling me that my system can only hold one truth per question?

**TEAL'C:** I am telling you that the current implementation assumes each key has one answer. If the world changes — if the volcanic glass weathers, if a river changes course, if a population shifts — the old answer is overwritten. *(beat)* The hash table forgets.

The room is quiet. In most software contexts, overwriting on duplicate key is correct behavior. But Pliny's encyclopedia is a record of a *world* — a world that changes over time. The entry from thirty years ago isn't wrong. It's the world as it was. Both are true. Both are valuable.

**PLINY:** *(standing, which he rarely does)* If my encyclopedia can only hold the latest observation, it is not an encyclopedia. It is a newspaper.

**CARTER:** We can chain the values — every observation for that key, ordered by date. The searcher gets the history, not just the current answer.

**TEAL'C:** *(with what the audience might recognize as satisfaction)* Then the test should be: insert two values with the same key. Retrieve both. The hash table remembers.

Carter implements it — the value becomes a chain of observations, the most recent first, the oldest preserved. Nothing is overwritten.

**TEAL'C:** *(running the new test)* VERITAS.

He doesn't smile. But his nod has weight.

This is the mirror of 1x02 — Teal'c's test philosophy coming full circle. The test didn't fail because the system is broken. The test failed because Teal'c tested a property the system should have had but didn't yet. A freed slave insisting that history not be overwritten. A man who was counted as inventory ensuring that the count preserves what it counts.

### Beat 14: Casey and Pliny — The Weight of Memory

Late afternoon. The archive room. The others have gone to eat. Casey and Pliny alone. Pliny has been writing hash keys — the *claves* for each entry in his encyclopedia — for hours. Volume 7 of 40. His hand hurts. His eyes ache. The work stretches ahead of him.

Casey brings him wine. Not because anyone asked. Because Casey is the kind of man who notices when someone else has been working too long. He sits down across from Pliny. They don't speak for a moment. Two men comfortable enough with each other for silence.

**PLINY:** *(setting down his stylus, flexing his hand)* I have spent eighty years trying to hold everything. Every plant I have named. Every mineral I have weighed. Every citizen I have counted. Every conversation I remember with someone who is now dead. *(beat)* The weight of it. Not the knowledge — the *memory*.

**CASEY:** *(quiet)* That's a gift.

**PLINY:** It does not feel like a gift. It feels like... *(searching)* ...an evacuation.

Long pause. Casey drinks his wine. Pliny stares at the stack of volumes.

**PLINY:** You have lived longer than I have. Two hundred years. How do you carry that much remembering?

**CASEY:** Badly, sometimes.

**PLINY:** *(pressing)* But what do you do with it? When the weight of memory becomes too heavy?

**CASEY:** I pray.

**PLINY:** To whom?

**CASEY:** The God who remembers everything.

Beat. Pliny processes this. He has spent his entire life trying to record everything — every plant, mineral, custom, star, tidal pattern, insect behavior, soil composition. The idea of a being who actually DOES remember everything is not theological to him. It's practical. It's the endpoint of his own aspiration.

**PLINY:** That... would be a considerable memory.

**CASEY:** *(the smallest smile)* Yeah.

**PLINY:** *(after a long pause, genuinely curious)* And this helps? Knowing that someone remembers, even if you forget?

**CASEY:** I've been alive for two hundred years. I've forgotten more than most people ever learn. Names. Faces. Whole decades, sometimes. And the thing that keeps me from losing my mind isn't that I wrote it all down. *(looking at Pliny's volumes)* It's knowing that even if I lose it, it's not lost.

**PLINY:** Because your God remembers.

**CASEY:** Because nothing that matters is ever really forgotten.

Pliny picks up his stylus again. He writes a hash key. Then he stops.

**PLINY:** *(to himself, almost)* The record matters more than the recorder. I have been telling myself that for thirty years. Your faith says the same thing — except that in your version, the recorder also matters.

**CASEY:** *(quietly)* That's a pretty good summary, actually.

**PLINY:** *(back to his keys, but lighter now)* I should add your religion to Volume 31. Comparative theology. *(beat)* It would require its own hash key. Several, perhaps. *(beat)* I have more questions. But they can wait for another evening.

**CASEY:** I'm not going anywhere.

**PLINY:** Good. I have been waiting eighty years for interesting answers. I can wait a few more evenings.

He's intrigued. Not converted. Intrigued. A polymath who has found a subject he hasn't exhausted. The scene ends with both men working — Pliny writing keys, Casey entering data — the silence comfortable again but different now. Warmer. Something has opened.

---

## INTERCUT — FLASHBACK

**Location:** Misenum, Bay of Naples. August 24, 79 AD. Afternoon.

Pliny is writing. He's always writing. The room is cluttered — scrolls, wax tablets, pressed botanical specimens, a half-eaten meal he forgot about. Through the window: Vesuvius. A cloud is rising from its peak. Not smoke. Something larger. Something that looks like a pine tree, the trunk rising and the branches spreading.

**PLINY (V.O., present day, slow and precise):** I was writing about the cultivation of olive trees when the mountain opened. I did not stop writing immediately. The cloud was... interesting. I recorded its dimensions, its color, its rate of expansion. A naturalist's instinct. Observe first. Feel later.

The flashback INTERCUTS with present-day Pliny working on the hash table keys. In 79 AD, Pliny was indexing the natural world as it was being destroyed. In the present, he's indexing his own life's work so that it survives him.

**Flashback continues:** Pliny receives a message from Rectina at Stabiae. The shore is burning. People need rescue. Pliny orders the fleet.

**PLINY:** *(to his nephew, urgent but controlled)* Stay here. Write down everything you see. If I don't come back, the record matters more than the recorder.

Pliny on the ship, sailing TOWARD the eruption. Ash falling. Pumice striking the deck. And Pliny — dictating. To a scribe who is shaking so badly the wax tablet jumps. Pliny dictating observations about the eruption column, the wind direction, the composition of the falling debris. Not because he doesn't feel the fear. Because observation is what he does with fear. He turns terror into data.

The ship reaches Stabiae. Pliny helps people onto the boats. The air is thick. He can't breathe. He falls. The volcanic gas — sulfur, carbon dioxide — is heavier than air. It pools on the ground. Pliny is on the ground.

**And then the light changes.** Not the light of eruption. Something else — a shimmer, a distortion. The temporal effect. Nova Roma pulling him through. He doesn't understand it. He's on his back, choking, and then he's somewhere else, choking differently, and the sky is full of unfamiliar stars.

**PLINY (V.O., present day):** I woke in a field I didn't recognize under stars I couldn't name. I spent the first three months naming them. *(beat)* I never stopped writing. The world changed. The habit didn't.

**SMASH CUT TO:** Present day. Pliny writing a *clavis* — a hash key — for his entry on the stars of Nova Roma. The same stars he named when he arrived. His first act on a new world: observation. His last great act on that world: indexing.

**DIRECTION NOTE:** The flashback runs 60-90 seconds. Shorter than Duncan's cathedral in 1x04. The brevity IS the point — Pliny's Vesuvius is not something he dwells in. It's the wound that drives everything he does, glimpsed rather than explored.

**Visual Rhymes:**
1. Pliny dictating to the terrified scribe (79 AD) -> Pliny dictating hash keys to Carter (present day)
2. Ash falling on the ship's deck -> ink drying on parchment in Pliny's study
3. The nephew told to stay and keep writing -> Marcus inheriting the census
4. Pliny falling, unable to breathe -> Pliny's quiet moment with Volume 1 — the same confrontation with mortality
5. The temporal shimmer that saves him -> the hash function — something that transforms what it touches without destroying it

---

## ACT FOUR: "Naturalis Historia" (The Natural History)

*Pliny's encyclopedia enters the machine. The metaphor deepens.*

### Beat 15: Digitizing the Encyclopedia

A montage — but not a quick one. This is the labor of the episode. Daniel and Lucia transcribing Pliny's entries into chorda records. Casey typing. Data verifying. Marcus managing the workflow.

Each entry is a key-value pair: the subject name (chorda) is the key. The entry content (a pointer to the full record in the Xar) is the value.

```c
tabula_dispersa_inserere(
    encyclopedia,
    chorda_ex_literis(piscina, "cinnabar"),
    (vacuum*)inscriptio_cinnabar);
```

Entry after entry. Hundreds. The table grows — from 128 to 256, from 256 to 512. Each growth cycle visible on screen.

**LUCIA:** *(transcribing, to Daniel)* "Cinnabar: a mineral of blood-red color, found in the mines of Hispania. Used for pigment and mercury extraction." *(looking up)* Pliny wrote this from memory. He never went to Hispania.

**DANIEL:** He had informants. Letters from every province. The Naturalis Historia was the first crowd-sourced encyclopedia.

**LUCIA:** *(archly)* We called it "correspondence."

### Beat 16: The Power-of-Two Architecture

Carter explains the table structure to Marcus, who is monitoring the growth cycles.

**CARTER:** The capacity is always a power of two. Sixteen. Thirty-two. Sixty-four. One hundred twenty-eight.

**MARCUS:** Why?

**CARTER:** Because of how we calculate the slot. A power of two, minus one, gives a bitmask — all ones in binary. The hash masked against it gives the slot position instantly. No division needed.

```c
capacitas      = CXXVIII;       /* 10000000 in binary */
capacitas_mask = CXXVII;        /* 01111111 in binary */
positus        = friatum & capacitas_mask;
```

**MARCUS:** *(studying it)* The mask strips away everything above the capacity. Like a sieve. Only the bottom grain passes through.

**CARTER:** Exactly. And a sieve is faster than a scale. We could divide the hash by the capacity and take the remainder — same result. But division is slow. Masking is instant.

**MARCUS:** An engineer always prefers the simpler mechanism when the result is identical.

### Beat 17: O'Neill's Question

O'Neill has been quiet this episode. He's watching the hash table demonstration — Carter entering Pliny's entries, the table distributing them across slots.

**O'NEILL:** So you put something in, and the table decides where it goes?

**CARTER:** The hash function decides. Based on the key.

**O'NEILL:** And nobody chooses?

**CARTER:** The math chooses.

**O'NEILL:** *(skeptical)* Who chose the math?

Carter opens her mouth. Closes it. Because it's a real question. The hash function was designed — by Carter, by convention, by decades of computer science. The math doesn't "choose" neutrally. Someone chose the math. And whoever chooses the math chooses what gets found easily and what gets buried.

**O'NEILL:** I've run intelligence operations, Carter. The filing system is never neutral. Whoever designs the index controls the questions people can ask.

He walks out. Carter stares at the hash function. O'Neill, as usual, has asked the simple question that nobody technical thought to ask. This seeds the S3 governance crisis — the epistemological question of who designs the systems that determine what's findable.

### Beat 18: The First Query on the Encyclopedia

The encyclopedia index is complete. Twelve hundred entries. Plants, minerals, animals, geography, customs, laws — the full Naturalis Historia Nova, indexed by subject.

Pliny sits at the keyboard. Casey beside him.

**PLINY:** *(typing, slowly but correctly)* Cinnabar.

One probe. The entry appears.

**PLINY:** Olives.

One probe. Thirty-seven related entries, cross-referenced.

**PLINY:** *(faster now)* The mountain passes east of the colony.

Two probes — a collision resolved in one additional step. The geographic entry appears, complete with Pliny's original survey notes from sixty years ago.

**PLINY:** *(his eyes bright)* I wrote that entry when I was twenty. I have not been able to find it in my own scrolls for thirty years.

He types more. Faster. Faster. Each query returning in an instant. The eighty-year-old scholar reconnecting with his own life's work through the machine.

The camera pulls back. Pliny at the keyboard. Casey standing beside him. The screen scrolling with entries. The scrolls stacked behind them. Two archives — one analog, one digital — and a bridge between them.

Pliny's reaction is disproportionate. Not joy. *Relief*. The kind of relief that comes from a man who watched his world buried under ash eighty years ago, and who has just been given a promise that this time, the knowledge survives the knower.

---

## ACT FIVE: "Tabularium" (The Archive)

*The public system launches. The implications land. The title earns itself.*

### Beat 19: The Tabularium

**Location:** The Forum Stellarum. The senate has allocated a room adjacent to the chamber for the new system. In Roman architecture, the *tabularium* is the public archive — the building that stores the records of the state. Rome's Tabularium was built in 78 BC on the Capitoline Hill: a 67-meter corridor with eleven great arches, a palace built for paperwork. Rome built monumental architecture for its filing cabinets.

Marcus has organized the room. The laptop on a stone desk. The census Xar and the name index running. Pliny's encyclopedia index running alongside. Two tables, two indices, one system.

A citizen approaches. A father looking for property records.

**CITIZEN:** I am looking for the boundary survey of district four, western section.

Casey types the query. The encyclopedia index resolves it in one probe. The survey record appears — Pliny's field notes from thirty years ago, now instantly retrievable.

Another citizen. A magistrate checking census data. Another. A farmer verifying his family's registration.

Each query: one to three probes. Instant.

### Beat 20: The Tombstone Tollbooth

Carter notices the tombstone count climbing.

**CARTER:** *(to Marcus)* The tombstones. They're filling up the floor. The inspector has to step over more and more of them.

```c
f32 factor_deletorum = tabula_dispersa_factor_deletorum(tabula);
/* 0.18 — approaching threshold */
```

**CARTER:** When tombstones reach twenty-five percent of the table, we rebuild. Same as the load factor growth — double the table, re-hash everything, and the tombstones are cleared. The dead are honored in the census Xar — they stay at their position forever. But in the index, their markers are swept away to make room for the living.

**PLINY:** *(understanding)* The record preserves. The index forgets. Because the index serves the present, and the record serves eternity.

**MARCUS:** *Indicium vivit pro vivis. Tabularium vivit pro omnibus.* The index lives for the living. The archive lives for all.

### Beat 21: The Statistics

Carter runs the system diagnostic. On screen:

```
Status Tabulae Dispersae:
  Capacitas: 4096
  Numerus: 3217
  Numerus deletorum: 43
  Factor oneris: 78.54%
  Factor deletorum: 1.05%
  Collisiones totales: 1847
  Distantia maxima: 11
```

**CARTER:** Maximum probe distance: eleven. Out of three thousand entries. The worst case — the hardest name to find — takes eleven steps. *(to Pliny)* Your eight-hour search, in the worst case, takes eleven steps.

**PLINY:** *(softly)* Eleven steps.

### Beat 22: Pliny's Dedication

Evening. The Tabularium is quiet. Pliny sits alone in the archive room, the screen glowing softly. He has a stylus and a fresh wax tablet.

He writes a dedication. We see it in his meticulous Roman hand:

*TABULARIUM NOVUM NOVAE ROMAE*
*Aedificatum anno coloniae LXXX*
*Ne quid sit quod inveniri non possit*
*"That nothing exist which cannot be found"*

He sets the tablet above the doorway. An old man dedicating a new building to a new way of knowing.

Then — alone in the archive — he rests his hand on a leather scrinium. Old, cracked. He touches it the way you'd touch something you built with your hands.

**PLINY:** *(to himself, barely audible)* I could not save the scrolls. *(beat)* But I can make sure they are never lost this way again.

---

## TAG

### Beat 23: Methos's Warning

Methos drifts through the archive room. He's been reading Pliny's Volume 14 — marine biology, which Pliny wrote during his first decade on Nova Roma. He catches the tail end of the day's celebration.

**METHOS:** How many entries can it hold?

**CARTER:** As many as we need. The table resizes.

**METHOS:** And if it holds everything Pliny has written? All forty volumes?

**CARTER:** Then you can find any entry by its key. Instantly.

**METHOS:** *(setting down his wine)* I have lived for five thousand years. I have been in three libraries that held everything their civilizations knew. Alexandria. The House of Wisdom. A library in a city whose name I've forgotten, which is rather the point. *(beat)* Each one was organized. Each one was searchable. Each one was the pride of its civilization. *(beat)* Each one burned.

Silence.

**METHOS:** The question is never "can you find what you're looking for?" The question is: when the fire comes — and the fire always comes — what survives? *(looking at Carter's laptop)* Your machine holds one copy. Pliny's parchment holds one copy. How many copies of this index will there be when the fire comes?

He picks up his wine and leaves. The room is quiet. Methos, the man who watched Alexandria burn, has just asked the backup question — not as a technical concern but as a five-thousand-year-old's survival instinct. This seeds 1x15 (TCP/networking) and 1x18 (TLS/Arcanum).

### Beat 24: The Road and the Archive

Night. Marcus and Carter on the road out of Nova Roma. Their recurring walk — it's becoming a ritual.

**MARCUS:** The road carries. The archive finds. These are the two operations. Storage and retrieval.

**CARTER:** That's all computing is, in the end. Storing things and finding things. Everything else is elaboration.

**MARCUS:** *(considering)* The Romans stored grain in *horrea* and found it with inventories. We stored laws in the *tabularium* and found them with indices. *(beat)* You have not taught us anything new. You have given us better tools for what we already do.

**CARTER:** *(smiling)* That might be the nicest thing you've ever said to me.

**MARCUS:** It is not a compliment. It is an observation. *(beat)* The compliment is: your tools are very good.

**CARTER:** Next we need to handle the case where two citizens have the same name.

**MARCUS:** In the census, no two citizens have the same full name. Roman *tria nomina* ensure it. If two Gaiuses exist in district four, their cognomina distinguish them.

**CARTER:** And if the data isn't names? If it's categories? You might have fifty entries for "mineral."

**MARCUS:** Then you need fifty bins for "mineral." Or... *(thinking)* ...one bin that holds fifty things.

**CARTER:** That's 1x06.

**MARCUS:** *(raised eyebrow)* You already know what comes next?

**CARTER:** I always know what comes next. That's the job.

FADE TO BLACK.

---

## STINGER

### Beat 25: Kirk's Index

**Location:** Earth. Night. A different location from 1x04 — a co-working space, after hours. Desks too orderly. Screens all showing the same shade of green.

Kirk stands before the whiteboard. The map from 1x04 is still there — five pins now. Portland. Seattle. San Francisco. The Pacific Northwest node. Denver. The number has been updated: **5 / 31**.

But there's a second whiteboard. This one is new.

It's not a map. It's a *directory*. Names. Not the names of cities — the names of *people*. Organized not alphabetically, not chronologically, but by *function*. Categories: "Infrastructure." "Recruitment." "Signal Processing." "Material Acquisition." Under each category: names, locations, codes. A human hash table.

Kirk is indexing his network.

**KIRK:** *(studying the directory)* Portland has eleven in infrastructure, four in signal. San Francisco?

**VOICE:** Twenty-two infrastructure. Nine signal. Two material.

**KIRK:** *(writing, organizing)* When I need a signal processor on the West Coast, how quickly can you find one?

**VOICE:** Seconds.

**KIRK:** We don't grow until we can manage what we have.

He writes one more thing on the directory board, in the upper right corner. Small. A label:

**TABULARIUM**

The camera holds on the word. Kirk's archive. The same word the episode used for the team's project. The same concept. The same problem. The same solution.

Kirk turns back to the map. Traces a line from Denver to a blank space — somewhere east. He doesn't add it yet. He checks the directory first. Calculates what he needs before he expands.

Then, capping the marker:

**KIRK:** Every name has a home.

He says it flatly. Almost warmly. It's Marcus's line from Act 1, spoken by a man for whom "home" means "assignment." The audience hears the echo.

SMASH TO BLACK.

END OF EPISODE.

---

## ON-SCREEN CODE MOMENTS

### Moment 1: The First Hash (Act 1)
```c
i32 friatum = friatio_fnv1a(nomen_civis);
/* "Gaius Aurelius Corvinus" -> 1847293651 */
```

### Moment 2: The Slot Calculation (Act 1)
```c
i32 positus = friatum & (capacitas - I);
/* 1847293651 & 15 = 3 -> slot III */
```

### Moment 3: The Collision Resolution (Act 2)
```c
/* Slotus III occupatus — proba proximum */
positus = (positus + I) & tabula->capacitas_mask;
```

### Moment 4: The Tombstone States (Act 2)
```
SLOT_VACUUM    = 0  (truly empty — search stops)
SLOT_OCCUPATUM = 1  (contains entry — check name)
SLOT_DELETUM   = 2  (tombstone — search continues)
```

### Moment 5: The FNV-1a Inner Loop (Act 3)
```c
per (i = ZEPHYRUM; i < clavis.mensura; i++)
{
    friatum ^= (i8)clavis.datum[i];
    friatum *= (i32)FNV_PRIME;
}
```

### Moment 6: The Census Index Construction (Act 3)
```c
TabulaDispersa* index = tabula_dispersa_creare_chorda(
    piscina, CXXVIII);
tabula_dispersa_inserere(index,
    civis->nomen, (vacuum*)civis);
```

### Moment 7: The Growth Function (Act 2 Break)
```c
tabula->capacitas = capacitas_vetus * II;
tabula->capacitas_mask = tabula->capacitas - I;
/* Re-friare omnes introitus */
```

### Moment 8: The Statistics Readout (Act 5)
```
Status Tabulae Dispersae:
  Capacitas: 4096
  Numerus: 3217
  Factor oneris: 78.54%
  Collisiones totales: 1847
  Distantia maxima: 11
```

---

## C-PLOT THREADING

### Beat C1 (Cold Open, simultaneous): The Compass

Carter is setting up equipment in the Forum Stellarum. She has a compass on a stone ledge. The needle is pointing north. Normal.

She walks by five minutes later. The needle is pointing east.

Carter stops. Picks it up. Taps it. The needle swings — not back to north, but in a slow, full 360-degree rotation. Then it stops. Points north again.

She looks around. Midas is crossing the forum, heading toward Data's workstation with two cups of something hot.

The compass spins again as Midas passes. Carter sees it. Midas doesn't.

**CARTER:** *(to herself, very quiet, reaching for her notebook)* That's not magnetic variation.

She writes a note. Quick. Clinical. She doesn't tell anyone. Not yet. But the notebook goes into her pocket, not onto the shelf.

### Beat C2 (During Act 2): The Second Anomaly

During a break from the hash table work. Carter is outside with the compass. Midas is in the courtyard, talking to Data. The compass needle spins.

Carter marks Midas's approximate position. Distance from compass. Time.

**CASEY:** *(who has followed her out)* What are you looking at?

**CARTER:** Have you noticed anything... off? With time? Here?

**CASEY:** *(careful)* The clocks don't always agree. I figured it was the equipment.

**CARTER:** It's not the equipment.

She puts the compass away. Casey's expression says: noted. Filed. He'll be watching too. This plants the seed for 1x08 (Horologium/Tempus).

### Beat C3 (Late Act 2 or Act 3): The Methos Reaction — The Black Stone Seed

The team is celebrating a successful hash table test. Midas is present, laughing with Data. Carter checks the compass reflexively — the needle is off.

In the background: Methos. Leaning against a pillar, wine cup in hand. But the camera catches something the characters don't.

Methos is watching Midas. Not the compass — Midas himself. And his expression shifts.

It's not surprise. It's not fear. It's *recognition*. Something very old waking up behind very old eyes.

The moment is three seconds long. Nobody sees it. The audience does.

Methos takes a drink. The expression vanishes. He says something dismissive — "You've built a filing system. The Romans invented those before your species invented shoes" — and the moment passes.

**DIRECTION NOTE:** This must be small enough to miss on first viewing and unmissable on rewatch. The audience who catches it will spend the next eight episodes wondering what Methos recognized.

### Beat C4 (Post-climax): Kirk's Infrastructure

O'Neill and Duncan, reviewing intelligence reports.

**O'NEILL:** NSA picked up another distributed signal. This one was routed through seven cities.

**DUNCAN:** Seven? Last time it was three.

**O'NEILL:** Kirk's expanding faster than we expected. Whatever he's building, he's getting organized about it.

**DUNCAN:** Organized how?

**O'NEILL:** The signals aren't random anymore. They're categorized. Different frequencies for different message types. He's built a routing system.

**DUNCAN:** *(absorbing)* He's not just growing. He's indexing.

**O'NEILL:** *(grim)* Same thing we did today. Except he's indexing people.

---

## ENSEMBLE BEATS (Not in the Main Thread)

### Duncan — Background, Watching

Duncan's presence is deliberate reduction after his spotlight in 1x04. One moment:

**DUNCAN:** *(watching Pliny write hash keys late at night, to Casey)* How old is he?

**CASEY:** Eighty. Give or take.

**DUNCAN:** He's writing like a man who knows he's running out of pages.

**CASEY:** He's writing like a man who has more to say than pages allow.

**DUNCAN:** *(long pause)* I've had four hundred years and never written a book.

**CASEY:** You built other things.

**DUNCAN:** Did I?

He walks away. The swordsman questioning what he's built. This deepens through the season.

### Methos — The Findability Dread

When the tabula dispersa goes live. Someone searches for "Methos." His entry appears. Instantly.

**METHOS:** *(watching his own entry appear)* ...That's disconcerting.

Played for comedy. But it's not funny. For the man who spent five thousand years being unfindable, being O(1)-retrievable is existentially threatening.

### The Cross-Franchise Algorithm Moment

Three franchise representatives describing hashing through different metaphors:

**PLINY:** It is like a naturalist's taxonomy — you reduce the animal to its genus, its species, its distinguishing features. The classification tells you where to find it in the record.

**DATA:** It is a deterministic mapping from an arbitrary-length input to a fixed-length output. Formally, a surjective function from the set of all possible keys to the set of bucket indices.

**CASEY:** It's like branding cattle. You look at the brand, you know which ranch. You don't have to chase the cow around the pasture.

Same algorithm. Three translations. All correct.

---

## THEMATIC NOTES

**"Tabularium Novum"** shifts meaning across the episode:
- **Act 1:** "The New Archive" = the literal problem. Pliny's encyclopedia can't be searched. He IS the index. When he dies, the index dies with him.
- **Act 2:** "The New Archive" = the hash table. The computational solution. The archive has learned to navigate itself.
- **Act 3:** "The New Archive" = the transition from 1x04's census to 1x05's retrieval. You can't search what you haven't counted, and counting is useless if you can't search.
- **Stinger:** "The New Archive" = Kirk's tabularium. His directory of operatives by function. Two new archives. One serves a republic; the other serves an invasion.
- **Meta:** "The New Archive" = the show's growing memory. Five episodes in, the audience is accumulating characters, technologies, threats, open questions. The show itself is a tabularium — and the threads are the hash functions that let the audience index what they know.

**The Archive and the Archivist:**

The episode's emotional core: what happens to knowledge when the knower dies? Pliny is eighty years old. He is mortal in a show full of Immortals. His encyclopedia exists because he remembers where everything is. The hash table doesn't replace his knowledge — it encodes his organizational principle into something that survives him. The hash table is the filing system made immortal.

**Growth Without Destruction — The Sequel:**

The Xar (1x04) grows without destruction. The Tabula Dispersa grows *through* reorganization. This is not a contradiction — it's a complementary truth. The archive preserves; the index adapts. The road stays; the signs move. Pliny understands this instantly because it describes his entire career: "When I write a new edition of my encyclopedia, the text does not change. But the index must be rewritten entirely."

**The Hash Function as Realist Operation:**

The hash function takes a thing and derives a fingerprint from *what the thing is* — processing every byte, respecting the full content of the key. This is realist classification: the label emerges from the nature of the thing, not from arbitrary assignment. Contrast with Kirk's stinger: he categorizes operatives by *function*, not identity. Two infrastructure operatives are interchangeable. Kirk's hash is nominalist — reduce the person to their utility. Marcus's census hashes over *names* (who they are). Kirk indexes over *roles* (what they do).

**Collisions as the Limits of Representation:**

Two different things producing the same hash proves only the *representation's* limits, not that the things lack distinct natures. The realist method: when surface classification fails, go deeper — compare the actual keys, examine the actual natures. The Borg make the opposite error: functional equivalence equals ontological identity.

**The Fingerprint of Knowledge:**

*Digitum essentiae* — Pliny's phrase. To know something is not to hold it — it is to be able to find it. Knowledge without retrieval is just weight. The hash table turns weight into access. The index transforms *having* into *finding*.

**Luke 15:4 (latent):**

"That nothing exist which cannot be found." Pliny's dedication echoes the shepherd who leaves ninety-nine sheep to find the one that is lost. An archive that allows nothing to be unfindable is an act of care disguised as engineering. Casey hears this resonance but doesn't say it. The audience can.

---

## GENRE ARCHITECTURE

**Foundation (Asimov):** Pliny IS Seldon. His encyclopedia IS the Encyclopedia Galactica. The First Foundation is the codebase (public, documented). The Second Foundation is the Immortals (long-lived, guiding from behind). Pliny is the bridge between them — mortal, but building what the immortals will maintain.

**SG-1 "The Fifth Race" (S2E16):** O'Neill once had an entire alien database dumped into his brain and couldn't find anything in it. Storage without retrieval. He LIVED the problem Pliny faces.

**SG-1 "The Torment of Tantalus" (S1E11):** Ernest Littlefield, 50 years on an alien planet with the combined knowledge of four ancient races. Couldn't read any of it. Not encrypted — un-indexed.

**TNG "The Inner Light" (S5E25):** Data carries Picard's memories. Data, who is now helping build a system for organizing memories, was himself a living hash table. "This is the first time I have helped build an index outside of myself."

**TNG "The Measure of a Man":** Data classified as *peregrinus amicus* in 1x04. Now his entry is IN the hash table. Findable. Permanent. If Data's slot number is 47 — THE Star Trek number — the fans will scream.

**Highlander — The Watcher Chronicles:** Joe Dawson's database was the ANTI-hash-table. Built without structure, constantly hacked, corrupted, stolen. Pliny is building the Watcher Chronicles correctly.

**Highlander — Methos's Findability:** For 5,000 years, Methos survived by being unfindable. The hash table makes everything O(1)-retrievable. His discomfort is genuine: searchability is power, and power requires trust.

**Casey at IBM, 1953:** Hans Peter Luhn invented hash tables at IBM Yorktown Heights. Casey was at IBM in the 1950s (established in 1x03). He may have watched hash tables get invented. "I've seen this before."

**Open Addressing = Entity Sovereignty:** Every entry gets its own slot. No chains. The anti-Borg hash table. Chaining is assimilation — multiple identities compressed into one location. Open addressing says: every piece of knowledge has its own place.

**SLOT_DELETUM as Roman Burial Mound:** The tombstone slot, commented as *tumulus* in the codebase. Roman funerary practice encoded into the data structure.

**The Compass and Determinism:** Hash tables require deterministic functions — same input, same output, always. The compass should point north. Near Midas, it doesn't. The very foundation of the hash table is unstable near the temporal anomaly. This is the Season 2 crisis in miniature.

**"Dispersit superbos" (Luke 1:51, Magnificat):** "Tabula Dispersa" — the scattered table. "He has scattered the proud." The hash table scatters entries; the Magnificat scatters the proud. Both impose order through distribution. Casey would hear the resonance. He wouldn't say anything.

---

## CONTINUITY NOTES

**From 1x01 (Piscina):** Both the Xar and the Tabula Dispersa allocate from the piscina. The dependency chain grows:
```
Piscina (memory, 1x01)
  -> Chorda (text, 1x03)
  -> Xar (collection, 1x04)
  -> Tabula Dispersa (index, 1x05)
```

**From 1x02 (Credo):** Teal'c's testing methodology. His test failure here is the inverse of 1x02 — in 1x02, Kirk passed every test (false positive). Here, the duplicate-key test reveals a property the system should have had but didn't yet.

**From 1x03 (Chorda):** The keys are Chordae — length-prefixed strings. The hash function operates on chorda data, not null-terminated strings. Daniel and Lucia's lexicon expands: "friatio," "tumulus," "digitum essentiae."

**From 1x04 (Xar):** The Xar holds the census data. The Tabula Dispersa indexes it. *Crescentia sine destructione* applies to the data (Xar); the index (Tabula Dispersa) grows through reorganization.

**To 1x06 (Internamentum):** Carter's closing line about non-unique keys and categories. The hash table's duplicate keys create the need for string interning.

**To 1x07 (Filum):** Methos's fire warning. The archive must be saved. An archive that lives only in memory dies with the machine.

**To 1x08 (Horologium):** Carter's compass observations need time-accurate timestamps. Casey noticed the clocks disagree. The time library emerges.

**To 1x13 (Aurum):** The compass seed, Methos's recognition, and eight episodes of accumulated temporal clues converge in the Midas flashback.

**Kirk's stinger parallel:** In 1x04, Kirk's network was a dark Xar — nodes that never move. In 1x05, Kirk's whiteboards are a dark Tabula Dispersa — names indexed by capability. Same infrastructure, opposite purposes.

---

## ADDITIONAL NOTES

**Casey at IBM (1953):**

When the hash table is demonstrated, Casey recognizes something:

**CASEY:** *(quiet, watching the demo)* I've seen this before.

**CARTER:** In the literature?

**CASEY:** In Yorktown Heights. 1953. A man named Luhn. He had index cards and a formula. He called it "scatter storage." Nobody paid attention.

The Immortal eyewitness to computing history, watching hash tables get invented again on a Roman planet in Latin.

**Daniel and Lucia — The Naming:**

**DANIEL:** Hashing. We need a Latin term.

**LUCIA:** Describe the process.

**DANIEL:** You take something complex — a name, a phrase — and you reduce it to a number. A fingerprint.

**LUCIA:** A *friatio*. A crumbling. You take something whole and crumble it into a powder. The powder tells you what it was made of, but you cannot reconstitute the stone from the powder.

**DANIEL:** *Friatio.* That's... actually really good.

**LUCIA:** *(dry)* You sound surprised.

Daniel hands her a stylus she dropped. Their fingers almost touch. Almost. She notices. He doesn't. Or pretends not to.

**Pliny's Encyclopedia Entry:**

After the hash table is working, Pliny opens a fresh wax tablet and writes:

*"De Tabulis Dispersis: instrumentum ordinandi per friationes. Genus: tabulae. Species: dispersa. Clavis ingredit; friatio dat locum; valor invenitur."*

He's filing the hash table alongside maritime creatures and Campanian flour. It's knowledge. It goes in the encyclopedia. The naturalist, observing the observation system.

---

## HISTORICAL PRODUCTION NOTES

**Pliny's archive room:** Modeled after the real Tabularium's interior rooms. Armaria on three walls (wood, individual compartments). Scrinia on tables (cylindrical containers with pinax tags). A large mensa in the center. An abacus (Roman counting board). Oil lamps — bronze, multiple wicks. Pliny works at night.

**The grain mill:** If shown, a *mola asinaria* (donkey-powered) or *mola manuaria* (hand-turned). The *meta* (fixed conical base) and *catillus* (rotating hopper) as two distinct stones. The grinding sound emphasized: stone on stone, rhythmic, heavy, ancient.

**Pliny's writing style:** Small, dense, incredibly neat. He fits more words per line than anyone else. Wax tablets for notes (erasable), papyrus for permanent records.

**Latin terminology audit:**
- *Tabula, friare, friatio, collisio, dispersa, capacitas, clavis, positus, distantia, numerus* — classical or grammatically regular formations
- *Slotus* — invented for the codebase, follows second-declension pattern
- *Valor, factor* — late/medieval Latin, transparent to any Latin reader

---

## OPEN QUESTIONS

1. **Pliny's typing.** Casey guides his hands. How much mechanical detail? The eighty-year-old scholar adapts because the hand-eye coordination from decades of stylus work transfers. The layout is alien, but the fine motor control isn't.

2. **The Teal'c test failure — which version?** The beat sheet uses the character writer's duplicate-key version (volcanic glass entries from different decades, overwrite problem) rather than the technical writer's function-collision version (two names producing the same FNV-1a hash). The duplicate-key version is more dramatically rich — "the hash table forgets" — and more philosophically loaded (nominalist vs. realist treatment of history). The function-collision concept (hash as proximity, not proof) could be seeded in dialogue without being the test failure itself.

3. **The Vesuvius flashback.** 60-90 seconds. Intercut with present-day Pliny writing hash keys. Shorter than Duncan's cathedral in 1x04. The brevity IS the point.

4. **Pliny's encyclopedia size.** 1200 entries from 40 volumes. The original Naturalis Historia had 2493 chapters — if we match that number on screen, the fans who check Wikipedia will realize Pliny has been writing the same book for eighty years.

5. **Data's slot number.** If Data's hash resolves to slot 47, it's the deepest Trek easter egg in the series. Worth computing whether the actual FNV-1a hash of "Data, peregrinus amicus" mod some power of 2 actually lands on 47.

6. **O'Neill's absence vs. presence.** O'Neill is quieter this episode (Pliny's spotlight), but the "Who chose the math?" beat gives him a crucial contribution that seeds S3 governance. Sufficient.

7. **The mola scene.** Should Pliny take Carter to a working grain mill, or is the metaphor sufficient in dialogue? The historian pitch argues for showing the physical technology on screen. Production decision.

8. **The Teal'c collision-as-displacement beat.** The genre fan pitched hash collision as a slavery/displacement metaphor (first entry gets natural slot, collisions are displaced). This is powerful but may compete with the duplicate-key test failure. Could be integrated as a separate, briefer observation from Teal'c during the collision discussion in Act 2, rather than the main test failure.
