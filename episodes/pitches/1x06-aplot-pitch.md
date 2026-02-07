# 1x06 "Unum Pro Omnibus" — A-Plot Technical Pitch
## The Dramatic Engine, Act Structure, and On-Screen Code

---

## THE DRAMATIC ENGINE

### The Problem

The hash table from 1x05 has made the census searchable. Pliny's encyclopedia is indexed. One-probe lookup. The system works.

But Carter has been running diagnostics overnight. The statistics tell a story the team didn't expect:

```
Status Tabulae Census:
  Capacitas: 4096
  Numerus: 3217
  Memoria allocata: 847,293 bytes
```

Eight hundred and forty-seven thousand bytes. For three thousand citizens.

Carter digs deeper. She queries the string allocations. The word "Faber" — craftsman — appears two hundred and three times in the census. Each time, a separate string. Two hundred and three separate allocations. Two hundred and three separate hash computations when comparing occupations.

"Forum Stellarum" appears in the address field of four hundred and twelve citizens. Four hundred and twelve copies of the same fourteen characters.

The district names: "Regio Prima," "Regio Secunda," through "Regio Duodecima." Twelve districts, but the district strings are copied for every citizen. "Regio Prima" alone: three hundred and nineteen copies.

**The hash table didn't create waste. It revealed it.** The census was always this redundant — three thousand people living in twelve districts, working in fifty occupations, residing on two hundred streets. The actual *vocabulary* of the census is small. But every entry carries its own private copy of every word. As if every citizen of Rome carried their own personal copy of the Latin dictionary in their pocket, with most pages identical.

This is the dramatic engine: **the waste of individuality applied where universality belongs.** Not every string needs to be unique. Some strings are *shared truth* — canonical facts that every record should reference rather than duplicate. "Faber" doesn't become a different word when a different person is a craftsman. The word is one. The craftsmen are many.

### Why This Matters Dramatically

The waste is not just computational. It's philosophical. It mirrors every system that treats interchangeable truths as if they were unique per person — the modern proliferation of "my truth" where a shared reality would serve better. And it mirrors the Borg's opposite error: where the Borg destroy individuality to achieve unity, the current census *destroys unity* by treating every shared word as individual. Both are wrong. Interning finds the middle path: the word is one, the people using it are many.

---

## COLD OPEN

**Location:** Carter's workshop. Pre-dawn. The screen glows. Carter has been up all night.

On screen: a visualization. The census data rendered as a memory map — blocks of color, each representing an allocated string. The visualization looks like a quilt. Patches of identical color scattered everywhere. Red patches (district names) — hundreds of them, but only twelve unique values. Blue patches (occupation names) — hundreds, but only fifty unique values. Green patches (street names) — hundreds, but only two hundred unique values.

Carter is looking at a quilt made of repetition.

She types a query:

```c
/* Computare redundantiam */
i32 chordae_totales = III_CCXVII;
i32 chordae_unicae = CCLXIII;
f32 redundantia = I - ((f32)chordae_unicae / (f32)chordae_totales);
/* redundantia = 0.918 — 91.8% redundantes */
```

Three thousand, two hundred and seventeen strings. Two hundred and sixty-three unique values. Ninety-two percent of every string in the census is a duplicate of something already there.

**CARTER:** *(to herself, staring)* Ninety-two percent.

She picks up a census scroll — Pliny's original — and begins counting. "Faber... Faber... Faber..." Each one written out in full. Each one using ink, using space, using Pliny's time. She's counting the waste in analog before she shows it in digital.

**CARTER:** *(setting down the scroll)* Every census-taker in Rome wrote the same fifty occupation names three thousand times. They could have written the word once and pointed to it.

She opens her notebook. Writes one word at the top of a blank page:

**CANONICA**

SMASH TO MAIN TITLES.

---

## ACT ONE: "Redundantia" (The Redundancy)

*The problem is named. The census reveals the waste. The team begins to understand what "canonical" means.*

### Beat 1: Carter's Morning Report

**Location:** The Tabularium. Morning. The team gathers. Carter has her visualization on screen — the quilt of redundancy.

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

### Beat 2: Pliny's Reaction — The Encyclopedist's Horror

Pliny has been listening. He approaches the screen.

**PLINY:** Show me my encyclopedia.

Carter runs the same analysis on Pliny's indexed data:

