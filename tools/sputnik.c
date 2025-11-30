/* sputnik.c - Interpres Lineae Mandati pro Sputnik
 *
 * Instrumentum pro executione filorum .sputnik ex linea mandati.
 *
 * USUS:
 *   sputnik <filum.sputnik>
 *   sputnik -e "print(2 + 2);"
 *   sputnik --db data.db script.sputnik
 *   sputnik --help
 */

#include "latina.h"
#include "sputnik_interpres.h"
#include "argumenta.h"
#include "filum.h"
#include "piscina.h"
#include "chorda.h"
#include "persistentia.h"
#include "entitas_repositorium.h"
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
_executare_fontem(
    chorda fons,
    Piscina* piscina,
    InternamentumChorda* intern,
    EntitasRepositorium* repositorium)
{
    SputnikInterpresResultus resultus;

    si (repositorium != NIHIL)
    {
        resultus = sputnik_evaluare_ex_chorda_cum_repositorio(
            fons, piscina, intern, repositorium);
    }
    alioquin
    {
        resultus = sputnik_evaluare_ex_chorda(fons, piscina, intern);
    }

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
    chorda db_via;
    chorda fons;
    integer exitus;
    Persistentia* persistentia;
    EntitasRepositorium* repositorium;

    /* Creare piscina */
    piscina = piscina_generare_dynamicum("sputnik", 1024 * 256);
    si (piscina == NIHIL)
    {
        fprintf(stderr, "Error: Non potest creare piscinam\n");
        redde I;
    }

    /* Usare internamentum globale ut repositorium et interpres
     * dividant eandem piscinam internamenti */
    intern = internamentum_globale();
    persistentia = NIHIL;
    repositorium = NIHIL;

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
    argumenta_addere_optionem(parser, "-d", "--db",
        "Via ad filum databasei entitatum");
    argumenta_addere_positionalem(parser, "filum",
        "Via ad filum .sputnik", FALSUM);

    argumenta_addere_exemplum(parser, "sputnik script.sputnik");
    argumenta_addere_exemplum(parser, "sputnik -e \"print(2 + 2);\"");
    argumenta_addere_exemplum(parser, "sputnik --db data.db script.sputnik");

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

    /* Tractare --db */
    db_via = argumenta_obtinere_optionem(fructus, "--db", piscina);
    si (db_via.mensura > ZEPHYRUM)
    {
        character* db_via_c;

        /* Convertere ad C string */
        db_via_c = piscina_allocare(piscina, (memoriae_index)(db_via.mensura + I));
        si (db_via_c == NIHIL)
        {
            fprintf(stderr, "Error: Memoria exhausta\n");
            piscina_destruere(piscina);
            redde I;
        }
        memcpy(db_via_c, db_via.datum, (size_t)db_via.mensura);
        db_via_c[db_via.mensura] = '\0';

        /* Aperire vel creare database */
        si (filum_existit(db_via_c))
        {
            persistentia = persistentia_nuntium_aperire(piscina, db_via_c);
        }
        alioquin
        {
            persistentia = persistentia_nuntium_creare(piscina, db_via_c);
        }

        si (persistentia == NIHIL)
        {
            fprintf(stderr, "Error: Non potest aperire database: %s\n", db_via_c);
            piscina_destruere(piscina);
            redde I;
        }

        repositorium = entitas_repositorium_creare(piscina, persistentia);
        si (repositorium == NIHIL)
        {
            fprintf(stderr, "Error: Non potest creare repositorium\n");
            piscina_destruere(piscina);
            redde I;
        }
    }

    /* Tractare -e / --eval */
    expressio = argumenta_obtinere_optionem(fructus, "--eval", piscina);
    si (expressio.mensura > ZEPHYRUM)
    {
        exitus = _executare_fontem(expressio, piscina, intern, repositorium);

        /* Cleanup */
        si (persistentia != NIHIL)
        {
            persistentia->sync(persistentia->datum);
            persistentia->claudere(persistentia->datum);
        }
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
    exitus = _executare_fontem(fons, piscina, intern, repositorium);

    /* Cleanup */
    si (persistentia != NIHIL)
    {
        persistentia->sync(persistentia->datum);
        persistentia->claudere(persistentia->datum);
    }
    piscina_destruere(piscina);
    redde exitus;
}
