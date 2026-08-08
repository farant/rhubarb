/* GENERATUM a canon_coquere e natura/cocta/planta.canon - NOLI MANU MUTARE.
 * (signum generata-custodis: editio manualis vetatur)
 * Lex folii: involve, numquam emenda - regeneratio
 * emendationes vincit. Lectio sola; plagulam canone
 * sanam praesumit (ignota tacite praeterit).
 * Regeneratio: bin/canon_coquere natura/cocta/planta.canon -praefixum Planta -caput include/planta_lectio.h -corpus lib/planta_lectio.c */
#ifndef PLANTA_LECTIO_H
#define PLANTA_LECTIO_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include "internamentum.h"
#include "stml.h"

nomen structura PlantaIndividua PlantaIndividua;
nomen structura PlantaPlanta PlantaPlanta;
nomen structura PlantaPlantaRadix PlantaPlantaRadix;
nomen structura PlantaPlantaCaulis PlantaPlantaCaulis;
nomen structura PlantaPlantaFolium PlantaPlantaFolium;
nomen structura PlantaPlantaFlos PlantaPlantaFlos;
nomen structura PlantaPlantaFructus PlantaPlantaFructus;
nomen structura PlantaPlantaSemen PlantaPlantaSemen;
nomen structura PlantaPlantaGenerat PlantaPlantaGenerat;
nomen structura PlantaPlantaLaborat PlantaPlantaLaborat;
nomen structura PlantaPlantaHistoria PlantaPlantaHistoria;
nomen structura PlantaRosaceae PlantaRosaceae;
nomen structura PlantaRosaceaeRadix PlantaRosaceaeRadix;
nomen structura PlantaRosaceaeCaulis PlantaRosaceaeCaulis;
nomen structura PlantaRosaceaeFolium PlantaRosaceaeFolium;
nomen structura PlantaRosaceaeFlos PlantaRosaceaeFlos;
nomen structura PlantaRosaceaeFructus PlantaRosaceaeFructus;
nomen structura PlantaRosaceaeSemen PlantaRosaceaeSemen;
nomen structura PlantaRosaceaeGenerat PlantaRosaceaeGenerat;
nomen structura PlantaRosaceaeLaborat PlantaRosaceaeLaborat;
nomen structura PlantaRosaceaeHistoria PlantaRosaceaeHistoria;
nomen structura PlantaRosa PlantaRosa;
nomen structura PlantaRosaRadix PlantaRosaRadix;
nomen structura PlantaRosaCaulis PlantaRosaCaulis;
nomen structura PlantaRosaFolium PlantaRosaFolium;
nomen structura PlantaRosaFlos PlantaRosaFlos;
nomen structura PlantaRosaFructus PlantaRosaFructus;
nomen structura PlantaRosaSemen PlantaRosaSemen;
nomen structura PlantaRosaGenerat PlantaRosaGenerat;
nomen structura PlantaRosaLaborat PlantaRosaLaborat;
nomen structura PlantaRosaHistoria PlantaRosaHistoria;
nomen structura PlantaMalus PlantaMalus;
nomen structura PlantaMalusRadix PlantaMalusRadix;
nomen structura PlantaMalusCaulis PlantaMalusCaulis;
nomen structura PlantaMalusFolium PlantaMalusFolium;
nomen structura PlantaMalusFlos PlantaMalusFlos;
nomen structura PlantaMalusFructus PlantaMalusFructus;
nomen structura PlantaMalusSemen PlantaMalusSemen;
nomen structura PlantaMalusGenerat PlantaMalusGenerat;
nomen structura PlantaMalusLaborat PlantaMalusLaborat;
nomen structura PlantaMalusHistoria PlantaMalusHistoria;
nomen structura PlantaRosaCanina PlantaRosaCanina;
nomen structura PlantaRosaCaninaRadix PlantaRosaCaninaRadix;
nomen structura PlantaRosaCaninaCaulis PlantaRosaCaninaCaulis;
nomen structura PlantaRosaCaninaFolium PlantaRosaCaninaFolium;
nomen structura PlantaRosaCaninaFlos PlantaRosaCaninaFlos;
nomen structura PlantaRosaCaninaFructus PlantaRosaCaninaFructus;
nomen structura PlantaRosaCaninaSemen PlantaRosaCaninaSemen;
nomen structura PlantaRosaCaninaGenerat PlantaRosaCaninaGenerat;
nomen structura PlantaRosaCaninaLaborat PlantaRosaCaninaLaborat;
nomen structura PlantaRosaCaninaHistoria PlantaRosaCaninaHistoria;
nomen structura PlantaMalusDomestica PlantaMalusDomestica;
nomen structura PlantaMalusDomesticaRadix PlantaMalusDomesticaRadix;
nomen structura PlantaMalusDomesticaCaulis PlantaMalusDomesticaCaulis;
nomen structura PlantaMalusDomesticaFolium PlantaMalusDomesticaFolium;
nomen structura PlantaMalusDomesticaFlos PlantaMalusDomesticaFlos;
nomen structura PlantaMalusDomesticaFructus PlantaMalusDomesticaFructus;
nomen structura PlantaMalusDomesticaSemen PlantaMalusDomesticaSemen;
nomen structura PlantaMalusDomesticaGenerat PlantaMalusDomesticaGenerat;
nomen structura PlantaMalusDomesticaLaborat PlantaMalusDomesticaLaborat;
nomen structura PlantaMalusDomesticaHistoria PlantaMalusDomesticaHistoria;
nomen structura PlantaGrannySmith PlantaGrannySmith;
nomen structura PlantaGrannySmithRadix PlantaGrannySmithRadix;
nomen structura PlantaGrannySmithCaulis PlantaGrannySmithCaulis;
nomen structura PlantaGrannySmithFolium PlantaGrannySmithFolium;
nomen structura PlantaGrannySmithFlos PlantaGrannySmithFlos;
nomen structura PlantaGrannySmithFructus PlantaGrannySmithFructus;
nomen structura PlantaGrannySmithSemen PlantaGrannySmithSemen;
nomen structura PlantaGrannySmithGenerat PlantaGrannySmithGenerat;
nomen structura PlantaGrannySmithLaborat PlantaGrannySmithLaborat;
nomen structura PlantaGrannySmithHistoria PlantaGrannySmithHistoria;
nomen structura PlantaNota PlantaNota;
nomen structura PlantaHistoriaEventum PlantaHistoriaEventum;

