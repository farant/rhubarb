/* probatio_officina_demissio.c - Demissio M1b A: plagulae
 * scalares + fluxus DEMITTUNTUR (sine sistere!), scripturae
 * aureae fixae, vectis lectoris byte-idem. Aurea absentia
 * PINGITUR (inspicienda ante commissionem). */

#include "officina_demissio.h"
#include "officina_medulla_textus.h"
#include "credo.h"
#include "lectiones.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

interior chorda
_ch (
    constans character* literis)
{
    chorda c;
    unio { constans character* c; i8* m; } u;

    u.c        = literis;
    c.datum    = u.m;
    c.mensura  = (i32)strlen(literis);
    redde c;
}

interior chorda
_plagulam_legere (
               Piscina* piscina,
    constans character* via_partis)
{
    character via[CCLVI];
    FILE* plagula;
    chorda fructus;
    long longitudo;
    constans character* radix = getenv("RHUBARB_RADIX");

    fructus.datum    = NIHIL;
    fructus.mensura  = ZEPHYRUM;
    sprintf(via, "%s/%s", (radix != NIHIL) ? radix : ".",
        via_partis);
    plagula = fopen(via, "rb");
    si (plagula == NIHIL)
    {
        redde fructus;
    }
    fseek(plagula, 0L, SEEK_END);
    longitudo = ftell(plagula);
    fseek(plagula, 0L, SEEK_SET);
    si (longitudo > 0L)
    {
        fructus.datum = (i8*)piscina_allocare(piscina,
            (memoriae_index)longitudo);
        si (   fructus.datum != NIHIL
            && fread(fructus.datum, I, (memoriae_index)longitudo,
                   plagula) == (memoriae_index)longitudo)
        {
            fructus.mensura = (i32)longitudo;
        }
        alioquin
        {
            fructus.datum = NIHIL;
        }
    }
    fclose(plagula);
    redde fructus;
}

interior vacuum
_plagulam_scribere (
    constans character* via_partis,
                chorda  contenta)
{
    character via[CCLVI];
    FILE* plagula;
    constans character* radix = getenv("RHUBARB_RADIX");

    sprintf(via, "%s/%s", (radix != NIHIL) ? radix : ".",
        via_partis);
    plagula = lectiones_fopen(via, "wb");
    si (plagula == NIHIL)
    {
        redde;
    }
    (vacuum)fwrite(contenta.datum, I,
        (memoriae_index)contenta.mensura, plagula);
    fclose(plagula);
}

interior MedullaModulus*
_demittere (
               Piscina* piscina,
          SilvaPiscina* piscina_silvae,
                chorda  fons,
    constans character* titulus)
{
    SilvaParsura* parsura = silva_c89_parsare(piscina_silvae,
        "probatio.c", (constans character*)fons.datum, fons.mensura,
        NIHIL);
    SilvaSemantica* sem;

    si (parsura == NIHIL || parsura->numerus_errorum != ZEPHYRUM)
    {
        redde NIHIL;
    }
    sem = silva_c89_semantica_analysare(piscina_silvae, parsura);
    si (sem == NIHIL)
    {
        redde NIHIL;
    }
    redde demissio_currere(piscina, parsura, sem, _ch(titulus));
}

