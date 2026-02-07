# NOVUM FUNDAMENTUM
## Episode 1x05: "Tabularium Novum" (The New Archive)
### A-Plot Technical Pitch (Draft 1)

---

## THE DRAMATIC ENGINE

Three thousand citizens are in the Xar. The census from 1x04 is complete — every person counted, every family placed, every district mapped. Growth without destruction. The data exists.

But Pliny can't find anyone.

He has spent his entire adult life as a scholar. Eighty years of writing and organizing the Naturalis Historia Nova — thirty-seven volumes of every plant, mineral, animal, custom, and settlement in Nova Roma. And now there is a digital census with three thousand entries, and when the senate asks "Where is Marcus Aurelius Corvinus, baker, district seven?" the only way to answer is to walk through every single entry from the beginning until you find him.

It is as if Pliny had written his encyclopedia but forgotten to include an index.

The Xar stores. The Tabula Dispersa *finds*. Storage was the achievement of 1x04. Retrieval is the challenge of 1x05. And the insight that bridges them — the hinge of the entire episode — is *friatio*: the act of grinding something down to its essence.

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

**CASEY:** *(sitting down across from him)* When you wrote the Naturalis Historia — all thirty-seven volumes — how did you find things?

**PLINY:** *(surprised by the question)* I wrote an index. Volume one. Every subject listed, every book and chapter referenced. No scholar would write an encyclopedia without an index. That would be —

He stops. Looks at the census report. Three thousand entries. No index.

**PLINY:** That would be what we have done.

**CASEY:** *(quiet)* We built the encyclopedia. We forgot the index.

SMASH TO MAIN TITLES.

---

## ACT ONE: "Index" (The Index)

*The problem crystallizes. The team grasps what they're missing.*

### Beat 1: Pliny Before the Senate

**Location:** Senate chamber. Morning session. Pliny reports on the property dispute. He has the answer — but it took him all night to find it. The senators are not impressed by the delay.

**SENATOR PRISCUS:** The census was complete within a day. The query took longer than the count?

**PLINY:** The ledger grows beautifully. But it does not answer questions. It merely... contains answers. The difference is significant.

Pliny is too diplomatic to say "your new technology is useless for lookup," but that's what he means. Marcus, in the gallery, hears it. Carter hears it.

**PLINY:** *(to the senate)* When Rome was small, the censor knew every family. He could be asked a name and give you the record from memory. Nova Roma has three thousand citizens. No man's memory is sufficient. We need a method.

### Beat 2: The Workshop — Defining the Problem

**Location:** Pliny's villa. Workshop. The team assembled. Pliny has come from the senate. He is tired but precise.

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

She writes the code:

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

Entry one: "Gaius Aurelius" → slot 3. Success.
Entry two: "Lucia Sempronia" → slot 11. Success.
Entry three: "Marcus Valerius" → slot 7. Success.

The team watches. Each name finds its slot. It's elegant. It's fast. Casey is smiling. Data is observing. Teal'c is writing tests.

Entry eight: "Titus Flavius" → slot 3.

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

She draws it — chains hanging from the table slots. It looks like hanging laundry.

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

### Beat 9: ACT BREAK — The Load Factor

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

Marcus looks at the growth code:

```c
interior b32
_tabula_dispersa_crescere(TabulaDispersa* tabula)
{
    /* Nova capacitas: duplex */
    tabula->capacitas = capacitas_vetus * II;

    /* Omnia re-friare */
    per (i = ZEPHYRUM; i < capacitas_vetus; i++)
    {
        si (sloti_veteres[i].status == SLOT_OCCUPATUM)
        {
            tabula_dispersa_inserere(tabula,
                sloti_veteres[i].clavis,
                sloti_veteres[i].valor);
        }
    }
}
```

**MARCUS:** You keep the old entries. You grind them again through a wider mill. The flour is the same — the bins are different.

Hard cut. The slate board now shows two tables side by side — the old (16 slots) and the new (32 slots). Arrows connecting names from old positions to new positions. Every name has moved. But every name is still *findable*.

