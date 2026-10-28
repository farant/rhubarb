/* ludus_fenestra.c - glutinum fenestrae */

#include "ludus_fenestra.h"
#include "imago_png.h"
#include "thema.h"
#include "color.h"

#include <stdio.h>
#include <string.h>

/* versio repositorii (insula una, C2) */
interior i32
versionem_summare (
    constans LudusFenestra* lf)
{
    redde insula_versio(lf->d->repo);
}

LudusFenestra*
ludus_fenestra_creare (
            Piscina* piscina,
        Dispensator* d,
    FiguraRegistrum* figurae,
                i32  thema,
          ImagoFons  fons,
             vacuum* fons_ctx,
    TabulaPixelorum* tabula)
{
    LudusFenestra* lf;

    si (!piscina || !d || !figurae || !tabula)
    {
        redde NIHIL;
    }
    lf = (LudusFenestra*)piscina_allocare(piscina,
        magnitudo(LudusFenestra));
    si (!lf)
    {
        redde NIHIL;
    }
    memset(lf, ZEPHYRUM, magnitudo(LudusFenestra));
    lf->d         = d;
    lf->figurae   = figurae;
    lf->thema     = thema;
    lf->fons      = fons;
    lf->fons_ctx  = fons_ctx;
    lf->tabula    = tabula;
    lf->piscina   = piscina;
    lf->piscina_quadri = piscina_generare_dynamicum("ludus_quadrum",
                                                    LXIV * M);
    si (!lf->piscina_quadri)
    {
        redde NIHIL;
    }
    redde lf;
}

/* Tempus stampare et tradere (sine conversione) */
interior vacuum
_tradere (
    LudusFenestra* lf,
          Eventus* e,
              s64  nunc)
{
    si (e->tempus == ZEPHYRUM)
    {
        e->tempus = nunc;
    }
    dispensator_tractare(lf->d, e);
}

/* Magnitudo initialis semel (013 B1), IAM in pixelis nostris
 * (tabulae) - directe traditur, sine conversione B3b. */
interior vacuum
_magnitudinem_nuntiare (
    LudusFenestra* lf,
              s64  nunc)
{
    Eventus e;

    si (lf->magnitudo_nuntiata)
    {
        redde;
    }
    lf->magnitudo_nuntiata = VERUM;
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                               = EVENTUS_MUTARE_MAGNITUDINEM;
    e.tempus                              = nunc;
    e.datum.mutare_magnitudinem.latitudo  = lf->tabula->latitudo;
    e.datum.mutare_magnitudinem.altitudo  = lf->tabula->altitudo;
    _tradere(lf, &e, nunc);
}

vacuum
ludus_fenestra_tractare (
       LudusFenestra* lf,
    constans Eventus* ev,
                 s64  nunc)
{
    Eventus e;

    si (!lf || !ev)
    {
        redde;
    }
    _magnitudinem_nuntiare(lf, nunc);
    e = *ev;
    si (e.genus == EVENTUS_MUTARE_MAGNITUDINEM && lf->piscina)
    {
        /* 013 B3b: puncta fenestrae -> tabula aptata (scala servata)
         * -> eventus in pixela nostra: superficies idem significat in
         * fenestra ac in terminali */
        (vacuum)tabula_pixelorum_ad_fenestram(lf->tabula, lf->piscina,
            e.datum.mutare_magnitudinem.latitudo,
            e.datum.mutare_magnitudinem.altitudo);
        e.datum.mutare_magnitudinem.latitudo = lf->tabula->latitudo;
        e.datum.mutare_magnitudinem.altitudo = lf->tabula->altitudo;
    }
    _tradere(lf, &e, nunc);
}

vacuum
ludus_quadrum (
    LudusFenestra* lf,
              s64  nunc)
{
    s64 t0;
    s64 t1;
    s64 t2;
    s64 t3;

    si (!lf)
    {
        redde;
    }
    _magnitudinem_nuntiare(lf, nunc);
    t0 = fenestra_tempus_ms();
    dispensator_pulsare(lf->d, nunc);
    t1 = fenestra_tempus_ms();
    piscina_vacare(lf->piscina_quadri);
    lf->mandata = mandata_creare(lf->piscina_quadri, lf->d->intern);
    pingere(dispensator_arbor(lf->d), lf->figurae, lf->thema,
        lf->mandata);
    t2 = fenestra_tempus_ms();
    tabula_pixelorum_vacare(lf->tabula,
        color_ad_pixelum(thema_color(COLOR_BACKGROUND)));
    delineare_mandata(lf->mandata, lf->tabula, lf->fons, lf->fons_ctx);
    t3 = fenestra_tempus_ms();
    lf->mensurae.quadra++;
    lf->versio_picta               = versionem_summare(lf);
    lf->mensurae.ms_compositionis  += t1 - t0;
    lf->mensurae.ms_pingendi       += t2 - t1;
    lf->mensurae.ms_delineandi     += t3 - t2;
    si (t3 - t0 > lf->mensurae.ms_quadri_maximum)
    {
        lf->mensurae.ms_quadri_maximum = t3 - t0;
    }
}

