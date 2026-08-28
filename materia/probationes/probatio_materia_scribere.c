/* probatio_materia_scribere.c - Emissio ad octetos
 *
 * Duae res hic probantur quas silva probare NON potest:
 *   1. REINSERENDA EX VIA SUBARBORIS (MG1 editio I). silva
 *      _scriptor_parare 'reinserenda = NIHIL' duratum tenet, ergo
 *      octeti quos nulla lexema arboris nominat ex via subarboris
 *      reddi non possunt. Probatio ea reddit et contra fontem VERUM
 *      octetim confert.
 *   2. AMBIGUUS ABSENS ACCIPITUR (correctio I). silva registrum sine
 *      genere 'ambiguus' RECUSAT; hic absentia licet, sed genus
 *      FRACTUM adhuc recusatur - 'non habeo' contra 'fractum habeo'.
 */

#include "latina.h"
#include "credo.h"
#include "materia_scribere.h"
#include "materia_nodus.h"
#include "materia_token.h"
#include "piscina.h"
#include <stdio.h>
#include <string.h>

hic_manens constans MateriaTokenForma FORMA = { ZEPHYRUM };

/* ---- registra ---- */

hic_manens constans MateriaTabLocus LOCI_SIMPLEX[] = {
    { "liberi", (s32)MATERIA_LOCUS_LISTA_MIXTA }
};
hic_manens constans MateriaTabGenus GENERA_SIMPLEX[] = {
    { "radix", ZEPHYRUM, (i32)I }
};
hic_manens constans MateriaRegistrumCoctum REG_SINE_AMBIGUO = {
    GENERA_SIMPLEX, (i32)I, LOCI_SIMPLEX, (i32)I
};

