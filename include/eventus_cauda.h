/* eventus_cauda.h - Cauda eventuum fontis: anulus + onera per lectionem
 * (eventus A3; spec Q10, Q13, Q16, D5)
 *
 * PURA (sine Cocoa, sine tessera): fenestra eam habet, fons
 * terminalis (phasis B) eadem utetur. Tempus NON hic stampatur (horologium
 * platformae est): fons tempus implet ante impulsum.
 *
 * ONERA (textus, exempla motus) in tabulis INTERNIS caudae copiantur; visus
 * eventuum (datum.textus.contentum) in eas monstrant. VITA: usque ad
 * lectionem proximam (eventus_cauda_lectio_incipit) - SED tabulae
 * vacantur SOLUM si cauda vacua est: eventa nondum extracta visus suos
 * servant. Tabula plena: textus truncatur (truncatum VERUM).
 */

#ifndef EVENTUS_CAUDA_H
#define EVENTUS_CAUDA_H

/* <aedilis nexus="purus"/> - clausura sine regula nexus (nulla
 * framework; eventus A1b: bin/aedilis --nexus-purus) */

#include "latina.h"
#include "eventus.h"

#define EVENTUS_CAUDA_CAPACITAS  CCLVI       /* eventa */
#define EVENTUS_CAUDA_TEXTUS     65536       /* octeti oneris textus */
#define EVENTUS_CAUDA_EXEMPLA    (IV * MXXIV) /* exempla motus coaliti */

nomen structura {
            Eventus eventus[EVENTUS_CAUDA_CAPACITAS];
                i32 caput;
                i32 finis;
                i32 numerus;
                i32 amissa;          /* eventa abiecta: cauda plena */
                 i8 textus[EVENTUS_CAUDA_TEXTUS];
                i32 textus_mensura;
    EventusExemplum exempla[EVENTUS_CAUDA_EXEMPLA];
                i32 exempla_mensura;
} EventusCauda;

vacuum
eventus_caudam_initiare (
    EventusCauda* cauda);

/* Initium lectionis fontis (fenestra: perscrutari). Si cauda vacua:
 * tabulae onerum vacantur (visus priores iam consumpti). */
vacuum
eventus_cauda_lectio_incipit (
    EventusCauda* cauda);

/* Eventum (valore) addere; FALSUM si plena (amissa++). */
b32
eventus_caudae_impellere (
          EventusCauda* cauda,
      constans Eventus* eventus);

/* EVENTUS_TEXTUS COMMISSUM cum octetis COPIATIS addere. Mensura 0 ->
 * nihil, FALSUM. Tabula plena -> pars quae capit, truncatum VERUM. */
b32
eventus_caudae_textum_impellere (
       EventusCauda* cauda,
                s64  tempus,
        constans i8* octeti,
                i32  mensura,
       EventusOrigo  origo);

/* EVENTUS_MUS_MOTUS cum COALITIONE (spec Q10, Q13, D5): si eventus
 * ULTIMUS caudae (nondum extractus) MOTUS est cum eisdem bottone,
 * modificantibus, indicatore - positio eius (x, y, tempus) exemplum
 * fit et novus eam supplet; alioquin impellitur ut eventus novus.
 * Exempla eventus unius CONTIGUA sunt (solum ultimus crescit), visus
 * in tabulam caudae. Plura quam EVENTUS_EXEMPLA_MAXIMA (aut tabula
 * plena): antiquissima servantur, positio ultima semper vera. */
b32
eventus_caudae_motum_impellere (
          EventusCauda* cauda,
      constans Eventus* eventus);

/* Pars integra residui (rotula; spec Q14): *residuum += delta, pars
 * integra (versus ZEPHYRUM truncata) redditur et a residuo demitur.
 * Fons residuum per fenestram tenet - fractiones non pereunt. */
s32
eventus_residuum_integrare (
     f64* residuum,
     f64  delta);

/* Eventum antiquissimum extrahere; FALSUM si vacua. */
b32
eventus_caudae_extrahere (
    EventusCauda* cauda,
         Eventus* exitus);

#endif /* EVENTUS_CAUDA_H */
