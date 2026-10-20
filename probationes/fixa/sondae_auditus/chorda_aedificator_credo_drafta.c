/* chorda_aedificator_credo_drafta.c - sonda: credo_draft sectiones,
 * omnes hodie RUBRAE esse debent */
#include "postulata_posix.h"
#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "chorda_aedificator.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <locale.h>
#include <sys/wait.h>
#include <unistd.h>

s32 principale (vacuum)
{
    Piscina* piscina;
        b32  praeteritus;

    piscina = piscina_generare_dynamicum("sonda_drafta", MM);
    credo_aperire(piscina);

    /* ---- f64 magni ---- */
    {
        ChordaAedificator* aedificator;
                character  exspectatum[DXII];

        imprimere("\n--- Probans appendere_f64 (textus >= CXXXII) ---\n");

        aedificator = chorda_aedificator_creare(piscina, XVI);
        sprintf(exspectatum, "%.2f", 1e128);
        CREDO_VERUM(chorda_aedificator_appendere_f64(aedificator, 1e128, II));
        CREDO_CHORDA_AEQUALIS_LITERIS(chorda_aedificator_spectare(aedificator),
            exspectatum);

        aedificator = chorda_aedificator_creare(piscina, XVI);
        sprintf(exspectatum, "%.0f", -1e130);
        CREDO_VERUM(chorda_aedificator_appendere_f64(aedificator, -1e130,
            ZEPHYRUM));
        CREDO_CHORDA_AEQUALIS_LITERIS(chorda_aedificator_spectare(aedificator),
            exspectatum);

        aedificator = chorda_aedificator_creare(piscina, XVI);
        sprintf(exspectatum, "%.0f", DBL_MAX);
        CREDO_VERUM(chorda_aedificator_appendere_f64(aedificator, DBL_MAX,
            ZEPHYRUM));
        CREDO_AEQUALIS_I32((i32)chorda_aedificator_longitudo(aedificator),
            CCCIX);
        CREDO_CHORDA_AEQUALIS_LITERIS(chorda_aedificator_spectare(aedificator),
            exspectatum);
    }

    /* ---- json: s.mensura * VI in i32 ---- */
    {
        ChordaAedificator* aedificator;
                       i8* magnus;
                   chorda  ingressus;
                      b32  r;
           memoriae_index  n_octeti;

        imprimere("\n--- Probans appendere_evasus_json (mensura * VI) ---\n");

        n_octeti = (memoriae_index)715827883UL;
        magnus = (i8*)malloc(n_octeti);
        CREDO_NON_NIHIL(magnus);
        si (magnus)
        {
            memset(magnus, 'a', n_octeti);
            ingressus.datum    = magnus;
            ingressus.mensura  = (i32)n_octeti;
            aedificator = chorda_aedificator_creare(piscina, XVI);
            r = chorda_aedificator_appendere_evasus_json(aedificator,
                ingressus);
            CREDO_VERUM(!r
                || chorda_aedificator_longitudo(aedificator) == n_octeti);
            free(magnus);
        }
    }

    /* ---- appendere_chorda vacua domus ---- */
    {
        ChordaAedificator* aedificator;
                   chorda  vacua;

        imprimere("\n--- Probans appendere_chorda (vacua NIHIL) ---\n");

        aedificator = chorda_aedificator_creare(piscina, XVI);
        vacua = chorda_ex_literis("", piscina);
        CREDO_VERUM(chorda_aedificator_appendere_chorda(aedificator, vacua));

        vacua.datum    = NIHIL;
        vacua.mensura  = ZEPHYRUM;
        CREDO_VERUM(chorda_aedificator_appendere_chorda(aedificator, vacua));
        CREDO_AEQUALIS_I32((i32)chorda_aedificator_longitudo(aedificator),
            ZEPHYRUM);
    }

    /* ---- push/pop libra ---- */
    {
        ChordaAedificator* aedificator;
                      i32  k;

        imprimere("\n--- Probans push/pop_indentationem (libra) ---\n");

        aedificator = chorda_aedificator_creare(piscina, XVI);
        per (k = ZEPHYRUM; k < MV; k++)
        {
            chorda_aedificator_push_indentationem(aedificator);
        }
        per (k = ZEPHYRUM; k < M; k++)
        {
            chorda_aedificator_pop_indentationem(aedificator);
        }
        CREDO_AEQUALIS_I32(chorda_aedificator_indentatio_gradus(aedificator),
            V);
    }

    /* ---- appendere_indentationem: gradus * SPATIA ---- */
    {
        ChordaAedificator* aedificator;

        imprimere("\n--- Probans appendere_indentationem (gradus magnus) ---\n");

        aedificator = chorda_aedificator_creare(piscina, XVI);
        CREDO_FALSUM(chorda_aedificator_appendere_indentationem(aedificator,
            (i32)0x80000000UL));
        CREDO_FALSUM(chorda_aedificator_appendere_indentationem(aedificator,
            (i32)0x80000001UL));
        CREDO_AEQUALIS_I32((i32)chorda_aedificator_longitudo(aedificator),
            ZEPHYRUM);
    }

    /* ---- piscina certa: defectus non fatalis ---- */
    {
        pid_t  filius;
          int  status;

        imprimere("\n--- Probans creare in piscina certa (NIHIL) ---\n");

        fflush(stdout);
        filius = fork();
        si (filius == ZEPHYRUM)
        {
                      Piscina* certa;
            ChordaAedificator* aedificator;

            certa = piscina_generare_certae_magnitudinis("probatio_certa",
                CCLVI);
            aedificator = chorda_aedificator_creare(certa, IV * MXXIV);
            _exit(aedificator == NIHIL ? ZEPHYRUM : II);
        }
        status = ZEPHYRUM;
        waitpid(filius, &status, ZEPHYRUM);
        CREDO_VERUM(WIFEXITED(status) && WEXITSTATUS(status) == ZEPHYRUM);
    }

    /* ---- f64 locale ---- */
    {
        ChordaAedificator* aedificator;
      constans character* locus;

        imprimere("\n--- Probans appendere_f64 (LC_NUMERIC) ---\n");

        locus = setlocale(LC_NUMERIC, "de_DE.UTF-8");
        si (locus)
        {
            aedificator = chorda_aedificator_creare(piscina, XVI);
            CREDO_VERUM(chorda_aedificator_appendere_f64(aedificator,
                3.14159, II));
            CREDO_CHORDA_AEQUALIS_LITERIS(
                chorda_aedificator_spectare(aedificator), "3.14");
            setlocale(LC_NUMERIC, "C");
        }
    }

    /* ---- json: UTF-8 invalidum ---- */
    {
        ChordaAedificator* aedificator;
                   chorda  ingressus;
                   chorda  result;
                      b32  r;
                      i32  k;
                      b32  crudus;
                       i8  malus[II];

        imprimere("\n--- Probans appendere_evasus_json (UTF-8 invalidum) ---\n");

        aedificator = chorda_aedificator_creare(piscina, XVI);
        malus[ZEPHYRUM]    = (i8)0x80;
        ingressus.datum    = malus;
        ingressus.mensura  = I;
        r = chorda_aedificator_appendere_evasus_json(aedificator, ingressus);
        result = chorda_aedificator_spectare(aedificator);
        crudus = FALSUM;
        per (k = ZEPHYRUM; k < result.mensura; k++)
        {
            si ((insignatus character)result.datum[k] == 0x80) crudus = VERUM;
        }
        CREDO_VERUM(!r || !crudus);

        aedificator = chorda_aedificator_creare(piscina, XVI);
        malus[ZEPHYRUM]    = (i8)0xC0;
        malus[I]           = (i8)0xAF;
        ingressus.datum    = malus;
        ingressus.mensura  = II;
        r = chorda_aedificator_appendere_evasus_json(aedificator, ingressus);
        result = chorda_aedificator_spectare(aedificator);
        crudus = FALSUM;
        per (k = ZEPHYRUM; k < result.mensura; k++)
        {
            si ((insignatus character)result.datum[k] == 0xC0) crudus = VERUM;
        }
        CREDO_VERUM(!r || !crudus);
    }

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
