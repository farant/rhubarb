/* nexus_ordines.c - ordines nexus ex analysi una (vide .h)
 *
 * Ex nexus_percursus.c levata (LEGATUS v0.1b); mores IDENTICI -
 * paritas octetim tsv = barra migrationis. AMBULATIO LATA servata
 * consulto (ordo emissionis = ordo tabulae). */

#include "latina.h"
#include "nexus_ordines.h"

#include "chorda.h"
#include "tabula_dispersa.h"
#include "xar.h"

#include <stdio.h>

constans character*
nexus_ordines_genus_titulus (int genus)
{
    commutatio (genus)
    {
        casus SYMBOLUM_VARIABILE:  redde "variabile";
        casus SYMBOLUM_FUNCTIO:    redde "functio";
        casus SYMBOLUM_TYPEDEF:    redde "typedef";
        casus SYMBOLUM_CONSTANS:   redde "constans";
        casus SYMBOLUM_PARAMETRUM: redde "parametrum";
        ordinarius:                redde "?";
    }
}

/* positio lexematis per radicem originis; viae sine '/' (copiae
 * basename capitum praebitorum) reiectae */
interior b32
_positionem_capere (constans SilvaParsura* parsura, SilvaToken* t,
    constans SilvaChorda** via_exitus,
    insignatus integer* linea_exitus,
    insignatus integer* columna_exitus)
{
    SilvaToken* radix;
    constans SilvaChorda* via;

    si (t == NIHIL || parsura->expansio == NIHIL)
    {
        redde FALSUM;
    }
    radix = silva_token_radix(t);
    si (radix == NIHIL || radix->byte_offset < ZEPHYRUM)
    {
        redde FALSUM;
    }
    via = silva_fons_via(parsura->expansio, radix->fons_index);
    si (via == NIHIL)
    {
        redde FALSUM;
    }
    {
        b32 habet_separatorem = FALSUM;
        s32 k;

        per (k = ZEPHYRUM; k < (s32)via->mensura; k++)
        {
            si (via->datum[k] == '/')
            {
                habet_separatorem = VERUM;
                frange;
            }
        }
        si (!habet_separatorem)
        {
            redde FALSUM;
        }
    }
    *via_exitus = via;
    *linea_exitus = radix->linea;
    *columna_exitus = radix->columna;
    redde VERUM;
}

/* sedes declarationum: omnia symbola TU (systema exclusa) */
interior vacuum
_symbola_fundere (constans SilvaParsura* parsura,
    constans SilvaSemantica* sem, NexusOrdinesReceptor receptor,
    vacuum* datum)
{
    insignatus integer numerus = silva_c89_symbola_numerus(sem);
    insignatus integer k;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        constans SemanticaSymbolum* symbolum =
            silva_c89_symbolum_per_indicem(sem, k);
        constans SilvaChorda* via;
        insignatus integer linea;
        insignatus integer columna;

        si (symbolum == NIHIL)
        {
            perge;
        }
        si (symbolum->ex_systemate)
        {
            perge;
        }
        si (symbolum->est_implicitum)
        {
            perge;
        }
        si (symbolum->lexema == NIHIL)
        {
            perge;
        }
        si (!_positionem_capere(parsura, symbolum->lexema, &via,
                &linea, &columna))
        {
            perge;
        }
        receptor(datum, &symbolum->titulus, "sedes",
            nexus_ordines_genus_titulus(symbolum->genus), via,
            linea, columna, symbolum->profunditas);
    }
}

/* sedes usus: folia identificatorum (vocati vocationum FOLIA
 * quoque - etiam impliciti; ambigui per canonicam sequuntur).
 * AMBULATIO LATA (series) - ordo emissionis pars contractus. */