nomen enumeratio {
    PLANTA_PLANTA_HABITUS_ABSENS = ZEPHYRUM,
    PLANTA_PLANTA_HABITUS_HERBA,
    PLANTA_PLANTA_HABITUS_FRUTEX,
    PLANTA_PLANTA_HABITUS_ARBOR,
    PLANTA_PLANTA_HABITUS_SCANDENS
} PlantaPlantaHabitus;

nomen enumeratio {
    PLANTA_PLANTA_DURATA_ABSENS = ZEPHYRUM,
    PLANTA_PLANTA_DURATA_ANNUA,
    PLANTA_PLANTA_DURATA_BIENNIS,
    PLANTA_PLANTA_DURATA_PERENNIS
} PlantaPlantaDurata;

nomen enumeratio {
    PLANTA_PLANTA_FOLIA_PERMANENTIA_ABSENS = ZEPHYRUM,
    PLANTA_PLANTA_FOLIA_PERMANENTIA_DECIDUA,
    PLANTA_PLANTA_FOLIA_PERMANENTIA_SEMPERVIRENS
} PlantaPlantaFoliaPermanentia;

nomen enumeratio {
    PLANTA_PLANTA_STATUS_VITA_ABSENS = ZEPHYRUM,
    PLANTA_PLANTA_STATUS_VITA_SEMEN,
    PLANTA_PLANTA_STATUS_VITA_GERMINANS,
    PLANTA_PLANTA_STATUS_VITA_IUVENIS,
    PLANTA_PLANTA_STATUS_VITA_FLORENS,
    PLANTA_PLANTA_STATUS_VITA_FRUCTIFERA,
    PLANTA_PLANTA_STATUS_VITA_QUIESCENS,
    PLANTA_PLANTA_STATUS_VITA_MORTUA
} PlantaPlantaStatusVita;

nomen enumeratio {
    PLANTA_PLANTA_ANIMA_ABSENS = ZEPHYRUM,
    PLANTA_PLANTA_ANIMA_VEGETATIVA,
    PLANTA_PLANTA_ANIMA_SENSITIVA,
    PLANTA_PLANTA_ANIMA_RATIONALIS
} PlantaPlantaAnima;

nomen enumeratio {
    PLANTA_PLANTA_STATUS_CONDITIO_ABSENS = ZEPHYRUM,
    PLANTA_PLANTA_STATUS_CONDITIO_RECENS,
    PLANTA_PLANTA_STATUS_CONDITIO_VETUS,
    PLANTA_PLANTA_STATUS_CONDITIO_CORRUPTA
} PlantaPlantaStatusConditio;

nomen enumeratio {
    PLANTA_ROSACEAE_HABITUS_ABSENS = ZEPHYRUM,
    PLANTA_ROSACEAE_HABITUS_HERBA,
    PLANTA_ROSACEAE_HABITUS_FRUTEX,
    PLANTA_ROSACEAE_HABITUS_ARBOR,
    PLANTA_ROSACEAE_HABITUS_SCANDENS
} PlantaRosaceaeHabitus;

nomen enumeratio {
    PLANTA_ROSACEAE_DURATA_ABSENS = ZEPHYRUM,
    PLANTA_ROSACEAE_DURATA_ANNUA,
    PLANTA_ROSACEAE_DURATA_BIENNIS,
    PLANTA_ROSACEAE_DURATA_PERENNIS
} PlantaRosaceaeDurata;

nomen enumeratio {
    PLANTA_ROSACEAE_FOLIA_PERMANENTIA_ABSENS = ZEPHYRUM,
    PLANTA_ROSACEAE_FOLIA_PERMANENTIA_DECIDUA,
    PLANTA_ROSACEAE_FOLIA_PERMANENTIA_SEMPERVIRENS
} PlantaRosaceaeFoliaPermanentia;

nomen enumeratio {
    PLANTA_ROSACEAE_STATUS_VITA_ABSENS = ZEPHYRUM,
    PLANTA_ROSACEAE_STATUS_VITA_SEMEN,
    PLANTA_ROSACEAE_STATUS_VITA_GERMINANS,
    PLANTA_ROSACEAE_STATUS_VITA_IUVENIS,
    PLANTA_ROSACEAE_STATUS_VITA_FLORENS,
    PLANTA_ROSACEAE_STATUS_VITA_FRUCTIFERA,
    PLANTA_ROSACEAE_STATUS_VITA_QUIESCENS,
    PLANTA_ROSACEAE_STATUS_VITA_MORTUA
} PlantaRosaceaeStatusVita;

nomen enumeratio {
    PLANTA_ROSACEAE_ANIMA_ABSENS = ZEPHYRUM,
    PLANTA_ROSACEAE_ANIMA_VEGETATIVA,
    PLANTA_ROSACEAE_ANIMA_SENSITIVA,
    PLANTA_ROSACEAE_ANIMA_RATIONALIS
} PlantaRosaceaeAnima;

nomen enumeratio {
    PLANTA_ROSACEAE_STATUS_CONDITIO_ABSENS = ZEPHYRUM,
    PLANTA_ROSACEAE_STATUS_CONDITIO_RECENS,
    PLANTA_ROSACEAE_STATUS_CONDITIO_VETUS,
    PLANTA_ROSACEAE_STATUS_CONDITIO_CORRUPTA
} PlantaRosaceaeStatusConditio;

