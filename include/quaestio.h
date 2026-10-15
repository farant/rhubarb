/* quaestio.h - quaestio una ad exemplar linguae (vates): $question in
 * scriba (Franus 2026-10-13)
 *
 * Responsum intra paginam datam petitur: systema formam paginae
 * (lineae, latitudo) dicit; max_tokens ex ea.
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
       chorda causa_finis;  /* cruda: "end_turn", "max_tokens"... */
} QuaestioResponsum;

/* ASCII + Markdown -> lineae logicae (paragraphus una linea, "# "
 * capitula, praefixa elementorum, codex '\t' ducente; vide
 * lib/quaestio.c). Communis rogationis, rivi et vici (quaestio-rivus
 * T5): textus finalis idem quacumque via. */
chorda
quaestio_textum_planare (
      chorda  crudum,
     Piscina* piscina);

/* lineae: lineae paginae pro textu; latitudo: columnae textus (post
 * margines). Systema formam paginae dicit (Franus 2026-10-14: lineae,
 * non characteres - paragraphus lineas totas consumit); max_tokens =
 * lineae * latitudo / III + LXIV */
/* petitio $question communis (rogare, rivus, porta interna - via B):
 * systema formam paginae dicit; exemplar vacua = QUAESTIO_EXEMPLAR */
VatesPetitio*
quaestio_petitionem_parare (
     chorda  exemplar,
     chorda  quaestio,
        s32  lineae,
        s32  latitudo,
    Piscina* piscina);

/* responsum vatis -> QuaestioResponsum (textus blocorum planatus) */
QuaestioResponsum
quaestio_responsum_ex_vate (
    constans VatesResponsum* responsum,
                    Piscina* piscina);

QuaestioResponsum
quaestio_rogare (
        Vates* vates,
       chorda  exemplar,      /* vacua = QUAESTIO_EXEMPLAR */
       chorda  quaestio,
          s32  lineae,
          s32  latitudo,
      Piscina* piscina);

/* idem per rivum: deltae CRUDAE textus (non cogitationis) per rivum
 * dum adveniunt; responsum finale ut quaestio_rogare (planatum) */
QuaestioResponsum
quaestio_rogare_rivum (
        Vates* vates,
       chorda  exemplar,
       chorda  quaestio,
          s32  lineae,
          s32  latitudo,
   VatesRivus  rivus,
       vacuum* datum,
      Piscina* piscina);

/* ---- tabulae adiutoris (bin/quaestio -> vicus; quaestio-rivus II.6)
 * "T <n>\n<n octeti>" textus, "E <n>\n<n octeti>" error,
 * "U <input> <output>\n" usus, "F <causa>\n" finis ---- */
nomen enumeratio {
    QUAESTIO_TABULA_TEXTUS = 0,
    QUAESTIO_TABULA_ERROR,
    QUAESTIO_TABULA_USUS,
    QUAESTIO_TABULA_FINIS
} QuaestioTabulaGenus;

nomen structura {
    QuaestioTabulaGenus genus;
                 chorda textus;   /* TEXTUS, ERROR; FINIS: causa */
                    s64 input;
                    s64 output;
} QuaestioTabula;

nomen enumeratio {
    QUAESTIO_LECTIO_TABULA = 0,   /* tabula impleta, positio promota */
    QUAESTIO_LECTIO_PLURA,        /* tabula imperfecta: exspecta */
    QUAESTIO_LECTIO_PRAVA         /* forma prava */
} QuaestioLectio;

chorda
quaestio_tabulam_scribere (
    constans QuaestioTabula* tabula,
                    Piscina* piscina);

/* tabula ab *positio in fluxu (textus copiatur in piscinam) */
QuaestioLectio
quaestio_tabulam_legere (
            chorda  fluxus,
               i32* positio,
    QuaestioTabula* tabula,
           Piscina* piscina);

/* adiutor (bin/quaestio): rogare_rivum, tabulae per scriptorem - T per
 * deltam STATIM; successus: U + "F <causa_finis_cruda>"; error (ante
 * aut post textum): E + "F error". Semper F ultima. */
nomen vacuum (*QuaestioScriptor)(vacuum* datum, chorda octeti);

vacuum
quaestio_adiutor_currere (
               Vates* vates,
              chorda  exemplar,
              chorda  quaestio,
                 s32  lineae,
                 s32  latitudo,
    QuaestioScriptor  scriptor,
              vacuum* datum,
             Piscina* piscina);

#endif /* QUAESTIO_H */
