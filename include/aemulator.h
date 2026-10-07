/* aemulator.h - nucleus emulatoris terminalis (aemulator-plan A1)
 *
 * Octeti programmatis intrant (aemulator_scribere), status schirmi
 * exit (cellulae, cursor, modi), responsa et eventus per EFFECTUS
 * (tabula functionum) exeunt. PURUS (decisio X): nulla I/O, nullum
 * tempus, nulla fila, nulla static mutabilis, memoria tota ex piscina
 * vocantis - wasm-abilis per constructionem.
 *
 * Speculum tesserae (research/app-vs-emulator-inversion): tessera
 * parce emittit, aemulator LIBERALITER accipit - series ignota
 * consumitur et numeratur, numquam fatalis; omnis magnitudo quam
 * programma regit limitata.
 *
 * Partes nostrae: series_terminalis (lexemata), stilus_terminalis
 * (SGR), runae (latitudo, graphemata). Exemplar: Ghostty
 * src/terminal @ 12752b2 (MIT).
 *
 * MEMORIA: capacitas schirmi in creatione; mutatio magnitudinis intra
 * capacitatem nihil allocat, ultra eam capacitatem GEOMETRICE auget
 * (numquam minuit). Status constans: nihil allocat.
 *
 * VISUS: chordae redditae (graphema cellulae) valent usque ad
 * vocationem mutantem proximam (scribere, amplitudo).
 */

#ifndef AEMULATOR_H
#define AEMULATOR_H

/* <aedilis nexus="purus"/> - clausura sine regula nexus (decisio X) */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "stilus_terminalis.h"

/* versio quam XTVERSION nuntiat (decisio VIII) */
#define AEMULATOR_VERSIO "0.1"

nomen structura Aemulator Aemulator;

/* EFFECTUS - omnis exitus nuclei praeter statum. Campus NIHIL =
 * ignoratum. Synchroni; aemulatorem intra vocationem non reintrare. */
nomen structura {
    vacuum* datum;
    /* responsa quaestionum (DA, DSR, XTVERSION) ad programma */
    vacuum (*responsum) (vacuum* datum, constans i8* octeti, i32 n);
    /* BEL */
    vacuum (*campana)   (vacuum* datum);
    /* OSC 0/2 (etiam vacuus); chorda valet in vocatione sola */
    vacuum (*titulus)   (vacuum* datum, chorda titulus);
} AemulatorEffectus;

nomen structura {
                  i32 latitudo;     /* cellulae, >= I */
                  i32 altitudo;
    AemulatorEffectus effectus;
    /* identitas (B2): XTVERSION respondet 'titulus versio'; DA1/DA2/
     * DA3 fixa ut Ghostty (VT220, colores ANSI). In creatione
     * copiantur. */
  constans character* titulus;
  constans character* versio;
    /* DECRQCRA (B4b, decisio XVIII): programma summam cellularum
     * rectanguli legere potest. Ordinarie FALSUM (decisio IX: Ghostty
     * eam non habet, iTerm2 clausam praebet); probationes (esctest)
     * aperiunt. Clausa: series ignota, nullum responsum. */
                  b32 lectio_schirmi;
    /* HISTORIA (phasis C, decisiones XIX-XX): limes historiae schirmi
     * primarii in octetis, ad paginas integras sursum rotundatus
     * (pagina una saltem). Paginae ex piscina crescunt usque ad
     * limitem, deinde vetustissima recyclatur - post id nihil
     * allocatur. 0 = nulla historia. Ordinarius X MB. */
                  i32 historia_octeti;
} AemulatorConfiguratio;

/* Configuratio ordinaria: LXXX x XXIV, effectus nulli, titulus
 * "aemulator", versio AEMULATOR_VERSIO, lectio_schirmi FALSUM,
 * historia_octeti X MB.
 * Campi postea addendi hic
 * defaltas accipiunt - vocantes semper ab hac incipiant. */
vacuum
aemulator_configuratio_initiare (
    AemulatorConfiguratio* cfg);

/* NIHIL si magnitudo mala aut piscina deficit */
Aemulator*
aemulator_creare (
                           Piscina* piscina,
    constans AemulatorConfiguratio* cfg);

/* Octetos programmatis consumere. Status inter vocationes servatur:
 * series et runae UTF-8 scissae licent. */
vacuum
aemulator_scribere (
       Aemulator* a,
     constans i8* octeti,
             i32  n);

