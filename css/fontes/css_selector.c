/* css_selector.c - Arbor selectorum (spec par. V, B9)
 *
 * Vide caput css_selector.h: arbor ANALYSEOS super lexemata
 * praeludii - lexemata communicantur, nodi numquam (folia servata
 * NOVA super lexemata eadem, ne pater arboris plagulae laedatur).
 *
 * GRAMMATICA (Selectors L4, species-level):
 *   lista      := selector (',' selector)*        commata suprema
 *   selector   := [compositus] (combinator compositus)*
 *   combinator := '>' | '+' | '~' | SPATIA (descendens)
 *   compositus := pars+
 *   pars       := IDENT | '*' | '.' IDENT | CANCELLUM
 *              | ':' IDENT | ':' FUNCTIO(...) | '::' IDENT
 *              | '[' IDENT (op valor)? flag? ']'
 * Nidificatio SINISTRORSUM: 'a + b ~ c' = ((a + b) ~ c).
 * Selector relativus ('> b'): sinister ABSENS. Combinator caudalis
 * ('a >'): dexter ABSENS. Combinatores consecutivi: complexus cum
 * dextro absente, deinde combinator novus pendet.
 *
 * ARGUMENTA pseudo-classium: functiones selectorum (not/is/where/
 * has/host/host-context/slotted) argumenta ut SELECTORES parsant
 * (recursio per introitum publicum); ceterae (nth-*, lang, dir)
 * argumenta CRUDA ferunt - folia servata nova super lexemata
 * argumentorum, ordine octetorum.
 *
 * TOTALIS: quod parsari nequit 'selector-malus' fit (lexemata
 * partis malae, ordine). NIHIL solum si memoria deficit.
 */

#include "css_selector.h"
#include "css_lexema.h"
#include "css_registrum.h"
#include "materia_arbor.h"
#include "materia_nodus.h"
#include "materia_token.h"
#include "xar.h"
#include <string.h>


nomen structura {
         Piscina* piscina;
    MateriaValor  elementa;    /* lista praeludii */
             i32  numerus;
} CssSelectio;

/* Pseudo-classes functionales quarum argumenta SELECTORES sunt.
 * Nomen lexematis FUNCTIO parenthesim fert ('not('). */
hic_manens constans character* FUNCTIONES_SELECTORUM[] = {
    "not(", "is(", "where(", "has(", "host(", "host-context(",
    "slotted("
};


/* ==================================================
 * Inspectio elementorum praeludii
 * ================================================== */

interior MateriaNodus*
_elementum (
    constans CssSelectio* s,
                     i32  i)
{
    constans MateriaValor* v;

    si (i < ZEPHYRUM || i >= s->numerus)
    {
        redde NIHIL;
    }
    v = materia_valor_lista_obtinere(s->elementa, i);
    si (v == NIHIL || v->genus != MATERIA_VALOR_NODUS)
    {
        redde NIHIL;
    }
    redde v->datum.nodus;
}

/* Lexema folii servati; NIHIL si nodus aliud est */
interior MateriaToken*
_folium (
    constans MateriaNodus* n)
{
    si (   n                               == NIHIL
        || n->genus != (s32)CSS_GENUS_LEXEMA_SERVATUM
        || n->loci[CSS_SERVATUM_TOK].genus != MATERIA_VALOR_TOKEN)
    {
        redde NIHIL;
    }
    redde n->loci[CSS_SERVATUM_TOK].datum.token;
}

interior b32
_est_lexema (
    constans MateriaNodus* n,
                      s32  genus)
{
    MateriaToken* t = _folium(n);

    redde (b32)(t != NIHIL && t->genus == genus);
}

interior b32
_est_delim (
    constans MateriaNodus* n,
                character  c)
{
    MateriaToken* t = _folium(n);

    redde (b32)(   t != NIHIL
                && t->genus == (s32)CSS_LEX_DELIM
                && t->valor.mensura == I
                && t->valor.datum[ZEPHYRUM] == c);
}

interior b32
_est_spatia (
    constans MateriaNodus* n)
{
    redde _est_lexema(n, (s32)CSS_LEX_SPATIA);
}

interior b32
_est_combinator_punctus (
    constans MateriaNodus* n)
{
    redde (b32)(_est_delim(n, '>') || _est_delim(n, '+')
             || _est_delim(n, '~'));
}

