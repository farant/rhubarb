/* interpolatio.h - Aestimatio, interpolatio, restitutio EXACTA
 *
 * Polynomium (Laurentianum) in Z[t, t^-1] ex valoribus eius modulo
 * primos: per primum valores in punctis colliguntur, interpolatio
 * Lagrange modulo p coefficientes dat, deinde restitutio Sinica
 * (congruentia_restitue) trans primos coefficientes integros reddit.
 * Ita computatio gravis (e.g. contractio retis) in arithmetica i32
 * fit et effectus tamen exactus est.
 *
 * EXACTITUDO. Numerus primorum ex LIMITE PROBATO coefficientium
 * (limes_bitorum, a vocante demonstratus) sequitur, numquam ex eo
 * quod responsum mutari desiit. Probationes additae (puncta addita
 * per primum, primus unus additus post restitutionem) culpas
 * arithmeticae, limitis aut fenestrae capiunt - non exemplar falsum
 * sed sibi constans, quod solum oraculum alienum capit.
 *
 * PUNCTA. Aestimator punctum ipse ELIGIT (index -> punctum, valor):
 * ita puncta sine radice quadrata modulari enumerari possunt (uncinus
 * per modulum vertex: omega = k + 2, A = i omega^2). Aestimator tres
 * status reddit - UTILE, INUTILE (hic index nihil dat), ERRATUM
 * (defectus verus, statim FALSUM) - et conatus per primum
 * finiuntur.
 *
 * PASSUS. f(x) = summa c_k x^(imus + passus k): interpolatur g(y) =
 * x^(-imus) f(x) in y = x^passus, ita exponentes in una classe modulo
 * passus (uncinus Kauffman: passus 4) puncta (summus - imus)/passus +
 * 1 tantum poscunt. Gradus 0 (imus == summus == 0) = restitutio
 * scalaris: numerus integer trans primos.
 *
 * USUS:
 *   InterpolatioQuaestio q = interpolatio_quaestio_ordinaria();
 *   q.gradus_imus    = -XII;
 *   q.gradus_summus  = XII;
 *   q.passus         = IV;
 *   q.limes_bitorum  = XVI;
 *   si (!interpolatio_polynomium(aestimare, &contextus, &q, piscina,
 *       &f)) refutare;
 *
 * Vide lib/interpolatio.worklog.md.
 */
/* <aedilis corpus="lib/interpolatio.c"/> */
#ifndef INTERPOLATIO_H
#define INTERPOLATIO_H

#include "latina.h"
#include "piscina.h"
#include "polynomium.h"


/* ==================================================
 * Interpolatio Lagrange modulo p
 * ================================================== */

/* numerus puncta DISTINCTA x (0 <= x < p, p primus) et valores y (0 <=
 * y < p) -> coefficientes c[0 .. numerus - 1] (c[j] pro t^j), unicum
 * polynomium gradus < numerus. FALSUM si numerus nullus, p non
 * primus, punctum aut valor >= p, aut puncta non distincta
 * (coefficientes tunc non tanguntur). Sumptus O(numerus^2). */
b32
interpolatio_lagrange_modulo (
    constans i32* x,
    constans i32* y,
             i32  numerus,
             i32  p,
         Piscina* piscina,
             i32* coefficientes);


/* ==================================================
 * Aestimatio et restitutio
 * ================================================== */

#define INTERPOLATIO_UTILE    I         /* punctum et valor scripti */
#define INTERPOLATIO_INUTILE  ZEPHYRUM  /* hic index nihil: proximum */
#define INTERPOLATIO_ERRATUM  (-I)      /* defectus verus: FALSUM statim */

/* limes_bitorum maximus admissus (2^24): ultra nullus usus, et summa
 * bitorum primorum in s64 tuta manet */
#define INTERPOLATIO_LIMES_MAXIMUS  ((i32)0x1000000UL)

/* aestimator: pro indice (0, 1, 2, ... intra primum) punctum x (0 <= x
 * < p) eligit et valorem f(x) mod p (0 <= valor < p) scribit, UTILE
 * reddens. Punctum 0 ut INUTILE omittitur (x^passus nihil
 * distinguit); punctum >= p aut valor >= p ut ERRATUM tractatur.
 * Status alius quam UTILE aut INUTILE = ERRATUM. */
nomen s32 (*InterpolatioAestimator) (
    vacuum* contextus,
       i32  index,
       i32  p,
       i32* punctum,
       i32* valor);

/* Membra a vocante post interpolatio_quaestio_ordinaria() posita:
 * membra futura ita valores ordinarios semper habebunt. */
nomen structura {
    s32 gradus_imus;      /* exponens minimus possibilis (0) */
    s32 gradus_summus;    /* maximus possibilis (0); (summus - imus) %
                           * passus == 0 */
    i32 passus;           /* exponentes imus + passus k (1) */
    i32 limes_bitorum;    /* |c_k| < 2^limes_bitorum PROBATUM (0 =
                           * omnes nulli) */
    i32 residuum_primi;   /* primi p = residuum (mod modulus_primi) (0);
                           * mdc(residuum mod modulus, modulus) == 1
                           * debet, aliter quaestio invalida */
    i32 modulus_primi;    /* < 2 = omnes primi (0) */
    i32 primus_limes;     /* primi < primus_limes descendentes
                           * (0xFFFFFFFF) */
    i32 puncta_addita;    /* puncta probationis per primum (I) */
    i32 conatus_maximi;   /* indices per primum ante FALSUM (2^20) */
} InterpolatioQuaestio;

InterpolatioQuaestio
interpolatio_quaestio_ordinaria (
    vacuum);

/* f in Z[t, t^-1] ex aestimationibus. Per primum: indices 0, 1, ...
 * dum K + puncta_addita puncta utilia, K = (summus - imus)/passus + 1,
 * cum y = punctum^passus DISTINCTIS (y iteratum omittitur, sed valorem
 * eundem dare debet); Lagrange in primis K, puncta addita congruere
 * debent. Primi descendentes infra primus_limes, filtro residui (per
 * gradus moduli), et soli qui satis y distincta dant ((p - 1) /
 * mdc(passus, p - 1) >= K + puncta_addita), donec summa floor(log2
 * p) >= limes_bitorum + 1 (productum > 2 max |c|); deinde UNUS
 * ADDITUS cuius residua restitutionem confirmare debent, et |c| <
 * 2^limes_bitorum. Coefficientes per congruentia_restitue
 * symmetricam; memoria effectus ex coefficientibus non nullis, non ex
 * fenestra.
 *
 * FALSUM (exitus non tangitur), SINE ULLA aestimatione, si quaestio
 * invalida: aestimare NIHIL, passus nullus, conatus_maximi nullus,
 * summus < imus, |imus| aut |summus| > POLYNOMIUM_EXPONENS_MAXIMUS,
 * fenestra non divisibilis per passum, K + puncta_addita > 2^31 - 1,
 * limes_bitorum > INTERPOLATIO_LIMES_MAXIMUS, filtrum non admissibile.
 * FALSUM etiam si aestimator ERRATUM reddit, conatus exhausti, primi
 * desunt, allocatio deficit, aut probatio discordat (y iteratum,
 * punctum additum, primus additus, limes). */
b32
interpolatio_polynomium (
           InterpolatioAestimator  aestimare,
                           vacuum* contextus,
    constans InterpolatioQuaestio* quaestio,
                          Piscina* piscina,
                       Polynomium* exitus);

#endif /* INTERPOLATIO_H */
