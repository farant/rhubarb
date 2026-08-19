/* differentia.h - differentia serierum et linearum (diff)
 *
 * NUCLEUS SUPER IDENTITATEM: differentia_seriei ordines
 * monstratorum comparat - aequalitas = identitas monstratoris,
 * numquam contentum. Vocator elementa INTERNAT (internamentum),
 * ergo nucleus unus lineis, signis silvae, verbis, nodis STML
 * aequaliter servit. differentia_linearum = involucrum quod pro
 * textu findit + internat + differt (internamentum PRIVATUM, in
 * piscina vocatoris - tabula globalis intacta).
 *
 * FRUCTUS = TRACTUS (cursus, non elementa singula):
 *   IDEM   - in ambobus
 *   DELETA - in A, non in B
 *   ADDITA - in B, non in A
 * INVARIANS (probandum): tractus IDEM+DELETA ordinem A totum
 * partiuntur, IDEM+ADDITA ordinem B totum - nihil celatur, nihil
 * bis numeratur. FORMA CANONICA: in massa mutationis DELETA ante
 * ADDITA semper (mos GNU) et tractus vicini numquam eiusdem
 * generis (fusi).
 *
 * ALGORITHMUS: Myers O(ND) - scriptum editionis MINIMUM. Consilium
 * Eskilianum: API congelata, corpus mutabile - patience/histogram
 * (saepe legibilior in codice) postea sub eadem API poni possunt.
 * Tectum pragmaticum: differentia media maior MMXLVIII gradibus
 * scriptum TRIVIALE accipit (DELETA omnia + ADDITA omnia) -
 * invarians servatur, minimalitas non (memoria vestigii quadratice
 * crescit et differentia tanta homini illegibilis est).
 *
 * EMISSIO SEIUNCTA: differentia_unificata (forma ---/+++/@@)
 * praesentatio est, non veritas - nucleus purus manet.
 */

#ifndef DIFFERENTIA_H
#define DIFFERENTIA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"


/* ==================================================
 * Nucleus: differentia seriei (identitas monstratoris)
 * ================================================== */

nomen enumeratio {
    DIFFERENTIA_IDEM = 0,
    DIFFERENTIA_DELETA,   /* in A, non in B */
    DIFFERENTIA_ADDITA    /* in B, non in A */
} DifferentiaGenus;

/* index_a/index_b = initium tractus in utroque ordine. AMBO semper
 * valida: pro DELETA index_b = positio insertionis in B (ubi A
 * desineret), pro ADDITA index_a similiter - emissori contextus
 * sine arithmetica. */
nomen structura {
    DifferentiaGenus genus;
                 i32 index_a;
                 i32 index_b;
                 i32 numerus;   /* quot elementa in tractu */
} DifferentiaTractus;

/* Xar de DifferentiaTractus, ordine; NIHIL = memoria defecit.
 * Ordines vacui liciti (a vacuum + b plenum = ADDITA unus, etc.;
 * ambo vacui = tractus nulli). */
Xar*
differentia_seriei (
    Piscina*                   piscina,
    constans vacuum* constans* a,
    i32                        numerus_a,
    constans vacuum* constans* b,
    i32                        numerus_b);


/* ==================================================
 * Involucrum linearum
 *
 * Fissio: '\n' terminat lineam; textus sine '\n' finali lineam
 * ultimam tamen fert (vexillum infra id notat); textus VACUUS =
 * lineae NULLAE (non linea vacua una); "\n" solum = linea vacua
 * una.
 * ================================================== */

nomen structura {
    Xar* tractus;    /* de DifferentiaTractus */
    Xar* lineae_a;   /* de chorda - lineae A (sine '\n') */
    Xar* lineae_b;
    b32  a_sine_fine;   /* A sine '\n' finali */
    b32  b_sine_fine;
} DifferentiaLinearum;

/* NIHIL = memoria defecit */
DifferentiaLinearum*
differentia_linearum (
    Piscina* piscina,
     chorda  a,
     chorda  b);


/* ==================================================
 * Emissio et summa
 * ================================================== */

/* Forma unificata classica:
 *   --- titulus_a
 *   +++ titulus_b
 *   @@ -l,n +l,n @@
 *   ' '/'-'/'+' lineae; '\ sine linea nova in fine' ubi opus.
 * contextus = lineae IDEM circum mutationes (mos: III); massae
 * quarum contextus se tangit COALESCUNT. Chorda vacua = nulla
 * differentia. */
chorda
differentia_unificata (
                         Piscina* piscina,
    constans DifferentiaLinearum* d,
              constans character* titulus_a,
              constans character* titulus_b,
                             i32  contextus);

/* pro conspectu '+N -M' (silex differentia, nomina primum) */
nomen structura {
    i32 additae;
    i32 deletae;
} DifferentiaSumma;

DifferentiaSumma
differentia_summa (
    constans Xar* tractus);

#endif /* DIFFERENTIA_H */
