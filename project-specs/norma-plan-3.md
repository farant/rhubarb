# norma plan 3 - the STML face (.norma) and the C it compiles to

> **For agentic workers:** execution mode FIXED by Fran's standing rule:
> inline, one task per turn, Fran approves each; no subagents.

**Goal:** `.norma` files (STML) read to the same `Norma*` the C builders
make, write back byte for byte, and compile to generated C that agrees
with both; vates' response schema moves into `lib/vates_responsum.norma`.

**Architecture:** the core `norma` gains one additive field (generator
NAME). `norma_stml` reads and writes the dialect, with `norma.canon`
embedded as a generated C array. `norma_ad_c` turns named normae into
`.h` + `.c` text; `bin/norma c` runs it as fabrica's generator. Five
oracles, each born red by a plant that compiles.

**Spec:** `project-specs/norma-spec-2.md` (cd963098). Headers (A0) are
the contract once Fran approves them.

## Global constraints

C89 + latina.h (`tools/vexilla.sh`); Latin identifiers, NO latina.h macro
word as an identifier (single capitals I V X L C D M are macros - never a
variable named `L`); no `_Capital`; `i32`/`i64` UNSIGNED, `s32`/`s64`
signed; `chorda` not NUL-terminated; compile-check every TU (examen
misses macro/enum clashes); worktree commit guard (absolute path +
`assert silva.__file__`); **every commissio in quarta passes
`sine_debitis='<cause>'`** (else the cold ledger path seeds
`tabularium.jsonl`, …VSY50E) and names gates by hand: `radix`,
`generata`, plus what main's `./tools/portae_debitae.sh <viae>` names
(run it FROM MAIN, read-only); new `lib/*.c` ->
`./tools/compile_tests_fontes_generare.sh` in the same commit; generated
files carry `GENERATUM` on line 1 and a fabrica generator action; every
behaviour born red by a plant that compiles, restored with plain `cp`.
Step 0 of A1: merge main into quarta (main moved 64 commits since
673d8111).

## Rulings made while planning (spec freedom; each costs little if wrong)

- **R1 - canon embedding: generated array, not capsula.** capsula writes
  its header beside its TOML (outside `include/`, so aedilis's
  `include/X.h -> lib/X.c` rule cannot find it) and its runtime pulls
  `flatura`, `via`, `iter_directoria` for one ~5 KB file.
  `tools/norma_canon_infigere.sh` writes `lib/norma_canon.c` (GENERATUM)
  behind a hand-written `include/norma_canon.h`. Cost if wrong: swap the
  generator; the function stays.
- **R2 - canon judges references and duplicate names.** canon already
  has key/keyref (`<citatio>`) and `<unicitas>`; `<ad norma="#nemo"/>`
  is `CANON_CITATIO_IRRITA`, a second `#x` is `CANON_NOMEN_BIS`. The
  loader keeps a defensive check. Also canon judges modifiers on the
  wrong type (`forma` on `integer` = `CANON_ATTRIBUTUM_IGNOTUM`), so the
  spec's "modifier on wrong type" class needs no loader code.
- **R3 - bounds come in pairs.** The core has no one-sided bound
  (`norma_intra(n, min, max)`); filling a missing side with the type's
  extreme would make FINES generate 4-billion-rune strings. A lone
  `minimum` (or `maximum`, `longitudo_minima`, `longitudo_maxima`) is the
  loader error `NORMA_STML_FINIS_DIMIDIATUS`. The spec's sketch
  `<integer minimum="0"/>` is therefore refused. vates' schema has no
  bounds. A later additive core change can relax this. **Flagged for
  Fran at A0.**
- **R4 - `NormaStmlVitium` carries a `causa` enum** (spec sketch had
  only line/column/message): oracle 4's coverage bitmask needs it.
- **R5 - fixtures are in WRITER form.** Measured 2026-10-08 (throwaway
  program): the parser turns `(>` captures into ordinary children
  (`campus liberi=1`); `stml_scribere(..., VERUM)` reproduces a
  formatted file byte for byte; an API-built tree comes out in capture
  form too. So oracle 1 compares against fixtures committed exactly as
  the writer emits them, and oracle 5 (formatter lint) is the in-test
  check `stml_scribere(stml_legere(x).radix, VERUM) == x`.
- **R6 - attribute text the writer cannot carry is refused.** STML
  writes attribute values RAW (`lib/stml.c` `_attributa_scribere`, no
  escaping); a key containing `"`, `&`, `<`, `>` or a newline would
  break the file. The writer refuses such keys / tags / discriminator
  keys / forms; enum values with those characters go to `<licitum>`
  children (text IS escaped). Note: STML does not print the value of an
  attribute equal to `true` (`titulus` alone) and reads it back as
  `true` - round trip intact, recorded in the worklog.
- **R7 - generated C uses one array `nodi[]` and decimal literals.**
  The only generated identifiers are `<praefixum><titulus>`,
  `<praefixum>gignens_<titulus>` and `_struere_<titulus>`; the identifier
  lint has no GENERATUM exemption (`oratio/vocabula.sh`), so titles must
  be lint-clean words like any C name. Strings > 509 bytes (C89 limit,
  `-Woverlength-strings`) and non-finite floats are refused.
- **R8 - `bin/norma` is built by aedilis** (`./bin/aedilis tools/norma.c`
  then its `struere.sh`), through `tools/norma_struere.sh`; the
  generator action lists `build/aedilis/norma/manifestum.stml` as input,
  like `lectores_cocti`.

## Review focus

1. **A key STML cannot carry** (`a"b`, `x&y`, newline): writer refuses
   with a named reason, never emits a broken file (A4 test).
2. **One-sided bound** (`<integer minimum="0"/>`): loader error
   `FINIS_DIMIDIATUS` with line:column, not a silent extreme (A3 test).
3. **Float bounds that must survive text** (0.1, 0.3, -1e-300): writer's
   shortest round-trip form reads back to the same f64; emitter's literal
   compiles to the same f64 (A4, A5 tests).
4. **A reference cycle** `#a -> #b -> #a`: loader error `CIRCULUS` naming
   the chain, no infinite recursion (A3 test); a builder-made cycle makes
   the writer refuse at depth CXXVIII (A4 test).
5. **A shared node referenced from two places**: loader returns ONE node
   (pointer-equal), writer writes `<ad>`, emitter builds it once per call
   (A3, A4, A5 tests).

---

### Task A0: headers to Fran

**Files:** Create `include/norma_stml.h`, `include/norma_ad_c.h`,
`include/norma_canon.h`; modify `include/norma.h` (additive). No `.c`.
Each header compile-checked in a scratch TU; no commit until Fran
approves (then one commit, docs + headers; gates `radix`).

- [ ] **A0.1** `include/norma.h` - in `NormaVisus` after `gignens_datum`:

```c
          chorda  gignens_titulus;  /* vacua si nullus (norma-spec-2) */
```

  and after `norma_gignens`:

```c
/* titulus functionis gignentis (norma-spec-2 §III.1): .norma eum
 * scribit, norma_ad_c symbolum ex eo facit. Functio NIHIL licet -
 * generator tunc genus suum ordinarium gignit. */
Norma*
norma_gignens_titulus (
                 Norma* n,
    constans character* titulus);
```

- [ ] **A0.2** `include/norma_canon.h`:

```c
/* norma_canon.h - textus norma.canon infixus (norma-spec-2; regula R1
 * norma-plan-3). Corpus lib/norma_canon.c GENERATUM est per
 * tools/norma_canon_infigere.sh - numquam manu. */
#ifndef NORMA_CANON_H
#define NORMA_CANON_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"

/* copia textus canonis in piscina */
chorda
norma_canon_textus (
    Piscina* piscina);

#endif /* NORMA_CANON_H */
```

- [ ] **A0.3** `include/norma_stml.h`:

```c
/* norma_stml.h - facies STML normae: plagulae .norma legere et
 * scribere (norma-spec-2 §II-§III)
 *
 * Dialectus in specificatione; canon norma.canon (infixus). Canon
 * PRIMUS iudicat: vocabularium, cardinalitas, genera attributorum,
 * citationes '#x', tituli unici, modificatores in genere alieno.
 * Lector deinde sensum: typus filius unus, fines (bini, parsabiles,
 * non inversi), electio, circuli, gignentes.
 *
 * STATUS: PROPOSITUM (norma-plan-3 A0).
 *
 * USUS:
 *   NormaStmlLectio l = norma_stml_legere(fons, NIHIL, 0, p);
 *   si (!l.successus) { ... l.vitia: NormaStmlVitium ... }
 *   Norma* r = norma_stml_quaerere(&l, "responsum");
 *   chorda s = norma_stml_scribere(normae, numerus, p, &causa);
 */
#ifndef NORMA_STML_H
#define NORMA_STML_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "norma.h"

/* registrum: gignens="titulus" -> functio */
nomen structura {
    constans character* titulus;
           NormaGignens functio;
                vacuum* datum;
} NormaGignensNominatum;

nomen enumeratio {
    NORMA_STML_FRACTUM = 0,          /* STML non parsabile */
    NORMA_STML_CANON,                /* vitium canonis (nuntius nominat) */
    NORMA_STML_LIBERI_TYPI,          /* typus filius non unus */
    NORMA_STML_FINIS_PRAVUS,         /* finis non parsabilis aut extra genus */
    NORMA_STML_FINIS_DIMIDIATUS,     /* minimum sine maximo aut contra */
    NORMA_STML_FINES_INVERSI,        /* minimum > maximum */
    NORMA_STML_ELECTIO_DUPLEX,       /* electio= et <licitum> simul */
    NORMA_STML_ELECTIO_VACUA,        /* electio sine valore */
    NORMA_STML_CIRCULUS,             /* catena <ad> in se redit */
    NORMA_STML_GIGNENS_IGNOTUM,      /* registro dato, titulus abest */
    NORMA_STML_GIGNENS_SINE_REGISTRO /* NOTA: titulus servatur */
} NormaStmlCausa;

nomen structura {
               i32  linea;     /* 1-basata; 0 = ignota */
               i32  columna;   /* 1-basata, octeti; 0 = ignota */
    NormaStmlCausa  causa;
            chorda  nuntius;
} NormaStmlVitium;

nomen structura {
    chorda  titulus;   /* sine '#' */
    Norma*  norma;
} NormaNominata;

nomen structura {
     b32  successus;
     Xar* normae;   /* NormaNominata, ordine documenti; vacuum si !successus */
     Xar* vitia;    /* NormaStmlVitium: canonis primum, deinde lectoris */
     Xar* notae;    /* NormaStmlVitium: GIGNENS_SINE_REGISTRO */
} NormaStmlLectio;

/* gignentes NIHIL = sine registro (tituli servantur, nota quisque);
 * registro dato titulus absens = vitium. Omnia aut nihil: vitio
 * quolibet normae vacuae; vitia OMNIA redduntur. */
NormaStmlLectio
norma_stml_legere (
                               chorda  fons,
    constans NormaGignensNominatum* gignentes,
                                  i32  numerus_gignentium,
                             Piscina* piscina);

/* titulus sine '#' ('#' praefixum toleratur); NIHIL si abest */
Norma*
norma_stml_quaerere (
    constans NormaStmlLectio* lectio,
         constans character* titulus);

/* Forma canonica = stml_scribere pulchrum. Nodus normae nominatae
 * alterius -> <ad norma="#x"/>. Recusat (chorda vacua, *causa
 * posita): titulus vacuus, nodus pravus (SCHEMA_PRAVA), gignens cum
 * functione sine titulo, textus attributi quem STML ferre nequit
 * (" & < > linea nova), circulus (profunditas > CXXVIII). */
chorda
norma_stml_scribere (
    constans NormaNominata* normae,
                       i32  numerus,
                  Piscina*  piscina,
                    chorda* causa);

#endif /* NORMA_STML_H */
```

- [ ] **A0.4** `include/norma_ad_c.h`:

```c
/* norma_ad_c.h - normae nominatae -> fons C qui easdem aedificat
 * (norma-spec-2 §III.3). Exitus GENERATUM (linea prima); bin/norma c
 * eum in plagulas scribit, fabrica iudicat.
 *
 * Pro praefixo "vates_norma_":
 *   Norma* vates_norma_<titulus>(Piscina*)          una per normam
 *   externus JsonValor* vates_norma_gignens_<g>(Sors*, Piscina*, vacuum*)
 *                                                   per gignens="g"
 * - vocans eam definit; nexor probat. Referentiae <ad> semel per
 * vocationem aedificantur (DAG servatur).
 *
 * STATUS: PROPOSITUM (norma-plan-3 A0). */
#ifndef NORMA_AD_C_H
#define NORMA_AD_C_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "norma.h"
#include "norma_stml.h"

nomen structura {
    chorda  caput;    /* .h */
    chorda  corpus;   /* .c */
} NormaFonsC;

/* caput_titulus: nomen plagulae capitis quod corpus includit
 * ("vates_responsum_norma.h"); fons_titulus: via .norma in commento
 * GENERATUM. Recusat (mensurae 0, *causa): titulus non identificator
 * C, nodus pravus, gignens sine titulo, numerus fluitans non finitus,
 * textus > DIX octeti (C89), profunditas > CXXVIII. */
NormaFonsC
norma_ad_c (
    constans NormaNominata* normae,
                       i32  numerus,
       constans character* praefixum,
       constans character* caput_titulus,
       constans character* fons_titulus,
                  Piscina* piscina,
                   chorda* causa);

#endif /* NORMA_AD_C_H */
```

- [ ] **A0.5** compile check: a scratch TU in the scratchpad including all
  four headers, built with `tools/vexilla.sh` flags and `-Iinclude`.
  Expected: no diagnostics. Then present the four headers + R3 to Fran.
- [ ] **A0.6** on approval: STATUS lines -> `PROBATUM a Frano <date>`;
  commit `include/norma.h include/norma_stml.h include/norma_ad_c.h
  include/norma_canon.h project-specs/norma-plan-3.md`; gates `radix`
  (+ main's portae_debitae names); `sine_debitis` with cause. NOTE: a
  header with no `lib/X.c` - if aedilis or the root suite objects to
  `norma_canon.h`/`norma_stml.h`/`norma_ad_c.h` lacking bodies, hold the
  three new headers back and commit them with their bodies (A2, A3, A5);
  record the ruling.

---
### Task A1: core - generator name

**Files:** Modify `lib/norma.c`, `probationes/probatio_norma.c`,
`probationes/probatio_norma_gignere.c`, `lib/norma.worklog.md`.

**Interfaces:** Produces `norma_gignens_titulus`, `NormaVisus.gignens_titulus`
(A0.1). Consumes nothing new.

- [ ] **A1.0** merge main into quarta: `git merge main` (expect no
  conflicts in norma/vates files); rebuild local judges if the session
  hook names them (`./tools/fabrica_struere.sh`, `aedilis_struere.sh`,
  `compilator_struere.sh` - never `instituere.sh` here).
- [ ] **A1.1 failing tests.** In `probationes/probatio_norma.c` add and
  call from `principale` after `probatio_json_schema(p)`:

```c
interior vacuum
probatio_gignens_titulus(Piscina* p)
{
     Norma* n = norma_gignens_titulus(norma_textus(p), "sententia");
    NormaVisus v = norma_visus(n);

    imprimere("\n--- Probans gignens_titulus ---\n");
    CREDO_CHORDA_AEQUALIS_LITERIS(v.gignens_titulus, "sententia");
    CREDO_NIHIL(v.gignens);
    CREDO_AEQUALIS_I32(norma_visus(norma_textus(p)).gignens_titulus.mensura, 0);
    /* titulus non iudicat: valor textus validus manet */
    CREDO_VERUM(_iud(n, "\"x\"", p).validum);
    /* NIHIL tolerantur */
    CREDO_NIHIL(norma_gignens_titulus(NIHIL, "x"));
    CREDO_AEQUALIS_I32(norma_visus(norma_gignens_titulus(norma_textus(p),
        NIHIL)).gignens_titulus.mensura, 0);
}
```

  In `probationes/probatio_norma_gignere.c` add and call from `principale`:

```c
/* titulus sine functione: generator ordinarius, octetis idem */
interior vacuum
probatio_titulus_sine_functione(Piscina* p)
{
    s64 semen;
    i32 diversa = 0;

    imprimere("\n--- Probans titulum sine functione ---\n");
    per (semen = 0; semen < L; semen++)
    {
        NormaGenitum a = norma_gignere(norma_gignens_titulus(
            norma_textus(p), "sententia"), NORMA_TYPICA, semen, p);
        NormaGenitum b = norma_gignere(norma_textus(p), NORMA_TYPICA,
            semen, p);

        si (!chorda_aequalis(json_scribere(a.valor, p),
                             json_scribere(b.valor, p)))
        {
            diversa++;
        }
    }
    CREDO_AEQUALIS_I32(diversa, 0);
}
```

- [ ] **A1.2** run `./compile_tests.sh probatio_norma > build/a1.log 2>&1; tail -20 build/a1.log`.
  Expected: compile FAIL - `norma_gignens_titulus` undeclared (header
  from A0 is in the tree; body is not) -> link error.
- [ ] **A1.3 implement** in `lib/norma.c`: field `chorda gignens_titulus;`
  after `gignens_datum` in `structura Norma`; builder after `norma_gignens`:

```c
Norma*
norma_gignens_titulus (
                 Norma* n,
    constans character* titulus)
{
    si (n && titulus)
    {
        n->gignens_titulus = chorda_ex_literis(titulus, n->piscina);
    }
    redde n;
}
```

  and in `norma_visus` after `v.gignens_datum = ...`:
  `v.gignens_titulus = n->gignens_titulus;` (the `!n` branch already
  memsets to zero; `_nodus` memsets, so an unset title has mensura 0).
- [ ] **A1.4** run both suites (`./compile_tests.sh probatio_norma`).
  Expected: all pass, including N4's oracle (generator untouched: it
  tests `v.gignens`, the function pointer, `lib/norma_gignere.c:272`).
- [ ] **A1.5 plant:** delete the `v.gignens_titulus = ...` line, rerun.
  Expected: `probatio_gignens_titulus` RED (first assertion). Restore
  with plain `cp`, rerun green.
