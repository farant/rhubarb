/* carmen.h - Carmen: cantus plurium partium in tempore musico
 *
 * EXEMPLAR (sonus-spec VIII-d): carmen -> partes (instrumentum per
 * titulum, lucrum, mutus, solus) -> frusta (positio in pulsibus) ->
 * notae (pulsus intra frustum, longitudo, nota MIDI, lucrum,
 * orthographia). Mappae carminis: temporis (CantusTempus, cantus.h -
 * mappa UNA cum lectore), metri, clavis. Tempus MUSICUM: pulsus
 * (CANTUS_PULSUS_QUARTAE per quartam); quadra solum in sonando
 * (cantor).
 * Purus: nihil sonat. Pagina musicae et notatio (postea) hoc exemplar
 * edunt; MML via una est frustum scribendi.
 *
 * FORMA (carmina.stml, infixus - carmina_infixum.h):
 *
 *   <carmina>
 *     <carmen titulus="korobeiniki" tempus="144" metrum="4/4"
 *             clavis="a minor">
 *       <tempus mensura="9" valor="160"/>         (* mutationes *)
 *       <metrum mensura="9" valor="3/4"/>
 *       <clavis mensura="9" valor="c maior"/>
 *       <pars titulus="melodia" instrumentum="quadrans" lucrum="0.8">
 *         <frustum mensura="1"><textus!>o5 l8 e4 <b >c d4</textus></>
 *       </>
 *     </>
 *   </carmina>
 *
 *   positio   mensura="N" (1 = prima, ex mappa metri) aut pulsus="N" -
 *             altera sola
 *   ictus     cum mensura, ubi pulsus admittitur (frustum, tempus,
 *             clavis, fermata): tempus N mensurae (1 = initium;
 *             unitas = carmen_tempus_mensurae, ut locus 'M:T' - in 6/8
 *             octavae tres)
 *   metrum    N/D, D potentia II (1..64), N 1..64
 *   agogica   in carmine (metro initiali) aut in <metrum>: pondera
 *             K partium aequalium mensurae ("0.86 1.15 0.99" in 3/4;
 *             "2 1 2 1 2 1 2 1" = octavae inaequales in 4/4), normata
 *             ad summam K - mensura longitudine sua, partes intra eam
 *             longius aut brevius sonant; 'vindobonensis' = "0.86 1.15
 *             0.99". Valet usque ad metrum proximum (sine agogica =
 *             aequale). K 2..XVI, pondera > 0, mensura in K partes
 *             pulsuum integras divisibilis
 *   clavis    littera [#|b] maior|minor (quintae -7..7, ut MusicXML)
 *   tempus    20..400 quartae per minutum (ordinarium CXX); sine
 *             'usque' gradus; cum usque="M" clivus: a tempore vigente
 *             ad valorem, linearis in pulsibus, finis ad initium
 *             mensurae M (nulla mutatio intra clivum)
 *   fermata   <fermata mensura="16" factor="2"/>: sonus a positione
 *             usque ad initium notae proximae (pars qualibet; finis
 *             carminis si nulla) factor-plo longior (1..4, ord. 2);
 *             fermatae non se tegunt
 *   dynamica  <dynamica [pars="P"] mensura="M" [ictus] valor="mf"
 *             [usque="U"]/>: lucrum partis (aut carminis totius sine
 *             pars) a positione - gradus, aut clivus (crescendo /
 *             diminuendo, aequalis in dB) ad initium mensurae U.
 *             valor pp p mp mf f ff (0.2 0.32 0.45 0.6 0.8 1) aut
 *             numerus 0..1; ordinarium 1. Lucrum notae = nota ('v') x
 *             pars x dynamica partis x dynamica carminis ad initium
 *             notae; notae sonantes clivum per tempora sequuntur.
 *             Clivus cum fine 0: linearis (dB nulla). Ordine
 *             positionis per partem; clivus eadem positione qua gradus
 *             prior eum servat; nulla mutatio intra clivum
 *   textus    MML (cantus.h) SINE 't' - tempus carminis est; '@'
 *             praeparatum per notam (percussio sonitus) admittitur
 *   pars      instrumentum necessarium; lucrum 0..1 (1); positio
 *             -1..1 (0, potentia constans); reverberatio 0..1 (0:
 *             missio ad aulam); mutus, solus (attributa sine valore);
 *             frusta in parte non se tegunt
 *   reverberatio  <reverberatio aula mollities latitudo lucrum/>
 *             (omnia 0..1; ordinaria 0.5, 0.5, 1, 0.25) in carmine aut
 *             in libro (ordinaria carminibus sine sua); nulla = lucrum
 *             0. Elementum totum substituitur, non attributa singula
 *
 * INSTRUMENTA (libri et carminis):
 *   <carmina>
 *     <instrumentum titulus="cembalo" basis="chorda" claritas="0.9"
 *                   decrementum="0.8" ambitus="41..89"/>
 *     <carmen titulus="...">
 *       <instrumentum titulus="campana_mollis" basis="campana"
 *                     index="3"/>          (* carminis proprium *)
 *       <pars instrumentum="cembalo" ...>
 *
 *   instrumentum  titulus [a-z0-9_]+; basis = praeparatum (tibia.h) aut
 *                 instrumentum libri SUPRA definitum (catena); sine
 *                 basi: genus unda, forma sinus, officium 0.5, tenor
 *                 1, amplitudo 0.5, cetera 0. Attributa = campi
 *                 TibiaParametra: genus (tibia_generis_titulus: unda|
 *                 chorda|modulatio|partiales|formantes), forma
 *                 (pulsus|serra|triangulum|sinus), officium, impetus,
 *                 lapsus, tenor, remissio, vibratus, celeritas, gravia,
 *                 amplitudo, decrementum, claritas, ratio, index,
 *                 partiales="r:a:d ...", vocalis (a|e|i|o|u aut 0..4),
 *                 flatus, resonans, acumen, motus (-48..48),
 *                 unisonus (1..7), dispersio (centesimae 0..100),
 *                 mora (vibrati, secundae);
 *                 ambitus="N..M" (MIDI, censurae). Ordo quaerendi:
 *                 carminis, libri, instrumentarii (instrumentarium.stml
 *                 infixus, carmina_legere), praeparata - titulus
 *                 inferior redefinitus vincit. Vitia: attributum
 *                 ignotum, numerus malus aut extra fines, basis
 *                 ignota, titulus iteratus in eodem gradu.
 *
 * MAPPA SONANDI: Carmen.tempora = gradus soli (clivi per
 * CARMEN_GRADUS_TEMPORIS pulsus - tricesima secunda - tempore medii
 * gradus; fermatae: tempus vigens / factor intra spatium suum;
 * agogica: pars mensurae tempore vigenti / pondere);
 * scripta in tempora_scripta et fermatae (partitura ea nominat).
 * Analysis tempus SCRIPTUM iudicat (pulsus), numquam sonandi.
 *
 * TOTUS LIBER IN LECTIONE IUDICATUR: vitium cum carmine, parte, frusto
 * et offset in textu frusti - numquam carmen semi-lectum.
 */
