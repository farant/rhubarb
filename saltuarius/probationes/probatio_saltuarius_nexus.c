/* probatio_saltuarius_nexus.c - Pons silvae (Chunk B1)
 *
 * Probantur: index latinorum (lexatus ex silva_latina_textus per
 * silvam ipsam), classificatio viarum (.c/.h), tabula
 * CLASSIS-PER-OCTETUM: verba C89 per genus silvae, latina per
 * indicem, COMMENTUM TRANS LINEAS (probatio spansionis - gratis ex
 * consilio per-octetum), directiva (# + verbum), chordae, numeri.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "saltuarius_nexus.h"
#include <stdio.h>
#include <string.h>

/* Offset literarum in textu (primus casus); -1 si absens */
interior s32
_ubi (constans character* textus, constans character* quaesitum)
{
    constans character* inventum = strstr(textus, quaesitum);

    si (inventum == NIHIL)
    {
        redde -I;
    }
    redde (s32)(inventum - textus);
}

s32 principale (vacuum)
{
    b32      praeteritus;
    Piscina* piscina;
    SaltuariusNexus* nexus;

    piscina = piscina_generare_dynamicum("probatio_salt_nexus",
        4194304);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    nexus = saltuarius_nexus_creare(piscina);
    CREDO_NON_NIHIL (nexus);

    /* ========================================================
     * PROBARE: index latinorum
     * ======================================================== */
    {
        imprimere("\n--- Probans indicem latinorum ---\n");
        imprimere("  nomina latina: %d\n",
            (int)nexus->numerus_latinorum);

        CREDO_MAIOR_I32 ((i32)nexus->numerus_latinorum, CC);
        CREDO_VERUM (saltuarius_nexus_est_latinum(nexus,
            chorda_ex_literis("si", piscina)));
        CREDO_VERUM (saltuarius_nexus_est_latinum(nexus,
            chorda_ex_literis("redde", piscina)));
        CREDO_VERUM (saltuarius_nexus_est_latinum(nexus,
            chorda_ex_literis("magnitudo", piscina)));
        CREDO_FALSUM (saltuarius_nexus_est_latinum(nexus,
            chorda_ex_literis("main", piscina)));
        CREDO_FALSUM (saltuarius_nexus_est_latinum(nexus,
            chorda_ex_literis("saltuarius", piscina)));
    }

    /* ========================================================
     * PROBARE: classificatio viarum
     * ======================================================== */
    {
        imprimere("\n--- Probans vias ---\n");

        CREDO_VERUM (saltuarius_nexus_est_fons_c(
            chorda_ex_literis("a/b.c", piscina)));
        CREDO_VERUM (saltuarius_nexus_est_fons_c(
            chorda_ex_literis("silva.h", piscina)));
        CREDO_FALSUM (saltuarius_nexus_est_fons_c(
            chorda_ex_literis("legendum.md", piscina)));
        CREDO_FALSUM (saltuarius_nexus_est_fons_c(
            chorda_ex_literis("sine_extensione", piscina)));
        CREDO_FALSUM (saltuarius_nexus_est_fons_c(
            chorda_ex_literis("d.c/intra", piscina)));
    }

    /* ========================================================
     * PROBARE: tabula classium
     * ======================================================== */
    {
        constans character* FONS =
            "si (x) redde 5; /* c1\nc2 */\n#define QQ 7\n"
            "char* s = \"ab\";\n";
        chorda textus = chorda_ex_literis(FONS, piscina);
        i8* classis = (i8*)piscina_allocare(piscina,
            (memoriae_index)textus.mensura);
        SilvaPiscina* arena = silva_piscina_generare_dynamicum(
            "probatio_nexus_silva", 1048576);

        imprimere("\n--- Probans tabulam classium ---\n");

        CREDO_NON_NIHIL (arena);
        saltuarius_nexus_classificare(nexus, arena, textus,
            classis);

        /* latina + planum */
        CREDO_AEQUALIS_I32 ((i32)classis[_ubi(FONS, "si")],
            (i32)SALT_CLASSIS_LATINA);
        CREDO_AEQUALIS_I32 ((i32)classis[_ubi(FONS, "redde")],
            (i32)SALT_CLASSIS_LATINA);
        CREDO_AEQUALIS_I32 ((i32)classis[_ubi(FONS, "x")],
            (i32)SALT_CLASSIS_PLANUM);

        /* numeri */
        CREDO_AEQUALIS_I32 ((i32)classis[_ubi(FONS, "5")],
            (i32)SALT_CLASSIS_NUMERUS);
        CREDO_AEQUALIS_I32 ((i32)classis[_ubi(FONS, "7")],
            (i32)SALT_CLASSIS_NUMERUS);

        /* COMMENTUM trans lineas: c1 ante \n, c2 post - spansio
         * gratis ex tabula per-octetum */
        CREDO_AEQUALIS_I32 ((i32)classis[_ubi(FONS, "c1")],
            (i32)SALT_CLASSIS_COMMENTUM);
        CREDO_AEQUALIS_I32 ((i32)classis[_ubi(FONS, "c2")],
            (i32)SALT_CLASSIS_COMMENTUM);

        /* directiva: # et 'define'; QQ NON directiva */
        CREDO_AEQUALIS_I32 ((i32)classis[_ubi(FONS, "#define")],
            (i32)SALT_CLASSIS_DIRECTIVA);
        CREDO_AEQUALIS_I32 ((i32)classis[_ubi(FONS, "define")],
            (i32)SALT_CLASSIS_DIRECTIVA);
        CREDO_AEQUALIS_I32 ((i32)classis[_ubi(FONS, "QQ")],
            (i32)SALT_CLASSIS_PLANUM);

        /* verbum C89 per genus silvae */
        CREDO_AEQUALIS_I32 ((i32)classis[_ubi(FONS, "char")],
            (i32)SALT_CLASSIS_VERBUM);

        /* chorda litteralis (quota inclusa) */
        CREDO_AEQUALIS_I32 ((i32)classis[_ubi(FONS, "\"ab\"")],
            (i32)SALT_CLASSIS_CHORDA);
        CREDO_AEQUALIS_I32 ((i32)classis[_ubi(FONS, "ab")],
            (i32)SALT_CLASSIS_CHORDA);

        silva_piscina_destruere(arena);
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
