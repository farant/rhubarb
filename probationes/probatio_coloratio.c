#include "coloratio.h"
#include "tabula_characterum.h"
#include "piscina.h"
#include "credo.h"
#include "registrum_commandi.h"
#include <stdio.h>
#include <string.h>


/* ==================================================
 * Dummy command for testing
 * ================================================== */

hic_manens b32
_dummy_command(ContextusCommandi* ctx)
{
    (vacuum)ctx;
    redde VERUM;
}


/* ==================================================
 * Helper: Write string to tabula at position
 * ================================================== */

hic_manens vacuum
_scribere_textum(
    TabulaCharacterum* tabula,
    i32 linea,
    i32 columna,
    constans character* textus)
{
    i32 i;
    i32 len;

    len = (i32)strlen(textus);
    per (i = ZEPHYRUM; i < len && columna + i < tabula->latitudo; i++)
    {
        tabula_cellula(tabula, linea, columna + i) = textus[i];
    }
}


/* ==================================================
 * Probationes: Creation
 * ================================================== */

hic_manens vacuum
probare_creare(Piscina* piscina)
{
    Coloratio* col;

    imprimere("  coloratio_creare...\n");

    col = coloratio_creare(piscina, XX, X);
    CREDO_NON_NIHIL(col);
    CREDO_AEQUALIS_I32(col->latitudo, XX);
    CREDO_AEQUALIS_I32(col->altitudo, X);
    CREDO_NON_NIHIL(col->indices);
    CREDO_NON_NIHIL(col->status_lineae);

    /* Verificare initiale valores sunt COLORATIO_DEFALTA */
    CREDO_AEQUALIS_I32((i32)coloratio_obtinere(col, ZEPHYRUM, ZEPHYRUM), COLORATIO_DEFALTA);
    CREDO_AEQUALIS_I32((i32)coloratio_obtinere(col, V, V), COLORATIO_DEFALTA);
}

hic_manens vacuum
probare_creare_ex_tabula(Piscina* piscina)
{
    TabulaCharacterum tabula;
    Coloratio* col;

    imprimere("  coloratio_creare_ex_tabula...\n");

    tabula_initiare(&tabula, piscina, XXX, XV);

    col = coloratio_creare_ex_tabula(piscina, &tabula);
    CREDO_NON_NIHIL(col);
    CREDO_AEQUALIS_I32(col->latitudo, XXX);
    CREDO_AEQUALIS_I32(col->altitudo, XV);
}

hic_manens vacuum
probare_creare_limites(Piscina* piscina)
{
    Coloratio* col;

    imprimere("  coloratio_creare limites...\n");

    /* Latitudo/altitudo invalidi */
    col = coloratio_creare(piscina, ZEPHYRUM, X);
    CREDO_NIHIL(col);

    col = coloratio_creare(piscina, X, ZEPHYRUM);
    CREDO_NIHIL(col);

    /* Piscina nihil */
    col = coloratio_creare(NIHIL, X, X);
    CREDO_NIHIL(col);
}


/* ==================================================
 * Probationes: Configuration
 * ================================================== */

hic_manens vacuum
probare_regulas(Piscina* piscina)
{
    Coloratio* col;

    imprimere("  coloratio regulae...\n");

    col = coloratio_creare(piscina, XX, X);
    CREDO_NON_NIHIL(col);

    /* Initiale: nullae regulae */
    CREDO_AEQUALIS_I32(col->regulae, ZEPHYRUM);

    /* Ponere regulas */
    coloratio_ponere_regulas(col, COLORATIO_REGULA_COMMANDA);
    CREDO_AEQUALIS_I32(col->regulae, COLORATIO_REGULA_COMMANDA);

    /* Addere regulam */
    coloratio_addere_regulam(col, COLORATIO_REGULA_STML);
    CREDO_AEQUALIS_I32(col->regulae, COLORATIO_REGULA_COMMANDA | COLORATIO_REGULA_STML);

    /* Removere regulam */
    coloratio_removere_regulam(col, COLORATIO_REGULA_COMMANDA);
    CREDO_AEQUALIS_I32(col->regulae, COLORATIO_REGULA_STML);

    /* Ponere replaces all */
    coloratio_ponere_regulas(col, COLORATIO_REGULA_SPUTNIK);
    CREDO_AEQUALIS_I32(col->regulae, COLORATIO_REGULA_SPUTNIK);
}


