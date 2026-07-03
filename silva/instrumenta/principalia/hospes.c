/* hospes.c - The first true host of the silva deliverable.
 *
 * VANILLA C89 ON PURPOSE: this file includes ONLY silva.h and the
 * standard library - no latina.h, no rhubarb headers. It proves two
 * things at once (Phase 6 Chunk C):
 *
 *   1. HOST POLLUTION (S43): variables named after latina keyword
 *      macros (si, per, character, nomen, structura, redde, dum...)
 *      compile and WORK below - if silva.h leaked a single latina
 *      define, this file would not build.
 *
 *   2. EQUIVALENCE SMOKE: the amalgamated library parses and emits
 *      byte-identically to the modular build's proven behavior -
 *      fixtures below mirror probatio_silva_scribere cases (macro
 *      expansion boundary included).
 *
 * Compiled BY amalgamare.sh against amalgama/silva.c (two TUs),
 * then run; exit 0 = all checks pass.
 */
#include <stdio.h>
#include <string.h>
#include "silva.h"

/* --- S43: latina keyword names as ordinary identifiers --- */
static int si = 1;
static double per = 2.0;
static char character = 'c';
static int nomen = 3;
static int structura = 4;
static int redde = 5;
static int dum = 6;
static int vacuum = 7;

static const SilvaGrammatica GRAMMATICA_SCELETI = {
    &SILVA_SCELETUM_TABULA,
    &SILVA_SCELETUM_REGISTRUM,
    silva_sceletum_construere,
    silva_sceletum_ambiguum_fabricare
};

static int
fidelis(SilvaPiscina* piscina, const char* fons)
{
    SilvaParsura* parsura;
    SilvaScriptura scriptura;
    unsigned int mensura = (unsigned int)strlen(fons);

    parsura = silva_parsare(piscina, "hospes.c", fons, mensura,
        &GRAMMATICA_SCELETI, NULL, NULL, NULL);
    if (parsura == NULL || !parsura->successus)
    {
        return 0;
    }
    scriptura = silva_scribere_fontem(piscina, parsura,
        &SILVA_SCELETUM_REGISTRUM, parsura->fons_princeps);
    if (!scriptura.successus || scriptura.textus.mensura != mensura)
    {
        return 0;
    }
    if (mensura > 0
        && memcmp(scriptura.textus.datum, fons, mensura) != 0)
    {
        return 0;
    }
    return 1;
}

