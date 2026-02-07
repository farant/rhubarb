# NOVUM FUNDAMENTUM
## Episode 1x06: "Unum Pro Omnibus" (One For All)
### Beat Sheet (Draft 1 -- Compiled from Writers' Room)

---

## COLD OPEN

**Location:** Carter's workshop. Pre-dawn. The screen glows. Carter has been up all night running diagnostics on the census system they built in 1x04-1x05.

On screen: a visualization. The census data rendered as a memory map -- blocks of color, each representing an allocated string. The visualization looks like a quilt. Red patches (district names) scattered everywhere -- hundreds of them, but only twelve unique values. Blue patches (occupation names) -- hundreds, but only fifty unique values. Green patches (street names) -- hundreds, but only two hundred unique values.

Carter is looking at a quilt made of repetition.

She types a query:

```c
/* Computare redundantiam */
i32 chordae_totales = III_CCXVII;
i32 chordae_unicae = CCLXIII;
f32 redundantia = I - ((f32)chordae_unicae / (f32)chordae_totales);
/* redundantia = 0.918 -- 91.8% redundantes */
```

Three thousand, two hundred and seventeen strings. Two hundred and sixty-three unique values. Ninety-two percent of every string in the census is a duplicate of something already there.

**CARTER:** *(to herself, staring at the numbers)* Ninety-two percent.

She picks up a census scroll -- Pliny's original -- and begins counting. "Faber... Faber... Faber..." Each one written out in full. Each one using ink, using space, using Pliny's time. She's counting the waste in analog before she shows it in digital.

**CARTER:** *(setting down the scroll)* Every census-taker in Rome wrote the same fifty occupation names three thousand times. They could have written the word once and pointed to it.

She opens her notebook. Writes one word at the top of a blank page:

**CANONICA**

SMASH TO MAIN TITLES.

---

## ACT ONE: "Redundantia" (The Redundancy)

*The problem is named. The census reveals the waste. The linguists bridge the concept. The name is given.*

### Beat 1: Carter's Morning Report

**Location:** The Tabularium. Morning. The team gathers. Carter has her visualization on screen -- the quilt of redundancy.

**CARTER:** The hash table works. The census is searchable. But we have a new problem.

She runs the numbers. On screen:

```
Chordae in censu: 3217
Chordae unicae: 263
Chordae redundantes: 2954 (91.8%)
Memoria consumpta: 847,293 bytes
Memoria necessaria: 23,107 bytes (si unicae solum)
```

Twenty-three thousand bytes versus eight hundred and forty-seven thousand. Thirty-six times the necessary memory.

**MARCUS:** *(studying the numbers)* The census is mostly repetition.

**CARTER:** Mostly repetition. Three thousand citizens. Twelve districts. Fifty occupations. Two hundred streets. The *vocabulary* is small. But every citizen carries their own copy of every word.

**MARCUS:** *(immediately understanding)* When a road sign says "Roma" in a hundred places, the stone-cutter does not invent the word "Roma" each time. He cuts the same letters. He *copies*.

**CARTER:** Right. But our census isn't copying from a master. Each entry has its own private "Roma." As if every milestone on the Via Appia carried its own private invention of the concept "Rome."

### Beat 2: Pliny's Reaction -- The Encyclopedist's Horror

Pliny has been listening. He approaches the screen.

**PLINY:** Show me my encyclopedia.

Carter runs the same analysis on Pliny's indexed data:

```
Chordae in encyclopaedia: 12,847
Chordae unicae: 1,203
Redundantia: 90.6%
```

Pliny sits down slowly.

**PLINY:** I have written *cinnabar* four hundred and twelve times. *Hispania* six hundred and three times. *(beat)* In forty volumes of careful observation, nine-tenths of my words are things I have already said.

**DANIEL:** *(from the corner)* Every language works that way. A few hundred words carry ninety percent of all communication. The vocabulary that matters is small. The *usage* is vast.

**PLINY:** *(to Carter, with the directness of a man who just learned he wasted eighty years of ink)* Then fix it.

### Beat 3: Daniel and Lucia -- The Linguistic Bridge

Daniel and Lucia are already thinking. This is THEIR territory -- the problem of canonical forms is what lexicographers have been solving since writing began.

**DANIEL:** This is a linguistic problem, not a computational one. Every language has a concept of the *canonical form* -- the dictionary entry, the root word, the lemma. "Fabrum," "fabri," "faber" -- the canonical form is "faber."

**LUCIA:** *(precisely)* In my lexicon, each word appears once. When I use it in a sentence, I do not re-create the word. I *reference* the lexicon. The sentence points to the word. The word exists once.

**LUCIA:** *(with the quiet authority of someone who has been doing this since childhood)* I have been interning words since I was twelve years old, Daniel. We simply didn't have an ugly name for it.

**DANIEL:** In English, we call it a "dictionary form." In linguistics, a "lemma." In Latin --

**LUCIA:** *Forma canonica.* The standard form. The one true copy from which all uses derive.

**CARTER:** *(writing it down)* One true copy. Every reference points to it. When you need the word, you don't create a new copy. You find the canonical one and point to it.

### Beat 4: Pointer Equality -- Content vs. Identity

Carter is at the board. She draws two boxes:

```
Civis A: "Faber"  [address: 0x1000]
Civis B: "Faber"  [address: 0x2000]
```

**CARTER:** Right now, when I compare these two strings, I compare every character. F-a-b-e-r. Five comparisons. They're equal.

She erases. Redraws:

```
CANONICA: "Faber"  [address: 0x1000]
Civis A: -> 0x1000
Civis B: -> 0x1000
```

**CARTER:** If both citizens point to the *same* "Faber" -- the one canonical copy -- then I don't compare characters at all. I compare *addresses*. One comparison. One machine instruction. Equal means *identical* -- the same object, not merely the same content.

**DATA:** *(from his workstation, precise)* This is a stronger form of equality. Content equality asks: "Do these two things have the same properties?" Identity equality asks: "Are these two things the same thing?" Content equality requires examination. Identity equality requires only recognition.

