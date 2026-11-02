# OFFICINA BIBLIOTHECAE Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** The interpreter loads the libraries interpreted code needs
from that code's own `#include` lines (closure by aedilis), in the
REPL, in #! scripts and in interpretare; and colloquium builds again.

**Architecture:** One new module, `officina/instrumenta/bibliothecae.{h,c}`,
wraps aedilis: it writes the include lines to a throwaway scope file,
derives twice (variant `posix` = what loads; configured preference =
detection of mac-only headers), applies the deny-list, and returns
radix-relative `.c` paths. sessio calls it on every directive turn
and loads new paths into its library pool; interpretare calls it for
the target function's home.

**Tech Stack:** C89 + latina.h, aedilis (lib/aedilis.c,
lib/aedilis_silva.c), credo, officina's bash launchers.

**Spec:** project-specs/officina-bibliothecae-spec.md (approved by
Fran 2026-11-02).

## Global Constraints

- House rules: Latin identifiers (lint `./oratio/vocabula.sh -nova`),
  latina.h macros never identifiers (`nomen`, `per`, numerals...),
  i32 UNSIGNED (s32 for signed), chorda not NUL-terminated, 72-byte
  lines, full warning flags (tools/vexilla.sh).
- Each task's HEADER TEXT goes to Fran for approval before code.
  One task per turn, inline.
- TDD: assertions red first; after green, a COMPILING plant per new
  assertion via `silva.planta(...)` (back up, `cmp` after = IDEM).
- File writes in officina code go through `filum_*` (examen 96).
- aedilis is not modified (spec V).
- Deny-list size limit: 4194304 octets (= interpretare.c:45
  `mensura_maxima`).
- Variant for loading: `"posix"`; detection: NIHIL (configured
  `macos posix`, aedilis.stml:62).
- `-plagulae` keeps working everywhere (override/addition).
- Commit only on Fran's word, via the detached commissio script.

## Review Focus

1. A typo'd header (`#include "chorda_x.h"`) -> named refusal
   naming the header, never a crash or silent accept. Test: Task 2.
2. `#include <stdio.h>` (system) -> accepted, nothing loaded (empty
   list, not NIHIL). Test: Task 2.
3. Two includes with overlapping closures in one session (chorda.h,
   then filum.h) -> each library loaded once, later calls still
   link. Test: Task 3.
4. A header-only include (`#include "latina.h"`) -> accepted, no
   module added. Test: Task 3.
5. A refused call leaves no scope file behind (refusals take an
   early-return path). Test: Task 2.

---

### Task 1: colloquium builds again, and a gate runs it

**Files:**
- Modify: `officina/colloquium.sh` (compile + link
  `silva/instrumenta/nexus_ordines.c`, as compile_probationes.sh
  step 2c-0 does, with the newest-header guard)
- Create: `officina/probationes/probatio_officina_lanceatores.c`

**Interfaces:**
- Produces: suite `probatio_officina_lanceatores`, sections
  extended by Tasks 3 and 4.

- [ ] **Step 1: Write the failing test.** Section `"colloquium
  scriptum"`: run `officina/colloquium.sh
  officina/probationes/fixa/sessio_scripta/salutatio.scriptum -posix
  -plagulae lib/piscina.c,lib/chorda.c,lib/chorda_aedificator.c,lib/lectiones.c,lib/filum.c`
  (paths radix-relative: colloquium.c `_plagulas_findere` passes
  them to sessio, which joins radix/via) from `RHUBARB_RADIX`
  through processus_posix (already linked), capture stdout.
  `CREDO_AEQUALIS_I32(exitus, 0)`;
  stdout contains `"ave ex scripto (14 octeti)"` and
  `"relectum: scriptum scripsit"`.
- [ ] **Step 2:** `./officina/compile_probationes.sh lanceatores` ->
  FRACTA (link error `_nexus_ordines_capitis_gradus`, exit != 0).
- [ ] **Step 3:** Add the nexus_ordines object to colloquium.sh.
- [ ] **Step 4:** Suite green; whole `./officina/compile_probationes.sh`
  green.
- [ ] **Step 5: Plant:** remove nexus_ordines from the link line ->
  suite FRACTA; restore, `cmp` IDEM.
- [ ] **Step 6:** Ledger: parcum 01M4XR3Q -> clausum (note: gate
  now runs the launcher). Commit on Fran's word.

