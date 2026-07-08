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
#include <string.h>

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

        /* enum: constantes 0, 5, 6 */
        CREDO_AEQUALIS_I32 ((i32)silva_c89_declarationem_tractare(
            sem, _nodus(parsura, 0)), ZEPHYRUM);
        {
            chorda titulus;
            unio { constans character* c; i8* m; } u;
            vacuum* sedes = NIHIL;

            u.c = "CAERULEUS";
            titulus.datum = u.m;
            titulus.mensura = IX;
            CREDO_VERUM (tabula_dispersa_invenire(sem->constantes,
                titulus, &sedes));
            CREDO_AEQUALIS_S64 (*(s64*)sedes, (s64)VI);
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

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    redde I;
}
