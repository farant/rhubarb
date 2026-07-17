/* silva_c89_fluxus.c - aedificator graphi fluxus imperii (FLUXUS-0)
 * (specificatio = project-specs/silva-fluxus-0-spec.md, simulatio XI)
 *
 * Ambulatio una super corpus definitionis: bloci = cursus fluxus,
 * margines typati, saltus per tabulam titulorum PRAE-collectam
 * (transitus duo: tituli primum - salta antrorsum gratis, etiam
 * trans bracchia fraterna [arbor_syntaxis:4460]). Conditio constans
 * per aestimatorem vocatoris PLICATUR (dum(2-1) infinita est
 * clang-i; paritas codicis 63 id postulat - XI oraculum).
 *
 * Politica roburis: numquam cadere. ERROR/AMBIGUUS inresolutum =
 * folium opacum, fluxus transit (conservativum). frange/perge sine
 * contextu et salta ignotus = DATA in tabulatis fluxus (gradus 66/67
 * legunt); margo omittitur.
 *
 * Acervus contextus fractionum: xar sine remotione, ergo altitudo
 * VIVA in aedificatore geritur - impositio scribit in loco altitudinis
 * (aut appendit), depositio altitudinem minuit. Loci sub altitudine
 * soli leguntur.
 */

#include "silva_c89_fluxus.h"
#include "silva_tabulae_c89.h"

#include <string.h>

/* ==================================================
 * Status aedificatoris
 * ================================================== */

/* Contextus fractionis: quo frange/perge exeunt. Commutatio
 * persecutionem NIHIL fert (perge eam transit). */
nomen structura {
    FluxusBlocus* ruptura;
    FluxusBlocus* persecutio;
} FluxusContextusFractionis;

nomen structura {
    Piscina*          piscina;
    FluxusAuxilia     aux;        /* normalizata (campi NIHIL licent) */
    FluxusFunctionis* fluxus;
    FluxusBlocus*     currens;    /* NIHIL = fluxus mortuus (post saltum) */
    Xar*              fractiones; /* FluxusContextusFractionis (valore) */
    i32               altitudo;   /* numerus vivus acervi fractionum */
} FluxusAedificator;

interior vacuum _sententiam_ambulare (FluxusAedificator* aed,
    constans SilvaNodus* nodus);
interior vacuum _titulos_colligere (FluxusAedificator* aed,
    constans SilvaNodus* nodus);

/* ==================================================
 * Auxilia parva
 * ================================================== */

interior constans SilvaNodus*
_nodalis (SilvaValor v)
{
    si (v.genus != SILVA_VALOR_NODUS)
    {
        redde NIHIL;
    }
    redde v.datum.nodus;
}

interior constans SilvaNodus*
_canonicum_per (FluxusAedificator* aed, constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || aed->aux.canonicum == NIHIL)
    {
        redde nodus;
    }
    redde aed->aux.canonicum(aed->aux.contextus, nodus);
}

/* Aestimatio conditionis: VERUM si constans (valor in *valor). */
interior b32
_aestimare (FluxusAedificator* aed, SilvaValor conditio_v, s64* valor)
{
    constans SilvaNodus* n = _nodalis(conditio_v);

    si (n == NIHIL || aed->aux.aestimator == NIHIL)
    {
        redde FALSUM;
    }
    redde aed->aux.aestimator(aed->aux.contextus, n, valor);
}

/* ==================================================
 * Bloci et margines
 * ================================================== */

interior FluxusBlocus*
_blocus_novus (FluxusAedificator* aed)
{
    FluxusBlocus* b = (FluxusBlocus*)xar_addere(aed->fluxus->bloci);

    b->index = xar_numerus(aed->fluxus->bloci) - I;
    b->sententiae = xar_creare(aed->piscina,
        (i32)magnitudo(constans SilvaNodus*));
    b->margines = xar_creare(aed->piscina, (i32)magnitudo(FluxusMargo));
    b->attingibilis = FALSUM;
    b->titulus_dux = NIHIL;
    redde b;
}

