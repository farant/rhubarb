/* fabrica.c - thistle -> proiectum in directorio dato (sine clang):
 *   fabrica <plagula.thistle> <directorium> [-fons <radix>]
 * Fons silicis: -fons, alioquin RHUBARB_RADIX, alioquin ascensus
 * (silex_fabricam_invenire). Exitus 0 scriptum | 1 recusatio/fractum
 * | 2 usus. Inventarium in stdout; recusatio in stderr cum linea.
 */

#include "latina.h"
#include "briar_arbor.h"
#include "briar_fabrica.h"
#include "briar_nexus.h"
#include "briar_silva.h"
#include "internamentum.h"
#include "piscina.h"
#include "silex.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

interior character*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura)
{
         FILE* f;
        longus longitudo;
    character* memoria;
        size_t lecti;

    f = fopen(via, "rb");
    si (f == NIHIL)
    {
        redde NIHIL;
    }
    si (fseek(f, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(f);
        redde NIHIL;
    }
    longitudo = ftell(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde NIHIL;
    }
    rewind(f);
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    lecti = fread(memoria, I, (size_t)longitudo, f);
    fclose(f);
    si (lecti != (size_t)longitudo)
    {
        redde NIHIL;
    }
    *mensura = (i32)longitudo;
    redde memoria;
}

s32
principale (
       integer   numerus_argumentorum,
     character** argumenta)
{
                Piscina* piscina;
    InternamentumChorda* intern;
     constans character* via          = NIHIL;
     constans character* directorium  = NIHIL;
     constans character* radix        = NIHIL;
     constans SilexFons* fons;
              character* textus;
                    i32  mensura = ZEPHYRUM;
           MateriaNodus* doc;
                    Xar* nexus;
    BriarFabricaOptiones optiones;
     BriarFabricaFructus fructus;
                  chorda octeti;
                  chorda causa;
                 integer i;

    per (i = I; i < numerus_argumentorum; i++)
    {
        si (   strcmp(argumenta[i], "-fons") == ZEPHYRUM
            && i + I < numerus_argumentorum)
        {
            radix  = argumenta[i + I];
            i      = i + I;
        }
        alioquin si (via == NIHIL)
        {
            via = argumenta[i];
        }
        alioquin si (directorium == NIHIL)
        {
            directorium = argumenta[i];
        }
    }
    si (via == NIHIL || directorium == NIHIL)
    {
        fprintf(stderr,
            "usus: fabrica <plagula.thistle> <directorium>"
            " [-fons <radix>]\n");
        redde II;
    }
    piscina  = piscina_generare_dynamicum("fabrica", 33554432);
    intern   = internamentum_creare(piscina);
    si (radix == NIHIL)
    {
        radix = getenv("RHUBARB_RADIX");
    }
    si (radix == NIHIL)
    {
        radix = silex_fabricam_invenire(piscina, ".");
    }
    fons = (radix != NIHIL) ? silex_fons_disci(piscina, radix) : NIHIL;
    si (fons == NIHIL)
    {
        fprintf(stderr,
            "fabrica: fons silicis non inventus (-fons <radix>)\n");
        redde I;
    }
    textus = _plagulam_legere(piscina, via, &mensura);
    si (textus == NIHIL)
    {
        fprintf(stderr, "fabrica: plagula non lecta: %s\n", via);
        redde I;
    }
    doc    = briar_arbor_parsare(piscina, textus, mensura);
    nexus  = briar_nexus_texere(piscina, doc, intern);
    si (   nexus == NIHIL || briar_silvam_texere(piscina, nexus, fons)
        < ZEPHYRUM)
    {
        fprintf(stderr, "fabrica: nexus fractus\n");
        redde I;
    }
    optiones.via_thistle   = via;
    optiones.stampa        = fons->titulus;
    optiones.fons_titulus  = "briar";
    octeti.datum           = (i8*)textus;
    octeti.mensura         = mensura;
    fructus = briar_fabricare(piscina, doc, nexus, fons, &optiones,
        octeti);
    si (!fructus.successus)
    {
        fprintf(stderr, "%s:%d: %.*s\n", via,
            (integer)fructus.linea_causae,
            (integer)fructus.causa.mensura,
            (constans character*)fructus.causa.datum);
        redde I;
    }
    si (!briar_fabricam_scribere(piscina, &fructus, directorium,
        &causa))
    {
        fprintf(stderr, "fabrica: %.*s\n", (integer)causa.mensura,
            (constans character*)causa.datum);
        redde I;
    }
    imprimere("fabrica: %s -> %s\n  forma %s  titulus %s  clavis %s\n"
        "  genitae %d  clausura %d  regiones C %d  probatio %s\n",
        via, directorium,
        fructus.forma == BRIAR_FORMA_VITREA ? "vitrea" : "plana",
        fructus.titulus, fructus.sigillum,
        (integer)xar_numerus(fructus.genitae),
        (integer)xar_numerus(fructus.clausura),
        (integer)fructus.regiones_c,
        fructus.probatio_adest ? "adest" : "abest");
    briar_silvam_solvere(nexus);
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
