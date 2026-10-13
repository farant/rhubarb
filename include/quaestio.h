/* quaestio.h - quaestio una ad exemplar linguae (vates): $question in
 * scriba (Franus 2026-10-13)
 *
 * Responsum TEXTUS PURUS intra spatium datum petitur: systema spatium
 * (characteres) et latitudinem paginae dicit; max_tokens ex spatio.
 * Textus ad ASCII redigitur (interpunctio Unicode communis -> ASCII,
 * cetera '?'): cellulae paginae scribae octeti singuli sunt. */

#ifndef QUAESTIO_H
#define QUAESTIO_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "vates.h"

#define QUAESTIO_EXEMPLAR "claude-opus-5-5"

nomen structura {
          b32 successus;
       chorda textus;     /* ASCII; paragraphi '\n' */
       chorda error;      /* si !successus: causa legibilis */
    VatesUsus usus;
} QuaestioResponsum;

QuaestioResponsum
quaestio_rogare (
        Vates* vates,
       chorda  exemplar,      /* vacua = QUAESTIO_EXEMPLAR */
       chorda  quaestio,
          s32  characteres,   /* spatium maximum responsi */
          s32  latitudo,      /* columnae paginae */
      Piscina* piscina);

#endif /* QUAESTIO_H */
