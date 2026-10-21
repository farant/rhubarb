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

/* Status unci per uncus.datum traditus (non staticum: sectio suum
 * habet, ordine libera). */
nomen structura {
    MateriaToken* radix;
             Xar* extentum;
             b32  recusat;
} UncusFictus;

hic_manens MateriaToken*
_radix_ficta (
                          vacuum*  datum,
                    MateriaToken*  token,
              constans character** causa)
{
    constans UncusFictus* fictus = (constans UncusFictus*)datum;

    si (fictus->recusat)
    {
        *causa = "origo ficta recusat";
        redde NIHIL;
    }
    /* Lexema cuius offset negativus est 'derivatum' habetur */
    si (token->byte_offset < ZEPHYRUM)
    {
        redde fictus->radix;
    }
    redde token;
}

hic_manens Xar*
_extentum_fictum (
                   vacuum* datum,
    constans MateriaToken* radix)
{
    (vacuum)radix;
    redde ((constans UncusFictus*)datum)->extentum;
}


/* ==================================================
 * PROBARE: ambiguus OPTIONALIS (correctio I)
 * ================================================== */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        MateriaScripturaConsilium consilium;
                     MateriaNodus* n;
                 MateriaScriptura  s;

        n = materia_nodus_creare(piscina, ZEPHYRUM, (i32)I);
        CREDO_NON_NIHIL (n);

        /* ABSENS: acceptum. silva hic 'tabularium sine forma
         * ambigui' recusaret. */
        materia_scriptura_consilium_nudum(&consilium,
            &REG_SINE_AMBIGUO);
        s = materia_scribere_nodum(piscina, n, &consilium);
        CREDO_VERUM (s.successus);
        CREDO_NIHIL (s.causa);

        /* PRAESENS ET INTEGER: acceptum */
        materia_scriptura_consilium_nudum(&consilium, &REG_CUM_AMBIGUO);
        s = materia_scribere_nodum(piscina, n, &consilium);
        CREDO_VERUM (s.successus);

        /* PRAESENS SED FRACTUS: RECUSATUR - distinctio servata */
        materia_scriptura_consilium_nudum(&consilium,
            &REG_AMBIGUUS_FRACTUS);
        s = materia_scribere_nodum(piscina, n, &consilium);
        CREDO_FALSUM (s.successus);
        CREDO_NON_NIHIL (s.causa);

        /* argumenta nulla */
        s = materia_scribere_nodum(piscina, NIHIL, &consilium);
        CREDO_FALSUM (s.successus);
        materia_scriptura_consilium_nudum(&consilium, NIHIL);
        s = materia_scribere_nodum(piscina, n, &consilium);
        CREDO_FALSUM (s.successus);
    }
}


/* ==================================================
 * PROBARE: emissio structuralis simplex
 * ================================================== */

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        MateriaScripturaConsilium consilium;
                     MateriaNodus* n;
                     MateriaToken* a;
                     MateriaToken* b;
                 MateriaScriptura  s;

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

        materia_scriptura_consilium_nudum(&consilium,
            &REG_SINE_AMBIGUO);
        s = materia_scribere_nodum(piscina, n, &consilium);
        CREDO_VERUM (s.successus);
        CREDO_AEQUALIS_I32 (s.textus.mensura, (i32)6);
        CREDO_VERUM (memcmp(s.textus.datum, "int f;", (size_t)6)
            == ZEPHYRUM);

        /* genus ignotum: fractura clara, non praetermissio tacita */
        n->genus  = (s32)99;
        s         = materia_scribere_nodum(piscina, n, &consilium);
        CREDO_FALSUM (s.successus);
        CREDO_AEQUALIS_PTR ((constans vacuum*)s.sedes,
                            (constans vacuum*)n);
        CREDO_AEQUALIS_I32 (s.textus.mensura, ZEPHYRUM);
        n->genus = ZEPHYRUM;
    }
}