interior vacuum
_margo_addere (FluxusBlocus* fons, s32 genus,
    FluxusBlocus* destinatio, constans SilvaNodus* origo)
{
    FluxusMargo* m;

    si (fons == NIHIL || destinatio == NIHIL)
    {
        redde;
    }
    m = (FluxusMargo*)xar_addere(fons->margines);
    m->genus = genus;
    m->destinatio = destinatio;
    m->origo = origo;
}

/* Blocus currens - si fluxus mortuus, blocum orbum aperit (sine
 * margine introitus = inattingibilis naturaliter; cibus codicis 65) */
interior FluxusBlocus*
_in_currentem (FluxusAedificator* aed)
{
    si (aed->currens == NIHIL)
    {
        aed->currens = _blocus_novus(aed);
    }
    redde aed->currens;
}

/* Transitio recta ad blocum datum (margo SEQUENS si fluxus vivus) */
interior vacuum
_transire_ad (FluxusAedificator* aed, FluxusBlocus* destinatio,
    constans SilvaNodus* origo)
{
    si (aed->currens != NIHIL)
    {
        _margo_addere(aed->currens, (s32)FLUXUS_MARGO_SEQUENS,
            destinatio, origo);
    }
    aed->currens = destinatio;
}

interior vacuum
_folium_addere (FluxusAedificator* aed, constans SilvaNodus* nodus)
{
    FluxusBlocus* b = _in_currentem(aed);
    constans SilvaNodus** locus =
        (constans SilvaNodus**)xar_addere(b->sententiae);

    *locus = nodus;
}

interior vacuum
_nodum_notare (Xar* tabulatum, constans SilvaNodus* nodus)
{
    constans SilvaNodus** locus =
        (constans SilvaNodus**)xar_addere(tabulatum);

    *locus = nodus;
}

/* ==================================================
 * Acervus contextus fractionum
 * ================================================== */

interior vacuum
_contextum_imponere (FluxusAedificator* aed, FluxusBlocus* ruptura,
    FluxusBlocus* persecutio)
{
    FluxusContextusFractionis* c;

    si (aed->altitudo < xar_numerus(aed->fractiones))
    {
        c = (FluxusContextusFractionis*)xar_obtinere(aed->fractiones,
            aed->altitudo);
    }
    alioquin
    {
        c = (FluxusContextusFractionis*)xar_addere(aed->fractiones);
    }
    c->ruptura = ruptura;
    c->persecutio = persecutio;
    aed->altitudo++;
}

interior vacuum
_contextum_deponere (FluxusAedificator* aed)
{
    si (aed->altitudo > ZEPHYRUM)
    {
        aed->altitudo--;
    }
}

/* Apex acervi (frange) - NIHIL si vacuus */
interior FluxusContextusFractionis*
_fractionis_apex (FluxusAedificator* aed)
{
    si (aed->altitudo == ZEPHYRUM)
    {
        redde NIHIL;
    }
    redde (FluxusContextusFractionis*)xar_obtinere(aed->fractiones,
        aed->altitudo - I);
}

/* Contextus persecutionis proximus (perge commutationem transit) -
 * numeratio deorsum signata (lectio vindex!) */
interior FluxusContextusFractionis*
_persecutionis_proxima (FluxusAedificator* aed)
{
    s32 k;

    per (k = (s32)aed->altitudo - I; k >= (s32)ZEPHYRUM; k--)
    {
        FluxusContextusFractionis* c = (FluxusContextusFractionis*)
            xar_obtinere(aed->fractiones, (i32)k);

        si (c->persecutio != NIHIL)
        {
            redde c;
        }
    }
    redde NIHIL;
}

/* ==================================================
 * Tabula titulorum (transitus primus)
 * ================================================== */

interior FluxusTitulus*
_titulum_invenire_per_nodum (FluxusAedificator* aed,
    constans SilvaNodus* nodus)
{
    i32 i;
    i32 m = xar_numerus(aed->fluxus->tituli);

    per (i = ZEPHYRUM; i < m; i++)
    {
        FluxusTitulus* t = (FluxusTitulus*)xar_obtinere(
            aed->fluxus->tituli, i);

        si (t->nodus == nodus)
        {
            redde t;
        }
    }
    redde NIHIL;
}

