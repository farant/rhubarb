/* aetherophonum.h - aetherophonum (theremin; Termen 'etherphone'):
 * latus vici, sonus-spec S5c
 *
 * x = altitudo (logarithmice: semitonia aequaliter per latitudinem,
 * octavae a nota_infima), y = lucrum (summum 1, imum 0). SONAT DUM
 * BOTTON PREMITUR (Franus 2026-10-22): ictus capturam ponit et vocem
 * incipit, motus vocem ponit (transitu brevi), solutio aut Esc portam
 * claudit. Mus super latus sine bottone nihil sonat. Ictus primus
 * (latus nondum focatum) iam sonat.
 *
 * STATUS: praeferentiae in ramo (manifestum apps/aetherophonum/
 * organum.stml, persistentes) - tibia, nota_infima, octavae, gradus,
 * vibratus; claves f g v [ ] - = eas mutant. Vox ipsa KINETICA: ansa,
 * nota et lucrum currentes in montatione, numquam in statu.
 *
 * SONUS per portam (AetherophoniPorta): latus sonum non novit - hospes
 * (vicus_applicatio) portam ponit quae muta et portam soni
 * (vicus_sonus.h) in vocatione iudicat. Porta nulla aut incipere 0
 * reddens = mutum (pictura tamen notam monstrat).
 */

#ifndef AETHEROPHONUM_H
#define AETHEROPHONUM_H

/* <aedilis corpus="lib/aetherophonum.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "insula.h"
#include "actio.h"
#include "figura.h"
#include "dispensator.h"
#include "organum_montatio.h"

/* glissando motus, ms: liber et ad semitonia (gradus) - hoc longius,
 * ne saltus semitonii crepitet */
#define AETHEROPHONUM_TRANSITUS          XV
#define AETHEROPHONUM_TRANSITUS_GRADUUM  XL

nomen structura {
    /* vox nova: tibia praeparata, vibratus, nota MIDI fracta, lucrum
     * 0..1; 0 = nulla (mutus, exitus nullus, sedes plenae) */
    i64    (*incipere)(vacuum* ctx, constans character* tibia,
                       b32 vibratus, f32 nota, f32 lucrum);
    vacuum (*ponere)(vacuum* ctx, i64 vox, f32 nota, f32 lucrum,
                     i32 ms);
    vacuum (*dimittere)(vacuum* ctx, i64 vox);
    vacuum* ctx;
} AetherophoniPorta;

nomen structura {
          InsulaRamus  ramus;
       ActioRegistrum* actiones;
      FiguraRegistrum* figurae;
    AetherophoniPorta  porta;       /* incipere NIHIL = nulla */
                  i64  vox;         /* kinetica: 0 = nulla */
                  b32  tenetur;     /* botton pressus (captura) */
                  f32  nota;        /* currens: pictura et vox */
                  f32  lucrum;
              Piscina* piscina;
  InternamentumChorda* intern;
                  i32  latitudo;    /* superficies montationis */
                  i32  altitudo;
} AetherophoniMontatio;

/* montare in ramo <aetherophonum id> (manifestum infixum); porta NIHIL
 * = muta (copiatur); systemata NIHIL licet */
b32
aetherophonum_montare (
              AetherophoniMontatio* m,
                           Piscina* piscina,
               InternamentumChorda* intern,
                InsulaRepositorium* repo,
                constans character* id,
        constans AetherophoniPorta* porta,
                               i32  latitudo,
                               i32  altitudo,
                   StatusSystemata* systemata);

/* compositor (dispensator; facies vici); ctx = AetherophoniMontatio* */
Componens*
aetherophonum_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx);

/* ictus primus (vicus: ictus qui latus focat): si in campo, vox statim
 * incipit - ut si latus iam focatum esset (theremin uno ictu sonat).
 * arbor = arbor composita hospitis; VERUM = actum */
b32
aetherophonum_ictus_primus (
                 vacuum* ctx,
     InsulaRepositorium* repo,
                  Motus* motus,
              Componens* arbor,
       constans Eventus* ev);

/* x in [0, latitudo) -> nota: nota_infima + octavae x XII x x /
 * latitudo; gradus: ad semitonium proximum. Extra fines coercetur */
f32
aetherophonum_nota (
    s32 x,
    s32 latitudo,
    s32 nota_infima,
    s32 octavae,
    b32 gradus);

/* y in [0, altitudo): summum 1, imum 0, linealiter; coercetur */
f32
aetherophonum_lucrum (
    s32 y,
    s32 altitudo);

#endif /* AETHEROPHONUM_H */
