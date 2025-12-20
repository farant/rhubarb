#ifndef ITER_DIRECTORIA_H
#define ITER_DIRECTORIA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"

/* ==================================================
 * ITER_DIRECTORIA - Iteratio Directoriorum
 *
 * Bibliotheca pro iteratione directoriorum.
 * Integrat cum piscina, chorda, et via.
 *
 * EXEMPLUM SIMPLEX:
 *   Piscina* p = piscina_generare_dynamicum("iter", 4096);
 *   DirectoriumIterator* iter = directorium_iterator_aperire("/tmp", p);
 *
 *   DirectoriumIntroitus* e;
 *   dum ((e = directorium_iterator_proximum(iter)) != NIHIL) {
 *       si (e->genus == INTROITUS_FILUM) {
 *           // processus filum
 *       }
 *   }
 *   directorium_iterator_claudere(iter);
 *
 * EXEMPLUM CUM AMBULATORE:
 *   s32 mea_functio(chorda via, constans DirectoriumIntroitus* e, vacuum* ctx) {
 *       // processus introitus
 *       redde 0; // perge
 *   }
 *   directorium_ambulare("/home", NIHIL, mea_functio, NIHIL, p);
 *
 * ================================================== */


/* ==================================================
 * Genera Introituum
 * ================================================== */

/* Genus introitus in directorio */
nomen enumeratio {
    INTROITUS_FILUM,        /* Filum regulare */
    INTROITUS_DIRECTORIUM,  /* Directorium */
    INTROITUS_ALIUS,        /* Alius (symlink, dispositum, etc.) */
    INTROITUS_IGNOTUS       /* Genus ignotum */
} IntroitusGenus;


/* ==================================================
 * Introitus Directorii
 * ================================================== */

/* Informatio de introitu in directorio */
nomen structura {
    chorda         titulus;  /* Titulus introitus (non via plena) */
    IntroitusGenus genus;    /* Genus introitus */
    memoriae_index mensura;  /* Mensura fili in bytes (0 pro directoriis) */
} DirectoriumIntroitus;


/* ==================================================
 * Iterator
 * ================================================== */

/* Manipulus iteratoris (opacus) */
nomen structura DirectoriumIterator DirectoriumIterator;

/* Aperire directorium pro iteratione
 *
 * via:     Via ad directorium
 * piscina: Piscina pro allocationibus
 *
 * Reddit: DirectoriumIterator* si successus, NIHIL si error
 */
DirectoriumIterator*
directorium_iterator_aperire(
    constans character* via,
    Piscina*            piscina);

/* Obtinere proximum introitum
 *
 * iterator: Iterator apertus
 *
 * Reddit: DirectoriumIntroitus* si est introitus,
 *         NIHIL si finis vel error
 *
 * NOTA: Pointer validus usque ad proximum vocatum
 *       directorium_iterator_proximum vel claudere
 */
DirectoriumIntroitus*
directorium_iterator_proximum(
    DirectoriumIterator* iterator);

/* Claudere iterator
 *
 * iterator: Iterator ad claudendum
 *
 * NOTA: Memoria liberata cum piscina destruitur
 */
vacuum
directorium_iterator_claudere(
    DirectoriumIterator* iterator);


/* ==================================================
 * Filtrum pro Ambulatore
 * ================================================== */

/* Configuratio filtri pro ambulatore recursivo */
nomen structura {
    IntroitusGenus* genera_accepta;     /* Array generum acceptorum (NIHIL = omnia) */
    i32             genera_numerus;     /* Numerus in genera_accepta */
    chorda          exemplar;           /* Exemplar nominis (* et ? supportati, vacua = omnia) */
    s32             profunditas_max;    /* Profunditas maxima (0 = sine limite, -1 = sine recursione) */
    b32             includere_occultos; /* Includere fila occulta (. prefix)? */
} DirectoriumFiltrum;

/* Creare filtrum quod omnia acceptat
 *
 * Reddit: DirectoriumFiltrum cum valoribus default
 */
DirectoriumFiltrum
directorium_filtrum_omnia(
    vacuum);


/* ==================================================
 * Ambulator Recursivus
 * ================================================== */

/* Genus functionis callback pro ambulatore
 *
 * via_plena:  Via plena ad introitum
 * introitus:  Informatio de introitu
 * contextus:  Contextus ab usore provisus
 *
 * Reddit: 0 ad pergendum, non-0 ad terminandum
 */
nomen s32 (*DirectoriumAmbulatorFunctio)(
    chorda                         via_plena,
    constans DirectoriumIntroitus* introitus,
    vacuum*                        contextus);

/* Ambulare per directorium recursive
 *
 * via:       Via ad directorium initiale
 * filtrum:   Filtrum (NIHIL = omnia accepta)
 * functio:   Functio callback pro quoque introitu
 * contextus: Contextus ad functionem passus
 * piscina:   Piscina pro allocationibus
 *
 * Reddit: 0 si successus, -1 si error
 *
 * NOTA: Callback reddit 0 ad pergendum, non-0 ad terminandum
 */
s32
directorium_ambulare(
    constans character*            via,
    constans DirectoriumFiltrum*   filtrum,
    DirectoriumAmbulatorFunctio    functio,
    vacuum*                        contextus,
    Piscina*                       piscina);


/* ==================================================
 * Utilitates
 * ================================================== */

/* Verificare si via est directorium
 *
 * via: Via ad verificandum
 *
 * Reddit: VERUM si est directorium, FALSUM alioquin
 */
b32
directorium_existit(
    constans character* via);

/* Verificare si titulus congruit cum exemplari
 * Supportat * (omnia) et ? (unus character)
 *
 * titulus:  Titulus ad verificandum
 * exemplar: Exemplar (* et ? supportati)
 *
 * Reddit: VERUM si congruit, FALSUM alioquin
 *
 * EXEMPLA:
 *   "file.txt" congruit "*.txt"     -> VERUM
 *   "file.txt" congruit "file.???"  -> VERUM
 *   "abc.txt"  congruit "*.md"      -> FALSUM
 */
b32
directorium_titulus_congruit(
    chorda titulus,
    chorda exemplar);


/* ==================================================
 * Constantae
 * ================================================== */

/* Profunditas maxima recursiva (contra cyclos symlink) */
#define DIRECTORIUM_PROFUNDITAS_MAXIMA C

#endif /* ITER_DIRECTORIA_H */
