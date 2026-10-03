# eventus — spec v1 (the shared, lossless input vocabulary)

*2026-10-01. Sources:
- the interview `project-specs/eventus-brainstorm.md` (27 Q&A, Fran);
- terminal-planning modules/003 (input vocabulary), 002 (sequence
  tokenizer), 005 (tessera reshaped) and 013 (ludus_tessera);
- the code read on 2026-10-01 (main 38bcf610 merged into secunda).

Decisions marked FRAN were made in the interview. D-numbers marked
PROPOSED need Fran before the plan.*

## 1. Goal

One event model, `Eventus`, carries keys, text, pointers, scroll, drops
and lifecycle LOSSLESSLY from every source: fenestra (native window), the
terminal (over tessera), later vitrea and the emulator. ludus consumes it
without linking any window system. Recordings replay across targets, and a
shared conformance table defines what "the same vocabulary" means.

| today | after |
|---|---|
| `Eventus`/`EVENTUS_*` live in `fenestra.h`; `dispensator.h` includes it → every ludus app links Cocoa | `include/eventus.h`, pure; fenestra.h includes it; ludus (dispensator, derivare, destinatio, actio) links no Cocoa |
| a key event carries ONE byte of typed text (`character typus`) | separate key events (physical code + logical key + press/repeat/release + sided modifiers) and TEXT events (UTF-8, commit/composing, origin) |
| shortcuts match the typed byte (`dispensator.c:450` Tab via `typus == '\t'`; `pictor_actiones.c` Escape and 'p' via `typus`) | shortcuts match the LOGICAL key; text input reads text events |
| pointer = x, y, button, modifiers; motion one event per sample | + pointer id, kind, pressure; motion coalesced per read WITH its samples |
| scroll = f32 deltas | integer our-pixels + precise/notched |
| no drop, no suspend/resume, no capabilities | drop (native; terminal-promoted paste), suspend/resume, per-source capabilities (learned, never queried) |
| replay = same target, raw positions (`eventus_stml`) | replay across targets: raw position + resolved target; raw or semantic mode |
| sources agree "by review" | a conformance table every source must pass |

## 2. What exists (read 2026-10-01)

- **97 files** use `Eventus`/`EVENTUS_*`:
  - ludus: dispensator, derivare, destinatio, actio, ludus_fenestra,
    manus_ludus, pictor_actiones;
  - the older widget generation: widget, layout, pagina, schirmata,
    navigator, calendario_visus, …;
  - villa, forum, speculum, atrium, vitrea's tests, persistentia, manus,
    tools (eventus_inspector, lector_eventuum).
- **`.typus`** is read in ludus by `dispensator.c` (Tab), `pictor_actiones.c`
  (Escape, 'p') and `manus_ludus.c` (synthesis), plus in the old widget
  generation. (The `.typus` hits in `arbor_*` and `lapifex` are an
  unrelated field.)
- **The event kinds (`eventus_genus_t`):** NIHIL, CLAUDERE,
  MUTARE_MAGNITUDINEM, FOCUS, DEFOCUS (window focus), EXPONERE,
  CLAVIS_DEPRESSUS/LIBERATUS, MUS_DEPRESSUS/LIBERATUS/MOTUS/ROTULA,
  MUS_DUPLEX, then the ludus-derived MUS_INTRAVIT/EXIIT and
  FOCUS_CAPTUS/AMISSUS/PETITUS, and MENU.
  - The order matters: it is the title table in `eventus_stml.c`.
  - `MOD_*` values equal `NSEventModifierFlags`.
- **Time is data:** `Eventus.tempus` holds s64 ms. fenestra stamps it;
  replay supplies it.
- **Derivation is already raw-only in ludus:** `derivare.c` makes
  `MUS_DUPLEX` from timestamps (an interval and a distance), "never from
  the clock". The dispensator derives hover and focus after targeting.
  `widget.c` (old generation) derives its own double-click, which is
  out of scope here.
- **fenestra (`fenestra_macos.m`):**
  - keys: `convertere_clavem(keyCode)` + the FIRST character of
    `[characters]` as `typus`;
  - mouse: window → table pixels via `scala_x/y` (already "our pixels");
  - scroll: `scrollingDeltaX/Y` as f32.
- **Injection (a test seam that already exists):**
  - `fenestra_clavem_immittere(fenestra, macOS keyCode, NSEventModifierFlags,
    UTF-8 characters, down/up)`;
  - `fenestra_murem_immittere(fenestra, FenestraMusGenus, x, y, mods)`.

  Both post real NSEvents through `[NSApp postEvent:]`, so the real
  decode path runs. The conformance table's fenestra runner uses them.
