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
 *
 * ADOPTION AGENCY (O7c, 2026-09-16): acervus apertorum acervus DOM
 * spec est; gradus quisque 'octeti' fert - apertus octetis (liberos
 * octetorum accipit) aut clausus octetis sed in DOM apertus (elementum
 * formans clausum, '</body>'). Nodus novus in gradum RECEPTOREM
 * (proximum octetis apertum) it et sedem verticis DOM accipit. Lista
 * formantium activorum (Formans, signa td/th/caption/applet/object/
 * marquee/template, arca Noe), reconstructio, algorithmus adoptionis
 * WHATWG in acervo DOM: elementum formans iterum apertum EXEMPLAR est
 * (nodus sine lexematibus, locus exemplar ad originale), nodus in
 * avum motus sedem et PRAECEDENTEM (fratrem priorem = elementum
 * formans) accipit, liberi blocci in exemplar involvuntur (chirurgia
 * octetis tuta). '</p>' sine p in scopo et '</br>' elementa ficta
 * clausa dant.
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
    /* O7c: octeti = gradus OCTETIS APERTUS (liberos octetorum accipit:
     * in via octetorum praesenti); FALSUM = in DOM apertus, octetis
     * clausus (elementum formans clausum, '</body>'): nodi novi in
     * receptorem eunt cum sede huius. receptor = index gradus proximi
     * octetis aperti hic aut infra (-I = documentum). */
    b32 octeti;
    s32 receptor;
} ScopiGradus;

/* Lista formantium activorum (WHATWG 'list of active formatting
 * elements', O7c): elementum formans apertum (a b big code em font i
 * nobr s small strike strong tt u) aut SIGNUM (marker: applet object
 * marquee template td th caption). index = gradus acervi in impulsu,
 * ratus dum _apertum(index) == elementum (lapsus = e acervo exiit:
 * reconstructio eum iterum aperit; signum lapsum listam ad se purgat
 * - pigre, in usu proximo). */
nomen structura {
    MateriaNodus* elementum;
             s32  index;
             b32  signum;
} Formans;

/* Gradus novus pro chirurgia acervi (adoption agency) */
nomen structura {
    MateriaNodus* nodus;
             b32  octeti;
} GradusNovus;

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
     * fovens = parens fovens tagi aperti pendentis (input: type=hidden
     * in tabula manet, decernitur cum attributa lecta sunt) */
            MateriaNodus* caput;
            MateriaNodus* corpus;
            MateriaNodus* radix;
            MateriaNodus* fovens;
    /* post '</body>' contentum non album aut tag venit: spec ad 'in
     * body' redit - commentaria quoque in verticem DOM (O7c: acervus
     * DOM post '</body>' manet, gradus octetis clausi) */
                     b32 corpus_iterum;
    /* O7c: lista formantium activorum (repositorium Xar, numerus
     * proprius formantium) et nodi cum sede (involutio adoptionis eos
     * in exemplar transfert) */
                     Xar* formantia;
                     i32  formantium;
                     Xar* sedentes;
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

/* Parens OCTETORUM: gradus receptor verticis (proximus octetis
 * apertus, O7c), documentum acervo vacuo aut receptore nullo. */
interior MateriaNodus*
_parens (
    constans HtmlParsura* p)
{
    si (p->profunditas > ZEPHYRUM)
    {
        constans ScopiGradus* g = (constans ScopiGradus*)xar_obtinere(
            p->scopi, p->profunditas - I);

        si (g->receptor >= ZEPHYRUM)
        {
            redde _apertum(p, (i32)g->receptor);
        }
    }
    redde p->documentum;
}

/* Vertex DOM (spec 'current node'): elementum apertum summum,
 * documentum acervo vacuo. Parens octetorum idem est nisi vertex
 * octetis clausus (O7c). */
interior MateriaNodus*
_vertex_nodus (
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
                  chorda  titulus,
                     b32  octeti);

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
    ScopiGradus gradus = _scopi_gradus(p, p->profunditas, titulus,
        VERUM);

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

/* Locus 'sedes' generis (-I si genus eum non fert) */
interior s32
_locus_sedis (
    s32 genus)
{
    commutatio ((HtmlGenus)genus)
    {
        casus HTML_GENUS_ELEMENTUM:
            redde (s32)HTML_ELEMENTUM_SEDES;
        casus HTML_GENUS_TEXTUS:
            redde (s32)HTML_TEXTUS_SEDES;
        casus HTML_GENUS_REFERENTIA:
            redde (s32)HTML_REFERENTIA_SEDES;
        casus HTML_GENUS_COMMENTARIUM:
            redde (s32)HTML_COMMENTARIUM_SEDES;
        ordinarius:
            redde (s32)-I;
    }
}

/* Locum scribere sive vacuum (ponere) sive iam scriptum (reponere) */
interior b32
_locum_scribere (
         MateriaNodus* nodus,
                  i32  locus,
         MateriaValor  valor,
  MateriaLocusSpecies  species)
{
    si (nodus->loci[locus].genus != MATERIA_VALOR_NIHIL)
    {
        redde materia_nodus_reponere(nodus, locus, valor, species);
    }
    redde materia_nodus_ponere(nodus, locus, valor, species);
}

/* Scopus referentiae loci, NIHIL si absens aut revocata */
interior MateriaNodus*
_referentia (
    constans MateriaNodus* nodus,
                      i32  locus)
{
    redde (nodus->loci[locus].genus == MATERIA_VALOR_REFERENTIA)
        ? nodus->loci[locus].datum.nodus : NIHIL;
}

/* Sedes nodi (parens DOM alibi), NIHIL si nulla */
interior MateriaNodus*
_sedes (
    constans MateriaNodus* nodus)
{
    s32 locus = _locus_sedis(nodus->genus);

    redde (locus >= ZEPHYRUM) ? _referentia(nodus, (i32)locus) : NIHIL;
}

/* Locum 'sedes' nodi ponere (O7b): referentia ad parentem DOM
 * (elementum aut documentum); nodus ipse ubi octeti sunt manet.
 * Iterum scribi potest (adoption agency parentem DOM mutat, O7c);
 * nodus in sedentes semel intrat. */
interior b32
_sedem_ponere (
     HtmlParsura* p,
    MateriaNodus* nodus,
    MateriaNodus* parens_dom)
{
    s32 locus = _locus_sedis(nodus->genus);

    si (locus < ZEPHYRUM)
    {
        redde VERUM;
    }
    si (nodus->loci[locus].genus == MATERIA_VALOR_NIHIL)
    {
        MateriaNodus** l = (MateriaNodus**)xar_addere(p->sedentes);

        si (l == NIHIL)
        {
            redde FALSUM;
        }
        *l = nodus;
    }
    redde _locum_scribere(nodus, (i32)locus,
        materia_valor_referentia(parens_dom), MATERIA_LOCUS_REFERENTIA);
}

