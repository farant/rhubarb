/* sancti.h - bibliotheca sanctorum (project-specs/sancti-plan.md)
 *
 * CORPUS = directorium plagularum '.sanctus' (dialectus sanctorum,
 * canon sancti.canon infixus in lib/sancti_canon.c): plagula una per
 * personam aut coetum martyrum, sancti/<initialis>/<id>.sanctus.
 * Descriptiones anglicae; omnis assertio fontes nominat, omnis fons
 * excerptum verbatim fert (excerpta in speculo fontium probantur:
 * tools/sancti_excerpta.py, non hic).
 *
 * LECTIO IUDICAT: canon (vocabularium, genera, fines, cardinalitas)
 * deinde regulae lectoris - id = nomen plagulae; ids unici per
 * corpus; 'fontes' (ids spatiis separati) ad <fons> eiusdem
 * plagulae referunt; dies mensi congruit; ortus non post obitum;
 * nomen anglicum principale UNUM. Vitia NOMINANTUR (via:linea:
 * nuntius); plagula vitiosa non oneratur, ceterae oneratae manent.
 *
 * DIES INCERTI: campi ZEPHYRUM = ignoti (annus 0 non exstat; ante
 * Christum negativi). saeculum solum si annus ignotus.
 *
 * SANCTORALE: '<directorium>/sanctorale.tsv' (si adest) titulos
 * calendarii (lib/sanctorale.c) cum ids iungit - 'mensis TAB dies TAB
 * titulus TAB ids' (ids spatiis separati, '-' = celebratio sine
 * sancto, e.g. festa Domini). Id ignotum = vitium.
 */

#ifndef SANCTI_H
#define SANCTI_H

/* <aedilis corpus="lib/sancti.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"

nomen enumeratio {
    SANCTI_PERSONA = ZEPHYRUM,
    SANCTI_COETUS               /* martyres aut socii sub titulo uno */
} SanctiGenus;

nomen enumeratio {
    SANCTI_SANCTUS = ZEPHYRUM,
    SANCTI_BEATUS,
    SANCTI_VENERABILIS,
    SANCTI_SERVUS_DEI
} SanctiStatus;

nomen enumeratio {
    SANCTI_CALENDARIUM_GENERALE = ZEPHYRUM,  /* Romanum Generale 1969 */
    SANCTI_CALENDARIUM_USA,                  /* proprium USCCB */
    SANCTI_CALENDARIUM_MARTYROLOGIUM,        /* Martyrologium Romanum */
    SANCTI_CALENDARIUM_VETUS,                /* generale MCMLXII */
    SANCTI_CALENDARIUM_ALIUD                 /* nota dicit quod */
} SanctiCalendarium;

nomen enumeratio {
    SANCTI_GRADUS_NULLUS = ZEPHYRUM,         /* fons gradum non dat */
    SANCTI_GRADUS_SOLLEMNITAS,
    SANCTI_GRADUS_FESTUM,
    SANCTI_GRADUS_MEMORIA,
    SANCTI_GRADUS_MEMORIA_AD_LIBITUM,
    SANCTI_GRADUS_COMMEMORATIO
} SanctiGradus;

nomen enumeratio {
    SANCTI_CANONIZATIO_NULLA = ZEPHYRUM,     /* beati et ceteri */
    SANCTI_CANONIZATIO_FORMALIS,
    SANCTI_CANONIZATIO_AEQUIPOLLENS,
    SANCTI_CANONIZATIO_ANTE_CONGREGATIONEM
} SanctiCanonizatio;

/* dies incertus: notus FALSUM = elementum absens */
nomen structura {
       b32 notus;
       s32 annus;       /* ZEPHYRUM = ignotus */
       s32 mensis;      /* 1-12; ZEPHYRUM = ignotus */
       s32 dies;        /* 1-31; ZEPHYRUM = ignotus */
       s32 saeculum;    /* ZEPHYRUM = non datum */
       b32 circa;
       b32 julianum;    /* calendarium julianum in fonte */
    chorda locus;       /* vacua = ignotus */
} SanctiDies;

nomen structura {
    chorda lingua;      /* "en" "la" ... "alia" */
    chorda textus;
       b32 primarium;
} SanctiNomen;

