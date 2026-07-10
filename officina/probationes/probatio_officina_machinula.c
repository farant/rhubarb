/* probatio_officina_machinula.c - Machinula: interpres (M2b)
 *
 * Fixturae per-familiam EXSECUTANTUR (omnes $main -> 42); halitus
 * honesti: sistere (causa), decipula (vocata), exit (aedificatum),
 * stiva exhausta (recursio infinita); numeratores. */

#include "officina_machinula.h"
#include "officina_medulla_textus.h"
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

interior chorda
_plagulam_legere (Piscina* piscina, constans character* via_partis)
{
    character via[CCLVI];
    FILE* plagula;
    chorda fructus;
    long longitudo;
    constans character* radix = getenv("RHUBARB_RADIX");

    fructus.datum = NIHIL;
    fructus.mensura = ZEPHYRUM;
    sprintf(via, "%s/%s", (radix != NIHIL) ? radix : ".",
        via_partis);
    plagula = fopen(via, "rb");
    si (plagula == NIHIL)
    {
        redde fructus;
    }
    fseek(plagula, 0L, SEEK_END);
    longitudo = ftell(plagula);
    fseek(plagula, 0L, SEEK_SET);
    si (longitudo > 0L)
    {
        fructus.datum = (i8*)piscina_allocare(piscina,
            (memoriae_index)longitudo);
        si (fructus.datum != NIHIL
            && fread(fructus.datum, I, (memoriae_index)longitudo,
                   plagula) == (memoriae_index)longitudo)
        {
            fructus.mensura = (i32)longitudo;
        }
        alioquin
        {
            fructus.datum = NIHIL;
        }
    }
    fclose(plagula);
    redde fructus;
}

/* textus -> machinula parata (regio/conexio recentes) */
interior Machinula*
_machinulam_parare (Piscina* piscina, chorda textus, Regio** regio_out)
{
    i32 linea_erroris = ZEPHYRUM;
    MedullaModulus* modulus = medulla_textum_legere(piscina, textus,
        &linea_erroris);
    Regio* regio;
    Conexio* conexio;

    si (modulus == NIHIL)
    {
        imprimere("FRACTA lectio (linea %d)\n", (int)linea_erroris);
        redde NIHIL;
    }
    regio = regio_generare(piscina);
    conexio = conexio_creare(piscina, regio);
    si (!conexio_modulum_addere(conexio, modulus)
        || !conexio_nectere(conexio))
    {
        redde NIHIL;
    }
    *regio_out = regio;
    redde machinula_creare(piscina, conexio, regio);
}