---

## ACT THREE: "Tabularium" (The Archive)

*The system comes together. Pliny's encyclopedia gets its index. The test failure reveals a truth.*

### Beat 10: Building the Census Index

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

Pliny approaches the keyboard. His first time typing in the series — he's watched others, but now he has a reason. Casey stands beside him, guiding his fingers.

**PLINY:** Gaius Aurelius Corvinus.

Casey helps him type it. The query runs:

```c
Civis* civis;
tabula_dispersa_invenire(index,
    nomen_chorda, (vacuum**)&civis);
```

One probe. Slot found. The citizen record appears on screen instantly. Name, age, district, occupation, family.

**PLINY:** *(very quietly)* Eight hours. That is what this took me last night.

He types another name. Another instant result. Another. Another.

**PLINY:** *(not looking up from the screen)* This changes everything.

### Beat 11: The Friatio Spectrum

Data approaches the group with an observation. He has been studying the friatio library.

**DATA:** You have implemented multiple hash functions of varying strength. FNV-1a for speed. DJB2 as an alternative. SHA-1 and SHA-256 for cryptographic purposes. CRC32 for error detection. *(beat)* This is a spectrum of grinding. Coarse to fine.

He draws on the slate — a line from left to right:

```
Coarse ←————————————————————→ Fine
FNV-1a    DJB2    CRC32    SHA-1    SHA-256
 fast      fast    medium   slow     slowest
 32-bit    32-bit  32-bit   160-bit  256-bit
```

**DATA:** FNV-1a grinds quickly. It produces a rough flour — adequate for finding the right bin. SHA-256 grinds slowly but produces a flour so fine that no two different grains have ever been known to produce the same result.

**PLINY:** *(fascinated)* The fineness of the grinding determines the certainty of identification?

**DATA:** Precisely. For a census lookup, coarse grinding is sufficient. For verifying that a document has not been altered — that requires the finest grinding possible.

**PLINY:** In my library, I use a rough index to find the scroll, and then I read the scroll to verify its content. Two levels of certainty. *(to Data)* You are describing the same principle across your entire... spectrum.

**DATA:** I am. The coarse hash is the librarian who points you to the shelf. The cryptographic hash is the seal that proves the scroll has not been tampered with.

**CARTER:** *(to Pliny)* For the census index, we use FNV-1a. Fast. Good distribution. The XOR-fold-multiply pattern.

She shows the code on screen:

```c
friatum = (i32)FNV_OFFSET_BASIS;  /* 2166136261 */

per (i = ZEPHYRUM; i < clavis.mensura; i++)
{
    friatum ^= (i8)clavis.datum[i];
    friatum *= (i32)FNV_PRIME;     /* 16777619 */
}
```

**PLINY:** Each letter of the name is folded into the grinding. The first letter, then the second changes the result, then the third changes it again. Each letter of the name matters.

**CARTER:** Every letter. Change one letter and the result is completely different. The *fingerprint* changes.

**PLINY:** *Digitum essentiae.* The fingerprint of the essence.

### Beat 12: The Iterator Test

Teal'c has been building his test suite. He runs it.

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

Eight tests. Eight VERITAS. Marcus watches. Pliny watches. The system holds.

Then Teal'c runs one more test.

```
probatio_nominum_simillimorum ...... FALSITAS
```

Silence.

**TEAL'C:** Failure.

### Beat 13: Teal'c's Test Failure

Teal'c explains the test. He created two names that are anagrams — the same letters in a different order.

**TEAL'C:** "Gaius" and "Suiga." The same letters. The same hash.

**CARTER:** *(checking)* That shouldn't... *(she runs it)* ...they don't hash to the same value. FNV-1a is order-dependent. The XOR-multiply chain is not commutative across the full sequence.

**TEAL'C:** Not those specific names. *(he pulls up the test)* These names.

He shows two names — carefully constructed — that produce the exact same FNV-1a hash for the 32-bit output space. A genuine collision. Not two names landing in the same slot by coincidence of table size — two names producing the *identical* hash value.

