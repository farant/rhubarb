/* probatio_insula_ramus.c - rami insulae (insula-rami-plan R1)
 *
 * Repositorium unum (hospes), duo rami eiusdem generis (libellus a,
 * b): lectio et scriptura relativae ad nodum rami; domini et canones
 * per ramum; canon radicis ramos delegatos non videt; ramus radicis
 * == API vetus. */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "chorda.h"
#include "stml.h"
#include "canon.h"
#include "insula.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

hic_manens Piscina*             piscina;
hic_manens InternamentumChorda* intern;

hic_manens constans character canon_libelli[] =
    "<canon dialectus=\"libellus\" versio=\"1\">"
    "<elementum nomen=\"libellus\" radix=\"verum\">"
    "<attributum nomen=\"id\" genus=\"nomen\"/>"
    "<attributum nomen=\"n\" genus=\"numerus\"/>"
    "</elementum></canon>";

hic_manens constans character canon_hospitis[] =
    "<canon dialectus=\"hospes\" versio=\"1\">"
    "<elementum nomen=\"hospes\" radix=\"verum\">"
    "<attributum nomen=\"activa\" genus=\"nomen\"/>"
    "<attributum nomen=\"nova\" genus=\"numerus\"/>"
    "</elementum></canon>";

interior Canon*
canon_ex (
    constans character* fons)
{
    chorda causa;

    redde canon_legere(chorda_ex_literis(fons, piscina), piscina,
        intern,
                       &causa);
}

/* mutator: n ponere (ctx = valor) in nodo dato - nodum rami accipit */
interior vacuum
n_ponere (
              StmlNodus* nodus,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    insula_attributum_ponere(nodus, p, in, "n",
                             (constans character*)ctx);
}

interior vacuum
activam_ponere (
              StmlNodus* nodus,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    insula_attributum_ponere(nodus, p, in, "activa",
                             (constans character*)ctx);
}

/* mutator: liberum ignotum radici addere */
interior vacuum
ignotum_addere (
              StmlNodus* nodus,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    StmlNodus* x;

    (vacuum)ctx;
    x = stml_elementum_creare(p, in, "ignotum");
    stml_liberum_addere(nodus, x);
}

interior b32
valor_est (
                chorda* a,
    constans character* v)
{
    redde a ? chorda_aequalis_literis(*a, v) : FALSUM;
}

/* piscina et intern (canon_ex) sectionis sunt: posita in parare,
 * vacata in purgare */
interior vacuum
_parare (
    CredoContextus* c)
{
    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
}

interior vacuum
_purgare (
    CredoContextus* c)
{
    (vacuum)c;
    piscina  = NIHIL;
    intern   = NIHIL;
}

/* gradus I-VII seriatim in uno repositorio: quisque statum prioris
 * legit, ergo sectio una cum gradibus nominatis; valores mutatorum
 * (ctx) locales - status nullus inter sectiones */
