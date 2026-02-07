# NOVUM FUNDAMENTUM
## Episode 1x04: "Codex Tabulae Exponentialis" (The Growing Ledger)
### Beat Sheet (Draft 1 — Compiled from Writers' Room)

---

## COLD OPEN

**Location:** Nova Roma. Census day. Morning. The Forum Stellarum is transformed — tables set up in a semicircle, wax tablets stacked at each station, Roman census officials seated with styluses ready. This is a civic ritual older than the colony itself. The Romans take census *seriously*.

Citizens line up. Families. Names called out. Occupations recorded. Children counted. It's organized, efficient, ancient. Pliny oversees from a chair — eighty years old, too stiff to stand all day, but his eyes miss nothing.

Marcus moves through the operation like a site foreman. Checking tallies. Coordinating the officials. He's built roads and aqueducts for the colony; now he's managing the flow of *people* through an administrative bottleneck.

The system works. It's always worked. Until today.

A group arrives from outside the gates. A delegation from the farming settlements that have spread beyond the original city walls. Thirty families. Maybe a hundred and fifty people. They've been growing grain in the valley for two generations. They want to be counted. They want to be *citizens*.

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

Marcus walks in and does something he hasn't done before: he speaks first.

**MARCUS:** The census is not a document. It is a living record. When a child is born, the record grows. When a family moves districts, the record changes. When a trade guild adds members, the count adjusts. *(beat)* What you have built so far — the pool, the strings, the tests — these are materials. Now we need a structure that grows.

Carter looks at Casey. A micro-expression — eyebrows up, the ghost of a smile. Casey returns it. They've been waiting for this.

Carter demonstrates the fixed-array approach on Casey's air-gapped laptop. A simple C array: `Civis census[MMM];` — three thousand citizen records, preallocated. She enters test records. It works.

**CARTER:** There. Every citizen has a slot. It's fast, it's simple, it's —

**MARCUS:** Full.

He's looking at the number. Three thousand slots. Three thousand citizens. No room.

**CARTER:** Right. If we need more, we'd have to create a bigger array and copy everything over. Every record. Every entry. Move the whole census to a new location.

**MARCUS:** *(standing — he always stands when thinking)* You are telling me that every time the colony grows beyond a boundary, you destroy the existing record and rebuild it?

**CARTER:** We don't *destroy* it, we *copy* —

**MARCUS:** You tear up the road and relay every stone because you need to add one more mile.

Silence. Marcus has found the metaphor that will drive the episode.

**CASEY:** *(to Carter, quiet)* Tell him how realloc works.

**CARTER:** When you outgrow the space, you request a larger block of memory. The system finds room, copies everything to the new location —

**MARCUS:** And every reference to the old location?

**CARTER:** Invalid.

**MARCUS:** *(genuinely appalled)* This is how you have been doing it?

**CASEY:** For fifty years.

There's an echo here of 1x03 — "This is how you have been doing it?" — Marcus's recurring disbelief at Earth computing conventions. But this time it's HIS turn to propose the alternative.

### Beat 2: Marcus Takes the Lead

Carter does something she hasn't done before. She pushes her chair back from the keyboard. Not dramatically — just enough. She creates space.

**CARTER:** Marcus. How would *you* build this?

It's the first time she's asked a Roman to design a data structure from scratch. In 1x01, she explained Piscina and Marcus understood it. In 1x03, Lucia corrected Daniel and Marcus validated the chorda design. But Carter always held the keyboard. Carter always wrote the first line.

Not this time.

Marcus doesn't sit down. He walks to the window. Through it, you can see the road that leads out of Nova Roma toward the valley settlements — the road Marcus himself built five years ago.

**MARCUS:** When I build a road, I do not know how far it will go. I know where it starts. I know the first mile. I pave that mile. And when someone needs to travel further, I pave the next mile. *(turning back)* I do not tear up the first mile to make a longer road. The first mile is *finished*. It stays where it is.

**CARTER:** And if the new section needs to be wider? Higher capacity?

**MARCUS:** Then the new section is wider. The old section served its purpose. The road still connects.

He's describing a segmented array. He doesn't know the term. He's describing it from engineering principles that are two thousand years old.

Casey is watching with an expression the audience hasn't seen from him before: recognition. Not recognition of the computer science concept — recognition that the Roman got there first.

### Beat 3: The First Segment

Marcus comes to the keyboard. This is the first time a Roman has typed code in the series. Carter is beside him, translating his design into C89. But the design is his.

