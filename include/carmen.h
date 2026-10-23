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
 *   metrum    N/D, D potentia II (1..64), N 1..64
 *   clavis    littera [#|b] maior|minor (quintae -7..7, ut MusicXML)
 *   tempus    20..400 quartae per minutum (ordinarium CXX)
 *   textus    MML (cantus.h) SINE 't' - tempus carminis est; '@'
 *             praeparatum per notam (percussio sonitus) admittitur
 *   pars      instrumentum necessarium; lucrum 0..1 (1); mutus, solus
 *             (attributa sine valore); frusta in parte non se tegunt
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
               b32  mutus;
               b32  solus;
     CarmenFrustum* frusta;    /* ordine pulsus */
               i32  numerus_frustorum;
} CarmenPars;

nomen structura {
    s64 pulsus;
    i32 numerator;
    i32 denominator;
} CarmenMetrum;

nomen structura {
    s64 pulsus;
    s32 quintae;              /* -7 (7 b) .. 7 (7 #) */
    b32 minor;
} CarmenClavis;

nomen structura {
           chorda  titulus;
     CantusTempus* tempora;           /* quadra computata */
              i32  numerus_temporum;
     CarmenMetrum* metra;
              i32  numerus_metrorum;
     CarmenClavis* claves;
              i32  numerus_clavium;
       CarmenPars* partes;
              i32  numerus_partium;
              s64  pulsus;           /* longitudo: finis frusti ultimi
                                      * (iteratio hic) */
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

/* librum STML legit et TOTUM iudicat. FALSUM: vitium impletum, liber
 * vacuus. Chordae (tituli, textus) in piscina - vivunt dum piscina */
b32
carmina_legere (
            chorda  stml,
           Piscina* piscina,
           Carmina* liber,
    CarminisVitium* vitium);

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

#endif /* CARMEN_H */
