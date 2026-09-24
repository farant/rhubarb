/* briar_mutationes.h - charta mutationum briar (briar/MUTATIONES.md,
 * in binario infixa): versio binarii EX charta ipsa legitur - caput
 * supremum '## vN ...' - ne numerus et charta dissentiant. PURUM:
 * lectio capsulae in tools/briar.c.
 */
#ifndef BRIAR_MUTATIONES_H
#define BRIAR_MUTATIONES_H

#include "latina.h"
#include "chorda.h"

/* Numerus versionis capitis supremi '## vN' (N >= 1, sequente
 * spatio aut fine lineae); ZEPHYRUM si nullum. '## inedita' et
 * capita alia praetermittuntur. */
i32
briar_mutationes_versio (
    chorda textus);

/* Textus capitis eiusdem post '## ' (e.g. "v3 — 2026-09-24"),
 * spatiis finalibus demptis; vacua si nullum. Spectat in textum. */
chorda
briar_mutationes_caput (
    chorda textus);

/* VERUM si capita '## vN' numeris STRICTE descendentibus stant
 * (novissimum primum) et unum saltem adest. */
b32
briar_mutationes_ordo_rectus (
    chorda textus);

/* Numerus mutationum ineditarum: lineae '- ' sub capite '## inedita'
 * usque ad caput '## ' proximum (lineae continuatae non numerantur);
 * ZEPHYRUM si caput abest aut vacuum. */
i32
briar_mutationes_inedita (
    chorda textus);

#endif /* BRIAR_MUTATIONES_H */
