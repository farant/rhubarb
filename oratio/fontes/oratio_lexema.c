/* oratio_lexema.c - Vide oratio_lexema.h. */

#include "oratio_lexema.h"
#include "oratio_lexicon.h"
#include <string.h>

hic_manens constans MateriaTokenForma FORMA = { ZEPHYRUM };

/* Genus octeti ad positionem: quid sit et quot octetos sumat. */
nomen enumeratio {
    OCTETUS_LITTERA = 0,
    OCTETUS_DIGITUS,
    OCTETUS_SPATIUM,
    OCTETUS_INTERPUNCTIO,
    OCTETUS_APOSTROPHUS_CURVUS,   /* U+2019 */
    OCTETUS_SIGNUM
} OctetusGenus;

interior s32
_longitudo_utf8 (
    constans character* fons,
                   s32  i,
                   s32  ad)
{
    insignatus character c = (insignatus character)fons[i];
                     s32 n;

    si (c < 0x80)
    {
        redde (s32)I;
    }
    n = (c >= 0xF0) ? (s32)IV : (c >= 0xE0) ? (s32)III
        : (c >= 0xC0) ? (s32)II : (s32)I;
    si (i + n > ad)
    {
        n = ad - i;
    }
    redde n;
}

interior OctetusGenus
_genus_octeti (
    constans character* fons,
                   s32  i,
                   s32  ad,
                   s32* longitudo)
{
    insignatus character c = (insignatus character)fons[i];

    *longitudo = (s32)I;
    si ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
    {
        redde OCTETUS_LITTERA;
    }
    si (c >= '0' && c <= '9')
    {
        redde OCTETUS_DIGITUS;
    }
    si (c == ' ' || c == '\t')
    {
        redde OCTETUS_SPATIUM;
    }
    si (c >= 0x80)
    {
        s32 n = _longitudo_utf8(fons, i, ad);

        *longitudo = n;
        /* NBSP C2 A0 */
                si (   n == (s32)II && c == 0xC2
                    && (insignatus character)fons[i + I] == 0xA0)
                {
            redde OCTETUS_SPATIUM;
                }
        /* General Punctuation U+2000..U+206F: E2 80 80 .. E2 81 AF */
                si (n == (s32)III && c == 0xE2)
                {
            insignatus character c2 = (insignatus character)fons[i + I];
            insignatus character c3 = (insignatus character)fons[i
                + II];

            si (c2 == 0x80 && c3 >= 0x80 && c3 <= 0x8A)
            {
                redde OCTETUS_SPATIUM;          /* U+2000..U+200A */
            }
            si (c2 == 0x80 && c3 == 0x99)
            {
                redde OCTETUS_APOSTROPHUS_CURVUS;   /* U+2019 */
            }
            si (   (c2 == 0x80 && c3 >= 0x8B)
                || (c2 == 0x81 && c3 <= 0xAF))
            {
                redde OCTETUS_INTERPUNCTIO;     /* U+200B..U+206F */
            }
                }
        redde OCTETUS_LITTERA;
    }
    si (c < 0x20 || c == 0x7F)
    {
        redde OCTETUS_SIGNUM;   /* imperii ('\n' '\r' a vocante tractati) */
    }
    redde OCTETUS_INTERPUNCTIO;   /* ASCII interpunctio */
}

b32
oratio_littera_est (
    constans character* fons,
                   s32  i,
                   s32  ad,
                   i32* longitudo)
{
             s32 n;
    OctetusGenus g;

    si (i < ZEPHYRUM || i >= ad)
    {
        *longitudo = ZEPHYRUM;
        redde FALSUM;
    }
    g           = _genus_octeti(fons, i, ad, &n);
    *longitudo  = (i32)n;
    redde (b32)(g == OCTETUS_LITTERA);
}

interior b32
_littera_ante (
    constans character* fons,
                   s32  i)
{
    insignatus character c;

    si (i <= ZEPHYRUM)
    {
        redde FALSUM;
    }
    c = (insignatus character)fons[i - I];
    /* octetus ultimus litterae ASCII aut sequentiae >= 0x80 (continuatio
     * 0x80..0xBF aut initium unius octeti) - approximatio: >= 0x80 ANTE
     * praeter General Punctuation quam fons[i-3..i-1] esse potest */
    si ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
    {
        redde VERUM;
    }
    si (c >= 0x80)
    {
        /* U+2000..U+206F terminant in 0x80..0xAF post E2 80/81 */
        si (   i >= III && (insignatus character)fons[i - III] == 0xE2
            && ((insignatus character)fons[i - II] == 0x80
                || (insignatus character)fons[i - II] == 0x81))
        {
            redde FALSUM;
        }
        si (   i >= II && (insignatus character)fons[i - II] == 0xC2
            && c == 0xA0)
        {
            redde FALSUM;   /* NBSP */
        }
        redde VERUM;
    }
    redde FALSUM;
}

