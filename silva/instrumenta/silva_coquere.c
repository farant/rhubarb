/* silva_coquere.c - Coctio tabularum in fontem C */

#include "silva_coquere.h"
#include <stdio.h>
#include <string.h>

/* ==================================================
 * Auxiliares
 * ================================================== */

/* Imprimere chordam in plagulam */
interior vacuum
_ch (FILE* pl, chorda* c)
{
    si (c != NIHIL)
    {
        fprintf(pl, "%.*s", (int)c->mensura, (constans character*)c->datum);
    }
}

/* Basename viae (post ultimum '/') */
interior constans character*
_basis_tituli (constans character* via)
{
    constans character* ultimum;

    ultimum = strrchr(via, '/');
    redde (ultimum != NIHIL) ? ultimum + I : via;
}

/* Titulus generis actionis */
interior constans character*
_actio_titulus (SilvaGenActioGenus genus)
{
    commutatio (genus)
    {
        casus SILVA_GEN_ACTIO_TRANSPONERE:
            redde "SILVA_TAB_ACTIO_TRANSPONERE";
        casus SILVA_GEN_ACTIO_REDUCERE:
            redde "SILVA_TAB_ACTIO_REDUCERE";
        casus SILVA_GEN_ACTIO_ACCIPERE:
            redde "SILVA_TAB_ACTIO_ACCIPERE";
        casus SILVA_GEN_ACTIO_ERRARE:
        ordinarius:
            redde "SILVA_TAB_ACTIO_ERRARE";
    }
}

/* Comparator actionum: (terminalis, actio, valor) - ordo stabilis */
interior s32
_actio_comparator (constans vacuum* a, constans vacuum* b)
{
    constans SilvaGenActioIntroitus* aa =
        (constans SilvaGenActioIntroitus*)a;
    constans SilvaGenActioIntroitus* ab =
        (constans SilvaGenActioIntroitus*)b;

    si (aa->terminalis != ab->terminalis)
        redde (aa->terminalis < ab->terminalis) ? -I : I;
    si (aa->actio != ab->actio)
        redde ((i32)aa->actio < (i32)ab->actio) ? -I : I;
    si (aa->valor != ab->valor)
        redde (aa->valor < ab->valor) ? -I : I;
    redde ZEPHYRUM;
}

/* Comparator goto: non_terminalis */
interior s32
_goto_comparator (constans vacuum* a, constans vacuum* b)
{
    constans SilvaGenGotoIntroitus* ga =
        (constans SilvaGenGotoIntroitus*)a;
    constans SilvaGenGotoIntroitus* gb =
        (constans SilvaGenGotoIntroitus*)b;

    si (ga->non_terminalis != gb->non_terminalis)
        redde (ga->non_terminalis < gb->non_terminalis) ? -I : I;
    redde ZEPHYRUM;
}

/* Emittere kebab-minusculas ut MAIUSCULAS_SERPENTES */
interior vacuum
_maiusculas (FILE* pl, chorda* c)
{
    i32 i;

    si (c == NIHIL) redde;
    per (i = ZEPHYRUM; i < c->mensura; i++)
    {
        character ch = (character)c->datum[i];

        si (ch >= 'a' && ch <= 'z')
        {
            ch = (character)(ch - 'a' + 'A');
        }
        alioquin si (ch == '-')
        {
            ch = '_';
        }
        fputc(ch, pl);
    }
}

/* Emittere kebab ut serpentes ('-' -> '_') */
interior vacuum
_serpentes (FILE* pl, chorda* c)
{
    i32 i;

    si (c == NIHIL) redde;
    per (i = ZEPHYRUM; i < c->mensura; i++)
    {
        character ch = (character)c->datum[i];

        fputc((ch == '-') ? '_' : ch, pl);
    }
}

/* Emittere literas minusculas (praefixum functionum) */
interior vacuum
_minusculas_literis (FILE* pl, constans character* s)
{
    dum (*s != '\0')
    {
        character ch = *s;

        si (ch >= 'A' && ch <= 'Z')
        {
            ch = (character)(ch - 'A' + 'a');
        }
        fputc(ch, pl);
        s++;
    }
}

/* Titulus speciei loci (SilvaLocusSpecies ut s32) */
interior constans character*
_species_titulus (s32 species)
{
    commutatio (species)
    {
        casus 0: redde "SILVA_LOCUS_NODUS";
        casus I: redde "SILVA_LOCUS_TOKEN";
        casus II: redde "SILVA_LOCUS_LISTA_NODUS";
        casus III: redde "SILVA_LOCUS_LISTA_TOKEN";
        casus IV: redde "SILVA_LOCUS_LISTA_MIXTA";
        ordinarius: redde "SILVA_LOCUS_INDEX";
    }
}

