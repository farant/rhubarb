/* chronica_nomina.h - declarationes personarum et locorum ex lineis
 * paginae (chronica C5a; spec XV). In pagina scribae:
 *   <person>Moses</person>              (aut </>)
 *   <person> <>John</> <>Greg</> </>    fragmenta anonyma = instantiae
 *   <person> <(>John <(>Greg </>        capturae anticae, idem
 *   <place>...</place>                  formae eaedem
 * Tag ANGLICE minusculis ("person", "place"; "people", "Person"
 * non). Regio tag (ab '<person' ad clausuram congruentem, profunditate
 * numerata: '<x' '<>' '<#' +1, '</' -1, '/>' et capturae '(>' nihil;
 * commenta praetermissa) e textu sumitur et SOLA per catenam STML
 * legitur (stml_legere + stml_distribuere: attributa involucri ad
 * singula descendunt, proprium vincit); prosa circa numquam ut STML.
 * Elementum quodque "person"/"place" arboris distributae = declaratio
 * una: titulus = textus, spatia collapsa, tonsus; attributa servata
 * (v1 non monstrantur). Vitia (regio tota; regio vitiosa nullam
 * declarationem dat): "non clausum" (ad proximum '<person' / '<place'
 * / '<event' aut finem), "STML: <causa>", "distributio mixta", "nomen
 * vacuum", "forma non admissa" (captura summa '<person (>'). Pura. */

#ifndef CHRONICA_NOMINA_H
#define CHRONICA_NOMINA_H

/* <aedilis corpus="lib/chronica_nomina.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "xar.h"
#include "chronica_eventus.h"

nomen structura {
    chorda titulus;
    chorda valor;
} ChronicaAttributum;

nomen enumeratio {
    CHRONICA_PERSONA = I,
    CHRONICA_LOCUS
} ChronicaGenusNominis;

nomen structura {
    ChronicaGenusNominis  genus;
                  chorda  titulus;           /* nomen tonsum */
                     Xar* attributa;         /* ChronicaAttributum */
                     i32  linea;             /* elementum (domum) */
                     i32  columna;
                     i32  regio_linea;       /* regio tota (C6) */
                     i32  regio_columna;
                     i32  regio_linea_finis;
                     i32  regio_columna_finis;
} ChronicaDeclaratio;

/* lineae ut chronica_eventa_legere (vacuae numerantur); declarationes
 * et vitia (ChronicaVitium, chronica_eventus.h) adduntur. FALSUM solum
 * si argumenta mala. */
b32
chronica_nomina_legere (
          constans Xar* lineae,
               Piscina* piscina,
   InternamentumChorda* intern,
                   Xar* declarationes,
                   Xar* vitia);

#endif /* CHRONICA_NOMINA_H */
