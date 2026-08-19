/* css_lexema.c - vide caput. Algorithmi CSS Syntax Level 3
 * (consume-a-token, §4.3) super octetis crudis, deviationibus in
 * capite nominatis. Cursor semper progreditur (delim = octetus
 * unus reliquus), ergo lexatio totalis terminatur et lexemata
 * fontem perfecte tegulant - unde fidelitas concatenationis. */
#include "css_lexema.h"
#include "chorda_aedificator.h"

#include <string.h>


/* ==================================================
 * classes octetorum
 * ================================================== */

interior b32
_est_spatium (
    character c);

interior b32
_est_spatium (
    character c)
{
    redde c == ' ' || c == '\t' || c == '\n' || c == '\r'
        || c == '\f';
}

interior b32
_est_linea_nova (
    character c);

interior b32
_est_linea_nova (
    character c)
{
    redde c == '\n' || c == '\r' || c == '\f';
}

interior b32
_est_digitus (
    character c);

interior b32
_est_digitus (
    character c)
{
    redde c >= '0' && c <= '9';
}

interior b32
_est_hex (
    character c);

interior b32
_est_hex (
    character c)
{
    redde _est_digitus(c) || (c >= 'a' && c <= 'f')
        || (c >= 'A' && c <= 'F');
}

interior b32
_est_nominis_initium (
    character c);

