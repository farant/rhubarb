/* probatio_organum_profundum.c - organa quavis profunditate
 * (status-plan S8b): organum ludicrum
 * (probationes/fixa/organum/ludibrium.stml) se ipsum montat, tres
 * gradus, id idem "liber" in omni gradu (ut montatio vera). Quisque
 * gradus: initium ex manifesto, canon suus, domini; conservatio per
 * viam (clavis = via, non id); relatio ramos eiusdem segmenti ultimi
 * sub parentibus diversis distinguit. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "stml.h"
#include "volumen.h"
#include "insula.h"
#include "organum.h"
#include "organum_montatio.h"
#include "relatio.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define VIA_LUDIBRII "probationes/fixa/organum/ludibrium.stml"

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
      constans character* scriptor,
      constans character* par,
                 Piscina* p)
{
    character copia[CXXVIII];

    strcpy(copia, par);
    insula_scriptorem_ponere(ramus->repo, chorda_ex_literis(scriptor,
        p));
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

/* tres gradus ludibrii, id "liber" ubique; clavis conservationis =
 * via rami */
nomen structura {
    InsulaRepositorium* repo;
           InsulaRamus  g[III];
} Gradus;

interior b32
gradus_montare (
                  Gradus* x,
        constans Organum* o,
         StatusSystemata* s,
                 Piscina* p,
     InternamentumChorda* in)
{
    InsulaRamus radix;
            i32 i;

    radix = insula_ramus_radix(x->repo);
    per (i = ZEPHYRUM; i < III; i++)
    {
        x->g[i] = insula_ramus_liberi(i == ZEPHYRUM ? &radix
            : &x->g[i - I], "ludibrium", "liber");
        si (!organum_ramum_parare(o, &x->g[i], s, NIHIL,
                chorda_ut_cstr(x->g[i].via, p), p, in))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior Organum*
ludibrium (
                Piscina* p,
    InternamentumChorda* in)
{
    redde organum_legere_viam(p, in, NIHIL, VIA_LUDIBRII, "probatio");
}

/* tres gradus: initium, canon et domini cuiusque */
interior vacuum
_probare_gradus (
    CredoContextus* c)
{
    InternamentumChorda* in;
                Organum* o;
                 Gradus  x;
                    i32  i;

    in  = internamentum_creare(c->piscina);
    o   = ludibrium(c->piscina, in);
    CREDO_NECESSE_NON_NIHIL(o);
    x.repo = insula_repositorium_creare(c->piscina, in, "<hospes/>");
    CREDO_NECESSE_NON_NIHIL(x.repo);
    CREDO_NECESSE(gradus_montare(&x, o, NIHIL, c->piscina, in));
    CREDO_CHORDA_AEQUALIS_LITERIS(x.g[II].via,
        "ludibrium:liber/ludibrium:liber/ludibrium:liber");
    per (i = ZEPHYRUM; i < III; i++)
    {
        CREDO_VERUM(insula_ramus_nodus(&x.g[i]) != NIHIL);
        CREDO_VERUM(valor_est(&x.g[i], "valor", "0"));
    }
    /* filius sub parente, non sub radice */
    CREDO_VERUM(stml_numerus_liberorum(insula_radix(x.repo)) == I);
    CREDO_VERUM(stml_numerus_liberorum(insula_ramus_nodus(&x.g[I]))
        == I);
    /* canon generatus in omni gradu; filius parentem non turbat */
    CREDO_VERUM(scribere(&x.g[ZEPHYRUM], "ludibrium.agere", "valor=1",
        c->piscina));
    CREDO_FALSUM(scribere(&x.g[II], "ludibrium.agere", "ignotum=1",
        c->piscina));
    CREDO_FALSUM(scribere(&x.g[II], "ludibrium.agere", "valor=magnus",
        c->piscina));
    /* domini in omni gradu */
    CREDO_FALSUM(scribere(&x.g[I], "quilibet", "valor=2", c->piscina));
    CREDO_VERUM(scribere(&x.g[I], "ludibrium.agere", "valor=2",
        c->piscina));
    CREDO_VERUM(valor_est(&x.g[I], "valor", "2"));
    CREDO_VERUM(valor_est(&x.g[ZEPHYRUM], "valor", "1"));
    CREDO_VERUM(valor_est(&x.g[II], "valor", "0"));
}

/* conservatio per viam: sessio altera omnes gradus restituit;
 * transiens non redit */
interior vacuum
_probare_conservationem (
    CredoContextus* c)
{
    InternamentumChorda* in;
                Volumen* vol;
                Organum* o;
                 Gradus  prima;
                 Gradus  altera;
        StatusSystemata  s;

    in = internamentum_creare(c->piscina);
    vol = volumen_temporarium(c->piscina,
        "probatio_organum_profundum");
    CREDO_NECESSE_NON_NIHIL(vol);
    o    = ludibrium(c->piscina, in);
    CREDO_NECESSE_NON_NIHIL(o);

    prima.repo = insula_repositorium_creare(c->piscina, in,
        "<hospes/>");
    CREDO_NECESSE_NON_NIHIL(prima.repo);
    memset(&s, ZEPHYRUM, magnitudo(s));
    s.conservatio = conservatio_creare(c->piscina, in, vol, prima.repo);
    CREDO_NECESSE_NON_NIHIL(s.conservatio);
    CREDO_NECESSE(gradus_montare(&prima, o, &s, c->piscina, in));
    CREDO_VERUM(scribere(&prima.g[ZEPHYRUM], "ludibrium.agere",
        "valor=1", c->piscina));
    CREDO_VERUM(scribere(&prima.g[I], "ludibrium.agere", "valor=2",
        c->piscina));
    CREDO_VERUM(scribere(&prima.g[II], "ludibrium.agere", "valor=3",
        c->piscina));
    CREDO_VERUM(scribere(&prima.g[II], "ludibrium.agere", "nota=x",
        c->piscina));

    altera.repo = insula_repositorium_creare(c->piscina, in,
        "<hospes/>");
    CREDO_NECESSE_NON_NIHIL(altera.repo);
    memset(&s, ZEPHYRUM, magnitudo(s));
    s.conservatio = conservatio_creare(c->piscina, in, vol,
        altera.repo);
    CREDO_NECESSE_NON_NIHIL(s.conservatio);
    CREDO_NECESSE(gradus_montare(&altera, o, &s, c->piscina, in));
    CREDO_VERUM(valor_est(&altera.g[ZEPHYRUM], "valor", "1"));
    CREDO_VERUM(valor_est(&altera.g[I], "valor", "2"));
    CREDO_VERUM(valor_est(&altera.g[II], "valor", "3"));
    CREDO_VERUM(valor_est(&altera.g[II], "nota", NIHIL));
    volumen_claudere(vol);
}

/* relatio: "liber" sub parentibus diversis = rami duo */
interior vacuum
_probare_relationem (
    CredoContextus* c)
{
     InternamentumChorda* in;
      InsulaRepositorium* repo;
                 Organum* o;
                 Relatio* r;
             InsulaRamus  radix;
             InsulaRamus  p;
             InsulaRamus  q;
             InsulaRamus  liber_p;
             InsulaRamus  liber_q;

    in  = internamentum_creare(c->piscina);
    o   = ludibrium(c->piscina, in);
    CREDO_NECESSE_NON_NIHIL(o);
    repo  = insula_repositorium_creare(c->piscina, in, "<hospes/>");
    CREDO_NECESSE_NON_NIHIL(repo);
    radix    = insula_ramus_radix(repo);
    p        = insula_ramus_liberi(&radix, "ludibrium", "p");
    q        = insula_ramus_liberi(&radix, "ludibrium", "q");
    liber_p  = insula_ramus_liberi(&p, "ludibrium", "liber");
    liber_q  = insula_ramus_liberi(&q, "ludibrium", "liber");
    r        = relatio_creare(c->piscina, in, repo);
    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_VERUM(relatio_ramum_addere(r, &liber_p, o));
    CREDO_VERUM(relatio_ramum_addere(r, &liber_q, o));
    /* idem ramus bis: recusatur */
    CREDO_FALSUM(relatio_ramum_addere(r, &liber_p, o));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "gradus",       _probare_gradus,          NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "conservatio",  _probare_conservationem,  NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "relatio",      _probare_relationem,      NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("organum_profundum", SECTIONES);
}
