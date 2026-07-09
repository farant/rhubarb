/* probatio_officina_demissio.c - Demissio M1b A: plagulae
 * scalares + fluxus DEMITTUNTUR (sine sistere!), scripturae
 * aureae fixae, vectis lectoris byte-idem. Aurea absentia
 * PINGITUR (inspicienda ante commissionem). */

#include "officina_demissio.h"
#include "officina_medulla_textus.h"
#include "credo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

interior chorda
_ch (constans character* literis)
{
    chorda c;
    unio { constans character* c; i8* m; } u;

    u.c = literis;
    c.datum = u.m;
    c.mensura = (i32)strlen(literis);
    redde c;
}

interior chorda
_plagulam_legere (Piscina* piscina, constans character* via_partis)
{
    character via[CCLVI];
    FILE* plagula;
    chorda fructus;
    long longitudo;
    constans character* radix = getenv("RHUBARB_RADIX");

    fructus.datum = NIHIL;
    fructus.mensura = ZEPHYRUM;
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
        si (fructus.datum != NIHIL
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
_plagulam_scribere (constans character* via_partis, chorda contenta)
{
    character via[CCLVI];
    FILE* plagula;
    constans character* radix = getenv("RHUBARB_RADIX");

    sprintf(via, "%s/%s", (radix != NIHIL) ? radix : ".",
        via_partis);
    plagula = fopen(via, "wb");
    si (plagula == NIHIL)
    {
        redde;
    }
    (vacuum)fwrite(contenta.datum, I,
        (memoriae_index)contenta.mensura, plagula);
    fclose(plagula);
}

interior MedullaModulus*
_demittere (Piscina* piscina, SilvaPiscina* piscina_silvae,
    chorda fons, constans character* titulus)
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
_plagulam_probare (Piscina* piscina, SilvaPiscina* piscina_silvae,
    constans character* via_fontis, constans character* via_aurei,
    constans character* titulus)
{
    chorda fons = _plagulam_legere(piscina, via_fontis);
    MedullaModulus* modulus;
    chorda scriptura;
    i32 i;
    i32 m;

    imprimere("\n--- Probans demissionem: %s ---\n", via_fontis);
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
        i32 linea = ZEPHYRUM;
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

s32 principale (vacuum)
{
    Piscina* piscina;
    SilvaPiscina* piscina_silvae;
    b32 praeteritus;

    piscina = piscina_generare_dynamicum("probatio_demissio",
        33554432);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    piscina_silvae = silva_piscina_generare_dynamicum(
        "probatio_demissio_silva", 33554432);
    si (!piscina_silvae)
    {
        imprimere("FRACTA: piscina silvae\n");
        redde I;
    }
    credo_aperire(piscina);

    _plagulam_probare(piscina, piscina_silvae,
        "officina/probationes/fixa/scalaria.c",
        "officina/probationes/fixa/scalaria.medulla", "scalaria.c");
    _plagulam_probare(piscina, piscina_silvae,
        "officina/probationes/fixa/fluxus.c",
        "officina/probationes/fixa/fluxus.medulla", "fluxus.c");
    _plagulam_probare(piscina, piscina_silvae,
        "officina/probationes/fixa/aggregata.c",
        "officina/probationes/fixa/aggregata.medulla",
        "aggregata.c");

    /* structura directa: duplum = functio prima scalariae */
    {
        chorda fons = _plagulam_legere(piscina,
            "officina/probationes/fixa/scalaria.c");
        MedullaModulus* modulus = _demittere(piscina,
            piscina_silvae, fons, "scalaria.c");

        imprimere("\n--- Probans structuram (duplum) ---\n");
        CREDO_NON_NIHIL (modulus);
        si (modulus != NIHIL)
        {
            MedullaFunctio** functio =
                (MedullaFunctio**)xar_obtinere(modulus->functiones,
                    ZEPHYRUM);
            constans MedullaBloccus* bloccus;
            constans MedullaInstructio* ultima;
            i32 n;

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

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    silva_piscina_destruere(piscina_silvae);
    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    redde I;
}
