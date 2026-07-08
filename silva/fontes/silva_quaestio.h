/* silva_quaestio.h - machina selectorum CSS-similis super arbores
 * silvae (QUAESTIO QA; consilium = project-specs/
 * silva-quaestio-design.md, DECISUS 2026-07-06)
 *
 * SUPERFICIES (QA subset; QB attributa/capturae/fratres, QC
 * pseudo-classes - IR eas iam fert, parser nondum):
 *
 *   declaratio                    genus-tag (registro-ductus)
 *   *                             universalis
 *   definitio-functionis corpus   descendens
 *   definitio-functionis > corpus filius directus
 *   declaratio, commutatio        unio (virgula)
 *
 * GRAMMATICAE-IGNARA: compilare registrum coctum accipit (tags =
 * tituli generum DECISUS); modulus nullum caput c89 includit.
 * Tag ignotum = NIHIL + causa (fractura CLARA - v1 tacite nihil
 * congruebat; mutatio deliberata, DECISUS).
 *
 * CONTRACTUS ARBORIS: exsequi/congruit arbores COMMISSAS
 * exspectant (pater fixus - omnis parsura per silva_parsare/
 * silva_c89_parsare talis est). Ambigua: descensus per lectionem
 * CANONICAM solam (DECISUS; :lectiones QC eam laxabit); nodus
 * involucri ipse congruibilis (tag "ambiguus"). Nodi ERROR
 * congruibiles (tag "error") - lexemata eorum non descensa.
 */

#ifndef SILVA_QUAESTIO_H
#define SILVA_QUAESTIO_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "silva_token.h"
#include "silva_nodus.h"
#include "silva_tabulae.h"

/* ==================================================
 * IR selectoris (QB/QC partes iam in formis - API primum)
 * ================================================== */

nomen enumeratio {
    SILVA_QUAESTIO_PARS_GENUS = 0,   /* tag: declaratio */
    SILVA_QUAESTIO_PARS_UNIVERSALIS, /* * */
    SILVA_QUAESTIO_PARS_ATTRIBUTUM,  /* [locus op "valor"] - QB */
    SILVA_QUAESTIO_PARS_PSEUDO,      /* :pseudo(arg) - QC */
    SILVA_QUAESTIO_PARS_CAPTURA      /* $nomen - QB */
} SilvaQuaestioParsGenus;

nomen enumeratio {
    SILVA_QUAESTIO_COMB_NULLUS = 0,  /* compositum primum */
    SILVA_QUAESTIO_COMB_DESCENDENS,  /* spatium */
    SILVA_QUAESTIO_COMB_FILIUS,      /* > */
    SILVA_QUAESTIO_COMB_FRATER_PROXIMUS, /* + (QB) */
    SILVA_QUAESTIO_COMB_FRATER_SEQUENS   /* ~ (QB) */
} SilvaQuaestioCombinator;

nomen structura {
    SilvaQuaestioParsGenus genus;
    s32                    nodi_genus;  /* PARS_GENUS: index generis */
    /* QB/QC: attributum {locus, op, valor}, pseudo {titulus, arg},
     * captura {titulus} hic crescent */
} SilvaQuaestioPars;

/* Gradus catenae: combinator ad SINISTRAM + partes composito */
nomen structura {
    SilvaQuaestioCombinator combinator; /* NULLUS in primo */
    Xar*                    partes;     /* SilvaQuaestioPars (valore) */
} SilvaQuaestioGradus;

/* Titulo signata (non anonyma): silva.h eam OPACAM praenuntiat
 * (exemplar SilvaOraculum) - amalgamator lineam nomen-unitatis
 * demittit (CADENDA), definitio interna manet */
nomen structura SilvaQuaestio SilvaQuaestio;
structura SilvaQuaestio {
    constans SilvaRegistrumCoctum* tabularium;
    Xar*                           catenae; /* Xar* graduum (unio) */
    /* forma ambigui (exemplar commissionis - descensus canonicus) */
    s32 genus_ambigui;
    i32 locus_interpretationum;
    i32 locus_canonicae;
};

/* ==================================================
 * Fructus
 * ================================================== */

nomen structura {
    constans SilvaNodus* nodus;   /* subiectum (compositum ultimum) */
    /* QB: capturae (nomen -> nodus, omnes gradus catenae) */
} SilvaQuaestioResultatum;

/* ==================================================
 * API
 * ================================================== */

/* Selector -> quaestio compilata (reusabilis trans arbores).
 * NIHIL + *causa_out (litterae statice vivae; NIHIL licet) si
 * selector malformatus aut tag registro ignotum. */
SilvaQuaestio*
silva_quaestio_compilare (
    Piscina*                       piscina,
    constans SilvaRegistrumCoctum* tabularium,
    constans character*            selector,
    constans character**           causa_out);

/* Omnes nodi congruentes sub radice (SilvaValor - NODUS aut LISTA,
 * commissio->radix directa). Xar de SilvaQuaestioResultatum (valore)
 * in piscinam; numquam NIHIL nisi piscina exhausta. Quisque nodus
 * semel subiectum - duplicata structuraliter impossibilia. */
Xar*
silva_quaestio_exsequi (
    constans SilvaQuaestio* quaestio,
    SilvaValor              radix,
    Piscina*                piscina);

/* Nodusne unus catenae alicui congruit (subiectum = nodus; maiores
 * per patrem probantur - arbor commissa)? */
b32
silva_quaestio_congruit (
    constans SilvaQuaestio* quaestio,
    constans SilvaNodus*    nodus);

#endif /* SILVA_QUAESTIO_H */
