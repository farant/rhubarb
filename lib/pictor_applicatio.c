/* pictor_applicatio.c - compositio pictoris communis (A4). Codex ex
 * pictor.c translatus; ratio in capite. */

#include "pictor_applicatio.h"
#include "chorda.h"
#include "filum.h"
#include "stml.h"
#include "canon.h"
#include <stdio.h>
#include <string.h>

/* Modulus: glyphus fons_6x8 = cellula */
#define CELLULA_LATITUDO  VI
#define CELLULA_ALTITUDO  VIII
#define STATUS_LINEAE     I      /* Franus 2026-10-03: linea una */
#define DOC_LATITUDO      CCCXX
#define DOC_ALTITUDO      CC

/* radix + "/" + via (radix NIHIL aut vacua: via sola) */
interior constans character*
_via (
               Piscina* p,
    constans character* radix,
    constans character* via)
{
    chorda c;

    si (!radix || radix[ZEPHYRUM] == '\0')
    {
        redde via;
    }
    c = chorda_concatenare(chorda_ex_literis(radix, p),
        chorda_ex_literis("/", p), p);
    c = chorda_concatenare(c, chorda_ex_literis(via, p), p);
    redde chorda_ut_cstr(c, p);
}

interior Canon*
canonem_legere (
                Piscina* p,
    InternamentumChorda* in,
     constans character* via)
{
    chorda  fons;
    chorda  causa;
     Canon* c;

    fons = filum_legere_totum(via, p);
    si (fons.mensura == ZEPHYRUM)
    {
        fprintf(stderr, "pictor: canon abest: %s\n", via);
        redde NIHIL;
    }
    c = canon_legere(fons, p, in, &causa);
    si (!c)
    {
        fprintf(stderr, "pictor: canon malus %s: %.*s\n", via,
                (int)causa.mensura, causa.datum);
    }
    redde c;
}

Volumen*
pictor_volumen_aperire (
       Piscina*  piscina,
           s32   argc,
     character** argv,
           b32*  fumus)
{
     constans character* via_voluminis = NIHIL;
                    s32  i;

    *fumus = FALSUM;
    per (i = I; i < argc; i++)
    {
        si (strcmp(argv[i], "-fumus") == ZEPHYRUM)
        {
            *fumus = VERUM;
        }
        alioquin si (   strcmp(argv[i], "-volumen") == ZEPHYRUM
                     && i + I < argc)
        {
            i++;
            via_voluminis = argv[i];
        }
    }
    si (*fumus)
    {
        redde volumen_temporarium(piscina, "pictor_fumus");
    }
    redde volumen_aperire_aut_creare(piscina,
        via_voluminis ? via_voluminis : "pictor.volumen");
}

b32
pictor_applicatio_aedificare (
       PictorApplicatio* app,
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     constans character* radix,
                    i32  latitudo,
                    i32  altitudo)
{
          chorda domini;
    StmlResultus res;
          chorda ephemera_initialis;
          chorda durabilis_initialis;

    si (!app || !piscina || !intern || !volumen)
    {
        redde FALSUM;
    }
    memset(app, ZEPHYRUM, magnitudo(PictorApplicatio));
    app->piscina  = piscina;
    app->intern   = intern;
    app->volumen  = volumen;

    /* documentum */
    app->doc = pictor_documentum_aperire(piscina, intern, volumen);
    si (!app->doc)
    {
        app->doc = pictor_documentum_creare(piscina, intern, volumen,
            DOC_LATITUDO, DOC_ALTITUDO, LXIV);
    }
    si (!app->doc)
    {
        fprintf(stderr, "pictor: documentum\n");
        redde FALSUM;
    }

    /* insulae + canones + domini */
    durabilis_initialis = chorda_ex_literis("<documentum latitudo=\"",
        piscina);
    durabilis_initialis = chorda_concatenare(durabilis_initialis,
        chorda_ex_s32((s32)app->doc->latitudo, piscina), piscina);
    durabilis_initialis = chorda_concatenare(durabilis_initialis,
        chorda_ex_literis("\" altitudo=\"", piscina), piscina);
    durabilis_initialis = chorda_concatenare(durabilis_initialis,
        chorda_ex_s32((s32)app->doc->altitudo, piscina), piscina);
    durabilis_initialis = chorda_concatenare(durabilis_initialis,
        chorda_ex_literis("\"/>", piscina), piscina);
    ephemera_initialis = chorda_ex_literis(
        "<ephemera instrumentum=\"penicillus\" color_primus=\"0\""
        " color_secundus=\"5\" magnitudo=\"1\" zoom=\"1\""
        " focus=\"tabula\"/>", piscina);
    app->repo = insula_repositorium_creare(piscina, intern,
        chorda_ut_cstr(durabilis_initialis, piscina),
        chorda_ut_cstr(ephemera_initialis, piscina));
    si (!app->repo)
    {
        redde FALSUM;
    }
    insula_ponere_canonem(app->repo, INSULA_DURABILIS,
        canonem_legere(piscina, intern, _via(piscina, radix,
        "apps/pictor/canones/durabilis.canon")));
    insula_ponere_canonem(app->repo, INSULA_EPHEMERA,
        canonem_legere(piscina, intern, _via(piscina, radix,
        "apps/pictor/canones/ephemera.canon")));
    domini = filum_legere_totum(_via(piscina, radix,
        "apps/pictor/canones/domini.stml"), piscina);
    res = stml_legere_ex_literis(chorda_ut_cstr(domini, piscina),
        piscina, intern);
    si (res.successus)
    {
        insula_dominos_legere(app->repo, INSULA_EPHEMERA,
            res.elementum_radix);
        insula_dominos_legere(app->repo, INSULA_DURABILIS,
            res.elementum_radix);
    }

    /* registra + dispensator */
    app->actiones          = actio_registrum_creare(piscina, intern);
    app->actiones_ctx.doc  = app->doc;
    pictor_actiones_registrare(app->actiones, &app->actiones_ctx);
    app->figurae = figura_registrum_creare(piscina);
    app->figurae_ctx.doc = app->doc;
    app->figurae_ctx.cellula_latitudo = CELLULA_LATITUDO;
    app->figurae_ctx.cellula_altitudo = CELLULA_ALTITUDO;
    pictor_figurae_registrare(app->figurae, ZEPHYRUM,
        &app->figurae_ctx);
    app->compositio.fenestra_latitudo  = latitudo;
    app->compositio.fenestra_altitudo  = altitudo;
    app->compositio.cellula_latitudo   = CELLULA_LATITUDO;
    app->compositio.cellula_altitudo   = CELLULA_ALTITUDO;
    app->compositio.status_lineae      = STATUS_LINEAE;
    app->d = dispensator_creare(piscina, intern, app->repo,
        app->actiones, pictor_componere, &app->compositio, CCC);
    redde app->d ? VERUM : FALSUM;
}
