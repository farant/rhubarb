/* probatio_pictor_montatio.c - montatio vera pictoris (status-plan
 * S2): canon et domini EX MANIFESTO in ramo montato vigent (via
 * pictor_applicatio_aedificare -> pictor_montare, non insulae manu
 * structae); manifestum absens = montatio recusatur; S3: status
 * persistens per sessiones servatur. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "thema.h"
#include "volumen.h"
#include "insula.h"
#include "pictor_applicatio.h"
#include "filum.h"
#include "organum.h"
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
      constans character* scriptor,
      constans character* par)
{
    character copia[CXXVIII];

    strcpy(copia, par);
    insula_scriptorem_ponere(app->repo, chorda_ex_literis(scriptor,
        app->piscina));
    redde mutare_ramum(&app->montatio.ramus, ponere, copia);
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
    CREDO_FALSUM(mutare_ramum(&app.montatio.ramus,
        ponere_ignotum, NIHIL));
    CREDO_FALSUM(mutare_ramum(&app.montatio.ramus,
        ponere_ignotum, NIHIL));
    /* typus: zoom numerus */
    CREDO_FALSUM(scribere(&app, "motus",
        "zoom=magnus"));

    /* domini generati: zoom solius motus */
    CREDO_FALSUM(scribere(&app, "zoom.ponere",
        "zoom=2"));
    CREDO_VERUM(scribere(&app, "motus", "zoom=2"));
    /* basis: focus solius dispensatoris */
    CREDO_FALSUM(scribere(&app, "quilibet",
        "focus=quadratum"));
    CREDO_VERUM(scribere(&app, "dispensator",
        "focus=quadratum"));
    /* S5c: popover - dominus aperiens et systema dimissio soli */
    CREDO_FALSUM(scribere(&app, "quilibet",
        "palette=color_primus"));
    CREDO_VERUM(scribere(&app, "palette.aperire",
        "palette=color_primus"));
    CREDO_VERUM(scribere(&app, "dimissio",
        "palette=exemplar"));
    /* S5a: systema relatio referentiam scribit, nihil aliud */
    CREDO_VERUM(scribere(&app, "relatio",
        "stratum_activum=9"));
    CREDO_FALSUM(scribere(&app, "quilibet",
        "stratum_activum=8"));
    CREDO_FALSUM(scribere(&app, "relatio", "zoom=5"));

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

/* valor in ramo aequat literas? (NIHIL = abest) */
interior b32
valor_est (
        PictorApplicatio* app,
      constans character* titulus,
      constans character* valor)
{
    chorda* a;

    a = insula_ramus_attributum(&app->montatio.ramus,
        titulus);
    si (!valor)
    {
        redde a == NIHIL;
    }
    redde a && chorda_aequalis_literis(*a, valor);
}

/* S3: status persistens in volumine applicationis; sessio altera
 * (repositorium recens, idem volumen) eum restituit; transiens non */
interior vacuum
_probare_conservationem (
    CredoContextus* c)
{
     InternamentumChorda* in;
                 Volumen* vol;
        PictorApplicatio  prima;
        PictorApplicatio  altera;

    in = internamentum_creare(c->piscina);
    thema_initiare();
    vol = volumen_temporarium(c->piscina, "probatio_pictor_montatio");
    CREDO_NECESSE_NON_NIHIL(vol);
    memset(&prima, ZEPHYRUM, magnitudo(prima));
    CREDO_NECESSE(pictor_applicatio_aedificare(&prima, c->piscina, in,
        vol, NIHIL, CDLXXX, CDLXXX));
    CREDO_VERUM(valor_est(&prima, "instrumentum", "penicillus"));
    CREDO_VERUM(scribere(&prima,
        "instrumentum.eligere",
        "instrumentum=linea"));
    CREDO_VERUM(scribere(&prima, "color_primus.ponere",
        "color_primus=5"));
    CREDO_VERUM(scribere(&prima, "motus", "zoom=2"));
    CREDO_VERUM(scribere(&prima, "palette.aperire",
        "palette=color_primus"));

    memset(&altera, ZEPHYRUM, magnitudo(altera));
    CREDO_NECESSE(pictor_applicatio_aedificare(&altera, c->piscina, in,
        vol, NIHIL, CDLXXX, CDLXXX));
    CREDO_VERUM(valor_est(&altera, "instrumentum", "linea"));
    CREDO_VERUM(valor_est(&altera, "color_primus", "5"));
    CREDO_VERUM(valor_est(&altera, "zoom", "2"));
    /* transiens: palette clausa */
    CREDO_VERUM(valor_est(&altera, "palette", NIHIL));
    /* domini post restitutionem vigent */
    CREDO_FALSUM(scribere(&altera, "zoom.ponere",
        "zoom=3"));
    volumen_claudere(vol);
}

/* S4: insula initialis = absentes manifesti VIVI (omnis campus
 * insulae cum absente, valore suo) + focus pictoris */
interior vacuum
_probare_initium (
    CredoContextus* c)
{
     InternamentumChorda* in;
                 Volumen* vol;
        PictorApplicatio  app;
                 Organum* o;
                     Xar* vitia;
                     i32  i;
                     s32  numerati;

    in = internamentum_creare(c->piscina);
    thema_initiare();
    o = organum_legere(filum_legere_totum("apps/pictor/organum.stml",
        c->piscina), c->piscina, in, &vitia);
    CREDO_NECESSE_NON_NIHIL(o);
    vol = volumen_temporarium(c->piscina, "probatio_pictor_montatio");
    CREDO_NECESSE_NON_NIHIL(vol);
    memset(&app, ZEPHYRUM, magnitudo(app));
    CREDO_NECESSE(pictor_applicatio_aedificare(&app, c->piscina, in,
        vol,
        NIHIL, CDLXXX, CDLXXX));
    numerati = ZEPHYRUM;
    per (i = ZEPHYRUM; i < organum_numerus_camporum(o); i++)
    {
        constans OrganumCampus* k;

        k = organum_campus_index(o, i);
        si (!k->habet_absens || k->sedes != ORGANUM_SEDES_INSULA)
        {
            perge;
        }
        CREDO_VERUM(valor_est(&app, chorda_ut_cstr(k->titulus,
            c->piscina), chorda_ut_cstr(k->absens, c->piscina)));
        numerati++;
    }
    CREDO_AEQUALIS_S32(numerati, X);
    CREDO_VERUM(valor_est(&app, "instrumentum", "penicillus"));
    CREDO_VERUM(valor_est(&app, "focus", "tabula"));
    volumen_claudere(vol);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "montatio",         _probare_montationem,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { "manifestum_absens", _probare_manifestum_absens,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { "conservatio",      _probare_conservationem,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { "initium",          _probare_initium,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("pictor_montatio", SECTIONES);
}
