/* tessellatio.h - Mandata in cellulas terminalis: via cellularum
 * (project-specs/tessellatio-plan.md D2, D3, D5, D6)
 *
 * PURA: exitus a vocante praebitur, nullus status, nulla dependentia
 * tesserae (positio in tessera tenuis et separata, T5). Coordinatae
 * Mandatorum sunt pixela nostra; cellulae per Modulum derivantur.
 *
 * CELLULA tenet UNITATEM textus (monstrator + mensura in chordam
 * Mandatorum: graphemata integra manent; valida dum Mandata vivunt)
 * AUT bita juncturarum (lineae), aut nihil (spatium). Latitudo 2 =
 * unitas lata; cellula sequens = CONTINUATIO (latitudo 0).
 *
 * TRANSFORMATIO coetus = rasterizatoris nativi exacte: origo + locale *
 * scala; sectio ∩ parentis. Positio primitivi = PAVIMENTUM (columna,
 * linea); rectangula impleta et sectio = margines PROXIMI (D5).
 *
 * VIA CELLULARUM (D6, ordine pictoris; posterior priorem tegit):
 * - textus: unitates runae (sub politica) a cellula positionis; '\n'
 *   ad lineam proximam (columna initii); fundus cellulae MANET (titulus
 *   super rectangulum aut imaginem); unitas latitudinis 0 omittitur;
 *   unitas lata cuius dimidium extra sectionem/extensionem cadit
 *   omittitur;
 * - rectangulum impletum: fundus; textus et juncturae purgantur;
 * - rectangulum non impletum: margo cellularum per juncturas (anguli
 *   et latera); cellula una (1x1) evanescit;
 * - linea axialis: juncturae OR-antur (┼ ├ ┬ ex intersectionibus);
 *   textum tegit;
 * - imago, polygonum, linea obliqua: NON hic (via pixelorum, T4).
 * Dimidium unitatis latae tectum: dimidium alterum spatium fit (regula
 * tesserae).
 */

#ifndef TESSELLATIO_H
#define TESSELLATIO_H

/* <aedilis corpus="lib/tessellatio.c"/> */

#include "latina.h"
#include "mandatum.h"
#include "modulus.h"

#define TESSELLATIO_JUNCTURA_SURSUM    I
#define TESSELLATIO_JUNCTURA_DEXTRA    II
#define TESSELLATIO_JUNCTURA_DEORSUM   IV
#define TESSELLATIO_JUNCTURA_SINISTRA  VIII

nomen structura {
    constans i8* unitas;           /* NIHIL = nulla unitas textus */
            i32  mensura;          /* octeti unitatis */
            i32  latitudo;         /* I, II; 0 = continuatio latae */
            i32  juncturae;        /* bita TESSELLATIO_JUNCTURA_* */
            i32  color_litterae;   /* 0x00RRGGBB */
            i32  color_fundi;
} TessellatioCellula;

/* Mensor scopi terminalis (D4): unitates runae sub politica. */
ModulusMensor
tessellatio_mensor (
    RunaePolitica politica);

/* Mandata -> cellulae. exitus: columnae x lineae cellularum quas
 * modulus_extensio_cellularum(modulus) reddit, ordine linearum;
 * initio omnes vacuae cum fundo 'fundus' et littera 'fundus'.
 * Colores Mandatorum per thema resolvuntur (THEMA, INDEX, RGBA). */
vacuum
tessellatio_computare (
        constans Mandata* m,
        constans Modulus* modulus,
           RunaePolitica  politica,
                     i32  fundus,
      TessellatioCellula* exitus);

/* Juncturae -> runa delineandi (─ │ ┌ ┐ └ ┘ ├ ┤ ┬ ┴ ┼); 0 si nullae.
 * Bitum solum (finis lineae) = linea plena eius axis. */
s32
tessellatio_runa_juncturae (
    i32 juncturae);

#endif /* TESSELLATIO_H */
