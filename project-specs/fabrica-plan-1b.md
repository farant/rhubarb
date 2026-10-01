# fabrica plan 1b — the vocabulary, then the executor (`sanare`)

> **For agentic workers:** execute task by task with the executing-plans
> discipline (test first, run red, implement, run green, plant, commit),
> INLINE on main, ONE TASK PER TURN with Fran's approval, no subagents.
> Several tasks OPEN with a measurement whose answer picks between
> named branches; a result outside the named branches is shown to Fran
> before work continues. Steps use checkbox syntax. Written 2026-09-30
> from `fabrica-spec-1b.md` (25194075; decisions Q38–Q47 in its §IX,
> ledger decretum 01M3TS766MNBV7T386AYEEZVHP). On approval: one opus
> per task in park 01KZYN4VPZ; `silva.commissio(opus=ID)` closes each.

**Goal:** fabrica states its vocabulary in code — artifact types
(enumerare, sigillare, locare) × currency strategies (iudicare:
regeneratio, relatio, memoria, ignota), composites judged worst-of —
and gains the executor `bin/fabrica sanare`: judge, run the stale
actions in dependency order inside one envelope, re-judge, report;
installers and briar decomposed into actions of one output type.

**Architecture:** the pure core (`lib/fabrica.c`) keeps reaching disk
and processes only through `FabricaSutura`; the two dispatch points
that switch on a kind today (`_particulas_colligere`, the
`provenientia` branch in `fabrica_iudicare`) become two registries of
structs of function pointers. `sanare`'s logic (order, skip, re-judge,
post-condition, footprint comparison) is core code tested on the
in-memory disk; the tool supplies new seam members (`agere`,
`vestigium_capere`, cursus records) that touch processes, the clock and
sqlite. Declarations stay STML; combinators stay C (Canon decree).

**Tech stack:** C89 in Latin (`latina.h`), `stml.h`, `sigillum.h`,
`processus.h`, `filum.h`, vendor sqlite via `scrinium` (tool only),
credo gates, bash for generators, installers and `fabrica_fumus.sh`.

**Spec:** `project-specs/fabrica-spec-1b.md` (§0 findings, §II
vocabulary, §III mapping, §IV order, §V fixes, §VI oracle, §IX
decisions); `fabrica-spec-v2.md` §VII "As built" for what 1a left.

## Global constraints

- C89 under `tools/vexilla.sh` flags. Latin identifiers; latina macros
  are forbidden identifiers (`nomen`, `casus`, `per`, `duplex`, …);
  single capitals and every Roman numeral 0–3999 are macros. `chorda`
  is not NUL-terminated; `i32`/`i64` are UNSIGNED (`s32`/`s64` for
  signed values: exit codes, mtimes). Names use `titulus`, never
  `nomen`.
- Lines ≤ 72 columns. Formatter: record `./silva/formator.sh -vitia`
  per touched file at HEAD first; commit with NO new divergence.
- **No `<word` tag openings in C comments or string literals** (the
  examen scanner evokes them).
- New identifier words: `git add -N` new files, then
  `./oratio/vocabula.sh -nova` must say NOVA 0 before committing.
- Commit only through `silva.commissio(msg, viae, portae)` with explicit
  paths; never Fran's files (`FAQ.md`, `gesta/annales/*`,
  `silva/grammatica/c89-formatted.stml`). commissio adds owed gates.
  Never build while a `commissio_umbra` is snapshotting.
- **`lib/*.c` must not reference sqlite or any symbol outside `lib/`**
  (installers link `build/*.o` blind). Records, processes, the clock
  and the tree walk live in `tools/fabrica.c` behind seam members.
- Digests: SHA-256 via `sigillum.h`, 64 hex, never truncated; no
  timestamp inside a digest.
- **Judging never writes to the tree** (scratch only). **`sanare` writes
  in place and never commits** (Q43); it never deletes a source.
- **Serial** (Q41): one action at a time; `-siccum` may SHOW what could
  run together, nothing runs together.
- Exit contract, both verbs: 0 all current/healed · 1 something stale,
  unknown, failed or skipped · 2 nothing judged, usage, lock held,
  judge itself stale.
- **Tests never touch the real `~/.bin`**: every fumus stage that runs a
  copy action sets `HOME` to a scratch directory.
- Every new gate is born red; every fix is proven by a PLANTED FAULT
  that compiles (`clang -fsyntax-only` the plant first), whose red is
  predicted before the run, and removed.
- Plant the fault, never the gate: when a plant needs a tree edit, use
  a comment or a byte in a source, restore by `git checkout -- PATH`.
- Element and attribute names marked PLACEHOLDER are fixed with Fran at
  the start of the task that introduces them (Q45 named the rest).

## Review Focus

1. **`bin/fabrica` itself stale when `sanare` starts** (lib/fabrica.c
   edited, not rebuilt) → refuse before running anything, exit 2,
   naming `./tools/fabrica_struere.sh`; a stale judge would heal with
   stale digests and record them. Test: T3 Step 6 (fumus XX).
2. **Per-run memos outlive the bytes they describe** (`sigilla`,
   `digesta`, `regenerationes` are keyed by path/title for ONE judge
   run; `sanare` changes files mid-run) → after every action that
   ran, all three are emptied, so a downstream action is judged on the
   NEW bytes. Test: T3 Step 1 (`memoria per cursum purgata`).
3. **An action exits 0 but its output is not current** (the masked
   `cp`, a generator that writes nothing, a link that writes elsewhere)
   → FRACTUM "exitus 0 sed non RECENS", never SANATUM. Test: T3 Step 1
   (`post-condicio`), then live in T5 Step 7.
4. **An upstream action fails** → every action downstream of it is
   OMISSUM naming the failed one and is never run against half-built
   inputs; independent actions still run. Test: T3 Step 1
   (`dependentia fracta`).
5. **A copy target outside the tree** (`~/.bin/X`): `~` expands from
   `HOME`, an absent `~/.bin` is created, an absent target is STALUM
   "artificium absens" (fresh machine), and no test reaches the real
   home. Test: T5 Step 1 (core `~` expansion) and Step 6 (fumus XXIII,
   `HOME` redirected).

