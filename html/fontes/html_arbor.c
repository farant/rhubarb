/* html_arbor.c - Vide html_arbor.h.
  * H3: documentum, elementa, attributa, contentum; clausura per tag
 * congruentem (litteris neglectis) ad elementum apertum proximum,
 * superiora implicite clausa. H4: elementa vacua et clausurae
 * implicitae per TABULAS (M7), vertice acervi solo; O2b-5 (2026-09-15):
 * p/li/dd/dt/button per SCOPUM (gradus visibilitatis per gradum
 * acervi, O(I) per lexema - vide _scopi_gradus).
 *
 * STATUS PENDENS. Lexator tag in lexemata plura scindit ('<div',
 * spatia, attributa, '>'); aedificator ergo tres res 'pendentes'
 * tenet inter lexemata: tag_apertum (elementum cuius tag legitur),
 * clausura (elementum cuius '</x' visum est, '>' exspectans), malum
 * (elementum-malum '>' exspectans). Lexema quod ad tag non pertinet
 * (contentum, FINIS, tag novum) statum pendentem CLAUDIT sine '>' -
 * locus absens, truncatio expressa (spec par. IV.2).
 *
 * SYNTHESIS (O7a, 2026-09-15): involucra html/head/body per modos
 * WHATWG 'before html' ... 'after head' (_involucra_fingere) et
 * partes tabulae tbody/tr/colgroup (_partes_tabulae_fingere) nodi
 * FICTI sunt - elementum sine lexematibus, locus 'synthesis' solus.
 * Fragmentum (html_arbor_parsare_fragmentum): contextus vertex sub
 * acervo (basis), involucra iam adsunt.
 *
 * SEDES (O7b, 2026-09-15): nodus cuius parens DOM alius est quam
 * parens octetorum locum 'sedes' (referentia) fert - nodus ubi octeti
 * sunt manet, visio cocta eum sub sede imprimit. Tres fontes: foster
 * parenting (modi tabularum: vertex table/tbody/tfoot/thead/tr, tag
 * aut textus quem modus tabulae non tractat -> sedes = parens
 * tabulae), contentum post '</body>' aut '</html>' (spec 'after
 * body': in body - '</body>' et '</html>' nunc CLAUDUNT, O5 ea mala
 * tenebat), contentum capitis post '</head>' (spec 'after head': in
 * head). Modi tabularum per gradum (modus_tabulae, tabulae,
 * pars_tabulae): tag partis tabulae acervum ad partem proximam
 * purgat, '<table>' in tabula tabulam claudit.
 */

#include "html_arbor.h"
#include "html_adaptare.h"
#include "html_lexicon.h"
#include "html_lexema.h"
#include "html_registrum.h"
#include "html_alienum.h"
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
/* Modi tabularum (WHATWG 'in table' / 'in table body' / 'in row' /
 * 'in cell' / 'in caption' / 'in column group'), O7b: per gradum ex
 * titulo proprio partis tabulae, hereditati, html et template eos
 * tollunt (limites scopi tabulae). NULLUS = 'in body' quoad tabulas. */
nomen enumeratio {
    TABULAE_NULLUS = 0,
    TABULAE_TABULA,
    TABULAE_SECTIO,
    TABULAE_ORDO,
    TABULAE_CELLA,
    TABULAE_INSCRIPTIO,
    TABULAE_COLUMNAE
} HtmlModusTabulae;

/* Scopi visibiles per gradum acervi (O2b-5; vide _scopi_gradus) */
nomen structura {
    s32 p;
    s32 membri;
    s32 definitionis;
    s32 bullae;
    /* spatium proprium et liberorum (O2b-6): eruptio, se-claudens */
    HtmlAlienum proprium;
    HtmlAlienum liberorum;
    /* O5: index select proximi visibilis (-I nullum); intra frameset */
    s32 selectum;
    b32 intra_compagem;
    /* O7a: index template aperti proximi (-I nullum): intra template
     * modi involucrorum tacent (spec 'in template'). O7b: modus
     * tabularum huius gradus, index table apertae proximae (-I nulla;
     * parens eius = parens fovens), titulus proprius pars tabulae est
     * (purgatio acervi hic sistit) */
                 s32 templi;
    HtmlModusTabulae modus_tabulae;
                 s32 tabulae;
                 b32 pars_tabulae;
    /* index partis tabulae VERAE proximae in acervo (-I nulla: modus ex
     * contextu fragmenti solo - spec 'in table scope' tum fallit) */
    s32 partis;
} ScopiGradus;

/* Modi involucrorum (WHATWG 'before html' -> 'before head' -> 'in
 * head' -> 'after head' -> 'in body'/'in frameset' -> 'after body' ->
 * 'after after body'); O7a, post corpus O7b. */
nomen enumeratio {
    MODUS_ANTE_RADICEM = 0,
    MODUS_ANTE_CAPUT,
    MODUS_IN_CAPITE,
    MODUS_POST_CAPUT,
    MODUS_IN_CORPORE,
    MODUS_POST_CORPUS,
    MODUS_POST_RADICEM
} HtmlModus;

