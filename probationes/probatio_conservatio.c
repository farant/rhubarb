/* probatio_conservatio.c - systema persistentiae (status-plan S3):
 * circuitus per volumen, transientes numquam, scriptura solum cum pars
 * persistens mutatur, rami separati, migratio (ignoti, non
 * persistentes, versio ignota), typus malus praeteritur. Organum
 * ludicrum 'ludus', rami <ludus id> in repositorio. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"
#include "stml.h"
#include "canon.h"
#include "volumen.h"
#include "insula.h"
#include "organum.h"
#include "conservatio.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

hic_manens constans character manifestum[] =
    "<organum genus=\"ludus\" versio=\"1\"><status>"
    "<campus nomen=\"id\" typus=\"nomen\" vita=\"persistens\"/>"
    "<campus nomen=\"color\" typus=\"numerus\" vita=\"persistens\"/>"
    "<campus nomen=\"zoom\" typus=\"numerus\" vita=\"persistens\""
    " rhythmus=\"mobilis\"/>"
    "<campus nomen=\"lat\" typus=\"numerus\" vita=\"persistens\"/>"
    "<campus nomen=\"palette\" typus=\"nomen\" vita=\"transiens\""
    " aspectus=\"popover\"/>"
    "<campus nomen=\"fuga\" typus=\"textus\" vita=\"probatio\"/>"
    "</status></organum>";

nomen structura {
      InsulaRepositorium* repo;
                 Organum* organum;
             Conservatio* conservatio;
             InsulaRamus  t1;
             InsulaRamus  t2;
} Mundus;

/* repositorium novum (status initialis idem semper), canon generatus
 * in ramis, conservatio supra volumen */
interior b32
mundum_creare (
                 Mundus* m,
                Piscina* p,
    InternamentumChorda* in,
                Volumen* vol)
{
      Xar* vitia;
    Canon* canon;
   chorda  causa;

    memset(m, ZEPHYRUM, magnitudo(*m));
    m->organum = organum_legere(chorda_ex_literis(manifestum, p), p, in,
        &vitia);
    si (!m->organum)
    {
        redde FALSUM;
    }
    m->repo = insula_repositorium_creare(p, in,
        "<hospes><ludus id=\"t1\" lat=\"5\" color=\"1\"/><ludus"
        " id=\"t2\" color=\"1\"/></hospes>");
    si (!m->repo)
    {
        redde FALSUM;
    }
    causa.mensura = ZEPHYRUM;
    causa.datum = NIHIL;
    canon = organum_canonem_generare(m->organum, p, in, &causa);
    m->t1 = insula_ramus(m->repo, "ludus", "t1");
    m->t2 = insula_ramus(m->repo, "ludus", "t2");
    insula_ramus_canonem_ponere(&m->t1, canon);
    insula_ramus_canonem_ponere(&m->t1, canon);
    insula_ramus_canonem_ponere(&m->t2, canon);
    insula_ramus_canonem_ponere(&m->t2, canon);
    m->conservatio = conservatio_creare(p, in, vol, m->repo);
    redde m->conservatio != NIHIL;
}

/* ctx = "attributum=valor" */
interior vacuum
ponere (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
     constans character* par;
     constans character* signum;
              character  titulus[LXIV];

    par     = (constans character*)ctx;
    signum  = strchr(par, '=');
    memcpy(titulus, par, (size_t)(signum - par));
    titulus[signum - par] = '\0';
    insula_attributum_ponere(radix, p, in, titulus, signum + I);
}

interior b32
scribere (
    constans InsulaRamus* ramus,
      constans character* par)
{
    character copia[CXXVIII];

    strcpy(copia, par);
    redde mutare_ramum(ramus, ponere, copia);
}

/* valor attributi rami aequat literas? (NIHIL literae = abest) */
interior b32
valor_est (
    constans InsulaRamus* ramus,
      constans character* titulus,
      constans character* valor)
{
    chorda* a;

    a = insula_ramus_attributum(ramus, titulus);
    si (!valor)
    {
        redde a == NIHIL;
    }
    redde a && chorda_aequalis_literis(*a, valor);
}

interior i32
numerus_actorum (
     Volumen* vol,
     Piscina* p)
{
    redde xar_numerus(volumen_acta_legere(vol, ZEPHYRUM, p));
}

/* praesentia clavis fragmentum continet? (D1: status extra acta) */
interior b32
praesens_continet (
               Volumen* vol,
               Piscina* p,
    constans character* clavis,
    constans character* fragmentum)
{
    chorda valor;
       b32 inventum;

    valor = volumen_praesens_legere(vol, chorda_ex_literis(clavis, p),
        p, &inventum);
    redde inventum && chorda_continet(valor, chorda_ex_literis(
        fragmentum, p));
}

