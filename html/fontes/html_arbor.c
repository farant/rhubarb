/* html_arbor.c - Vide html_arbor.h.
  * H3: documentum, elementa, attributa, contentum; clausura per tag
 * congruentem (litteris neglectis) ad elementum apertum proximum,
 * superiora implicite clausa. H4: elementa vacua et clausurae
 * implicitae per TABULAS (M7), vertice acervi solo.
 *
 * STATUS PENDENS. Lexator tag in lexemata plura scindit ('<div',
 * spatia, attributa, '>'); aedificator ergo tres res 'pendentes'
 * tenet inter lexemata: tag_apertum (elementum cuius tag legitur),
 * clausura (elementum cuius '</x' visum est, '>' exspectans), malum
 * (elementum-malum '>' exspectans). Lexema quod ad tag non pertinet
 * (contentum, FINIS, tag novum) statum pendentem CLAUDIT sine '>' -
 * locus absens, truncatio expressa (spec par. IV.2).
 */

#include "html_arbor.h"
#include "html_adaptare.h"
#include "html_lexicon.h"
#include "html_lexema.h"
#include "html_registrum.h"
#include "materia_arbor.h"
#include "materia_lexicon.h"
#include "materia_token.h"
#include "xar.h"
#include <string.h>


/* ==================================================
 * Status parsurae
 * ================================================== */

/* plana = lexemata unum pro uno, index = positio in eis; acervus =
 * MateriaNodus* aperta, profunditas = numerus eorum; tag_apertum =
 * elementum cuius tag legitur, attributum = attributum eius apertum;
 * clausura = elementum cuius '</x' visum est ('>' exspectat); malum =
 * elementum-malum '>' exspectans. */
nomen structura {
                 Piscina* piscina;
     MateriaLexiconRatum  lexicon;
                     Xar* plana;
                     i32  index;
             HtmlLigator  ligator;
            MateriaNodus* documentum;
                     Xar* acervus;
                     i32  profunditas;
            MateriaNodus* tag_apertum;
            MateriaNodus* attributum;
            MateriaNodus* clausura;
            MateriaNodus* malum;
} HtmlParsura;


/* Lexema significans proximum: trivia (intra tag) cumulantur et ad
 * significans proximum ligantur. NIHIL = fluxus exhaustus (post
 * FINIS, quod html_lexare ultimum promittit). */
interior MateriaToken*
_significans_proximum (
    HtmlParsura* p)
{
    dum (p->index < xar_numerus(p->plana))
    {
        MateriaToken* token;

        token     = *(MateriaToken**)xar_obtinere(p->plana, p->index);
        p->index  = p->index + I;

        si (materia_lexicon_trivium_est(&p->lexicon, token->genus))
        {
            si (!html_ligator_cumulare(&p->ligator, token))
            {
                redde NIHIL;
            }
            perge;
        }
        si (!html_ligator_solvere(&p->ligator, token))
        {
            redde NIHIL;
        }
        redde token;
    }
    redde NIHIL;
}


/* ==================================================
 * Acervus apertorum et parens praesens
 * ================================================== */

interior MateriaNodus*
_apertum (
    constans HtmlParsura* p,
                     i32  k)
{
    redde *(MateriaNodus**)xar_obtinere(p->acervus, k);
}

interior MateriaNodus*
_parens (
    constans HtmlParsura* p)
{
    si (p->profunditas > ZEPHYRUM)
    {
        redde _apertum(p, p->profunditas - I);
    }
    redde p->documentum;
}

interior b32
_impellere (
     HtmlParsura* p,
    MateriaNodus* elementum)
{
    si (p->profunditas < xar_numerus(p->acervus))
    {
        si (!xar_ponere(p->acervus, p->profunditas, &elementum))
        {
            redde FALSUM;
        }
    }
    alioquin
    {
        MateriaNodus** locus = (MateriaNodus**)xar_addere(p->acervus);

        si (locus == NIHIL)
        {
            redde FALSUM;
        }
        *locus = elementum;
    }
    p->profunditas = p->profunditas + I;
    redde VERUM;
}

