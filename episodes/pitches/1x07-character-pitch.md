# 1x07 CHARACTER PITCH: "Tabulae Ceratae" (Wax Tablets)
## Filum — File I/O — Persistence

---

## THE EMOTIONAL THESIS

This episode is about the difference between *remembering* and *recording*. Every character on this show carries memory — some carry centuries of it. But memory dies with the person. Recording outlives them. File I/O is the moment the show's technology stops being clever and starts being *permanent*. What was volatile becomes durable. What lived only in the present now has a future.

The emotional question: **What do you save, and for whom?**

---

## 1. JOE DAWSON — "The Man Who Writes Things Down"

### Arrival

Joe comes through the gate with a small group of Earth personnel — logistics, communications, a couple of engineers O'Neill requisitioned. He's listed as "cultural liaison." He walks with prosthetic legs, carries a guitar case and a leather satchel. He's the last one through, and he pauses at the gate, looking back — not at Earth, but at the event horizon. A beat. He's crossing a threshold he can't uncross.

First person he sees on the other side: Duncan.

**JOE:** *(looking around at Nova Roma, the Forum, the Romans, the villa complex)* You could've mentioned the Romans.

**DUNCAN:** I did mention the Romans.

**JOE:** You said "a colony." You said "some locals." *(gestures at the Forum Stellarum, the senate building, the three thousand citizens going about their day)* Duncan, this is a CIVILIZATION.

**DUNCAN:** *(the smallest smile)* Yeah. That's why I asked you to come.

### Who Joe Is

Joe is warm where Methos is cold. Grounded where Data is precise. Tired where Casey is patient. He's a man who has spent his entire adult life watching other people do extraordinary things and writing it down. Not because he's passive — because he believes the record is the thing that matters. Without the record, the extraordinary thing happened to no one.

He lost his legs in Vietnam. He doesn't talk about it much. When he does, it's matter-of-fact. He found the Watchers afterward — a secret society that observes and records Immortals without interfering. The Watchers gave him purpose: you can't run anymore, but you can *watch*, and watching is enough.

Except it was never enough for Joe. He broke the rules. He befriended Duncan. He warned Immortals when their lives were in danger. He intervened. The Watchers expelled him twice and took him back both times because his chronicles were too good to lose.

**Joe's voice:** Blues-inflected, wry, tired-warm. He tells stories the way a bartender tells stories — with timing, with eye contact, with the glass moving in his hand. He doesn't lecture. He anecdotes. When he makes a point, it lands sideways, through a story about something else.

### Joe and the File I/O Theme

Joe IS the human version of `filum_scribere`. His entire career: observe, record, persist. The Watcher Chronicles are a centuries-long file system — generations of observers writing to the same archive, reading each other's entries, appending new observations to existing records.

But Joe also knows the failures of persistence. Archives that were lost to fire. Chronicles that were falsified. Watchers who stopped watching and started writing fiction. The record is only as good as the recorder, and recorders are human.

**His arrival coincides with the persistence episode because Joe's first question on Nova Roma will be the same question he's been asking his whole life: "Who's keeping the record?"**

---

## 2. THE JOE/PLINY SCENE — "Two Men Who Write Things Down"

### The Setup

This is the B-plot's emotional center. It should happen in Act Two or early Act Three — after Joe has been introduced but before the technical A-plot reaches its climax.

Pliny is in his villa, working on the encyclopedia. The hash-table-indexed, interned-string-optimized encyclopedia — the one the team spent six episodes building infrastructure to support. He's categorizing new entries. It's late afternoon. The light is golden. He has wine.

Joe is exploring. Duncan gave him the tour, but Joe doesn't absorb things through tours. He absorbs them through conversation. He wanders into Pliny's villa because he heard there was a scholar who'd written thirty-seven volumes about everything. Joe has spent his life around men who've done improbable things. He recognizes the type.

### The Scene

Joe enters. Sees the villa — scrolls, armaria, wax tablets, the laptop with the census and encyclopedia databases running side by side. He stands in the doorway.

**JOE:** They told me you wrote an encyclopedia.

**PLINY:** *(not looking up)* Thirty-seven volumes. Forty years.

**JOE:** *(walking in, looking at the shelves)* I kept chronicles. Thirty-one years. Different subjects, same problem.

