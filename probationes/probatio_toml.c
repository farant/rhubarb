/* probatio_toml.c - Probationes pro TOML parser */

#include "toml.h"
#include "credo.h"
#include <stdio.h>

/* ==================================================
 * Probationes Chordarum Simplicium
 * ================================================== */

hic_manens vacuum
probatio_chorda_simplex(vacuum)
{
    Piscina* p = piscina_generare_dynamicum("test", MMMMXCVI);
    TomlDocumentum* doc;
    chorda valor;

    doc = toml_legere_literis("Title = \"Hello World\"", p);

    CREDO_VERUM(toml_successus(doc));
    CREDO_AEQUALIS_I32(toml_numerus_introituum(doc), 1);
    CREDO_VERUM(toml_habet(doc, "Title"));

    valor = toml_capere_chorda(doc, "Title");
    CREDO_AEQUALIS_I32(valor.mensura, 11);
    CREDO_VERUM(chorda_aequalis(valor, chorda_ex_literis("Hello World", p)));

    piscina_destruere(p);
    imprimere("  [OK] Chorda simplex\n");
}

hic_manens vacuum
probatio_chorda_vacua(vacuum)
{
    Piscina* p = piscina_generare_dynamicum("test", MMMMXCVI);
    TomlDocumentum* doc;
    chorda valor;

    doc = toml_legere_literis("Empty = \"\"", p);

    CREDO_VERUM(toml_successus(doc));
    valor = toml_capere_chorda(doc, "Empty");
    CREDO_AEQUALIS_I32(valor.mensura, 0);

    piscina_destruere(p);
    imprimere("  [OK] Chorda vacua\n");
}

/* ==================================================
 * Probationes Numerorum
 * ================================================== */

hic_manens vacuum
probatio_numerus_positivus(vacuum)
{
    Piscina* p = piscina_generare_dynamicum("test", MMMMXCVI);
    TomlDocumentum* doc;
    s32 valor;

    doc = toml_legere_literis("Year = 1920", p);

    CREDO_VERUM(toml_successus(doc));
    CREDO_VERUM(toml_habet(doc, "Year"));

    valor = toml_capere_numerum(doc, "Year");
    CREDO_AEQUALIS_S32(valor, 1920);

    piscina_destruere(p);
    imprimere("  [OK] Numerus positivus\n");
}

hic_manens vacuum
probatio_numerus_negativus(vacuum)
{
    Piscina* p = piscina_generare_dynamicum("test", MMMMXCVI);
    TomlDocumentum* doc;
    s32 valor;

    doc = toml_legere_literis("Offset = -100", p);

    CREDO_VERUM(toml_successus(doc));
    valor = toml_capere_numerum(doc, "Offset");
    CREDO_AEQUALIS_S32(valor, -100);

    piscina_destruere(p);
    imprimere("  [OK] Numerus negativus\n");
}

/* ==================================================
 * Probationes Tabulatorum
 * ================================================== */

hic_manens vacuum
probatio_tabulatum_simplex(vacuum)
{
    Piscina* p = piscina_generare_dynamicum("test", MMMMXCVI);
    TomlDocumentum* doc;
    Xar* tags;
    chorda* tag;

    doc = toml_legere_literis("Tags = [\"fiction\", \"drama\"]", p);

    CREDO_VERUM(toml_successus(doc));
    CREDO_VERUM(toml_habet(doc, "Tags"));

    tags = toml_capere_tabulatum(doc, "Tags");
    CREDO_NON_NIHIL(tags);
    CREDO_AEQUALIS_I32(xar_numerus(tags), 2);

    tag = (chorda*)xar_obtinere(tags, 0);
    CREDO_VERUM(chorda_aequalis(*tag, chorda_ex_literis("fiction", p)));

    tag = (chorda*)xar_obtinere(tags, 1);
    CREDO_VERUM(chorda_aequalis(*tag, chorda_ex_literis("drama", p)));

    piscina_destruere(p);
    imprimere("  [OK] Tabulatum simplex\n");
}

hic_manens vacuum
probatio_tabulatum_multilinea(vacuum)
{
    Piscina* p = piscina_generare_dynamicum("test", MMMMXCVI);
    TomlDocumentum* doc;
    Xar* tags;

    doc = toml_legere_literis(
        "Tags = [\n"
        "    \"fiction\",\n"
        "    \"drama\",\n"
        "    \"classic\"\n"
        "]", p);

    CREDO_VERUM(toml_successus(doc));
    tags = toml_capere_tabulatum(doc, "Tags");
    CREDO_NON_NIHIL(tags);
    CREDO_AEQUALIS_I32(xar_numerus(tags), 3);

    piscina_destruere(p);
    imprimere("  [OK] Tabulatum multilinea\n");
}

/* ==================================================
 * Probationes Chordarum Multilinearum
 * ================================================== */