/* '~' '|' '^' '$' '*' - praefixum operatoris attributi ante '=' */
interior b32
_est_operator_praefixus (
    constans MateriaNodus* n)
{
    redde (b32)(_est_delim(n, '~') || _est_delim(n, '|')
             || _est_delim(n, '^') || _est_delim(n, '$')
             || _est_delim(n, '*'));
}

interior b32
_est_valor_attributi (
    constans MateriaNodus* n)
{
    redde (b32)(_est_lexema(n, (s32)CSS_LEX_IDENTIFICATOR)
             || _est_lexema(n, (s32)CSS_LEX_CHORDA));
}

interior b32
_est_functio (
    constans MateriaNodus* n)
{
    redde (b32)(n != NIHIL && n->genus == (s32)CSS_GENUS_FUNCTIO);
}

/* saeptum '[' - attributum */
interior b32
_est_saeptum_attributi (
    constans MateriaNodus* n)
{
    constans MateriaValor* apertum;

    si (n == NIHIL || n->genus != (s32)CSS_GENUS_SAEPTUM)
    {
        redde FALSUM;
    }
    apertum = &n->loci[CSS_SAEPTUM_TOK_APERTUM];
    redde (b32)(   apertum->genus == MATERIA_VALOR_TOKEN
                && apertum->datum.token->genus
                   == (s32)CSS_LEX_QUADRA_APERTA);
}