/* <aedilis corpus="lib/carmen.c"/> */
#ifndef CARMEN_H
#define CARMEN_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "cantus.h"
#include "tibia.h"

/* gradus mappae sonandi in clivo: tricesima secunda (pulsus) */
#define CARMEN_GRADUS_TEMPORIS CXX

nomen structura {
          s64 pulsus;        /* intra frustum */
          s64 longitudo;     /* pulsus, > 0 */
          i32 nota;          /* MIDI 0..127 */
          f32 lucrum;        /* 0..1 ('v') */
    character littera;       /* orthographia 'a'..'g' scripta; '\0' =
                               * ex clave */
       chorda praeparatum;   /* '@' (percussio: sonitus pro nota);
                               * vacua = instrumentum partis */
          i32 offset;        /* in textu frusti */
} CarmenNota;

nomen structura {
            s64  pulsus;      /* initium in carmine */
         chorda  textus;      /* MML ut scriptus */
     CarmenNota* notae;       /* ordine pulsus */
            i32  numerus;
            s64  longitudo;   /* pulsus, pausa ultima inclusa */
    CantusLinea* lineae;      /* '|' scripta, pulsus intra frustum (ex
                               * cantilena lecta, eadem piscina) */
            i32 numerus_linearum;
} CarmenFrustum;

