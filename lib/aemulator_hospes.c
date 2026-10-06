/* aemulator_hospes.c - hospes emulatoris (aemulator-plan B4,
 * decisio XVII)
 *
 * Cauda circularis ad infantem: initus vocantis usque ad capacitatem
 * minus parte decima sexta (reservatum), responsa nuclei usque ad
 * capacitatem totam - pasta magna quaestionem numquam esurire facit.
 * Responsum quod ne reservato quidem capitur perit et numeratur.
 *
 * Effectus nuclei: datum = hospes; responsum in caudam, campana et
 * titulus ad effectus vocantis (cum dato vocantis) transeunt.
 *
 * PURUS: solum <string.h>; infans per tabulam Pseudoterminale.
 */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "aemulator.h"
#include "pseudoterminale.h"
#include "aemulator_hospes.h"
#include <string.h>

/* lectio una: sacculus; pulsus plures lectiones facit usque ad
 * octeti_per_pulsum */
#define SACCULUS_LECTIONIS (IV * MXXIV)

structura AemulatorHospes {
                Aemulator* a;
          Pseudoterminale* pt;
        AemulatorEffectus  vocantis;   /* campana, titulus vocantis */
                       i8* cauda;
                      i32  capacitas;
                      i32  reservatum;
                      i32  caput;      /* octetus primus mittendus */
                      i32  mensura;    /* octeti in cauda */
                       i8* sacculus;
                      i32  per_pulsum;
                      b32  finis_lectus;
                      b32  messus;
                      b32  amplitudo_mutata;
                      b32  clausum;
                      i32  responsa_amissa;
    PseudoterminaleExitus  exitus;
};

/* in caudam ponere usque ad 'limes' octetos in ea; reddit positos */
interior i32
in_caudam (
    AemulatorHospes* h,
        constans i8* octeti,
                i32  n,
                i32  limes)
{
    i32 locus;
    i32 k;

    /* i32 insignatus: limes - mensura sub zephyro non cadat
     * (responsa caudam ultra limitem initus implere possunt) */
    si (n == ZEPHYRUM || h->mensura >= limes)
    {
        redde ZEPHYRUM;
    }
    si (n > limes - h->mensura)
    {
        n = limes - h->mensura;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        locus            = (h->caput + h->mensura + k) % h->capacitas;
        h->cauda[locus]  = octeti[k];
    }
    h->mensura += n;
    redde n;
}

/* caudam infanti mittere quantum accipit; reddit missos */
interior i32
caudam_mittere (
    AemulatorHospes* h)
{
    i32 missi;
    i32 segmentum;
    s32 r;

    missi = ZEPHYRUM;
    dum (h->mensura > ZEPHYRUM && !h->clausum)
    {
        segmentum = h->capacitas - h->caput;
        si (segmentum > h->mensura)
        {
            segmentum = h->mensura;
        }
        r = h->pt->scribere(h->pt->datum, h->cauda + h->caput,
            segmentum);
        si (r < ZEPHYRUM)
        {
            /* infans abiit: cauda perit */
            h->mensura = ZEPHYRUM;
            frange;
        }
        h->caput    = (h->caput + (i32)r) % h->capacitas;
        h->mensura  -= (i32)r;
        missi       += (i32)r;
        si ((i32)r < segmentum)
        {
            frange;
        }
    }
    redde missi;
}

interior vacuum
responsum_capere (
           vacuum* datum,
      constans i8* octeti,
              i32  n)
{
    AemulatorHospes* h;

    h = (AemulatorHospes*)datum;
    si (in_caudam(h, octeti, n, h->capacitas) < n)
    {
        h->responsa_amissa++;
    }
}

interior vacuum
campanam_transmittere (
    vacuum* datum)
{
    AemulatorHospes* h;

    h = (AemulatorHospes*)datum;
    si (h->vocantis.campana)
    {
        h->vocantis.campana(h->vocantis.datum);
    }
}

interior vacuum
titulum_transmittere (
     vacuum* datum,
     chorda  titulus)
{
    AemulatorHospes* h;

    h = (AemulatorHospes*)datum;
    si (h->vocantis.titulus)
    {
        h->vocantis.titulus(h->vocantis.datum, titulus);
    }
}

vacuum
aemulator_hospes_configuratio_initiare (
    AemulatorHospesConfiguratio* cfg)
{
    si (!cfg)
    {
        redde;
    }
    aemulator_configuratio_initiare(&cfg->aemulator);
    cfg->octeti_per_pulsum  = LXIV * MXXIV;
    cfg->cauda_capacitas    = LXIV * MXXIV;
}

