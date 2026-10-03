# amalgamatio — worklog

## 2026-08-20 — the day the amalgamation machinery got exercised

Started as one item on the arbor plan (T1: a freshness gate, because
`amalgamare.sh` rebuilds from committed manifests and nothing checked
them). Finished having found that **two of three projects could not
amalgamate at all**, and had not been able to for months.

### What was broken, in the order it surfaced

**1. The excludenda generator, dead for silva.** Its amalgamator build
recipe is a duplicate of the one in `amalgamare.sh`. When
`silva_unitates.c` was extracted (fed3e78) only silva's copy grew the
new source, so the generator failed to link: `Undefined symbols:
_silva_lineam_finire`. Nobody knew, because you only run it when
adding a module or changing which vendored functions get called —
rare events.

**2. It destroyed the committed manifest when it failed.** The fixpoint
writes `excludenda_generata.h` on every iteration (the amalgamator
`#include`s it and is rebuilt each round) — and once BEFORE the loop
with EMPTY lists, since that is the fixpoint's starting point. So a
first-round failure left the file gutted: 153 entries gone, silently.
Measured the hard way — it happened to me. Now snapshots up front and
restores through a `trap`; only success keeps the new file.

**3. Manifests stale in all three projects.** `postulata_posix.h`
entered silva's closure through `silva_c89_semantica` at 7ba8d84 and
never reached any manifest. The gate caught silva on its first run and
tessera + officina the moment they were probed.

**4. tessera and officina could not amalgamate.** Same `silva_unitates`
divergence, two more copies. `AMALGAMA HORUM DUORUM PROIECTORUM
CONFICI NON POTERAT` — and it took building the gate to find out,
because building the gate meant *running* them.

**5. haruspex red, and dark.** Its exclusion for `systema_c89.h` was
written as one NAME when the reasoning covered a CLASS (lexicon files
carry synthetic types and cannot compile standalone). `systema_posix.h`
became the second lexicon; the condition never grew. Now green: 243
TUs, 7,503 shape assertions against clang.

### The root cause, stated once

**Single cases that should have been classes.** One hardcoded filename
in haruspex. One build recipe copied four times. One manifest entry
nobody re-derived. Each was fine until the second member of its class
appeared — and every failure was silent, because the tools that would
have complained were the tools nobody ran.

### Two drift modes, and they are NOT alike

Worth keeping straight, because it decides where gates are worth paying
for:

- **fontes drift (a file missing from the manifest) is SILENT.** The
  file is never read, so nothing references it, so nothing errors.
  Measured: with a planted module and a stale manifest, the amalgamator
  succeeded, the module was absent (grep = 0), and the output compiled
  clean under the full flag set.
- **excludenda drift (a pruned function gaining a caller) is LOUD.**
  The amalgam references an undefined symbol and the standalone compile
  fails. This is what bit T0: a new static helper in lib/stml.c called
  `chorda_aedificator_spectare`, pruned as dead since nothing had ever
  called it.

The exploratory pass had reported both as silent. Only measurement
separated them. The root suite's manifest turns out to be the loud kind
too (`SOURCE_FILES` feeds clang directly) — its own header says so:
*nexus fractus clamat*.

### Gate placement — a flaw in my own first design

Gate 0 was placed before any building, reasoning that failing fast
beats discarding work. But the excludenda probe LINKS an amalgamator,
which needs warm objects — so on a cold tree the gate breaks itself.
Silva hid this because its objects were already warm. tessera and
officina, whose `mech_silva_unitates.o` had never existed, exposed it.
The gate now runs after objects and before amalgamation, keeping nearly
all the fail-fast value.

### Costs, measured

- committed amalgams compile: **0.9s** (all three)
- excludenda probe: **7s**; fontes probe: a few seconds
- full regeneration, warm objects: **silva 53s, officina 16s,
  tessera 10s → 79s**
- root manifest probe: 36s
- haruspex: 121s

Which is why the tiers ended up where they did: the 0.9s artifact check
runs in the ordinary suite; the regeneration gates ride `amalgamare.sh`;
the 36s and 121s checks want a deliberate slow tier that does not exist
yet.

### Still open

- `--excludenda <path>` on the amalgamator, so the generator physically
  cannot write the committed file (today's trap is a mitigation; the
  class survives).
- 11 of 19 generated files do not declare their regeneration command —
  finish that and a `recens.sh` sweep can verify all of them by reading
  their own banners.
- Nothing checks whether a committed amalgam is stale relative to its
  SOURCES. The gates check manifests; the suite checks the artifact
  compiles. "Is the artifact current?" is still unasked, and is the
  cheap check that would tell you when the 79s is owed.
- haruspex runs nowhere.
- The amalgamator build recipe is shared for silva's three sources and
  the two-file mechanism, but tessera/officina still name their own
  `amalgamator.c` inline. One source each, so drift risk is low.

## 2026-09-25 — silva-migratio T13a: the derivation layer learns materia

