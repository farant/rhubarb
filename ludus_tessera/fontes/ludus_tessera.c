/* ludus_tessera.c - glutinum tesserae (A3). Ratio in capite. */

#include "ludus_tessera.h"
#include "ludus_tessera_pons.h"
#include "ludus_tessera_demissio.h"
#include "fenestra_tempus.h"
#include "terminalis.h"
#include "rivus_terminalis.h"
#include "thema.h"
#include "color.h"

#include <stdio.h>
#include <string.h>

/* S1c: exspectatio maxima cum applicatione viva */
#define MORA_PULSUS  XVI

/* Octeti per lectionem terminalis (ut auscultator_terminalis) */
#define LECTIO  CCLVI

interior vacuum
_modulum_renovare (
    LudusTessera* lt)
{
    s32 cw = lt->modulus.cellula_latitudo;
    s32 ch = lt->modulus.cellula_altitudo;

    lt->modulus = modulus_creare(cw, ch,
        (s32)tessera_latitudo(lt->opus) * cw,
        (s32)tessera_altitudo(lt->opus) * ch);
}

/* Magnitudo initialis semel (013 B1): superficies status est,
 * dispensator eam in ephemera scribit. Vexillum ANTE traditionem
 * ponitur - tractare hanc functionem iterum vocat. */
interior vacuum
_magnitudinem_nuntiare (
    LudusTessera* lt,
             s64  nunc)
{
    Eventus e;

    si (lt->magnitudo_nuntiata)
    {
        redde;
    }
    lt->magnitudo_nuntiata = VERUM;
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus   = EVENTUS_MUTARE_MAGNITUDINEM;
    e.tempus  = nunc;
    e.datum.mutare_magnitudinem.latitudo =
        (i32)lt->modulus.extensio_latitudo;
    e.datum.mutare_magnitudinem.altitudo =
        (i32)lt->modulus.extensio_altitudo;
    ludus_tessera_tractare(lt, &e, nunc);
}

LudusTessera*
ludus_tessera_creare (
            Piscina* piscina,
        Dispensator* d,
    FiguraRegistrum* figurae,
                i32  thema,
          ImagoFons  fons,
             vacuum* fons_ctx,
        TesseraOpus* opus,
                s32  cellula_latitudo,
                s32  cellula_altitudo)
{
    LudusTessera* lt;

    si (!piscina || !d || !figurae || !opus)
    {
        redde NIHIL;
    }
    lt = (LudusTessera*)piscina_allocare(piscina,
        magnitudo(LudusTessera));
    si (!lt)
    {
        redde NIHIL;
    }
    memset(lt, ZEPHYRUM, magnitudo(LudusTessera));
    lt->d                       = d;
    lt->figurae                 = figurae;
    lt->thema                   = thema;
    lt->fons                    = fons;
    lt->fons_ctx                = fons_ctx;
    lt->opus                    = opus;
    lt->piscina                 = piscina;
    lt->claudendi_runa          = (s32)'c';
    lt->claudendi_modificantes  = MOD_IMPERIUM;
    lt->modulus = modulus_creare(cellula_latitudo, cellula_altitudo, I,
        I);
    _modulum_renovare(lt);
    lt->piscina_quadri = piscina_generare_dynamicum(
        "ludus_tessera_quadrum", LXIV * M);
    si (!lt->piscina_quadri)
    {
        redde NIHIL;
    }
    redde lt;
}

b32
ludus_tessera_claudendum_est (
    constans LudusTessera* lt,
         constans Eventus* ev)
{
    si (!lt || !ev || ev->genus != EVENTUS_CLAVIS_DEPRESSUS)
    {
        redde FALSUM;
    }
    redde (ev->datum.clavis.runa == lt->claudendi_runa
        && (ev->datum.clavis.modificantes & lt->claudendi_modificantes))
        ? VERUM : FALSUM;
}

