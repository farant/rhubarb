# sonus — the sound system (synthesis, mixing, output)

*Born 2026-10-20 in sexta, from a conversation with Fran while scoping
an arcade of games (a Tetris-like first) as vicus panes. Fran: "i like
breaking tasks down into general purpose modules first if they are
clear and adding some general purpose sound / synthesizing stuff might
be a good first slice"; "sound and music would eventually be made in
vicus"; "we want to lean towards being able to synthesize sounds
because that will just be the easiest way to make sound assets but
its likely we will have canned sound assets/music for certain things
at some point too"; "for aesthetic leaning towards chiptunes ... to
start ... but ... not necessarily ... the only aesthetic long term";
"a thread for realtime makes sense"; "think about it as designing a
greenfield sound system and wanting to take advantage of good design
patterns".*

*Status: design PROPOSED (this document), not yet approved. Library
names SEALED by Fran 2026-10-20 ("those names sound good"); the
decisions taken that day are in §X. Nothing is coded.*

*Discipline: the PULL rule - a layer is built when a named consumer
(§I) needs it, never to complete the table. Header text is shown to
Fran before code; tests red first with plants (`silva.planta`); the
real-time path and the test path are ONE code path (§IV.1).*

## I. Consumers (what pulls)

| # | consumer | what it needs first |
|---|---|---|
| 1 | **Claude notification sounds** in vicus, replacing the two `afplay` hooks (§II) - muteable as state | sound-effect voices, mixer, output, a preference field |
| 2 | **a theremin pane** - pitch and loudness follow the mouse | a LIVE voice whose parameters change while it sounds, smoothing, low latency |
| 3 | **the arcade** (Tetris-like first; event-sourced games) | scheduled sound effects; music; offline render of a replay |
| 4 | **UI sounds** (clicks, toasts) | buses (a UI bus to turn down), preferences |
| 5 | **sound-design and music panes** (the ludus principle: tools built from the blocks they author with) | patches as data, introspectable parameters, instruments, a sequencer |
| 6 | **canned assets** (recorded sounds, music) - later | file reading, resampling, a sample-player voice |

## II. What exists (cited)

- **Rhubarb has no audio at all.** No header or source names sound,
  audio or a synth (`ls include lib`, 2026-10-20; the only matches
  were `json` and `personae`).
- **Solarium's audio stack** (`../solarium`, commit cbf129a, already
  surveyed in `project-specs/solarium-comparatio.md:121,136`: "Near 1:1
  test ratio, small, pure - only `platform_audio` touches
  AudioToolbox"):
  - `synth.h/.c` (77 + 193 lines): sfxr-lineage one-voice engine, 20
    knobs, named presets = complete knob settings, offline render to a
    mono float buffer, same params + seed = identical buffer, seamless
    loops by tail crossfade (`synth_render_loop`).
  - `mixer.h/.c` (69 + 77): 24 voices, constant-power pan, fixed-size
    command (START / STOP / SET / REVERB), generation stamps ignore
    stale commands, stereo render + clamp.
  - `reverb.h/.c`: Freeverb (8 combs, 4 allpasses), mono tail.
  - `wav.h/.c` (18 + 58): mono float -> 16-bit PCM WAV writer.
  - `platform_audio.h/.c` (38 + 134): CoreAudio default output unit;
    the render callback drains a lock-free SPSC ring of commands
    (C11 atomics, quarantined to that file) and calls `mixer_render`.
  - `sounds.stml`: per-preset knob overrides, re-minted on save.
  - Tests: `synth_test.c` 272, `mixer_test.c` 206 lines.
- **The house has no threads by design**: `include/processus.h:181`
  ("domus filis CARET (nullum pthread in lib/)"),
  `lib/processus.worklog.md:76`. §IV.1 makes the ONE exception, for
  the domain reason in §III.
- **Randomness**: `include/sors.h` - seeded, deterministic PCG32,
  `sors_miscere` (shuffle), `sors_derivare` (child streams).
- **Files**: raw `fopen` outside `filum` is examen code 96; WAV I/O
  goes through `filum`.
- **The notification sounds today**: `~/.claude/settings.json:55`
  (`afplay /System/Library/Sounds/Ping.aiff` on Notification,
  matcher `permission_prompt`) and `:75`
  (`afplay /System/Library/Sounds/Glass.aiff` on Stop). Not muteable
  except by editing the file.
