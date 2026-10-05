/* scriba_applicatio.c - compositio scribae communis (S3; exemplar
 * pictor_applicatio). Ratio in capite. */

#include "scriba_applicatio.h"
#include "chorda.h"
#include "filum.h"
#include "stml.h"
#include "canon.h"
#include <stdio.h>
#include <string.h>

/* Modulus: glyphus fons_6x8 = cellula */
#define CELLULA_LATITUDO  VI
#define CELLULA_ALTITUDO  VIII
#define STATUS_LINEAE     I
#define FOLIUM_LATITUDO   TABULA_LATITUDO_DEFALTA
#define FOLIUM_ALTITUDO   TABULA_ALTITUDO_DEFALTA
#define INTERVALLUM       LXIV

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
        fprintf(stderr, "scriba: canon abest: %s\n", via);
        redde NIHIL;
    }
    c = canon_legere(fons, p, in, &causa);
    si (!c)
    {
        fprintf(stderr, "scriba: canon malus %s: %.*s\n", via,
                (int)causa.mensura, causa.datum);
    }
    redde c;
}

Volumen*
scriba_volumen_aperire (
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
        redde volumen_temporarium(piscina, "scriba_fumus");
    }
    redde volumen_aperire_aut_creare(piscina,
        via_voluminis ? via_voluminis : "scriba.volumen");
}

b32
scriba_applicatio_aedificare (
       ScribaApplicatio* app,
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     constans character* radix,
                    i32  latitudo,
                    i32  altitudo)
{
                        chorda  domini;
                  StmlResultus  res;
                        chorda  durabilis_initialis;
    constans TabulaCharacterum* folium;

    si (!app || !piscina || !intern || !volumen)
    {
        redde FALSUM;
    }
    memset(app, ZEPHYRUM, magnitudo(ScribaApplicatio));
    app->piscina  = piscina;
    app->intern   = intern;
    app->volumen  = volumen;

    /* documentum: exsistens aut novum */
    app->doc = scriba_documentum_aperire(piscina, intern, volumen);
    si (!app->doc)
    {
        app->doc = scriba_documentum_creare(piscina, intern, volumen,
            FOLIUM_LATITUDO, FOLIUM_ALTITUDO, INTERVALLUM);
    }
    si (!app->doc)
    {
        fprintf(stderr, "scriba: documentum\n");
        redde FALSUM;
    }
    folium = scriba_documentum_tabula(app->doc);

    /* insulae + canones + domini */
    durabilis_initialis = chorda_ex_literis("<documentum latitudo=\"",
        piscina);
    durabilis_initialis = chorda_concatenare(durabilis_initialis,
        chorda_ex_s32((s32)folium->latitudo, piscina), piscina);
    durabilis_initialis = chorda_concatenare(durabilis_initialis,
        chorda_ex_literis("\" altitudo=\"", piscina), piscina);
    durabilis_initialis = chorda_concatenare(durabilis_initialis,
        chorda_ex_s32((s32)folium->altitudo, piscina), piscina);
    durabilis_initialis = chorda_concatenare(durabilis_initialis,
        chorda_ex_literis("\"/>", piscina), piscina);
    app->repo = insula_repositorium_creare(piscina, intern,
        chorda_ut_cstr(durabilis_initialis, piscina),
        "<ephemera focus=\"pagina\" modus=\"normalis\"/>");
    si (!app->repo)
    {
        redde FALSUM;
    }
    insula_ponere_canonem(app->repo, INSULA_DURABILIS,
        canonem_legere(piscina, intern, _via(piscina, radix,
        "apps/scriba/canones/durabilis.canon")));
    insula_ponere_canonem(app->repo, INSULA_EPHEMERA,
        canonem_legere(piscina, intern, _via(piscina, radix,
        "apps/scriba/canones/ephemera.canon")));
    domini = filum_legere_totum(_via(piscina, radix,
        "apps/scriba/canones/domini.stml"), piscina);
    res = stml_legere_ex_literis(chorda_ut_cstr(domini, piscina),
        piscina, intern);
    si (res.successus)
    {
        insula_dominos_legere(app->repo, INSULA_EPHEMERA,
            res.elementum_radix);
        insula_dominos_legere(app->repo, INSULA_DURABILIS,
            res.elementum_radix);
    }

    /* registra, dispensator, gestus */
    app->actiones = actio_registrum_creare(piscina, intern);
    scriba_actiones_initiare(&app->actiones_ctx, app->doc, piscina);
    scriba_actiones_registrare(app->actiones, &app->actiones_ctx);
    app->figurae         = figura_registrum_creare(piscina);
    app->figurae_ctx.sa  = &app->actiones_ctx;
    scriba_figurae_registrare(app->figurae, ZEPHYRUM,
        &app->figurae_ctx);
    app->compositio.fenestra_latitudo  = latitudo;
    app->compositio.fenestra_altitudo  = altitudo;
    app->compositio.cellula_latitudo   = CELLULA_LATITUDO;
    app->compositio.cellula_altitudo   = CELLULA_ALTITUDO;
    app->compositio.status_lineae      = STATUS_LINEAE;
    app->d = dispensator_creare(piscina, intern, app->repo,
        app->actiones, scriba_componere, &app->compositio, CCC);
    si (!app->d)
    {
        redde FALSUM;
    }
    scriba_gestum_ponere(dispensator_motus(app->d), &app->actiones_ctx);
    redde VERUM;
}
