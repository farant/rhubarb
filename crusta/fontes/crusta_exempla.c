/* crusta_exempla.c - Lector casuum crustae (vide .h) */

#include "crusta_exempla.h"
#include <string.h>

constans character* constans CRUSTA_FREEBSD_PLAGULAE[] = {
    "probationes/fixa/crusta/freebsd/alias1.0",
    "probationes/fixa/crusta/freebsd/alias10.0",
    "probationes/fixa/crusta/freebsd/alias11.0",
    "probationes/fixa/crusta/freebsd/alias12.0",
    "probationes/fixa/crusta/freebsd/alias13.0",
    "probationes/fixa/crusta/freebsd/alias14.0",
    "probationes/fixa/crusta/freebsd/alias15.0",
    "probationes/fixa/crusta/freebsd/alias16.0",
    "probationes/fixa/crusta/freebsd/alias17.0",
    "probationes/fixa/crusta/freebsd/alias18.0",
    "probationes/fixa/crusta/freebsd/alias19.0",
    "probationes/fixa/crusta/freebsd/alias2.0",
    "probationes/fixa/crusta/freebsd/alias20.0",
    "probationes/fixa/crusta/freebsd/alias3.0",
    "probationes/fixa/crusta/freebsd/alias4.0",
    "probationes/fixa/crusta/freebsd/alias5.0",
    "probationes/fixa/crusta/freebsd/alias6.0",
    "probationes/fixa/crusta/freebsd/alias7.0",
    "probationes/fixa/crusta/freebsd/alias8.0",
    "probationes/fixa/crusta/freebsd/alias9.0",
    "probationes/fixa/crusta/freebsd/and-pipe-not.0",
    "probationes/fixa/crusta/freebsd/case1.0",
    "probationes/fixa/crusta/freebsd/case2.0",
    "probationes/fixa/crusta/freebsd/comment1.0",
    "probationes/fixa/crusta/freebsd/comment2.42",
    "probationes/fixa/crusta/freebsd/dollar-quote1.0",
    "probationes/fixa/crusta/freebsd/dollar-quote10.0",
    "probationes/fixa/crusta/freebsd/dollar-quote11.0",
    "probationes/fixa/crusta/freebsd/dollar-quote12.0",
    "probationes/fixa/crusta/freebsd/dollar-quote13.0",
    "probationes/fixa/crusta/freebsd/dollar-quote2.0",
    "probationes/fixa/crusta/freebsd/dollar-quote3.0",
    "probationes/fixa/crusta/freebsd/dollar-quote4.0",
    "probationes/fixa/crusta/freebsd/dollar-quote5.0",
    "probationes/fixa/crusta/freebsd/dollar-quote6.0",
    "probationes/fixa/crusta/freebsd/dollar-quote7.0",
    "probationes/fixa/crusta/freebsd/dollar-quote8.0",
    "probationes/fixa/crusta/freebsd/dollar-quote9.0",
    "probationes/fixa/crusta/freebsd/empty-braces1.0",
    "probationes/fixa/crusta/freebsd/empty-cmd1.0",
    "probationes/fixa/crusta/freebsd/for1.0",
    "probationes/fixa/crusta/freebsd/for2.0",
    "probationes/fixa/crusta/freebsd/func1.0",
    "probationes/fixa/crusta/freebsd/func2.0",
    "probationes/fixa/crusta/freebsd/func3.0",
    "probationes/fixa/crusta/freebsd/heredoc1.0",
    "probationes/fixa/crusta/freebsd/heredoc10.0",
    "probationes/fixa/crusta/freebsd/heredoc11.0",
    "probationes/fixa/crusta/freebsd/heredoc12.0",
    "probationes/fixa/crusta/freebsd/heredoc13.0",
    "probationes/fixa/crusta/freebsd/heredoc14.0",
    "probationes/fixa/crusta/freebsd/heredoc15.0",
    "probationes/fixa/crusta/freebsd/heredoc16.0",
    "probationes/fixa/crusta/freebsd/heredoc2.0",
    "probationes/fixa/crusta/freebsd/heredoc3.0",
    "probationes/fixa/crusta/freebsd/heredoc4.0",
    "probationes/fixa/crusta/freebsd/heredoc5.0",
    "probationes/fixa/crusta/freebsd/heredoc6.0",
    "probationes/fixa/crusta/freebsd/heredoc7.0",
    "probationes/fixa/crusta/freebsd/heredoc8.0",
    "probationes/fixa/crusta/freebsd/heredoc9.0",
    "probationes/fixa/crusta/freebsd/line-cont1.0",
    "probationes/fixa/crusta/freebsd/line-cont10.0",
    "probationes/fixa/crusta/freebsd/line-cont11.0",
    "probationes/fixa/crusta/freebsd/line-cont12.0",
    "probationes/fixa/crusta/freebsd/line-cont2.0",
    "probationes/fixa/crusta/freebsd/line-cont3.0",
    "probationes/fixa/crusta/freebsd/line-cont4.0",
    "probationes/fixa/crusta/freebsd/line-cont5.0",
    "probationes/fixa/crusta/freebsd/line-cont6.0",
    "probationes/fixa/crusta/freebsd/line-cont7.0",
    "probationes/fixa/crusta/freebsd/line-cont8.0",
    "probationes/fixa/crusta/freebsd/line-cont9.0",
    "probationes/fixa/crusta/freebsd/no-space1.0",
    "probationes/fixa/crusta/freebsd/no-space2.0",
    "probationes/fixa/crusta/freebsd/nul1.0",
    "probationes/fixa/crusta/freebsd/only-redir1.0",
    "probationes/fixa/crusta/freebsd/only-redir2.0",
    "probationes/fixa/crusta/freebsd/only-redir3.0",
    "probationes/fixa/crusta/freebsd/only-redir4.0",
    "probationes/fixa/crusta/freebsd/pipe-not1.0",
    "probationes/fixa/crusta/freebsd/ps1-expand1.0",
    "probationes/fixa/crusta/freebsd/ps1-expand2.0",
    "probationes/fixa/crusta/freebsd/ps1-expand3.0",
    "probationes/fixa/crusta/freebsd/ps1-expand4.0",
    "probationes/fixa/crusta/freebsd/ps1-expand5.0",
    "probationes/fixa/crusta/freebsd/ps2-expand1.0",
    "probationes/fixa/crusta/freebsd/set-v1.0",
    "probationes/fixa/crusta/freebsd/var-assign1.0",
    NIHIL
};