**PLINY:** *(slowly, his scholar's mind turning)* When a Roman citizen presents himself to the censor, the censor does not re-examine his name. He looks at the mark on the tablet and says: "This is the record I already have." He *recognizes* the citizen. He does not re-identify him.

**DATA:** Pointer comparison is recognition. String comparison is re-identification. The first is O(1). The second is O(n).

**MARCUS:** *(the engineer, always practical)* How do you ensure that the two citizens point to the same "Faber"?

**CARTER:** You build a table. A *single source of truth*. Every word in the census exists once in the table. When a new citizen record arrives, you don't store the word. You look it up. If it's already in the table, you point to the existing copy. If it's new, you add it. Then you point to it.

### Beat 5: ACT BREAK -- The Naming of the Internamentum

Daniel is thinking about names. This is the naming scene -- and for the first time, it's a three-way collaboration: Daniel, Lucia, and Pliny.

**DANIEL:** We need a term. A process name. What do you call the act of replacing a private copy with a reference to the canonical form?

**LUCIA:** *Internare.* To place within. You take what is scattered and place it within a single canonical structure.

**DANIEL:** *Internamentum.* The act of interning. The thing that holds the interned.

**PLINY:** *(who has been listening, now objecting with scholarly precision)* We already have a word. *Exemplar.* The master copy from which all others derive.

**LUCIA:** Then the internamentum is the *theca exemplarium* -- the treasury of master copies.

**PLINY:** *(writing it down, considering)* I prefer *internamentum.* It implies the act of placing something inside a secure space. An exemplar could be anywhere. An interned string is *held.*

Lucia writes on the board:

***Internamentum: Locus ubi unum pro omnibus conservatur.***
*The place where one is kept for all.*

**PLINY:** *(reading it, approving)* One for all. *Unum pro omnibus.*

**CARTER:** *(looking at the phrase, then at the quilt of redundancy on screen)* Unum pro omnibus. One copy. For everyone.

Hard cut to the board. The title of the episode, written in Lucia's hand.

---

## ACT TWO: "Aedificare Super Fundamenta" (Building On Foundations)

*The dependency chain moment. The core implementation. The Borg parallel. The warning.*

### Beat 6: The Dependency Chain

This is the structural landmark of the episode. For the first time, a new library is built *on top of* a previous episode's library. The audience should feel the accretion -- the stacking of capabilities, each one enabling the next.

Carter opens a new file. On screen:

```c
#include "internamentum.h"
```

But the camera lingers on the includes within that header:

```c
#include "chorda.h"           /* Verba Nova (1x03) */
#include "piscina.h"          /* Capacitas Nova (1x01) */
#include "tabula_dispersa.h"  /* Tabularium Novum (1x05) */
```

**CARTER:** We can't build this from nothing. String interning needs a hash table to find existing copies quickly. It needs an arena to allocate canonical copies permanently. It needs strings to be length-prefixed so we know their content without null terminators.

She writes the dependency on the board:

```
Piscina (1x01) -- the arena where canonical copies live
  -> Chorda (1x03) -- the string type that knows its length
    -> Tabula Dispersa (1x05) -- the lookup table that finds existing copies
      -> Internamentum (1x06) -- one copy, for all
```

**MARCUS:** *(studying the chain)* Each stone rests on the stone below it.

**CARTER:** Each library rests on the library before it. Remove any one of these and the one above it collapses.

**DATA:** *(quiet)* That is also how ecosystems develop. Each organism creates the conditions for the next. The first plants fix nitrogen. The nitrogen enables more complex plants. The complex plants enable animals. No organism planned the ecosystem. Each one built what the previous one made possible.

### Beat 7: Core Implementation + Three Voices

Carter codes. The audience sees the core function:

```c
chorda*
chorda_internare(
    InternamentumChorda* intern,
    chorda s)
{
    vacuum* valor_existens;
    chorda* canonica;

    /* Quaerere in tabula: iam internata? */
    si (tabula_dispersa_invenire(
        intern->tabula, s, &valor_existens))
    {
        /* Iam internata -- redde canonicam */
        redde (chorda*)valor_existens;
    }

    /* Nova -- creare copiam canonicam in piscina */
    canonica = piscina_allocare(
        intern->piscina, magnitudo(chorda));
    *canonica = chorda_transcribere(s, intern->piscina);

    /* Inserere in tabulam */
    tabula_dispersa_inserere(
        intern->tabula, *canonica, canonica);

    redde canonica;
}
```

**CARTER:** *(walking through it)* Step one: look up the string in the hash table. If it's already there, return the existing canonical pointer. Done. One probe. Step two: if it's NOT there, allocate space in the arena. Copy the string. This is the *canonical copy*. Insert it into the hash table so the next person who asks for it will find it. Step three: return the canonical pointer. From now on, everyone who has this word has the *same* pointer. Not a copy. THE word.

**MARCUS:** *(testing his understanding)* The first citizen who says "Faber" creates the canonical copy. The second citizen who says "Faber" receives a pointer to the first citizen's word?

**CARTER:** Not the first citizen's word. The *canonical* word. It belongs to no citizen. It belongs to the dictionary. The first citizen triggered its creation, but once it's in the table, it's no one's private copy. It's the shared truth.

*This is the philosophical crux. Marcus's question -- "the first citizen's word?" -- is exactly the wrong frame. The canonical copy is not anyone's property. Carter's correction is the episode's thesis in miniature.*

A beat. Carter has shown the code. Now three characters explain what it MEANS through their own lens.

**METHOS:** *(leaning back, wine in hand)* I had forty-seven names before I came here. Adam Pierson, Benjamin Adams, a Sumerian name I won't try to pronounce. Every name was a different man. One canonical identity, hidden behind forty-seven pointers. *(drinks)* Your machine does in microseconds what took me five thousand years to manage badly.

**DATA:** *(standing)* String interning transforms the nature of the operation from *examination* to *recognition*. Two interned strings are not merely equivalent. They are identical. The comparison is not "do these match?" but "are these the same?" Recognition, not examination.

**CASEY:** *(hands in pockets, looking at the screen)* When I was a kid -- a real kid, not an Immortal kid -- my grandfather had one Bible. One copy. King James. Everyone in the family read from the same book. Not copies of it. The same physical book. You'd open it and see your grandmother's notes in the margin. Your father's underlining. Forty years of family reading, all pointing to the same canonical text. *(beat)* That's interning. One copy. Everyone points to it. And because they all share it, they all share the conversation.

*Three translations of the same algorithm. Methos: the spy who maintained dozens of false identities resolving to one truth. Data: the analytical mind that sees the mathematical transformation. Casey: the man whose faith tradition has practiced canonical referencing for millennia. All correct. All revealing.*

### Beat 8: The Borg Parallel -- Methos Speaks

Methos has been listening from his usual position -- leaning against a pillar, cup of wine, air of studied indifference. He sets down the cup.

**METHOS:** You know who else does this.

The room goes still. Not because Methos rarely speaks -- he speaks constantly, about nothing. Because of his tone. Flat. The tone he uses when something ancient wakes up.

**CARTER:** Who?

**METHOS:** The Borg.

Silence.

**METHOS:** *(pushing off the pillar, walking to the board)* The Borg assimilate. One drone, identical to every other drone. One consciousness, shared. One purpose, universal. *(pointing at Carter's diagram)* One copy, for all. *Unum pro omnibus.*

**O'NEILL:** *(dangerous quiet)* You're saying we're building Borg tech.

**METHOS:** I'm saying the *efficiency* is the same. One copy instead of many. Shared instead of duplicated. The Borg solve the same problem you're solving -- redundancy. Waste. The cost of individuality. *(beat)* They solved it by destroying the individual.

### Beat 9: Carter's Answer -- The Distinction

Carter thinks. She doesn't rush. She goes to the board.

**CARTER:** *(drawing)* When the Borg assimilate, they destroy the original. The individual ceases to exist. Their memories, their personality, their will -- overwritten. What's left is a copy of the collective.

She draws a second diagram.

**CARTER:** When we intern, we preserve the content. The canonical copy is the *word* -- not the *person*. "Faber" is interned. The craftsman is not. Two hundred citizens are craftsmen. They all use the same word. They remain two hundred different people.

**DATA:** *(standing, which he rarely does for dialogue)* The distinction is precise. Interning operates on the *name*, not the *named*. The canonical copy preserves the content of the string -- every character, in order, exactly as it was. Nothing is lost. The Borg eliminate the *nature*. They do not preserve the content. They replace it.

**METHOS:** *(after a long moment, nodding slowly)* The Borg share identity. You share vocabulary. *(beat)* That's a thinner line than you think.

He picks up his wine and sits back down. The warning planted.

**CASEY:** *(from his workbench, not looking up)* One for all isn't the same as all for one.

The room hears it. Nobody responds. Casey's one-liner lands as the beat's final word -- the man whose crucifix is in frame, half-quoting the Musketeers motto, drawing the line between the episode's two philosophies in eight words.

*DIRECTION NOTE: During this exchange -- from Beat 8 through Beat 9 -- Casey is at his workbench. The crucifix is visible behind him. No one looks at it. No one mentions it. But the composition places it in the same frame as the word "canonical." The audience makes the connection. The camera says it. Nobody else does.*

### Beat 10: ACT BREAK -- The Global Singleton

Carter makes a design decision. On screen:

```c
/* Internamentum globale -- unum pro omnibus */
hic_manens InternamentumChorda* _internamentum_globale = NIHIL;

InternamentumChorda*
internamentum_globale(vacuum)
{
    si (_internamentum_globale == NIHIL)
    {
        /* Pigra initialisatio */
        _piscina_globalis = piscina_generare_dynamicum(
            "internamentum_globale", 65536);
        _internamentum_globale = internamentum_creare(
            _piscina_globalis);
    }
    redde _internamentum_globale;
}
```

**CARTER:** One table. For the entire system. Every module, every library, every component that interns a string -- they all use the same table.

**MARCUS:** *(immediately)* A shared resource. Like a public well. Every household draws from it. No household owns it.

**O'NEILL:** *(from the back, quiet)* One table. One point of failure.

**CARTER:** *(acknowledging)* Yes.

**O'NEILL:** *(to the room)* That's a lot of trust in one table.

He walks out. Carter watches him go. O'Neill, as usual, has identified the strategic risk in three sentences.

---

## ACT THREE: "Probatio" (The Test)

*Interning the census. The pointer comparison payoff. Teal'c tests. The vocabulary of a civilization.*

### Beat 11: Interning the Census

The team converts the census. Carter writes the transformation:

```c
/* Convertere censum ad chordas internatas */
per (i = ZEPHYRUM; i < census->numerus; i++)
{
    Civis* civis = xar_elementum(census, i);

    /* Internare singulas chordas */
    civis->occupatio = chorda_internare(
        intern, *civis->occupatio);
    civis->regio = chorda_internare(
        intern, *civis->regio);
    civis->via = chorda_internare(
        intern, *civis->via);
}
```

On screen: the progress counter. Three thousand citizens. Each citizen's occupation, district, and street interned. The memory visualization from the cold open -- the quilt of redundancy -- transforms in real time. Red patches merge. Blue patches merge. Green patches merge. The quilt shrinks, consolidates, becomes simpler. Two hundred and three copies of "Faber" become one. Four hundred and twelve copies of "Forum Stellarum" become one.

```
Memoria ante internamentum: 847,293 bytes
Memoria post internamentum: 24,891 bytes
Reductio: 97.1%
```

*DIRECTION NOTE: During the interning montage, when Data's name scrolls by in the census entries, the counter on screen reads "XLVII." Not called out. One-frame visual. Freeze-frame gold.*

During a lull in the montage, Carter is coding. Casey brings her coffee. A brief exchange:

**CARTER:** *(not looking up)* One copy serves all references. I wish intelligence worked that way. One truth, shared with everyone.

**CASEY:** What truth are you carrying?

**CARTER:** *(deflecting, back to the keyboard)* Too many.

Casey doesn't push. But he's noticed. Casey always notices. The weight of the compass data, the Midas temporal file, the growing secret burden -- it shows for two lines and then Carter is back to work.

### Beat 12: The Pointer Comparison Moment

Carter demonstrates the payoff. Two citizens from the census:

```c
/* Sine internatione: compare per characteres */
b32 aequales = chorda_aequales(
    *primus->occupatio, *secundus->occupatio);
/* 5 comparationes characterum */

/* Cum internatione: compare per pointer */
b32 eadem = (primus->occupatio == secundus->occupatio);
/* 1 comparatio. Unum. */
```

**CARTER:** Before interning: we compare every character. F. A. B. E. R. Five operations. After interning: we compare addresses. Are they the same pointer? Yes or no. One operation.

She runs the benchmark:

```
Tempus quaerendi sine internatione: 1,247 microsecundae
Tempus quaerendi cum internatione: 34 microsecundae
Acceleratio: 36.7x
```

**PLINY:** *(understanding something profound)* It is the difference between two copies of the same scroll and two hands holding the same scroll. The copies must be compared. The scroll speaks for itself.

### Beat 13: Teal'c's Test Suite

Teal'c has been building tests since Beat 6. He runs the suite.

```
probatio_internare_simplex ............ VERITAS
probatio_internare_duplicatum ......... VERITAS
probatio_pointer_aequalis ............. VERITAS
probatio_internare_ex_literis ......... VERITAS
probatio_chorda_est_internata ......... VERITAS
probatio_numerus_internatarum ......... VERITAS
probatio_internare_vacuam ............. VERITAS
```

Seven tests. Seven VERITAS.

Brief preamble -- Teal'c has one observation before the main test:

**TEAL'C:** I tested interning with the same string in different cases. "CINNABAR" and "cinnabar." They produce different canonical copies.

**CARTER:** Correct. They're different strings.

**TEAL'C:** *(noting it, filing it)* A freed slave and a free citizen have different legal status but are the same person.

Carter confirms it's by design. Teal'c nods -- seeds the case-sensitivity question for a future discussion. Then he writes one more test:

```
probatio_internare_post_mutationem .... FALSITAS
```

The room goes quiet.

**TEAL'C:** I interned a string. Then I modified the original. Then I interned the modified version. The table returned the OLD canonical copy.

**CARTER:** *(frowning)* That's correct behavior. The table compared the new string against its contents and --

**TEAL'C:** I know it is correct behavior. That is not the concern. *(beat)* The concern is that someone will intern a string, modify their local copy, and expect the canonical version to reflect the change. It will not. The canonical copy is *immutable*.

**CARTER:** The canonical copy HAS to be immutable. If one user could change the shared copy, everyone who references it would see the change. That's --

**DATA:** Assimilation. If one voice changes the canonical word, every reference to that word is changed without consent.

The room is very still.

**TEAL'C:** The canonical copy is a *treaty*. Once interned, it belongs to no one. It cannot be unilaterally revised. *(beat)* This is correct. But it must be documented and tested, because the temptation to modify a shared truth is universal.

*This is the episode's deepest test -- not a technical failure but a philosophical boundary. Immutability is the price of sharing. The canonical copy is incorruptible because no one can change it. The moment you allow modification, you have the Borg: one consciousness, mutated from the top, all drones receiving the mutation without consent.*

### Beat 14: ACT BREAK -- The Vocabulary of a Civilization

Carter runs the final statistics:

```
Internamentum Globale:
  Chordae unicae: 263 (census) + 1,203 (encyclopaedia) = 1,466
  Chordae totales referentes: 16,064
  Ratio: 1 canonica pro 10.96 referentibus
```

**CARTER:** Fourteen hundred and sixty-six words. That's the vocabulary of Nova Roma. Everything the census knows, everything the encyclopedia records -- all of it reduces to fourteen hundred unique words.

**PLINY:** *(standing at the screen, his hand on the desk)* The vocabulary of a civilization. *(beat)* I spent eighty years writing. All of it -- every observation, every classification, every measurement -- comes down to fourteen hundred words arranged in different orders.

**DANIEL:** *(gently)* That's what language is, Pliny. A finite vocabulary arranged in infinite combinations. You didn't write fourteen hundred words. You wrote forty volumes. The words are the atoms. The volumes are the world.

**PLINY:** *(a long look at the screen, then at Daniel)* Then the Internamentum is the periodic table. Every element, listed once. Every compound, assembled from the list.

---

## ACT FOUR: "Methos Meminit" (Methos Remembers)

*The emotional center of the episode. Two deeply personal scenes, simultaneous. The armor cracks. The uninternable walk together.*

### Beat 15: Methos and Daniel -- Alexandria

Evening. Daniel is working alone in Pliny's archive, cross-referencing terms between the census and the encyclopedia. Methos enters. Not his usual casual drift -- something has been working on him since the Borg discussion in Beat 8.

Methos sits down. Pours wine. He opens with the historical frame -- the scholar's register, not the personal one:

**METHOS:** The Ptolemies had a policy. Every ship that docked in Alexandria harbor -- the officials confiscated every scroll on board. Copied the ones they wanted. Returned the copies to the ship and kept the originals. The Library catalog labeled them *ek ploion* -- "from the ships."

**DANIEL:** *(the linguist, recognizing the source)* That's Galen. Third-century commentary on Hippocrates.

**METHOS:** *(a flicker -- Daniel knows the source, which means Methos can't hide behind vague attribution)* The canonical text wasn't preserved. It was *seized*. The scrolls that entered Alexandria entered a system. One copy kept. All variants eliminated. The scholars -- Zenodotus, Aristophanes, Aristarchus -- they established THE Homer, THE Hesiod, THE canon. Every subsequent copy in the Mediterranean derived from their editions.

He drinks.

**METHOS:** They didn't just choose which version was canonical. They *destroyed* the variants. The Ptolemies confiscated every scroll that entered the harbor. Copied the ones they wanted. Returned the copies and kept the originals.

Daniel has been watching Methos for months. The way Methos talks about ancient history isn't academic. It's *eyewitness*. And now -- the tense. Methos has slipped.

**DANIEL:** *(very quiet)* You said "they." Not "the Ptolemies." Not "the librarians." *They.*

**METHOS:** *(realizing his mistake, already reaching for deflection)* I've read a lot about it.

**DANIEL:** You weren't reading. You were *remembering.*

Long beat. Methos holds the wine cup very still. Five thousand years of practiced deflection calibrate the response. But Daniel Jackson isn't a casual historian. Daniel Jackson decoded the Stargate. Daniel's entire career is identifying when a text is primary source versus secondary. And Methos just spoke like a primary source.

**DANIEL:** *(not a question)* You were in Alexandria.

**METHOS:** *(the mask slipping -- Peter Wingfield's whole face changing, the "Comes a Horseman" register)* I was... in Alexandria. For a time.

**DANIEL:** You were THERE?

**METHOS:** I was there for a lot of things, Daniel. It was a big library. I was a small man with a wine habit. I shelved things.

He's trying to make it small. Mundane. But the deflection is too fast, too practiced, and Daniel can smell it.

And then it comes out -- not because Methos wants to tell it, but because the interning discussion has opened something. The concept of canonical copies touches something he has been carrying for two thousand years.

**METHOS:** The Library wasn't just scrolls. There were sub-chambers. Lower levels. The scholars had their collection above -- philosophy, mathematics, literature. Below that... *(he drinks)* There were older collections. Things that predated the Ptolemies. Things that predated Egypt.

**DANIEL:** *(the archaeologist, suddenly very alert)* What kind of things?

**METHOS:** Cults. Alexandria was a crossroads -- every mystery religion in the ancient world within a mile of each other. Isis. Mithras. Dionysus. And older ones. Ones that didn't have names anymore. *(beat)* There was a priestess. She guarded a sub-chamber. I tried to catalog what was in there once. She told me the contents weren't meant to be cataloged.

**DANIEL:** What was in the sub-chamber?

**METHOS:** Artifacts. Tablets. And a stone. Black. Dense. Warm to the touch even in winter. *(he stops)* The priestess said it spoke to people. Not in words -- in... patterns. People who spent time near it started thinking the same way. Finishing each other's sentences. Moving in unison. They called it communion. I called it something --

He catches himself. Sets down the wine cup. The humor is completely gone. Five thousand years of horror surface for a single beat.

**FLASH (10-15 seconds):** A torchlit sub-chamber. Stone walls. A woman's face -- Ptolemaic Egyptian, kohled eyes, half-shadow. Behind her: a pedestal. On the pedestal: a stone. Black. It absorbs the torchlight rather than reflecting it. The woman chanting -- under her breath, a rhythm. Behind her, in the deeper shadows, figures moving in unison. Not dancing. *Synchronized.* Their movements identical, their eyes unfocused. Young Methos -- the audience should barely recognize him, because this Methos doesn't have the sardonic mask yet, this is a younger face, more open, more frightened -- watching from behind a column. The stone seems to pulse once. The chanting stops. The priestess turns toward him. **SMASH CUT BACK TO:**

Present-day Methos, wine cup in hand, eyes distant.

**METHOS:** *(with visible effort, changing the subject)* The point is that canonical copies are a good idea. The Library proved it. Your interning system is sound. *(heading for the door)* The Library burned because it was a single point of failure. Your global singleton is the same. Think about that.

He leaves. Daniel stares at the doorway. Methos voluntarily told him about Alexandria -- something he never does -- and then stopped himself before the word that would have made it all explicit. Daniel writes in his notebook. Not the interning comparison. The other thing. The stones.

*DIRECTION NOTE: Methos's deflection must be visible -- the audience sees him catch himself. He was about to say "assimilation." The audience who heard the Borg discussion in Act Two will fill in the word themselves. This is the Nightcrawler approach: let the viewer make the connection. Methos never says it. The audience does.*

### Beat 16: Data and Midas -- "The Uninternable"

Simultaneous with Beat 15. A different room. Data and Midas are cataloging botanical specimens that Pliny has collected -- pressed flowers, leaf samples, seed pods. The beginning of a biological database. Rosa canina specimens on the table between them.

**MIDAS:** See that garden? Tertia Valeria planted it. She says the soil here reminds her of Phrygia. I told her Phrygia was nothing like this. She told me I don't remember Phrygia as well as I think. *(laughs)* She's probably right. Three thousand years is a long time to remember dirt.

Data has been processing the interning concept all day. Now, cataloging specimens with Midas, something clicks.

**DATA:** Midas. Consider the following. When a string is interned, it becomes canonical -- the one copy that all references point to. It cannot be interned a second time. It already exists. It is, by definition, *unique within the system*.

**MIDAS:** I follow you.

**DATA:** You cannot be interned.

**MIDAS:** What?

**DATA:** There is no canonical copy of Midas. There is no template from which you were derived. There is no other instance to which you could be compared. You are not a reference to a canonical form. You ARE the canonical form. The only one.

**MIDAS:** *(after a beat)* That's an odd way to be told you're alone.

**DATA:** I was not telling you that you are alone. I was telling you that you are *original*. *(beat)* I am also original. There is no canonical copy of Data. I cannot be interned. I exist as a singular instance.

**MIDAS:** Two uninternable things, walking together.

**DATA:** Precisely.

**MIDAS:** *(after a long pause, genuine warmth)* I have three thousand years of being the only one of whatever I am. And you -- how long?

**DATA:** Thirty-six years since my activation.

**MIDAS:** Thirty-six years. *(genuine warmth)* You're a child.

**DATA:** By your measure, yes.

**MIDAS:** Then I should warn you. Being the only copy of yourself... *(he looks at his hands -- the hands that once made everything they touched shift through time)* ...the loneliest part isn't that no one is like you. The loneliest part is that no one can *verify* you. When there is no canonical form to compare against, how do you know you haven't drifted? How do you know you're still *you*?

**DATA:** *(processing -- this is a profound question for an android who has always relied on self-diagnostics)* I... run internal consistency checks.

**MIDAS:** And if the checks themselves have drifted?

Data has no answer. This is the deepest question anyone has asked him on Nova Roma.

*DIRECTION NOTE: Act Four contains no code. No technical demonstration. Two scenes, two rooms, two deeply personal conversations happening simultaneously. The parallel structure -- Methos cracking open his past in one room, Data and Midas confronting their uniqueness in another -- gives the act its emotional architecture. This is the heart of the episode.*

---

## ACT FIVE: "Unum Pro Omnibus"

*The system goes live. The encyclopedia is transformed. The title earns itself.*

### Beat 17: The Interned Census Goes Live

**Location:** The Tabularium. The full team. Carter runs the interned census for the senate demonstration.

A citizen approaches: "I need the occupation records for all craftsmen in Regio Tertia."

Carter writes the query. Because all "Faber" strings now point to the same canonical copy, and all "Regio Tertia" strings point to the same canonical copy, the query compares pointers, not characters.

```c
/* Quaerere omnes fabros in Regio Tertia */
chorda* faber_canonica = chorda_internare_ex_literis(
    intern, "Faber");
chorda* regio_canonica = chorda_internare_ex_literis(
    intern, "Regio Tertia");

per (i = ZEPHYRUM; i < census->numerus; i++)
{
    Civis* civis = xar_elementum(census, i);
    si (civis->occupatio == faber_canonica &&
        civis->regio == regio_canonica)
    {
        /* Inventus! */
    }
}
```

The results appear instantly.

**MARCUS:** *(who has been running census queries all week)* That query took four minutes yesterday.

**CARTER:** Yesterday, every comparison was character-by-character. Today, every comparison is a single check: same pointer, or not.

**MARCUS:** Four minutes to microseconds. *(a long beat)* Every Roman magistrate who has ever waited for a census query will understand this improvement.

### Beat 18: Pliny's Encyclopedia Interned

Pliny has been quiet since Beat 14. He approaches the screen.

**PLINY:** My encyclopedia. Intern it.

Carter runs the transformation on Pliny's twelve hundred entries. The memory visualization shrinks. Cross-references collapse. Every time Pliny wrote "cinnabar" in a different volume, the word now points to the same canonical copy.

**PLINY:** *(watching the cross-references form)* Every volume that mentions cinnabar... is now connected. Not by my index. By the word itself.

**CARTER:** When the same canonical string appears in multiple entries, those entries share a reference. The connection is structural, not annotated.

**PLINY:** *(his eyes bright)* I spent forty years building cross-references by hand. Noting that Volume 7 mentions the same mineral as Volume 23. Writing the connections in ink. *(beat)* The Internamentum creates the connections by nature. The shared word IS the cross-reference.

*This is the unexpected payoff. Interning isn't just about saving memory. It creates a semantic web -- entries that share canonical strings are structurally linked. Pliny's lifelong dream of a fully cross-referenced encyclopedia happens automatically as a side effect of interning.*

### Beat 19: The Title Moment

**Location:** The Tabularium. Late day. The system running. The full team gathered around the board.

Carter writes the final statistics:

```
INTERNAMENTUM NOVAE ROMAE

Chordae canonicae: 1,466
Referentiae totales: 16,064
Memoria conservata: 97.1%
Comparationes acceleratae: 36.7x

Dependentiae:
  Piscina (memoria)
  Chorda (verba)
  Tabula Dispersa (index)
  Internamentum (canon)
```

**LUCIA:** *(reading the board)* Piscina holds the memory. Chorda holds the words. Tabula Dispersa finds the words. Internamentum makes the words one.

**MARCUS:** *(studying the dependency chain)* An aqueduct is useless without a reservoir. A reservoir is useless without pipes. Pipes are useless without a source. You have built a source, a reservoir, pipes, and a delivery system. Each one necessary. Each one insufficient alone.

**PLINY:** *(writing the dedication, his steady hand)* *Unum pro omnibus, non omnes pro uno.* One for all -- not all for one. The word serves the people. The people do not serve the word.

*This is the episode's final thesis statement. The Borg version of "unum pro omnibus" is "all serve the one" -- every drone subsumed into the collective. The Nova Roma version is the reverse: one canonical truth serves all users, but the users remain sovereign. The word is shared. The person is not.*

---

## TAG

### Beat 20: Carter and Marcus -- The Evening Walk

Their ritual. The road out of Nova Roma. Stars overhead.

**MARCUS:** The dependency chain. Four libraries deep. Six episodes to build.

**CARTER:** It'll be deeper by the end of the season.

**MARCUS:** *(walking in silence for a moment)* In Rome, the greatest buildings are the ones where you cannot remove a single stone without the structure failing. Every stone bears weight. Every stone is necessary. *(beat)* Your four libraries. Can any be removed?

**CARTER:** No. Remove the arena, everything collapses. Remove strings, the hash table has nothing to hash. Remove the hash table, the interning table has no way to find existing copies.

**MARCUS:** Then you have built something Roman. *(beat)* I do not say that lightly.

They walk. The stars are bright.

**CARTER:** Next, we need to save things to disk. Everything we've built lives in memory. If the machine stops, it's gone.

**MARCUS:** *(the Roman, who understands stone vs. wax)* Then we need to carve what we've written in wax into stone.

**CARTER:** Exactly. *(beat)* File I/O.

**MARCUS:** Is that 1x07?

**CARTER:** *(smiling)* That's 1x07.

### Beat 21: Data/Midas Callback + Methos Silent

Two sub-beats:

**Sub-beat 1:** Data returns to Midas. Later in the evening. The botanical specimens are put away. Midas is sitting, looking at the stars.

**DATA:** I have been considering your question. About verification without a canonical reference.

**MIDAS:** And?

**DATA:** I believe the answer is: other people. If I cannot verify myself against a canonical copy, I can verify myself against the observations of those who know me. You remember who I was yesterday. If I drift, you will notice.

**MIDAS:** *(slowly)* You're saying friendship is a form of external verification.

**DATA:** I am saying that friendship is what you use when interning is impossible.

**MIDAS:** *(the warmest smile we've seen from him)* Then I'm glad I know you, Data. You can be my consistency check.

**DATA:** And you mine.

*45 seconds. Warm. Quiet. Two uninternable things who have found their answer.*

**Sub-beat 2:** Methos alone in the archive. Post-conversation with Daniel. He pours wine. The interned data is on screen -- the census, the encyclopedia, 1,466 canonical words glowing in the dim light. Methos stands before the screen. He touches it where the word "canonica" appears. His hand lingers.

He drinks. He looks at the screen the way you look at something you've seen before, in a different room, in a different century, lit by torchlight instead of electricity.

No dialogue. 30 seconds. The silence is the statement.

FADE TO BLACK.

---

## STINGER

### Beat 22: Kirk's LEXICON

**Location:** Earth. Night. A conference room in a real office building -- not a strip mall, not a co-working space. Kirk has graduated. The walls are glass. The desks are real. The company has a name now, visible in frosted lettering on the glass: "Synthesis Dynamics" -- the kind of name that tells you nothing.

Kirk stands at the head of a conference table. Six people seated. They don't look Borg -- they look like middle management. Polo shirts and khakis. But they sit identically. They listen identically. They take notes at the same moment.

On the screen behind Kirk: a document. Not a map. Not a directory. A *style guide*.

**KIRK:** *(reading from the screen)* "When contacting a recruitment target, use the phrase 'leadership opportunity' -- not 'career change,' not 'new role,' not 'position.' The phrase is 'leadership opportunity.' When describing the organization, use 'aligned network' -- not 'company,' not 'group,' not 'team.' Every communication follows this lexicon. Every interaction uses the same words."

He pauses. Turns to the room.

**KIRK:** Why?

**AIDE:** *(robotically eager)* Consistency creates trust. The target hears the same language from multiple contacts and concludes it's an institution, not a scheme.

**KIRK:** That's one reason. The deeper reason: when everyone uses the same words, you can search for those words. Our monitoring tools flag "leadership opportunity" across every platform -- email, text, social media. If someone uses the phrase, they're ours. If they use a different phrase, they're freelancing.

He writes on the whiteboard: **LEXICON**.

Below it: **6 / 31**.

A new city on the map -- somewhere east. The network is growing.

**KIRK:** One language. One vocabulary. One set of canonical phrases. Everything searchable, everything standardized, everyone interchangeable.

He closes the laptop. Straightens his tie. Says it to himself, almost warmly, like a man who has found the right word for what he's building:

**KIRK:** Unum pro omnibus.

The camera pulls back through the glass wall. The conference room shrinks. The building is one of hundreds in a business park. Identical buildings. Identical glass. Identical frosted names. From the air, they look like slots in a table.

SMASH TO BLACK.

END OF EPISODE.

---

## ON-SCREEN CODE MOMENTS

### Moment 1: The Redundancy Query (Cold Open)
```c
i32 chordae_totales = III_CCXVII;
i32 chordae_unicae = CCLXIII;
f32 redundantia = I - ((f32)chordae_unicae / (f32)chordae_totales);
/* 91.8% redundantes */
```

### Moment 2: The Include Chain (Act 2)
```c
#include "internamentum.h"
/* <- chorda.h <- piscina.h <- tabula_dispersa.h */
```
Four libraries. Six episodes. The dependency chain visualized.

### Moment 3: The Core Intern Function (Act 2)
```c
si (tabula_dispersa_invenire(intern->tabula, s, &valor_existens))
{
    redde (chorda*)valor_existens;  /* Iam canonica */
}
canonica = piscina_allocare(intern->piscina, magnitudo(chorda));
*canonica = chorda_transcribere(s, intern->piscina);
tabula_dispersa_inserere(intern->tabula, *canonica, canonica);
redde canonica;
```

### Moment 4: The Global Singleton (Act 2 Break)
```c
hic_manens InternamentumChorda* _internamentum_globale = NIHIL;
/* Pigra initialisatio -- creatur in primo usu */
```

### Moment 5: Census Interning Loop (Act 3)
```c
civis->occupatio = chorda_internare(intern, *civis->occupatio);
civis->regio = chorda_internare(intern, *civis->regio);
civis->via = chorda_internare(intern, *civis->via);
```

### Moment 6: Pointer Comparison (Act 3)
```c
/* Sine internatione: O(n) per characteres */
b32 aequales = chorda_aequales(*primus->occupatio, *secundus->occupatio);

/* Cum internatione: O(1) per pointer */
b32 eadem = (primus->occupatio == secundus->occupatio);
```

### Moment 7: The Interned Query (Act 5)
```c
si (civis->occupatio == faber_canonica &&
    civis->regio == regio_canonica)
```
Two pointer comparisons. Microseconds.

### Moment 8: Memory Statistics (Act 5)
```
INTERNAMENTUM NOVAE ROMAE
  Chordae canonicae: 1,466
  Referentiae: 16,064
  Memoria conservata: 97.1%
  Acceleratio: 36.7x
```

---

## C-PLOT THREADING

### Beat C1 (Early Act 1): Kirk's Acquisitions

O'Neill and Duncan reviewing intelligence. Brief, efficient, establishing.

**O'NEILL:** Three more acquisitions this week. A logistics startup in Portland, a data analytics firm in Denver, and a consulting group in -- *(checks)* -- Boise.

**DUNCAN:** Boise?

**O'NEILL:** Kirk's not building a tech company. He's building an organization. Logistics, analytics, consulting -- these are capabilities, not businesses.

**DUNCAN:** He's not buying products. He's buying *functions*.

**DUNCAN:** *(quiet, the Immortal who watches patterns across centuries)* Every empire that tried to grow by absorbing.

He doesn't say "the Borg." He doesn't know about the Borg yet. But the pattern is old enough that a 400-year-old swordsman recognizes it.

**Placement:** Between Beats 1-2, during the morning briefing sequence.

### Beat C2 (During Acts 2-3): The Compass Update

Carter checking her instruments during a break from the technical work. Midas walks past. The needle spins -- but differently this time. Instead of 1x05's full 360-degree rotation, it stutters. Half-rotation. Pause. Quarter-rotation. Pause. Returns to north.

Carter marks it. Compares to her notes from 1x05. The spin pattern is changing. The field around Midas isn't constant -- it *fluctuates*.

**CARTER:** *(to herself, writing)* Variable amplitude. The effect isn't decaying -- it's oscillating.

She places a second instrument -- a small device that measures electromagnetic field strength -- in a spot she's marked with chalk. Two data sources instead of one. Carter the scientist, building her experiment.

**Placement:** Between Beats 10-11, during the act break transition.

### Beat C3 (During Acts 3-4): Duncan Recognizes Kirk's Pattern

Duncan has been thinking about the acquisitions. He finds O'Neill.

**DUNCAN:** The Portland startup Kirk acquired -- what was their specialty?

**O'NEILL:** Internal communications platforms. Workplace messaging.

**DUNCAN:** And the Denver firm?

**O'NEILL:** Data normalization. Taking messy data from multiple sources and making it consistent.

**DUNCAN:** *(slowly)* Communications. Normalization. He bought the ability to make everyone talk the same way, and the ability to make all the data look the same. *(beat)* He's not indexing anymore. He's *standardizing*.

**O'NEILL:** *(grim)* Same thing the team built today. One copy of each word.

**DUNCAN:** Except the team chose which words already existed. Kirk is choosing which words *will* exist.

**Placement:** Between Beats 14-15, during the Act Three/Four transition.

### Beat C4 (Late, Post-Climax): Data's Eyes

A quiet scene. Late. Midas and Data sitting together.

**MIDAS:** Data. Your eyes.

**DATA:** What about them?

**MIDAS:** They were green when you arrived. Now they're... more green some days than others. Have you noticed?

**DATA:** I have not. Green is my standard ocular pigmentation, though I acknowledge the lighting on Nova Roma differs from --

**MIDAS:** *(casually)* Yesterday they were almost gold.

Data pauses. This is the first indication -- subtle, easily dismissed -- that proximity to Midas is affecting Data too. Not temporally, but *chromatically*. Midas's temporal field is interacting with Data's optical systems.

**DATA:** I will run a self-diagnostic.

**MIDAS:** *(already moved on)* Do you want more wine?

**Placement:** After Beat 19, before the Tag. The scene ends warm. Two friends. One made of circuits, one leaking time. Neither knows what they are to each other yet.

---

## ENSEMBLE BEATS

### Carter -- The Implementer Under Strain
Carter drives this episode more than any since 1x01. She identifies the waste, proposes the solution, implements it, and defends it against Methos's philosophical challenge. But she's also carrying the compass data, the Midas temporal file, and the growing awareness that something is wrong. Her two-line exchange with Casey in Beat 11 is the crack that shows the weight. She's the builder -- but the builder is carrying secrets.

### Marcus -- The Bridge Builder
Two episodes as design partner now. Marcus is comfortable. His metaphors carry the episode's Roman dimension: the road sign, the public well, the aqueduct, the stone that bears weight. His closing line -- "Then you have built something Roman" -- is the highest compliment a man like Marcus can give.

### Pliny -- Energized, Seeing Waste
Pliny just had his life's work made searchable in 1x05. Now he's seeing the waste -- and he's offended. His reaction to 91% redundancy is personal: "I have written *cinnabar* four hundred and twelve times." His role shifts from 1x05's emotional center to 1x06's motivated user. His encyclopedia interning in Beat 18 -- the auto-cross-references -- is his payoff.

### Daniel -- The Archaeologist-Investigator
Daniel bridges the linguistic and the archaeological. He names the process with Lucia. He pushes Methos in Act Four -- not aggressively, but with the precision of a man who identifies primary sources. He writes in his notebook. He files. He'll be back.

### Lucia -- The Lexicographer
"I have been interning words since I was twelve." Lucia's authority in this episode is her training -- canonical forms, lemmas, forma canonica. She is the one who writes the episode title on the board. The naming scene is her, Daniel, and Pliny in the best three-way exchange since the show started.

### Methos -- The Cracked Armor
The Borg parallel in Act Two. The Alexandria revelation in Act Four. The silent Tag. Methos's arc this episode is a controlled demolition of one specific wall in his five-thousand-year-old defense system. He says more than he's said in five episodes combined -- and then stops himself before the word that would explain everything. The audience gets a keyhole. The full door is 1x17.

### Data -- The Philosopher of Uniqueness
Data provides the analytical precision (examination to recognition, the Borg distinction) and the philosophical depth (the uninternable conversation with Midas). His friendship callback in the Tag -- "friendship is what you use when interning is impossible" -- is one of the most Data lines in the series.

### Midas -- The Warm Anomaly
Midas doesn't know what he is. He laughs easily, tends gardens, calls Data "a child" with genuine affection. His self-verification question ("how do you know you haven't drifted?") is the deepest thing anyone says to Data this season. The eye-color observation in C4 hints at what neither friend understands yet.

### Teal'c -- The Treaty Maker
"The canonical copy is a treaty." Teal'c's contribution is the philosophical test -- not a bug but a boundary. Immutability as the price of sharing. The freed slave who insists that shared truth cannot be unilaterally revised.

### O'Neill -- Two Load-Bearing Lines
"You're saying we're building Borg tech" (Beat 8) -- the instinct. "One table. One point of failure" (Beat 10) -- the strategic mind. O'Neill's quieter this episode, but both lines are load-bearing. His C-plot presence with Duncan tracks Kirk's growth. Sufficient.

### Casey -- The Quiet Center
The faith thread rests. Casey brings coffee. He says "One for all isn't the same as all for one" -- the beat's best line. He tells the story of his grandfather's Bible -- the episode's most human moment. The crucifix is in frame during the canonical discussion. No dialogue about it. He notices Carter's weight and doesn't push. Casey is the still point around which the episode's moral argument orbits, and he says almost nothing.

### Duncan -- The Pattern Reader
C-plot only. His value: "He's not indexing anymore. He's *standardizing*." Four centuries of watching empires gives Duncan the ability to name what Kirk is doing before anyone else can.

---

## THEMATIC NOTES

### Interning vs. Assimilation -- The Episode's Central Argument

The parallel is established across three registers:

| | Team (Interning) | Kirk (Standardizing) | Borg (Assimilating) |
|---|---|---|---|
| **What's unified** | A word | A phrase | A person |
| **The canonical form** | Already existed | Imposed by authority | Imposed by force |
| **The sources** | Remain distinct | Become interchangeable | Are destroyed |
| **Purpose** | Efficient comparison | Efficient monitoring | Efficient expansion |
| **Philosophy** | Realist: nature determines form | Nominalist: authority determines form | Anti-realist: form replaces nature |

Kirk is the middle position -- not yet Borg, but moving toward them. He imposes canonical phrases (nominalist), but his operatives are still individual humans (not yet drones). The gradient from interning to assimilation: first you standardize the language, then you standardize the people, then there's nothing left to standardize because there's nothing left that's different.

### Nominalism / Realism -- The Deepest Layer Yet

String interning is the most explicitly realist operation the team has built. The realist claim: things have natures. Two instances of "Faber" are not merely similar -- they ARE the same word. They share a nature. Interning *recognizes* this. The canonical copy IS the universal made concrete. The universal isn't separate from the instances (it's in the same memory), but it's logically prior to them (it was stored first, they point to it).

This is moderate realism articulated as a data structure: universals are real and can be instantiated. The Borg's "interning" is anti-realist: the canonical form replaces nature. Kirk's standardization is nominalist: the canonical form is imposed by authority, not discovered in nature. The team's interning is realist: the canonical form emerges from what the string already IS.

### The Borg Inversion -- Lossy vs. Lossless

The Borg's "interning" is LOSSY. Locutus retained Picard's tactical knowledge but lost his poetry, his love of Earl Grey, his archaeological curiosity. The Borg kept the function and discarded the nature.

The team's interning is LOSSLESS. The canonical copy is a FULL TRANSCRIPTION: `chorda_transcribere` -- every byte preserved. Nothing lost. The string's complete nature is what gets stored. When other instances point to the canonical copy, they're pointing to the WHOLE thing, not a functional reduction.

### The Musketeers -- The Unspoken Half

"Unus pro omnibus, omnes pro uno" -- One for all, all for one. The show uses only the first half. The second half -- "all for one" -- is the Borg's version. Leaving it unspoken is the point. The team builds the first half. The Borg demand the second. Genre-literate fans will catch the half-motto. Dumas fans will love it.

### The Singleton as Self-Referential Principle

The global interning table is itself interned -- there is only ONE canonical internamentum. `internamentum_globale()` is a true singleton: it serves whoever calls it. It doesn't control the strings it stores. It doesn't modify the content. The Borg Queen is a corrupted singleton -- she controls rather than serves. The distinction between a queen and a service.

### Deferred Genre Connections (for Series Bible)

- **Hugh (TNG "I, Borg"):** De-assimilation as reverse interning. Hugh went from pointer-to-Collective to canonical individual. Geordi naming him was the `chorda_internare` call that created his independent entry.
- **Seven of Nine:** Her recovery across four Voyager seasons was re-interning. She became a NEW canonical version -- not restored Annika, but Seven. The canonical copy isn't reversion; it's rebirth with preserved nature.
- **"Darmok" (TNG):** The Tamarians' language is entirely interned metaphors, but the canonical store requires cultural context to dereference. Warning: private interning is meaningless. The canonical version must be shared.
- **BSG "All this has happened before":** A universe without interning. Every cycle, the same events treated as new instances. The internamentum breaks the cycle by making repetition VISIBLE.
- **Foundation (Asimov):** Pliny as Seldon at the Second Crisis -- realizing the Encyclopedia needs canonical content, not just content. The linguistic drift across Foundation worlds is the failure of interning at civilizational scale.

### The Title's Four Layers

1. **Technical (stated explicitly):** One canonical copy for all references. The A-plot.
2. **Borg (stated explicitly by Methos):** One consciousness for all drones. The dark mirror.
3. **Theological (visual only -- Nightcrawler approach):** The crucifix in frame. "One for all" = one sacrifice for all. Signal zero. Resonance entirely in the viewer.
4. **Musketeers (implied by absence):** Only the first half of the motto used. The missing second half IS the Borg version.

---

## GENRE ARCHITECTURE

### The Borg Parallel -- From Subtext to Text

This is the episode where the show's central metaphor goes from subtext to TEXT. Five episodes of infrastructure. Now the enemy's philosophy is named. The distinction: the Borg's "interning" is lossy (keeps function, discards nature); Nova Roma's interning is lossless (full transcription, complete nature preserved). Same efficiency. Opposite philosophy. This is the thesis statement for the next hundred episodes.

### Foundation -- Pliny as Seldon

In 1x05, Pliny was Seldon building the Encyclopedia. In 1x06, Pliny is Seldon at the standardization crisis -- realizing that the Encyclopedia needs canonical terms, not just content. Multiple copies of "ferrum" across scrolls might have slightly different definitions. The internamentum picks one and makes it authoritative. "An encyclopedia without canonical terms is a collection of opinions."

### SG-1 "The Fifth Race" (S2E16)

O'Neill had an entire alien database dumped into his brain. Unindexed. Unsearchable. Storage without retrieval. He LIVED the problem that interning solves -- millions of entries, no canonical references, no way to look anything up.

### Highlander -- Methos's Findability

For 5,000 years, Methos survived by being UN-internable -- thousands of different identities, none canonical. Now the internamentum canonicalizes him. One authoritative "Methos" in the system. Five thousand years of carefully maintained multiplicity, reduced to a single canonical truth.

### TNG "The Offspring" -- Lal Resonance

Data created Lal -- attempted to create a canonical copy of himself. Lal died. Data's uniqueness is not a feature; it's a fate. When the team demonstrates that any string CAN be interned, Data's 10-second silence during the demonstration is the statement the fans will catch: *I am the string that cannot be interned.*

*DIRECTION NOTE: Data's silence during the interning demo -- the android who has opinions about everything going quiet -- IS the Lal resonance. Don't call it out. Let it sit.*

### The Dependency Chain as Season Architecture

The code's `#include` directives are literally the show's dependency graph: `piscina.h` (1x01) -> `chorda.h` (1x03) -> `tabula_dispersa.h` (1x05) -> `internamentum.h` (1x06). Four episodes, imported. The season arc IS the dependency chain. Each episode builds on the last.

---

## CONTINUITY NOTES

### Backward Connections

**From 1x01 (Piscina):** All interned strings are allocated from the piscina. The arena allocator now serves four layers. The dependency chain:
```
Piscina (memory, 1x01)
  -> Chorda (text, 1x03)
  -> Xar (collection, 1x04)
  -> Tabula Dispersa (index, 1x05)
  -> Internamentum (canonical copies, 1x06)
```

**From 1x02 (Credo):** Teal'c's testing methodology. His immutability test here echoes 1x02's philosophy: the test reveals a property the system should have, not a bug.

**From 1x03 (Chorda):** The keys are Chordae -- length-prefixed, not null-terminated. The interning system uses the Chorda's known length to compute hashes efficiently. Daniel and Lucia's lexicon expands.

**From 1x04 (Xar):** The Xar holds the census data. The Tabula Dispersa indexes it. The Internamentum canonicalizes its vocabulary. Three layers of the stack, each enabling the next.

**From 1x05 (Tabula Dispersa):** The interning system IS a hash table -- the Tabula Dispersa from 1x05, repurposed. The audience sees the hash table as infrastructure -- a building block reused for a new purpose. Also: Methos's 3-second reaction to the compass spinning near Midas (1x05, C3) NOW pays off in the Alexandria revelation. The audience who caught the 1x05 seed gets the satisfaction of seeing it sprout.

**From 1x05 (Pliny/Casey):** The faith thread rests. But Pliny's relationship with Casey -- the scholarly curiosity, the "more evenings" -- is background warmth. They're working together. The questions will come later.

### Forward Connections

**To 1x07 (Filum / File I/O):** Carter's closing line: "Everything we've built lives in memory. If the machine stops, it's gone." Methos's fire warning from 1x05 still echoes. The interned strings are canonical, efficient, fast -- but volatile. The need to persist motivates file I/O.

**To 1x08 (Horologium / Tempus):** Carter's growing temporal data set (compass + oscillation pattern) needs reliable timestamps. But the clocks disagree. The time library is motivated by making Carter's anomaly measurements trustworthy.

**To 1x09 (Arbor / XML):** Interned strings are flat -- just words. But Pliny's encyclopedia has structure: minerals belong to geology, geology belongs to natural sciences. The flat interned keys need hierarchical context. The XML tree provides it.

**To 1x13 (Aurum / Midas flashback):** Daniel now has a thread to pull: Alexandria, sub-chamber, artifacts, a priestess, cults that change people. The audience has: Methos's 1x05 reaction + Methos's 1x06 revelation + compass data + clock oscillation + Data's eyes changing color. Eight episodes of accumulated clues converge in the Midas flashback.

**To 1x17 (Quinque Milia Annorum / Methos flashback):** Methos's "I called it something --" explicitly creates an expectation of more. When 1x17 delivers the full flashback -- the sub-chamber, the stone, the priests who heard whispers -- the audience will remember that Daniel was given the first fragment here. Eleven episodes of Daniel pulling the thread.

**Kirk's stinger parallel:** In 1x04, Kirk's network was a dark Xar -- nodes that never move. In 1x05, Kirk's whiteboards were a dark Tabula Dispersa -- names indexed by capability. In 1x06, Kirk's style guide is a dark Internamentum -- canonical phrases imposed by authority. Same infrastructure, opposite purposes. The progression: TABULARIUM -> LEXICON.

---

## ADDITIONAL NOTES

### Production Design -- The Methos Flash

10-15 seconds. The visual must establish:
- **Setting:** Below the Serapeum. Stone corridors, oil lamps, narrowing passages. Rougher stone than the library above -- a different construction era. Something that was here before the Serapeum was built on top of it.
- **The Priestess:** Ptolemaic Egyptian. Kohled eyes. Half-shadow. Chanting under her breath -- a rhythm, not a song.
- **The Stone:** On a low pedestal. Black. Dense. It absorbs torchlight rather than reflecting it. NOT displayed prominently -- wrapped in cloth on a low table. Its power is a secret, not a spectacle.
- **The Figures:** Behind the priestess, in deeper shadow. Moving in unison. Synchronized. Their eyes unfocused.
- **Young Methos:** Barely recognizable. No sardonic mask. More open. Frightened. Watching from behind a column.
- **Sound Design:** The library above should sound alive (murmured Greek, scratching pens, harbor sounds). The sub-chamber should sound dead (silence, dripping water, distant settling stone). When the priestess speaks, her voice echoes.

*The flash establishes the stone, the synchronized behavior, and Methos's fear. The audience now knows why he reacted in 1x05 (C3). This pays forward to 1x17.*

### Performance Notes

**Methos (Peter Wingfield):** The "Comes a Horseman" register for the revelation. The humor drops. The eyes go flat. Five thousand years of horror surface for a single beat, and then the mask goes back on. The distinction from "Comes a Horseman": there, the horror was what Methos DID. Here, the horror is what he SAW.

**Data (Brent Spiner):** 10-second silence during the interning demo. The android who has opinions about everything going quiet. The Lal resonance -- the string that cannot be interned. Do not explain it. Let it sit.

**Pliny:** His reaction to the auto-cross-references in Beat 18 is not wonder but *relief*. The cross-references exist. They were always there. The interning made them visible. This is a man who has been building something for forty years and just discovered the machine can finish it.

### Historical Grounding

The Ptolemaic ship-search policy (Galen source), the Serapeum underground chambers (Rufinus), the Cybele cult's documented presence in Alexandria, the Zenodotus-Aristophanes-Aristarchus chain as the first textual interning project -- these are all historically attested. Methos's revelation is fictive but sits within a real cultural matrix.

The naming scene uses historically grounded Latin: *exemplar* (classical -- master copy), *internare* (medieval -- to place within), *forma canonica* (ecclesiastical -- the standard form). Pliny's objection ("We already have a word -- *exemplar*") and eventual preference for *internamentum* is a legitimate Latin linguistic debate.

---

## THREAD STATUS APPENDIX: After 1x06

| Thread | Status After 1x06 | Next Beat |
|--------|-------------------|-----------|
| **Casey's Faith** | **RESTING.** Crucifix visible during "canonical" discussion. No dialogue. Grandfather's Bible speech is personal memory, not faith beat. The thread breathes after 1x05's major escalation. | 1x07-08: Casey says grace. O'Neill notices. |
| **Kirk Stinger** | **6/31.** Kirk has moved from organization to standardization. "LEXICON" on the whiteboard. Canonical phrases. Standard protocols. He's interning language. Real office building -- he's corporate now. | 1x07: Kirk continues to grow. Stinger progression tracks the season's library progression. |
| **C-Plot (Kirk/Earth)** | **ACTIVE.** Three acquisitions -- logistics, analytics, consulting. Kirk buying capabilities by category. Duncan recognizes the imperial pattern: "He's not indexing. He's standardizing." | Ongoing. Kirk's acquisitions accelerate through mid-season. |
| **Black Stone / Cybele** | **ESCALATED.** Methos revealed Alexandria: sub-chamber, priestess, artifacts, cults that change people. Tense-slip caught by Daniel. Flash shown to audience. Methos stopped himself before "assimilation." Daniel filed it. He'll pursue. | 1x13: Midas flashback shows the Phrygian stone and the Cybele cult. 1x17: Methos's own flashback -- the full sub-chamber. |
| **Midas Temporal** | **GROWING.** Compass now oscillating (not just spinning). Carter building multi-source data set. Data's eye color shifting (Midas/Data private observation, not yet known to Carter). | 1x08: Full clock episode. Carter has weeks of data. Casey's time library enables proper measurement. |
| **Nominalism / Realism** | **DEEPEST YET.** Interning as moderate realism: the canonical form IS the universal, instances point to it. The Borg mirror: assimilation imposes identity, interning recognizes it. Kirk's standardization as the nominalist middle position. | Ongoing. Each library adds to the realist case. The thesis is never stated. |
| **Pliny's Mortality** | **BACKGROUND.** Pliny is present, energized, contributing -- but not spotlighted after his major 1x05 episode. He's using the systems he helped build. The auto-cross-references are his quiet triumph. | Ongoing through S1-3. The mortality thread is implicit in every scene where Pliny works. |
| **Data/Midas Friendship** | **DEEPENING.** The "uninternable" scene. The self-verification question. The "friendship is external verification" callback. The eye-color observation. Two anomalies keeping watch over each other. | Ongoing. By 1x13 and 2x11-13, this friendship is the emotional core of the show. |
| **Daniel as Investigator** | **ACTIVATED.** Daniel now has fragments: Alexandria, sub-chamber, priestess, cults, artifacts, a word Methos caught himself before saying. His archaeologist's instinct is engaged. He will pursue. | 1x09 (XML): Daniel working with ancient texts. 1x13/1x17: The thread pays off. |
| **Methos's Secret** | **PARTIALLY REVEALED.** He confirmed Alexandria. He described the sub-chamber. He mentioned the priestess and the cults. He stopped himself. The audience knows there's more. Methos knows more than he's telling, and now Daniel knows that Methos knows more. | 1x17: The full revelation. The stone, the whispers, the connection to the gate technology. |
| **Carter's Secret Burden** | **GROWING.** Compass data accumulating. Midas file unshared. The two-line exchange with Casey ("What truth are you carrying?" / "Too many.") signals the weight is building. | 1x08: Carter needs reliable timestamps for her data. The burden increases as the data set grows. |

---

## OPEN QUESTIONS

None. All open questions from the compilation plan have been resolved in the beat sheet.

---

*Compiled from A-Plot Technical Pitch, Character & Flashback Pitch, Serialized Threads Pitch, Genre Fan Pitch, and Historian Pitch per the Compilation Plan decisions document.*
