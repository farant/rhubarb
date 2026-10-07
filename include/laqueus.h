/* laqueus.h - Laquei polygonales EXACTI in Q^3: nodi et catenae
 *
 * Laqueus = componentes clausae polygonales, verticibus rationalibus
 * (situs.h). Omnis decisio geometrica exacta (situs): simplicitas,
 * transitus in proiectione (supra/infra, signum), motus trianguli.
 * Invariantes ex diagrammate: scriptura (writhe), numerus ligationis,
 * uncinus Kauffman, Jones, Alexander (polynomium, matrix).
 *
 * Segmentum i = a vertice i ad sequentem eiusdem componentis (ultimus
 * ad primum). Valores immutabiles in piscina; motus laqueum NOVUM
 * reddunt. Functiones laqueum simplicem praesumunt (laqueus_simplex
 * probat).
 *
 * USUS:
 *   Laqueus trifolium;
 *   Diagramma d;
 *   Polynomium v;
 *   (vacuum)laqueus_ex_chorda(chorda_ex_literis("[(0, 0, 0), ...]",
 *       piscina), piscina, &trifolium);
 *   si (laqueus_diagramma_genericum(trifolium, piscina, &d)
 *       && diagramma_jones(d, piscina, &v)) ... (-t^4 + t^3 + t)
 *
 * Vide lib/laqueus.worklog.md.
 */
/* <aedilis corpus="lib/laqueus.c"/> */
#ifndef LAQUEUS_H
#define LAQUEUS_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "fractio.h"
#include "situs.h"
#include "polynomium.h"

/* transitus plures uncinus per summam statuum (2^c) non computat */
#define LAQUEUS_TRANSITUS_MAXIMI XXIV

/* Membra PRIVATA - per functiones legenda */
nomen structura {
    constans Punctum* vertices;
                 i32  numerus;
        constans i32* initia;        /* componentes + 1 */
                 i32  componentes;
} Laqueus;

/* transitus in diagrammate: segmenta supra et infra, positiones in eis
 * (0 < s < 1), signum regulae dextrae (situs_transitus) */
nomen structura {
        i32 supra;
        i32 infra;
    Fractio parametrum_supra;
    Fractio parametrum_infra;
        s32 signum;
} Transitus;

/* Membra PRIVATA. percursus: transitus ordine laquei, elementum 2k
 * (transitus k supra) aut 2k + 1 (infra); initia_percursus per
 * componentes. */
nomen structura {
               Laqueus  laqueus;
               Punctum  directio;
    constans Transitus* transitus;
                   i32  numerus;
          constans i32* percursus;
          constans i32* initia_percursus;
} Diagramma;


/* ==================================================
 * Constructio et textus
 * ================================================== */

/* "[(x, y, z), (x, y, z), ...; (...)]": coordinatae fractiones,
 * componentes per ';', quaeque >= III vertices. FALSUM si malformatum
 * (exitus non tangitur). */
b32
laqueus_ex_chorda (
      chorda  textus,
     Piscina* piscina,
     Laqueus* exitus);

/* ex punctis: componens k = puncta[initia[k] .. initia[k+1] - 1]
 * (initia componentes + 1 elementa, ultimum = numerus); copiantur.
 * FALSUM si componens < III vertices. */
b32
laqueus_ex_punctis (
    constans Punctum* puncta,
        constans i32* initia,
                 i32  componentes,
             Piscina* piscina,
             Laqueus* exitus);

chorda
laqueus_ad_chordam (
     Laqueus  l,
     Piscina* piscina);

i32
laqueus_numerus (
    Laqueus l);

i32
laqueus_componentes (
    Laqueus l);

Punctum
laqueus_vertex (
    Laqueus l,
        i32 i);

/* nulla segmenta se tangunt nisi contigua in vertice communi (et ea
 * solum ibi) */
b32
laqueus_simplex (
     Laqueus  l,
     Piscina* piscina);


/* ==================================================
 * Diagramma: proiectio secundum v
 * ================================================== */

/* FALSUM si proiectio non generica (contactus non proprius, segmenta
 * contigua superposita, transitus duo in eodem puncto segmenti): vocans
 * directionem mutat */
b32
laqueus_diagramma (
        Laqueus  l,
        Punctum  v,
        Piscina* piscina,
      Diagramma* exitus);

/* directiones ex serie fixa deterministica ((0,0,1), (1,2,3), ...),
 * deinde (1, k, k^2) pro k = 1, 2, ... donec generica; FALSUM si
 * laqueus non simplex (nulla directio generica). Conatus irriti
 * reficiuntur. */
b32
laqueus_diagramma_genericum (
        Laqueus  l,
        Piscina* piscina,
      Diagramma* exitus);

i32
diagramma_numerus (
    Diagramma d);

Transitus
diagramma_transitus (
    Diagramma d,
          i32 k);


/* ==================================================
 * Invariantes
 * ================================================== */

/* scriptura (writhe): summa signorum */
s32
diagramma_scriptura (
    Diagramma d);

/* numerus ligationis componentium a != b: (summa signorum inter eas) /
 * 2 */
s32
diagramma_numerus_ligationis (
    Diagramma d,
          i32 a,
          i32 b);

/* uncinus Kauffman D in A (circulus nodatus O -> 1), per summam
 * statuum; FALSUM si transitus > LAQUEUS_TRANSITUS_MAXIMI */
b32
diagramma_uncinus (
     Diagramma  d,
       Piscina* piscina,
    Polynomium* exitus);

/* Jones V(t) = f(A = t^(-1/4)), f = (-A^3)^(-w) uncinus(D); FALSUM si
 * exponentes dimidii (catenae componentium numeri paris - ante uncinum
 * refutatur) aut transitus nimii */
b32
diagramma_jones (
     Diagramma  d,
       Piscina* piscina,
    Polynomium* exitus);

/* Alexander per praesentationem Wirtinger (calculus Fox, minor
 * matricis): nodi soli (FALSUM pro catenis); forma normalis ad +-t^k */
b32
diagramma_alexander (
     Diagramma  d,
       Piscina* piscina,
    Polynomium* exitus);


/* ==================================================
 * Motus trianguli
 * ================================================== */

/* segmentum i -> (v_i, c, v_sequens) sse triangulum nihil aliud tangit
 * (situs_triangulum_segmentum, _vicinum) et non degener */
b32
laqueus_motus_addere (
     Laqueus  l,
         i32  i,
     Punctum  c,
     Piscina* piscina,
     Laqueus* exitus);

/* vertex i removetur (v_prior -> v_sequens) sse triangulum nihil aliud
 * tangit; vertex collinearis inter vicinos semper; componens >= IV
 * vertices requirit */
b32
laqueus_motus_removere (
     Laqueus  l,
         i32  i,
     Piscina* piscina,
     Laqueus* exitus);

#endif /* LAQUEUS_H */