vacuum
ludus_fenestra_pulsum_ponere (
    LudusFenestra* lf,
    LudusPulsator  fn,
           vacuum* ctx)
{
    si (!lf)
    {
        redde;
    }
    lf->pulsator      = fn;
    lf->pulsator_ctx  = fn ? ctx : NIHIL;
}

b32
ludus_fenestra_pingendum (
    LudusFenestra* lf,
              b32  eventa,
              s64  nunc)
{
    b32 mutatum;

    si (!lf || !lf->pulsator)
    {
        redde VERUM;
    }
    lf->mensurae.pulsus++;
    mutatum = lf->pulsator(lf->pulsator_ctx);
    si (eventa || mutatum || lf->mensurae.quadra == ZEPHYRUM)
    {
        redde VERUM;
    }
    /* nihil novi: dispensator SOLUS pulsatur (horologia, e.g.
     * scriptura differta). Signum = VERSIO repositorii contra quadrum
     * ultimum (compositiones non: dispensator omni eventu recomponit,
     * 'regula staleness') */
    dispensator_pulsare(lf->d, nunc);
    redde versionem_summare(lf) != lf->versio_picta;
}

vacuum
ludus_fenestra_quietem_ponere (
       LudusFenestra* lf,
    LudusMoraProxima  mora,
              vacuum* ctx,
             Reactor* reactor)
{
    si (!lf)
    {
        redde;
    }
    lf->quies           = VERUM;
    lf->mora_proxima    = mora;
    lf->mora_ctx        = ctx;
    lf->reactor         = reactor;
    lf->fossae_positae  = FALSUM;   /* reactor alius: renovandae */
}

/* minimum duarum morarum; -1 = nulla */
interior Mora
_moram_minuere (
    Mora a,
    Mora b);

vacuum
ludus_fenestra_reactorem_ponere (
    LudusFenestra* lf,
          Reactor* reactor)
{
    si (lf && !lf->quies)
    {
        lf->reactor         = reactor;
        lf->fossae_positae  = FALSUM;
    }
}

interior Mora
_moram_minuere (
    Mora a,
    Mora b)
{
    si (a < ZEPHYRUM)
    {
        redde b;
    }
    si (b < ZEPHYRUM)
    {
        redde a;
    }
    redde a < b ? a : b;
}

Mora
ludus_fenestra_mora (
    constans LudusFenestra* lf)
{
    Mora mora;

    si (!lf || lf->mensurae.quadra == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    si (!lf->quies)
    {
        /* plan-5 T5: reactor sine quiete - hora eius proxima */
        redde lf->reactor ? _moram_minuere((Mora)lf->d->quies_ms,
            (Mora)reactor_mora_proxima(lf->reactor))
            : (Mora)lf->d->quies_ms;
    }
    /* vicus quies T5: minimum debitorum, -1 si nihil */
    mora = (Mora)dispensator_mora_proxima(lf->d, fenestra_tempus_ms());
    si (lf->mora_proxima)
    {
        mora = _moram_minuere(mora, lf->mora_proxima(lf->mora_ctx));
    }
    si (lf->reactor)
    {
        mora = _moram_minuere(mora,
            (Mora)reactor_mora_proxima(lf->reactor));
    }
    redde mora;
}

/* Ansa fenestrae perscrutabilis (macOS): API publica - programmata
 * briar (.thistle) eam vocant, testa macOS (lib/testa.c) quoque. In
 * Android nulla fenestra perscrutabilis (testa_android quadra per
 * chorographum ducit), in iOS nec (testa_ios per CADisplayLink;
 * testa-ios R1): non compilatur. Restituta 2026-10-23 (fusio:
 * briar-fumus et instrumenta fracta post T1). */
#if !defined(__ANDROID__) \
    && !defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__)

/* S1c: exspectatio maxima cum applicatione viva (ut terminale) */
#define MORA_PULSUS  XVI

/* vicus quies T5: nihil debitum - hora (eventus aut fossa citius) */
#define MORA_SINE_FINE  (LX * LX * M)

nomen structura {
    FenestraFossa* fossae;
              i32  numerus;
              i32  capacitas;
} FossaeLudi;

interior vacuum
_fossam_addere (
     vacuum* ctx,
    integer  fd,
        i32  eventus)
{
       FossaeLudi* fl = (FossaeLudi*)ctx;
    FenestraFossa* f;

    fl->numerus++;
    si (fl->numerus > fl->capacitas)
    {
        redde;
    }
    f           = &fl->fossae[fl->numerus - I];
    f->fd       = fd;
    f->eventus  = ZEPHYRUM;
    si (eventus & REACTOR_LEGERE)
    {
        f->eventus |= FENESTRA_FOSSA_LEGERE;
    }
    si (eventus & REACTOR_SCRIBERE)
    {
        f->eventus |= FENESTRA_FOSSA_SCRIBERE;
    }
}

