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
 * CLAVES: eventus singuli per codificator_terminalis sub modis quos
 * aemulator_modi reddit (D6b: DECCKM, kitty, mus, glutinum, focus,
 * LNM) in aemulator_hospes_scribere (initus visum ad imum reducit).
 * MUS: ictus ad programma si petivit. ROTULA (lineae; trackpad
 * pixela -> lineae per altitudinem cellulae): ad programma si murem
 * petivit; in schirmo altero cum ?1007 sagittae; aliter visum movet.
 * FOCUS: ?1004 positum relationem statim mittit.
 *
 * MAGNITUDO: superficies (dispensator: superficies_*) / cellula ->
 * columnae x lineae -> aemulator_hospes_amplitudo - in PULSU, non in
 * componendo (componere purum manet).
 *
 * COLORES (D6b): thema in configurationem aemulatoris (litterae,
 * fundus, cursor); pinguntur ex aemulator_color - OSC 4/10/11/12
 * programmatis videntur. Nativus non mutatus = signum thematis (thema
 * vivum); RGB ipsum. Inversum, crassum (color clarior 0-7) ut
 * Ghostty.
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
                 vacuum* contextus;         /* privatum (D6b) */
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

/* Titulus (OSC 0/2, D6b): ultimus quem programma posuit (vacuus =
 * nullus; praecisus ad CCLVI octetos). *mutatus VERUM semel post
 * quamque mutationem - principale tunc titulum fenestrae ponit.
 * Chorda valet usque ad pulsum proximum. */
chorda
terminale_titulus (
    TerminaleApplicatio* app,
                    b32* mutatus);

/* Infantem claudere (aemulator_hospes_claudere). */
vacuum
terminale_claudere (
    TerminaleApplicatio* app);

#endif /* TERMINALE_H */
