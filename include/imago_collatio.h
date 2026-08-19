/*
 * imago_collatio.h - Collatio imaginum: differentia MENSURATA
 *
 * Duas imagines confert, differentiam numerat, et iudicium reddit.
 *
 * CARDO: haec bibliotheca plagulas non tangit. Imagines accipit;
 * numeros et imaginem novam reddit. Nihil legit, nihil scribit.
 * Ergo probari potest imaginibus fictis solis - sine fenestra, sine
 * schirmate, sine exemplaribus in disco servatis. Probatio quae
 * ambitum non tangit mentiri de ambitu non potest.
 *
 * METRUM: differentia pixeli = maxima differentia absoluta inter
 * quattuor canales (R, G, B, A). Pixelum DIVERSUM est cum haec
 * differentia tolerantiam excedit (non aequat - tolerantia I
 * significat "differentiam unius canalis ferre").
 *
 * LENIMENTUM (antialiasing): pixela in oris litterarum et linearum
 * oblique ductarum inter duas percursiones legitime variant. Sine
 * remedio omnis collatio schirmatum falsas differentias parit, et
 * probatio quae semper clamat mox ignoratur - quod peius est quam
 * probatio nulla, quia fiduciam simulat.
 *   Cum regula.sperne_lenimentum, pixela talia deprehenduntur et
 * inter diversa NON numerantur (in pictura tamen flavo signantur).
 * Algorithmus: Kotsarenko & Ramos, "Measuring perceived color
 * difference" - idem quo 'pixelmatch' utitur.
 *
 * MENSURAE DISCREPANTES non sunt culpa sed SENTENTIA propria: duo
 * schirmata diversarum mensurarum significant fenestram mutatam
 * esse. Hoc INVENTUM est, non defectus collationis, ergo nomen
 * proprium meret et non erroris nuntium.
 *
 * ARCA MUTATIONIS: numerus pixelorum diversorum quaestioni "quid
 * fractum est" non respondet. Arca respondet. "III centesimae
 * pixelorum discrepant" nihil docet; "mutatio intra (412,80) et
 * (690,120) iacet" partem fractam NOMINAT.
 */

#ifndef IMAGO_COLLATIO_H
#define IMAGO_COLLATIO_H

#include "latina.h"
#include "piscina.h"
#include "imago_typus.h"   /* typum solum poscimus, non decodificatorem */


/* ============================================================
 * Typi
 * ============================================================ */

/*
 * CollatioSententia - Iudicium collationis
 *
 * CONGRUUNT et DISCREPANT sub regula data dicuntur: eaedem
 * imagines tolerantia mutata aliter iudicantur.
 */
nomen enumeratio {
    COLLATIO_CONGRUUNT = 0,        /* pixela diversa intra pixela_ferenda */
    COLLATIO_DISCREPANT,           /* pixela diversa pixela_ferenda EXCEDUNT */
    COLLATIO_MENSURAE_DISCREPANT,  /* latitudo aut altitudo diversa */
    COLLATIO_ARGUMENTA_INVALIDA    /* NIHIL, aut imago sine pixelis */
} CollatioSententia;

