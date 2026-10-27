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

nomen structura {
       s32 positio;
       s32 numerus;
    chorda titulus;   /* D5 S3c: nomen paginae (campus 'pagina') */
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
    si (indicium->titulus.mensura > ZEPHYRUM)
    {
        insula_attributum_ponere(nodus, p, in, "pagina",
            chorda_ut_cstr(indicium->titulus, p));
    }
}

/* S2b: index paginae in ramo ephemero (linea status) - pagina visus
 * (plagula) et numerus libri; D5 S3c: nomen paginae in campo 'pagina'
 * (semita lateris; scriptor 'pagina.clavis'). Sine coactione solum si
 * numerus mutatus (visus alius paginam creavit); VERUM si scriptum. */
interior b32
paginam_indicare (
    ScribaMontatio* m,
               b32  cogere)
{
    IndexPaginae  indicium;
          chorda  pagina;
          chorda  prior;
          chorda* a;
             s32  vetus;
             b32  inventum;
             b32  scriptum;

    si (!m->liber)
    {
        redde FALSUM;
    }
    indicium.numerus = (s32)scriba_liber_numerus(m->liber);
    a = insula_ramus_attributum(&m->ramus,
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
    indicium.titulus = scriba_liber_nomen(m->liber,
        (i32)(indicium.positio - I));
    /* campus 'pagina' domino 'pagina.clavis' (vocans alius esse
     * potest: pulsus, scriba_librum_ponere) */
    prior.datum    = NIHIL;
    prior.mensura  = ZEPHYRUM;
    si (m->ramus.repo)
    {
        prior = m->ramus.repo->scriptor;
        insula_scriptorem_ponere(m->ramus.repo, chorda_ex_literis(
            "pagina.clavis", m->ramus.repo->piscina));
    }
    scriptum = mutare_ramum(&m->ramus, index_mutator, &indicium);
    si (m->ramus.repo)
    {
        insula_scriptorem_ponere(m->ramus.repo, prior);
    }
    redde scriptum;
}

/* A6c: montatio ad ramum datum (radix, <scriba id> radicis, aut
 * liberum parentis) - scriba_montare et scriba_montare_in_ramo */
interior b32
ad_ramum_montare (
         ScribaMontatio* m,
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
            InsulaRamus  ramus,
     constans character* id,
                    i32  latitudo,
                    i32  altitudo,
            ScribaLiber* liber,
        StatusSystemata* systemata)
{
                        chorda  attributa;
                       Organum* organum;
                        chorda  visus;
                        chorda  pagina;
                           b32  inventum;
    constans TabulaCharacterum* folium;
            constans character* spatium;

    si (!m || !piscina || !intern || !volumen)
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
    folium    = scriba_documentum_tabula(m->doc);
    m->ramus  = ramus;

    /* status-plan S6b: organum per modulum communem (manifestum cum
     * basi, canon, initium, restitutio, domini); manifestum infixum
     * (S7c-1) */
    organum = organum_legere_genus(piscina, intern, "scriba", "scriba");
    si (!organum)
    {
        redde FALSUM;
    }
    /* addita: dimensiones folii (derivatae) + focus scribae (id
     * componentis); modus ex absente manifesti */
    attributa = chorda_ex_literis(" latitudo=\"", piscina);
    attributa = chorda_concatenare(attributa,
        chorda_ex_s32((s32)folium->latitudo, piscina), piscina);
    attributa = chorda_concatenare(attributa,
        chorda_ex_literis("\" altitudo=\"", piscina), piscina);
    attributa = chorda_concatenare(attributa,
        chorda_ex_s32((s32)folium->altitudo, piscina), piscina);
    attributa = chorda_concatenare(attributa,
        chorda_ex_literis("\" focus=\"pagina\"", piscina), piscina);
    si (!organum_ramum_parare(organum, &m->ramus, systemata,
            chorda_ut_cstr(attributa, piscina), id ? id : "scriba",
            piscina, intern))
    {
        redde FALSUM;
    }

    /* contextus et registra propria */
    m->actiones = actio_registrum_creare(piscina, intern);
    scriba_actiones_initiare(&m->actiones_ctx, m->doc, piscina);
    m->actiones_ctx.ramus = m->ramus;
    m->actiones_ctx.liber = liber;
    m->actiones_ctx.visus = visus;
    /* comites P2: liber et visus montationis (domus) */
    m->liber_domus = liber;
    m->visus_domus = visus;
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
scriba_montare (
         ScribaMontatio* m,
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     InsulaRepositorium* repo,
     constans character* id,
                    i32  latitudo,
                    i32  altitudo,
            ScribaLiber* liber,
        StatusSystemata* systemata)
{
    si (!repo)
    {
        redde FALSUM;
    }
    redde ad_ramum_montare(m, piscina, intern, volumen, id
        ? insula_ramus(repo, "scriba", id) : insula_ramus_radix(repo),
        id,
        latitudo, altitudo, liber, systemata);
}

b32
scriba_montare_in_ramo (
          ScribaMontatio* m,
                 Piscina* piscina,
     InternamentumChorda* intern,
                 Volumen* volumen,
    constans InsulaRamus* parens,
      constans character* id,
                     i32  latitudo,
                     i32  altitudo,
             ScribaLiber* liber,
         StatusSystemata* systemata)
{
    si (!parens || !id || !id[ZEPHYRUM])
    {
        redde FALSUM;
    }
    redde ad_ramum_montare(m, piscina, intern, volumen,
        insula_ramus_liberi(parens, "scriba", id), id, latitudo,
        altitudo,
        liber, systemata);
}

b32
scriba_librum_ponere (
    ScribaMontatio* m,
       ScribaLiber* liber)
{
    chorda prior;
       b32 ok;

    si (!m || !liber || !m->ramus.repo)
    {
        redde FALSUM;
    }
    /* domini rami scribae scripturas 'pagina.clavis' solas admittunt
     * (vocans actio alia est: calendarium.agere) */
    prior = m->ramus.repo->scriptor;
    insula_scriptorem_ponere(m->ramus.repo, chorda_ex_literis(
        "pagina.clavis", m->ramus.repo->piscina));
    ok = scriba_actiones_librum_ponere(&m->ramus, NIHIL,
        &m->actiones_ctx,
        liber);
    insula_scriptorem_ponere(m->ramus.repo, prior);
    si (!ok)
    {
        redde FALSUM;
    }
    m->liber  = liber;
    m->doc    = m->actiones_ctx.doc;
    (vacuum)paginam_indicare(m, VERUM);
    redde VERUM;
}

b32
scriba_paginam_ostendere (
    ScribaMontatio* m,
            chorda  titulus)
{
    chorda prior;
       b32 ok;

    si (!m || !m->liber || !m->ramus.repo)
    {
        redde FALSUM;
    }
    /* domini rami scribae 'pagina.clavis' solam admittunt (vocans
     * actio alia est: indagatio.agere) */
    prior = m->ramus.repo->scriptor;
    insula_scriptorem_ponere(m->ramus.repo, chorda_ex_literis(
        "pagina.clavis", m->ramus.repo->piscina));
    ok = scriba_actiones_paginam_ostendere(&m->ramus, NIHIL,
        &m->actiones_ctx, titulus);
    insula_scriptorem_ponere(m->ramus.repo, prior);
    si (!ok)
    {
        redde FALSUM;
    }
    m->doc = m->actiones_ctx.doc;
    (vacuum)paginam_indicare(m, VERUM);
    redde VERUM;
}

b32
scriba_semitam_secuta (
    vacuum* ctx)
{
    ScribaMontatio* m;
            chorda* a;
            chorda  petita;
            chorda  liber;
            chorda  currens;
               b32  inventum;

    m = (ScribaMontatio*)ctx;
    si (!m || !m->liber || !m->doc)
    {
        redde FALSUM;
    }
    /* copiae: mutationes infra ramum mutant */
    a       = insula_ramus_attributum(&m->ramus, "pagina");
    petita  = (a && a->mensura > ZEPHYRUM)
            ? chorda_transcribere(*a, m->doc->piscina)
            : chorda_ex_literis("", m->doc->piscina);
    /* comites P2: liber primum (porta posita) */
    a       = insula_ramus_attributum(&m->ramus, "liber");
    liber   = (a && a->mensura > ZEPHYRUM)
            ? chorda_transcribere(*a, m->doc->piscina)
            : chorda_ex_literis("", m->doc->piscina);
    si (   m->libri
        && !chorda_aequalis(liber, scriba_liber_praefixum(m->liber))
        && !scriba_librum_praefixi_ponere(m, liber))
    {
        redde FALSUM;
    }
    si (petita.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    currens  = volumen_plagulam_promere(m->doc->volumen,
        m->actiones_ctx.visus, m->doc->piscina, &inventum);
    si (inventum && chorda_aequalis(currens, petita))
    {
        redde VERUM;
    }
    redde scriba_paginam_ostendere(m, petita);
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
    /* opus currens (quaestio-rivus T7) ANTE iussum dilatum: pulsus quo
     * opus incipit nuntium solum pingit, textus pulsu proximo */
    mutatum  = scriba_opus_pulsare(sa);
    /* $question: iussum dilatum (pictura nuntii prius, deinde verbum) */
    mutatum  = scriba_iussum_dilatum_currere(sa) || mutatum;
    mutatum  = (m->doc != sa->doc) || mutatum;
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
scriba_occupata (
    constans ScribaMontatio* m)
{
    redde m && (   m->actiones_ctx.opus != NIHIL
                || m->actiones_ctx.dilatum != ZEPHYRUM);
}

b32
scriba_applicatio_aedificare (
       ScribaApplicatio* app,
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
                    i32  latitudo,
                    i32  altitudo)
{
    StatusSystemata systemata;

    memset(&systemata, ZEPHYRUM, magnitudo(systemata));
    si (!app || !piscina || !intern || !volumen)
    {
        redde FALSUM;
    }
    memset(app, ZEPHYRUM, magnitudo(ScribaApplicatio));
    app->piscina  = piscina;
    app->intern   = intern;
    app->volumen  = volumen;

    /* repositorium cuius radix IPSA scriba est; montatio in radice */
    app->repo = insula_repositorium_creare(piscina, intern,
        "<scriba focus=\"pagina\"/>");
    /* S6b: persistentia (cursor) - scriba nullum popover nec
     * referentiam habet */
    si (app->repo)
    {
        systemata.conservatio = conservatio_creare(piscina, intern,
            volumen, app->repo);
    }
    si (   !app->repo
        || !scriba_montare(&app->montatio, piscina, intern, volumen,
               app->repo, NIHIL, latitudo, altitudo, NIHIL,
               &systemata))
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

/* comites P2: campus 'liber' (praefixum; vacuum = liber domus) */
interior vacuum
libri_mutator (
              StmlNodus* nodus,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    /* vacuum: campus tollitur (STML valorem vacuum ut veritatem
     * legeret) */
    si (((chorda*)ctx)->mensura == ZEPHYRUM)
    {
        insula_attributum_tollere(nodus, "liber");
        redde;
    }
    insula_attributum_ponere(nodus, p, in, "liber",
        chorda_ut_cstr(*(chorda*)ctx, p));
}

vacuum
scriba_libros_ponere (
     ScribaMontatio* m,
     ScribaLibrorum  porta,
             vacuum* ctx)
{
    si (m)
    {
        m->libri      = porta;
        m->libri_ctx  = ctx;
    }
}

b32
scriba_librum_praefixi_ponere (
    ScribaMontatio* m,
            chorda  praefixum)
{
    ScribaLiber* liber;
         chorda  prior;
            b32  ok;

    si (!m || !m->ramus.repo)
    {
        redde FALSUM;
    }
    liber = praefixum.mensura == ZEPHYRUM ? m->liber_domus
          : m->libri ? m->libri(m->libri_ctx, praefixum) : NIHIL;
    si (!liber)
    {
        redde FALSUM;
    }
    /* pagina visa: comes in libro suo, domus in plagula montationis */
    m->actiones_ctx.visus = praefixum.mensura == ZEPHYRUM
        ? m->visus_domus
        : chorda_concatenare(chorda_transcribere(praefixum,
              m->doc->piscina), chorda_ex_literis("/visus",
              m->doc->piscina), m->doc->piscina);
    si (liber != m->liber && !scriba_librum_ponere(m, liber))
    {
        redde FALSUM;
    }
    prior = m->ramus.repo->scriptor;
    insula_scriptorem_ponere(m->ramus.repo, chorda_ex_literis(
        "pagina.clavis", m->ramus.repo->piscina));
    ok = mutare_ramum(&m->ramus, libri_mutator, &praefixum);
    insula_scriptorem_ponere(m->ramus.repo, prior);
    redde ok;
}

/* comites P3: campus 'liber_titulus' (vacuum: tollitur) */
interior vacuum
tituli_libri_mutator (
              StmlNodus* nodus,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    si (((chorda*)ctx)->mensura == ZEPHYRUM)
    {
        insula_attributum_tollere(nodus, "liber_titulus");
        redde;
    }
    insula_attributum_ponere(nodus, p, in, "liber_titulus",
        chorda_ut_cstr(*(chorda*)ctx, p));
}

b32
scriba_comitem_ponere (
    ScribaMontatio* m,
            chorda  praefixum,
            chorda  titulus)
{
    chorda prior;
       b32 ok;

    si (!scriba_librum_praefixi_ponere(m, praefixum))
    {
        redde FALSUM;
    }
    prior = m->ramus.repo->scriptor;
    insula_scriptorem_ponere(m->ramus.repo, chorda_ex_literis(
        "pagina.clavis", m->ramus.repo->piscina));
    ok = mutare_ramum(&m->ramus, tituli_libri_mutator, &titulus);
    insula_scriptorem_ponere(m->ramus.repo, prior);
    redde ok;
}
