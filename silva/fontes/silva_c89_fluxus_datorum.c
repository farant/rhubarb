/* silva_c89_fluxus_datorum.c - extractio eventorum def/usus
 * (FLUXUS-1 chunk A; specificatio silva-fluxus-1-spec-v2.md §4)
 *
 * Ambulatio granulorum ordine blocorum, intra granulum ordine
 * aestimationis (approximatio: ordo locorum sinistrorsum-dextrorsum;
 * C ordinem operandorum fraternorum non definit - discrimen solum in
 * pathologiis eiusdem variabilis eiusdem sententiae).
 *
 * REGULAE (pinnae calibrationis ligantes - vide .h):
 * - assignatio: dexter ANTE definitionem sinistri; composita (+=)
 *   usum sinistri primum emittit
 * - incrementa (++x, x++, --): usus deinde definitio
 * - &x = DEFINITIO_LOCI nisi argumentum vocationis cum parametro
 *   T* constanti (tunc NULLUM eventum - s04f)
 * - scriptio membri/elementi (s.a=, a[i]=) = DEFINITIO totius (s19);
 *   lectio membri/elementi aggregati invisibilis (s05); basis
 *   monstratoris scalaris (p[i], p->a, *p) = USUS monstratoris
 * - magnitudo praetermissa (s18); (vacuum)x identificatoris nudi
 *   praetermissum (s01c)
 * - BINARIUM(&&,||)/TERNARIUS praetermissi INTEGRI - operanda in
 *   blocis praecedentibus granulata (invariantum fissionis chunk 0)
 * - folium ERROR/AMBIGUUS opacum = DEFINITIO variabilis -1 (OMNES;
 *   abstentio conservativa)
 */

#include "silva_c89_fluxus_datorum.h"
#include "silva_tabulae_c89.h"

#include <string.h>

/* ==================================================
 * Status extractoris
 * ================================================== */

nomen structura {
    Piscina*              piscina;
    FluxusDatorumAuxilia  aux;
    FluxusDatorum*        datorum;
    FluxusDatorumBlocus*  blocus_currens;
    constans vacuum*      declarator_identitas;  /* int x = x (s02) */
} FluxusExtractor;

interior vacuum _expressionem_ambulare (FluxusExtractor* ex,
    constans SilvaNodus* n);
interior vacuum _locum_resolvere (FluxusExtractor* ex,
    constans SilvaNodus* n, b32 per_elementum, s32 genus_emittendi);

/* ==================================================
 * Auxilia parva
 * ================================================== */

interior constans SilvaNodus*
_ut_nodus (SilvaValor v)
{
    si (v.genus != SILVA_VALOR_NODUS)
    {
        redde NIHIL;
    }
    redde v.datum.nodus;
}

interior constans SilvaNodus*
_lectio_canonica (FluxusExtractor* ex, constans SilvaNodus* n)
{
    si (n == NIHIL || ex->aux.canonicum == NIHIL)
    {
        redde n;
    }
    redde ex->aux.canonicum(ex->aux.contextus, n);
}

interior constans SilvaNodus*
_sine_parenthesibus (FluxusExtractor* ex, constans SilvaNodus* n)
{
    n = _lectio_canonica(ex, n);
    dum (n != NIHIL && n->genus == (s32)SILVA_C89_GENUS_PARENTHESIS)
    {
        n = _lectio_canonica(ex,
            _ut_nodus(silva_c89_parenthesis_internum(n)));
    }
    redde n;
}

interior SilvaLexemaGenus
_operator_lexis (SilvaValor tok_v)
{
    si (tok_v.genus != SILVA_VALOR_TOKEN)
    {
        redde SILVA_LEX_EOF;   /* sentinella: numquam operator */
    }
    redde tok_v.datum.token->genus;
}

