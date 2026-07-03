/* tessera_pons.h - Pons machinae: tabula functionum (Phase A)
 *
 * PONS EST SUTURA PROBATIONUM (spec-v2 par 1.6, CLAUDE.md pin):
 * omnia supra pontem per pontem memoriae probantur (octeti scripti
 * intro, effugia capta foras). Capita systematis in
 * tessera_pons_posix.c SOLO vivunt (Phase B) - numquam in capitibus
 * publicis (mos tcp.h).
 *
 * Contractus:
 *   legere    - octetos usque ad capacitatem intra moram (ms);
 *               reddit numerum lectorum, 0 = mora exacta, -1 = error
 *   scribere  - octetos effundere (totos; FALSUM in errore)
 *   amplitudo - mensura scrinii in cellulis
 *   intrare   - modus crudus + scrinium alternum + al. (status
 *               machinae; pons memoriae solum numerat)
 *   egredi     - omnia restituere
 */

#ifndef TESSERA_PONS_H
#define TESSERA_PONS_H

#include "latina.h"

nomen structura TesseraPons TesseraPons;

structura TesseraPons {
    vacuum* datum;
    s32  (*legere)    (vacuum* datum, i8* buffer, i32 capacitas,
                       s32 mora_ms);
    b32  (*scribere)  (vacuum* datum, constans i8* octeti,
                       i32 numerus);
    b32  (*amplitudo) (vacuum* datum, i32* latitudo_out,
                       i32* altitudo_out);
    b32  (*intrare)   (vacuum* datum);
    b32  (*egredi)     (vacuum* datum);
    /* Resumptio post SIGCONT vel simile (roga-et-purga); NIHIL
     * licet (Phase B additum) - lector eventum RESUMPTUM emittit */
    b32  (*resumptum) (vacuum* datum);
};

#endif /* TESSERA_PONS_H */