/* ==================================================
 * Probationes: $command Highlighting
 * ================================================== */

hic_manens vacuum
probare_commanda_simplex(Piscina* piscina)
{
    TabulaCharacterum tabula;
    Coloratio* col;

    imprimere("  $command simplex...\n");

    tabula_initiare(&tabula, piscina, XXX, V);
    _scribere_textum(&tabula, ZEPHYRUM, ZEPHYRUM, "Try $date here");

    col = coloratio_creare_ex_tabula(piscina, &tabula);
    coloratio_ponere_regulas(col, COLORATIO_REGULA_COMMANDA);
    coloratio_computare(col, &tabula);

    /* "Try " (positions 0-3) = default */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, ZEPHYRUM), COLORATIO_DEFALTA);  /* T */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, I), COLORATIO_DEFALTA);         /* r */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, II), COLORATIO_DEFALTA);        /* y */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, III), COLORATIO_DEFALTA);       /* ' ' */

    /* "$date" (positions 4-8) = command */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, IV), COLORATIO_COMMANDUM);      /* $ */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, V), COLORATIO_COMMANDUM);       /* d */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, VI), COLORATIO_COMMANDUM);      /* a */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, VII), COLORATIO_COMMANDUM);     /* t */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, VIII), COLORATIO_COMMANDUM);    /* e */

    /* " here" (positions 9+) = default */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, IX), COLORATIO_DEFALTA);        /* ' ' */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, X), COLORATIO_DEFALTA);         /* h */
}

hic_manens vacuum
probare_commanda_multiplex(Piscina* piscina)
{
    TabulaCharacterum tabula;
    Coloratio* col;

    imprimere("  $command multiplex...\n");

    tabula_initiare(&tabula, piscina, L, V);
    _scribere_textum(&tabula, ZEPHYRUM, ZEPHYRUM, "$first and $second");

    col = coloratio_creare_ex_tabula(piscina, &tabula);
    coloratio_ponere_regulas(col, COLORATIO_REGULA_COMMANDA);
    coloratio_computare(col, &tabula);

    /* "$first" (positions 0-5) = command */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, ZEPHYRUM), COLORATIO_COMMANDUM);
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, V), COLORATIO_COMMANDUM);

    /* " and " = default */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, VI), COLORATIO_DEFALTA);
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, X), COLORATIO_DEFALTA);

    /* "$second" (positions 11-17) = command */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, XI), COLORATIO_COMMANDUM);
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, XVII), COLORATIO_COMMANDUM);
}

hic_manens vacuum
probare_commanda_sine_regula(Piscina* piscina)
{
    TabulaCharacterum tabula;
    Coloratio* col;

    imprimere("  $command sine regula...\n");

    tabula_initiare(&tabula, piscina, XXX, V);
    _scribere_textum(&tabula, ZEPHYRUM, ZEPHYRUM, "Try $date here");

    col = coloratio_creare_ex_tabula(piscina, &tabula);
    /* Non ponere COLORATIO_REGULA_COMMANDA */
    coloratio_computare(col, &tabula);

    /* Omnia debent esse default */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, IV), COLORATIO_DEFALTA);
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, V), COLORATIO_DEFALTA);
}

