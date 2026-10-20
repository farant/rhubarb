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
     StmlNodus* radix,
        chorda  via,
        vacuum* ctx)
{
    (vacuum)radix;
    (vacuum)via;
    (*(i32*)ctx)++;
}

/* SECTIO UNA (credo II): scaena gradibus nominatis (CREDO_GRADUS) -
 * repositorium unum per gradus mutatur (versio, acta, canon, domini),
 * ergo gradus sectiones liberae esse non possunt */
interior vacuum
_sectio_prima (
    CredoContextus* c)
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

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);

    CREDO_GRADUS("creationem et lectionem");
    /* C2: insula una - olim latitudo/altitudo in arbore durabili */
    repo = insula_repositorium_creare(piscina, intern,
        "<insula latitudo=\"320\" altitudo=\"200\""
        " instrumentum=\"penicillus\" focus=\"tabula\"/>");
    CREDO_NON_NIHIL(repo);
    a = insula_attributum(repo, "latitudo");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "320");
    a = insula_attributum(repo, "instrumentum");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "penicillus");
    CREDO_NIHIL(insula_attributum(repo, "nemo"));
    CREDO_AEQUALIS_I32(insula_versio(repo), ZEPHYRUM);
    CREDO_FALSUM(insula_mendacium(repo));

    CREDO_GRADUS("portam");
    acta = ZEPHYRUM;
    insula_ponere_actarium(repo, numerare, &acta);
    CREDO_VERUM(insula_mutare(repo, ponere_instrumentum, impletio));
    a = insula_attributum(repo, "instrumentum");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "impletio");
    CREDO_AEQUALIS_I32(insula_versio(repo), I);
    CREDO_AEQUALIS_I32(acta, I);
    /* ponere = substituere, non geminare: IV attributa manent */
    CREDO_AEQUALIS_I32(
        xar_numerus(insula_radix(repo)->attributa),
        IV);

    CREDO_GRADUS("restitutionem HONESTAM");
    CREDO_VERUM(insula_restituere(repo));
    CREDO_FALSUM(insula_mendacium(repo));
    a = insula_attributum(repo, "instrumentum");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "impletio");
    a = insula_attributum(repo, "focus");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "tabula");
    CREDO_AEQUALIS_I32(insula_versio(repo), I);

    CREDO_GRADUS("scriptorem MENTIENTEM (lex rehydrationis)");
    radix = insula_radix(repo);
    /* extra portam: pointer lectionis solae abusus */
    insula_attributum_ponere(radix, piscina, intern, "instrumentum",
                             furtivus);
    a = insula_attributum(repo, "instrumentum");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "furtivus");
    CREDO_FALSUM(insula_restituere(repo));
    CREDO_VERUM(insula_mendacium(repo));
    CREDO_CHORDA_NON_VACUA(insula_causa(repo));
    a = insula_attributum(repo, "instrumentum");
    CREDO_NON_NIHIL(a);
    /* ex textu honesto refecta */
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "impletio");
    /* mendacium non est scriptura: versio immota */
    CREDO_AEQUALIS_I32(insula_versio(repo), I);
    CREDO_AEQUALIS_I32(acta, I);

    CREDO_GRADUS("canonem: scriptura recusata");
    canon = canon_legere(chorda_ex_literis(
        "<canon dialectus=\"insula\" versio=\"1\">"
        "<elementum nomen=\"insula\" radix=\"verum\">"
        "<attributum nomen=\"latitudo\" genus=\"numerus\"/>"
        "<attributum nomen=\"altitudo\" genus=\"numerus\"/>"
        "<attributum nomen=\"instrumentum\" genus=\"textus\"/>"
        "<attributum nomen=\"focus\" genus=\"textus\"/>"
        "</elementum>"
        "</canon>", piscina), piscina, intern, &causa);
    CREDO_NON_NIHIL(canon);
    insula_ponere_canonem(repo, canon);
    CREDO_VERUM(insula_mutare(repo, ponere_instrumentum, linea));
    /* liberum non declaratum: canon recusat, arbor immota */
    CREDO_FALSUM(insula_mutare(repo, addere_ignotum, NIHIL));
    CREDO_AEQUALIS_I32(insula_versio(repo), II);
    CREDO_AEQUALIS_I32(acta, II);
    CREDO_CHORDA_NON_VACUA(insula_causa(repo));
    CREDO_CHORDA_CONTINET(insula_causa(repo),
                          chorda_ex_literis("canon", piscina));
    a = insula_attributum(repo, "instrumentum");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "linea");
    CREDO_AEQUALIS_I32(
        stml_numerus_liberorum(insula_radix(repo)),
        ZEPHYRUM);
    /* post recusationem arbor manet honesta */
    CREDO_VERUM(insula_restituere(repo));
    CREDO_FALSUM(insula_mendacium(repo));

    CREDO_GRADUS("dominos: alienus recusatur");
    CREDO_VERUM(insula_dominum_ponere(repo, "focus",
                                      "dispensator"));
    CREDO_FALSUM(insula_dominum_ponere(repo, "", "x"));
    /* anonymus (scriptor vacuus) attributum possessum tangit:
     * recusatur */
    insula_scriptorem_ponere(repo, chorda_ex_literis("", piscina));
    CREDO_FALSUM(insula_mutare(repo, ponere_focum, focus_b1));
    CREDO_CHORDA_CONTINET(insula_causa(repo),
                          chorda_ex_literis("dominus", piscina));
    /* dominus ipse: licet */
    insula_scriptorem_ponere(repo, chorda_ex_literis("dispensator",
        piscina));
    CREDO_VERUM(insula_mutare(repo, ponere_focum, focus_b1));
    a = insula_attributum(repo, "focus");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "b1");
    /* scriptor alienus attributum NON possessum tangit: licet */
    insula_scriptorem_ponere(repo, chorda_ex_literis("quisquis",
        piscina));
    CREDO_VERUM(insula_mutare(repo, ponere_instrumentum, linea));
    /* scriptor alienus attributum possessum TOLLIT: recusatur */
    CREDO_FALSUM(insula_mutare(repo, tollere_focum, NIHIL));
    /* versio immota a recusationibus: III scripturae acceptae */
    CREDO_AEQUALIS_I32(insula_versio(repo), IV);
    insula_scriptorem_ponere(repo, chorda_ex_literis("", piscina));

    CREDO_GRADUS("scribere");
    /* C2: arbor una - latitudo et instrumentum simul */
    CREDO_CHORDA_CONTINET(
        insula_scribere(repo, piscina),
        chorda_ex_literis("latitudo", piscina));
    CREDO_CHORDA_CONTINET(
        insula_scribere(repo, piscina),
        chorda_ex_literis("linea", piscina));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "tria genera, portae unicae, restitutio, mendacium",
        _sectio_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("insula", SECTIONES);
}
