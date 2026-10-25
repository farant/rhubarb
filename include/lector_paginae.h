/* lector_paginae.h - caput codicis (codex.h) in paginas (lector L3;
 * spec project-specs/lector-spec.md VI.2, VI.10). Paragraphi toti
 * fluunt: verba ad spatia ASCII franguntur, verbum linea longius ad
 * runam secatur. Titulus in FONS_TITULI (8x16), cetera FONS_PARVUS
 * (6x8); versus linea per lineam, continuatio II cellulis indentata;
 * lineae textus spatio duplici (VIII px vacui post quamque, non post
 * ultimam paginae); inter clausulas linea vacua VIII px addita (non in
 * summa pagina). Functio
 * pura magnitudinis: eadem area -> eaedem paginae. Pagina saltem
 * lineam unam habet; caput sine textu = pagina una vacua. */

#ifndef LECTOR_PAGINAE_H
#define LECTOR_PAGINAE_H

/* <aedilis corpus="lib/lector_paginae.c"/> */

#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "codex.h"

nomen structura {
    i32 initium;   /* octeti in textu capitis */
    i32 finis;     /* exclusivus, spatia finalia dempta */
    i32 fons;      /* FONS_TITULI aut FONS_PARVUS */
    s32 x;         /* px in area textus */
    s32 y;
} LectorLinea;

nomen structura {
     Xar* lineae;    /* LectorLinea */
     i32  initium;   /* octetus primus (lineae primae) */
} LectorPagina;

/* caput in paginas areae latitudo x altitudo (px); Xar de
 * LectorPagina, saltem una */
Xar*
lector_paginare (
    constans CodexCaput* caput,
                    i32  latitudo,
                    i32  altitudo,
                Piscina* piscina);

/* index paginae ubi octetus positio iacet (initium paginae <= positio
 * < initium sequentis); ultra finem -> ultima */
i32
lector_pagina_positionis (
    Xar* paginae,
    i32  positio);

/* sententia (aut cursus sententiarum: initium primae, finis ultimae)
 * in fonte dato ad latitudinem involuta; '\n' linea nova. Xar de
 * LectorLinea (x = y = 0) */
Xar*
lector_sententiam_involvere (
        constans CodexCaput* caput,
    constans CodexSententia* sententia,
                        i32  fons,
                        i32  latitudo,
                    Piscina* piscina);

/* sententia sola (visus sententiae) in area latitudo x altitudo:
 * FONS_TITULI si tota capit, aliter FONS_PARVUS spatio duplici; grex
 * centratus (per lineam longissimam et verticaliter), lineae intra eum
 * ad sinistrum; '\n' (titulus, versus) linea nova. Non capit: lineae
 * quae capiunt et *praecisa VERUM. Xar de LectorLinea (x, y in area) */
Xar*
lector_sententiam_disponere (
        constans CodexCaput* caput,
    constans CodexSententia* sententia,
                        i32  latitudo,
                        i32  altitudo,
                        b32* praecisa,
                    Piscina* piscina);

#endif /* LECTOR_PAGINAE_H */
