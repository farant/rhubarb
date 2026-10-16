/* nervus.h - Retia EXACTA per partes linearia (ReLU), ut data
 *
 * RETE: programma rectum (sine saltibus) super registra. Registra
 * 0 .. introitus-1 sunt introitus; instructio k registrum
 * introitus + k scribit (assignatio unica). Duo genera instructionum:
 *   NERVUS_AFFINIS  r = c_1 f_1 + ... + c_t f_t + b
 *   NERVUS_RELU     r = max(0, c_1 f_1 + ... + c_t f_t + b)
 * f_j registra iam definita (indice minore quam r), c_j et b Magnus
 * (integri exacti, sine limite). min(0, x) = x - relu(x). Exitus:
 * registra nominata (eadem iterari licet; introitus quoque).
 *
 * FORMA ACTIVATIONIS: bitus unus per ReLU, ordine instructionum.
 * ReLU ACTIVA est si et solum si argumentum > 0 (ad 0 ambae partes 0
 * dant, ergo valor non pendet a conventione; forma pendet, et haec
 * est). Bitus k in bitus[k >> 3], positione (k & 7).
 *
 * NULLAE: forma post nervus_aestimare numerum ReLU fert quarum
 * argumentum EXACTE 0 erat. Tale punctum in pariete regionis iacet:
 * forma eius est cellae dimensionis inferioris, non regionis apertae
 * (|x| in x = 0: forma propria, sed |x| duas regiones habet). Retia
 * sine addendis parietes per originem habent, ergo puncta integra in
 * parietibus frequentia sunt: regiones numerans puncta cum nullae > 0
 * separet. Claves et aequalitas formarum nullas ignorant.
 *
 * FIGURA: profunditas registri = 1 + maxima profunditas fontium (0
 * si fontes nulli; introitus profunditatis 0), profunditas retis =
 * maxima; profunditas_relu idem ReLU numerans; latitudo = registra
 * viva plurima, ante primam instructionem et post quamque (vivum:
 * definitum et postea lectum aut exitus).
 *
 * STRUCTURA PUBLICA: rete manu factum per nervus_validus probatur;
 * omnis functio rete invalidum refutat (FALSUM), numquam extra alveum
 * legit. Construere solum per nervus_affinis / nervus_relu /
 * nervus_exitus (quae ordinem et indices custodiunt).
 *
 * FORMA COACTA: nervus_aestimare_coacte ReLU k bitum k formae
 * sequi cogit (activa: argumentum ipsum, etiam negativum; non activa:
 * 0), quacumque signo. Forma fixa rete AFFINE facit: x -> M x + c
 * (nervus_forma_affinis, exacte, per formas affines propagatas). In
 * regione formae (ubi aestimatio ordinaria eandem formam dat) mappa
 * aestimationem ordinariam aequat; extra eam aestimationem coactam.
 * Sine addendis c = 0.
 *
 * COMPOSITIO: nervus_componere(a, b) = b post a: exitus a introitus b
 * fiunt; ReLU ordine a deinde b, ergo forma compositi = forma a
 * deinde forma b.
 *
 * REGIONES PLANI: rete sine addendis cum II introitibus positive
 * homogeneum est, ergo regiones eius CONI ab origine sunt: sectores
 * inter radios rationales (muros). Puncta formae eiusdem regionem
 * CONVEXAM faciunt (sub forma priorum argumentum cuiusque ReLU
 * lineare est, ergo condicio eius semiplanum), ergo forma una sectorem
 * unum dat, angulus <= pi, formae sectorum diversae. Murus: radius ubi
 * forma mutatur. Sectores contra horologium; primus incipit a muro
 * primo ab (1, 0) inclusive. Nulli muri: sector unus, totum planum,
 * initium = finis = (1, 0). NULLAE sectoris: ReLU quarum argumentum in
 * sectore IDENTICE 0 est - bitus 0, et nervus_aestimare in omni puncto
 * interiore easdem nullas reddit (puncta interiora numerans talem
 * sectorem non videt). FRUSTUM (pars linearis): sectores contigui
 * eadem matrice coniuncti; frusta <= sectores. Algorithmus: circulus
 * in IV arcus per axes divisus; per instructionem forma linearis
 * cuiusque registri in quoque arcu; ad ReLU arcus ubi argumentum g
 * signum STRICTE mutat in radio |g(v)| u + |g(u)| v (exacte,
 * primitivo) dividitur; arcus semper < pi; postremo arcus contigui
 * forma eadem coniunguntur. Sumptus O(arcus x registra) Magnus.
 *
 * MEMORIA: rete in piscina sua (data in nervus_creare) crescit;
 * coefficientes in eam transcribuntur. nervus_aestimare valores et
 * spatium operis in piscina vocantis allocat: in gyris
 * piscina_notare / piscina_reficere circum vocationem, exitus per
 * magnus_transcribe servandi. Exitus membra cum introitu (exitus qui
 * introitus est) aut cum addendis retis (instructio constans) PARTIRI
 * potest, ut in magnus. FALSUM -> exitus non tangitur.
 *
 * CAUTIO TYPORUM: numeri i32 (SINE signo), indices registrorum s32.
 *
 * USUS (AND(x, y) = relu(x + y - 1)):
 *   Nervus* n;
 *   s32     f[II] = { 0, 1 };
 *   Magnus  c[II];
 *   s32     r;
 *   c[0] = c[1] = magnus_ex_s64(I);
 *   (vacuum)nervus_creare(II, piscina, &n);
 *   (vacuum)nervus_relu(n, II, f, c, magnus_ex_s64(-I), &r);
 *   (vacuum)nervus_exitus(n, I, &r);
 *
 * Consilium: project-specs/nervus-plan.md. Vide lib/nervus.worklog.md.
 */