hic_manens vacuum
probare_commanda_solum_signum(Piscina* piscina)
{
    TabulaCharacterum tabula;
    Coloratio* col;

    imprimere("  $ solum (no word)...\n");

    tabula_initiare(&tabula, piscina, XXX, V);
    _scribere_textum(&tabula, ZEPHYRUM, ZEPHYRUM, "Price: $ 100");

    col = coloratio_creare_ex_tabula(piscina, &tabula);
    coloratio_ponere_regulas(col, COLORATIO_REGULA_COMMANDA);
    coloratio_computare(col, &tabula);

    /* "$ " alone should NOT be highlighted (no word after $) */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, VII), COLORATIO_DEFALTA);  /* $ */
}

hic_manens vacuum
probare_commanda_ad_finem_lineae(Piscina* piscina)
{
    TabulaCharacterum tabula;
    Coloratio* col;

    imprimere("  $command ad finem lineae...\n");

    tabula_initiare(&tabula, piscina, XV, V);
    _scribere_textum(&tabula, ZEPHYRUM, ZEPHYRUM, "end $cmd");

    col = coloratio_creare_ex_tabula(piscina, &tabula);
    coloratio_ponere_regulas(col, COLORATIO_REGULA_COMMANDA);
    coloratio_computare(col, &tabula);

    /* "$cmd" at end = command */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, IV), COLORATIO_COMMANDUM);   /* $ */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, VII), COLORATIO_COMMANDUM);  /* d */
}


/* ==================================================
 * Probationes: Multi-line Block State
 * ================================================== */

hic_manens vacuum
probare_status_lineae_normalis(Piscina* piscina)
{
    TabulaCharacterum tabula;
    Coloratio* col;

    imprimere("  status lineae normalis...\n");

    tabula_initiare(&tabula, piscina, XXX, V);
    _scribere_textum(&tabula, ZEPHYRUM, ZEPHYRUM, "Normal text");
    _scribere_textum(&tabula, I, ZEPHYRUM, "More normal");

    col = coloratio_creare_ex_tabula(piscina, &tabula);
    coloratio_computare(col, &tabula);

    CREDO_AEQUALIS_I32(coloratio_obtinere_status_lineae(col, ZEPHYRUM), STATUS_LINEA_NORMALIS);
    CREDO_AEQUALIS_I32(coloratio_obtinere_status_lineae(col, I), STATUS_LINEA_NORMALIS);
}

hic_manens vacuum
probare_status_lineae_sputnik_block(Piscina* piscina)
{
    TabulaCharacterum tabula;
    Coloratio* col;

    imprimere("  status lineae sputnik block...\n");

    tabula_initiare(&tabula, piscina, L, X);
    _scribere_textum(&tabula, ZEPHYRUM, ZEPHYRUM, "Normal text");
    _scribere_textum(&tabula, I, ZEPHYRUM, "<sputnik>");
    _scribere_textum(&tabula, II, ZEPHYRUM, "sit x = 5;");
    _scribere_textum(&tabula, III, ZEPHYRUM, "si (x > 3) {");
    _scribere_textum(&tabula, IV, ZEPHYRUM, "</sputnik>");
    _scribere_textum(&tabula, V, ZEPHYRUM, "Normal again");

    col = coloratio_creare_ex_tabula(piscina, &tabula);
    coloratio_computare(col, &tabula);

    /* Line 0: Normal → remains NORMALIS */
    CREDO_AEQUALIS_I32(coloratio_obtinere_status_lineae(col, ZEPHYRUM), STATUS_LINEA_NORMALIS);

    /* Line 1: <sputnik> → becomes IN_SPUTNIK */
    CREDO_AEQUALIS_I32(coloratio_obtinere_status_lineae(col, I), STATUS_LINEA_IN_SPUTNIK);

    /* Lines 2,3: inside block → stays IN_SPUTNIK */
    CREDO_AEQUALIS_I32(coloratio_obtinere_status_lineae(col, II), STATUS_LINEA_IN_SPUTNIK);
    CREDO_AEQUALIS_I32(coloratio_obtinere_status_lineae(col, III), STATUS_LINEA_IN_SPUTNIK);

    /* Line 4: </sputnik> → returns to NORMALIS */
    CREDO_AEQUALIS_I32(coloratio_obtinere_status_lineae(col, IV), STATUS_LINEA_NORMALIS);

    /* Line 5: Normal → NORMALIS */
    CREDO_AEQUALIS_I32(coloratio_obtinere_status_lineae(col, V), STATUS_LINEA_NORMALIS);
}