/* ASCII sine casu, longitudine aequali */
interior b32
_sine_casu_aequalis (
                chorda  valor,
    constans character* exemplar)
{
    i32 mensura = (i32)strlen(exemplar);
    i32 i;

    si (valor.mensura != mensura)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < mensura; i++)
    {
        character c = (character)valor.datum[i];

        si (c >= 'A' && c <= 'Z')
        {
            c = (character)(c + XXXII);
        }
        si (c != exemplar[i])
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior b32
_functio_selectorum (
    constans MateriaToken* titulus)
{
    i32 numerus = (i32)(magnitudo(FUNCTIONES_SELECTORUM)
                      / magnitudo(FUNCTIONES_SELECTORUM[0]));
    i32 i;

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        si (_sine_casu_aequalis(titulus->valor,
                                FUNCTIONES_SELECTORUM[i]))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}


/* ==================================================
 * Constructores
 * ================================================== */

/* Lexemata subarboris ordine locorum (= ordine octetorum) */
interior MateriaValor
_lexemata_colligere (
                  Piscina* piscina,
             MateriaValor  lista,
    constans MateriaNodus* n)
{
    i32 i;

    si (n == NIHIL)
    {
        redde lista;
    }
    per (i = ZEPHYRUM; i < n->numerus_locorum; i++)
    {
        constans MateriaValor* v = &n->loci[i];

        si (v->genus == MATERIA_VALOR_TOKEN)
        {
            lista = materia_valor_lista_appendere(piscina, lista,
                materia_valor_token(v->datum.token));
        }
        alioquin si (v->genus == MATERIA_VALOR_NODUS)
        {
            lista = _lexemata_colligere(piscina, lista, v->datum.nodus);
        }
        alioquin si (v->genus == MATERIA_VALOR_LISTA)
        {
            i32 numerus = materia_valor_lista_numerus(*v);
            i32 j;

            per (j = ZEPHYRUM; j < numerus; j++)
            {
                constans MateriaValor* e =
                    materia_valor_lista_obtinere(*v, j);

                si (e == NIHIL)
                {
                    perge;
                }
                si (e->genus == MATERIA_VALOR_TOKEN)
                {
                    lista = materia_valor_lista_appendere(piscina,
                        lista,
                        materia_valor_token(e->datum.token));
                }
                alioquin si (e->genus == MATERIA_VALOR_NODUS)
                {
                    lista = _lexemata_colligere(piscina, lista,
                        e->datum.nodus);
                }
            }
        }
    }
    redde lista;
}

interior MateriaNodus*
_malus_creare (
         Piscina* piscina,
    MateriaValor  lexemata)
{
    MateriaNodus* malus;

    malus = materia_nodus_creare(piscina, (s32)CSS_GENUS_SELECTOR_MALUS,
        (i32)I);
    si (malus == NIHIL)
    {
        redde NIHIL;
    }
    si (!materia_nodus_ponere(malus, (i32)CSS_MALUM_TOKENS, lexemata,
            MATERIA_LOCUS_LISTA_TOKEN))
    {
        redde NIHIL;
    }
    redde malus;
}

/* Malus ex elementis [a, b) praeludii */
interior MateriaNodus*
_malus_ex_elementis (
    constans CssSelectio* s,
                     i32  a,
                     i32  b)
{
    MateriaValor lexemata = materia_valor_lista_nova(s->piscina);
             i32 i;

    per (i = a; i < b; i++)
    {
        lexemata = _lexemata_colligere(s->piscina, lexemata,
            _elementum(s, i));
    }
    redde _malus_creare(s->piscina, lexemata);
}

/* Nodus unius loci TOKEN (typi, universalis, identitatis) */
interior MateriaNodus*
_folium_unum (
          Piscina* piscina,
              s32  genus,
     MateriaToken* token)
{
    MateriaNodus* n = materia_nodus_creare(piscina, genus, (i32)I);

    si (n == NIHIL)
    {
        redde NIHIL;
    }
    si (!materia_nodus_ponere(n, ZEPHYRUM, materia_valor_token(token),
            MATERIA_LOCUS_TOKEN))
    {
        redde NIHIL;
    }
    redde n;
}

/* Nodus duorum locorum TOKEN (classis, pseudo-elementum) */
interior MateriaNodus*
_folia_duo (
          Piscina* piscina,
              s32  genus,
     MateriaToken* primum,
     MateriaToken* alterum)
{
    MateriaNodus* n = materia_nodus_creare(piscina, genus, (i32)II);

    si (n == NIHIL)
    {
        redde NIHIL;
    }
    si (!materia_nodus_ponere(n, ZEPHYRUM, materia_valor_token(primum),
            MATERIA_LOCUS_TOKEN))
    {
        redde NIHIL;
    }
    si (!materia_nodus_ponere(n, (i32)I, materia_valor_token(alterum),
            MATERIA_LOCUS_TOKEN))
    {
        redde NIHIL;
    }
    redde n;
}

/* Folium servatum NOVUM super lexema idem (argumenta cruda) */
interior MateriaNodus*
_servatum_novum (
         Piscina* piscina,
    MateriaToken* token)
{
    redde _folium_unum(piscina, (s32)CSS_GENUS_LEXEMA_SERVATUM, token);
}

interior MateriaNodus*
_complexus_creare (
         Piscina* piscina,
    MateriaNodus* sinister,
    MateriaToken* combinator,
    MateriaNodus* dexter)
{
    MateriaNodus* n = materia_nodus_creare(piscina,
        (s32)CSS_GENUS_SELECTOR_COMPLEXUS, (i32)III);

    si (n == NIHIL)
    {
        redde NIHIL;
    }
    si (   sinister != NIHIL && !materia_nodus_ponere(n,
            (i32)CSS_COMPLEXUS_SINISTER, materia_valor_nodus(sinister),
            MATERIA_LOCUS_NODUS))
    {
        redde NIHIL;
    }
    si (   combinator != NIHIL && !materia_nodus_ponere(n,
            (i32)CSS_COMPLEXUS_COMBINATOR,
            materia_valor_token(combinator),
            MATERIA_LOCUS_TOKEN))
    {
        redde NIHIL;
    }
    si (   dexter != NIHIL && !materia_nodus_ponere(n,
            (i32)CSS_COMPLEXUS_DEXTER, materia_valor_nodus(dexter),
            MATERIA_LOCUS_NODUS))
    {
        redde NIHIL;
    }
    redde n;
}


/* ==================================================
 * Partes compositi
 * ================================================== */

/* '[' nomen (op valor)? flag? ']' - aliter malus ex saepto toto */
interior MateriaNodus*
_attributum_parsare (
    constans CssSelectio* s,
            MateriaNodus* saeptum)
{
    MateriaValor contentum = saeptum->loci[CSS_SAEPTUM_CONTENTUM];
    MateriaNodus* plena[VIII];
    i32 numerus_plenorum = ZEPHYRUM;
    i32 numerus;
    i32 i;
    MateriaToken* titulus   = NIHIL;
    MateriaToken* operator_ = NIHIL;
    MateriaToken* valor     = NIHIL;
    i32 k;
    b32 sanum;
    MateriaNodus* n;
    MateriaValor apertum;
    MateriaValor clausum;

    apertum = saeptum->loci[CSS_SAEPTUM_TOK_APERTUM];
    clausum = saeptum->loci[CSS_SAEPTUM_TOK_CLAUSUM];
    numerus = (contentum.genus == MATERIA_VALOR_LISTA)
            ? materia_valor_lista_numerus(contentum) : ZEPHYRUM;
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        constans MateriaValor* v =
            materia_valor_lista_obtinere(contentum, i);

        si (   v == NIHIL || v->genus != MATERIA_VALOR_NODUS
            || _est_spatia(v->datum.nodus))
        {
            perge;
        }
        si (numerus_plenorum >= (i32)VIII)
        {
            numerus_plenorum = numerus_plenorum + I;   /* nimis multa */
            frange;
        }
        plena[numerus_plenorum]  = v->datum.nodus;
        numerus_plenorum         = numerus_plenorum + I;
    }

    /* forma: IDENT [ ('=' | X '=') (IDENT|CHORDA) [IDENT] ] */
    sanum  = FALSUM;
    k      = ZEPHYRUM;
    si (   numerus_plenorum >= I && numerus_plenorum <= (i32)VIII
        && _est_lexema(plena[ZEPHYRUM], (s32)CSS_LEX_IDENTIFICATOR))
    {
        titulus  = _folium(plena[ZEPHYRUM]);
        k        = I;
        sanum    = VERUM;
        si (k < numerus_plenorum)
        {
            si (_est_delim(plena[k], '='))
            {
                operator_  = _folium(plena[k]);
                k          = k + I;
            }
            alioquin si (   k + I < numerus_plenorum
                         && _est_operator_praefixus(plena[k])
                         && _est_delim(plena[k + I], '='))
            {
                operator_  = _folium(plena[k]);
                k          = k + II;
            }
            alioquin
            {
                sanum = FALSUM;
            }
            si (sanum)
            {
                si (   k < numerus_plenorum
                    && _est_valor_attributi(plena[k]))
                {
                    valor  = _folium(plena[k]);
                    k      = k + I;
                }
                alioquin
                {
                    sanum = FALSUM;
                }
            }
            /* vexillum casus ('i' | 's') - non refertur */
            si (   sanum && k < numerus_plenorum
                && _est_lexema(plena[k], (s32)CSS_LEX_IDENTIFICATOR))
            {
                k = k + I;
            }
            si (k != numerus_plenorum)
            {
                sanum = FALSUM;
            }
        }
    }
    si (!sanum)
    {
        redde _malus_creare(s->piscina, _lexemata_colligere(s->piscina,
            materia_valor_lista_nova(s->piscina), saeptum));
    }

    n = materia_nodus_creare(s->piscina,
        (s32)CSS_GENUS_SELECTOR_ATTRIBUTI, (i32)V);
    si (n == NIHIL)
    {
        redde NIHIL;
    }
    si (   apertum.genus == MATERIA_VALOR_TOKEN
        && !materia_nodus_ponere(n, (i32)CSS_ATTRIBUTI_TOK_APERTUM,
                apertum, MATERIA_LOCUS_TOKEN))
    {
        redde NIHIL;
    }
    si (!materia_nodus_ponere(n, (i32)CSS_ATTRIBUTI_TOK_NOMEN,
            materia_valor_token(titulus), MATERIA_LOCUS_TOKEN))
    {
        redde NIHIL;
    }
    si (   operator_ != NIHIL && !materia_nodus_ponere(n,
            (i32)CSS_ATTRIBUTI_TOK_OPERATOR,
            materia_valor_token(operator_),
            MATERIA_LOCUS_TOKEN))
    {
        redde NIHIL;
    }
    si (   valor != NIHIL
        && !materia_nodus_ponere(n, (i32)CSS_ATTRIBUTI_VALOR,
            materia_valor_token(valor), MATERIA_LOCUS_TOKEN))
    {
        redde NIHIL;
    }
    si (   clausum.genus == MATERIA_VALOR_TOKEN
        && !materia_nodus_ponere(n, (i32)CSS_ATTRIBUTI_TOK_CLAUSUM,
                clausum, MATERIA_LOCUS_TOKEN))
    {
        redde NIHIL;
    }
    redde n;
}