```
Chordae in encyclopaedia: 12,847
Chordae unicae: 1,203
Redundantia: 90.6%
```

Pliny sits down slowly. Twelve thousand entries. Twelve hundred unique terms. He has been writing the same words — genus names, mineral categories, geographic regions, measurement units — thousands of times across forty volumes.

**PLINY:** I have written *cinnabar* four hundred and twelve times. *Hispania* six hundred and three times. *(beat)* In forty volumes of careful observation, nine-tenths of my words are things I have already said.

**DANIEL:** *(from the corner)* Every language works that way. A few hundred words carry ninety percent of all communication. The vocabulary that matters is small. The *usage* is vast.

**PLINY:** *(to Carter, with the directness of a man who just learned he wasted eighty years of ink)* Then fix it.

### Beat 3: Daniel and Lucia — The Linguists Name It

Daniel and Lucia are already thinking.

**DANIEL:** This is a linguistic problem, not a computational one. Every language has a concept of the *canonical form* — the dictionary entry, the root word, the lemma. "Fabrum," "fabri," "faber" — the canonical form is "faber."

**LUCIA:** *(precisely)* In my lexicon, each word appears once. When I use it in a sentence, I do not re-create the word. I *reference* the lexicon. The sentence points to the word. The word exists once.

**DANIEL:** In English, we call it a "dictionary form." In linguistics, a "lemma." In Latin —

**LUCIA:** *Forma canonica.* The standard form. The one true copy from which all uses derive.

**CARTER:** *(writing it down)* One true copy. Every reference points to it. When you need the word, you don't create a new copy. You find the canonical one and point to it.

**LUCIA:** That is how language has always worked. The dictionary holds the word. The speaker *uses* the word. The word itself does not multiply when spoken.

This beat establishes the conceptual framework BEFORE the code. The linguists see it first because language already solves this problem. The computer scientists are catching up to a principle that natural language discovered millennia ago.

### Beat 4: Carter's Insight — Pointer Equality

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

**CARTER:** If both citizens point to the *same* "Faber" — the one canonical copy — then I don't compare characters at all. I compare *addresses*. One comparison. One machine instruction. Equal means *identical* — the same object, not merely the same content.

**DATA:** *(from his workstation, precise)* This is a stronger form of equality. Content equality asks: "Do these two things have the same properties?" Identity equality asks: "Are these two things the same thing?" Content equality requires examination. Identity equality requires only recognition.

