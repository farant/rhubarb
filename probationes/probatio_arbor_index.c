/* probatio_arbor_index.c - Probationes pro project index */

#include "credo.h"
#include "piscina.h"
#include "internamentum.h"
#include "arbor_syntaxis.h"
#include "arbor_index.h"
#include <stdio.h>
#include <string.h>

/* Via ad fixtures */
#define VIA_FIXA "probationes/fixa/arbor_index/"

/* ==================================================
 * Probationes - Creare
 * ================================================== */

interior vacuum
probatio_creare(Piscina* piscina, InternamentumChorda* intern)
{
    ArborProiectum* proj;

    imprimere("\n--- Creare Proiectum ---\n");

    proj = arbor_index_creare(piscina, intern);
    CREDO_NON_NIHIL(proj);

    imprimere("  arbor_index_creare: OK\n");
}

/* ==================================================
 * Probationes - Genus Nomen
 * ================================================== */

interior vacuum
probatio_genus_nomen(vacuum)
{
    constans character* str;

    imprimere("\n--- Genus Nomen ---\n");

    str = arbor_symbolum_genus_nomen(ARBOR_SYMBOLUM_FUNCTIO);
    CREDO_VERUM(strcmp(str, "functio") == ZEPHYRUM);
    imprimere("  ARBOR_SYMBOLUM_FUNCTIO: %s - OK\n", str);

    str = arbor_symbolum_genus_nomen(ARBOR_SYMBOLUM_VARIABILIS);
    CREDO_VERUM(strcmp(str, "variabilis") == ZEPHYRUM);
    imprimere("  ARBOR_SYMBOLUM_VARIABILIS: %s - OK\n", str);

    str = arbor_symbolum_genus_nomen(ARBOR_SYMBOLUM_TYPUS);
    CREDO_VERUM(strcmp(str, "typus") == ZEPHYRUM);
    imprimere("  ARBOR_SYMBOLUM_TYPUS: %s - OK\n", str);

    str = arbor_nexus_nomen(ARBOR_NEXUS_EXTERNUS);
    CREDO_VERUM(strcmp(str, "externus") == ZEPHYRUM);
    imprimere("  ARBOR_NEXUS_EXTERNUS: %s - OK\n", str);

    str = arbor_nexus_nomen(ARBOR_NEXUS_INTERNUS);
    CREDO_VERUM(strcmp(str, "internus") == ZEPHYRUM);
    imprimere("  ARBOR_NEXUS_INTERNUS: %s - OK\n", str);
}

/* ==================================================
 * Probationes - Symbolum Invenire (vacua)
 * ================================================== */

interior vacuum
probatio_symbolum_invenire_vacua(Piscina* piscina, InternamentumChorda* intern)
{
    ArborProiectum* proj;
    ArborIndexSymbolum* sym;
    chorda* func_titulus;

    imprimere("\n--- Symbolum Invenire (vacua) ---\n");

    proj = arbor_index_creare(piscina, intern);
    CREDO_NON_NIHIL(proj);

    /* Probare quod invenire reddat NIHIL pro symbolo non-existente */
    func_titulus = chorda_internare_ex_literis(intern, "nonexistent");
    sym = arbor_index_invenire(proj, func_titulus);
    CREDO_VERUM(sym == NIHIL);
    imprimere("  Invenire non-existent: OK (reddidit NIHIL)\n");
}

/* ==================================================
 * Probationes - Addere Via Include
 * ================================================== */

interior vacuum
probatio_via_include(Piscina* piscina, InternamentumChorda* intern)
{
    ArborProiectum* proj;

    imprimere("\n--- Via Include ---\n");

    proj = arbor_index_creare(piscina, intern);
    CREDO_NON_NIHIL(proj);

    /* Addere via include - non debet frangere */
    arbor_index_addere_via_include(proj, "/usr/include");
    arbor_index_addere_via_include(proj, "/usr/local/include");

    imprimere("  arbor_index_addere_via_include: OK\n");
}

/* ==================================================
 * Probationes - Aedificare (Empty Project)
 * ================================================== */

