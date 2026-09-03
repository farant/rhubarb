/* md_lexema.c - Vide md_lexema.h. */

#include "md_lexema.h"
#include "md_lexicon.h"
#include <string.h>


/* ==================================================
 * Adiumenta
 * ================================================== */

interior b32
_spatium (
    character c)
{
    redde (b32)(c == ' ' || c == '\t');
}

interior b32
_digitus (
    character c)
{
    redde (b32)(c >= '0' && c <= '9');
}

/* Spatia/tabulae ab 'ab'; reddit post. */
interior s32
_spatia_post (
    constans character* fons,
                   s32  ab,
                   s32  ad)
{
    dum (ab < ad && _spatium(fons[ab]))
    {
        ab = ab + I;
    }
    redde ab;
}

/* Spatia/tabulae ante 'ad' (retro); reddit initium cursus. */
interior s32
_spatia_ante (
    constans character* fons,
                   s32  ab,
                   s32  ad)
{
    dum (ad > ab && _spatium(fons[ad - I]))
    {
        ad = ad - I;
    }
    redde ad;
}

/* Columnae unius cursus spatiorum/tabularum [ab, post) a columna. */
interior i32
_columnae (
    constans character* fons,
                   s32  ab,
                   s32  post,
                   i32  columna)
{
    i32 c = columna;

    dum (ab < post)
    {
        si (fons[ab] == '\t')
        {
            c = c + (IV - (c % IV));
        }
        alioquin
        {
            c = c + I;
        }
        ab = ab + I;
    }
    redde c - columna;
}


/* ==================================================
 * Tabula linearum
 * ================================================== */

b32
md_lineae_scindere (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura,
              MdLineae* exitus)
{
    s32 i;
    s32 initium;
    i32 numerus;

    exitus->fons     = fons;
    exitus->mensura  = mensura;
    exitus->lineae   = xar_creare(piscina, (i32)magnitudo(MdLinea));
    si (exitus->lineae == NIHIL)
    {
        redde FALSUM;
    }

    i        = ZEPHYRUM;
    initium  = ZEPHYRUM;
    numerus  = ZEPHYRUM;
    dum (i < (s32)mensura)
    {
        character c = fons[i];

        si (c == '\n' || c == '\r')
        {
            MdLinea* l;
                i32  term = I;

            si (   c           == '\r' && i + I < (s32)mensura
                && fons[i + I] == '\n')
            {
                term = II;
            }
            l = (MdLinea*)xar_addere(exitus->lineae);
            si (l == NIHIL)
            {
                redde FALSUM;
            }
            numerus        = numerus + I;
            l->offset      = initium;
            l->mensura     = (i32)(i - initium);
            l->terminator  = term;
            l->numerus     = numerus;
            i              = i + (s32)term;
            initium        = i;
            perge;
        }
        i = i + I;
    }
    si (initium < (s32)mensura)
    {
        MdLinea* l = (MdLinea*)xar_addere(exitus->lineae);

        si (l == NIHIL)
        {
            redde FALSUM;
        }
        numerus        = numerus + I;
        l->offset      = initium;
        l->mensura     = (i32)((s32)mensura - initium);
        l->terminator  = ZEPHYRUM;
        l->numerus     = numerus;
    }
    redde VERUM;
}

i32
md_lineae_numerus (
    constans MdLineae* lineae)
{
    redde xar_numerus(lineae->lineae);
}

constans MdLinea*
md_linea (
    constans MdLineae* lineae,
                  i32  index)
{
    redde (constans MdLinea*)xar_obtinere(lineae->lineae, index);
}

