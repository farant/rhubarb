/* vicus_applicatio.h - compositio communis hospitis (insula-rami-plan
 * T4)
 *
 * Quod principalia vici (fenestra, terminalis) communicant: genera
 * scriba, pictor, terminale registrata (montare + describere - hic,
 * non in vico: vicus applicationes non novit), vicus apertus
 * (dispositio ordinaria: decem tabulae - 1 scriba | terminale, 2
 * scriba | pictor, 3-10 scriba | scriba), dispensator super
 * repositorium vici ligatus (Motus, destinatio). Glutina accipiunt
 * d, registra figurarum vici (vicus_figurae) et fontem imaginum
 * (vicus_imago_fons, ctx = vicus) - indices semel dati, numquam
 * mutati. */

#ifndef VICUS_APPLICATIO_H
#define VICUS_APPLICATIO_H

/* <aedilis corpus="lib/vicus_applicatio.c"/> */

#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "volumen.h"
#include "dispensator.h"
#include "vicus.h"

nomen structura {
                Piscina* piscina;
    InternamentumChorda* intern;
                Volumen* volumen;
                  Vicus* vicus;
            Dispensator* d;
} VicusApplicatio;

/* Argumenta communia: -fumus (volumen temporarium), -volumen <via>
 * (ordinarie "vicus.volumen"). Reddit volumen apertum aut NIHIL. */
Volumen*
vicus_volumen_aperire (
       Piscina*  piscina,
           s32   argc,
     character** argv,
           b32*  fumus);

/* Compositio tota super volumen apertum. radix = praefixum viarum
 * canonum (NIHIL = directorium currens); latitudo/altitudo fenestrae
 * in pixelis nostris. FALSUM si pars deficit (causa in stderr). */
b32
vicus_applicatio_aedificare (
        VicusApplicatio* app,
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     constans character* radix,
                    i32  latitudo,
                    i32  altitudo);

#endif /* VICUS_APPLICATIO_H */
