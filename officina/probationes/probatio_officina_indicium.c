/* probatio_officina_indicium.c - INDICIUM M3 chunk 3: circuitus
 * (coque -> scribe -> reclude -> quaere), acies macronum acervata,
 * retro, variabilia, asserta magnitudinum (deriva ABI), probatio
 * FIGENS pacti ambulationis canonicae (indicium == congelatio
 * machinulae), determinismus (bis scripta = octeti idem), magica
 * corrupta reiecta. Fixtura: fixa/indicium_exemplum.c (lineae
 * FIXAE: GEMINARE definitum linea 4, usum linea 15; linea plana
 * 12). */

#include "officina_indicium.h"
#include "officina_demissio.h"
#include "officina_machinula.h"
#include "officina_regio.h"
#include "credo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VIA_FIXTURAE   "officina/probationes/fixa/indicium_exemplum.c"
#define VIA_PARSURAE   "indicium_exemplum.c"
#define VIA_PLAGULAE_A "officina/build/probatio_indicium_a.indicium"
#define VIA_PLAGULAE_B "officina/build/probatio_indicium_b.indicium"

#define LINEA_DEFINITIONIS 4
#define LINEA_PLANA        12
#define LINEA_MACRONIS     15

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

interior vacuum
_viam_plenam (
    constans character* via_partis,
             character* cella)
{
    constans character* radix = getenv("RHUBARB_RADIX");

    sprintf(cella, "%s/%s", (radix != NIHIL) ? radix : ".",
        via_partis);
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

    fructus.datum    = NIHIL;
    fructus.mensura  = ZEPHYRUM;
    _viam_plenam(via_partis, via);
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

interior b32
_variabilem_invenire (
       constans IndiciumLector*  lector,
    constans IndiciumVariabile*  acies,
                           i32   numerus,
            constans character*  titulus,
    constans IndiciumVariabile** v_out)
{
    i32 i;

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        chorda t = indicium_chorda(lector, acies[i].titulus);

        si (   t.mensura == (i32)strlen(titulus)
            && memcmp(t.datum, titulus,
                   (memoriae_index)t.mensura) == 0)
        {
            *v_out = &acies[i];
            redde VERUM;
        }
    }
    redde FALSUM;
}