/* Argumenta pseudo-classis functionalis in nodum pseudo appendere */
interior b32
_argumenta_parsare (
    constans CssSelectio* s,
            MateriaNodus* pseudo,
            MateriaNodus* functio)
{
    MateriaValor  argumenta = functio->loci[CSS_FUNCTIO_ARGUMENTA];
    MateriaToken* titulus =
        functio->loci[CSS_FUNCTIO_TOK_NOMEN].datum.token;

    si (_functio_selectorum(titulus))
    {
        MateriaNodus* sub = css_selector_parsare(s->piscina, argumenta);

        si (sub == NIHIL)
        {
            redde FALSUM;
        }
        redde materia_nodus_appendere(s->piscina, pseudo,
            (i32)CSS_PSEUDO_CLASSIS_ARGUMENTA, materia_valor_nodus(sub),
            MATERIA_LOCUS_LISTA_NODUS);
    }
    /* cruda: folia nova super lexemata argumentorum, ordine */
    {
        MateriaValor lexemata = materia_valor_lista_nova(s->piscina);
                 i32 numerus;
                 i32 i;

        si (argumenta.genus == MATERIA_VALOR_LISTA)
        {
            numerus = materia_valor_lista_numerus(argumenta);
            per (i = ZEPHYRUM; i < numerus; i++)
            {
                constans MateriaValor* e = materia_valor_lista_obtinere(
                    argumenta, i);

                si (e != NIHIL && e->genus == MATERIA_VALOR_NODUS)
                {
                    lexemata = _lexemata_colligere(s->piscina, lexemata,
                        e->datum.nodus);
                }
            }
        }
        numerus = materia_valor_lista_numerus(lexemata);
        per (i = ZEPHYRUM; i < numerus; i++)
        {
            MateriaNodus* folium = _servatum_novum(s->piscina,
                materia_valor_lista_obtinere(lexemata, i)->datum.token);

            si (   folium == NIHIL
                || !materia_nodus_appendere(s->piscina, pseudo,
                        (i32)CSS_PSEUDO_CLASSIS_ARGUMENTA,
                        materia_valor_nodus(folium),
                        MATERIA_LOCUS_LISTA_NODUS))
            {
                redde FALSUM;
            }
        }
    }
    redde VERUM;
}

