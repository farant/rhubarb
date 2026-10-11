/* pictor_applicatio.c - compositio pictoris communis (A4). Codex ex
 * pictor.c translatus; ratio in capite. */

#include "pictor_applicatio.h"
#include "chorda.h"
#include "filum.h"
#include "stml.h"
#include "canon.h"
#include "organum.h"
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

/* manifestum legere (radix + via); NIHIL si deest aut malum - vitia
 * in stderr */
interior Organum*
manifestum_legere (
                Piscina* p,
    InternamentumChorda* in,
     constans character* radix,
     constans character* via)
{
     chorda  fons;
    Organum* o;
        Xar* vitia;
        i32  i;

    fons = filum_legere_totum(_via(p, radix, via), p);
    si (fons.mensura == ZEPHYRUM)
    {
        fprintf(stderr, "pictor: manifestum abest: %s\n", via);
        redde NIHIL;
    }
    o = organum_legere(fons, p, in, &vitia);
    per (i = ZEPHYRUM; i < xar_numerus(vitia); i++)
    {
        chorda* v;

        v = (chorda*)xar_obtinere(vitia, i);
        fprintf(stderr, "pictor: %s: %.*s\n", via, (int)v->mensura,
                (constans character*)v->datum);
    }
    redde o;
}

/* S5b: enumerator stratorum (relatio) - ordine imum primum */
interior i32
strata_numerus (
    constans vacuum* documentum)
{
    redde pictor_documentum_numerus_stratorum(
        (constans PictorDocumentum*)documentum);
}

interior chorda
strati_id (
     constans vacuum* documentum,
                 i32  index,
             Piscina* piscina)
{
    redde chorda_ex_s32(pictor_documentum_stratum(
        (constans PictorDocumentum*)documentum, index)->id, piscina);
}

