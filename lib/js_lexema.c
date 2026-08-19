/* js_lexema.c - vide caput. Lexator manu scriptus super octetis
 * crudis. Duo status trans lexemata feruntur: acervus formularum
 * (interpolationes apertae, numerus '{' intra quamque - ut '}'
 * resumens a '}' interpunctionis distinguatur) et lexema
 * significans prius (pro '/' iudicando: regex an divisio).
 * Cursor semper progreditur, ergo lexatio totalis terminatur et
 * lexemata fontem perfecte tegulant - unde fidelitas. */
#include "js_lexema.h"
#include "chorda_aedificator.h"

#include <string.h>


/* ==================================================
 * classes octetorum
 * ================================================== */

interior b32
_est_spatium (character c);

interior b32
_est_spatium (
    character c)
{
    redde c == ' ' || c == '\t' || c == '\n' || c == '\r'
        || c == '\f' || c == '\v';
}

interior b32
_est_linea_nova (character c);

interior b32
_est_linea_nova (
    character c)
{
    redde c == '\n' || c == '\r';
}

interior b32
_est_digitus (character c);

interior b32
_est_digitus (
    character c)
{
    redde c >= '0' && c <= '9';
}

interior b32
_est_hex (character c);

interior b32
_est_hex (
    character c)
{
    redde _est_digitus(c) || (c >= 'a' && c <= 'f')
        || (c >= 'A' && c <= 'F');
}

interior b32
_est_nominis_initium (character c);

