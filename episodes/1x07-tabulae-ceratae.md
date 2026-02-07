# NOVUM FUNDAMENTUM
## Episode 1x07: "Tabulae Ceratae" (Wax Tablets)
### Beat Sheet (Draft 1 — Compiled from Writers' Room)

---

**Library:** Filum (File I/O — persistence to disk)
**Dependencies:** Piscina (1x01), Chorda / ChordaAedificator (1x03)
**Theme:** The transition from volatile to permanent — from wax to stone. What you save, and for whom.
**Dramatic Engine:** Everything built in 1x01–1x06 is volatile. A power fluctuation wipes it all. File I/O is the bridge from wax (volatile) to stone (permanent). The first three acts ask HOW to persist. The last two ask WHY.

---

## COLD OPEN

**Location:** The Tabularium. Night. Storm outside — rain hammering the stone. The team is asleep. Only the machines are running.

The camera moves through the workshop. On screen: the interning table. The census. Pliny's encyclopedia. The 1,466 canonical words glowing in the dim light. The dependency chain, six episodes deep, all of it alive in memory.

A lightning flash. The lights flicker. The screen goes black for two heartbeats.

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

*The loss is understood. The Roman analogy is born. The solution is named. Joe Dawson arrives.*

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

**O'NEILL:** *(from the doorway, arms crossed)* You're telling me we lose a week of work because of a thunderstorm?

**CARTER:** *(meeting his eyes)* Yes.

**O'NEILL:** Fix that.

He leaves. Efficient as always.

### Beat C1 (Interwoven — Early Act 1): Joe/O'Neill Briefing

Joe Dawson has arrived through the gate with a small group of Earth personnel — logistics, communications, engineers O'Neill requisitioned. Joe is listed as "cultural liaison." He's the last one through. He pauses at the event horizon — a man crossing a threshold he can't uncross.

On the Nova Roma side: the limp, the cane, the guitar case on his back, the leather satchel. He takes in Nova Roma.

First person he sees: Duncan.

**JOE:** *(looking around at the Forum, the senate building, the three thousand citizens)* You could've mentioned the Romans.

**DUNCAN:** I did mention the Romans.

**JOE:** You said "a colony." You said "some locals." *(gestures)* Duncan, this is a CIVILIZATION.

**DUNCAN:** *(the smallest smile)* Yeah. That's why I asked you to come.

O'Neill is present. He pulls Joe aside shortly after.

**O'NEILL:** One thing. We have a situation. An Earth-side situation. You might know something useful.

**JOE:** What kind of situation?

**O'NEILL:** We're tracking a network. Kirk's operation — seven sites we know about, probably more. Systematic acquisition. Standardized communications. Growing.

**JOE:** *(his Watcher instincts engaging — decades of tracking covert Immortal networks through aliases and front companies)* Growing how?

**O'NEILL:** New city on the map every two weeks. All legitimate-looking businesses. All connected.

**JOE:** I might have some thoughts on that. The Watchers have been tracking covert networks for a thousand years.

Joe scans the environment — the villa, the computing equipment, the Romans. His eyes settle on Data working at a terminal. Then on Midas, walking through the Forum with a basket of herbs. The eyes of a man who has spent thirty years cataloging. O'Neill has a new intelligence asset.

**Timing:** 90 seconds total (arrival + briefing).

### Beat 2: Marcus Names It — Wax and Stone

Marcus approaches the board.

**MARCUS:** In Rome, we have two materials for writing. *(he draws)* Cera — wax. And lapis — stone.

He draws a wax tablet:

**MARCUS:** A tabula cerata — a wax tablet. You write in it with a stylus. When you're finished, you smooth the wax and write again. It is for temporary things: notes, calculations, drafts, letters. The writing is real while it exists. But it is understood to be *temporary*. The wax exists to be overwritten.

He draws a stone inscription:

**MARCUS:** An inscriptio lapidea — a stone inscription. You carve it with a chisel. It takes time. It takes skill. It takes intention. And once it is carved, it endures. Laws. Treaties. Dedications to the gods. The names of the dead. *(beat)* These are carved in stone because they must survive the person who carved them.

He pulls a stylus from his satchel — an iron rod, pointed at one end, flat and spatulate at the other.

**MARCUS:** The stylus. Write on one end. *(he turns it)* Erase on the other. The instrument itself holds both operations. Cicero called it *stilum vertere* — "to turn the stylus." It means to revise. The entire concept of revision lives in a single tool.

*PRODUCTION NOTE: The stylus should be a recurring visual in this episode — the physical prop that embodies the create/overwrite concept. Marcus demonstrates the dual function: inscribe, turn, smooth.*

**MARCUS:** *(turning to the blank screen)* Your machine has been a wax tablet. Everything you wrote on it was real — but it was written in wax. The storm smoothed the wax. Now you need to learn to carve in stone.

**CARTER:** *(the connection clicking)* Memory is wax. Disk is stone.

**MARCUS:** And every civilization needs both. You draft in wax. You preserve in stone. The error was not in using wax — wax is useful, wax is fast, wax lets you think quickly. The error was in having *only* wax. In never carving what mattered into something that would survive the next storm.

*This is the episode's thesis in Marcus's voice: the error was not in using wax but in having only wax.*

### Beat 3: Carter's Blueprint — The Filum Library

Carter is at the board. She writes:

```
FILUM — "Thread" / "File"

Legere   — Read   (lapis → cera: stone to wax, disk to memory)
Scribere — Write  (cera → lapis: wax to stone, memory to disk)
Appendere — Append (add more carving to existing stone)
```

**CARTER:** Three operations. Read: take what's on disk and bring it into memory. Write: take what's in memory and put it on disk. Append: add to what's already on disk without destroying what was there before.

**DANIEL:** *(from the side)* *Filum* — thread. As in the thread of a narrative? The Ariadne thread?

**LUCIA:** And a filum is what connects. Ariadne's thread connects the present to the path already walked. A file connects the present state to the recorded past.

**DANIEL:** *(the etymologist, warming to it)* And the medieval *filum chartarum* — documents strung on a thread for storage. A *file* of papers, literally threaded together. The English word "file" comes from this. *(beat)* *Codex* is the same thread — the word starts as a block of wood, becomes a bundle of tablets, becomes a bound book, becomes "code." The tablet is in the code.

**CARTER:** *(writing)* Two modes:

```
FILUM_MODUS_CREARE   — Create / Overwrite
  (Smooth the wax. Start fresh.)

FILUM_MODUS_APPENDERE — Append
  (Carve below the last line.)
```

**MARCUS:** *(immediately)* The first is for drafts. The second is for records.

**PLINY:** *(from his chair, very quiet)* My encyclopedia. Forty volumes. Each volume appended to the last. I have never smoothed the wax on a completed volume. I have only added new pages.

### Beat 4: ACT BREAK — The Dependency Chain Grows

Carter writes the new dependency chain on the board:

```
Piscina (1x01) — arena allocation
  -> Chorda (1x03) — strings
    -> ChordaAedificator (1x03) — string builder
      -> Filum (1x07) — persistence
```

