/* copia_terminalis.h - Scriptura copiae (clipboard) terminalis: OSC 52
 * (eventus B3b-ii; spec Q25)
 *
 * Octeti EXEUNTES, non initus: rivus_terminalis initum legit, haec
 * series copiam scribit. Separata ut tessera (rivum vendicans) base64
 * non trahat. Lectio copiae = glutinum (?2004). Terminal ignarus
 * tacite neglegit: facultas scriptura_copiae FORTASSE, numquam CERTA
 * (terminalis responsum non dat).
 */

#ifndef COPIA_TERMINALIS_H
#define COPIA_TERMINALIS_H

/* <aedilis nexus="purus"/> - clausura sine regula nexus (nulla
 * framework; eventus A1b: bin/aedilis --nexus-purus) */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"

/* Octeti OSC 52 qui textum in copiam scribunt: ESC ] 52 ; c ; base64
 * ESC \ (in piscina). Textus vacuus: series sine onere. Piscina
 * deficiens: chorda vacua. */
chorda
copia_terminalis_componere (
        Piscina* piscina,
    constans i8* textus,
            i32  mensura);

#endif /* COPIA_TERMINALIS_H */