/* Mutatio magnitudinis (phasis A: praecidere aut implere; refluxus
 * dilatus - decisio VII). FALSUM si magnitudo mala aut piscina
 * deficit (status priori integer). */
b32
aemulator_amplitudo (
    Aemulator* a,
          i32  latitudo,
          i32  altitudo);


/* ==================================================
 * Lectio
 * ================================================== */

i32
aemulator_latitudo (
    constans Aemulator* a);

i32
aemulator_altitudo (
    constans Aemulator* a);

nomen structura {
    i32 x;              /* 0-based, in schirmo activo */
    i32 y;
    b32 pendens;        /* involutio pendens (columna ultima scripta) */
    b32 visibilis;      /* DECTCEM */
} AemulatorCursor;

AemulatorCursor
aemulator_cursor (
    constans Aemulator* a);

nomen enumeratio {
    AEMULATOR_ANGUSTA = ZEPHYRUM,  /* cellula una */
    AEMULATOR_LATA,                /* graphema latum, cauda sequitur */
    AEMULATOR_CAUDA,               /* post latam: ne pingatur */
    AEMULATOR_CAPUT                /* lata in lineam proximam fluxit */
} AemulatorLatitudo;

nomen structura {
               chorda graphema;   /* UTF-8; vacua = numquam scripta */
    AemulatorLatitudo latitudo;
     StilusTerminalis stilus;
} AemulatorCellula;

/* FALSUM si (x, y) extra schirmum activum */
b32
aemulator_cellula (
    constans Aemulator* a,
                   i32  x,
                   i32  y,
      AemulatorCellula* cellula);

/* schirmum alterum (1049) activum */
b32
aemulator_alterum (
    constans Aemulator* a);

/* modus ANSI (privatus FALSUM) aut DEC privatus (VERUM); ignotus =
 * FALSUM */
b32
aemulator_modus (
    constans Aemulator* a,
                   i32  numerus,
                   b32  privatus);

/* series ignotae consumptae ab creatione */
i32
aemulator_ignota (
    constans Aemulator* a);

/* Effusio plana schirmi activi, ut Ghostty plainString: cellulae
 * numquam scriptae ante textum = spatia, spatia scripta manent,
 * caudae omittuntur, lineae vacuae finales absunt, lineae per '\n'. */
chorda
aemulator_textum_effundere (
    constans Aemulator* a,
               Piscina* piscina);


/* ==================================================
 * Historia et visus (phasis C)
 *
 * Lineae quae schirmum primarium per volutionem TOTIUS schirmi
 * relinquunt (LF/IND in imo, SU) in historiam intrant; volutio
 * regionis, IL/DL et schirmum alterum numquam. VISUS = fenestra
 * altitudinis lineas quam facies ostendit: 0 = imum (schirmum vivum),
 * n = n lineis supra. Sub effusione visus MANET (decisio XXI: eaedem
 * lineae ostenduntur, visus crescit); linea ostensa evicta = visus ad
 * historiam vetustissimam praeciditur. Functiones superiores
 * (cellula, cursor, textum_effundere) schirmum VIVUM legunt, ut
 * programma - visus solum faciei est.
 * ================================================== */

/* lineae in historia (schirmo altero activo: 0 - nullam habet) */
i32
aemulator_historia (
    constans Aemulator* a);

/* positio visus: lineae supra schirmum vivum (0 = imum) */
i32
aemulator_visus (
    constans Aemulator* a);

/* visum movere: delta > 0 sursum (in historiam), < 0 deorsum;
 * praeciditur ad [0, historia]. Status emulatoris non mutatur. */
vacuum
aemulator_visum_movere (
    Aemulator* a,
          s32  delta);

/* cellula visus: y in [0, altitudo) lineae visus; visus 0 = idem ac
 * aemulator_cellula */
b32
aemulator_visus_cellula (
    constans Aemulator* a,
                   i32  x,
                   i32  y,
      AemulatorCellula* cellula);

/* linea visus y in lineam proximam involvitur (volutio mollis -
 * selectio et refluxus futuri) */
b32
aemulator_visus_involuta (
    constans Aemulator* a,
                   i32  y);

/* effusiones planae ut aemulator_textum_effundere (Ghostty
 * plainString) super visum et super historiam + schirmum vivum
 * (Ghostty dumpString .viewport / .screen) */
chorda
aemulator_visum_effundere (
    constans Aemulator* a,
               Piscina* piscina);

chorda
aemulator_historiam_effundere (
    constans Aemulator* a,
               Piscina* piscina);

#endif /* AEMULATOR_H */