hic_manens vacuum
probatio_chorda_multilinea(vacuum)
{
    Piscina* p = piscina_generare_dynamicum("test", MMMMXCVI);
    TomlDocumentum* doc;
    chorda valor;

    doc = toml_legere_literis(
        "Summary = \"\"\"\n"
        "This is a multi-line\n"
        "string value.\n"
        "\"\"\"", p);

    CREDO_VERUM(toml_successus(doc));
    CREDO_VERUM(toml_habet(doc, "Summary"));

    valor = toml_capere_chorda(doc, "Summary");
    CREDO_VERUM(valor.mensura > 0);

    piscina_destruere(p);
    imprimere("  [OK] Chorda multilinea\n");
}

/* ==================================================
 * Probationes Documentorum Mixtorum
 * ================================================== */

hic_manens vacuum
probatio_documentum_mixtum(vacuum)
{
    Piscina* p = piscina_generare_dynamicum("test", MMMMXCVI);
    TomlDocumentum* doc;

    doc = toml_legere_literis(
        "Title = \"The Great Gatsby\"\n"
        "Author = \"F. Scott Fitzgerald\"\n"
        "Year = 1925\n"
        "\n"
        "Tags = [\"fiction\", \"classic\", \"american\"]\n"
        "\n"
        "Summary = \"\"\"\n"
        "A novel about the American Dream.\n"
        "\"\"\"", p);

    CREDO_VERUM(toml_successus(doc));
    CREDO_AEQUALIS_I32(toml_numerus_introituum(doc), 5);

    CREDO_VERUM(chorda_aequalis(toml_capere_chorda(doc, "Title"),
                          chorda_ex_literis("The Great Gatsby", p)));

    CREDO_AEQUALIS_S32(toml_capere_numerum(doc, "Year"), 1925);

    CREDO_AEQUALIS_I32(xar_numerus(toml_capere_tabulatum(doc, "Tags")), 3);

    piscina_destruere(p);
    imprimere("  [OK] Documentum mixtum\n");
}

/* ==================================================
 * Probationes Commentorum
 * ================================================== */

hic_manens vacuum
probatio_commenta(vacuum)
{
    Piscina* p = piscina_generare_dynamicum("test", MMMMXCVI);
    TomlDocumentum* doc;

    doc = toml_legere_literis(
        "# This is a comment\n"
        "Title = \"Hello\"\n"
        "# Another comment\n"
        "Year = 2020", p);

    CREDO_VERUM(toml_successus(doc));
    CREDO_AEQUALIS_I32(toml_numerus_introituum(doc), 2);

    piscina_destruere(p);
    imprimere("  [OK] Commenta\n");
}

/* ==================================================
 * Probationes Spatii
 * ================================================== */

hic_manens vacuum
probatio_spatium(vacuum)
{
    Piscina* p = piscina_generare_dynamicum("test", MMMMXCVI);
    TomlDocumentum* doc;

    doc = toml_legere_literis(
        "  Title   =   \"Hello\"  \n"
        "Year=1920", p);

    CREDO_VERUM(toml_successus(doc));
    CREDO_VERUM(toml_habet(doc, "Title"));
    CREDO_VERUM(toml_habet(doc, "Year"));

    piscina_destruere(p);
    imprimere("  [OK] Spatium\n");
}

/* ==================================================
 * Probationes Errorum
 * ================================================== */

hic_manens vacuum
probatio_error_sine_aequali(vacuum)
{
    Piscina* p = piscina_generare_dynamicum("test", MMMMXCVI);
    TomlDocumentum* doc;

    doc = toml_legere_literis("Title \"Hello\"", p);

    CREDO_FALSUM(toml_successus(doc));
    CREDO_AEQUALIS_I32(doc->linea_erroris, 1);

    piscina_destruere(p);
    imprimere("  [OK] Error sine aequali\n");
}

hic_manens vacuum
probatio_error_chorda_non_clausa(vacuum)
{
    Piscina* p = piscina_generare_dynamicum("test", MMMMXCVI);
    TomlDocumentum* doc;

    doc = toml_legere_literis(
        "Summary = \"\"\"\n"
        "Unclosed multiline", p);

    CREDO_FALSUM(toml_successus(doc));

    piscina_destruere(p);
    imprimere("  [OK] Error chorda non clausa\n");
}

/* ==================================================
 * Probationes Boolean et Table Headers
 * ================================================== */

hic_manens vacuum
probatio_boolean(vacuum)
{
    Piscina* p = piscina_generare_dynamicum("test", MMMMXCVI);
    TomlDocumentum* doc;

    doc = toml_legere_literis(
        "Active = true\n"
        "Disabled = false", p);

    CREDO_VERUM(toml_successus(doc));
    CREDO_AEQUALIS_I32(toml_numerus_introituum(doc), 2);
    CREDO_VERUM(toml_capere_boolean(doc, "Active"));
    CREDO_FALSUM(toml_capere_boolean(doc, "Disabled"));

    piscina_destruere(p);
    imprimere("  [OK] Boolean\n");
}

