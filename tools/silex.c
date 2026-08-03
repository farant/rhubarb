/* silex.c (instrumentum) - lapis ignarius: proiecta e fabrica
 * excudere. Verbum unicum v0: novum. Vide include/silex.h.
 *
 * Usus:
 *   silex novum 001 -f /via/ad/rhubarb -d /via/ad/silicetum
 *   SILEX_FABRICA=/via/ad/rhubarb silex novum 001
 */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "argumenta.h"
#include "silex.h"

#include <stdio.h>
#include <stdlib.h>

s32
principale (integer argc, character** argv)
{
    Piscina*            piscina;
    ArgumentaParser*    parser;
    ArgumentaFructus*   lecta;
    chorda              verbum;
    chorda              titulus;
    chorda              fabrica_opt;
    chorda              destinatio_opt;
    constans character* fabrica;
    constans character* destinatio;
    SilexNovumOptiones  optiones;
    SilexNovumFructus   fructus;

    piscina = piscina_generare_dynamicum("silex", 65536);
    si (piscina == NIHIL)
    {
        fprintf(stderr, "silex: piscina generari non potuit\n");
        redde I;
    }

    parser = argumenta_creare(piscina);
    argumenta_ponere_descriptionem(parser,
        "silex - proiecta nova e fabrica rhubarb excudere");
    argumenta_addere_positionalem(parser, "verbum",
        "verbum (v0: novum)", VERUM);
    argumenta_addere_positionalem(parser, "titulus",
        "nomen proiecti", VERUM);
    argumenta_addere_optionem(parser, "-f", "--fabrica",
        "radix arboris rhubarb (aut SILEX_FABRICA)");
    argumenta_addere_optionem(parser, "-d", "--destinatio",
        "directorium parens proiecti (ordinarie '.')");
    argumenta_addere_exemplum(parser,
        "silex novum 001 -f ~/Documents/projects/rhubarb");

    lecta = argumenta_conari_parsere(parser, (i32)argc,
        (constans character* constans*)argv);
    si (lecta == NIHIL)
    {
        argumenta_imprimere_errorem(parser);
        argumenta_imprimere_auxilium(parser);
        redde I;
    }

    verbum = argumenta_obtinere_positionalem(lecta, 0, piscina);
    titulus = argumenta_obtinere_positionalem(lecta, 1, piscina);

    si (!chorda_aequalis_literis(verbum, "novum"))
    {
        fprintf(stderr, "silex: verbum ignotum: %.*s"
            " (v0 solum 'novum' novit)\n",
            (integer)verbum.mensura,
            (constans character*)verbum.datum);
        redde I;
    }

    fabrica_opt = argumenta_obtinere_optionem(lecta, "--fabrica",
        piscina);
    si (fabrica_opt.mensura > ZEPHYRUM)
    {
        fabrica = chorda_ut_cstr(fabrica_opt, piscina);
    }
    alioquin
    {
        fabrica = getenv("SILEX_FABRICA");
        si (fabrica == NIHIL || fabrica[0] == '\0')
        {
            fprintf(stderr, "silex: fabrica ignota - da --fabrica"
                " aut SILEX_FABRICA pone\n");
            redde I;
        }
    }

    destinatio_opt = argumenta_obtinere_optionem(lecta,
        "--destinatio", piscina);
    si (destinatio_opt.mensura > ZEPHYRUM)
    {
        destinatio = chorda_ut_cstr(destinatio_opt, piscina);
    }
    alioquin
    {
        destinatio = ".";
    }

    optiones.fabrica = fabrica;
    optiones.destinatio = destinatio;
    optiones.titulus = chorda_ut_cstr(titulus, piscina);

    fructus = silex_novum(piscina, &optiones);
    si (!fructus.successus)
    {
        fprintf(stderr, "silex novum: FRACTUM - %s\n",
            fructus.erratum == NIHIL ? "causa ignota"
                : fructus.erratum);
        redde I;
    }

    imprimere("silex novum: %s excusum\n", optiones.titulus);
    imprimere("  volumen:    %s (veritas)\n", fructus.volumen_via);
    imprimere("  vendicatae: %d plagulae (e %s)\n",
        (integer)fructus.vendicatae, fabrica);
    imprimere("  genitae:    %d plagulae\n",
        (integer)fructus.genitae);
    imprimere("  deinde:     cd %s/%s && ./aedificare.sh &&"
        " ./probare.sh\n", destinatio, optiones.titulus);

    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