nomen enumeratio {
    PLANTA_ROSA_HABITUS_ABSENS = ZEPHYRUM,
    PLANTA_ROSA_HABITUS_HERBA,
    PLANTA_ROSA_HABITUS_FRUTEX,
    PLANTA_ROSA_HABITUS_ARBOR,
    PLANTA_ROSA_HABITUS_SCANDENS
} PlantaRosaHabitus;

nomen enumeratio {
    PLANTA_ROSA_DURATA_ABSENS = ZEPHYRUM,
    PLANTA_ROSA_DURATA_ANNUA,
    PLANTA_ROSA_DURATA_BIENNIS,
    PLANTA_ROSA_DURATA_PERENNIS
} PlantaRosaDurata;

nomen enumeratio {
    PLANTA_ROSA_FOLIA_PERMANENTIA_ABSENS = ZEPHYRUM,
    PLANTA_ROSA_FOLIA_PERMANENTIA_DECIDUA,
    PLANTA_ROSA_FOLIA_PERMANENTIA_SEMPERVIRENS
} PlantaRosaFoliaPermanentia;

nomen enumeratio {
    PLANTA_ROSA_STATUS_VITA_ABSENS = ZEPHYRUM,
    PLANTA_ROSA_STATUS_VITA_SEMEN,
    PLANTA_ROSA_STATUS_VITA_GERMINANS,
    PLANTA_ROSA_STATUS_VITA_IUVENIS,
    PLANTA_ROSA_STATUS_VITA_FLORENS,
    PLANTA_ROSA_STATUS_VITA_FRUCTIFERA,
    PLANTA_ROSA_STATUS_VITA_QUIESCENS,
    PLANTA_ROSA_STATUS_VITA_MORTUA
} PlantaRosaStatusVita;

nomen enumeratio {
    PLANTA_ROSA_ANIMA_ABSENS = ZEPHYRUM,
    PLANTA_ROSA_ANIMA_VEGETATIVA,
    PLANTA_ROSA_ANIMA_SENSITIVA,
    PLANTA_ROSA_ANIMA_RATIONALIS
} PlantaRosaAnima;

nomen enumeratio {
    PLANTA_ROSA_STATUS_CONDITIO_ABSENS = ZEPHYRUM,
    PLANTA_ROSA_STATUS_CONDITIO_RECENS,
    PLANTA_ROSA_STATUS_CONDITIO_VETUS,
    PLANTA_ROSA_STATUS_CONDITIO_CORRUPTA
} PlantaRosaStatusConditio;

nomen enumeratio {
    PLANTA_MALUS_HABITUS_ABSENS = ZEPHYRUM,
    PLANTA_MALUS_HABITUS_HERBA,
    PLANTA_MALUS_HABITUS_FRUTEX,
    PLANTA_MALUS_HABITUS_ARBOR,
    PLANTA_MALUS_HABITUS_SCANDENS
} PlantaMalusHabitus;

nomen enumeratio {
    PLANTA_MALUS_DURATA_ABSENS = ZEPHYRUM,
    PLANTA_MALUS_DURATA_ANNUA,
    PLANTA_MALUS_DURATA_BIENNIS,
    PLANTA_MALUS_DURATA_PERENNIS
} PlantaMalusDurata;

nomen enumeratio {
    PLANTA_MALUS_FOLIA_PERMANENTIA_ABSENS = ZEPHYRUM,
    PLANTA_MALUS_FOLIA_PERMANENTIA_DECIDUA,
    PLANTA_MALUS_FOLIA_PERMANENTIA_SEMPERVIRENS
} PlantaMalusFoliaPermanentia;

nomen enumeratio {
    PLANTA_MALUS_STATUS_VITA_ABSENS = ZEPHYRUM,
    PLANTA_MALUS_STATUS_VITA_SEMEN,
    PLANTA_MALUS_STATUS_VITA_GERMINANS,
    PLANTA_MALUS_STATUS_VITA_IUVENIS,
    PLANTA_MALUS_STATUS_VITA_FLORENS,
    PLANTA_MALUS_STATUS_VITA_FRUCTIFERA,
    PLANTA_MALUS_STATUS_VITA_QUIESCENS,
    PLANTA_MALUS_STATUS_VITA_MORTUA
} PlantaMalusStatusVita;

nomen enumeratio {
    PLANTA_MALUS_ANIMA_ABSENS = ZEPHYRUM,
    PLANTA_MALUS_ANIMA_VEGETATIVA,
    PLANTA_MALUS_ANIMA_SENSITIVA,
    PLANTA_MALUS_ANIMA_RATIONALIS
} PlantaMalusAnima;

nomen enumeratio {
    PLANTA_MALUS_STATUS_CONDITIO_ABSENS = ZEPHYRUM,
    PLANTA_MALUS_STATUS_CONDITIO_RECENS,
    PLANTA_MALUS_STATUS_CONDITIO_VETUS,
    PLANTA_MALUS_STATUS_CONDITIO_CORRUPTA
} PlantaMalusStatusConditio;

nomen enumeratio {
    PLANTA_ROSA_CANINA_HABITUS_ABSENS = ZEPHYRUM,
    PLANTA_ROSA_CANINA_HABITUS_HERBA,
    PLANTA_ROSA_CANINA_HABITUS_FRUTEX,
    PLANTA_ROSA_CANINA_HABITUS_ARBOR,
    PLANTA_ROSA_CANINA_HABITUS_SCANDENS
} PlantaRosaCaninaHabitus;

nomen enumeratio {
    PLANTA_ROSA_CANINA_DURATA_ABSENS = ZEPHYRUM,
    PLANTA_ROSA_CANINA_DURATA_ANNUA,
    PLANTA_ROSA_CANINA_DURATA_BIENNIS,
    PLANTA_ROSA_CANINA_DURATA_PERENNIS
} PlantaRosaCaninaDurata;