**CARTER:** Filum needs Piscina to allocate read buffers. It needs Chorda to represent file contents. It needs ChordaAedificator for the streaming reader — when a line is longer than the buffer, the builder assembles the fragments.

**MARCUS:** *(studying the board)* Six libraries deep. Seven episodes.

**CARTER:** And for the first time, the dependency chain *branches*. Filum doesn't depend on the hash table or the internamentum. It goes back to Chorda and Piscina and builds a *different* thing.

**DATA:** *(precise)* The previous libraries transformed data within the system. This library transfers data between the system and its environment. It is the boundary between computation and persistence. Between thought and record.

**MARCUS:** Between wax and stone.

---

## ACT TWO: "Scribere" (To Write)

*The first write. The sync moment. The overwrite/append distinction deepens.*

### Beat 5: The First Write

Carter codes. The audience sees the simplest possible operation:

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

The disk light blinks. One blink.

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

O'Neill has been standing at the back. Very quietly, after the successful write/read:

**O'NEILL:** ...That would have been useful.

Carter hears it. She knows what he means. Nobody explains. The man who once had the entire knowledge of the Ancients dumped into his brain — volatile, unindexed, unsaveable — and lost it all when the storage medium was wiped.

*DIRECTION NOTE: The "Fifth Race" resonance is implicit, never stated. O'Neill doesn't say "when the Ancients filled my brain." The audience fills it in.*

### Beat 6: The Streaming Writer — Carving Line by Line

Carter introduces the streaming writer:

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

**CARTER:** Three lines. Three citizens. Each one carved into the file. But here's the thing — these lines are not on disk yet.

**MARCUS:** *(confused)* You just wrote them.

**CARTER:** I wrote them to a *buffer*. A holding area. The machine collects the writes and sends them to disk in batches — it's more efficient. But it means there's a gap between "I said write" and "it's actually on the disk."

**MARCUS:** *(his engineer's instinct firing)* A gap during which the power could drop.

**CARTER:** Exactly.

### Beat 7: The Sync Moment — The Chisel Strikes

```c
/* Sync — "carve it now" */
filum_scriptor_sync(scriptor);
```

**CARTER:** *Sync*. This is the command that says: "Stop holding. Write it now. Push everything in the buffer to the physical disk. Don't return until it's done."

**MARCUS:** *(understanding)* The chisel strikes the stone.

**CARTER:** Exactly. Before sync, the data is in the buffer — wax. After sync, the data is on disk — stone. The sync is the moment of commitment.

**PLINY:** *(thoughtfully)* In my practice, there is the same moment. The draft is in ink on papyrus — changeable, scratchable, temporary. When I hand the scroll to the copyist and say "this is the final text" — that is the sync. The words pass from my custody to the archive's. They become the record.

Carter writes on the board:

**Buffer is wax. Disk is stone. Sync is the chisel.**

**MARCUS:** *(standing, walking to the board)* And the discipline is knowing *when* to sync. If you sync after every word, you spend all your time carving and none thinking. If you never sync, everything stays in wax and the first wind destroys it. *(beat)* The Roman builder syncs at the completion of each course of stone — not each individual block, but each completed layer.

### Beat 8: ACT BREAK — Overwrite vs. Append

Carter demonstrates the two modes:

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

**PLINY:** *(who has been writing since the conversation started)* An encyclopedia that overwrites its own entries is not an encyclopedia. It is propaganda.

**DATA:** *(precise)* An append-only record is a history. An overwrite record is a state. History preserves the past. State replaces it. The distinction between FILUM_MODUS_APPENDERE and FILUM_MODUS_CREARE is the distinction between a civilization that remembers and one that forgets.

Marcus writes on the board in Latin:

***Civilitas quae appendit, meminit. Civilitas quae superscribit, obliviscitur.***
*A civilization that appends, remembers. A civilization that overwrites, forgets.*

---

## ACT THREE: "Legere" (To Read)

*Reading files. The streaming reader. The census and encyclopedia saved. The system is honest about what does not exist.*

### Beat 9: The First Read + File Status

Carter demonstrates reading:

```c
/* Legere filum totum — read entire file */
chorda contentum = filum_legere_totum(
    "census.txt", piscina);

imprimere("Legere %d bytes ex disco\n",
          contentum.mensura);
```

**CARTER:** Read. The stone speaks. What was carved yesterday is read today.

**PLINY:** *(a question that reveals the scholar)* When I read a scroll, I can feel the age of the papyrus. I can see where the ink has faded, where a previous scribe corrected a word. When the machine reads from disk, does it know anything about the file?

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

**CARTER:** Size. Type. Permissions — can it be read, can it be written. The file carries metadata. Not as rich as your papyrus — but the operating system records what it knows.

**PLINY:** *(writing in his notebook)* Size, type, permissions. A catalog entry. Every file is cataloged.

### Beats 10–11: Streaming Reader + Saving the Census and Encyclopedia

For large files, you don't read everything at once. You read line by line:

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

**CARTER:** The reader opens the file and reads one line at a time. It tracks the line number. It strips newlines automatically. And when the line is longer than the internal buffer — four thousand and ninety-six bytes — it uses the ChordaAedificator from episode three to assemble the fragments.

**DATA:** *(noting)* Reuse. The characteristic of well-designed infrastructure. A road built for military movement is later used for commerce. The purpose changes. The road remains.

Now the team saves the census to disk:

```c
/* Salvare censum ad discum */
FilumScriptor* scriptor = filum_scriptor_aperire(
    "census_nova_roma.txt",
    FILUM_MODUS_CREARE,
    piscina);

per (i = ZEPHYRUM; i < census->numerus; i++)
{
    Civis* civis = xar_elementum(census, i);
    filum_scriptor_scribere_literis(scriptor,
        civis->praenomen);
    filum_scriptor_scribere_literis(scriptor, ", ");
    filum_scriptor_scribere_literis(scriptor,
        civis->occupatio->datum);
    filum_scriptor_scribere_literis(scriptor, "\n");
}

filum_scriptor_sync(scriptor);
filum_scriptor_claudere(scriptor);
```

The progress counter runs:

```
Census salvatus: 3217 cives
Mensura in disco: 187,433 bytes
Tempus scripturae: 0.34 secundae
```

**MARCUS:** Three thousand citizens. Carved to stone in a third of a second. *(beat)* The last Roman census took four years to complete and three months to compile. This takes a third of a second to *save*.

Carter runs the same operation on the encyclopedia:

```
Encyclopaedia salvata: 12,847 indices
Mensura in disco: 2,341,872 bytes
Tempus scripturae: 1.87 secundae
```

**PLINY:** *(very quietly)* Forty years of writing. Two seconds to preserve. *(beat)* If I had had this tool in Stabiae, the original would have survived the eruption.

The room hears it. Pliny's mortality — the Vesuvius thread from 1x05 — surfaces for a single sentence and then submerges.

### Beat C2 (Interwoven — During Act 3): Carter Saves Temporal Data

Carter, alone at her workstation. The first thing she does with the new Filum library in private isn't census data or encyclopedia backup. It's her secret.

She writes her compass data to disk. Every reading. Every oscillation pattern. Every measurement she's been accumulating since 1x05.

**CARTER:** *(staring at the file listing, alone)* There. Now it's real.

And then the implication hits her. Everything in memory was safe because it was *ephemeral*. A file on disk is different. A file can be found. A file can be read. A file is *evidence*.

She stares at the filename. She doesn't delete it. The secret has changed form — from ephemeral thought to persistent artifact. Carter's burden didn't lighten. It solidified.

But she notices something she couldn't have noticed with intermittent handwritten measurements: with persistent, continuous data, the oscillation pattern isn't random. It's *periodic*. A frequency. Not chaotic, not decaying — structured. This is the first hint that the temporal anomaly isn't a random disturbance. It's a signal.

She doesn't share this. She saves it. Closes the file. Seeds 1x08.

**Timing:** 45–60 seconds.

### Beat 12: ACT BREAK — Teal'c's Error Test

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

**CARTER:** Right. The error buffer stores the most recent error. It doesn't crash — it reports and lets you decide what to do.

**TEAL'C:** *(a beat)* The test that matters is not whether the system can read what exists. It is whether the system is honest about what does not exist. *(beat)* Many systems fail this test. They pretend. They fabricate. They return something rather than nothing, because nothing is uncomfortable.

**CARTER:** The filum library returns nothing. Honestly.

**TEAL'C:** Good. I will write the tests.

---

## ACT FOUR: "Conservare" (To Preserve)

*Joe Dawson arrives in full. Two documentarians meet. The philosophy of preservation. Casey says grace. The WHY of persistence.*

### Beat 13: Joe Dawson — First Contact

**Location:** The Forum. Midday. Duncan gives Joe the tour. Joe doesn't absorb things through tours. He absorbs them through conversation.

**DUNCAN:** Marcus designed the census infrastructure. Carter built the hash tables. Pliny's encyclopedia is fully indexed now.

**JOE:** And you?

**DUNCAN:** I hit things with a sword when they need hitting.

**JOE:** *(amused)* Some things don't change.

**DUNCAN:** Some things do. *(a beat, genuinely)* I'm learning to build, Joe. I spent four hundred years fighting. Here, I'm learning what happens after you win.

**JOE:** *(quietly)* That's the hard part, Mac. Winning is dramatic. Building is work.

Joe is warm where Methos is cold. Grounded where Data is precise. He's a man who has spent his entire adult life watching other people do extraordinary things and writing it down — because he believes the record is the thing that matters. He lost his legs in Vietnam. He found the Watchers afterward. They expelled him twice and took him back both times because his chronicles were too good to lose.

*Who knows what: Duncan knows everything. O'Neill knows Joe is a Watcher (he arranged the trip). Casey knows about Watchers (he's Immortal). The rest of the team sees "Duncan's friend, cultural liaison."*

