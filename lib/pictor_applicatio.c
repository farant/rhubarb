/* pictor_applicatio.c - compositio pictoris communis (A4). Codex ex
 * pictor.c translatus; ratio in capite. */

#include "pictor_applicatio.h"
#include "chorda.h"
#include "filum.h"
#include "stml.h"
#include "canon.h"
#include "organum.h"
#include "semita.h"
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

b32
pictor_montare (
         PictorMontatio* m,
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     InsulaRepositorium* repo,
     constans character* id,
                    i32  latitudo,
                    i32  altitudo,
        StatusSystemata* systemata)
{
                 chorda  initium;
     constans character* spatium;
                Organum* organum;

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

    /* status-plan S6a: organum per modulum communem - manifestum cum
     * basi, canon, initium, restitutio, domini, dimissio; manifestum
     * infixum (S7c-1) */
    organum = organum_legere_genus(piscina, intern, "pictor", "pictor");
    si (!organum)
    {
        redde FALSUM;
    }
    si (systemata)
    {
        m->systemata = *systemata;
    }
    /* addita: dimensiones documenti (derivatae) + focus pictoris (id
     * componentis - basis communis eum scire nequit) */
    initium = chorda_ex_literis(" latitudo=\"", piscina);
    initium = chorda_concatenare(initium,
        chorda_ex_s32((s32)m->doc->latitudo, piscina), piscina);
    initium = chorda_concatenare(initium,
        chorda_ex_literis("\" altitudo=\"", piscina), piscina);
    initium = chorda_concatenare(initium,
        chorda_ex_s32((s32)m->doc->altitudo, piscina), piscina);
    initium = chorda_concatenare(initium,
        chorda_ex_literis("\" focus=\"tabula\"", piscina), piscina);
    /* semitae R4c: pictura ostensa (spatium lateris) */
    si (id && id[ZEPHYRUM])
    {
        initium = chorda_concatenare(initium, chorda_ex_literis(
            " pictura=\"", piscina), piscina);
        initium = chorda_concatenare(initium, chorda_ex_literis(id,
            piscina), piscina);
        initium = chorda_concatenare(initium, chorda_ex_literis("\"",
            piscina), piscina);
    }
    si (!organum_ramum_parare(organum, &m->ramus,
            systemata ? &m->systemata : NIHIL,
            chorda_ut_cstr(initium, piscina), id ? id : "pictor",
            piscina,
            intern))
    {
        redde FALSUM;
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

/* S3e: pictura ostensa (ctx) - dimensiones durabiles ex documento;
 * semitae R4c: campus pictura = spatium */
nomen structura {
    constans PictorDocumentum* doc;
           constans character* spatium;
} PicturaOstensa;

interior vacuum
dimensiones_mutator (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    constans PicturaOstensa* ostensa;

    ostensa = (constans PicturaOstensa*)ctx;
    insula_attributum_ponere(radix, p, in, "latitudo", chorda_ut_cstr(
        chorda_ex_s32((s32)ostensa->doc->latitudo, p), p));
    insula_attributum_ponere(radix, p, in, "altitudo", chorda_ut_cstr(
        chorda_ex_s32((s32)ostensa->doc->altitudo, p), p));
    insula_attributum_ponere(radix, p, in, "pictura", ostensa->spatium);
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
       PicturaOstensa  ostensa;

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
    ostensa.doc      = doc;
    ostensa.spatium  = spatium;
    redde mutare_ramum(&m->ramus, dimensiones_mutator, &ostensa);
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

/* semitae R4c: SemitaOrganum pro argumento - manifestum pictoris in
 * scheda vocantis; ctx = Organum* */
interior constans Organum*
organa_pictoris (
     vacuum* ctx,
     chorda  genus)
{
    redde chorda_aequalis_literis(genus, "pictor")
        ? (constans Organum*)ctx : NIHIL;
}

/* pictura exstat? (memoria communis aut bibliotheca - ut
 * pictor_picturam_ponere, sine mutatione) */
interior b32
pictura_exstat (
      constans PictorMontatio* m,
              constans chorda  spatium,
                      Piscina* p)
{
    vacuum* valor;

    valor = NIHIL;
    redde (   m->documenta
           && tabula_dispersa_invenire(m->documenta, clavis_memoriae(
                  chorda_ut_cstr(spatium, p), p), &valor))
        || in_bibliotheca(m->doc->volumen, spatium, p);
}

/* URI semitae pictoris? ("pictor", "pictor/...", "pictor?...") */
interior b32
uri_pictoris (
    constans character* argumentum)
{
    i32 l;

    l = (i32)strlen(argumentum);
    redde strncmp(argumentum, "pictor", VI) == ZEPHYRUM
        && (l == VI || argumentum[VI] == '/' || argumentum[VI] == '?');
}

b32
pictor_argumentum_ponere (
          PictorMontatio* m,
      constans character* argumentum,
                     b32  condere)
{
                Piscina* scheda;
    InternamentumChorda* in;
                Organum* organum;
              StmlNodus* semita;
                 chorda* pictura;
                 chorda  vitium;
                    b32  bonum;

    si (!m || !m->doc || !argumentum)
    {
        redde FALSUM;
    }
    si (!uri_pictoris(argumentum))
    {
        /* cognomen "<spatium>" (S3e/S3f) */
        redde pictor_picturam_ponere(m, argumentum)
            || (condere && pictor_picturam_condere(m, argumentum));
    }
    /* URI: picturam exstantem ostendit, numquam condit - documentum
     * prius (ignota recusatur), deinde semita (status ceteri) */
    scheda = piscina_generare_dynamicum("pictor_semita", XVI * M);
    si (!scheda)
    {
        redde FALSUM;
    }
    in       = internamentum_creare(scheda);
    organum  = in ? organum_legere_genus(scheda, in, "pictor", "pictor")
                  : NIHIL;
    semita   = organum ? semita_ex_uri(organa_pictoris, organum,
        chorda_ex_literis(argumentum, scheda), scheda, in, &vitium)
        : NIHIL;
    pictura  = semita ? stml_attributum_capere(semita, "pictura")
                      : NIHIL;
    bonum    = semita
        && (   !pictura || pictura->mensura == ZEPHYRUM
            || pictura_exstat(m, *pictura, scheda))
        && semita_applicare(organa_pictoris, organum, &m->ramus, semita,
               scheda, &vitium)
        && (   !pictura || pictura->mensura == ZEPHYRUM
            || pictor_picturam_ponere(m, chorda_ut_cstr(*pictura,
                   scheda)));
    piscina_destruere(scheda);
    redde bonum;
}

b32
pictor_semitam_secuta (
    vacuum* ctx)
{
    PictorMontatio* m;
            chorda* pictura;

    m = (PictorMontatio*)ctx;
    si (!m)
    {
        redde FALSUM;
    }
    pictura = insula_ramus_attributum(&m->ramus, "pictura");
    si (!pictura || pictura->mensura == ZEPHYRUM)
    {
        redde VERUM;
    }
    redde pictor_picturam_ponere(m, chorda_ut_cstr(*pictura,
        m->doc->piscina));
}

b32
pictor_applicatio_aedificare (
       PictorApplicatio* app,
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
    memset(app, ZEPHYRUM, magnitudo(PictorApplicatio));
    app->piscina  = piscina;
    app->intern   = intern;
    app->volumen  = volumen;

    /* repositorium cuius radix IPSA pictor est; montatio in radice */
    app->repo = insula_repositorium_creare(piscina, intern,
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
               app->repo, NIHIL, latitudo, altitudo, &systemata))
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
