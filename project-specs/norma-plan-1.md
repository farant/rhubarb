# norma plan 1 - the three headers, for review

> **For agentic workers:** execution mode FIXED by Fran's standing rule:
> inline, one task per turn, Fran approves each; no subagents.

**Goal:** put `include/fictio.h`, `include/norma.h`, `include/norma_gignere.h`
in front of Fran before any implementation (Eskil's rule). Plan 2 (N1-N5)
is written against the approved headers.

**Spec:** `project-specs/norma-spec.md` (4890a056). Worktree `../rhubarb-quarta`.

## Global constraints

C89 + latina.h; flags `tools/vexilla.sh`; Latin identifiers, NO latina.h
macro word as an identifier (compile-check every header in a TU: examen
alone misses it, quaestio …2Y6MT); no `_Capital`; `chorda` not
NUL-terminated; `i32` unsigned, `s32`/`s64` signed; worktree commit guard
(absolute path + assert); gates by hand + `sine_debitis`.

## Decisions made while drafting (beyond the spec; Fran reviews them)

1. **`NormaVisus`** - read-only view of an opaque node (the
   `http_petitio_visus` precedent): the generator needs to walk a schema,
   and docs / import will later. The node stays opaque; v2 adds functions.
2. `norma_aut_nullum` (not `nullabile`: not Latin) and
   `norma_intra_fluitans` (not `_f`).
3. `fictio_difficilia_numerus` + `fictio_difficile(i)`: exhaustive,
   ordered access to the difficult strings for tests, besides the seeded
   `fictio_textus_difficilis`.
5. NormaCausa constants carry the prefix `NORMA_CAUSA_` (C enum constants share ONE namespace: `NORMA_DISCRIMEN` was both a genus and a causa - caught by the N0 compile check; spec amended); `quodvis` -> `liberum` (real Latin form: no glossary entry, per the house rule).
4. `NormaGenitum.valor == NIHIL` means "nothing violable" in INVALIDA
   (no extra flag); `via_fracta` is then empty.

### Task N0: draft, compile-check, lint, STOP for review

- [ ] Write `include/fictio.h`:

```c
/* fictio.h - valores ficti sine JSON (norma-spec par. IV)
 *
 * Nomina, email, uuid, tempora, Latina VERA (numquam lorem ipsum),
 * numeri et textus intra fines, scrinium textuum difficilium. Utilis
 * extra JSON quoque (UI ficti, fixa STML, data probationum C).
 *
 * Omnis functio Sors VOCANTIS accipit: semen idem -> octeti idem.
 *
 * STATUS: PROPOSITUM (norma-plan-1 N0) - a Frano probandum.
 */
#ifndef FICTIO_H
#define FICTIO_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "sors.h"

/* praenomen + nomen gentile ("Marcus Tullius", "Gaia Iulia") */
chorda
fictio_nomen (
       Sors* sors,
    Piscina* piscina);

/* praenomen.nomen@example.org|com|net - RFC 2606 SOLUM, numquam
 * inscriptio vera */
chorda
fictio_email (
       Sors* sors,
    Piscina* piscina);

/* forma v4: 8-4-4-4-12 hex minuscula */
chorda
fictio_uuid (
       Sors* sors,
    Piscina* piscina);

/* ISO 8601 UTC ('Z'), instans in [ab, ad] (epocha unix secundorum) */
chorda
fictio_tempus (
       Sors* sors,
        s64  ab,
        s64  ad,
    Piscina* piscina);

/* sententiae verae ex corpore (fons citatus in fictio.c), spatio
 * iunctae; sententiae >= I */
chorda
fictio_textus_latinus (
       Sors* sors,
        i32  sententiae,
    Piscina* piscina);

/* longitudo in RUNIS (code points) in [minimum, maximum]; alphabetum
 * UTF-8 (runae eliguntur), NIHIL = 'a'..'z' */
chorda
fictio_textus (
                 Sors* sors,
                  i32  minimum,
                  i32  maximum,
    constans character* alphabetum,
              Piscina* piscina);

s64
fictio_integer (
    Sors* sors,
     s64  minimum,
     s64  maximum);

f64
fictio_numerus (
    Sors* sors,
     f64  minimum,
     f64  maximum);

/* optiones NIHIL-terminatae; NIHIL si vacuae */
constans character*
fictio_eligere (
                     Sors* sors,
    constans character* constans* optiones);

/* SCRINIUM DIFFICILIUM: notae combinantes, littera non-BMP, textus
 * dextrorsum, virgulae + lineae obliquae, U+0000 effugiendum, spatia
 * sola, emoji cum modificatore, textus longus - omnia UTF-8 valida.
 * Seminatum: */
chorda
fictio_textus_difficilis (
       Sors* sors,
    Piscina* piscina);

/* ...et omnia ordine (probationes exhaustivae): */
i32
fictio_difficilia_numerus (vacuum);

chorda
fictio_difficile (
         i32 index,
    Piscina* piscina);

#endif /* FICTIO_H */
```

- [ ] Write `include/norma.h`:

```c
/* norma.h - schema valoris JSON: iudicare, exportare (norma-spec)
 *
 * Schema UNUM per aedificatores C; facies tres: iudicium (vitia ET
 * notae cum via nominata, omnia, non primum solum), exportatio JSON
 * Schema (input_schema instrumentorum), generatio (norma_gignere.h -
 * hic solum TYPUS functionis gignentis declaratur).
 *
 * Nodi in piscina aedificatorum vivunt; nodus a parentibus pluribus
 * communicari potest (DAG). Recursio nondum.
 *
 * STATUS: PROPOSITUM (norma-plan-1 N0) - a Frano probandum.
 *
 * USUS:
 *   Norma* n = norma_objectum(p);
 *   norma_campus(n, "id", norma_intra(norma_integer(p), 0, MMXLVIII), VERUM);
 *   NormaIudicium j = norma_iudicare(n, valor, p);
 *   si (!j.validum) { ... j.vitia: NormaVitium {via, causa, nuntius} ... }
 *   JsonValor* js = norma_json_schema(n, p);
 */
#ifndef NORMA_H
#define NORMA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "json.h"
#include "sors.h"

nomen structura Norma Norma;   /* opacum */

nomen enumeratio {
    NORMA_LIBERUM = 0,
    NORMA_NULLUM,
    NORMA_BOOLEAN,
    NORMA_INTEGER,
    NORMA_NUMERUS,       /* integer aut fluitans */
    NORMA_TEXTUS,
    NORMA_TABULATUM,
    NORMA_OBJECTUM,
    NORMA_DISCRIMEN      /* unio per campum tag */
} NormaGenus;

/* claves non declaratae (objectum) / tag ignotum (discrimen) */
nomen enumeratio {
    NORMA_CLAUSUM = 0,   /* vitium (ordinarium) */
    NORMA_APERTUM,       /* licet, tacite */
    NORMA_NOTANDUM       /* licet, NOTA redditur - valor validus manet */
} NormaModus;

/* functio gignens per campum (norma_gignere.h eam vocat) */
nomen JsonValor* (*NormaGignens)(
       Sors* sors,
    Piscina* piscina,
     vacuum* datum);


/* ======================================================================
 * AEDIFICATORES - reddunt nodum suum ut nidificentur. Usus pravus
 * (campus in non-objecto, minimum > maximum, filius NIHIL ...) nodum
 * notat; norma_iudicare id ut NORMA_CAUSA_SCHEMA_PRAVA ad '$' reddit.
 * ====================================================================== */

Norma* norma_liberum   (Piscina* piscina);
Norma* norma_nullum    (Piscina* piscina);
Norma* norma_boolean   (Piscina* piscina);
Norma* norma_integer   (Piscina* piscina);
Norma* norma_numerus   (Piscina* piscina);
Norma* norma_textus    (Piscina* piscina);

Norma* norma_tabulatum (Piscina* piscina, Norma* elementum);
Norma* norma_objectum  (Piscina* piscina);

/* campus objecti; titulus copiatur */
Norma* norma_campus (
                 Norma* objectum,
    constans character* titulus,
                 Norma* valor,
                    b32 requiritur);

Norma* norma_modus (
         Norma* objectum_aut_discrimen,
    NormaModus  modus);

/* unio: clavis = nomen campi tag (e.g. "type") */
Norma* norma_discrimen (
               Piscina* piscina,
    constans character* clavis);

/* variatio: objectum cuius campus tag == valor; tag in variatione
 * implicite declaratur */
Norma* norma_variatio (
                 Norma* discrimen,
    constans character* valor,
                 Norma* objectum);

/* modificatores */
Norma* norma_aut_nullum     (Norma* n);
Norma* norma_intra          (Norma* n, s64 minimum, s64 maximum);
Norma* norma_intra_fluitans (Norma* n, f64 minimum, f64 maximum);
/* textus: RUNAE (code points); tabulatum: elementa */
Norma* norma_longitudo      (Norma* n, i32 minimum, i32 maximum);
/* electio: NIHIL-terminata, copiatur */
Norma* norma_electio        (Norma* n, constans character* constans* licita);
/* forma: "date-time", "uuid", "email", "uri" a iudice probantur;
 * ceterae solum indicium generatoris */
Norma* norma_forma          (Norma* n, constans character* forma);
Norma* norma_descriptio     (Norma* n, constans character* textus);
Norma* norma_gignens        (Norma* n, NormaGignens functio, vacuum* datum);


/* ======================================================================
 * VISUS - lectio nodi opaci (generator, documenta, importatio futura)
 * ====================================================================== */

nomen structura {
    chorda  titulus;
    Norma*  valor;
       b32  requiritur;
} NormaCampus;

nomen structura {
    chorda  valor;       /* valor tag */
    Norma*  objectum;
} NormaVariatio;

nomen structura {
      NormaGenus  genus;
             b32  aut_nullum;
      NormaModus  modus;            /* objectum, discrimen */
           Norma* elementum;        /* tabulatum */
             Xar* campi;            /* NormaCampus (objectum) */
          chorda  clavis_discriminis;
             Xar* variationes;      /* NormaVariatio */
             b32  habet_intra;
             s64  minimum;
             s64  maximum;
             b32  habet_intra_fluitans;
             f64  minimum_fluitans;
             f64  maximum_fluitans;
             b32  habet_longitudinem;
             i32  longitudo_minima;
             i32  longitudo_maxima;
             Xar* licita;           /* chorda */
          chorda  forma;
          chorda  descriptio;
    NormaGignens  gignens;
          vacuum* gignens_datum;
          chorda  error_schematis;  /* vacua si nodus sanus */
} NormaVisus;

NormaVisus norma_visus (constans Norma* n);


/* ======================================================================
 * IUDICIUM
 * ====================================================================== */

nomen enumeratio {
    NORMA_GENUS = 0,     /* genus falsum */
    NORMA_DEEST,         /* campus requisitus deest */
    NORMA_EXTRA,         /* clavis non declarata (vitium aut nota) */
    NORMA_MINIMUM,
    NORMA_MAXIMUM,
    NORMA_LONGITUDO,
    NORMA_ELECTIO,       /* extra electionem */
    NORMA_FORMA,         /* forma nota fracta */
    NORMA_VARIATIO,      /* tag ignotum (vitium aut nota) */
    NORMA_DISCRIMEN,     /* tag deest aut non textus */
    NORMA_CAUSA_LIMES,         /* CCLVI vitia: iudicium cessavit */
    NORMA_CAUSA_SCHEMA_PRAVA   /* aedificatores male usi */
} NormaCausa;

nomen structura {
    chorda      via;      /* $.content[2].input.n ; clavis rara: $["a.b"] */
    NormaCausa  causa;
    chorda      nuntius;  /* sententia humana; valorem ipsum numquam refert */
} NormaVitium;

nomen structura {
     b32  validum;
    Xar*  vitia;          /* NormaVitium - validitatem frangunt */
    Xar*  notae;          /* NormaVitium - NOTANDUM: licita sed nova */
} NormaIudicium;

NormaIudicium norma_iudicare (
    constans Norma* n,
        JsonValor* valor,
          Piscina* piscina);

constans character* norma_causa_descriptio (NormaCausa causa);


/* ======================================================================
 * EXPORTATIO - JSON Schema (ordo clavium deterministicus; sine $schema)
 * ====================================================================== */

JsonValor* norma_json_schema (
    constans Norma* n,
          Piscina* piscina);

#endif /* NORMA_H */
```

- [ ] Write `include/norma_gignere.h`:

```c
/* norma_gignere.h - valores JSON ficti ex schemate (norma-spec par. IV)
 *
 * Modi: TYPICA (verisimilis), FINES (omnis nodus finitus in margine),
 * INVALIDA (vitium UNUM, via et causa redditae). Semina PER VIAM
 * (sors_derivare): campus additus valores fratrum non mutat.
 * Valores per fictio.h; functio gignens campi ordinarium substituit.
 *
 * Oraculum mutuum (probatio_norma_gignere): TYPICA et FINES iudicium
 * transeunt; INVALIDA vitium unum ad via_fracta cum causa_fracta.
 *
 * STATUS: PROPOSITUM (norma-plan-1 N0) - a Frano probandum.
 */
#ifndef NORMA_GIGNERE_H
#define NORMA_GIGNERE_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "json.h"
#include "norma.h"

nomen enumeratio {
    NORMA_TYPICA = 0,
    NORMA_FINES,
    NORMA_INVALIDA
} NormaModusGignendi;

/* INVALIDA: via_fracta + causa_fracta nominant vitium unum infixum.
 * valor NIHIL in INVALIDA = nihil violabile (schema totum apertum
 * aut liberum); via_fracta tunc vacua. TYPICA/FINES: via_fracta vacua. */
nomen structura {
    JsonValor*  valor;
        chorda  via_fracta;
    NormaCausa  causa_fracta;
} NormaGenitum;

NormaGenitum norma_gignere (
       constans Norma* n,
    NormaModusGignendi modus,
                  s64  semen,
             Piscina*  piscina);

#endif /* NORMA_GIGNERE_H */
```

- [ ] Compile-check all three in one TU with `VEXILLA_C89`; `./silva/examen.sh`
  on each; Latin lint (`VOCABULA_VIAE_ADDITAE=... ./oratio/vocabula.sh -nova`
  after `./silva/nexus.sh -renovare`); glossary entries for house
  coinages only (`fictio` is real Latin; `uuid`, `email` check).
- [ ] **STOP - Fran reviews** the three headers, especially decisions 1-4
  above.
- [ ] Commit the approved headers (+ glossary if touched); gates `oratio`
  if the glossary changed.
- [ ] Write `project-specs/norma-plan-2.md` (N1-N5) against them.