/* <aedilis corpus="lib/nervus.c"/> */
#ifndef NERVUS_H
#define NERVUS_H

#include "latina.h"
#include "piscina.h"
#include "magnus.h"
#include "chorda.h"
#include "matrix.h"

/* introitus + instructiones; termini per instructionem; exitus */
#define NERVUS_REGISTRA_MAXIMA  16777216
#define NERVUS_TERMINI_MAXIMI   65536

nomen enumeratio {
    NERVUS_AFFINIS = 0,
    NERVUS_RELU    = 1
} NervusGenus;

nomen structura {
    NervusGenus  genus;
            i32  termini;
            s32* fontes;           /* termini registra, < registrum */
         Magnus* coefficientes;    /* termini */
         Magnus  addendum;         /* b */
} NervusInstructio;

nomen structura {
                 i32  introitus;
                 i32  numerus;          /* instructiones */
                 i32  capacitas;
    NervusInstructio* instructiones;
                 i32  relu;             /* ReLU inter instructiones */
                 i32  exitus_numerus;
                 s32* exitus;           /* registra exitus */
             Piscina* piscina;
} Nervus;

nomen structura {
     i32  numerus;     /* ReLU */
      i8* bitus;       /* (numerus + 7) / 8 octeti */
     i32  nullae;      /* ReLU argumento exacte 0 (vide NULLAE) */
} NervusForma;

nomen structura {
     i32 instructiones;
     i32 relu;
     i64 termini;              /* summa terminorum */
     i32 profunditas;
     i32 profunditas_relu;
     i32 latitudo;
} NervusFigura;


/* ==================================================
 * Constructio
 * ================================================== */

/* rete vacuum cum introitus registris (0 licet: rete constans);
 * FALSUM si introitus > NERVUS_REGISTRA_MAXIMA */
b32
nervus_creare (
         i32   introitus,
     Piscina*  piscina,
      Nervus** exitus);

/* instructio affinis; *registrum_novum = index registri novi. FALSUM
 * (rete non mutatum) si caput retis invalidum (numeri, alvei,
 * piscina: O(1), non instructiones singulae - eas nervus_validus
 * probat), termini > NERVUS_TERMINI_MAXIMI, fontes
 * aut coefficientes NIHIL cum termini > 0, fons extra 0 .. registra
 * iam definita, aut registra plena */
b32
nervus_affinis (
               Nervus* rete,
                  i32  termini,
         constans s32* fontes,
      constans Magnus* coefficientes,
               Magnus  addendum,
                  s32* registrum_novum);

/* idem, ReLU */
b32
nervus_relu (
               Nervus* rete,
                  i32  termini,
         constans s32* fontes,
      constans Magnus* coefficientes,
               Magnus  addendum,
                  s32* registrum_novum);

/* exitus ponit (priores reponit); FALSUM (rete non mutatum) si caput
 * retis invalidum, numerus > NERVUS_REGISTRA_MAXIMA, registrum non
 * definitum, aut registra NIHIL cum numerus > 0 */
b32
nervus_exitus (
            Nervus* rete,
               i32  numerus,
      constans s32* registra);

/* VERUM si rete validum: indices omnes intra fines, fontes ante
 * registrum suum, genera nota, relu numeratum recte, alvei non NIHIL
 * ubi opus. O(termini) */
b32
nervus_validus (
      constans Nervus* rete);


/* ==================================================
 * Aestimatio
 * ================================================== */

