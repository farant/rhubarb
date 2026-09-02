/* vindex_onerator.c - onerator mundi app-lateris (vide .h) */

#include "vindex_onerator.h"
#include "praeparator.h"

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "tabula_dispersa.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

structura VindexOnerator {
            Piscina* piscina_ctx;      /* app: xar, buffers */
    OfficinaPiscina* piscina_off;      /* moduli, lineae, indicium */
        Praeparatio  praeparatio;      /* ctx + systema (praeparator
                                        * - unitas communis) */
                 Xar* moduli;           /* MedullaModulus* */
                 Xar* lineae;           /* MedullaLineae* */
    IndiciumScriptor* scriptor;
      IndiciumLector* lector;
             Conexio* conexio;
           character  radix[512];
};

interior OfficinaChorda
_och (
    constans character* literis)
{
    OfficinaChorda c;
    unio { constans character* c; insignatus character* m; } u;

    u.c        = literis;
    c.datum    = u.m;
    c.mensura  = (insignatus integer)strlen(literis);
    redde c;
}


/* ==================================================
 * API
 * ================================================== */

VindexOnerator*
vindex_onerator_creare (
    constans character* radix)
{
    VindexOnerator* o;
           Piscina* piscina_ctx = piscina_generare_dynamicum(
               "vindex_onerator", 8388608);

    si (piscina_ctx == NIHIL) redde NIHIL;
    o = piscina_allocare(piscina_ctx,
        magnitudo(VindexOnerator));
    si (o == NIHIL) redde NIHIL;
    memset(o, ZEPHYRUM, magnitudo(VindexOnerator));
    o->piscina_ctx = piscina_ctx;
    si (strlen(radix) >= magnitudo(o->radix)) redde NIHIL;
    strcpy(o->radix, radix);
    o->piscina_off = officina_piscina_generare_dynamicum(
        "vindex_off", 33554432);
    si (o->piscina_off == NIHIL)
    {
        redde NIHIL;
    }
    o->moduli = xar_creare(piscina_ctx,
        (i32)magnitudo(vacuum*));
    o->lineae = xar_creare(piscina_ctx,
        (i32)magnitudo(vacuum*));
    o->scriptor = indicium_scriptor_creare(o->piscina_off);
    si (   o->moduli   == NIHIL || o->lineae == NIHIL
        || o->scriptor == NIHIL)
    {
        redde NIHIL;
    }

    /* systema ISO+POSIX + capita (praeparator - unitas communis;
     * mores servati: sine latina in systemate) */
    {
        PraeparatorConfiguratio pcfg;

        memset(&pcfg, ZEPHYRUM, magnitudo(PraeparatorConfiguratio));
        pcfg.radix      = o->radix;
        pcfg.cum_posix  = I;
        si (!praeparator_praeparare(&o->praeparatio, piscina_ctx,
                &pcfg))
        {
            redde NIHIL;
        }
    }
    redde o;
}

b32
vindex_onerator_demittere (
        VindexOnerator* o,
    constans character* via)
{
             character via_lecta[1024];
    insignatus integer mensura = ZEPHYRUM;
    SilvaPiscina* piscina_arboris;
    character* fons;
    SilvaParsura* parsura = NIHIL;
    SilvaSemantica* sem;
    MedullaModulus* modulus;
    MedullaLineae* lineae = NIHIL;

    si (o == NIHIL || via == NIHIL) redde FALSUM;
    sprintf(via_lecta, "%s/%s", o->radix, via);
    piscina_arboris = silva_piscina_generare_dynamicum(
        "vindex_arbor", 8388608);
    si (piscina_arboris == NIHIL) redde FALSUM;
    fons = praeparator_plagulam_legere(o->piscina_ctx, via_lecta,
        &mensura);
    si (fons == NIHIL)
    {
        silva_piscina_destruere(piscina_arboris);
        redde FALSUM;
    }
    sem = praeparator_analysare(&o->praeparatio, piscina_arboris,
        via, fons, mensura, &parsura);
    si (   sem == NIHIL || parsura == NIHIL
        || parsura->numerus_errorum > 0U)
    {
        silva_piscina_destruere(piscina_arboris);
        redde FALSUM;
    }
    modulus = demissio_currere(o->piscina_off, parsura, sem,
        _och(via));
    si (modulus != NIHIL)
    {
        /* fenestra collige-ante-destrue: lineae + indicium */
        lineae = demissio_lineas_colligere(o->piscina_off, modulus,
            parsura);
        (vacuum)indicium_modulum_colligere(o->scriptor, modulus,
            parsura, sem);
    }
    silva_piscina_destruere(piscina_arboris);
    si (modulus == NIHIL) redde FALSUM;
    {
        vacuum** locellus    = xar_addere(o->moduli);
        vacuum** locellus_l  = xar_addere(o->lineae);

        si (locellus == NIHIL || locellus_l == NIHIL) redde FALSUM;
        *locellus    = modulus;
        *locellus_l  = lineae;
    }
    redde VERUM;
}

Machinula*
vindex_onerator_nectere (
        VindexOnerator* o,
    constans character* via_indicii)
{
    Regio* regio;
    Machinula* machinula;
    i32 k;

    si (o == NIHIL) redde NIHIL;
    regio = regio_generare(o->piscina_off);
    si (regio == NIHIL) redde NIHIL;
    o->conexio = conexio_creare(o->piscina_off, regio);
    si (o->conexio == NIHIL) redde NIHIL;
    per (k = ZEPHYRUM; k < xar_numerus(o->moduli); k++)
    {
        si (!conexio_modulum_addere(o->conexio,
                *(MedullaModulus**)xar_obtinere(o->moduli, k)))
        {
            redde NIHIL;
        }
    }
    si (!conexio_nectere(o->conexio)) redde NIHIL;
    machinula = machinula_creare(o->piscina_off, o->conexio,
        regio);
    si (machinula == NIHIL) redde NIHIL;
    per (k = ZEPHYRUM; k < xar_numerus(o->lineae); k++)
    {
        MedullaLineae* lineae_k = *(MedullaLineae**)xar_obtinere(
            o->lineae, k);

        si (lineae_k != NIHIL)
        {
            machinula_lineas_praebere(machinula, (integer)k,
                lineae_k);
        }
    }
    si (via_indicii != NIHIL)
    {
        si (!indicium_scribere(o->scriptor, o->conexio,
                via_indicii))
        {
            redde NIHIL;
        }
        o->lector = indicium_aperire(o->piscina_off, via_indicii);
        si (o->lector == NIHIL) redde NIHIL;
    }
    redde machinula;
}

IndiciumLector*
vindex_onerator_lector (
    VindexOnerator* o)
{
    redde (o != NIHIL) ? o->lector : NIHIL;
}

OfficinaChorda
vindex_onerator_querela (
    VindexOnerator* o)
{
    OfficinaChorda vacua;

    si (o != NIHIL && o->conexio != NIHIL)
    {
        constans OfficinaChorda* symbolum =
            conexio_querela_symbolum(o->conexio);

        si (symbolum != NIHIL)
        {
            redde *symbolum;
        }
    }
    vacua.datum    = NIHIL;
    vacua.mensura  = 0U;
    redde vacua;
}
