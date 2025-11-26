#ifndef INTERNAMENTUM_H
#define INTERNAMENTUM_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "tabula_dispersa.h"

/* ==================================================
 * Internamentum Chordae - String Interning
 * "Conservare unicam copiam cuiusque chordae"
 * ================================================== */

/* Tabula internamenti globalis */
nomen structura {
    TabulaDispersa* tabula;  /* chorda -> chorda* (canonica) */
    Piscina* piscina;        /* Omnes chordae internatae hic allocantur */
} InternamentumChorda;


/* ==================================================
 * Singleton Globale
 * ================================================== */

/* Obtinere internamentum globale (singleton)
 * Creatur pigre in primo usu cum piscina propria.
 * Utile quando omnes bibliothecae debent eandem
 * tabulam internamenti participare.
 */
InternamentumChorda*
internamentum_globale(vacuum);

/* ==================================================
 * Creatio
 * ================================================== */

/* Creare novum internamentum (pro casibus specialibus)
 * Plerumque melius est uti internamentum_globale()
 */
InternamentumChorda*
internamentum_creare(
    Piscina* piscina);


/* ==================================================
 * Internatio
 * ================================================== */

/* Internare chordam - redde pointer ad copiam canonicam */
chorda*
chorda_internare(
    InternamentumChorda* intern,
    chorda s);

/* Convenientia: internare ex literis C */
chorda*
chorda_internare_ex_literis(
    InternamentumChorda* intern,
    constans character* cstr);


/* ==================================================
 * Quaestio
 * ================================================== */

/* Verificare si chorda iam internata est */
b32
chorda_est_internata(
    InternamentumChorda* intern,
    chorda s);


/* ==================================================
 * Statisticae
 * ================================================== */

/* Numerum chordaram internatarum obtinere */
i32
internamentum_numerus(
    InternamentumChorda* intern);

/* Vacare omnes chordas internatas (reset tabula) */
vacuum
internamentum_vacare(
    InternamentumChorda* intern);

#endif /* INTERNAMENTUM_H */