**TEAL'C:** The mill produces the same flour from two different grains. Not the same bin — the same *flour*. This is not a table collision. This is a function collision.

**CARTER:** *(looking at the code)* He's right. With a 32-bit hash, there are only four billion possible values. With enough names, some will collide at the hash level, not just the table level. The comparison function catches it — we compare the actual names, not just the hashes. So the table still works correctly. But...

**TEAL'C:** But the hash alone does not identify. It only locates.

**PLINY:** *(understanding the distinction)* The index points you to the shelf. But you must still read the scroll to confirm it is the right one.

**TEAL'C:** Correct. My test expected the hash to be unique. It is not. The table is correct despite this — the comparison function verifies identity. But the hash provides *proximity*, not *proof*.

**MARCUS:** *(to Teal'c)* You tested the mill, not the archive. The mill is not broken — it has a finite number of bins. The archive compensates.

**TEAL'C:** *(a nod)* The test was correct to fail. It revealed an assumption I held that was wrong. *(beat)* That is the purpose of testing.

This is the mirror of 1x02 — Teal'c's test philosophy coming full circle. The test did not fail because the system is broken. The test failed because Teal'c tested a property the hash was never meant to guarantee. The distinction between *locating* (hash) and *identifying* (comparison) is the episode's deepest technical insight.

### Beat 14: The Hash-Cachatum Optimization

In the aftermath of the test failure discussion, Marcus notices something in the code.

**MARCUS:** *(reading tabula_dispersa.c)* You store the hash alongside the entry. *Hash_cachatum.*

**CARTER:** The cached hash. When we search, we first compare the cached hash — a single integer comparison. Only if the hashes match do we compare the full names — which is slower because it compares every character.

```c
si (tabula->sloti[positus].hash_cachatum == friatum &&
    tabula->comparatio(
        tabula->sloti[positus].clavis, clavis) == ZEPHYRUM)
```

**MARCUS:** A sentinel at the gate. The sentinel checks your *appearance* — quick, approximate. Only if you look right does the gate open and the full inspection begin.

**CARTER:** That's exactly what it is. The hash comparison is the quick check. The string comparison is the thorough one.

**MARCUS:** Two levels. The rough index and the fine verification. *(to Pliny)* Your library principle.

**PLINY:** *(pleased)* My library principle, expressed in numbers.

### Beat 15: ACT BREAK — Pliny's Request

Pliny has been quiet. Watching. Absorbing. Now he speaks.

**PLINY:** *(to Carter)* This table indexes names. Can it index anything?

**CARTER:** Anything that can be ground to a number.

**PLINY:** Subjects. Categories. Plant species. Mineral types. Geographic features. Every entry in the Naturalis Historia Nova — indexed not by its position in the scroll, but by its *nature*.

He stands again. The energy of the scholar who has seen the future of his life's work.

**PLINY:** I have spent eighty years writing an encyclopedia that can only be read from beginning to end. You are offering me the ability to *ask it questions*. *(beat)* Not "What is on page four hundred?" but "Tell me about *cinnabar*." Not position — substance.

**CARTER:** We can do that.

**PLINY:** Then let us do it.

Hard cut. The workshop. Pliny's scrolls stacked beside Carter's keyboard. Two systems of knowledge about to merge.

---

## ACT FOUR: "Naturalis Historia" (The Natural History)

*Pliny's encyclopedia enters the machine. The metaphor deepens. Casey and Pliny connect.*

### Beat 16: Digitizing the Encyclopedia

A montage — but not a quick one. This is the labor of the episode. Daniel and Lucia transcribing Pliny's entries into chorda records. Casey typing. Data verifying. Marcus managing the workflow.

Each entry is a key-value pair: the subject name (chorda) is the key. The entry content (a pointer to the full record in the Xar) is the value.

```c
tabula_dispersa_inserere(
    encyclopedia,
    chorda_ex_literis(piscina, "cinnabar"),
    (vacuum*)inscriptio_cinnabar);
```

Entry after entry. Hundreds. The table grows — from 128 to 256, from 256 to 512. Each growth cycle visible on screen: the brief pause, the re-hashing, the new capacity.

**LUCIA:** *(transcribing, to Daniel)* "Cinnabar: a mineral of blood-red color, found in the mines of Hispania. Used for pigment and mercury extraction." *(looking up)* Pliny wrote this from memory. He never went to Hispania.

**DANIEL:** He had informants. Letters from every province. The Naturalis Historia was the first crowd-sourced encyclopedia.

**LUCIA:** *(archly)* We called it "correspondence."

### Beat 17: The Power-of-Two Architecture

Carter explains the table structure to Marcus, who is monitoring the growth cycles.

**CARTER:** The capacity is always a power of two. Sixteen. Thirty-two. Sixty-four. One hundred twenty-eight.

**MARCUS:** Why?

**CARTER:** Because of how we calculate the slot. *(she writes)* A power of two, minus one, gives a bitmask — all ones in binary. The hash masked against it gives the slot position instantly. No division needed.

```c
capacitas      = CXXVIII;       /* 10000000 in binary */
capacitas_mask = CXXVII;        /* 01111111 in binary */
positus        = friatum & capacitas_mask;
```

**MARCUS:** *(studying it)* The mask... strips away everything above the capacity. Like a sieve. Only the bottom grain passes through.

**CARTER:** Exactly. And a sieve is faster than a scale. We could divide the hash by the capacity and take the remainder — that gives the same result. But division is slow. Masking is instant.

**MARCUS:** *(nodding)* An engineer always prefers the simpler mechanism when the result is identical.

This is the only time the power-of-two design is explained. It's brief, it's clear, and Marcus's sieve metaphor makes it visceral.

### Beat 18: Casey and Pliny — The Weight of Memory

Late afternoon. Casey and Pliny are alone in the workshop. The others have gone to eat. Pliny is reviewing entries that have been indexed. Casey is cleaning up the transcription queue.

The energy is quiet. Two men who carry things — Casey his faith, Pliny his knowledge. Both forms of burden.

**PLINY:** *(not looking up)* You pray.

**CASEY:** *(caught off guard)* ...Yes.

**PLINY:** Marcus told me about your beads. The counting device for prayers. *(beat)* He was impressed. A man who counts prayers the way he counts citizens.

**CASEY:** It's not the same.

**PLINY:** Is it not? You repeat the same words in sequence. You track your position. You cycle through the full count and begin again. That is remarkably similar to what we have been building.

Casey looks at him. He hadn't considered his rosary as an iterator before.

**PLINY:** I have carried the weight of this colony's knowledge for eighty years. Every plant, every stone, every family name — I held them. Indexed them in my mind. Cross-referenced them by memory. *(beat)* The weight of memory is considerable.

**CASEY:** *(quietly)* What do you do when it gets too heavy?

**PLINY:** I write. That is why I have thirty-seven volumes. I cannot hold them in my head, so I put them on the page. The page holds them for me. *(beat)* What do you do?

**CASEY:** I pray.

**PLINY:** To whom?

**CASEY:** The God who remembers everything.

**PLINY:** *(long pause, genuinely considering this)* That... would be a considerable memory.

Casey half-smiles. Pliny isn't being dismissive. He's a scholar. He's evaluating the claim by its magnitude. A being who indexes all of reality — that's a data structure Pliny can respect even if he doesn't yet believe in the operator.

**PLINY:** In my library, every entry points to every other entry it relates to. Cinnabar points to mercury. Mercury points to Hispania. Hispania points to the mines. The mines point back to cinnabar. Everything connects to everything. *(beat)* Your God — does everything connect in the same way?

**CASEY:** *(after a moment)* That's... actually a beautiful way to put it.

**PLINY:** I am merely extending the metaphor. If your God remembers everything, then everything has an index entry. Nothing is lost. Nothing is unfindable.

He goes back to his work. Casey watches him for a moment. A polymath and a man of faith, finding common ground in the architecture of retrieval. Pliny has not been converted. He has been *intrigued*. The difference matters.

### Beat 19: ACT BREAK — The First Query on the Encyclopedia

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

The camera pulls back. Pliny at the keyboard. Casey standing beside him. The screen scrolling with entries. The scrolls stacked behind them. Two archives — one analog, one digital — and a bridge between them that transforms an old man's sorrow into joy.

---

## ACT FIVE: "Tabularium" (The Archive)

*The public system launches. The implications land. The title earns itself.*

### Beat 20: The Tabularium

**Location:** The Forum Stellarum. The senate has allocated a room adjacent to the chamber for the new system. In Roman architecture, the *tabularium* is the public archive — the building that stores the records of the state. Rome's Tabularium still stands at the foot of the Capitoline Hill.

Marcus has organized the room. The laptop on a stone desk. The census Xar and the name index running. Pliny's encyclopedia index running alongside. Two tables, two indices, one system.

A citizen approaches. A father looking for property records.

**CITIZEN:** I am looking for the boundary survey of district four, western section.

Casey types the query. The encyclopedia index resolves it in one probe. The survey record appears — Pliny's field notes from thirty years ago, now instantly retrievable.

Another citizen. A magistrate checking census data. Another. A farmer verifying his family's registration.

Each query: one to three probes. Instant.

**MARCUS:** *(to Carter, watching the traffic)* The road was for transportation. The table is for information. Both must be walked. But the table walks itself.

### Beat 21: The Tombstone Tollbooth

Carter notices the tombstone count climbing. Citizens who have died, entries marked SLOT_DELETUM — they're accumulating. The table is growing more cluttered.

**CARTER:** *(to Marcus)* The tombstones. They're filling up the floor. The inspector has to step over more and more of them.

She shows the tombstone ratio:

```c
f32 factor_deletorum = tabula_dispersa_factor_deletorum(tabula);
/* 0.18 — approaching threshold */
```

**CARTER:** When tombstones reach twenty-five percent of the table, we rebuild. Same as the load factor growth — double the table, re-hash everything, and the tombstones are cleared. The dead are honored in the census Xar — they stay at their position forever. But in the index, their markers are swept away to make room for the living.

**PLINY:** *(understanding)* The record preserves. The index forgets. Because the index serves the present, and the record serves eternity.

**MARCUS:** *Indicium vivit pro vivis. Tabularium vivit pro omnibus.* The index lives for the living. The archive lives for all.

### Beat 22: The Statistics

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

**MARCUS:** *(reading)* Collisions: one thousand eight hundred forty-seven. That means —

**CARTER:** Some names share initial slots. But the open addressing resolves them. Every collision is a detour of one or two steps. The system absorbs it.

**DATA:** The total collision count is a cumulative metric across all insertions and lookups. The average per lookup is approximately one point four probes. Effectively instantaneous.

**PLINY:** *(to no one in particular)* I have spent my life building an encyclopedia that could not be searched. Now it can be searched in fewer steps than I have fingers.

### Beat 23: Pliny's Dedication

Evening. The Tabularium is quiet. Pliny sits alone in the archive room, the screen glowing softly. He has a stylus and a fresh wax tablet.

He writes a dedication. We see it in his meticulous Roman hand:

*TABULARIUM NOVUM NOVAE ROMAE*
*Aedificatum anno colonia LXXX*
*Ne quid sit quod inveniri non possit*
*"That nothing exist which cannot be found"*

He sets the tablet above the doorway. An old man dedicating a new building to a new way of knowing.

---

## TAG

### Beat 24: The Road and the Archive

Night. Marcus and Carter on the road out of Nova Roma. Their recurring walk — it's becoming a ritual, the way they process each day's work.

**MARCUS:** The road carries. The archive finds. These are the two operations. Storage and retrieval.

**CARTER:** That's all computing is, in the end. Storing things and finding things. Everything else is elaboration.

**MARCUS:** *(considering)* The Romans stored grain in *horrea* and found it with inventories. We stored laws in the *tabularium* and found them with indices. *(beat)* You have not taught us anything new. You have given us better tools for what we already do.

**CARTER:** *(smiling)* That might be the nicest thing you've ever said to me.

**MARCUS:** It is not a compliment. It is an observation. *(beat)* The compliment is: your tools are very good.

Carter laughs. They walk. The stars are sharp. The road is solid.

**CARTER:** Next we need to handle the case where two citizens have the same name. The index assumes unique keys.

**MARCUS:** In the census, no two citizens have the same full name. Roman *tria nomina* ensure it — praenomen, nomen, cognomen. If two Gaiuses exist in district four, their cognomina distinguish them.

**CARTER:** And if the data isn't names? If it's categories? You might have fifty entries for "mineral."

**MARCUS:** Then you need fifty bins for "mineral." Or... *(thinking)* ...one bin that holds fifty things.

**CARTER:** That's 1x06.

**MARCUS:** *(raised eyebrow)* You already know what comes next?

**CARTER:** I always know what comes next. That's the job.

FADE TO BLACK.

---

## STINGER

### Beat 25: Kirk's Index

**Location:** Earth. Night. The unmarked office from 1x04. The whiteboard still shows 4/31. But now there's a second whiteboard beside it.

This one has names. Real names. Not pins on a map — *people*. Each name has a number beside it. Not a slot number — a capability code. Skills. Clearances. Access.

Kirk stands between the two whiteboards. Map on the left (where). Names on the right (who).

**KIRK:** *(to the room)* The Portland node lost two operators last week.

**VOICE:** Burned out. Went home.

**KIRK:** Replaced?

**VOICE:** We have three candidates. *(reading)* Chen. Okonkwo. Ramirez.

Kirk walks to the name board. He writes the three names. Each gets a number.

**KIRK:** Chen has data center experience?

**VOICE:** Seven years. She designed the Portland backup system.

Kirk circles "Chen." Connects her with a line to "Portland" on the map board. One probe. Found.

The camera pulls back. Two whiteboards. One for where. One for who. A hash table built in dry-erase marker by a man who is, in his own dark way, building a census. Counting his people. Indexing his resources. Finding them instantly.

The difference: Marcus counts citizens. Kirk counts *assets*. The same data structure. The same friatio. One grinds names to serve. The other grinds names to use.

**KIRK:** *(capping the marker)* Every name has a home.

He says it flatly. Almost warmly. It's Marcus's line from Act 1, spoken by a man for whom "home" means "assignment." The audience hears the echo.

SMASH TO BLACK.

END OF EPISODE.

---

## ON-SCREEN CODE MOMENTS

The episode should feature these specific code moments, each tied to a dramatic beat:

### Moment 1: The First Hash (Act 1)
```c
i32 friatum = friatio_fnv1a(nomen_civis);
/* "Gaius Aurelius Corvinus" → 1847293651 */
```
The name goes in, the number comes out. The grinding demonstrated.

### Moment 2: The Slot Calculation (Act 1)
```c
i32 positus = friatum & (capacitas - I);
/* 1847293651 & 15 = 3 → slot III */
```
The bitmask. The sieve that reduces a billion to a position.

### Moment 3: The Collision Resolution (Act 2)
```c
/* Slotus III occupatus — proba proximum */
positus = (positus + I) & tabula->capacitas_mask;
/* Slotus IV vacuus — insere */
```
Walking the floor. Linear probing made visible.

### Moment 4: The Tombstone States (Act 2)
```
SLOT_VACUUM    = 0  (truly empty — search stops)
SLOT_OCCUPATUM = 1  (contains entry — check name)
SLOT_DELETUM   = 2  (tombstone — search continues)
```
Three states. The moral architecture of deletion.

### Moment 5: The FNV-1a Inner Loop (Act 3)
```c
per (i = ZEPHYRUM; i < clavis.mensura; i++)
{
    friatum ^= (i8)clavis.datum[i];
    friatum *= (i32)FNV_PRIME;
}
```
The grinding. Each character folded in. The XOR-multiply chain.

### Moment 6: The Census Index Construction (Act 3)
```c
TabulaDispersa* index = tabula_dispersa_creare_chorda(
    piscina, CXXVIII);

/* Indicem censualis aedificare */
tabula_dispersa_inserere(index,
    civis->nomen, (vacuum*)civis);
```
The Xar and the Tabula Dispersa working together. Storage meets retrieval.

### Moment 7: The Growth Function (Act 2 Break)
```c
tabula->capacitas = capacitas_vetus * II;
tabula->capacitas_mask = tabula->capacitas - I;
/* Re-friare omnes introitus */
```
The signs move. The road stays.

### Moment 8: The Statistics Readout (Act 5)
```
Status Tabulae Dispersae:
  Capacitas: 4096
  Numerus: 3217
  Factor oneris: 78.54%
  Collisiones totales: 1847
  Distantia maxima: 11
```
The health of the system, quantified.

---

## THE MILL ANALOGY — COMPLETE MAPPING

| Tabula Dispersa Concept | Mill/Archive Analogy | Dramatic Moment |
|------------------------|---------------------|-----------------|
| Hash function (friatio) | The mill grinding grain to flour | Pliny: "The act of reducing to its essence" |
| Hash value (friatum) | The flour — uniform, workable | Carter: "Same number, always" |
| Table slots | Flour bins | Marcus: "Every name has a home" |
| Collision | Two grains in one bin | "Slot three is occupied" |
| Open addressing | Walking the floor | Marcus: "The inspector walks the row" |
| Linear probing | Stepping to the next slot | "Try slot four" |
| Tombstone (SLOT_DELETUM) | A marker where a bin was cleared | Pliny: "The road continues past it" |
| Load factor | Road congestion | Marcus: "Three-quarters capacity is manageable" |
| Growth (re-hash) | Rebuilding the index, not the archive | Pliny: "The knowledge is stable, the finding aid adapts" |
| Power-of-two capacity | The sieve | Marcus: "Only the bottom grain passes through" |
| Cached hash | The sentinel at the gate | Quick check before full comparison |
| FNV-1a (XOR-fold-multiply) | Coarse mill — fast, good enough | Data's spectrum: the fast grind |
| SHA-256 | Finest mill — slow, perfect flour | Data's spectrum: the certain grind |
| Iterator | Walking the archive floor | From 1x04, now applied to the table |
| Key-value pair | Subject and entry in the encyclopedia | Pliny's thirty-seven volumes |
| Index vs. data separation | Volume one (index) vs. volumes 2-37 (content) | "You are building my volume one" |

---

## PLINY'S CHARACTER ARC

This is Pliny's spotlight episode. His progression:

**Cold Open:** The scholar searching linearly. Eight hours for one name. The old way — the *only* way he's ever known.

**Act 1:** The naming. "Friatio." Pliny finds the metaphor before Carter finds the code. The mill is HIS image. This matters — Pliny isn't learning Carter's concept. Carter is implementing Pliny's insight.

**Act 2:** The silent observer. Pliny watches as Carter and Marcus work through collisions, tombstones, load factors. He absorbs. He cross-references against his own experience.

**Act 3:** The test failure. Pliny grasps the hash/identity distinction instantly — because he's been navigating approximate indices his entire career. "The index points you to the shelf. But you must still read the scroll."

**Act 4:** The encyclopedia enters the machine. This is Pliny's life's work being made searchable. The emotional core of the episode — eighty years of writing, now findable in one step.

**Act 5:** The dedication. *Ne quid sit quod inveniri non possit.* "That nothing exist which cannot be found." This is the scholar's prayer. The opposite of entropy. The dream of total retrieval.

This episode is about Pliny more than any other character because the hash table is, at its core, a *librarian's tool*. It solves the librarian's problem: how do you find what you're looking for in a vast collection? Pliny has been solving this problem with ink and memory for eighty years. The Tabula Dispersa doesn't replace his knowledge — it *amplifies* it.

---

## THEMATIC CORE: THE FINGERPRINT OF KNOWLEDGE

**"Digitum essentiae."** — Pliny's phrase. The fingerprint of the essence.

**Technical:** Every piece of data has a hash — a unique (or near-unique) numerical fingerprint derived from its content. The hash doesn't contain the data; it *points to* the data. It is the minimum information needed to find the maximum information stored.

**Philosophical:** To know something is not to hold it — it is to be able to find it. Pliny's encyclopedia holds thirty-seven volumes of knowledge. But knowledge without retrieval is just weight. The hash table turns weight into access. The index transforms *having* into *finding*.

**Theological (latent):** "That nothing exist which cannot be found." Pliny's dedication echoes Luke 15:4 — the shepherd who leaves ninety-nine sheep to find the one that is lost. An archive that allows nothing to be unfindable is an act of care disguised as engineering. Casey hears this resonance but doesn't say it. The audience can.

**Epistemological:** The distinction between *locating* and *identifying* — between the hash and the comparison — mirrors the distinction between knowing where to look and knowing what you're looking at. The hash gets you close. The comparison confirms. Two operations, two kinds of knowledge. Pliny has been doing this with his memory for eighty years: the rough recall of where the scroll is, followed by the careful verification of its content.

**Anti-Borg:** The Borg don't index. They don't need to — they absorb, and what is absorbed loses its individual identity. A hash table's entire purpose is to maintain individual identity within a collection. Each key is unique. Each entry is distinguishable. The Tabula Dispersa is architecturally anti-Borg.

---

## CONTINUITY NOTES

**From 1x01:** Piscina is the foundation. Both the Xar and the Tabula Dispersa allocate from the piscina.

**From 1x02:** Teal'c's testing methodology. His test failure here is the inverse of 1x02 — in 1x02, Kirk passed every test (false positive). Here, the hash fails Teal'c's test — but the system still works. A false negative on the hash, compensated by the comparison. Testing the right property matters.

**From 1x03:** Chorda stores the keys. The hash function operates on chorda data (not null-terminated strings). Daniel and Lucia's lexicon expands — "friatio," "tumulus," "digitum essentiae."

**From 1x04:** The Xar holds the census data. The Tabula Dispersa indexes it. The episode directly continues from 1x04's ending — Marcus's road now has signposts. *Crescentia sine destructione* applies to the data (Xar); the index (Tabula Dispersa) grows through reorganization. The tension between these philosophies is acknowledged and resolved: the archive preserves, the index adapts.

**To 1x06:** Carter's closing line about non-unique keys. Categories with multiple entries. This motivates the next data structure — multi-value tables or linked structures for handling one-to-many relationships.

**Kirk's stinger parallel:** In 1x04, Kirk's network was a dark Xar — nodes that never move. In 1x05, Kirk's whiteboards are a dark Tabula Dispersa — names indexed by capability, instantly findable. He is building the same infrastructure as Nova Roma, for opposite purposes.

---

## OPEN QUESTIONS

1. **Pliny's typing.** The beat where Pliny types for the first time — how much mechanical reality do we give this? Does Casey guide his hands, or does Pliny adapt quickly? An eighty-year-old scholar who has never seen a keyboard but has spent decades with a stylus. The hand-eye coordination transfers. The layout is alien.

2. **The Teal'c test failure names.** What two specific names produce an FNV-1a collision in the actual codebase? This should be real — computed from the actual hash function. The show's code is real code.

3. **Pliny's encyclopedia size.** Twelve hundred entries — is this right for a colony encyclopedia covering natural history? Pliny the Elder's original Naturalis Historia had approximately twenty thousand facts. A colony version would be smaller, more focused.

4. **The Casey-Pliny scene.** This is the B-plot enhancement from the season outline. Does it carry enough weight in this pitch or does the character pitch need to develop it further? The "God who remembers everything" line is the emotional center.

5. **O'Neill's absence.** O'Neill is notably quiet in this episode. Is this intentional (he had a strong 1x04) or should he have a moment? Perhaps a dry comment about the tombstone metaphor — "I've seen enough tombstones."

6. **The Tabularium room.** Is a dedicated archive room too much too soon? Or does it signal that the infrastructure is becoming *civic* — not just a workshop project but a public institution?
