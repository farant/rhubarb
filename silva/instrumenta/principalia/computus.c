/* computus.c - imago memoriae et temporis parseris super plagulam
 *
 * usus:
 *   ./silva/computus.sh <plagula.c> [-nudum] [-machina]
 *
 * Clausura inclusionum per aedilem (ut arbor.sh); '-nudum' eam non
 * praebet (sine latina.h codex latinizatus NON parsatur - numeri
 * tunc fracturam metiuntur, non parsuram). '-machina' = ordo TSV cum
 * capite '#'. Exitus: 0 parsura sana | 1 parsura fracta (numeri
 * tamen impressi) | 2 usus malus / plagula absens.
 */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include "silva_computus.h"
#include <stdio.h>
#include <string.h>

#define CLAUSURA_MAXIMA 96
#define VIA_MAXIMA 512

/* Clausuram capitum domesticorum ex aedile petere (ut arbor.c) */
interior i32
_clausuram_petere (
     constans character* via_relativa,
              character  series[CLAUSURA_MAXIMA][VIA_MAXIMA])
{
    character mandatum[1024];
    character linea[VIA_MAXIMA + 64];
        FILE* tubus;
         i32  numerus;

    numerus = ZEPHYRUM;
    sprintf(mandatum, "./bin/aedilis '%s' --partes 2>/dev/null",
        via_relativa);
    tubus = popen(mandatum, "r");
    si (tubus == NIHIL)
    {
        redde ZEPHYRUM;
    }
    dum (fgets(linea, (integer)magnitudo(linea), tubus) != NIHIL)
    {
        memoriae_index m;

        si (linea[ZEPHYRUM] != 'C' || linea[I] != '\t')
        {
            perge;
        }
        m = strlen(linea);
        dum (   m > ZEPHYRUM
             && (linea[m - I] == '\n' || linea[m - I] == '\r'))
        {
            linea[m - I] = '\0';
            m--;
        }
        si (   m      <= II || numerus >= CLAUSURA_MAXIMA
            || m - II >= VIA_MAXIMA)
        {
            perge;
        }
        strcpy(series[numerus], linea + II);
        numerus++;
    }
    pclose(tubus);
    redde numerus;
}

interior duplex
_mb (
    memoriae_index octeti)
{
    redde (duplex)octeti / (1024.0 * 1024.0);
}

interior vacuum
_imprimere_hominem (
        constans character* via,
    constans SilvaComputus* c,
                       i32  numerus_clausurae)
{
    imprimere("computus %s%s\n", via,
        numerus_clausurae > ZEPHYRUM ? "" : " (-nudum)");
    imprimere("  fons          %u octeti\n",
        (insignatus)c->octeti_fontis);
    imprimere("  lexemata      %u (cruda %u) · errores %u · parsura %s\n",
        (insignatus)c->lexemata, (insignatus)c->lexemata_cruda,
        (insignatus)c->errores, c->successus ? "sana" : "FRACTA");
    imprimere("  tempus        lexare %.2f ms · parsare %.2f ms\n",
        c->ms_lexandi, c->ms_parsandi);
    imprimere("  memoria       dati %.2f MB · commissa %.2f MB · otiosa %.2f MB"
              " · apex %.2f MB\n",
        _mb(c->usus), _mb(c->commissa), _mb(c->otiosa), _mb(c->apex));
    imprimere("  alvei         %lu · allocationes %lu"
              " (lexatio sola: %.2f MB, %lu allocationes)\n",
        (insignatus longus)c->alvei, (insignatus longus)c->allocationes,
        _mb(c->usus_lexandi),
        (insignatus longus)c->allocationes_lexandi);
}

interior vacuum
_imprimere_machinam (
        constans character* via,
    constans SilvaComputus* c)
{
    imprimere("# via\toctetos\tlexemata\tlexemata_cruda\terrores\tsana"
              "\tms_lexandi\tms_parsandi\tusus\tcommissa\totiosa\tapex"
              "\talvei\tallocationes\tusus_lexandi\tallocationes_lexandi\n");
    imprimere("%s\t%u\t%u\t%u\t%u\t%u\t%.3f\t%.3f\t%lu\t%lu\t%lu\t%lu"
              "\t%lu\t%lu\t%lu\t%lu\n",
        via, (insignatus)c->octeti_fontis, (insignatus)c->lexemata,
        (insignatus)c->lexemata_cruda, (insignatus)c->errores,
        (insignatus)(c->successus ? I : ZEPHYRUM),
        c->ms_lexandi, c->ms_parsandi,
        (insignatus longus)c->usus, (insignatus longus)c->commissa,
        (insignatus longus)c->otiosa, (insignatus longus)c->apex,
        (insignatus longus)c->alvei, (insignatus longus)c->allocationes,
        (insignatus longus)c->usus_lexandi,
        (insignatus longus)c->allocationes_lexandi);
}

s32
principale (
               integer  numerus_argumentorum,
    constans character* argumenta[])
{
                  Piscina* piscina;
       constans character* via;
                      b32  nudum;
                      b32  machina;
                  integer  i;
                   chorda  fons;
                      i32  numerus_clausurae;
     hic_manens character  clausura_viae[CLAUSURA_MAXIMA][VIA_MAXIMA];
    SilvaComputusClausura  clausura[CLAUSURA_MAXIMA];
            SilvaComputus  c;
                      b32  sana;

    via      = NIHIL;
    nudum    = FALSUM;
    machina  = FALSUM;
    per (i = I; i < numerus_argumentorum; i++)
    {
        si (strcmp(argumenta[i], "-nudum") == ZEPHYRUM)
        {
            nudum = VERUM;
        }
        alioquin si (strcmp(argumenta[i], "-machina") == ZEPHYRUM)
        {
            machina = VERUM;
        }
        alioquin si (via == NIHIL)
        {
            via = argumenta[i];
        }
    }
    si (via == NIHIL)
    {
        fprintf(stderr,
            "usus: computus <plagula.c> [-nudum] [-machina]\n"
            "  -nudum    clausuram inclusionum NON praebere\n"
            "  -machina  ordo TSV (caput '#')\n");
        redde II;
    }

    piscina = piscina_generare_dynamicum("computus", MMMMXCVI * CCLVI);
    si (piscina == NIHIL)
    {
        fprintf(stderr, "computus: piscina non creata\n");
        redde II;
    }
    fons = filum_legere_totum(via, piscina);
    si (fons.mensura == ZEPHYRUM || fons.datum == NIHIL)
    {
        fprintf(stderr, "computus: plagula non lecta (aut vacua): %s\n",
            via);
        piscina_destruere(piscina);
        redde II;
    }

    numerus_clausurae = ZEPHYRUM;
    si (!nudum)
    {
        i32 k;

        numerus_clausurae = _clausuram_petere(via, clausura_viae);
        per (k = ZEPHYRUM; k < numerus_clausurae; k++)
        {
            chorda caput;

            caput = filum_legere_totum(clausura_viae[k], piscina);
            clausura[k].via = clausura_viae[k];
            clausura[k].textus = (constans character*)caput.datum;
            clausura[k].mensura = caput.mensura;
        }
    }

    sana = silva_computus_metiri(via, (constans character*)fons.datum,
        fons.mensura, numerus_clausurae > ZEPHYRUM ? clausura : NIHIL,
        numerus_clausurae, &c);

    si (machina)
    {
        _imprimere_machinam(via, &c);
    }
    alioquin
    {
        _imprimere_hominem(via, &c, numerus_clausurae);
    }
    piscina_destruere(piscina);
    redde sana ? ZEPHYRUM : I;
}