nomen structura {
                 Piscina* piscina;
     MateriaLexiconRatum  lexicon;
                     Xar* plana;
                     i32  index;
             HtmlLigator  ligator;
            MateriaNodus* documentum;
                     Xar* acervus;
                     Xar* scopi;
                     i32  profunditas;
            MateriaNodus* tag_apertum;
            MateriaNodus* attributum;
            MateriaNodus* clausura;
            MateriaNodus* malum;
    /* O5 (2026-09-15) - modi neglegendi: lexemata quae DOM nodo non
     * retinet in elementum-malum eunt (octeti manent, visio cocta ea
     * omittit). html/head/body iterata, frameset post contentum,
     * doctype post contentum. compages_licet = 'frameset-ok'. */
                     b32 html_visum;
                     b32 head_visum;
                     b32 body_visum;
                     b32 compages_visa;
                     b32 contentum_visum;
                     b32 compages_licet;
    /* O7a (2026-09-15) - involucra et fragmenta: modus involucrorum;
     * basis = gradus scoporum SUB acervo (contextus fragmenti, aut
     * vacuus in documento); contextus = titulus elementi contextus
     * (vacuus in documento) - vertex acervi vacui. */
               HtmlModus modus;
             ScopiGradus basis;
                  chorda contextus;
                     b32 fragmentum;
    /* O7b: involucra (vera aut ficta; NIHIL si nondum) - sedes
     * contenti capitis post head; corpus pro remotione; radix = html.
     * sedes_posterior = vertex DOM cum '</body>'/'</html>' venit (spec:
     * acervus manet, modus solus mutatur) - sedes contenti posterioris.
     * fovens = parens fovens tagi aperti pendentis (input: type=hidden
     * in tabula manet, decernitur cum attributa lecta sunt) */
            MateriaNodus* caput;
            MateriaNodus* corpus;
            MateriaNodus* radix;
            MateriaNodus* sedes_posterior;
            MateriaNodus* fovens;
    /* post '</body>' contentum non album aut tag venit: spec ad 'in
     * body' redit - commentaria quoque in sedem posteriorem */
                     b32 corpus_iterum;
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

interior ScopiGradus
_scopi_gradus (
    constans HtmlParsura* p,
                     i32  k,
                  chorda  titulus);

interior chorda
_tag_titulus (
    constans MateriaToken* token);

interior chorda
_titulus_elementi (
    constans MateriaNodus* elementum);

/* Elementum (et gradum scoporum eius, ex titulo) impellere - loci
 * acervorum reusi sub profunditate. */
interior b32
_impellere (
     HtmlParsura* p,
    MateriaNodus* elementum,
          chorda  titulus)
{
    ScopiGradus gradus = _scopi_gradus(p, p->profunditas, titulus);

    si (p->profunditas < xar_numerus(p->acervus))
    {
        si (   !xar_ponere(p->acervus, p->profunditas, &elementum)
            || !xar_ponere(p->scopi, p->profunditas, &gradus))
        {
            redde FALSUM;
        }
    }
    alioquin
    {
         MateriaNodus** locus = (MateriaNodus**)xar_addere(p->acervus);
          ScopiGradus*  sedes = (ScopiGradus*)xar_addere(p->scopi);

        si (locus == NIHIL || sedes == NIHIL)
        {
            redde FALSUM;
        }
        *locus = elementum;
        *sedes = gradus;
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

/* Numerus locorum generis ex registro cocto - ne tabula manu scripta
 * a declaratione labatur (O7b: textus et referentia locum sedes
 * ferunt). */
interior i32
_loci_numerus (
    s32 genus)
{
    redde HTML_REGISTRUM.genera[genus].loci_numerus;
}

/* Folium contenti: genus cuius locus primus 'tok' est (index ZEPHYRUM
 * in omni genere contenti - registrum id asserit per titulos). */
interior MateriaNodus*
_folium (
     HtmlParsura* p,
             s32  genus,
    MateriaToken* token)
{
    MateriaNodus* nodus;

    nodus = materia_nodus_creare(p->piscina, genus,
        _loci_numerus(genus));
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

/* Lexema loci TOKEN, NIHIL si absens */
interior MateriaToken*
_tok (
    constans MateriaNodus* nodus,
                      i32  locus)
{
    redde (nodus->loci[locus].genus == MATERIA_VALOR_TOKEN)
        ? nodus->loci[locus].datum.token : NIHIL;
}

/* Statum pendentem CLAUDERE: lexema advenit quod ad tag pendens non
 * pertinet ('>' numquam venit - EOF aut tag novum). Loci absentes
 * truncationem exprimunt. */
interior b32
_fovens_solvere (
    HtmlParsura* p);

interior vacuum
_pendentia_claudere (
    HtmlParsura* p)
{
    /* input in tabula abruptum: fotum (attributa lecta iudicant) */
    _fovens_solvere(p);
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

interior chorda
_vacua (vacuum)
{
    chorda c;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    redde c;
}


/* ==================================================
 * Synthesis (O7a): tituli elementorum fictorum
 * ================================================== */

/* Tabula loci 'synthesis' ordine HtmlSynthesis. SCRIPTIBILIS consulto
 * (non constans), ut chorda (i8*) in eam sine allocatione spectet. */
hic_manens character SYNTHESES[HTML_SYNTHESIS_NUMERUS][IX] = {
    "", "html", "head", "body", "tbody", "tr", "colgroup"
};

constans character*
html_arbor_synthesis_titulus (
    s32 synthesis)
{
    si (   synthesis <= ZEPHYRUM
        || synthesis >= (s32)HTML_SYNTHESIS_NUMERUS)
    {
        redde NIHIL;
    }
    redde SYNTHESES[synthesis];
}

/* Titulus elementi: ex tag apertionis, aut ex loco synthesis
 * (elementum fictum); vacuus si neutrum (tag ad EOF scissum numquam:
 * apertura semper adest, sed defensive). */
interior chorda
_titulus_elementi (
    constans MateriaNodus* elementum)
{
    constans MateriaValor* apertura;
    constans MateriaValor* synthesis;
                   chorda  titulus;

    apertura   = &elementum->loci[HTML_ELEMENTUM_TOK_APERTURA];
    synthesis  = &elementum->loci[HTML_ELEMENTUM_SYNTHESIS];
    si (apertura->genus == MATERIA_VALOR_TOKEN)
    {
        redde _tag_titulus(apertura->datum.token);
    }
    titulus = _vacua();
    si (   synthesis->genus == MATERIA_VALOR_INDEX
        && synthesis->datum.index > ZEPHYRUM
        && synthesis->datum.index < (s32)HTML_SYNTHESIS_NUMERUS)
    {
        titulus.datum = (i8*)SYNTHESES[synthesis->datum.index];
        titulus.mensura =
            (i32)strlen(SYNTHESES[synthesis->datum.index]);
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
        si (_tituli_pares(_titulus_elementi(_apertum(p, k - I)),
                titulus))
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
 * iterum dum vertex novus quoque clauditur. Vertex solus erat lex H1
 * ('<p><b>x<div>' p non claudebat); O2b-5 scopum HTML5 addidit
 * (mechanismus I, spec par. VI.1) ante hanc tabulam: p in scopo
 * 'button', li/dd/dt per ambulationem 'special', button in scopo -
 * per gradus visibilitatis, non per ambulationem (_scopi_gradus).
 * Tabulae ex oraculo html5lib
 * (O2b, 2026-09-15) impletae: tabula p plena WHATWG 'in body'
 * (center dialog dir search summary listing plaintext xmp li dd dt),
 * sectiones tabularum a caption/col/colgroup, annotationes ruby,
 * capita h1-h6 inter se, a/nobr/button/select a se ipsis (claudentia
 * NIHIL = titulus proprius), head et colgroup ab OMNI tag praeter
 * exceptiones (nisi VERUM: tabula = exceptiones). */
hic_manens constans character* constans CLAUDENTIA_PARAGRAPHI[] = {
    "address", "article", "aside", "blockquote", "center", "details",
    "dialog", "dir", "div", "dl", "fieldset", "figcaption", "figure",
    "footer", "form", "h1", "h2", "h3", "h4", "h5", "h6", "header",
    "hgroup", "hr", "main", "menu", "nav", "ol", "p", "pre", "section",
    "table", "ul", "search", "summary", "listing", "plaintext", "xmp",
    "li", "dd", "dt"
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
    "tr", "tbody", "thead", "tfoot", "caption", "col", "colgroup"
};
hic_manens constans character* constans CLAUDENTIA_CELLAE[] = {
    "td", "th", "tr", "tbody", "thead", "tfoot", "caption", "col",
    "colgroup"
};
hic_manens constans character* constans CLAUDENTIA_SECTIONIS[] = {
    "tbody", "thead", "tfoot", "caption", "col", "colgroup"
};
hic_manens constans character* constans CLAUDENTIA_INSCRIPTIONIS[] = {
    "caption", "col", "colgroup", "tbody", "td", "tfoot", "th", "thead",
    "tr"
};
hic_manens constans character* constans CLAUDENTIA_ANNOTATIONIS[] = {
    "rb", "rt", "rtc", "rp"
};
hic_manens constans character* constans CLAUDENTIA_ANNOTATIONUM[] = {
    "rb", "rtc"
};
hic_manens constans character* constans CLAUDENTIA_CAPITUM[] = {
    "h1", "h2", "h3", "h4", "h5", "h6"
};
/* nisi: quae head NON claudunt (WHATWG 'in head') */
hic_manens constans character* constans PERMISSA_CAPITIS[] = {
    "base", "basefont", "bgsound", "link", "meta", "title", "noscript",
    "noframes", "style", "script", "template", "head"
};
/* nisi: quae colgroup NON claudunt (WHATWG 'in column group') */
hic_manens constans character* constans PERMISSA_COLUMNARUM[] = {
    "col", "template"
};

/* claudentia NIHIL = titulus proprius solus; nisi VERUM = claudentia
 * sunt EXCEPTIONES (omne aliud tag claudit). */
nomen structura {
              constans character* apertum;
    constans character* constans*  claudentia;
                             i32 numerus;
                             b32 nisi;
} ClausuraImplicita;

#define REGULA(apertum, tabula) \
    { apertum, tabula, TABULAE_NUMERUS(tabula), FALSUM }
#define REGULA_NISI(apertum, tabula) \
    { apertum, tabula, TABULAE_NUMERUS(tabula), VERUM }
#define REGULA_IPSIUS(apertum) \
    { apertum, NIHIL, ZEPHYRUM, FALSUM }

hic_manens constans ClausuraImplicita CLAUSURAE_IMPLICITAE[] = {
    REGULA("p",        CLAUDENTIA_PARAGRAPHI),
    REGULA("li",       CLAUDENTIA_MEMBRI),
    REGULA("dt",       CLAUDENTIA_DEFINITIONIS),
    REGULA("dd",       CLAUDENTIA_DEFINITIONIS),
    REGULA("option",   CLAUDENTIA_OPTIONIS),
    REGULA("optgroup", CLAUDENTIA_GREGIS),
    REGULA("tr",       CLAUDENTIA_ORDINIS),
    REGULA("td",       CLAUDENTIA_CELLAE),
    REGULA("th",       CLAUDENTIA_CELLAE),
    REGULA("thead",    CLAUDENTIA_SECTIONIS),
    REGULA("tbody",    CLAUDENTIA_SECTIONIS),
    REGULA("tfoot",    CLAUDENTIA_SECTIONIS),
    REGULA("caption",  CLAUDENTIA_INSCRIPTIONIS),
    REGULA("rb",       CLAUDENTIA_ANNOTATIONIS),
    REGULA("rt",       CLAUDENTIA_ANNOTATIONIS),
    REGULA("rp",       CLAUDENTIA_ANNOTATIONIS),
    REGULA("rtc",      CLAUDENTIA_ANNOTATIONUM),
    REGULA("h1",       CLAUDENTIA_CAPITUM),
    REGULA("h2",       CLAUDENTIA_CAPITUM),
    REGULA("h3",       CLAUDENTIA_CAPITUM),
    REGULA("h4",       CLAUDENTIA_CAPITUM),
    REGULA("h5",       CLAUDENTIA_CAPITUM),
    REGULA("h6",       CLAUDENTIA_CAPITUM),
    REGULA_IPSIUS("a"),
    REGULA_IPSIUS("nobr"),
    REGULA_IPSIUS("button"),
    REGULA_IPSIUS("select"),
    REGULA_NISI("head",     PERMISSA_CAPITIS),
    REGULA_NISI("colgroup", PERMISSA_COLUMNARUM)
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

/* Modus tabularum quem titulus proprius dat (NULLUS = non pars) */
interior HtmlModusTabulae
_modus_tabulae_tituli (
    chorda titulus)
{
    si (_titulus_est(titulus, "table"))
    {
        redde TABULAE_TABULA;
    }
    si (   _titulus_est(titulus, "tbody")
        || _titulus_est(titulus, "thead")
        || _titulus_est(titulus, "tfoot"))
    {
        redde TABULAE_SECTIO;
    }
    si (_titulus_est(titulus, "tr"))
    {
        redde TABULAE_ORDO;
    }
    si (_titulus_est(titulus, "td") || _titulus_est(titulus, "th"))
    {
        redde TABULAE_CELLA;
    }
    si (_titulus_est(titulus, "caption"))
    {
        redde TABULAE_INSCRIPTIO;
    }
    si (_titulus_est(titulus, "colgroup"))
    {
        redde TABULAE_COLUMNAE;
    }
    redde TABULAE_NULLUS;
}

/* WHATWG 'in table': vertex quo locus insertionis fovetur (foster
 * parenting) - table, tbody, tfoot, thead, tr */
hic_manens constans character* constans FOVENTIA[] = {
    "table", "tbody", "tfoot", "thead", "tr"
};
/* tags quae modi tabularum ipsi tractant (partes, table, regulae
 * capitis, form, input) - numquam fota */
hic_manens constans character* constans TABULAE_ACCEPTA[] = {
    "caption", "col", "colgroup", "tbody", "tfoot", "thead", "td", "th",
    "tr", "table", "style", "script", "template", "form"
};

/* SCOPUS (WHATWG 'has an element in scope', O2b-5 2026-09-15 - lex
 * verticis solius pro p/li/dd/dt/button relicta). Limites scopi
 * elementum quaesitum CELANT (etiam puncta integrationis MathML/SVG -
 * aedificator spatia nominum nescit, tituli sufficiunt). Scopus
 * 'button' = limites + button. */
hic_manens constans character* constans LIMITES_SCOPI[] = {
    "applet", "caption", "html", "table", "td", "th", "marquee",
    "object", "template", "mi", "mo", "mn", "ms", "mtext",
    "annotation-xml", "foreignObject", "desc", "title"
};

/* WHATWG categoria 'special': ambulatio li/dd/dt hic sistit (praeter
 * address, div, p - spec) */
hic_manens constans character* constans SPECIALIA[] = {
    "address", "applet", "area", "article", "aside", "base", "basefont",
    "bgsound", "blockquote", "body", "br", "button", "caption",
    "center", "col", "colgroup", "dd", "details", "dir", "div", "dl",
    "dt", "embed", "fieldset", "figcaption", "figure", "footer", "form",
    "frame", "frameset", "h1", "h2", "h3", "h4", "h5", "h6", "head",
    "header", "hgroup", "hr", "html", "iframe", "img", "input",
    "keygen", "li", "link", "listing", "main", "marquee", "menu",
    "meta", "nav", "noembed", "noframes", "noscript", "object", "ol",
    "p", "param", "plaintext", "pre", "script", "search", "section",
    "select", "source", "style", "summary", "table", "tbody", "td",
    "template", "textarea", "tfoot", "th", "thead", "title", "tr",
    "track", "ul", "wbr", "xmp", "mi", "mo", "mn", "ms", "mtext",
    "annotation-xml", "foreignObject", "desc"
};

interior b32
_limes_scopi_est (
    chorda titulus)
{
    redde _in_tabula(titulus, LIMITES_SCOPI,
        TABULAE_NUMERUS(LIMITES_SCOPI));
}

/* 'special' praeter address/div/p: ambulationem li/dd/dt sistit */
interior b32
_sistit_membrum (
    chorda titulus)
{
    redde (b32)(_in_tabula(titulus, SPECIALIA,
        TABULAE_NUMERUS(SPECIALIA))
                && !_titulus_est(titulus, "address")
                && !_titulus_est(titulus, "div")
                && !_titulus_est(titulus, "p"));
}

/* Gradus scoporum pro elemento impellendo (index k, titulus eius):
 * quisque campus = index elementi proximi VISIBILIS ab hoc gradu, aut
 * -I. Computatus in impulsu ex gradu inferiore - O(I) per lexema, ut
 * aedificator C milia gradus profundus iterativus et linearis maneat
 * (lex H1): ambulatio acervi per tag quadratica erat, porta
 * totalitatis id primo cursu monuit (CDLXXXIX s, pinnae profunditatis
 * mortuae). */
/* Gradus sub acervo vacuo: nihil visibile (documentum; fragmentum
 * contextum suum superponit - vide _parsare). */
interior ScopiGradus
_basis_vacua (vacuum)
{
    ScopiGradus g;

    g.p               = (s32)-I;
    g.membri          = (s32)-I;
    g.definitionis    = (s32)-I;
    g.bullae          = (s32)-I;
    g.proprium        = HTML_ALIENUM_NULLUM;
    g.liberorum       = HTML_ALIENUM_NULLUM;
    g.selectum        = (s32)-I;
    g.intra_compagem  = FALSUM;
    g.templi          = (s32)-I;
    g.modus_tabulae   = TABULAE_NULLUS;
    g.tabulae         = (s32)-I;
    g.pars_tabulae    = FALSUM;
    g.partis          = (s32)-I;
    redde g;
}

interior ScopiGradus
_scopi_gradus (
    constans HtmlParsura* p,
                     i32  k,
                  chorda  titulus)
{
    ScopiGradus g;
    ScopiGradus infra;

    si (k > ZEPHYRUM)
    {
        infra = *(ScopiGradus*)xar_obtinere(p->scopi, k - I);
    }
    alioquin
    {
        infra = p->basis;   /* contextus fragmenti aut vacuus (O7a) */
    }
    si (_titulus_est(titulus, "p"))
    {
        g.p = (s32)k;
    }
    alioquin si (   _limes_scopi_est(titulus)
                 || _titulus_est(titulus, "button"))
    {
        g.p = (s32)-I;
    }
    alioquin
    {
        g.p = infra.p;
    }
    si (_titulus_est(titulus, "li"))
    {
        g.membri = (s32)k;
    }
    alioquin si (_sistit_membrum(titulus))
    {
        g.membri = (s32)-I;
    }
    alioquin
    {
        g.membri = infra.membri;
    }
    si (_titulus_est(titulus, "dd") || _titulus_est(titulus, "dt"))
    {
        g.definitionis = (s32)k;
    }
    alioquin si (_sistit_membrum(titulus))
    {
        g.definitionis = (s32)-I;
    }
    alioquin
    {
        g.definitionis = infra.definitionis;
    }
    si (_titulus_est(titulus, "button"))
    {
        g.bullae = (s32)k;
    }
    alioquin si (_limes_scopi_est(titulus))
    {
        g.bullae = (s32)-I;
    }
    alioquin
    {
        g.bullae = infra.bullae;
    }
    /* spatia nominum: attributa in impulsu nondum visa (annotation-xml
     * tum MathML) */
    g.proprium  = html_alienum_proprium(infra.proprium, infra.liberorum,
        titulus);
    g.liberorum = html_alienum_liberorum(g.proprium, titulus, NIHIL);
    g.selectum = _titulus_est(titulus,
        "select") ? (s32)k : infra.selectum;
    g.intra_compagem = (b32)(_titulus_est(titulus, "frameset")
                             || infra.intra_compagem);
    g.templi = _titulus_est(titulus,
        "template") ? (s32)k : infra.templi;
    /* O7b: modi tabularum - titulus proprius partis HTML modum dat
     * (elementum alienum 'math tr' numquam), html et template eum
     * tollunt (limites scopi tabulae), ceteri hereditant (elementum
     * fotum in modo tabulae manet: spec modum non mutat) */
    {
        HtmlModusTabulae proprius = (g.proprium == HTML_ALIENUM_NULLUM)
                                  ? _modus_tabulae_tituli(titulus)
                                  : TABULAE_NULLUS;
                     b32 limes = (b32)(_titulus_est(titulus, "html")
                                       || _titulus_est(titulus,
                                           "template"));

        g.pars_tabulae = (b32)(proprius != TABULAE_NULLUS);
        si (proprius != TABULAE_NULLUS)
        {
            g.modus_tabulae  = proprius;
            g.partis         = (s32)k;
        }
        alioquin
        {
            g.modus_tabulae =
                limes ? TABULAE_NULLUS : infra.modus_tabulae;
            g.partis = limes ? (s32)-I : infra.partis;
        }
        si (proprius == TABULAE_TABULA)
        {
            g.tabulae = (s32)k;
        }
        alioquin
        {
            g.tabulae = limes ? (s32)-I : infra.tabulae;
        }
    }
    redde g;
}

/* Clausurae per scopum ante tabulam verticis: li/dd/dt per gradum
 * membri, button in scopo, deinde p in scopo 'button' a claudentibus
 * p. Omnia supra clauduntur locis clausurae absentibus (lex H4). */
interior vacuum
_scopos_claudere (
    HtmlParsura* p,
         chorda  titulus)
{
    constans ScopiGradus* vertex;
                     s32  k = (s32)-I;

    si (p->profunditas == ZEPHYRUM)
    {
        redde;
    }
    vertex = (constans ScopiGradus*)xar_obtinere(p->scopi,
        p->profunditas - I);
    /* ERUPTIO ex contento alieno (WHATWG 'in foreign content'): tag
     * HTML rumpens elementa aliena claudit usque ad elementum cuius
     * liberi HTML sunt (HTML ipsum aut punctum integrationis). In
     * fragmento NUMQUAM (spec 'fragment case': ut tag aliud quodlibet;
     * x/net parse.go 'if !p.fragment' - O7a). */
    si (   !p->fragmentum
        && vertex->liberorum != HTML_ALIENUM_NULLUM
        && html_alienum_rumpit(titulus))
    {
        dum (   p->profunditas > ZEPHYRUM
             && vertex->liberorum != HTML_ALIENUM_NULLUM)
        {
            p->profunditas = p->profunditas - I;
            si (p->profunditas > ZEPHYRUM)
            {
                vertex = (constans ScopiGradus*)xar_obtinere(p->scopi,
                    p->profunditas - I);
            }
        }
        si (p->profunditas == ZEPHYRUM)
        {
            redde;
        }
    }
    si (_titulus_est(titulus, "li"))
    {
        k = vertex->membri;
    }
    alioquin si (   _titulus_est(titulus, "dd")
                 || _titulus_est(titulus, "dt"))
    {
        k = vertex->definitionis;
    }
    alioquin si (_titulus_est(titulus, "button"))
    {
        k = vertex->bullae;
    }
    si (k >= ZEPHYRUM)
    {
        p->profunditas = (i32)k;
    }
    si (   p->profunditas > ZEPHYRUM
        && _in_tabula(titulus, CLAUDENTIA_PARAGRAPHI,
            TABULAE_NUMERUS(CLAUDENTIA_PARAGRAPHI)))
    {
        vertex = (constans ScopiGradus*)xar_obtinere(p->scopi,
            p->profunditas - I);
        si (vertex->p >= ZEPHYRUM)
        {
            p->profunditas = (i32)vertex->p;
        }
    }
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
            b32 in_tabula;

            si (regula->claudentia == NIHIL)
            {
                redde _titulus_est(novum, regula->apertum);
            }
            in_tabula = _in_tabula(novum, regula->claudentia,
                regula->numerus);
            redde regula->nisi ? (b32)!in_tabula : in_tabula;
        }
    }
    redde FALSUM;
}


/* ==================================================
 * Lexemata singula
 * ================================================== */

interior chorda
_titulus_aperti (
    constans HtmlParsura* p,
                     i32  k)
{
    redde _titulus_elementi(_apertum(p, k));
}

/* Titulus verticis acervi; acervo vacuo contextus fragmenti (vacuus
 * in documento) - contextus vertex SUB acervo est (O7a). */
interior chorda
_titulus_verticis (
    constans HtmlParsura* p)
{
    si (p->profunditas > ZEPHYRUM)
    {
        redde _titulus_aperti(p, p->profunditas - I);
    }
    redde p->contextus;
}

/* Tags clausurae in tres classes (WHATWG 'in body', O6):
 * PROPRIAE - regula sua, hic elementum apertum proximum eiusdem
 * tituli UBIQUE (approximatio H3: p, li, dd/dt, capita, formae per
 * adoption, partes tabularum, select, template, cruda ...);
 * SCOPI - tags blocorum: elementum proximum IN SCOPO (limites
 * LIMITES_SCOPI sistunt: '<div><table><td></div>' </div> neglectum);
 * ceterae ('any other end tag'): ambulatio ab vertice, titulus par
 * claudit, elementum 'special' ante id sistit -> neglectum (malum):
 * '</div>' intra template, '</span>' intra td. */
hic_manens constans character* constans CLAUSURAE_PROPRIAE[] = {
    "html", "body", "br", "p", "li", "dd", "dt", "h1", "h2", "h3", "h4",
    "h5", "h6", "a", "b", "big", "code", "em", "font", "i", "nobr", "s",
    "small", "strike", "strong", "tt", "u", "table", "caption", "col",
    "colgroup", "tbody", "tfoot", "thead", "tr", "td", "th", "select",
    "optgroup", "option", "template", "form", "frameset", "frame",
    "head", "noscript", "script", "style", "title", "textarea", "svg",
    "math"
};
hic_manens constans character* constans CLAUSURAE_SCOPI[] = {
    "address", "article", "aside", "blockquote", "button", "center",
    "details", "dialog", "dir", "div", "dl", "fieldset", "figcaption",
    "figure", "footer", "header", "hgroup", "listing", "main", "menu",
    "nav", "ol", "pre", "search", "section", "summary", "ul", "applet",
    "marquee", "object"
};

/* Index elementi aperti proximi eiusdem tituli IN SCOPO; -I si limes
 * prius occurrit. */
interior s32
_apertum_in_scopo_invenire (
    constans HtmlParsura* p,
                  chorda  titulus)
{
    i32 k;

    per (k = p->profunditas; k > ZEPHYRUM; k--)
    {
        chorda apertum = _titulus_aperti(p, k - I);

        si (_tituli_pares(apertum, titulus))
        {
            redde (s32)(k - I);
        }
        si (_limes_scopi_est(apertum))
        {
            redde (s32)-I;
        }
    }
    redde (s32)-I;
}


/* Index elementi aperti pro tag clausurae 'any other end tag': -I si
 * elementum 'special' prius occurrit (aut nullum par). */
interior s32
_apertum_generale_invenire (
    constans HtmlParsura* p,
                  chorda  titulus)
{
    i32 k;

    per (k = p->profunditas; k > ZEPHYRUM; k--)
    {
        chorda apertum = _titulus_aperti(p, k - I);

        si (_tituli_pares(apertum, titulus))
        {
            redde (s32)(k - I);
        }
        si (_in_tabula(apertum, SPECIALIA, TABULAE_NUMERUS(SPECIALIA)))
        {
            redde (s32)-I;
        }
    }
    redde (s32)-I;
}


/* ==================================================
 * Modi neglegendi (O5): quae DOM nodo non retinet
 * ================================================== */

/* WHATWG: tags quae vexillum 'frameset-ok' NON exstinguunt */
hic_manens constans character* constans COMPAGIS_INNOCUA[] = {
    "html", "head", "body", "frameset", "frame", "noframes", "base",
    "basefont", "bgsound", "link", "meta", "title", "style", "script",
    "noscript", "template"
};
/* in frameset / post frameset: tags apertionis permissa */
hic_manens constans character* constans COMPAGIS_PERMISSA[] = {
    "frameset", "frame", "noframes"
};
/* 'in select': tags apertionis permissa (input/keygen/textarea select
 * claudunt et normaliter tractantur; select ipsum a se clauditur) */
hic_manens constans character* constans SELECT_PERMISSA[] = {
    "option", "optgroup", "hr", "script", "template", "select", "input",
    "keygen", "textarea"
};
hic_manens constans character* constans SELECT_CLAUSURAE[] = {
    "option", "optgroup", "select", "template"
};
/* WHATWG 'in body': partes tabulae extra modum tabulae neglectae
 * (O7a; O7b per modum, non per scopum) */
hic_manens constans character* constans PARTES_TABULAE[] = {
    "caption", "col", "colgroup", "tbody", "td", "tfoot", "th", "thead",
    "tr"
};

/* Gradus verticis; acervo vacuo basis (contextus fragmenti aut
 * nihil visibile) - numquam NIHIL (O7a). */
interior constans ScopiGradus*
_vertex_gradus (
    constans HtmlParsura* p)
{
    si (p->profunditas == ZEPHYRUM)
    {
        redde &p->basis;
    }
    redde (constans ScopiGradus*)xar_obtinere(p->scopi,
        p->profunditas - I);
}

/* Tag apertionis quod DOM neglegit (spec 'parse error, ignore the
 * token') -> malum. */
interior b32
_tag_neglegendum (
    constans HtmlParsura* p,
                  chorda  titulus)
{
    constans ScopiGradus* vertex = _vertex_gradus(p);
                     b32  intra_compagem;

    intra_compagem = (b32)(vertex != NIHIL && vertex->intra_compagem);

    si (_titulus_est(titulus, "html"))
    {
        redde p->html_visum;
    }
    si (_titulus_est(titulus, "head"))
    {
        redde (b32)(p->head_visum || p->body_visum
            || p->compages_visa);
    }
    si (_titulus_est(titulus, "body"))
    {
        redde (b32)(p->body_visum || p->compages_visa);
    }
    si (_titulus_est(titulus, "frameset") && !intra_compagem)
    {
        redde (b32)!p->compages_licet;
    }
    si (intra_compagem || p->compages_visa)
    {
        redde (b32)!_in_tabula(titulus, COMPAGIS_PERMISSA,
            TABULAE_NUMERUS(COMPAGIS_PERMISSA));
    }
    si (vertex != NIHIL && vertex->selectum >= ZEPHYRUM)
    {
        redde (b32)!_in_tabula(titulus, SELECT_PERMISSA,
            TABULAE_NUMERUS(SELECT_PERMISSA));
    }
    /* O7a/O7b: partes tabulae extra modum tabulae ('in body': parse
     * error, ignore) - intra template acceptae (spec 'in template'),
     * in contento alieno numquam (svg tr elementum alienum est) */
    si (   vertex                != NIHIL
        && vertex->modus_tabulae == TABULAE_NULLUS
        && vertex->templi < ZEPHYRUM
        && vertex->liberorum     == HTML_ALIENUM_NULLUM
        && _in_tabula(titulus, PARTES_TABULAE,
            TABULAE_NUMERUS(PARTES_TABULAE)))
    {
        redde VERUM;
    }
    /* O7b: modus tabulae ex contextu fragmenti SOLO (pars vera nulla
     * in acervo): spec 'has an element in table scope' fallit - in
     * table omnia praeter table, in table body tr/td/th, in row td/th,
     * in caption nihil, in column group col; cetera neglecta */
    si (   vertex                != NIHIL
        && vertex->modus_tabulae != TABULAE_NULLUS
        && vertex->partis < ZEPHYRUM
        && vertex->liberorum     == HTML_ALIENUM_NULLUM
        && (_titulus_est(titulus, "table")
            || _in_tabula(titulus, PARTES_TABULAE,
                TABULAE_NUMERUS(PARTES_TABULAE))))
    {
        b32 cella = (b32)(_titulus_est(titulus, "td")
                          || _titulus_est(titulus, "th"));

        commutatio (vertex->modus_tabulae)
        {
            casus TABULAE_TABULA:
                redde _titulus_est(titulus, "table");
            casus TABULAE_SECTIO:
                redde (b32)!(cella || _titulus_est(titulus, "tr"));
            casus TABULAE_ORDO:
                redde (b32)!cella;
            casus TABULAE_COLUMNAE:
                redde (b32)!_titulus_est(titulus, "col");
            casus TABULAE_INSCRIPTIO:
                /* 'in caption': partes neglectae, table in body nova */
                redde (b32)!_titulus_est(titulus, "table");
            ordinarius:
                redde VERUM;
        }
    }
    redde FALSUM;
}

/* Vexilla documenti post tag apertionis acceptum */
interior vacuum
_vexilla_renovare (
    HtmlParsura* p,
         chorda  titulus)
{
    /* modus involucrorum (O7a): tag verum acceptum modum movet ut
     * fictum moveret */
    si (_titulus_est(titulus, "html"))
    {
        p->html_visum = VERUM;
        si (p->modus == MODUS_ANTE_RADICEM)
        {
            p->modus = MODUS_ANTE_CAPUT;
        }
    }
    alioquin si (_titulus_est(titulus, "head"))
    {
        p->head_visum  = VERUM;
        p->modus       = MODUS_IN_CAPITE;
    }
    alioquin si (_titulus_est(titulus, "body"))
    {
        p->body_visum  = VERUM;
        p->modus       = MODUS_IN_CORPORE;
    }
    alioquin si (_titulus_est(titulus, "frameset"))
    {
        p->compages_visa = VERUM;
        /* post '</body>'/'</html>' modus posterior manet (O7b) */
        si (p->modus < MODUS_IN_CORPORE)
        {
            p->modus = MODUS_IN_CORPORE;
        }
    }
    /* O7b: contentum post '</body>'/'</html>' modum 'in body' iterum
     * facit (spec 'anything else'): commentaria deinde in body */
    si (p->modus >= MODUS_POST_CORPUS)
    {
        p->corpus_iterum = VERUM;
    }
    si (!_in_tabula(titulus, COMPAGIS_INNOCUA,
            TABULAE_NUMERUS(COMPAGIS_INNOCUA)))
    {
        p->compages_licet = FALSUM;
    }
    p->contentum_visum = VERUM;
}

interior b32
_textus_albus (
    constans MateriaToken* token)
{
    i32 i;

    per (i = ZEPHYRUM; i < token->valor.mensura; i++)
    {
        i8 c = token->valor.datum[i];

        si (   c != ' ' && c != '\t' && c != '\n' && c != '\r'
            && c != '\f')
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}


/* ==================================================
 * Synthesis (O7a, 2026-09-15): elementa quae spec fingit
 * ================================================== */

/* Elementum fictum: nodus generis elementum sine lexematibus, locus
 * 'synthesis' solus (lex H4 servata - lexemata ficta nulla, nodus
 * fictus annotatus; emissor octetorum nihil scribit). Parenti
 * praesenti appensum, impulsum si iussum. */
interior b32
_elementum_fingere (
      HtmlParsura* p,
    HtmlSynthesis  synthesis,
              b32  impellendum)
{
    MateriaNodus* elementum;

    elementum = materia_nodus_creare(p->piscina,
        (s32)HTML_GENUS_ELEMENTUM,
        _loci_numerus((s32)HTML_GENUS_ELEMENTUM));
    si (elementum == NIHIL)
    {
        redde FALSUM;
    }
    si (!materia_nodus_ponere(elementum, (i32)HTML_ELEMENTUM_SYNTHESIS,
            materia_valor_index((s32)synthesis), MATERIA_LOCUS_INDEX))
    {
        redde FALSUM;
    }
    si (!_liberum_appendere(p, elementum))
    {
        redde FALSUM;
    }
    /* sedes futurae (O7b) */
    si (synthesis == HTML_SYNTHESIS_RADIX)
    {
        p->radix = elementum;
    }
    alioquin si (synthesis == HTML_SYNTHESIS_CAPUT)
    {
        p->caput = elementum;
    }
    alioquin si (synthesis == HTML_SYNTHESIS_CORPUS)
    {
        p->corpus = elementum;
    }
    si (!impellendum)
    {
        redde VERUM;
    }
    redde _impellere(p, elementum, _titulus_elementi(elementum));
}

/* Tag quod in head manet (WHATWG 'in head', praeter head ipsum) */
interior b32
_caput_contentum (
    chorda titulus)
{
    redde (b32)(_in_tabula(titulus, PERMISSA_CAPITIS,
        TABULAE_NUMERUS(PERMISSA_CAPITIS))
                && !_titulus_est(titulus, "head"));
}

/* Index elementi aperti proximi cuius titulus litteris datis par est;
 * -I si nullum. */
interior s32
_apertum_invenire_literis (
    constans HtmlParsura* p,
      constans character* litterae)
{
    i32 k;

    per (k = p->profunditas; k > ZEPHYRUM; k--)
    {
        si (_titulus_est(_titulus_aperti(p, k - I), litterae))
        {
            redde (s32)(k - I);
        }
    }
    redde (s32)-I;
}

/* INVOLUCRA html/head/body (WHATWG 'before html' ... 'after head'):
 * ante lexema quod ea postulat, quae desunt FINGERE. titulus = tag
 * apertionis (vacuus: textus non albus aut EOF); caput_contentum =
 * tag quod in head manet. Textus albus et commentaria numquam
 * fingunt (spec: neglecta aut in parentem praesentem). Head fictum
 * sine contento non impellitur (vacuum, iam clausum). Contentum
 * capitis POST head (spec: in head reponitur) in html manet - octeti
 * eius post head iacent (sedes, O7b). */
interior b32
_involucra_fingere (
    HtmlParsura* p,
         chorda  titulus,
            b32  caput_contentum,
            b32  finis)
{
    /* intra template (in head aut post head) modi involucrorum tacent:
     * contentum in template manet (spec 'in template'); O(I) per
     * gradum verticis, non ambulatio. EOF template apertum claudit et
     * body fingit (spec 'in template' EOF). */
    si (!finis && _vertex_gradus(p)->templi >= ZEPHYRUM)
    {
        redde VERUM;
    }
    si (p->modus == MODUS_ANTE_RADICEM)
    {
        si (_titulus_est(titulus, "html"))
        {
            redde VERUM;
        }
        si (!_elementum_fingere(p, HTML_SYNTHESIS_RADIX, VERUM))
        {
            redde FALSUM;
        }
        p->html_visum  = VERUM;
        p->modus       = MODUS_ANTE_CAPUT;
    }
    si (p->modus == MODUS_ANTE_CAPUT)
    {
        si (_titulus_est(titulus, "head"))
        {
            redde VERUM;
        }
        si (!_elementum_fingere(p, HTML_SYNTHESIS_CAPUT,
            caput_contentum))
        {
            redde FALSUM;
        }
        p->head_visum = VERUM;
        si (caput_contentum)
        {
            p->modus = MODUS_IN_CAPITE;
            redde VERUM;
        }
        p->modus = MODUS_POST_CAPUT;
    }
    si (p->modus == MODUS_IN_CAPITE)
    {
        s32 k;

        si (caput_contentum)
        {
            redde VERUM;
        }
        k = _apertum_invenire_literis(p, "head");
        si (k >= ZEPHYRUM)
        {
            p->profunditas = (i32)k;
        }
        p->modus = MODUS_POST_CAPUT;
    }
    si (p->modus == MODUS_POST_CAPUT)
    {
        si (   caput_contentum
            || _titulus_est(titulus, "body")
            || _titulus_est(titulus, "frameset"))
        {
            redde VERUM;
        }
        si (!_elementum_fingere(p, HTML_SYNTHESIS_CORPUS, VERUM))
        {
            redde FALSUM;
        }
        p->body_visum  = VERUM;
        p->modus       = MODUS_IN_CORPORE;
    }
    redde VERUM;
}

/* '<frameset>' post body FICTUM (spec 'in body', frameset-ok manente:
 * body e parente removetur, frameset in html inseritur): body sine
 * lexematibus cuius liberi mala sola sunt (aut nulli) e liberis
 * parentis tollitur - ultimus est - et mala eius in parentem
 * transferuntur (octeti manent, ordo servatur; nodus sine octetis
 * removeri POTEST, cum octetis non - O5). Body verum aut cum contento
 * manet: rubrum consulto. */
interior b32
_corpus_fictum_removere (
    HtmlParsura* p)
{
    MateriaNodus* corpus = p->corpus;
    MateriaNodus* parens;
    MateriaValor* liberi;
    MateriaValor* ultimus;
             s32  k = _apertum_invenire_literis(p, "body");
             i32  locus;
             i32  n;
             i32  i;

    /* body vivum aut iam clausum ('</body>' / '</html>' ante, O7b) */
    si (   corpus        == NIHIL
        || corpus->loci[HTML_ELEMENTUM_SYNTHESIS].genus
               != MATERIA_VALOR_INDEX
        || corpus->pater == NIHIL)
    {
        redde VERUM;
    }
    liberi = &corpus->loci[HTML_ELEMENTUM_LIBERI];
    n = (liberi->genus == MATERIA_VALOR_LISTA)
        ? materia_valor_lista_numerus(*liberi) : ZEPHYRUM;
    per (i = ZEPHYRUM; i < n; i++)
    {
        MateriaValor* v = materia_valor_lista_obtinere(
            corpus->loci[HTML_ELEMENTUM_LIBERI], i);

        si (   v == NIHIL || v->genus != MATERIA_VALOR_NODUS
            || v->datum.nodus->genus != (s32)HTML_GENUS_ELEMENTUM_MALUM)
        {
            redde VERUM;
        }
    }
    parens = corpus->pater;
    locus  = (parens->genus == (s32)HTML_GENUS_DOCUMENTUM)
           ? (i32)HTML_DOCUMENTUM_LIBERI : (i32)HTML_ELEMENTUM_LIBERI;
    liberi = &parens->loci[locus];
    si (   liberi->genus               != MATERIA_VALOR_LISTA
        || liberi->datum.lista.mensura == ZEPHYRUM)
    {
        redde VERUM;
    }
    ultimus = materia_valor_lista_obtinere(*liberi,
        liberi->datum.lista.mensura - I);
    si (   ultimus == NIHIL || ultimus->genus != MATERIA_VALOR_NODUS
        || ultimus->datum.nodus != corpus)
    {
        redde VERUM;
    }
    liberi->datum.lista.mensura = liberi->datum.lista.mensura - I;
    si (k >= ZEPHYRUM)
    {
        p->profunditas = (i32)k;
    }
    p->corpus = NIHIL;
    /* sedes contenti posterioris erat body: nunc html */
    si (p->sedes_posterior == corpus)
    {
        p->sedes_posterior = p->radix;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        MateriaValor* v = materia_valor_lista_obtinere(
            corpus->loci[HTML_ELEMENTUM_LIBERI], i);

        si (!materia_nodus_appendere(p->piscina, parens, locus, *v,
                MATERIA_LOCUS_LISTA_NODUS))
        {
            redde FALSUM;
        }
    }
    /* '</body>' iam in ficto (O7b): lexemata clausurae in malum
     * transeunt - octeti manent, DOM nodum nullum retinet (lex O5);
     * porta circuitus id primo cursu invenit (octeti perditi) */
    si (   _tok(corpus, (i32)HTML_ELEMENTUM_TOK_CLAUSURA) != NIHIL
        || _tok(corpus, (i32)HTML_ELEMENTUM_TOK_CLAUSURA_FINIS)
            != NIHIL)
    {
        MateriaNodus* malum = materia_nodus_creare(p->piscina,
            (s32)HTML_GENUS_ELEMENTUM_MALUM, (i32)I);
        i32 loci[II];

        loci[ZEPHYRUM]  = (i32)HTML_ELEMENTUM_TOK_CLAUSURA;
        loci[I]         = (i32)HTML_ELEMENTUM_TOK_CLAUSURA_FINIS;
        si (malum == NIHIL)
        {
            redde FALSUM;
        }
        per (i = ZEPHYRUM; i < II; i++)
        {
            MateriaToken* t = _tok(corpus, loci[i]);

            si (   t != NIHIL
                && !materia_nodus_appendere(p->piscina, malum,
                    (i32)HTML_MALUM_TOKENS, materia_valor_token(t),
                    MATERIA_LOCUS_LISTA_TOKEN))
            {
                redde FALSUM;
            }
        }
        si (!materia_nodus_appendere(p->piscina, parens, locus,
                materia_valor_nodus(malum), MATERIA_LOCUS_LISTA_NODUS))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* Partes tabulae fictae (WHATWG 'in table' / 'in table body'): tr
 * sub table -> tbody; td/th sub table -> tbody et tr, sub sectione
 * -> tr; col sub table -> colgroup. Vertex solus spectatur (modi
 * tabularum pleni et foster parenting = O7b). */
interior b32
_partes_tabulae_fingere (
    HtmlParsura* p,
         chorda  titulus)
{
    b32 cella = (b32)(_titulus_est(titulus, "td")
                      || _titulus_est(titulus, "th"));

    /* in contento alieno nihil fingitur (svg table alienum est) */
    si (_vertex_gradus(p)->liberorum != HTML_ALIENUM_NULLUM)
    {
        redde VERUM;
    }
    si (cella || _titulus_est(titulus, "tr"))
    {
        chorda vertex = _titulus_verticis(p);

        si (_titulus_est(vertex, "table"))
        {
            si (!_elementum_fingere(p, HTML_SYNTHESIS_CORPUS_TABULAE,
                    VERUM))
            {
                redde FALSUM;
            }
            vertex = _titulus_verticis(p);
        }
        si (   cella
            && (_titulus_est(vertex, "tbody")
                || _titulus_est(vertex, "thead")
                || _titulus_est(vertex, "tfoot")))
        {
            redde _elementum_fingere(p, HTML_SYNTHESIS_ORDO, VERUM);
        }
        redde VERUM;
    }
    si (   _titulus_est(titulus, "col")
        && _titulus_est(_titulus_verticis(p), "table"))
    {
        redde _elementum_fingere(p, HTML_SYNTHESIS_COLUMNAE, VERUM);
    }
    redde VERUM;
}


/* ==================================================
 * Sedes (O7b, 2026-09-15): parens DOM alibi
 * ================================================== */

/* Locum 'sedes' nodi ponere: referentia ad parentem DOM (elementum
 * aut documentum). Nodus ipse ubi octeti sunt manet. */
interior b32
_sedem_ponere (
    MateriaNodus* nodus,
             i32  locus,
    MateriaNodus* parens_dom)
{
    redde materia_nodus_ponere(nodus, locus,
        materia_valor_referentia(parens_dom), MATERIA_LOCUS_REFERENTIA);
}

/* Parens fovens (spec 'foster parent'): parens table apertae proximae;
 * sine table in acervo (fragmentum contextu tabulae) radix. */
interior MateriaNodus*
_parens_fovens (
    constans HtmlParsura* p)
{
    s32 tabulae = _vertex_gradus(p)->tabulae;

    redde (tabulae > ZEPHYRUM) ? _apertum(p, (i32)tabulae - I)
                               : p->documentum;
}

/* An locus insertionis foveatur: vertex (aut contextus) table/tbody/
 * tfoot/thead/tr HTML in modo tabulae (spec: 'foster parenting
 * enabled and target is table, tbody, tfoot, thead, or tr'). */
interior b32
_fovendum (
    constans HtmlParsura* p)
{
    constans ScopiGradus* vertex = _vertex_gradus(p);

    redde (b32)(vertex->liberorum == HTML_ALIENUM_NULLUM
                && vertex->modus_tabulae != TABULAE_NULLUS
                && _in_tabula(_titulus_verticis(p), FOVENTIA,
                    TABULAE_NUMERUS(FOVENTIA)));
}

/* Post '</body>' aut '</html>' (spec 'after body' / 'after after
 * body': lexemata praeter commentaria 'in body' iterum tractantur -
 * in vertice DOM tum aperto inseruntur, qui in spec manet): nodus in
 * html aut documento inserendus sedem illius verticis accipit
 * (sedes_posterior: body, elementum in eo apertum, aut html post
 * frameset). NIHIL si sedes = parens octetorum (superflua) aut nulla.
 * Liberi elementorum ibi apertorum normales. */
interior MateriaNodus*
_sedes_posterior (
    constans HtmlParsura* p)
{
    si (   (p->modus == MODUS_POST_CORPUS
            || p->modus == MODUS_POST_RADICEM)
        && p->sedes_posterior != NIHIL
        && (p->profunditas == ZEPHYRUM
            || _titulus_est(_titulus_verticis(p), "html"))
        && p->sedes_posterior != _parens(p))
    {
        redde p->sedes_posterior;
    }
    redde NIHIL;
}

/* '<input type=hidden>' in tabula manet (spec 'in table': input non
 * celatus fovetur); attributa post tag leguntur - decisio pendens
 * (fovens) hic solvitur, cum tag clauditur aut abrumpitur. */
interior b32
_input_celatum (
    constans MateriaNodus* elementum)
{
    constans MateriaValor* attributa =
        &elementum->loci[HTML_ELEMENTUM_ATTRIBUTA];
                      i32 n;
                      i32 i;

    si (attributa->genus != MATERIA_VALOR_LISTA)
    {
        redde FALSUM;
    }
    n = materia_valor_lista_numerus(*attributa);
    per (i = ZEPHYRUM; i < n; i++)
    {
        MateriaValor* v = materia_valor_lista_obtinere(*attributa, i);
        MateriaNodus* a;
        MateriaToken* titulus;
        MateriaToken* valor;
              chorda  c;

        si (v == NIHIL || v->genus != MATERIA_VALOR_NODUS)
        {
            perge;
        }
        a        = v->datum.nodus;
        titulus  = _tok(a, (i32)HTML_ATTRIBUTUM_TOK_NOMEN);
        valor    = _tok(a, (i32)HTML_ATTRIBUTUM_TOK_VALOR);
        si (   titulus == NIHIL || valor == NIHIL
            || !_titulus_est(titulus->valor, "type"))
        {
            perge;
        }
        c = valor->valor;
        si (   c.mensura >= I
            && (c.datum[ZEPHYRUM] == '"' || c.datum[ZEPHYRUM] == '\''))
        {
            c.datum    = c.datum + I;
            c.mensura  = c.mensura - I;
            si (   c.mensura >= I
                && c.datum[c.mensura - I]
                    == valor->valor.datum[ZEPHYRUM])
            {
                c.mensura = c.mensura - I;
            }
        }
        redde _titulus_est(c, "hidden");
    }
    redde FALSUM;
}

interior b32
_fovens_solvere (
    HtmlParsura* p)
{
    MateriaNodus* elementum  = p->tag_apertum;
    MateriaNodus* fovens     = p->fovens;

    p->fovens = NIHIL;
    si (   fovens == NIHIL || elementum == NIHIL
        || _input_celatum(elementum))
    {
        redde VERUM;
    }
    redde _sedem_ponere(elementum, (i32)HTML_ELEMENTUM_SEDES, fovens);
}

/* Contentum capitis post '</head>' (spec 'after head': in head
 * reponitur): elementum in html inserendum sedem head accipit. */
interior b32
_post_caput (
    constans HtmlParsura* p,
                  chorda  titulus)
{
    redde (b32)(p->modus == MODUS_POST_CAPUT && p->caput != NIHIL
                && _caput_contentum(titulus)
                && p->profunditas > ZEPHYRUM
                && _titulus_est(_titulus_verticis(p), "html"));
}

/* Modi tabularum ante clausuras implicitas (O7b). Tag partis tabulae
 * in modo tabulae acervum ad partem tabulae proximam purgat (spec
 * 'clear the stack back to a table / table body / row context';
 * cella et caption per tabulam clausurarum deinde cadunt) - elementa
 * fota inter eas cadunt. '<table>' in modis table/section/row/colgroup
 * tabulam apertam claudit (spec 'in table' <table>: pop until table);
 * in cella et caption nidificat. */
interior vacuum
_tabulas_purgare (
    HtmlParsura* p,
         chorda  titulus)
{
    constans ScopiGradus* vertex = _vertex_gradus(p);

    /* in contento alieno nihil (math tbody elementum alienum est) */
    si (   vertex->modus_tabulae == TABULAE_NULLUS
        || vertex->liberorum     != HTML_ALIENUM_NULLUM)
    {
        redde;
    }
    si (_titulus_est(titulus, "table"))
    {
        si (   vertex->modus_tabulae != TABULAE_CELLA
            && vertex->modus_tabulae != TABULAE_INSCRIPTIO
            && vertex->tabulae       >= ZEPHYRUM)
        {
            p->profunditas = (i32)vertex->tabulae;
        }
        redde;
    }
    si (!_in_tabula(titulus, PARTES_TABULAE,
            TABULAE_NUMERUS(PARTES_TABULAE)))
    {
        redde;
    }
    dum (p->profunditas > ZEPHYRUM && !_vertex_gradus(p)->pars_tabulae)
    {
        p->profunditas = p->profunditas - I;
    }
}


/* ==================================================
 * Lexemata singula: tags
 * ================================================== */

interior b32
_aperturam_tractare (
     HtmlParsura* p,
    MateriaToken* token)
{
    MateriaNodus* elementum;
          chorda  titulus;
             b32  fovendum;
             b32  forma_tabulae;

    _pendentia_claudere(p);
    titulus = _tag_titulus(token);

    /* O5: tag neglectum -> malum pendens (attributa et '>' per vias
     * H8 sequuntur; tag_apertum NIHIL manet) */
    si (_tag_neglegendum(p, titulus))
    {
        redde _malum_addere(p, token);
    }
    /* 'in select': input/keygen/textarea select claudunt (spec), deinde
     * tag normaliter */
    {
        constans ScopiGradus* vertex = _vertex_gradus(p);

        si (   vertex != NIHIL && vertex->selectum >= ZEPHYRUM
            && (_titulus_est(titulus, "input")
                || _titulus_est(titulus, "keygen")
                || _titulus_est(titulus, "textarea")))
        {
            p->profunditas = (i32)vertex->selectum;
        }
    }
    /* O7a: '<frameset>' acceptum post body fictum: body removetur
     * (etiam post '</body>'/'</html>', O7b: spec 'after body' ->
     * 'in body' iterum) */
    si (   _titulus_est(titulus, "frameset")
        && p->modus >= MODUS_IN_CORPORE && !p->compages_visa
        && !_corpus_fictum_removere(p))
    {
        redde FALSUM;
    }
    /* O7a: involucra quae desunt ante tag fingere (html/head/body) */
    si (!_involucra_fingere(p, titulus, _caput_contentum(titulus),
            FALSUM))
    {
        redde FALSUM;
    }
    _vexilla_renovare(p, titulus);

    /* O7b: modi tabularum (purgatio acervi, '<table>' in tabula),
     * clausurae per scopum (O2b-5), deinde vertex acervi dum tabula
     * id iubet */
    _tabulas_purgare(p, titulus);
    _scopos_claudere(p, titulus);
    dum (p->profunditas > ZEPHYRUM)
    {
        chorda vertex = _titulus_aperti(p, p->profunditas - I);

        si (vertex.mensura == ZEPHYRUM || !_claudit(vertex, titulus))
        {
            frange;
        }
        p->profunditas = p->profunditas - I;
    }
    /* O7a: partes tabulae quas spec fingit (tbody/tr/colgroup) */
    si (!_partes_tabulae_fingere(p, titulus))
    {
        redde FALSUM;
    }
    /* O7b: foster parenting - vertex table/tbody/tfoot/thead/tr et tag
     * quod modus tabulae non tractat: nodus hic manet, sedes = parens
     * tabulae; form in tabula inseritur nec impellitur (spec 'in
     * table' form: insert, pop) */
    fovendum      = (b32)(_fovendum(p)
                          && !_in_tabula(titulus, TABULAE_ACCEPTA,
                              TABULAE_NUMERUS(TABULAE_ACCEPTA)));
    forma_tabulae = (b32)(_fovendum(p)
        && _titulus_est(titulus, "form"));

    elementum = materia_nodus_creare(p->piscina,
        (s32)HTML_GENUS_ELEMENTUM,
        _loci_numerus((s32)HTML_GENUS_ELEMENTUM));
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
    /* O7b: sedes - fotum ante tabulam (input: decisio pendens usque ad
     * attributa), post body in vertice tum aperto, contentum capitis
     * post head in head */
    si (fovendum && _titulus_est(titulus, "input"))
    {
        p->fovens = _parens_fovens(p);
    }
    alioquin si (fovendum)
    {
        si (!_sedem_ponere(elementum, (i32)HTML_ELEMENTUM_SEDES,
                _parens_fovens(p)))
        {
            redde FALSUM;
        }
    }
    alioquin si (_sedes_posterior(p) != NIHIL)
    {
        si (!_sedem_ponere(elementum, (i32)HTML_ELEMENTUM_SEDES,
                _sedes_posterior(p)))
        {
            redde FALSUM;
        }
    }
    alioquin si (_post_caput(p, titulus))
    {
        si (!_sedem_ponere(elementum, (i32)HTML_ELEMENTUM_SEDES,
                p->caput))
        {
            redde FALSUM;
        }
    }
    si (!_liberum_appendere(p, elementum))
    {
        redde FALSUM;
    }
    si (_titulus_est(titulus, "html"))
    {
        p->radix = elementum;
    }
    alioquin si (_titulus_est(titulus, "head"))
    {
        p->caput = elementum;
    }
    alioquin si (_titulus_est(titulus, "body"))
    {
        p->corpus = elementum;
    }
    /* vacuum numquam aperitur: non impellitur; tag eius tamen
     * pendens manet (attributa, '>') */
    si (!_vacuum_est(titulus) && !forma_tabulae)
    {
        si (!_impellere(p, elementum, titulus))
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

        /* O7b: input in tabula - attributa nunc lecta: fotum nisi
         * type=hidden */
        si (!_fovens_solvere(p))
        {
            redde FALSUM;
        }
        p->tag_apertum  = NIHIL;
        p->attributum   = NIHIL;
        /* tag se-claudens in contento alieno elementum CLAUDIT (spec:
         * 'acknowledge self-closing flag'; O2b-6) - in HTML numquam
         * (caput): elementum in vertice, spatium proprium alienum */
        si (   token->genus == (s32)HTML_LEX_TAG_FINIS_SOLUS
            && p->profunditas > ZEPHYRUM
            && _apertum(p, p->profunditas - I) == elementum
            && ((constans ScopiGradus*)xar_obtinere(p->scopi,
                p->profunditas - I))->proprium != HTML_ALIENUM_NULLUM)
        {
            p->profunditas = p->profunditas - I;
        }
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
    constans ScopiGradus* vertex;
                  chorda  titulus;
                     s32  k;

    _pendentia_claudere(p);
    titulus  = _tag_titulus(token);
    /* O7a: '</body>' '</html>' '</br>' ante corpus (spec 'anything
     * else' in modis involucrorum): involucra usque ad body ficta,
     * deinde ut in corpore */
    si (   p->modus != MODUS_IN_CORPORE
        && (_titulus_est(titulus, "body")
        || _titulus_est(titulus, "html")
            || _titulus_est(titulus, "br")))
    {
        si (!_involucra_fingere(p, _vacua(), FALSUM, FALSUM))
        {
            redde FALSUM;
        }
    }
    vertex   = _vertex_gradus(p);
    /* O5: in select tags clausurae aliena neglecta. O7b: '</body>' et
     * '</html>' CLAUDUNT (O5 ea mala tenebat, corpus ad EOF apertum) -
     * contentum posterius sedem verticis tum aperti accipit
     * (_sedes_posterior); intra template et in modis tabularum
     * neglecta (spec 'in template', 'in table' ... 'in cell') */
    si (   (vertex != NIHIL && vertex->selectum >= ZEPHYRUM
            && !_in_tabula(titulus, SELECT_CLAUSURAE,
                TABULAE_NUMERUS(SELECT_CLAUSURAE)))
        || (vertex != NIHIL
            && (vertex->templi >= ZEPHYRUM
                || vertex->modus_tabulae != TABULAE_NULLUS)
            && (_titulus_est(titulus, "body")
                || _titulus_est(titulus, "html"))))
    {
        redde _malum_addere(p, token);
    }
    /* O7a: '</head>' ante head (spec 'anything else': head fingitur,
     * deinde ab hoc tag clauditur - clausura vera in elemento ficto) */
    si (   _titulus_est(titulus, "head")
        && (p->modus == MODUS_ANTE_RADICEM
            || p->modus == MODUS_ANTE_CAPUT))
    {
        si (!_involucra_fingere(p, _vacua(), VERUM, FALSUM))
        {
            redde FALSUM;
        }
    }
    si (_in_tabula(titulus, CLAUSURAE_PROPRIAE,
            TABULAE_NUMERUS(CLAUSURAE_PROPRIAE)))
    {
        k = _apertum_invenire(p, titulus);
    }
    alioquin si (_in_tabula(titulus, CLAUSURAE_SCOPI,
                     TABULAE_NUMERUS(CLAUSURAE_SCOPI)))
    {
        k = _apertum_in_scopo_invenire(p, titulus);
    }
    alioquin
    {
        k = _apertum_generale_invenire(p, titulus);
    }
    si (k < ZEPHYRUM)
    {
        redde _malum_addere(p, token);
    }
    /* O7b: vertex DOM cum '</body>' aut '</html>' venit sedes contenti
     * posterioris fit (spec: acervus non mutatur); semel */
    si (   p->sedes_posterior == NIHIL && p->profunditas > ZEPHYRUM
        && (_titulus_est(titulus, "body")
        || _titulus_est(titulus, "html")))
    {
        p->sedes_posterior = _apertum(p, p->profunditas - I);
    }
    p->clausura     = _apertum(p, (i32)k);
    p->profunditas  = (i32)k;
    /* head clausum: modus 'after head' (O7a); body clausum: 'after
     * body', html clausum: 'after after body' (O7b) */
    si (p->modus == MODUS_IN_CAPITE && _titulus_est(titulus, "head"))
    {
        p->modus = MODUS_POST_CAPUT;
    }
    alioquin si (   p->modus == MODUS_IN_CORPORE
                 && _titulus_est(titulus, "body"))
    {
        p->modus = MODUS_POST_CORPUS;
    }
    alioquin si (   (p->modus == MODUS_IN_CORPORE
                     || p->modus == MODUS_POST_CORPUS)
                 && _titulus_est(titulus, "html"))
    {
        p->modus = MODUS_POST_RADICEM;
    }
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
             b32  albus;

    _pendentia_claudere(p);
    genus = _genus_contenti(token->genus);
    albus = _textus_albus(token);
    /* O5: doctype post contentum et textus non albus in frameset
     * neglecta -> malum unius lexematis */
    {
        constans ScopiGradus* vertex     = _vertex_gradus(p);
                         b32  neglectum  = FALSUM;

        si (genus == (s32)HTML_GENUS_DOCTYPE && p->contentum_visum)
        {
            neglectum = VERUM;
        }
        alioquin si (   (genus == (s32)HTML_GENUS_TEXTUS
                         || genus == (s32)HTML_GENUS_REFERENTIA
                         || genus == (s32)HTML_GENUS_CDATA)
                     && !albus
                     && ((vertex != NIHIL && vertex->intra_compagem)
                         || p->compages_visa))
        {
            neglectum = VERUM;
        }
        /* O7a: textus albus ante html/head (spec 'before html' et
         * 'before head' eum neglegunt) */
        alioquin si (   genus == (s32)HTML_GENUS_TEXTUS && albus
                     && (p->modus == MODUS_ANTE_RADICEM
                         || p->modus == MODUS_ANTE_CAPUT))
        {
            neglectum = VERUM;
        }
        si (neglectum)
        {
            si (!_malum_addere(p, token))
            {
                redde FALSUM;
            }
            p->malum = NIHIL;
            redde VERUM;
        }
        si (   !albus
            && (genus == (s32)HTML_GENUS_TEXTUS
                || genus == (s32)HTML_GENUS_REFERENTIA
                || genus == (s32)HTML_GENUS_CDATA))
        {
            p->contentum_visum  = VERUM;
            p->compages_licet   = FALSUM;
            /* O7a: textus corporis involucra postulat (cdata extra
             * alienos commentarium est - non fingit, ut commentaria) */
            si (   genus != (s32)HTML_GENUS_CDATA
                && !_involucra_fingere(p, _vacua(), FALSUM, FALSUM))
            {
                redde FALSUM;
            }
        }
    }
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
    /* O7b: sedes textus - fotus (spec 'in table text': textus non
     * albus in vertice table/tbody/tfoot/thead/tr; colgroup prius
     * clauditur, spec 'in column group') aut post body; commentarium
     * post body solum si contentum iam in body rediit */
    si (   genus == (s32)HTML_GENUS_TEXTUS
        || genus == (s32)HTML_GENUS_REFERENTIA)
    {
        i32 locus = (genus == (s32)HTML_GENUS_TEXTUS)
                  ? (i32)HTML_TEXTUS_SEDES : (i32)HTML_REFERENTIA_SEDES;

        si (   !albus && p->profunditas > ZEPHYRUM
            && _titulus_est(_titulus_verticis(p), "colgroup"))
        {
            p->profunditas = p->profunditas - I;
        }
        si (!albus && _fovendum(p))
        {
            si (!_sedem_ponere(folium, locus, _parens_fovens(p)))
            {
                redde FALSUM;
            }
        }
        alioquin si (_sedes_posterior(p) != NIHIL)
        {
            si (!_sedem_ponere(folium, locus, _sedes_posterior(p)))
            {
                redde FALSUM;
            }
            si (!albus)
            {
                p->corpus_iterum = VERUM;
            }
        }
    }
    alioquin si (   genus == (s32)HTML_GENUS_COMMENTARIUM
                 && p->corpus_iterum && _sedes_posterior(p) != NIHIL)
    {
        si (!_sedem_ponere(folium, (i32)HTML_COMMENTARIUM_SEDES,
                _sedes_posterior(p)))
        {
            redde FALSUM;
        }
    }
    redde _liberum_appendere(p, folium);
}


/* ==================================================
 * Ingressus
 * ================================================== */

interior MateriaNodus*
_parsare (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura,
                chorda  contextus,
           HtmlAlienum  spatium,
                   b32  fragmentum)
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
    p.piscina         = piscina;
    p.compages_licet  = VERUM;
    p.basis           = _basis_vacua();
    p.contextus       = contextus;
    p.modus           = MODUS_ANTE_RADICEM;
    p.fragmentum      = fragmentum;
    si (fragmentum)
    {
        b32 radix = _titulus_est(contextus, "html");

        /* contextus = vertex SUB acervo: spatia, select, frameset,
         * tabula ex eo; involucra iam adsunt (spec 'reset the
         * insertion mode': 'in body', nisi contextu html - 'before
         * head', head et body sub radice fingenda) */
        p.basis.proprium        = spatium;
        p.basis.liberorum       = html_alienum_liberorum(spatium,
            contextus, NIHIL);
        p.basis.selectum        = _titulus_est(contextus, "select")
                                ? ZEPHYRUM : (s32)-I;
        p.basis.intra_compagem  = _titulus_est(contextus, "frameset");
        /* modus tabulae contextus (spec 'reset the insertion mode':
         * td/th ultimi ad 'in body' recidunt, caption/colgroup manent);
         * table nulla in acervo: parens fovens = radix */
        p.basis.modus_tabulae   = _modus_tabulae_tituli(contextus);
        si (p.basis.modus_tabulae == TABULAE_CELLA)
        {
            p.basis.modus_tabulae = TABULAE_NULLUS;
        }
        p.modus           = radix ? MODUS_ANTE_CAPUT : MODUS_IN_CORPORE;
        p.html_visum      = VERUM;
        p.head_visum      = (b32)!radix;
        p.body_visum      = (b32)!radix;
        p.compages_licet  = radix;
    }

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

    p.acervus  = xar_creare(piscina, magnitudo(MateriaNodus*));
    p.scopi    = xar_creare(piscina, magnitudo(ScopiGradus));
    p.documentum = materia_nodus_creare(piscina,
        (s32)HTML_GENUS_DOCUMENTUM, (i32)II);
    si (p.acervus == NIHIL || p.scopi == NIHIL || p.documentum == NIHIL)
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
            /* O7a: involucra quae desunt ad EOF (spec 'anything else'
             * usque ad body fictum, template apertum non obstante) */
            si (!_involucra_fingere(&p, _vacua(), FALSUM, VERUM))
            {
                redde NIHIL;
            }
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

MateriaNodus*
html_arbor_parsare (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura)
{
    redde _parsare(piscina, fons, mensura, _vacua(),
        HTML_ALIENUM_NULLUM,
        FALSUM);
}

MateriaNodus*
html_arbor_parsare_fragmentum (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura,
                chorda  contextus,
           HtmlAlienum  spatium)
{
    redde _parsare(piscina, fons, mensura, contextus, spatium, VERUM);
}