hic_manens vacuum
probare_status_lineae_sputnik_inline(Piscina* piscina)
{
    TabulaCharacterum tabula;
    Coloratio* col;

    imprimere("  status lineae sputnik inline...\n");

    tabula_initiare(&tabula, piscina, L, V);
    _scribere_textum(&tabula, ZEPHYRUM, ZEPHYRUM, "Before <sputnik>code</sputnik> after");

    col = coloratio_creare_ex_tabula(piscina, &tabula);
    coloratio_computare(col, &tabula);

    /* Open and close on same line → ends as NORMALIS */
    CREDO_AEQUALIS_I32(coloratio_obtinere_status_lineae(col, ZEPHYRUM), STATUS_LINEA_NORMALIS);
}


/* ==================================================
 * Probationes: Vacare
 * ================================================== */

hic_manens vacuum
probare_vacare(Piscina* piscina)
{
    TabulaCharacterum tabula;
    Coloratio* col;

    imprimere("  coloratio_vacare...\n");

    tabula_initiare(&tabula, piscina, XXX, V);
    _scribere_textum(&tabula, ZEPHYRUM, ZEPHYRUM, "$cmd");

    col = coloratio_creare_ex_tabula(piscina, &tabula);
    coloratio_ponere_regulas(col, COLORATIO_REGULA_COMMANDA);
    coloratio_computare(col, &tabula);

    /* Verificare highlighted */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, ZEPHYRUM), COLORATIO_COMMANDUM);

    /* Vacare */
    coloratio_vacare(col);

    /* Verificare cleared */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, ZEPHYRUM), COLORATIO_DEFALTA);
    CREDO_AEQUALIS_I32(coloratio_obtinere_status_lineae(col, ZEPHYRUM), STATUS_LINEA_NORMALIS);
}


/* ==================================================
 * Probationes: Query Limites
 * ================================================== */

hic_manens vacuum
probare_obtinere_limites(Piscina* piscina)
{
    Coloratio* col;

    imprimere("  coloratio_obtinere limites...\n");

    col = coloratio_creare(piscina, XX, X);

    /* Out of bounds → DEFALTA (cast to avoid sign conversion warning) */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, (i32)-1, ZEPHYRUM), COLORATIO_DEFALTA);
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, (i32)-1), COLORATIO_DEFALTA);
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, X, ZEPHYRUM), COLORATIO_DEFALTA);
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, XX), COLORATIO_DEFALTA);

    /* NIHIL coloratio → DEFALTA */
    CREDO_AEQUALIS_I32(coloratio_obtinere(NIHIL, ZEPHYRUM, ZEPHYRUM), COLORATIO_DEFALTA);
}


/* ==================================================
 * Probationes: Registry-based Command Validation
 * ================================================== */