interior vacuum
_usus_fundere (constans SilvaParsura* parsura,
    constans SilvaSemantica* sem, Piscina* effimera,
    NexusOrdinesReceptor receptor, vacuum* datum)
{
    Xar* series = xar_creare(effimera, (i32)magnitudo(SilvaValor));
    i32 cursor = ZEPHYRUM;

    si (series == NIHIL)
    {
        redde;
    }
    {
        SilvaValor* radix = (SilvaValor*)xar_addere(series);

        si (radix == NIHIL)
        {
            redde;
        }
        *radix = parsura->commissio->radix;
    }

    dum (cursor < xar_numerus(series))
    {
        SilvaValor v = *(SilvaValor*)xar_obtinere(series, cursor);

        cursor++;

        si (v.genus == SILVA_VALOR_LISTA)
        {
            insignatus integer k;

            per (k = ZEPHYRUM;
                 k < silva_valor_lista_numerus(v); k++)
            {
                SilvaValor* elem = silva_valor_lista_obtinere(v,
                    k);

                si (elem != NIHIL)
                {
                    SilvaValor* novus =
                        (SilvaValor*)xar_addere(series);

                    si (novus != NIHIL)
                    {
                        *novus = *elem;
                    }
                }
            }
            perge;
        }
        si (v.genus != SILVA_VALOR_NODUS || v.datum.nodus == NIHIL)
        {
            perge;
        }

        {
            constans SilvaNodus* nodus = v.datum.nodus;
            insignatus integer k;

            si (nodus->genus == (int)SILVA_C89_GENUS_AMBIGUUS)
            {
                SilvaValor interp =
                    silva_c89_ambiguus_interpretationes(nodus);
                SilvaValor canonica =
                    silva_c89_ambiguus_canonica(nodus);

                si (canonica.genus == SILVA_VALOR_INDEX)
                {
                    SilvaValor* lectio =
                        silva_valor_lista_obtinere(interp,
                            (insignatus integer)
                                canonica.datum.index);

                    si (lectio != NIHIL)
                    {
                        SilvaValor* novus =
                            (SilvaValor*)xar_addere(series);

                        si (novus != NIHIL)
                        {
                            *novus = *lectio;
                        }
                    }
                }
                perge;
            }
            si (nodus->genus == (int)SILVA_C89_GENUS_ERROR
                || nodus->genus
                    == (int)SILVA_C89_GENUS_RAMUS_OMISSUS)
            {
                perge;
            }

            si (nodus->genus
                == (int)SILVA_C89_GENUS_FOLIUM_IDENTIFICATOR)
            {
                constans SemanticaSymbolum* symbolum =
                    silva_c89_symbolum_nodi(sem, nodus);

                si (symbolum != NIHIL)
                {
                    SilvaValor tok =
                        silva_c89_folium_identificator_tok_valor(
                            nodus);

                    si (tok.genus == SILVA_VALOR_TOKEN
                        && tok.datum.token != NIHIL)
                    {
                        constans SilvaChorda* via;
                        insignatus integer linea;
                        insignatus integer columna;

                        si (_positionem_capere(parsura,
                                tok.datum.token, &via, &linea,
                                &columna))
                        {
                            receptor(datum, &symbolum->titulus,
                                "usus",
                                nexus_ordines_genus_titulus(
                                    symbolum->genus),
                                via, linea, columna,
                                symbolum->profunditas);
                        }
                    }
                }
            }

            per (k = ZEPHYRUM; k < nodus->numerus_locorum; k++)
            {
                SilvaValor* novus = (SilvaValor*)xar_addere(series);

                si (novus != NIHIL)
                {
                    *novus = nodus->loci[k];
                }
            }
        }
    }
}

/* ==================================================
 * Macra (LEGATUS v0.2): sedes definitionum ex vista actorum
 * (silva_macro_vista - nomen/genus/fons/linea; columna in
 * definitione irrecuperabilis, I scribitur); usus ex fluxu expanso
 * per radicem originis (provenientia vera, non verba). Usus
 * macrorum in latina.h definitorum EXCLUSI (verba reservata -
 * tertia pars omnium lexematum; exemplar exclusionum lexici
 * examinis). Sedes latinae MANENT (saltus definitionis).
 * ================================================== */

/* SilvaChorda -> chorda hospitis (clavis tabulae; datum in piscinam
 * parsurae spectat - vita fundendi sufficit) */
