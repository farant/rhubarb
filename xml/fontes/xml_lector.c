/* xml_lector.c - Lector xml (vide xml_lector.h)
 *
 * Functio (modi, positionis): nulla memoria praeter situm et modum.
 * Lexema = segmentum fontis [cursor, ad); fons numquam copiatur.
 */

#include "xml_lector.h"
#include "xml_nomina.h"
#include "utf8.h"
#include <string.h>


/* ==================================================
 * Octeti et situs
 * ================================================== */

interior s32
_octetus (
    constans XmlLector* l,
                   s32  i)
{
    si (i < ZEPHYRUM || i >= l->mensura)
    {
        redde (s32)-I;
    }
    redde (s32)(insignatus character)l->fons[i];
}

/* fons ab i incipit a litteris 'verbum'? */
interior b32
_incipit (
    constans XmlLector* l,
                   s32  i,
    constans character* verbum)
{
    s32 n = (s32)strlen(verbum);

    redde i + n <= l->mensura
        && memcmp(l->fons + i, verbum, (size_t)n) == ZEPHYRUM;
}

/* positio primae occurrentiae 'verbum' ab i, aut -I */
interior s32
_quaerere (
    constans XmlLector* l,
                   s32  i,
    constans character* verbum)
{
    s32 n = (s32)strlen(verbum);
    s32 j;

    per (j = i; j + n <= l->mensura; j++)
    {
        si (   l->fons[j] == verbum[ZEPHYRUM]
            && memcmp(l->fons + j, verbum, (size_t)n) == ZEPHYRUM)
        {
            redde j;
        }
    }
    redde (s32)-I;
}

