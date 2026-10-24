/* concentus_auditus.h - exitus concentus super auditus (CoreAudio)
 *
 * Adaptator UNUS ConcentusExitus -> auditus.h, communis porta soni vici
 * et auditioni (antea in vicus_sonus_porta.c interior).
 *
 * ANULI auditus in piscina PROPRIA, per aperitionem creata et in
 * claudendo destructa: concentus post quietem claudit et iterum aperit
 * - in piscina vocantis anuli per dies cumularentur.
 *
 * Auditus unus per processum (auditus.h): unus concentus super hunc
 * exitum simul.
 *
 * USUS:
 *   forma.voces = XLVIII;
 *   forma.iussa = forma.nuntii = CXXVIII;
 *   e = concentus_exitus_auditus(piscina, &forma);
 *   k = e ? concentus_creare(piscina, &forma, e) : NIHIL;
 */
/* <aedilis corpus="lib/concentus_auditus.c"/> */
#ifndef CONCENTUS_AUDITUS_H
#define CONCENTUS_AUDITUS_H

#include "latina.h"
#include "piscina.h"
#include "mixtura.h"
#include "concentus.h"

/* exitus et status eius e piscina (vivant dum concentus); capacitates
 * anulorum = forma->iussa, forma->nuntii (eadem forma concentui
 * datur). Nihil aperitur. NIHIL si memoria deest */
ConcentusExitus*
concentus_exitus_auditus (
                  Piscina* piscina,
    constans MixturaForma* forma);

#endif /* CONCENTUS_AUDITUS_H */
