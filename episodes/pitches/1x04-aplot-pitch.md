# NOVUM FUNDAMENTUM
## Episode 1x04: "Codex Tabulae Exponentialis" (The Growing Ledger)
### A-Plot Technical Pitch (Draft 1)

---

## THE DRAMATIC ENGINE

The Nova Roma census. Pliny has been doing it by hand for eighty years. Wax tablets. Scroll upon scroll. Every citizen counted, categorized, verified. The 1x03 report generator proved the system could produce a formatted report for *one* citizen. Now the senate wants the real thing: all three thousand citizens, organized, queryable, growing as the colony grows.

The problem isn't formatting. They solved that last episode. The problem is *storage*. Where do three thousand records go? And what happens when it's three thousand one? Or four thousand? Or ten thousand?

Carter proposes a fixed-size array. She knows this is wrong before she finishes explaining it, but she starts here because that's where Earth computing started, and because the error is the lesson.

**CARTER:** We allocate space for three thousand entries. One slot per citizen.

**MARCUS:** And the three thousand and first?

And there it is. The episode question. Marcus asks it immediately because he's an engineer and engineers count before they build. Carter knows the Earth answer — `realloc`, copy everything, hope nothing breaks — and she hates it. She's been watching the Romans do things better for three episodes. She's ready for them to do it again.

---

## COLD OPEN

**Location:** Nova Roma. Census day. Morning. The Forum Stellarum is transformed — tables set up in a semicircle, wax tablets stacked at each station, Roman census officials seated with styluses ready. This is a civic ritual older than the colony itself. The Romans take census *seriously*.

Citizens line up. Families. Names called out. Occupations recorded. Children counted. It's organized, efficient, ancient. Pliny oversees from a chair — eighty years old, too stiff to stand all day, but his eyes miss nothing.

Marcus moves through the operation like a site foreman. Checking tallies. Coordinating the officials. He's built roads and aqueducts for the colony; now he's managing the flow of *people* through an administrative bottleneck.

The system works. It's always worked. Until today.

A group arrives from outside the gates. Not the usual colonists — a delegation from the farming settlements that have spread beyond the original city walls. Thirty families. Maybe a hundred and fifty people. They've been growing grain in the valley for two generations. They want to be counted. They want to be *citizens*.

The chief census official looks at Pliny. His tablets are nearly full. He's been allocating one tablet per district, and the districts were fixed twenty years ago.

**CENSUS OFFICIAL:** There is no district for the valley settlers.

**PLINY:** Then create one.

**CENSUS OFFICIAL:** I have no more tablets prepared. The format assumes twelve districts. There are now thirteen.

Pliny looks at the settlers. At the tablets. At Marcus.

**PLINY:** *(to Marcus)* We have been keeping this count for eighty years. Every decade, the colony grows. Every decade, we run out of tablets and start again. *(beat)* There must be a better way.

Marcus looks at the settlers. Looks at Carter, who is watching from the edge of the forum.

**MARCUS:** There is.

SMASH TO MAIN TITLES.

---

## ACT ONE: "Census" (The Count)

*Counting what you have. Discovering it's not enough.*

### Beat 1: The Scale of the Problem

**Location:** Pliny's villa. Workshop. The core team gathered: Carter, Casey, Marcus, Teal'c, Daniel. Lucia observing. Data in the corner, still partially damaged, making notes.

Carter demonstrates the fixed-array approach on Casey's air-gapped laptop. A simple C array: `Civis census[MMM];` — three thousand citizen records, preallocated.

She enters ten test records. It works. She enters a hundred. It works. She enters three thousand. It works.

**CARTER:** There. Every citizen has a slot. It's fast, it's simple, it's —

**MARCUS:** Full.

He's looking at the number. Three thousand slots. Three thousand citizens. No room.

**CARTER:** Right. If we need more, we'd have to...

She trails off. She doesn't want to say it.

**CASEY:** Say it.

**CARTER:** We'd have to create a bigger array and copy everything over. Every record. Every entry. Move the whole census to a new location.

**MARCUS:** *(standing — he always stands when thinking)* You are telling me that every time the colony grows beyond a boundary, you destroy the existing record and rebuild it?

**CARTER:** We don't *destroy* it, we *copy* —