/*
 * CollatioRegula - Quid differentiam CONSTITUAT
 *
 * @tolerantia:        differentia canalis (0-255) quae ferenda est.
 *                     ZEPHYRUM = collatio exacta.
 * @sperne_lenimentum: pixela leniata deprehendere et non numerare.
 * @pixela_ferenda:    QUOT pixela diversa ferri possint antequam
 *                     sententia DISCREPANT fiat (vide infra).
 * @picturam_facere:   picturam differentiae aedificare (pretium:
 *                     altera imago in piscina).
 *
 * CUR PIXELA_FERENDA, CUM TOLERANTIA IAM ADSIT: tolerantia de
 * pixelo UNO iudicat; haec de NUMERO eorum. Duae res sunt et
 * altera alteram supplere non potest.
 *   MENSURATUM 2026-08-13 (textus verus per CoreGraphics pictus,
 * motu SUBPIXELARI 0.3 puncti - contentum idem, sola pictura
 * litterarum mutata):
 *     sine spretione lenimenti:  DXVIII pixela diversa
 *     cum spretione lenimenti:   CCXI diversa, CCCVII leniata
 * Spretio ergo tres quintas partes tollit - CCXI tamen clamant de
 * mutatione quae nulla est. Nec tolerantia auxiliatur: delta
 * maximum illic CXV erat, ergo tolerantia tanta mutationes VERAS
 * quoque devoraret.
 *   Ideo limes in NUMERO poni debet. Qui rationem (e.g. milesimam
 * partem) vult eam ipse computet: regula.pixela_ferenda =
 * (lat * alt) / M. Numerus enim strepitus lenimenti cum LONGITUDINE
 * ORARUM crescit, non cum area - ergo numerus nudus saepe rectior
 * est quam ratio.
 */
nomen structura {
    i32 tolerantia;
    b32 sperne_lenimentum;
    i32 pixela_ferenda;
    b32 picturam_facere;
} CollatioRegula;

/*
 * CollatioFructus - Quid inventum sit
 *
 * @pixela_leniata: pixela quae differebant sed lenimento tribuuntur.
 *                  Numerus magnus hic significat regulam operari;
 *                  numerus magnus cum sperne_lenimentum FALSO
 *                  significat cur probatio clamet.
 * @delta_maximum:  maxima differentia in OMNIBUS pixelis, sive
 *                  numerata sive spreta sive infra tolerantiam.
 *                  Mensura nuda est, non iudicium: ideo docet
 *                  quantum tolerantiae supersit antequam probatio
 *                  clamare incipiat.
 * @arca_*:         arca minima omnia pixela diversa continens.
 *                  arca_latitudo == ZEPHYRUM => nulla mutatio.
 * @pictura:        imago RGBA: fundus canus et pallidus, pixela
 *                  diversa rubra, pixela leniata flava.
 */
nomen structura {
    CollatioSententia sententia;

    i32 pixela_tota;
    i32 pixela_diversa;
    i32 pixela_leniata;
    i32 delta_maximum;

    i32 arca_x;
    i32 arca_y;
    i32 arca_latitudo;
    i32 arca_altitudo;

      b32 pictura_praesto;
    Imago pictura;
} CollatioFructus;


/* ============================================================
 * Functiones
 * ============================================================ */

/*
 * collatio_regula_solita - Regula usitata
 *
 * tolerantia ZEPHYRUM, lenimentum spernitur, pixela_ferenda
 * ZEPHYRUM, pictura non fit.
 *
 * Lenimentum SPERNITUR ex more quia schirmata sine eo numquam
 * congruunt; qui exactitudinem vult eam poscat.
 *
 * pixela_ferenda tamen ZEPHYRUM manet CONSULTO: numerus quisque
 * alius numerus magicus esset sine fundamento. Quantum strepitus
 * ferendum sit ex probatione ipsa pendet - stratum regressionis
 * eum ponat, non haec bibliotheca.
 */
CollatioRegula
collatio_regula_solita (vacuum);

/*
 * imago_conferre - Duas imagines conferre
 *
 * @primus:   imago prior (exemplar servatum)
 * @secundus: imago posterior (quod nunc videtur)
 * @regula:   quid differentiam constituat
 * @piscina:  pro pictura (adhibita solum si regula.picturam_facere)
 *
 * Redde: CollatioFructus. Piscina NIHIL esse potest si pictura
 *        non petitur.
 *
 * Ordo argumentorum momentum habet solum in pictura: fundus canus
 * ex PRIMO ducitur.
 */
CollatioFructus
imago_conferre (
                constans Imago* primus,
                constans Imago* secundus,
                CollatioRegula  regula,
                       Piscina* piscina);

#endif /* IMAGO_COLLATIO_H */
