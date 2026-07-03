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

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
