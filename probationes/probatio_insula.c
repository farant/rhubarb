/* probatio_insula.c - tria genera, portae unicae, restitutio,
 * mendacium */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "stml.h"
#include "canon.h"
#include "insula.h"
#include "credo.h"
#include <stdio.h>

/* Mutator: instrumentum ponere (ctx = valor, cstr) */
interior vacuum
ponere_instrumentum (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    insula_attributum_ponere(radix, p, in, "instrumentum",
                             (constans character*)ctx);
}

/* Mutator: liberum non declaratum addere (canon recusabit) */
interior vacuum
addere_ignotum (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    (vacuum)ctx;
    stml_liberum_addere(radix, stml_elementum_creare(p, in, "ignotum"));
}

/* Mutator: focum ponere (ctx = valor, cstr) */
interior vacuum
ponere_focum (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    insula_attributum_ponere(radix, p, in, "focus",
                             (constans character*)ctx);
}

/* Mutator: focum tollere */
interior vacuum
tollere_focum (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    (vacuum)p;
    (vacuum)in;
    (vacuum)ctx;
    insula_attributum_tollere(radix, "focus");
}

/* Actarius: scripturas numerare */
interior vacuum
numerare (
    InsulaGenus  genus,
      StmlNodus* radix,
         vacuum* ctx)
{
    (vacuum)genus;
    (vacuum)radix;
    (*(i32*)ctx)++;
}

