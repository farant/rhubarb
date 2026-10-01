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
