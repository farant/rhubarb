/*
 * plastica_specimen.h - mensurae et specimen rei: quomodo Claudius
 * videt quod finxit
 *
 * Statua (ex plastica_legere aut undecumque) -> numeri (fines,
 * triangula, vestigium) et pagina aspectuum una
 * (project-specs/plastica-spec.md IV.3). Instrumentum bin/plastica
 * paginam ut PNG scribit; Claudius eam legit et rem emendat.
 *
 * Nihil de mundo scit (spec 9a): pictura rei in loco stantis iuxta
 * personam res mundi est (bin/mundus specimen, plastica-plan T9).
 *
 * PAGINA: IV columnae x IV ordines cellarum quadratarum.
 *
 *   columnae (aspectus, sine perspectiva, scala una per paginam):
 *     frons          gyrus 0,  inclinatio 0
 *     latus          gyrus 90, inclinatio 0
 *     desuper        inclinatio 90
 *     tribus quartis gyrus 45, inclinatio 35 (anguli visus mundi,
 *                    hic iterum scripti; mundus eos non hinc sumit)
 *   ordines (modi):
 *     crudum         colores veri, illuminatum
 *     exemplaria     ut personae in mundo dithering accipiunt
 *     atkinson       ut humus in mundo dithering accipit
 *     partes         quaeque pars colore plano proprio (XII colores
 *                    in orbem), illuminatum; obices invisi hic
 *                    quoque non pinguntur
 *
 * Scala: sphaera quietis rei cellam cum margine implet, eadem in
 * omni cella - aspectus inter se comparari possunt. Charta alba;
 * dithering solum pixela rei tangit.
 *
 * Determinata: statua eadem paginam eandem dat, octetum pro octeto.
 */

#ifndef PLASTICA_SPECIMEN_H
#define PLASTICA_SPECIMEN_H

/* <aedilis corpus="lib/plastica_specimen.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "imago_typus.h"
#include "stereometria.h"
#include "statua.h"

#define PLASTICA_SPECIMEN_CELLA      CLX    /* pixela per latus */
#define PLASTICA_SPECIMEN_LATITUDO   (IV * PLASTICA_SPECIMEN_CELLA)
#define PLASTICA_SPECIMEN_ALTITUDO   (IV * PLASTICA_SPECIMEN_CELLA)
#define PLASTICA_SPECIMEN_COLORES    XII

/* latus cellae vestigii, unitates: idem ac tabula gressus mundi
 * (quattuor cellae per unitatem), hic iterum scriptum */
#define PLASTICA_CELLA_VESTIGII      0.25f

nomen structura {
    StereometriaVector minimum;      /* fines positurae quietis */
    StereometriaVector maximum;
                   i32 triangulorum;
                   i32 partium;      /* nodi cum reti */
                   i32 invisae;      /* partes quae non pinguntur */
                   i32 cellae_solidae;
                   i32 cellae_gressibiles;
} PlasticaMensurae;

/*
 * plastica_metiri - numeri rei in positura quietis
 *
 * VESTIGIUM: craticula cellarum PLASTICA_CELLA_VESTIGII ab origine
 * rei. Cella numeratur cum CENTRUM eius sub triangulo partis
 * signatae iacet (triangula erecta nihil tegunt); ubi plures,
 * summum vincit - regula tabulae gressus mundi. Cellae solidae 0 in
 * re quae obstare debet: res tenuis est et obice inviso eget (spec
 * 14a). Craticula rei propria est: in loco posita res aliter in
 * cellas cadit, ergo numerus hic indicium est, iudicium structura
 * loci (T9).
 *
 * statua aut piscina NIHIL: omnia 0.
 */
PlasticaMensurae
plastica_metiri (
    constans Statua* statua,
            Piscina* piscina);

/* nomen Anglicum coloris partis k in ordine 'partes' (k per
 * PLASTICA_SPECIMEN_COLORES involvitur): "red", "orange", ... */
constans character*
plastica_specimen_color (
    i32 pars);

/*
 * plastica_specimen_pingere - pagina aspectuum
 *
 * @exitus: imago PLASTICA_SPECIMEN_LATITUDO x _ALTITUDO in piscina
 *
 * Pars k (ordine nodorum cum reti) colorem plastica_specimen_color(k)
 * in ordine 'partes' accipit.
 *
 * Redde: FALSUM si statua, exitus aut piscina NIHIL, aut statua
 * sine verticibus; aliter VERUM
 */
b32
plastica_specimen_pingere (
    constans Statua* statua,
              Imago* exitus,
            Piscina* piscina);

#endif /* PLASTICA_SPECIMEN_H */
