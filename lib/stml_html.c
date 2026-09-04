/* stml_html.c - emissio STML -> HTML (spec instrumenti par. 5)
 *
 * Emissor arborem POST catenam ambulat et solum discrepantias
 * syntacticas tractat - tagi auctoris sunt, validitas HTML numquam
 * iudicatur (singularia gemina emittuntur ut scripta, decretum
 * Franis 2026-09-01).
 *
 * LEX SPATII ALBI (par. 5.9, gravissima): spatium album cum linea
 * nova NON in nodis TEXTUS iacet sed in triviis (spatia_ante/post/
 * clausurae). Emissor qui liberos solos ambulat 'x y' in 'xy'
 * contrahit. Ergo: ante liberum quemque spatia_ante, post eum
 * spatia_post; interius elementi ante tag claudens spatia_clausurae
 * (in documento = cauda plagulae).
 *
 * EFFUGIUM DUPLEX (par. 5.4): textus in arbore DECOCTUS -> evade
 * ('&' '<' '>'); attributa inscripta CRUDA -> octeti ut sunt;
 * elementa attributorum decocta -> evade ('&' '"').
 * stml_attributum_capere HIC NUMQUAM adhibetur (01M16YRSZ5). */

#include "stml_html.h"
#include "chorda_aedificator.h"
#include "xar.h"

#include <string.h>

/* Elementa VACUA HTML (par. 5.3): nuda emittuntur, liberi = vitium */
interior constans character* constans _tabula_vacuorum[] = {
    "area", "base", "br", "col", "embed", "hr", "img", "input",
    "link", "meta", "param", "source", "track", "wbr", NIHIL
};

/* Attributa BOOLEANA nota (par. 5.11): his solis '="true"' in
 * formam nudam collabitur; ceteris = vitium (forma elementi
 * attributi character verum fert) */
interior constans character* constans _tabula_booleanorum[] = {
    "disabled", "checked", "required", "readonly", "selected",
    "multiple", "autofocus", "hidden", "async", "defer",
    "novalidate", "open", "reversed", "loop", "muted", "controls",
    "default", "ismap", "itemscope", "nomodule", "playsinline",
    NIHIL
};

nomen structura {
    ChordaAedificator* aed;
              Piscina* piscina;
       StmlHtmlVitium  vitium;
               chorda  detail;
                  b32  litteralis;  /* arbor genita: octeti litterales */
} VersioContextus;

interior vacuum
_nodum_emittere (
    VersioContextus* ctx,
          StmlNodus* nodus);

interior vacuum
_vitium_ponere (
    VersioContextus* ctx,
     StmlHtmlVitium  vitium,
             chorda  detail)
{
    si (ctx->vitium != STML_HTML_BENE)
    {
        redde;
    }
    ctx->vitium = vitium;
    ctx->detail = detail;
}

interior chorda
_vacua_chorda (vacuum)
{
    chorda c;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    redde c;
}

