/* probatio_insula_profunda.c - rami quavis profunditate (status-plan
 * S8a): via internata a radice ("a:a1/b:b1/c:c1"), insula_ramus_liberi,
 * initiatio sub parente (parens absens recusatur), scriptura, domini et
 * canones per viam - canon nodum suum iudicat sine ramis descendentibus
 * qui canonem proprium habent (proximus vincit). */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "stml.h"
#include "canon.h"
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

/* tres gradus initiati: radix > a:a1 > b:b1 > c:c1 */
nomen structura {
    InsulaRepositorium* repo;
           InsulaRamus  a;
           InsulaRamus  b;
           InsulaRamus  c;
} Arbor;

interior b32
arborem_struere (
                  Arbor* x,
                Piscina* p,
    InternamentumChorda* in)
{
    InsulaRamus radix;

    x->repo = insula_repositorium_creare(p, in, "<radix/>");
    si (!x->repo)
    {
        redde FALSUM;
    }
    radix  = insula_ramus_radix(x->repo);
    x->a   = insula_ramus_liberi(&radix, "a", "a1");
    x->b   = insula_ramus_liberi(&x->a, "b", "b1");
    x->c   = insula_ramus_liberi(&x->b, "c", "c1");
    redde insula_ramum_initiare(&x->a, "<a id=\"a1\"/>")
        && insula_ramum_initiare(&x->b, "<b id=\"b1\"/>")
        && insula_ramum_initiare(&x->c, "<c id=\"c1\"/>");
}

/* via: segmenta "elementum:id" per '/'; ultimum in elementum et id */
interior vacuum
_probare_viam (
    CredoContextus* c)
{
     InternamentumChorda* in;
      InsulaRepositorium* repo;
             InsulaRamus  radix;
             InsulaRamus  a;
             InsulaRamus  b;
             InsulaRamus  vetus;

    in    = internamentum_creare(c->piscina);
    repo  = insula_repositorium_creare(c->piscina, in, "<radix/>");
    CREDO_NECESSE_NON_NIHIL(repo);
    radix = insula_ramus_radix(repo);
    CREDO_CHORDA_VACUA(radix.via);
    a = insula_ramus_liberi(&radix, "a", "a1");
    b = insula_ramus_liberi(&a, "b", "b1");
    CREDO_CHORDA_AEQUALIS_LITERIS(a.via, "a:a1");
    CREDO_CHORDA_AEQUALIS_LITERIS(b.via, "a:a1/b:b1");
    CREDO_CHORDA_AEQUALIS_LITERIS(b.elementum, "b");
    CREDO_CHORDA_AEQUALIS_LITERIS(b.id, "b1");
    /* insula_ramus = liberum radicis */
    vetus = insula_ramus(repo, "a", "a1");
    CREDO_CHORDA_AEQUALIS_LITERIS(vetus.via, "a:a1");
    /* internata: eadem via, idem pointer */
    CREDO_VERUM(vetus.via.datum == a.via.datum);
}

/* initiatio sub parente; parens absens recusatur (causa nominata) */
interior vacuum
_probare_initiationem (
    CredoContextus* c)
{
     InternamentumChorda* in;
      InsulaRepositorium* repo;
             InsulaRamus  radix;
             InsulaRamus  a;
             InsulaRamus  b;
                   Arbor  x;
               StmlNodus* n;

    in    = internamentum_creare(c->piscina);
    repo  = insula_repositorium_creare(c->piscina, in, "<radix/>");
    CREDO_NECESSE_NON_NIHIL(repo);
    radix  = insula_ramus_radix(repo);
    a      = insula_ramus_liberi(&radix, "a", "a1");
    b      = insula_ramus_liberi(&a, "b", "b1");
    /* b ante a: parens abest */
    CREDO_FALSUM(insula_ramum_initiare(&b, "<b id=\"b1\"/>"));
    CREDO_CHORDA_CONTINET(insula_causa(repo),
        chorda_ex_literis("parens",
        c->piscina));
    CREDO_NIHIL(insula_ramus_nodus(&b));

    CREDO_NECESSE(arborem_struere(&x, c->piscina, in));
    n = insula_ramus_nodus(&x.c);
    CREDO_NECESSE_NON_NIHIL(n);
    CREDO_CHORDA_AEQUALIS_LITERIS(*n->titulus, "c");
    /* c sub b, non sub radice */
    CREDO_VERUM(stml_numerus_liberorum(insula_radix(x.repo)) == I);
    CREDO_VERUM(stml_numerus_liberorum(insula_ramus_nodus(&x.b)) == I);
    /* exstans intactum */
    CREDO_VERUM(scribere(&x.c, "quilibet", "gamma=1", c->piscina));
    CREDO_VERUM(insula_ramum_initiare(&x.c,
        "<c id=\"c1\" gamma=\"2\"/>"));
    CREDO_VERUM(valor_est(&x.c, "gamma", "1"));
}