interior FluxusTitulus*
_titulum_invenire_per_chordam (FluxusAedificator* aed, chorda titulus)
{
    i32 i;
    i32 m = xar_numerus(aed->fluxus->tituli);

    per (i = ZEPHYRUM; i < m; i++)
    {
        FluxusTitulus* t = (FluxusTitulus*)xar_obtinere(
            aed->fluxus->tituli, i);

        si (chorda_aequalis(t->titulus, titulus))
        {
            redde t;
        }
    }
    redde NIHIL;
}

interior vacuum
_listam_colligere (FluxusAedificator* aed, SilvaValor lista)
{
    i32 i;
    i32 m = silva_valor_lista_numerus(lista);

    per (i = ZEPHYRUM; i < m; i++)
    {
        SilvaValor* v = silva_valor_lista_obtinere(lista, i);

        si (v != NIHIL && v->genus == SILVA_VALOR_NODUS)
        {
            _titulos_colligere(aed, v->datum.nodus);
        }
    }
}

interior vacuum
_titulos_colligere (FluxusAedificator* aed, constans SilvaNodus* nodus)
{
    si (nodus == NIHIL)
    {
        redde;
    }
    nodus = _canonicum_per(aed, nodus);
    commutatio (nodus->genus)
    {
        casus (s32)SILVA_C89_GENUS_TITULATUM:
        {
            SilvaValor tok_v = silva_c89_titulatum_tok_titulus(nodus);
            FluxusTitulus* t = (FluxusTitulus*)xar_addere(
                aed->fluxus->tituli);

            t->blocus = _blocus_novus(aed);
            t->blocus->titulus_dux = nodus;
            t->nodus = nodus;
            si (tok_v.genus == SILVA_VALOR_TOKEN)
            {
                t->titulus = tok_v.datum.token->valor;
            }
            alioquin
            {
                t->titulus.datum = NIHIL;
                t->titulus.mensura = ZEPHYRUM;
            }
            _titulos_colligere(aed,
                _nodalis(silva_c89_titulatum_sententia(nodus)));
            frange;
        }
        casus (s32)SILVA_C89_GENUS_CORPUS:
            _listam_colligere(aed, silva_c89_corpus_elementa(nodus));
            frange;
        casus (s32)SILVA_C89_GENUS_SI:
            _titulos_colligere(aed,
                _nodalis(silva_c89_si_consequens(nodus)));
            _titulos_colligere(aed,
                _nodalis(silva_c89_si_alioquin(nodus)));
            frange;
        casus (s32)SILVA_C89_GENUS_DUM:
            _titulos_colligere(aed,
                _nodalis(silva_c89_dum_corpus(nodus)));
            frange;
        casus (s32)SILVA_C89_GENUS_FAC_DUM:
            _titulos_colligere(aed,
                _nodalis(silva_c89_fac_dum_corpus(nodus)));
            frange;
        casus (s32)SILVA_C89_GENUS_PER:
            _titulos_colligere(aed,
                _nodalis(silva_c89_per_corpus(nodus)));
            frange;
        casus (s32)SILVA_C89_GENUS_COMMUTATIO:
            _titulos_colligere(aed,
                _nodalis(silva_c89_commutatio_corpus(nodus)));
            frange;
        casus (s32)SILVA_C89_GENUS_CASUS:
            _listam_colligere(aed, silva_c89_casus_sententiae(nodus));
            frange;
        casus (s32)SILVA_C89_GENUS_ORDINARIUS:
            _listam_colligere(aed,
                silva_c89_ordinarius_sententiae(nodus));
            frange;
        casus (s32)SILVA_C89_GENUS_CONDITIONALIS:
            _listam_colligere(aed,
                silva_c89_conditionalis_rami(nodus));
            frange;
        casus (s32)SILVA_C89_GENUS_RAMUS_SUMPTUS:
            _listam_colligere(aed,
                silva_c89_ramus_sumptus_contentum(nodus));
            frange;
        ordinarius:
            frange;
    }
}

/* ==================================================
 * Ambulatio (transitus alter) - sententiae compositae
 * ================================================== */