interior vacuum
probatio_aedificare_vacua(Piscina* piscina, InternamentumChorda* intern)
{
    ArborProiectum* proj;

    imprimere("\n--- Aedificare Vacua ---\n");

    proj = arbor_index_creare(piscina, intern);
    CREDO_NON_NIHIL(proj);

    /* Aedificare sine filis non debet frangere */
    arbor_index_aedificare(proj);

    imprimere("  arbor_index_aedificare (vacua): OK\n");
}

/* ==================================================
 * Probationes - Simplex (functiones et variabiles)
 * ================================================== */

interior vacuum
probatio_simplex(Piscina* piscina, InternamentumChorda* intern)
{
    ArborProiectum* proj;
    ArborIndexSymbolum* sym;
    chorda* titulus;

    imprimere("\n--- Simplex (functiones et variabiles) ---\n");

    proj = arbor_index_creare(piscina, intern);
    CREDO_NON_NIHIL(proj);

    arbor_index_addere_filum(proj, VIA_FIXA "simplex.c");
    arbor_index_aedificare(proj);

    /* Probare functio 'addere' */
    titulus = chorda_internare_ex_literis(intern, "addere");
    sym = arbor_index_invenire(proj, titulus);
    CREDO_NON_NIHIL(sym);
    CREDO_VERUM(sym->genus == ARBOR_SYMBOLUM_FUNCTIO);
    CREDO_VERUM(sym->nexus == ARBOR_NEXUS_EXTERNUS);
    imprimere("  Functio 'addere': genus=%s nexus=%s - OK\n",
              arbor_symbolum_genus_nomen(sym->genus),
              arbor_nexus_nomen(sym->nexus));

    /* Probare functio 'subtrahere' */
    titulus = chorda_internare_ex_literis(intern, "subtrahere");
    sym = arbor_index_invenire(proj, titulus);
    CREDO_NON_NIHIL(sym);
    CREDO_VERUM(sym->genus == ARBOR_SYMBOLUM_FUNCTIO);
    imprimere("  Functio 'subtrahere': OK\n");

    /* Probare variabilis 'numerus_globalis' */
    titulus = chorda_internare_ex_literis(intern, "numerus_globalis");
    sym = arbor_index_invenire(proj, titulus);
    CREDO_NON_NIHIL(sym);
    CREDO_VERUM(sym->genus == ARBOR_SYMBOLUM_VARIABILIS);
    CREDO_VERUM(sym->nexus == ARBOR_NEXUS_EXTERNUS);
    imprimere("  Variabilis 'numerus_globalis': genus=%s nexus=%s - OK\n",
              arbor_symbolum_genus_nomen(sym->genus),
              arbor_nexus_nomen(sym->nexus));
}

/* ==================================================
 * Probationes - Staticus (nexus internus)
 * ================================================== */

interior vacuum
probatio_staticus(Piscina* piscina, InternamentumChorda* intern)
{
    ArborProiectum* proj;
    ArborIndexSymbolum* sym;
    chorda* titulus;

    imprimere("\n--- Staticus (nexus internus) ---\n");

    proj = arbor_index_creare(piscina, intern);
    CREDO_NON_NIHIL(proj);

    arbor_index_addere_filum(proj, VIA_FIXA "staticus.c");
    arbor_index_aedificare(proj);

    /* Probare functio publica (externus) */
    titulus = chorda_internare_ex_literis(intern, "functio_publica");
    sym = arbor_index_invenire(proj, titulus);
    CREDO_NON_NIHIL(sym);
    CREDO_VERUM(sym->genus == ARBOR_SYMBOLUM_FUNCTIO);
    CREDO_VERUM(sym->nexus == ARBOR_NEXUS_EXTERNUS);
    imprimere("  Functio 'functio_publica': nexus=%s - OK\n",
              arbor_nexus_nomen(sym->nexus));

    /* Probare functio privata (internus/static) */
    titulus = chorda_internare_ex_literis(intern, "functio_privata");
    sym = arbor_index_invenire(proj, titulus);
    CREDO_NON_NIHIL(sym);
    CREDO_VERUM(sym->genus == ARBOR_SYMBOLUM_FUNCTIO);
    CREDO_VERUM(sym->nexus == ARBOR_NEXUS_INTERNUS);
    imprimere("  Functio 'functio_privata': nexus=%s - OK\n",
              arbor_nexus_nomen(sym->nexus));

    /* Probare variabilis privata (internus/static) */
    titulus = chorda_internare_ex_literis(intern, "numerus_privatus");
    sym = arbor_index_invenire(proj, titulus);
    CREDO_NON_NIHIL(sym);
    CREDO_VERUM(sym->genus == ARBOR_SYMBOLUM_VARIABILIS);
    CREDO_VERUM(sym->nexus == ARBOR_NEXUS_INTERNUS);
    imprimere("  Variabilis 'numerus_privatus': nexus=%s - OK\n",
              arbor_nexus_nomen(sym->nexus));
}