s32
principale (vacuum)
{
    Piscina* piscina;
    SilvaPiscina* piscina_silvae;
    b32 successus;

    imprimere("\n========================================\n");
    imprimere("PROBATIONES OFFICINA INDICIUM\n");
    imprimere("========================================\n");

    piscina = piscina_generare_dynamicum("probatio_indicium",
        LXIV * M * M);
    si (piscina == NIHIL)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    piscina_silvae = silva_piscina_generare_dynamicum(
        "probatio_indicium_silva", 33554432);
    si (piscina_silvae == NIHIL)
    {
        imprimere("FRACTA: piscina silvae\n");
        redde I;
    }
    credo_aperire(piscina);

    /* --- magnitudines in disco (deriva ABI figitur) --- */
    imprimere("\n--- Probans magnitudines formarum ---\n");
    CREDO_AEQUALIS_I32 ((i32)magnitudo(IndiciumCaput), XVI);
    CREDO_AEQUALIS_I32 ((i32)magnitudo(IndiciumSectio), VIII);
    CREDO_AEQUALIS_I32 ((i32)magnitudo(IndiciumVia), XVI);
    CREDO_AEQUALIS_I32 ((i32)magnitudo(IndiciumModulus), IV);
    CREDO_AEQUALIS_I32 ((i32)magnitudo(IndiciumFunctio), XXXII);
    CREDO_AEQUALIS_I32 ((i32)magnitudo(IndiciumLinea), XVI);
    CREDO_AEQUALIS_I32 ((i32)magnitudo(IndiciumSitus), VIII);
    CREDO_AEQUALIS_I32 ((i32)magnitudo(IndiciumVariabile), XVI);

    /* --- coquere: parse -> demissio -> collige -> necte --- */
    {
        chorda fons = _plagulam_legere(piscina, VIA_FIXTURAE);
        SilvaParsura* parsura;
        SilvaSemantica* sem;
        MedullaModulus* modulus;
        IndiciumScriptor* scriptor;
        Regio* regio;
        Conexio* conexio;
        Machinula* machinula;
        IndiciumLector* lector;
        s32 functio_index;
        character via_a[CCLVI];
        character via_b[CCLVI];

        imprimere("\n--- Probans coctionem fixturae ---\n");
        CREDO_VERUM (fons.mensura > ZEPHYRUM);
        parsura = silva_c89_parsare(piscina_silvae, VIA_PARSURAE,
            (constans character*)fons.datum, (insignatus integer)
            fons.mensura, NIHIL);
        CREDO_NON_NIHIL (parsura);
        CREDO_AEQUALIS_I32 ((i32)parsura->numerus_errorum,
            ZEPHYRUM);
        sem = silva_c89_semantica_analysare(piscina_silvae,
            parsura);
        CREDO_NON_NIHIL (sem);
        modulus = demissio_currere(piscina, parsura, sem,
            _ch(VIA_PARSURAE));
        CREDO_NON_NIHIL (modulus);
        si (modulus == NIHIL)
        {
            redde I;
        }

        scriptor = indicium_scriptor_creare(piscina);
        CREDO_NON_NIHIL (scriptor);
        CREDO_VERUM (indicium_modulum_colligere(scriptor, modulus,
            parsura, sem));

        regio = regio_generare(piscina);
        CREDO_NON_NIHIL (regio);
        conexio = conexio_creare(piscina, regio);
        CREDO_NON_NIHIL (conexio);
        CREDO_VERUM (conexio_modulum_addere(conexio, modulus));
        CREDO_VERUM (conexio_nectere(conexio));
        machinula = machinula_creare(piscina, conexio, regio);
        CREDO_NON_NIHIL (machinula);

        /* --- scribere (bis - determinismus) --- */
        imprimere("\n--- Probans scripturam + determinismum ---\n");
        _viam_plenam(VIA_PLAGULAE_A, via_a);
        _viam_plenam(VIA_PLAGULAE_B, via_b);
        CREDO_VERUM (indicium_scribere(scriptor, conexio, via_a));
        CREDO_VERUM (indicium_scribere(scriptor, conexio, via_b));
        {
            chorda a = _plagulam_legere(piscina, VIA_PLAGULAE_A);
            chorda b = _plagulam_legere(piscina, VIA_PLAGULAE_B);

            CREDO_VERUM (a.mensura > ZEPHYRUM);
            CREDO_AEQUALIS_I32 ((i32)a.mensura, (i32)b.mensura);
            CREDO_VERUM (memcmp(a.datum, b.datum,
                (memoriae_index)a.mensura) == 0);
        }

        /* --- recludere + quaerere --- */
        imprimere("\n--- Probans lectorem ---\n");
        lector = indicium_aperire(piscina, via_a);
        CREDO_NON_NIHIL (lector);
        si (lector == NIHIL)
        {
            redde I;
        }
        functio_index = indicium_functionem_quaerere(lector,
            _ch("computare"));
        CREDO_VERUM (functio_index >= ZEPHYRUM);

        /* probatio FIGENS: ambulatio collectionis == congelatio */
        imprimere("\n--- Probans pactum ambulationis ---\n");
        {
            constans IndiciumFunctio* ifu = indicium_functio(
                lector, (i32)functio_index);

            CREDO_NON_NIHIL (ifu);
            CREDO_VERUM (ifu->instructiones_numerus > ZEPHYRUM);
            CREDO_AEQUALIS_I32 (ifu->instructiones_numerus,
                machinula_numerus_instructionum_planarum(machinula,
                    functio_index));
        }

        /* retro + prorsum: linea plana (12) */
        imprimere("\n--- Probans lineam planam ---\n");
        {
            constans IndiciumSitus* situs = NIHIL;
            i32 numerus = indicium_situs_de_linea(lector,
                _ch(VIA_PARSURAE), (i32)LINEA_PLANA, &situs);

            CREDO_VERUM (numerus > ZEPHYRUM);
            si (numerus > ZEPHYRUM)
            {
                constans IndiciumLinea* acies = NIHIL;
                i32 n = indicium_lineas_de_instructione(lector,
                    (i32)functio_index, situs[ZEPHYRUM].instructio,
                    &acies);

                CREDO_AEQUALIS_I32 (n, I);
                CREDO_AEQUALIS_I32 (acies[ZEPHYRUM].linea,
                    (i32)LINEA_PLANA);
                CREDO_AEQUALIS_I32 (acies[ZEPHYRUM].profunditas,
                    ZEPHYRUM);
            }
        }

        /* retro + prorsum: acies macronis acervata (15 <- 4) */
        imprimere("\n--- Probans aciem macronis ---\n");
        {
            constans IndiciumSitus* situs = NIHIL;
            i32 numerus = indicium_situs_de_linea(lector,
                _ch(VIA_PARSURAE), (i32)LINEA_MACRONIS, &situs);
            b32 acervata_inventa = FALSUM;
            i32 k;

            CREDO_VERUM (numerus > ZEPHYRUM);
            per (k = ZEPHYRUM; k < numerus; k++)
            {
                constans IndiciumLinea* acies = NIHIL;
                i32 n = indicium_lineas_de_instructione(lector,
                    (i32)functio_index, situs[k].instructio,
                    &acies);

                si (   n >= II
                    && acies[ZEPHYRUM].profunditas == ZEPHYRUM
                    && acies[ZEPHYRUM].linea == (i32)LINEA_MACRONIS
                    && acies[I].profunditas == I
                    && acies[I].linea == (i32)LINEA_DEFINITIONIS)
                {
                    chorda titulus_macronis = indicium_chorda(
                        lector, acies[I].nomen_macro);

                    si (   titulus_macronis.mensura == VIII
                        && memcmp(titulus_macronis.datum,
                               "GEMINARE", VIII) == 0)
                    {
                        acervata_inventa = VERUM;
                    }
                }
            }
            CREDO_VERUM (acervata_inventa);
        }

        /* variabilia: nomina + tags + typi scripti */
        imprimere("\n--- Probans variabilia ---\n");
        {
            constans IndiciumVariabile* acies = NIHIL;
            i32 numerus = indicium_variabilia_functionis(lector,
                (i32)functio_index, &acies);
            constans IndiciumVariabile* v = NIHIL;

            CREDO_VERUM (numerus >= IV);   /* fons, a, b, p */
            CREDO_VERUM (_variabilem_invenire(lector, acies,
                numerus, "a", &v));
            si (v != NIHIL)
            {
                chorda typus = indicium_chorda(lector,
                    v->typus_scriptus);

                CREDO_AEQUALIS_I32 ((i32)v->typus_medulla,
                    (i32)MEDULLA_TYPUS_S32);
                CREDO_VERUM (typus.mensura == III
                    && memcmp(typus.datum, "int", III) == 0);
            }
            CREDO_VERUM (_variabilem_invenire(lector, acies,
                numerus, "b", &v));
            si (v != NIHIL)
            {
                chorda typus = indicium_chorda(lector,
                    v->typus_scriptus);

                CREDO_AEQUALIS_I32 ((i32)v->typus_medulla,
                    (i32)MEDULLA_TYPUS_I32);
                CREDO_VERUM (typus.mensura == XII
                    && memcmp(typus.datum, "unsigned int",
                           XII) == 0);
            }
            CREDO_VERUM (_variabilem_invenire(lector, acies,
                numerus, "p", &v));
            si (v != NIHIL)
            {
                chorda typus = indicium_chorda(lector,
                    v->typus_scriptus);

                CREDO_AEQUALIS_I32 ((i32)v->typus_medulla,
                    (i32)MEDULLA_TYPUS_I64);
                CREDO_VERUM (typus.mensura == V
                    && memcmp(typus.datum, "char*", V) == 0);
            }
            CREDO_VERUM (_variabilem_invenire(lector, acies,
                numerus, "fons", &v));
            si (v != NIHIL)
            {
                CREDO_AEQUALIS_I32 ((i32)v->typus_medulla,
                    (i32)MEDULLA_TYPUS_S32);
            }
        }

        /* magica corrupta -> reiecta */
        imprimere("\n--- Probans reiectionem magicae ---\n");
        {
            chorda octeti = _plagulam_legere(piscina,
                VIA_PLAGULAE_B);
            character via_corrupta[CCLVI];
            FILE* plagula;

            CREDO_VERUM (octeti.mensura > ZEPHYRUM);
            octeti.datum[ZEPHYRUM] = (i8)'X';
            _viam_plenam(VIA_PLAGULAE_B, via_corrupta);
            plagula = fopen(via_corrupta, "wb");
            CREDO_NON_NIHIL (plagula);
            si (plagula != NIHIL)
            {
                (vacuum)fwrite(octeti.datum, I,
                    (memoriae_index)octeti.mensura, plagula);
                fclose(plagula);
                CREDO_VERUM (indicium_aperire(piscina,
                    via_corrupta) == NIHIL);
            }
        }
    }

    imprimere("\n");
    credo_imprimere_compendium();
    imprimere("========================================\n");
    successus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(piscina);
    redde successus ? ZEPHYRUM : I;
}
