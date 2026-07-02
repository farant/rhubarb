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
    i32             numerus_errorum;     /* nodi ERROR in arbore */
    i32             numerus_segmentorum;

    /* Numeratores aggregati trans segmenta (summae; frons max) */
    i32 fusiones;
    i32 transmutationes;
    i32 transmutationes_negatae;
    i32 eventa_marginis_novi;
    i32 frons_maxima;
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
    vacuum*                   contextus);

/* Ingressus lexematum (forma silva_lexare: Xar de SilvaToken*, EOF
 * ultimo) - pro fluxibus iam expansis aut probationibus. */
SilvaParsura*
silva_lexemata_parsare (
    Piscina*                  piscina,
    constans Xar*             lexemata,
    constans SilvaGrammatica* grammatica,
    constans SilvaOraculum*   oraculum,
    SilvaResolutor            resolutor,
    vacuum*                   contextus);

#endif /* SILVA_PARSARE_H */