Now Pliny looks up. He sees an older man — not Roman, clearly — who walks with difficulty and carries himself with the specific gravity of someone who has spent decades paying attention. Pliny recognizes the posture. The attention. The eyes that are always cataloging.

**PLINY:** What did you chronicle?

**JOE:** People. Specific people. Their lives, their fights, their choices. Everything they did and everyone they met.

**PLINY:** How many?

**JOE:** Personally? One primary subject for twenty years. But the organization I worked for — we had thousands of chroniclers, going back centuries. Hundreds of subjects.

Pliny sets down his stylus. This has his full attention.

**PLINY:** An organization devoted to observation?

**JOE:** Devoted to the record. We watched. We wrote it down. We weren't supposed to interfere. Just... bear witness.

**PLINY:** *(with genuine reverence)* You are describing my life's ambition as an institution.

**JOE:** *(a tired laugh)* Yeah. It was messy. Lots of politics. People breaking the rules, rewriting the chronicles, losing whole archives because someone forgot to maintain the building —

**PLINY:** *(leaning forward)* You lost archives?

**JOE:** We lost archives. Not to fire. Not to war. To neglect. A roof leaked for a decade. Twenty years of records, water-damaged beyond recovery. Nobody attacked it. We just... stopped maintaining it.

Pliny stands. He walks to his armaria — the wooden cabinets holding his scrolls. He touches one.

**PLINY:** When Vesuvius erupted, I lost everything I had written in that house. Forty years of notes. Draft manuscripts. Letters from every naturalist in the provinces. *(beat)* The library in Alexandria burned. The library in my study burned. I have come to believe that the default state of knowledge is *not existing*. Preservation is the aberration. Loss is the norm.

**JOE:** *(quiet, recognizing something profound)* That's... yeah. That's exactly it.

**PLINY:** What survived of your chronicles?

**JOE:** The originals, some of them. Copies. The organization kept redundant archives in different cities. But you always worry about the one copy you don't have a backup for.

**PLINY:** *(a long look at the laptop, at the encyclopedia index running on screen)* They are teaching us to save our work to a device. Carter says it stores the information even when the machine stops.

**JOE:** *(understanding immediately)* Persistent storage. Writing to disk.

**PLINY:** *(nodding)* My encyclopedia exists in two places now — in scrolls, and in their machine's memory. But the memory is volatile. If the machine stops, the indexed version is gone.

**JOE:** Then you need to write it to something that doesn't forget when the power goes out.

**PLINY:** That is what today's lesson is about, I believe.

