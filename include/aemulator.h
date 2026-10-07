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

/* color sine valore proprio (color_cursoris: litteras sequitur) */
#define AEMULATOR_COLOR_NULLUS  0xFFFFFFFF

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
    /* COLORES (D5, decisio XXVIII): 0xRRGGBB quos OSC 10/11/12 et 4
     * respondent et quos programma mutare potest (OSC 4/10/11/12);
     * restitutio (104/110/111/112) ad hos valores redit.
     * tabula_colorum: CCLVI colores (in creatione copiantur) aut NIHIL
     * = tabula xterm ordinaria (0-15 xterm, 16-231 cubus, 232-255
     * gradus grisei).
     * color_cursoris AEMULATOR_COLOR_NULLUS = colorem litterarum
     * currentem sequitur (Ghostty: cursor sine colore proprio). */
                  i32  color_litterae;
                  i32  color_fundi;
                  i32  color_cursoris;
         constans i32* tabula_colorum;
} AemulatorConfiguratio;

/* Configuratio ordinaria: LXXX x XXIV, effectus nulli, titulus
 * "aemulator", versio AEMULATOR_VERSIO, lectio_schirmi FALSUM,
 * historia_octeti X MB, color_litterae 0xFFFFFF, color_fundi 0x000000,
 * color_cursoris AEMULATOR_COLOR_NULLUS, tabula_colorum NIHIL.
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

/* modus ANSI (privatus FALSUM) aut DEC privatus (VERUM), bitum crudum
 * ut DECRQM nuntiat (D2): ANSI IV XX; DEC I VI VII IX XXV XLV XLVII
 * LXVI M-MVII MXV MXVI MXLV MXLVII-MXLIX MMIV MMXXVI. Ignotus =
 * FALSUM. */
b32
aemulator_modus (
    constans Aemulator* a,
                   i32  numerus,
                   b32  privatus);

/* MUS - eventa muris quae programma petivit (DECSET 9 / 1000 / 1002 /
 * 1003). Ut Ghostty: positum ultimum vincit; quodlibet eorum remotum
 * = NULLUS (etiam si alius manet positus). */
nomen enumeratio {
    AEMULATOR_MUS_NULLUS = ZEPHYRUM,
    AEMULATOR_MUS_X10,          /* ?9: pressio sola, sine modis */
    AEMULATOR_MUS_PRESSIO,      /* ?1000: pressio + solutio */
    AEMULATOR_MUS_TRACTUS,      /* ?1002: + motus cum bottone */
    AEMULATOR_MUS_OMNIS         /* ?1003: + omnis motus */
} AemulatorMus;

/* forma relationis muris (?1005 / 1006 / 1015 / 1016); remota = X10 */
nomen enumeratio {
    AEMULATOR_MUS_FORMA_X10 = ZEPHYRUM,   /* octeti 32 + valor */
    AEMULATOR_MUS_FORMA_UTF8,             /* ?1005 */
    AEMULATOR_MUS_FORMA_SGR,              /* ?1006 */
    AEMULATOR_MUS_FORMA_URXVT,            /* ?1015 */
    AEMULATOR_MUS_FORMA_SGR_PIXELA        /* ?1016 */
} AemulatorMusForma;

/* MODI INITUS - quod hospes legit ut claves, murem, glutinum, focum
 * codificet (D6: in CodificatorModi transferuntur). Instantanea per
 * valorem; post aemulator_scribere iterum legenda. Campi postea
 * addendi (D4: kitty_vexilla) in fine. Relatio foci statim post ?1004
 * positum (Ghostty) res hospitis est: modum mutatum videt. */
nomen structura {
                 b32 sagittae_applicationis; /* DECCKM ?1: SS3 A */
                 b32 tabula_applicationis;   /* DECKPAM ESC = / ?66 */
        AemulatorMus mus;
   AemulatorMusForma mus_forma;
                 b32 glutinum;               /* ?2004 bracketed paste */
                 b32 focus;                  /* ?1004 */
                 b32 lnm;                    /* LNM 20: Enter = CR LF */
                 i32 kitty_vexilla;          /* D4: protocollum clavium
                                              * kitty, schirmi activi
                                              * (0-31; 0 = legacy) */
                 b32 rotula_sagittis;        /* D6b: ?1007 (ordinarie
                                              * VERUM): rotula in
                                              * schirmo altero sine
                                              * mure = sagittae */
} AemulatorModi;

AemulatorModi
aemulator_modi (
    constans Aemulator* a);

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


/* ==================================================
 * Colores (phasis D5)
 * ================================================== */

/* indices aemulator_color praeter tabulam 0-CCLV */
#define AEMULATOR_COLOR_LITTERAE  CCLVI
#define AEMULATOR_COLOR_FUNDI     CCLVII
#define AEMULATOR_COLOR_CURSORIS  CCLVIII

/* color vivus 0xRRGGBB post OSC 4/10/11/12 et 104/110/111/112 (quod
 * hospes pingit; D6); cursor sine proprio = litterae currentes. Index
 * extra 0-CCLVIII: 0. */
i32
aemulator_color (
    constans Aemulator* a,
                   i32  index);

#endif /* AEMULATOR_H */
