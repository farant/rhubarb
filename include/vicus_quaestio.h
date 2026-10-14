/* vicus_quaestio.h - typi portae $question (quaestio-rivus T8)
 *
 * Seorsum a vicus_applicatio.h ut porta ipsa (vicus_quaestio_porta)
 * applicationem totam non includat: clausura eius CXVII capitum erat
 * (limes apparatus silvae XCVI - frigida 2e4172e0 rubra). Typi soli,
 * nullum corpus. */

#ifndef VICUS_QUAESTIO_H
#define VICUS_QUAESTIO_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"

/* $question per rivum (quaestio-rivus-spec II.8): porta incipit,
 * pulsat, abrumpit - numquam obstat. Pulsus textum PLANATUM totum
 * hactenus reddit (lineae logicae, quaestio.h: porta planat, non
 * vicus - ruling R9: vicus_applicatio sine vate et md manet). Porta
 * nulla (gemellus terminalis, probationes): '$question' errorem
 * nominatum dat. Applicatio fenestrae portam ponit quae bin/quaestio
 * per processum currit (clavem SOLUS adiutor legit). */
nomen structura {
     b32 finitus;      /* F visa (aut adiutor mortuus) */
     b32 mutatum;      /* textus mutatus post pulsum priorem */
  chorda textus;       /* planatum totum hactenus (si mutatum) */
  chorda error;        /* non vacuum: defectus */
     s64 input;
     s64 output;
} VicusPulsusQuaestionis;

nomen structura {
    /* opus incipere; NIHIL + *error si nequit. lineae: pro textu
     * (sine linea "<answer>"); latitudo: columnae textus (sine
     * marginibus) */
    vacuum* (*incipere)(vacuum* ctx, chorda quaestio, s32 lineae,
                        s32 latitudo, Piscina* piscina, chorda* error);
    VicusPulsusQuaestionis (*pulsare)(vacuum* manubrium,
                                      Piscina* piscina);
    vacuum (*abrumpere)(vacuum* manubrium);
    vacuum* ctx;
} VicusPortaQuaestionis;

#endif /* VICUS_QUAESTIO_H */
