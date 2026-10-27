/* testa_cursus.c - cursus communis testarum (testa-spec II.1, II.3) */

#include "testa_cursus.h"

#include <stdio.h>
#include <string.h>

vacuum
testa_optiones_legere (
              s32   argc,
        character** argv,
    TestaOptiones*  o)
{
    s32 k;

    o->via_imaginis  = NIHIL;
    o->fumus         = FALSUM;
    per (k = I; k < argc; k++)
    {
        si (strcmp(argv[k], "-fumus") == ZEPHYRUM)
        {
            o->fumus = VERUM;
        }
        alioquin si (   strcmp(argv[k], "-imago") == ZEPHYRUM
                     && k + I < argc)
        {
            o->via_imaginis = argv[k + I];
        }
    }
}

constans character*
testa_titulus_processus (
    constans character* programma)
{
    constans character* basis;
    constans character* p;

    si (!programma)
    {
        redde "applicatio";
    }
    basis = programma;
    per (p = programma; *p != '\0'; p++)
    {
        si (*p == '/')
        {
            basis = p + I;
        }
    }
    redde *basis != '\0' ? basis : "applicatio";
}

b32
testa_formam_probare (
     constans TestaForma*  f,
      constans character** causa)
{
    si (!f->titulus)
    {
        *causa = "forma: titulus NIHIL";
        redde FALSUM;
    }
    si (f->scala < I)
    {
        *causa = "forma: scala < I";
        redde FALSUM;
    }
    si (f->latitudo < I || f->altitudo < I)
    {
        *causa = "forma: magnitudo non positiva";
        redde FALSUM;
    }
    si (!f->d)
    {
        *causa = "forma: dispensator NIHIL";
        redde FALSUM;
    }
    si (!f->figurae)
    {
        *causa = "forma: figurae NIHIL";
        redde FALSUM;
    }
    redde VERUM;
}

b32
testa_cursum_incipere (
              TestaCursus*  c,
    constans TestaAmbitus*  a,
             TestaInitium   initium,
       constans character** causa)
{
    memset(c, ZEPHYRUM, magnitudo(TestaCursus));
    si (!initium(a, &c->forma))
    {
        *causa = "applicatio_initium FALSUM reddidit";
        redde FALSUM;
    }
    c->initiatus = VERUM;
    redde testa_formam_probare(&c->forma, causa);
}

b32
testa_cursum_ludum_struere (
        TestaCursus* c,
            Piscina* piscina,
    TabulaPixelorum* tabula)
{
    si (!c->initiatus || c->lf)
    {
        redde FALSUM;
    }
    c->lf = ludus_fenestra_creare(piscina, c->forma.d, c->forma.figurae,
        c->forma.thema, c->forma.fons, c->forma.fons_ctx, tabula);
    si (!c->lf)
    {
        redde FALSUM;
    }
    ludus_fenestra_pulsum_ponere(c->lf, c->forma.pulsator,
        c->forma.pulsator_ctx);
#if !defined(__ANDROID__) \
    && !defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__)
    /* vicus quies T6: macOS solum - testae iOS/Android moram suam
     * ducunt (ludus_fenestra_mora -1 non intellegunt) */
    si (c->forma.mora_proxima)
    {
        ludus_fenestra_quietem_ponere(c->lf, c->forma.mora_proxima,
            c->forma.mora_ctx, c->forma.reactor);
    }
#endif
    redde VERUM;
}

vacuum
testa_cursum_finire (
    TestaCursus* c)
{
    LudusFenestra* lf;

    lf = c->lf;
    si (!lf)
    {
        redde;
    }
    /* S1a: pendentia (gestus, pan/zoom) ante exitum effunduntur */
    dispensator_finire(lf->d);
    imprimere("ludus: quadra=%d compositio=%ldms pingere=%ldms"
              " delineare=%ldms maximum=%ldms\n",
              (int)lf->mensurae.quadra,
              (long)lf->mensurae.ms_compositionis,
              (long)lf->mensurae.ms_pingendi,
              (long)lf->mensurae.ms_delineandi,
              (long)lf->mensurae.ms_quadri_maximum);
}

vacuum
testa_cursum_dimittere (
    TestaCursus* c)
{
    si (!c->initiatus || c->finitus)
    {
        redde;
    }
    c->finitus = VERUM;
    si (c->forma.finis)
    {
        c->forma.finis(c->forma.finis_ctx);
    }
}