/* forma vacua (omnes bitus 0) pro rete; FALSUM si rete invalidum */
b32
nervus_forma_creare (
      constans Nervus* rete,
              Piscina* piscina,
          NervusForma* exitus);

/* VERUM si ReLU k activa; FALSUM si non, aut k extra fines */
b32
nervus_forma_bitus (
      constans NervusForma* forma,
                       i32  k);

/* introitus[0 .. introitus-1] -> exitus[0 .. exitus_numerus-1],
 * exacte; forma (NIHIL licet) bitus et nullas accipit. FALSUM si rete
 * invalidum, alveus necessarius NIHIL, aut forma non huius retis
 * (numerus differt) */
b32
nervus_aestimare (
      constans Nervus* rete,
      constans Magnus* introitus,
               Magnus* exitus,
          NervusForma* forma,
              Piscina* piscina);

/* aestimatio coacta (vide FORMA COACTA); FALSUM ut nervus_aestimare,
 * aut forma NIHIL */
b32
nervus_aestimare_coacte (
         constans Nervus* rete,
         constans Magnus* introitus,
                  Magnus* exitus,
    constans NervusForma* forma,
                 Piscina* piscina);

/* figura retis (vide FIGURA supra); FALSUM si rete invalidum. Memoria
 * temporaria propria, ante reditum destructa */
b32
nervus_figura (
      constans Nervus* rete,
         NervusFigura* exitus);


/* ==================================================
 * Formae
 * ================================================== */

/* bitum k ponit; FALSUM si forma invalida aut k extra fines */
b32
nervus_forma_ponere (
    NervusForma* forma,
            i32  k,
            b32  activa);

/* numerus ReLU activarum; 0 si forma invalida */
i32
nervus_forma_activae (
    constans NervusForma* forma);

/* VERUM si numerus idem et bitus omnes idem; FALSUM si non, aut forma
 * invalida */
b32
nervus_formae_aequales (
    constans NervusForma* a,
    constans NervusForma* b);

/* clavis pro tabula_dispersa: octeti formae (bitus ultra numerum
 * nulli), (numerus + 7) / 8 octeti; formae aequales <-> claves
 * aequales (pro numero eodem). FALSUM si forma invalida */
b32
nervus_forma_clavis (
    constans NervusForma* forma,
                 Piscina* piscina,
                  chorda* exitus);

/* mappa affinis formae: M (exitus_numerus x introitus, super
 * ANULUS_INTEGRORUM, elementa Magnus) et c (exitus_numerus Magnus;
 * NIHIL si exitus nulli) in piscina vocantis. Formae affines in
 * piscina temporaria propagantur: memoria O(registra x introitus).
 * FALSUM si rete invalidum, forma NIHIL aut non huius retis, aut
 * registra x (introitus + 1) > 2^27 */
b32
nervus_forma_affinis (
         constans Nervus*  rete,
    constans NervusForma*  forma,
                 Piscina*  piscina,
                  Matrix*  m,
                  Magnus** c);


/* ==================================================
 * Compositio
 * ================================================== */

/* b post a (vide COMPOSITIO); rete novum in piscina. FALSUM si rete
 * invalidum, a->exitus_numerus != b->introitus, aut registra
 * NERVUS_REGISTRA_MAXIMA superant */
b32
nervus_componere (
    constans Nervus*  a,
    constans Nervus*  b,
            Piscina*  piscina,
             Nervus** exitus);


/* ==================================================
 * Regiones plani (sine addendis, II introitus)
 * ================================================== */

/* directio ab origine: (x, y) integri primitivi (divisor communis 1),
 * non ambo nulli */
nomen structura {
    Magnus x;
    Magnus y;
} NervusRadius;

/* regio aperta inter murum initium et murum finis, contra horologium
 * (vide REGIONES PLANI) */
nomen structura {
    NervusRadius initium;
    NervusRadius finis;
    NervusRadius medius;      /* directio intra sectorem */
     NervusForma forma;       /* nullae: argumento identice 0 */
          Matrix m;           /* exitus_numerus x II: f(x) = M x */
             i32 frustum;     /* index partis linearis, 0 .. frusta-1 */
} NervusSector;

nomen structura {
             i32  numerus;     /* sectores = regiones */
    NervusSector* regiones;
             i32  frusta;      /* partes lineares */
} NervusPlanum;

/* regiones activationis retis in plano, exacte (vide REGIONES PLANI),
 * in piscina vocantis. FALSUM (exitus non tangitur) si rete invalidum,
 * introitus != II, addendum aliquod non nullum, aut memoria deficit */
b32
nervus_regiones_plani (
    constans Nervus* rete,
            Piscina* piscina,
       NervusPlanum* exitus);

#endif /* NERVUS_H */
