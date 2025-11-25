#include "xml.h"
#include "chorda_aedificator.h"
#include <string.h>

/* ==================================================
 * Adiutores Interni
 * ================================================== */

/* Verificare si character est spatium album */
interior b32
_est_spatium(character c)
{
    redde c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

/* Verificare si character est valida in nomine XML */
interior b32
_est_nomen_character(character c)
{
    redde (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           (c >= '0' && c <= '9') ||
           c == '_' || c == '-' || c == ':' || c == '.';
}

/* Comparare chorda cum literis C */
interior b32
_chorda_aequalis_literis(chorda* ch, constans character* cstr)
{
    i32 len;
    i32 i;

    si (!ch || !cstr)
    {
        redde FALSUM;
    }

    /* Mensura literarum */
    len = ZEPHYRUM;
    dum (cstr[len] != '\0')
    {
        len++;
    }

    si (ch->mensura != len)
    {
        redde FALSUM;
    }

    per (i = ZEPHYRUM; i < len; i++)
    {
        si ((character)ch->datum[i] != cstr[i])
        {
            redde FALSUM;
        }
    }

    redde VERUM;
}

/* Praeterire spatium album */
interior vacuum
_praeterire_spatium(chorda* input, i32* positus)
{
    dum (*positus < input->mensura && _est_spatium((character)input->datum[*positus]))
    {
        (*positus)++;
    }
}

/* Internare chordam ex literis */
interior chorda*
_internare_literis(
    InternamentumChorda* intern,
    constans character*  cstr)
{
    redde chorda_internare_ex_literis(intern, cstr);
}

/* Internare chorda sectio */
interior chorda*
_internare_sectio(
    InternamentumChorda* intern,
    chorda               fonte,
    i32                  initium,
    i32                  finis)
{
    chorda sectio;

    sectio = chorda_sectio(fonte, initium, finis);
    redde chorda_internare(intern, sectio);
}

/* ==================================================
 * Constructio - Creare Nodos
 * ================================================== */

XmlNodus*
xml_elementum_creare(
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  titulus)
{
    XmlNodus* nodus;

    si (!piscina || !intern || !titulus)
    {
        redde NIHIL;
    }

    nodus = piscina_allocare(piscina, magnitudo(XmlNodus));
    si (!nodus)
    {
        redde NIHIL;
    }

    nodus->genus = XML_NODUS_ELEMENTUM;
    nodus->titulus = _internare_literis(intern, titulus);
    nodus->valor = NIHIL;
    nodus->attributa = xar_creare(piscina, magnitudo(XmlAttributum));
    nodus->liberi = xar_creare(piscina, magnitudo(XmlNodus*));
    nodus->parens = NIHIL;

    redde nodus;
}

XmlNodus*
xml_textum_creare(
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  textus)
{
    XmlNodus* nodus;

    si (!piscina || !intern || !textus)
    {
        redde NIHIL;
    }

    nodus = piscina_allocare(piscina, magnitudo(XmlNodus));
    si (!nodus)
    {
        redde NIHIL;
    }

    nodus->genus = XML_NODUS_TEXTUS;
    nodus->titulus = NIHIL;
    nodus->valor = _internare_literis(intern, textus);
    nodus->attributa = NIHIL;
    nodus->liberi = NIHIL;
    nodus->parens = NIHIL;

    redde nodus;
}

XmlNodus*
xml_textum_creare_ex_chorda(
    Piscina*             piscina,
    InternamentumChorda* intern,
    chorda               textus)
{
    XmlNodus* nodus;

    si (!piscina || !intern)
    {
        redde NIHIL;
    }

    nodus = piscina_allocare(piscina, magnitudo(XmlNodus));
    si (!nodus)
    {
        redde NIHIL;
    }

    nodus->genus = XML_NODUS_TEXTUS;
    nodus->titulus = NIHIL;
    nodus->valor = chorda_internare(intern, textus);
    nodus->attributa = NIHIL;
    nodus->liberi = NIHIL;
    nodus->parens = NIHIL;

    redde nodus;
}

XmlNodus*
xml_commentum_creare(
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  textus)
{
    XmlNodus* nodus;

    si (!piscina || !intern || !textus)
    {
        redde NIHIL;
    }

    nodus = piscina_allocare(piscina, magnitudo(XmlNodus));
    si (!nodus)
    {
        redde NIHIL;
    }

    nodus->genus = XML_NODUS_COMMENTUM;
    nodus->titulus = NIHIL;
    nodus->valor = _internare_literis(intern, textus);
    nodus->attributa = NIHIL;
    nodus->liberi = NIHIL;
    nodus->parens = NIHIL;

    redde nodus;
}

b32
xml_attributum_addere(
    XmlNodus*            nodus,
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  titulus,
    constans character*  valor)
{
    XmlAttributum* attr;

    si (!nodus || !piscina || !intern || !titulus || !valor)
    {
        redde FALSUM;
    }

    si (nodus->genus != XML_NODUS_ELEMENTUM)
    {
        redde FALSUM;
    }

    si (!nodus->attributa)
    {
        nodus->attributa = xar_creare(piscina, magnitudo(XmlAttributum));
        si (!nodus->attributa)
        {
            redde FALSUM;
        }
    }

    attr = xar_addere(nodus->attributa);
    si (!attr)
    {
        redde FALSUM;
    }

    attr->titulus = _internare_literis(intern, titulus);
    attr->valor = _internare_literis(intern, valor);

    redde VERUM;
}

b32
xml_attributum_addere_chorda(
    XmlNodus*            nodus,
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  titulus,
    chorda               valor)
{
    XmlAttributum* attr;

    si (!nodus || !piscina || !intern || !titulus)
    {
        redde FALSUM;
    }

    si (nodus->genus != XML_NODUS_ELEMENTUM)
    {
        redde FALSUM;
    }

    si (!nodus->attributa)
    {
        nodus->attributa = xar_creare(piscina, magnitudo(XmlAttributum));
        si (!nodus->attributa)
        {
            redde FALSUM;
        }
    }

    attr = xar_addere(nodus->attributa);
    si (!attr)
    {
        redde FALSUM;
    }

    attr->titulus = _internare_literis(intern, titulus);
    attr->valor = chorda_internare(intern, valor);

    redde VERUM;
}

b32
xml_liberum_addere(
    XmlNodus* parens,
    XmlNodus* liberum)
{
    XmlNodus** slot;

    si (!parens || !liberum)
    {
        redde FALSUM;
    }

    si (parens->genus != XML_NODUS_ELEMENTUM)
    {
        redde FALSUM;
    }

    si (!parens->liberi)
    {
        redde FALSUM;
    }

    slot = xar_addere(parens->liberi);
    si (!slot)
    {
        redde FALSUM;
    }

    *slot = liberum;
    liberum->parens = parens;

    redde VERUM;
}

b32
xml_textum_addere(
    XmlNodus*            parens,
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  textus)
{
    XmlNodus* nodus_textus;

    nodus_textus = xml_textum_creare(piscina, intern, textus);
    si (!nodus_textus)
    {
        redde FALSUM;
    }

    redde xml_liberum_addere(parens, nodus_textus);
}

/* ==================================================
 * Quaestio - Invenire in Arbore
 * ================================================== */

i32
xml_numerus_liberorum(
    XmlNodus* nodus)
{
    si (!nodus || !nodus->liberi)
    {
        redde ZEPHYRUM;
    }

    redde xar_numerus(nodus->liberi);
}

XmlNodus*
xml_liberum_ad_indicem(
    XmlNodus* nodus,
    i32       index)
{
    XmlNodus** slot;

    si (!nodus || !nodus->liberi)
    {
        redde NIHIL;
    }

    slot = xar_obtinere(nodus->liberi, index);
    si (!slot)
    {
        redde NIHIL;
    }

    redde *slot;
}

XmlNodus*
xml_invenire_liberum(
    XmlNodus*           nodus,
    constans character* titulus)
{
    i32 i;
    i32 num;
    XmlNodus* liberum;

    si (!nodus || !titulus || !nodus->liberi)
    {
        redde NIHIL;
    }

    num = xar_numerus(nodus->liberi);

    per (i = ZEPHYRUM; i < num; i++)
    {
        liberum = xml_liberum_ad_indicem(nodus, i);
        si (liberum && liberum->genus == XML_NODUS_ELEMENTUM && liberum->titulus)
        {
            /* Comparare nomina (interned, ergo pointer comparatio) */
            si (_chorda_aequalis_literis(liberum->titulus, titulus))
            {
                redde liberum;
            }
        }
    }

    redde NIHIL;
}

Xar*
xml_invenire_omnes_liberos(
    XmlNodus*           nodus,
    constans character* titulus,
    Piscina*            piscina)
{
    Xar* resultus;
    i32 i;
    i32 num;
    XmlNodus* liberum;
    XmlNodus** slot;

    si (!nodus || !titulus || !piscina || !nodus->liberi)
    {
        redde NIHIL;
    }

    resultus = xar_creare(piscina, magnitudo(XmlNodus*));
    si (!resultus)
    {
        redde NIHIL;
    }

    num = xar_numerus(nodus->liberi);

    per (i = ZEPHYRUM; i < num; i++)
    {
        liberum = xml_liberum_ad_indicem(nodus, i);
        si (liberum && liberum->genus == XML_NODUS_ELEMENTUM && liberum->titulus)
        {
            si (_chorda_aequalis_literis(liberum->titulus, titulus))
            {
                slot = xar_addere(resultus);
                si (slot)
                {
                    *slot = liberum;
                }
            }
        }
    }

    redde resultus;
}

chorda*
xml_attributum_capere(
    XmlNodus*           nodus,
    constans character* titulus)
{
    i32 i;
    i32 num;
    XmlAttributum* attr;

    si (!nodus || !titulus || !nodus->attributa)
    {
        redde NIHIL;
    }

    num = xar_numerus(nodus->attributa);

    per (i = ZEPHYRUM; i < num; i++)
    {
        attr = xar_obtinere(nodus->attributa, i);
        si (attr && attr->titulus)
        {
            si (_chorda_aequalis_literis(attr->titulus, titulus))
            {
                redde attr->valor;
            }
        }
    }

    redde NIHIL;
}

b32
xml_attributum_habet(
    XmlNodus*           nodus,
    constans character* titulus)
{
    redde xml_attributum_capere(nodus, titulus) != NIHIL;
}

chorda
xml_textus_internus(
    XmlNodus* nodus,
    Piscina*  piscina)
{
    ChordaAedificator* aed;
    i32 i;
    i32 num;
    XmlNodus* liberum;
    chorda resultus;

    si (!nodus || !piscina)
    {
        resultus.datum = NIHIL;
        resultus.mensura = ZEPHYRUM;
        redde resultus;
    }

    /* Si nodus est textus, redde valor directe */
    si (nodus->genus == XML_NODUS_TEXTUS && nodus->valor)
    {
        redde *nodus->valor;
    }

    /* Si elementum, concatenare textum omnium liberorum */
    si (nodus->genus != XML_NODUS_ELEMENTUM || !nodus->liberi)
    {
        resultus.datum = NIHIL;
        resultus.mensura = ZEPHYRUM;
        redde resultus;
    }

    aed = chorda_aedificator_creare(piscina, CXXVIII);
    si (!aed)
    {
        resultus.datum = NIHIL;
        resultus.mensura = ZEPHYRUM;
        redde resultus;
    }

    num = xar_numerus(nodus->liberi);

    per (i = ZEPHYRUM; i < num; i++)
    {
        liberum = xml_liberum_ad_indicem(nodus, i);
        si (liberum && liberum->genus == XML_NODUS_TEXTUS && liberum->valor)
        {
            chorda_aedificator_appendere_chorda(aed, *liberum->valor);
        }
    }

    redde chorda_aedificator_finire(aed);
}

/* ==================================================
 * Scriptio - Scribere XML
 * ================================================== */

/* Scribere textum cum effugio XML */
interior b32
_scribere_textum_effugiens(
    ChordaAedificator* aed,
    chorda*            textus)
{
    i32 i;
    character c;

    si (!textus)
    {
        redde VERUM;
    }

    per (i = ZEPHYRUM; i < textus->mensura; i++)
    {
        c = (character)textus->datum[i];

        si (c == '<')
        {
            chorda_aedificator_appendere_literis(aed, "&lt;");
        }
        alioquin si (c == '>')
        {
            chorda_aedificator_appendere_literis(aed, "&gt;");
        }
        alioquin si (c == '&')
        {
            chorda_aedificator_appendere_literis(aed, "&amp;");
        }
        alioquin
        {
            chorda_aedificator_appendere_character(aed, c);
        }
    }

    redde VERUM;
}

/* Scribere valor attributi cum effugio */
interior b32
_scribere_attributum_valor(
    ChordaAedificator* aed,
    chorda*            valor)
{
    i32 i;
    character c;

    si (!valor)
    {
        redde VERUM;
    }

    per (i = ZEPHYRUM; i < valor->mensura; i++)
    {
        c = (character)valor->datum[i];

        si (c == '<')
        {
            chorda_aedificator_appendere_literis(aed, "&lt;");
        }
        alioquin si (c == '>')
        {
            chorda_aedificator_appendere_literis(aed, "&gt;");
        }
        alioquin si (c == '&')
        {
            chorda_aedificator_appendere_literis(aed, "&amp;");
        }
        alioquin si (c == '"')
        {
            chorda_aedificator_appendere_literis(aed, "&quot;");
        }
        alioquin
        {
            chorda_aedificator_appendere_character(aed, c);
        }
    }

    redde VERUM;
}

/* Scribere indentationem */
interior vacuum
_scribere_indentationem(
    ChordaAedificator* aed,
    i32                gradus)
{
    i32 i;

    per (i = ZEPHYRUM; i < gradus; i++)
    {
        chorda_aedificator_appendere_literis(aed, "  ");
    }
}

b32
xml_scribere_ad_aedificator(
    XmlNodus*          nodus,
    ChordaAedificator* aed,
    b32                pulchrum,
    i32                indentatio)
{
    i32 i;
    i32 num_attr;
    i32 num_lib;
    XmlAttributum* attr;
    XmlNodus* liberum;
    b32 habet_liberos;
    b32 solum_textus;

    si (!nodus || !aed)
    {
        redde FALSUM;
    }

    si (nodus->genus == XML_NODUS_TEXTUS)
    {
        _scribere_textum_effugiens(aed, nodus->valor);
        redde VERUM;
    }

    si (nodus->genus == XML_NODUS_COMMENTUM)
    {
        si (pulchrum)
        {
            _scribere_indentationem(aed, indentatio);
        }
        chorda_aedificator_appendere_literis(aed, "<!-- ");
        si (nodus->valor)
        {
            chorda_aedificator_appendere_chorda(aed, *nodus->valor);
        }
        chorda_aedificator_appendere_literis(aed, " -->");
        si (pulchrum)
        {
            chorda_aedificator_appendere_lineam_novam(aed);
        }
        redde VERUM;
    }

    si (nodus->genus != XML_NODUS_ELEMENTUM)
    {
        redde FALSUM;
    }

    /* Elementum */
    num_lib = nodus->liberi ? xar_numerus(nodus->liberi) : ZEPHYRUM;
    habet_liberos = num_lib > ZEPHYRUM;

    /* Verificare si solum textus (unus liberum textus) */
    solum_textus = FALSUM;
    si (num_lib == I)
    {
        liberum = xml_liberum_ad_indicem(nodus, ZEPHYRUM);
        si (liberum && liberum->genus == XML_NODUS_TEXTUS)
        {
            solum_textus = VERUM;
        }
    }

    /* Indentatio */
    si (pulchrum)
    {
        _scribere_indentationem(aed, indentatio);
    }

    /* Tag aperiens */
    chorda_aedificator_appendere_character(aed, '<');
    chorda_aedificator_appendere_chorda(aed, *nodus->titulus);

    /* Attributa */
    num_attr = nodus->attributa ? xar_numerus(nodus->attributa) : ZEPHYRUM;
    per (i = ZEPHYRUM; i < num_attr; i++)
    {
        attr = xar_obtinere(nodus->attributa, i);
        si (attr && attr->titulus && attr->valor)
        {
            chorda_aedificator_appendere_character(aed, ' ');
            chorda_aedificator_appendere_chorda(aed, *attr->titulus);
            chorda_aedificator_appendere_literis(aed, "=\"");
            _scribere_attributum_valor(aed, attr->valor);
            chorda_aedificator_appendere_character(aed, '"');
        }
    }

    /* Si nullos liberos, claudere cum /> */
    si (!habet_liberos)
    {
        chorda_aedificator_appendere_literis(aed, "/>");
        si (pulchrum)
        {
            chorda_aedificator_appendere_lineam_novam(aed);
        }
        redde VERUM;
    }

    /* Claudere tag aperiens */
    chorda_aedificator_appendere_character(aed, '>');

    /* Si solum textus, non addere lineam novam */
    si (!solum_textus && pulchrum)
    {
        chorda_aedificator_appendere_lineam_novam(aed);
    }

    /* Liberi */
    per (i = ZEPHYRUM; i < num_lib; i++)
    {
        liberum = xml_liberum_ad_indicem(nodus, i);
        si (liberum)
        {
            si (solum_textus)
            {
                /* Scribere textum sine indentatio */
                xml_scribere_ad_aedificator(liberum, aed, FALSUM, ZEPHYRUM);
            }
            alioquin
            {
                xml_scribere_ad_aedificator(liberum, aed, pulchrum, indentatio + I);
            }
        }
    }

    /* Tag claudens */
    si (!solum_textus && pulchrum)
    {
        _scribere_indentationem(aed, indentatio);
    }
    chorda_aedificator_appendere_literis(aed, "</");
    chorda_aedificator_appendere_chorda(aed, *nodus->titulus);
    chorda_aedificator_appendere_character(aed, '>');

    si (pulchrum)
    {
        chorda_aedificator_appendere_lineam_novam(aed);
    }

    redde VERUM;
}

chorda
xml_scribere(
    XmlNodus* nodus,
    Piscina*  piscina,
    b32       pulchrum)
{
    ChordaAedificator* aed;
    chorda resultus;

    si (!nodus || !piscina)
    {
        resultus.datum = NIHIL;
        resultus.mensura = ZEPHYRUM;
        redde resultus;
    }

    aed = chorda_aedificator_creare(piscina, CCLVI);
    si (!aed)
    {
        resultus.datum = NIHIL;
        resultus.mensura = ZEPHYRUM;
        redde resultus;
    }

    xml_scribere_ad_aedificator(nodus, aed, pulchrum, ZEPHYRUM);

    redde chorda_aedificator_finire(aed);
}

/* ==================================================
 * Parsatio - Legere XML
 * ================================================== */

/* Contextus parsationis */
nomen structura {
    chorda               input;
    i32                  positus;
    i32                  linea;
    i32                  columna;
    Piscina*             piscina;
    InternamentumChorda* intern;
    XmlStatus            status;
    chorda               error;
} XmlContextus;

/* Legere titulum (tag vel attributum) */
interior chorda*
_legere_titulus(XmlContextus* ctx)
{
    i32 initium;

    initium = ctx->positus;

    dum (ctx->positus < ctx->input.mensura &&
         _est_nomen_character((character)ctx->input.datum[ctx->positus]))
    {
        ctx->positus++;
        ctx->columna++;
    }

    si (ctx->positus == initium)
    {
        redde NIHIL;
    }

    redde _internare_sectio(ctx->intern, ctx->input, initium, ctx->positus);
}

/* Legere valor attributi (post =") */
interior chorda*
_legere_valor_attributi(XmlContextus* ctx)
{
    character quota;
    ChordaAedificator* aed;
    chorda resultus;

    si (ctx->positus >= ctx->input.mensura)
    {
        redde NIHIL;
    }

    quota = (character)ctx->input.datum[ctx->positus];
    si (quota != '"' && quota != '\'')
    {
        redde NIHIL;
    }

    ctx->positus++;
    ctx->columna++;

    /* Usare aedificator pro effugiis */
    aed = chorda_aedificator_creare(ctx->piscina, LXIV);
    si (!aed)
    {
        redde NIHIL;
    }

    dum (ctx->positus < ctx->input.mensura)
    {
        character c;

        c = (character)ctx->input.datum[ctx->positus];

        si (c == quota)
        {
            ctx->positus++;
            ctx->columna++;
            frange;
        }

        si (c == '&')
        {
            /* Tractare entity reference */
            si (ctx->positus + III < ctx->input.mensura &&
                ctx->input.datum[ctx->positus + I] == 'l' &&
                ctx->input.datum[ctx->positus + II] == 't' &&
                ctx->input.datum[ctx->positus + III] == ';')
            {
                chorda_aedificator_appendere_character(aed, '<');
                ctx->positus += IV;
                ctx->columna += IV;
            }
            alioquin si (ctx->positus + III < ctx->input.mensura &&
                         ctx->input.datum[ctx->positus + I] == 'g' &&
                         ctx->input.datum[ctx->positus + II] == 't' &&
                         ctx->input.datum[ctx->positus + III] == ';')
            {
                chorda_aedificator_appendere_character(aed, '>');
                ctx->positus += IV;
                ctx->columna += IV;
            }
            alioquin si (ctx->positus + IV < ctx->input.mensura &&
                         ctx->input.datum[ctx->positus + I] == 'a' &&
                         ctx->input.datum[ctx->positus + II] == 'm' &&
                         ctx->input.datum[ctx->positus + III] == 'p' &&
                         ctx->input.datum[ctx->positus + IV] == ';')
            {
                chorda_aedificator_appendere_character(aed, '&');
                ctx->positus += V;
                ctx->columna += V;
            }
            alioquin si (ctx->positus + V < ctx->input.mensura &&
                         ctx->input.datum[ctx->positus + I] == 'q' &&
                         ctx->input.datum[ctx->positus + II] == 'u' &&
                         ctx->input.datum[ctx->positus + III] == 'o' &&
                         ctx->input.datum[ctx->positus + IV] == 't' &&
                         ctx->input.datum[ctx->positus + V] == ';')
            {
                chorda_aedificator_appendere_character(aed, '"');
                ctx->positus += VI;
                ctx->columna += VI;
            }
            alioquin si (ctx->positus + V < ctx->input.mensura &&
                         ctx->input.datum[ctx->positus + I] == 'a' &&
                         ctx->input.datum[ctx->positus + II] == 'p' &&
                         ctx->input.datum[ctx->positus + III] == 'o' &&
                         ctx->input.datum[ctx->positus + IV] == 's' &&
                         ctx->input.datum[ctx->positus + V] == ';')
            {
                chorda_aedificator_appendere_character(aed, '\'');
                ctx->positus += VI;
                ctx->columna += VI;
            }
            alioquin
            {
                /* Entity non recognita, copiere & */
                chorda_aedificator_appendere_character(aed, '&');
                ctx->positus++;
                ctx->columna++;
            }
        }
        alioquin
        {
            si (c == '\n')
            {
                ctx->linea++;
                ctx->columna = I;
            }
            alioquin
            {
                ctx->columna++;
            }

            chorda_aedificator_appendere_character(aed, c);
            ctx->positus++;
        }
    }

    resultus = chorda_aedificator_finire(aed);
    redde chorda_internare(ctx->intern, resultus);
}

/* Legere textum inter tags */
interior chorda*
_legere_textus(XmlContextus* ctx)
{
    ChordaAedificator* aed;
    chorda resultus;
    b32 habet_contentum;

    aed = chorda_aedificator_creare(ctx->piscina, LXIV);
    si (!aed)
    {
        redde NIHIL;
    }

    habet_contentum = FALSUM;

    dum (ctx->positus < ctx->input.mensura)
    {
        character c;

        c = (character)ctx->input.datum[ctx->positus];

        si (c == '<')
        {
            frange;
        }

        si (c == '&')
        {
            habet_contentum = VERUM;

            /* Tractare entity reference */
            si (ctx->positus + III < ctx->input.mensura &&
                ctx->input.datum[ctx->positus + I] == 'l' &&
                ctx->input.datum[ctx->positus + II] == 't' &&
                ctx->input.datum[ctx->positus + III] == ';')
            {
                chorda_aedificator_appendere_character(aed, '<');
                ctx->positus += IV;
                ctx->columna += IV;
            }
            alioquin si (ctx->positus + III < ctx->input.mensura &&
                         ctx->input.datum[ctx->positus + I] == 'g' &&
                         ctx->input.datum[ctx->positus + II] == 't' &&
                         ctx->input.datum[ctx->positus + III] == ';')
            {
                chorda_aedificator_appendere_character(aed, '>');
                ctx->positus += IV;
                ctx->columna += IV;
            }
            alioquin si (ctx->positus + IV < ctx->input.mensura &&
                         ctx->input.datum[ctx->positus + I] == 'a' &&
                         ctx->input.datum[ctx->positus + II] == 'm' &&
                         ctx->input.datum[ctx->positus + III] == 'p' &&
                         ctx->input.datum[ctx->positus + IV] == ';')
            {
                chorda_aedificator_appendere_character(aed, '&');
                ctx->positus += V;
                ctx->columna += V;
            }
            alioquin si (ctx->positus + V < ctx->input.mensura &&
                         ctx->input.datum[ctx->positus + I] == 'q' &&
                         ctx->input.datum[ctx->positus + II] == 'u' &&
                         ctx->input.datum[ctx->positus + III] == 'o' &&
                         ctx->input.datum[ctx->positus + IV] == 't' &&
                         ctx->input.datum[ctx->positus + V] == ';')
            {
                chorda_aedificator_appendere_character(aed, '"');
                ctx->positus += VI;
                ctx->columna += VI;
            }
            alioquin si (ctx->positus + V < ctx->input.mensura &&
                         ctx->input.datum[ctx->positus + I] == 'a' &&
                         ctx->input.datum[ctx->positus + II] == 'p' &&
                         ctx->input.datum[ctx->positus + III] == 'o' &&
                         ctx->input.datum[ctx->positus + IV] == 's' &&
                         ctx->input.datum[ctx->positus + V] == ';')
            {
                chorda_aedificator_appendere_character(aed, '\'');
                ctx->positus += VI;
                ctx->columna += VI;
            }
            alioquin
            {
                chorda_aedificator_appendere_character(aed, '&');
                ctx->positus++;
                ctx->columna++;
            }
        }
        alioquin
        {
            si (!_est_spatium(c))
            {
                habet_contentum = VERUM;
            }

            si (c == '\n')
            {
                ctx->linea++;
                ctx->columna = I;
            }
            alioquin
            {
                ctx->columna++;
            }

            chorda_aedificator_appendere_character(aed, c);
            ctx->positus++;
        }
    }

    si (!habet_contentum)
    {
        chorda_aedificator_destruere(aed);
        redde NIHIL;
    }

    resultus = chorda_aedificator_finire(aed);
    redde chorda_internare(ctx->intern, resultus);
}

/* Declaratio anticipata */
interior XmlNodus* _legere_elementum(XmlContextus* ctx);

/* Legere contentum elementi (liberi) */
interior b32
_legere_contentum(XmlContextus* ctx, XmlNodus* parens)
{
    dum (ctx->positus < ctx->input.mensura)
    {
        character c;

        _praeterire_spatium(&ctx->input, &ctx->positus);

        si (ctx->positus >= ctx->input.mensura)
        {
            frange;
        }

        c = (character)ctx->input.datum[ctx->positus];

        si (c == '<')
        {
            /* Verificare si tag claudens */
            si (ctx->positus + I < ctx->input.mensura &&
                ctx->input.datum[ctx->positus + I] == '/')
            {
                /* Tag claudens - finis contentum */
                redde VERUM;
            }

            /* Verificare si commentum */
            si (ctx->positus + III < ctx->input.mensura &&
                ctx->input.datum[ctx->positus + I] == '!' &&
                ctx->input.datum[ctx->positus + II] == '-' &&
                ctx->input.datum[ctx->positus + III] == '-')
            {
                /* Saltare commentum */
                ctx->positus += IV;
                ctx->columna += IV;

                dum (ctx->positus + II < ctx->input.mensura)
                {
                    si (ctx->input.datum[ctx->positus] == '-' &&
                        ctx->input.datum[ctx->positus + I] == '-' &&
                        ctx->input.datum[ctx->positus + II] == '>')
                    {
                        ctx->positus += III;
                        ctx->columna += III;
                        frange;
                    }

                    si (ctx->input.datum[ctx->positus] == '\n')
                    {
                        ctx->linea++;
                        ctx->columna = I;
                    }
                    alioquin
                    {
                        ctx->columna++;
                    }
                    ctx->positus++;
                }

                perge;
            }

            /* Verificare si declaratio XML (<?xml ...?>) */
            si (ctx->positus + I < ctx->input.mensura &&
                ctx->input.datum[ctx->positus + I] == '?')
            {
                /* Saltare declaratio */
                ctx->positus += II;
                ctx->columna += II;

                dum (ctx->positus + I < ctx->input.mensura)
                {
                    si (ctx->input.datum[ctx->positus] == '?' &&
                        ctx->input.datum[ctx->positus + I] == '>')
                    {
                        ctx->positus += II;
                        ctx->columna += II;
                        frange;
                    }

                    si (ctx->input.datum[ctx->positus] == '\n')
                    {
                        ctx->linea++;
                        ctx->columna = I;
                    }
                    alioquin
                    {
                        ctx->columna++;
                    }
                    ctx->positus++;
                }

                perge;
            }

            /* Elementum liberum */
            {
                XmlNodus* liberum;

                liberum = _legere_elementum(ctx);
                si (!liberum)
                {
                    redde FALSUM;
                }

                xml_liberum_addere(parens, liberum);
            }
        }
        alioquin
        {
            /* Textus */
            chorda* textus;
            XmlNodus* nodus_textus;

            textus = _legere_textus(ctx);
            si (textus)
            {
                nodus_textus = xml_textum_creare_ex_chorda(ctx->piscina, ctx->intern,
                                                           *textus);
                si (nodus_textus)
                {
                    xml_liberum_addere(parens, nodus_textus);
                }
            }
        }
    }

    redde VERUM;
}

/* Legere elementum */
interior XmlNodus*
_legere_elementum(XmlContextus* ctx)
{
    XmlNodus* nodus;
    chorda* tag_titulus;

    /* Expectare < */
    si (ctx->positus >= ctx->input.mensura ||
        ctx->input.datum[ctx->positus] != '<')
    {
        ctx->status = XML_ERROR_SYNTAXIS;
        redde NIHIL;
    }

    ctx->positus++;
    ctx->columna++;

    /* Legere titulum */
    tag_titulus = _legere_titulus(ctx);
    si (!tag_titulus)
    {
        ctx->status = XML_ERROR_SYNTAXIS;
        redde NIHIL;
    }

    /* Creare nodum */
    nodus = piscina_allocare(ctx->piscina, magnitudo(XmlNodus));
    si (!nodus)
    {
        ctx->status = XML_ERROR_MEMORIA;
        redde NIHIL;
    }

    nodus->genus = XML_NODUS_ELEMENTUM;
    nodus->titulus = tag_titulus;
    nodus->valor = NIHIL;
    nodus->attributa = xar_creare(ctx->piscina, magnitudo(XmlAttributum));
    nodus->liberi = xar_creare(ctx->piscina, magnitudo(XmlNodus*));
    nodus->parens = NIHIL;

    /* Legere attributa */
    dum (ctx->positus < ctx->input.mensura)
    {
        character c;

        _praeterire_spatium(&ctx->input, &ctx->positus);

        si (ctx->positus >= ctx->input.mensura)
        {
            ctx->status = XML_ERROR_TAG_NON_CLAUSUM;
            redde NIHIL;
        }

        c = (character)ctx->input.datum[ctx->positus];

        /* Self-closing tag */
        si (c == '/')
        {
            si (ctx->positus + I >= ctx->input.mensura ||
                ctx->input.datum[ctx->positus + I] != '>')
            {
                ctx->status = XML_ERROR_SYNTAXIS;
                redde NIHIL;
            }

            ctx->positus += II;
            ctx->columna += II;
            redde nodus;
        }

        /* Finis tag aperiens */
        si (c == '>')
        {
            ctx->positus++;
            ctx->columna++;
            frange;
        }

        /* Attributum */
        {
            chorda* attr_titulus;
            chorda* attr_valor;
            XmlAttributum* attr;

            attr_titulus = _legere_titulus(ctx);
            si (!attr_titulus)
            {
                ctx->status = XML_ERROR_ATTRIBUTUM;
                redde NIHIL;
            }

            _praeterire_spatium(&ctx->input, &ctx->positus);

            si (ctx->positus >= ctx->input.mensura ||
                ctx->input.datum[ctx->positus] != '=')
            {
                ctx->status = XML_ERROR_ATTRIBUTUM;
                redde NIHIL;
            }

            ctx->positus++;
            ctx->columna++;

            _praeterire_spatium(&ctx->input, &ctx->positus);

            attr_valor = _legere_valor_attributi(ctx);
            si (!attr_valor)
            {
                ctx->status = XML_ERROR_ATTRIBUTUM;
                redde NIHIL;
            }

            attr = xar_addere(nodus->attributa);
            si (attr)
            {
                attr->titulus = attr_titulus;
                attr->valor = attr_valor;
            }
        }
    }

    /* Legere contentum */
    si (!_legere_contentum(ctx, nodus))
    {
        redde NIHIL;
    }

    /* Expectare tag claudens </titulus> */
    _praeterire_spatium(&ctx->input, &ctx->positus);

    si (ctx->positus + I >= ctx->input.mensura ||
        ctx->input.datum[ctx->positus] != '<' ||
        ctx->input.datum[ctx->positus + I] != '/')
    {
        ctx->status = XML_ERROR_TAG_NON_CLAUSUM;
        redde NIHIL;
    }

    ctx->positus += II;
    ctx->columna += II;

    {
        chorda* titulus_claudens;

        titulus_claudens = _legere_titulus(ctx);
        si (!titulus_claudens)
        {
            ctx->status = XML_ERROR_SYNTAXIS;
            redde NIHIL;
        }

        /* Verificare si tituli congruunt */
        si (titulus_claudens != tag_titulus)
        {
            /* Comparare chordas si non idem pointer */
            si (!chorda_aequalis(*titulus_claudens, *tag_titulus))
            {
                ctx->status = XML_ERROR_TAG_IMPROPRIE;
                redde NIHIL;
            }
        }
    }

    _praeterire_spatium(&ctx->input, &ctx->positus);

    si (ctx->positus >= ctx->input.mensura ||
        ctx->input.datum[ctx->positus] != '>')
    {
        ctx->status = XML_ERROR_SYNTAXIS;
        redde NIHIL;
    }

    ctx->positus++;
    ctx->columna++;

    redde nodus;
}

XmlResultus
xml_legere(
    chorda               input,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    XmlResultus resultus;
    XmlContextus ctx;

    resultus.successus = FALSUM;
    resultus.radix = NIHIL;
    resultus.status = XML_SUCCESSUS;
    resultus.linea_erroris = ZEPHYRUM;
    resultus.columna_erroris = ZEPHYRUM;
    resultus.error.datum = NIHIL;
    resultus.error.mensura = ZEPHYRUM;

    si (!piscina || !intern)
    {
        resultus.status = XML_ERROR_MEMORIA;
        redde resultus;
    }

    si (input.mensura == ZEPHYRUM || !input.datum)
    {
        resultus.status = XML_ERROR_VACUUM_INPUT;
        redde resultus;
    }

    ctx.input = input;
    ctx.positus = ZEPHYRUM;
    ctx.linea = I;
    ctx.columna = I;
    ctx.piscina = piscina;
    ctx.intern = intern;
    ctx.status = XML_SUCCESSUS;
    ctx.error.datum = NIHIL;
    ctx.error.mensura = ZEPHYRUM;

    /* Saltare spatium et declarationes initiales */
    dum (ctx.positus < ctx.input.mensura)
    {
        character c;

        _praeterire_spatium(&ctx.input, &ctx.positus);

        si (ctx.positus >= ctx.input.mensura)
        {
            frange;
        }

        c = (character)ctx.input.datum[ctx.positus];

        si (c == '<')
        {
            /* Verificare si declaratio XML */
            si (ctx.positus + I < ctx.input.mensura &&
                ctx.input.datum[ctx.positus + I] == '?')
            {
                /* Saltare declaratio */
                ctx.positus += II;
                ctx.columna += II;

                dum (ctx.positus + I < ctx.input.mensura)
                {
                    si (ctx.input.datum[ctx.positus] == '?' &&
                        ctx.input.datum[ctx.positus + I] == '>')
                    {
                        ctx.positus += II;
                        ctx.columna += II;
                        frange;
                    }
                    ctx.positus++;
                    ctx.columna++;
                }

                perge;
            }

            /* Verificare si commentum */
            si (ctx.positus + III < ctx.input.mensura &&
                ctx.input.datum[ctx.positus + I] == '!' &&
                ctx.input.datum[ctx.positus + II] == '-' &&
                ctx.input.datum[ctx.positus + III] == '-')
            {
                /* Saltare commentum */
                ctx.positus += IV;
                ctx.columna += IV;

                dum (ctx.positus + II < ctx.input.mensura)
                {
                    si (ctx.input.datum[ctx.positus] == '-' &&
                        ctx.input.datum[ctx.positus + I] == '-' &&
                        ctx.input.datum[ctx.positus + II] == '>')
                    {
                        ctx.positus += III;
                        ctx.columna += III;
                        frange;
                    }
                    ctx.positus++;
                    ctx.columna++;
                }

                perge;
            }

            /* Elementum radix */
            frange;
        }
        alioquin
        {
            ctx.positus++;
            ctx.columna++;
        }
    }

    /* Legere elementum radix */
    resultus.radix = _legere_elementum(&ctx);

    si (ctx.status != XML_SUCCESSUS)
    {
        resultus.status = ctx.status;
        resultus.linea_erroris = ctx.linea;
        resultus.columna_erroris = ctx.columna;
        redde resultus;
    }

    si (!resultus.radix)
    {
        resultus.status = XML_ERROR_VACUUM_INPUT;
        redde resultus;
    }

    resultus.successus = VERUM;
    resultus.status = XML_SUCCESSUS;

    redde resultus;
}

XmlResultus
xml_legere_ex_literis(
    constans character*  cstr,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    chorda input;
    i32 len;
    unio {
        constans character* cc;
        i8* uc;
    } conv;

    len = ZEPHYRUM;
    dum (cstr[len] != '\0')
    {
        len++;
    }

    /* Conversio constans -> non-constans (XML parser non mutat input) */
    conv.cc = cstr;
    input.datum = conv.uc;
    input.mensura = len;

    redde xml_legere(input, piscina, intern);
}
