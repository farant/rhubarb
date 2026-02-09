# NOVUM FUNDAMENTUM
## Episode 1x08: "Horologium" (The Clock)
### Beat Sheet (Draft 1 — Compiled from Writers' Room)

---

**Library:** Tempus (Time management — clocks, frame timing, easing, waves, interpolation, utilities)
**Dependencies:** Piscina (1x01)
**Theme:** Precision vs. truth. A clock that lies about a strange reality is less precise than a clock that honestly reports it. Building something beautiful on ground that shifts.
**Dramatic Engine:** Casey builds an elegant 47-function time library. The clocks disagree. Not a bug — Nova Roma's spacetime is sinusoidal. The craftsman who has been alone for thirty years because the world stopped building things properly discovers a universe that isn't proper, either. And he accepts it.

---

## COLD OPEN

**Location:** Casey's workshop. Before dawn. The room is spare — tools organized with military precision. A single terminal. Casey's crucifix on the wall (established 1x02). His rosary on the desk beside the keyboard.

Casey is writing code. Not for the team. For himself. He does this — gets up early, works alone, builds the next thing before anyone asks. It's who he is. The man who has been building alone for thirty years doesn't stop just because he has a team now.

On screen:

```c
/* Tempus — horologium computationale */
tempus_initiare();

f64 nunc = tempus_nunc();
imprimere("Tempus currentis: %f secundae\n", nunc);
```

The output appears: `Tempus currentis: 0.000043 secundae`. Forty-three microseconds since initialization. The clock is running.

Casey nods. Types more:

```c
/* Creare horologium independens */
Tempus* horologium = tempus_creare(piscina);

dum (tempus_elapsum(horologium) < 1.0)
{
    /* nihil */
}

f64 elapsum = tempus_elapsum(horologium);
imprimere("Elapsum: %f secundae\n", elapsum);
```

`Elapsum: 1.000017 secundae`. One second, plus seventeen microseconds of overhead. Precise. Clean. Casey allows himself the smallest smile.

He writes a comment:

```c
/* Horologium purum. Sicut debet esse. */
/* A clean clock. As it should be. */
```

**A 5-second visual fragment:** Casey's hands on the keyboard — CUT TO — Casey's hands on cobbler's leather, an older world, amber light, a workshop that isn't this one — CUT BACK. Casey blinks. A memory. Just a flash. The audience sees: there's more.

He opens a new file. Types the header:

```c
/* TEMPUS — Bibliotheca Temporis
 *
 * Horologium globale, horologia independentia,
 * administratio quadrorum, functiones lenitionis,
 * interpolatio, undae, utilitas.
 *
 * Casey scripsit. Ad usum Nova Roma.
 */
```

*Casey wrote this. For the use of Nova Roma.*

The first time we've seen Casey put his name on something. The hermit claims his work. Small moment. Big shift.

SMASH TO MAIN TITLES.

---

## ACT ONE: "Pulsus" (The Pulse)

*The need for time. Casey presents the library. The first frame loop. Easing functions as philosophy. Spring physics.*

### Beat 1: Carter's Timestamp Request

**Location:** The Tabularium. Morning. Carter has the timestamp problem on the board:

```
Census salvatus: 187,433 bytes
  — Quando? (When?)

Encyclopaedia salvata: 2,341,872 bytes
  — Quando?

Vocabularium salvatum: 18,291 bytes
  — Quando?
```

**CARTER:** Everything we saved yesterday has no date. The census file exists on disk. When was it written? We don't know. When was it last modified? We don't know. If we save it again tomorrow, which version is newer? *(beat)* We can't tell.

**MARCUS:** *(immediately)* A record without a date is evidence without a witness.

**CARTER:** We need a clock. A precise, reliable, programmable clock that the entire system can reference. *(she looks at Casey)* Casey, you've been building timing systems since before most of us were born.