## File structure

| file | responsibility |
|---|---|
| `include/fabrica.h`, `lib/fabrica.c` | T1 types/strategies/registries; T2 composites, preconditions, ignota; T3 `fabrica_sanare`; T4 footprint comparison and waves |
| `probationes/probatio_fabrica.c` | each task's core tests on the in-memory disk (`DiscusFictus` gains scripted `agere` in T3, snapshots in T4) |
| `tools/fabrica.c` | T2 composite selection/verdict lines; T3 `sanare` CLI + `agere` seam (log, clock, deadline); T4 `vestigium_capere` (tree walk); T5 `~` expansion; T7 `cursus` |
| `tools/fabrica_fumus.sh` | stages XV–XXIV |
| `aedificatio.canon` | T1 `genus` on exitus; T2 composite + precondition elements; T4 footprint element; T5 cheap-regeneration attribute (if Fran picks it) |
| root `aedificatio.stml`, `silva/`, `tessera/`, `officina/` `aedificatio.stml` | T2 composites; T4 footprints; T5 preconditions, split installers; T6 briar units |
| `compile_tests.sh`, `compile_tools.sh` | T5: a real objects-only mode (exit 0) |
| `tools/{briar,briar_spectator,stml,silex}_struere.sh`, `tools/instituere.sh` (NEW) | T5: link only; the copy is its own action |
| `tools/mensor_ui_struere.sh` | T5: stops regenerating the committed capsula (Q44) |
| `tools/corpus_infixum.sh`, `tools/briar_{facies,icon,mutationes}_capsula.sh` | T6: scratch mode, one output each |
| `lib/fabrica.worklog.md`, `fabrica-spec-1b.md` "As built", MEMORY, ledger | records per task; T7 closes |

---

### Task T1: the two interfaces, behaviour-preserving

The refactor that tests the vocabulary: if one of today's eight input
kinds or two provenances does not fit the verbs cleanly, the interface
is wrong — stop and show Fran before building on it (spec §IV.1).

**Files:** `include/fabrica.h` (enums `FabricaGenusIngressus`,
`FabricaProvenientia` removed); `lib/fabrica.c:300-700` (the
`_X_explicare` family, `_particulas_colligere`, `fabrica_actio_tacta`),
`:893-1180` (`_relationem_iudicare`, `fabrica_iudicare`), `:1260-1310`
(`_genus_ingressus`, `_provenientia`); `tools/fabrica.c` (`-tacta`
filter); `probationes/probatio_fabrica.c` (helpers `_ingressum_addere`,
`_exitum_addere` take names); `aedificatio.canon` (optional `genus` on
`exitus`).

**Interfaces:**
- Produces (in `include/fabrica.h`):

```c
nomen structura FabricaGenus     FabricaGenus;
nomen structura FabricaStrategia FabricaStrategia;

/* particula: via (plagula, aut directorium cum '/' finali) et
 * sigillum - quod ingressus ad sigillum actionis confert */
nomen structura {
      chorda via;
    Sigillum octeti;
} FabricaParticula;

/* locus: ubi artificium habitat aut quod ingressus custodit */
nomen enumeratio {
    FABRICA_LOCUS_PLAGULA = ZEPHYRUM, /* via ipsa */
    FABRICA_LOCUS_PLAGULAE,           /* gradu 0, suffixis filtratae */
    FABRICA_LOCUS_ARBOR               /* arbor tota sub via */
} FabricaFormaLoci;

nomen structura {
    FabricaFormaLoci forma;
              chorda via;
              chorda suffixa;   /* PLAGULAE: ".c .h"; vacua = omnes */
} FabricaLocus;

nomen structura {
    constans FabricaGenus* genus;
                    chorda via;
                    chorda suffixa;
} FabricaIngressus;

nomen structura {
                        chorda  via;
                        chorda  scriptura;
        constans FabricaGenus*  genus;      /* absens: ex strategia */
    constans FabricaStrategia*  strategia;  /* attributum provenientia */
} FabricaExitus;

structura FabricaGenus {
    constans character* titulus;
    /* sigillare: particulas ingressus addere (sigillum actionis) */
    b32 (*sigillare)(constans FabricaSutura* sutura,
                     constans FabricaIngressus* ingressus,
                     constans Xar* exclusa, Piscina* piscina,
                     Xar* particulae, chorda* causa_out);
    /* enumerare: loci quos ingressus legit aut custodit (-tacta);
     * NIHIL = ex particulis (plagula -> PLAGULA, 'dir/' ->
     * PLAGULAE sine suffixis) */
    b32 (*enumerare)(constans FabricaSutura* sutura,
                     constans FabricaIngressus* ingressus,
                     Piscina* piscina, Xar* loci,
                     chorda* causa_out);
    /* locare: loci quos exitus huius generis scribit; NIHIL =
     * genus ingressus solum */
    b32 (*locare)(constans FabricaExitus* exitus, Piscina* piscina,
                  Xar* loci);
    /* VERUM: octeti ex ingressibus determinati (regeneratio licet);
     * binarium FALSUM (LC_UUID, signatura - T6 1a) */
    b32 reproducibile;
};

structura FabricaStrategia {
    constans character* titulus;
    /* VERUM: artificium octetis comparatur (regeneratio) - '-tacta'
     * id solum iudicat; genus reproducibile postulat */
    b32 octetis_comparat;
    FabricaIudicium (*iudicare)(constans FabricaSutura* sutura,
                                constans FabricaActio* actio,
                                constans FabricaExitus* exitus,
                                constans Sigillum* ingressus,
                                b32 plenus, Piscina* piscina);
};

/* registra: NIHIL si titulus ignotus */
constans FabricaGenus*
fabrica_genus_invenire (
    chorda titulus);

constans FabricaStrategia*
fabrica_strategia_invenire (
    chorda titulus);

/* loci ingressuum actionis (enumerare omnium) - '-tacta' super eos */
b32
fabrica_actionem_enumerare (
    constans FabricaSutura* sutura,
     constans FabricaActio* actio,
                   Piscina* piscina,
                       Xar* loci,
                    chorda* causa_out);
```

