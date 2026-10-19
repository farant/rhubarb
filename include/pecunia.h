/* pecunia.h - Pecunia ut quantitates decimales EXACTAE
 *
 * Quantitas = s64 decimillesimarum (PECUNIA_SCALA = 10000) cum
 * moneta ISO 4217; nulli fluitantes usquam. Operationes quae
 * praecisionem perdere possunt modum rotundationis EXPLICITUM
 * accipiunt; partitio summam exacte servat. Omnis functio quae
 * deficere potest PecuniaVitium reddit (nominatum, numquam
 * FALSUM nudum) et exitum intactum relinquit.
 *
 * Tabula monetarum GENERATA est (lib/pecunia_monetae.c) ex
 * indicibus ISO 4217 fixis (probationes/fixa/iso4217/) et
 * supplemento cum fontibus. Praefixum moneta_ ALIENUM est
 * (include/moneta.h = cusio identitatum): omnia nomina monetarum
 * hic sub pecunia_.
 *
 * Spec: project-specs/pecunia-spec.md; plan: pecunia-plan.md.
 */

#ifndef PECUNIA_H
#define PECUNIA_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "fractio.h"

/* quantitas in decimillesimis: USD 12.34 = 123400 */
#define PECUNIA_SCALA (X * M)

/* Codex ISO 4217 majusculis, NUL terminatum ("USD"). SOLUM per
 * pecunia_moneta_ex_codice nascitur - ergo semper ordinem tabulae
 * nominat. */
nomen structura {
    character codex[IV];
} PecuniaMoneta;

nomen enumeratio {
    PECUNIA_BENE = ZEPHYRUM,
    PECUNIA_VITIUM_FORMA,               /* textus malformatus */
    PECUNIA_VITIUM_DECIMALES,           /* plus quam IV decimales */
    PECUNIA_VITIUM_SUPERFLUXUS,         /* extra s64 */
    PECUNIA_VITIUM_MONETA_IGNOTA,       /* codex non in tabula */
    PECUNIA_VITIUM_MONETA_SINE_UNITATE, /* XAU XXX... (N.A. in ISO) */
    PECUNIA_VITIUM_MONETAE_DIVERSAE,    /* USD + EUR */
    PECUNIA_VITIUM_NON_SOLVIBILIS,      /* partiri ad unitatem: totum
                                         * non multiplex unitatis */
    PECUNIA_VITIUM_RATIONES             /* rationes negativae aut
                                         * summa nulla */
} PecuniaVitium;

nomen enumeratio {
    PECUNIA_MONETA_CURRENS = ZEPHYRUM,  /* in indice ISO uno */
    PECUNIA_MONETA_HISTORICA,           /* retracta (index tertius) */
    PECUNIA_MONETA_EXTRA_ISO            /* GGP IMP JEP */
} PecuniaMonetaStatus;

/* Quantitas cum moneta - VALOR (XVI octeti), nulla allocatio. */
nomen structura {
              s64 quantitas;   /* decimillesimae */
    PecuniaMoneta moneta;
} Pecunia;

/* margines quantitatis (+-922337203685477.5807 unitatum) */
#define PECUNIA_QUANTITAS_MAXIMA ((s64)0x7FFFFFFFFFFFFFFFLL)
#define PECUNIA_QUANTITAS_MINIMA (-PECUNIA_QUANTITAS_MAXIMA - I)

nomen structura {
     constans character* codex;         /* "USD" */
                    s32  numerus;       /* 840; 0 si EXTRA_ISO */
                    s32  exponens;      /* unitas minor: 0..IV */
    PecuniaMonetaStatus  status;
     constans character* titulus;       /* "US Dollar" */
} PecuniaMonetaInformatio;


/* ==================================================
 * Vitia et monetae
 * ================================================== */

/* titulus vitii (numquam NIHIL, numquam vacuus) */
constans character*
pecunia_vitium_titulus (
    PecuniaVitium vitium);

/* Codex trium litterarum, sine casu ("usd" == "USD"); nihil aliud
 * condonatur (spatia, longitudo alia -> MONETA_IGNOTA). */
PecuniaVitium
pecunia_moneta_ex_codice (
           chorda  codex,
    PecuniaMoneta* exitus);

/* ordo tabulae; NIHIL solum pro moneta non ex
 * pecunia_moneta_ex_codice nata */
constans PecuniaMonetaInformatio*
pecunia_moneta_informatio (
    PecuniaMoneta m);

/* exponens unitatis minoris (USD 2, JPY 0); -I si ignota */
s32
pecunia_moneta_exponens (
    PecuniaMoneta m);

b32
pecunia_moneta_aequalis (
    PecuniaMoneta a,
    PecuniaMoneta b);


/* ==================================================
 * Valor et arithmetica (recusat, numquam circumvolvit)
 * ================================================== */