hic_manens constans MateriaTabLocus LOCI_CUM[] = {
    { "liberi",           (s32)MATERIA_LOCUS_LISTA_MIXTA },
    { "interpretationes", (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "canonica",         (s32)MATERIA_LOCUS_INDEX }
};
hic_manens constans MateriaTabGenus GENERA_CUM[] = {
    { "radix",    ZEPHYRUM, (i32)I },
    { "ambiguus", (i32)I,   (i32)II }
};
hic_manens constans MateriaRegistrumCoctum REG_CUM_AMBIGUO = {
    GENERA_CUM, (i32)II, LOCI_CUM, (i32)III
};

/* ambiguus praesens sed LOCO 'canonica' carens */
hic_manens constans MateriaTabLocus LOCI_FRACTI[] = {
    { "liberi",           (s32)MATERIA_LOCUS_LISTA_MIXTA },
    { "interpretationes", (s32)MATERIA_LOCUS_LISTA_NODUS }
};
hic_manens constans MateriaTabGenus GENERA_FRACTA[] = {
    { "radix",    ZEPHYRUM, (i32)I },
    { "ambiguus", (i32)I,   (i32)I }
};
hic_manens constans MateriaRegistrumCoctum REG_AMBIGUUS_FRACTUS = {
    GENERA_FRACTA, (i32)II, LOCI_FRACTI, (i32)II
};

/* ---- unci ficti ---- */

hic_manens MateriaToken* RADIX_FICTA;
hic_manens Xar*          EXTENTUM_FICTUM;
hic_manens b32           RADIX_RECUSAT;

hic_manens MateriaToken*
_radix_ficta (vacuum* datum, MateriaToken* token,
              constans character** causa)
{
    (vacuum)datum;
    si (RADIX_RECUSAT)
    {
        *causa = "origo ficta recusat";
        redde NIHIL;
    }
    /* Lexema cuius offset negativus est 'derivatum' habetur */
    si (token->byte_offset < ZEPHYRUM)
    {
        redde RADIX_FICTA;
    }
    redde token;
}

hic_manens Xar*
_extentum_fictum (vacuum* datum, constans MateriaToken* radix)
{
    (vacuum)datum;
    (vacuum)radix;
    redde EXTENTUM_FICTUM;
}

s32 principale (vacuum)
{
        b32  praeteritus;
   Piscina*  piscina;

    piscina = piscina_generare_dynamicum("probatio_materia_scribere", 65536);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ========================================================
     * PROBARE: ambiguus OPTIONALIS (correctio I)
     * ======================================================== */
    {
        MateriaScripturaConsilium c;
        MateriaNodus*             n;
        MateriaScriptura          s;

        imprimere("\n--- Probans ambiguum optionalem ---\n");

        n = materia_nodus_creare(piscina, ZEPHYRUM, (i32)I);
        CREDO_NON_NIHIL (n);

        /* ABSENS: acceptum. silva hic 'tabularium sine forma
         * ambigui' recusaret. */
        materia_scriptura_consilium_nudum(&c, &REG_SINE_AMBIGUO);
        s = materia_scribere_nodum(piscina, n, &c);
        CREDO_VERUM (s.successus);
        CREDO_NIHIL (s.causa);

        /* PRAESENS ET INTEGER: acceptum */
        materia_scriptura_consilium_nudum(&c, &REG_CUM_AMBIGUO);
        s = materia_scribere_nodum(piscina, n, &c);
        CREDO_VERUM (s.successus);

        /* PRAESENS SED FRACTUS: RECUSATUR - distinctio servata */
        materia_scriptura_consilium_nudum(&c, &REG_AMBIGUUS_FRACTUS);
        s = materia_scribere_nodum(piscina, n, &c);
        CREDO_FALSUM (s.successus);
        CREDO_NON_NIHIL (s.causa);

        /* argumenta nulla */
        s = materia_scribere_nodum(piscina, NIHIL, &c);
        CREDO_FALSUM (s.successus);
        materia_scriptura_consilium_nudum(&c, NIHIL);
        s = materia_scribere_nodum(piscina, n, &c);
        CREDO_FALSUM (s.successus);
    }


    /* ========================================================
     * PROBARE: emissio structuralis simplex
     * ======================================================== */
    {
        MateriaScripturaConsilium c;
        MateriaNodus*             n;
        MateriaToken*             a;
        MateriaToken*             b;
        MateriaScriptura          s;

        imprimere("\n--- Probans emissionem structuralem ---\n");

        n = materia_nodus_creare(piscina, ZEPHYRUM, (i32)I);
        a = materia_token_creare(piscina, &FORMA, ZEPHYRUM,
            chorda_ex_literis("int", piscina), ZEPHYRUM, (i32)I, (i32)I,
            ZEPHYRUM);
        b = materia_token_creare(piscina, &FORMA, ZEPHYRUM,
            chorda_ex_literis(" f;", piscina), (s32)3, (i32)I, (i32)4,
            ZEPHYRUM);
        CREDO_VERUM (materia_nodus_appendere(piscina, n, ZEPHYRUM,
            materia_valor_token(a), MATERIA_LOCUS_LISTA_MIXTA));
        CREDO_VERUM (materia_nodus_appendere(piscina, n, ZEPHYRUM,
            materia_valor_token(b), MATERIA_LOCUS_LISTA_MIXTA));

        materia_scriptura_consilium_nudum(&c, &REG_SINE_AMBIGUO);
        s = materia_scribere_nodum(piscina, n, &c);
        CREDO_VERUM (s.successus);
        CREDO_AEQUALIS_I32 (s.textus.mensura, (i32)6);
        CREDO_VERUM (memcmp(s.textus.datum, "int f;", (size_t)6)
            == ZEPHYRUM);

        /* genus ignotum: fractura clara, non praetermissio tacita */
        n->genus = (s32)99;
        s = materia_scribere_nodum(piscina, n, &c);
        CREDO_FALSUM (s.successus);
        CREDO_AEQUALIS_PTR ((constans vacuum*)s.sedes,
                            (constans vacuum*)n);
        CREDO_AEQUALIS_I32 (s.textus.mensura, ZEPHYRUM);
        n->genus = ZEPHYRUM;
    }


    /* ========================================================
     * PROBARE: REINSERENDA EX VIA SUBARBORIS (MG1)
     * ======================================================== */
    {
        MateriaScripturaConsilium c;
        MateriaNodus*             n;
        MateriaToken*             t_int;
        MateriaToken*             t_f;
        MateriaToken*             d1;
        Xar*                      lamina;
        Xar*                      reins;
        MateriaReinserendum*      r;
        MateriaScriptura          s;
        constans character*       FONS_VERUS = "#define X 1\nint f;\n";

        imprimere("\n--- Probans reinserenda ex via subarboris (MG1) ---\n");

        /* Arbor lexemata offset XII..XVIII tegit; linea #define
         * (offset 0..XI) a NULLO lexemate arboris nominatur -
         * exacte casus quem C89 fert, et quem silva ex hac via
         * reddere non potest. */
        n     = materia_nodus_creare(piscina, ZEPHYRUM, (i32)I);
        t_int = materia_token_creare(piscina, &FORMA, ZEPHYRUM,
            chorda_ex_literis("int", piscina), (s32)12, (i32)2, (i32)1,
            ZEPHYRUM);
        t_f   = materia_token_creare(piscina, &FORMA, ZEPHYRUM,
            chorda_ex_literis(" f;\n", piscina), (s32)15, (i32)2, (i32)4,
            ZEPHYRUM);
        CREDO_VERUM (materia_nodus_appendere(piscina, n, ZEPHYRUM,
            materia_valor_token(t_int), MATERIA_LOCUS_LISTA_MIXTA));
        CREDO_VERUM (materia_nodus_appendere(piscina, n, ZEPHYRUM,
            materia_valor_token(t_f), MATERIA_LOCUS_LISTA_MIXTA));

        d1 = materia_token_creare(piscina, &FORMA, ZEPHYRUM,
            chorda_ex_literis("#define X 1\n", piscina), ZEPHYRUM,
            (i32)I, (i32)I, ZEPHYRUM);
        lamina = xar_creare(piscina, magnitudo(MateriaToken*));
        *(MateriaToken**)xar_addere(lamina) = d1;

        reins = xar_creare(piscina, magnitudo(MateriaReinserendum));
        r = (MateriaReinserendum*)xar_addere(reins);
        r->offset = ZEPHYRUM;
        r->lamina = lamina;
        xar_ordinare(reins, materia_reinserenda_comparare);

        /* SINE reinserendis: sola arbor - lacuna manet */
        materia_scriptura_consilium_nudum(&c, &REG_SINE_AMBIGUO);
        s = materia_scribere_nodum(piscina, n, &c);
        CREDO_VERUM (s.successus);
        CREDO_AEQUALIS_I32 (s.textus.mensura, (i32)7);

        /* CUM reinserendis: fons VERUS octetim */
        c.reinserenda = reins;
        s = materia_scribere_nodum(piscina, n, &c);
        CREDO_VERUM (s.successus);
        CREDO_AEQUALIS_I32 (s.textus.mensura, (i32)strlen(FONS_VERUS));
        CREDO_VERUM (memcmp(s.textus.datum, FONS_VERUS,
            strlen(FONS_VERUS)) == ZEPHYRUM);
    }


    /* ========================================================
     * PROBARE: uncus originis
     * ======================================================== */
    {
        MateriaScripturaConsilium c;
        MateriaOrigoUncus         uncus;
        MateriaNodus*             n;
        MateriaToken*             derivatum;
        MateriaToken*             derivatum2;
        MateriaScriptura          s;

        imprimere("\n--- Probans uncum originis ---\n");

        RADIX_RECUSAT   = FALSUM;
        EXTENTUM_FICTUM = NIHIL;
        RADIX_FICTA = materia_token_creare(piscina, &FORMA, ZEPHYRUM,
            chorda_ex_literis("SI", piscina), ZEPHYRUM, (i32)I, (i32)I,
            ZEPHYRUM);

        /* lexema 'derivatum': offset -I, ergo uncus RADIX_FICTAM dat */
        derivatum = materia_token_creare(piscina, &FORMA, ZEPHYRUM,
            chorda_ex_literis("if", piscina), (s32)-I, (i32)I, (i32)I,
            ZEPHYRUM);
        n = materia_nodus_creare(piscina, ZEPHYRUM, (i32)I);
        CREDO_VERUM (materia_nodus_appendere(piscina, n, ZEPHYRUM,
            materia_valor_token(derivatum), MATERIA_LOCUS_LISTA_MIXTA));

        uncus.datum             = NIHIL;
        uncus.sedes_quaerere    = NIHIL;
        uncus.radix_quaerere    = _radix_ficta;
        uncus.extentum_quaerere = NIHIL;

        materia_scriptura_consilium_nudum(&c, &REG_SINE_AMBIGUO);
        s = materia_scribere_nodum(piscina, n, &c);
        /* SINE unco: lexema se ipsum emittit */
        CREDO_VERUM (s.successus);
        CREDO_VERUM (memcmp(s.textus.datum, "if", (size_t)2) == ZEPHYRUM);

        /* CUM unco: RADIX emittitur, non lexema */
        c.origo = &uncus;
        s = materia_scribere_nodum(piscina, n, &c);
        CREDO_VERUM (s.successus);
        CREDO_AEQUALIS_I32 (s.textus.mensura, (i32)2);
        CREDO_VERUM (memcmp(s.textus.datum, "SI", (size_t)2) == ZEPHYRUM);

        /* DEDUPLICATIO: duo lexemata derivata, radix una - semel */
        derivatum2 = materia_token_creare(piscina, &FORMA, ZEPHYRUM,
            chorda_ex_literis("(", piscina), (s32)-I, (i32)I, (i32)I,
            ZEPHYRUM);
        CREDO_VERUM (materia_nodus_appendere(piscina, n, ZEPHYRUM,
            materia_valor_token(derivatum2), MATERIA_LOCUS_LISTA_MIXTA));
        s = materia_scribere_nodum(piscina, n, &c);
        CREDO_VERUM (s.successus);
        CREDO_AEQUALIS_I32 (s.textus.mensura, (i32)2);   /* non IV */

        /* EXTENTUM: radix plus quam se tegit */
        {
            MateriaToken* e1;
            MateriaToken* e2;

            e1 = materia_token_creare(piscina, &FORMA, ZEPHYRUM,
                chorda_ex_literis("SI", piscina), ZEPHYRUM, (i32)I,
                (i32)I, ZEPHYRUM);
            e2 = materia_token_creare(piscina, &FORMA, ZEPHYRUM,
                chorda_ex_literis("(x)", piscina), (s32)2, (i32)I,
                (i32)3, ZEPHYRUM);
            EXTENTUM_FICTUM = xar_creare(piscina, magnitudo(MateriaToken*));
            *(MateriaToken**)xar_addere(EXTENTUM_FICTUM) = e1;
            *(MateriaToken**)xar_addere(EXTENTUM_FICTUM) = e2;
            uncus.extentum_quaerere = _extentum_fictum;

            s = materia_scribere_nodum(piscina, n, &c);
            CREDO_VERUM (s.successus);
            CREDO_AEQUALIS_I32 (s.textus.mensura, (i32)5);
            CREDO_VERUM (memcmp(s.textus.datum, "SI(x)", (size_t)5)
                == ZEPHYRUM);
            uncus.extentum_quaerere = NIHIL;
        }

        /* RECUSATIO unci = fractura cum CAUSA UNCI, non generica */
        RADIX_RECUSAT = VERUM;
        s = materia_scribere_nodum(piscina, n, &c);
        CREDO_FALSUM (s.successus);
        CREDO_NON_NIHIL (s.causa);
        CREDO_VERUM (strcmp(s.causa, "origo ficta recusat") == ZEPHYRUM);
        RADIX_RECUSAT = FALSUM;
    }


    /* ========================================================
     * PROBARE: filtrum fontis
     * ======================================================== */
    {
        MateriaScripturaConsilium c;
        MateriaNodus*             n;
        MateriaScriptura          s;

        imprimere("\n--- Probans filtrum fontis ---\n");

        n = materia_nodus_creare(piscina, ZEPHYRUM, (i32)I);
        CREDO_VERUM (materia_nodus_appendere(piscina, n, ZEPHYRUM,
            materia_valor_token(materia_token_creare(piscina, &FORMA,
                ZEPHYRUM, chorda_ex_literis("aa", piscina), ZEPHYRUM,
                (i32)I, (i32)I, ZEPHYRUM)),
            MATERIA_LOCUS_LISTA_MIXTA));
        CREDO_VERUM (materia_nodus_appendere(piscina, n, ZEPHYRUM,
            materia_valor_token(materia_token_creare(piscina, &FORMA,
                ZEPHYRUM, chorda_ex_literis("bb", piscina), ZEPHYRUM,
                (i32)I, (i32)I, (s32)I)),
            MATERIA_LOCUS_LISTA_MIXTA));

        materia_scriptura_consilium_nudum(&c, &REG_SINE_AMBIGUO);
        /* -I = quaelibet plagula */
        s = materia_scribere_nodum(piscina, n, &c);
        CREDO_AEQUALIS_I32 (s.textus.mensura, (i32)4);

        /* fons 0 solum */
        c.fons_index = ZEPHYRUM;
        s = materia_scribere_nodum(piscina, n, &c);
        CREDO_AEQUALIS_I32 (s.textus.mensura, (i32)2);
        CREDO_VERUM (memcmp(s.textus.datum, "aa", (size_t)2) == ZEPHYRUM);

        /* fons 1 solum */
        c.fons_index = (s32)I;
        s = materia_scribere_nodum(piscina, n, &c);
        CREDO_AEQUALIS_I32 (s.textus.mensura, (i32)2);
        CREDO_VERUM (memcmp(s.textus.datum, "bb", (size_t)2) == ZEPHYRUM);
    }


    /* ========================================================
     * PROBARE: trivia et uncus valoris
     * ======================================================== */
    {
        MateriaScripturaConsilium c;
        MateriaNodus*             n;
        MateriaToken*             t;
        MateriaToken*             tr[1];
        MateriaScriptura          s;

        imprimere("\n--- Probans trivia ---\n");

        t = materia_token_creare(piscina, &FORMA, ZEPHYRUM,
            chorda_ex_literis("x", piscina), (s32)2, (i32)I, (i32)3,
            ZEPHYRUM);
        tr[0] = materia_token_creare(piscina, &FORMA, ZEPHYRUM,
            chorda_ex_literis("  ", piscina), ZEPHYRUM, (i32)I, (i32)I,
            ZEPHYRUM);
        CREDO_VERUM (materia_token_trivia_ante_ponere(t, piscina, tr,
            (i32)I));

        n = materia_nodus_creare(piscina, ZEPHYRUM, (i32)I);
        CREDO_VERUM (materia_nodus_appendere(piscina, n, ZEPHYRUM,
            materia_valor_token(t), MATERIA_LOCUS_LISTA_MIXTA));

        materia_scriptura_consilium_nudum(&c, &REG_SINE_AMBIGUO);
        s = materia_scribere_nodum(piscina, n, &c);
        CREDO_VERUM (s.successus);
        CREDO_AEQUALIS_I32 (s.textus.mensura, (i32)3);
        CREDO_VERUM (memcmp(s.textus.datum, "  x", (size_t)3) == ZEPHYRUM);
    }


    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}