nomen enumeratio {
    PLANTA_ROSA_CANINA_FOLIA_PERMANENTIA_ABSENS = ZEPHYRUM,
    PLANTA_ROSA_CANINA_FOLIA_PERMANENTIA_DECIDUA,
    PLANTA_ROSA_CANINA_FOLIA_PERMANENTIA_SEMPERVIRENS
} PlantaRosaCaninaFoliaPermanentia;

nomen enumeratio {
    PLANTA_ROSA_CANINA_STATUS_VITA_ABSENS = ZEPHYRUM,
    PLANTA_ROSA_CANINA_STATUS_VITA_SEMEN,
    PLANTA_ROSA_CANINA_STATUS_VITA_GERMINANS,
    PLANTA_ROSA_CANINA_STATUS_VITA_IUVENIS,
    PLANTA_ROSA_CANINA_STATUS_VITA_FLORENS,
    PLANTA_ROSA_CANINA_STATUS_VITA_FRUCTIFERA,
    PLANTA_ROSA_CANINA_STATUS_VITA_QUIESCENS,
    PLANTA_ROSA_CANINA_STATUS_VITA_MORTUA
} PlantaRosaCaninaStatusVita;

nomen enumeratio {
    PLANTA_ROSA_CANINA_ANIMA_ABSENS = ZEPHYRUM,
    PLANTA_ROSA_CANINA_ANIMA_VEGETATIVA,
    PLANTA_ROSA_CANINA_ANIMA_SENSITIVA,
    PLANTA_ROSA_CANINA_ANIMA_RATIONALIS
} PlantaRosaCaninaAnima;

nomen enumeratio {
    PLANTA_ROSA_CANINA_STATUS_CONDITIO_ABSENS = ZEPHYRUM,
    PLANTA_ROSA_CANINA_STATUS_CONDITIO_RECENS,
    PLANTA_ROSA_CANINA_STATUS_CONDITIO_VETUS,
    PLANTA_ROSA_CANINA_STATUS_CONDITIO_CORRUPTA
} PlantaRosaCaninaStatusConditio;

nomen enumeratio {
    PLANTA_MALUS_DOMESTICA_HABITUS_ABSENS = ZEPHYRUM,
    PLANTA_MALUS_DOMESTICA_HABITUS_HERBA,
    PLANTA_MALUS_DOMESTICA_HABITUS_FRUTEX,
    PLANTA_MALUS_DOMESTICA_HABITUS_ARBOR,
    PLANTA_MALUS_DOMESTICA_HABITUS_SCANDENS
} PlantaMalusDomesticaHabitus;

nomen enumeratio {
    PLANTA_MALUS_DOMESTICA_DURATA_ABSENS = ZEPHYRUM,
    PLANTA_MALUS_DOMESTICA_DURATA_ANNUA,
    PLANTA_MALUS_DOMESTICA_DURATA_BIENNIS,
    PLANTA_MALUS_DOMESTICA_DURATA_PERENNIS
} PlantaMalusDomesticaDurata;

nomen enumeratio {
    PLANTA_MALUS_DOMESTICA_FOLIA_PERMANENTIA_ABSENS = ZEPHYRUM,
    PLANTA_MALUS_DOMESTICA_FOLIA_PERMANENTIA_DECIDUA,
    PLANTA_MALUS_DOMESTICA_FOLIA_PERMANENTIA_SEMPERVIRENS
} PlantaMalusDomesticaFoliaPermanentia;

nomen enumeratio {
    PLANTA_MALUS_DOMESTICA_STATUS_VITA_ABSENS = ZEPHYRUM,
    PLANTA_MALUS_DOMESTICA_STATUS_VITA_SEMEN,
    PLANTA_MALUS_DOMESTICA_STATUS_VITA_GERMINANS,
    PLANTA_MALUS_DOMESTICA_STATUS_VITA_IUVENIS,
    PLANTA_MALUS_DOMESTICA_STATUS_VITA_FLORENS,
    PLANTA_MALUS_DOMESTICA_STATUS_VITA_FRUCTIFERA,
    PLANTA_MALUS_DOMESTICA_STATUS_VITA_QUIESCENS,
    PLANTA_MALUS_DOMESTICA_STATUS_VITA_MORTUA
} PlantaMalusDomesticaStatusVita;

nomen enumeratio {
    PLANTA_MALUS_DOMESTICA_ANIMA_ABSENS = ZEPHYRUM,
    PLANTA_MALUS_DOMESTICA_ANIMA_VEGETATIVA,
    PLANTA_MALUS_DOMESTICA_ANIMA_SENSITIVA,
    PLANTA_MALUS_DOMESTICA_ANIMA_RATIONALIS
} PlantaMalusDomesticaAnima;

nomen enumeratio {
    PLANTA_MALUS_DOMESTICA_STATUS_CONDITIO_ABSENS = ZEPHYRUM,
    PLANTA_MALUS_DOMESTICA_STATUS_CONDITIO_RECENS,
    PLANTA_MALUS_DOMESTICA_STATUS_CONDITIO_VETUS,
    PLANTA_MALUS_DOMESTICA_STATUS_CONDITIO_CORRUPTA
} PlantaMalusDomesticaStatusConditio;

nomen enumeratio {
    PLANTA_GRANNY_SMITH_HABITUS_ABSENS = ZEPHYRUM,
    PLANTA_GRANNY_SMITH_HABITUS_HERBA,
    PLANTA_GRANNY_SMITH_HABITUS_FRUTEX,
    PLANTA_GRANNY_SMITH_HABITUS_ARBOR,
    PLANTA_GRANNY_SMITH_HABITUS_SCANDENS
} PlantaGrannySmithHabitus;

nomen enumeratio {
    PLANTA_GRANNY_SMITH_DURATA_ABSENS = ZEPHYRUM,
    PLANTA_GRANNY_SMITH_DURATA_ANNUA,
    PLANTA_GRANNY_SMITH_DURATA_BIENNIS,
    PLANTA_GRANNY_SMITH_DURATA_PERENNIS
} PlantaGrannySmithDurata;

