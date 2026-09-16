/* entitates_html.c - Vide entitates_html.h. Tabula ENTITATES_HTML in
 * entitates_html_tabula.c GENERATA. */

#include "entitates_html.h"
#include <string.h>

/* C1 (0x80-0x9F) -> Windows-1252 (WHATWG 'numeric character reference
 * end state'); ZEPHYRUM = non in tabula (punctum ipsum manet) */
hic_manens constans i32 C1_SUBSTITUTA[32] = {
    0x20AC, 0,      0x201A, 0x0192, 0x201E, 0x2026, 0x2020, 0x2021,
    0x02C6, 0x2030, 0x0160, 0x2039, 0x0152, 0,      0x017D, 0,
    0,      0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 0x2013, 0x2014,
    0x02DC, 0x2122, 0x0161, 0x203A, 0x0153, 0,      0x017E, 0x0178
};

interior b32
_littera_aut_digitus (
    character c)
{
    redde (b32)((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
                || (c >= '0' && c <= '9'));
}

interior i32
_utf8 (
          i32  punctum,
    character* exitus)
{
    si (punctum < 0x80)
    {
        exitus[ZEPHYRUM] = (character)punctum;
        redde I;
    }
    si (punctum < 0x800)
    {
        exitus[ZEPHYRUM]  = (character)(0xC0 | (punctum >> VI));
        exitus[I]         = (character)(0x80 | (punctum & 0x3F));
        redde II;
    }
    si (punctum < 0x10000)
    {
        exitus[ZEPHYRUM] = (character)(0xE0 | (punctum >> XII));
        exitus[I] = (character)(0x80 | ((punctum
            >> VI) & 0x3F));
        exitus[II] = (character)(0x80 | (punctum & 0x3F));
        redde III;
    }
    exitus[ZEPHYRUM]  = (character)(0xF0 | (punctum >> XVIII));
    exitus[I]         = (character)(0x80 | ((punctum >> XII) & 0x3F));
    exitus[II]        = (character)(0x80 | ((punctum >> VI) & 0x3F));
    exitus[III]       = (character)(0x80 | (punctum & 0x3F));
    redde IV;
}

/* titulus tabulae (NUL terminatus) contra clavem [n]: < 0, 0, > 0 -
 * praefixum brevius prius (ordo Pythonis sorted, idem) */
interior integer
_conferre (
    constans character* titulus,
    constans character* clavis,
                   i32  n)
{
    i32 k;

    per (k = ZEPHYRUM; k < n; k++)
    {
        insignatus character a = (insignatus character)titulus[k];
        insignatus character b = (insignatus character)clavis[k];

        si (a == ZEPHYRUM)
        {
            redde -I;
        }
        si (a != b)
        {
            redde (integer)a - (integer)b;
        }
    }
    redde titulus[n] == '\0' ? ZEPHYRUM : I;
}

constans EntitasHtml*
entitates_html_quaerere (
    constans character* titulus,
                   i32  mensura)
{
    s32 a = ZEPHYRUM;
    s32 b = (s32)ENTITATES_HTML_NUMERUS - I;

    si (mensura == ZEPHYRUM)
    {
        redde NIHIL;
    }
    dum (a <= b)
    {
            s32 m = a + (b - a) / II;
        integer r = _conferre(ENTITATES_HTML[m].titulus, titulus,
            mensura);

        si (r == ZEPHYRUM)
        {
            redde &ENTITATES_HTML[m];
        }
        si (r < ZEPHYRUM)
        {
            a = m + I;
        }
        alioquin
        {
            b = m - I;
        }
    }
    redde NIHIL;
}

interior b32
_numerum_decoquere (
    constans character* fons,
                   s32  i,
                   s32  ad,
             character* exitus,
                   i32* longitudo,
                   s32* post)
{
    i32 punctum  = ZEPHYRUM;
    i32 digiti   = ZEPHYRUM;
    b32 nimium   = FALSUM;
    b32 hex      = FALSUM;

    /* i post '#' */
    si (i < ad && (fons[i] == 'x' || fons[i] == 'X'))
    {
        hex  = VERUM;
        i    = i + I;
    }
    dum (i < ad)
    {
        character c = fons[i];
              i32 v;

        si (c >= '0' && c <= '9')
        {
            v = (i32)(c - '0');
        }
        alioquin si (hex && c >= 'a' && c <= 'f')
        {
            v = (i32)(c - 'a') + X;
        }
        alioquin si (hex && c >= 'A' && c <= 'F')
        {
            v = (i32)(c - 'A') + X;
        }
        alioquin
        {
            frange;
        }
        si (punctum > 0x10FFFF)
        {
            nimium = VERUM;   /* consumere pergit, valor non crescit */
        }
        alioquin
        {
            punctum = punctum * (hex ? XVI : X) + v;
        }
        digiti  = digiti + I;
        i       = i + I;
    }
    si (digiti == ZEPHYRUM)
    {
        redde FALSUM;   /* '&#' nudum: nulla referentia */
    }
    si (i < ad && fons[i] == ';')
    {
        i = i + I;
    }
    si (   nimium || punctum == ZEPHYRUM || punctum > 0x10FFFF
        || (punctum >= 0xD800 && punctum <= 0xDFFF))
    {
        punctum = 0xFFFD;
    }
    alioquin si (   punctum >= 0x80 && punctum <= 0x9F
                 && C1_SUBSTITUTA[punctum - 0x80] != ZEPHYRUM)
    {
        punctum = C1_SUBSTITUTA[punctum - 0x80];
    }
    *longitudo  = _utf8(punctum, exitus);
    *post       = i;
    redde VERUM;
}

b32
entitates_html_decoquere (
    constans character* fons,
                   s32  ab,
                   s32  ad,
                   b32  in_attributo,
             character* exitus,
                   i32* longitudo,
                   s32* post)
{
    s32 i = ab + I;
    i32 n = ZEPHYRUM;
    i32 l;
    b32 semicolon;

    si (ab >= ad || fons[ab] != '&')
    {
        redde FALSUM;
    }
    si (i < ad && fons[i] == '#')
    {
        redde _numerum_decoquere(fons, i + I, ad, exitus, longitudo,
            post);
    }
    dum (   i + (s32)n < ad && n < XXXII && _littera_aut_digitus(fons[i
        + (s32)n]))
    {
        n = n + I;
    }
    si (n == ZEPHYRUM)
    {
        redde FALSUM;
    }
    semicolon = (b32)(i + (s32)n < ad && fons[i + (s32)n] == ';');
    per (l = n; l >= I; l--)
    {
        constans EntitasHtml* e;

        si (semicolon && l == n)
        {
            e = entitates_html_quaerere(fons + i, n + I);
            si (e != NIHIL)
            {
                memcpy(exitus, e->octeti, (size_t)e->mensura);
                *longitudo  = e->mensura;
                *post       = i + (s32)n + I;
                redde VERUM;
            }
        }
        e = entitates_html_quaerere(fons + i, l);
        si (e != NIHIL)
        {
            /* forma vetus sine ';' */
            si (in_attributo)
            {
                character c = (i + (s32)l < ad) ? fons[i
                    + (s32)l] : '\0';

                si (c == '=' || _littera_aut_digitus(c))
                {
                    redde FALSUM;
                }
            }
            memcpy(exitus, e->octeti, (size_t)e->mensura);
            *longitudo  = e->mensura;
            *post       = i + (s32)l;
            redde VERUM;
        }
    }
    redde FALSUM;
}