/* Nodum in liberos parentis OCTETORUM appendere; parens DOM datus
 * (vertex DOM plerumque) alius quam parens octetorum et nodus sedem
 * nondum ferens -> sedes = parens DOM (O7c; sedem posteriorem O7b
 * generalizat: post '</body>' vertex DOM octetis clausus est). Lista
 * in primo appendendo nascitur: locus absens = lista vacua (lex md). */
interior b32
_liberum_appendere_ad (
     HtmlParsura* p,
    MateriaNodus* nodus,
    MateriaNodus* parens_dom)
{
    MateriaNodus* parens = _parens(p);
             i32  locus;

    si (   parens_dom                 != NIHIL && parens_dom != parens
        && _locus_sedis(nodus->genus) >= ZEPHYRUM
        && _sedes(nodus)              == NIHIL
        && !_sedem_ponere(p, nodus, parens_dom))
    {
        redde FALSUM;
    }
    locus = (parens->genus == (s32)HTML_GENUS_DOCUMENTUM)
          ? (i32)HTML_DOCUMENTUM_LIBERI
          : (i32)HTML_ELEMENTUM_LIBERI;
    redde materia_nodus_appendere(p->piscina, parens, locus,
        materia_valor_nodus(nodus), MATERIA_LOCUS_LISTA_NODUS);
}

interior b32
_liberum_appendere (
     HtmlParsura* p,
    MateriaNodus* nodus)
{
    redde _liberum_appendere_ad(p, nodus, _vertex_nodus(p));
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
    "", "html", "head", "body", "tbody", "tr", "colgroup", "p", "br"
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

/* Originale elementi (O7c): scopus loci exemplar si adest, aliter
 * ipsum - titulus et attributa exemplaris per id leguntur. */
interior MateriaNodus*
_originale (
    MateriaNodus* elementum)
{
    MateriaNodus* scopus = _referentia(elementum,
        (i32)HTML_ELEMENTUM_EXEMPLAR);

    redde (scopus != NIHIL) ? scopus : elementum;
}

/* Titulus elementi: ex tag apertionis, aut ex loco synthesis
 * (elementum fictum), aut originalis (exemplar, O7c); vacuus si
 * nullum (tag ad EOF scissum numquam: apertura semper adest, sed
 * defensive). */
interior chorda
_titulus_elementi (
    constans MateriaNodus* elementum)
{
    constans MateriaValor* apertura;
    constans MateriaValor* synthesis;
    constans MateriaNodus* originale;
                   chorda  titulus;

    originale  = _referentia(elementum, (i32)HTML_ELEMENTUM_EXEMPLAR);
    si (originale != NIHIL)
    {
        elementum = originale;
    }
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
 * capita h1-h6 inter se, button/select a se ipsis (claudentia NIHIL =
 * titulus proprius; a et nobr per adoption agency, O7c), head et
 * colgroup ab OMNI tag praeter exceptiones (nisi VERUM: tabula =
 * exceptiones). */
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
    g.octeti          = VERUM;
    g.receptor        = (s32)-I;
    redde g;
}

interior ScopiGradus
_scopi_gradus (
    constans HtmlParsura* p,
                     i32  k,
                  chorda  titulus,
                     b32  octeti)
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
    /* O7c: via octetorum - gradus octetis apertus receptor suus est,
     * clausus receptorem inferioris hereditat */
    g.octeti    = octeti;
    g.receptor  = octeti ? (s32)k : infra.receptor;
    redde g;
}

/* Gradus ab 'ab' usque ad verticem RENOVARE (post chirurgiam acervi,
 * O7c: insertio, remotio, clausura octetorum) - campi visibilitatis
 * ex gradu inferiore iterum computati, octeti cuiusque servati. */
interior vacuum
_gradus_renovare (
    HtmlParsura* p,
            i32  ab)
{
    i32 i;

    per (i = ab; i < p->profunditas; i++)
    {
        ScopiGradus* g = (ScopiGradus*)xar_obtinere(p->scopi, i);

        *g = _scopi_gradus(p, i, _titulus_elementi(_apertum(p, i)),
            g->octeti);
    }
}

/* Gradus ab 'ab' ad verticem octetis CLAUDERE: octeti eorum finiti
 * (elementum formans clausum, '</body>'), in DOM aperti manent - nodi
 * novi in receptorem inferiorem cum sede eorum eunt. */
