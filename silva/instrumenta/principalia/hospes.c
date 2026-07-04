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
    silva_sceletum_ambiguum_fabricare,
    NULL
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

    /* --- ADDITIONES II: strata + fenestrae lectionis --- */
    {
        SilvaExpansio* exp = silva_expansio_creare(piscina);
        const char* FONS =
            "#define GEMINARE(x) ((x)+(x))\n"
            "#if 0\nint a;\n#else\nint b;\n#endif\n"
            "GEMINARE(2);\n";
        SilvaParsura* parsura = NULL;

        if (exp != NULL)
        {
            parsura = silva_parsare_cum_expansione(piscina, exp,
                "additiones.c", FONS, (unsigned int)strlen(FONS),
                &GRAMMATICA_SCELETI, NULL, NULL, NULL);
        }

        /* strata: GEMINARE mutat -> saltem unum stratum;
         * strata[ultimum] ALIASES lexemata */
        summa++;
        if (parsura != NULL && parsura->strata != NULL
            && silva_xar_numerus(parsura->strata) >= 1
            && *(SilvaXar**)silva_xar_obtinere(parsura->strata,
                   silva_xar_numerus(parsura->strata) - 1)
               == parsura->lexemata)
        {
            fideles++;
        }
        else
        {
            fprintf(stderr, "hospes: INFIDELIS: strata\n");
        }

        /* fontes: princeps habet viam; extra fines -> NULL */
        summa++;
        if (parsura != NULL
            && silva_fontes_numerus(exp) >= 1
            && silva_fons_via(exp, parsura->fons_princeps) != NULL
            && silva_fons_via(exp, 9999) == NULL)
        {
            fideles++;
        }
        else
        {
            fprintf(stderr, "hospes: INFIDELIS: fons_via\n");
        }

        /* rami: #if 0 / #else -> II rami; primus numquam nec
         * sumptus, secundus sumptus; extenta OCTETI >= 0 */
        summa++;
        if (parsura != NULL && silva_rami_numerus(exp) == 2)
        {
            SilvaRamusVista primus;
            SilvaRamusVista secundus;

            if (silva_ramus_vista(exp, 0, &primus)
                && silva_ramus_vista(exp, 1, &secundus)
                && primus.genus == SILVA_RAMUS_IF
                && !primus.est_sumptum && primus.est_numquam
                && secundus.genus == SILVA_RAMUS_ELSE
                && secundus.est_sumptum
                && secundus.corpus_initium >= 0
                && secundus.corpus_finis > secundus.corpus_initium)
            {
                fideles++;
            }
            else
            {
                fprintf(stderr, "hospes: INFIDELIS: rami vista\n");
            }
        }
        else
        {
            fprintf(stderr, "hospes: INFIDELIS: rami numerus\n");
        }

        /* macros: GEMINARE definitio functionis */
        summa++;
        if (parsura != NULL && silva_macros_numerus(exp) == 1)
        {
            SilvaMacroVista vista;

            if (silva_macro_vista(exp, 0, &vista)
                && vista.est_functio
                && vista.titulus != NULL
                && vista.titulus->mensura == 8
                && memcmp(vista.titulus->datum, "GEMINARE", 8) == 0)
            {
                fideles++;
            }
            else
            {
                fprintf(stderr, "hospes: INFIDELIS: macro vista\n");
            }
        }
        else
        {
            fprintf(stderr,
                "hospes: INFIDELIS: macros numerus\n");
        }

        /* inclusio vista: fixum multi-fons supra iam parsavit; hic
         * exp NOSTRUM inclusiones nullas habet */
        summa++;
        {
            SilvaInclusioVista vista;

            if (silva_inclusiones_numerus(exp) == 0
                && !silva_inclusio_vista(exp, 0, &vista))
            {
                fideles++;
            }
            else
            {
                fprintf(stderr,
                    "hospes: INFIDELIS: inclusiones\n");
            }
        }
    }

    /* grammatica c89 (M2a): furca typedef per amalgama - lectiones
     * ambae retentae, octeti exacti. Custos declarationum silva.h
     * (regula hospitis: omnis functio nova vocatur eodem mutatu). */
    {
        static const SilvaGrammatica GRAMMATICA_C89 = {
            &SILVA_C89_TABULA,
            &SILVA_C89_REGISTRUM,
            silva_c89_construere,
            silva_c89_ambiguum_fabricare,
            NULL
        };
        const char* fons_c89 = "(foo)(x);";
        SilvaParsura* parsura;

        summa++;
        parsura = silva_parsare(piscina, "hospes_c89.c", fons_c89,
            (unsigned int)strlen(fons_c89), &GRAMMATICA_C89,
            NULL, NULL, NULL);
        if (parsura != NULL && parsura->successus
            && parsura->numerus_errorum == 0
            && silva_xar_numerus(parsura->commissio->ambigui) == 1)
        {
            SilvaScriptura scriptura = silva_scribere_fontem(piscina,
                parsura, &SILVA_C89_REGISTRUM, parsura->fons_princeps);

            if (scriptura.successus
                && scriptura.textus.mensura
                    == (unsigned int)strlen(fons_c89)
                && memcmp(scriptura.textus.datum, fons_c89,
                       strlen(fons_c89)) == 0)
            {
                fideles++;
            }
            else
            {
                fprintf(stderr, "hospes: INFIDELIS: c89 octeti\n");
            }
        }
        else
        {
            fprintf(stderr, "hospes: INFIDELIS: c89 furca\n");
        }
    }

    /* saltatio oraculi c89 (M2b): vocatio una per amalgama -
     * custos declarationum novarum silva.h (situ, ambulator,
     * involucrum) */
    {
        const char* fons_c89b = "typedef int foo; foo * bar;";
        SilvaOraculum* oraculum = silva_oraculum_creare(piscina);
        SilvaParsura* parsura;
        int bene = 0;

        summa++;
        parsura = (oraculum != NULL)
            ? silva_c89_parsare(piscina, "hospes_c89.c", fons_c89b,
                  (unsigned int)strlen(fons_c89b), oraculum)
            : NULL;
        if (parsura != NULL && parsura->successus
            && parsura->numerus_errorum == 0
            && silva_xar_numerus(parsura->commissio->ambigui) == 0)
        {
            static unsigned char foo_litterae[] = { 'f', 'o', 'o' };
            SilvaChorda quaesitum;
            int situs = -1;

            quaesitum.mensura = 3;
            quaesitum.datum = foo_litterae;
            /* typus didicitur positionaliter; situs primus servatur */
            if (silva_oraculum_situs_typi(oraculum, quaesitum, &situs)
                && situs > 0
                && silva_oraculum_typum_addere_situ(oraculum,
                       quaesitum, 9999)
                && silva_oraculum_situs_typi(oraculum, quaesitum,
                       &situs)
                && situs > 0 && situs < 9999)
            {
                /* ambulator declaratoris: elementum[1] declaratio,
                 * declaratores (locus [1]) -> primus nodus ->
                 * titulus "bar" */
                SilvaValor* e = silva_valor_lista_obtinere(
                    parsura->commissio->radix, 1);

                if (e != NULL && e->genus == SILVA_VALOR_NODUS
                    && e->datum.nodus->numerus_locorum >= 2)
                {
                    SilvaValor decll = e->datum.nodus->loci[1];
                    SilvaValor* d = silva_valor_lista_obtinere(
                        decll, 0);
                    SilvaToken* titulus = (d != NULL
                            && d->genus == SILVA_VALOR_NODUS)
                        ? silva_c89_declaratoris_titulus(
                              d->datum.nodus)
                        : NULL;

                    if (titulus != NULL
                        && titulus->valor.mensura == 3
                        && memcmp(titulus->valor.datum, "bar", 3)
                            == 0)
                    {
                        bene = 1;
                    }
                }
            }
        }
        if (bene)
        {
            fideles++;
        }
        else
        {
            fprintf(stderr, "hospes: INFIDELIS: saltatio c89\n");
        }

        /* vista declarationum per amalgama */
        summa++;
        if (parsura != NULL
            && silva_c89_declarationes_numerus(parsura) == 2)
        {
            SilvaDeclaratioVista vista;

            if (silva_c89_declaratio_vista(parsura, 1, &vista)
                && vista.titulus.mensura == 3
                && memcmp(vista.titulus.datum, "bar", 3) == 0
                && vista.genus != NULL)
            {
                fideles++;
            }
            else
            {
                fprintf(stderr, "hospes: INFIDELIS: vista ordo\n");
            }
        }
        else
        {
            fprintf(stderr, "hospes: INFIDELIS: vista numerus\n");
        }
    }

    /* vista FUNCTIONES + subscriptio per amalgama (M2c C) */
    {
        static const char fons_func[] =
            "int quaestum(int a) { return a; }";
        SilvaParsura* parsura = silva_c89_parsare(piscina,
            "hospes_func.c", fons_func,
            (unsigned int)(sizeof(fons_func) - 1), NULL);

        summa++;
        if (parsura != NULL
            && silva_c89_declarationes_numerus(parsura) == 1)
        {
            SilvaDeclaratioVista vista;
            SilvaScriptura subscriptio;

            subscriptio = silva_c89_functionis_subscriptio(
                piscina, parsura, 0);
            if (silva_c89_declaratio_vista(parsura, 0, &vista)
                && vista.titulus.mensura == 8
                && memcmp(vista.titulus.datum, "quaestum", 8) == 0
                && subscriptio.successus
                && subscriptio.textus.mensura == 20
                && memcmp(subscriptio.textus.datum,
                       "int quaestum(int a) ", 20) == 0)
            {
                fideles++;
            }
            else
            {
                fprintf(stderr,
                    "hospes: INFIDELIS: functionis ordo\n");
            }
        }
        else
        {
            fprintf(stderr,
                "hospes: INFIDELIS: functionis numerus\n");
        }
    }

    /* c89 cum contextu latino per amalgama (M2d A): fons latinus
     * expanditur, definitio functionis vera fit */
    {
        static const char fons_lat[] =
            "interior int quaesitum_lat(int a) { redde a + 1; }";
        SilvaContextus* ctx = silva_contextus_creare(piscina);
        SilvaParsura* parsura = NULL;

        summa++;
        if (ctx != NULL && silva_contextus_latinam_addere(ctx))
        {
            parsura = silva_c89_parsare_cum_contextu(piscina, ctx,
                "hospes_lat.c", fons_lat,
                (unsigned int)(sizeof(fons_lat) - 1), NULL);
        }
        if (parsura != NULL && parsura->numerus_errorum == 0
            && silva_c89_declarationes_numerus(parsura) == 1)
        {
            SilvaDeclaratioVista vista;

            if (silva_c89_declaratio_vista(parsura, 0, &vista)
                && vista.titulus.mensura == 13
                && memcmp(vista.titulus.datum, "quaesitum_lat",
                       13) == 0
                && vista.genus != NULL
                && strcmp(vista.genus, "definitio-functionis")
                    == 0)
            {
                fideles++;
            }
            else
            {
                fprintf(stderr,
                    "hospes: INFIDELIS: c89 latina ordo\n");
            }
        }
        else
        {
            fprintf(stderr,
                "hospes: INFIDELIS: c89 latina parsura\n");
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
