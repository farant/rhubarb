/* probatio_motus.c - structura motus, quies, effusio, captura */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "xar.h"
#include "stml.h"
#include "insula.h"
#include "motus.h"
#include "credo.h"
#include <stdio.h>

/* Mutator: pan ponere */
interior vacuum
pan_ponere (
     Motus* motus,
    vacuum* ctx)
{
    (vacuum)ctx;
    motus->pan.x = XL;
    motus->pan.y = XX;
}

/* Mutator: pan iterum movere */
interior vacuum
pan_movere (
     Motus* motus,
    vacuum* ctx)
{
    (vacuum)ctx;
    motus->pan.x = L;
}

/* Mutator: pan negativum (spatium signatum) */
interior vacuum
pan_negare (
     Motus* motus,
    vacuum* ctx)
{
    (vacuum)ctx;
    motus->pan.x = -XL;
    motus->pan.y = -X;
}

/* Mutator: punctum ictui pendenti addere (ctx = Punctum*) */
interior vacuum
punctum_addere (
     Motus* motus,
    vacuum* ctx)
{
    Punctum* p;

    p   = (Punctum*)xar_addere(motus->ictus_pendens);
    *p  = *(Punctum*)ctx;
}

s32 principale (vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
     InsulaRepositorium* repo;
                  Motus  motus;
                Punctum  p;
                 chorda* a;

    piscina = piscina_generare_dynamicum("probatio_motus", XVI * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    repo = insula_repositorium_creare(piscina, intern,
        "<documentum/>", "<ephemera/>");
    CREDO_NON_NIHIL(repo);
    motus_initiare(&motus, piscina);
    CREDO_VERUM(chorda_vacua(motus.captura));
    CREDO_FALSUM(motus.sordida);
    CREDO_AEQUALIS_I32(motus.zoom, I);

    imprimere("\n--- Mutatio motus NON tangit insulam ---\n");
    mutare_motum(&motus, pan_ponere, NIHIL, M);
    CREDO_AEQUALIS_S32(motus.pan.x, XL);
    CREDO_AEQUALIS_S32(motus.pan.y, XX);
    CREDO_VERUM(motus.sordida);
    CREDO_AEQUALIS_I32(insula_versio(repo, INSULA_EPHEMERA), ZEPHYRUM);
    CREDO_NIHIL(insula_attributum(repo, INSULA_EPHEMERA, "pan_x"));

    imprimere("\n--- Quies iudicatur ex tempore ---\n");
    CREDO_FALSUM(motus_quies(&motus, M + CC, CCC));
    CREDO_VERUM(motus_quies(&motus, M + CCC, CCC));
    p.x = I;
    p.y = II;
    /* mutatio nova quietem differt */
    mutare_motum(&motus, punctum_addere, &p, M + CCL);
    CREDO_FALSUM(motus_quies(&motus, M + CCC, CCC));
    CREDO_VERUM(motus_quies(&motus, M + D + L, CCC));
    CREDO_AEQUALIS_I32(xar_numerus(motus.ictus_pendens), I);

    imprimere("\n--- Effusio in quiete scribit insulam SEMEL ---\n");
    CREDO_VERUM(motus_effundere(&motus, repo));
    CREDO_AEQUALIS_I32(insula_versio(repo, INSULA_EPHEMERA), I);
    a = insula_attributum(repo, INSULA_EPHEMERA, "pan_x");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "40");
    a = insula_attributum(repo, INSULA_EPHEMERA, "pan_y");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "20");
    a = insula_attributum(repo, INSULA_EPHEMERA, "zoom");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "1");
    CREDO_FALSUM(motus.sordida);
    /* nihil sordidum: quies numquam, quamvis tempus fluat */
    CREDO_FALSUM(motus_quies(&motus, X * M, CCC));
    /* ictus pendens effusionem supervivit: insulam NUMQUAM tangit */
    CREDO_AEQUALIS_I32(xar_numerus(motus.ictus_pendens), I);

    imprimere("\n--- Effusio altera SUBSTITUIT, non geminat ---\n");
    mutare_motum(&motus, pan_movere, NIHIL, II * M);
    CREDO_VERUM(motus_quies(&motus, III * M, CCC));
    CREDO_VERUM(motus_effundere(&motus, repo));
    CREDO_AEQUALIS_I32(insula_versio(repo, INSULA_EPHEMERA), II);
    a = insula_attributum(repo, INSULA_EPHEMERA, "pan_x");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "50");
    CREDO_AEQUALIS_I32(
        xar_numerus(insula_radix(repo, INSULA_EPHEMERA)->attributa),
        III);
    CREDO_VERUM(insula_restituere(repo));
    CREDO_FALSUM(insula_mendacium(repo));

    imprimere("\n--- Pan negativum: coordinatae signatae ---\n");
    mutare_motum(&motus, pan_negare, NIHIL, IV * M);
    CREDO_AEQUALIS_S32(motus.pan.x, -XL);
    CREDO_VERUM(motus_quies(&motus, V * M, CCC));
    CREDO_VERUM(motus_effundere(&motus, repo));
    a = insula_attributum(repo, INSULA_EPHEMERA, "pan_x");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "-40");
    a = insula_attributum(repo, INSULA_EPHEMERA, "pan_y");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "-10");

    imprimere("\n--- Captura ---\n");
    motus_captura_ponere(&motus, chorda_ex_literis("tabula", piscina));
    CREDO_CHORDA_AEQUALIS_LITERIS(motus.captura, "tabula");
    motus_captura_tollere(&motus);
    CREDO_VERUM(chorda_vacua(motus.captura));

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