**MARCUS:** The census begins with space for sixteen citizens. A cohort.

**CARTER:** Why sixteen?

**MARCUS:** A *contubernium* — the smallest military unit. Eight soldiers, but sixteen with their equipment counted separately. It is a number Romans trust.

Carter types:

```c
Xar* census = xar_creare(piscina, magnitudo(Civis));
```

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

*The road extends. The design deepens. The past intrudes.*

### Beat 5: The Doubling Principle

The workshop. Marcus at the slate board. He's drawing the segment structure.

**MARCUS:** The first section: sixteen. The second section: sixteen. The same size. *(drawing)* Like the two ends of a bridge that must meet in the middle. They should match.

**CARTER:** And then?

**MARCUS:** Then the road opens up. *(he draws a wider section)* The third section: thirty-two. Double. Because the further you go, the more traffic you expect.

**CARTER:** Why double? Why not add sixteen each time?

Marcus draws a road with equal-width sections. Then another road where each section is wider.

**MARCUS:** If every section is the same width, you need many sections for a large colony. Many sections means many joints. Many joints means many places for cracks. *(tapping the second drawing)* If you double, you have fewer sections. Fewer joints. The road is stronger.

He's independently arrived at the logarithmic segment count argument. Fewer allocations, fewer boundary crossings — but he's expressing it as structural engineering.

**CASEY:** *(to Carter, under his breath)* He just reinvented the growth factor.

Carter writes the doubling rule on screen:

```
Segmentum 0: XVI    (indices 0-15)
Segmentum 1: XVI    (indices 16-31)
Segmentum 2: XXXII  (indices 32-63)
Segmentum 3: LXIV   (indices 64-127)
Segmentum 4: CXXVIII (indices 128-255)
```

**MARCUS:** *(reading)* Each section holds all the travelers that came before it. The road remembers its own growth.

**DATA:** *(from the corner, precise)* The total capacity after N segments is the sum of a geometric series. The capacity approximately doubles with each new segment.

**MARCUS:** *(to Data)* That is what I said.

**DATA:** Yes. I was providing the mathematical formalization.

**MARCUS:** *(beat, then a rare warmth)* ...Thank you.

This is the first moment Marcus addresses Data directly with something other than caution. Two engineers recognizing the same truth in different languages.

### Beat 6: Stable Ground

Carter explains what makes the Xar different from realloc. She draws two diagrams beside Marcus's road drawing. The first: a traditional dynamic array — grow, copy, invalidate. The second: the Xar — grow by adding a new segment. Nothing moves.

**CARTER:** In the old way, when you grow, everything moves. Every reference breaks. It's like extending a road by picking up every stone and relaying them in a new location. Every milestone now points to the wrong distance. Every map is wrong.

**MARCUS:** *(disgusted)* No Roman engineer would do this.

**CARTER:** No. They wouldn't. And that's the point.

She types the key function: `xar_addere`. The function that adds a citizen. The system finds which segment has room. If the current segment is full, it allocates a new one — a new mile of road. The citizen goes into the new segment. Every other citizen stays exactly where they were.

**MARCUS:** And the address of each citizen?

**CARTER:** Permanent. Once a citizen is entered at position forty-seven, they are *always* at position forty-seven.

Marcus processes this. In his world, a milestone that moves is useless. A census entry that moves is chaos.

**MARCUS:** *Terra firma.* Stable ground. The foundation does not shift when you build higher.

**TEAL'C:** *(from behind them)* That would make an excellent test name.

### Beat 7: The O(1) Discovery

The most technical moment. How do you find which segment holds citizen number forty-seven?

Marcus works through it at the board. He traces his finger along the segments and their indices.

**MARCUS:** The section boundaries double. So to find which section... *(working it out)* ...you subtract the first two sections. Then you find which doubling contains your number.

**CARTER:** You can do that with a single operation. A bit shift.

She explains — doubling and halving as left and right shifts, like moving counters on an abacus.

**MARCUS:** *(getting it)* So to find any citizen, no matter how large the census grows, the work is always the same?

**CARTER:** Always the same. Whether you have a hundred citizens or a hundred thousand.

**MARCUS:** *(slowly)* *Semper eodem labore.* Always the same effort.

Marcus's equivalent of his "Ah" in 1x01 and "...Good" in 1x02. O(1) lookup isn't an abstraction to him — it's a guarantee that the census official who needs to find citizen number three thousand doesn't have to search any harder than the one who finds citizen number three.

