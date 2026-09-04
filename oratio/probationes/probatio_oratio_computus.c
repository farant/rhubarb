/* probatio_oratio_computus.c - PORTA COMPUTI ORATIONIS: numeri deterministici
 * corporis fixi (fixa/txt - snapshots, FONTES.md) contra aurum pinnati (nodi, lexemata,
 * octeti STML, memoria piscinae parsurae, allocationes); tempora
 * imprimuntur, non pinnantur. Gemellus probatio_md_computus.
 *
 * COMPUTUS_SCRIBERE=1 aurum rescribit - AUDI: regeneratio sine causa
 * nominata portam in numerum vacuum vertit. Causa in commissione.
 */
#include "latina.h"
#include "credo.h"
#include "piscina.h"
#include "oratio_computus.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VIA_MAXIMA 512
#define COLUMNAE   11
#define NUMERUS_CORPORIS 5

hic_manens constans character* CORPUS[NUMERUS_CORPORIS] = {
    "hilarius.txt",
    "propertius.txt",
    "cicero.txt",
    "lincoln.txt",
    "trinity.txt"
};

hic_manens constans character* TITULI[COLUMNAE] = {
    "octeti", "lexemata", "nodi", "octeti_stml", "usus", "commissa",
    "otiosa", "apex", "alvei", "allocationes", "allocationes_arboris"
};

interior i8*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura_exitus)
{
    FILE* pl;
    longus mensura;
      i8* buffer;

    pl = fopen(via, "rb");
    si (pl == NIHIL)
    {
        redde NIHIL;
    }
    fseek(pl, 0L, SEEK_END);
    mensura = ftell(pl);
    si (mensura < 0L)
    {
        fclose(pl);
        redde NIHIL;
    }
    rewind(pl);
    buffer = (i8*)piscina_allocare(piscina, (memoriae_index)(mensura
        + 1L));
    si (buffer == NIHIL)
    {
        fclose(pl);
        redde NIHIL;
    }
    si (   mensura > 0L
        && fread(buffer, I, (memoriae_index)mensura, pl)
               != (memoriae_index)mensura)
    {
        fclose(pl);
        redde NIHIL;
    }
    fclose(pl);
    *mensura_exitus = (i32)mensura;
    redde buffer;
}

/* columnae pinnatae, ordine auri */
interior vacuum
_columnas (
     constans OratioComputus* c,
           insignatus longus  v[COLUMNAE])
{
    v[0]   = (insignatus longus)c->octeti_fontis;
    v[1]   = (insignatus longus)c->lexemata;
    v[2]   = (insignatus longus)c->nodi;
    v[3]   = (insignatus longus)c->octeti_stml;
    v[4]   = (insignatus longus)c->usus;
    v[5]   = (insignatus longus)c->commissa;
    v[6]   = (insignatus longus)c->otiosa;
    v[7]   = (insignatus longus)c->apex;
    v[8]   = (insignatus longus)c->alvei;
    v[9]   = (insignatus longus)c->allocationes;
    v[10]  = (insignatus longus)c->allocationes_arboris;
}

