/* tabularium_principale.c - involucrum tenue servi MCP (machina in
 * gesta/fontes/tabularium.c; exemplar legati: launcher cd radicem).
 * Viae codicis relativae ad arborem; acta in SEDE ANNALIUM
 * (annales_sedes.h). Usus: tabularium -mcp -radix <dir> [-genesis] */

#include "tabularium.h"
#include "annales_sedes.h"
#include <string.h>
#include <stdio.h>

s32
principale (
      integer   argc,
    character** argv)
{
    TabulariumConfiguratio  cfg;
        constans character* radix          = ".";
        constans character* signum         = NIHIL;
        constans character* via_binarii    = NIHIL;
        constans character* via_manifesti  = NIHIL;
                       b32  renatus        = FALSUM;
                       b32  genesis        = FALSUM;
                   integer  k;
                   Piscina* piscina;
             AnnaliumSedes  sedes;
                    chorda  causa;

    per (k = I; k < argc; k++)
    {
        si (strcmp(argv[k], "-radix") == ZEPHYRUM && k + I < argc)
        {
            radix = argv[k + I];
            k++;
        }
        alioquin si (strcmp(argv[k], "-renatus") == ZEPHYRUM)
        {
            renatus = VERUM;
        }
        alioquin si (strcmp(argv[k], "-genesis") == ZEPHYRUM)
        {
            genesis = VERUM;
        }
        alioquin si (   strcmp(argv[k], "-signum") == ZEPHYRUM
                     && k + I < argc)
        {
            signum = argv[k + I];
            k++;
        }
        alioquin si (   strcmp(argv[k], "-binarium") == ZEPHYRUM
                     && k + I < argc)
        {
            via_binarii = argv[k + I];
            k++;
        }
        alioquin si (   strcmp(argv[k], "-manifestum") == ZEPHYRUM
                     && k + I < argc)
        {
            via_manifesti = argv[k + I];
            k++;
        }
        /* -mcp acceptum et omissum (modus unicus) */
    }
    /* SEDES ANNALIUM (domus tota, non arbor): custos ante aperturam
     * - arbor recens scrinium vacuum NON incipit (MKMD2) */
    piscina = piscina_generare_dynamicum("tabularium_sedes", 65536);
    si (piscina == NIHIL)
    {
        redde I;
    }
    si (   !annales_sedem_invenire(radix, piscina, &sedes, &causa)
        || !annales_custodire(&sedes, ANNALES_TABULARII,
               ANNALES_SCRINIUM_TABULARII, genesis, piscina, &causa))
    {
        fprintf(stderr, "tabularium: RECUSATUM - %.*s\n",
            (integer)causa.mensura, (constans character*)causa.datum);
        redde I;
    }
    cfg.radix             = radix;
    cfg.via_scrinii       = chorda_ut_cstr(annales_via(&sedes,
        ANNALES_SCRINIUM_TABULARII, piscina), piscina);
    cfg.via_annalium      = chorda_ut_cstr(annales_via(&sedes,
        ANNALES_TABULARII, piscina), piscina);
    cfg.via_nexus        = "build/nexus.tsv";
    cfg.via_identitatum  = "build/identitates.tsv";
    cfg.via_citationum   = "build/citationes.tsv";
    cfg.via_tabulae       = chorda_ut_cstr(annales_via(&sedes,
        ANNALES_TABULA, piscina), piscina);
    cfg.via_entitatum     = chorda_ut_cstr(annales_via(&sedes,
        ANNALES_ENTIA, piscina), piscina);
    cfg.signum            = signum;
    cfg.via_binarii       = via_binarii;
    cfg.via_manifesti     = via_manifesti;
    cfg.via_renovatoris   = NIHIL;   /* derivata: radix + launcher */
    cfg.renovatio_exitus  = FALSUM; /* stdio: exec, non exitus */
    cfg.renatus           = renatus;
    redde tabularium_currere(&cfg, stdin, stdout);
}