### Beat 8: Duncan's Cathedral

**INTERCUT — FLASHBACK:**

**Location:** Cologne, Germany. ~1630s. Autumn.

Duncan MacLeod stands at the edge of a construction site. Above him, the skeleton of Cologne Cathedral — begun in 1248, not finished, not close to finished. The south tower stands at half-height. It's been that way for nearly two hundred years. The money ran out. The will persisted. Masons work on the nave, stone by stone, with no guarantee anyone alive will see it completed.

Duncan wears a sword. Nobody else does.

A master mason directs a team lifting a keystone into a window arch. The stone settles. He checks the line with a plumb bob. Adjusts. Checks again. Nods. Moves on. His hands are calloused, precise. His entire world is the stone and the line and the truth of the plumb.

A young apprentice, mixing mortar in a trough, sees Duncan's sword.

**APPRENTICE:** You're not a mason.

**DUNCAN:** No.

**APPRENTICE:** The master says the nave won't be finished for another thirty years. His son will finish it. Maybe his grandson.

**DUNCAN:** And that doesn't bother him?

**APPRENTICE:** *(genuinely confused)* Why would it?

Duncan watches. He's fascinated. He's restless. There's a conflict in Aquitaine. Someone needs a sword. Someone always needs a sword.

**CUT TO:** Duncan packing a saddlebag in a dim room. Dawn light through a rough window. He pauses. Looks out at the scaffolding. The cathedral isn't finished. Not even close. The workers are already stirring — another day, another course of stone.

He rides out before the sun clears the hills.

**QUICK MONTAGE (30 seconds, wordless):**
- A different town, different century. A bridge being built. Duncan watches from horseback. Rides on.
- A monastery. Monks copying manuscripts. Duncan in the cloister, reading over a monk's shoulder. He's interested. He's already packed.
- A ship under construction, ribs of the hull like a cathedral inverted. Duncan on the dock. Looking back as he boards a different ship.

**DUNCAN (V.O.):** Four hundred years. I could have watched a thousand things get built. Instead I watched a thousand things get threatened, and I drew my sword.

**SMASH CUT TO:** Pliny's workshop. Present day. Marcus is drawing on a wax tablet — the Xar growth patterns, in clean Roman engineering notation, with structural annotations in a shorthand that Carter recognizes as load-bearing analysis. The way a Roman engineer would annotate an aqueduct plan.

Duncan is in the doorway. He came to ask a logistical question. But he stopped. He's watching Marcus's hands. The stylus on the wax tablet. The same focused precision as the master mason four hundred years ago.

But this time, Duncan doesn't walk away.

He leans against the doorframe. Crosses his arms. Stays.

No dialogue marks the moment. The camera holds on Duncan in the doorway, then pulls back to include Marcus's drawing hand in the foreground and Duncan's still figure in the background. The composition rhymes exactly with the flashback: a builder in focus, a swordsman watching. Except this time, the swordsman isn't leaving.

**DIRECTION NOTE:** Five visual rhymes connect the flashback to the present:
1. The master mason's plumb line → Marcus's straight-edge on the wax tablet
2. The scaffolding of the cathedral → the indentation structure of the code on screen
3. Duncan's hand on his sword pommel (flashback) → Duncan's arms crossed (present) — same readiness, different purpose
4. The apprentice's mortar trough → the Piscina allocation output on screen
5. Duncan riding away at dawn → Duncan leaning in the doorway at dusk — same framing, opposite motion

### Beat 9: The Rosary and the Census

Late afternoon. Marcus and Casey are at a workbench together. Marcus scratching census categories onto a wax tablet — family name, trade, district, household members — while Casey enters them into the Xar as test data. They're working in tandem, the way a foreman and a surveyor work.

Casey reaches into his jacket pocket for a stylus. As his hand comes out, something catches — a loop of dark wooden beads, briefly visible, tangled with the stylus. Casey untangles them without looking, the way you'd untangle keys from a lanyard. A habitual motion. He tucks them back.

Marcus's eyes follow the beads. He's been counting all day.

**MARCUS:** Your beads. What are they for?

**CASEY:** They help me count.

**MARCUS:** *(the census-taker, immediately interested)* Count what?

**CASEY:** Prayers.

**MARCUS:** How many prayers?

**CASEY:** Enough.

**MARCUS:** *(nodding slowly)* A man who counts knows the value of enough.

He goes back to his work. Casey watches him for a moment — something like recognition in his expression. Two men who count things. Different things. The same discipline.

