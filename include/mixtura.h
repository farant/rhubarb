/* mixtura.h - Mixtura vocum: voces, ansae, greges, iussa ordinata,
 * nuntii
 *
 * Cor systematis soni (project-specs/sonus-spec.md §IV, §VI). Omnis
 * sonus per mixturam sonat: voces (tabula immutabilis aut sonitus
 * vivus) -> greges (effectus, musica, interfacies) -> summa -> exitus
 * stereo intertextus.
 *
 * DUAE PARTES, quia auditus (S3) eas in filis duobus ponet:
 *   Mixtura        - status vocum; iubere / reddere / nuntium_legere.
 *                    In S3 tota in filo soni vivit; hic (S2) filum
 *                    unum, sine auditu - probationes et exportatio.
 *   MixturaRector  - pars PRINCIPALIS: sedes vocum eligit et ansas
 *                    dat, ex nuntiis discit quae sedes liberae sunt.
 *                    Filum principale statum mixturae NUMQUAM legit.
 *
 * TEMPUS IN QUADRIS: omne iussum momentum fert ('incipe quadro n'),
 * numquam 'cum advenit'. mixtura_reddere iussa ordine temporis ad
 * quadrum EXACTUM intra agmen reddendum applicat. Iussum praeteritum
 * initio agminis proximi agit (tardum, numquam amissum) et numeratur.
 * Ergo eadem iussa = eadem exempla, offline aut vivum.
 *
 * NUMQUAM ALLOCAT POST CREATIONEM: voces, agmen iussorum, agmen
 * nuntiorum omnia in mixtura_creare. Tabulae exemplorum immutabiles
 * a vocante servantur donec nuntius VOX_FINITA eas liberat (mixtura
 * numquam liberat).
 *
 * ANSAE: (sedes, generatio). Iussum ad ansam stalam (sedes iam alii
 * voci data) ignoratur et numeratur - 'siste illud' post finem nihil
 * custodiae eget.
 *
 * USUS (offline):
 *   MixturaForma forma = { XXXII, CCLVI, CCLVI };
 *   Mixtura*       m = mixtura_creare(piscina, &forma);
 *   MixturaRector* r = mixtura_rectorem_creare(piscina, forma.voces);
 *   MixturaIussum  i;
 *   mixtura_iussum_parare(&i, MIXTURA_IUSSUM_SONITUM, ZEPHYRUM);
 *   i.ansa       = mixtura_rector_capere(r, I);
 *   i.parametra  = *sonitus_praeparatum("radius");
 *   (vacuum)mixtura_iubere(m, &i);
 *   mixtura_reddere(m, exitus, CCLVI);          (* exitus[DXII] *)
 *   dum (mixtura_nuntium_legere(m, &n))
 *       mixtura_rector_tractare(r, &n);
 */
/* <aedilis corpus="lib/mixtura.c"/> */
#ifndef MIXTURA_H
#define MIXTURA_H

#include "latina.h"
#include "piscina.h"
#include "sonitus.h"
#include "tibia.h"


/* ================================================================
 * ANSAE et GREGES
 * ================================================================ */

nomen structura {
    i32 sedes;       /* index vocis in [0, voces) */
    i32 generatio;   /* 0 = ansa nulla; aliter crescit per sedem */
} MixturaAnsa;

b32
mixtura_ansa_nulla (
    MixturaAnsa ansa);

/* arbor fixa (spec §VI-c): greges in summam, summa in exitum. Summa
 * silentium magistrum fert (spec §X.3: mutus summae) */
nomen enumeratio {
    MIXTURA_GREX_SUMMA = 0,
    MIXTURA_GREX_EFFECTUS,
    MIXTURA_GREX_MUSICA,
    MIXTURA_GREX_INTERFACIES,
    MIXTURA_GREGES
} MixturaGrex;


/* ================================================================
 * IUSSA - magnitudinis fixae (per anulum S3 transeunt)
 * ================================================================ */

nomen enumeratio {
    MIXTURA_IUSSUM_TABULAM = 0,   /* vox tabulae: exempla mono immutabilia */
    MIXTURA_IUSSUM_SONITUM,       /* vox sonitus viva (parametra + semen) */
    MIXTURA_IUSSUM_SISTERE,       /* evanescit per clivum, deinde finis */
    MIXTURA_IUSSUM_VOCEM_PONERE,  /* lucrum et positio vocis, cum clivo */
    MIXTURA_IUSSUM_GREGEM_PONERE, /* lucrum et mutus gregis, cum clivo */
    MIXTURA_IUSSUM_CENSUM_PETERE, /* nuntium CENSUS postulat */
    MIXTURA_IUSSUM_TIBIAM,        /* vox tibiae: parametra + nota,
                                   * porta aperta */
    MIXTURA_IUSSUM_NOTAM_PONERE,  /* nota tibiae cum clivo (quadra) */
    MIXTURA_IUSSUM_PORTAM_CLAUDERE /* tibia: remissio, deinde
                                   * VOX_FINITA naturalis; vox alia:
                                   * irritum */
} MixturaIussumGenus;

/* Campi plani (non unio): quisque genus suos legit, ceteros ignorat.
 * Semper per mixtura_iussum_parare incipe - ordinaria ponit */
