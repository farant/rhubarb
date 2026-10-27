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
 * (errores), genus non congruit (EXPRESSIO intra 'redde ...;' parsatur:
 * sententia data ut EXPRESSIO non parsatur; SENTENTIA non una). Textus
 * non NUL-terminatus licet. LIMES: SENTENTIA 'f($x);' nuda ambigua est
 * (declaratio sine prototypo) - vocatio nuda forma EXPRESSIO 'f($x)'
 * scribitur et contra expressionem sententiae congruit. */
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


/* ==================================================
 * Congruentia (transformare T2)
 * ================================================== */

/* captura lacunae: '$x' -> nodus (subarbor fontis ipsa, non copia);
 * '$...r' -> elementa (Xar de MateriaValor: nodi et commata listae
 * fontis, ordine; vacua licet) */
nomen structura {
                 chorda  titulus;
                    b32  reliquum;
    constans SilvaNodus* nodus;
                    Xar* elementa;
} SilvaCaptura;

/* Congruitne forma cum nodo? Comparatio per genera et valores
 * lexematum (trivia - spatia, commentaria - neglecta; arbor post
 * expansionem: 'si'/'if' et 'ZEPHYRUM'/'0' aequalia). '$x' quemlibet
 * nodum capit; '$x' iteratus subarborem aequalem poscit; '$...r'
 * elementum ULTIMUM listae reliquum capit (nullum licet - tunc comma
 * praecedens formae quoque vacuum congruit). capturae (Xar de
 * SilvaCaptura, a vocatore creata) evacuatur et impletur; NIHIL
 * licet. Congruentia fracta capturas partiales relinquere potest. */
b32
silva_forma_congruit (
                Piscina* piscina,
    constans SilvaForma* forma,
    constans SilvaNodus* nodus,
                    Xar* capturae);

/* Aequalitas subarborum trivia neglecta (genera, loci, valores
 * lexematum) - lacunae ut identificatores ordinarii */
b32
silva_nodi_aequales (
    constans SilvaNodus* a,
    constans SilvaNodus* b);

#endif /* SILVA_FORMA_H */
