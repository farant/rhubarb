/*
 * statua_pictura.h - pictor statuae: triangula in pixela, in CPU
 *
 * Statua + matrices nodorum (statua_positura) + visus -> pictura RGBA
 * et tabula tecti (project-specs/statuae-spec.md IV.4). Pictura cruda
 * est: dithering postea fit (dithering.h), ergo pixela hic colores
 * veros tenent, sine lenitione marginum (anti-aliasing) - margines
 * duri et texturis pixelatis et dithering conveniunt.
 *
 * Determinatum: eadem inputa -> idem octetim. Nihil inter vocationes
 * servatur.
 *
 * VISUS: oculus statuam e longinquo spectat, centrum sphaerae
 * ambientis (positurae quietis, statua.h) in medio picturae; statua
 * quiescens omni gyro intra picturam cum margine manet. Pars quam
 * cyclus longe fert (saxum iactum) picturam relinquere potest. Gyrus
 * positivus frontem statuae ad dextram spectantis vertit; inclinatio
 * positiva oculum supra fert, negativa infra.
 *
 * LUX (illuminatum): fons unus e sinistro supero, visui affixus (non
 * statuae), et lux fundi constans; claritas una per triangulum
 * (facies planae). Materia emissiva lucem neglegit. Planum: claritas
 * I ubique.
 */

#ifndef STATUA_PICTURA_H
#define STATUA_PICTURA_H

/* <aedilis corpus="lib/statua_pictura.c"/> */

#include "latina.h"
#include "piscina.h"
#include "statua.h"
#include "stereometria.h"

/* visus ordinarius: ante, tribus quartis (gradus) */
#define STATUA_GYRUS_ORDINARIUS        30.0f
#define STATUA_INCLINATIO_ORDINARIA    15.0f
/* inclinatio cogitur ad +- hanc: statua numquam invertitur */
#define STATUA_INCLINATIO_MAXIMA       85.0f
/* campus visus (gradus) per latus picturae brevius */
#define STATUA_CAMPUS_GRADUS           30.0f
/* margo circa sphaeram, pars lateris brevioris */
#define STATUA_MARGO                   0.08f
/* claritas faciei a luce aversae (I = plena) */
#define STATUA_LUX_FUNDI               0.35f

nomen structura {
    f32 gyrus;         /* gradus circa axem verticalem; quivis */
    f32 inclinatio;    /* gradus; cogitur ad +- MAXIMA */
    b32 illuminatum;   /* FALSUM = planum (colores ut picti) */
} StatuaVisus;

nomen structura {
     i8* rgba;         /* IV per pixelum, ordo summus primus; ubi
                        * statua non tegit: album opacum */
     i8* tectum;       /* I per pixelum: I = statua, 0 = charta */
    i32  latitudo;
    i32  altitudo;
} StatuaPictura;

/*
 * statua_pingere - statuam in posituram datam pingere
 *
 * @statua:   finita (statua_finire)
 * @matrices: nodorum matrices ex statua_positura
 * @visus:    NIHIL = ordinarius, illuminatus
 * @latitudo, @altitudo: pixela; 0 licet (pictura vacua)
 * @exitus:   pictura in piscina
 *
 * Profunditas per pixelum (propius vincit); terga omittuntur nisi
 * materia bilateralis; textura proxima (sine interpolatione),
 * perspectiva correcta; pixelum cuius alpha infra limen materiae est
 * non pingitur. Triangula planum propinquum secantia secantur;
 * degenerata omittuntur.
 *
 * Redde: FALSUM solum si argumenta NIHIL; aliter VERUM
 */
b32
statua_pingere (
                constans Statua* statua,
    constans StereometriaMatrix* matrices,
           constans StatuaVisus* visus,
                            i32  latitudo,
                            i32  altitudo,
                  StatuaPictura* exitus,
                        Piscina* piscina);


/* ==================================================
 * Scaena: statuae plures locatae, visus parallelus
 * ================================================== */

/*
 * Pictor alter (mundus: project-specs/mundus-spec.md IV.2). Statuae
 * plures, quaeque matrice 'locatio' in mundum posita, in picturam
 * unam cum profunditate una pinguntur, SINE perspectiva.
 *
 * PIXELUM MUNDI puncti p: E = rotatio_x(inclinatio) *
 * rotatio_y(gyrus);
 *     x = E(p).x * pixela_per_unitatem
 *     y = -E(p).y * pixela_per_unitatem     (deorsum crescit)
 *     profunditas = -E(p).z                  (unitates mundi; maius =
 *                                             remotius)
 * Nulla translatio per centrum: pixelum picturae = pixelum mundi
 * minus origo (integra). Ergo picturae duae quarum origines pixelis
 * integris differunt in omni pixelo communi octetim conveniunt
 * (rgba, tectum, profunditas), et profunditates picturarum
 * diversarum inter se conferri possunt - humus semel picta labi
 * potest, corpus seorsum pictum contra eam iudicari.
 *
 * Cetera ut statua_pingere: regula marginum, textura proxima,
 * excisio alpha, claritas una per triangulum, determinatum. Textura
 * linealiter interpolatur; tergum est facies cuius normalis ab oculo
 * avertitur secundum axem visus; planum propinquum nullum.
 */

#define STATUA_LOCATAE_MAXIMAE CCLV

nomen structura {
                constans Statua* statua;
    constans StereometriaMatrix* matrices;  /* statua_positura */
             StereometriaMatrix  locatio;   /* statua -> mundus */
} StatuaLocata;

nomen structura {
    f32 gyrus;                /* gradus */
    f32 inclinatio;           /* gradus */
    f32 pixela_per_unitatem;  /* > 0 */
    s32 origo_x;              /* pixelum mundi in angulo summo
                               * sinistro picturae */
    s32 origo_y;
    b32 illuminatum;
} StatuaScaenaVisus;

nomen structura {
     i8* rgba;         /* IV per pixelum; vacuum: album opacum */
     i8* tectum;       /* 0 = nihil; k = locata k-1 */
    f32* profunditas;  /* unitates mundi secundum axem visus; maius
                        * = remotius; vacuum: valor immanis
                        * (> 1.0e20) */
    i32 latitudo;
    i32 altitudo;
} StatuaScaenaPictura;

/*
 * statua_scaena_proicere - punctum mundi -> pixelum mundi (fractum)
 * et profunditas
 *
 * origo visus non legitur (pixela MUNDI, non picturae). visus aut
 * exitus NIHIL: nihil fit.
 */
vacuum
statua_scaena_proicere (
    constans StatuaScaenaVisus* visus,
            StereometriaVector  punctum,
                           f32* x,
                           f32* y,
                           f32* profunditas);

/*
 * statua_scaenam_pingere - statuas locatas in picturam unam pingere
 *
 * @locatae:  numerus elementa; NIHIL licet si numerus 0
 * @numerus:  <= STATUA_LOCATAE_MAXIMAE; 0 = pictura alba
 * @latitudo, @altitudo: pixela; 0 licet (pictura vacua)
 *
 * Ubi duae statuae eandem profunditatem habent, prior in tabula
 * vincit.
 *
 * Redde: FALSUM si visus, exitus aut piscina NIHIL, numerus nimius,
 * pixela_per_unitatem non > 0, aut locata sine statua aut sine
 * matricibus; aliter VERUM
 */
b32
statua_scaenam_pingere (
         constans StatuaLocata* locatae,
                           i32  numerus,
    constans StatuaScaenaVisus* visus,
                           i32  latitudo,
                           i32  altitudo,
           StatuaScaenaPictura* exitus,
                       Piscina* piscina);

#endif /* STATUA_PICTURA_H */