nomen structura {
    MixturaIussumGenus genus;
                   s64 momentum;    /* quadrum quo agit (horologium
                                     * mixturae) */
           MixturaAnsa ansa;        /* voces: incipere, sistere, ponere */
           MixturaGrex grex;        /* incipere: grex vocis;
                                     * gregem_ponere: grex ipse */
                   f32 lucrum;      /* lineare, >= 0 */
                   f32 positio;     /* -1 sinistra .. 0 medium .. +1
                                     * dextra (potentia constans) */
                   b32 mutus;       /* gregem_ponere: tempus servat,
                                     * exitum nullat */
                   i32 clivus;      /* quadra transitus lucri /
                                     * positionis / evanescentiae; 0 =
                                     * statim (crepitum facere potest) */
    /* tabula */
         constans f32* exempla;     /* mono, immutabilia usque ad
                                     * VOX_FINITA */
                   i32 longitudo;   /* quadra */
                   b32 iterans;     /* circulus sine fine (ventus) */
    /* sonitus */
      SonitusParametra parametra;
                   i64 semen;
    /* tibia (incipere: tibia + nota; notam_ponere: nota + clivus) */
        TibiaParametra tibia;
                   f32 nota;        /* MIDI, fractiones licent */
} MixturaIussum;

/* omnia nulla, deinde: genus, momentum, lucrum 1, positio 0, grex
 * EFFECTUS, clivus 0 */
vacuum
mixtura_iussum_parare (
          MixturaIussum* iussum,
     MixturaIussumGenus  genus,
                    s64  momentum);


/* ================================================================
 * NUNTII - a mixtura ad filum principale
 * ================================================================ */

nomen enumeratio {
    MIXTURA_NUNTIUS_VOX_FINITA = 0,
    MIXTURA_NUNTIUS_CENSUS
} MixturaNuntiusGenus;

nomen enumeratio {
    MIXTURA_FINIS_NATURALIS = 0,  /* tabula aut sonitus finitus */
    MIXTURA_FINIS_IUSSU,        /* iussum SISTERE, post evanescentiam */
    MIXTURA_FINIS_SURREPTA        /* sedes voci novae data (rector
                                   * surripuit) */
} MixturaCausaFinis;

/* numeri ab creatione cumulati; apex ab censu priore */
nomen structura {
    i32 voces_activae;
    i32 iussa_tarda;      /* momentum praeteritum: initio agminis acta */
    i32 iussa_irrita;     /* ansa stala: ignorata */
    i32 iussa_amissa;     /* agmen iussorum plenum: mixtura_iubere
                           * FALSUM reddidit */
    i32 nuntii_amissi;    /* agmen nuntiorum plenum - sedes et tabulae
                           * tunc a rectore non liberantur: capacitatem
                           * >= II x voces pone */
    f32 apex;             /* |exemplum| maximum summae */
} MixturaCensus;

nomen structura {
    MixturaNuntiusGenus  genus;
                    s64  momentum;    /* quadrum eventus */
            MixturaAnsa  ansa;        /* VOX_FINITA */
      MixturaCausaFinis  causa;       /* VOX_FINITA */
           constans f32* exempla;     /* VOX_FINITA vocis tabulae: iam
                                      * liberabilis; aliter NIHIL */
          MixturaCensus census;      /* CENSUS */
} MixturaNuntius;


/* ================================================================
 * MIXTURA
 * ================================================================ */

nomen structura Mixtura Mixtura;

/* capacitates; omnia in creatione allocantur */
nomen structura {
    i32 voces;
    i32 iussa;     /* iussa nondum acta simul */
    i32 nuntii;    /* nuntii nondum lecti simul */
} MixturaForma;

/* NIHIL si forma invalida (aliqua 0) aut memoria deest */
Mixtura*
mixtura_creare (
                  Piscina* piscina,
    constans MixturaForma* forma);

/* iussum in agmen (copiatur); FALSUM si agmen plenum (numeratur) */
b32
mixtura_iubere (
                   Mixtura* m,
    constans MixturaIussum* iussum);

/* 'quadra' quadra stereo intertexta (L R L R ...) in exitus[II x
 * quadra] scribit; horologium provehit. Numquam allocat */
vacuum
mixtura_reddere (
    Mixtura* m,
        f32* exitus,
        i32  quadra);

/* FALSUM si nullus nuntius manet */
b32
mixtura_nuntium_legere (
           Mixtura* m,
    MixturaNuntius* nuntius);

/* quadra reddita ab creatione */
s64
mixtura_momentum (
    constans Mixtura* m);


/* ================================================================
 * RECTOR - pars principalis: sedes et ansae
 * ================================================================ */

nomen structura MixturaRector MixturaRector;

MixturaRector*
mixtura_rectorem_creare (
    Piscina* piscina,
        i32  voces);

/* sedes libera (generatio nova); plenis: surripit vocem prioritatis
 * infimae (aequalibus: vetustissimam) si eius prioritas <= haec -
 * mixtura veterem cum FINIS_SURREPTA nuntiat; aliter ansa nulla
 * (sonus non incipit) */
MixturaAnsa
mixtura_rector_capere (
    MixturaRector* r,
              i32  prioritas);

/* VOX_FINITA: sedes libera si generatio par (surrepta iam aliter data
 * - ignoratur); ceteri nuntii ignorantur */
vacuum
mixtura_rector_tractare (
              MixturaRector* r,
    constans MixturaNuntius* nuntius);

/* sedes ansae occupata generatione eadem */
b32
mixtura_rector_vivit (
    constans MixturaRector* r,
               MixturaAnsa  ansa);

/* sedes capta sed vox numquam incepta (iussum recusatum): libera, si
 * generatio par; aliter nihil */
vacuum
mixtura_rector_dimittere (
     MixturaRector* r,
       MixturaAnsa  ansa);

/* sedes occupatae */
i32
mixtura_rector_occupatae (
    constans MixturaRector* r);

#endif /* MIXTURA_H */