interior vacuum
_listam_fluere (FluxusAedificator* aed, SilvaValor lista)
{
    i32 i;
    i32 m = silva_valor_lista_numerus(lista);

    per (i = ZEPHYRUM; i < m; i++)
    {
        SilvaValor* v = silva_valor_lista_obtinere(lista, i);

        si (v != NIHIL && v->genus == SILVA_VALOR_NODUS)
        {
            _sententiam_ambulare(aed, v->datum.nodus);
        }
    }
}

interior vacuum
_si_fluere (FluxusAedificator* aed, constans SilvaNodus* nodus)
{
    SilvaValor conditio_v = silva_c89_si_conditio(nodus);
    constans SilvaNodus* alterum_n =
        _nodalis(silva_c89_si_alioquin(nodus));
    FluxusBlocus* apertura = _in_currentem(aed);
    FluxusBlocus* dein;
    FluxusBlocus* dein_finis;
    FluxusBlocus* alterum_finis = NIHIL;
    FluxusBlocus* iunctio;
    s64 valor = ZEPHYRUM;
    b32 constansne = _aestimare(aed, conditio_v, &valor);

    dein = _blocus_novus(aed);
    si (!constansne || valor != ZEPHYRUM)
    {
        _margo_addere(apertura, (s32)FLUXUS_MARGO_VERUS, dein, nodus);
    }
    aed->currens = dein;
    _sententiam_ambulare(aed, _nodalis(silva_c89_si_consequens(nodus)));
    dein_finis = aed->currens;

    si (alterum_n != NIHIL)
    {
        FluxusBlocus* alterum = _blocus_novus(aed);

        si (!constansne || valor == ZEPHYRUM)
        {
            _margo_addere(apertura, (s32)FLUXUS_MARGO_FALSUS,
                alterum, nodus);
        }
        aed->currens = alterum;
        _sententiam_ambulare(aed, alterum_n);
        alterum_finis = aed->currens;
    }

    iunctio = _blocus_novus(aed);
    si (alterum_n == NIHIL && (!constansne || valor == ZEPHYRUM))
    {
        _margo_addere(apertura, (s32)FLUXUS_MARGO_FALSUS, iunctio,
            nodus);
    }
    si (dein_finis != NIHIL)
    {
        _margo_addere(dein_finis, (s32)FLUXUS_MARGO_SEQUENS, iunctio,
            nodus);
    }
    si (alterum_finis != NIHIL)
    {
        _margo_addere(alterum_finis, (s32)FLUXUS_MARGO_SEQUENS,
            iunctio, nodus);
    }
    aed->currens = iunctio;
}

interior vacuum
_dum_fluere (FluxusAedificator* aed, constans SilvaNodus* nodus)
{
    SilvaValor conditio_v = silva_c89_dum_conditio(nodus);
    FluxusBlocus* caput = _blocus_novus(aed);
    FluxusBlocus* corpus_b;
    FluxusBlocus* postis;
    s64 valor = ZEPHYRUM;
    b32 constansne = _aestimare(aed, conditio_v, &valor);

    _transire_ad(aed, caput, nodus);
    corpus_b = _blocus_novus(aed);
    postis = _blocus_novus(aed);
    si (!constansne || valor != ZEPHYRUM)
    {
        _margo_addere(caput, (s32)FLUXUS_MARGO_VERUS, corpus_b, nodus);
    }
    si (!constansne || valor == ZEPHYRUM)
    {
        _margo_addere(caput, (s32)FLUXUS_MARGO_FALSUS, postis, nodus);
    }
    _contextum_imponere(aed, postis, caput);
    aed->currens = corpus_b;
    _sententiam_ambulare(aed, _nodalis(silva_c89_dum_corpus(nodus)));
    si (aed->currens != NIHIL)
    {
        _margo_addere(aed->currens, (s32)FLUXUS_MARGO_SEQUENS, caput,
            nodus);
    }
    _contextum_deponere(aed);
    aed->currens = postis;
}

