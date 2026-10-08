/* aemulator_esctest.c - esctest2 (../esctest2 @ 2798f12, GPL-2.0,
 * numquam venditum - solum ut infans currit) intra aemulatorem nostrum
 * (aemulator-plan B4b)
 *
 * Infans: argumenta ductoris post nomen eius (python3 esctest.py
 * ...), a tools/aemulator_esctest.sh tradita. Hospes LXXX x XXV
 * (esctest fenestram ad XXV x LXXX ponit), lectio_schirmi VERUM
 * (decisio XVIII: DECRQCRA sola via qua esctest cellulas legit).
 * Pulsus donec infans exit aut mora tota praeterit.
 *
 * Exitus: codex infantis (esctest: 0 omnes transierunt); II si
 * hospes non creatus aut mora exacta. Linea ultima effusionis:
 * 'aemulator_esctest: ignota N' (series consumptae non tractatae).
 */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "aemulator.h"
#include "pseudoterminale.h"
#include "aemulator_hospes.h"
#include <stdio.h>
#include <stdlib.h>

/* mora tota: pulsus C ms; esctest totus paucis minutis currit */
#define PULSUS_MORA_MS   C
#define PULSUS_MAXIMI    (XX * M * VI)

s32 principale (
                integer  numerus,
    constans character** argumenta)
{
                        Piscina* piscina;
                Pseudoterminale* pt;
                AemulatorHospes* h;
    PseudoterminaleConfiguratio  cfg_pt;
    AemulatorHospesConfiguratio  cfg;
           PseudoterminaleError  error;
                         chorda  descriptio;
          PseudoterminaleExitus  exitus;
                            i32  i;

    si (numerus < II)
    {
        fprintf(stderr,
            "usus: aemulator_esctest <python3> <esctest.py> "
                        "[argumenta esctest...]\n");
        redde II;
    }
    piscina = piscina_generare_dynamicum("aemulator_esctest",
        MXXIV * MXXIV);
    pseudoterminale_configuratio_initiare(&cfg_pt);
    cfg_pt.argumenta    = argumenta + I;
    cfg_pt.latitudo     = LXXX;
    cfg_pt.altitudo     = XXV;
    descriptio.datum    = NIHIL;
    descriptio.mensura  = ZEPHYRUM;
    pt = pseudoterminale_posix_creare(piscina, &cfg_pt, &error,
        &descriptio);
    si (!pt)
    {
        fprintf(stderr, "aemulator_esctest: infans non generatus: %s "
                        "(%.*s)\n", pseudoterminale_error_nomen(error),
            (integer)descriptio.mensura,
            (constans character*)descriptio.datum);
        redde II;
    }
    aemulator_hospes_configuratio_initiare(&cfg);
    cfg.aemulator.latitudo = LXXX;
    cfg.aemulator.altitudo = XXV;
    cfg.aemulator.lectio_schirmi = VERUM;
    h = aemulator_hospes_creare(piscina, &cfg, pt);
    si (!h)
    {
        fprintf(stderr, "aemulator_esctest: hospes non creatus\n");
        redde II;
    }
    per (i = ZEPHYRUM; i < PULSUS_MAXIMI; i++)
    {
        si (aemulator_hospes_pulsare(h, PULSUS_MORA_MS).finitus)
        {
            frange;
        }
    }
    printf("aemulator_esctest: ignota %u\n",
        aemulator_ignota(aemulator_hospes_aemulator(h)));
    si (i >= PULSUS_MAXIMI)
    {
        fprintf(stderr, "aemulator_esctest: mora tota exacta\n");
        aemulator_hospes_claudere(h);
        redde II;
    }
    (vacuum)aemulator_hospes_exitus(h, &exitus);
    aemulator_hospes_claudere(h);
    redde (s32)exitus.codex;
}
