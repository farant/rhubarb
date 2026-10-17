/* consilium.h - CONSILIUM: effectus ut data, consilium ante actum
 * (plan / apply; spec project-specs/consilium-spec.md, gradus S2)
 *
 * Officia (gradus) effectus suos PROPONUNT antequam fiunt; consilium =
 * gradus cum effectibus et ordinibus expressis. Nucleus PURUS (nulla
 * I/O): ordo per undas ex datis (qui scribit ante eum qui legit),
 * cyclus NOMINATUS, conflictus scriptorum, deduplicatio, sententiae
 * politicae per effectum. Mundus non rogatur - omnia in consilio
 * stant; expansio ('quae binaria ex hoc fonte?') consumptoris est
 * (gradus S3, silva.commissio).
 *
 * Recordum effectus = effectus.canon extensum (spec par. XI.1): verbum,
 * res cum forma, classis, certitudo, cognitio, exstat.
 *
 * ORDO EX DATIS: gradus B gradum A sequitur si A scribit (scribere,
 * delere, movere, instituere) rem quam B legit (legere, inspicere,
 * enumerare, exsequi, fontare) - etiam rem ABSENTEM probatam (exstat
 * FALSUM: creatio eam mutat, spec M3). Lectio rei quam gradus IPSE
 * scribit ingressus non est (M1). Res congruunt per formam: via
 * aequalis, praefixum, globus ('*' etiam '/' transit) - inter duo
 * exemplaria CONSERVATIVE (praefixa litteralia comparantur). Gradus
 * CAECUS (effectus ignoti) murus est: priores ante, posteriores post.
 */
#ifndef CONSILIUM_H
#define CONSILIUM_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"

nomen enumeratio {
    CONSILIUM_LEGERE = ZEPHYRUM,  /* contentum legitur */
    CONSILIUM_INSPICERE,          /* exsistentia/tempus probatur */
    CONSILIUM_ENUMERARE,          /* directorium enumeratur */
    CONSILIUM_SCRIBERE,
    CONSILIUM_DELERE,
    CONSILIUM_MOVERE,
    CONSILIUM_EXSEQUI,            /* programma currit */
    CONSILIUM_FONTARE,            /* codex in processum legitur */
    CONSILIUM_AMBITUM_LEGERE,     /* variabilis ambitus */
    CONSILIUM_INSTITUERE,         /* binarium extra arborem ponitur */
    CONSILIUM_RETE,               /* retis accessus */
    CONSILIUM_COMMITTERE,         /* git commit / push, tabularium */
    CONSILIUM_VERBA_NUMERUS
} ConsiliumVerbum;

/* scopus (effectus 'classis' + domus, spec M5) */
nomen enumeratio {
    CONSILIUM_ARBOR = ZEPHYRUM,     /* fons arboris */
    CONSILIUM_BUILD,                /* sub build/ */
    CONSILIUM_INSTRUMENTUM_DOMUS,   /* bin/ arboris */
    CONSILIUM_DOMUS,                /* ~ extra arborem (~/.bin,
                                     * ~/.rhubarb) */
    CONSILIUM_EXTERNA,              /* alibi extra arborem */
    CONSILIUM_SYSTEMA,
    CONSILIUM_TEMPORARIA,
    CONSILIUM_CLASSES_NUMERUS
} ConsiliumClassis;

nomen enumeratio {
    CONSILIUM_VIA = ZEPHYRUM,       /* res aequalis */
    CONSILIUM_PRAEFIXUM,            /* res sic incipit */
    CONSILIUM_GLOBUS                /* '*' '?' */
} ConsiliumForma;

nomen enumeratio {
    CONSILIUM_PLENA = ZEPHYRUM,
    CONSILIUM_EX_PARTE,
    CONSILIUM_NULLA
} ConsiliumCertitudo;

/* quomodo scimus (spec par. XI.1) */
nomen enumeratio {
    CONSILIUM_PROPOSITUM = ZEPHYRUM,  /* gradus ipse proponit */
    CONSILIUM_DECLARATUM,             /* declaratio (fabrica actio) */
    CONSILIUM_DERIVATUM,              /* analysis (crusta, portae) */
    CONSILIUM_OBSERVATUM              /* liber lectionum */
} ConsiliumCognitio;

nomen structura {
     ConsiliumVerbum verbum;
              chorda res;
      ConsiliumForma forma;
    ConsiliumClassis classis;
  ConsiliumCertitudo certitudo;
   ConsiliumCognitio cognitio;
                 b32 exstat;      /* lectiones: FALSUM = res probata
                                   * ABSENS (dependentia absentiae) */
              chorda causa;
} ConsiliumEffectus;

nomen structura ConsiliumGradus ConsiliumGradus;
structura ConsiliumGradus {
     chorda titulus;      /* identitas in consilio */
     chorda genus;        /* generator formatio institutio
                            * iudicium ... */
        Xar* effectus;     /* ConsiliumEffectus */
        Xar* sequitur;     /* chorda: tituli quos sequi debet */
        Xar* vestigium;    /* chorda: exemplaria (globi) scripturae
                            * PERMISSAE; vacuum = nulla restrictio */
        Xar* auctores;     /* chorda: qui eum proposuerunt
                            * (deduplicatio) */
         b32 caecus;       /* effectus ignoti: murus */
         s32 sumptus_ms;   /* -I = ignotus */
         i32 ordo_additionis;
};

nomen structura Consilium Consilium;

Consilium*
consilium_creare (
    Piscina* piscina);

