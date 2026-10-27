/* contractio.h - Retia tensorum et contractio eorum EXACTA
 *
 * Rete = indices (dimensio d >= 1) et tensores super indices
 * distinctos; quisque index in duobus tensoribus exacte (rete
 * clausum) summatur. Contractio valorem retis reddit: summa super
 * omnes assignationes indicum producti elementorum.
 *
 * STRUCTURA SEPARATA A VALORIBUS. Rete et ordo eius semel
 * aedificantur; valores per exsecutionem dantur. Unum rete, unus
 * ordo, una officina omnibus punctis et primis serviunt.
 *
 * ORDO. contractio_ordinare ordinem binarium deterministicum eligit
 * (v1: avidus, clavis exacta infra) et omnem tensorem intermedium in
 * una tabula (slab) disponit - offsets fixi per vitam, primum aptum.
 * Magnitudo tabulae in octetis ANTE allocationem nota est (sumptus);
 * officina semel allocatur et supra limitem vocantis refutatur, non
 * abortitur. Exsecutio calida deinde nihil allocat.
 *
 * CLAVIS AVIDA. Inter paria tensorum vivorum quae indicem saltem unum
 * communem habent, minimum (elementa effectus, saturata; deinde id
 * minor; deinde id maior). Effectus id proximum accipit (tensores
 * initiales 0 .. n - 1, intermedii n, n + 1, ...). Si nullum par
 * indicem communem habet (componentes disiunctae, scalares), eadem
 * clavis super OMNIA paria (productum externum). Clavis ab id
 * indicum non pendet: renumeratio indicum ordinem et sumptus non
 * mutat; ordo insertionis tensorum sumptus mutare potest.
 *
 * TENSOR INTERMEDIUS super indices non contractos, ordine id
 * ascendente; index ULTIMUS celerrimus (ut valores initiales).
 *
 * DUO GRADUS, UNUM RESPONSUM. Gradus calidus: residua i32 modulo n
 * (2 <= n < 2^32; accumulator duorum verborum, nulla exundatio).
 * Gradus referentiae: quilibet anulus domus (anulus.h). Probationes
 * gradum calidum accipiunt solum ubi responso referentiae modulo n
 * aequalis est.
 *
 * USUS:
 *   ContractioRete*     r = contractio_rete_creare(piscina);
 *   ContractioOptiones  o = contractio_optiones_ordinariae();
 *   ContractioOrdo*     ordo;
 *   ContractioOfficina* officina;
 *   i32 a, b, t0, t1;
 *   contractio_index_addere(r, II, &a);
 *   ... tensores ...
 *   ordo     = contractio_ordinare(r, &o, piscina);
 *   officina = contractio_officina_creare(ordo, limes, piscina);
 *   si (!contractio_exsequi_modulo(r, ordo, officina, valores, p,
 *       &valor)) refutare;
 *
 * Vide lib/contractio.worklog.md et project-specs/contractio-plan.md.
 */
/* <aedilis corpus="lib/contractio.c"/> */
#ifndef CONTRACTIO_H
#define CONTRACTIO_H

#include "latina.h"
#include "piscina.h"
#include "anulus.h"

nomen structura ContractioRete     ContractioRete;     /* opacum */
nomen structura ContractioOrdo     ContractioOrdo;     /* opacum */
nomen structura ContractioOfficina ContractioOfficina; /* opacum */


/* ==================================================
 * Rete
 * ================================================== */

ContractioRete*
contractio_rete_creare (
    Piscina* piscina);

/* index dimensionis d >= 1; id = 0, 1, 2, ... ordine additionis.
 * FALSUM si d == 0 aut rete iam ordinatum (id non tangitur). */
b32
contractio_index_addere (
    ContractioRete* rete,
               i32  dimensio,
               i32* id);

/* tensor super indices distinctos (ordo valorum: index ULTIMUS
 * celerrimus; numerus 0 = scalaris, indices tunc NIHIL licet).
 * FALSUM si index ignotus, index bis in eodem tensore (v1: vocans
 * vestigium ipse plicat), index iam duobus tensoribus usus, aut rete
 * iam ordinatum - rete tunc non mutatur, id non tangitur. */
b32
contractio_tensor_addere (
        ContractioRete* rete,
          constans i32* indices,
                   i32  numerus,
                   i32* id);

/* VERUM sse quisque index in duobus tensoribus exacte (rete vacuum
 * clausum est) */
b32
contractio_rete_clausum (
    constans ContractioRete* rete);


/* ==================================================
 * Ordo
 * ================================================== */

#define CONTRACTIO_AVIDUS I

nomen structura {
    i32 methodus;             /* CONTRACTIO_AVIDUS (v1 solus) */
} ContractioOptiones;

/* vocans SEMPER ab hac incipit: membra postea addita valores
 * ordinarios habent */
ContractioOptiones
contractio_optiones_ordinariae (vacuum);