AemulatorHospes*
aemulator_hospes_creare (
                          Piscina* piscina,
    constans AemulatorHospesConfiguratio* cfg,
                  Pseudoterminale* pt)
{
          AemulatorHospes* h;
    AemulatorConfiguratio  nucleus;

    si (!pt)
    {
        redde NIHIL;
    }
    si (!piscina || !cfg)
    {
        pt->claudere(pt->datum);
        redde NIHIL;
    }
    h = (AemulatorHospes*)piscina_conari_allocare(piscina,
        magnitudo(AemulatorHospes));
    si (!h)
    {
        pt->claudere(pt->datum);
        redde NIHIL;
    }
    memset(h, ZEPHYRUM, magnitudo(AemulatorHospes));
    h->pt        = pt;
    h->vocantis  = cfg->aemulator.effectus;
    h->capacitas   = cfg->cauda_capacitas ? cfg->cauda_capacitas
                                          : LXIV * MXXIV;
    h->reservatum  = h->capacitas / XVI;
    h->per_pulsum  = cfg->octeti_per_pulsum ? cfg->octeti_per_pulsum
                                            : LXIV * MXXIV;
    h->cauda       = (i8*)piscina_conari_allocare(piscina,
        (memoriae_index)h->capacitas);
    h->sacculus    = (i8*)piscina_conari_allocare(piscina,
        SACCULUS_LECTIONIS);
    nucleus                     = cfg->aemulator;
    nucleus.effectus.datum      = h;
    nucleus.effectus.responsum  = responsum_capere;
    nucleus.effectus.campana    = campanam_transmittere;
    nucleus.effectus.titulus    = titulum_transmittere;
    h->a                        = aemulator_creare(piscina, &nucleus);
    si (!h->cauda || !h->sacculus || !h->a)
    {
        pt->claudere(pt->datum);
        redde NIHIL;
    }
    (vacuum)pt->amplitudo(pt->datum, nucleus.latitudo,
        nucleus.altitudo, ZEPHYRUM, ZEPHYRUM);
    redde h;
}

AemulatorHospesPulsus
aemulator_hospes_pulsare (
    AemulatorHospes* h,
                s32  mora_ms)
{
    AemulatorHospesPulsus p;
                      s32 r;
                      i32 petiti;

    memset(&p, ZEPHYRUM, magnitudo(p));
    si (h->clausum)
    {
        p.finitus = VERUM;
        redde p;
    }
    p.missi = caudam_mittere(h);
    dum (!h->finis_lectus && p.lecti < h->per_pulsum)
    {
        petiti = h->per_pulsum - p.lecti;
        si (petiti > SACCULUS_LECTIONIS)
        {
            petiti = SACCULUS_LECTIONIS;
        }
        /* mora solum ante octetum primum */
        r = h->pt->legere(h->pt->datum, h->sacculus, petiti,
            p.lecti == ZEPHYRUM ? mora_ms : ZEPHYRUM);
        si (r < ZEPHYRUM)
        {
            h->finis_lectus = VERUM;
            frange;
        }
        si (r == ZEPHYRUM)
        {
            frange;
        }
        aemulator_scribere(h->a, h->sacculus, (i32)r);
        p.lecti += (i32)r;
    }
    p.missi              += caudam_mittere(h);
    p.mutatum            = p.lecti > ZEPHYRUM || h->amplitudo_mutata;
    h->amplitudo_mutata  = FALSUM;
    si (h->finis_lectus && !h->messus)
    {
        h->messus = h->pt->finitus(h->pt->datum, &h->exitus);
    }
    p.finitus = h->finis_lectus && h->messus;
    redde p;
}

i32
aemulator_hospes_scribere (
     AemulatorHospes* h,
         constans i8* octeti,
                 i32  n)
{
    si (h->clausum)
    {
        redde ZEPHYRUM;
    }
    redde in_caudam(h, octeti, n, h->capacitas - h->reservatum);
}

b32
aemulator_hospes_amplitudo (
    AemulatorHospes* h,
                i32  latitudo,
                i32  altitudo,
                i32  px_latitudo,
                i32  px_altitudo)
{
    si (!aemulator_amplitudo(h->a, latitudo, altitudo))
    {
        redde FALSUM;
    }
    si (!h->clausum)
    {
        (vacuum)h->pt->amplitudo(h->pt->datum, latitudo, altitudo,
            px_latitudo, px_altitudo);
    }
    h->amplitudo_mutata = VERUM;
    redde VERUM;
}

constans Aemulator*
aemulator_hospes_aemulator (
    constans AemulatorHospes* h)
{
    redde h->a;
}

s32
aemulator_hospes_fossa (
    constans AemulatorHospes* h)
{
    si (h->clausum)
    {
        redde -I;
    }
    redde h->pt->fossa(h->pt->datum);
}

b32
aemulator_hospes_exitus (
          AemulatorHospes* h,
    PseudoterminaleExitus* exitus)
{
    si (!h->messus && !h->clausum)
    {
        h->messus = h->pt->finitus(h->pt->datum, &h->exitus);
    }
    si (h->messus && exitus)
    {
        *exitus = h->exitus;
    }
    redde h->messus;
}

vacuum
aemulator_hospes_claudere (
    AemulatorHospes* h)
{
    si (h->clausum)
    {
        redde;
    }
    h->pt->claudere(h->pt->datum);
    si (!h->messus)
    {
        h->messus = h->pt->finitus(h->pt->datum, &h->exitus);
    }
    h->clausum = VERUM;
    h->mensura = ZEPHYRUM;
}
