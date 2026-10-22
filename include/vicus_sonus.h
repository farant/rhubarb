/* vicus_sonus.h - typi portae soni (sonus S4 gradus IV)
 *
 * Seorsum a vicus_applicatio.h, ut vicus_quaestio.h: vicus sonum NON
 * novit - nec auditus (CoreAudio) nec tintinnabulum in clausura vici
 * aut in indice manuali ludus_tessera. Applicatio portam ponit
 * (vicus_sonus_porta.h: tintinnabulum super auditus, in eodem
 * processu); porta nulla (probationes) = silentium, '$sound' tamen
 * campos radicis vertit. Typi soli, nullum corpus. */

#ifndef VICUS_SONUS_H
#define VICUS_SONUS_H

#include "latina.h"
#include "chorda.h"

nomen structura {
    /* occasio: genus admonitionis ("permissio", "finis", "vitium");
     * FALSUM = nihil sonat (genus sine cantione, exitus nullus) */
    b32    (*pulsare)(vacuum* ctx, chorda genus);
    /* omni pulsu vici; porta tempus suum legit */
    vacuum (*agere)(vacuum* ctx);
    /* mutus factus: sonans evanescit, exspectata abicitur */
    vacuum (*tacere)(vacuum* ctx);
    vacuum* ctx;
} VicusPortaSoni;

#endif /* VICUS_SONUS_H */