**MARCUS:** You tear up the road and relay every stone because you need to add one more mile.

Silence. Marcus has found the metaphor that will drive the episode. Carter knows he's right. Casey leans back. He knew they'd get here.

**CASEY:** *(to Carter, quiet)* Tell him how realloc works.

**CARTER:** When you outgrow the space, you request a larger block of memory. The system finds room, copies everything to the new location —

**MARCUS:** And every reference to the old location?

**CARTER:** Invalid.

**MARCUS:** Every person who knew where the census was kept?

**CARTER:** They'd need to be told the new address.

**MARCUS:** *(genuinely appalled)* This is how you have been doing it?

**CASEY:** For fifty years.

There's an echo here of 1x03 — "This is how you have been doing it?" — Marcus's recurring disbelief at Earth computing conventions. But this time it's HIS turn to propose the alternative.

### Beat 2: Marcus Takes the Lead

Carter does something she hasn't done before. She pushes her chair back from the keyboard. Not dramatically — just enough. She creates space.

**CARTER:** Marcus. How would *you* build this?

It's the first time she's asked a Roman to design a data structure from scratch. In 1x01, she explained Piscina and Marcus understood it. In 1x02, Teal'c designed the testing philosophy and Marcus learned it. In 1x03, Lucia corrected Daniel and Marcus validated the chorda design. But Carter always held the keyboard. Carter always wrote the first line.

Not this time.

Marcus doesn't sit down. He walks to the window. Through it, you can see the road that leads out of Nova Roma toward the valley settlements — the road Marcus himself built five years ago.

**MARCUS:** When I build a road, I do not know how far it will go. I know where it starts. I know the first mile. I pave that mile. And when someone needs to travel further, I pave the next mile. *(turning back)* I do not tear up the first mile to make a longer road. The first mile is *finished*. It stays where it is.

**CARTER:** And if the new section needs to be wider? Higher capacity?

**MARCUS:** Then the new section is wider. The old section served its purpose. The road still connects.

He's describing a segmented array. He doesn't know the term. He's describing it from engineering principles that are two thousand years old.

Casey is watching with an expression the audience hasn't seen from him before: recognition. Not recognition of the computer science concept — recognition that the Roman got there first. That the instinct is *right* and Earth just didn't listen to it.

### Beat 3: The First Segment

Marcus comes to the keyboard. This is the first time a Roman has typed code in the series. Carter is beside him, translating his design into C89. But the design is his.

**MARCUS:** The census begins with space for sixteen citizens. A cohort.

**CARTER:** Why sixteen?

**MARCUS:** A *contubernium* — the smallest military unit. Eight soldiers, but sixteen with their equipment counted separately. *(beat)* It is a number Romans trust.

Carter types:

```c
Xar* census = xar_creare(piscina, magnitudo(Civis));
```

**CARTER:** Xar. That's what we'll call it. The growing ledger.

**DANIEL:** *(from the side)* *Tabula exponentialis* — the exponential table. Because the sections grow by doubling?

**MARCUS:** Not yet. First, explain the sections.

This is Marcus leading. Carter implementing. Daniel naming. The whole team working together, but Marcus driving.

### Beat 4: ACT BREAK — The First Growth

They enter the first sixteen citizens. The first segment fills. Marcus adds the seventeenth.

On screen, the code executes. A new segment allocates. The seventeenth citizen goes into fresh space. The first sixteen haven't moved.

**MARCUS:** *(watching the screen)* The first mile of road is still there.

**CARTER:** Still there. Same place. Same address. Nothing moved.

**MARCUS:** *(the beginning of a smile)* Good.

But Teal'c, who has been watching:

**TEAL'C:** You have demonstrated that it can grow once. You have not demonstrated that it can grow under pressure.

Marcus looks at Teal'c. Two episodes ago, Teal'c taught him that failure is information. Marcus nods.

**MARCUS:** Then let us find where it breaks.

---

## ACT TWO: "Via" (The Road)

*The road extends. The design deepens. Marcus teaches Carter.*

### Beat 5: The Doubling Principle

The workshop. Marcus at the board (a smoothed piece of slate that Carter gave him in 1x02 for diagramming). He's drawing the segment structure.

**MARCUS:** The first section: sixteen. The second section: sixteen. The same size. *(drawing)* Like the two ends of a bridge that must meet in the middle. They should match.