interior vacuum
_sectio_prima (
    CredoContextus* ctx)
{
    InsulaRepositorium* repo;
           InsulaRamus  radix;
           InsulaRamus  a;
           InsulaRamus  b;
           InsulaRamus  c;
                   i32  versio;
             character  littera_a[]         = "a";
             character  littera_b[]         = "b";
             character  numerus_duo[]       = "2";
             character  numerus_tres[]      = "3";
             character  numerus_quattuor[]  = "4";
             character  numerus_novem[]     = "9";
             character  non_numerus[]       = "abc";

    (vacuum)ctx;
    repo = insula_repositorium_creare(piscina, intern,
        "<hospes activa=\"a\"><libellus id=\"a\""
        " n=\"1\"/><libellus id=\"b\" n=\"1\"/></hospes>");
    CREDO_NON_NIHIL(repo);
    radix  = insula_ramus_radix(repo);
    a      = insula_ramus(repo, "libellus", "a");
    b      = insula_ramus(repo, "libellus", "b");
    c      = insula_ramus(repo, "libellus", "c");

    CREDO_GRADUS("I: ramus radicis == API vetus");
    CREDO_VERUM(insula_ramus_nodus(&radix)
                == insula_radix(repo));
    CREDO_VERUM(valor_est(insula_ramus_attributum(&radix, "activa"),
        "a"));
    CREDO_VERUM(mutare_ramum(&radix, activam_ponere,
        littera_b));
    CREDO_VERUM(valor_est(insula_attributum(repo,
        "activa"), "b"));

    CREDO_GRADUS("II: ramus - lectio et scriptura relativae");
    CREDO_VERUM(valor_est(insula_ramus_attributum(&a,
        "n"), "1"));
    CREDO_VERUM(mutare_ramum(&a, n_ponere,
        numerus_duo));
    CREDO_VERUM(valor_est(insula_ramus_attributum(&a,
        "n"), "2"));
    CREDO_VERUM(valor_est(insula_ramus_attributum(&b,
        "n"), "1"));
    /* radix ipsa n non accepit (mutator nodum rami accepit) */
    CREDO_NIHIL(insula_attributum(repo, "n"));

    CREDO_GRADUS("III: ramus absens");
    CREDO_NIHIL(insula_ramus_nodus(&c));
    CREDO_NIHIL(insula_ramus_attributum(&c, "n"));
    versio = insula_versio(repo);
    CREDO_FALSUM(mutare_ramum(&c, n_ponere,
        numerus_novem));
    CREDO_AEQUALIS_I32(insula_versio(repo), versio);
    CREDO_VERUM(insula_causa(repo).mensura > ZEPHYRUM);

    CREDO_GRADUS("IV: domini per ramum");
    CREDO_VERUM(insula_ramus_dominum_ponere(&a, "n",
        "scriptor_a"));
    insula_scriptorem_ponere(repo, chorda_ex_literis("alius", piscina));
    CREDO_FALSUM(mutare_ramum(&a, n_ponere,
        numerus_tres));
    CREDO_VERUM(valor_est(insula_ramus_attributum(&a,
        "n"), "2"));
    /* b non possessus: alius scribere potest */
    CREDO_VERUM(mutare_ramum(&b, n_ponere,
        numerus_tres));
    insula_scriptorem_ponere(repo, chorda_ex_literis("scriptor_a",
        piscina));
    CREDO_VERUM(mutare_ramum(&a, n_ponere,
        numerus_tres));
    insula_scriptorem_ponere(repo, chorda_ex_literis("", piscina));

    CREDO_GRADUS("V: canones per ramum; radix delegatos ignorat");
    insula_ramus_canonem_ponere(&a,
        canon_ex(canon_libelli));
    insula_ramus_canonem_ponere(&b,
        canon_ex(canon_libelli));
    insula_ponere_canonem(repo,
        canon_ex(canon_hospitis));
    insula_scriptorem_ponere(repo, chorda_ex_literis("scriptor_a",
        piscina));
    /* canon rami a numerum exigit */
    CREDO_FALSUM(mutare_ramum(&a, n_ponere,
        non_numerus));
    CREDO_VERUM(valor_est(insula_ramus_attributum(&a,
        "n"), "3"));
    CREDO_VERUM(mutare_ramum(&a, n_ponere,
        numerus_quattuor));
    insula_scriptorem_ponere(repo, chorda_ex_literis("", piscina));
    /* canon hospitis libellos (delegatos) non videt */
    CREDO_VERUM(mutare_ramum(&radix, activam_ponere,
        littera_a));
    /* sed liberum ignotum radicis iudicat */
    CREDO_FALSUM(mutare_ramum(&radix, ignotum_addere,
        NIHIL));
    /* arbor vera libellos servat (visio canonis sola eos omittit) */
    CREDO_AEQUALIS_I32(stml_numerus_liberorum(insula_radix(repo)), II);

    CREDO_GRADUS("VI: domini e plagula in ramum");
    {
        StmlResultus res;

        res =
            stml_legere_ex_literis("<domini><dominus genus=\"ephemera\""
            " attributum=\"n\" scriptor=\"scriptor_b\"/></domini>",
            piscina, intern);
        /* C2: 'genus' lineae ignoratur - linea legitur */
        CREDO_AEQUALIS_I32(insula_ramus_dominos_legere(&b,
            res.elementum_radix), I);
    }

    CREDO_GRADUS("VII: ramum initiare (T1a)");
    {
        InsulaRamus d;
                i32 liberi;

        d = insula_ramus(repo, "libellus", "d");
        insula_ramus_canonem_ponere(&d,
            canon_ex(canon_libelli));
        CREDO_NIHIL(insula_ramus_nodus(&d));
        liberi = stml_numerus_liberorum(insula_radix(repo));
        /* liberum novum, canone suo iudicatum (hospes eum non videt) */
        CREDO_VERUM(insula_ramum_initiare(&d,
            "<libellus id=\"d\" n=\"7\"/>"));
        CREDO_VERUM(valor_est(insula_ramus_attributum(&d, "n"), "7"));
        /* iterum: nihil mutat (exstat) */
        CREDO_VERUM(insula_ramum_initiare(&d,
            "<libellus id=\"d\" n=\"8\"/>"));
        CREDO_VERUM(valor_est(insula_ramus_attributum(&d, "n"), "7"));
        CREDO_AEQUALIS_I32(stml_numerus_liberorum(insula_radix(repo)),
            liberi + I);
        /* canon rami elementum malum recusat */
        {
            InsulaRamus e;

            e = insula_ramus(repo, "libellus", "e");
            insula_ramus_canonem_ponere(&e,
                canon_ex(canon_libelli));
            CREDO_FALSUM(insula_ramum_initiare(&e,
                "<libellus id=\"e\" n=\"abc\"/>"));
            CREDO_NIHIL(insula_ramus_nodus(&e));
        }
        /* ramus radicis: attributa absentia SOLA radici adduntur */
        CREDO_VERUM(insula_ramum_initiare(&radix,
            "<hospes activa=\"zz\" nova=\"1\"/>"));
        CREDO_VERUM(valor_est(insula_ramus_attributum(&radix, "nova"),
            "1"));
        /* exstans intactum (activa ex V manet) */
        CREDO_VERUM(valor_est(insula_ramus_attributum(&radix, "activa"),
            "a"));
    }

    CREDO_GRADUS("VII: restitutio honesta");
    CREDO_VERUM(insula_restituere(repo));
    CREDO_FALSUM(insula_mendacium(repo));
    CREDO_VERUM(valor_est(insula_ramus_attributum(&a,
        "n"), "4"));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "rami insulae", _sectio_prima, _parare, _purgare, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("insula_ramus", SECTIONES);
}
