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
 * VERIFICATIO (2026-11-02, principium 01M6FMVW). Auctoritas secunda,
 * electione sola: VERIFICATUM sistit ubi candidatus probationes in
 * punctis fortuitis modulo primis fortuitis transit, cum limite erroris
 * DICTO (2^-s) - numquam quia responsum mutari desiit. Effectus semper
 * genus suum, semen, verificatorem in relatione fert; PROBATUM manet
 * ordinarium. Vide sectionem Verificatio infra.
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

/* fasciculus (puncta-parallela-plan): indices index_primus .. +
 * numerus - 1 primi p simul. Pro omni j < numerus status[j] scribit
 * (UTILE, INUTILE aut ERRATUM, sensu InterpolatioAestimator), et ubi
 * UTILE puncta[j], valores[j]; bibliotheca ante vocationem puncta et
 * valores nullos, status ERRATUM ponit. FALSUM = defectus totius
 * (transitus, opifex mortuus): nihil numeratur, FALSUM redditur.
 *
 * PURITAS (contractus): effectus in (p, index) a nullo alio pendet -
 * non a fasciculo, non a vicinis, non a numero vocationum, non ab ordine
 * interno. Bibliotheca effectus ordine indicis consumit, ergo fasciculus
 * purus eadem omnia reddit ac aestimator singulus idem in eisdem
 * indicibus (puncta, relatio, tractus, numeri). */
nomen b32 (*InterpolatioAestimatorFasciculi) (
    vacuum* contextus,
       i32  index_primus,
       i32  numerus,
       i32  p,
       i32* puncta,
       i32* valores,
       s32* status);

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
    /* verificatio (interpolatio_polynomium_relatum solum; vide infra) */
    i32 modus;            /* INTERPOLATIO_PROBATUM (ordinarium) aut
                           * INTERPOLATIO_VERIFICATUM */
    i32 bita_securitatis; /* s: limes erroris 2^-s (LXIV); 1 .. CCLVI */
    i64 semen;            /* semen tractuum (0): a vocante, numquam ex
                           * entropia bibliothecae */
    i32 semen_origo;      /* INTERPOLATIO_SEMEN_DATUM (ordinarium) aut
                           * _FORTUITUM - solum relatum, nihil mutat */
    i32 verificationis_imus;   /* primi probationum in [imus, summus)
                                * (2^30) */
    i32 verificationis_summus; /* (2^31). Alia ambitus solum
                                * probationibus: latitudo <= 2^24,
                                * primi numerantur */
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


/* ==================================================
 * Verificatio: terminatio praecox cum probationibus fortuitis
 * (project-specs/interpolatio-verificatio-plan.md; principium
 * tabularii 01M6FMVW)
 *
 * Auctoritas effectus duplex. PROBATUM: primi per limitem probatum et
 * additus (ut interpolatio_polynomium). VERIFICATUM: post quemque primum
 * candidatus Q_k restituitur; (1) coefficiens >= 2^limes -> primus
 * proximus sine probatione; (2) probatio SUI: aestimator principalis in
 * puncto fortuito modulo primo fortuito; (3) si transit, m probationes
 * CERTIFICANTES per verificatorem; omnes transeunt -> VERIFICATUM. Error
 * acceptationis <= (k_max - 1) eps^m <= 2^-s, eps = c (K - 1) / (q_min -
 * r) + floor((limes + 1) / floor(log2 q_min)) / N (vide
 * interpolatio_probationes_necessariae). Ad k_max: primus additus ut
 * hodie, genus PROBATUM (status AD_LIMITEM). Verificator qui candidatum
 * semel reiecit, si idem candidatus deinde acceptandus est, aut eundem
 * bis reicit, aut effectum probatum ad limitem reicit -> FALSUM, status
 * DISCORDIA: bibliotheca nescit uter erret.
 *
 * TRACTUS (documentati, reproducibiles ex semine): sors_seminare(semen,
 * 0), id est PCG32 (pcg32_srandom_r) cum initstate = effusio prima
 * SplitMix64 ex semine, initseq = 0; per probationem: primus q = imus +
 * sors_intra(summus - imus) (valor quilibet) iteratum donec filtro
 * residui quaestionis congruit, primus est, et inter primos restitutionis
 * iam usos (0 .. k - 1) non est - primi probationum iterari possunt
 * (tractus cum repositione); deinde index = sors_intra(q) iteratum dum
 * INUTILE aut punctum 0 (usque ad LXIV). Ordo: probationes ordine quo
 * fiunt (sui, deinde certificantes), primus ante indicem.
 * ================================================== */

