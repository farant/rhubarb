# fabrica plan 2 — reads observed, objects shared

> **For agentic workers:** execute task by task with the executing-plans
> discipline (test first, run red, implement, run green, plant, commit),
> INLINE on main, ONE TASK PER TURN with Fran's approval, no subagents.
> Several tasks OPEN with a measurement or with names; a result outside
> the named branches is shown to Fran before work continues. Steps use
> checkbox syntax. Written 2026-10-02 from `fabrica-spec-2.md` (v2,
> 08a4eeee; interview Q1–Q24 in `fabrica-interview-2.md`). On approval:
> one opus per task in park 01KZYN4VPZ; `silva.commissio(opus=ID)`
> closes each.

**Goal:** house tools report what they actually read and write (a
ledger in `filum`), fabrica keys actions on those recorded reads
instead of hand-proved declarations, objects and aedilis extraction
records live in a per-tree content-addressed store, the toml runner
builds through it, the executor runs independent actions in parallel,
and excubitor retires.

**Architecture:** a recorder module (`lib/lectiones.c`) is the single
place a ledger line is written; `filum`, `via_existit`,
`iter_directoria` and aedilis's two raw directory loops call it. The
store (`lib/thesaurus.c`) is pure `filum` + `sigillum`: blobs by
content digest, action entries by key, all on disk (no sqlite), so any
house tool can use it. Compilation through the store is a drop-in tool
(`bin/compilator`, same arguments as `clang -c`), so runners change one
word per compile line. fabrica's core stays pure: traces arrive through
new seam members; parallelism is a scheduler over the async
`processus` path in `tools/fabrica.c`, restricted to actions whose
writes are known.

