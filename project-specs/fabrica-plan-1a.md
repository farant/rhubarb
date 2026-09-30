# fabrica plan 1a — the judge: what is stale, why, and the healing plan

> **For agentic workers:** execute task by task with the executing-plans
> discipline (test first, run red, implement, run green, plant, commit),
> INLINE on main, ONE TASK PER TURN with Fran's approval, no subagents.
> Several tasks OPEN with a measurement whose answer picks between
> named branches; a result outside the named branches is shown to Fran
> before work continues. Steps use checkbox syntax. Written 2026-09-29
> from `fabrica-spec-v2.md` (2474e4c2; decisions Q33–Q37 in its §VI and
> ledger decretum 01M3QYCN6Q).

> **ORDER CHANGED 2026-09-30 (Fran, after T5):** P1, P2, T1–T5, **T7,
> then T6**, T8. Reason: T7's inputs (aedilis manifests, complete since
> P1) are sound and it fixes the motivating bug (bin/manus stale six
> weeks); T6's cache is only as good as declared inputs, and T4/T5's
> are honest but incomplete. T6 gains a per-action **cacheable** mark:
> a record may skip regeneration ONLY for actions whose inputs are
> provably complete (manifest-derived); hand-listed actions always
> regenerate under `-plenus`.

**Goal:** `bin/fabrica iudicare` names every registered artifact that is
not built from today's inputs — committed amalgams, committed generated
sources, installed binaries — with the reason and an ordered healing
plan, and builds nothing.

**Architecture:** a pure core (`lib/fabrica.c`: input kinds, input-set
digests, declarations, verdicts, plan order) reaches disk and processes
only through a seam (`FabricaSutura`, the `AedilisExtractor` pattern),
so its gate runs on in-memory fixtures. The CLI (`tools/fabrica.c`)
supplies the real seam: files, directory listings, `processus_exsequi`
for regeneration and `-provenientia`, and sqlite records. Declarations
are STML per subsystem, listed by the root `fabrica.stml` (Q33).
aedilis stays the dependency oracle; P1 makes its manifests say when
they are incomplete.

**Tech stack:** C89 in Latin (`latina.h`), `stml.h`, `sigillum.h`,
`aedilis.h`, `processus.h`, vendor sqlite (tool only), credo gates,
bash for generators and installers.

**Spec:** `project-specs/fabrica-spec-v2.md` (§0 findings, §I AUDIENDA,
§II model amendments, §III `-provenientia`, §IV task table, §VI
decisions); `fabrica-spec-v1.md` §II–§XII (model, decisions) where v2
does not amend.

## Global constraints

- C89 under `tools/vexilla.sh` flags. Latin identifiers; latina macros
  are forbidden identifiers (`nomen`, `casus`, `per`, `duplex`, …);
  single capitals and every Roman numeral 0–3999 are macros. `chorda`
  is not NUL-terminated; `i32`/`i64` are UNSIGNED (`s32` for `-I`).
  Attribute and field names use `titulus`, never `nomen`.
- Lines ≤ 72 columns. Formatter: record `./silva/formator.sh -vitia`
  per touched file at HEAD first; commit with NO new divergence.
- **No `<word` tag openings in C comments or string literals** (the
  examen scanner evokes them): write "sectio inresolutarum", never the
  element in angle brackets, inside C.
- New identifier words: `git add -N` new files, then
  `./oratio/vocabula.sh -nova` must say NOVA 0 before committing.
- Commit only through `silva.commissio(msg, viae, portae)` with explicit
  paths; never Fran's files (`FAQ.md`, `gesta/annales/*`,
  `silva/grammatica/c89-formatted.stml`). commissio adds owed gates.
  Never build while a `commissio_umbra` is snapshotting.
- **`lib/*.c` must not reference sqlite or any symbol outside `lib/`**:
  four installers (briar, spectator, silex, compile_tools) link
  `build/*.o` BLIND, so any lib object in `build/` with an unresolved
  external breaks their links. Records code lives in `tools/`. No
  `externus` data a lib function needs but only generated files define.
- Digests: SHA-256 via `sigillum.h`, 64 hex everywhere, never
  truncated; no timestamp inside a digest.
- The judge never writes to the tree: regeneration lands in scratch
  (`build/fabrica/scriptura/…`), never over a committed file.
- Exit contract 0 all current · 1 something stale or unknown · 2
  nothing judged.
- Every new gate is born red; every fix is proven by a PLANTED FAULT
  that compiles, whose red is predicted before the run, and removed.
- Element and attribute names of the declaration dialect are
  PLACEHOLDERS until Fran names them in T2; the subsystem file's name
  is `aedificatio.stml` as the working name, fixed in T2 (never
  `fabrica.stml` below the root — silex finds that name by ascent).

## Review Focus

1. **A committed artifact edited BY HAND while its inputs stay the
   same** → STALUM ("artificium ipsum mutatum"), never a record hit.
   A record matches only when input digest AND artifact digest both
   match. Test: T6 Step 1 (`_probare_memoria_artificium_mutatum`).
2. **A new header file appears in an include root** (it may shadow a
   previous resolution or resolve a former "system" include) → every
   manifest-derived verdict for that root is STALUM. Directory listings
   of include roots are inputs. Test: T1 Step 1
   (`_probare_directorium_novum`), used again in T6.
3. **The binary's own provenance file is in its closure** (the
   generated `build/fabrica/provenientia/<t>.c` is an annotated
   object) → excluded from its digest; otherwise every install is
   stale the moment it is built. Test: T7 Step 1.
4. **A generator that fails or writes nothing** during `-plenus` →
   IGNOTUM naming the command and its last line, never RECENS by
   comparing two absent files. Test: T1 Step 1
   (`_probare_regeneratio_fracta`).
5. **`bin/fabrica` itself stale** (lib/fabrica.c edited since it was
   built) → the judge says so FIRST, before any verdict it prints.
   Test: T3 Step 5.

