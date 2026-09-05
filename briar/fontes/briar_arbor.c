/* briar_arbor.c - Vide briar_arbor.h. */

#include "briar_arbor.h"
#include "briar_registrum.h"
#include "briar_lexicon.h"
#include "briar_lexema.h"
#include "materia_arbor.h"
#include "materia_token.h"
#include "xar.h"
#include <string.h>

nomen structura {
         Piscina* piscina;
     BriarLineae  lineae;
    BriarFabrica  fabrica;
    MateriaNodus* documentum;
             s32  prosa_initium;   /* offset prosae pendentis; -I = nulla */
} BriarParsura;

interior MateriaNodus*
_nodus (
    BriarParsura* p,
      BriarGenus  genus)
{
    redde materia_nodus_creare(p->piscina, (s32)genus,
        BRIAR_REGISTRUM.genera[genus].loci_numerus);
}

interior b32
_ponere_lexema (
    MateriaNodus* nodus,
             i32  locus,
    MateriaToken* lexema)
{
    si (lexema == NIHIL)
    {
        redde FALSUM;
    }
    redde materia_nodus_ponere(nodus, locus,
        materia_valor_token(lexema),
        MATERIA_LOCUS_TOKEN);
}

interior b32
_ponere_index (
    MateriaNodus* nodus,
             i32  locus,
             s32  valor)
{
    redde materia_nodus_ponere(nodus, locus, materia_valor_index(valor),
        MATERIA_LOCUS_INDEX);
}

interior b32
_partem_addere (
    BriarParsura* p,
    MateriaNodus* pars)
{
    redde materia_nodus_appendere(p->piscina, p->documentum,
        (i32)BRIAR_DOCUMENTUM_PARTES, materia_valor_nodus(pars),
        MATERIA_LOCUS_LISTA_NODUS);
}

interior s32
_finis_lineae (
    constans BriarLinea* l)
{
    redde l->offset + (s32)l->mensura + (s32)l->terminator;
}

/* prosam pendentem usque ad 'ad' deponere (nihil si nulla/vacua) */
interior b32
_prosam_deponere (
    BriarParsura* p,
             s32  ad)
{
    MateriaNodus* n;
    MateriaToken* t;

    si (p->prosa_initium < ZEPHYRUM || ad <= p->prosa_initium)
    {
        p->prosa_initium = (s32)-I;
        redde VERUM;
    }
    n = _nodus(p, BRIAR_GENUS_PROSA);
    t = briar_lexema_fabricare(&p->fabrica, (s32)BRIAR_LEX_PROSA,
        p->prosa_initium, ad);
    p->prosa_initium = (s32)-I;
    si (n == NIHIL || t == NIHIL)
    {
        redde FALSUM;
    }
    redde _ponere_lexema(n, (i32)BRIAR_PROSA_TOK, t)
        && _partem_addere(p, n);
}

