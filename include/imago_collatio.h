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
    COLLATIO_CONGRUUNT = 0,        /* nulla differentia super tolerantiam */
    COLLATIO_DISCREPANT,           /* differentia inventa */
    COLLATIO_MENSURAE_DISCREPANT,  /* latitudo aut altitudo diversa */
    COLLATIO_ARGUMENTA_INVALIDA    /* NIHIL, aut imago sine pixelis */
} CollatioSententia;

/*
 * CollatioRegula - Quid differentiam CONSTITUAT
 *
 * @tolerantia:        differentia canalis (0-255) quae ferenda est.
 *                     ZEPHYRUM = collatio exacta.
 * @sperne_lenimentum: pixela leniata deprehendere et non numerare.
 * @picturam_facere:   picturam differentiae aedificare (pretium:
 *                     altera imago in piscina).
 */
nomen structura {
    i32 tolerantia;
    b32 sperne_lenimentum;
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

    b32   pictura_praesto;
    Imago pictura;
} CollatioFructus;

/* ============================================================
 * Functiones
 * ============================================================ */

/*
 * collatio_regula_solita - Regula usitata
 *
 * tolerantia ZEPHYRUM, lenimentum spernitur, pictura non fit.
 * Lenimentum SPERNITUR ex more quia schirmata sine eo numquam
 * congruunt; qui exactitudinem vult eam poscat.
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
imago_conferre (constans Imago* primus,
                constans Imago* secundus,
                CollatioRegula  regula,
                Piscina*        piscina);

#endif /* IMAGO_COLLATIO_H */
