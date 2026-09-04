/* md_census.c - Vide md_census.h. */

#include "md_census.h"
#include "md_arbor.h"
#include "md_lexicon.h"
#include "materia_nodus.h"
#include "materia_token.h"
#include "chorda_aedificator.h"
#include <string.h>

vacuum
md_census_nudum (
    MdCensus* c,
     Piscina* piscina)
{
    memset(c, ZEPHYRUM, magnitudo(*c));
    c->linguae = xar_creare(piscina, (i32)magnitudo(MdCensusLingua));
}

interior s32
_index (
    constans MateriaNodus* n,
                      i32  locus)
{
    si (   locus < n->numerus_locorum
        && n->loci[locus].genus == MATERIA_VALOR_INDEX)
    {
        redde n->loci[locus].datum.index;
    }
    redde (s32)-I;
}

interior constans MateriaToken*
_lexema (
    constans MateriaNodus* n,
                      i32  locus)
{
    si (   locus < n->numerus_locorum
        && n->loci[locus].genus == MATERIA_VALOR_TOKEN)
    {
        redde n->loci[locus].datum.token;
    }
    redde NIHIL;
}

interior vacuum
_linguam_addere (
     MdCensus* c,
       chorda  lingua)
{
    i32 n = xar_numerus(c->linguae);
    i32 i;
    i32 l = lingua.mensura < MD_CENSUS_LINGUA_MENSURA - I
            ? (i32)lingua.mensura : MD_CENSUS_LINGUA_MENSURA - I;
    MdCensusLingua* novus;

    per (i = ZEPHYRUM; i < n; i++)
    {
        MdCensusLingua* e = (MdCensusLingua*)xar_obtinere(c->linguae,
            i);

        si (   (i32)strlen(e->lingua)                     == l
            && memcmp(e->lingua, lingua.datum, (size_t)l) == ZEPHYRUM)
        {
            e->numerus++;
            redde;
        }
    }
    novus = (MdCensusLingua*)xar_addere(c->linguae);
    memcpy(novus->lingua, lingua.datum, (size_t)l);
    novus->lingua[l]  = '\0';
    novus->numerus    = I;
}

interior vacuum
_lexema_numerare (
                 MdCensus* c,
    constans MateriaToken* t)
{
    c->lexemata++;
    si (t->fons_index == MD_FONS_DERIVATUS)
    {
        c->derivata++;
    }
    si (t->genus == (s32)MD_LEX_LINEA)
    {
        c->lineae++;
    }
}

interior vacuum
_nodum_numerare (
                 MdCensus* c,
    constans MateriaNodus* n)
{
    i32 i;

    si (n == NIHIL)
    {
        redde;
    }
    si (n->genus >= ZEPHYRUM && n->genus < (s32)MD_GENUS_NUMERUS)
    {
        c->genera[n->genus]++;
    }
    commutatio (n->genus)
    {
        casus MD_GENUS_CAPITULUM:
        {
            s32 g = _index(n, (i32)MD_CAPITULUM_GRADUS);

            si (g >= I && g <= VI)
            {
                c->capitula_gradus[g]++;
            }
            frange;
        }
        casus MD_GENUS_LISTA:
            si (_index(n, (i32)MD_LISTA_GENUS)
                == (s32)MD_LISTA_NUMERATA)
            {
                c->listae_numeratae++;
            }
            alioquin
            {
                c->listae_punctatae++;
            }
            si (_index(n, (i32)MD_LISTA_LAXA) == I)
            {
                c->listae_laxae++;
            }
            frange;
        casus MD_GENUS_ELEMENTUM:
        {
            s32 o = _index(n, (i32)MD_ELEMENTUM_OFFICIUM);

            si (o == (s32)MD_OFFICIUM_APERTUM)
            {
                c->officia_aperta++;
            }
            alioquin si (o == (s32)MD_OFFICIUM_PERFECTUM)
            {
                c->officia_perfecta++;
            }
            frange;
        }
        casus MD_GENUS_SAEPTUM:
        {
            constans MateriaToken* lingua = _lexema(n,
                (i32)MD_SAEPTUM_LINGUA);

            si (_index(n, (i32)MD_SAEPTUM_FORMA)
                == (s32)MD_SAEPTUM_INDENTATUS)
            {
                c->saepta_indentata++;
            }
            alioquin
            {
                c->saepta_saepta++;
            }
            si (lingua != NIHIL && lingua->valor.mensura > ZEPHYRUM)
            {
                _linguam_addere(c, lingua->valor);
            }
            frange;
        }
        casus MD_GENUS_NEXUS:
        {
            s32 f = _index(n, (i32)MD_NEXUS_FORMA);

            si (f == (s32)MD_NEXUS_REFERENS)
            {
                c->nexus_referentes++;
            }
            alioquin si (f == (s32)MD_NEXUS_AUTO)
            {
                c->nexus_auto++;
            }
            alioquin
            {
                c->nexus_inlinei++;
            }
            frange;
        }
        ordinarius:
            frange;
    }
    per (i = ZEPHYRUM; i < n->numerus_locorum; i++)
    {
        constans MateriaValor* v = &n->loci[i];

        si (v->genus == MATERIA_VALOR_NODUS)
        {
            _nodum_numerare(c, v->datum.nodus);
        }
        alioquin si (v->genus == MATERIA_VALOR_TOKEN)
        {
            _lexema_numerare(c, v->datum.token);
        }
        alioquin si (v->genus == MATERIA_VALOR_LISTA)
        {
            i32 m = materia_valor_lista_numerus(*v);
            i32 j;

            per (j = ZEPHYRUM; j < m; j++)
            {
                constans MateriaValor* e =
                    materia_valor_lista_obtinere(*v, j);

                si (e == NIHIL)
                {
                    perge;
                }
                si (e->genus == MATERIA_VALOR_NODUS)
                {
                    _nodum_numerare(c, e->datum.nodus);
                }
                alioquin si (e->genus == MATERIA_VALOR_TOKEN)
                {
                    _lexema_numerare(c, e->datum.token);
                }
            }
        }
    }
}