/* linea 'index' est '</titulus>' in columna 0 (spatia finalia licent)? */
interior b32
_clausura_est (
    constans BriarLineae* lineae,
                     i32  index,
                  chorda  titulus)
{
    constans BriarLinea* l = briar_linea(lineae, index);
     constans character* t;
                    i32  n;
                    i32  k;

    si (l == NIHIL)
    {
        redde FALSUM;
    }
    t = lineae->fons + l->offset;
    n = l->mensura;
    si (n < titulus.mensura + III || t[0] != '<' || t[1] != '/')
    {
        redde FALSUM;
    }
    si (memcmp(t + II, titulus.datum, (size_t)titulus.mensura)
        != ZEPHYRUM)
    {
        redde FALSUM;
    }
    k = II + titulus.mensura;
    si (t[k] != '>')
    {
        redde FALSUM;
    }
    per (k = k + I; k < n; k++)
    {
        si (t[k] != ' ' && t[k] != '\t')
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* clausuram quaerere ab linea 'ab'; -I si nulla (s32: sentinella
 * negativa in i32 insignato invisibilis esset - examen id notavit) */
interior s32
_clausuram_quaerere (
    constans BriarLineae* lineae,
                     i32  ab,
                  chorda  titulus)
{
    i32 n = briar_lineae_numerus(lineae);
    i32 j;

    per (j = ab; j < n; j++)
    {
        si (_clausura_est(lineae, j, titulus))
        {
            redde (s32)j;
        }
    }
    redde (s32)-I;
}

/* regio cruda ab linea 'index' (tag apertus crudus). Reddit indicem
 * lineae proximae; *bene FALSUM = memoria. */
interior i32
_regionem_legere (
             BriarParsura* p,
                      i32  index,
    constans BriarClassis* c,
                      b32* bene)
{
    constans BriarLinea* l = briar_linea(&p->lineae, index);
                    i32  n = briar_lineae_numerus(&p->lineae);
                    i32  j;
           MateriaNodus* nodus;
           MateriaToken* apertum;
           MateriaToken* titulus;
                    s32  contentum_ab = _finis_lineae(l);
                    s32  contentum_ad;
                    s32  vitium = (s32)BRIAR_VITIUM_NULLUM;

    *bene = FALSUM;
    nodus = _nodus(p, BRIAR_GENUS_REGIO);
    apertum = briar_lexema_fabricare(&p->fabrica,
        (s32)BRIAR_LEX_TAG_APERTUM,
        l->offset, contentum_ab);
    si (nodus == NIHIL || apertum == NIHIL)
    {
        redde n;
    }
    titulus = briar_lexema_derivatum(&p->fabrica,
        (s32)BRIAR_LEX_DERIVATUM,
        c->titulus, apertum);
    si (   !_ponere_lexema(nodus, (i32)BRIAR_REGIO_APERTUM, apertum)
        || !_ponere_lexema(nodus, (i32)BRIAR_REGIO_TITULUS, titulus))
    {
        redde n;
    }
    j = index + I;
    si (!c->auto_clausum)
    {
        s32 clausura = _clausuram_quaerere(&p->lineae, j, c->titulus);

        contentum_ad = (clausura >= ZEPHYRUM)
            ? briar_linea(&p->lineae, (i32)clausura)->offset
            : (s32)p->lineae.mensura;
        si (contentum_ad > contentum_ab)
        {
            MateriaToken* contentum =
                briar_lexema_fabricare(&p->fabrica,
                (s32)BRIAR_LEX_CRUDUM, contentum_ab, contentum_ad);

            si (!_ponere_lexema(nodus, (i32)BRIAR_REGIO_CONTENTUM,
                contentum))
            {
                redde n;
            }
        }
        si (clausura >= ZEPHYRUM)
        {
            constans BriarLinea* lc = briar_linea(&p->lineae,
                (i32)clausura);
                  MateriaToken* clausum =
                      briar_lexema_fabricare(&p->fabrica,
                      (s32)BRIAR_LEX_TAG_CLAUSUM, lc->offset,
                      _finis_lineae(lc));

            si (!_ponere_lexema(nodus, (i32)BRIAR_REGIO_CLAUSUM,
                clausum))
            {
                redde n;
            }
            j = (i32)clausura + I;
        }
        alioquin
        {
            vitium  = (s32)BRIAR_VITIUM_REGIO_NON_CLAUSA;
            j       = n;
        }
    }
    si (   !_ponere_index(nodus, (i32)BRIAR_REGIO_VITIUM, vitium)
        || !_partem_addere(p, nodus))
    {
        redde n;
    }
    *bene = VERUM;
    redde j;
}

/* elementum STML ab linea 'index' (tag apertus non crudus): usque ad
 * '</titulus>' in columna 0 aut linea una se-claudens */
interior i32
_elementum_legere (
             BriarParsura* p,
                      i32  index,
    constans BriarClassis* c,
                      b32* bene)
{
    constans BriarLinea* l = briar_linea(&p->lineae, index);
                    i32  n = briar_lineae_numerus(&p->lineae);
                    i32  j = index + I;
           MateriaNodus* nodus;
           MateriaToken* tok;
           MateriaToken* titulus;
                    s32  ad = _finis_lineae(l);
                    s32  vitium = (s32)BRIAR_VITIUM_NULLUM;

    *bene = FALSUM;
    si (!c->auto_clausum)
    {
        s32 clausura = _clausuram_quaerere(&p->lineae, j, c->titulus);

        si (clausura >= ZEPHYRUM)
        {
            ad  = _finis_lineae(briar_linea(&p->lineae, (i32)clausura));
            j   = (i32)clausura + I;
        }
        alioquin
        {
            ad      = (s32)p->lineae.mensura;
            j       = n;
            vitium  = (s32)BRIAR_VITIUM_ELEMENTUM_NON_CLAUSUM;
        }
    }
    nodus = _nodus(p, BRIAR_GENUS_ELEMENTUM);
    tok = briar_lexema_fabricare(&p->fabrica, (s32)BRIAR_LEX_STML,
        l->offset, ad);
    si (nodus == NIHIL || tok == NIHIL)
    {
        redde n;
    }
    titulus = briar_lexema_derivatum(&p->fabrica,
        (s32)BRIAR_LEX_DERIVATUM,
        c->titulus, tok);
    si (   !_ponere_lexema(nodus, (i32)BRIAR_ELEMENTUM_TOK, tok)
        || !_ponere_lexema(nodus, (i32)BRIAR_ELEMENTUM_TITULUS, titulus)
        || !_ponere_index(nodus, (i32)BRIAR_ELEMENTUM_VITIUM, vitium)
        || !_partem_addere(p, nodus))
    {
        redde n;
    }
    *bene = VERUM;
    redde j;
}

MateriaNodus*
briar_arbor_parsare (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura)
{
    BriarParsura p;
    BriarSaeptum saeptum;
    MateriaToken* finis;
             i32  n;
             i32  i;

    si (piscina == NIHIL || (fons == NIHIL && mensura > ZEPHYRUM))
    {
        redde NIHIL;
    }
    memset(&p, ZEPHYRUM, magnitudo(p));
    p.piscina        = piscina;
    p.prosa_initium  = (s32)-I;
    si (!briar_lineae_scindere(piscina, fons, mensura, &p.lineae))
    {
        redde NIHIL;
    }
    briar_fabrica_incipere(&p.fabrica, piscina, &p.lineae);
    p.documentum = _nodus(&p, BRIAR_GENUS_DOCUMENTUM);
    si (p.documentum == NIHIL)
    {
        redde NIHIL;
    }
    saeptum.aperta     = FALSUM;
    saeptum.signum     = ' ';
    saeptum.longitudo  = ZEPHYRUM;
    n                  = briar_lineae_numerus(&p.lineae);
    i                  = ZEPHYRUM;
    dum (i < n)
    {
        constans BriarLinea* l = briar_linea(&p.lineae, i);
               BriarClassis  c = briar_lineam_classificare(&p.lineae, i,
                   &saeptum);
                       b32 bene;

        si (c.genus == BRIAR_LINEA_INTERPRES)
        {
            MateriaNodus* in;
            MateriaToken* t;

            si (!_prosam_deponere(&p, l->offset))
            {
                redde NIHIL;
            }
            in = _nodus(&p, BRIAR_GENUS_INTERPRES);
            t = briar_lexema_fabricare(&p.fabrica,
                (s32)BRIAR_LEX_INTERPRES,
                l->offset, _finis_lineae(l));
            si (   in == NIHIL
                || !_ponere_lexema(in, (i32)BRIAR_INTERPRES_TOK, t)
                || !materia_nodus_ponere(p.documentum,
                       (i32)BRIAR_DOCUMENTUM_INTERPRES,
                       materia_valor_nodus(in),
                       MATERIA_LOCUS_NODUS))
            {
                redde NIHIL;
            }
            i = i + I;
        }
        alioquin si (c.genus == BRIAR_LINEA_TAG_APERTUM)
        {
            si (!_prosam_deponere(&p, l->offset))
            {
                redde NIHIL;
            }
            i = c.crudus ? _regionem_legere(&p, i, &c, &bene)
                         : _elementum_legere(&p, i, &c, &bene);
            si (!bene)
            {
                redde NIHIL;
            }
        }
        alioquin
        {
            /* prosa, et clausura vaga */
            si (p.prosa_initium < ZEPHYRUM)
            {
                p.prosa_initium = l->offset;
            }
            i = i + I;
        }
    }
    si (!_prosam_deponere(&p, (s32)mensura))
    {
        redde NIHIL;
    }
    finis = briar_lexema_finis(&p.fabrica);
    si (   !_ponere_lexema(p.documentum, (i32)BRIAR_DOCUMENTUM_FINIS,
        finis)
        || !_ponere_index(p.documentum, (i32)BRIAR_DOCUMENTUM_VITIUM,
               saeptum.aperta ? (s32)BRIAR_VITIUM_SAEPTUM_NON_CLAUSUM
                              : (s32)BRIAR_VITIUM_NULLUM))
    {
        redde NIHIL;
    }
    materia_arbor_patres_figere(piscina, p.documentum);
    redde p.documentum;
}

i32
briar_partes_numerus (
    constans MateriaNodus* documentum)
{
    si (   documentum        == NIHIL
        || documentum->genus != (s32)BRIAR_GENUS_DOCUMENTUM)
    {
        redde ZEPHYRUM;
    }
    redde materia_valor_lista_numerus(
        documentum->loci[BRIAR_DOCUMENTUM_PARTES]);
}

MateriaNodus*
briar_pars (
    constans MateriaNodus* documentum,
                      i32  index)
{
    MateriaValor* v;

    si (index < ZEPHYRUM || index >= briar_partes_numerus(documentum))
    {
        redde NIHIL;
    }
    v =
        materia_valor_lista_obtinere(documentum->loci[BRIAR_DOCUMENTUM_PARTES],
        index);
    redde (v != NIHIL
        && v->genus == MATERIA_VALOR_NODUS) ? v->datum.nodus
                                                          : NIHIL;
}

MateriaToken*
briar_lexema (
    constans MateriaNodus* nodus,
                      i32  locus)
{
    si (   nodus == NIHIL || locus < ZEPHYRUM
        || locus >= nodus->numerus_locorum)
    {
        redde NIHIL;
    }
    redde (nodus->loci[locus].genus == MATERIA_VALOR_TOKEN)
        ? nodus->loci[locus].datum.token : NIHIL;
}

chorda
briar_titulus (
    constans MateriaNodus* pars)
{
    MateriaToken* t = NIHIL;
          chorda  vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    si (pars == NIHIL)
    {
        redde vacua;
    }
    si (pars->genus == (s32)BRIAR_GENUS_REGIO)
    {
        t = briar_lexema(pars, (i32)BRIAR_REGIO_TITULUS);
    }
    alioquin si (pars->genus == (s32)BRIAR_GENUS_ELEMENTUM)
    {
        t = briar_lexema(pars, (i32)BRIAR_ELEMENTUM_TITULUS);
    }
    redde (t != NIHIL) ? t->valor : vacua;
}

s32
briar_vitium (
    constans MateriaNodus* nodus)
{
    i32 locus;

    si (nodus == NIHIL)
    {
        redde (s32)BRIAR_VITIUM_NULLUM;
    }
    si (nodus->genus == (s32)BRIAR_GENUS_DOCUMENTUM)
    {
        locus = (i32)BRIAR_DOCUMENTUM_VITIUM;
    }
    alioquin si (nodus->genus == (s32)BRIAR_GENUS_REGIO)
    {
        locus = (i32)BRIAR_REGIO_VITIUM;
    }
    alioquin si (nodus->genus == (s32)BRIAR_GENUS_ELEMENTUM)
    {
        locus = (i32)BRIAR_ELEMENTUM_VITIUM;
    }
    alioquin
    {
        redde (s32)BRIAR_VITIUM_NULLUM;
    }
    redde (nodus->loci[locus].genus == MATERIA_VALOR_INDEX)
        ? nodus->loci[locus].datum.index : (s32)BRIAR_VITIUM_NULLUM;
}

MateriaScriptura
briar_emittere (
               Piscina* piscina,
    constans MateriaNodus* radix)
{
    MateriaScripturaConsilium consilium;

    materia_scriptura_consilium_nudum(&consilium, &BRIAR_REGISTRUM);
    consilium.fons_index = BRIAR_FONS_PLAGULAE;
    redde materia_scribere_nodum(piscina, radix, &consilium);
}