vacuum
ludus_tessera_tractare (
        LudusTessera* lt,
    constans Eventus* ev,
                 s64  nunc)
{
    Eventus e;

    si (!lt || !ev)
    {
        redde;
    }
    _magnitudinem_nuntiare(lt, nunc);
    e = *ev;
    si (e.tempus == ZEPHYRUM)
    {
        e.tempus = nunc;
    }
    si (e.genus == EVENTUS_MUTARE_MAGNITUDINEM)
    {
        /* opus amplitudinem a ponte relegit (pictura plena sequitur) */
        (vacuum)tessera_magnitudinem_renovare(lt->opus);
        _modulum_renovare(lt);
    }
    alioquin si (e.genus == EVENTUS_RESUMPTIO)
    {
        /* terminalis post SIGCONT modos iam reintravit: pictura
         * plena */
        tessera_resumere(lt->opus);
    }
    dispensator_tractare(lt->d, &e);
}

/* summa versionum repositorii (durabilis + ephemera) */
interior i32
versionem_summare (
    constans LudusTessera* lt)
{
    redde insula_versio(lt->d->repo, INSULA_DURABILIS)
        + insula_versio(lt->d->repo, INSULA_EPHEMERA);
}

vacuum
ludus_tessera_pulsum_ponere (
            LudusTessera* lt,
    LudusTesseraPulsator  fn,
                  vacuum* ctx)
{
    si (!lt)
    {
        redde;
    }
    lt->pulsator      = fn;
    lt->pulsator_ctx  = fn ? ctx : NIHIL;
}

b32
ludus_tessera_pingendum (
    LudusTessera* lt,
             b32  eventa,
             s64  nunc)
{
    b32 mutatum;

    si (!lt || !lt->pulsator)
    {
        redde VERUM;
    }
    mutatum = lt->pulsator(lt->pulsator_ctx);
    si (eventa || mutatum || lt->mensurae.quadra == ZEPHYRUM)
    {
        redde VERUM;
    }
    /* ut ludus_fenestra_pingendum: dispensator solus, signum =
     * versio repositorii contra quadrum ultimum */
    dispensator_pulsare(lt->d, nunc);
    redde versionem_summare(lt) != lt->versio_picta;
}

vacuum
ludus_tessera_quadrum (
    LudusTessera* lt,
             s64  nunc)
{
    Color fundus;
      s64 t0;
      s64 t1;
      s64 t2;
      s64 t3;

    si (!lt)
    {
        redde;
    }
    _magnitudinem_nuntiare(lt, nunc);
    t0 = fenestra_tempus_ms();
    dispensator_pulsare(lt->d, nunc);
    t1 = fenestra_tempus_ms();
    piscina_vacare(lt->piscina_quadri);
    lt->mandata = mandata_creare(lt->piscina_quadri, lt->d->intern);
    pingere(dispensator_arbor(lt->d), lt->figurae, lt->thema,
        lt->mandata);
    t2      = fenestra_tempus_ms();
    fundus  = thema_color(COLOR_BACKGROUND);
    (vacuum)ludus_tessera_demittere(lt->opus, lt->mandata, &lt->modulus,
        ((i32)fundus.r << XVI) | ((i32)fundus.g << VIII)
            | (i32)fundus.b,
        lt->fons, lt->fons_ctx, lt->piscina_quadri);
    t3 = fenestra_tempus_ms();
    lt->mensurae.quadra++;
    lt->versio_picta               = versionem_summare(lt);
    lt->mensurae.ms_compositionis  += t1 - t0;
    lt->mensurae.ms_pingendi       += t2 - t1;
    lt->mensurae.ms_demittendi     += t3 - t2;
    si (t3 - t0 > lt->mensurae.ms_quadri_maximum)
    {
        lt->mensurae.ms_quadri_maximum = t3 - t0;
    }
}

