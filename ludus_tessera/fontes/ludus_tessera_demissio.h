/* ludus_tessera_demissio.h - Mandata quadri in cellulas operis (A2;
 * project-specs/ludus-tessera-plan.md)
 *
 * Demissio = tessellatio_computare (lib, D2: tesseram nescit) +
 * musivum_pingere (pars pura musivi: cellulae -> tessera). Hic solum
 * POLITICA UNA: tessellatio unitates sub RunaePolitica metitur,
 * tessera_graphema_ponere sub politica operis - si dissentiunt,
 * graphemata pereunt aut columnae labuntur (ZWJ familia: GRAPHEMATUM
 * unitas una II cellularum, SIMPLEX tres emoji VI cellularum). Ergo
 * politica tessellationis EX OPERE derivatur, numquam a vocante datur.
 */

#ifndef LUDUS_TESSERA_DEMISSIO_H
#define LUDUS_TESSERA_DEMISSIO_H

#include "latina.h"
#include "piscina.h"
#include "runae.h"
#include "mandatum.h"
#include "modulus.h"
#include "delineare_mandata.h"
#include "tessera_opus.h"

/* Politica runarum quam opus in tessera_graphema_ponere adhibet
 * (eadem regula ac tessera_opus.c) */
RunaePolitica
ludus_tessera_politica_runarum (
    constans TesseraOpus* opus);

/* Mandata (pixela nostra; modulus: cellula et extensio) in cellulas
 * operis ab (0, 0): tessellatio sub politica operis, deinde
 * musivum_pingere. fundus 0x00RRGGBB; fons/fons_ctx ut
 * tessellatio_computare; piscina = memoria quadri (cellulae + via
 * pixelorum; vocans per quadrum vacat). FALSUM si argumentum NIHIL aut
 * memoria deficit. */
b32
ludus_tessera_demittere (
            TesseraOpus* opus,
       constans Mandata* mandata,
       constans Modulus* modulus,
                    i32  fundus,
              ImagoFons  fons,
                 vacuum* fons_ctx,
                Piscina* piscina);

#endif /* LUDUS_TESSERA_DEMISSIO_H */
