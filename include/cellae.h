/* cellae.h - Cellae activationis combinatae (sector x axis) super
 * catalogum quaternionum EXACTUM
 *
 * Machina communis demonstrationum activationis (knotapel D82, D84 et
 * sequentium): catalogus per clausuram, tabula axium, cella summae
 * exacta (regula et paritates), separabilitas functionum paritatis,
 * quaestiones avida et exhaustiva. Planum: project-specs/cellae-plan.md.
 *
 * CATALOGUS: clausura BFS generatorum, ORDINE certo (ordo est pars
 * semanticae: tabulam axium, regulam axis primi maximi et ordinem
 * quaestionis avidae figit):
 *   - elementum 0 = 1 (profunditas 0, torsio 0);
 *   - generatores ordine dato, si nondum adsunt (profunditas 0, torsio
 *     sua);
 *   - gradus r = 1, 2, ...: omne elementum i ante initium gradus, ordine,
 *     per omnem generatorem g ordine: i g, si nondum adest (profunditas
 *     r, torsio i + torsio g);
 *   - aequalitas AD SIGNUM: q et -q eadem rotatio, unum elementum;
 *   - desinit si gradus nihil addit, aut post gradus_maximi gradus
 *     crescentes (gradu 0 incluso); limes: ultra eum nihil additur, etiam
 *     in medio gradu, et catalogus TRUNCATUS notatur (nihil tacite).
 * D82/D84: generatores (g1, g1*, g2, g2*), torsiones (+1, -1, +1, -1),
 * limes MMMMXCVI, gradus maximi XVI.
 *
 * Anulus ORDINATUS postulatur (signum: repraesentans q aut -q per
 * signum partis primae non nullae). Valores in piscina data.
 *
 * Vide lib/cellae.worklog.md.
 */
/* <aedilis corpus="lib/cellae.c"/> */
#ifndef CELLAE_H
#define CELLAE_H

#include "latina.h"
#include "piscina.h"
#include "quaternio.h"

nomen structura Catalogus Catalogus;

/* clausura (vide supra); limes 0 / gradus_maximi 0: sine limite.
 * FALSUM si numerus 0, generator anulo alieno aut sine ordine, aut
 * operatio refutata; exitus non tangitur. */
b32
catalogus_clausura (
    constans Quaternio*  generatores,
          constans s32*  torsiones,
                   i32   numerus,
                   i32   limes,
                   i32   gradus_maximi,
               Piscina*  piscina,
             Catalogus** exitus);

i32
catalogus_numerus (
    constans Catalogus* catalogus);

/* i extra [0, numerus): quaternio cum anulo NIHIL */
Quaternio
catalogus_elementum (
    constans Catalogus* catalogus,
                   i32  i);

/* gradus quo elementum natum est (1 et generatores: 0); -1 extra */
i32
catalogus_profunditas (
    constans Catalogus* catalogus,
                   i32  i);

s32
catalogus_torsio (
    constans Catalogus* catalogus,
                   i32  i);

/* pars realis nulla (Re q = 0: semiangulus 90 gradus) */
b32
catalogus_nullum (
    constans Catalogus* catalogus,
                   i32  i);

/* limes aliquid novum refutavit */
b32
catalogus_truncatus (
    constans Catalogus* catalogus);

/* gradus crescentes, gradu 0 incluso (D82 n_rounds) */
i32
catalogus_gradus (
    constans Catalogus* catalogus);

/* catalogus novus ex indicibus, ORDINE indicum (profunditas, torsio
 * servantur; truncatus hereditur). FALSUM si index extra aut
 * repetitus. */
b32
catalogus_pars (
    constans Catalogus*  catalogus,
          constans i32*  indices,
                   i32   numerus,
               Piscina*  piscina,
             Catalogus** exitus);


/* ==================================================
 * Activatio: tabula axium et cella summae (semantica D82/D84
 * combined_cell, exacte)
 *
 * Tabula: axes partium vectoriarum catalogi, ordine primae apparitionis,
 * antipodes idem; axes_maximae (0 omnes) secat et TRUNCATAM notat (D82:
 * D). Cella summae s = (a, v) ad k, nd = numerus axium:
 *   - s = 0: (k - 1)(nd + 1) + nd;
 *   - v = 0: sector 0, axis nd (cella nd);
 *   - a = 0 (nulla): sector floor(k/2); k par: paritas cum k/2 - 1;
 *   - aliter: sector = #{m < k/2 : tan^2 h >= tan^2(m pi/k)}, tan^2 h =
 *     |v|^2 / a^2; paritas si aequalitas ad aliquod m (sector m vel
 *     m - 1);
 *   - axis: primus maximus (v.u)^2 / |u|^2 (quaternio_proximi);
 *     paritas: omnes maximi.
 * cella = sector (nd + 1) + axis; regula = sector floor, axis primus;
 * ligatae = omnes combinationes (regula inclusa).
 *
 * v1: corpus QUADRATICUM ordinatum (Q(sqrt d)): tan^2 h radix
 * polynomii t^2 - Tr t + N, termini tan^2(m pi/k) radices S_k(u) = sum
 * C(k, 2l+1) (-1)^l u^l per POSITIONEM (radices). Aliter FALSUM.
 * Memoria cellarum per punctum exactum (s et -s idem) in piscina
 * activationis.
 * ================================================== */

nomen structura Activatio Activatio;

nomen structura {
             i32  regula;
             i32  numerus;     /* ligatae, regula inclusa */
    constans i32* ligatae;     /* valida dum activatio vivit */
} Cella;

/* FALSUM si corpus non quadraticum ordinatum, aut catalogus sine axe */
b32
activatio_ex_catalogo (
    constans Catalogus*  catalogus,
                   i32   axes_maximae,
               Piscina*  piscina,
             Activatio** exitus);

i32
activatio_axes (
    constans Activatio* activatio);

/* axes_maximae axem novum refutavit */
b32
activatio_truncata (
    constans Activatio* activatio);

/* k >= 1; summa in anulo catalogi; FALSUM aliter */
b32
activatio_cella (
     Activatio* activatio,
     Quaternio  summa,
           i32  k,
         Cella* exitus);

#endif /* CELLAE_H */
