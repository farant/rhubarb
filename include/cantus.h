/* cantus.h - Cantilena ex textu: notae in tempore (dialectus MML)
 *
 * Pars PRIMA bibliothecae cantus (spec sonus §V: 'notes in time ->
 * iussa'): textus brevis -> notae cum altitudine (MIDI), initio et
 * longitudine in QUADRIS (SONUS_FREQUENTIA); deinde notae -> iussa
 * mixturae (sectio IUSSA infra). Purus: nihil sonat, nihil iubet.
 *
 * DIALECTUS - progenies MML ('Music Macro Language': PLAY in BASIC
 * MCMLXXXI, chiptune Iaponica); litterae magnae et parvae aequae,
 * spatia ignorantur; '|' (linea mensurae, ad legendum) sonum non mutat
 * sed SERVATUR (Cantilena.lineae: ubi auctor eam posuit - censura
 * lineas scriptas cum mensuris carminis comparat):
 *
 *   cantilena := { signum }
 *   signum    := nota | pausa | mandatum | ' ' | '|'
 *   nota      := c|d|e|f|g|a|b  [ '#' | '+' | '-' ]  [ mensura ]
 *   pausa     := r  [ mensura ]
 *   mensura   := longitudo { '.' }        1 2 4 8 16 32 64; '.' = x1.5,
 *                                         '..' = x1.75 (ad II puncta)
 *   mandatum  := o N      octava 0..8     (ordinarium 4; o4 c = MIDI LX,
 *                                          C medium)
 *              | > | <    octava sursum / deorsum
 *              | l mensura                longitudo ordinaria (ordinarium 4)
 *              | t N      tempus, quartae per minutum 20..400 (ord. 120)
 *              | v N      lucrum 0..100 centesimae (ordinarium 100)
 *              | @titulus praeparatum sonitus pro notis sequentibus
 *                         ([A-Za-z0-9_]+ ut scriptus, spatio
 *                          terminatum; vocans
 *                          resolvit - lector nomina non novit)
 *
 *   "t180 o6 l16 e g# b >e8"     - arpeggium E maior sursum
 *   "@campana t96 o5 g4. e8 | c2" - tres notae cum praeparato
 *
 * TEMPUS SINE ERRORE CUMULATO: positio in pulsibus
 * (CANTUS_PULSUS_QUARTAE = CMLX per quartam) numeratur; quadrum =
 * rotundatio positionis EXACTAE intra segmentum temporis - mille notae
 * sedecimae non labuntur. Mutatio 't' segmentum novum incipit.
 *
 * NOTA = longitudo SCRIPTA; quantum sonet (staccato, remissio)
 * color vocantis decernit, non lector.
 *
 * VITIA: lector FALSUM reddit cum offset octeti in textu et causa
 * Latina ('longitudo 3 non est potentia II') - nihil semi-lectum.
 * Nota extra MIDI 0..127 vitium est.
 *
 * NONDUM (in PULL): ligaturae '&', triolae, 'q' (pars sonans),
 * syllabae Guidonis (ut re mi ...), voces plures simul.
 *
 * USUS:
 *   Cantilena    c;
 *   CantusVitium v;
 *   si (!cantus_legere(chorda_ex_literis("t180 o6 l16 e g# b >e8"),
 *                      piscina, &c, &v))
 *       imprimere("%d: %s\n", (integer)v.offset, v.causa);
 *   per (i = ZEPHYRUM; i < c.numerus; i++)
 *       ... c.notae[i].nota, .initium, .longitudo ...
 */
/* <aedilis corpus="lib/cantus.c"/> */
#ifndef CANTUS_H
#define CANTUS_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "mixtura.h"

/* pulsus per quartam: CMLX (DAW, MIDI, MusicXML) - dividitur per III,
 * V, VI, XV (quintolae possibiles; olim DCCLXVIII per integram). Tempus
 * MUSICUM in pulsibus est (sonus-spec VIII-d): quadra solum in sonando
 * ex mappa temporis */
#define CANTUS_PULSUS_QUARTAE  CMLX
#define CANTUS_PULSUS_INTEGRAE MMMDCCCXL

/* MAPPA TEMPORIS: segmentum quodque a pulsu suo, tempus (quartae per
 * minutum); quadrum = initium segmenti in quadris (DERIVATUM, per
 * cantus_tempora_computare - ut lector: segmentum claudit ad quadrum
 * suum rotundatum, ergo nulla aberratio cumulata) */
nomen structura {
    s64 pulsus;     /* initium segmenti, pulsus absoluti */
    i32 tempus;     /* 20..400 */
    s64 quadrum;    /* initium segmenti, quadra (derivatum) */
} CantusTempus;

nomen structura {
            i32 nota;           /* MIDI 0..127 (LX = C4) */
            s64 initium;        /* quadrum ab initio cantilenae */
            s64 longitudo;      /* quadra scripta (> 0) */
            s64 pulsus;         /* positio, pulsus ab initio */
            s64 pulsus_longitudo;  /* longitudo scripta, pulsus (> 0) */
      character littera;        /* 'a'..'g' ut scripta (orthographia:
                                 * c# = C sharp, d- = D flat) */
            f32 lucrum;         /* 'v' / C : 0..1 */
         chorda praeparatum;    /* '@titulus' (scissura textus) aut
                                 * vacua = ordinarium vocantis */
            i32 offset;         /* octetus notae in textu */
} CantusNota;

/* linea mensurae scripta ('|') */
nomen structura {
    s64 pulsus;   /* ab initio cantilenae */
    i32 offset;   /* octetus '|' in textu */
} CantusLinea;

