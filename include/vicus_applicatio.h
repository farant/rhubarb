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
#include "vicus_sonus.h"      /* porta soni (typi) */

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
                 vacuum* soni;         /* sonus S4: contextus opacus */
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

/* T4a (colloquia-plan): fontes libri praeconii (via; applicationes
 * praeconium_via dant) - sequax admonitionum (systema 'admonitiones'
 * in pulsu) et T5 fons sessionum (lateribus $sessions visibilibus
 * pulsatus) ex cauda initiantur; T6 systema 'terminalia' (per V s)
 * latera terminalis tmux adhaerentia notat: argumentum "tmux
 * <servitor|-> <sessio>" (clientes servitorum sessionum, deinde
 * ordinarii; per tty), detachata tollit. Argumentum tale montat
 * "/bin/sh -c '<terminale respondet>; <tmux attach>; exec $SHELL -l'"
 * (T6b: attach solum postquam vicus terminale pulsat - tmux responsa
 * V s solum exspectat; post detachmentum concha). Ante vocationem
 * (probationes) centrum ($notifications) et sessiones vacua, nihil
 * legitur, tmux non interrogatur. FALSUM si iam aperta. */
b32
vicus_applicatio_admonitiones_aperire (
        VicusApplicatio* app,
     constans character* via);

/* porta NIHIL = nulla; copia servatur */
vacuum
vicus_applicatio_quaestionis_portam_ponere (
                   VicusApplicatio* app,
    constans VicusPortaQuaestionis* porta);

/* sonus S4: porta soni (vicus_sonus.h) - NIHIL = nulla (silentium);
 * copia servatur. '$sound' (mutus totus) et '$sound(notifications)'
 * (admonitiones solae) campos radicis sonus_mutus et
 * sonus_admonitionum_mutus (persistentes) vertunt, etiam sine porta.
 * Admonitio nova (systema 'admonitiones') genus recentissimae pulsat
 * nisi muta; systema 'sonus' omni pulsu mutum factum tacet, deinde
 * agit. Eventus 'cantus' libri praeconii (praeco cantus: nuntius =
 * MML, genus "intactus" aut vacuum) recentissimus canitur, nisi mutus
 * QUILIBET; cum admonitione nova eodem pulsu, ordine temporis
 * (posterior vincit). */
vacuum
vicus_applicatio_soni_portam_ponere (
            VicusApplicatio* app,
    constans VicusPortaSoni* porta);

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
