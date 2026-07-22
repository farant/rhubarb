/* silva_quaestio.h - machina selectorum CSS-similis super arbores
 * silvae (QUAESTIO QA+QB; consilium = project-specs/
 * silva-quaestio-design.md, DECISUS 2026-07-06)
 *
 * SUPERFICIES (QA+QB+QC):
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
 *   :pseudo / :pseudo(arg)        pseudo-classis (QC)
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
 * PSEUDO-CLASSES (QC; decreta 2026-07-22 in phase-log):
 * structurales :primus/:ultimus - filius nodalis primus/ultimus in
 * lista continente (lexemata interposita transiliuntur); sine lista
 * continente (locus NODUS directus, radix) = filius solus = AMBO
 * congruunt. logicales :habet(sel) - descendens strictus congruens;
 * catena nidificata intra subarborem subiecti CONFINATA (paritas
 * CSS, limes); :non(sel) - nodus ipse non congruit (ascensus
 * liber, unio intra licet). semantici (genera per nomen resoluta,
 * exemplar ambigui - grammaticae absens = numquam congruit):
 * :vocat(f) CONTINET vocationem cuius functio = folium directum f
 * ((*fp)() non congruit - limes nominatus); :definit(x)
 * declarator-titulus x continet; :utitur(x) folium-identificator x
 * continet; :reddit sententiam redde continet. silvani: :ambiguum
 * (nodus involucri ipse); :lectiones - vexillum AMBULATIONIS:
 * exsequi lectiones OMNES descendit (non canonicam solam), intra
 * :habet subarborem eius quoque laxat, ut praedicatum neutrale;
 * :sumptus/:omissus - maior-aut-ipse est bracchium conditionale
 * (omissum liberos nodales non habet - nodus bracchii ipse solus
 * congruere potest). argumenta regulam citationis QB sequuntur
 * (sine fugis); capturae intra selectores nidificatos numquam
 * ligantur (neutrales).
 *
 * PSEUDO USORIS (QC): tabula ANTE compilationem impletur
 * (registrum_creare + registrare), tum compilare_cum_registro.
 * pseudo ignotum = fractura compilationis CLARA - nulla acceptatio
 * tacita (forma v1 [registratio post compilationem] cum hoc
 * decreto incompatibilis erat). nomina nativa obumbrari non
 * possunt; titulus idem iterum registratus functionem renovat;
 * functiones in quaestionem compilatam COPIANTUR (mutationes
 * tabulae posteriores quaestiones exsistentes non tangunt).
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

nomen enumeratio {
    SILVA_QUAESTIO_PSEUDO_PRIMUS = 0,  /* :primus */
    SILVA_QUAESTIO_PSEUDO_ULTIMUS,     /* :ultimus */
    SILVA_QUAESTIO_PSEUDO_HABET,       /* :habet(sel) */
    SILVA_QUAESTIO_PSEUDO_NON,         /* :non(sel) */
    SILVA_QUAESTIO_PSEUDO_VOCAT,       /* :vocat(f) - CONTINET */
    SILVA_QUAESTIO_PSEUDO_DEFINIT,     /* :definit(x) */
    SILVA_QUAESTIO_PSEUDO_UTITUR,      /* :utitur(x) */
    SILVA_QUAESTIO_PSEUDO_REDDIT,      /* :reddit */
    SILVA_QUAESTIO_PSEUDO_AMBIGUUM,    /* :ambiguum */
    SILVA_QUAESTIO_PSEUDO_LECTIONES,   /* :lectiones (vexillum) */
    SILVA_QUAESTIO_PSEUDO_SUMPTUS,     /* :sumptus */
    SILVA_QUAESTIO_PSEUDO_OMISSUS,     /* :omissus */
    SILVA_QUAESTIO_PSEUDO_USOR         /* registratus */
} SilvaQuaestioPseudoGenus;

/* Functio pseudo-classis usoris: nodus sub probatione + argumentum
 * (chorda vacua si sine parenthesibus) + datum registrationis.
 * VERUM = congruit. */
nomen b32 (*SilvaQuaestioPseudoFunctio) (
    constans SilvaNodus* nodus,
    chorda               argumentum,
    vacuum*              datum);

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
    /* QC pseudo: :titulus(arg); -I = non-pseudo */
    s32                        pseudo_genus; /* SilvaQuaestioPseudoGenus */
    chorda                     pseudo_arg;   /* vocat/definit/utitur/usor */
    structura SilvaQuaestio*   pseudo_quaestio; /* habet/non compilata */
    SilvaQuaestioPseudoFunctio pseudo_functio;  /* USOR */
    vacuum*                    pseudo_datum;    /* USOR */
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
    /* QC: vexillum :lectiones + formae generum semanticorum
     * (per nomen resolutae, -I = grammaticae absens = praedicatum
     * numquam congruit) */
    b32 lectiones_omnes;
    s32 genus_vocationis;
    s32 locus_functionis;
    s32 genus_folii;
    s32 locus_folii_valoris;
    s32 genus_declaratoris;
    s32 locus_declaratoris_tituli;
    s32 genus_redde;
    s32 genus_rami_sumpti;
    s32 genus_rami_omissi;
};

/* Tabula pseudo-classium usoris (QC) - impletur ANTE compilationem.
 * Titulo signata: silva.h eam OPACAM praenuntiat (exemplar
 * SilvaQuaestio) */
nomen structura SilvaQuaestioPseudoRegistrum
    SilvaQuaestioPseudoRegistrum;

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

/* Ut compilare, sed pseudo-classes usoris ex tabula data agnoscit
 * (registro NIHIL = nativae solae). Tabula per compilationem solam
 * legitur - functiones in quaestionem copiantur. */
SilvaQuaestio*
silva_quaestio_compilare_cum_registro (
    Piscina*                               piscina,
    constans SilvaRegistrumCoctum*         tabularium,
    constans SilvaQuaestioPseudoRegistrum* registro,
    constans character*                    selector,
    constans character**                   causa_out);

/* Tabulam pseudo-classium vacuam creare. NIHIL = exhaustum. */
SilvaQuaestioPseudoRegistrum*
silva_quaestio_registrum_creare (Piscina* piscina);

/* Pseudo-classem usoris registrare. FALSUM: argumentum NIHIL,
 * titulus vacuus aut litteris non-tag ([a-z0-9_-]), aut nomen
 * nativum obumbraret. Titulus idem iterum = functio/datum
 * renovata (VERUM). */
b32
silva_quaestio_registrare (
    SilvaQuaestioPseudoRegistrum* registro,
    constans character*           titulus,
    SilvaQuaestioPseudoFunctio    functio,
    vacuum*                       datum);

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
