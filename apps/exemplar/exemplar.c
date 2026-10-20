/* exemplar.c - applicatio probans hospitii v3 (exemplar.golden.earth)
 *
 *   exemplar -data <dir> [-portus N] [-hospes 127.0.0.1]
 *            [-invitatio <codex> | -invitatio-via <filum>]
 *            [-procurator] [-acta]
 *
 * -invitatio-via: codex ex filo (spatia finalia abiecta) - in
 * productione (systemd), ne codex in 'ps' aut in unitate appareat.
 *
 * Principale tenue: argumenta, reactor UNUS (servus, labores
 * argon2id, signa), SIGTERM/SIGINT -> sistere molliter (X s), deinde
 * volumen clausum et exitus 0. -procurator: post nginx (X-Forwarded-*
 * creduntur ex 127.0.0.1). Nucleus in exemplar_nucleus.c. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "reactor.h"
#include "hospitium.h"
#include "exemplar_nucleus.h"
#include "filum.h"
#include "chorda.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXEMPLAR_MORA_SISTENDI (X * M)

interior vacuum
_sistere (vacuum* datum)
{
    Exemplar* e = (Exemplar*)datum;

    fprintf(stderr, "exemplar: signum - sistere molliter (X s)\n");
    hospitium_sistere_molliter(exemplar_hospitium(e),
        EXEMPLAR_MORA_SISTENDI);
}

interior s32
_usus (vacuum)
{
    fprintf(stderr, "usus: exemplar -data <dir> [-portus N] "
        "[-hospes H] [-invitatio C | -invitatio-via F] [-procurator] "
        "[-acta]\n");
    redde II;
}

s32
principale (
      integer   argc,
    character** argv)
{
    ExemplarConfiguratio  cfg;
                 Piscina* p;
                 Reactor* r;
                Exemplar* e;
                 integer  k;
      constans character* via_invitationis = NIHIL;

    memset(&cfg, ZEPHYRUM, magnitudo(cfg));
    per (k = I; k < argc; k++)
    {
        si (strcmp(argv[k], "-data") == ZEPHYRUM && k + I < argc)
        {
            cfg.via_datorum = argv[++k];
        }
        alioquin si (   strcmp(argv[k], "-portus") == ZEPHYRUM
                     && k + I < argc)
        {
            cfg.portus = (i32)atoi(argv[++k]);
        }
        alioquin si (   strcmp(argv[k], "-hospes") == ZEPHYRUM
                     && k + I < argc)
        {
            cfg.hospes = argv[++k];
        }
        alioquin si (   strcmp(argv[k], "-invitatio") == ZEPHYRUM
                     && k + I < argc)
        {
            cfg.invitatio = argv[++k];
        }
        alioquin si (   strcmp(argv[k], "-invitatio-via") == ZEPHYRUM
                     && k + I < argc)
        {
            via_invitationis = argv[++k];
        }
        alioquin si (strcmp(argv[k], "-procurator") == ZEPHYRUM)
        {
            cfg.procurator_fidus = VERUM;
        }
        alioquin si (strcmp(argv[k], "-acta") == ZEPHYRUM)
        {
            cfg.acta_accessus = VERUM;
        }
        alioquin
        {
            redde _usus();
        }
    }
    si (!cfg.via_datorum)
    {
        redde _usus();
    }

    p = piscina_generare_dynamicum("exemplar", LXIV * MXXIV);
    r = p ? reactor_creare(p) : NIHIL;
    si (!r)
    {
        redde I;
    }
    si (via_invitationis)
    {
        chorda c = filum_legere_totum(via_invitationis, p);

        dum (   c.mensura > 0 && (c.datum[c.mensura - I] == '\n'
             || c.datum[c.mensura - I] == '\r'
             || c.datum[c.mensura - I] == ' '))
        {
            c.mensura--;
        }
        si (c.mensura == 0)
        {
            fprintf(stderr, "exemplar: invitatio %s legi nequit - "
                "inscriptio clausa\n", via_invitationis);
        }
        alioquin
        {
            cfg.invitatio = chorda_ut_cstr(c, p);
        }
    }
    cfg.reactor  = r;
    e            = exemplar_creare(&cfg, p);
    si (!e)
    {
        reactor_destruere(r);
        piscina_destruere(p);
        redde I;
    }
    (vacuum)reactor_signum_adicere(r, SIGTERM, _sistere, e);
    (vacuum)reactor_signum_adicere(r, SIGINT, _sistere, e);
    fprintf(stderr, "exemplar: http://%s:%u (data %s, inscriptio %s)\n",
        cfg.hospes ? cfg.hospes : "127.0.0.1",
        (insignatus integer)hospitium_portus(exemplar_hospitium(e)),
        cfg.via_datorum, cfg.invitatio ? "aperta" : "clausa");

    dum (hospitium_gressus(exemplar_hospitium(e), M))
    {
        /* ansa una: servus, labores, timeri, signa */
    }
    exemplar_claudere(e);
    reactor_destruere(r);
    piscina_destruere(p);
    fprintf(stderr, "exemplar: clausum\n");
    redde ZEPHYRUM;
}