/* chorda.datum i8* est sed fons constans: unio, ut css_lexema.c. */
chorda
md_chorda_fontis (
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


/* ==================================================
 * Scanners
 * ================================================== */

s32
md_scan_indentatio (
    constans character* fons,
                   s32  ab,
                   s32  ad,
                   i32  columna,
                   i32* columnae)
{
    s32 post = _spatia_post(fons, ab, ad);

    *columnae = _columnae(fons, ab, post, columna);
    redde post;
}

s32
md_scan_citatio (
    constans character* fons,
                   s32  ab,
                   s32  ad)
{
    s32 post;

    si (ab >= ad || fons[ab] != '>')
    {
        redde ab;
    }
    post = ab + I;
    si (post < ad && _spatium(fons[post]))
    {
        post = post + I;
    }
    redde post;
}

b32
md_scan_marca_listae (
    constans character* fons,
                   s32  ab,
                   s32  ad,
                   i32  columna,
         MdMarcaListae* m)
{
    s32 i = ab;

    si (ab >= ad)
    {
        redde FALSUM;
    }
    memset(m, ZEPHYRUM, magnitudo(*m));

    si (fons[i] == '-' || fons[i] == '+' || fons[i] == '*')
    {
        m->numerata     = FALSUM;
        m->delimitator  = fons[i];
        i               = i + I;
    }
    alioquin si (_digitus(fons[i]))
    {
        i32 digiti  = ZEPHYRUM;
        i32 valor   = ZEPHYRUM;

        dum (i < ad && _digitus(fons[i]) && digiti < IX)
        {
            valor   = valor * X + (i32)(fons[i] - '0');
            digiti  = digiti + I;
            i       = i + I;
        }
        si (i >= ad || (fons[i] != '.' && fons[i] != ')'))
        {
            redde FALSUM;   /* X+ digiti, aut delimitator absens */
        }
        m->numerata     = VERUM;
        m->initium      = valor;
        m->delimitator  = fons[i];
        i               = i + I;
    }
    alioquin
    {
        redde FALSUM;
    }

    /* post marcam: spatium/tabula aut finis lineae */
    si (i < ad && !_spatium(fons[i]))
    {
        redde FALSUM;
    }
    m->marca_ad  = i;
    m->latitudo  = (i32)(i - ab);
    m->ad        = _spatia_post(fons, i, ad);
    m->spatia    = _columnae(fons, i, m->ad, columna + m->latitudo);
    redde VERUM;
}

b32
md_scan_officium (
    constans character* fons,
                   s32  ab,
                   s32  ad,
        MdMarcaOfficii* o)
{
    si (ab + III > ad)
    {
        redde FALSUM;
    }
    si (fons[ab] != '[' || fons[ab + II] != ']')
    {
        redde FALSUM;
    }
    si (   fons[ab + I] != ' ' && fons[ab + I] != 'x'
        && fons[ab + I] != 'X')
    {
        redde FALSUM;
    }
    si (ab + III < ad && !_spatium(fons[ab + III]))
    {
        redde FALSUM;
    }
    o->ad         = ab + III;
    o->perfectum  = (b32)(fons[ab + I] != ' ');
    redde VERUM;
}

b32
md_scan_atx (
    constans character* fons,
                   s32  ab,
                   s32  ad,
                 MdAtx* r)
{
    s32 i = ab;
    i32 n = ZEPHYRUM;
    s32 e;
    s32 h;

    dum (i < ad && fons[i] == '#' && n < VII)
    {
        n = n + I;
        i = i + I;
    }
    si (n < I || n > VI)
    {
        redde FALSUM;
    }
    si (i < ad && !_spatium(fons[i]))
    {
        redde FALSUM;
    }
    r->gradus    = n;
    r->marca_ad  = _spatia_post(fons, i, ad);

    /* clausura: retro spatia, deinde '#'+, praecedente spatio aut
     * contento vacuo */
    e = _spatia_ante(fons, r->marca_ad, ad);
    h = e;
    dum (h > r->marca_ad && fons[h - I] == '#')
    {
        h = h - I;
    }
    si (h < e && (h == r->marca_ad || _spatium(fons[h - I])))
    {
        r->clausura_ab   = _spatia_ante(fons, r->marca_ad, h);
        r->contentum_ad  = r->clausura_ab;
    }
    alioquin
    {
        r->clausura_ab   = ad;
        r->contentum_ad  = ad;
    }
    redde VERUM;
}

b32
md_scan_subductio (
    constans character* fons,
                   s32  ab,
                   s32  ad,
                   i32* gradus)
{
    character c;
          s32 i = ab;

    si (ab >= ad)
    {
        redde FALSUM;
    }
    c = fons[ab];
    si (c != '=' && c != '-')
    {
        redde FALSUM;
    }
    dum (i < ad && fons[i] == c)
    {
        i = i + I;
    }
    si (_spatia_post(fons, i, ad) != ad)
    {
        redde FALSUM;
    }
    *gradus = (c == '=') ? I : II;
    redde VERUM;
}

b32
md_scan_divisio (
    constans character* fons,
                   s32  ab,
                   s32  ad)
{
    character c;
          i32 n = ZEPHYRUM;
          s32 i;

    si (ab >= ad)
    {
        redde FALSUM;
    }
    c = fons[ab];
    si (c != '-' && c != '*' && c != '_')
    {
        redde FALSUM;
    }
    per (i = ab; i < ad; i++)
    {
        si (fons[i] == c)
        {
            n = n + I;
        }
        alioquin si (!_spatium(fons[i]))
        {
            redde FALSUM;
        }
    }
    redde (b32)(n >= III);
}

b32
md_scan_saeptum_apertum (
    constans character* fons,
                   s32  ab,
                   s32  ad,
             MdSaeptum* r)
{
    character c;
          s32 i = ab;
          s32 j;

    si (ab >= ad)
    {
        redde FALSUM;
    }
    c = fons[ab];
    si (c != '`' && c != '~')
    {
        redde FALSUM;
    }
    dum (i < ad && fons[i] == c)
    {
        i = i + I;
    }
    si ((i32)(i - ab) < III)
    {
        redde FALSUM;
    }
    si (c == '`')
    {
        per (j = i; j < ad; j++)
        {
            si (fons[j] == '`')
            {
                redde FALSUM;
            }
        }
    }
    r->signum      = c;
    r->longitudo   = (i32)(i - ab);
    r->signum_ad   = i;
    r->info_ab     = i;
    r->info_ad     = ad;
    r->info_adest  = (b32)(_spatia_post(fons, i, ad) != ad);
    redde VERUM;
}

b32
md_scan_saeptum_clausum (
    constans character* fons,
                   s32  ab,
                   s32  ad,
             character  signum,
                   i32  longitudo_min,
                   s32* signum_ad)
{
    s32 i = ab;

    dum (i < ad && fons[i] == signum)
    {
        i = i + I;
    }
    si ((i32)(i - ab) < longitudo_min)
    {
        redde FALSUM;
    }
    si (_spatia_post(fons, i, ad) != ad)
    {
        redde FALSUM;
    }
    *signum_ad = i;
    redde VERUM;
}

i32
md_scan_tabula_separator (
    constans character* fons,
                   s32  ab,
                   s32  ad,
                   i32* ordinationes,
                   i32  capacitas)
{
    s32 i = ab;
    i32 n = ZEPHYRUM;

    i = _spatia_post(fons, i, ad);
    si (i < ad && fons[i] == '|')
    {
        i = i + I;
    }
    dum (VERUM)
    {
        b32 sinistra  = FALSUM;
        b32 dextra    = FALSUM;
        i32 lineolae  = ZEPHYRUM;

        i = _spatia_post(fons, i, ad);
        si (i >= ad)
        {
            frange;   /* post pipam finalem */
        }
        si (fons[i] == ':')
        {
            sinistra  = VERUM;
            i         = i + I;
        }
        dum (i < ad && fons[i] == '-')
        {
            lineolae  = lineolae + I;
            i         = i + I;
        }
        si (lineolae == ZEPHYRUM)
        {
            redde ZEPHYRUM;
        }
        si (i < ad && fons[i] == ':')
        {
            dextra  = VERUM;
            i       = i + I;
        }
        i = _spatia_post(fons, i, ad);
        si (i < ad && fons[i] != '|')
        {
            redde ZEPHYRUM;
        }
        si (n < capacitas)
        {
            ordinationes[n] = (sinistra && dextra) ? (i32)II
                            : sinistra ? (i32)I
                            : dextra ? (i32)III
                            : (i32)ZEPHYRUM;
        }
        n = n + I;
        si (i >= ad)
        {
            frange;
        }
        i = i + I;   /* '|' */
    }
    redde n;
}


/* ==================================================
 * Fabrica
 * ================================================== */

vacuum
md_fabrica_incipere (
            MdFabrica* fabrica,
              Piscina* piscina,
    constans MdLineae* lineae)
{
    fabrica->piscina               = piscina;
    fabrica->lineae                = lineae;
    fabrica->forma.mensura_caudae  = ZEPHYRUM;
}

MateriaToken*
md_lexema_fabricare (
    MdFabrica* fabrica,
          s32  genus,
          i32  linea_index,
          s32  ab,
          s32  ad)
{
    constans MdLinea* l = md_linea(fabrica->lineae, linea_index);

    si (l == NIHIL)
    {
        redde NIHIL;
    }
    redde materia_token_creare(fabrica->piscina, &fabrica->forma, genus,
        md_chorda_fontis(fabrica->lineae->fons, ab, ad), ab, l->numerus,
        (i32)(ab - l->offset) + I, MD_FONS_PLAGULAE);
}

MateriaToken*
md_lexema_terminator (
    MdFabrica* fabrica,
          i32  linea_index)
{
     constans MdLinea* l = md_linea(fabrica->lineae, linea_index);
                  s32  ab;
                  s32  genus;

    si (l == NIHIL || l->terminator == ZEPHYRUM)
    {
        redde NIHIL;
    }
    ab    = l->offset + (s32)l->mensura;
    genus = (fabrica->lineae->fons[ab + (s32)l->terminator - I] == '\n')
          ? (s32)MD_LEX_LINEA : (s32)MD_LEX_LINEA_CR;
    redde md_lexema_fabricare(fabrica, genus, linea_index, ab,
        ab + (s32)l->terminator);
}

MateriaToken*
md_lexema_finis (
    MdFabrica* fabrica)
{
    i32 n        = md_lineae_numerus(fabrica->lineae);
    i32 linea    = I;
    i32 columna  = I;

    si (n > ZEPHYRUM)
    {
        constans MdLinea* l = md_linea(fabrica->lineae, n - I);

        si (l->terminator > ZEPHYRUM)
        {
            linea = l->numerus + I;
        }
        alioquin
        {
            linea    = l->numerus;
            columna  = l->mensura + I;
        }
    }
    redde materia_token_creare(fabrica->piscina, &fabrica->forma,
        (s32)MD_LEX_FINIS,
        md_chorda_fontis(fabrica->lineae->fons,
        (s32)fabrica->lineae->mensura,
            (s32)fabrica->lineae->mensura),
        (s32)fabrica->lineae->mensura, linea, columna,
        MD_FONS_PLAGULAE);
}


/* ==================================================
 * Lexatio cruda
 * ================================================== */

interior b32
_addere (
             Xar* exitus,
    MateriaToken* t)
{
    MateriaToken** locus;

    si (t == NIHIL)
    {
        redde FALSUM;
    }
    locus = (MateriaToken**)xar_addere(exitus);
    si (locus == NIHIL)
    {
        redde FALSUM;
    }
    *locus = t;
    redde VERUM;
}

Xar*
md_lexare_crude (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura)
{
     MdLineae* lineae;
    MdFabrica  fabrica;
          Xar* exitus;
          i32  i;
          i32  n;

    lineae = (MdLineae*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(MdLineae));
    si (   lineae == NIHIL
        || !md_lineae_scindere(piscina, fons, mensura, lineae))
    {
        redde NIHIL;
    }
    exitus = xar_creare(piscina, (i32)magnitudo(MateriaToken*));
    si (exitus == NIHIL)
    {
        redde NIHIL;
    }
    md_fabrica_incipere(&fabrica, piscina, lineae);

    n = md_lineae_numerus(lineae);
    per (i = ZEPHYRUM; i < n; i++)
    {
         constans MdLinea* l   = md_linea(lineae, i);
                      s32  ab  = l->offset;
                      s32  ad  = l->offset + (s32)l->mensura;
                      i32  columnae;
                      s32  post;

        post = md_scan_indentatio(fons, ab, ad, ZEPHYRUM, &columnae);
        si (post > ab)
        {
            si (!_addere(exitus, md_lexema_fabricare(&fabrica,
                    (s32)MD_LEX_INDENTATIO, i, ab, post)))
            {
                redde NIHIL;
            }
        }
        si (ad > post)
        {
            si (!_addere(exitus, md_lexema_fabricare(&fabrica,
                    (s32)MD_LEX_TEXTUS, i, post, ad)))
            {
                redde NIHIL;
            }
        }
        si (l->terminator > ZEPHYRUM)
        {
            si (!_addere(exitus, md_lexema_terminator(&fabrica, i)))
            {
                redde NIHIL;
            }
        }
    }
    si (!_addere(exitus, md_lexema_finis(&fabrica)))
    {
        redde NIHIL;
    }
    redde exitus;
}