interior vacuum
_fac_dum_fluere (FluxusAedificator* aed, constans SilvaNodus* nodus)
{
    SilvaValor conditio_v = silva_c89_fac_dum_conditio(nodus);
    FluxusBlocus* corpus_b = _blocus_novus(aed);
    FluxusBlocus* conditio_b;
    FluxusBlocus* postis;
    s64 valor = ZEPHYRUM;
    b32 constansne = _aestimare(aed, conditio_v, &valor);

    _transire_ad(aed, corpus_b, nodus);
    conditio_b = _blocus_novus(aed);
    postis = _blocus_novus(aed);
    _contextum_imponere(aed, postis, conditio_b);
    aed->currens = corpus_b;
    _sententiam_ambulare(aed,
        _nodalis(silva_c89_fac_dum_corpus(nodus)));
    si (aed->currens != NIHIL)
    {
        _margo_addere(aed->currens, (s32)FLUXUS_MARGO_SEQUENS,
            conditio_b, nodus);
    }
    _contextum_deponere(aed);
    si (!constansne || valor != ZEPHYRUM)
    {
        _margo_addere(conditio_b, (s32)FLUXUS_MARGO_VERUS, corpus_b,
            nodus);
    }
    si (!constansne || valor == ZEPHYRUM)
    {
        _margo_addere(conditio_b, (s32)FLUXUS_MARGO_FALSUS, postis,
            nodus);
    }
    aed->currens = postis;
}

interior vacuum
_per_fluere (FluxusAedificator* aed, constans SilvaNodus* nodus)
{
    constans SilvaNodus* clausula =
        _canonicum_per(aed, _nodalis(silva_c89_per_clausula(nodus)));
    constans SilvaNodus* initium_n = NIHIL;
    constans SilvaNodus* conditio_n = NIHIL;
    constans SilvaNodus* passus_n = NIHIL;
    FluxusBlocus* caput;
    FluxusBlocus* corpus_b;
    FluxusBlocus* passus_b;
    FluxusBlocus* postis;
    s64 valor = ZEPHYRUM;
    b32 constansne = FALSUM;

    si (clausula != NIHIL
        && clausula->genus == (s32)SILVA_C89_GENUS_PER_CLAUSULA)
    {
        initium_n = _nodalis(silva_c89_per_clausula_initium(clausula));
        conditio_n = _nodalis(silva_c89_per_clausula_conditio(
            clausula));
        passus_n = _nodalis(silva_c89_per_clausula_passus(clausula));
    }
    si (initium_n != NIHIL)
    {
        _folium_addere(aed, initium_n);
    }
    si (conditio_n != NIHIL)
    {
        constansne = _aestimare(aed,
            silva_c89_per_clausula_conditio(clausula), &valor);
    }
    caput = _blocus_novus(aed);
    _transire_ad(aed, caput, nodus);
    corpus_b = _blocus_novus(aed);
    passus_b = _blocus_novus(aed);
    postis = _blocus_novus(aed);
    /* conditio absens = semper vera (per (;;)) */
    si (conditio_n == NIHIL || !constansne || valor != ZEPHYRUM)
    {
        _margo_addere(caput, (s32)FLUXUS_MARGO_VERUS, corpus_b, nodus);
    }
    si (conditio_n != NIHIL && (!constansne || valor == ZEPHYRUM))
    {
        _margo_addere(caput, (s32)FLUXUS_MARGO_FALSUS, postis, nodus);
    }
    _contextum_imponere(aed, postis, passus_b);
    aed->currens = corpus_b;
    _sententiam_ambulare(aed, _nodalis(silva_c89_per_corpus(nodus)));
    si (aed->currens != NIHIL)
    {
        _margo_addere(aed->currens, (s32)FLUXUS_MARGO_SEQUENS,
            passus_b, nodus);
    }
    _contextum_deponere(aed);
    si (passus_n != NIHIL)
    {
        constans SilvaNodus** locus = (constans SilvaNodus**)
            xar_addere(passus_b->sententiae);

        *locus = passus_n;
    }
    _margo_addere(passus_b, (s32)FLUXUS_MARGO_SEQUENS, caput, nodus);
    aed->currens = postis;
}

