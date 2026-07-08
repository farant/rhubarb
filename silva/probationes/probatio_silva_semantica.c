/* probatio_silva_semantica.c - M0a Chunk A: typi + forma +
 * aestimator (fixa ex colloquio pineata - consilium
 * project-specs/silva-semantica-design.md) */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "silva_token.h"
#include "silva_nodus.h"
#include "silva_parsare.h"
#include "silva_commissio.h"
#include "silva_c89_oraculum.h"
#include "silva_tabulae_c89.h"
#include "silva_c89_semantica.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

interior chorda
_ch (constans character* literis)
{
    chorda c;
    unio { constans character* c; i8* m; } u;

    u.c = literis;
    c.datum = u.m;
    c.mensura = (i32)strlen(literis);
    redde c;
}

interior constans SilvaNodus*
_nodus (constans SilvaParsura* parsura, i32 index)
{
    SilvaValor* e = silva_valor_lista_obtinere(
        parsura->commissio->radix, index);

    si (e == NIHIL || e->genus != SILVA_VALOR_NODUS)
    {
        redde NIHIL;
    }
    redde e->datum.nodus;
}

interior SilvaParsura*
_parsare (Piscina* piscina, constans character* fons)
{
    redde silva_c89_parsare(piscina, "probatio.c", fons,
        (i32)strlen(fons), NIHIL);
}

/* Basis declarationis (specificatores) + typus declaratoris primi */
interior TypusC89*
_typus_declarationis (SilvaSemantica* sem,
    constans SilvaNodus* declaratio, SilvaToken** titulus_out)
{
    TypusC89* basis;
    SilvaValor declaratores;
    i32 i;
    i32 m;

    basis = silva_c89_typus_ex_specificatoribus(sem,
        silva_c89_declaratio_specificatores(declaratio), NIHIL);
    declaratores = silva_c89_declaratio_declaratores(declaratio);
    m = (i32)silva_valor_lista_numerus(declaratores);
    per (i = ZEPHYRUM; i < m; i++)
    {
        SilvaValor* dv = silva_valor_lista_obtinere(declaratores, i);

        si (dv != NIHIL && dv->genus == SILVA_VALOR_NODUS)
        {
            redde silva_c89_typus_ex_declaratore(sem, basis,
                dv->datum.nodus, titulus_out);
        }
    }
    redde basis;
}

