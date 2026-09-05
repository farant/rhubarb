/* probatio_componens.c - Probationes arboris componentium */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "componens.h"
#include "credo.h"
#include <stdio.h>

s32 principale (vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
              Componens* radix;
              Componens* bottone;
              Componens* tabula;
              Componens* inventus;
              Componens* radix2;
                  Fines  f;
                 chorda  textus;
                 chorda  quaesitum;

    piscina = piscina_generare_dynamicum("probatio_componens",
        LXIV * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);

    imprimere("\n--- Probans aedificationem ---\n");
    radix = componens_creare(piscina, intern, "radix", PARTES_NULLUM);
    CREDO_NON_NIHIL (radix);
    f.x = ZEPHYRUM; f.y = ZEPHYRUM; f.latitudo = CDLXXX; f.altitudo =
                                                             CCC;
    componens_ponere_fines(radix, f);

    bottone = componens_creare(piscina, intern, "servare",
        PARTES_BOTTONE);
    f.x = X; f.y = X; f.latitudo = L; f.altitudo = XX;
    componens_ponere_fines(bottone, f);
    componens_ponere_actio(bottone, "documentum.servare");
    componens_ponere_titulum(bottone, "Servare");
    componens_ponere_focusabilis(bottone, VERUM);
    componens_addere_liberum(radix, bottone);

    tabula = componens_creare(piscina, intern, "tabula", PARTES_TABULA);
    f.x = C; f.y = X; f.latitudo = CCC; f.altitudo = CC;
    componens_ponere_fines(tabula, f);
    componens_ponere_praedicatum(tabula, PRAEDICATUM_PROPRIUS);
    componens_ponere_actio(tabula, "penicillus.ictus");
    componens_ponere_focusabilis(tabula, VERUM);
    componens_addere_liberum(radix, tabula);

    CREDO_AEQUALIS_I32 (componens_numerus_liberorum(radix), II);
    CREDO_VERUM (componens_liberum(radix, I) == tabula);
    CREDO_VERUM (tabula->parens == radix);

    imprimere("\n--- Probans invenire_per_id ---\n");
    quaesitum  = chorda_ex_literis("tabula", piscina);
    inventus   = componens_invenire_per_id(radix, quaesitum);
    CREDO_VERUM (inventus == tabula);
    quaesitum  = chorda_ex_literis("nemo", piscina);
    inventus   = componens_invenire_per_id(radix, quaesitum);
    CREDO_NIHIL (inventus);

    imprimere("\n--- Probans partes tituli ---\n");
    CREDO_VERUM (partes_ex_titulo("bottone") == PARTES_BOTTONE);
    textus = chorda_ex_literis(partes_titulus(PARTES_TABULA), piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS (textus, "tabula");

    imprimere("\n--- Probans circuitum STML ---\n");
    textus = componens_scribere_stml(radix, piscina, intern, VERUM);
    CREDO_CHORDA_NON_VACUA (textus);
    radix2 = componens_legere_stml(chorda_ut_cstr(textus, piscina),
                                   piscina, intern);
    CREDO_NON_NIHIL (radix2);
    CREDO_VERUM (componens_aequalis(radix, radix2));
    quaesitum  = chorda_ex_literis("servare", piscina);
    inventus   = componens_invenire_per_id(radix2, quaesitum);
    CREDO_NON_NIHIL (inventus);
    CREDO_VERUM (inventus->partes == PARTES_BOTTONE);
    CREDO_VERUM (inventus->focusabilis);
    CREDO_CHORDA_AEQUALIS_LITERIS (inventus->actio,
        "documentum.servare");

    imprimere("\n--- Probans inaequalitatem ---\n");
    inventus->fines.x = inventus->fines.x + I;
    CREDO_FALSUM (componens_aequalis(radix, radix2));

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
