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
    constans SilvaNodus*  fons_valoris_currens;  /* expressio valoris
                             * definitionis proximae (fluxus formae) -
                             * positus ante _locum_resolvere/emissionem,
                             * purgatus post */
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

/* Index densus: invenire aut addere per identitatem. Ordines
 * membrorum praetermissi (identitas basis consulto communis -
 * titulus_membri eos discriminat, inquisitio basium hic). */
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
        si (v->identitas == facta->identitas && !v->membrum_est)
        {
            redde (s32)i;
        }
    }
    v = (FluxusVariabilis*)xar_addere(ex->datorum->variabiles);
    v->identitas = facta->identitas;
    v->titulus = facta->titulus;
    v->declarans = facta->declarans;
    v->parametrum = facta->parametrum;
    v->effugit = FALSUM;
    v->membrum_est = FALSUM;
    v->basis = -I;
    v->titulus_membri.mensura = ZEPHYRUM;
    v->titulus_membri.datum = NIHIL;
    redde (s32)m;
}

interior b32
_membri_tituli_pares (chorda a, chorda b)
{
    redde (a.mensura == b.mensura
        && (a.mensura == ZEPHYRUM
            || memcmp(a.datum, b.datum,
                (memoriae_index)a.mensura) == ZEPHYRUM))
        ? VERUM : FALSUM;
}

/* Ordo membri (basis, titulus_membri): invenire aut addere.
 * Basis primum inveniatur-aut-addatur (semita lectionis ordinem
 * basis nondum creavit - lectio totius invisibilis est). Ordo
 * membri parametrum basis haeret (semen introitus machinae
 * intervallorum); effugium NUMQUAM proprium - basis consulitur. */
interior s32
_variabilis_membri_index (FluxusExtractor* ex,
    constans FluxusSymbolumFacta* facta_basis, chorda titulus_membri)
{
    s32 index_basis = _variabilis_index(ex, facta_basis);
    i32 i;
    i32 m = xar_numerus(ex->datorum->variabiles);
    FluxusVariabilis* v;

    per (i = ZEPHYRUM; i < m; i++)
    {
        v = (FluxusVariabilis*)xar_obtinere(ex->datorum->variabiles,
            i);
        si (v->membrum_est
            && v->identitas == facta_basis->identitas
            && _membri_tituli_pares(v->titulus_membri, titulus_membri))
        {
            redde (s32)i;
        }
    }
    v = (FluxusVariabilis*)xar_addere(ex->datorum->variabiles);
    v->identitas = facta_basis->identitas;
    v->titulus = facta_basis->titulus;
    v->declarans = facta_basis->declarans;
    v->parametrum = facta_basis->parametrum;
    v->effugit = FALSUM;
    v->membrum_est = VERUM;
    v->basis = index_basis;
    v->titulus_membri = titulus_membri;
    redde (s32)m;
}

/* Forma valoris (fluxus formae): classificatio syntactica, parentheses
 * perforatae, assignatio simplex in dextrum recursat (x = y = a-b).
 * NIHIL -> ALIA (crementa, scriptio membri, parametra). */
