/* mensa.h - mensa (tabula cogitationum): status tabulae persistens
 * pro apps vitreis - latus C pilae frontis
 *
 * NOMEN SIGILLATUM 2026-08-04 (res 01KZ58K3NH). Paradigma: scriptoria
 * nidificata - scidae tractabiles, scidulae, positio in centesimis;
 * POSITIO INTER SESSIONES PERSISTIT. Hoc caput = dimidium C:
 * tractatores internuntii generici quos QUAEVIS app vitrea montat -
 * pila, non app.
 *
 * FORMA: eventa tabulae (JS: 'mensa-actum' CustomEvent ->
 * persistentia.js -> internuntius) in VOLUMINE conduntur (genus
 * "mensa:<genus>", datum JSON cum "id"); status = plica actorum
 * (fusio plana per-id; "mensa:deletum" removet). Event-sourced =
 * undo/historia gratis postea.
 *
 * Tractatores registrati:
 *   mensa_status {}                -> {"elementa":{id:{...fusum...}}}
 *   mensa_actum  {genus, datum}    -> {"seq": n}
 *
 * Usus (app vitrea):
 *   ctx = mensa_praebere(inx, piscina,
 *       "/Users/nomen/.rhubarb/app.volumen");
 */

#ifndef MENSA_H
#define MENSA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "volumen.h"
#include "internuntius.h"

nomen structura MensaContextus MensaContextus;

/* volumen aperitur (creatur si abest - directorium parens quoque);
 * tractatores registrantur. NIHIL = defectus (causa in stderr). */
MensaContextus*
mensa_praebere (
    Internuntius*       internuntius,
    Piscina*            piscina,
    constans character* via_voluminis);

/* volumen contextus claudere (ante piscina_destruere) */
vacuum
mensa_claudere (
    MensaContextus* contextus);

/* plica actorum in statum - JSON scriptum (publica pro probatione;
 * tractator mensa_status eadem via) */
chorda
mensa_statum_fingere (
    Volumen* volumen,
    Piscina* piscina);

#endif /* MENSA_H */
