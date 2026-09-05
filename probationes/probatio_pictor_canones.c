/* probatio_pictor_canones.c - canones veri insularum pictoris et
 * tabula dominorum: leguntur, iudicant, refusant */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "chorda.h"
#include "filum.h"
#include "stml.h"
#include "canon.h"
#include "insula.h"
#include "credo.h"
#include <stdio.h>

interior Canon*
canonem_legere (
                Piscina* p,
    InternamentumChorda* in,
     constans character* via)
{
    chorda  fons;
    chorda  causa;
     Canon* c;

    fons = filum_legere_totum(via, p);
    si (fons.mensura == ZEPHYRUM)
    {
        redde NIHIL;
    }
    c = canon_legere(fons, p, in, &causa);
    si (!c)
    {
        imprimere("canon %s: %.*s\n", via, (int)causa.mensura,
                  causa.datum);
    }
    redde c;
}

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

interior vacuum
ponere_zoom (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    (vacuum)ctx;
    insula_attributum_ponere(radix, p, in, "zoom", "2");
}

s32 principale (vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
                  Canon* durabilis;
                  Canon* ephemera;
     InsulaRepositorium* repo;
           StmlResultus  res;
                 chorda  domini;

    piscina = piscina_generare_dynamicum("probatio_pictor_canones",
        LXIV * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);

    imprimere("\n--- Canones leguntur ---\n");
    durabilis = canonem_legere(piscina, intern,
                               "apps/pictor/canones/durabilis.canon");
    ephemera  = canonem_legere(piscina, intern,
                               "apps/pictor/canones/ephemera.canon");
    CREDO_NON_NIHIL(durabilis);
    CREDO_NON_NIHIL(ephemera);

    imprimere("\n--- Insulae pictoris iudicantur ---\n");
    repo = insula_repositorium_creare(piscina, intern,
        "<documentum latitudo=\"320\" altitudo=\"200\" paletta=\"\">"
        "<stratum titulus=\"fundus\" visibilis=\"verum\" ordo=\"0\"/>"
        "</documentum>",
        "<ephemera instrumentum=\"penicillus\" color_primus=\"0\""
        " color_secundus=\"5\" magnitudo=\"1\" stratum_activum=\"0\""
        " zoom=\"1\" focus=\"tabula\"/>");
    CREDO_NON_NIHIL(repo);
    insula_ponere_canonem(repo, INSULA_DURABILIS, durabilis);
    insula_ponere_canonem(repo, INSULA_EPHEMERA, ephemera);
    /* attributum non declaratum: recusatur */
    CREDO_FALSUM(mutare_ephemera(repo, ponere_ignotum, NIHIL));
    CREDO_CHORDA_CONTINET(insula_causa(repo),
                          chorda_ex_literis("canon", piscina));

    imprimere("\n--- Domini leguntur; motus solus zoom scribit ---\n");
    domini = filum_legere_totum("apps/pictor/canones/domini.stml",
                                piscina);
    CREDO_CHORDA_NON_VACUA(domini);
    res = stml_legere_ex_literis(chorda_ut_cstr(domini, piscina),
                                 piscina, intern);
    CREDO_VERUM(res.successus);
    CREDO_AEQUALIS_I32(insula_dominos_legere(repo, INSULA_EPHEMERA,
                                             res.elementum_radix), XI);
    CREDO_AEQUALIS_I32(insula_dominos_legere(repo, INSULA_DURABILIS,
                                             res.elementum_radix), I);
    insula_scriptorem_ponere(repo,
        chorda_ex_literis("zoom.ponere", piscina));
    CREDO_FALSUM(mutare_ephemera(repo, ponere_zoom, NIHIL));
    insula_scriptorem_ponere(repo, chorda_ex_literis("motus", piscina));
    CREDO_VERUM(mutare_ephemera(repo, ponere_zoom, NIHIL));
    CREDO_VERUM(insula_restituere(repo));

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