/* scriptura in ramo profundo: nodus suus solus */
interior vacuum
_probare_scripturam (
    CredoContextus* c)
{
    InternamentumChorda* in;
                  Arbor  x;

    in = internamentum_creare(c->piscina);
    CREDO_NECESSE(arborem_struere(&x, c->piscina, in));
    CREDO_VERUM(scribere(&x.c, "quilibet", "gamma=3", c->piscina));
    CREDO_VERUM(valor_est(&x.c, "gamma", "3"));
    CREDO_VERUM(valor_est(&x.b, "gamma", NIHIL));
    CREDO_VERUM(valor_est(&x.a, "gamma", NIHIL));
    CREDO_VERUM(scribere(&x.b, "quilibet", "beta=2", c->piscina));
    CREDO_VERUM(valor_est(&x.b, "beta", "2"));
    CREDO_VERUM(valor_est(&x.c, "beta", NIHIL));
}

/* domini per viam: idem attributum in b liberum, in c possessum */
interior vacuum
_probare_dominos (
    CredoContextus* c)
{
    InternamentumChorda* in;
                  Arbor  x;

    in = internamentum_creare(c->piscina);
    CREDO_NECESSE(arborem_struere(&x, c->piscina, in));
    CREDO_NECESSE(insula_ramus_dominum_ponere(&x.c, "gamma",
        "actio.c"));
    CREDO_FALSUM(scribere(&x.c, "quilibet", "gamma=4", c->piscina));
    CREDO_CHORDA_CONTINET(insula_causa(x.repo), chorda_ex_literis(
        "gamma", c->piscina));
    CREDO_VERUM(scribere(&x.c, "actio.c", "gamma=4", c->piscina));
    /* b: gamma non possessum */
    CREDO_VERUM(scribere(&x.b, "quilibet", "gamma=5", c->piscina));
}

#define CANON_RADICIS \
    "<canon dialectus=\"radix\" versio=\"1\">" \
    "<elementum nomen=\"radix\" radix=\"verum\">" \
    "<liberum nomen=\"a\"/></elementum>" \
    "<elementum nomen=\"a\">" \
    "<attributum nomen=\"id\" genus=\"nomen\"/>" \
    "<attributum nomen=\"alpha\" genus=\"numerus\"/>" \
    "</elementum></canon>"

#define CANON_B \
    "<canon dialectus=\"b\" versio=\"1\">" \
    "<elementum nomen=\"b\" radix=\"verum\">" \
    "<attributum nomen=\"id\" genus=\"nomen\"/>" \
    "<attributum nomen=\"beta\" genus=\"numerus\"/>" \
    "</elementum></canon>"

#define CANON_C \
    "<canon dialectus=\"c\" versio=\"1\">" \
    "<elementum nomen=\"c\" radix=\"verum\">" \
    "<attributum nomen=\"id\" genus=\"nomen\"/>" \
    "<attributum nomen=\"gamma\" genus=\"numerus\"/>" \
    "</elementum></canon>"

