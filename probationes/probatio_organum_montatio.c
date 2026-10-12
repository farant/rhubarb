/* probatio_organum_montatio.c - organum in ramo montare (status-plan
 * S6a): manifestum per viam cum basi; ramus radicis et liberi parati
 * (canon, initium, domini); status servatus restituitur; addita
 * canoni obnoxia. Manifestum verum pictoris adhibetur. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "stml.h"
#include "volumen.h"
#include "insula.h"
#include "organum.h"
#include "organum_montatio.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

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
      InsulaRepositorium* repo,
    constans InsulaRamus* ramus,
      constans character* scriptor,
      constans character* par,
                 Piscina* p)
{
    character copia[CXXVIII];

    strcpy(copia, par);
    insula_scriptorem_ponere(repo, chorda_ex_literis(scriptor, p));
    redde mutare_ramum(ramus, ponere, copia);
}

interior b32
valor_est (
    constans InsulaRamus* ramus,
      constans character* titulus,
      constans character* valor)
{
    chorda* a;

    a = insula_ramus_attributum(ramus, titulus);
    redde valor ? (a
        && chorda_aequalis_literis(*a, valor)) : a == NIHIL;
}

interior Organum*
pictor (
                Piscina* p,
    InternamentumChorda* in)
{
    redde organum_legere_viam(p, in, NIHIL, "apps/pictor/organum.stml",
        "probatio");
}


interior vacuum
_probare_legere_viam (
    CredoContextus* c)
{
    InternamentumChorda* in;
                Organum* o;

    in  = internamentum_creare(c->piscina);
    o   = pictor(c->piscina, in);
    CREDO_NECESSE_NON_NIHIL(o);
    CREDO_CHORDA_AEQUALIS_LITERIS(organum_genus(o), "pictor");
    CREDO_NON_NIHIL(organum_campus(o, chorda_ex_literis("zoom",
        c->piscina)));
    /* basis communis addita */
    CREDO_NON_NIHIL(organum_campus(o, chorda_ex_literis("focus",
        c->piscina)));
    /* via absens */
    CREDO_NIHIL(organum_legere_viam(c->piscina, in, NIHIL,
        "apps/nullum/organum.stml", "probatio"));
}

interior vacuum
_probare_radicem (
    CredoContextus* c)
{
     InternamentumChorda* in;
      InsulaRepositorium* repo;
             InsulaRamus  r;
                 Organum* o;

    in  = internamentum_creare(c->piscina);
    o   = pictor(c->piscina, in);
    CREDO_NECESSE_NON_NIHIL(o);
    repo = insula_repositorium_creare(c->piscina, in, "<pictor/>");
    CREDO_NECESSE_NON_NIHIL(repo);
    r = insula_ramus_radix(repo);
    CREDO_NECESSE(organum_ramum_parare(o, &r, NIHIL,
        " latitudo=\"40\" focus=\"tabula\"", "pictor", c->piscina, in));
    /* initium: absentes manifesti + addita */
    CREDO_VERUM(valor_est(&r, "zoom", "1"));
    CREDO_VERUM(valor_est(&r, "instrumentum", "penicillus"));
    CREDO_VERUM(valor_est(&r, "latitudo", "40"));
    CREDO_VERUM(valor_est(&r, "focus", "tabula"));
    /* canon generatus */
    CREDO_FALSUM(scribere(repo, &r, "motus", "ignotissimum=1",
        c->piscina));
    CREDO_FALSUM(scribere(repo, &r, "motus", "zoom=magnus",
        c->piscina));
    /* domini */
    CREDO_FALSUM(scribere(repo, &r, "quilibet", "zoom=2", c->piscina));
    CREDO_VERUM(scribere(repo, &r, "motus", "zoom=2", c->piscina));
    CREDO_FALSUM(scribere(repo, &r, "quilibet", "palette=x",
        c->piscina));
    CREDO_VERUM(scribere(repo, &r, "dimissio", "palette=x",
        c->piscina));
}

interior vacuum
_probare_ramum (
    CredoContextus* c)
{
     InternamentumChorda* in;
      InsulaRepositorium* repo;
             InsulaRamus  r;
                 Organum* o;

    in  = internamentum_creare(c->piscina);
    o   = pictor(c->piscina, in);
    CREDO_NECESSE_NON_NIHIL(o);
    repo = insula_repositorium_creare(c->piscina, in, "<hospes/>");
    CREDO_NECESSE_NON_NIHIL(repo);
    r = insula_ramus(repo, "pictor", "p1");
    CREDO_NECESSE(organum_ramum_parare(o, &r, NIHIL, "", "p1",
        c->piscina, in));
    CREDO_NON_NIHIL(insula_ramus_nodus(&r));
    CREDO_VERUM(valor_est(&r, "id", "p1"));
    CREDO_VERUM(valor_est(&r, "zoom", "1"));
    /* addita extra canonem: initiatio recusatur */
    r = insula_ramus(repo, "pictor", "p2");
    CREDO_FALSUM(organum_ramum_parare(o, &r, NIHIL,
        " ignotissimum=\"1\"", "p2", c->piscina, in));
    CREDO_NIHIL(insula_ramus_nodus(&r));
}

interior vacuum
_probare_restitutionem (
    CredoContextus* c)
{
     InternamentumChorda* in;
                 Volumen* vol;
      InsulaRepositorium* repo;
             InsulaRamus  r;
                 Organum* o;
         StatusSystemata  s;

    in   = internamentum_creare(c->piscina);
    vol  = volumen_temporarium(c->piscina, "probatio_organum_montatio");
    CREDO_NECESSE_NON_NIHIL(vol);
    CREDO_NECESSE(volumen_plagulam_condere(vol, chorda_ex_literis(
        "status/p1", c->piscina), chorda_ex_literis(
        "<status versio=\"2\" genus=\"pictor\" zoom=\"3\"/>",
        c->piscina), "probatio") > ZEPHYRUM);
    o    = pictor(c->piscina, in);
    CREDO_NECESSE_NON_NIHIL(o);
    repo = insula_repositorium_creare(c->piscina, in, "<hospes/>");
    CREDO_NECESSE_NON_NIHIL(repo);
    memset(&s, ZEPHYRUM, magnitudo(s));
    s.conservatio = conservatio_creare(c->piscina, in, vol, repo);
    CREDO_NECESSE_NON_NIHIL(s.conservatio);
    r = insula_ramus(repo, "pictor", "p1");
    CREDO_NECESSE(organum_ramum_parare(o, &r, &s, "", "p1", c->piscina,
        in));
    CREDO_VERUM(valor_est(&r, "zoom", "3"));
    /* restitutio ante dominos: zoom tamen a motu solo scribitur */
    CREDO_FALSUM(scribere(repo, &r, "quilibet", "zoom=4", c->piscina));
    volumen_claudere(vol);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "legere_viam",  _probare_legere_viam,  NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "radix",        _probare_radicem,      NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "ramus",        _probare_ramum,        NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "restitutio",   _probare_restitutionem, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("organum_montatio", SECTIONES);
}
