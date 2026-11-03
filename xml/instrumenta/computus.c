/* computus.c - CLI computi xml: ./xml/computus.sh <plagula.xml>
 * [-machina] [-iter N]. Gemellus toml/instrumenta/computus.c (plagula
 * per filum_legere_totum, non fopen).
 * '-machina' = ordo TSV cum capite '#' (idem
 * mos ac silva/computus: pythonica silva.metiri eum legit per
 * titulos columnarum). -iter N = mensura N vicibus, ultima impressa
 * (min-of-N a vocante). */
#include "latina.h"
#include "piscina.h"
#include "xml_computus.h"
#include "filum.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

interior f64
_mb (
    memoriae_index n)
{
    redde (f64)n / (1024.0 * 1024.0);
}

interior vacuum
_imprimere_hominem (
         constans character* via,
       constans XmlComputus* c)
{
    imprimere("computus xml %s\n", via);
    imprimere("  fons          %u octeti\n",
        (insignatus)c->octeti_fontis);
    imprimere("  arbor         nodi %u · lexemata %u · STML %u octeti"
              " · circuitus %s · diagnostica %u\n",
        (insignatus)c->nodi, (insignatus)c->lexemata,
        (insignatus)c->octeti_stml, c->successus ? "sanus" : "FRACTUS",
        (insignatus)c->diagnostica);
    imprimere("  tempus        parsare %.2f ms · emittere %.2f"
              " · STML scribere %.2f"
              " · legere %.2f · conferre %.2f · coquere %.2f\n",
        c->ms_parsandi, c->ms_emittendi, c->ms_arbor_scribendi,
        c->ms_arbor_legendi, c->ms_comparandi, c->ms_coquendi);
    imprimere("  per octetum   arbor %.1f x · coctio %.1f x\n",
        c->octeti_fontis > ZEPHYRUM ? (f64)c->usus
            / (f64)c->octeti_fontis : 0.0,
        c->octeti_fontis > ZEPHYRUM ? (f64)c->usus_coquendi
            / (f64)c->octeti_fontis : 0.0);
    imprimere("  memoria       dati %.2f MB · commissa %.2f MB"
              " · otiosa %.2f MB"
              " · apex %.2f MB\n",
        _mb(c->usus), _mb(c->commissa), _mb(c->otiosa), _mb(c->apex));
    imprimere("  alvei         %lu · allocationes %lu (arbor %lu)\n",
        (insignatus longus)c->alvei, (insignatus longus)c->allocationes,
        (insignatus longus)c->allocationes_arboris);
}

interior vacuum
_imprimere_machinam (
         constans character* via,
       constans XmlComputus* c)
{
    imprimere("# via\tocteti\tlexemata\tnodi\tocteti_stml\tsana"
              "\tdiagnostica\tms_parsandi\tms_emittendi"
              "\tms_arbor_scribendi\tms_arbor_legendi\tms_comparandi"
              "\tms_coquendi\tusus\tcommissa"
              "\totiosa"
              "\tapex\talvei\tallocationes\tallocationes_arboris"
              "\tusus_coquendi\n");
    imprimere("%s\t%u\t%u\t%u\t%u\t%u\t%u\t%.3f\t%.3f\t%.3f\t%.3f"
              "\t%.3f\t%.3f\t%lu\t%lu\t%lu\t%lu\t%lu\t%lu\t%lu\t%lu\n",
        via, (insignatus)c->octeti_fontis, (insignatus)c->lexemata,
        (insignatus)c->nodi, (insignatus)c->octeti_stml,
        (insignatus)(c->successus ? I : ZEPHYRUM),
        (insignatus)c->diagnostica,
        c->ms_parsandi, c->ms_emittendi, c->ms_arbor_scribendi,
        c->ms_arbor_legendi, c->ms_comparandi, c->ms_coquendi,
        (insignatus longus)c->usus, (insignatus longus)c->commissa,
        (insignatus longus)c->otiosa, (insignatus longus)c->apex,
        (insignatus longus)c->alvei, (insignatus longus)c->allocationes,
        (insignatus longus)c->allocationes_arboris,
        (insignatus longus)c->usus_coquendi);
}

s32
principale (
      integer   numerus_argumentorum,
    character** argumenta)
{
                                        b32  machina;
                                        i32  iter;
                                    integer  i;
                                        i32  q;
                                        b32  sana;
                         constans character* via;
                                    Piscina* piscina;
                                     chorda  fons;
                                XmlComputus  c;

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
            "usus: computus <plagula.xml> [-machina] [-iter N]\n"
            "  -machina  ordo TSV (caput '#')\n"
            "  -iter N   mensura N vicibus (ultima impressa)\n");
        redde II;
    }
    piscina = piscina_generare_dynamicum("xml_computus",
        IV * MXXIV * CCLVI);
    si (piscina == NIHIL)
    {
        fprintf(stderr, "computus: piscina non creata\n");
        redde II;
    }
    fons = filum_legere_totum(via, piscina);
    si (fons.datum == NIHIL && !filum_existit(via))
    {
        fprintf(stderr, "computus: plagula non lecta: %s\n", via);
        piscina_destruere(piscina);
        redde II;
    }
    sana = FALSUM;
    per (q = ZEPHYRUM; q < iter; q++)
    {
        sana = xml_computus_metiri((constans character*)fons.datum,
            fons.mensura, &c);
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
