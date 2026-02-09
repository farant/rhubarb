# 1x08 "Horologium" (The Clock) — A-Plot Technical Pitch
## The Dramatic Engine, Act Structure, and On-Screen Code

---

## THE DRAMATIC ENGINE

### The Problem

Carter needs timestamps. Everything the team saved to disk in 1x07 — the census, the encyclopedia, the vocabulary — has no date. "A record without a date is evidence without a witness," Marcus said. But timestamps require a clock. And clocks require agreement — a shared, reliable source of truth about when things happen.

Casey builds the Tempus library. He creates a global clock — `tempus_initiare()`. He creates independent timers — `tempus_creare(piscina)`. He creates frame-rate management — `tempus_ponere_quadra_destinata()`, `tempus_exspectare_quadrum()`. He creates easing functions for smooth animation — `lenire_cubicus_egressus()`, `lenire_elasticus_ingressus()`. He creates wave generators — `unda_sinus()`, `unda_triangulus()`. He creates interpolation — `interpolare()`, `gradus_lenis()`. He creates spring physics — `elastrum_ad()`.

Everything works. The API is clean. The tests pass. Casey is good at this — thirty years of building clean systems alone.

Then Carter asks him to synchronize two independent clocks.

And the clocks disagree.

Not by much. Not in a way that indicates a bug. They drift. One clock runs slightly fast, the other slightly slow. Casey resets them, re-synchronizes. They agree for a moment. Then they drift again. The drift is not constant — it oscillates. Sometimes Clock A leads. Sometimes Clock B leads. The pattern shifts.

Casey, the precision engineer, the man who spent thirty years at IBM building systems where microseconds mattered, cannot make two clocks on the same planet agree with each other.

This is Midas's temporal effect. The temporal distortion that Carter has been tracking since 1x05 — the compass spinning, the periodic frequency she discovered in 1x07 — is warping time itself on Nova Roma. Not much. Not enough to notice with a wristwatch. But enough that high-resolution clocks, measuring in nanoseconds, detect the difference. `fenestra_tempus_obtinere_pulsus()` returns a hardware counter. The hardware counter is accurate to the physical oscillations of the processor's crystal. If time itself is non-uniform in this region of spacetime, the counter is faithfully recording a warped signal.

The clocks aren't lying. They're telling the truth about a reality where time doesn't flow evenly.

### Why This Matters Dramatically

This is Casey's episode. The man who has been alone for thirty years because the world stopped building things properly. The man who left Web3 because the foundations were rotten. The man who came to Nova Roma because here, finally, people build from the ground up.

And now the ground itself is shifting.

Casey's identity is precision. His flashback (handled by the character writer) will show us why. But in the A-plot, we see it in real time: Casey builds the most elegant time library you've ever seen, and it doesn't work. Not because of a bug. Not because of a bad API. Because the planet is lying about what time it is.

This forces Casey's first moment of collaboration under duress. He can't solve this alone. He needs Carter's physics, Data's analytical capabilities, and — in a quiet scene — Marcus's pragmatic engineering perspective. Casey, the hermit, has to ask for help. Not because he's not good enough. Because the problem is bigger than one person.

The dramatic engine is not "Casey builds a clock." It's "Casey discovers that even perfect engineering fails when your assumptions about reality are wrong." And the assumption here is the deepest one in all of computing: that time moves uniformly.

### The Dependency Chain Continues to Branch

```
Piscina (1x01) — arena allocation
  -> Chorda (1x03) — strings
    -> ChordaAedificator (1x03) — string builder
    -> Filum (1x07) — file I/O
  -> Tempus (1x08) — time management
```

Tempus depends on Piscina (independent timers are arena-allocated via `tempus_creare(piscina)`) but NOT on Chorda, Filum, Tabula Dispersa, or Internamentum. The dependency tree branches again — Tempus is a sibling to Filum, not a child. The architecture grows wider, not just taller. This is the second branching after 1x07.

---

## COLD OPEN

**Location:** Casey's workshop. Before dawn. The room is spare — tools organized with military precision. A single terminal. Casey's crucifix on the wall (established 1x02). His rosary on the desk beside the keyboard (the faith thread, accumulating).

Casey is writing code. Not for the team. For himself. He does this — gets up early, works alone, builds the next thing before anyone asks. It's who he is. The man who has been building alone for thirty years doesn't stop just because he has a team now.

On screen, we see him type:

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

/* Exspectare unam secundam */
dum (tempus_elapsum(horologium) < 1.0)
{
    /* nihil */
}

f64 elapsum = tempus_elapsum(horologium);
imprimere("Elapsum: %f secundae\n", elapsum);
```

`Elapsum: 1.000017 secundae`. One second, plus seventeen microseconds of overhead. Precise. Clean. Casey allows himself the smallest smile. This is what he does.

He resets the timer:

```c
tempus_restituere(horologium);
f64 post_restitutionem = tempus_elapsum(horologium);
/* post_restitutionem ≈ 0.000002 */
```

Two microseconds. The timer resets cleanly to zero. He writes a comment in the code:

```c
/* Horologium purum. Sicut debet esse. */
/* A clean clock. As it should be. */
```

Then he opens Carter's file — the temporal data she saved to disk in 1x07. The compass readings. The oscillation patterns. The periodic frequency she discovered. He reads the numbers. His expression changes. Not alarm — interest. A craftsman recognizing that the next problem is harder than the last.

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

The first time we've seen Casey put his name on something. The hermit is claiming his work. Small moment. Big shift.

SMASH TO MAIN TITLES.

---

## ACT ONE: "Pulsus" (The Pulse)

*The need for time. Casey presents the library. The first on-screen frame loop. Easing functions as philosophy.*

### Beat 1: Carter's Request

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

1. Horologium Globale  — one clock, shared by all
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

### Beat 2: The First Frame Loop

Casey builds the frame loop live. This is the heart of real-time computing — the rhythm that drives everything.

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

Casey looks at Marcus with genuine surprise. It's the first time a Roman has given him an analogy he hadn't considered.

**CASEY:** ...That's exactly right. The variable hour. Delta time is the variable hour.

### Beat 3: Easing Functions — The Philosophy of Motion

Casey demonstrates easing. This is where the library moves from infrastructure to expression.

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
  × ingressus, egressus, ingressus_egressus
```

