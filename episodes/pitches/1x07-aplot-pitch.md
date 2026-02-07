# 1x07 "Tabulae Ceratae" (Wax Tablets) — A-Plot Technical Pitch
## The Dramatic Engine, Act Structure, and On-Screen Code

---

## THE DRAMATIC ENGINE

### The Problem

Everything the team has built across six episodes lives in volatile memory. The arena allocator, the string library, the hash table, the interning system, Pliny's cross-referenced encyclopedia, the census with its 1,466 canonical words — all of it exists as electrical charge in silicon. If the power fluctuates, if the machine overheats, if a storm rolls through the hills above Nova Roma — gone. Not damaged. Not corrupted. *Gone*. As if it never existed.

This has been seeded across the last two episodes with increasing urgency:

- **1x05 Tag:** Methos — "Each one burned. When the fire comes — and the fire always comes — what survives?"
- **1x06 Beat 10:** O'Neill — "One table. One point of failure."
- **1x06 Tag:** Carter — "Everything we've built lives in memory. If the machine stops, it's gone. Next, we need to save things to disk."
- **1x06 Tag:** Marcus — "Then we need to carve what we've written in wax into stone."

The dramatic engine is not the technical challenge — file I/O is conceptually simple. The dramatic engine is the *realization* of what volatility means. Six episodes of building. Six episodes of accumulation — the dependency chain, the canonical vocabulary, the cross-references, the census. The audience has watched this team construct something remarkable from nothing. Now they learn it's all written in sand.

### Why This Matters Dramatically

The show's first six episodes have been about *building*. This episode is about the discovery that building is not enough — you must also *preserve*. The Romans understood this distinction viscerally: wax tablets (*tabulae ceratae*) were for notes, drafts, temporary records — the wax could be smoothed and rewritten. Stone inscriptions (*inscriptiones lapideae*) were for laws, treaties, dedications — permanent, costly, deliberate. Every Roman magistrate knew the difference between what you wrote in wax and what you carved in stone.

The filum library bridges both modes: `FILUM_MODUS_CREARE` overwrites, like smoothing wax. `FILUM_MODUS_APPENDERE` adds to what exists, like carving another line below the last. The episode dramatizes the discovery that memory is wax and disk is stone — and that a civilization requires both.

---

## COLD OPEN

**Location:** The Tabularium. Night. Storm outside — rain hammering the stone. The team is asleep. Only the machines are running.

The camera moves through the workshop. On screen: the interning table. The census. Pliny's encyclopedia. The 1,466 canonical words glowing in the dim light. The dependency chain, seven episodes deep, all of it alive in memory.

A lightning flash. The lights flicker.

The screen goes black for two heartbeats.

Then it comes back. The systems restart. The login prompt appears. The machines are fine — the power interruption was momentary, a fluctuation, not a failure.

But the interning table is gone. The census is gone. Pliny's encyclopedia — the cross-references, the canonical vocabulary, the forty years of observation indexed and interned — is gone. The screen shows what it showed on the first day they arrived: nothing.

Carter, in her quarters, hears the silence. Not the silence of night — the silence of machines that have stopped humming and restarted. She's dressed and running before she's fully awake.

She reaches the workshop. Sits down. Stares at the empty screen. Types:

```c
/* Quaerere internamentum globale */
InternamentumChorda* intern = internamentum_globale();
i32 numerus = internamentum_numerus(intern);
/* numerus = 0 */
```

Zero. The internamentum is empty. Not corrupted — *empty*. The global singleton has been re-initialized from nothing. Lazy initialization means it creates itself on first access — but it creates itself *empty*. The 1,466 words, the 16,064 references, the 97.1% memory savings — none of it persisted. None of it *could* persist. There was nowhere for it to go.

Carter stares at the zero.

**CARTER:** *(to no one)* It was all in memory.

She opens her notebook. The page from 1x06 where she wrote "CANONICA." Below it she writes two words:

**TABULAE CERATAE**

SMASH TO MAIN TITLES.

---

## ACT ONE: "Cera et Lapis" (Wax and Stone)

*The loss is understood. The Roman analogy is born. The solution is named.*

### Beat 1: The Morning After

**Location:** The Tabularium. Dawn. The team assembles. Carter has the empty screen behind her. She's been up since the fluctuation.

**CARTER:** We had a power fluctuation last night. The machines restarted.

**MARCUS:** And?

**CARTER:** Everything is gone.

Silence. Then Pliny:

**PLINY:** *(very carefully)* Define "everything."

**CARTER:** The census. The encyclopedia. The interning table. The canonical vocabulary. All fourteen hundred and sixty-six words. All sixteen thousand references. The cross-references. The queries. Everything that was in memory when the power dropped is gone.

**PLINY:** *(sitting down, the color leaving his face)* My encyclopedia. The cross-references that formed by nature — the shared words, the structural links —

**CARTER:** Gone. We can rebuild it. We still have the source scrolls — your physical manuscripts, the census tablets. The data isn't lost. The *organization* is lost. We have to re-enter everything. Re-intern every string. Re-build every reference.

**MARCUS:** *(not angry — pragmatic, an engineer)* How long?

**CARTER:** Days. Maybe a week, with the whole team.

**O'NEILL:** *(from the doorway, arms crossed)* And the next time the power drops?

**CARTER:** Same thing. Start over.

**O'NEILL:** So we do a week of work that can be destroyed in a tenth of a second by a thunderstorm.

**CARTER:** *(meeting his eyes)* Yes.

**O'NEILL:** Fix that.

He leaves. Efficient as always.

### Beat 2: Marcus Names It — Wax and Stone

Marcus has been thinking. He approaches the board.

**MARCUS:** In Rome, we have two materials for writing. *(he draws on the board)* Cera — wax. And lapis — stone.

He draws a wax tablet:

**MARCUS:** A tabula cerata — a wax tablet. You write in it with a stylus. When you're finished, you smooth the wax and write again. It is for temporary things: notes, calculations, drafts, letters. The writing is real while it exists. But it is understood to be *temporary*. The wax exists to be overwritten.