- [ ] **A1.6** worklog entry in `lib/norma.worklog.md` (name without
  function = default generation; why: STML/C faces need a name);
  commit `lib/norma.c probationes/probatio_norma.c
  probationes/probatio_norma_gignere.c lib/norma.worklog.md`; gates
  `radix`, `generata` + main's portae_debitae for those paths.

---

### Task A2: norma.canon, embedded

**Files:** Create `norma.canon`, `tools/norma_canon_infigere.sh`,
`lib/norma_canon.c` (GENERATUM), `probationes/probatio_norma_stml.c`
(canon part); modify `canones.registrum`, `aedificatio.stml`; regenerate
the source list.

**Interfaces:** Produces `norma_canon_textus(Piscina*)` (A0.2), the
dialect contract of spec §II. Consumes `canon_legere`, `canon_iudicare`,
`stml_legere`, `internamentum_creare`, `filum_legere_totum`.

- [ ] **A2.1** `norma.canon` (repository root, beside `aedilis.canon`):

```xml
<!--
  norma.canon - canon dialecti NORMAE (plagulae .norma)
  Specificatio: project-specs/norma-spec-2.md §II.

  QUID CANON TENET: vocabularium clausum; attributa per genus (ergo
  'forma' in integer = attributum ignotum); genera attributorum;
  citationes <ad norma="#x"> ad <norma titulus="#x">; tituli normarum
  unici; claves campi intra objectum unicae; valores variationis
  intra discrimen unici.

  QUID LECTOR TENET (lib/norma_stml.c), CONSULTO: typus filius UNUS
  (canon numerum per nomen solum novit, non per gregem nominum);
  fines bini et parsabiles (minimum numeri fluitans est - textus
  hic); electio non vacua nec cum <licitum> simul; circuli
  referentiarum; gignentes contra registrum.

  INFIXUS: lib/norma_canon.c ex hac plagula GENERATUR
  (tools/norma_canon_infigere.sh) - mutata hac, regenera.
-->
<canon dialectus="normae" versio="1">

  <elementum nomen="normae" radix="verum">
    <attributum nomen="versio" genus="numerus" necessarium="verum"
      minimum="1" maximum="1"/>
    <liberum nomen="norma"/>
  </elementum>

  <elementum nomen="norma">
    <attributum nomen="titulus" genus="identitas" necessarium="verum"/>
    <liberum nomen="liberum"    maximum="1"/>
    <liberum nomen="nullum"     maximum="1"/>
    <liberum nomen="boolean"    maximum="1"/>
    <liberum nomen="integer"    maximum="1"/>
    <liberum nomen="numerus"    maximum="1"/>
    <liberum nomen="textus"     maximum="1"/>
    <liberum nomen="tabulatum"  maximum="1"/>
    <liberum nomen="objectum"   maximum="1"/>
    <liberum nomen="discrimen"  maximum="1"/>
    <liberum nomen="ad"         maximum="1"/>
  </elementum>

  <elementum nomen="campus">
    <attributum nomen="titulus"    genus="textus" necessarium="verum"/>
    <attributum nomen="requiritur" genus="veritas" ordinarius="verum"/>
    <liberum nomen="liberum"    maximum="1"/>
    <liberum nomen="nullum"     maximum="1"/>
    <liberum nomen="boolean"    maximum="1"/>
    <liberum nomen="integer"    maximum="1"/>
    <liberum nomen="numerus"    maximum="1"/>
    <liberum nomen="textus"     maximum="1"/>
    <liberum nomen="tabulatum"  maximum="1"/>
    <liberum nomen="objectum"   maximum="1"/>
    <liberum nomen="discrimen"  maximum="1"/>
    <liberum nomen="ad"         maximum="1"/>
  </elementum>

  <elementum nomen="tabulatum">
    <attributum nomen="aut_nullum"       genus="veritas"/>
    <attributum nomen="gignens"          genus="nomen"/>
    <attributum nomen="longitudo_minima" genus="numerus" minimum="0"/>
    <attributum nomen="longitudo_maxima" genus="numerus" minimum="0"/>
    <liberum nomen="descriptio" maximum="1"/>
    <liberum nomen="liberum"    maximum="1"/>
    <liberum nomen="nullum"     maximum="1"/>
    <liberum nomen="boolean"    maximum="1"/>
    <liberum nomen="integer"    maximum="1"/>
    <liberum nomen="numerus"    maximum="1"/>
    <liberum nomen="textus"     maximum="1"/>
    <liberum nomen="tabulatum"  maximum="1"/>
    <liberum nomen="objectum"   maximum="1"/>
    <liberum nomen="discrimen"  maximum="1"/>
    <liberum nomen="ad"         maximum="1"/>
  </elementum>

  <elementum nomen="liberum">
    <attributum nomen="aut_nullum" genus="veritas"/>
    <attributum nomen="gignens"    genus="nomen"/>
    <liberum nomen="descriptio" maximum="1"/>
  </elementum>

  <elementum nomen="nullum">
    <attributum nomen="gignens" genus="nomen"/>
    <liberum nomen="descriptio" maximum="1"/>
  </elementum>

  <elementum nomen="boolean">
    <attributum nomen="aut_nullum" genus="veritas"/>
    <attributum nomen="gignens"    genus="nomen"/>
    <liberum nomen="descriptio" maximum="1"/>
  </elementum>

  <elementum nomen="integer">
    <attributum nomen="aut_nullum" genus="veritas"/>
    <attributum nomen="gignens"    genus="nomen"/>
    <attributum nomen="minimum"    genus="numerus"/>
    <attributum nomen="maximum"    genus="numerus"/>
    <liberum nomen="descriptio" maximum="1"/>
  </elementum>

  <elementum nomen="numerus">
    <attributum nomen="aut_nullum" genus="veritas"/>
    <attributum nomen="gignens"    genus="nomen"/>
    <attributum nomen="minimum"    genus="textus"
      nota="f64 - lector parsat (1e-300 licet; canon numerus non)"/>
    <attributum nomen="maximum"    genus="textus"/>
    <liberum nomen="descriptio" maximum="1"/>
  </elementum>

  <elementum nomen="textus">
    <attributum nomen="aut_nullum"       genus="veritas"/>
    <attributum nomen="gignens"          genus="nomen"/>
    <attributum nomen="longitudo_minima" genus="numerus" minimum="0"/>
    <attributum nomen="longitudo_maxima" genus="numerus" minimum="0"/>
    <attributum nomen="forma"            genus="compositum"/>
    <attributum nomen="electio"          genus="textus"
      nota="valores spatiis separati; valor cum spatio -> licitum"/>
    <liberum nomen="descriptio" maximum="1"/>
    <liberum nomen="licitum"/>
  </elementum>

  <elementum nomen="objectum">
    <attributum nomen="aut_nullum" genus="veritas"/>
    <attributum nomen="gignens"    genus="nomen"/>
    <attributum nomen="modus"      genus="electio" ordinarius="clausum">
      <optio>clausum</optio>
      <optio>apertum</optio>
      <optio>notandum</optio>
    </attributum>
    <liberum nomen="descriptio" maximum="1"/>
    <liberum nomen="campus"/>
  </elementum>

  <elementum nomen="discrimen">
    <attributum nomen="clavis"     genus="textus"  necessarium="verum"/>
    <attributum nomen="aut_nullum" genus="veritas"/>
    <attributum nomen="gignens"    genus="nomen"/>
    <attributum nomen="modus"      genus="electio" ordinarius="clausum">
      <optio>clausum</optio>
      <optio>apertum</optio>
      <optio>notandum</optio>
    </attributum>
    <liberum nomen="descriptio" maximum="1"/>
    <liberum nomen="variatio" minimum="1"/>
  </elementum>

  <elementum nomen="variatio">
    <attributum nomen="valor" genus="textus" necessarium="verum"/>
    <liberum nomen="objectum" minimum="1" maximum="1"/>
  </elementum>

  <elementum nomen="ad"
    nota="sine aut_nullum consulto: nodus communis mutaretur ubique">
    <attributum nomen="norma" genus="referentia" necessarium="verum"/>
  </elementum>

  <elementum nomen="descriptio" textus="verum"/>
  <elementum nomen="licitum"    textus="verum"/>

  <citatio nomen="normae-citatae" attributum="norma"
    ad="norma/titulus" super="ad"/>
  <unicitas nomen="normarum-tituli" attributum="titulus" super="norma"/>
  <unicitas nomen="campi-objecti" attributum="titulus" super="campus"
    intra="objectum"/>
  <unicitas nomen="variationes-discriminis" attributum="valor"
    super="variatio" intra="discrimen"/>

</canon>
```

  Check the canon itself: `bin/canon_examen norma.canon` (judges a canon
  against `canon.canon` via the `.canon` registry line). Expected: 0
  vitia. If `intra=` does not scope to the NEAREST instance for nested
  objecta (an inner `campus id` falsely clashing with an outer one), drop
  the two `intra` unicitates and ledger a ruling: the core already
  refuses duplicate keys/variants as SCHEMA_PRAVA, which the loader
  surfaces (A3.3 `_pravitas_colligere`).

- [ ] **A2.2** `canones.registrum` - append (tab-separated, beside the
  other dialect lines):

```
<normae>	norma.canon
.norma	norma.canon
```

- [ ] **A2.3** `tools/norma_canon_infigere.sh`:

```bash
#!/bin/bash
# tools/norma_canon_infigere.sh - norma.canon -> lib/norma_canon.c
# (GENERATUM; regula R1 norma-plan-3). Sub FABRICA_SCRIPTURA exitus in
# scripturam iudicis scribitur (viae eaedem), arbor intacta.
#
# Usus: ./tools/norma_canon_infigere.sh
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/.." || exit 2
FONS=norma.canon
RADIX="${FABRICA_SCRIPTURA:-.}"
EXITUS="$RADIX/lib/norma_canon.c"
[ -f "$FONS" ] || { echo "norma_canon_infigere: $FONS deest" >&2; exit 2; }
mkdir -p "$RADIX/lib" || exit 2
{
    printf '/* GENERATUM a tools/norma_canon_infigere.sh ex norma.canon - noli manu mutare */\n'
    printf '#include "norma_canon.h"\n\n#include <string.h>\n\n'
    printf 'interior constans insignatus character _octeti[] = {\n'
    od -An -v -tx1 "$FONS" | awk '{ s = "   "; for (i = 1; i <= NF; i++) s = s " 0x" $i ","; print s }'
    printf '};\n\n'
    printf 'chorda\nnorma_canon_textus (\n    Piscina* piscina)\n{\n'
    printf '    chorda c;\n\n'
    printf '    c.mensura  = (i32)magnitudo(_octeti);\n'
    printf '    c.datum    = (i8*)piscina_allocare(piscina, (memoriae_index)c.mensura);\n'
    printf '    memcpy(c.datum, _octeti, (size_t)c.mensura);\n'
    printf '    redde c;\n}\n'
} > "$EXITUS.tmp" && mv "$EXITUS.tmp" "$EXITUS"
```

  `chmod +x`, run it. Expected: `lib/norma_canon.c` line 1 contains
  `GENERATUM`; a second run leaves it byte-identical (`cmp`).

- [ ] **A2.4** `aedificatio.stml` - generator action beside the capsula
  actions:

```xml
  <actio titulus="norma_canon" genus="generator">
    <mandatum>
      <verbum! (>./tools/norma_canon_infigere.sh
    </mandatum>
    <ingressus genus="fasciculus" via="tools/norma_canon_infigere.sh"/>
    <ingressus genus="fasciculus" via="norma.canon"/>
    <exitus via="lib/norma_canon.c" provenientia="regeneratio"/>
  </actio>
```

  Then `./tools/compile_tests_fontes_generare.sh` (new `lib/*.c`) and
  `bin/fabrica iudicare -plenus -omnia > build/a2.fabrica.log 2>&1;
  tail build/a2.fabrica.log`. Expected: `norma_canon` judged fresh.

- [ ] **A2.5 failing test** - `probationes/probatio_norma_stml.c`
  (canon part; A3/A4 extend this file):

```c
/* probatio_norma_stml.c - facies STML normae: canon infixus (A2),
 * lector (A3), scriptor et oracula I, II, V (A4) - norma-plan-3 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "json.h"
#include "filum.h"
#include "internamentum.h"
#include "stml.h"
#include "canon.h"
#include "norma.h"
#include "norma_canon.h"

#include <stdio.h>
#include <string.h>

/* numerus vitiorum canonis infixi super documentum; primum genus */
interior i32
_canon_vitia (
    constans character* documentum,
      CanonVitiumGenus* primum,
               Piscina* p)
{
    InternamentumChorda* in = internamentum_creare(p);
                 chorda  causa;
                 Canon* canon = canon_legere(norma_canon_textus(p), p, in,
                                    &causa);
           StmlResultus  r = stml_legere_ex_literis(documentum, p, in);
                    Xar* v;

    CREDO_NON_NIHIL(canon);
    CREDO_VERUM(r.successus);
    si (!canon || !r.successus)
    {
        redde M;
    }
    v = canon_iudicare(canon, r.radix, p);
    si (primum && xar_numerus(v) > 0)
    {
        *primum = ((CanonVitium*)xar_obtinere(v, 0))->genus;
    }
    redde xar_numerus(v);
}

interior vacuum
probatio_canon_infixus(Piscina* p)
{
    CanonVitiumGenus g = (CanonVitiumGenus)0;
              chorda  discus = filum_legere_totum("norma.canon", p);

    imprimere("\n--- Probans canonem infixum ---\n");
    /* infixus == plagula: generatio non rancida */
    CREDO_CHORDA_AEQUALIS(norma_canon_textus(p), discus);
    CREDO_AEQUALIS_I32(_canon_vitia(
        "<normae versio=\"1\" (> <norma titulus=\"#a\" (>"
        " <objectum modus=\"notandum\" (>"
        " <campus titulus=\"x\" (> <ad norma=\"#b\"/>\n"
        "<norma titulus=\"#b\" (> <textus electio=\"p q\"/>\n</>\n",
        NIHIL, p), 0);
    CREDO_AEQUALIS_I32(_canon_vitia("<normae versio=\"1\" (> <nemo/>\n",
        &g, p), I);
    CREDO_VERUM(g == CANON_ELEMENTUM_IGNOTUM);
    _canon_vitia("<normae versio=\"1\" (> <norma titulus=\"#a\" (>"
        " <integer forma=\"uuid\"/>\n", &g, p);
    CREDO_VERUM(g == CANON_ATTRIBUTUM_IGNOTUM);
    _canon_vitia("<normae versio=\"1\" (> <norma titulus=\"#a\" (>"
        " <ad norma=\"#nemo\"/>\n", &g, p);
    CREDO_VERUM(g == CANON_CITATIO_IRRITA);
    _canon_vitia("<normae versio=\"1\" ((> <norma titulus=\"#a\" (>"
        " <textus/>\n <norma titulus=\"#a\" (> <textus/>\n", &g, p);
    CREDO_VERUM(g == CANON_NOMEN_BIS);
    _canon_vitia("<normae versio=\"1\" (> <norma titulus=\"#a\" (>"
        " <objectum(> <campus titulus=\"x\" requiritur=\"fortasse\" (>"
        " <textus/>\n", &g, p);
    CREDO_VERUM(g == CANON_VALOR_MALUS);
    _canon_vitia("<normae versio=\"1\" (> <norma titulus=\"#a\" ((>"
        " <textus/>\n <textus/>\n", &g, p);
    CREDO_VERUM(g == CANON_LIBERI_MULTI);
}

s32
principale (vacuum)
{
    Piscina* p = piscina_generare_dynamicum("probatio_norma_stml", M * M);
        b32  successus;

    credo_aperire(p);
    probatio_canon_infixus(p);
    credo_imprimere_compendium();
    successus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(p);
    redde successus ? 0 : I;
}
```

  The second `<norma>` in a capture chain: if `((>` on `<normae>` does
  not capture two following lines in this inline form, rewrite the
  fixture strings as explicit `<normae versio="1"> ... </normae>` - the
  assertions are about canon verdicts, not capture syntax.

- [ ] **A2.6** run before A2.3 exists (`./compile_tests.sh probatio_norma_stml`).
  Expected: link FAIL (`norma_canon_textus`). After A2.3: all pass.
- [ ] **A2.7 plants:** (a) edit one byte of `norma.canon` without
  regenerating -> `CREDO_CHORDA_AEQUALIS(norma_canon_textus, discus)`
  RED; (b) remove the `<citatio>` line and regenerate -> the
  `CITATIO_IRRITA` assertion RED. Restore both, regenerate, green.
- [ ] **A2.8** create `lib/norma_stml.worklog.md` (R1, canon/loader
  split, nested `intra` finding); commit `norma.canon
  canones.registrum tools/norma_canon_infigere.sh lib/norma_canon.c
  include/norma_canon.h aedificatio.stml probationes/probatio_norma_stml.c
  lib/norma_stml.worklog.md` + regenerated source list; gates `radix`,
  `generata` + main's portae_debitae names (aedificatio.stml and
  canones.registrum likely owe more - take what it prints).

---
### Task A3: the reader

**Files:** Create `lib/norma_stml.c` (part A, reader),
`probationes/fixa/norma/normae_fixae.norma`; extend
`probationes/probatio_norma_stml.c`; regenerate the source list.

**Interfaces:** Produces `norma_stml_legere`, `norma_stml_quaerere`
(A0.3). Consumes `norma_canon_textus` (A2), `norma_gignens_titulus`
(A1), `stml_legere`, `stml_attributum_capere`, `stml_textus_valor`,
`canon_legere`, `canon_iudicare`, `canon_nuntius`, the norma builders.

- [ ] **A3.1 fixture** `probationes/fixa/norma/normae_fixae.norma` -
  hand-written in explicit-close form (A4 replaces it with the writer's
  canonical form; R5). Covers every element and modifier, a key needing
  brackets, a non-ASCII key, a shared reference, licitum with a space,
  a generator name, float bounds that do not print exactly:

```
<normae versio="1">
  <norma titulus="#usus">
    <objectum modus="notandum">
      <descriptio>Usus signorum.</descriptio>
      <campus titulus="input_tokens"><integer minimum="0" maximum="1000000"/></campus>
      <campus titulus="cache_read_input_tokens" requiritur="falsum"><integer minimum="0" maximum="1000000"/></campus>
      <campus titulus="ratio"><numerus minimum="0.1" maximum="0.3"/></campus>
      <campus titulus="minima" requiritur="falsum"><numerus minimum="-1e-300" maximum="1e-300"/></campus>
    </objectum>
  </norma>
  <norma titulus="#omnia">
    <objectum aut_nullum="verum">
      <campus titulus="quodvis"><liberum/></campus>
      <campus titulus="nihil"><nullum/></campus>
      <campus titulus="verum_falsum"><boolean aut_nullum="verum"/></campus>
      <campus titulus="a.b"><integer minimum="-5" maximum="5"/></campus>
      <campus titulus="clāvis"><textus longitudo_minima="1" longitudo_maxima="40"/></campus>
      <campus titulus="dies"><textus forma="date-time"/></campus>
      <campus titulus="stop_reason"><textus aut_nullum="verum" electio="end_turn tool_use max_tokens"/></campus>
      <campus titulus="spatiosa"><textus><licitum>cum spatio</licitum><licitum>sine</licitum></textus></campus>
      <campus titulus="origo"><textus gignens="ex_functione"/></campus>
      <campus titulus="tags" requiritur="falsum"><tabulatum longitudo_minima="0" longitudo_maxima="4"><textus electio="a b c"/></tabulatum></campus>
      <campus titulus="usus_primus"><ad norma="#usus"/></campus>
      <campus titulus="usus_secundus" requiritur="falsum"><ad norma="#usus"/></campus>
      <campus titulus="input"><objectum modus="apertum"/></campus>
      <campus titulus="content">
        <tabulatum>
          <discrimen clavis="type" modus="notandum">
            <variatio valor="text"><objectum><campus titulus="text"><textus/></campus></objectum></variatio>
            <variatio valor="tool_use"><objectum><campus titulus="id"><textus/></campus><campus titulus="input"><liberum/></campus></objectum></variatio>
          </discrimen>
        </tabulatum>
      </campus>
    </objectum>
  </norma>
</normae>
```

- [ ] **A3.2 failing tests** - add to `probatio_norma_stml.c` (includes
  `norma_stml.h`, `chorda_aedificator.h`), call from `principale` after
  `probatio_canon_infixus`:

```c
#define FIXA "probationes/fixa/norma/normae_fixae.norma"

interior JsonValor*
_ex_functione (
       Sors* sors,
    Piscina* p,
     vacuum* datum)
{
    (vacuum)sors;
    (vacuum)datum;
    redde json_chorda_creare_literis(p, "ex functione");
}

hic_manens constans NormaGignensNominatum _registrum[] = {
    { "ex_functione", _ex_functione, NIHIL }
};

interior chorda
_js (
     Norma* n,
    Piscina* p)
{
    redde json_scribere(norma_json_schema(n, p), p);
}

interior NormaStmlLectio
_lege (
    constans character* fons,
               Piscina* p)
{
    redde norma_stml_legere(chorda_ex_literis(fons, p), NIHIL, 0, p);
}

interior NormaStmlVitium*
_vitium_primum (NormaStmlLectio l)
{
    redde xar_numerus(l.vitia) > 0
        ? (NormaStmlVitium*)xar_obtinere(l.vitia, 0) : NIHIL;
}

/* causa vitii primi; bit in *visae */
interior b32
_causa (
    constans character* fons,
         NormaStmlCausa causa,
                   i32* visae,
               Piscina* p)
{
    NormaStmlLectio l = _lege(fons, p);
    NormaStmlVitium* v = _vitium_primum(l);

    CREDO_FALSUM(l.successus);
    CREDO_AEQUALIS_I32(xar_numerus(l.normae), 0);
    si (!v)
    {
        redde FALSUM;
    }
    *visae |= (i32)I << (i32)v->causa;
    si (v->causa != causa)
    {
        imprimere("  exspectata %d, inventa %d: %.*s\n", (integer)causa,
            (integer)v->causa, (integer)v->nuntius.mensura,
            (constans character*)v->nuntius.datum);
    }
    redde v->causa == causa;
}

/* "#usus" aedificatum manu - idem ac FIXA */
interior Norma*
_usus_manu (Piscina* p)
{
    Norma* o = norma_modus(norma_objectum(p), NORMA_NOTANDUM);

    norma_descriptio(o, "Usus signorum.");
    norma_campus(o, "input_tokens", norma_intra(norma_integer(p), 0,
        M * M), VERUM);
    norma_campus(o, "cache_read_input_tokens", norma_intra(
        norma_integer(p), 0, M * M), FALSUM);
    norma_campus(o, "ratio", norma_intra_fluitans(norma_numerus(p), 0.1,
        0.3), VERUM);
    norma_campus(o, "minima", norma_intra_fluitans(norma_numerus(p),
        -1e-300, 1e-300), FALSUM);
    redde o;
}

interior vacuum
probatio_lector(Piscina* p)
{
    chorda  fons = filum_legere_totum(FIXA, p);
    NormaStmlLectio l;
    NormaStmlLectio sine;
    Norma* omnia;
    NormaVisus v;
    NormaCampus* primus;
    NormaCampus* secundus;
    i32 i;

    imprimere("\n--- Probans lectorem ---\n");
    l = norma_stml_legere(fons, _registrum, I, p);
    CREDO_VERUM(l.successus);
    CREDO_AEQUALIS_I32(xar_numerus(l.vitia), 0);
    CREDO_AEQUALIS_I32(xar_numerus(l.notae), 0);
    CREDO_AEQUALIS_I32(xar_numerus(l.normae), II);
    CREDO_CHORDA_AEQUALIS_LITERIS(((NormaNominata*)xar_obtinere(l.normae,
        0))->titulus, "usus");
    CREDO_CHORDA_AEQUALIS(_js(norma_stml_quaerere(&l, "usus"), p),
                          _js(_usus_manu(p), p));
    CREDO_VERUM(norma_stml_quaerere(&l, "#usus")
                == norma_stml_quaerere(&l, "usus"));
    CREDO_NIHIL(norma_stml_quaerere(&l, "nemo"));
    /* referentia communis: idem nodus bis, idem ac norma nominata */
    omnia     = norma_stml_quaerere(&l, "omnia");
    v         = norma_visus(omnia);
    primus    = NIHIL;
    secundus  = NIHIL;
    per (i = 0; i < xar_numerus(v.campi); i++)
    {
        NormaCampus* c = (NormaCampus*)xar_obtinere(v.campi, i);

        si (chorda_aequalis_literis(c->titulus, "usus_primus")) primus = c;
        si (chorda_aequalis_literis(c->titulus, "usus_secundus")) secundus = c;
    }
    CREDO_NON_NIHIL(primus);
    CREDO_NON_NIHIL(secundus);
    si (primus && secundus)
    {
        CREDO_VERUM(primus->valor == secundus->valor);
        CREDO_VERUM(primus->valor == norma_stml_quaerere(&l, "usus"));
        CREDO_FALSUM(secundus->requiritur);
    }
    CREDO_VERUM(v.aut_nullum);
    /* gignens: registro dato functio ponitur */
    CREDO_VERUM(_iud_validum(omnia, p));
    /* sine registro: nota una, titulus servatus, successus */
    sine = norma_stml_legere(fons, NIHIL, 0, p);
    CREDO_VERUM(sine.successus);
    CREDO_AEQUALIS_I32(xar_numerus(sine.notae), I);
    si (xar_numerus(sine.notae) == I)
    {
        CREDO_VERUM(((NormaStmlVitium*)xar_obtinere(sine.notae, 0))->causa
                    == NORMA_STML_GIGNENS_SINE_REGISTRO);
    }
}
```

  `_iud_validum` (above `probatio_lector`): generate one TYPICA value
  with `norma_gignere(n, NORMA_TYPICA, VII, p)`, judge it, return
  `validum`, AND check that field `origo` equals `"ex functione"`
  (proof the registry function was attached):

```c
interior b32
_iud_validum (Norma* n, Piscina* p)
{
    NormaGenitum g = norma_gignere(n, NORMA_TYPICA, VII, p);
    JsonValor* origo;

    si (!g.valor)
    {
        redde FALSUM;
    }
    si (json_est_nullum(g.valor))
    {
        g = norma_gignere(n, NORMA_TYPICA, VIII, p);   /* aut_nullum */
    }
    origo = json_objectum_capere(g.valor, "origo");
    CREDO_VERUM(origo && json_est_chorda(origo));
    redde norma_iudicare(n, g.valor, p).validum;
}
```

  (add `#include "norma_gignere.h"`; if seed VIII also yields null, pick
  the first seed in 0..L that does not - the point is one non-null value.)

  Error classes, line:column, all-errors, coverage:

```c
#define CAPUT "<normae versio=\"1\">\n<norma titulus=\"#a\">\n"
#define CAUDA "\n</norma>\n</normae>\n"

interior vacuum
probatio_vitia_lectoris(Piscina* p)
{
    i32 visae = 0;
    NormaStmlLectio l;
    NormaStmlVitium* v;

    imprimere("\n--- Probans vitia lectoris ---\n");
    CREDO_VERUM(_causa("<normae versio=\"1\"", NORMA_STML_FRACTUM, &visae, p));
    CREDO_VERUM(_causa(CAPUT "<nemo/>" CAUDA, NORMA_STML_CANON, &visae, p));
    CREDO_VERUM(_causa(CAPUT "<objectum><campus titulus=\"x\"/></objectum>"
        CAUDA, NORMA_STML_LIBERI_TYPI, &visae, p));
    CREDO_VERUM(_causa(CAPUT "<objectum><campus titulus=\"x\"><textus/>"
        "<integer/></campus></objectum>" CAUDA, NORMA_STML_LIBERI_TYPI,
        &visae, p));
    CREDO_VERUM(_causa(CAPUT "<numerus minimum=\"abc\" maximum=\"2\"/>"
        CAUDA, NORMA_STML_FINIS_PRAVUS, &visae, p));
    CREDO_VERUM(_causa(CAPUT "<integer minimum=\"99999999999999999999\""
        " maximum=\"1\"/>" CAUDA, NORMA_STML_FINIS_PRAVUS, &visae, p));
    CREDO_VERUM(_causa(CAPUT "<numerus minimum=\"0\" maximum=\"inf\"/>"
        CAUDA, NORMA_STML_FINIS_PRAVUS, &visae, p));
    CREDO_VERUM(_causa(CAPUT "<integer minimum=\"0\"/>" CAUDA,
        NORMA_STML_FINIS_DIMIDIATUS, &visae, p));
    CREDO_VERUM(_causa(CAPUT "<textus longitudo_maxima=\"3\"/>" CAUDA,
        NORMA_STML_FINIS_DIMIDIATUS, &visae, p));
    CREDO_VERUM(_causa(CAPUT "<integer minimum=\"5\" maximum=\"2\"/>"
        CAUDA, NORMA_STML_FINES_INVERSI, &visae, p));
    CREDO_VERUM(_causa(CAPUT "<textus electio=\"a\"><licitum>b</licitum>"
        "</textus>" CAUDA, NORMA_STML_ELECTIO_DUPLEX, &visae, p));
    CREDO_VERUM(_causa(CAPUT "<textus electio=\"   \"/>" CAUDA,
        NORMA_STML_ELECTIO_VACUA, &visae, p));
    CREDO_VERUM(_causa("<normae versio=\"1\">\n"
        "<norma titulus=\"#a\"><ad norma=\"#b\"/></norma>\n"
        "<norma titulus=\"#b\"><ad norma=\"#a\"/></norma>\n</normae>\n",
        NORMA_STML_CIRCULUS, &visae, p));
    /* circulus catenam nominat */
    v = _vitium_primum(_lege("<normae versio=\"1\">\n"
        "<norma titulus=\"#a\"><ad norma=\"#b\"/></norma>\n"
        "<norma titulus=\"#b\"><ad norma=\"#a\"/></norma>\n</normae>\n", p));
    CREDO_VERUM(v && chorda_continet(v->nuntius,
        chorda_ex_literis("#a -> #b -> #a", p)));
    /* gignens ignotum: registro dato */
    l = norma_stml_legere(chorda_ex_literis(CAPUT
        "<textus gignens=\"nemo\"/>" CAUDA, p), _registrum, I, p);
    CREDO_FALSUM(l.successus);
    v = _vitium_primum(l);
    CREDO_VERUM(v && v->causa == NORMA_STML_GIGNENS_IGNOTUM);
    si (v)
    {
        visae |= (i32)I << (i32)v->causa;
    }
    /* nota sine registro: successus, nota */
    l = _lege(CAPUT "<textus gignens=\"sententia\"/>" CAUDA, p);
    CREDO_VERUM(l.successus);
    CREDO_AEQUALIS_I32(xar_numerus(l.notae), I);
    si (xar_numerus(l.notae) == I)
    {
        visae |= (i32)I << (i32)((NormaStmlVitium*)xar_obtinere(l.notae,
            0))->causa;
    }
    /* linea et columna: <integer> in linea III, columna I */
    v = _vitium_primum(_lege(CAPUT "<integer minimum=\"0\"/>" CAUDA, p));
    CREDO_VERUM(v != NIHIL);
    si (v)
    {
        CREDO_AEQUALIS_I32(v->linea, III);
        CREDO_AEQUALIS_I32(v->columna, I);
    }
    /* vitia OMNIA, non primum solum */
    l = _lege(CAPUT "<objectum><campus titulus=\"x\"><integer minimum=\"0\"/>"
        "</campus><campus titulus=\"y\"><integer minimum=\"5\""
        " maximum=\"2\"/></campus></objectum>" CAUDA, p);
    CREDO_AEQUALIS_I32(xar_numerus(l.vitia), II);
    /* tegumentum: classis omnis visa (FRACTUM..SINE_REGISTRO) */
    CREDO_AEQUALIS_I32(visae, ((i32)I << (NORMA_STML_GIGNENS_SINE_REGISTRO
        + I)) - I);
}
```

- [ ] **A3.3** run (`./compile_tests.sh probatio_norma_stml`).
  Expected: link FAIL - `norma_stml_legere` undefined.
- [ ] **A3.4 implement** `lib/norma_stml.c` part A:

```c
/* norma_stml.c - facies STML normae (norma-spec-2; norma-plan-3: A3
 * lector, A4 scriptor). Canon primus iudicat (norma.canon infixus),
 * lector sensum. */
#include "norma_stml.h"
#include "norma_canon.h"
#include "stml.h"
#include "canon.h"
#include "internamentum.h"
#include "chorda_aedificator.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* ====================================================================
 * A. LECTOR
 * ==================================================================== */

/* norma nominata: status 0 nondum, I in aedificatione, II facta */
nomen structura {
        chorda  titulus;    /* sine '#' */
    StmlNodus*  nodus;      /* <norma> */
        Norma*  facta;
           i32  status;
} NominataLectoris;

nomen structura {
                           Piscina* p;
                            chorda  fons;
    constans NormaGignensNominatum* gignentes;
                               i32  numerus_gignentium;
                               Xar* vitia;
                               Xar* notae;
                               Xar* nominatae;   /* NominataLectoris */
                               Xar* catena;      /* i32: in aedificatione */
} Lector;

interior constans character* constans _typi[] = {
    "liberum", "nullum", "boolean", "integer", "numerus", "textus",
    "tabulatum", "objectum", "discrimen", "ad", NIHIL
};

interior b32
_est (
             StmlNodus* n,
    constans character* titulus)
{
    redde n && n->genus == STML_NODUS_ELEMENTUM && n->titulus
        && chorda_aequalis_literis(*n->titulus, titulus);
}

interior b32
_est_typus (
    StmlNodus* n)
{
    i32 i;

    per (i = 0; _typi[i]; i++)
    {
        si (_est(n, _typi[i]))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior StmlNodus*
_filius (
    StmlNodus* n,
          i32  i)
{
    redde *(StmlNodus**)xar_obtinere(n->liberi, i);
}

/* columna 1-basata ex positu nodi (octeti post lineam novam) */
interior i32
_columna (
       Lector* lec,
    StmlNodus* n)
{
    i32 i;

    si (!n || n->linea == 0)
    {
        redde 0;
    }
    i = n->positus_initium;
    dum (i > 0 && lec->fons.datum[i - I] != '\n')
    {
        i--;
    }
    redde n->positus_initium - i + I;
}

interior vacuum
_vitium (
                 Lector* lec,
              StmlNodus* nodus,
         NormaStmlCausa  causa,
    constans character* nuntius)
{
    NormaStmlVitium* v = (NormaStmlVitium*)xar_addere(
        causa == NORMA_STML_GIGNENS_SINE_REGISTRO ? lec->notae
                                                  : lec->vitia);

    v->linea    = nodus ? nodus->linea : 0;
    v->columna  = _columna(lec, nodus);
    v->causa    = causa;
    v->nuntius  = chorda_ex_literis(nuntius, lec->p);
}

/* attributum ut literae C propriae (NIHIL si abest) */
interior character*
_attributum (
                 Lector* lec,
              StmlNodus* n,
    constans character* titulus)
{
    chorda* c = stml_attributum_capere(n, titulus);

    redde c ? chorda_ut_cstr(*c, lec->p) : NIHIL;
}

interior b32
_verum (
                 Lector* lec,
              StmlNodus* n,
    constans character* titulus,
                    b32  ordinarius)
{
    character* s = _attributum(lec, n, titulus);

    redde s ? strcmp(s, "verum") == 0 : ordinarius;
}

interior NormaModus
_modus (
       Lector* lec,
    StmlNodus* n)
{
    character* s = _attributum(lec, n, "modus");

    si (s && strcmp(s, "apertum") == 0)
    {
        redde NORMA_APERTUM;
    }
    si (s && strcmp(s, "notandum") == 0)
    {
        redde NORMA_NOTANDUM;
    }
    redde NORMA_CLAUSUM;
}

interior b32
_integrum (
                 Lector* lec,
              StmlNodus* n,
    constans character* s,
                    s64  infimum,
                    s64  summum,
                   s64* exitus)
{
    character* finis;
       longus  v;
    character  nuntius[CCLVI];

    errno  = 0;
    v      = strtol(s, &finis, X);
    si (   errno == ERANGE || finis == s || *finis != '\0'
        || (s64)v < infimum || (s64)v > summum)
    {
        sprintf(nuntius, "finis pravus: '%.100s'", s);
        _vitium(lec, n, NORMA_STML_FINIS_PRAVUS, nuntius);
        redde FALSUM;
    }
    *exitus = (s64)v;
    redde VERUM;
}

interior b32
_fluitans (
                 Lector* lec,
              StmlNodus* n,
    constans character* s,
                   f64* exitus)
{
    character* finis;
          f64  v;
    character  nuntius[CCLVI];

    errno  = 0;
    v      = strtod(s, &finis);
    si (   finis == s || *finis != '\0' || errno == ERANGE
        || v != v || v - v != 0.0)
    {
        sprintf(nuntius, "finis pravus (f64 finitus postulatur): '%.100s'",
            s);
        _vitium(lec, n, NORMA_STML_FINIS_PRAVUS, nuntius);
        redde FALSUM;
    }
    *exitus = v;
    redde VERUM;
}

/* par finium (R3): 0 neuter, I ambo, II vitium iam positum */
interior i32
_par (
                 Lector* lec,
              StmlNodus* n,
    constans character* a,
    constans character* b,
             character** sa,
             character** sb)
{
    character nuntius[CCLVI];

    *sa = _attributum(lec, n, a);
    *sb = _attributum(lec, n, b);
    si (!*sa && !*sb)
    {
        redde 0;
    }
    si (!*sa || !*sb)
    {
        sprintf(nuntius, "'%s' sine '%s' (fines bini; norma-plan-3 R3)",
            *sa ? a : b, *sa ? b : a);
        _vitium(lec, n, NORMA_STML_FINIS_DIMIDIATUS, nuntius);
        redde II;
    }
    redde I;
}

interior vacuum
_inversi (
       Lector* lec,
    StmlNodus* n)
{
    _vitium(lec, n, NORMA_STML_FINES_INVERSI, "minimum > maximum");
}

interior vacuum
_intra_integrum (
       Lector* lec,
    StmlNodus* n,
        Norma* norma)
{
    character *sa, *sb;
          s64  a, b;

    si (_par(lec, n, "minimum", "maximum", &sa, &sb) != I)
    {
        redde;
    }
    si (   !_integrum(lec, n, sa, (s64)LONG_MIN, (s64)LONG_MAX, &a)
        || !_integrum(lec, n, sb, (s64)LONG_MIN, (s64)LONG_MAX, &b))
    {
        redde;
    }
    si (a > b)
    {
        _inversi(lec, n);
        redde;
    }
    norma_intra(norma, a, b);
}

interior vacuum
_intra_fluitans (
       Lector* lec,
    StmlNodus* n,
        Norma* norma)
{
    character *sa, *sb;
          f64  a, b;

    si (_par(lec, n, "minimum", "maximum", &sa, &sb) != I)
    {
        redde;
    }
    si (!_fluitans(lec, n, sa, &a) || !_fluitans(lec, n, sb, &b))
    {
        redde;
    }
    si (a > b)
    {
        _inversi(lec, n);
        redde;
    }
    norma_intra_fluitans(norma, a, b);
}

interior vacuum
_longitudo (
       Lector* lec,
    StmlNodus* n,
        Norma* norma)
{
    character *sa, *sb;
          s64  a, b;

    si (_par(lec, n, "longitudo_minima", "longitudo_maxima", &sa, &sb)
        != I)
    {
        redde;
    }
    si (   !_integrum(lec, n, sa, 0, (s64)0xFFFFFFFFL, &a)
        || !_integrum(lec, n, sb, 0, (s64)0xFFFFFFFFL, &b))
    {
        redde;
    }
    si (a > b)
    {
        _inversi(lec, n);
        redde;
    }
    norma_longitudo(norma, (i32)a, (i32)b);
}

interior b32
_spatium (character c)
{
    redde c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

interior vacuum
_electio (
       Lector* lec,
    StmlNodus* n,
        Norma* norma)
{
              character* e       = _attributum(lec, n, "electio");
                    Xar* licita  = xar_creare(lec->p,
                                       (i32)magnitudo(character*));
                    i32  ex_liberis = 0;
                    i32  i;
    constans character** tabula;

    per (i = 0; i < xar_numerus(n->liberi); i++)
    {
        StmlNodus* f = _filius(n, i);

        si (_est(f, "licitum"))
        {
            *(character**)xar_addere(licita) = chorda_ut_cstr(
                stml_textus_valor(f, lec->p), lec->p);
            ex_liberis++;
        }
    }
    si (e && ex_liberis > 0)
    {
        _vitium(lec, n, NORMA_STML_ELECTIO_DUPLEX,
            "electio= et <licitum> simul");
        redde;
    }
    si (e)
    {
        character* s = e;

        dum (*s)
        {
            dum (_spatium(*s))
            {
                s++;
            }
            si (!*s)
            {
                frange;
            }
            *(character**)xar_addere(licita) = s;
            dum (*s && !_spatium(*s))
            {
                s++;
            }
            si (*s)
            {
                *s = '\0';
                s++;
            }
        }
        si (xar_numerus(licita) == 0)
        {
            _vitium(lec, n, NORMA_STML_ELECTIO_VACUA,
                "electio sine valore");
            redde;
        }
    }
    si (xar_numerus(licita) == 0)
    {
        redde;
    }
    tabula = (constans character**)piscina_allocare(lec->p,
        (memoriae_index)((xar_numerus(licita) + I)
                         * magnitudo(character*)));
    per (i = 0; i < xar_numerus(licita); i++)
    {
        tabula[i] = *(character**)xar_obtinere(licita, i);
    }
    tabula[i] = NIHIL;
    norma_electio(norma, tabula);
}

interior vacuum
_descriptio (
       Lector* lec,
    StmlNodus* n,
        Norma* norma)
{
    i32 i;

    per (i = 0; i < xar_numerus(n->liberi); i++)
    {
        si (_est(_filius(n, i), "descriptio"))
        {
            norma_descriptio(norma, chorda_ut_cstr(stml_textus_valor(
                _filius(n, i), lec->p), lec->p));
        }
    }
}

interior vacuum
_gignens (
       Lector* lec,
    StmlNodus* n,
        Norma* norma)
{
    character* g = _attributum(lec, n, "gignens");
          i32  i;
    character  nuntius[CCLVI];

    si (!g)
    {
        redde;
    }
    norma_gignens_titulus(norma, g);
    si (!lec->gignentes)
    {
        sprintf(nuntius, "gignens '%.100s' sine registro: titulus servatus",
            g);
        _vitium(lec, n, NORMA_STML_GIGNENS_SINE_REGISTRO, nuntius);
        redde;
    }
    per (i = 0; i < lec->numerus_gignentium; i++)
    {
        si (strcmp(lec->gignentes[i].titulus, g) == 0)
        {
            norma_gignens(norma, lec->gignentes[i].functio,
                lec->gignentes[i].datum);
            redde;
        }
    }
    sprintf(nuntius, "gignens ignotum: '%.100s'", g);
    _vitium(lec, n, NORMA_STML_GIGNENS_IGNOTUM, nuntius);
}

interior Norma* _struere (Lector* lec, StmlNodus* n);

/* typus filius unicus continentis (norma, campus, tabulatum) */
interior Norma*
_typus_unicus (
       Lector* lec,
    StmlNodus* continens)
{
    StmlNodus* typus    = NIHIL;
          i32  numerus  = 0;
          i32  i;
    character  nuntius[CCLVI];

    per (i = 0; i < xar_numerus(continens->liberi); i++)
    {
        si (_est_typus(_filius(continens, i)))
        {
            typus = _filius(continens, i);
            numerus++;
        }
    }
    si (numerus != I)
    {
        sprintf(nuntius, "<%.*s> typum unum postulat, %u invenit",
            (integer)continens->titulus->mensura,
            (constans character*)continens->titulus->datum,
            (insignatus integer)numerus);
        _vitium(lec, continens, NORMA_STML_LIBERI_TYPI, nuntius);
        redde NIHIL;
    }
    redde _struere(lec, typus);
}

interior i32
_index_nominatae (
                 Lector* lec,
    constans character* titulus)
{
    i32 i;

    per (i = 0; i < xar_numerus(lec->nominatae); i++)
    {
        si (chorda_aequalis_literis(((NominataLectoris*)xar_obtinere(
                lec->nominatae, i))->titulus, titulus))
        {
            redde i;
        }
    }
    redde xar_numerus(lec->nominatae);
}

interior NominataLectoris*
_nominatam (
    Lector* lec,
       i32  i)
{
    redde (NominataLectoris*)xar_obtinere(lec->nominatae, i);
}

interior vacuum
_circulus (
       Lector* lec,
    StmlNodus* citans,
          i32  index)
{
    ChordaAedificator* a = chorda_aedificator_creare(lec->p, CCLVI);
                  i32  i;
                  b32  intra = FALSUM;
               chorda  c;

    chorda_aedificator_appendere_literis(a, "circulus: ");
    per (i = 0; i < xar_numerus(lec->catena); i++)
    {
        i32 k = *(i32*)xar_obtinere(lec->catena, i);

        si (k == index)
        {
            intra = VERUM;
        }
        si (intra)
        {
            chorda_aedificator_appendere_literis(a, "#");
            chorda_aedificator_appendere_chorda(a,
                _nominatam(lec, k)->titulus);
            chorda_aedificator_appendere_literis(a, " -> ");
        }
    }
    chorda_aedificator_appendere_literis(a, "#");
    chorda_aedificator_appendere_chorda(a, _nominatam(lec, index)->titulus);
    c = chorda_aedificator_finire(a);
    _vitium(lec, citans, NORMA_STML_CIRCULUS,
        chorda_ut_cstr(c, lec->p));
}

interior Norma*
_nominata (
       Lector* lec,
          i32  index,
    StmlNodus* citans)
{
    NominataLectoris* nl = _nominatam(lec, index);

    si (nl->status == II)
    {
        redde nl->facta;
    }
    si (nl->status == I)
    {
        _circulus(lec, citans, index);
        redde NIHIL;
    }
    nl->status = I;
    *(i32*)xar_addere(lec->catena) = index;
    nl->facta = _typus_unicus(lec, nl->nodus);
    xar_removere_ultimum(lec->catena);
    nl->status = II;
    redde nl->facta;
}

interior Norma*
_struere (
       Lector* lec,
    StmlNodus* n)
{
        Norma* norma = NIHIL;
    character* s;
          i32  i;

    si (_est(n, "ad"))
    {
        s = _attributum(lec, n, "norma");
        i = _index_nominatae(lec, s && s[0] == '#' ? s + I : "");
        si (i == xar_numerus(lec->nominatae))
        {
            _vitium(lec, n, NORMA_STML_CANON, "citatio irrita");
            redde NIHIL;   /* canon id iam clamavit; custodia */
        }
        redde _nominata(lec, i, n);
    }
    si (_est(n, "liberum"))
    {
        norma = norma_liberum(lec->p);
    }
    alioquin si (_est(n, "nullum"))
    {
        norma = norma_nullum(lec->p);
    }
    alioquin si (_est(n, "boolean"))
    {
        norma = norma_boolean(lec->p);
    }
    alioquin si (_est(n, "integer"))
    {
        norma = norma_integer(lec->p);
        _intra_integrum(lec, n, norma);
    }
    alioquin si (_est(n, "numerus"))
    {
        norma = norma_numerus(lec->p);
        _intra_fluitans(lec, n, norma);
    }
    alioquin si (_est(n, "textus"))
    {
        norma = norma_textus(lec->p);
        _longitudo(lec, n, norma);
        s = _attributum(lec, n, "forma");
        si (s)
        {
            norma_forma(norma, s);
        }
        _electio(lec, n, norma);
    }
    alioquin si (_est(n, "tabulatum"))
    {
        Norma* e = _typus_unicus(lec, n);

        norma = norma_tabulatum(lec->p, e ? e : norma_liberum(lec->p));
        _longitudo(lec, n, norma);
    }
    alioquin si (_est(n, "objectum"))
    {
        norma = norma_modus(norma_objectum(lec->p), _modus(lec, n));
        per (i = 0; i < xar_numerus(n->liberi); i++)
        {
            StmlNodus* c = _filius(n, i);
                Norma* valor;

            si (!_est(c, "campus"))
            {
                perge;
            }
            valor = _typus_unicus(lec, c);
            si (valor)
            {
                norma_campus(norma, _attributum(lec, c, "titulus"), valor,
                    _verum(lec, c, "requiritur", VERUM));
            }
        }
    }
    alioquin si (_est(n, "discrimen"))
    {
        norma = norma_modus(norma_discrimen(lec->p,
            _attributum(lec, n, "clavis")), _modus(lec, n));
        per (i = 0; i < xar_numerus(n->liberi); i++)
        {
            StmlNodus* va = _filius(n, i);
                   i32  k;

            si (!_est(va, "variatio"))
            {
                perge;
            }
            per (k = 0; k < xar_numerus(va->liberi); k++)
            {
                si (_est(_filius(va, k), "objectum"))
                {
                    Norma* o = _struere(lec, _filius(va, k));

                    si (o)
                    {
                        norma_variatio(norma, _attributum(lec, va, "valor"),
                            o);
                    }
                }
            }
        }
    }
    si (!norma)
    {
        redde NIHIL;
    }
    _descriptio(lec, n, norma);
    _gignens(lec, n, norma);
    si (_verum(lec, n, "aut_nullum", FALSUM))
    {
        norma_aut_nullum(norma);
    }
    redde norma;
}

/* nodus pravus (custodia: canon claves duplicatas iam vetat) */
interior b32
_pravum (
    constans Norma* n,
          chorda* nuntius)
{
    NormaVisus v = norma_visus(n);
           i32 i;

    si (!n)
    {
        redde FALSUM;
    }
    si (v.error_schematis.mensura > 0)
    {
        *nuntius = v.error_schematis;
        redde VERUM;
    }
    si (v.elementum && _pravum(v.elementum, nuntius))
    {
        redde VERUM;
    }
    per (i = 0; v.campi && i < xar_numerus(v.campi); i++)
    {
        si (_pravum(((NormaCampus*)xar_obtinere(v.campi, i))->valor,
                nuntius))
        {
            redde VERUM;
        }
    }
    per (i = 0; v.variationes && i < xar_numerus(v.variationes); i++)
    {
        si (_pravum(((NormaVariatio*)xar_obtinere(v.variationes,
                i))->objectum, nuntius))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

NormaStmlLectio
norma_stml_legere (
                               chorda  fons,
    constans NormaGignensNominatum* gignentes,
                                  i32  numerus_gignentium,
                             Piscina* piscina)
{
       NormaStmlLectio  l;
                Lector  lec;
    InternamentumChorda* in;
          StmlResultus  r;
                 Canon* canon;
                chorda  causa;
                   Xar* cv;
                   i32  i;
             character  nuntius[DXII];

    memset(&l, 0, magnitudo(l));
    memset(&lec, 0, magnitudo(lec));
    lec.p                   = piscina;
    lec.fons                = fons;
    lec.gignentes           = gignentes;
    lec.numerus_gignentium  = numerus_gignentium;
    lec.vitia      = xar_creare(piscina, (i32)magnitudo(NormaStmlVitium));
    lec.notae      = xar_creare(piscina, (i32)magnitudo(NormaStmlVitium));
    lec.nominatae  = xar_creare(piscina, (i32)magnitudo(NominataLectoris));
    lec.catena     = xar_creare(piscina, (i32)magnitudo(i32));
    l.normae       = xar_creare(piscina, (i32)magnitudo(NormaNominata));
    l.vitia        = lec.vitia;
    l.notae        = lec.notae;

    in  = internamentum_creare(piscina);
    r   = stml_legere(fons, piscina, in);
    si (!r.successus)
    {
        NormaStmlVitium* v = (NormaStmlVitium*)xar_addere(lec.vitia);

        v->linea    = r.linea_erroris;
        v->columna  = r.columna_erroris;
        v->causa    = NORMA_STML_FRACTUM;
        v->nuntius  = r.error;
        redde l;
    }
    canon = canon_legere(norma_canon_textus(piscina), piscina, in, &causa);
    si (!canon)
    {
        sprintf(nuntius, "canon infixus fractus: %.*s",
            (integer)(causa.mensura > CC ? CC : causa.mensura),
            (constans character*)causa.datum);
        _vitium(&lec, NIHIL, NORMA_STML_CANON, nuntius);
        redde l;
    }
    cv = canon_iudicare(canon, r.radix, piscina);
    per (i = 0; i < xar_numerus(cv); i++)
    {
        CanonVitium* c = (CanonVitium*)xar_obtinere(cv, i);

        sprintf(nuntius, "%s%s%.*s%s%.*s", canon_nuntius(c->genus),
            c->elementum ? ": <" : "",
            c->elementum ? (integer)c->elementum->mensura : 0,
            c->elementum ? (constans character*)c->elementum->datum : "",
            c->detail ? "> " : (c->elementum ? ">" : ""),
            c->detail ? (integer)c->detail->mensura : 0,
            c->detail ? (constans character*)c->detail->datum : "");
        _vitium(&lec, c->nodus, NORMA_STML_CANON, nuntius);
    }
    si (xar_numerus(lec.vitia) > 0)
    {
        redde l;
    }
    per (i = 0; i < xar_numerus(r.elementum_radix->liberi); i++)
    {
        StmlNodus* f = _filius(r.elementum_radix, i);

        si (_est(f, "norma"))
        {
            NominataLectoris* nl = (NominataLectoris*)xar_addere(
                lec.nominatae);
                   character* t  = _attributum(&lec, f, "titulus");

            nl->titulus  = chorda_ex_literis(t + I, piscina);  /* '#' */
            nl->nodus    = f;
            nl->facta    = NIHIL;
            nl->status   = 0;
        }
    }
    per (i = 0; i < xar_numerus(lec.nominatae); i++)
    {
        chorda pravum;

        _nominata(&lec, i, NIHIL);
        si (_pravum(_nominatam(&lec, i)->facta, &pravum))
        {
            sprintf(nuntius, "schema pravum: %.*s",
                (integer)(pravum.mensura > CC ? CC : pravum.mensura),
                (constans character*)pravum.datum);
            _vitium(&lec, _nominatam(&lec, i)->nodus, NORMA_STML_CANON,
                nuntius);
        }
    }
    si (xar_numerus(lec.vitia) > 0)
    {
        redde l;
    }
    per (i = 0; i < xar_numerus(lec.nominatae); i++)
    {
        NormaNominata* nn = (NormaNominata*)xar_addere(l.normae);

        nn->titulus  = _nominatam(&lec, i)->titulus;
        nn->norma    = _nominatam(&lec, i)->facta;
    }
    l.successus = VERUM;
    redde l;
}

Norma*
norma_stml_quaerere (
    constans NormaStmlLectio* lectio,
         constans character* titulus)
{
    i32 i;

    si (!lectio || !lectio->normae || !titulus)
    {
        redde NIHIL;
    }
    si (titulus[0] == '#')
    {
        titulus++;
    }
    per (i = 0; i < xar_numerus(lectio->normae); i++)
    {
        NormaNominata* nn = (NormaNominata*)xar_obtinere(lectio->normae, i);

        si (chorda_aequalis_literis(nn->titulus, titulus))
        {
            redde nn->norma;
        }
    }
    redde NIHIL;
}
```

  Needs `#include <limits.h>` for `LONG_MIN`/`LONG_MAX` (s64 = long long
  but strtol gives long - on macOS LP64 the ranges agree; ledger a note).
  `_typus_unicus` on the `<norma>` node: its type child is the norma.

