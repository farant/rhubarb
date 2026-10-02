/* auscultator.c - Eventus fenestrae ut STML in terminali (eventus A3;
 * aspectus Frani)
 *
 * Fenestram aperit; omnem Eventum (eventus.h, vocabularium sine
 * iactura) ut lineam STML unam in stdout scribit - clavis (codex
 * physicus, runa logica, actio, latera in modificantibus), textus
 * separatus, mus, rotula. Exitus: fenestram claudere.
 *
 * Curre per: ./tools/auscultator.sh [mora_ms]
 *
 * mora_ms (A3b): post lectionem quamque dormire - applicatio lenta
 * (quadrum ~XVI ms) simulatur, ut motus coalitus cum exemplis
 * appareat; sine mora fere quisque motus solus legitur.
 */
#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "internamentum.h"
#include "fenestra.h"
#include "eventus_stml.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

s32
principale (
          s32   argc,
    character** argv)
{
                 Piscina* piscina;
     InternamentumChorda* intern;
    FenestraConfiguratio  configuratio;
                Fenestra* fenestra;
                 Eventus  eventus;
                     b32  currens  = VERUM;
                     s32  mora     = ZEPHYRUM;

    si (argc > I)
    {
        mora = (s32)atoi(argv[I]);
    }
    piscina = piscina_generare_dynamicum("auscultator", 16777216);
    si (piscina == NIHIL)
    {
        fprintf(stderr, "auscultator: piscina deest\n");
        redde I;
    }
    intern = internamentum_creare(piscina);
    memset(&configuratio, ZEPHYRUM, magnitudo(FenestraConfiguratio));
    configuratio.titulus =
        "auscultator - claves, mus, rotula (claude ut exeas)";
    configuratio.x         = C;
    configuratio.y         = C;
    configuratio.latitudo  = DC;
    configuratio.altitudo  = CD;
    configuratio.vexilla   = FENESTRA_ORDINARIA;
    fenestra               = fenestra_creare(piscina, &configuratio);
    si (fenestra == NIHIL)
    {
        fprintf(stderr, "auscultator: fenestra deest\n");
        redde I;
    }
    fenestra_monstrare(fenestra);
    fenestra_clavem_capere(fenestra);
    printf("auscultator: scribe, preme, move, rota in fenestra; "
        "claude ut exeas.\n");
    fflush(stdout);
    dum (currens && !fenestra_debet_claudere(fenestra))
    {
        fenestra_expectare_eventus(fenestra, CC);
        dum (fenestra_obtinere_eventus(fenestra, &eventus))
        {
               Xar* unum;
            chorda  linea;

            si (eventus.genus == EVENTUS_CLAUDERE)
            {
                currens = FALSUM;
            }
            unum = xar_creare(piscina, (i32)magnitudo(Eventus));
            *(Eventus*)xar_addere(unum) = eventus;
            linea = eventus_scribere_stml(unum, piscina, intern,
                FALSUM);
            printf("%.*s\n", (int)linea.mensura,
                (constans character*)linea.datum);
            fflush(stdout);
        }
        si (mora > ZEPHYRUM)
        {
            fenestra_dormire((i32)mora * M);
        }
    }
    fenestra_destruere(fenestra);
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