/* Pseudo-classis: colon + nomen (IDENT aut FUNCTIO); argumenta
 * postea appenduntur */
interior MateriaNodus*
_pseudo_classem_creare (
         Piscina* piscina,
    MateriaToken* colon,
    MateriaValor  titulus)
{
    MateriaNodus* n = materia_nodus_creare(piscina,
        (s32)CSS_GENUS_PSEUDO_CLASSIS, (i32)III);

    si (n == NIHIL)
    {
        redde NIHIL;
    }
    si (   !materia_nodus_ponere(n, (i32)CSS_PSEUDO_CLASSIS_TOK_COLON,
                materia_valor_token(colon), MATERIA_LOCUS_TOKEN)
        || !materia_nodus_ponere(n, (i32)CSS_PSEUDO_CLASSIS_TOK_NOMEN,
                titulus, MATERIA_LOCUS_TOKEN))
    {
        redde NIHIL;
    }
    redde n;
}

/* Partem unam consumere, compositu appendere; indicem proximum
 * reddere (semper > i). Fractura memoriae: -I - ergo SIGNATUM
 * redditur (i32 domus insignatus est; examen sentinellam negativam
 * in functione insignata cepit). */
interior s32
_partem_consumere (
    constans CssSelectio* s,
            MateriaNodus* compositus,
                     i32  i,
                     i32  b)
{
    MateriaNodus* n         = _elementum(s, i);
    MateriaToken* tok       = _folium(n);
    MateriaNodus* pars      = NIHIL;
             i32  proximus  = i + I;

    si (tok != NIHIL)
    {
        commutatio (tok->genus)
        {
            casus (s32)CSS_LEX_IDENTIFICATOR:
                pars = _folium_unum(s->piscina,
                    (s32)CSS_GENUS_SELECTOR_TYPI, tok);
                frange;
            casus (s32)CSS_LEX_CANCELLUM:
                pars = _folium_unum(s->piscina,
                    (s32)CSS_GENUS_SELECTOR_IDENTITATIS, tok);
                frange;
            casus (s32)CSS_LEX_DELIM:
                si (_est_delim(n, '*'))
                {
                    pars = _folium_unum(s->piscina,
                        (s32)CSS_GENUS_SELECTOR_UNIVERSALIS, tok);
                }
                alioquin si (   _est_delim(n, '.') && i + I < b
                             && _est_lexema(_elementum(s, i + I),
                                   (s32)CSS_LEX_IDENTIFICATOR))
                {
                    pars = _folia_duo(s->piscina,
                        (s32)CSS_GENUS_SELECTOR_CLASSIS, tok,
                        _folium(_elementum(s, i + I)));
                    proximus = i + II;
                }
                alioquin
                {
                    pars = _malus_ex_elementis(s, i, i + I);
                }
                frange;
            casus (s32)CSS_LEX_COLON:
                si (   i + II < b
                    && _est_lexema(_elementum(s, i + I),
                                   (s32)CSS_LEX_COLON)
                    && _est_lexema(_elementum(s, i + II),
                                   (s32)CSS_LEX_IDENTIFICATOR))
                {
                    pars = _folia_duo(s->piscina,
                        (s32)CSS_GENUS_PSEUDO_ELEMENTUM, tok,
                        _folium(_elementum(s, i + II)));
                    proximus = i + III;
                }
                alioquin si (   i + I < b
                             && _est_lexema(_elementum(s, i + I),
                                            (s32)CSS_LEX_IDENTIFICATOR))
                {
                    MateriaToken* titulus =
                        _folium(_elementum(s, i + I));

                    pars = _pseudo_classem_creare(s->piscina, tok,
                        materia_valor_token(titulus));
                    proximus = i + II;
                }
                alioquin si (   i + I < b
                             && _est_functio(_elementum(s, i + I)))
                {
                    MateriaNodus* functio = _elementum(s, i + I);

                    pars = _pseudo_classem_creare(s->piscina, tok,
                        functio->loci[CSS_FUNCTIO_TOK_NOMEN]);
                    si (   pars != NIHIL
                        && !_argumenta_parsare(s, pars, functio))
                    {
                        pars = NIHIL;
                    }
                    proximus = i + II;
                }
                alioquin
                {
                    pars = _malus_ex_elementis(s, i, i + I);
                }
                frange;
            ordinarius:
                pars = _malus_ex_elementis(s, i, i + I);
                frange;
        }
    }
    alioquin si (_est_saeptum_attributi(n))
    {
        pars = _attributum_parsare(s, n);
    }
    alioquin
    {
        pars = _malus_ex_elementis(s, i, i + I);
    }

    si (   pars == NIHIL
        || !materia_nodus_appendere(s->piscina, compositus,
            (i32)CSS_COMPOSITUS_PARTES, materia_valor_nodus(pars),
            MATERIA_LOCUS_LISTA_NODUS))
    {
        redde (s32)-I;
    }
    redde (s32)proximus;
}