- Header order: the two forward typedefs first; the bodies of
  `FabricaGenus` and `FabricaStrategia` after `FabricaSutura`,
  `FabricaActio` and `FabricaIudicium` (their pointers name them).
- Registry contents (names unchanged in declarations): types
  `fasciculus`, `configuratio`, `instrumentum` (one implementation:
  file bytes; three documentation names), `directorium`, `manifestum`,
  `plagulae`, `manifesta`, `radices`, and output type `binarium`
  (file bytes, `reproducibile` FALSUM). `fasciculus` and `binarium`
  implement `locare` (one PLAGULA); the others are input-only.
  Strategies `regeneratio` (memoria in front of it, gated by
  `memorabilis` exactly as today) and `relatio`.
- `exitus` with no `genus`: `relatio` → `binarium`, `regeneratio` →
  `fasciculus` (no declaration changes). The reader refuses a strategy
  with `octetis_comparat` on a type without `reproducibile`:
  "strategia regeneratio generi binarium non licet".
- `fabrica_ingressus_sigillare`, `fabrica_iudicare`,
  `fabrica_actio_tacta`, `fabrica_actionem_sigillare` keep their
  signatures. `fabrica_actio_tacta` is rewritten over
  `fabrica_actionem_enumerare` (no switch on a kind remains).

- [ ] **Step 1: the oracle, captured BEFORE any edit.**
  `cp bin/fabrica build/fabrica/vetus`; run
  `build/fabrica/vetus iudicare -plenus -omnia` twice (the first warms
  records) and keep the second as `scratchpad/ante.txt`; for every
  `titulus` in the declarations, `build/fabrica/vetus digestum T`
  → `scratchpad/digesta_ante.txt`. Record the credo assertion count of
  `./compile_tests.sh probatio_fabrica` (expected 166).
- [ ] **Step 2: failing tests.** New block "genera et strategiae":
  every registry title resolves, an unknown title → NIHIL; each input
  type has `sigillare`, `fasciculus`/`binarium` have `locare` giving
  one PLAGULA; `exitus genus="binarium" provenientia="regeneratio"` is
  refused with the message above; `fabrica_actionem_enumerare` on a
  `plagulae` input gives one PLAGULAE locus with its suffixes. Rewrite
  the helpers to take names (perl over the 47 enum uses; assertions
  untouched). Run → red (does not compile: the functions are missing).