hic_manens vacuum
probare_commanda_cum_registro(Piscina* piscina)
{
    TabulaCharacterum tabula;
    Coloratio* col;
    RegistrumCommandi* reg;

    imprimere("  $command cum registro...\n");

    /* Creare registrum et registrare solum "date" */
    reg = registrum_commandi_creare(piscina);
    registrum_commandi_registrare(reg, "date", _dummy_command, NIHIL);

    tabula_initiare(&tabula, piscina, L, V);
    _scribere_textum(&tabula, ZEPHYRUM, ZEPHYRUM, "$date $time $unknown");

    col = coloratio_creare_ex_tabula(piscina, &tabula);
    coloratio_ponere_regulas(col, COLORATIO_REGULA_COMMANDA);
    coloratio_ponere_registrum(col, reg);
    coloratio_computare(col, &tabula);

    /* $date (0-4) should be highlighted - registered */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, ZEPHYRUM), COLORATIO_COMMANDUM);  /* $ */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, I), COLORATIO_COMMANDUM);         /* d */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, II), COLORATIO_COMMANDUM);        /* a */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, III), COLORATIO_COMMANDUM);       /* t */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, IV), COLORATIO_COMMANDUM);        /* e */

    /* Space at 5 - default */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, V), COLORATIO_DEFALTA);

    /* $time (6-10) should NOT be highlighted - not registered */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, VI), COLORATIO_DEFALTA);   /* $ */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, VII), COLORATIO_DEFALTA);  /* t */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, VIII), COLORATIO_DEFALTA); /* i */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, IX), COLORATIO_DEFALTA);   /* m */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, X), COLORATIO_DEFALTA);    /* e */

    /* $unknown (12-19) should NOT be highlighted - not registered */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, XII), COLORATIO_DEFALTA);  /* $ */
}

hic_manens vacuum
probare_commanda_sine_registro(Piscina* piscina)
{
    TabulaCharacterum tabula;
    Coloratio* col;

    imprimere("  $command sine registro (all highlighted)...\n");

    /* Sine registrum, all $word patterns highlighted */
    tabula_initiare(&tabula, piscina, L, V);
    _scribere_textum(&tabula, ZEPHYRUM, ZEPHYRUM, "$date $time $anything");

    col = coloratio_creare_ex_tabula(piscina, &tabula);
    coloratio_ponere_regulas(col, COLORATIO_REGULA_COMMANDA);
    /* Note: no coloratio_ponere_registrum() call - reg_commandi stays NIHIL */
    coloratio_computare(col, &tabula);

    /* All commands should be highlighted when no registry */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, ZEPHYRUM), COLORATIO_COMMANDUM);  /* $date */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, VI), COLORATIO_COMMANDUM);        /* $time */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, XII), COLORATIO_COMMANDUM);       /* $anything */
}


/* ==================================================
 * Probationes: Tag Highlighting
 * ================================================== */

hic_manens vacuum
probare_tag_simplex(Piscina* piscina)
{
    TabulaCharacterum tabula;
    Coloratio* col;

    imprimere("  tag simplex <div>...\n");

    tabula_initiare(&tabula, piscina, L, V);
    _scribere_textum(&tabula, ZEPHYRUM, ZEPHYRUM, "<div>");

    col = coloratio_creare_ex_tabula(piscina, &tabula);
    coloratio_ponere_regulas(col, COLORATIO_REGULA_STML);
    coloratio_computare(col, &tabula);

    /* <div> - all TAGUM */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, ZEPHYRUM), COLORATIO_TAGUM);  /* < */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, I), COLORATIO_TAGUM);         /* d */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, II), COLORATIO_TAGUM);        /* i */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, III), COLORATIO_TAGUM);       /* v */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, IV), COLORATIO_TAGUM);        /* > */
}

hic_manens vacuum
probare_tag_closing(Piscina* piscina)
{
    TabulaCharacterum tabula;
    Coloratio* col;

    imprimere("  tag closing </div>...\n");

    tabula_initiare(&tabula, piscina, L, V);
    _scribere_textum(&tabula, ZEPHYRUM, ZEPHYRUM, "</div>");

    col = coloratio_creare_ex_tabula(piscina, &tabula);
    coloratio_ponere_regulas(col, COLORATIO_REGULA_STML);
    coloratio_computare(col, &tabula);

    /* </div> - all TAGUM */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, ZEPHYRUM), COLORATIO_TAGUM);  /* < */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, I), COLORATIO_TAGUM);         /* / */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, II), COLORATIO_TAGUM);        /* d */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, V), COLORATIO_TAGUM);         /* > */
}