- [ ] **A3.5** `./tools/compile_tests_fontes_generare.sh`; run the suite.
  Expected: all A2 + A3 assertions pass. Debug with printf on any
  mismatch (the class prints "exspectata / inventa").
- [ ] **A3.6 plants** (each compiles, each restored):
  (a) `_par` returns I when one side is missing -> `FINIS_DIMIDIATUS`
  assertions RED; (b) `_nominata` skips the `status == I` check (guard
  by depth: `if (xar_numerus(lec->catena) > X) redde NIHIL;` added
  instead so it terminates) -> CIRCULUS RED and coverage RED;
  (c) `_columna` returns 0 -> line/column assertion RED.
- [ ] **A3.7** worklog (`lib/norma_stml.worklog.md`: canon vs loader
  split as found, capture = children measured, LONG range note);
  commit `lib/norma_stml.c probationes/probatio_norma_stml.c
  probationes/fixa/norma/normae_fixae.norma include/norma_stml.h
  lib/norma_stml.worklog.md` + source list; gates `radix`, `generata`
  + main's portae_debitae names.

---
### Task A4: the writer - oracles I, II, V

**Files:** Extend `lib/norma_stml.c` (part B, writer),
`probationes/probatio_norma_stml.c`; REPLACE
`probationes/fixa/norma/normae_fixae.norma` with the writer's form.

**Interfaces:** Produces `norma_stml_scribere` (A0.3). Consumes
`stml_elementum_creare`, `stml_attributum_addere{,_chorda}`,
`stml_liberum_addere`, `stml_textum_addere`, `stml_scribere`,
`norma_visus`.

- [ ] **A4.1 failing tests** - add to `probatio_norma_stml.c`, call from
  `principale` after `probatio_vitia_lectoris`:

```c
interior NormaNominata*
_tabula (
        Xar* x,
    Piscina* p)
{
    NormaNominata* t = (NormaNominata*)piscina_allocare(p,
        (memoriae_index)((xar_numerus(x) + I) * magnitudo(NormaNominata)));

    xar_copiare_ad_tabulam(x, t, 0, xar_numerus(x));
    redde t;
}

interior chorda
_scribe (
        Xar* normae,
    Piscina* p)
{
    chorda causa;
    chorda s = norma_stml_scribere(_tabula(normae, p), xar_numerus(normae),
                   p, &causa);

    si (s.mensura == 0)
    {
        imprimere("  recusatum: %.*s\n", (integer)causa.mensura,
            (constans character*)causa.datum);
    }
    redde s;
}

interior b32
_iudicia_aequalia (
    NormaIudicium a,
    NormaIudicium b)
{
    i32 i;

    si (   a.validum != b.validum
        || xar_numerus(a.vitia) != xar_numerus(b.vitia)
        || xar_numerus(a.notae) != xar_numerus(b.notae))
    {
        redde FALSUM;
    }
    per (i = 0; i < xar_numerus(a.vitia) + xar_numerus(a.notae); i++)
    {
        b32 vit = i < xar_numerus(a.vitia);
        i32 k   = vit ? i : i - xar_numerus(a.vitia);
        NormaVitium* x = (NormaVitium*)xar_obtinere(vit ? a.vitia : a.notae, k);
        NormaVitium* y = (NormaVitium*)xar_obtinere(vit ? b.vitia : b.notae, k);

        si (x->causa != y->causa || !chorda_aequalis(x->via, y->via))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* duae normae idem dicunt: exportatio eadem, iudicia eadem super
 * valores ab UTRAQUE genitos (TYPICA, FINES, INVALIDA; L semina) */
interior i32
_idem_dicunt (
      Norma* a,
      Norma* b,
    Piscina* p)
{
    i32 diversa = 0;
    s64 semen;
    i32 m;
    i32 q;

    si (!chorda_aequalis(_js(a, p), _js(b, p)))
    {
        diversa++;
    }
    per (q = 0; q < II; q++)
    {
        per (semen = 0; semen < L; semen++)
        {
            per (m = (i32)NORMA_TYPICA; m <= (i32)NORMA_INVALIDA; m++)
            {
                NormaGenitum g = norma_gignere(q == 0 ? a : b,
                    (NormaModusGignendi)m, semen, p);

                si (g.valor && !_iudicia_aequalia(norma_iudicare(a,
                        g.valor, p), norma_iudicare(b, g.valor, p)))
                {
                    diversa++;
                }
            }
        }
    }
    redde diversa;
}

/* oraculum V: forma pulchra = forma plagulae (formator nihil mutaret) */
interior b32
_formatum (
    chorda  x,
    Piscina* p)
{
    StmlResultus r = stml_legere(x, p, internamentum_creare(p));

    redde r.successus && chorda_aequalis(stml_scribere(r.radix, p, VERUM),
                                         x);
}

/* corpus aedificatorum (copiae ex probatio_norma_gignere.c, titulo
 * gignentis addito; aliter scriptor recte recusat) */
interior Norma*
_schema_simplex (Piscina* p)
{
    constans character* constans licita[] = { "a", "b", "c", NIHIL };
    Norma* o = norma_objectum(p);

    norma_campus(o, "id", norma_intra(norma_integer(p), 0, MMXLVIII),
        VERUM);
    norma_campus(o, "nomen", norma_longitudo(norma_textus(p), I, XL),
        FALSUM);
    norma_campus(o, "email", norma_forma(norma_textus(p), "email"), VERUM);
    norma_campus(o, "codex", norma_longitudo(norma_forma(norma_textus(p),
        "uuid"), XXXVI, XXXVI), VERUM);
    norma_campus(o, "tags", norma_longitudo(norma_tabulatum(p,
        norma_electio(norma_textus(p), licita)), 0, IV), FALSUM);
    norma_campus(o, "ratio", norma_intra_fluitans(norma_numerus(p), 0.1,
        0.3), VERUM);
    norma_campus(o, "nota", norma_aut_nullum(norma_textus(p)), FALSUM);
    norma_campus(o, "datum", norma_forma(norma_textus(p), "date-time"),
        VERUM);
    norma_campus(o, "origo", norma_gignens_titulus(norma_gignens(
        norma_textus(p), _ex_functione, NIHIL), "ex_functione"), VERUM);
    redde o;
}

interior Norma*
_schema_responsi (Piscina* p)
{
    constans character* constans fines[] = { "end_turn", "tool_use",
        "max_tokens", NIHIL };
    Norma* textus   = norma_objectum(p);
    Norma* petitum  = norma_objectum(p);
    Norma* blocus   = norma_discrimen(p, "type");
    Norma* usus     = norma_objectum(p);
    Norma* r        = norma_objectum(p);

    norma_campus(textus, "text", norma_textus(p), VERUM);
    norma_campus(petitum, "id", norma_textus(p), VERUM);
    norma_campus(petitum, "name", norma_textus(p), VERUM);
    norma_campus(petitum, "input", norma_modus(norma_objectum(p),
        NORMA_APERTUM), VERUM);
    norma_variatio(blocus, "text", textus);
    norma_variatio(blocus, "tool_use", petitum);
    norma_campus(usus, "input_tokens", norma_intra(norma_integer(p), 0,
        M * M), VERUM);
    norma_campus(usus, "output_tokens", norma_intra(norma_integer(p), 0,
        M * M), VERUM);
    norma_campus(r, "id", norma_textus(p), VERUM);
    norma_campus(r, "content", norma_tabulatum(p, blocus), VERUM);
    norma_campus(r, "stop_reason", norma_electio(norma_textus(p), fines),
        VERUM);
    norma_campus(r, "usage", usus, VERUM);
    redde r;
}

interior vacuum
probatio_oraculum_primum(Piscina* p)
{
    chorda fixa = filum_legere_totum(FIXA, p);
    NormaStmlLectio l1 = norma_stml_legere(fixa, _registrum, I, p);
    chorda s1 = _scribe(l1.normae, p);
    NormaStmlLectio l2 = norma_stml_legere(s1, _registrum, I, p);
    chorda s2 = _scribe(l2.normae, p);
    NormaStmlLectio l3 = norma_stml_legere(s2, _registrum, I, p);
    chorda s3 = _scribe(l3.normae, p);
    NormaVisus ratio;

    imprimere("\n--- Oraculum I: scribere(legere(x)) == x, bis ---\n");
    CREDO_VERUM(l1.successus && l2.successus && l3.successus);
    CREDO_CHORDA_AEQUALIS(s1, fixa);
    CREDO_CHORDA_AEQUALIS(s2, s1);
    CREDO_CHORDA_AEQUALIS(s3, s2);
    CREDO_AEQUALIS_I32(_idem_dicunt(norma_stml_quaerere(&l1, "omnia"),
        norma_stml_quaerere(&l3, "omnia"), p), 0);
    /* fines fluitantes per textum intacti */
    ratio = norma_visus(((NormaCampus*)xar_obtinere(norma_visus(
        norma_stml_quaerere(&l3, "usus")).campi, II))->valor);
    CREDO_VERUM(ratio.minimum_fluitans == 0.1);
    CREDO_VERUM(ratio.maximum_fluitans == 0.3);
    imprimere("\n--- Oraculum V: plagula forma pulchra ---\n");
    CREDO_VERUM(_formatum(fixa, p));
}

interior vacuum
probatio_oraculum_secundum(Piscina* p)
{
    Norma* corpus[II];
    i32 i;

    imprimere("\n--- Oraculum II: aedificator -> scriptor -> lector ---\n");
    corpus[0] = _schema_simplex(p);
    corpus[I] = _schema_responsi(p);
    per (i = 0; i < II; i++)
    {
        NormaNominata nn;
        chorda causa;
        chorda s;
        NormaStmlLectio l;

        nn.titulus  = chorda_ex_literis("x", p);
        nn.norma    = corpus[i];
        s           = norma_stml_scribere(&nn, I, p, &causa);
        CREDO_VERUM(s.mensura > 0);
        l = norma_stml_legere(s, _registrum, I, p);
        CREDO_VERUM(l.successus);
        si (!l.successus)
        {
            perge;
        }
        CREDO_AEQUALIS_I32(_idem_dicunt(corpus[i],
            norma_stml_quaerere(&l, "x"), p), 0);
        CREDO_CHORDA_AEQUALIS(_scribe(l.normae, p), s);
        CREDO_VERUM(_formatum(s, p));
    }
}

interior vacuum
probatio_scriptor_communis_et_recusationes(Piscina* p)
{
    Norma* usus = norma_objectum(p);
    Norma* r    = norma_objectum(p);
    Norma* circ = norma_objectum(p);
    NormaNominata nn[II];
    NormaStmlLectio l;
    chorda causa;
    chorda s;
    NormaVisus v;

    imprimere("\n--- Probans scriptorem: communia, recusationes ---\n");
    norma_campus(usus, "n", norma_integer(p), VERUM);
    norma_campus(r, "a", usus, VERUM);
    norma_campus(r, "b", usus, FALSUM);
    nn[0].titulus = chorda_ex_literis("usus", p);
    nn[0].norma   = usus;
    nn[I].titulus = chorda_ex_literis("r", p);
    nn[I].norma   = r;
    s = norma_stml_scribere(nn, II, p, &causa);
    CREDO_VERUM(chorda_continet(s, chorda_ex_literis(
        "<ad norma=\"#usus\"/>", p)));
    l = norma_stml_legere(s, NIHIL, 0, p);
    v = norma_visus(norma_stml_quaerere(&l, "r"));
    CREDO_VERUM(((NormaCampus*)xar_obtinere(v.campi, 0))->valor
                == ((NormaCampus*)xar_obtinere(v.campi, I))->valor);
    /* clavis quam STML ferre nequit (R6) */
    nn[0].norma = norma_campus(norma_objectum(p), "a\"b", norma_textus(p),
        VERUM);
    CREDO_AEQUALIS_I32(norma_stml_scribere(nn, I, p, &causa).mensura, 0);
    CREDO_VERUM(chorda_continet(causa, chorda_ex_literis("STML ferre nequit",
        p)));
    nn[0].norma = norma_campus(norma_objectum(p), "a\nb", norma_textus(p),
        VERUM);
    CREDO_AEQUALIS_I32(norma_stml_scribere(nn, I, p, &causa).mensura, 0);
    /* electio cum '<' -> licitum (textus effugitur), non recusatio */
    {
        constans character* constans licita[] = { "a<b", "c d", NIHIL };

        nn[0].norma = norma_electio(norma_textus(p), licita);
        s = norma_stml_scribere(nn, I, p, &causa);
        CREDO_VERUM(s.mensura > 0);
        l = norma_stml_legere(s, NIHIL, 0, p);
        CREDO_VERUM(l.successus);
        CREDO_AEQUALIS_I32(_idem_dicunt(nn[0].norma,
            norma_stml_quaerere(&l, "usus"), p), 0);
    }
    /* gignens cum functione sine titulo */
    nn[0].norma = norma_gignens(norma_textus(p), _ex_functione, NIHIL);
    CREDO_AEQUALIS_I32(norma_stml_scribere(nn, I, p, &causa).mensura, 0);
    /* nodus pravus */
    nn[0].norma = norma_longitudo(norma_textus(p), V, II);
    CREDO_AEQUALIS_I32(norma_stml_scribere(nn, I, p, &causa).mensura, 0);
    /* circulus aedificatorum: nominatus et innominatus */
    norma_campus(circ, "ego", circ, VERUM);
    nn[0].norma = circ;
    CREDO_AEQUALIS_I32(norma_stml_scribere(nn, I, p, &causa).mensura, 0);
    CREDO_VERUM(chorda_continet(causa, chorda_ex_literis("circulus", p)));
    nn[0].norma = norma_campus(norma_objectum(p), "intus", circ, VERUM);
    CREDO_AEQUALIS_I32(norma_stml_scribere(nn, I, p, &causa).mensura, 0);
    CREDO_VERUM(chorda_continet(causa, chorda_ex_literis("profunditas",
        p)));
}
```

- [ ] **A4.2** run. Expected: link FAIL (`norma_stml_scribere`).
- [ ] **A4.3 implement** `lib/norma_stml.c` part B (append):

