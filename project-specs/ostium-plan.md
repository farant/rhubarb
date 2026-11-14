# ostium — implementation plan

> **For agentic workers:** house execution, not the skill default:
> inline in the secunda worktree, one task per turn, Fran approves each
> task's header text before code, commit only on Fran's word through
> the commissio script. Steps use checkbox (`- [ ]`) syntax.
> Signatures below are proposals; the header Fran approves at the
> start of each task is what binds.

**Goal:** in `$world`, walk to the hut, see `[E]`, press E, be in the
hut scene; take the scene's way out and stand at the hut again.

**Architecture:** doorways are level data kept beside the level in
the volume (their own act); the scene pane grows a hosted form; the
world pane reads doorways, shows the prompt, and embeds the hosted
scene while inside.

**Tech stack:** C89 + latina.h; house libraries only.

**Spec:** `project-specs/ostium-spec.md` (agreed by Fran 2026-11-13).
Read it first; decision numbers below refer to its section II.

## Global constraints

- The global constraints of `project-specs/graphis-plan.md` hold
  unchanged (Latin identifiers at NOVA 0, `./silva/scribe.sh` for new
  C files, `examen` ACCIPE, `formator` CONFORMIS, TDD with compiling
  plants, new code in new modules, both subsystem runners before a
  commit, tests write only under `credo_area(c)` and close temporary
  volumes, no raw IO outside `filum_*` / `lectiones_*`).
- `scaena*` headers include no `mundus*` header. The world pane may
  include the scene pane.
- The scene pane's observable behaviour does not change: the 9
  sections of `probationes/probatio_scaena_latus.c` pass without
  edits to their assertions.
- Same sheet, same bytes: doorway numbers are stored as integer
  thousandths.
- Only `bin/mundus condere`, run by Claude in task 5, writes into
  Fran's live volume (standing OK, 2026-11-11).

## Review focus

1. A broken doorway line: missing or non-numeric `locus`, radius
   outside 0.5 .. 8, bad scene title, unknown attribute, child
   elements, more than 64. Refused with reason and line; nothing
   written. (T1)
2. A doorway nobody can reach: outside the level, or no walkable
   cell within its radius. Refused at build. (T1)
3. The level rebuilt while vicus runs and the character is inside,
   or the doorway list changed under a character standing in range:
   no crash, prompt follows the new list, leaving still lands on
   walkable ground. (T3, T4)
4. A doorway to a scene that is absent, removed while inside, or
   unreadable: the message and a way back, never a stuck pane. (T4)
5. Keys held while entering (W down, press E, release W inside):
   the character does not walk on its own after leaving. (T4)

## Files

| File | Task | Is |
|---|---|---|
| `lib/mundus_locus.c`, `include/mundus_locus.h` | 1 | `<ostium>` read from the level sheet |
| `include/mundus_ostia.h`, `lib/mundus_ostia.c` (+ worklog) | 1 | doorway checks, the act, reading the list back |
| `probationes/probatio_mundus_ostia.c` | 1 | new suite |
| `tools/mundus.c` | 1 | `condere` writes doorways with the level |
| `include/scaena_latus.h`, `lib/scaena_latus.c` | 2 | hosted form |
| `probationes/probatio_scaena_latus.c` | 2 | new sections only |
| `include/mundus_pictura.h`, `lib/mundus_pictura.c` | 3 | a public "world point to world pixel" |
| `include/mundus_latus.h`, `lib/mundus_latus.c` | 3, 4 | prompt; enter, leave, memory |
| `apps/mundus/organum.stml` | 4 | three fields |
| `probationes/probatio_mundus_latus.c` | 3, 4 | new sections |
| `apps/mundus/loci/clearing.stml` | 5 | the hut doorway |

---

### Task 1: doorways in the level sheet and in the volume

**Produces:**