## File structure

| file | responsibility |
|---|---|
| `include/aedilis.h`, `lib/aedilis.c`, `tools/aedilis.c` | P1: include form through the seam; `inresolutae` in the fruit and the manifest; stderr warning |
| `aedilis.stml` | P1: new include roots |
| `probationes/probatio_aedilis.c` | P1: quoted-unresolved vs angled cases |
| `apps/mensor/assets/capsula_mensor.c`, `fabrica.tsv` | P2 |
| `include/fabrica.h`, `lib/fabrica.c` (NEW) | T1: pure core — kinds, digests, manifest reading, verdicts, order; T2: declaration reader |
| `probationes/probatio_fabrica.c`, `probationes/fixa/fabrica/` (NEW) | T1–T2 gate on an in-memory seam + declaration fixtures |
| `fabrica.stml`, `fabrica.canon`, `aedificatio.canon` (NEW), `canones.registrum` | T2: root lists subsystems; the declaration dialect |
| `tools/fabrica.c`, `tools/fabrica_struere.sh` (NEW) | T3: CLI + bootstrap; T6: records (sqlite) |
| `silva/`, `tessera/`, `officina/` `aedificatio.stml` (NEW) | T4 amalgam chain; T5 silva generated sources |
| `aedificatio.stml` (root, NEW) | T5: numerals, runae, entitates, capsulae; T7: installed binaries |
| generator scripts (listed in T4/T5) | `FABRICA_SCRIPTURA` scratch mode |
| `include/provenientia.h`, `lib/provenientia.c` (NEW) | T7 |
| installer scripts + tool mains (listed in T7) | T7: write and answer `-provenientia` |
| `tools/fabrica_oraculum.sh` (NEW), `pythonica/silva.py`, `.claude/settings.json`, `.claude/hooks/fabrica-celer.sh` (NEW) | T8 |
| `lib/fabrica.worklog.md`, `project-specs/fabrica-spec-v2.md` "As built", MEMORY | records per task |

---

### Task P1: aedilis names the includes it could not resolve

**Files:** `include/aedilis.h:38-44` (extractor typedef), `:130-138`
(`AedilisFructus`); `lib/aedilis.c:821` (fruit init), `:930-950`
(systemata branch), `:1424-1436` (manifest section); `tools/aedilis.c:355-430`
(`_extractor_silvae`), `:491-530` (memo extractor); `probationes/probatio_aedilis.c:47-120`
(fixture extractor); `aedilis.stml:2-25` (`<inclusa>`).

**Interfaces:**
- Produces: `AedilisExtractor` gains a last parameter
  `Xar** angulatae_out` — Xar of `b32`, parallel to `directivae_out`;
  the extractor MAY leave it `NIHIL` (form unknown → legacy rule:
  systema). `AedilisFructus.inresolutae` (Xar of chorda: the requested
  text of quoted includes resolved nowhere). Manifest section
  `inresolutae` with one `caput via="…"` child per entry, emitted
  after `systemata` (only when non-empty — manifests of clean roots
  stay byte-identical).

- [ ] **Step 1: consumers of the manifest format.** `git grep -n
  'manifestum.stml\|systemata' -- '*.sh' '*.py' '*.c'` outside
  lib/aedilis.c. Record every reader. Rule: a reader that walks
  sections by name is unaffected by a new trailing section; one that
  counts children or assumes the last section is `vexilla-annotata`
  gets fixed in this task. Record in the worklog.
- [ ] **Step 2: failing test.** In the fixture extractor, have
  `lib/alpha.c` also request `"nusquam.h"` (quoted, form known) and
  `<stdio.h>`-form `"stdint.h"` (angled). Fill `angulatae_out` for that
  case only. New section in `principale`:

```c
    {
        imprimere("\n--- Probans inresolutas citatas ---\n");
        /* fructus derivatus ut in probationibus clausurae supra */
        CREDO_NON_NIHIL(fructus->inresolutae);
        CREDO_AEQUALIS_I32(xar_numerus(fructus->inresolutae), I);
        CREDO_CHORDA_AEQUALIS_LITERIS(
            *(chorda*)xar_obtinere(fructus->inresolutae, 0),
            "nusquam.h");
        /* angulata manet inter systemata */
        CREDO_VERUM(_xar_continet(fructus->systemata, "stdint.h"));
        CREDO_FALSUM(_xar_continet(fructus->systemata, "nusquam.h"));
        /* manifestum sectionem fert */
        CREDO_CHORDA_CONTINET(manifestum, "inresolutae");
    }
```

  (`_xar_continet` is a 10-line helper added to the test file: linear
  scan comparing with `chorda_aequalis_literis`.) Every other fixture
  path passes `*angulatae_out = NIHIL`.
- [ ] **Step 3: run red.** `./compile_tests.sh aedilis` → compile error
  (`inresolutae` unknown) — the expected red.
- [ ] **Step 4: implement.** Typedef + all three implementers
  (`_extractor_silvae` fills from `vista.est_angulata` beside each
  `_chordam_in_xar(*directivae_out, …)`; the `-MM` path and the memo
  pass through / leave NIHIL). In `lib/aedilis.c:930-950`: when
  `resoluta.mensura == 0`, look up the form at index `i`; quoted →
  `inresolutae` (deduplicated by the same `visa_*` table pattern),
  else → `systemata` as today. `tools/aedilis.c`: after derivation,
  one stderr line per entry: `AEDILIS CAUTIO: inclusio citata
  inresoluta "x.h" (scopus <via>)` — exit code unchanged (decree:
  aedilis reports, never refuses).
- [ ] **Step 5: run green**, then `./tools/aedilis_porta.sh` (porta
  aedilis) green.
- [ ] **Step 6: measure the three installers.** `bin/aedilis tools/briar.c`,
  `tools/briar_spectator.c`, `tools/silex.c` → expect the CAUTIO lines
  §0.3 predicts (briar 15, spectator 9, silex 2). Numbers differ → stop,
  show Fran.