/* Nodum in liberos parentis praesentis appendere. Lista in primo
 * appendendo nascitur: locus absens = lista vacua (lex md). */
interior b32
_liberum_appendere (
     HtmlParsura* p,
    MateriaNodus* nodus)
{
    MateriaNodus* parens = _parens(p);
             i32  locus;

    locus = (parens->genus == (s32)HTML_GENUS_DOCUMENTUM)
          ? (i32)HTML_DOCUMENTUM_LIBERI
          : (i32)HTML_ELEMENTUM_LIBERI;
    redde materia_nodus_appendere(p->piscina, parens, locus,
        materia_valor_nodus(nodus), MATERIA_LOCUS_LISTA_NODUS);
}


/* ==================================================
 * Nodi
 * ================================================== */

/* Folium contenti: genus unius loci 'tok' (index ZEPHYRUM in omni
 * genere contenti - registrum id asserit per titulos). */
interior MateriaNodus*
_folium (
     HtmlParsura* p,
             s32  genus,
    MateriaToken* token)
{
    MateriaNodus* nodus;

    nodus = materia_nodus_creare(p->piscina, genus, (i32)I);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }
    si (!materia_nodus_ponere(nodus, ZEPHYRUM,
            materia_valor_token(token), MATERIA_LOCUS_TOKEN))
    {
        redde NIHIL;
    }
    redde nodus;
}

/* Genus nodi contenti ex genere lexematis; -I si lexema contentum
 * non est (tag, attributum, FINIS). */
interior s32
_genus_contenti (
    s32 genus_lexematis)
{
    commutatio ((HtmlLexemaGenus)genus_lexematis)
    {
        casus HTML_LEX_TEXTUS:
            redde (s32)HTML_GENUS_TEXTUS;
        casus HTML_LEX_REFERENTIA:
            redde (s32)HTML_GENUS_REFERENTIA;
        casus HTML_LEX_TEXTUS_CRUDUS:
        casus HTML_LEX_TEXTUS_CRUDUS_IMPERFECTUS:
            redde (s32)HTML_GENUS_TEXTUS_CRUDUS;
        casus HTML_LEX_COMMENTARIUM:
        casus HTML_LEX_COMMENTARIUM_IMPERFECTUM:
        casus HTML_LEX_COMMENTARIUM_PRAVUM:
            redde (s32)HTML_GENUS_COMMENTARIUM;
        casus HTML_LEX_DOCTYPE:
        casus HTML_LEX_DOCTYPE_IMPERFECTUM:
            redde (s32)HTML_GENUS_DOCTYPE;
        casus HTML_LEX_CDATA:
        casus HTML_LEX_CDATA_IMPERFECTA:
            redde (s32)HTML_GENUS_CDATA;
        ordinarius:
            redde (s32)-I;
    }
}

/* Lexema in elementum-malum pendens addere (creatum si abest et
 * parenti appensum). Vocans statum 'malum' claudit cum malum
 * perfectum est. */
interior b32
_malum_addere (
     HtmlParsura* p,
    MateriaToken* token)
{
    si (p->malum == NIHIL)
    {
        p->malum = materia_nodus_creare(p->piscina,
            (s32)HTML_GENUS_ELEMENTUM_MALUM, (i32)I);
        si (p->malum == NIHIL)
        {
            redde FALSUM;
        }
        si (!_liberum_appendere(p, p->malum))
        {
            redde FALSUM;
        }
    }
    redde materia_nodus_appendere(p->piscina, p->malum,
        (i32)HTML_MALUM_TOKENS, materia_valor_token(token),
        MATERIA_LOCUS_LISTA_TOKEN);
}