nomen enumeratio {
    PLANTA_GRANNY_SMITH_FOLIA_PERMANENTIA_ABSENS = ZEPHYRUM,
    PLANTA_GRANNY_SMITH_FOLIA_PERMANENTIA_DECIDUA,
    PLANTA_GRANNY_SMITH_FOLIA_PERMANENTIA_SEMPERVIRENS
} PlantaGrannySmithFoliaPermanentia;

nomen enumeratio {
    PLANTA_GRANNY_SMITH_STATUS_VITA_ABSENS = ZEPHYRUM,
    PLANTA_GRANNY_SMITH_STATUS_VITA_SEMEN,
    PLANTA_GRANNY_SMITH_STATUS_VITA_GERMINANS,
    PLANTA_GRANNY_SMITH_STATUS_VITA_IUVENIS,
    PLANTA_GRANNY_SMITH_STATUS_VITA_FLORENS,
    PLANTA_GRANNY_SMITH_STATUS_VITA_FRUCTIFERA,
    PLANTA_GRANNY_SMITH_STATUS_VITA_QUIESCENS,
    PLANTA_GRANNY_SMITH_STATUS_VITA_MORTUA
} PlantaGrannySmithStatusVita;

nomen enumeratio {
    PLANTA_GRANNY_SMITH_ANIMA_ABSENS = ZEPHYRUM,
    PLANTA_GRANNY_SMITH_ANIMA_VEGETATIVA,
    PLANTA_GRANNY_SMITH_ANIMA_SENSITIVA,
    PLANTA_GRANNY_SMITH_ANIMA_RATIONALIS
} PlantaGrannySmithAnima;

nomen enumeratio {
    PLANTA_GRANNY_SMITH_STATUS_CONDITIO_ABSENS = ZEPHYRUM,
    PLANTA_GRANNY_SMITH_STATUS_CONDITIO_RECENS,
    PLANTA_GRANNY_SMITH_STATUS_CONDITIO_VETUS,
    PLANTA_GRANNY_SMITH_STATUS_CONDITIO_CORRUPTA
} PlantaGrannySmithStatusConditio;

nomen enumeratio {
    PLANTA_HISTORIA_EVENTUM_ACTIO_ABSENS = ZEPHYRUM,
    PLANTA_HISTORIA_EVENTUM_ACTIO_GERMINARE,
    PLANTA_HISTORIA_EVENTUM_ACTIO_FLORERE,
    PLANTA_HISTORIA_EVENTUM_ACTIO_FRUCTIFICARE,
    PLANTA_HISTORIA_EVENTUM_ACTIO_PROPAGARI,
    PLANTA_HISTORIA_EVENTUM_ACTIO_NOMINARI
} PlantaHistoriaEventumActio;

structura PlantaIndividua {
    StmlNodus*  nodus;
    Xar*        planta;   /* PlantaPlanta* */
    Xar*        rosaceae;   /* PlantaRosaceae* */
    Xar*        rosa;   /* PlantaRosa* */
    Xar*        malus;   /* PlantaMalus* */
    Xar*        rosa_canina;   /* PlantaRosaCanina* */
    Xar*        malus_domestica;   /* PlantaMalusDomestica* */
    Xar*        granny_smith;   /* PlantaGrannySmith* */
};

structura PlantaPlanta {
    StmlNodus*  nodus;
    chorda*     nomen_v;
    PlantaPlantaHabitus habitus;
    PlantaPlantaDurata durata;
    PlantaPlantaFoliaPermanentia folia_permanentia;
    s64         altitudo_matura;
    b32         altitudo_matura_praesens;
    PlantaPlantaStatusVita status_vita;
    chorda*     auctor_nominis;
    chorda*     nomen_publicatum_in;
    chorda*     pollinatur_a;
    PlantaPlantaAnima anima;
    chorda*     massa;
    b32         perituram;
    b32         perituram_praesens;
    PlantaPlantaStatusConditio status_conditio;
    chorda*     sita_in;
    PlantaPlantaRadix* radix;
    PlantaPlantaCaulis* caulis;
    PlantaPlantaFolium* folium;
    PlantaPlantaFlos* flos;
    PlantaPlantaFructus* fructus;
    PlantaPlantaSemen* semen;
    Xar*        generat;   /* PlantaPlantaGenerat* */
    Xar*        laborat;   /* PlantaPlantaLaborat* */
    PlantaPlantaHistoria* historia;
    Xar*        nota;   /* PlantaNota* */
};

structura PlantaPlantaRadix {
    StmlNodus*  nodus;
    chorda*     nota;
};

structura PlantaPlantaCaulis {
    StmlNodus*  nodus;
    chorda*     nota;
};

structura PlantaPlantaFolium {
    StmlNodus*  nodus;
    chorda*     nota;
};

structura PlantaPlantaFlos {
    StmlNodus*  nodus;
    chorda*     nota;
};

structura PlantaPlantaFructus {
    StmlNodus*  nodus;
    chorda*     nota;
};

structura PlantaPlantaSemen {
    StmlNodus*  nodus;
    chorda*     nota;
};

structura PlantaPlantaGenerat {
    StmlNodus*  nodus;
    chorda*     ad;
};

structura PlantaPlantaLaborat {
    StmlNodus*  nodus;
    chorda*     ad;
};

structura PlantaPlantaHistoria {
    StmlNodus*  nodus;
    Xar*        eventum;   /* PlantaHistoriaEventum* */
};