**CASEY:** Twenty-four easing curves. Eight families, three variants each. Ingressus — ease in, starts slow. Egressus — ease out, ends slow. Ingressus-egressus — both.

**DANIEL:** *(the linguist)* *Lenire*. To soothe. You're soothing the motion. Removing the harshness of linearity.

**CASEY:** *(a small nod)* The name is Lucia's suggestion. *(a beat — Lucia, somewhere in the room, barely acknowledges this. She names things. It's what she does.)* And it's the right word. Easing IS soothing. You're making movement kind.

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

**CASEY:** *(to the room)* The spring is the most honest physical model. It has momentum — the velocity pointer carries state between frames. It responds to change — move the target, the spring follows. It settles — the damping dissipates energy.

**MARCUS:** *(who has been watching intently)* This is the principle of the arch. An arch under load deflects. If the load increases, it deflects further. If the load decreases, it returns. The stone is the stiffness. The mortar is the damping. *(beat)* Your spring is an arch that moves.

Casey stares at Marcus for a beat. Then:

**CASEY:** I've been building these systems for sixty-seven years. Nobody has ever described spring physics to me in terms of an arch. *(beat)* That's better than my explanation.

Carter notices. Casey just accepted someone else's framing. The hermit is learning.

---

## ACT TWO: "Unda" (The Wave)

*Wave functions. The frame-rate statistics. Carter uses Tempus to analyze her temporal data. The first hint that something is wrong.*

### Beat 5: Waves as the Language of Periodicity

Casey demonstrates the wave functions:

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

**CASEY:** Noise. Pseudorandom values from time. Not a wave — the absence of one. The thing waves are measured against.

**DATA:** *(studying the noise function)* Deterministic noise. The same input always produces the same output. It appears random but is entirely predictable. *(beat)* A useful illusion.

### Beat 6: The Statistics Engine — Measuring the Machine's Own Heartbeat

Casey shows the frame statistics:

```c
StatisticaeQuadri* stats = tempus_obtinere_statisticas();

imprimere("Quadra destinata: %.1f QPS\n",
          stats->quadra_destinata_per_secundum);
imprimere("Quadra currentia: %.1f QPS\n",
          stats->quadra_currentia_per_secundum);
imprimere("Quadra media:     %.1f QPS\n",
          stats->quadra_media_per_secundum);
imprimere("Minimum:          %.4f sec\n",
          stats->tempus_minimum_quadri);
imprimere("Maximum:          %.4f sec\n",
          stats->tempus_maximum_quadri);
imprimere("Quadra totales:   %d\n",
          stats->numerus_quadrorum);
```

```
Quadra destinata: 60.0 QPS
Quadra currentia: 60.1 QPS
Quadra media:     59.9 QPS
Minimum:          0.0162 sec
Maximum:          0.0171 sec
Quadra totales:   3847
```

**CASEY:** The machine measures itself. Target: sixty frames per second. Current: sixty point one. Average: fifty-nine point nine. The minimum frame took sixteen point two milliseconds. The maximum took seventeen point one. That's a variance of less than one millisecond across three thousand eight hundred and forty-seven frames.

**TEAL'C:** *(studying the numbers)* Consistent performance. The machine is reliable.

**CASEY:** The machine is *predictable*. That's the prerequisite for reliable timing. If you can't predict how long each frame takes, you can't pace anything.

**CARTER:** *(from the side, a thought forming)* Casey — can I borrow your wave functions?

### Beat 7: Carter's Analysis — The Periodic Frequency

**Location:** Carter's workstation. She's alone — or thinks she is. She opens her temporal data file, the one she saved in 1x07. The compass readings. The oscillation patterns. She's been measuring Midas's temporal effect by hand, with intermittent readings, writing them in a notebook. In 1x07 she saved them to disk and noticed a periodicity — not random, structured.

Now she has Casey's wave library. She can analyze.

```c
/* Legere datas temporales ex disco */
chorda data = filum_legere_totum("anomalia_temporalis.txt", piscina);

/* Comparare cum unda sinus */
per (i = ZEPHYRUM; i < numerus_punctorum; i++)
{
    f32 observatum = puncta[i];
    f32 praedictum = unda_sinus(
        (f32)i * intervallum,
        frequentia_coniecta,  /* estimated frequency */
        amplitudo_coniecta);  /* estimated amplitude */

    f32 error = observatum - praedictum;
    error_totalis += error * error;
}
```

She's fitting a sine wave to her compass data. The error decreases as she adjusts the parameters. The fit improves. She finds the frequency.

