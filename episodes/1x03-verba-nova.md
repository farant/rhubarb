# NOVUM FUNDAMENTUM
## Episode 1x03: "Verba Nova"
### Beat Sheet (Draft 1 — Compiled from Writers' Room)

---

## COLD OPEN

**Location:** Nova Roma. Pliny's villa. Morning. Three days after 1x02.

Daniel is proud. He's been working all night. He presents a document to Lucia Serena and Pliny — a glossary of computing terms translated into Latin. Beautiful calligraphy. Methodical. He clearly put his heart into it.

Lucia reads. Her expression changes from polite interest to something colder. She holds a stylus in her right hand — her thinking prop. She taps it against her palm twice, then sets it down. Decision made.

**LUCIA:** *Textilis.* You've translated "string" as *textilis*. Woven fabric.

**DANIEL:** It's the closest Latin equivalent. Threads of text woven together —

**LUCIA:** When you weave fabric, you cannot see the individual threads. They disappear into the cloth. Is that what your "strings" do?

**DANIEL:** Well, no. Each character remains distinct —

**LUCIA:** *(setting the scroll down)* Then this is wrong.

Pliny watches. He's seen this dynamic before — between Roman scholars and foreign ones. He says nothing. This is Lucia's arena.

**LUCIA:** You have given us the *sound* of your ideas in our language. You have not given us the *ideas*. *(beat)* You are putting your thoughts in our words. That is not language. That is costume.

Daniel opens his mouth. Closes it. His palms go flat on the table — grounding himself. She's right.

**LUCIA:** *(softer, but precise)* We have a word. *Chorda*. The string of a lyre. Each note distinct. The length known before you play. *(she produces an actual lyre string from her teaching materials, holds it between her hands)* When a Roman picks up a cord, what does he know first?

**DANIEL:** ...The length.

**LUCIA:** He holds both ends. He does not run his hand along it to find the other end. A musician *knows* her instrument. *(to Carter, who has been listening from the doorway)* Is that what your system needs? A sequence of known length?

**CARTER:** *(surprised)* Yes. Exactly that.

**LUCIA:** Then let us begin again. With *our* words. Not yours in disguise.

SMASH TO MAIN TITLES.

---

## ACT ONE: "Verba" (Words)

*The words themselves. What do we call things, and why does it matter?*

### Beat 1: The Naming Problem

Carter's workshop, Pliny's villa. The Piscina and Credo framework are working. Carter and Casey are building the next layer: text handling. They need strings.

Carter explains the Earth convention to the room: null-terminated strings. A sequence of characters followed by a special marker — the null byte, the zero — that says "stop here."

Marcus frowns. He's standing — Roman engineers don't work sitting down.

**MARCUS:** How do you know how long it is?

**CARTER:** You walk the string until you find the null terminator.

**MARCUS:** You walk the *entire* string? Every time?

**CARTER:** ...Yes.

**MARCUS:** That is like measuring an aqueduct by walking its full length every time someone asks how far the water travels. We know how far it goes. *We built it.* Why would we forget?

Casey, in the corner, almost laughs. This is the argument he's been having with C programmers for decades.

**MARCUS:** And what if there is no marker at the end? What if someone forgets to place the seal?

**CARTER:** Then you run past the end. Into other memory. You read garbage. You corrupt things you didn't mean to touch.

**MARCUS:** *(incredulous)* This is how you have been doing it?

**CASEY:** *(to Carter, quietly)* For fifty years.

### Beat 2: Chorda Takes Shape

Lucia joins the technical conversation, bringing the lyre-string metaphor into direct contact with the implementation.

Carter writes the chorda struct:

```c
structura chorda {
    i32  mensura;
     i8* datum;
};
```

**MARCUS:** Mensura first. You always count the stones before you lay them.

**CARTER:** The length is carried with the data. The string knows itself. No scanning. No terminator.

**LUCIA:** *(to Daniel)* *Chorda*. Not *textilis*. Not *catena*. A cord has a known length. The metaphor teaches the concept.

