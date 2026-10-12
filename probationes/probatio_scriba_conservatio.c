/* probatio_scriba_conservatio.c - scriba per manifestum (status-plan
 * S6b): cursor post restitutionem redit, modus vim ut vim incipit
 * (normalis), nuntius et clavis praecedens transientes; canon et
 * domini ex manifesto in montatione vera vigent. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "thema.h"
#include "volumen.h"
#include "insula.h"
#include "scriba_applicatio.h"
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
       ScribaApplicatio* app,
     constans character* scriptor,
     constans character* par)
{
    character copia[CXXVIII];

    strcpy(copia, par);
    insula_scriptorem_ponere(app->repo, chorda_ex_literis(scriptor,
        app->piscina));
    redde mutare_ramum(&app->montatio.ramus, ponere, copia);
}

interior b32
valor_est (
       ScribaApplicatio* app,
     constans character* titulus,
     constans character* valor)
{
    chorda* a;

    a = insula_ramus_attributum(&app->montatio.ramus, titulus);
    redde valor ? (a
        && chorda_aequalis_literis(*a, valor)) : a == NIHIL;
}

interior vacuum
_probare_circuitum (
    CredoContextus* c)
{
     InternamentumChorda* in;
                 Volumen* vol;
        ScribaApplicatio  prima;
        ScribaApplicatio  altera;

    in = internamentum_creare(c->piscina);
    thema_initiare();
    vol = volumen_temporarium(c->piscina,
        "probatio_scriba_conservatio");
    CREDO_NECESSE_NON_NIHIL(vol);
    memset(&prima, ZEPHYRUM, magnitudo(prima));
    CREDO_NECESSE(scriba_applicatio_aedificare(&prima, c->piscina, in,
        vol,
        NIHIL, CDLXXX, CDLXXX));
    CREDO_VERUM(valor_est(&prima, "modus", "normalis"));
    CREDO_VERUM(scribere(&prima, "pagina.clavis", "cursor_linea=2"));
    CREDO_VERUM(scribere(&prima, "pagina.clavis", "cursor_columna=3"));
    CREDO_VERUM(scribere(&prima, "pagina.clavis", "modus=inserere"));
    CREDO_VERUM(scribere(&prima, "pagina.clavis", "nuntius=error"));
    CREDO_VERUM(scribere(&prima, "pagina.clavis",
        "clavis_praecedens=g"));

    memset(&altera, ZEPHYRUM, magnitudo(altera));
    CREDO_NECESSE(scriba_applicatio_aedificare(&altera, c->piscina, in,
        vol, NIHIL, CDLXXX, CDLXXX));
    /* persistens redit */
    CREDO_VERUM(valor_est(&altera, "cursor_linea", "2"));
    CREDO_VERUM(valor_est(&altera, "cursor_columna", "3"));
    /* transientes: ut vim incipit */
    CREDO_VERUM(valor_est(&altera, "modus", "normalis"));
    CREDO_VERUM(valor_est(&altera, "nuntius", NIHIL));
    CREDO_VERUM(valor_est(&altera, "clavis_praecedens", NIHIL));
    volumen_claudere(vol);
}

interior vacuum
_probare_canonem_dominos (
    CredoContextus* c)
{
     InternamentumChorda* in;
                 Volumen* vol;
        ScribaApplicatio  app;

    in = internamentum_creare(c->piscina);
    thema_initiare();
    vol = volumen_temporarium(c->piscina,
        "probatio_scriba_conservatio");
    CREDO_NECESSE_NON_NIHIL(vol);
    memset(&app, ZEPHYRUM, magnitudo(app));
    CREDO_NECESSE(scriba_applicatio_aedificare(&app, c->piscina, in,
        vol,
        NIHIL, CDLXXX, CDLXXX));
    /* canon: ignotum et electio mala recusantur */
    CREDO_FALSUM(scribere(&app, "pagina.clavis", "ignotissimum=1"));
    CREDO_FALSUM(scribere(&app, "pagina.clavis", "modus=volans"));
    /* domini: cursor solius pagina.clavis */
    CREDO_FALSUM(scribere(&app, "quilibet", "cursor_linea=5"));
    CREDO_VERUM(scribere(&app, "pagina.clavis", "cursor_linea=5"));
    /* basis: superficies solius dispensatoris */
    CREDO_FALSUM(scribere(&app, "quilibet", "superficies_latitudo=10"));
    volumen_claudere(vol);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "circuitus",      _probare_circuitum,       NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "canon_domini",   _probare_canonem_dominos, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("scriba_conservatio", SECTIONES);
}
