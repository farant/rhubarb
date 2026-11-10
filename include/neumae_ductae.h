/* neumae_ductae.h - partes neumarum quadratarum (notatio Vaticana)
 * ut figurae ductae ad magnitudinem systematis, non ex fonte
 *
 * Spec: project-specs/gabc-spec.md §IV, G14 (glyphae procedurales).
 * Planum: project-specs/gabc-plan-2.md P1-P2. Exemplar:
 * glyphae_ductae.h (larva opacitatis; picturae ASCII in probationibus
 * leges figunt).
 *
 * PARTES, NON NEUMAE: caput, virga, linea obliqua porrecti, clavis,
 * signum. gabc_compositio.h neumas ex partibus componit (tabula
 * COMPOSITIO NEUMARUM ibi); caules ligaturarum et lineae connectentes
 * ibi rectangula sunt, non partes. Purum: nulla allocatio, solum
 * latina.h. LARVA = octetus per pixelum (0 vacuum ... CCLV plenum),
 * ordine linearum, latitudo x altitudo ut neumae_ductae_metiri
 * reddit; vocans in mandata vertit (gabc_pictura).
 *
 * MENSURA: spatium = altitudo spatii inter duas lineas systematis,
 * pixelis logicis; par, VI..XLVIII. gradus = spatium/II (positio una:
 * linea -> spatium). k = crassitudo lineae = max(I, spatium/VIII) -
 * caules, lineae connectentes, additae, systematis, divisiones.
 *
 * ORIGO: punctum larvae ad quod compositio partem ponit. x = margo
 * sinister partis (caput: pixel primus capitis; porrectus: initium
 * lineae obliquae; clavis, signum: margo sinister). y = medium
 * positionis cui pars adhaeret (caput: centrum; clavis: linea clavis;
 * signa supra et infra: centrum signi). capitis_latitudo = ubi caulis
 * dexter et linea connectens adhaerent, ab origine (porrectus: finis
 * lineae obliquae).
 *
 * REGULAE PICTURAE (P2 eas in picturis ASCII ad VIII et XII figit;
 * formae ex tabulis glypharum GregorioRef 6.1.0, appendix A, lectae
 * 2026-11-04):
 *   PUNCTUM: quadratum spatium x spatium, centrum in positione.
 *   INCLINATUM: rhombus spatium latus et altus. INCLINATUM_DEMINUTUM:
 *     rhombus spatium/II.
 *   VIRGA: caput, caulis in margine dextro deorsum; parametrum =
 *     longitudo caulis gradibus a centro (II..VI). VIRGA_REVERSA:
 *     caulis in margine sinistro.
 *   STROPHA: caput angulo superiore dextro ablato (apostropha).
 *   ORISCUS: caput cum cauda undosa dextra.
 *   QUILISMA: caput dentatum, tres dentes supra.
 *   CAVUM: quadratum vacuum, margo k.
 *   LINEA_PUNCTUM: cavum inter duas lineas verticales (R).
 *   DEMINUTUM: caput parvum spatium/II latum et altum (liquescens:
 *     epiphonus, cephalicus, initio debilis).
 *   AUCTUM_ASCENDENS, AUCTUM_DESCENDENS: caput cum cauda parva in
 *     margine dextro sursum, deorsum (< >).
 *   PORRECTUS: fascia obliqua II spatia lata, altitudine verticali
 *     spatium, a positione notae primae deorsum parametrum gradibus
 *     (I..V); capitis_latitudo = II spatia.
 *   LINEA: fascia horizontalis II spatia lata, spatium/II alta (=).
 *   BEMOLLE, NATURALE, DIESIS: signa ante notam, venter (bemolle)
 *     aut medium in positione; latitudo spatium*III/IV.
 *   CLAVIS_C: duo quadrata spatium*III/IV, supra et infra lineam
 *     clavis, caule sinistro iuncta.
 *   CLAVIS_F: clavis C, et ad sinistram caput cum caule deorsum
 *     (forma Vaticana; P4 eam cum Gregorio comparat).
 *   CUSTOS: caput spatium/II latum cum caule tenui; parametrum +I
 *     caulis sursum, -I deorsum.
 *   VIRGULA: comma (`); origo y = centrum commatis.
 *   PUNCTUM_MORAE: quadratum max(II, spatium/IV).
 *   ICTUS: linea verticalis spatium/II longa, k lata.
 *   EPISEMA: linea horizontalis k alta; parametrum = latitudo
 *     pixelis (I..CCLVI); origo in margine sinistro, y medium.
 *   ACCENTUS, ACCENTUS_REVERSUS, CIRCULUS, SEMICIRCULUS,
 *     SEMICIRCULUS_REVERSUS: signa supra notam (r1-r5), spatium/II.
 *
 * Parametrum ceterarum partium 0 esse debet.
 */
