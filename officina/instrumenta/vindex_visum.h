/* vindex_visum.h - conspectus + pictura vindicis (M3 chunk 6)
 *
 * SUTURA SALTUARII: functiones picturae CONSPECTUM PURUM accipiunt
 * (snapshot planum - numquam Machinula*, numquam IndiciumLector*,
 * numquam plagulae). Probationes cellularum sine capite conspectum
 * litteralem tradunt per tessera_pons_memoriae.
 *
 * Ordinatio: vectis tituli (0) | tabula fontis sinistra + columna
 * dextra (stiva supra, tabula mutabilis infra) | linea positionis
 * (h-2, acies macronum hic vivit) | status aut linea imperii (h-1).
 */

#ifndef VINDEX_VISUM_H
#define VINDEX_VISUM_H

#include "tessera.h"
#include "latina.h"
#include "chorda.h"

nomen enumeratio {
    VINDEX_TABULA_VARIABILIA = 0,
    VINDEX_TABULA_ANULUS,
    VINDEX_TABULA_PUNCTA,
    VINDEX_TABULA_MEMORIA
} VindexTabulaDextra;

nomen structura {
    chorda titulus;                /* nomen variabilis */
    chorda typus;                  /* typus scriptus */
    chorda valor;                  /* valor formatus */
} VindexOrdoVariabilis;

nomen structura {
    chorda functio;
    chorda positio;                /* "via:linea" */
} VindexOrdoTabulati;

nomen structura {
    /* fons */
    constans chorda* lineae;       /* lineae fontis (1-basatae) */
    i32     numerus_linearum;
    i32     linea_prima;           /* volumen: linea summa visa */
    i32     linea_currens;         /* 0 = nulla */
    constans i32* puncta_lineae;   /* lineae punctorum (huius viae) */
    i32     puncta_numerus;
    chorda  titulus_fontis;
    /* status */
    chorda modus;                 /* "PAUSA"/"BENE"/"VITIUM"... */
    chorda positio;               /* linea positionis (acies) */
    chorda status;                /* nuntius lineae imae */
    chorda imperium;              /* buffer lineae ':' */
       b32 imperium_apertum;
    /* columna dextra */
    constans VindexOrdoTabulati* tabulata;
    i32     tabulata_numerus;
    s32     tabula_dextra;         /* VindexTabulaDextra */
    constans VindexOrdoVariabilis* variabilia;
    i32     variabilia_numerus;
    constans chorda* anulus;       /* ordines positionum */
    i32     anulus_numerus;
    constans chorda* puncta;       /* ordines punctorum */
    i32     puncta_ordines_numerus;
    constans chorda* memoria;      /* ordines hex+ascii */
    i32     memoria_numerus;
} VindexConspectus;

/* pingit conspectum totum in opus (purgat + scribit; praesentare
 * vocantis est) */
vacuum
vindex_visum_pingere (
    constans VindexConspectus* conspectus,
                  TesseraOpus* opus);

#endif /* VINDEX_VISUM_H */