/* S (spatium, TAB, LF, CR) */
interior b32
_spatium (
    s32 c)
{
    redde c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

interior vacuum
_progredi (
    XmlLector* l,
          s32  ad)
{
    s32 i;

    per (i = l->cursor; i < ad; i++)
    {
        si (l->fons[i] == '\n')
        {
            l->linea++;
            l->linea_initium = i + I;
        }
    }
    l->cursor = ad;
}

/* chorda.datum i8* est sed fons constans: unio (exemplar toml) */
interior chorda
_segmentum (
    constans character* fons,
                   s32  ab,
                   s32  ad)
{
    chorda c;
    unio { constans character* c; i8* m; } u;

    u.c        = fons + ab;
    c.datum    = u.m;
    c.mensura  = (i32)(ad - ab);
    redde c;
}

/* lexema [cursor, ad) generis dati */
interior MateriaToken*
_facere (
    XmlLector* l,
          s32  genus,
          s32  ad)
{
    MateriaToken* t;
             s32  ab = l->cursor;

    t = materia_token_creare(l->piscina, &l->forma, genus,
        _segmentum(l->fons, ab, ad), ab, l->linea,
        (i32)(ab - l->linea_initium) + I, ZEPHYRUM);
    si (t == NIHIL)
    {
        redde NIHIL;
    }
    _progredi(l, ad);
    redde t;
}

/* sequentia UTF-8 una (aut octetus unus si invalida) ut IGNOTUM */
interior MateriaToken*
_ignotum (
    XmlLector* l)
{
    constans i8* p        = (constans i8*)(l->fons + l->cursor);
    constans i8* initium  = p;
    constans i8* finis    = (constans i8*)(l->fons + l->mensura);
            s32  longitudo;

    (vacuum)utf8_decodere(&p, finis);
    longitudo = (s32)(p - initium);
    si (longitudo < (s32)I)
    {
        longitudo = (s32)I;
    }
    redde _facere(l, (s32)XML_LEX_IGNOTUM, l->cursor + longitudo);
}

/* longitudo tituli (aut Nmtoken) ab i; 0 si nullus */
interior s32
_titulus (
    constans XmlLector* l,
                   s32  i,
                   b32  nmtoken)
{
    redde xml_titulum_metiri(l->fons + i, l->mensura - i, nmtoken);
}


/* ==================================================
 * Lexemata communia
 * ================================================== */

interior MateriaToken*
_spatia (
    XmlLector* l)
{
    s32 j = l->cursor;

    dum (_spatium(_octetus(l, j)))
    {
        j++;
    }
    redde _facere(l, (s32)XML_LEX_SPATIA, j);
}

/* lexema totum usque ad 'claudens' (inclusum) ab i + initium, aut ad
 * finem fontis */
interior MateriaToken*
_usque (
             XmlLector* l,
                   s32  genus,
                   s32  initium,
    constans character* claudens)
{
    s32 j = _quaerere(l, l->cursor + initium, claudens);

    redde _facere(l, genus, j < ZEPHYRUM ? l->mensura
        : j + (s32)strlen(claudens));
}

/* "..." aut '...': ad citationem claudentem; si non clauditur, ante
 * '<' (sistere_ad_minor) aut ad finem fontis */
interior MateriaToken*
_valor (
    XmlLector* l,
          b32  sistere_ad_minor)
{
    s32 citatio  = _octetus(l, l->cursor);
    s32 j        = l->cursor + I;

    dum (j < l->mensura)
    {
        s32 c = _octetus(l, j);

        si (c == citatio)
        {
            redde _facere(l, (s32)XML_LEX_VALOR, j + I);
        }
        si (c == '<' && sistere_ad_minor)
        {
            frange;
        }
        j++;
    }
    redde _facere(l, (s32)XML_LEX_VALOR, j);
}

/* &titulus; &#N; &#xH; - longitudo, aut 0 si forma deest */
interior s32
_referentia (
    constans XmlLector* l,
                   s32  i,
                   s32  signum)
{
    s32 j = i + I;
    s32 n;

    si (signum == '&' && _octetus(l, j) == '#')
    {
        j++;
        si (_octetus(l, j) == 'x')
        {
            j++;
            n = j;
            dum (   (_octetus(l, j) >= '0' && _octetus(l, j) <= '9')
                 || (_octetus(l, j) >= 'a' && _octetus(l, j) <= 'f')
                 || (_octetus(l, j) >= 'A' && _octetus(l, j) <= 'F'))
            {
                j++;
            }
        }
        alioquin
        {
            n = j;
            dum (_octetus(l, j) >= '0' && _octetus(l, j) <= '9')
            {
                j++;
            }
        }
        si (j == n || _octetus(l, j) != ';')
        {
            redde ZEPHYRUM;
        }
        redde j + I - i;
    }
    n = _titulus(l, j, FALSUM);
    si (n == ZEPHYRUM || _octetus(l, j + n) != ';')
    {
        redde ZEPHYRUM;
    }
    redde n + II;
}


/* ==================================================
 * Modi
 * ================================================== */

interior MateriaToken*
_contentum (
    XmlLector* l)
{
    s32 i = l->cursor;
    s32 c = _octetus(l, i);
    s32 n;
    s32 j;

    si (   i == ZEPHYRUM && c == 0xEF && _octetus(l, I) == 0xBB
        && _octetus(l, II) == 0xBF)
    {
        redde _facere(l, (s32)XML_LEX_SIGNUM_ORDINIS, III);
    }
    si (c == '<')
    {
        si (_incipit(l, i, "<!--"))
        {
            redde _usque(l, (s32)XML_LEX_COMMENTARIUM, IV, "-->");
        }
        si (_incipit(l, i, "<![CDATA["))
        {
            redde _usque(l, (s32)XML_LEX_CDATA, IX, "]]>");
        }
        si (_incipit(l, i, "<!DOCTYPE"))
        {
            l->modus = XML_MODUS_DOCTYPE;
            redde _facere(l, (s32)XML_LEX_DOCTYPE, i + IX);
        }
        si (   _incipit(l, i, "<?xml")
            && (i == ZEPHYRUM || (i == (s32)III
                && _octetus(l, ZEPHYRUM) == 0xEF
                && _octetus(l, I) == 0xBB && _octetus(l, II) == 0xBF))
            && _spatium(_octetus(l, i + V)))
        {
            l->modus = XML_MODUS_DECLARATIO_XML;
            redde _facere(l, (s32)XML_LEX_DECLARATIO_XML, i + V);
        }
        si (_octetus(l, i + I) == '?')
        {
            redde _usque(l, (s32)XML_LEX_INSTRUCTIO, II, "?>");
        }
        si (_octetus(l, i + I) == '/')
        {
            n = _titulus(l, i + II, FALSUM);
            si (n == ZEPHYRUM)
            {
                redde _facere(l, (s32)XML_LEX_IGNOTUM, i + II);
            }
            l->modus = XML_MODUS_TAG;
            redde _facere(l, (s32)XML_LEX_TAG_CLAUSURA, i + II + n);
        }
        si (_octetus(l, i + I) == '!')
        {
            redde _facere(l, (s32)XML_LEX_IGNOTUM, i + II);
        }
        n = _titulus(l, i + I, FALSUM);
        si (n == ZEPHYRUM)
        {
            redde _facere(l, (s32)XML_LEX_IGNOTUM, i + I);
        }
        l->modus = XML_MODUS_TAG;
        redde _facere(l, (s32)XML_LEX_TAG_APERTURA, i + I + n);
    }
    si (c == '&')
    {
        n = _referentia(l, i, '&');
        si (n == ZEPHYRUM)
        {
            redde _facere(l, (s32)XML_LEX_IGNOTUM, i + I);
        }
        redde _facere(l, (s32)XML_LEX_REFERENTIA, i + n);
    }
    si (_incipit(l, i, "]]>"))
    {
        redde _facere(l, (s32)XML_LEX_IGNOTUM, i + III);
    }
    j = i + I;
    dum (   j < l->mensura && l->fons[j] != '<' && l->fons[j] != '&'
         && !_incipit(l, j, "]]>"))
    {
        j++;
    }
    redde _facere(l, (s32)XML_LEX_TEXTUS, j);
}

/* TAG et DECLARATIO_XML: tituli, '=', valores, claudens */
interior MateriaToken*
_tag (
    XmlLector* l)
{
    s32 i = l->cursor;
    s32 c = _octetus(l, i);
    s32 n;

    si (_spatium(c))
    {
        redde _spatia(l);
    }
    si (l->modus == XML_MODUS_DECLARATIO_XML && _incipit(l, i, "?>"))
    {
        l->modus = XML_MODUS_CONTENTUM;
        redde _facere(l, (s32)XML_LEX_INSTRUCTIO_FINIS, i + II);
    }
    si (l->modus == XML_MODUS_TAG)
    {
        si (c == '>')
        {
            l->modus = XML_MODUS_CONTENTUM;
            redde _facere(l, (s32)XML_LEX_TAG_FINIS, i + I);
        }
        si (_incipit(l, i, "/>"))
        {
            l->modus = XML_MODUS_CONTENTUM;
            redde _facere(l, (s32)XML_LEX_TAG_FINIS_SOLUS, i + II);
        }
    }
    si (c == '=')
    {
        redde _facere(l, (s32)XML_LEX_AEQUALE, i + I);
    }
    si (c == '"' || c == '\'')
    {
        redde _valor(l, VERUM);
    }
    si (c == '<')
    {
        l->modus = XML_MODUS_CONTENTUM;
        redde _contentum(l);
    }
    n = _titulus(l, i, FALSUM);
    si (n > ZEPHYRUM)
    {
        redde _facere(l, (s32)XML_LEX_TITULUS, i + n);
    }
    redde _ignotum(l);
}

interior MateriaToken*
_doctype (
    XmlLector* l)
{
    s32 i = l->cursor;
    s32 c = _octetus(l, i);
    s32 n;

    si (_spatium(c))
    {
        redde _spatia(l);
    }
    si (c == '[')
    {
        l->modus = XML_MODUS_PARS_INTERNA;
        redde _facere(l, (s32)XML_LEX_PARS_INTERNA_APERTURA, i + I);
    }
    si (c == '>')
    {
        l->modus = XML_MODUS_CONTENTUM;
        redde _facere(l, (s32)XML_LEX_TAG_FINIS, i + I);
    }
    si (c == '"' || c == '\'')
    {
        redde _valor(l, FALSUM);
    }
    si (c == '<')
    {
        l->modus = XML_MODUS_CONTENTUM;
        redde _contentum(l);
    }
    n = _titulus(l, i, FALSUM);
    si (n > ZEPHYRUM)
    {
        redde _facere(l, (s32)XML_LEX_TITULUS, i + n);
    }
    redde _ignotum(l);
}

hic_manens constans character* constans DECLARATIONES[] = {
    "<!ELEMENT", "<!ATTLIST", "<!ENTITY", "<!NOTATION", NIHIL
};

interior MateriaToken*
_pars_interna (
    XmlLector* l)
{
    s32 i = l->cursor;
    s32 c = _octetus(l, i);
    s32 n;
    i32 k;

    si (_spatium(c))
    {
        redde _spatia(l);
    }
    si (c == ']')
    {
        l->modus = XML_MODUS_DOCTYPE;
        redde _facere(l, (s32)XML_LEX_PARS_INTERNA_CLAUSURA, i + I);
    }
    si (c == '%')
    {
        n = _referentia(l, i, '%');
        si (n > ZEPHYRUM)
        {
            redde _facere(l, (s32)XML_LEX_REFERENTIA_PARAMETRI, i + n);
        }
        redde _facere(l, (s32)XML_LEX_IGNOTUM, i + I);
    }
    si (c == '<')
    {
        per (k = ZEPHYRUM; DECLARATIONES[k] != NIHIL; k++)
        {
            si (_incipit(l, i, DECLARATIONES[k]))
            {
                l->modus = XML_MODUS_DECLARATIO;
                redde _facere(l, (s32)XML_LEX_DECLARATIO,
                    i + (s32)strlen(DECLARATIONES[k]));
            }
        }
        si (_incipit(l, i, "<!--"))
        {
            redde _usque(l, (s32)XML_LEX_COMMENTARIUM, IV, "-->");
        }
        si (_octetus(l, i + I) == '?')
        {
            redde _usque(l, (s32)XML_LEX_INSTRUCTIO, II, "?>");
        }
        si (_incipit(l, i, "<!["))
        {
            redde _facere(l, (s32)XML_LEX_IGNOTUM, i + III);
        }
        si (_octetus(l, i + I) == '!')
        {
            redde _facere(l, (s32)XML_LEX_IGNOTUM, i + II);
        }
        redde _facere(l, (s32)XML_LEX_IGNOTUM, i + I);
    }
    redde _ignotum(l);
}

interior MateriaToken*
_declaratio (
    XmlLector* l)
{
    s32 i = l->cursor;
    s32 c = _octetus(l, i);
    s32 n;

    si (_spatium(c))
    {
        redde _spatia(l);
    }
    si (c == '>')
    {
        l->modus = XML_MODUS_PARS_INTERNA;
        redde _facere(l, (s32)XML_LEX_TAG_FINIS, i + I);
    }
    si (c == '<' || c == ']')
    {
        l->modus = XML_MODUS_PARS_INTERNA;
        redde _pars_interna(l);
    }
    si (c == '"' || c == '\'')
    {
        redde _valor(l, FALSUM);
    }
    si (c == '%')
    {
        n = _referentia(l, i, '%');
        si (n > ZEPHYRUM)
        {
            redde _facere(l, (s32)XML_LEX_REFERENTIA_PARAMETRI, i + n);
        }
        redde _facere(l, (s32)XML_LEX_DELIM, i + I);
    }
    si (c == '#')
    {
        n = _titulus(l, i + I, FALSUM);
        si (n > ZEPHYRUM)
        {
            redde _facere(l, (s32)XML_LEX_VERBUM_PRAEDEFINITUM,
                i + I + n);
        }
        redde _facere(l, (s32)XML_LEX_IGNOTUM, i + I);
    }
    si (   c == '(' || c == ')' || c == '|' || c == ',' || c == '?'
        || c == '*' || c == '+')
    {
        redde _facere(l, (s32)XML_LEX_DELIM, i + I);
    }
    n = _titulus(l, i, VERUM);
    si (n > ZEPHYRUM)
    {
        redde _facere(l, (s32)XML_LEX_TITULUS, i + n);
    }
    redde _ignotum(l);
}


/* ==================================================
 * Publica
 * ================================================== */

b32
xml_lector_incipere (
             XmlLector* lector,
               Piscina* piscina,
    constans character* fons,
                   s32  mensura)
{
    si (fons == NIHIL && mensura == ZEPHYRUM)
    {
        fons = "";
    }
    si (   lector == NIHIL || piscina == NIHIL || fons == NIHIL
        || mensura < ZEPHYRUM)
    {
        redde FALSUM;
    }
    lector->piscina               = piscina;
    lector->fons                  = fons;
    lector->mensura               = mensura;
    lector->cursor                = ZEPHYRUM;
    lector->linea                 = I;
    lector->linea_initium         = ZEPHYRUM;
    lector->modus                 = XML_MODUS_CONTENTUM;
    lector->forma.mensura_caudae  = ZEPHYRUM;
    redde VERUM;
}

MateriaToken*
xml_lector_proximum (
    XmlLector* l)
{
    si (l->cursor >= l->mensura)
    {
        redde _facere(l, (s32)XML_LEX_FINIS, l->mensura);
    }
    commutatio (l->modus)
    {
        casus XML_MODUS_TAG:
        casus XML_MODUS_DECLARATIO_XML:
            redde _tag(l);
        casus XML_MODUS_DOCTYPE:
            redde _doctype(l);
        casus XML_MODUS_PARS_INTERNA:
            redde _pars_interna(l);
        casus XML_MODUS_DECLARATIO:
            redde _declaratio(l);
        casus XML_MODUS_CONTENTUM:
        ordinarius:
            frange;
    }
    redde _contentum(l);
}

XmlModus
xml_lector_modus (
    constans XmlLector* lector)
{
    redde lector->modus;
}