hic_manens vacuum
probatio_table_headers(vacuum)
{
    Piscina* p = piscina_generare_dynamicum("test", MMMMXCVI);
    TomlDocumentum* doc;

    doc = toml_legere_literis(
        "Title = \"Test\"\n"
        "[Tags]\n"
        "categories = [\"fiction\"]\n"
        "active = true", p);

    CREDO_VERUM(toml_successus(doc));
    /* Table header [Tags] should be skipped */
    CREDO_VERUM(toml_habet(doc, "Title"));
    CREDO_VERUM(toml_habet(doc, "categories"));
    CREDO_VERUM(toml_habet(doc, "active"));

    piscina_destruere(p);
    imprimere("  [OK] Table headers\n");
}

hic_manens vacuum
probatio_numerus_negativus_magnus(vacuum)
{
    Piscina* p = piscina_generare_dynamicum("test", MMMMXCVI);
    TomlDocumentum* doc;
    s32 annus;

    doc = toml_legere_literis("Year = -371", p);

    CREDO_VERUM(toml_successus(doc));
    annus = toml_capere_numerum(doc, "Year");
    CREDO_AEQUALIS_S32(annus, -371);

    piscina_destruere(p);
    imprimere("  [OK] Numerus negativus magnus\n");
}

/* ==================================================
 * Probatio Formatis LLM
 * ================================================== */

hic_manens vacuum
probatio_format_llm(vacuum)
{
    Piscina* p = piscina_generare_dynamicum("test", MMMMXCVI * IV);
    TomlDocumentum* doc;
    Xar* tags;

    /* Simulare output LLM typicum */
    doc = toml_legere_literis(
        "Title = \"Fruitfulness\"\n"
        "Author = \"Emile Zola\"\n"
        "Year = 1899\n"
        "\n"
        "Tags = [\n"
        "    \"fiction\",\n"
        "    \"naturalism\"\n"
        "]\n"
        "\n"
        "Summary = \"\"\"\n"
        "This is a novel about family life in fin-de-siecle France.\n"
        "It promotes large families as a solution to social problems.\n"
        "\"\"\"\n"
        "\n"
        "Notes = \"\"\"\n"
        "Year chosen as 1899 when the novel was first published in French.\n"
        "\"\"\"", p);

    CREDO_VERUM(toml_successus(doc));
    CREDO_AEQUALIS_I32(toml_numerus_introituum(doc), 6);

    CREDO_VERUM(chorda_aequalis(toml_capere_chorda(doc, "Title"),
                          chorda_ex_literis("Fruitfulness", p)));

    CREDO_VERUM(chorda_aequalis(toml_capere_chorda(doc, "Author"),
                          chorda_ex_literis("Emile Zola", p)));

    CREDO_AEQUALIS_S32(toml_capere_numerum(doc, "Year"), 1899);

    tags = toml_capere_tabulatum(doc, "Tags");
    CREDO_NON_NIHIL(tags);
    CREDO_AEQUALIS_I32(xar_numerus(tags), 2);

    CREDO_VERUM(toml_capere_chorda(doc, "Summary").mensura > 0);

    CREDO_VERUM(toml_capere_chorda(doc, "Notes").mensura > 0);

    piscina_destruere(p);
    imprimere("  [OK] Format LLM\n");
}

/* ==================================================
 * Principale
 * ================================================== */

s32 principale(s32 argc, character** argv)
{
    Piscina* piscina;
    b32 praeteritus;

    (vacuum)argc;
    (vacuum)argv;

    piscina = piscina_generare_dynamicum("credo", MMMMXCVI);
    credo_aperire(piscina);

    imprimere("=== PROBATIONES TOML ===\n\n");

    imprimere("Chordae Simplices:\n");
    probatio_chorda_simplex();
    probatio_chorda_vacua();

    imprimere("\nNumeri:\n");
    probatio_numerus_positivus();
    probatio_numerus_negativus();

    imprimere("\nTabulata:\n");
    probatio_tabulatum_simplex();
    probatio_tabulatum_multilinea();

    imprimere("\nChordae Multilineae:\n");
    probatio_chorda_multilinea();

    imprimere("\nDocumenta Mixta:\n");
    probatio_documentum_mixtum();

    imprimere("\nCommenta et Spatium:\n");
    probatio_commenta();
    probatio_spatium();

    imprimere("\nErrores:\n");
    probatio_error_sine_aequali();
    probatio_error_chorda_non_clausa();

    imprimere("\nBoolean et Table Headers:\n");
    probatio_boolean();
    probatio_table_headers();
    probatio_numerus_negativus_magnus();

    imprimere("\nFormat LLM:\n");
    probatio_format_llm();

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