s32 principale (vacuum)
{
    Piscina* piscina;
    b32 praeteritus;

    piscina = piscina_generare_dynamicum("probatio_semantica",
        16777216);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);

    /* ========================================================
     * PROBARE: primitivi - singuli, canonici, TRES characteres
     * ======================================================== */
    {
        SilvaSemantica* sem = silva_c89_semantica_creare(piscina);
        SilvaParsura* parsura;
        TypusC89* a;
        TypusC89* b;
        TypusC89* c;

        imprimere("\n--- Probans primitivos ---\n");
        CREDO_NON_NIHIL (sem);

        parsura = _parsare(piscina,
            "char a; signed char b; unsigned char c;\n"
            "unsigned long x; long unsigned int y;\n"
            "int z;\n");
        CREDO_NON_NIHIL (parsura);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);

        /* tres characteres = TRES typi distincti */
        a = _typus_declarationis(sem, _nodus(parsura, 0), NIHIL);
        b = _typus_declarationis(sem, _nodus(parsura, I), NIHIL);
        c = _typus_declarationis(sem, _nodus(parsura, II), NIHIL);
        CREDO_AEQUALIS_PTR (a,
            silva_c89_typus_primitivum(sem, PRIMITIVUM_CHARACTER));
        CREDO_AEQUALIS_PTR (b, silva_c89_typus_primitivum(sem,
            PRIMITIVUM_CHARACTER_SIGNATUM));
        CREDO_AEQUALIS_PTR (c, silva_c89_typus_primitivum(sem,
            PRIMITIVUM_CHARACTER_INSIGNATUM));
        CREDO_INAEQUALITAS_PTR (a, b);
        CREDO_INAEQUALITAS_PTR (b, c);
        CREDO_AEQUALIS_I32 (a->magnitudo_octetorum, I);

        /* canonicalizatio: unsigned long == long unsigned int */
        a = _typus_declarationis(sem, _nodus(parsura, III), NIHIL);
        b = _typus_declarationis(sem, _nodus(parsura, IV), NIHIL);
        CREDO_AEQUALIS_PTR (a, b);
        CREDO_AEQUALIS_PTR (a, silva_c89_typus_primitivum(sem,
            PRIMITIVUM_LONGUS_INSIGNATUM));
        CREDO_AEQUALIS_I32 (a->magnitudo_octetorum, VIII);

        /* int */
        c = _typus_declarationis(sem, _nodus(parsura, V), NIHIL);
        CREDO_AEQUALIS_PTR (c,
            silva_c89_typus_primitivum(sem, PRIMITIVUM_INTEGER));
        CREDO_AEQUALIS_I32 (c->magnitudo_octetorum, IV);
        CREDO_AEQUALIS_I32 (c->ordinatio, IV);
    }

    /* ========================================================
     * PROBARE: qualificator scindit cursum; typedef + const
     * (fixum classicum: const IntPtr = int* const!)
     * ======================================================== */
    {
        SilvaSemantica* sem = silva_c89_semantica_creare(piscina);
        SilvaParsura* parsura;
        TypusC89* t;

        imprimere("\n--- Probans qualificatos ---\n");

        parsura = _parsare(piscina,
            "unsigned const long x;\n"
            "typedef int* IntPtr;\n"
            "const IntPtr p;\n");
        CREDO_NON_NIHIL (parsura);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);

        /* unsigned const long -> qualificatus(const, ulong) */
        t = _typus_declarationis(sem, _nodus(parsura, 0), NIHIL);
        CREDO_AEQUALIS_S32 (t->genus, (s32)TYPUS_C89_QUALIFICATUS);
        CREDO_AEQUALIS_S32 ((s32)t->datum.qualificatus.quales,
            (s32)QUALIS_CONSTANS);
        CREDO_AEQUALIS_PTR (t->datum.qualificatus.internum,
            silva_c89_typus_primitivum(sem,
                PRIMITIVUM_LONGUS_INSIGNATUM));

        /* typedef registratur per declarationem_tractare */
        CREDO_AEQUALIS_I32 ((i32)silva_c89_declarationem_tractare(
            sem, _nodus(parsura, I)), I);

        /* const IntPtr p -> int* const (NON const int*) */
        t = _typus_declarationis(sem, _nodus(parsura, II), NIHIL);
        CREDO_AEQUALIS_S32 (t->genus, (s32)TYPUS_C89_QUALIFICATUS);
        CREDO_AEQUALIS_S32 (t->datum.qualificatus.internum->genus,
            (s32)TYPUS_C89_MONSTRATOR);
        CREDO_AEQUALIS_PTR (t->datum.qualificatus.internum
                ->datum.monstrator.internum,
            silva_c89_typus_primitivum(sem, PRIMITIVUM_INTEGER));
    }

    /* ========================================================
     * PROBARE: declaratores - acies, monstratores, functiones
     * ======================================================== */
    {
        SilvaSemantica* sem = silva_c89_semantica_creare(piscina);
        SilvaParsura* parsura;
        TypusC89* t;
        SilvaToken* tok = NIHIL;

        imprimere("\n--- Probans declaratores ---\n");

        parsura = _parsare(piscina,
            "char *p[3];\n"
            "int (*f)(void);\n"
            "int g(int a[4], char b);\n"
            "int h();\n");
        CREDO_NON_NIHIL (parsura);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);

        /* char *p[3] = acies[3] monstratorum ad char; 24/8 */
        t = _typus_declarationis(sem, _nodus(parsura, 0), &tok);
        CREDO_NON_NIHIL (tok);
        CREDO_AEQUALIS_S32 (t->genus, (s32)TYPUS_C89_ACIES);
        CREDO_AEQUALIS_S32 (t->datum.acies.numerus, III);
        CREDO_AEQUALIS_S32 (t->datum.acies.elementum->genus,
            (s32)TYPUS_C89_MONSTRATOR);
        CREDO_VERUM (silva_c89_formam_computare(sem, t));
        CREDO_AEQUALIS_I32 (t->magnitudo_octetorum, XXIV);
        CREDO_AEQUALIS_I32 (t->ordinatio, VIII);

        /* int (*f)(void) = monstrator ad functionem */
        t = _typus_declarationis(sem, _nodus(parsura, I), &tok);
        CREDO_AEQUALIS_S32 (t->genus, (s32)TYPUS_C89_MONSTRATOR);
        CREDO_AEQUALIS_S32 (t->datum.monstrator.internum->genus,
            (s32)TYPUS_C89_FUNCTIO);
        CREDO_AEQUALIS_I32 ((i32)t->datum.monstrator.internum
            ->datum.functio.numerus_parametrorum, ZEPHYRUM);
        CREDO_VERUM (t->datum.monstrator.internum
            ->datum.functio.est_prototypata);

        /* g: parametrum acies -> monstrator (accommodatio C89) */
        t = _typus_declarationis(sem, _nodus(parsura, II), &tok);
        CREDO_AEQUALIS_S32 (t->genus, (s32)TYPUS_C89_FUNCTIO);
        CREDO_AEQUALIS_I32 ((i32)t->datum.functio
            .numerus_parametrorum, II);
        CREDO_AEQUALIS_S32 (t->datum.functio.parametra[0]->genus,
            (s32)TYPUS_C89_MONSTRATOR);
        CREDO_AEQUALIS_PTR (t->datum.functio.parametra[I],
            silva_c89_typus_primitivum(sem, PRIMITIVUM_CHARACTER));

        /* h() = non prototypata */
        t = _typus_declarationis(sem, _nodus(parsura, III), &tok);
        CREDO_AEQUALIS_S32 (t->genus, (s32)TYPUS_C89_FUNCTIO);
        CREDO_FALSUM (t->datum.functio.est_prototypata);
    }

    /* ========================================================
     * PROBARE: structura sui-referens + forma; unio; incompleta
     * ======================================================== */
    {
        SilvaSemantica* sem = silva_c89_semantica_creare(piscina);
        SilvaParsura* parsura;
        TypusC89* t;
        TypusC89* tag;

        imprimere("\n--- Probans structuras ---\n");

        parsura = _parsare(piscina,
            "typedef struct Nodus { int valor;"
            " struct Nodus* proximus; } Nodus;\n"
            "typedef union { const char* c;"
            " unsigned char* m; } U;\n"
            "struct Ignotus* p;\n");
        CREDO_NON_NIHIL (parsura);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);

        /* structura sui-referens: 16/8, offsets 0 et 8;
         * membrum[1] = monstrator ad IPSUM typum tag */
        CREDO_AEQUALIS_I32 ((i32)silva_c89_declarationem_tractare(
            sem, _nodus(parsura, 0)), I);
        {
            chorda titulus;
            unio { constans character* c; i8* m; } u;

            u.c = "Nodus";
            titulus.datum = u.m;
            titulus.mensura = V;
            tag = silva_c89_typedef_invenire(sem, titulus);
        }
        CREDO_NON_NIHIL (tag);
        CREDO_AEQUALIS_S32 (tag->genus, (s32)TYPUS_C89_STRUCTURA);
        CREDO_VERUM (tag->datum.tag.completa);
        CREDO_AEQUALIS_I32 ((i32)tag->datum.tag.numerus_membrorum,
            II);
        CREDO_VERUM (silva_c89_formam_computare(sem, tag));
        CREDO_AEQUALIS_I32 (tag->magnitudo_octetorum, XVI);
        CREDO_AEQUALIS_I32 (tag->datum.tag.membra[0].offset,
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 (tag->datum.tag.membra[I].offset, VIII);
        CREDO_AEQUALIS_S32 (tag->datum.tag.membra[I].typus->genus,
            (s32)TYPUS_C89_MONSTRATOR);
        CREDO_AEQUALIS_PTR (tag->datum.tag.membra[I].typus
            ->datum.monstrator.internum, tag);

        /* unio const-strip: 8/8, offsets ambo 0 */
        CREDO_AEQUALIS_I32 ((i32)silva_c89_declarationem_tractare(
            sem, _nodus(parsura, I)), I);
        {
            chorda titulus;
            unio { constans character* c; i8* m; } u;

            u.c = "U";
            titulus.datum = u.m;
            titulus.mensura = I;
            t = silva_c89_typedef_invenire(sem, titulus);
        }
        CREDO_NON_NIHIL (t);
        CREDO_AEQUALIS_S32 (t->genus, (s32)TYPUS_C89_UNIO);
        CREDO_VERUM (silva_c89_formam_computare(sem, t));
        CREDO_AEQUALIS_I32 (t->magnitudo_octetorum, VIII);
        CREDO_AEQUALIS_I32 (t->datum.tag.membra[I].offset, ZEPHYRUM);

        /* incompleta: monstrator bene (8), tag ipse sine forma */
        t = _typus_declarationis(sem, _nodus(parsura, II), NIHIL);
        CREDO_AEQUALIS_S32 (t->genus, (s32)TYPUS_C89_MONSTRATOR);
        CREDO_VERUM (silva_c89_formam_computare(sem, t));
        CREDO_AEQUALIS_I32 (t->magnitudo_octetorum, VIII);
        CREDO_FALSUM (silva_c89_formam_computare(sem,
            t->datum.monstrator.internum));
    }

    /* ========================================================
     * PROBARE: enumerationes + aestimator + mensurae acierum
     * ======================================================== */
    {
        SilvaSemantica* sem = silva_c89_semantica_creare(piscina);
        SilvaParsura* parsura;
        TypusC89* t;

        imprimere("\n--- Probans aestimatorem ---\n");

        parsura = _parsare(piscina,
            "enum Color { RUBER, VIRIDIS = 5, CAERULEUS };\n"
            "int a[CAERULEUS - 4];\n"
            "char b[sizeof(long)];\n"
            "int c[(2 * 3 + 1) << 1];\n"
            "int d['A' == 65 ? 3 : 99];\n");
        CREDO_NON_NIHIL (parsura);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);

        /* enum: constantes 0, 5, 6 - in spatio ORDINARIO (symbola) */
        CREDO_AEQUALIS_I32 ((i32)silva_c89_declarationem_tractare(
            sem, _nodus(parsura, 0)), ZEPHYRUM);
        {
            SemanticaSymbolum* symbolum = silva_c89_symbolum_invenire(
                sem, _ch("CAERULEUS"));

            CREDO_NON_NIHIL (symbolum);
            CREDO_AEQUALIS_S32 (symbolum->genus,
                (s32)SYMBOLUM_CONSTANS);
            CREDO_AEQUALIS_S64 (symbolum->valor, (s64)VI);
        }

        /* int a[CAERULEUS - 4] = acies[2] */
        t = _typus_declarationis(sem, _nodus(parsura, I), NIHIL);
        CREDO_AEQUALIS_S32 (t->genus, (s32)TYPUS_C89_ACIES);
        CREDO_AEQUALIS_S32 (t->datum.acies.numerus, II);

        /* char b[sizeof(long)] = acies[8] (recursio mutua!) */
        t = _typus_declarationis(sem, _nodus(parsura, II), NIHIL);
        CREDO_AEQUALIS_S32 (t->datum.acies.numerus, VIII);

        /* (2*3+1) << 1 = 14 */
        t = _typus_declarationis(sem, _nodus(parsura, III), NIHIL);
        CREDO_AEQUALIS_S32 (t->datum.acies.numerus, XIV);

        /* 'A' == 65 ? 3 : 99 = 3 (folium-character + ternarius) */
        t = _typus_declarationis(sem, _nodus(parsura, IV), NIHIL);
        CREDO_AEQUALIS_S32 (t->datum.acies.numerus, III);
    }

    /* ========================================================
     * PROBARE: internamentum - aequalitas monstratorum
     * ======================================================== */
    {
        SilvaSemantica* sem = silva_c89_semantica_creare(piscina);
        TypusC89* intus = silva_c89_typus_primitivum(sem,
            PRIMITIVUM_INTEGER);

        imprimere("\n--- Probans internamentum ---\n");

        CREDO_AEQUALIS_PTR (
            silva_c89_typus_monstrator(sem, intus),
            silva_c89_typus_monstrator(sem, intus));
        CREDO_AEQUALIS_PTR (
            silva_c89_typus_acies(sem, intus, X),
            silva_c89_typus_acies(sem, intus, X));
        CREDO_INAEQUALITAS_PTR (
            silva_c89_typus_acies(sem, intus, X),
            silva_c89_typus_acies(sem, intus, XI));
        CREDO_AEQUALIS_PTR (
            silva_c89_typus_qualificatus(sem, intus, QUALIS_CONSTANS),
            silva_c89_typus_qualificatus(sem, intus,
                QUALIS_CONSTANS));
        /* qualificatus sine qualibus = internum ipsum */
        CREDO_AEQUALIS_PTR (
            silva_c89_typus_qualificatus(sem, intus, ZEPHYRUM),
            intus);
    }

    /* ========================================================
     * PROBARE: venenum sine cascata (typus ignotus semanticae)
     * ======================================================== */
    {
        SilvaSemantica* sem = silva_c89_semantica_creare(piscina);
        SilvaOraculum* oraculum = silva_oraculum_creare(piscina);
        SilvaParsura* parsura;
        TypusC89* t;

        imprimere("\n--- Probans venenum ---\n");

        /* oraculum novit (parsura placida) sed semantica NON novit */
        CREDO_VERUM (silva_oraculum_typum_addere_literis(oraculum,
            "ignotus"));
        {
            constans character* fons_v = "ignotus x;\nint y;\n";

            parsura = silva_c89_parsare(piscina, "probatio.c",
                fons_v, (i32)strlen(fons_v), oraculum);
        }
        CREDO_NON_NIHIL (parsura);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);

        t = _typus_declarationis(sem, _nodus(parsura, 0), NIHIL);
        CREDO_AEQUALIS_S32 (t->genus, (s32)TYPUS_C89_ERROR);
        CREDO_VERUM (xar_numerus(sem->diagnostica) > ZEPHYRUM);
        CREDO_FALSUM (silva_c89_formam_computare(sem, t));

        /* sine cascata: declaratio sequens sana */
        t = _typus_declarationis(sem, _nodus(parsura, I), NIHIL);
        CREDO_AEQUALIS_PTR (t,
            silva_c89_typus_primitivum(sem, PRIMITIVUM_INTEGER));
    }

    /* ========================================================
     * PROBARE (Chunk B): analysis integra - scopi, symbola,
     * conditionalis, definitio functionis, umbratio
     * ======================================================== */
    {
        SilvaParsura* parsura;
        SilvaSemantica* sem;

        imprimere("\n--- Probans analysin integram ---\n");

        parsura = _parsare(piscina,
            "typedef int T;\n"
            "int g;\n"
            "#if 1\n"
            "int in_sumpto;\n"
            "#else\n"
            "int in_omisso;\n"
            "#endif\n"
            "static int quadratum(int x)\n"
            "{\n"
            "    int y;\n"
            "    { char y; }\n"
            "    return x;\n"
            "}\n");
        CREDO_NON_NIHIL (parsura);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);

        sem = silva_c89_semantica_analysare(piscina, parsura);
        CREDO_NON_NIHIL (sem);
        CREDO_AEQUALIS_I32 (xar_numerus(sem->diagnostica), ZEPHYRUM);

        /* symbola fili (post analysem scopus currens = summus) */
        {
            SemanticaSymbolum* symbolum;

            symbolum = silva_c89_symbolum_invenire(sem, _ch("T"));
            CREDO_NON_NIHIL (symbolum);
            CREDO_AEQUALIS_S32 (symbolum->genus,
                (s32)SYMBOLUM_TYPEDEF);
            CREDO_AEQUALIS_PTR (symbolum->typus,
                silva_c89_typus_primitivum(sem, PRIMITIVUM_INTEGER));

            symbolum = silva_c89_symbolum_invenire(sem, _ch("g"));
            CREDO_NON_NIHIL (symbolum);
            CREDO_AEQUALIS_S32 (symbolum->genus,
                (s32)SYMBOLUM_VARIABILE);
            CREDO_AEQUALIS_I32 ((i32)symbolum->profunditas,
                ZEPHYRUM);

            /* ramus sumptus visitatur; omissus (cruda) NON */
            CREDO_NON_NIHIL (silva_c89_symbolum_invenire(sem,
                _ch("in_sumpto")));
            CREDO_NIHIL (silva_c89_symbolum_invenire(sem,
                _ch("in_omisso")));

            symbolum = silva_c89_symbolum_invenire(sem,
                _ch("quadratum"));
            CREDO_NON_NIHIL (symbolum);
            CREDO_AEQUALIS_S32 (symbolum->genus,
                (s32)SYMBOLUM_FUNCTIO);
            CREDO_VERUM ((symbolum->repositio & REPOSITIO_STATICA)
                != ZEPHYRUM);
            CREDO_AEQUALIS_S32 (symbolum->typus->genus,
                (s32)TYPUS_C89_FUNCTIO);
            CREDO_AEQUALIS_I32 ((i32)symbolum->typus
                ->datum.functio.numerus_parametrorum, I);
        }

        /* index: parametrum x (prof I), y bis (int prof II,
         * char prof III) - umbratio per scopos nidificatos */
        {
            i32 i;
            i32 numerus_y = ZEPHYRUM;
            b32 x_parametrum = FALSUM;
            b32 y_char_profundius = FALSUM;

            per (i = ZEPHYRUM; i < xar_numerus(sem->symbola); i++)
            {
                SemanticaSymbolum* symbolum =
                    *(SemanticaSymbolum**)xar_obtinere(sem->symbola,
                        i);

                si (symbolum->titulus.mensura == I
                    && symbolum->titulus.datum[ZEPHYRUM] == 'y')
                {
                    numerus_y++;
                    si (symbolum->typus == silva_c89_typus_primitivum(
                            sem, PRIMITIVUM_CHARACTER)
                        && symbolum->profunditas >= III)
                    {
                        y_char_profundius = VERUM;
                    }
                }
                si (symbolum->titulus.mensura == I
                    && symbolum->titulus.datum[ZEPHYRUM] == 'x'
                    && symbolum->genus == (s32)SYMBOLUM_PARAMETRUM)
                {
                    x_parametrum = VERUM;
                }
            }
            CREDO_AEQUALIS_I32 ((i32)numerus_y, II);
            CREDO_VERUM (x_parametrum);
            CREDO_VERUM (y_char_profundius);
        }
    }

    /* ========================================================
     * PROBARE (Chunk B): conflictus generis eodem scopo
     * ======================================================== */
    {
        SilvaParsura* parsura;
        SilvaSemantica* sem;

        imprimere("\n--- Probans conflictum generis ---\n");

        parsura = _parsare(piscina,
            "enum E2 { K };\n"
            "int K;\n");
        CREDO_NON_NIHIL (parsura);
        sem = silva_c89_semantica_analysare(piscina, parsura);
        CREDO_NON_NIHIL (sem);
        CREDO_VERUM (xar_numerus(sem->diagnostica) > ZEPHYRUM);
    }

    /* ========================================================
     * PROBARE (Chunk C): oraculum trivalens - lectiones nominum
     * NON-typorum occiduntur (collapsus ad commissionem)
     * ======================================================== */
    {
        SilvaOraculum* oraculum;
        SilvaParsura* parsura;
        constans character* fons_v;

        imprimere("\n--- Probans oraculum trivalens ---\n");

        /* praeoneratum: foo non-typus -> (foo)(x) collapsus */
        oraculum = silva_oraculum_creare(piscina);
        CREDO_VERUM (silva_oraculum_non_typum_addere_literis(
            oraculum, "foo"));
        CREDO_VERUM (silva_oraculum_non_typum_novit(oraculum,
            _ch("foo")));
        fons_v = "(foo)(x);\n";
        parsura = silva_c89_parsare(piscina, "probatio.c", fons_v,
            (i32)strlen(fons_v), oraculum);
        CREDO_NON_NIHIL (parsura);
        CREDO_AEQUALIS_I32 (xar_numerus(
            parsura->commissio->ambigui), ZEPHYRUM);

        /* registratio praecommissionis IPSIUS PLAGULAE: acies
         * REGULAE declarata -> sizeof(REGULAE) collapsus (THE
         * mechanismus 84% - census par VII) */
        fons_v = "int REGULAE[10];\nint n = sizeof(REGULAE);\n";
        parsura = silva_c89_parsare(piscina, "probatio.c", fons_v,
            (i32)strlen(fons_v), NIHIL);
        CREDO_NON_NIHIL (parsura);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (xar_numerus(
            parsura->commissio->ambigui), ZEPHYRUM);

        /* nomen vere ignotum: retentio manet (honesta) */
        fons_v = "int n = sizeof(IGNOTUM_X);\n";
        parsura = silva_c89_parsare(piscina, "probatio.c", fons_v,
            (i32)strlen(fons_v), NIHIL);
        CREDO_NON_NIHIL (parsura);
        CREDO_AEQUALIS_I32 (xar_numerus(
            parsura->commissio->ambigui), I);
    }

    /* ========================================================
     * PROBARE (Chunk C): systema - parsura, harvest, praeoneratio,
     * analysis cum systemate (diagnostica -> 0), clausura
     * ======================================================== */
    {
        constans character* radix_env;
        character via[1024];
        FILE* pl;
        character* fons_sys = NIHIL;
        long mensura_sys = 0L;
        SilvaParsura* parsura_sys = NIHIL;
        SilvaSemantica* sem_sys = NIHIL;

        imprimere("\n--- Probans systema ---\n");

        radix_env = getenv("RHUBARB_RADIX");
        si (radix_env == NIHIL)
        {
            radix_env = "..";
        }
        sprintf(via, "%s/silva/fontes/systema_c89.h", radix_env);
        pl = fopen(via, "rb");
        CREDO_NON_NIHIL (pl);
        si (pl != NIHIL)
        {
            fseek(pl, 0L, SEEK_END);
            mensura_sys = ftell(pl);
            fseek(pl, 0L, SEEK_SET);
            fons_sys = (character*)piscina_allocare(piscina,
                (memoriae_index)(mensura_sys + 1L));
            si (fread(fons_sys, I, (memoriae_index)mensura_sys, pl)
                != (memoriae_index)mensura_sys)
            {
                fons_sys = NIHIL;
            }
            fclose(pl);
        }
        CREDO_NON_NIHIL (fons_sys);

        /* systema parsatur MUNDE: 0 errores, 0 ambigui (ordo
         * typorum consulto) */
        parsura_sys = silva_c89_parsare(piscina, "systema_c89.h",
            fons_sys, (i32)mensura_sys, NIHIL);
        CREDO_NON_NIHIL (parsura_sys);
        CREDO_AEQUALIS_I32 (parsura_sys->numerus_errorum, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (xar_numerus(
            parsura_sys->commissio->ambigui), ZEPHYRUM);

        /* harvest: size_t TYPEDEF, malloc FUNCTIO, ex_systemate */
        sem_sys = silva_c89_semantica_analysare(piscina, parsura_sys);
        CREDO_NON_NIHIL (sem_sys);
        CREDO_AEQUALIS_I32 (xar_numerus(sem_sys->diagnostica),
            ZEPHYRUM);
        {
            SemanticaSymbolum* symbolum;

            symbolum = silva_c89_symbolum_invenire(sem_sys,
                _ch("size_t"));
            CREDO_NON_NIHIL (symbolum);
            CREDO_AEQUALIS_S32 (symbolum->genus,
                (s32)SYMBOLUM_TYPEDEF);
            CREDO_AEQUALIS_PTR (symbolum->typus,
                silva_c89_typus_primitivum(sem_sys,
                    PRIMITIVUM_LONGUS_INSIGNATUM));

            symbolum = silva_c89_symbolum_invenire(sem_sys,
                _ch("malloc"));
            CREDO_NON_NIHIL (symbolum);
            CREDO_AEQUALIS_S32 (symbolum->genus,
                (s32)SYMBOLUM_FUNCTIO);
        }

        /* praeoneratio per augere: (size_t)(x) collapsus */
        {
            SilvaOraculum* oraculum = silva_oraculum_creare(piscina);
            SilvaParsura* parsura;
            constans character* fons_v = "(size_t)(x);\n";

            CREDO_VERUM (silva_c89_semantica_oraculum_augere(
                sem_sys, oraculum) > (i32)C);
            parsura = silva_c89_parsare(piscina, "probatio.c",
                fons_v, (i32)strlen(fons_v), oraculum);
            CREDO_NON_NIHIL (parsura);
            CREDO_AEQUALIS_I32 (xar_numerus(
                parsura->commissio->ambigui), ZEPHYRUM);
        }

        /* analysis cum systemate: size_t/FILE noti -> diagnostica 0,
         * typus per systema resolutus. NB: FILE* f est furca classica
         * (foo * bar) - parsura ORACULO PRAEONERATO indiget (canalis
         * praeonerationis!); size_t n superstes unicus etiam sine */
        {
            constans character* fons_v = "size_t n;\nFILE* f;\n";
            SilvaOraculum* oraculum = silva_oraculum_creare(piscina);
            SilvaParsura* parsura;
            SilvaSemantica* sem;
            SemanticaSymbolum* symbolum;

            (vacuum)silva_c89_semantica_oraculum_augere(sem_sys,
                oraculum);
            parsura = silva_c89_parsare(piscina, "probatio.c",
                fons_v, (i32)strlen(fons_v), oraculum);
            CREDO_NON_NIHIL (parsura);
            CREDO_AEQUALIS_I32 (xar_numerus(
                parsura->commissio->ambigui), ZEPHYRUM);
            sem = silva_c89_semantica_analysare_cum_systemate(
                piscina, parsura, parsura_sys);
            CREDO_NON_NIHIL (sem);
            CREDO_AEQUALIS_I32 (xar_numerus(sem->diagnostica),
                ZEPHYRUM);
            symbolum = silva_c89_symbolum_invenire(sem, _ch("n"));
            CREDO_NON_NIHIL (symbolum);
            si (symbolum != NIHIL)
            {
                CREDO_FALSUM (symbolum->ex_systemate);
                CREDO_AEQUALIS_PTR (symbolum->typus,
                    silva_c89_typus_primitivum(sem,
                        PRIMITIVUM_LONGUS_INSIGNATUM));
            }
            symbolum = silva_c89_symbolum_invenire(sem, _ch("f"));
            CREDO_NON_NIHIL (symbolum);
            si (symbolum != NIHIL)
            {
                CREDO_AEQUALIS_S32 (symbolum->typus->genus,
                    (s32)TYPUS_C89_MONSTRATOR);
            }
        }

        /* clausura: non-typus localis (corpus - praecommissio eum
         * non registrat) -> retentum ad commissionem, versum per
         * recanonicare cum oraculo aucto */
        {
            constans character* fons_v =
                "void f(void)\n"
                "{\n"
                "    int localis[4];\n"
                "    int n = sizeof(localis);\n"
                "}\n";
            SilvaOraculum* oraculum = silva_oraculum_creare(piscina);
            SilvaParsura* parsura = silva_c89_parsare(piscina,
                "probatio.c", fons_v, (i32)strlen(fons_v), oraculum);
            SilvaSemantica* sem;

            CREDO_NON_NIHIL (parsura);
            CREDO_AEQUALIS_I32 (xar_numerus(
                parsura->commissio->ambigui), I);
            sem = silva_c89_semantica_analysare(piscina, parsura);
            CREDO_NON_NIHIL (sem);
            /* ante clausuram: indecisum (localis in corpore -
             * praecommissio eum non registrat) */
            CREDO_AEQUALIS_I32 (silva_c89_ambigua_indecisa_numerare(
                parsura->commissio, oraculum), I);
            (vacuum)silva_c89_semantica_oraculum_augere(sem,
                oraculum);
            silva_oraculum_responsa_vacare(oraculum);
            /* post: DECISUM (localis non-typus notus). NB versio
             * nulla - canonica expressionis iam recta erat
             * (decisum != versum; recanonicare solum versiones
             * numerat) */
            CREDO_AEQUALIS_I32 (silva_c89_ambigua_indecisa_numerare(
                parsura->commissio, oraculum), ZEPHYRUM);
            (vacuum)silva_recanonicare(parsura->commissio, oraculum,
                silva_c89_resolutor, NIHIL);
        }
    }

    /* ========================================================
     * PROBARE (Chunk C): catenae conversionum nidificatae - furca
     * intra furcam (formae GLR); contagio "omnes" sanata (lectio
     * exterior ob alternativam interiorem non occiditur), catena
     * PROFUNDA manet parca nominata (residuum I)
     * ======================================================== */
    {
        constans character* fons_v =
            "#define SCR_ELEM(basis, idx, m)"
            " ((const char*)(basis) + (idx) * (m))\n"
            "static void probare(const void *values,"
            " unsigned long magnitudo)\n"
            "{\n"
            "    unsigned long i;\n"
            "    for (i = 0; i < 4; i++) {\n"
            "        const void *elem = SCR_ELEM(values, i,"
            " magnitudo);\n"
            "        (void)elem;\n"
            "    }\n"
            "}\n";
        SilvaOraculum* oraculum = silva_oraculum_creare(piscina);
        SilvaParsura* parsura = silva_c89_parsare(piscina,
            "probatio.c", fons_v, (i32)strlen(fons_v), oraculum);
        SilvaSemantica* sem;

        imprimere("\n--- Probans catenas nidificatas ---\n");

        CREDO_NON_NIHIL (parsura);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (xar_numerus(
            parsura->commissio->ambigui), III);
        sem = silva_c89_semantica_analysare(piscina, parsura);
        CREDO_NON_NIHIL (sem);
        (vacuum)silva_c89_semantica_oraculum_augere(sem, oraculum);
        silva_oraculum_responsa_vacare(oraculum);
        /* II decisa (contagione sanata); I residuum = catena
         * profunda (parca resolutoris, vide worklog) */
        CREDO_AEQUALIS_I32 (silva_c89_ambigua_indecisa_numerare(
            parsura->commissio, oraculum), I);
    }

    /* ========================================================
     * PROBARE (M0a clausura): typedef scopi CORPORIS (decisiones
     * 13) - oraculum praecommissionis eum non registrat (retentio
     * ad commissionem), semantica scopis eum NOVIT -> clausura
     * DECIDIT (promissum consilii §II confirmatum numero)
     * ======================================================== */
    {
        constans character* fons_v =
            "void f(void)\n"
            "{\n"
            "    typedef int T;\n"
            "    T x;\n"
            "    (T)(x);\n"
            "}\n";
        SilvaOraculum* oraculum = silva_oraculum_creare(piscina);
        SilvaParsura* parsura = silva_c89_parsare(piscina,
            "probatio.c", fons_v, (i32)strlen(fons_v), oraculum);
        SilvaSemantica* sem;

        imprimere("\n--- Probans typedef corporis (dec. 13) ---\n");

        CREDO_NON_NIHIL (parsura);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
        /* retentum ad commissionem (oraculum T nescit - dec. 13) */
        CREDO_VERUM (xar_numerus(parsura->commissio->ambigui)
            >= I);
        sem = silva_c89_semantica_analysare(piscina, parsura);
        CREDO_NON_NIHIL (sem);
        (vacuum)silva_c89_semantica_oraculum_augere(sem, oraculum);
        silva_oraculum_responsa_vacare(oraculum);
        /* clausura decidit: typedef scopi corporis TYPUS notus */
        CREDO_AEQUALIS_I32 (silva_c89_ambigua_indecisa_numerare(
            parsura->commissio, oraculum), ZEPHYRUM);
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    redde I;
}