```c
/* ====================================================================
 * B. SCRIPTOR
 * ==================================================================== */

nomen structura {
                   Piscina* p;
       InternamentumChorda* in;
    constans NormaNominata* normae;
                       i32  numerus;
                       i32  radix;     /* index normae quae scribitur */
                    chorda* causa;
                       b32  fractum;
} Scriptor;

interior vacuum
_recusare (
               Scriptor* s,
    constans character* nuntius)
{
    si (!s->fractum)
    {
        s->fractum = VERUM;
        si (s->causa)
        {
            *s->causa = chorda_ex_literis(nuntius, s->p);
        }
    }
}

/* R6: STML valores attributorum CRUDOS scribit */
interior b32
_tutum (chorda c)
{
    i32 i;

    per (i = 0; i < c.mensura; i++)
    {
        character k = (character)c.datum[i];

        si (   k == '"' || k == '&' || k == '<' || k == '>' || k == '\n'
            || k == '\r')
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior StmlNodus*
_elementum (
               Scriptor* s,
              StmlNodus* parens,
    constans character* titulus)
{
    StmlNodus* e = stml_elementum_creare(s->p, s->in, titulus);

    si (parens)
    {
        stml_liberum_addere(parens, e);
    }
    redde e;
}

interior vacuum
_attr (
               Scriptor* s,
              StmlNodus* e,
    constans character* titulus,
                 chorda  valor,
    constans character* quid)
{
    character nuntius[CCLVI];

    si (!_tutum(valor))
    {
        sprintf(nuntius, "%s '%.*s' STML ferre nequit (\" & < > linea"
            " nova; norma-plan-3 R6)", quid,
            (integer)(valor.mensura > LXXX ? LXXX : valor.mensura),
            (constans character*)valor.datum);
        _recusare(s, nuntius);
        redde;
    }
    stml_attributum_addere_chorda(e, s->p, s->in, titulus, valor);
}

interior vacuum
_attr_literae (
               Scriptor* s,
              StmlNodus* e,
    constans character* titulus,
    constans character* valor)
{
    stml_attributum_addere(e, s->p, s->in, titulus, valor);
}

/* f64 brevissimum quod strtod idem reddit (oraculum I) */
interior vacuum
_fluitans_scribere (
          f64  v,
    character* buffer)
{
    i32 praecisio;

    per (praecisio = I; praecisio <= XVII; praecisio++)
    {
        sprintf(buffer, "%.*g", (integer)praecisio, v);
        si (strtod(buffer, NIHIL) == v)
        {
            redde;
        }
    }
}

interior constans character*
_titulus_generis (NormaGenus g)
{
    commutatio (g)
    {
        casus NORMA_LIBERUM:    redde "liberum";
        casus NORMA_NULLUM:     redde "nullum";
        casus NORMA_BOOLEAN:    redde "boolean";
        casus NORMA_INTEGER:    redde "integer";
        casus NORMA_NUMERUS:    redde "numerus";
        casus NORMA_TEXTUS:     redde "textus";
        casus NORMA_TABULATUM:  redde "tabulatum";
        casus NORMA_OBJECTUM:   redde "objectum";
        casus NORMA_DISCRIMEN:  redde "discrimen";
    }
    redde "liberum";
}

/* electio per attributum: omnes non vacuae, sine spatio, tutae */
interior b32
_electio_simplex (Xar* licita)
{
    i32 i;
    i32 k;

    per (i = 0; i < xar_numerus(licita); i++)
    {
        chorda c = *(chorda*)xar_obtinere(licita, i);

        si (c.mensura == 0 || !_tutum(c))
        {
            redde FALSUM;
        }
        per (k = 0; k < c.mensura; k++)
        {
            si (_spatium((character)c.datum[k]))
            {
                redde FALSUM;
            }
        }
    }
    redde VERUM;
}

interior i32
_index_normae (
          Scriptor* s,
    constans Norma* n)
{
    i32 i;

    per (i = 0; i < s->numerus; i++)
    {
        si (s->normae[i].norma == n)
        {
            redde i;
        }
    }
    redde s->numerus;
}

/* '#' + titulus */
interior chorda
_signatum (
    Scriptor* s,
       chorda  titulus)
{
    ChordaAedificator* a = chorda_aedificator_creare(s->p, LXIV);

    chorda_aedificator_appendere_character(a, '#');
    chorda_aedificator_appendere_chorda(a, titulus);
    redde chorda_aedificator_finire(a);
}

interior vacuum
_nodum_scribere (
          Scriptor* s,
    constans Norma* n,
         StmlNodus* parens,
               i32  profunditas,
               b32  ad_licet)
{
    NormaVisus v;
    StmlNodus* e;
          i32  k;
          i32  i;
          b32  simplex;
    character  buffer[LXIV];
    character  nuntius[CCLVI];

    si (s->fractum)
    {
        redde;
    }
    si (profunditas > CXXVIII)
    {
        _recusare(s, "profunditas > CXXVIII: circulus aedificatorum?");
        redde;
    }
    k = _index_normae(s, n);
    si (k < s->numerus && profunditas > 0)
    {
        si (k == s->radix)
        {
            _recusare(s, "circulus: norma se ipsam continet");
            redde;
        }
        si (ad_licet)
        {
            e = _elementum(s, parens, "ad");
            _attr(s, e, "norma", _signatum(s, s->normae[k].titulus),
                "titulus normae");
            redde;
        }
    }
    v = norma_visus(n);
    si (!n || v.error_schematis.mensura > 0)
    {
        sprintf(nuntius, "nodus pravus: %.*s",
            (integer)(v.error_schematis.mensura > CC ? CC
                      : v.error_schematis.mensura),
            v.error_schematis.datum ? (constans character*)v.error_schematis.datum
                                    : "NIHIL");
        _recusare(s, nuntius);
        redde;
    }
    e = _elementum(s, parens, _titulus_generis(v.genus));
    /* attributa, ordine fixo */
    si (v.genus == NORMA_DISCRIMEN)
    {
        _attr(s, e, "clavis", v.clavis_discriminis, "clavis discriminis");
    }
    si (   (v.genus == NORMA_OBJECTUM || v.genus == NORMA_DISCRIMEN)
        && v.modus != NORMA_CLAUSUM)
    {
        _attr_literae(s, e, "modus",
            v.modus == NORMA_APERTUM ? "apertum" : "notandum");
    }
    si (v.genus == NORMA_INTEGER && v.habet_intra)
    {
        sprintf(buffer, "%ld", (longus)v.minimum);
        _attr_literae(s, e, "minimum", buffer);
        sprintf(buffer, "%ld", (longus)v.maximum);
        _attr_literae(s, e, "maximum", buffer);
    }
    si (v.genus == NORMA_NUMERUS && v.habet_intra_fluitans)
    {
        _fluitans_scribere(v.minimum_fluitans, buffer);
        _attr_literae(s, e, "minimum", buffer);
        _fluitans_scribere(v.maximum_fluitans, buffer);
        _attr_literae(s, e, "maximum", buffer);
    }
    si (v.habet_longitudinem)
    {
        sprintf(buffer, "%lu", (insignatus longus)v.longitudo_minima);
        _attr_literae(s, e, "longitudo_minima", buffer);
        sprintf(buffer, "%lu", (insignatus longus)v.longitudo_maxima);
        _attr_literae(s, e, "longitudo_maxima", buffer);
    }
    si (v.forma.mensura > 0)
    {
        _attr(s, e, "forma", v.forma, "forma");
    }
    simplex = v.licita && xar_numerus(v.licita) > 0
              && _electio_simplex(v.licita);
    si (simplex)
    {
        ChordaAedificator* a = chorda_aedificator_creare(s->p, CXXVIII);

        per (i = 0; i < xar_numerus(v.licita); i++)
        {
            si (i > 0)
            {
                chorda_aedificator_appendere_character(a, ' ');
            }
            chorda_aedificator_appendere_chorda(a,
                *(chorda*)xar_obtinere(v.licita, i));
        }
        _attr(s, e, "electio", chorda_aedificator_finire(a), "electio");
    }
    si (v.aut_nullum)
    {
        _attr_literae(s, e, "aut_nullum", "verum");
    }
    si (v.gignens_titulus.mensura > 0)
    {
        _attr(s, e, "gignens", v.gignens_titulus, "gignens");
    }
    alioquin si (v.gignens)
    {
        _recusare(s, "gignens cum functione sine titulo"
                     " (norma_gignens_titulus)");
        redde;
    }
    /* liberi: descriptio primum, licita, typi */
    si (v.descriptio.mensura > 0)
    {
        stml_textum_addere(_elementum(s, e, "descriptio"), s->p, s->in,
            chorda_ut_cstr(v.descriptio, s->p));
    }
    si (v.licita && xar_numerus(v.licita) > 0 && !simplex)
    {
        per (i = 0; i < xar_numerus(v.licita); i++)
        {
            stml_textum_addere(_elementum(s, e, "licitum"), s->p, s->in,
                chorda_ut_cstr(*(chorda*)xar_obtinere(v.licita, i), s->p));
        }
    }
    si (v.genus == NORMA_TABULATUM)
    {
        _nodum_scribere(s, v.elementum, e, profunditas + I, VERUM);
    }
    per (i = 0; v.genus == NORMA_OBJECTUM && i < xar_numerus(v.campi); i++)
    {
        NormaCampus* c  = (NormaCampus*)xar_obtinere(v.campi, i);
          StmlNodus* ce = _elementum(s, e, "campus");

        _attr(s, ce, "titulus", c->titulus, "clavis campi");
        si (!c->requiritur)
        {
            _attr_literae(s, ce, "requiritur", "falsum");
        }
        _nodum_scribere(s, c->valor, ce, profunditas + I, VERUM);
    }
    per (i = 0; v.genus == NORMA_DISCRIMEN
                && i < xar_numerus(v.variationes); i++)
    {
        NormaVariatio* va = (NormaVariatio*)xar_obtinere(v.variationes, i);
            StmlNodus* ve = _elementum(s, e, "variatio");

        _attr(s, ve, "valor", va->valor, "valor variationis");
        /* canon: <variatio> objectum ipsum postulat - numquam <ad> */
        _nodum_scribere(s, va->objectum, ve, profunditas + I, FALSUM);
    }
}

interior b32
_titulus_identitatis (chorda t)
{
    i32 i;

    si (t.mensura == 0)
    {
        redde FALSUM;
    }
    per (i = 0; i < t.mensura; i++)
    {
        character k = (character)t.datum[i];

        si (!(   (k >= 'a' && k <= 'z') || (k >= 'A' && k <= 'Z')
              || (k >= '0' && k <= '9') || k == '_' || k == '-'))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

chorda
norma_stml_scribere (
    constans NormaNominata* normae,
                       i32  numerus,
                  Piscina*  piscina,
                    chorda* causa)
{
     Scriptor  s;
    StmlNodus* radix;
          i32  i;
       chorda  vacua;
       chorda  scriptum;

    vacua.datum    = NIHIL;
    vacua.mensura  = 0;
    memset(&s, 0, magnitudo(s));
    s.p        = piscina;
    s.in       = internamentum_creare(piscina);
    s.normae   = normae;
    s.numerus  = numerus;
    s.causa    = causa;
    si (causa)
    {
        *causa = vacua;
    }
    radix = _elementum(&s, NIHIL, "normae");
    _attr_literae(&s, radix, "versio", "1");
    per (i = 0; i < numerus && !s.fractum; i++)
    {
        StmlNodus* nn;

        si (!_titulus_identitatis(normae[i].titulus))
        {
            _recusare(&s, "titulus normae vacuus aut extra [A-Za-z0-9_-]");
            frange;
        }
        nn = _elementum(&s, radix, "norma");
        _attr(&s, nn, "titulus", _signatum(&s, normae[i].titulus),
            "titulus normae");
        s.radix = i;
        _nodum_scribere(&s, normae[i].norma, nn, 0, VERUM);
    }
    si (s.fractum)
    {
        redde vacua;
    }
    scriptum = stml_scribere(radix, piscina, VERUM);
    si (scriptum.mensura > 0 && scriptum.datum[scriptum.mensura - I] != '\n')
    {
        ChordaAedificator* a = chorda_aedificator_creare(piscina,
            (memoriae_index)scriptum.mensura + II);

        chorda_aedificator_appendere_chorda(a, scriptum);
        chorda_aedificator_appendere_character(a, '\n');
        scriptum = chorda_aedificator_finire(a);
    }
    redde scriptum;
}
```

  (`_spatium` from part A is reused. If the element-level write differs
  from the document-level rewrite used by `_formatum` - e.g. trivia at
  the tail - build the tree under a document node instead (find its
  constructor in `include/stml.h`) and ledger the ruling.)

- [ ] **A4.4 fixture to writer form:** with the A3 fixture still in place,
  run a scratch program (or a temporary `imprimere` in the test) that
  writes `_scribe(norma_stml_legere(FIXA...).normae)` to
  `build/normae_fixae.scriptum`; inspect it by eye against the
  hand-written version (same fields, same order, `<ad>` twice, licitum
  for "cum spatio", floats `0.1` `0.3` `-1e-300` `1e-300`), then `cp` it
  over `probationes/fixa/norma/normae_fixae.norma`. Ruling line: the
  committed fixture is writer output (R5).
- [ ] **A4.5** run the suite. Expected: every A2-A4 assertion passes;
  printed "recusatum:" lines appear only for the deliberate refusals.
- [ ] **A4.6 plants** (compile; each restored):
  (a) writer never emits `requiritur="falsum"` -> oracle I (`s1 ==
  fixa`) and oracle II (`_idem_dicunt`) RED; (b) `_index_normae` always
  returns `s->numerus` (inline shared nodes) -> oracle I and the
  pointer-equality assertion RED; (c) in the READER, `_modus` returns
  `NORMA_CLAUSUM` always -> oracle II RED (`input` is APERTUM).
- [ ] **A4.7** worklog (R5 measurement, R6 + the `true` quirk, variatio
  never `<ad>`, trailing newline); commit `lib/norma_stml.c
  probationes/probatio_norma_stml.c probationes/fixa/norma/normae_fixae.norma
  lib/norma_stml.worklog.md`; gates `radix`, `generata` + main's names.

---
### Task A5: the emitter, bin/norma, oracle III

**Files:** Create `lib/norma_ad_c.c`, `tools/norma.c`,
`tools/norma_struere.sh`, `tools/norma_c_regenerare.sh`,
`probationes/fixa/norma/normae_fixae.h` + `.c` (GENERATUM); extend
`probationes/probatio_norma_stml.c`, `aedificatio.stml`; regenerate the
source list.

**Interfaces:** Produces `norma_ad_c` (A0.4), `bin/norma c|iudicare`.
Consumes `norma_stml_legere` (A3), `NormaNominata`, `norma_visus`.

- [ ] **A5.1 failing tests** - in `probatio_norma_stml.c`, at the top
  after the includes add `#include "norma_ad_c.h"` and
  `#include "fixa/norma/normae_fixae.c"` (the GENERATED fixture; its own
  `#include "normae_fixae.h"` resolves beside it), then:

```c
/* gignens quem fons generatus externum declarat (R7) */
JsonValor*
fixa_norma_gignens_ex_functione (
       Sors* sors,
    Piscina* p,
     vacuum* datum)
{
    redde _ex_functione(sors, p, datum);
}

interior vacuum
probatio_oraculum_tertium(Piscina* p)
{
    NormaGignensNominatum registrum[I];
    chorda fixa = filum_legere_totum(FIXA, p);
    NormaStmlLectio l;
    NormaFonsC fons;
    chorda causa;
    Norma* omnia;
    NormaVisus v;
    NormaVisus a;
    NormaVisus b;

    imprimere("\n--- Oraculum III: emissor contra lectorem ---\n");
    registrum[0].titulus  = "ex_functione";
    registrum[0].functio  = fixa_norma_gignens_ex_functione;
    registrum[0].datum    = NIHIL;
    l = norma_stml_legere(fixa, registrum, I, p);
    CREDO_VERUM(l.successus);
    CREDO_AEQUALIS_I32(_idem_dicunt(norma_stml_quaerere(&l, "usus"),
        fixa_norma_usus(p), p), 0);
    CREDO_AEQUALIS_I32(_idem_dicunt(norma_stml_quaerere(&l, "omnia"),
        fixa_norma_omnia(p), p), 0);
    /* DAG in C servatum */
    omnia = fixa_norma_omnia(p);
    v = norma_visus(omnia);
    {
        Norma* primus = NIHIL;
        Norma* secundus = NIHIL;
        i32 i;

        per (i = 0; i < xar_numerus(v.campi); i++)
        {
            NormaCampus* c = (NormaCampus*)xar_obtinere(v.campi, i);

            si (chorda_aequalis_literis(c->titulus, "usus_primus"))
            {
                primus = c->valor;
            }
            si (chorda_aequalis_literis(c->titulus, "usus_secundus"))
            {
                secundus = c->valor;
            }
        }
        CREDO_VERUM(primus && primus == secundus);
    }
    /* fines fluitantes per litteras C intacti: campus 'minima' (III) */
    a = norma_visus(((NormaCampus*)xar_obtinere(norma_visus(
        norma_stml_quaerere(&l, "usus")).campi, III))->valor);
    b = norma_visus(((NormaCampus*)xar_obtinere(norma_visus(
        fixa_norma_usus(p)).campi, III))->valor);
    CREDO_VERUM(a.minimum_fluitans == b.minimum_fluitans);
    CREDO_VERUM(a.maximum_fluitans == b.maximum_fluitans);
    /* fons commissus recens: emissor hodiernus idem scribit */
    fons = norma_ad_c(_tabula(l.normae, p), xar_numerus(l.normae),
        "fixa_norma_", "normae_fixae.h", FIXA, p, &causa);
    CREDO_CHORDA_AEQUALIS(fons.caput, filum_legere_totum(
        "probationes/fixa/norma/normae_fixae.h", p));
    CREDO_CHORDA_AEQUALIS(fons.corpus, filum_legere_totum(
        "probationes/fixa/norma/normae_fixae.c", p));
}

interior vacuum
probatio_emissor_recusat(Piscina* p)
{
    NormaNominata nn;
    chorda causa;
    character longus_textus[DCCC];

    imprimere("\n--- Probans emissorem recusantem ---\n");
    nn.titulus = chorda_ex_literis("a-b", p);
    nn.norma   = norma_textus(p);
    CREDO_AEQUALIS_I32(norma_ad_c(&nn, I, "x_", "x.h", "x.norma", p,
        &causa).corpus.mensura, 0);
    nn.titulus = chorda_ex_literis("bonum", p);
    memset(longus_textus, 'a', DCCC - I);
    longus_textus[DCCC - I] = '\0';
    nn.norma = norma_descriptio(norma_textus(p), longus_textus);
    CREDO_AEQUALIS_I32(norma_ad_c(&nn, I, "x_", "x.h", "x.norma", p,
        &causa).corpus.mensura, 0);
    CREDO_VERUM(chorda_continet(causa, chorda_ex_literis("DIX", p)));
    nn.norma = norma_intra_fluitans(norma_numerus(p), 0.0, HUGE_VAL);
    CREDO_AEQUALIS_I32(norma_ad_c(&nn, I, "x_", "x.h", "x.norma", p,
        &causa).corpus.mensura, 0);
    nn.norma = norma_gignens(norma_textus(p), _ex_functione, NIHIL);
    CREDO_AEQUALIS_I32(norma_ad_c(&nn, I, "x_", "x.h", "x.norma", p,
        &causa).corpus.mensura, 0);
    /* literae: '?' et '"' et linea nova effugiuntur (trigraphi!) */
    nn.norma = norma_descriptio(norma_textus(p), "a?\?-b \"c\"\nd\\e");
    CREDO_VERUM(chorda_continet(norma_ad_c(&nn, I, "x_", "x.h", "x.norma",
        p, &causa).corpus, chorda_ex_literis(
        "\"a\\?\\?-b \\\"c\\\"\\nd\\\\e\"", p)));
}
```

  (`#include <math.h>` for `HUGE_VAL`.) Call both from `principale`.

- [ ] **A5.2** run. Expected: compile FAIL - `fixa/norma/normae_fixae.c`
  does not exist yet (that is the RED: the oracle needs the emitter).
- [ ] **A5.3 implement** `lib/norma_ad_c.c`:

```c
/* norma_ad_c.c - normae nominatae -> fons C (norma-spec-2 §III.3;
 * norma-plan-3 A5, regula R7). Nodi in tabula 'nodi[]' post-ordine;
 * referentia <ad> = vocatio _struere_<titulus> memorata in 'facta'. */
#include "norma_ad_c.h"
#include "chorda_aedificator.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

nomen structura {
                   Piscina* p;
    constans NormaNominata* normae;
                       i32  numerus;
                       i32  radix;
       constans character* praefixum;
         ChordaAedificator* sententiae;  /* functio currens */
                       Xar* gignentes;   /* chorda, unici */
                       i32  nodi;
                    chorda* causa;
                       b32  fractum;
} Emissor;

interior vacuum
_recusare (
                Emissor* em,
    constans character* nuntius)
{
    si (!em->fractum)
    {
        em->fractum = VERUM;
        si (em->causa)
        {
            *em->causa = chorda_ex_literis(nuntius, em->p);
        }
    }
}

interior b32
_identificator (chorda c)
{
    i32 i;

    si (c.mensura == 0 || (c.datum[0] >= '0' && c.datum[0] <= '9'))
    {
        redde FALSUM;
    }
    per (i = 0; i < c.mensura; i++)
    {
        character k = (character)c.datum[i];

        si (!(   (k >= 'a' && k <= 'z') || (k >= 'A' && k <= 'Z')
              || (k >= '0' && k <= '9') || k == '_'))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* litterae C: effugia " \ ? (trigraphi) et moderatores; > DIX recusat */
interior vacuum
_literae (
               Emissor* em,
    ChordaAedificator* a,
                chorda  c)
{
          i32  i;
    character  octalis[VIII];

    si (c.mensura > DIX)
    {
        _recusare(em, "textus longior quam DIX octeti (C89)");
        redde;
    }
    chorda_aedificator_appendere_character(a, '"');
    per (i = 0; i < c.mensura; i++)
    {
        insignatus character k = (insignatus character)c.datum[i];

        si (k == '"' || k == '\\' || k == '?')
        {
            chorda_aedificator_appendere_character(a, '\\');
            chorda_aedificator_appendere_character(a, (character)k);
        }
        alioquin si (k == '\n')
        {
            chorda_aedificator_appendere_literis(a, "\\n");
        }
        alioquin si (k < 0x20 || k == 0x7F)
        {
            sprintf(octalis, "\\%03o", (insignatus integer)k);
            chorda_aedificator_appendere_literis(a, octalis);
        }
        alioquin
        {
            chorda_aedificator_appendere_character(a, (character)k);
        }
    }
    chorda_aedificator_appendere_character(a, '"');
}

interior vacuum
_sententia (
               Emissor* em,
    constans character* textus)
{
    chorda_aedificator_appendere_literis(em->sententiae, textus);
}

interior vacuum
_nodus_nominatus (
    Emissor* em,
         i32  i)
{
    character b[XXXII];

    sprintf(b, "nodi[%u]", (insignatus integer)i);
    _sententia(em, b);
}

/* "    norma_X(nodi[i]" - vocatio modificatoris aperta */
interior vacuum
_modificator (
               Emissor* em,
    constans character* functio,
                    i32  i)
{
    _sententia(em, "    ");
    _sententia(em, functio);
    _sententia(em, "(");
    _nodus_nominatus(em, i);
}

interior vacuum
_s64 (
    Emissor* em,
         s64  v)
{
    character b[XLVIII];

    si (v == (s64)LONG_MIN)
    {
        _sententia(em, "(s64)(-9223372036854775807L - 1L)");
        redde;
    }
    sprintf(b, "(s64)%ldL", (longus)v);
    _sententia(em, b);
}

interior vacuum
_f64 (
    Emissor* em,
         f64  v)
{
    character b[XLVIII];
          i32  praecisio;

    si (v != v || v - v != 0.0)
    {
        _recusare(em, "numerus fluitans non finitus");
        redde;
    }
    per (praecisio = I; praecisio <= XVII; praecisio++)
    {
        sprintf(b, "%.*g", (integer)praecisio, v);
        si (strtod(b, NIHIL) == v)
        {
            frange;
        }
    }
    si (!strchr(b, '.') && !strchr(b, 'e'))
    {
        strcat(b, ".0");
    }
    _sententia(em, b);
}

interior i32
_index_normae (
           Emissor* em,
    constans Norma* n)
{
    i32 i;

    per (i = 0; i < em->numerus; i++)
    {
        si (em->normae[i].norma == n)
        {
            redde i;
        }
    }
    redde em->numerus;
}

interior constans character*
_constructor (NormaGenus g)
{
    commutatio (g)
    {
        casus NORMA_NULLUM:    redde "norma_nullum";
        casus NORMA_BOOLEAN:   redde "norma_boolean";
        casus NORMA_INTEGER:   redde "norma_integer";
        casus NORMA_NUMERUS:   redde "norma_numerus";
        casus NORMA_TEXTUS:    redde "norma_textus";
        casus NORMA_OBJECTUM:  redde "norma_objectum";
        ordinarius:            redde "norma_liberum";
    }
}

interior vacuum
_gignens_notare (
    Emissor* em,
     chorda  titulus)
{
    i32 i;

    per (i = 0; i < xar_numerus(em->gignentes); i++)
    {
        si (chorda_aequalis(*(chorda*)xar_obtinere(em->gignentes, i),
                titulus))
        {
            redde;
        }
    }
    *(chorda*)xar_addere(em->gignentes) = titulus;
}

/* sententias nodi emittit (post-ordine); reddit indicem in nodi[] */
interior i32
_nodus (
            Emissor* em,
    constans Norma* n,
                i32  profunditas)
{
    NormaVisus v;
          i32  k;
          i32  me;
          i32  i;
          i32  f;

    si (em->fractum)
    {
        redde 0;
    }
    si (profunditas > CXXVIII)
    {
        _recusare(em, "profunditas > CXXVIII: circulus aedificatorum?");
        redde 0;
    }
    k = _index_normae(em, n);
    si (k < em->numerus && profunditas > 0)
    {
        si (k == em->radix)
        {
            _recusare(em, "circulus: norma se ipsam continet");
            redde 0;
        }
        me = em->nodi++;
        _sententia(em, "    ");
        _nodus_nominatus(em, me);
        _sententia(em, " = _struere_");
        chorda_aedificator_appendere_chorda(em->sententiae,
            em->normae[k].titulus);
        _sententia(em, "(piscina, facta);\n");
        redde me;
    }
    v = norma_visus(n);
    si (!n || v.error_schematis.mensura > 0)
    {
        _recusare(em, "nodus pravus");
        redde 0;
    }
    si (v.genus == NORMA_TABULATUM)
    {
        f   = _nodus(em, v.elementum, profunditas + I);
        me  = em->nodi++;
        _sententia(em, "    ");
        _nodus_nominatus(em, me);
        _sententia(em, " = norma_tabulatum(piscina, ");
        _nodus_nominatus(em, f);
        _sententia(em, ");\n");
    }
    alioquin si (v.genus == NORMA_DISCRIMEN)
    {
        me = em->nodi++;
        _sententia(em, "    ");
        _nodus_nominatus(em, me);
        _sententia(em, " = norma_discrimen(piscina, ");
        _literae(em, em->sententiae, v.clavis_discriminis);
        _sententia(em, ");\n");
    }
    alioquin
    {
        me = em->nodi++;
        _sententia(em, "    ");
        _nodus_nominatus(em, me);
        _sententia(em, " = ");
        _sententia(em, _constructor(v.genus));
        _sententia(em, "(piscina);\n");
    }
    per (i = 0; v.genus == NORMA_OBJECTUM && i < xar_numerus(v.campi); i++)
    {
        NormaCampus* c = (NormaCampus*)xar_obtinere(v.campi, i);

        f = _nodus(em, c->valor, profunditas + I);
        _modificator(em, "norma_campus", me);
        _sententia(em, ", ");
        _literae(em, em->sententiae, c->titulus);
        _sententia(em, ", ");
        _nodus_nominatus(em, f);
        _sententia(em, c->requiritur ? ", VERUM);\n" : ", FALSUM);\n");
    }
    per (i = 0; v.genus == NORMA_DISCRIMEN
                && i < xar_numerus(v.variationes); i++)
    {
        NormaVariatio* va = (NormaVariatio*)xar_obtinere(v.variationes, i);

        f = _nodus(em, va->objectum, profunditas + I);
        _modificator(em, "norma_variatio", me);
        _sententia(em, ", ");
        _literae(em, em->sententiae, va->valor);
        _sententia(em, ", ");
        _nodus_nominatus(em, f);
        _sententia(em, ");\n");
    }
    si (   (v.genus == NORMA_OBJECTUM || v.genus == NORMA_DISCRIMEN)
        && v.modus != NORMA_CLAUSUM)
    {
        _modificator(em, "norma_modus", me);
        _sententia(em, v.modus == NORMA_APERTUM ? ", NORMA_APERTUM);\n"
                                                : ", NORMA_NOTANDUM);\n");
    }
    si (v.habet_intra)
    {
        _modificator(em, "norma_intra", me);
        _sententia(em, ", ");
        _s64(em, v.minimum);
        _sententia(em, ", ");
        _s64(em, v.maximum);
        _sententia(em, ");\n");
    }
    si (v.habet_intra_fluitans)
    {
        _modificator(em, "norma_intra_fluitans", me);
        _sententia(em, ", ");
        _f64(em, v.minimum_fluitans);
        _sententia(em, ", ");
        _f64(em, v.maximum_fluitans);
        _sententia(em, ");\n");
    }
    si (v.habet_longitudinem)
    {
        character b[XLVIII];

        _modificator(em, "norma_longitudo", me);
        sprintf(b, ", (i32)%luUL, (i32)%luUL);\n",
            (insignatus longus)v.longitudo_minima,
            (insignatus longus)v.longitudo_maxima);
        _sententia(em, b);
    }
    si (v.forma.mensura > 0)
    {
        _modificator(em, "norma_forma", me);
        _sententia(em, ", ");
        _literae(em, em->sententiae, v.forma);
        _sententia(em, ");\n");
    }
    si (v.licita && xar_numerus(v.licita) > 0)
    {
        _sententia(em, "    {\n        constans character* constans"
                       " licita[] = { ");
        per (i = 0; i < xar_numerus(v.licita); i++)
        {
            _literae(em, em->sententiae, *(chorda*)xar_obtinere(v.licita,
                i));
            _sententia(em, ", ");
        }
        _sententia(em, "NIHIL };\n\n    ");
        _modificator(em, "norma_electio", me);
        _sententia(em, ", licita);\n    }\n");
    }
    si (v.descriptio.mensura > 0)
    {
        _modificator(em, "norma_descriptio", me);
        _sententia(em, ", ");
        _literae(em, em->sententiae, v.descriptio);
        _sententia(em, ");\n");
    }
    si (v.gignens_titulus.mensura > 0)
    {
        si (!_identificator(v.gignens_titulus))
        {
            _recusare(em, "gignens non identificator C");
            redde 0;
        }
        _gignens_notare(em, v.gignens_titulus);
        _modificator(em, "norma_gignens", me);
        _sententia(em, ", ");
        _sententia(em, em->praefixum);
        _sententia(em, "gignens_");
        chorda_aedificator_appendere_chorda(em->sententiae,
            v.gignens_titulus);
        _sententia(em, ", NIHIL);\n");
        _modificator(em, "norma_gignens_titulus", me);
        _sententia(em, ", ");
        _literae(em, em->sententiae, v.gignens_titulus);
        _sententia(em, ");\n");
    }
    alioquin si (v.gignens)
    {
        _recusare(em, "gignens cum functione sine titulo");
        redde 0;
    }
    si (v.aut_nullum)
    {
        _modificator(em, "norma_aut_nullum", me);
        _sententia(em, ");\n");
    }
    redde me;
}

interior chorda
_custos (
    constans character* caput_titulus,
               Piscina* p)
{
    chorda c = chorda_ex_literis(caput_titulus, p);
       i32 i;

    per (i = 0; i < c.mensura; i++)
    {
        character k = (character)c.datum[i];

        si (k >= 'a' && k <= 'z')
        {
            c.datum[i] = (i8)(k - 'a' + 'A');
        }
        alioquin si (!((k >= 'A' && k <= 'Z') || (k >= '0' && k <= '9')))
        {
            c.datum[i] = '_';
        }
    }
    redde c;
}

NormaFonsC
norma_ad_c (
    constans NormaNominata* normae,
                       i32  numerus,
       constans character* praefixum,
       constans character* caput_titulus,
       constans character* fons_titulus,
                  Piscina* piscina,
                   chorda* causa)
{
              Emissor  em;
           NormaFonsC  fons;
    ChordaAedificator* caput   = chorda_aedificator_creare(piscina, M);
    ChordaAedificator* corpus  = chorda_aedificator_creare(piscina, M * IV);
    ChordaAedificator* functiones = chorda_aedificator_creare(piscina,
                                        M * IV);
                  i32  i;
            character  b[CXXVIII];
               chorda  custos  = _custos(caput_titulus, piscina);

    memset(&em, 0, magnitudo(em));
    memset(&fons, 0, magnitudo(fons));
    em.p          = piscina;
    em.normae     = normae;
    em.numerus    = numerus;
    em.praefixum  = praefixum;
    em.causa      = causa;
    em.gignentes  = xar_creare(piscina, (i32)magnitudo(chorda));
    si (causa)
    {
        causa->datum    = NIHIL;
        causa->mensura  = 0;
    }
    si (   !_identificator(chorda_ex_literis(praefixum, piscina))
        || strstr(fons_titulus, "*/"))
    {
        _recusare(&em, "praefixum non identificator aut fons cum '*/'");
        redde fons;
    }
    /* functiones _struere_<titulus> */
    per (i = 0; i < numerus && !em.fractum; i++)
    {
        ChordaAedificator* sententiae = chorda_aedificator_creare(piscina,
                                            M);
                      i32  radix_nodi;

        si (!_identificator(normae[i].titulus))
        {
            _recusare(&em, "titulus normae non identificator C");
            frange;
        }
        em.sententiae  = sententiae;
        em.radix       = i;
        em.nodi        = 0;
        radix_nodi     = _nodus(&em, normae[i].norma, 0);
        chorda_aedificator_appendere_literis(functiones,
            "\ninterior Norma*\n_struere_");
        chorda_aedificator_appendere_chorda(functiones, normae[i].titulus);
        sprintf(b, " (\n    Piscina* piscina,\n     Norma** facta)\n{\n"
            "    Norma* nodi[%u];\n\n    si (facta[%u])\n    {\n"
            "        redde facta[%u];\n    }\n",
            (insignatus integer)em.nodi, (insignatus integer)i,
            (insignatus integer)i);
        chorda_aedificator_appendere_literis(functiones, b);
        chorda_aedificator_appendere_chorda(functiones,
            chorda_aedificator_finire(sententiae));
        sprintf(b, "    facta[%u] = nodi[%u];\n    redde nodi[%u];\n}\n",
            (insignatus integer)i, (insignatus integer)radix_nodi,
            (insignatus integer)radix_nodi);
        chorda_aedificator_appendere_literis(functiones, b);
    }
    si (em.fractum)
    {
        redde fons;
    }
    /* caput */
    sprintf(b, "/* GENERATUM a bin/norma c ex %.80s - noli manu mutare */\n",
        fons_titulus);
    chorda_aedificator_appendere_literis(caput, b);
    chorda_aedificator_appendere_literis(caput, "#ifndef ");
    chorda_aedificator_appendere_chorda(caput, custos);
    chorda_aedificator_appendere_literis(caput, "\n#define ");
    chorda_aedificator_appendere_chorda(caput, custos);
    chorda_aedificator_appendere_literis(caput, "\n\n#include \"latina.h\"\n"
        "#include \"piscina.h\"\n#include \"norma.h\"\n\n");
    per (i = 0; i < numerus; i++)
    {
        chorda_aedificator_appendere_literis(caput, "Norma*\n");
        chorda_aedificator_appendere_literis(caput, praefixum);
        chorda_aedificator_appendere_chorda(caput, normae[i].titulus);
        chorda_aedificator_appendere_literis(caput,
            " (\n    Piscina* piscina);\n\n");
    }
    per (i = 0; i < xar_numerus(em.gignentes); i++)
    {
        chorda_aedificator_appendere_literis(caput,
            "/* vocans definit (norma-spec-2 §III.3) */\nJsonValor*\n");
        chorda_aedificator_appendere_literis(caput, praefixum);
        chorda_aedificator_appendere_literis(caput, "gignens_");
        chorda_aedificator_appendere_chorda(caput,
            *(chorda*)xar_obtinere(em.gignentes, i));
        chorda_aedificator_appendere_literis(caput,
            " (\n       Sors* sors,\n    Piscina* piscina,\n"
            "     vacuum* datum);\n\n");
    }
    chorda_aedificator_appendere_literis(caput, "#endif\n");
    /* corpus */
    sprintf(b, "/* GENERATUM a bin/norma c ex %.80s - noli manu mutare */\n",
        fons_titulus);
    chorda_aedificator_appendere_literis(corpus, b);
    chorda_aedificator_appendere_literis(corpus, "#include \"");
    chorda_aedificator_appendere_literis(corpus, caput_titulus);
    chorda_aedificator_appendere_literis(corpus, "\"\n\n");
    per (i = 0; i < numerus; i++)
    {
        chorda_aedificator_appendere_literis(corpus,
            "interior Norma* _struere_");
        chorda_aedificator_appendere_chorda(corpus, normae[i].titulus);
        chorda_aedificator_appendere_literis(corpus,
            " (Piscina* piscina, Norma** facta);\n");
    }
    chorda_aedificator_appendere_chorda(corpus,
        chorda_aedificator_finire(functiones));
    per (i = 0; i < numerus; i++)
    {
        chorda_aedificator_appendere_literis(corpus, "\nNorma*\n");
        chorda_aedificator_appendere_literis(corpus, praefixum);
        chorda_aedificator_appendere_chorda(corpus, normae[i].titulus);
        sprintf(b, " (\n    Piscina* piscina)\n{\n    Norma* facta[%u];\n"
            "      i32  i;\n\n    per (i = 0; i < %u; i++)\n    {\n"
            "        facta[i] = NIHIL;\n    }\n    redde _struere_",
            (insignatus integer)numerus, (insignatus integer)numerus);
        chorda_aedificator_appendere_literis(corpus, b);
        chorda_aedificator_appendere_chorda(corpus, normae[i].titulus);
        chorda_aedificator_appendere_literis(corpus, "(piscina, facta);\n}\n");
    }
    fons.caput   = chorda_aedificator_finire(caput);
    fons.corpus  = chorda_aedificator_finire(corpus);
    redde fons;
}
```

  Needs `#include <limits.h>` (LONG_MIN).