**CARTER:** And then?

**MARCUS:** Then the road opens up. *(he draws a wider section)* The third section: thirty-two. Double. Because the further you go, the more traffic you expect.

**CARTER:** Why double? Why not add sixteen each time?

Marcus turns. He picks up a piece of chalk. On the slate, he draws a road with equal-width sections. Then he draws another road where each section is wider.

**MARCUS:** If every section is the same width, you need many sections for a large colony. Many sections means many joints. Many joints means many places for cracks. *(tapping the second drawing)* If you double, you have fewer sections. Fewer joints. The road is stronger.

He's independently arrived at the logarithmic segment count argument. Fewer allocations, fewer boundary crossings, better amortized performance — but he's expressing it as structural engineering.

**CASEY:** *(to Carter, under his breath)* He just reinvented the growth factor.

Carter writes the doubling rule on screen. The key code moment:

```
Segmentum 0: XVI    (indices 0-15)
Segmentum 1: XVI    (indices 16-31)
Segmentum 2: XXXII  (indices 32-63)
Segmentum 3: LXIV   (indices 64-127)
Segmentum 4: CXXVIII (indices 128-255)
```

**MARCUS:** *(reading the numbers)* Each section holds all the travelers that came before it. The road remembers its own growth.

**DATA:** *(from the corner, precise)* The total capacity after N segments is the sum of a geometric series. The capacity approximately doubles with each new segment.

**MARCUS:** *(to Data)* That is what I said.

**DATA:** Yes. I was providing the mathematical formalization.

**MARCUS:** *(beat, then a rare warmth)* ...Thank you.

This is the first moment Marcus addresses Data directly with something other than caution. Two engineers recognizing the same truth in different languages.

### Beat 6: Stable Ground

The critical technical moment. Carter explains what makes the Xar different from realloc.

She draws two diagrams on the slate beside Marcus's road drawing. The first: a traditional dynamic array. Grow, copy, invalidate. The second: the Xar. Grow by adding a new segment. Nothing moves.

**CARTER:** In the old way, when you grow, everything moves. Every reference breaks. It's like...

She pauses. Looks at Marcus.

**CARTER:** It's like if you extended a road by picking up every stone and relaying them in a new location, plus the new stones. Every milestone now points to the wrong distance. Every map is wrong.

**MARCUS:** *(disgusted)* No Roman engineer would do this.

**CARTER:** No. They wouldn't. And that's the point.

She types the key function: `xar_addere`.

```c
vacuum*
xar_addere(Xar* xar)
{
    /* Invenire ubi elementum novum ibit */
    /* Assecurare segmentum existit */
    /* Incrementare numerum */
    /* Reddere indicem ad elementum novum */
}
```

**CARTER:** You add a citizen. The system finds which segment has room. If the current segment is full, it allocates a new one — a new mile of road. The citizen goes into the new segment. Every other citizen stays exactly where they were.

**MARCUS:** And the address of each citizen?

**CARTER:** Permanent. Once a citizen is entered at position forty-seven, they are *always* at position forty-seven. The pointer — the reference — never changes.

Marcus processes this. You can see it landing: the engineering elegance of stable references. In his world, a milestone that moves is useless. A census entry that moves is chaos.

**MARCUS:** *Terra firma.* Stable ground. The foundation does not shift when you build higher.

**TEAL'C:** *(from behind them)* That would make an excellent test name.

### Beat 7: The O(1) Discovery

The most technical moment. How do you find which segment holds citizen number forty-seven?

Marcus works through it at the board. He's drawn the segments with their indices. He traces his finger along them.

**MARCUS:** If I know the first section holds sixteen, and the second holds sixteen, then the thirty-third citizen is in the third section. The forty-ninth citizen is in the third section. The sixty-fourth is the last in the third section. *(he pauses)* There is a pattern.

**CARTER:** There is.

