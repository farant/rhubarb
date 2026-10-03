# eventus — D2 step 3 inventory: `typus` and the f32 scroll deltas

Written 2026-10-03, after phase B (rhubarb-secunda / main ab959fdc).
Spec D2 step 3: "`typus` and the f32 deltas are deleted; the old widget
generation is either migrated or keeps a local projection. That is
decided at step 3 with an inventory." This is that inventory.

How it was taken: every access path `clavis.typus` / `clavis->typus`
and `rotula.delta_x/y` in `.c`, `.m` and `.h` (amalgams and build
excluded; `apps/` included: none there). The semantic rename tool
(`silva/renominare.sh -membrum`) refuses here: it handles direct
members only, and these sit two anonymous structs deep in `Eventus`'s
union (`datum.clavis.typus`). The access path is unique to `Eventus`,
so a path search is exact for this inventory. (The tool gap is worth
its own fix.)

## 1. The finding that changes the premise

**`typus` is not redundant.** The spec assumed TEXT replaces it. But
under Alt, Ctrl and Cmd no TEXT event is emitted, and `typus` is then
the ONLY record of the character the key produced:
- Alt+`A` vs Alt+`a` (B3a-i: the decoder fills `typus` with the actual
  character precisely for this);
- Shift+Alt+`.` must encode as `ESC >` (B6a-i: the encoder's legacy
  path reads `typus` where Ghostty reads `utf8`);
- `runa` can't stand in: it's the UNSHIFTED key (`.` here, not `>`),
  and the shifted character depends on the keyboard layout.

What's actually wrong with `typus` is its TYPE: one signed `character`,
so ASCII only. fenestra truncates `characters[0]`; a non-ASCII
character under Alt (Option+key on macOS) is lost.

## 2. The sites (41 in 17 files)

**Producers (5 + tests)**
| file | role |
|---|---|
| `lib/fenestra_macos.m:766` | `characters[0]` (truncated to `character`) |
| `lib/interpres_terminalis.c:74` | `_clavem_typo`: the actual character (B3a-i) |
| `lib/manus_ludus.c:342` | synthetic keys (`manus_ludus_clavem`) |
| `lib/eventus_stml.c:300, 616` | writes / reads the `typus` attribute |
| tests | `probatio_dispensator`, `probatio_eventus_stml` (3), `probatio_codificator_terminalis`, `probatio_iteratio_transversa`, `probatio_interpres_terminalis` (helper) |

**New-stack consumers with a REAL need (3)**
| file | why it needs the produced character |
|---|---|
| `lib/codificator_terminalis.c:576-579` | legacy Alt/Ctrl encoding without TEXT |
| `tessera/fontes/tessera_eventum.c:146` | the projection's rune (printable, not Ctrl) |
| `probationes/probatio_interpres_terminalis.c:316` | test helper |

**New-stack consumer that should move to the logical key (1)**
| file | today | should be |
|---|---|---|
| `probationes/ludus_toy.h:293` | `typus == 27` (Escape) | `clavis == CLAVIS_EFFUGIUM` |

**The old widget generation (the concha family; 25 sites, 8 files)**
| file | sites | pattern |
|---|---|---|
| `lib/thema_visus.c` | 6 | command letters h l f b j k |
| `lib/librarium_visus.c` | 5 | `commutatio (typus)` command letters |
| `lib/navigator_entitatum.c` | 4 | h j k l |
| `lib/biblia_visus.c` | 1 | `commutatio`: j k l h |
| `lib/pagina.c` | 1 | `convertere_clavem(clavis, typus)` (vim) |
| `lib/elementa.c` | 1 | text field: typed character |
| `lib/arx_caeli.c` | 3 | text field: "typus (respects shift/caps)" |
| `probationes/probatio_navigator.c` | 2 | Escape / 'q' |

Two patterns: COMMAND LETTERS (vim-style h/j/k/l; case matters, so
they want the produced character, which is what `typus` gives today)
and TEXT ENTRY (elementa, arx_caeli insert the typed character; the
lossless way is to consume EVENTUS_TEXTUS, a semantic change, since
TEXT arrives as a separate event). This family is also park 009's
subject (migrating concha / probatio_combinado to the new stack).

## 3. The f32 scroll deltas

