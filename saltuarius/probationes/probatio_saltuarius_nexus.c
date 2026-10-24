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
_ubi (
    constans character* textus,
    constans character* quaesitum)
{
    constans character* inventum = strstr(textus, quaesitum);

    si (inventum == NIHIL)
    {
        redde -I;
    }
    redde (s32)(inventum - textus);
}


/* creatio nexus semel asserta (ut olim ante blocos); sectiones nexum
 * suum quaeque creant - status nexus inter sectiones non communis */
interior vacuum
_suitam_parare (
    CredoContextus* c)
{
    CREDO_NON_NIHIL (saltuarius_nexus_creare(c->piscina));
}


/* ==================================================
 * PROBARE: index latinorum
 * ================================================== */

interior vacuum
_latina_probare (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
    SaltuariusNexus* nexus;

    nexus = saltuarius_nexus_creare(piscina);
    si (nexus == NIHIL)
    {
        redde;   /* creatio in parare_suitae asserta */
    }
    {
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
}


/* ==================================================
 * PROBARE: classificatio viarum
 * ================================================== */

interior vacuum
_vias_probare (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;

    {
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
}


/* ==================================================
 * PROBARE: tabula classium
 * ================================================== */

interior vacuum
_classes_probare (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
    SaltuariusNexus* nexus;

    nexus = saltuarius_nexus_creare(piscina);
    si (nexus == NIHIL)
    {
        redde;   /* creatio in parare_suitae asserta */
    }
    {
        constans character* FONS =
            "si (x) redde 5; /* c1\nc2 */\n#define QQ 7\n"
            "char* s = \"ab\";\n";
        chorda  textus = chorda_ex_literis(FONS, piscina);
            i8* classis = (i8*)piscina_allocare(piscina,
                (memoriae_index)textus.mensura);
        SilvaPiscina* arena = silva_piscina_generare_dynamicum(
            "probatio_nexus_silva", 1048576);


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
}


/* ==================================================
 * PROBARE: fistula parsandi (C1) - praebenda iniecta, parse,
 * strata, inclusio, resolutio
 * ================================================== */

interior vacuum
_fistulam_probare (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
    SaltuariusNexus* nexus;

    nexus = saltuarius_nexus_creare(piscina);
    si (nexus == NIHIL)
    {
        redde;   /* creatio in parare_suitae asserta */
    }
    {
        SaltuariusCaput capita[I];
        SilvaPiscina* arena;
        SilvaParsura* parsura;


        capita[ZEPHYRUM].titulus = chorda_ex_literis("geminare.h",
            piscina);
        capita[ZEPHYRUM].textus = chorda_ex_literis(
            "#define G(x) ((x)+(x))\n", piscina);
        capita[ZEPHYRUM].via_absoluta = chorda_ex_literis(
            "/abs/geminare.h", piscina);
        CREDO_AEQUALIS_S32 (
            saltuarius_nexus_silvam_parare(nexus, capita, I), I);

        /* resolutio */
        CREDO_CHORDA_AEQUALIS_LITERIS (
            saltuarius_nexus_fons_resolvere(nexus,
                chorda_ex_literis("geminare.h", piscina)),
            "/abs/geminare.h");
        CREDO_AEQUALIS_I32 (
            saltuarius_nexus_fons_resolvere(nexus,
                chorda_ex_literis("ignotum.h", piscina)).mensura,
            ZEPHYRUM);

        /* parse: inclusio resoluta + strata retenta */
        arena = silva_piscina_generare_dynamicum("probatio_parse",
            4194304);
        CREDO_NON_NIHIL (arena);
        parsura = saltuarius_nexus_parsare(nexus, arena, "probe.c",
            chorda_ex_literis(
                "#include \"geminare.h\"\nG(2);\n", piscina));
        CREDO_NON_NIHIL (parsura);
        si (parsura != NIHIL)
        {
            SilvaInclusioVista vista;

            CREDO_VERUM (parsura->successus);
            CREDO_NON_NIHIL (parsura->strata);
            si (parsura->strata != NIHIL)
            {
                CREDO_MAIOR_I32 (
                    (i32)silva_xar_numerus(parsura->strata),
                    ZEPHYRUM);
            }
            /* inclusiones: geminare.h QUAERITUR per viam (contextus
             * proprias addere potest - imprimimus quae sint) */
            {
                i32 n_inc = (i32)silva_inclusiones_numerus(
                    parsura->expansio);
                b32 inventa = FALSUM;
                i32 j;

                per (j = ZEPHYRUM; j < n_inc; j++)
                {
                    si (   silva_inclusio_vista(parsura->expansio,
                        j, &vista) && vista.via != NIHIL)
                    {
                        imprimere("  inclusio[%d]: %.*s fons_ad=%d\n",
                            (int)j, (int)vista.via->mensura,
                            (constans character*)vista.via->datum,
                            (int)vista.fons_ad);
                        si (   vista.via->mensura == X
                            && memcmp(vista.via->datum,
                                "geminare.h", X) == ZEPHYRUM
                            && vista.fons_ad      >= ZEPHYRUM)
                        {
                            inventa = VERUM;
                        }
                    }
                }
                CREDO_VERUM (inventa);
            }
        }
        silva_piscina_destruere(arena);
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "index latinorum", _latina_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "classificatio viarum", _vias_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "tabula classium", _classes_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "fistula parsandi (C1)", _fistulam_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("saltuarius_nexus", SECTIONES,
        _suitam_parare, NIHIL);
}
