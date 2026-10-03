# eventus phase-log

Spec: `project-specs/eventus-spec.md` (D1–D8 approved by Fran
2026-10-01). Plan: `project-specs/eventus-plan-A.md`. Interview:
`project-specs/eventus-brainstorm.md` (27 Q&A).

## A1 — `eventus.h` split out of `fenestra.h` (2026-10-01)

**INTENTIO (D1).** A pure move, the T4a way. `fenestra.h` lines 85–238
(event kinds, `clavis_t`, `mod_vexilla_t`, `mus_botton_t`, `Eventus`) are
cut verbatim into `include/eventus.h`. It has no fenestra include and no
corpus (types only). `fenestra.h` includes it, so callers see the same.

**Which headers switch.** Checked by name, not by guess. dispensator,
derivare, destinatio, actio and eventus_stml use ONLY the vocabulary:
their headers and `.c` files name no `Fenestra*`, `fenestra_*` or pixel
table. They now include `eventus.h`. `ludus_fenestra.h` uses `Fenestra`
and `TabulaPixelorum` (it is the window glue), so it keeps `fenestra.h`.

**Verified:**
- An aedilis probe (dispensator + derivare + destinatio + actio +
  eventus_stml) → 21 plain libraries, NO `fenestra_macos.m`, no
  framework. ludus's interaction core is window-free.
- Root suite 178/178, briar 20/20, tessera 15/15.
- pictor, villa and forum build. villa and forum need their wrappers'
  generation steps first (`capsula_generare` + `speculum_generare.sh`:
  "fons absens: build/speculum/…" was a missing generated input, not a
  compile error). Run without the final `exec`; git unchanged.

**Plant (compiling):** `eventus.h` including `fenestra.h` → the probe
shows `fenestra_macos.m` + `-framework Cocoa` again. Restored → clean.

**Gap, recorded:** "the ludus core / tabula_pixelorum / tessellatio link
no Cocoa" is checked by a HAND probe, not a committed gate. A
`tools/sine_cocoa_probare.sh` (aedilis-probe a header list, fail on
`fenestra_macos.m`) would make it durable. Wiring it as a commissio gate
touches the gates inventory (main side), so that is Fran's call.

## A1b — `<aedilis nexus="purus"/>`: "links no framework" as a gate (2026-10-01)

**INTENTIO (Fran: "an STML comment annotation that opts a file in …
silva tooling vs grep").** The "no Cocoa" property (tabula_pixelorum,
the ludus core, tessellatio) was checked only by hand probes. Its home is
**aedilis**, not examen:
- examen judges one translation unit's C;
- linking is the OBJECT CLOSURE (aedilis's header→object variants + the
  `nexus` rules in aedilis.stml);
- aedilis already reads `<aedilis corpus="…"/>` through silva's STML
  parser.

`nexus` is NOT "Objective-C files". It is aedilis's list of objects
needing extra link flags. Today that is fenestra_macos (Cocoa), tls_macos
(Security, CoreFoundation), vitrea_macos (WebKit, Cocoa) and
clipboard_platform_macos (Cocoa), which happen to be the 4 `.m` files.
The `*_posix.c` variants have no rule (libSystem). So "nexus purus" =
"links nothing beyond the C library".