- **`eventus_stml`** gives `Eventus[] ↔ STML` (titles, not numbers). The
  committed recording `probationes/pictor/toy.eventus.stml` must keep
  loading.
- **The terminal:**
  - `TesseraEventum` is deliberately lossy (Tab = Ctrl+I, `\r` = `\n`, no
    releases, shift only "where knowable"); paste is `GLUTINUM` (a view,
    64 KiB cap);
  - modes set: `?2004`, `?2026`, `?1002`;
  - 238 input vectors, 108 debt shapes waiting on module 002's tokenizer.

## 3. The model (`include/eventus.h`)

*Names follow the existing ones; new ones are marked (unsealed).*

**Kinds.** Existing kinds keep their titles. New kinds are APPENDED
(STML stores titles, so the numeric order only matters for the title
table, which grows at the end):
- `EVENTUS_TEXTUS`: text commit or composition (preedit);
- `EVENTUS_DEPOSITIO`: a drop of paths;
- `EVENTUS_SUSPENSIO` / `EVENTUS_RESUMPTIO`: suspend/resume;
- `EVENTUS_FACULTATES`: capabilities published or changed.

The existing `FOCUS`/`DEFOCUS` are window focus, `MUTARE_MAGNITUDINEM`
is resize and `CLAUDERE` is close (FRAN Q27: all recorded).

**Key** (`datum.clavis`, extended; FRAN Q1, Q9):
- `clavis` (existing `clavis_t`): the LOGICAL key for named keys
  (arrows, F-keys, Escape, Return…); for character keys, the ASCII value
  as today (back-compatible), or IGNOTA for non-ASCII;
- `runa` (new): the unshifted codepoint the layout produces for a
  character key (é on French, ф on Russian), 0 for named keys. This is
  the logical identity for shortcuts;
- `codex` (new): the PHYSICAL key, a W3C `code` subset (D3), IGNOTUS when
  the source can't know;
- `actio` (new): PRESSA / ITERATA / SOLUTA. The existing DEPRESSUS /
  LIBERATUS kinds stay, and ITERATA marks auto-repeat;
- `modificantes`: the existing bits + side bits (new, only when the
  modifier key itself is reported);
- `typus`: DEPRECATED (migration step 1 keeps it filled, step 3 removes
  it).

**Text** (`datum.textus`, new; FRAN Q1, Q11):
- `textus` is a chorda VIEW (FRAN Q16: valid until the next read);
- `genus`: COMMISSUM / COMPONENS (preedit);
- `cursor`: COMPONENS only, a byte offset;
- `origo`: TYPED / PASTED (replaces tessera's GLUTINUM at the vocabulary
  level);
- `truncatum`.

**Pointer** (`datum.mus`, extended; FRAN Q2, Q13, Q15):
- `x`, `y` in OUR pixels. fenestra already gives table pixels. The
  terminal gives the cell centre via the Modulus;
- the existing `botton` and `modificantes`;
- `indicator` (id: 0 = mouse), `indicator_genus` (MUS / STILUS /
  TACTUS), `pressio` (0..1000; −1 unknown);
- `exempla` + `numerus_exemplorum`: a VIEW of the coalesced intermediate
  samples (x, y, tempus) for a MOTUS event, valid until the next read
  (FRAN Q10, Q13).

**Scroll** (`datum.rotula`, extended; FRAN Q14):
- `dx`, `dy`: integer our-pixels (the source accumulates sub-pixel
  remainders);
- `genus`: PRECISUS / GRADATUS;
- the f32 `delta_x/y` are DEPRECATED (one consumer: `importatio_visus.c`).

**Drop** (`datum.depositio`, new; FRAN Q7):
- `x`, `y`;
- `viae`: a chorda VIEW, newline-separated absolute paths;
- `numerus`;
- `promota` (VERUM = a terminal paste promoted heuristically).

**Capabilities** (`EventusFacultates`, new; FRAN Q4, Q17):
- `liberationes` (releases reported);
- `codex_physicus`;
- `tabula_distincta` (Tab ≠ Ctrl+I);
- `latera` (left/right modifiers);
- `super` (hover available);
- `praeeditio` (IME preedit);
- `scriptura_copiae` (clipboard write: NULLA / FORTASSE / CERTA);
- `depositio` (NULLA / HEURISTICA / NATIVA);
- `gradus_rotulae` (our pixels per notch);
- `pressio`.

A source publishes its capabilities once (an `EVENTUS_FACULTATES` event,
first in every stream) and AGAIN when it learns more (the terminal: the
first kitty-format sequence seen). Recordings include these events.