A dry run of the thaw (scratch snapshot of HEAD, the real tree and the
frozen amalgam untouched) broke in three layers, each hidden by the one
above:

1. **aedilis could not see materia at all.** `aedilis.stml` listed no
   `materia/fontes` root, so `materia_token.h` & co. were classified
   `S` (system, like `string.h`) and dropped from every closure —
   silently (fontes drift is the SILENT kind, per the 08-20 entry). The
   same blindness held for css/md/crusta/html. Fix: one `<via>` line.
   `materia_arbor_aequalitas.c` has no header of its own, so the probe
   by name never finds it; `materia_arbor.h` now carries
   `<aedilis corpus="materia/fontes/materia_arbor_aequalitas.c"/>`
   (the arbor2_glr tabula precedent — annotations compose with the
   probe). Measured blast radius of the new root: of the 40
   aedilis-generated manifests in the house, ONE changed (silva's
   amalgam manifest: 42 capita, 39 corpora, +7 materia bodies + frons
   + lexicon_c89); the other 39 byte-identical.
2. **The amalgamator binary links silva_token.o**, which since T7b is a
   MateriaToken + C89 tail → `materia_token.o` needed in all three link
   lines (amalgamare, excludenda harvest, ligare). Then the single TU
   exposed 7 static helpers defined in both a materia body and its
   silva counterpart (arbor 4, aequalitas 2, scribere 1) — some true
   copies (`_scriptura_fracta`, `_spatium_solum` identical modulo the
   typedef), some homonyms. Silva's side renamed (`_silvae_*`,
   `_parsurae_*` for the parsura comparator). Deduplicating the true
   copies is T19's.
3. **silva.h is hand-written and owns the public types** (CADENDA) —
   the facade's `#define SilvaNodus MateriaNodus` contradicts it
   ("conflicting types" on silva's own functions). That is T13b.

**ligare was dead twice over.** Baseline run before any edit: fatal
`materia_token.h not found` (since T7b). After the include fix: undefined
`_silva_lineam_finire` — it named `silva_amalgama.c` alone, never
switched to `tools/mechanismus_fontes.sh` when silva_unitates.c was
extracted (fed3e78). The exact class the mechanism table was built to
end, one member missed, and nothing runs ligare so nothing knew. Now
sources the table; green on lib/xar.c and on materia/fontes/materia_nodus.c.

renominare refuses dirty files (git = its undo), so after the first
write per file the remaining statics went by word-boundary replace,
each gated on renominare's own plan count == occurrence count.

## 2026-09-25 — silva-migratio T13b: the amalgam thaws onto materia

First regeneration since the T7b freeze. How materia got in, and what the
compile found on the way:

- **Materia is vendicata, exactly like lib/.** Every identifier token of
  every file passes the rename tables, macro bodies included, so the
  facade needed no special case: `Materia*` types → the `Silva*` names
  silva.h already publishes (14, TYPI_EXACTI), enumerators likewise
  (MATERIA_VALOR_*/LOCUS_*/ARBOR_COMPARATIO_* → SILVA_*), `materia_` →
  `silva_materia_` (PRAEFIXA). `#define SilvaNodus MateriaNodus` comes
  out as `#define SilvaNodus SilvaNodus` — legal, inert. CADENDA lists
  the ORIGINAL materia names (the unit scanner reads pre-rename tokens,
  as with "Piscina").
- **silva.h rewritten where materia's layout differs:** SilvaToken (valor,
  exact trivia arrays + counts, genus int, …, vexilla; the C89 tail is
  internal, accessors only), REFERENTIA added to both enums (materia
  appended them last, so ordinals match). Three public functions became
  facade macros → `#define silva_nodus_liberi silva_materia_nodus_liberi`
  in silva.h + NON_STATICA (the internal facade macro, after renaming, is
  a token-identical redefinition). `silva_token_genus` published: with
  `genus` an int, hospes' `silva_lexema_genus_nomen(primum->genus)` tripped
  -Wsign-conversion.
- **One macro could not stay a facade:** `SILVA_ARBOR_VIA_CAPACITAS` is a
  macro in silva.h (256); the facade's `MATERIA_ARBOR_VIA_CAPACITAS` body
  would be an incompatible redefinition in the one TU. Back to the literal
  in silva_arbor.h + a typedef-array assertion tying it to materia's.
- **Collisions S41 cannot see:** S41 checks duplicate static FUNCTIONS
  only. Data (`HEX_CIFRAE`, identical copy) and a file-local typedef
  (`ArborComparator`) collided too; a one-off awk sweep of file-scope names
  (macro/typedef/tag/static data) over materia vs silva bodies found only
  these. Silva's side renamed (`HEX_CIFRAE_SILVAE`, `ComparatorParsurarum`).
- **Companion bodies.** `materia_arbor.h` declares the comparator whose
  body is `materia_arbor_aequalitas.c` (the aedilis corpus annotation).
  Per-base excludenda put `materia_arbor_aequalis` in the aequalitas list
  while its prototype sat under the arbor header's list → a static
  prototype with no definition, -Wunused-function. Policy now has
  `corpora_socia_pro` (one header, several bodies, ONE list); the
  harvest's classifier searches `lib/`, `materia/fontes/` and companions.
  Harvest: 165 names, 6 rounds.
