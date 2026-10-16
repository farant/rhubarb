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
 * FIGURA: profunditas = catena longissima instructionum ab introitu
 * (introitus profunditatis 0); profunditas_relu = plurimae ReLU in
 * catena; latitudo = registra viva plurima post instructionem
 * quamque (vivum: definitum et postea lectum aut exitus).
 *
 * STRUCTURA PUBLICA: rete manu factum per nervus_validus probatur;
 * omnis functio rete invalidum refutat (FALSUM), numquam extra alveum
 * legit. Construere solum per nervus_affinis / nervus_relu /
 * nervus_exitus (quae ordinem et indices custodiunt).
 *
 * MEMORIA: rete in piscina sua (data in nervus_creare) crescit;
 * coefficientes in eam transcribuntur. nervus_aestimare valores et
 * spatium operis in piscina vocantis allocat: in gyris
 * piscina_notare / piscina_reficere circum vocationem, exitus per
 * magnus_transcribe servandi. FALSUM -> exitus non tangitur.
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
 * exacte; forma (NIHIL licet) bitus accipit. FALSUM si rete
 * invalidum, alveus necessarius NIHIL, aut forma non huius retis
 * (numerus differt) */
b32
nervus_aestimare (
      constans Nervus* rete,
      constans Magnus* introitus,
               Magnus* exitus,
          NervusForma* forma,
              Piscina* piscina);

/* figura retis (vide FIGURA supra); FALSUM si rete invalidum. Memoria
 * temporaria propria, ante reditum destructa */
b32
nervus_figura (
      constans Nervus* rete,
         NervusFigura* exitus);

#endif /* NERVUS_H */
