/* html_exempla.c - Vide html_exempla.h. */

#include "html_exempla.h"
#include <string.h>

#define RADIX_FIXURAE "probationes/fixa/html/html5lib/"

constans character* constans HTML_EXEMPLA_PLAGULAE[] = {
    RADIX_FIXURAE "adoption01.dat",
    RADIX_FIXURAE "adoption02.dat",
    RADIX_FIXURAE "blocks.dat",
    RADIX_FIXURAE "comments01.dat",
    RADIX_FIXURAE "doctype01.dat",
    RADIX_FIXURAE "domjs-unsafe.dat",
    RADIX_FIXURAE "entities01.dat",
    RADIX_FIXURAE "entities02.dat",
    RADIX_FIXURAE "foreign-fragment.dat",
    RADIX_FIXURAE "html5test-com.dat",
    RADIX_FIXURAE "inbody01.dat",
    RADIX_FIXURAE "isindex.dat",
    RADIX_FIXURAE "main-element.dat",
    RADIX_FIXURAE "math.dat",
    RADIX_FIXURAE "menuitem-element.dat",
    RADIX_FIXURAE "namespace-sensitivity.dat",
    RADIX_FIXURAE "noscript01.dat",
    RADIX_FIXURAE "pending-spec-changes-plain-text-unsafe.dat",
    RADIX_FIXURAE "pending-spec-changes.dat",
    RADIX_FIXURAE "plain-text-unsafe.dat",
    RADIX_FIXURAE "ruby.dat",
    RADIX_FIXURAE "scriptdata01.dat",
    RADIX_FIXURAE "svg.dat",
    RADIX_FIXURAE "tables01.dat",
    RADIX_FIXURAE "template.dat",
    RADIX_FIXURAE "tests1.dat",
    RADIX_FIXURAE "tests10.dat",
    RADIX_FIXURAE "tests11.dat",
    RADIX_FIXURAE "tests12.dat",
    RADIX_FIXURAE "tests14.dat",
    RADIX_FIXURAE "tests15.dat",
    RADIX_FIXURAE "tests16.dat",
    RADIX_FIXURAE "tests17.dat",
    RADIX_FIXURAE "tests18.dat",
    RADIX_FIXURAE "tests19.dat",
    RADIX_FIXURAE "tests2.dat",
    RADIX_FIXURAE "tests20.dat",
    RADIX_FIXURAE "tests21.dat",
    RADIX_FIXURAE "tests22.dat",
    RADIX_FIXURAE "tests23.dat",
    RADIX_FIXURAE "tests24.dat",
    RADIX_FIXURAE "tests25.dat",
    RADIX_FIXURAE "tests26.dat",
    RADIX_FIXURAE "tests3.dat",
    RADIX_FIXURAE "tests4.dat",
    RADIX_FIXURAE "tests5.dat",
    RADIX_FIXURAE "tests6.dat",
    RADIX_FIXURAE "tests7.dat",
    RADIX_FIXURAE "tests8.dat",
    RADIX_FIXURAE "tests9.dat",
    RADIX_FIXURAE "tests_innerHTML_1.dat",
    RADIX_FIXURAE "tricky01.dat",
    RADIX_FIXURAE "webkit01.dat",
    RADIX_FIXURAE "webkit02.dat",
    NIHIL
};

/* clavis sectionis apertae */
enumeratio {
    SECTIO_NULLA = 0,
    SECTIO_IGNORATA,
    SECTIO_DATUM,
    SECTIO_ERRORES,
    SECTIO_CONTEXTUS,
    SECTIO_SCRIPTUM_APERTUM,
    SECTIO_SCRIPTUM_CLAUSUM,
    SECTIO_DOCUMENTUM
};

nomen structura {
    i32 clavis;
    /* offset lineae primae contenti */
    s32 initium;
    /* offset post lineam ultimam ('\n' inclusum) */
    s32 finis;
} SectioCurrens;

/* spatium album quod Python str.strip() detondet */
interior b32
_spatium_est (
    character c)
{
    redde (b32)(c == ' ' || c == '\t' || c == '\r' || c == '\n'
                || c == '\v' || c == '\f');
}

interior b32
_titulus_est (
    constans character* fons,
                   s32  a,
                   s32  b,
    constans character* litterae)
{
    i32 n = (i32)strlen(litterae);

    redde (b32)((i32)(b - a) == n
                && memcmp(fons + a, litterae, (size_t)n) == ZEPHYRUM);
}

/* titulus [a, b) sine '#' -> clavis; '#data' aliunde tractatur */
interior i32
_clavis (
    constans character* fons,
                   s32  a,
                   s32  b)
{
    si (_titulus_est(fons, a, b, "errors"))
    {
        redde SECTIO_ERRORES;
    }
    si (_titulus_est(fons, a, b, "document-fragment"))
    {
        redde SECTIO_CONTEXTUS;
    }
    si (_titulus_est(fons, a, b, "script-on"))
    {
        redde SECTIO_SCRIPTUM_APERTUM;
    }
    si (_titulus_est(fons, a, b, "script-off"))
    {
        redde SECTIO_SCRIPTUM_CLAUSUM;
    }
    si (_titulus_est(fons, a, b, "document"))
    {
        redde SECTIO_DOCUMENTUM;
    }
    redde SECTIO_IGNORATA;   /* new-errors et ignota */
}

/* Octeti [a, b) in piscinam copiati; '\n' caudalia usque ad 'demptio'
 * dempta (normaliseOutput = I; ante '#data' proximum = II). datum
 * NIHIL = memoria deficit (sectio vacua datum non-NIHIL fert). */