He draws a stone inscription:

**MARCUS:** An inscriptio lapidea — a stone inscription. You carve it with a chisel. It takes time. It takes skill. It takes intention. And once it is carved, it endures. Laws. Treaties. Dedications to the gods. The names of the dead. *(beat)* These are carved in stone because they must survive the person who carved them.

**MARCUS:** *(turning to the blank screen)* Your machine has been a wax tablet. Everything you wrote on it was real — but it was written in wax. The storm smoothed the wax. Now you need to learn to carve in stone.

**CARTER:** *(the connection clicking)* Memory is wax. Disk is stone.

**MARCUS:** *(nodding)* And every civilization needs both. You draft in wax. You preserve in stone. The error was not in using wax — wax is useful, wax is fast, wax lets you think quickly. The error was in having *only* wax. In never carving what mattered into something that would survive the next storm.

This is the episode's thesis in Marcus's voice: *the error was not in using wax but in having only wax.* Memory is not the enemy. Volatility is not a flaw. The flaw is the absence of permanence — the failure to distinguish what should endure from what can be smoothed away.

### Beat 3: Carter's Blueprint — The Filum Library

Carter is at the board. She writes:

```
FILUM — "Thread" / "File"

Legere  — Read   (lapis → cera: stone to wax, disk to memory)
Scribere — Write  (cera → lapis: wax to stone, memory to disk)
Appendere — Append (add more carving to existing stone)
```

**CARTER:** Three operations. Read: take what's on disk and bring it into memory. Write: take what's in memory and put it on disk. Append: add to what's already on disk without destroying what was there before.

**DANIEL:** *(from the side, the linguist)* *Filum* — thread. As in the thread of a narrative? The Ariadne thread?

**CARTER:** As in a file. A sequence of data stored on disk.

**DANIEL:** But the Latin works both ways. A filum is a thread — a continuous strand. A file on disk is a continuous sequence of bytes. The metaphor is precise.

**LUCIA:** *(adding)* And a filum is what connects. Ariadne's thread connects the present to the path already walked. A file connects the present state to the recorded past.

**CARTER:** *(writing it)* Read, write, append. Stone to wax. Wax to stone. More carving on existing stone.

**MARCUS:** *(studying the board)* You said "append." Not overwrite. You add to what exists.

**CARTER:** There are two modes. *(she writes)*

```
FILUM_MODUS_CREARE   — Create / Overwrite
  (Smooth the wax. Start fresh.)

FILUM_MODUS_APPENDERE — Append
  (Carve below the last line.)
```

**MARCUS:** *(immediately)* The first is for drafts. The second is for records.

**CARTER:** Exactly. Overwrite when you're revising. Append when you're documenting.

**MARCUS:** *(a slow nod)* The census is appended. Each citizen added below the last. You do not smooth the wax and rewrite from the beginning each time a child is born.

**PLINY:** *(from his chair, very quiet)* My encyclopedia. Forty volumes. Each volume appended to the last. I have never smoothed the wax on a completed volume. I have only added new pages.

The Roman analogy maps perfectly because the Romans *already solved this problem* in their material culture. Wax tablets were temporary storage. Stone inscriptions were persistent storage. The census was an append-only document. Pliny's encyclopedia was an append-only document. The filum library isn't teaching the Romans anything new — it's giving them digital tools for something they already understood.

### Beat 4: ACT BREAK — The Dependency Chain Grows

Carter writes the new dependency chain on the board:

```
Piscina (1x01) — arena allocation
  -> Chorda (1x03) — strings
    -> Chorda Aedificator (1x03) — string builder
      -> Tabula Dispersa (1x05) — hash table
        -> Internamentum (1x06) — canonical copies
          -> Filum (1x07) — persistence
```

**CARTER:** Filum needs Piscina to allocate read buffers. It needs Chorda to represent file contents. It needs Chorda Aedificator for the streaming reader — when a line is longer than the buffer, the builder assembles the fragments.

**MARCUS:** *(counting on the board)* Six libraries deep. Seven episodes.

**CARTER:** And for the first time, the dependency chain has a *practical* output. Everything before this was in-memory infrastructure. Filum is the first library that touches the world outside the machine.

**DATA:** *(precise)* The previous libraries transformed data within the system. This library transfers data between the system and its environment. It is the boundary between computation and persistence. Between thought and record.

**MARCUS:** Between wax and stone.

---

## ACT TWO: "Scribere" (To Write)

*The first write. The sync moment. The overwrite/append distinction deepens.*

### Beat 5: The First Write

Carter codes. The audience sees the simplest possible operation — taking a string and saving it to disk:

```c
/* Prima scriptio — salvare chordam ad discum */
chorda salutatio = chorda_ex_literis("Salve, Nova Roma!", piscina);

b32 successus = filum_scribere("salutatio.txt", salutatio);

si (successus)
{
    imprimere("Scriptum in discum: %d bytes\n",
              salutatio.mensura);
}
```

Carter types the command. The disk light blinks. One blink.

**CARTER:** Seventeen bytes. Written to disk. If we lose power right now, that file survives.

She unplugs the machine. Plugs it back in. The screen goes dark. Reboots. She navigates to the file:

```c
/* Legere post defectum — reading after failure */
chorda lecta = filum_legere_totum("salutatio.txt", piscina);
imprimere("Lectum: '%.*s'\n", lecta.mensura, lecta.datum);
/* Output: "Lectum: 'Salve, Nova Roma!'" */
```

**CARTER:** There it is. Seventeen bytes. Survived the reboot. Survived the loss of power. The wax was smoothed, but the stone remains.

**PLINY:** *(leaning forward)* Do that with my encyclopedia.

**CARTER:** *(smiling)* We're getting there.

### Beat 6: The Streaming Writer — Carving Line by Line

Carter introduces the streaming writer. This is the tool for large operations — when you're not writing one string but building a document line by line.