interior vacuum
_commutationem_fluere (FluxusAedificator* aed,
    constans SilvaNodus* nodus)
{
    constans SilvaNodus* corpus_n =
        _canonicum_per(aed, _nodalis(silva_c89_commutatio_corpus(
            nodus)));
    FluxusBlocus* apertura = _in_currentem(aed);
    FluxusBlocus* postis = _blocus_novus(aed);
    b32 ordinarius_visus = FALSUM;

    _contextum_imponere(aed, postis, NIHIL);
    aed->currens = NIHIL;   /* ante titulum primum: fluxus mortuus */

    si (corpus_n != NIHIL
        && corpus_n->genus == (s32)SILVA_C89_GENUS_CORPUS)
    {
        SilvaValor lista = silva_c89_corpus_elementa(corpus_n);
        i32 i;
        i32 m = silva_valor_lista_numerus(lista);

        per (i = ZEPHYRUM; i < m; i++)
        {
            SilvaValor* v = silva_valor_lista_obtinere(lista, i);
            constans SilvaNodus* e;

            si (v == NIHIL || v->genus != SILVA_VALOR_NODUS)
            {
                perge;
            }
            e = _canonicum_per(aed, v->datum.nodus);
            si (e->genus == (s32)SILVA_C89_GENUS_CASUS
                || e->genus == (s32)SILVA_C89_GENUS_ORDINARIUS)
            {
                FluxusBlocus* grex = _blocus_novus(aed);
                s32 genus_marginis;

                grex->titulus_dux = e;
                si (e->genus == (s32)SILVA_C89_GENUS_ORDINARIUS)
                {
                    genus_marginis = (s32)FLUXUS_MARGO_ORDINARIUS;
                    ordinarius_visus = VERUM;
                }
                alioquin
                {
                    genus_marginis = (s32)FLUXUS_MARGO_CASUS;
                }
                _margo_addere(apertura, genus_marginis, grex, e);
                si (aed->currens != NIHIL)
                {
                    /* finis gregis prioris apertus - lapsus */
                    _margo_addere(aed->currens,
                        (s32)FLUXUS_MARGO_LAPSUS, grex, e);
                }
                aed->currens = grex;
                si (e->genus == (s32)SILVA_C89_GENUS_ORDINARIUS)
                {
                    _listam_fluere(aed,
                        silva_c89_ordinarius_sententiae(e));
                }
                alioquin
                {
                    _listam_fluere(aed, silva_c89_casus_sententiae(e));
                }
            }
            alioquin
            {
                /* elementum ante titulum primum (aut inter greges
                 * numquam in corpore reali) - blocus orbus */
                _sententiam_ambulare(aed, e);
            }
        }
    }
    alioquin si (corpus_n != NIHIL)
    {
        /* corpus non-corpus (sententia una sine uncis) - robustum:
         * sine dispersione, regio orba */
        _sententiam_ambulare(aed, corpus_n);
    }

    si (aed->currens != NIHIL)
    {
        /* grex ultimus apertus cadit e commutatione */
        _margo_addere(aed->currens, (s32)FLUXUS_MARGO_SEQUENS, postis,
            nodus);
    }
    si (!ordinarius_visus)
    {
        /* semita non-congruentiae implicita */
        _margo_addere(apertura, (s32)FLUXUS_MARGO_ORDINARIUS, postis,
            nodus);
    }
    _contextum_deponere(aed);
    aed->currens = postis;
}

/* ==================================================
 * Ambulatio - dispersio
 * ================================================== */