/* fossae reactoris fenestrae traduntur solum cum versio mutatur.
 * FALSUM = traditio fracta (plures quam CXXVIII, systema recusat):
 * vocator ad MORA_PULSUS redit, ne fossa ignota dormiat */
interior b32
_fossas_renovare (
    LudusFenestra* lf)
{
    FenestraFossa fossae[CXXVIII];
       FossaeLudi fl;
              i32 versio;

    si (!lf->reactor)
    {
        si (!lf->fossae_positae)
        {
            lf->fossae_positae = fenestra_fossas_ponere(NIHIL,
                ZEPHYRUM);
        }
        redde lf->fossae_positae;
    }
    versio = reactor_versio_fossarum(lf->reactor);
    si (lf->fossae_positae && versio == lf->fossae_versio)
    {
        redde VERUM;
    }
    fl.fossae     = fossae;
    fl.numerus    = ZEPHYRUM;
    fl.capacitas  = CXXVIII;
    reactor_fossas_visitare(lf->reactor, _fossam_addere, &fl);
    lf->fossae_versio   = versio;
    lf->fossae_positae  = fl.numerus <= fl.capacitas
                       && fenestra_fossas_ponere(fossae, fl.numerus);
    redde lf->fossae_positae;
}

s32
ludus_fenestra_currere (
    LudusFenestra* lf,
         Fenestra* fenestra,
              i32  quadra_maxima)
{
    Eventus e;
        s64 nunc;
        b32 claudendum;

    si (!lf || !fenestra)
    {
        redde I;
    }
    claudendum = FALSUM;
    dum (!claudendum && !fenestra_debet_claudere(fenestra))
    {
        Mora mora;
         b32 eventa;

        mora = ludus_fenestra_mora(lf);
        si (lf->quies)
        {
            /* vicus quies T5: usque ad debitum proximum; fossae
             * reactoris expergefaciunt (traditio fracta: XVI ms) */
            si (!_fossas_renovare(lf))
            {
                mora = _moram_minuere(mora, MORA_PULSUS);
            }
            alioquin si (mora < ZEPHYRUM)
            {
                mora = MORA_SINE_FINE;
            }
        }
        /* S1c: applicatio viva (pulsus) - XVI ms ad summum */
        alioquin si (   lf->pulsator
                     && (mora == ZEPHYRUM || mora > MORA_PULSUS))
        {
            mora = MORA_PULSUS;
        }
        fenestra_expectare_eventus(fenestra, mora);
        lf->mensurae.excitationes++;
        /* plan-5 T5: reactor (communio) omni excitatione agitur */
        si (lf->reactor)
        {
            (vacuum)reactor_poll(lf->reactor, ZEPHYRUM);
        }
        nunc    = fenestra_tempus_ms();
        eventa  = FALSUM;
        dum (fenestra_obtinere_eventus(fenestra, &e))
        {
            si (e.genus == EVENTUS_CLAUDERE)
            {
                claudendum = VERUM;
                frange;
            }
            ludus_fenestra_tractare(lf, &e, nunc);
            eventa = VERUM;
        }
        /* fumus (quadra_maxima): omne quadrum pingitur - aliter
         * applicatio viva otiosa post quadrum primum numquam exit */
        si (   quadra_maxima == ZEPHYRUM
            && !ludus_fenestra_pingendum(lf, eventa, nunc))
        {
            perge;
        }
        ludus_quadrum(lf, nunc);
        fenestra_praesentare_pixela(fenestra, lf->tabula);
        si (   quadra_maxima > ZEPHYRUM
            && lf->mensurae.quadra >= quadra_maxima)
        {
            claudendum = VERUM;
        }
    }
    /* S1a: pendentia (gestus, pan/zoom) ante exitum effunduntur */
    dispensator_finire(lf->d);
    si (lf->quies)
    {
        (vacuum)fenestra_fossas_ponere(NIHIL, ZEPHYRUM);
    }
    imprimere("ludus: quadra=%d compositio=%ldms pingere=%ldms"
              " delineare=%ldms maximum=%ldms excitationes=%ld"
              " pulsus=%ld\n",
              (int)lf->mensurae.quadra,
              (long)lf->mensurae.ms_compositionis,
              (long)lf->mensurae.ms_pingendi,
              (long)lf->mensurae.ms_delineandi,
              (long)lf->mensurae.ms_quadri_maximum,
              (long)lf->mensurae.excitationes,
              (long)lf->mensurae.pulsus);
    redde ZEPHYRUM;
}


#endif /* nec Android nec iOS */

b32
ludus_fenestra_imaginem_scribere (
    constans LudusFenestra* lf,
        constans character* via)
{
         Imago imago;
    PngFructus fructus;

    si (!lf || !via || lf->mensurae.quadra == ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (!tabula_pixelorum_in_imaginem(lf->tabula, lf->piscina_quadri,
            &imago))
    {
        redde FALSUM;
    }
    fructus = imago_png_scribere(&imago, via, lf->piscina_quadri);
    redde fructus.successus;
}
