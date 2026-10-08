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
/* folium novum (vicus-latera S2c): magnitudine superficiei, non hoc
 * minimo minus */
#define FOLIUM_LATITUDO_MINIMA  XX
#define FOLIUM_ALTITUDO_MINIMA  X
#define INTERVALLUM       LXIV

/* cellulae superficiei minus margines (cellula utrimque) et, in
 * altitudine, lineae status; minimum servatur */
interior i32
folii_dimensio (
    i32 pixela,
    s32 cellula,
    s32 demendum,
    s32 minimum)
{
    s32 n;

    n = (s32)pixela / cellula - demendum;
    redde (i32)(n < minimum ? minimum : n);
}

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

/* elementum initiale: <scriba [id="…"] attributa/> */
interior constans character*
elementum (
                Piscina* piscina,
     constans character* id,
                 chorda  attributa)
{
    chorda c;

    c = chorda_ex_literis("<scriba", piscina);
    si (id)
    {
        c = chorda_concatenare(c, chorda_ex_literis(" id=\"", piscina),
                               piscina);
        c = chorda_concatenare(c, chorda_ex_literis(id, piscina),
            piscina);
        c = chorda_concatenare(c, chorda_ex_literis("\"", piscina),
                               piscina);
    }
    c = chorda_concatenare(c, attributa, piscina);
    c = chorda_concatenare(c, chorda_ex_literis("/>", piscina),
        piscina);
    redde chorda_ut_cstr(c, piscina);
}

b32
scriba_montare (
         ScribaMontatio* m,
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     InsulaRepositorium* repo,
     constans character* id,
     constans character* radix,
                    i32  latitudo,
                    i32  altitudo)
{
                        chorda  domini;
                  StmlResultus  res;
                        chorda  attributa;
    constans TabulaCharacterum* folium;
            constans character* spatium;

    si (!m || !piscina || !intern || !volumen || !repo)
    {
        redde FALSUM;
    }
    memset(m, ZEPHYRUM, magnitudo(ScribaMontatio));
    spatium = id ? id : "";

    /* documentum: exsistens aut novum, in spatio montationis */
    m->doc = scriba_documentum_aperire(piscina, intern, volumen,
        spatium);
    si (!m->doc)
    {
        m->doc = scriba_documentum_creare(piscina, intern, volumen,
            spatium,
            folii_dimensio(latitudo, CELLULA_LATITUDO, II,
                FOLIUM_LATITUDO_MINIMA),
            folii_dimensio(altitudo, CELLULA_ALTITUDO, II
                + STATUS_LINEAE,
                FOLIUM_ALTITUDO_MINIMA),
            INTERVALLUM);
    }
    si (!m->doc)
    {
        fprintf(stderr, "scriba: documentum\n");
        redde FALSUM;
    }
    folium   = scriba_documentum_tabula(m->doc);
    m->ramus = id ? insula_ramus(repo, "scriba", id)
                  : insula_ramus_radix(repo);

    /* canones PRIMUM: hospes elementum montatum numquam videt */
    insula_ramus_canonem_ponere(&m->ramus, INSULA_DURABILIS,
        canonem_legere(piscina, intern, _via(piscina, radix,
        "apps/scriba/canones/durabilis.canon")));
    insula_ramus_canonem_ponere(&m->ramus, INSULA_EPHEMERA,
        canonem_legere(piscina, intern, _via(piscina, radix,
        "apps/scriba/canones/ephemera.canon")));

    /* elementum initiale */
    attributa = chorda_ex_literis(" latitudo=\"", piscina);
    attributa = chorda_concatenare(attributa,
        chorda_ex_s32((s32)folium->latitudo, piscina), piscina);
    attributa = chorda_concatenare(attributa,
        chorda_ex_literis("\" altitudo=\"", piscina), piscina);
    attributa = chorda_concatenare(attributa,
        chorda_ex_s32((s32)folium->altitudo, piscina), piscina);
    attributa = chorda_concatenare(attributa,
        chorda_ex_literis("\"", piscina), piscina);
    si (   !insula_ramum_initiare(&m->ramus, INSULA_DURABILIS,
               elementum(piscina, id, attributa))
        || !insula_ramum_initiare(&m->ramus, INSULA_EPHEMERA,
               elementum(piscina, id, chorda_ex_literis(
                   " modus=\"normalis\" focus=\"pagina\"", piscina))))
    {
        fprintf(stderr, "scriba: elementum initiale: %.*s\n",
                (int)insula_causa(repo).mensura,
                insula_causa(repo).datum);
        redde FALSUM;
    }

    /* domini POST initiationem */
    domini = filum_legere_totum(_via(piscina, radix,
        "apps/scriba/canones/domini.stml"), piscina);
    res = stml_legere_ex_literis(chorda_ut_cstr(domini, piscina),
        piscina, intern);
    si (res.successus)
    {
        (vacuum)insula_ramus_dominos_legere(&m->ramus, INSULA_EPHEMERA,
            res.elementum_radix);
        (vacuum)insula_ramus_dominos_legere(&m->ramus, INSULA_DURABILIS,
            res.elementum_radix);
    }

    /* contextus et registra propria */
    m->actiones = actio_registrum_creare(piscina, intern);
    scriba_actiones_initiare(&m->actiones_ctx, m->doc, piscina);
    m->actiones_ctx.ramus = m->ramus;
    scriba_actiones_registrare(m->actiones, &m->actiones_ctx);
    m->figurae         = figura_registrum_creare(piscina);
    m->figurae_ctx.sa  = &m->actiones_ctx;
    scriba_figurae_registrare(m->figurae, ZEPHYRUM, &m->figurae_ctx);
    m->compositio.fenestra_latitudo  = latitudo;
    m->compositio.fenestra_altitudo  = altitudo;
    m->compositio.cellula_latitudo   = CELLULA_LATITUDO;
    m->compositio.cellula_altitudo   = CELLULA_ALTITUDO;
    m->compositio.status_lineae      = STATUS_LINEAE;
    m->compositio.ramus              = m->ramus;
    redde VERUM;
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
    si (!app || !piscina || !intern || !volumen)
    {
        redde FALSUM;
    }
    memset(app, ZEPHYRUM, magnitudo(ScribaApplicatio));
    app->piscina  = piscina;
    app->intern   = intern;
    app->volumen  = volumen;

    /* repositorium cuius radix IPSA scriba est; montatio in radice */
    app->repo = insula_repositorium_creare(piscina, intern, "<scriba/>",
        "<scriba focus=\"pagina\"/>");
    si (   !app->repo
        || !scriba_montare(&app->montatio, piscina, intern, volumen,
               app->repo, NIHIL, radix, latitudo, altitudo))
    {
        redde FALSUM;
    }
    app->doc       = app->montatio.doc;
    app->actiones  = app->montatio.actiones;
    app->figurae   = app->montatio.figurae;
    app->d = dispensator_creare(piscina, intern, app->repo,
        app->montatio.actiones, scriba_componere,
        &app->montatio.compositio, CCC);
    si (!app->d)
    {
        redde FALSUM;
    }
    scriba_gestum_ponere(dispensator_motus(app->d),
                         &app->montatio.actiones_ctx);
    redde VERUM;
}
