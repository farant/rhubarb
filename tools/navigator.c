/* navigator.c - instrumentum navigatoris (navigator N2.5): articulum
 * petere et servare, articulos servatos enumerare. Volumen SEMPER
 * datur (-volumen) - volumen verum vici numquam tangitur.
 *
 * USUS:
 *   navigator petere <URL> -volumen <via>
 *                            petit (User-Agent ex ~/.rhubarb/navigator/
 *                            agens), servat; exitus et versionem
 *                            imprimit; exitus 0 si servatum
 *   navigator index -volumen <via>
 *                            articuli servati et versiones eorum
 *
 * Tenue: logica in lib/navigator_petitio.c et
 * lib/navigator_bibliotheca.c. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "reactor.h"
#include "volumen.h"
#include "navigator_bibliotheca.h"
#include "navigator_petitio.h"
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

nomen structura {
                         b32 finita;
    NavigatorPetitioResultus resultus;
} Exspectatio;

interior s64
ms_nunc (vacuum)
{
    structura timeval tv;

    gettimeofday(&tv, NIHIL);
    redde (s64)tv.tv_sec * M + (s64)tv.tv_usec / M;
}

interior vacuum
finis (
                               vacuum* datum,
    constans NavigatorPetitioResultus* r)
{
    Exspectatio* e = (Exspectatio*)datum;

    e->finita    = VERUM;
    e->resultus  = *r;
}

interior constans character*
status_nomen (
    NavigatorPetitioStatus s)
{
    commutatio (s)
    {
        casus NAVIGATOR_PETITIO_SERVATA:     redde "servata";
        casus NAVIGATOR_PETITIO_IGNOTA:      redde "situs ignotus";
        casus NAVIGATOR_PETITIO_NON_INVENTA: redde "non inventa";
        casus NAVIGATOR_PETITIO_RETE:        redde "rete";
        casus NAVIGATOR_PETITIO_ABRUPTA:     redde "abrupta";
        ordinarius:                          redde "?";
    }
}

interior constans character*
exitus_nomen (
    NavigatorExitus x)
{
    commutatio (x)
    {
        casus NAVIGATOR_NOVUM:        redde "novum (versio prima)";
        casus NAVIGATOR_VERSIO_NOVA:  redde "versio nova";
        casus NAVIGATOR_IDEM:         redde "idem (nihil scriptum)";
        ordinarius:                   redde "vitium";
    }
}

interior vacuum
versionem_imprimere (
    constans NavigatorVersio* v)
{
    imprimere("    %.*s  %.*s  %.*s\n      %.*s\n",
        (integer)v->identitas.mensura,
        (constans character*)v->identitas.datum,
        (integer)v->momentum.mensura,
        (constans character*)v->momentum.datum,
        (integer)v->titulus.mensura,
        (constans character*)v->titulus.datum,
        (integer)v->url.mensura, (constans character*)v->url.datum);
}

interior s32
petere (
                 Volumen* volumen,
    NavigatorBibliotheca* b,
      constans character* url,
                 Piscina* piscina)
{
                        Reactor* r;
               NavigatorPetitio* np;
       NavigatorPetitioOptiones  opt;
                    Exspectatio  e;
                            s64  initium;
    constans NavigatorArticulus* a;

    (vacuum)volumen;
    r = reactor_creare(piscina);
    memset(&opt, ZEPHYRUM, magnitudo(opt));
    memset(&e, ZEPHYRUM, magnitudo(e));
    opt.agens = navigator_agens_legere(NIHIL, piscina);
    imprimere("User-Agent: %.*s\n", (integer)opt.agens.mensura,
        (constans character*)opt.agens.datum);
    initium = ms_nunc();
    np = navigator_petere(r, b, chorda_ex_literis(url, piscina), finis,
        &e, &opt);
    si (!np)
    {
        fprintf(stderr, "navigator: petitio non incepit\n");
        redde I;
    }
    dum (!e.finita)
    {
        (vacuum)reactor_poll(r, C);
    }
    imprimere("status: %s (%u ms)\n", status_nomen(e.resultus.status),
        (insignatus integer)(ms_nunc() - initium));
    si (e.resultus.status != NAVIGATOR_PETITIO_SERVATA)
    {
        imprimere("causa: %.*s\n",
            (integer)e.resultus.descriptio.mensura,
            (constans character*)e.resultus.descriptio.datum);
        navigator_petitionem_dimittere(np);
        redde I;
    }
    a = e.resultus.articulus;
    imprimere("exitus: %s\n", exitus_nomen(e.resultus.exitus));
    si (a)
    {
        imprimere("articulus: %.*s (%.*s), versiones %u\n",
            (integer)a->url.mensura, (constans character*)a->url.datum,
            (integer)a->identitas.mensura,
            (constans character*)a->identitas.datum,
            (insignatus integer)xar_numerus(a->versiones));
    }
    si (e.resultus.versio)
    {
        versionem_imprimere(e.resultus.versio);
    }
    navigator_petitionem_dimittere(np);
    redde ZEPHYRUM;
}

interior s32
indicem_imprimere (
    NavigatorBibliotheca* b)
{
    i32 i;
    i32 j;

    per (i = ZEPHYRUM; i < navigator_articulorum_numerus(b); i++)
    {
        constans NavigatorArticulus* a = navigator_articulus(b, i);

        imprimere("%.*s  [%.*s]\n", (integer)a->url.mensura,
            (constans character*)a->url.datum,
            (integer)a->situs.mensura,
            (constans character*)a->situs.datum);
        per (j = ZEPHYRUM; j < xar_numerus(a->versiones); j++)
        {
            versionem_imprimere((constans NavigatorVersio*)xar_obtinere(
                a->versiones, j));
        }
    }
    imprimere("articuli: %u\n",
        (insignatus integer)navigator_articulorum_numerus(b));
    redde ZEPHYRUM;
}

interior vacuum
usum_imprimere (vacuum)
{
    fprintf(stderr,
        "usus: navigator petere <URL> -volumen <via>\n"
        "      navigator index -volumen <via>\n");
}

s32
principale (
         integer   argc,
       character** argv)
{
                 Piscina* piscina;
                 Volumen* volumen;
    NavigatorBibliotheca* b;
      constans character* via = NIHIL;
      constans character* url = NIHIL;
                 integer  k;
                     s32  exitus;

    per (k = II; k < argc; k++)
    {
        si (strcmp(argv[k], "-volumen") == ZEPHYRUM && k + I < argc)
        {
            via = argv[++k];
        }
        alioquin si (!url)
        {
            url = argv[k];
        }
    }
    si (   argc < II || !via || (strcmp(argv[I], "petere") == ZEPHYRUM
        && !url) || (strcmp(argv[I], "petere") != ZEPHYRUM
        && strcmp(argv[I], "index") != ZEPHYRUM))
    {
        usum_imprimere();
        redde II;
    }
    piscina = piscina_generare_dynamicum("navigator", M * M);
    volumen = volumen_aperire_aut_creare(piscina, via);
    si (!volumen)
    {
        fprintf(stderr, "navigator: volumen %s aperiri non potest\n",
            via);
        redde I;
    }
    b = navigator_bibliotheca_aperire(volumen, NIHIL, NIHIL, piscina);
    exitus = strcmp(argv[I], "petere") == ZEPHYRUM
        ? petere(volumen, b, url, piscina) : indicem_imprimere(b);
    volumen_claudere(volumen);
    piscina_destruere(piscina);
    redde exitus;
}