- [ ] **Step 7: add include roots, measured.** Append to `<inclusa>`:
  `briar/fontes`, `md/fontes`, `officina/instrumenta` (0 basename
  collisions measured 2026-09-29). Re-run Step 6. Then the silex pair
  (`silva_token.h`, `silva_lexema.h`, in `silva/fontes/`): add
  `silva/fontes` LAST and run `sh build/aedilis/silex/struere.sh`
  (hermetic link). Branches: (a) links → keep; (b) duplicate symbols
  against `silva/amalgama/silva.c` → do NOT add the root; instead record
  the two includes as known in the worklog and leave silex reporting 2
  (T7 then reports silex IGNOTUM with that cause). Show Fran the branch.
- [ ] **Step 8: the amalgam manifests.** `./tools/generata_probare.sh`
  stage IV (and VII). Branches: (a) green → new roots touched no amalgam
  closure; (b) a `fontes_generata.h` changed → an amalgam closure was
  ALSO silently incomplete: stop and show Fran the diff before
  regenerating anything.
- [ ] **Step 9: plant.** Remove `briar/fontes` from `<inclusa>` →
  predict: `bin/aedilis tools/briar.c` prints 15 CAUTIO lines and the
  manifest carries them. Run, observe, restore.
- [ ] **Step 10: commit** (`include/aedilis.h lib/aedilis.c
  tools/aedilis.c aedilis.stml probationes/probatio_aedilis.c` +
  worklog). Gates: radix (aedilis), generata. Rebuild `bin/aedilis`
  (`./tools/aedilis_struere.sh`) after.

> **Executed 2026-09-29 — 982fec44.** DEVIATION (Step 2): the entry
> stays in `systemata` too; `inresolutae` is a named subset, because
> Step 1 found ~12 consumers of `--partes` (speculum keeps every non-S
> row) and no reader of manifest sections. Step 7 branch (a): all four
> roots added (briar/fontes, md/fontes, officina/instrumenta,
> silva/fontes); silex links hermetically from its aedilis closure;
> briar's hermetic link misses only the six symbols of the units
> `briar_struere.sh` generates (identity + three capsulae) — closure
> otherwise complete (27 → 73 objects; spectator 43 → 77). Step 8
> branch (a): generata green, amalgam manifests unchanged, snippets
> 38/0. Plant: 14 CAUTIO lines (not 15 — `compendium.h` now resolves
> via officina/instrumenta), as predicted. Worklog:
> tools/aedilis.worklog.md 2026-09-29.

### Task P2: capsula_mensor raw, excubitor's dead rows

**Files:** `apps/mensor/assets/capsula_mensor.c`, `fabrica.tsv:29-34`.

- [ ] **Step 1:** `bin/capsula_generare apps/mensor/assets/mensor.toml`
  → `git diff --stat` shows whitespace-only change (verify: the
  whitespace-stripped files `cmp` equal, as measured 2026-09-29).
- [ ] **Step 2:** delete the six `generatum silva/grammatica/silva_tabulae_*`
  rows from `fabrica.tsv` (targets do not exist; the real files are in
  `silva/fontes/`, covered by T5).
- [ ] **Step 3: proof.** `./tools/mensor_ui_struere.sh`, then `git
  status --short apps/mensor` → empty (was: modified). `./excubitor.sh
  -tacitus silva/` verdict line unchanged from before Step 2.
- [ ] **Step 4: commit** both paths. Gates owed: whatever commissio
  adds (mensor).

> **Executed 2026-09-29 — in TWO commits; the first was half.**
> d09e6dd1 deleted the six dead `fabrica.tsv` rows (verdict of
> `./excubitor.sh silva_tabulae` identical before/after: inspecta 8,
> STALA 5 by mtime; NOTE the `silva/` filter inspects 0 objects — a
> vacuous PURUS). Its capsula half SILENTLY DID NOT LAND: the
> pre-commit hook (`tools/unci-git/pre-commit:123-149`, whole-file
> formatting since 2026-09-01) reformatted the raw capsula back into
> HEAD's bytes, git saw no change, and the next mensor_ui build
> dirtied the tree again. Root cause of the whole generate→format
> pattern (lexicon, runae, entitates): the hook, not a choice.
> Fran chose **route B**: a generated file is committed as its
> generator's exact output. Second commit: the hook skips any staged
> file whose FIRST LINE contains the word `GENERATUM` (unci fumus
> XIX, born red); capsula_generare, runae_generare, entitates and the
> lexicon generator write `GENERATUM` on line 1; seven TOML capsulae,
> runae_tabulae.c, entitates_html_tabula.c, silva_lexicon_c89.{c,h}
> re-committed raw (content identical to HEAD with line 1 dropped and
> whitespace stripped — measured per file); runae `-probare` and
> generata II now compare EXACTLY (born red against the formatted
> files); generata gains stage VIII (entitates, exact). T5's
> `formatio` kind is no longer needed for these families.

### Task T1: lib/fabrica core (pure)

**Files:** Create `include/fabrica.h`, `lib/fabrica.c`,
`probationes/probatio_fabrica.c`, `lib/fabrica.worklog.md`. Regenerate
`compile_tests_fontes_generata.sh` (`./tools/compile_tests_fontes_generare.sh`)
so `lib/fabrica.c` joins SOURCE_FILES.

**Interfaces (Produces — every later task uses these names):**

