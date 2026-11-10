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

#endif /* STATUA_PICTURA_H */