interior b32
_in_tabula (
              constans chorda* titulus,
    constans character* constans* tabula)
{
    i32 i;

    per (i = ZEPHYRUM; tabula[i] != NIHIL; i++)
    {
        si (chorda_aequalis_literis(*titulus, tabula[i]))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior vacuum
_spatia_emittere (
    VersioContextus* ctx,
             chorda* spatia)
{
    si (spatia != NIHIL)
    {
        chorda_aedificator_appendere_chorda(ctx->aed, *spatia);
    }
}

/* Ens ambiguum in textu decocto (par. 5.10): '&verbum;' aut
 * '&#...;' - '&nbsp;' et '&amp;nbsp;' arborem EANDEM dant, ergo
 * emissor rectus non exsistit; figura ipsa recusatur. Reddit
 * extensionem entis primi aut vacuam. */
interior chorda
_ens_ambiguum (
    chorda textus)
{
    i32 i;
    i32 j;

    per (i = ZEPHYRUM; i < textus.mensura; i++)
    {
        si (textus.datum[i] != (i8)'&')
        {
            perge;
        }
        j = i + I;
        si (j >= textus.mensura)
        {
            frange;
        }
        si (textus.datum[j] == (i8)'#')
        {
            j++;
            dum (   j < textus.mensura
                 && (   (textus.datum[j] >= (i8)'0'
                         && textus.datum[j] <= (i8)'9')
                     || (textus.datum[j] >= (i8)'a'
                         && textus.datum[j] <= (i8)'f')
                     || (textus.datum[j] >= (i8)'A'
                         && textus.datum[j] <= (i8)'F')
                     || textus.datum[j] == (i8)'x'
                     || textus.datum[j] == (i8)'X'))
            {
                j++;
            }
            si (   j > i + II && j < textus.mensura
                && textus.datum[j] == (i8)';')
            {
                chorda ens;

                ens.datum    = textus.datum + i;
                ens.mensura  = j + I - i;
                redde ens;
            }
        }
        alioquin si (   (textus.datum[j] >= (i8)'a'
                         && textus.datum[j] <= (i8)'z')
                     || (textus.datum[j] >= (i8)'A'
                         && textus.datum[j] <= (i8)'Z'))
        {
            dum (   j < textus.mensura
                 && (   (textus.datum[j] >= (i8)'a'
                         && textus.datum[j] <= (i8)'z')
                     || (textus.datum[j] >= (i8)'A'
                         && textus.datum[j] <= (i8)'Z')
                     || (textus.datum[j] >= (i8)'0'
                         && textus.datum[j] <= (i8)'9')))
            {
                j++;
            }
            si (j < textus.mensura && textus.datum[j] == (i8)';')
            {
                chorda ens;

                ens.datum    = textus.datum + i;
                ens.mensura  = j + I - i;
                redde ens;
            }
        }
    }
    redde _vacua_chorda();
}

/* Effugium textus (decocti): '&' '<' '>' */
interior vacuum
_textum_evadere (
    VersioContextus* ctx,
             chorda  textus)
{
          i32 i;
    character c;

    per (i = ZEPHYRUM; i < textus.mensura; i++)
    {
        c = (character)textus.datum[i];
        si (c == '&')
        {
            chorda_aedificator_appendere_literis(ctx->aed, "&amp;");
        }
        alioquin si (c == '<')
        {
            chorda_aedificator_appendere_literis(ctx->aed, "&lt;");
        }
        alioquin si (c == '>')
        {
            chorda_aedificator_appendere_literis(ctx->aed, "&gt;");
        }
        alioquin
        {
            chorda_aedificator_appendere_character(ctx->aed, c);
        }
    }
}

/* Effugium valoris attributi (decocti): '&' '"' */
interior vacuum
_attributum_evadere (
    VersioContextus* ctx,
             chorda  valor)
{
          i32 i;
    character c;

    per (i = ZEPHYRUM; i < valor.mensura; i++)
    {
        c = (character)valor.datum[i];
        si (c == '&')
        {
            chorda_aedificator_appendere_literis(ctx->aed, "&amp;");
        }
        alioquin si (c == '"')
        {
            chorda_aedificator_appendere_literis(ctx->aed,
                                                 "&quot;");
        }
        alioquin
        {
            chorda_aedificator_appendere_character(ctx->aed, c);
        }
    }
}

/* Attributa emittere: cursus INSCRIPTUS (crudus, par. 5.4 gradus
 * I) tum PRAEFIXUM elementorum attributorum (decoctum, gradus II
 * - frange primo liberо non-attributo, speculum 'capere'). Reddit
 * numerum liberorum praefixi consumptorum. */
interior i32
_attributa_emittere (
    VersioContextus* ctx,
          StmlNodus* nodus)
{
    i32 i;
    i32 num;
    i32 consumpta = ZEPHYRUM;

    si (nodus->attributa != NIHIL)
    {
        num = xar_numerus(nodus->attributa);
        per (i = ZEPHYRUM; i < num; i++)
        {
            StmlAttributum* attr = (StmlAttributum*)
                xar_obtinere(nodus->attributa, i);

            chorda_aedificator_appendere_character(ctx->aed, ' ');
            chorda_aedificator_appendere_chorda(ctx->aed,
                                                *attr->titulus);
            si (attr->valor == NIHIL)
            {
                /* a="" -> valor NIHIL = chorda VACUA, non absens
                 * (01M16XHBE2); 'alt=""' onus fert - servanda */
                chorda_aedificator_appendere_literis(ctx->aed,
                                                     "=\"\"");
            }
            alioquin si (chorda_aequalis_literis(*attr->valor,
                                                 "true"))
            {
                /* '<x a>' et '<x a="true">' arbor EADEM (par.
                 * 5.11): booleanis notis forma nuda; ceteris
                 * vitium - forma '<@a=>true</>' verum litterale
                 * fert */
                si (!_in_tabula(attr->titulus, _tabula_booleanorum))
                {
                    _vitium_ponere(ctx, STML_HTML_ATTRIBUTUM_VERUM,
                                   *attr->titulus);
                    redde consumpta;
                }
            }
            alioquin
            {
                chorda_aedificator_appendere_literis(ctx->aed,
                                                     "=\"");
                si (ctx->litteralis)
                {
                    _attributum_evadere(ctx, *attr->valor);
                }
                alioquin
                {
                    chorda_aedificator_appendere_chorda(ctx->aed,
                                                        *attr->valor);
                }
                chorda_aedificator_appendere_character(ctx->aed, '"');
            }
        }
    }

    si (nodus->liberi == NIHIL)
    {
        redde consumpta;
    }
    num = xar_numerus(nodus->liberi);
    dum (consumpta < num)
    {
        StmlNodus* l = *(StmlNodus**)
            xar_obtinere(nodus->liberi, consumpta);

        si (   l                     == NIHIL
            || l->genus              != STML_NODUS_ELEMENTUM
            || l->attributum_titulus == NIHIL)
        {
            frange;
        }
        si (   l->liberi              == NIHIL
            || xar_numerus(l->liberi) == ZEPHYRUM)
        {
            /* sepulcrum '<@a=/>' - nomen omittitur */
        }
        alioquin
        {
            StmlNodus* valor_nodus = *(StmlNodus**)
                xar_obtinere(l->liberi, ZEPHYRUM);

            chorda_aedificator_appendere_character(ctx->aed, ' ');
            chorda_aedificator_appendere_chorda(
                ctx->aed, *l->attributum_titulus);
            chorda_aedificator_appendere_literis(ctx->aed, "=\"");
            si (valor_nodus != NIHIL && valor_nodus->valor != NIHIL)
            {
                _attributum_evadere(ctx, *valor_nodus->valor);
            }
            chorda_aedificator_appendere_character(ctx->aed, '"');
        }
        consumpta++;
    }
    redde consumpta;
}

/* Liberos ab indice emittere, triviis lectis (par. 5.9). Elementum
 * attributi extra praefixum = argumentum vocationis quod expansio
 * consumere debuit - anomalum, RECUSATUR (par. 5.4). */
interior vacuum
_liberos_emittere (
    VersioContextus* ctx,
          StmlNodus* nodus,
                i32  initium)
{
    i32 i;
    i32 num;

    si (nodus->liberi == NIHIL)
    {
        redde;
    }
    num = xar_numerus(nodus->liberi);
    per (i = initium; i < num; i++)
    {
        StmlNodus* l = *(StmlNodus**)
            xar_obtinere(nodus->liberi, i);

        si (ctx->vitium != STML_HTML_BENE)
        {
            redde;
        }
        si (l == NIHIL)
        {
            perge;
        }
        si (   l->genus              == STML_NODUS_ELEMENTUM
            && l->attributum_titulus != NIHIL)
        {
            _vitium_ponere(ctx, STML_HTML_ATTRIBUTUM_SOLUTUM,
                           *l->attributum_titulus);
            redde;
        }
        _spatia_emittere(ctx, l->spatia_ante);
        _nodum_emittere(ctx, l);
        _spatia_emittere(ctx, l->spatia_post);
    }
}

interior vacuum
_elementum_emittere (
    VersioContextus* ctx,
          StmlNodus* nodus)
{
    i32 consumpta;
    b32 crudus_requisitus;

    si (nodus->fragmentum)
    {
        /* fragmentum DISSOLVITUR: liberi soli, nullum tagum */
        _liberos_emittere(ctx, nodus, ZEPHYRUM);
        _spatia_emittere(ctx, nodus->spatia_clausurae);
        redde;
    }
    si (nodus->augmentum_clavis != NIHIL)
    {
        _vitium_ponere(ctx, STML_HTML_AUGMENTATIO,
                       nodus->titulus != NIHIL ? *nodus->titulus
                                               : _vacua_chorda());
        redde;
    }
    si (nodus->titulus == NIHIL)
    {
        _vitium_ponere(ctx, STML_HTML_TITULUS_NIHIL,
                       _vacua_chorda());
        redde;
    }
    si (   nodus->titulus->mensura > ZEPHYRUM
        && nodus->titulus->datum[ZEPHYRUM] == (i8)'.')
    {
        _vitium_ponere(ctx, STML_HTML_TITULUS_PUNCTATUS,
                       *nodus->titulus);
        redde;
    }

    /* script/style sine '!' = contentum iam ut notatio parsatum -
     * aliquid superius fractum est (par. 5.5) */
    crudus_requisitus =    chorda_aequalis_literis(*nodus->titulus,
                                                   "script")
                        || chorda_aequalis_literis(*nodus->titulus,
                                                   "style")
                        || chorda_aequalis_literis(*nodus->titulus,
                                                   "crudum");
    si (crudus_requisitus && !nodus->crudus)
    {
        _vitium_ponere(ctx, STML_HTML_CRUDUS_DEEST,
                       *nodus->titulus);
        redde;
    }
    si (chorda_aequalis_literis(*nodus->titulus, "crudum"))
    {
        /* SPLEX octetorum sine tags (decretum 2026-09-03, B3.1 md):
         * liberi textus VERBATIM, nulla evasio, nulla tagi -
         * nomen unum decretum, semper crudum ('!') */
        i32 i;
        i32 num;

        num = nodus->liberi != NIHIL ? xar_numerus(nodus->liberi)
                                     : ZEPHYRUM;
        per (i = ZEPHYRUM; i < num; i++)
        {
            StmlNodus* l = *(StmlNodus**)xar_obtinere(nodus->liberi, i);

            si (   l        != NIHIL
                && l->genus == STML_NODUS_TEXTUS
                && l->valor != NIHIL)
            {
                chorda_aedificator_appendere_chorda(ctx->aed,
                                                    *l->valor);
            }
        }
        redde;
    }

    chorda_aedificator_appendere_character(ctx->aed, '<');
    chorda_aedificator_appendere_chorda(ctx->aed, *nodus->titulus);
    consumpta = _attributa_emittere(ctx, nodus);
    si (ctx->vitium != STML_HTML_BENE)
    {
        redde;
    }

    si (nodus->crudus)
    {
        /* Contentum crudum FURCATUR (Fran, 2026-09-01): script/
         * style = RAWTEXT HTML - navigator contentum numquam
         * decoquit, ergo VERBATIM (effugium 'a && b' in
         * 'a &amp;&amp; b' litterale corrumperet). Tagi CETERI
         * ('<pre!>', '<code!>'): navigator contentum ut NOTATIONEM
         * legit, ergo crudum STML = textus litteralis -> EVADITUR
         * ('&' '<' '>'). Entia hic numquam ambigua (crudum numquam
         * decoquitur, '&' auctoris semper litteralis), ergo
         * recusatio par. 5.10 NON currit - ianua ad textum DE
         * entibus: '<code!>&nbsp;</code>' litterale ostendit. */
        i32 i;
        i32 num;

        chorda_aedificator_appendere_character(ctx->aed, '>');
        si (nodus->liberi != NIHIL)
        {
            num = xar_numerus(nodus->liberi);
            per (i = consumpta; i < num; i++)
            {
                StmlNodus* l = *(StmlNodus**)
                    xar_obtinere(nodus->liberi, i);

                si (   l        != NIHIL
                    && l->genus == STML_NODUS_TEXTUS
                    && l->valor != NIHIL)
                {
                    si (crudus_requisitus)
                    {
                        chorda_aedificator_appendere_chorda(
                            ctx->aed, *l->valor);
                    }
                    alioquin
                    {
                        _textum_evadere(ctx, *l->valor);
                    }
                }
            }
        }
        chorda_aedificator_appendere_literis(ctx->aed, "</");
        chorda_aedificator_appendere_chorda(ctx->aed,
                                            *nodus->titulus);
        chorda_aedificator_appendere_character(ctx->aed, '>');
        redde;
    }

    si (_in_tabula(nodus->titulus, _tabula_vacuorum))
    {
        /* vacuum: nudum SEMPER ('<br/>' navigatori div apertus
         * esset); liberi reales = vitium ('<br></br>' illicitum) */
        si (   nodus->liberi != NIHIL
            && consumpta < xar_numerus(nodus->liberi))
        {
            _vitium_ponere(ctx, STML_HTML_VACUUM_CUM_LIBERIS,
                           *nodus->titulus);
            redde;
        }
        chorda_aedificator_appendere_character(ctx->aed, '>');
        redde;
    }

    /* ordinarium: par SEMPER, etiam sine liberis */
    chorda_aedificator_appendere_character(ctx->aed, '>');
    _liberos_emittere(ctx, nodus, consumpta);
    _spatia_emittere(ctx, nodus->spatia_clausurae);
    chorda_aedificator_appendere_literis(ctx->aed, "</");
    chorda_aedificator_appendere_chorda(ctx->aed, *nodus->titulus);
    chorda_aedificator_appendere_character(ctx->aed, '>');
}

interior vacuum
_nodum_emittere (
    VersioContextus* ctx,
          StmlNodus* nodus)
{
    si (ctx->vitium != STML_HTML_BENE)
    {
        redde;
    }
    commutatio (nodus->genus)
    {
        casus STML_NODUS_TEXTUS:
        {
            chorda ens;

            si (nodus->valor == NIHIL)
            {
                redde;
            }
            ens =
                ctx->litteralis ? _vacua_chorda() : _ens_ambiguum(*nodus->valor);
            si (ens.mensura > ZEPHYRUM)
            {
                /* '&nbsp;' et '&amp;nbsp;' indiscernibiles -
                 * character ipse scribendus (par. 5.10) */
                _vitium_ponere(ctx, STML_HTML_ENS_AMBIGUUM, ens);
                redde;
            }
            _textum_evadere(ctx, *nodus->valor);
            redde;
        }
        casus STML_NODUS_COMMENTUM:
            chorda_aedificator_appendere_literis(ctx->aed, "<!--");
            si (nodus->valor != NIHIL)
            {
                /* valor CRUDUS, numquam re-escapatur */
                chorda_aedificator_appendere_chorda(ctx->aed,
                                                    *nodus->valor);
            }
            chorda_aedificator_appendere_literis(ctx->aed, "-->");
            redde;
        casus STML_NODUS_DOCTYPE:
            /* IGNORATUR - emissor suum iam scripsit */
            redde;
        casus STML_NODUS_PROCESSIO:
            _vitium_ponere(ctx, STML_HTML_PROCESSIO,
                           nodus->valor != NIHIL ? *nodus->valor
                                                 : _vacua_chorda());
            redde;
        casus STML_NODUS_TRANSCLUSIO:
            _vitium_ponere(ctx, STML_HTML_TRANSCLUSIO,
                           nodus->valor != NIHIL ? *nodus->valor
                                                 : _vacua_chorda());
            redde;
        casus STML_NODUS_DOCUMENTUM:
            _liberos_emittere(ctx, nodus, ZEPHYRUM);
            _spatia_emittere(ctx, nodus->spatia_clausurae);
            redde;
        casus STML_NODUS_ELEMENTUM:
            _elementum_emittere(ctx, nodus);
            redde;
        ordinarius:
            redde;
    }
}

StmlHtmlResultus
stml_html_vertere (
    StmlNodus* radix,
      Piscina* piscina)
{
    StmlHtmlResultus resultus;
     VersioContextus ctx;

    resultus.successus  = FALSUM;
    resultus.html       = _vacua_chorda();
    resultus.vitium     = STML_HTML_BENE;
    resultus.detail     = _vacua_chorda();

    ctx.aed         = chorda_aedificator_creare(piscina, 4096);
    ctx.piscina     = piscina;
    ctx.vitium      = STML_HTML_BENE;
    ctx.detail      = _vacua_chorda();
    ctx.litteralis  = FALSUM;
    si (ctx.aed == NIHIL)
    {
        resultus.vitium = STML_HTML_MEMORIA;
        redde resultus;
    }

    chorda_aedificator_appendere_literis(ctx.aed,
                                         "<!DOCTYPE html>\n");
    si (radix != NIHIL)
    {
        _nodum_emittere(&ctx, radix);
    }
    si (ctx.vitium != STML_HTML_BENE)
    {
        resultus.vitium = ctx.vitium;
        resultus.detail = ctx.detail;
        redde resultus;
    }
    resultus.html       = chorda_aedificator_finire(ctx.aed);
    resultus.successus  = VERUM;
    redde resultus;
}

StmlHtmlResultus
stml_html_vertere_liberos (
     StmlNodus* parens,
       Piscina* piscina,
           b32  litteralis)
{
    StmlHtmlResultus resultus;
     VersioContextus ctx;
                 i32 i;
                 i32 num;

    resultus.successus  = FALSUM;
    resultus.html       = _vacua_chorda();
    resultus.vitium     = STML_HTML_BENE;
    resultus.detail     = _vacua_chorda();

    ctx.aed         = chorda_aedificator_creare(piscina, 4096);
    ctx.piscina     = piscina;
    ctx.vitium      = STML_HTML_BENE;
    ctx.detail      = _vacua_chorda();
    ctx.litteralis  = litteralis;
    si (ctx.aed == NIHIL)
    {
        resultus.vitium = STML_HTML_MEMORIA;
        redde resultus;
    }
    num = (parens != NIHIL && parens->liberi != NIHIL)
        ? xar_numerus(parens->liberi) : ZEPHYRUM;
    per (i = ZEPHYRUM; i < num; i++)
    {
        StmlNodus* l = *(StmlNodus**)xar_obtinere(parens->liberi, i);

        si (l != NIHIL)
        {
            _nodum_emittere(&ctx, l);
        }
    }
    si (ctx.vitium != STML_HTML_BENE)
    {
        resultus.vitium = ctx.vitium;
        resultus.detail = ctx.detail;
        redde resultus;
    }
    resultus.html       = chorda_aedificator_finire(ctx.aed);
    resultus.successus  = VERUM;
    redde resultus;
}

constans character*
stml_html_vitium_titulus (
    StmlHtmlVitium vitium)
{
    commutatio (vitium)
    {
        casus STML_HTML_BENE:               redde "BENE";
        casus STML_HTML_PROCESSIO:          redde "PROCESSIO";
        casus STML_HTML_TRANSCLUSIO:        redde "TRANSCLUSIO";
        casus STML_HTML_AUGMENTATIO:        redde "AUGMENTATIO";
        casus STML_HTML_TITULUS_PUNCTATUS:  redde "TITULUS_PUNCTATUS";
        casus STML_HTML_TITULUS_NIHIL:      redde "TITULUS_NIHIL";
        casus STML_HTML_VACUUM_CUM_LIBERIS: redde "VACUUM_CUM_LIBERIS";
        casus STML_HTML_CRUDUS_DEEST:       redde "CRUDUS_DEEST";
        casus STML_HTML_ENS_AMBIGUUM:       redde "ENS_AMBIGUUM";
        casus STML_HTML_ATTRIBUTUM_VERUM:   redde "ATTRIBUTUM_VERUM";
        casus STML_HTML_ATTRIBUTUM_SOLUTUM: redde "ATTRIBUTUM_SOLUTUM";
        ordinarius:                         redde "IGNOTUM";
    }
}