/* signum in praesentia: si superest, conservatio non scripsit */
interior b32
signum_ponere (
    Volumen* vol,
    Piscina* p)
{
    redde volumen_praesens_ponere(vol, chorda_ex_literis("status/t1",
        p), chorda_ex_literis("SIGNUM", p));
}


/* ==================================================
 * I. circuitus: scribere, aperire, restituere
 * ================================================== */

interior vacuum
_probare_circuitum (
    CredoContextus* c)
{
    InternamentumChorda* in;
                Volumen* vol;
                 Mundus  a;
                 Mundus  b;

    in   = internamentum_creare(c->piscina);
    vol  = volumen_temporarium(c->piscina, "probatio_conservatio");
    CREDO_NECESSE_NON_NIHIL(vol);
    CREDO_NECESSE(mundum_creare(&a, c->piscina, in, vol));
    CREDO_AEQUALIS_I32(conservatio_ramum_addere(a.conservatio, &a.t1,
        a.organum, "t1"), ZEPHYRUM);
    CREDO_VERUM(scribere(&a.t1, "color=7"));
    CREDO_VERUM(scribere(&a.t1, "zoom=3"));
    CREDO_VERUM(scribere(&a.t1, "lat=9"));
    CREDO_VERUM(scribere(&a.t1, "palette=colores"));
    CREDO_VERUM(scribere(&a.t1, "fuga=x"));

    /* sessio nova: repositorium recens, idem volumen */
    CREDO_NECESSE(mundum_creare(&b, c->piscina, in, vol));
    CREDO_VERUM(valor_est(&b.t1, "color", "1"));
    CREDO_AEQUALIS_I32(conservatio_ramum_addere(b.conservatio, &b.t1,
        b.organum, "t1"), III);  /* color, zoom, lat (id idem) */
    CREDO_VERUM(valor_est(&b.t1, "color", "7"));
    CREDO_VERUM(valor_est(&b.t1, "zoom", "3"));
    CREDO_VERUM(valor_est(&b.t1, "lat", "9"));
    /* transiens et probatio numquam servantur */
    CREDO_VERUM(valor_est(&b.t1, "palette", NIHIL));
    CREDO_VERUM(valor_est(&b.t1, "fuga", NIHIL));
    volumen_claudere(vol);
}


/* ==================================================
 * II. scriptura solum cum pars persistens mutatur
 * ================================================== */

interior vacuum
_probare_parsimoniam (
    CredoContextus* c)
{
    InternamentumChorda* in;
                Volumen* vol;
                 Mundus  a;
                 Mundus  b;
                    i32  ante;

    in   = internamentum_creare(c->piscina);
    vol  = volumen_temporarium(c->piscina, "probatio_conservatio");
    CREDO_NECESSE_NON_NIHIL(vol);
    CREDO_NECESSE(mundum_creare(&a, c->piscina, in, vol));
    (vacuum)conservatio_ramum_addere(a.conservatio, &a.t1, a.organum,
        "t1");

    /* D1: scripturae status diarium NON augent */
    ante = numerus_actorum(vol, c->piscina);
    CREDO_VERUM(signum_ponere(vol, c->piscina));
    /* transiens solum: nihil */
    CREDO_VERUM(scribere(&a.t1, "palette=colores"));
    CREDO_VERUM(praesens_continet(vol, c->piscina, "status/t1",
        "SIGNUM"));
    /* idem valor: nihil */
    CREDO_VERUM(scribere(&a.t1, "color=1"));
    CREDO_VERUM(praesens_continet(vol, c->piscina, "status/t1",
        "SIGNUM"));
    /* persistens mutatus: scribitur (in praesentiam, non in acta) */
    CREDO_VERUM(scribere(&a.t1, "color=4"));
    CREDO_VERUM(praesens_continet(vol, c->piscina, "status/t1",
        "color=\"4\""));
    CREDO_AEQUALIS_I32(numerus_actorum(vol, c->piscina), ante);
    /* ramus non registratus: nihil */
    CREDO_VERUM(scribere(&a.t2, "color=6"));
    CREDO_FALSUM(praesens_continet(vol, c->piscina, "status/t2", ""));

    /* restitutio = ultimum: scriptura transiens post eam nihil */
    CREDO_NECESSE(mundum_creare(&b, c->piscina, in, vol));
    CREDO_MAIOR_I32(conservatio_ramum_addere(b.conservatio, &b.t1,
        b.organum, "t1"), ZEPHYRUM);
    CREDO_VERUM(signum_ponere(vol, c->piscina));
    CREDO_VERUM(scribere(&b.t1, "palette=colores"));
    CREDO_VERUM(praesens_continet(vol, c->piscina, "status/t1",
        "SIGNUM"));
    CREDO_AEQUALIS_I32(numerus_actorum(vol, c->piscina), ante);
    volumen_claudere(vol);
}


