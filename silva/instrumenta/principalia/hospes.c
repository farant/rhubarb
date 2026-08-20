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

/* (GRAMMATICA_SCELETI retired M2d Chunk B - the c89 grammar is
 * the amalgam's single grammar; sceletum stays in-repo as the
 * generator fixture only) */

static int
fidelis(SilvaPiscina* piscina, const char* fons)
{
    SilvaParsura* parsura;
    SilvaScriptura scriptura;
    unsigned int mensura = (unsigned int)strlen(fons);

    parsura = silva_parsare(piscina, "hospes.c", fons, mensura,
        &SILVA_C89_GRAMMATICA, NULL, NULL, NULL);
    if (parsura == NULL || !parsura->successus)
    {
        return 0;
    }
    scriptura = silva_scribere_fontem(piscina, parsura,
        &SILVA_C89_REGISTRUM, parsura->fons_princeps);
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

/* initiator declaratoris primi declarationis radicis[index]
 * (navigatio M0b/M1a communis) */
static const SilvaNodus*
initiator_indice(const SilvaParsura* parsura, unsigned int index)
{
    SilvaValor* decl;
    SilvaValor ds;
    SilvaValor* d0;
    SilvaValor iv;

    if (parsura == NULL || parsura->commissio == NULL)
    {
        return NULL;
    }
    decl = silva_valor_lista_obtinere(parsura->commissio->radix,
        index);
    if (decl == NULL || decl->genus != SILVA_VALOR_NODUS)
    {
        return NULL;
    }
    ds = silva_c89_declaratio_declaratores(decl->datum.nodus);
    d0 = silva_valor_lista_obtinere(ds, 0);
    if (d0 == NULL || d0->genus != SILVA_VALOR_NODUS)
    {
        return NULL;
    }
    iv = silva_c89_declarator_initiatus_initiator(d0->datum.nodus);
    if (iv.genus != SILVA_VALOR_NODUS)
    {
        return NULL;
    }
    return iv.datum.nodus;
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
                &SILVA_C89_GRAMMATICA, NULL, NULL, NULL);
            if (parsura != NULL && parsura->successus)
            {
                scriptura = silva_scribere_fontem(piscina, parsura,
                    &SILVA_C89_REGISTRUM, parsura->fons_princeps);
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
            (unsigned int)strlen(FONS), &SILVA_C89_GRAMMATICA, NULL,
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
                &SILVA_C89_GRAMMATICA, NULL, NULL, NULL);
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

        /* corpus macronis lexematim: ((x)+(x)) = IX lexemata,
         * tertium identificator "x"; extra fines -> NULL */
        summa++;
        {
            SilvaToken* tertium = silva_macro_corpus_lexema(exp,
                0, 2);

            if (silva_macro_corpus_numerus(exp, 0) == 9
                && tertium != NULL
                && tertium->genus == SILVA_LEX_IDENTIFICATOR
                && tertium->valor.mensura == 1
                && tertium->valor.datum[0] == 'x'
                && silva_macro_corpus_lexema(exp, 0, 9999) == NULL
                && silva_macro_corpus_numerus(exp, 9999) == 0)
            {
                fideles++;
            }
            else
            {
                fprintf(stderr,
                    "hospes: INFIDELIS: corpus macronis\n");
            }
        }

        /* lamina rami omissi lexata: "int a;" identificator "a"
         * inest; ramus sumptus lexemata cruda nulla */
        summa++;
        {
            unsigned int m = silva_ramus_lexemata_numerus(exp, 0);
            unsigned int j;
            int a_inventum = 0;

            for (j = 0; j < m; j++)
            {
                SilvaToken* tok = silva_ramus_lexema_crudum(exp,
                    0, j);

                if (tok != NULL
                    && tok->genus == SILVA_LEX_IDENTIFICATOR
                    && tok->valor.mensura == 1
                    && tok->valor.datum[0] == 'a')
                {
                    a_inventum = 1;
                }
            }
            if (m > 0 && a_inventum
                && silva_ramus_lexemata_numerus(exp, 1) == 0
                && silva_ramus_lexema_crudum(exp, 0, 9999) == NULL)
            {
                fideles++;
            }
            else
            {
                fprintf(stderr,
                    "hospes: INFIDELIS: lamina rami omissi\n");
            }
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

        /* est_angulata: forma inclusionis servata (01KY118F1J) -
         * citata 0, angulata 1; irresolutae aeque memorantur */
        summa++;
        {
            SilvaExpansio* exp2 = silva_expansio_creare(piscina);
            const char* FONS_INCL =
                "#include \"a.h\"\n#include <b.h>\nint c;\n";
            SilvaParsura* p2 = NULL;
            SilvaInclusioVista citata;
            SilvaInclusioVista angulata;

            if (exp2 != NULL)
            {
                p2 = silva_parsare_cum_expansione(piscina, exp2,
                    "anguli.c", FONS_INCL,
                    (unsigned int)strlen(FONS_INCL),
                    &SILVA_C89_GRAMMATICA, NULL, NULL, NULL);
            }
            if (p2 != NULL && silva_inclusiones_numerus(exp2) == 2
                && silva_inclusio_vista(exp2, 0, &citata)
                && silva_inclusio_vista(exp2, 1, &angulata)
                && !citata.est_angulata
                && angulata.est_angulata)
            {
                fideles++;
            }
            else
            {
                fprintf(stderr,
                    "hospes: INFIDELIS: est_angulata\n");
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

    /* accessores generum generati (superficies publica): OMNES
     * vocantur (regula hospitis) - genus-probati et NULL-tuti:
     * accessor cum NULL vocatus valorem generis SILVA_VALOR_NIHIL
     * reddere debet. Tabula inter signa GENERATUR (generare.sh). */
    {
        typedef SilvaValor (*SilvaHospitisAccessor)(
            const SilvaNodus*);
        /* >>> GENERATUM (silva_coquere): superficies publica SILVA_C89 >>> */
        static const SilvaHospitisAccessor ACCESSORES[176] = {
            silva_c89_definitio_functionis_specificatores,
            silva_c89_definitio_functionis_declarator,
            silva_c89_definitio_functionis_declarationes_kr,
            silva_c89_definitio_functionis_corpus,
            silva_c89_sententia_vacua_tok_terminator,
            silva_c89_corpus_tok_aperta,
            silva_c89_corpus_elementa,
            silva_c89_corpus_tok_clausa,
            silva_c89_si_tok_verbum,
            silva_c89_si_tok_apertum,
            silva_c89_si_conditio,
            silva_c89_si_tok_clausum,
            silva_c89_si_consequens,
            silva_c89_si_tok_alioquin,
            silva_c89_si_alioquin,
            silva_c89_dum_tok_verbum,
            silva_c89_dum_tok_apertum,
            silva_c89_dum_conditio,
            silva_c89_dum_tok_clausum,
            silva_c89_dum_corpus,
            silva_c89_fac_dum_tok_fac,
            silva_c89_fac_dum_corpus,
            silva_c89_fac_dum_tok_dum,
            silva_c89_fac_dum_tok_apertum,
            silva_c89_fac_dum_conditio,
            silva_c89_fac_dum_tok_clausum,
            silva_c89_fac_dum_tok_terminator,
            silva_c89_per_tok_verbum,
            silva_c89_per_tok_apertum,
            silva_c89_per_clausula,
            silva_c89_per_tok_clausum,
            silva_c89_per_corpus,
            silva_c89_per_clausula_initium,
            silva_c89_per_clausula_tok_terminator_i,
            silva_c89_per_clausula_conditio,
            silva_c89_per_clausula_tok_terminator_ii,
            silva_c89_per_clausula_passus,
            silva_c89_commutatio_tok_verbum,
            silva_c89_commutatio_tok_apertum,
            silva_c89_commutatio_discrimen,
            silva_c89_commutatio_tok_clausum,
            silva_c89_commutatio_corpus,
            silva_c89_casus_tok_verbum,
            silva_c89_casus_valor,
            silva_c89_casus_tok_colon,
            silva_c89_casus_sententiae,
            silva_c89_ordinarius_tok_verbum,
            silva_c89_ordinarius_tok_colon,
            silva_c89_ordinarius_sententiae,
            silva_c89_titulatum_tok_titulus,
            silva_c89_titulatum_tok_colon,
            silva_c89_titulatum_sententia,
            silva_c89_salta_tok_verbum,
            silva_c89_salta_tok_destinatio,
            silva_c89_salta_tok_terminator,
            silva_c89_frange_tok_verbum,
            silva_c89_frange_tok_terminator,
            silva_c89_perge_tok_verbum,
            silva_c89_perge_tok_terminator,
            silva_c89_redde_tok_verbum,
            silva_c89_redde_valor,
            silva_c89_redde_tok_terminator,
            silva_c89_declaratio_specificatores,
            silva_c89_declaratio_declaratores,
            silva_c89_declaratio_tok_terminator,
            silva_c89_typus_nominatus_tok_titulus,
            silva_c89_typus_primitivus_tok_verba,
            silva_c89_declarator_initiatus_declarator,
            silva_c89_declarator_initiatus_tok_operator,
            silva_c89_declarator_initiatus_initiator,
            silva_c89_congeries_tok_aperta,
            silva_c89_congeries_elementa,
            silva_c89_congeries_tok_clausa,
            silva_c89_structura_tok_verbum,
            silva_c89_structura_tok_titulus,
            silva_c89_structura_tok_aperta,
            silva_c89_structura_membra,
            silva_c89_structura_tok_clausa,
            silva_c89_unio_tok_verbum,
            silva_c89_unio_tok_titulus,
            silva_c89_unio_tok_aperta,
            silva_c89_unio_membra,
            silva_c89_unio_tok_clausa,
            silva_c89_membrum_specificatores,
            silva_c89_membrum_declaratores,
            silva_c89_membrum_tok_terminator,
            silva_c89_campus_declarator,
            silva_c89_campus_tok_colon,
            silva_c89_campus_latitudo,
            silva_c89_enumeratio_tok_verbum,
            silva_c89_enumeratio_tok_titulus,
            silva_c89_enumeratio_tok_aperta,
            silva_c89_enumeratio_enumeratores,
            silva_c89_enumeratio_tok_clausa,
            silva_c89_enumerator_tok_titulus,
            silva_c89_enumerator_tok_operator,
            silva_c89_enumerator_valor,
            silva_c89_declarator_monstrator_tok_stella,
            silva_c89_declarator_monstrator_qualificatores,
            silva_c89_declarator_monstrator_internum,
            silva_c89_declarator_titulus_tok_titulus,
            silva_c89_parenthesis_tok_apertum,
            silva_c89_parenthesis_internum,
            silva_c89_parenthesis_tok_clausum,
            silva_c89_declarator_aciei_internum,
            silva_c89_declarator_aciei_tok_apertum,
            silva_c89_declarator_aciei_mensura,
            silva_c89_declarator_aciei_tok_clausum,
            silva_c89_declarator_functionis_internum,
            silva_c89_declarator_functionis_tok_apertum,
            silva_c89_declarator_functionis_parametra,
            silva_c89_declarator_functionis_tok_clausum,
            silva_c89_parametrum_specificatores,
            silva_c89_parametrum_declarator,
            silva_c89_sententia_expressionis_expressio,
            silva_c89_sententia_expressionis_tok_terminator,
            silva_c89_virgula_sinister,
            silva_c89_virgula_tok_operator,
            silva_c89_virgula_dexter,
            silva_c89_assignatio_sinister,
            silva_c89_assignatio_tok_operator,
            silva_c89_assignatio_dexter,
            silva_c89_ternarius_conditio,
            silva_c89_ternarius_tok_quaestio,
            silva_c89_ternarius_verum,
            silva_c89_ternarius_tok_colon,
            silva_c89_ternarius_falsum,
            silva_c89_binarium_sinister,
            silva_c89_binarium_tok_operator,
            silva_c89_binarium_dexter,
            silva_c89_conversio_tok_apertum,
            silva_c89_conversio_typus,
            silva_c89_conversio_tok_clausum,
            silva_c89_conversio_internum,
            silva_c89_unarium_tok_operator,
            silva_c89_unarium_internum,
            silva_c89_magnitudo_expressionis_tok_verbum,
            silva_c89_magnitudo_expressionis_internum,
            silva_c89_magnitudo_typi_tok_verbum,
            silva_c89_magnitudo_typi_tok_apertum,
            silva_c89_magnitudo_typi_typus,
            silva_c89_magnitudo_typi_tok_clausum,
            silva_c89_species_typi_specificatores,
            silva_c89_species_typi_declarator,
            silva_c89_declarator_abstractus_tok_stella,
            silva_c89_declarator_abstractus_qualificatores,
            silva_c89_declarator_abstractus_internum,
            silva_c89_subscriptio_basis,
            silva_c89_subscriptio_tok_apertum,
            silva_c89_subscriptio_index,
            silva_c89_subscriptio_tok_clausum,
            silva_c89_vocatio_functio,
            silva_c89_vocatio_tok_apertum,
            silva_c89_vocatio_argumenta,
            silva_c89_vocatio_tok_clausum,
            silva_c89_accessus_basis,
            silva_c89_accessus_tok_operator,
            silva_c89_accessus_tok_titulus,
            silva_c89_postcrementum_internum,
            silva_c89_postcrementum_tok_operator,
            silva_c89_folium_identificator_tok_valor,
            silva_c89_folium_integer_tok_valor,
            silva_c89_folium_fluitans_tok_valor,
            silva_c89_folium_character_tok_valor,
            silva_c89_folium_chorda_tok_valor,
            silva_c89_error_tokens,
            silva_c89_ambiguus_interpretationes,
            silva_c89_ambiguus_canonica,
            silva_c89_conditionalis_rami,
            silva_c89_conditionalis_finis,
            silva_c89_ramus_sumptus_directiva,
            silva_c89_ramus_sumptus_contentum,
            silva_c89_ramus_sumptus_conditio_id,
            silva_c89_ramus_omissus_directiva,
            silva_c89_ramus_omissus_cruda,
            silva_c89_ramus_omissus_conditio_id
        };
        /* <<< GENERATUM (silva_coquere): finis superficiei publicae SILVA_C89 <<< */
        int k_acc;
        int bene_acc = 1;

        summa++;
        for (k_acc = 0;
             k_acc < (int)(sizeof(ACCESSORES)
                 / sizeof(ACCESSORES[0]));
             k_acc++)
        {
            if (ACCESSORES[k_acc] == NULL
                || ACCESSORES[k_acc](NULL).genus
                    != SILVA_VALOR_NIHIL)
            {
                bene_acc = 0;
            }
        }
        if (bene_acc)
        {
            fideles++;
        }
        else
        {
            fprintf(stderr, "hospes: INFIDELIS: accessores\n");
        }
    }

    /* quaestio (QA clausura): compilare/exsequi/congruit per
     * amalgama - custos declarationum silva.h (regula hospitis) */
    {
        static const char fons_q[] =
            "int a;\n"
            "int quadratum(int x) { int y; y = x * x; return y; }\n";
        SilvaParsura* parsura = silva_c89_parsare(piscina,
            "hospes_q.c", fons_q,
            (unsigned int)(sizeof(fons_q) - 1), NULL);
        const char* causa = NULL;
        SilvaQuaestio* q_def = silva_quaestio_compilare(piscina,
            &SILVA_C89_REGISTRUM, "definitio-functionis", &causa);
        SilvaQuaestio* q_decl = silva_quaestio_compilare(piscina,
            &SILVA_C89_REGISTRUM, "corpus > declaratio", &causa);
        int bene_q = 0;

        summa++;
        if (parsura != NULL && parsura->numerus_errorum == 0
            && q_def != NULL && q_decl != NULL)
        {
            SilvaXar* rr = silva_quaestio_exsequi(q_def,
                parsura->commissio->radix, piscina);
            SilvaXar* rd = silva_quaestio_exsequi(q_decl,
                parsura->commissio->radix, piscina);

            if (rr != NULL && silva_xar_numerus(rr) == 1
                && rd != NULL && silva_xar_numerus(rd) == 1)
            {
                SilvaQuaestioResultatum* r =
                    (SilvaQuaestioResultatum*)silva_xar_obtinere(
                        rr, 0);

                if (r != NULL && r->nodus != NULL
                    && silva_quaestio_congruit(q_def, r->nodus)
                    && !silva_quaestio_congruit(q_decl, r->nodus))
                {
                    bene_q = 1;
                }
            }
        }
        if (bene_q)
        {
            fideles++;
        }

        else
        {
            fprintf(stderr, "hospes: INFIDELIS: quaestio\n");
        }

        /* QB: attributum + captura omnium graduum + frater - per
         * amalgama (custos SilvaQuaestioCaptura in silva.h) */
        summa++;
        {
            const char* causa_b = NULL;
            SilvaQuaestio* q_attr = silva_quaestio_compilare(
                piscina, &SILVA_C89_REGISTRUM,
                "definitio-functionis$f"
                " declarator-titulus[tok_titulus=\"y\"]$t",
                &causa_b);
            SilvaQuaestio* q_frater = silva_quaestio_compilare(
                piscina, &SILVA_C89_REGISTRUM,
                "declaratio + sententia-expressionis", &causa_b);
            int bene_b = 0;

            if (parsura != NULL && q_attr != NULL
                && q_frater != NULL)
            {
                SilvaXar* ra = silva_quaestio_exsequi(q_attr,
                    parsura->commissio->radix, piscina);
                SilvaXar* rf = silva_quaestio_exsequi(q_frater,
                    parsura->commissio->radix, piscina);

                if (ra != NULL && silva_xar_numerus(ra) == 1
                    && rf != NULL && silva_xar_numerus(rf) == 1)
                {
                    SilvaQuaestioResultatum* r =
                        (SilvaQuaestioResultatum*)
                        silva_xar_obtinere(ra, 0);

                    if (r != NULL && r->capturae != NULL
                        && silva_xar_numerus(r->capturae) == 2)
                    {
                        SilvaQuaestioCaptura* cap =
                            (SilvaQuaestioCaptura*)
                            silva_xar_obtinere(r->capturae, 1);

                        if (cap != NULL && cap->nodus == r->nodus
                            && cap->titulus.mensura == 1
                            && cap->titulus.datum != NULL
                            && cap->titulus.datum[0] == 't')
                        {
                            bene_b = 1;
                        }
                    }
                }
            }
            if (bene_b)
            {
                fideles++;
            }
            else
            {
                fprintf(stderr,
                    "hospes: INFIDELIS: quaestio-qb\n");
            }
        }

        /* fractura clara: tag ignotum = NULL + causa */
        summa++;
        causa = NULL;
        if (silva_quaestio_compilare(piscina, &SILVA_C89_REGISTRUM,
                "nodus-inexistens", &causa) == NULL
            && causa != NULL)
        {
            fideles++;
        }
        else
        {
            fprintf(stderr,
                "hospes: INFIDELIS: quaestio fractura\n");
        }
    }

    /* quaestiones nominatae (2026-07-31): bibliotheca selectorum -
     * legere/numerus/ad_indicem/invenire/parare per amalgama
     * (regula hospitis: omnis functio nova vocatur) */
    {
        static char fons_bib[] =
            "<quaestiones>"
            "<quaestio titulus=\"vocantes\" gradus=\"oculi\">"
            "<selector>definitio-functionis:vocat($functio)"
            "</selector>"
            "<parametrum titulus=\"functio\"/>"
            "<causa>hospes</causa></quaestio>"
            "</quaestiones>";
        static const char fons_qc[] =
            "int quadratum(int x) { return x * x; }\n"
            "int bis(int x) { return quadratum(x); }\n";
        SilvaChorda fons;
        SilvaChorda culpa;
        SilvaQuaestiones* bibliotheca;
        int bene_qn = 0;

        summa++;
        fons.datum = (unsigned char*)fons_bib;
        fons.mensura = (unsigned int)(sizeof(fons_bib) - 1);
        bibliotheca = silva_quaestiones_legere(piscina,
            &SILVA_C89_REGISTRUM, NULL, fons, &culpa);
        if (bibliotheca != NULL
            && silva_quaestiones_numerus(bibliotheca) == 1
            && silva_quaestiones_ad_indicem(bibliotheca, 0) != NULL
            && silva_quaestiones_ad_indicem(bibliotheca, 1) == NULL)
        {
            const SilvaQuaestioNominata* nominata =
                silva_quaestiones_invenire(bibliotheca, "vocantes");
            SilvaParsura* parsura = silva_c89_parsare(piscina,
                "hospes_qn.c", fons_qc,
                (unsigned int)(sizeof(fons_qc) - 1), NULL);

            if (nominata != NULL
                && nominata->gradus == SILVA_QUAESTIONES_OCULI
                && parsura != NULL)
            {
                /* series plana - forma quam hospes aedificare
                 * potest (Xar amalgamatis legendus solum) */
                static char t_functio[] = "functio";
                static char v_quadratum[] = "quadratum";
                SilvaQuaestionesArgumentum argumenta[1];
                SilvaQuaestio* quaestio;

                argumenta[0].titulus.datum =
                    (unsigned char*)t_functio;
                argumenta[0].titulus.mensura = 7;
                argumenta[0].valor.datum =
                    (unsigned char*)v_quadratum;
                argumenta[0].valor.mensura = 9;
                quaestio = silva_quaestiones_parare(piscina,
                    bibliotheca, nominata, argumenta, 1, &culpa);
                if (quaestio != NULL)
                {
                    SilvaXar* resultata = silva_quaestio_exsequi(
                        quaestio, parsura->commissio->radix,
                        piscina);

                    if (resultata != NULL
                        && silva_xar_numerus(resultata) == 1)
                    {
                        bene_qn = 1;
                    }
                }
            }
        }
        if (bene_qn)
        {
            fideles++;
        }
        else
        {
            fprintf(stderr,
                "hospes: INFIDELIS: quaestiones nominatae\n");
        }
    }

    /* semantica M0a (publica Chunk D): analysis, index, typi,
     * forma, oraculum trivalens + clausura - custos declarationum
     * silva.h (regula hospitis: omnis functio nova vocatur) */
    {
        /* x declaratum - typatio M0b expressiones examinat et
         * ignotos diagnosticat (assertum ==0 infra) */
        static const char fons_sem[] =
            "typedef int Aetas;\n"
            "enum Color { RUBER, VIRIDIS = 5 };\n"
            "static int x;\n"
            "static long y = 1 + 2;\n"
            "static int foo(int a) { return a; }\n"
            "(foo)(x);\n"
            "static const char* verba = \"ab\\ncd\" \"!\";\n"
            "static long z = y + 1;\n";
        SilvaOraculum* oraculum = silva_oraculum_creare(piscina);
        SilvaParsura* parsura = silva_c89_parsare(piscina,
            "hospes_sem.c", fons_sem,
            (unsigned int)(sizeof(fons_sem) - 1), oraculum);
        SilvaSemantica* sem = NULL;
        int bene_sem = 0;

        summa++;
        if (parsura != NULL)
        {
            sem = silva_c89_semantica_analysare_cum_systemate(
                piscina, parsura, NULL);
        }
        if (sem != NULL
            && silva_c89_semantica_analysare(piscina, parsura)
                != NULL)
        {
            static unsigned char aetas_litterae[] =
                { 'A', 'e', 't', 'a', 's' };
            SilvaChorda quaesitum;
            SemanticaSymbolum* symbolum;

            quaesitum.mensura = 5;
            quaesitum.datum = aetas_litterae;
            symbolum = silva_c89_symbolum_invenire(sem, quaesitum);
            if (symbolum != NULL
                && symbolum->genus == (int)SYMBOLUM_TYPEDEF
                && symbolum->est_implicitum == 0
                && symbolum->typus == silva_c89_typus_primitivum(
                       sem, (int)PRIMITIVUM_INTEGER)
                && silva_c89_formam_computare(sem, symbolum->typus)
                && symbolum->typus->magnitudo_octetorum == 4
                && symbolum->typus->ordinatio == 4
                && silva_c89_symbola_numerus(sem) >= 4
                && silva_c89_symbolum_per_indicem(sem, 0) != NULL
                && silva_c89_diagnostica_numerus(sem) == 0
                && silva_c89_diagnosticum_per_indicem(sem, 0)
                    == NULL)
            {
                /* clausura: (foo)(x) furca; foo notum non-typus
                 * post augere -> nihil indecisum */
                (void)silva_c89_semantica_oraculum_augere(sem,
                    oraculum);
                silva_oraculum_responsa_vacare(oraculum);  /* M1a */
                if (parsura->commissio != NULL
                    && silva_c89_ambigua_indecisa_numerare(
                           parsura->commissio, oraculum) == 0)
                {
                    /* typatio M0b (rows novae vocatae): initiator
                     * "1 + 2" -> int, conversus long (ad finem) */
                    SilvaValor* decl_y = silva_valor_lista_obtinere(
                        parsura->commissio->radix, 3);
                    const SilvaNodus* initiator = NULL;

                    if (decl_y != NULL
                        && decl_y->genus == SILVA_VALOR_NODUS)
                    {
                        SilvaValor ds =
                            silva_c89_declaratio_declaratores(
                                decl_y->datum.nodus);
                        SilvaValor* d0 = silva_valor_lista_obtinere(
                            ds, 0);

                        if (d0 != NULL
                            && d0->genus == SILVA_VALOR_NODUS)
                        {
                            SilvaValor iv =
                                silva_c89_declarator_initiatus_initiator(
                                    d0->datum.nodus);

                            if (iv.genus == SILVA_VALOR_NODUS)
                            {
                                initiator = iv.datum.nodus;
                            }
                        }
                    }
                    if (initiator != NULL
                        && silva_c89_typus_expressionis(sem,
                               initiator)
                            == silva_c89_typus_primitivum(sem,
                                   (int)PRIMITIVUM_INTEGER)
                        && silva_c89_conversio_expressionis(sem,
                               initiator)
                            == silva_c89_typus_primitivum(sem,
                                   (int)PRIMITIVUM_LONGUS)
                        && silva_c89_typationes_numerus(sem) > 0)
                    {
                        /* M1a Chunk A: exporta demissionis -
                         * symbolum_nodi (usus y in "y + 1"),
                         * nexus_numerus, constans_aestimare
                         * ("1 + 2" -> 3), chorda_decodere
                         * ("ab\ncd" "!" -> 6 octeti) */
                        const SilvaNodus* init_verba =
                            initiator_indice(parsura, 6);
                        const SilvaNodus* init_z =
                            initiator_indice(parsura, 7);
                        const SemanticaSymbolum* usus_y = NULL;
                        long long valor_y = 0;
                        SilvaChorda octeti;

                        if (init_z != NULL)
                        {
                            SilvaValor sv =
                                silva_c89_binarium_sinister(init_z);

                            if (sv.genus == SILVA_VALOR_NODUS)
                            {
                                usus_y = silva_c89_symbolum_nodi(
                                    sem, sv.datum.nodus);
                            }
                        }
                        if (usus_y != NULL
                            && usus_y->genus
                                == (int)SYMBOLUM_VARIABILE
                            && usus_y->titulus.mensura == 1
                            && usus_y->titulus.datum[0] == 'y'
                            && silva_c89_nexus_numerus(sem) > 0
                            && silva_c89_constans_aestimare(sem,
                                   initiator, &valor_y)
                            && valor_y == 3
                            && init_verba != NULL
                            && silva_c89_chorda_decodere(piscina,
                                   init_verba, &octeti)
                            && octeti.mensura == 6
                            && memcmp(octeti.datum, "ab\ncd!", 6)
                                == 0)
                        {
                            bene_sem = 1;
                        }
                    }
                }
            }
        }
        if (bene_sem)
        {
            fideles++;
        }
        else
        {
            fprintf(stderr, "hospes: INFIDELIS: semantica\n");
        }
    }

    /* examen M4a chunk A: diagnosticum v2 - identificator ignotus
     * figitur, ordines novi leguntur (codex/severitas/positio/
     * provisionale/socius; via ex parsura ambulationis) */
    {
        static const char fons_ex[] =
            "static int q = mysterium;\n"
            "static int* pi; static long* pl;\n";
        SilvaParsura* parsura_ex = silva_c89_parsare(piscina,
            "hospes_examen.c", fons_ex,
            (unsigned int)(sizeof(fons_ex) - 1), NULL);
        int bene_ex = 0;

        summa++;
        if (parsura_ex != NULL)
        {
            SilvaSemantica* sem_ex =
                silva_c89_semantica_analysare(piscina, parsura_ex);

            if (sem_ex != NULL
                && silva_c89_diagnostica_numerus(sem_ex) == 1)
            {
                const SemanticaDiagnosticum* d =
                    silva_c89_diagnosticum_per_indicem(sem_ex, 0);

                if (d != NULL
                    && d->codex
                        == (int)EXAMEN_CODEX_IDENTIFICATOR_IGNOTUS
                    && d->severitas == (int)EXAMEN_VIOLATIO
                    && d->provisionale == 0
                    && d->linea == 1
                    && d->columna == 16
                    && d->via.mensura > 0
                    && d->causa != NULL
                    && d->socius == NULL
                    && d->nodus != NULL)
                {
                    bene_ex = 1;
                }
            }
        }
        if (bene_ex)
        {
            fideles++;
        }
        else
        {
            fprintf(stderr, "hospes: INFIDELIS: examen\n");
        }

        /* chunk B: relatio + iudicium (ordines novi vocantur) */
        summa++;
        if (parsura_ex != NULL)
        {
            SilvaSemantica* sem_rel =
                silva_c89_semantica_analysare(piscina, parsura_ex);
            int bene_rel = 0;

            if (sem_rel != NULL)
            {
                static unsigned char pi_lit[] = { 'p', 'i' };
                static unsigned char pl_lit[] = { 'p', 'l' };
                SilvaChorda q_pi;
                SilvaChorda q_pl;
                TypusC89* t_int = silva_c89_typus_primitivum(
                    sem_rel, (int)PRIMITIVUM_INTEGER);
                TypusC89* t_longus = silva_c89_typus_primitivum(
                    sem_rel, (int)PRIMITIVUM_LONGUS);
                SemanticaSymbolum* s_pi;
                SemanticaSymbolum* s_pl;
                TypusC89* p_int = NULL;
                TypusC89* p_longus = NULL;
                int codex_rel = -1;

                q_pi.mensura = 2; q_pi.datum = pi_lit;
                q_pl.mensura = 2; q_pl.datum = pl_lit;
                s_pi = silva_c89_symbolum_invenire(sem_rel, q_pi);
                s_pl = silva_c89_symbolum_invenire(sem_rel, q_pl);
                if (s_pi != NULL) { p_int = s_pi->typus; }
                if (s_pl != NULL) { p_longus = s_pl->typus; }

                if (p_int != NULL && p_longus != NULL
                    && silva_c89_typi_compatibiles(t_int, t_int)
                    && !silva_c89_typi_compatibiles(t_int, t_longus)
                    && silva_c89_assignationem_iudicare(sem_rel,
                           NULL, t_int, t_longus, NULL)
                        == (int)EXAMEN_LICET_CONVERSIO
                    && silva_c89_assignationem_iudicare(sem_rel,
                           NULL, p_int, p_longus, &codex_rel)
                        == (int)EXAMEN_VETITUM
                    && codex_rel
                        == (int)EXAMEN_CODEX_MONSTRATORES_INCOMPATIBILES
                    && silva_c89_codicis_causa(codex_rel) != NULL
                    && silva_c89_codicis_severitas(codex_rel)
                        == (int)EXAMEN_VIOLATIO
                    && silva_c89_codicis_severitas(-1)
                        == (int)EXAMEN_INFRA)
                {
                    bene_rel = 1;
                }
            }
            if (bene_rel)
            {
                fideles++;
            }
            else
            {
                fprintf(stderr, "hospes: INFIDELIS: relatio\n");
            }
        }
        else
        {
            fprintf(stderr, "hospes: INFIDELIS: relatio\n");
        }
    }

    /* extensio fontis + redditor typorum (LEGATUS chunk 0) */
    {
        int minimum = -1;
        int maximum = 0;
        unsigned int la = 9u;
        unsigned int ca = 9u;
        unsigned int lb = 9u;
        unsigned int cb = 9u;
        char buffer[64];
        SilvaValor valor_nihil;

        valor_nihil.genus = SILVA_VALOR_NIHIL;
        valor_nihil.datum.nodus = NULL;
        silva_nodus_extensionem(NULL, 0, &minimum, &maximum);
        silva_valor_extensionem(valor_nihil, 0, &minimum, &maximum);
        silva_nodus_extensionem_lineis(NULL, 0, &la, &ca, &lb, &cb);
        summa++;
        if (minimum < 0 && maximum == 0
            && la == 0u && ca == 0u && lb == 0u && cb == 0u
            && silva_c89_typum_scribere(NULL, buffer, 64u) == 0u)
        {
            fideles++;
        }
        else
        {
            fprintf(stderr, "hospes: INFIDELIS: extensio\n");
        }
    }

    /* annotationes STML (chunk A, 2026-07-21): omnis functio nova
     * silva.h vocatur - colligere/unitates + subcopia stml legendi
     * + internamentum (regula stans ADDITIONES) */
    {
        static const char* FONS_ANN =
            "/* <intentio nid>\n"
            " *   <causa>quia hospes probat</causa>\n"
            " * </intentio> */\n"
            "int a;\n"
            "int b;\n";
        static unsigned char STML_TAMPON[] = "<radix v=\"x\"/>";
        SilvaInternamentumChorda* intern =
            silva_internamentum_creare(piscina);
        SilvaParsura* parsura = silva_c89_parsare(piscina,
            "hospes.c", FONS_ANN, (unsigned int)strlen(FONS_ANN),
            NULL);
        int bene_ann = 0;

        summa++;
        if (intern != NULL && parsura != NULL && parsura->successus)
        {
            SilvaXar* annotationes = silva_annotationes_colligere(
                piscina, parsura, intern);
            SilvaXar* unitates = silva_annotationes_unitates(
                piscina, parsura);

            if (annotationes != NULL && unitates != NULL
                && silva_xar_numerus(annotationes) == 1u
                && silva_xar_numerus(unitates) == 2u)
            {
                SilvaAnnotatio* an = (SilvaAnnotatio*)
                    silva_xar_obtinere(annotationes, 0u);

                if (an != NULL && an->parsata
                    && an->modus == SILVA_ANNOTATIO_SUPRA
                    && an->unitas != NULL
                    && an->arbor != NULL
                    && an->arbor->genus == STML_NODUS_ELEMENTUM
                    && silva_stml_attributum_habet(an->arbor, "nid")
                    && silva_stml_attributum_capere(an->arbor,
                           "nid") != NULL
                    && silva_stml_numerus_liberorum(an->arbor) > 0u
                    && silva_stml_liberum_ad_indicem(an->arbor, 0u)
                        != NULL)
                {
                    SilvaStmlNodus* causa =
                        silva_stml_invenire_liberum(an->arbor,
                            "causa");

                    if (causa != NULL)
                    {
                        SilvaChorda textus =
                            silva_stml_textus_internus(causa,
                                piscina);

                        if (textus.mensura
                                == (unsigned int)strlen(
                                       "quia hospes probat")
                            && memcmp(textus.datum,
                                   "quia hospes probat",
                                   textus.mensura) == 0)
                        {
                            SilvaChorda input;
                            SilvaStmlResultus r;

                            input.mensura = (unsigned int)strlen(
                                (const char*)STML_TAMPON);
                            input.datum = STML_TAMPON;
                            r = silva_stml_legere(input, piscina,
                                intern);
                            if (r.successus
                                && r.elementum_radix != NULL
                                && r.status == STML_SUCCESSUS)
                            {
                                bene_ann = 1;
                            }
                        }
                    }
                }
            }
        }
        if (bene_ann)
        {
            fideles++;
        }
        else
        {
            fprintf(stderr, "hospes: INFIDELIS: annotationes\n");
        }
    }

    /* ARBOR: circuitus STML per silva.h (M1).
     *
     * NON ornamentum. silva.c caput silva.h NON includit, ergo
     * discrepantia inter prototypa manu scripta et corpora
     * amalgamata a NULLA compilatione capitur - nisi quis eas PER
     * CAPUT vocet et nectat. Hoc facit id. Sine hoc, superficies
     * arboris in silva.h declarata sed a nemine probata esset:
     * classis 'porta tacita = porta mortua'. */
    {
        const char*     fons = "int n = 0;\n";
        SilvaParsura*   parsura;
        SilvaNodus*     origo = NULL;
        SilvaNodus*     lecta;
        SilvaChorda     sigillum;
        SilvaArborScriptura   scriptura;
        SilvaArborVitium      vitium;
        SilvaArborDifferentia differentia;
        int             bene_arbor = 0;

        summa++;
        parsura = silva_parsare(piscina, "hospes-arbor.c", fons,
            (unsigned int)strlen(fons), &SILVA_C89_GRAMMATICA,
            NULL, NULL, NULL);
        if (parsura != NULL && parsura->commissio != NULL)
        {
            SilvaValor radix = parsura->commissio->radix;
            unsigned int i;
            unsigned int quot = silva_valor_lista_numerus(radix);

            for (i = 0; i < quot; i++)
            {
                SilvaValor* elem = silva_valor_lista_obtinere(radix, i);

                if (elem != NULL && elem->genus == SILVA_VALOR_NODUS)
                {
                    origo = elem->datum.nodus;
                    break;
                }
            }
        }

        /* sigillum: forma VIII characterum */
        sigillum = silva_arbor_sigillum(piscina, &SILVA_C89_REGISTRUM);

        if (origo != NULL
            && sigillum.mensura == SILVA_ARBOR_SIGILLI_LONGITUDO)
        {
            scriptura = silva_arbor_scribere_nodum(piscina, origo,
                &SILVA_C89_REGISTRUM, "c89", parsura->expansio, NULL);
            if (scriptura.successus)
            {
                lecta = silva_arbor_legere(piscina, NULL,
                    scriptura.textus, &SILVA_C89_REGISTRUM, "c89",
                    &vitium);
                if (lecta != NULL
                    && silva_arbor_aequalis(origo, lecta,
                           SILVA_ARBOR_COMPARATIO_FIDELITAS,
                           &differentia))
                {
                    bene_arbor = 1;
                }
                else if (lecta == NULL)
                {
                    fprintf(stderr, "hospes: arbor recusata: %s\n",
                        vitium.causa ? vitium.causa : "(nulla)");
                }
                else
                {
                    fprintf(stderr, "hospes: arbor divergens: %s\n",
                        differentia.campus ? differentia.campus
                                           : "(nullus)");
                }
            }
            else
            {
                fprintf(stderr, "hospes: arbor scriptura: %s\n",
                    scriptura.causa ? scriptura.causa : "(nulla)");
            }
        }

        if (bene_arbor)
        {
            fideles++;
        }
        else
        {
            fprintf(stderr, "hospes: INFIDELIS: arbor circuitus\n");
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