**CASEY:** *(standing, walking to the board — spare, economical, a man who doesn't waste movement)* I built my first clock subsystem at IBM in 1957. Crystal oscillator, counted electronic pulses. Accurate to one part in ten million. *(beat)* That was sixty-seven years ago. The principle hasn't changed.

He erases Carter's board. Writes:

```
TEMPUS — Bibliotheca Temporis

1. Horologium Globale   — one clock, shared by all
2. Horologia Independentia — private clocks, per task
3. Administratio Quadrorum — frame rate management
4. Functiones Lenitionis — smooth motion
5. Interpolatio — in-between values
6. Undae — periodic oscillation
7. Utilitas — clamp, wrap, remap
```

**CASEY:** Seven sections. The global clock gives everyone the same answer to "what time is it?" Independent clocks let you measure duration without affecting the shared clock. Frame management keeps the system running at a steady rhythm. The rest is for animation — making things move smoothly.

**MARCUS:** *(studying the list)* Animation? We need timestamps, not moving pictures.

**CASEY:** You need both. Timestamps tell you *when*. Animation tells you *how fast*. A clock that can only report the time but can't pace a process is like a sundial that can't cast a shadow — it knows the answer but can't show you.

**Who:** Carter, Marcus, Casey, full team.

### Beat C1 (Background, Act 1): Earth-Side Chyron

A laptop with Earth news feeds visible in the workshop background — supply personnel from 1x07 have established a small information pipeline. A news crawl visible on screen:

**CHYRON:** *"TECH GIANT ANNOUNCES 'SMART TOWN' PILOT PROGRAM IN NEW HAMPSHIRE — $14M INVESTMENT IN MUNICIPAL AI"*

Nobody comments. Nobody reads it aloud. O'Neill might glance at it. The camera holds on it for two seconds.

**Why:** Seeds the New Hampshire thread. In S2, this is the project Big Head runs. In S4-5, it becomes the first deployment of clean infrastructure.

**Timing:** 3 seconds of screen time.

### Beat 2: The First Frame Loop

Casey builds the frame loop live:

```c
/* Initiare tempus et ponere rhythmum */
tempus_initiare();
tempus_ponere_quadra_destinata(60.0);

dum (VERUM)
{
    /* Actualizare horologium */
    tempus_quadrum();

    /* Obtinere intervallum */
    f64 delta = tempus_delta();

    /* ... computationes ... */

    /* Exspectare proximum quadrum */
    tempus_exspectare_quadrum();
}
```

**CASEY:** Sixty frames per second. That's the target. `tempus_quadrum()` marks the beginning of each frame — it measures how long the last frame took. `tempus_delta()` gives you that measurement. `tempus_exspectare_quadrum()` sleeps until it's time for the next frame.

**DATA:** *(precise)* The delta time decouples computation from frame rate. If the system slows, delta increases. If the system accelerates, delta decreases. The same code produces the same behavior regardless of frame rate. *(beat)* It is the separation of logic from clock speed.

**CASEY:** *(nodding — Data gets it immediately)* That's right. On a fast machine, delta is small. On a slow machine, delta is large. But `position += velocity * delta` gives you the same movement per second either way.

**MARCUS:** *(the engineer, seeing the principle)* The Roman hour varies by season — longer in summer, shorter in winter. But the *work* of the day remains the same. The hour is the delta. The work is the computation.

Casey looks at Marcus with genuine surprise.

**CASEY:** ...That's exactly right. The variable hour. Delta time is the variable hour.

**Who:** Casey, Data, Marcus.

### Beat 3: Easing Functions — The Philosophy of Motion

Casey demonstrates easing:

```c
/* Motus linearis — constans, mechanicus */
f32 positio_linearis = lenire_linearis(t);

/* Motus cubicus egressus — celer initio, lenis fine */
f32 positio_cubica = lenire_cubicus_egressus(t);

/* Motus elasticus — oscillat ante quietem */
f32 positio_elastica = lenire_elasticus_egressus(t);
```

On screen: three dots moving from left to right. The linear dot moves at constant speed — mechanical, robotic. The cubic-out dot starts fast and decelerates smoothly — like a ball rolling to a stop. The elastic dot overshoots, bounces back, overshoots again, and settles — like a spring.

**CASEY:** Three ways to get from here to there. Linear is honest but ugly. Cubic is natural — things in the real world decelerate. Elastic is expressive — it shows effort, momentum, resistance.

**PLINY:** *(watching the dots, fascinated)* The third one — it goes past the destination and returns. Why?

**CASEY:** Because that's what springs do. When you stretch a spring and release it, it doesn't stop at equilibrium. It overshoots. It oscillates. Each oscillation is smaller than the last. It *settles*.

**PLINY:** *(writing)* A mathematical description of settling. Of finding rest.

**MARCUS:** *(watching the cubic-out dot)* The second one. The one that decelerates. That is how a cart stops. You do not simply cease moving. You slow, and slow, and then you stop. *(beat)* Your linear motion — that is how a machine stops. Instantly. Unnaturally.

**CASEY:** Right. The easing functions exist because reality isn't linear. Nothing in nature moves at constant speed. Things accelerate, decelerate, overshoot, bounce. The functions model reality.

He writes on the board:

```
lenire — "to soothe, to ease, to soften"

24 functions:
  linearis, quadraticus, cubicus, sinus,
  exponens, retro, elasticus, resiliens
  x ingressus, egressus, ingressus_egressus
```

**DANIEL:** *Lenire*. To soothe. You're soothing the motion. Removing the harshness of linearity.

**CASEY:** *(a small nod)* The name is Lucia's suggestion. And it's the right word. Easing IS soothing. You're making movement kind.

**Midas wanders through** — cheerful, carrying herbs from the garden. He pauses near the board.

**MIDAS:** *(looking at the three dots, smiling)* Time. When Herodotus visited me once, he brought a sundial. A beautiful Greek one. He set it in my garden and we watched it together for an afternoon. *(beat)* His sundial and the sundial in the Forum disagreed by an hour. He blamed the latitude. I never did learn what the difference was.

He wanders out. Thirty seconds. The comedy of a man who doesn't know that HIS PRESENCE is the reason sundials disagree. The audience who knows about the temporal anomaly will re-read this line on rewatch.

**Who:** Casey, Pliny, Marcus, Daniel, (Midas brief, 30 seconds).

### Beat F1: Roman Apprentice and the Rosary (Interwoven, Act 1-2)

**Location:** The workshop. During the easing functions teaching. Casey's rosary is around his wrist — not hidden, not displayed. Just there. A Roman apprentice, one of Marcus's engineers, has been working alongside Casey.

**ROMAN APPRENTICE:** *(casual, working)* The beads on your wrist. Marcus says they are for counting.

**CASEY:** *(still typing)* That's right.

**ROMAN APPRENTICE:** Counting what?

**CASEY:** Prayers.

**ROMAN APPRENTICE:** *(genuine curiosity — Romans count votive offerings too)* How many prayers?

**CASEY:** *(pauses his work. Looks at the rosary. A beat.)* Enough.

Five lines. Done. "Enough" is a loaded word in an episode about time. The Roman doesn't press further. He files it — this foreigner has a devotional practice involving repetitive prayer. Romans understand votive repetition. It's not alien. It's recognizable.

*PRODUCTION NOTE: The rosary should be visually paired with timepieces throughout the episode. Both are counting instruments. Both measure something invisible. Camera should find moments where Casey's wrist (rosary) is in frame next to whatever clock mechanism he's building. Never called out. Just composed.*

**Who:** Casey, Roman apprentice.

### Beat 4: ACT BREAK — Spring Physics

Casey demonstrates the spring:

```c
f32 positio = 0.0f;
f32 destinatum = 100.0f;
f32 velocitas = 0.0f;

/* Elastrum — elatere physicum */
positio = elastrum_ad(
    positio,       /* currens */
    destinatum,    /* destinatum */
    &velocitas,    /* velocitas — mutatur! */
    10.0f,         /* rigiditas */
    0.5f,          /* damnum */
    delta);        /* delta_tempus */
```

On screen: a dot. It springs toward the target, overshoots, bounces back, overshoots again (less this time), settles. Casey adjusts the parameters:

**CASEY:** Rigiditas — stiffness. Higher means faster oscillation. *(he increases it; the dot vibrates)* Damnum — damping. Higher means less bouncing. *(he increases it; the dot smoothly approaches without overshooting)*

**MARCUS:** *(who has been watching intently)* This is the principle of the arch. An arch under load deflects. If the load increases, it deflects further. If the load decreases, it returns. The stone is the stiffness. The mortar is the damping. *(beat)* Your spring is an arch that moves.

Casey stares at Marcus for a beat. Then:

**CASEY:** I've been building these systems for sixty-seven years. Nobody has ever described spring physics to me in terms of an arch. *(beat)* That's better than my explanation.

Carter notices. Casey just accepted someone else's framing. The hermit is learning.

**ACT BREAK.**

**Who:** Casey, Marcus, Carter.

---

## ACT TWO: "Unda" (The Wave)

*Wave functions. Frame statistics. Carter's private frequency analysis. Casey's first opening. Interpolation and color.*

### Beat 5: Wave Functions

Casey demonstrates:

```c
f32 t = (f32)tempus_nunc();

/* Unda sinus — motus naturalis */
f32 sinus = unda_sinus(t, 1.0f, 50.0f);

/* Unda triangulus — motus mechanicus */
f32 triangulus = unda_triangulus(t, 1.0f, 50.0f);

/* Unda dens serrae — incrementum graduale, casus subitus */
f32 dens = unda_dens_serrae(t, 1.0f, 50.0f);

/* Unda quadratus — ON vel OFF, nihil inter */
f32 quadratus = unda_quadratus(t, 1.0f, 50.0f);
```

On screen: four lines, each oscillating differently. The sine wave is smooth and natural. The triangle wave is sharp and linear. The sawtooth ramps up and drops. The square wave flips between two states.

**CASEY:** Four waveforms. Sine is nature — orbits, pendulums, sound. Triangle is mechanical — linear ramps, back and forth. Sawtooth is accumulation and reset — pressure building, then release. Square is binary — on or off, nothing between.

**PLINY:** *(transfixed by the sine wave)* That is the motion of the sea. The wave at Stabiae looked like that — rising, cresting, falling, rising again. *(a quiet beat)* I watched the sea from the ship while the sky burned.

Nobody responds. There's nothing to say. Pliny's Vesuvius moments are always like this — brief, devastating, private.

**CASEY:** *(after a respectful pause)* And there's noise:

```c
f32 tumultus = unda_tumultus(t);
/* Pseudo-aleatorius: -1.0 ad +1.0 */
```

On screen: a jagged, unpredictable line. No pattern. No rhythm. Just chaos.

**CASEY:** Noise. Pseudorandom values from time. Not a wave — the absence of one.

**DATA:** *(studying the noise function)* Deterministic noise. The same input always produces the same output. It appears random but is entirely predictable. *(beat)* A useful illusion.

**Who:** Casey, Pliny, Data.

### Beat 6: Frame Statistics

```c
StatisticaeQuadri* stats = tempus_obtinere_statisticas();

imprimere("Quadra destinata: %.1f QPS\n",
          stats->quadra_destinata_per_secundum);
imprimere("Quadra currentia: %.1f QPS\n",
          stats->quadra_currentia_per_secundum);
imprimere("Quadra media:     %.1f QPS\n",
          stats->quadra_media_per_secundum);
```

```
Quadra destinata: 60.0 QPS
Quadra currentia: 60.1 QPS
Quadra media:     59.9 QPS
```

**CASEY:** The machine measures itself. Target: sixty frames per second. Current: sixty point one. Average: fifty-nine point nine. Variance of less than one millisecond across three thousand eight hundred and forty-seven frames.

**TEAL'C:** *(studying the numbers)* Consistent performance. The machine is reliable.

**CASEY:** The machine is *predictable*. That's the prerequisite for reliable timing.

**CARTER:** *(from the side, a thought forming)* Casey — can I borrow your wave functions?

**Who:** Casey, Teal'c, Carter.

### Beat C2 (Interwoven, Act 2): Carter's Private Frequency Analysis

**Location:** Carter's workstation. She's alone. She opens her temporal data file — saved in 1x07. The compass readings. The oscillation patterns. Now she has Casey's wave library. She can analyze.

```c
/* Legere datas temporales ex disco */
chorda data = filum_legere_totum("anomalia_temporalis.txt", piscina);

/* Comparare cum unda sinus */
per (i = ZEPHYRUM; i < numerus_punctorum; i++)
{
    f32 observatum = puncta[i];
    f32 praedictum = unda_sinus(
        (f32)i * intervallum,
        frequentia_coniecta,
        amplitudo_coniecta);

    f32 error = observatum - praedictum;
    error_totalis += error * error;
}
```

She's fitting a sine wave to her compass data. The error decreases as she adjusts parameters. She finds the frequency.

```
Frequentia optima: 0.0037 Hz
Periodus: 270.3 secundae (~4.5 minutae)
Amplitudo: 0.0042
R^2: 0.94
```

The temporal distortion oscillates with a period of four and a half minutes. The sine fit is 94%. This is not noise. This is a signal.

She tries the square wave fit:

```c
f32 praedictum_q = unda_quadratus(
    (f32)i * intervallum,
    0.0037f,
    amplitudo_coniecta);
```

Square wave fit: 71%. Definitely sinusoidal, not binary. Smooth oscillation. Like breathing.

**CARTER:** *(to herself, very quiet)* It has a pulse.

She saves the analysis to disk — Filum (1x07) to read the data, Tempus wave functions (1x08) to analyze it, Piscina (1x01) to allocate the buffers. Three libraries from three episodes in one scene.

**CARTER:** *(staring at the results)* It's a heartbeat.

She closes the file. The secret has changed form again — from ephemeral to persistent (1x07), and now from approximate to precise. Carter can't dismiss this as an anomaly anymore. She's measuring something alive.

**Who:** Carter alone.
**Timing:** 60-90 seconds.

### Beat 7: "Huh. That's Better."

**Location:** Workshop. Casey and Carter at a workstation. The design session.

Carter proposes independent clocks with a shared frequency — the `tempus_creare` architecture. Each clock allocated from the piscina, each with its own epoch, but all referencing the same hardware frequency. Casey had been designing with a single global clock. Carter's version allows multiple independent timekeepers.

Casey's first instinct — physical, visible, the actor should show it — is to reject. His jaw tightens. His hand moves toward the keyboard.

Then he stops. He reads Carter's design again. The independent clocks still reference a shared frequency. They're independent but *calibrated*. It's not chaos — it's federation.

**CASEY:** *(beat)* Huh. That's better.

Four words. The turning point. The hermit acknowledges that another person's design improves on his own.

**CARTER:** *(carefully, because she knows what this means for him)* It's both of ours. Your clock, my synchronization.

**CASEY:** Yeah. *(beat)* Yeah, that's how it should work.

*DRAMATIC NOTE: Casey has been alone for thirty years because he decided collaboration was impossible in a world where nothing meant anything. Carter just showed him that collaboration works when both people care about the craft. The Web3 founder couldn't answer "What does it do for a person?" Carter can.*

**Who:** Casey, Carter.

### Flashback 1: Taylor Lecture (Interwoven, Act 2) — 60-90 seconds

**1911, Philadelphia.** Sepia-toned, period-film visual language.

*VISUAL RHYME: Casey's hands adjusting parameters on the Tempus code — CUT TO — Casey's hands holding a printed program in 1911.*

A lecture hall. Frederick Winslow Taylor on stage. "The Principles of Scientific Management." Charming, persuasive, completely certain. Taylor demonstrates with a volunteer — timing him with a stopwatch, correcting his posture, adjusting his grip. The audience applauds.

Casey watches from the back. Engineers. Managers. Industrialists. They are fascinated. They see efficiency. Casey sees something else.

He stands. He doesn't make a speech. He walks out. The door closes behind him. Taylor doesn't notice.

In the lobby, Casey looks at the printed program. The subtitle reads: **"The One Best Way."** He drops it in the trash.

*THE NOMINALISM (never stated): Taylorism reduces a person to a function — inputs, outputs, optimal parameters. People become measurements. Casey walking out tells the audience everything: he saw where this leads.*

### Beat 8: ACT BREAK — Interpolation and Color

Casey shows interpolation:

```c
/* Interpolare — valor inter duos fines */
f32 medio = interpolare(0.0f, 100.0f, 0.5f);
/* medio = 50.0 */

/* Gradus lenis — transitus sine asperitate */
f32 t = gradus_lenis(100.0f, 200.0f, 150.0f);
/* t ~ 0.5, sed leniore quam linearis */

/* Cohibere — continere intra limites */
f32 securum = cohibere(valor, 0.0f, 1.0f);
/* numquam infra 0, numquam supra 1 */
```

**MARCUS:** *(nodding)* The level. The plumb line. The compass. Three tools for ensuring a structure is true. Interpolation finds the midpoint — where the keystone goes. Smoothstep is the mortar curve — gradual compression, no cracks. Clamping is the boundary wall — nothing goes past the edge.

And color interpolation:

```c
i32 ruber = 0x000000FF;
i32 caeruleus = 0x00FF0000;

i32 purpureus = interpolare_colorem(ruber, caeruleus, 0.5f);
```

On screen: a smooth gradient from red to blue.

**PLINY:** *(watching the gradient, delighted)* The colors of dawn. The sky does not change from black to blue instantaneously. It interpolates. *(he writes furiously)* The machine can describe dawn.

**ACT BREAK.**

**Who:** Casey, Marcus, Pliny.

---

## ACT THREE: "Duo Horologia" (Two Clocks)

*Independent clocks. The drift. Casey's frustration. Casey and Carter — the revelation.*

### Beat 9: Independent Clocks

```c
/* Creare duo horologia independentia */
Tempus* horologium_A = tempus_creare(piscina);
Tempus* horologium_B = tempus_creare(piscina);

/* Ambo initiata eodem momento */
```

**CASEY:** Two clocks. Created from the same arena. Initialized at the same instant. They should agree.

After ten seconds:

```c
f64 A = tempus_elapsum(horologium_A);
f64 B = tempus_elapsum(horologium_B);
f64 differentia = A - B;

imprimere("A: %.9f\n", A);
imprimere("B: %.9f\n", B);
imprimere("Differentia: %.9f\n", differentia);
```

```
A: 10.000000127
B: 10.000000131
Differentia: -0.000000004
```

**CASEY:** Four nanoseconds of difference after ten seconds. Crystal oscillator jitter. Normal. Acceptable.

**CARTER:** Can you run them longer?

**Who:** Casey, Carter.

### Beat 10: The Drift

Casey lets the clocks run. He creates a visualization — two dots, one for each clock, moving across the screen at a rate determined by their `tempus_elapsum()` values, smoothed with `animare_ad()`.

For the first few minutes, the dots move together. Perfectly synchronized.

Then, around minute three, one dot pulls ahead. By a pixel. Then two. Then it falls back. The other dot leads. Then falls back. The dots are oscillating around each other.

**CASEY:** *(staring at the screen)* That's not jitter.

He measures. Plots the differences. On screen: a curve. Not random. Not noise. A sine wave. The two clocks disagree in a sinusoidal pattern.

**CASEY:** *(very quiet)* That's a waveform. The clocks are drifting in a periodic pattern.

**CARTER:** *(who has gone very still)* What's the frequency?

**CASEY:** Point zero zero three seven hertz. Period of about two hundred seventy seconds. Four and a half minutes.

Carter says nothing. But the audience has seen this number before — in Beat C2. The same frequency.

**TEAL'C:** *(a pause. One beat.)* I have... experience with unreliable time.

**O'NEILL:** *(from the back of the room)* Don't remind me.

Two lines. The fans who know "Window of Opportunity" will SCREAM. Everyone else sees a character moment.

*DIRECTION NOTE: O'Neill stays quiet during the rest of the drift discovery while others react. He files. He doesn't contribute. That silence is louder than dialogue.*

**Who:** Casey, Carter, O'Neill, Teal'c.

### Beat 11: Casey's Frustration

Casey tears the system apart:

```c
/* Probare: restituere et re-synchronizare */
tempus_restituere(horologium_A);
tempus_restituere(horologium_B);
/* ...drift redire post ~180 secundae */

/* Probare: unum horologium, duo lecturae */
f64 A1 = tempus_elapsum(horologium_A);
f64 A2 = tempus_elapsum(horologium_A);
/* A2 - A1 ~ 0.000000002 — normalis */

/* Probare: horologium globale contra independens */
f64 globalis = tempus_nunc();
f64 independens = tempus_elapsum(horologium_A);
/* ...etiam hic drift sinusoidalis! */
```

Resets, re-syncs. Drift returns. Tests single clock, two readings — normal. Tests global vs. independent — sinusoidal drift. Even the global clock and the independent clock disagree.

**CASEY:** *(the frustration of a craftsman whose tools report impossible things)* The hardware is fine. The code is correct. The oscillator is accurate. I've verified every path. *(beat)* The clocks are right. They are faithfully reporting what the hardware tells them. And the hardware is faithfully counting what the crystal tells it. And the crystal...

He stops.

**CASEY:** The crystal is faithfully oscillating in local spacetime. If spacetime itself is non-uniform...

**DATA:** *(completing the thought)* Then the clock is accurate. It is time that is inconsistent.

Silence.

**CASEY:** *(sitting down heavily)* The clock isn't broken. The planet is.

**Who:** Casey, Data.

### Flashback 2: IBM (Interwoven, Act 3) — 90-120 seconds

**1957, Poughkeepsie, New York.** Warm Technicolor palette. The hum of mainframes.

*VISUAL RHYME: Casey's hands debugging clock code on Nova Roma — CUT TO — Casey's hands tracing a pencil along a printout in 1957.*

Casey in a white shirt and tie, at a drafting table. Punch cards everywhere. The IBM 704 humming in an adjacent room. He's been here six years. He's good at this. He has colleagues he respects.

Casey is teaching a junior engineer how to read a core memory dump. The printout is three feet long. He traces a line with his pencil.

**CASEY:** See this? This is Mrs. Patterson's social security number. And this — *(he moves down the page)* — is her sister's. They're in adjacent records because the census bureau filed alphabetically by county.

**JUNIOR ENGINEER:** You can read the data? The actual people?

**CASEY:** Someone has to remember there are people in the machine.

The junior engineer nods. Casey goes back to the printout. He's content here. The craft is honest. The machines do what you tell them.

*PRODUCTION NOTE: Through the window: a military officer in the corridor. The machines were built for the census, then requisitioned for defense. The camera catches it. Casey doesn't look up. But the audience sees the trajectory — the tool doesn't determine its use. The institution that funds the tool does.*

This is the warmest flashback. Casey at his happiest. The audience should feel what he lost when he eventually left.

### Beat 12: Casey and Carter — The Revelation (Act 3 Break)

Carter pulls Casey aside. Private.

**CARTER:** I need to show you something.

She opens her file. The compass data. The periodic frequency. 0.0037 Hz. The sine fit.

**CASEY:** *(looking at the numbers)* That's the same frequency.

**CARTER:** I've been tracking this since 1x05. The compass anomaly. It wasn't random — it was periodic. I saved the data in 1x07 and discovered the frequency. And now your clocks are showing the same period.

**CASEY:** *(slowly)* The compass and the clocks are detecting the same phenomenon.

**CARTER:** Whatever is distorting the compass is also distorting time. *(beat)* It's centered on Midas.

Casey absorbs this. The craftsman's mind works differently from the physicist's — he doesn't think in theories, he thinks in tolerances.

**CASEY:** Can I build a clock that works here?

**CARTER:** You already did. The clock works. It reports what's real. What's real is that time on Nova Roma oscillates.

**CASEY:** That's not acceptable.

**CARTER:** *(gently)* It's not a matter of accepting it. It's a matter of accounting for it. Your clock doesn't need to pretend time is uniform. It needs to be honest about what it measures.

This is the turning point. Casey enters the conspiracy of silence. He is now the second person to know about the Midas temporal anomaly. His reaction is the craftsman's reaction: "Can I build a clock that works here?" Carter: "You already did."

**ACT BREAK.**

**Who:** Casey, Carter.

### Flashback 3: Web3 (Interwoven, late Act 3) — 90-120 seconds

**2019, San Francisco.** Washed-out blue-tinted palette. A WeWork.

*VISUAL RHYME: Casey's hands on the Nova Roma keyboard, after the revelation — CUT TO — Casey's hands, the same stillness, arms crossed in 2019.*

Open floor plan. Standing desks. A founder pitching to investors. Casey in the back, arms crossed.

**FOUNDER:** We're building a decentralized governance protocol for — *(checks notes)* — actually, governance is just one vertical. The platform is agnostic. The underlying asset is irrelevant.

After the pitch. Casey and the founder alone.

**CASEY:** What does it *do*?

**FOUNDER:** It provides a substrate for —

**CASEY:** What does it *do*. For a person. A specific person.

**FOUNDER:** *(genuinely confused)* It's a platform. It doesn't do something *for* someone. It enables —

**CASEY:** What?

**FOUNDER:** *(long pause)* ...Anything?

**CASEY:** *(standing)* I've been building things for a long time. Every time someone says "it can be anything," it turns out to be nothing.

He walks out. The same walk. The same door-closing beat as the Taylor lecture. The rhyme is deliberate.

**POST-WALKOUT:** Casey at a desk. Late night. The WeWork is empty. He pauses. Looks at the whiteboard. One phrase visible: **"Decentralized Everything."**

He erases the whiteboard. The whole thing. Clean. Walks out.

This is the breaking point. After this, Casey goes solo. Thirty years alone.

*THE NOMINALISM (never stated): Peak nominalism. Nothing has a nature. Everything is a platform. The very concept of "doing something for someone specific" is incomprehensible. The cobbler who knew feet. The factory that forgot feet. Taylor who measured feet. IBM that processed feet as numbers. Web3 declares feet irrelevant.*

---

## ACT FOUR: "Circumvolvere" (To Wrap Around)

*Casey accepts the drift. Timestamps on shifting ground. Utility functions. The three-voice moment. Teal'c's test suite.*

### Beat 13: Casey Accepts the Drift

Casey returns to the workshop. He doesn't rewrite the library. He adds a comment:

```c
/* NOTA: Tempus in Nova Roma non est uniformis.
 * Horologia fideliter referunt pulsus oscillatoris.
 * Oscillator fideliter refert tempus locale.
 * Tempus locale oscillat cum periodo ~270 secundarum.
 * Hoc non est defectus — est proprietas loci.
 *
 * NOTE: Time on Nova Roma is not uniform.
 * Clocks faithfully report oscillator pulses.
 * The oscillator faithfully reports local time.
 * Local time oscillates with a period of ~270 seconds.
 * This is not a defect — it is a property of the place.
 */
```

**CASEY:** *(to himself, writing)* Not a defect. A property.

This is enormous for Casey. The man who left Web3 because the foundations were rotten now writes documentation that says "the foundation moves, and that's okay." Not because it's acceptable — because it's *true*. The documentation is honest. The clock is honest. The drift is real.

**Who:** Casey alone.

### Beat F2: Marcus and the Rosary — "That I'm Not in Charge" (Act 4)

After Casey writes the drift documentation. Marcus finds him. A quiet moment.

**MARCUS:** You said "enough." I asked someone what you meant. They said you count prayers.

**CASEY:** I do.

**MARCUS:** You build clocks that measure in microseconds. You count prayers. What are you counting FOR?

**CASEY:** *(beat — the man who has just been humbled by a universe that won't cooperate)* Reminders. That I'm not in charge.

Marcus files it. Pietas — proper relationship to something greater. The engineer who counts to sub-microsecond accuracy, reminding himself he's not in charge. To a Roman, this is recognizable. Respectable. Marcus doesn't understand it fully. But he understands the shape of it.

*PRODUCTION NOTE: The crucifix between the two workstations is in frame. Background. Compositional. Nobody comments.*

**Who:** Casey, Marcus.

### Beat 14: Timestamps on Shifting Ground

Carter and Casey work together:

```c
/* Sigillum temporis — timestamp for persistence */
f64 nunc = tempus_nunc();

/* Scribere sigillum ad filum */
FilumScriptor* scriptor = filum_scriptor_aperire(
    "census_nova_roma.txt",
    FILUM_MODUS_APPENDERE,
    piscina);

filum_scriptor_scribere_literis(scriptor,
    "# Salvatum: ");
/* ... scribere nunc ut chorda ... */
filum_scriptor_sync(scriptor);
filum_scriptor_claudere(scriptor);
```

**CARTER:** The timestamp reports local time. If local time oscillates, the timestamp oscillates with it.

**CASEY:** So two files saved four and a half minutes apart might show timestamps that differ by four minutes and twenty-eight seconds. Or four minutes and thirty-two seconds.

**CARTER:** Correct. But the *ordering* is preserved. File A was saved before File B. The sequence is correct even if the intervals are slightly warped.

**CASEY:** *(reluctantly)* Sequence is enough. For now.

**MARCUS:** *(who has been listening)* The Roman calendar has the same problem. The lunar months don't divide evenly into a solar year. We add intercalary days. We adjust. The date is approximate, but the sequence is correct — January is always before February. *(beat)* Record faithfully, adjust when you can, trust the sequence.

**Who:** Casey, Carter, Marcus.

### Beat C3 (Interwoven, Act 3-4): Joe/Pliny Working Session

**Location:** Pliny's villa. Day. Joe and Pliny at a table covered with documents — Pliny's scrolls on one side, Joe's leather satchel on the other. Wine. Bread. The *adversaria* (field notebooks) side by side.

They're organizing. Comparing chronicle methodologies.

**PLINY:** *(fascinated by the Chronicle structure)* Each observation is attributed? By name?

**JOE:** By Watcher designation. Anonymous in the Chronicle itself.

**PLINY:** *(disapproving)* Anonymous? A scholar should stand behind his observations. But who wrote it IS part of what it says!

**JOE:** *(a long beat — Pliny is making an argument Joe has privately agreed with for years)* ...Yeah. I've thought that too.

They're building complementary filing systems. Pliny's indexed by subject (encyclopedia structure). Joe's indexed by person (chronicle structure). Together: what happened, and who it happened to.

**Who:** Joe, Pliny.
**Timing:** 45-60 seconds.

### Beat C4 (Interwoven, Act 4-5): O'Neill/Joe Intelligence — Ramirez Goes Dark

**Location:** O'Neill's quarters.

**O'NEILL:** Ramirez hasn't checked in.

**JOE:** How long?

**O'NEILL:** Two weeks. He was tracking Kirk's Seattle operation. Never missed a check-in in three years.

**JOE:** *(the Watcher instinct)* Two options. Either he's compromised, or he's been turned.

**O'NEILL:** The thing that bothers me: Ramirez's last transmission had a timestamp discrepancy. Off by minutes. Through the gate? Transmission should be instantaneous.

A beat. O'Neill looks toward the workshop where Casey is struggling with clock synchronization.

**O'NEILL:** Time discrepancy between Earth and Nova Roma. Casey's clocks don't match. And now Ramirez's timestamp doesn't match. *(beat)* I don't like coincidences.

**Who:** O'Neill, Joe.
**Timing:** 45 seconds.

### Beat 15: Utility Functions

Casey demonstrates:

```c
/* Circumvolvere — si excedis limitem, redis ad initium */
f32 angulus = circumvolvere(370.0f, 0.0f, 360.0f);
/* angulus = 10.0 — 370 degrees wraps to 10 degrees */

/* Oscillare — ire et redire sine fine */
f32 valor = oscillare(t, 100.0f);
/* 0 -> 100 -> 0 -> 100 -> 0... */

/* Remappare — transferre valorem ex uno campo ad alium */
f32 normalis = remappare(150.0f, 100.0f, 200.0f, 0.0f, 1.0f);
/* normalis = 0.5 — 150 is halfway between 100 and 200 */
```

**CASEY:** Circumvolvere — wrap. When an angle goes past three-sixty, it wraps back to zero. Not clamped — *wrapped*. The journey continues in the same direction but the counter resets.

**MARCUS:** The sundial. At noon the shadow reaches the bottom. It does not stop. It wraps back to the beginning and starts again.

**CASEY:** Oscillare — oscillate. Back and forth between zero and a value. A pendulum. A breath.

**CASEY:** And remappare — remap. Take a value from one scale and express it in another. Translate a coordinate. See the same thing from a different perspective.

**METHOS:** *(from his pillar, the first time he's spoken this episode — wine in hand, sardonic, earned)* That last one. Remapping. That's what translation is. You take a meaning from one language and express it in another. The value is the same. The scale changes. *(beat)* I've been doing that for five thousand years. Nice to see a function for it.

**Who:** Casey, Marcus, Methos.

### Three-Voice Algorithm Moment (Late Act 4)

Grows organically from the utility functions scene. Methos has broken the seal. The conversation deepens.

**CASEY:** *(looking at the easing functions on screen)* Time isn't constant. Some years take a decade. Some decades pass in an afternoon. The easing functions aren't abstractions. They're what time actually feels like. *(pointing)* That one — `lenire_retro_ingressus` — pulls backward before moving forward. That's memory. Every time you remember something vividly, time pulls you backward before it lets you go forward.

**METHOS:** *(wine, pillar, five thousand years)* Five thousand years. Most of it is waiting. Waiting for something to matter. Waiting for something to last. *(beat)* Time isn't long or short. It's dense or sparse. A decade where nothing happens is a single point. A single day where everything changes is infinite. *(gestures at the screen)* The constant one? `lenire_linearis`? That one is a lie. Time has never been linear.

**DATA:** *(precise, processing something he doesn't quite have the emotional vocabulary for)* My internal chronometer records time at nanosecond precision. Each second is identical to the last. There is no acceleration, no deceleration, no elastic behavior. My experience of time is `lenire_linearis` — perfectly constant. *(a beat that is longer than Data's usual beats)* And yet. When I am engaged in conversation with Midas, or solving a problem with Carter, or listening to Joe play his guitar — something is not. I believe the easing functions describe something my chronometer cannot measure. Something that exists between the ticks.

Three translations:
- **Casey:** Time as biographical experience — the Immortal who has lived it, felt decades compress and expand.
- **Methos:** Time as existential density — the oldest man alive, for whom most time is empty and some time is infinite.
- **Data:** Time as the gap between measurement and experience — the android who measures perfectly and suspects there's something more.

*The architecture: Casey speaks from the body. Methos speaks from the abyss. Data speaks from the gap between his measurements and his experience. Time management isn't about precision. It's about understanding that time has SHAPE.*

**Who:** Casey, Methos, Data.

### Beat 16: Teal'c's Test Suite (Act 4 Break)

```
probatio_tempus_initiare ............. VERITAS
probatio_tempus_nunc ................. VERITAS
probatio_tempus_delta ................ VERITAS
probatio_tempus_quadrum .............. VERITAS
probatio_horologium_independens ...... VERITAS
probatio_tempus_elapsum .............. VERITAS
probatio_tempus_restituere ........... VERITAS
probatio_tempus_actualizare .......... VERITAS
probatio_lenire_linearis ............. VERITAS
probatio_lenire_cubicus .............. VERITAS
probatio_lenire_elasticus ............ VERITAS
probatio_lenire_resiliens ............ VERITAS
probatio_elastrum_ad ................. VERITAS
probatio_interpolare ................. VERITAS
probatio_interpolare_angulum ......... VERITAS
probatio_cohibere .................... VERITAS
probatio_gradus_lenis ................ VERITAS
probatio_unda_sinus .................. VERITAS
probatio_unda_quadratus .............. VERITAS
probatio_circumvolvere ............... VERITAS
probatio_oscillare ................... VERITAS
probatio_remappare ................... VERITAS
```

Twenty-two tests. Twenty-two VERITAS.

**TEAL'C:** All operations verified. The global clock initializes and reports time. Independent clocks measure duration. Frame statistics track performance. All twenty-four easing functions produce values within the expected range. Spring physics converges. Interpolation is exact. Wave functions are periodic. Utility functions constrain correctly.

**CASEY:** Clean sweep.

**TEAL'C:** *(one observation, delivered with the weight of a man who understands unjust systems)* I tested the clocks extensively. Each clock, in isolation, is precise. The drift you observed between clocks is not a defect of the clocks. It is a property of the system they are measuring. *(beat)* The test that matters is not whether the clock agrees with another clock. It is whether the clock is honest about what it observes. Your clocks are honest. The disagreement is between them and the universe. That is not a failing of the clocks.

**ACT BREAK.**

**Who:** Teal'c, Casey.

---

## ACT FIVE: "Horologium Verum" (The True Clock)

*The completed library. The title moment. The test visualization.*

### Beat 17: The Completed Library

Casey stands before the board. The full Tempus API:

```
TEMPUS — Bibliotheca Temporis

Horologium Globale:
  tempus_initiare()           — initialize the clock
  tempus_nunc()               — current time in seconds
  tempus_delta()              — time since last frame
  tempus_quadrum()            — mark new frame

Administratio Quadrorum:
  tempus_ponere_quadra_destinata()  — set target FPS
  tempus_exspectare_quadrum()       — wait for next frame

Horologia Independentia:
  tempus_creare(piscina)      — create timer
  tempus_elapsum(tempus)      — elapsed time
  tempus_actualizare(tempus)  — update timer
  tempus_restituere(tempus)   — reset timer

Lenitionis: 24 functiones
Adiutores: animare_ad, elastrum_ad
Interpolatio: interpolare, angulum, colorem, gradus_lenis, cohibere
Undae: sinus, triangulus, dens_serrae, quadratus, tumultus
Utilitas: circumvolvere, oscillare, remappare
```

**CASEY:** Forty-seven functions. One global clock. Independent timers. Frame management. Animation. Interpolation. Waves. Utilities. *(beat)* And the clocks drift by four nanoseconds per oscillation cycle on a planet where time is sinusoidal.

**CARTER:** *(smiling)* The most honest time library ever written.

**CASEY:** *(not smiling — serious)* The documentation says the drift exists. The documentation says it's not a bug. The documentation says what the frequency is. *(beat)* I don't ship broken tools. But I don't lie about what they measure, either.

**Who:** Casey, Carter.

### Beat 18: The Title Moment

**Location:** The Tabularium. Late afternoon. The team gathered.

Carter writes on the board:

```
Dependency tree (season so far):

Piscina (1x01)
  +-- Credo (1x02) — testing
  +-- Chorda (1x03) — strings
  |     +-- ChordaAedificator (1x03)
  |     +-- TabulaDispersa (1x05) — hash table
  |     |     +-- Internamentum (1x06)
  |     +-- Filum (1x07) — file I/O
  +-- Tempus (1x08) — time  <--
```

**CARTER:** Eight episodes. Eight libraries. The tree has three branches now. Tempus stands alongside Filum and the hash table chain — it shares the arena, but it doesn't need strings or files or hashing. It's a different kind of foundation.

**CASEY:** *(looking at the dependency tree — his library, his branch)* Every library so far has been about *what*. What to store, what to look up, what to save. Tempus is about *when*. It's the first library that exists in time rather than in space.

**MARCUS:** *(standing)* An horologium. In Rome, the horologium is not merely a device for telling time. It is a public institution. The Horologium Augusti — the sundial of Augustus — stood in the Campus Martius. The shadow of an Egyptian obelisk falling across a marble floor inscribed with the zodiac and the days of the year.

He turns to the board.

**MARCUS:** Your `tempus_nunc()` — "what time is it now?" — is the Horologium Augusti. One answer, shared by all. Your `tempus_creare()` — a private clock — is the personal sundial. A civilization needs both.

**CASEY:** And when the public clock and the private timer disagree?

**MARCUS:** *(a long beat)* The Horologium Augusti was wrong for thirty years — brought from the wrong latitude, nobody noticed. A wrong public clock is better than no public clock. *(beat)* Your clocks are not wrong. They tell you time is strange in this place. That is more valuable than a clock that pretends time is normal.

**CASEY:** *(quietly)* You're saying the drift is data.

**MARCUS:** I'm saying the drift is evidence.

Casey writes on the board:

**HOROLOGIUM**

**CASEY:** The clock. Not a timekeeping device — a truth-telling device. It tells you what time does here.

**Who:** Casey, Carter, Marcus, full team.

### Beat 19: Test Visualization

Casey builds a final demo:

```c
tempus_initiare();
tempus_ponere_quadra_destinata(60.0);

f32 x = 0.0f;
f32 x_vel = 0.0f;
f32 color_t = 0.0f;

dum (VERUM)
{
    tempus_quadrum();
    f32 delta = (f32)tempus_delta();
    f32 t = (f32)tempus_nunc();

    /* Elatere ad destinatum */
    x = elastrum_ad(x, destinatum, &x_vel, 10.0f, 0.5f, delta);

    /* Pulsus sinusoidalis */
    f32 pulsus = unda_sinus(t, 1.0f, 20.0f);

    /* Transitus coloris */
    color_t = cohibere(color_t + delta * 0.1f, 0.0f, 1.0f);
    i32 color = interpolare_colorem(ruber, caeruleus, color_t);

    /* FPS */
    StatisticaeQuadri* stats = tempus_obtinere_statisticas();

    tempus_exspectare_quadrum();
}
```

On screen: an animated dot springs toward the cursor, pulsing with sine-wave breathing, its color transitioning smoothly from red to blue. In the corner, the FPS counter holds steady at 60. Below the dot, the two clock readings scroll — and the sine-wave drift is visible. A beautiful, unsettling animation. Everything works. Everything is precise. And the ground shifts.

The team watches. Marcus sees Roman engineering made digital. Pliny sees the mathematics of nature. Data sees clean architecture. Casey sees his life's work — and the flaw in reality it exposed.

**Who:** Casey, full team observing.

---

## TAG

### Beat 20: Joe/Casey at the Tavern

**Location:** A corner of a Roman tavern Joe has claimed. Evening. Wine on the table. Casey enters.

**JOE:** Duncan says you've been alone for thirty years.

**CASEY:** Duncan talks too much.

**JOE:** I've spent thirty years watching someone else live. *(beat)* What made you stop watching?

**CASEY:** *(sits, takes the offered wine)* I came here.

**JOE:** *(long beat)* Yeah. Me too.

Two men who chose isolation — Joe as Watcher, Casey as hermit — arriving at the same place from opposite directions.

**Who:** Joe, Casey.
**Timing:** 30 seconds.

### Beat 21A: Casey and Carter — The Evening Walk

Casey doesn't usually walk with Carter. That's Carter and Marcus's ritual. But tonight Carter finds Casey on the road, walking alone.

**CARTER:** You built a beautiful library.

**CASEY:** I built a clock that tells you time is broken.

**CARTER:** You built a clock that tells you what time *does*. That's more useful than a clock that lies.

Silence. Walking.

**CASEY:** *(very quiet)* Midas. You said it's centered on Midas.

**CARTER:** The compass readings. The clock drift. They share the same frequency. The same waveform. The strongest readings are always near Midas.

**CASEY:** *(after a long silence)* He doesn't know, does he?

**CARTER:** No.

**CASEY:** A man who warps time just by existing. And we're building a clock on his planet.

**CARTER:** We're building a clock *despite* his planet. And the clock works. It's honest. That's enough for now.

Casey stops. Looks up at the stars — different constellations than Earth. A different sky. A different time.

**CASEY:** Next, we need to structure the data. Everything we're saving is flat text. The census is lines in a file. But the data *has* structure — hierarchy, categories, relationships. *(beat)* XML.

**CARTER:** Arbor documentorum. Tree of documents.

**CASEY:** *(a small nod)* The tree.

**Who:** Casey, Carter.
**Timing:** 60 seconds.

### Beat 21B: Data and Casey — The Quiet Scene

**Location:** Workshop. Late. Data is reviewing the Tempus code. Casey returns.

**DATA:** Your code is elegant.

**CASEY:** *(surprised)* It's functional.

**DATA:** It is elegant *because* it is functional. Twenty-four functions. No unnecessary state. No allocation. Each function takes a value from zero to one and returns a value from zero to one. Pure transformation. *(beat)* Captain Picard once showed me a watch. A mechanical watch. Gears, springs, escapement. He said: "The measure of its elegance is the absence of anything unnecessary."

Casey sits down. This is the first time anyone has appreciated his work at the level of craft, not just utility.

**CASEY:** The easing functions are the oldest part. I wrote the first versions in 1971. On an IBM System/370. Punch cards. *(beat)* The math hasn't changed. The sine function in 1971 is the same sine function today. That's the thing about math. It doesn't deprecate.

**DATA:** *(precise)* In the twenty-fourth century, the same easing curves are used. The implementation language changes. The mathematics endures.

**CASEY:** *(quietly)* That's... good to know.

A beat. Two builders — one immortal, one android — recognizing that the things they build from mathematics will outlast everything else.

FADE TO BLACK.

**Who:** Data, Casey.
**Timing:** 60 seconds.

---

## STINGER

### Beat 22: Kirk's CHRONOGRAPHIA

**Location:** Earth. Night. A monitoring station in the Synthesis Dynamics office (established 1x06). Screens showing real-time data feeds. Timestamps scrolling. Kirk alone.

He's watching Nova Roma's temporal signature through intercepted gate telemetry. On one screen: a sine wave. Period: ~270 seconds. Amplitude: tiny but consistent. The same wave Carter found. The same frequency Casey's clocks drift.

Kirk circles the frequency on the screen.

**KIRK:** *(to himself)* The anomaly has a pulse.

He opens a physical folder — handwritten, consistent with the filing-cabinet methodology from 1x07. The header reads: **MIDAS — TEMPORAL PROFILE**.

He writes one line: **Freq: 0.0037 Hz. Sinusoidal. Consistent. Biological?**

The question mark is the gut-punch. Kirk suspects what Carter suspects — that the temporal distortion is connected to Midas as a living being. That the distortion *breathes*. But he doesn't confirm. He suspects.

He closes the folder. Opens another drawer. Pulls out a stopwatch — analog, mechanical. Winds it. Sets it next to a digital clock on the wall. The digital display reads 23:47:12. The stopwatch starts ticking.

**KIRK:** *(watching both clocks)* Two clocks. Let's see if they agree.

He writes on the whiteboard: **8 / 31**.

Below ARCHIVUM: **CHRONOGRAPHIA**

Kirk stinger progression updated: TABULARIUM (1x05) → LEXICON (1x06) → ARCHIVUM (1x07) → CHRONOGRAPHIA (1x08). The words track Kirk's operational evolution: indexing → standardizing → persisting → *timing*. The team built an horologium (a clock for the city). Kirk is building a chronographia (a timeline for the target).

**KIRK:** *(looking at the word, then at the two clocks)* Time tells. *(beat)* It always tells.

SMASH TO BLACK.

END OF EPISODE.

---

## ON-SCREEN CODE MOMENTS

### Moment 1: Casey's Dawn Clock (Cold Open)
```c
tempus_initiare();
f64 nunc = tempus_nunc();
/* 0.000043 secundae */
```
The first tick. Casey alone, building in the dark.

### Moment 2: The Frame Loop (Act 1)
```c
tempus_quadrum();
f64 delta = tempus_delta();
tempus_exspectare_quadrum();
```
The heartbeat. Sixty frames per second.

### Moment 3: Easing Demonstration (Act 1)
```c
f32 lin = lenire_linearis(t);
f32 cub = lenire_cubicus_egressus(t);
f32 ela = lenire_elasticus_egressus(t);
```
Three dots. Three philosophies of motion.

### Moment 4: Spring Physics (Act 1 Break)
```c
positio = elastrum_ad(positio, destinatum,
    &velocitas, rigiditas, damnum, delta);
```
The spring. Marcus's arch.

### Moment 5: Wave Functions (Act 2)
```c
unda_sinus(t, 1.0f, 50.0f);
unda_triangulus(t, 1.0f, 50.0f);
unda_dens_serrae(t, 1.0f, 50.0f);
unda_quadratus(t, 1.0f, 50.0f);
```
Four waveforms. Pliny sees the sea.

### Moment 6: Carter's Frequency Analysis (Act 2)
```c
f32 praedictum = unda_sinus(
    (f32)i * intervallum,
    frequentia_coniecta,
    amplitudo_coniecta);
/* Frequentia optima: 0.0037 Hz */
```
The temporal anomaly has a pulse. Three libraries, three episodes, one analysis.

### Moment 7: Two Clocks Diverge (Act 3)
```c
f64 A = tempus_elapsum(horologium_A);
f64 B = tempus_elapsum(horologium_B);
f64 differentia = A - B;
/* sinusoidal drift — 0.0037 Hz */
```
The mystery. Two clocks, same hardware, different answers.

### Moment 8: Casey's Documentation (Act 4)
```c
/* Hoc non est defectus — est proprietas loci. */
/* This is not a defect — it is a property of the place. */
```
The craftsman accepts the drift. Two lines of comment. A philosophical shift.

### Moment 9: The Timestamp (Act 4)
```c
f64 nunc = tempus_nunc();
filum_scriptor_scribere_literis(scriptor, "# Salvatum: ");
```
1x07's files gain dates. Filum + Tempus.

### Moment 10: The Full Demo (Act 5)
```c
x = elastrum_ad(x, dest, &vel, 10.0f, 0.5f, delta);
f32 pulsus = unda_sinus(t, 1.0f, 20.0f);
i32 color = interpolare_colorem(ruber, caeruleus, t);
StatisticaeQuadri* stats = tempus_obtinere_statisticas();
```
Everything together. Spring, wave, color, statistics. Beautiful. Unsettling.

---

## ENSEMBLE BEATS

### Casey — The Spotlight
Casey drives both A-plot and flashback. His arc: Mastery → Crisis → Acceptance. What we learn: he works before dawn alone; he names his work for the first time ("Casey scripsit"); sixty-seven years of timing expertise; when his tools report impossible things, his instinct is to doubt the tools, not reality — Carter teaches him to trust the tools; he accepts collaboration genuinely ("Huh. That's better"); he's the second person to learn about Midas. The hermit who builds perfect things confronts a universe that isn't perfect, and his precision deepens rather than breaks.

### Carter — The Partner Under Strain
Casey's co-designer and the person who inducts him into the conspiracy of silence. Her private frequency analysis (Beat C2) uses three libraries from three episodes — the dependency chain alive in one scene. She carries the Midas data from intuition (1x05) to persistent evidence (1x07) to precise, timestamped, publishable data (1x08). "It's a heartbeat" is the moral knife — Midas isn't a malfunction. He's alive. And she can't un-know it.

### Marcus — The Roman Engineer
Consistently excellent without a centerpiece scene: the variable hour (Beat 2), the arch as spring physics (Beat 4), keystone/mortar/boundary (Beat 8), the sundial wrapping (Beat 15), the Horologium Augusti (Beat 18), "the drift is evidence." Marcus provides the Roman engineering lens for every concept. His crowning line: "A wrong public clock is better than no public clock."

### Teal'c — The Honest Tester
The "Window of Opportunity" callback (Beat 10) — "I have... experience with unreliable time" — two lines, maximum impact. His test suite (Beat 16, 22 VERITAS) culminates in the episode's philosophical anchor: "The disagreement is between them and the universe. That is not a failing of the clocks." The freed slave who insists that systems tell the truth.

### Data — The Measured Mystery
Data grasps decoupled timing instantly (Beat 2). His key insight — "It is time that is inconsistent" (Beat 11) — catalyzes Casey's understanding. His three-voice moment ("something that exists between the ticks") is the android discovering the limit of his own measurement. The quiet scene with Casey (Beat 21B) is a recognition between two builders that mathematics endures.

### Pliny — The Poet-Observer
The sea at Stabiae (Beat 5) — brief, devastating, private. "The machine can describe dawn" (Beat 8). Pliny is not spotlighted but his observations give the code emotional resonance. His working session with Joe (Beat C3) advances the documentary partnership from 1x07.

### Methos — Surgical Precision
One speech, perfectly placed. "Remapping. That's what translation is. I've been doing that for five thousand years." (Beat 15). This breaks the seal for the three-voice moment. Then: "The constant one? `lenire_linearis`? That one is a lie." (Three-voice). Methos at his best — sardonic, earned, light touch.

### Joe — Finding His Role
Three appearances: working session with Pliny (C3), intelligence consulting with O'Neill (C4), mirror scene with Casey (Beat 20). Joe intersects every running thread — the documentary partnership, the Kirk intelligence plot, and Casey's isolation. "What made you stop watching?" / "I came here." / "Yeah. Me too." — the emotional palette cleanser.

### O'Neill — The Filer
"Don't remind me" (Beat 10 — Window of Opportunity callback). The intelligence review with Joe (C4 — "I don't like coincidences"). Silence during the clock-drift discovery (Beat 10 — direction note, not dialogue). O'Neill notices everything and says almost nothing.

### Midas — The Unknowing Anomaly
Thirty seconds. Wanders through, drops the Herodotus sundial anecdote, wanders out (Beat 3). A plant hiding in plain sight. The man whose very existence is the mystery everyone else is investigating, cheerful and oblivious.

### Duncan — Background
Not spotlighted. His work is offscreen — arranging Joe's settlement, coordinating with Marcus on construction. Present at the easing functions demonstration. His relationship with Casey is implied through Joe's comment: "Duncan says you've been alone for thirty years."

### Lucia — The Namer
Named the easing functions: "Lenire — to soften." Casey credits her (Beat 3). Her contribution is linguistic grounding — connecting computational concepts to classical Latin.

---

## THREE-VOICE ALGORITHM MOMENT

| Episode | Three Voices | Algorithm | Lens |
|---------|-------------|-----------|------|
| 1x05 | Pliny / Data / Casey | Hashing | Fingerprint of knowledge |
| 1x06 | Methos / Data / Casey | Interning | Canonical copy, one for all |
| 1x07 | Joe / Pliny / Data | Persistence | Writing to survive |
| **1x08** | **Casey / Methos / Data** | **Time management** | **Time as biography / density / gap** |

**Placement:** Late Act 4, growing organically from the utility functions scene (Beat 15) after Methos's first line of the episode.

**Structure:**
- **Casey:** Time as biographical experience — the easing functions are what time actually feels like. Memory is `lenire_retro_ingressus` — pulling backward before moving forward.
- **Methos:** Time as existential density — five thousand years, most of it waiting. The constant function is a lie.
- **Data:** Time as the gap between measurement and experience — nanosecond precision, identical seconds, and yet something exists between the ticks that his chronometer cannot measure.

---

## THEMATIC NOTES

### Nominalism / Realism — Casey's Flashbacks as Thesis

The nominalism thread runs through Casey's biography, never stated, always dramatized:

| Flashback Era | What Casey Sees | Nominalist Step |
|--------------|----------------|-----------------|
| **Fragment: 1858 (First Death)** | Cobbler's hands on leather — a craft with particular knowledge | Pre-nominalist baseline (saved for full treatment later) |
| **1911: Taylorism** | People measured, optimized, timed. Stopwatches everywhere. | People as machines to be optimized. Time as tool of control. |
| **1957: IBM** | Computing born from census bureau. Mrs. Patterson's social security number. "Someone has to remember there are people in the machine." | Institutions use computing as control, but the engineering itself has integrity. The craft is real. |
| **2019: Web3** | "What does it DO?" "...Anything?" Nothing has a nature. Everything is a platform. | Peak nominalism. The cobbler who knew feet becomes irrelevant when feet are irrelevant. |

**The present-day counterpoint:** Casey builds a time library on Nova Roma — the most fundamental realist act in computing. Time HAS a nature. It CAN be measured. The measurement is true even when the reality is strange. A clock that honestly reports warped time is more truthful than a platform that can be "anything."

### Easing Functions as Philosophy

The easing library encodes the realist position: time has shape. Some moments are heavier than others. The twenty-four functions are twenty-four ways change can happen — none of them linear. `lenire_linearis` is the nominalist position (time is a neutral container, one second like any other). Casey builds twenty-three alternatives because he's LIVED twenty-three alternatives.

### The Variable Roman Hour

Marcus's contribution: "In Rome, time breathes." The Roman hour stretched in summer and compressed in winter because Rome tied time to the sun. Near Midas, time stretches and compresses because of the temporal anomaly. To the Romans, this isn't a violation of physics — it's *familiar*. Their hours have always been elastic. Marcus is the only person on Nova Roma who ISN'T disturbed by temporal variability.

### Chronos and Kairos

The Tempus library encodes both Greek concepts without naming either. The global clock (`tempus_nunc`, `tempus_delta`, `tempus_quadrum`) is pure chronos — mechanical, sequential, invariant. The easing functions and wave functions (`lenire_*`, `unda_*`) are kairos-adjacent — they describe how time feels, how not all moments are equal. Casey's library contains the mathematics of both: invariant measurement AND qualitative variation.

### Casey's Rosary and Casey's Clock

Both are counting instruments. Both measure something invisible. Both require patience. The rosary counts prayers. The clock counts pulses. The man who builds clocks that measure in microseconds carries prayer beads. "Reminders. That I'm not in charge." Two systems of counting, both essential to him. The camera finds them together in frame. Never called out.

---

## GENRE ARCHITECTURE

### The Dependency Graph Branches Again

This is the second branching of the dependency graph. Tempus goes back to Piscina (1x01) and builds a different thing — it doesn't need Chorda, Filum, Tabula Dispersa, or Internamentum. The tree grows wider, not just taller.

```
Piscina (1x01)
  +-- Chorda (1x03)
  |     +-- ... (hash table chain)
  |     +-- Filum (1x07)
  +-- Tempus (1x08)  <-- second branch
```

### Casey as Time Personified

The Immortal building a clock. 200 years of lived time, formalized into 47 functions. The easing functions are autobiography — `lenire_retro_ingressus` (pulling backward before moving forward) is every Highlander flashback. `elastrum_ad` (spring toward destination, damped by community) is Casey's entire season arc. The code IS the subtext.

### Franchise Resonances

- **SG-1 "Window of Opportunity":** Teal'c's Latin from the time loops → reading Nova Roma code comments since 1x01 → "I have experience with unreliable time" (Beat 10).
- **TNG "Cause and Effect":** Carter's periodic frequency is Data's threes — a signal embedded in a temporal anomaly. Carter is reading the anomaly's poker hand.
- **TNG "The Inner Light":** Time as compressed experience. Data carries Picard's Katan memories. When Casey describes non-linear time, Data has a reference point that isn't his own.
- **VOY "Year of Hell" / Annorax:** Kirk studying time to exploit it. Kirk as Annorax — the villain who treats time as a weapon.
- **Highlander "The Gathering":** Casey's 5-second First Death fragment — Highlander cold-open energy. The gasping return.

### Kirk's Stinger as Dark Mirror

The team builds an horologium (a clock for the city). Kirk builds a chronographia (a timeline for the target). Same data. Same frequency. Same question mark next to "Biological?" The team discovers time is strange through building. Kirk discovers it through surveillance. Same math. Different purpose.

Kirk stinger progression: TABULARIUM → LEXICON → ARCHIVUM → CHRONOGRAPHIA. Indexing → standardizing → persisting → *timing*.

---

## CONTINUITY NOTES

### Backward Connections

**From 1x01 (Piscina):** Independent timers are arena-allocated via `tempus_creare(piscina)`. Carter's frequency analysis allocates buffers from the piscina. The arena now serves the time layer directly.

**From 1x02 (Credo):** Teal'c's testing methodology continues. 22 VERITAS. The philosophy from 1x02 carries: the test reveals a property the system should have.

**From 1x03 (Chorda):** Casey's IBM flashback (1957) — the same era as the null-termination argument in 1x03's flashback. The audience who remembers sees continuity: same building, same man, five years apart.

**From 1x05 (Tabula Dispersa):** Carter's compass anomaly first detected. Kirk's whiteboard word "TABULARIUM" started the dark-mirror vocabulary.

**From 1x06 (Internamentum):** Carter's compass data became persistent. Methos's Alexandria revelation. Kirk's "LEXICON."

**From 1x07 (Filum):** Carter saved temporal data to disk. Discovered periodicity. Files have no dates — motivating the Tempus library. Casey says grace at Pliny's villa; O'Neill notices ("Huh"). Joe arrives. Kirk's "ARCHIVUM." Joe/Pliny bond established.

**From 1x07 (Carter's C-plot):** Carter discovered the oscillation is periodic with persistent data. In 1x08, she confirms the frequency (0.0037 Hz) with Casey's precision tools.

### Forward Connections

**To 1x09 (Arbor / XML):** Casey and Carter's closing line: "Arbor documentorum. Tree of documents." Flat files are limited — Pliny's encyclopedia has structure, Joe's Watcher records have hierarchy. The XML episode is motivated by the limitations of flat file I/O with timestamps but no structure.

**To 1x09-10 (Joe/Methos):** Deferred reunion. Private. Late night. Wine. Joe and Methos haven't spoken directly yet — the 3-second eye contact at dinner in 1x07, and separate orbits in 1x08. The tension builds.

**To 1x13 (Aurum / Midas):** Carter and Casey now share the secret. The investigation has two scientists. Daniel has Watcher records corroborating Methos's fragments. The dossier grows toward the Midas flashback.

**To 1x15-16 (Casey teaches):** Casey's brief teaching moment with Marcus (Beat 3-4 — accepting the arch analogy) seeds the longer teaching arc. He's terrible at teaching but getting better.

**To 1x18 (TLS / Encryption):** Carter's growing concern about findable secrets — persistent timestamped data about Midas is discoverable evidence.

---

## THREAD STATUS APPENDIX: After 1x08

| Thread | Status After 1x08 | Next Beat |
|--------|-------------------|-----------|
| **Casey's Faith** | **ESCALATED TO CULTURE.** Roman apprentice asks about rosary beads. "How many prayers?" "Enough." Then Marcus circles back: "Reminders. That I'm not in charge." Two faith beats — the community noticing (F1) and the personal deepening (F2). Rosary visually paired with clocks all episode. | 1x09-10: "Enough for what?" Pliny or another Roman presses. Casey: "This is going to take more than one evening." |
| **Kirk Stinger** | **8/31. CHRONOGRAPHIA.** Kirk analyzing gate telemetry. Midas temporal profile folder. "Biological?" Two clocks (analog + digital). "Time tells." The dark-mirror divergence: team builds horologium (clock for the city), Kirk builds chronographia (timeline for the target). | 1x09: Kirk mirrors XML/structure. |
| **C-Plot (Kirk/Earth)** | **CONTACT LOST.** Ramirez goes dark. Timestamp discrepancy between his transmission and Nova Roma receipt. O'Neill and Joe working as intelligence team. | Ongoing. More contacts go dark. Kirk's network absorbing assets. |
| **Joe Dawson** | **SETTLING IN.** First working session with Pliny. Intelligence consulting with O'Neill. Mirror scene with Casey. Finding his role — not tourist, not observer, but participant. | 1x09-12: More sessions. Bar location. Joe/Methos private conversation (due 1x09-10). |
| **Joe/Pliny Partnership** | **FIRST WORKING SESSION.** Complementary filing systems. Attribution debate ("who wrote it IS part of what it says"). Joe privately agrees. | 1x09-12: Deeper collaboration. By 1x13: the bar opens as second workspace. |
| **Black Stone / Cybele** | **UNCHANGED from 1x07.** No direct beat this episode. Daniel working with dossier offscreen. | 1x09 (XML): Daniel with structured ancient texts. |
| **Midas Temporal** | **FREQUENCY CONFIRMED + CONSPIRACY EXPANDED.** Carter has precise timestamps. 0.0037 Hz, 270 seconds, sinusoidal. "It's a heartbeat." Casey enters conspiracy of silence — the second person to know. His clocks confirmed what Carter's compass showed. Plus: time discrepancy noticed by O'Neill (C4). Kirk analyzing same frequency from Earth (stinger). | 1x09-10: Does the frequency shift with Midas's emotional state? |
| **Nominalism / Realism** | **CASEY'S BIOGRAPHY IS THE ARGUMENT.** Three flashback eras = three nominalist steps (Taylor/measurement, IBM/institutional corruption, Web3/nothing has a nature). Present-day counterpoint: Casey builds a clock that honestly measures a strange reality = realist act. Easing functions as anti-linear philosophy. | Ongoing. Each library deepens. |
| **Pliny's Mortality** | **BACKGROUND.** Pliny present, energized by working sessions with Joe. The mortality thread rests — Pliny is too engaged to be melancholy. | Ongoing. |
| **Data/Midas Friendship** | **BACKGROUND.** Data's three-voice moment ("something between the ticks") touches Midas indirectly — he mentions conversations with Midas as moments where his chronometer doesn't match his experience. | Ongoing. Data may begin his own analysis of Midas's patterns. |
| **Daniel as Investigator** | **WORKING.** Cross-referencing Methos fragments with Watcher records. Not spotlighted — Casey is focus. | 1x09 (XML): Tools for structured text analysis. Investigation accelerates. |
| **Methos's Secret** | **RESTING.** One speech in the three-voice moment. No spotlight — his was 1x06. | 1x09-10: Joe/Methos private conversation. |
| **Carter's Secret Burden** | **SCIENTIFIC + SHARED.** The burden is precise, timestamped, publishable data. And now Casey knows. The burden is lighter (shared with one person) and heavier (undeniable evidence of something alive). | 1x09-10: The data demands she tell someone else. Her silence becomes harder to justify. |
| **Earth-Side Seed** | **PLANTED.** "Smart Town" New Hampshire chyron. 3 seconds. Nobody notices. | 1x10: Gilfoyle/Dinesh introduction per season outline. |
| **Time Discrepancy** | **NEW THREAD (facet of Midas Temporal).** O'Neill notices Ramirez's timestamp doesn't match. Minutes off through an instantaneous gate. Casey's clocks don't match in the A-plot. Flagged by intelligence, not just by the physicist. | 1x09-12: More data points accumulate. |

---

## ADDITIONAL NOTES

### Production Design — The Sundial

Nova Roma should have sundials visible in the Forum. Type: hemicyclium (hemispherical, carved into stone). The hour markings are for variable hours — lines closer together in summer, farther apart in winter. This visual detail grounds the variable-hour concept without dialogue. When Marcus says "In Rome, time breathes," the set design should already be showing it.

### Production Design — Casey's Workshop

Key elements:
- Crucifix on the wall (established 1x02, in every Casey workshop scene)
- Rosary on desk or wrist (escalating visibility through the season)
- Mechanical watch — analog, not digital. Twelve fixed equal hours: the visual contrast to the Roman sundial's elastic divisions.
- Spare, organized, military precision. Casey's space reflects his mind.

### Performance Notes

**Casey (Cold Open):** The 5-second First Death fragment should be startling — a flash of amber light, leather, a different world — and then GONE. Casey blinks. The audience doesn't know what they just saw. But they know there's more.

**Casey (Beat 13 — "Not a defect. A property."):** Delivered to himself. The weight of a man rewriting his understanding of reality in two lines of code comment. Not dramatic. Private.

**Casey (Beat F2 — "That I'm not in charge."):** After the clock crisis, after accepting the drift, after being humbled by a universe that won't cooperate. This is the line that connects Casey's faith to his engineering. The man who counts microseconds, reminding himself he's not in charge.

**Marcus (Beat 18 — "The drift is evidence."):** This is Marcus at his best — the pragmatic Roman engineer who sees truth in imperfection. Delivered with quiet authority.

**Methos (Beat 15 — "Nice to see a function for it."):** Light. Sardonic. The wine is always in his hand. Five thousand years of watching people reinvent things, and he's amused by it, not dismissive.

**Teal'c (Beat 16 — "That is not a failing of the clocks."):** Delivered with the weight of a man who understands systems that are just within unjust contexts. The freed slave who knows the difference between a broken instrument and a broken world.

### Historical Grounding

- The Horologium Augusti: the sundial of Augustus, wrong for 30+ years (Pliny, NH 36.72-73). Marcus would know this.
- The variable Roman hour: 12 seasonal hours, stretching and compressing (Pliny, NH 2.188; Censorinus, *De Die Natali* 23-24).
- The Julian calendar reform: 46 BC, the "Year of Confusion" — 445 days to realign the calendar. Sosigenes the Alexandrian astronomer. Foreign expertise standardizing Roman time.
- The canonical hours: the Rule of St. Benedict (c. 530 AD), eight liturgical hours. The monastery bell as the first standardized time signal.
- Casey's IBM era: the IBM 704 (1954-1960), crystal oscillator timing, core memory dumps. "Mrs. Patterson's social security number" — the census data that the machines were originally built to process.
- Augustine on time: *Confessions* XI.14 — "If no one asks me, I know. If I wish to explain, I do not know." Casey's gap between experiential knowledge and formal definition.

### The Casey Flashback Visual Vocabulary

Each era has a distinct visual language:
- **1858 (First Death fragment):** Amber light, leather, Highlander cold-open energy. 5 seconds.
- **1911 (Taylor):** Sepia-toned, period lecture hall, industrial formality. 60-90 seconds.
- **1957 (IBM):** Warm Technicolor. White shirts, punch cards, the hum of mainframes. 90-120 seconds.
- **2019 (Web3):** Washed-out blue. Standing desks, branded hoodies, a whiteboard of buzzwords. 90-120 seconds.

The hand motif connects them: cobbler's hands on leather (fragment) → hands on printed program (Taylor) → hands on printout (IBM) → hands erasing whiteboard (Web3) → hands on Tempus code (present). The rhyme lands without commentary.

---

*Compiled from A-Plot Technical Pitch, Character & Flashback Pitch, Serialized Threads Pitch, Genre Fan Pitch, and Historian Pitch per the Compilation Plan decisions document. All open questions resolved per showrunner approval.*
