/* praeco.c - praeco colloquiorum Claude Code (colloquia-plan T2)
 *
 * USUS:
 *   praeco hamus             JSON hami ex stdin + TMUX_PANE/TMUX ->
 *                            liber (praeconium_via); exitus 0 SEMPER
 *                            (hamus numquam sessionem frangit)
 *   praeco status <textus>   eventus "status" ab agente (panellus ex
 *                            TMUX_PANE; sessio per T3 resolvitur)
 *   praeco cauda [n]         ultimi n eventus (XX) legibiles
 *
 * Tenue: omnis logica in lib/praeconium.c.
 */
/* plagula provenientiae (fabrica T7): '-provenientia' respondetur -
 * sine ea installatio (sanare installata) praeconem recusat */
/* <aedilis obiectum="build/fabrica/provenientia/praeco.c"/> */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "praeconium.h"
#include "provenientia.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

/* hamus legit quantum datur; JSON hami parvus est */
#define PRAECO_STDIN_MAXIMUM (IV * M * M)

interior s64
nunc_ms (
    vacuum)
{
    structura timeval t;

    si (gettimeofday(&t, NIHIL) != ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    redde (s64)t.tv_sec * (s64)M + (s64)(t.tv_usec / M);
}

interior chorda
stdin_legere (
    Piscina* piscina)
{
       chorda  c;
       size_t  n;
       size_t  r;
    character* alveus;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    alveus     = (character*)piscina_allocare(piscina,
        PRAECO_STDIN_MAXIMUM);
    si (!alveus)
    {
        redde c;
    }
    n = ZEPHYRUM;
    dum (n < PRAECO_STDIN_MAXIMUM)
    {
        r = fread(alveus + n, I, PRAECO_STDIN_MAXIMUM - n, stdin);
        si (r == ZEPHYRUM)
        {
            frange;
        }
        n += r;
    }
    c.datum    = (i8*)alveus;
    c.mensura  = (i32)n;
    redde c;
}

interior integer
hamus (
    Piscina* piscina)
{
    PraeconiumEventus e;

    si (praeconium_ex_hamo(stdin_legere(piscina), getenv("TMUX_PANE"),
            getenv("TMUX"), nunc_ms(), &e, piscina))
    {
        (vacuum)praeconium_scribere(praeconium_via(piscina), &e,
            piscina);
    }
    redde ZEPHYRUM;
}

interior integer
status (
                Piscina* piscina,
     constans character* textus)
{
     PraeconiumEventus  e;
    constans character* panellus;
    constans character* tmux;
                   i32  i;

    memset(&e, ZEPHYRUM, magnitudo(e));
    panellus    = getenv("TMUX_PANE");
    tmux        = getenv("TMUX");
    e.tempus    = nunc_ms();
    e.eventus   = chorda_ex_literis("status", piscina);
    e.panellus  = chorda_ex_literis(panellus ? panellus : "", piscina);
    e.servitor  = chorda_ex_literis(tmux ? tmux : "", piscina);
    per (i = ZEPHYRUM; i < e.servitor.mensura
        && e.servitor.datum[i] != ','; i++)
    {
    }
    e.servitor.mensura  = i;
    e.via               =
        chorda_ex_literis(getenv("PWD") ? getenv("PWD")
        : "", piscina);
    e.nuntius           = chorda_ex_literis(textus, piscina);
    si (!praeconium_scribere(praeconium_via(piscina), &e, piscina))
    {
        fprintf(stderr, "praeco: scribere non potui\n");
        redde I;
    }
    redde ZEPHYRUM;
}

interior integer
cauda (
    Piscina* piscina,
    integer  n)
{
                           Xar* l;
    constans PraeconiumEventus* e;
                           i32  i;
                           i32  primus;

    l = praeconium_caudam_legere(praeconium_via(piscina), LXIV * M,
        piscina);
    si (!l)
    {
        redde I;
    }
    primus = xar_numerus(l) > (i32)n ? xar_numerus(l) - (i32)n
        : ZEPHYRUM;
    per (i = primus; i < xar_numerus(l); i++)
    {
        e = (constans PraeconiumEventus*)xar_obtinere(l, i);
        imprimere("%ld %.*s %.*s %.*s %.*s %.*s\n", (long)e->tempus,
            (integer)e->eventus.mensura,
            (constans character*)e->eventus.datum,
            (integer)e->panellus.mensura,
            (constans character*)e->panellus.datum,
            (integer)e->genus.mensura,
            (constans character*)e->genus.datum,
            (integer)e->sessio.mensura,
            (constans character*)e->sessio.datum,
            (integer)e->nuntius.mensura,
            (constans character*)e->nuntius.datum);
    }
    redde ZEPHYRUM;
}

externus constans ProvenientiaRelatio provenientia_praeco;

integer
principale (
      integer   argc,
    character** argv)
{
    Piscina* piscina;

    si (provenientia_respondere((s32)argc, argv, &provenientia_praeco))
    {
        redde ZEPHYRUM;
    }
    piscina = piscina_generare_dynamicum("praeco", LXIV * M);
    si (!piscina || argc < II)
    {
        fprintf(stderr, "usus: praeco hamus | status <textus> | cauda "
            "[n]\n");
        /* hamus sine piscina aut argumentis: tamen 0 */
        redde argc >= II && strcmp(argv[I], "hamus") == ZEPHYRUM
            ? ZEPHYRUM : I;
    }
    si (strcmp(argv[I], "hamus") == ZEPHYRUM)
    {
        redde hamus(piscina);
    }
    si (strcmp(argv[I], "status") == ZEPHYRUM && argc >= III)
    {
        redde status(piscina, argv[II]);
    }
    si (strcmp(argv[I], "cauda") == ZEPHYRUM)
    {
        redde cauda(piscina, argc >= III ? atoi(argv[II]) : XX);
    }
    fprintf(stderr,
        "usus: praeco hamus | status <textus> | cauda [n]\n");
    redde I;
}
