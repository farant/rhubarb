/* md_inlinea.c - Vide md_inlinea.h.
 *
 * Structura: I. copia contigua + segmenta; II. scansio in seriem
 * ITEMORUM (folium lexematis, nodus factus, fractura) cum acervo
 * delimitatorum et acervo uncorum; III. emphasis processare
 * (appendix A CommonMark); IV. itema in nodos materiae.
 */

#include "md_inlinea.h"
#include "md_lexicon.h"
#include "md_registrum.h"
#include "md_decoctum.h"
#include "materia_token.h"
#include "xar.h"
#include <string.h>
#include "chorda_aedificator.h"


/* ==================================================
 * Segmenta et copia
 * ================================================== */

nomen structura {
             s32  copia_ab;     /* in copia */
             s32  copia_ad;
             s32  fons_ab;      /* in fonte */
             i32  linea;        /* index lineae (ZEPHYRUM-basatus) */
    MateriaNodus* fractura;     /* fractura-mollis POST segmentum (NIHIL ultimo) */
} MdSegmentum;


/* ==================================================
 * Itema
 * ================================================== */

nomen enumeratio {
    ITEM_LEXEMA = 0,   /* folium: lexema unum in spatio copiae */
    ITEM_NODUS,        /* nodus factus */
    ITEM_FRACTURA      /* limes lineae (segmentum k -> k+1) */
} MdItemGenus;

nomen structura MdItem MdItem;
structura MdItem {
    MdItem* prior;
    MdItem* proximus;
       i32  genus;
    /* LEXEMA */
    s32 lex_genus;
    s32 ab;
    s32 ad;
    b32 decoctus;      /* effugium/ens/spatia finalia: valor differt */
    /* NODUS */
    MateriaNodus* nodus;
    /* FRACTURA */
    i32 segmentum;
    b32 dura;
    s32 signum_ab;
    s32 signum_ad;
    /* delimitator ('*' '_' '~') */
    character signum;
          b32 potest_aperire;
          b32 potest_claudere;
          i32 numerus_orig;
    /* uncus ('[' '![') */
    b32 imago;
    b32 activus;
};

nomen structura MdDelim MdDelim;
structura MdDelim {
    MdDelim* prior;
    MdDelim* proximus;
     MdItem* item;
};

nomen structura MdUncus MdUncus;
structura MdUncus {
    MdUncus* prior;
     MdItem* item;
    MdDelim* delim_ante;   /* delimitator summus ante uncum */
        b32  imago;
        b32  activus;
};

nomen structura {
    MdInlineaContextus* c;
             character* copia;
                   s32  mensura;
           MdSegmentum* segmenta;
                   i32  n_segmenta;
                MdItem* primus;
                MdItem* ultimus;
               MdDelim* delim_summus;
               MdUncus* uncus_summus;
} MdInl;


/* ==================================================
 * Adiumenta
 * ================================================== */

