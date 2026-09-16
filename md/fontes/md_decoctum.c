/* md_decoctum.c - Vide md_decoctum.h. */

#include "md_decoctum.h"
#include "entitates_html.h"
#include <string.h>


interior b32
_interpunctio_ascii (
    character c)
{
    redde (b32)((c >= '!' && c <= '/') || (c >= ':' && c <= '@')
                || (c >= '[' && c <= '`') || (c >= '{' && c <= '~'));
}

interior i32
_utf8 (
          i32  punctum,
    character* exitus)
{
    si (punctum < 0x80)
    {
        exitus[0] = (character)punctum;
        redde I;
    }
    si (punctum < 0x800)
    {
        exitus[0] = (character)(0xC0 | (punctum >> 6));
        exitus[1] = (character)(0x80 | (punctum & 0x3F));
        redde II;
    }
    si (punctum < 0x10000)
    {
        exitus[0] = (character)(0xE0 | (punctum >> 12));
        exitus[1] = (character)(0x80 | ((punctum >> 6) & 0x3F));
        exitus[2] = (character)(0x80 | (punctum & 0x3F));
        redde III;
    }
    exitus[0] = (character)(0xF0 | (punctum >> 18));
    exitus[1] = (character)(0x80 | ((punctum >> 12) & 0x3F));
    exitus[2] = (character)(0x80 | ((punctum >> 6) & 0x3F));
    exitus[3] = (character)(0x80 | (punctum & 0x3F));
    redde IV;
}

b32
md_ens_decoquere (
    constans character* fons,
                   s32  ab,
                   s32  ad,
             character* exitus,
                   i32* longitudo,
                   s32* post)
{
    s32 i        = ab + I;
    i32 punctum  = ZEPHYRUM;

    si (ab >= ad || fons[ab] != '&')
    {
        redde FALSUM;
    }
    si (i < ad && fons[i] == '#')
    {
        i32 digiti = ZEPHYRUM;

        i = i + I;
        si (i < ad && (fons[i] == 'x' || fons[i] == 'X'))
        {
            i = i + I;
            dum (i < ad && digiti < VI)
            {
                character c = fons[i];
                      i32 v;

                si (c >= '0' && c <= '9')
                {
                    v = (i32)(c - '0');
                }
                alioquin si (c >= 'a' && c <= 'f')
                {
                    v = (i32)(c - 'a') + X;
                }
                alioquin si (c >= 'A' && c <= 'F')
                {
                    v = (i32)(c - 'A') + X;
                }
                alioquin
                {
                    frange;
                }
                punctum  = punctum * XVI + v;
                digiti   = digiti + I;
                i        = i + I;
            }
        }
        alioquin
        {
            dum (   i < ad && digiti < VII && fons[i] >= '0'
                 && fons[i] <= '9')
            {
                punctum  = punctum * X + (i32)(fons[i] - '0');
                digiti   = digiti + I;
                i        = i + I;
            }
        }
        si (digiti == ZEPHYRUM || i >= ad || fons[i] != ';')
        {
            redde FALSUM;
        }
        si (   punctum == ZEPHYRUM || punctum > 0x10FFFF
            || (punctum >= 0xD800 && punctum <= 0xDFFF))
        {
            punctum = 0xFFFD;
        }
    }
    alioquin
    {
                         s32  n_ab = i;
        constans EntitasHtml* entitas;

        dum (   i < ad && ((fons[i] >= 'a' && fons[i] <= 'z')
                        || (fons[i] >= 'A' && fons[i] <= 'Z')
                        || (fons[i] >= '0' && fons[i] <= '9')))
        {
            i = i + I;
        }
        si (i == n_ab || i >= ad || fons[i] != ';')
        {
            redde FALSUM;
        }
        /* tabula HTML5 tota (lib/entitates_html, MMCCXXXI): titulus
         * cum ';' EXACTUS - CommonMark formas veteres sine ';' non
         * decoquit (O2b-4, 2026-09-15; olim tabula LVII hic) */
        entitas = entitates_html_quaerere(fons + n_ab,
            (i32)(i - n_ab + I));
        si (entitas == NIHIL)
        {
            redde FALSUM;
        }
        memcpy(exitus, entitas->octeti, (size_t)entitas->mensura);
        *longitudo  = entitas->mensura;
        *post       = i + I;
        redde VERUM;
    }
    *longitudo  = _utf8(punctum, exitus);
    *post       = i + I;
    redde VERUM;
}