- **Vicus already follows those events**: hooks call
  `~/.bin/praeco hamus`, which appends to the praeconium log
  (`include/praeconium.h`); vicus's host system `admonitiones`
  (`lib/vicus_applicatio.c` `systema_admonitionum`) reads new events on
  every pulse and writes the root field `admonitio_nova`
  (`apps/vicus/organum.stml`). `PraeconiumEventus.eventus` =
  `hook_event_name`, `.genus` = `notification_type` / `trigger`.
- **Input for the theremin**: `EVENTUS_MUS_MOTUS`, key down / up
  (`include/eventus.h:38-42`).
- **Float type**: `f32` (`include/latina.h:4081`).

## III. The domain in brief (why it is shaped this way)

Digital audio is a stream of samples (here 48,000 per second per
channel) that the device consumes at a fixed rate and NEVER waits
for. A block not ready in time is an *underrun*: an audible click. So
audio has a hard real-time deadline, unlike graphics, where a late
frame is merely dropped.

Every audio system answers "who produces the next block, and how is
it on time?" The trade-off is latency against safety: a few ms queued
ahead feels instant but clicks on any stall; 100 ms is safe but laggy
(about 50 ms is noticeable for sound effects, much less for playing an
instrument - the theremin).

The families, for reference:

| family | shape | examples |
|---|---|---|
| callback (pull) | the OS calls you on a real-time thread: fill N frames now | CoreAudio AudioUnit, PortAudio, SDL2 callback, miniaudio, JACK |
| queue (push) | you submit filled buffers; latency = what you queued | SDL3 streams, OpenAL queues, XAudio2, AudioQueue |
| engine / graph | voices, buses, effect nodes, events by name, scheduling in audio time | Web Audio, FMOD, Wwise, SoLoud |
| fixed channels | a few hardware-like channels set per frame; music as patterns | NES APU, Game Boy, PICO-8, trackers (MOD/XM, Famitracker) |

How a sound is MADE is a separate axis: sample playback, procedural
sound effects (sfxr), subtractive (oscillator -> filter -> amplifier
with envelopes), FM, wavetable, modular graphs.

