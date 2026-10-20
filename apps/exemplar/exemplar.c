/* exemplar.c - applicatio probans hospitii v3 (exemplar.golden.earth)
 *
 *   exemplar -data <dir> [-portus N] [-hospes 127.0.0.1]
 *            [-procurator] [-acta] [-basis URL] [-sonda]
 *   exemplar -data <dir> -invitare "<nota>" [-basis URL]
 *
 * -invitare (ianua P1): invitatio una (VII dies) ex linea mandatorum,
 * servo SISTENTE (sera data/sera; currens -> exitus II). Codex et
 * nexus ad stdout SOLUM - numquam ad acta aut stderr. Invitationes
 * ceterae per paginam /invitationes.
 *
 * Principale tenue: argumenta, reactor UNUS (servus, labores
 * argon2id, signa), SIGTERM/SIGINT -> sistere molliter (X s), deinde
 * volumen clausum et exitus 0. -procurator: post nginx (X-Forwarded-*
 * creduntur ex 127.0.0.1). -basis: URL publica (auctoritas OAuth,
 * resource MCP), e.g. https://exemplar.golden.earth - sine ea
 * http://<hospes>:<portus>. -sonda: petitiones /mcp, /.well-known/...,
 * /oauth/... ad stderr (capita secreta redacta; mcp-remotus T1).
 * Nucleus in exemplar_nucleus.c, MCP/OAuth in exemplar_mcp.c. */
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
        "[-hospes H] [-procurator] [-acta] [-basis URL] [-sonda]\n"
        "       exemplar -data <dir> -invitare NOTA [-basis URL]\n");
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
      constans character* nota_invitationis = NIHIL;

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
        alioquin si (   strcmp(argv[k], "-invitare") == ZEPHYRUM
                     && k + I < argc)
        {
            nota_invitationis = argv[++k];
        }
        alioquin si (strcmp(argv[k], "-procurator") == ZEPHYRUM)
        {
            cfg.procurator_fidus = VERUM;
        }
        alioquin si (strcmp(argv[k], "-acta") == ZEPHYRUM)
        {
            cfg.acta_accessus = VERUM;
        }
        alioquin si (   strcmp(argv[k], "-basis") == ZEPHYRUM
                     && k + I < argc)
        {
            cfg.basis = argv[++k];
        }
        alioquin si (strcmp(argv[k], "-sonda") == ZEPHYRUM)
        {
            cfg.sonda = VERUM;
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
    si (!p)
    {
        redde I;
    }
    si (nota_invitationis)
    {
        chorda codex;
           s32 exitus = exemplar_invitare(cfg.via_datorum,
                            nota_invitationis, &codex, p);

        si (exitus == II)
        {
            fprintf(stderr, "exemplar: servus currit - siste prius "
                "(sera %s/sera)\n", cfg.via_datorum);
        }
        alioquin si (exitus == ZEPHYRUM)
        {
            /* stdout SOLUM: codex numquam in actis */
            printf("codex: %.*s\n", (integer)codex.mensura,
                (constans character*)codex.datum);
            si (cfg.basis)
            {
                printf("nexus: %s/inscribere?invitatio=%.*s\n",
                    cfg.basis,
                    (integer)codex.mensura,
                    (constans character*)codex.datum);
            }
        }
        alioquin
        {
            fprintf(stderr, "exemplar: invitatio creari nequit\n");
        }
        piscina_destruere(p);
        redde exitus;
    }
    r = reactor_creare(p);
    si (!r)
    {
        redde I;
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
    fprintf(stderr, "exemplar: http://%s:%u (data %s)\n",
        cfg.hospes ? cfg.hospes : "127.0.0.1",
        (insignatus integer)hospitium_portus(exemplar_hospitium(e)),
        cfg.via_datorum);

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