nomen structura {
            chorda  titulus;
            chorda  instrumentum;
               f32  lucrum;
               f32  positio;        /* -1 sinistra .. 1 dextra (0) */
               f32  reverberatio;   /* missio ad aulam 0..1 (0) */
               b32  mutus;
               b32  solus;
     CarmenFrustum* frusta;    /* ordine pulsus */
               i32  numerus_frustorum;
} CarmenPars;

#define CARMEN_PONDERA_MAXIMA XVI

nomen structura {
       s64 pulsus;
       i32 numerator;
       i32 denominator;
       f32 pondera[CARMEN_PONDERA_MAXIMA];  /* agogica, summa =
                                              * numerus_ponderum */
       i32 numerus_ponderum;                 /* 0 = aequale */
    chorda inaequalitas;                     /* 'agogica' ut scripta;
                                              * vacua */
} CarmenMetrum;

nomen structura {
    s64 pulsus;
    s32 quintae;              /* -7 (7 b) .. 7 (7 #) */
    b32 minor;
} CarmenClavis;

/* dynamica SCRIPTA (<dynamica>; intensio, nomen Latinum): gradus
 * (usque 0) aut clivus aequalis in dB */
nomen structura {
       s32 pars;       /* index partis; -1 = carmen totum */
       s64 pulsus;     /* initium */
       f32 valor;      /* 0..1 (finis clivi) */
       s64 usque;      /* finis clivi, pulsus; 0 = gradus */
    chorda textus;     /* valor ut scriptus ('mf', '0.4') */
} CarmenIntensio;

/* fermata SCRIPTA (<fermata>; corona, nomen signi Latinum): spatium
 * [pulsus, finis) factor-plo longius */
nomen structura {
    s64 pulsus;     /* initium */
    s64 finis;      /* nota proxima aut finis carminis (derivatum) */
    f32 factor;     /* 1..4 */
} CarmenCorona;

/* mutatio temporis SCRIPTA: gradus (usque 0) aut clivus */
nomen structura {
    s64 pulsus;     /* initium */
    i32 tempus;     /* valor scriptus (finis clivi) */
    s64 usque;      /* finis clivi, pulsus; 0 = gradus */
} CarmenTempus;

nomen structura {
            chorda titulus;
    TibiaParametra parametra;
               i32 infima;     /* ambitus; 0 et CXXVII si non datus */
               i32 summa;
} CarmenInstrumentum;

nomen structura {
           chorda  titulus;
     CantusTempus* tempora;           /* mappa SONANDI (clivi in
                                       * gradus), quadra computata */
              i32  numerus_temporum;
     CarmenTempus* tempora_scripta;   /* ut in libro; primum = tempus
                                       * carminis ad pulsum 0 */
              i32  numerus_temporum_scriptorum;
     CarmenCorona* coronae;          /* ordine pulsus */
              i32  numerus_coronarum;
   CarmenIntensio* intensiones;      /* <dynamica>, ordine libri */
              i32  numerus_intensionum;
     CarmenMetrum* metra;
              i32  numerus_metrorum;
     CarmenClavis* claves;
              i32  numerus_clavium;
       CarmenPars* partes;
              i32  numerus_partium;
              s64  pulsus;           /* longitudo: finis frusti ultimi
                                      * (iteratio hic) */
    CarmenInstrumentum* instrumenta;  /* carminis, libri, deinde
                                       * instrumentarii */
                   i32 numerus_instrumentorum;
                   i32 numerus_instrumentarii; /* ultimi tot ex
                                                * instrumentario */
    SonusReverberatioForma reverberatio;      /* aula carminis */
                       f32 lucrum_reverberationis;  /* reditus; 0 =
                                                     * nulla */
} Carmen;