```
Frequentia optima: 0.0037 Hz
Periodus: 270.3 secundae (~4.5 minutae)
Amplitudo: 0.0042
R²: 0.94
```

The temporal distortion oscillates with a period of four and a half minutes. The sine fit is 94% — almost perfect. This is not noise. This is a signal.

Carter stares at the numbers. Then she types one more thing:

```c
/* Comparare cum unda quadratus */
f32 praedictum_q = unda_quadratus(
    (f32)i * intervallum,
    0.0037f,
    amplitudo_coniecta);
```

The square wave fit is worse — 71%. This is definitely sinusoidal, not binary. Not on-off. Smooth oscillation. Like breathing.

**CARTER:** *(to herself, very quiet)* It has a pulse.

She doesn't share this. She saves the analysis to disk — using the Filum library she built yesterday. The data goes from volatile analysis to persistent evidence. Another file in a growing secret folder.

This scene uses three libraries from three episodes: Filum (1x07) to read the data, Tempus wave functions (1x08) to analyze it, and the piscina (1x01) to allocate the buffers. The dependency chain in action — not as a lecture, but as a character doing her job.

### Beat 8: ACT BREAK — Interpolation and Color

Casey shows interpolation:

```c
/* Interpolare — valor inter duos fines */
f32 medio = interpolare(0.0f, 100.0f, 0.5f);
/* medio = 50.0 */

/* Gradus lenis — transitus sine asperitate */
f32 t = gradus_lenis(100.0f, 200.0f, 150.0f);
/* t ≈ 0.5, sed leniore quam linearis */

/* Cohibere — continere intra limites */
f32 securum = cohibere(valor, 0.0f, 1.0f);
/* numquam infra 0, numquam supra 1 */
```

**CASEY:** Three tools for working between values. Interpolation finds the point between two known values. Smoothstep does the same thing but with smooth entry and exit — no abrupt changes. Clamping keeps a value within bounds.

**MARCUS:** *(nodding)* The level. The plumb line. The compass. Three tools for ensuring a structure is true. *(he lists them)* Interpolation finds the midpoint — where the keystone goes. Smoothstep is the mortar curve — gradual compression, no cracks. Clamping is the boundary wall — nothing goes past the edge.

And color interpolation:

```c
/* Interpolare colorem — transitus inter colores */
i32 ruber = 0x000000FF;   /* ruber */
i32 caeruleus = 0x00FF0000; /* caeruleus */

i32 purpureus = interpolare_colorem(ruber, caeruleus, 0.5f);
/* purpureus — medio inter rubrum et caeruleum */
```

On screen: a smooth gradient from red to blue. The color transition is seamless.

**PLINY:** *(watching the gradient, delighted)* The colors of dawn. The sky does not change from black to blue instantaneously. It interpolates. *(he writes furiously)* The machine can describe dawn.

---

## ACT THREE: "Duo Horologia" (Two Clocks)

*Casey creates independent clocks. Carter asks him to synchronize them. The clocks disagree. The mystery begins.*

### Beat 9: Independent Clocks

Casey introduces independent timers:

```c
/* Creare duo horologia independentia */
Tempus* horologium_A = tempus_creare(piscina);
Tempus* horologium_B = tempus_creare(piscina);

/* Ambo initiata eodem momento */
```

**CASEY:** Two clocks. Created from the same arena. Initialized at the same instant. They should agree.

He lets them run. After ten seconds:

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

**CASEY:** Four nanoseconds of difference after ten seconds. That's crystal oscillator jitter. Normal. Acceptable.

**CARTER:** Can you run them longer?

### Beat 10: The Drift

Casey lets the clocks run. He creates a visualization — a simple animation using his own easing functions. Two dots, one for each clock, moving across the screen at a rate determined by their respective `tempus_elapsum()` values.

He uses `animare_ad()` to smooth the motion:

```c
f32 positio_A = animare_ad(
    positio_A,
    (f32)tempus_elapsum(horologium_A) * 10.0f,
    200.0f,
    delta);

f32 positio_B = animare_ad(
    positio_B,
    (f32)tempus_elapsum(horologium_B) * 10.0f,
    200.0f,
    delta);
```

For the first few minutes, the dots move together. Perfectly synchronized. Casey is satisfied.

Then, around minute three, one dot pulls ahead. By a pixel. Then two pixels. Then it falls back. The other dot leads. Then falls back. The dots are oscillating around each other.

**CASEY:** *(staring at the screen)* That's not jitter.

He writes a more precise measurement:

```c
per (quadrum = ZEPHYRUM; quadrum < 36000; quadrum++) /* 10 minutae */
{
    tempus_actualizare(horologium_A);
    tempus_actualizare(horologium_B);

    f64 diff = horologium_A->tempus_totale
             - horologium_B->tempus_totale;

    /* Salvare differentia ad filum */
    filum_scriptor_scribere_literis(scriptor, ...);
}
```

He plots the differences. On screen: a curve. Not random. Not noise. A sine wave. The two clocks disagree in a sinusoidal pattern.

**CASEY:** *(very quiet, the precision engineer confronted with imprecision)* That's a waveform. The clocks are drifting in a periodic pattern.

**CARTER:** *(who has gone very still)* What's the frequency?

Casey measures:

```c
/* Comparare cum unda sinus */
f32 freq_drift = 0.0037f;
```

**CASEY:** Point zero zero three seven hertz. Period of about two hundred seventy seconds. Four and a half minutes.