/* Sectio brevis? (praetermissio - operanda iam granulata) */
interior b32
_est_sectio (constans SilvaNodus* n)
{
    si (n->genus == (s32)SILVA_C89_GENUS_TERNARIUS)
    {
        redde VERUM;
    }
    si (n->genus == (s32)SILVA_C89_GENUS_BINARIUM)
    {
        SilvaLexemaGenus lexis = _operator_lexis(
            silva_c89_binarium_tok_operator(n));

        redde (lexis == SILVA_LEX_ET_ET || lexis == SILVA_LEX_VEL_VEL)
            ? VERUM : FALSUM;
    }
    redde FALSUM;
}

/* ==================================================
 * Tabula variabilium + emissio
 * ================================================== */

/* Index densus: invenire aut addere per identitatem */
interior s32
_variabilis_index (FluxusExtractor* ex,
    constans FluxusSymbolumFacta* facta)
{
    i32 i;
    i32 m = xar_numerus(ex->datorum->variabiles);
    FluxusVariabilis* v;

    per (i = ZEPHYRUM; i < m; i++)
    {
        v = (FluxusVariabilis*)xar_obtinere(ex->datorum->variabiles,
            i);
        si (v->identitas == facta->identitas)
        {
            redde (s32)i;
        }
    }
    v = (FluxusVariabilis*)xar_addere(ex->datorum->variabiles);
    v->identitas = facta->identitas;
    v->titulus = facta->titulus;
    v->declarans = facta->declarans;
    v->parametrum = facta->parametrum;
    redde (s32)m;
}

interior vacuum
_eventum_emittere (FluxusExtractor* ex, s32 variabilis, s32 genus,
    constans SilvaNodus* nodus, b32 in_initiatore_proprio)
{
    FluxusEventum* e = (FluxusEventum*)xar_addere(
        ex->blocus_currens->eventa);

    e->variabilis = variabilis;
    e->genus = genus;
    e->nodus = nodus;
    e->in_initiatore_proprio = in_initiatore_proprio;
}

/* Facta pro folio/declaratore; FALSUM = non resolutum aut non
 * tractum (nullum eventum) */
interior b32
_facta_tracta (FluxusExtractor* ex, constans SilvaNodus* n,
    FluxusSymbolumFacta* facta)
{
    si (n == NIHIL || ex->aux.symbolum == NIHIL)
    {
        redde FALSUM;
    }
    si (!ex->aux.symbolum(ex->aux.contextus, n, facta))
    {
        redde FALSUM;
    }
    redde (facta->localis_automata || facta->parametrum)
        ? VERUM : FALSUM;
}

/* ==================================================
 * Conversio ad vacuum identificatoris nudi (s01c)
 * ================================================== */