```c
/* include/fabrica.h */
#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include "sigillum.h"

nomen enumeratio {
    FABRICA_INGRESSUS_FASCICULUS = ZEPHYRUM, /* octeti */
    FABRICA_INGRESSUS_MANIFESTUM,    /* viae manifesti aedilis */
    FABRICA_INGRESSUS_CONFIGURATIO,  /* aedilis.stml tota */
    FABRICA_INGRESSUS_INSTRUMENTUM,  /* binarium aut textus versionis */
    FABRICA_INGRESSUS_DIRECTORIUM    /* nomina ordinata */
} FabricaGenusIngressus;

nomen enumeratio {
    FABRICA_ACTIO_GENERATOR = ZEPHYRUM,
    FABRICA_ACTIO_FORMATIO,
    FABRICA_ACTIO_INSTALLATIO
} FabricaGenusActionis;

nomen enumeratio {
    FABRICA_PROVENIENTIA_REGENERATIO = ZEPHYRUM,
    FABRICA_PROVENIENTIA_RELATIO
} FabricaGenusProvenientiae;

nomen enumeratio {
    FABRICA_RECENS = ZEPHYRUM,
    FABRICA_STALUM,
    FABRICA_IGNOTUM,
    FABRICA_NON_IUDICATUM          /* celer: regeneratio omissa */
} FabricaStatus;

nomen structura {
    FabricaGenusIngressus genus;
                   chorda via;
} FabricaIngressus;

nomen structura {
                       chorda via;        /* artificium */
                       chorda scriptura;  /* ubi regeneratio cadit */
    FabricaGenusProvenientiae provenientia;
} FabricaExitus;

nomen structura {
                  chorda titulus;
    FabricaGenusActionis genus;
                    Xar* mandatum;   /* chorda: argv fixum */
                    Xar* ingressus;  /* FabricaIngressus */
                    Xar* exitus;     /* FabricaExitus */
                  chorda sedes;      /* "plagula:linea" */
} FabricaActio;

/* Sutura: machina discum et processus per eam SOLAM tangit. */
nomen structura {
    vacuum* datum;
    /* FALSUM = absens */
    b32 (*legere)(vacuum* datum, constans character* via,
                  Piscina* piscina, chorda* contentum_out);
    /* nomina (chorda) ordinata; FALSUM = directorium absens */
    b32 (*enumerare)(vacuum* datum, constans character* via,
                     Piscina* piscina, Xar** nomina_out);
    /* mandatum currere (argv fixum, variabile FABRICA_SCRIPTURA
     * posita); FALSUM + ultima linea in causa_out si fractum */
    b32 (*currere)(vacuum* datum, constans Xar* mandatum,
                   constans character* scriptura_dir,
                   Piscina* piscina, chorda* causa_out);
    /* binarium '-provenientia' rogare; FALSUM = nulla relatio */
    b32 (*rogare)(vacuum* datum, constans character* via,
                  Piscina* piscina, chorda* relatio_out);
    /* memoria: VERUM si verificatio (titulus, ingressus, artificium)
     * iam scripta; NIHIL licet (sine memoria) */
    b32 (*meminisse)(vacuum* datum, constans character* titulus,
                     constans Sigillum* ingressus,
                     constans Sigillum* artificium);
} FabricaSutura;

nomen structura {
           chorda artificium;
    FabricaStatus status;
           chorda causa;
} FabricaIudicium;

/* Viae manifesti aedilis (obiecta, capita, vendores; systemata
 * NON) et inresolutae. FALSUM + causa si malformatum. */
b32
fabrica_manifestum_legere (
                 chorda  contentum,
               Piscina* piscina,
                   Xar** viae_out,
                   Xar** inresolutae_out,
                chorda* causa_out);

/* Sigillum copiae ingressuum: pro quoque ingressu ordine viae,
 * "via NUL sigillum(octetorum)"; manifestum in viae suas
 * explicatur, directorium in nomina. Exclusa (Xar chorda, NIHIL
 * licet) praetermittuntur (provenientia binarii ipsius). FALSUM +
 * causa: ingressus absens, manifestum incompletum (inresoluta
 * nominata). */
b32
fabrica_ingressus_sigillare (
    constans FabricaSutura* sutura,
     constans FabricaActio* actio,
                constans Xar* exclusa,
                   Piscina* piscina,
                  Sigillum* sigillum_out,
                    chorda* causa_out);

/* Iudicium unius exitus. plenus FALSUM = celer (regeneratio numquam;
 * relatio et memoria licent). */
FabricaIudicium
fabrica_iudicare (
     constans FabricaSutura* sutura,
      constans FabricaActio* actio,
     constans FabricaExitus* exitus,
                        b32  plenus,
                    Piscina* piscina);

/* Actiones ordine dependentiae: ingressus qui exitus alterius est
 * post eam. NIHIL + causa (cyclus, viis nominatis). */
Xar*
fabrica_ordinare (
    constans Xar* actiones,   /* FabricaActio* */
         Piscina* piscina,
          chorda* causa_out);
```

Verdict rules (the gate pins each):
- REGENERATIO, `plenus`: digest inputs; if `meminisse(titulus,
  ingressus, sigillum(artificium))` → RECENS ("memoria"); else
  `currere(mandatum, scriptura)`; failure → IGNOTUM with the command
  and the causa; `scriptura` absent after success → IGNOTUM
  ("generator nihil scripsit"); bytes equal → RECENS; unequal → STALUM
  with the count of differing lines.
- REGENERATIO, celer: record hit → RECENS ("memoria"); else
  NON_IUDICATUM.
- RELATIO: `rogare(via)`; no report → IGNOTUM ("sine provenientia");
  parse `ingressus <64 hex>`; equal to today's digest → RECENS; else
  STALUM.
- Any input missing, or a manifest with `inresolutae` → IGNOTUM with
  the path (Q9: refuse, name it).