interior vacuum
_sententiam_ambulare (FluxusAedificator* aed, constans SilvaNodus* nodus)
{
    si (nodus == NIHIL)
    {
        redde;
    }
    nodus = _canonicum_per(aed, nodus);
    commutatio (nodus->genus)
    {
        casus (s32)SILVA_C89_GENUS_CORPUS:
            _listam_fluere(aed, silva_c89_corpus_elementa(nodus));
            frange;
        casus (s32)SILVA_C89_GENUS_SI:
            _si_fluere(aed, nodus);
            frange;
        casus (s32)SILVA_C89_GENUS_DUM:
            _dum_fluere(aed, nodus);
            frange;
        casus (s32)SILVA_C89_GENUS_FAC_DUM:
            _fac_dum_fluere(aed, nodus);
            frange;
        casus (s32)SILVA_C89_GENUS_PER:
            _per_fluere(aed, nodus);
            frange;
        casus (s32)SILVA_C89_GENUS_COMMUTATIO:
            _commutationem_fluere(aed, nodus);
            frange;
        casus (s32)SILVA_C89_GENUS_TITULATUM:
        {
            FluxusTitulus* t = _titulum_invenire_per_nodum(aed, nodus);

            si (t != NIHIL)
            {
                _transire_ad(aed, t->blocus, nodus);
            }
            _sententiam_ambulare(aed,
                _nodalis(silva_c89_titulatum_sententia(nodus)));
            frange;
        }
        casus (s32)SILVA_C89_GENUS_SALTA:
        {
            SilvaValor tok_v = silva_c89_salta_tok_destinatio(nodus);
            FluxusTitulus* t = NIHIL;
            FluxusBlocus* b = _in_currentem(aed);

            _folium_addere(aed, nodus);
            si (tok_v.genus == SILVA_VALOR_TOKEN)
            {
                t = _titulum_invenire_per_chordam(aed,
                    tok_v.datum.token->valor);
            }
            si (t != NIHIL)
            {
                _margo_addere(b, (s32)FLUXUS_MARGO_SALTUS, t->blocus,
                    nodus);
            }
            alioquin
            {
                /* codex 67 hoc legit */
                _nodum_notare(aed->fluxus->saltus_ignoti, nodus);
            }
            aed->currens = NIHIL;
            frange;
        }
        casus (s32)SILVA_C89_GENUS_FRANGE:
        {
            FluxusContextusFractionis* c = _fractionis_apex(aed);
            FluxusBlocus* b = _in_currentem(aed);

            _folium_addere(aed, nodus);
            si (c != NIHIL)
            {
                _margo_addere(b, (s32)FLUXUS_MARGO_RUPTURA, c->ruptura,
                    nodus);
            }
            alioquin
            {
                /* codex 66 hoc legit */
                _nodum_notare(aed->fluxus->fractiones_extra, nodus);
            }
            aed->currens = NIHIL;
            frange;
        }
        casus (s32)SILVA_C89_GENUS_PERGE:
        {
            FluxusContextusFractionis* c = _persecutionis_proxima(aed);
            FluxusBlocus* b = _in_currentem(aed);

            _folium_addere(aed, nodus);
            si (c != NIHIL)
            {
                _margo_addere(b, (s32)FLUXUS_MARGO_PERSECUTIO,
                    c->persecutio, nodus);
            }
            alioquin
            {
                _nodum_notare(aed->fluxus->fractiones_extra, nodus);
            }
            aed->currens = NIHIL;
            frange;
        }
        casus (s32)SILVA_C89_GENUS_REDDE:
        {
            FluxusBlocus* b = _in_currentem(aed);

            _folium_addere(aed, nodus);
            _margo_addere(b, (s32)FLUXUS_MARGO_REDDITUS,
                aed->fluxus->exitus, nodus);
            aed->currens = NIHIL;
            frange;
        }
        casus (s32)SILVA_C89_GENUS_CASUS:
            /* extra explorationem commutationis (corpus sine uncis
             * aut forma insolita) - sententiae influunt, titulus
             * amittitur (conservativum, numquam cadere) */
            _listam_fluere(aed, silva_c89_casus_sententiae(nodus));
            frange;
        casus (s32)SILVA_C89_GENUS_ORDINARIUS:
            _listam_fluere(aed, silva_c89_ordinarius_sententiae(nodus));
            frange;
        casus (s32)SILVA_C89_GENUS_CONDITIONALIS:
            /* semita sumpta tantum (pin INTENTIO: CFG = visus) */
            _listam_fluere(aed, silva_c89_conditionalis_rami(nodus));
            frange;
        casus (s32)SILVA_C89_GENUS_RAMUS_SUMPTUS:
            _listam_fluere(aed, silva_c89_ramus_sumptus_contentum(
                nodus));
            frange;
        casus (s32)SILVA_C89_GENUS_RAMUS_OMISSUS:
            /* bracchium non sumptum: crudum, sine fluxu (semita
             * sumpta sola - pin INTENTIO) */
            frange;
        ordinarius:
            /* folium: declaratio, sententia-expressionis,
             * sententia-vacua, ERROR, AMBIGUUS inresolutum */
            _folium_addere(aed, nodus);
            frange;
    }
}

