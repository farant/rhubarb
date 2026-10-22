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
#include "organa_infixa.h"
#include "filum.h"
#include "iter_directoria.h"
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

/* S7c-1: manifestum infixum per genus (applicationes sine arbore) */
interior vacuum
_probare_legere_genus (
    CredoContextus* c)
{
    InternamentumChorda* in;
                Organum* o;

    in  = internamentum_creare(c->piscina);
    o   = organum_legere_genus(c->piscina, in, "pictor", "probatio");
    CREDO_NECESSE_NON_NIHIL(o);
    CREDO_CHORDA_AEQUALIS_LITERIS(organum_genus(o), "pictor");
    CREDO_NON_NIHIL(organum_campus(o, chorda_ex_literis("zoom",
        c->piscina)));
    /* basis communis (infixa et ipsa) addita */
    CREDO_NON_NIHIL(organum_campus(o, chorda_ex_literis("focus",
        c->piscina)));
    /* genus ignotum */
    CREDO_NIHIL(organum_legere_genus(c->piscina, in, "nullum",
        "probatio"));
}

/* manifestum infixum = fons in disco; stalum nominatur */
interior b32
infixum_congruit (
                Piscina* p,
    InternamentumChorda* in,
     constans character* genus,
     constans character* via)
{
     chorda  fons;
     chorda  infixum;
    Organum* o;

    fons     = filum_legere_totum(via, p);
    infixum  = organum_infixum_textus(genus, p);
    si (fons.mensura == ZEPHYRUM || !chorda_aequalis(fons, infixum))
    {
        fprintf(stderr, "infixum stalum: %s (genus %s) - "
            "./tools/organa_infigere.sh\n", via, genus);
        redde FALSUM;
    }
    /* clavis tabulae = genus manifesti */
    si (chorda_aequalis_literis(chorda_ex_literis(genus, p),
            "organum_commune"))
    {
        redde VERUM;
    }
    o = organum_legere_genus(p, in, genus, "probatio");
    si (!o || !chorda_aequalis_literis(organum_genus(o), genus))
    {
        fprintf(stderr, "infixum: %s genus non %s\n", via, genus);
        redde FALSUM;
    }
    redde VERUM;
}

/* S7c-1: omne apps/<genus>/organum.stml infixum et recens (porta
 * generata idem in commissione iudicat; hic in probatione statim) */
interior vacuum
_probare_infixa_fontes (
    CredoContextus* c)
{
     InternamentumChorda* in;
     DirectoriumIterator* it;
    DirectoriumIntroitus* introitus;
                  chorda  via;
      constans character* genus;
                     i32  numerus;

    in  = internamentum_creare(c->piscina);
    CREDO_VERUM(infixum_congruit(c->piscina, in, "organum_commune",
        "apps/organum_commune.stml"));
    it  = directorium_iterator_aperire("apps", c->piscina);
    CREDO_NECESSE_NON_NIHIL(it);
    numerus = ZEPHYRUM;
    dum ((introitus = directorium_iterator_proximum(it)) != NIHIL)
    {
        si (introitus->genus != INTROITUS_DIRECTORIUM)
        {
            perge;
        }
        genus = chorda_ut_cstr(introitus->titulus, c->piscina);
        via = chorda_concatenare(chorda_ex_literis("apps/", c->piscina),
            introitus->titulus, c->piscina);
        via = chorda_concatenare(via, chorda_ex_literis(
            "/organum.stml", c->piscina), c->piscina);
        si (!filum_existit(chorda_ut_cstr(via, c->piscina)))
        {
            perge;
        }
        numerus++;
        CREDO_VERUM(infixum_congruit(c->piscina, in, genus,
            chorda_ut_cstr(via, c->piscina)));
    }
    directorium_iterator_claudere(it);
    /* pictor, scriba, vicus saltem */
    CREDO_VERUM(numerus >= III);
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
    CREDO_NECESSE(volumen_praesens_ponere(vol, chorda_ex_literis(
        "status/p1", c->piscina), chorda_ex_literis(
        "<status versio=\"2\" genus=\"pictor\" zoom=\"3\"/>",
        c->piscina)));
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
    { "legere_genus", _probare_legere_genus, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "infixa_fontes", _probare_infixa_fontes, NIHIL, NIHIL, NIHIL,
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