interior s32
_forma_valoris (FluxusExtractor* ex, constans SilvaNodus* n)
{
    i32 custos = ZEPHYRUM;

    dum (custos < XXXII)
    {
        custos++;
        n = _sine_parenthesibus(ex, n);
        si (n == NIHIL)
        {
            redde (s32)FLUXUS_FORMA_ALIA;
        }
        si (n->genus == (s32)SILVA_C89_GENUS_BINARIUM)
        {
            redde (_operator_lexis(silva_c89_binarium_tok_operator(n))
                    == SILVA_LEX_MINUS)
                ? (s32)FLUXUS_FORMA_SUBTRACTIO
                : (s32)FLUXUS_FORMA_ALIA;
        }
        si (n->genus == (s32)SILVA_C89_GENUS_ASSIGNATIO)
        {
            SilvaLexemaGenus lexis = _operator_lexis(
                silva_c89_assignatio_tok_operator(n));

            si (lexis == SILVA_LEX_MINUS_ASSIGNATIO)
            {
                redde (s32)FLUXUS_FORMA_SUBTRACTIO;
            }
            si (lexis == SILVA_LEX_ASSIGNATIO)
            {
                n = _ut_nodus(silva_c89_assignatio_dexter(n));
                perge;   /* valor catenae = dextrum internum */
            }
            redde (s32)FLUXUS_FORMA_ALIA;
        }
        redde (s32)FLUXUS_FORMA_ALIA;
    }
    redde (s32)FLUXUS_FORMA_ALIA;
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
    e->fons_valoris = NIHIL;
    e->forma = (s32)FLUXUS_FORMA_IGNOTA;
    e->stirps = (s32)FLUXUS_STIRPS_IGNOTA;
    si (genus == (s32)FLUXUS_EVENTUM_DEFINITIO
        || genus == (s32)FLUXUS_EVENTUM_MEMBRUM_DEFINITIO)
    {
        e->fons_valoris = ex->fons_valoris_currens;
        e->forma = _forma_valoris(ex, ex->fons_valoris_currens);
        e->stirps = (ex->aux.stirps_valoris != NIHIL)
            ? ex->aux.stirps_valoris(ex->aux.contextus,
                  ex->fons_valoris_currens)
            : (s32)FLUXUS_STIRPS_NEUTRA;
    }
    alioquin si ((genus == (s32)FLUXUS_EVENTUM_DEFINITIO_LOCI
            || genus == (s32)FLUXUS_EVENTUM_LOCI_ACCUMULAT)
        && variabilis >= ZEPHYRUM)
    {
        /* effugium: forma variabilis perpetuo MIXTA (v1 sanum) */
        FluxusVariabilis* v = (FluxusVariabilis*)xar_obtinere(
            ex->datorum->variabiles, (i32)variabilis);

        si (v != NIHIL)
        {
            v->effugit = VERUM;
        }
    }
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

/* Emissio eventi membri ad nodum ACCESSUS punctum: basis folium
 * tractum aggregatum (non acies) directum - profunditas I ipsa
 * structura. FALSUM = non emissum (basis alia: nihil, sanum). */
interior b32
_membrum_emittere (FluxusExtractor* ex, constans SilvaNodus* accessus,
    s32 genus_eventi)
{
    constans SilvaNodus* basis_nuda = _sine_parenthesibus(ex,
        _ut_nodus(silva_c89_accessus_basis(accessus)));
    FluxusSymbolumFacta facta;
    SilvaValor tit_v;

    si (basis_nuda == NIHIL || basis_nuda->genus
            != (s32)SILVA_C89_GENUS_FOLIUM_IDENTIFICATOR)
    {
        redde FALSUM;
    }
    si (!_facta_tracta(ex, basis_nuda, &facta)
        || !facta.aggregatum || facta.acies)
    {
        redde FALSUM;
    }
    tit_v = silva_c89_accessus_tok_titulus(accessus);
    si (tit_v.genus != SILVA_VALOR_TOKEN)
    {
        redde FALSUM;
    }
    _eventum_emittere(ex,
        _variabilis_membri_index(ex, &facta,
            tit_v.datum.token->valor),
        genus_eventi, accessus, FALSUM);
    redde VERUM;
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
            /* membrum ut pseudo-variabilis (01KYMYW75S): scriptio
             * DIRECTA (non per elementum - c.tab[i] membrum ipsum
             * non redefinit, def totius supra membra omnia delet)
             * eventum membri POST definitionem totius - ordo
             * dele-deinde-pone */
            si (genus_emittendi == (s32)FLUXUS_EVENTUM_DEFINITIO
                && !per_elementum)
            {
                (vacuum)_membrum_emittere(ex, n,
                    (s32)FLUXUS_EVENTUM_MEMBRUM_DEFINITIO);
            }
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
            {
                b32 accumulatne = FALSUM;

                si (!constansne
                    && ex->aux.parametrum_accumulat != NIHIL)
                {
                    accumulatne = ex->aux.parametrum_accumulat(
                        ex->aux.contextus, functio_folium,
                        (i32)index_argumenti);
                }
                _locum_resolvere(ex,
                    _ut_nodus(silva_c89_unarium_internum(nucleus)),
                    FALSUM,
                    constansne ? -I
                        : accumulatne
                            ? (s32)FLUXUS_EVENTUM_LOCI_ACCUMULAT
                            : (s32)FLUXUS_EVENTUM_DEFINITIO_LOCI);
            }
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
            /* fons valoris (fluxus formae): composita = nodus ipse
             * (operator formam fert), simplex = dextrum */
            ex->fons_valoris_currens = composita
                ? n : _ut_nodus(silva_c89_assignatio_dexter(n));
            _locum_resolvere(ex,
                _ut_nodus(silva_c89_assignatio_sinister(n)),
                FALSUM, (s32)FLUXUS_EVENTUM_DEFINITIO);
            ex->fons_valoris_currens = NIHIL;
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
            si (ex->aux.expressio_acies != NIHIL
                && ex->aux.expressio_acies(ex->aux.contextus, n))
            {
                /* membrum-acies positione valoris DECADIT: locus
                 * in aggregatum sumptus (memset(r.c,...)) */
                _locum_resolvere(ex,
                    _ut_nodus(silva_c89_accessus_basis(n)),
                    FALSUM, (s32)FLUXUS_EVENTUM_DEFINITIO_LOCI);
                redde;
            }
            /* membrum ut pseudo-variabilis (01KYMYW75S): lectio
             * membri directi eventum proprium fert (lectio TOTIUS
             * invisibilis manet - pinna s05a intacta) */
            (vacuum)_membrum_emittere(ex, n,
                (s32)FLUXUS_EVENTUM_MEMBRUM_USUS);
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

                si (facta.acies)
                {
                    /* decasus aciei (T[] -> T*): locus sumptus,
                     * non lectio - clang idem tacet */
                    _eventum_emittere(ex,
                        _variabilis_index(ex, &facta),
                        (s32)FLUXUS_EVENTUM_DEFINITIO_LOCI, n,
                        FALSUM);
                    redde;
                }
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
                ex->fons_valoris_currens = _ut_nodus(
                    silva_c89_declarator_initiatus_initiator(d));
                _eventum_emittere(ex, _variabilis_index(ex, &facta),
                    (s32)FLUXUS_EVENTUM_DEFINITIO, d, FALSUM);
                ex->fons_valoris_currens = NIHIL;
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
 * Punctum fixum may/must (chunk B)
 *
 * Impulsus-successorum: blocus cuius exitus mutatur contributiones
 * per margines suos EXEUNTES pellit (may |= , must &=) - index
 * praedecessorum non necessarius (chunk C eum aedificat pro
 * ambulatione inevitabilitatis). Bloci numquam processi (
 * inattingibiles, rami plicati) nihil pellunt: regula
 * praedecessorum-invisorum STRUCTURALIS.
 * ================================================== */

interior vacuum
_verba_implere (i64* verba, i32 numerus, i64 valor)
{
    i32 i;

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        verba[i] = valor;
    }
}

interior vacuum
_bitum_ponere (i64* verba, s32 index)
{
    verba[index / LXIV] |= (i64)I << (i32)(index % LXIV);
}

/* Exitus = introitus + definitiones eventorum (USUS iners hic) */
interior vacuum
_exitum_computare (FluxusDatorum* datorum, FluxusDatorumBlocus* b)
{
    i32 n = datorum->numerus_verborum;
    i32 e;
    i32 m = xar_numerus(b->eventa);

    memcpy(b->may_exitus, b->may_introitus,
        (memoriae_index)n * magnitudo(i64));
    memcpy(b->must_exitus, b->must_introitus,
        (memoriae_index)n * magnitudo(i64));
    per (e = ZEPHYRUM; e < m; e++)
    {
        FluxusEventum* ev = (FluxusEventum*)xar_obtinere(b->eventa,
            e);

        si (ev->genus != (s32)FLUXUS_EVENTUM_DEFINITIO
            && ev->genus != (s32)FLUXUS_EVENTUM_DEFINITIO_LOCI)
        {
            perge;
        }
        si (ev->variabilis < ZEPHYRUM)
        {
            /* def-omnia (folium opacum) */
            _verba_implere(b->may_exitus, n, ~(i64)ZEPHYRUM);
            _verba_implere(b->must_exitus, n, ~(i64)ZEPHYRUM);
        }
        alioquin
        {
            _bitum_ponere(b->may_exitus, ev->variabilis);
            _bitum_ponere(b->must_exitus, ev->variabilis);
        }
    }
}

interior vacuum
_punctum_fixum (Piscina* piscina, FluxusDatorum* datorum)
{
    i32 n_var = xar_numerus(datorum->variabiles);
    i32 n_verba = (n_var + (i32)LXIV - I) / (i32)LXIV;
    i32 numerus_blocorum = xar_numerus(datorum->bloci);
    i32 b;
    b32* in_indice;
    Xar* index_operis;
    i32 lector = ZEPHYRUM;

    si (n_verba == ZEPHYRUM)
    {
        n_verba = I;   /* verbum unum etiam sine variabilibus */
    }
    datorum->numerus_verborum = n_verba;

    per (b = ZEPHYRUM; b < numerus_blocorum; b++)
    {
        FluxusDatorumBlocus* db = (FluxusDatorumBlocus*)xar_obtinere(
            datorum->bloci, b);

        db->may_introitus = (i64*)piscina_allocare(piscina,
            (memoriae_index)n_verba * magnitudo(i64));
        db->may_exitus = (i64*)piscina_allocare(piscina,
            (memoriae_index)n_verba * magnitudo(i64));
        db->must_introitus = (i64*)piscina_allocare(piscina,
            (memoriae_index)n_verba * magnitudo(i64));
        db->must_exitus = (i64*)piscina_allocare(piscina,
            (memoriae_index)n_verba * magnitudo(i64));
        _verba_implere(db->may_introitus, n_verba, ZEPHYRUM);
        _verba_implere(db->may_exitus, n_verba, ZEPHYRUM);
        _verba_implere(db->must_introitus, n_verba, ~(i64)ZEPHYRUM);
        _verba_implere(db->must_exitus, n_verba, ~(i64)ZEPHYRUM);
    }

    /* introitus: bita parametrorum sola (may = must = parametra) */
    {
        FluxusDatorumBlocus* db = (FluxusDatorumBlocus*)xar_obtinere(
            datorum->bloci, datorum->fluxus->introitus->index);
        i32 v;

        _verba_implere(db->must_introitus, n_verba, ZEPHYRUM);
        per (v = ZEPHYRUM; v < n_var; v++)
        {
            FluxusVariabilis* var = (FluxusVariabilis*)xar_obtinere(
                datorum->variabiles, v);

            si (var->parametrum)
            {
                _bitum_ponere(db->may_introitus, (s32)v);
                _bitum_ponere(db->must_introitus, (s32)v);
            }
        }
    }

    in_indice = (b32*)piscina_allocare(piscina,
        (memoriae_index)numerus_blocorum * magnitudo(b32));
    per (b = ZEPHYRUM; b < numerus_blocorum; b++)
    {
        in_indice[b] = FALSUM;
    }
    index_operis = xar_creare(piscina, (i32)magnitudo(i32));
    {
        i32* locus = (i32*)xar_addere(index_operis);

        *locus = datorum->fluxus->introitus->index;
        in_indice[datorum->fluxus->introitus->index] = VERUM;
    }

    dum (lector < xar_numerus(index_operis))
    {
        i32 index_bloci = *(i32*)xar_obtinere(index_operis, lector);
        FluxusDatorumBlocus* db = (FluxusDatorumBlocus*)xar_obtinere(
            datorum->bloci, index_bloci);
        constans FluxusBlocus* fb = (constans FluxusBlocus*)
            xar_obtinere(datorum->fluxus->bloci, index_bloci);
        i32 m;
        i32 k;

        lector++;
        in_indice[index_bloci] = FALSUM;
        _exitum_computare(datorum, db);

        m = xar_numerus(fb->margines);
        per (k = ZEPHYRUM; k < m; k++)
        {
            constans FluxusMargo* margo = (constans FluxusMargo*)
                xar_obtinere(fb->margines, k);
            FluxusDatorumBlocus* dd;
            b32 mutatum = FALSUM;
            i32 w;

            si (margo->destinatio == NIHIL)
            {
                perge;
            }
            dd = (FluxusDatorumBlocus*)xar_obtinere(datorum->bloci,
                margo->destinatio->index);
            per (w = ZEPHYRUM; w < n_verba; w++)
            {
                i64 may_novum = dd->may_introitus[w]
                    | db->may_exitus[w];
                i64 must_novum = dd->must_introitus[w]
                    & db->must_exitus[w];

                si (may_novum != dd->may_introitus[w]
                    || must_novum != dd->must_introitus[w])
                {
                    dd->may_introitus[w] = may_novum;
                    dd->must_introitus[w] = must_novum;
                    mutatum = VERUM;
                }
            }
            si (mutatum && !in_indice[margo->destinatio->index])
            {
                i32* locus = (i32*)xar_addere(index_operis);

                *locus = margo->destinatio->index;
                in_indice[margo->destinatio->index] = VERUM;
            }
        }
    }
}

/* ==================================================
 * Punctum fixum formarum (fluxus formae)
 *
 * Ad exemplar chunk B (impulsus-successorum, index operis idem),
 * sed iunctio reticuli non bitalis: IGNOTA identitas, aequales
 * manent, dissentientes MIXTA. Effugium NON hic tractatur -
 * variabilis.effugit ad tempus quaestionis consulitur (perpetuum).
 * ================================================== */

interior s32
_formam_iungere (s32 a, s32 b)
{
    si (a == (s32)FLUXUS_FORMA_IGNOTA)
    {
        redde b;
    }
    si (b == (s32)FLUXUS_FORMA_IGNOTA || a == b)
    {
        redde a;
    }
    redde (s32)FLUXUS_FORMA_MIXTA;
}

/* Exitus = introitus + definitiones (replay ordine eventorum) */
interior vacuum
_formae_exitum_computare (FluxusDatorum* datorum,
    FluxusDatorumBlocus* b)
{
    i32 n_var = xar_numerus(datorum->variabiles);
    i32 v;
    i32 e;
    i32 m = xar_numerus(b->eventa);

    per (v = ZEPHYRUM; v < n_var; v++)
    {
        b->formae_exitus[v] = b->formae_introitus[v];
    }
    per (e = ZEPHYRUM; e < m; e++)
    {
        FluxusEventum* ev = (FluxusEventum*)xar_obtinere(b->eventa,
            e);

        si (ev->genus != (s32)FLUXUS_EVENTUM_DEFINITIO)
        {
            perge;
        }
        si (ev->variabilis < ZEPHYRUM)
        {
            /* def-omnia (folium opacum): omnes MIXTA */
            per (v = ZEPHYRUM; v < n_var; v++)
            {
                b->formae_exitus[v] = (s32)FLUXUS_FORMA_MIXTA;
            }
        }
        alioquin
        {
            b->formae_exitus[ev->variabilis] = ev->forma;
        }
    }
}

interior vacuum
_punctum_fixum_formarum (Piscina* piscina, FluxusDatorum* datorum)
{
    i32 n_var = xar_numerus(datorum->variabiles);
    i32 n_loci = (n_var > ZEPHYRUM) ? n_var : I;
    i32 numerus_blocorum = xar_numerus(datorum->bloci);
    i32 b;
    i32 v;
    b32* in_indice;
    Xar* index_operis;
    i32 lector = ZEPHYRUM;

    per (b = ZEPHYRUM; b < numerus_blocorum; b++)
    {
        FluxusDatorumBlocus* db = (FluxusDatorumBlocus*)xar_obtinere(
            datorum->bloci, b);

        db->formae_introitus = (s32*)piscina_allocare(piscina,
            (memoriae_index)n_loci * magnitudo(s32));
        db->formae_exitus = (s32*)piscina_allocare(piscina,
            (memoriae_index)n_loci * magnitudo(s32));
        per (v = ZEPHYRUM; v < n_loci; v++)
        {
            db->formae_introitus[v] = (s32)FLUXUS_FORMA_IGNOTA;
            db->formae_exitus[v] = (s32)FLUXUS_FORMA_IGNOTA;
        }
    }

    /* introitus: parametra formam ALIA ferunt (definita, non
     * subtractione) */
    {
        FluxusDatorumBlocus* db = (FluxusDatorumBlocus*)xar_obtinere(
            datorum->bloci, datorum->fluxus->introitus->index);

        per (v = ZEPHYRUM; v < n_var; v++)
        {
            FluxusVariabilis* var = (FluxusVariabilis*)xar_obtinere(
                datorum->variabiles, v);

            si (var->parametrum)
            {
                db->formae_introitus[v] = (s32)FLUXUS_FORMA_ALIA;
            }
        }
    }

    in_indice = (b32*)piscina_allocare(piscina,
        (memoriae_index)numerus_blocorum * magnitudo(b32));
    per (b = ZEPHYRUM; b < numerus_blocorum; b++)
    {
        in_indice[b] = FALSUM;
    }
    index_operis = xar_creare(piscina, (i32)magnitudo(i32));
    {
        i32* locus = (i32*)xar_addere(index_operis);

        *locus = datorum->fluxus->introitus->index;
        in_indice[datorum->fluxus->introitus->index] = VERUM;
    }

    dum (lector < xar_numerus(index_operis))
    {
        i32 index_bloci = *(i32*)xar_obtinere(index_operis, lector);
        FluxusDatorumBlocus* db = (FluxusDatorumBlocus*)xar_obtinere(
            datorum->bloci, index_bloci);
        constans FluxusBlocus* fb = (constans FluxusBlocus*)
            xar_obtinere(datorum->fluxus->bloci, index_bloci);
        i32 m;
        i32 k;

        lector++;
        in_indice[index_bloci] = FALSUM;
        _formae_exitum_computare(datorum, db);

        m = xar_numerus(fb->margines);
        per (k = ZEPHYRUM; k < m; k++)
        {
            constans FluxusMargo* margo = (constans FluxusMargo*)
                xar_obtinere(fb->margines, k);
            FluxusDatorumBlocus* dd;
            b32 mutatum = FALSUM;

            si (margo->destinatio == NIHIL)
            {
                perge;
            }
            dd = (FluxusDatorumBlocus*)xar_obtinere(datorum->bloci,
                margo->destinatio->index);
            per (v = ZEPHYRUM; v < n_var; v++)
            {
                s32 novum = _formam_iungere(dd->formae_introitus[v],
                    db->formae_exitus[v]);

                si (novum != dd->formae_introitus[v])
                {
                    dd->formae_introitus[v] = novum;
                    mutatum = VERUM;
                }
            }
            si (mutatum && !in_indice[margo->destinatio->index])
            {
                i32* locus = (i32*)xar_addere(index_operis);

                *locus = margo->destinatio->index;
                in_indice[margo->destinatio->index] = VERUM;
            }
        }
    }
}

/* ==================================================
 * Punctum fixum stirpium (vestigatio generum, codex 82)
 *
 * Instantia TERTIA formae reticuli (post may/must et formas):
 * IGNOTA identitas, aequales manent, dissentientes AMISSA.
 * Parametra NEUTRA seruntur (sedes vocationis iam iudicata;
 * provenientia interprocedualis = ianua nominata). Effugium ad
 * tempus quaestionis consulitur (variabilis.effugit), non hic.
 * ================================================== */

interior s32
_stirpem_iungere (s32 a, s32 b)
{
    si (a == (s32)FLUXUS_STIRPS_IGNOTA)
    {
        redde b;
    }
    si (b == (s32)FLUXUS_STIRPS_IGNOTA || a == b)
    {
        redde a;
    }
    redde (s32)FLUXUS_STIRPS_AMISSA;
}

/* Exitus = introitus + definitiones (replay ordine eventorum);
 * eventa membrorum cribrata (lex eventorum additivorum).
 * Classificatio ambitu-conscia (aux, v2): def contra tabulam
 * exitus MEDIO-renovatam - ordo eventorum = status currens verus
 * (usus-ante-def compositorum ordinem servat); involucro absente
 * columna caeca ev->stirps. */
interior vacuum
_stirpes_exitum_computare (FluxusDatorum* datorum,
    FluxusDatorumBlocus* b, constans FluxusDatorumAuxilia* aux)
{
    i32 n_var = xar_numerus(datorum->variabiles);
    i32 v;
    i32 e;
    i32 m = xar_numerus(b->eventa);

    per (v = ZEPHYRUM; v < n_var; v++)
    {
        b->stirpes_exitus[v] = b->stirpes_introitus[v];
    }
    per (e = ZEPHYRUM; e < m; e++)
    {
        FluxusEventum* ev = (FluxusEventum*)xar_obtinere(b->eventa,
            e);

        si (ev->genus != (s32)FLUXUS_EVENTUM_DEFINITIO)
        {
            perge;
        }
        si (ev->variabilis < ZEPHYRUM)
        {
            /* def-omnia (folium opacum): provenientia perit */
            per (v = ZEPHYRUM; v < n_var; v++)
            {
                b->stirpes_exitus[v] = (s32)FLUXUS_STIRPS_AMISSA;
            }
        }
        alioquin
        {
            b->stirpes_exitus[ev->variabilis] =
                (aux != NIHIL && aux->stirps_valoris_ambitu != NIHIL)
                ? aux->stirps_valoris_ambitu(aux->contextus,
                      ev->fons_valoris, datorum, b->stirpes_exitus)
                : ev->stirps;
        }
    }
}

interior vacuum
_punctum_fixum_stirpium (Piscina* piscina, FluxusDatorum* datorum,
    constans FluxusDatorumAuxilia* aux)
{
    i32 n_var = xar_numerus(datorum->variabiles);
    i32 n_loci = (n_var > ZEPHYRUM) ? n_var : I;
    i32 numerus_blocorum = xar_numerus(datorum->bloci);
    i32 b;
    i32 v;
    b32* in_indice;
    Xar* index_operis;
    i32 lector = ZEPHYRUM;

    per (b = ZEPHYRUM; b < numerus_blocorum; b++)
    {
        FluxusDatorumBlocus* db = (FluxusDatorumBlocus*)xar_obtinere(
            datorum->bloci, b);

        db->stirpes_introitus = (s32*)piscina_allocare(piscina,
            (memoriae_index)n_loci * magnitudo(s32));
        db->stirpes_exitus = (s32*)piscina_allocare(piscina,
            (memoriae_index)n_loci * magnitudo(s32));
        per (v = ZEPHYRUM; v < n_loci; v++)
        {
            db->stirpes_introitus[v] = (s32)FLUXUS_STIRPS_IGNOTA;
            db->stirpes_exitus[v] = (s32)FLUXUS_STIRPS_IGNOTA;
        }
    }

    /* introitus: parametra stirpem NEUTRAM ferunt */
    {
        FluxusDatorumBlocus* db = (FluxusDatorumBlocus*)xar_obtinere(
            datorum->bloci, datorum->fluxus->introitus->index);

        per (v = ZEPHYRUM; v < n_var; v++)
        {
            FluxusVariabilis* var = (FluxusVariabilis*)xar_obtinere(
                datorum->variabiles, v);

            si (var->parametrum)
            {
                db->stirpes_introitus[v] = (s32)FLUXUS_STIRPS_NEUTRA;
            }
        }
    }

    in_indice = (b32*)piscina_allocare(piscina,
        (memoriae_index)numerus_blocorum * magnitudo(b32));
    per (b = ZEPHYRUM; b < numerus_blocorum; b++)
    {
        in_indice[b] = FALSUM;
    }
    index_operis = xar_creare(piscina, (i32)magnitudo(i32));
    {
        i32* locus = (i32*)xar_addere(index_operis);

        *locus = datorum->fluxus->introitus->index;
        in_indice[datorum->fluxus->introitus->index] = VERUM;
    }

    dum (lector < xar_numerus(index_operis))
    {
        i32 index_bloci = *(i32*)xar_obtinere(index_operis, lector);
        FluxusDatorumBlocus* db = (FluxusDatorumBlocus*)xar_obtinere(
            datorum->bloci, index_bloci);
        constans FluxusBlocus* fb = (constans FluxusBlocus*)
            xar_obtinere(datorum->fluxus->bloci, index_bloci);
        i32 m;
        i32 k;

        lector++;
        in_indice[index_bloci] = FALSUM;
        _stirpes_exitum_computare(datorum, db, aux);

        m = xar_numerus(fb->margines);
        per (k = ZEPHYRUM; k < m; k++)
        {
            constans FluxusMargo* margo = (constans FluxusMargo*)
                xar_obtinere(fb->margines, k);
            FluxusDatorumBlocus* dd;
            b32 mutatum = FALSUM;

            si (margo->destinatio == NIHIL)
            {
                perge;
            }
            dd = (FluxusDatorumBlocus*)xar_obtinere(datorum->bloci,
                margo->destinatio->index);
            per (v = ZEPHYRUM; v < n_var; v++)
            {
                s32 novum = _stirpem_iungere(
                    dd->stirpes_introitus[v], db->stirpes_exitus[v]);

                si (novum != dd->stirpes_introitus[v])
                {
                    dd->stirpes_introitus[v] = novum;
                    mutatum = VERUM;
                }
            }
            si (mutatum && !in_indice[margo->destinatio->index])
            {
                i32* locus = (i32*)xar_addere(index_operis);

                *locus = margo->destinatio->index;
                in_indice[margo->destinatio->index] = VERUM;
            }
        }
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
        ex.aux.parametrum_accumulat = NIHIL;
        ex.aux.expressio_acies = NIHIL;
        ex.aux.stirps_valoris = NIHIL;
        ex.aux.stirps_valoris_ambitu = NIHIL;
        ex.aux.canonicum = NIHIL;
        ex.aux.contextus = NIHIL;
    }
    ex.datorum = datorum;
    ex.blocus_currens = NIHIL;
    ex.declarator_identitas = NIHIL;
    ex.fons_valoris_currens = NIHIL;

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

    /* chunk B: punctum fixum may/must super eventa extracta */
    _punctum_fixum(piscina, datorum);

    /* fluxus formae: punctum fixum alterum (formae definitionum) */
    _punctum_fixum_formarum(piscina, datorum);

    /* vestigatio generum: punctum fixum tertium (stirpes signatae) */
    _punctum_fixum_stirpium(piscina, datorum, &ex.aux);

    redde datorum;
}
