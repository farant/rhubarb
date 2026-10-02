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