| file | role |
|---|---|
| `lib/fenestra_macos.m:469-470` | producer (`scrollingDeltaX/Y`) |
| `lib/interpres_terminalis.c:355-356` | producer (copies dx/dy) |
| `lib/eventus_stml.c:322-325, 632-633` | writes / reads |
| **`lib/importatio_visus.c:695`** | **the one consumer: zoom = `delta_y * 0.5f`** |
| `probationes/probatio_eventus_stml.c` (5) | tests |

Spec D2 step 2 listed `importatio_visus.c` → integer scroll. **That
migration was never done**; it's the only thing keeping the f32 deltas
alive. After it, they can be deleted with no loss (`dx`/`dy` + `genus`
carry everything; the stml reader keeps reading old recordings'
attributes into dx/dy).

## 4. Decisions for Fran

1. **`typus`: delete, or replace with an honest field?** Recommended:
   REPLACE. A widened, honestly named field, e.g. `s32 producta` (the
   character the key produced, full Unicode, 0 = none), filled by
   fenestra (all of `characters`, not one byte) and the decoder; every
   reader switches; `typus` is deleted. The deprecation becomes a
   correction.
2. **The old widget generation:** (a) switch its 25 reads to the new
   field now (mechanical, same semantics for ASCII), leaving the real
   migration to park 009; (b) migrate it properly now (command letters
   by key, text fields to TEXT events); (c) a local projection helper
   in the old generation. Recommended: (a). It removes the deprecated
   field without changing behaviour, and the real migration belongs
   with park 009.
3. **The f32 deltas:** migrate `importatio_visus` to `dy` + `genus`,
   then delete them (recommended; this is overdue step 2 work).
4. **`ludus_toy.h`'s Escape check** → `clavis == CLAVIS_EFFUGIUM`
   (recommended; logical key, as A5 did for pictor and the dispensator).

Cost of the recommended path: a vocabulary change in `eventus.h` (wide
gates), about 45 mechanical sites, `eventus_stml` keeps reading the old
`typus` attribute into the new field (old recordings stay valid), plus
the toy and importatio changes. One task, maybe two (field + readers,
then the deltas).

## 5. DECIDED (Fran, 2026-10-03)

1. `typus` → **`s32 producta`** (replace; delete `typus`).
2. Old widget generation: **switch its reads to `producta`** (same
   behaviour for ASCII); the real migration waits for park 009.
3. f32 deltas: **migrate `importatio_visus` to `dy` + `genus`, then
   delete `delta_x` / `delta_y`.**
4. `ludus_toy.h` Escape → `clavis == CLAVIS_EFFUGIUM` (the recommendation,
   taken with 1).

Tasks: **S3a** `producta` in, every reader and producer switched,
`typus` deleted, stml reads old `typus` attributes into `producta`;
**S3b** importatio to integer scroll, f32 deltas deleted.

## 6. S3a DONE (2026-10-03)

`typus` deleted; `s32 producta` in `eventus.h`. fenestra fills it from
the FIRST Unicode scalar of `characters` (surrogate pairs joined;
AppKit's private-use function-key characters 0xF700-0xF8FF give 0);
the decoder fills it with the full rune (Alt+é now survives); manus
from its synthetic character. eventus_stml writes `producta` and reads
an old recording's `typus` attribute into it. Readers: the encoder's
legacy path encodes `producta` as UTF-8; tessera's projection uses it;
the old widget generation reads `producta` (ASCII semantics kept,
non-ASCII given as 0 where a `character` is needed); the toy checks
the logical key (`CLAVIS_EFFUGIUM`), and `toy.eventus.stml` now says
`clavis="27" producta="27"`.

## 7. S3b DONE (2026-10-03) — D2 step 3 complete

`importatio_visus` zooms from `dy` + `genus`: a wheel notch is
`dy / gradus_rotulae × 0.5` (the gradus from the source's FACULTATES;
16 for fenestra, the cell height for the terminal), trackpad and old
recordings `dy × 0.5` (our pixels ≈ points at scale 1). Both equal the
old `scrollingDelta × 0.5`. `delta_x` / `delta_y` deleted from
`Eventus`; fenestra and the decoder no longer fill them; eventus_stml
no longer writes them and rounds an old recording's `delta_*` into
`dx`/`dy` when those are absent (present ones win, even 0).