interior b32
_pares (chorda* a, chorda* b)
{
    si (a == NIHIL || b == NIHIL) redde FALSUM;
    si (a->mensura != b->mensura) redde FALSUM;
    si (a->mensura == ZEPHYRUM) redde VERUM;
    redde (memcmp(a->datum, b->datum, (memoriae_index)a->mensura)
        == ZEPHYRUM) ? VERUM : FALSUM;
}

interior SilvaGenGenusDef*
_genus_def (Xar* genera, chorda* titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(genera); i++)
    {
        SilvaGenGenusDef* def = (SilvaGenGenusDef*)xar_obtinere(genera, i);

        si (def != NIHIL && _pares(def->titulus, titulus))
        {
            redde def;
        }
    }
    redde NIHIL;
}

interior s32
_locus_index (SilvaGenGenusDef* def, chorda* titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(def->loci); i++)
    {
        SilvaGenLocusDef* locus = (SilvaGenLocusDef*)xar_obtinere(
            def->loci, i);

        si (locus != NIHIL && _pares(locus->titulus, titulus))
        {
            redde (s32)i;
        }
    }
    redde -I;
}

/* Imprimere nomen terminalis pro commento ("$" pro -1) */
interior vacuum
_terminalem_commentare (FILE* pl, SilvaGenGrammatica* g, s32 terminalis)
{
    si (terminalis < ZEPHYRUM)
    {
        fprintf(pl, "$");
        redde;
    }
    {
        SilvaGenSymbolum* sym = (SilvaGenSymbolum*)xar_obtinere(
            g->symbola, (i32)terminalis);
        si (sym != NIHIL)
        {
            _ch(pl, sym->titulus);
        }
    }
}

/* ==================================================
 * Coquere
 * ================================================== */