b32
md_census_addere (
              MdCensus* c,
               Piscina* piscina,
    constans character* fons,
                   i32  mensura)
{
    MateriaNodus* radix = md_arbor_parsare(piscina, fons, mensura);

    c->plagulae++;
    c->octeti = c->octeti + (s64)mensura;
    si (radix == NIHIL)
    {
        c->fractae++;
        redde FALSUM;
    }
    _nodum_numerare(c, radix);
    redde VERUM;
}

interior vacuum
_citata_colligere (
                      Xar* acervus,
                  Piscina* piscina,
    constans MateriaNodus* n)
{
    i32 i;

    si (n == NIHIL)
    {
        redde;
    }
    si (n->genus == (s32)MD_GENUS_VERBATIM)
    {
        constans MateriaToken* valor = _lexema(n,
            (i32)MD_VERBATIM_VALOR);
        constans MateriaValor* cruda  = &n->loci[MD_VERBATIM_CRUDUM];
                    MdCitatum* x      = (MdCitatum*)xar_addere(acervus);

        x->linea              = ZEPHYRUM;
        x->contentum.datum    = NIHIL;
        x->contentum.mensura  = ZEPHYRUM;
        si (   cruda->genus == MATERIA_VALOR_LISTA
            && materia_valor_lista_numerus(*cruda) > ZEPHYRUM)
        {
            constans MateriaValor* primum =
                materia_valor_lista_obtinere(
                *cruda, ZEPHYRUM);

            si (primum != NIHIL && primum->genus == MATERIA_VALOR_TOKEN)
            {
                x->linea = primum->datum.token->linea;
            }
        }
        si (valor != NIHIL)
        {
            x->contentum = valor->valor;
        }
        alioquin si (cruda->genus == MATERIA_VALOR_LISTA)
        {
            ChordaAedificator* aed = chorda_aedificator_creare(piscina,
                LXIV);
                          i32 m =
                              materia_valor_lista_numerus(*cruda);
                          i32 j;

            per (j = ZEPHYRUM; j < m; j++)
            {
                constans MateriaValor* e =
                    materia_valor_lista_obtinere(*cruda, j);

                si (e != NIHIL && e->genus == MATERIA_VALOR_TOKEN)
                {
                    chorda_aedificator_appendere_chorda(aed,
                        e->datum.token->valor);
                }
            }
            x->contentum = chorda_aedificator_finire(aed);
        }
        redde;
    }
    per (i = ZEPHYRUM; i < n->numerus_locorum; i++)
    {
        constans MateriaValor* v = &n->loci[i];

        si (v->genus == MATERIA_VALOR_NODUS)
        {
            _citata_colligere(acervus, piscina, v->datum.nodus);
        }
        alioquin si (v->genus == MATERIA_VALOR_LISTA)
        {
            i32 m = materia_valor_lista_numerus(*v);
            i32 j;

            per (j = ZEPHYRUM; j < m; j++)
            {
                constans MateriaValor* e =
                    materia_valor_lista_obtinere(*v, j);

                si (e != NIHIL && e->genus == MATERIA_VALOR_NODUS)
                {
                    _citata_colligere(acervus, piscina, e->datum.nodus);
                }
            }
        }
    }
}

Xar*
md_census_citata (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura)
{
    MateriaNodus* radix = md_arbor_parsare(piscina, fons, mensura);
             Xar* acervus;

    si (radix == NIHIL)
    {
        redde NIHIL;
    }
    acervus = xar_creare(piscina, (i32)magnitudo(MdCitatum));
    _citata_colligere(acervus, piscina, radix);
    redde acervus;
}
