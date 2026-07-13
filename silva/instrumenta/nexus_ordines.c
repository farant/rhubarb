/* nexus_ordines.c - ordines nexus ex analysi una (vide .h)
 *
 * Ex nexus_percursus.c levata (LEGATUS v0.1b); mores IDENTICI -
 * paritas octetim tsv = barra migrationis. AMBULATIO LATA servata
 * consulto (ordo emissionis = ordo tabulae). */

#include "latina.h"
#include "nexus_ordines.h"

#include "xar.h"

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
}