Carter says nothing. But the audience has seen this number before. In Beat 7. Carter's compass data. The temporal anomaly. *The same frequency.*

**O'NEILL:** *(from the back of the room, sensing the silence)* Is that bad?

**CASEY:** *(turning to look at Carter)* The clocks shouldn't drift at all. They're counting the same crystal oscillations from the same hardware. *(beat)* Something is modulating time itself.

### Beat 11: Casey's Frustration

Casey tears the system apart. He tries everything:

```c
/* Probare: restituere et re-synchronizare */
tempus_restituere(horologium_A);
tempus_restituere(horologium_B);
/* ...drift redire post ~180 secundae */

/* Probare: unum horologium, duo lecturae */
f64 A1 = tempus_elapsum(horologium_A);
f64 A2 = tempus_elapsum(horologium_A);
/* A2 - A1 ≈ 0.000000002 — normalis */

/* Probare: horologium globale contra independens */
f64 globalis = tempus_nunc();
f64 independens = tempus_elapsum(horologium_A);
/* ...etiam hic drift sinusoidalis! */
```

Even the global clock and the independent clock disagree. The drift is universal. Everything that reads time on this machine shows the same sinusoidal modulation.

**CASEY:** *(the frustration of a craftsman whose tools are reporting impossible things)* The hardware is fine. The code is correct. The oscillator is accurate. I've verified every path. *(beat)* The clocks are right. They are faithfully reporting what the hardware tells them. And the hardware is faithfully counting what the crystal tells it. And the crystal...

He stops.

**CASEY:** The crystal is faithfully oscillating in local spacetime. If spacetime itself is non-uniform...

**DATA:** *(completing the thought)* Then the clock is accurate. It is time that is inconsistent.

Silence.

