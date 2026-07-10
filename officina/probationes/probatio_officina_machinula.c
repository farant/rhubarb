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
     * PROBARE: time aedificatum (ISO) - scriptura per
     * monstratorem eodem valore ac fructus (vocatio UNA, ergo
     * determinatum); valor post 2020; ordo monotonus
     * ======================================================== */
    {
        constans character* textus_literis =
            "modulus \"tempus.medulla\"\n"
            "\n"
            "functio $main () -> s32\n"
            "@initium:\n"
            "    %v1 = vocare.s64 $time, 0\n"
            "    %p = arca 8, 8\n"
            "    %v2 = vocare.s64 $time, %p\n"
            "    %w = legere.s64 %p\n"
            "    %aeq = aequalis.s64 %v2, %w\n"
            "    %post = maior.s64 %v1, 1577836800\n"
            "    %ordo = maior_aequalis.s64 %v2, %v1\n"
            "    %t1 = et.i32 %aeq, %post\n"
            "    %t2 = et.i32 %t1, %ordo\n"
            "    %t3 = multiplicare.s32 %t2, 42\n"
            "    redde %t3\n";
        chorda textus = _ch(textus_literis);
        Regio* regio = NIHIL;
        Machinula* machinula = _machinulam_parare(piscina, textus,
            &regio);
        MachinulaExitus exitus;

        imprimere("\n--- Probans time aedificatum ---\n");
        CREDO_NON_NIHIL (machinula);
        exitus = machinula_currere(machinula, _ch("main"));
        CREDO_AEQUALIS_S32 (exitus.genus, MACHINULA_BENE);
        CREDO_VERUM (exitus.codex == (s64)XLII);
        CREDO_VERUM (machinula_numerus_aedificatorum(machinula)
            == II);
        regio_destruere(regio);
    }

    /* ========================================================
     * PROBARE: ansae plagularum (M2d) - vita tota: fopen/fputc/
     * fwrite/ftell/fclose -> stat(st_size @96!) -> fopen/fread ->
     * remove -> gettimeofday/getcwd. 'A'+'B'+'C' = 198; omnia
     * recta -> 42. Via sub officina/build/ (CWD = radix).
     * ======================================================== */
    {
        constans character* textus_literis =
            "modulus \"ansae.medulla\"\n"
            "\n"
            "functio $main () -> s32\n"
            "@initium:\n"
            "    %via = locus $via_pr\n"
            "    %mw = locus $modus_w\n"
            "    %pl = vocare.s64 $fopen, %via, %mw\n"
            "    %ok1 = maior_aequalis.s64 %pl, 3\n"
            "    %c = vocare.s32 $fputc, 65, %pl\n"
            "    %ok2 = aequalis.s32 %c, 65\n"
            "    %bc = locus $bc_datum\n"
            "    %w = vocare.s64 $fwrite, %bc, 1, 2, %pl\n"
            "    %ok3 = aequalis.s64 %w, 2\n"
            "    %t = vocare.s64 $ftell, %pl\n"
            "    %ok4 = aequalis.s64 %t, 3\n"
            "    %f1 = vocare.s32 $fclose, %pl\n"
            "    %ok5 = aequalis.s32 %f1, 0\n"
            "    %arca_st = arca 144, 8\n"
            "    %st = vocare.s32 $stat, %via, %arca_st\n"
            "    %ok6 = aequalis.s32 %st, 0\n"
            "    %p_sz = addere.i64 %arca_st, 96\n"
            "    %sz = legere.s64 %p_sz\n"
            "    %ok7 = aequalis.s64 %sz, 3\n"
            "    %mr = locus $modus_r\n"
            "    %pl2 = vocare.s64 $fopen, %via, %mr\n"
            "    %buf = arca 8, 8\n"
            "    %r = vocare.s64 $fread, %buf, 1, 3, %pl2\n"
            "    %ok8 = aequalis.s64 %r, 3\n"
            "    %b0 = legere.i8 %buf\n"
            "    %p1 = addere.i64 %buf, 1\n"
            "    %b1 = legere.i8 %p1\n"
            "    %p2 = addere.i64 %buf, 2\n"
            "    %b2 = legere.i8 %p2\n"
            "    %s01 = addere.s32 %b0, %b1\n"
            "    %sum = addere.s32 %s01, %b2\n"
            "    %ok9 = aequalis.s32 %sum, 198\n"
            "    %f2 = vocare.s32 $fclose, %pl2\n"
            "    %rm = vocare.s32 $remove, %via\n"
            "    %ok10 = aequalis.s32 %rm, 0\n"
            "    %tv = arca 16, 8\n"
            "    %g = vocare.s32 $gettimeofday, %tv\n"
            "    %ok11 = aequalis.s32 %g, 0\n"
            "    %sec = legere.s64 %tv\n"
            "    %ok12 = maior.s64 %sec, 0\n"
            "    %cwb = arca 256, 8\n"
            "    %cw = vocare.s64 $getcwd, %cwb, 256\n"
            "    %ok13 = inaequalis.s64 %cw, 0\n"
            "    %e1 = et.i32 %ok1, %ok2\n"
            "    %e2 = et.i32 %e1, %ok3\n"
            "    %e3 = et.i32 %e2, %ok4\n"
            "    %e4 = et.i32 %e3, %ok5\n"
            "    %e5 = et.i32 %e4, %ok6\n"
            "    %e6 = et.i32 %e5, %ok7\n"
            "    %e7 = et.i32 %e6, %ok8\n"
            "    %e8 = et.i32 %e7, %ok9\n"
            "    %e9 = et.i32 %e8, %ok10\n"
            "    %e10 = et.i32 %e9, %ok11\n"
            "    %e11 = et.i32 %e10, %ok12\n"
            "    %e12 = et.i32 %e11, %ok13\n"
            "    %fr = multiplicare.s32 %e12, 42\n"
            "    redde %fr\n"
            "\n"
            "datum $via_pr magnitudo 24 ordinatio 1\n"
            "    octeti 6f666669 63696e61 2f627569 6c642f70"
            " 612e7478 74000000\n"
            "datum $modus_w magnitudo 4 ordinatio 1\n"
            "    octeti 77000000\n"
            "datum $modus_r magnitudo 4 ordinatio 1\n"
            "    octeti 72000000\n"
            "datum $bc_datum magnitudo 4 ordinatio 1\n"
            "    octeti 42430000\n";
        chorda textus = _ch(textus_literis);
        Regio* regio = NIHIL;
        Machinula* machinula = _machinulam_parare(piscina, textus,
            &regio);
        MachinulaExitus exitus;

        imprimere("\n--- Probans ansas plagularum ---\n");
        CREDO_NON_NIHIL (machinula);
        exitus = machinula_currere(machinula, _ch("main"));
        CREDO_AEQUALIS_S32 (exitus.genus, MACHINULA_BENE);
        CREDO_VERUM (exitus.codex == (s64)XLII);
        CREDO_VERUM (machinula_numerus_aedificatorum(machinula)
            == XII);
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