```c
/* mundus_locus.h */
nomen structura {
    chorda scaena;   /* ut scripta; non probata hic */
       f32 x;
       f32 z;
       f32 radius;   /* ordinarius 1.5 */
       i32 linea;
} MundusLocusOstium;
/* MundusLocusFructus += MundusLocusOstium* ostia; i32 ostiorum; */

/* mundus_ostia.h */
#define MUNDUS_OSTIA_ACTUM    "ostia-posita"
#define MUNDUS_OSTIA_MAXIMA   LXIV
nomen structura {
    chorda scaena;
       s32 x_millesimae;
       s32 z_millesimae;
       s32 radius_millesimae;
} MundusOstium;

/* probat (spec 5) contra tabulam gressus loci instructi */
MundusOstiaFructus mundus_ostia_probare (
    constans MundusLocusOstium* ostia, i32 numerus,
    constans MundusSolum* solum, Piscina* piscina);
b32  mundus_ostia_ponere (Volumen* v, chorda locus,
    constans MundusOstium* ostia, i32 numerus);
Xar* mundus_ostia_legere (Volumen* v, chorda locus, Piscina* p);
```

- [ ] Header text to Fran; wait for his word.
- [ ] Tests red against a stub: sheet with zero, one, several
  doorways; each refusal of review focus 1 and 2 with its line;
  default radius; write then read back equal; same list written
  twice writes nothing; a newer act replaces; empty list clears;
  two levels do not see each other's doorways; an unknown act kind
  in the volume is ignored.
- [ ] Implement; `./compile_tests.sh mundus_ostia`,
  `./compile_tests.sh mundus_locus`.
- [ ] `tools/mundus.c`: `condere` checks doorways after the level is
  built, writes level and doorways in one transaction, prints
  `ostia N` per level. A refused doorway refuses the level (nothing
  written), as other refusals do.
- [ ] Plants (silva.planta), each red.
- [ ] Commit on Fran's word.

**As built (2026-11-13).** As the header Fran approved. Suite
`probatio_mundus_ostia` has 7 sections; 30 library plants and 8 tool
plants (hand, `tools/mundus_fumus.sh`), all red. Added beyond the
plan: `MUNDUS_LOCUS_OSTII_RADIUS`; the smoke script's doorway cases
(doorway alone changed, each field alone changed, refused outside,
refused inside a solid tower); "reachable" tested with the body
radius 0.5 and as a circle (a water-cross fixture pins it). Level
output line now ends `, ostia N`.

### Task 2: the scene pane's hosted form

**Consumes:** `ScaenaLatusMontatio` and its statics as they are.

**Produces:**

```c
/* hospes: latus aliud scaenam in ramo SUO ostendit. Sine organo,
 * sine registris; sagittae et [ ] absunt. Campi 'prospectus' et
 * 'tactum' in ramo hospitis sint, scriptore dato. */
b32 scaena_latus_montare_hospitem (
    ScaenaLatusMontatio* m, Piscina* piscina,
    InternamentumChorda* intern, Volumen* volumen,
    InsulaRamus ramus, constans character* scriptor,
    constans character* quadri_titulus,
    i32 latitudo, i32 altitudo);

nomen enumeratio {
    SCAENA_LATUS_NIHIL = ZEPHYRUM,
    SCAENA_LATUS_MUTATUM,
    SCAENA_LATUS_FORAS      /* exitus "mundus" tactus */
} ScaenaLatusEventum;

ScaenaLatusEventum scaena_latus_agere_hospitis (
    ScaenaLatusMontatio* m, constans Destinatio* des,
    Componens* nodus, constans Eventus* ev);
vacuum scaena_latus_pingere_hospitis (
    ScaenaLatusMontatio* m, constans Componens* c, Mandata* md);
vacuum scaena_latus_areas_addere (
    ScaenaLatusMontatio* m, Componens* radix, Piscina* piscina,
    InternamentumChorda* intern, constans character* actio);
/* VERUM si prospectus legi potest (causa: scaena_latus_causa) */
b32 scaena_latus_valet (constans ScaenaLatusMontatio* m);
```

- [ ] Header text to Fran; wait for his word.
- [ ] Tests red: a hosted mount in a ramus of another kind shows the
  view named by that ramus's `prospectus`; touch, exit between
  views, `j` `k` Enter work; `[` `]` and arrow clicks do nothing; a
  `mundus` exit returns FORAS and shows its text; a missing view
  gives `scaena_latus_valet` FALSUM.