b32
silva_gen_coquere (
    SilvaGenGrammatica* grammatica,
    SilvaGenTabula*     tabula,
    constans character* praefixum,
    constans character* basis_via,
    constans character* via_grammaticae)
{
    FILE* pl;
    character via[512];
    character custos[128];
    constans character* basis;
    i32 num_status;
    i32 num_symbolorum;
    i32 num_productionum;
    i32 s;
    i32 i;
    i32 offset_actionum;
    i32 offset_goto;
    s32 productio_augmentata;

    Xar* genera;

    si (grammatica == NIHIL || tabula == NIHIL)
    {
        redde FALSUM;
    }

    /* Registrum generum (S20 unificatio + errores) */
    genera = silva_gen_registrum_computare(grammatica);
    si (genera == NIHIL)
    {
        redde FALSUM;
    }

    basis = _basis_tituli(basis_via);
    num_status = xar_numerus(tabula->status_tabulae);
    num_symbolorum = xar_numerus(grammatica->symbola);
    num_productionum = xar_numerus(grammatica->productiones);

    /* Custos includendi: BASIS_H maiusculis */
    {
        i32 k;
        i32 m = (i32)strlen(basis);

        si (m > 120)
        {
            m = 120;
        }
        per (k = ZEPHYRUM; k < m; k++)
        {
            character c = basis[k];

            si (c >= 'a' && c <= 'z')
            {
                c = (character)(c - 'a' + 'A');
            }
            alioquin si (!((c >= 'A' && c <= 'Z')
                || (c >= '0' && c <= '9')))
            {
                c = '_';
            }
            custos[k] = c;
        }
        custos[m] = '_';
        custos[m + I] = 'H';
        custos[m + II] = '\0';
    }

    /* Productio augmentata */
    productio_augmentata = -I;
    per (i = ZEPHYRUM; i < num_productionum; i++)
    {
        SilvaGenProductio* prod = (SilvaGenProductio*)xar_obtinere(
            grammatica->productiones, i);
        si (prod != NIHIL && prod->sinistrum == grammatica->initium_index)
        {
            productio_augmentata = prod->index;
            frange;
        }
    }

    /* ----------------------------------------
     * Plagula .h
     * ---------------------------------------- */
    sprintf(via, "%s.h", basis_via);
    pl = fopen(via, "wb");
    si (pl == NIHIL)
    {
        redde FALSUM;
    }

    fprintf(pl,
        "/* %s.h - GENERATUM per silva_coquere ex %s\n"
        " * NOLI MANU MUTARE - regenera per silva/generare.sh.\n"
        " */\n\n"
        "#ifndef %s\n"
        "#define %s\n\n"
        "#include \"silva_tabulae.h\"\n"
        "#include \"silva_nodus.h\"\n\n"
        "externus constans SilvaTabulaCocta %s_TABULA;\n"
        "externus constans SilvaRegistrumCoctum %s_REGISTRUM;\n\n",
        basis, via_grammaticae, custos, custos, praefixum, praefixum);

    /* Enumeratio generum */
    fprintf(pl, "/* Genera nodorum (registrum unum: grammatica + extra) */\n"
        "enumeratio {\n");
    per (i = ZEPHYRUM; i < xar_numerus(genera); i++)
    {
        SilvaGenGenusDef* def = (SilvaGenGenusDef*)xar_obtinere(genera, i);

        fprintf(pl, "    %s_GENUS_", praefixum);
        _maiusculas(pl, def->titulus);
        fprintf(pl, " = %d%s\n", (int)i,
            (i < xar_numerus(genera) - I) ? "," : "");
    }
    fprintf(pl, "};\n\n");

    /* Prototypa manu (effugia) */
    per (i = ZEPHYRUM; i < num_productionum; i++)
    {
        SilvaGenProductio* prod = (SilvaGenProductio*)xar_obtinere(
            grammatica->productiones, i);

        si (prod != NIHIL && prod->manu != NIHIL)
        {
            fprintf(pl, "/* implendum manu (P%d) */\nSilvaValor ",
                (int)prod->index);
            _ch(pl, prod->manu);
            fprintf(pl,
                " (Piscina* piscina, constans SilvaValor* valores);\n\n");
        }
    }

    /* Accessores generati (probati: genus + fines) */
    per (i = ZEPHYRUM; i < xar_numerus(genera); i++)
    {
        SilvaGenGenusDef* def = (SilvaGenGenusDef*)xar_obtinere(genera, i);
        i32 k;

        per (k = ZEPHYRUM; k < xar_numerus(def->loci); k++)
        {
            SilvaGenLocusDef* locus = (SilvaGenLocusDef*)xar_obtinere(
                def->loci, k);

            fprintf(pl, "SilvaValor ");
            _minusculas_literis(pl, praefixum);
            fprintf(pl, "_");
            _serpentes(pl, def->titulus);
            fprintf(pl, "_");
            _serpentes(pl, locus->titulus);
            fprintf(pl, " (constans SilvaNodus* nodus);\n");
        }
    }

    /* Constructio generata */
    fprintf(pl, "\n/* Constructio ex reductione GLR: PURA (S26) - allocat\n"
        " * et implet solum; pater post-acceptum (S27) */\n"
        "SilvaValor ");
    _minusculas_literis(pl, praefixum);
    fprintf(pl, "_construere (\n"
        "    Piscina*             piscina,\n"
        "    s32                  productio,\n"
        "    constans SilvaValor* valores);\n");

    /* Fabrica ambigui (spec-v2 par 12): motor GLR valores furcarum
     * divergentium involvit sine registro cognito */
    fprintf(pl, "\n/* Fabrica ambigui: involucrum AMBIGUUS pro motore\n"
        " * GLR (interpretationes = lista valorum, canonica = index\n"
        " * spinae canonicae) */\n"
        "SilvaValor ");
    _minusculas_literis(pl, praefixum);
    fprintf(pl, "_ambiguum_fabricare (\n"
        "    Piscina*   piscina,\n"
        "    SilvaValor interpretationes,\n"
        "    s32        canonica);\n");

    fprintf(pl, "\n#endif /* %s */\n", custos);
    fclose(pl);

    /* ----------------------------------------
     * Plagula .c
     * ---------------------------------------- */
    sprintf(via, "%s.c", basis_via);
    pl = fopen(via, "wb");
    si (pl == NIHIL)
    {
        redde FALSUM;
    }

    fprintf(pl,
        "/* %s.c - GENERATUM per silva_coquere ex %s\n"
        " * NOLI MANU MUTARE - regenera per silva/generare.sh.\n"
        " *\n"
        " * status: %d, productiones: %d, symbola: %d,\n"
        " * conflictus servati (GLR): %d\n"
        " */\n\n"
        "#include \"%s.h\"\n\n",
        basis, via_grammaticae,
        (int)num_status, (int)num_productionum, (int)num_symbolorum,
        (int)tabula->numerus_conflictuum,
        basis);

    /* --- Symbola --- */
    fprintf(pl, "/* ==================================================\n"
        " * Symbola\n"
        " * ================================================== */\n\n"
        "hic_manens constans SilvaTabSymbolum %s_SYMBOLA[%d] = {\n",
        praefixum, (int)num_symbolorum);
    per (i = ZEPHYRUM; i < num_symbolorum; i++)
    {
        SilvaGenSymbolum* sym = (SilvaGenSymbolum*)xar_obtinere(
            grammatica->symbola, i);

        fprintf(pl, "    /* [%2d] */ { \"", (int)i);
        _ch(pl, sym->titulus);
        fprintf(pl, "\", ");
        si (sym->est_terminale && sym->genus != NIHIL)
        {
            _ch(pl, sym->genus);
        }
        alioquin
        {
            fprintf(pl, "SILVA_LEX_EOF");
        }
        fprintf(pl, ", %s }%s\n",
            sym->est_terminale ? "VERUM" : "FALSUM",
            (i < num_symbolorum - I) ? "," : "");
    }
    fprintf(pl, "};\n\n");

    /* --- Productiones --- */
    fprintf(pl, "/* ==================================================\n"
        " * Productiones\n"
        " * ================================================== */\n\n"
        "hic_manens constans SilvaTabProductio %s_PRODUCTIONES[%d] = {\n",
        praefixum, (int)num_productionum);
    per (i = ZEPHYRUM; i < num_productionum; i++)
    {
        SilvaGenProductio* prod = (SilvaGenProductio*)xar_obtinere(
            grammatica->productiones, i);
        SilvaGenSymbolum* sin = (SilvaGenSymbolum*)xar_obtinere(
            grammatica->symbola, (i32)prod->sinistrum);

        fprintf(pl, "    /* P%-2d ", (int)prod->index);
        _ch(pl, sin->titulus);
        fprintf(pl, " (%d symbola) */ { %d, %d, ",
            (int)xar_numerus(prod->dextrum),
            (int)prod->sinistrum,
            (int)xar_numerus(prod->dextrum));
        si (prod->id != NIHIL)
        {
            fprintf(pl, "\"");
            _ch(pl, prod->id);
            fprintf(pl, "\"");
        }
        alioquin
        {
            fprintf(pl, "NIHIL");
        }
        fprintf(pl, " }%s\n", (i < num_productionum - I) ? "," : "");
    }
    fprintf(pl, "};\n\n");

    /* --- Actiones (planae, ordinatae per statum) --- */
    fprintf(pl, "/* ==================================================\n"
        " * Actiones (laminae per statum in %s_STATUS)\n"
        " * ================================================== */\n\n"
        "hic_manens constans SilvaTabActio %s_ACTIONES[] = {\n",
        praefixum, praefixum);

    offset_actionum = ZEPHYRUM;
    per (s = ZEPHYRUM; s < num_status; s++)
    {
        SilvaGenStatusTabula* st = (SilvaGenStatusTabula*)xar_obtinere(
            tabula->status_tabulae, s);
        i32 num_act = xar_numerus(st->actiones);

        xar_ordinare(st->actiones, _actio_comparator);

        fprintf(pl, "    /* --- status %d%s --- */\n", (int)s,
            st->habet_conflictum ? " [CONFLICTUS SERVATUS]" : "");
        per (i = ZEPHYRUM; i < num_act; i++)
        {
            SilvaGenActioIntroitus* actio =
                (SilvaGenActioIntroitus*)xar_obtinere(st->actiones, i);

            fprintf(pl, "    { %3d, %s, %3d, 0 },  /* ",
                (int)actio->terminalis,
                _actio_titulus(actio->actio),
                (int)actio->valor);
            _terminalem_commentare(pl, grammatica, actio->terminalis);
            commutatio (actio->actio)
            {
                casus SILVA_GEN_ACTIO_TRANSPONERE:
                    fprintf(pl, ": transponere -> %d", (int)actio->valor);
                    frange;
                casus SILVA_GEN_ACTIO_REDUCERE:
                {
                    SilvaGenProductio* prod =
                        (SilvaGenProductio*)xar_obtinere(
                            grammatica->productiones, (i32)actio->valor);

                    fprintf(pl, ": reducere P%d", (int)actio->valor);
                    si (prod != NIHIL && prod->id != NIHIL)
                    {
                        fprintf(pl, " ");
                        _ch(pl, prod->id);
                    }
                    frange;
                }
                casus SILVA_GEN_ACTIO_ACCIPERE:
                    fprintf(pl, ": accipere");
                    frange;
                ordinarius:
                    frange;
            }
            fprintf(pl, " */\n");
        }
        offset_actionum += num_act;
    }
    fprintf(pl, "};\n\n");

    /* --- Goto (plana, ordinata per statum) --- */
    fprintf(pl, "/* ==================================================\n"
        " * Goto\n"
        " * ================================================== */\n\n"
        "hic_manens constans SilvaTabGoto %s_GOTO[] = {\n", praefixum);
    per (s = ZEPHYRUM; s < num_status; s++)
    {
        SilvaGenStatusTabula* st = (SilvaGenStatusTabula*)xar_obtinere(
            tabula->status_tabulae, s);
        i32 num_g = xar_numerus(st->goto_introitus);

        xar_ordinare(st->goto_introitus, _goto_comparator);

        si (num_g > ZEPHYRUM)
        {
            fprintf(pl, "    /* --- status %d --- */\n", (int)s);
        }
        per (i = ZEPHYRUM; i < num_g; i++)
        {
            SilvaGenGotoIntroitus* g =
                (SilvaGenGotoIntroitus*)xar_obtinere(st->goto_introitus, i);
            SilvaGenSymbolum* sym = (SilvaGenSymbolum*)xar_obtinere(
                grammatica->symbola, (i32)g->non_terminalis);

            fprintf(pl, "    { %3d, %3d },  /* ",
                (int)g->non_terminalis, (int)g->status_novus);
            _ch(pl, sym->titulus);
            fprintf(pl, " -> %d */\n", (int)g->status_novus);
        }
    }
    /* introitus terminator (series vacua C89 illicita) */
    fprintf(pl, "    { -1, -1 }   /* terminator */\n};\n\n");

    /* --- Status (laminae) --- */
    fprintf(pl, "/* ==================================================\n"
        " * Status (offset + numerus in series planas)\n"
        " * ================================================== */\n\n"
        "hic_manens constans SilvaTabStatus %s_STATUS[%d] = {\n",
        praefixum, (int)num_status);
    offset_actionum = ZEPHYRUM;
    offset_goto = ZEPHYRUM;
    per (s = ZEPHYRUM; s < num_status; s++)
    {
        SilvaGenStatusTabula* st = (SilvaGenStatusTabula*)xar_obtinere(
            tabula->status_tabulae, s);
        i32 num_act = xar_numerus(st->actiones);
        i32 num_g = xar_numerus(st->goto_introitus);

        fprintf(pl, "    /* status %2d */ { %4d, %2d, %4d, %2d }%s\n",
            (int)s,
            (int)offset_actionum, (int)num_act,
            (int)offset_goto, (int)num_g,
            (s < num_status - I) ? "," : "");
        offset_actionum += num_act;
        offset_goto += num_g;
    }
    fprintf(pl, "};\n\n");

    /* --- Tabula integra --- */
    fprintf(pl, "/* ==================================================\n"
        " * Tabula cocta integra\n"
        " * ================================================== */\n\n"
        "constans SilvaTabulaCocta %s_TABULA = {\n"
        "    %s_SYMBOLA, %d,\n"
        "    %d,  /* terminalia */\n"
        "    %s_PRODUCTIONES, %d,\n"
        "    %s_STATUS, %d,\n"
        "    %s_ACTIONES, %d,\n"
        "    %s_GOTO, %d,\n"
        "    %d,  /* initium (augmentatum) */\n"
        "    %d,  /* productio augmentata */\n"
        "    %d   /* conflictus servati */\n"
        "};\n",
        praefixum,
        praefixum, (int)num_symbolorum,
        (int)grammatica->numerus_terminalium,
        praefixum, (int)num_productionum,
        praefixum, (int)num_status,
        praefixum, (int)offset_actionum,
        praefixum, (int)offset_goto,
        (int)grammatica->initium_index,
        (int)productio_augmentata,
        (int)tabula->numerus_conflictuum);

    /* --- Registrum generum (layouts, S21) --- */
    {
        i32 offset_locorum = ZEPHYRUM;
        i32 g_i;

        fprintf(pl, "\n/* ==================================================\n"
            " * Registrum generum (layouts locorum)\n"
            " * ================================================== */\n\n"
            "hic_manens constans SilvaTabLocus %s_LOCI[] = {\n", praefixum);
        per (g_i = ZEPHYRUM; g_i < xar_numerus(genera); g_i++)
        {
            SilvaGenGenusDef* def = (SilvaGenGenusDef*)xar_obtinere(
                genera, g_i);
            i32 k;

            fprintf(pl, "    /* ");
            _ch(pl, def->titulus);
            fprintf(pl, " */\n");
            per (k = ZEPHYRUM; k < xar_numerus(def->loci); k++)
            {
                SilvaGenLocusDef* locus = (SilvaGenLocusDef*)xar_obtinere(
                    def->loci, k);

                fprintf(pl, "    { \"");
                _ch(pl, locus->titulus);
                fprintf(pl, "\", %s },\n", _species_titulus(locus->species));
            }
        }
        fprintf(pl, "    { NIHIL, -1 }   /* terminator */\n};\n\n");

        fprintf(pl,
            "hic_manens constans SilvaTabGenus %s_GENERA[] = {\n", praefixum);
        per (g_i = ZEPHYRUM; g_i < xar_numerus(genera); g_i++)
        {
            SilvaGenGenusDef* def = (SilvaGenGenusDef*)xar_obtinere(
                genera, g_i);

            fprintf(pl, "    /* [%2d] */ { \"", (int)g_i);
            _ch(pl, def->titulus);
            fprintf(pl, "\", %d, %d },\n",
                (int)offset_locorum, (int)xar_numerus(def->loci));
            offset_locorum += xar_numerus(def->loci);
        }
        fprintf(pl, "    { NIHIL, 0, 0 }   /* terminator */\n};\n\n");

        fprintf(pl, "constans SilvaRegistrumCoctum %s_REGISTRUM = {\n"
            "    %s_GENERA, %d,\n"
            "    %s_LOCI, %d\n"
            "};\n",
            praefixum,
            praefixum, (int)xar_numerus(genera),
            praefixum, (int)offset_locorum);
    }

    /* --- Accessores generati --- */
    {
        i32 g_i;

        fprintf(pl, "\n/* ==================================================\n"
            " * Accessores (probati: genus + fines)\n"
            " * ================================================== */\n");
        per (g_i = ZEPHYRUM; g_i < xar_numerus(genera); g_i++)
        {
            SilvaGenGenusDef* def = (SilvaGenGenusDef*)xar_obtinere(
                genera, g_i);
            i32 k;

            per (k = ZEPHYRUM; k < xar_numerus(def->loci); k++)
            {
                SilvaGenLocusDef* locus = (SilvaGenLocusDef*)xar_obtinere(
                    def->loci, k);

                fprintf(pl, "\nSilvaValor\n");
                _minusculas_literis(pl, praefixum);
                fprintf(pl, "_");
                _serpentes(pl, def->titulus);
                fprintf(pl, "_");
                _serpentes(pl, locus->titulus);
                fprintf(pl, " (constans SilvaNodus* nodus)\n{\n"
                    "    si (nodus == NIHIL || nodus->genus != (s32)%s_GENUS_",
                    praefixum);
                _maiusculas(pl, def->titulus);
                fprintf(pl, "\n        || %d >= nodus->numerus_locorum)\n"
                    "    {\n"
                    "        redde silva_valor_nihil();\n"
                    "    }\n"
                    "    redde nodus->loci[%d];\n"
                    "}\n", (int)k, (int)k);
            }
        }
    }

    /* --- Constructio generata --- */
    {
        fprintf(pl, "\n/* ==================================================\n"
            " * Constructio ex reductione (PURA - S26; pater numquam - S27)\n"
            " * ================================================== */\n\n"
            "SilvaValor\n");
        _minusculas_literis(pl, praefixum);
        fprintf(pl, "_construere (\n"
            "    Piscina*             piscina,\n"
            "    s32                  productio,\n"
            "    constans SilvaValor* valores)\n"
            "{\n"
            "    (vacuum)piscina;\n"
            "    (vacuum)valores;\n"
            "    commutatio (productio)\n"
            "    {\n");

        per (i = ZEPHYRUM; i < num_productionum; i++)
        {
            SilvaGenProductio* prod = (SilvaGenProductio*)xar_obtinere(
                grammatica->productiones, i);
            i32 num_dex = xar_numerus(prod->dextrum);

            fprintf(pl, "    casus %d:  /* ", (int)prod->index);
            si (prod->id != NIHIL)
            {
                _ch(pl, prod->id);
            }
            alioquin si (prod->sinistrum == grammatica->initium_index)
            {
                fprintf(pl, "augmentata");
            }
            alioquin
            {
                fprintf(pl, "transitus");
            }
            fprintf(pl, " */\n");

            si (prod->manu != NIHIL)
            {
                fprintf(pl, "        redde ");
                _ch(pl, prod->manu);
                fprintf(pl, "(piscina, valores);\n");
            }
            alioquin si (prod->modus != NIHIL
                && prod->modus->mensura == XIII
                && memcmp(prod->modus->datum, "lista-initium",
                       XIII) == ZEPHYRUM)
            {
                i32 k;

                fprintf(pl, "    {\n"
                    "        SilvaValor lista = "
                    "silva_valor_lista_nova(piscina);\n");
                per (k = ZEPHYRUM; k < num_dex; k++)
                {
                    SilvaGenLocusMappa* mappa =
                        (SilvaGenLocusMappa*)xar_obtinere(prod->loci, k);

                    si (mappa == NIHIL || mappa->titulus == NIHIL) perge;
                    fprintf(pl, "        lista = "
                        "silva_valor_lista_appendere(piscina, lista, "
                        "valores[%d]);\n", (int)k);
                }
                fprintf(pl, "        redde lista;\n    }\n");
            }
            alioquin si (prod->modus != NIHIL)
            {
                /* lista-appendere: valor @locus+ est lista accumulans */
                s32 k_plus = -I;
                i32 k;

                per (k = ZEPHYRUM; k < num_dex; k++)
                {
                    SilvaGenLocusMappa* mappa =
                        (SilvaGenLocusMappa*)xar_obtinere(prod->loci, k);

                    si (mappa != NIHIL && mappa->appendere)
                    {
                        k_plus = (s32)k;
                        frange;
                    }
                }
                si (k_plus < ZEPHYRUM)
                {
                    fprintf(stderr, "silva_coquere: P%d lista-appendere "
                        "sine @loco+\n", (int)prod->index);
                    fclose(pl);
                    redde FALSUM;
                }

                /* Appendere PURUM (A½): prospectus furcarum aliarum
                 * numquam laeduntur - copia-in-divergentia in
                 * silva_valor_lista_appendere */
                fprintf(pl, "    {\n"
                    "        SilvaValor lista = valores[%d];\n"
                    "        si (lista.genus != SILVA_VALOR_LISTA)\n"
                    "        {\n"
                    "            redde silva_valor_nihil();\n"
                    "        }\n", (int)k_plus);
                per (k = ZEPHYRUM; k < num_dex; k++)
                {
                    SilvaGenLocusMappa* mappa =
                        (SilvaGenLocusMappa*)xar_obtinere(prod->loci, k);

                    si (mappa == NIHIL || mappa->titulus == NIHIL
                        || k == (i32)k_plus)
                    {
                        perge;
                    }
                    fprintf(pl, "        lista = "
                        "silva_valor_lista_appendere(piscina, lista, "
                        "valores[%d]);\n", (int)k);
                }
                fprintf(pl, "        redde lista;\n    }\n");
            }
            alioquin si (prod->genus != NIHIL)
            {
                SilvaGenGenusDef* def = _genus_def(genera, prod->genus);
                i32 k;

                si (def == NIHIL)
                {
                    fclose(pl);
                    redde FALSUM;
                }
                fprintf(pl, "    {\n"
                    "        SilvaNodus* nodus = silva_nodus_creare(piscina,\n"
                    "            (s32)%s_GENUS_", praefixum);
                _maiusculas(pl, def->titulus);
                fprintf(pl, ", %d);\n"
                    "        si (nodus == NIHIL)\n"
                    "        {\n"
                    "            redde silva_valor_nihil();\n"
                    "        }\n", (int)xar_numerus(def->loci));

                per (k = ZEPHYRUM; k < num_dex; k++)
                {
                    SilvaGenLocusMappa* mappa =
                        (SilvaGenLocusMappa*)xar_obtinere(prod->loci, k);
                    SilvaGenLocusDef* locus;
                    s32 idx;

                    si (mappa == NIHIL || mappa->titulus == NIHIL) perge;
                    idx = _locus_index(def, mappa->titulus);
                    locus = (SilvaGenLocusDef*)xar_obtinere(
                        def->loci, (i32)idx);

                    si (mappa->appendere)
                    {
                        fprintf(pl, "        si (!silva_nodus_appendere("
                            "piscina, nodus, %d, valores[%d], %s))\n"
                            "        {\n"
                            "            redde silva_valor_nihil();\n"
                            "        }\n",
                            (int)idx, (int)k,
                            _species_titulus(locus->species));
                    }
                    alioquin
                    {
                        fprintf(pl, "        si (!silva_nodus_ponere("
                            "nodus, %d, valores[%d], %s))\n"
                            "        {\n"
                            "            redde silva_valor_nihil();\n"
                            "        }\n",
                            (int)idx, (int)k,
                            _species_titulus(locus->species));
                    }
                }
                fprintf(pl,
                    "        redde silva_valor_nodus(nodus);\n    }\n");
            }
            alioquin si (num_dex >= I)
            {
                /* transitus (vel augmentata): valor primus */
                fprintf(pl, "        redde valores[ZEPHYRUM];\n");
            }
            alioquin
            {
                /* epsilon sine modo */
                fprintf(pl, "        redde silva_valor_nihil();\n");
            }
        }

        fprintf(pl, "    ordinarius:\n"
            "        redde silva_valor_nihil();\n"
            "    }\n"
            "}\n");
    }

    /* Fabrica ambigui: forma ex registro lecta (loci per nomen, non
     * positione fixa) - generator formam iam validavit */
    {
        SilvaGenGenusDef* ambiguum_def = NIHIL;
        s32 locus_interp = -I;
        s32 locus_canonica = -I;
        s32 species_interp = II;
        i32 k;

        per (i = ZEPHYRUM; i < xar_numerus(genera); i++)
        {
            SilvaGenGenusDef* def =
                (SilvaGenGenusDef*)xar_obtinere(genera, i);

            si (def != NIHIL && def->titulus != NIHIL
                && chorda_aequalis_literis(*def->titulus, "ambiguus"))
            {
                ambiguum_def = def;
                frange;
            }
        }
        si (ambiguum_def == NIHIL)
        {
            fprintf(stderr,
                "silva_coquere: genus 'ambiguus' abest (validatio "
                "generatoris praeterita?)\n");
            fclose(pl);
            redde FALSUM;
        }
        per (k = ZEPHYRUM; k < xar_numerus(ambiguum_def->loci); k++)
        {
            SilvaGenLocusDef* locus = (SilvaGenLocusDef*)xar_obtinere(
                ambiguum_def->loci, k);

            si (locus == NIHIL || locus->titulus == NIHIL) perge;
            si (chorda_aequalis_literis(*locus->titulus,
                    "interpretationes"))
            {
                locus_interp = (s32)k;
                species_interp = locus->species;
            }
            si (chorda_aequalis_literis(*locus->titulus, "canonica"))
            {
                locus_canonica = (s32)k;
            }
        }
        si (locus_interp < ZEPHYRUM || locus_canonica < ZEPHYRUM)
        {
            fprintf(stderr, "silva_coquere: forma ambigui manca\n");
            fclose(pl);
            redde FALSUM;
        }

        fprintf(pl, "\n/* ==================================================\n"
            " * Fabrica ambigui (spec-v2 par 12)\n"
            " * ================================================== */\n\n"
            "SilvaValor\n");
        _minusculas_literis(pl, praefixum);
        fprintf(pl, "_ambiguum_fabricare (\n"
            "    Piscina*   piscina,\n"
            "    SilvaValor interpretationes,\n"
            "    s32        canonica)\n"
            "{\n"
            "    SilvaNodus* nodus = silva_nodus_creare(piscina,\n"
            "        (s32)%s_GENUS_", praefixum);
        _maiusculas(pl, ambiguum_def->titulus);
        fprintf(pl, ", %d);\n"
            "    si (nodus == NIHIL)\n"
            "    {\n"
            "        redde silva_valor_nihil();\n"
            "    }\n"
            "    si (!silva_nodus_ponere(nodus, %d, interpretationes, "
            "%s))\n"
            "    {\n"
            "        redde silva_valor_nihil();\n"
            "    }\n"
            "    si (!silva_nodus_ponere(nodus, %d, "
            "silva_valor_index(canonica),\n"
            "            SILVA_LOCUS_INDEX))\n"
            "    {\n"
            "        redde silva_valor_nihil();\n"
            "    }\n"
            "    redde silva_valor_nodus(nodus);\n"
            "}\n",
            (int)xar_numerus(ambiguum_def->loci),
            (int)locus_interp, _species_titulus(species_interp),
            (int)locus_canonica);
    }

    fclose(pl);
    redde VERUM;
}