**Done:**
- **Engine (`lib/aedilis.c`).** The annotation verb `nexus` accepts the
  single value `purus`; anything else is refused loudly ("nexus
  ignotum"), since a typo must not silently drop the promise. Promising
  files are recorded in `AedilisFructus.pura`.
- **`aedilis_nexum_purum_probare`** derives the tagged file AS the scope
  and fails if any object in its closure carries a nexus rule, OR if the
  file's OWN implementation does (see below). The cause NAMES the chain:
  `include/dispensator.h -> include/derivare.h -> include/eventus.h ->
  include/fenestra.h -> lib/fenestra_macos.m [-framework Cocoa]`.
- **Tool (`bin/aedilis --nexus-purus`)** scans the include roots
  (non-recursive) for `.h` files carrying the promise, through the
  memoised silva extractor (not grep), and judges each. Exit 1 if any is
  broken OR if none promise (a silent gate is a dead gate).
- **Gate.** `tools/aedilis_porta.sh` runs it.
- **Tagged (11 headers):** tabula_pixelorum, delineare,
  delineare_mandata, modulus, tessellatio, eventus, dispensator,
  derivare, destinatio, actio, eventus_stml. Each tag is its own comment
  (aedilis annotations take one attribute). examen ACCIPE on them.

**Tests (`probatio_aedilis`, fixture extractor):**
- `purum.h` (pure);
- `impurum.h` → beta.h → beta_macos.m (Cocoa), with the exact chain;
- `orbis_a/b/c` (an include CYCLE b↔c): the chain is named once, without
  circling;
- an unpromised file with a Cocoa object → VERUM (nothing promised);
- `proprium.h`, whose OWN implementation carries a rule → fails;
- `nexus="mixtus"` → refused.

**Three bugs found on the way:**
1. **`i32` is unsigned.** A reverse loop `for (j = n - 1; j >= 0; j--)`
   never ends: `j` wraps to 2^32 − 1 and indexes past the array (a bus
   error). The fix is a signed `s32 k`. Lesson: clang's `-Wall -Wextra`
   does NOT warn on `unsigned >= 0`; `-Wtautological-unsigned-zero-compare`
   would. House-wide adoption is a candidate park, since it would surface
   existing tautologies such as `x < ZEPHYRUM` on unsigned pixel
   coordinates.
2. **The chain circled** on an include cycle (the real-tree plant made
   eventus.h ↔ fenestra.h). Fixed: the backward walk stops at the first
   revisited header. The cycle fixture was red first.
3. **A MUTE plant exposed a real hole.** "promise ignored" passed
   because the test used `beta.h` itself as the scope, and a header as
   scope does NOT pull its own implementation (the variant rule applies
   to INCLUDED headers only). So tagging `fenestra.h` itself as pure
   would have passed. Fixed: the check also maps the tagged header to its
   own implementation (`_obiectum_probare` + `_regulam_nexus_invenire`).
   The test now uses `fons/scopus.c` (untagged, really reaching Cocoa)
   and `proprium.h` (its own implementation has a rule).

**Plants (compiling, 0 errors), all caught:**
- A: any nexus value accepted;
- B: nexus flags ignored;
- C: the promise ignored (mute at first; see 3);
- D: the own-implementation check removed.

On the real tree: the A1 plant (eventus.h including fenestra.h) → 6/11
broken, each with its chain named; restored → 11/11.

**Note:** the fixture's aedilis.stml gained a second nexus rule
(`proprium_macos`); the configuration test's pin on that count was
updated (1 → 2) deliberately.

## A2 — the lossless fields, capabilities, physical codes, recording (2026-10-01)

**INTENTIO (spec par. III; D2 step 1 shape; D3).** `Eventus` grows; nothing
is removed.
- **Key:** `runa` (unshifted codepoint, the logical identity), `codex`
  (`EventusCodex`, physical, W3C subset), `actio` (PRESSA / ITERATA /
  SOLUTA). `typus` is marked DEPRECATED but kept.
- **Pointer:** `indicator`, `indicator_genus` (MUS / STILUS / TACTUS),
  `pressio` (0..1000, `EVENTUS_PRESSIO_IGNOTA` = −1), `exempla` (a VIEW)
  + `numerus_exemplorum`.
- **Scroll:** integer `dx`/`dy` + `genus` (IGNOTA for old recordings /
  PRAECISA / GRADATA). The f32 deltas are DEPRECATED but kept.
- **New union members:** `textus` (contentum VIEW, genus, cursor, origo,
  truncatum), `depositio` (x, y, viae VIEW, numerus, promota),
  `facultates` (`EventusFacultates`).
- **New kinds APPENDED:** TEXTUS, DEPOSITIO, SUSPENSIO, RESUMPTIO,
  FACULTATES (the title table grows at the end).
- **Side modifiers:** `MOD_*_SINISTER/DEXTER` with values = macOS
  device-dependent masks. fenestra ALREADY copied raw `modifierFlags`, so
  the side bits were already in events and recordings. Other sources map
  into this convention.
- **`EventusCodex`:** 84 codes (letters, digits and F-keys as RANGES:
  `EVENTUS_CODEX_LITTERAE + (c − 'A')`), so there is no per-letter
  identifier for the Latin lint. STML titles are W3C's own strings
  ("KeyA", "ArrowLeft", "CapsLock"; IGNOTUS = "Unidentified"). The title
  table is generated from the enum order, and the test pins 84 + a
  round-trip of all codes.

**`eventus_stml`:**
- **SPARSE writing:** a new attribute appears only when it differs from
  its default, so old events re-serialize unchanged.
- The reader defaults absent attributes (pressure → IGNOTA, so an old
  mouse event never claims pressure 0).
- **Text payloads** (`contentum`, `viae`) use an attribute with a minimal
  percent-encoding: `%`, `"` and control bytes become %XX; UTF-8 stays
  raw. It is exact (leading spaces, newlines) and readable. A quote
  cannot live in an STML attribute (stml.h), and text-node whitespace
  semantics are subtle.
- **Samples** are `<exemplum x y tempus/>` children.
- Payloads read back are COPIED into the reading piscina (the recording
  owns its bytes, FRAN Q16).

**FINDING: the plan's oracle was wrong.** "Re-serialize toy.eventus.stml
byte-identically" assumed the file came from the writer. It was
HAND-WRITTEN (f84e06b3, compact one-line form); the writer pretty-prints
attributes on aligned lines. The right promise is "the new writer writes
old events exactly as the OLD writer did". The golden
`probationes/fixa/eventus/toy.rescriptum.stml` was produced by
temporarily restoring HEAD's `eventus_stml.c`, running a probe on the toy
events, and restoring the new writer. The test compares byte for byte.

**Red first:**
- titles of the new kinds;
- all 84 codes;
- a round-trip of every new field, including a hard paste text (leading
  spaces, a quote, `%`, a newline, e + combining acute, é, 中) and paths
  with a quote and `%`;
- samples copied (not the caller's pointer);
- the toy recording's mouse pressure IGNOTA and its key's codex IGNOTUS;
- the golden.

**Plants (compiling, 0 errors), all caught:**
- A: `runa` never written;
- B: `typus` no longer read (the toy reading and the old round-trip);
- C: code titles shifted by one (8+ assertions);
- D: the sparse rule broken (pressure always written → golden mismatch);
- E: the quote not escaped. The WHOLE document became unparseable, as
  stml.h warns.

**AUDIENDA closed:** `persistentia_*`'s "eventus" are data-model events
(create entity, set property) that only share the prefix. They store no
input events, so D6's format doesn't apply.

**Lint:**
- "precisa" was my Latin error; it is now *praecisa* (praecidere; cf.
  the house's *praecisio*);
- `tituli_rotularum` → `tituli_rotulae`;
- glossary: *suspensio*, *resumptio*, *gradatus*;
- my own slip: `ordinarius` (a latina.h macro = `default`) used as a
  parameter name; renamed `si_abest`.

pictor, villa and forum build with the grown `Eventus`.

**A2 addendum (Fran's question: "the compact style might be a result
of running the stml formatter?").** Checked:
- the writer's pretty output is ALREADY formatter-stable;
- the toy file was NOT formatter output (`stml formare -probare`: it
  diverges; the formatter existed since 09-01, before the toy file of
  09-05);
- the decisive fact: `stml formare` of the toy file == the old-writer
  golden, byte for byte.

So the toy recording was formatted into house style, the separate golden
was deleted, and the test is the plan's ORIGINAL oracle: reading the
recording and writing it back gives the file itself. Plant D (the sparse
rule broken) is still caught. probatio_pictor_toy (which replays this
file) is green.

## A3a — keys and text from fenestra (2026-10-01)

**Design change from the plan.** The plan said to test "through the
injection API", but injected NSEvents need a live window + event loop,
which can't run headless (`probatio_fenestra.c` is an interactive loop).
So it is a pure core + thin Cocoa glue, as the house does elsewhere. Two
new PURE modules (tagged `<aedilis nexus="purus"/>`; the nexus check is
now 13/13):
- **`claves_physicae`:** `claves_codex_ex_macos(s32)`, a switch over
  Apple's kVK_* constants (cross-checked against fenestra's
  `convertere_clavem`). 114 = kVK_Help = the PC Insert position.
  Phase B adds kitty key numbers here.
- **`eventus_cauda`:** the event queue (ring of 256) + a per-poll text
  buffer (64 KiB). `eventus_cauda_lectio_incipit` resets the buffer ONLY
  if the queue is empty (views of unextracted events stay valid).
  `eventus_caudae_textum_impellere` COPIES the bytes; on overflow it
  truncates with `truncatum`; a full ring increments `amissa`. The
  terminal source (phase B) will reuse it.

**fenestra (`fenestra_macos.m`):**
- the ring is replaced by `EventusCauda`;
- the Fenestra struct now comes from `piscina_allocare_ordinatum(..., VIII)`
  (plain `piscina_allocare` is byte-packed, and the struct holds
  pointers and s64), with the queue explicitly initialised;
- `lectio_incipit` runs at each poll;
- one key path `_clavem_impellere`:
  - codex (physical);
  - runa = the unshifted codepoint;
  - actio (ITERATA from `isARepeat`, SOLUTA on release);
  - side bits already present in the raw `modifierFlags`;
  - `typus` kept;
  - then a TEXT event (same tempus), unless control, Apple private-use
    (F700..F8FF: arrows, F-keys) or under Cmd/Ctrl (shortcuts aren't
    typing).

**Unshifted rune: two attempts.** `charactersIgnoringModifiers` ignores
everything EXCEPT Shift ("A" for Shift+a).
- The first fix was Carbon's `UCKeyTranslate`. It needs
  `-framework Carbon`, and about eight scripts link fenestra by hand
  (compile_tests, compile_library, silex (lib/silex.c), briar
  spectator, vitrea …), so adding a framework would have rippled into a
  released product. Reverted.
- `-[NSEvent charactersByApplyingModifiers:0]` (AppKit, macOS 10.15+)
  gives the same answer with Cocoa alone.

**Look tool:** `tools/auscultator.c` + `tools/auscultator.sh` (built by
aedilis) open a window and print each event as one STML line.

**Tests:**
- `probatio_claves_physicae`: all 83 kVK pairs, AND every one of our 83
  codes reachable (no gaps), unknowns → IGNOTUS;
- `probatio_eventus_cauda`: FIFO, full → amissa, text COPIED, views
  survive a non-empty poll, truncation.

**Plants (compiling, 0 errors), all caught:**
- buffer always reset;
- text not copied;
- amissa not counted;
- Digit5/6 swapped (wrong mapping AND an unreachable code).

**Fran's look (2026-10-01): "everything looks okay".** The log
confirmed:
- a → `codex KeyA runa 97` + TEXT "a";
- Shift+a → runa 97 (unshifted), TEXT "A", left-Shift bit 0x2;
- Ctrl+I → `KeyI`, no text (≠ Tab: the lossless point);
- Tab → no text;
- Cmd+C → no text, left-Cmd bit 0x8;
- HOLD a → ITERATA after ~500 ms, then every ~84 ms (macOS defaults),
  each with its TEXT event; SOLUTA on release.

**Findings:**
- **Dead keys do NOT compose through raw NSEvents.** Option+e carries
  EMPTY characters (typus 0, no text). é needs the Cocoa text-input
  system (NSTextInputClient / interpretKeyEvents:). Native `praeeditio`
  stays FALSE until then. Park (terminal-planning parks/004).
- Bit 0x100 is set on every event: macOS's always-on "non-coalesced"
  flag, harmless.
- The pointer jumps OUTSIDE the window (x 289 → 939 as it leaves
  600×400): pre-existing, events from outside the content view. That's
  for A3b, with coalescing.

**A3a, the first commit attempt failed. Finding: `tempus` linked all of
Cocoa.** aedilis's gate: 10 tests (actor, http, hospitium, tractator,
vitrea_servus, …) failed to LINK, missing eventus_cauda / utf8 symbols.
They reach `fenestra_macos.m` not through fenestra.h but through
`lib/tempus.c`'s `<aedilis obiectum="lib/fenestra_macos.m"/>`. tempus
needed three clock functions, so it declared them `extern` and linked
the WHOLE Cocoa window file. aedilis does not walk annotated objects,
so fenestra_macos.m's new dependencies never entered those closures.

The functions are pure mach/POSIX (`mach_absolute_time`,
`mach_timebase_info`, `usleep`). Fix, the T4a pattern:
- `include/fenestra_tempus.h` (`nexus="purus"`; 14 pure headers now),
  with its declarations cut verbatim from fenestra.h, which includes it;
- `lib/fenestra_tempus_macos.c` (the platform-variant rule;
  `postulata_posix.h` first), with its definitions cut verbatim from the
  .m;
- `tempus.c` includes the header (the annotation and the hand-written
  externs are deleted).

All 10 tests build and link NO fenestra_macos at all. tempus users no
longer drag Cocoa. `compile_library.sh`'s hard-coded fenestra list
gained the new file. examen ACCIPE (2 suspecta: the mach functions are
absent from the POSIX lexicon).

## A3b — motion merging with samples, integer scroll, capabilities (2026-10-01)

**Pure core (`eventus_cauda`):**
- `eventus_caudae_motum_impellere`: a MOTUS merges into the queue's LAST
  event (not yet extracted) when that event is a MOTUS with the same
  button, modifiers and pointer (id + kind). The old position becomes a
  sample (x, y, tempus) and the new one replaces it.
  - The samples of one event are contiguous in the queue's buffer
    (`EVENTUS_CAUDA_EXEMPLA` = IV * MXXIV), because only the last event
    grows. The view points into the buffer.
  - At most 64 samples, keeping the oldest; the final position is always
    exact (D5).
  - The buffer is cleared under the same rule as text: at the start of a
    read, and only if the queue is empty.
- `eventus_residuum_integrare`: returns the whole part of an
  accumulator. The f64 → s32 cast truncates toward zero, which C89
  defines (unlike negative integer division).

Tests VI–X were red first. Four plants were caught by name: samples not
cleared per read; the cap off by one (65); merging that ignores modifiers;
rounding instead of truncating.

**fenestra:**
- Motion goes through the merge in `impellere_eventum`.
- One position helper, `_murem_implere`, replaces three copy-pasted blocks.
- `_rotulam_implere`: `hasPreciseScrollingDeltas` → PRAECISA, scaled to
  our pixels; otherwise GRADATA, lines × `FENESTRA_GRADUS_ROTULAE` (XVI).
  Each window keeps its remainders, and they are cleared when the scroll
  kind changes.
- `_facultates_impellere`: the first event of every window. praeeditio
  is FALSE (park 004), depositio NULLA, clipboard CERTA, pressio FALSE.
- auscultator takes `[mora_ms]` to imitate an app that reads once per
  frame.

**Found during Fran's looks:**
1. **Screen coordinates.** A move outside the key window arrives with
   `[event window] == nil`, and its `locationInWindow` is in SCREEN
   coordinates. That was the A3a jump 743,16 → 96,363. Positions are now
   always converted through the screen into OUR window.
   - Off-window positions are clamped to the edges for now. Before this,
     a negative f64 was cast to the unsigned i32, which is undefined;
     on this CPU it happened to give 0.
   - Fran chose to make `mus.x`/`mus.y` s32 in A3c.
2. **Poll-time stamps.** Events were stamped when we READ them, so a
   whole batch got nearly the same time (a sample 3 ms from its event
   while 18 px away). They now carry `[NSEvent timestamp]`, which runs on
   the same clock as `mach_absolute_time` (measured: they differ by
   0.003 ms). `_tempus_eventus` converts it.
3. **Samples stay rare on macOS.** With a 50 ms read delay there is still
   about one move per read, stamped with the latest hardware time.
   - My best explanation: the window server keeps only the newest
     pending move for an app that hasn't fetched it.
   - `[NSEvent setMouseCoalescingEnabled:NO]` (AppKit's own merging) is
     now set at NSApp creation, but it had no visible effect. It stays
     as a no-loss setting.
   - AppKit has no API for the lost positions of a mouse. A drawing app
     wants fine strokes has to fetch often.
   - The terminal source (phase B) will be the opposite: motion reports
     are bytes and nothing merges them.
4. **Scroll.**
   - Fran's trackpad reports whole-point deltas, so the remainder carry
     is invisible here (the unit test covers it).
   - The zero events are gesture boundaries: finger lift, then
     momentum, then momentum end. The vocabulary has no phase and no
     momentum, so this is parked as terminal-planning 006, a spec
     addition.
   - The wheel path (GRADATA) has not been seen yet; it needs a real
     wheel.
5. **Resize.** The resize event reports the FRAME size (428 for a 400
   content area). It predates eventus and is parked as terminal-planning
   005.

## A3c — pointer coordinates are s32 (2026-10-01)

Fran's decision, after A3b found the off-window case: `mus.x/y` and
`depositio.x/y` are `s32`, the same as `EventusExemplum`. A pointer off
the window reports its TRUE position: negative to the left or above,
≥ width/height to the right or below. fenestra's interim clamp is gone.
`_murem_implere` uses `floor`: −0.5 is pixel −1 (outside), not 0.

**The inventory was the compiler.** I changed the type and let
`-Wsign-conversion` mark every site where the sign matters (a grep had
found ~85 reads in 23 files):
- **eventus_stml, eventus_cauda, manus_ludus:** direct `s32`, with the
  casts removed.
- **derivare, destinatio, calendario_visus, the importatio drag delta,
  arx's drag ghost:** they already cast to `(s32)`, so nothing changed.
- **Old widget generation (`i32` coordinate space):**
  - press / double-click sites (arx_caeli, widget, schirmata, layout,
    biblia_visus, importatio_visus): an explicit `(i32)` cast, EXACT
    because a press always happens inside the window;
  - hover stores (elementa, concha, dialogus_importatio): `(i32)` cast.
    Off the window, a negative wraps to ≥ 2^31, which fails every
    `x < px + w` test and the cursor's bounds check, so it hits
    nothing. That is correct.
  - Each site carries a comment saying which of the two it is.
- **A real bug: `schirmata.c:1339`.** `i32 mus_x` was tested against
  `>= 320`, so a drag off the left edge counted as "in the right panel".
  It is now a signed local. The same code reads `datum.mus` for ROTULA
  events, but the union lays out the scroll fields differently. That is
  an old bug, only commented.

**Tests:**
- **eventus_stml (red first):** read `x="-12"` and ask `x < 0`. It
  failed 3× under `i32`. The value itself survived the wrap, so
  "can the consumer ask the sign" was the real difference.
- **destinatio (a guard, not red before: it already cast):** (−5, 15)
  hits nothing (not the edge button, not the root). A drag out of the
  window stays with its capture.
  - Learned while writing it: `punctum_locale` belongs to the
    GEOMETRIC hit, not the captured component, so it is 0 off-window.
    A consumer that drags outside the window computes its local point
    itself.
- **Plants:** the STML reader clamping negatives; destinatio clamping
  to the edge. Both caught by name.

**Look:** below y=493, left x=−26, above y=−76, right x=667. Samples
are signed too (x=−1, y=−31). pictor, villa and forum build.

## A4 — the conformance table + fenestra runner (2026-10-01)

**Pure core: `eventus_conformitas`** (`nexus purus`; 15 headers now
promise it). It reads the table (`<conformitas><scaena titulus genera>`
with `<immissio>` (`<clavis codex modificantes characteres depressa>`,
`<mus genus x y>`) and `<expectata>` (`<eventus .../>`)), and compares a
scenario against an actual stream:
- `genera` lists the kinds CONSIDERED. Other kinds are dropped (real
  mouse moves, capabilities, resize). A considered kind the expectation
  doesn't list is a failure: text under Ctrl must not appear.
- Order exact, count exact.
- Expected attributes are a SUBSET of the actual ones. `tempus` is
  always ignored, and children (samples) are not compared.
- The actual events are written by `eventus_scribere_stml` and re-read,
  so titles and forms are the recordings' own. A side effect of the
  sparse writer: defaults (`actio` PRESSA, `runa` 0) are absent, so a
  scenario cannot name them.
- The diagnosis names the first difference (`eventus 1: contentum
  expectatum "a", actuale "A"`) followed by both streams.

The unit test was red first (the stub). Four compiling plants were
caught by name: ignoring order; not filtering kinds; tolerating extra
events (count `<` instead of `!=`); comparing `tempus`.

**Runner: `tools/conformitas.sh [tabula]`, on demand** (not a suite
member: it opens a real window).
- `FENESTRA_RETRO`: the window opens behind the others and takes no
  focus. Injected events go into the app's own queue, which is the queue
  fenestra reads, so focus doesn't matter.
- Per scenario: drain, inject, then read until 100 ms pass with no new
  event (500 ms at most).
- Text and sample VIEWS are copied while collecting: they only live
  until the next read. Without the copy, a later read would overwrite
  earlier text.
- Exit 0 means all conform, 1 means some don't, 2 means the table is
  bad.

**First run (the plan's deliberate mismatch):** shift-a expected text
"a" and FAILED with the right diagnosis, which proves the runner can
fail.

The same run found **a real fact: AppKit adds
`NSEventModifierFlagFunction` (0x800000) to arrow keys itself**, for
injected events too, the same as hardware. Shift+Left = 0x820002 =
8519682. The table now records it, with a comment, rather than hiding
it.

The other six conformed on the first run:
- Tab ≠ Ctrl+I (codex Tab / KeyI, and no text under Ctrl);
- Escape;
- a click;
- a RAW double-click is two press/release pairs, with no `mus_duplex`
  from the source (it was considered, so its absence is checked);
- a drag: two `tractus` injections arrive as ONE merged MOTUS ending at
  30,30. This is the A3b merging, proven through the real NSEvent path.

**8/8 conform.**

Not yet in the table:
- resize (park 005: the event reports the frame size);
- scroll (there is no injection API for scroll);
- the terminal column (B4).

## A5 — ludus consumers by logical key + drag derivation (2026-10-02)

**dispensator, Tab traversal.**
- Tab is now `clavis == CLAVIS_TABULA`, not `typus == '\t'`. Ctrl+I also
  carries `typus '\t'`, so it used to move focus.
- **A second, older bug:** "back" was `modificantes & I`. 0x1 is the
  LEFT CONTROL device bit, not Shift. With real fenestra flags,
  Shift+Tab (0x20002) went FORWARD and Ctrl+Tab (0x40001) went BACK.
  The tests never saw it because manus_ludus passed `I` meaning
  "shift". It is now `& MOD_SHIFT`. The manus test passes the real
  flags (`MOD_SHIFT | MOD_SHIFT_SINISTER`).
- **Test design:** the toy tree has TWO focusables (b1, tabula), so from
  either one forward and back land on the other, and direction is
  invisible. The direction tests start from NO focus: forward → first
  (b1), back → last (tabula). The manus test can't tell direction; the
  dispensator test can.

**pictor.**
- Escape is `clavis == CLAVIS_EFFUGIUM`.
- The pencil is `runa == 'p'` (logical: the current layout's unshifted
  character), and only with neither Cmd nor Ctrl. Before this, Cmd+P
  (print) and Ctrl+P selected the pencil.

**manus_ludus.** Synthesized keys fill the vocabulary from `typus`, the
same way fenestra does:
- clavis: letters UPPERCASE ASCII (as fenestra: KeyA → 65); other
  values are clavis_t ASCII itself (Tab 9, Escape 27);
- runa: lowercase letter, or the printable character;
- codex: by US position (letters, digits, named keys, otherwise
  IGNOTUS);
- actio: PRESSA, then SOLUTA.

manus does NOT yet emit a TEXT event after a key. No ludus consumer
reads text yet, and a stack payload passed into the dispensator would
have to survive any recorder that holds it. That waits for the first
text widget.

**derivare: drag (spec D4).**
- A press, then a motion BEYOND `distantia` (IV) on either axis →
  TRACTUS_INCIPIT, carrying the press ORIGIN (x/y), the time of the
  crossing motion, and the press button.
- Each later motion → TRACTUS at the current position.
- Release → TRACTUS_FINIT.
- Starting a drag clears the pending first click, so a drag is never
  the first half of a double-click.
- Derived events come AFTER their raw event, as DUPLEX does.
- The three kinds are appended to the enum and the title table
  ("tractus_incipit", "tractus", "tractus_finit"). The writer gives
  them pointer fields; the reader's fall-through branch already reads
  them.
- destinatio counts them as pointer-positioned, so capture wins.

**Tests:**
- Red first: derivare (the exact 4 px boundary is no drag; INCIPIT is
  at the origin; FINIT; a click without motion; hover without a press;
  drag vs double-click); dispensator (Ctrl+I; Shift+Tab and Ctrl+Tab
  from no focus); pictor (Ctrl+P, Cmd+P).
- Five compiling plants caught by name: the threshold `>=`; Tab by
  typus again; pictor ignoring modifiers; a drag still setting up a
  double-click; INCIPIT at the current position.
- pictor, villa and forum build.

## A6a — the notarius: recording with targets (2026-10-02)

The plan's "the dispensator's recorder writes `scopus`" assumed a
recorder. There wasn't one: the only recording (toy.eventus.stml) was
hand-written. Fran split A6: A6a = the recorder + format; A6b = RAW /
SEMANTIC replay + divergence + the nudged-layout proof. Names (Fran):
**notarius** (the Roman shorthand scribe) / **EventusNotatum**.

**Shape.**
- `EventusNotatum {Eventus eventus; chorda scopus; s32 scopus_x,
  scopus_y;}` lives in `eventus.h`. The target sits BESIDE the event,
  not inside it: a target isn't source data.
- `dispensator_notarium_ponere(d, Xar*)` switches the notarius on;
  NIHIL switches it off. It notes every RAW event the dispensator
  receives (effusio[0]; derived events are never noted).
  - Pulses (`EVENTUS_NIHIL`) ARE noted: quies (idle time) runs on them,
    and the toy recording itself contains one.
  - Pointer kinds get the component the event is DELIVERED to (the
    captured one if capture holds) and the point in that component's
    space. Keys get no target.
- The target is resolved from the OLD tree, before delivery. The id is
  copied into the dispensator's piscina, because the tree piscinae are
  recycled ping-pong.
- `destinatio_ad_locale(c, schirmum)` inverts the chain of parents
  (fines, translatio, scala) with EXACTLY `invenire`'s arithmetic. For
  a geometric hit it equals `punctum_locale` (tested). For a captured
  component it gives what `punctum_locale` can't, because that belongs
  to the geometric hit (the A3c finding).
- `eventus_stml`: the per-event writer and reader bodies were extracted
  (`_eventum_nodare`, `_eventum_legere`), so the plain and noted
  formats share them.
  - `eventus_notata_scribere/legere_stml` add `scopus`, `scopus_x` and
    `scopus_y` sparsely (absent without a target). A plain recording
    reads back with empty targets.

**Tests (`probatio_notarius`, toy-based). Red first** (with
`_notare` / `ad_locale` stubbed):
- every event is noted, the pulse included;
- b1 at (25,10); tabula at (50,40); Escape has no target;
- without targets, the recording equals the HAND-WRITTEN toy file
  BYTE FOR BYTE;
- a noted round trip is byte-stable;
- under capture, a motion outside tabula is noted as `tabula` at
  (−5,40);
- switching the notarius off stops noting.

`probatio_destinatio`: ad_locale = punctum_locale through the panel's
translation, and negative outside. My first point, (75,55), sat under
the test's `umbra` overlay; a probe showed it and the test now uses
(125,60).

Three compiling plants caught: noting derived events; ad_locale ignoring
`translatio`; skipping pulses.

**Process finding (cost a failed A5 commit).** Plain `silva.commissio`
runs its gates in the LIVE tree. `commissio_umbra` snapshots per gate,
one after another, so later gates also see later edits. I edited
eventus_stml.c (the A6 extraction) during A5's gates; a gate compiled
the half-done file ("unused variable") and A5 was refused. Recovery:
the A6 work was saved aside, the three files were restored to A5 (the
pre-extraction copy was verified as exactly A5's change), and A5 was
recommitted with the tree frozen. A6a was drafted in the scratchpad
meanwhile as an apply-script. Memory corrected: freeze the tree while
ANY commit's gates run.

## A6b — RAW / SEMANTIC replay + divergence (2026-10-02)

**`manus_ludus_iterare(m, notata, modus, divergentiae)`.** It lives in
manus_ludus because manus already drives the dispensator synthetically
and owns `manus_ludus_ad_schirmum`, the exact local→screen inverse of
targeting.
- **CRUDA** delivers the events as recorded; targeting runs again.
- **SEMANTICA** sends each pointer event that has a target to its
  component, found by id in the CURRENT tree. The position is
  `ad_schirmum(component, scopus_x/y)`. If the target is missing, the
  raw position is used.
- **Divergence is reported in BOTH modes, before each event is
  delivered:** what the RECORDED position would hit now (the dispensator's
  own strategy, so capture counts), against the recorded target. It
  returns the count and fills `Xar de ManusDivergentia {index,
  scopus_notatus, scopus_crudus}`.

**The proof (`probatio_iteratio`).** The toy layout gained
`translatio_y` (b1 and tabula move down). Three tests that initialised
`ToyStatus` field by field now use `memset`.
- A: the toy session recorded by the notarius gives the reference end
  state (both insulae + focus).
- B: SEMANTIC, same layout: 0 divergences, state = A.
- C: RAW, layout moved down XV: **2 divergences**, index 0 and 1 (b1's
  press and release; recorded `b1`, raw now `radix`). `numerus` is
  absent, so state ≠ A.
- D: SEMANTIC, same moved layout: the SAME 2 divergences are reported,
  but **state = A**: semantic replay survives the layout change.
- E: `divergentiae` may be NIHIL.

Red first (against the stub). Two compiling plants caught: **semantic
replay using the raw position** (the plan's plant) and semantic mode
not reporting divergences.

The motions under capture never diverge: the strategy routes them to
the captured component whatever the geometry. That is correct: they
were delivered to tabula, and still would be.

## A7 — RELATIO: phase A (2026-10-02)

**What was built.** One lossless input vocabulary (`include/eventus.h`),
filled by fenestra and consumed by ludus, with a conformance table,
recording and replay.

| Task | Commit | What |
|---|---|---|
| A1 | 18b9ff00 | `eventus.h` split out of `fenestra.h`; ludus's core links no Cocoa |
| A1b | 4941f626 | `<aedilis nexus="purus"/>` promise + `--nexus-purus` gate |
| A2 | 6b223156 | the vocabulary grows (codex, runa, actio, sides, text, pointer id/pressure/samples, integer scroll, drop, capabilities); eventus_stml sparse |
| A3a | 35382b69 | fenestra fills keys + text; `claves_physicae`, `eventus_cauda`; `fenestra_tempus` split (tempus no longer links Cocoa); auscultator |
| A3b | 5556477f | motion merged per read with samples; integer scroll; capabilities first; real NSEvent timestamps; screen-coordinate fix |
| A3c | 09d67e5f | pointer coordinates s32 (Fran) |
| A4 | faf9d02a | conformance table + `tools/conformitas.sh` (8/8) |
| A5 | 08fabe21 | ludus by logical key (Tab, Escape, 'p'); drag derivation |
| A6a | e92824a2 | the notarius: recordings with targets |
| A6b | 2b1a3eb4 | RAW / SEMANTIC replay + divergence |

**How it was proven.** Every task went red first, with plants that
COMPILE and are caught by name (about 30 in all). The window tasks
ended with Fran's look through the auscultator. The conformance table
runs against a real window, and semantic replay was proven against a
moved layout.

**What we found (the reusable lessons):**
1. **aedilis links by HEADER, and an annotated object isn't walked.**
   `tempus.c` linked ALL of Cocoa for three clock functions; ten tests
   broke the moment fenestra_macos.m grew. Hence the
   `nexus="purus"` promise, now held by 15 headers.
2. **macOS input facts:**
   - an event outside the key window carries SCREEN coordinates;
   - AppKit adds `NSEventModifierFlagFunction` to arrow keys;
   - the window server keeps only the newest pending move for an app
     that hasn't fetched yet, so per-read samples are rare on native
     windows. The terminal source will be the opposite.
   - `[NSEvent timestamp]` runs on the same clock as
     `mach_absolute_time` (0.003 ms).
3. **Old bugs the vocabulary exposed:**
   - Tab's "back" tested `& 1` (the left-Ctrl side bit), not Shift;
   - Ctrl+I moved focus, and Cmd+P selected pictor's pencil;
   - schirmata treated a drag off the left edge as "in the right panel";
   - a negative f64 cast to an unsigned i32 was undefined behaviour.
4. **Process:** plain `commissio` runs gates in the LIVE tree, and
   `commissio_umbra` snapshots per gate. Freeze the tree while any
   commit's gates run (a failed A5 commit taught it).
5. **Test design:** with two focusables, Tab direction is invisible
   except from no focus. A probe beats an assumption (the umbra overlay
   in probatio_destinatio).

**AUDIENDA (the spec's open measurements):**
- CLOSED: `persistentia_*` stores no input events (A2).
- CLOSED: `tools/eventus_inspector.c` inspects persistentia's
  DATA-MODEL events, so it could never be the look tool; the
  auscultator was written instead (A3a).
- CLOSED: injection carries the raw modifier bits through, side bits
  included (A4 table).
- CARRIED: injection cannot synthesize a REPEAT (`isARepeat:NO` is
  hard-coded), so the conformance table has no repeat scenario yet.
- CARRIED (park 004): IME and dead keys. `[NSEvent characters]` is
  empty for a dead key, so composition needs `NSTextInputClient`;
  `praeeditio` stays FALSE.
- CARRIED (phase B): kitty → W3C mapping; what Terminal.app sends for a
  file drop; the `typus` deletion inventory (D2 step 3).

**Parks filed** (terminal-planning):
- 004: IME / dead keys;
- 005: the resize event reports the frame size;
- 006: scroll phase + momentum missing from the vocabulary;
- 007: one pictor hang, not reproduced (capture with `sample`).

**Not done in phase A:**
- scroll injection (so no scroll scenario in the table);
- manus emitting TEXT events (waits for the first text widget);
- `typus` deletion (D2 step 3).

Phase B (the terminal source) gets its own plan after this RELATIO.

# Phase B — the terminal source (plan `project-specs/eventus-plan-B.md`)

D8 decided by Fran (2026-10-02): (a), tokenizer first. Sealed: name
`series_terminalis`; DCS whole and truncated in v1; C1 8-bit controls
not recognized; a separate library; the table built at creation.

## B1a — `series_terminalis`, the tokenizer (2026-10-02)

**Shape.** Pull API: `series_lexema_proximum(lector, &ptr, finis,
&lexema)` returns ONE token per call (`SeriesLexema`):
- IMPRIMERE: a printable run, as a zero-copy view into the input;
- EXSEQUI: a C0 control or DEL;
- ESC, CSI, SS (SS2/SS3), OSC, DCS, APC (also PM/SOS, via
  `introductor`);
- FUGA: an aborted or flushed sequence, with its raw bytes.

Fields: parameters (24) + a `separatores` bitset (`:` after param i),
intermediates (4), `privatum` (`? > < =`), `introductor` (the byte after
ESC), `finale`, `praefixum`, `textus` (a string payload ≤ 2048 bytes,
copied, `truncatum`), and `crudum` (the sequence's raw bytes, ≤ 64).
Split sequences stay in the reader's state; `_pendet` / `_evacuare` let
the CALLER own the ESC timeout. Nothing is allocated after creation.

**The state machine** is Ghostty's `parse_table.zig` (MIT, pin 12752b2)
as a rules function (`_tabulam_struere`, one `_regula` line per row of
the vt100.net diagram), built into a [256][15] table at creation. ESC,
CAN/SUB and string terminators are handled BEFORE the table, because
the divergences live there.

**Deliberate divergences from Ghostty (all for input):**
- C1 not recognized (0x80+ prints: UTF-8);
- DEL in ground is a control (the backspace key);
- CSI with `:` kept for any final (kitty `CSI 97:65;2u`; Ghostty drops
  non-`m`);
- `ESC N|O [params] final` is ONE token (application-mode arrows
  `ESC O A`, xterm `ESC O 2 P`);
- `ESC ESC …` sets `praefixum` (Fran's FUGAE_PRAEFIXUM decision);
- an aborted sequence becomes FUGA with its raw bytes, the aborting
  byte NOT consumed;
- `ESC \` after a string is swallowed as its terminator;
- ESC followed by a high byte = a lone ESC (FUGA) + the UTF-8 run
  (alt+é).

As in Ghostty: more than 24 params drops the WHOLE sequence (a half SGR
is worse than none), and `csi_ignore` completes silently.

**Tests (`probatio_series_terminalis`, 167 assertions):**
- I: Ghostty's 24 Parser.zig tests ported (C1 and the colon rule
  asserted as OUR behaviour);
- II: the divergences, one test each;
- III: pendet/evacuare (lone ESC, `ESC [`, `ESC O`, a half mouse
  report, `ESC ESC`; after a string closed by ESC, the timeout returns
  nothing);
- IV: the SPLIT SWEEP: 37 sequences, each fed whole, at every 2-way
  split, and byte by byte. The renders (adjacent printable runs merged,
  raw bytes included) must be identical;
- V: hostile input (10,000 params, a 1 MB unterminated OSC, 64 KiB of
  random bytes in random chunks): bounded, and the reader's piscina
  usage is UNCHANGED after creation.

Red first: 141 failures against a stub. The first implementation
failed 4:
- 1 real bug: CAN/SUB in GROUND were swallowed, because the C0 helper
  excludes them for the in-sequence abort path and ground never got a
  rule;
- 1 typo in my expected string.

Four compiling plants caught by name: the param cap off by one; `:`
read as `;`; state lost between calls (caught by the SWEEP: the
property test earns its keep); OSC terminated only by BEL.

**Lint:** `esc`, `osc`, `dcs`, `apc` entered the glossary as tolerated
protocol acronyms (`csi` was already tolerated from older code).

**Not yet:** tessera uses none of this; that's B1b, where the 27
debt vectors get paid.

## B1b — tessera's reader on the tokenizer (2026-10-02)

**The bar held, and the debts were paid.** `probatio_tessera_eventum`
68/68 is UNCHANGED. The vector harness went from 1,425 assertions with
108 debt shapes (27 vectors) to **1,534 with 0 debts**: all 27 were
promoted by name, and the `CAUSA_*` macros were deleted.

**Shape.** `tessera_eventum.c` keeps all its SEMANTICS (the key tables,
`_modificatores_csi`, mouse classification, control bytes, the paste
collector) and the reader loop. The hand-rolled `_csi_parsare` /
`_ss3_parsare` / `_parsare` over the gestation buffer became one token
per call from `series_terminalis` (INPUT MODE), plus handlers:
- `_csi_tractare`: mouse SGR, paste start, keys; unknown forms
  silently consumed;
- `_ss_tractare`;
- `_runam_parsare`: the first rune of a printable run; the rest stays
  in the buffer, which is safe because the tokenizer is in ground;
- `_moram_tractare`: the timeout.

Three RAW side channels stay in the reader, because they aren't DEC
grammar:
- X10 mouse: 3 raw bytes after `CSI M` (AUDIENDA closed: a reader
  special case);
- foreign tails: `CSI [` then bytes up to a final (Linux console
  `[[A`, putty `[[5~`);
- the paste body (unchanged).

**Timeout rules (`_moram_tractare`):**
- the pending sequence is EVACUATED;
- a lone ESC = Escape;
- `ESC ESC` = Escape now, and the SECOND Escape at the very next call,
  without waiting again (both have already outlived the timeout;
  re-feeding it to the tokenizer made `ESC ESC` + timeout + `a` read
  alt+a: the first run's one VALET failure);
- `ESC x` alone = alt+x (`ESC [`, `ESC O`, `ESC P`);
- anything else is discarded (H7);
- RELIQUIAE (H8): a half SGR mouse report (`CSI <…`) and a lone ESC
  already given out as Escape are stashed. They are fed back to the
  tokenizer only if the next bytes continue them (`;5M`, `[<…`,
  `[M…`). `[` alone after an ESC waits one more read; the split forms
  needed this (the first run's 4 remaining debt shapes).

**Tokenizer INPUT MODE** (`series_lectorem_initus_ponere`; output
stays pure DEC, tested). Input and output share bytes but not meaning:
- `ESC` + 0x20..0x2F = ESC final (alt+space, alt+!), not an
  intermediate;
- `ESC` + C0/DEL = FUGA (lone ESC), the byte not consumed → alt+control;
- `ESC N/X/^` = ESC final (terminals never send SS2/SOS/PM);
- `ESC P` + a letter immediately = alt+P + letter. Real DCS replies
  start with a digit, `>`, `!` or `$`.
- I first also made `ESC ]` + non-digit and `ESC _` + non-`G` into alt
  keys, then REMOVED both: no VALET vector types alt+] or alt+_, and
  the rules broke realistic debts (`ESC _ OK`). Those two stay strings;
  a typed alt+] still works through the timeout (FUGA introductor →
  alt+]).

**The one contradiction in the test data** (Fran decided, 2026-10-02):
VALET `alt+P` SEQUENS (`ESC P a` in one read → alt+P, `a`) vs debt
"DCS partialis + mora" (`ESC P partial` → nothing). Both start
`ESC P` + a lowercase letter, so no rule passes both. The debt's bytes
became a realistic half reply, `ESC P >|kit`; H7's intent is unchanged.

**Amalgam.** tessera vendors `series_terminalis`:
- `SeriesLector` → `TesseraSeriesLector`, with the typedef dropped
  (tessera.h owns it);
- `series_` → `tessera_series_`;
- the HAND-WRITTEN `tessera.h` mirrors `TesseraLector`'s new fields in
  source order (the harvest build caught the stale struct). The stash
  size is the tessera-owned `TESSERA_RELIQUIAE_CAPACITAS 64`, written in
  digits in BOTH headers, so an identical redefinition is legal and a
  differing one is a compile error (LXIV vs 64 would not be
  "identical").
- Verified: standalone at full severity, host 7/7, nm intersection 0.

The four demo source lists (`*_fontes_generata.sh`) were regenerated;
spectaculum failed to LINK before that (caught by building it for the
look, not by any suite).

**Plants (compiling, caught by name):**
- reliquiae never returned → the three H8 vectors;
- `praefixum` ignored → both ESC-prefix vectors;
- input mode off → the alt+control family.

**Lint:** `sgr` added to the glossary (an acronym); a helper renamed to
`_octetos_tradere` (*lexemator* isn't a house word).

## B2a — `interpres_terminalis`: legacy tokens → Eventus (2026-10-02)

**Sealed by Fran:**
- the name (*interpres*, the Roman go-between who rendered one tongue
  into another);
- the split: B2a legacy, B2b kitty;
- the legacy text policy: KEY + TEXT, HONEST.

**The boundary.** The decoder is PURE: a `SeriesLexema` in, Eventus
pushed into phase A's `EventusCauda` (text copied, motion merged). Its
only state is the alt prefix (a lone ESC before the next token). The
PIPELINE stays in the source:
- the ESC timeout (the source hands evacuated sequences in with
  `post_moram`);
- the stashed partials;
- the raw channels, with entry points `interpres_x10` and
  `interpres_glutinum`.

This is what keeps the alt prefix and the timeout logic from existing
twice when B3/B5 rebase tessera.

**Decoding (tables translated from tessera's B1b reader into the
Eventus vocabulary):**
- **printables:** per rune KEY PRESSA (clavis: letters UPPERCASE as in
  fenestra; runa: letters lowercase; codex IGNOTUS; NO shift bit) +
  TEXT SCRIPTA carrying the case. No TEXT under alt or ctrl (the
  terminal already decided it's a shortcut).
- **controls, honest:** `\r` Enter, `\t` Tab, DEL Backspace; `\n` =
  Ctrl+J; 0x08 = Ctrl+H; NUL = Ctrl+Space; 0x01..0x1A Ctrl+letter;
  0x1C..0x1F Ctrl+symbol. tessera's merges (`\n`→Enter, 0x08→Backspace)
  will be ITS projection (B5).
- **alt:** a lone-ESC FUGA arms MOD_ALT for the next key, and ONLY the
  next. ESC dispatch = alt+char. After a timeout: ESC = Escape (codex
  IGNOTUS: it could be Ctrl+[), ESC ESC = two, `ESC x` = alt+x, other
  partials dropped.
- **named keys** (CSI A-D/H/F/Z, `~` codes 1-8 and F1-F12, SS3 A-D/H/F
  and P-S): the codex is SET because the sequence names the physical
  key. xterm modifier param: shift 1, alt 2, ctrl 4, meta 8 → super.
  `praefixum` adds alt. Insert has no `clavis_t`, so clavis IGNOTA +
  codex INSERERE.
- **mouse** (SGR, X10): pointer at the CELL CENTRE in our pixels;
  press/release/motion (held or hover; motion goes through the queue's
  merge); wheel = ROTULA GRADATA, one notch = one cell height (64 → dy
  +, 65 → dy −, 66 → dx +, 67 → dx −; the sign is an assumption, which
  B4's conformance run checks against fenestra).
- focus `CSI I` / `CSI O`; paste → TEXT GLUTINATA; OSC/DCS/APC and the
  paste markers produce nothing.

**Tests (`probatio_interpres_terminalis`, 61 assertions):** red first
(58 against a stub). Byte vectors go through `series_terminalis` (input
mode) + the decoder; events are rendered compactly (`KA:a TA`,
`Kup+C#ArrowUp`, `DL@25,30`, `W0,20g`).

The first green attempt HUNG (exit 137 after 190 s): the test's hex
renderer counted DOWN with an `i32` (UNSIGNED here), so `k >= 0` was
never false. clang `-Wextra` doesn't flag it, which is the memory's
classic trap, hit again; it's now `s32` with a comment. Then one test
typo (9 for an 8-byte string).

Three compiling plants caught by name: 1-based coordinates not shifted;
the alt prefix never cleared; text emitted under alt.

**Lint:** reused tessera's names (`_clavem_tildae`, `_modificantes_csi`)
instead of coining `tildam` / `xterm` words.

**B2a's first commit was REFUSED by `generata`:** the tessera amalgam
was 3 lines stale against B1b's COMMITTED source. Cause: in B1b I
edited `tessera_eventum.c` (the `memcpy` line) AFTER formatting it,
then regenerated the amalgam from the unformatted text. The pre-commit
hook reformatted the source at commit time, after the gates had
compared it, and the two drifted by one line wrap. Fix: the regenerated
amalgam rides with B2a (plus the tessera gate). main already fixed the
trap in dfd9a2a4 ("commissio: forma ANTE iudicium": formatting before
the gates). Until secunda merges main: run the formatter after the LAST
edit, and regenerate derived files after that.

## B2b — kitty keyboard decoding (2026-10-02)

**`claves_physicae` gains two tables:**
- `claves_codex_ex_littera`: a US base-layout character → physical code
  (letters, digits, the 12 punctuation keys, space; uppercase and
  non-ASCII → IGNOTUS). Cross-checked against the macOS kVK table:
  same position, same code.
- `claves_codex_ex_kitty`: kitty's functional numbers → codex.
  - Escape 27, Enter 13, Tab 9, Backspace 127, Caps Lock 57358, and the
    eight SIDED modifiers 57441..57450 (Ghostty `kitty.zig`, MIT, pin
    12752b2).
  - Keypad and F13+ have no code in our vocabulary → IGNOTUS.

**The decoder:**
- CSI parameters are split into kitty FIELDS (`;`) and parts (`:`) from
  the tokenizer's `separatores` bitset (`_campos_legere`); an empty field
  = absent.
- `CSI key[:shifted[:base]] [;mods[:event]] [;text] u`:
  - event → PRESSA / ITERATA / SOLUTA (release = genus LIBERATUS);
  - the kitty modifier set: Caps/Num Lock added, hyper/meta have no
    counterpart;
  - associated text ONLY when the field is present;
  - the codex from the BASE key; if the base is absent and ALTERNAE was
    pushed, base = key; otherwise IGNOTUS.
  - Escape/Enter/Tab/Backspace are now unambiguous, so they get a codex;
    Ctrl+I = `105;5u` ≠ Tab.
  - Sided modifier keys, the keypad, Caps/Num Lock.
- Legacy-form keys carry the event part too: `CSI 1;5:3A`, `~` keys,
  and kitty's F1/F2/F4 as `CSI P/Q/S` (F3 is `CSI 13~`, because `CSI R`
  is the cursor-position reply and is ignored).
- The named-key helpers carry `actio` (`_clavem_actio`; `_clavem` =
  PRESSA).
- **Learning by observation:** the decoder holds its `EventusFacultates`
  (legacy defaults from init: clipboard FORTASSE, drop HEURISTICA, notch
  = cell height). The FIRST kitty sequence (`u`, or an event part) sets
  `tabula_distincta`, plus `liberationes` / `codex_physicus` from the
  flags the source PUSHED (`kitty_vexilla`, set by B3), and pushes a
  FACULTATES event BEFORE the key.
- `CSI ? flags u` (the flags reply) is ignored, as are DA replies.

**Tests:**
- red first: 19 kitty assertions in `probatio_interpres_terminalis` (now
  80) and 64 in `probatio_claves_physicae` (stubs);
- cases: the AZERTY `q` → KeyA; Cyrillic ф + text; release and repeat
  of `a` and the arrows; the left Shift alone; caps-lock state; a
  keypad digit; legacy after kitty unchanged; without ALTERNAE the
  codex is unknown;
- three compiling plants caught: release read as press; the base key
  ignored (AZERTY `q` then reports KeyQ, the exact failure it guards);
  capabilities never learned.

**Lint:** `kitty` entered the glossary (a protocol name); the field
part is `pars`.

## B3a-i — scroll position + modifiers; decoder fidelity (2026-10-02)

Fran sealed B3: the name `rivus_terminalis`, and architecture (A), ONE
pipeline moved out of tessera with tessera's reader becoming a
projection at once (B5 folded in). Designing that projection exposed
three gaps, fixed here first, in their own commit, because one is a
vocabulary change and owes the wide gates.

1. **A vocabulary gap: scroll had no position and no modifiers.**
   tessera's vectors expect shift+wheel, ctrl+wheel and the cell under
   the pointer; fenestra had all of it in the NSEvent and DROPPED it.
   `datum.rotula` gains `x`, `y` (our pixels, like the pointer) and
   `modificantes`, appended at the end of the struct. The
   `eventus_stml` writer is sparse, so the old recordings stay
   byte-identical (toy test green), and the reader reads them. fenestra
   fills them through a `_positionem` helper shared with the pointer
   (the same screen→window conversion). The decoder fills them (cell
   centre, wheel modifier bits). It also lets ludus target a scroll by
   pointer later (today it goes to focus).
2. **`typus` = the ACTUAL character** for rune keys (fenestra's meaning:
   `characters[0]`). Alt has no TEXT, so `typus` is the only record of
   alt+`A` vs alt+`a`; the projection needs it ("alt+A" vector). The
   key constructor became `_clavem_typo` (explicit typus);
   `_clavem_actio` derives it from the clavis as before.
3. **motion+wheel (SGR 96/97) dropped,** as tessera does.

Red first: 3 in `probatio_eventus_stml` (rotula x/y/mods round trip),
8 in `probatio_interpres_terminalis` (wheel renders `@x,y` + mods,
shift+wheel, motion+wheel silent, typus ×4). pictor, villa and forum
build.

## B3a-ii — the pipeline moves to `rivus_terminalis`; tessera projects (2026-10-02)

The terminal input pipeline left tessera. `lib/rivus_terminalis` is the
whole bytes → Eventus path, and it is PURE: no fd, no clock, no poll.
The caller

- pushes bytes (`rivus_tradere`, room from `rivus_spatium`, buffer 256);
- asks how long it may wait (`rivus_mora_ms`: 3000 inside a paste,
  25 while anything is undecided, 0 otherwise);
- reports silence (`rivus_moram`), which settles what was waiting: a
  lone ESC becomes Escape, a partial SGR mouse report is kept, x10 and
  alien remnants are dropped, a paste ends truncated, a half rune
  loses one byte;
- pulls events (`rivus_eventum`). Decoding is LAZY: tokens are decoded
  only when the queue is empty, so motion is NOT merged here (the
  eager, merging read is B3b's consumer API).

The raw channels stay in rivus, not in the decoder: CSI M (x10 mouse,
three raw bytes), the rxvt-style alien `CSI [`, and bracketed paste
(`CSI 200~` … `CSI 201~`, terminator matched incrementally across
feeds, 64 KiB cap, an empty paste is still a TEXT GLUTINATA, truncation
marked on the last event). Only complete runes reach the decoder; a
half rune at the end of the buffer waits for the next feed.

**tessera's reader is a projection** of Eventus into the lossy
`TesseraEventum` (`_proicere`): named keys, F1-F12, Ctrl+J → Enter and
Ctrl+H → Backspace (ctrl removed), the rune is `typus` when printable
and not ctrl, else `runa`; keys with no rune are dropped; pasted TEXT
→ GLUTINUM; hover motion dropped; ROTULA → ROTA_* with cell and
modifiers. `TesseraLector` shrank to the fd bridge plus a
`RivusTerminalis*`; its reliquiae buffer and capacity constant are gone.

**Behaviour change, Fran's call:** 5 tessera vectors used to swallow
kitty and rxvt sequences silently; the pipeline now decodes them (kitty
`a`/ctrl+a/`:`-forms, x10 + kitty, rxvt `7~ 8~` → Home/End). Updated
with a comment naming the decision.

**Bugs found on the way:**
- an empty paste produced no event at all (now an empty GLUTINATA);
- kitty `typus` under Shift was the base key: it is the shifted key
  when the sequence carries one, else the rune (`lib/interpres_terminalis.c`);
- the interpres test helper read the FACULTATES event (pushed before
  the first kitty key) instead of the key: `_typus` takes the first
  KEY event.

**Vendoring into the tessera amalgam:** the amalgamator renames
`interpres_`, `eventus_`, `claves_`, `rivus_` → `tessera_*` and
`RivusTerminalis` → `TesseraRivusTerminalis`; `fontes_politica.sh`
names the five new bases for exclusion (without them the excludenda
harvest could not classify `claves_codex_ex_macos`). Static function
names must be unique across vendored files: `_sola_fuga` collided and
became `_rivi_fuga_sola`.

**Tests:** `probatio_rivus_terminalis` is a CHARACTERIZATION test,
written after the code (wait times, no motion merge, paste split across
feeds, empty paste, silence truncation, the 256 capacity, an H8 split
stitched). Two compiling plants bite: eager decoding merges motion (2
vectors red), the cell off by one (most mouse vectors red). tessera's
full suite: vectors 1534/1534, eventum 68/68. The four demos link.

**Order lesson, again:** format after the LAST edit, THEN regenerate
the amalgam; the other way round leaves a stale amalgam for `generata`
to refuse (B2a paid for that once).

## B3b-i — eager read, declared modes, capabilities first (2026-10-02)

Fran approved B3b split three ways (pure surface / OSC 52 + paste
promotion / raw-mode layer + auscultator + look) and raw mode as a lib/
platform layer. This is the pure surface.

- **`rivus_eventum_coalitum`**: when the queue is EMPTY, decode
  everything present (until half the queue is used), then extract.
  Decoding only on an empty queue keeps the payload tables (text,
  samples) clearing, which they do only when the queue is empty; and it
  gives the spec's "coalesce per read": consecutive motions in one read
  become one MOTUS with samples. The lazy `rivus_eventum` stays for
  tessera's projection.
- **Declared modes**: `rivus_modos_intrare(r, modi, buffer, cap)`
  returns the bytes (rivus stays pure: it never writes) in the order
  ?1000 ?1002 ?1003 ?1006 ?2004 ?1004, then `CSI > 31 u`;
  `rivus_modos_exire` returns the exact reverse of what was entered
  (kitty popped first) and forgets it, so a second exit writes nothing.
  `?1003` only when SUPER is declared (spec Q24), and SUPER brings the
  mouse set with it. Entering sets `kitty_vexilla` (31) and
  `facultates.super`, and publishes FACULTATES again. Twice without an
  exit, or a buffer under `RIVUS_MODI_MAXIMUM`: nothing written, state
  untouched (kitty is a STACK in the terminal; a double push would
  leave one behind).
- **Capabilities first**: `rivus_creare` queues FACULTATES before
  anything else (tessera's projection drops it; its loop skips dropped
  events). The decoder's default `super` is now FALSE: nothing is
  hovered until ?1003 is declared (it said VERUM, "requested by the
  source", which tessera never requested).

**Bug found (latent since B3a-ii):** a printable run went to the
decoder in ONE step, and each rune makes KEY + TEXT, so 256 typed bytes
(no bracketed paste, or a fast typist over a slow link) made 512 events
for a 256-slot queue: half silently lost. Runs are now cut at 64 bytes
on a rune boundary (`CURSUS_MAXIMUS`), so one step makes at most 128
events. The capacity test now counts events, red before the fix.

**Amalgam:** tessera doesn't declare modes, so the amalgamator drops
`rivus_modos_*`; a file-scope static table left behind became an
unused variable under -Werror. The table now lives INSIDE the one
helper that uses it (`_modos_scribere`), and goes out with it.

**Tool trap (cost a confusing half hour):** with a broken amalgam,
`compile_tests.sh` fails at the amalgam step, does NOT rebuild lib
objects, and still runs the OLD test binary and reports its verdict. A
plant I had reverted kept "failing" because its build was the one
being run. Always read the runner's own output, not only the test log.

Tests: `probatio_rivus_terminalis` 69 (red first: capabilities first,
event count on a long run, coalesced read, mode bytes). Plants caught:
kitty not popped on exit; capabilities not first. tessera 1534/1534 +
68/68; the interpres, series, cauda, claves, eventus_stml suites green;
the four demos link.

## B3b-ii — OSC 52 write; paste → DEPOSITIO promotion (2026-10-02)

**Clipboard write is its own module, `copia_terminalis`.** The plan put
OSC 52 in rivus. Built there first, it pulled `base64` into tessera's
amalgam: tessera never writes the clipboard, the amalgamator dropped
the function, and base64.c's two FILE-SCOPE tables were left orphaned
(-Werror). Excluding base64 from tessera's policy then broke the demos,
which link `lib/rivus_terminalis.c` directly and do need it. Moving the
tables inside base64's functions would have been the general fix, but
base64.c isn't formatter-clean, so touching it meant a whole-file
reformat of a foundation library for this. The honest split was
simpler: rivus is INPUT, the clipboard write is OUTPUT.
`copia_terminalis_componere(piscina, textus, mensura)` returns
`ESC ] 52 ; c ; base64 ESC \` (ST, not BEL; empty text = an empty
payload). tessera never includes it, so base64 never reaches its
amalgam. The capability stays FORTASSE: a terminal never confirms.

**Paste → DEPOSITIO is DECLARED** (`RIVUS_MODUS_DEPOSITIO`, which brings
?2004 with it, like SUPER brings the mouse). Undeclared, a paste of
paths is text: an editor pasting `/usr/bin` wants text. Declared, the
capability says HEURISTICA (the decoder's default is now NULLA; it
claimed HEURISTICA unasked). Exit resets super and depositio.

The heuristic (`_vias_legere`, text only, no filesystem): tokens split
on unescaped whitespace, shell-style `\x`, `'...'`, `"..."` (with
`\" \\ \$ \``), `file://` and `file://localhost/` URIs with %XX
decoded; EVERY token must be absolute, with no newline or NUL, or the
paste stays text. `~/x`, an unterminated quote, prose with one path,
blank space: text. A truncated paste is never promoted. Output is
written to a second 64 KiB buffer (it never grows: separators and
escapes only shrink).

**Position: the last pointer cell the decoder saw** (Fran's call), cell
centre in our pixels, 0,0 when no pointer has been seen. The decoder
now remembers `indicator_x/y` in `_murem` (one computation shared by
press, motion and wheel). `interpres_depositio` pushes the event; the
queue copies the paths (`eventus_caudae_depositionem_impellere`, a view
into its text table like TEXT). Paths can't be truncated meaningfully,
so a full table REFUSES (FALSUM, nothing pushed) and rivus falls back
to the text event.

Tests (red first): `probatio_eventus_cauda` +9 (copy, view into the
table, refusal), `probatio_rivus_terminalis` 103, new
`probatio_copia_terminalis` 3. Plants caught: promotion without the
declaration; backslash escape ignored; position ignored; BEL instead
of ST. tessera 1534/1534; the four demos link.

## B3b-iii — raw mode as a platform layer; the terminal auscultator; the look (2026-10-02)

**`terminalis` (`include/terminalis.h`, `lib/terminalis_posix.c`)** -
the platform layer under rivus, which stays pure. `_posix` is the house
name for termios code (aedilis finds it through its `macos posix`
fallback chain; no config). Modelled on tessera's bridge, but the mode
bytes come from the caller (rivus), not fixed literals:
`terminalis_intrare(intrandi, n, exeundi, m)` copies both strings so the
signal handlers (fatal signals, TSTP, CONT) and atexit can restore the
terminal with `write` + `tcsetattr` only. Raw mode as tessera does it:
ISIG kept for SUSP alone, so Ctrl-Z really suspends and SIGCONT
re-enters; Ctrl-C and Ctrl-\ are keys. `terminalis_legere` uses `poll`;
`terminalis_amplitudo` reports pixels too (0 when unknown); one-shot
flags for "resumed" and "resized". SIGHUP is not in the house lexicon,
so the handler set is exactly tessera's.

**`rivus_modos_exeundi`**: the exit bytes WITHOUT leaving, so an app can
hand them to `terminalis_intrare` at entry; `exire` now reuses it. Red
first; plant (entry order instead of reverse) caught.

**`tools/auscultator_terminalis.sh`**: declares all six modes, reads
through `rivus_eventum_coalitum`, prints each Eventus as one STML line
(the format of fenestra's auscultator, so the two sources compare), adds
RESUMPTIO after `fg` and MUTARE_MAGNITUDINEM on SIGWINCH; Ctrl-C quits.
A headless smoke test via macOS `script` (a real pty) passed before the
look: modes out, capabilities first, a click at cell (5,3)'s centre, a
pasted path promoted to a drop there, exit bytes in exact reverse.

**The look (Fran, Ghostty + Terminal.app): everything working.** It
answered both B3 AUDIENDA:
- a Finder drop is a bracketed paste of the path in both terminals (no
  drop sequence), and promotion works;
- the DROP POSITION is stale: during a drag from another app macOS sends
  mouse events to the drag session, so the terminal reports nothing;
  the last position seen is where the pointer was BEFORE the drag. Both
  logs show the post-drop motion starting 200-640 px away in cell
  steps. Fran's question (Cmd+Tab: maybe the mouse really didn't
  move?) is answered by that first post-drop report: a still pointer
  would report one cell away. Decision (a): keep it, documented as the
  pre-drag pointer in rivus's and the decoder's headers; target a
  promoted drop by focus. Rejected (b), holding the drop ~50 ms for the
  next report: Ghostty's came after 12 ms, Terminal.app's after 320.
  Park 008 has both logs.
- Terminal.app reports pixel sizes too (13 px cells here), so the 10x20
  fallback was not needed in either.

The pty-based headless test of `terminalis_posix.c` waits for B4, which
needs the same harness (and a lexicon change for `posix_openpt`).

## B4a — the comparator learns excuses (2026-10-02)

**A correction first.** I had told Fran B4 needed a pty harness (and so a
lexicon change for `posix_openpt`). Reading A4's design showed it
doesn't: rivus is pure, so a terminal runner pushes bytes into it and
compares; no terminal, no window. The pty harness remains a later item,
only for `terminalis_posix.c`.

Walking the 8 scenes through the decoder by hand found what the
comparator must learn, and two differences no capability explained.
Fran decided both:
- **Modifiers compare on VOCABULARY bits only** (MOD_* + sides), for
  every source. fenestra's arrows carry AppKit's Function flag
  (0x800000), a raw platform bit A4 found and recorded; a terminal never
  has it. Recordings keep it; conformance ignores it.
- **A new capability `modificantes_textus`**: "modifiers on printable
  keys are reported". Legacy terminals send `A` for Shift+A (B2a chose
  not to guess: Caps Lock looks the same). fenestra TRUE; the decoder
  FALSE until kitty is seen with OMNES pushed (all keys as `CSI u`).
  Appended at the end of `EventusFacultates`; written like its siblings
  (no committed fixture holds a facultates line).

**The excuses (spec D7)**, named by capability in the scene attribute
`excusationes`, and valid ONLY when the stream's LAST FACULTATES deny
that capability. No FACULTATES in the stream means nothing is excused,
so fenestra (its FACULTATES are drained before the scenes) is exactly as
strict as before: 8/8.

| excuse | when the capability is denied |
|---|---|
| liberationes | expected key releases are dropped |
| codex_physicus | `codex` is not compared |
| latera | side bits are masked from `modificantes` |
| tabula_distincta | the whole scene is EXCUSATA (Ctrl+I is Tab) |
| modificantes_textus | `modificantes` is not compared |

`eventus_conformitas_comparare` now returns a verdict (FRACTA /
CONFORMIS / EXCUSATA); `tools/conformitas.c` prints EXCUSATA. An
unknown capability name makes the table bad, like an unknown kind.

Tests (red first): conformitas 39 (reading, the vocabulary mask, each
excuse with and without its capability, last FACULTATES wins, the
EXCUSATA verdict); eventus_stml round trip; interpres `Fac:LCTM`.
Plants caught: an excuse applied without its capability; the raw
modifier compare; the FIRST FACULTATES used. fenestra conformance 8/8;
pictor, villa, forum, both auscultators build; tessera 1534/1534.

## B4b — the terminal column + the terminal runner (2026-10-02)

**The table gained its terminal column**: each scene has
`<terminalis profilum="legacy|kitty" octeti="..."/>`, and the scenes
whose terminal stream legitimately differs name their excuses:

| scene | excusationes | legacy bytes | kitty bytes |
|---|---|---|---|
| shift-a | liberationes codex_physicus latera modificantes_textus | `A` | `\e[97:65;2;65u` + release |
| tabula | liberationes codex_physicus | `\t` | `\e[9u` + release |
| ctrl-i | tabula_distincta latera | `\t` (EXCUSATA) | `\e[105;5u` + release |
| sagittae | latera | `\e[1;2D\e[1;3C\e[A\e[B` | same + releases |
| effugium | codex_physicus | `\e` (then silence) | `\e[27u` + release |
| ictus, ictus-duplex-crudus, tractus | none | SGR at cell+1 | same |

**`octeti` uses C escapes** (`\e`, `\t`, `\r`, `\n`, `\\`, `\xHH`;
unknown = a bad table), decoded by the table reader: STML attribute
values are RAW by design (only named entities in TEXT are decoded).
**Finding in A4's table**: fenestra's `characteres="&#27;"`, `"&#9;"`,
`"&#xF702;"` were never decoded either, so fenestra has been injecting
the literal text `&#27;`. The scenes conform only because fenestra
emits no text for those keys. Noted in the table's comment; a follow-up
for Fran (decode `characteres` the same way).

**The runner is a suite test, `probatio_conformitas_terminalis`**, on
the SAME table: per scene and profile, a fresh rivus with 1x1-pixel
cells (cell c = pixel c-1, so fenestra's pixel expectations hold),
modes declared (mouse + paste, plus kitty for the kitty profile), bytes
pushed, the coalescing read, silence (`rivus_moram`), read again,
compare. The excuses come from the FACULTATES rivus itself publishes.
It NAMES what is excused: exactly one run, ctrl-i under legacy. A
silent excuse would be a dead gate.

**Result: 16/16 runs conform** (15 CONFORMIS + 1 EXCUSATA), first run.
Four compiling plants, each caught where predicted: the lazy read (the
drag's motions no longer coalesce: tractus x2); 10x20 cells (the three
mouse scenes x2); no silence (legacy escape); `latera` removed from
sagittae's excuses (both profiles). fenestra still 8/8 on the edited
table.

## B4b follow-up — `characteres` decoded with `entitates_html` (2026-10-02)

Fran asked whether entity decoding is general enough to live somewhere
shared. It already does: `lib/entitates_html` (born 09-15 for the
html/markdown clients) is a full WHATWG HTML5 character-reference
decoder. stml stays raw by design (byte-exact round trips). The table
reader now decodes `characteres` with it, so the A4 table needed no
edit: `&#27;`, `&#9;`, `&#xF702;` finally reach fenestra as ESC, Tab and
U+F702 instead of literal text. `octeti` keep their C escapes: an
entity names a CODE POINT, not a byte (`&#200;` is two UTF-8 bytes, and
HTML5 remaps C1, so `&#155;` would become U+203A, not the 8-bit CSI
0x9B). Rule for the file: characters as entities, bytes as escapes.

Red first (a key with `&#27;&#xF702;&amp;` must decode to ESC + EF 9C 82
+ `&`); plant (decoding skipped) caught. fenestra still 8/8 with the
real characters injected; the terminal runner 16/16.

## B6a-i — the key encoder `codificator_terminalis` (2026-10-02)

Fran split B6: replay INTO the terminal goes through an ENCODER (bytes,
then rivus), on A6's toy app with per-target geometry; the encoder is
its own library (`codificator_terminalis`; the house verb is
`base64_codificare`), with no DECCKM/keypad/X10 until the emulator.
B6a-i is the keys; B6a-ii the mouse, paste and focus.

**Oracle: Ghostty** (`../ghostty` @ 12752b2, `key_encode.zig`,
`function_keys.zig`, `kitty.zig`), its logic translated into the
Eventus vocabulary: key = clavis (or codex where the vocabulary has no
logical key, Insert), utf8 = the following TEXT (legacy: else `typus`,
because the decoder emits no TEXT under Ctrl/Alt), unshifted codepoint =
runa. We have no `consumed_mods`; the one Ghostty test that depends on
them (shift+a under disambiguate only) was left out.

**API**: `codificator_eventa(modi, eventa, n, aedificator)` encodes the
first event of a slice and returns how many it consumed: a key plus its
TEXT is ONE sequence (`A` in legacy, the kitty text field). What a mode
can't express writes nothing but is consumed (a release in legacy, a
modifier key without OMNES). `CodificatorModi` already holds the mouse,
paste, focus and Modulus fields for B6a-ii (API first).

**Canonical forms (Ghostty's)**, which also corrected B4b's table:
- no text on release, so no shifted alternate: Shift+A release is
  `\e[97;2:3u`, not `\e[97:65;2:3u`;
- press `:1` omitted in the `u`/`~` forms but INCLUDED in the special
  forms: `\e[1;2:1D`.
The table's kitty bytes were canonicalised; the conformance runner still
passes 16/16 on them.

**Tests (red first, 225):**
1. 69 ported vectors (34 kitty: plain text, Enter/Tab/Backspace in each
   mode, releases with and without OMNES, Shift/Alt+Backspace, modifier
   keys, alternates, associated text, Delete, F-keys; 35 legacy: C0
   table, Alt as ESC, xterm modifier params, SS3 F1-F4, modifyOtherKeys
   for modified Enter/Tab/Escape, Backspace with Ctrl/Alt, fixterms
   `CSI u` for Ctrl+I, macOS Command eats text, Ctrl+1 = `1`).
2. Consumption (key + TEXT = 2; key + key = 1).
3. The ROUND TRIP: every vector's bytes decoded through rivus and
   re-encoded must be byte-identical.
4. The TABLE ORACLE: the conformance table's key scenes (5 scenes x
   legacy/kitty) decoded and re-encoded give back exactly their bytes.

**The round trip found one decoder gap**, as predicted: xterm's
modifyOtherKeys `CSI 27;m;c~` (legacy Ctrl+Tab, Shift/Ctrl+Enter,
Ctrl+Escape) decoded to nothing. The decoder now reads it (6 red tests
first). That made 3 tessera vectors, which expected the form swallowed
as noise, see keys instead: updated per Fran's B3a precedent
("decoded now, formerly swallowed"), flagged for Fran.

**Also:** `claves_littera_ex_codex` (the inverse table, for kitty's
base-layout alternate; built by scanning the forward table so they can
never disagree; 48 round-trip asserts); `codificator` entered the
glossary. Plants caught: Shift dropped from the modifier number (29
asserts); `:1` omitted in special forms; legacy encoding a release; the
following TEXT not consumed. The functional-key lookup is a table
inside its function (the formatter mangled dense `casus` lines).
