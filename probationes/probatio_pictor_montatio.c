/* probatio_pictor_montatio.c - montatio vera pictoris (status-plan
 * S2): canon et domini EX MANIFESTO in ramo montato vigent (via
 * pictor_applicatio_aedificare -> pictor_montare, non insulae manu
 * structae); manifestum absens = montatio recusatur. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "thema.h"
#include "volumen.h"
#include "insula.h"
#include "pictor_applicatio.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

interior vacuum
ponere_ignotum (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    (vacuum)ctx;
    insula_attributum_ponere(radix, p, in, "ignotissimum", "x");
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

/* scriptore dato in ramo scribere */
interior b32
scribere (
        PictorApplicatio* app,
             InsulaGenus  genus,
      constans character* scriptor,
      constans character* par)
{
    character copia[CXXVIII];

    strcpy(copia, par);
    insula_scriptorem_ponere(app->repo, chorda_ex_literis(scriptor,
        app->piscina));
    redde mutare_ramum(&app->montatio.ramus, genus, ponere, copia);
}

interior vacuum
_probare_montationem (
    CredoContextus* c)
{
     InternamentumChorda* in;
                 Volumen* vol;
        PictorApplicatio  app;

    in = internamentum_creare(c->piscina);
    thema_initiare();
    vol = volumen_temporarium(c->piscina, "probatio_pictor_montatio");
    CREDO_NECESSE_NON_NIHIL(vol);
    memset(&app, ZEPHYRUM, magnitudo(app));
    CREDO_NECESSE(pictor_applicatio_aedificare(&app, c->piscina, in,
        vol,
        NIHIL, CDLXXX, CDLXXX));

    /* canon generatus in utraque insula */
    insula_scriptorem_ponere(app.repo, chorda_ex_literis("quilibet",
        c->piscina));
    CREDO_FALSUM(mutare_ramum(&app.montatio.ramus, INSULA_EPHEMERA,
        ponere_ignotum, NIHIL));
    CREDO_FALSUM(mutare_ramum(&app.montatio.ramus, INSULA_DURABILIS,
        ponere_ignotum, NIHIL));
    /* typus: zoom numerus */
    CREDO_FALSUM(scribere(&app, INSULA_EPHEMERA, "motus",
        "zoom=magnus"));

    /* domini generati: zoom solius motus */
    CREDO_FALSUM(scribere(&app, INSULA_EPHEMERA, "zoom.ponere",
        "zoom=2"));
    CREDO_VERUM(scribere(&app, INSULA_EPHEMERA, "motus", "zoom=2"));
    /* basis: focus solius dispensatoris */
    CREDO_FALSUM(scribere(&app, INSULA_EPHEMERA, "quilibet",
        "focus=quadratum"));
    CREDO_VERUM(scribere(&app, INSULA_EPHEMERA, "dispensator",
        "focus=quadratum"));
    /* aspectus popover: sine domino */
    CREDO_VERUM(scribere(&app, INSULA_EPHEMERA, "quilibet",
        "palette=color_primus"));

    volumen_claudere(vol);
}

/* manifestum absens: montatio recusatur, non tacite sine canone */
interior vacuum
_probare_manifestum_absens (
    CredoContextus* c)
{
     InternamentumChorda* in;
                 Volumen* vol;
        PictorApplicatio  app;

    in = internamentum_creare(c->piscina);
    thema_initiare();
    vol = volumen_temporarium(c->piscina, "probatio_pictor_montatio");
    CREDO_NECESSE_NON_NIHIL(vol);
    memset(&app, ZEPHYRUM, magnitudo(app));
    CREDO_FALSUM(pictor_applicatio_aedificare(&app, c->piscina, in, vol,
        "/via/nulla", CDLXXX, CDLXXX));
    volumen_claudere(vol);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "montatio",         _probare_montationem,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { "manifestum_absens", _probare_manifestum_absens,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("pictor_montatio", SECTIONES);
}
