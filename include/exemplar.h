#ifndef EXEMPLAR_H
#define EXEMPLAR_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"

/* ==================================================
 * EXEMPLAR - Bibliotheca Congruendi Exemplarium
 *
 * Regex-subset pro congruendo exemplarium in chordis.
 * Syntaxis compatibilis cum regex, sed simplicior.
 *
 * SYNTAXIS SUPPORTATA:
 *   .        Quilibet character singularis
 *   *        Zero vel plus praecedentis
 *   +        Unus vel plus praecedentis
 *   ?        Zero vel unus praecedentis
 *   [abc]    Classis characterum
 *   [a-z]    Intervallum characterum
 *   [^abc]   Classis negata
 *   ^        Ancora initii
 *   $        Ancora finis
 *   \        Effugium proximi characteris
 *
 * EXEMPLUM:
 *   Piscina* p = piscina_generare_dynamicum("ex", 4096);
 *   Exemplar* ex = exemplar_compilare(
 *       chorda_ex_literis("[0-9]+", p), NIHIL, p);
 *
 *   ExemplarFructus f = exemplar_invenire(ex,
 *       chorda_ex_literis("Anno 1847", p));
 *   si (f.congruit) {
 *       // f.initium = 5, f.longitudo = 4
 *   }
 *
 * ALGORITHMUS: Thompson NFA - O(n*m) tempus garantitum
 *
 * ================================================== */


/* ==================================================
 * Status Compilationis
 * ================================================== */

nomen enumeratio {
    EXEMPLAR_OK,                        /* Successus */
    EXEMPLAR_ERROR_SYNTAXIS,            /* Syntaxis mala */
    EXEMPLAR_ERROR_CLASSIS_NON_CLAUSA,  /* [ sine ] */
    EXEMPLAR_ERROR_EFFUGIUM_FINIS,      /* \ ad finem */
    EXEMPLAR_ERROR_CLASSIS_VACUA,       /* [] vacua */
    EXEMPLAR_ERROR_ORDO_MALUS           /* [z-a] inversus */
} ExemplarStatus;


/* ==================================================
 * Fructus Congruendi
 * ================================================== */

nomen structura {
    b32 congruit;   /* VERUM si congruentia inventa */
    i32 initium;    /* Index initii congruentiae (0-based) */
    i32 longitudo;  /* Longitudo congruentiae */
} ExemplarFructus;


/* ==================================================
 * Exemplar Compilatum (Opacum)
 * ================================================== */

nomen structura Exemplar Exemplar;


/* ==================================================
 * Compilatio
 * ================================================== */

/* Compilare exemplar ex chorda
 *
 * exemplar_textus: Chorda exemplaris (e.g., "[0-9]+")
 * status:          Pointer ad statum erroris (potest esse NIHIL)
 * piscina:         Piscina pro allocationibus
 *
 * Redde: Exemplar* si successus, NIHIL si error
 */
Exemplar*
exemplar_compilare(
    chorda          exemplar_textus,
    ExemplarStatus* status,
    Piscina*        piscina);


/* ==================================================
 * Congruere
 * ================================================== */

/* Congruere totam chordam (implicite ^...$)
 *
 * exemplar: Exemplar compilatum
 * textus:   Chorda ad congruendum
 *
 * Redde: VERUM si tota chorda congruit
 */
b32
exemplar_congruit_totum(
    constans Exemplar* exemplar,
    chorda             textus);

/* Invenire primam congruentiam in chorda
 *
 * exemplar: Exemplar compilatum
 * textus:   Chorda in qua quaerere
 *
 * Redde: ExemplarFructus cum positione (congruit=FALSUM si non inventum)
 */
ExemplarFructus
exemplar_invenire(
    constans Exemplar* exemplar,
    chorda             textus);

/* Invenire omnes congruentias in chorda
 *
 * exemplar:     Exemplar compilatum
 * textus:       Chorda in qua quaerere
 * fructus_out:  Pointer ad array fructuum (allocatur ex piscina)
 * piscina:      Piscina pro allocationibus
 *
 * Redde: Numerum congruentiarum inventarum
 */
i32
exemplar_invenire_omnes(
    constans Exemplar*  exemplar,
    chorda              textus,
    ExemplarFructus**   fructus_out,
    Piscina*            piscina);


/* ==================================================
 * Functiones Commodae
 * ================================================== */

/* Compilare et congruere in uno passu
 * Commodum quando exemplar non reutilizatur
 *
 * exemplar_cstr: C-string exemplaris
 * textus:        Chorda ad congruendum
 * piscina:       Piscina pro allocationibus temporariis
 *
 * Redde: VERUM si congruentia inventa
 */
b32
exemplar_congruit_literis(
    constans character* exemplar_cstr,
    chorda              textus,
    Piscina*            piscina);

/* Convertere statum ad descriptionem humanam
 *
 * status: Status erroris
 *
 * Redde: C-string descriptionis (staticum, non liberandum)
 */
constans character*
exemplar_status_descriptio(
    ExemplarStatus status);


/* ==================================================
 * Constantae
 * ================================================== */

/* Numerus maximus statuum NFA */
#define EXEMPLAR_STATUS_MAXIMUS CCLVI

/* Numerus maximus classium characterum */
#define EXEMPLAR_CLASSES_MAXIMAE XXXII

#endif /* EXEMPLAR_H */