**MARCUS:** The section boundaries double. So to find which section... *(he's working it out, muttering, counting on his fingers)* ...you subtract the first two sections. Then you find which doubling contains your number.

**CARTER:** You can do that with a single operation. A bit shift.

Marcus doesn't know what a bit shift is. Carter explains — doubling and halving as left and right shifts of the digits, like moving counters on an abacus.

**MARCUS:** *(getting it)* So to find any citizen, no matter how large the census grows, the work is always the same?

**CARTER:** Always the same. Whether you have a hundred citizens or a hundred thousand.

**MARCUS:** *(slowly)* *Semper eodem labore.* Always the same effort.

This is Marcus's equivalent of his "Ah" in 1x01 and "...Good" in 1x02. The moment where the Roman engineer grasps a computer science concept through its structural truth. O(1) lookup isn't an abstraction to him — it's a guarantee that the census official who needs to find citizen number three thousand doesn't have to search any harder than the one who finds citizen number three.

### Beat 8: ACT BREAK — Teal'c Tests

Teal'c has been writing tests the entire act. Silently. Methodically. He hands Marcus a tablet.

**TEAL'C:** Your road needs inspection.

Marcus reads. The tests:

- Add one citizen. Retrieve them. `VERITAS`.
- Add sixteen citizens. Retrieve the last. `VERITAS`.
- Add seventeen citizens. Retrieve the first. The first hasn't moved. `VERITAS`.
- Add one hundred citizens. Retrieve citizen forty-seven. `VERITAS`.
- Add three thousand citizens. Retrieve citizen one. Still at the same address. `VERITAS`.
- Add three thousand and one citizens. Retrieve citizen one. *Still* at the same address. `VERITAS`.

But the last test:

- Fill the census beyond all allocated segments. What happens?

**MARCUS:** It should allocate a new section.

**TEAL'C:** Does it?

Marcus looks at Carter. Carter looks at the code. There's a boundary condition they haven't tested — the moment the sixty-fourth segment would need to be created, exceeding the maximum.

**MARCUS:** We need a limit.

**TEAL'C:** You need to decide what happens at the limit. That is the test.

Hard cut. The slate diagram of segments, growing and growing, each one larger than the last. At the top, Marcus has written in chalk: `LXIV segmenta maxima` — 64 segments maximum.

---

## ACT THREE: "Crescentia" (Growth)

*Growth without destruction. The census runs. Marcus becomes a partner.*

### Beat 9: The Iterator

The census data is in the Xar. Three thousand citizens across multiple segments. Now they need to walk through all of them — to generate the census report the senate requested.

Carter explains the naive approach: a loop with an index, calling `xar_obtinere` for each one.

Marcus shakes his head.

**MARCUS:** When I inspect a road, I do not go back to the beginning and count paces to reach each milestone. I walk the road. I remember which section I am in. When I reach the end of one section, I step onto the next.

He's describing the iterator. Walking through segments sequentially rather than recalculating position from the beginning each time.

Carter implements `xar_iterator_initium`, `xar_iterator_proximum`, `xar_iterator_finis`. The iterator remembers which segment it's in, which offset within that segment. When it reaches the end of a segment, it moves to the next one. No recalculation.

**CARTER:** It's like having a guide who walks the road with you.

**MARCUS:** It is like *being* the guide. You are the road inspector. You know where you are.

The iterator walks through all three thousand citizens. Each one retrieved in order. The structured report from 1x03 — the ChordaAedificator — now builds the full census document.

On screen, the report generates. Line by line. District by district. Family by family. Three thousand names. Including the valley settlers. Including the new district. Including everyone.

### Beat 10: Carter Steps Back

This is the character beat that defines the episode.

The census report is generating. Carter and Marcus are watching it scroll. And Carter does something she's never done in the history of the franchise. She turns to Marcus and says:

**CARTER:** I want you to review the code.

Not "I'll teach you the code." Not "Here's what the code does." *Review*. As in: find the mistakes. Test the assumptions. Tell me what's wrong.

Marcus sits down at the keyboard. For a moment, he just reads. The Latin variable names help — he can follow the logic in his own language. Pliny's educational system has worked; Marcus learned to read code from Carter, but he reads it in Latin thanks to Daniel and Lucia.

He finds something.

**MARCUS:** Here. When the segment allocates, you zero the memory. *(pointing)* But when the census is large, this is wasted work. You will write over it immediately with the citizen data. You are painting a wall white before hanging the fresco.

Carter looks. He's right. For bulk insertions — like loading the entire census at once — zeroing memory that's about to be overwritten is wasted effort.

**CARTER:** You're right. We can add a flag. *Sine zephyrum* — without zeroing.

**MARCUS:** *(a small correction, half-smiling)* *Sine zephyro.*

**DANIEL:** *(from across the room)* Actually, *sine zephyro* would mean "without the west wind." The zeroing flag should be —

**MARCUS:** *(to Daniel, dry)* Doctor Jackson. I am reviewing code. Your grammar can wait.

Carter laughs. Actually laughs. The dynamic has shifted. Marcus isn't a student reviewing his teacher's homework. He's a colleague reading his partner's code. The relationship that 1x01 seeded — Marcus's "Ah" — has become the thing Carter came here to build: Roman engineers who don't need her to tell them what to do.

### Beat 11: The Full Census

The culminating sequence. The Forum Stellarum. The same tables from the cold open, but now Carter's laptop is among the wax tablets. Not replacing them — supplementing them. The analog and digital running in parallel.

Marcus oversees both. He moves between the census officials with their styluses and Carter with her keyboard with equal fluency. He's the bridge.

Citizens approach. Names given. Data entered — into both the wax and the Xar.

A family arrives. The father gives his name.

**CENSUS OFFICIAL:** District?

**FATHER:** The valley.

The official looks at Marcus. Marcus nods. On the laptop, a new entry is added. The Xar grows. A new segment allocates — you can see it on the debug output Carter has running in a side panel. The capacity steps up. The growth is visible. And nothing that came before it moves.

More families. More entries. The Xar grows again. And again. The segments stack up like miles of road extending toward the horizon. Three thousand becomes three thousand one hundred. Three thousand one hundred becomes three thousand two hundred.

**MARCUS:** *(to Carter, quiet, as the entries scroll)* It does not forget.

**CARTER:** It doesn't forget.

**MARCUS:** And it does not move what it has already counted.

**CARTER:** Never.

**MARCUS:** *(watching the numbers)* *Crescentia sine destructione.* Growth without destruction. *(beat)* That is what we are building.

Carter nods. She doesn't say anything. The moment doesn't need her to.

### Beat 12: Pliny

Evening. The census is complete. Pliny has the final report — generated by the system, verified against the wax tablets. Every citizen counted. Every family placed.

He holds the report. Eighty years of census work. Forty volumes of the Naturalis Historia Nova. And now a machine that can do the counting in hours instead of weeks.

**PLINY:** When I was young in Rome, the census was a sacred act. The *lustrum*. Every five years, the censors would count the people, and the counting itself was a kind of prayer. You cannot govern what you do not know. You cannot protect what you have not counted.

He sets the report down on his desk, beside his manuscripts.

**PLINY:** The colony grew again today. It will grow again tomorrow. *(to Marcus)* Your ledger will grow with it?

**MARCUS:** It will grow with it.

**PLINY:** *(nodding, settling into his chair)* Then we are ready for whatever comes next.

He doesn't say "the Borg." He doesn't have to. But his eighty-year-old eyes look toward the gate, and the audience knows he's thinking about a census that might someday count not thousands but millions.

---

## TAG

### Beat 13: The Road

Night. Marcus alone on the road out of Nova Roma. The road he built. He walks it — checking the stones, as a Roman engineer does. It's a real road. Physical. Stone and gravel and labor.

Carter finds him.

**CARTER:** Can't sleep?

**MARCUS:** I am inspecting my road.

She walks beside him. They don't speak for a moment. The stars are different here — sharper, as they noticed in 1x01. The road stretches into the dark toward the valley settlements.

**CARTER:** You did good work today.

**MARCUS:** *(quiet)* It is the same work. A road is a road. Whether it carries feet or... data. The principles are the same. Build in sections. Extend without destroying. Know the capacity before you build.

**CARTER:** On Earth, we forgot those principles. We built systems that tear themselves apart to grow. It seemed easier at the time.

**MARCUS:** A road that tears itself apart is not a road. It is a promise broken and remade. Eventually, no one trusts it.

Carter stops walking. She's heard something important. Not about data structures — about trust. About why stable references matter. About why it matters that citizen number one is *always* at position one.

**CARTER:** Marcus. The thing you built today — the growth pattern, the segments, the stable addresses — we need that for everything we're going to build next. The lookup tables. The parsers. The network protocols. Everything grows. Everything needs to grow without breaking.

**MARCUS:** *(looking at his road)* Then we will build more roads.

**CARTER:** Together.

**MARCUS:** *(beat)* Together.

They walk back toward Nova Roma. Behind them, the road stretches into the dark — solid, unmoving, ready for whatever traffic comes.

FADE TO BLACK.

---

## ON-SCREEN CODE MOMENTS

The episode should feature these specific code moments, each tied to a dramatic beat:

### Moment 1: The Fixed Array Failure (Act 1)
```c
/* Modus vetus — tabula fixa */
Civis census[MMM]; /* Exacte MMM cives */
/* ...et civis MMM et I? */
```
Marcus's question — "And the three thousand and first?" — makes the limitation visceral.

### Moment 2: The First Xar (Act 1)
```c
Xar* census = xar_creare(piscina, magnitudo(Civis));
```
Simple creation. Marcus chooses the initial size. The name is born.

### Moment 3: Adding the Seventeenth Citizen (Act 1 Break)
```c
Civis* civis = (Civis*)xar_addere(census);
civis->nomen = chorda_ex_literis(piscina, "Gaius Tertius");
civis->districtus = XIII; /* Novus districtus! */
```
The seventeenth entry that triggers the first segment allocation. Nothing moves.

### Moment 4: The Segment Growth Diagram (Act 2)
```
Segmentum 0: [  XVI  ] indices 0-15
Segmentum 1: [  XVI  ] indices 16-31
Segmentum 2: [ XXXII ] indices 32-63
Segmentum 3: [ LXIV  ] indices 64-127
```
This should be a visual — Marcus's chalk drawing on the slate, mirrored by Carter's ASCII art on screen.

### Moment 5: The Lookup (Act 2)
```c
Civis* civis = (Civis*)xar_obtinere(census, XLVII);
```
One line. Instant. O(1). Citizen forty-seven, no matter how large the census grows.

### Moment 6: The Iterator Walk (Act 3)
```c
XarIterator iter = xar_iterator_initium(census);
dum (!xar_iterator_finis(&iter))
{
    Civis* civis = (Civis*)xar_iterator_proximum(&iter);
    aedificator_addere_civem(aed, civis);
}
```
The road inspector walking the road. Each citizen visited in order. The report builds.

### Moment 7: The Flag Discovery (Act 3)
```c
Xar* census = xar_creare_cum_vexillis(
    piscina,
    magnitudo(Civis),
    XVI,
    XAR_VEXILLUM_SINE_ZEPHYRUM  /* Marcus: "sine zephyro" */
);
```
Marcus's optimization. Don't paint the wall before the fresco.

---

## THE ROMAN ROADS ANALOGY — COMPLETE MAPPING

This is the thematic spine. Every Xar concept maps to Roman road engineering:

| Xar Concept | Roman Road Analogy | Dramatic Moment |
|-------------|-------------------|-----------------|
| Segments | Sections of road | Marcus: "I pave that mile" |
| Growth (new segment) | Extending the road | "I do not tear up the first mile" |
| Doubling capacity | Wider sections for more traffic | "The further you go, the more traffic" |
| Stable pointers | Milestones don't move | "Every reference to the old location?" |
| O(1) lookup | Road inspector knows the sections | "Always the same effort" |
| Iterator | Walking the road sequentially | "I do not go back to the beginning" |
| `xar_addere` | Laying a new stone at the road's end | The seventeenth citizen |
| `xar_obtinere` | Finding a specific milestone | Citizen forty-seven |
| First two segments same size | Bridge ends that must match | "Like the two ends of a bridge" |
| 64-segment maximum | Every road has a terminus | "We need a limit" |
| Arena allocation (piscina) | The road exists within the territory | Everything comes from the pool |
| `xar_vacare` (clear without freeing) | Road cleared for new traffic | Segments stay, count resets |

---

## MARCUS'S CHARACTER ARC

This is the episode where Marcus goes from *student* to *partner*. The progression across four episodes:

**1x01:** Marcus asks questions. Carter explains. He has the "Ah" moment — understanding arena allocation through the hypocaust metaphor. He is *learning*.

**1x02:** Marcus works with Teal'c on testing. He experiences failure as information — "...Good." He is *internalizing*.

**1x03:** Marcus validates Carter's design choices. His instinct confirms that chorda is right, that length-prefixed is Roman. He is *recognizing*.

**1x04:** Marcus *designs*. The segmented growth pattern is his concept, from Roman engineering principles. Carter implements his ideas. She asks him to review her code. He finds a real optimization. He is *leading*.

The transition should feel earned because it IS earned — three episodes of foundation work. Carter doesn't hand Marcus the keys because she's generous. She hands them over because he's demonstrated that his engineering instincts produce better designs than Earth's conventions.

The key dramatic moment is when Carter says "How would *you* build this?" — the question that turns a student into a colleague. And the moment Marcus says to Daniel "I am reviewing code. Your grammar can wait" — the humor of a man who has fully entered his professional domain.

---

## THEMATIC CORE: GROWTH WITHOUT DESTRUCTION

**"Crescentia sine destructione."**

This is the line. Marcus says it watching the census scroll. It means "growth without destruction." It's the episode's thesis, and it operates on multiple levels:

**Technical:** The Xar grows by adding segments, never by copying or moving existing data. Growth doesn't destroy what came before. This is the fundamental difference from realloc-based dynamic arrays.

**Civilizational:** Nova Roma grows by adding new districts, new families, new settlements. The colony doesn't destroy its existing structure to accommodate growth. The valley settlers are added; the original twelve districts remain.

**Character:** Marcus grows from student to partner. His growth doesn't diminish Carter — she's not replaced, she's amplified. The same "wings" metaphor Pliny used in 1x03, now applied to Carter's relationship with Marcus.

**Series:** The team's capabilities grow episode by episode. Each library builds on the last without invalidating it. Piscina supports Chorda supports Xar. The foundation stays stable as the structure rises.

**Anti-Borg:** The Borg grow by assimilation — consuming and destroying individual identity. Nova Roma grows by extension — adding new members while preserving existing identity. The census literally counts individuals; the Borg literally erase them. *Crescentia sine destructione* vs. *crescentia per destructionem*.

---

## CONTINUITY NOTES

**From 1x01:** Piscina is the foundation. The Xar allocates from the piscina. Marcus remembers the hypocaust metaphor.

**From 1x02:** Teal'c's testing methodology. "A good test is adversarial." The Credo framework runs on every Xar function.

**From 1x03:** The ChordaAedificator generates the census report. The census entry format established in 1x03's Act 2 break is now used at full scale. Chorda stores citizen names. Lucia's lexicon is expanding.

**To 1x05 (Tabula Dispersa):** The census data needs to be searchable. Linear search through three thousand entries is too slow. Pliny needs to find a citizen by name. This motivates the hash table. The Xar holds the data; the Tabula Dispersa will index it.

**The code on screen** should use the actual Xar API from the codebase: `xar_creare`, `xar_addere`, `xar_obtinere`, `xar_iterator_initium`, `xar_iterator_proximum`, `xar_iterator_finis`, `xar_numerus`, `xar_capacitas`. The show's code should be *real* code that compiles.

---

## OPEN QUESTIONS

1. **The Civis struct.** What fields does a Nova Roma census entry contain? Name (chorda), age (i32), district (i32), occupation (chorda), family head (chorda)? The struct should be simple enough to show on screen but rich enough to feel like a real census.

2. **Data's role.** Data is still partially damaged. He contributes the mathematical formalization in Act 2. Does he have a larger moment? His "four hundred twelve species" observation from 1x03 could continue — "In my time, the most resilient data structures were segmented."

3. **The naming debate.** "Xar" is the actual codebase name. Does the episode show the naming debate? Or does Marcus coin it? Daniel and Lucia's naming process from 1x03 could repeat here.

4. **Teal'c's test suite.** How much of the testing do we show? The Act 1 and Act 2 breaks are test-driven. Should we see specific test code, or just the results scrolling — VERITAS, VERITAS, VERITAS?

5. **The O(1) explanation.** How technically deep do we go? The bit-shift explanation is elegant but might lose casual viewers. Marcus's "always the same effort" line might be sufficient.

6. **Midas.** He doesn't appear in this pitch. Does he have a moment? He's usually the comic relief. Maybe he's helping with the physical census — "I've been counted in twelve civilizations. You'd be surprised how many get it wrong."
