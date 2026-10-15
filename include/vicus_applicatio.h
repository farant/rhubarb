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
#include "imago.h"
#include "vicus_quaestio.h"   /* porta $question (typi) */

nomen structura {
                Piscina* piscina;
    InternamentumChorda* intern;
                Volumen* volumen;
                  Vicus* vicus;
            Dispensator* d;
                 vacuum* pictoris;   /* L5: contextus communis pictoris
                                      * (opacus) - porta imaginis */
                 vacuum* quaestionis; /* $question: contextus (opacus) -
                                       * porta quaestionis */
                 vacuum* admonitionum; /* T4a: contextus (opacus) */
} VicusApplicatio;


/* Argumenta communia: -fumus (volumen temporarium), -volumen <via>
 * (ordinarie "vicus.volumen"). Reddit volumen apertum aut NIHIL. */
Volumen*
vicus_volumen_aperire (
       Piscina*  piscina,
           s32   argc,
     character** argv,
           b32*  fumus);

/* Compositio tota super volumen apertum; latitudo/altitudo fenestrae
 * in pixelis nostris. FALSUM si pars deficit (causa in stderr). */
b32
vicus_applicatio_aedificare (
        VicusApplicatio* app,
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
                    i32  latitudo,
                    i32  altitudo);

/* L5: porta imaginis (Cmd+V, clipboard) omnium laterum pictoris -
 * NIHIL = nulla (gemellus terminalis sine Cocoa); applicatio fenestrae
 * clipboard_capere_imaginem ponit. Latera iam montata quoque. */
vacuum
vicus_applicatio_imaginis_portam_ponere (
     VicusApplicatio* app,
        ImagoFructus (*porta)(vacuum* ctx, Piscina* piscina),
              vacuum* ctx);

/* T4a (colloquia-plan): admonitiones ex libro praeconii (via;
 * applicationes praeconium_via dant) - sequax initiatur (cauda), deinde
 * systema 'admonitiones' eum in pulsu sequitur. Ante vocationem
 * (probationes) centrum ($notifications) vacuum, nihil legitur.
 * FALSUM si iam aperta. */
b32
vicus_applicatio_admonitiones_aperire (
        VicusApplicatio* app,
     constans character* via);

/* porta NIHIL = nulla; copia servatur */
vacuum
vicus_applicatio_quaestionis_portam_ponere (
                   VicusApplicatio* app,
    constans VicusPortaQuaestionis* porta);

/* textus planatus -> lineae paginae latitudinis w intra l lineas
 * ("<answer>" ... "</>", margines, praecisio ad finem sententiae), '\n'
 * ducente: quod $question in folium inserit, per pulsum et in
 * fine */
chorda
vicus_quaestio_disponere (
      chorda  planatum,
         s32  w,
         s32  l,
     Piscina* piscina);

#endif /* VICUS_APPLICATIO_H */