interior vacuum
_plagulam_probare (
               Piscina* piscina,
          SilvaPiscina* piscina_silvae,
    constans character* via_fontis,
    constans character* via_aurei,
    constans character* titulus)
{
            chorda fons = _plagulam_legere(piscina, via_fontis);
    MedullaModulus* modulus;
            chorda  scriptura;
               i32  i;
               i32  m;

    CREDO_VERUM (fons.mensura > ZEPHYRUM);
    modulus = _demittere(piscina, piscina_silvae, fons, titulus);
    CREDO_NON_NIHIL (modulus);
    si (modulus == NIHIL)
    {
        redde;
    }
    /* NULLA sistere: plagulae A plene demittuntur */
    CREDO_AEQUALIS_I32 ((i32)xar_numerus(modulus->causae),
        ZEPHYRUM);
    /* omnes functiones terminatae */
    m = xar_numerus(modulus->functiones);
    CREDO_VERUM (m > ZEPHYRUM);
    per (i = ZEPHYRUM; i < m; i++)
    {
        MedullaFunctio** functio = (MedullaFunctio**)xar_obtinere(
            modulus->functiones, i);

        CREDO_VERUM (medulla_functio_terminata(*functio));
    }
    scriptura = medulla_textum_scribere(piscina, modulus);
    CREDO_VERUM (scriptura.mensura > ZEPHYRUM);
    /* vectis lectoris: scriptura byte-idem per circulum */
    {
                   i32  linea = ZEPHYRUM;
        MedullaModulus* iterum = medulla_textum_legere(piscina,
            scriptura, &linea);

        CREDO_NON_NIHIL (iterum);
        CREDO_AEQUALIS_I32 ((i32)linea, ZEPHYRUM);
        si (iterum != NIHIL)
        {
            CREDO_CHORDA_AEQUALIS (scriptura,
                medulla_textum_scribere(piscina, iterum));
        }
    }
    /* aureum: comparare aut pingere */
    {
        chorda aureum = _plagulam_legere(piscina, via_aurei);

        si (aureum.mensura == ZEPHYRUM)
        {
            _plagulam_scribere(via_aurei, scriptura);
            imprimere("AUREUM FIXUM: %s (inspicienda!)\n",
                via_aurei);
        }
        alioquin
        {
            CREDO_CHORDA_AEQUALIS (scriptura, aureum);
        }
    }
}

/* piscina silvae PROPRIA sectionis (olim una omnibus blocis) */
interior vacuum
_silvam_parare (
    CredoContextus* c)
{
    c->datum = silva_piscina_generare_dynamicum(
        "probatio_demissio_silva", 33554432);
    si (c->datum == NIHIL)
    {
        imprimere("FRACTA: piscina silvae\n");
    }
}

interior vacuum
_silvam_purgare (
    CredoContextus* c)
{
    si (c->datum != NIHIL)
    {
        silva_piscina_destruere((SilvaPiscina*)c->datum);
    }
}


/* ==================================================
 * PROBARE: plagulae demissae, aureae, vectis lectoris
 * ================================================== */

interior vacuum
_scalaria_probare (
    CredoContextus* c)
{
    si (c->datum == NIHIL)
    {
        redde;   /* sine assertione: sectio VACUA, suita FRACTA */
    }
    _plagulam_probare(c->piscina, (SilvaPiscina*)c->datum,
        "officina/probationes/fixa/scalaria.c",
        "officina/probationes/fixa/scalaria.medulla", "scalaria.c");
}

interior vacuum
_fluxum_probare (
    CredoContextus* c)
{
    si (c->datum == NIHIL)
    {
        redde;   /* sine assertione: sectio VACUA, suita FRACTA */
    }
    _plagulam_probare(c->piscina, (SilvaPiscina*)c->datum,
        "officina/probationes/fixa/fluxus.c",
        "officina/probationes/fixa/fluxus.medulla", "fluxus.c");
}

interior vacuum
_aggregata_probare (
    CredoContextus* c)
{
    si (c->datum == NIHIL)
    {
        redde;   /* sine assertione: sectio VACUA, suita FRACTA */
    }
    _plagulam_probare(c->piscina, (SilvaPiscina*)c->datum,
        "officina/probationes/fixa/aggregata.c",
        "officina/probationes/fixa/aggregata.medulla", "aggregata.c");
}

interior vacuum
_statica_probare (
    CredoContextus* c)
{
    si (c->datum == NIHIL)
    {
        redde;   /* sine assertione: sectio VACUA, suita FRACTA */
    }
    _plagulam_probare(c->piscina, (SilvaPiscina*)c->datum,
        "officina/probationes/fixa/statica.c",
        "officina/probationes/fixa/statica.medulla", "statica.c");
}


/* ==================================================
 * PROBARE: structura directa: duplum = functio prima scalariae
 * ================================================== */