interior b32
_est_conversio_discardans (FluxusExtractor* ex,
    constans SilvaNodus* conversio)
{
    constans SilvaNodus* typus =
        _lectio_canonica(ex, _ut_nodus(silva_c89_conversio_typus(conversio)));
    constans SilvaNodus* internum =
        _sine_parenthesibus(ex,
            _ut_nodus(silva_c89_conversio_internum(conversio)));
    b32 vacuum_visum = FALSUM;
    i32 i;

    si (internum == NIHIL
        || internum->genus != (s32)SILVA_C89_GENUS_FOLIUM_IDENTIFICATOR
        || typus == NIHIL)
    {
        redde FALSUM;
    }
    /* species: TYPUS_PRIMITIVUS cuius verba = "void" solum, sine
     * declaratore abstracto (ne (void*)x discardans habeatur).
     * verba = LISTA lexematum (exemplar _verba_numerare). */
    per (i = ZEPHYRUM; i < typus->numerus_locorum; i++)
    {
        constans SilvaValor* v = &typus->loci[i];
        constans SilvaNodus* filius;

        si (v->genus == SILVA_VALOR_LISTA)
        {
            i32 k;
            i32 m = silva_valor_lista_numerus(*v);

            per (k = ZEPHYRUM; k < m; k++)
            {
                SilvaValor* e = silva_valor_lista_obtinere(*v, k);

                si (e == NIHIL || e->genus != SILVA_VALOR_NODUS)
                {
                    perge;   /* lexemata (qualia) praetermissa */
                }
                filius = _lectio_canonica(ex, e->datum.nodus);
                si (filius == NIHIL)
                {
                    perge;
                }
                si (filius->genus
                        != (s32)SILVA_C89_GENUS_TYPUS_PRIMITIVUS)
                {
                    redde FALSUM;   /* declarator abstractus etc. */
                }
                {
                    SilvaValor verba =
                        silva_c89_typus_primitivus_tok_verba(filius);
                    i32 w;
                    i32 wm = silva_valor_lista_numerus(verba);

                    per (w = ZEPHYRUM; w < wm; w++)
                    {
                        SilvaValor* t = silva_valor_lista_obtinere(
                            verba, w);

                        si (t == NIHIL
                            || t->genus != SILVA_VALOR_TOKEN)
                        {
                            perge;
                        }
                        si (t->datum.token->genus == SILVA_LEX_VOID)
                        {
                            vacuum_visum = VERUM;
                        }
                        alioquin
                        {
                            redde FALSUM;
                        }
                    }
                }
            }
        }
        alioquin si (v->genus == SILVA_VALOR_NODUS)
        {
            filius = _ut_nodus(*v);
            si (filius != NIHIL)
            {
                redde FALSUM;       /* declarator abstractus */
            }
        }
    }
    redde vacuum_visum;
}

/* ==================================================
 * Resolutio loci (destinatum assignationis / operandum &)
 * ================================================== */

/* per_elementum: per subscriptio/membrum transivimus - basis
 * scalaris (monstrator) LEGITUR, aggregata definitionem totius
 * accipiunt. genus_emittendi: FLUXUS_EVENTUM_* aut -1 = nullum
 * eventum in basi (parametrum constans s04f). */
interior vacuum
_locum_resolvere (FluxusExtractor* ex, constans SilvaNodus* n,
    b32 per_elementum, s32 genus_emittendi)
{
    n = _sine_parenthesibus(ex, n);
    si (n == NIHIL)
    {
        redde;
    }
    commutatio (n->genus)
    {
        casus (s32)SILVA_C89_GENUS_FOLIUM_IDENTIFICATOR:
        {
            FluxusSymbolumFacta facta;

            si (!_facta_tracta(ex, n, &facta))
            {
                redde;
            }
            si (per_elementum && !facta.aggregatum)
            {
                /* p[i]= / &p[i]: monstrator scalaris legitur */
                _eventum_emittere(ex, _variabilis_index(ex, &facta),
                    (s32)FLUXUS_EVENTUM_USUS, n, FALSUM);
                redde;
            }
            si (genus_emittendi >= ZEPHYRUM)
            {
                _eventum_emittere(ex, _variabilis_index(ex, &facta),
                    genus_emittendi, n, FALSUM);
            }
            redde;
        }
        casus (s32)SILVA_C89_GENUS_SUBSCRIPTIO:
            _expressionem_ambulare(ex,
                _ut_nodus(silva_c89_subscriptio_index(n)));
            _locum_resolvere(ex,
                _ut_nodus(silva_c89_subscriptio_basis(n)),
                VERUM, genus_emittendi);
            redde;
        casus (s32)SILVA_C89_GENUS_ACCESSUS:
        {
            SilvaLexemaGenus lexis = _operator_lexis(
                silva_c89_accessus_tok_operator(n));

            si (lexis == SILVA_LEX_SAGITTA)
            {
                /* p->a: monstrator legitur, nulla definitio tracta */
                _expressionem_ambulare(ex,
                    _ut_nodus(silva_c89_accessus_basis(n)));
                redde;
            }
            /* punctum: scriptio membri = definitio totius (s19) */
            _locum_resolvere(ex,
                _ut_nodus(silva_c89_accessus_basis(n)),
                per_elementum, genus_emittendi);
            redde;
        }
        casus (s32)SILVA_C89_GENUS_UNARIUM:
        {
            SilvaLexemaGenus lexis = _operator_lexis(
                silva_c89_unarium_tok_operator(n));

            si (lexis == SILVA_LEX_STAR)
            {
                /* *p = : monstrator legitur (s01f) */
                _expressionem_ambulare(ex,
                    _ut_nodus(silva_c89_unarium_internum(n)));
                redde;
            }
            _expressionem_ambulare(ex, n);
            redde;
        }
        casus (s32)SILVA_C89_GENUS_ERROR:
        casus (s32)SILVA_C89_GENUS_AMBIGUUS:
            _eventum_emittere(ex, -I, (s32)FLUXUS_EVENTUM_DEFINITIO,
                n, FALSUM);
            redde;
        ordinarius:
            /* forma inexspectata: lectio conservativa */
            _expressionem_ambulare(ex, n);
            redde;
    }
}