### Beat 10: ACT BREAK — Teal'c's Test Suite

Teal'c has been writing tests the entire act. Silently. Methodically. He hands Marcus a tablet.

**TEAL'C:** Your road needs inspection.

Marcus reads. The tests:

- Add one citizen. Retrieve them. `VERITAS`.
- Add sixteen citizens. Retrieve the last. `VERITAS`.
- Add seventeen citizens. Retrieve the first. The first hasn't moved. `VERITAS`.
- Add one hundred citizens. Retrieve citizen forty-seven. `VERITAS`.
- Add three thousand citizens. Retrieve citizen one. Still at the same address. `VERITAS`.
- Add three thousand and one. Retrieve citizen one. *Still* at the same address. `VERITAS`.

But the last test:

- Fill the census beyond all allocated segments. What happens?

**MARCUS:** It should allocate a new section.

**TEAL'C:** Does it?

There's a boundary condition they haven't tested — the maximum segment count.

**MARCUS:** We need a limit.

**TEAL'C:** You need to decide what happens at the limit. That is the test.

Hard cut. The slate diagram of segments, growing and growing. At the top, Marcus has written in chalk: `LXIV segmenta maxima` — 64 segments maximum.

---

## ACT THREE: "Crescentia" (Growth)

*Growth without destruction. The census runs. Marcus becomes a partner.*

### Beat 11: The Iterator

Three thousand citizens across multiple segments. They need to walk through all of them to generate the census report.

Carter explains the naive approach: a loop with an index, calling `xar_obtinere` for each one.

Marcus shakes his head.

**MARCUS:** When I inspect a road, I do not go back to the beginning and count paces to reach each milestone. I walk the road. I remember which section I am in. When I reach the end of one section, I step onto the next.

He's describing the iterator. Carter implements `xar_iterator_initium`, `xar_iterator_proximum`, `xar_iterator_finis`. The iterator remembers which segment it's in, which offset. When it reaches the end of a segment, it moves to the next one.

**CARTER:** It's like having a guide who walks the road with you.

**MARCUS:** It is like *being* the guide. You are the road inspector. You know where you are.

The iterator walks through all three thousand citizens. The structured report from 1x03 — the ChordaAedificator — builds the full census document. On screen, the report generates. Line by line. District by district. Family by family.

### Beat 12: Carter Steps Back

Carter turns the screen toward Marcus.

**CARTER:** I want you to review the code.

Not "I'll teach you the code." Not "Here's what the code does." *Review*. As in: find the mistakes.

Marcus sits down at the keyboard. He reads. The Latin variable names help — he can follow the logic in his own language.

He finds something.

**MARCUS:** Here. When the segment allocates, you zero the memory. But when the census is large, this is wasted work. You will write over it immediately with the citizen data. You are painting a wall white before hanging the fresco.

Carter looks. He's right. For bulk insertions, zeroing memory that's about to be overwritten is wasted effort.

**CARTER:** You're right. We can add a flag. *Sine zephyrum* — without zeroing.

**MARCUS:** *(a small correction, half-smiling)* *Sine zephyro.*

**DANIEL:** *(from across the room)* Actually, *sine zephyro* would mean "without the west wind." The zeroing flag should be —

**MARCUS:** *(to Daniel, dry)* Doctor Jackson. I am reviewing code. Your grammar can wait.

Carter actually laughs. The dynamic has shifted. Marcus isn't a student reviewing his teacher's homework. He's a colleague reading his partner's code.

### Beat 13: Data and Midas — The Census Test

Data and Midas volunteer as the first test entries in the Xar-based census. A Roman Immortal and a Starfleet android — both anomalies, both uncategorizable by normal standards.

**MIDAS:** *(watching his entry appear on screen)* "Midas. Age: three thousand, approximately. Trade: historical consultant. District: Forum." *(to Data)* They made me sound boring.

**DATA:** Accuracy and interest are not correlated.

**MIDAS:** You are the most honest person I've ever met, and I've met Aristotle.

**DATA:** I met Captain Picard. I believe the experiences are comparable.

**MIDAS:** *(delighted)* Data, that might be the first joke you've told.

**DATA:** It was not a joke. *(pause)* Although I acknowledge the structural similarity.

When Data's entry comes up, Marcus pauses. A genuine Roman legal question: what is Data's status? Citizen? Visitor? Tool? In Roman law, categories matter — *civis*, *socius*, *peregrinus*.

Midas solves it immediately.