interior chorda
_chorda_hospitis (constans SilvaChorda* s)
{
    chorda c;

    c.mensura = (i32)s->mensura;
    c.datum = (i8*)s->datum;
    redde c;
}

/* via in "latina.h" desinit? (forma basename praebita ET forma
 * plena ambulationis directae "include/latina.h") */
interior b32
_via_latinae_est (constans SilvaChorda* via)
{
    constans character* suffixum = "latina.h";
    insignatus integer m = VIII;
    insignatus integer k;

    si (via == NIHIL || via->mensura < m)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < m; k++)
    {
        si (via->datum[via->mensura - m + k]
            != (insignatus character)suffixum[k])
        {
            redde FALSUM;
        }
    }
    si (via->mensura > m
        && via->datum[via->mensura - m - I] != '/')
    {
        redde FALSUM;
    }
    redde VERUM;
}

/* titulus macronis EXTIMI (in fonte scripti): catenam originis
 * ascendere; titulus ultimus ante radicem FONS = macro invocatum.
 * API = finis catenae. custos contra catenas corruptas.
 * (NB "nomen" ut variabile VETITUM - macro latinae = typedef) */
constans SilvaChorda*
nexus_ordines_titulus_macronis (SilvaToken* tok)
{
    SilvaToken* cur = tok;
    constans SilvaChorda* titulus = NIHIL;
    insignatus integer custos = ZEPHYRUM;

    dum (cur != NIHIL && custos < LXIV)
    {
        custos++;
        commutatio (cur->origo.genus)
        {
            casus SILVA_ORIGO_EXPANSIO:
                titulus = cur->origo.datum.expansio.nomen_macro;
                cur = cur->origo.datum.expansio.invocatio;
                frange;
            casus SILVA_ORIGO_PASTA:
                titulus = cur->origo.datum.pasta.nomen_macro;
                cur = cur->origo.datum.pasta.sinister;
                frange;
            casus SILVA_ORIGO_CHORDA:
                titulus =
                    cur->origo.datum.stringificatio.nomen_macro;
                cur = cur->origo.datum.stringificatio.primus;
                frange;
            casus SILVA_ORIGO_API:
                redde cur->origo.datum.api.nomen_macro;
            ordinarius:
                redde titulus;   /* FONS - finis catenae */
        }
    }
    redde titulus;
}

