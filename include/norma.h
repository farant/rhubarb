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
 * STATUS: PROBATUM a Frano 2026-10-08 (norma-plan-1 N0); implementatio in norma-plan-2.
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

Norma*
norma_liberum (
    Piscina* piscina);
Norma*
norma_nullum (
    Piscina* piscina);
Norma*
norma_boolean (
    Piscina* piscina);
Norma*
norma_integer (
    Piscina* piscina);
Norma*
norma_numerus (
    Piscina* piscina);
Norma*
norma_textus (
    Piscina* piscina);

Norma*
norma_tabulatum (
    Piscina* piscina,
      Norma* elementum);
Norma*
norma_objectum (
    Piscina* piscina);

/* campus objecti; titulus copiatur */
Norma*
norma_campus (
                  Norma* objectum,
     constans character* titulus,
                  Norma* valor,
                    b32  requiritur);

Norma*
norma_modus (
         Norma* objectum_aut_discrimen,
    NormaModus  modus);

/* unio: clavis = nomen campi tag (e.g. "type") */
Norma*
norma_discrimen (
               Piscina* piscina,
    constans character* clavis);

/* variatio: objectum cuius campus tag == valor; tag in variatione
 * implicite declaratur */
Norma*
norma_variatio (
                 Norma* discrimen,
    constans character* valor,
                 Norma* objectum);

/* modificatores */
Norma*
norma_aut_nullum (
    Norma* n);
Norma*
norma_intra (
    Norma* n,
      s64  minimum,
      s64  maximum);
Norma*
norma_intra_fluitans (
    Norma* n,
      f64  minimum,
      f64  maximum);
/* textus: RUNAE (code points); tabulatum: elementa */
Norma*
norma_longitudo (
    Norma* n,
      i32  minimum,
      i32  maximum);
/* electio: NIHIL-terminata, copiatur */
Norma*
norma_electio (
    Norma* n,
    constans character* constans* licita);
/* forma: "date-time", "uuid", "email", "uri" a iudice probantur;
 * ceterae solum indicium generatoris */
Norma*
norma_forma (
                 Norma* n,
    constans character* forma);
Norma*
norma_descriptio (
                 Norma* n,
    constans character* textus);
Norma*
norma_gignens (
           Norma* n,
    NormaGignens  functio,
          vacuum* datum);
/* titulus functionis gignentis (norma-spec-2 §III.1): .norma eum
 * scribit, norma_ad_c symbolum ex eo facit. Functio NIHIL licet -
 * generator tunc genus suum ordinarium gignit. */
Norma*
norma_gignens_titulus (
                 Norma* n,
    constans character* titulus);


/* ======================================================================
 * VISUS - lectio nodi opaci (generator, documenta, importatio futura)
 * ====================================================================== */

nomen structura {
    chorda  titulus;
     Norma* valor;
       b32  requiritur;
} NormaCampus;

nomen structura {
    chorda  valor;       /* valor tag */
     Norma* objectum;
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
          chorda  gignens_titulus;  /* vacua si nullus (norma-spec-2) */
          chorda  error_schematis;  /* vacua si nodus sanus */
} NormaVisus;

NormaVisus
norma_visus (
    constans Norma* n);


/* ======================================================================
 * IUDICIUM
 * ====================================================================== */

nomen enumeratio {
    NORMA_CAUSA_GENUS = 0,     /* genus falsum */
    NORMA_CAUSA_DEEST,         /* campus requisitus deest */
    NORMA_CAUSA_EXTRA,         /* clavis non declarata (vitium aut nota) */
    NORMA_CAUSA_MINIMUM,
    NORMA_CAUSA_MAXIMUM,
    NORMA_CAUSA_LONGITUDO,
    NORMA_CAUSA_ELECTIO,       /* extra electionem */
    NORMA_CAUSA_FORMA,         /* forma nota fracta */
    NORMA_CAUSA_VARIATIO,      /* tag ignotum (vitium aut nota) */
    NORMA_CAUSA_DISCRIMEN,     /* tag deest aut non textus */
    NORMA_CAUSA_LIMES,         /* CCLVI vitia: iudicium cessavit */
    NORMA_CAUSA_SCHEMA_PRAVA   /* aedificatores male usi */
} NormaCausa;

nomen structura {
        chorda via;      /* $.content[2].input.n ; clavis rara: $["a.b"] */
    NormaCausa causa;
        chorda nuntius;  /* sententia humana; valorem ipsum numquam refert */
} NormaVitium;

nomen structura {
     b32  validum;
     Xar* vitia;          /* NormaVitium - validitatem frangunt */
     Xar* notae;          /* NormaVitium - NOTANDUM: licita sed nova */
} NormaIudicium;

NormaIudicium
norma_iudicare (
    constans Norma* n,
         JsonValor* valor,
           Piscina* piscina);

constans character*
norma_causa_descriptio (
    NormaCausa causa);


/* ======================================================================
 * EXPORTATIO - JSON Schema (ordo clavium deterministicus; sine $schema)
 * ====================================================================== */

JsonValor*
norma_json_schema (
    constans Norma* n,
           Piscina* piscina);

#endif /* NORMA_H */
