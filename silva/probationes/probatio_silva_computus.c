/* probatio_silva_computus.c - porta instrumenti computus (RP §11.3)
 *
 * Corpus fixum (fixa/roundtrip, -nudum: sine clausura, ergo sui
 * iuris et deterministicum); aurum fixa/computus/basis.tsv numeros
 * DETERMINISTICOS pinnat (lexemata, errores, usus, commissa, otiosa,
 * apex, alvei, allocationes, lexatio sola) - tempora imprimuntur,
 * non pinnantur. Aurum absens FRANGIT (numquam tacite viridis).
 * COMPUTUS_SCRIBERE=1 aurum rescribit - AUDI: regeneratio sine
 * causa nominata (constantes RP partis II) regressionem sepelit.
 *
 * Radix repositorii per RHUBARB_RADIX (compile_probationes.sh eam
 * ponit); sine ea '..'.
 */
#include "latina.h"
#include "piscina.h"
#include "silva_computus.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VIA_MAXIMA 1024
#define COLUMNAE 11

hic_manens constans character* CORPUS[] = {
    "arrays.c", "base64.c", "cast_init.c", "color.c"
};
#define NUMERUS_CORPORIS IV

interior i8*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura_exitus)
{
              FILE* pl;
                i8* buffer;
    signatus longus  mensura;

    pl = fopen(via, "rb");
    si (pl == NIHIL) redde NIHIL;
    si (fseek(pl, 0L, SEEK_END) != ZEPHYRUM)
    { fclose(pl); redde NIHIL;
    }
    mensura = ftell(pl);
    si (mensura < 0L)
    { fclose(pl); redde NIHIL;
    }
    rewind(pl);
    buffer = (i8*)piscina_allocare(piscina, (memoriae_index)(mensura
        + 1L));
    si (buffer == NIHIL)
    { fclose(pl); redde NIHIL;
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
    constans SilvaComputus* c,
         insignatus longus  v[COLUMNAE])
{
    v[0]   = (insignatus longus)c->lexemata;
    v[1]   = (insignatus longus)c->lexemata_cruda;
    v[2]   = (insignatus longus)c->errores;
    v[3]   = (insignatus longus)c->usus;
    v[4]   = (insignatus longus)c->commissa;
    v[5]   = (insignatus longus)c->otiosa;
    v[6]   = (insignatus longus)c->apex;
    v[7]   = (insignatus longus)c->alvei;
    v[8]   = (insignatus longus)c->allocationes;
    v[9]   = (insignatus longus)c->usus_lexandi;
    v[10]  = (insignatus longus)c->allocationes_lexandi;
}

hic_manens constans character* TITULI[COLUMNAE] = {
    "lexemata", "lexemata_cruda", "errores", "usus", "commissa",
        "otiosa",
    "apex", "alvei", "allocationes", "usus_lexandi",
        "allocationes_lexandi"
};

s32 principale (vacuum)
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

    piscina = piscina_generare_dynamicum("probatio_silva_computus",
        MMMMXCVI * XVI);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);

    radix = getenv("RHUBARB_RADIX");
    si (radix == NIHIL) radix = "..";
    scribere = getenv("COMPUTUS_SCRIBERE") != NIHIL;
    sprintf(via_auri, "%s/silva/probationes/fixa/computus/basis.tsv",
        radix);

    imprimere("\n--- I. mensura corporis (fixa/roundtrip, -nudum) ---\n");
    per (i = ZEPHYRUM; i < NUMERUS_CORPORIS; i++)
    {
        SilvaComputus  c;
                   i8* fons;
                  i32  mensura;

        metita[i] = FALSUM;
        sprintf(via, "%s/probationes/fixa/roundtrip/%s", radix,
            CORPUS[i]);
        fons = _plagulam_legere(piscina, via, &mensura);
        CREDO_NON_NIHIL (fons);
        si (fons == NIHIL) perge;
        (vacuum)silva_computus_metiri(CORPUS[i],
            (constans character*)fons,
            mensura, NIHIL, ZEPHYRUM, &c);
        _columnas(&c, facta[i]);
        metita[i] = VERUM;
        imprimere("  %-14s lexemata %5u  dati %6.2f MB  commissa %6.2f MB"
                  "  alvei %lu  alloc %lu  lex %.2f ms  pars %.2f ms\n",
            CORPUS[i], (insignatus)c.lexemata,
            (duplex)c.usus / (1024.0 * 1024.0),
            (duplex)c.commissa / (1024.0 * 1024.0),
            (insignatus longus)c.alvei,
            (insignatus longus)c.allocationes,
            c.ms_lexandi, c.ms_parsandi);

        /* proprietates, semper: commissa = usus + otiosa; allocationes
         * lexemata superant (RP 3.2: Xar per lexema quattuor); apex
         * usum non superat sine reficere */
        CREDO_VERUM (c.commissa == c.usus + c.otiosa);
        CREDO_VERUM (c.allocationes > (memoriae_index)c.lexemata);
        CREDO_VERUM (c.apex <= c.commissa);
        CREDO_VERUM (c.alvei >= I);
    }

    si (scribere)
    {
        imprimere("\n--- AURUM SCRIBITUR (COMPUTUS_SCRIBERE) - AUDI causam ---\n");
        aurum = fopen(via_auri, "w");
        CREDO_NON_NIHIL (aurum);
        si (aurum != NIHIL)
        {
            fprintf(aurum, "# via");
            per (k = ZEPHYRUM; k < COLUMNAE; k++) fprintf(aurum, "\t%s",
                                                      TITULI[k]);
            fprintf(aurum, "\n");
            per (i = ZEPHYRUM; i < NUMERUS_CORPORIS; i++)
            {
                si (!metita[i]) perge;
                fprintf(aurum, "%s", CORPUS[i]);
                per (k = ZEPHYRUM; k < COLUMNAE; k++) fprintf(aurum,
                                                          "\t%lu",
                                                          facta[i][k]);
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

            si (   linea[ZEPHYRUM] == '#'
                || linea[ZEPHYRUM] == '\n') perge;
            campus = strtok(linea, "\t\n");
            si (campus == NIHIL) perge;
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
                si (strcmp(CORPUS[i], titulus) == ZEPHYRUM) idx =
                                                                (s32)i;
            }
            CREDO_VERUM (idx >= ZEPHYRUM);
            si (idx < ZEPHYRUM || !metita[idx]) perge;
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
        /* aurum plagulas OMNES corporis ferat - lectae < corpus =
         * aurum vetus aut truncatum */
        CREDO_AEQUALIS_I32 ((i32)lectae, (i32)NUMERUS_CORPORIS);
    }

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