The callback family is the industry standard for anything that plays
an instrument or must not glitch while the program does slow work -
and vicus does slow work (SQLite commits, file loads, layout). The
rules of that thread are strict and well known (Ross Bencina, "Real-
time audio programming 101: time waits for nothing"): no locks, no
allocation, no I/O, nothing that can block.

## IV. Doctrine (PROPOSITUM)

1. **Pure core, one quarantined real-time edge.** Synthesis, mixing
   and sequencing are pure C89 over caller memory. ONE platform file
   (`auditus_macos.m`, §VII) owns the real-time thread; its callback
   drains a ring and calls the SAME render function the tests call.
   Nothing above the seam knows a thread exists. (We never create the
   thread: CoreAudio owns it and calls us on it - no `pthread_create`
   anywhere in the house.) The only non-C89 operations in the whole
   system are two memory-ordering primitives behind the `atomus` seam
   (§X.7).
2. **Two rings.** Commands main -> audio; REPORTS audio -> main
   (voice ended, buffer released, meters, underruns). The audio thread
   never frees: it reports, and the main thread frees. (Solarium had
   only the first ring and relied on session-lived buffers.)
3. **Time is in frames, and commands are scheduled.** Every command
   carries a frame timestamp ("start at frame n"), never "when it
   arrives". The main thread reads the audio clock and schedules a
   little ahead. Result: sample-accurate music, effects that do not
   jitter with the frame rate, and DETERMINISM - the same command list
   renders the same samples offline or live (Web Audio's model).
4. **The real-time path never allocates, locks or blocks.**
   Preallocated voice pool, fixed-size commands, assets immutable once
   handed over.
5. **Handles, not pointers.** A playing sound is (slot, generation); a
   command aimed at a stale handle is ignored, so "stop that" after it
   ended needs no bookkeeping.
6. **One voice interface.** A voice is anything that fills a buffer
   from its own state: a sample player, a procedural sound-effect
   voice, an instrument note. The mixer knows only the interface. Live
   synthesis and canned assets are two kinds of voice; new kinds slot
   in.
7. **A fixed bus tree first.** Voices -> buses (effects, music, UI,
   ...) -> master; each bus with gain, mute and a short insert chain.
   The FMOD/Wwise mixing model. A general node graph is not precluded
   by the interfaces and is not built (PULL).
8. **Sounds are data.** A patch (sound-effect knobs, an instrument) is
   STML with a canon; a named preset is a complete patch; a reference
   is preset + overrides (solarium's "types are presets"). Same patch
   + same seed = identical sound; a seeded jitter knob varies each
   trigger. The parameter schema is introspectable, because the
   sound-design pane builds its controls from it.
9. **The aesthetic is parameters, not architecture.** Chiptune
   character (pulse and triangle waves, bit-crush, sample-hold, a
   channel-limited sequencer mode) is knob settings and an optional
   constraint; the engine is f32 at 48 kHz throughout. Naive
   oscillators alias at high pitch by design (solarium: "folded
   harmonics are the crunch"); band-limited oscillators are a later
   option for other aesthetics, not a different engine.
10. **Parameters ramp.** Gain, pan, pitch and cutoff changes glide over
    a few ms (no "zipper" clicks). The theremin depends on it.
11. **Observable without printf.** Meters, voice counts, underrun and
    dropped-command counters come back on the report ring - for tests,
    for panes, for a mixer view later.

## V. Architecture

```
 panes / host (vicus main thread - single-threaded as today)
   games · theremin · notification sounds · UI · sound-design pane
        │ ansae (handles), iussa scheduled in frames
        ▼
 cantus (sequencer)  - notes in time -> iussa          [pure]
        ▼
 mixtura (mixer)     - voices, buses, iussa, nuntii, render  [pure]
   ▲  voices: sonitus (sfx) · tibia (instrument note) · sample player
   │  built from sonus primitives                       [pure]
   │
 ═══════════════ the seam (auditus.h) ═══════════════
 auditus_macos.m     - AudioUnit callback thread; ring in, ring out;
                       calls mixtura_reddere            [quarantined]
   uses: circulus (SPSC ring, pure C89)
           └─ atomus (publish-release / read-acquire)   [per compiler]
```

| layer (provisional name) | contents | pure |
|---|---|---|
| **sonus** - primitives | oscillators (pulse with duty, saw, triangle, sine, noise from `sors`), envelope (ADSR, `involucrum`), one-pole + biquad filters, crush / sample-hold, parameter ramps | yes |
| **sonitus** - sound-effect voice | the sfxr-lineage knob set (solarium's 20) re-expressed on sonus; presets; offline render + loop render | yes |
| **tibia** - instrument | a patch that plays notes (pitch, velocity, gate): oscillators, filter, envelopes; chiptune patches first | yes |
| **mixtura** - mixer | voice pool, handles, buses + inserts, scheduled commands, reports, `render` | yes |
| **cantus** - sequencer | time-stamped note events -> mixer commands; patterns (tracker-like) as STML later; channel-constraint mode | yes |
| **WAV I/O** | write first (audition, goldens, replay export); read + resample when canned assets arrive; through `filum` | I/O |
| **auditus** - output seam | open / start / stop / latency / clock; macOS = AudioUnit callback thread + two SPSC rings | quarantined |
| **circulus** - SPSC ring | fixed-size slots, one writer and one reader, never blocks, never allocates after creation; general-purpose (not sound-specific) | yes, except two calls into atomus |
| **atomus** - ordering seam | `atomus_i32_publicare` (store-release) / `atomus_i32_legere` (load-acquire); one implementation per compiler (clang/gcc `__atomic` builtins today) | per compiler |

Offline rendering (sonitus minting a sound into a buffer once) and
live rendering (a voice computing samples inside the callback) both
exist from the start: offline is cheaper and fine for most effects;
live is required for anything whose parameters move while it sounds
(theremin, held notes, pitch that follows game state).

## VI. Key models (PROPOSITUM)

### VI-a. Time and scheduling

- The audio clock counts frames rendered since start (`s64`).
  `auditus` reports it with the current output latency; the main
  thread schedules `now + latency + margin`.
- A command with a timestamp in the past applies at the start of the
  next block (late, never lost); the report ring counts lateness.
- Within a block, a voice starts at its exact frame offset
  (sample-accurate).
- Offline (tests, WAV export, replay render) the clock is simply the
  frame counter of the render loop: same commands, same samples.

### VI-b. Voices and handles

- Pool of N voices (N fixed at mixer creation; caller memory). A start
  command names the slot the MAIN thread chose (round-robin steal for
  one-shots, reserved slots for loops - solarium's producer-driven
  ownership) and stamps a generation.
- A voice = { kind, state block, bus, gain ramp, pan ramp, start
  frame, generation }. Kinds: buffer player (a minted or loaded
  buffer), sonitus live, tibia note.
- Stealing policy when full: lowest priority, then oldest (priority
  per command).

### VI-c. Buses

- Fixed tree declared at mixer creation: master + named buses (first
  set: `effectus`, `musica`, `interfacies`). Each: gain (ramped),
  mute, insert chain (filter now; reverb later).
- Mute that keeps time (solarium's `audio_set_muted`: voices advance,
  output zeroed) is a per-bus property.

### VI-d. Memory and lifetime

- Buffers handed to the mixer are immutable; the main thread keeps
  them alive until the report ring says the last voice using them has
  ended (a reference count kept on the MAIN side, decremented by
  reports).
- The audio thread owns the mixer state after start; the main thread
  never reads it directly - only through reports.

### VI-e. Patches as data

```xml
<!-- a sound-effect patch: preset + overrides (solarium sounds.stml) -->
<sonitus genus="blip" frequentia="660" crush="4"/>

<!-- an instrument patch (tibia) -->
<tibia titulus="pulsus_quadrans">
  <oscillator forma="pulsus" duty="0.25"/>
  <involucrum impetus="0.002" casura="0.1" tenor="0.6" remissio="0.08"/>
</tibia>
```

(Attribute names are illustrative; the canon fixes them in S1/S5.
`casura` = decay is a placeholder - `casus` is a latina macro, so the
attribute list is checked against the forbidden identifiers before
approval. `remissio`, not `dimissio`, for release: `dimissio` is the
popover system's library.)

### VI-f. Live parameters (the theremin case)

A live voice exposes named parameters (pitch, gain, cutoff ...). A
SET command targets (handle, parameter, value, ramp ms); the voice
glides. Mouse motion at the pane's event rate becomes SET commands;
the ramp hides the step between events.

### VI-g. Determinism and tests

- Offline render == live render (one function). Tests render command
  lists headless and assert on samples: frequency via zero crossings,
  constant-power pan (L² + R² = 1), stale handles ignored,
  sample-accurate start, ramps without discontinuity, envelope shape,
  bit-identical repeat renders.
- WAV export of any render: the audition path (`afplay`) and a source
  of golden digests.
- An event-sourced game replayed offline renders its audio
  identically - audio regression tests from game replays.

## VII. Header text (PROPOSITUM - shapes only, for discussion)

Names provisional (§X.1); every identifier to be checked against
latina.h macros and the oracle before approval.

```c
/* mixtura.h - mixtor sonorum (sonus-spec §V): voces, viae (busses),
 * iussa tempore in quadris (frames) ordinata, nuntii retro. PURUS:
 * sine filis, sine allocatione post creationem, sine I/O. Idem
 * codex in filo temporis veri (auditus) et in probationibus. */

nomen structura {
    i32 sedes;          /* index vocis */
    i32 generatio;      /* iussum generationis aliae ignoratur */
} MixturaAnsa;

nomen enumeratio {
    MIXTURA_INCIPERE,   /* vocem incipere (genus, fons, via, ...) */
    MIXTURA_SISTERE,
    MIXTURA_PONERE,     /* parametrum vocis cum clivo (ramp) */
    MIXTURA_VIAM_PONERE /* amplitudo / silentium viae */
} MixturaIussumGenus;

nomen structura {
    MixturaIussumGenus genus;
                   s64 momentum;    /* quadrum applicationis */
           MixturaAnsa ansa;
                   ... (fixa magnitudine)
} MixturaIussum;

nomen structura {
    /* vox finita, fons liberatus, metra, defectus (underruns) */
} MixturaNuntius;

Mixtura* mixtura_creare  (Piscina* piscina, i32 voces, ...);
b32      mixtura_iubere  (Mixtura* m, constans MixturaIussum* iussum);
/* quadra stereo f32 intercalata; nuntios in m colligit */
vacuum   mixtura_reddere (Mixtura* m, f32* exitus, i32 quadra);
b32      mixtura_nuntium_legere (Mixtura* m, MixturaNuntius* n);
```

```c
/* auditus.h - porta exitus soni (sonus-spec §IV.1): unica plagula
 * cum filo (auditus_macos.m). Supra hanc portam domus filis caret. */

b32  auditus_aperire  (Mixtura* m, i32 frequentia_exemplorum);
vacuum auditus_claudere (vacuum);
/* filo principali solo: iussum in anulum; FALSUM = plenus (omissum,
 * numeratur) */
b32  auditus_iubere   (constans MixturaIussum* iussum);
/* nuntii ab filo temporis veri */
b32  auditus_nuntium_legere (MixturaNuntius* n);
/* horologium: quadra reddita + mora exitus */
s64  auditus_momentum (vacuum);
s64  auditus_mora     (vacuum);
```

## VIII. First consumers in detail

### VIII-a. Notification sounds in vicus (S4)

- A host system in vicus (beside `admonitiones`) plays a patch when a
  new praeconium event arrives: `permission_prompt` -> one patch,
  `Stop` -> another (initially chosen to evoke Ping and Glass,
  synthesized, not copied).
- Mute (§X.3): TWO persistent preferences on the vicus root - a
  master mute for all vicus sound, and a mute for notification sounds
  alone - each with a key or click (master in the vicus chrome,
  notifications in the notification centre). Per-event-kind muting
  is not built until asked for.
- Then the two `afplay` hooks come out of `~/.claude/settings.json`
  (Fran's file - Claude proposes the edit, Fran applies or approves).
- Sounds play only while vicus runs (§X.4, accepted). Playing them
  outside vicus is a later option, not designed now.

### VIII-b. The theremin pane (S5) - `aetherophonum`

- A vicus pane (genus `aetherophonum`, Termen's "etherphone"; Fran
  2026-10-22): x = pitch on a logarithmic scale (a few octaves), y =
  loudness; SOUNDS WHILE THE BUTTON IS HELD (Fran 10-22: no accidental
  sound when the pointer crosses the pane; hover only shows the pitch
  line). Every mouse sample (coalesced motion samples carry their own
  time, `EventusExemplum`) becomes a pitch SET with a short ramp; keys
  choose the waveform and toggle a pitch grid (snap to scale) and
  vibrato.
- The first proof of live voices, smoothing and the latency budget.
- State: waveform, range, snap = persistent widget state; pointer
  position = kinetic (never written to the store).
- Pulls a MINIMAL `tibia` forward (Fran 10-22): a sonitus voice is an
  effect (its length lives in its parameters, its pitch only follows
  its own slide), a theremin needs a held note whose pitch moves on
  command - the instrument voice of §V. S6 then adds patches as data
  and songs on top.
- Pulls a SHARED sound host into vicus (Fran 10-22): one output per
  process (`auditus`), and S4's tintinnabulum owns a mixer of its own -
  the theremin and the jingles must share one mixer + rector + output.

Slices (2026-10-22):

| slice | content |
|---|---|
| S5a | `tibia` minimal (gated note: oscillator + ADSR + vibrato + low-pass; pitch glides in semitones = log frequency) + mixtura voice kind TIBIAE: start, pitch SET with ramp, gate close (release); loudness through the existing per-voice gain ramp. Pure, headless tests |
| S5b | one sound host in vicus: mixer + rector + auditus shared by every client (lazy open, close after quiet with no voice held); tintinnabulum becomes a client (API change); the vicus sound port gains live-voice calls |
| S5c | the `aetherophonum` pane: figura (pitch grid, note names, current note), key table, button-held voice, motion -> pitch/gain SETs |
| S5d | measure output latency and the real motion-event rate (§XI); Fran's ears |

### VIII-d. Music: songs and the sequencer (S6) - decided 2026-10-22

Fran 10-22: a music-authoring pane ("garageband / ableton type") and
agents authoring music (MCP or similar) are expected later, and probably a
sheet-music view. S6 builds only songs + a sequencer, but these choices
are made NOW because they are cheap now and expensive to retrofit:

1. **Musical time is the model.** Notes carry TICKS (position, length);
   frames are computed only at playback from the song's tempo map. Tempo
   change, seek, loop regions, drag-a-note = tick arithmetic, no re-parse.
2. **960 ticks per quarter (3840 per whole)** - the DAW / MIDI / MusicXML
   convention; divides by 3, 5, 6, 15 (the old 768-per-whole reader could
   not express quintuplets).
3. **A structured song model, MML as one way to write it.** Song ->
   tracks (instrument by name, gain, mute, solo) -> clips placed at tick
   positions -> notes (tick, length, pitch, velocity, optional spelling).
   MML compiles into a clip (agents write it well, errors carry offsets);
   an editor will edit notes directly. Later: songs as insula documents
   with acts (undo, history).
4. **Meter map, key map, per-note spelling** in the song - the facts a
   score cannot derive (barlines; C# vs Db; the author's B# over C).
   Beams, stems, ties across barlines, layout, glyphs, MusicXML belong to a
   future notation pane.
5. **The sequencer reads the live model.** `cantor` keeps a playhead in
   ticks and each pulse schedules only the next ~250 ms from the CURRENT
   song (an edit while playing is heard next pass); transport = play,
   stop, seek, loop region; per-track gain / mute / solo applied at
   scheduling (the three fixed buses do not scale to many tracks yet).
6. **Instruments by name through a resolver** (like cantus's preset
   resolver): user patches plug in later without touching the sequencer.
   A generic live "set parameter" mixer command (a filter knob while a
   note sounds) is NOT built in S6 and nothing in S6 forecloses it.
7. **Agent feedback**: every text input validated with offsets; outputs an
   agent can read - deterministic WAV render (for Fran), a textual summary
   (tracks, bars, notes, range), later a piano-roll image.

Names (Fran 10-22): `cantor` (sequencer, "the singer"), `carmen` (song),
book `carmina.stml` (embedded, like `cantuale.stml`). Music plays first
through `auditio -carmen` (live and WAV); vicus `$music` and the game
(S7) come when they pull. First song: *Korobeiniki*, melody + bass.

| slice | content |
|---|---|
| S6a | cantus at 960 per quarter; notes expose tick position and length (frames stay for jingles); a tempo-map type |
| S6b | `carmen`: tracks -> clips -> notes in ticks, instrument by name, tempo / meter / key maps, optional spelling; STML reading with MML clips; book `carmina.stml` |
| S6c | `cantor`: tick playhead, lookahead from the live model, loop / seek / stop, per-track gain / mute / solo, instrument resolver |
| S6d | *Korobeiniki* + `auditio -carmen` (live, WAV) -> Fran's ears |
| later | editor pane (piano roll; insula song documents with acts), notation pane, patch editor, generic parameter SET, more buses, agent tool (`praeco carmen` or MCP) |

### VIII-c. The arcade (S7)

- Sound effects through sonitus presets; music through cantus +
  tibia; the game's acts drive scheduled commands, so a replay renders
  identical audio offline.

## IX. Phases

| phase | content |
|---|---|
| **S0** | this spec approved; names sealed (§X.1); forbidden-identifier check of the header text |
| **S1** | `sonus` primitives + `sonitus` (solarium's synth re-expressed on them, `sors` for noise and jitter) + WAV writer through `filum` + a small audition tool (render a patch to a WAV). Tests: §VI-g. Audible via `afplay` |
| **S2** | `mixtura` offline: pool, handles, buses, scheduled commands, ramps, reports. Headless tests |
| **S3** | `atomus` seam + `circulus` SPSC ring (headless tests, plus a two-thread stress test in the probe tier), then `auditus` macOS: AudioUnit callback thread, two rings, clock + latency; a smoke tool that plays a blip (§X.7) |
| **S4** | first consumer: notification sounds in vicus + mute preference; retire the `afplay` hooks |
| **S5** | theremin pane `aetherophonum`: S5a minimal `tibia` + mixer voice kind, S5b shared sound host in vicus, S5c the pane, S5d latency + ears (§VIII-b) |
| **S6** | songs + sequencer (§VIII-d): S6a ticks at 960/quarter, S6b `carmen` song model + `carmina.stml`, S6c `cantor` sequencer, S6d *Korobeiniki* (19th-century Russian folk song - public domain; the Tetris theme) via `auditio -carmen` |
| **S7** | the Tetris-like game (its own spec) |
| later, on PULL | WAV read + resampling (canned assets), reverb (port solarium's Freeverb), patches in a sound-design pane, tracker patterns, a music-making app |

## X. Decisions (2026-10-20) and what is still open

1. **Names - SEALED.** `sonus` (primitives), `sonitus` (sound effect),
   `tibia` (instrument), `mixtura` (mixer), `cantus` (sequencer),
   `auditus` (output seam) - Fran: "those names sound good". Added the
   same day for §X.7: `circulus` (SPSC ring - `anulus` is TAKEN, it is
   the algebraic ring of matrix/magnus) and `atomus` (ordering seam);
   both in the oracle, neither a library prefix yet.
2. **Patches: a C preset table in S1** (default kept; Fran did not
   object). STML patches + canon when the first pane edits them.
3. **Mute: master + notifications.** Fran: "we might have a master
   mute but we might also want a mute just for notifications at some
   point". Both are persistent root preferences (§VIII-a); the master
   mute lives in mixtura's master bus gain (one place), the
   notification mute in the vicus system that triggers them.
4. **Sounds only while vicus runs - ACCEPTED** for now ("maybe at some
   point we would have them independently").
5. **48 kHz f32 stereo internally** - default kept.
6. **S4 / S5 order** - Fran: "either one". S4 (notification sounds)
   first: smaller, and it proves the whole chain on a sound heard every
   day; the theremin then stresses live voices on a working pipeline.
7. **Atomics without C11.** Fran asked whether this can be C89. ISO
   C89 has no memory model at all, and `volatile` is NOT enough: on
   Apple Silicon (weakly ordered) the CPU may make the ring's index
   visible before the slot it guards, so the reader takes a half-
   written command (x86 hides this bug; ARM shows it). But the ring
   needs only TWO ordered operations - store-release on publish,
   load-acquire on read - so the decision is: `circulus` is pure C89
   and calls `atomus`, a two-function seam with one implementation per
   compiler. Verified 2026-10-20 (scratch probe, not committed):
   - `__atomic_store_n(.., __ATOMIC_RELEASE)` /
     `__atomic_load_n(.., __ATOMIC_ACQUIRE)` compile CLEAN under the
     full house set (`tools/vexilla.sh`: `-std=c89 -pedantic -Werror
     -Wconversion ...`), in `.c` and in `.m` (`.m` files get the same
     flags, `compile_tests.sh:196`); clang -O2 emits exactly `stlr`
     and `ldapr` on arm64.
   - `./silva/examen.sh` REJECTS the same file: `__ATOMIC_RELEASE` /
     `__ATOMIC_ACQUIRE` = "identificator ignotus" (violatio), the
     builtins = "vocatio implicita" (suspectum). House precedent for
     intrinsics: `lib/xar.c:267` hides `__builtin_clz` behind `#if
     defined(__clang__) || defined(__GNUC__)` with a C89 `#else` -
     silva reads the `#else`. Atomics have no correct C89 `#else`, so
     S3's first step chooses: (a) teach silva the compiler-predefined
     `__ATOMIC_*` macros and the `__atomic_*` builtin signatures
     (preferred - the tool learns, nothing is exempted), or (b) a
     declared exemption for `lib/atomus_*.c` alone.
   Precedent outside the house: before C11 (2011) every serious C
   project did exactly this - the Linux kernel's `smp_store_release` /
   `smp_load_acquire`, PortAudio's `pa_memorybarrier.h` beside its
   `pa_ringbuffer` - a ring in plain C over a tiny per-compiler barrier
   header. C11 `<stdatomic.h>` standardized that header; we keep
   our own.

## XI. AUDIENDA

- Settled 2026-10-20 (§X.7): clang `__atomic` builtins pass the
  compiler under house flags; examen does not yet know them. Not yet
  checked: whether the Latin identifier lint or other commit gates
  object to `__atomic_*` names, and whether examen sweeps `.m` files
  at all.
- Not verified: that an AudioUnit output works in `vicus_terminalis`
  (a process without NSApp) - expected yes (`afplay` is a CLI tool).
- Not measured: callback block size and output latency on this
  machine; the theremin's latency budget.
- Not checked: the event rate at which vicus delivers
  `EVENTUS_MUS_MOTUS` to a pane (theremin smoothness), and whether
  key-up reaches panes through the dispatcher.
- Solarium's code was read, not compiled or run here; its presets were
  "tuned by ear" for a 3D palace, not for UI or games.
- The exact praeconium `genus` strings for each hook event were taken
  from the header comment (`include/praeconium.h`), not from a log.
- The doctrine (§IV) is assembled from standard practice and two
  hours of conversation; no part is measured in this house yet.