**MIDAS:** He is *peregrinus amicus*. A foreign friend. He is here. He contributes. He is counted.

Marcus considers this. Nods. Writes it into the census. "The Measure of a Man" resolved in thirty seconds by Roman legal categories — not through a trial, but through a civilization that takes classification seriously.

### Beat 14: Teal'c's Census Moment

Teal'c reviews the final test results. Every test passes. VERITAS, VERITAS, VERITAS. He reads the census printout — three thousand citizens, each one individually enumerated, each one a person.

A quiet beat, almost throwaway:

**TEAL'C:** On Chulak, the Goa'uld counted us. We were not citizens. We were inventory.

Beat.

**TEAL'C:** These tests verify that every person in the census is counted *as a person*. That is... significant.

Marcus nods. He doesn't know the full weight of what Teal'c just said. But he respects the tone. A freed slave ensuring the count is honest — the most emotionally devastating beat in the episode, and it's delivered in Teal'c's matter-of-fact register.

### Beat 15: O'Neill's Question

O'Neill has been sitting in the back. He watches his own entry appear on screen.

**O'NEILL:** "Jack O'Neill. Age: classified. Trade: military consultant. District: Gate precinct." *(beat)* "Military consultant." That's generous.

**MARCUS:** What would you prefer?

**O'NEILL:** "Retired general who keeps getting un-retired against his expressed wishes." But that might not fit in the field.

**MARCUS:** The field grows to accommodate.

**O'NEILL:** *(looking at the screen)* That's actually pretty good.

Then, quieter:

**O'NEILL:** What happens when someone dies?

Everyone stops. Because the Xar only grows. You can add. But what do you do when a citizen dies? In Roman tradition, the *infamia* — the mark against a citizen's record — was the most feared punishment. Erasing someone from the census was a kind of civic death.

O'Neill doesn't know any of this. He's thinking about the war. People die in wars.

**PLINY:** *(from his chair)* In Rome, the dead remain in the census. They are marked, not erased. The record remembers they were here.

**O'NEILL:** *(beat)* Good.

O'Neill's question opens the philosophical door: a census that never erases is a census that honors the dead. The Xar, which never moves its elements, becomes a kind of memorial — every citizen remains at their original position, even after death. The data structure IS the philosophy.

### Beat 16: The Full Census

The Forum Stellarum. The same tables from the cold open, but now Carter's laptop is among the wax tablets. Not replacing them — supplementing. Analog and digital running in parallel.

Marcus oversees both. He moves between the census officials and Carter's keyboard with equal fluency.

The valley settlers approach. The father gives his name.

**CENSUS OFFICIAL:** District?

**FATHER:** The valley.

Marcus nods. On the laptop, a new entry is added. The Xar grows. A new segment allocates — visible on the debug output. The capacity steps up. Nothing that came before it moves.

More families. More entries. Three thousand becomes three thousand one hundred. Three thousand two hundred.

**MARCUS:** *(quiet, watching)* It does not forget.

**CARTER:** It doesn't forget.

**MARCUS:** And it does not move what it has already counted.

**CARTER:** Never.

**MARCUS:** *(watching the numbers)* *Crescentia sine destructione.* Growth without destruction. *(beat)* That is what we are building.

### Beat 17: Pliny

Evening. The census is complete. Pliny has the final report — generated by the system, verified against the wax tablets. Every citizen counted.

**PLINY:** For eighty years, every name in Nova Roma passed through my hand. I held each one. Weighed the ink. Knew the family. Saw the face of the father who came to register.

Pause.

**PLINY:** This is better. A census that depends on one old man's hand is a census that dies when the old man does. A census that grows on its own... that lives. *(to Marcus)* You have built something that will outlast me. That is the kindest thing an engineer can do for a scholar.

Then, softer:

**PLINY:** You will make a good senator someday, Marcus.

**MARCUS:** *(surprised)* I am an engineer.

**PLINY:** So was the man who built the first aqueduct. They made him consul.

The S3 election arc begins here, in a throwaway comment from a dying polymath who sees further than anyone.

### Beat 18: Marcus Presents to the Senate

The Nova Roma senate chamber. Stone seats in a semicircle. Oil lamps. Marcus stands alone before the senators. Carter is in the gallery. She doesn't speak.

**MARCUS:** We count ourselves because a republic that does not know its people cannot serve its people. This is not new. Rome has counted since the days of Servius Tullius. What is new is the ledger.

*(He gestures to the screen.)*

