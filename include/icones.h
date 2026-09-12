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

nomen enumeratio {
    ICONES_SUCCESSUS = ZEPHYRUM,
    ICONES_ERROR_DESUNT,        /* fons aut titulus */
    ICONES_ERROR_FONS,          /* pixela NIHIL aut dimensio <= 0 */
    ICONES_ERROR_MINIMUS,       /* latus < XVI: NIHIL tegitur */
    ICONES_ERROR_LATERA,        /* vexillum sine codice .icns */
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
 * geminae EOSDEM octetos ferunt (idem datum), ergo .iconset et .icns
 * constructione consentiunt, non duabus semitis. */
nomen structura {
       i32 latera;   /* 16, 32, 64 ... pixela */
    chorda semita;   /* "icon_16x16.png" */
    chorda octeti;   /* PNG */
} IconesPars;

nomen structura {
     Xar* partes;         /* IconesPars */
     i32  omissa;         /* vexilla laterum quae fons non tulit */
     i32  latera_fontis;  /* quadratum post recisionem */
} IconesFructus;

/* Reddere: PURUM (nihil in disco tangit). Quadratum ad centrum recidit,
 * latera <= quadrato reddit, cetera in 'omissa' refert. */
b32
icones_reddere (
    constans Icones* petitio,
      IconesFructus* fructus,
       IconesStatus* status,
             chorda* sedes_vitii,
            Piscina* piscina);

/* Scribere .iconset: directorium <via_radicis>/<titulus>.iconset cum
 * parentibus, deinde plagulam per partem. */
b32
icones_iconset_scribere (
    constans IconesFructus* fructus,
        constans character* via_radicis,
              IconesStatus* status,
                    chorda* sedes_vitii,
                   Piscina* piscina);

/* Codificare .icns: PURUM. 'icns', longitudo tota, deinde chunki
 * (codex IV litterarum + longitudo INCLUSO capite VIII octetorum +
 * onus PNG). Redde chordam vacuam nisi status == SUCCESSUS. */
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
