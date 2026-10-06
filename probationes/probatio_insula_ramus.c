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

hic_manens character littera_a[]        = "a";
hic_manens character littera_b[]        = "b";
hic_manens character numerus_duo[]      = "2";
hic_manens character numerus_tres[]     = "3";
hic_manens character numerus_quattuor[] = "4";
hic_manens character numerus_novem[]    = "9";
hic_manens character non_numerus[]      = "abc";

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

s32 principale (vacuum)
{
    InsulaRepositorium* repo;
           InsulaRamus  radix;
           InsulaRamus  a;
           InsulaRamus  b;
           InsulaRamus  c;
                   i32  versio;

    piscina = piscina_generare_dynamicum("probatio_insula_ramus",
        XVI * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    repo = insula_repositorium_creare(piscina, intern,
        "<hospes activa=\"a\"><libellus id=\"a\" n=\"1\"/>"
        "<libellus id=\"b\" n=\"1\"/></hospes>",
        "<hospes/>");
    CREDO_NON_NIHIL(repo);
    radix  = insula_ramus_radix(repo);
    a      = insula_ramus(repo, "libellus", "a");
    b      = insula_ramus(repo, "libellus", "b");
    c      = insula_ramus(repo, "libellus", "c");

    imprimere("\n--- I: ramus radicis == API vetus ---\n");
    CREDO_VERUM(insula_ramus_nodus(&radix, INSULA_DURABILIS)
                == insula_radix(repo, INSULA_DURABILIS));
    CREDO_VERUM(valor_est(insula_ramus_attributum(&radix,
        INSULA_DURABILIS, "activa"), "a"));
    CREDO_VERUM(mutare_ramum(&radix, INSULA_DURABILIS, activam_ponere,
        littera_b));
    CREDO_VERUM(valor_est(insula_attributum(repo, INSULA_DURABILIS,
        "activa"), "b"));

    imprimere("\n--- II: ramus - lectio et scriptura relativae ---\n");
    CREDO_VERUM(valor_est(insula_ramus_attributum(&a, INSULA_DURABILIS,
        "n"), "1"));
    CREDO_VERUM(mutare_ramum(&a, INSULA_DURABILIS, n_ponere,
        numerus_duo));
    CREDO_VERUM(valor_est(insula_ramus_attributum(&a, INSULA_DURABILIS,
        "n"), "2"));
    CREDO_VERUM(valor_est(insula_ramus_attributum(&b, INSULA_DURABILIS,
        "n"), "1"));
    /* radix ipsa n non accepit (mutator nodum rami accepit) */
    CREDO_NIHIL(insula_attributum(repo, INSULA_DURABILIS, "n"));

    imprimere("\n--- III: ramus absens ---\n");
    CREDO_NIHIL(insula_ramus_nodus(&c, INSULA_DURABILIS));
    CREDO_NIHIL(insula_ramus_attributum(&c, INSULA_DURABILIS, "n"));
    versio = insula_versio(repo, INSULA_DURABILIS);
    CREDO_FALSUM(mutare_ramum(&c, INSULA_DURABILIS, n_ponere,
        numerus_novem));
    CREDO_AEQUALIS_I32(insula_versio(repo, INSULA_DURABILIS), versio);
    CREDO_VERUM(insula_causa(repo).mensura > ZEPHYRUM);

    imprimere("\n--- IV: domini per ramum ---\n");
    CREDO_VERUM(insula_ramus_dominum_ponere(&a, INSULA_DURABILIS, "n",
        "scriptor_a"));
    insula_scriptorem_ponere(repo, chorda_ex_literis("alius", piscina));
    CREDO_FALSUM(mutare_ramum(&a, INSULA_DURABILIS, n_ponere,
        numerus_tres));
    CREDO_VERUM(valor_est(insula_ramus_attributum(&a, INSULA_DURABILIS,
        "n"), "2"));
    /* b non possessus: alius scribere potest */
    CREDO_VERUM(mutare_ramum(&b, INSULA_DURABILIS, n_ponere,
        numerus_tres));
    insula_scriptorem_ponere(repo, chorda_ex_literis("scriptor_a",
        piscina));
    CREDO_VERUM(mutare_ramum(&a, INSULA_DURABILIS, n_ponere,
        numerus_tres));
    insula_scriptorem_ponere(repo, chorda_ex_literis("", piscina));

    imprimere("\n--- V: canones per ramum; radix delegatos ignorat\n");
    insula_ramus_canonem_ponere(&a, INSULA_DURABILIS,
        canon_ex(canon_libelli));
    insula_ramus_canonem_ponere(&b, INSULA_DURABILIS,
        canon_ex(canon_libelli));
    insula_ponere_canonem(repo, INSULA_DURABILIS,
        canon_ex(canon_hospitis));
    insula_scriptorem_ponere(repo, chorda_ex_literis("scriptor_a",
        piscina));
    /* canon rami a numerum exigit */
    CREDO_FALSUM(mutare_ramum(&a, INSULA_DURABILIS, n_ponere,
        non_numerus));
    CREDO_VERUM(valor_est(insula_ramus_attributum(&a, INSULA_DURABILIS,
        "n"), "3"));
    CREDO_VERUM(mutare_ramum(&a, INSULA_DURABILIS, n_ponere,
        numerus_quattuor));
    insula_scriptorem_ponere(repo, chorda_ex_literis("", piscina));
    /* canon hospitis libellos (delegatos) non videt */
    CREDO_VERUM(mutare_ramum(&radix, INSULA_DURABILIS, activam_ponere,
        littera_a));
    /* sed liberum ignotum radicis iudicat */
    CREDO_FALSUM(mutare_ramum(&radix, INSULA_DURABILIS, ignotum_addere,
        NIHIL));
    /* arbor vera libellos servat (visio canonis sola eos omittit) */
    CREDO_AEQUALIS_I32(stml_numerus_liberorum(insula_radix(repo,
        INSULA_DURABILIS)), II);

    imprimere("\n--- VI: domini e plagula in ramum ---\n");
    {
        StmlResultus res;

        res =
            stml_legere_ex_literis("<domini><dominus genus=\"ephemera\""
            " attributum=\"n\" scriptor=\"scriptor_b\"/></domini>",
            piscina, intern);
        CREDO_AEQUALIS_I32(insula_ramus_dominos_legere(&b,
            INSULA_DURABILIS, res.elementum_radix), ZEPHYRUM);
        CREDO_AEQUALIS_I32(insula_ramus_dominos_legere(&b,
            INSULA_EPHEMERA, res.elementum_radix), I);
    }

    imprimere("\n--- VII: ramum initiare (T1a) ---\n");
    {
        InsulaRamus d;
                i32 liberi;

        d = insula_ramus(repo, "libellus", "d");
        insula_ramus_canonem_ponere(&d, INSULA_DURABILIS,
            canon_ex(canon_libelli));
        CREDO_NIHIL(insula_ramus_nodus(&d, INSULA_DURABILIS));
        liberi = stml_numerus_liberorum(insula_radix(repo,
            INSULA_DURABILIS));
        /* liberum novum, canone suo iudicatum (hospes eum non videt) */
        CREDO_VERUM(insula_ramum_initiare(&d, INSULA_DURABILIS,
            "<libellus id=\"d\" n=\"7\"/>"));
        CREDO_VERUM(valor_est(insula_ramus_attributum(&d,
            INSULA_DURABILIS, "n"), "7"));
        /* iterum: nihil mutat (exstat) */
        CREDO_VERUM(insula_ramum_initiare(&d, INSULA_DURABILIS,
            "<libellus id=\"d\" n=\"8\"/>"));
        CREDO_VERUM(valor_est(insula_ramus_attributum(&d,
            INSULA_DURABILIS, "n"), "7"));
        CREDO_AEQUALIS_I32(stml_numerus_liberorum(insula_radix(repo,
            INSULA_DURABILIS)), liberi + I);
        /* canon rami elementum malum recusat */
        {
            InsulaRamus e;

            e = insula_ramus(repo, "libellus", "e");
            insula_ramus_canonem_ponere(&e, INSULA_DURABILIS,
                canon_ex(canon_libelli));
            CREDO_FALSUM(insula_ramum_initiare(&e, INSULA_DURABILIS,
                "<libellus id=\"e\" n=\"abc\"/>"));
            CREDO_NIHIL(insula_ramus_nodus(&e, INSULA_DURABILIS));
        }
        /* ramus radicis: attributa absentia SOLA radici adduntur */
        CREDO_VERUM(insula_ramum_initiare(&radix, INSULA_EPHEMERA,
            "<hospes activa=\"zz\" nova=\"1\"/>"));
        CREDO_VERUM(valor_est(insula_ramus_attributum(&radix,
            INSULA_EPHEMERA, "nova"), "1"));
    }

    imprimere("\n--- VII: restitutio honesta ---\n");
    CREDO_VERUM(insula_restituere(repo));
    CREDO_FALSUM(insula_mendacium(repo));
    CREDO_VERUM(valor_est(insula_ramus_attributum(&a, INSULA_DURABILIS,
        "n"), "4"));

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