/* ==================================================
 * Probationes - Typus (struct, enum, typedef)
 * ================================================== */

interior vacuum
probatio_typus(Piscina* piscina, InternamentumChorda* intern)
{
    ArborProiectum* proj;
    ArborIndexSymbolum* sym;
    chorda* titulus;

    imprimere("\n--- Typus (struct, enum, typedef) ---\n");

    proj = arbor_index_creare(piscina, intern);
    CREDO_NON_NIHIL(proj);

    arbor_index_addere_filum(proj, VIA_FIXA "typus.c");
    arbor_index_aedificare(proj);

    /* Probare struct Punctum */
    titulus = chorda_internare_ex_literis(intern, "Punctum");
    sym = arbor_index_invenire(proj, titulus);
    CREDO_NON_NIHIL(sym);
    CREDO_VERUM(sym->genus == ARBOR_SYMBOLUM_TYPUS);
    imprimere("  Struct 'Punctum': genus=%s - OK\n",
              arbor_symbolum_genus_nomen(sym->genus));

    /* Probare union Valor */
    titulus = chorda_internare_ex_literis(intern, "Valor");
    sym = arbor_index_invenire(proj, titulus);
    CREDO_NON_NIHIL(sym);
    CREDO_VERUM(sym->genus == ARBOR_SYMBOLUM_TYPUS);
    imprimere("  Union 'Valor': genus=%s - OK\n",
              arbor_symbolum_genus_nomen(sym->genus));

    /* Probare enum Color */
    titulus = chorda_internare_ex_literis(intern, "Color");
    sym = arbor_index_invenire(proj, titulus);
    CREDO_NON_NIHIL(sym);
    CREDO_VERUM(sym->genus == ARBOR_SYMBOLUM_TYPUS);
    imprimere("  Enum 'Color': genus=%s - OK\n",
              arbor_symbolum_genus_nomen(sym->genus));

    /* Probare enum valores */
    titulus = chorda_internare_ex_literis(intern, "COLOR_RUBRUM");
    sym = arbor_index_invenire(proj, titulus);
    CREDO_NON_NIHIL(sym);
    CREDO_VERUM(sym->genus == ARBOR_SYMBOLUM_ENUM_VALOR);
    imprimere("  Enum valor 'COLOR_RUBRUM': genus=%s - OK\n",
              arbor_symbolum_genus_nomen(sym->genus));

    /* Probare typedef Integer32 */
    titulus = chorda_internare_ex_literis(intern, "Integer32");
    sym = arbor_index_invenire(proj, titulus);
    CREDO_NON_NIHIL(sym);
    CREDO_VERUM(sym->genus == ARBOR_SYMBOLUM_TYPUS);
    imprimere("  Typedef 'Integer32': genus=%s - OK\n",
              arbor_symbolum_genus_nomen(sym->genus));

    /* Probare functio creare_punctum */
    titulus = chorda_internare_ex_literis(intern, "creare_punctum");
    sym = arbor_index_invenire(proj, titulus);
    CREDO_NON_NIHIL(sym);
    CREDO_VERUM(sym->genus == ARBOR_SYMBOLUM_FUNCTIO);
    imprimere("  Functio 'creare_punctum': OK\n");
}

/* ==================================================
 * Probationes - Multi (plures fila)
 * ================================================== */

