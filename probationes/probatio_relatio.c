/* probatio_relatio.c - systema relationum (status-plan S5b) supra
 * documentum ludicrum: historia cum VIII numeratoribus, actum "k"
 * numeratorem k auget; ens "k" exstat dum numerator k > 0 - revocatio
 * ens tollit. Organum 'ludus': electum (summum), alterum (nullum),
 * tertium (sine orbato), referentiae ad documentum/ens. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"
#include "stml.h"
#include "volumen.h"
#include "insula.h"
#include "historia.h"
#include "organum.h"
#include "relatio.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

nomen structura {
    i8 numeratores[VIII];
} Ludicra;

interior vacuum
ludicra_vacare (
    vacuum* ctx)
{
    memset(((Ludicra*)ctx)->numeratores, ZEPHYRUM, VIII);
}

interior vacuum
ludicra_applicare (
    vacuum* ctx,
    chorda  actum)
{
    s32 k;

    si (chorda_ut_s32(actum, &k) && k >= ZEPHYRUM && k < VIII)
    {
        ((Ludicra*)ctx)->numeratores[k]++;
    }
}

/* enumerator: entia exstantia ordine ascendente */
interior i32
entia_numerus (
    constans vacuum* documentum)
{
    constans Ludicra* l;
                 i32  i;
                 i32  n;

    l = (constans Ludicra*)documentum;
    n = ZEPHYRUM;
    per (i = ZEPHYRUM; i < VIII; i++)
    {
        si (l->numeratores[i] > ZEPHYRUM)
        {
            n++;
        }
    }
    redde n;
}

interior chorda
entis_id (
     constans vacuum* documentum,
                 i32  index,
             Piscina* piscina)
{
    constans Ludicra* l;
                 i32  i;
                 i32  n;

    l = (constans Ludicra*)documentum;
    n = ZEPHYRUM;
    per (i = ZEPHYRUM; i < VIII; i++)
    {
        si (l->numeratores[i] > ZEPHYRUM)
        {
            si (n == index)
            {
                redde chorda_ex_s32((s32)i, piscina);
            }
            n++;
        }
    }
    redde chorda_ex_literis("", piscina);
}

hic_manens constans character manifestum[] =
    "<organum genus=\"ludus\" versio=\"1\">"
    "<ligamina><ligamen nomen=\"documentum\" genus=\"ludus-documentum\""
    " a=\"parens\"/></ligamina>"
    "<actiones><actio nomen=\"a.b\"/></actiones><status>"
    "<campus nomen=\"id\" typus=\"nomen\" vita=\"persistens\"/>"
    "<campus nomen=\"electum\" typus=\"numerus\" vita=\"persistens\""
    " scriptor=\"a.b\" referentia=\"documentum/ens\""
    " orbatus=\"summum\"/>"
    "<campus nomen=\"alterum\" typus=\"numerus\" vita=\"persistens\""
    " scriptor=\"a.b\" referentia=\"documentum/ens\""
    " orbatus=\"nullum\"/>"
    "<campus nomen=\"tertium\" typus=\"numerus\" vita=\"persistens\""
    " referentia=\"documentum/ens\"/>"
    "</status></organum>";

nomen structura {
      InsulaRepositorium* repo;
                 Organum* organum;
                 Relatio* relatio;
                Historia* historia;
                 Ludicra  ludicra;
             InsulaRamus  r1;
             InsulaRamus  r2;
} Mundus;

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
            StmlNodus* domini;
    HistoriaProiectio  pr;
    RelatioEnumerator  en;

    memset(m, ZEPHYRUM, magnitudo(*m));
    m->organum = organum_legere(chorda_ex_literis(manifestum, p), p, in,
        &vitia);
    m->repo = insula_repositorium_creare(p, in, "<h/>",
        "<h><ludus id=\"r1\"/><ludus id=\"r2\"/></h>");
    si (!m->organum || !m->repo)
    {
        redde FALSUM;
    }
    causa.mensura = ZEPHYRUM;
    causa.datum = NIHIL;
    canon = organum_canonem_generare(m->organum, p, in, &causa);
    domini = organum_dominos_generare(m->organum, p, in);
    m->r1 = insula_ramus(m->repo, "ludus", "r1");
    m->r2 = insula_ramus(m->repo, "ludus", "r2");
    insula_ramus_canonem_ponere(&m->r1, INSULA_EPHEMERA, canon);
    insula_ramus_canonem_ponere(&m->r2, INSULA_EPHEMERA, canon);
    (vacuum)insula_ramus_dominos_legere(&m->r1, INSULA_EPHEMERA,
        domini);
    (vacuum)insula_ramus_dominos_legere(&m->r2, INSULA_EPHEMERA,
        domini);

    memset(&pr, ZEPHYRUM, magnitudo(pr));
    pr.memoria    = m->ludicra.numeratores;
    pr.mensura    = VIII;
    pr.vacare     = ludicra_vacare;
    pr.applicare  = ludicra_applicare;
    pr.ctx        = &m->ludicra;
    m->historia = historia_creare(p, in, vol, "", "ludus",
        "ludus:checkpoint", LXIV, pr);
    m->relatio  = relatio_creare(p, in, m->repo);
    en.numerus  = entia_numerus;
    en.id       = entis_id;
    redde m->historia && m->relatio
        && relatio_genus_addere(m->relatio, "ludus-documentum", "ens",
               en);
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