/* sumptus: omnia i64 SATURANTUR ad CONTRACTIO_SATURATUM (2^64 - 1);
 * latitudo ex magnitudinibus EXACTIS, non ex apice saturato */
#define CONTRACTIO_SATURATUM ((i64)~(i64)ZEPHYRUM)

nomen structura {
    i64 apex;                 /* elementa tensoris intermedii maximi
                               * (0 si nullus intermedius) */
    i64 octeti_officinae;     /* tabula residuorum i32, octeti */
    i64 multiplicationes;     /* summa per gradus producti dimensionum
                               * indicum unionis */
    i32 latitudo;             /* max per intermedios log2 elementorum,
                               * sursum (exacte, etiam ultra 2^64) */
} ContractioSumptus;

/* ordo binarius deterministicus (clavis avida supra) et dispositio
 * tabulae: offsets per vitam, primum aptum (effectus gradus non
 * superponitur operandis eius). Rete post hoc immutabile. NIHIL si
 * rete non clausum (v1; rete tunc mutabile manet) aut optiones
 * invalidae. Rete vacuum aut tensor solus: ordo sine gradibus.
 * Memoria ordinis (magnitudine retis, non tensorum) per
 * piscina_allocare: in piscina certae magnitudinis defectus fatalis
 * est, non NIHIL. */
ContractioOrdo*
contractio_ordinare (
                 ContractioRete* rete,
    constans ContractioOptiones* optiones,
                        Piscina* piscina);

ContractioSumptus
contractio_sumptus (
    constans ContractioOrdo* ordo);


/* ==================================================
 * Inspectio ordinis (probationes, mensura)
 * ================================================== */

/* numerus graduum: n - 1 pro n >= 1 tensoribus, 0 pro rete vacuo */
i32
contractio_ordo_gradus_numerus (
    constans ContractioOrdo* ordo);

/* gradus k: tensores sinister < dexter in effectum contrahuntur.
 * FALSUM si k extra fines (exitus non tanguntur). */
b32
contractio_ordo_gradus (
    constans ContractioOrdo* ordo,
                        i32  k,
                        i32* sinister,
                        i32* dexter,
                        i32* effectus);

/* sedes tensoris intermedii in tabula (offset et elementa, in
 * elementis i32). FALSUM pro tensore initiali aut ignoto, et si
 * ELEMENTA tabulae saturantur (intermedius aut finis >= 2^64 - 1;
 * si solum octeti saturantur, sedes tamen redduntur) - exitus tunc
 * non tanguntur. */
b32
contractio_ordo_sedes (
    constans ContractioOrdo* ordo,
                        i32  tensor,
                        i64* offset,
                        i64* elementa);

/* indices tensoris (initialis: ordine vocantis; intermedius: ordine
 * id ascendente), memoria ordinis - non mutanda. Scalaris: numerus
 * 0, indices NIHIL. FALSUM si tensor ignotus (exitus non tanguntur). */
b32
contractio_ordo_indices (
    constans ContractioOrdo*  ordo,
                        i32   tensor,
               constans i32** indices,
                        i32*  numerus);


/* ==================================================
 * Exsecutio
 * ================================================== */

/* officina gradus calidi: tabula UNA allocatione
 * (piscina_conari_allocare) octeti_officinae octetorum, plus
 * structura parva; reutilis per omnia puncta et primos. NIHIL si
 * octeti_officinae > octeti_maximi aut > 2^62, si sumptus saturati
 * (octeti aut multiplicationes: 2^64 operationes numquam finiuntur),
 * aut si allocatio deficit. */
ContractioOfficina*
contractio_officina_creare (
    constans ContractioOrdo* ordo,
                        i64  octeti_maximi,
                    Piscina* piscina);

/* valores[k] = elementa tensoris k, 0 <= v < modulus (praesumptum,
 * non probatum in ansa calida), 2 <= modulus < 2^32. exitus = tensor
 * densus super indices APERTOS ordine id (v1: unum elementum; rete
 * vacuum: 1). Nulla allocatio. FALSUM si ordo non huius retis aut
 * officina non huius ordinis. (T2: nondum exsequitur - FALSUM.) */
b32
contractio_exsequi_modulo (
      constans ContractioRete*  rete,
      constans ContractioOrdo*  ordo,
         ContractioOfficina*    officina,
    constans i32* constans*     valores,
                        i32     modulus,
                        i32*    exitus);

/* gradus referentiae: piscina temporaria per gradum
 * (notare/reficere), effectus per anulus->transcribe in piscinam
 * exitus. Ad retia parva: nullus limes octetorum. (T2: nondum
 * exsequitur - FALSUM.) */
b32
contractio_exsequi_anulo (
      constans ContractioRete*  rete,
      constans ContractioOrdo*  ordo,
    constans vacuum* constans*  valores,
           constans Anulus*     anulus,
                    Piscina*    piscina,
                     vacuum*    exitus);

#endif /* CONTRACTIO_H */