- [ ] **Step 3: implement.** Each `_X_explicare` becomes the
  `sigillare` of its type (bodies unchanged); `_particulas_colligere`
  calls `ingressus->genus->sigillare`; `_relationem_iudicare` and the
  regeneration half of `fabrica_iudicare` become the two strategies'
  `iudicare`; `fabrica_iudicare` = sigillare once, then
  `exitus->strategia->iudicare`. Reader: names → registry pointers.
  `tools/fabrica.c` `-tacta` filter: `exitus->strategia->octetis_comparat`.
  Canon: `exitus` gains optional `genus` (options `fasciculus`,
  `binarium`). `grep -n 'commutatio\|FABRICA_INGRESSUS_\|FABRICA_PROVENIENTIA_'`
  over the three files → no kind switch left (the status switch in the
  CLI's counting stays: it is not a kind).
- [ ] **Step 4: green.** `./compile_tests.sh probatio_fabrica` →
  166 + the new assertions, 0 failures.
- [ ] **Step 5: two judges, one tree.** `./tools/fabrica_struere.sh`;
  run `bin/fabrica iudicare -plenus -omnia` → `scratchpad/post.txt`;
  run `build/fabrica/vetus` again on the SAME tree →
  `scratchpad/ante2.txt`. `diff ante2.txt post.txt` → EMPTY (same tree,
  old judge vs new). Same for every `digestum`. (Comparing against
  `ante.txt` would differ by design: lib/fabrica.c is an input of
  briar, briar_spectator and silex — those three lines turn STALUM.
  Record that in the worklog; T3 heals them.)
- [ ] **Step 6: plant.** Register `plagulae` with the `fasciculus`
  `sigillare`. Predict: the core block for plagulae red, and in Step
  5's comparison exactly the actions with a `genus="plagulae"` input
  differ (count them by `grep` first). Run, compare, restore.
- [ ] **Step 7: gates.** `./tools/fabrica_fumus.sh` 14/14,
  `./tools/fabrica_oraculum.sh` consensus. Worklog entry (what fit,
  what did not). Commit. Gates: radix, fabrica, fabrica-fumus (+ owed).

> **Executed 2026-10-01** (commit below). Fit cleanly: no kind switch
> left (status and locus-shape switches remain). Deviation:
> `FabricaStrategia.genus_ordinarium` (one home for the default output
> type); reader also refuses an input-only type as output. Oracle:
> old/new judge on one tree identical (145 verdicts, 64 digests) after
> a record-ordering artefact (worklog). Plant: 15 predicted actions +
> `fabrica` itself (its own closure, not predicted). 166 → 213.

### Task T2: composites, preconditions, `ignota`

**Opens with names (Fran):** PLACEHOLDERS — composite element
`compositum titulus="…"` with children `pars` (exactly one of
`artificium="via"`, `actio="titulus"` (all its outputs),
`compositum="titulus"`); precondition child of `actio`:
`praecondicio actio="titulus"`. Also: which composites to declare now
— recommendation `installata` (the 15 installer actions, by `actio`
parts) and `amalgamata` (the amalgam actions); `briar` waits for its
parts (T6).

**Files:** `include/fabrica.h`, `lib/fabrica.c`, `tools/fabrica.c`,
`aedificatio.canon`, root `aedificatio.stml`,
`probationes/probatio_fabrica.c`, `tools/fabrica_fumus.sh` (XV).

**Interfaces:**
- Consumes: T1's `FabricaStrategia`, `fabrica_strategia_invenire`.
- Produces:

```c
/* FabricaStrategia gains: */
    b32 iudicatur;   /* FALSUM: ignota - praecondicio solum, numquam
                      * iudicatur, numquam pars, numquam ingressus */

/* FabricaActio gains: */
    Xar* praecondiciones;   /* chorda: tituli actionum - ordo sine
                             * sigillo ('realiza ante me') */

nomen enumeratio {
    FABRICA_PARS_ARTIFICIUM = ZEPHYRUM,
    FABRICA_PARS_ACTIO,
    FABRICA_PARS_COMPOSITUM
} FabricaFormaPartis;

nomen structura {
    FabricaFormaPartis forma;
                chorda titulus;
} FabricaPars;

nomen structura {
    chorda titulus;
       Xar* partes;   /* FabricaPars */
    chorda sedes;
} FabricaCompositum;

/* composita plagulae (eadem dialectus ac actiones) */
Xar*
fabrica_composita_legere (
                 chorda  contentum,
     constans character* via,
                Piscina* piscina,
    InternamentumChorda* intern,
                 chorda* causa_out);

/* artificia compositi, plana, sine duplicibus (Xar de chorda).
 * NIHIL + causa "sedes: ...": pars ignota, cyclus (tituli nominati),
 * pars cuius strategia non iudicatur. */
Xar*
fabrica_compositum_explicare (
    constans Xar* composita,
    constans Xar* actiones,
           chorda titulus,
         Piscina* piscina,
          chorda* causa_out);

/* pessimum partium: RECENS < IGNOTUM < STALUM (NON_IUDICATUM inter
 * RECENS et IGNOTUM); causa nominat partes pessimas (III, deinde
 * "+N"). Xar de FabricaIudicium. */
FabricaIudicium
fabrica_iudicia_coniungere (
    constans Xar* iudicia,
           chorda titulus,
         Piscina* piscina);

/* praecondiciones: actio nominata exstat; exitus 'ignota' nusquam
 * ingressus aut pars. NIHIL-causa "sedes: ...". */
b32
fabrica_praecondiciones_probare (
    constans Xar* actiones,
         Piscina* piscina,
          chorda* causa_out);
```

- `fabrica_ordinare` treats a precondition as an edge. Strategy
  `ignota` registered with `iudicatur` FALSUM. CLI: outputs whose
  strategy is not judged are skipped (not counted, listed under
  `-omnia` as `PRAECONDICIO: titulus`); a positional argument that
  names a composite selects its artifacts and prints, after them,
  `COMPOSITUM titulus: STATUS (causa)`.

- [ ] **Step 1: failing tests.** Block "composita": worst-of over
  {RECENS, STALUM} → STALUM naming the stale part; {RECENS, IGNOTUM}
  → IGNOTUM; nested composite = flattened composite (associativity);
  duplicate part counted once; cycle refused naming both titles;
  unknown part refused with sedes; a part whose action's outputs are
  `ignota` refused. Block "praecondiciones": order puts the
  precondition first; unknown action refused; an `ignota` output named
  as another action's INPUT refused. Run → red.
- [ ] **Step 2: implement core** (reader, explicare, coniungere,
  probare, ordinare edge, `ignota`). Run → green.
- [ ] **Step 3: canon + CLI.** Canon: the two new elements (names from
  the opening), `provenientia` option `ignota`. CLI: composites read
  from every subsystem file, explained after all declarations load
  (parts cross subsystems); precondition check at load; selection and
  the `COMPOSITUM` line.
- [ ] **Step 4: declarations** (the composites agreed at the opening)
  in root `aedificatio.stml`. `bin/fabrica iudicare installata` →
  composite line agrees with the parts printed above it.
- [ ] **Step 5: fumus XV (born red).** Temp root: composite of two
  artifacts, one stale → exit 1 and `COMPOSITUM … STALUM (… stale
  part …)`; the other RECENS not named in the reason. Write the stage,
  run against the T1 binary → red (selection unknown); then green.
- [ ] **Step 6: plant.** Break `fabrica_iudicia_coniungere`'s order
  (IGNOTUM above STALUM). Predict: two core assertions red (which) and
  XV still green (it has no IGNOTUM part). Run, restore.
- [ ] **Step 7: commit.** Worklog. Gates: radix, fabrica,
  fabrica-fumus, examen-canon if owed.

> **Executed 2026-10-01** (commit below). Names as placeholdered
> (Fran). Composites `installata` (15) and `amalgamata` (9) declared;
> every composite explained at load (broken = exit 2). Precondition
> edge general (order without digest), `ignota` reachable only by it.
> XV red against the pre-composite judge; plant exact (4 assertions).
> 213 → 259.

### Task T3: `sanare` — the executor and its envelope

**Files:** `include/fabrica.h`, `lib/fabrica.c` (`fabrica_sanare`),
`tools/fabrica.c` (verb, `agere` seam, log, deadline, self-check),
`probationes/probatio_fabrica.c` (scripted `agere` in `DiscusFictus`),
`tools/fabrica_fumus.sh` (XVI–XX).

**Interfaces:**
- Consumes: T1 registries; T2 `fabrica_compositum_explicare`,
  preconditions; 1a `fabrica_ordinare`, `fabrica_iudicare`.
- Produces:

```c
nomen structura {
       s32 codex;        /* exitus processus; -1 = non incepit aut
                          * terminus excessus */
       i32 duratio_ms;
    chorda cauda;        /* ultimae lineae (erratum, aliter effusio) */
} FabricaActum;

/* FabricaSutura gains: */
    /* mandatum actionis IN LOCO currere (FABRICA_SCRIPTURA nulla),
     * effusionem in acta_via scribere. NIHIL licet (sanare tum solum
     * siccum potest). */
    b32 (*agere)(vacuum* datum, constans FabricaActio* actio,
                 constans character* acta_via, Piscina* piscina,
                 FabricaActum* actum_out);

nomen enumeratio {
    FABRICA_SANATUM = ZEPHYRUM, /* actum, exitus RECENS post */
    FABRICA_PRAEPARATUM,        /* praecondicio acta, exitus 0 */
    FABRICA_FRACTUM,            /* codex != 0, terminus, aut post-
                                 * condicio non RECENS */
    FABRICA_OMISSUM,            /* dependentia fracta aut omissa */
    FABRICA_AGENDUM,            /* siccum: stalum/ignotum nunc */
    FABRICA_FORTASSE            /* siccum: post actionem agendam */
} FabricaEventus;

nomen structura {
    constans FabricaActio* actio;
            FabricaEventus eventus;
                    chorda causa;
                       i32 duratio_ms;
} FabricaSanatio;

/* Sanare. 'electa': viae artificiorum (Xar de chorda; NIHIL =
 * omnia); actiones earum et omnes supra eas (ingressus, praecondiciones)
 * in ambitu. Per ordinem: exitus actionis NUNC iudicantur (plenus); omnes
 * RECENS -> nihil; dependentia FRACTUM/OMISSUM -> OMISSUM;
 * aliter praecondiciones (semel per cursum), agere, memoriae per
 * cursum (sigilla, digesta, regenerationes) VACANTUR, exitus iterum
 * iudicantur -> SANATUM aut FRACTUM. siccum: nihil agitur. Acta:
 * build/fabrica/acta/TITULUS.log. Xar de FabricaSanatio; NIHIL +
 * causa si nihil in ambitu. */
Xar*
fabrica_sanare (
    constans FabricaSutura* sutura,
              constans Xar* ordo,      /* FabricaActio*, ordinatae */
              constans Xar* electa,
                       b32  siccum,
                   Piscina* piscina,
                    chorda* causa_out);
```

- CLI: `bin/fabrica sanare [-siccum] [artificium|compositum…]`. Order:
  self-check (bin/fabrica judged first; STALUM/IGNOTUM → exit 2
  "iudex ipse stalus: ./tools/fabrica_struere.sh prius"), lock
  `build/fabrica/sera` (shared with `-plenus`), `fabrica_sanare`,
  per action a line before (`[i/n] titulus …`) and after (eventus,
  seconds, on FRACTUM the last 20 log lines), summary `fabrica sanare:
  N sanata, N fracta, N omissa (T s)`, and — when any `regeneratio`
  action ran — `plagulae commissae mutatae: committe per
  silva.commissio`. Deadline `MORA_SANATIONIS_MS` 1 800 000 (one
  constant; per-action deadlines are not in 1b). Log = effusio then
  erratum, each under a header line (processus captures them apart).

- [ ] **Step 1: failing tests** on the in-memory disk, scripted `agere`
  (per action title: files to write, exit code): healed (A stale →
  SANATUM, re-judged RECENS); **memo purge** (A writes X, B consumes X
  and was RECENS before A ran → B judged on the new X, SANATUM — red
  if memos are kept, Review Focus 2); **post-condition** (A exits 0,
  writes nothing → FRACTUM "exitus 0 sed non RECENS", Review Focus
  3); **dependency** (A fails → B downstream OMISSUM naming A, C
  independent SANATUM, Review Focus 4); precondition run once before
  two dependants; `electa` limits scope to the target and what is
  above it; siccum: AGENDUM / FORTASSE, `agere` call count 0.
- [ ] **Step 2: implement `fabrica_sanare`.** Run → green.
- [ ] **Step 3: the tool's `agere`.** `processus_exsequi` with the
  deadline; log file; `FabricaActum`; `FABRICA_SCRIPTURA` explicitly
  unset. `mkdir -p build/fabrica/acta`.
- [ ] **Step 4: the verb** as specified above; usage line updated.
- [ ] **Step 5: measure post-condition cost** on the real tree:
  `bin/fabrica sanare -siccum` (time); then make one generated output
  stale by editing the OUTPUT (append a blank line to the
  `entitates_html` action's committed output — not a widely-included
  header), `bin/fabrica sanare <that path>`: time the run and the
  re-judge share, record in worklog. The run itself restores the
  file: `git diff` on it → empty (a self-checking plant).
- [ ] **Step 6: fumus XVI–XX** (born red: each stage run first
  against `build/fabrica/vetus` = the T2 binary, which has no `sanare`
  verb → exit 2 ≠ expected; then against the new one): XVI healed (temp root: generator script
  writes the output; stale → `sanare` → exit 0, `iudicare` → RECENS);
  XVII exit 0 but not RECENS → exit 1, FRACTUM; XVIII failing upstream
  → downstream OMISSUM, exit 1; XIX lock held (python flock as XII) →
  exit 2 naming the lock; XX judge stale (temp root declares
  `bin/fabrica` with a relatio that does not match) → exit 2 naming
  `fabrica_struere.sh`, `agere` never ran (a marker file absent).
- [ ] **Step 7: dogfood — the oracle of spec §VI.** On the real tree
  (briar, briar_spectator, silex STALUM since T1): `bin/fabrica sanare
  -siccum` lists exactly them (+ anything else stale: show Fran);
  `bin/fabrica sanare` → SANATUM ×3; `bin/fabrica iudicare` → all
  RECENS; for each, `bin/X -provenientia` ingressus ==
  `bin/fabrica digestum X`. Time it; worklog.
- [ ] **Step 8: plant.** Remove the memo purge. Predict: the memo
  block red, nothing else. Restore. Commit. Gates: radix, fabrica,
  fabrica-fumus (+ owed).

> **Executed 2026-10-01** (commit below). As specified; the memo test
> models two real stale-memo paths (self-input generator feeding a
> binary; post-condition reusing the pre-run regeneration). Dogfood:
> briar/spectator/silex healed (5 min), relatio == digestum ×3. Fumus
> XVI–XX red against the T2 judge. Plant exact (4). 259 → 299.

### Task T4: footprints and the envelope's check

**Opens with a measurement, then names (Fran):** PLACEHOLDERS — owned
workspace element `vestigium via="…" forma="arbor|plagulae"
suffixa="…"` (child of `actio`); shared cache attribute
`communis="verum"` on it. The footprint of an action = `locare` of its
outputs ∪ its `vestigium` entries ∪ the envelope's own
(`build/fabrica/acta/TITULUS.log`, `build/fabrica/provenientia/TITULUS.c`,
`build/fabrica/scriptura/TITULUS/`).

**Files:** `include/fabrica.h`, `lib/fabrica.c`, `tools/fabrica.c`
(`vestigium_capere`), `aedificatio.canon`, every `aedificatio.stml`,
`probationes/probatio_fabrica.c`, `tools/fabrica_fumus.sh` (XXI–XXII).

**Interfaces:**
- Consumes: T1 `FabricaLocus`, `locare`; T3 `fabrica_sanare`, `agere`.
- Produces:

```c
nomen structura {
    chorda via;
       s64 tempus_ns;   /* mtime */
       s64 mensura;
} FabricaVestigium;

/* FabricaSutura gains: */
    /* arbor tota (sine .git) + loci exituum extra arborem; Xar de
     * FabricaVestigium ordinata per viam. NIHIL licet (sine
     * probatione vestigii). */
    b32 (*vestigium_capere)(vacuum* datum, Piscina* piscina,
                            Xar** vestigia_out);

/* FabricaActio gains: */
    Xar* vestigia;   /* FabricaLocus (opera propria) */
    Xar* communia;   /* FabricaLocus (cache communis idempotens) */

/* viae novae, deletae aut mutatae (tempus aut mensura) inter ante et
 * post EXTRA vestigium actionis (Xar de chorda) */
Xar*
fabrica_vestigia_comparare (
    constans FabricaSutura* sutura,
     constans FabricaActio* actio,
              constans Xar* ante,
              constans Xar* post,
                   Piscina* piscina);

/* undae: actiones quae simul currere POSSENT (nulla via inter eas,
 * vestigia disiuncta, nulla communia) - Xar de Xar de FabricaActio*.
 * Monstratur solum (Q41). */
Xar*
fabrica_undas_formare (
    constans FabricaSutura* sutura,
              constans Xar* actiones,
                   Piscina* piscina);
```

- `fabrica_sanare`: snapshot before and after each `agere`; any path
  outside the footprint → FRACTUM "scripsit extra vestigium: VIA (+N)
  — aut manu mutata dum currebat". `-siccum` prints the waves.

- [ ] **Step 1: measure the write sets.** Snapshot cost on the real
  tree (29 330 files outside `.git`, walk 0.08 s measured 2026-09-30;
  with `stat`: measure). Then for every declared action, snapshot →
  run its mandatum by hand → snapshot → diff (a throwaway script in
  scratchpad). Branches per action: (a) writes ⊆ outputs + envelope
  → nothing to declare; (b) writes into a workspace of its own →
  `vestigium`; (c) writes into `build/aedilis/obiecta/` →
  `communis`; (d) writes a COMMITTED or another action's file →
  a defect: list it for T5 (expected: `mensor_ui` →
  `apps/mensor/assets/capsula_mensor.c`; snippets → `bin/aedilis`
  if the T6-1a guard leaks). Show Fran the table and the names.
- [ ] **Step 2: failing tests.** Comparison: write inside output →
  none; inside a declared `arbor` → none; a new file elsewhere →
  named; a deleted file → named; an identical rewrite (same bytes, new
  mtime) → named. Waves: two independent disjoint actions → one wave;
  an edge → two waves; a shared `communia` → two waves. Run → red.
- [ ] **Step 3: implement** core + tool (`stat` mtime in ns via
  `filum` — measure what it offers; add `filum_tempus_ns` if absent,
  with its own test) + canon + reader. Green.
- [ ] **Step 4: declarations** from Step 1's table.
- [ ] **Step 5: born red on a real defect.** Plant a comment in
  `apps/mensor/mensor_ui.c`; predict: `bin/fabrica sanare
  bin/mensor_ui` → FRACTUM naming `apps/mensor/assets/capsula_mensor.c`
  (rewritten even when its bytes are identical). Run. Restore the
  plant; leave the defect for T5.
- [ ] **Step 6: fumus XXI** (temp root: an action that also writes
  `alia.txt` → FRACTUM naming it) and **XXII** (`-siccum` prints two
  waves for two independent actions). Born red, then green.
- [ ] **Step 7: commit.** Worklog. Gates: radix, fabrica,
  fabrica-fumus, examen-canon if owed.

> **Executed 2026-10-01** (commit below). Step 1 measured all 64
> actions (snapshot 0.45 s; table in lib/fabrica.worklog.md): 52
> footprints declared; SIX defect classes, not one (D1 mensor_ui
> capsula; D2 canon/natura sibling relinks; D3 generators relink
> bin/aedilis in place; D4 amalgama rewrites excludenda's output; D5
> transient in-place edit of silva_latina_datum.{c,h}; D6 tabulae_silvae
> touches silva.h, hospes.c). Fran 2026-10-01: names as placeholdered;
> D1–D6 fixed in T5; D2 by MERGING each family into one action
> (same-type multi-output). Born red live on D1. Envelope gained the
> executor's own state (fabrica.db{,-wal,-shm}, sera) - found by the
> live plant, excluded from wave conflicts. Deviation: comparare and
> undae take no sutura. 299 → 317.

### Task T5: installers — preconditions, link/copy split, fixes

**Inherited from T4 (Fran 2026-10-01):** fix defects D1–D6 (T4
executed note): D2 by merging `canon_examen`+`canon_coquere` and the
four `natura_*` into one action each (several `binarium` outputs, one
script); D3 generators never build bin/aedilis (precondition edge to
`aedilis`); D4 amalgamare stops regenerating excludenda (declared edge
instead); D5 the latina_datum transient edit moves to a scratch copy;
D6 measure silva.h/hospes.c - declare as outputs or stop the touch.
Done = `bin/fabrica sanare` over every action with NO "extra
vestigium" (the T4 measurement script re-run: every write inside a
footprint).
Also found by the first live `sanare installata` after T4 (cold paths
the warm measurement missed): briar's corpus regeneration writes
gitignored ROOT files `corpus.symbola.tsv`, `corpus.versio`,
`corpus_silicis.toml` (+4) - they belong to the corpus action (T6);
silex links `silva/build/*.o` - `silva/build` communis for silex.