**PLINY:** *(slowly, his scholar's mind turning)* When a Roman citizen presents himself to the censor, the censor does not re-examine his name. He looks at the mark on the tablet and says: "This is the record I already have." He *recognizes* the citizen. He does not re-identify him.

**DATA:** Pointer comparison is recognition. String comparison is re-identification. The first is O(1). The second is O(n).

**CARTER:** One operation versus *n* operations. For the word "Forum Stellarum" — fourteen characters — that's fourteen operations reduced to one. For every comparison. Across the entire census.

Silence as the magnitude settles.

**MARCUS:** *(the engineer, always practical)* How do you ensure that the two citizens point to the same "Faber"?

**CARTER:** You build a table. A *single source of truth*. Every word in the census exists once in the table. When a new citizen record arrives, you don't store the word. You look it up. If it's already in the table, you point to the existing copy. If it's new, you add it. Then you point to it.

**MARCUS:** *(his eyebrows rising as he understands)* The table *is* the dictionary.

**CARTER:** The table is the dictionary. One entry per word. Every citizen, every record, every reference — they all point to the dictionary.

### Beat 5: ACT BREAK — The Name

Daniel is thinking about names.

**DANIEL:** We need a term. A process name. What do you call the act of replacing a private copy with a reference to the canonical form?

**LUCIA:** The act of *submitting* a word to the canon. Of surrendering your private copy and accepting the shared one.

**DANIEL:** *Internare?* To make something internal — to bring it inside, into the shared space?

**LUCIA:** *(considering)* *Internamentum.* The act of interning. The thing that holds the interned.

She writes it on the board:

***Internamentum: Locus ubi unum pro omnibus conservatur.***
*The place where one is kept for all.*

**PLINY:** *(reading it, approving)* One for all. *Unum pro omnibus.*

Carter looks at the phrase. Looks at the screen. Looks at the quilt of redundancy.

**CARTER:** *Unum pro omnibus.* One copy. For everyone.

Hard cut to the board. The title of the episode, written in Lucia's hand.

---

## ACT TWO: "Aedificare Super Fundamenta" (Building On Foundations)

*The implementation begins. The dependency chain moment. The Borg parallel emerges.*

### Beat 6: The Dependency Chain — The Moment

This is the structural landmark of the episode. For the first time, a new library is built *on top of* a previous episode's library. The audience should feel the accretion — the stacking of capabilities, each one enabling the next.

Carter opens a new file. On screen:

```c
#include "internamentum.h"
```

But the camera lingers on the includes within that header:

```c
#include "chorda.h"       /* Verba Nova (1x03) */
#include "piscina.h"      /* Capacitas Nova (1x01) */
#include "tabula_dispersa.h"  /* Tabularium Novum (1x05) */
```

**CARTER:** *(to the team, and to the audience)* We can't build this from nothing. String interning needs a hash table to find existing copies quickly. It needs an arena to allocate canonical copies permanently. It needs strings to be length-prefixed so we know their content without null terminators.

She writes the dependency on the board:

```
Piscina (1x01) — the arena where canonical copies live
  -> Chorda (1x03) — the string type that knows its length
    -> Tabula Dispersa (1x05) — the lookup table that finds existing copies
      -> Internamentum (1x06) — one copy, for all
```

**MARCUS:** *(studying the chain)* Each stone rests on the stone below it.

**CARTER:** Each library rests on the library before it. Remove any one of these and the one above it collapses.

**PLINY:** *(the naturalist, understanding this instantly)* A taxonomy. Each level of classification rests on the level below. You cannot name the species without first establishing the genus.

**MARCUS:** *(to Carter, with something like respect)* You planned this.

**CARTER:** *(honest)* Some of it. The census in 1x04 created the data. The hash table in 1x05 created the index. The index revealed the redundancy. The redundancy creates the need for interning. Each problem creates the next solution.

**DATA:** *(quiet)* That is also how ecosystems develop. Each organism creates the conditions for the next. The first plants fix nitrogen. The nitrogen enables more complex plants. The complex plants enable animals. No organism planned the ecosystem. Each one built what the previous one made possible.

A beat. The team looks at the dependency chain on the board. Four episodes. Four libraries. Each one enabling the next. The system is growing the way ecosystems grow — not by plan, but by possibility.

### Beat 7: The Core Implementation — On Screen

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
        /* Iam internata — redde canonicam */
        redde (chorda*)valor_existens;
    }

    /* Nova — creare copiam canonicam in piscina */
    canonica = piscina_allocare(
        intern->piscina, magnitudo(chorda));
    *canonica = chorda_transcribere(s, intern->piscina);

    /* Inserere in tabulam */
    tabula_dispersa_inserere(
        intern->tabula, *canonica, canonica);

    redde canonica;
}
```

**CARTER:** *(walking through it)* Step one: look up the string in the hash table. If it's already there, return the existing canonical pointer. Done. One probe.

**CARTER:** Step two: if it's NOT there — this is a new word we haven't seen before. Allocate space in the arena. Copy the string into the arena. This is the *canonical copy*. Then insert it into the hash table so the next person who asks for it will find it.

**CARTER:** Step three: return the canonical pointer. From now on, everyone who has this word has the *same* pointer. Not a copy. THE word.

**MARCUS:** *(testing his understanding)* The first citizen who says "Faber" creates the canonical copy. The second citizen who says "Faber" receives a pointer to the first citizen's word?

**CARTER:** Not the first citizen's word. The *canonical* word. It belongs to no citizen. It belongs to the dictionary. The first citizen triggered its creation, but once it's in the table, it's no one's private copy. It's the shared truth.

This is the philosophical crux. Marcus's question — "the first citizen's word?" — is exactly the wrong frame. The canonical copy is not anyone's property. It's the word itself, existing once, referenced by all. Carter's correction is the episode's thesis in miniature.

### Beat 8: The Borg Parallel — Methos Speaks

Methos has been listening from his usual position — leaning against a pillar, cup of wine, air of studied indifference. He sets down the cup.

**METHOS:** You know who else does this.

The room goes still. Not because Methos rarely speaks (he doesn't speak rarely — he speaks constantly, about nothing). Because of his tone. Flat. The tone he uses when something ancient wakes up.

**CARTER:** Who?

**METHOS:** The Borg.

Silence.

**METHOS:** *(pushing off the pillar, walking to the board)* The Borg assimilate. One drone, identical to every other drone. One consciousness, shared. One purpose, universal. *(pointing at Carter's diagram)* One copy, for all. *Unum pro omnibus.*

**O'NEILL:** *(dangerous quiet)* You're saying we're building Borg tech.

**METHOS:** I'm saying the *efficiency* is the same. One copy instead of many. Shared instead of duplicated. The Borg solve the same problem you're solving — redundancy. Waste. The cost of individuality. *(beat)* They solved it by destroying the individual.

Dead silence.

**CARTER:** *(carefully)* The difference is what gets shared.

**METHOS:** *(turning to her)* Is it? Explain the difference. Because from where I'm standing, "one canonical copy that everyone references" sounds an awful lot like "one consciousness that everyone shares."

This is the moment the episode earns its dramatic weight. The Borg comparison is not wrong. The efficiency IS the same. The mechanism IS the same. One copy, for all. The question is whether you can have the efficiency without the annihilation.

### Beat 9: Carter's Answer — The Difference

Carter thinks. She doesn't rush. She goes to the board.

**CARTER:** *(drawing)* When the Borg assimilate, they destroy the original. The individual ceases to exist. Their memories, their personality, their will — overwritten. What's left is a copy of the collective.

She draws a second diagram.

**CARTER:** When we intern, we preserve the content. The canonical copy is the *word* — not the *person*. "Faber" is interned. The craftsman is not. *(beat)* Two hundred citizens are craftsmen. They all use the same word. They remain two hundred different people.

**METHOS:** *(pressing)* And if the "word" you're interning is a person's identity? Their classification? Their role?

**CARTER:** Then you've crossed the line. Interning a word is sharing vocabulary. Interning a person is assimilation.

**DATA:** *(standing, which he rarely does for dialogue)* The distinction is precise. Interning operates on the *name*, not the *named*. The canonical copy preserves the content of the string — every character, in order, exactly as it was. Nothing is lost. The string's *nature* is preserved. What is eliminated is only the *redundant copy* — the unnecessary duplicate. The Borg eliminate the *nature*. They do not preserve the content. They replace it.

**METHOS:** *(after a long moment, nodding slowly)* The Borg share identity. You share vocabulary. *(beat)* That's a thinner line than you think.

He picks up his wine and sits back down. The warning is planted. The line between sharing vocabulary and sharing identity is the line between interning and assimilating. The show will walk that line for five seasons.

### Beat 10: ACT BREAK — The Global Singleton

Carter makes a design decision. On screen:

```c
/* Internamentum globale — unum pro omnibus */
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

