/* md_extenta.h - Extenta structuralia markdown per selectorem (C1)
 *
 * Fons markdown parsatur (md_arbor), in STML proicitur (consilium md),
 * documentum legitur (stml_legere) et selector CSS domus (selectio.h -
 * idem quo silva/selecta.sh) super arborem STML quaerit. Elementum
 * congruens in OCTETOS fontis redditur per CORRESPONDENTIAM lexematum:
 * lexemata arboris materiae (ordine praevio) et elementa lexematum
 * documenti STML (tag praefixo lexici, eodem ordine) I:I respondent -
 * scriptor et lector eundem ordinem ambulant; correspondentia in
 * quoque cursu PROBATUR (numerus idem, tag = praefixum + titulus
 * generis lexematis cuiusque) et fracta CLAMAT, numquam extenta
 * mendacia reddit. Extentum = [octetus primus lexematis fontis primi,
 * post octetum ultimum lexematis fontis ultimi) subarboris; lexemata
 * DERIVATA (fons I) octetos non tenent et praetereuntur. Elementum
 * sine lexematibus fontis congruit sed extentum non habet.
 *
 * Gemellum formatoris '-extenta' quod Prosa pythonicae legit
 * (md/extenta.sh, spec par. VIII/IX).
 */

#ifndef MD_EXTENTA_H
#define MD_EXTENTA_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "internamentum.h"
#include "xar.h"

nomen structura {
    constans character* tag;      /* titulus elementi congruentis */
                   s32  initium;  /* octetus primus in fonte */
                   s32  finis;    /* post octetum ultimum */
                   i32  linea;    /* I-basata, lexematis primi */
                   i32  columna;  /* I-basata, lexematis primi */
                   i32  linea_finis;   /* lexematis ultimi */
} MdExtentum;

nomen structura {
       b32  successus;
    chorda  causa;          /* vitium nominatum (aut vacua) */
       Xar* extenta;        /* MdExtentum, ordine congruentiarum */
       i32  congruentiae;   /* elementa congruentia, etiam sine octetis */
       i32  lexemata;       /* lexemata correspondentia probata */
} MdExtentaResultus;

MdExtentaResultus
md_extenta_quaerere (
                Piscina* piscina,
    InternamentumChorda* intern,
     constans character* fons,
                    i32  mensura,
     constans character* selector);

#endif /* MD_EXTENTA_H */