/* scriptore dominante (a.b) scribere */
interior b32
scribere (
                  Mundus* m,
    constans InsulaRamus* ramus,
      constans character* par,
                 Piscina* p)
{
    character copia[CXXVIII];

    strcpy(copia, par);
    insula_scriptorem_ponere(m->repo, chorda_ex_literis("a.b", p));
    redde mutare_ramum(ramus, INSULA_EPHEMERA, ponere, copia);
}

interior b32
valor_est (
    constans InsulaRamus* ramus,
      constans character* titulus,
      constans character* valor)
{
    chorda* a;

    a = insula_ramus_attributum(ramus, INSULA_EPHEMERA, titulus);
    redde valor ? (a
        && chorda_aequalis_literis(*a, valor)) : a == NIHIL;
}

interior vacuum
acta (
                Mundus* m,
               Piscina* p,
    constans character* signa)
{
    i32 i;

    per (i = ZEPHYRUM; signa[i]; i++)
    {
        character u[II];

        u[ZEPHYRUM]  = signa[i];
        u[I]         = '\0';
        (vacuum)historia_actum(m->historia, chorda_ex_literis(u, p));
    }
}


/* ==================================================
 * I. summum, nullum, sine orbato; undo non revocat
 * ================================================== */

interior vacuum
_probare_orbatos (
    CredoContextus* c)
{
    InternamentumChorda* in;
                Volumen* vol;
                 Mundus  m;

    in   = internamentum_creare(c->piscina);
    vol  = volumen_temporarium(c->piscina, "probatio_relatio");
    CREDO_NECESSE_NON_NIHIL(vol);
    CREDO_NECESSE(mundum_creare(&m, c->piscina, in, vol));
    acta(&m, c->piscina, "025");                  /* entia 0, 2, 5 */
    CREDO_NECESSE(relatio_ramum_addere(m.relatio, &m.r1, m.organum));
    CREDO_NECESSE(relatio_ligare(m.relatio, &m.r1, "documentum",
        &m.ludicra, m.historia));
    CREDO_VERUM(scribere(&m, &m.r1, "electum=5", c->piscina));
    CREDO_VERUM(scribere(&m, &m.r1, "alterum=5", c->piscina));
    {
        character par[] = "tertium=5";

        insula_scriptorem_ponere(m.repo,
            chorda_ex_literis("tertium.scr",
            c->piscina));
        CREDO_VERUM(mutare_ramum(&m.r1, INSULA_EPHEMERA, ponere, par));
    }

    /* revocatio ens 5 tollit */
    CREDO_VERUM(historia_revocare(m.historia));
    CREDO_VERUM(valor_est(&m.r1, "electum", "2"));   /* summum */
    CREDO_VERUM(valor_est(&m.r1, "alterum", NIHIL)); /* nullum */
    CREDO_VERUM(valor_est(&m.r1, "tertium", "5"));   /* intactus */
    /* scriptor post reparationem restitutus */
    CREDO_CHORDA_AEQUALIS_LITERIS(m.repo->scriptor, "tertium.scr");

    /* refectio ens 5 reddit; electum manet (a: non revocatur) */
    CREDO_VERUM(historia_reficere(m.historia));
    CREDO_VERUM(valor_est(&m.r1, "electum", "2"));

    /* validum: actum novum nihil mutat */
    acta(&m, c->piscina, "7");
    CREDO_VERUM(valor_est(&m.r1, "electum", "2"));

    /* omnia revocata: summum sine entibus = tollitur */
    dum (historia_revocare(m.historia))
    {
    }
    CREDO_VERUM(valor_est(&m.r1, "electum", NIHIL));
    volumen_claudere(vol);
}


/* ==================================================
 * II. ligatio verificat statim
 * ================================================== */