/* Attributum novum in tag apertum. */
interior MateriaNodus*
_attributum_novum (
    HtmlParsura* p)
{
    MateriaNodus* attributum;

    attributum = materia_nodus_creare(p->piscina,
        (s32)HTML_GENUS_ATTRIBUTUM, (i32)III);
    si (attributum == NIHIL)
    {
        redde NIHIL;
    }
    si (!materia_nodus_appendere(p->piscina, p->tag_apertum,
            (i32)HTML_ELEMENTUM_ATTRIBUTA,
            materia_valor_nodus(attributum), MATERIA_LOCUS_LISTA_NODUS))
    {
        redde NIHIL;
    }
    p->attributum = attributum;
    redde attributum;
}

interior b32
_locus_scriptus (
    constans MateriaNodus* nodus,
                      i32  locus)
{
    redde (b32)(nodus->loci[locus].genus != MATERIA_VALOR_NIHIL);
}

/* Statum pendentem CLAUDERE: lexema advenit quod ad tag pendens non
 * pertinet ('>' numquam venit - EOF aut tag novum). Loci absentes
 * truncationem exprimunt. */
interior vacuum
_pendentia_claudere (
    HtmlParsura* p)
{
    p->tag_apertum  = NIHIL;
    p->attributum   = NIHIL;
    p->clausura     = NIHIL;
    p->malum        = NIHIL;
}


/* ==================================================
 * Tituli tagorum - litteris neglectis (H1)
 * ================================================== */

/* Titulus sine '<' aut '</'. */
interior chorda
_tag_titulus (
    constans MateriaToken* token)
{
    chorda titulus   = token->valor;
       i32 praefixum;

    praefixum = (token->genus == (s32)HTML_LEX_TAG_CLAUSURA)
              ? (i32)II : (i32)I;
    si (titulus.mensura >= praefixum)
    {
        titulus.datum    = titulus.datum + praefixum;
        titulus.mensura  = titulus.mensura - praefixum;
    }
    redde titulus;
}