**CASEY:** *(sitting down heavily — the man who builds clean systems, confronted with a universe that isn't clean)* The clock isn't broken. The planet is.

### Beat 12: ACT BREAK — Casey and Carter

Carter pulls Casey aside. Private.

**CARTER:** I need to show you something.

She opens her file. The compass data. The periodic frequency. 0.0037 Hz. The sine fit.

**CASEY:** *(looking at the numbers)* That's the same frequency.

**CARTER:** I've been tracking this since 1x05. The compass anomaly near Midas. It wasn't random — it was periodic. I saved the data in 1x07 and discovered the frequency. And now your clocks are showing the same period.

**CASEY:** *(slowly)* The compass and the clocks are detecting the same phenomenon.

**CARTER:** Whatever is distorting the compass is also distorting time. *(beat)* It's centered on Midas.

Casey absorbs this. The craftsman's mind works differently from the physicist's — he doesn't think in theories, he thinks in tolerances.

**CASEY:** Can I build a clock that works here?

**CARTER:** You already did. The clock works. It reports what's real. What's real is that time on Nova Roma oscillates.

**CASEY:** That's not acceptable.

**CARTER:** *(gently)* It's not a matter of accepting it. It's a matter of accounting for it. Your clock doesn't need to pretend time is uniform. It needs to be honest about what it measures.

This is the turning point. Casey has been trying to build a perfect clock. Carter is telling him the clock IS perfect — it's reality that's imperfect. The craftsman has to accept that precision means reporting the truth, not enforcing an ideal.

---

## ACT FOUR: "Circumvolvere" (To Wrap Around)

*Casey adapts. The library holds. The team builds timestamps on top of imperfect time. The utility functions become philosophical.*

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

This is enormous for Casey. The man who left Web3 because the foundations were rotten. The man who spent thirty years building clean systems. Now he's writing documentation that says "the foundation moves, and that's okay." Not because it's acceptable — because it's *true*. The documentation is honest. The clock is honest. The drift is real.

### Beat 14: Timestamps on Shifting Ground

Carter and Casey work together. They build timestamps on top of the Tempus library:

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

**CARTER:** The timestamp reports local time. It's accurate to local time. If local time oscillates, the timestamp oscillates with it.

**CASEY:** So two files saved four and a half minutes apart might show timestamps that differ by four minutes and twenty-eight seconds. Or four minutes and thirty-two seconds. Depending on where in the oscillation they were saved.

**CARTER:** Correct. But the *ordering* is preserved. File A was saved before File B. The sequence is correct even if the intervals are slightly warped.

**CASEY:** *(reluctantly)* Sequence is enough. For now.

**MARCUS:** *(who has been listening)* The Roman calendar has the same problem. The lunar months don't divide evenly into a solar year. We add intercalary days. We adjust. The date is approximate, but the sequence is correct — January is always before February. *(beat)* Your oscillating time is our intercalary problem. The solution is the same: record faithfully, adjust when you can, trust the sequence.

### Beat 15: Utility Functions — The Engineer's Wisdom

Casey demonstrates the utility functions. These are the quiet workhorses — functions that don't animate or oscillate but *constrain*.

```c
/* Circumvolvere — si excedis limitem, redis ad initium */
f32 angulus = circumvolvere(370.0f, 0.0f, 360.0f);
/* angulus = 10.0 — 370° wraps to 10° */

/* Oscillare — ire et redire sine fine */
f32 valor = oscillare(t, 100.0f);
/* 0→100→0→100→0... */

/* Remappare — transferre valorem ex uno campo ad alium */
f32 normalis = remappare(150.0f, 100.0f, 200.0f, 0.0f, 1.0f);
/* normalis = 0.5 — 150 is halfway between 100 and 200 */
```

**CASEY:** Circumvolvere — wrap. When an angle goes past three-sixty, it wraps back to zero. Not clamped — *wrapped*. The journey continues in the same direction but the counter resets.

**MARCUS:** The sundial. At noon the shadow reaches the bottom. It does not stop. It wraps back to the beginning and starts again.

**CASEY:** Oscillare — oscillate. Back and forth between zero and a value. A pendulum. A breath. *(beat)* And remappare — remap. Take a value from one scale and express it in another. Convert a measurement. Translate a coordinate. *(quieter)* See the same thing from a different perspective.

**METHOS:** *(from his pillar, the first time he's spoken this episode)* That last one. Remapping. *(he swirls his wine)* That's what translation is. You take a meaning from one language and express it in another. The value is the same. The scale changes. *(beat)* I've been doing that for five thousand years. Nice to see a function for it.

### Beat 16: ACT BREAK — Teal'c's Test Suite

Teal'c has been testing throughout the episode:

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

**TEAL'C:** *(one observation)* I tested the clocks extensively. Each clock, in isolation, is precise. The drift you observed between clocks is not a defect of the clocks. It is a property of the system they are measuring.

**CASEY:** *(quietly)* Yeah. I know.

**TEAL'C:** *(with the weight of a man who understands unjust systems)* The test that matters is not whether the clock agrees with another clock. It is whether the clock is honest about what it observes. Your clocks are honest. The disagreement is between them and the universe. *(beat)* That is not a failing of the clocks.

---

## ACT FIVE: "Horologium Verum" (The True Clock)

*The library is complete. The timestamps work. Casey names what he's built. The title earns itself.*

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

### Beat 18: The Title Moment

**Location:** The Tabularium. Late afternoon. The team gathered.

Carter writes on the board:

```
Dependency tree (season so far):

Piscina (1x01)
  ├── Credo (1x02) — testing
  ├── Chorda (1x03) — strings
  │     ├── ChordaAedificator (1x03)
  │     ├── TabulaDispersa (1x05) — hash table
  │     │     └── Internamentum (1x06)
  │     └── Filum (1x07) — file I/O
  └── Tempus (1x08) — time ←
```

**CARTER:** Eight episodes. Eight libraries. The tree has three branches now. Tempus stands alongside Filum and the hash table chain — it shares the arena, but it doesn't need strings or files or hashing. It's a different kind of foundation.

**CASEY:** *(looking at the dependency tree — his library, his branch)* Every library so far has been about *what*. What to store, what to look up, what to save. Tempus is about *when*. It's the first library that exists in time rather than in space.

**MARCUS:** *(standing)* An horologium. *(he writes the word)* In Rome, the horologium is not merely a device for telling time. It is a public institution. The Horologium Augusti — the sundial of Augustus — stood in the Campus Martius. It was the largest sundial in the world. The shadow of an Egyptian obelisk, one hundred feet tall, falling across a marble floor inscribed with the signs of the zodiac and the days of the year. *(beat)* It was not a clock for private use. It was a clock for the city. The public measurement of time — shared, visible, authoritative.

He turns to the board.

**MARCUS:** Your `tempus_nunc()` — "what time is it now?" — is the Horologium Augusti. One answer, shared by all. Your `tempus_creare()` — a private clock, allocated from the arena — is the personal sundial. For private measurements, private durations, private rhythms. *(beat)* A civilization needs both. The public clock and the private timer.

**CASEY:** *(quietly)* And when the public clock and the private timer disagree?

**MARCUS:** *(a long beat)* Then you have discovered something about the nature of time in this place. *(beat)* A Roman horologium that showed a different time from the stars would tell us something about the horologium, or about the stars, or about the nature of the place where it stood. Yours tells you about the nature of Nova Roma.

**CASEY:** *(looking at Marcus with something approaching gratitude)* You're saying the drift is data.

**MARCUS:** I'm saying the drift is evidence. A clock that tells you time is strange in this place is more valuable than a clock that pretends time is normal.

Casey writes on the board:

**HOROLOGIUM**

**CASEY:** The clock. Not a timekeeping device — a truth-telling device. It tells you what time does here. And what time does here is... complicated.

### Beat 19: The Test Visualization

Casey builds a final demo that uses nearly every feature of Tempus. On screen:

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

---

## TAG

### Beat 20: Casey and Carter — The Evening Walk

Casey doesn't usually walk with Carter. That's Carter and Marcus's ritual. But tonight Carter finds Casey on the road, walking alone.

**CARTER:** You built a beautiful library.

**CASEY:** I built a clock that tells you time is broken.

**CARTER:** You built a clock that tells you what time *does*. That's more useful than a clock that lies.

Silence. Walking.

**CASEY:** At IBM, we had a saying. "The machine doesn't make mistakes." Whatever it reports, that's what happened. If the result surprises you, the surprise is in you, not the machine.

**CARTER:** And the drift surprised you.

**CASEY:** *(a long beat)* I've been building timing systems for sixty-seven years. Clocks agree. That's what clocks do. You calibrate them against a standard and they agree. *(beat)* I don't have a standard here. I have two clocks and a planet that oscillates. What do I calibrate against?

**CARTER:** The data. *(she looks at him)* You calibrate against what's real. Even when what's real is strange.

**CASEY:** *(very quiet)* Midas. You said it's centered on Midas.

**CARTER:** The compass readings. The clock drift. They share the same frequency. The same waveform. And the strongest readings are always near Midas.

**CASEY:** *(after a long silence)* He doesn't know, does he?

**CARTER:** No.

**CASEY:** *(walking, the weight of it settling)* A man who warps time just by existing. And we're building a clock on his planet.

**CARTER:** We're building a clock *despite* his planet. And the clock works. It's honest. That's enough for now.

Casey stops. Looks up at the stars — different constellations than Earth. A different sky. A different time.

**CASEY:** *(quietly)* Next, we need to structure the data. Everything we're saving is flat text. The census is lines in a file. Pliny's encyclopedia is lines in a file. But the data *has* structure — hierarchy, categories, relationships. We need a way to express that. *(beat)* XML.

**CARTER:** Arbor documentorum. Tree of documents.

**CASEY:** *(a small nod)* The tree.

### Beat 21: Data and Casey — The Quiet Scene

**Location:** The workshop. Late. Data is reviewing the Tempus code. Casey returns.

**DATA:** Your code is elegant.

**CASEY:** *(surprised — he's not used to compliments)* It's functional.

**DATA:** It is elegant *because* it is functional. *(he indicates the easing functions on screen)* Twenty-four functions. No unnecessary state. No allocation. Each function takes a value from zero to one and returns a value from zero to one. Pure transformation. *(beat)* Captain Picard once showed me a watch. A mechanical watch. Gears, springs, escapement. He said: "The measure of its elegance is the absence of anything unnecessary." Your functions are the same.

Casey sits down. This is the first time anyone has appreciated his work at the level of craft, not just utility.

**CASEY:** The easing functions are the oldest part. I wrote the first versions in 1971. On an IBM System/370. Punch cards. *(beat)* The math hasn't changed. The sine function in 1971 is the same sine function today. That's the thing about math. It doesn't deprecate.

**DATA:** *(precise)* Unlike software libraries, mathematical functions have no version number. They are permanent. *(beat)* In the future I came from, the same easing curves are used in the twenty-fourth century. The implementation language changes. The mathematics endures.

**CASEY:** *(quietly)* That's... good to know.

A beat. Two builders — one immortal, one android — recognizing that the things they build from mathematics will outlast everything else.

FADE TO BLACK.

---

## STINGER

### Beat 22: Kirk's CHRONOGRAPHIA

**Location:** Earth. Night. A different room in the Synthesis Dynamics office — a monitoring station. Screens showing real-time data feeds. Timestamps scrolling. Kirk alone.

He's watching Nova Roma's temporal signature. He has equipment sophisticated enough to detect it from Earth — or rather, through the gate's telemetry data he's been siphoning.

On one screen: a sine wave. Period: ~270 seconds. Amplitude: tiny but consistent. The same wave Carter found. The same frequency Casey's clocks drift.

Kirk circles the frequency on the screen.

**KIRK:** *(to himself)* The anomaly has a pulse.

He opens a file. Handwritten — physical, not digital. Consistent with the filing-cabinet methodology from 1x07. The header reads: **MIDAS — TEMPORAL PROFILE**.

He writes one line: **Freq: 0.0037 Hz. Sinusoidal. Consistent. Biological?**

The question mark is the gut-punch. Kirk suspects what Carter suspects — that the temporal distortion is connected to Midas as a living being. That the distortion *breathes*.

He closes the folder. Opens another drawer. Pulls out a stopwatch — analog, mechanical. Winds it. Sets it next to a digital clock on the wall. The digital display reads 23:47:12. The stopwatch starts ticking.

**KIRK:** *(watching both clocks)* Two clocks. Let's see if they agree.

He writes on the whiteboard: **8 / 31**.

Below it: **CHRONOGRAPHIA**

Kirk stinger progression updated: TABULARIUM (1x05) → LEXICON (1x06) → ARCHIVUM (1x07) → CHRONOGRAPHIA (1x08). The words track Kirk's operational evolution: indexing → standardizing → persisting → *timing*. Kirk is building the same capabilities as the team, in shadow. The team built a clock. Kirk is building a timeline.

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
The heartbeat. Sixty frames per second. The rhythm of real-time computing.

### Moment 3: Easing Demonstration (Act 1)
```c
f32 lin = lenire_linearis(t);
f32 cub = lenire_cubicus_egressus(t);
f32 ela = lenire_elasticus_egressus(t);
```
Three dots. Three philosophies of motion. Mechanical, natural, expressive.

### Moment 4: Spring Physics (Act 1 Break)
```c
positio = elastrum_ad(positio, destinatum,
    &velocitas, rigiditas, damnum, delta);
```
The spring. Marcus's arch. Energy, momentum, settling.

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
/* ... timestamp salvatum ad filum ... */
```
1x07's files gain dates. The dependency chain in action: Filum + Tempus.

### Moment 10: The Full Demo (Act 5)
```c
x = elastrum_ad(x, dest, &vel, 10.0f, 0.5f, delta);
f32 pulsus = unda_sinus(t, 1.0f, 20.0f);
i32 color = interpolare_colorem(ruber, caeruleus, t);
StatisticaeQuadri* stats = tempus_obtinere_statisticas();
```
Everything together. Spring, wave, color, statistics. Beautiful. Unsettling.

---

## HOW EACH TEMPUS FEATURE MAPS TO A DRAMATIC MOMENT

| Tempus Feature | Dramatic Moment | Act |
|---|---|---|
| `tempus_initiare()` | Casey's dawn clock — the hermit building alone before anyone wakes | Cold Open |
| `tempus_nunc()` | "What time is it?" — the question that drives the episode | 1, throughout |
| `tempus_delta()` | The frame loop — Data immediately grasps decoupled timing | Act 1 |
| `tempus_quadrum()` | The heartbeat — frame management as the rhythm of computation | Act 1 |
| `tempus_ponere_quadra_destinata()` | 60 FPS — Casey sets the target, the machine meets it | Act 1 |
| `tempus_exspectare_quadrum()` | The hybrid sleep — waiting precisely for the next frame | Act 1 |
| `tempus_obtinere_statisticas()` | The machine measures itself — frame statistics | Act 2 |
| `tempus_creare(piscina)` | Two independent clocks — the experiment that reveals the mystery | Act 3 |
| `tempus_elapsum()` | The drift measurement — clocks disagree | Act 3 |
| `tempus_actualizare()` | Per-frame drift logging — 36,000 samples | Act 3 |
| `tempus_restituere()` | Casey resets, re-synchronizes — the drift returns | Act 3 |
| `lenire_linearis()` | Mechanical motion — the contrast | Act 1 |
| `lenire_cubicus_egressus()` | Natural deceleration — Marcus's cart | Act 1 |
| `lenire_elasticus_egressus()` | Overshoot and settle — Pliny's fascination | Act 1 |
| `lenire_resiliens_egressus()` | Bounce — implied in the spring demo | Act 1 Break |
| `elastrum_ad()` | Spring physics — Marcus's arch | Act 1 Break |
| `animare_ad()` | Smooth dot animation — the clock visualization | Act 3 |
| `interpolare()` | Finding the midpoint — Marcus's keystone | Act 2 Break |
| `gradus_lenis()` | Smooth transitions — the mortar curve | Act 2 Break |
| `cohibere()` | Clamping within bounds — the boundary wall | Act 2 Break |
| `interpolare_colorem()` | Dawn gradient — Pliny's delight | Act 2 Break |
| `unda_sinus()` | Carter's frequency analysis / the drift shape / Pliny's sea | Act 2, 3 |
| `unda_triangulus()` | Mechanical oscillation — visual contrast | Act 2 |
| `unda_dens_serrae()` | Accumulation and reset — sawtooth | Act 2 |
| `unda_quadratus()` | Binary: Carter rules out square-wave fit | Act 2 |
| `unda_tumultus()` | Noise — the absence of pattern | Act 2 |
| `circumvolvere()` | The sundial wrapping — Marcus | Act 4 |
| `oscillare()` | Pendulum — Casey's philosophical demo | Act 4 |
| `remappare()` | Translation — Methos's five-thousand-year perspective | Act 4 |
| `interpolare_angulum()` | Implied — angle wrap in clock visualization | Act 3 |

---

## THE CLOCK DISAGREEMENT MYSTERY — MIDAS CONNECTION

### What the Audience Knows

By the end of 1x08, the audience has these pieces:

1. **1x05:** Carter's compass spins near Midas. First anomaly detected.
2. **1x06:** More anomalies. Carter starts measuring.
3. **1x07:** Carter saves temporal data to disk. Discovers periodicity — 0.0037 Hz.
4. **1x08:** Casey's clocks show the SAME frequency. Carter confirms the compass and the clocks are detecting the same phenomenon. The distortion is centered on Midas.

### What the Characters Know

- **Carter:** Knows the frequency, the source (Midas), and that the distortion is temporal, not just magnetic. Has not shared with anyone except Casey (as of 1x08).
- **Casey:** Now knows the clocks drift because of a real temporal anomaly centered on Midas. He is the second person in the conspiracy of silence.
- **Data:** Suspects something is wrong with time on Nova Roma. His line — "It is time that is inconsistent" — shows he's deduced the general problem but doesn't know the source.
- **Midas:** Oblivious. Still joyful, still the social glue of Nova Roma.
- **Kirk:** Knows. Has the same data. This is the stinger's implication — Kirk is independently measuring the same phenomenon.

### The Science (Simplified for TV)

Midas's temporal artifact (the Black Stone of Cybele, from the 1x13 flashback) embedded a temporal distortion in his physiology. He radiates a field that slightly warps local spacetime — not enough to notice with human senses, but enough that high-precision instruments detect it. The distortion oscillates sinusoidally because it's coupled to Midas's biological rhythms — his heartbeat, his breathing, his circadian cycle. The 270-second period corresponds to a deep autonomic rhythm.

The clocks disagree because they are at different distances from Midas. Clock A, closer to Midas, runs slightly differently than Clock B, further away. The oscillation is the field's natural breathing. This will eventually be explained in the 2x11-13 Midas reveal arc. In 1x08, it's a mystery.

---

## KIRK STINGER PROGRESSION

| Episode | Number | Word | Kirk's Operation |
|---------|--------|------|-----------------|
| 1x01 | — | — | Arrival. Data/Kirk misdirect. |
| 1x02 | — | — | Exposed. Escapes. |
| 1x03 | — | — | Sightings. |
| 1x04 | 3/31 | — | Network building. |
| 1x05 | 5/31 | TABULARIUM | Whiteboards. Dark hash table. |
| 1x06 | 6/31 | LEXICON | Style guide. Dark interning. |
| 1x07 | 7/31 | ARCHIVUM | Filing cabinets. Dark file I/O. |
| **1x08** | **8/31** | **CHRONOGRAPHIA** | **Two clocks. Midas temporal profile. Dark timing.** |

Kirk's stinger mirrors the episode perfectly: the team discovers that time is strange on Nova Roma through building a clock. Kirk independently discovers the same thing through surveillance. Same data. Same frequency. Same question mark next to "biological?"

The word CHRONOGRAPHIA — "time writing," from the Greek — is Kirk naming the discipline of temporal surveillance. The team built an horologium (a clock for the city). Kirk is building a chronographia (a timeline for the target).

The operative count rises to 8/31. Kirk adds one operative per episode. By mid-season he'll have sixteen. By the finale, thirty-one — a full network.

---

## STRUCTURAL NOTES

### The Episode's Architecture

1. **Cold Open:** Casey builds alone. The craftsman at dawn. The hermit's ritual.
2. **Act 1:** Casey presents the library. Frame loops, easing, springs. The API as philosophy.
3. **Act 2:** Waves and analysis. Carter uses the tools to analyze Midas's temporal signature.
4. **Act 3:** Two clocks. The drift. Casey's crisis — the craftsman whose tools report impossible things.
5. **Act 4:** Acceptance. Casey documents the drift. Timestamps built on shifting ground. Utility functions.
6. **Act 5:** Completion. The full library. The title moment. The horologium as truth-teller.

### The Emotional Arc

Acts 1–2 are **mastery** — Casey demonstrating what he does best. Clean systems, precise engineering, elegant API design. The audience sees why this man is good at what he does.

Act 3 is **crisis** — the clocks disagree, and Casey cannot fix it because it's not broken. The craftsman who builds perfect things confronts a universe that isn't perfect.

Acts 4–5 are **acceptance** — not resignation, but a deeper kind of precision. A clock that tells you time is strange is more precise than a clock that pretends time is normal. Casey doesn't lower his standards. He expands them.

### Casey's Episode

This is Casey's spotlight. The API is his. The crisis is his. The resolution is his. What we learn about Casey:

- He works before dawn, alone. The thirty-year habit.
- He names his work. "Casey scripsit." The hermit claiming ownership.
- He knows more about timing than anyone alive. Sixty-seven years of experience.
- When his tools report something impossible, his instinct is to doubt the tools, not reality. Carter teaches him to trust the tools.
- He accepts collaboration reluctantly but genuinely. Marcus's arch analogy surprises and delights him. Data's compliment means something to him.
- He's the second person to learn about Midas. The conspiracy of silence gains a member.

### The Marcus Continuing Role

Marcus doesn't have a centerpiece scene like 1x04 or 1x07, but his contributions are consistently excellent:
- The variable hour (Beat 2) — delta time as seasonal Roman hours
- The arch (Beat 4) — spring physics as structural engineering
- The sundial (Beat 15) — `circumvolvere` as solar time
- The keystone, mortar, boundary wall (Beat 8) — interpolation as building technique
- "The drift is evidence" (Beat 18) — the horologium as truth, not accuracy

Marcus provides the Roman engineering lens through which the audience understands each concept. His analogies are not decoration — they are the bridge between computational abstraction and physical intuition.

---

## TECHNICAL FIDELITY NOTES

All code shown on screen uses the actual tempus.h API:

- `tempus_initiare()` — exact signature
- `tempus_nunc()` — exact signature, returns f64
- `tempus_delta()` — exact signature, returns f64
- `tempus_quadrum()` — exact signature
- `tempus_ponere_quadra_destinata(f64)` — exact signature
- `tempus_exspectare_quadrum()` — exact signature
- `tempus_obtinere_statisticas()` — returns `StatisticaeQuadri*`
- `tempus_creare(Piscina*)` — exact signature, returns `Tempus*`
- `tempus_elapsum(Tempus*)` — exact signature, returns f64
- `tempus_actualizare(Tempus*)` — exact signature
- `tempus_restituere(Tempus*)` — exact signature
- `lenire_linearis(f32)` through all 24 easing functions — exact signatures
- `elastrum_ad(f32, f32, f32*, f32, f32, f32)` — exact signature
- `animare_ad(f32, f32, f32, f32)` — exact signature
- `interpolare(f32, f32, f32)` — exact signature
- `interpolare_angulum(f32, f32, f32)` — exact signature
- `interpolare_colorem(i32, i32, f32)` — exact signature
- `gradus_lenis(f32, f32, f32)` — exact signature
- `cohibere(f32, f32, f32)` — exact signature
- `unda_sinus(f32, f32, f32)` through all wave functions — exact signatures
- `circumvolvere(f32, f32, f32)` — exact signature
- `oscillare(f32, f32)` — exact signature
- `remappare(f32, f32, f32, f32, f32)` — exact signature

The `Tempus` struct fields (`pulsus_initii`, `pulsus_currentis`, `frequentia`, `delta_tempus`, `tempus_totale`) and `StatisticaeQuadri` struct fields (`quadra_destinata_per_secundum`, `quadra_currentia_per_secundum`, `quadra_media_per_secundum`, `numerus_quadrorum`, `tempus_minimum_quadri`, `tempus_maximum_quadri`) are used correctly.

The internal implementation detail — `fenestra_tempus_obtinere_pulsus()` and `fenestra_tempus_obtinere_frequentiam()` as platform functions — is referenced in the clock-drift discussion but not shown in on-screen code (appropriate since these are internal).

Dependencies are accurate: Tempus includes `piscina.h` for independent timer allocation. The file I/O in Carter's analysis scene correctly uses the Filum API from 1x07.