#ifndef NEUMAE_DUCTAE_H
#define NEUMAE_DUCTAE_H

/* <aedilis corpus="lib/neumae_ductae.c"/> */

#include "latina.h"

nomen enumeratio {
    NEUMAE_PUNCTUM,
    NEUMAE_INCLINATUM,
    NEUMAE_INCLINATUM_DEMINUTUM,
    NEUMAE_VIRGA,                /* parametrum: gradus caulis */
    NEUMAE_VIRGA_REVERSA,        /* parametrum: gradus caulis */
    NEUMAE_STROPHA,
    NEUMAE_ORISCUS,
    NEUMAE_QUILISMA,
    NEUMAE_CAVUM,
    NEUMAE_LINEA_PUNCTUM,
    NEUMAE_DEMINUTUM,
    NEUMAE_AUCTUM_ASCENDENS,
    NEUMAE_AUCTUM_DESCENDENS,
    NEUMAE_PORRECTUS,            /* parametrum: gradus descensus */
    NEUMAE_LINEA,
    NEUMAE_BEMOLLE,
    NEUMAE_NATURALE,
    NEUMAE_DIESIS,
    NEUMAE_CLAVIS_C,
    NEUMAE_CLAVIS_F,
    NEUMAE_CUSTOS,               /* parametrum: +I sursum, -I deorsum */
    NEUMAE_VIRGULA,
    NEUMAE_PUNCTUM_MORAE,
    NEUMAE_ICTUS,
    NEUMAE_EPISEMA,              /* parametrum: latitudo pixelis */
    NEUMAE_ACCENTUS,
    NEUMAE_ACCENTUS_REVERSUS,
    NEUMAE_CIRCULUS,
    NEUMAE_SEMICIRCULUS,
    NEUMAE_SEMICIRCULUS_REVERSUS,
    NEUMAE_NUMERUS_PARTIUM
} NeumaePars;

/* latitudo * altitudo omnis partis (spatium XLVIII, porrectus V
 * gradibus: XCVI x CLXVIII) - larva in acervo vocantis capit */
#define NEUMAE_LARVA_MAXIMA (CXXVIII * CXXVIII)

nomen structura {
    i32 latitudo;                /* larvae */
    i32 altitudo;
    s32 origo_x;                 /* in larva (vide ORIGO) */
    s32 origo_y;
    i32 capitis_latitudo;        /* ab origine */
} NeumaeMensura;

/* mensura partis; FALSUM si pars ignota, spatium impar aut extra
 * VI..XLVIII, parametrum extra ambitum (mensura tunc intacta) */
b32
neumae_ductae_metiri (
       NeumaePars  pars,
              i32  spatium,
              s32  parametrum,
    NeumaeMensura* mensura);

/* larvam implet (prius vacuat); latitudo et altitudo ut metiri
 * reddit, aliter FALSUM (larva tunc intacta) */
b32
neumae_ductae_pingere (
    NeumaePars  pars,
           i32  spatium,
           s32  parametrum,
           i32  latitudo,
           i32  altitudo,
            i8* larva);

/* titulus Latinus partis ("punctum", "clavis_c" ...) - probationes,
 * forma canonica compositionis; pars ignota -> "ignota" */
constans character*
neumae_partis_titulus (
    NeumaePars pars);

#endif /* NEUMAE_DUCTAE_H */