interior vacuum
_macros_fundere (constans SilvaParsura* parsura, Piscina* effimera,
    NexusOrdinesReceptor receptor, vacuum* datum)
{
    TabulaDispersa* latina_tituli;
    TabulaDispersa* usus_visi;
    insignatus integer numerus;
    insignatus integer k;

    si (parsura->expansio == NIHIL)
    {
        redde;
    }
    latina_tituli = tabula_dispersa_creare_chorda(effimera, CCLVI);
    usus_visi = tabula_dispersa_creare_chorda(effimera, CCLVI);
    si (latina_tituli == NIHIL || usus_visi == NIHIL)
    {
        redde;
    }

    /* I. sedes definitionum (acta = productum: redefinitiones =
     * ordines plures, quaeque sedes vera fontis) */
    numerus = silva_macros_numerus(parsura->expansio);
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        SilvaMacroVista vista;
        constans SilvaChorda* via;

        si (!silva_macro_vista(parsura->expansio, k, &vista)
            || vista.titulus == NIHIL)
        {
            perge;
        }
        via = silva_fons_via(parsura->expansio, vista.fons_index);
        si (via == NIHIL)
        {
            perge;
        }
        si (_via_latinae_est(via))
        {
            /* notare ANTE legem viae - forma basename praebita
             * quoque nomina excludenda fert */
            (vacuum)tabula_dispersa_inserere(latina_tituli,
                _chorda_hospitis(vista.titulus), NIHIL);
        }
        {
            /* lex viae: basename praebita (sine '/') omissa - ordo
             * canonicus ex ambulatione directa capitis venit */
            b32 habet_separatorem = FALSUM;
            insignatus integer j;

            per (j = ZEPHYRUM; j < via->mensura; j++)
            {
                si (via->datum[j] == '/')
                {
                    habet_separatorem = VERUM;
                    frange;
                }
            }
            si (!habet_separatorem)
            {
                perge;
            }
        }
        receptor(datum, vista.titulus, "sedes", "macro", via,
            vista.linea, I, ZEPHYRUM);
    }

    /* II. usus expansionum: lexema non-FONS = expansum; radix =
     * sedes invocationis in fonte. Dedup (nomen|via|linea) -
     * radices argumentorum transmissorum in eadem linea
     * collabuntur (approximatio v0, vide phase-log). */
    si (parsura->lexemata == NIHIL)
    {
        redde;
    }
    {
        insignatus integer n = silva_xar_numerus(parsura->lexemata);
        SilvaToken* radix_prior = NIHIL;

        per (k = ZEPHYRUM; k < n; k++)
        {
            SilvaToken** cella = (SilvaToken**)silva_xar_obtinere(
                parsura->lexemata, k);
            SilvaToken* tok;
            SilvaToken* radix;
            constans SilvaChorda* titulus;
            constans SilvaChorda* via;
            insignatus integer linea;
            insignatus integer columna;

            si (cella == NIHIL || *cella == NIHIL)
            {
                perge;
            }
            tok = *cella;
            si (tok->origo.genus == SILVA_ORIGO_FONS)
            {
                perge;
            }
            radix = silva_token_radix(tok);
            si (radix == NIHIL || radix == radix_prior)
            {
                perge;   /* eadem invocatio - lexemata contigua */
            }
            radix_prior = radix;
            titulus = nexus_ordines_titulus_macronis(tok);
            si (titulus == NIHIL)
            {
                perge;
            }
            si (tabula_dispersa_continet(latina_tituli,
                    _chorda_hospitis(titulus)))
            {
                perge;
            }
            si (!_positionem_capere(parsura, tok, &via, &linea,
                    &columna))
            {
                perge;
            }
            {
                character clavis_litterae[CCLVI * II];
                chorda clavis;
                int scripti;

                si (titulus->mensura + via->mensura + XXXII
                    > (insignatus integer)magnitudo(
                          clavis_litterae))
                {
                    perge;
                }
                scripti = sprintf(clavis_litterae, "%.*s|%.*s|%u",
                    (int)titulus->mensura,
                    (constans character*)titulus->datum,
                    (int)via->mensura,
                    (constans character*)via->datum,
                    linea);
                si (scripti <= ZEPHYRUM)
                {
                    perge;
                }
                clavis.mensura = (i32)scripti;
                clavis.datum = (i8*)clavis_litterae;
                si (tabula_dispersa_continet(usus_visi, clavis))
                {
                    perge;
                }
                (vacuum)tabula_dispersa_inserere(usus_visi,
                    chorda_transcribere(clavis, effimera), NIHIL);
            }
            receptor(datum, titulus, "usus", "macro", via, linea,
                columna, ZEPHYRUM);
        }
    }
}

/* ==================================================
 * Lexemata retenta (stadium quartum, 01KYX2DSKK sequela):
 * identificatores in CORPORIBUS macronum et in laminis ramorum
 * OMISSORUM - sedes nomine-dominii quas nullum instrumentum
 * videbat ("quae macra symbolum X referunt" nunc respondibile;
 * renominare machinationem probavit). Lexemata vera, non textus -
 * chordae et commenta non fallunt.
 * ================================================== */

interior vacuum
_lexema_retentum_fundere (constans SilvaParsura* parsura,
    SilvaToken* tok, constans character* genus,
    NexusOrdinesReceptor receptor, vacuum* datum)
{
    constans SilvaChorda* via;
    insignatus integer linea;
    insignatus integer columna;

    si (tok == NIHIL || tok->genus != SILVA_LEX_IDENTIFICATOR
        || tok->byte_offset < ZEPHYRUM)
    {
        redde;
    }
    si (!_positionem_capere(parsura, tok, &via, &linea, &columna))
    {
        redde;
    }
    {
        SilvaChorda titulus;

        titulus.datum = tok->valor.datum;
        titulus.mensura = tok->valor.mensura;
        receptor(datum, &titulus, "usus", genus, via, linea,
            columna, ZEPHYRUM);
    }
}