> **Part A1 executed 2026-10-01** (commit below): ordering edges from
> `enumerare` (manifest closures) - one bootstrap cycle, broken by
> `instrumentum` enumerating nothing (Fran); D1; D2 (families merged;
> whole provenance directory excluded from digests). D3–D6 found
> larger than planned: D5 and D6 are undeclared GENERATED outputs
> (latina_datum by the amalgamator; spliced tables in silva.h and
> hospes.c), both also written in place under the judge.

> **Part A2 executed 2026-10-01** (commit below): FABRICA_AGIT (D3/D4);
> D5 = own action `latina_datum` (amalgamator only reads); D6 = splice
> in scratch, silva.h/hospes.c declared; mechanism objects as `ignota`
> preconditions (`amalgamare.sh -obiecta`). Fumus XXIII.

**Opens with a measurement and two decisions (Fran):**
1. Object stores each installer links blind (expected from §0.3 and
   `briar_struere.sh`: root `build/*.o` for silex, stml, briar,
   briar_spectator, compile_tools; `briar/build/*.o` for briar).
   Each store becomes ONE `ignota` action reached only by precondition
   edges (Q38 rule, applied to every such store). A store not listed
   here → show Fran.
2. Tools installers invoke (`bin/aedilis`, `bin/fabrica`,
   `bin/capsula_generare`): precondition edges (order, no digest —
   their bytes are not reproducible, so a digest edge would mark every
   binary STALUM on each relink).