chorda
md_decoquere (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura,
                   b32* mutatus)
{
       chorda  c;
    character* d;
          s32  i = ZEPHYRUM;
          i32  n = ZEPHYRUM;

    /* decoctum crudo LONGIUS esse potest: '&nGt;' (V octeti) -> VI
     * octeti UTF-8 (puncta duo; tabula tota HTML5, O2b-4) */
    d = (character*)piscina_allocare(piscina,
        (memoriae_index)mensura * II + VIII);
    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    *mutatus   = FALSUM;
    si (d == NIHIL)
    {
        redde c;
    }
    dum (i < (s32)mensura)
    {
        character ch = fons[i];

        si (   ch == '\\' && i + I < (s32)mensura
            && _interpunctio_ascii(fons[i + I]))
        {
            d[n]      = fons[i + I];
            n         = n + I;
            i         = i + II;
            *mutatus  = VERUM;
            perge;
        }
        si (ch == '&')
        {
            character e[VIII];
                  i32 l;
                  s32 post;

            si (md_ens_decoquere(fons, i, (s32)mensura, e, &l, &post))
            {
                memcpy(d + n, e, (size_t)l);
                n         = n + l;
                i         = post;
                *mutatus  = VERUM;
                perge;
            }
        }
        d[n]  = ch;
        n     = n + I;
        i     = i + I;
    }
    {
        unio { character* c; i8* m; } u;

        u.c        = d;
        c.datum    = u.m;
        c.mensura  = n;
    }
    redde c;
}

chorda
md_clavem_normalizare (
               Piscina* piscina,
    constans character* fons,
                   s32  ab,
                   s32  ad)
{
    character* d = (character*)piscina_allocare(piscina,
        (memoriae_index)(ad - ab) + I);
       i32 n = ZEPHYRUM;
       s32 i;
       b32 spatium = VERUM;
    chorda c;
    unio { character* c; i8* m; } u;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    si (d == NIHIL)
    {
        redde c;
    }
    per (i = ab; i < ad; i++)
    {
        character ch = fons[i];

        si (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r')
        {
            si (!spatium)
            {
                d[n]     = ' ';
                n        = n + I;
                spatium  = VERUM;
            }
            perge;
        }
        d[n] = (ch >= 'A'
            && ch <= 'Z') ? (character)(ch + ('a' - 'A')) : ch;
        n        = n + I;
        spatium  = FALSUM;
    }
    dum (n > ZEPHYRUM && d[n - I] == ' ')
    {
        n = n - I;
    }
    u.c        = d;
    c.datum    = u.m;
    c.mensura  = n;
    redde c;
}

interior b32
_octetus_tutus (
    i8 c)
{
    si (   (c >= (i8)'a' && c <= (i8)'z')
        || (c >= (i8)'A' && c <= (i8)'Z')
        || (c >= (i8)'0' && c <= (i8)'9'))
    {
        redde VERUM;
    }
    commutatio (c)
    {
        casus (i8)'-': casus (i8)'_': casus (i8)'.': casus (i8)'~':
        casus (i8)'!': casus (i8)'*': casus (i8)'\'': casus (i8)'(':
        casus (i8)')': casus (i8)';': casus (i8)':': casus (i8)'@':
        casus (i8)'&': casus (i8)'=': casus (i8)'+': casus (i8)'$':
        casus (i8)',': casus (i8)'/': casus (i8)'?': casus (i8)'#':
        casus (i8)'%':
            redde VERUM;
        ordinarius:
            redde FALSUM;
    }
}

chorda
md_url_codificare (
    Piscina* piscina,
     chorda  url)
{
    hic_manens constans character  HEX[] = "0123456789ABCDEF";
                              i32  i;
                              i32  codificandi = ZEPHYRUM;
                           chorda  exitus;
                               i8* d;
                              i32  o = ZEPHYRUM;

    per (i = ZEPHYRUM; i < url.mensura; i++)
    {
        si (!_octetus_tutus(url.datum[i]))
        {
            codificandi++;
        }
    }
    si (codificandi == ZEPHYRUM)
    {
        redde url;
    }
    exitus.mensura = url.mensura + codificandi * II;
    d = (i8*)piscina_allocare(piscina, (memoriae_index)exitus.mensura
        + I);
    si (d == NIHIL)
    {
        exitus.datum    = NIHIL;
        exitus.mensura  = ZEPHYRUM;
        redde exitus;
    }
    per (i = ZEPHYRUM; i < url.mensura; i++)
    {
        i8 c = url.datum[i];

        si (_octetus_tutus(c))
        {
            d[o++] = c;
        }
        alioquin
        {
            insignatus character u = (insignatus character)c;

            d[o++] = (i8)'%';
            d[o++] = (i8)HEX[u >> IV];
            d[o++] = (i8)HEX[u & XV];
        }
    }
    exitus.datum = d;
    redde exitus;
}
