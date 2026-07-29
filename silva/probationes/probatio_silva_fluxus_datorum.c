/* probatio_silva_fluxus_datorum.c - FLUXUS-1 chunk A: extractio
 * eventorum def/usus. Resolutor probationis nomine-clavatus (facta
 * manu data - semantica non tangitur); identitas = ordo tabulae.
 * Regulae probatae = pinnae calibrationis (s01-s20). */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "silva_token.h"
#include "silva_nodus.h"
#include "silva_parsare.h"
#include "silva_commissio.h"
#include "silva_c89_oraculum.h"
#include "silva_tabulae_c89.h"
#include "silva_c89_fluxus.h"
#include "silva_c89_fluxus_datorum.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

/* ==================================================
 * Resolutor probationis (nomine-clavatus)
 * ================================================== */

nomen structura {
    constans character* titulus;
    b32 localis;
    b32 parametrum;
    b32 aggregatum;
    b32 acies;
} ProbatioSymbolum;

/* Partes fixae: a/b/c/i parametra, x/y/p locales scalares,
 * s/arr locales aggregata. Functiones (g, peek, sinkp) ABSENT =
 * non resolutae = nulla eventa. */
hic_manens ProbatioSymbolum _symbola[] = {
    { "a",   FALSUM, VERUM,  FALSUM, FALSUM },
    { "b",   FALSUM, VERUM,  FALSUM, FALSUM },
    { "c",   FALSUM, VERUM,  FALSUM, FALSUM },
    { "i",   FALSUM, VERUM,  FALSUM, FALSUM },
    { "x",   VERUM,  FALSUM, FALSUM, FALSUM },
    { "y",   VERUM,  FALSUM, FALSUM, FALSUM },
    { "p",   VERUM,  FALSUM, FALSUM, FALSUM },
    { "s",   VERUM,  FALSUM, VERUM,  FALSUM },
    { "arr", VERUM,  FALSUM, VERUM,  VERUM  }
};

interior b32
_nomen_aequale (chorda c, constans character* litterae)
{
    memoriae_index m = strlen(litterae);

    redde (c.mensura == (i32)m && c.datum != NIHIL
        && memcmp(c.datum, litterae, m) == ZEPHYRUM) ? VERUM : FALSUM;
}

interior constans SilvaNodus*
_nodalis_probationis (SilvaValor v)
{
    si (v.genus != SILVA_VALOR_NODUS)
    {
        redde NIHIL;
    }
    redde v.datum.nodus;
}

/* Descensus declaratoris ad titulum (resolutor probationis -
 * semantica per nexum declaratorum resolvit) */
interior constans SilvaNodus*
_declaratoris_titulus (constans SilvaNodus* d)
{
    dum (d != NIHIL)
    {
        commutatio (d->genus)
        {
            casus (s32)SILVA_C89_GENUS_DECLARATOR_TITULUS:
                redde d;
            casus (s32)SILVA_C89_GENUS_DECLARATOR_INITIATUS:
                d = _nodalis_probationis(
                    silva_c89_declarator_initiatus_declarator(d));
                frange;
            casus (s32)SILVA_C89_GENUS_DECLARATOR_MONSTRATOR:
                d = _nodalis_probationis(
                    silva_c89_declarator_monstrator_internum(d));
                frange;
            casus (s32)SILVA_C89_GENUS_DECLARATOR_ACIEI:
                d = _nodalis_probationis(
                    silva_c89_declarator_aciei_internum(d));
                frange;
            casus (s32)SILVA_C89_GENUS_PARENTHESIS:
                d = _nodalis_probationis(
                    silva_c89_parenthesis_internum(d));
                frange;
            ordinarius:
                redde NIHIL;
        }
    }
    redde NIHIL;
}