- **S43 held by a rename:** `LEXICON_C89` (silva_lexicon_c89.c, entered
  with the frons) was an unprefixed external → `SILVA_LEXICON_C89` in the
  amalgam only. Exports 390 before and after; all `silva_*` now; the only
  materia externals are the three `silva_materia_*`.

**The bootstrap, and a hazard.** aedilis is built from the COMMITTED amalgam:
`aedilis.c` against the live `silva.h`, linked with `silva.o` from
`silva.c` — which embeds the OLD header verbatim. After the SilvaToken edit,
aedilis read token fields at wrong offsets, missed the `<aedilis corpus>`
comment, and the silva manifest silently lost a body (39 → 38). And
amalgamare's gate 0 needs aedilis. Bootstrap used: HEAD's silva.h+silva.c
for aedilis → derive the manifest → restore the new silva.h → harvest →
amalgamator once by hand → full `amalgamare.sh` (byte-identical to the
bootstrap output). Desideratum …XHAW3H names a guard.

**renominare on a type name** renamed the homonymous, unrelated file-local
typedef in materia and in the committed silva.c — reverted by git; question
…SFJ3T. (Static data was correctly scoped the same day.)

Gates on the thawed amalgam: amalgamare VERIFICATUM (standalone, hospes
40/40, nm 0, censor); amalgamata 3/3; aedilis 187; saltuarius 13/13; briar
19/19; officina 15/15 — consumer objects verified newer than silva.c.

## 2026-09-25 — half-amalgam closed, derived artefacts gated (silva-migratio T19a; …XHAW3H)

aedilis now compiles against the header EXTRACTED from silva.c
(`tools/amalgama_caput.sh` → build/aedilis/caput/silva.h): the pair is
consistent by construction, so amalgamare's circle (it needs aedilis,
aedilis links the committed silva.c) no longer needs the hand bootstrap.
It warns when the live silva.h differs, and does not refuse (refusing
would block the ordinary "edit silva.h, then amalgamare" flow T17b used).
Plant: a field added to SilvaToken in the live silva.h → new path:
monitum, same aedilis output (26 lines); old path (-I silva/amalgama)
segfaults (rc 139) — the T13b failure mode, reproduced.

Commit-time teeth for every other consumer: new gate `generata`
(tools/generata_probare.sh), sections:
 I   amalgam pairs silva/officina/tessera: live .h == header inside .c;
 II  silva_lexicon_c89.{c,h} == generator (whitespace-normalized — the
     committed file is formatted after generation);
 III 37 aedilis snippets regenerated IN PLACE by each one's
     '# regeneratio:' command, compared to copies, always restored (trap);
 IV  amalgamator manifests via tools/porta_vetustatis.sh — which existed
     since 08-20 (planted then) but ran only inside amalgamare.sh, so no
     commit ever owed it.
The aedilis.stml plant (materia root deleted) is invisible to the
aedilis gate (187 green) and to III (snippets list lib/*.c only); IV
catches it (silva manifest loses every materia header). Three plants at
once → three named STALA; ~2m20s. macOS bash 3.2 has no mapfile.

## 2026-10-03 — the excludenda harvest could converge FALSELY

Vendoring `stilus_terminalis` into tessera (module 004 T3) needed a rename
prefix that is NOT `tessera_` + old name: the codec's `stilus_nativus` /
`stilus_aequalis` would become `tessera_stilus_nativus` / `_aequalis`,
which tessera already defines (same TU → redefinition). Chosen:
`stilus_` → `tessera_stilus_terminalis_`.

The harvester reversed warning names by stripping `<proiectum>_` only, so
`tessera_stilus_terminalis_applicare` → `stilus_terminalis_applicare`, a
name that exists nowhere. It was still CLASSIFIED (longest base-prefix
match `stilus_terminalis_`), listed, never dropped by the amalgamator —
and on gyrus 2 the same warning returned as "already listed, not new",
so NOVAE=0 read as a fixed point. Exit 0, a list of fictional names; the
only witness was amalgamare.sh's -Werror standalone build two steps later
(2 unused-function errors).

Fixes: (1) tessera's amalgamator `--enumerare` also prints its rename
tables (`TE` exact, `PF` prefixes); the harvester reverses through THAT
table (longest novum prefix), falling back to stripping `<proiectum>_`
for amalgamators that print none (silva, officina — their renames are
all of that form). (2) Invariant: a name already listed cannot come back
as unused (the amalgamator would have dropped it) — returning = fracture,
named. Plant (empty table = old reversal): "nomen exclusum rediit:
stilus_terminalis_applicare (… gyrus 2)". Lesson in the house shape: "no
new names" was a proxy for "no warnings"; the proxy forgave a name that
never matched.