### Task 2: the resolver module

**Files:**
- Create: `officina/instrumenta/bibliothecae.h`,
  `officina/instrumenta/bibliothecae.c`
- Create: `officina/probationes/probatio_officina_bibliothecae.c`
- Create: `officina/probationes/fixa/bibliothecae/nexus.tsv`
- Modify: `officina/compile_probationes.sh` (RADIX_FONTES += the
  module's lib closure - take it from
  `bin/aedilis officina/instrumenta/bibliothecae.c --enumerare`:
  expect aedilis, aedilis_silva, stml, via, thesaurus, selectio,
  iter_directoria...; and an object step for bibliothecae.c beside
  sessio/praeparator)

**Interfaces:**
- Produces (header text to Fran first):

```c
nomen structura Bibliothecae Bibliothecae;

/* radix = radix repositorii (aedilis.stml ibi legitur). NIHIL +
 * causa si configuratio aut extractor parari nequit. */
Bibliothecae*
bibliothecae_creare (
               Piscina* piscina,
    constans character* radix,
                chorda* causa_out);

/* textus = C cuius lineae #include bibliothecas declarant (turnus,
 * directivae scripti, "#include \"x.h\"\n" synthetica). Reddit Xar
 * de chorda: viae radici relativae ("lib/chorda.c"), sine
 * duplicatis; vacuum si nihil onerandum. NIHIL + causa si
 * recusatum. */
Xar*
bibliothecae_ex_textu (
    Bibliothecae* b,
           chorda textus,
          Piscina* piscina,
           chorda* causa_out);

/* via = fons .c radici relativus; fructus ipsam viam PRIMAM
 * continet, deinde clausuram eius. */
Xar*
bibliothecae_ex_via (
          Bibliothecae* b,
    constans character* via,
               Piscina* piscina,
                chorda* causa_out);

#define BIBLIOTHECAE_MENSURA_MAXIMA 4194304

nomen enumeratio {
    BIBLIOTHECAE_SEDES_INVENTA = 0,
    BIBLIOTHECAE_SEDES_IGNOTA,     /* index absens aut functio
                                    * non in eo */
    BIBLIOTHECAE_SEDES_AMBIGUA
} BibliothecaeSedes;

/* Domus functionis ex indice symbolorum (build/nexus.tsv forma:
 * titulus \t sedes \t functio \t via ...). Solae sedes sub include/
 * et lib/; include/x.h et lib/x.c = familia una (caput praefertur
 * in via_out). AMBIGUA: causa candidatos nominat. */
BibliothecaeSedes
bibliothecae_sedem_quaerere (
    constans character* via_indicis,
    constans character* functio,
               Piscina* piscina,
                chorda* via_out,
                chorda* causa_out);
```

- Algorithm (not determined by the signatures):
  1. Write `textus` to a scope file
     `build/bibliothecae_scopi/scopus_<pid>_<n>.c` under radix (n =
     per-process counter: AedilisSilva caches per path, so a path is
     never reused). Delete it on EVERY return path.
  2. Derive with variant `"posix"` -> load set. Derive with NIHIL ->
     detection set. (The scope's own includes are walked; a header
     scope would miss its own implementation - measured 2026-11-02:
     `bin/aedilis include/filum.h` omits filum.c and lectiones.c.)
  3. Refuse (whole call, NIHIL) in this order, causa naming file and
     the header that pulled it in (`AedilisObiectum.caput`):
     - each `.m` in the detection set whose caput has no object in
       the load set: `"<caput> solum implementationem macOS habet
       (<via.m>) - machinula Objective-C non currit"`;
     - any entry of `inresolutae`: `"caput non inventum: <nomen>"`;
     - any vendor source, any `absens` object;
     - any object over BIBLIOTHECAE_MENSURA_MAXIMA: `"<via> nimis
       magna (<n> octeti > 4194304), inclusa per <caput>"`.
  4. Return load-set `via`s in derivation order, deduplicated.