**CARTER:** One table. For the entire system. Every module, every library, every component that interns a string — they all use the same table.

**MARCUS:** *(immediately)* A shared resource. Like a public well. Every household draws from it. No household owns it.

**O'NEILL:** *(from the back, quiet)* One table. One point of failure.

**CARTER:** *(acknowledging)* Yes.

**O'NEILL:** If it breaks, everything that depends on it breaks.

**CARTER:** Yes.

**O'NEILL:** *(to the room)* That's a lot of trust in one table.

He walks out. Carter watches him go. O'Neill, as usual, has identified the strategic risk in three sentences. The global singleton is powerful and fragile — exactly like a shared language, a shared currency, a shared faith. One failure point for all users. *Unum pro omnibus* cuts both ways.

---

## ACT THREE: "Probatio" (The Test)

*Building the interned census. The pointer comparison moment. Teal'c tests.*

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

On screen: the progress counter. Three thousand citizens. Each citizen's occupation, district, and street interned. The memory visualization from the cold open — the quilt of redundancy — transforms in real time. Red patches merge. Blue patches merge. Green patches merge. The quilt shrinks, consolidates, becomes simpler. Two hundred and three copies of "Faber" become one. Four hundred and twelve copies of "Forum Stellarum" become one. The memory footprint collapses.

```
Memoria ante internamentum: 847,293 bytes
Memoria post internamentum: 24,891 bytes
Reductio: 97.1%
```

**PLINY:** *(watching the numbers)* Ninety-seven percent of the memory... was saying the same thing over and over?