interior chorda
_copiare (
               Piscina* piscina,
    constans character* fons,
                   s32  a,
                   s32  b,
                   i32  demptio)
{
        chorda  c;
     character* d;
           i32  n;

    dum (demptio > ZEPHYRUM && b > a && fons[b - I] == '\n')
    {
        b        = b - I;
        demptio  = demptio - I;
    }
    n = (i32)(b - a);
    d = (character*)piscina_allocare(piscina, (memoriae_index)n + I);
    si (d == NIHIL)
    {
        c.datum    = NIHIL;
        c.mensura  = ZEPHYRUM;
        redde c;
    }
    memcpy(d, fons + a, (size_t)n);
    d[n]       = '\0';
    c.datum    = (i8*)d;
    c.mensura  = n;
    redde c;
}

/* Sectionem currentem in exemplum scribere. FALSUM = memoria deficit;
 * sine exemplo aut clavi ignorata nihil agit (VERUM). */
interior b32
_claudere (
                   Piscina* piscina,
        constans character* fons,
    constans SectioCurrens* s,
              HtmlExemplum* e,
                       i32  demptio)
{
    chorda* campus;

    si (e == NIHIL)
    {
        redde VERUM;
    }
    commutatio (s->clavis)
    {
        casus SECTIO_DATUM:
            campus = &e->datum;
            frange;
        casus SECTIO_ERRORES:
            campus = &e->errores;
            frange;
        casus SECTIO_CONTEXTUS:
            e->fragmentum = VERUM;
            campus        = &e->contextus;
            frange;
        casus SECTIO_DOCUMENTUM:
            campus = &e->documentum;
            frange;
        casus SECTIO_SCRIPTUM_APERTUM:
            e->scriptum = HTML_EXEMPLUM_SCRIPTUM_APERTUM;
            redde VERUM;
        casus SECTIO_SCRIPTUM_CLAUSUM:
            e->scriptum = HTML_EXEMPLUM_SCRIPTUM_CLAUSUM;
            redde VERUM;
        ordinarius:
            redde VERUM;
    }
    *campus = _copiare(piscina, fons, s->initium, s->finis, demptio);
    redde (b32)(campus->datum != NIHIL);
}

Xar*
html_exempla_legere (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura)
{
              Xar* exitus;
     HtmlExemplum* e        = NIHIL;
    SectioCurrens  s;
              s32  i        = ZEPHYRUM;
              i32  linea    = ZEPHYRUM;
              i32  numerus  = ZEPHYRUM;

    exitus = xar_creare(piscina, (i32)magnitudo(HtmlExemplum));
    si (exitus == NIHIL)
    {
        redde NIHIL;
    }
    s.clavis   = SECTIO_NULLA;
    s.initium  = ZEPHYRUM;
    s.finis    = ZEPHYRUM;
    dum (i < (s32)mensura)
    {
        s32 a = i;
        s32 b;
        s32 post;
        s32 primus;
        s32 ultimus;

        dum (i < (s32)mensura && fons[i] != '\n')
        {
            i = i + I;
        }
        b       = i;
        post    = (i < (s32)mensura) ? i + I : i;
        linea   = linea + I;
        primus  = a;
        dum (primus < b && _spatium_est(fons[primus]))
        {
            primus = primus + I;
        }
        ultimus = b;
        dum (ultimus > primus && _spatium_est(fons[ultimus - I]))
        {
            ultimus = ultimus - I;
        }
        si (primus < ultimus && fons[primus] == '#')
        {
            b32 novum = _titulus_est(fons, primus + I, ultimus, "data");

            si (!_claudere(piscina, fons, &s, e, novum ? II : I))
            {
                redde NIHIL;
            }
            si (novum)
            {
                e = (HtmlExemplum*)xar_addere(exitus);
                si (e == NIHIL)
                {
                    redde NIHIL;
                }
                memset(e, ZEPHYRUM, magnitudo(*e));
                numerus      = numerus + I;
                e->numerus   = numerus;
                e->linea     = linea;
                e->scriptum  = HTML_EXEMPLUM_SCRIPTUM_INCERTUM;
                s.clavis     = SECTIO_DATUM;
            }
            alioquin
            {
                s.clavis = _clavis(fons, primus + I, ultimus);
            }
            s.initium  = post;
            s.finis    = post;
        }
        alioquin si (s.clavis != SECTIO_NULLA)
        {
            s.finis = post;
        }
        i = post;
    }
    si (!_claudere(piscina, fons, &s, e, I))
    {
        redde NIHIL;
    }
    redde exitus;
}

vacuum
html_exempla_contextus (
        Piscina* piscina,
         chorda  contextus,
    HtmlAlienum* spatium,
         chorda* titulus)
{
    HtmlAlienum s = HTML_ALIENUM_NULLUM;
         chorda t = contextus;
            i32 i;

    per (i = ZEPHYRUM; i < contextus.mensura; i++)
    {
        si (contextus.datum[i] == ' ')
        {
            chorda primum;

            primum.datum    = contextus.datum;
            primum.mensura  = i;
            si (chorda_aequalis_literis(primum, "svg"))
            {
                s = HTML_ALIENUM_SVG;
            }
            alioquin si (chorda_aequalis_literis(primum, "math"))
            {
                s = HTML_ALIENUM_MATHEMATICA;
            }
            t.datum    = contextus.datum + i + I;
            t.mensura  = contextus.mensura - i - I;
            frange;
        }
    }
    *spatium = s;
    *titulus = html_alienum_titulus(piscina, t, s);
}