```c
/* Creare scriptorem — open for writing */
FilumScriptor* scriptor = filum_scriptor_aperire(
    "census_novus.txt",
    FILUM_MODUS_CREARE,
    piscina);

/* Scribere lineas — write lines */
filum_scriptor_lineam_scribere(scriptor,
    "Gaius Tertius, Faber, Regio Prima");
filum_scriptor_lineam_scribere(scriptor,
    "Lucia Serena, Grammatica, Regio Secunda");
filum_scriptor_lineam_scribere(scriptor,
    "Marcus Vitruvius, Architectus, Regio Prima");
```

**CARTER:** Three lines. Three citizens. Each one carved into the file. But here's the thing —

She pauses at the keyboard.

**CARTER:** These lines are not on disk yet.

**MARCUS:** *(confused)* You just wrote them.

**CARTER:** I wrote them to a *buffer*. A holding area. The machine collects the writes and sends them to disk in batches — it's more efficient. But it means there's a gap between "I said write" and "it's actually on the disk."

**MARCUS:** *(his engineer's instinct firing)* A gap during which the power could drop.

**CARTER:** Exactly.

### Beat 7: The Sync Moment — Carving in Stone

This is the dramatic turning point of the technical A-plot. The concept of `sync` — flushing the buffer to disk — is the bridge between wax and stone.

**CARTER:** Watch. *(she types)*

```c
/* Sync — "carve it now" */
filum_scriptor_sync(scriptor);
```

**CARTER:** *Sync*. This is the command that says: "Stop holding. Write it now. Push everything in the buffer to the physical disk. Don't return until it's done."

**MARCUS:** *(understanding)* The chisel strikes the stone.

**CARTER:** Exactly. Before sync, the data is in the buffer — wax. After sync, the data is on disk — stone. The sync is the moment of commitment.

**PLINY:** *(thoughtfully)* In my practice, there is the same moment. The draft is in ink on papyrus — changeable, scratchable, temporary. When I hand the scroll to the copyist and say "this is the final text" — that is the sync. The words pass from my custody to the archive's. They become the record.

**CARTER:** *(writing on the board)* Buffer is wax. Disk is stone. Sync is the chisel.

**MARCUS:** *(standing, walking to the board)* And the discipline is knowing *when* to sync. *(beat)* If you sync after every word, you spend all your time carving and none thinking. If you never sync, everything stays in wax and the first wind destroys it. The Roman builder syncs at the completion of each course of stone — not each individual block, but each completed layer.

This is Marcus's engineering insight, and it's correct. The practical challenge of sync is not how to do it but *when* to do it. Too often, and you lose performance. Too rarely, and you risk data loss. The Roman builder knows: you commit at meaningful boundaries.

### Beat 8: ACT BREAK — Overwrite vs. Append

Carter demonstrates the two modes. On the board:

```c
/* MODUS CREARE — smooth the wax, start fresh */
FilumScriptor* scriptor_novus = filum_scriptor_aperire(
    "census.txt", FILUM_MODUS_CREARE, piscina);
/* Previous contents destroyed. File is empty. */

/* MODUS APPENDERE — carve below the last line */
FilumScriptor* scriptor_addens = filum_scriptor_aperire(
    "census.txt", FILUM_MODUS_APPENDERE, piscina);
/* Previous contents preserved. New data added after. */
```

**CARTER:** Two modes. Create overwrites — it smooths the wax and starts from nothing. Append adds to the end — it preserves everything that was there and adds more.

**METHOS:** *(from his usual pillar, wine in hand)* The Borg overwrite. They don't append to a civilization. They replace it.

The room goes still. After last episode's Borg discussion, Methos invoking them again has weight.

**METHOS:** Every culture the Borg assimilate — the history, the art, the language, the identity — *overwritten*. FILUM_MODUS_CREARE. Previous contents destroyed. *(beat)* Civilizations append. Each generation adds a line below the last. No generation smooths the wax on the generations before it.

**CARTER:** *(quiet)* That's... a good way to think about it.

**PLINY:** *(who has been writing since the conversation started)* The Naturalis Historia is thirty-seven volumes. I have never rewritten Volume One. I have only written Volume Thirty-Eight, and Thirty-Nine, and Forty. Each volume is an act of appending. *(beat)* An encyclopedia that overwrites its own entries is not an encyclopedia. It is propaganda.

**DATA:** *(precise)* An append-only record is a history. An overwrite record is a state. History preserves the past. State replaces it. The distinction between FILUM_MODUS_APPENDERE and FILUM_MODUS_CREARE is the distinction between a civilization that remembers and one that forgets.

Marcus writes on the board in Latin:

***Civilitas quae appendit, meminit. Civilitas quae superscribit, obliviscitur.***
*A civilization that appends, remembers. A civilization that overwrites, forgets.*

---

## ACT THREE: "Legere" (To Read)

*Reading files. The streaming reader. The line-by-line encounter with the past.*

### Beat 9: The First Read

Carter demonstrates reading. She's saved a test file — now she reads it back:

```c
/* Legere filum totum — read entire file */
chorda contentum = filum_legere_totum(
    "census.txt", piscina);

imprimere("Legere %d bytes ex disco\n",
          contentum.mensura);
```

**CARTER:** Read. The stone speaks. What was carved yesterday is read today. What was synced to disk survives the death of the machine that wrote it.

**PLINY:** *(a question that reveals the scholar)* When I read a scroll, I hold it. I can feel the age of the papyrus. I can see where the ink has faded, where a previous scribe corrected a word. *(beat)* When the machine reads from disk, does it know anything about the file? Its age? Its author? Its history?

**CARTER:** *(typing)* Actually — yes.

```c
/* Interrogare statum fili */
FilumStatus status;
filum_status("census.txt", &status);

imprimere("Mensura: %zu bytes\n", status.mensura);
imprimere("Est filum: %s\n",
          status.est_filum ? "VERUM" : "FALSUM");
imprimere("Legibile: %s\n",
          status.potest_legere ? "VERUM" : "FALSUM");
imprimere("Scribabile: %s\n",
          status.potest_scribere ? "VERUM" : "FALSUM");
```

**CARTER:** Size. Type. Permissions — can it be read, can it be written. The file carries metadata. Not as rich as your papyrus — no handwriting, no ink quality — but the operating system records what it knows.

**PLINY:** *(immediately)* Size, type, permissions. *(he writes in his notebook)* A catalog entry. Every file is cataloged.

### Beat 10: The Streaming Reader — Reading a Chronicle

For large files, you don't read everything at once. You read line by line — the way a scholar reads a scroll, one passage at a time.

```c
/* Aperire lectorem — open for reading */
FilumLector* lector = filum_lector_aperire(
    "encyclopedia.txt", piscina);

chorda linea;
dum (filum_lector_lineam_proximam(lector, &linea))
{
    imprimere("Versus %d: %.*s\n",
              filum_lector_numerus_versus(lector),
              linea.mensura, linea.datum);
}

filum_lector_claudere(lector);
```

**CARTER:** The reader opens the file and reads one line at a time. It tracks the line number — useful for error reporting. It strips the newline characters automatically. And when the line is longer than the internal buffer — four thousand and ninety-six bytes — it uses the ChordaAedificator from episode three to assemble the fragments.

**MARCUS:** *(studying the code)* The string builder from three episodes ago. Used here.

**CARTER:** Exactly. The dependency chain in action. We built the string builder for assembling long strings from pieces. Now the file reader uses it to assemble long lines from buffer-sized chunks. The tool we built for one problem solves a different problem.

**DATA:** *(noting)* Reuse. The characteristic of well-designed infrastructure. A road built for military movement is later used for commerce. The purpose changes. The road remains.

**MARCUS:** *(a rare smile)* You are learning to think like a Roman.

### Beat 11: Saving the Census — The First Real Persistence

The team saves the census to disk. This is the payoff for three episodes of data entry and indexing.

Carter writes the export function:

```c
/* Salvare censum ad discum */
FilumScriptor* scriptor = filum_scriptor_aperire(
    "census_nova_roma.txt",
    FILUM_MODUS_CREARE,
    piscina);

per (i = ZEPHYRUM; i < census->numerus; i++)
{
    Civis* civis = xar_elementum(census, i);

    /* Scribere singulum civem */
    filum_scriptor_scribere_literis(scriptor,
        civis->praenomen);
    filum_scriptor_scribere_literis(scriptor, ", ");
    filum_scriptor_scribere_literis(scriptor,
        civis->occupatio->datum);
    filum_scriptor_scribere_literis(scriptor, ", ");
    filum_scriptor_scribere_literis(scriptor,
        civis->regio->datum);
    filum_scriptor_scribere_literis(scriptor, "\n");
}

/* Sync — commit to stone */
filum_scriptor_sync(scriptor);
filum_scriptor_claudere(scriptor);
```

The progress counter runs. Three thousand citizens. Three thousand lines carved to disk. The disk light blinks steadily — a heartbeat.

```
Census salvatus: 3217 cives
Mensura in disco: 187,433 bytes
Tempus scripturae: 0.34 secundae
```

**MARCUS:** *(watching the progress)* Three thousand citizens. Carved to stone in a third of a second. *(beat)* The last Roman census took four years to complete and three months to compile. This takes a third of a second to *save*.

**CARTER:** And if the power drops tonight, we read it back tomorrow. The census survives.

**PLINY:** *(standing)* My encyclopedia. Save it now.

Carter runs the same operation on the encyclopedia. Twelve thousand entries. The disk light blinks for two seconds.

```
Encyclopaedia salvata: 12,847 indices
Mensura in disco: 2,341,872 bytes
Tempus scripturae: 1.87 secundae
```

**PLINY:** *(very quietly)* Forty years of writing. Two seconds to preserve. *(beat)* If I had had this tool in Stabiae, the original would have survived the eruption.

The room hears it. Pliny's mortality — the Vesuvius thread from 1x05 — surfaces for a single sentence and then submerges. He is a man who died trying to save his work. Now the work can save itself.

### Beat 12: ACT BREAK — The Error

Teal'c has been testing. He tries to read a file that doesn't exist:

```c
chorda resultatus = filum_legere_totum(
    "non_existens.txt", piscina);

si (resultatus.mensura == ZEPHYRUM)
{
    constans character* error = filum_error_recens();
    imprimere("Error: %s\n", error);
    /* "stat fracta" */
}
```

**TEAL'C:** The file does not exist. The read returns empty. The error system reports the failure.

**CARTER:** Right. The error buffer stores the most recent error. Thread-local. It doesn't crash — it reports and lets you decide what to do.

**TEAL'C:** *(a beat)* What do you do when the stone you are reading has been destroyed?

**CARTER:** You handle the error. You tell the user the file doesn't exist. You don't pretend it does.

**TEAL'C:** *(nodding)* The test that matters is not whether the system can read what exists. It is whether the system is honest about what does not exist. *(beat)* Many systems fail this test. They pretend. They fabricate. They return something rather than nothing, because nothing is uncomfortable.

**CARTER:** The filum library returns nothing. Honestly.

**TEAL'C:** Good. I will write the tests.

---

## ACT FOUR: "Conservare" (To Preserve)

*The emotional center. Joe Dawson arrives. Two documentarians meet. The philosophy of preservation.*

### Beat 13: Joe Dawson's Introduction

**Location:** The road into Nova Roma. Midday. A figure walks toward the settlement — limping, using a cane, carrying a leather bag. He looks like a man who has walked a very long way and is used to it.

Duncan sees him first. Stops dead.

**DUNCAN:** *(to O'Neill, very quiet)* That's Joe Dawson.

**O'NEILL:** Who?

**DUNCAN:** A Watcher. They observe Immortals. Record our lives. They don't interfere — they document.

**O'NEILL:** *(instinctively)* He's intelligence.

**DUNCAN:** He's a *librarian*.

Joe reaches them. His face when he sees Duncan — relief, warmth, the expression of a man who has been looking for someone for a long time.

**JOE:** MacLeod. You are a hard man to find.

**DUNCAN:** Joe. *(they embrace — old friends)* How did you get here?

**JOE:** I followed the trail. Your trail goes cold in interesting places, Duncan. When it went cold through a *Stargate*, I figured the story was worth the walk.

**O'NEILL:** *(assessing)* You walked through the Stargate alone?

**JOE:** I've walked into worse places for a good story.

Joe's introduction is quick, warm, and establishes his character in thirty seconds: he's a documentarian, he's stubborn, he's been doing this for decades, and he's not afraid of anything except an untold story.

### Beat 14: Joe and Pliny — Two Documentarians

**Location:** Pliny's archive room. Evening. Joe is shown around. Pliny's scrolls on one side. The digital encyclopedia on the other. Joe takes it all in — the leather bag over his shoulder, the same expression Pliny has when he sees something worth recording.

**JOE:** So you're the one who cataloged nature.

**PLINY:** I attempted to catalog *all* of it. I fell short.

**JOE:** *(laughing)* Brother, I've been trying to catalog Immortal history for thirty years. I've got... maybe forty percent of one country.

They look at each other. Two men who have dedicated their lives to documentation — different subjects, different centuries, the same compulsion.

**PLINY:** Why do you document them?

**JOE:** Because if nobody writes it down, it didn't happen. *(beat)* I've seen Immortals die. I've seen their chronicles closed. And the ones who had a Watcher — at least someone knows they existed. The ones who didn't... *(he shrugs)* They're just gone.

**PLINY:** *(very still)* I know what that is. I spent eighty years ensuring that observations of the natural world survived my death. Not the observations — those will be repeated. The *connections*. The understanding that this mineral comes from this mine, that this plant grows in this climate, that this remedy uses this herb. The web of knowledge. *(beat)* If I had died in Stabiae without my books...

**JOE:** ...then nobody would know what you knew.

**PLINY:** Nobody would know what *anyone* knew. I was compiling. Not inventing. Recording what a thousand observers before me had seen and organizing it into a system. *(beat)* If the compilation burns, you don't just lose one man's work. You lose the thread that connected a thousand observations into knowledge.

**JOE:** *(setting down his bag, sitting down)* You know what the Watchers say? "History is not what happened. History is what was written down." *(beat)* That's why I'm here. Because whatever you people are building — I want to write it down.

**PLINY:** Then we need to show you how we save things to disk.

**JOE:** *(pulling out a leather journal — physical, analog, handwritten)* I'll use my own system too, if you don't mind. Belt and suspenders.

**PLINY:** *(looking at Joe's journal with the respect of one documentarian for another)* Two copies. One in stone. One in leather. *(beat)* Good.

This is the Joe/Pliny relationship: two men who believe that documentation IS civilization. They understand each other instantly because they share the same faith — not religious faith (though the show will parallel that), but the faith that writing things down matters. That the record is sacred. That the person who preserves knowledge serves everyone who comes after.

### Beat 15: Joe's Story — The Archive That Nearly Died

Joe tells a story. Not the Watcher mythology — a specific, personal story. The one that made him understand what preservation means.

**JOE:** We had an archive in Lyon. Watcher records going back to the sixteenth century. Handwritten chronicles, sketches, maps of Immortal movements across Europe. Four hundred years of observation.

**PLINY:** *(the scholar, immediately attentive)* What happened?

**JOE:** Nobody attacked it. Nobody burned it. The building was sound. The records were organized. *(beat)* The roof leaked. One winter. One bad storm. Water got into the east wing. Twenty years of records — 1780 to 1800, the Revolutionary period — soaked through. The ink ran. The pages welded together. Twenty years of *irreplaceable* observation, destroyed by a leak.

**PLINY:** *(quietly, a man who died in an eruption)* Not by the enemy. By neglect.

**JOE:** By neglect. The records weren't attacked. They weren't stolen. They just... weren't maintained. The roof was somebody's responsibility and that somebody retired and nobody replaced him and one winter... *(he trails off)*

**CARTER:** *(who has been listening from the doorway)* Digital files don't have roofs. But they have their own version of the same problem. If the disk fills up, if the filesystem corrupts, if the hardware degrades — the data is just as gone.

**JOE:** So what's the solution?

**CARTER:** Copies. Backups. Redundancy. The same word kept in two places. Three places. *(beat)* But that's for later. Right now we're learning to carve in stone for the first time. The backup strategy comes after we have something *to* back up.

**METHOS:** *(from the shadows — he's been listening, as always)* The Library of Alexandria had copies. Copies in Pergamum, copies in Antioch, copies in individual collections. The Library burned. Some copies survived. Most didn't. *(beat)* Copies help. But the original fire is the one you remember.

### Beat 16: ACT BREAK — The Save

Carter saves the interning table itself — the vocabulary of Nova Roma.

```c
/* Salvare internamentum — save the vocabulary */
FilumScriptor* scriptor = filum_scriptor_aperire(
    "vocabularium_nova_roma.txt",
    FILUM_MODUS_CREARE,
    piscina);

/* Scribere singulam chordam canonicam */
per (i = ZEPHYRUM; i < internamentum_numerus(intern); i++)
{
    chorda* canonica = internamentum_elementum(intern, i);
    filum_scriptor_scribere(scriptor, *canonica);
    filum_scriptor_scribere_literis(scriptor, "\n");
}

filum_scriptor_sync(scriptor);
filum_scriptor_claudere(scriptor);
```

```
Vocabularium salvatum: 1,466 chordae canonicae
Mensura in disco: 18,291 bytes
```

**CARTER:** The vocabulary of Nova Roma. Fourteen hundred and sixty-six words. On disk. If the power drops tonight, the words survive.

**PLINY:** *(touching the screen the way he touched it in the 1x06 Tag — Methos's gesture, unconsciously echoed)* The vocabulary of a civilization. Carved in stone.

**CARTER:** Not yet in stone. On a magnetic disk. But the principle is the same.

**MARCUS:** *(correcting her gently)* The principle is *older* than stone. The principle is: what matters must endure. How it endures is an engineering problem. *That* it must endure is a human one.

---

## ACT FIVE: "Quod Scribitur, Manet" (What Is Written, Remains)

*The full persistence. The reload test. The title earns itself.*

### Beat 17: The Reload Test

The team runs the definitive test. Carter saves everything — census, encyclopedia, vocabulary — to disk. Then she powers down the machine. Deliberately. Not a fluctuation — a full shutdown.

The screen goes dark.

The room is quiet. Everyone watching a blank screen.

Carter powers it back on. The machine boots. She opens the terminal.

```c
/* Legere censum ex disco */
chorda census_salvatus = filum_legere_totum(
    "census_nova_roma.txt", piscina);
imprimere("Census: %d bytes lecti\n",
          census_salvatus.mensura);

/* Legere encyclopaediam ex disco */
chorda enc_salvata = filum_legere_totum(
    "encyclopedia_nova_roma.txt", piscina);
imprimere("Encyclopaedia: %d bytes lecti\n",
          enc_salvata.mensura);

/* Legere vocabularium ex disco */
chorda vocab_salvatum = filum_legere_totum(
    "vocabularium_nova_roma.txt", piscina);
imprimere("Vocabularium: %d bytes lecti\n",
          vocab_salvatum.mensura);
```

```
Census: 187,433 bytes lecti
Encyclopaedia: 2,341,872 bytes lecti
Vocabularium: 18,291 bytes lecti
```

Everything. Every byte. Every citizen, every encyclopedia entry, every canonical word. Read back from disk. Identical to what was saved.

**MARCUS:** *(the engineering pragmatist, satisfied)* The stone held.

**PLINY:** *(reading his encyclopedia entries on screen — forty years of work, two seconds to save, one second to restore)* It is all here. Every entry. Every cross-reference indicator. Every volume, every classification, every observation. *(beat)* The eruption could not have taken this.

It's a small line, delivered quietly. But for a man who died at Vesuvius trying to save his work and his friends, the ability to persist knowledge beyond the death of the machine that created it is personal in a way it isn't for anyone else in the room.

### Beat 18: Teal'c's Test Suite

Teal'c has been building tests throughout the episode:

```
probatio_scribere_et_legere ........... VERITAS
probatio_appendere .................... VERITAS
probatio_superscribere ................ VERITAS
probatio_filum_existit ................ VERITAS
probatio_filum_non_existit ............ VERITAS
probatio_mensura ...................... VERITAS
probatio_lector_lineas ................ VERITAS
probatio_lector_linea_longa ........... VERITAS
probatio_scriptor_sync ................ VERITAS
probatio_directorium .................. VERITAS
probatio_copiare ...................... VERITAS
probatio_movere ....................... VERITAS
probatio_delere ....................... VERITAS
probatio_status ....................... VERITAS
probatio_error_recens ................. VERITAS
```

Fifteen tests. Fifteen VERITAS.

**TEAL'C:** All operations verified. Write, read, append, copy, move, delete. Directory creation. Error reporting. The streaming reader handles lines longer than the buffer. The streaming writer syncs on command.

**CARTER:** Clean sweep.

**TEAL'C:** *(one observation)* I tested the append mode extensively. When you append to a file that does not exist, the system creates it. This is generous behavior. *(beat)* Some systems would refuse. Yours creates the file and writes to it as though it already existed.

**CARTER:** The Roman civil law principle — when a citizen presents a new document to the archive, the archive does not refuse because no prior document exists. It creates the record.

**TEAL'C:** *(the freed slave who understands civic institutions)* A just system. It does not punish the first entry for being first.

### Beat 19: The Title Moment — Wax Tablets to Stone Inscriptions

**Location:** The Tabularium. Late day. The team gathered. The full dataset saved to disk. The dependency chain on the board, now six libraries deep.

Carter writes:

```
FILUM — TABULAE CERATAE

Dependency chain:
  Piscina (1x01) — arena allocation
  Chorda (1x03) — strings
  ChordaAedificator (1x03) — string builder
  TabulaDispersa (1x05) — hash table
  Internamentum (1x06) — canonical copies
  Filum (1x07) — persistence

Operations:
  filum_legere_totum    — read stone to wax
  filum_scribere        — write wax to stone
  filum_appendere       — carve below the last line
  filum_scriptor_sync   — the chisel strikes
```

**MARCUS:** *(reading the board, then addressing the room)* In Rome, the first thing a new settlement builds is walls. The second thing is a water supply. The third thing — before the forum, before the temples, before the baths — is the *tabularium*. The archive. The record office. *(beat)* Because a settlement without records is a camp. A settlement with records is a city.

**PLINY:** *(standing beside Marcus)* We have been a camp. Building tools, building vocabulary, building indexes — but all of it in wax. Erasable. Temporary. Vulnerable to the next storm.

**MARCUS:** Now we have a tabularium that writes in stone. *(beat)* Now we are becoming a city.

**CARTER:** *(smiling at Marcus)* Tabulae ceratae. Wax tablets. That's what we *were*. The episode title isn't what we've built — it's what we've left behind.

**MARCUS:** *(nodding)* We wrote in wax. Now we carve in stone. The wax tablets were necessary — you must draft before you carve. But the civilization is in the carving.

---

## TAG

### Beat 20: Carter and Marcus — The Evening Walk

Their ritual. The road out of Nova Roma. Stars overhead.

**MARCUS:** Today was different.

**CARTER:** How?

**MARCUS:** The previous episodes — the arena, the strings, the hash table, the interning — those were tools for the mind. Ways of organizing thought within the machine. *(beat)* Today was the first time the machine spoke to the world outside itself. The first time it reached beyond its own memory and left a mark.

**CARTER:** Persistence. The bridge between computation and record.

**MARCUS:** *(walking)* Every Roman road is a mark. Every aqueduct is a mark. Every inscription on every triumphal arch. These are the things that survive us. Not the thoughts — the records of thoughts. Not the conversations — the written letters. Not the Senate's debates — the carved laws.

**CARTER:** You're saying the file is more important than the program.

**MARCUS:** I'm saying the carving outlasts the carver. Your programs will be rewritten. Your data structures will be replaced. Better arenas, better hash tables — someone will build them. But the records you save today — the census, the encyclopedia, the vocabulary — those are the things that will matter in a thousand years.

**CARTER:** *(a long silence)* You really think in those terms. Thousands of years.

**MARCUS:** I am Roman. We built roads two thousand years ago that people still walk on. *(beat)* What are you building that will last two thousand years?

Carter doesn't answer. But she's thinking about it.

**CARTER:** Next, we need timestamps. Everything we're saving — the census, the encyclopedia — none of it has a date. We're carving in stone, but the stone doesn't say *when*.

**MARCUS:** *(the engineer)* A record without a date is evidence without a witness. It tells you *what* but not *when*.

**CARTER:** That's 1x08.

### Beat 21: Pliny and Joe — The Evening

**Location:** Pliny's villa. Wine. Two documentarians sitting across from each other.

**PLINY:** How many Immortals have you documented?

**JOE:** Personally? Thirty-seven. The full Watcher network? Thousands. Maybe ten thousand across all recorded history.

**PLINY:** *(staggered)* Ten thousand beings who do not die. *(he reaches for his notebook)* And you write their histories?

**JOE:** We try. Some cooperate. Some don't know we exist. Some know and resent it. *(beat)* The ones who resent it are usually the ones with the most to hide.

**PLINY:** *(a scholar's smile)* The subjects who most need documenting are the subjects who least want it.

**JOE:** *(laughing)* Brother, you have just summarized thirty years of my career.

**PLINY:** How do you preserve the records?

**JOE:** *(patting his journal)* Handwritten chronicles. Stored in secure locations. Copied by hand when the originals age.

**PLINY:** *(gesturing to the screen where the encyclopedia glows, saved to disk)* We can do better than that now. *(beat)* Carter showed me today. The machine saves to disk. The disk survives the loss of the machine. If we save your chronicles...

**JOE:** *(considering)* Digital copies of Watcher records. *(a long pause)* The Watchers have been pen-and-paper since the eighteenth century. Before that, we were papyrus and vellum. *(beat)* Are you suggesting we digitize?

**PLINY:** I am suggesting that a chronicle that exists in one copy is a candle. A chronicle that exists on disk is a lamp. And a chronicle that exists on multiple disks...

**JOE:** ...is a lighthouse.

They drink. Two documentarians, separated by two thousand years, arriving at the same conclusion: redundancy is the guardian of knowledge.

FADE TO BLACK.

---

## STINGER

### Beat 22: Kirk — The Filing System

**Location:** Earth. Night. The Synthesis Dynamics office. Larger than before — the glass-walled conference room from 1x06, but now an entire floor. Kirk's operation has grown.

Kirk stands before a wall of filing cabinets — not digital, physical. Metal drawers. Manila folders. Old-fashioned, deliberate, analog.

An aide enters.

**AIDE:** Sir, the digital systems are secure. Why do we maintain physical files?

**KIRK:** *(not turning around)* Because digital can be hacked. Digital can be searched by someone other than us. Digital can be subpoenaed. *(he opens a drawer, pulls a folder)* Paper can be burned. Paper can be moved. Paper exists where you put it and nowhere else.

He opens the folder. Inside: photographs, printed emails, handwritten notes. A dossier. The name on the tab: **MACLEOD, D.**

**KIRK:** *(closing the folder, returning it)* Digital for operations. Paper for secrets. *(beat)* The Romans understood this. Wax for messages — you smooth the wax, the message is gone. Stone for propaganda — you want the public to see it. But the real records? The intelligence? *(he taps the filing cabinet)* Those go in a place with a lock and a match.

He pulls another folder. The tab reads: **NOVA ROMA — GATE ACCESS**

**KIRK:** *(to the aide)* Seven cities, thirty-one operatives. Every operation filed. Every contact documented. Every pathway recorded. *(beat)* If I lose the digital systems tomorrow, I still have these.

He writes on the whiteboard: **7 / 31**.

Below it, a new word: **TABULARIUM**

**KIRK:** *(looking at the word, appreciating the irony of using the same Latin term the team uses)* The Romans built tabularia to store the records of their empire. Census data, tax records, military dispatches. *(beat)* The records ARE the empire. Lose the records, lose the empire.

He closes the filing cabinet. Locks it. The camera holds on the lock.

**KIRK:** *(to himself, heading for the door)* What is written, remains.

The same phrase Marcus said. The same principle. But Kirk's records are surveillance, not scholarship. Kirk's tabularium is a weapon, not an archive. Same technology. Same Latin word. Opposite purpose.

SMASH TO BLACK.

END OF EPISODE.

---

## ON-SCREEN CODE MOMENTS

### Moment 1: The Empty Internamentum (Cold Open)
```c
InternamentumChorda* intern = internamentum_globale();
i32 numerus = internamentum_numerus(intern);
/* numerus = 0 */
```
The wake-up call. Everything gone.

### Moment 2: The First Write (Act 2)
```c
chorda salutatio = chorda_ex_literis("Salve, Nova Roma!", piscina);
b32 successus = filum_scribere("salutatio.txt", salutatio);
```
Seventeen bytes. The first mark carved in digital stone.

### Moment 3: The Streaming Writer (Act 2)
```c
FilumScriptor* scriptor = filum_scriptor_aperire(
    "census_novus.txt", FILUM_MODUS_CREARE, piscina);
filum_scriptor_lineam_scribere(scriptor,
    "Gaius Tertius, Faber, Regio Prima");
```
Line by line. The chisel and the stone.

### Moment 4: The Sync (Act 2)
```c
filum_scriptor_sync(scriptor);
```
One line. The most important line. The moment wax becomes stone.

### Moment 5: The Two Modes (Act 2 Break)
```c
FILUM_MODUS_CREARE    /* Smooth the wax. */
FILUM_MODUS_APPENDERE /* Carve below the last line. */
```
The Borg overwrite. Civilizations append.

### Moment 6: The Streaming Reader (Act 3)
```c
FilumLector* lector = filum_lector_aperire(
    "encyclopedia.txt", piscina);
chorda linea;
dum (filum_lector_lineam_proximam(lector, &linea))
{
    /* Process each line */
}
```
The stone speaks. Line by line.

### Moment 7: File Status Query (Act 3)
```c
FilumStatus status;
filum_status("census.txt", &status);
/* mensura, est_filum, potest_legere, potest_scribere */
```
The catalog entry. Every file tells you what it is.

### Moment 8: The Vocabulary Save (Act 4 Break)
```c
per (i = ZEPHYRUM; i < internamentum_numerus(intern); i++)
{
    chorda* canonica = internamentum_elementum(intern, i);
    filum_scriptor_scribere(scriptor, *canonica);
}
filum_scriptor_sync(scriptor);
```
1,466 canonical words. Saved to disk. The vocabulary survives.

### Moment 9: The Reload Test (Act 5)
```c
chorda census_salvatus = filum_legere_totum(
    "census_nova_roma.txt", piscina);
/* 187,433 bytes — identical to what was saved */
```
The stone speaks the same words tomorrow that you carved today.

---

## STRUCTURAL NOTES

### The Episode's Architecture

The episode has a **loss-to-recovery arc** organized around the wax/stone metaphor:

1. **Cold Open:** The loss. Everything gone. The vulnerability of wax.
2. **Act 1:** The diagnosis and the metaphor. Marcus names the problem. Carter names the solution.
3. **Act 2:** Writing. Learning to carve. The sync as commitment. The overwrite/append distinction.
4. **Act 3:** Reading. Learning to listen to the stone. The streaming reader. The first real persistence (census and encyclopedia saved).
5. **Act 4:** The philosophy. Joe Dawson and Pliny — why preservation matters. Not just how, but *why*.
6. **Act 5:** The proof. The reload test. The carving survives the carver.

### The Emotional Structure

Acts 1-3 are *technical* — learning to save and read files. The audience learns what persistence means through code and Roman analogy.

Act 4 is *emotional* — Joe and Pliny, two men who have spent their lives documenting, recognize each other. The Watcher archive that nearly died from neglect. The encyclopedia that Pliny couldn't save from Vesuvius. The human *need* for persistence — not just the technical capability.

Act 5 is *triumphant* — the reload test. The stone holds. The carving survives.

### The Marcus Episode

This is Marcus's strongest episode since 1x04. The wax/stone metaphor is HIS — it comes from his material culture, his engineering training, his understanding of Roman infrastructure. Carter builds the library. Marcus provides the conceptual framework. When he says "Now we are becoming a city," it's the highest compliment he can give, and it carries because Marcus has been establishing the meaning of Roman urbanism since his introduction.

### The Joe/Pliny Pairing

Joe and Pliny are the heart of this episode. They're not the technical implementers — Carter, Teal'c, and Data handle that. They're the *reason* the technology matters. Two documentarians who understand, from decades of personal experience, that knowledge dies when it isn't preserved. Their instant recognition of each other — the shared compulsion, the shared faith in documentation — is the B-plot's emotional engine.

### The Title's Meaning

"Tabulae Ceratae" — Wax Tablets — is what the team's system has been until now. Everything in volatile memory. Everything written in wax. The title names what they're *leaving behind*, not what they're becoming. The episode is a farewell to wax. The future is stone.

---

## TECHNICAL FIDELITY NOTES

All code shown on screen uses the actual filum.h API:
- `filum_legere_totum(via, piscina)` — exact signature
- `filum_scribere(via, contentum)` — exact signature
- `filum_scriptor_aperire(via, modus, piscina)` — exact signature with FilumModus enum
- `filum_scriptor_lineam_scribere(scriptor, linea)` — exact signature
- `filum_scriptor_sync(scriptor)` — exact signature
- `filum_lector_aperire(via, piscina)` — exact signature
- `filum_lector_lineam_proximam(lector, linea_out)` — exact signature
- `filum_status(via, status_out)` — exact signature with FilumStatus struct
- `filum_error_recens()` — exact signature
- `FILUM_MODUS_CREARE` and `FILUM_MODUS_APPENDERE` — exact enum values

The dependency chain is accurate: filum.h includes chorda.h and piscina.h. The implementation uses chorda_aedificator.h for long-line handling. The error system is thread-local (simplified to static in the actual implementation).

---

## KIRK STINGER PROGRESSION

| Episode | Number | Word | Kirk's Operation |
|---------|--------|------|-----------------|
| 1x01 | — | — | Arrival. Data/Kirk misdirect. |
| 1x02 | — | — | Exposed. Escapes. |
| 1x03 | — | — | Sightings. |
| 1x04 | 3/31 | — | Network building. Dark Xar — nodes that don't move. |
| 1x05 | 5/31 | TABULARIUM | Whiteboards. Names indexed. Dark hash table. |
| 1x06 | 6/31 | LEXICON | Style guide. Canonical phrases. Dark interning. |
| **1x07** | **7/31** | **TABULARIUM** | **Filing cabinets. Physical records. Dark file I/O.** |

Kirk's stinger inverts the team's achievement: the team learns to save data digitally for the first time; Kirk maintains *physical* files because digital can be hacked. Same preservation instinct, opposite security model. The team is moving from analog to digital. Kirk is maintaining both — and the physical files are the ones that matter because they can be destroyed.

The word TABULARIUM appears on Kirk's whiteboard for the *second* time — first in 1x05 (the hash table episode, where it meant "the archive"), now in 1x07 (the file I/O episode, where it means "the records office"). The repetition is deliberate: Kirk is naming his infrastructure after the same Roman institution the team uses. Two tabularia. One for knowledge. One for control.