**CARTER:** Ninety-seven percent of the memory was carrying private copies of shared truth.

### Beat 12: The Pointer Comparison Moment

Carter demonstrates the payoff. Two citizens from the census:

```c
Civis* primus = /* ... Gaius, Faber, Regio Prima */
Civis* secundus = /* ... Lucius, Faber, Regio Prima */

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

**DATA:** Thirty-six times faster. Not because the comparison is faster. Because the comparison is *unnecessary*. When two references point to the same canonical copy, the question "are these equal?" has already been answered. They are not merely equal. They are identical. The same object.

**PLINY:** *(understanding something profound)* It is the difference between two copies of the same scroll and two hands holding the same scroll. The copies must be compared. The scroll speaks for itself.

This is Pliny's contribution to the episode's philosophy. "Two hands holding the same scroll." Not identical twins — the *same person*. Not equivalent content — the *same object*. Interning doesn't make things equal. It reveals that they were always the same thing, wrongly duplicated.

### Beat 13: Teal'c's Test Suite

Teal'c has been building tests since Beat 6.

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

**TEAL'C:** *(to the room)* All tests pass.

But he's not done. He writes one more test:

```
probatio_internare_post_mutationem .... FALSITAS
```

The room goes quiet.

**TEAL'C:** I interned a string. Then I modified the original. Then I interned the modified version. The table returned the OLD canonical copy.

**CARTER:** *(frowning)* That's... correct behavior. The table doesn't know the original was modified. It compared the new string against its contents and —

**TEAL'C:** I know it is correct behavior. That is not the concern. *(beat)* The concern is that someone will intern a string, modify their local copy, and expect the canonical version to reflect the change. It will not. The canonical copy is *immutable*.

**CARTER:** The canonical copy HAS to be immutable. If one user could change the shared copy, everyone who references it would see the change. That's —

**DATA:** Assimilation. If one voice changes the canonical word, every reference to that word is changed without consent.

The room is very still.

**TEAL'C:** The canonical copy is a *treaty*. Once interned, it belongs to no one. It cannot be unilaterally revised. *(beat)* This is correct. But it must be documented and tested, because the temptation to modify a shared truth is universal.

This is the episode's deepest test — not a technical failure but a philosophical boundary. Immutability is the price of sharing. The canonical copy is incorruptible *because* no one can change it. The moment you allow modification, you have the Borg: one consciousness, mutated from the top, all drones receiving the mutation without consent.

### Beat 14: ACT BREAK — The Vocabulary of a Civilization

Carter runs the final statistics:

```
Internamentum Globale:
  Chordae unicae: 263 (census) + 1,203 (encyclopaedia) = 1,466
  Chordae totales referentes: 16,064
  Ratio: 1 canonica pro 10.96 referentibus
