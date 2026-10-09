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

nomen structura {
    s32 positio;
    s32 numerus;
} IndexPaginae;

interior vacuum
index_mutator (
              StmlNodus* nodus,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    constans IndexPaginae* indicium;

    indicium = (constans IndexPaginae*)ctx;
    insula_attributum_ponere(nodus, p, in, "pagina_positio",
        chorda_ut_cstr(chorda_ex_s32(indicium->positio, p), p));
    insula_attributum_ponere(nodus, p, in, "paginae_numerus",
        chorda_ut_cstr(chorda_ex_s32(indicium->numerus, p), p));
}

/* S2b: index paginae in ramo ephemero (linea status) - pagina visus
 * (plagula) et numerus libri. Sine coactione solum si numerus mutatus
 * (visus alius paginam creavit); VERUM si scriptum. */
interior b32
paginam_indicare (
    ScribaMontatio* m,
               b32  cogere)
{
    IndexPaginae  indicium;
          chorda  pagina;
          chorda* a;
             s32  vetus;
             b32  inventum;

    si (!m->liber)
    {
        redde FALSUM;
    }
    indicium.numerus = (s32)scriba_liber_numerus(m->liber);
    a = insula_ramus_attributum(&m->ramus, INSULA_EPHEMERA,
        "paginae_numerus");
    si (   !cogere && a && chorda_ut_s32(*a, &vetus)
        && vetus == indicium.numerus)
    {
        redde FALSUM;
    }
    pagina = volumen_plagulam_promere(m->doc->volumen,
        m->actiones_ctx.visus, m->doc->piscina, &inventum);
    indicium.positio = (inventum ? scriba_liber_index(m->liber, pagina)
                           : ZEPHYRUM) + I;
    si (indicium.positio < I)
    {
        indicium.positio = I;
    }
    redde mutare_ramum(&m->ramus, INSULA_EPHEMERA, index_mutator,
        &indicium);
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
                    i32  altitudo,
            ScribaLiber* liber)
{
                        chorda  domini;
                  StmlResultus  res;
                        chorda  attributa;
                        chorda  visus;
                        chorda  pagina;
                           b32  inventum;
    constans TabulaCharacterum* folium;
            constans character* spatium;

    si (!m || !piscina || !intern || !volumen || !repo)
    {
        redde FALSUM;
    }
    memset(m, ZEPHYRUM, magnitudo(ScribaMontatio));
    spatium   = id ? id : "";
    m->liber  = liber;
    visus     = chorda_concatenare(chorda_ex_literis("scriba/visus/",
        piscina), chorda_ex_literis(id ? id : "radix", piscina),
        piscina);

    /* S2b: visus paginae libri - pagina ex plagula visus (absens aut
     * ignota: prima), documentum libri commune */
    si (liber)
    {
        pagina = volumen_plagulam_promere(volumen, visus, piscina,
            &inventum);
        si (!inventum || scriba_liber_index(liber, pagina) < ZEPHYRUM)
        {
            pagina = scriba_liber_nomen(liber, ZEPHYRUM);
        }
        m->doc = scriba_liber_pagina(liber, pagina);
    }
    /* documentum proprium: exsistens aut novum (spatium montationis) */
    si (!liber)
    {
        m->doc = scriba_documentum_aperire(piscina, intern, volumen,
            spatium);
    }
    si (!m->doc && !liber)
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
    m->actiones_ctx.liber = liber;
    m->actiones_ctx.visus = visus;
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
    (vacuum)paginam_indicare(m, VERUM);
    redde VERUM;
}

b32
scriba_reficere (
    ScribaMontatio* m)
{
                ScribaActiones* sa;
    constans TabulaCharacterum* t;
                           b32  mutatum;

    si (!m || !m->actiones_ctx.doc)
    {
        redde FALSUM;
    }
    sa       = &m->actiones_ctx;
    mutatum  = m->doc != sa->doc;
    m->doc   = sa->doc;
    si (paginam_indicare(m, mutatum))
    {
        mutatum = VERUM;
    }
    /* visus stalus: alius visus commisit (solus focatus scribit -
     * hic gestus pendens nullus) */
    si (scriba_documentum_cursor(sa->doc) != sa->cursor_laboris)
    {
        t = scriba_documentum_tabula(sa->doc);
        memcpy(sa->laboris.cellulae, t->cellulae,
            (memoriae_index)(t->latitudo * t->altitudo));
        memcpy(sa->laboris.indentatio, t->indentatio,
            (memoriae_index)t->altitudo * magnitudo(s32));
        sa->cursor_laboris  = scriba_documentum_cursor(sa->doc);
        mutatum             = VERUM;
    }
    redde mutatum;
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
               app->repo, NIHIL, radix, latitudo, altitudo, NIHIL))
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