nomen structura {
    SanctiCalendarium calendarium;
                  s32 mensis;
                  s32 dies;
         SanctiGradus gradus;
               chorda nota;
} SanctiFestum;

nomen structura {
    chorda textus;      /* nomen socii */
    chorda ad;          /* id plagulae eius; vacua = nulla */
} SanctiSocius;

nomen structura {
    chorda  id;
    chorda  opus;        /* mr1916 butler1812 ce1913 sedes usccb2026 */
    chorda  locus;
    chorda  url;         /* vacua = nulla */
       Xar* excerpta;    /* chorda */
} SanctiFons;

nomen structura {
               chorda  id;
               chorda  via;              /* plagula fontis */
          SanctiGenus  genus;
         SanctiStatus  status;
               chorda  titulus;          /* nomen anglicum principale */
               chorda  titulus_latinus;  /* vacua si nullum */
                  Xar* nomina;           /* SanctiNomen */
           SanctiDies  ortus;
           SanctiDies  obitus;
                  b32  martyr;
    SanctiCanonizatio  canonizatio;
           SanctiDies  canonizatio_dies;
               chorda  canonizatio_papa;
           SanctiDies  beatificatio;     /* notus FALSUM = nulla */
               chorda  beatificatio_papa;
                  Xar* festa;            /* SanctiFestum */
                  Xar* munera;           /* chorda */
                  Xar* patrocinia;       /* chorda */
                  Xar* socii;            /* SanctiSocius */
                  Xar* paragraphi;       /* chorda (prosa) */
                  Xar* fontes;           /* SanctiFons */
} Sanctus;

/* vitium nominatum: via:linea (linea ZEPHYRUM = plagula tota) */
nomen structura {
    chorda via;
       i32 linea;
    chorda nuntius;
} SanctiVitium;

nomen structura SanctiCorpus SanctiCorpus;

/* Plagulam unam legere et iudicare. via (forma 'x/<id>.sanctus')
 * vitia nominat et id contra nomen plagulae probat (vacua = non
 * probatur). Vitia ADDUNTUR ad 'vitia' (Xar de SanctiVitium); NIHIL
 * si plagula vitiosa est (aut memoria deficit). */
Sanctus*
sancti_legere (
                 chorda  textus,
                 chorda  via,
                Piscina* piscina,
    InternamentumChorda* intern,
                    Xar* vitia);

/* Corpus ex directorio (recursive, '*.sanctus'; sanctorale.tsv si
 * adest). NIHIL solum si directorium legi nequit aut memoria
 * deficit - vitia plagularum per sancti_vitia. */
SanctiCorpus*
sancti_corpus_aperire (
     constans character* directorium,
                Piscina* piscina,
    InternamentumChorda* intern);

/* vitia corporis (Xar de SanctiVitium), ordine viae et lineae */
constans Xar*
sancti_vitia (
    constans SanctiCorpus* corpus);

i32
sancti_numerus (
    constans SanctiCorpus* corpus);

/* sanctus ad indicem, ordine id */
constans Sanctus*
sancti_ad_indicem (
    constans SanctiCorpus* corpus,
                      i32  index);

/* per id; NIHIL si ignotus */
constans Sanctus*
sancti_invenire (
    constans SanctiCorpus* corpus,
                   chorda  id);

/* sancti cuius festum in calendario die dato cadit - Xar de
 * (constans Sanctus*), ordine id */
Xar*
sancti_diei (
    constans SanctiCorpus* corpus,
        SanctiCalendarium  calendarium,
                      s32  mensis,
                      s32  dies,
                  Piscina* piscina);

/* sancti quorum nomen aliquod fragmentum continet (litterae ASCII
 * sine discrimine maiusculi) - Xar de (constans Sanctus*), ordine id */
Xar*
sancti_quaerere (
    constans SanctiCorpus* corpus,
                   chorda  fragmentum,
                  Piscina* piscina);

/* sancti celebrationis sanctoralis (mensis, dies, titulus ut in
 * lib/sanctorale.c): Xar de (constans Sanctus*); vacuum si celebratio
 * sine sancto ('-'); NIHIL si celebratio in sanctorale.tsv absens */
Xar*
sancti_celebrationis (
    constans SanctiCorpus* corpus,
                      s32  mensis,
                      s32  dies,
                   chorda  titulus,
                  Piscina* piscina);

#endif /* SANCTI_H */