**Tech stack:** C89 in Latin (`latina.h`), `filum.h`, `sigillum.h`,
`processus.h`, `stml.h`; sqlite via `scrinium` (tool only, for
fabrica's own records); credo gates; bash for runners and gates.

**Spec:** `project-specs/fabrica-spec-2.md` (§I framing, §II ledger,
§III store, §IV kinds, §V pilot, §VI parallel, §VII excubitor, §VIII
order, §IX done, §X review focus, §XI AUDIENDA, §XII codebase pass).

## Global constraints

- C89 under `tools/vexilla.sh` flags. Latin identifiers; latina macros
  are forbidden identifiers (`nomen`, `casus`, `per`, `duplex`, …);
  single capitals and every Roman numeral 0–3999 are macros. `chorda`
  is not NUL-terminated; `i32`/`i64` are UNSIGNED (`s32`/`s64` for
  signed values). Names use `titulus`, never `nomen`.
- Lines ≤ 72 columns; `./silva/formator.sh -vitia` per touched file
  shows no NEW divergence (commissio now formats before judging).
- No `<word` tag openings in C comments or string literals.
- New identifier words: `git add -N` new files, then
  `./oratio/vocabula.sh -nova` must say NOVA 0 before committing.
- Commit only through `silva.commissio(msg, viae, portae)` with
  explicit paths; never Fran's files. Long commits are launched
  DETACHED (`nohup sh -c '…; echo rc=$? >> out' &`) and polled.
- `lib/*.c` must not reference sqlite or any symbol outside `lib/`.
- Digests: SHA-256 via `sigillum.h`, 64 hex, never truncated; no
  timestamp inside a digest.
- **The ledger is OFF unless its environment variable names a file**;
  off = zero behaviour change and no allocation.
- **A failed or crashed run never leaves a trusted ledger or a cached
  result.**
- **Generators and stores never rewrite unchanged output** (write to a
  temp name, compare, rename only if different; a7a7d6f2 lesson).
- **Parallel only for actions whose writes are known** (spec §XII.3);
  everything else stays serial with the tree-snapshot check.
- Every new gate is born red; every fix is proven by a PLANTED FAULT
  that compiles (`clang -fsyntax-only` first), red predicted before the
  run, removed after.
- Names marked PLACEHOLDER are settled with Fran at the start of the
  task that introduces them (recommendations in §AUDIENDA below).

## Review Focus

1. **A read that bypasses the recorder** (a raw `fopen`/`stat`/`opendir`
   left on the pilot path) → the trace misses an input and a stale
   result is served as RECENS. The pilot-path lint must be clean before
   any trace is trusted. Test: T2 Step 5 (lint plant: a raw `fopen`
   added to `lib/via.c` is named, pilot path blocked).
2. **A new header shadowing an old one on the `-I` path** → clang's
   depfile cannot see it; the include-root listing digest must change
   the key. Test: T4 Step 6 (plant: `toml/fontes/chorda.h` created →
   miss, object rebuilt; removed → miss again).
3. **Two writers of one blob, action entry or ledger at once** → torn
   blob or interleaved ledger lines. Test: T3 Step 5 (two processes
   `thesaurus_ponere` the same bytes concurrently, 200 rounds, every
   blob verifies) and T1 Step 3 (two children appending, every line
   whole).
4. **Compiler upgrade** → every object key must change. Test: T4 Step 7
   (plant: `FABRICA_CLANG` pointing at a wrapper script with different
   bytes → all misses).
5. **A memo hit that is wrong** (an input nobody declared and the
   ledger did not see) → the memo audit catches it loudly. Test: T2
   Step 8 (plant: a trace with one entry deleted by hand → audit names
   the action, `cursus` row eventus AUDITUM_DISCORS).

## AUDIENDA — recommendations for Fran (settle at plan review)

- **A1 names.** Ledger variable `FABRICA_LECTIONES`; recorder module
  `lectiones` (`lib/lectiones.c`); store module `thesaurus`
  (`lib/thesaurus.c`, directory `build/aedilis/obiecta/`); compile tool
  `bin/compilator`; kinds `compilatio`, `nexus`, `familia`; GC verb
  `bin/fabrica purgare`; ledger event letters `L A X D S E`.
- **A2 rates.** Memo audit: 1 hit in 20 re-run (and every hit under
  `iudicare -audit`); blob verify: 1 read in 16 (and every read under
  `-plenus`). Both as constants in one place, measured after T5.
- **A3 GC depth.** Keep blobs referenced by the last 5 generations.
- **A4 environment lines.** Pilot path only in this slice (the `getenv`
  wrapper exists; migration of other tools is the later batch job).
- **A5 working directory.** Put the working directory in the object key
  now (no flag change, debug paths unchanged); revisit
  `-fdebug-prefix-map` when a shared store is proposed.

## File structure

| file | responsibility |
|---|---|
| `include/lectiones.h`, `lib/lectiones.c` (NEW) | T1 recorder: one function per event, lazy open from the env var, whole-line appends |
| `lib/filum.c`, `lib/via.c`, `lib/iter_directoria.c`, `tools/aedilis.c` | T1 read/exists/list hooks; T2 write hooks |
| `probationes/probatio_lectiones.c` (NEW) | T1–T2 recorder tests |
| `tools/lectiones_spica.sh` (NEW) | T1 spike: aedilis ledger vs manifest |
| `tools/lectiones_lint.sh` (NEW) | T2 raw-IO lint (warnings; pilot path blocking) |
| `include/fabrica.h`, `lib/fabrica.c`, `probationes/probatio_fabrica.c` | T2 traces in the seam + judge; memo audit; T4 `familia`; T6 waves |
| `tools/fabrica.c` | T2 migration III (`lectiones` table), per-action ledger file; T6 parallel scheduler; T3/T7 `purgare` |
| `include/thesaurus.h`, `lib/thesaurus.c` (NEW), `probationes/probatio_thesaurus.c` (NEW) | T3 CAS + action entries + generations + GC |
| `tools/compilator.c` (NEW), `tools/compilator_struere.sh` (NEW) | T4 drop-in `clang -c` through the store |
| `aedificatio.canon`, `toml/aedificatio.stml` (NEW) | T4 `familia`; T5 toml test binaries |
| `toml/compile_probationes.sh` | T5 thin runner |
| `tools/toml_oraculum.sh` (NEW) | T5 oracle: old runner vs new, objects + output |
| `tools/aedilis.c` | T3 extraction records in the store (`--thesaurus`) |
| `excubitor.sh`, `fabrica.tsv`, `compile_tests.sh:1225` | T7 oracle, then deletion |
| `lib/fabrica.worklog.md`, `fabrica-spec-2.md` "As built", MEMORY, ledger | per task; T8 closes |

---

### Task T1: the ledger spike (reads, lookups, listings)

**Opens with names (Fran):** A1 ledger variable, module and event
letters.

**Opens with a measurement whose answer decides the slice (spec
§II.4):** after Step 6, the ledger of `aedilis <root>` is compared with
the manifest it writes. Branch PASS: ledger ⊇ manifest and every extra
read is named and justified in the worklog → continue to T2. Branch
GAP: a manifest entry missing from the ledger, or an extra read nobody
can explain → stop, show Fran the gap, do not start T2.

**Files:** `include/lectiones.h`, `lib/lectiones.c` (NEW);
`lib/filum.c` (`filum_legere_totum` :71, `filum_existit` :142,
`filum_mensura` :157, `filum_lector_aperire` :188, `filum_status`
:1208, `filum_directorium_existit` :1190); `lib/via.c:481`
(`via_existit`); `lib/iter_directoria.c` (:85 open, :176 :335 stat);
`tools/aedilis.c:928,1012` (raw `opendir` loops);
`probationes/probatio_lectiones.c`, `tools/lectiones_spica.sh` (NEW).

**Interfaces:**
- Produces (`include/lectiones.h`):

```c
/* lectiones.h - liber lectionum: quod processus legit, quaesivit,
 * enumeravit, scripsit (fabrica slice 2). Variabilis ambitus
 * FABRICA_LECTIONES viam plagulae nominat; absens = nihil fit. */
#ifndef LECTIONES_H
#define LECTIONES_H
#include "latina.h"

nomen enumeratio {
    LECTIO_LEGIT = 0,     /* 'L' plagula lecta */
    LECTIO_ABSENS,        /* 'A' quaesita, non inventa */
    LECTIO_EXSTAT,        /* 'X' exstare confirmata, non lecta */
    LECTIO_ENUMERAVIT,    /* 'D' directorium enumeratum */
    LECTIO_SCRIPSIT,      /* 'S' plagula scripta/deleta/mota (T2) */
    LECTIO_AMBITUS        /* 'E' variabilis ambitus lecta (T2) */
} LectioGenus;

/* lineam unam appendere: "<littera>\t<via>\n" (E: "E\t<titulus>
 * \t<valor>\n"). Una vocatio write() per lineam (O_APPEND) - filii
 * paralleli lineas integras servant. Nihil si variabilis absens. */
vacuum lectiones_notare (LectioGenus genus, constans character* via);
#endif
```

- Consumes: nothing new.

- [ ] **Step 1: Write the failing test** (`probationes/probatio_lectiones.c`)

```c
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "credo.h"
#include "chorda.h"
#include "filum.h"
#include "via.h"
#include "lectiones.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

s32 principale (vacuum)
{
    Piscina* piscina;
    chorda   liber;
    b32      praeteritus;

    piscina = piscina_generare_dynamicum("probatio_lectiones", 1048576);
    credo_aperire(piscina);

    /* I. absens variabilis: nihil scribitur */
    unsetenv("FABRICA_LECTIONES");
    (vacuum)remove("build/probatio_lectiones.tsv");
    (vacuum)filum_legere_totum("probationes/probatio_lectiones.c",
        piscina);
    CREDO_FALSUM(filum_existit("build/probatio_lectiones.tsv"));

    /* II. praesens: L, A, X, D */
    setenv("FABRICA_LECTIONES", "build/probatio_lectiones.tsv", I);
    (vacuum)filum_legere_totum("probationes/probatio_lectiones.c",
        piscina);
    (vacuum)via_existit(chorda_ex_literis("include/nusquam.h",
        piscina));
    (vacuum)via_existit(chorda_ex_literis("include/latina.h",
        piscina));
    unsetenv("FABRICA_LECTIONES");
    liber = filum_legere_totum("build/probatio_lectiones.tsv", piscina);
    CREDO_VERUM(chorda_continet(liber, chorda_ex_literis(
        "L\tprobationes/probatio_lectiones.c\n", piscina)));
    CREDO_VERUM(chorda_continet(liber, chorda_ex_literis(
        "A\tinclude/nusquam.h\n", piscina)));
    CREDO_VERUM(chorda_continet(liber, chorda_ex_literis(
        "X\tinclude/latina.h\n", piscina)));

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
```

(`chorda_continet(fenum, acus)` is the house substring test,
`include/chorda.h:124`.)

- [ ] **Step 2: Run it red.** `./compile_tests.sh lectiones` → fails to
  compile (`lectiones.h` missing).

- [ ] **Step 3: Recorder + concurrency.** Implement `lib/lectiones.c`:
  read `getenv("FABRICA_LECTIONES")` once per process (static flag;
  re-read when the value changes, so tests can toggle it), open with
  `open(via, O_WRONLY|O_APPEND|O_CREAT, 0644)`, format the line into a
  stack buffer (path truncated lines are refused, never split) and
  write it with ONE `write()`. The recorder uses raw `open`/`write`
  by design (it is the channel; the lint exempts it). Add to the test:
  fork two children that each append 500 `L` lines; assert 1000 lines,
  each matching `^L\t[^\t\n]+$`.

- [ ] **Step 4: Hooks.** `filum_legere_totum`, `filum_lector_aperire`
  → `L` on success, `A` on ENOENT; `filum_existit`,
  `filum_directorium_existit`, `filum_mensura`, `filum_status`,
  `via_existit` → `X` if present, `A` if absent; directory opens in
  `lib/iter_directoria.c` and the two `opendir` loops in
  `tools/aedilis.c:928,1012` → `D`. Paths recorded as given (relative
  stays relative).

- [ ] **Step 5: Run green.** `./compile_tests.sh lectiones` and
  `./compile_tests.sh filum` and `./compile_tests.sh via` pass;
  `./tools/aedilis_struere.sh` builds.

- [ ] **Step 6: The spike.** `tools/lectiones_spica.sh <root.c>`:
  run `FABRICA_LECTIONES=$T/l.tsv ./bin/aedilis <root> --enumerare
  --manifestum $T/m.stml`, extract manifest paths (`via="…"`), and
  print three lists: in manifest but not in ledger (`L` or `X`) — must
  be EMPTY; in ledger but not in manifest — each line followed by its
  justification category (tool configuration, `latina.h`/system
  header, failed lookup on the include path, directory listing);
  counts. Run on `silva/fontes/silva_token.c`, `lib/xar.c`,
  `probationes/probatio_toml_tabula.c` (or the nearest toml root).

- [ ] **Step 7: Plant.** Remove the `via_existit` hook → the spike's
  "ledger but not manifest" loses every `A` line and the "manifest but
  not ledger" list grows (headers resolved via existence only) — red;
  restore.

- [ ] **Step 8: Decide (Fran).** Show the spike output; PASS or GAP per
  the branch rule above. Worklog entry with the three lists.

- [ ] **Step 9: Commit.** `silva.commissio(…, ['include/lectiones.h',
  'lib/lectiones.c', 'lib/filum.c', 'lib/via.c',
  'lib/iter_directoria.c', 'tools/aedilis.c',
  'probationes/probatio_lectiones.c', 'tools/lectiones_spica.sh',
  'lib/fabrica.worklog.md'])` — widely included headers untouched, but
  `lib/filum.c` is in most closures: launch detached.

---

### Task T2: writes, environment, lint, verifying traces, memo audit

**Files:** `lib/filum.c` (scriptor/scribere/appendere/delere/movere/
copiare/directorium_creare*: `S`), `lib/lectiones.c` + `.h`
(`lectiones_ambitus`), `tools/lectiones_lint.sh` (NEW),
`include/fabrica.h`, `lib/fabrica.c`, `tools/fabrica.c` (migration
III, per-action ledger file, `-audit`), `aedificatio.canon`
(`lectiones="verum"` on `actio`, PLACEHOLDER), one real action family
switched (see Step 7), `probationes/probatio_fabrica.c`,
`probationes/probatio_lectiones.c`, `tools/fabrica_fumus.sh`.

**Interfaces:**
- Produces (`include/lectiones.h`):

```c
/* getenv per librum: valorem reddit (aut NIHIL) et 'E' notat */
constans character* lectiones_ambitus (constans character* titulus);
```

- Produces (`include/fabrica.h`): a trace entry and two seam members.

```c
nomen structura {
    LectioGenus  genus;
         chorda  via;
       Sigillum  sigillum;   /* L: contenta; A/X: praesentia; D: nomina */
} FabricaLectio;

/* in FabricaSutura: */
b32 (*lectiones_legere)(vacuum* datum, constans character* actio,
                        Piscina* piscina, Xar** lectiones_out);
vacuum (*lectiones_scribere)(vacuum* datum, constans character* actio,
                             constans Xar* lectiones);
```

- Produces (judge rule, `lib/fabrica.c`): an action with
  `lectiones="verum"` and a stored trace is RECENS iff every entry
  re-digests equal (L: bytes; A: still absent; X: still present; D:
  same sorted name list); any difference → regenerate. Declared
  `ingressus` of such an action are STILL digested (they cover what
  the ledger cannot see: bash scripts, argv) — the trace replaces the
  `manifesta` / `radices` inputs, not the scripts.

- [ ] **Step 1: Failing core tests** (`probatio_fabrica.c`, in-memory
  disk): (a) trace with one `L` entry, bytes unchanged → RECENS without
  `currere`; (b) bytes changed → regenerated; (c) `A` entry, file now
  exists → regenerated (shadowing); (d) `D` entry, a new name in the
  directory → regenerated; (e) no trace → IGNOTUM-then-regenerate as
  today; (f) failed run → no trace stored.

- [ ] **Step 2: Run red.** `./compile_tests.sh fabrica`.

- [ ] **Step 3: Implement the judge rule** in `lib/fabrica.c` behind
  the two seam members; the in-memory disk implements them with a Xar.

- [ ] **Step 4: Tool side.** Migration III in `tools/fabrica.c`:
  `CREATE TABLE lectiones (titulus TEXT NOT NULL, genus TEXT NOT NULL,
  via TEXT NOT NULL, sigillum TEXT NOT NULL)`; the executor and the
  judge's regeneration set `FABRICA_LECTIONES` to
  `build/fabrica/lectiones/<titulus>.tsv` immediately before starting
  an action with `lectiones="verum"`, parse it after a successful run,
  digest each entry, store; delete the file either way.

- [ ] **Step 5: Lint.** `tools/lectiones_lint.sh`: `./silva/selecta.sh`
  (or `nexus.sh` usage rows) for calls to `fopen`, `opendir`, `stat`,
  `lstat`, `access`, `getenv` outside `lib/filum.c`,
  `lib/lectiones.c`; print WARNINGS for all; exit 1 if any hit is in a
  PILOT-PATH file (the closure of `tools/aedilis.c` +
  `tools/compilator.c` from T4, listed by `bin/aedilis --enumerare`).
  Plant: add `fopen("x","r")` to `lib/via.c` → named, exit 1;
  restore.

- [ ] **Step 6: Writes and environment.** `S` from every `filum`
  write/delete/move/mkdir; `lectiones_ambitus` used for every `getenv`
  on the pilot path (none today on aedilis's path, spec §XII.1).
  Recorder tests: write → `S` line; `lectiones_ambitus("HOME")` → `E`
  line with value.

- [ ] **Step 7: First real family.** Switch the `fragmentum_*` snippet
  actions (they run aedilis per root) to `lectiones="verum"`; remove
  their `manifesta` and `radices` inputs (keep `fasciculus` script
  inputs and `configuratio`). Plants (fumus stage, PLACEHOLDER number):
  (i) a comment appended to a header in a snippet's closure → that
  snippet regenerates, others hit; (ii) a new header created earlier on
  an include root with a name the closure includes → miss (`A`→present);
  (iii) a new `.c` in a listed directory → miss (`D`).

- [ ] **Step 8: Memo audit.** `iudicare -audit` re-runs every memo hit;
  without it, 1 in 20 (A2, deterministic: hash of titulus + run counter)
  re-runs. A disagreement prints `AUDITUM DISCORS <titulus>` and writes
  a `cursus` row eventus `AUDITUM_DISCORS`; exit 1. Plant: delete one
  line of a stored trace in `build/fabrica.db` by hand, edit the file
  that line named → normal judge serves RECENS (the hole), `-audit`
  catches it.

- [ ] **Step 9: Run green + measure** (`-plenus` before/after; snippet
  family misses after a header edit, before/after) and commit
  (detached; owes fabrica, fabrica-fumus, radix).

---

### Task T3: the store (`thesaurus`) and aedilis records

**Opens with names (Fran):** A1 store names, A3 GC depth.

**Files:** `include/thesaurus.h`, `lib/thesaurus.c`,
`probationes/probatio_thesaurus.c` (NEW); `tools/aedilis.c`
(`--thesaurus <dir>`); `tools/fabrica.c` (`purgare`).

**Interfaces:**
- Produces (`include/thesaurus.h`):

```c
nomen structura Thesaurus Thesaurus;

/* radix: directorium (creatur si absens). */
Thesaurus* thesaurus_aperire (constans character* radix,
                              Piscina* piscina);

/* blob: octeti -> sigillum (SHA-256 contentorum). Scriptura atomica
 * (nomen temporarium + rename); blob iam praesens non rescribitur. */
b32 thesaurus_ponere (Thesaurus* t, chorda octeti, Sigillum* out);

/* via plagulae blobi (legenda/nectenda); FALSUM si absens. Lectio
 * per specimen verificatur (A2); verificatio fracta -> blob deletur,
 * FALSUM. */
b32 thesaurus_via (Thesaurus* t, constans Sigillum* sigillum,
                   Piscina* piscina, chorda* via_out);

/* actio: clavis -> sigilla exituum (plura, ordine). */
b32 thesaurus_actio_ponere (Thesaurus* t, constans Sigillum* clavis,
                            constans Xar* sigilla_exituum);
b32 thesaurus_actio_capere (Thesaurus* t, constans Sigillum* clavis,
                            Piscina* piscina, Xar** sigilla_out);

/* generatio: claves hoc cursu adhibitae (ad purgationem) */
vacuum thesaurus_generationem_notare (Thesaurus* t,
                                      constans Sigillum* clavis);
b32 thesaurus_purgare (Thesaurus* t, i32 generationes_servandae,
                       b32 verificare, i32* deleta_out);
```

- Layout under `build/aedilis/obiecta/`: `blobi/<2 hex>/<62 hex>`,
  `actiones/<2 hex>/<62 hex>` (text: one output digest per line),
  `generationes/<YYYYmmddTHHMMSS>-<pid>.lst` (keys used by one run).

- [ ] **Step 1: Failing tests** (`probatio_thesaurus.c`): put/get round
  trip; same bytes twice → one blob, second put does not rewrite (mtime
  unchanged); action entry round trip with 2 outputs; absent key →
  FALSUM; corrupted blob (byte flipped) under forced verification →
  FALSUM and blob gone; purge keeps keys of the last N generation lists
  and their blobs, deletes the rest.

- [ ] **Step 2: Run red.**
- [ ] **Step 3: Implement** with `filum` + `sigillum` only.
- [ ] **Step 4: Run green.**
- [ ] **Step 5: Concurrency plant** (Review Focus 3): fork two children
  that `thesaurus_ponere` the same 64 KiB buffer 200 times each; every
  `thesaurus_via` afterwards verifies. Plant: write the blob in place
  instead of temp+rename → torn reads appear (or the test is shown
  unable to detect it, in which case strengthen it until it does).
- [ ] **Step 6: aedilis records.** `aedilis --thesaurus <dir>`: the
  extraction memo (`_extractor_memor`) looks up key = SHA-256(file
  bytes ‖ configuration digest ‖ extractor version) before parsing;
  stores the serialized record (directives, annotations, angle
  includes) on miss. Equivalence: `--partes` and `--differentia`
  outputs for all 199 test roots identical with and without
  `--thesaurus` (cold and warm store). Measure: snippet generators per
  miss, and the aedilis gate (`tools/aedilis_porta.sh` passes
  `--thesaurus` once this step lands), before/after - these are two of
  the speedups spec §IX requires.
- [ ] **Step 7: `bin/fabrica purgare [-verificare]`** wrapping
  `thesaurus_purgare` (A3 generations), and a fumus stage.
- [ ] **Step 8: Commit** (detached).

---

### Task T4: `compilatio` through the store, `nexus`, `familia`

**Opens with names (Fran):** `compilator`, `familia` attributes
(PLACEHOLDER: `via`, `praefixum`, `suffixum`).

**Files:** `tools/compilator.c`, `tools/compilator_struere.sh` (NEW);
`aedificatio.canon` (`familia`); `lib/fabrica.c` (familia expansion in
the declaration reader); `probationes/probatio_fabrica.c`;
`tools/fabrica_fumus.sh`.

**Interfaces:**
- `bin/compilator [clang -c arguments] -o <obj> <fons>`: same arguments
  as `clang -c`. Head key = SHA-256(fons path ‖ fons bytes ‖ argument
  list ‖ clang binary digest ‖ working directory (A5) ‖ include-root
  listing digest). Action entry under the head key stores the last
  depfile header list; full key = head key ‖ each header (path ‖
  bytes). Hit → place the object at `-o` (hard link if same device,
  else copy; never rewrite an identical target). Miss → `clang -c …
  -MD -MF <tmp>`, store object blob + action entries + depfile list,
  place at `-o`. Exit code and stderr of clang passed through; a failed
  compile stores nothing.
- `FABRICA_CLANG` (env) overrides the clang path (tests, plants).
- `familia` element (in `aedificatio.canon`): expands at load time to
  one `actio` per matching file, title `<familia>:<basis>`, with the
  family's template children; data only (no expressions).

- [ ] **Step 1: Failing tests** (fumus stage, PLACEHOLDER): cold compile
  of `lib/chorda.c` → object byte-identical to plain `clang -c` with the
  same arguments in the same directory; second call → hit (no clang:
  `FABRICA_CLANG` points at a counting wrapper, count unchanged);
  touched-only source (same bytes) → hit; edited header in the depfile
  → miss.
- [ ] **Step 2: Run red.**
- [ ] **Step 3: Implement `tools/compilator.c`** (`processus_exsequi`
  for clang; `thesaurus`; `sigillum`; `filum`), build script.
- [ ] **Step 4: Run green.**
- [ ] **Step 5: `familia`** core tests (in-memory disk: a directory with
  3 matching files + 1 non-matching → 3 actions with the expected
  titles; a new file → 4) and implementation.
- [ ] **Step 6: Shadowing plant** (Review Focus 2): create
  `toml/fontes/chorda.h` (empty) while `toml/fontes` is an include root
  ahead of `include/` for a toml source → miss and the object changes;
  remove it → miss again, object back to the original bytes.
- [ ] **Step 7: Compiler plant** (Review Focus 4): `FABRICA_CLANG` →
  a wrapper script calling real clang (different bytes) → every
  object misses once, then hits.
- [ ] **Step 8: Commit** (detached).

---

### Task T5: the toml pilot and its oracle

**Files:** `toml/compile_probationes.sh`, `toml/aedificatio.stml`
(NEW: `familia` of test binaries as `nexus` actions), `fabrica.stml`
(add `<subsystema via="toml"/>`), `tools/toml_oraculum.sh` (NEW).

- [ ] **Step 1: Oracle first, born red.** `tools/toml_oraculum.sh`:
  in a scratch worktree from HEAD (`git worktree add`), run the OLD
  runner, collect `build/toml/*.o` and the per-test output lines; in
  the live tree run the NEW runner, collect the same; compare objects
  byte for byte (same tree path, spec §XII.2) and outputs line for
  line; exit 1 on any difference. Before Step 2 it must fail (the new
  runner does not exist yet → "nihil comparabile").
- [ ] **Step 2: Thin runner.** Object lists come from
  `bin/aedilis <test> --enumerare` (closures) instead of the hand lists
  (:133-186); every `clang … -c` becomes `bin/compilator …`; linking
  stays `clang` (binaries are judged by relatio); the lock, logging and
  reporting are unchanged. The coarse "newest header" block is deleted.
- [ ] **Step 3: Declarations.** `toml/aedificatio.stml`: a `familia`
  over `toml/probationes`, `probatio_`, `.c`, each instance a binary
  `build/toml/<basis>` with `provenientia="relatio"` (PLACEHOLDER: if
  the runner keeps linking, these are declared for judging only).
- [ ] **Step 4: Oracle green** on HEAD; then on three real commits
  (tree states) chosen to touch toml, materia and include/.
- [ ] **Step 5: Measure:** cold and warm toml suite wall time,
  before/after (`tempora.tsv` row via the runner, or `/usr/bin/time`).
- [ ] **Step 6: Commit** (detached; owes toml, fabrica).

---

### Task T6: parallel execution

**Files:** `tools/fabrica.c` (scheduler over `processus_incipere` /
`processus_pulsare` / `processus_metere`), `lib/fabrica.c` (which
actions are parallel-safe: `lectiones="verum"` with `S` trace, or a
`nexus`/`compilatio` instance whose single output fabrica names),
`probationes/probatio_fabrica.c`, `tools/fabrica_fumus.sh`.

- [ ] **Step 1: Failing core test:** three independent parallel-safe
  actions + one unsafe; the plan the core returns runs the three in one
  wave and the unsafe one alone; dependents after their inputs.
- [ ] **Step 2: Run red; implement** wave selection in the core.
- [ ] **Step 3: Scheduler** in the tool: up to `FABRICA_FILA` (default
  `sysctl -n hw.perflevel0.physicalcpu`) at once; each child gets its
  own `FABRICA_LECTIONES` file (set immediately before
  `processus_incipere`); unsafe actions run alone with the snapshot
  check; on a failure, running children finish, nothing new starts,
  dependents OMISSUM, the failure is reported first; `cursus` rows
  written in a deterministic order (by action title).
- [ ] **Step 4: Plants:** (i) an action that sleeps 2 s, ×4 → wall < 4 s
  with `FABRICA_FILA=4`; (ii) one of four fails → the other three are
  SANATUM, its dependents OMISSUM, exit 1; (iii) an unsafe action is
  never in a wave with another (scheduler log).
- [ ] **Step 5: Measure** `sanare` of the toml family cold, serial vs
  parallel; commit (detached).

---

### Task T7: excubitor — oracle, then deletion

- [ ] **Step 1:** For the toml objects, compare `./excubitor.sh -tacitus`
  verdicts with the store's (an object excubitor calls stale must be a
  miss in the compilator; record disagreements in the worklog) across
  the T5 Step 4 commits.
- [ ] **Step 2 (Fran):** decide deletion on that record.
- [ ] **Step 3:** delete `excubitor.sh`, `fabrica.tsv`, the call at
  `compile_tests.sh:1225`; commit (detached; owes radix).

---

### Task T8: closing

- [ ] Spec 2 "As built" section; worklog; MEMORY (`fabrica-project.md`,
  index line); ledger: close the slice-2 opera, file what was deferred.
- [ ] Record the "done means" evidence (spec §IX): oracle runs, the
  family keyed on traces, measured speedups, excubitor gone.

## Not in plan 2 (stated)

Program kinds as capability bundles; jobs returning effect plans;
pulled inputs inside a job; the full `filum` migration beyond the
pilot path (later tabularium batch job); tests as fabrica actions
(slice 3); a store shared across worktrees (keys are kept
tree-relative so it stays possible); per-consumer token-level input
views and change classification in stale reports (Fran's later ideas,
which the CAS's old versions enable).