- [ ] **Step 1: failing tests.** `probationes/probatio_fabrica.c` with
  an in-memory seam: a table `via → contentum` (`_Discus`), a directory
  table, a scripted `currere` that writes a chosen text to
  `<scriptura>` in the table or fails, a scripted `rogare`, and a
  `meminisse` over a small list. Sections (each a block in
  `principale`, credo pattern of `probatio_piscina.c`):
  - `_probare_sigillum_ordo`: same inputs listed in two orders → equal
    digests; one byte changed in one input → different.
  - `_probare_manifestum`: a fixture manifest (from
    `probationes/fixa/fabrica/manifestum_parvum.stml`, a trimmed copy
    of a real one) → `viae` = its objects + headers + vendor sources,
    `systemata` excluded; with a section `inresolutae` → digest FALSUM,
    causa names the header.
  - `_probare_directorium_novum`: an input of kind DIRECTORIUM; add a
    name to the listing → digest changes (Review Focus 2).
  - `_probare_regeneratio`: scripted generator writes the committed
    text → RECENS; writes different text → STALUM, causa counts lines;
    fails → IGNOTUM naming the command; succeeds but writes nothing →
    IGNOTUM (Review Focus 4); celer → NON_IUDICATUM.
  - `_probare_relatio`: report with today's digest → RECENS; other
    digest → STALUM; no report → IGNOTUM "sine provenientia".
  - `_probare_ordo`: B reads A's output → A before B; cycle A↔B → NIHIL
    and causa names both.
- [ ] **Step 2: run red** — `./compile_tests.sh fabrica` fails to
  compile (header absent).
- [ ] **Step 3: implement** `lib/fabrica.c` (manifest via `stml_legere`
  + `stml_invenire_omnes_liberos`; digest via `sigillum_incipere/
  addere/finire`; line counting for the causa; topological order by
  matching `FabricaIngressus.via` to `FabricaExitus.via`).
- [ ] **Step 4: run green.** Then confirm the blind-link rule:
  `nm build/fabrica.o | grep ' U '` lists only symbols defined by
  other `build/*.o` (spot-check with `./tools/silex_struere.sh`
  linking).
- [ ] **Step 5: plant.** Make `fabrica_ingressus_sigillare` skip the
  `via` bytes (hash contents only) → predict `_probare_sigillum_ordo`
  still green but a new assert — two inputs swapping their contents →
  different digests — goes red; add that assert, run, observe, remove
  the plant.
- [ ] **Step 6: commit** (header, lib, test, fixture, regenerated
  snippet, worklog). Gates: radix, generata (snippet regenerated).

> **Executed 2026-09-29** (commit below). As planned, with: enum
> names after the Latin lint — `FABRICA_ACTIO_INSTITUTIO` (not
> INSTALLATIO) and type `FabricaProvenientia`; the STALUM causa reads
> `regeneratio differt (lineae differentes: N)` (decimal). Plant
> (Step 5) refined: the swap assert did not pin the path (see
> lib/fabrica.worklog.md); a rename assert was added and went red
> alone. 55/55. Blind link verified (`nm -u` + `compile_tools.sh`).

### Task T2: the declaration dialect

**Files:** `fabrica.stml`, `fabrica.canon` (dialect v2),
`aedificatio.canon` (NEW), `canones.registrum`, `lib/fabrica.c` +
`include/fabrica.h` (reader), `probationes/probatio_fabrica.c`,
`probationes/fixa/fabrica/*.stml`.

**Interfaces:**
- Produces: `Xar* fabrica_declarationes_legere(chorda contentum,
  constans character* via, Piscina*, InternamentumChorda*, chorda*
  causa_out)` → Xar of `FabricaActio` with `sedes` = "via:linea";
  `Xar* fabrica_subsystemata_legere(chorda contentum, Piscina*,
  InternamentumChorda*, chorda* causa_out)` → Xar of chorda (dirs).

- [ ] **Step 1: names with Fran.** Show the placeholder shape and get
  names (element for an action, input, output; attribute for kind,
  provenance, scratch; the subsystem file name, working name
  `aedificatio.stml`):

```xml
<!-- fabrica.stml (radix) - dialectus fabrica v2 -->
<fabrica titulus="rhubarb">
  <subsystema via="silva"/>
  <subsystema via="tessera"/>
</fabrica>

<!-- silva/aedificatio.stml -->
<aedificatio>
  <actio titulus="amalgama_silva" genus="generator">
    <mandatum>
      <verbum!(>./silva/amalgamare.sh
    </mandatum>
    <ingressus genus="fasciculus"
               via="silva/instrumenta/principalia/fontes_generata.h"/>
    <exitus via="silva/amalgama/silva.c"
            provenientia="regeneratio"
            scriptura="silva/amalgama/silva.c"/>
  </actio>
</aedificatio>
```

  (`scriptura` = the path RELATIVE to the scratch dir where the
  generator, run with `FABRICA_SCRIPTURA=<dir>`, writes.)
- [ ] **Step 2: failing tests.** Fixtures: `decl_bona.stml` (two
  actions, one input of each kind), `decl_genus_ignotum.stml`,
  `decl_sine_exitu.stml`, `radix_bona.stml`. Assert counts, fields,
  `sedes` line numbers, and the two refusals name their line.
- [ ] **Step 3: run red; implement; run green.**
- [ ] **Step 4: canons.** `fabrica.canon` v2 admits `subsystema`
  children (silex reads only existence — `lib/silex.c:277` — so it is
  unaffected; confirm `./compile_tests.sh silex` green). New canon for
  the subsystem dialect, registered by root element in
  `canones.registrum`. `bin/canon_examen` on every fixture: bona pass,
  malae fail.
- [ ] **Step 5: plant.** Canon without `necessarium` on the output's
  `via` → predict `decl_sine_exitu` passes the canon (but still fails
  the reader); observe; restore.
- [ ] **Step 6: commit.** Gates: radix, canon (commissio adds).

> **Executed 2026-09-30** (commit below). Names as proposed (Fran:
> "looks good"). One layout correction: `verbum` captures go one per
> line (a raw capture eats to end of line). Reader also refuses a
> duplicate `titulus` (scratch-dir collision) — the one rule the canon
> cannot express; `decl_titulus_duplex.stml` passes the canon by
> design. Gate 88/88; plant on `minimum="1"` as predicted. Glossary:
> `subsystema`. Root `fabrica.stml` untouched until T4.