structura PlantaRosaceae {
    StmlNodus*  nodus;
    chorda*     nomen_v;
    PlantaRosaceaeHabitus habitus;
    PlantaRosaceaeDurata durata;
    PlantaRosaceaeFoliaPermanentia folia_permanentia;
    s64         altitudo_matura;
    b32         altitudo_matura_praesens;
    PlantaRosaceaeStatusVita status_vita;
    chorda*     auctor_nominis;
    chorda*     nomen_publicatum_in;
    chorda*     pollinatur_a;
    PlantaRosaceaeAnima anima;
    chorda*     massa;
    b32         perituram;
    b32         perituram_praesens;
    PlantaRosaceaeStatusConditio status_conditio;
    chorda*     sita_in;
    PlantaRosaceaeRadix* radix;
    PlantaRosaceaeCaulis* caulis;
    PlantaRosaceaeFolium* folium;
    PlantaRosaceaeFlos* flos;
    PlantaRosaceaeFructus* fructus;
    PlantaRosaceaeSemen* semen;
    Xar*        generat;   /* PlantaRosaceaeGenerat* */
    Xar*        laborat;   /* PlantaRosaceaeLaborat* */
    PlantaRosaceaeHistoria* historia;
    Xar*        nota;   /* PlantaNota* */
};

structura PlantaRosaceaeRadix {
    StmlNodus*  nodus;
    chorda*     nota;
};

structura PlantaRosaceaeCaulis {
    StmlNodus*  nodus;
    chorda*     nota;
};

structura PlantaRosaceaeFolium {
    StmlNodus*  nodus;
    chorda*     nota;
};

structura PlantaRosaceaeFlos {
    StmlNodus*  nodus;
    chorda*     nota;
};

structura PlantaRosaceaeFructus {
    StmlNodus*  nodus;
    chorda*     nota;
};

structura PlantaRosaceaeSemen {
    StmlNodus*  nodus;
    chorda*     nota;
};

structura PlantaRosaceaeGenerat {
    StmlNodus*  nodus;
    chorda*     ad;
};

structura PlantaRosaceaeLaborat {
    StmlNodus*  nodus;
    chorda*     ad;
};

structura PlantaRosaceaeHistoria {
    StmlNodus*  nodus;
    Xar*        eventum;   /* PlantaHistoriaEventum* */
};

structura PlantaRosa {
    StmlNodus*  nodus;
    chorda*     nomen_v;
    PlantaRosaHabitus habitus;
    PlantaRosaDurata durata;
    PlantaRosaFoliaPermanentia folia_permanentia;
    s64         altitudo_matura;
    b32         altitudo_matura_praesens;
    PlantaRosaStatusVita status_vita;
    chorda*     auctor_nominis;
    chorda*     nomen_publicatum_in;
    chorda*     pollinatur_a;
    PlantaRosaAnima anima;
    chorda*     massa;
    b32         perituram;
    b32         perituram_praesens;
    PlantaRosaStatusConditio status_conditio;
    chorda*     sita_in;
    PlantaRosaRadix* radix;
    PlantaRosaCaulis* caulis;
    PlantaRosaFolium* folium;
    PlantaRosaFlos* flos;
    PlantaRosaFructus* fructus;
    PlantaRosaSemen* semen;
    Xar*        generat;   /* PlantaRosaGenerat* */
    Xar*        laborat;   /* PlantaRosaLaborat* */
    PlantaRosaHistoria* historia;
    Xar*        nota;   /* PlantaNota* */
};

structura PlantaRosaRadix {
    StmlNodus*  nodus;
    chorda*     nota;
};

structura PlantaRosaCaulis {
    StmlNodus*  nodus;
    chorda*     nota;
};

structura PlantaRosaFolium {
    StmlNodus*  nodus;
    chorda*     nota;
};

structura PlantaRosaFlos {
    StmlNodus*  nodus;
    chorda*     nota;
};

structura PlantaRosaFructus {
    StmlNodus*  nodus;
    chorda*     nota;
};

structura PlantaRosaSemen {
    StmlNodus*  nodus;
    chorda*     nota;
};

structura PlantaRosaGenerat {
    StmlNodus*  nodus;
    chorda*     ad;
};

structura PlantaRosaLaborat {
    StmlNodus*  nodus;
    chorda*     ad;
};

structura PlantaRosaHistoria {
    StmlNodus*  nodus;
    Xar*        eventum;   /* PlantaHistoriaEventum* */
};

structura PlantaMalus {
    StmlNodus*  nodus;
    chorda*     nomen_v;
    PlantaMalusHabitus habitus;
    PlantaMalusDurata durata;
    PlantaMalusFoliaPermanentia folia_permanentia;
    s64         altitudo_matura;
    b32         altitudo_matura_praesens;
    PlantaMalusStatusVita status_vita;
    chorda*     auctor_nominis;
    chorda*     nomen_publicatum_in;
    chorda*     pollinatur_a;
    PlantaMalusAnima anima;
    chorda*     massa;
    b32         perituram;
    b32         perituram_praesens;
    PlantaMalusStatusConditio status_conditio;
    chorda*     sita_in;
    PlantaMalusRadix* radix;
    PlantaMalusCaulis* caulis;
    PlantaMalusFolium* folium;
    PlantaMalusFlos* flos;
    PlantaMalusFructus* fructus;
    PlantaMalusSemen* semen;
    Xar*        generat;   /* PlantaMalusGenerat* */
    Xar*        laborat;   /* PlantaMalusLaborat* */
    PlantaMalusHistoria* historia;
    Xar*        nota;   /* PlantaNota* */
};

structura PlantaMalusRadix {
    StmlNodus*  nodus;
    chorda*     nota;
};

structura PlantaMalusCaulis {
    StmlNodus*  nodus;
    chorda*     nota;
};

structura PlantaMalusFolium {
    StmlNodus*  nodus;
    chorda*     nota;
};

structura PlantaMalusFlos {
    StmlNodus*  nodus;
    chorda*     nota;
};

structura PlantaMalusFructus {
    StmlNodus*  nodus;
    chorda*     nota;
};

structura PlantaMalusSemen {
    StmlNodus*  nodus;
    chorda*     nota;
};

structura PlantaMalusGenerat {
    StmlNodus*  nodus;
    chorda*     ad;
};

structura PlantaMalusLaborat {
    StmlNodus*  nodus;
    chorda*     ad;
};

structura PlantaMalusHistoria {
    StmlNodus*  nodus;
    Xar*        eventum;   /* PlantaHistoriaEventum* */
};

