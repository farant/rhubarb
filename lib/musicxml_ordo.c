/* musicxml_ordo.c - ordo exsecutionis (caput include/musicxml.h)
 *
 * Visitationes (mensura, transitus) ex lineis mensurae et saltibus.
 * Nunc ordo SCRIPTUS: quaeque mensura semel, transitu I. Repetitiones,
 * conclusiones (voltae) et saltus: plan P6
 * (project-specs/musicxml-plan.md).
 */

#include "musicxml.h"
#include <string.h>

MusicxmlOrdo*
musicxml_ordo (
    constans MusicxmlOpus* opus,
                  Piscina* piscina)
{
    MusicxmlOrdo* ordo;
             i32  k;

    si (opus == NIHIL || piscina == NIHIL)
    {
        redde NIHIL;
    }
    ordo = (MusicxmlOrdo*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(MusicxmlOrdo));
    si (ordo == NIHIL)
    {
        redde NIHIL;
    }
    memset(ordo, ZEPHYRUM, magnitudo(MusicxmlOrdo));
    si (opus->numerus_mensurarum == ZEPHYRUM)
    {
        redde ordo;
    }
    ordo->visitationes = (MusicxmlVisitatio*)piscina_allocare(piscina,
        (memoriae_index)(opus->numerus_mensurarum
            * (i32)magnitudo(MusicxmlVisitatio)));
    si (ordo->visitationes == NIHIL)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < opus->numerus_mensurarum; k++)
    {
        ordo->visitationes[k].mensura    = k;
        ordo->visitationes[k].transitus  = I;
        ordo->visitationes[k].initium    = opus->initia[k];
    }
    ordo->numerus    = opus->numerus_mensurarum;
    ordo->longitudo  = opus->longitudo;
    redde ordo;
}