Daniel nods. He's not defending anymore — his head tilts, listening. Something is shifting in him.

**PLINY:** *(watching the struct on screen)* Mensura et datum. The measure and the substance. That is how we catalog everything in the Naturalis Historia. First the extent, then the content. *(to Carter)* Your instinct is already Roman.

**CARTER:** I think the Romans' instinct is already better than ours.

Carter explains the operations: creating a chorda from text (`chorda_ex_literis`), slicing without copying (`chorda_sectio`). Key moment on slicing:

**CARTER:** A slice is a *view*. It points to the same data. It doesn't copy.

**MARCUS:** An architect's drawing of a wall is not a second wall. It is a view of the same wall. If the wall changes, the drawing is wrong.

**CARTER:** That's exactly right. And that's the trade-off.

**MARCUS:** Then the architect must know which drawings are *his* and which are views of someone else's wall.

Carter pushes back from the keyboard slightly — making space for him. Literally and figuratively. She looks at Casey, who gives a small nod. They're both recognizing the same thing: Marcus isn't just learning. He's validating.

**MARCUS:** *(quietly, almost to himself)* This is Roman. We would build it this way.

### Beat 3: Daniel's Crisis

Daniel pulls Lucia aside. The courtyard. Morning light through olive trees.

**DANIEL:** I need to understand something. When you said "costume" —

**LUCIA:** I was not trying to be cruel.

**DANIEL:** No, you were right. I've spent my career translating between cultures. I thought I was good at it.

**LUCIA:** You *are* good at it. That is the problem. You are so skilled at finding equivalences that you forget they are not identities. A Roman pool is not an English pool. A *chorda* is not a "string." The gap between them is where the learning happens.

**DANIEL:** I've been translating assumptions, not concepts.

Beat. He's arrived at it himself. She watches him get there — and her expression shifts, just slightly. Respect for someone who can admit the thing.

**DANIEL:** So what should I be doing?

**LUCIA:** *(carefully)* When a Roman hears the word and thinks of your concept, you have translated. When a Roman hears the word and thinks of *their experience*, you have found the right word. *(beat)* You should be midwifing. Helping us give birth to concepts that are ours. Not dressing your concepts in our clothing.

### Beat 4: Data's Contribution

The group is debating whether length-prefixed is genuinely superior or just different. Data, sitting at the edge of the room — still partially damaged from 1x02, one eye cover missing, movement slightly stiff — speaks for the first time this episode.

**DATA:** In my time, we encountered four hundred twelve species with written language. The species that separated content from length — that encoded the measure of text within the text itself — demonstrated lower error rates across all transmission media.

Silence.

**LUCIA:** *(to Daniel)* Your machine agrees with me.

**DATA:** I am not a machine in the way you mean. And I do not agree. I am reporting an observation. The data is... *(a half-second delay, damaged processors clicking)* ...impartialis. Impartial.

**LUCIA:** *(studying him, genuinely curious)* ...You are more interesting than I expected.

Midas, sitting next to Data, is completely relaxed — the barometer for the audience. His hand rests on Data's shoulder. Data doesn't reject it.

### Beat 5: ACT BREAK — Midas and Teal'c

The technical debate gets heated. Daniel is frustrated that everyone keeps rejecting his original vocabulary. Lucia is frustrated that he keeps reaching for English metaphors.

Midas breaks the tension:

**MIDAS:** In Athens, Aristotle and I had a similar argument about the word for "category." He was wrong, but he was also my host, so I let it go. *(to Daniel)* You are not my guest, so: she is correct.

**DANIEL:** *(laughing despite himself)* Thank you, Midas.

**MIDAS:** De nihilo. I have been wrong about words for three thousand years. I recognize it instantly in others.

And then the kill shot. Teal'c, who has been silent the entire act, stands and hands Marcus a wax tablet covered in notation.

**MARCUS:** *(reading)* ...When did you write these?