hic_manens vacuum
probare_tag_self_closing(Piscina* piscina)
{
    TabulaCharacterum tabula;
    Coloratio* col;

    imprimere("  tag self-closing <br/>...\n");

    tabula_initiare(&tabula, piscina, L, V);
    _scribere_textum(&tabula, ZEPHYRUM, ZEPHYRUM, "<br/>");

    col = coloratio_creare_ex_tabula(piscina, &tabula);
    coloratio_ponere_regulas(col, COLORATIO_REGULA_STML);
    coloratio_computare(col, &tabula);

    /* <br/> - all TAGUM */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, ZEPHYRUM), COLORATIO_TAGUM);  /* < */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, I), COLORATIO_TAGUM);         /* b */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, II), COLORATIO_TAGUM);        /* r */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, III), COLORATIO_TAGUM);       /* / */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, IV), COLORATIO_TAGUM);        /* > */
}

hic_manens vacuum
probare_tag_cum_attributo(Piscina* piscina)
{
    TabulaCharacterum tabula;
    Coloratio* col;

    imprimere("  tag cum attributo...\n");

    tabula_initiare(&tabula, piscina, L, V);
    /* <a href="url"> = 15 chars */
    _scribere_textum(&tabula, ZEPHYRUM, ZEPHYRUM, "<a href=\"url\">");

    col = coloratio_creare_ex_tabula(piscina, &tabula);
    coloratio_ponere_regulas(col, COLORATIO_REGULA_STML);
    coloratio_computare(col, &tabula);

    /* < a = TAGUM */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, ZEPHYRUM), COLORATIO_TAGUM);  /* < */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, I), COLORATIO_TAGUM);         /* a */

    /* space = DEFALTA */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, II), COLORATIO_DEFALTA);      /* space */

    /* href = ATTRIBUTUM */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, III), COLORATIO_ATTRIBUTUM);  /* h */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, IV), COLORATIO_ATTRIBUTUM);   /* r */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, V), COLORATIO_ATTRIBUTUM);    /* e */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, VI), COLORATIO_ATTRIBUTUM);   /* f */

    /* = OPERANS */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, VII), COLORATIO_OPERANS);     /* = */

    /* "url" = VALOR_ATTR */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, VIII), COLORATIO_VALOR_ATTR); /* " */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, IX), COLORATIO_VALOR_ATTR);   /* u */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, X), COLORATIO_VALOR_ATTR);    /* r */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, XI), COLORATIO_VALOR_ATTR);   /* l */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, XII), COLORATIO_VALOR_ATTR);  /* " */

    /* > = TAGUM */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, XIII), COLORATIO_TAGUM);      /* > */
}

hic_manens vacuum
probare_tag_boolean_attr(Piscina* piscina)
{
    TabulaCharacterum tabula;
    Coloratio* col;

    imprimere("  tag boolean attribute...\n");

    tabula_initiare(&tabula, piscina, L, V);
    /* <button disabled> = 17 chars */
    _scribere_textum(&tabula, ZEPHYRUM, ZEPHYRUM, "<button disabled>");

    col = coloratio_creare_ex_tabula(piscina, &tabula);
    coloratio_ponere_regulas(col, COLORATIO_REGULA_STML);
    coloratio_computare(col, &tabula);

    /* <button = TAGUM */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, ZEPHYRUM), COLORATIO_TAGUM);  /* < */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, I), COLORATIO_TAGUM);         /* b */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, VI), COLORATIO_TAGUM);        /* n */

    /* disabled = ATTRIBUTUM */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, VIII), COLORATIO_ATTRIBUTUM); /* d */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, XV), COLORATIO_ATTRIBUTUM);   /* d */

    /* > = TAGUM */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, XVI), COLORATIO_TAGUM);       /* > */
}