**JOE:** *(sitting down, pouring himself wine without asking — a bartender's instinct)* Then I showed up on the right day.

### What This Scene Establishes

1. **The handshake.** Two documentarians recognizing each other across two thousand years. They don't need to explain themselves. They share a vocation.

2. **The theme of negligence over destruction.** Joe's Watcher archive wasn't burned — it was neglected. This is the nominalism thread in miniature: indifference to preservation is a symptom of believing nothing has intrinsic worth. If knowledge is just data, why maintain the building?

3. **Pliny's vulnerability.** He's lost everything before. Vesuvius. The temporal displacement. The scrolls left behind. File I/O isn't technical to Pliny — it's *existential*. It's the promise that this time, the knowledge survives.

4. **Joe's credibility.** He arrives on Nova Roma and immediately has something to contribute. He's not a tourist. He's a professional recorder who understands the stakes of persistence because he's lived the consequences of its failure.

---

## 3. CASEY'S GRACE — "He Prays Over Everything"

### The Scene

Evening. Pliny's villa. A meal — not a feast, but a proper dinner. Pliny has invited the team. It's a Roman meal: reclined seating for the Romans, chairs brought in for the Earth people. Bread, olives, wine, roasted vegetables, a stew. Pliny is hosting because the encyclopedia is about to be saved to disk for the first time, and in Roman tradition, you mark the eve of something significant with a meal.

Joe is there — his first night on Nova Roma. He's charming the Romans already, telling stories, pouring wine. He's a natural at this. A bartender in any century.

The food arrives. People begin eating. Casey, seated between O'Neill and Marcus, pauses. He bows his head. Brief. Five seconds. He crosses himself. He picks up his bread.

O'Neill, across the table, sees it. His fork — or Roman equivalent — stops midway. Not dramatically. Just... a pause. O'Neill files things. He's been doing it all season: the singleton risk, the strategic vulnerability, Kirk's pattern. He files this too.

Casey doesn't notice O'Neill noticing. He's already eating, talking to Marcus about brick composition.

### The Follow-Up

Later. After the meal. O'Neill and Duncan walking back to the quarters. Stars overhead. Nova Roma is quiet.

**O'NEILL:** Did Casey just pray over Roman food?

**DUNCAN:** He prays over everything.

**O'NEILL:** Huh.

That's it. Three lines. No judgment from O'Neill. No explanation from Duncan. Just the commander of the mission noticing something about one of his people and filing it alongside everything else he files.

### Why This Matters for the Season Arc

The faith thread has been climbing through observers of increasing authority:

- **1x01:** Nobody. Just the camera.
- **1x02:** The camera, via crucifix placement.
- **1x03-04:** Marcus notices the rosary beads. A colleague.
- **1x05:** Pliny asks about the crucifix. A respected elder. Two faith beats — the factual ("A man who died for everyone") and the personal ("The God who remembers everything").
- **1x06:** REST. Crucifix in frame. No dialogue.
- **1x07:** O'Neill notices the grace. The commanding officer. The man who makes personnel decisions.

Each observer represents a higher level of institutional attention. Marcus is a peer. Pliny is a mentor. O'Neill is the authority. The faith thread is climbing not toward confrontation but toward *visibility* — the quiet practice becoming part of the team's shared awareness.

O'Neill's "Huh" is NOT dismissive. It's O'Neill's processing sound. He said "Huh" about the first time he saw an alien weapon work. He said "Huh" when he realized Kirk was building a network. O'Neill files things and acts on them later. The audience doesn't know yet what he'll do with this. That's the point.

### The Specific Dramatic Weight

Casey is praying at PLINY'S table. The same Pliny who asked about the crucifix in 1x05. The same villa where they had the "God who remembers everything" conversation. Casey's faith is now happening *in the space where it's been observed* — Pliny's domain. And O'Neill is watching. The geography of the scene matters: Casey's prayer is at the intersection of the scholar who's cataloging it and the commander who's filing it.

---

## 4. O'NEILL — "The Man Who Notices"

O'Neill has two roles this episode, and they're both the same role wearing different clothes.

### Strategic O'Neill

O'Neill sees file I/O as a military operation: backup, redundancy, survival planning. When Carter explains that everything they've built lives in memory and can be lost to a power failure, O'Neill's response is immediate:

**O'NEILL:** You're telling me six episodes of work disappears if someone trips over the power cable?

**CARTER:** Technically, yes.

**O'NEILL:** Then that's the priority. Everything else stops until we can save our work.

This is O'Neill at his best — cutting through technical abstraction to the operational reality. He doesn't care about file handles or buffer sizes. He cares that the mission's infrastructure has a single point of failure that can be triggered by an accident.

He'll also connect file I/O to intelligence: writing to disk means the information can be *found*. Carter's secrets (compass data, Midas temporal file) become more dangerous once they're persistent. If someone finds the disk, they find the secrets.

**O'NEILL:** *(to Carter, quiet)* Before you save anything — think about who might read it.

### Observer O'Neill

The grace scene. O'Neill watching Casey. The "Huh."

But also: O'Neill watching Joe and Pliny bond. O'Neill watching Methos react to the idea of persistent storage. O'Neill is the episode's observer-in-chief — the man whose superpower is *noticing* and whose discipline is *not reacting yet*.

He notices Casey's prayer the same way he notices the strategic vulnerability. Both are data points. Both get filed. Both will inform decisions later.

---

## 5. CARTER — "The Weight of What You Save"

Carter teased file I/O at the end of 1x06: "Everything lives in memory. Next: file I/O." She's been carrying this technical need since the cold open of 1x01 — the entire infrastructure has been volatile. Now she builds the bridge to permanence.

But Carter is also carrying secrets. The compass data. The Midas temporal anomaly file. The oscillation patterns she's been tracking since 1x05. These have lived in her head and in volatile memory. Now she can *save them to disk*.

### The Weight

Saving to disk changes secrets. A secret in your head is private. A secret in volatile memory is temporary. A secret on disk is *evidence*. It persists. It can be found. It can be copied.

Carter should have a moment — quiet, alone — where she sits in front of the file I/O interface, cursor blinking, and she has to decide: does she save the compass data? Does she write the temporal anomaly log to a file? Because once she does, it's *there*. It survives her, survives the session, survives a power failure. If someone finds it, the secret is no longer hers to control.

**CARTER:** *(to herself, staring at the screen)* Save to disk. *(beat)* Permanent. *(beat)* Findable.

She saves it. She creates a directory — `anomalia_temporalis` — and writes the files. Then she sets permissions. Then she thinks about encryption (which they don't have yet — that's TLS in 1x18). Then she creates a second copy in a different location.

Carter the scientist wants the data to survive. Carter the keeper of secrets wants to control who reads it. File I/O puts these two impulses in direct tension.

### The Casey Exchange (Carried Forward)

The 1x06 exchange — "What truth are you carrying?" / "Too many" — reverberates here. Casey noticed Carter's weight. Now Carter is materializing that weight on disk. The audience should feel the gravity: every `filum_scribere` call Carter makes for her secret data is a choice to make the secret more real.

---

## 6. METHOS — "A Disk Can Burn Too"

Methos has been warning about fire since 1x05. "When the fire comes, what survives?" The compass reaction, the Alexandria revelation in 1x06, the sub-chamber — all leading to THIS question: what survives the disaster?

File I/O is the first answer the team has built. And Methos's reaction should be... complicated.

### Relief and Skepticism

**METHOS:** *(watching Carter demonstrate file writes)* So now it doesn't disappear when the machine stops.

**CARTER:** Now it persists.

**METHOS:** Until the disk fails. Or the building burns. Or the invaders smash the hardware.

**CARTER:** That's why we make copies.

**METHOS:** *(a long pause, drinking)* I watched the Library of Alexandria burn. They had copies too. Copies in the Serapeum, copies in the daughter library at Pergamum, copies on the ships that came and went. Do you know what happened to all the copies?

**CARTER:** They burned too.

**METHOS:** They didn't burn. Most of them rotted. Decades of neglect. Regime changes where nobody cared about old scrolls. Institutional memory that nobody maintained because there was always something more urgent. The fire was the dramatic end. The real end was centuries of indifference.

This parallels Joe's leaking-roof story. Two characters, from opposite ends of history, arriving at the same conclusion: the enemy of persistence isn't destruction. It's neglect. This IS the nominalism thread: if nothing has intrinsic worth, why maintain anything?

### Methos's Personal Relationship to Persistence

Methos has spent five thousand years NOT persisting. His survival strategy is the opposite of file I/O — he writes nothing down, keeps no records, maintains no fixed identity. "Adam Pierson" was a persona. Forty-seven names, each one deleted when it was no longer useful.

File I/O threatens Methos's model. A system that persists everything is a system where the past can always be found. For a man who has spent five millennia erasing his trail, that's terrifying.

**METHOS:** *(to nobody in particular, late in the episode)* The problem with writing things down is that someone eventually reads them.

---

## 7. DATA AND MIDAS — "The Machine Remembers"

### Data's Relationship to Persistence

Data has always been "the one who remembers." His positronic brain is the most reliable storage medium on Nova Roma — perfect recall, no degradation, no emotional distortion. But Data's memory is *his*. It lives in his head. It cannot be copied (Lal proved that). It cannot be backed up.

Now the machine can remember too. File I/O is the first time Data's monopoly on perfect recall is challenged — not by a person, but by a technology. And Data should find this... liberating.

**DATA:** *(to Midas, during a quiet moment)* When I was the only reliable storage medium, I felt a weight. If I ceased to function, the records ceased to exist. Now the records can survive independently of me.

**MIDAS:** Does that bother you?

**DATA:** *(after processing)* It relieves me. I was never comfortable being essential to the preservation of knowledge. Essentiality implies fragility. If I am essential, then the loss of me is catastrophic. I prefer to be... supplementary.

**MIDAS:** *(gently)* You can't be supplementary, Data. You're my friend.

**DATA:** Friendship and archival function are different categories.

**MIDAS:** Are they?

### The Friendship Continues

Their 1x06 bond — the "uninternable" scene, the "friendship is external verification" callback — continues naturally. They're comfortable together now. Two anomalies who've found their consistency check in each other. The eye-color observation from 1x06 C4 doesn't need to be referenced directly, but Data is subtly more attuned to Midas, watching him the way you watch someone you're responsible for.

---

## 8. DUNCAN — "Joe's Oldest Friend"

Duncan and Joe's relationship is the longest, most complicated friendship in the show after Methos. Joe watched Duncan for the Watchers. They became friends despite the rules — a friendship that nearly destroyed both of them multiple times. Joe was expelled from the Watchers for his loyalty to Duncan. Duncan nearly killed a Watcher to protect Joe.

### The Dynamic on Nova Roma

Duncan is the one who arranged for Joe to come to Nova Roma. The official reason: Joe's expertise as a chronicler, his connections, his understanding of Immortal history. The real reason: Duncan missed his friend.

**DUNCAN:** *(walking Joe through the Forum, pointing things out)* Marcus designed the census infrastructure. Carter built the hash tables. Pliny's encyclopedia is fully indexed now.

**JOE:** *(looking around, taking it all in)* And you?

**DUNCAN:** I hit things with a sword when they need hitting.

**JOE:** *(amused)* Some things don't change.

**DUNCAN:** Some things do. *(a beat, genuinely)* I'm learning to build, Joe. I spent four hundred years fighting. Here, I'm learning what happens after you win.

**JOE:** *(quietly)* That's the hard part, Mac. Winning is dramatic. Building is work.

This exchange grounds Duncan's season arc. He's been the pattern-reader in the C-plot, the strategic mind who sees Kirk's moves. But his deeper arc is the one seeded in 1x04's cathedral flashback: a man who always left before things were finished, now staying to build. Joe's arrival forces him to articulate this because Joe is the one person who's known him long enough to notice the change.

### Joe and Duncan's Shorthand

They have decades of shared history. They don't need to explain themselves. Joe can say "Mac" and Duncan hears everything — the friend, the Watcher, the man who lost his legs in a war Duncan was too old to fight in and too young-looking to enlist for. When they talk, it's economical. Old friends don't over-explain.

**JOE:** *(at the dinner, watching the team work)* Good people.

**DUNCAN:** The best.

**JOE:** *(a nod toward Methos)* He tell you about Alexandria yet?

**DUNCAN:** *(a look)* Some of it.

**JOE:** *(a knowing silence)* Yeah. "Some of it" is all you ever get from him.

---

## 9. THE ENSEMBLE AND PERSISTENCE

Each character's relationship to the concept of saving/persistence:

| Character | What They Want to Persist | What They Fear About Persistence |
|---|---|---|
| **Joe** | The chronicle — Immortal history, human history, the record | That records can be falsified, neglected, or used against those they describe |
| **Pliny** | His encyclopedia — everything he's ever observed | That he's running out of time before it's all recorded (mortality thread) |
| **Carter** | Her research — and her secrets | That saving secrets makes them findable |
| **Casey** | Nothing specific — he trusts the God who remembers | That others don't have that trust and need the technology to compensate |
| **O'Neill** | Mission-critical data — operational survival | That everything saved is also everything exposed to the enemy |
| **Methos** | Nothing — he survives by NOT persisting | That persistence makes the past discoverable, and his past is dangerous |
| **Data** | Everything — he's a compulsive recorder | That being the sole reliable record makes him catastrophically essential |
| **Midas** | His friendships, his garden, his present | Nothing — Midas is the most present-tense character on the show |
| **Duncan** | The memory of people he's loved | That memory fades even for Immortals, and the written record is a poor substitute for the living person |
| **Marcus** | Roman engineering standards, methods, principles | That the written specification can be misapplied by someone who doesn't understand the context |

### The Wax Tablets vs. Stone Inscriptions

The episode title — "Tabulae Ceratae" — is the conceptual bridge. Wax tablets are the Roman temporary medium: you write, you smooth the wax, you write again. Overwrite. `FILUM_MODUS_CREARE`. Stone and bronze inscriptions are permanent: once carved, they stay. `FILUM_MODUS_APPENDERE`.

The team is building both. Carter's overwrite mode is the wax tablet — create or replace, start fresh. Carter's append mode is the inscription — add to what exists, preserve the history.

The characters map to this:
- **Methos** is wax. He overwrites. Forty-seven names, each smoothed away.
- **Pliny** is stone. He appends. Thirty-seven volumes, each building on the last.
- **Joe** is both. He writes chronicles (append) but has seen them rewritten (overwrite by falsifiers).
- **Casey** trusts a recorder who never overwrites and never loses an entry.

---

## 10. EMOTIONAL THROUGHLINE — "What the Episode Is About"

At its heart, 1x07 is about the transition from **living in the present** to **building for the future**.

For six episodes, the team has been doing brilliant, real-time work — building arenas, testing frameworks, writing strings, growing arrays, hashing names, interning words. All of it volatile. All of it contingent on the machine staying on. They've been sprinting and assuming the ground will hold.

File I/O is the moment they stop sprinting and start *building foundations that outlast them*.

Pliny feels this most acutely. He's mortal. He's eighty. Everything he builds is a race against his own death — and he knows it, because he died once before, on a beach in Misenum, choking on volcanic gas. The temporal displacement saved his body but not his scrolls. Now he has the scrolls back (rebuilt, re-indexed, re-optimized), and he has the chance to save them in a way that doesn't depend on any one person, any one building, any one eruption.

Joe's arrival amplifies this. Joe is the man who has spent his life doing what Pliny dreams of — running a persistence system at institutional scale. And Joe has learned the hard way that persistence fails not because of enemies but because of *neglect*. The record dies when nobody maintains the building.

The emotional climax isn't technical. It's the moment Pliny saves his encyclopedia to disk for the first time and knows — KNOWS — that even if the machine stops, even if he dies, the knowledge survives. It's the moment Casey says grace over Roman food and O'Neill notices, filing one more data point about the man who prays over everything. It's the moment Carter saves her secret files and realizes she's committed them to a future she can't fully control.

**What do you save, and for whom?**

Pliny saves for everyone who comes after him. Joe saves because he was trained that the record matters more than the recorder. Carter saves because the data demands it, even though persistence makes her secrets more dangerous. Methos saves nothing, because five thousand years have taught him that everything written down is eventually used against you. Casey doesn't need to save, because he trusts a memory that never fails — but he understands why the others do, and he helps them build the tools.

And Data saves because for the first time, saving means he's not the only copy.

---

## OPEN QUESTIONS FOR THE COMPILATION

1. **Joe's cover story depth.** How much does the team know about the Watchers at this point? Duncan knows everything. Casey knows (he's Immortal, he'd know about Watchers). Does O'Neill know? Does Carter? Joe's arrival creates a knowledge asymmetry that could be dramatic or could be background. I lean toward: O'Neill knows Joe is a Watcher (he arranged the trip), most of the team doesn't, and it doesn't become an issue this episode. Joe is just "Duncan's friend who keeps records."

2. **Joe/Methos.** Joe and Methos know each other from the Highlander timeline. Their dynamic is complicated — Methos was Joe's biggest secret (the oldest living Immortal, hidden in plain sight as a Watcher researcher). Do they have a scene together in 1x07, or does that get saved for a later episode? I'd suggest: they SEE each other, there's a LOOK — mutual recognition, decades of history in a glance — but the conversation happens later. 1x07 is already introducing Joe and building Joe/Pliny. Joe/Methos is too big a scene to share real estate.

3. **Joe's bar.** The season outline says Joe opens a bar in 1x13. Does he scout the location in 1x07? Does he mention wanting to set up a bar? A small seed — Joe looking at a space in the Forum, Pliny asking what a "bar" is, Joe describing it — would be a nice forward connection.

4. **How much Watcher Chronicle material Joe brings.** Does Joe arrive with physical or digital records? Watcher archives on a laptop? Handwritten journals? The answer affects how he interacts with the file I/O theme. If he's brought records, they need to be saved to Nova Roma's system — a literal act of file I/O, importing one archive into another.

5. **Joe and the file I/O A-plot.** How directly does Joe participate in building the filum library, versus being a B-plot character who resonates thematically? I lean toward: Joe doesn't write code, but his presence and his Watcher archival experience INFORMS the design. He describes how the Chronicles are organized (by subject, by date, with cross-references — essentially a file system). Carter or Casey hears this and incorporates it.