### Task T3: bin/fabrica and its bootstrap

**Files:** Create `tools/fabrica.c`, `tools/fabrica_struere.sh`.

**Interfaces:**
- Consumes: T1, T2.
- Produces: `bin/fabrica iudicare [-plenus] [artificium…]`,
  `bin/fabrica digestum <titulus-actionis>` (prints 64 hex),
  `bin/fabrica -provenientia` (answers only from T7 on; until then
  the flag is absent and usage does not list it).
  Real seam: `legere` = file read; `enumerare` = `iter_directoria`
  sorted; `currere` = `processus_exsequi` with env
  `FABRICA_SCRIPTURA`, cwd = repo root, deadline 600 s; `rogare` =
  `processus_exsequi(via, "-provenientia")`, 5 s deadline;
  `meminisse` = NIHIL until T6.
- Output: one line per non-RECENS artifact
  `STALUM <via> - <causa>` / `IGNOTUM …` / `NON IUDICATUM (celer) …`,
  then `SANATIO:` and the healing commands in `fabrica_ordinare`
  order, then `fabrica: N recentia, M stala, K ignota`.

- [ ] **Step 1:** `tools/fabrica_struere.sh` in the `manus_struere.sh`
  shape (aedilis → generated struere.sh → `rm` then `cp` to
  `bin/fabrica`). Build it.
- [ ] **Step 2: failing CLI checks** in a scratch script (becomes T8's
  gate body): no declarations found → exit 2 and "nihil iudicatum";
  unknown flag → exit 2.