s32
ludus_tessera_currere (
    LudusTessera* lt,
             i32  quadra_maxima)
{
         RivusTerminalis* rivus;
     TerminalisAmplitudo  amplitudo;
                 Eventus  e;
                      i8  intrandi[LUDUS_TESSERA_MODI_MAXIMI];
                      i8  exeundi[LUDUS_TESSERA_MODI_MAXIMI];
                      i8  buffer[LECTIO];
                     i32  n_intrandi;
                     i32  n_exeundi;
                     s64  nunc;
                     b32  currens;

    si (!lt || !terminalis_adest())
    {
        redde I;
    }
    rivus = rivus_creare(lt->piscina, lt->modulus.cellula_latitudo,
        lt->modulus.cellula_altitudo);
    si (   !rivus
        || !ludus_tessera_modos_componere(rivus,
               RIVUS_MODUS_MUS | RIVUS_MODUS_GLUTINUM, intrandi,
               &n_intrandi, exeundi, &n_exeundi)
        || !terminalis_intrare(intrandi, n_intrandi, exeundi,
        n_exeundi))
    {
        redde I;
    }
    (vacuum)tessera_magnitudinem_renovare(lt->opus);
    _modulum_renovare(lt);

    currens = VERUM;
    dum (currens)
    {
        s32 mora;
        s32 capax;
        s32 lecti;
        b32 eventa;

        nunc    = fenestra_tempus_ms();
        eventa  = FALSUM;
        dum (rivus_eventum_coalitum(rivus, nunc, &e))
        {
            si (ludus_tessera_claudendum_est(lt, &e))
            {
                currens = FALSUM;
                frange;
            }
            ludus_tessera_tractare(lt, &e, nunc);
            eventa = VERUM;
        }
        si (!currens)
        {
            frange;
        }
        si (terminalis_resumptum())
        {
            memset(&e, ZEPHYRUM, magnitudo(Eventus));
            e.genus = EVENTUS_RESUMPTIO;
            ludus_tessera_tractare(lt, &e, nunc);
        }
        si (   terminalis_amplitudo_mutata()
            && terminalis_amplitudo(&amplitudo))
        {
            memset(&e, ZEPHYRUM, magnitudo(Eventus));
            e.genus = EVENTUS_MUTARE_MAGNITUDINEM;
            e.datum.mutare_magnitudinem.latitudo =
                (i32)(amplitudo.columnae
                * lt->modulus.cellula_latitudo);
            e.datum.mutare_magnitudinem.altitudo =
                (i32)(amplitudo.lineae
                * lt->modulus.cellula_altitudo);
            ludus_tessera_tractare(lt, &e, nunc);
        }
        /* S1c: applicatio viva - quadrum solum si pingendum; fumus
         * (quadra_maxima) omne quadrum pingit - aliter applicatio viva
         * otiosa post quadrum primum numquam exit */
        si (   quadra_maxima > ZEPHYRUM
            || ludus_tessera_pingendum(lt, eventa, nunc))
        {
            ludus_tessera_quadrum(lt, nunc);
            (vacuum)tessera_praesentare(lt->opus);
            lt->mensurae.octeti_emissi =
                (i32)lt->opus->fructus.octeti_emissi;
        }
        si (   quadra_maxima > ZEPHYRUM
            && lt->mensurae.quadra >= (i32)quadra_maxima)
        {
            frange;
        }

        /* mora: series pendens (rivus) aut sedes quietis (v1: quies_ms
         * semper - quadrum otiosum differentiam nullam emittit) */
        mora  = rivus_mora_ms(rivus);
        si (mora <= ZEPHYRUM)
        {
            mora = (s32)lt->d->quies_ms;
        }
        si (lt->pulsator && mora > MORA_PULSUS)
        {
            mora = MORA_PULSUS;   /* S1c: applicatio viva */
        }
        capax = (s32)rivus_spatium(rivus);
        lecti = terminalis_legere(buffer,
            (i32)((capax < LECTIO) ? capax : LECTIO), mora);
        si (lecti > ZEPHYRUM)
        {
            (vacuum)rivus_tradere(rivus, buffer, (i32)lecti);
        }
        alioquin si (lecti == ZEPHYRUM)
        {
            si (rivus_mora_ms(rivus) > ZEPHYRUM)
            {
                rivus_moram(rivus, fenestra_tempus_ms());
            }
        }
        alioquin
        {
            currens = FALSUM;   /* terminalis abiit */
        }
    }

    /* S1a: pendentia (gestus, pan/zoom) ante exitum effunduntur */
    dispensator_finire(lt->d);
    tessera_claudere(lt->opus);
    (vacuum)terminalis_exire();
    imprimere("ludus_tessera: quadra=%d compositio=%ldms pingere=%ldms"
              " demittere=%ldms maximum=%ldms octeti=%d\n",
              (int)lt->mensurae.quadra,
              (long)lt->mensurae.ms_compositionis,
              (long)lt->mensurae.ms_pingendi,
              (long)lt->mensurae.ms_demittendi,
              (long)lt->mensurae.ms_quadri_maximum,
              (int)lt->mensurae.octeti_emissi);
    redde ZEPHYRUM;
}
