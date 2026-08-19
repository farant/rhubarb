/* html_lexema.c - vide caput. Machina statuum HTML5 ad dimidium
 * tractabile redacta: modus datorum / intra-tag / crudus trans
 * lexemata servatur. Cursor semper progreditur (quisque trames
 * octetum unum minimum consumit), ergo lexatio totalis terminatur
 * et lexemata fontem perfecte tegulant - unde fidelitas
 * concatenationis. */
#include "html_lexema.h"
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
_est_littera (
    character c);

interior b32
_est_littera (
    character c)
{
    redde (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
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

interior character
_minuscula (
    character c);

interior character
_minuscula (
    character c)
{
    si (c >= 'A' && c <= 'Z')
    {
        redde (character)(c + ('a' - 'A'));
    }
    redde c;
}

/* chorda ex fonte (visus, nulla copia): chorda.datum i8* est sed
 * fons constans - unio castum celat; nemo per visum scribit
 * (contractus; idem mos silvae et css_lexema) */
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


/* ==================================================
 * status lexatoris
 * ================================================== */

nomen enumeratio {
    MODUS_DATA,        /* textus, tags, commentaria */
    MODUS_INTRA_TAG,   /* attributa, '>' aut '/>' */
    MODUS_CRUDUS       /* script/style/title/textarea contentum */
} HtmlModus;

nomen structura {
    constans character* fons;
                   i32  mensura;
                   i32  k;              /* cursor */
             HtmlModus  modus;
    constans character* crudus_datum;   /* titulus elementi crudi */
                   i32  crudus_mensura;
    constans character* tag_datum;      /* titulus tagi praesentis */
                   i32  tag_mensura;
                   b32  tag_clausura;   /* '</...' praesens? */
                   b32  post_aequale;   /* proximum = valor */
} HtmlLexator;

interior character
_ad (
    HtmlLexator* lx,
            i32  delta);

interior character
_ad (
    HtmlLexator* lx,
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
    HtmlLexator* lx,
            i32  delta);

interior b32
_intra (
    HtmlLexator* lx,
            i32  delta)
{
    redde lx->k + delta < lx->mensura;
}

/* titulus == literis, casu neglecto (literis minusculis dandum) */
interior b32
_titulus_aequat (
    constans character* datum,
                   i32  mensura,
    constans character* literis);

interior b32
_titulus_aequat (
    constans character* datum,
                   i32  mensura,
    constans character* literis)
{
    i32 k;

    per (k = 0; k < mensura; k = k + 1)
    {
        si (   literis[k]           == '\0'
            || _minuscula(datum[k]) != literis[k])
        {
            redde FALSUM;
        }
    }
    redde literis[mensura] == '\0';
}

/* elementa cruda: RAWTEXT (script, style) + RCDATA (title,
 * textarea) - hic idem lexata, distinctio decoctionis = arbor */
interior b32
_est_crudum (
    constans character* datum,
                   i32  mensura);

interior b32
_est_crudum (
    constans character* datum,
                   i32  mensura)
{
    redde _titulus_aequat(datum, mensura, "script")
        || _titulus_aequat(datum, mensura, "style")
        || _titulus_aequat(datum, mensura, "title")
        || _titulus_aequat(datum, mensura, "textarea");
}


/* ==================================================
 * prospectus (nihil consumunt)
 * ================================================== */

/* mensura referentiae strictae '&...;' ad cursorem, 0 si nulla.
 * Tectum LXIV octetis: prospectus linearis manet (tituli nominati
 * veri <= XXXIII, numerici tam longi digni non sunt). */
interior i32
_referentia_mensura (
    HtmlLexator* lx);

interior i32
_referentia_mensura (
    HtmlLexator* lx)
{
    i32 n;

    si (_ad(lx, 1) == '#')
    {
        si (_ad(lx, 2) == 'x' || _ad(lx, 2) == 'X')
        {
            n = 3;
            si (!_est_hex(_ad(lx, n)))
            {
                redde 0;
            }
            dum (n < 64 && _est_hex(_ad(lx, n)))
            {
                n = n + 1;
            }
        }
        alioquin
        {
            n = 2;
            si (!_est_digitus(_ad(lx, n)))
            {
                redde 0;
            }
            dum (n < 64 && _est_digitus(_ad(lx, n)))
            {
                n = n + 1;
            }
        }
    }
    alioquin
    {
        si (!_est_littera(_ad(lx, 1)))
        {
            redde 0;
        }
        n = 2;
        dum (   n < 64 && (_est_littera(_ad(lx, n))
            || _est_digitus(_ad(lx, n))))
        {
            n = n + 1;
        }
    }
    si (_ad(lx, n) != ';')
    {
        redde 0;
    }
    redde n + 1;
}

/* inciperetne notatio ad '<'? (spec: littera, '!', '?', aut '/'
 * cum octeto sequente; '<' aliter textus manet) */
interior b32
_incipit_notatio (
    HtmlLexator* lx);

interior b32
_incipit_notatio (
    HtmlLexator* lx)
{
    character c = _ad(lx, 1);

    si (_est_littera(c) || c == '!' || c == '?')
    {
        redde VERUM;
    }
    si (c == '/')
    {
        redde _intra(lx, 2);   /* '</' + EOF -> textus (spec) */
    }
    redde FALSUM;
}

/* stat clausura elementi crudi ad cursorem? '</' + titulus (casu
 * neglecto) + (spatium | '/' | '>') - aliter textus crudus manet
 * ('</scripty' scriptum non claudit; spec) */
interior b32
_clausura_cruda_hic (
    HtmlLexator* lx);

interior b32
_clausura_cruda_hic (
    HtmlLexator* lx)
{
          i32 j;
    character c;

    si (_ad(lx, 0) != '<' || _ad(lx, 1) != '/')
    {
        redde FALSUM;
    }
    per (j = 0; j < lx->crudus_mensura; j = j + 1)
    {
        si (_minuscula(_ad(lx, j + 2))
            != _minuscula(lx->crudus_datum[j]))
        {
            redde FALSUM;
        }
    }
    c = _ad(lx, lx->crudus_mensura + 2);
    redde _est_spatium(c) || c == '/' || c == '>';
}


/* ==================================================
 * consumptores
 * ================================================== */

/* titulum tagi consumere (cursor post '<' aut '</'); statum
 * intra-tag instituit */
interior vacuum
_tag_titulum_consumere (
    HtmlLexator* lx);

interior vacuum
_tag_titulum_consumere (
    HtmlLexator* lx)
{
    i32 initium = lx->k;

    dum (   _intra(lx, 0) && !_est_spatium(_ad(lx, 0))
         && _ad(lx, 0) != '/' && _ad(lx, 0) != '>')
    {
        lx->k = lx->k + 1;
    }
    lx->tag_datum     = lx->fons + initium;
    lx->tag_mensura   = lx->k - initium;
    lx->modus         = MODUS_INTRA_TAG;
    lx->post_aequale  = FALSUM;
}

/* post '>' aut '/>': modum sequentem eligere ('<script/>' modum
 * crudum intrat - mos navigatorum, signum se-claudens pro
 * elementis crudis ignoratur) */
interior vacuum
_tag_finitum (
    HtmlLexator* lx);

interior vacuum
_tag_finitum (
    HtmlLexator* lx)
{
    lx->post_aequale = FALSUM;
    si (   !lx->tag_clausura
        && _est_crudum(lx->tag_datum, lx->tag_mensura))
    {
        lx->modus           = MODUS_CRUDUS;
        lx->crudus_datum    = lx->tag_datum;
        lx->crudus_mensura  = lx->tag_mensura;
        redde;
    }
    lx->modus = MODUS_DATA;
}

/* commentarium (cursor in '<!--'): clauditur per '-->' aut
 * '--!>'; abrupta '<!-->' et '<!--->' clausa (spec, cum errore) */
interior HtmlLexemaGenus
_commentarium_consumere (
    HtmlLexator* lx);

interior HtmlLexemaGenus
_commentarium_consumere (
    HtmlLexator* lx)
{
    lx->k = lx->k + 4;   /* '<!--' */
    si (_ad(lx, 0) == '>')
    {
        lx->k = lx->k + 1;
        redde HTML_LEX_COMMENTARIUM;
    }
    si (_ad(lx, 0) == '-' && _ad(lx, 1) == '>')
    {
        lx->k = lx->k + 2;
        redde HTML_LEX_COMMENTARIUM;
    }
    dum (_intra(lx, 0))
    {
        si (_ad(lx, 0) == '-' && _ad(lx, 1) == '-')
        {
            si (_ad(lx, 2) == '>')
            {
                lx->k = lx->k + 3;
                redde HTML_LEX_COMMENTARIUM;
            }
            si (_ad(lx, 2) == '!' && _ad(lx, 3) == '>')
            {
                lx->k = lx->k + 4;
                redde HTML_LEX_COMMENTARIUM;
            }
        }
        lx->k = lx->k + 1;
    }
    redde HTML_LEX_COMMENTARIUM_IMPERFECTUM;
}

/* doctype (cursor in '<!doctype'): lexema unum ad '>' primum -
 * quod spec etiam intra identificatores citatos facit (abrupte) */
interior HtmlLexemaGenus
_doctype_consumere (
    HtmlLexator* lx);

interior HtmlLexemaGenus
_doctype_consumere (
    HtmlLexator* lx)
{
    lx->k = lx->k + 9;   /* '<!doctype' */
    dum (_intra(lx, 0))
    {
        si (_ad(lx, 0) == '>')
        {
            lx->k = lx->k + 1;
            redde HTML_LEX_DOCTYPE;
        }
        lx->k = lx->k + 1;
    }
    redde HTML_LEX_DOCTYPE_IMPERFECTUM;
}

/* sectio CDATA (cursor in '<![CDATA['): ad ']]>' */
interior HtmlLexemaGenus
_cdata_consumere (
    HtmlLexator* lx);

interior HtmlLexemaGenus
_cdata_consumere (
    HtmlLexator* lx)
{
    lx->k = lx->k + 9;   /* '<![CDATA[' */
    dum (_intra(lx, 0))
    {
        si (   _ad(lx, 0) == ']' && _ad(lx, 1) == ']'
            && _ad(lx, 2) == '>')
        {
            lx->k = lx->k + 3;
            redde HTML_LEX_CDATA;
        }
        lx->k = lx->k + 1;
    }
    redde HTML_LEX_CDATA_IMPERFECTA;
}

/* commentarium pravum (cursor post praefixum '<!', '<?', '</'):
 * ad '>' primum inclusum, aut EOF */
interior HtmlLexemaGenus
_pravum_consumere (
    HtmlLexator* lx);

interior HtmlLexemaGenus
_pravum_consumere (
    HtmlLexator* lx)
{
    dum (_intra(lx, 0))
    {
        si (_ad(lx, 0) == '>')
        {
            lx->k = lx->k + 1;
            frange;
        }
        lx->k = lx->k + 1;
    }
    redde HTML_LEX_COMMENTARIUM_PRAVUM;
}

/* notatio (cursor in '<', _incipit_notatio iam vera) */
interior HtmlLexemaGenus
_notationem_consumere (
    HtmlLexator* lx);

interior HtmlLexemaGenus
_notationem_consumere (
    HtmlLexator* lx)
{
    character c = _ad(lx, 1);

    si (_est_littera(c))
    {
        lx->k             = lx->k + 1;
        lx->tag_clausura  = FALSUM;
        _tag_titulum_consumere(lx);
        redde HTML_LEX_TAG_APERTURA;
    }
    si (c == '/')
    {
        si (_est_littera(_ad(lx, 2)))
        {
            lx->k             = lx->k + 2;
            lx->tag_clausura  = VERUM;
            _tag_titulum_consumere(lx);
            redde HTML_LEX_TAG_CLAUSURA;
        }
        lx->k = lx->k + 2;   /* '</>' , '</3>' ... - pravum */
        redde _pravum_consumere(lx);
    }
    si (c == '?')
    {
        lx->k = lx->k + 2;
        redde _pravum_consumere(lx);
    }
    /* c == '!' */
    si (_ad(lx, 2) == '-' && _ad(lx, 3) == '-')
    {
        redde _commentarium_consumere(lx);
    }
    si (   _minuscula(_ad(lx, 2)) == 'd'
        && _minuscula(_ad(lx, 3)) == 'o'
        && _minuscula(_ad(lx, 4)) == 'c'
        && _minuscula(_ad(lx, 5)) == 't'
        && _minuscula(_ad(lx, 6)) == 'y'
        && _minuscula(_ad(lx, 7)) == 'p'
        && _minuscula(_ad(lx, 8)) == 'e')
    {
        redde _doctype_consumere(lx);
    }
    si (   _ad(lx, 2) == '[' && _ad(lx, 3) == 'C'
        && _ad(lx, 4) == 'D' && _ad(lx, 5) == 'A'
        && _ad(lx, 6) == 'T' && _ad(lx, 7) == 'A'
        && _ad(lx, 8) == '[')
    {
        redde _cdata_consumere(lx);
    }
    lx->k = lx->k + 2;
    redde _pravum_consumere(lx);
}

/* intra tag: spatia / '>' / valor (post '=') / '/>' / '=' / '/'
 * solivagus / titulus attributi. ORDO gravis: valor ante '/>'
 * probatur, ut '<a href=/>' valorem '/' det et tag normaliter
 * finiat (mos spec et navigatorum) */
interior HtmlLexemaGenus
_intra_tag_consumere (
    HtmlLexator* lx);

interior HtmlLexemaGenus
_intra_tag_consumere (
    HtmlLexator* lx)
{
    character c = _ad(lx, 0);

    si (_est_spatium(c))
    {
        dum (_intra(lx, 0) && _est_spatium(_ad(lx, 0)))
        {
            lx->k = lx->k + 1;
        }
        redde HTML_LEX_SPATIA;   /* post_aequale manet */
    }
    si (c == '>')
    {
        lx->k = lx->k + 1;
        _tag_finitum(lx);
        redde HTML_LEX_TAG_FINIS;
    }
    si (lx->post_aequale)
    {
        lx->post_aequale = FALSUM;
        si (c == '"' || c == '\'')
        {
            lx->k = lx->k + 1;
            dum (_intra(lx, 0))
            {
                si (_ad(lx, 0) == c)
                {
                    lx->k = lx->k + 1;
                    redde HTML_LEX_ATTRIBUTUM_VALOR;
                }
                lx->k = lx->k + 1;   /* lineae novae licitae */
            }
            redde HTML_LEX_ATTRIBUTUM_VALOR_IMPERFECTUS;
        }
        /* valor nudus: ad spatium aut '>' ('/' valoris pars est -
         * '<a href=x/>' valorem 'x/' dat; spec) */
        dum (   _intra(lx, 0) && !_est_spatium(_ad(lx, 0))
             && _ad(lx, 0) != '>')
        {
            lx->k = lx->k + 1;
        }
        redde HTML_LEX_ATTRIBUTUM_VALOR;
    }
    si (c == '/' && _ad(lx, 1) == '>')
    {
        lx->k = lx->k + 2;
        _tag_finitum(lx);
        redde HTML_LEX_TAG_FINIS_SOLUS;
    }
    si (c == '=')
    {
        lx->k             = lx->k + 1;
        lx->post_aequale  = VERUM;
        redde HTML_LEX_AEQUALE;
    }
    si (c == '/')
    {
        lx->k = lx->k + 1;
        redde HTML_LEX_DELIM;   /* '/' solivagus (spec: neglectus) */
    }
    /* titulus attributi: ad spatium / '/' / '>' / '=' */
    dum (   _intra(lx, 0) && !_est_spatium(_ad(lx, 0))
         && _ad(lx, 0) != '/' && _ad(lx, 0) != '>'
         && _ad(lx, 0) != '=')
    {
        lx->k = lx->k + 1;
    }
    redde HTML_LEX_ATTRIBUTUM_NOMEN;
}


/* ==================================================
 * lexema unum consumere
 * ================================================== */

interior HtmlLexemaGenus
_lexema_consumere (
    HtmlLexator* lx);

interior HtmlLexemaGenus
_lexema_consumere (
    HtmlLexator* lx)
{
    character c;

    si (lx->modus == MODUS_CRUDUS)
    {
        si (!_clausura_cruda_hic(lx))
        {
            dum (_intra(lx, 0))
            {
                si (_ad(lx, 0) == '<' && _clausura_cruda_hic(lx))
                {
                    redde HTML_LEX_TEXTUS_CRUDUS;
                }
                lx->k = lx->k + 1;
            }
            redde HTML_LEX_TEXTUS_CRUDUS_IMPERFECTUS;
        }
        lx->modus = MODUS_DATA;
        /* clausura infra via datorum lexatur */
    }
    si (lx->modus == MODUS_INTRA_TAG)
    {
        redde _intra_tag_consumere(lx);
    }
    /* modus datorum */
    c = _ad(lx, 0);
    si (c == '&')
    {
        i32 n = _referentia_mensura(lx);

        si (n > 0)
        {
            lx->k = lx->k + n;
            redde HTML_LEX_REFERENTIA;
        }
    }
    si (c == '<' && _incipit_notatio(lx))
    {
        redde _notationem_consumere(lx);
    }
    /* cursus textus: ad notationem aut referentiam proximam
     * ('<' et '&' nudi textus manent - totalitas) */
    dum (_intra(lx, 0))
    {
        c = _ad(lx, 0);
        si (c == '<' && _incipit_notatio(lx))
        {
            frange;
        }
        si (c == '&' && _referentia_mensura(lx) > 0)
        {
            frange;
        }
        lx->k = lx->k + 1;
    }
    redde HTML_LEX_TEXTUS;
}


/* ==================================================
 * facies publica
 * ================================================== */

Xar*
html_lexare (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura)
{
    HtmlLexator  lx;
            Xar* lexemata;
            i32  linea    = 1;
            i32  columna  = 1;

    lexemata = xar_creare(piscina, (i32)magnitudo(HtmlLexema));
    si (lexemata == NIHIL)
    {
        redde NIHIL;
    }
    lx.fons            = fons;
    lx.mensura         = mensura;
    lx.k               = 0;
    lx.modus           = MODUS_DATA;
    lx.crudus_datum    = NIHIL;
    lx.crudus_mensura  = 0;
    lx.tag_datum       = NIHIL;
    lx.tag_mensura     = 0;
    lx.tag_clausura    = FALSUM;
    lx.post_aequale    = FALSUM;

    dum (lx.k < lx.mensura)
    {
        HtmlLexema* l;
               i32  initium = lx.k;
               i32  j;

        HtmlLexemaGenus genus = _lexema_consumere(&lx);

        l = (HtmlLexema*)xar_addere(lexemata);
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
        HtmlLexema* l = (HtmlLexema*)xar_addere(lexemata);

        si (l == NIHIL)
        {
            redde NIHIL;
        }
        l->genus    = HTML_LEX_FINIS;
        l->valor    = _chorda_ex_fonte(fons + mensura, 0);
        l->offset   = mensura;
        l->linea    = linea;
        l->columna  = columna;
    }
    redde lexemata;
}

chorda
html_lexemata_emittere (
    Piscina* piscina,
        Xar* lexemata)
{
    ChordaAedificator* aed;
                  i32  k;

    aed = chorda_aedificator_creare(piscina, (memoriae_index)256);
    per (k = 0; k < xar_numerus(lexemata); k = k + 1)
    {
        HtmlLexema* l = (HtmlLexema*)xar_obtinere(lexemata, k);

        chorda_aedificator_appendere_chorda(aed, l->valor);
    }
    redde chorda_aedificator_finire(aed);
}

constans character*
html_lexema_genus_nomen (
    HtmlLexemaGenus genus)
{
    commutatio (genus)
    {
        casus HTML_LEX_FINIS:           redde "finis";
        casus HTML_LEX_TEXTUS:          redde "textus";
        casus HTML_LEX_REFERENTIA:      redde "referentia";
        casus HTML_LEX_TEXTUS_CRUDUS:   redde "textus-crudus";
        casus HTML_LEX_TEXTUS_CRUDUS_IMPERFECTUS:
            redde "textus-crudus-imperfectus";
        casus HTML_LEX_TAG_APERTURA:    redde "tag-apertura";
        casus HTML_LEX_TAG_CLAUSURA:    redde "tag-clausura";
        casus HTML_LEX_TAG_FINIS:       redde "tag-finis";
        casus HTML_LEX_TAG_FINIS_SOLUS: redde "tag-finis-solus";
        casus HTML_LEX_ATTRIBUTUM_NOMEN:
            redde "attributum-nomen";
        casus HTML_LEX_AEQUALE:         redde "aequale";
        casus HTML_LEX_ATTRIBUTUM_VALOR:
            redde "attributum-valor";
        casus HTML_LEX_ATTRIBUTUM_VALOR_IMPERFECTUS:
            redde "attributum-valor-imperfectus";
        casus HTML_LEX_COMMENTARIUM:    redde "commentarium";
        casus HTML_LEX_COMMENTARIUM_IMPERFECTUM:
            redde "commentarium-imperfectum";
        casus HTML_LEX_COMMENTARIUM_PRAVUM:
            redde "commentarium-pravum";
        casus HTML_LEX_DOCTYPE:         redde "doctype";
        casus HTML_LEX_DOCTYPE_IMPERFECTUM:
            redde "doctype-imperfectum";
        casus HTML_LEX_CDATA:           redde "cdata";
        casus HTML_LEX_CDATA_IMPERFECTA:
            redde "cdata-imperfecta";
        casus HTML_LEX_SPATIA:          redde "spatia";
        casus HTML_LEX_DELIM:           redde "delim";
        ordinarius:                     redde "ignotum";
    }
}