interior vacuum
_probare_ligationem (
    CredoContextus* c)
{
    InternamentumChorda* in;
                Volumen* vol;
                 Mundus  m;

    in   = internamentum_creare(c->piscina);
    vol  = volumen_temporarium(c->piscina, "probatio_relatio");
    CREDO_NECESSE_NON_NIHIL(vol);
    CREDO_NECESSE(mundum_creare(&m, c->piscina, in, vol));
    acta(&m, c->piscina, "13");
    CREDO_VERUM(scribere(&m, &m.r1, "electum=6", c->piscina));
    CREDO_NECESSE(relatio_ramum_addere(m.relatio, &m.r1, m.organum));
    /* ligamen ignotum, ramus ignotus */
    CREDO_FALSUM(relatio_ligare(m.relatio, &m.r1, "pagina", &m.ludicra,
        m.historia));
    CREDO_FALSUM(relatio_ligare(m.relatio, &m.r2, "documentum",
        &m.ludicra, m.historia));
    CREDO_VERUM(valor_est(&m.r1, "electum", "6"));
    CREDO_VERUM(relatio_ligare(m.relatio, &m.r1, "documentum",
        &m.ludicra, m.historia));
    CREDO_VERUM(valor_est(&m.r1, "electum", "3"));
    volumen_claudere(vol);
}


/* ==================================================
 * III. rami plures, historia una: observator semel
 * ================================================== */

interior vacuum
_probare_ramos (
    CredoContextus* c)
{
    InternamentumChorda* in;
                Volumen* vol;
                 Mundus  m;

    in   = internamentum_creare(c->piscina);
    vol  = volumen_temporarium(c->piscina, "probatio_relatio");
    CREDO_NECESSE_NON_NIHIL(vol);
    CREDO_NECESSE(mundum_creare(&m, c->piscina, in, vol));
    acta(&m, c->piscina, "14");
    CREDO_NECESSE(relatio_ramum_addere(m.relatio, &m.r1, m.organum));
    CREDO_NECESSE(relatio_ramum_addere(m.relatio, &m.r2, m.organum));
    CREDO_NECESSE(relatio_ligare(m.relatio, &m.r1, "documentum",
        &m.ludicra, m.historia));
    CREDO_NECESSE(relatio_ligare(m.relatio, &m.r2, "documentum",
        &m.ludicra, m.historia));
    CREDO_AEQUALIS_I32(xar_numerus(m.historia->observatores), I);
    CREDO_VERUM(scribere(&m, &m.r1, "electum=4", c->piscina));
    CREDO_VERUM(scribere(&m, &m.r2, "electum=4", c->piscina));
    CREDO_VERUM(historia_revocare(m.historia));
    CREDO_VERUM(valor_est(&m.r1, "electum", "1"));
    CREDO_VERUM(valor_est(&m.r2, "electum", "1"));
    volumen_claudere(vol);
}


/* ==================================================
 * IV. genus sine enumeratore: intactum; genus bis
 * ================================================== */

interior vacuum
_probare_genera (
    CredoContextus* c)
{
    InternamentumChorda* in;
                Volumen* vol;
                 Mundus  m;
                 Mundus  n;
      RelatioEnumerator  en;

    in   = internamentum_creare(c->piscina);
    vol  = volumen_temporarium(c->piscina, "probatio_relatio");
    CREDO_NECESSE_NON_NIHIL(vol);
    CREDO_NECESSE(mundum_creare(&m, c->piscina, in, vol));
    en.numerus  = entia_numerus;
    en.id       = entis_id;
    CREDO_FALSUM(relatio_genus_addere(m.relatio, "ludus-documentum",
        "ens", en));
    CREDO_VERUM(relatio_genus_addere(m.relatio, "ludus-documentum",
        "aliud", en));

    /* relatio sine generibus */
    CREDO_NECESSE(mundum_creare(&n, c->piscina, in, vol));
    n.relatio = relatio_creare(c->piscina, in, n.repo);
    CREDO_NECESSE_NON_NIHIL(n.relatio);
    acta(&n, c->piscina, "2");
    CREDO_VERUM(scribere(&n, &n.r1, "electum=6", c->piscina));
    CREDO_NECESSE(relatio_ramum_addere(n.relatio, &n.r1, n.organum));
    CREDO_VERUM(relatio_ligare(n.relatio, &n.r1, "documentum",
        &n.ludicra, n.historia));
    CREDO_VERUM(valor_est(&n.r1, "electum", "6"));
    volumen_claudere(vol);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "orbati",   _probare_orbatos,   NIHIL, NIHIL, NIHIL, NIHIL },
    { "ligatio",  _probare_ligationem, NIHIL, NIHIL, NIHIL, NIHIL },
    { "rami",     _probare_ramos,     NIHIL, NIHIL, NIHIL, NIHIL },
    { "genera",   _probare_genera,    NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("relatio", SECTIONES);
}