/* ==================================================
 * Ambulatio expressionum (positio valoris)
 * ================================================== */

interior vacuum
_lista_ambulare (FluxusExtractor* ex, SilvaValor lista)
{
    i32 i;
    i32 m = silva_valor_lista_numerus(lista);

    per (i = ZEPHYRUM; i < m; i++)
    {
        SilvaValor* v = silva_valor_lista_obtinere(lista, i);

        si (v != NIHIL && v->genus == SILVA_VALOR_NODUS)
        {
            _expressionem_ambulare(ex, v->datum.nodus);
        }
    }
}

interior vacuum
_loci_ambulare (FluxusExtractor* ex, constans SilvaNodus* n)
{
    i32 i;

    per (i = ZEPHYRUM; i < n->numerus_locorum; i++)
    {
        constans SilvaValor* v = &n->loci[i];

        si (v->genus == SILVA_VALOR_NODUS)
        {
            _expressionem_ambulare(ex, v->datum.nodus);
        }
        alioquin si (v->genus == SILVA_VALOR_LISTA)
        {
            _lista_ambulare(ex, *v);
        }
    }
}

interior vacuum
_vocationem_ambulare (FluxusExtractor* ex, constans SilvaNodus* n)
{
    constans SilvaNodus* functio_folium = _sine_parenthesibus(ex,
        _ut_nodus(silva_c89_vocatio_functio(n)));
    SilvaValor argumenta = silva_c89_vocatio_argumenta(n);
    i32 i;
    i32 m = silva_valor_lista_numerus(argumenta);
    i32 index_argumenti = ZEPHYRUM;

    /* expressio functionis: identificator functionis non tractus
     * (nullum eventum), monstrator functionis variabilis = USUS */
    _expressionem_ambulare(ex,
        _ut_nodus(silva_c89_vocatio_functio(n)));

    per (i = ZEPHYRUM; i < m; i++)
    {
        SilvaValor* v = silva_valor_lista_obtinere(argumenta, i);
        constans SilvaNodus* arg;
        constans SilvaNodus* nucleus;

        si (v == NIHIL || v->genus != SILVA_VALOR_NODUS)
        {
            perge;   /* lexemata virgularum in lista */
        }
        arg = v->datum.nodus;
        nucleus = _sine_parenthesibus(ex, arg);
        si (nucleus != NIHIL
            && nucleus->genus == (s32)SILVA_C89_GENUS_UNARIUM
            && _operator_lexis(silva_c89_unarium_tok_operator(nucleus))
                == SILVA_LEX_AMPERSAND)
        {
            b32 constansne = FALSUM;

            si (ex->aux.parametrum_constans != NIHIL)
            {
                constansne = ex->aux.parametrum_constans(
                    ex->aux.contextus, functio_folium,
                    (i32)index_argumenti);
            }
            _locum_resolvere(ex,
                _ut_nodus(silva_c89_unarium_internum(nucleus)),
                FALSUM,
                constansne ? -I
                    : (s32)FLUXUS_EVENTUM_DEFINITIO_LOCI);
        }
        alioquin
        {
            _expressionem_ambulare(ex, arg);
        }
        index_argumenti++;
    }
}