/* ==================================================
 * Selector unus: compositi combinatoribus iuncti
 * ================================================== */

nomen structura {
    MateriaNodus* arbor;        /* accumulatum, sinistrorsum */
    MateriaNodus* compositus;   /* in constructione */
    MateriaToken* pendens;      /* combinator sine dextro adhuc */
} CssSelectorStatus;

/* Compositum currentem in arborem claudere */
interior b32
_compositum_claudere (
    constans CssSelectio* s,
       CssSelectorStatus* st)
{
    si (st->compositus == NIHIL)
    {
        redde VERUM;
    }
    si (st->pendens != NIHIL || st->arbor != NIHIL)
    {
        st->arbor = _complexus_creare(s->piscina, st->arbor,
            st->pendens, st->compositus);
        st->pendens = NIHIL;
    }
    alioquin
    {
        st->arbor = st->compositus;
    }
    st->compositus = NIHIL;
    redde (b32)(st->arbor != NIHIL);
}

interior b32
_combinator_applicare (
    constans CssSelectio* s,
       CssSelectorStatus* st,
            MateriaToken* combinator)
{
    si (!_compositum_claudere(s, st))
    {
        redde FALSUM;
    }
    si (st->pendens != NIHIL)
    {
        /* consecutivi: dexter absens */
        st->arbor = _complexus_creare(s->piscina, st->arbor,
            st->pendens, NIHIL);
        si (st->arbor == NIHIL)
        {
            redde FALSUM;
        }
    }
    st->pendens = combinator;
    redde VERUM;
}