#define INTERPOLATIO_PROBATUM     ZEPHYRUM
#define INTERPOLATIO_VERIFICATUM  I

#define INTERPOLATIO_SEMEN_DATUM      ZEPHYRUM  /* a vocante electum:
                                                 * limes nullus in actis */
#define INTERPOLATIO_SEMEN_FORTUITUM  I         /* ex entropia vocantis */

/* status relationis */
#define INTERPOLATIO_STATUS_NULLUS  ZEPHYRUM  /* FALSUM alia causa */
#define INTERPOLATIO_ACCEPTUM       I         /* VERIFICATUM */
#define INTERPOLATIO_AD_LIMITEM     II        /* PROBATUM (k_max + additus) */
#define INTERPOLATIO_DISCORDIA      III       /* verificator discordat:
                                               * FALSUM */
#define INTERPOLATIO_PROMOTUM       IV        /* PROBATUM per promotionem */

/* generator tractuum (probatio_sors vectores aureos tenet) */
#define INTERPOLATIO_GENERATOR  "sors PCG32-XSH-RR/SplitMix64"

/* verificator: aestimator (contractus idem ac InterpolatioAestimator,
 * indices in [0, q)) et quae limes erroris ab eo poscit */
nomen structura {
    InterpolatioAestimator  aestimare;
                    vacuum* contextus;
                       i32  multiplicitas;    /* c: indices in [0, q) per
                                               * valorem y = x^passus,
                                               * maximus (>= 1) */
                       i32 inutilia_maxima;  /* r: indices INUTILE (aut
                                               * punctum 0) per primum,
                                               * maximus */
       constans character* titulus;          /* in relatione */
       constans character* communia;         /* codex cum aestimatore
                                               * principali communis */
} InterpolatioVerificator;

/* tractus unus (probatio una) */
nomen structura {
    i32 gradus;        /* primi restitutionis tunc (stadium k) */
    i32 primus;        /* q */
    i32 index;         /* index utilis (INUTILIA iterata non notantur) */
    b32 certificans;   /* FALSUM = probatio sui */
    b32 transiit;
} InterpolatioTractus;

/* relatio: omnia ex piscina vocationis */
nomen structura {
    InterpolatioQuaestio quaestio;          /* copia */
                     i32 genus;             /* auctoritas: PROBATUM aut
                                              * VERIFICATUM */
                    i32  status;
                    i32* primi;             /* restitutionis, sine
                                              * addito */
                    i32 numerus_primorum;
                    i32 primus_additus;    /* 0 si nullus (ACCEPTUM) */
                    i32 k_maximus;         /* per limitem probatum */
                    i64 aestimationes;     /* indices aestimatoris
                                              * principalis CONSUMPTI in
                                              * primis (= vocationes si
                                              * singulus; probationes
                                              * non) */
                    i32 probationes_sui;
                    i32 probationes_certificantes;
                    i32 probationes_requisitae;    /* m */
                    i32 bita_erroris;      /* limes erroris 2^-bita
                                              * (ACCEPTUM; aliter 0) */
    InterpolatioTractus* tractus;
                    i32  numerus_tractuum;
     constans character* titulus_verificatoris;
     constans character* communia;
     constans character* generator;
} InterpolatioRelatio;

/* m minimus cum (k_maximus - 1) eps^m <= 2^-s, EXACTE (Magnus: eps = a /
 * b, (k_maximus - 1) a^m 2^s <= b^m), et *bita_erroris = maximum beta cum
 * (k_maximus - 1) a^m 2^beta <= b^m. K, limes, s, ambitus ex quaestione;
 * c, r ex verificatore; N = 2^24 pro ambitu ordinario (primi 1 mod 4 in
 * [2^30, 2^31) sunt 25348870, omnes 50697537; filtrum aliud quam nullum
 * aut modulo IV refutatur), aliter primi ambitus numerati minus
 * (k_maximus + 1). k_maximus <= 1: m = 0 (nullum stadium ante limitem).
 * FALSUM si s extra 1 .. CCLVI, ambitus aut verificator invalidus, eps
 * >= 1, aut m > CCLVI. */