nomen structura {
      CantusNota* notae;       /* e piscina, ordine initii */
             i32  numerus;
             s64  longitudo;   /* quadra totius, pausa ultima inclusa */
             s64  pulsus;      /* pulsus totius, pausa ultima inclusa */
    CantusTempus* tempora;     /* mutationes 't' ordine (e piscina);
                                * prima ad pulsum 0 - CXX si textus 't'
                                * initio caret */
             i32  numerus_temporum;
     CantusLinea* lineae;      /* ordine textus (e piscina); NIHIL si
                                * nullae */
             i32 numerus_linearum;
} Cantilena;

nomen structura {
                    i32  offset;   /* octetus ubi lector desiit */
     constans character* causa;    /* sententia Latina, statica */
} CantusVitium;

/* textum totum legit. VERUM: cantilena plena (textus vacuus = nullae
 * notae). FALSUM: vitium impletum, cantilena nulla (piscina fortasse
 * partim usa). Notae et praeparata textum NON copiant: scissurae
 * eius sunt - textus vivat dum notae vivunt */
b32
cantus_legere (
          chorda  textus,
         Piscina* piscina,
       Cantilena* cantilena,
    CantusVitium* vitium);

/* status lectoris post praefixum textus [0, offset): quod signum
 * proximum invenit. Emendatio (emendatio.h) textum novum in statu
 * loci legit et statum post eum restituit. FALSUM: praefixum vitium
 * habet (vitium impletum), offset > mensura */
nomen structura {
       s32 octava;        /* 'o' '<' '>' (extra 0..8 licet - nota
                           * proxima vitium fit) */
       i32 ordinaria;     /* 'l', pulsus */
       i32 lucrum;        /* 'v', centesimae 0..100 */
    chorda praeparatum;   /* '@' vigens (sectio textus); vacua =
                           * nullum */
       s64 pulsus;        /* positio ad offset */
} CantusStatus;

b32
cantus_status_ad (
          chorda  textus,
             i32  offset,
    CantusStatus* status,
    CantusVitium* vitium);


/* quadra initiorum segmentorum ex pulsibus et temporibus (ordine
 * pulsus crescente; primum ad pulsum 0) */
vacuum
cantus_tempora_computare (
    CantusTempus* tempora,
             i32  numerus);

/* pulsus -> quadrum: initium segmenti + rotundatio intra segmentum, ut
 * lector (idem quadrum quod nota eo pulsu habet). Mappa vacua: CXX */
s64
cantus_quadrum_pulsus (
    constans CantusTempus* tempora,
                      i32  numerus,
                      s64  pulsus);

/* quadrum -> pulsus MAXIMUS cuius quadrum <= quadrum datum (lusor:
 * positio currens, quaerere) */
s64
cantus_pulsus_quadri (
    constans CantusTempus* tempora,
                      i32  numerus,
                      s64  quadrum);


/* ================================================================
 * IUSSA - cantilena -> iussa mixturae (S4 gradus II)
 *
 * Nota quaeque = UNUM iussum SONITUM: vox sua (caudae remissionis
 * notam proximam tegere possunt), parametra coloris cum frequentia
 * notae (sonus_notae_frequentia), momentum = forma->momentum +
 * initium notae, lucrum = nota.lucrum x forma->lucrum, semen =
 * forma->semen + index (varietas per notam, determinata).
 *
 * TENERE: VERUM = porta synthesis - nota per longitudinem scriptam
 *   tenetur (mora = longitudo - impetus, >= 0), deinde remissio
 *   coloris. FALSUM = color intactus (ictus: campana, chorda vellicata)
 *   - sola altitudo et initium a nota.
 *
 * SEDES: rector sedem per notam capit cum forma->prioritas, OMNES
 * statim (iussa praesentia futura ferunt). Ansa nulla (voces plenae
 * prioritatis altioris) = ea nota tacet, ut quivis sonus mixturae -
 * non vitium. Cantilenae longae (> voces) melius per tempus iubentur:
 * sequentia cantus, S6.
 *
 * Pura: nihil iubet - vocans iussa ad auditus_iubere (vivum) aut
 * mixtura_iubere (offline) mittit.
 * ================================================================ */

/* '@titulus' -> parametra; NIHIL = ignotum (vitium cum offset notae) */
nomen constans SonitusParametra* (*CantusResolutor) (
          chorda  titulus,
          vacuum* contextus);

/* resolutor ordinarius: sonitus_praeparatum (titulus <= LXIII octeti) */
constans SonitusParametra*
cantus_praeparatum_sonitus (
    chorda  titulus,
    vacuum* contextus);

nomen structura {
     constans SonitusParametra* color;     /* notae sine '@' */
               CantusResolutor  resolutor;   /* NIHIL = ordinarius */
                        vacuum* contextus;   /* resolutori */
                           b32  tenere;
                   MixturaGrex  grex;        /* INTERFACIES: nuntii */
                           f32  lucrum;      /* totius cantilenae */
                           i32  prioritas;
                           i64  semen;
                           s64  momentum;    /* quadrum initii (auditus:
                                              * nunc + mora + margo) */
} CantusForma;

/* iussa[0..numerus) implet (vocans allocat cantilena->numerus).
 * FALSUM: praeparatum ignotum - vitium.offset = nota eius; nulla sedes
 * capta (praeparata omnia ANTE sedes resolvuntur) */
b32
cantus_iussa_parare (
          constans Cantilena* cantilena,
        constans CantusForma* forma,
               MixturaRector* rector,
               MixturaIussum* iussa,
                CantusVitium* vitium);

#endif /* CANTUS_H */
