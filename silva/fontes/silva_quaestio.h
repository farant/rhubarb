/* silva_quaestio.h - machina selectorum CSS-similis super arbores
 * silvae (QUAESTIO QA+QB; consilium = project-specs/
 * silva-quaestio-design.md, DECISUS 2026-07-06)
 *
 * SUPERFICIES (QA+QB; QC pseudo-classes - IR eas fert, parser
 * nondum):
 *
 *   declaratio                    genus-tag (registro-ductus)
 *   *                             universalis
 *   definitio-functionis corpus   descendens
 *   definitio-functionis > corpus filius directus
 *   declaratio + sententia-*      frater proximus (QB)
 *   declaratio ~ sententia-*      frater sequens (QB)
 *   declaratio, commutatio        unio (virgula)
 *   [locus]                       attributum exsistit (QB)
 *   [locus="valor"]               aequalis; ^= incipit, $= desinit,
 *                                 *= continet (QB)
 *   tag$nomen                     captura (QB; iuxtaposita - spatium
 *                                 = combinator)
 *
 * ATTRIBUTA (QB): locus PER GENUS nodi sub probatione resolvitur
 * (nomina locorum = vocabularium annotationum, DECISUS Q2). Locus
 * TOKEN valorem lexematis comparat (mensura-conscius); locus LISTA
 * congruit si elementum lexematis ULLUM congruit (folium-chorda);
 * loci non-lexematis: exsistentia = non-NIHIL, operationes
 * comparationis = FALSUM. Compositum cum genus-tag: locus generi
 * ignotus = fractura compilationis CLARA (viae attributorum mortuae
 * v1 exstirpatae); compositum sine tag differt ad non-congruentiam
 * temporis cursus. Valor citatus: octeti ad '"' clausum, SINE
 * sequentiis fugae (LIMES NOMINATUS v1).
 *
 * CAPTURAE (QB): $nomen gradus OMNES catenae congruentis ligat
 * (insectum v1 #1 exstirpatum); unio = catena congruens PRIMA
 * ligat. congruit() booleanum manet - exsequi() ligat.
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

nomen enumeratio {
    SILVA_QUAESTIO_ATTR_EXSISTIT = 0,  /* [locus] */
    SILVA_QUAESTIO_ATTR_AEQUALIS,      /* = */
    SILVA_QUAESTIO_ATTR_INCIPIT,       /* ^= */
    SILVA_QUAESTIO_ATTR_DESINIT,       /* $= */
    SILVA_QUAESTIO_ATTR_CONTINET       /* *= */
} SilvaQuaestioAttrOp;

nomen structura {
    SilvaQuaestioParsGenus genus;
    s32                    nodi_genus;  /* PARS_GENUS: index generis */
    /* QB attributum: [locus op "valor"] - chordae in piscinam
     * copiatae (quaestio selectorem superviveat) */
    chorda                 locus_titulus;
    s32                    attr_op;     /* SilvaQuaestioAttrOp */
    chorda                 attr_valor;
    /* QB captura: $nomen */
    chorda                 captura_titulus;
    /* QC: pseudo {titulus, arg} hic crescet */
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
    i32                            gradus_maximi; /* vestigia (QB) */
    /* forma ambigui (exemplar commissionis - descensus canonicus) */
    s32 genus_ambigui;
    i32 locus_interpretationum;
    i32 locus_canonicae;
};

/* ==================================================
 * Fructus
 * ================================================== */

/* Captura ligata: gradus catenae congruentis cum $nomine (QB) */
nomen structura {
    chorda               titulus;
    constans SilvaNodus* nodus;
} SilvaQuaestioCaptura;

nomen structura {
    constans SilvaNodus* nodus;   /* subiectum (compositum ultimum) */
    Xar*                 capturae; /* SilvaQuaestioCaptura (valore);
                                    * NIHIL = catena sine capturis */
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