interior vacuum
_expressionem_ambulare (FluxusExtractor* ex, constans SilvaNodus* n)
{
    n = _lectio_canonica(ex, n);
    si (n == NIHIL)
    {
        redde;
    }
    commutatio (n->genus)
    {
        casus (s32)SILVA_C89_GENUS_PARENTHESIS:
            _expressionem_ambulare(ex,
                _ut_nodus(silva_c89_parenthesis_internum(n)));
            redde;
        casus (s32)SILVA_C89_GENUS_BINARIUM:
            si (_est_sectio(n))
            {
                redde;   /* invariantum praetermissionis */
            }
            _expressionem_ambulare(ex,
                _ut_nodus(silva_c89_binarium_sinister(n)));
            _expressionem_ambulare(ex,
                _ut_nodus(silva_c89_binarium_dexter(n)));
            redde;
        casus (s32)SILVA_C89_GENUS_TERNARIUS:
            redde;       /* invariantum praetermissionis */
        casus (s32)SILVA_C89_GENUS_MAGNITUDO_EXPRESSIONIS:
        casus (s32)SILVA_C89_GENUS_MAGNITUDO_TYPI:
        casus (s32)SILVA_C89_GENUS_SPECIES_TYPI:
            redde;       /* operanda non aestimata (s18) */
        casus (s32)SILVA_C89_GENUS_CONVERSIO:
            si (_est_conversio_discardans(ex, n))
            {
                redde;   /* (vacuum)x - non usus (s01c) */
            }
            _expressionem_ambulare(ex,
                _ut_nodus(silva_c89_conversio_internum(n)));
            redde;
        casus (s32)SILVA_C89_GENUS_ASSIGNATIO:
        {
            b32 composita = (_operator_lexis(
                silva_c89_assignatio_tok_operator(n))
                    != SILVA_LEX_ASSIGNATIO);

            si (composita)
            {
                /* x += y: usus sinistri primum */
                _expressionem_ambulare(ex,
                    _ut_nodus(silva_c89_assignatio_sinister(n)));
            }
            _expressionem_ambulare(ex,
                _ut_nodus(silva_c89_assignatio_dexter(n)));
            _locum_resolvere(ex,
                _ut_nodus(silva_c89_assignatio_sinister(n)),
                FALSUM, (s32)FLUXUS_EVENTUM_DEFINITIO);
            redde;
        }
        casus (s32)SILVA_C89_GENUS_UNARIUM:
        {
            SilvaLexemaGenus lexis = _operator_lexis(
                silva_c89_unarium_tok_operator(n));

            si (lexis == SILVA_LEX_AMPERSAND)
            {
                /* & extra argumentum vocationis: def loci (v1
                 * conservativum - sedes vocationis solae
                 * constantiam probant) */
                _locum_resolvere(ex,
                    _ut_nodus(silva_c89_unarium_internum(n)),
                    FALSUM, (s32)FLUXUS_EVENTUM_DEFINITIO_LOCI);
                redde;
            }
            si (lexis == SILVA_LEX_INCREMENTUM
                || lexis == SILVA_LEX_DECREMENTUM)
            {
                _expressionem_ambulare(ex,
                    _ut_nodus(silva_c89_unarium_internum(n)));
                _locum_resolvere(ex,
                    _ut_nodus(silva_c89_unarium_internum(n)),
                    FALSUM, (s32)FLUXUS_EVENTUM_DEFINITIO);
                redde;
            }
            _expressionem_ambulare(ex,
                _ut_nodus(silva_c89_unarium_internum(n)));
            redde;
        }
        casus (s32)SILVA_C89_GENUS_POSTCREMENTUM:
            _expressionem_ambulare(ex,
                _ut_nodus(silva_c89_postcrementum_internum(n)));
            _locum_resolvere(ex,
                _ut_nodus(silva_c89_postcrementum_internum(n)),
                FALSUM, (s32)FLUXUS_EVENTUM_DEFINITIO);
            redde;
        casus (s32)SILVA_C89_GENUS_VOCATIO:
            _vocationem_ambulare(ex, n);
            redde;
        casus (s32)SILVA_C89_GENUS_SUBSCRIPTIO:
            /* lectio elementi: index legitur; basis aggregata
             * invisibilis (s05d), monstrator scalaris legitur */
            _expressionem_ambulare(ex,
                _ut_nodus(silva_c89_subscriptio_index(n)));
            _locum_resolvere(ex,
                _ut_nodus(silva_c89_subscriptio_basis(n)),
                VERUM, -I);
            redde;
        casus (s32)SILVA_C89_GENUS_ACCESSUS:
        {
            SilvaLexemaGenus lexis = _operator_lexis(
                silva_c89_accessus_tok_operator(n));

            si (lexis == SILVA_LEX_SAGITTA)
            {
                _expressionem_ambulare(ex,
                    _ut_nodus(silva_c89_accessus_basis(n)));
                redde;
            }
            /* lectio membri aggregati invisibilis (s05a) */
            _locum_resolvere(ex,
                _ut_nodus(silva_c89_accessus_basis(n)),
                VERUM, -I);
            redde;
        }
        casus (s32)SILVA_C89_GENUS_FOLIUM_IDENTIFICATOR:
        {
            FluxusSymbolumFacta facta;

            si (_facta_tracta(ex, n, &facta))
            {
                b32 proprius = (ex->declarator_identitas != NIHIL
                    && facta.identitas == ex->declarator_identitas)
                    ? VERUM : FALSUM;

                _eventum_emittere(ex, _variabilis_index(ex, &facta),
                    (s32)FLUXUS_EVENTUM_USUS, n, proprius);
            }
            redde;
        }
        casus (s32)SILVA_C89_GENUS_FOLIUM_INTEGER:
        casus (s32)SILVA_C89_GENUS_FOLIUM_FLUITANS:
        casus (s32)SILVA_C89_GENUS_FOLIUM_CHARACTER:
        casus (s32)SILVA_C89_GENUS_FOLIUM_CHORDA:
            redde;
        casus (s32)SILVA_C89_GENUS_ERROR:
        casus (s32)SILVA_C89_GENUS_AMBIGUUS:
            /* opacum: def-omnia (abstentio conservativa) */
            _eventum_emittere(ex, -I, (s32)FLUXUS_EVENTUM_DEFINITIO,
                n, FALSUM);
            redde;
        ordinarius:
            _loci_ambulare(ex, n);
            redde;
    }
}