A stone tablet holds what you carve. A scroll holds what you write. But the colony grows. Families form. Children are born. The old ledger does not grow with us. It is a portrait, not a mirror. What we have built is a mirror. It shows us as we are — now, today. When we change, it changes. When we grow, it grows. Not by erasing what was, but by adding what is.

*(Silence. A senator nods. Then another.)*

Carter, in the gallery, watches Marcus handle a senator's technical question — without looking to her for confirmation.

**CASEY:** *(quietly, to Carter)* He doesn't need us anymore.

**CARTER:** *(watching Marcus)* He needs us less. That's not the same thing. But it's the goal.

---

## TAG

### Beat 19: Duncan and Marcus

Evening. After the senate session. Marcus alone, organizing tablets. Duncan finds him.

**MARCUS:** You were watching today. In the gallery.

**DUNCAN:** I was.

**MARCUS:** You watch often. Like a man waiting for something.

**DUNCAN:** *(long pause)* I've spent a long time watching things get built. I usually leave before they're finished.

**MARCUS:** *(matter-of-fact)* Then don't leave.

Duncan looks at him. Marcus isn't being philosophical. He's being Roman — direct, practical, unsentimentally honest. *Don't leave* isn't an emotional plea. It's engineering advice. If the structure requires your presence, stay.

**DUNCAN:** *(quietly)* I'm working on it.

### Beat 20: The Road

Night. Marcus alone on the road out of Nova Roma. The road he built — physical, stone and gravel and labor. He walks it, checking the stones, as a Roman engineer does.

Carter finds him.

**CARTER:** Can't sleep?

**MARCUS:** I am inspecting my road.

She walks beside him. The stars are different here — sharper, as they noticed in 1x01.

**CARTER:** You did good work today.

**MARCUS:** It is the same work. A road is a road. Whether it carries feet or data. The principles are the same. Build in sections. Extend without destroying. Know the capacity before you build.

**CARTER:** On Earth, we forgot those principles.

**MARCUS:** A road that tears itself apart is not a road. It is a promise broken and remade. Eventually, no one trusts it.

Carter stops walking. She's heard something important. Not about data structures — about trust. About why stable references matter. About why it matters that citizen number one is *always* at position one.

**CARTER:** Marcus. The thing you built today — the growth pattern, the segments, the stable addresses — we need that for everything we're going to build next. Everything grows. Everything needs to grow without breaking.

**MARCUS:** Then we will build more roads.

**CARTER:** Together.

**MARCUS:** *(beat)* Together.

They walk back toward Nova Roma. Behind them, the road stretches into the dark — solid, unmoving, ready for whatever traffic comes.

FADE TO BLACK.

---

## STINGER

### Beat 21: Kirk's Ledger

**Location:** Earth. Night. A strip mall parking lot somewhere in the Pacific Northwest. UPS Store. Nail salon. Subway. At the end: an unmarked office with blacked-out windows.

Kirk walks across the parking lot. Suit. No hurry. A man who belongs wherever he decides to belong.

Inside: three people at workstations. They don't look up. Already working. The green glow of their monitors — slightly wrong, slightly too organic, all the same shade.

Kirk walks past them to a whiteboard on the far wall. A map. Pins — four of them now. Portland (1x03). Seattle. San Francisco. And now this location. Lines drawn between them.

He adds the fourth pin. Steps back. Looks at the map the way Marcus looks at a census tablet: assessing what he has, calculating what he needs.

**KIRK:** *(to the room, without turning)* The Portland relay is stable?

**VOICE:** Stable. Sixty-three hosts.

**KIRK:** San Francisco?

**VOICE:** One hundred eleven.

Kirk nods. He traces the lines between pins. The network is growing. Each node independent. Each node intact. None destroyed to build the next.

His Xar, so to speak.

He writes a number on the whiteboard: **4 / 31**. Four nodes active. Twenty-seven to go.

He caps the marker. The camera holds on the map. Four pins. Four segments of a growing array. The ledger expands.

SMASH TO BLACK.

END OF EPISODE

---

## C-PLOT THREADING

**Beat C1 (Early Act 1):** Duncan, during a morning briefing, reports: two tech workers in Seattle have gone off-grid. Not dead — just gone. Social media silent. Phones off. Apartments abandoned. Their employer says they resigned by email on the same day, using identical language.

**DUNCAN:** Identical resignation letters. Word for word.

**O'NEILL:** Maybe they used the same template.

**DUNCAN:** *(flat)* Maybe.