interior Canon*
canonem (
         constans character* fons,
                    Piscina* p,
        InternamentumChorda* in)
{
    chorda causa;

    redde canon_legere(chorda_ex_literis(fons, p), p, in, &causa);
}

/* canones: radix a solum novit (b delegatus, in profunditate II); b c
 * non novit (c delegatus); quisque nodum suum iudicat */
interior vacuum
_probare_canones (
    CredoContextus* c)
{
    InternamentumChorda* in;
                  Arbor  x;
                  Canon* cr;
                  Canon* cb;
                  Canon* cc;

    in = internamentum_creare(c->piscina);
    CREDO_NECESSE(arborem_struere(&x, c->piscina, in));
    cr = canonem(CANON_RADICIS, c->piscina, in);
    cb = canonem(CANON_B, c->piscina, in);
    cc = canonem(CANON_C, c->piscina, in);
    CREDO_NECESSE(cr && cb && cc);
    insula_ponere_canonem(x.repo, cr);
    insula_ramus_canonem_ponere(&x.b, cb);
    insula_ramus_canonem_ponere(&x.c, cc);
    /* omnes gradus licita scribunt */
    CREDO_VERUM(scribere(&x.a, "quilibet", "alpha=1", c->piscina));
    CREDO_VERUM(scribere(&x.b, "quilibet", "beta=2", c->piscina));
    CREDO_VERUM(scribere(&x.c, "quilibet", "gamma=3", c->piscina));
    /* quisque canon suum nodum iudicat */
    CREDO_FALSUM(scribere(&x.c, "quilibet", "beta=9", c->piscina));
    CREDO_FALSUM(scribere(&x.b, "quilibet", "gamma=9", c->piscina));
    CREDO_FALSUM(scribere(&x.a, "quilibet", "beta=9", c->piscina));
    CREDO_FALSUM(scribere(&x.c, "quilibet", "gamma=magnus",
        c->piscina));
}

/* S8c: via -> ramus (inversum liberi); via mala recusatur */
interior vacuum
_probare_ex_via (
    CredoContextus* c)
{
     InternamentumChorda* in;
                   Arbor  x;
             InsulaRamus  r;

    in = internamentum_creare(c->piscina);
    CREDO_NECESSE(arborem_struere(&x, c->piscina, in));
    r = insula_ramus_ex_via(x.repo, chorda_ex_literis("a:a1/b:b1/c:c1",
        c->piscina));
    CREDO_VERUM(r.repo == x.repo);
    CREDO_VERUM(r.via.datum == x.c.via.datum);
    CREDO_CHORDA_AEQUALIS_LITERIS(r.elementum, "c");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.id, "c1");
    CREDO_VERUM(insula_ramus_nodus(&r) == insula_ramus_nodus(&x.c));
    /* vacua = radix */
    r = insula_ramus_ex_via(x.repo, chorda_ex_literis("", c->piscina));
    CREDO_VERUM(r.repo == x.repo);
    CREDO_CHORDA_VACUA(r.via);
    /* absens validus (nodus non quaeritur) */
    r = insula_ramus_ex_via(x.repo, chorda_ex_literis("a:a1/z:z9",
        c->piscina));
    CREDO_VERUM(r.repo == x.repo);
    CREDO_NIHIL(insula_ramus_nodus(&r));
    /* malae */
    CREDO_NIHIL(insula_ramus_ex_via(x.repo, chorda_ex_literis("a:a1/b",
        c->piscina)).repo);
    CREDO_NIHIL(insula_ramus_ex_via(x.repo, chorda_ex_literis(
        "a:a1//b:b1", c->piscina)).repo);
    CREDO_NIHIL(insula_ramus_ex_via(x.repo, chorda_ex_literis("a:a1/",
        c->piscina)).repo);
    CREDO_NIHIL(insula_ramus_ex_via(x.repo, chorda_ex_literis(":a1",
        c->piscina)).repo);
    CREDO_NIHIL(insula_ramus_ex_via(x.repo, chorda_ex_literis("a:",
        c->piscina)).repo);
}

