/* silva_annotationes.h - Collector annotationum STML
 * (parcum 01KY3D7EJP, frustum A; spec = project-specs/
 * annotationes-stml-spec.md)
 *
 * Stratum identitatis et intentionis: commentarium cuius contentum
 * primum non-spatium tag STML apertum est ANNOTATIO est - corpus
 * totum (delimitatoribus decorationeque exutis) ad stml_legere it.
 * Cetera prosa sunt, numquam lecta. Commentarium ancoratum quod
 * parsare nequit INVENTUM manet (parsata FALSUM + positiones) -
 * numquam tacite prosa reclassificatum: omne ancoratum in fructu
 * EST, consumptores errare nequeunt.
 *
 * Praetransitus decorationis (spec par 2.2): lineae continuationis
 * spatia ducentia + asteriscum unum + spatium unum optionale
 * exuunt; linea prima intacta (post delimitatorem).
 *
 * Affixio (spec par 2.3, octetis per fontem): SUPRA = unitas
 * suprema proxima post commentarium (salta-vacua - lineae vacuae
 * numquam frangunt, praecedens EXSPECTA-PROXIMA); INTERIOR =
 * commentarium intra extensionem unitatis (unitas continens -
 * additio post-caerimoniam, Fran benedixit 2026-07-21); PLAGULA =
 * sine unitate sequente (scopus plagulae). Trivia per catenam
 * originis attinguntur (lectio TOLERA: commentarium ante lineam
 * verbo latino ductam in lexemate invocationis equitat).
 */

#ifndef SILVA_ANNOTATIONES_H
#define SILVA_ANNOTATIONES_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"
#include "stml.h"
#include "silva_token.h"
#include "silva_nodus.h"
#include "silva_parsare.h"


/* ==================================================
 * Modus affixionis
 * ================================================== */

nomen enumeratio {
    SILVA_ANNOTATIO_SUPRA = 0,   /* supra unitatem sequentem */
    SILVA_ANNOTATIO_INTERIOR,    /* intra unitatem continentem */
    SILVA_ANNOTATIO_PLAGULA      /* scopus plagulae (nulla unitas) */
} SilvaAnnotatioModus;


/* ==================================================
 * Annotatio collecta
 * ================================================== */

nomen structura {
    /* fons commentarii */
    chorda textus;        /* corpus purgatum (praetransitus factus) */
    chorda crudum;        /* octeti pleni delimitatoribus inclusis */
       s32 fons_index;
       i32 linea;         /* 1-basata (commentarii initium) */
       i32 columna;       /* 1-basata */
       s32 byte_offset;   /* intra fontem */

    /* affixio */
    SilvaAnnotatioModus  modus;
    constans SilvaNodus* unitas;      /* NIHIL = PLAGULA */

    /* fructus stml_legere super textu purgato. Linea fontis
     * erroris = linea + linea_erroris - 1 (praetransitus lineas
     * servat); columnae sub decoratione exuta moventur. */
          b32  parsata;       /* successus parsationis */
    StmlNodus* documentum;    /* radix documenti (liberi = elementa
                               * omnia - commentarium unum elementa
                               * plura ferre potest) */
     StmlNodus* arbor;         /* elementum primum; NIHIL si fractum */
    StmlStatus  status;
           i32  linea_erroris;    /* intra textum purgatum, 1-basata */
           i32  columna_erroris;
        chorda  error;
} SilvaAnnotatio;


/* ==================================================
 * API
 * ================================================== */

/* Colligere annotationes parsurae totius: ambulatio triviorum
 * (lexemata + radices originis, duplicata per (fons, byte_offset)
 * omissa), ancoratio, purgatio, stml_legere, affixio. Ordo =
 * ordo fluminis lexematum. intern NIHIL licet (collector suum
 * creat). Xar de SilvaAnnotatio (per valorem); NIHIL in errore
 * allocationis. */
Xar*
silva_annotationes_colligere (
                  Piscina* piscina,
    constans SilvaParsura* parsura,
      InternamentumChorda* intern);

/* Unitates supremae parsurae: radix commissionis ("fortasse
 * religata") hodie LISTA sententiarum est, post normalizationem
 * NODUS esse posset - ambo hic accepta, nodi soli redduntur.
 * Xar de SilvaNodus* ordine fontis; NIHIL sine commissione. */
Xar*
silva_annotationes_unitates (
                  Piscina* piscina,
    constans SilvaParsura* parsura);


/* ==================================================
 * Identitates (frustum B): nid lectae + petitiones mintationis
 * ================================================== */

nomen enumeratio {
    SILVA_INSERTIO_NIHIL = 0,        /* identitas iam data */
    SILVA_INSERTIO_POST_ATTRIBUTUM,  /* ="ULID" post titulum attributi */
    SILVA_INSERTIO_POST_TITULUM      /* " v=\"ULID\"" post titulum elementi */
} SilvaInsertioGenus;

nomen structura {
             StmlNodus* elementum;       /* elementum ferens */
                chorda  valor;           /* ULID; vacua = petitio */
                   b32  petitio;         /* mintanda */
    SilvaInsertioGenus  insertio_genus;
                   s32  insertio_offset; /* fons-absolutum; -1 = nulla
                                         * aut INCERTA (numeri arborum
                                         * et textus dissentiunt -
                                         * instrumentum recusat) */
} SilvaIdentitas;

/* Identitates annotationis unius: latus arboris (ordine documenti)
 * identitates datas et petitiones legit; latus textus (octeti crudi
 * commentarii - offsets in fonte VERI) sedes insertionis petitionum
 * locat (scansio intra-tag, status quotarum, decoratione intra tag
 * saltata, regula v in elemento nid solo; tags claudentes numquam).
 * Numeri laterum dissentientes -> offsets petitionum = -1. Currit in
 * annotationibus PARSATIS solis (malformata = res lint, numquam
 * mintanda); NIHIL si annotatio non parsata. Xar de SilvaIdentitas. */
Xar*
silva_annotationes_identitates (
                    Piscina* piscina,
    constans SilvaAnnotatio* annotatio);

#endif /* SILVA_ANNOTATIONES_H */
