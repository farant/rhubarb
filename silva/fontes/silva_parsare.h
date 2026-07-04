/* silva_parsare.h - Gubernator parsurae (Phase 4 Chunk D)
 *
 * Compositio totius fistulae: expansio -> lexemata -> GLR -> commissio.
 * GUBERNATOR EST CIRCUITUS SECANS (spec-v2 par 3.1, plan of record):
 * fluxus in segmenta per limites secatur (sceletum: SEMICOLON in
 * profunditate parenthesium 0), quodque segmentum + lexema EOF
 * independenter parsatur:
 *   - successus -> sententiae eius radici coniunguntur;
 *   - fractura  -> nodus ERROR lexemata segmenti fert (recuperatio
 *     per-rem gratuita; GSS per sententiam terminatus).
 * OMNIS input arborem completam commissam reddit (par 8.2 - vectis
 * tree-sitter). Inventor limitum C89 (laminae, definitiones
 * functionum) cum grammatica C89 veniet - nominatum.
 */

#ifndef SILVA_PARSARE_H
#define SILVA_PARSARE_H

#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "silva_token.h"
#include "silva_nodus.h"
#include "silva_tabulae.h"
#include "silva_glr.h"
#include "silva_expandere.h"
#include "silva_commissio.h"
#include "silva_contextus.h"

/* ==================================================
 * Ligamen grammaticae (superficies generata una)
 * ================================================== */

nomen structura {
    constans SilvaTabulaCocta*     tabula;
    constans SilvaRegistrumCoctum* tabularium;
    SilvaGLRConstructor            constructor;
    SilvaGLRFabricaAmbigui         fabrica;
} SilvaGrammatica;


/* ==================================================
 * Fructus gubernatoris
 * ================================================== */

nomen structura {
    b32             successus;       /* arbor completa exsistit (semper
                                      * post recuperationem, nisi
                                      * apparatus ipse fractus) */
    SilvaCommissio* commissio;       /* radix + ambigui + resolutiones */
    SilvaToken*     lexema_finis;    /* EOF (trivia caudae - Phase 5) */
    Xar*            lexemata;        /* fluxus expansus (queribilis) */
    SilvaExpansio*  expansio;        /* strata/diarium; NIHIL in
                                      * ingressu lexematum */
    Xar*            directivae;      /* Xar de Xar de SilvaToken* -
                                      * lineae directivae consumptae
                                      * (reconstructio fontis, Phase
                                      * 5); NIHIL in ingressu
                                      * lexematum */
    s32             fons_princeps;   /* index fontis principalis (cum
                                      * praebere fontes priores indices
                                      * capiunt!); -1 in ingressu
                                      * lexematum */
    i32             numerus_errorum;     /* nodi ERROR in arbore */
    i32             numerus_segmentorum;

    /* Numeratores aggregati trans segmenta (summae; frons max) */
    i32 fusiones;
    i32 transmutationes;
    i32 transmutationes_negatae;
    i32 eventa_marginis_novi;
    i32 frons_maxima;

    /* Fines + intermissio (Phase 7 Chunk A). Sub finibus/intermissione
     * arbor COMPLETA manet (cauda in nodum ERROR - nihil amittitur,
     * reconstructio byte-exacta tenet); successus VERUM cum vexillis
     * his veritatem dicentibus. */
    b32 est_intermissa;        /* pergere FALSUM reddidit */
    b32 expansio_decisa;       /* expansio trunca (limen tactum) */
    b32 fines_tactae;          /* quilibet limen tactus */
    i32 segmenta_ultra_limen;  /* segmenta fronte ultra limen fracta */

    /* Textura conditionalium (Phase 7 Chunk B): regiones in arborem
     * textae vs degradatae (limes sententiae transgressus, fontes
     * mixti, elementum opacum - lamina reinserenda possessa manet) */
    i32 regiones_textae;
    i32 regiones_omissae;
    Xar* strata;         /* Xar de Xar* - fluxus per generationem
                          * MUTANTEM (additiones II; GRATIS - arenae
                          * iam solvuntur, gubernator modo tenet).
                          * NIHIL si fluxus numquam mutatus.
                          * strata[ultimum] == lexemata (alias!) */
} SilvaParsura;


/* ==================================================
 * API
 * ================================================== */

/* Fistula tota ex fonte crudo: praeprocessor incluso. Oraculum/
 * resolutor NIHIL licent (nulla resolutio - ambigua supersunt). */
SilvaParsura*
silva_parsare (
    Piscina*                  piscina,
    constans character*       titulus_fontis,
    constans character*       fons,
    i32                       mensura,
    constans SilvaGrammatica* grammatica,
    constans SilvaOraculum*   oraculum,
    SilvaResolutor            resolutor,
    vacuum*                   datum_resolutoris);

/* Fistula tota cum expansione PRAEPARATA a vocatore: includenda iam
 * praebita (silva_includendum_praebere), macros iam iniecta
 * (silva_macro_addere). Haec est sutura contextus (SilvaContextus
 * Phase 7 eam equitabit); silva_parsare = involucrum tenue huius. */
SilvaParsura*
silva_parsare_cum_expansione (
    Piscina*                  piscina,
    SilvaExpansio*            expansio,
    constans character*       titulus_fontis,
    constans character*       fons,
    i32                       mensura,
    constans SilvaGrammatica* grammatica,
    constans SilvaOraculum*   oraculum,
    SilvaResolutor            resolutor,
    vacuum*                   datum_resolutoris);

/* Fistula tota cum CONTEXTU hospitis (Phase 7): receptum contextus
 * expansioni recenti applicatur (fines, pergere, lexica ante fontem
 * processata - latina hac via, praebenda), deinde fistula normalis.
 * Contextus diu vivit et inter parsuras reusabilis est. */
SilvaParsura*
silva_parsare_cum_contextu (
    Piscina*                  piscina,
    constans SilvaContextus*  contextus,
    constans character*       titulus_fontis,
    constans character*       fons,
    i32                       mensura,
    constans SilvaGrammatica* grammatica,
    constans SilvaOraculum*   oraculum,
    SilvaResolutor            resolutor,
    vacuum*                   datum_resolutoris);

/* Ingressus lexematum (forma silva_lexare: Xar de SilvaToken*, EOF
 * ultimo) - pro fluxibus iam expansis aut probationibus. */
SilvaParsura*
silva_lexemata_parsare (
    Piscina*                  piscina,
    constans Xar*             lexemata,
    constans SilvaGrammatica* grammatica,
    constans SilvaOraculum*   oraculum,
    SilvaResolutor            resolutor,
    vacuum*                   datum_resolutoris);

#endif /* SILVA_PARSARE_H */