### Beat 14: Joe and Pliny — Two Documentarians

**Location:** Pliny's villa. Evening. Joe exploring. Scrolls on one side. The digital encyclopedia on the other. Joe takes it all in — the leather bag over his shoulder, the same expression Pliny has when he sees something worth recording.

Pliny sees him in the doorway.

**PLINY:** You have the look of a man who writes things down.

**JOE:** *(laughing)* That obvious?

**PLINY:** I have spent eighty years looking for people who write things down. They are rarer than you think.

**JOE:** I've spent thirty years doing it. Watching. Recording. Never interfering. *(beat)* Well. Almost never.

**PLINY:** *(sitting, pouring wine)* Thirty years of observation. Of what?

**JOE:** People. Specific people. Their lives, their fights, their choices. Everything they did and everyone they met. *(beat)* Personally? One primary subject for twenty years.

**PLINY:** *(setting down the cup, with genuine reverence)* An organization devoted to observation?

**JOE:** Devoted to the record. We observe. We record. We don't interfere.

Pliny walks to his armaria — the wooden cabinets holding his scrolls. He touches one. Then pulls out a smaller notebook — his *adversaria*, his personal field notes.

**PLINY:** *(holding it)* My adversaria. My notes of observations and extracts. The working files. *(he looks at Joe's leather satchel)* You have your own.

**JOE:** *(patting the satchel)* Thirty years in here. *(beat)* And your thirty-seven volumes?

**PLINY:** Forty volumes now. Eighty years. Every observation verified by my own eyes or attributed to its source.

They recognize each other — same compulsion, same vocation. Two documentarians sitting across from each other with wine, separated by two thousand years.

**PLINY:** *(with sudden passion)* You *must* interfere. A scholar who only observes is a mirror. Useful, but limited. A scholar who engages — who asks the question that provokes the insight — is a *lens*. You magnify what you study.

**JOE:** That's been the argument for a thousand years.

**PLINY:** And what has a thousand years of non-interference produced?

**JOE:** *(long beat)* A lot of files. Some of them very good.

**PLINY:** I wrote forty volumes by going to places and asking questions and *touching* the things I described. Your order — these "Watchers" — they have the opposite instinct. They are scholars who fear corrupting their observations by participating.

**JOE:** *(quietly)* Yeah. So do I.

Joe pours himself wine without asking — a bartender's instinct. They settle in. Two documentarians, recognizing they share a faith — that writing things down matters. That the record is sacred.

*This is the episode's emotional center.*

### Beat C3 (Interwoven — During Act 4): Casey's Grace + O'Neill's "Huh"

**Location:** Pliny's villa. Evening. A proper dinner — not a feast, but generous. Pliny hosting: bread, olives, wine, roasted vegetables, a stew. Roman seating mixed with chairs for the Earth people. The full team present.

*PRODUCTION NOTE: Include the lararium (household shrine) in frame — a small shrine with figurines and a flame. Roman mealtime libation is established practice. Casey's prayer is recognizable as religio to the Romans, different in content but identical in form. Pliny's household would observe their own pre-meal offerings at the lararium.*

Joe is there — his first night on Nova Roma. He's charming the Romans already, telling stories, pouring wine. A bartender in any century. He's seated next to Pliny because the two documentarians have already been talking for hours.

The food arrives. People begin eating. Casey, seated between O'Neill and Marcus, pauses. He bows his head. Brief. Three seconds. His lips move. He crosses himself. He picks up his bread.

O'Neill, across the table, sees it. His fork stops midway. Not dramatically. Just... a pause. O'Neill files things. He's been doing it all season. He files this too.

Casey doesn't notice O'Neill noticing. He's already eating, talking to Marcus about brick composition.

At the same table: Joe and Methos are both present. Their eyes meet across the table. Methos raises his wine cup — the smallest acknowledgment. Joe nods. That's it. Three seconds. The Highlander fans will vibrate. Everyone else sees an enigmatic moment. The conversation comes later.

**Later.** Walking back from Pliny's villa. O'Neill falls into step with Duncan.

**O'NEILL:** Did Casey just pray over Roman food?

**DUNCAN:** He prays over everything.

**O'NEILL:** *(beat)* Huh.

Three lines. No judgment. No explanation. The commander of the mission noticing something about one of his people and filing it alongside everything else he files.

**Timing:** Two beats totaling 45 seconds (the grace itself + the walk-back exchange).

### Beat 15: Joe's Story — The Leaking Roof + Methos on Copies

**Location:** Still at Pliny's villa — after dinner. Smaller group now. Joe, Pliny, Carter in the doorway. Methos in the shadows.

**JOE:** We had an archive in Lyon. Watcher records going back to the sixteenth century. Handwritten chronicles, sketches, maps of Immortal movements across Europe. Four hundred years of observation.

**PLINY:** *(the scholar, immediately attentive)* What happened?

**JOE:** Nobody attacked it. Nobody burned it. The building was sound. The records were organized. *(beat)* The roof leaked. One winter. One bad storm. Water got into the east wing. Twenty years of records — 1780 to 1800, the Revolutionary period — soaked through. The ink ran. The pages welded together. Twenty years of *irreplaceable* observation, destroyed by a leak.

**PLINY:** *(quietly, a man who died in an eruption)* Not by the enemy. By neglect.

**JOE:** By neglect. The records weren't attacked. They weren't stolen. They just... weren't maintained. The roof was somebody's responsibility and that somebody retired and nobody replaced him and one winter...

**CARTER:** *(from the doorway)* Digital files don't have roofs. But they have their own version of the same problem. If the disk fills up, if the filesystem corrupts, if the hardware degrades — the data is just as gone.

**METHOS:** *(from the shadows — he's been listening, as always)* The Library of Alexandria had copies. Copies in Pergamum, copies in Antioch, copies in individual collections. *(beat)* Do you know what happened to all the copies?

**CARTER:** They burned too.

**METHOS:** They didn't burn. Most of them rotted. Decades of neglect. Regime changes where nobody cared about old scrolls. Institutional memory that nobody maintained because there was always something more urgent. *(beat)* The fire was the dramatic end. The real end was centuries of indifference.

*Two characters, from opposite ends of history, arriving at the same conclusion: the enemy of persistence isn't destruction. It's neglect. This IS the nominalism thread: if nothing has intrinsic worth, why maintain anything?*

### Beat 16: ACT BREAK — Saving the Vocabulary

Carter saves the interning table — the vocabulary of Nova Roma.

```c
/* Salvare internamentum — save the vocabulary */
FilumScriptor* scriptor = filum_scriptor_aperire(
    "vocabularium_nova_roma.txt",
    FILUM_MODUS_CREARE,
    piscina);

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

**MARCUS:** *(correcting her gently)* The principle is *older* than stone. The principle is: what matters must endure. How it endures is an engineering problem. *That* it must endure is a human one.

---

## ACT FIVE: "Quod Scribitur, Manet" (What Is Written, Remains)

*The full persistence. The reload test. The title earns itself.*

### Beat 17: The Reload Test

The team runs the definitive test. Carter saves everything — census, encyclopedia, vocabulary — to disk. Then she powers down the machine. Deliberately. Not a fluctuation — a full shutdown.

The screen goes dark.

The room is quiet. Everyone watching a blank screen. The cold open in reverse.

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

Everything. Every byte. Identical to what was saved.

**MARCUS:** *(the engineering pragmatist, satisfied)* The stone held.

**PLINY:** *(reading his encyclopedia entries on screen — forty years of work, two seconds to save, one second to restore)* It is all here. Every entry. Every cross-reference indicator. Every volume, every classification, every observation. *(beat)* The eruption could not have taken this.

Small line, delivered quietly. For a man who died at Vesuvius trying to save his work and his friends, the ability to persist knowledge beyond the death of the machine is personal in a way it isn't for anyone else in the room.

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

**TEAL'C:** *(one observation)* I tested the append mode extensively. When you append to a file that does not exist, the system creates it. This is generous behavior. Some systems would refuse. Yours creates the file and writes to it as though it already existed.

**CARTER:** The Roman civil law principle — when a citizen presents a new document to the archive, the archive does not refuse because no prior document exists. It creates the record.

**TEAL'C:** *(the freed slave who understands civic institutions)* A just system. It does not punish the first entry for being first.

### Beat 19: The Title Moment — Wax Tablets to Stone Inscriptions

**Location:** The Tabularium. Late day. The team gathered. The full dataset saved to disk. The dependency chain on the board, now seven libraries across two branches.

Carter writes:

```
FILUM — TABULAE CERATAE

Dependency chain:
  Piscina (1x01)       — arena allocation
  Chorda (1x03)        — strings
  ChordaAedificator (1x03) — string builder
  TabulaDispersa (1x05)    — hash table
  Internamentum (1x06)     — canonical copies
  Filum (1x07)             — persistence
```

**MARCUS:** *(addressing the room)* In Rome, the first thing a new settlement builds is walls. The second thing is a water supply. The third thing — before the forum, before the temples, before the baths — is the *tabularium*. The archive. The record office. *(beat)* Because a settlement without records is a camp. A settlement with records is a city.

He pauses. Looks at the full dependency chain on the board, at the data saved to disk, at the team.

**MARCUS:** Now we are becoming a city.

**CARTER:** *(looking at the title she wrote in the cold open)* Tabulae ceratae. Wax tablets. That's what we *were*. The title is what we've left behind.

---

## TAG

### Beat 20: Carter and Marcus — The Evening Walk

Their ritual. The road out of Nova Roma. Stars overhead.

**MARCUS:** Today was different.

**CARTER:** How?

**MARCUS:** The previous libraries — the arena, the strings, the hash table, the interning — those were tools for the mind. Ways of organizing thought within the machine. *(beat)* Today was the first time the machine spoke to the world outside itself. The first time it reached beyond its own memory and left a mark.

**CARTER:** Persistence. The bridge between computation and record.

**MARCUS:** *(walking)* Every Roman road is a mark. Every aqueduct is a mark. Every inscription on every triumphal arch. These are the things that survive us. Not the thoughts — the records of thoughts. Not the conversations — the written letters.

**CARTER:** You're saying the file is more important than the program.

**MARCUS:** I'm saying the carving outlasts the carver. Your programs will be rewritten. Your data structures will be replaced. Better arenas, better hash tables — someone will build them. But the records you save today — the census, the encyclopedia, the vocabulary — those are the things that will matter in a thousand years.

**CARTER:** *(a long silence)* You really think in those terms. Thousands of years.

**MARCUS:** I am Roman. We built roads two thousand years ago that people still walk on. *(beat)* What are you building that will last two thousand years?

Carter doesn't answer. But she's thinking about it.

**CARTER:** Next, we need timestamps. Everything we're saving — the census, the encyclopedia — none of it has a date. We're carving in stone, but the stone doesn't say *when*.

**MARCUS:** *(the engineer)* A record without a date is evidence without a witness. It tells you *what* but not *when*.

**CARTER:** That's 1x08.

### Beat 21: Pliny and Joe — The Evening + Three Voices

**Location:** Pliny's villa. Late evening. Wine. Two documentarians who have been circling each other all day, now settled in. The villa is warm. The *lararium* flickers.

**Sub-beat 1: The Three-Voice Algorithm Moment**

Data arrives at the villa. He's been working on file I/O all day and comes to join the conversation. Joe and Pliny have been talking about persistence for hours. Data's arrival crystallizes the three-voice moment:

**JOE:** *(leaning back, wine in hand)* You want to know what this is? It's what I do every night. I open the Chronicle. I write what I saw. Not what I think happened — what I SAW. Every detail. Then I close the book. The book goes on the shelf. If I die tonight — if the building burns, if the world ends — the book is still there. Someone else can open it. Someone else can read what I wrote. *(beat)* The observation survives the observer.

**PLINY:** *(hearing a kindred spirit)* I wrote for forty years. Every observation verified by my own eyes or attributed to its source. I died. *(the Vesuvius of it — a beat)* The volumes survived. Nineteen hundred years later, scholars still read them. Not because I was exceptional. Because I wrote them down. *(looking at Joe)* The observation survived the observer.

**DATA:** *(precise, moved in a way he doesn't fully parse)* A file is a named sequence of bytes stored on a non-volatile medium. When the system terminates — when power is removed, when the machine ceases to function — the file persists. The program that created the file may no longer exist. The programmer who wrote the program may no longer exist. The civilization that produced the programmer may no longer exist. The file remains. *(quieter)* File I/O is how the dead speak to the living.

*Three translations: Joe — persistence as the PRACTICE of documentation (the Watcher lens). Pliny — persistence as the TRIUMPH over mortality (the scholar's lens). Data — persistence as COMMUNICATION ACROSS TIME (the existential lens). The repeated phrase "the observation survives the observer" from Joe and Pliny, and then Data's inversion — "the dead speak to the living" — gives the three-voice moment its structure.*

**Sub-beat 2: Joe's Chronicle Entry**

The villa quiets. Pliny and Data have retired. Joe sits alone. He opens his leather journal — physical, analog, handwritten. He writes his first Nova Roma entry. The camera shows the handwriting:

*New assignment. Location: Nova Roma. Status: active.*

He pauses. Looks at the room — the scrolls, the screens, the *lararium* still flickering. Writes one more line:

*The observation survives the observer.*

He closes the journal. `FILUM_MODUS_CREARE`. A new file, opened, first line written.

FADE TO BLACK.

### Beat C4 (Tag): Daniel and Joe — The Methos File

**Location:** Joe's quarters. Late night. Daniel finds him.

**DANIEL:** Joe. I need to ask you something. About Methos.

**JOE:** *(carefully)* What about him?

**DANIEL:** He told me something. About Alexandria. About being there. *(beat)* About something he found under the Library.

Joe goes very still. The Methos Chronicle is the longest, most incomplete, most contradictory file in the Watcher archive. Five thousand years of fragments.

**JOE:** The Methos Chronicle is... complicated. There are entries going back to the Bronze Age that may or may not be the same man. What specifically did he tell you?

**DANIEL:** A sub-chamber. A priestess. Artifacts. Cults that changed people. He stopped himself before he finished.

**JOE:** *(a long exhale)* There's a Watcher entry from roughly 280 BC. Alexandria. An observer noted that "a man matching prior descriptions" was seen in the vicinity of the Serapeum. The entry says — and I'm quoting from memory — "the subject appeared disturbed by something in the lower levels."

**DANIEL:** "Disturbed."

**JOE:** That's a Watcher understatement. The observer's margin note, which didn't make the official record, says: "First recorded instance of the subject voluntarily leaving a center of learning."

Daniel files this. A Watcher observer in 280 BC saw Methos leave Alexandria in a hurry after going below the Serapeum. Independent corroboration.

**DANIEL:** Can I see the Chronicle?

**JOE:** *(long pause)* Parts of it. The ones that are safe to share. *(beat)* The Methos Chronicle has killed three Watchers and driven two others to quit. It has that effect on people who read too much of it.

**Timing:** 60–90 seconds. The thread connects Joe's arrival to the Methos/Alexandria/Black Stone thread from 1x06. The investigation goes from "Methos told me something weird" to "two independent sources confirm something happened under Alexandria."

---

## STINGER

### Beat 22: Kirk's ARCHIVUM

**Location:** Earth. Night. The Synthesis Dynamics office (established 1x06). Expanded. Kirk alone.

A wall of filing cabinets — not digital, *physical*. Metal drawers. Manila folders. Old-fashioned, deliberate, analog.

An aide enters.

**AIDE:** Sir, the digital systems are secure. Why do we maintain physical files?

**KIRK:** *(not turning around)* Because digital can be hacked. Digital can be searched by someone other than us. Digital can be subpoenaed. *(he opens a drawer, pulls a folder)* Paper can be burned. Paper can be moved. Paper exists where you put it and nowhere else.

He opens the folder. Inside: photographs, printed emails, handwritten notes. A dossier. The name on the tab: **MACLEOD, D.**

The gut-punch for Highlander fans. Kirk having a physical file on Duncan — with photos, printed communications, handwritten notes — tells the audience Kirk's intelligence operation is deeper than the team knows.

He closes the folder. Returns it.

**KIRK:** Digital for operations. Paper for secrets.

He pulls another folder. The tab reads: **NOVA ROMA — GATE ACCESS**.

**KIRK:** Seven cities, thirty-one operatives. Every operation filed. Every contact documented. Every pathway recorded. *(beat)* Everything filed. Everything recoverable.

He writes on the whiteboard: **7 / 31**.

Below it, a new word: **ARCHIVUM**

Kirk stinger progression updated: TABULARIUM (1x05) → LEXICON (1x06) → ARCHIVUM (1x07). Each word represents an escalation: indexing → standardizing → persisting.

**KIRK:** *(looking at the word, then at the filing cabinet)* What is written, remains.

The same phrase Marcus said in Act Five. Same principle. Same Latin phrasing. Opposite purpose. Marcus preserves for a city. Kirk preserves for control.

He closes the filing cabinet. Locks it. The camera holds on the lock.

*The team just learned to go FROM analog TO digital. Kirk is maintaining analog records BECAUSE digital is vulnerable. Same persistence instinct, opposite security model.*

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
    /* Processare singulam lineam */
}
```
The stone speaks. Line by line.

### Moment 7: File Status Query (Act 3)
```c
FilumStatus status;
filum_status("census.txt", &status);
/* mensura, est_filum, potest_legere, potest_scribere */
```
The catalog entry. Every file knows what it is.

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

## ENSEMBLE BEATS

### Carter — The Builder Under Strain
Carter drives this episode more than any since 1x01. She identifies the vulnerability, designs the solution, implements it, and defends the persistence model. But she's also carrying the compass data, the Midas temporal anomaly, and the growing awareness that something is wrong. Her private C2 beat — saving her secrets to disk, then realizing persistent secrets are findable secrets — shows the tension. She's the builder, but the builder is carrying weight.

### Marcus — The Roman Voice
This is Marcus's strongest episode since 1x04. The wax/stone metaphor is HIS — it comes from his material culture, his engineering training, his understanding of Roman infrastructure. He names the problem (Beat 2), provides the sync insight (Beat 7), delivers the title moment (Beat 19), and walks with Carter (Beat 20). When he says "Now we are becoming a city," it's the highest compliment a Roman engineer can give.

### Pliny — From Horror to Hope
Pliny's arc this episode moves from horror (Beat 1 — "Define 'everything'") to desperate urgency ("Do that with my encyclopedia") to quiet personal triumph (Beat 17 — "The eruption could not have taken this"). His bond with Joe is the episode's emotional discovery — two documentarians who understand each other instantly because they share the same faith: that writing things down matters.

### Joe Dawson — The New Voice
Joe arrives and immediately has something to contribute. He's not a tourist. He's a professional recorder who understands the stakes of persistence because he's lived the consequences of its failure. His leaking-roof story, his bond with Pliny, his intelligence value to O'Neill, and his Methos Chronicle data for Daniel — Joe intersects every running thread in his first episode. His three-voice algorithm moment defines him to the audience through his deepest insight.

### Methos — The Man Who Watched Libraries Burn
Methos's contribution is surgical. The Borg-overwrite callback in Beat 8. The Alexandria/copies warning in Beat 15 ("They didn't burn. They rotted."). The 3-second Joe acknowledgment at dinner. Methos doesn't have a centerpiece scene — his centerpiece was 1x06. Here he's a commentator, a warning system, and a man watching the team build tools he wishes had existed two thousand years ago. His silence at dinner, across the table from his Watcher, speaks louder than dialogue.

### Data — The Analyst Liberated
Data helps implement file I/O throughout the A-plot. His key lines: "The boundary between computation and persistence" (Beat 4), "Reuse" (Beats 10-11), and "File I/O is how the dead speak to the living" (Beat 21). This last line is the episode's best single line. Data also feels relief — he was the only reliable storage medium, and now he's supplementary. "I was never comfortable being essential to the preservation of knowledge."

### Teal'c — The Just System
Teal'c's contributions are his error test (Beat 12 — "Is the system honest about what does not exist?") and his test suite (Beat 18 — "It does not punish the first entry for being first"). Both are philosophical observations dressed as test results. The freed slave who insists on systems that tell the truth and treat newcomers fairly.

### O'Neill — The Filer
Three load-bearing beats: "Fix that" (Beat 1 — strategic command), "That would have been useful" (Beat 5 — personal resonance, Fifth Race implicit), and "Huh" (Beat C3 — filing Casey's prayer). O'Neill is the episode's observer-in-chief. He notices everything and acts on it later.

### Casey — The Quiet Practice
The faith thread re-engages after 1x06's rest. Casey says grace at Pliny's table — the same villa where the "God who remembers everything" conversation happened in 1x05. O'Neill notices. The circle of observers expands to include the commanding officer. Casey doesn't know he's being watched. He never does. That's the point.

### Duncan — Joe's Anchor
Duncan is background in the A-plot but essential in the character work. He arranged Joe's arrival. Their shorthand — "Mac" / "Good people" / "The best" — tells the audience this is a friendship built over decades. Duncan's line "I'm learning what happens after you win" grounds his season arc: the man who always left before things were finished, now staying to build.

### Lucia — The Etymologist
Lucia's contribution is the "filum" etymology scene (Beat 3) — the thread of Ariadne, the *filum chartarum*, the codex connection. She gives the library its Latin grounding, connecting the computational concept to two thousand years of material culture.

### Midas — Present but Peripheral
Midas is at the dinner. He walks through the Forum (Joe's eyes follow him — an uncharted Immortal, the Watcher's instincts firing). His temporal effect is what Carter saves in C2. He doesn't have a scene this episode — his spotlight was 1x06. Here he's the anomaly that radiates through other people's observations.

---

## THREE-VOICE ALGORITHM MOMENT

| Episode | Three Voices | Algorithm | Lens |
|---------|-------------|-----------|------|
| 1x05 | Pliny / Data / Casey | Hashing | Fingerprint of knowledge |
| 1x06 | Methos / Data / Casey | Interning | Canonical copy, one for all |
| **1x07** | **Joe / Pliny / Data** | **Persistence** | **Writing to survive** |

**Placement:** Tag, Beat 21 (Pliny's villa, evening). The three voices happen AFTER the team has built persistence, when Joe and Pliny are reflecting on what it means. Data joins them.

**Structure:**
- **Joe:** Persistence as PRACTICE — "The observation survives the observer."
- **Pliny:** Persistence as TRIUMPH OVER MORTALITY — "The observation survived the observer."
- **Data:** Persistence as COMMUNICATION ACROSS TIME — "File I/O is how the dead speak to the living."

The repeated phrase from Joe and Pliny, and Data's inversion, give the moment its architecture.

---

## THEMATIC NOTES

### Nominalism / Realism — Persistence as Ontological Upgrade

**In memory:** Data exists only while the machine runs. It's contingent — dependent on power, on process, on someone paying attention. Stop the machine, the data vanishes. The nominalist position: data is just information in use. It has no independent existence.

**On disk:** Data persists independently. It exists whether anyone reads it or not. It endures beyond the machine that created it, beyond the process that wrote it, beyond the person who entered it. The realist position: data has content — a nature — that deserves preservation independent of its use.

**The wax tablet metaphor is the bridge.** Wax tablets are re-writable — Roman schoolchildren wrote on wax, scraped it clean, wrote again. Stone inscriptions are permanent. `FILUM_MODUS_CREARE` (overwrite) is the nominalist mode — data is temporary, in service of current use. `FILUM_MODUS_APPENDERE` (append) is the realist mode — data has value beyond immediate use.

**Joe's Watcher philosophy** adds a new dimension: the Watchers observe but don't engage (nominalist approach — the observer should be transparent). Pliny engages with his subjects (realist approach — engagement doesn't corrupt, it enriches). Their dinner conversation dramatizes this without naming it.

**Kirk's persistence** is the dark nominalist version: data saved for *control*, not for *truth*. Kirk persists data so it can be *used* — recovered, deployed, weaponized. The team persists data so it can be *known* — preserved, studied, shared. Same operation. Different ontology.

### The Borg Parallel — Overwrite vs. Append

The Borg conversation from 1x06 carries forward through Methos's callback in Beat 8. The distinction deepens:

| | Team (File I/O) | Kirk (Archives) | Borg (Assimilation) |
|---|---|---|---|
| **Mode** | Both — create AND append | Create only — old reports overwritten | Overwrite — previous contents destroyed |
| **What persists** | The full record | Only the latest version | Only the collective |
| **Purpose** | Preserve knowledge | Preserve control | Preserve the hive |
| **The file** | Has a name, a nature, an identity | Has a classification, a use | Has no individuality |

### The Title's Four Layers

1. **Technical (stated explicitly by Carter, Beat 19):** Wax tablets are what the system has been — volatile, erasable, temporary. "The title is what we've left behind." The episode is a farewell to wax.

2. **Roman cultural (stated by Marcus, Beat 2):** The wax-to-stone transition is the Roman workflow for making things official. Drafts in wax, laws in stone. The team has been drafting. Now they're carving.

3. **Civilizational (stated by Marcus, Beat 19):** "A settlement without records is a camp. A settlement with records is a city." File I/O is what makes Nova Roma a city, not a camp.

4. **Philosophical (implied, Nightcrawler approach):** Writing things down is a realist act — declaring something worth preserving. The nominalism thread runs under the surface. The audience who has been tracking it since 1x03 sees it. Never named.

### Append-Only as Civilizational Principle

Marcus's Latin: *Civilitas quae appendit, meminit. Civilitas quae superscribit, obliviscitur.* This is the episode's thesis written on the board. Append-only logs are the most honest form of documentation — you can't cover up your mistakes because the mistakes are still in the file. You can only add corrections. This is how Joe works. This is how Pliny works. This is how civilizations persist.

### `FilumStatus` as Realist Philosophy

The `FilumStatus` struct is realist philosophy encoded in C89. The file knows what it IS. It knows its size, its type, its permissions. It has a nature that can be queried and that reports honestly. The file isn't whatever the system decides to call it. It HAS properties. `filum_status` is the "essence precedes existence" API.

---

## GENRE ARCHITECTURE

### The Dependency Graph Branches

This is the first time the dependency graph BRANCHES. Episodes 1x01–1x06 were roughly linear — each building on the last. 1x07 goes back to the Chorda/Piscina foundation and builds a DIFFERENT thing. Filum doesn't need the hash table or the internamentum. It shares only the common ancestors.

The stack is becoming a tree. The season architecture is developing the same structure as the code. The library and the market share the same streets but serve different purposes.

### SG-1 "The Torment of Tantalus" Resonance

Daniel lost an entire library's worth of alien knowledge because the building collapsed before they could write it down. When Carter says "Everything we've built lives in memory — if the machine stops, it's gone," Daniel hears Ernest's library. He already lived this problem.

*DIRECTION NOTE: Not dialogue. Direction note for the actor. Daniel's face during the cold open aftermath should carry the weight of someone who has been here before.*

### TNG "The Inner Light" / Kataan Parallel

The Kataan probe: an entire civilization that SAVED ITSELF TO A FILE and launched it into space. The Kataan people understood that volatile memory (living brains, a dying sun) was going to be destroyed, and they needed persistence. Data carries Picard's Katan memories. When Data says "File I/O is how the dead speak to the living," the Inner Light fans will feel the resonance.

### Kirk's Stinger as Dark File I/O

The team learns to go from analog to digital. Kirk maintains BOTH — and the physical files are the ones that matter because they can't be hacked. Same preservation instinct, opposite security model. The team builds a tabularium for knowledge. Kirk builds one for control.

Kirk's stinger progression: seed → germination → root → branches → organization → standardization → **persistence**. TABULARIUM → LEXICON → ARCHIVUM.

### The Watchers as File I/O

The Watcher Chronicles map directly to the filum API:
- `filum_scriptor_aperire` + `FILUM_MODUS_APPENDERE` = Opening a Chronicle, adding a new entry
- `filum_lector_lineam_proximam` = Reading through entries chronologically
- `filum_existit` = "Do we have a Chronicle on this Immortal?"
- `filum_error_recens` = "The field report was corrupted. Water damage."

Joe IS `FilumScriptor`. Pliny IS `FilumScriptor`. Two human persistence layers, same API, different file formats.

---

## CONTINUITY NOTES

### Backward Connections

**From 1x01 (Piscina):** All file I/O buffers allocated from the arena. The piscina now serves the persistence layer directly.

**From 1x02 (Credo):** Teal'c's testing methodology continues. His error test (Beat 12) and fifteen-test suite (Beat 18) carry the philosophy from 1x02: the test reveals a property the system should have.

**From 1x03 (Chorda):** The ChordaAedificator from 1x03 used in the streaming reader for long-line handling. The dependency chain in action — a tool built for assembling long strings from pieces now assembles long file lines from buffer-sized chunks. Data's line: "Reuse. A road built for military movement is later used for commerce."

**From 1x04 (Xar):** Census data stored in Xar arrays can now be saved and loaded. The data structures from 1x04 gain durability.

**From 1x05 (Tabula Dispersa):** The hash table that indexes the census and encyclopedia can be reconstructed from the saved flat files. Pliny's searchable encyclopedia survives shutdown. Kirk's whiteboard word "TABULARIUM" (1x05) is the same Roman institution the team is now digitally building.

**From 1x06 (Internamentum):** The interning table's 1,466 canonical words are saved to disk (Beat 16). The global singleton that O'Neill warned about ("One table. One point of failure") now has a backup. Carter writes "TABULAE CERATAE" below the "CANONICA" from 1x06 in her notebook — physical continuity of the paper trail.

**From 1x06 (Methos/Alexandria):** Daniel's fragments from Methos are now supplemented by Joe's Watcher records (Beat C4). Independent corroboration from 280 BC. The investigation gains a second source.

**From 1x06 (Carter/Midas):** Carter's temporal data moves from notebook to disk (Beat C2). The compass readings become persistent evidence. The oscillation pattern reveals a periodic frequency — not random, structured.

**From 1x05 (Methos fire warning):** Methos said "When the fire comes — and the fire always comes — what survives?" In 1x07, the team builds the first answer. And Methos adds to his own warning: "They didn't burn. They rotted."

### Forward Connections

**To 1x08 (Horologium / Tempus):** Carter's closing line: "We need timestamps." The frequency she discovered in C2 requires precise timing to analyze. But the clocks disagree. Casey's time library is motivated by making Carter's Filum data temporally coherent.

**To 1x09 (Arbor / XML):** Flat files are limited. Pliny's encyclopedia has structure — hierarchy, categories, relationships. Joe's Watcher records also have hierarchy (subject → chronicle → entries). The XML episode is motivated by the limitations of flat file I/O.

**To 1x13 (Aurum / Midas):** Joe opens a bar (established in season outline). His arrival in 1x07 and his bond with Pliny lay the groundwork. Daniel has two sources now — Methos's fragments and Watcher records. The investigation thread builds toward the Midas flashback.

**To 1x15 (TCP / Networking):** Methos's warning about single copies in single locations ("The Library burned because it was a single point of failure") motivates distribution and redundancy.

**To 1x17 (Quinque Milia Annorum / Methos flashback):** The Watcher records Joe shares with Daniel (Beat C4) are the foundation of Daniel's investigation. By 1x17, Daniel will have cross-referenced Methos's fragments with Watcher observations spanning centuries.

**To 1x18 (TLS / Encryption):** Carter's concern about findable secrets (C2 — saving temporal data to disk makes it discoverable) seeds the need for encryption and access control.

---

## THREAD STATUS APPENDIX: After 1x07

| Thread | Status After 1x07 | Next Beat |
|--------|-------------------|-----------|
| **Casey's Faith** | **RE-ENGAGED.** Grace before meal at Pliny's villa. O'Neill notices. "He prays over everything." "Huh." The observer circle expands to include the team commander. Lararium in frame — Roman mealtime religio as parallel. | 1x08-09: Rosary visible. Roman asks about beads. |
| **Kirk Stinger** | **7/31. ARCHIVUM.** Physical filing cabinets. MACLEOD, D. dossier. "Digital for operations. Paper for secrets." Kirk's persistence is analog BECAUSE digital is vulnerable — the inverse of the team's achievement. Kirk stinger progression: organization → standardization → **persistence**. TABULARIUM → LEXICON → ARCHIVUM. | 1x08: Kirk's stinger mirrors the time library. |
| **C-Plot (Kirk/Earth)** | **ACTIVE.** O'Neill briefs Joe on Kirk's network (Beat C1). Joe's Watcher intelligence — decades of tracking covert Immortal networks — makes him O'Neill's most useful new intelligence asset. | Ongoing. Joe's intelligence contributions build through mid-season. |
| **Joe Dawson** | **INTRODUCED.** Arrives through gate. Duncan vouches, O'Neill is wary. Pliny connection immediate and deep. Daniel gets Watcher corroboration of Methos/Alexandria. Joe brings selected Chronicles — his personal field journals, the Methos Chronicle (portions), and key records for known Nova Roma Immortals. | 1x08-12: Joe settles in. Working sessions with Pliny. By 1x13: bar opens. |
| **Joe/Pliny Partnership** | **NEW THREAD.** Two documentarians. Different methodologies (observer vs. participant). Deep mutual recognition. Pliny challenges Joe's non-interference philosophy ("A mirror vs. a lens"). Joe begins transformation from observer to participant. | 1x08-12: Working sessions. Complementary institutions (bar for flow, archive for structure). |
| **Black Stone / Cybele** | **SUPPLEMENTED.** Joe's Watcher records provide independent corroboration: 280 BC observer saw Methos flee Serapeum after visiting lower levels. "First recorded instance of the subject voluntarily leaving a center of learning." Daniel now has two sources. Investigation gains evidentiary weight. | 1x09 (XML): Daniel working with structured ancient texts. 1x13/1x17: Thread pays off. |
| **Midas Temporal** | **FREQUENCY DETECTED.** Carter saves temporal data to disk (C2). With persistent, continuous data, she discovers the oscillation has a periodic frequency — not random, structured. This is a signal, not noise. | 1x08: Carter needs Casey's time library for proper frequency analysis. |
| **Nominalism / Realism** | **PERSISTENCE AS ONTOLOGY.** In memory = contingent existence (nominalist). On disk = independent existence (realist). Wax tablets (overwrite) vs. stone inscriptions (endure). Joe's Watcher philosophy (observe/don't engage = nominalist) vs. Pliny's scholarship (engage/participate = realist). Kirk persists for control (nominalist). Team persists for truth (realist). | Ongoing. Each library adds depth. |
| **Pliny's Mortality** | **DEEPENING.** "The eruption could not have taken this." His life's work can now outlast him — not just the machine but the session. Digital persistence mirrors his papyrus. Every persistence operation Pliny witnesses is a small victory against death. | Ongoing. By 3x20-21, this thread pays off. |
| **Data/Midas Friendship** | **CONTINUING.** Data feels relief at no longer being the sole storage medium. "I was never comfortable being essential to the preservation of knowledge." The friendship continues from 1x06's "external verification." | Ongoing. The file Data keeps on Midas will matter. |
| **Daniel as Investigator** | **ESCALATED.** Two sources: Methos's fragments (1x06) + Watcher records (1x07 C4). Investigation saved to disk. Persistent working files. The dossier is real and growing. | 1x09: Structured text work. 1x13/1x17: Thread pays off. |
| **Methos's Secret** | **BACKGROUNDED.** Methos present at dinner, does not seek out Joe. The man hiding from the Watchers sitting with his Watcher. The eye-contact beat across the table — mutual recognition, decades of history, no words. Methos's Alexandria/copies warning in Beat 15 adds to his fire warnings. | Deferred Joe/Methos reunion: 1x09 or 1x10. Private. Late night. Wine. |
| **Carter's Secret Burden** | **STATE CHANGE: Ephemeral → Persistent.** The compass data is on disk. Files can be found. The burden solidified. Relief (it won't be lost) and risk (it can be found) in equal measure. | 1x08: Carter needs timestamps. The persistent file demands temporal coherence. |

---

## ADDITIONAL NOTES

### Production Design — The Stylus

Marcus's stylus should appear in Beat 2 and be referenced throughout. Iron or bronze. Pointed at one end for inscribing; flat and spatulate at the other for smoothing. The dual function is the episode's visual motif: write on one end, erase on the other. *Stilum vertere* — Cicero's word for revision, embodied in a tool.

### Production Design — Pliny's Villa

Key elements for scenes set at Pliny's villa:
- The *lararium* (household shrine) — in frame during the grace scene (Beat C3). Figurines, flame, offerings.
- The *armaria* (scroll cabinets) — where Pliny keeps his manuscripts. Joe should touch these with the reverence of a fellow archivist.
- The *adversaria* — Pliny's personal notebooks. The parallel to Joe's leather journal. Two sets of field notes, two thousand years apart.
- Simple dinner setting — Pliny the frugal scholar, not the lavish senator. The conversation is the entertainment.

### Performance Notes

**Joe (casting: warm, blues-inflected, bartender's timing):** Joe tells stories the way a bartender tells stories — with timing, with eye contact, with the glass moving in his hand. He doesn't lecture. He anecdotes. When he makes a point, it lands sideways, through a story about something else. His leaking-roof story should feel like a man telling you about a friend who died — the pauses carry the weight.

**Pliny (Beat 17):** "The eruption could not have taken this." Delivered quietly. Not a dramatic declaration — a private realization. The relief of a man who has been carrying this weight since he died.

**O'Neill (Beat 5):** "...That would have been useful." Understated. The ellipsis is acted, not written — a moment before speaking where the audience sees O'Neill remembering something he doesn't name. Carter hears it. She knows.

**Marcus (Beat 19):** "Now we are becoming a city." This is the climax of Marcus's arc since 1x04. He should deliver it with genuine emotion — a Roman engineer who has been watching these strangers build, and who now recognizes what they've built as *Roman*.

### Historical Grounding

- The wax tablet construction, the stylus's dual function, and the wax/stone legislative workflow are all historically attested (Quintilian, Cicero, CIL).
- The *codex* etymology (block of wood → tablet bundle → bound book → code) is real.
- Roman mealtime *religio* and the *lararium* are well-documented (Cato, *De Agri Cultura* 143).
- Pliny's *adversaria* (personal notebooks, inherited by Pliny the Younger) are mentioned in *Epistulae* 3.5.
- The Serapeum underground chambers, the Ptolemaic ship-search policy (Galen source), and the Watchers' 280 BC observation — the first two are historically attested; the third is fictional but sits within a real cultural matrix.
- The *acta diurna* (Rome's daily gazette, established by Caesar 59 BC) is the historical precedent for append-only public logs.
- The survival paradox (disposable pottery survives millennia, precious papyrus rots) is one of archaeology's fundamental observations.

---

*Compiled from A-Plot Technical Pitch, Character & Flashback Pitch, Serialized Threads Pitch, Genre Fan Pitch, and Historian Pitch per the Compilation Plan decisions document. All open questions resolved per showrunner approval.*