interior b32
_littera_post (
    constans character* fons,
                   s32  i,
                   s32  ad)
{
    i32 n;

    redde oratio_littera_est(fons, i, ad, &n);
}

nomen structura {
               Piscina* piscina;
    constans character* fons;
                   Xar* lexemata;
                   i32  linea;
                   i32  columna;   /* I-basata, octetus in linea */
} Lexator;

interior b32
_emittere (
    Lexator* l,
        s32  genus,
        s32  ab,
        s32  ad)
{
    MateriaToken* t;
    MateriaToken** locus;
    chorda valor;
    unio { constans character* c; i8* m; } u;   /* fons constans: unio ut md */

    u.c            = l->fons + ab;
    valor.datum    = u.m;
    valor.mensura  = (i32)(ad - ab);
    t = materia_token_creare(l->piscina, &FORMA, genus, valor, ab,
        l->linea,
        l->columna, ORATIO_FONS_PLAGULAE);
    si (t == NIHIL)
    {
        redde FALSUM;
    }
    locus = (MateriaToken**)xar_addere(l->lexemata);
    si (locus == NIHIL)
    {
        redde FALSUM;
    }
    *locus      = t;
    l->columna  = l->columna + (i32)(ad - ab);
    redde VERUM;
}

Xar*
oratio_lexare (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura)
{
    Lexator l;
        s32 i   = ZEPHYRUM;
        s32 ad  = (s32)mensura;

    l.piscina   = piscina;
    l.fons      = fons;
    l.lexemata  = xar_creare(piscina, (i32)magnitudo(MateriaToken*));
    l.linea     = I;
    l.columna   = I;
    si (l.lexemata == NIHIL)
    {
        redde NIHIL;
    }
    dum (i < ad)
    {
        insignatus character c = (insignatus character)fons[i];
                         s32 n;
                OctetusGenus g;
                         s32 ab = i;

        /* lineae novae */
        si (c == '\n' || c == '\r')
        {
            si (c == '\r' && i + I < ad && fons[i + I] == '\n')
            {
                si (!_emittere(&l, (s32)ORATIO_LEX_LINEA, i, i + II))
                {
                    redde NIHIL;
                }
                i = i + II;
            }
            alioquin si (c == '\r')
            {
                si (!_emittere(&l, (s32)ORATIO_LEX_LINEA_CR, i, i + I))
                {
                    redde NIHIL;
                }
                i = i + I;
            }
            alioquin
            {
                si (!_emittere(&l, (s32)ORATIO_LEX_LINEA, i, i + I))
                {
                    redde NIHIL;
                }
                i = i + I;
            }
            l.linea    = l.linea + I;
            l.columna  = I;
            perge;
        }
        g = _genus_octeti(fons, i, ad, &n);
        commutatio (g)
        {
            casus OCTETUS_LITTERA:
                dum (i < ad)
                {
                                     s32 m;
                    insignatus character d =
                        (insignatus character)fons[i];

                    si (d == '\n' || d == '\r')
                    {
                        frange;
                    }
                    si (_genus_octeti(fons, i, ad, &m)
                        != OCTETUS_LITTERA)
                    {
                        frange;
                    }
                    i = i + m;
                }
                si (!_emittere(&l, (s32)ORATIO_LEX_LITTERAE, ab, i))
                {
                    redde NIHIL;
                }
                frange;
            casus OCTETUS_DIGITUS:
                dum (i < ad && fons[i] >= '0' && fons[i] <= '9')
                {
                    i = i + I;
                }
                si (!_emittere(&l, (s32)ORATIO_LEX_DIGITI, ab, i))
                {
                    redde NIHIL;
                }
                frange;
            casus OCTETUS_SPATIUM:
                dum (i < ad)
                {
                                     s32 m;
                    insignatus character d =
                        (insignatus character)fons[i];

                    si (d == '\n' || d == '\r')
                    {
                        frange;
                    }
                    si (_genus_octeti(fons, i, ad, &m)
                        != OCTETUS_SPATIUM)
                    {
                        frange;
                    }
                    i = i + m;
                }
                si (!_emittere(&l, (s32)ORATIO_LEX_SPATIUM, ab, i))
                {
                    redde NIHIL;
                }
                frange;
            casus OCTETUS_SIGNUM:
                dum (i < ad)
                {
                                     s32 m;
                    insignatus character d =
                        (insignatus character)fons[i];

                    si (d == '\n' || d == '\r')
                    {
                        frange;
                    }
                    si (_genus_octeti(fons, i, ad, &m)
                        != OCTETUS_SIGNUM)
                    {
                        frange;
                    }
                    i = i + m;
                }
                si (!_emittere(&l, (s32)ORATIO_LEX_SIGNUM, ab, i))
                {
                    redde NIHIL;
                }
                frange;
            casus OCTETUS_APOSTROPHUS_CURVUS:
                si (   _littera_ante(fons, i)
                    && _littera_post(fons, i + n, ad))
                {
                    si (!_emittere(&l, (s32)ORATIO_LEX_APOSTROPHUS, i, i
                        + n))
                    {
                        redde NIHIL;
                    }
                }
                alioquin si (!_emittere(&l,
                             (s32)ORATIO_LEX_INTERPUNCTIO, i,
                                        i + n))
                {
                    redde NIHIL;
                }
                i = i + n;
                frange;
            casus OCTETUS_INTERPUNCTIO:
            ordinarius:
                si (n > (s32)I)
                {
                    /* interpunctio unicode: lexema unum per sequentiam */
                    si (!_emittere(&l, (s32)ORATIO_LEX_INTERPUNCTIO, i,
                        i + n))
                    {
                        redde NIHIL;
                    }
                    i = i + n;
                    frange;
                }
                si (   c == '-' && _littera_ante(fons, i)
                    && _littera_post(fons, i + I, ad))
                {
                    si (!_emittere(&l, (s32)ORATIO_LEX_HYPHEN, i, i
                        + I))
                    {
                        redde NIHIL;
                    }
                    i = i + I;
                    frange;
                }
                si (   c == '\'' && _littera_ante(fons, i)
                    && _littera_post(fons, i + I, ad))
                {
                    si (!_emittere(&l, (s32)ORATIO_LEX_APOSTROPHUS, i, i
                        + I))
                    {
                        redde NIHIL;
                    }
                    i = i + I;
                    frange;
                }
                si (   c == '.' && !(i + I < ad && fons[i + I] == '.')
                    && !(i > ZEPHYRUM && fons[i - I] == '.'))
                {
                    si (!_emittere(&l, (s32)ORATIO_LEX_PUNCTUM, i, i
                        + I))
                    {
                        redde NIHIL;
                    }
                    i = i + I;
                    frange;
                }
                /* cursus eiusdem octeti */
                i = i + I;
                dum (i < ad && (insignatus character)fons[i] == c)
                {
                    i = i + I;
                }
                si (!_emittere(&l, (s32)ORATIO_LEX_INTERPUNCTIO, ab, i))
                {
                    redde NIHIL;
                }
                frange;
        }
    }
    /* FINIS */
    {
        MateriaToken* t;
        MateriaToken** locus;
        chorda vacua;

        vacua.datum    = NIHIL;
        vacua.mensura  = ZEPHYRUM;
        t = materia_token_creare(piscina, &FORMA, (s32)ORATIO_LEX_FINIS,
            vacua,
            ad, l.linea, l.columna, ORATIO_FONS_PLAGULAE);
        si (t == NIHIL)
        {
            redde NIHIL;
        }
        locus = (MateriaToken**)xar_addere(l.lexemata);
        si (locus == NIHIL)
        {
            redde NIHIL;
        }
        *locus = t;
    }
    redde l.lexemata;
}

MateriaToken*
oratio_lexema_derivatum (
                  Piscina* piscina,
                      s32  genus,
                   chorda  valor,
    constans MateriaToken* origo)
{
    chorda copia;

    copia.mensura = valor.mensura;
    copia.datum   = (i8*)piscina_allocare(piscina,
        (memoriae_index)(valor.mensura > ZEPHYRUM ? valor.mensura : I));
    si (copia.datum == NIHIL)
    {
        redde NIHIL;
    }
    si (valor.mensura > ZEPHYRUM)
    {
        memcpy(copia.datum, valor.datum, (size_t)valor.mensura);
    }
    redde materia_token_creare(piscina, &FORMA, genus, copia, (s32)-I,
        origo != NIHIL ? origo->linea : (i32)I,
        origo != NIHIL ? origo->columna : (i32)I,
        ORATIO_FONS_DERIVATUS);
}