/* ==================================================
 * Auxilia
 * ================================================== */

interior b32
_incipit (
    constans character* fons,
                   i32  ab,
                   i32  finis,
    constans character* praefixum)
{
    i32 n = (i32)strlen(praefixum);

    redde finis - ab >= n
        && memcmp(fons + ab, praefixum, (size_t)n) == ZEPHYRUM;
}

interior b32
_vacans (
    character c)
{
    redde c == ' ' || c == '\t' || c == '\r';
}

/* copia [ab, ad) in piscina; datum NIHIL si memoria deficit */
interior chorda
_segmentum (
               Piscina* piscina,
    constans character* fons,
                   i32  ab,
                   i32  ad)
{
       chorda  c;
    character* copia = (character*)piscina_allocare(piscina,
        (memoriae_index)(ad - ab) + I);

    c.mensura = ad - ab;
    si (copia == NIHIL)
    {
        c.datum = NIHIL;
        redde c;
    }
    memcpy(copia, fons + ab, (size_t)(ad - ab));
    copia[ad - ab]  = '\0';
    c.datum         = (i8*)copia;
    redde c;
}

/* segmentum [ab, ad) detonsum (spatia, tabulae, '\r' utrimque) */
interior chorda
_detonsa (
               Piscina* piscina,
    constans character* fons,
                   i32  ab,
                   i32  ad)
{
    dum (ab < ad && _vacans(fons[ab]))
    {
        ab++;
    }
    dum (ad > ab && _vacans(fons[ad - I]))
    {
        ad--;
    }
    redde _segmentum(piscina, fons, ab, ad);
}


/* ==================================================
 * Lector
 * ================================================== */

