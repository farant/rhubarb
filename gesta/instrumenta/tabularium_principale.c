/* tabularium_principale.c - involucrum tenue servi MCP (machina in
 * gesta/fontes/tabularium.c; exemplar legati: launcher cd radicem,
 * viae relativae). Usus: tabularium -mcp -radix <dir> */

#include "tabularium.h"
#include <string.h>
#include <stdio.h>

s32 principale (integer argc, character** argv)
{
    TabulariumConfiguratio cfg;
    constans character* radix = ".";
    constans character* signum = NIHIL;
    constans character* via_binarii = NIHIL;
    constans character* via_manifesti = NIHIL;
    integer k;

    per (k = I; k < argc; k++)
    {
        si (strcmp(argv[k], "-radix") == ZEPHYRUM && k + I < argc)
        {
            radix = argv[k + I];
            k++;
        }
        alioquin si (strcmp(argv[k], "-signum") == ZEPHYRUM
            && k + I < argc)
        {
            signum = argv[k + I];
            k++;
        }
        alioquin si (strcmp(argv[k], "-binarium") == ZEPHYRUM
            && k + I < argc)
        {
            via_binarii = argv[k + I];
            k++;
        }
        alioquin si (strcmp(argv[k], "-manifestum") == ZEPHYRUM
            && k + I < argc)
        {
            via_manifesti = argv[k + I];
            k++;
        }
        /* -mcp acceptum et omissum (modus unicus) */
    }
    cfg.radix = radix;
    cfg.via_scrinii = "tabularium.db";
    cfg.via_annalium = "gesta/annales/tabularium.jsonl";
    cfg.via_nexus = "build/nexus.tsv";
    cfg.via_identitatum = "build/identitates.tsv";
    cfg.via_citationum = "build/citationes.tsv";
    cfg.via_tabulae = "gesta/annales/tabula.md";
    cfg.via_entitatum = "gesta/annales/entities";
    cfg.signum = signum;
    cfg.via_binarii = via_binarii;
    cfg.via_manifesti = via_manifesti;
    redde tabularium_currere(&cfg, stdin, stdout);
}
