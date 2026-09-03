/* computus.c - CLI computi markdown: ./md/computus.sh <plagula.md>
 * [-machina] [-iter N]. '-machina' = ordo TSV cum capite '#' (idem
 * mos ac silva/computus: pythonica silva.metiri eum legit per
 * titulos columnarum). -iter N = mensura N vicibus, ultima impressa
 * (min-of-N a vocante). */
#include "latina.h"
#include "piscina.h"
#include "md_computus.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

interior duplex
_mb (
    memoriae_index n)
{
    redde (duplex)n / (1024.0 * 1024.0);
}

interior i8*
_plagulam_legere (
    constans character* via,
               Piscina* piscina,
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

interior vacuum
_imprimere_hominem (
      constans character* via,
     constans MdComputus* c)
{
    imprimere("computus md %s\n", via);
    imprimere("  fons          %u octeti\n",
        (insignatus)c->octeti_fontis);
    imprimere("  arbor         nodi %u · lexemata %u · STML %u octeti · circuitus %s\n",
        (insignatus)c->nodi, (insignatus)c->lexemata,
        (insignatus)c->octeti_stml, c->successus ? "sanus" : "FRACTUS");
    imprimere("  tempus        parsare %.2f ms · emittere %.2f · STML scribere %.2f"
              " · legere %.2f · conferre %.2f\n",
        c->ms_parsandi, c->ms_emittendi, c->ms_arbor_scribendi,
        c->ms_arbor_legendi, c->ms_comparandi);
    imprimere("  memoria       dati %.2f MB · commissa %.2f MB · otiosa %.2f MB"
              " · apex %.2f MB\n",
        _mb(c->usus), _mb(c->commissa), _mb(c->otiosa), _mb(c->apex));
    imprimere("  alvei         %lu · allocationes %lu (arbor %lu)\n",
        (insignatus longus)c->alvei, (insignatus longus)c->allocationes,
        (insignatus longus)c->allocationes_arboris);
}

interior vacuum
_imprimere_machinam (
      constans character* via,
     constans MdComputus* c)
{
    imprimere("# via\tocteti\tlexemata\tnodi\tocteti_stml\tsana"
              "\tms_parsandi\tms_emittendi\tms_arbor_scribendi"
              "\tms_arbor_legendi\tms_comparandi\tusus\tcommissa\totiosa"
              "\tapex\talvei\tallocationes\tallocationes_arboris\n");
    imprimere("%s\t%u\t%u\t%u\t%u\t%u\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f"
              "\t%lu\t%lu\t%lu\t%lu\t%lu\t%lu\t%lu\n",
        via, (insignatus)c->octeti_fontis, (insignatus)c->lexemata,
        (insignatus)c->nodi, (insignatus)c->octeti_stml,
        (insignatus)(c->successus ? I : ZEPHYRUM),
        c->ms_parsandi, c->ms_emittendi, c->ms_arbor_scribendi,
        c->ms_arbor_legendi, c->ms_comparandi,
        (insignatus longus)c->usus, (insignatus longus)c->commissa,
        (insignatus longus)c->otiosa, (insignatus longus)c->apex,
        (insignatus longus)c->alvei, (insignatus longus)c->allocationes,
        (insignatus longus)c->allocationes_arboris);
}

s32
principale (
      integer   numerus_argumentorum,
    character** argumenta)
{
                                      b32  machina;
                   i32  iter;
               integer  i;      /* contra numerus_argumentorum (integer signatum) */
                   i32  q;
                   b32  sana;
    constans character* via;
               Piscina* piscina;
                   i8*  fons;
                   i32  mensura;
           MdComputus  c;

    machina  = FALSUM;
    iter     = I;
    via      = NIHIL;
    per (i = I; i < numerus_argumentorum; i++)
    {
        si (strcmp(argumenta[i], "-machina") == ZEPHYRUM)
        {
            machina = VERUM;
        }
        alioquin si (   strcmp(argumenta[i], "-iter") == ZEPHYRUM
                     && i + I < numerus_argumentorum)
        {
            iter = (i32)atoi(argumenta[++i]);
            si (iter < I)
            {
                iter = I;
            }
        }
        alioquin
        {
            via = argumenta[i];
        }
    }
    si (via == NIHIL)
    {
        fprintf(stderr,
            "usus: computus <plagula.css> [-machina] [-iter N]\n"
            "  -machina  ordo TSV (caput '#')\n"
            "  -iter N   mensura N vicibus (ultima impressa)\n");
        redde II;
    }
    piscina = piscina_generare_dynamicum("md_computus",
        MMMMXCVI * CCLVI);
    si (piscina == NIHIL)
    {
        fprintf(stderr, "computus: piscina non creata\n");
        redde II;
    }
    fons = _plagulam_legere(via, piscina, &mensura);
    si (fons == NIHIL)
    {
        fprintf(stderr, "computus: plagula non lecta: %s\n", via);
        piscina_destruere(piscina);
        redde II;
    }
    sana = FALSUM;
    per (q = ZEPHYRUM; q < iter; q++)
    {
        sana = md_computus_metiri((constans character*)fons, mensura,
            &c);
    }
    si (machina)
    {
        _imprimere_machinam(via, &c);
    }
    alioquin
    {
        _imprimere_hominem(via, &c);
    }
    piscina_destruere(piscina);
    redde sana ? ZEPHYRUM : I;
}
