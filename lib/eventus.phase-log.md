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
