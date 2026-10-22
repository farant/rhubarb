/* probatio_officina_vindex_visum.c - M3 chunk 6: pictura vindicis
 * sine capite. Conspectus LITTERALIS (numquam Machinula) per
 * tessera_pons_memoriae pingitur; cellulae assertae (sutura
 * saltuarii: pingere accipit snapshot purum - haec probatio est
 * cur). */

#include "vindex_visum.h"
#include "credo.h"
#include "piscina.h"

#include <stdio.h>
#include <string.h>

#define LATITUDO 100U
#define ALTITUDO 30U

interior chorda
_ch (
    constans character* literis)
{
    chorda c;
    unio { constans character* c; i8* m; } u;

    u.c        = literis;
    c.datum    = u.m;
    c.mensura  = (i32)strlen(literis);
    redde c;
}

/* columna prima ubi textus in linea y apparet; -I si absens */
interior s32
_quaerere (
    constans TesseraOpus* opus,
                 integer  y,
      constans character* textus)
{
    memoriae_index mensura = strlen(textus);
           integer x;

    per (x = 0; x + (integer)mensura <= (integer)LATITUDO; x++)
    {
        memoriae_index k;
                   b32 congruit = VERUM;

        per (k = 0U; k < mensura; k++)
        {
            TesseraCellula cellula = tessera_cellulam_legere(
                opus, x + (integer)k, y);

            si (cellula.signum != (insignatus integer)
                (insignatus character)textus[k])
            {
                congruit = FALSUM;
                frange;
            }
        }
        si (congruit) redde (s32)x;
    }
    redde -I;
}

/* piscina tesserae sectionis (olim in praeludio, numquam destructa) */
interior vacuum
_tesseram_parare (
    CredoContextus* c)
{
    c->datum = tessera_piscina_generare_dynamicum("probatio_visum_t",
        4194304);
    si (c->datum == NIHIL)
    {
        imprimere("FRACTA: piscinae\n");
    }
}

interior vacuum
_tesseram_purgare (
    CredoContextus* c)
{
    si (c->datum != NIHIL)
    {
        tessera_piscina_destruere((TesseraPiscina*)c->datum);
    }
}


/* ==================================================
 * PROBARE: conspectus litteralis pictus, cellulae lectae (scaena:
 * pictura una, deinde imperium et anulus eam continuant)
 * ================================================== */

