/*
 * graphis_lumen.h - graphis: lux picturae, arithmetica integra
 *
 * Fons picturae (color cuiusque pixeli ante picturam) luce
 * multiplicatur: lux ambiens cum umbra angulorum, lumina puncti et
 * lumina areae. Functio pura super tabulas; de STML nihil scit
 * (graphis.c descriptionem legit et hanc vocat). Arithmetica
 * fluitans nulla: lux in partibus 4096 (GRAPHIS_LUX_UNA = 1.0)
 * (project-specs/graphis-spec.md 13, 14, 23d).
 *
 * IN DESCRIPTIONE (graphis.h):
 *
 *   <pictura lux="0.55" tinctura="#d1dcff" umbra="0.35">
 *     ...
 *     <rectangulum titulus="fenestra" ... lucens="verum"/>
 *     <lumen locus="170 140" radii="75 58" color="#ffb873"
 *            vis="0.85"/>
 *     <lumen area="214,156 286,156 322,198 226,198" mollities="3"
 *            color="#fff0d8" vis="0.55"/>
 *   </pictura>
 *
 * LUX PIXELI, per canalem (r, g, b):
 *
 *   ambiens  lux x tinctura x (1 - umbra x d), ubi d = quadratum
 *            distantiae a centro picturae, 0 in centro, 1 in
 *            angulis. Ordinaria: lux 1, tinctura alba, umbra 0 -
 *            pictura sine luce descripta immutata manet
 *   punctum  vis x color x g, ubi g = exp(-t / 2) et
 *            t = (dx / radius_x)^2 + (dy / radius_y)^2: radius est
 *            distantia ubi lumen ad 0.61 cadit; ultra IV radios
 *            nihil. g ex tabula LXV integrorum legitur, inter
 *            gradus lineariter
 *   area     vis x color x tectum, ubi tectum = pars punctorum
 *            (pixelum ipsum bis, quattuor vicini ad distantiam
 *            'mollities' semel) quae intra polygonum sunt.
 *            mollities 0: margo durus
 *
 * Lux = ambiens + summa luminum. Fons illustratus = fons x lux,
 * ad CCLV saturatus. Lumina formae non sunt: ordo eorum nihil
 * mutat, pixela non possident, in gregibus non stant.
 *
 * LUCENS: forma cum lucens="verum" lucem dat nec accipit (fenestra,
 * flamma): pixela eius lucem 1.0 exacte habent - fons immutatus.
 *
 * FINES: lux 0 .. 1; umbra 0 .. 1; vis luminis 0 .. 2 (tres loci
 * decimales summum); radii I .. 4096; mollities 0 .. XVI; area
 * III .. LXIV puncta; lumina summum XVI.
 *
 * RECUSAT graphis_legere, cum linea: lucem, umbram aut vim extra
 * fines aut cum plus tribus locis; radium nullum aut negativum;
 * lumen cum 'locus' et 'area' simul, aut sine utroque; 'radii' sine
 * 'locus', 'mollities' sine 'area'; aream minus III punctis; lumen
 * intra gregem; plus quam XVI lumina; lucens aliud quam "verum".
 */

#ifndef GRAPHIS_LUMEN_H
#define GRAPHIS_LUMEN_H

/* <aedilis corpus="lib/graphis_lumen.c"/> */

#include "latina.h"
#include "piscina.h"

#define GRAPHIS_LUX_UNA           (IV * MXXIV)
#define GRAPHIS_LUMINA_MAXIMA     XVI
#define GRAPHIS_AREA_PUNCTA       LXIV
#define GRAPHIS_MOLLITIES_MAXIMA  XVI

nomen structura {
    b32  punctum;      /* VERUM: locus et radii; FALSUM: area */
    s32  x;
    s32  y;
    s32  radius_x;     /* I .. 4096 */
    s32  radius_y;
    s32* area_x;       /* punctorum coordinatae */
    s32* area_y;
    i32  punctorum;
    i32  mollities;
     i8  color[III];
    i32  vis;          /* 0 .. II x GRAPHIS_LUX_UNA */
} GraphisLumen;

nomen structura {
             i32  lux;        /* 0 .. GRAPHIS_LUX_UNA */
             i32  umbra;      /* 0 .. GRAPHIS_LUX_UNA */
              i8  tinctura[III];
    GraphisLumen* lumina;
             i32  luminum;
} GraphisLux;

/*
 * graphis_illustrare - fontem luce multiplicare
 *
 * @fons:      rgba, IV octeti per pixelum, latitudo x altitudo
 * @lucentes:  octetus per pixelum, non 0 = lucens (lux 1.0 exacte);
 *             NIHIL = nulla
 * @latitudo:  pixela (centrum umbrae = centrum huius magnitudinis)
 * @altitudo:  pixela
 * @lux:       ambiens et lumina
 * @piscina:   exitus
 *
 * Redde: rgba illustratum (alpha CCLV); NIHIL si argumentum NIHIL,
 * magnitudo nulla, valor extra fines supra dictos, aut memoria
 * deficit
 */
i8*
graphis_illustrare (
            constans i8* fons,
            constans i8* lucentes,
                    i32  latitudo,
                    i32  altitudo,
    constans GraphisLux* lux,
                Piscina* piscina);

#endif /* GRAPHIS_LUMEN_H */