/* ==================================================
 * Granula (sententiae, declarationes, conditiones)
 * ================================================== */

interior vacuum
_declarationem_ambulare (FluxusExtractor* ex, constans SilvaNodus* n)
{
    SilvaValor declaratores = silva_c89_declaratio_declaratores(n);
    i32 i;
    i32 m = silva_valor_lista_numerus(declaratores);

    per (i = ZEPHYRUM; i < m; i++)
    {
        SilvaValor* v = silva_valor_lista_obtinere(declaratores, i);
        constans SilvaNodus* d;

        si (v == NIHIL || v->genus != SILVA_VALOR_NODUS)
        {
            perge;
        }
        d = _lectio_canonica(ex, v->datum.nodus);
        si (d == NIHIL
            || d->genus != (s32)SILVA_C89_GENUS_DECLARATOR_INITIATUS)
        {
            perge;   /* declarator nudus: nullum eventum */
        }
        {
            /* declarator TOTUS clavis nexus (semantica eum
             * registrat; resolutor probationis descendit ipse) */
            FluxusSymbolumFacta facta;
            b32 tracta = _facta_tracta(ex, d, &facta);

            si (tracta)
            {
                ex->declarator_identitas = facta.identitas;
            }
            _expressionem_ambulare(ex,
                _ut_nodus(silva_c89_declarator_initiatus_initiator(
                    d)));
            ex->declarator_identitas = NIHIL;
            si (tracta)
            {
                _eventum_emittere(ex, _variabilis_index(ex, &facta),
                    (s32)FLUXUS_EVENTUM_DEFINITIO, d, FALSUM);
            }
        }
    }
}