interior vacuum
_retenta_fundere (constans SilvaParsura* parsura,
    Piscina* effimera, NexusOrdinesReceptor receptor,
    vacuum* datum)
{
    TabulaDispersa* sumpta;
    insignatus integer n;
    insignatus integer k;
    insignatus integer j;

    si (parsura->expansio == NIHIL)
    {
        redde;
    }
    n = silva_macros_numerus(parsura->expansio);
    per (k = ZEPHYRUM; k < n; k++)
    {
        insignatus integer m = silva_macro_corpus_numerus(
            parsura->expansio, k);

        per (j = ZEPHYRUM; j < m; j++)
        {
            _lexema_retentum_fundere(parsura,
                silva_macro_corpus_lexema(parsura->expansio, k,
                    j), "corpus-macronis", receptor, datum);
        }
    }
    /* UMBRAE INCLUSIONIS REPETITAE: inclusio secunda capitis
     * custodita (#ifndef X_H iam definito) corpus TOTUM ut
     * laminam omissam fert - non codex dormiens sed idem codex
     * IAM parsatus. Ramus omissus cuius (fons|initium) rami
     * SUMPTI congruit = umbra, supprimitur; rami platformarum
     * (#ifdef __linux__...) initium proprium habent et manent. */
    sumpta = tabula_dispersa_creare_chorda(effimera, CCLVI);
    n = silva_rami_numerus(parsura->expansio);
    per (k = ZEPHYRUM; k < n; k++)
    {
        SilvaRamusVista vista;

        si (silva_ramus_vista(parsura->expansio, k, &vista)
            && vista.est_sumptum && vista.corpus_initium >= ZEPHYRUM
            && sumpta != NIHIL)
        {
            character clavis_l[LXIV];
            int scripti = sprintf(clavis_l, "%ld|%ld",
                (long)vista.fons_index,
                (long)vista.corpus_initium);

            si (scripti > ZEPHYRUM)
            {
                chorda clavis;

                clavis.mensura = (i32)scripti;
                clavis.datum = (i8*)clavis_l;
                (vacuum)tabula_dispersa_inserere(sumpta,
                    chorda_transcribere(clavis, effimera), NIHIL);
            }
        }
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        SilvaRamusVista vista;
        insignatus integer m;

        si (!silva_ramus_vista(parsura->expansio, k, &vista))
        {
            perge;
        }
        si (!vista.est_sumptum && vista.corpus_initium >= ZEPHYRUM
            && sumpta != NIHIL)
        {
            character clavis_l[LXIV];
            int scripti = sprintf(clavis_l, "%ld|%ld",
                (long)vista.fons_index,
                (long)vista.corpus_initium);

            si (scripti > ZEPHYRUM)
            {
                chorda clavis;

                clavis.mensura = (i32)scripti;
                clavis.datum = (i8*)clavis_l;
                si (tabula_dispersa_continet(sumpta, clavis))
                {
                    perge;   /* umbra inclusionis repetitae */
                }
            }
        }
        m = silva_ramus_lexemata_numerus(parsura->expansio, k);
        per (j = ZEPHYRUM; j < m; j++)
        {
            _lexema_retentum_fundere(parsura,
                silva_ramus_lexema_crudum(parsura->expansio, k,
                    j), "ramus-omissus", receptor, datum);
        }
    }
}

vacuum
nexus_ordines_fundere (constans SilvaParsura* parsura,
    constans SilvaSemantica* sem, Piscina* effimera,
    NexusOrdinesReceptor receptor, vacuum* datum)
{
    si (parsura == NIHIL || sem == NIHIL || effimera == NIHIL
        || receptor == NIHIL || parsura->commissio == NIHIL)
    {
        redde;
    }
    _symbola_fundere(parsura, sem, receptor, datum);
    _usus_fundere(parsura, sem, effimera, receptor, datum);
    _macros_fundere(parsura, effimera, receptor, datum);
    _retenta_fundere(parsura, effimera, receptor, datum);
}
