/* ludus_tessera_demissio.c - Mandata quadri in cellulas operis (A2).
 * Ratio in capite. */

#include "ludus_tessera_demissio.h"
#include "tessellatio.h"
#include "musivum_pictura.h"

RunaePolitica
ludus_tessera_politica_runarum (
    constans TesseraOpus* opus)
{
    /* eadem regula ac tessera_graphema_ponere (tessera_opus.c) */
    redde (opus && opus->politica == TESSERA_POLITICA_SIMPLEX)
        ? RUNAE_POLITICA_SIMPLEX : RUNAE_POLITICA_GRAPHEMATUM;
}

b32
ludus_tessera_demittere (
            TesseraOpus* opus,
       constans Mandata* mandata,
       constans Modulus* modulus,
                    i32  fundus,
              ImagoFons  fons,
                 vacuum* fons_ctx,
                Piscina* piscina)
{
     TessellatioCellula* cellulae;
                    s32  columnae;
                    s32  lineae;

    si (!opus || !mandata || !modulus || !piscina)
    {
        redde FALSUM;
    }
    modulus_extensio_cellularum(modulus, &columnae, &lineae);
    si (columnae <= ZEPHYRUM || lineae <= ZEPHYRUM)
    {
        redde FALSUM;
    }
    cellulae = (TessellatioCellula*)piscina_allocare_ordinatum(piscina,
        (memoriae_index)(columnae * lineae)
        * magnitudo(TessellatioCellula), VIII);
    si (!cellulae)
    {
        redde FALSUM;
    }
    tessellatio_computare(mandata, modulus,
        ludus_tessera_politica_runarum(opus), fundus, fons, fons_ctx,
        piscina, cellulae);
    musivum_pingere(opus, ZEPHYRUM, ZEPHYRUM, cellulae, columnae,
        lineae);
    redde VERUM;
}
