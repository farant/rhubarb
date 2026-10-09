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
#define STATUS_LINEAE     III    /* P1a (Franus 2026-10-09): linea
                                  * instrumentorum, quadrata XX x XX;
                                  * olim linea una (2026-10-03) */
/* tabula nova (vicus-latera S2c): prospectus superficiei minus margo
 * cellulae utrimque (ut folium scribae; altitudo etiam minus linea
 * status), non hoc minimo minus */
#define DOC_MINIMUM       LXIV

interior i32
tabulae_dimensio (
    i32 pixela,
    s32 demendum)
{
    s32 n;

    n = (s32)pixela - demendum;
    redde (i32)(n < DOC_MINIMUM ? DOC_MINIMUM : n);
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

/* elementum initiale: <pictor [id="…"] attributa/> */
interior constans character*
elementum (
               Piscina* piscina,
    constans character* id,
    constans character* attributa)
{
    chorda c;

    c = chorda_ex_literis("<pictor", piscina);
    si (id)
    {
        c = chorda_concatenare(c, chorda_ex_literis(" id=\"", piscina),
                               piscina);
        c = chorda_concatenare(c, chorda_ex_literis(id, piscina),
            piscina);
        c = chorda_concatenare(c, chorda_ex_literis("\"", piscina),
                               piscina);
    }
    c = chorda_concatenare(c, chorda_ex_literis(attributa, piscina),
                           piscina);
    c = chorda_concatenare(c, chorda_ex_literis("/>", piscina),
        piscina);
    redde chorda_ut_cstr(c, piscina);
}

b32
pictor_montare (
         PictorMontatio* m,
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
                 chorda  durabilis;
     constans character* spatium;

    si (!m || !piscina || !intern || !volumen || !repo)
    {
        redde FALSUM;
    }
    memset(m, ZEPHYRUM, magnitudo(PictorMontatio));
    spatium = id ? id : "";

    /* documentum: exsistens aut novum, in spatio montationis */
    m->doc = pictor_documentum_aperire(piscina, intern, volumen,
        spatium);
    si (!m->doc)
    {
        m->doc = pictor_documentum_creare(piscina, intern, volumen,
            spatium, tabulae_dimensio(latitudo, II * CELLULA_LATITUDO),
            tabulae_dimensio(altitudo,
            (STATUS_LINEAE + II) * CELLULA_ALTITUDO),
            LXIV);
    }
    si (!m->doc)
    {
        fprintf(stderr, "pictor: documentum\n");
        redde FALSUM;
    }
    m->ramus = id ? insula_ramus(repo, "pictor", id)
                  : insula_ramus_radix(repo);

    /* canones PRIMUM: hospes elementum montatum numquam videt */
    insula_ramus_canonem_ponere(&m->ramus, INSULA_DURABILIS,
        canonem_legere(piscina, intern, _via(piscina, radix,
        "apps/pictor/canones/durabilis.canon")));
    insula_ramus_canonem_ponere(&m->ramus, INSULA_EPHEMERA,
        canonem_legere(piscina, intern, _via(piscina, radix,
        "apps/pictor/canones/ephemera.canon")));

    /* elementum initiale */
    durabilis = chorda_ex_literis(" latitudo=\"", piscina);
    durabilis = chorda_concatenare(durabilis,
        chorda_ex_s32((s32)m->doc->latitudo, piscina), piscina);
    durabilis = chorda_concatenare(durabilis,
        chorda_ex_literis("\" altitudo=\"", piscina), piscina);
    durabilis = chorda_concatenare(durabilis,
        chorda_ex_s32((s32)m->doc->altitudo, piscina), piscina);
    durabilis = chorda_concatenare(durabilis,
        chorda_ex_literis("\"", piscina), piscina);
    si (   !insula_ramum_initiare(&m->ramus, INSULA_DURABILIS,
               elementum(piscina, id, chorda_ut_cstr(durabilis,
               piscina)))
        || !insula_ramum_initiare(&m->ramus, INSULA_EPHEMERA,
               elementum(piscina, id,
                   " instrumentum=\"penicillus\" color_primus=\"0\""
                   " color_secundus=\"-1\" magnitudo=\"1\" zoom=\"1\""
                   " focus=\"tabula\"")))
    {
        fprintf(stderr, "pictor: elementum initiale: %.*s\n",
                (int)insula_causa(repo).mensura,
                insula_causa(repo).datum);
        redde FALSUM;
    }

    /* domini POST initiationem */
    domini = filum_legere_totum(_via(piscina, radix,
        "apps/pictor/canones/domini.stml"), piscina);
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
    m->actiones            = actio_registrum_creare(piscina, intern);
    m->actiones_ctx.doc    = m->doc;
    m->actiones_ctx.ramus  = m->ramus;
    pictor_actiones_registrare(m->actiones, &m->actiones_ctx);
    m->figurae                       = figura_registrum_creare(piscina);
    m->figurae_ctx.doc               = m->doc;
    m->figurae_ctx.cellula_latitudo  = CELLULA_LATITUDO;
    m->figurae_ctx.cellula_altitudo  = CELLULA_ALTITUDO;
    pictor_figurae_registrare(m->figurae, ZEPHYRUM, &m->figurae_ctx);
    m->compositio.fenestra_latitudo  = latitudo;
    m->compositio.fenestra_altitudo  = altitudo;
    m->compositio.cellula_latitudo   = CELLULA_LATITUDO;
    m->compositio.cellula_altitudo   = CELLULA_ALTITUDO;
    m->compositio.status_lineae      = STATUS_LINEAE;
    m->compositio.ramus              = m->ramus;
    redde VERUM;
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
    si (!app || !piscina || !intern || !volumen)
    {
        redde FALSUM;
    }
    memset(app, ZEPHYRUM, magnitudo(PictorApplicatio));
    app->piscina  = piscina;
    app->intern   = intern;
    app->volumen  = volumen;

    /* repositorium cuius radix IPSA pictor est; montatio in radice */
    app->repo = insula_repositorium_creare(piscina, intern, "<pictor/>",
        "<pictor focus=\"tabula\"/>");
    si (   !app->repo
        || !pictor_montare(&app->montatio, piscina, intern, volumen,
               app->repo, NIHIL, radix, latitudo, altitudo))
    {
        redde FALSUM;
    }
    app->doc       = app->montatio.doc;
    app->actiones  = app->montatio.actiones;
    app->figurae   = app->montatio.figurae;
    app->d = dispensator_creare(piscina, intern, app->repo,
        app->montatio.actiones, pictor_componere,
        &app->montatio.compositio, CCC);
    redde app->d ? VERUM : FALSUM;
}
