#include "piscina.h"
#include "persistentia.h"
#include "entitas_repositorium.h"
#include "internamentum.h"
#include <stdio.h>
#include <string.h>

/*
 * EVENTUS INSPECTOR - Instrumentum pro inspicere eventus in log file
 *
 * Usus: ./bin/probatio_eventus_inspector [via_log]
 * Default: data/combinado.log
 */

constans character* EVENTUS_NOMINA[] = {
    "CREARE_ENTITAS",
    "DELERE_ENTITAS",
    "PONERE_PROPRIETAS",
    "DELERE_PROPRIETAS",
    "ADDERE_RELATIO",
    "DELERE_RELATIO",
    "ADDERE_NOTA",
    "DELERE_NOTA"
};

interior constans character*
_eventus_nomen(EventusGenus genus)
{
    /* Nota: EventusGenus incipit ab I, non ZEPHYRUM */
    si (genus >= I && genus <= VIII)
    {
        redde EVENTUS_NOMINA[genus - I];
    }
    redde "IGNOTUM";
}

interior vacuum
_imprimere_chorda(chorda* c)
{
    i32 i;
    si (c == NIHIL || c->datum == NIHIL)
    {
        imprimere("(nil)");
        redde;
    }
    per (i = ZEPHYRUM; i < c->mensura && i < L; i++)
    {
        imprimere("%c", c->datum[i]);
    }
    si (c->mensura > L)
    {
        imprimere("...");
    }
}

integer
principale(integer argc, character** argv)
{
    Piscina*             piscina;
    Persistentia*        persistentia;
    EntitasRepositorium* repo;
    Xar*                 eventus;
    i32                  i;
    i32                  numerus;
    constans character*  via_log;

    /* Statisticae */
    i32 counts[VIII];

    per (i = ZEPHYRUM; i < VIII; i++)
    {
        counts[i] = ZEPHYRUM;
    }

    /* Via log ex argumento vel default */
    si (argc > I)
    {
        via_log = argv[I];
    }
    alioquin
    {
        via_log = "data/combinado.log";
    }

    imprimere("═══════════════════════════════════════\n");
    imprimere("EVENTUS INSPECTOR\n");
    imprimere("═══════════════════════════════════════\n");
    imprimere("Via: %s\n\n", via_log);

    /* Creare piscina */
    piscina = piscina_generare_dynamicum("inspector", M * M * II);  /* 2MB */
    si (!piscina)
    {
        imprimere("Fractura: non potest creare piscinam\n");
        redde I;
    }

    /* Aperire persistentia */
    persistentia = persistentia_nuntium_aperire(piscina, via_log);
    si (!persistentia)
    {
        imprimere("Fractura: non potest aperire %s\n", via_log);
        piscina_destruere(piscina);
        redde I;
    }

    /* Creare repo (hoc legit eventus) */
    repo = entitas_repositorium_creare(piscina, persistentia);
    si (!repo)
    {
        imprimere("Fractura: non potest creare repositorium\n");
        piscina_destruere(piscina);
        redde I;
    }

    /* Legere omnes eventus */
    eventus = repo->legere_omnes_eventus(repo->datum);
    si (!eventus)
    {
        imprimere("Nullae eventus in log\n");
        piscina_destruere(piscina);
        redde ZEPHYRUM;
    }

    numerus = xar_numerus(eventus);
    imprimere("Numerus eventorum: %d\n\n", numerus);

    /* Imprimere eventus */
    imprimere("─────────────────────────────────────────\n");
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        Eventum* e;
        e = *(Eventum**)xar_obtinere(eventus, i);
        si (e)
        {
            /* Incrementare count */
            si (e->genus >= I && e->genus <= VIII)
            {
                counts[e->genus - I]++;
            }

            /* Imprimere eventum */
            imprimere("[%4d] %-20s ", i, _eventus_nomen(e->genus));

            /* Entitas ID (truncatum) */
            si (e->entitas_id)
            {
                _imprimere_chorda(e->entitas_id);
            }

            /* Genus entitatis */
            si (e->entitas_genus)
            {
                imprimere(" (");
                _imprimere_chorda(e->entitas_genus);
                imprimere(")");
            }

            /* Payload */
            commutatio (e->genus)
            {
                casus EVENTUS_PONERE_PROPRIETAS:
                    imprimere(" ");
                    _imprimere_chorda(e->datum.proprietas.clavis);
                    imprimere("=");
                    _imprimere_chorda(e->datum.proprietas.valor);
                    frange;

                casus EVENTUS_DELERE_PROPRIETAS:
                    imprimere(" -");
                    _imprimere_chorda(e->datum.proprietas.clavis);
                    frange;

                casus EVENTUS_ADDERE_RELATIO:
                    imprimere(" --[");
                    _imprimere_chorda(e->datum.relatio.relatio_genus);
                    imprimere("]--> ");
                    _imprimere_chorda(e->datum.relatio.destinatio_id);
                    frange;

                casus EVENTUS_ADDERE_NOTA:
                case EVENTUS_DELERE_NOTA:
                    imprimere(" #");
                    _imprimere_chorda(e->datum.nota.nota);
                    frange;

                ordinarius:
                    frange;
            }

            imprimere("\n");
        }
    }

    /* Summa */
    imprimere("\n═══════════════════════════════════════\n");
    imprimere("SUMMA PER GENUS\n");
    imprimere("═══════════════════════════════════════\n");
    per (i = ZEPHYRUM; i < VIII; i++)
    {
        si (counts[i] > ZEPHYRUM)
        {
            imprimere("  %-20s %d\n", EVENTUS_NOMINA[i], counts[i]);
        }
    }
    imprimere("───────────────────────────────────────\n");
    imprimere("  TOTALIS              %d\n", numerus);

    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