s32 principale (vacuum)
{
    Piscina* piscina;
    b32 praeteritus;

    piscina = piscina_generare_dynamicum("probatio_machinula",
        33554432);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);

    /* ========================================================
     * PROBARE: fixturae per-familiam - omnes $main -> 42
     * ======================================================== */
    {
        constans character* fixturae[] = {
            "officina/probationes/fixa/machinula/arithmetica.medulla",
            "officina/probationes/fixa/machinula/bitalia.medulla",
            "officina/probationes/fixa/machinula/comparationes.medulla",
            "officina/probationes/fixa/machinula/conversiones.medulla",
            "officina/probationes/fixa/machinula/memoria.medulla",
            "officina/probationes/fixa/machinula/fluxus.medulla",
            "officina/probationes/fixa/machinula/vocationes.medulla",
            "officina/probationes/fixa/machinula/data.medulla",
            "officina/probationes/fixa/machinula/sistere.medulla"
        };
        i32 numerus_fixturarum = IX;
        i32 i;

        imprimere("\n--- Probans fixturas (exsecutio -> 42) ---\n");
        per (i = ZEPHYRUM; i < numerus_fixturarum; i++)
        {
            chorda textus = _plagulam_legere(piscina, fixturae[i]);
            Regio* regio = NIHIL;
            Machinula* machinula;
            MachinulaExitus exitus;

            CREDO_VERUM (textus.mensura > ZEPHYRUM);
            machinula = _machinulam_parare(piscina, textus, &regio);
            CREDO_NON_NIHIL (machinula);
            si (machinula == NIHIL)
            {
                perge;
            }
            exitus = machinula_currere(machinula, _ch("main"));
            si (exitus.genus != MACHINULA_BENE
                || exitus.codex != (s64)XLII)
            {
                imprimere("FRACTA fixtura %s (genus %d codex"
                    " %ld)\n", fixturae[i], (int)exitus.genus,
                    (long)exitus.codex);
            }
            CREDO_AEQUALIS_S32 (exitus.genus, MACHINULA_BENE);
            CREDO_VERUM (exitus.codex == (s64)XLII);
            CREDO_VERUM (machinula_numerus_instructionum(machinula)
                > ZEPHYRUM);
            regio_destruere(regio);
        }
    }

    /* ========================================================
     * PROBARE: sistere exsecutum - halitus cum causa
     * ======================================================== */
    {
        chorda textus = _plagulam_legere(piscina,
            "officina/probationes/fixa/machinula/sistere.medulla");
        Regio* regio = NIHIL;
        Machinula* machinula = _machinulam_parare(piscina, textus,
            &regio);
        MachinulaExitus exitus;

        imprimere("\n--- Probans sistere exsecutum ---\n");
        CREDO_NON_NIHIL (machinula);
        exitus = machinula_currere(machinula, _ch("cade"));
        CREDO_AEQUALIS_S32 (exitus.genus, MACHINULA_SISTERE);
        CREDO_VERUM (chorda_aequalis_literis(exitus.nuntius,
            "probatio halitus"));
        regio_destruere(regio);
    }

    /* ========================================================
     * PROBARE: decipula vocata + exit aedificatum + stiva
     * ======================================================== */
    {
        constans character* textus_literis =
            "modulus \"halitus.medulla\"\n"
            "\n"
            "functio $main () -> s32\n"
            "@initium:\n"
            "    %t1 = vocare.s32 $ignotus_omnino\n"
            "    redde %t1\n"
            "\n"
            "functio $exiens () -> s32\n"
            "@initium:\n"
            "    vocare $exit, 7\n"
            "    redde 0\n"
            "\n"
            "functio $abyssus () -> s32\n"
            "@initium:\n"
            "    %t1 = vocare.s32 $abyssus\n"
            "    redde %t1\n";
        chorda textus = _ch(textus_literis);
        Regio* regio = NIHIL;
        Machinula* machinula = _machinulam_parare(piscina, textus,
            &regio);
        MachinulaExitus exitus;

        imprimere("\n--- Probans halitus honestos ---\n");
        CREDO_NON_NIHIL (machinula);

        exitus = machinula_currere(machinula, _ch("main"));
        CREDO_AEQUALIS_S32 (exitus.genus, MACHINULA_DECIPULA);

        exitus = machinula_currere(machinula, _ch("exiens"));
        CREDO_AEQUALIS_S32 (exitus.genus, MACHINULA_BENE);
        CREDO_VERUM (exitus.codex == (s64)VII);

        exitus = machinula_currere(machinula, _ch("abyssus"));
        CREDO_AEQUALIS_S32 (exitus.genus, MACHINULA_VITIUM);

        /* functio absens */
        exitus = machinula_currere(machinula, _ch("nusquam"));
        CREDO_AEQUALIS_S32 (exitus.genus, MACHINULA_VITIUM);

        regio_destruere(regio);
    }

    /* ========================================================
     * PROBARE: aedificata memoriae + formator (snprintf in
     * regionem)
     * ======================================================== */
    {
        constans character* textus_literis =
            "modulus \"aedificata.medulla\"\n"
            "\n"
            "functio $main () -> s32\n"
            "@initium:\n"
            "    %p = vocare.i64 $malloc, 64\n"
            "    %n = vocare.s32 $snprintf, %p, 64, $forma, 6, 7\n"
            "    %l = vocare.i64 $strlen, %p\n"
            "    %t1 = amputare.i64.s32 %l\n"
            "    %t2 = addere.s32 %t1, %n\n"
            "    vocare $free, %p\n"
            "    %t3 = multiplicare.s32 %t2, 3\n"
            "    redde %t3\n"
            "\n"
            "datum $forma magnitudo 8 ordinatio 1\n"
            "    octeti 25642b25 64000000\n";
        /* forma = \"%d+%d\" -> \"6+7\" (3 octeti); 3+... n=3, l=3;
         * (3+3)*3 = 18... immo XVIII non XLII - proba directa */
        chorda textus = _ch(textus_literis);
        Regio* regio = NIHIL;
        Machinula* machinula = _machinulam_parare(piscina, textus,
            &regio);
        MachinulaExitus exitus;

        imprimere("\n--- Probans aedificata + formatorem ---\n");
        CREDO_NON_NIHIL (machinula);
        exitus = machinula_currere(machinula, _ch("main"));
        CREDO_AEQUALIS_S32 (exitus.genus, MACHINULA_BENE);
        CREDO_VERUM (exitus.codex == (s64)XVIII);
        CREDO_VERUM (machinula_numerus_aedificatorum(machinula)
            == IV);
        regio_destruere(regio);
    }

    /* ========================================================
     * PROBARE: numeratores + apex stivae
     * ======================================================== */
    {
        chorda textus = _plagulam_legere(piscina,
            "officina/probationes/fixa/machinula/fluxus.medulla");
        Regio* regio = NIHIL;
        Machinula* machinula = _machinulam_parare(piscina, textus,
            &regio);
        MachinulaExitus exitus;

        imprimere("\n--- Probans numeratores ---\n");
        exitus = machinula_currere(machinula, _ch("main"));
        CREDO_AEQUALIS_S32 (exitus.genus, MACHINULA_BENE);
        /* ansa 6 iterationum: addere bis per iterationem + semel */
        CREDO_VERUM (machinula_numerus_op(machinula,
            MEDULLA_OP_ADDERE) >= (i64)XII);
        CREDO_VERUM (machinula_numerus_op(machinula,
            MEDULLA_OP_RAMUS) == (i64)VII);
        CREDO_VERUM (machinula_numerus_vocationum(machinula)
            == (i64)I);
        CREDO_VERUM (machinula_stiva_apex(machinula) > ZEPHYRUM);
        regio_destruere(regio);
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