**Sources emit raw events only** (FRAN Q12). Derived kinds (MUS_DUPLEX,
INTRAVIT/EXIIT, FOCUS_CAPTUS/…) are produced only by ludus. fenestra
already complies (it never emits DUPLEX: `derivare.c` does).

## 4. Decisions

**FRAN (the interview):**
- Q1: separate key and text events.
- Q2: pointer positions in our pixels.
- Q3: cross-target replay.
- Q4: per-source capabilities.
- Q5: commands DEFERRED. Long term: registered commands + a fuzzy
  command palette.
- Q6: record the raw position + the resolved target.
- Q7: a drop event, with terminal promotion.
- Q8: keep the name `Eventus`, in its own header.
- Q9: both code + logical key. Keyboard-layout detection is a later
  platform layer.
- Q10: coalesce motion at the source per read.
- Q11: preedit as a native-only capability.
- Q12: sources emit raw events only.
- Q13: coalesced motion carries its samples.
- Q14: integer scroll + kind.
- Q15: pointer id, kind and pressure.
- Q16: views; the recorder copies.
- Q17: kitty pushed, capabilities learned by observation.
- Q18: extend, then retire `typus`.
- Q19: decode now, with an encode-ready API.
- Q20: the conformance table is the acceptance bar.
- Q21: widget states degrade by capability.
- Q22: keyboard-only operability is a tested rule.
- Q23: the terminal source is a library over tessera.
- Q24: `?1003` only when hover is declared.
- Q25: OSC 52 write; read = paste.
- Q26: Shift+drag is left to the terminal.
- Q27: lifecycle events are in the vocabulary and recorded.

**D1–D8: APPROVED by Fran 2026-10-01 ("that sounds great"), including D8 = (a), module 002 as Phase B.** (The text below is kept as proposed.)

**D1 — the header split, the T4a way.** `include/eventus.h` holds the
kinds, `clavis_t`, `MOD_*`, `mus_botton_t`, `Eventus` and
`EventusFacultates`. No fenestra include. `fenestra.h` includes it, so
every caller sees what it saw. `dispensator.h`, `derivare.h`,
`destinatio.h`, `actio.h`, `eventus_stml.h` and `manus_ludus` include
`eventus.h`, NOT `fenestra.h`. **Acceptance:** an aedilis probe of
`dispensator.h` links no `fenestra_macos.m`. The same as T4a's probe,
which also confirmed aedilis links by header.

**D2 — the migration in three steps (FRAN Q18), each green:**
1. The new fields exist; fenestra fills them AND the deprecated
   `typus` / f32 deltas; `eventus_stml` writes the new attributes and
   still READS old recordings (`toy.eventus.stml` untouched).
2. The ludus consumers move: `dispensator.c` Tab → logical key;
   `pictor_actiones.c` Escape/'p' → logical key; text fields → text
   events; `importatio_visus.c` → integer scroll.
3. `typus` and the f32 deltas are deleted; the old widget generation is
   either migrated or keeps a local projection. That is decided at
   step 3 with an inventory, not now.

**D3 — the W3C `code` subset for v1:** what a US Mac keyboard produces.
Letters, digits, punctuation, Space, Enter, Tab, Backspace, Escape,
arrows, Home/End/PageUp/PageDown, Delete, F1–F12, the modifier keys
(both sides), CapsLock. About 80 codes, from a table: macOS virtual
keyCode → code (fenestra) and kitty key number → code (terminal). Others
are IGNOTUS until needed.

**D4 — thresholds as constants, OS settings later.** Double-click
300 ms / 4 px (today's `derivator_initiare(CCC, IV)`) and a drag
threshold of 4 our-pixels stay ludus constants. An OS preference as a
capability (interview option "raw only, OS settings as data") is a
later extension, not v1.

**D5 — the sample view is capped.** At most 64 samples per coalesced
motion, in the source's own buffer. If more arrive in one read, the
OLDEST are kept and the last is the event's position (a stroke's shape
survives, the endpoint is exact). Recordings copy the samples.

**D6 — recordings.** `eventus_stml` gains attributes for the new fields
plus `scopus` / `scopus_x` / `scopus_y` (the resolved target: component
id + local offset), written by the DISPENSATOR's recorder (it knows the
target), not by sources. Replay modes:
- RAW: positions → targeting runs;
- SEMANTIC: the target id + offset → the position is recomputed from the
  current layout.

A raw/semantic DIVERGENCE is reported as a finding (FRAN Q6).

