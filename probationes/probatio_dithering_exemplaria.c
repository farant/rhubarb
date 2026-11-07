/* probatio_dithering_exemplaria.c - modus dithering per exemplaria
 * (classicus)
 *
 * Sectiones in probationes/dithering_exemplaria_commune.h habitant
 * (suita velox easdem currit); hic fixura suitae solum functionem
 * classicam eligit.
 */
#include "latina.h"
#include "piscina.h"
#include "credo.h"
#include "dithering.h"
#include "dithering_exemplaria.h"
#include "dithering_exemplaria_commune.h"

interior vacuum
_parare_suitae (
    CredoContextus* c)
{
    DitheringExemplariaProbandum* p = (DitheringExemplariaProbandum*)
        piscina_allocare(c->piscina,
                                                                       (memoriae_index)magnitudo(
                                                                           DitheringExemplariaProbandum));

    CREDO_NECESSE(p != NIHIL);
    p->functio  = dithering_exemplaria_colorum;
    c->datum    = p;
}

hic_manens constans CredoSectio SECTIONES[] = {
    DITHERING_EXEMPLARIA_SECTIONES_COMMUNES
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("dithering_exemplaria", SECTIONES,
        _parare_suitae, NIHIL);
}