interior b32
continet (
    constans character* via,
    constans character* descendens,
               Piscina* p)
{
    redde insula_via_continet(chorda_ex_literis(via, p),
        chorda_ex_literis(descendens, p));
}

/* S8c: continentia viarum per segmenta, non per praefixum */
interior vacuum
_probare_continentiam (
    CredoContextus* c)
{
    CREDO_VERUM(continet("a:1", "a:1", c->piscina));
    CREDO_VERUM(continet("a:1", "a:1/b:2", c->piscina));
    CREDO_VERUM(continet("a:1", "a:1/b:2/c:3", c->piscina));
    CREDO_FALSUM(continet("a:1", "a:10", c->piscina));
    CREDO_FALSUM(continet("a:1", "a:10/b:2", c->piscina));
    CREDO_FALSUM(continet("a:1/b:2", "a:1", c->piscina));
    CREDO_FALSUM(continet("a:1", "b:1", c->piscina));
    CREDO_VERUM(continet("", "a:1", c->piscina));
    CREDO_VERUM(continet("", "", c->piscina));
    CREDO_FALSUM(continet("a:1", "", c->piscina));
}

/* mutator qui per 'parens' extra ramum suum scribit */
interior vacuum
parentem_scribere (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    (vacuum)ctx;
    insula_attributum_ponere(radix->parens, p, in, "fuga", "1");
}

/* S8e: custodia - mutatio extra subarborem recusatur, nihil mutatum */
interior vacuum
_probare_custodiam (
    CredoContextus* c)
{
    InternamentumChorda* in;
                  Arbor  x;
                    i32  versio;
              character  par[XVI];

    in = internamentum_creare(c->piscina);
    CREDO_NECESSE(arborem_struere(&x, c->piscina, in));
    versio = insula_versio(x.repo);
    CREDO_FALSUM(mutare_ramum(&x.c, parentem_scribere, NIHIL));
    CREDO_CHORDA_CONTINET(insula_causa(x.repo), chorda_ex_literis(
        "extra ramum", c->piscina));
    CREDO_VERUM(valor_est(&x.b, "fuga", NIHIL));
    CREDO_AEQUALIS_I32(insula_versio(x.repo), versio);
    /* intra subarborem licet */
    CREDO_VERUM(scribere(&x.c, "quilibet", "gamma=1", c->piscina));
    /* radix: tota arbor sua */
    strcpy(par, "omega=1");
    CREDO_VERUM(insula_mutare(x.repo, ponere, par));
}

nomen structura {
       i32 numerus;
    chorda via;
} Acta;

interior vacuum
actum_notare (
     StmlNodus* radix,
        chorda  via,
        vacuum* ctx)
{
    Acta* a;

    (vacuum)radix;
    a = (Acta*)ctx;
    a->numerus++;
    a->via = via;
}

/* S8e: actarius viam scriptam accipit; radix = vacua */
interior vacuum
_probare_actarium (
    CredoContextus* c)
{
    InternamentumChorda* in;
                  Arbor  x;
                   Acta  a;
              character  par[XVI];

    in = internamentum_creare(c->piscina);
    CREDO_NECESSE(arborem_struere(&x, c->piscina, in));
    memset(&a, ZEPHYRUM, magnitudo(a));
    insula_ponere_actarium(x.repo, actum_notare, &a);
    CREDO_VERUM(scribere(&x.c, "quilibet", "gamma=1", c->piscina));
    CREDO_AEQUALIS_I32(a.numerus, I);
    CREDO_VERUM(a.via.datum == x.c.via.datum);
    strcpy(par, "omega=1");
    CREDO_VERUM(insula_mutare(x.repo, ponere, par));
    CREDO_AEQUALIS_I32(a.numerus, II);
    CREDO_CHORDA_VACUA(a.via);
}

#define CANON_D \
    "<canon dialectus=\"d\" versio=\"1\">" \
    "<elementum nomen=\"d\" radix=\"verum\">" \
    "<attributum nomen=\"id\" genus=\"nomen\"/>" \
    "</elementum></canon>"

