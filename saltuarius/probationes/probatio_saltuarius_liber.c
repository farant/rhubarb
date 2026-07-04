/* probatio_saltuarius_liber.c - Filum apertum (Chunk B1)
 *
 * Textus INICITUR (discus non tangitur). Probantur: index linearum
 * (CRLF tonsum, cauda sine \n, plagula vacua), coloratio per
 * extensionem, cursor clausus (columna ad lineam breviorem),
 * runae utf8, volumina aptata, destructio.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "saltuarius_liber.h"
#include <stdio.h>
#include <string.h>

s32 principale (vacuum)
{
    b32      praeteritus;
    Piscina* piscina;
    SaltuariusNexus* nexus;

    piscina = piscina_generare_dynamicum("probatio_salt_liber",
        8388608);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    nexus = saltuarius_nexus_creare(piscina);
    CREDO_NON_NIHIL (nexus);

    /* ========================================================
     * PROBARE: index linearum + coloratio
     * ======================================================== */
    {
        SaltuariusLiber* liber;

        imprimere("\n--- Probans indicem linearum ---\n");

        liber = saltuarius_liber_aperire(piscina, nexus,
            chorda_ex_literis("probe.c", piscina),
            chorda_ex_literis("si x;\nduo\r\ntres", piscina));
        CREDO_NON_NIHIL (liber);
        CREDO_AEQUALIS_I32 ((i32)liber->numerus_linearum, III);
        CREDO_CHORDA_AEQUALIS_LITERIS (
            saltuarius_liber_linea(liber, ZEPHYRUM), "si x;");
        CREDO_CHORDA_AEQUALIS_LITERIS (
            saltuarius_liber_linea(liber, I), "duo");   /* \r tonsum */
        CREDO_CHORDA_AEQUALIS_LITERIS (
            saltuarius_liber_linea(liber, II), "tres");
        CREDO_AEQUALIS_I32 (
            saltuarius_liber_linea(liber, X).mensura, ZEPHYRUM);

        /* colorata (.c): classis existit, 'si' LATINA */
        CREDO_VERUM (liber->est_colorata);
        CREDO_NON_NIHIL (liber->classis);
        CREDO_AEQUALIS_I32 ((i32)liber->classis[ZEPHYRUM],
            (i32)SALT_CLASSIS_LATINA);

        saltuarius_liber_destruere(liber);
    }

    /* ========================================================
     * PROBARE: merus (.md) + plagula vacua + cauda cum \n
     * ======================================================== */
    {
        SaltuariusLiber* liber;

        imprimere("\n--- Probans merum et vacuum ---\n");

        liber = saltuarius_liber_aperire(piscina, nexus,
            chorda_ex_literis("legendum.md", piscina),
            chorda_ex_literis("titulus\n", piscina));
        CREDO_NON_NIHIL (liber);
        CREDO_FALSUM (liber->est_colorata);
        CREDO_AEQUALIS_PTR ((constans vacuum*)liber->classis,
            (constans vacuum*)NIHIL);
        /* "titulus\n" = II lineae (ultima vacua) */
        CREDO_AEQUALIS_I32 ((i32)liber->numerus_linearum, II);
        CREDO_AEQUALIS_I32 (
            saltuarius_liber_linea(liber, I).mensura, ZEPHYRUM);
        saltuarius_liber_destruere(liber);

        liber = saltuarius_liber_aperire(piscina, nexus,
            chorda_ex_literis("vacuum.c", piscina),
            chorda_ex_literis("", piscina));
        CREDO_NON_NIHIL (liber);
        CREDO_AEQUALIS_I32 ((i32)liber->numerus_linearum, I);
        CREDO_AEQUALIS_I32 (
            saltuarius_liber_linea(liber, ZEPHYRUM).mensura,
            ZEPHYRUM);
        CREDO_FALSUM (liber->est_colorata);   /* mensura 0 */
        saltuarius_liber_destruere(liber);
    }

    /* ========================================================
     * PROBARE: cursor clausus + runae utf8 + volumina
     * ======================================================== */
    {
        SaltuariusLiber* liber;

        imprimere("\n--- Probans cursorem et volumina ---\n");

        /* linea II brevis; linea III cum runa 2-octetorum */
        liber = saltuarius_liber_aperire(piscina, nexus,
            chorda_ex_literis("mensura.md", piscina),
            chorda_ex_literis("longissima\nab\nx\xCE\xB2y\n",
                piscina));
        CREDO_NON_NIHIL (liber);
        CREDO_AEQUALIS_I32 (
            saltuarius_liber_linea_runae(liber, ZEPHYRUM), X);
        CREDO_AEQUALIS_I32 (
            saltuarius_liber_linea_runae(liber, II), III);

        /* columna ad lineam breviorem clausa */
        saltuarius_liber_movere_col(liber, VIII);
        CREDO_AEQUALIS_S32 (liber->cursor_columna, VIII);
        saltuarius_liber_movere(liber, I);   /* "ab": runae II */
        CREDO_AEQUALIS_S32 (liber->cursor_linea, I);
        CREDO_AEQUALIS_S32 (liber->cursor_columna, II);

        /* clausurae */
        saltuarius_liber_movere(liber, -C);
        CREDO_AEQUALIS_S32 (liber->cursor_linea, ZEPHYRUM);
        saltuarius_liber_movere(liber, C);
        CREDO_AEQUALIS_S32 (liber->cursor_linea, III);
        saltuarius_liber_movere_col(liber, -C);
        CREDO_AEQUALIS_S32 (liber->cursor_columna, ZEPHYRUM);

        /* primum/ultimum */
        saltuarius_liber_primum(liber);
        CREDO_AEQUALIS_S32 (liber->cursor_linea, ZEPHYRUM);
        saltuarius_liber_ultimum(liber);
        CREDO_AEQUALIS_S32 (liber->cursor_linea, III);

        /* volumina: fenestra II alta, V lata */
        saltuarius_liber_aptare(liber, II, V);
        CREDO_AEQUALIS_S32 (liber->volumen_y, II);
        saltuarius_liber_primum(liber);
        saltuarius_liber_aptare(liber, II, V);
        CREDO_AEQUALIS_S32 (liber->volumen_y, ZEPHYRUM);
        saltuarius_liber_movere_col(liber, IX);   /* col IX */
        saltuarius_liber_aptare(liber, II, V);
        CREDO_AEQUALIS_S32 (liber->volumen_x, V);
        saltuarius_liber_movere_col(liber, -IX);
        saltuarius_liber_aptare(liber, II, V);
        CREDO_AEQUALIS_S32 (liber->volumen_x, ZEPHYRUM);

        saltuarius_liber_destruere(liber);
    }

    /* ========================================================
     * PROBARE: strata (C1) - parse, materializatio, OMISSUM
     * ======================================================== */
    {
        SaltuariusLiber* liber;
        constans character* FONS =
            "#define G(x) ((x)+(x))\n"
            "#if 0\nint a;\n#else\nint b;\n#endif\n"
            "G(2);\n";
        constans SaltuariusStratum* stratum;

        imprimere("\n--- Probans strata ---\n");

        /* fistula sine praebendis (fixum sine #include) */
        CREDO_AEQUALIS_S32 (
            saltuarius_nexus_silvam_parare(nexus, NIHIL, ZEPHYRUM),
            ZEPHYRUM);

        liber = saltuarius_liber_aperire(piscina, nexus,
            chorda_ex_literis("probe.c", piscina),
            chorda_ex_literis(FONS, piscina));
        CREDO_NON_NIHIL (liber);
        CREDO_NON_NIHIL (liber->parsura);
        CREDO_AEQUALIS_I32 ((i32)liber->numerus_stratorum, II);

        /* OMISSUM: "int a;" fuscum, "int b;" non */
        {
            constans character* a = strstr(FONS, "int a;");
            constans character* b = strstr(FONS, "int b;");

            CREDO_AEQUALIS_I32 (
                (i32)liber->classis[(s32)(a - FONS)],
                (i32)SALT_CLASSIS_OMISSUM);
            CREDO_AEQUALIS_I32 (
                (i32)liber->classis[(s32)(b - FONS)],
                (i32)SALT_CLASSIS_VERBUM);
        }

        /* stratum 1: materializatum, "((2)+(2))" in textu */
        saltuarius_liber_stratum_ponere(liber, I);
        CREDO_AEQUALIS_S32 (liber->stratum_currens, I);
        stratum = saltuarius_liber_stratum(liber, nexus);
        CREDO_VERUM (stratum->parata);
        CREDO_MAIOR_I32 (stratum->textus.mensura, ZEPHYRUM);
        {
            constans character* textus_literis = chorda_ut_cstr(
                stratum->textus, piscina);

            CREDO_VERUM (strstr(textus_literis, "((2)+(2))")
                != NIHIL);
            /* rami omissi in strato expanso absunt */
            CREDO_AEQUALIS_PTR (
                (constans vacuum*)strstr(textus_literis, "int a;"),
                (constans vacuum*)NIHIL);
        }
        CREDO_MAIOR_I32 ((i32)stratum->numerus_positionum,
            ZEPHYRUM);
        CREDO_VERUM (stratum->positiones[ZEPHYRUM].finis
            > stratum->positiones[ZEPHYRUM].initium);

        /* accessores stratum activum legunt */
        CREDO_MAIOR_I32 ((i32)stratum->numerus_linearum, ZEPHYRUM);
        saltuarius_liber_ultimum(liber);
        CREDO_AEQUALIS_S32 (liber->cursor_linea,
            (s32)stratum->numerus_linearum - I);

        /* retro ad stratum 0: linea prima ut scripta */
        saltuarius_liber_stratum_ponere(liber, ZEPHYRUM);
        saltuarius_liber_primum(liber);
        CREDO_CHORDA_AEQUALIS_LITERIS (
            saltuarius_liber_linea(liber, ZEPHYRUM),
            "#define G(x) ((x)+(x))");

        /* clausura strati */
        saltuarius_liber_stratum_ponere(liber, C);
        CREDO_AEQUALIS_S32 (liber->stratum_currens, I);
        saltuarius_liber_stratum_ponere(liber, -V);
        CREDO_AEQUALIS_S32 (liber->stratum_currens, ZEPHYRUM);

        saltuarius_liber_destruere(liber);
    }

    /* ========================================================
     * PROBARE: iunctio tuta - trivia expansione perdita spatium
     * unum recipiunt (constPiscina numquam!)
     * ======================================================== */
    {
        SaltuariusLiber* liber;
        constans SaltuariusStratum* stratum;

        imprimere("\n--- Probans iunctionem tutam ---\n");

        liber = saltuarius_liber_aperire(piscina, nexus,
            chorda_ex_literis("iunctio.c", piscina),
            chorda_ex_literis(
                "#define VACUUM void\n#define CONSTANS const\n"
                "CONSTANS int x;\nVACUUM f(VACUUM);\n", piscina));
        CREDO_NON_NIHIL (liber);
        CREDO_NON_NIHIL (liber->parsura);
        saltuarius_liber_stratum_ponere(liber, I);
        stratum = saltuarius_liber_stratum(liber, nexus);
        CREDO_VERUM (stratum->parata);
        {
            constans character* textus_literis = chorda_ut_cstr(
                stratum->textus, piscina);

            CREDO_VERUM (strstr(textus_literis, "const int")
                != NIHIL);
            CREDO_VERUM (strstr(textus_literis, "void f(void)")
                != NIHIL);
            CREDO_AEQUALIS_PTR (
                (constans vacuum*)strstr(textus_literis,
                    "constint"),
                (constans vacuum*)NIHIL);
            CREDO_AEQUALIS_PTR (
                (constans vacuum*)strstr(textus_literis, "voidf"),
                (constans vacuum*)NIHIL);
        }
        saltuarius_liber_destruere(liber);
    }

    /* ========================================================
     * PROBARE: degradatio - merus liber stratum unicum
     * ======================================================== */
    {
        SaltuariusLiber* liber;

        imprimere("\n--- Probans degradationem stratorum ---\n");

        liber = saltuarius_liber_aperire(piscina, nexus,
            chorda_ex_literis("notae.md", piscina),
            chorda_ex_literis("verba\n", piscina));
        CREDO_NON_NIHIL (liber);
        CREDO_AEQUALIS_PTR ((constans vacuum*)liber->parsura,
            (constans vacuum*)NIHIL);
        CREDO_AEQUALIS_I32 ((i32)liber->numerus_stratorum, I);
        saltuarius_liber_stratum_ponere(liber, III);
        CREDO_AEQUALIS_S32 (liber->stratum_currens, ZEPHYRUM);
        saltuarius_liber_destruere(liber);
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