interior b32
_est_nominis_initium (
    character c)
{
    redde (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
        || c == '_' || c == '$' || (insignatus character)c >= 0x80;
}

interior b32
_est_nominis (character c);

interior b32
_est_nominis (
    character c)
{
    redde _est_nominis_initium(c) || _est_digitus(c);
}

/* chorda ex fonte (visus, nulla copia): unio castum celat; nemo
 * per visum scribit (contractus; mos silvae/css/html) */
interior chorda
_chorda_ex_fonte (constans character* fons, i32 mensura);

interior chorda
_chorda_ex_fonte (
    constans character* fons,
                   i32  mensura)
{
    chorda c;
    unio { constans character* c; i8* m; } u;

    u.c        = fons;
    c.datum    = u.m;
    c.mensura  = mensura;
    redde c;
}


/* ==================================================
 * verba reservata
 * ================================================== */

interior constans character* constans _verba[] = {
    "break", "case", "catch", "class", "const", "continue",
    "debugger", "default", "delete", "do", "else", "extends",
    "false", "finally", "for", "function", "if", "in",
    "instanceof", "let", "new", "null", "return", "static",
    "super", "switch", "this", "throw", "true", "try", "typeof",
    "var", "void", "while", "with", "yield", NIHIL
};

interior b32
_est_verbum (constans character* datum, i32 mensura);

interior b32
_est_verbum (
    constans character* datum,
                   i32  mensura)
{
    i32 v;

    per (v = 0; _verba[v] != NIHIL; v = v + 1)
    {
        si (   strncmp(datum, _verba[v], (memoriae_index)mensura) == 0
            && _verba[v][mensura] == '\0')
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* verba post quae '/' divisio est (valorem ferunt) */
interior b32
_verbum_valorem_fert (constans character* datum, i32 mensura);

interior b32
_verbum_valorem_fert (
    constans character* datum,
                   i32  mensura)
{
    interior constans character* constans valentia[] = {
        "this", "true", "false", "null", "super", NIHIL
    };
    i32 v;

    per (v = 0; valentia[v] != NIHIL; v = v + 1)
    {
        si (   strncmp(datum, valentia[v],
                (memoriae_index)mensura) == 0
            && valentia[v][mensura] == '\0')
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}


/* ==================================================
 * status lexatoris
 * ================================================== */

#define FORMULARUM_TECTUM 64

nomen structura {
    constans character* fons;
                   i32  mensura;
                   i32  k;              /* cursor */
    /* acervus interpolationum: pro quaque aperta, numerus '{'
       apertorum intra eam (0 = '}' proximus formulam resumit) */
    i32 acervus[FORMULARUM_TECTUM];
    i32 altitudo;
    /* lexema significans prius (trivia exclusa) - '/' iudicat */
         JsLexemaGenus  prior_genus;
    constans character* prior_datum;
                   i32  prior_mensura;
} JsLexator;

interior character
_ad (JsLexator* lx, i32 delta);

interior character
_ad (
    JsLexator* lx,
          i32  delta)
{
    si (lx->k + delta >= lx->mensura)
    {
        redde '\0';   /* sentinella - numquam consumitur */
    }
    redde lx->fons[lx->k + delta];
}

interior b32
_intra (JsLexator* lx, i32 delta);

interior b32
_intra (
    JsLexator* lx,
          i32  delta)
{
    redde lx->k + delta < lx->mensura;
}

/* estne '/' hic divisio (non regex)? heuristice classica super
 * lexemate significante priore; simplificationes in capite */
interior b32
_divisio_hic (JsLexator* lx);

interior b32
_divisio_hic (
    JsLexator* lx)
{
    commutatio (lx->prior_genus)
    {
        casus JS_LEX_IDENTIFICATOR:
        casus JS_LEX_NUMERUS:
        casus JS_LEX_CHORDA:
        casus JS_LEX_CHORDA_IMPERFECTA:
        casus JS_LEX_REGEX:
        casus JS_LEX_REGEX_IMPERFECTA:
        casus JS_LEX_FORMULA_TOTA:
        casus JS_LEX_FORMULA_FINIS:
            redde VERUM;
        casus JS_LEX_VERBUM:
            redde _verbum_valorem_fert(lx->prior_datum,
                lx->prior_mensura);
        casus JS_LEX_INTERPUNCTIO:
        {
            character u = lx->prior_datum[lx->prior_mensura - 1];

            si (u == ')' || u == ']')
            {
                redde VERUM;
            }
            /* '++'/'--' postfixa praesumpta (a++ / b divisio) */
            si (   lx->prior_mensura  == 2
                && (u == '+' || u == '-')
                && lx->prior_datum[0] == u)
            {
                redde VERUM;
            }
            redde FALSUM;
        }
        ordinarius:
            redde FALSUM;   /* initium, formula aperta, ignotum */
    }
}


/* ==================================================
 * consumptores
 * ================================================== */

/* chordam consumere (cursor in citatione claudente) */
interior JsLexemaGenus
_chordam_consumere (JsLexator* lx);

interior JsLexemaGenus
_chordam_consumere (
    JsLexator* lx)
{
    character claudens = _ad(lx, 0);

    lx->k = lx->k + 1;
    dum (_intra(lx, 0))
    {
        character c = _ad(lx, 0);

        si (c == claudens)
        {
            lx->k = lx->k + 1;
            redde JS_LEX_CHORDA;
        }
        si (_est_linea_nova(c))
        {
            /* linea nova NON consumitur (mos css_lexema) */
            redde JS_LEX_CHORDA_IMPERFECTA;
        }
        si (c == '\\')
        {
            si (!_intra(lx, 1))
            {
                lx->k = lx->k + 1;
                perge;
            }
            /* continuatio '\' + \r\n = una */
            si (_ad(lx, 1) == '\r' && _ad(lx, 2) == '\n')
            {
                lx->k = lx->k + 3;
                perge;
            }
            lx->k = lx->k + 2;   /* effugium quodlibet (et \n) */
            perge;
        }
        lx->k = lx->k + 1;
    }
    redde JS_LEX_CHORDA_IMPERFECTA;
}

/* numerum consumere: digiti [. digiti] [eE [+-] digiti],
 * 0x hex, 0b/0o (digiti lati - arbor validat) */
interior vacuum
_numerum_consumere (JsLexator* lx);

interior vacuum
_numerum_consumere (
    JsLexator* lx)
{
    si (   _ad(lx, 0) == '0'
        && (_ad(lx, 1) == 'x' || _ad(lx, 1) == 'X'))
    {
        lx->k = lx->k + 2;
        dum (_est_hex(_ad(lx, 0)))
        {
            lx->k = lx->k + 1;
        }
        redde;
    }
    si (   _ad(lx, 0) == '0'
        && (_ad(lx, 1) == 'b' || _ad(lx, 1) == 'B'
            || _ad(lx, 1) == 'o' || _ad(lx, 1) == 'O'))
    {
        lx->k = lx->k + 2;
        dum (_est_digitus(_ad(lx, 0)))
        {
            lx->k = lx->k + 1;
        }
        redde;
    }
    dum (_est_digitus(_ad(lx, 0)))
    {
        lx->k = lx->k + 1;
    }
    si (_ad(lx, 0) == '.')
    {
        lx->k = lx->k + 1;
        dum (_est_digitus(_ad(lx, 0)))
        {
            lx->k = lx->k + 1;
        }
    }
    si (   (_ad(lx, 0) == 'e' || _ad(lx, 0) == 'E')
        && (_est_digitus(_ad(lx, 1))
            || ((_ad(lx, 1) == '+' || _ad(lx, 1) == '-')
                && _est_digitus(_ad(lx, 2)))))
    {
        lx->k = lx->k + 2;
        dum (_est_digitus(_ad(lx, 0)))
        {
            lx->k = lx->k + 1;
        }
    }
}

/* regex consumere (cursor in '/', divisio et commentarium iam
 * exclusa): corpus cum classibus [..] ('/' intus litteralis),
 * effugiis; deinde vexilla. Linea nova cruda -> IMPERFECTA. */
interior JsLexemaGenus
_regex_consumere (JsLexator* lx);

interior JsLexemaGenus
_regex_consumere (
    JsLexator* lx)
{
    b32 in_classe = FALSUM;

    lx->k = lx->k + 1;
    dum (_intra(lx, 0))
    {
        character c = _ad(lx, 0);

        si (_est_linea_nova(c))
        {
            redde JS_LEX_REGEX_IMPERFECTA;   /* non consumpta */
        }
        si (c == '\\')
        {
            si (_intra(lx, 1) && !_est_linea_nova(_ad(lx, 1)))
            {
                lx->k = lx->k + 2;
                perge;
            }
            lx->k = lx->k + 1;
            perge;
        }
        si (c == '[')
        {
            in_classe = VERUM;
        }
        alioquin si (c == ']')
        {
            in_classe = FALSUM;
        }
        alioquin si (c == '/' && !in_classe)
        {
            lx->k = lx->k + 1;
            dum (_est_nominis(_ad(lx, 0)))
            {
                lx->k = lx->k + 1;   /* vexilla gi... */
            }
            redde JS_LEX_REGEX;
        }
        lx->k = lx->k + 1;
    }
    redde JS_LEX_REGEX_IMPERFECTA;
}

/* corpus formulae scandere (cursor post '`' aut '}' resumens):
 * ad '`' (finis) aut '${' (interpolatio - acervus crescit) aut
 * EOF. Lineae novae intus LICITAE (lexema multilineare). */
interior JsLexemaGenus
_formulam_scandere (JsLexator* lx, b32 ab_initio);

interior JsLexemaGenus
_formulam_scandere (
    JsLexator* lx,
          b32  ab_initio)
{
    dum (_intra(lx, 0))
    {
        character c = _ad(lx, 0);

        si (c == '\\')
        {
            si (_intra(lx, 1))
            {
                lx->k = lx->k + 2;
            }
            alioquin
            {
                lx->k = lx->k + 1;
            }
            perge;
        }
        si (c == '`')
        {
            lx->k = lx->k + 1;
            redde ab_initio ? JS_LEX_FORMULA_TOTA
                            : JS_LEX_FORMULA_FINIS;
        }
        si (c == '$' && _ad(lx, 1) == '{')
        {
            si (lx->altitudo < FORMULARUM_TECTUM)
            {
                lx->k                      = lx->k + 2;
                lx->acervus[lx->altitudo]  = 0;
                lx->altitudo               = lx->altitudo + 1;
                redde ab_initio ? JS_LEX_FORMULA_INITIUM
                                : JS_LEX_FORMULA_MEDIA;
            }
            /* acervus plenus: '${' pars formulae manet
               (degradatio totalis, numquam fractura) */
            lx->k = lx->k + 2;
            perge;
        }
        lx->k = lx->k + 1;
    }
    redde JS_LEX_FORMULA_IMPERFECTA;
}

/* mensura interpunctionis ad cursorem (munch maximus), 0 si
 * octetus nulli operatori congruit */
interior i32
_interpunctionis_mensura (JsLexator* lx);

interior i32
_interpunctionis_mensura (
    JsLexator* lx)
{
    character a = _ad(lx, 0);
    character b = _ad(lx, 1);
    character c = _ad(lx, 2);
    character d = _ad(lx, 3);

    si (a == '>' && b == '>' && c == '>' && d == '=')
    {
        redde 4;
    }
    si (   (a == '=' && b == '=' && c == '=')
        || (a == '!' && b == '=' && c == '=')
        || (a == '*' && b == '*' && c == '=')
        || (a == '<' && b == '<' && c == '=')
        || (a == '>' && b == '>' && c == '=')
        || (a == '>' && b == '>' && c == '>')
        || (a == '.' && b == '.' && c == '.')
        || (a == '&' && b == '&' && c == '=')
        || (a == '|' && b == '|' && c == '=')
        || (a == '?' && b == '?' && c == '='))
    {
        redde 3;
    }
    si (a == '?' && b == '.')
    {
        /* '?.' + digitus = ternarium cum .5 (regula ES2020) */
        redde _est_digitus(c) ? 1 : 2;
    }
    si (   (a == '=' && b == '>')
        || (a == '=' && b == '=') || (a == '!' && b == '=')
        || (a == '<' && b == '=') || (a == '>' && b == '=')
        || (a == '&' && b == '&') || (a == '|' && b == '|')
        || (a == '+' && b == '+') || (a == '-' && b == '-')
        || (a == '+' && b == '=') || (a == '-' && b == '=')
        || (a == '*' && b == '=') || (a == '/' && b == '=')
        || (a == '%' && b == '=') || (a == '&' && b == '=')
        || (a == '|' && b == '=') || (a == '^' && b == '=')
        || (a == '<' && b == '<') || (a == '>' && b == '>')
        || (a == '*' && b == '*') || (a == '?' && b == '?'))
    {
        redde 2;
    }
    commutatio (a)
    {
        casus '+': casus '-': casus '*': casus '/': casus '%':
        casus '=': casus '<': casus '>': casus '!': casus '&':
        casus '|': casus '^': casus '~': casus '?': casus ':':
        casus ';': casus ',': casus '.': casus '(': casus ')':
        casus '[': casus ']': casus '{': casus '}':
            redde 1;
        ordinarius:
            redde 0;
    }
}


/* ==================================================
 * lexema unum consumere
 * ================================================== */

interior JsLexemaGenus
_lexema_consumere (JsLexator* lx);

interior JsLexemaGenus
_lexema_consumere (
    JsLexator* lx)
{
    character c = _ad(lx, 0);

    /* spatia */
    si (_est_spatium(c))
    {
        dum (_intra(lx, 0) && _est_spatium(_ad(lx, 0)))
        {
            lx->k = lx->k + 1;
        }
        redde JS_LEX_SPATIA;
    }
    /* commentaria et '/' (commentarium ANTE regex - stella aut
       virga post virgam numquam regex incipit) */
    si (c == '/')
    {
        si (_ad(lx, 1) == '/')
        {
            lx->k = lx->k + 2;
            dum (_intra(lx, 0) && !_est_linea_nova(_ad(lx, 0)))
            {
                lx->k = lx->k + 1;
            }
            redde JS_LEX_COMMENTARIUM;   /* ad EOF perfectum */
        }
        si (_ad(lx, 1) == '*')
        {
            lx->k = lx->k + 2;
            dum (_intra(lx, 0))
            {
                si (_ad(lx, 0) == '*' && _ad(lx, 1) == '/')
                {
                    lx->k = lx->k + 2;
                    redde JS_LEX_COMMENTARIUM;
                }
                lx->k = lx->k + 1;
            }
            redde JS_LEX_COMMENTARIUM_IMPERFECTUM;
        }
        si (!_divisio_hic(lx))
        {
            redde _regex_consumere(lx);
        }
        /* divisio: cadit ad interpunctionem ('/=' munch) */
    }
    /* chordae */
    si (c == '\'' || c == '"')
    {
        redde _chordam_consumere(lx);
    }
    /* formula nova */
    si (c == '`')
    {
        lx->k = lx->k + 1;
        redde _formulam_scandere(lx, VERUM);
    }
    /* '}' formulam resumens (interpolatio ad librum) */
    si (   c                             == '}' && lx->altitudo > 0
        && lx->acervus[lx->altitudo - 1] == 0)
    {
        lx->altitudo  = lx->altitudo - 1;
        lx->k         = lx->k + 1;
        redde _formulam_scandere(lx, FALSUM);
    }
    /* numeri */
    si (   _est_digitus(c)
        || (c == '.' && _est_digitus(_ad(lx, 1))))
    {
        _numerum_consumere(lx);
        redde JS_LEX_NUMERUS;
    }
    /* identificatores et verba */
    si (_est_nominis_initium(c))
    {
        i32 initium = lx->k;

        dum (_intra(lx, 0) && _est_nominis(_ad(lx, 0)))
        {
            lx->k = lx->k + 1;
        }
        redde _est_verbum(lx->fons + initium, lx->k - initium)
            ? JS_LEX_VERBUM : JS_LEX_IDENTIFICATOR;
    }
    /* liber bracarum intra interpolationem */
    si (lx->altitudo > 0)
    {
        si (c == '{')
        {
            lx->acervus[lx->altitudo - 1] =
                lx->acervus[lx->altitudo - 1] + 1;
        }
        alioquin si (c == '}')
        {
            lx->acervus[lx->altitudo - 1] =
                lx->acervus[lx->altitudo - 1] - 1;
        }
    }
    /* interpunctio (munch maximus) */
    {
        i32 n = _interpunctionis_mensura(lx);

        si (n > 0)
        {
            lx->k = lx->k + n;
            redde JS_LEX_INTERPUNCTIO;
        }
    }
    /* reliquum - octetus solus (NUL, '#', '@', '\' ...) */
    lx->k = lx->k + 1;
    redde JS_LEX_IGNOTUM;
}


/* ==================================================
 * facies publica
 * ================================================== */

Xar*
js_lexare (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura)
{
    JsLexator  lx;
          Xar* lexemata;
          i32  linea    = 1;
          i32  columna  = 1;

    lexemata = xar_creare(piscina, (i32)magnitudo(JsLexema));
    si (lexemata == NIHIL)
    {
        redde NIHIL;
    }
    lx.fons           = fons;
    lx.mensura        = mensura;
    lx.k              = 0;
    lx.altitudo       = 0;
    lx.prior_genus    = JS_LEX_FINIS;   /* initium: regex licet */
    lx.prior_datum    = NIHIL;
    lx.prior_mensura  = 0;

    dum (lx.k < lx.mensura)
    {
        JsLexema* l;
             i32  initium = lx.k;
             i32  j;

        JsLexemaGenus genus = _lexema_consumere(&lx);

        l = (JsLexema*)xar_addere(lexemata);
        si (l == NIHIL)
        {
            redde NIHIL;
        }
        l->genus = genus;
        l->valor = _chorda_ex_fonte(fons + initium,
            lx.k - initium);
        l->offset   = initium;
        l->linea    = linea;
        l->columna  = columna;
        /* lexema significans prius (trivia exclusa) */
        si (   genus != JS_LEX_SPATIA
            && genus != JS_LEX_COMMENTARIUM
            && genus != JS_LEX_COMMENTARIUM_IMPERFECTUM)
        {
            lx.prior_genus    = genus;
            lx.prior_datum    = fons + initium;
            lx.prior_mensura  = lx.k - initium;
        }
        /* positio proximi: octetos lexematis ambulare */
        per (j = initium; j < lx.k; j = j + 1)
        {
            si (fons[j] == '\n')
            {
                linea    = linea + 1;
                columna  = 1;
            }
            alioquin
            {
                columna = columna + 1;
            }
        }
    }
    {
        JsLexema* l = (JsLexema*)xar_addere(lexemata);

        si (l == NIHIL)
        {
            redde NIHIL;
        }
        l->genus    = JS_LEX_FINIS;
        l->valor    = _chorda_ex_fonte(fons + mensura, 0);
        l->offset   = mensura;
        l->linea    = linea;
        l->columna  = columna;
    }
    redde lexemata;
}

chorda
js_lexemata_emittere (
    Piscina* piscina,
        Xar* lexemata)
{
    ChordaAedificator* aed;
                  i32  k;

    aed = chorda_aedificator_creare(piscina, (memoriae_index)256);
    per (k = 0; k < xar_numerus(lexemata); k = k + 1)
    {
        JsLexema* l = (JsLexema*)xar_obtinere(lexemata, k);

        chorda_aedificator_appendere_chorda(aed, l->valor);
    }
    redde chorda_aedificator_finire(aed);
}

constans character*
js_lexema_genus_nomen (
    JsLexemaGenus genus)
{
    commutatio (genus)
    {
        casus JS_LEX_FINIS:              redde "finis";
        casus JS_LEX_IDENTIFICATOR:      redde "identificator";
        casus JS_LEX_VERBUM:             redde "verbum";
        casus JS_LEX_CHORDA:             redde "chorda";
        casus JS_LEX_CHORDA_IMPERFECTA:  redde "chorda-imperfecta";
        casus JS_LEX_NUMERUS:            redde "numerus";
        casus JS_LEX_REGEX:              redde "regex";
        casus JS_LEX_REGEX_IMPERFECTA:   redde "regex-imperfecta";
        casus JS_LEX_FORMULA_TOTA:       redde "formula-tota";
        casus JS_LEX_FORMULA_INITIUM:    redde "formula-initium";
        casus JS_LEX_FORMULA_MEDIA:      redde "formula-media";
        casus JS_LEX_FORMULA_FINIS:      redde "formula-finis";
        casus JS_LEX_FORMULA_IMPERFECTA:
            redde "formula-imperfecta";
        casus JS_LEX_SPATIA:             redde "spatia";
        casus JS_LEX_COMMENTARIUM:       redde "commentarium";
        casus JS_LEX_COMMENTARIUM_IMPERFECTUM:
            redde "commentarium-imperfectum";
        casus JS_LEX_INTERPUNCTIO:       redde "interpunctio";
        casus JS_LEX_IGNOTUM:            redde "ignotum";
        ordinarius:                      redde "?";
    }
}