/* gradum addere; titulus iam praesens = IDEM gradus redditur (auctor
 * additur, nihil duplicatur). auctor vacuum licet */
ConsiliumGradus*
consilium_gradum_addere (
     Consilium* consilium,
        chorda  titulus,
        chorda  genus,
        chorda  auctor);

/* effectum addere; effectus aequalis (verbum, res, forma, exstat) iam
 * in gradu = nihil additur */
vacuum
consilium_effectum_addere (
               Consilium* consilium,
         ConsiliumGradus* gradus,
       ConsiliumEffectus  effectus);

/* effectus cum valoribus ordinariis: forma VIA, classis ARBOR,
 * certitudo PLENA, cognitio PROPOSITUM, exstat VERUM */
ConsiliumEffectus
consilium_effectus (
    ConsiliumVerbum verbum,
             chorda res);

constans Xar*
consilium_gradus (
    constans Consilium* consilium);   /* ConsiliumGradus*, ordine
                                       * additionis */

ConsiliumGradus*
consilium_gradum_invenire (
     constans Consilium* consilium,
                 chorda  titulus);


/* ==================================================
 * ORDO
 * ================================================== */

/* arcus unus: 'posterior' 'prior' sequitur, cum causa */
nomen structura {
     constans ConsiliumGradus* prior;
     constans ConsiliumGradus* posterior;
                       chorda  causa;   /* 'A scribit R quod legitur',
                                        * 'sequitur expresse', 'murus
                                        * gradus caeci' */
} ConsiliumArcus;

nomen structura {
    Xar* undae;      /* Xar de Xar (ConsiliumGradus*): unda quaeque post
                      * priores; intra undam ordo additionis */
    Xar* arcus;      /* ConsiliumArcus */
    Xar* cyclus;     /* ConsiliumGradus*: gradus cycli NOMINATI; vacuus
                      * = nullus. Cum cyclo undae gradus extra cyclum
                      * solos tenent */
    /* chorda: sequitur ignotum ('B sequitur X: non in consilio') */
    Xar* recusationes;
} ConsiliumOrdo;

ConsiliumOrdo*
consilium_ordinare (
    constans Consilium* consilium,
               Piscina* piscina);


/* ==================================================
 * CONFLICTUS: duo gradus eandem rem scribunt sine ordine inter eos
 * (nec arcu nec catena arcuum). delere + delere conflictus non est
 * (spec M2); scribere/movere/instituere cum quolibet scriptore est.
 * ================================================== */

nomen structura {
     constans ConsiliumGradus* a;
     constans ConsiliumGradus* b;
                       chorda  res;
} ConsiliumConflictus;

Xar*
consilium_conflictus (
     constans Consilium* consilium,
 constans ConsiliumOrdo* ordo,
                Piscina* piscina);


/* ==================================================
 * POLITICA: regulae super effectus - prima congruens vincit;
 * nulla = LICET. Vestigium gradus (si datum) scripturas extra id
 * VETAT (forma generalis 'scripsit extra vestigium').
 * ================================================== */

nomen enumeratio {
    CONSILIUM_LICET = ZEPHYRUM,
    CONSILIUM_ROGANDUM,
    CONSILIUM_VETITUM
} ConsiliumSententia;

/* in regula: verbum aut classis quaelibet */
#define CONSILIUM_OMNIA (-I)

nomen structura {
                chorda titulus;     /* nomen regulae - in iudicio
                                     * citatur */
                   s32 verbum;      /* ConsiliumVerbum aut OMNIA */
                   s32 classis;     /* ConsiliumClassis aut OMNIA */
                chorda exemplar;    /* globus super res; vacuum =
                                     * quaelibet */
    ConsiliumSententia sententia;
                chorda causa;
} ConsiliumRegula;

nomen structura {
      constans ConsiliumGradus* gradus;
    constans ConsiliumEffectus* effectus;
            ConsiliumSententia  sententia;
                        chorda  regula;   /* titulus, aut 'vestigium' */
                        chorda  causa;
} ConsiliumIudicium;

/* iudicia effectuum quibus sententia non LICET (Xar de
 * ConsiliumIudicium, ordine graduum et effectuum) */
Xar*
consilium_iudicare (
    constans Consilium* consilium,
          constans Xar* regulae,     /* ConsiliumRegula */
               Piscina* piscina);


/* ==================================================
 * EFFUSIO: consilium legibile (homo) aut TSV (machina):
 *   UNDA n titulus genus
 *   ARCUS prior posterior causa
 *   CYCLUS titulus...
 *   CONFLICTUS a b res
 *   IUDICIUM gradus verbum res sententia regula causa
 * ================================================== */

chorda
consilium_scribere (
         constans Consilium* consilium,
     constans ConsiliumOrdo* ordo,
               constans Xar* conflictus,
               constans Xar* iudicia,
                        b32  machina,
                    Piscina* piscina);


/* ==================================================
 * DOCUMENTUM (consilium.canon, gradus S3a)
 * ================================================== */

/* consilium ex textu STML legere. NIHIL = RECUSATIO: *causa_out causas
 * OMNES nominat ('linea N: ...', valores ignoti cum notis).
 * *regulae_out (NIHIL licet) = Xar de ConsiliumRegula, ordine
 * documenti */
Consilium*
consilium_legere (
      chorda   textus,
     Piscina*  piscina,
         Xar** regulae_out,
      chorda*  causa_out);

constans character*
consilium_verbum_titulus (
    ConsiliumVerbum verbum);

constans character*
consilium_classis_titulus (
    ConsiliumClassis classis);

#endif /* CONSILIUM_H */
