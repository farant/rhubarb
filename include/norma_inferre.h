/* norma_inferre.h - schema ex exemplis JSON inferre: ADUMBRATIO, non
 * veritas (norma-spec-3, fasciculus B1)
 *
 * Exempla singula adduntur; inferentiae duae coniungi possunt (lex:
 * coniungere(A, B) idem dicit ac omnia utriusque addita). Adumbratio
 * dicit solum quod exempla ostendunt: requisitum = in OMNI instantia
 * parentis visum; modus ordinarius NOTANDUM; formae solum si omne
 * exemplum eas transit; electiones et fines solum rogati (corpus
 * congelant et valores ex responsis fortasse privatis effunderent).
 * Valores numquam exeunt nisi tag discriminis et, rogatae, electiones.
 * Discrimen invenitur (norma-spec-3 §IV): clavis in omni instantia,
 * semper textus, paucis valoribus, cuius partitio claves requisitas
 * auget. Positio tabulae similis (claves multae, singulae rarae)
 * objectum apertum fit cum descriptione - typus tabulae (ceteri)
 * fasciculus posterior.
 *
 * STATUS: PROBATUM a Frano 2026-10-09 (norma-spec-3 B1.0).
 *
 * USUS:
 *   Inferentia* inf = inferentia_creare(p, NIHIL);
 *   per (...) inferentia_addere(inf, exemplum);
 *   Norma* adumbratio = inferentia_normam(inf, p);
 *   -> norma_stml_scribere (norma_stml.h) ad recensionem
 */
#ifndef NORMA_INFERRE_H
#define NORMA_INFERRE_H

#include "latina.h"
#include "piscina.h"
#include "json.h"
#include "norma.h"
#include "norma_stml.h"   /* NormaCommentarius - typus solus */

nomen structura {
    NormaModus modus;             /* objecta et discrimina; ordinarium NOTANDUM */
           b32 formae;            /* formas proponere; ordinarium VERUM */
           b32 electio;           /* electiones textuum; ordinarium FALSUM */
           b32 fines;             /* intra / longitudo observata; ordinarium FALSUM */
           i32 exempla_minima;    /* formae, electio, fines solum si >= N; 0 = V */
           i32 electio_maxima;    /* valores distincti maximi electionis; 0 = VIII */
           i32 discrimen_maximum; /* valores tag distincti maximi; 0 = XVI */
} InferentiaOptiones;

nomen structura Inferentia Inferentia;   /* opaca */

InferentiaOptiones
inferentia_optiones_ordinariae (vacuum);

/* optiones NIHIL = ordinariae; copiantur */
Inferentia*
inferentia_creare (
                        Piscina* piscina,
    constans InferentiaOptiones* optiones);

/* exemplum unum (radix quaelibet; NIHIL ignoratur) */
vacuum
inferentia_addere (
    Inferentia* inferentia,
     JsonValor* exemplum);

/* inferentia nova = a et b coniunctae (optiones ex a); neutra mutatur */
Inferentia*
inferentia_coniungere (
    constans Inferentia* a,
    constans Inferentia* b,
                Piscina* piscina);

/* exempla addita */
i32
inferentia_numerus (
    constans Inferentia* inferentia);

/* adumbratio; NIHIL si nullum exemplum */
Norma*
inferentia_normam (
    constans Inferentia* inferentia,
                Piscina* piscina);


/* ======================================================================
 * TESTIMONIA (norma-spec-4 §III): cur quaeque linea adumbrationis talis
 * sit - numeri et decisiones solum, numquam valores (praeter tag
 * discriminis et electiones rogatas)
 * ====================================================================== */

nomen structura InferentiaTestimonia InferentiaTestimonia;   /* opaca */

/* adumbratio + testimonium cuiusque nodi; NIHIL si nullum exemplum */
Norma*
inferentia_normam_testatam (
     constans Inferentia*  inferentia,
                 Piscina*  piscina,
    InferentiaTestimonia** testimonia);

/* NormaCommentarius pro norma_stml_scribere_cum_commentis; datum =
 * InferentiaTestimonia* ex inferentia_normam_testatam */
chorda
inferentia_commentarius (
    constans Norma* nodus,
           Piscina* piscina,
            vacuum* testimonia);


/* ======================================================================
 * DISCREPANTIA (norma-spec-4 §IV): schema declaratum contra figuras
 * exemplorum - numeri, numquam valores
 * ====================================================================== */

nomen enumeratio {
    NORMA_DISCREPANTIA_FORTASSE_REQUISITUM = 0, /* optionale, in omnibus visum */
    NORMA_DISCREPANTIA_REQUISITUM_ABSENS,       /* requisitum, in quibusdam deest */
    NORMA_DISCREPANTIA_NUMQUAM_VISUM,           /* declaratum, numquam visum */
    NORMA_DISCREPANTIA_NON_DECLARATUM,          /* visum, non declaratum */
    NORMA_DISCREPANTIA_GENUS_LATIUS,            /* genera visa extra declaratum */
    NORMA_DISCREPANTIA_NULLUM_NOVUM,            /* null visum, non aut_nullum */
    NORMA_DISCREPANTIA_VARIATIO_NUMQUAM_VISA,
    NORMA_DISCREPANTIA_VARIATIO_NON_DECLARATA,
    NORMA_DISCREPANTIA_NON_COMPARABILE          /* e.g. tag non candidatus */
} NormaDiscrepantiaGenus;

nomen structura {
                    chorda via;      /* $.content[].type ; $["a.b"] */
    NormaDiscrepantiaGenus genus;
                    chorda nuntius;  /* numeri, numquam valores */
} NormaDiscrepantia;

/* Xar de NormaDiscrepantia, ordine ambulationis (vacuum = sine
 * discrepantia); NIHIL si declarata aut inferentia NIHIL */
Xar*
norma_comparare (
         constans Norma* declarata,
    constans Inferentia* inferentia,
                Piscina* piscina);

/* nomen genus discrepantiae ("FORTASSE_REQUISITUM" ...) */
constans character*
norma_discrepantia_descriptio (
    NormaDiscrepantiaGenus genus);

#endif /* NORMA_INFERRE_H */