/* ==================================================
 * PROBARE: REINSERENDA EX VIA SUBARBORIS (MG1)
 * ================================================== */

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        MateriaScripturaConsilium consilium;
                     MateriaNodus* n;
                     MateriaToken* t_int;
                     MateriaToken* t_f;
                     MateriaToken* d1;
                              Xar* lamina;
                              Xar* reins;
              MateriaReinserendum* r;
                 MateriaScriptura  s;
               constans character* FONS_VERUS =
                   "#define X 1\nint f;\n";

        /* Arbor lexemata offset XII..XVIII tegit; linea #define
         * (offset 0..XI) a NULLO lexemate arboris nominatur -
         * exacte casus quem C89 fert, et quem silva ex hac via
         * reddere non potest. */
        n     = materia_nodus_creare(piscina, ZEPHYRUM, (i32)I);
        t_int = materia_token_creare(piscina, &FORMA, ZEPHYRUM,
            chorda_ex_literis("int", piscina), (s32)12, (i32)2, (i32)1,
            ZEPHYRUM);
        t_f   = materia_token_creare(piscina, &FORMA, ZEPHYRUM,
            chorda_ex_literis(" f;\n", piscina), (s32)15, (i32)2,
            (i32)4,
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
        materia_scriptura_consilium_nudum(&consilium,
            &REG_SINE_AMBIGUO);
        s = materia_scribere_nodum(piscina, n, &consilium);
        CREDO_VERUM (s.successus);
        CREDO_AEQUALIS_I32 (s.textus.mensura, (i32)7);

        /* CUM reinserendis: fons VERUS octetim */
        consilium.reinserenda = reins;
        s = materia_scribere_nodum(piscina, n, &consilium);
        CREDO_VERUM (s.successus);
        CREDO_AEQUALIS_I32 (s.textus.mensura, (i32)strlen(FONS_VERUS));
        CREDO_VERUM (memcmp(s.textus.datum, FONS_VERUS,
            strlen(FONS_VERUS)) == ZEPHYRUM);

        /* CAUDA (silva-migratio T9): reinserendum POST lexema ultimum
         * arboris ('#endif' custodiae capitis) nullum lexema sequens
         * habet quod id fundat. Sine reinserenda_finire tacite perit
         * - consulto pro subarbore (reinserenda ultra eam non sua
         * sunt); cum eo, scriptura plagulae TOTIUS id reddit. */
        {
            MateriaToken* d2;
                     Xar* lamina_finis;
            constans character* FONS_CAUDA =
                "#define X 1\nint f;\n#endif\n";

            d2 = materia_token_creare(piscina, &FORMA, ZEPHYRUM,
                chorda_ex_literis("#endif\n", piscina), (s32)19, (i32)3,
                (i32)I, ZEPHYRUM);
            lamina_finis = xar_creare(piscina,
                magnitudo(MateriaToken*));
            *(MateriaToken**)xar_addere(lamina_finis) = d2;
            r = (MateriaReinserendum*)xar_addere(reins);
            r->offset = (s32)19;
            r->lamina = lamina_finis;
            xar_ordinare(reins, materia_reinserenda_comparare);

            s = materia_scribere_nodum(piscina, n, &consilium);
            CREDO_VERUM (s.successus);
            CREDO_AEQUALIS_I32 (s.textus.mensura,
                (i32)strlen(FONS_VERUS));

            consilium.reinserenda_finire = VERUM;
            s = materia_scribere_nodum(piscina, n, &consilium);
            CREDO_VERUM (s.successus);
            CREDO_AEQUALIS_I32 (s.textus.mensura,
                (i32)strlen(FONS_CAUDA));
            CREDO_VERUM (memcmp(s.textus.datum, FONS_CAUDA,
                strlen(FONS_CAUDA)) == ZEPHYRUM);

            s = materia_scribere_valorem(piscina,
                materia_valor_nodus(n),
                &consilium);
            CREDO_AEQUALIS_I32 (s.textus.mensura,
                (i32)strlen(FONS_CAUDA));
        }
    }
}