/* canon ramorum a gradu primo: a, b, c sine canonibus propriis */
#define CANON_A \
    "<canon dialectus=\"a\" versio=\"1\">" \
    "<elementum nomen=\"a\" radix=\"verum\">" \
    "<attributum nomen=\"id\" genus=\"nomen\"/>" \
    "<liberum nomen=\"b\"/><liberum nomen=\"d\"/></elementum>" \
    "<elementum nomen=\"b\">" \
    "<attributum nomen=\"id\" genus=\"nomen\"/>" \
    "<liberum nomen=\"c\"/></elementum>" \
    "<elementum nomen=\"c\">" \
    "<attributum nomen=\"id\" genus=\"nomen\"/>" \
    "<attributum nomen=\"gamma\" genus=\"numerus\"/>" \
    "</elementum></canon>"

/* S8e: iudicant soli qui subarborem videre possunt */
interior vacuum
_probare_scopum_canonum (
    CredoContextus* c)
{
    InternamentumChorda* in;
                  Arbor  x;
            InsulaRamus  d;
                  Canon* cr;
                  Canon* cb;
                  Canon* ca;
              character  par[XVI];

    in = internamentum_creare(c->piscina);
    /* frater d invalidus (zeta ante canonem suum): scriptura in c
     * eum non tangit - canon d non iudicat */
    CREDO_NECESSE(arborem_struere(&x, c->piscina, in));
    d = insula_ramus_liberi(&x.a, "d", "d1");
    CREDO_NECESSE(insula_ramum_initiare(&d, "<d id=\"d1\"/>"));
    CREDO_NECESSE(scribere(&d, "quilibet", "zeta=1", c->piscina));
    insula_ramus_canonem_ponere(&d, canonem(CANON_D, c->piscina, in));
    CREDO_VERUM(scribere(&x.c, "quilibet", "gamma=1", c->piscina));
    CREDO_FALSUM(scribere(&d, "quilibet", "zeta=2", c->piscina));

    /* radix invalida (zeta): ramus cum canone proprio (b) eam non
     * tangit; ramus sine canone proprio (a) a radice iudicatur */
    CREDO_NECESSE(arborem_struere(&x, c->piscina, in));
    strcpy(par, "zeta=1");
    CREDO_NECESSE(insula_mutare(x.repo, ponere, par));
    cr = canonem(CANON_RADICIS, c->piscina, in);
    cb = canonem(CANON_B, c->piscina, in);
    CREDO_NECESSE(cr && cb);
    insula_ponere_canonem(x.repo, cr);
    insula_ramus_canonem_ponere(&x.b, cb);
    insula_ramus_canonem_ponere(&x.c, canonem(CANON_C, c->piscina, in));
    CREDO_VERUM(scribere(&x.b, "quilibet", "beta=2", c->piscina));
    CREDO_FALSUM(scribere(&x.a, "quilibet", "alpha=1", c->piscina));

    /* canon in gradu primo solo: scriptura in c ab eo iudicatur */
    CREDO_NECESSE(arborem_struere(&x, c->piscina, in));
    ca = canonem(CANON_A, c->piscina, in);
    CREDO_NECESSE(ca);
    insula_ramus_canonem_ponere(&x.a, ca);
    CREDO_VERUM(scribere(&x.c, "quilibet", "gamma=3", c->piscina));
    CREDO_FALSUM(scribere(&x.c, "quilibet", "delta=1", c->piscina));
    CREDO_FALSUM(scribere(&x.c, "quilibet", "gamma=magnus",
        c->piscina));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "via",          _probare_viam,          NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "initiatio",    _probare_initiationem,  NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "scriptura",    _probare_scripturam,    NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "domini",       _probare_dominos,       NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "canones",      _probare_canones,       NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "ex_via",       _probare_ex_via,        NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "continentia",  _probare_continentiam,  NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "custodia",     _probare_custodiam,     NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "actarius",     _probare_actarium,      NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "scopus_canonum", _probare_scopum_canonum, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("insula_profunda", SECTIONES);
}
