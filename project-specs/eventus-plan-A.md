# eventus — plan A (the vocabulary, fenestra, ludus, recording, conformance)

*2026-10-01. Spec: `project-specs/eventus-spec.md` (D1–D8 approved by
Fran 2026-10-01). Interview: `project-specs/eventus-brainstorm.md`.
Worktree `../rhubarb-secunda`. Executed INLINE, one task per turn, Fran
approving each. Red first; plants that COMPILE, caught by name; tasks
with a window step end with Fran's look before the commit. Phase B (the
terminal: module 002 tokenizer → decode → source) gets its own plan after
A's RELATIO.*

**EXECUTED 2026-10-02** (A1–A7; A3 and A6 split during execution into
A3a/b/c and A6a/b - see `lib/eventus.phase-log.md`, RELATIO at its end).
AUDIENDA outcomes: there too.

## 0. Facts this plan rests on (read 2026-10-01)

- **The event queue.** fenestra queues `Eventus` values in a fixed ring
  (`MAXIMUS_EVENTUUM`). EVERY source (NSEvent handlers, the injection API)
  passes through `impellere_eventum`, which stamps `tempus` if it is
  zero. That is the one place for coalescing, timestamping and payload
  copying.
- **Payload lifetime.** `[NSEvent characters]` gives an NSString whose
  UTF-8 pointer dies with the autorelease pool. Text payloads need a
  fenestra-owned byte buffer, valid until the next
  `fenestra_perscrutari_eventus` (FRAN Q16's rule, made concrete).
- **Injection.** `fenestra_clavem_immittere` / `fenestra_murem_immittere`
  post REAL NSEvents, so tests through them exercise the real decode path
  (and need the window to be key: `fenestra_clavem_capere`).
- **Derivation.** `derivare.c` derives `MUS_DUPLEX` from timestamps
  (300 ms, 4 px). The dispensator derives hover/focus after targeting.
- **Recordings.** `eventus_stml` writes titles, not numbers.
  `probationes/pictor/toy.eventus.stml` is the committed recording that
  must keep loading.
- **Reach.** 97 files use `Eventus`; 75 reads of `.typus` (most in the
  old widget generation; ludus: dispensator Tab, pictor Escape/'p',
  manus_ludus synthesis).

## 1. Tasks

**A1 — `eventus.h` split (D1).** A pure move, no field changes.
- `include/eventus.h`: `eventus_genus_t`, `clavis_t`, `mod_vexilla_t`,
  `mus_botton_t`, `Eventus` (verbatim from fenestra.h). No fenestra
  include. aedilis corpus: whichever existing `.c` the T4a-style probe
  shows is right (likely none needed: types only).
- `fenestra.h` includes `eventus.h`. `dispensator.h`, `derivare.h`,
  `destinatio.h`, `actio.h`, `eventus_stml.h`, `ludus_fenestra.h` (it
  keeps fenestra.h: it IS the window glue) and `manus_ludus` switch to
  `eventus.h` where they need only the vocabulary.
- **Gate:**
  - root suite (all), briar, tessera, generata;
  - builds of pictor, villa, forum (apps that include these headers);
  - an aedilis probe: `dispensator.h` + `derivare.h` + `destinatio.h` →
    no `fenestra_macos.m`, no framework.
- **Plant:** `eventus.h` including `fenestra.h` (the probe sees Cocoa
  again).

**A2 — the new fields + capabilities + STML (spec §3; D2 step 1 shape).**
- `Eventus` extended as in spec §3:
  - key: `runa`, `codex`, `actio`, side bits;
  - text: `textus` view, `genus`, `cursor`, `origo`, `truncatum`;
  - pointer: `indicator`, `indicator_genus`, `pressio`, `exempla` view +
    count;
  - scroll: `dx`, `dy`, `genus`;
  - drop: `x`, `y`, `viae` view, `numerus`, `promota`;
  - new kinds appended: TEXTUS, DEPOSITIO, SUSPENSIO, RESUMPTIO,
    FACULTATES.
- `EventusFacultates`.
- `EventusCodex` (D3, about 80 codes) + `eventus_codex_titulus` /
  `_ex_titulo` (STML names = the W3C strings: "KeyA", "ArrowLeft", …).
- `eventus_stml` writes and reads every new field. Text, drop paths and
  samples are COPIED into the reading piscina (recordings own their
  bytes). `typus` and the f32 deltas are still written and read.
- **Red first:**
  - a round-trip for every kind;
  - reading `toy.eventus.stml` gives events EQUAL to today's reading
    (byte-compare the re-serialization);
  - the codex title table round-trips all codes.
- **Plants:** a new field not serialized; an old attribute no longer
  read (the toy recording breaks); codex titles off by one.

**A3 — fenestra fills the vocabulary (D2 step 1, D3, D5).**
- `keyCode` → `codex` via a 128-entry macOS-virtual-keycode table.
- `runa` = the unshifted character (`charactersIgnoringModifiers`, first
  codepoint).
- `isARepeat` → ITERATA.
- `NSEventModifierFlags` device bits → side bits (AUDIENDA: measure
  them).
- A keyDown with characters pushes the KEY event, then a TEXT event
  (COMMISSUM, TYPED), with the same tempus. The text bytes are copied
  into a per-poll byte buffer, reset at each `fenestra_perscrutari_eventus`.
- `typus` keeps its old value (step 1).
- **Scroll:** `hasPreciseScrollingDeltas` → PRECISUS, with integer `dx/dy`
  and remainders accumulated per window; else GRADATUS with one notch =
  `gradus_rotulae` (published). The f32 deltas are still filled.
- **Motion:** in `impellere_eventum`, a MOTUS whose predecessor in the
  ring is a MOTUS with the same buttons MERGES. The predecessor's position
  is appended to the per-poll sample buffer (cap 64, D5, oldest kept),
  and the new position becomes the event's.
- **Capabilities:** an `EVENTUS_FACULTATES` event is queued first at
  window creation (releases ✓, codex ✓, Tab ≠ Ctrl+I ✓, sides ✓,
  hover ✓, preedit — per AUDIENDA, clipboard CERTA, drop NULLA until
  implemented, notch size, pressure — only if the device reports it).
- **Red first** (through the injection API):
  - 'a' with Shift → KEY(KeyA, runa 'a', SHIFT) + TEXT("A");
  - a repeat;
  - Tab vs Ctrl+I distinct;
  - é via the injected characters → TEXT("é");
  - 10 motions in one poll → one MOTUS with 9 samples;
  - a precise scroll accumulates.
- **Look:** Fran types in a small fenestra probe that prints events
  (the existing `tools/eventus_inspector.c` may serve; to check first).
- **Plants:** text before key; samples not reset per poll; a repeat
  reported as PRESSA.

**A4 — the conformance table + fenestra runner (D7).**
- `probationes/fixa/eventus/conformitas.stml`: scenarios with fenestra
  input (injection calls), terminal input (bytes; unused until B4), the
  expected Eventus stream (eventus STML), and capability-keyed
  `excusationes`.
- The fenestra runner: inject → poll → compare the stream (tempus
  ignored, order exact).
- First scenarios: Shift+a; Tab; Ctrl+I; arrows ± Shift/Alt; Escape;
  click; double-click (raw: two PRESSA/SOLUTA pairs — DUPLEX is NOT in a
  source stream); drag; wheel; resize; paste-as-text (once fenestra pastes
  are text events, a Cmd+V path if exercisable).
- **Red first:** the table + runner against A3. One scenario
  deliberately mismatched first, to prove the runner can fail.
- **Plant:** the runner ignoring order (a swapped pair passes → must
  fail).

**A5 — the ludus consumers move (D2 step 2) + drag derivation (D4).**
- `dispensator.c:450`: Tab traversal by logical key (`clavis ==
  CLAVIS_TABULA`, Shift for back), not `typus == '\t'`.
- `pictor_actiones.c`: Escape and 'p' by logical key (`runa`/`clavis`).
- `manus_ludus.c`: synthesis fills the new fields.
- `derivare.c`: a raw-only DRAG derivation (press + motion beyond 4
  our-pixels → TRACTUS_INCIPIT / TRACTUS / TRACTUS_FINIT, appended
  derived kinds) from timestamps and positions, never the clock.
- **Red first:**
  - Ctrl+I must NOT move focus (it did when Tab was `typus == '\t'`
    after a lossy mapping; proves the logical key);
  - pictor's 'p' works on AZERTY's logical 'p';
  - the drag thresholds.
- **Plants:** Tab by typus again; the drag threshold off by one.

**A6 — recording with targets + replay modes (D6).**
- The dispensator's recorder writes `scopus` / `scopus_x` / `scopus_y`
  for pointer events (destinatio's result).
- Replay RAW (positions → targeting) and SEMANTIC (target id + offset →
  position recomputed from the current layout); divergence reported.
- **Proof:** record a pictor toy session; replay it RAW and SEMANTIC
  after nudging the layout by a few pixels. RAW may diverge and the report
  says where; SEMANTIC reproduces the end state.
- **Plant:** semantic replay using the raw position.

**A7 — RELATIO.**
- Phase log.
- terminal-planning modules/003 status, features/008.
- The spec's AUDIENDA closed or carried.
- Merge main in, fast-forward main (check the main session first).
- briar MUTATIONES for `eventus.h` (a corpus header).

## 2. Gates per task

- Root suite (A1 and every header change: `fenestra.h` / `eventus.h` are
  widely included).
- briar, tessera, generata.
- Builds of pictor / villa / forum.
- oratio when the glossary changes (new Latin words: `codex`, `exemplum`,
  `depositio`, … checked by the lint first).
- `eventus_stml` + `probatio_derivare` + `probatio_dispensator` +
  `probatio_fenestra` for A2–A6.

## 3. Names to seal (Fran)

The new kinds:
- `EVENTUS_TEXTUS`;
- `EVENTUS_DEPOSITIO`;
- `EVENTUS_SUSPENSIO` / `RESUMPTIO`;
- `EVENTUS_FACULTATES`;
- the drag kinds.

Also `EventusCodex`, `EventusFacultates`, and the field names in spec §3.
Every name is checked against latina.h's reserved words first.

## 4. AUDIENDA (carried from the spec, with the task that measures each)

- IME: is commit text in `[NSEvent characters]` or only via
  `insertText:`? Measured in A3 (preedit may need `NSTextInputClient`,
  possibly beyond A).
- Can injection synthesize ISREPEAT and side-specific modifier bits?
  Measured in A3.
- Does `tools/eventus_inspector.c` serve as A3's look tool? Checked in
  A3.
- Do `persistentia_*` store events (so need A2's format)? Checked in A2.
- Consumers that break at D2 step 3 (deleting `typus`): the inventory is
  made then, outside plan A.