interior vacuum
_granulum_ambulare (FluxusExtractor* ex, constans SilvaNodus* n)
{
    n = _lectio_canonica(ex, n);
    si (n == NIHIL)
    {
        redde;
    }
    commutatio (n->genus)
    {
        casus (s32)SILVA_C89_GENUS_DECLARATIO:
            _declarationem_ambulare(ex, n);
            redde;
        casus (s32)SILVA_C89_GENUS_SENTENTIA_EXPRESSIONIS:
            _expressionem_ambulare(ex,
                _ut_nodus(silva_c89_sententia_expressionis_expressio(
                    n)));
            redde;
        casus (s32)SILVA_C89_GENUS_REDDE:
            _expressionem_ambulare(ex,
                _ut_nodus(silva_c89_redde_valor(n)));
            redde;
        casus (s32)SILVA_C89_GENUS_SALTA:
        casus (s32)SILVA_C89_GENUS_FRANGE:
        casus (s32)SILVA_C89_GENUS_PERGE:
        casus (s32)SILVA_C89_GENUS_SENTENTIA_VACUA:
            redde;
        ordinarius:
            /* granula expressionum (conditiones, operanda fissionis,
             * initium/passus ansae per) */
            _expressionem_ambulare(ex, n);
            redde;
    }
}

/* ==================================================
 * API
 * ================================================== */

FluxusDatorum*
silva_c89_fluxus_datorum_aedificare (Piscina* piscina,
    constans FluxusFunctionis* fluxus,
    constans FluxusDatorumAuxilia* auxilia)
{
    FluxusExtractor ex;
    FluxusDatorum* datorum;
    i32 b;
    i32 numerus_blocorum;

    si (fluxus == NIHIL)
    {
        redde NIHIL;
    }
    datorum = (FluxusDatorum*)piscina_allocare(piscina,
        (i32)magnitudo(FluxusDatorum));
    datorum->fluxus = fluxus;
    datorum->variabiles = xar_creare(piscina,
        (i32)magnitudo(FluxusVariabilis));
    datorum->bloci = xar_creare(piscina,
        (i32)magnitudo(FluxusDatorumBlocus));

    ex.piscina = piscina;
    si (auxilia != NIHIL)
    {
        ex.aux = *auxilia;
    }
    alioquin
    {
        ex.aux.symbolum = NIHIL;
        ex.aux.parametrum_constans = NIHIL;
        ex.aux.canonicum = NIHIL;
        ex.aux.contextus = NIHIL;
    }
    ex.datorum = datorum;
    ex.blocus_currens = NIHIL;
    ex.declarator_identitas = NIHIL;

    numerus_blocorum = xar_numerus(fluxus->bloci);
    per (b = ZEPHYRUM; b < numerus_blocorum; b++)
    {
        constans FluxusBlocus* blocus = (constans FluxusBlocus*)
            xar_obtinere(fluxus->bloci, b);
        i32 s;
        i32 numerus_sententiarum = xar_numerus(blocus->sententiae);

        ex.blocus_currens = (FluxusDatorumBlocus*)xar_addere(
            datorum->bloci);
        ex.blocus_currens->eventa = xar_creare(piscina,
            (i32)magnitudo(FluxusEventum));
        per (s = ZEPHYRUM; s < numerus_sententiarum; s++)
        {
            constans SilvaNodus** locus = (constans SilvaNodus**)
                xar_obtinere(blocus->sententiae, s);

            _granulum_ambulare(&ex, *locus);
        }
    }
    redde datorum;
}