- [ ] **Step 1: Write the failing tests** (sections; `b` created
  once with radix = `RHUBARB_RADIX`):
  - `"filum"`: ex_textu(`#include "filum.h"\n`) -> 5 entries, set
    = {lib/filum.c, lib/lectiones.c, lib/chorda.c, lib/piscina.c,
    lib/chorda_aedificator.c}.
  - `"via"`: ex_via("lib/filum.c") -> entry 0 == "lib/filum.c",
    contains lib/lectiones.c, 5 entries.
  - `"corpus"`: ex_textu(`#include "calendarium_liturgicum.h"\n`)
    contains lib/sanctorale.c.
  - `"variant posix"`: ex_textu(`#include "fenestra_tempus.h"\n`)
    (both lib/fenestra_tempus_macos.c and _posix.c exist; measured:
    default picks macos, posix picks posix) -> contains
    `lib/fenestra_tempus_posix.c`, not `_macos.c`; non-NIHIL (a
    `_macos.c` in the detection set is NOT a refusal - only `.m`).
  - `"macOS recusatur"`: ex_textu(`#include "fenestra.h"\n`) ->
    NIHIL; causa contains `"lib/fenestra_macos.m"` and
    `"include/fenestra.h"`.
  - `"magnitudo recusatur"`: ex_textu(`#include "silva.h"\n`) ->
    NIHIL; causa contains `"silva/amalgama/silva.c"` and
    `"4194304"`.
  - `"caput ignotum"` (RF1): `#include "chorda_x.h"\n` -> NIHIL;
    causa contains `"chorda_x.h"`.
  - `"systema"` (RF2): `#include <stdio.h>\n` -> non-NIHIL, 0
    entries.
  - `"nulla vestigia"` (RF5): after all the above,
    `build/bibliothecae_scopi/` holds no file.
  - `"sedes"`: against `fixa/bibliothecae/nexus.tsv` (fixture rows:
    a function with `sedes` in include/a.h AND lib/a.c -> INVENTA,
    via "include/a.h"; one only in lib/b.c -> INVENTA "lib/b.c"; one
    in include/c.h and include/d.h -> AMBIGUA, causa contains both;
    one only under knotapel/ -> IGNOTA; absent name -> IGNOTA;
    missing tsv path -> IGNOTA).
- [ ] **Step 2:** `./officina/compile_probationes.sh bibliothecae` ->
  FRACTA (compile: header absent).
- [ ] **Step 3:** Implement per the algorithm.
- [ ] **Step 4:** Suite green; whole officina suite green.
- [ ] **Step 5: Plants** (each -> named section FRACTA, restore IDEM):
  detection derivation skipped; scope deletion skipped on refusal;
  variant NIHIL for the load set; size check removed; family merge
  removed (fasti-like row -> AMBIGUA).
- [ ] **Step 6:** Worklog `officina/instrumenta/bibliothecae.worklog.md`
  (the header-scope finding, the per-path cache). Commit on Fran's
  word.

### Task 3: sessio loads libraries from directive turns

**Files:**
- Modify: `officina/instrumenta/sessio.c` (Sessio gains
  `Bibliothecae* bibliothecae` and `TabulaDispersa* oneratae`
  [paths loaded]; `sessio_creare` creates both when `radix !=
  NIHIL`, records cfg->plagulae paths in `oneratae`; the directive
  branch of `sessio_turnum_offerre` (sessio.c ~2645) calls
  `bibliothecae_ex_textu(turn text)`, loads each path not in
  `oneratae` with the existing `_bibliothecam_demittere`, records it;
  refusal or a failed load -> `SESSIO_REIECTUM`, diagnostic nuntius
  = causa)
- Modify: `officina/instrumenta/sessio.h` (comment on
  `plagulae`: now an addition, includes load the rest)
- Modify: `officina/colloquium.sh`, `officina/compile_probationes.sh`
  (link bibliothecae + its closure; regenerate
  `officina/colloquium_fontes_generata.sh` with
  `officina/instrumenta/sessio.c officina/instrumenta/bibliothecae.c`
  added to its fontes_generare.sh inputs - the list's line 2 holds
  the command)
- Test: `officina/probationes/probatio_officina_sessio.c`,
  `officina/probationes/probatio_officina_lanceatores.c`

**Interfaces:**
- Consumes: `bibliothecae_creare`, `bibliothecae_ex_textu` (Task 2).
- Produces: no new public sessio API.