interior vacuum
_picturam_probare (
    CredoContextus* c)
{
         TesseraPiscina* tp = (TesseraPiscina*)c->datum;
    TesseraPonsMemoriae* pm;
            TesseraOpus* opus;

    si (tp == NIHIL)
    {
        redde;   /* sine assertione: sectio VACUA, suita FRACTA */
    }
    pm = tessera_pons_memoriae_creare(tp, LATITUDO, ALTITUDO);
    CREDO_NON_NIHIL (pm);
    opus = tessera_aperire(tp, &pm->pons);
    CREDO_NON_NIHIL (opus);
    si (opus == NIHIL)
    {
        redde;   /* NON_NIHIL supra fractum: sectio FRACTA */
    }

    {
                      chorda lineae[5];
                         i32 puncta_lineae[1];
          VindexOrdoTabulati tabulata[2];
        VindexOrdoVariabilis variabilia[2];
            VindexConspectus conspectus;

        lineae[0]              = _ch("int computare(int fons)");
        lineae[1]              = _ch("{");
        lineae[2]              = _ch("    int a;");
        lineae[3]              = _ch("    a = fons + 1;");
        lineae[4]              = _ch("}");
        puncta_lineae[0]       = IV;
        tabulata[0].functio    = _ch("computare");
        tabulata[0].positio    = _ch("probe.c:4");
        tabulata[1].functio    = _ch("main");
        tabulata[1].positio    = _ch("probe.c:9");
        variabilia[0].titulus  = _ch("a");
        variabilia[0].typus    = _ch("int");
        variabilia[0].valor    = _ch("7");
        variabilia[1].titulus  = _ch("fons");
        variabilia[1].typus    = _ch("int");
        variabilia[1].valor    = _ch("6");

        memset(&conspectus, ZEPHYRUM,
            magnitudo(VindexConspectus));
        conspectus.lineae            = lineae;
        conspectus.numerus_linearum  = V;
        conspectus.linea_prima       = I;
        conspectus.linea_currens     = IV;
        conspectus.puncta_lineae     = puncta_lineae;
        conspectus.puncta_numerus    = I;
        conspectus.titulus_fontis    = _ch("probe.c");
        conspectus.modus             = _ch("PAUSA");
        conspectus.positio = _ch(
            "probe.c:4  per EXEMPLUM <- probe.h:2");
        conspectus.status              = _ch("status probationis");
        conspectus.tabulata            = tabulata;
        conspectus.tabulata_numerus    = II;
        conspectus.tabula_dextra       = (s32)VINDEX_TABULA_VARIABILIA;
        conspectus.variabilia          = variabilia;
        conspectus.variabilia_numerus  = II;

        vindex_visum_pingere(&conspectus, opus);

        /* vectis tituli */
        CREDO_GRADUS ("vectem tituli");
        CREDO_VERUM (_quaerere(opus, 0, "VINDEX") >= ZEPHYRUM);
        CREDO_VERUM (_quaerere(opus, 0, "probe.c") >= ZEPHYRUM);
        CREDO_VERUM (_quaerere(opus, 0, "[PAUSA]") >= ZEPHYRUM);

        /* fons: signa punctorum + lineae currentis in linea 4
         * (ordo visus = 1 + (4 - linea_prima) = y 4) */
        CREDO_GRADUS ("tabulam fontis");
        {
            TesseraCellula stellula = tessera_cellulam_legere(
                opus, 0, 4);
            TesseraCellula sagitta = tessera_cellulam_legere(
                opus, 1, 4);

            CREDO_AEQUALIS_I32 (stellula.signum,
                (i32)(insignatus character)'*');
            CREDO_AEQUALIS_I32 (sagitta.signum,
                (i32)(insignatus character)'>');
        }
        CREDO_VERUM (_quaerere(opus, 4, "a = fons + 1;")
            >= ZEPHYRUM);
        CREDO_VERUM (_quaerere(opus, 4, "   4|") >= ZEPHYRUM);
        /* linea 1 sine signis */
        {
            TesseraCellula cellula = tessera_cellulam_legere(
                opus, 0, 1);

            CREDO_VERUM (cellula.signum
                != (insignatus integer)(insignatus character)'*');
        }

        /* columna dextra: stiva + variabilia */
        CREDO_GRADUS ("columnam dextram");
        CREDO_VERUM (_quaerere(opus, 1, "STIVA") >= ZEPHYRUM);
        CREDO_VERUM (_quaerere(opus, 2, "computare") >= ZEPHYRUM);
        CREDO_VERUM (_quaerere(opus, 3, "main") >= ZEPHYRUM);
        {
                s32 y_variabilium = -I;
            integer y;

            per (y = 0; y < (integer)ALTITUDO; y++)
            {
                si (_quaerere(opus, y, "VARIABILIA") >= ZEPHYRUM)
                {
                    y_variabilium = (s32)y;
                    frange;
                }
            }
            CREDO_VERUM (y_variabilium > ZEPHYRUM);
            si (y_variabilium > ZEPHYRUM)
            {
                CREDO_VERUM (_quaerere(opus,
                    (integer)y_variabilium + 1, "a")
                    >= ZEPHYRUM);
                CREDO_VERUM (_quaerere(opus,
                    (integer)y_variabilium + 1, "7")
                    >= ZEPHYRUM);
            }
        }

        /* linea positionis (acies macronum) + status */
        CREDO_GRADUS ("positionem + statum");
        CREDO_VERUM (_quaerere(opus, (integer)ALTITUDO - 2,
            "per EXEMPLUM <- probe.h:2") >= ZEPHYRUM);
        CREDO_VERUM (_quaerere(opus, (integer)ALTITUDO - 1,
            "status probationis") >= ZEPHYRUM);

        /* linea imperii aperta */
        CREDO_GRADUS ("lineam imperii");
        conspectus.imperium_apertum  = VERUM;
        conspectus.imperium          = _ch("punctum probe.c:4");
        vindex_visum_pingere(&conspectus, opus);
        {
            TesseraCellula prima = tessera_cellulam_legere(opus,
                0, (integer)ALTITUDO - 1);

            CREDO_AEQUALIS_I32 (prima.signum,
                (i32)(insignatus character)':');
        }
        CREDO_VERUM (_quaerere(opus, (integer)ALTITUDO - 1,
            "punctum probe.c:4") >= ZEPHYRUM);
        CREDO_AEQUALIS_I32 ((i32)opus->cursor_x,
            (i32)(1 + 17));
        /* tabula anuli */
        conspectus.imperium_apertum  = FALSUM;
        conspectus.tabula_dextra     = (s32)VINDEX_TABULA_ANULUS;
        {
            chorda anulus[1];

            anulus[0]                  = _ch("computare +3  probe.c:4");
            conspectus.anulus          = anulus;
            conspectus.anulus_numerus  = I;
            vindex_visum_pingere(&conspectus, opus);
            CREDO_VERUM (_quaerere(opus, 0, "VINDEX")
                >= ZEPHYRUM);
            {
                    s32 inventum = -I;
                integer y;

                per (y = 0; y < (integer)ALTITUDO; y++)
                {
                    si (_quaerere(opus, y, "ANULUS") >= ZEPHYRUM)
                    {
                        inventum = (s32)y;
                        frange;
                    }
                }
                CREDO_VERUM (inventum > ZEPHYRUM);
                si (inventum > ZEPHYRUM)
                {
                    CREDO_VERUM (_quaerere(opus,
                        (integer)inventum + 1, "computare +3")
                        >= ZEPHYRUM);
                }
            }
        }
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "picturam vindicis (cellulae)", _picturam_probare,
        _tesseram_parare, _tesseram_purgare, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    s32 exitus;

    imprimere("\n========================================\n");
    imprimere("PROBATIONES VINDEX VISUM (cellulae)\n");
    imprimere("========================================\n");
    exitus = credo_suitam_currere("officina_vindex_visum", SECTIONES);
    imprimere("========================================\n");
    redde exitus;
}
