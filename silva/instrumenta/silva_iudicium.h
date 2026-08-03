/* silva_iudicium.h - recipe iudicii examinis ut modulus communis
 *
 * Examen.c praefationem suam nominat: "geminus tertius oneratorum
 * (percursus + vindex_onerator + hic) - migratio ad fontem communem
 * = parca nominata, tactu proximo." Emendare (2026-08-03) quartum
 * geminum pareret - ergo recipe HUC extrahitur; emendare consumit,
 * examen tactu SUO proximo migrat.
 *
 * Bifasium consulto: praeparare ambulationem capitum repositorii
 * SEMEL facit (pretium percursus per plagulam maximum), iudicare
 * per plagulam contextum recentem struit (lexicon per plagulam
 * derivatur - contextus communicari non potest) sed capita e
 * repositione, non e disco.
 *
 * Recipe fidele examinis: lexicon compositum (ISO + POSIX derivatum
 * + externa plagulae) -> systema parsatum + analysatum -> capita
 * praebita -> parsura cum contextu -> custodia finium -> oraculum
 * praeseminatum + BIS-analysis post recanonicare.
 */

#ifndef SILVA_IUDICIUM_H
#define SILVA_IUDICIUM_H

#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "silva_parsare.h"
#include "silva_c89_semantica.h"

nomen structura {
    Piscina*   piscina;
    Xar*       capita;       /* IudiciumCaput per valorem */
    character* fons_iso;
    i32        mensura_iso;
    character* fons_px;
    i32        mensura_px;
} IudiciumApparatus;

nomen structura {
    character* via;          /* NUL-terminata (piscina) */
    character* textus;
    i32        mensura;
} IudiciumCaput;

nomen structura {
    SilvaParsura*   parsura;
    SilvaSemantica* sem;
    b32             apparatus_fractus;  /* INFRA - iudicium nullum */
    b32             recusatum;          /* fines tactae (RECUSO) */
} IudiciumFructus;

/* Ambulatio radicis ("."), systema lecta. NIHIL = apparatus deest.
 * Ex radice repositorii vocandum (viae systematis relativae). */
IudiciumApparatus*
silva_iudicium_praeparare (Piscina* piscina);

/* Plagulam UNAM iudicare (fons in memoria - emendator textum
 * mutatum sine disco iudicat). VERUM = iudicium exstat (fructus->sem
 * lectilis); FALSUM = apparatus fractus aut recusatum (vexilla
 * dicunt quid).
 *
 * 'opus' = piscina iudicii huius (contextus/parsurae/semantica) -
 * vocator per plagulam creat et destruit, ne arena trans plagulas
 * multas cumuletur; apparatus in piscina SUA vivit et supervivit. */
b32
silva_iudicium_iudicare (IudiciumApparatus* app, Piscina* opus,
    constans character* via, constans character* fons, i32 mensura,
    IudiciumFructus* fructus);

/* Plagulam e disco legere (auxiliaris communis) */
character*
silva_iudicium_plagulam_legere (Piscina* piscina,
    constans character* via, i32* mensura_out);

#endif /* SILVA_IUDICIUM_H */