/* status-plan S2: manifestum pictoris cum basi communi */
interior Organum*
organum_pictoris (
                Piscina* p,
    InternamentumChorda* in,
     constans character* radix)
{
    Organum* o;
    Organum* basis;

    o = manifestum_legere(p, in, radix, "apps/pictor/organum.stml");
    basis = manifestum_legere(p, in, radix,
        "apps/organum_commune.stml");
    si (!o || !basis || !organum_basim_addere(o, basis))
    {
        redde NIHIL;
    }
    redde o;
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

/* S3e: clavis memoriae: spatium, aut "." pro radice ("" clavis
 * tabulae dispersae non est; '.' in id rami numquam - canon) */
interior chorda
clavis_memoriae (
    constans character* spatium,
               Piscina* p)
{
    redde chorda_ex_literis(spatium
        && spatium[ZEPHYRUM] ? spatium : ".",
        p);
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
                    i32  altitudo,
        StatusSystemata* systemata)
{
                 chorda  durabilis;
     constans character* spatium;
                Organum* organum;
                  Canon* canon;
                 chorda  causa;
              StmlNodus* domini;

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
    /* S3e: memoria picturarum ostensarum, prima haec */
    m->documenta = tabula_dispersa_creare_chorda(piscina, XVI);
    si (m->documenta)
    {
        (vacuum)tabula_dispersa_inserere(m->documenta,
            clavis_memoriae(spatium, piscina), m->doc);
    }
    m->ramus = id ? insula_ramus(repo, "pictor", id)
                  : insula_ramus_radix(repo);

    /* canones PRIMUM (hospes elementum montatum numquam videt): ex
     * manifesto GENERATI (status-plan S2), idem utrique insulae dum
     * divisio manet */
    organum = organum_pictoris(piscina, intern, radix);
    si (!organum)
    {
        redde FALSUM;
    }
    causa.mensura = ZEPHYRUM;
    causa.datum = NIHIL;
    canon = organum_canonem_generare(organum, piscina, intern, &causa);
    si (!canon)
    {
        fprintf(stderr, "pictor: canon generatus: %.*s\n",
                (int)causa.mensura, (constans character*)causa.datum);
        redde FALSUM;
    }
    insula_ramus_canonem_ponere(&m->ramus, INSULA_DURABILIS, canon);
    insula_ramus_canonem_ponere(&m->ramus, INSULA_EPHEMERA, canon);

    /* elementum initiale: durabilis VACUUM (status-plan C1 - insula
     * una; ramus solum, donec genus durabile tollitur); ephemera =
     * dimensiones documenti (derivatae) + absentes manifesti (S4) +
     * focus pictoris (id componentis - basis communis eum scire
     * nequit) */
    durabilis = chorda_ex_literis(" latitudo=\"", piscina);
    durabilis = chorda_concatenare(durabilis,
        chorda_ex_s32((s32)m->doc->latitudo, piscina), piscina);
    durabilis = chorda_concatenare(durabilis,
        chorda_ex_literis("\" altitudo=\"", piscina), piscina);
    durabilis = chorda_concatenare(durabilis,
        chorda_ex_s32((s32)m->doc->altitudo, piscina), piscina);
    durabilis = chorda_concatenare(durabilis,
        chorda_ex_literis("\"", piscina), piscina);
    durabilis = chorda_concatenare(durabilis,
        organum_initium(organum, piscina), piscina);
    durabilis = chorda_concatenare(durabilis,
        chorda_ex_literis(" focus=\"tabula\"", piscina), piscina);
    si (   !insula_ramum_initiare(&m->ramus, INSULA_DURABILIS,
               elementum(piscina, id, ""))
        || !insula_ramum_initiare(&m->ramus, INSULA_EPHEMERA,
               elementum(piscina, id, chorda_ut_cstr(durabilis,
               piscina))))
    {
        fprintf(stderr, "pictor: elementum initiale: %.*s\n",
                (int)insula_causa(repo).mensura,
                insula_causa(repo).datum);
        redde FALSUM;
    }

    /* S3: status servatus restituitur (post initiationem, ante
     * dominos - scriptura anonyma), ramus deinde servatur */
    si (systemata)
    {
        m->systemata = *systemata;
    }
    si (m->systemata.conservatio)
    {
        (vacuum)conservatio_ramum_addere(m->systemata.conservatio,
            &m->ramus, organum, id ? id : "pictor");
    }

    /* domini POST initiationem, ex manifesto generati */
    domini = organum_dominos_generare(organum, piscina, intern);
    si (domini)
    {
        (vacuum)insula_ramus_dominos_legere(&m->ramus, INSULA_EPHEMERA,
            domini);
        (vacuum)insula_ramus_dominos_legere(&m->ramus, INSULA_DURABILIS,
            domini);
    }

    /* S5b: relatio - strata documenti enumerantur, ramus registratur,
     * documentum ligatur (stratum_activum statim verificatur) */
    si (m->systemata.relatio)
    {
        RelatioEnumerator en;

        en.numerus  = strata_numerus;
        en.id       = strati_id;
        (vacuum)relatio_genus_addere(m->systemata.relatio,
            "pictor-documentum", "stratum", en);
        (vacuum)relatio_ramum_addere(m->systemata.relatio, &m->ramus,
            organum);
        (vacuum)relatio_ligare(m->systemata.relatio, &m->ramus,
            "documentum", m->doc, m->doc->historia);
    }

    /* S5c: dimissio - campi popover rami (palette) */
    si (m->systemata.dimissio)
    {
        (vacuum)dimissio_ramum_addere(m->systemata.dimissio, &m->ramus,
            organum);
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
    m->compositio.doc                = m->doc;
    redde VERUM;
}

/* S3e: dimensiones durabiles ex documento (ctx) */
interior vacuum
dimensiones_mutator (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    constans PictorDocumentum* doc;

    doc = (constans PictorDocumentum*)ctx;
    insula_attributum_ponere(radix, p, in, "latitudo", chorda_ut_cstr(
        chorda_ex_s32((s32)doc->latitudo, p), p));
    insula_attributum_ponere(radix, p, in, "altitudo", chorda_ut_cstr(
        chorda_ex_s32((s32)doc->altitudo, p), p));
}

/* S3e: spatium in bibliotheca? (plagula alterius originis - scriba -
 * documentum pictoris non est, etsi "spatium/documentum") */
interior b32
in_bibliotheca (
            Volumen* volumen,
             chorda  spatium,
            Piscina* piscina)
{
    Xar* l;
    i32  i;

    l = pictor_documenta_enumerare(volumen, piscina);
    per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
    {
        si (chorda_aequalis(*(chorda*)xar_obtinere(l, i), spatium))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

b32
pictor_picturam_ponere (
         PictorMontatio* m,
     constans character* spatium)
{
    PictorDocumentum* doc;
              vacuum* valor;
              chorda  clavis;
             Piscina* p;

    si (!m || !m->doc || !spatium)
    {
        redde FALSUM;
    }
    p       = m->doc->piscina;
    clavis  = clavis_memoriae(spatium, p);
    doc     = NIHIL;
    valor   = NIHIL;
    si (   m->documenta
        && tabula_dispersa_invenire(m->documenta, clavis, &valor))
    {
        doc = (PictorDocumentum*)valor;
    }
    si (!doc)
    {
        si (!in_bibliotheca(m->doc->volumen, chorda_ex_literis(spatium,
                p), p))
        {
            redde FALSUM;
        }
        doc = pictor_documentum_aperire(p, m->doc->intern,
            m->doc->volumen, spatium);
        si (!doc)
        {
            redde FALSUM;
        }
        si (m->documenta)
        {
            (vacuum)tabula_dispersa_inserere(m->documenta, clavis, doc);
        }
    }
    m->doc               = doc;
    m->actiones_ctx.doc  = doc;
    m->figurae_ctx.doc   = doc;
    m->compositio.doc    = doc;
    /* S5b: ligatio nova - stratum_activum contra documentum novum */
    si (m->systemata.relatio)
    {
        (vacuum)relatio_ligare(m->systemata.relatio, &m->ramus,
            "documentum", doc, doc->historia);
    }
    redde mutare_ramum(&m->ramus, INSULA_EPHEMERA, dimensiones_mutator,
        doc);
}

b32
pictor_picturam_condere (
         PictorMontatio* m,
     constans character* spatium)
{
    PictorDocumentum* doc;
             Piscina* p;

    si (!m || !m->doc || !spatium || !spatium[ZEPHYRUM])
    {
        redde FALSUM;
    }
    p = m->doc->piscina;
    si (in_bibliotheca(m->doc->volumen, chorda_ex_literis(spatium, p),
            p))
    {
        redde FALSUM;
    }
    /* magnitudo tabulae novae (ut pictor_montare): superficies minus
     * margines et linea status */
    doc = pictor_documentum_creare(p, m->doc->intern, m->doc->volumen,
        spatium, tabulae_dimensio((i32)m->compositio.fenestra_latitudo,
        II * CELLULA_LATITUDO), tabulae_dimensio(
        (i32)m->compositio.fenestra_altitudo,
        (STATUS_LINEAE + II) * CELLULA_ALTITUDO), LXIV);
    si (!doc)
    {
        redde FALSUM;
    }
    si (m->documenta)
    {
        (vacuum)tabula_dispersa_inserere(m->documenta,
            clavis_memoriae(spatium, p), doc);
    }
    redde pictor_picturam_ponere(m, spatium);
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
    StatusSystemata systemata;

    memset(&systemata, ZEPHYRUM, magnitudo(systemata));
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
    si (app->repo)
    {
        systemata.conservatio = conservatio_creare(piscina, intern,
            volumen, app->repo);
        systemata.relatio     = relatio_creare(piscina, intern,
            app->repo);
        systemata.dimissio    = dimissio_creare(piscina, intern,
            app->repo);
    }
    si (   !app->repo
        || !pictor_montare(&app->montatio, piscina, intern, volumen,
               app->repo, NIHIL, radix, latitudo, altitudo, &systemata))
    {
        redde FALSUM;
    }
    app->doc       = app->montatio.doc;
    app->actiones  = app->montatio.actiones;
    app->figurae   = app->montatio.figurae;
    app->d = dispensator_creare(piscina, intern, app->repo,
        app->montatio.actiones, pictor_componere,
        &app->montatio.compositio, CCC);
    /* S5c: dimissio in via eventus */
    dimissio_dispensatori_ligare(systemata.dimissio, app->d);
    redde app->d ? VERUM : FALSUM;
}