integer
principale (vacuum)
{
                   b32  praeteritus;
               Piscina* piscina;
    constans character* radix;
                   b32  scribere;
             character  via_auri[VIA_MAXIMA];
             character  via[VIA_MAXIMA];
                  FILE* aurum;
                   i32  i;
                   i32  k;
     insignatus longus  facta[NUMERUS_CORPORIS][COLUMNAE];
                   b32  metita[NUMERUS_CORPORIS];

    piscina = piscina_generare_dynamicum("probatio_oratio_computus",
        MMMMXCVI * XVI);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);
    radix = getenv("RHUBARB_RADIX");
    si (radix == NIHIL)
    {
        radix = ".";
    }
    scribere = getenv("COMPUTUS_SCRIBERE") != NIHIL;
    sprintf(via_auri, "%s/oratio/probationes/fixa/computus/basis.tsv",
        radix);

    imprimere("\n--- I. mensura corporis (fixa/txt) ---\n");
    per (i = ZEPHYRUM; i < NUMERUS_CORPORIS; i++)
    {
         OratioComputus  c;
                     i8* fons;
                    i32  mensura;

        metita[i] = FALSUM;
        sprintf(via, "%s/oratio/probationes/fixa/txt/%s", radix,
            CORPUS[i]);
        fons = _plagulam_legere(piscina, via, &mensura);
        CREDO_NON_NIHIL (fons);
        si (fons == NIHIL)
        {
            perge;
        }
        (vacuum)oratio_computus_metiri((constans character*)fons,
            mensura,
            &c);
        _columnas(&c, facta[i]);
        metita[i] = VERUM;
        imprimere("  %-26s nodi %5u lex %5u stml %6u  dati %5.2f MB"
                  "  alloc %lu  pars %.2f ms  emit %.2f  stml %.2f/%.2f"
                  "  conf %.2f\n",
            CORPUS[i], (insignatus)c.nodi, (insignatus)c.lexemata,
            (insignatus)c.octeti_stml,
            (duplex)c.usus / (1024.0 * 1024.0),
            (insignatus longus)c.allocationes, c.ms_parsandi,
            c.ms_emittendi, c.ms_arbor_scribendi, c.ms_arbor_legendi,
            c.ms_comparandi);
        /* proprietates, semper: circuitus totus sanus; commissa = usus
         * + otiosa; apex usum commissum non superat; allocationes nodos
         * superant (Xar per nodum) */
        CREDO_VERUM (c.successus);
        CREDO_VERUM (c.commissa == c.usus + c.otiosa);
        CREDO_VERUM (c.apex <= c.commissa);
        CREDO_VERUM (c.alvei >= I);
        CREDO_VERUM (c.allocationes > (memoriae_index)c.nodi);
        CREDO_VERUM (c.lexemata >= I && c.octeti_stml > ZEPHYRUM);
    }

    si (scribere)
    {
        imprimere("\n--- AURUM SCRIBITUR (COMPUTUS_SCRIBERE) - AUDI causam ---\n");
        aurum = fopen(via_auri, "w");
        CREDO_NON_NIHIL (aurum);
        si (aurum != NIHIL)
        {
            fprintf(aurum, "# via");
            per (k = ZEPHYRUM; k < COLUMNAE; k++)
            {
                fprintf(aurum, "\t%s", TITULI[k]);
            }
            fprintf(aurum, "\n");
            per (i = ZEPHYRUM; i < NUMERUS_CORPORIS; i++)
            {
                si (!metita[i])
                {
                    perge;
                }
                fprintf(aurum, "%s", CORPUS[i]);
                per (k = ZEPHYRUM; k < COLUMNAE; k++)
                {
                    fprintf(aurum, "\t%lu", facta[i][k]);
                }
                fprintf(aurum, "\n");
            }
            fclose(aurum);
            imprimere("  scriptum: %s\n", via_auri);
        }
    }

    imprimere("\n--- II. collatio cum auro (%s) ---\n", via_auri);
    aurum = fopen(via_auri, "r");
    si (aurum == NIHIL)
    {
        imprimere("  FRACTA: aurum absens - primum: COMPUTUS_SCRIBERE=1\n");
        CREDO_NON_NIHIL (aurum);
    }
    alioquin
    {
        character linea[2048];
              i32 lectae;

        lectae = ZEPHYRUM;
        dum (fgets(linea, (integer)magnitudo(linea), aurum) != NIHIL)
        {
                    character* campus;
            insignatus longus  exspectata[COLUMNAE];
                    character  titulus[128];
                          i32  n;
                          s32  idx;   /* -I = non inventum */

            si (linea[ZEPHYRUM] == '#' || linea[ZEPHYRUM] == '\n')
            {
                perge;
            }
            campus = strtok(linea, "\t\n");
            si (campus == NIHIL)
            {
                perge;
            }
            strncpy(titulus, campus, magnitudo(titulus) - I);
            titulus[magnitudo(titulus) - I]  = '\0';
            n                                = ZEPHYRUM;
            dum (   n < COLUMNAE
                 && (campus = strtok(NIHIL, "\t\n")) != NIHIL)
            {
                exspectata[n] = strtoul(campus, NIHIL, 10);
                n++;
            }
            CREDO_AEQUALIS_I32 ((i32)n, (i32)COLUMNAE);
            idx = -I;
            per (i = ZEPHYRUM; i < NUMERUS_CORPORIS; i++)
            {
                si (strcmp(CORPUS[i], titulus) == ZEPHYRUM)
                {
                    idx = (s32)i;
                }
            }
            CREDO_VERUM (idx >= ZEPHYRUM);
            si (idx < ZEPHYRUM || !metita[idx])
            {
                perge;
            }
            lectae++;
            per (k = ZEPHYRUM; k < COLUMNAE; k++)
            {
                si (facta[idx][k] != exspectata[k])
                {
                    imprimere("  DIVERGIT %s.%s: aurum %lu, factum %lu\n",
                        titulus, TITULI[k], exspectata[k],
                        facta[idx][k]);
                }
                CREDO_VERUM (facta[idx][k] == exspectata[k]);
            }
        }
        fclose(aurum);
        /* aurum plagulas OMNES corporis ferat */
        CREDO_AEQUALIS_I32 ((i32)lectae, (i32)NUMERUS_CORPORIS);
    }

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