interior b32
_probatio_symbolum (vacuum* contextus, constans SilvaNodus* nodus,
    FluxusSymbolumFacta* facta)
{
    SilvaValor tok_v;
    chorda textus;
    i32 k;
    i32 m = (i32)(magnitudo(_symbola) / magnitudo(_symbola[0]));

    (vacuum)contextus;
    si (nodus == NIHIL)
    {
        redde FALSUM;
    }
    si (nodus->genus == (s32)SILVA_C89_GENUS_FOLIUM_IDENTIFICATOR)
    {
        tok_v = silva_c89_folium_identificator_tok_valor(nodus);
    }
    alioquin
    {
        constans SilvaNodus* titulus = _declaratoris_titulus(nodus);

        si (titulus == NIHIL)
        {
            redde FALSUM;
        }
        tok_v = silva_c89_declarator_titulus_tok_titulus(titulus);
    }
    si (tok_v.genus != SILVA_VALOR_TOKEN)
    {
        redde FALSUM;
    }
    textus = tok_v.datum.token->valor;
    per (k = ZEPHYRUM; k < m; k++)
    {
        si (_nomen_aequale(textus, _symbola[k].titulus))
        {
            facta->identitas = (vacuum*)&_symbola[k];
            facta->titulus = textus;
            facta->declarans = nodus;
            facta->localis_automata = _symbola[k].localis;
            facta->parametrum = _symbola[k].parametrum;
            facta->aggregatum = _symbola[k].aggregatum;
            facta->acies = _symbola[k].acies;
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* peek = functio cum parametris T* constantibus (pinna s04f) */
interior b32
_probatio_parametrum_constans (vacuum* contextus,
    constans SilvaNodus* functio_folium, i32 index)
{
    SilvaValor tok_v;

    (vacuum)contextus;
    (vacuum)index;
    si (functio_folium == NIHIL
        || functio_folium->genus
            != (s32)SILVA_C89_GENUS_FOLIUM_IDENTIFICATOR)
    {
        redde FALSUM;
    }
    tok_v = silva_c89_folium_identificator_tok_valor(functio_folium);
    si (tok_v.genus != SILVA_VALOR_TOKEN)
    {
        redde FALSUM;
    }
    redde _nomen_aequale(tok_v.datum.token->valor, "peek");
}

/* ==================================================
 * Fistula: fons -> CFG -> datorum
 * ================================================== */

interior constans SilvaNodus*
_definitio_prima (constans SilvaParsura* parsura)
{
    SilvaValor* e = silva_valor_lista_obtinere(
        parsura->commissio->radix, ZEPHYRUM);

    si (e == NIHIL || e->genus != SILVA_VALOR_NODUS)
    {
        redde NIHIL;
    }
    redde e->datum.nodus;
}

/* Aestimator litteralis parvus (exemplar probatio_silva_fluxus):
 * folium-integer decimale per parentheses - plicatio si (0) */
interior b32
_aestimator_litteralis (vacuum* contextus,
    constans SilvaNodus* expressio, s64* valor)
{
    SilvaValor tok_v;
    chorda textus;
    s64 v = ZEPHYRUM;
    i32 i;

    (vacuum)contextus;
    dum (expressio != NIHIL
        && expressio->genus == (s32)SILVA_C89_GENUS_PARENTHESIS)
    {
        expressio = _nodalis_probationis(
            silva_c89_parenthesis_internum(expressio));
    }
    si (expressio == NIHIL
        || expressio->genus != (s32)SILVA_C89_GENUS_FOLIUM_INTEGER)
    {
        redde FALSUM;
    }
    tok_v = silva_c89_folium_integer_tok_valor(expressio);
    si (tok_v.genus != SILVA_VALOR_TOKEN)
    {
        redde FALSUM;
    }
    textus = tok_v.datum.token->valor;
    si (textus.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < textus.mensura; i++)
    {
        character c = (character)textus.datum[i];

        si (c < '0' || c > '9')
        {
            redde FALSUM;
        }
        v = v * X + (s64)(c - '0');
    }
    *valor = v;
    redde VERUM;
}

/* Classificator stirpium probationis (ambitu caecus): folium 'a'
 * -> PRIMA, folium 'b' -> PRIMA+1, cetera NEUTRA */
interior s32
_probatio_stirps (vacuum* contextus, constans SilvaNodus* expressio)
{
    SilvaValor tok_v;

    (vacuum)contextus;
    dum (expressio != NIHIL
        && expressio->genus == (s32)SILVA_C89_GENUS_PARENTHESIS)
    {
        expressio = _nodalis_probationis(
            silva_c89_parenthesis_internum(expressio));
    }
    si (expressio == NIHIL || expressio->genus
        != (s32)SILVA_C89_GENUS_FOLIUM_IDENTIFICATOR)
    {
        redde (s32)FLUXUS_STIRPS_NEUTRA;
    }
    tok_v = silva_c89_folium_identificator_tok_valor(expressio);
    si (tok_v.genus != SILVA_VALOR_TOKEN)
    {
        redde (s32)FLUXUS_STIRPS_NEUTRA;
    }
    si (_nomen_aequale(tok_v.datum.token->valor, "a"))
    {
        redde (s32)FLUXUS_STIRPS_PRIMA;
    }
    si (_nomen_aequale(tok_v.datum.token->valor, "b"))
    {
        redde (s32)FLUXUS_STIRPS_PRIMA + I;
    }
    redde (s32)FLUXUS_STIRPS_NEUTRA;
}

/* Classificator AMBITU-CONSCIUS probationis: folium 'a' -> PRIMA
 * (semen); cetera = status currens variabilis 'x' (echo -
 * sui-referentia servat, exemplar t = t - m) */
interior s32
_probatio_stirps_ambitu (vacuum* contextus,
    constans SilvaNodus* expressio,
    constans FluxusDatorum* datorum, constans s32* stirpes)
{
    (vacuum)contextus;
    {
        constans SilvaNodus* e = expressio;

        dum (e != NIHIL
            && e->genus == (s32)SILVA_C89_GENUS_PARENTHESIS)
        {
            e = _nodalis_probationis(
                silva_c89_parenthesis_internum(e));
        }
        si (e != NIHIL && e->genus
            == (s32)SILVA_C89_GENUS_FOLIUM_IDENTIFICATOR)
        {
            SilvaValor tok_v =
                silva_c89_folium_identificator_tok_valor(e);

            si (tok_v.genus == SILVA_VALOR_TOKEN
                && _nomen_aequale(tok_v.datum.token->valor, "a"))
            {
                redde (s32)FLUXUS_STIRPS_PRIMA;
            }
        }
    }
    {
        i32 i;
        i32 m = xar_numerus(datorum->variabiles);

        per (i = ZEPHYRUM; i < m; i++)
        {
            constans FluxusVariabilis* v =
                (constans FluxusVariabilis*)xar_obtinere(
                    datorum->variabiles, i);

            si (v != NIHIL && !v->membrum_est
                && _nomen_aequale(v->titulus, "x"))
            {
                redde stirpes[i];
            }
        }
    }
    redde (s32)FLUXUS_STIRPS_NEUTRA;
}

interior FluxusDatorum*
_extrahere_cum (Piscina* piscina, constans character* fons,
    s32 (*stirps_cb)(vacuum*, constans SilvaNodus*),
    s32 (*ambitu_cb)(vacuum*, constans SilvaNodus*,
        constans FluxusDatorum*, constans s32*))
{
    SilvaParsura* parsura = silva_c89_parsare(piscina, "probatio.c",
        fons, (i32)strlen(fons), NIHIL);
    constans SilvaNodus* definitio;
    FluxusFunctionis* fluxus;
    FluxusAuxilia aux_fluxus;
    FluxusDatorumAuxilia aux;

    si (parsura == NIHIL || parsura->numerus_errorum != ZEPHYRUM)
    {
        redde NIHIL;
    }
    definitio = _definitio_prima(parsura);
    si (definitio == NIHIL)
    {
        redde NIHIL;
    }
    aux_fluxus.canonicum = NIHIL;
    aux_fluxus.aestimator = _aestimator_litteralis;
    aux_fluxus.contextus = NIHIL;
    fluxus = silva_c89_fluxus_aedificare(piscina, definitio,
        &aux_fluxus);
    si (fluxus == NIHIL)
    {
        redde NIHIL;
    }
    aux.symbolum = _probatio_symbolum;
    aux.parametrum_constans = _probatio_parametrum_constans;
    aux.parametrum_accumulat = NIHIL;
    aux.expressio_acies = NIHIL;   /* e2e semanticae hoc probat */
    aux.stirps_valoris = stirps_cb;
    aux.stirps_valoris_ambitu = ambitu_cb;
    aux.canonicum = NIHIL;
    aux.contextus = NIHIL;
    redde silva_c89_fluxus_datorum_aedificare(piscina, fluxus, &aux);
}

interior FluxusDatorum*
_extrahere (Piscina* piscina, constans character* fons)
{
    redde _extrahere_cum(piscina, fons, NIHIL, NIHIL);
}

/* ==================================================
 * Probatio seriei eventorum (plana, ordine blocorum)
 * ================================================== */

nomen structura {
    constans character* titulus;   /* "*" = def-omnia (variabilis -1) */
    s32 genus;
    b32 proprius;                /* in_initiatore_proprio */
} EventumExspectatum;

interior vacuum
_seriem_probare (FluxusDatorum* datorum,
    constans EventumExspectatum* exspectata, i32 numerus)
{
    i32 visa = ZEPHYRUM;
    i32 b;
    i32 numerus_blocorum = xar_numerus(datorum->bloci);

    per (b = ZEPHYRUM; b < numerus_blocorum; b++)
    {
        FluxusDatorumBlocus* blocus = (FluxusDatorumBlocus*)
            xar_obtinere(datorum->bloci, b);
        i32 e;
        i32 m = xar_numerus(blocus->eventa);

        per (e = ZEPHYRUM; e < m; e++)
        {
            FluxusEventum* ev = (FluxusEventum*)xar_obtinere(
                blocus->eventa, e);

            /* eventa membrorum additiva (01KYMYW75S) praetermissa -
             * disciplina consumptorum verorum speculata (genera
             * explicite cribrant); series membrorum probationem
             * propriam habet infra */
            si (ev->genus == (s32)FLUXUS_EVENTUM_MEMBRUM_USUS
                || ev->genus
                    == (s32)FLUXUS_EVENTUM_MEMBRUM_DEFINITIO)
            {
                perge;
            }
            si (visa < numerus)
            {
                constans EventumExspectatum* ex = &exspectata[visa];

                CREDO_AEQUALIS_S32 (ev->genus, ex->genus);
                CREDO_VERUM (ev->in_initiatore_proprio
                    == ex->proprius);
                si (ev->variabilis < ZEPHYRUM)
                {
                    /* def-omnia (folium opacum) */
                    CREDO_VERUM (strcmp(ex->titulus, "*") == ZEPHYRUM);
                }
                alioquin
                {
                    FluxusVariabilis* v = (FluxusVariabilis*)
                        xar_obtinere(datorum->variabiles,
                            (i32)ev->variabilis);

                    CREDO_VERUM (_nomen_aequale(v->titulus,
                        ex->titulus));
                }
            }
            visa++;
        }
    }
    CREDO_AEQUALIS_I32 (visa, numerus);
}

#define USUS_  (s32)FLUXUS_EVENTUM_USUS
#define DEF_   (s32)FLUXUS_EVENTUM_DEFINITIO
#define LOCI_  (s32)FLUXUS_EVENTUM_DEFINITIO_LOCI

/* ==================================================
 * Probatio seriei eventorum MEMBRORUM (01KYMYW75S): solum genera
 * membrorum, ordine blocorum - complementum _seriem_probare (quae
 * ea praetermittit)
 * ================================================== */

nomen structura {
    constans character* basis;      /* titulus basis */
    constans character* membrum;    /* titulus membri */
    s32 genus;
} MembrumExspectatum;

interior vacuum
_seriem_membrorum_probare (FluxusDatorum* datorum,
    constans MembrumExspectatum* exspectata, i32 numerus)
{
    i32 visa = ZEPHYRUM;
    i32 b;
    i32 numerus_blocorum = xar_numerus(datorum->bloci);

    per (b = ZEPHYRUM; b < numerus_blocorum; b++)
    {
        FluxusDatorumBlocus* blocus = (FluxusDatorumBlocus*)
            xar_obtinere(datorum->bloci, b);
        i32 e;
        i32 m = xar_numerus(blocus->eventa);

        per (e = ZEPHYRUM; e < m; e++)
        {
            FluxusEventum* ev = (FluxusEventum*)xar_obtinere(
                blocus->eventa, e);

            si (ev->genus != (s32)FLUXUS_EVENTUM_MEMBRUM_USUS
                && ev->genus
                    != (s32)FLUXUS_EVENTUM_MEMBRUM_DEFINITIO)
            {
                perge;
            }
            si (visa < numerus)
            {
                constans MembrumExspectatum* ex = &exspectata[visa];
                FluxusVariabilis* v = (FluxusVariabilis*)
                    xar_obtinere(datorum->variabiles,
                        (i32)ev->variabilis);

                CREDO_AEQUALIS_S32 (ev->genus, ex->genus);
                CREDO_NON_NIHIL (v);
                si (v != NIHIL)
                {
                    CREDO_VERUM (v->membrum_est);
                    CREDO_VERUM (_nomen_aequale(v->titulus,
                        ex->basis));
                    CREDO_VERUM (_nomen_aequale(v->titulus_membri,
                        ex->membrum));
                }
            }
            visa++;
        }
    }
    CREDO_AEQUALIS_I32 (visa, numerus);
}

/* ==================================================
 * Auxilia chunk B: lectio statuum may/must
 * ================================================== */

interior s32
_variabilis_titulo (FluxusDatorum* d, constans character* titulus)
{
    i32 i;
    i32 m = xar_numerus(d->variabiles);

    per (i = ZEPHYRUM; i < m; i++)
    {
        FluxusVariabilis* v = (FluxusVariabilis*)xar_obtinere(
            d->variabiles, i);

        si (_nomen_aequale(v->titulus, titulus) && !v->membrum_est)
        {
            redde (s32)i;
        }
    }
    redde -I;
}

interior b32
_bitum_lectum (constans i64* verba, s32 index)
{
    redde ((verba[index / LXIV] >> (i32)(index % LXIV)) & (i64)I)
        != (i64)ZEPHYRUM ? VERUM : FALSUM;
}

/* Status ad exitum functionis (introitus bloci exitus) */
interior vacuum
_status_exitus (FluxusDatorum* d, constans character* titulus,
    b32* may, b32* must)
{
    FluxusDatorumBlocus* db = (FluxusDatorumBlocus*)xar_obtinere(
        d->bloci, d->fluxus->exitus->index);
    s32 v = _variabilis_titulo(d, titulus);

    *may = FALSUM;
    *must = FALSUM;
    si (v < ZEPHYRUM)
    {
        redde;
    }
    *may = _bitum_lectum(db->may_introitus, v);
    *must = _bitum_lectum(db->must_introitus, v);
}

/* Stirps variabilis nominatae ad exitum functionis (introitus
 * bloci exitus) - punctum fixum tertium */
interior s32
_stirps_ad_exitum (FluxusDatorum* d, constans character* titulus)
{
    FluxusDatorumBlocus* db = (FluxusDatorumBlocus*)xar_obtinere(
        d->bloci, d->fluxus->exitus->index);
    s32 v = _variabilis_titulo(d, titulus);

    si (v < ZEPHYRUM)
    {
        redde -I;
    }
    redde db->stirpes_introitus[v];
}

/* Status ad introitum bloci usus primi variabilis nominatae */
interior vacuum
_status_ad_usum (FluxusDatorum* d, constans character* usus_titulus,
    constans character* quaesitum, b32* may, b32* must)
{
    s32 vu = _variabilis_titulo(d, usus_titulus);
    s32 vq = _variabilis_titulo(d, quaesitum);
    i32 b;
    i32 numerus_blocorum = xar_numerus(d->bloci);

    *may = FALSUM;
    *must = FALSUM;
    si (vu < ZEPHYRUM || vq < ZEPHYRUM)
    {
        redde;
    }
    per (b = ZEPHYRUM; b < numerus_blocorum; b++)
    {
        FluxusDatorumBlocus* db = (FluxusDatorumBlocus*)xar_obtinere(
            d->bloci, b);
        i32 e;
        i32 m = xar_numerus(db->eventa);

        per (e = ZEPHYRUM; e < m; e++)
        {
            FluxusEventum* ev = (FluxusEventum*)xar_obtinere(
                db->eventa, e);

            si (ev->genus == USUS_ && ev->variabilis == vu)
            {
                *may = _bitum_lectum(db->may_introitus, vq);
                *must = _bitum_lectum(db->must_introitus, vq);
                redde;
            }
        }
    }
}

s32 principale (vacuum)
{
    Piscina* piscina;
    b32 praeteritus;

    piscina = piscina_generare_dynamicum("probatio_fluxus_datorum",
        33554432);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);

    /* ordo fundamentalis: dexter ante definitionem sinistri */
    {
        FluxusDatorum* d = _extrahere(piscina,
            "int f(int a) { int x; x = a + 1; return x; }");
        EventumExspectatum s[] = {
            { "a", USUS_, FALSUM },
            { "x", DEF_,  FALSUM },
            { "x", USUS_, FALSUM }
        };

        CREDO_NON_NIHIL (d);
        _seriem_probare(d, s, III);
        CREDO_AEQUALIS_I32 (xar_numerus(d->variabiles), II);
        {
            FluxusVariabilis* v = (FluxusVariabilis*)xar_obtinere(
                d->variabiles, ZEPHYRUM);

            CREDO_VERUM (v->parametrum);   /* a */
        }
    }

    /* lectio in assignatione propria: usus ANTE def (s01e) */
    {
        FluxusDatorum* d = _extrahere(piscina,
            "int f(int a) { int x; x = x + 1; return x; }");
        EventumExspectatum s[] = {
            { "x", USUS_, FALSUM },
            { "x", DEF_,  FALSUM },
            { "x", USUS_, FALSUM }
        };

        CREDO_NON_NIHIL (d);
        _seriem_probare(d, s, III);
    }

    /* composita: usus sinistri primum */
    {
        FluxusDatorum* d = _extrahere(piscina,
            "int f(int a) { int x; x = 0; x += a; return x; }");
        EventumExspectatum s[] = {
            { "x", DEF_,  FALSUM },
            { "x", USUS_, FALSUM },
            { "a", USUS_, FALSUM },
            { "x", DEF_,  FALSUM },
            { "x", USUS_, FALSUM }
        };

        CREDO_NON_NIHIL (d);
        _seriem_probare(d, s, V);
    }

    /* incrementa: usus deinde def (post et prae) */
    {
        FluxusDatorum* d = _extrahere(piscina,
            "int f(void) { int x; x = 0; x++; ++x; return x; }");
        EventumExspectatum s[] = {
            { "x", DEF_,  FALSUM },
            { "x", USUS_, FALSUM },
            { "x", DEF_,  FALSUM },
            { "x", USUS_, FALSUM },
            { "x", DEF_,  FALSUM },
            { "x", USUS_, FALSUM }
        };

        CREDO_NON_NIHIL (d);
        _seriem_probare(d, s, VI);
    }

    /* declaratio initiata + initiatio propria (s02: int y = y) */
    {
        FluxusDatorum* d = _extrahere(piscina,
            "int f(int a) { int x = a; int y = y; return x + y; }");
        EventumExspectatum s[] = {
            { "a", USUS_, FALSUM },
            { "x", DEF_,  FALSUM },
            { "y", USUS_, VERUM },    /* in initiatore proprio! */
            { "y", DEF_,  FALSUM },
            { "x", USUS_, FALSUM },
            { "y", USUS_, FALSUM }
        };

        CREDO_NON_NIHIL (d);
        _seriem_probare(d, s, VI);
    }

    /* amper: def loci; peek(&x) constans = NULLUM eventum (s04f) */
    {
        FluxusDatorum* d = _extrahere(piscina,
            "void f(int a) { int x; int* p = &x; g(&x);"
            " peek(&x); }");
        EventumExspectatum s[] = {
            { "x", LOCI_, FALSUM },
            { "p", DEF_,  FALSUM },
            { "x", LOCI_, FALSUM }
        };

        CREDO_NON_NIHIL (d);
        _seriem_probare(d, s, III);
    }

    /* magnitudo praetermissa (s18) + conversio discardans (s01c);
     * (void)g(x) NON discardans - usus x manet */
    {
        FluxusDatorum* d = _extrahere(piscina,
            "void f(void) { int x; int y; y = (int)sizeof x;"
            " (void)x; (void)g(x); }");
        EventumExspectatum s[] = {
            { "y", DEF_,  FALSUM },
            { "x", USUS_, FALSUM }
        };

        CREDO_NON_NIHIL (d);
        _seriem_probare(d, s, II);
    }

    /* aggregata: scriptio membri/elementi = def totius (s19);
     * lectio membri/elementi invisibilis (s05); usus toti-valoris */
    {
        FluxusDatorum* d = _extrahere(piscina,
            "void f(int i) { struct S s; int arr[4]; int y;"
            " s.a = 1; y = s.a; arr[i] = 1; y = arr[i];"
            " y = s; }");
        EventumExspectatum s[] = {
            { "s",   DEF_,  FALSUM },
            { "y",   DEF_,  FALSUM },
            { "i",   USUS_, FALSUM },
            { "arr", DEF_,  FALSUM },
            { "i",   USUS_, FALSUM },
            { "y",   DEF_,  FALSUM },
            { "s",   USUS_, FALSUM },   /* usus toti-valoris */
            { "y",   DEF_,  FALSUM }
        };

        CREDO_NON_NIHIL (d);
        _seriem_probare(d, s, VIII);
    }

    /* monstratores scalares: *p et p[a] legunt p (s01f) */
    {
        FluxusDatorum* d = _extrahere(piscina,
            "void f(int a) { int* p; int y; *p = 1; y = p[a]; }");
        EventumExspectatum s[] = {
            { "p", USUS_, FALSUM },
            { "a", USUS_, FALSUM },
            { "p", USUS_, FALSUM },
            { "y", DEF_,  FALSUM }
        };

        CREDO_NON_NIHIL (d);
        _seriem_probare(d, s, IV);
    }

    /* folium AMBIGUUS inresolutum (canonicum NIHIL): def-omnia -
     * abstentio conservativa. sizeof(g2) = ambiguitas classica
     * typus-vs-expressio; sedes vera semantica canonicum tradet. */
    {
        FluxusDatorum* d = _extrahere(piscina,
            "void f(void) { int x; x = sizeof(g2); }");
        EventumExspectatum s[] = {
            { "*", DEF_,  FALSUM },
            { "x", DEF_,  FALSUM }
        };

        CREDO_NON_NIHIL (d);
        _seriem_probare(d, s, II);
    }

    /* fissio (chunk 0) + invariantum praetermissionis: operanda in
     * blocis segmentorum, sententia granulum def in iunctione */
    {
        FluxusDatorum* d = _extrahere(piscina,
            "int f(int a, int b) { int x; x = a && b; return x; }");
        EventumExspectatum s[] = {
            { "a", USUS_, FALSUM },
            { "b", USUS_, FALSUM },
            { "x", DEF_,  FALSUM },
            { "x", USUS_, FALSUM }
        };

        CREDO_NON_NIHIL (d);
        _seriem_probare(d, s, IV);
    }

    /* conditio granulum est (fons unicus post chunk 0) */
    {
        FluxusDatorum* d = _extrahere(piscina,
            "int f(int a) { if (a > 0) { return a; } return 0; }");
        EventumExspectatum s[] = {
            { "a", USUS_, FALSUM },
            { "a", USUS_, FALSUM }
        };

        CREDO_NON_NIHIL (d);
        _seriem_probare(d, s, II);
    }

    /* ternarius praetermissus integer - operanda granulata */
    {
        FluxusDatorum* d = _extrahere(piscina,
            "int f(int c) { int x; x = c ? c : 0; return x; }");
        EventumExspectatum s[] = {
            { "c", USUS_, FALSUM },
            { "c", USUS_, FALSUM },
            { "x", DEF_,  FALSUM },
            { "x", USUS_, FALSUM }
        };

        CREDO_NON_NIHIL (d);
        _seriem_probare(d, s, IV);
    }

    /* ========================================================
     * CHUNK B: punctum fixum may/must
     * ======================================================== */
    {
        FluxusDatorum* d;
        b32 may;
        b32 must;

        /* si sine alioquin: may sine must (forma s06a) */
        d = _extrahere(piscina,
            "int f(int a) { int x; if (a) { x = 1; } return x; }");
        CREDO_NON_NIHIL (d);
        _status_exitus(d, "x", &may, &must);
        CREDO_VERUM (may);
        CREDO_FALSUM (must);

        /* ambo rami definiunt: must (forma s06b) */
        d = _extrahere(piscina,
            "int f(int a) { int x; if (a) { x = 1; }"
            " else { x = 2; } return x; }");
        CREDO_NON_NIHIL (d);
        _status_exitus(d, "x", &may, &must);
        CREDO_VERUM (may);
        CREDO_VERUM (must);

        /* ansa pura: may sine must (forma s15a - margo retro) */
        d = _extrahere(piscina,
            "int f(int a) { int x; while (a) { x = 1; }"
            " return x; }");
        CREDO_NON_NIHIL (d);
        _status_exitus(d, "x", &may, &must);
        CREDO_VERUM (may);
        CREDO_FALSUM (must);

        /* dum+frange (forma s07b): ad caput x NEC in may -
         * praeconditio marginis culpabilis gradus C */
        d = _extrahere(piscina,
            "int f(int a) { int x; while (a) { x = 1; break; }"
            " return x; }");
        CREDO_NON_NIHIL (d);
        _status_ad_usum(d, "a", "x", &may, &must);
        CREDO_FALSUM (may);

        /* fac_dum: corpus semper currit - must (forma s07c) */
        d = _extrahere(piscina,
            "int f(int a) { int x; do { x = 1; } while (a);"
            " return x; }");
        CREDO_NON_NIHIL (d);
        _status_exitus(d, "x", &may, &must);
        CREDO_VERUM (must);

        /* plicatum si (0): contributio rami mortui INVISIBILIS
         * (forma s04d - structuralis, blocus numquam processus) */
        d = _extrahere(piscina,
            "int f(void) { int x; if (0) { fill(&x); }"
            " return x; }");
        CREDO_NON_NIHIL (d);
        _status_exitus(d, "x", &may, &must);
        CREDO_FALSUM (may);

        /* parametra initiata in introitu */
        d = _extrahere(piscina,
            "int f(int a) { return a; }");
        CREDO_NON_NIHIL (d);
        _status_exitus(d, "a", &may, &must);
        CREDO_VERUM (must);

        /* def-omnia tegit variabiles omnes (abstentio) */
        d = _extrahere(piscina,
            "int f(void) { int x; int y; x = sizeof(g2);"
            " return y; }");
        CREDO_NON_NIHIL (d);
        _status_exitus(d, "y", &may, &must);
        CREDO_VERUM (must);
    }

    /* ========================================================
     * MEMBRA UT PSEUDO-VARIABILES (01KYMYW75S): series eventorum
     * membrorum exacta - quod probationes supra praetermittunt,
     * hic asseritur (rete utrimque: pinnae vetustae membra non
     * vident, series membrorum lacunas non celat)
     * ======================================================== */
    {
        FluxusDatorum* d;

        /* scriptio directa: def totius ANTE eventum membri (ordo
         * dele-deinde-pone); lectio directa: usus membri */
        d = _extrahere(piscina,
            "int f(void) { struct S s; s.m = 1; return s.m; }");
        CREDO_NON_NIHIL (d);
        si (d != NIHIL)
        {
            constans MembrumExspectatum series[] = {
                { "s", "m", (s32)FLUXUS_EVENTUM_MEMBRUM_DEFINITIO },
                { "s", "m", (s32)FLUXUS_EVENTUM_MEMBRUM_USUS }
            };

            _seriem_membrorum_probare(d, series, II);
        }
        /* ordo intra granulum: DEF (totius) praecedit MEMBRUM_DEF */
        si (d != NIHIL)
        {
            FluxusDatorumBlocus* blocus = (FluxusDatorumBlocus*)
                xar_obtinere(d->bloci, ZEPHYRUM);
            i32 e;
            i32 m = xar_numerus(blocus->eventa);
            s32 index_def_totius = -I;
            s32 index_def_membri = -I;

            per (e = ZEPHYRUM; e < m; e++)
            {
                FluxusEventum* ev = (FluxusEventum*)xar_obtinere(
                    blocus->eventa, e);

                si (ev->genus == (s32)FLUXUS_EVENTUM_DEFINITIO
                    && index_def_totius < ZEPHYRUM)
                {
                    index_def_totius = (s32)e;
                }
                si (ev->genus
                    == (s32)FLUXUS_EVENTUM_MEMBRUM_DEFINITIO)
                {
                    index_def_membri = (s32)e;
                }
            }
            CREDO_VERUM (index_def_totius >= ZEPHYRUM);
            CREDO_VERUM (index_def_membri > index_def_totius);
        }

        /* ordo membri tabulae: membrum_est, basis recta, parametrum
         * basis haeret; inquisitio basium ordinem praeterit */
        {
            s32 vb = _variabilis_titulo(d, "s");
            i32 i;
            i32 m = xar_numerus(d->variabiles);
            FluxusVariabilis* vm = NIHIL;

            CREDO_VERUM (vb >= ZEPHYRUM);
            per (i = ZEPHYRUM; i < m; i++)
            {
                FluxusVariabilis* v = (FluxusVariabilis*)
                    xar_obtinere(d->variabiles, i);

                si (v->membrum_est)
                {
                    vm = v;
                }
            }
            CREDO_NON_NIHIL (vm);
            si (vm != NIHIL)
            {
                CREDO_AEQUALIS_S32 (vm->basis, vb);
                CREDO_FALSUM (vm->parametrum);
                CREDO_VERUM (_nomen_aequale(vm->titulus_membri,
                    "m"));
            }
        }

        /* sagitta: basis monstrator - NULLUM eventum membri;
         * scriptio per elementum (s.tab[i]=) - def totius solus,
         * NULLUM eventum membri (membrum ipsum non redefinitur);
         * compositum (s.m += 1) - usus deinde def */
        d = _extrahere(piscina,
            "int f(struct S* p) { return p->m; }");
        CREDO_NON_NIHIL (d);
        _seriem_membrorum_probare(d, NIHIL, ZEPHYRUM);

        d = _extrahere(piscina,
            "void f(int i) { struct S s; s.tab[i] = 1; }");
        CREDO_NON_NIHIL (d);
        _seriem_membrorum_probare(d, NIHIL, ZEPHYRUM);

        d = _extrahere(piscina,
            "void f(void) { struct S s; s.m += 1; }");
        CREDO_NON_NIHIL (d);
        {
            constans MembrumExspectatum series[] = {
                { "s", "m", (s32)FLUXUS_EVENTUM_MEMBRUM_USUS },
                { "s", "m", (s32)FLUXUS_EVENTUM_MEMBRUM_DEFINITIO }
            };

            _seriem_membrorum_probare(d, series, II);
        }

        /* profunditas 2: basis non folium - NULLUM eventum membri
         * externi; internum (s intra t absens ex resolutore -
         * fons t ignotus, nulla eventa omnino hic: t non in
         * tabula symbolorum probationis). Basis tracta s.m.n:
         * eventum membri INTERNI (s,"m") def - conservativum */
        d = _extrahere(piscina,
            "void f(void) { struct S s; s.m.n = 1; }");
        CREDO_NON_NIHIL (d);
        {
            constans MembrumExspectatum series[] = {
                { "s", "m", (s32)FLUXUS_EVENTUM_MEMBRUM_DEFINITIO }
            };

            _seriem_membrorum_probare(d, series, I);
        }

        /* effugium per membrum: &s.m - basis LOCI (mos pristinus),
         * NULLUM eventum membri (lectores effugium basis
         * consulunt) */
        d = _extrahere(piscina,
            "void f(void) { struct S s; g2(&s.m); }");
        CREDO_NON_NIHIL (d);
        si (d != NIHIL)
        {
            s32 vb;

            _seriem_membrorum_probare(d, NIHIL, ZEPHYRUM);
            vb = _variabilis_titulo(d, "s");
            CREDO_VERUM (vb >= ZEPHYRUM);
            si (vb >= ZEPHYRUM)
            {
                FluxusVariabilis* v = (FluxusVariabilis*)
                    xar_obtinere(d->variabiles, (i32)vb);

                CREDO_VERUM (v->effugit);
            }
        }
    }

    /* ==================================================
     * Punctum fixum stirpium (vestigatio generum - columna tertia,
     * arcus fluxus 01KYPZ3XPW)
     * ================================================== */
    {
        FluxusDatorum* d;

        /* def fert valorem classificatoris ad exitum; parametrum
         * numquam redefinitum stirpem NEUTRAM seminis servat */
        d = _extrahere_cum(piscina,
            "int f(int a) { int x; x = a; return x; }",
            _probatio_stirps, NIHIL);
        CREDO_NON_NIHIL (d);
        si (d != NIHIL)
        {
            CREDO_AEQUALIS_I32 ((i32)_stirps_ad_exitum(d, "x"),
                (i32)FLUXUS_STIRPS_PRIMA);
            CREDO_AEQUALIS_I32 ((i32)_stirps_ad_exitum(d, "a"),
                (i32)FLUXUS_STIRPS_NEUTRA);
        }

        /* iunctio dissentiens -> AMISSA */
        d = _extrahere_cum(piscina,
            "int f(int a, int b, int c) { int x;"
            " if (c) { x = a; } else { x = b; } return x; }",
            _probatio_stirps, NIHIL);
        CREDO_NON_NIHIL (d);
        si (d != NIHIL)
        {
            CREDO_AEQUALIS_I32 ((i32)_stirps_ad_exitum(d, "x"),
                (i32)FLUXUS_STIRPS_AMISSA);
        }

        /* iunctio consentiens -> stirps servata */
        d = _extrahere_cum(piscina,
            "int f(int a, int c) { int x;"
            " if (c) { x = a; } else { x = a; } return x; }",
            _probatio_stirps, NIHIL);
        CREDO_NON_NIHIL (d);
        si (d != NIHIL)
        {
            CREDO_AEQUALIS_I32 ((i32)_stirps_ad_exitum(d, "x"),
                (i32)FLUXUS_STIRPS_PRIMA);
        }

        /* redefinitio littera -> NEUTRA delet */
        d = _extrahere_cum(piscina,
            "int f(int a) { int x; x = a; x = 1; return x; }",
            _probatio_stirps, NIHIL);
        CREDO_NON_NIHIL (d);
        si (d != NIHIL)
        {
            CREDO_AEQUALIS_I32 ((i32)_stirps_ad_exitum(d, "x"),
                (i32)FLUXUS_STIRPS_NEUTRA);
        }

        /* stirps_valoris NIHIL -> definitiones NEUTRAE */
        d = _extrahere(piscina,
            "int f(int a) { int x; x = a; return x; }");
        CREDO_NON_NIHIL (d);
        si (d != NIHIL)
        {
            CREDO_AEQUALIS_I32 ((i32)_stirps_ad_exitum(d, "x"),
                (i32)FLUXUS_STIRPS_NEUTRA);
        }

        /* ansa sui-referens: classificator caecus delet (corpus
         * NEUTRA -> iunctio capitis AMISSA); ambitu-conscius echo
         * servat (v2 - fixpunctum cum statu currenti) */
        d = _extrahere_cum(piscina,
            "int f(int a, int c) { int x; x = a;"
            " while (c) { x = x - 1; } return x; }",
            _probatio_stirps, NIHIL);
        CREDO_NON_NIHIL (d);
        si (d != NIHIL)
        {
            CREDO_AEQUALIS_I32 ((i32)_stirps_ad_exitum(d, "x"),
                (i32)FLUXUS_STIRPS_AMISSA);
        }
        d = _extrahere_cum(piscina,
            "int f(int a, int c) { int x; x = a;"
            " while (c) { x = x - 1; } return x; }",
            _probatio_stirps, _probatio_stirps_ambitu);
        CREDO_NON_NIHIL (d);
        si (d != NIHIL)
        {
            CREDO_AEQUALIS_I32 ((i32)_stirps_ad_exitum(d, "x"),
                (i32)FLUXUS_STIRPS_PRIMA);
        }
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    redde I;
}
