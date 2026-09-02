/* probatio_cliens_tabularii.c - Probationes clientis daemonis.
 *
 * TEGUMENTUM PRIMUM huius codicis: ~CCCXL lineae in apps/forum/forum.c
 * vixerunt, et probationes radicis apps/ NON vident - ergo cliens
 * (cum defectu-CITO subtili incluso) nullum tegumentum automaticum
 * umquam habuit. Extractio in lib/ hoc sanat.
 *
 * OFFLINE: daemon EPHEMERUS portu 0 in mundo scratch (exemplar
 * probatio_fori.c) - nulla rete externa, nullus daemon vivus tactus.
 */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "json.h"
#include "credo.h"
#include "cliens_tabularii.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define VIA_DB   "build/probatio_ct.db"
#define VIA_AN   "build/probatio_ct.jsonl"
#define VIA_PORT "build/probatio_ct_portus.txt"
#define VIA_PID  "build/probatio_ct_pid.txt"

interior vacuum
_purgare (vacuum)
{
    remove(VIA_DB);
    remove(VIA_DB "-wal");
    remove(VIA_DB "-shm");
    remove(VIA_AN);
    remove(VIA_PORT);
    remove(VIA_PID);
}

s32 principale (vacuum)
{
    Piscina* piscina;
        b32  praeteritus;
    integer  portus_int = 0;
    integer  k;

    piscina = piscina_generare_dynamicum("probatio_ct", 16777216);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);
    _purgare();

    /* daemon ephemerus portu auto (exemplar probatio_fori.c) */
    CREDO_VERUM (system("./gesta/tabulariumd.sh -struere"
        " > /dev/null 2>&1") == ZEPHYRUM);
    CREDO_VERUM (system("gesta/build/tabulariumd -portus 0"
        " -scrinium " VIA_DB " -annales " VIA_AN
        " > " VIA_PORT " 2> /dev/null"
        " & echo $! > " VIA_PID) == ZEPHYRUM);
    per (k = ZEPHYRUM; k < XXV; k++)
    {
        FILE* pf = fopen(VIA_PORT, "r");

        si (pf != NIHIL)
        {
            si (   fscanf(pf, "portus %d", &portus_int) == I
                && portus_int > 0)
            {
                fclose(pf);
                frange;
            }
            fclose(pf);
        }
        (vacuum)system("sleep 0.2");
    }
    CREDO_VERUM (portus_int > 0);

    si (portus_int > 0)
    {
        ClientTabularii cliens;
                 chorda culpa;
                 chorda res_id;

        culpa.mensura  = ZEPHYRUM;
        culpa.datum    = NIHIL;

        /* I. initium: valores ordinarii */
        imprimere("\n--- I. initium clientis ---\n");
        cliens_tabularii_incipere(&cliens, (i32)portus_int);
        CREDO_AEQUALIS_I32 (cliens.portus, (i32)portus_int);
        CREDO_VERUM (strcmp(cliens.actor, "fran") == ZEPHYRUM);
        CREDO_FALSUM (cliens.genitus);

        /* II. vocare: instrumentum reale trans TCP */
        {
            JsonValor* args = json_objectum_creare(piscina);
               chorda  textus;

            imprimere("\n--- II. vocare (addere) ---\n");
            json_objectum_ponere(args, "genus",
                json_chorda_creare_literis(piscina, "pipatum"));
            json_objectum_ponere(args, "titulus",
                json_chorda_creare_literis(piscina,
                "pipatum clientis"));
            json_objectum_ponere(args, "corpus",
                json_chorda_creare_literis(piscina,
                    "per clientem extractum."));
            textus = cliens_tabularii_vocare(&cliens, piscina,
                "addere", args, &culpa);

            CREDO_VERUM (textus.mensura > ZEPHYRUM);
            CREDO_VERUM (strstr(cliens_tabularii_litterae(piscina,
                textus), "creata") != NIHIL);

            /* III. res_id ex prosa daemonis extractum */
            imprimere("\n--- III. res_id ex textu ---\n");
            res_id = cliens_tabularii_res_id(textus, piscina);
            CREDO_AEQUALIS_I32 (res_id.mensura, XXVI);
        }

        /* IV. index petitionis crescit (id JSON-RPC unicus) */
        imprimere("\n--- IV. index petitionis ---\n");
        CREDO_VERUM (cliens.petitio_index > (s64)I);

        /* V. legere: tabulatum parsatum, quantum honoratum */
        {
            JsonValor* res;

            imprimere("\n--- V. legere ---\n");
            res = cliens_tabularii_legere(&cliens, piscina,
                chorda_ex_literis("pipatum", piscina), (i32)C,
                &culpa);
            CREDO_NON_NIHIL (res);
            si (res != NIHIL)
            {
                CREDO_VERUM (json_est_tabulatum(res));
                CREDO_VERUM (json_tabulatum_numerus(res) >= I);
            }
        }

        /* VI. tractator transmittere: allowlist + actor iniectus */
        {
            JsonValor* arg        = json_objectum_creare(piscina);
            JsonValor* interiora  = json_objectum_creare(piscina);
            JsonValor* fructus;

            imprimere("\n--- VI. transmittere ---\n");
            json_objectum_ponere(interiora, "textus",
                json_chorda_creare_literis(piscina, "pipatum"));
            json_objectum_ponere(arg, "instrumentum",
                json_chorda_creare_literis(piscina, "quaerere"));
            json_objectum_ponere(arg, "argumenta", interiora);
            culpa.mensura = ZEPHYRUM;
            fructus = cliens_tabularii_transmittere(arg, piscina,
                &cliens, &culpa);
            CREDO_NON_NIHIL (fructus);

            /* instrumentum extra allowlist RECUSATUR */
            {
                JsonValor* pravum     = json_objectum_creare(piscina);
                JsonValor* nihil_arg  = json_objectum_creare(piscina);

                json_objectum_ponere(pravum, "instrumentum",
                    json_chorda_creare_literis(piscina,
                    "delere-omnia"));
                json_objectum_ponere(pravum, "argumenta", nihil_arg);
                culpa.mensura  = ZEPHYRUM;
                culpa.datum    = NIHIL;
                fructus = cliens_tabularii_transmittere(pravum,
                    piscina, &cliens, &culpa);
                CREDO_NIHIL (fructus);
                CREDO_VERUM (culpa.mensura > ZEPHYRUM);
            }
        }

        /* VII. DEFECTUS CITUS: daemon mortuus non congelat.
         * Hoc est lex subtilis quam extractio servare debuit -
         * launcher SEMEL excitatur, deinde defectus statim, ne
         * anquisitio IS (II s) tractatorem V s obstruentem ferat. */
        {
            ClientTabularii  mortuus;
                  JsonValor* args;
                     chorda  textus;

            imprimere("\n--- VII. defectus citus ---\n");
            (vacuum)system("kill $(cat " VIA_PID ") 2>/dev/null");
            (vacuum)system("sleep 0.3");

            cliens_tabularii_incipere(&mortuus, (i32)portus_int);
            /* launcher falsus: excitatio nihil facit, ergo
             * conatus primus solus moram fert */
            mortuus.launcher   = "true";
            mortuus.praefixum  = "[probatio]";

            args           = json_objectum_creare(piscina);
            culpa.mensura  = ZEPHYRUM;
            culpa.datum    = NIHIL;
            textus = cliens_tabularii_vocare(&mortuus, piscina,
                "legere", args, &culpa);
            CREDO_AEQUALIS_I32 (textus.mensura, ZEPHYRUM);
            CREDO_VERUM (culpa.mensura > ZEPHYRUM);
            CREDO_VERUM (mortuus.genitus);

            /* conatus SECUNDUS: statim, sine mora launcheri */
            culpa.mensura  = ZEPHYRUM;
            culpa.datum    = NIHIL;
            textus = cliens_tabularii_vocare(&mortuus, piscina,
                "legere", args, &culpa);
            CREDO_AEQUALIS_I32 (textus.mensura, ZEPHYRUM);
            CREDO_VERUM (culpa.mensura > ZEPHYRUM);
        }
    }

    (vacuum)system("kill $(cat " VIA_PID ") 2>/dev/null");

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    redde I;
}