- [ ] **Step 3: implement `tools/fabrica.c`**: read root `fabrica.stml`,
  each subsystem's declaration file, judge every output, print, exit.
  Orphans (spec v1 Q24, narrowed): for every `build/aedilis/<t>/`
  whose manifest's `scopus` file is gone, print `ORPHANUM:
  build/aedilis/<t>/ (scopus <via> absens)` — never delete. On
  today's tree: `probatio_toml`, `probatio_arbor_quaestio`.
- [ ] **Step 4: run** against an empty declaration set → exit 2.
- [ ] **Step 5: self-staleness (Review Focus 5).** Declare `bin/fabrica`
  itself in the root declarations as an INSTALLATIO with a MANIFESTUM
  input (`build/aedilis/fabrica/manifestum.stml`); until T7 it reports
  IGNOTUM "sine provenientia" — the first line printed. Assert the
  line order in the scratch script.
- [ ] **Step 6: commit.** Gates: radix (+ whatever commissio adds).

> **Executed 2026-09-30** (commit below). CLI as specified; celer
> NON IUDICATUM does not fail the exit. Root declarations via
> `<subsystema via="."/>` + root `aedificatio.stml` (the judge itself).
> Gate `tools/fabrica_fumus.sh` IX stages (registered in PORTAE at T8).
> Code preceded the gate — each behavior then proven red by a plant
> (A: self-first order → IX only; B: cross-subsystem duplicate → V
> only). Orphans on the live tree: 8 (not 2). The mtime stale-build bug
> struck during plant B's restore (see lib/fabrica.worklog.md; ledger
> ictus). Run time 0.02 s.

### Task T4: the amalgam chain declared

**Files:** `silva/aedificatio.stml`, `tessera/aedificatio.stml`,
`officina/aedificatio.stml` (NEW); `fabrica.stml` lists them;
`silva/amalgamare.sh`, `tessera/amalgamare.sh`, `officina/amalgamare.sh`,
`tools/amalgama_fontes_generare.sh`, `tools/amalgama_excludenda_generare.sh`
(scratch mode).

- [ ] **Step 1: measure each generator's output paths** (`grep -n` for
  where each writes; the amalgamators already write
  `build/comparatio_X.c` under `AMALGAMA_COMPARARE=1`). Rule for the
  scratch mode, one shape everywhere: when `FABRICA_SCRIPTURA` is set,
  every output path `P` becomes `$FABRICA_SCRIPTURA/P` (mkdir -p its
  dir), inputs are read from the tree as usual, verifications are
  skipped (as `AMALGAMA_COMPARARE` already does), exit 0/1/2 kept.
- [ ] **Step 2: implement the scratch mode** in the five scripts; for
  each, run once with `FABRICA_SCRIPTURA=build/fabrica/scriptura/probe`
  and `cmp` the scratch output against the committed file → equal on
  today's tree (they are current: generata IV/VII green).
- [ ] **Step 3: declarations.** Per subsystem: `fontes_generata.h`
  and `excludenda_generata.h` (inputs: MANIFESTUM of the root's aedilis
  manifest, CONFIGURATIO, INSTRUMENTUM `bin/aedilis`, DIRECTORIUM of
  each include root, FASCICULUS `fontes_politica.sh`) → amalgam `.c`
  (inputs: the two `.h`, the amalgamator's sources as FASCICULUS).
- [ ] **Step 4: agreement on a clean tree.** `bin/fabrica iudicare
  -plenus` → the nine artifacts RECENS; `./tools/generata_probare.sh`
  stages IV and VII green.
- [ ] **Step 5: plant (agreement on red).** Edit a comment in one
  header of `silva/fontes/` → predict: fabrica STALUM for
  `silva/amalgama/silva.c` (and nothing else of the nine), generata VII
  STALUM for silva. Run both, compare, restore.
- [ ] **Step 6: commit.** Gates: generata, silva (amalgam scripts),
  tessera, officina.

> **Executed 2026-09-30** (commit below). Nine artifacts declared and
> RECENS under `-plenus` (42 s); agreement with generata on a valid
> plant (silva.c only). Deviations/finds (lib/fabrica.worklog.md):
> fontes_generata.h is its own input (praelatio); excludenda writes in
> place transiently (trap-restored); excludenda links `<sub>/build/`
> objects built by amalgamare.sh — declared now (found via an invalid
> first plant that broke them); FABRICA_SCRIPTURA exported absolute;
> orphans-under-flags bug fixed (fumus VIII). Declared inputs do NOT
> yet cover the radices' closures — T6 must add them before records
> may skip regeneration for these actions.

### Task T5: generated sources declared

**Files:** `silva/aedificatio.stml` (lexicon, grammar tables), root
`aedificatio.stml` (numerals, runae, entitates, 7 TOML capsulae),
generators: `silva/instrumenta/lexicon_c89_generare.sh`,
`silva/generare.sh`, `tools/latina_numeri.sh`,
`tools/runae_generare.sh`, `tools/entitates_html_generare.sh`,
`bin/capsula_generare` (via a wrapper or its toml's output path).

- [ ] **Step 1: measure** each generator's output path and whether it
  already takes an output dir (lexicon has `EXITUS_DIR`; `silva/generare.sh`
  takes an output base argument; `capsula_generare` writes where the
  toml says). Apply the T4 scratch rule to the rest.
- [ ] **Step 2: `formatio`.** For lexicon .c, runae_tabulae.c,
  entitates_html_tabula.c: a FORMATIO action after the generator —
  `./silva/formator.sh -scribere <scratch file>`; inputs include the
  formator binary (INSTRUMENTUM). Verify on today's tree: generator →
  formatio → `cmp` with committed → equal (measured 2026-09-29 for all
  three). If the formator refuses a path outside the tree's source
  dirs: copy into `build/fabrica/scriptura/` mirror, format there.
- [ ] **Step 3: declarations** for all families; the two TOML-less
  capsulae (`book_assets/capsula_libri.c`, `probationes/capsula_assets.c`)
  are declared with `provenientia` absent → IGNOTUM "generator ignotus"
  (reported, Fran decides later).
- [ ] **Step 4: agreement.** `-plenus` RECENS for all current ones;
  generata II, V, VI green.
- [ ] **Step 5: new coverage born red.** Three plants, one at a time,
  each predicted: (a) edit a production in `silva/grammatica/sceletum.stml`
  → `silva_tabulae_sceleti.{c,h}` STALUM; (b) edit one entity in
  entitates' source table → `lib/entitates_html_tabula.c` STALUM; (c)
  edit `apps/villa/assets/villa.toml` asset list → `capsula_villa.c`
  STALUM. Restore each.
- [ ] **Step 6: commit.** Gates: generata + owed.

> **Executed 2026-09-30** (commit below). 26 artifacts added (35 total;
> +`silva/c89.canon`, generated too); no formatio (route B); capsulae via
> new `tools/capsula_regenerare.sh`. `-plenus` 78 s, 35 RECENS; three
> plants red as predicted (two outside any old gate). Deviation: the
> two TOML-less capsulae are left undeclared (desideratum 01M3R27R5J)
> rather than inventing an "unknown provenance" kind.

### Task T6: records, and the snippet win (runs AFTER T7)

> **Amended 2026-09-30:** records are consulted only for actions marked
> cacheable (attribute on `actio`, name fixed with Fran at T6 start;
> default: not cacheable). A cacheable action must have manifest-derived
> or otherwise provably complete inputs — Step 3's measurement decides
> which actions qualify; the rest keep regenerating.

**Files:** `tools/fabrica.c` (records), `tools/fabrica_memoria.c` (NEW,
sqlite; tool-only), declarations for the 37 committed
`*_fontes_generata.sh`.

**Interfaces:**
- Produces: `build/fabrica.db`, table
  `verificationes(titulus TEXT, ingressus TEXT, artificium TEXT,
  tempus INTEGER, duratio_ms INTEGER, PRIMARY KEY(titulus, ingressus,
  artificium))`; a row is written only after a `-plenus` RECENS by
  regeneration; `meminisse` = row exists for (titulus, today's input
  digest, today's artifact digest).

- [ ] **Step 1: failing tests (core).** In `probatio_fabrica.c`:
  `_probare_memoria`: record hit → RECENS without calling `currere`
  (the scripted `currere` counts calls: 0); `_probare_memoria_artificium_mutatum`:
  same inputs, artifact bytes changed → `currere` called, STALUM
  (Review Focus 1).
- [ ] **Step 2: run red; wire `meminisse` into `fabrica_iudicare`; run
  green.**
- [ ] **Step 3: measure the snippets' true inputs.** A snippet is the
  enumeration of aedilis closures of named roots. Find what each
  generator runs (`# regeneratio:` line) and whether `bin/aedilis
  <root>` leaves a manifest per root in `build/aedilis/<t>/`. Branches:
  (a) manifests are left → inputs = MANIFESTUM of each root +
  CONFIGURATIO + INSTRUMENTUM `bin/aedilis` + DIRECTORIUM of every
  include root + the globbed dirs (`silva/fontes` for
  `silva_fontes_generare.sh`); (b) not left → add a flag to the
  generators to keep them. Show Fran the branch taken.
- [ ] **Step 4: sqlite in the tool.** `tools/fabrica_memoria.c` opens
  `build/fabrica.db` (create table if absent), implements `meminisse`
  and `scribere`. aedilis links vendor sqlite already (object in
  `build/aedilis/obiecta/`).
- [ ] **Step 5: the win, measured.** `time bin/fabrica iudicare -plenus`
  twice on an unchanged tree: first ≈ today's generata III cost
  (~300 s), second: record; target ≪ 60 s. Record both numbers in the
  worklog and on park …GTQHQ.
- [ ] **Step 6: plant.** Change one byte in a `.c` inside ONE snippet's
  closure → predict: that snippet regenerated (and still RECENS if the
  file list did not change), all others record hits. Observe via the
  per-artifact "memoria" / "regeneratum" reason. Restore.
- [ ] **Step 7: commit.** Gates: radix, generata.

### Task T7: installed binaries answer `-provenientia`

**Files:** Create `include/provenientia.h`, `lib/provenientia.c`,
`probationes/probatio_provenientia.c`. Modify the mains and installers
of: manus, mensor, mensor_ui, stml, aedilis, natura (4 binaries), canon
(2), fabrica; then briar, spectator, silex per P1's outcome.

**Interfaces (amends spec §III — no `externus` data in lib, per the
blind-link constraint):**

```c
/* include/provenientia.h */
nomen structura {
    constans character* artificium;  /* e.g. "bin/manus" */
    constans character* ingressus;   /* LXIV hex */
    constans character* commissum;   /* "<hash>" aut "<hash> SORDIDUM" */
} ProvenientiaRelatio;

