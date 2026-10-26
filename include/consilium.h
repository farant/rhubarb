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
         b32 factum;       /* iam actum ANTE consilium (S5c2):
                             * iudicatur, numquam agitur */
         s32  sumptus_ms;   /* -I = ignotus */
         i32  ordo_additionis;
         Xar* mandatum;     /* chorda: argv exsecutionis (S5c1;
                            * <mandatum><argumentum valor=../>);
                            * vacuum = gradus non exsequibilis */
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
 * DOMUS (gradus S4): quae consumptores duo (silva.commissio,
 * bin/fabrica sanare) communicant
 * ================================================== */

/* classis effectus ex via: '~' aut '~/...' = DOMUS; absoluta sub domus
 * (extra radicem) = DOMUS; absoluta alibi extra radicem = EXTERNA;
 * 'bin/...' = INSTRUMENTUM_DOMUS; 'build/...' aut '.../build/...' =
 * BUILD; ceterum ARBOR. radix et domus absolutae sine '/' finali; NIHIL
 * licet (tum ea comparatio omittitur) */
ConsiliumClassis
consilium_classis_viae (
                  chorda  via,
      constans character* radix,
      constans character* domus);

/* regula 'arbor_secundaria': in arbore secundaria (git worktree) nihil
 * extra arborem scribitur - classis DOMUS VETITA (Franus: institutiones
 * ex main solo, 'bin/fabrica sanare installata' in main) */
ConsiliumRegula
consilium_regula_arboris_secundariae (
    Piscina* piscina);


/* ==================================================
 * EXSECUTIO (gradus S5a): consilium agere, observare, comparare
 *
 * Undae ordine, intra undam ordo additionis, gradus SERIATIM. Ante
 * quodque actum photographia (si sutura observat), post actum iterum:
 * scripturae observatae (photographiae differentia + scripturae a
 * gradu ipso notatae) EXTRA propositum gradus (effectus scriptores et
 * vestigium) = FRACTUM 'scripsit extra propositum'. Gradus caecus aut
 * cum effectu non PLENO extra nominat, non frangitur (nihil
 * promisit). Viae observatae in forma eadem ac res effectuum
 * (relativae ad radicem; domus '~/...') - consumptoris est.
 * ================================================== */

nomen enumeratio {
    CONSILIUM_ACTUM = ZEPHYRUM,  /* actum, codex 0, nihil extra
                                  * propositum */
    CONSILIUM_FRACTUM,           /* non incepit, codex != 0, scripsit
                                  * extra propositum, observatio
                                  * deficit */
    CONSILIUM_OMISSUM            /* dependentia fracta aut post
                                  * fracturam (SISTERE) */
} ConsiliumEventus;

nomen enumeratio {
    CONSILIUM_SISTERE = ZEPHYRUM,  /* post fracturam nihil novum
                                    * incipitur */
    CONSILIUM_PERGERE              /* independentes pergunt: fracturae
                                    * omnes nominantur */
} ConsiliumModus;

/* res una photographiae */
nomen structura {
    chorda via;
       s64 tempus_ns;
       s64 mensura;
} ConsiliumObservatio;

/* quod sutura->agere reddit */
nomen structura {
       s32  codex;        /* exitus; -I = non incepit */
       i32  duratio_ms;
    chorda  cauda;        /* effusionis cauda aut causa non incepti */
       Xar* notata;      /* chorda: scripturae a gradu ipso notatae
                          * (liber lectionum) - etiam extra
                          * photographiam; NIHIL = nullae */
} ConsiliumActum;

nomen structura {
    constans ConsiliumGradus* gradus;
            ConsiliumEventus  eventus;
                         i32  unda;       /* I-basata */
              ConsiliumActum  actum;      /* OMISSUM: codex -I */
                      chorda  causa;      /* 'exitus 2: ...',
                                           * 'dependentia fracta: X',
                                           * 'post fracturam ...',
                                           * 'scripsit extra
                                           * propositum: a, b +N' */
                         Xar* observata;  /* chorda: scripturae
                                           * observatae; NIHIL = non
                                           * observatum */
                         Xar* extra;      /* chorda: observatae extra
                                           * propositum (vacua =
                                           * sanum) */
                         Xar* monita;     /* chorda: DOMUS ALIENA
                                           * (parcum …QX6SQ0): ubi
                                           * regula scripturam in domo
                                           * VETAT (arbor secundaria),
                                           * mutatio in '~/' OBSERVATA
                                           * nec a gradu notata alterius
                                           * est (arbor alia, sessio) -
                                           * monitum, numquam fractura;
                                           * notata manet extra */
} ConsiliumActus;