**TEAL'C:** While they argued.

He's written the Credo test suite for chorda. Setup, action, assertion — every function Daniel and Lucia were debating, Teal'c already has tests for. The episode's biggest laugh. Simple, devastating, perfectly Teal'c.

Hard cut. Carter reading Teal'c's tests, nodding. They're good.

---

## ACT TWO: "Aedificator" (The Builder)

*From words to sentences. From understanding to construction.*

### Beat 6: The Builder Problem

Lucia, examining the chorda operations, asks the question that opens Act Two:

**LUCIA:** You can take a chorda apart. You can compare them. You can find things inside them. But how do you *build* one? If I am writing a letter, I do not start with the whole letter. I start with a word. Then another. Then another.

Carter and Casey exchange a look. She's describing the string builder.

Casey picks up one of Marcus's wax tablets.

**CASEY:** Think of this. You write on it. Erase. Write more. When you're satisfied, you copy the final version onto papyrus. The tablet is the workspace. The papyrus is the finished string.

**PLINY:** I have been doing this for eighty years. I write drafts. Many drafts. The final copy is the work. Everything before it is scaffolding.

**CASEY:** *Aedificator.* The builder. The scaffolding.

**PLINY:** *(pleased)* That is a good word.

### Beat 7: Building the Builder

Carter implements the ChordaAedificator. The key dramatic question: what happens when the builder runs out of space?

**CARTER:** When the buffer fills, we double its capacity. Allocate a larger space from the Piscina, copy everything over, continue.

**MARCUS:** Like extending the road. You do not tear up what you have built. You pave more.

**TEAL'C:** What happens if the extension fails?

**CARTER:** We return an error.

**TEAL'C:** Show me.

He writes the test before the code exists. 1x02's Credo philosophy in action: specify the failure before you implement the success.

### Beat 8: The Indentation Discovery

The aedificator needs to track indentation — for generating structured reports. Carter explains nested levels: each level adds spaces to show structure.

**LUCIA:** *(watching)* This is rhetoric. In a speech, when you move to a subordinate clause, you signal the shift. The listener knows they are inside a nested argument. When you return to the main clause, you signal the return.

**DANIEL:** *Indentatio* — literally "toothing in." Like stones that tooth into each other at a corner.

**MARCUS:** *(nodding)* A builder's technique. The stones interlock.

The indentation system (`push_indentationem` / `pop_indentationem`) is born from rhetoric and masonry simultaneously. Two Roman metaphors for the same computational idea. Daniel and Lucia glance at each other — the first moment where they're building together instead of debating.

### Beat 9: O'Neill Catches the Bug