Xar*
crusta_exempla_legere (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura)
{
               Xar* exempla;
    CrustaExemplum* e              = NIHIL;
      CrustaSectio* sectio_aperta  = NIHIL;
    /* initium dati aperti; -I = clausum */
    s32 datum_initium   = (s32)-I;
    s32 sectio_initium  = ZEPHYRUM;
    i32 ls              = ZEPHYRUM;
    i32 linea           = ZEPHYRUM;

    exempla = xar_creare(piscina, (i32)magnitudo(CrustaExemplum));
    si (exempla == NIHIL)
    {
        redde NIHIL;
    }
    dum (ls < mensura)
    {
        i32 le = ls;
        i32 proxima;

        dum (le < mensura && fons[le] != '\n')
        {
            le++;
        }
        proxima = le < mensura ? le + I : le;
        linea++;

        /* intra sectionem plurium linearum solum '## END' valet */
        si (sectio_aperta != NIHIL)
        {
            si (_incipit(fons, ls, le, "## "))
            {
                chorda reliquum = _detonsa(piscina, fons, ls + III, le);

                si (   reliquum.mensura == (i32)III
                    && memcmp(reliquum.datum, "END", (size_t)III)
                        == ZEPHYRUM)
                {
                    sectio_aperta->textus = _segmentum(piscina, fons,
                        (i32)sectio_initium, ls);
                    sectio_aperta = NIHIL;
                }
            }
            ls = proxima;
            perge;
        }
        si (_incipit(fons, ls, le, "#### "))
        {
            si (e != NIHIL && datum_initium >= ZEPHYRUM)
            {
                e->datum = _segmentum(piscina, fons, (i32)datum_initium,
                    ls);
            }
            e = (CrustaExemplum*)xar_addere(exempla);
            si (e == NIHIL)
            {
                redde NIHIL;
            }
            memset(e, ZEPHYRUM, magnitudo(*e));
            e->numerus  = xar_numerus(exempla);
            e->linea    = linea;
            e->titulus  = _detonsa(piscina, fons, ls + V, le);
            e->sectiones  = xar_creare(piscina,
                (i32)magnitudo(CrustaSectio));
            si (e->sectiones == NIHIL)
            {
                redde NIHIL;
            }
            datum_initium = (s32)proxima;
            e->datum = _segmentum(piscina, fons, proxima, proxima);
            ls = proxima;
            perge;
        }
        si (e != NIHIL && _incipit(fons, ls, le, "## "))
        {
                     i32  colon = ls + III;
            CrustaSectio* s;

            si (datum_initium >= ZEPHYRUM)
            {
                e->datum       = _segmentum(piscina, fons,
                    (i32)datum_initium,
                    ls);
                datum_initium  = (s32)-I;
            }
            dum (colon < le && fons[colon] != ':')
            {
                colon++;
            }
            s = (CrustaSectio*)xar_addere(e->sectiones);
            si (s == NIHIL)
            {
                redde NIHIL;
            }
            s->titulus = _detonsa(piscina, fons, ls + III, colon);
            si (colon < le)
            {
                s->textus = _detonsa(piscina, fons, colon + I, le);
                si (s->textus.mensura == ZEPHYRUM)
                {
                    /* '## NOMEN:' solum: plures lineae usque ad
                     * '## END' */
                    sectio_aperta   = s;
                    sectio_initium  = (s32)proxima;
                    s->textus = _segmentum(piscina, fons, proxima,
                        proxima);
                }
            }
            alioquin
            {
                s->textus = _segmentum(piscina, fons, le, le);
            }
            ls = proxima;
            perge;
        }
        ls = proxima;
    }
    si (sectio_aperta != NIHIL)
    {
        sectio_aperta->textus = _segmentum(piscina, fons,
            (i32)sectio_initium,
            mensura);
    }
    si (e != NIHIL && datum_initium >= ZEPHYRUM)
    {
        e->datum = _segmentum(piscina, fons, (i32)datum_initium,
            mensura);
    }
    redde exempla;
}

chorda
crusta_exemplum_sectio (
    constans CrustaExemplum* exemplum,
         constans character* titulus)
{
    chorda nulla;
       i32 n = (i32)strlen(titulus);
       i32 i;

    nulla.mensura  = ZEPHYRUM;
    nulla.datum    = NIHIL;
    si (exemplum == NIHIL || exemplum->sectiones == NIHIL)
    {
        redde nulla;
    }
    per (i = ZEPHYRUM; i < xar_numerus(exemplum->sectiones); i++)
    {
        constans CrustaSectio* s =
            (constans CrustaSectio*)xar_obtinere(exemplum->sectiones,
            i);

        si (   s->titulus.mensura                           == n
            && memcmp(s->titulus.datum, titulus, (size_t)n) == ZEPHYRUM)
        {
            redde s->textus;
        }
    }
    redde nulla;
}
