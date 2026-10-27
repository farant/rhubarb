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
    /* cantilena DATA (praeco cantus): textus MML; FALSUM = fracta aut
     * exitus nullus - nihil sonat. Ut pulsare: una simul, recentissima
     * vincit */
    b32    (*canere)(vacuum* ctx, chorda textus, b32 intactus);
    /* VOX VIVA (S5c aetherophonum): tibia praeparata ("aether",
     * "quadrans", "triangulum"), vibratus aut non, nota MIDI fracta,
     * lucrum 0..1; prioritas supra cantilenas. 0 = nulla (exitus
     * nullus, sedes plenae) */
    i64    (*vocem_incipere)(vacuum* ctx, constans character* tibia,
                             b32 vibratus, f32 nota, f32 lucrum);
    /* nota et lucrum cum glissando 'ms' (0 = statim) */
    vacuum (*vocem_ponere)(vacuum* ctx, i64 vox, f32 nota, f32 lucrum,
                           i32 ms);
    /* porta clausa: remissio. tacere (mutus) portas omnium vocum
     * vivarum quoque claudit */
    vacuum (*vocem_dimittere)(vacuum* ctx, i64 vox);
    /* vicus quies T6d: ms usque ad agere proximum necessarium - XVI
     * dum sonat aut exspectat; dum exitus apertus et quietus, usque ad
     * clausuram (CONCENTUS_QUIES post sonum ultimum); -1 si clausus.
     * NIHIL = XVI (mos vetus) */
    s64    (*mora)(vacuum* ctx);
    vacuum* ctx;
} VicusPortaSoni;

#endif /* VICUS_SONUS_H */