interior vacuum
_octetos_claudere (
    HtmlParsura* p,
            i32  ab)
{
    i32 i;

    per (i = ab; i < p->profunditas; i++)
    {
        ((ScopiGradus*)xar_obtinere(p->scopi, i))->octeti = FALSUM;
    }
    _gradus_renovare(p, ab);
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
    /* body (post '</body>' in acervo DOM manens, O7c) et superiora
     * cadunt: spec 'pop until html' */
    si (k >= ZEPHYRUM)
    {
        p->profunditas = (i32)k;
    }
    p->corpus = NIHIL;
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

/* Parens fovens (spec 'foster parent'): parens DOM table apertae
 * proximae (sedes eius si alibi, aliter pater octetorum - O7c: gradus
 * sub tabula alius esse potest, '<a>' non in scopo e acervo remoto);
 * sine table in acervo (fragmentum contextu tabulae) radix. */
interior MateriaNodus*
_parens_fovens (
    constans HtmlParsura* p)
{
    s32 tabulae = _vertex_gradus(p)->tabulae;
    MateriaNodus* tabula;
    MateriaNodus* sedes;

    si (tabulae < ZEPHYRUM)
    {
        redde p->documentum;
    }
    tabula  = _apertum(p, (i32)tabulae);
    sedes   = _sedes(tabula);
    si (sedes != NIHIL)
    {
        redde sedes;
    }
    redde (tabula->pater != NIHIL) ? tabula->pater : p->documentum;
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
    redde _sedem_ponere(p, elementum, fovens);
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
 * Chirurgia acervi (O7c)
 * ================================================== */

/* Segmentum acervi [k, m) per gradus novos REPONERE (adoption agency:
 * exemplaria pro formantibus intermediis, elementum formans remotum,
 * exemplar eius post bloccum insertum). Gradus superiores per delta
 * moventur, gradus ab k recomputantur, indices listae formantium >= m
 * per delta moventur (indices in [k, m) vocans ipse ponit). */
interior b32
_acervum_reponere (
             HtmlParsura* p,
                     i32  k,
                     i32  m,
    constans GradusNovus* novi,
                     i32  numerus)
{
    s32 delta  = (s32)numerus - (s32)(m - k);
    i32 supra  = p->profunditas - m;
    i32 nova   = (i32)((s32)p->profunditas + delta);
    i32 i;

    dum (xar_numerus(p->acervus) < nova)
    {
        si (   xar_addere(p->acervus) == NIHIL
            || xar_addere(p->scopi)   == NIHIL)
        {
            redde FALSUM;
        }
    }
    si (delta > ZEPHYRUM)
    {
        per (i = supra; i > ZEPHYRUM; i--)
        {
            i32 a = m + i - I;
            i32 b = (i32)((s32)a + delta);

            *(MateriaNodus**)xar_obtinere(p->acervus, b) =
                *(MateriaNodus**)xar_obtinere(p->acervus, a);
            *(ScopiGradus*)xar_obtinere(p->scopi, b) =
                *(ScopiGradus*)xar_obtinere(p->scopi, a);
        }
    }
    alioquin si (delta < ZEPHYRUM)
    {
        per (i = ZEPHYRUM; i < supra; i++)
        {
            i32 a = m + i;
            i32 b = (i32)((s32)a + delta);

            *(MateriaNodus**)xar_obtinere(p->acervus, b) =
                *(MateriaNodus**)xar_obtinere(p->acervus, a);
            *(ScopiGradus*)xar_obtinere(p->scopi, b) =
                *(ScopiGradus*)xar_obtinere(p->scopi, a);
        }
    }
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        *(MateriaNodus**)xar_obtinere(p->acervus, k + i) =
            novi[i].nodus;
        ((ScopiGradus*)xar_obtinere(p->scopi, k + i))->octeti =
            novi[i].octeti;
    }
    p->profunditas = nova;
    _gradus_renovare(p, k);
    per (i = ZEPHYRUM; i < p->formantium; i++)
    {
        Formans* f = (Formans*)xar_obtinere(p->formantia, i);

        si (f->index >= (s32)m)
        {
            f->index = f->index + delta;
        }
    }
    redde VERUM;
}


/* ==================================================
 * Lista formantium activorum (O7c)
 * ================================================== */

/* WHATWG categoria 'formatting' */
hic_manens constans character* constans FORMANTIA[] = {
    "a", "b", "big", "code", "em", "font", "i", "nobr", "s", "small",
    "strike", "strong", "tt", "u"
};
/* elementa quae SIGNUM in listam impellunt (spec) */
hic_manens constans character* constans SIGNANTIA[] = {
    "applet", "object", "marquee", "template", "td", "th", "caption"
};
/* tags apertionis 'in body' quae listam NON reconstruunt (spec:
 * regulae suae sine 'reconstruct the active formatting elements') -
 * cetera reconstruunt */
hic_manens constans character* constans NON_RESTITUENTIA[] = {
    "html", "base", "basefont", "bgsound", "link", "meta", "noframes",
    "script", "style", "template", "title", "body", "frameset",
    "address", "article", "aside", "blockquote", "center", "details",
    "dialog", "dir", "div", "dl", "fieldset", "figcaption", "figure",
    "footer", "header", "hgroup", "main", "menu", "nav", "ol", "p",
    "search", "section", "summary", "ul", "h1", "h2", "h3", "h4", "h5",
    "h6", "pre", "listing", "form", "li", "dd", "dt", "plaintext",
    "table", "hr", "textarea", "iframe", "noembed", "noscript", "rb",
    "rtc", "rp", "rt", "caption", "col", "colgroup", "frame", "head",
    "tbody", "td", "tfoot", "th", "thead", "tr"
};

interior b32
_formans_est (
    chorda titulus)
{
    redde _in_tabula(titulus, FORMANTIA, TABULAE_NUMERUS(FORMANTIA));
}

interior Formans*
_formans (
    constans HtmlParsura* p,
                     i32  i)
{
    redde (Formans*)xar_obtinere(p->formantia, i);
}

/* Formans in acervo vivit? index ratus et nodus idem */
interior b32
_formans_vivus (
     constans HtmlParsura* p,
         constans Formans* f)
{
    redde (b32)(f->index >= ZEPHYRUM && f->index < (s32)p->profunditas
                && _apertum(p, (i32)f->index) == f->elementum);
}

/* Formantem in repositorium scribere (i <= formantium) */
interior b32
_formantem_scribere (
    HtmlParsura* p,
            i32  i,
        Formans  f)
{
    Formans* l;

    si (i < xar_numerus(p->formantia))
    {
        redde xar_ponere(p->formantia, i, &f);
    }
    l = (Formans*)xar_addere(p->formantia);
    si (l == NIHIL)
    {
        redde FALSUM;
    }
    *l = f;
    redde VERUM;
}

/* Signa lapsa purgare: elementum signans e acervo exiit (td, caption,
 * template clausum) - spec 'clear the list up to the last marker',
 * hic PIGRE in usu proximo listae. */
interior vacuum
_formantia_purgare (
    HtmlParsura* p)
{
    dum (p->formantium > ZEPHYRUM)
    {
        s32 i;

        per (i = (s32)p->formantium - I; i >= ZEPHYRUM; i--)
        {
            si (_formans(p, (i32)i)->signum)
            {
                frange;
            }
        }
        si (i < ZEPHYRUM || _formans_vivus(p, _formans(p, (i32)i)))
        {
            redde;
        }
        p->formantium = (i32)i;
    }
}

/* Index in lista elementi formantis ULTIMI tituli dati post signum
 * ultimum; -I si nullum. */
interior s32
_formantia_invenire (
    HtmlParsura* p,
         chorda  titulus)
{
    s32 i;

    _formantia_purgare(p);
    per (i = (s32)p->formantium - I; i >= ZEPHYRUM; i--)
    {
        constans Formans* f = _formans(p, (i32)i);

        si (f->signum)
        {
            redde (s32)-I;
        }
        si (_tituli_pares(_titulus_elementi(f->elementum), titulus))
        {
            redde i;
        }
    }
    redde (s32)-I;
}

/* Index in lista nodi dati (etiam ante signum); -I si nullum */
interior s32
_formantia_index_nodi (
     constans HtmlParsura* p,
    constans MateriaNodus* nodus)
{
    s32 i;

    per (i = (s32)p->formantium - I; i >= ZEPHYRUM; i--)
    {
        constans Formans* f = _formans(p, (i32)i);

        si (!f->signum && f->elementum == nodus)
        {
            redde i;
        }
    }
    redde (s32)-I;
}

interior vacuum
_formantia_removere (
    HtmlParsura* p,
            i32  i)
{
    i32 j;

    per (j = i + I; j < p->formantium; j++)
    {
        *_formans(p, j - I) = *_formans(p, j);
    }
    p->formantium = p->formantium - I;
}

interior b32
_formantia_inserere (
    HtmlParsura* p,
            i32  i,
        Formans  f)
{
    s32 j;

    si (!_formantem_scribere(p, p->formantium, f))
    {
        redde FALSUM;
    }
    per (j = (s32)p->formantium; j > (s32)i; j--)
    {
        *_formans(p, (i32)j) = *_formans(p, (i32)j - I);
    }
    *_formans(p, i)  = f;
    p->formantium    = p->formantium + I;
    redde VERUM;
}

/* Attributa paria (arca Noe: titulus, spatium, attributa eadem) - per
 * originalia; tituli litteris neglectis, valores octetim. */
interior b32
_attributa_paria (
    constans MateriaNodus* a,
    constans MateriaNodus* b)
{
    constans MateriaValor* lista_a = &a->loci[HTML_ELEMENTUM_ATTRIBUTA];
    constans MateriaValor* lista_b = &b->loci[HTML_ELEMENTUM_ATTRIBUTA];
                      i32  numerus_a;
                      i32  numerus_b;
                      i32  i;

    numerus_a = (lista_a->genus == MATERIA_VALOR_LISTA)
        ? materia_valor_lista_numerus(*lista_a) : ZEPHYRUM;
    numerus_b = (lista_b->genus == MATERIA_VALOR_LISTA)
        ? materia_valor_lista_numerus(*lista_b) : ZEPHYRUM;
    si (numerus_a != numerus_b)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < numerus_a; i++)
    {
        constans MateriaValor* valor_a =
            materia_valor_lista_obtinere(*lista_a,
            i);
        constans MateriaValor* valor_b =
            materia_valor_lista_obtinere(*lista_b,
            i);
               MateriaToken* lexema_a;
               MateriaToken* lexema_b;

        si (   valor_a        == NIHIL || valor_b == NIHIL
            || valor_a->genus != MATERIA_VALOR_NODUS
            || valor_b->genus != MATERIA_VALOR_NODUS)
        {
            redde FALSUM;
        }
        lexema_a = _tok(valor_a->datum.nodus,
            (i32)HTML_ATTRIBUTUM_TOK_NOMEN);
        lexema_b = _tok(valor_b->datum.nodus,
            (i32)HTML_ATTRIBUTUM_TOK_NOMEN);
        si ((lexema_a == NIHIL) != (lexema_b == NIHIL))
        {
            redde FALSUM;
        }
        si (   lexema_a != NIHIL
            && !_tituli_pares(lexema_a->valor, lexema_b->valor))
        {
            redde FALSUM;
        }
        lexema_a = _tok(valor_a->datum.nodus,
            (i32)HTML_ATTRIBUTUM_TOK_VALOR);
        lexema_b = _tok(valor_b->datum.nodus,
            (i32)HTML_ATTRIBUTUM_TOK_VALOR);
        si ((lexema_a == NIHIL) != (lexema_b == NIHIL))
        {
            redde FALSUM;
        }
        si (   lexema_a != NIHIL
            && !chorda_aequalis(lexema_a->valor, lexema_b->valor))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* Elementum formans in listam impellere cum ARCA NOE (spec 'push onto
 * the list of active formatting elements'): si tria eiusdem tituli et
 * attributorum post signum ultimum iam adsunt, primum eorum cadit. */
interior b32
_formantia_impellere (
     HtmlParsura* p,
    MateriaNodus* elementum,
             i32  index)
{
         Formans  f;
          chorda  titulus    = _titulus_elementi(elementum);
    MateriaNodus* originale  = _originale(elementum);
             i32  paria      = ZEPHYRUM;
             s32  primum     = (s32)-I;
             s32  i;

    _formantia_purgare(p);
    per (i = (s32)p->formantium - I; i >= ZEPHYRUM; i--)
    {
        constans Formans* g = _formans(p, (i32)i);

        si (g->signum)
        {
            frange;
        }
        si (   _tituli_pares(_titulus_elementi(g->elementum), titulus)
            && _attributa_paria(_originale(g->elementum), originale))
        {
            paria   = paria + I;
            primum  = i;
        }
    }
    si (paria >= III)
    {
        _formantia_removere(p, (i32)primum);
    }
    f.elementum  = elementum;
    f.index      = (s32)index;
    f.signum     = FALSUM;
    si (!_formantem_scribere(p, p->formantium, f))
    {
        redde FALSUM;
    }
    p->formantium = p->formantium + I;
    redde VERUM;
}

interior b32
_formantia_signum (
     HtmlParsura* p,
    MateriaNodus* elementum,
             i32  index)
{
    Formans f;

    f.elementum  = elementum;
    f.index      = (s32)index;
    f.signum     = VERUM;
    si (!_formantem_scribere(p, p->formantium, f))
    {
        redde FALSUM;
    }
    p->formantium = p->formantium + I;
    redde VERUM;
}


/* ==================================================
 * Exemplar, insertio, reconstructio (O7c)
 * ================================================== */

/* Exemplar: elementum formans iterum apertum (spec 'create an element
 * for the token for which node was created') - nodus generis
 * elementum sine lexematibus, locus exemplar ad originale VERUM;
 * titulus et attributa per id leguntur, emissor nihil scribit. Nondum
 * appensum. */
interior MateriaNodus*
_exemplar_fingere (
     HtmlParsura* p,
    MateriaNodus* elementum)
{
    MateriaNodus* e;

    e = materia_nodus_creare(p->piscina, (s32)HTML_GENUS_ELEMENTUM,
        _loci_numerus((s32)HTML_GENUS_ELEMENTUM));
    si (e == NIHIL)
    {
        redde NIHIL;
    }
    si (!materia_nodus_ponere(e, (i32)HTML_ELEMENTUM_EXEMPLAR,
            materia_valor_referentia(_originale(elementum)),
            MATERIA_LOCUS_REFERENTIA))
    {
        redde NIHIL;
    }
    redde e;
}

/* Locum 'praecedens' ponere (frater DOM prior) aut revocare (NIHIL:
 * absentia scripta, locus nondum scriptus intactus). */
interior b32
_praecedentem_ponere (
    MateriaNodus* nodus,
    MateriaNodus* prior)
{
    si (   prior == NIHIL
        && nodus->loci[HTML_ELEMENTUM_PRAECEDENS].genus
               == MATERIA_VALOR_NIHIL)
    {
        redde VERUM;
    }
    redde _locum_scribere(nodus, (i32)HTML_ELEMENTUM_PRAECEDENS,
        materia_valor_referentia(prior), MATERIA_LOCUS_REFERENTIA);
}

/* Elementum (verum aut exemplar) in arborem INSERERE (spec 'insert an
 * HTML element'): sedes fovens si locus insertionis fovetur (input:
 * decisio pendens usque ad attributa), contentum capitis post head in
 * head, aliter parens octetorum cum sede automatica (vertex DOM
 * octetis clausus); impulsum si iussum. */
interior b32
_elementum_inserere (
     HtmlParsura* p,
    MateriaNodus* elementum,
          chorda  titulus,
             b32  impellendum)
{
    b32 fovendum = (b32)(_fovendum(p)
                         && !_in_tabula(titulus, TABULAE_ACCEPTA,
                             TABULAE_NUMERUS(TABULAE_ACCEPTA)));

    si (fovendum && _titulus_est(titulus, "input"))
    {
        p->fovens = _parens_fovens(p);
    }
    alioquin si (fovendum)
    {
        si (!_sedem_ponere(p, elementum, _parens_fovens(p)))
        {
            redde FALSUM;
        }
    }
    alioquin si (_post_caput(p, titulus))
    {
        si (!_sedem_ponere(p, elementum, p->caput))
        {
            redde FALSUM;
        }
    }
    si (!_liberum_appendere(p, elementum))
    {
        redde FALSUM;
    }
    si (impellendum && !_impellere(p, elementum, titulus))
    {
        redde FALSUM;
    }
    redde VERUM;
}

/* Reconstructio listae formantium activorum (spec 'reconstruct the
 * active formatting elements'): elementa formantia in lista sed non
 * in acervo (per clausuram alienam cadentia: '<p><b>x</p>y') iterum
 * aperiuntur ut exemplaria - post signum ultimum aut elementum vivum
 * ultimum, ordine listae; exemplar insertum (fotum si locus fovetur)
 * et impulsum, formans substitutus. */
interior b32
_formantia_restituere (
    HtmlParsura* p)
{
    s32 i;

    _formantia_purgare(p);
    si (p->formantium == ZEPHYRUM)
    {
        redde VERUM;
    }
    i = (s32)p->formantium - I;
    {
        constans Formans* f = _formans(p, (i32)i);

        si (f->signum || _formans_vivus(p, f))
        {
            redde VERUM;
        }
    }
    dum (i > ZEPHYRUM)
    {
        constans Formans* f;

        i = i - I;
        f = _formans(p, (i32)i);
        si (f->signum || _formans_vivus(p, f))
        {
            i = i + I;
            frange;
        }
    }
    per (; i < (s32)p->formantium; i++)
    {
             Formans* g = _formans(p, (i32)i);
        MateriaNodus* e = _exemplar_fingere(p, g->elementum);

        si (e == NIHIL)
        {
            redde FALSUM;
        }
        si (!_elementum_inserere(p, e, _titulus_elementi(e), VERUM))
        {
            redde FALSUM;
        }
        g->elementum  = e;
        g->index      = (s32)(p->profunditas - I);
    }
    redde VERUM;
}

/* An tag apertionis listam reconstruat (spec: regulae 'in body' fere
 * omnes praeter NON_RESTITUENTIA; numquam in select, in contento
 * alieno, aut extra corpus - lista tum vacua) */
interior b32
_restituendum (
    constans HtmlParsura* p,
                  chorda  titulus)
{
    constans ScopiGradus* vertex = _vertex_gradus(p);

    redde (b32)(vertex->liberorum == HTML_ALIENUM_NULLUM
                && vertex->selectum < ZEPHYRUM
                && !_in_tabula(titulus, NON_RESTITUENTIA,
                    TABULAE_NUMERUS(NON_RESTITUENTIA)));
}


/* ==================================================
 * Adoption agency (O7c)
 * ================================================== */

/* Elementum 'special' primum SUPRA gradum k (spec 'furthest block');
 * -I si nullum. */
interior s32
_bloccum_ultimum (
    constans HtmlParsura* p,
                     i32  k)
{
    i32 i;

    per (i = k + I; i < p->profunditas; i++)
    {
        si (_in_tabula(_titulus_aperti(p, i), SPECIALIA,
                TABULAE_NUMERUS(SPECIALIA)))
        {
            redde (s32)i;
        }
    }
    redde (s32)-I;
}

/* Gradus k in scopo (spec 'has an element in scope'): limes scopi
 * nullus supra eum. */
interior b32
_gradus_in_scopo (
    constans HtmlParsura* p,
                     i32  k)
{
    i32 i;

    per (i = k + I; i < p->profunditas; i++)
    {
        si (_limes_scopi_est(_titulus_aperti(p, i)))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* Lexema clausurae in elementum gradus k ponere et acervum ad k
 * purgare: elementum octetis apertum clausuram accipit (superiora
 * implicite clausa, H4); octetis clausum (in DOM solum apertum, O7c)
 * eam accipere non potest - octeti in malum eunt post purgationem. */
interior b32
_clausuram_ponere (
     HtmlParsura* p,
             i32  k,
    MateriaToken* token)
{
    b32 octeti = ((constans ScopiGradus*)xar_obtinere(p->scopi,
        k))->octeti;
    MateriaNodus* elementum = _apertum(p, k);

    p->profunditas = k;
    si (!octeti)
    {
        redde _malum_addere(p, token);
    }
    p->clausura = elementum;
    redde materia_nodus_ponere(elementum,
        (i32)HTML_ELEMENTUM_TOK_CLAUSURA, materia_valor_token(token),
        MATERIA_LOCUS_TOKEN);
}

/* 'any other end tag' (WHATWG 'in body'): ambulatio ab vertice,
 * titulus par claudit, elementum 'special' ante id sistit (malum). */
interior b32
_clausuram_generalem_tractare (
     HtmlParsura* p,
    MateriaToken* token,
          chorda  titulus)
{
    s32 k = _apertum_generale_invenire(p, titulus);

    si (k < ZEPHYRUM)
    {
        redde _malum_addere(p, token);
    }
    redde _clausuram_ponere(p, (i32)k, token);
}

/* Liberos DOM blocci in exemplar INVOLVERE (spec gradus XV-XVII:
 * 'take all of the child nodes of furthest block and append them to
 * the element created', deinde exemplar in bloccum) - chirurgia
 * arboris octetorum octetis TUTA: exemplar nihil emittit, liberi
 * ordinem servant. Liberi octetorum cum sede aliena (foti) in blocco
 * manent; nodi alibi quorum sedes bloccum est in exemplar spectant. */
interior b32
_liberos_involvere (
     HtmlParsura* p,
    MateriaNodus* bloccum,
    MateriaNodus* exemplar)
{
    MateriaValor* liberi    = &bloccum->loci[HTML_ELEMENTUM_LIBERI];
    MateriaValor  manentes  = materia_valor_lista_nova(p->piscina);
             i32  n;
             i32  i;

    n = (liberi->genus == MATERIA_VALOR_LISTA)
        ? materia_valor_lista_numerus(*liberi) : ZEPHYRUM;
    per (i = ZEPHYRUM; i < n; i++)
    {
        MateriaValor* v = materia_valor_lista_obtinere(*liberi, i);

        si (v == NIHIL)
        {
            perge;
        }
        si (   v->genus               == MATERIA_VALOR_NODUS
            && _sedes(v->datum.nodus) != NIHIL)
        {
            manentes = materia_valor_lista_appendere(p->piscina,
                manentes, *v);
            perge;
        }
        si (!materia_nodus_appendere(p->piscina, exemplar,
                (i32)HTML_ELEMENTUM_LIBERI, *v,
                MATERIA_LOCUS_LISTA_NODUS))
        {
            redde FALSUM;
        }
    }
    manentes = materia_valor_lista_appendere(p->piscina, manentes,
        materia_valor_nodus(exemplar));
    si (!_locum_scribere(bloccum, (i32)HTML_ELEMENTUM_LIBERI, manentes,
            MATERIA_LOCUS_LISTA_NODUS))
    {
        redde FALSUM;
    }
    n = xar_numerus(p->sedentes);
    per (i = ZEPHYRUM; i < n; i++)
    {
        MateriaNodus* s = *(MateriaNodus**)xar_obtinere(p->sedentes, i);

        si (   _sedes(s) == bloccum
            && !_sedem_ponere(p, s, exemplar))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* An insertio cum avo ut scopo (spec 'override target') foveatur:
 * modus tabulae vivus et avus table/tbody/tfoot/thead/tr HTML. */
interior b32
_avus_fovendus (
    constans HtmlParsura* p,
                     i32  k)
{
    constans ScopiGradus* g;

    si (   k                                == ZEPHYRUM
        || _vertex_gradus(p)->modus_tabulae == TABULAE_NULLUS)
    {
        redde FALSUM;
    }
    g = (constans ScopiGradus*)xar_obtinere(p->scopi, k - I);
    redde (b32)(g->liberorum == HTML_ALIENUM_NULLUM
                && _in_tabula(_titulus_aperti(p, k - I), FOVENTIA,
                    TABULAE_NUMERUS(FOVENTIA)));
}

/* ADOPTION AGENCY (WHATWG 'in body', tag clausurae elementi formantis;
 * etiam '<a>' cum a in lista et '<nobr>' cum nobr in scopo - token
 * NIHIL). In acervo DOM currit ut in spec; arbor octetorum intacta
 * praeter exemplaria (nodi sine lexematibus) et involutionem: nodus
 * in avum motus sedem (parentem DOM novum) et praecedentem (fratrem
 * priorem = elementum formans) accipit, aut sedem foventem si avus
 * fovetur. Lexema clausurae in elementum formans cadit si octetis
 * apertum (superiora octetis clauduntur), aliter in malum. */
interior b32
_adoptionem_agere (
     HtmlParsura* p,
    MateriaToken* token,
          chorda  titulus)
{
    i32 gyrus;

    /* gradus I: vertex ipse eiusdem tituli, non in lista -> clausura */
    si (p->profunditas > ZEPHYRUM)
    {
        i32 vertex = p->profunditas - I;

        si (   _tituli_pares(_titulus_aperti(p, vertex), titulus)
            && _formantia_index_nodi(p, _apertum(p, vertex)) < ZEPHYRUM)
        {
            si (token != NIHIL)
            {
                redde _clausuram_ponere(p, vertex, token);
            }
            p->profunditas = vertex;
            redde VERUM;
        }
    }
    per (gyrus = ZEPHYRUM; gyrus < VIII; gyrus++)
    {
                  s32 index_formantis = _formantia_invenire(p,
                      titulus);
              Formans* f;
        MateriaNodus*  formans;
                  i32  k;
                  s32  bloccum;
        MateriaNodus*  bloccum_nodus;
        MateriaNodus*  avus;
        MateriaNodus*  receptor;
                  s32  signum;
        MateriaNodus*  ultimus;
        MateriaNodus*  exemplar;
                 Xar*  catena;
                 Xar*  novi;
                  i32  gyrus_interior;
                  b32  octeti;
                  s32  i;

        si (index_formantis < ZEPHYRUM)
        {
            redde (token != NIHIL)
                ? _clausuram_generalem_tractare(p, token, titulus)
                : VERUM;
        }
        f        = _formans(p, (i32)index_formantis);
        formans  = f->elementum;
        si (!_formans_vivus(p, f))
        {
            _formantia_removere(p, (i32)index_formantis);
            redde (token != NIHIL) ? _malum_addere(p, token) : VERUM;
        }
        k = (i32)f->index;
        si (!_gradus_in_scopo(p, k))
        {
            redde (token != NIHIL) ? _malum_addere(p, token) : VERUM;
        }
        bloccum = _bloccum_ultimum(p, k);
        si (bloccum < ZEPHYRUM)
        {
            _formantia_removere(p, (i32)index_formantis);
            si (token != NIHIL)
            {
                redde _clausuram_ponere(p, k, token);
            }
            p->profunditas = k;
            redde VERUM;
        }
        /* lexema clausurae (gyro primo): in elementum formans si
         * octetis apertum - superiora octetis clausa; aliter malum.
         * Sine lexemate ('<a>' iterum): clausura implicita, superiora
         * aeque clausa */
        octeti = ((constans ScopiGradus*)xar_obtinere(p->scopi,
            k))->octeti;
        si (token != NIHIL)
        {
            si (octeti)
            {
                p->clausura = formans;
                si (!materia_nodus_ponere(formans,
                        (i32)HTML_ELEMENTUM_TOK_CLAUSURA,
                        materia_valor_token(token),
                        MATERIA_LOCUS_TOKEN))
                {
                    redde FALSUM;
                }
            }
            alioquin si (!_malum_addere(p, token))
            {
                redde FALSUM;
            }
            token = NIHIL;
        }
        si (octeti)
        {
            i32 j;

            per (j = k + I; j < p->profunditas; j++)
            {
                ((ScopiGradus*)xar_obtinere(p->scopi, j))->octeti =
                    FALSUM;
            }
        }
        avus      = (k > ZEPHYRUM) ? _apertum(p, k - I) : p->documentum;
        receptor  = p->documentum;
        si (k > ZEPHYRUM)
        {
            constans ScopiGradus* g =
                (constans ScopiGradus*)xar_obtinere(
                p->scopi, k - I);

            si (g->receptor >= ZEPHYRUM)
            {
                receptor = _apertum(p, (i32)g->receptor);
            }
        }
        signum = index_formantis;
        bloccum_nodus = _apertum(p, (i32)bloccum);
        ultimus = bloccum_nodus;
        catena = xar_creare(p->piscina, magnitudo(MateriaNodus*));
        novi = xar_creare(p->piscina, magnitudo(GradusNovus));
        si (catena == NIHIL || novi == NIHIL)
        {
            redde FALSUM;
        }
        /* gyrus interior: a blocco deorsum ad elementum formans */
        gyrus_interior = ZEPHYRUM;
        per (i = bloccum - I; i > (s32)k; i--)
        {
            MateriaNodus* nodus = _apertum(p, (i32)i);
                     s32  index_listae;
            MateriaNodus* e;
            MateriaNodus** l;

            gyrus_interior  = gyrus_interior + I;
            index_listae    = _formantia_index_nodi(p, nodus);
            si (gyrus_interior > III && index_listae >= ZEPHYRUM)
            {
                _formantia_removere(p, (i32)index_listae);
                si (index_listae < index_formantis)
                {
                    index_formantis = index_formantis - I;
                }
                si (index_listae < signum)
                {
                    signum = signum - I;
                }
                index_listae = (s32)-I;
            }
            si (index_listae < ZEPHYRUM)
            {
                perge;   /* e acervo exit (non in novis) */
            }
            e = _exemplar_fingere(p, nodus);
            si (e == NIHIL)
            {
                redde FALSUM;
            }
            /* ultimus in exemplar (DOM): bloccum per sedem, exemplar
             * prius ut liber octetorum (vacuum, nihil emittit) */
            si (ultimus == bloccum_nodus)
            {
                si (   !_sedem_ponere(p, ultimus, e)
                    || !_praecedentem_ponere(ultimus, NIHIL))
                {
                    redde FALSUM;
                }
                signum = index_listae + I;
            }
            alioquin si (!materia_nodus_appendere(p->piscina, e,
                         (i32)HTML_ELEMENTUM_LIBERI,
                         materia_valor_nodus(ultimus),
                         MATERIA_LOCUS_LISTA_NODUS))
            {
                redde FALSUM;
            }
            _formans(p, (i32)index_listae)->elementum = e;
            ultimus = e;
            l = (MateriaNodus**)xar_addere(catena);
            si (l == NIHIL)
            {
                redde FALSUM;
            }
            *l = e;
        }
        /* gradus XIV: ultimus in avum (fotum si avus fovetur) */
        si (_avus_fovendus(p, k))
        {
            si (   !_sedem_ponere(p, ultimus, _parens_fovens(p))
                || !_praecedentem_ponere(ultimus, NIHIL))
            {
                redde FALSUM;
            }
        }
        alioquin si (ultimus == bloccum_nodus || receptor != avus)
        {
            si (   !_sedem_ponere(p, ultimus, avus)
                || !_praecedentem_ponere(ultimus, formans))
            {
                redde FALSUM;
            }
        }
        si (ultimus != bloccum_nodus)
        {
            i32 locus = (receptor->genus == (s32)HTML_GENUS_DOCUMENTUM)
                      ? (i32)HTML_DOCUMENTUM_LIBERI
                      : (i32)HTML_ELEMENTUM_LIBERI;

            si (!materia_nodus_appendere(p->piscina, receptor, locus,
                    materia_valor_nodus(ultimus),
                    MATERIA_LOCUS_LISTA_NODUS))
            {
                redde FALSUM;
            }
        }
        /* gradus XV-XVII: exemplar elementi formantis liberos blocci
         * involvit */
        exemplar = _exemplar_fingere(p, formans);
        si (   exemplar == NIHIL
            || !_liberos_involvere(p, bloccum_nodus, exemplar))
        {
            redde FALSUM;
        }
        /* gradus XVIII: lista - formans remotum, exemplar ad signum */
        _formantia_removere(p, (i32)index_formantis);
        si (index_formantis < signum)
        {
            signum = signum - I;
        }
        {
            Formans formans_novus;

            formans_novus.elementum  = exemplar;
            formans_novus.index      = (s32)-I;
            formans_novus.signum     = FALSUM;
            si (!_formantia_inserere(p, (i32)signum, formans_novus))
            {
                redde FALSUM;
            }
        }
        /* gradus XIX: acervus - [k, bloccum] -> catena (ordine acervi:
         * catena inversa), bloccum, exemplar; superiora manent */
        {
            i32 catenae = xar_numerus(catena);
            b32 octeti_blocci = ((constans ScopiGradus*)xar_obtinere(
                p->scopi, (i32)bloccum))->octeti;
            i32 j;

            per (j = catenae; j > ZEPHYRUM; j--)
            {
                GradusNovus* g = (GradusNovus*)xar_addere(novi);

                si (g == NIHIL)
                {
                    redde FALSUM;
                }
                g->nodus = *(MateriaNodus**)xar_obtinere(catena, j
                    - I);
                g->octeti = VERUM;
            }
            {
                GradusNovus* g = (GradusNovus*)xar_addere(novi);

                si (g == NIHIL)
                {
                    redde FALSUM;
                }
                g->nodus   = bloccum_nodus;
                g->octeti  = octeti_blocci;
                g          = (GradusNovus*)xar_addere(novi);
                si (g == NIHIL)
                {
                    redde FALSUM;
                }
                g->nodus   = exemplar;
                g->octeti  = octeti_blocci;
            }
            si (!_acervum_reponere(p, k, (i32)bloccum + I,
                    (constans GradusNovus*)xar_obtinere(novi, ZEPHYRUM),
                    xar_numerus(novi)))
            {
                redde FALSUM;
            }
            per (j = ZEPHYRUM; j < xar_numerus(novi); j++)
            {
                constans GradusNovus* g = (constans GradusNovus*)
                    xar_obtinere(novi, j);
                s32 index_listae = _formantia_index_nodi(p, g->nodus);

                si (index_listae >= ZEPHYRUM)
                {
                    _formans(p, (i32)index_listae)->index = (s32)(k
                        + j);
                }
            }
        }
    }
    redde VERUM;
}

/* '</p>' sine p in scopo button et '</br>' (spec: p fictum insertum
 * et clausum; '</br>' ut '<br>'): elementum fictum in locum insertionis
 * (fotum si fovetur), clausura vera in ficto. */
interior b32
_fictum_claudere (
      HtmlParsura* p,
    HtmlSynthesis  synthesis,
     MateriaToken* token)
{
    MateriaNodus* e;

    e = materia_nodus_creare(p->piscina, (s32)HTML_GENUS_ELEMENTUM,
        _loci_numerus((s32)HTML_GENUS_ELEMENTUM));
    si (e == NIHIL)
    {
        redde FALSUM;
    }
    si (!materia_nodus_ponere(e, (i32)HTML_ELEMENTUM_SYNTHESIS,
            materia_valor_index((s32)synthesis), MATERIA_LOCUS_INDEX))
    {
        redde FALSUM;
    }
    si (!_elementum_inserere(p, e, _titulus_elementi(e), FALSUM))
    {
        redde FALSUM;
    }
    p->clausura = e;
    redde materia_nodus_ponere(e, (i32)HTML_ELEMENTUM_TOK_CLAUSURA,
        materia_valor_token(token), MATERIA_LOCUS_TOKEN);
}


/* ==================================================
 * Lexemata singula: tags
 * ================================================== */

/* Index in acervo nodi dati; -I si nullus */
interior s32
_apertum_invenire_nodum (
     constans HtmlParsura* p,
    constans MateriaNodus* nodus)
{
    i32 k;

    per (k = p->profunditas; k > ZEPHYRUM; k--)
    {
        si (_apertum(p, k - I) == nodus)
        {
            redde (s32)(k - I);
        }
    }
    redde (s32)-I;
}

interior b32
_aperturam_tractare (
     HtmlParsura* p,
    MateriaToken* token)
{
    MateriaNodus* elementum;
          chorda  titulus;
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
    /* O7c: '<a>' cum a in lista post signum: adoption agency, deinde
     * a illud e lista et acervo (spec); reconstructio listae ante
     * insertionem; '<nobr>' in scopo: adoption agency inter
     * reconstructiones duas */
    si (_vertex_gradus(p)->liberorum == HTML_ALIENUM_NULLUM)
    {
        si (_titulus_est(titulus, "a"))
        {
            s32 index_formantis = _formantia_invenire(p, titulus);

            si (index_formantis >= ZEPHYRUM)
            {
                MateriaNodus* prior =
                    _formans(p, (i32)index_formantis)->elementum;
                         s32 index_listae;

                si (!_adoptionem_agere(p, NIHIL, titulus))
                {
                    redde FALSUM;
                }
                index_listae = _formantia_index_nodi(p, prior);
                si (index_listae >= ZEPHYRUM)
                {
                    _formantia_removere(p, (i32)index_listae);
                }
                index_listae = _apertum_invenire_nodum(p, prior);
                si (   index_listae >= ZEPHYRUM
                    && !_acervum_reponere(p, (i32)index_listae,
                    (i32)index_listae + I,
                    NIHIL,
                        ZEPHYRUM))
                {
                    redde FALSUM;
                }
            }
        }
        si (_restituendum(p, titulus) && !_formantia_restituere(p))
        {
            redde FALSUM;
        }
        si (   _titulus_est(titulus, "nobr")
            && _apertum_in_scopo_invenire(p, titulus) >= ZEPHYRUM)
        {
            si (   !_adoptionem_agere(p, NIHIL, titulus)
                || !_formantia_restituere(p))
            {
                redde FALSUM;
            }
        }
    }
    /* O7a: partes tabulae quas spec fingit (tbody/tr/colgroup) */
    si (!_partes_tabulae_fingere(p, titulus))
    {
        redde FALSUM;
    }
    /* O7b: form in tabula inseritur nec impellitur (spec 'in table'
     * form: insert, pop); foster parenting in _elementum_inserere */
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
    /* vacuum numquam aperitur: non impellitur; tag eius tamen
     * pendens manet (attributa, '>') */
    si (!_elementum_inserere(p, elementum, titulus,
            (b32)(!_vacuum_est(titulus) && !forma_tabulae)))
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
    /* O7c: lista formantium - elementum formans HTML impulsum in
     * listam (arca Noe), signantia signum impellunt */
    si (   p->profunditas > ZEPHYRUM
        && _apertum(p, p->profunditas - I) == elementum
        && _vertex_gradus(p)->proprium     == HTML_ALIENUM_NULLUM)
    {
        si (_formans_est(titulus))
        {
            si (!_formantia_impellere(p, elementum, p->profunditas - I))
            {
                redde FALSUM;
            }
        }
        alioquin si (_in_tabula(titulus, SIGNANTIA,
                         TABULAE_NUMERUS(SIGNANTIA)))
        {
            si (!_formantia_signum(p, elementum, p->profunditas - I))
            {
                redde FALSUM;
            }
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
        vertex = _vertex_gradus(p);
    }
    /* O7c (contentum HTML, in corpore aut in template - modi
     * involucrorum et frameset '</p>' neglegunt): '</p>' sine p in
     * scopo button -> p fictum clausum; '</br>' -> br fictum (spec: ut
     * '<br>', reconstructio); elementum formans -> adoption agency */
    si (   vertex->proprium == HTML_ALIENUM_NULLUM
        && !vertex->intra_compagem && !p->compages_visa)
    {
        si (   _titulus_est(titulus, "p") && vertex->p < ZEPHYRUM
            && (p->modus >= MODUS_IN_CORPORE
            || vertex->templi >= ZEPHYRUM))
        {
            redde _fictum_claudere(p, HTML_SYNTHESIS_PARAGRAPHUS,
                token);
        }
        si (_titulus_est(titulus, "br"))
        {
            si (   _restituendum(p, titulus)
                && !_formantia_restituere(p))
            {
                redde FALSUM;
            }
            redde _fictum_claudere(p, HTML_SYNTHESIS_FRACTURA, token);
        }
        si (_formans_est(titulus))
        {
            redde _adoptionem_agere(p, token, titulus);
        }
    }
    /* '</body>' / '</html>' (O7c): acervus DOM MANET (spec 'after
     * body': modus solus mutatur), octeti ab elemento clauduntur -
     * contentum posterius in verticem DOM per sedem; elementum iam
     * octetis clausum: malum */
    si (_titulus_est(titulus, "body") || _titulus_est(titulus, "html"))
    {
        constans ScopiGradus* g;

        k = _apertum_invenire(p, titulus);
        si (k < ZEPHYRUM)
        {
            redde _malum_addere(p, token);
        }
        g = (constans ScopiGradus*)xar_obtinere(p->scopi, (i32)k);
        si (!g->octeti)
        {
            redde _malum_addere(p, token);
        }
        p->clausura = _apertum(p, (i32)k);
        si (!materia_nodus_ponere(p->clausura,
                (i32)HTML_ELEMENTUM_TOK_CLAUSURA,
                materia_valor_token(token), MATERIA_LOCUS_TOKEN))
        {
            redde FALSUM;
        }
        _octetos_claudere(p, (i32)k);
        /* body clausum: 'after body', html clausum: 'after after body'
         * (O7b) */
        si (   p->modus == MODUS_IN_CORPORE
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
        redde VERUM;
    }
    si (_titulus_est(titulus, "p"))
    {
        k = vertex->p;   /* in scopo button (O2b-5) */
    }
    alioquin si (_in_tabula(titulus, CLAUSURAE_PROPRIAE,
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
    /* head clausum: modus 'after head' (O7a) */
    si (p->modus == MODUS_IN_CAPITE && _titulus_est(titulus, "head"))
    {
        p->modus = MODUS_POST_CAPUT;
    }
    redde _clausuram_ponere(p, (i32)k, token);
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
     * clauditur, spec 'in column group'); post body sedes automatica
     * (vertex DOM octetis clausus, O7c). O7c: reconstructio listae
     * formantium ante textum (etiam album, praeter album in tabula -
     * spec 'in table text' album sine reconstructione), in contento
     * HTML extra select. */
    si (   genus == (s32)HTML_GENUS_TEXTUS
        || genus == (s32)HTML_GENUS_REFERENTIA)
    {
        constans ScopiGradus* vertex;

        si (   !albus && p->profunditas > ZEPHYRUM
            && _titulus_est(_titulus_verticis(p), "colgroup"))
        {
            p->profunditas = p->profunditas - I;
        }
        vertex = _vertex_gradus(p);
        si (   vertex->liberorum == HTML_ALIENUM_NULLUM
            && vertex->selectum < ZEPHYRUM
            && !(vertex->intra_compagem || p->compages_visa)
            && (!albus || !_fovendum(p))
            && !_formantia_restituere(p))
        {
            redde FALSUM;
        }
        si (!albus && _fovendum(p))
        {
            si (!_sedem_ponere(p, folium, _parens_fovens(p)))
            {
                redde FALSUM;
            }
        }
        si (!albus && p->modus >= MODUS_POST_CORPUS)
        {
            p->corpus_iterum = VERUM;
        }
    }
    alioquin si (   genus    == (s32)HTML_GENUS_COMMENTARIUM
                 && p->modus >= MODUS_POST_CORPUS && !p->corpus_iterum)
    {
        /* spec 'after body': commentarium in html; 'after after body':
         * in documentum - non in verticem DOM */
        redde _liberum_appendere_ad(p, folium,
            (p->modus == MODUS_POST_CORPUS && p->radix != NIHIL)
                ? p->radix : p->documentum);
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

    p.acervus    = xar_creare(piscina, magnitudo(MateriaNodus*));
    p.scopi      = xar_creare(piscina, magnitudo(ScopiGradus));
    p.formantia  = xar_creare(piscina, magnitudo(Formans));
    p.sedentes   = xar_creare(piscina, magnitudo(MateriaNodus*));
    p.documentum = materia_nodus_creare(piscina,
        (s32)HTML_GENUS_DOCUMENTUM, (i32)II);
    si (   p.acervus    == NIHIL || p.scopi == NIHIL
        || p.documentum == NIHIL
        || p.formantia  == NIHIL || p.sedentes == NIHIL)
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