s32 principale (vacuum)
{
                 Piscina* piscina;
     InternamentumChorda* intern;
      InsulaRepositorium* repo;
                  chorda* a;
                     i32  acta;
                  chorda  causa;
                   Canon* canon;
               StmlNodus* radix;
               character  impletio[]  = "impletio";
               character  linea[]     = "linea";
               character  furtivus[]  = "furtivus";
               character  focus_b1[]  = "b1";

    piscina = piscina_generare_dynamicum("probatio_insula",
        LXIV * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);

    imprimere("\n--- Probans creationem et lectionem ---\n");
    repo = insula_repositorium_creare(piscina, intern,
        "<documentum latitudo=\"320\" altitudo=\"200\"/>",
        "<ephemera instrumentum=\"penicillus\" focus=\"tabula\"/>");
    CREDO_NON_NIHIL(repo);
    a = insula_attributum(repo, INSULA_DURABILIS, "latitudo");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "320");
    a = insula_attributum(repo, INSULA_EPHEMERA, "instrumentum");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "penicillus");
    CREDO_NIHIL(insula_attributum(repo, INSULA_EPHEMERA, "nemo"));
    CREDO_AEQUALIS_I32(insula_versio(repo, INSULA_EPHEMERA), ZEPHYRUM);
    CREDO_FALSUM(insula_mendacium(repo));

    imprimere("\n--- Probans portam ephemerae ---\n");
    acta = ZEPHYRUM;
    insula_ponere_actarium(repo, numerare, &acta);
    CREDO_VERUM(mutare_ephemera(repo, ponere_instrumentum, impletio));
    a = insula_attributum(repo, INSULA_EPHEMERA, "instrumentum");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "impletio");
    CREDO_AEQUALIS_I32(insula_versio(repo, INSULA_EPHEMERA), I);
    CREDO_AEQUALIS_I32(acta, I);
    CREDO_AEQUALIS_I32(insula_versio(repo, INSULA_DURABILIS), ZEPHYRUM);
    /* ponere = substituere, non geminare: unum attributum manet */
    CREDO_AEQUALIS_I32(
        xar_numerus(insula_radix(repo, INSULA_EPHEMERA)->attributa),
        II);

    imprimere("\n--- Probans restitutionem HONESTAM ---\n");
    CREDO_VERUM(insula_restituere(repo));
    CREDO_FALSUM(insula_mendacium(repo));
    a = insula_attributum(repo, INSULA_EPHEMERA, "instrumentum");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "impletio");
    a = insula_attributum(repo, INSULA_EPHEMERA, "focus");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "tabula");
    CREDO_AEQUALIS_I32(insula_versio(repo, INSULA_EPHEMERA), I);

    imprimere("\n--- Probans scriptorem MENTIENTEM (lex rehydrationis)"
              " ---\n");
    radix = insula_radix(repo, INSULA_EPHEMERA);
    /* extra portam: pointer lectionis solae abusus */
    insula_attributum_ponere(radix, piscina, intern, "instrumentum",
                             furtivus);
    a = insula_attributum(repo, INSULA_EPHEMERA, "instrumentum");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "furtivus");
    CREDO_FALSUM(insula_restituere(repo));
    CREDO_VERUM(insula_mendacium(repo));
    CREDO_CHORDA_NON_VACUA(insula_causa(repo));
    a = insula_attributum(repo, INSULA_EPHEMERA, "instrumentum");
    CREDO_NON_NIHIL(a);
    /* ex textu honesto refecta */
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "impletio");
    /* mendacium non est scriptura: versio immota */
    CREDO_AEQUALIS_I32(insula_versio(repo, INSULA_EPHEMERA), I);
    CREDO_AEQUALIS_I32(acta, I);

    imprimere("\n--- Probans canonem: scriptura recusata ---\n");
    canon = canon_legere(chorda_ex_literis(
        "<canon dialectus=\"ephemera\" versio=\"1\">"
        "<elementum nomen=\"ephemera\" radix=\"verum\">"
        "<attributum nomen=\"instrumentum\" genus=\"textus\"/>"
        "<attributum nomen=\"focus\" genus=\"textus\"/>"
        "</elementum>"
        "</canon>", piscina), piscina, intern, &causa);
    CREDO_NON_NIHIL(canon);
    insula_ponere_canonem(repo, INSULA_EPHEMERA, canon);
    CREDO_VERUM(mutare_ephemera(repo, ponere_instrumentum, linea));
    /* liberum non declaratum: canon recusat, arbor immota */
    CREDO_FALSUM(mutare_ephemera(repo, addere_ignotum, NIHIL));
    CREDO_AEQUALIS_I32(insula_versio(repo, INSULA_EPHEMERA), II);
    CREDO_AEQUALIS_I32(acta, II);
    CREDO_CHORDA_NON_VACUA(insula_causa(repo));
    CREDO_CHORDA_CONTINET(insula_causa(repo),
                          chorda_ex_literis("canon", piscina));
    a = insula_attributum(repo, INSULA_EPHEMERA, "instrumentum");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "linea");
    CREDO_AEQUALIS_I32(
        stml_numerus_liberorum(insula_radix(repo, INSULA_EPHEMERA)),
        ZEPHYRUM);
    /* post recusationem arbor manet honesta */
    CREDO_VERUM(insula_restituere(repo));
    CREDO_FALSUM(insula_mendacium(repo));

    imprimere("\n--- Probans portam durabilem ---\n");
    CREDO_VERUM(mutare_durabile(repo, ponere_instrumentum, linea));
    CREDO_AEQUALIS_I32(insula_versio(repo, INSULA_DURABILIS), I);
    CREDO_AEQUALIS_I32(insula_versio(repo, INSULA_EPHEMERA), II);
    CREDO_AEQUALIS_I32(acta, III);

        imprimere("\n--- Probans dominos: alienus recusatur ---\n");
    CREDO_VERUM(insula_dominum_ponere(repo, INSULA_EPHEMERA, "focus",
                                      "dispensator"));
    CREDO_FALSUM(insula_dominum_ponere(repo, INSULA_EPHEMERA, "", "x"));
    /* anonymus (scriptor vacuus) attributum possessum tangit:
     * recusatur */
    insula_scriptorem_ponere(repo, chorda_ex_literis("", piscina));
    CREDO_FALSUM(mutare_ephemera(repo, ponere_focum, focus_b1));
    CREDO_CHORDA_CONTINET(insula_causa(repo),
                          chorda_ex_literis("dominus", piscina));
    /* dominus ipse: licet */
    insula_scriptorem_ponere(repo, chorda_ex_literis("dispensator",
        piscina));
    CREDO_VERUM(mutare_ephemera(repo, ponere_focum, focus_b1));
    a = insula_attributum(repo, INSULA_EPHEMERA, "focus");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "b1");
    /* scriptor alienus attributum NON possessum tangit: licet */
    insula_scriptorem_ponere(repo, chorda_ex_literis("quisquis",
        piscina));
    CREDO_VERUM(mutare_ephemera(repo, ponere_instrumentum, linea));
    /* scriptor alienus attributum possessum TOLLIT: recusatur */
    CREDO_FALSUM(mutare_ephemera(repo, tollere_focum, NIHIL));
    /* versio immota a recusationibus: III scripturae acceptae */
    CREDO_AEQUALIS_I32(insula_versio(repo, INSULA_EPHEMERA), IV);
    insula_scriptorem_ponere(repo, chorda_ex_literis("", piscina));

    imprimere("\n--- Probans scribere ---\n");
    CREDO_CHORDA_CONTINET(
        insula_scribere(repo, INSULA_DURABILIS, piscina),
        chorda_ex_literis("latitudo", piscina));
    CREDO_CHORDA_CONTINET(
        insula_scribere(repo, INSULA_EPHEMERA, piscina),
        chorda_ex_literis("linea", piscina));

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