**D7 — the conformance table is DATA.** `probationes/fixa/eventus/
conformitas.stml` holds scenarios. Each scenario has:
- a name;
- the fenestra input (a list of injection calls);
- the terminal input (bytes, the vector format);
- the expected Eventus stream (eventus STML);
- an `excusationes` list: differences allowed BY CAPABILITY, e.g. "no
  SOLUTA without `liberationes`".

One runner per source; both read the same file. New sources (vitrea, the
emulator) add a runner, never a copy of the table.

**D8 — the terminal decoder (Phase B; THE open choice).** Lossless
terminal Eventus needs a decoder that keeps what tessera's lossy reader
drops. Options:
- (a) **Build module 002, the sequence tokenizer**, then module 003's
  decode: the planned path (module 005, "tessera reshaped").
  - Pays tessera's 108 debt shapes.
  - Both tessera's projection and the Eventus source sit on ONE parser.
  - Biggest: a DEC/Williams state machine.
- (b) **tessera's reader exposes a lossless intermediate** (a new
  accessor). The public `TesseraEventum` projection is unchanged.
  - Smaller.
  - Grows tessera past "grid + input" toward a vocabulary it shouldn't
    own.
- (c) **The Eventus source has its own small decoder** for the subset
  tessera already handles, plus kitty.
  - Two parsers of the same bytes, the duplication module 002 exists to
    prevent.

**Recommendation: (a)**, scheduled as Phase B after Phase A has proven
the model on fenestra.

**DECIDED (Fran, 2026-10-02): (a)**, after phase A closed (RELATIO in
`lib/eventus.phase-log.md`). Plan: `project-specs/eventus-plan-B.md`.

## 5. Phases and tasks (sketch for the plan)

**Phase A: the vocabulary, fenestra, ludus, recording, conformance.**
- A1 `eventus.h` split (D1): pure move + `fenestra.h` includes it.
  Root/briar/tessera suites + the aedilis probe (dispensator Cocoa-free).
- A2 The new fields + `EventusFacultates` + title-table growth;
  `eventus_stml` round-trips the new attributes AND reads
  `toy.eventus.stml` (red first).
- A3 fenestra fills them (D2 step 1):
  - keyCode → codex (D3 table);
  - `characters` → a TEXT event (keyDown with characters: key event THEN
    text event, in that order, same tempus);
  - repeats → ITERATA;
  - side bits;
  - `scrollingDelta` → integer + PRECISUS/GRADATUS
    (`hasPreciseScrollingDeltas`);
  - capabilities published at window creation;
  - motion coalesced per `fenestra_perscrutari_eventus` read with
    samples.

  Tested through the injection API.
- A4 The conformance table format + fenestra runner (D7), with the first
  scenarios: Shift+a, Tab vs Ctrl+I, arrows with modifiers, click, drag,
  wheel, paste-as-text, resize.
- A5 ludus consumers (D2 step 2): Tab focus traversal and pictor's
  shortcuts by logical key; `MUS_DUPLEX` derivation unchanged; drag
  derivation (new, raw-only, D4).
- A6 Recording with targets + RAW/SEMANTIC replay (D6), proven on a
  pictor toy scenario.
- A7 RELATIO.

**Phase B: the terminal source** (after D8):
- B1 module 002 (if a);
- B2 decode → Eventus;
- B3 the source library over tessera (`?1003` on declaration, kitty
  pushed and learned, OSC 52, paste → text/drop promotion);
- B4 the terminal runner of the SAME conformance table;
- B5 tessera's `TesseraEventum` re-derived as a projection (its 238
  vectors prove nothing visible changed);
- B6 cross-target replay demo (record in the terminal, replay in
  fenestra).

**Then (separate plans):** widgets in ludus (button, checkbox,
single-line field, list, with the keyboard-only rule tested), the edit
core (features/018) for the text area, and the design vocabulary.

## 6. AUDIENDA (not verified)

- Whether `[NSEvent characters]` on keyDown always carries the commit
  text, or whether IME commits come through `insertText:` (an
  NSTextInputClient adoption may be needed for preedit; A3 measures it).
- Whether `fenestra_clavem_immittere` can synthesize ISREPEAT and the
  side bits (`NSEventModifierFlags` has device-dependent side masks, e.g.
  `NX_DEVICELSHIFTKEYMASK`; unverified).
- How many Eventus consumers outside ludus read `rotula.delta_*` or
  `typus` in ways step 3 breaks (inventory at step 3).
- The kitty key numbers → W3C code mapping (Ghostty's table, MIT, to be
  read before B2).
- Whether Terminal.app sends anything for a file drop beyond the pasted
  path text (B3 measures it).
- Whether `persistentia_*` (which appears in the Eventus usage list)
  stores events, and so whether it needs D6's format too.
