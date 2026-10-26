/* silva_forma.h - FORMAE: fragmenta C cum lacunis super arbores silvae
 * (transformare T1-T2; spec project-specs/transformare-spec.md)
 *
 * Forma = fragmentum C (expressio aut sententia) in quo '$nomen'
 * lacuna est (expressionem unam capit) et '$...nomen' reliquum
 * listae argumentorum (nullum aut plura). Fragmentum ut C parsatur
 * cum lexico latino (si, redde, ZEPHYRUM...): arbor POST expansionem
 * est, ergo forma 'si (f($x) != ZEPHYRUM)' et fons 'if (f(y) != 0)'
 * eadem arbore congruunt, dum capturae octetos fontis ipsius servant.
 *
 * Lacunae ante lexationem nomina reservata fiunt ('$x' -> '__trf_x',
 * '$...r' -> '__trfv_r') - numquam in plagulam veram perveniunt;
 * litterae chordarum et characterum intactae manent.
 */
#ifndef SILVA_FORMA_H
#define SILVA_FORMA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "silva_nodus.h"
#include "silva_parsare.h"

nomen enumeratio {
    SILVA_FORMA_EXPRESSIO = ZEPHYRUM,   /* fragmentum = expressio una */
    SILVA_FORMA_SENTENTIA               /* fragmentum = sententia una */
} SilvaFormaGenus;

/* lacuna formae: nomen sine '$' (et sine '...'), reliquum = '$...' */
nomen structura {
    chorda titulus;
       b32 reliquum;
} SilvaLacuna;

nomen structura {
        SilvaFormaGenus  genus;
           SilvaParsura* parsura;   /* arbor fragmenti involuti */
    constans SilvaNodus* nodus;    /* expressio aut sententia ipsa */
                    Xar* lacunae;   /* SilvaLacuna, ordine primae
                                    * occurrentiae, unicae */
} SilvaForma;

/* Fragmentum parsare. NIHIL + causa nominata si: lacuna deformis
 * ('$' sine nomine, '$...' sine nomine), fragmentum non parsatur
 * (errores), genus non congruit (EXPRESSIO quae sententia expressionis
 * non est; SENTENTIA non una). Textus non NUL-terminatus licet. */
SilvaForma*
silva_formam_parsare (
                Piscina* piscina,
     constans character* textus,
                    i32  mensura,
        SilvaFormaGenus  genus,
                 chorda* causa_out);

/* Estne nodus lacuna? (folium-identificator nominis reservati) -
 * titulus et reliquum ponuntur si non NIHIL. */
b32
silva_lacuna_est (
    constans SilvaNodus* nodus,
                 chorda* titulus_out,
                    b32* reliquum_out);

/* Locus nodi per NOMEN (registrum C89): NIHIL si nodus aut locus
 * ignotus. Ambulationes formarum loca nominant, numquam indices. */
constans SilvaValor*
silva_nodi_locus (
    constans SilvaNodus* nodus,
     constans character* titulus);

/* Nomen generis nodi (registrum C89); "" si ignotum */
constans character*
silva_nodi_genus_titulus (
    constans SilvaNodus* nodus);

#endif /* SILVA_FORMA_H */