interior vacuum
_structuram_probare (
    CredoContextus* c)
{
         Piscina* piscina         = c->piscina;
    SilvaPiscina* piscina_silvae  = (SilvaPiscina*)c->datum;

    si (piscina_silvae == NIHIL)
    {
        redde;   /* sine assertione: sectio VACUA, suita FRACTA */
    }
    {
        chorda fons = _plagulam_legere(piscina,
            "officina/probationes/fixa/scalaria.c");
        MedullaModulus* modulus = _demittere(piscina,
            piscina_silvae, fons, "scalaria.c");

        CREDO_NON_NIHIL (modulus);
        si (modulus != NIHIL)
        {
            MedullaFunctio** functio =
                (MedullaFunctio**)xar_obtinere(modulus->functiones,
                    ZEPHYRUM);
               constans MedullaBloccus* bloccus;
            constans MedullaInstructio* ultima;
                                   i32  n;

            CREDO_NON_NIHIL (functio);
            CREDO_AEQUALIS_I32 ((i32)xar_numerus(
                (*functio)->parametra), I);
            bloccus = medulla_bloccum_obtinere(*functio, ZEPHYRUM);
            CREDO_NON_NIHIL ((constans vacuum*)bloccus);
            n = xar_numerus(bloccus->instructiones);
            CREDO_VERUM (n >= II);
            ultima = (constans MedullaInstructio*)xar_obtinere(
                bloccus->instructiones, n - I);
            CREDO_AEQUALIS_S32 (ultima->op, MEDULLA_OP_REDDE);
        }
    }
}


/* ==================================================
 * PROBARE: distillatio linearum (M2a): origo -> (via, linea)
 * ================================================== */

interior vacuum
_distillationem_probare (
    CredoContextus* c)
{
         Piscina* piscina         = c->piscina;
    SilvaPiscina* piscina_silvae  = (SilvaPiscina*)c->datum;

    si (piscina_silvae == NIHIL)
    {
        redde;   /* sine assertione: sectio VACUA, suita FRACTA */
    }
    {
        chorda fons = _plagulam_legere(piscina,
            "officina/probationes/fixa/scalaria.c");
        SilvaParsura* parsura = silva_c89_parsare(piscina_silvae,
            "probatio.c", (constans character*)fons.datum,
            fons.mensura, NIHIL);
        SilvaSemantica* sem;
        MedullaModulus* modulus;
        MedullaLineae* lineae;

        CREDO_NON_NIHIL (parsura);
        sem = silva_c89_semantica_analysare(piscina_silvae, parsura);
        CREDO_NON_NIHIL (sem);
        modulus = demissio_currere(piscina, parsura, sem,
            _ch("scalaria.c"));
        CREDO_NON_NIHIL (modulus);
        lineae = demissio_lineas_colligere(piscina, modulus,
            parsura);
        CREDO_NON_NIHIL (lineae);
        si (modulus != NIHIL && lineae != NIHIL)
        {
            MedullaFunctio** functio =
                (MedullaFunctio**)xar_obtinere(modulus->functiones,
                    ZEPHYRUM);
            constans MedullaBloccus* bloccus =
                medulla_bloccum_obtinere(*functio, ZEPHYRUM);
            constans MedullaInstructio* instructio =
                (constans MedullaInstructio*)xar_obtinere(
                    bloccus->instructiones, ZEPHYRUM);
            chorda via;
               i32 linea = ZEPHYRUM;

            CREDO_NON_NIHIL ((constans vacuum*)instructio->origo);
            CREDO_VERUM (medulla_lineam_quaerere(lineae,
                instructio->origo, &via, &linea));
            CREDO_VERUM (linea > ZEPHYRUM);
            CREDO_VERUM (chorda_aequalis_literis(via, "probatio.c"));
            /* origo alienum -> FALSUM */
            CREDO_FALSUM (medulla_lineam_quaerere(lineae,
                (constans structura SilvaNodus*)(constans vacuum*)
                    &via, NIHIL, NIHIL));
        }
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "demissionem: officina/probationes/fixa/scalaria.c",
        _scalaria_probare, _silvam_parare, _silvam_purgare, NIHIL,
        NIHIL },
    { "demissionem: officina/probationes/fixa/fluxus.c",
        _fluxum_probare, _silvam_parare, _silvam_purgare, NIHIL,
        NIHIL },
    { "demissionem: officina/probationes/fixa/aggregata.c",
        _aggregata_probare, _silvam_parare, _silvam_purgare, NIHIL,
        NIHIL },
    { "demissionem: officina/probationes/fixa/statica.c",
        _statica_probare, _silvam_parare, _silvam_purgare, NIHIL,
        NIHIL },
    { "structuram (duplum)", _structuram_probare, _silvam_parare,
        _silvam_purgare, NIHIL, NIHIL },
    { "distillationem linearum", _distillationem_probare,
        _silvam_parare, _silvam_purgare, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("officina_demissio", SECTIONES);
}
