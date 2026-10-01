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