interior MateriaNodus*
_selectorem_parsare (
    constans CssSelectio* s,
                     i32  a,
                     i32  b)
{
    CssSelectorStatus st;
                  i32 i;
                  s32 proximus;

    dum (a < b && _est_spatia(_elementum(s, a)))
    {
        a = a + I;
    }
    dum (b > a && _est_spatia(_elementum(s, b - I)))
    {
        b = b - I;
    }
    si (a >= b)
    {
        redde _malus_creare(s->piscina,
            materia_valor_lista_nova(s->piscina));
    }

    st.arbor       = NIHIL;
    st.compositus  = NIHIL;
    st.pendens     = NIHIL;
    i              = a;
    dum (i < b)
    {
        MateriaNodus* n = _elementum(s, i);

        si (_est_spatia(n))
        {
            i32 j = i;

            dum (j < b && _est_spatia(_elementum(s, j)))
            {
                j = j + I;
            }
            si (j >= b)
            {
                frange;
            }
            si (!_est_combinator_punctus(_elementum(s, j)))
            {
                /* descendens: spatium IPSUM combinator */
                si (!_combinator_applicare(s, &st, _folium(n)))
                {
                    redde NIHIL;
                }
            }
            i = j;
            perge;
        }
        si (_est_combinator_punctus(n))
        {
            si (!_combinator_applicare(s, &st, _folium(n)))
            {
                redde NIHIL;
            }
            i = i + I;
            dum (i < b && _est_spatia(_elementum(s, i)))
            {
                i = i + I;
            }
            perge;
        }
        si (st.compositus == NIHIL)
        {
            st.compositus = materia_nodus_creare(s->piscina,
                (s32)CSS_GENUS_SELECTOR_COMPOSITUS, (i32)I);
            si (st.compositus == NIHIL)
            {
                redde NIHIL;
            }
        }
        proximus = _partem_consumere(s, st.compositus, i, b);
        si (proximus < ZEPHYRUM)
        {
            redde NIHIL;
        }
        i = (i32)proximus;
    }

    si (!_compositum_claudere(s, &st))
    {
        redde NIHIL;
    }
    si (st.pendens != NIHIL)
    {
        /* caudalis: dexter absens */
        st.arbor = _complexus_creare(s->piscina, st.arbor, st.pendens,
            NIHIL);
    }
    si (st.arbor == NIHIL)
    {
        redde _malus_creare(s->piscina,
            materia_valor_lista_nova(s->piscina));
    }
    redde st.arbor;
}


/* ==================================================
 * Introitus
 * ================================================== */

MateriaNodus*
css_selector_parsare (
         Piscina* piscina,
    MateriaValor  praeludium)
{
    CssSelectio s;
    MateriaNodus* radix;
    i32 commata = ZEPHYRUM;
    i32 i;

    s.piscina   = piscina;
    s.elementa  = praeludium;
    s.numerus  = (praeludium.genus == MATERIA_VALOR_LISTA)
               ? materia_valor_lista_numerus(praeludium) : ZEPHYRUM;

    per (i = ZEPHYRUM; i < s.numerus; i++)
    {
        si (_est_lexema(_elementum(&s, i), (s32)CSS_LEX_COMMA))
        {
            commata = commata + I;
        }
    }

    si (commata == ZEPHYRUM)
    {
        radix = _selectorem_parsare(&s, ZEPHYRUM, s.numerus);
    }
    alioquin
    {
        i32 initium = ZEPHYRUM;

        radix = materia_nodus_creare(piscina,
            (s32)CSS_GENUS_SELECTOR_LISTA, (i32)I);
        si (radix == NIHIL)
        {
            redde NIHIL;
        }
        per (i = ZEPHYRUM; i <= s.numerus; i++)
        {
            si (   i == s.numerus
                || _est_lexema(_elementum(&s, i), (s32)CSS_LEX_COMMA))
            {
                MateriaNodus* sub = _selectorem_parsare(&s, initium, i);

                si (   sub == NIHIL
                    || !materia_nodus_appendere(piscina, radix,
                            (i32)CSS_SEL_LISTA_SELECTORES,
                            materia_valor_nodus(sub),
                            MATERIA_LOCUS_LISTA_NODUS))
                {
                    redde NIHIL;
                }
                initium = i + I;
            }
        }
    }
    si (radix != NIHIL)
    {
        materia_arbor_patres_figere(piscina, radix);
    }
    redde radix;
}