interior b32
_est_nominis_initium (
    character c)
{
    redde (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
        || c == '_' || (insignatus character)c >= 0x80;
}

interior b32
_est_nominis (
    character c);

interior b32
_est_nominis (
    character c)
{
    redde _est_nominis_initium(c) || _est_digitus(c) || c == '-';
}

/* chorda ex fonte (visus, nulla copia): chorda.datum i8* est sed
 * fons constans - unio castum celat; nemo per visum scribit
 * (contractus; idem mos silvae) */
interior chorda
_chorda_ex_fonte (
    constans character* fons,
                   i32  mensura);

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

/* non imprimibilis (spec §4.2): 00-08, 0B, 0E-1F, 7F */
interior b32
_non_imprimibilis (
    character c);

interior b32
_non_imprimibilis (
    character c)
{
    insignatus character u = (insignatus character)c;

    redde u <= 0x08 || u == 0x0B || (u >= 0x0E && u <= 0x1F)
        || u == 0x7F;
}


/* ==================================================
 * status lexatoris
 * ================================================== */

nomen structura {
    constans character* fons;
                   i32  mensura;
                   i32  k;        /* cursor */
} CssLexator;

interior character
_ad (
    CssLexator* lx,
           i32  delta);

interior character
_ad (
    CssLexator* lx,
           i32  delta)
{
    si (lx->k + delta >= lx->mensura)
    {
        redde '\0';   /* sentinella - numquam consumitur */
    }
    redde lx->fons[lx->k + delta];
}

interior b32
_intra (
    CssLexator* lx,
           i32  delta);

interior b32
_intra (
    CssLexator* lx,
           i32  delta)
{
    redde lx->k + delta < lx->mensura;
}

/* effugium validum ad positionem k+delta: '\' + non linea nova */
interior b32
_effugium_validum (
    CssLexator* lx,
           i32  delta);

interior b32
_effugium_validum (
    CssLexator* lx,
           i32  delta)
{
    si (!_intra(lx, delta) || _ad(lx, delta) != '\\')
    {
        redde FALSUM;
    }
    si (!_intra(lx, delta + 1))
    {
        redde VERUM;   /* '\' + EOF: effugium (octetum devorat) */
    }
    redde !_est_linea_nova(_ad(lx, delta + 1));
}

/* effugium consumere (cursor in '\'): '\' + hex{1,6} + spatium
 * unum optionale, aut '\' + octetus unus */
interior vacuum
_effugium_consumere (
    CssLexator* lx);

interior vacuum
_effugium_consumere (
    CssLexator* lx)
{
    i32 n = 0;

    lx->k = lx->k + 1;   /* '\' */
    si (!_intra(lx, 0))
    {
        redde;
    }
    si (_est_hex(_ad(lx, 0)))
    {
        dum (n < 6 && _intra(lx, 0) && _est_hex(_ad(lx, 0)))
        {
            lx->k  = lx->k + 1;
            n      = n + 1;
        }
        /* spatium unum post effugium hex PARS effugii est
         * ("\41 x" identificator "Ax" est - fines recti) */
        si (_intra(lx, 0) && _est_spatium(_ad(lx, 0)))
        {
            si (_ad(lx, 0) == '\r' && _ad(lx, 1) == '\n')
            {
                lx->k = lx->k + 2;
            }
            alioquin
            {
                lx->k = lx->k + 1;
            }
        }
        redde;
    }
    lx->k = lx->k + 1;   /* octetus quilibet */
}

/* inciperetne identificator ad k+delta? (spec §4.3.9) */
interior b32
_incipit_identificator (
    CssLexator* lx,
           i32  delta);

interior b32
_incipit_identificator (
    CssLexator* lx,
           i32  delta)
{
    character c = _ad(lx, delta);

    si (c == '-')
    {
        redde _est_nominis_initium(_ad(lx, delta + 1))
            || _ad(lx, delta + 1) == '-'
            || _effugium_validum(lx, delta + 1);
    }
    si (_est_nominis_initium(c))
    {
        redde VERUM;
    }
    redde _effugium_validum(lx, delta);
}

/* inciperetne numerus ad k+delta? (spec §4.3.10) */
interior b32
_incipit_numerus (
    CssLexator* lx,
           i32  delta);

interior b32
_incipit_numerus (
    CssLexator* lx,
           i32  delta)
{
    character c = _ad(lx, delta);

    si (c == '+' || c == '-')
    {
        si (_est_digitus(_ad(lx, delta + 1)))
        {
            redde VERUM;
        }
        redde _ad(lx, delta + 1) == '.'
            && _est_digitus(_ad(lx, delta + 2));
    }
    si (c == '.')
    {
        redde _est_digitus(_ad(lx, delta + 1));
    }
    redde _est_digitus(c);
}

/* nomen consumere (cursor in initio nominis) */
interior vacuum
_nomen_consumere (
    CssLexator* lx);

interior vacuum
_nomen_consumere (
    CssLexator* lx)
{
    dum (_intra(lx, 0))
    {
        si (_est_nominis(_ad(lx, 0)))
        {
            lx->k = lx->k + 1;
        }
        alioquin si (_effugium_validum(lx, 0))
        {
            _effugium_consumere(lx);
        }
        alioquin
        {
            frange;
        }
    }
}

/* numerum consumere: [+-]? digiti [. digiti] [eE [+-] digiti] */
interior vacuum
_numerum_consumere (
    CssLexator* lx);

interior vacuum
_numerum_consumere (
    CssLexator* lx)
{
    si (_ad(lx, 0) == '+' || _ad(lx, 0) == '-')
    {
        lx->k = lx->k + 1;
    }
    dum (_est_digitus(_ad(lx, 0)))
    {
        lx->k = lx->k + 1;
    }
    si (_ad(lx, 0) == '.' && _est_digitus(_ad(lx, 1)))
    {
        lx->k = lx->k + 2;
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

/* url intus consumere (cursor post 'url('); genus redditur.
 * Reliquiae url malae (spec §4.3.14): ad ')' aut EOF. */
interior CssLexemaGenus
_url_consumere (
    CssLexator* lx);

interior CssLexemaGenus
_url_consumere (
    CssLexator* lx)
{
    dum (_est_spatium(_ad(lx, 0)) && _intra(lx, 0))
    {
        lx->k = lx->k + 1;
    }
    dum (VERUM)
    {
        character c;

        si (!_intra(lx, 0))
        {
            redde CSS_LEX_URL_IMPERFECTA;   /* EOF - clamor */
        }
        c = _ad(lx, 0);
        si (c == ')')
        {
            lx->k = lx->k + 1;
            redde CSS_LEX_URL;
        }
        si (_est_spatium(c))
        {
            dum (_est_spatium(_ad(lx, 0)) && _intra(lx, 0))
            {
                lx->k = lx->k + 1;
            }
            si (!_intra(lx, 0))
            {
                redde CSS_LEX_URL_IMPERFECTA;
            }
            si (_ad(lx, 0) == ')')
            {
                lx->k = lx->k + 1;
                redde CSS_LEX_URL;
            }
            frange;   /* spatium intus -> url mala */
        }
        si (   c == '"' || c == '\'' || c == '('
            || _non_imprimibilis(c))
        {
            frange;   /* url mala */
        }
        si (c == '\\')
        {
            si (_effugium_validum(lx, 0))
            {
                _effugium_consumere(lx);
                perge;
            }
            frange;   /* effugium invalidum -> url mala */
        }
        lx->k = lx->k + 1;
    }
    /* reliquiae url malae */
    dum (_intra(lx, 0))
    {
        si (_ad(lx, 0) == ')')
        {
            lx->k = lx->k + 1;
            frange;
        }
        si (_effugium_validum(lx, 0))
        {
            _effugium_consumere(lx);
        }
        alioquin
        {
            lx->k = lx->k + 1;
        }
    }
    redde CSS_LEX_URL_IMPERFECTA;
}

/* chordam consumere (cursor in claudente); genus redditur */
interior CssLexemaGenus
_chordam_consumere (
    CssLexator* lx);

interior CssLexemaGenus
_chordam_consumere (
    CssLexator* lx)
{
    character claudens = _ad(lx, 0);

    lx->k = lx->k + 1;
    dum (_intra(lx, 0))
    {
        character c = _ad(lx, 0);

        si (c == claudens)
        {
            lx->k = lx->k + 1;
            redde CSS_LEX_CHORDA;
        }
        si (_est_linea_nova(c))
        {
            /* linea nova NON consumitur (spec: chorda mala ibi
             * finit; linea sequentibus lexematibus manet) */
            redde CSS_LEX_CHORDA_IMPERFECTA;
        }
        si (c == '\\')
        {
            si (!_intra(lx, 1))
            {
                lx->k = lx->k + 1;   /* '\' + EOF */
                perge;
            }
            si (_est_linea_nova(_ad(lx, 1)))
            {
                /* continuatio: '\' + linea nova (\r\n = una) */
                si (_ad(lx, 1) == '\r' && _ad(lx, 2) == '\n')
                {
                    lx->k = lx->k + 3;
                }
                alioquin
                {
                    lx->k = lx->k + 2;
                }
                perge;
            }
            _effugium_consumere(lx);
            perge;
        }
        lx->k = lx->k + 1;
    }
    redde CSS_LEX_CHORDA_IMPERFECTA;   /* EOF - clamor */
}


/* ==================================================
 * lexema unum consumere (spec §4.3.1)
 * ================================================== */

interior CssLexemaGenus
_lexema_consumere (
    CssLexator* lx);

interior CssLexemaGenus
_lexema_consumere (
    CssLexator* lx)
{
    character c = _ad(lx, 0);

    /* commentarium */
    si (c == '/' && _ad(lx, 1) == '*')
    {
        lx->k = lx->k + 2;
        dum (_intra(lx, 0))
        {
            si (_ad(lx, 0) == '*' && _ad(lx, 1) == '/')
            {
                lx->k = lx->k + 2;
                redde CSS_LEX_COMMENTARIUM;
            }
            lx->k = lx->k + 1;
        }
        redde CSS_LEX_COMMENTARIUM_IMPERFECTUM;
    }
    /* spatia */
    si (_est_spatium(c))
    {
        dum (_intra(lx, 0) && _est_spatium(_ad(lx, 0)))
        {
            lx->k = lx->k + 1;
        }
        redde CSS_LEX_SPATIA;
    }
    /* chordae */
    si (c == '"' || c == '\'')
    {
        redde _chordam_consumere(lx);
    }
    /* cancellum */
    si (c == '#')
    {
        si (_est_nominis(_ad(lx, 1)) || _effugium_validum(lx, 1))
        {
            lx->k = lx->k + 1;
            _nomen_consumere(lx);
            redde CSS_LEX_CANCELLUM;
        }
        lx->k = lx->k + 1;
        redde CSS_LEX_DELIM;
    }
    /* interpunctio simplex */
    si (c == '(')
    { lx->k = lx->k + 1; redde CSS_LEX_PAREN_APERTA;
    }
    si (c == ')')
    { lx->k = lx->k + 1; redde CSS_LEX_PAREN_CLAUSA;
    }
    si (c == '[')
    { lx->k = lx->k + 1; redde CSS_LEX_QUADRA_APERTA;
    }
    si (c == ']')
    { lx->k = lx->k + 1; redde CSS_LEX_QUADRA_CLAUSA;
    }
    si (c == '{')
    { lx->k = lx->k + 1; redde CSS_LEX_BRACE_APERTA;
    }
    si (c == '}')
    { lx->k = lx->k + 1; redde CSS_LEX_BRACE_CLAUSA;
    }
    si (c == ',')
    { lx->k = lx->k + 1; redde CSS_LEX_COMMA;
    }
    si (c == ':')
    { lx->k = lx->k + 1; redde CSS_LEX_COLON;
    }
    si (c == ';')
    { lx->k = lx->k + 1; redde CSS_LEX_SEMICOLON;
    }
    /* numeri et signa ambigua */
    si (c == '+' || c == '.')
    {
        si (_incipit_numerus(lx, 0))
        {
            salta numerus_via;
        }
        lx->k = lx->k + 1;
        redde CSS_LEX_DELIM;
    }
    si (c == '-')
    {
        si (_incipit_numerus(lx, 0))
        {
            salta numerus_via;
        }
        si (_ad(lx, 1) == '-' && _ad(lx, 2) == '>')
        {
            lx->k = lx->k + 3;
            redde CSS_LEX_CDC;
        }
        si (_incipit_identificator(lx, 0))
        {
            salta identificator_via;
        }
        lx->k = lx->k + 1;
        redde CSS_LEX_DELIM;
    }
    si (c == '<')
    {
        si (   _ad(lx, 1) == '!' && _ad(lx, 2) == '-'
            && _ad(lx, 3) == '-')
        {
            lx->k = lx->k + 4;
            redde CSS_LEX_CDO;
        }
        lx->k = lx->k + 1;
        redde CSS_LEX_DELIM;
    }
    si (c == '@')
    {
        si (_incipit_identificator(lx, 1))
        {
            lx->k = lx->k + 1;
            _nomen_consumere(lx);
            redde CSS_LEX_APUD_VERBUM;
        }
        lx->k = lx->k + 1;
        redde CSS_LEX_DELIM;
    }
    si (c == '\\')
    {
        si (_effugium_validum(lx, 0))
        {
            salta identificator_via;
        }
        lx->k = lx->k + 1;
        redde CSS_LEX_DELIM;
    }
    si (_est_digitus(c))
    {
        salta numerus_via;
    }
    si (_est_nominis_initium(c))
    {
        salta identificator_via;
    }
    /* reliquum - delim octeti unius (NUL inclusus) */
    lx->k = lx->k + 1;
    redde CSS_LEX_DELIM;

numerus_via:
    _numerum_consumere(lx);
    si (_incipit_identificator(lx, 0))
    {
        _nomen_consumere(lx);
        redde CSS_LEX_DIMENSIO;
    }
    si (_ad(lx, 0) == '%')
    {
        lx->k = lx->k + 1;
        redde CSS_LEX_CENTESIMA;
    }
    redde CSS_LEX_NUMERUS;

identificator_via:
    {
        i32 initium = lx->k;

        _nomen_consumere(lx);
        /* url( ... ) - nisi citatum sequitur (tunc functio) */
        si (   lx->k - initium == 3
            && (lx->fons[initium] == 'u' || lx->fons[initium] == 'U')
            && (lx->fons[initium + 1] == 'r'
                || lx->fons[initium + 1] == 'R')
            && (lx->fons[initium + 2] == 'l'
                || lx->fons[initium + 2] == 'L')
            && _ad(lx, 0)      == '(')
        {
            i32 prospectus = 1;

            dum (_est_spatium(_ad(lx, prospectus)))
            {
                prospectus = prospectus + 1;
            }
            si (   _ad(lx, prospectus) == '"'
                || _ad(lx, prospectus) == '\'')
            {
                lx->k = lx->k + 1;
                redde CSS_LEX_FUNCTIO;
            }
            lx->k = lx->k + 1;
            redde _url_consumere(lx);
        }
        si (_ad(lx, 0) == '(')
        {
            lx->k = lx->k + 1;
            redde CSS_LEX_FUNCTIO;
        }
        redde CSS_LEX_IDENTIFICATOR;
    }
}


/* ==================================================
 * facies publica
 * ================================================== */

Xar*
css_lexare (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura)
{
    CssLexator  lx;
           Xar* lexemata;
           i32  linea    = 1;
           i32  columna  = 1;

    lexemata = xar_creare(piscina, (i32)magnitudo(CssLexema));
    si (lexemata == NIHIL)
    {
        redde NIHIL;
    }
    lx.fons     = fons;
    lx.mensura  = mensura;
    lx.k        = 0;

    dum (lx.k < lx.mensura)
    {
        CssLexema* l;
              i32  initium = lx.k;
              i32  j;

        CssLexemaGenus genus = _lexema_consumere(&lx);

        l = (CssLexema*)xar_addere(lexemata);
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
        CssLexema* l = (CssLexema*)xar_addere(lexemata);

        si (l == NIHIL)
        {
            redde NIHIL;
        }
        l->genus    = CSS_LEX_FINIS;
        l->valor    = _chorda_ex_fonte(fons + mensura, 0);
        l->offset   = mensura;
        l->linea    = linea;
        l->columna  = columna;
    }
    redde lexemata;
}

chorda
css_lexemata_emittere (
    Piscina* piscina,
        Xar* lexemata)
{
    ChordaAedificator* aed;
                  i32  k;

    aed = chorda_aedificator_creare(piscina, (memoriae_index)256);
    per (k = 0; k < xar_numerus(lexemata); k = k + 1)
    {
        CssLexema* l = (CssLexema*)xar_obtinere(lexemata, k);

        chorda_aedificator_appendere_chorda(aed, l->valor);
    }
    redde chorda_aedificator_finire(aed);
}

constans character*
css_lexema_genus_nomen (
    CssLexemaGenus genus)
{
    commutatio (genus)
    {
        casus CSS_LEX_FINIS:            redde "finis";
        casus CSS_LEX_IDENTIFICATOR:    redde "identificator";
        casus CSS_LEX_FUNCTIO:          redde "functio";
        casus CSS_LEX_APUD_VERBUM:      redde "apud-verbum";
        casus CSS_LEX_CANCELLUM:        redde "cancellum";
        casus CSS_LEX_CHORDA:           redde "chorda";
        casus CSS_LEX_CHORDA_IMPERFECTA: redde "chorda-imperfecta";
        casus CSS_LEX_URL:              redde "url";
        casus CSS_LEX_URL_IMPERFECTA:   redde "url-imperfecta";
        casus CSS_LEX_NUMERUS:          redde "numerus";
        casus CSS_LEX_CENTESIMA:        redde "centesima";
        casus CSS_LEX_DIMENSIO:         redde "dimensio";
        casus CSS_LEX_SPATIA:           redde "spatia";
        casus CSS_LEX_COMMENTARIUM:     redde "commentarium";
        casus CSS_LEX_COMMENTARIUM_IMPERFECTUM:
            redde "commentarium-imperfectum";
        casus CSS_LEX_CDO:              redde "cdo";
        casus CSS_LEX_CDC:              redde "cdc";
        casus CSS_LEX_COLON:            redde "colon";
        casus CSS_LEX_SEMICOLON:        redde "semicolon";
        casus CSS_LEX_COMMA:            redde "comma";
        casus CSS_LEX_QUADRA_APERTA:    redde "quadra-aperta";
        casus CSS_LEX_QUADRA_CLAUSA:    redde "quadra-clausa";
        casus CSS_LEX_PAREN_APERTA:     redde "paren-aperta";
        casus CSS_LEX_PAREN_CLAUSA:     redde "paren-clausa";
        casus CSS_LEX_BRACE_APERTA:     redde "brace-aperta";
        casus CSS_LEX_BRACE_CLAUSA:     redde "brace-clausa";
        casus CSS_LEX_DELIM:            redde "delim";
        ordinarius:                     redde "ignotum";
    }
}