- [ ] Implement by routing the existing static functions through the
  hosted entry points; the pane's own `agere` and figure call them.
- [ ] The 9 existing sections pass with no assertion edited; re-run
  the 17 existing plants: all still red.
- [ ] New plants, each red.
- [ ] Commit on Fran's word.

**As built (2026-11-13).** As the header Fran approved, plus one
private field (`foras`, the flag behind FORAS) and a `monitum`
parameter (the host's hint under "cannot be shown", so the scene
code names no key). Differences from `$scene` when hosted: no
arrows, no "n/m", no `[` `]`; an unknown or empty `prospectus` shows
the message, never the first view; a `mundus` exit writes nothing
and returns FORAS, so its own line is not shown. Suite
`probatio_scaena_latus` has 14 sections (5 new, none of the 9 old
assertions edited); 18 new plants red, and the 16 plants of graphis
T8 re-run and still red.

### Task 3: the prompt

**Consumes:** `mundus_ostia_legere` (T1).

**Produces:** `mundus_pictura.h`:
`vacuum mundus_pixelum_puncti (StereometriaVector punctum, s32* x,
s32* y);` (today's static `_pixelum_mundi`, `lib/mundus_pictura.c:566`).
`MundusLatusMontatio` gains the doorway list (re-read with the
level) and `s32 ostium_propinquum` (-1 = none; kinetic).

- [ ] Header text to Fran; wait for his word.
- [ ] Tests red: no doorways, no prompt; character outside the
  radius, none; inside, the prompt's box is at the projected point
  (spec 7) and does not move in the world when the character steps;
  two in range, the nearer; clamped to the picture edge when the
  point is off it; doorway list replaced in the volume, prompt
  follows within one volume check.
- [ ] Implement: nearest-doorway check each pulse and after each
  level read; draw `[E]` box in the figure; a click area `ostium`.
- [ ] Headless screenshot to Fran (copy the recipe of
  `build/iactura/scaena_imago.c`).
- [ ] Plants, each red. Commit on Fran's word.

### Task 4: enter, leave, memory

**Consumes:** T2's hosted form, T3's `ostium_propinquum`.

**Produces:** `apps/mundus/organum.stml` fields `prospectus`,
`tactum`, `ostium` (persistent, writer `mundus.agere`);
`MundusLatusMontatio` embeds a `ScaenaLatusMontatio`; then
`./tools/organa_infigere.sh`.

- [ ] Header text to Fran; wait for his word.
- [ ] Tests red: E in range enters (state fields set, held keys
  cleared, world stops stepping); E out of range does nothing; a
  click on the prompt enters; inside, W A S D and `g` do nothing;
  the scene's `mundus` exit leaves, character at the walkable cell
  nearest the doorway; a remount with `prospectus` set starts
  inside, and leaving then still lands at the doorway; missing
  scene shows the message, E leaves (spec 9); doorway spot no
  longer usable after a rebuild, leaving lands at the level start;
  review focus 3 and 5.
- [ ] Implement.
- [ ] Headless screenshots (inside; missing scene) to Fran.
- [ ] Plants, each red. Both subsystem runners. Commit on Fran's
  word.

### Task 5: the hut, and finish

- [ ] Find the hut door on the map sheet (`bin/mundus mappa`,
  `bin/mundus quadrum`); add the `<ostium>` to
  `apps/mundus/loci/clearing.stml` leading to `hut-door`. If the
  clearing has no hut to stand at, say so to Fran before choosing a
  spot.
- [ ] `bin/mundus condere -volumen ~/.rhubarb/paginae-proba.volumen`
  (standing OK); rebuild `bin/vicus` in secunda.
- [ ] Spec: status BUILT and an "As built" section. Worklogs. Close
  ledger question `01M7BBC3T43PAQFSSAAEEPQK6M`; file what is left
  open. Memory files.
- [ ] Commit on Fran's word.