- [ ] **A5.4** `tools/norma.c`:

```c
/* norma.c (instrumentum) - plagulae .norma (norma-spec-2 §III.3)
 *
 * Verba:
 *   norma c <x.norma> -praefixum P -caput <h> -corpus <c>
 *   norma iudicare <x.norma> <valor.json> [-norma <titulus>]
 * Exitus: 0 bene / validum; 1 invalidum (iudicare); 2 usus, lectio
 * fracta, recusatio (nuntius in stderr, via:linea:columna). */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include "json.h"
#include "norma.h"
#include "norma_stml.h"
#include "norma_ad_c.h"

#include <stdio.h>
#include <string.h>

interior s32
_usus (vacuum)
{
    fputs("usus: norma c <x.norma> -praefixum P -caput <h> -corpus <c>\n"
          "      norma iudicare <x.norma> <valor.json> [-norma <titulus>]\n",
          stderr);
    redde II;
}

interior b32
_legere (
    constans character* via,
       NormaStmlLectio* l,
               Piscina* p)
{
    chorda fons = filum_legere_totum(via, p);
       i32 i;

    si (fons.mensura == 0)
    {
        fprintf(stderr, "norma: %s legi non potuit\n", via);
        redde FALSUM;
    }
    *l = norma_stml_legere(fons, NIHIL, 0, p);
    per (i = 0; i < xar_numerus(l->vitia); i++)
    {
        NormaStmlVitium* v = (NormaStmlVitium*)xar_obtinere(l->vitia, i);

        fprintf(stderr, "%s:%u:%u: %.*s\n", via, (insignatus integer)v->linea,
            (insignatus integer)v->columna, (integer)v->nuntius.mensura,
            (constans character*)v->nuntius.datum);
    }
    redde l->successus;
}

interior NormaNominata*
_tabula (
        Xar* x,
    Piscina* p)
{
    NormaNominata* t = (NormaNominata*)piscina_allocare(p,
        (memoriae_index)((xar_numerus(x) + I) * magnitudo(NormaNominata)));

    xar_copiare_ad_tabulam(x, t, 0, xar_numerus(x));
    redde t;
}

interior s32
_c (
       integer  argc,
    character** argv,
       Piscina* p)
{
    constans character* praefixum = NIHIL;
    constans character* caput = NIHIL;
    constans character* corpus = NIHIL;
    constans character* nomen_capitis;
        NormaStmlLectio  l;
             NormaFonsC  fons;
                 chorda  causa;
                integer  i;

    per (i = III; i + I < argc; i += II)
    {
        si (strcmp(argv[i], "-praefixum") == 0) praefixum = argv[i + I];
        alioquin si (strcmp(argv[i], "-caput") == 0) caput = argv[i + I];
        alioquin si (strcmp(argv[i], "-corpus") == 0) corpus = argv[i + I];
        alioquin redde _usus();
    }
    si (argc < III || !praefixum || !caput || !corpus)
    {
        redde _usus();
    }
    si (!_legere(argv[II], &l, p))
    {
        redde II;
    }
    nomen_capitis = strrchr(caput, '/') ? strrchr(caput, '/') + I : caput;
    fons = norma_ad_c(_tabula(l.normae, p), xar_numerus(l.normae),
        praefixum, nomen_capitis, argv[II], p, &causa);
    si (fons.corpus.mensura == 0)
    {
        fprintf(stderr, "norma c: %.*s\n", (integer)causa.mensura,
            (constans character*)causa.datum);
        redde II;
    }
    si (!filum_scribere(caput, fons.caput) || !filum_scribere(corpus,
            fons.corpus))
    {
        fprintf(stderr, "norma c: scribere non potuit\n");
        redde II;
    }
    redde 0;
}

interior s32
_iudicare (
       integer  argc,
    character** argv,
       Piscina* p)
{
    constans character* titulus = NIHIL;
        NormaStmlLectio  l;
           JsonResultus  j;
                 Norma* n;
          NormaIudicium  iud;
                    i32  i;

    si (argc < IV)
    {
        redde _usus();
    }
    si (argc >= VI && strcmp(argv[IV], "-norma") == 0)
    {
        titulus = argv[V];
    }
    si (!_legere(argv[II], &l, p))
    {
        redde II;
    }
    n = titulus ? norma_stml_quaerere(&l, titulus)
        : (xar_numerus(l.normae) > 0
           ? ((NormaNominata*)xar_obtinere(l.normae, 0))->norma : NIHIL);
    j = json_legere(filum_legere_totum(argv[III], p), p);
    si (!n || !j.successus)
    {
        fprintf(stderr, "norma iudicare: %s\n", !n ? "norma abest"
            : "valor JSON fractus");
        redde II;
    }
    iud = norma_iudicare(n, j.radix, p);
    per (i = 0; i < xar_numerus(iud.vitia) + xar_numerus(iud.notae); i++)
    {
        b32 vit = i < xar_numerus(iud.vitia);
        NormaVitium* v = (NormaVitium*)xar_obtinere(vit ? iud.vitia
            : iud.notae, vit ? i : i - xar_numerus(iud.vitia));

        printf("%s %.*s %s: %.*s\n", vit ? "VITIUM" : "NOTA",
            (integer)v->via.mensura, (constans character*)v->via.datum,
            norma_causa_descriptio(v->causa), (integer)v->nuntius.mensura,
            (constans character*)v->nuntius.datum);
    }
    redde iud.validum ? 0 : I;
}

s32
principale (
       integer  argc,
    character** argv)
{
    Piscina* p = piscina_generare_dynamicum("norma", M * M);
         s32  exitus;

    si (argc < II)
    {
        exitus = _usus();
    }
    alioquin si (strcmp(argv[I], "c") == 0)
    {
        exitus = _c(argc, argv, p);
    }
    alioquin si (strcmp(argv[I], "iudicare") == 0)
    {
        exitus = _iudicare(argc, argv, p);
    }
    alioquin
    {
        exitus = _usus();
    }
    piscina_destruere(p);
    redde exitus;
}
```

- [ ] **A5.5** `tools/norma_struere.sh` and `tools/norma_c_regenerare.sh`:

```bash
#!/bin/bash
# tools/norma_struere.sh - bin/norma per aedilis (regula R8 norma-plan-3)
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/.." || exit 2
[ -x bin/aedilis ] || { echo "norma_struere: bin/aedilis deest" >&2; exit 2; }
./bin/aedilis tools/norma.c > /dev/null || exit 2
bash build/aedilis/norma/struere.sh > /dev/null || exit 2
cp build/aedilis/norma/norma bin/norma || exit 2
```

```bash
#!/bin/bash
# tools/norma_c_regenerare.sh <x.norma> <praefixum> <caput.h> <corpus.c>
# bin/norma c; sub FABRICA_SCRIPTURA exitus in scripturam iudicis
# (viae eaedem), arbor intacta.
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/.." || exit 2
[ $# -eq 4 ] || { echo "usus: norma_c_regenerare.sh <x.norma> <praefixum> <caput.h> <corpus.c>" >&2; exit 2; }
./tools/norma_struere.sh || exit 2
R="${FABRICA_SCRIPTURA:-.}"
mkdir -p "$R/$(dirname "$3")" "$R/$(dirname "$4")" || exit 2
exec bin/norma c "$1" -praefixum "$2" -caput "$R/$3" -corpus "$R/$4"
```

  `chmod +x` both. If fabrica's judge objects to the side effect of
  rebuilding `bin/norma` from inside a judged generator, switch to the
  `canon_coquere.sh` stance (refuse with exit 2 and name the build
  command when `bin/norma` is older than its sources) and ledger it.

- [ ] **A5.6** generate the fixture C:
  `./tools/norma_c_regenerare.sh probationes/fixa/norma/normae_fixae.norma fixa_norma_ probationes/fixa/norma/normae_fixae.h probationes/fixa/norma/normae_fixae.c`.
  Expected: exit 0; both files start with `/* GENERATUM`; compile-check
  the .c alone in a scratch TU with the vexilla flags (`-Iinclude
  -Iprobationes/fixa/norma`). Read it by eye: one `_struere_usus`, one
  `_struere_omnia` with `nodi[k] = _struere_usus(piscina, facta)` twice.
- [ ] **A5.7** `aedificatio.stml` - generator action:

```xml
  <actio titulus="normae_fixae" genus="generator">
    <mandatum>
      <verbum! (>./tools/norma_c_regenerare.sh
      <verbum! (>probationes/fixa/norma/normae_fixae.norma
      <verbum! (>fixa_norma_
      <verbum! (>probationes/fixa/norma/normae_fixae.h
      <verbum! (>probationes/fixa/norma/normae_fixae.c
    </mandatum>
    <ingressus genus="fasciculus" via="tools/norma_c_regenerare.sh"/>
    <ingressus genus="fasciculus" via="tools/norma_struere.sh"/>
    <ingressus genus="manifestum" via="build/aedilis/norma/manifestum.stml"/>
    <ingressus genus="fasciculus" via="probationes/fixa/norma/normae_fixae.norma"/>
    <exitus via="probationes/fixa/norma/normae_fixae.h" provenientia="regeneratio"/>
    <exitus via="probationes/fixa/norma/normae_fixae.c" provenientia="regeneratio"/>
  </actio>
```

  `./tools/compile_tests_fontes_generare.sh`; `bin/fabrica iudicare
  -plenus -omnia > build/a5.fabrica.log 2>&1; tail build/a5.fabrica.log`.
  Expected: `normae_fixae` fresh.
- [ ] **A5.8** run the suite. Expected: oracle III and the refusal tests
  pass. Also by hand: `bin/norma iudicare probationes/fixa/norma/normae_fixae.norma <(echo '{"input_tokens":1}') -norma usus`
  -> one VITIUM (`ratio` missing), exit 1.
- [ ] **A5.9 plants:** (a) emitter swaps minimum/maximum in `norma_intra`
  -> regenerate the fixture C -> oracle III RED (and the core marks the
  node pravus: SCHEMA_PRAVA judgments differ); (b) emitter drops the
  `facta` memo (`si (facta[k])` block) -> pointer-equality RED;
  (c) change one byte of the committed `.c` without regenerating ->
  the in-test freshness assertion RED. Restore, regenerate, green.
- [ ] **A5.10** worklog (`lib/norma_ad_c.worklog.md`: R7, R8, trigraph
  escape, header holds the gignens prototypes); commit
  `lib/norma_ad_c.c include/norma_ad_c.h tools/norma.c
  tools/norma_struere.sh tools/norma_c_regenerare.sh
  probationes/fixa/norma/normae_fixae.h probationes/fixa/norma/normae_fixae.c
  probationes/probatio_norma_stml.c aedificatio.stml
  lib/norma_ad_c.worklog.md` + source list; gates `radix`, `generata` +
  main's names.

---
### Task A6: vates' response schema moves to `.norma`

**Files:** Create `lib/vates_responsum.norma`,
`include/vates_responsum_norma.h` + `lib/vates_responsum_norma.c`
(GENERATUM); modify `lib/vates.c` (delete `_forma_responsi`),
`probationes/probatio_vates.c`, `aedificatio.stml`,
`lib/vates.worklog.md`; regenerate the source list.

**Interfaces:** Produces `vates_norma_responsum(Piscina*)` (plus
`vates_norma_usus`, `vates_norma_blocus`). Consumes `bin/norma c` (A5),
`norma_stml_scribere` (A4).

- [ ] **A6.1 failing test** - `probationes/probatio_vates.c`: add
  `#include "norma_gignere.h"`, `#include "norma_stml.h"`,
  `#include "vates_responsum_norma.h"`, `#include "internamentum.h"`,
  `#include "stml.h"`, a VERBATIM copy of today's `_forma_responsi`
  (`lib/vates.c:528-582`) renamed `_forma_responsi_vetus`, the
  `_iudicia_aequalia` helper from A4.1, and:

```c
/* norma-plan-3 A6: schema ex .norma generatum idem dicit ac aedificator
 * vetus (exportatio + iudicia super valores ab utroque genitos) */
interior vacuum
probatio_forma_ex_norma(Piscina* p)
{
    Norma* vetus = _forma_responsi_vetus(p);
    Norma* nova  = vates_norma_responsum(p);
    i32 diversa  = 0;
    s64 semen;
    i32 m;
    i32 q;
    chorda fons = filum_legere_totum("lib/vates_responsum.norma", p);
    StmlResultus r = stml_legere(fons, p, internamentum_creare(p));

    imprimere("\n--- Probans formam responsi ex .norma ---\n");
    CREDO_CHORDA_AEQUALIS(json_scribere(norma_json_schema(vetus, p), p),
                          json_scribere(norma_json_schema(nova, p), p));
    per (q = 0; q < II; q++)
    {
        per (semen = 0; semen < L; semen++)
        {
            per (m = (i32)NORMA_TYPICA; m <= (i32)NORMA_INVALIDA; m++)
            {
                NormaGenitum g = norma_gignere(q == 0 ? vetus : nova,
                    (NormaModusGignendi)m, semen, p);

                si (g.valor && !_iudicia_aequalia(norma_iudicare(vetus,
                        g.valor, p), norma_iudicare(nova, g.valor, p)))
                {
                    diversa++;
                }
            }
        }
    }
    CREDO_AEQUALIS_I32(diversa, 0);
    /* oraculum V: plagula forma pulchra */
    CREDO_VERUM(r.successus);
    CREDO_CHORDA_AEQUALIS(stml_scribere(r.radix, p, VERUM), fons);
    /* lector eam sine vitio legit, sine notis (nullus gignens) */
    CREDO_VERUM(norma_stml_legere(fons, NIHIL, 0, p).successus);
}
```

  Call it from `principale`. Run `./compile_tests.sh probatio_vates`.
  Expected: compile FAIL - `vates_responsum_norma.h` not found.

- [ ] **A6.2 write the `.norma` FROM the old builder** (equivalence by
  construction): a scratch program `build/vates_norma_scribere.c` with a
  copy of `_forma_responsi` changed to also hand back its `usus` and
  `blocus` nodes, calling `norma_stml_scribere` on
  `{ "responsum": r, "usus": usus, "blocus": blocus }` (that order) and
  `filum_scribere("lib/vates_responsum.norma", ...)`. Build with
  `./bin/aedilis build/vates_norma_scribere.c` + its `struere.sh`; run;
  delete the scratch files. Read the result: `#responsum` with
  `<campus titulus="usage" (> <ad norma="#usus"/>` and
  `<tabulatum(> <ad norma="#blocus"/>`; every object `modus="notandum"`.
  Then prepend NOTHING by hand (the writer's form is canonical - R5).
- [ ] **A6.3** generate: `./tools/norma_c_regenerare.sh lib/vates_responsum.norma vates_norma_ include/vates_responsum_norma.h lib/vates_responsum_norma.c`.
  Expected: exit 0, both GENERATUM.
- [ ] **A6.4** `aedificatio.stml` - action `vates_responsum`, the same
  shape as `normae_fixae` (A5.7) with these paths and praefixum
  `vates_norma_`. `./tools/compile_tests_fontes_generare.sh`.
- [ ] **A6.5** `lib/vates.c`: `#include "vates_responsum_norma.h"`;
  `_novitates_ex_norma` calls `vates_norma_responsum(p)`; delete
  `_forma_responsi`. Run `./compile_tests.sh probatio_vates` and
  `./compile_tests.sh probatio_norma`. Expected: all green - vates' 106
  prior assertions, the specimen novelty test (0 new sightings), and
  A6.1.
- [ ] **A6.6 plants:** (a) in `lib/vates_responsum.norma` drop
  `requiritur="falsum"` from `stop_sequence`, regenerate -> A6.1 RED
  (samples without `stop_sequence` judged differently) and the specimen
  test RED if a specimen lacks it; (b) edit one byte of
  `lib/vates_responsum_norma.c` -> `./tools/generata_iudicare.sh` RED.
  Restore both (plain `cp`), regenerate, green.
- [ ] **A6.7** worklog (`lib/vates.worklog.md`: schema source moved,
  how the file was produced, old builder kept only as a test oracle);
  commit `lib/vates_responsum.norma include/vates_responsum_norma.h
  lib/vates_responsum_norma.c lib/vates.c probationes/probatio_vates.c
  aedificatio.stml lib/vates.worklog.md` + source list; gates `radix`,
  `generata` + main's names. Ledger: note on …MPPK82 (slice A done,
  commit range); slice B stays open there.

---

## Finish

- Whole-branch self-review against the spec (no subagents: Fran's
  standing rule) - `Final review: self-review` in the ledger
  `build/norma-plan-3.progress.md`, rulings and deferred minors listed in
  the final message.
- Spec §VI "Done means" checked line by line; `norma-spec-2.md` gets an
  "As built" note for R1-R8 where the build departed from the sketch.
- Merge back to main per the tertia procedure (frigida_probare, then ff)
  only when Fran asks.

## AUDIENDA (not verified while planning)

- canon `intra=` scoping on NESTED objecta (A2.1 has the fallback).
- that a quoted `#include "fixa/norma/normae_fixae.c"` inside a probatio
  passes aedilis/compile_tests without a closure complaint (A5.2 shows).
- that fabrica's judge tolerates `norma_struere.sh` rebuilding
  `bin/norma` during judging (A5.5 has the fallback).
- that element-level `stml_scribere` equals the document-level rewrite
  `_formatum` uses (A4.3 has the fallback).
- `LONG_MIN/LONG_MAX` as the s64 range assumes LP64 (macOS only, per
  CLAUDE.md); ledgered in A3.