/* ex unitatibus minoribus: -1234 USD -> USD -12.3400; moneta non
 * ex pecunia_moneta_ex_codice -> MONETA_IGNOTA */
PecuniaVitium
pecunia_ex_unitatibus (
              s64  unitates,
    PecuniaMoneta  m,
          Pecunia* exitus);

Pecunia
pecunia_nihil (
    PecuniaMoneta m);

/* monetae diversae -> MONETAE_DIVERSAE (numquam conversio);
 * extra s64 -> SUPERFLUXUS */
PecuniaVitium
pecunia_adde (
     Pecunia  a,
     Pecunia  b,
     Pecunia* exitus);

PecuniaVitium
pecunia_subtrahe (
     Pecunia  a,
     Pecunia  b,
     Pecunia* exitus);

/* nega(MINIMA) -> SUPERFLUXUS */
PecuniaVitium
pecunia_nega (
     Pecunia  a,
     Pecunia* exitus);

/* summa ordine dato (superfluxus in medio cursu recusatur etsi summa
 * finalis caperet); n == 0 -> FORMA */
PecuniaVitium
pecunia_summa (
     constans Pecunia* a,
       memoriae_index  n,
              Pecunia* exitus);

/* *exitus -I, 0, +I; monetae diversae -> MONETAE_DIVERSAE */
PecuniaVitium
pecunia_compara (
    Pecunia  a,
    Pecunia  b,
        s32* exitus);

s32
pecunia_signum (
    Pecunia a);

/* eadem moneta et quantitas */
b32
pecunia_aequalis (
    Pecunia a,
    Pecunia b);


/* ==================================================
 * Rotundatio, multiplicatio, unitates solvibiles, cambium
 * ================================================== */

/* Modus SEMPER explicitus - nullus ordinarius tacitus. */
nomen enumeratio {
    PECUNIA_PAR = ZEPHYRUM,    /* dimidium ad par (argentariorum) */
    PECUNIA_DIMIDIUM_SURSUM,   /* dimidium a nihilo: 2.5->3, -2.5->-3 */
    PECUNIA_VERSUS_NIHIL,      /* truncatio */
    PECUNIA_SOLUM,             /* versus -infinitum */
    PECUNIA_TECTUM             /* versus +infinitum */
} PecuniaModus;

/* quantitas * f exacte (Magnus), rotundatio UNA ad scalam; moneta
 * servatur. 8.875% = 71/800. Extra s64 -> SUPERFLUXUS. */
PecuniaVitium
pecunia_multiplica (
         Pecunia  a,
         Fractio  f,
    PecuniaModus  modus,
         Piscina* piscina,
         Pecunia* exitus);

/* ad unitatem minorem monetae (USD 3.4590 -> 3.46 sub PAR; JPY ad
 * yen integros); effectus in scala IV */
PecuniaVitium
pecunia_ad_unitatem (
         Pecunia  a,
    PecuniaModus  modus,
         Pecunia* exitus);

/* VERUM si quantitas multiplex unitatis minoris est */
b32
pecunia_solvibilis (
    Pecunia a);

/* cambium = unitates monetae 'ad' per unitatem a.moneta, > 0 (aliter
 * FORMA); productum exactum, rotundatio una ad scalam (non ad
 * unitatem - vocans pecunia_ad_unitatem postea vocat si vult) */
PecuniaVitium
pecunia_convertere (
          Pecunia  a,
          Fractio  cambium,
    PecuniaMoneta  ad,
     PecuniaModus  modus,
          Piscina* piscina,
          Pecunia* exitus);


/* ==================================================
 * Partitio (summa partium = totum EXACTE)
 * ================================================== */

nomen enumeratio {
    PECUNIA_GRANUM_SCALA = ZEPHYRUM,  /* partes in decimillesimis */
    PECUNIA_GRANUM_UNITAS             /* partes solvibiles */
} PecuniaGranum;

/* Residuum maximum: pars i = solum(grana * r_i / summa) granorum,
 * grana reliqua singula partibus residuo maximo, PARIA AD INDICEM
 * MINOREM. Totum negativum per magnitudinem partitur, signum servat.
 * GRANUM_UNITAS: totum solvibile esse debet (NON_SOLVIBILIS); rationes
 * >= 0 cum summa positiva, n > 0 (RATIONES). exitus[n]; intacti in
 * vitio. Intermedia in Magno (piscina). */
PecuniaVitium
pecunia_partiri (
           Pecunia  totum,
      constans s64* rationes,
    memoriae_index  n,
     PecuniaGranum  granum,
           Piscina* piscina,
           Pecunia* exitus);

/* rationibus aequalibus: partes granum unum ad summum differunt */
PecuniaVitium
pecunia_dividere (
           Pecunia  totum,
    memoriae_index  n,
     PecuniaGranum  granum,
           Piscina* piscina,
           Pecunia* exitus);

#endif /* PECUNIA_H */