interior vacuum
probatio_multi(Piscina* piscina, InternamentumChorda* intern)
{
    ArborProiectum* proj;
    ArborIndexSymbolum* sym;
    chorda* titulus;

    imprimere("\n--- Multi (plures fila) ---\n");

    proj = arbor_index_creare(piscina, intern);
    CREDO_NON_NIHIL(proj);

    arbor_index_addere_filum(proj, VIA_FIXA "multi/main.c");
    arbor_index_addere_filum(proj, VIA_FIXA "multi/utils.c");
    arbor_index_aedificare(proj);

    /* Probare functiones ex utils.c */
    titulus = chorda_internare_ex_literis(intern, "multiplicare");
    sym = arbor_index_invenire(proj, titulus);
    CREDO_NON_NIHIL(sym);
    CREDO_VERUM(sym->genus == ARBOR_SYMBOLUM_FUNCTIO);
    imprimere("  Functio 'multiplicare' (utils.c): OK\n");

    titulus = chorda_internare_ex_literis(intern, "dividere");
    sym = arbor_index_invenire(proj, titulus);
    CREDO_NON_NIHIL(sym);
    CREDO_VERUM(sym->genus == ARBOR_SYMBOLUM_FUNCTIO);
    imprimere("  Functio 'dividere' (utils.c): OK\n");

    /* Probare functiones ex main.c */
    titulus = chorda_internare_ex_literis(intern, "computare");
    sym = arbor_index_invenire(proj, titulus);
    CREDO_NON_NIHIL(sym);
    CREDO_VERUM(sym->genus == ARBOR_SYMBOLUM_FUNCTIO);
    imprimere("  Functio 'computare' (main.c): OK\n");

    titulus = chorda_internare_ex_literis(intern, "main");
    sym = arbor_index_invenire(proj, titulus);
    CREDO_NON_NIHIL(sym);
    CREDO_VERUM(sym->genus == ARBOR_SYMBOLUM_FUNCTIO);
    imprimere("  Functio 'main' (main.c): OK\n");
}

/* ==================================================
 * Probationes - Referentiae
 * ================================================== */

interior vacuum
probatio_referentiae(Piscina* piscina, InternamentumChorda* intern)
{
    ArborProiectum* proj;
    Xar* refs;
    chorda* titulus;

    imprimere("\n--- Referentiae ---\n");

    proj = arbor_index_creare(piscina, intern);
    CREDO_NON_NIHIL(proj);

    arbor_index_addere_filum(proj, VIA_FIXA "multi/main.c");
    arbor_index_addere_filum(proj, VIA_FIXA "multi/utils.c");
    arbor_index_aedificare(proj);

    /* Probare referentias ad 'multiplicare' */
    titulus = chorda_internare_ex_literis(intern, "multiplicare");
    refs = arbor_index_referentiae(proj, titulus);
    /* Debet habere saltem definitionem */
    imprimere("  Referentiae 'multiplicare': %s\n",
              refs != NIHIL ? "inventae" : "NIHIL");

    /* Probare referentias ad symbolum non-existentem */
    titulus = chorda_internare_ex_literis(intern, "nonexistent");
    refs = arbor_index_referentiae(proj, titulus);
    CREDO_VERUM(refs == NIHIL);
    imprimere("  Referentiae non-existent: NIHIL - OK\n");
}

/* ==================================================
 * Main
 * ================================================== */

integer principale(vacuum)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    b32 praeteritus;

    imprimere("===========================================\n");
    imprimere("PROBATIO ARBOR INDEX\n");
    imprimere("===========================================\n");

    piscina = piscina_generare_dynamicum("probatio_arbor_index", M * LXIV);
    si (piscina == NIHIL)
    {
        imprimere("FRACTA: piscina_generare_dynamicum\n");
        redde I;
    }

    intern = internamentum_creare(piscina);
    si (intern == NIHIL)
    {
        imprimere("FRACTA: internamentum_creare\n");
        piscina_destruere(piscina);
        redde I;
    }

    credo_aperire(piscina);

    /* Probationes fundamentales */
    probatio_creare(piscina, intern);
    probatio_genus_nomen();
    probatio_symbolum_invenire_vacua(piscina, intern);
    probatio_via_include(piscina, intern);
    probatio_aedificare_vacua(piscina, intern);

    /* Probationes cum fixis */
    probatio_simplex(piscina, intern);
    probatio_staticus(piscina, intern);
    probatio_typus(piscina, intern);
    probatio_multi(piscina, intern);
    probatio_referentiae(piscina, intern);

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();

    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}