O'Neill has been sitting in the back all episode, ostensibly reading one of Pliny's scrolls. (He'll never admit he finds them interesting.)

The team is reviewing the aedificator's `finire` function — the function that converts a builder into a finished chorda.

O'Neill, without looking up:

**O'NEILL:** What happens if you finish it twice?

**CARTER:** You can't. It's —

**O'NEILL:** Yeah, but what if someone does?

Silence.

**CARTER:** *(checking the code)* ...Huh.

**O'NEILL:** "Huh" good or "huh" bad?

**CARTER:** It would use the builder after its buffer has been reassigned. The data could be overwritten. *(to Casey)* Did you —

**CASEY:** *(already typing)* On it.

Later, Marcus congratulates O'Neill.

**MARCUS:** That was well observed.

**O'NEILL:** Lucky guess.

**MARCUS:** Carter said luck as well. But you are the one who looked. The others assumed.

**O'NEILL:** *(uncomfortable)* I just... it sounded like the kind of thing someone would do. Finish something and then forget you already finished it.

**MARCUS:** *(seriously)* That is not luck. That is wisdom.

**O'NEILL:** *(standing up, changing subject)* So where's the food? Is there food here? I heard you people have olives.

The comedy is that O'Neill is genuinely embarrassed by the compliment. He doesn't want to be seen as technically competent. Marcus's Roman directness — calling it wisdom without irony — gets under his skin in a way Carter's scientific praise wouldn't.

### Beat 10: The Daniel/Lucia Scene

Night. Pliny's courtyard. Wine. Stars. Daniel and Lucia, alone for the first time.

**DANIEL:** You changed how I think about this today.

**LUCIA:** I challenged an assumption. That is what philosophers do.

**DANIEL:** No, it's more than that. You made me realize I've been treating Latin as a dead language. Something to pour ideas into, like a mold. But it's alive here. It's *your* language.

**LUCIA:** *(pause)* Latin was alive when Cicero used it to argue for the republic. It was alive when Virgil used it to sing of arms and the man. It was alive when the exiles used it to name every river and mountain on this world. And now you want us to use it to name... *(she gestures at Carter's laptop visible through the window)* ...whatever *that* is.

**DANIEL:** Yes.

**LUCIA:** Then the words must be worthy of the language. Not translations. Not approximations. Words that a Roman child will grow up with and never know came from anywhere else.

**DANIEL:** That's what I want.

**LUCIA:** Then stop trying to translate. Start trying to *listen*.

Daniel watches her. She holds his gaze for a moment, then looks away — back to the stars. He leans in slightly. She maintains her distance. Whatever this is, it's going to take a very long time.

**DIRECTION NOTE:** The physical dynamic between Daniel and Lucia is the opposite of their intellectual dynamic. Intellectually, they close the distance across the episode — from opposition to collaboration. Physically, they maintain distance. She sits still; he gestures. When she leans in once — just slightly, when he says something that genuinely surprises her — they both notice. They both pretend they didn't.

### Beat 11: Casey's Moment

Casey and Carter, after the others have left.

Casey reaches into his jacket pocket for his pen. As he does, something shifts — a rosary, dark wooden beads, briefly visible. Not displayed. Just *there*, the way a wallet or a pocketknife is there. A Roman across the room — one of Marcus's engineers — notices. His eyes stay on it for a beat. He says nothing. But something registers: the stranger carries prayer beads. Romans know prayer beads.

**CASEY:** Length-prefixed. I've been using length-prefixed strings for twenty years.

**CARTER:** And?

**INTERCUT — CASEY'S FLASHBACK: IBM, 1952.** A younger Casey (still looking exactly the same — the Immortal tell the audience already knows) at a drafting table. Punch cards. A colleague arguing for null-terminated character arrays. "It's simpler. Just scan until you hit zero." Casey, stubborn: "A string should know its own length. You don't measure a bridge by walking until you fall off the end."

He lost that argument. The industry went with null termination. He's been watching the consequences for fifty years.

**BACK TO NOVA ROMA.**

**CASEY:** And she's right. The Romans would have done it this way. We did it wrong, Sam. For fifty years. We scanned for the end of every string because Ken Thompson made that choice in 1972 and nobody questioned it. *(beat)* The Romans questioned it in five minutes.

Casey smiles when Lucia explains her reasoning — not the smile of someone being polite, but the smile of a man watching history correct itself. The nominalism connection, which the show won't name until later: null-terminated strings are nominalist. The string has no intrinsic property of length — you discover it externally, by scanning for absence. Length-prefixed strings are realist. The string *knows what it is*. The measure is part of the substance. Lucia's Roman instinct — *mensura et datum* — is moderate realism applied to data structures, and she arrived there in five minutes because the Romans never had Ockham's razor taken to their ontology.

**CARTER:** That's why we're here.

**CASEY:** *(quietly)* Yeah. That's why we're here.

Casey's moment: recognizing that the clean-room rebuild isn't just about security. It's about making better choices because fresh eyes see what accumulated convention hides. And maybe — though he won't say it — because a civilization that never went through the Reformation still trusts that things can know what they are.

### Beat 12: ACT BREAK

Carter demonstrates the aedificator building a simple structured document — a census entry for a single Nova Roma citizen. Name. Age. Occupation. Trade. It works. Marcus reads it on screen.

**MARCUS:** One citizen. *(beat)* There are three thousand.

Cut to: Daniel staring at a blank wax tablet. He's trying to write the Latin terms for a full census report. Not translating — *creating*. And he's stuck. Lucia sits down across from him. Picks up her stylus.

**LUCIA:** Together.

---

## ACT THREE: "Nuntius" (The Report)

*From words to message. The first structured document for Nova Roma.*

### Beat 13: Building the Report

The Nova Roma senate — a council of elders in Pliny's tradition — has requested a report. They want to understand what the strangers are building and why. Pliny has offered to present it. The report itself should be generated by the system they've built — proof of concept.

Daniel and Lucia sit together over the vocabulary. He proposes terms. She refines, redirects, sometimes rejects outright.

- Daniel proposes a word for "allocate" — Lucia says it implies permanent ownership. She suggests one that implies temporary stewardship instead. She's right; arena allocation IS temporary stewardship.
- Lucia invents a term Daniel hadn't considered — a gerund form for the act of building a string, not just the string itself.
- Daniel brings cross-cultural insight: "In Chinese, the character for 'text' contains the character for 'pattern.' They've always seen text as structure." Lucia: "A scroll is not read character by character. A scroll is *unrolled*. The reader sees the shape before the words." Together they arrive somewhere neither could alone.

Meanwhile, Carter, Casey, Marcus, and Teal'c build the report generator. Each function from this episode serves the construction:

- `chorda_ex_literis` — the header text
- `ChordaAedificator` with indentation — the structured body
- `chorda_concatenare` — joining sections
- `chorda_ex_s32` — numerical data (population counts, resource inventories)
- `push_indentationem` / `pop_indentationem` — nested structure

We see the code working in montage. The aedificator appends header, pushes indentation, adds data, pops indentation. The structure emerges line by line. Craft satisfaction — watching the tools they built *work* together.

### Beat 14: The Case Conversions (Light Technical Beat)

A fun moment: Marcus asks why some of Carter's variable names look different. Why is one `helloMundus` and another `hello_mundus`?

Carter explains case conventions. Daniel and Lucia are delighted.

**LUCIA:** You mean different forms of the same word carry different grammatical information? We have been doing this for two thousand years.

**DANIEL:** *(laughing)* We reinvented Latin declensions and called them "case conventions."

Carter writes `chorda_serpens` and `chorda_camelus` — snake_case and camelCase converters. Lucia names the PascalCase converter `chorda_pascalis`, making up a plausible Roman etymology on the spot. Marcus almost smiles.

### Beat 15: Pliny's Wings

The full report generates. The aedificator builds it line by line. Pliny reads it in silence.

A census summary — population of Nova Roma, organized by family, trade, district. Numbers and names in structured, indented Latin. Clean. Readable. Generated from data, not hand-written.

**PLINY:** In eighty years, I have written forty volumes by hand. Every number counted. Every name recorded. Every entry checked against the last.

He looks at the screen.

**PLINY:** This is one report. Generated in the time it takes to pour wine. And it is *accurate*.

Beat. The silence should be felt.

**PLINY:** I am not being replaced. I am being given wings.

This is the episode's climactic "ah" moment — but it's Pliny's, not Marcus's. The scholar who has spent eighty years documenting a civilization by hand, watching the machine build a perfect census report in seconds. Not displaced. *Amplified.* His life's work is the data. The system just gave him a faster pen.

### Beat 16: The Senate

The Nova Roma senate chamber. Stone seats in a semicircle. Oil lamps. Very Roman.

Pliny presents the report. Carter and Daniel stand to the side. Marcus is in the gallery — a Roman watching Romans receive technology from strangers.

The senators read. They ask questions. Marcus translates the technical ones to Carter. The questions are sharp — Roman political minds are not easily impressed.

A senator asks: "Can it lie?"

**PLINY:** The report contains only what we put into it. If the data is honest, the report is honest.

**SENATOR:** And who ensures the data is honest?

**TEAL'C:** *(from the back, where he's been standing)* We test it.

The senator doesn't know the word. But he understands the tone. He nods. The Credo framework from 1x02, invoked by its architect — testing is how you ensure the words are true.

---

## TAG

### Beat 17: The Lexicon

Evening. Daniel finds Lucia in the courtyard. She's writing on a wax tablet by lamplight. Her handwriting is small and precise. She never crosses anything out — she thinks before she writes.

**DANIEL:** What are you writing?

**LUCIA:** A lexicon. The words we created today. With their reasoning. So that when the next person uses *chorda*, they will understand why it is *chorda* and not *textilis* and not *catena*.

**DANIEL:** You're building a dictionary.

**LUCIA:** I am building a memory. So the words remember where they came from.

Daniel watches her write. She's documenting the etymology of a programming language. This is the beginning of the curriculum that will define Nova Roma's computational vocabulary for generations.

**DANIEL:** In the end it's not our language or your language.

**LUCIA:** *(without looking up)* No. It is the language of the work.

FADE TO BLACK.

---

## STINGER

### Beat 18: Kirk's Signal

**Location:** Earth. Night. A nondescript office building in Portland, Oregon. Server racks humming behind glass.

A security guard makes his rounds. Flashlight sweeping empty corridors. Normal night.

He passes a door. Light underneath it. He frowns — this floor should be empty. He opens the door.

A server room, but wrong. Modified. Additional hardware patched in with cables that look too organic, too fluid. A green status LED blinks on equipment that doesn't match any manufacturer's catalog.

The guard's radio crackles. A voice — warm, reassuring, the kind of voice that makes you feel like everything is fine:

**KIRK (V.O.):** Thank you for checking. Everything's in order. You can continue your rounds.

The guard hesitates. Then nods. Turns. Walks away. He won't remember stopping.

The camera stays on the server room. The green LED blinks. Data is flowing — not much, not yet. But the infrastructure is being laid. Piece by piece. Byte by byte.

Kirk is building something too. And he doesn't need to argue about the right words. He just takes them.

SMASH TO BLACK.

END OF EPISODE

---

## THEMATIC NOTES

**"Verba Nova"** shifts meaning across the episode:
- **Act 1:** "New Words" = translation. Daniel bringing English computing terms into Latin. New words for old concepts. Lucia's correction: "That is not language. That is costume."
- **Act 2:** "New Words" = creation. Words that don't translate FROM anything. *Chorda* isn't a translation of "string." It's a new word for a new concept, born from Roman experience.
- **Act 3:** "New Words" = the report. The first structured text generated by the system — words the Romans wrote, in Latin, using tools they helped design. Nova Roma's words.
- **Tag:** "New Words" = Daniel's realization. He's not a translator anymore. He's a midwife. The language is being born, and it belongs to the Romans.

**Null-Terminated vs Length-Prefixed as Worldview:**
- Null-terminated: you don't know the scope. Scan until you hit the boundary. Discover the length by searching for the absence. If the terminator is missing, you run off the edge into corruption. The reactive worldview.
- Length-prefixed: you know the scope before you begin. The word carries its own boundary. No dangerous scan. No missing terminator. The intentional worldview.
- The Romans' physical instinct — a cord has a known length, you hold both ends — maps perfectly to length-prefixed. Earth's convention is the weird one.

**"Costume vs. Language" Operates on Multiple Levels:**
- **The software:** Daniel dresses C concepts in Latin words. The structure of thought remains English.
- **The civilization:** This is what colonizers do — arrive with pre-formed concepts and search for local words to drape over them. Daniel, the show's most culturally sensitive character, is unconsciously colonizing Latin.
- **The show itself:** "Verba Nova" could have been "Americans teaching Romans to use computers." Lucia's challenge forces it to become genuine collaboration where Roman thinking IMPROVES the technology.

**The Aedificator as Composition:**
The builder is the space where thought becomes language. You add piece by piece — uncertain, growing — then call `finire` and the accumulation crystallizes into a known thing. The wax tablet before the papyrus. The drafts before the final copy. Casey's whole life has been scaffolding for this moment.

**Kirk as Anti-Language:**
Kirk takes. He doesn't create. His network is built from stolen infrastructure, compromised people, co-opted systems. The Borg assimilate — they overwrite your language with theirs. Kirk's stinger mirrors the episode's thesis from the dark side: while the team argues about the right word for "string," Kirk doesn't argue at all. He just installs his code. The Borg's fundamental weakness: they can only assimilate, never create. *Verba nova* — new words — is the one thing the Collective cannot produce.

**Connection to 1x01 and 1x02:**
- 1x01 (Piscina) → 1x03 (Chorda): The pool is where memory lives. The string is the first thing that lives in it. `chorda_ex_literis` takes a `Piscina*` parameter. The foundation literally supports the language.
- 1x02 (Credo) → 1x03 (Chorda): Credo asked "how do you verify trust?" Chorda asks "how do you express truth?" Teal'c writes chorda tests in the background, applying 1x02's methodology to 1x03's subject matter. The epistemology meets the medium.
- Kirk in 1x02 was a null-terminated human — you had to scan all the way to the end to discover he wasn't what he appeared to be. Chorda is Data — self-describing, carrying its own measure, honest about what it contains.

**Genre Architecture:**
- "Darmok" inversion: in TNG, the alien teaches the human through shared mythology. Here, the Roman teaches the Earthling through philosophy. The resolution in both: stop translating and start understanding.
- Arrival / Sapir-Whorf: language shapes thought. Length-prefixed vs null-terminated isn't just a design choice — it's a cognitive framework that determines how a civilization relates to text.
- "Window of Opportunity" O'Neill: the man who secretly learned Latin in a time loop catches a bug because he's been reading code the whole time. He doesn't want to talk about it.
- "Measure of a Man" for data types: the definitions we choose have consequences for generations. Is text something you scan until it ends, or something that knows its own length?
- Galaxy Quest parallel: the Romans take the concepts literally — and their literal interpretation is BETTER than Earth's accumulated metaphorical spaghetti.

---

## C-PLOT THREADING

**Beat C1 (early Act 1):** Duncan reports: Kirk spotted in Colorado Springs two days ago. Then Seattle. Then gone. Moving fast, leaving no trail.

**Beat C2 (mid Act 2):** Casey intercepts a brief data transmission matching Borg tablet encoding. Source: a server farm in Oregon. By the time they triangulate, the server is wiped. Kirk is building infrastructure.

**Beat C3 (late Act 2):** O'Neill, between the bug-catching and the olives, quietly increases the watch on the gate room. When Carter asks:

**O'NEILL:** He said he'd see us again. Kirk doesn't bluff. He beats the bluff.

**Beat C4 (during senate presentation):** Duncan and Methos, not at the presentation, on the roof of the villa. Scanning the horizon.

**METHOS:** This place is peaceful.

**DUNCAN:** It won't stay that way.

**METHOS:** No. *(beat)* It never does.

---

## OPEN QUESTIONS

1. **Joe Dawson.** Does he appear? The bar isn't open yet (1x13). He could be at the senate presentation as an observer/chronicler.

2. **The report format.** Is the senate report plain text, or early STML? Plain text feels right for 1x03 — XML/STML is 1x09-10.

3. **Methos's correction.** The genre-fan pitched Methos casually correcting Daniel on a Latin etymological point. Worth including? Could be a single devastating line that sets up his 1x06 spotlight.

4. **Lucia's backstory.** How much do we learn about her this episode? She's a philosopher and teacher — but what's her story? Did she teach in Rome before exile? Was she born on Nova Roma?

5. **The double-finire bug.** O'Neill's catch — is this technically accurate to the actual codebase? The aedificator uses arena allocation, so "use after free" works differently. May need to adjust the specific bug while keeping the comedic beat.