int main(void)
{
    SilvaPiscina* piscina;
    int fideles = 0;
    int summa = 0;

    /* the polluted names must actually work */
    si += nomen;
    per += (double)structura;
    character = (char)(character + 1);
    redde += dum + vacuum;
    if (si != 4 || character != 'd' || redde != 18)
    {
        fprintf(stderr, "hospes: nomina latina corrupta?!\n");
        return 1;
    }

    piscina = silva_piscina_generare_dynamicum("hospes", 4194304);
    if (piscina == NULL)
    {
        fprintf(stderr, "hospes: piscina deest\n");
        return 1;
    }

    /* equivalence fixtures (mirror probatio_silva_scribere) */
    {
        static const char* FIXA[] = {
            "int x;",
            "int  x ;\n    x + 1;\n",
            "foo * bar;",
            "#define T int\nT x;",
            "#define ADDERE(a, b) a + b\nADDERE(1, 2);",
            "#if 0\nint a;\n#else\nint b;\n#endif\n",
            "@ $ garbage &&& ;;;"
        };
        int k;

        for (k = 0; k < (int)(sizeof(FIXA) / sizeof(FIXA[0])); k++)
        {
            summa++;
            if (fidelis(piscina, FIXA[k]))
            {
                fideles++;
            }
            else
            {
                fprintf(stderr, "hospes: INFIDELIS: %s\n", FIXA[k]);
            }
        }
    }

    /* multi-fons: include praebere'd through the public API */
    {
        SilvaExpansio* exp = silva_expansio_creare(piscina);
        const char* MODULUS =
            "#ifndef MODULUS_H\n#define MODULUS_H\nint m;\n#endif\n";
        const char* FONS = "#include \"modulus.h\"\nint x;\n";
        SilvaParsura* parsura;
        SilvaScriptura scriptura;

        summa++;
        if (exp != NULL
            && silva_includendum_praebere(exp, "modulus.h", MODULUS,
                   (unsigned int)strlen(MODULUS)) >= 0)
        {
            parsura = silva_parsare_cum_expansione(piscina, exp,
                "hospes.c", FONS, (unsigned int)strlen(FONS),
                &GRAMMATICA_SCELETI, NULL, NULL, NULL);
            if (parsura != NULL && parsura->successus)
            {
                scriptura = silva_scribere_fontem(piscina, parsura,
                    &SILVA_SCELETUM_REGISTRUM, parsura->fons_princeps);
                if (scriptura.successus
                    && scriptura.textus.mensura
                        == (unsigned int)strlen(FONS)
                    && memcmp(scriptura.textus.datum, FONS,
                           strlen(FONS)) == 0)
                {
                    fideles++;
                }
            }
        }
    }

    /* --- ADDITIONES I: every new silva.h declaration is CALLED -
     * the ONLY gate that can catch a declaration/definition type
     * mismatch (C links without mangling; the standalone compile
     * never sees silva.h). STANDING RULE: every function silva.h
     * gains, gets a call here in the same change. --- */
    {
        const char* FONS = "si x; /* c */ redde";
        unsigned int mensura = (unsigned int)strlen(FONS);
        SilvaXar* lexemata;
        SilvaXar* cruda;
        SilvaToken* primum = NULL;

        /* lexare: 5 lexemata vera (si x ; redde EOF) */
        summa++;
        lexemata = silva_lexare(piscina, FONS, mensura, 0);
        if (lexemata != NULL && silva_xar_numerus(lexemata) == 5)
        {
            primum = *(SilvaToken**)silva_xar_obtinere(lexemata, 0);
            if (primum->genus == SILVA_LEX_IDENTIFICATOR
                && primum->valor.mensura == 2
                && primum->linea == 1)
            {
                fideles++;
            }
            else
            {
                fprintf(stderr, "hospes: INFIDELIS: lexare genus\n");
            }
        }
        else
        {
            fprintf(stderr, "hospes: INFIDELIS: lexare numerus\n");
        }

        /* cruda: trivia (commentum, spatia) IN fluxu */
        summa++;
        cruda = silva_lexare_cruda(piscina, FONS, mensura, 0);
        if (cruda != NULL && lexemata != NULL
            && silva_xar_numerus(cruda)
                > silva_xar_numerus(lexemata))
        {
            fideles++;
        }
        else
        {
            fprintf(stderr, "hospes: INFIDELIS: cruda trivia\n");
        }

        /* auxilia catenae originis in lexemate fontis */
        summa++;
        if (primum != NULL
            && silva_token_radix(primum) == primum
            && silva_token_profunditas(primum) == 0
            && silva_token_est_fons(primum))
        {
            fideles++;
        }
        else
        {
            fprintf(stderr, "hospes: INFIDELIS: auxilia catenae\n");
        }

        /* tituli generum */
        summa++;
        if (primum != NULL
            && silva_lexema_genus_nomen(primum->genus) != NULL
            && primum->origo.genus == SILVA_ORIGO_FONS
            && silva_origo_genus_nomen(primum->origo.genus) != NULL)
        {
            fideles++;
        }
        else
        {
            fprintf(stderr, "hospes: INFIDELIS: tituli generum\n");
        }
    }

    /* liberi nodales in arbore parsata */
    {
        const char* FONS = "int x; int y;";
        SilvaParsura* parsura;

        summa++;
        parsura = silva_parsare(piscina, "hospes.c", FONS,
            (unsigned int)strlen(FONS), &GRAMMATICA_SCELETI, NULL,
            NULL, NULL);
        if (parsura != NULL && parsura->successus
            && parsura->commissio != NULL
            && silva_valor_lista_numerus(
                   parsura->commissio->radix) >= 1)
        {
            SilvaValor* elementum = silva_valor_lista_obtinere(
                parsura->commissio->radix, 0);

            /* segmentum sceleti: liberi nodales vacui esse possunt
             * (loci = lexemata) - vocatio ipsa est custos typorum;
             * xar validum sufficit */
            if (elementum != NULL
                && elementum->genus == SILVA_VALOR_NODUS
                && silva_nodus_liberi(piscina,
                       elementum->datum.nodus) != NULL)
            {
                fideles++;
            }
            else
            {
                fprintf(stderr, "hospes: INFIDELIS: liberi\n");
            }
        }
        else
        {
            fprintf(stderr, "hospes: INFIDELIS: liberi parsura\n");
        }
    }

    /* telemetria arenae */
    {
        size_t usus = silva_piscina_summa_usus(piscina);
        size_t apex = silva_piscina_summa_apex_usus(piscina);

        summa++;
        if (usus > 0 && apex >= usus)
        {
            fideles++;
        }
        else
        {
            fprintf(stderr, "hospes: INFIDELIS: telemetria\n");
        }
    }

    printf("hospes: %d/%d fideles (pollutio nulla)\n", fideles, summa);
    silva_piscina_destruere(piscina);
    return (fideles == summa) ? 0 : 1;
}