Kirk is recruiting. The characters suspect it. Kirk's roster is expanding.

**Beat C2 (Mid Act 2):** Carter intercepts another brief Borg-encoded transmission — the third since 1x03's Oregon signal. This one from a commercial data center in San Jose. The signal was distributed across forty servers, each contributing a fragment.

**CARTER:** He's not using single points anymore. The signal was distributed. Forty sources, one message.

**CASEY:** *(quiet)* He's building redundancy.

Kirk's network, like the Xar, distributes across segments. If you destroy one server, the others continue.

**Beat C3 (Late Act 2):** O'Neill pulls Casey aside.

**O'NEILL:** How many people know about the gate location?

**CASEY:** Our team. The Romans. Midas. That's it.

**O'NEILL:** Kirk's got nodes in Portland, Seattle, San Francisco. He's building infrastructure. He's recruiting people. That's not random growth. That's an approach vector.

O'Neill applying census logic to threat assessment — counting what Kirk has, projecting what he needs.

**Beat C4 (After the senate presentation):** Duncan standing on the walls at dusk. Methos finds him.

**METHOS:** The census went well.

**DUNCAN:** Three thousand people. *(beat)* Every one of them a target now.

**METHOS:** They were targets before we counted them.

**DUNCAN:** But now we know how many. And so does Kirk, if he's paying attention. A census tells you what you have to protect. It also tells an enemy what you have to lose.

**METHOS:** *(after a moment)* That's always been the problem with writing things down.

This plants the seed for 1x18 (Arcanum/TLS) — the census must eventually be encrypted because it's the first thing worth stealing.

---

## THEMATIC NOTES

**"Codex Tabulae Exponentialis"** shifts meaning across the episode:
- **Act 1:** "The Growing Ledger" = the census. Marcus's practical problem. Nova Roma has outgrown its record-keeping. Literal meaning: a book of records that can expand.
- **Act 2:** "The Growing Ledger" = the Xar. A data structure that grows without destroying what came before. Technical meaning: a table that doubles its capacity exponentially.
- **Act 3:** "The Growing Ledger" = the show itself. The audience is accumulating characters, technologies, threats. The show is a Xar of episodes. Nothing is discarded. Everything compounds.
- **Stinger:** "The Growing Ledger" = Kirk's ledger. 4/31. The enemy counts too.

**Growth Without Destruction — The Unifying Principle:**
- **Computational:** The Xar adds segments. Existing data never moves. Indices stay stable.
- **Biological:** Nova Roma's population grows. New districts, new families. No displacement.
- **Architectural:** Roman roads extend by building more road. You never tear up what's behind you.
- **Personal (Marcus):** Student in 1x01, collaborator in 1x03, leader in 1x04. Growth that doesn't erase who he was.
- **Personal (Duncan):** The man who fought for four hundred years learning to stay and watch things get built. Growth that doesn't require sheathing the sword.
- **Civilizational:** Each library builds on the last without invalidating it. Piscina → Chorda → Xar.
- **Anti-Borg:** The Borg grow by assimilation — consuming identity. Nova Roma grows by extension — preserving it. The census counts individuals; the Borg erase them.
- **Dark Mirror:** Kirk's network grows the same way — nodes added, none destroyed. Same mechanic, opposite purpose.

**Moderate Realism and the Nature of Collections:**
The Xar knows its own size (`xar_numerus`). It has intrinsic properties — it doesn't need external scanning to know what it contains. The realist position: things have natures. Contrast with null-terminated strings (nominalist — you discover the length by scanning for absence) and with Kirk's census of nodes-as-resources (nominalist — people counted as things to exploit, not persons to serve).

**The Census and Luke 2:**
*"In those days Caesar Augustus issued a decree that a census should be taken of the entire Roman world."* A census that brought everyone to the right place at the right time. The show doesn't need to say this. But a Roman census, on a world reached through a gate, where something new is being born — the resonance is there for anyone who finds it. Pliny might note in passing that census records have always preceded great changes.

**The False Positive Continued (Kirk):**
In 1x02, Kirk was the false positive — passed every test, was the threat. In 1x04, his network is the dark Xar — growing the same way, using the same principles of non-destructive addition. The subreddit will overlay Kirk's whiteboard map with Marcus's segment diagram.

