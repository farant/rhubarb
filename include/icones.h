/* icones.h - Icones macOS (.icns, .iconset) ex una imagine.
 *
 * Ex imagine una fasciculum iconum reddit: .iconset (directorium
 * plagularum PNG) et .icns (continens unum). fasciculum iconem PONIT et
 * in CFBundleIconFile nominat - eam facere non potest. Hoc id claudit.
 *
 * NON DECODIFICAT: vocans Imaginem iam decodificatam praebet, ergo
 * clausura huius bibliothecae imago_typus + imago_png + flatura est,
 * omnia nostra. stb_image in instrumento vivit, non hic (imago_typus.h
 * ob hoc ipsum a imago.h scissum est: forum vendores I -> 0).
 *
 * Specificatio: project-specs/icones-spec.md v1.
 *
 * NON tractat: litteras marginales, acuitionem, lectionem PIXELORUM
 * .icns, formas non Apple - singula in par. X parcata.
 */

#ifndef ICONES_H
#define ICONES_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "imago_typus.h"
#include "xar.h"

/* Omnis recusatio sedem nominat (sedes_vitii): campum absentem, latus,
 * bita ignota, semitam partis vitiosae aut viam. Nomina camporum in
 * memoria STATICA vivunt, ergo etiam piscina absens nominatur. */
nomen enumeratio {
    ICONES_SUCCESSUS = ZEPHYRUM,
    ICONES_ERROR_DESUNT,        /* argumentum NIHIL, titulus, fons
                                 * absens, partes, octeti aut onus
                                 * partis */
    ICONES_ERROR_FONS,          /* fons praesens sed pixela NIHIL aut
                                 * dimensio ZEPHYRUM */
    ICONES_ERROR_MINIMUS,       /* latus < XVI, aut latus PETITUM
                                 * nullum tegitur (D10) */
    ICONES_ERROR_LATERA,        /* bit ignotum, aut semita partis
                                 * tabulae ignota */
    ICONES_ERROR_PNG,           /* imago_png recusavit */
    ICONES_ERROR_DIRECTORIUM,
    ICONES_ERROR_SCRIPTIO,
    ICONES_ERROR_MEMORIA
} IconesStatus;

/* Recisio: una electio in v1. Enumeratio, non b32, quia litterae
 * marginales parcatae sunt et enumerationes appenduntur - ergo
 * parcatum sine signatura mutata advenit. */
nomen enumeratio {
    ICONES_RECIDERE_CENTRUM
} IconesModusRecidendi;

/* Latera petita ut VEXILLA: universum magnitudinum fixum et parvum est,
 * ergo tabula bitorum exacta est et piscina non eget; tabula a vocante
 * data latera admitteret quibus nullus codex .icns respondet. */
#define ICONES_LATERA_XVI       I      /*   16 px */
#define ICONES_LATERA_XXXII     II     /*   32 px */
#define ICONES_LATERA_LXIV      IV     /*   64 px */
#define ICONES_LATERA_CXXVIII   VIII   /*  128 px */
#define ICONES_LATERA_CCLVI     XVI    /*  256 px */
#define ICONES_LATERA_DXII      XXXII  /*  512 px */
#define ICONES_LATERA_MXXIV     LXIV   /* 1024 px */
#define ICONES_SETUM_APPLE      (I|II|IV|VIII|XVI|XXXII|LXIV)

nomen structura {
       constans Imago* fons;
               chorda  titulus;   /* "AppIcon" -> AppIcon.iconset */
 IconesModusRecidendi  recidere;
                  i32  latera_petita;  /* ZEPHYRUM = SETUM_APPLE */
} Icones;

/* Pars = PLAGULA una .iconset. Decem partes, SEPTEM redditiones: tria
 * latera bis sub nominibus logicis diversis apparent, et partes
 * geminae EOSDEM octetos PNG ferunt (idem datum).
 *
 * onus_icns = octeti quos CONTINENS .icns fert. Pro codicibus
 * recentioribus IIDEM ac octeti (idem datum). Pro ic04 et ic05 ARGB
 * earundem pixelorum: Finder et iconutil PNG in his locis PERPERAM
 * legunt - strepitus, non icon (mensuratum 2026-09-12; sips recte
 * legit, Finder non). Ergo .iconset et .icns PIXELIS consentiunt, non
 * semper octetis. */
nomen structura {
       i32 latera;      /* 16, 32, 64 ... pixela */
    chorda semita;      /* "icon_16x16.png" */
    chorda octeti;      /* PNG - plagula .iconset */
    chorda onus_icns;   /* chunkus .icns: PNG aut ARGB */
} IconesPars;

nomen structura {
     Xar* partes;         /* IconesPars */
     i32  omissa;         /* vexilla laterum quae fons non tulit */
     i32  latera_fontis;  /* quadratum post recisionem */
  chorda  titulus;        /* ex petitione transcriptus: scriptor
                           * .iconset fructum SOLUM accipit, ergo
                           * sine hoc titulum directorii derivare
                           * non posset */
} IconesFructus;

/* Reddere: PURUM (nihil in disco tangit). Quadratum ad centrum recidit,
 * latera <= quadrato reddit, cetera in 'omissa' refert. Si latus
 * PETITUM NULLUM tegitur, RECUSAT (MINIMUS, sedes latera petita ut
 * "128 1024"): successus sine parte icon nullus esset (D10). */
b32
icones_reddere (
    constans Icones* petitio,
      IconesFructus* fructus,
       IconesStatus* status,
             chorda* sedes_vitii,
            Piscina* piscina);

/* Scribere .iconset: directorium <via_radicis>/<titulus>.iconset cum
 * parentibus, deinde plagulam per partem. Scriptores TRES fructum
 * IUDICANT antequam quicquam tangant: partes nullae, semita tabulae
 * ignota et octeti vacui recusantur (fructus manu factus). */
b32
icones_iconset_scribere (
    constans IconesFructus* fructus,
        constans character* via_radicis,
              IconesStatus* status,
                    chorda* sedes_vitii,
                   Piscina* piscina);

/* Codificare .icns: PURUM. 'icns', longitudo tota, deinde chunki
 * (codex IV litterarum + longitudo INCLUSO capite VIII octetorum +
 * onus_icns partis: PNG, aut ARGB pro ic04/ic05). Redde chordam vacuam
 * nisi status == SUCCESSUS. */
chorda
icones_icns_codificare (
    constans IconesFructus* fructus,
              IconesStatus* status,
                   Piscina* piscina);

b32
icones_icns_scribere (
    constans IconesFructus* fructus,
        constans character* via,
              IconesStatus* status,
                    chorda* sedes_vitii,
                   Piscina* piscina);

#endif /* ICONES_H */