- [ ] **Step 1: Write the failing tests** (config as D2:
  `sine_capitibus FALSUM`, `cum_posix VERUM`, no plagulae):
  - `"#include initio"`: turns `#include "chorda.h"`, `Piscina* p =
    piscina_generare_dynamicum("t", 4096);`, `chorda c =
    chorda_ex_literis("ave", p);`, `printf("%.*s", (int)c.mensura,
    (constans character*)c.datum);` -> effusio `"ave"`;
    `c.mensura;` -> valor 3. All ACCEPTUM.
  - `"#include medio"`: `int x = 2;`, `x + 1;` (valor 3), then the
    same five turns, same results.
  - `"#include fenestra"`: turn `#include "fenestra.h"` ->
    REIECTUM; a diagnostic nuntius contains
    `"lib/fenestra_macos.m"`.
  - `"clausurae communes"` (RF3): `#include "chorda.h"`, then
    `#include "filum.h"` -> both ACCEPTUM; library module count
    (xar_numerus of moduli_bibliothecarum, via a test-visible count
    or the effusio of a call into filum: `filum_scribere_literis` to
    officina/build/ returns 1) -> no duplicate-symbol failure.
  - `"caput solum"` (RF4): `#include "latina.h"` -> ACCEPTUM, no
    module added.
  - D2 (`_scriptum_probare`): delete `plagulae_scripti`; set
    `plagulae_numerus = 0`. Existing assertions unchanged.
  - lanceatores `"colloquium scriptum"`: drop the `-plagulae`
    argument (keep `-posix`).
- [ ] **Step 2:** `./officina/compile_probationes.sh sessio` ->
  FRACTA (`#include initio` traps `decipula vocata`).
- [ ] **Step 3:** Implement.
- [ ] **Step 4:** sessio, lanceatores, whole officina suite green;
  `./officina/amalgamare.sh` if any fontes touched (should be none).
- [ ] **Step 5: Plants:** resolution call removed -> `#include
  initio` FRACTA; `oneratae` check removed -> `clausurae communes`
  FRACTA; refusal ignored (accept on NIHIL) -> `#include fenestra`
  FRACTA.
- [ ] **Step 6:** sessio.worklog.md note (AUDIENDA §1 answered:
  mid-session load + generation link). Commit on Fran's word.

### Task 4: interpretare derives by default

**Files:**
- Modify: `officina/instrumenta/principalia/interpretare.c` (when no
  `-plagulae`: `bibliothecae_sedem_quaerere("build/nexus.tsv",
  functio)`; INVENTA + include/x.h -> `bibliothecae_ex_textu("#include
  \"x.h\"\n")`; INVENTA + lib/x.c -> `bibliothecae_ex_via`; load
  exactly those paths (exact match, not the substring filter);
  AMBIGUA or refusal -> causa on stderr, exit 2; IGNOTA -> today's
  whole-lib path with its warning)
- Modify: `officina/interpretare.sh` (delete the `-derivare` block;
  link bibliothecae + closure; regenerate
  `officina/interpretare_fontes_generata.sh` with bibliothecae.c
  added)
- Modify: `officina/CLAUDE.md` Build/test bullet for interpretare
- Test: `officina/probationes/probatio_officina_lanceatores.c`

**Interfaces:**
- Consumes: `bibliothecae_creare`, `bibliothecae_ex_textu`,
  `bibliothecae_ex_via`, `bibliothecae_sedem_quaerere` (Task 2).

- [ ] **Step 1: Write the failing tests** (lanceatores):
  - `"interpretare sine plagulis"`: `officina/interpretare.sh
    piscina_generare_dynamicum '"x"' 1024` -> exit 0; stderr
    contains `"mundus: 1 moduli"`; stdout contains `"BENE"`.
  - `"interpretare fasti"`: `officina/interpretare.sh
    fasti_est_bissextilis 2024` -> stdout line `1`.
- [ ] **Step 2:** Run -> FRACTA (today: whole lib/, "mundus: N
  moduli" with N > 1).
- [ ] **Step 3:** Implement.
- [ ] **Step 4:** lanceatores + whole officina suite green.
- [ ] **Step 5: Plant:** sedes lookup forced to IGNOTA -> `interpretare
  sine plagulis` FRACTA.
- [ ] **Step 6:** Records: spec phases marked DONE, officina
  phase-log RELATIO with the instrument debrief (officina/CLAUDE.md
  CULTURA), ledger desideratum 01KYB9JM -> impletum, memory
  officina-project.md line. Commit on Fran's word.
