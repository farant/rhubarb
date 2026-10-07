/* terminale.h - applicatio terminalis: aemulator_hospes in ludo
 * (aemulator-plan E-thin, decisiones XXIII-XXV)
 *
 * Compositio communis fenestrae (apps/terminale/terminale.c) et
 * terminalis (terminale_terminalis.c), ut pictor et scriba - sed
 * bibliotheca UNA (frustum tenue): repositorium in memoria (nihil
 * durabile, decisio III), componens unus focusabilis (schirmum
 * cellularum, actio "terminale.clavis"), figura quae VISUM hospitis
 * legit (cellulae, stili, cursor), actiones, applicatio.
 *
 * CLAVES: eventus singuli per codificator_terminalis (legacy - modi
 * clavium et muris ab aemulatore phasis D) in
 * aemulator_hospes_scribere (initus visum ad imum reducit). ROTULA:
 * aemulator_hospes_visum_movere (lineae; trackpad pixela -> lineae
 * per altitudinem cellulae).
 *
 * MAGNITUDO: superficies (dispensator: superficies_*) / cellula ->
 * columnae x lineae -> aemulator_hospes_amplitudo - in PULSU, non in
 * componendo (componere purum manet).
 *
 * COLORES: nativus = thema (COLOR_TEXT / COLOR_BACKGROUND); 0-15
 * xterm, 16-231 cubus, 232-255 gradus grisei; RGB ipsum. Inversum,
 * crassum (color clarior 0-7), obscurum ut Ghostty.
 *
 * ANSA: principale suam ansam possidet (decisio XXV): quadrum quodque
 * terminale_pulsare(mora brevis) deinde tractare, quadrum, praesentare.
 */

#ifndef TERMINALE_H
#define TERMINALE_H

/* <aedilis corpus="lib/terminale.c"/> */

#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "dispensator.h"
#include "figura.h"
#include "actio.h"
#include "pseudoterminale.h"
#include "aemulator_hospes.h"

nomen structura {
                Piscina* piscina;
    InternamentumChorda* intern;
     InsulaRepositorium* repo;
         ActioRegistrum* actiones;
        FiguraRegistrum* figurae;
            Dispensator* d;
        AemulatorHospes* hospes;
                    i32  cellula_latitudo;  /* pixela nostra */
                    i32  cellula_altitudo;
} TerminaleApplicatio;

/* Argumenta infantis: -fumus -> /bin/sh scriptum breve (imago
 * certa, exitus); aliter $SHELL (aut /bin/zsh) cum "-l" (concha
 * initialis, ut Terminal.app et Ghostty). Vector NIHIL-terminatus in
 * piscina. */
constans character* constans*
terminale_argumenta (
     Piscina*  piscina,
         s32   argc,
   character** argv,
         b32*  fumus);

/* Compositio tota: repositorium, registra, dispensator, hospes super
 * pt (hospes eum possidet). latitudo/altitudo superficiei in pixelis
 * nostris; cellula ordinaria VI x VIII. FALSUM si pars deficit (pt
 * tunc clausus). */
b32
terminale_applicatio_aedificare (
    TerminaleApplicatio* app,
                Piscina* piscina,
    InternamentumChorda* intern,
        Pseudoterminale* pt,
                    i32  latitudo,
                    i32  altitudo);

/* Pulsus unus ante quadrum: magnitudinem ex superficie aptare,
 * hospitem pulsare (mora ut aemulator_hospes_pulsare). Pulsus hospitis
 * redditur (mutatum -> quadrum pingendum; finitus -> concha exiit). */
AemulatorHospesPulsus
terminale_pulsare (
    TerminaleApplicatio* app,
                    s32  mora_ms);

/* Infantem claudere (aemulator_hospes_claudere). */
vacuum
terminale_claudere (
    TerminaleApplicatio* app);

#endif /* TERMINALE_H */