nomen structura {
     Carmen* carmina;   /* ordine libri */
        i32  numerus;
} Carmina;

nomen structura {
                 chorda  carmen;    /* vacua = liber ipse */
                 chorda  pars;      /* vacua = carmen ipsum */
                    s32  frustum;   /* index in parte; -1 = nullum */
                    i32  offset;    /* in textu frusti (0 aliter) */
     constans character* causa;     /* Latina, statica */
} CarminisVitium;

/* librum STML legit et TOTUM iudicat, cum instrumentario ordinario
 * (instrumentarium_ordinarium) sub libro. FALSUM: vitium impletum,
 * liber vacuus. Chordae (tituli, textus) in piscina - vivunt dum
 * piscina */
b32
carmina_legere (
            chorda  stml,
           Piscina* piscina,
           Carmina* liber,
    CarminisVitium* vitium);

/* liber cum instrumentario dato (NIHIL, 0 = nullum): basis et
 * quaestio ea sub libro vident */
b32
carmina_legere_cum (
                         chorda  stml,
    constans CarmenInstrumentum* instrumentarium,
                            i32  numerus_instrumentarii,
                        Piscina* piscina,
                        Carmina* liber,
                 CarminisVitium* vitium);

/* instrumentarium: <instrumentarium> cum filiis <instrumentum>
 * (grammatica libri; basis = praeparatum aut instrumentum SUPRA).
 * Chordae in piscina */
b32
instrumentarium_legere (
                 chorda   stml,
                Piscina*  piscina,
     CarmenInstrumentum** instrumenta,
                    i32*  numerus,
         CarminisVitium*  vitium);

/* instrumentarium ordinarium (instrumentarium.stml infixus) */
b32
instrumentarium_ordinarium (
                Piscina*  piscina,
     CarmenInstrumentum** instrumenta,
                    i32*  numerus,
         CarminisVitium*  vitium);

/* liber ordinarius (carmina.stml infixus) */
b32
carmina_ordinaria (
           Piscina* piscina,
           Carmina* liber,
    CarminisVitium* vitium);

/* NIHIL si ignotum */
constans Carmen*
carmina_invenire (
    constans Carmina* liber,
              chorda  titulus);

/* initium mensurae (1 = prima = pulsus 0) ex mappa metri; mensurae
 * ultra mappam metro ultimo procedunt */
s64
carmen_pulsus_mensurae (
    constans Carmen* carmen,
                i32  mensura);

/* tempus metri ad pulsum (absolutum), in pulsibus: 1/denominator;
 * metra composita (denominator VIII, numerator > III et multiplex III:
 * 6/8 9/8 12/8) tres octavae. Unitas una: linea harmoniae, tempora
 * fortia censurae, locus emendationis ('M:T') */
s64
carmen_tempus_mensurae (
    constans Carmen* carmen,
                s64  pulsus);

/* lucrum dynamicae ad pulsum: partis 'pars' x carminis totius (1 sine
 * <dynamica>); clivus aequalis in dB inter valorem priorem et suum
 * (linearis si alter 0). pars extra partes: carminis solum */
f32
carmen_intensio (
    constans Carmen* carmen,
                s32  pars,
                s64  pulsus);

/* NIHIL si nec carmen nec liber definit (praeparata non hic) */
constans CarmenInstrumentum*
carmen_instrumentum (
    constans Carmen* carmen,
             chorda  titulus);

#endif /* CARMEN_H */