**Genre Architecture:**
- **SG-1 "2010" Inversion:** The Aschen census was a kill list. Marcus's census is a foundation. The Xar can't erase anyone — you can only add. The anti-Aschen.
- **TNG "The Measure of a Man" Resolution:** Data classified as *peregrinus amicus* in thirty seconds by Roman legal categories. No trial needed.
- **"Window of Opportunity" Teal'c:** Teal'c learned Latin during the time loops. He's been reading code comments the entire time. He never says anything about it.
- **Highlander "Homeland" Echo:** Duncan's pattern of leaving before things are finished. The cathedral he walked away from. The colony he stays for.
- **Cologne Cathedral:** Started 1248. Unfinished until 1880. 632 years. The incomplete tower that waited four centuries for someone to come back. That's the show in one building.
- **Foundation (Asimov):** The census is the first brick of the Encyclopedia. Marcus is Seldon — building the tool the future needs without knowing what the future looks like.
- **Teal'c as freed slave:** A former Jaffa — counted as inventory by the Goa'uld — now testing a census that counts free citizens. The most quietly devastating beat in the episode.

**Connection to the Series:**
- *Crescentia sine destructione* — growth without destruction. The episode's thesis and the show's engineering philosophy.
- Marcus's senate presentation seeds his S3 election arc. Pliny: "You will make a good senator someday."
- Pliny's graceful ceding of the census foreshadows his S3 death — a man peacefully making himself unnecessary.
- Duncan's cathedral fragment is deliberately incomplete. The full Florence/Brunelleschi flashback is deferred to S2, where it will land harder because the audience knows Duncan stayed.
- Teal'c's dangling pointer question (Beat 5, via Teal'c's observation about old maps) is not resolved. It runs through the series.

---

## ADDITIONAL NOTES

**Methos's One Contribution:**

Methos drifts through the workshop once, catches the tail of Marcus's road metaphor:

**METHOS:** The Romans built fifty thousand miles of road. Do you know what happened to most of them?

**MARCUS:** They lasted.

**METHOS:** They lasted because they were *maintained*. Every road had a curator. Someone responsible for its upkeep. Your growing array — does it have a curator? *(he takes a sip of wine)* Or do you assume it will maintain itself?

He goes back to his wine. One question. Delivered with absolute casualness. The maintenance/stewardship question — who owns the data structure over time? — is a thread the team will be answering for the rest of the series.

**Daniel and Lucia — The Naming:**

**LUCIA:** *Xar*. You propose this as the name?

**DANIEL:** It's Carter's working term.

**LUCIA:** What does it mean?

**DANIEL:** It's an abbreviation.

**LUCIA:** *(distaste)* Acronyms are the death of language. What does the thing DO?

**DANIEL:** It grows. It holds things. More things than you expected.

**LUCIA:** *Xar* is ugly but honest. It tells you nothing and hides nothing. Perhaps that is the right quality for a container that holds anything. *(she writes in her lexicon)* *Xar: receptaculum longitudine variabili.* A container of variable length. Your acronym is now a Latin word. You're welcome.

**The Cross-Franchise Algorithm Moment:**

Three franchise representatives describing the same growth pattern through different metaphors:

Marcus: cohorts — "Each cohort self-contained, additional cohorts added as the legion grows."
Data: "This is remarkably similar to the memory architecture of a positronic brain."
Casey: "It's how I laid out the pastures on my ranch."

Same algorithm. Three translations. All correct.

---

## OPEN QUESTIONS

1. **The Civis struct.** What fields does a Nova Roma census entry contain? Name (chorda), age (i32), district (i32), occupation (chorda), family (chorda)? Simple enough to show on screen, rich enough to feel real.

2. **Duncan's flashback length.** The cathedral sequence plus montage should be 90-120 seconds maximum. The brevity IS the point — Duncan can recall building but can't dwell in it. Not yet.

3. **The "4/31" number.** 31 major data centers? 31 Stargate addresses Kirk has identified? The specificity is what matters — Kirk has a plan with a scope. The audience can speculate.

4. **Midas at the physical census.** An opportunity for Midas comedy: "I've been counted in twelve civilizations. You'd be surprised how many get it wrong."

5. **O'Neill's death question.** Is it technically accurate? The Xar doesn't have a deletion mechanism — it grows but doesn't shrink. Pliny's answer (mark, don't erase) is the Roman legal position. Does this map to the actual codebase?

6. **Teal'c's Latin.** If we commit to Teal'c having learned Latin during the "Window of Opportunity" time loops, this is a series-long background detail. He's been reading code comments since 1x01. When do we reveal it? Or do we never explicitly reveal it and let the subreddit figure it out?