structura PlantaRosaCanina {
    StmlNodus*  nodus;
    chorda*     nomen_v;
    PlantaRosaCaninaHabitus habitus;
    PlantaRosaCaninaDurata durata;
    PlantaRosaCaninaFoliaPermanentia folia_permanentia;
    s64         altitudo_matura;
    b32         altitudo_matura_praesens;
    PlantaRosaCaninaStatusVita status_vita;
    chorda*     auctor_nominis;
    chorda*     nomen_publicatum_in;
    chorda*     pollinatur_a;
    PlantaRosaCaninaAnima anima;
    chorda*     massa;
    b32         perituram;
    b32         perituram_praesens;
    PlantaRosaCaninaStatusConditio status_conditio;
    chorda*     sita_in;
    PlantaRosaCaninaRadix* radix;
    PlantaRosaCaninaCaulis* caulis;
    PlantaRosaCaninaFolium* folium;
    PlantaRosaCaninaFlos* flos;
    PlantaRosaCaninaFructus* fructus;
    PlantaRosaCaninaSemen* semen;
    Xar*        generat;   /* PlantaRosaCaninaGenerat* */
    Xar*        laborat;   /* PlantaRosaCaninaLaborat* */
    PlantaRosaCaninaHistoria* historia;
    Xar*        nota;   /* PlantaNota* */
};

structura PlantaRosaCaninaRadix {
    StmlNodus*  nodus;
    chorda*     nota;
};

structura PlantaRosaCaninaCaulis {
    StmlNodus*  nodus;
    chorda*     nota;
};

structura PlantaRosaCaninaFolium {
    StmlNodus*  nodus;
    chorda*     nota;
};

structura PlantaRosaCaninaFlos {
    StmlNodus*  nodus;
    chorda*     nota;
};

structura PlantaRosaCaninaFructus {
    StmlNodus*  nodus;
    chorda*     nota;
};

structura PlantaRosaCaninaSemen {
    StmlNodus*  nodus;
    chorda*     nota;
};

structura PlantaRosaCaninaGenerat {
    StmlNodus*  nodus;
    chorda*     ad;
};

structura PlantaRosaCaninaLaborat {
    StmlNodus*  nodus;
    chorda*     ad;
};

structura PlantaRosaCaninaHistoria {
    StmlNodus*  nodus;
    Xar*        eventum;   /* PlantaHistoriaEventum* */
};

structura PlantaMalusDomestica {
    StmlNodus*  nodus;
    chorda*     nomen_v;
    PlantaMalusDomesticaHabitus habitus;
    PlantaMalusDomesticaDurata durata;
    PlantaMalusDomesticaFoliaPermanentia folia_permanentia;
    s64         altitudo_matura;
    b32         altitudo_matura_praesens;
    PlantaMalusDomesticaStatusVita status_vita;
    chorda*     auctor_nominis;
    chorda*     nomen_publicatum_in;
    chorda*     pollinatur_a;
    PlantaMalusDomesticaAnima anima;
    chorda*     massa;
    b32         perituram;
    b32         perituram_praesens;
    PlantaMalusDomesticaStatusConditio status_conditio;
    chorda*     sita_in;
    PlantaMalusDomesticaRadix* radix;
    PlantaMalusDomesticaCaulis* caulis;
    PlantaMalusDomesticaFolium* folium;
    PlantaMalusDomesticaFlos* flos;
    PlantaMalusDomesticaFructus* fructus;
    PlantaMalusDomesticaSemen* semen;
    Xar*        generat;   /* PlantaMalusDomesticaGenerat* */
    Xar*        laborat;   /* PlantaMalusDomesticaLaborat* */
    PlantaMalusDomesticaHistoria* historia;
    Xar*        nota;   /* PlantaNota* */
};

structura PlantaMalusDomesticaRadix {
    StmlNodus*  nodus;
    chorda*     nota;
};

structura PlantaMalusDomesticaCaulis {
    StmlNodus*  nodus;
    chorda*     nota;
};

structura PlantaMalusDomesticaFolium {
    StmlNodus*  nodus;
    chorda*     nota;
};

structura PlantaMalusDomesticaFlos {
    StmlNodus*  nodus;
    chorda*     nota;
};

structura PlantaMalusDomesticaFructus {
    StmlNodus*  nodus;
    chorda*     nota;
};

structura PlantaMalusDomesticaSemen {
    StmlNodus*  nodus;
    chorda*     nota;
};

structura PlantaMalusDomesticaGenerat {
    StmlNodus*  nodus;
    chorda*     ad;
};

structura PlantaMalusDomesticaLaborat {
    StmlNodus*  nodus;
    chorda*     ad;
};

structura PlantaMalusDomesticaHistoria {
    StmlNodus*  nodus;
    Xar*        eventum;   /* PlantaHistoriaEventum* */
};

structura PlantaGrannySmith {
    StmlNodus*  nodus;
    chorda*     nomen_v;
    PlantaGrannySmithHabitus habitus;
    PlantaGrannySmithDurata durata;
    PlantaGrannySmithFoliaPermanentia folia_permanentia;
    s64         altitudo_matura;
    b32         altitudo_matura_praesens;
    PlantaGrannySmithStatusVita status_vita;
    chorda*     auctor_nominis;
    chorda*     nomen_publicatum_in;
    chorda*     pollinatur_a;
    PlantaGrannySmithAnima anima;
    chorda*     massa;
    b32         perituram;
    b32         perituram_praesens;
    PlantaGrannySmithStatusConditio status_conditio;
    chorda*     sita_in;
    PlantaGrannySmithRadix* radix;
    PlantaGrannySmithCaulis* caulis;
    PlantaGrannySmithFolium* folium;
    PlantaGrannySmithFlos* flos;
    PlantaGrannySmithFructus* fructus;
    PlantaGrannySmithSemen* semen;
    Xar*        generat;   /* PlantaGrannySmithGenerat* */
    Xar*        laborat;   /* PlantaGrannySmithLaborat* */
    PlantaGrannySmithHistoria* historia;
    Xar*        nota;   /* PlantaNota* */
};