interior b32
_spatium (
    character c)
{
    redde (b32)(c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

interior b32
_interpunctio (
    character c)
{
    redde (b32)((c >= '!' && c <= '/') || (c >= ':' && c <= '@')
                || (c >= '[' && c <= '`') || (c >= '{' && c <= '~'));
}

interior b32
_alnum (
    character c)
{
    redde (b32)((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
                || (c >= '0' && c <= '9'));
}

interior character
_char (
    constans MdInl* in,
               s32  i)
{
    redde (i < ZEPHYRUM || i >= in->mensura) ? '\n' : in->copia[i];
}

interior MateriaNodus*
_nodus (
      MdInl* in,
    MdGenus  genus)
{
    redde materia_nodus_creare(in->c->piscina, (s32)genus,
        MD_REGISTRUM.genera[genus].loci_numerus);
}

/* Segmentum continens offset copiae (nova linea inter segmenta pertinet
 * segmento praecedenti pro quaestione) */
interior i32
_segmentum (
    constans MdInl* in,
               s32  o)
{
    i32 k;

    per (k = ZEPHYRUM; k < in->n_segmenta; k++)
    {
        si (o <= in->segmenta[k].copia_ad)
        {
            redde k;
        }
    }
    redde in->n_segmenta - I;
}

/* Lexema ex [ab, ad) copiae, intra segmentum unum */
interior MateriaToken*
_lexema (
     MdInl* in,
       s32  genus,
       s32  ab,
       s32  ad)
{
                     i32  k = _segmentum(in, ab);
    constans MdSegmentum* s = &in->segmenta[k];
                     s32  fab  = s->fons_ab + (ab
                         - s->copia_ab);

    redde md_lexema_fabricare(in->c->fabrica, genus, s->linea, fab,
        fab + (ad - ab));
}

interior b32
_appendere_lexema (
           MdInl* in,
    MateriaNodus* nodus,
             i32  locus,
    MateriaToken* t)
{
    si (t == NIHIL)
    {
        redde FALSUM;
    }
    redde materia_nodus_appendere(in->c->piscina, nodus, locus,
        materia_valor_token(t), MATERIA_LOCUS_LISTA_TOKEN);
}

interior b32
_appendere_nodum (
           MdInl* in,
    MateriaNodus* nodus,
             i32  locus,
    MateriaNodus* filius)
{
    si (filius == NIHIL)
    {
        redde FALSUM;
    }
    redde materia_nodus_appendere(in->c->piscina, nodus, locus,
        materia_valor_nodus(filius), MATERIA_LOCUS_LISTA_NODUS);
}

/* Lexemata fracturae k (terminator + praefixa lineae sequentis) in locum
 * LISTA_TOKEN appendere */
interior b32
_fracturam_appendere_cruda (
           MdInl* in,
    MateriaNodus* nodus,
             i32  locus,
             i32  k)
{
    MateriaNodus* fr = in->segmenta[k].fractura;
    MateriaValor  pv;
             i32  n;
             i32  i;

    si (fr == NIHIL)
    {
        redde VERUM;
    }
    si (   fr->loci[MD_MOLLIS_FINIS].genus == MATERIA_VALOR_TOKEN
        && !_appendere_lexema(in, nodus, locus,
               fr->loci[MD_MOLLIS_FINIS].datum.token))
    {
        redde FALSUM;
    }
    pv = fr->loci[MD_MOLLIS_PRAEFIXA];
    si (pv.genus != MATERIA_VALOR_LISTA)
    {
        redde VERUM;
    }
    n = materia_valor_lista_numerus(pv);
    per (i = ZEPHYRUM; i < n; i++)
    {
        si (!_appendere_lexema(in, nodus, locus,
                materia_valor_lista_obtinere(pv, i)->datum.token))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* [ab, ad) copiae ut lexemata generis dati in locum LISTA_TOKEN, per
 * segmenta scissa, fracturis crudis inter */
interior b32
_lexemata_inter (
           MdInl* in,
    MateriaNodus* nodus,
             i32  locus,
             s32  genus,
             s32  ab,
             s32  ad)
{
    s32 i = ab;

    dum (i < ad)
    {
        i32 k      = _segmentum(in, i);
        s32 fin    = in->segmenta[k].copia_ad;
        s32 usque  = (ad < fin) ? ad : fin;

        dum (i < usque)
        {
            s32 fin = md_scissio_proxima(in->copia, i, usque);

            si (!_appendere_lexema(in, nodus, locus, _lexema(in, genus,
                i, fin)))
            {
                redde FALSUM;
            }
            i = fin;
        }
        si (i < ad)
        {
            /* '\n' inter segmenta */
            si (!_fracturam_appendere_cruda(in, nodus, locus, k))
            {
                redde FALSUM;
            }
            i = i + I;
        }
    }
    redde VERUM;
}

interior MateriaToken*
_derivatum (
     MdInl* in,
    chorda  valor,
       s32  origo_ab)
{
    MateriaToken* origo = _lexema(in, (s32)MD_LEX_TEXTUS, origo_ab,
        origo_ab);

    redde md_lexema_derivatum(in->c->fabrica, (s32)MD_LEX_DERIVATUM,
        valor, origo);
}

interior chorda
_chorda_copiae (
    constans MdInl* in,
               s32  ab,
               s32  ad)
{
    chorda c;
    unio { character* c; i8* m; } u;

    u.c        = in->copia + ab;
    c.datum    = u.m;
    c.mensura  = (i32)(ad - ab);
    redde c;
}


/* ==================================================
 * Itema: creatio et lista
 * ================================================== */

interior MdItem*
_item_novus (
     MdInl* in,
       i32  genus)
{
    MdItem* it = (MdItem*)piscina_allocare(in->c->piscina,
        (memoriae_index)magnitudo(MdItem));

    si (it == NIHIL)
    {
        redde NIHIL;
    }
    memset(it, ZEPHYRUM, magnitudo(*it));
    it->genus = genus;
    redde it;
}

interior vacuum
_item_appendere (
     MdInl* in,
    MdItem* it)
{
    it->prior     = in->ultimus;
    it->proximus  = NIHIL;
    si (in->ultimus != NIHIL)
    {
        in->ultimus->proximus = it;
    }
    alioquin
    {
        in->primus = it;
    }
    in->ultimus = it;
}

interior MdItem*
_lexema_item (
     MdInl* in,
       s32  genus,
       s32  ab,
       s32  ad,
       b32  decoctus)
{
    MdItem* it = _item_novus(in, ITEM_LEXEMA);

    si (it == NIHIL)
    {
        redde NIHIL;
    }
    it->lex_genus  = genus;
    it->ab         = ab;
    it->ad         = ad;
    it->decoctus   = decoctus;
    _item_appendere(in, it);
    redde it;
}

interior MdItem*
_nodus_item (
           MdInl* in,
    MateriaNodus* nodus)
{
    MdItem* it = _item_novus(in, ITEM_NODUS);

    si (it == NIHIL)
    {
        redde NIHIL;
    }
    it->nodus = nodus;
    _item_appendere(in, it);
    redde it;
}

/* Itema [a, b] (inclusiva) e lista excidere, per 'novum' substituere */
interior vacuum
_substituere (
     MdInl* in,
    MdItem* a,
    MdItem* b,
    MdItem* novum)
{
    novum->prior     = a->prior;
    novum->proximus  = b->proximus;
    si (a->prior != NIHIL)
    {
        a->prior->proximus = novum;
    }
    alioquin
    {
        in->primus = novum;
    }
    si (b->proximus != NIHIL)
    {
        b->proximus->prior = novum;
    }
    alioquin
    {
        in->ultimus = novum;
    }
}

interior vacuum
_excidere (
     MdInl* in,
    MdItem* it)
{
    si (it->prior != NIHIL)
    {
        it->prior->proximus = it->proximus;
    }
    alioquin
    {
        in->primus = it->proximus;
    }
    si (it->proximus != NIHIL)
    {
        it->proximus->prior = it->prior;
    }
    alioquin
    {
        in->ultimus = it->prior;
    }
}


/* ==================================================
 * IV. Itema -> nodi materiae (praedeclaratio: emphasis recursat)
 * ================================================== */

interior b32
_liberos_construere (
           MdInl* in,
    MateriaNodus* nodus,
             i32  locus,
          MdItem* a,
          MdItem* b);

/* Textus ex cursu itemorum lexematum [a, b] */
interior MateriaNodus*
_textus_ex (
     MdInl* in,
    MdItem* a,
    MdItem* b)
{
    MateriaNodus* t = _nodus(in, MD_GENUS_TEXTUS);
          MdItem* it;
             b32  decoctus  = FALSUM;
             s32  ab        = a->ab;
             s32  ad        = b->ad;

    si (t == NIHIL)
    {
        redde NIHIL;
    }
    per (it = a; ; it = it->proximus)
    {
        si (!_appendere_lexema(in, t, (i32)MD_TEXTUS_CRUDUM,
                _lexema(in, it->lex_genus, it->ab, it->ad)))
        {
            redde NIHIL;
        }
        si (it->decoctus)
        {
            decoctus = VERUM;
        }
        si (it == b)
        {
            frange;
        }
    }
    si (decoctus)
    {
           b32 mutatus;
        chorda valor;
           s32 fin = ad;

        /* spatia finalia lineae ultimae: e valore sublata */
        si (b->lex_genus == (s32)MD_LEX_SPATIA_FINALIA)
        {
            fin = b->ab;
        }
        valor = md_decoquere(in->c->piscina, in->copia + ab, (i32)(fin
            - ab),
            &mutatus);
        si (valor.datum == NIHIL && fin > ab)
        {
            redde NIHIL;
        }
        si (!materia_nodus_ponere(t, (i32)MD_TEXTUS_VALOR,
                materia_valor_token(_derivatum(in, valor, ab)),
                MATERIA_LOCUS_TOKEN))
        {
            redde NIHIL;
        }
    }
    redde t;
}
hic_manens i8 OCTETUS_NOVAE_LINEAE[I] = { (i8)'\n' };

/* Lexemata loci LISTA_TOKEN (crudum) appendere */
interior vacuum
_lexemata_plana (
        ChordaAedificator* aed,
    constans MateriaValor* v)
{
    i32 m;
    i32 j;

    si (v->genus != MATERIA_VALOR_LISTA)
    {
        redde;
    }
    m = materia_valor_lista_numerus(*v);
    per (j = ZEPHYRUM; j < m; j++)
    {
        constans MateriaValor* e = materia_valor_lista_obtinere(*v, j);

        si (e != NIHIL && e->genus == MATERIA_VALOR_TOKEN)
        {
            chorda_aedificator_appendere_chorda(aed,
                e->datum.token->valor);
        }
    }
}

interior vacuum
_textum_planum_appendere (
        ChordaAedificator* aed,
    constans MateriaNodus* n);

interior vacuum
_liberos_planos (
        ChordaAedificator* aed,
    constans MateriaValor* v)
{
    i32 m;
    i32 j;

    si (v->genus != MATERIA_VALOR_LISTA)
    {
        redde;
    }
    m = materia_valor_lista_numerus(*v);
    per (j = ZEPHYRUM; j < m; j++)
    {
        constans MateriaValor* e = materia_valor_lista_obtinere(*v, j);

        si (e != NIHIL && e->genus == MATERIA_VALOR_NODUS)
        {
            _textum_planum_appendere(aed, e->datum.nodus);
        }
    }
}

/* Textus planus subarboris inlineae (alt imaginis, CommonMark par.
 * 6.4: descriptio ut textus nudus - ornamenta sublata, nexus per
 * liberos, imago nidificata per alt suum, fracturae '\n', html
 * inlineum nihil). Valor decoctus praefertur crudo. */
interior vacuum
_textum_planum_appendere (
        ChordaAedificator* aed,
    constans MateriaNodus* n)
{
    si (n == NIHIL)
    {
        redde;
    }
    commutatio (n->genus)
    {
        casus (s32)MD_GENUS_TEXTUS:
            si (n->loci[MD_TEXTUS_VALOR].genus == MATERIA_VALOR_TOKEN)
            {
                chorda_aedificator_appendere_chorda(aed,
                    n->loci[MD_TEXTUS_VALOR].datum.token->valor);
            }
            alioquin
            {
                _lexemata_plana(aed, &n->loci[MD_TEXTUS_CRUDUM]);
            }
            frange;
        casus (s32)MD_GENUS_VERBATIM:
            si (n->loci[MD_VERBATIM_VALOR].genus == MATERIA_VALOR_TOKEN)
            {
                chorda_aedificator_appendere_chorda(aed,
                    n->loci[MD_VERBATIM_VALOR].datum.token->valor);
            }
            alioquin
            {
                _lexemata_plana(aed, &n->loci[MD_VERBATIM_CRUDUM]);
            }
            frange;
        casus (s32)MD_GENUS_EMPHASIS:
        casus (s32)MD_GENUS_FORTIS:
        casus (s32)MD_GENUS_DELETIO:
            _liberos_planos(aed, &n->loci[MD_ORNATUS_LIBERI]);
            frange;
        casus (s32)MD_GENUS_NEXUS:
            _liberos_planos(aed, &n->loci[MD_NEXUS_LIBERI]);
            frange;
        casus (s32)MD_GENUS_IMAGO:
            si (n->loci[MD_NEXUS_ALT].genus == MATERIA_VALOR_TOKEN)
            {
                chorda_aedificator_appendere_chorda(aed,
                    n->loci[MD_NEXUS_ALT].datum.token->valor);
            }
            alioquin
            {
                _liberos_planos(aed, &n->loci[MD_NEXUS_LIBERI]);
            }
            frange;
        casus (s32)MD_GENUS_FRACTURA_MOLLIS:
        casus (s32)MD_GENUS_FRACTURA_DURA:
            chorda_aedificator_appendere_character(aed, '\n');
            frange;
        ordinarius:
            frange;
    }
}

interior MateriaNodus*
_fractura_ex (
     MdInl* in,
    MdItem* it)
{
    MateriaNodus* vetus = in->segmenta[it->segmentum].fractura;
    MateriaNodus* fr;
          chorda  nl;

    si (vetus == NIHIL)
    {
        redde NIHIL;
    }
    si (!it->dura)
    {
        redde vetus;   /* mollis: nodus originalis servatur */
    }
    fr = _nodus(in, MD_GENUS_FRACTURA_DURA);
    si (fr == NIHIL)
    {
        redde NIHIL;
    }
    si (!materia_nodus_ponere(fr, (i32)MD_DURA_SIGNUM,
            materia_valor_token(_lexema(in,
                (in->copia[it->signum_ab]
                    == '\\') ? (s32)MD_LEX_EFFUGIUM
                                                   : (s32)MD_LEX_SPATIA_FINALIA,
                it->signum_ab, it->signum_ad)), MATERIA_LOCUS_TOKEN))
    {
        redde NIHIL;
    }
    si (   vetus->loci[MD_MOLLIS_FINIS].genus == MATERIA_VALOR_TOKEN
        && !materia_nodus_ponere(fr, (i32)MD_DURA_FINIS,
               vetus->loci[MD_MOLLIS_FINIS], MATERIA_LOCUS_TOKEN))
    {
        redde NIHIL;
    }
    si (   vetus->loci[MD_MOLLIS_PRAEFIXA].genus == MATERIA_VALOR_LISTA
        && !materia_nodus_ponere(fr, (i32)MD_DURA_PRAEFIXA,
               vetus->loci[MD_MOLLIS_PRAEFIXA],
               MATERIA_LOCUS_LISTA_TOKEN))
    {
        redde NIHIL;
    }
    /* valor redditus '\n' post <br/> (B2.1 md) */
    nl.datum    = OCTETUS_NOVAE_LINEAE;
    nl.mensura  = I;
    si (!materia_nodus_ponere(fr, (i32)MD_DURA_VALOR,
            materia_valor_token(_derivatum(in, nl, it->signum_ab)),
            MATERIA_LOCUS_TOKEN))
    {
        redde NIHIL;
    }
    redde fr;
}

interior b32
_liberos_construere (
           MdInl* in,
    MateriaNodus* nodus,
             i32  locus,
          MdItem* a,
          MdItem* b)
{
    MdItem* it = a;

    dum (it != NIHIL)
    {
        MdItem* proximus = (it == b) ? NIHIL : it->proximus;

        si (it->genus == ITEM_LEXEMA)
        {
            MdItem* fin = it;

            dum (   fin                  != b && fin->proximus != NIHIL
                 && fin->proximus->genus == ITEM_LEXEMA)
            {
                fin = fin->proximus;
            }
            si (!_appendere_nodum(in, nodus, locus, _textus_ex(in, it,
                fin)))
            {
                redde FALSUM;
            }
            proximus = (fin == b) ? NIHIL : fin->proximus;
        }
        alioquin si (it->genus == ITEM_NODUS)
        {
            si (!_appendere_nodum(in, nodus, locus, it->nodus))
            {
                redde FALSUM;
            }
        }
        alioquin
        {
            si (!_appendere_nodum(in, nodus, locus, _fractura_ex(in,
                it)))
            {
                redde FALSUM;
            }
        }
        it = proximus;
    }
    redde VERUM;
}

/* Nodus ornatus (emphasis/fortis/deletio) ex apertura, liberis [a, b],
 * clausura; itema substituit. */
interior b32
_ornatum_facere (
      MdInl* in,
    MdGenus  genus,
     MdItem* apertum,
        s32  ap_ab,
        s32  ap_ad,
     MdItem* clausum,
        s32  cl_ab,
        s32  cl_ad)
{
    MateriaNodus* n = _nodus(in, genus);
          MdItem* a = apertum->proximus;
          MdItem* b = clausum->prior;
          MdItem* novum;
             b32  ap_totum = (b32)(ap_ab == apertum->ab);   /* cursus apertus consumptus */
             b32  cl_totum = (b32)(cl_ad == clausum->ad);

    si (n == NIHIL)
    {
        redde FALSUM;
    }
    si (!materia_nodus_ponere(n, (i32)MD_ORNATUS_APERTUM,
            materia_valor_token(_lexema(in, (s32)MD_LEX_DELIMITATOR,
            ap_ab, ap_ad)),
            MATERIA_LOCUS_TOKEN))
    {
        redde FALSUM;
    }
    si (   a != clausum
        && !_liberos_construere(in, n, (i32)MD_ORNATUS_LIBERI, a, b))
    {
        redde FALSUM;
    }
    si (!materia_nodus_ponere(n, (i32)MD_ORNATUS_CLAUSUM,
            materia_valor_token(_lexema(in, (s32)MD_LEX_DELIMITATOR,
            cl_ab, cl_ad)),
            MATERIA_LOCUS_TOKEN))
    {
        redde FALSUM;
    }
    novum = _item_novus(in, ITEM_NODUS);
    si (novum == NIHIL)
    {
        redde FALSUM;
    }
    novum->nodus = n;
    /* extenta SEMPER minuuntur (aliter par idem iterum congruit - ansa
     * infinita natalis): apertura a fine, clausura ab initio */
    apertum->ad = ap_ab;
    clausum->ab = cl_ad;
    si (ap_totum && cl_totum)
    {
        _substituere(in, apertum, clausum, novum);
    }
    alioquin si (ap_totum)
    {
        _substituere(in, apertum, clausum->prior, novum);
    }
    alioquin si (cl_totum)
    {
        _substituere(in, apertum->proximus, clausum, novum);
    }
    alioquin si (apertum->proximus == clausum)
    {
        novum->prior       = apertum;
        novum->proximus    = clausum;
        apertum->proximus  = novum;
        clausum->prior     = novum;
    }
    alioquin
    {
        _substituere(in, apertum->proximus, clausum->prior, novum);
    }
    redde VERUM;
}


/* ==================================================
 * III. Emphasis processare (CommonMark appendix A)
 * ================================================== */

interior vacuum
_delim_excidere (
      MdInl* in,
    MdDelim* d)
{
    si (d->prior != NIHIL)
    {
        d->prior->proximus = d->proximus;
    }
    si (d->proximus != NIHIL)
    {
        d->proximus->prior = d->prior;
    }
    alioquin
    {
        in->delim_summus = d->prior;
    }
}

interior b32
_emphasin_processare (
      MdInl* in,
    MdDelim* fundus)
{
    MdDelim* currens;

    si (in->delim_summus == NIHIL || in->delim_summus == fundus)
    {
        redde VERUM;
    }
    /* infimus supra fundum */
    currens = in->delim_summus;
    dum (currens->prior != fundus)
    {
        currens = currens->prior;
    }
    dum (currens != NIHIL)
    {
        MdItem* cl = currens->item;

        si (!cl->potest_claudere || (cl->ad - cl->ab) == ZEPHYRUM)
        {
            currens = currens->proximus;
            perge;
        }
        {
            MdDelim* op        = currens->prior;
                b32  inventus  = FALSUM;

            dum (op != NIHIL && op != fundus)
            {
                MdItem* o   = op->item;
                   i32  no  = (i32)(o->ad - o->ab);
                   i32  nc  = (i32)(cl->ad - cl->ab);

                si (   o->signum == cl->signum && o->potest_aperire
                    && no > ZEPHYRUM)
                {
                    b32 saltus;

                    si (cl->signum == '~')
                    {
                        saltus = (b32)(no != nc || no > II);
                    }
                    alioquin
                    {
                        saltus = (b32)((o->potest_claudere
                            || cl->potest_aperire)
                            && ((o->numerus_orig + cl->numerus_orig)
                                % (i32)III) == ZEPHYRUM
                            && !((o->numerus_orig % (i32)III)
                                == ZEPHYRUM
                                 && (cl->numerus_orig % (i32)III)
                                     == ZEPHYRUM));
                    }
                    si (!saltus)
                    {
                        inventus = VERUM;
                        frange;
                    }
                }
                op = op->prior;
            }
            si (inventus)
            {
                MdItem* o   = op->item;
                   i32  no  = (i32)(o->ad - o->ab);
                   i32  nc  = (i32)(cl->ad - cl->ab);
                   i32  usus = (cl->signum == '~') ? no : ((no >= II
                       && nc >= II) ? II : I);
                MdGenus genus = (cl->signum == '~') ? MD_GENUS_DELETIO
                              : (usus
                                  == II) ? MD_GENUS_FORTIS : MD_GENUS_EMPHASIS;
                MdDelim* d;
                MdDelim* proximus_delim = currens->proximus;

                d = op->proximus;
                dum (d != currens)
                {
                    MdDelim* px = d->proximus;

                    _delim_excidere(in, d);
                    d = px;
                }
                si (!_ornatum_facere(in, genus, o, o->ad - (s32)usus,
                    o->ad, cl, cl->ab,
                        cl->ab + (s32)usus))
                {
                    redde FALSUM;
                }
                si ((o->ad - o->ab) == ZEPHYRUM)
                {
                    _delim_excidere(in, op);
                }
                si ((cl->ad - cl->ab) == ZEPHYRUM)
                {
                    _delim_excidere(in, currens);
                    currens = proximus_delim;
                }
            }
            alioquin
            {
                MdDelim* px = currens->proximus;

                si (!cl->potest_aperire)
                {
                    _delim_excidere(in, currens);
                }
                currens = px;
            }
        }
    }
    dum (in->delim_summus != NIHIL && in->delim_summus != fundus)
    {
        _delim_excidere(in, in->delim_summus);
    }
    redde VERUM;
}


/* ==================================================
 * II. Scansio
 * ================================================== */

interior b32
_delim_impellere (
     MdInl* in,
    MdItem* it)
{
    MdDelim* d = (MdDelim*)piscina_allocare(in->c->piscina,
        (memoriae_index)magnitudo(MdDelim));

    si (d == NIHIL)
    {
        redde FALSUM;
    }
    d->item      = it;
    d->prior     = in->delim_summus;
    d->proximus  = NIHIL;
    si (in->delim_summus != NIHIL)
    {
        in->delim_summus->proximus = d;
    }
    in->delim_summus = d;
    redde VERUM;
}

/* Cursus delimitatorum a i: flanking, item + delim */
interior s32
_cursus_delimitatorum (
     MdInl* in,
       s32  i)
{
    character  c = in->copia[i];
          s32  j = i;
    character  ante;
    character  post;
          b32  sin_flank;
          b32  dex_flank;
       MdItem* it;

    dum (j < in->mensura && in->copia[j] == c)
    {
        j = j + I;
    }
    ante = _char(in, i - I);
    post = _char(in, j);
    sin_flank = (b32)(!_spatium(post)
        && (!_interpunctio(post) || _spatium(ante)
        || _interpunctio(ante)));
    dex_flank = (b32)(!_spatium(ante)
        && (!_interpunctio(ante) || _spatium(post)
        || _interpunctio(post)));
    it = _lexema_item(in, (s32)MD_LEX_DELIMITATOR, i, j, FALSUM);
    si (it == NIHIL)
    {
        redde (s32)-I;
    }
    it->signum        = c;
    it->numerus_orig  = (i32)(j - i);
    si (c == '_')
    {
        it->potest_aperire = (b32)(sin_flank
            && (!dex_flank || _interpunctio(ante)));
        it->potest_claudere = (b32)(dex_flank
            && (!sin_flank || _interpunctio(post)));
    }
    alioquin
    {
        it->potest_aperire   = sin_flank;
        it->potest_claudere  = dex_flank;
    }
    si (c == '~' && (j - i) > II)
    {
        it->potest_aperire   = FALSUM;
        it->potest_claudere  = FALSUM;
    }
    si (!_delim_impellere(in, it))
    {
        redde (s32)-I;
    }
    redde j;
}

/* Code span a i ('`'): reddit finem aut -I (litterae) */
interior s32
_verbatim (
     MdInl* in,
       s32  i)
{
    s32 j = i;
    i32 n;
    s32 k;

    dum (j < in->mensura && in->copia[j] == '`')
    {
        j = j + I;
    }
    n = (i32)(j - i);
    k = j;
    dum (k < in->mensura)
    {
        si (in->copia[k] == '`')
        {
            s32 m = k;

            dum (m < in->mensura && in->copia[m] == '`')
            {
                m = m + I;
            }
            si ((i32)(m - k) == n)
            {
                MateriaNodus* v = _nodus(in, MD_GENUS_VERBATIM);
                MateriaToken* ap = _lexema(in, (s32)MD_LEX_GRAVIS, i,
                    j);
                MateriaToken* cl = _lexema(in, (s32)MD_LEX_GRAVIS, k,
                    m);
                      chorda  valor;
                   character* d;
                         s32  q;
                         i32  len          = ZEPHYRUM;
                         b32  spatia_sola  = VERUM;

                si (   v == NIHIL || ap == NIHIL || cl == NIHIL
                    || !materia_nodus_ponere(v,
                    (i32)MD_VERBATIM_APERTUM,
                           materia_valor_token(ap), MATERIA_LOCUS_TOKEN)
                    || !_lexemata_inter(in, v, (i32)MD_VERBATIM_CRUDUM,
                           (s32)MD_LEX_TEXTUS, j, k)
                    || !materia_nodus_ponere(v,
                    (i32)MD_VERBATIM_CLAUSUM,
                           materia_valor_token(cl),
                           MATERIA_LOCUS_TOKEN))
                {
                    redde (s32)-II;
                }
                /* valor: lineae novae -> spatia; spatium unum utrimque
                 * sublatum si utrimque adest et non spatia sola */
                d = (character*)piscina_allocare(in->c->piscina,
                    (memoriae_index)(k - j) + I);
                si (d == NIHIL)
                {
                    redde (s32)-II;
                }
                per (q = j; q < k; q++)
                {
                    character ch = in->copia[q];

                    d[len] = (ch == '\n') ? ' ' : ch;
                    si (d[len] != ' ')
                    {
                        spatia_sola = FALSUM;
                    }
                    len = len + I;
                }
                {
                    s32 va = ZEPHYRUM;
                    s32 vb = (s32)len;

                    si (   len >= II && d[0] == ' ' && d[len - I] == ' '
                        && !spatia_sola)
                    {
                        va = I;
                        vb = (s32)len - I;
                    }
                    valor.mensura = (i32)(vb - va);
                    {
                        unio { character* c; i8* m; } u;

                        u.c          = d + va;
                        valor.datum  = u.m;
                    }
                    /* differt a crudo? (linea nova aut spatia sublata) */
                    si (   valor.mensura != (i32)(k - j)
                        || memcmp(valor.datum, in->copia + j,
                        (size_t)valor.mensura) != ZEPHYRUM)
                    {
                        si (!materia_nodus_ponere(v,
                            (i32)MD_VERBATIM_VALOR,
                                materia_valor_token(_derivatum(in,
                                valor, j)),
                                MATERIA_LOCUS_TOKEN))
                        {
                            redde (s32)-II;
                        }
                    }
                }
                si (_nodus_item(in, v) == NIHIL)
                {
                    redde (s32)-II;
                }
                redde m;
            }
            k = m;
            perge;
        }
        k = k + I;
    }
    redde (s32)-I;
}

/* Autonexus '<scheme:...>' aut '<email>' aut html inlineum a i ('<').
 * Reddit finem aut -I. */
interior s32
_angulus (
     MdInl* in,
       s32  i)
{
    s32 j = i + I;
    s32 k;
    b32 scheme_ok = FALSUM;

    /* autonexus URI: scheme = littera + [a-zA-Z0-9+.-]{1,31} ':' , deinde
     * sine spatiis/'<'/'>' usque ad '>' */
    si (j < in->mensura && ((in->copia[j] >= 'a' && in->copia[j] <= 'z')
                            || (in->copia[j] >= 'A'
                                && in->copia[j] <= 'Z')))
    {
        k = j + I;
        dum (   k < in->mensura
             && (_alnum(in->copia[k]) || in->copia[k] == '+'
                                 || in->copia[k] == '.'
                                 || in->copia[k] == '-')
             && (k - j) < (s32)32)
        {
            k = k + I;
        }
        si (k < in->mensura && in->copia[k] == ':' && (k - j) >= II)
        {
            scheme_ok = VERUM;
        }
    }
    si (scheme_ok)
    {
        k = j;
        dum (   k < in->mensura && in->copia[k] != '>'
             && in->copia[k] != '<'
             && !_spatium(in->copia[k])
             && (insignatus character)in->copia[k] > 0x1F)
        {
            k = k + I;
        }
        si (k < in->mensura && in->copia[k] == '>')
        {
            MateriaNodus* nx = _nodus(in, MD_GENUS_NEXUS);
            MateriaNodus* t;

            si (   nx == NIHIL
                || !materia_nodus_ponere(nx, (i32)MD_NEXUS_FORMA,
                       materia_valor_index((s32)MD_NEXUS_AUTO),
                       MATERIA_LOCUS_INDEX)
                || !materia_nodus_ponere(nx, (i32)MD_NEXUS_APERTUM,
                       materia_valor_token(_lexema(in,
                       (s32)MD_LEX_NEXUS_APERTUM, i, i + I)),
                       MATERIA_LOCUS_TOKEN))
            {
                redde (s32)-II;
            }
            t = _nodus(in, MD_GENUS_TEXTUS);
            si (   t                   == NIHIL
                || !_appendere_lexema(in, t, (i32)MD_TEXTUS_CRUDUM,
                       _lexema(in, (s32)MD_LEX_AUTONEXUS, j, k))
                || !_appendere_nodum(in, nx, (i32)MD_NEXUS_LIBERI, t)
                || !_appendere_lexema(in, nx, (i32)MD_NEXUS_CAUDA,
                       _lexema(in, (s32)MD_LEX_NEXUS_CAUDA, k, k + I))
                || !materia_nodus_ponere(nx, (i32)MD_NEXUS_URL,
                       materia_valor_token(_derivatum(in,
                       md_url_codificare(in->c->piscina,
                       _chorda_copiae(in, j, k)), j)),
                       MATERIA_LOCUS_TOKEN)
                || _nodus_item(in, nx) == NIHIL)
            {
                redde (s32)-II;
            }
            redde k + I;
        }
    }
    /* autonexus email: local@domain, sine spatiis, '>' */
    {
        s32 at = (s32)-I;

        k = j;
        dum (   k < in->mensura
             && (_alnum(in->copia[k])
            || strchr(".!#$%&'*+/=?^_`{|}~-@", in->copia[k]) != NIHIL))
        {
            si (in->copia[k] == '@')
            {
                si (at >= ZEPHYRUM)
                {
                    at = (s32)-II;
                    frange;
                }
                at = k;
            }
            k = k + I;
        }
        si (   at > j && k < in->mensura && in->copia[k] == '>'
            && at + I < k
            && in->copia[k - I] != '-' && in->copia[k - I] != '.')
        {
            MateriaNodus* nx = _nodus(in, MD_GENUS_NEXUS);
            MateriaNodus* t;
            chorda        url;
            character*    d =
                (character*)piscina_allocare(in->c->piscina,
                (memoriae_index)(k - j) + VIII);

            si (nx == NIHIL || d == NIHIL)
            {
                redde (s32)-II;
            }
            memcpy(d, "mailto:", VII);
            memcpy(d + VII, in->copia + j, (size_t)(k - j));
            {
                unio { character* c; i8* m; } u;

                u.c          = d;
                url.datum    = u.m;
                url.mensura  = (i32)(k - j) + VII;
            }
            t = _nodus(in, MD_GENUS_TEXTUS);
            si (   t                   == NIHIL
                || !materia_nodus_ponere(nx, (i32)MD_NEXUS_FORMA,
                       materia_valor_index((s32)MD_NEXUS_AUTO),
                       MATERIA_LOCUS_INDEX)
                || !materia_nodus_ponere(nx, (i32)MD_NEXUS_APERTUM,
                       materia_valor_token(_lexema(in,
                       (s32)MD_LEX_NEXUS_APERTUM, i, i + I)),
                       MATERIA_LOCUS_TOKEN)
                || !_appendere_lexema(in, t, (i32)MD_TEXTUS_CRUDUM,
                       _lexema(in, (s32)MD_LEX_AUTONEXUS, j, k))
                || !_appendere_nodum(in, nx, (i32)MD_NEXUS_LIBERI, t)
                || !_appendere_lexema(in, nx, (i32)MD_NEXUS_CAUDA,
                       _lexema(in, (s32)MD_LEX_NEXUS_CAUDA, k, k + I))
                || !materia_nodus_ponere(nx, (i32)MD_NEXUS_URL,
                materia_valor_token(_derivatum(in,
                md_url_codificare(in->c->piscina, url), j)),
                MATERIA_LOCUS_TOKEN)
                || _nodus_item(in, nx) == NIHIL)
            {
                redde (s32)-II;
            }
            redde k + I;
        }
    }
    /* html inlineum: tag apertum, clausum, commentum, PI, declaratio, CDATA */
    {
        s32 fin = (s32)-I;

        si (j < in->mensura && in->copia[j] == '/')
        {
            k = j + I;
            si (   k < in->mensura
                && ((in->copia[k] >= 'a' && in->copia[k] <= 'z')
                                    || (in->copia[k] >= 'A'
                                        && in->copia[k] <= 'Z')))
            {
                dum (   k < in->mensura
                     && (_alnum(in->copia[k]) || in->copia[k] == '-'))
                {
                    k = k + I;
                }
                dum (k < in->mensura && _spatium(in->copia[k]))
                {
                    k = k + I;
                }
                si (k < in->mensura && in->copia[k] == '>')
                {
                    fin = k + I;
                }
            }
        }
        alioquin si (   j + II < in->mensura && in->copia[j] == '!'
                     && in->copia[j + I]  == '-'
                     && in->copia[j + II] == '-')
        {
            k = j + III;
            dum (k + II < in->mensura)
            {
                si (   in->copia[k] == '-' && in->copia[k + I] == '-'
                    && in->copia[k + II] == '>')
                {
                    fin = k + III;
                    frange;
                }
                k = k + I;
            }
        }
        alioquin si (j < in->mensura && in->copia[j] == '?')
        {
            k = j + I;
            dum (k + I < in->mensura)
            {
                si (in->copia[k] == '?' && in->copia[k + I] == '>')
                {
                    fin = k + II;
                    frange;
                }
                k = k + I;
            }
        }
        alioquin si (   j + VIII < in->mensura
                     && memcmp(in->copia + j, "![CDATA[", VIII)
                         == ZEPHYRUM)
        {
            k = j + VIII;
            dum (k + II < in->mensura)
            {
                si (   in->copia[k] == ']' && in->copia[k + I] == ']'
                    && in->copia[k + II] == '>')
                {
                    fin = k + III;
                    frange;
                }
                k = k + I;
            }
        }
        alioquin si (   j + I < in->mensura && in->copia[j] == '!'
                     && ((in->copia[j + I] >= 'a'
                     && in->copia[j + I] <= 'z')
                         || (in->copia[j + I] >= 'A'
                             && in->copia[j + I] <= 'Z')))
        {
            k = j + I;
            dum (k < in->mensura && in->copia[k] != '>')
            {
                k = k + I;
            }
            si (k < in->mensura)
            {
                fin = k + I;
            }
        }
        alioquin si (   j < in->mensura
                     && ((in->copia[j] >= 'a' && in->copia[j] <= 'z')
                                         || (in->copia[j] >= 'A'
                                             && in->copia[j] <= 'Z')))
        {
            /* tag apertum cum attributis */
            k = j + I;
            dum (   k < in->mensura
                 && (_alnum(in->copia[k]) || in->copia[k] == '-'))
            {
                k = k + I;
            }
            dum (VERUM)
            {
                s32 s = k;

                dum (s < in->mensura && _spatium(in->copia[s]))
                {
                    s = s + I;
                }
                si (s >= in->mensura)
                {
                    frange;
                }
                si (in->copia[s] == '/')
                {
                    si (s + I < in->mensura && in->copia[s + I] == '>')
                    {
                        fin = s + II;
                    }
                    frange;
                }
                si (in->copia[s] == '>')
                {
                    fin = s + I;
                    frange;
                }
                si (s == k)
                {
                    frange;   /* attributum sine spatio */
                }
                k = s;
                si (!((in->copia[k] >= 'a' && in->copia[k] <= 'z')
                      || (in->copia[k] >= 'A' && in->copia[k] <= 'Z')
                      || in->copia[k] == '_' || in->copia[k] == ':'))
                {
                    frange;
                }
                dum (   k < in->mensura
                     && (_alnum(in->copia[k]) || in->copia[k] == '_'
                                         || in->copia[k] == '.'
                                         || in->copia[k] == ':'
                                         || in->copia[k] == '-'))
                {
                    k = k + I;
                }
                s = k;
                dum (s < in->mensura && _spatium(in->copia[s]))
                {
                    s = s + I;
                }
                si (s < in->mensura && in->copia[s] == '=')
                {
                    s = s + I;
                    dum (s < in->mensura && _spatium(in->copia[s]))
                    {
                        s = s + I;
                    }
                    si (s >= in->mensura)
                    {
                        frange;
                    }
                    si (in->copia[s] == '"' || in->copia[s] == '\'')
                    {
                        character q = in->copia[s];

                        s = s + I;
                        dum (s < in->mensura && in->copia[s] != q)
                        {
                            s = s + I;
                        }
                        si (s >= in->mensura)
                        {
                            frange;
                        }
                        k = s + I;
                    }
                    alioquin
                    {
                        s32 v = s;

                        dum (v < in->mensura && !_spatium(in->copia[v])
                             && strchr("\"'=<>`", in->copia[v])
                                   == NIHIL)
                        {
                            v = v + I;
                        }
                        si (v == s)
                        {
                            frange;
                        }
                        k = v;
                    }
                }
            }
        }
        si (fin > ZEPHYRUM)
        {
            MateriaNodus* h = _nodus(in, MD_GENUS_HTML_INLINEUM);
                     s32  q;
                     b32  lineae_plures = FALSUM;

            si (   h == NIHIL
                || !_lexemata_inter(in, h, (i32)MD_HTML_INLINEUM_CRUDUM,
                       (s32)MD_LEX_HTML, i, fin))
            {
                redde (s32)-II;
            }
            /* lineas plures tenens: valor derivatus cum '\n' (terminator
             * octetos in proiectione non fert - B3.3) */
            per (q = i; q < fin; q++)
            {
                si (in->copia[q] == '\n')
                {
                    lineae_plures = VERUM;
                    frange;
                }
            }
            si (   lineae_plures
                && !materia_nodus_ponere(h, (i32)MD_HTML_INLINEUM_VALOR,
                       materia_valor_token(_derivatum(in,
                           _chorda_copiae(in, i, fin), i)),
                       MATERIA_LOCUS_TOKEN))
            {
                redde (s32)-II;
            }
            si (_nodus_item(in, h) == NIHIL)
            {
                redde (s32)-II;
            }
            redde fin;
        }
    }
    redde (s32)-I;
}

/* Autonexus GFM nudus a i (www. / http:// / https://): reddit finem aut -I */
interior s32
_autonexus_nudus (
     MdInl* in,
       s32  i)
{
          s32 j = i;
          s32 k;
          b32 www   = FALSUM;
    character ante  = _char(in, i - I);

    si (!(_spatium(ante) || ante == '*' || ante == '_' || ante == '~'
        || ante == '('))
    {
        redde (s32)-I;
    }
    si (   i + IV                            <= in->mensura
        && memcmp(in->copia + i, "www.", IV) == ZEPHYRUM)
    {
        www  = VERUM;
        j    = i + IV;
    }
    alioquin si (   i + VII                               <= in->mensura
                 && memcmp(in->copia + i, "http://", VII) == ZEPHYRUM)
    {
        j = i + VII;
    }
    alioquin si (   i + VIII <= in->mensura
                 && memcmp(in->copia + i, "https://", VIII) == ZEPHYRUM)
    {
        j = i + VIII;
    }
    alioquin
    {
        redde (s32)-I;
    }
    /* domain: alnum/_/-/. cum puncto uno saltem */
    k = j;
    {
        b32 punctum  = FALSUM;
        s32 d        = k;

        dum (   d < in->mensura
             && (_alnum(in->copia[d]) || in->copia[d] == '_'
                                 || in->copia[d] == '-'
                                 || in->copia[d] == '.'))
        {
            si (in->copia[d] == '.')
            {
                punctum = VERUM;
            }
            d = d + I;
        }
        si (!punctum || d == k)
        {
            redde (s32)-I;
        }
        k = d;
    }
    dum (   k < in->mensura && !_spatium(in->copia[k])
         && in->copia[k] != '<')
    {
        k = k + I;
    }
    /* interpunctio finalis exclusa; parentheses aequatae */
    dum (k > j && strchr("?!.,:*_~'\"", in->copia[k - I]) != NIHIL)
    {
        k = k - I;
    }
    si (k > j && in->copia[k - I] == ')')
    {
        i32 ap = ZEPHYRUM;
        i32 cl = ZEPHYRUM;
        s32 q;

        per (q = j; q < k; q++)
        {
            si (in->copia[q] == '(') ap++;
            si (in->copia[q] == ')') cl++;
        }
        dum (cl > ap && k > j && in->copia[k - I] == ')')
        {
            k   = k - I;
            cl  = cl - I;
        }
    }
    si (k <= j)
    {
        redde (s32)-I;
    }
    {
        MateriaNodus* nx = _nodus(in, MD_GENUS_NEXUS);
        MateriaNodus* t;
        chorda        url;

        si (www)
        {
            character* d = (character*)piscina_allocare(in->c->piscina,
                (memoriae_index)(k - i) + VIII);
            unio { character* c; i8* m; } u;

            si (d == NIHIL)
            {
                redde (s32)-II;
            }
            memcpy(d, "http://", VII);
            memcpy(d + VII, in->copia + i, (size_t)(k - i));
            u.c          = d;
            url.datum    = u.m;
            url.mensura  = (i32)(k - i) + VII;
        }
        alioquin
        {
            url = _chorda_copiae(in, i, k);
        }
        t = _nodus(in, MD_GENUS_TEXTUS);
        si (   nx                  == NIHIL || t == NIHIL
            || !materia_nodus_ponere(nx, (i32)MD_NEXUS_FORMA,
                   materia_valor_index((s32)MD_NEXUS_AUTO),
                   MATERIA_LOCUS_INDEX)
            || !_appendere_lexema(in, t, (i32)MD_TEXTUS_CRUDUM,
                   _lexema(in, (s32)MD_LEX_AUTONEXUS, i, k))
            || !_appendere_nodum(in, nx, (i32)MD_NEXUS_LIBERI, t)
            || !materia_nodus_ponere(nx, (i32)MD_NEXUS_URL,
            materia_valor_token(_derivatum(in,
            md_url_codificare(in->c->piscina, url), i)),
            MATERIA_LOCUS_TOKEN)
            || _nodus_item(in, nx) == NIHIL)
        {
            redde (s32)-II;
        }
    }
    redde k;
}

/* Spatia intra caudam nexus: spatia, tabuli, et lineam novam UNAM
 * ad summum (CommonMark par. 6.6: 'spaces, tabs, and up to one line
 * ending') - reddit indicem post spatia */
interior s32
_spatia_caudae (
    constans MdInl* in,
               s32  j,
               s32  fin)
{
    b32 linea_nova = FALSUM;

    dum (j < fin)
    {
        character c = in->copia[j];

        si (c == ' ' || c == '\t')
        {
            j = j + I;
            perge;
        }
        si (c == '\n' && !linea_nova)
        {
            linea_nova  = VERUM;
            j           = j + I;
            perge;
        }
        frange;
    }
    redde j;
}

/* Cauda nexus inlinei a i ('(' post ']'): destinatio et descriptio in
 * eadem linea. Reddit finem (post ')') aut -I. Extenta per *da..*tb. */
interior s32
_cauda_inlinea (
     MdInl* in,
       s32  i,
       s32* da,
       s32* db,
       s32* ta,
       s32* tb,
       b32* descriptio)
{
    s32 j    = i + I;
    s32 fin  = in->mensura;
    s32 k;

    *descriptio = FALSUM;
    /* spatia (cum linea nova una) post '(' - B3.3: olim linea una sola */
    j = _spatia_caudae(in, j, fin);
    si (j < fin && in->copia[j] == '<')
    {
        k = j + I;
        dum (   k < fin && in->copia[k] != '>' && in->copia[k] != '<'
             && in->copia[k] != '\n')
        {
            si (in->copia[k] == '\\' && k + I < fin)
            {
                k = k + I;
            }
            k = k + I;
        }
        si (k >= fin || in->copia[k] != '>')
        {
            redde (s32)-I;
        }
        *da  = j + I;
        *db  = k;
        j    = k + I;
    }
    alioquin
    {
        i32 profunditas = ZEPHYRUM;

        k = j;
        dum (   k < fin && in->copia[k] != ' ' && in->copia[k] != '\t'
             && (insignatus character)in->copia[k] > 0x1F)
        {
            si (in->copia[k] == '\\' && k + I < fin)
            {
                k = k + II;
                perge;
            }
            si (in->copia[k] == '(')
            {
                profunditas = profunditas + I;
            }
            alioquin si (in->copia[k] == ')')
            {
                si (profunditas == ZEPHYRUM)
                {
                    frange;
                }
                profunditas = profunditas - I;
            }
            k = k + I;
        }
        si (profunditas != ZEPHYRUM)
        {
            redde (s32)-I;
        }
        *da  = j;
        *db  = k;
        j    = k;
    }
    {
        s32 post = _spatia_caudae(in, j, fin);

        si (post < fin && in->copia[post] == ')')
        {
            redde post + I;
        }
        si (post == j || post >= fin)
        {
            redde (s32)-I;
        }
        si (   in->copia[post] == '"' || in->copia[post] == '\''
            || in->copia[post] == '(')
        {
            character cl = (in->copia[post]
                == '(') ? ')' : in->copia[post];

            k = post + I;
            dum (k < fin && in->copia[k] != cl)
            {
                /* descriptio lineas tenere potest, vacuam non */
                si (   in->copia[k]     == '\n' && k + I < fin
                    && in->copia[k + I] == '\n')
                {
                    redde (s32)-I;
                }
                si (in->copia[k] == '\\' && k + I < fin)
                {
                    k = k + I;
                }
                k = k + I;
            }
            si (k >= fin)
            {
                redde (s32)-I;
            }
            *ta          = post + I;
            *tb          = k;
            *descriptio  = VERUM;
            k            = _spatia_caudae(in, k + I, fin);
            si (k < fin && in->copia[k] == ')')
            {
                redde k + I;
            }
        }
    }
    redde (s32)-I;
}

/* Titulus referens '[...]' a i ('['): reddit finem (post ']') aut -I;
 * vacuus '[]' -> *vacuus */
interior s32
_titulus_referens (
     MdInl* in,
       s32  i,
       b32* vacuus)
{
    s32 k = i + I;

    *vacuus = FALSUM;
    dum (k < in->mensura && in->copia[k] != ']' && (k - i) < (s32)1000)
    {
        si (in->copia[k] == '\\' && k + I < in->mensura)
        {
            k = k + I;
        }
        alioquin si (in->copia[k] == '[')
        {
            redde (s32)-I;
        }
        k = k + I;
    }
    si (k >= in->mensura || in->copia[k] != ']')
    {
        redde (s32)-I;
    }
    *vacuus = (b32)(k == i + I);
    redde k + I;
}

interior MdUncus*
_uncum_impellere (
     MdInl* in,
    MdItem* it,
       b32  imago)
{
    MdUncus* u = (MdUncus*)piscina_allocare(in->c->piscina,
        (memoriae_index)magnitudo(MdUncus));

    si (u == NIHIL)
    {
        redde NIHIL;
    }
    u->item           = it;
    u->imago          = imago;
    u->activus        = VERUM;
    u->delim_ante     = in->delim_summus;
    u->prior          = in->uncus_summus;
    in->uncus_summus  = u;
    redde u;
}

/* ']' a i: nexum/imaginem claudere. Reddit finem aut -I (litterae). */
interior s32
_uncum_claudere (
     MdInl* in,
       s32  i)
{
         MdUncus* u = in->uncus_summus;
          MdItem* apertum;
             s32  fin         = (s32)-I;
             s32  da          = ZEPHYRUM;
             s32  db          = ZEPHYRUM;
             s32  ta          = ZEPHYRUM;
             s32  tb          = ZEPHYRUM;
             b32  descriptio  = FALSUM;
    MateriaNodus* definitio   = NIHIL;
             b32  inlineus    = FALSUM;

    si (u == NIHIL)
    {
        redde (s32)-I;
    }
    in->uncus_summus = u->prior;
    si (!u->activus)
    {
        redde (s32)-I;
    }
    apertum = u->item;
    si (i + I < in->mensura && in->copia[i + I] == '(')
    {
        fin = _cauda_inlinea(in, i + I, &da, &db, &ta, &tb,
            &descriptio);
        si (fin >= ZEPHYRUM)
        {
            inlineus = VERUM;
        }
    }
    si (!inlineus)
    {
           b32 vacuus   = FALSUM;
           s32 ref_fin  = (s32)-I;
        chorda clavis;

        si (i + I < in->mensura && in->copia[i + I] == '[')
        {
            ref_fin = _titulus_referens(in, i + I, &vacuus);
        }
        si (ref_fin >= ZEPHYRUM && !vacuus)
        {
            clavis = md_clavem_normalizare(in->c->piscina, in->copia, i
                + II,
                ref_fin - I);
            fin = ref_fin;
        }
        alioquin
        {
            clavis = md_clavem_normalizare(in->c->piscina, in->copia,
                apertum->ad, i);
            fin = (ref_fin >= ZEPHYRUM && vacuus) ? ref_fin : i + I;
        }
        si (   clavis.datum != NIHIL && clavis.mensura > ZEPHYRUM
            && in->c->definitio_quaerere != NIHIL)
        {
            definitio = in->c->definitio_quaerere(in->c->datum, clavis);
        }
        si (definitio == NIHIL)
        {
            redde (s32)-I;
        }
    }
    {
        MateriaNodus* nx = _nodus(in,
            u->imago ? MD_GENUS_IMAGO : MD_GENUS_NEXUS);
              MdItem* novum;
              MdItem* a;
              MdItem* b;

        si (nx == NIHIL)
        {
            redde (s32)-II;
        }
        /* emphasis intra textum nexus - lista mutatur, ergo a/b POST */
        si (!_emphasin_processare(in, u->delim_ante))
        {
            redde (s32)-II;
        }
        a = apertum->proximus;
        b = in->ultimus;
        si (   !materia_nodus_ponere(nx, (i32)MD_NEXUS_FORMA,
                materia_valor_index(inlineus ? (s32)MD_NEXUS_INLINEUS
                                             : (s32)MD_NEXUS_REFERENS),
                MATERIA_LOCUS_INDEX)
            || !materia_nodus_ponere(nx, (i32)MD_NEXUS_APERTUM,
                   materia_valor_token(_lexema(in,
                   (s32)MD_LEX_NEXUS_APERTUM,
                       apertum->ab, apertum->ad)), MATERIA_LOCUS_TOKEN))
        {
            redde (s32)-II;
        }
                si (   a != NIHIL && b != apertum
                    && !_liberos_construere(in, nx,
                    (i32)MD_NEXUS_LIBERI, a, b))
                {
            redde (s32)-II;
                }
        si (u->imago)
        {
            /* alt = textus planus liberorum (B2.1 md, derivatum) */
            ChordaAedificator* aed = chorda_aedificator_creare(
                in->c->piscina, LXIV);

            si (aed == NIHIL)
            {
                redde (s32)-II;
            }
            _liberos_planos(aed, &nx->loci[MD_NEXUS_LIBERI]);
            si (!materia_nodus_ponere(nx, (i32)MD_NEXUS_ALT,
                    materia_valor_token(_derivatum(in,
                        chorda_aedificator_finire(aed), apertum->ab)),
                    MATERIA_LOCUS_TOKEN))
            {
                redde (s32)-II;
            }
        }
        si (!_lexemata_inter(in, nx, (i32)MD_NEXUS_CAUDA,
            (s32)MD_LEX_NEXUS_CAUDA, i, fin))
        {
            redde (s32)-II;
        }
        si (inlineus)
        {
               b32 mutatus;
            chorda url = md_decoquere(in->c->piscina, in->copia + da,
                (i32)(db - da),
                &mutatus);

            si (!materia_nodus_ponere(nx, (i32)MD_NEXUS_URL,
                materia_valor_token(_derivatum(in,
                md_url_codificare(in->c->piscina, url), da)),
                MATERIA_LOCUS_TOKEN))
            {
                redde (s32)-II;
            }
            si (descriptio)
            {
                chorda t = md_decoquere(in->c->piscina, in->copia + ta,
                    (i32)(tb - ta),
                    &mutatus);

                si (!materia_nodus_ponere(nx, (i32)MD_NEXUS_DESCRIPTIO,
                        materia_valor_token(_derivatum(in, t, ta)),
                        MATERIA_LOCUS_TOKEN))
                {
                    redde (s32)-II;
                }
            }
        }
        alioquin
        {
            constans MateriaValor* dv =
                &definitio->loci[MD_DEFINITIO_URL];
            constans MateriaValor* dd =
                &definitio->loci[MD_DEFINITIO_DESCRIPTIO];

            si (   dv->genus == MATERIA_VALOR_TOKEN
                && !materia_nodus_ponere(nx, (i32)MD_NEXUS_URL,
                       materia_valor_token(_derivatum(in,
                       dv->datum.token->valor,
                           apertum->ab)), MATERIA_LOCUS_TOKEN))
            {
                redde (s32)-II;
            }
            si (   dd->genus == MATERIA_VALOR_TOKEN
                && !materia_nodus_ponere(nx, (i32)MD_NEXUS_DESCRIPTIO,
                       materia_valor_token(_derivatum(in,
                       dd->datum.token->valor,
                           apertum->ab)), MATERIA_LOCUS_TOKEN))
            {
                redde (s32)-II;
            }
        }
        novum = _item_novus(in, ITEM_NODUS);
        si (novum == NIHIL)
        {
            redde (s32)-II;
        }
        novum->nodus = nx;
        _substituere(in, apertum, in->ultimus, novum);
        si (!u->imago)
        {
            MdUncus* v = in->uncus_summus;

            dum (v != NIHIL)
            {
                si (!v->imago)
                {
                    v->activus = FALSUM;
                }
                v = v->prior;
            }
        }
        redde fin;
    }
}

/* Textum pendentem [ab, usque) ut item TEXTUS effundere */
interior b32
_textum_effundere (
     MdInl* in,
       s32  ab,
       s32  usque)
{
    si (usque <= ab)
    {
        redde VERUM;
    }
    redde (b32)(_lexema_item(in, (s32)MD_LEX_TEXTUS, ab, usque, FALSUM)
        != NIHIL);
}

interior b32
_scandere (
    MdInl* in)
{
    s32 i          = ZEPHYRUM;
    s32 textus_ab  = ZEPHYRUM;

    dum (i < in->mensura)
    {
        character c = in->copia[i];
              s32 r;

        commutatio (c)
        {
        casus '\n':
        {
               s32  sp = i;
            MdItem* fr;

            dum (sp > textus_ab && in->copia[sp - I] == ' ')
            {
                sp = sp - I;
            }
            fr = _item_novus(in, ITEM_FRACTURA);
            si (fr == NIHIL)
            {
                redde FALSUM;
            }
            fr->segmentum = _segmentum(in, i);
            si (i - sp >= II)
            {
                /* fractura dura: spatia finalia signum eius (possessor unus) */
                si (!_textum_effundere(in, textus_ab, sp))
                {
                    redde FALSUM;
                }
                fr->dura       = VERUM;
                fr->signum_ab  = sp;
                fr->signum_ad  = i;
            }
            alioquin si (!_textum_effundere(in, textus_ab, i))
            {
                redde FALSUM;
            }
            _item_appendere(in, fr);
            i          = i + I;
            textus_ab  = i;
            frange;
        }
        casus '\\':
            si (i + I < in->mensura && in->copia[i + I] == '\n')
            {
                MdItem* fr;

                si (!_textum_effundere(in, textus_ab, i))
                {
                    redde FALSUM;
                }
                fr = _item_novus(in, ITEM_FRACTURA);
                si (fr == NIHIL)
                {
                    redde FALSUM;
                }
                fr->segmentum  = _segmentum(in, i + I);
                fr->dura       = VERUM;
                fr->signum_ab  = i;
                fr->signum_ad  = i + I;
                _item_appendere(in, fr);
                i          = i + II;
                textus_ab  = i;
                frange;
            }
            si (i + I < in->mensura && _interpunctio(in->copia[i + I]))
            {
                si (   !_textum_effundere(in, textus_ab, i)
                    || _lexema_item(in, (s32)MD_LEX_EFFUGIUM, i, i + II,
                    VERUM) == NIHIL)
                {
                    redde FALSUM;
                }
                i          = i + II;
                textus_ab  = i;
                frange;
            }
            i = i + I;
            frange;
        casus '&':
        {
            character e[VIII];
                  i32 l;
                  s32 post;

            si (md_ens_decoquere(in->copia, i, in->mensura, e, &l,
                &post))
            {
                si (   !_textum_effundere(in, textus_ab, i)
                    || _lexema_item(in, (s32)MD_LEX_ENS, i, post, VERUM)
                        == NIHIL)
                {
                    redde FALSUM;
                }
                i          = post;
                textus_ab  = i;
                frange;
            }
            i = i + I;
            frange;
        }
        casus '`':
            si (!_textum_effundere(in, textus_ab, i))
            {
                redde FALSUM;
            }
            r = _verbatim(in, i);
            si (r == (s32)-II)
            {
                redde FALSUM;
            }
            si (r < ZEPHYRUM)
            {
                s32 j = i;

                dum (j < in->mensura && in->copia[j] == '`')
                {
                    j = j + I;
                }
                si (_lexema_item(in, (s32)MD_LEX_TEXTUS, i, j, FALSUM)
                    == NIHIL)
                {
                    redde FALSUM;
                }
                i = j;
            }
            alioquin
            {
                i = r;
            }
            textus_ab = i;
            frange;
        casus '*':
        casus '_':
        casus '~':
            si (!_textum_effundere(in, textus_ab, i))
            {
                redde FALSUM;
            }
            r = _cursus_delimitatorum(in, i);
            si (r < ZEPHYRUM)
            {
                redde FALSUM;
            }
            i = r;
            textus_ab = i;
            frange;
        casus '!':
            si (i + I < in->mensura && in->copia[i + I] == '[')
            {
                MdItem* it;

                si (!_textum_effundere(in, textus_ab, i))
                {
                    redde FALSUM;
                }
                it = _lexema_item(in, (s32)MD_LEX_NEXUS_APERTUM, i, i
                    + II, FALSUM);
                si (   it                              == NIHIL
                    || _uncum_impellere(in, it, VERUM) == NIHIL)
                {
                    redde FALSUM;
                }
                i          = i + II;
                textus_ab  = i;
                frange;
            }
            i = i + I;
            frange;
        casus '[':
        {
            MdItem* it;

            si (!_textum_effundere(in, textus_ab, i))
            {
                redde FALSUM;
            }
            it = _lexema_item(in, (s32)MD_LEX_NEXUS_APERTUM, i, i + I,
                FALSUM);
            si (   it                               == NIHIL
                || _uncum_impellere(in, it, FALSUM) == NIHIL)
            {
                redde FALSUM;
            }
            i          = i + I;
            textus_ab  = i;
            frange;
        }
        casus ']':
            si (!_textum_effundere(in, textus_ab, i))
            {
                redde FALSUM;
            }
            r = _uncum_claudere(in, i);
            si (r == (s32)-II)
            {
                redde FALSUM;
            }
            si (r < ZEPHYRUM)
            {
                si (_lexema_item(in, (s32)MD_LEX_TEXTUS, i, i + I,
                    FALSUM) == NIHIL)
                {
                    redde FALSUM;
                }
                i = i + I;
            }
            alioquin
            {
                i = r;
            }
            textus_ab = i;
            frange;
        casus '<':
            si (!_textum_effundere(in, textus_ab, i))
            {
                redde FALSUM;
            }
            r = _angulus(in, i);
            si (r == (s32)-II)
            {
                redde FALSUM;
            }
            si (r < ZEPHYRUM)
            {
                si (_lexema_item(in, (s32)MD_LEX_TEXTUS, i, i + I,
                    FALSUM) == NIHIL)
                {
                    redde FALSUM;
                }
                i = i + I;
            }
            alioquin
            {
                i = r;
            }
            textus_ab = i;
            frange;
        casus 'w':
        casus 'h':
            r = _autonexus_nudus(in, i);
            si (r == (s32)-II)
            {
                redde FALSUM;
            }
            si (r > ZEPHYRUM)
            {
                /* nodus iam appensus: textum pendens ANTE eum */
                MdItem* nx = in->ultimus;

                _excidere(in, nx);
                si (!_textum_effundere(in, textus_ab, i))
                {
                    redde FALSUM;
                }
                _item_appendere(in, nx);
                i          = r;
                textus_ab  = i;
                frange;
            }
            i = i + I;
            frange;
        ordinarius:
            i = i + I;
            frange;
        }
    }
    /* spatia finalia lineae ultimae: item decoctum (valor ea omittit) */
    {
        s32 sp = in->mensura;

        dum (sp > textus_ab && in->copia[sp - I] == ' ')
        {
            sp = sp - I;
        }
        si (!_textum_effundere(in, textus_ab, sp))
        {
            redde FALSUM;
        }
        si (   sp < in->mensura
            && _lexema_item(in, (s32)MD_LEX_SPATIA_FINALIA, sp,
            in->mensura, VERUM) == NIHIL)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}


/* ==================================================
 * I. Ingressus
 * ================================================== */

b32
md_inlinea_construere (
    MdInlineaContextus* c,
          MateriaNodus* inlinea)
{
           MdInl in;
    MateriaValor lv = inlinea->loci[MD_INLINEA_LIBERI];
             i32 n = (lv.genus
                 == MATERIA_VALOR_LISTA) ? materia_valor_lista_numerus(lv) : ZEPHYRUM;
             i32 k;
             s32 mensura = ZEPHYRUM;

    si (n == ZEPHYRUM)
    {
        redde VERUM;
    }
    memset(&in, ZEPHYRUM, magnitudo(in));
    in.c           = c;
    in.n_segmenta  = (n + I) / II;
    in.segmenta = (MdSegmentum*)piscina_allocare(c->piscina,
        (memoriae_index)in.n_segmenta * (memoriae_index)magnitudo(MdSegmentum));
    si (in.segmenta == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < in.n_segmenta; k++)
    {
        MateriaNodus* t = materia_valor_lista_obtinere(lv,
            k * II)->datum.nodus;
        MateriaToken* tok =
            materia_valor_lista_obtinere(t->loci[MD_TEXTUS_CRUDUM],
            ZEPHYRUM)->datum.token;
        MdSegmentum* s = &in.segmenta[k];

        s->copia_ab  = mensura;
        s->copia_ad  = mensura + (s32)tok->valor.mensura;
        s->fons_ab   = tok->byte_offset;
        s->linea     = tok->linea - I;
        s->fractura = (k * II + I < n)
            ? materia_valor_lista_obtinere(lv, k * II
                + I)->datum.nodus : NIHIL;
        mensura = s->copia_ad + I;   /* '\n' */
    }
    mensura = mensura - I;
    in.copia = (character*)piscina_allocare(c->piscina,
        (memoriae_index)mensura + I);
    si (in.copia == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < in.n_segmenta; k++)
    {
        MdSegmentum* s = &in.segmenta[k];

        memcpy(in.copia + s->copia_ab, c->fons + s->fons_ab,
            (size_t)(s->copia_ad - s->copia_ab));
        si (k + I < in.n_segmenta)
        {
            in.copia[s->copia_ad] = '\n';
        }
    }
    in.mensura = mensura;

    si (!_scandere(&in))
    {
        redde FALSUM;
    }
    si (!_emphasin_processare(&in, NIHIL))
    {
        redde FALSUM;
    }
    /* liberi novi: prospectus ad ZEPHYRUM, deinde appendere */
    inlinea->loci[MD_INLINEA_LIBERI].datum.lista.mensura = ZEPHYRUM;
    si (   in.primus != NIHIL
        && !_liberos_construere(&in, inlinea, (i32)MD_INLINEA_LIBERI,
        in.primus,
               in.ultimus))
    {
        redde FALSUM;
    }
    redde VERUM;
}