/* ==================================================
 * III. rami separati
 * ================================================== */

interior vacuum
_probare_ramos (
    CredoContextus* c)
{
    InternamentumChorda* in;
                Volumen* vol;
                 Mundus  a;
                 Mundus  b;

    in   = internamentum_creare(c->piscina);
    vol  = volumen_temporarium(c->piscina, "probatio_conservatio");
    CREDO_NECESSE_NON_NIHIL(vol);
    CREDO_NECESSE(mundum_creare(&a, c->piscina, in, vol));
    (vacuum)conservatio_ramum_addere(a.conservatio, &a.t1, a.organum,
        "t1");
    (vacuum)conservatio_ramum_addere(a.conservatio, &a.t2, a.organum,
        "t2");
    CREDO_VERUM(scribere(&a.t1, "color=2"));
    CREDO_VERUM(scribere(&a.t2, "color=8"));

    CREDO_NECESSE(mundum_creare(&b, c->piscina, in, vol));
    (vacuum)conservatio_ramum_addere(b.conservatio, &b.t1, b.organum,
        "t1");
    (vacuum)conservatio_ramum_addere(b.conservatio, &b.t2, b.organum,
        "t2");
    CREDO_VERUM(valor_est(&b.t1, "color", "2"));
    CREDO_VERUM(valor_est(&b.t2, "color", "8"));
    volumen_claudere(vol);
}


/* ==================================================
 * IV. migratio: ignoti, non persistentes, versio ignota
 * ================================================== */

interior vacuum
_probare_migrationem (
    CredoContextus* c)
{
    InternamentumChorda* in;
                Volumen* vol;
                 Mundus  a;

    in   = internamentum_creare(c->piscina);
    vol  = volumen_temporarium(c->piscina, "probatio_conservatio");
    CREDO_NECESSE_NON_NIHIL(vol);
    CREDO_NECESSE(volumen_praesens_ponere(vol, chorda_ex_literis(
        "status/t1", c->piscina), chorda_ex_literis(
        "<status versio=\"1\" genus=\"ludus\"><ephemera color=\"5\""
        " ignotum=\"x\" palette=\"colores\" fuga=\"y\"/></status>",
        c->piscina)));
    CREDO_NECESSE(mundum_creare(&a, c->piscina, in, vol));
    CREDO_AEQUALIS_I32(conservatio_ramum_addere(a.conservatio, &a.t1,
        a.organum, "t1"), I);
    CREDO_VERUM(valor_est(&a.t1, "color", "5"));
    CREDO_VERUM(valor_est(&a.t1, "ignotum", NIHIL));
    CREDO_VERUM(valor_est(&a.t1, "palette", NIHIL));
    CREDO_VERUM(valor_est(&a.t1, "fuga", NIHIL));

    /* versio ignota: totus praeteritur */
    CREDO_NECESSE(volumen_praesens_ponere(vol, chorda_ex_literis(
        "status/t2", c->piscina), chorda_ex_literis(
        "<status versio=\"2\" genus=\"ludus\"><ephemera color=\"5\"/>"
        "</status>", c->piscina)));
    CREDO_AEQUALIS_I32(conservatio_ramum_addere(a.conservatio, &a.t2,
        a.organum, "t2"), ZEPHYRUM);
    CREDO_VERUM(valor_est(&a.t2, "color", "1"));
    volumen_claudere(vol);
}


/* ==================================================
 * V. typus malus: praeteritur, montatio non frangitur
 * ================================================== */