/* Si argv '-provenientia' fert, relationem scribit (quattuor
 * lineae) et VERUM reddit - vocans tum exit 0. */
b32
provenientia_respondere (
                          s32  argc,
                   character** argv,
    constans ProvenientiaRelatio* relatio);
```

The installer writes `build/fabrica/provenientia/<t>.c` defining
`constans ProvenientiaRelatio provenientia_<t> = { … };` (only when its
content changes — no needless relink); the tool's main declares it
`externus` and calls `provenientia_respondere` first. The generated .c
enters the aedilis closure by an object annotation in the tool's main
comment (origo annotatio; absent-until-generated is allowed).

- [ ] **Step 1: failing tests.** `probatio_provenientia.c`: argv
  without the flag → FALSUM, nothing printed; with it → VERUM and the
  four exact lines (capture via a buffer-writing variant or compare
  stdout in the credo pattern the house uses for printers). In
  `probatio_fabrica.c`: `_probare_exclusa`: the provenance file listed
  as input and in `exclusa` → digest equals the digest without it
  (Review Focus 3).
- [ ] **Step 2: run red; implement; run green.**
- [ ] **Step 3: `bin/fabrica digestum <titulus>`** uses the same
  `fabrica_ingressus_sigillare` with `exclusa` = the action's
  provenance file.
- [ ] **Step 4: installers, one at a time** (manus first, the six-week
  case): installer runs `bin/aedilis`, then `bin/fabrica digestum`,
  writes the .c, then the struere.sh; `bin/manus -provenientia` →
  four lines; `bin/fabrica iudicare` → bin/manus RECENS.
- [ ] **Step 5: plant (the six-week bug).** Edit a comment in
  `lib/manus.c` → predict: `bin/manus` STALUM naming `lib/manus.c`
  (when a record lists changed inputs) or "ingressus mutati"; rebuild
  → RECENS. Restore.
- [ ] **Step 6: the rest of the list**, each ending with its RECENS
  line; briar keeps `-versio`'s `fontes` line unchanged (briar-fumus XX
  green); `bin/` binaries without an installer → one line `IGNOTUM:
  N binaria in bin/ sine declaratione` (Q36).
- [ ] **Step 7: commit** (in two commits if large: library + manus;
  then the rest). Gates: radix, briar, briar-fumus (briar main
  touched), and owed.

> **Executed (part 1) 2026-09-30** (commit below): library, writer
> script, bin/fabrica (built twice for a self-computed digest) and
> bin/manus declared RELATIO and RECENS; the six-week bug caught by a
> plant. Deviations: `_digestum` first missed the exclusion (caught by
> the judge itself; fumus XI added); pre-T7 bootstrap needs pass-1
> tolerance. Part 2 (mensor, mensor_ui, stml, aedilis, natura, canon,
> briar, spectator, silex) follows.

### Task T8: surfaces and the oracle gate

**Files:** Create `tools/fabrica_oraculum.sh`,
`.claude/hooks/fabrica-celer.sh`; modify `pythonica/silva.py`
(`PORTAE`, `commissio`), `.claude/settings.json` (SessionStart).

- [ ] **Step 1: the oracle gate, born red.** `tools/fabrica_oraculum.sh`
  runs `./tools/generata_probare.sh` and `bin/fabrica iudicare
  -plenus`, maps each generata stage line to the fabrica verdicts of
  the same artifacts, exit 1 on any disagreement (naming both
  verdicts). Plant: make fabrica skip one family (declaration
  commented out → IGNOTUM where generata says ok) → gate red. Restore
  → green. Register as `fabrica` in `PORTAE` with its "cucurrit" regex.
- [ ] **Step 2: celer at session start.** Hook runs `bin/fabrica
  iudicare` (celer) and prints only non-RECENS lines (silent when all
  current); measure wall time on this tree — must be < 2 s (spec §I.3
  predicts ≤ 1 s); record the number.
- [ ] **Step 3: commissio.** `silva.commissio` runs `bin/fabrica
  iudicare -plenus` on the artifacts whose inputs intersect the
  committed paths; a STALUM blocks the commit with the healing command
  (the same posture as the Latin lint).
- [ ] **Step 4: records.** Spec v2 "As built" section, `lib/fabrica.worklog.md`,
  memory, ledger (park 01KZYN4VPZ note; …GTQHQ with T6's numbers);
  `generata` stages stay until the oracle gate has agreed on real
  history (Q15).
- [ ] **Step 5: commit.** Gates: fabrica (new), generata, owed.

---

## Self-review (2026-09-29)

- Spec coverage: §0.1 → T2; §0.2 → P2 (rows), excubitor otherwise
  untouched (Q35); §0.3 → P1; §0.4 → T5 (+P2 capsula); §0.5 → T3 Step 3 (ORPHANUM lines). §I.3 budget → T8 Step 2. §III → T7 (interface amended, reason
  stated). §IV table → P1–T8 one to one.
- Types: `FabricaActio`, `FabricaExitus`, `FabricaSutura`,
  `FabricaIudicium`, `fabrica_ingressus_sigillare` (with `exclusa`)
  used consistently T1–T8; `meminisse` defined in T1, filled in T6.
- Placeholders: element names are placeholders BY DECISION (Fran names
  in T2); every measurement step names its branches.