hic_manens vacuum
probare_tag_bare_value(Piscina* piscina)
{
    TabulaCharacterum tabula;
    Coloratio* col;

    imprimere("  tag bare value...\n");

    tabula_initiare(&tabula, piscina, L, V);
    /* <x val=123> = 11 chars */
    _scribere_textum(&tabula, ZEPHYRUM, ZEPHYRUM, "<x val=123>");

    col = coloratio_creare_ex_tabula(piscina, &tabula);
    coloratio_ponere_regulas(col, COLORATIO_REGULA_STML);
    coloratio_computare(col, &tabula);

    /* <x = TAGUM */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, ZEPHYRUM), COLORATIO_TAGUM);  /* < */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, I), COLORATIO_TAGUM);         /* x */

    /* val = ATTRIBUTUM */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, III), COLORATIO_ATTRIBUTUM);  /* v */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, V), COLORATIO_ATTRIBUTUM);    /* l */

    /* = OPERANS */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, VI), COLORATIO_OPERANS);      /* = */

    /* 123 = VALOR_ATTR */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, VII), COLORATIO_VALOR_ATTR);  /* 1 */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, VIII), COLORATIO_VALOR_ATTR); /* 2 */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, IX), COLORATIO_VALOR_ATTR);   /* 3 */

    /* > = TAGUM */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, X), COLORATIO_TAGUM);         /* > */
}

hic_manens vacuum
probare_tag_cum_commando(Piscina* piscina)
{
    TabulaCharacterum tabula;
    Coloratio* col;

    imprimere("  tag cum $command...\n");

    tabula_initiare(&tabula, piscina, L, V);
    /* <div>$date</div> = 16 chars */
    _scribere_textum(&tabula, ZEPHYRUM, ZEPHYRUM, "<div>$date</div>");

    col = coloratio_creare_ex_tabula(piscina, &tabula);
    coloratio_ponere_regulas(col, COLORATIO_REGULA_STML | COLORATIO_REGULA_COMMANDA);
    coloratio_computare(col, &tabula);

    /* <div> = TAGUM */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, ZEPHYRUM), COLORATIO_TAGUM);  /* < */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, IV), COLORATIO_TAGUM);        /* > */

    /* $date = COMMANDUM */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, V), COLORATIO_COMMANDUM);     /* $ */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, VI), COLORATIO_COMMANDUM);    /* d */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, IX), COLORATIO_COMMANDUM);    /* e */

    /* </div> = TAGUM */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, X), COLORATIO_TAGUM);         /* < */
    CREDO_AEQUALIS_I32(coloratio_obtinere(col, ZEPHYRUM, XV), COLORATIO_TAGUM);        /* > */
}


/* ==================================================
 * Main
 * ================================================== */

integer principale(vacuum)
{
    Piscina* piscina;

    piscina = piscina_generare_dynamicum("probatio_coloratio", M * LXIV);

    credo_aperire(piscina);

    imprimere("\n--- Probatio Coloratio ---\n");

    /* Creation */
    probare_creare(piscina);
    probare_creare_ex_tabula(piscina);
    probare_creare_limites(piscina);

    /* Configuration */
    probare_regulas(piscina);

    /* $command highlighting */
    probare_commanda_simplex(piscina);
    probare_commanda_multiplex(piscina);
    probare_commanda_sine_regula(piscina);
    probare_commanda_solum_signum(piscina);
    probare_commanda_ad_finem_lineae(piscina);

    /* Multi-line state */
    probare_status_lineae_normalis(piscina);
    probare_status_lineae_sputnik_block(piscina);
    probare_status_lineae_sputnik_inline(piscina);

    /* Vacare */
    probare_vacare(piscina);

    /* Query limites */
    probare_obtinere_limites(piscina);

    /* Registry-based validation */
    probare_commanda_cum_registro(piscina);
    probare_commanda_sine_registro(piscina);

    /* Tag highlighting */
    probare_tag_simplex(piscina);
    probare_tag_closing(piscina);
    probare_tag_self_closing(piscina);
    probare_tag_cum_attributo(piscina);
    probare_tag_boolean_attr(piscina);
    probare_tag_bare_value(piscina);
    probare_tag_cum_commando(piscina);

    credo_claudere();

    piscina_destruere(piscina);

    redde ZEPHYRUM;
}
