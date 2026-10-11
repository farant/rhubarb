/* probatio_insula_systemata.c - domini cum systematibus (status-plan
 * S5a): dominus scribit; systema in indice scribit; systema extra
 * indicem et scriptor alius recusantur; dominus sine systematibus ut
 * olim; in ramo idem. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "stml.h"
#include "insula.h"
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
    redde ramus ? mutare_ramum(ramus, INSULA_EPHEMERA, ponere, copia)
                : mutare_ephemera(repo, ponere, copia);
}

hic_manens constans character domini_textus[] =
    "<domini>"
    "<dominus genus=\"ephemera\" attributum=\"stratum\""
    " scriptor=\"strata.agere\" systemata=\"relatio popover\"/>"
    "<dominus genus=\"ephemera\" attributum=\"zoom\""
    " scriptor=\"motus\"/>"
    "</domini>";

interior StmlNodus*
domini (
                Piscina* p,
    InternamentumChorda* in)
{
    redde stml_legere_ex_literis(domini_textus, p, in).elementum_radix;
}

interior vacuum
_probare_radicem (
    CredoContextus* c)
{
    InternamentumChorda* in;
     InsulaRepositorium* repo;

    in   = internamentum_creare(c->piscina);
    repo = insula_repositorium_creare(c->piscina, in, "<r/>",
        "<r stratum=\"1\" zoom=\"1\"/>");
    CREDO_NECESSE_NON_NIHIL(repo);
    CREDO_AEQUALIS_I32(insula_dominos_legere(repo, INSULA_EPHEMERA,
        domini(c->piscina, in)), II);

    CREDO_VERUM(scribere(repo, NIHIL, "strata.agere", "stratum=2",
        c->piscina));
    /* systemata in indice */
    CREDO_VERUM(scribere(repo, NIHIL, "relatio", "stratum=3",
        c->piscina));
    CREDO_VERUM(scribere(repo, NIHIL, "popover", "stratum=4",
        c->piscina));
    /* systema extra indicem, scriptor alius */
    CREDO_FALSUM(scribere(repo, NIHIL, "motus", "stratum=5",
        c->piscina));
    CREDO_CHORDA_CONTINET(insula_causa(repo), chorda_ex_literis(
        "relatio", c->piscina));
    CREDO_FALSUM(scribere(repo, NIHIL, "quilibet", "stratum=5",
        c->piscina));
    /* praefixum nominis systematis non sufficit */
    CREDO_FALSUM(scribere(repo, NIHIL, "rel", "stratum=5", c->piscina));
    CREDO_FALSUM(scribere(repo, NIHIL, "relatio popover", "stratum=5",
        c->piscina));
    /* dominus sine systematibus: ut olim */
    CREDO_VERUM(scribere(repo, NIHIL, "motus", "zoom=2", c->piscina));
    CREDO_FALSUM(scribere(repo, NIHIL, "relatio", "zoom=3",
        c->piscina));
}

interior vacuum
_probare_ramum (
    CredoContextus* c)
{
     InternamentumChorda* in;
      InsulaRepositorium* repo;
             InsulaRamus  ramus;

    in   = internamentum_creare(c->piscina);
    repo = insula_repositorium_creare(c->piscina, in, "<h/>",
        "<h><w id=\"a\" stratum=\"1\" zoom=\"1\"/></h>");
    CREDO_NECESSE_NON_NIHIL(repo);
    ramus = insula_ramus(repo, "w", "a");
    CREDO_AEQUALIS_I32(insula_ramus_dominos_legere(&ramus,
        INSULA_EPHEMERA, domini(c->piscina, in)), II);
    CREDO_VERUM(scribere(repo, &ramus, "relatio", "stratum=7",
        c->piscina));
    CREDO_FALSUM(scribere(repo, &ramus, "quilibet", "stratum=8",
        c->piscina));
    CREDO_FALSUM(scribere(repo, &ramus, "relatio", "zoom=4",
        c->piscina));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "radix", _probare_radicem, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ramus", _probare_ramum,   NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("insula_systemata", SECTIONES);
}