interior vacuum
_probare_typum_malum (
    CredoContextus* c)
{
    InternamentumChorda* in;
                Volumen* vol;
                 Mundus  a;

    in   = internamentum_creare(c->piscina);
    vol  = volumen_temporarium(c->piscina, "probatio_conservatio");
    CREDO_NECESSE_NON_NIHIL(vol);
    CREDO_NECESSE(volumen_praesens_ponere(vol, chorda_ex_literis(
        "status/t1", c->piscina), chorda_ex_literis(
        "<status versio=\"1\" genus=\"ludus\">"
        "<ephemera color=\"rubrum\""
        " zoom=\"2\"/></status>", c->piscina)));
    CREDO_NECESSE(mundum_creare(&a, c->piscina, in, vol));
    CREDO_AEQUALIS_I32(conservatio_ramum_addere(a.conservatio, &a.t1,
        a.organum, "t1"), ZEPHYRUM);
    CREDO_VERUM(valor_est(&a.t1, "color", "1"));
    CREDO_VERUM(valor_est(&a.t1, "zoom", NIHIL));
    /* ramus vivit: scriptura sequens servatur */
    CREDO_VERUM(scribere(&a.t1, "color=3"));
    volumen_claudere(vol);
}


/* ==================================================
 * VI. actarius alius: conservatio recusatur
 * ================================================== */

interior vacuum
actarius_alius (
     StmlNodus* radix,
        chorda  via,
        vacuum* ctx)
{
    (vacuum)radix;
    (vacuum)via;
    (vacuum)ctx;
}

interior vacuum
_probare_actarium_alium (
    CredoContextus* c)
{
    InternamentumChorda* in;
                Volumen* vol;
     InsulaRepositorium* repo;

    in   = internamentum_creare(c->piscina);
    vol  = volumen_temporarium(c->piscina, "probatio_conservatio");
    CREDO_NECESSE_NON_NIHIL(vol);
    repo = insula_repositorium_creare(c->piscina, in,
        "<hospes/>");
    CREDO_NECESSE_NON_NIHIL(repo);
    insula_ponere_actarium(repo, actarius_alius, NIHIL);
    CREDO_NIHIL(conservatio_creare(c->piscina, in, vol, repo));
    volumen_claudere(vol);
}


/* ==================================================
 * VII. C2: forma versionis 2; migratio versionis 1 (insulae duae)
 * ================================================== */

interior vacuum
_probare_versiones (
    CredoContextus* c)
{
    InternamentumChorda* in;
                Volumen* vol;
                 Mundus  a;
                 Mundus  b;
                 chorda  status;
                    b32  inventum;

    in   = internamentum_creare(c->piscina);
    vol  = volumen_temporarium(c->piscina, "probatio_conservatio");
    CREDO_NECESSE_NON_NIHIL(vol);

    /* scriptura nova: versio 2, campi ut attributa radicis */
    CREDO_NECESSE(mundum_creare(&a, c->piscina, in, vol));
    (vacuum)conservatio_ramum_addere(a.conservatio, &a.t1, a.organum,
        "t1");
    CREDO_VERUM(scribere(&a.t1, "color=6"));
    status = volumen_praesens_legere(vol,
        chorda_ex_literis("status/t1",
        c->piscina), c->piscina, &inventum);
    CREDO_NECESSE(inventum);
    CREDO_VERUM(chorda_continet(status, chorda_ex_literis(
        "<status versio=\"2\" genus=\"ludus\"", c->piscina)));
    CREDO_VERUM(chorda_continet(status, chorda_ex_literis("color=\"6\"",
        c->piscina)));
    CREDO_FALSUM(chorda_continet(status, chorda_ex_literis("ephemera",
        c->piscina)));

    /* versio 1 cum insulis duabus: fusio, ephemera vincit */
    CREDO_NECESSE(volumen_praesens_ponere(vol, chorda_ex_literis(
        "status/t2", c->piscina), chorda_ex_literis(
        "<status versio=\"1\" genus=\"ludus\">"
        "<durabilis lat=\"9\" color=\"2\"/>"
        "<ephemera color=\"7\" zoom=\"3\"/></status>",
        c->piscina)));
    CREDO_NECESSE(mundum_creare(&b, c->piscina, in, vol));
    CREDO_AEQUALIS_I32(conservatio_ramum_addere(b.conservatio, &b.t2,
        b.organum, "t2"), III);   /* lat, color, zoom */
    CREDO_VERUM(valor_est(&b.t2, "lat", "9"));
    CREDO_VERUM(valor_est(&b.t2, "color", "7"));
    CREDO_VERUM(valor_est(&b.t2, "zoom", "3"));
    volumen_claudere(vol);
}


hic_manens constans CredoSectio SECTIONES[] = {
    { "circuitus",      _probare_circuitum,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { "parsimonia",     _probare_parsimoniam,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { "rami",           _probare_ramos,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { "versiones",      _probare_versiones,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { "migratio",       _probare_migrationem,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { "typus_malus",    _probare_typum_malum,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { "actarius_alius", _probare_actarium_alium,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("conservatio", SECTIONES);
}
