/* probatio_toml_computus.c - PORTA COMPUTI TOML: numeri deterministici
 * corporis fixi (tria fixa toml-test, congelata) contra aurum pinnati
 * (nodi, lexemata, octeti STML, diagnostica, memoria piscinae parsurae,
 * allocationes); tempora imprimuntur, non pinnantur. Gemellus
 * probatio_crusta_computus (P10).
 *
 * Corpus: exemplum specificationis, plagula valida maxima, casus
 * invalidus (lex octetorum etiam ibi tenet). Plan dixit 'plagulam
 * silvestrem maximam' - contentum alienum in repositorio non ponitur
 * (Fran 2026-09-28) et aurum input CONGELATUM poscit; toml-test
 * commissum et immutabile est.
 *
 * COMPUTUS_SCRIBERE=1 aurum rescribit - AUDI: regeneratio sine causa
 * nominata portam in numerum vacuum vertit. Causa in commissione.
 */
#include "latina.h"
#include "credo.h"
#include "piscina.h"
#include "toml_computus.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VIA_MAXIMA        DXII
#define COLUMNAE          XII
#define NUMERUS_CORPORIS  III

hic_manens constans character* CORPUS[NUMERUS_CORPORIS] = {
    "toml/probationes/fixa/toml-test/tests/valid/spec-example-1.toml",
    "toml/probationes/fixa/toml-test/tests/valid/multibyte.toml",
    ("toml/probationes/fixa/toml-test/tests/invalid/spec-1.0.0/"
        "string-4-0.toml")
};

hic_manens constans character* TITULI[COLUMNAE] = {
    "octeti", "lexemata", "nodi", "octeti_stml", "diagnostica", "usus",
    "commissa", "otiosa", "apex", "alvei", "allocationes",
    "allocationes_arboris"
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
    constans TomlComputus* c,
        insignatus longus  v[COLUMNAE])
{
    v[ZEPHYRUM]  = (insignatus longus)c->octeti_fontis;
    v[I]         = (insignatus longus)c->lexemata;
    v[II]        = (insignatus longus)c->nodi;
    v[III]       = (insignatus longus)c->octeti_stml;
    v[IV]        = (insignatus longus)c->diagnostica;
    v[V]         = (insignatus longus)c->usus;
    v[VI]        = (insignatus longus)c->commissa;
    v[VII]       = (insignatus longus)c->otiosa;
    v[VIII]      = (insignatus longus)c->apex;
    v[IX]        = (insignatus longus)c->alvei;
    v[X]         = (insignatus longus)c->allocationes;
    v[XI]        = (insignatus longus)c->allocationes_arboris;
}

s32
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

    piscina = piscina_generare_dynamicum("probatio_toml_computus",
        IV * MXXIV * XVI);
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
    sprintf(via_auri, "%s/toml/probationes/fixa/computus/basis.tsv",
        radix);

    imprimere("\n--- I. mensura corporis (fixa toml-test) ---\n");
    per (i = ZEPHYRUM; i < NUMERUS_CORPORIS; i++)
    {
        TomlComputus  c;
                  i8* fons;
                 i32  mensura;

        metita[i] = FALSUM;
        sprintf(via, "%s/%s", radix, CORPUS[i]);
        fons = _plagulam_legere(piscina, via, &mensura);
        CREDO_NON_NIHIL (fons);
        si (fons == NIHIL)
        {
            perge;
        }
        (vacuum)toml_computus_metiri((constans character*)fons, mensura,
            &c);
        _columnas(&c, facta[i]);
        metita[i] = VERUM;
        imprimere("  %s\n    nodi %u lex %u stml %u diag %u  "
                  "dati %.2f MB"
                  "  alloc %lu  pars %.2f ms  emit %.2f  stml %.2f/%.2f"
                  "  conf %.2f  coq %.2f\n",
            CORPUS[i], (insignatus)c.nodi, (insignatus)c.lexemata,
            (insignatus)c.octeti_stml, (insignatus)c.diagnostica,
            (f64)c.usus / (1024.0 * 1024.0),
            (insignatus longus)c.allocationes, c.ms_parsandi,
            c.ms_emittendi, c.ms_arbor_scribendi, c.ms_arbor_legendi,
            c.ms_comparandi, c.ms_coquendi);
        /* proprietates, semper: circuitus totus sanus (etiam invalida);
         * commissa = usus + otiosa; apex usum commissum non superat;
         * allocationes nodos superant */
        CREDO_VERUM (c.successus);
        CREDO_VERUM (c.commissa == c.usus + c.otiosa);
        CREDO_VERUM (c.apex <= c.commissa);
        CREDO_VERUM (c.alvei >= I);
        CREDO_VERUM (c.allocationes > (memoriae_index)c.nodi);
        CREDO_VERUM (c.lexemata >= I && c.octeti_stml > ZEPHYRUM);
    }
    /* invalidum diagnostica habet, valida nulla */
    CREDO_VERUM (facta[ZEPHYRUM][IV] == ZEPHYRUM);
    CREDO_VERUM (facta[I][IV] == ZEPHYRUM);
    CREDO_VERUM (facta[II][IV] > ZEPHYRUM);

    si (scribere)
    {
        imprimere("\n--- AURUM SCRIBITUR - AUDI causam ---\n");
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
        imprimere("  FRACTA: aurum absens - COMPUTUS_SCRIBERE=1\n");
        CREDO_NON_NIHIL (aurum);
    }
    alioquin
    {
        character linea[MMXLVIII];
              i32 lectae = ZEPHYRUM;

        dum (fgets(linea, (integer)magnitudo(linea), aurum) != NIHIL)
        {
                    character* campus;
            insignatus longus  exspectata[COLUMNAE];
                    character  titulus[DXII];
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
                exspectata[n] = strtoul(campus, NIHIL, X);
                n++;
            }
            CREDO_AEQUALIS_I32 ((i32)n, (i32)COLUMNAE);
            idx = (s32)-I;
            per (i = ZEPHYRUM; i < NUMERUS_CORPORIS; i++)
            {
                si (strcmp(CORPUS[i], titulus) == ZEPHYRUM)
                {
                    idx = (s32)i;
                }
            }
            CREDO_VERUM (idx >= ZEPHYRUM);
            si (idx < ZEPHYRUM || !metita[idx] || n != COLUMNAE)
            {
                perge;
            }
            lectae++;
            per (k = ZEPHYRUM; k < COLUMNAE; k++)
            {
                si (facta[idx][k] != exspectata[k])
                {
                    imprimere("  DIVERGIT %s.%s: %lu pro %lu\n",
                        titulus, TITULI[k], facta[idx][k],
                        exspectata[k]);
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
