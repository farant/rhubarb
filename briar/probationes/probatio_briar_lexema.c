/* probatio_briar_lexema.c - Tabula linearum, classificatio, saepta,
 * fabrica, corpus fixturarum octetim exactum.
 *
 * LEX COLUMNAE 0 (spec par. II): tag aperit solum si '<' + initium
 * nominis in columna 0; '< 3', '<-', '</ x', ' <c!>' prosa sunt. LEX
 * SAEPTI: intra ``` aut ~~~ omnis linea prosa; clausura idem signum,
 * cursus >= aperiens, indentatio <= III.
 */

#include "latina.h"
#include "credo.h"
#include "briar_lexema.h"
#include "briar_lexicon.h"
#include "materia_token.h"
#include "piscina.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

hic_manens constans character* FIXA[] = {
    "briar/probationes/fixa/thistle/salve.thistle",
    "briar/probationes/fixa/thistle/salve_vitreum.thistle"
};

interior character*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura)
{
         FILE* f;
        longus longitudo;
    character* memoria;
        size_t lecti;

    f = fopen(via, "rb");
    si (f == NIHIL)
    {
        redde NIHIL;
    }
    si (fseek(f, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(f);
        redde NIHIL;
    }
    longitudo = ftell(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde NIHIL;
    }
    rewind(f);
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    lecti = fread(memoria, I, (size_t)longitudo, f);
    fclose(f);
    si (lecti != (size_t)longitudo)
    {
        redde NIHIL;
    }
    *mensura = (i32)longitudo;
    redde memoria;
}

/* valores concatenati == fons, byte_offset currens */
interior b32
_circuitus (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura,
                   i32* lexemata)
{
    Xar* xs      = briar_lexare_crude(piscina, fons, mensura);
    s32  cursor  = ZEPHYRUM;
    i32  i;
    i32  n;

    si (xs == NIHIL)
    {
        redde FALSUM;
    }
    n = xar_numerus(xs);
    per (i = ZEPHYRUM; i < n; i++)
    {
        constans MateriaToken* t = *(MateriaToken**)xar_obtinere(xs, i);

        si (t->byte_offset != cursor)
        {
            imprimere("    (lexema %d: offset %d, cursor %d)\n",
                (integer)i, (integer)t->byte_offset, (integer)cursor);
            redde FALSUM;
        }
        si (   t->valor.mensura > ZEPHYRUM
            && memcmp(t->valor.datum, fons + cursor,
                   (size_t)t->valor.mensura) != ZEPHYRUM)
        {
            redde FALSUM;
        }
        cursor = cursor + (s32)t->valor.mensura;
    }
    *lexemata = n;
    redde (b32)(cursor == (s32)mensura);
}

/* classificatio lineae 'index' documenti dati, statu saepti ab initio
 * ambulato usque ad eam */
interior BriarClassis
_classis (
               Piscina* piscina,
    constans character* fons,
                   i32  index)
{
     BriarLineae lineae;
    BriarSaeptum saeptum;
    BriarClassis c;
             i32 i;

    saeptum.aperta     = FALSUM;
    saeptum.signum     = ' ';
    saeptum.longitudo  = ZEPHYRUM;
    (vacuum)briar_lineae_scindere(piscina, fons, (i32)strlen(fons),
        &lineae);
    c.genus = BRIAR_LINEA_PROSA;
    per (i = ZEPHYRUM; i <= index; i++)
    {
        c = briar_lineam_classificare(&lineae, i, &saeptum);
    }
    redde c;
}

interior b32
_titulus_est (
                chorda  c,
    constans character* literae)
{
    redde (b32)(c.mensura == (i32)strlen(literae)
        && memcmp(c.datum, literae, (size_t)c.mensura) == ZEPHYRUM);
}

s32
principale (vacuum)
{
             b32  praeteritus;
         Piscina* piscina;
    BriarClassis  c;

    piscina = piscina_generare_dynamicum("probatio_briar_lexema",
        1048576);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    imprimere("\n--- Probans tabulam linearum ---\n");
    {
        BriarLineae lineae;

        CREDO_VERUM (briar_lineae_scindere(piscina, "a\nbb\r\n\nc", 8,
            &lineae));
        CREDO_AEQUALIS_I32 (briar_lineae_numerus(&lineae), (i32)4);
        CREDO_AEQUALIS_I32 (briar_linea(&lineae, ZEPHYRUM)->mensura, I);
        CREDO_AEQUALIS_I32 (briar_linea(&lineae, I)->terminator, II);
        CREDO_AEQUALIS_I32 (briar_linea(&lineae, II)->mensura,
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 (briar_linea(&lineae, III)->terminator,
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 (briar_linea(&lineae, III)->numerus, (i32)4);
        CREDO_NIHIL (briar_linea(&lineae, (i32)4));
        CREDO_VERUM (briar_lineae_scindere(piscina, "", ZEPHYRUM,
            &lineae));
        CREDO_AEQUALIS_I32 (briar_lineae_numerus(&lineae), ZEPHYRUM);
    }

    imprimere("\n--- Probans classificationem (lex columnae 0) ---\n");
    c = _classis(piscina, "#!/usr/bin/env briar\n", ZEPHYRUM);
    CREDO_AEQUALIS_S32 ((s32)c.genus, (s32)BRIAR_LINEA_INTERPRES);
    c = _classis(piscina, "x\n#!/usr/bin/env briar\n", I);
    CREDO_AEQUALIS_S32 ((s32)c.genus, (s32)BRIAR_LINEA_PROSA);
    c = _classis(piscina, "<c!>\n", ZEPHYRUM);
    CREDO_AEQUALIS_S32 ((s32)c.genus, (s32)BRIAR_LINEA_TAG_APERTUM);
    CREDO_VERUM (c.crudus);
    CREDO_FALSUM (c.dedens);
    CREDO_FALSUM (c.auto_clausum);
    CREDO_VERUM (_titulus_est(c.titulus, "c"));
    c = _classis(piscina, "<c! methodus=\"salve\">\n", ZEPHYRUM);
    CREDO_VERUM (c.crudus && _titulus_est(c.titulus, "c"));
    c = _classis(piscina, "<c!\\>\n", ZEPHYRUM);
    CREDO_VERUM (c.crudus && c.dedens);
    c = _classis(piscina, "<fenestra titulus=\"x\"/>  \n", ZEPHYRUM);
    CREDO_AEQUALIS_S32 ((s32)c.genus, (s32)BRIAR_LINEA_TAG_APERTUM);
    CREDO_FALSUM (c.crudus);
    CREDO_VERUM (c.auto_clausum);
    CREDO_VERUM (_titulus_est(c.titulus, "fenestra"));
    c = _classis(piscina, "<md-html>\n", ZEPHYRUM);
    CREDO_VERUM (_titulus_est(c.titulus, "md-html"));
    c = _classis(piscina, "</c>\n", ZEPHYRUM);
    CREDO_AEQUALIS_S32 ((s32)c.genus, (s32)BRIAR_LINEA_TAG_CLAUSUM);
    CREDO_VERUM (_titulus_est(c.titulus, "c"));
    c = _classis(piscina, "< 3\n", ZEPHYRUM);
    CREDO_AEQUALIS_S32 ((s32)c.genus, (s32)BRIAR_LINEA_PROSA);
    c = _classis(piscina, "<-\n", ZEPHYRUM);
    CREDO_AEQUALIS_S32 ((s32)c.genus, (s32)BRIAR_LINEA_PROSA);
    c = _classis(piscina, "</ x>\n", ZEPHYRUM);
    CREDO_AEQUALIS_S32 ((s32)c.genus, (s32)BRIAR_LINEA_PROSA);
    c = _classis(piscina, " <c!>\n", ZEPHYRUM);
    CREDO_AEQUALIS_S32 ((s32)c.genus, (s32)BRIAR_LINEA_PROSA);
    c = _classis(piscina, "<c>", ZEPHYRUM);   /* sine terminatore */
    CREDO_AEQUALIS_S32 ((s32)c.genus, (s32)BRIAR_LINEA_TAG_APERTUM);

    imprimere("\n--- Probans lex saepti ---\n");
    c = _classis(piscina, "```html\n<html>\n```\n<html!>\n", I);
    CREDO_AEQUALIS_S32 ((s32)c.genus, (s32)BRIAR_LINEA_PROSA);
    c = _classis(piscina, "```html\n<html>\n```\n<html!>\n", III);
    CREDO_AEQUALIS_S32 ((s32)c.genus, (s32)BRIAR_LINEA_TAG_APERTUM);
    c = _classis(piscina, "~~~\n<c!>\n~~~\n<c!>\n", I);
    CREDO_AEQUALIS_S32 ((s32)c.genus, (s32)BRIAR_LINEA_PROSA);
    c = _classis(piscina, "~~~\n<c!>\n~~~\n<c!>\n", III);
    CREDO_AEQUALIS_S32 ((s32)c.genus, (s32)BRIAR_LINEA_TAG_APERTUM);
    /* clausura brevior non claudit; signum alienum non claudit */
    c = _classis(piscina, "````\n```\n<c!>\n", II);
    CREDO_AEQUALIS_S32 ((s32)c.genus, (s32)BRIAR_LINEA_PROSA);
    c = _classis(piscina, "```\n~~~\n<c!>\n", II);
    CREDO_AEQUALIS_S32 ((s32)c.genus, (s32)BRIAR_LINEA_PROSA);
    /* indentatio III aperit, IV non */
    c = _classis(piscina, "   ```\n<c!>\n", I);
    CREDO_AEQUALIS_S32 ((s32)c.genus, (s32)BRIAR_LINEA_PROSA);
    c = _classis(piscina, "    ```\n<c!>\n", I);
    CREDO_AEQUALIS_S32 ((s32)c.genus, (s32)BRIAR_LINEA_TAG_APERTUM);
    /* saeptum apertum in fine */
    {
         BriarLineae lineae;
        BriarSaeptum saeptum;

        saeptum.aperta     = FALSUM;
        saeptum.signum     = ' ';
        saeptum.longitudo  = ZEPHYRUM;
        (vacuum)briar_lineae_scindere(piscina, "```\nx\n", (i32)6,
            &lineae);
        (vacuum)briar_lineam_classificare(&lineae, ZEPHYRUM, &saeptum);
        (vacuum)briar_lineam_classificare(&lineae, I, &saeptum);
        CREDO_VERUM (saeptum.aperta);
        CREDO_AEQUALIS_I32 (saeptum.longitudo, III);
    }

    imprimere("\n--- Probans fabricam ---\n");
    {
         BriarLineae lineae;
        BriarFabrica fabrica;
        MateriaToken* t;
        constans character* fons = "ab\ncd\nef";

        (vacuum)briar_lineae_scindere(piscina, fons, (i32)8, &lineae);
        briar_fabrica_incipere(&fabrica, piscina, &lineae);
        t = briar_lexema_fabricare(&fabrica, (s32)BRIAR_LEX_PROSA,
            (s32)3,
            (s32)8);
        CREDO_NON_NIHIL (t);
        CREDO_AEQUALIS_I32 (t->linea, II);
        CREDO_AEQUALIS_I32 (t->columna, I);
        CREDO_AEQUALIS_I32 (t->valor.mensura, (i32)5);
        CREDO_AEQUALIS_S32 (t->byte_offset, (s32)3);
        t = briar_lexema_fabricare(&fabrica, (s32)BRIAR_LEX_PROSA,
            (s32)7,
            (s32)7);
        CREDO_AEQUALIS_I32 (t->valor.mensura, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (t->columna, II);
        t = briar_lexema_finis(&fabrica);
        CREDO_AEQUALIS_S32 (t->byte_offset, (s32)8);
        CREDO_AEQUALIS_I32 (t->linea, III);
        CREDO_AEQUALIS_I32 (t->columna, III);
        t = briar_lexema_derivatum(&fabrica, (s32)BRIAR_LEX_DERIVATUM,
            briar_chorda_fontis(fons, ZEPHYRUM, I), t);
        CREDO_AEQUALIS_S32 (t->byte_offset, (s32)-I);
        CREDO_AEQUALIS_S32 (t->fons_index, BRIAR_FONS_DERIVATUS);
    }

    imprimere("\n--- Probans corpus fixturarum (octetim) ---\n");
    {
        i32 i;
        i32 lexemata_summa = ZEPHYRUM;

        per (i = ZEPHYRUM; i < (i32)(magnitudo(FIXA)
            / magnitudo(FIXA[0]));
            i++)
        {
            character* textus;
                  i32  mensura   = ZEPHYRUM;
                  i32  lexemata  = ZEPHYRUM;

            textus = _plagulam_legere(piscina, FIXA[i], &mensura);
            CREDO_NON_NIHIL (textus);
            si (textus == NIHIL)
            {
                perge;
            }
            CREDO_VERUM (_circuitus(piscina, textus, mensura,
                &lexemata));
            lexemata_summa = lexemata_summa + lexemata;
        }
        /* tegumentum suum metitur */
        CREDO_MAIOR_I32 (lexemata_summa, (i32)40);
    }

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
