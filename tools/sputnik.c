/* sputnik.c - Interpres Lineae Mandati pro Sputnik
 *
 * Instrumentum pro executione filorum .sputnik ex linea mandati.
 *
 * USUS:
 *   sputnik <filum.sputnik>
 *   sputnik -e "print(2 + 2);"
 *   sputnik --help
 */

#include "latina.h"
#include "sputnik_interpres.h"
#include "argumenta.h"
#include "filum.h"
#include "piscina.h"
#include "chorda.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


interior vacuum
_imprimere_valorem(SputnikValor* valor, Piscina* piscina)
{
    chorda s;

    s = sputnik_valor_ad_chordam(valor, piscina);
    si (s.mensura > ZEPHYRUM)
    {
        printf("%.*s\n", s.mensura, s.datum);
    }
}


interior integer
_executare_fontem(chorda fons, Piscina* piscina, InternamentumChorda* intern)
{
    SputnikInterpresResultus resultus;

    resultus = sputnik_evaluare_ex_chorda(fons, piscina, intern);

    si (!resultus.successus)
    {
        fprintf(stderr, "Error");
        si (resultus.error_linea > ZEPHYRUM)
        {
            fprintf(stderr, " [%d:%d]",
                resultus.error_linea,
                resultus.error_columna);
        }
        fprintf(stderr, ": %.*s\n",
            resultus.error_nuntius.mensura,
            resultus.error_nuntius.datum);
        redde I;
    }

    /* Imprimere valorem ultimum si non nihil */
    si (resultus.valor.genus != SPUTNIK_VALOR_NIHIL)
    {
        _imprimere_valorem(&resultus.valor, piscina);
    }

    redde ZEPHYRUM;
}


integer principale(integer argc, constans character* constans* argv)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    ArgumentaParser* parser;
    ArgumentaFructus* fructus;
    chorda filum_via;
    chorda expressio;
    chorda fons;
    integer exitus;

    /* Creare piscina */
    piscina = piscina_generare_dynamicum("sputnik", 1024 * 256);
    si (piscina == NIHIL)
    {
        fprintf(stderr, "Error: Non potest creare piscinam\n");
        redde I;
    }

    intern = internamentum_creare(piscina);

    /* Configurare parser argumentorum */
    parser = argumenta_creare(piscina);
    argumenta_ponere_descriptionem(parser,
        "sputnik - Interpres pro lingua Sputnik");

    argumenta_addere_vexillum(parser, "-h", "--help",
        "Ostendere auxilium");
    argumenta_addere_vexillum(parser, "-v", "--version",
        "Ostendere versionem");
    argumenta_addere_optionem(parser, "-e", "--eval",
        "Evaluare expressionem directe");
    argumenta_addere_positionalem(parser, "filum",
        "Via ad filum .sputnik", FALSUM);

    argumenta_addere_exemplum(parser, "sputnik script.sputnik");
    argumenta_addere_exemplum(parser, "sputnik -e \"print(2 + 2);\"");

    /* Parsere argumenta */
    fructus = argumenta_conari_parsere(parser, (i32)argc, argv);
    si (fructus == NIHIL)
    {
        argumenta_imprimere_errorem(parser);
        piscina_destruere(piscina);
        redde I;
    }

    /* Tractare --help */
    si (argumenta_habet_vexillum(fructus, "--help"))
    {
        argumenta_imprimere_auxilium(parser);
        piscina_destruere(piscina);
        redde ZEPHYRUM;
    }

    /* Tractare --version */
    si (argumenta_habet_vexillum(fructus, "--version"))
    {
        printf("sputnik v0.1.0\n");
        piscina_destruere(piscina);
        redde ZEPHYRUM;
    }

    /* Tractare -e / --eval */
    expressio = argumenta_obtinere_optionem(fructus, "--eval", piscina);
    si (expressio.mensura > ZEPHYRUM)
    {
        exitus = _executare_fontem(expressio, piscina, intern);
        piscina_destruere(piscina);
        redde exitus;
    }

    /* Tractare filum positionale */
    filum_via = argumenta_obtinere_positionalem(fructus, ZEPHYRUM, piscina);
    si (filum_via.mensura == ZEPHYRUM)
    {
        fprintf(stderr, "Error: Nullum filum vel expressio data\n");
        fprintf(stderr, "Usa: sputnik <filum.sputnik>\n");
        fprintf(stderr, "     sputnik -e \"expressio\"\n");
        fprintf(stderr, "     sputnik --help\n");
        piscina_destruere(piscina);
        redde I;
    }

    /* Verificare si filum existit */
    {
        /* Convertere chorda ad C string temporaneum */
        character* via_c;
        via_c = piscina_allocare(piscina, (memoriae_index)(filum_via.mensura + I));
        si (via_c == NIHIL)
        {
            fprintf(stderr, "Error: Memoria exhausta\n");
            piscina_destruere(piscina);
            redde I;
        }
        memcpy(via_c, filum_via.datum, (size_t)filum_via.mensura);
        via_c[filum_via.mensura] = '\0';

        si (!filum_existit(via_c))
        {
            fprintf(stderr, "Error: Filum non existit: %s\n", via_c);
            piscina_destruere(piscina);
            redde I;
        }

        /* Legere filum */
        fons = filum_legere_totum(via_c, piscina);
        si (fons.mensura == ZEPHYRUM)
        {
            /* Filum vacuum vel error */
            constans character* err;
            err = filum_error_recens();
            si (err != NIHIL)
            {
                fprintf(stderr, "Error: Non potest legere filum: %s\n", err);
                piscina_destruere(piscina);
                redde I;
            }
            /* Filum vacuum - nihil facere */
            piscina_destruere(piscina);
            redde ZEPHYRUM;
        }
    }

    /* Executare */
    exitus = _executare_fontem(fons, piscina, intern);

    piscina_destruere(piscina);
    redde exitus;
}