3. **DECISION — the session hook and copies:** under celer a
   regeneratio output with no record is NON_IUDICATUM, so the hook
   would NOT see `~/.bin/briar` lag (spec Q42's reason). Options:
   (a) an action attribute PLACEHOLDER `celer="verum"` — regeneration
   cheap enough to run under celer (four copies: measure, expected
   < 0.5 s against the hook's 15 s); (b) accept NON_IUDICATUM in the
   hook. Recommendation: (a) — generic (any cheap generator may opt
   in), and it is the reason the copies were split.
4. The objects-only flag name for `compile_tests.sh` (PLACEHOLDER
   `--obiecta`).

**Files:** `compile_tests.sh`, `compile_tools.sh`,
`tools/{briar,briar_spectator,stml,silex}_struere.sh`,
`tools/instituere.sh` (NEW), `tools/mensor_ui_struere.sh`,
`tools/fabrica.c` (`~` expansion in the seam), `lib/fabrica.c` (`celer`
if (a)), `aedificatio.canon`, root `aedificatio.stml`,
`probationes/probatio_fabrica.c`, `tools/fabrica_fumus.sh` (XXIII).

**Interfaces:**
- Consumes: T2 preconditions, `ignota`, composites; T3 `sanare`; T4
  footprint check.
- Produces: `tools/instituere.sh BINARIUM` — `mkdir -p "$HOME/.bin"`,
  `rm -f` then `cp … || exit 1` (macOS vnode signature: new inode),
  under `FABRICA_SCRIPTURA` writes `$FABRICA_SCRIPTURA/BINARIUM`
  instead; actions `institutio_X` (output `~/.bin/X`, `regeneratio`,
  `memorabilis`, `scriptura="X"`, input `bin/X`) for the four; the
  link actions keep their titles and `relatio`; `obiecta_radicis`
  (and `obiecta_briar` if measured) with `ignota` outputs; composite
  `installata` gains the copies. `FabricaActio` gains `b32 celer` if
  (a).

- [ ] **Step 1: failing tests.** Core: an exitus `~/.bin/x` is read
  through the seam as `$HOME/.bin/x` (the in-memory seam records the
  path it was asked for); absent → STALUM "artificium absens";
  (a) a `celer` action regenerates under celer, a plain one stays
  NON_IUDICATUM. Run → red.
- [ ] **Step 2: `compile_tests.sh` objects-only mode** (exit 0 when the
  library objects are built, 1 on failure; no filter matching nothing,
  no exit 2); `compile_tools.sh` uses it and checks the code.
- [ ] **Step 3: the split.** The four `_struere.sh` stop copying
  (their last line names `bin/fabrica sanare X` / `tools/instituere.sh`
  for humans); `tools/instituere.sh`; declarations: copies,
  preconditions, tool edges, `installata`.
- [ ] **Step 4: `mensor_ui` (Q44).** Remove the capsula regeneration;
  confirm `capsula_mensor` is upstream of `mensor_ui` in `fabrica
  ordinare` (its output is in mensor_ui's manifest closure — measure;
  if not, declare the input). The T4 Step 5 plant now → SANATUM.
- [ ] **Step 5: implement** `~` expansion (tool seam: `legere`,
  `locare` consumers, snapshot) and `celer` if chosen. Green.
- [ ] **Step 6: fumus XXIII** (`HOME` = scratch dir, Review Focus 5):
  a copy action with `~/.bin` absent → STALUM; `sanare` → `~/.bin`
  created, SANATUM; `iudicare` (celer if (a)) → RECENS; then `bin/x`
  changed → STALUM (celer if (a)). The real `~/.bin` untouched
  (digest of `~/.bin/briar` before/after equal). Born red.
- [ ] **Step 7: live.** `bin/fabrica sanare -siccum` (expected: the
  four links STALUM — their inputs changed with the split — and the
  four copies); `bin/fabrica sanare installata` → healed; plant: make
  `tools/instituere.sh`'s `cp` target unwritable in a scratch `HOME`
  → FRACTUM by exit code (the masked failure of §0.6 can no longer
  exit 0). `iudicare` → all RECENS; hook silent.
- [ ] **Step 8: commit.** MEMORY: humans now "rebake" with `bin/fabrica
  sanare installata` (or a title). Worklog. Gates: radix, fabrica,
  fabrica-fumus, compile_tools/briar/silex suites as owed.

> **Executed 2026-10-01** in five commits (A1 d8327b36, A2 4da395a9,
> B1 507f0ace, B2 below). Beyond the plan: ordering edges by places
> (manifest closures) and the one bootstrap cycle broken by
> `instrumentum`; families merged with the whole provenance directory
> excluded from digests; D5/D6 were undeclared GENERATED outputs, not
> transient edits; mechanism objects as `ignota` preconditions too.
> Names (Fran): `--obiecta` (`-obiecta` for amalgamare.sh), `celer`,
> FABRICA_AGIT. Done check: 70/70 actions, 0 writes outside a footprint
> (warm). 317 → 337 assertions; fumus XXIII–XXIV.

### Task T6: briar decomposed

**Opens with a measurement and a decision (Fran):**
`build/capsula_corpus_silicis.c` carries a stamp (`commit=… dies=…`,
`tools/corpus_infixum.sh:77-89`) and regenerates by mtime — it is NOT
byte-reproducible, so `regeneratio` cannot judge it. Options:
(a) move the stamp out of the capsule into the link step (a file
generated at link time, as `build/briar_aedificatio.c` already is) —
the capsule becomes reproducible, `briar -versio`/`silex -versio` keep
printing a stamp (now the link's), provenance already carries the
commit; (b) keep the stamp; judge the capsule only through the link
(not decomposed). Recommendation: (a). Measure the facies, icon and
mutationes capsules for stamps too (expected: none).

**Files:** `tools/corpus_infixum.sh`, `tools/briar_facies_capsula.sh`,
`tools/briar_icon_capsula.sh`, `tools/briar_mutationes_capsula.sh`
(scratch mode, one output each, no mtime skip under
`FABRICA_SCRIPTURA`), `tools/briar_struere.sh`, `tools/silex_struere.sh`
(link only: consume the capsules, no nested producers), root
`aedificatio.stml`, `tools/fabrica_fumus.sh` only if a new behaviour
appears (none expected).

**Interfaces:**
- Consumes: everything above.
- Produces: actions `corpus_silicis` (shared by briar and silex),
  `capsula_facies_briar`, `capsula_icon_briar`,
  `capsula_mutationes_briar` (outputs in `build/`, `regeneratio`,
  `memorabilis` where inputs are complete); `briar` link takes the
  four capsules as inputs (edges) instead of the corpus's `plagulae`;
  composites `briar` (four capsules + link + copy) and `silex`.

- [ ] **Step 1: scratch mode** in the four capsule scripts; for each,
  `cmp` the scratch output against `build/…` → equal (after (a) for
  the corpus).
- [ ] **Step 2: declarations**; `bin/fabrica iudicare -plenus briar` →
  the composite line; link and copy STALUM (inputs changed shape).
- [ ] **Step 3: the win, measured.** `time bin/fabrica sanare briar`
  (cold); then edit `briar/MUTATIONES.md` only → predict: mutationes
  capsule + link + copy run, corpus/facies/icon do not; time it
  against today's full `briar_struere.sh`. Restore and heal.
- [ ] **Step 4: plant.** A comment in a `lib/*.c` → predict: corpus
  STALUM → `sanare briar silex` runs corpus once, both links, both
  copies. Restore and heal.
- [ ] **Step 5: commit.** Tester-visible change (`-versio` source of the
  stamp): one line in `briar/MUTATIONES.md` under `## inedita`.
  Worklog. Gates: radix, fabrica, fabrica-fumus, briar, silex (+ owed).

> **Executed 2026-10-01** (commit below). Option a' (Fran): the stamp
> was functional (briar cache key, silex corpus title) - now a content
> SIGILLUM, reproducible and a better key. Shadow root
> (tools/capsula_radicis.sh) lets root-glob capsules regenerate in
> scratch. Corpus deterministic, ~10 s. T4 snapshot bug fixed (it
> followed symlinked dirs). MUTATIONES edit -> 3 actions (7 s); lib
> edit -> corpus once + both links + copies. Judge cost (amalgam chain
> regenerated, not memorabilis) now dominates: 2-3 min per `sanare
> briar`.

### Task T7: `cursus` records, estimates, closing

**Files:** `tools/fabrica.c` (table, seam members, `-siccum`
estimates), `include/fabrica.h` (seam members), `lib/fabrica.c`
(`fabrica_sanare` calls them), `tools/fabrica_fumus.sh` (XXIV),
`project-specs/fabrica-spec-1b.md` ("As built"), ledger, MEMORY.

**Interfaces:**
- Produces: table `cursus(titulus TEXT, initium INTEGER, duratio_ms
  INTEGER, codex INTEGER, eventus TEXT, causa TEXT)` in
  `build/fabrica.db`; seam members

```c
    /* post quodque actum (non siccum). NIHIL licet. */
    vacuum (*cursum_inscribere)(vacuum* datum,
                                constans FabricaSanatio* sanatio,
                                s64 initium);
    /* duratio ultimi cursus SANATI; FALSUM = nullus. NIHIL licet. */
    b32 (*cursum_legere)(vacuum* datum, constans character* titulus,
                         i32* duratio_ms_out);
```

- `-siccum` prints `~T s` per action (or `tempus ignotum`) and the
  total.

- [ ] **Step 1: measure** how `tools/fabrica.c` creates
  `verificationes` (and whether `scrinium` has a schema version);
  branch: (a) `CREATE TABLE IF NOT EXISTS` suffices for an added table
  → use it; (b) a version mechanism exists → bump it.
- [ ] **Step 2: failing test** (core: `fabrica_sanare` calls
  `cursum_inscribere` once per action that ran, never in siccum) and
  fumus XXIV (temp root: after one `sanare`, `-siccum` on a re-staled
  action prints a time, not `tempus ignotum`). Red, implement, green.
- [ ] **Step 3: fill the timing lens.** From `cursus`, the producers
  inventory's duration lens (41 of 70 blank on 09-29) for every action
  that has run; the rest stay blank (no forced runs).
- [ ] **Step 4: close.** Spec 1b "As built" (numbers of §VII, before →
  after; deviations); MEMORY `fabrica-project.md`; worklog; ledger
  note on the park. Commit. Gates: radix, fabrica, fabrica-fumus.

> **Executed 2026-10-01** (commit below). Migration II (branch b);
> no `codex` column (the causa carries "exitus N"); estimates need
> `tempus_notum`. Found: empty causa bound as SQL NULL by scrinium -
> fixed locally, library question to Fran (…BVN7). Timing lens filled
> from T5's warm measurement (20 rows). Spec par. X "As built". PLAN 1b
> COMPLETE.

## Not in 1b (stated)

Parallel execution (Q41: after the footprint check has proven itself,
and near `build/aedilis/obiecta/` after slice 2's content addressing);
launchers and the operate verb (Q39); verdicts as artifacts and a flake
policy (slice 3); `compile_tests.sh --clean` deleting `bin/*` (the
footprint check would flag it if an action ran it; no action does);
read auditing (`fs_usage`, spec §II.7.1); generating declarations
(slice 4).
