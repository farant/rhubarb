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
    integer k;

    per (k = I; k < argc; k++)
    {
        si (strcmp(argv[k], "-radix") == ZEPHYRUM && k + I < argc)
        {
            radix = argv[k + I];
            k++;
        }
        /* -mcp acceptum et omissum (modus unicus) */
    }
    cfg.radix = radix;
    cfg.via_scrinii = "tabularium.db";
    cfg.via_annalium = "gesta/annales/tabularium.jsonl";
    cfg.via_nexus = "build/nexus.tsv";
    redde tabularium_currere(&cfg, stdin, stdout);
}