/* ==================================================
 * PROBARE: uncus originis
 * ================================================== */

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        MateriaScripturaConsilium consilium;
                MateriaOrigoUncus uncus;
                      UncusFictus fictus;
                     MateriaNodus* n;
                     MateriaToken* derivatum;
                     MateriaToken* derivatum2;
                 MateriaScriptura  s;

        fictus.recusat   = FALSUM;
        fictus.extentum  = NIHIL;
        fictus.radix = materia_token_creare(piscina, &FORMA, ZEPHYRUM,
            chorda_ex_literis("SI", piscina), ZEPHYRUM, (i32)I, (i32)I,
            ZEPHYRUM);

        /* lexema 'derivatum': offset -I, ergo uncus RADIX_FICTAM dat */
        derivatum = materia_token_creare(piscina, &FORMA, ZEPHYRUM,
            chorda_ex_literis("if", piscina), (s32)-I, (i32)I, (i32)I,
            ZEPHYRUM);
        n = materia_nodus_creare(piscina, ZEPHYRUM, (i32)I);
        CREDO_VERUM (materia_nodus_appendere(piscina, n, ZEPHYRUM,
            materia_valor_token(derivatum), MATERIA_LOCUS_LISTA_MIXTA));

        uncus.datum              = &fictus;
        uncus.sedes_quaerere     = NIHIL;
        uncus.radix_quaerere     = _radix_ficta;
        uncus.extentum_quaerere  = NIHIL;

        materia_scriptura_consilium_nudum(&consilium,
            &REG_SINE_AMBIGUO);
        s = materia_scribere_nodum(piscina, n, &consilium);
        /* SINE unco: lexema se ipsum emittit */
        CREDO_VERUM (s.successus);
        CREDO_VERUM (memcmp(s.textus.datum, "if", (size_t)2)
            == ZEPHYRUM);

        /* CUM unco: RADIX emittitur, non lexema */
        consilium.origo = &uncus;
        s = materia_scribere_nodum(piscina, n, &consilium);
        CREDO_VERUM (s.successus);
        CREDO_AEQUALIS_I32 (s.textus.mensura, (i32)2);
        CREDO_VERUM (memcmp(s.textus.datum, "SI", (size_t)2)
            == ZEPHYRUM);

        /* DEDUPLICATIO: duo lexemata derivata, radix una - semel */
        derivatum2 = materia_token_creare(piscina, &FORMA, ZEPHYRUM,
            chorda_ex_literis("(", piscina), (s32)-I, (i32)I, (i32)I,
            ZEPHYRUM);
        CREDO_VERUM (materia_nodus_appendere(piscina, n, ZEPHYRUM,
            materia_valor_token(derivatum2),
            MATERIA_LOCUS_LISTA_MIXTA));
        s = materia_scribere_nodum(piscina, n, &consilium);
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
            fictus.extentum = xar_creare(piscina,
                magnitudo(MateriaToken*));
            *(MateriaToken**)xar_addere(fictus.extentum) = e1;
            *(MateriaToken**)xar_addere(fictus.extentum) = e2;
            uncus.extentum_quaerere = _extentum_fictum;

            s = materia_scribere_nodum(piscina, n, &consilium);
            CREDO_VERUM (s.successus);
            CREDO_AEQUALIS_I32 (s.textus.mensura, (i32)5);
            CREDO_VERUM (memcmp(s.textus.datum, "SI(x)", (size_t)5)
                == ZEPHYRUM);
            uncus.extentum_quaerere = NIHIL;
        }

        /* RECUSATIO unci = fractura cum CAUSA UNCI, non generica */
        fictus.recusat = VERUM;
        s = materia_scribere_nodum(piscina, n, &consilium);
        CREDO_FALSUM (s.successus);
        CREDO_NON_NIHIL (s.causa);
        CREDO_VERUM (strcmp(s.causa, "origo ficta recusat")
            == ZEPHYRUM);
        fictus.recusat = FALSUM;
    }
}


/* ==================================================
 * PROBARE: filtrum fontis
 * ================================================== */

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        MateriaScripturaConsilium consilium;
                     MateriaNodus* n;
                 MateriaScriptura  s;

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

        materia_scriptura_consilium_nudum(&consilium,
            &REG_SINE_AMBIGUO);
        /* -I = quaelibet plagula */
        s = materia_scribere_nodum(piscina, n, &consilium);
        CREDO_AEQUALIS_I32 (s.textus.mensura, (i32)4);

        /* fons 0 solum */
        consilium.fons_index = ZEPHYRUM;
        s = materia_scribere_nodum(piscina, n, &consilium);
        CREDO_AEQUALIS_I32 (s.textus.mensura, (i32)2);
        CREDO_VERUM (memcmp(s.textus.datum, "aa", (size_t)2)
            == ZEPHYRUM);

        /* fons 1 solum */
        consilium.fons_index = (s32)I;
        s = materia_scribere_nodum(piscina, n, &consilium);
        CREDO_AEQUALIS_I32 (s.textus.mensura, (i32)2);
        CREDO_VERUM (memcmp(s.textus.datum, "bb", (size_t)2)
            == ZEPHYRUM);
    }
}


/* ==================================================
 * PROBARE: trivia et uncus valoris
 * ================================================== */

interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        MateriaScripturaConsilium consilium;
                     MateriaNodus* n;
                     MateriaToken* t;
                     MateriaToken* tr[1];
                 MateriaScriptura  s;

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

        materia_scriptura_consilium_nudum(&consilium,
            &REG_SINE_AMBIGUO);
        s = materia_scribere_nodum(piscina, n, &consilium);
        CREDO_VERUM (s.successus);
        CREDO_AEQUALIS_I32 (s.textus.mensura, (i32)3);
        CREDO_VERUM (memcmp(s.textus.datum, "  x", (size_t)3)
            == ZEPHYRUM);
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "ambiguum optionalem", _sectio_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "emissionem structuralem", _sectio_secunda, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "reinserenda ex via subarboris (MG1)", _sectio_tertia, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "uncum originis", _sectio_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "filtrum fontis", _sectio_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "trivia", _sectio_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("materia_scribere", SECTIONES);
}