```

One canonical copy for every eleven references. One word, shared eleven times. One truth, held in common by eleven hands.

**CARTER:** Fourteen hundred and sixty-six words. That's the vocabulary of Nova Roma. Everything the census knows, everything the encyclopedia records — all of it reduces to fourteen hundred unique words.

**PLINY:** *(standing at the screen, his hand on the desk)* The vocabulary of a civilization. *(beat)* I spent eighty years writing. All of it — every observation, every classification, every measurement — comes down to fourteen hundred words arranged in different orders.

**DANIEL:** *(gently)* That's what language is, Pliny. A finite vocabulary arranged in infinite combinations. You didn't write fourteen hundred words. You wrote forty volumes. The words are the atoms. The volumes are the world.

**PLINY:** *(a long look at the screen, then at Daniel)* Then the Internamentum is the periodic table. Every element, listed once. Every compound, assembled from the list.

---

## ACT FOUR: "Methos Meminit" (Methos Remembers)

*Methos's Alexandria revelation. The library that burned. The Borg parallel deepens.*

### Beat 15: Methos and Daniel — The Library

Evening. Daniel is working alone in Pliny's archive, cross-referencing terms between the census and the encyclopedia. Methos enters. Not his usual casual drift — something has been working on him since Beat 8.

**METHOS:** Your interning. The one canonical copy. I need to tell you something about that.

Daniel stops. Methos never *needs* to tell anyone anything. He wants to, or he doesn't. "Need" is new.

**METHOS:** Alexandria. I was there.

**DANIEL:** *(carefully)* The Library of Alexandria? You were there when —

**METHOS:** When it was still standing. *(beat)* Everyone talks about the burning. That's the story that survived — the great library, the great fire, the great loss. But before the fire, before any of that... the Library worked.

He sits down. Pours wine.

**METHOS:** Every scroll that entered Alexandria was copied. The original was kept. The copy was given back to the owner. *(looking at Daniel)* Sound familiar?

**DANIEL:** *(realizing)* They interned the scrolls.

**METHOS:** They interned the scrolls. One canonical copy in the Library. The original returned. Every scholar in the Mediterranean knew: if you wanted the true text — the unaltered, verified, canonical text — you went to Alexandria. Not because they had the only copy. Because they had the *right* copy.

He drinks.

**METHOS:** The scholars didn't come to Alexandria because it had the most scrolls. They came because the scrolls were *authoritative*. Verified. Cross-referenced. Canonical. *(beat)* The Library of Alexandria was the first interning table. And it burned.

**DANIEL:** *(pressing, because Daniel always presses)* You said you were *there*. Not just "it existed." You were there.

**METHOS:** *(the deflection failing)* I... had friends there. Among the scholars. Among the —

He stops. His expression shifts. Something older than the Library surfaces.

**METHOS:** There were cults in Alexandria that the scholars didn't document. Older than the Library. Older than Egypt. A priestess in a sub-chamber. Ecstatic rites. Things they kept below the main collection, in rooms the public didn't see.

**DANIEL:** *(the archaeologist, suddenly very alert)* What kind of things?

**METHOS:** Artifacts. Stones. *(catching himself)* This isn't relevant to your string library.

**DANIEL:** *(quiet)* You brought it up.

**METHOS:** *(standing, the shutters coming down)* I brought up Alexandria because your interning is the same principle. Canonical copies. Shared truth. One source. *(heading for the door)* The Library burned because it was a single point of failure. Your global singleton is the same. Think about that.

He leaves. Daniel stares at the doorway. Methos voluntarily told him about Alexandria — something he never does — and then deflected from what was clearly the real revelation. The cults. The sub-chamber. The stones.

Daniel writes in his notebook. Not the interning comparison. The other thing. The stones.

**DIRECTION NOTE:** This scene plants the Cybele/Black Stone thread for 1x13 and 1x17. Methos's deflection must be visible — the audience sees him catch himself. The Alexandria/interning parallel is real and earned. The deeper revelation is what escapes.

### Beat 16: Data and Midas — Friendship Deepens

Simultaneous with Beat 15. A different room. Data and Midas are cataloging botanical specimens that Pliny has collected — the beginning of a biological database.

Midas holds up a pressed flower.

**MIDAS:** What do I call this?

**DATA:** The classification in Pliny's system is *Rosa canina*.

**MIDAS:** And if there are forty specimens of *Rosa canina*?

**DATA:** Then we intern the name. One canonical entry. Forty references.

**MIDAS:** *(turning the flower in his hand)* One name. Forty flowers. But each flower is different. This one has five petals. This one has six. This one was picked in spring. This one in autumn.

**DATA:** *(precise)* The name is shared. The specimens are not. Interning does not claim the flowers are identical. It claims their *name* is the same.

**MIDAS:** *(smiling, his peculiar Phrygian warmth)* A good system. Names should be shared. People should not.

Data pauses. Not a processing pause — a *thinking* pause. The distinction Midas just drew — names shared, people not — maps exactly to the Borg/interning parallel from Beat 8. But Midas didn't hear that conversation. He arrived at the same conclusion independently, through flowers.

**DATA:** Midas. You are philosophically consistent in a way I find... unusual among biological organisms.

**MIDAS:** *(laughing)* Is that a compliment?

**DATA:** It is an observation. I value observations.

**MIDAS:** *(setting down the flower, looking at Data directly)* You are the most honest person I have ever met. And I have met people for three thousand years.

**DATA:** I am an android. I lack the capacity for dishonesty.

**MIDAS:** *(shaking his head)* That is not the same thing.

A beat. Two ancient beings — one made of circuits, one displaced in time — cataloging flowers. The friendship deepening in the quiet space between classification and care.

---

## ACT FIVE: "Unum Pro Omnibus"

*The system launches. The implications land. The title earns itself.*

### Beat 17: The Interned Census Goes Live

The Tabularium. The full team. Carter runs the interned census for the senate demonstration.

A citizen approaches: "I need the occupation records for all craftsmen in Regio Tertia."

Carter writes the query. Because all "Faber" strings now point to the same canonical copy, and all "Regio Tertia" strings point to the same canonical copy, the query doesn't compare characters. It compares pointers.

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

**CARTER:** Two pointer comparisons per citizen. Not two string comparisons — two *pointer* comparisons. Across three thousand citizens: six thousand single-instruction operations. Done in microseconds.

The results appear instantly. Every craftsman in District Three.

**MARCUS:** *(who has been running census queries all week)* That query took four minutes yesterday.

**CARTER:** Yesterday, every comparison was character-by-character. Today, every comparison is a single check: same pointer, or not.

**MARCUS:** Four minutes to microseconds. *(a long beat)* Every Roman magistrate who has ever waited for a census query will understand this improvement.

### Beat 18: Pliny's Canonical Encyclopedia

Pliny has been quiet since Beat 14. He approaches the screen.

**PLINY:** My encyclopedia. Intern it.

Carter runs the transformation on Pliny's twelve hundred entries. The memory visualization shrinks. Cross-references collapse. Every time Pliny wrote "cinnabar" in a different volume, the word now points to the same canonical copy.

**PLINY:** *(watching the cross-references form)* Every volume that mentions cinnabar... is now connected. Not by my index. By the word itself.

**CARTER:** When the same canonical string appears in multiple entries, those entries share a reference. The connection is structural, not annotated. The encyclopedia *knows* which entries share terminology because they share pointers.

**PLINY:** *(his eyes bright)* I spent forty years building cross-references by hand. Noting that Volume 7 mentions the same mineral as Volume 23. Writing the connections in ink. *(beat)* The Internamentum creates the connections by nature. The shared word IS the cross-reference.

This is the unexpected payoff. Interning isn't just about saving memory. It creates a *semantic web* — entries that share canonical strings are structurally linked. Pliny's lifelong dream of a fully cross-referenced encyclopedia happens *automatically* as a side effect of interning.

### Beat 19: The Title Moment

The Tabularium. Late day. The system running. Carter, Marcus, Pliny, Daniel, Lucia gathered around the board.

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

**CARTER:** Four libraries. Six episodes. Each one built on the last.

**MARCUS:** *(studying the dependency chain)* An aqueduct is useless without a reservoir. A reservoir is useless without pipes. Pipes are useless without a source. You have built a source, a reservoir, pipes, and a delivery system. Each one necessary. Each one insufficient alone.

**PLINY:** *(writing the dedication for the Internamentum, his steady hand)* *Unum pro omnibus, non omnes pro uno.* One for all — not all for one. The word serves the people. The people do not serve the word.

This is the episode's final thesis statement. The Borg version of "unum pro omnibus" is "all serve the one" — every drone subsumed into the collective. The Nova Roma version is the reverse: one canonical truth serves all users, but the users remain sovereign. The word is shared. The person is not.

---

## TAG

### Beat 20: Carter and Marcus — The Evening Walk

Their ritual. The road out of Nova Roma. Stars overhead.

**MARCUS:** The dependency chain. Four libraries deep. Five episodes to build.

**CARTER:** It'll be deeper by the end of the season.

**MARCUS:** *(walking in silence for a moment)* In Rome, the greatest buildings are the ones where you cannot remove a single stone without the structure failing. Every stone bears weight. Every stone is necessary.

**CARTER:** That's... actually a good design principle.

**MARCUS:** It is the *only* design principle. *(beat)* Your four libraries. Can any be removed?

**CARTER:** No. Remove the arena, everything collapses. Remove strings, the hash table has nothing to hash. Remove the hash table, the interning table has no way to find existing copies.

**MARCUS:** Then you have built something Roman. *(beat)* I do not say that lightly.

They walk. The stars are bright. Two builders recognizing the same principle across two thousand years.

**CARTER:** Next, we need to save things to disk. Everything we've built lives in memory. If the machine stops, it's gone.

**MARCUS:** *(the Roman, who understands stone vs. wax)* Then we need to carve what we've written in wax into stone.

**CARTER:** Exactly. *(beat)* File I/O.

**MARCUS:** Is that 1x07?

**CARTER:** *(smiling)* That's 1x07.

FADE TO BLACK.

---

## STINGER

### Beat 21: Kirk's Canonical Network

**Location:** Earth. Night. A corporate office that shouldn't be occupied at this hour.

Kirk is on a video call. Six faces on screen — but the camera shows us something the faces don't know we're seeing. Below each face, a data field. Not their name. Their *designation*.

```
NORTHWEST-INFRA-7
NORTHWEST-SIGNAL-3
PACIFIC-RECRUIT-12
MOUNTAIN-MATERIAL-2
COASTAL-SIGNAL-9
CENTRAL-INFRA-15
```

Six people. Six designations. When Kirk addresses them, he doesn't use their names. He uses their functions.

**KIRK:** Northwest Infrastructure, status.

**NORTHWEST-INFRA-7:** Three facilities secured. Two pending.

**KIRK:** Pacific Recruitment, status.

**PACIFIC-RECRUIT-12:** Nineteen new contacts. Seven viable.

The call continues. Efficient. Fast. Each person is their function. Interchangeable within their category. Kirk doesn't need to know their names. He needs to know their *slot*.

After the call ends, Kirk turns to a second screen. On it: a database. The same designations. But with a modification: duplicate entries for the same function are being consolidated. Two "Northwest Infrastructure" operatives who do the same thing — Kirk is merging their assignments. Reducing redundancy.

He's interning his network.

But where Carter's interning preserves the word while letting the people remain distinct, Kirk's interning preserves the *function* while letting the people become interchangeable. Carter interns vocabulary. Kirk interns identity.

Kirk types a note in the database:

```
OPTIMIZATION: Where operatives share function class,
consolidate to single point of contact per region.
One representative per function. Canonical.
```

He uses the word *canonical*. The same word the team used all episode. The audience hears the echo and feels the chill.

**KIRK:** *(to himself, closing the laptop)* Unum pro omnibus.

Same phrase. Opposite meaning. One copy for all — Kirk means one *person* for all, the rest redundant. The Borg version of interning. The title of the episode, spoken by the villain, in the final seconds.

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
/* Pigra initialisatio — creatur in primo usu */
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

## STRUCTURAL NOTES

### The Episode's Architecture

The episode has a **mirror structure** around the Borg parallel:

1. **Acts 1-2:** The problem (waste) and the solution (interning) are presented as purely beneficial.
2. **Beat 8 (The Mirror):** Methos names the Borg parallel. The solution becomes ambiguous.
3. **Acts 3-5:** The team works through the ambiguity and arrives at the distinction — interning vocabulary vs. interning identity.
4. **Stinger:** Kirk demonstrates the Borg version. The mirror is complete.

### The Dependency Chain as Structural Theme

This is the first episode where the *process of building on previous work* is itself thematic. The dependency chain isn't just technical — it's the show's argument that complex capability emerges from simple foundations stacked carefully. Piscina enabled Chorda enabled Tabula Dispersa enabled Internamentum. Each episode's A-plot made the next one possible.

For the audience, this creates a sense of *accumulation* — the show is building something, and each episode adds a visible layer. By 1x06, the audience can see four layers of the stack. By 1x22, they'll see the entire foundation.

### Carter as the Driver

Carter drives this episode more than any since 1x01. She identifies the waste, proposes the solution, implements it, and defends it against Methos's philosophical challenge. This is her episode to own — not the emotional center (that's Methos/Data/Pliny), but the *intellectual* center. She sees the redundancy. She sees the solution. She sees the Borg distinction. She's the builder.

### Pliny's Role: The Living Cross-Reference

Pliny's arc this episode is subtler than 1x05. There, he was the emotional center — his encyclopedia, his Vesuvius, his mortality. Here, he's the *user* — the man who has been cross-referencing for forty years and discovers the machine does it automatically. His reaction (Beat 18) is not wonder but *relief*. The cross-references exist. They were always there. The interning just made them visible.

### The Title's Double Meaning

"Unum Pro Omnibus" means "One For All." But it carries two readings:
1. **The Nova Roma reading:** One canonical copy serves all users. The word is shared. The people are sovereign.
2. **The Kirk reading:** One function-bearer serves all needs. The person is interchangeable. The function is sovereign.

The stinger flips the title. The audience leaves the episode with both readings in their head, unable to separate them. That's the design.
