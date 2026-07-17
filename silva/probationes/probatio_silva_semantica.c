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

/* Ordines praeter inutilia (69/70, revisio 2026-07-17): fontes
 * probationum = scaffolding typationis, variabiles consulto numquam
 * lectae - asserta numerorum sensum TYPATIONIS servant, non censum
 * inutilium */
interior i32
_ordines_praeter_inutilia (constans SilvaSemantica* sem)
{
    i32 i;
    i32 m = (i32)silva_c89_diagnostica_numerus(sem);
    i32 n = ZEPHYRUM;

    per (i = ZEPHYRUM; i < m; i++)
    {
        constans SemanticaDiagnosticum* d =
            silva_c89_diagnosticum_per_indicem(sem, i);

        si (d != NIHIL
            && d->codex != (s32)EXAMEN_CODEX_VARIABILIS_INUTILIS
            && d->codex != (s32)EXAMEN_CODEX_PARAMETRUM_INUTILE)
        {
            n++;
        }
    }
    redde n;
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

/* M0b: initiator declarationis (primus declarator initiatus) */
interior constans SilvaNodus*
_initiator_declarationis (constans SilvaNodus* declaratio)
{
    SilvaValor declaratores;
    i32 i;
    i32 m;

    si (declaratio == NIHIL
        || declaratio->genus != (s32)SILVA_C89_GENUS_DECLARATIO)
    {
        redde NIHIL;
    }
    declaratores = silva_c89_declaratio_declaratores(declaratio);
    m = (i32)silva_valor_lista_numerus(declaratores);
    per (i = ZEPHYRUM; i < m; i++)
    {
        SilvaValor* dv = silva_valor_lista_obtinere(declaratores, i);

        si (dv != NIHIL && dv->genus == SILVA_VALOR_NODUS
            && dv->datum.nodus->genus
                == (s32)SILVA_C89_GENUS_DECLARATOR_INITIATUS)
        {
            SilvaValor init_v =
                silva_c89_declarator_initiatus_initiator(
                    dv->datum.nodus);

            si (init_v.genus == SILVA_VALOR_NODUS)
            {
                redde init_v.datum.nodus;
            }
        }
    }
    redde NIHIL;
}

/* M0b: elementum corporis functionis (definitio in indice fili) */
interior constans SilvaNodus*
_elementum_corporis (constans SilvaParsura* parsura,
    i32 index_definitionis, i32 index_elementi)
{
    constans SilvaNodus* def = _nodus(parsura, index_definitionis);
    SilvaValor corpus_v;
    SilvaValor elementa;
    SilvaValor* e;

    si (def == NIHIL
        || def->genus != (s32)SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS)
    {
        redde NIHIL;
    }
    corpus_v = silva_c89_definitio_functionis_corpus(def);
    si (corpus_v.genus != SILVA_VALOR_NODUS)
    {
        redde NIHIL;
    }
    elementa = silva_c89_corpus_elementa(corpus_v.datum.nodus);
    e = silva_valor_lista_obtinere(elementa, index_elementi);
    si (e == NIHIL || e->genus != SILVA_VALOR_NODUS)
    {
        redde NIHIL;
    }
    redde e->datum.nodus;
}

/* M0b C: argumentum k-um vocationis (nodi soli, virgulae saltatae) */
interior constans SilvaNodus*
_argumentum_vocationis (constans SilvaNodus* vocatio, i32 k)
{
    SilvaValor argumenta;
    i32 i;
    i32 m;
    i32 a = ZEPHYRUM;

    si (vocatio == NIHIL
        || vocatio->genus != (s32)SILVA_C89_GENUS_VOCATIO)
    {
        redde NIHIL;
    }
    argumenta = silva_c89_vocatio_argumenta(vocatio);
    m = (i32)silva_valor_lista_numerus(argumenta);
    per (i = ZEPHYRUM; i < m; i++)
    {
        SilvaValor* v = silva_valor_lista_obtinere(argumenta, i);

        si (v != NIHIL && v->genus == SILVA_VALOR_NODUS)
        {
            si (a == k)
            {
                redde v->datum.nodus;
            }
            a++;
        }
    }
    redde NIHIL;
}

/* M0b C: symbolum per nomen in INDICE (scopi post analysin clausi -
 * implicita in scopo corporis registrata sunt) */
interior SemanticaSymbolum*
_symbolum_indicis (constans SilvaSemantica* sem,
    constans character* titulus)
{
    i32 i;
    i32 m = silva_c89_symbola_numerus(sem);
    chorda quaesitum = _ch(titulus);

    per (i = ZEPHYRUM; i < m; i++)
    {
        constans SemanticaSymbolum* symbolum =
            silva_c89_symbolum_per_indicem(sem, i);

        si (symbolum != NIHIL
            && symbolum->titulus.mensura == quaesitum.mensura
            && memcmp(symbolum->titulus.datum, quaesitum.datum,
                   (memoriae_index)quaesitum.mensura) == ZEPHYRUM)
        {
            unio { constans SemanticaSymbolum* c;
                   SemanticaSymbolum* m; } u;

            u.c = symbolum;
            redde u.m;
        }
    }
    redde NIHIL;
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
     * PROBARE: redditor typorum - baptisma tag anonymi, acies,
     * anonyma sine typedef (inventum agitationis legati 2026-07-13:
     * hover in legatus_currere signaturam totam perdebat)
     * ======================================================== */
    {
        SilvaSemantica* sem = silva_c89_semantica_creare(piscina);
        SilvaParsura* parsura;
        TypusC89* t;
        character buffer[CXXVIII];

        imprimere("\n--- Probans redditorem typorum ---\n");

        parsura = _parsare(piscina,
            "typedef struct { int radix; } Configuratio;\n"
            "typedef struct Titulata { int a; } Titulata;\n"
            "void currere(const Configuratio* cfg);\n"
            "char textus[256];\n"
            "extern int series[];\n"
            "struct { int x; } innominata;\n");
        CREDO_NON_NIHIL (parsura);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);

        CREDO_AEQUALIS_I32 ((i32)silva_c89_declarationem_tractare(
            sem, _nodus(parsura, 0)), I);
        CREDO_AEQUALIS_I32 ((i32)silva_c89_declarationem_tractare(
            sem, _nodus(parsura, I)), I);

        /* baptisma: typedef nomen structurae anonymae dedit */
        t = silva_c89_typedef_invenire(sem, _ch("Configuratio"));
        CREDO_NON_NIHIL (t);
        CREDO_AEQUALIS_I32 ((i32)t->datum.tag.titulus.mensura, XII);
        CREDO_VERUM (memcmp(t->datum.tag.titulus.datum,
            "Configuratio", XII) == ZEPHYRUM);

        /* titulus proprius NON superscribitur */
        t = silva_c89_typedef_invenire(sem, _ch("Titulata"));
        CREDO_NON_NIHIL (t);
        CREDO_AEQUALIS_I32 ((i32)t->datum.tag.titulus.mensura, VIII);
        CREDO_VERUM (memcmp(t->datum.tag.titulus.datum,
            "Titulata", VIII) == ZEPHYRUM);

        /* signatura per typum baptizatum redditur (ante:
         * irreddibilis - ipse casus legatus_currere) */
        t = _typus_declarationis(sem, _nodus(parsura, II), NIHIL);
        CREDO_NON_NIHIL (t);
        CREDO_VERUM (silva_c89_typum_scribere(t, buffer,
            (insignatus integer)magnitudo(buffer)) > ZEPHYRUM);
        CREDO_VERUM (strcmp(buffer,
            "vacuum(constans structura Configuratio*)") == ZEPHYRUM);

        /* acies completa et incompleta (ante: irreddibiles) */
        t = _typus_declarationis(sem, _nodus(parsura, III), NIHIL);
        CREDO_VERUM (silva_c89_typum_scribere(t, buffer,
            (insignatus integer)magnitudo(buffer)) > ZEPHYRUM);
        CREDO_VERUM (strcmp(buffer, "character[256]") == ZEPHYRUM);
        t = _typus_declarationis(sem, _nodus(parsura, IV), NIHIL);
        CREDO_VERUM (silva_c89_typum_scribere(t, buffer,
            (insignatus integer)magnitudo(buffer)) > ZEPHYRUM);
        CREDO_VERUM (strcmp(buffer, "integer[]") == ZEPHYRUM);

        /* anonyma sine typedef: locum tenens, non defectus */
        t = _typus_declarationis(sem, _nodus(parsura, V), NIHIL);
        CREDO_VERUM (silva_c89_typum_scribere(t, buffer,
            (insignatus integer)magnitudo(buffer)) > ZEPHYRUM);
        CREDO_VERUM (strcmp(buffer,
            "structura <anonyma>") == ZEPHYRUM);
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
        CREDO_AEQUALIS_I32 (_ordines_praeter_inutilia(sem), ZEPHYRUM);

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
     * exterior ob alternativam interiorem non occiditur).
     * NOTA (sanatio catenarum 2026-07-10): residuum I = furca
     * MORTUA (lectiones AMBAE impossibiles - intra lectionem a
     * patre iam reiectam; extra spinam canonicam, typationi
     * invisibilis). Retentio honesta - victor eligi non potest.
     * Furcae VIVAE omnes deciduntur (exterior + interior)
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
        /* II vivae decisa; I residuum = furca mortua (extra
         * spinam - vide notam supra) */
        CREDO_AEQUALIS_I32 (silva_c89_ambigua_indecisa_numerare(
            parsura->commissio, oraculum), I);
    }

    /* ========================================================
     * PROBARE (sanatio catenarum 2026-07-10): conversio cum
     * operando parenthesato TYPUM ferente - "(u32)(sizeof(catena))"
     * ut VOCATIO parsata retinebatur (nominatus interior AMBAS
     * lectiones exteriores typo-positivas faciebat - unicitatis
     * regula (c) frangebatur). Regula trivalens DUALIS lectionem
     * vocationis occidit (u32 typus notus in positione
     * expressionis); fluxus victoris solius catenam solvit.
     * Forma corporis M2c: friatio.c:133, url.c:209 (19 suites)
     * ======================================================== */
    {
        constans character* fons_v =
            "typedef unsigned int u32;\n"
            "typedef unsigned char u8;\n"
            "typedef struct { char* datum; int mensura; } catena;\n"
            "static unsigned long probare(const char* buf, int i)\n"
            "{\n"
            "    unsigned long a = (u32)(sizeof(catena)) * 2;\n"
            "    u32 b = ((u32)((u8)buf[i * 4] & 0xFF) << 24);\n"
            "    return a + b;\n"
            "}\n";
        SilvaOraculum* oraculum = silva_oraculum_creare(piscina);
        SilvaParsura* parsura = silva_c89_parsare(piscina,
            "probatio.c", fons_v, (i32)strlen(fons_v), oraculum);
        SilvaSemantica* sem;

        imprimere("\n--- Probans conversionem parenthesatam"
            " (dualis) ---\n");

        CREDO_NON_NIHIL (parsura);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
        /* FORTIUS quam exspectatum: typedef ordine visibiles ->
         * regula dualis in ambulatione praecommissionis decidit ->
         * furcae TRANSMUTATAE (nodi definiti), ambigui NULLI
         * retenti (ante sanationem: retentio - uterque lectio
         * typo-positiva erat, unicitas (c) frangebatur) */
        CREDO_AEQUALIS_I32 (xar_numerus(
            parsura->commissio->ambigui), ZEPHYRUM);
        sem = silva_c89_semantica_analysare(piscina, parsura);
        CREDO_NON_NIHIL (sem);
        (vacuum)silva_c89_semantica_oraculum_augere(sem, oraculum);
        silva_oraculum_responsa_vacare(oraculum);
        CREDO_AEQUALIS_I32 (silva_c89_ambigua_indecisa_numerare(
            parsura->commissio, oraculum), ZEPHYRUM);
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

    /* ========================================================
     * PROBARE (M0b Chunk A): typatio litteralium + initiatores
     * ======================================================== */
    {
        constans character* fons_t =
            "int a = 5;\n"
            "unsigned int b = 10u;\n"
            "long c = 10L;\n"
            "unsigned int h = 0xffffffff;\n"
            "long big = 4294967296;\n"
            "double d = 1.5;\n"
            "float fl = 1.5f;\n"
            "int ch = 'a';\n"
            "char s[] = \"ab\" \"cd\";\n"
            "char e2[] = \"a\\x41\\102z\";\n";
        SilvaParsura* parsura = _parsare(piscina, fons_t);
        SilvaSemantica* sem;
        TypusC89* char_t;

        imprimere("\n--- Probans typationem litteralium (M0b A) ---\n");
        CREDO_NON_NIHIL (parsura);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
        sem = silva_c89_semantica_analysare(piscina, parsura);
        CREDO_NON_NIHIL (sem);
        CREDO_AEQUALIS_I32 (silva_c89_diagnostica_numerus(sem),
            ZEPHYRUM);
        char_t = silva_c89_typus_primitivum(sem, PRIMITIVUM_CHARACTER);

        CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem,
            _initiator_declarationis(_nodus(parsura, 0))),
            silva_c89_typus_primitivum(sem, PRIMITIVUM_INTEGER));
        CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem,
            _initiator_declarationis(_nodus(parsura, I))),
            silva_c89_typus_primitivum(sem,
                PRIMITIVUM_INTEGER_INSIGNATUM));
        CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem,
            _initiator_declarationis(_nodus(parsura, II))),
            silva_c89_typus_primitivum(sem, PRIMITIVUM_LONGUS));
        /* hex sine suffixo supra INT_MAX -> unsigned int (C89) */
        CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem,
            _initiator_declarationis(_nodus(parsura, III))),
            silva_c89_typus_primitivum(sem,
                PRIMITIVUM_INTEGER_INSIGNATUM));
        /* decimale supra INT_MAX -> long (LP64) */
        CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem,
            _initiator_declarationis(_nodus(parsura, IV))),
            silva_c89_typus_primitivum(sem, PRIMITIVUM_LONGUS));
        CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem,
            _initiator_declarationis(_nodus(parsura, V))),
            silva_c89_typus_primitivum(sem, PRIMITIVUM_DUPLEX));
        CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem,
            _initiator_declarationis(_nodus(parsura, VI))),
            silva_c89_typus_primitivum(sem, PRIMITIVUM_FLUITANS));
        /* litterale characteris = INT (C89 6.1.3.4) */
        CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem,
            _initiator_declarationis(_nodus(parsura, VII))),
            silva_c89_typus_primitivum(sem, PRIMITIVUM_INTEGER));
        /* chordae adiacentes: "ab" "cd" -> char[5] */
        CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem,
            _initiator_declarationis(_nodus(parsura, VIII))),
            silva_c89_typus_acies(sem, char_t, V));
        /* fugae: a \x41 \102 z -> char[5] */
        CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem,
            _initiator_declarationis(_nodus(parsura, IX))),
            silva_c89_typus_acies(sem, char_t, V));
        CREDO_VERUM (silva_c89_typationes_numerus(sem) >= X);
    }

    /* ========================================================
     * PROBARE (M0b Chunk A): identificatores + unarium +
     * postcrementum + conditio scalaris (lapsus)
     * ======================================================== */
    {
        constans character* fons_u =
            "int g;\n"
            "char gc;\n"
            "int* p;\n"
            "char buf[4];\n"
            "enum Color { RUBER = 3, VIRIDIS };\n"
            "int f(int par)\n"
            "{\n"
            "    int loc = g;\n"
            "    int* lp = &g;\n"
            "    int dr = *p;\n"
            "    int nn = !g;\n"
            "    int ng = -gc;\n"
            "    char pc = gc;\n"
            "    char po = gc++;\n"
            "    int en = RUBER;\n"
            "    while (buf) { }\n"
            "    return g;\n"
            "}\n";
        SilvaParsura* parsura = _parsare(piscina, fons_u);
        SilvaSemantica* sem;
        TypusC89* int_t;
        TypusC89* char_t;
        constans SilvaNodus* elem;

        imprimere("\n--- Probans typationem primariorum (M0b A) ---\n");
        CREDO_NON_NIHIL (parsura);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
        sem = silva_c89_semantica_analysare(piscina, parsura);
        CREDO_NON_NIHIL (sem);
        CREDO_AEQUALIS_I32 (_ordines_praeter_inutilia(sem),
            ZEPHYRUM);
        int_t = silva_c89_typus_primitivum(sem, PRIMITIVUM_INTEGER);
        char_t = silva_c89_typus_primitivum(sem, PRIMITIVUM_CHARACTER);

        /* g -> int */
        CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem,
            _initiator_declarationis(_elementum_corporis(parsura,
                V, 0))), int_t);
        /* &g -> int* */
        CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem,
            _initiator_declarationis(_elementum_corporis(parsura,
                V, I))), silva_c89_typus_monstrator(sem, int_t));
        /* *p -> int */
        CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem,
            _initiator_declarationis(_elementum_corporis(parsura,
                V, II))), int_t);
        /* !g -> int */
        CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem,
            _initiator_declarationis(_elementum_corporis(parsura,
                V, III))), int_t);
        /* -gc -> int (promotio integralis!) */
        CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem,
            _initiator_declarationis(_elementum_corporis(parsura,
                V, IV))), int_t);
        /* gc -> char (naturalis, sine promotione) */
        CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem,
            _initiator_declarationis(_elementum_corporis(parsura,
                V, V))), char_t);
        /* gc++ -> char (postcrementum typum operandi habet) */
        CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem,
            _initiator_declarationis(_elementum_corporis(parsura,
                V, VI))), char_t);
        /* RUBER -> int (constans enumeri) */
        CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem,
            _initiator_declarationis(_elementum_corporis(parsura,
                V, VII))), int_t);
        /* while (buf): naturalis char[4], conversus char* (lapsus) */
        elem = _elementum_corporis(parsura, V, VIII);
        CREDO_NON_NIHIL (elem);
        CREDO_AEQUALIS_I32 ((i32)elem->genus,
            (i32)SILVA_C89_GENUS_DUM);
        {
            SilvaValor cond_v = silva_c89_dum_conditio(elem);

            CREDO_VERUM (cond_v.genus == SILVA_VALOR_NODUS);
            CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem,
                cond_v.datum.nodus),
                silva_c89_typus_acies(sem, char_t, IV));
            CREDO_AEQUALIS_PTR (silva_c89_conversio_expressionis(sem,
                cond_v.datum.nodus),
                silva_c89_typus_monstrator(sem, char_t));
        }
        /* return g -> int */
        elem = _elementum_corporis(parsura, V, IX);
        CREDO_NON_NIHIL (elem);
        CREDO_AEQUALIS_I32 ((i32)elem->genus,
            (i32)SILVA_C89_GENUS_REDDE);
        {
            SilvaValor val_v = silva_c89_redde_valor(elem);

            CREDO_VERUM (val_v.genus == SILVA_VALOR_NODUS);
            CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem,
                val_v.datum.nodus), int_t);
        }
        CREDO_VERUM (silva_c89_typationes_numerus(sem) >= XII);
    }

    /* ========================================================
     * PROBARE (M0b Chunk A): venenum ignoti + fuga \x in
     * aestimatore (exparcatum)
     * ======================================================== */
    {
        constans character* fons_i =
            "enum E2 { LITTERA_A = '\\x41' };\n"
            "int f2(void)\n"
            "{\n"
            "    return incognitus;\n"
            "}\n";
        SilvaParsura* parsura = _parsare(piscina, fons_i);
        SilvaSemantica* sem;
        SemanticaSymbolum* symbolum;
        constans SilvaNodus* elem;

        imprimere("\n--- Probans venenum + fugam \\x (M0b A) ---\n");
        CREDO_NON_NIHIL (parsura);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
        sem = silva_c89_semantica_analysare(piscina, parsura);
        CREDO_NON_NIHIL (sem);

        /* aestimator per fugam hexadecimalem (exparcatum): A = 65 */
        symbolum = silva_c89_symbolum_invenire(sem, _ch("LITTERA_A"));
        CREDO_NON_NIHIL (symbolum);
        CREDO_AEQUALIS_I32 ((i32)symbolum->valor, 65);

        /* identificator ignotus -> venenum + diagnosticum */
        elem = _elementum_corporis(parsura, I, 0);
        CREDO_NON_NIHIL (elem);
        {
            SilvaValor val_v = silva_c89_redde_valor(elem);

            CREDO_VERUM (val_v.genus == SILVA_VALOR_NODUS);
            CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem,
                val_v.datum.nodus), sem->typus_erroris);
        }
        CREDO_VERUM (silva_c89_diagnostica_numerus(sem) >= I);
    }

    /* ========================================================
     * PROBARE (M0b Chunk B): UAC + operatores + conversiones
     * ======================================================== */
    {
        constans character* fons_b =
            "int i; long l; unsigned int ui; unsigned long ul;\n"
            "long long ll; char ch; float fp; double dp;\n"
            "int arr[4]; int* p; int* q; void* vp;\n"
            "long f(int c)\n"
            "{\n"
            "    long a = i + l;\n"
            "    unsigned long long b = ul + ll;\n"
            "    int c2 = ch + ch;\n"
            "    float f2 = fp + i;\n"
            "    double d2 = dp + fp;\n"
            "    long s = l << i;\n"
            "    int r = i < l;\n"
            "    int* p2 = arr + 1;\n"
            "    long df = p - q;\n"
            "    int* p3 = c ? p : 0;\n"
            "    void* v2 = c ? vp : p;\n"
            "    long t3 = c ? 1 : 2L;\n"
            "    ch = i;\n"
            "    ch += 1;\n"
            "    return i;\n"
            "}\n";
        SilvaParsura* parsura = _parsare(piscina, fons_b);
        SilvaSemantica* sem;
        TypusC89* int_t;
        TypusC89* long_t;
        TypusC89* char_t;
        constans SilvaNodus* init;
        constans SilvaNodus* elem;

        imprimere("\n--- Probans UAC + operatores (M0b B) ---\n");
        CREDO_NON_NIHIL (parsura);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
        sem = silva_c89_semantica_analysare(piscina, parsura);
        CREDO_NON_NIHIL (sem);
        /* revisio consulta (conversio signi 2026-07-16): fons_b
         * "ul + ll" conversionem signi VERAM continet (ll s64 ->
         * u64; clang eam quoque monet) - unus ordo DOMESTICUS
         * exspectatus, non zephyrum.
         * revisio altera (angustatio 2026-07-17): "ch = i" quoque
         * angustatio vera (int -> character; clang
         * -Wimplicit-int-conversion) - ordines II (praeter
         * inutilia scaffolding). */
        CREDO_AEQUALIS_I32 (_ordines_praeter_inutilia(sem), II);
        {
            constans SemanticaDiagnosticum* d =
                silva_c89_diagnosticum_per_indicem(sem, ZEPHYRUM);

            CREDO_NON_NIHIL (d);
            si (d != NIHIL)
            {
                CREDO_AEQUALIS_S32 (d->codex,
                    (s32)EXAMEN_CODEX_CONVERSIO_SIGNI);
            }
        }
        int_t = silva_c89_typus_primitivum(sem, PRIMITIVUM_INTEGER);
        long_t = silva_c89_typus_primitivum(sem, PRIMITIVUM_LONGUS);
        char_t = silva_c89_typus_primitivum(sem, PRIMITIVUM_CHARACTER);

        /* i + l -> long; i conversus long */
        init = _initiator_declarationis(_elementum_corporis(parsura,
            XII, 0));
        CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem, init),
            long_t);
        {
            SilvaValor sin_v = silva_c89_binarium_sinister(init);

            CREDO_VERUM (sin_v.genus == SILVA_VALOR_NODUS);
            CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem,
                sin_v.datum.nodus), int_t);
            CREDO_AEQUALIS_PTR (silva_c89_conversio_expressionis(sem,
                sin_v.datum.nodus), long_t);
        }
        /* ul + ll -> unsigned long long (angulus LP64!) */
        init = _initiator_declarationis(_elementum_corporis(parsura,
            XII, I));
        CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem, init),
            silva_c89_typus_primitivum(sem,
                PRIMITIVUM_LONGUS_LONGUS_INSIGNATUM));
        /* ch + ch -> int (promotiones) */
        init = _initiator_declarationis(_elementum_corporis(parsura,
            XII, II));
        CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem, init),
            int_t);
        /* fp + i -> float (scala fluitans C89) */
        init = _initiator_declarationis(_elementum_corporis(parsura,
            XII, III));
        CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem, init),
            silva_c89_typus_primitivum(sem, PRIMITIVUM_FLUITANS));
        /* dp + fp -> double */
        init = _initiator_declarationis(_elementum_corporis(parsura,
            XII, IV));
        CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem, init),
            silva_c89_typus_primitivum(sem, PRIMITIVUM_DUPLEX));
        /* l << i -> long (sinister promotus, NON UAC) */
        init = _initiator_declarationis(_elementum_corporis(parsura,
            XII, V));
        CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem, init),
            long_t);
        /* i < l -> int; i conversus long */
        init = _initiator_declarationis(_elementum_corporis(parsura,
            XII, VI));
        CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem, init),
            int_t);
        {
            SilvaValor sin_v = silva_c89_binarium_sinister(init);

            CREDO_AEQUALIS_PTR (silva_c89_conversio_expressionis(sem,
                sin_v.datum.nodus), long_t);
        }
        /* arr + 1 -> int*; arr conversus int* (lapsus) */
        init = _initiator_declarationis(_elementum_corporis(parsura,
            XII, VII));
        CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem, init),
            silva_c89_typus_monstrator(sem, int_t));
        {
            SilvaValor sin_v = silva_c89_binarium_sinister(init);

            CREDO_AEQUALIS_PTR (silva_c89_conversio_expressionis(sem,
                sin_v.datum.nodus),
                silva_c89_typus_monstrator(sem, int_t));
        }
        /* p - q -> long (ptrdiff LP64) */
        init = _initiator_declarationis(_elementum_corporis(parsura,
            XII, VIII));
        CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem, init),
            long_t);
        /* c ? p : 0 -> int*; 0 conversus int* (constans nulla) */
        init = _initiator_declarationis(_elementum_corporis(parsura,
            XII, IX));
        CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem, init),
            silva_c89_typus_monstrator(sem, int_t));
        {
            SilvaValor f_v = silva_c89_ternarius_falsum(init);

            CREDO_AEQUALIS_PTR (silva_c89_conversio_expressionis(sem,
                f_v.datum.nodus),
                silva_c89_typus_monstrator(sem, int_t));
        }
        /* c ? vp : p -> void* (compositum) */
        init = _initiator_declarationis(_elementum_corporis(parsura,
            XII, X));
        CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem, init),
            silva_c89_typus_monstrator(sem,
                silva_c89_typus_primitivum(sem, PRIMITIVUM_VACUUM)));
        /* c ? 1 : 2L -> long */
        init = _initiator_declarationis(_elementum_corporis(parsura,
            XII, XI));
        CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem, init),
            long_t);
        /* ch = i: valor char; i conversus char */
        elem = _elementum_corporis(parsura, XII, XII);
        CREDO_NON_NIHIL (elem);
        {
            SilvaValor expr_v =
                silva_c89_sententia_expressionis_expressio(elem);
            SilvaValor dex_v;

            CREDO_VERUM (expr_v.genus == SILVA_VALOR_NODUS);
            CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem,
                expr_v.datum.nodus), char_t);
            dex_v = silva_c89_assignatio_dexter(expr_v.datum.nodus);
            CREDO_AEQUALIS_PTR (silva_c89_conversio_expressionis(sem,
                dex_v.datum.nodus), char_t);
        }
        /* ch += 1: valor char; ch conversus int (op implicita) */
        elem = _elementum_corporis(parsura, XII, XIII);
        CREDO_NON_NIHIL (elem);
        {
            SilvaValor expr_v =
                silva_c89_sententia_expressionis_expressio(elem);
            SilvaValor sin_v;

            CREDO_VERUM (expr_v.genus == SILVA_VALOR_NODUS);
            CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem,
                expr_v.datum.nodus), char_t);
            sin_v = silva_c89_assignatio_sinister(expr_v.datum.nodus);
            CREDO_AEQUALIS_PTR (silva_c89_conversio_expressionis(sem,
                sin_v.datum.nodus), int_t);
        }
        /* return i: i conversus long (typus reditus f) */
        elem = _elementum_corporis(parsura, XII, XIV);
        CREDO_NON_NIHIL (elem);
        {
            SilvaValor val_v = silva_c89_redde_valor(elem);

            CREDO_VERUM (val_v.genus == SILVA_VALOR_NODUS);
            CREDO_AEQUALIS_PTR (silva_c89_conversio_expressionis(sem,
                val_v.datum.nodus), long_t);
        }
    }

    /* ========================================================
     * PROBARE (M0b Chunk C): vocatio + accessus + subscriptio +
     * magnitudo-expressionis (exparcata) + est_implicitum
     * ======================================================== */
    {
        constans character* fons_c =
            "struct Punctum { int x; char c; };\n"
            "struct Ignota;\n"
            "int arr[4];\n"
            "int decl(long x);\n"
            "int varia(char* f, ...);\n"
            "enum { SZ = sizeof(arr) };\n"
            "int f(struct Ignota* ig)\n"
            "{\n"
            "    struct Punctum p;\n"
            "    const struct Punctum* q = &p;\n"
            "    int a1 = p.x;\n"
            "    char a2 = q->c;\n"
            "    int s1 = arr[1];\n"
            "    int s2 = 1[arr];\n"
            "    int v1 = decl(2);\n"
            "    int v2 = ignotus_vocatus(3);\n"
            "    int v3 = varia(\"x\", 'a', 1.5f);\n"
            "    unsigned long z1 = sizeof(arr);\n"
            "    int malum = ig->x;\n"
            "    return v1;\n"
            "}\n";
        SilvaParsura* parsura = _parsare(piscina, fons_c);
        SilvaSemantica* sem;
        TypusC89* int_t;
        TypusC89* char_t;
        SemanticaSymbolum* symbolum;
        constans SilvaNodus* init;

        imprimere("\n--- Probans postfixa + implicitum (M0b C) ---\n");
        CREDO_NON_NIHIL (parsura);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
        sem = silva_c89_semantica_analysare(piscina, parsura);
        CREDO_NON_NIHIL (sem);
        /* DUO diagnostica: ig->x incompleta + vocatio implicita
         * ignotus_vocatus (materializatio chunk C, SUSPECTUM) -
         * praeter inutilia scaffolding */
        CREDO_AEQUALIS_I32 (_ordines_praeter_inutilia(sem), II);
        int_t = silva_c89_typus_primitivum(sem, PRIMITIVUM_INTEGER);
        char_t = silva_c89_typus_primitivum(sem, PRIMITIVUM_CHARACTER);

        /* sizeof(arr) in aestimatore: UNDECAYED = 16 (exparcatum) */
        symbolum = silva_c89_symbolum_invenire(sem, _ch("SZ"));
        CREDO_NON_NIHIL (symbolum);
        CREDO_AEQUALIS_I32 ((i32)symbolum->valor, XVI);

        /* p.x -> int */
        CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem,
            _initiator_declarationis(_elementum_corporis(parsura,
                VI, II))), int_t);
        /* q->c -> const char (quales basis propagantur) */
        CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem,
            _initiator_declarationis(_elementum_corporis(parsura,
                VI, III))),
            silva_c89_typus_qualificatus(sem, char_t,
                QUALIS_CONSTANS));
        /* arr[1] -> int; arr conversus int* */
        init = _initiator_declarationis(_elementum_corporis(parsura,
            VI, IV));
        CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem, init),
            int_t);
        {
            SilvaValor b_v = silva_c89_subscriptio_basis(init);

            CREDO_AEQUALIS_PTR (silva_c89_conversio_expressionis(sem,
                b_v.datum.nodus),
                silva_c89_typus_monstrator(sem, int_t));
        }
        /* 1[arr] -> int (commutativum C89) */
        CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem,
            _initiator_declarationis(_elementum_corporis(parsura,
                VI, V))), int_t);
        /* decl(2) -> int; argumentum 2 conversus long */
        init = _initiator_declarationis(_elementum_corporis(parsura,
            VI, VI));
        CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem, init),
            int_t);
        CREDO_AEQUALIS_PTR (silva_c89_conversio_expressionis(sem,
            _argumentum_vocationis(init, 0)),
            silva_c89_typus_primitivum(sem, PRIMITIVUM_LONGUS));
        /* ignotus_vocatus(3) -> int; EXTERN INT IMPLICITUM cum
         * est_implicitum + declarans = sedes vocationis */
        init = _initiator_declarationis(_elementum_corporis(parsura,
            VI, VII));
        CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem, init),
            int_t);
        symbolum = _symbolum_indicis(sem, "ignotus_vocatus");
        CREDO_NON_NIHIL (symbolum);
        CREDO_AEQUALIS_I32 ((i32)symbolum->genus,
            (i32)SYMBOLUM_FUNCTIO);
        CREDO_VERUM (symbolum->est_implicitum);
        CREDO_NON_NIHIL (symbolum->declarans);
        CREDO_AEQUALIS_I32 ((i32)symbolum->declarans->genus,
            (i32)SILVA_C89_GENUS_FOLIUM_IDENTIFICATOR);
        CREDO_VERUM (symbolum->typus != NIHIL
            && symbolum->typus->genus == TYPUS_C89_FUNCTIO
            && !symbolum->typus->datum.functio.est_prototypata);
        /* declarata NON implicita */
        symbolum = _symbolum_indicis(sem, "decl");
        CREDO_NON_NIHIL (symbolum);
        CREDO_VERUM (!symbolum->est_implicitum);
        /* varia("x", 'a', 1.5f): "x" conversus char*;
         * 1.5f conversus double (promotio variadica) */
        init = _initiator_declarationis(_elementum_corporis(parsura,
            VI, VIII));
        CREDO_AEQUALIS_PTR (silva_c89_conversio_expressionis(sem,
            _argumentum_vocationis(init, 0)),
            silva_c89_typus_monstrator(sem, char_t));
        CREDO_AEQUALIS_PTR (silva_c89_conversio_expressionis(sem,
            _argumentum_vocationis(init, II)),
            silva_c89_typus_primitivum(sem, PRIMITIVUM_DUPLEX));
        /* sizeof(arr) ut expressio -> size_t (unsigned long) */
        CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem,
            _initiator_declarationis(_elementum_corporis(parsura,
                VI, IX))),
            silva_c89_typus_primitivum(sem,
                PRIMITIVUM_LONGUS_INSIGNATUM));
        /* ig->x: incompleta -> venenum */
        CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem,
            _initiator_declarationis(_elementum_corporis(parsura,
                VI, X))), sem->typus_erroris);
    }

    /* ========================================================
     * PROBARE (M0b Chunk C): congeries - typus deorsum + elisio
     * ======================================================== */
    {
        constans character* fons_g =
            "int m2[2][2] = {{1,2},{3,4}};\n"
            "struct P2 { int x; char c; };\n"
            "struct P2 ps = {1, 'a'};\n"
            "int me[2] = {1, 2L};\n"
            "char cs[2][4] = {\"ab\", \"cd\"};\n"
            "int e2[2][2] = {1, 2, 3, 4};\n";
        SilvaParsura* parsura = _parsare(piscina, fons_g);
        SilvaSemantica* sem;
        TypusC89* int_t;
        TypusC89* char_t;
        constans SilvaNodus* init;

        imprimere("\n--- Probans congeriem (M0b C) ---\n");
        CREDO_NON_NIHIL (parsura);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
        sem = silva_c89_semantica_analysare(piscina, parsura);
        CREDO_NON_NIHIL (sem);
        int_t = silva_c89_typus_primitivum(sem, PRIMITIVUM_INTEGER);
        char_t = silva_c89_typus_primitivum(sem, PRIMITIVUM_CHARACTER);

        /* elisio: QUATTUOR elementa scalaria ubi int[2] exspectatur
         * (mensura corporis - parca nominata) */
        CREDO_AEQUALIS_I32 (silva_c89_diagnostica_numerus(sem), IV);

        /* m2 congeries -> int[2][2]; interior [0] -> int[2] */
        init = _initiator_declarationis(_nodus(parsura, 0));
        CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem, init),
            silva_c89_typus_acies(sem,
                silva_c89_typus_acies(sem, int_t, II), II));
        {
            SilvaValor elementa = silva_c89_congeries_elementa(init);
            SilvaValor* e0 = silva_valor_lista_obtinere(elementa, 0);

            CREDO_VERUM (e0 != NIHIL
                && e0->genus == SILVA_VALOR_NODUS);
            CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem,
                e0->datum.nodus),
                silva_c89_typus_acies(sem, int_t, II));
        }
        /* ps = {1, 'a'}: 'a' (int) conversus char (membrum) */
        init = _initiator_declarationis(_nodus(parsura, II));
        {
            SilvaValor elementa = silva_c89_congeries_elementa(init);
            i32 i;
            i32 m = (i32)silva_valor_lista_numerus(elementa);
            i32 a = ZEPHYRUM;
            constans SilvaNodus* secundum = NIHIL;

            per (i = ZEPHYRUM; i < m; i++)
            {
                SilvaValor* v = silva_valor_lista_obtinere(
                    elementa, i);

                si (v != NIHIL && v->genus == SILVA_VALOR_NODUS)
                {
                    si (a == I)
                    {
                        secundum = v->datum.nodus;
                    }
                    a++;
                }
            }
            CREDO_NON_NIHIL (secundum);
            CREDO_AEQUALIS_PTR (silva_c89_conversio_expressionis(sem,
                secundum), char_t);
        }
        /* me = {1, 2L}: 2L conversus int */
        init = _initiator_declarationis(_nodus(parsura, III));
        CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem, init),
            silva_c89_typus_acies(sem, int_t, II));
    }

    /* ========================================================
     * PROBARE (M0b Chunk D): FIXUM VERSIONIS - typatio relativa
     * canonicae (DECISUS 2): sem1 ante clausuram venenum videt,
     * sem2 post versionem typum verum (auditus M0b, inventum 2)
     * ======================================================== */
    {
        constans character* fons_v2 =
            "void f(void)\n"
            "{\n"
            "    typedef int T;\n"
            "    T x;\n"
            "    (T)(x);\n"
            "}\n";
        SilvaOraculum* oraculum = silva_oraculum_creare(piscina);
        SilvaParsura* parsura = silva_c89_parsare(piscina,
            "probatio.c", fons_v2, (i32)strlen(fons_v2), oraculum);
        SilvaSemantica* sem1;
        SilvaSemantica* sem2;
        constans SilvaNodus* expressio = NIHIL;

        imprimere("\n--- Probans fixum versionis (M0b D) ---\n");
        CREDO_NON_NIHIL (parsura);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
        /* furca (T)(x) retenta ad commissionem (dec. 13) */
        CREDO_VERUM (xar_numerus(parsura->commissio->ambigui) >= I);

        sem1 = silva_c89_semantica_analysare(piscina, parsura);
        CREDO_NON_NIHIL (sem1);
        /* expressio sententiae tertiae corporis: involucrum
         * ambiguum - quaestio canonicae-conscia */
        {
            constans SilvaNodus* elem = _elementum_corporis(parsura,
                0, II);
            SilvaValor expr_v;

            CREDO_NON_NIHIL (elem);
            CREDO_AEQUALIS_I32 ((i32)elem->genus,
                (i32)SILVA_C89_GENUS_SENTENTIA_EXPRESSIONIS);
            expr_v = silva_c89_sententia_expressionis_expressio(elem);
            CREDO_VERUM (expr_v.genus == SILVA_VALOR_NODUS);
            expressio = expr_v.datum.nodus;
        }
        /* ANTE versionem: canonica = vocatio T(x) - venenum
         * (typedef vocatum, intra ambiguum silens) */
        CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem1,
            expressio), sem1->typus_erroris);

        /* clausura: T typus notus -> canonica VERTITUR */
        (vacuum)silva_c89_semantica_oraculum_augere(sem1, oraculum);
        silva_oraculum_responsa_vacare(oraculum);
        CREDO_VERUM ((i32)silva_recanonicare(parsura->commissio,
            oraculum, silva_c89_resolutor, NIHIL) >= I);

        /* POST versionem: sem2 canonicam conversionis videt ->
         * (T)(x) typum int habet. FIXUM: sem1 != sem2. */
        sem2 = silva_c89_semantica_analysare(piscina, parsura);
        CREDO_NON_NIHIL (sem2);
        CREDO_AEQUALIS_PTR (silva_c89_typus_expressionis(sem2,
            expressio),
            silva_c89_typus_primitivum(sem2, PRIMITIVUM_INTEGER));
    }

    /* ========================================================
     * PROBARE (M1a Chunk A): exporta demissionis - nexus
     * symbolorum (sedes usus -> symbolum, etiam implicitus) +
     * octeti chordae decodati (fugae, fragmenta, L-parca)
     * ======================================================== */
    {
        constans character* fons_n =
            "int quadrare(int x) { int y = x + 1; return y; }\n"
            "int f(void) { int r = g(); return r; }\n"
            "char* s = \"a\\x41\\0b\" \"cd\";\n"
            "char* w = \"a\\qb\";\n";
        SilvaParsura* parsura = _parsare(piscina, fons_n);
        SilvaSemantica* sem;
        constans SilvaNodus* init;
        constans SemanticaSymbolum* usus;
        chorda octeti;

        imprimere("\n--- Probans exporta demissionis (M1a A) ---\n");
        CREDO_NON_NIHIL (parsura);
        CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
        sem = silva_c89_semantica_analysare(piscina, parsura);
        CREDO_NON_NIHIL (sem);
        /* fuga invalida (\q) in w + vocatio implicita g()
         * (materializatio chunk C, SUSPECTUM) */
        CREDO_AEQUALIS_I32 (silva_c89_diagnostica_numerus(sem), II);

        /* usus x in "x + 1" -> symbolum PARAMETRUM idem quod
         * index tenet (identitas monstratorum) */
        init = _initiator_declarationis(_elementum_corporis(parsura,
            ZEPHYRUM, ZEPHYRUM));
        CREDO_NON_NIHIL (init);
        {
            SilvaValor sv = silva_c89_binarium_sinister(init);

            CREDO_VERUM (sv.genus == SILVA_VALOR_NODUS);
            usus = silva_c89_symbolum_nodi(sem, sv.datum.nodus);
        }
        CREDO_NON_NIHIL (usus);
        CREDO_AEQUALIS_I32 ((i32)usus->genus,
            (i32)SYMBOLUM_PARAMETRUM);
        CREDO_VERUM (usus->profunditas >= I);
        CREDO_AEQUALIS_PTR (usus, _symbolum_indicis(sem, "x"));

        /* vocatus implicitus g() -> symbolum synthetizatum
         * nectitur quoque */
        init = _initiator_declarationis(_elementum_corporis(parsura,
            I, ZEPHYRUM));
        CREDO_NON_NIHIL (init);
        {
            SilvaValor fv = silva_c89_vocatio_functio(init);

            CREDO_VERUM (fv.genus == SILVA_VALOR_NODUS);
            usus = silva_c89_symbolum_nodi(sem, fv.datum.nodus);
        }
        CREDO_NON_NIHIL (usus);
        CREDO_VERUM (usus->est_implicitum);
        CREDO_AEQUALIS_PTR (usus, _symbolum_indicis(sem, "g"));
        CREDO_VERUM (silva_c89_nexus_numerus(sem) >= III);

        /* decodere: "a\x41\0b" "cd" -> sex octeti aA(nul)bcd -
         * fugae solutae, fragmenta coniuncta, nullus intus tutus */
        init = _initiator_declarationis(_nodus(parsura, II));
        CREDO_NON_NIHIL (init);
        CREDO_VERUM (silva_c89_chorda_decodere(piscina, init,
            &octeti));
        CREDO_AEQUALIS_I32 (octeti.mensura, VI);
        CREDO_VERUM (memcmp(octeti.datum, "aA\0bcd",
            (memoriae_index)VI) == ZEPHYRUM);

        /* fuga invalida (\q) -> FALSUM (L-chorda ante parsatorem
         * frangitur - parca profundior quam decodere) */
        init = _initiator_declarationis(_nodus(parsura, III));
        CREDO_NON_NIHIL (init);
        CREDO_VERUM (!silva_c89_chorda_decodere(piscina, init,
            &octeti));
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