interior b32
_tituli_pares (
    chorda a,
    chorda b)
{
    i32 i;

    si (a.mensura != b.mensura)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < a.mensura; i++)
    {
        i8 x = a.datum[i];
        i8 y = b.datum[i];

        si (x >= 'A' && x <= 'Z')
        {
            x = (i8)(x + ('a' - 'A'));
        }
        si (y >= 'A' && y <= 'Z')
        {
            y = (i8)(y + ('a' - 'A'));
        }
        si (x != y)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* Index in acervo elementi aperti PROXIMI cuius titulus congruit;
 * -I si nullum. */
interior s32
_apertum_invenire (
    constans HtmlParsura* p,
                  chorda  titulus)
{
    i32 k;

    per (k = p->profunditas; k > ZEPHYRUM; k--)
    {
        MateriaNodus* elementum = _apertum(p, k - I);
        MateriaToken* apertura;

        apertura = elementum->loci[HTML_ELEMENTUM_TOK_APERTURA]
            .datum.token;
        si (   apertura != NIHIL
            && _tituli_pares(_tag_titulus(apertura), titulus))
        {
            redde (s32)(k - I);
        }
    }
    redde (s32)-I;
}


/* ==================================================
 * Tabulae (M7): elementa vacua, clausurae implicitae
 * ================================================== */

#define TABULAE_NUMERUS(t) ((i32)(magnitudo(t) / magnitudo((t)[0])))

/* WHATWG, XIII: numquam aperiuntur - loci clausurae absentes per
 * constructionem. '</br>' sine pari malum manet (HTML5 id in '<br>'
 * vertit - deviatio nominata, spec par. XI.3). */
hic_manens constans character* constans VACUA[] = {
    "area", "base", "br", "col", "embed", "hr", "img", "input",
    "link", "meta", "source", "track", "wbr"
};

/* Clausurae implicitae (spec par. XI.7): elementum apertum in
 * VERTICE acervi clauditur cum tag apertionis nominatum advenit,
 * iterum dum vertex novus quoque clauditur. Vertex solus
 * (aedificator simplex, H1): '<p><b>x<div>' p NON claudit quia b in
 * vertice est - algorithmus plenus HTML5 scopum 'button' quaereret
 * (mechanismus I, spec par. VI.1). */
hic_manens constans character* constans CLAUDENTIA_PARAGRAPHI[] = {
    "address", "article", "aside", "blockquote", "details", "div",
    "dl", "fieldset", "figcaption", "figure", "footer", "form", "h1",
    "h2", "h3", "h4", "h5", "h6", "header", "hgroup", "hr", "main",
    "menu", "nav", "ol", "p", "pre", "section", "table", "ul"
};
hic_manens constans character* constans CLAUDENTIA_MEMBRI[] = {
    "li"
};
hic_manens constans character* constans CLAUDENTIA_DEFINITIONIS[] = {
    "dt", "dd"
};
hic_manens constans character* constans CLAUDENTIA_OPTIONIS[] = {
    "option", "optgroup"
};
hic_manens constans character* constans CLAUDENTIA_GREGIS[] = {
    "optgroup"
};
hic_manens constans character* constans CLAUDENTIA_ORDINIS[] = {
    "tr", "tbody", "thead", "tfoot"
};
hic_manens constans character* constans CLAUDENTIA_CELLAE[] = {
    "td", "th", "tr", "tbody", "thead", "tfoot"
};
hic_manens constans character* constans CLAUDENTIA_SECTIONIS[] = {
    "tbody", "thead", "tfoot"
};

nomen structura {
              constans character* apertum;
    constans character* constans*  claudentia;
                             i32 numerus;
} ClausuraImplicita;

hic_manens constans ClausuraImplicita CLAUSURAE_IMPLICITAE[] = {
    { "p",        CLAUDENTIA_PARAGRAPHI,
        TABULAE_NUMERUS(CLAUDENTIA_PARAGRAPHI) },
    { "li",       CLAUDENTIA_MEMBRI,
        TABULAE_NUMERUS(CLAUDENTIA_MEMBRI) },
    { "dt",       CLAUDENTIA_DEFINITIONIS,
        TABULAE_NUMERUS(CLAUDENTIA_DEFINITIONIS) },
    { "dd",       CLAUDENTIA_DEFINITIONIS,
        TABULAE_NUMERUS(CLAUDENTIA_DEFINITIONIS) },
    { "option",   CLAUDENTIA_OPTIONIS,
        TABULAE_NUMERUS(CLAUDENTIA_OPTIONIS) },
    { "optgroup", CLAUDENTIA_GREGIS,
        TABULAE_NUMERUS(CLAUDENTIA_GREGIS) },
    { "tr",       CLAUDENTIA_ORDINIS,
        TABULAE_NUMERUS(CLAUDENTIA_ORDINIS) },
    { "td",       CLAUDENTIA_CELLAE,
        TABULAE_NUMERUS(CLAUDENTIA_CELLAE) },
    { "th",       CLAUDENTIA_CELLAE,
        TABULAE_NUMERUS(CLAUDENTIA_CELLAE) },
    { "thead",    CLAUDENTIA_SECTIONIS,
        TABULAE_NUMERUS(CLAUDENTIA_SECTIONIS) },
    { "tbody",    CLAUDENTIA_SECTIONIS,
        TABULAE_NUMERUS(CLAUDENTIA_SECTIONIS) },
    { "tfoot",    CLAUDENTIA_SECTIONIS,
        TABULAE_NUMERUS(CLAUDENTIA_SECTIONIS) }
};

/* Titulus tagi (litteris neglectis) == litterae tabulae? */
interior b32
_titulus_est (
                 chorda  titulus,
     constans character* litterae)
{
    i32 mensura = (i32)strlen(litterae);
    i32 i;

    si (titulus.mensura != mensura)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < mensura; i++)
    {
        i8 x = titulus.datum[i];

        si (x >= 'A' && x <= 'Z')
        {
            x = (i8)(x + ('a' - 'A'));
        }
        si ((character)x != litterae[i])
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior b32
_in_tabula (
                          chorda  titulus,
    constans character* constans* tabula,
                             i32  numerus)
{
    i32 k;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        si (_titulus_est(titulus, tabula[k]))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior b32
_vacuum_est (
    chorda titulus)
{
    redde _in_tabula(titulus, VACUA, TABULAE_NUMERUS(VACUA));
}

/* An tag apertionis 'novum' elementum apertum 'apertum' implicite
 * claudat. */
interior b32
_claudit (
    chorda apertum,
    chorda novum)
{
    i32 k;

    per (k = ZEPHYRUM; k < TABULAE_NUMERUS(CLAUSURAE_IMPLICITAE); k++)
    {
        constans ClausuraImplicita* regula = &CLAUSURAE_IMPLICITAE[k];

        si (_titulus_est(apertum, regula->apertum))
        {
            redde _in_tabula(novum, regula->claudentia,
                regula->numerus);
        }
    }
    redde FALSUM;
}


/* ==================================================
 * Lexemata singula
 * ================================================== */

interior b32
_aperturam_tractare (
     HtmlParsura* p,
    MateriaToken* token)
{
    MateriaNodus* elementum;
          chorda  titulus;

    _pendentia_claudere(p);
    titulus = _tag_titulus(token);

    /* clausurae implicitae: vertex acervi dum tabula id iubet */
    dum (p->profunditas > ZEPHYRUM)
    {
        MateriaNodus* vertex = _apertum(p, p->profunditas - I);
        MateriaToken* apertura;

        apertura =
            vertex->loci[HTML_ELEMENTUM_TOK_APERTURA].datum.token;
        si (   apertura == NIHIL
            || !_claudit(_tag_titulus(apertura), titulus))
        {
            frange;
        }
        p->profunditas = p->profunditas - I;
    }

    elementum = materia_nodus_creare(p->piscina,
        (s32)HTML_GENUS_ELEMENTUM, (i32)VI);
    si (elementum == NIHIL)
    {
        redde FALSUM;
    }
    si (!materia_nodus_ponere(elementum,
        (i32)HTML_ELEMENTUM_TOK_APERTURA,
            materia_valor_token(token), MATERIA_LOCUS_TOKEN))
    {
        redde FALSUM;
    }
    si (!_liberum_appendere(p, elementum))
    {
        redde FALSUM;
    }
    /* vacuum numquam aperitur: non impellitur; tag eius tamen
     * pendens manet (attributa, '>') */
    si (!_vacuum_est(titulus))
    {
        si (!_impellere(p, elementum))
        {
            redde FALSUM;
        }
    }
    p->tag_apertum = elementum;
    redde VERUM;
}

interior b32
_titulum_attributi_tractare (
     HtmlParsura* p,
    MateriaToken* token)
{
    MateriaNodus* attributum;

    si (p->tag_apertum == NIHIL)
    {
        /* Tag clausurae cum attributis ('</h3 x=y>': '>' mutatum aut
         * absens - porta totalitatis id primo cursu invenit): clausura
         * pendens CLAUDITUR (finis absens) et lexemata in malum UNUM
         * pendens eunt, quod '>' sequens accipit. Sine clausura
         * clausa '>' serius elemento iam clauso adhaereret et ANTE
         * lexemata media emitteretur - ordo octetorum fractus. */
        p->clausura = NIHIL;
        redde _malum_addere(p, token);
    }
    attributum = _attributum_novum(p);
    si (attributum == NIHIL)
    {
        redde FALSUM;
    }
    redde materia_nodus_ponere(attributum,
        (i32)HTML_ATTRIBUTUM_TOK_NOMEN,
        materia_valor_token(token), MATERIA_LOCUS_TOKEN);
}

/* '=' aut valor: attributo aperto si locus eius vacat, aliter
 * attributo NOVO cum lexemate solo (absentia H4-formata: '<a = x>'
 * attributum sine titulo dat, non lexema perditum). */
interior b32
_partem_attributi_tractare (
     HtmlParsura* p,
    MateriaToken* token,
             i32  locus)
{
    si (p->tag_apertum == NIHIL)
    {
        /* ut supra: malum pendens usque ad '>' */
        p->clausura = NIHIL;
        redde _malum_addere(p, token);
    }
    si (   p->attributum == NIHIL
        || _locus_scriptus(p->attributum, locus)
        || _locus_scriptus(p->attributum,
        (i32)HTML_ATTRIBUTUM_TOK_VALOR))
    {
        si (_attributum_novum(p) == NIHIL)
        {
            redde FALSUM;
        }
    }
    redde materia_nodus_ponere(p->attributum, locus,
        materia_valor_token(token), MATERIA_LOCUS_TOKEN);
}

/* '>' aut '/>': clausurae pendenti, deinde malo pendenti, deinde
 * tagi aperto; solivagum malum unius lexematis. '/>' elementum NON
 * claudit (caput). */
interior b32
_finem_tractare (
     HtmlParsura* p,
    MateriaToken* token)
{
    si (p->clausura != NIHIL)
    {
        MateriaNodus* elementum = p->clausura;

        p->clausura = NIHIL;
        redde materia_nodus_ponere(elementum,
            (i32)HTML_ELEMENTUM_TOK_CLAUSURA_FINIS,
            materia_valor_token(token), MATERIA_LOCUS_TOKEN);
    }
    si (p->malum != NIHIL)
    {
        si (!_malum_addere(p, token))
        {
            redde FALSUM;
        }
        p->malum = NIHIL;
        redde VERUM;
    }
    si (p->tag_apertum != NIHIL)
    {
        MateriaNodus* elementum = p->tag_apertum;

        p->tag_apertum  = NIHIL;
        p->attributum   = NIHIL;
        redde materia_nodus_ponere(elementum,
            (i32)HTML_ELEMENTUM_TOK_FINIS,
            materia_valor_token(token), MATERIA_LOCUS_TOKEN);
    }
    si (!_malum_addere(p, token))
    {
        redde FALSUM;
    }
    p->malum = NIHIL;
    redde VERUM;
}

/* '</x': elementum apertum proximum eiusdem tituli claudit,
 * superiora implicite (loci clausurae absentes); sine pari malum
 * ('>' sequens ei additur). */
interior b32
_clausuram_tractare (
     HtmlParsura* p,
    MateriaToken* token)
{
    s32 k;

    _pendentia_claudere(p);
    k = _apertum_invenire(p, _tag_titulus(token));
    si (k < ZEPHYRUM)
    {
        redde _malum_addere(p, token);
    }
    p->clausura     = _apertum(p, (i32)k);
    p->profunditas  = (i32)k;
    redde materia_nodus_ponere(p->clausura,
        (i32)HTML_ELEMENTUM_TOK_CLAUSURA, materia_valor_token(token),
        MATERIA_LOCUS_TOKEN);
}

interior b32
_contentum_tractare (
     HtmlParsura* p,
    MateriaToken* token)
{
    MateriaNodus* folium;
             s32  genus;

    _pendentia_claudere(p);
    genus = _genus_contenti(token->genus);
    si (genus < ZEPHYRUM)
    {
        /* lexema ignotum - tabula nostra deficit; totalitas tamen */
        si (!_malum_addere(p, token))
        {
            redde FALSUM;
        }
        p->malum = NIHIL;
        redde VERUM;
    }
    folium = _folium(p, genus, token);
    si (folium == NIHIL)
    {
        redde FALSUM;
    }
    redde _liberum_appendere(p, folium);
}


/* ==================================================
 * Ingressus
 * ================================================== */

MateriaNodus*
html_arbor_parsare (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura)
{
           HtmlParsura  p;
    MateriaLexIudicium  iudicium;
                   Xar* lexemata;

    si (piscina == NIHIL)
    {
        redde NIHIL;
    }

    /* ZEPHYRUM PRIMUM (vide html_ligator_incipere pro ratione). */
    memset(&p, ZEPHYRUM, magnitudo(HtmlParsura));
    p.piscina = piscina;

    /* PORTA ONERIS, politica degradationis B: recusatio hic vitium
     * TABULAE NOSTRAE nominat, non fontis. */
    si (!materia_lexicon_ratum_facere(&p.lexicon, &HTML_LEXICON,
            &iudicium))
    {
        redde NIHIL;
    }

    lexemata = html_lexare(piscina, fons, mensura);
    si (lexemata == NIHIL)
    {
        redde NIHIL;
    }
    p.plana = html_adaptare(piscina, lexemata, NIHIL);
    si (p.plana == NIHIL)
    {
        redde NIHIL;
    }
    html_ligator_incipere(&p.ligator, piscina, &p.lexicon);

    p.acervus = xar_creare(piscina, magnitudo(MateriaNodus*));
    p.documentum = materia_nodus_creare(piscina,
        (s32)HTML_GENUS_DOCUMENTUM, (i32)II);
    si (p.acervus == NIHIL || p.documentum == NIHIL)
    {
        redde NIHIL;
    }

    per (;;)
    {
        MateriaToken* token;
                 b32  sanum;
                 s32  genus;

        token = _significans_proximum(&p);
        si (token == NIHIL)
        {
            frange;
        }
        genus = token->genus;

        si (materia_lexicon_munus(&p.lexicon, genus)
                == MATERIA_MUNUS_FINIS)
        {
            _pendentia_claudere(&p);
            si (!materia_nodus_ponere(p.documentum,
                    (i32)HTML_DOCUMENTUM_CAUDA,
                    materia_valor_token(token),
                    MATERIA_LOCUS_TOKEN))
            {
                redde NIHIL;
            }
            frange;
        }

        si (genus == (s32)HTML_LEX_TAG_APERTURA)
        {
            sanum = _aperturam_tractare(&p, token);
        }
        alioquin si (genus == (s32)HTML_LEX_ATTRIBUTUM_NOMEN)
        {
            sanum = _titulum_attributi_tractare(&p, token);
        }
        alioquin si (genus == (s32)HTML_LEX_AEQUALE)
        {
            sanum = _partem_attributi_tractare(&p, token,
                (i32)HTML_ATTRIBUTUM_TOK_AEQUALE);
        }
        alioquin si (   genus == (s32)HTML_LEX_ATTRIBUTUM_VALOR
                     || genus
                         == (s32)HTML_LEX_ATTRIBUTUM_VALOR_IMPERFECTUS)
        {
            sanum = _partem_attributi_tractare(&p, token,
                (i32)HTML_ATTRIBUTUM_TOK_VALOR);
        }
        alioquin si (   genus == (s32)HTML_LEX_TAG_FINIS
                     || genus == (s32)HTML_LEX_TAG_FINIS_SOLUS)
        {
            sanum = _finem_tractare(&p, token);
        }
        alioquin si (genus == (s32)HTML_LEX_TAG_CLAUSURA)
        {
            sanum = _clausuram_tractare(&p, token);
        }
        alioquin
        {
            sanum = _contentum_tractare(&p, token);
        }

        si (!sanum)
        {
            redde NIHIL;
        }
    }

    /* Verba ponentia patres iam figunt (2026-09-10); vocatio haec
     * politicam reconstructionis lectori aequat (css exemplar). */
    materia_arbor_patres_figere(piscina, p.documentum);
    redde p.documentum;
}