/* ==================================================
 * Attingibilitas
 * ================================================== */

interior vacuum
_attingibilitatem_signare (FluxusFunctionis* fluxus, Piscina* piscina)
{
    Xar* index_operis = xar_creare(piscina,
        (i32)magnitudo(FluxusBlocus*));
    i32 lector = ZEPHYRUM;

    fluxus->introitus->attingibilis = VERUM;
    {
        FluxusBlocus** locus = (FluxusBlocus**)xar_addere(index_operis);

        *locus = fluxus->introitus;
    }
    dum (lector < xar_numerus(index_operis))
    {
        FluxusBlocus* b = *(FluxusBlocus**)xar_obtinere(index_operis,
            lector);
        i32 i;
        i32 m = xar_numerus(b->margines);

        lector++;
        per (i = ZEPHYRUM; i < m; i++)
        {
            FluxusMargo* margo = (FluxusMargo*)xar_obtinere(
                b->margines, i);

            si (margo->destinatio != NIHIL
                && !margo->destinatio->attingibilis)
            {
                FluxusBlocus** locus;

                margo->destinatio->attingibilis = VERUM;
                locus = (FluxusBlocus**)xar_addere(index_operis);
                *locus = margo->destinatio;
            }
        }
    }
}

/* ==================================================
 * API
 * ================================================== */

FluxusFunctionis*
silva_c89_fluxus_aedificare (Piscina* piscina,
    constans SilvaNodus* definitio, constans FluxusAuxilia* auxilia)
{
    FluxusAedificator aed;
    FluxusFunctionis* fluxus;
    constans SilvaNodus* corpus_n;
    FluxusBlocus* cadens;

    si (definitio == NIHIL)
    {
        redde NIHIL;
    }
    corpus_n = _nodalis(silva_c89_definitio_functionis_corpus(
        definitio));
    si (corpus_n == NIHIL)
    {
        redde NIHIL;
    }

    fluxus = (FluxusFunctionis*)piscina_allocare(piscina,
        (i32)magnitudo(FluxusFunctionis));
    fluxus->definitio = definitio;
    fluxus->bloci = xar_creare(piscina, (i32)magnitudo(FluxusBlocus));
    fluxus->tituli = xar_creare(piscina, (i32)magnitudo(FluxusTitulus));
    fluxus->saltus_ignoti = xar_creare(piscina,
        (i32)magnitudo(constans SilvaNodus*));
    fluxus->fractiones_extra = xar_creare(piscina,
        (i32)magnitudo(constans SilvaNodus*));
    fluxus->cadit_attingibilis = FALSUM;

    aed.piscina = piscina;
    si (auxilia != NIHIL)
    {
        aed.aux = *auxilia;
    }
    alioquin
    {
        aed.aux.canonicum = NIHIL;
        aed.aux.aestimator = NIHIL;
        aed.aux.contextus = NIHIL;
    }
    aed.fluxus = fluxus;
    aed.currens = NIHIL;
    aed.fractiones = xar_creare(piscina,
        (i32)magnitudo(FluxusContextusFractionis));
    aed.altitudo = ZEPHYRUM;

    fluxus->introitus = _blocus_novus(&aed);
    fluxus->exitus = _blocus_novus(&aed);

    _titulos_colligere(&aed, corpus_n);

    aed.currens = fluxus->introitus;
    _sententiam_ambulare(&aed, corpus_n);
    cadens = aed.currens;
    si (cadens != NIHIL)
    {
        _margo_addere(cadens, (s32)FLUXUS_MARGO_CADIT, fluxus->exitus,
            definitio);
    }

    _attingibilitatem_signare(fluxus, piscina);
    si (cadens != NIHIL && cadens->attingibilis)
    {
        fluxus->cadit_attingibilis = VERUM;
    }
    redde fluxus;
}