structura PlantaGrannySmithRadix {
    StmlNodus*  nodus;
    chorda*     nota;
};

structura PlantaGrannySmithCaulis {
    StmlNodus*  nodus;
    chorda*     nota;
};

structura PlantaGrannySmithFolium {
    StmlNodus*  nodus;
    chorda*     nota;
};

structura PlantaGrannySmithFlos {
    StmlNodus*  nodus;
    chorda*     nota;
};

structura PlantaGrannySmithFructus {
    StmlNodus*  nodus;
    chorda*     nota;
};

structura PlantaGrannySmithSemen {
    StmlNodus*  nodus;
    chorda*     nota;
};

structura PlantaGrannySmithGenerat {
    StmlNodus*  nodus;
    chorda*     ad;
};

structura PlantaGrannySmithLaborat {
    StmlNodus*  nodus;
    chorda*     ad;
};

structura PlantaGrannySmithHistoria {
    StmlNodus*  nodus;
    Xar*        eventum;   /* PlantaHistoriaEventum* */
};

structura PlantaNota {
    StmlNodus*  nodus;
    chorda      textus;
};

structura PlantaHistoriaEventum {
    StmlNodus*  nodus;
    chorda*     quando;
    PlantaHistoriaEventumActio actio;
    chorda*     certitudo;
    chorda*     fons;
    chorda*     nota;
    chorda      textus;
};

PlantaIndividua* planta_individua_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaIndividua* planta_individua_legere(
    chorda fons, Piscina* piscina,
    InternamentumChorda* intern, chorda* causa);
PlantaPlanta* planta_planta_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaPlantaRadix* planta_planta_radix_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaPlantaCaulis* planta_planta_caulis_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaPlantaFolium* planta_planta_folium_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaPlantaFlos* planta_planta_flos_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaPlantaFructus* planta_planta_fructus_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaPlantaSemen* planta_planta_semen_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaPlantaGenerat* planta_planta_generat_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaPlantaLaborat* planta_planta_laborat_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaPlantaHistoria* planta_planta_historia_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaRosaceae* planta_rosaceae_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaRosaceaeRadix* planta_rosaceae_radix_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaRosaceaeCaulis* planta_rosaceae_caulis_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaRosaceaeFolium* planta_rosaceae_folium_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaRosaceaeFlos* planta_rosaceae_flos_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaRosaceaeFructus* planta_rosaceae_fructus_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaRosaceaeSemen* planta_rosaceae_semen_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaRosaceaeGenerat* planta_rosaceae_generat_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaRosaceaeLaborat* planta_rosaceae_laborat_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaRosaceaeHistoria* planta_rosaceae_historia_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaRosa* planta_rosa_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaRosaRadix* planta_rosa_radix_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaRosaCaulis* planta_rosa_caulis_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaRosaFolium* planta_rosa_folium_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaRosaFlos* planta_rosa_flos_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaRosaFructus* planta_rosa_fructus_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaRosaSemen* planta_rosa_semen_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaRosaGenerat* planta_rosa_generat_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaRosaLaborat* planta_rosa_laborat_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaRosaHistoria* planta_rosa_historia_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaMalus* planta_malus_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaMalusRadix* planta_malus_radix_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaMalusCaulis* planta_malus_caulis_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaMalusFolium* planta_malus_folium_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaMalusFlos* planta_malus_flos_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaMalusFructus* planta_malus_fructus_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaMalusSemen* planta_malus_semen_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaMalusGenerat* planta_malus_generat_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaMalusLaborat* planta_malus_laborat_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaMalusHistoria* planta_malus_historia_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaRosaCanina* planta_rosa_canina_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaRosaCaninaRadix* planta_rosa_canina_radix_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaRosaCaninaCaulis* planta_rosa_canina_caulis_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaRosaCaninaFolium* planta_rosa_canina_folium_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaRosaCaninaFlos* planta_rosa_canina_flos_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaRosaCaninaFructus* planta_rosa_canina_fructus_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaRosaCaninaSemen* planta_rosa_canina_semen_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaRosaCaninaGenerat* planta_rosa_canina_generat_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaRosaCaninaLaborat* planta_rosa_canina_laborat_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaRosaCaninaHistoria* planta_rosa_canina_historia_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaMalusDomestica* planta_malus_domestica_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaMalusDomesticaRadix* planta_malus_domestica_radix_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaMalusDomesticaCaulis* planta_malus_domestica_caulis_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaMalusDomesticaFolium* planta_malus_domestica_folium_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaMalusDomesticaFlos* planta_malus_domestica_flos_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaMalusDomesticaFructus* planta_malus_domestica_fructus_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaMalusDomesticaSemen* planta_malus_domestica_semen_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaMalusDomesticaGenerat* planta_malus_domestica_generat_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaMalusDomesticaLaborat* planta_malus_domestica_laborat_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaMalusDomesticaHistoria* planta_malus_domestica_historia_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaGrannySmith* planta_granny_smith_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaGrannySmithRadix* planta_granny_smith_radix_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaGrannySmithCaulis* planta_granny_smith_caulis_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaGrannySmithFolium* planta_granny_smith_folium_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaGrannySmithFlos* planta_granny_smith_flos_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaGrannySmithFructus* planta_granny_smith_fructus_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaGrannySmithSemen* planta_granny_smith_semen_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaGrannySmithGenerat* planta_granny_smith_generat_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaGrannySmithLaborat* planta_granny_smith_laborat_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaGrannySmithHistoria* planta_granny_smith_historia_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaNota* planta_nota_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);
PlantaHistoriaEventum* planta_historia_eventum_legere_nodum(
    StmlNodus* nodus, Piscina* piscina,
    InternamentumChorda* intern);

#endif /* PLANTA_LECTIO_H */