b32
interpolatio_probationes_necessariae (
       constans InterpolatioQuaestio* quaestio,
    constans InterpolatioVerificator* verificator,
                                 i32  k_maximus,
                             Piscina* piscina,
                                 i32* probationes,
                                 i32* bita_erroris);

/* k_max: primi restitutionis per limitem probatum (sine addito), ut
 * interpolatio_polynomium eos enumerat; nihil aestimat. FALSUM si
 * quaestio invalida aut primi desunt. */
b32
interpolatio_primi_probati (
    constans InterpolatioQuaestio* quaestio,
                          Piscina* piscina,
                              i32* k_maximus);

/* ut interpolatio_polynomium, cum relatione. quaestio->modus
 * PROBATUM: verificator neglegitur (NIHIL licet), effectus idem ac
 * interpolatio_polynomium. VERIFICATUM: verificator requiritur (ad
 * certificandum per se ipsum, vocans aestimatorem principalem in eo
 * ponit, cum c et r suis). relatio
 * NIHIL licet. FALSUM sine aestimatione si quaestio aut verificator
 * invalidus; FALSUM (exitus non tangitur) si aestimator aut verificator
 * ERRATUM reddit, conatus exhausti, aut DISCORDIA. */
b32
interpolatio_polynomium_relatum (
              InterpolatioAestimator  aestimare,
                              vacuum* contextus,
    constans InterpolatioVerificator* verificator,
       constans InterpolatioQuaestio* quaestio,
                             Piscina* piscina,
                          Polynomium* exitus,
                 InterpolatioRelatio* relatio);

/* ut interpolatio_polynomium_relatum, aestimatore principali per
 * fasciculos (verificator manet singulus). capacitas = indices per
 * petitionem maximi (0 = sine limite). Petitio: min(puncta quae desunt,
 * capacitas, conatus_maximi - index); index quisque punctum unum ad
 * summum addit, ergo nullus index petitur quem cursus singulus non
 * attingeret. relatio->aestimationes = indices CONSUMPTI: in successu
 * idem ac singulus; in refutatione intra fasciculum indices post
 * refutantem aestimati sunt sed non numerantur (relatio eadem). Probatio
 * sui = fasciculus unius indicis per tractum (series sortis eadem).
 * Fasciculus purus (vide supra) -> exitus et relatio eadem ac
 * interpolatio_polynomium_relatum cum aestimatore singulo eodem. */
b32
interpolatio_polynomium_relatum_fasciculis (
     InterpolatioAestimatorFasciculi  fasciculus,
                              vacuum* contextus,
                                 i32  capacitas,
    constans InterpolatioVerificator* verificator,
       constans InterpolatioQuaestio* quaestio,
                             Piscina* piscina,
                          Polynomium* exitus,
                 InterpolatioRelatio* relatio);

/* VERIFICATUM -> PROBATUM: primi k (relatio->primi) cum serie quaestionis
 * regenerata congruere debent; residua verificati in eis ex ipso
 * computantur, primi k+1 .. k_max et additus aestimantur, restitutio
 * tota verificatum EXACTE reddere debet. relatio: genus VERIFICATUM,
 * status ACCEPTUM (aliter FALSUM). FALSUM (exitus non tangitur) si series
 * discordat, verificatum extra fenestram, aestimatio deficit, aut
 * restitutio aliud reddit (verificatum falsum: refutatio principii,
 * referenda). relatio_nova: genus PROBATUM, status PROMOTUM; non eadem ac
 * relatio.
 *
 * AUCTORITAS HEREDITARIA: residua primorum 1 .. k ex verificato sumuntur,
 * NON recomputantur. PROMOTUM igitur probat effectum relative ad residua
 * cursus verificati: verificatum quod ille cursus reddidit. Mendum
 * fortuitum (linea truncata, par UNCINUS/RELATIO falsum) deprehenditur;
 * verificatum consulto fabricatum (Q + multiplum producti primorum k+1 ..
 * k_max et additi, intra limitem) NON deprehenditur - recensio T4 M1.
 * Auctoritas plena sine hereditate = PROBATUM directe. */
b32
interpolatio_promovere (
           InterpolatioAestimator  aestimare,
                           vacuum* contextus,
    constans InterpolatioQuaestio* quaestio,
     constans InterpolatioRelatio* relatio,
              constans Polynomium* verificatum,
                          Piscina* piscina,
                       Polynomium* exitus,
              InterpolatioRelatio* relatio_nova);

#endif /* INTERPOLATIO_H */