nomen structura {
    vacuum* datum;
    /* gradum agere; FALSUM = non incepit (actum->cauda causam dicit) */
    b32 (*agere)(vacuum* datum, constans ConsiliumGradus* gradus,
        Piscina* piscina, ConsiliumActum* actum);
    /* photographia: Xar de ConsiliumObservatio ORDINATA per via;
     * NIHIL (functio) = sine observatione; FALSUM = deficit */
    b32 (*observare)(vacuum* datum, Piscina* piscina, Xar** out);
    /* ante actum / post eventum (acta, progressus); NIHIL licet */
    vacuum (*incipit)(vacuum* datum, constans ConsiliumGradus* gradus,
        i32 unda);
    vacuum (*notare)(vacuum* datum, constans ConsiliumActus* actus);
} ConsiliumSutura;

/* consilium agere. RECUSATIO ante quodvis actum (NIHIL + causa omnes
 * nominans): cyclus, sequitur ignotum, conflictus, sententia non
 * LICET sub 'regulae' (approbatio ROGANDI vocantis est). Aliter Xar de
 * ConsiliumActus, ordine exsecutionis; gradus omnes adsunt (OMISSI
 * quoque). 'facta' (S5b, ex consilium_diarium_resumere): gradus iam
 * acti non aguntur - ACTUM 'ex actis' (codex 0, sine incipit; notare
 * vocatur), dependentes procedunt. Gradus 'factum' (S5c2) idem: ACTUM
 * 'factum ante consilium'. */
Xar*
consilium_exsequi (
          constans Consilium* consilium,
                constans Xar* regulae,    /* ConsiliumRegula; NIHIL
                                          * licet */
    constans ConsiliumSutura* sutura,
              ConsiliumModus  modus,
                constans Xar* facta,      /* chorda tituli; NIHIL =
                                          * nulli */
                     Piscina* piscina,
                      chorda* causa_out);

/* viae mutatae inter photographias (ordinatas): novae, deletae,
 * tempore aut mensura mutatae - Xar de chorda, ordine viae */
Xar*
consilium_observata_comparare (
    constans Xar* ante,
    constans Xar* post,
         Piscina* piscina);

constans character*
consilium_eventus_titulus (
    ConsiliumEventus eventus);

/* acta legibilia (homo) aut TSV (machina):
 *   ACTUS unda titulus eventus codex duratio_ms causa
 *   EXTRA titulus via
 *   MONITUM titulus via   (domus aliena, …QX6SQ0: non fractura)
 * summa ultima: 'SUMMA acta N fracta N omissa N' */
chorda
consilium_acta_scribere (
    constans Xar* acta,      /* ConsiliumActus */
             b32  machina,
         Piscina* piscina);


/* ==================================================
 * DIARIUM (gradus S5b): acta exsecutionis, appendenda solum
 *
 * Lineae TSV (campi sine tab aut linea nova - purgantur in spatia):
 *   CONSILIUM  sigillum  arbor  novum|resumptum
 *   INCEPIT    unda  titulus
 *   ACTUS      unda  titulus  eventus  codex  duratio_ms  causa
 *   EXTRA      titulus  via
 *   ARBOR      arbor              (post quemque gradum)
 * 'arbor' = vestigium arboris a CONSUMPTORE computatum (HEAD + status
 * fontium; build/ et ignorata neglecta - Franus 2026-10-17):
 * bibliotheca id comparat solum. Pura: lineas componit et legit;
 * plagula consumptoris est.
 * ================================================== */

/* sigillum consilii (hex LXIV): gradus ordine additionis (titulus,
 * genus, caecus, effectus omnes, sequitur, vestigium) et regulae -
 * identitas eius quod agitur. Auctores, sumptus, causae extra (non
 * mutant quod agitur). */
chorda
consilium_sigillum (
    constans Consilium* consilium,
          constans Xar* regulae,   /* NIHIL licet */
               Piscina* piscina);

/* linea CONSILIUM (initium cursus) */
chorda
consilium_diarium_caput (
      chorda  sigillum,
      chorda  arbor,
         b32  resumptum,
     Piscina* piscina);

/* linea INCEPIT */
chorda
consilium_diarium_incepti (
    constans ConsiliumGradus* gradus,
                         i32  unda,
                     Piscina* piscina);

/* lineae ACTUS, EXTRA (quaeque), ARBOR */
chorda
consilium_diarium_actus (
    constans ConsiliumActus* actus,
                     chorda  arbor,
                    Piscina* piscina);

/* RESUMPTIO: gradus iam ACTI ('facta', Xar de chorda tituli) si
 * cursus ultimus idem consilium agit (sigillum) et arbor non mutata
 * est post lineam ARBOR ultimam (aut caput, si nulla). Catena: cursus
 * ultimus et priores 'resumptum' retro usque ad 'novum' primum
 * (inclusum) - cursus novus priores non hereditat. Linea ultima sine
 * '\n' (scriptura interrupta) neglegitur. FALSUM + causa: acta vacua,
 * consilium mutatum, arbor mutata, linea deformis (numerata). */
b32
consilium_diarium_resumere (
      chorda   textus,
      chorda   sigillum,
      chorda   arbor,
     Piscina*  piscina,
         Xar** facta_out,
      chorda*  causa_out);


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
