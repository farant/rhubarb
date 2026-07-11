/* probatio_officina_puncta.c - M3 chunk 4: pausa/gradus/puncta.
 * Fixtura: fixa/machinula/vocationes.medulla (main -> 42 per
 * vocationes directas ET per descriptorem). Vectis: pergere sine
 * punctis == currere; punctum tactum -> PAUSA positione recta ANTE
 * exsecutionem; registra lecta in pausa; resumptio -> BENE 42 idem;
 * punctum in vocato (profunditas 2) et per iter descriptoris;
 * tollere; gradus singuli numerati. */

#include "officina_machinula.h"
#include "officina_medulla_textus.h"
#include "officina_conexio.h"
#include "officina_regio.h"
#include "credo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VIA_FIXTURAE \
    "officina/probationes/fixa/machinula/vocationes.medulla"

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

interior s32
_functionem_quaerere (constans Conexio* conexio,
    constans character* titulus)
{
    s32 index = conexio_symbolum_quaerere(conexio, _ch(titulus));
    constans ConexioSymbolum* symbolum;
    constans ConexioDescriptor* descriptor;

    si (index < ZEPHYRUM)
    {
        redde -I;
    }
    symbolum = conexio_symbolum_obtinere(conexio, index);
    si (symbolum->genus != CONEXIO_SYMBOLUM_FUNCTIO)
    {
        redde -I;
    }
    descriptor = symbolum->sedes;
    redde (s32)descriptor->index;
}

s32
principale (vacuum)
{
    Piscina* piscina;
    b32 successus;

    imprimere("\n========================================\n");
    imprimere("PROBATIONES OFFICINA PUNCTA (M3)\n");
    imprimere("========================================\n");

    piscina = piscina_generare_dynamicum("probatio_puncta",
        LXIV * M * M);
    si (piscina == NIHIL)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);

    {
        chorda textus = _plagulam_legere(piscina, VIA_FIXTURAE);
        i32 linea_erroris = ZEPHYRUM;
        MedullaModulus* modulus;
        Regio* regio;
        Conexio* conexio;
        Machinula* machinula;
        s32 index_main;
        s32 index_addens;
        s32 index_geminare;
        i64 instructiones_cursus = ZEPHYRUM;

        CREDO_VERUM (textus.mensura > ZEPHYRUM);
        modulus = medulla_textum_legere(piscina, textus,
            &linea_erroris);
        CREDO_NON_NIHIL (modulus);
        regio = regio_generare(piscina);
        conexio = conexio_creare(piscina, regio);
        CREDO_VERUM (conexio_modulum_addere(conexio, modulus));
        CREDO_VERUM (conexio_nectere(conexio));
        machinula = machinula_creare(piscina, conexio, regio);
        CREDO_NON_NIHIL (machinula);
        si (machinula == NIHIL)
        {
            redde I;
        }
        index_main = _functionem_quaerere(conexio, "main");
        index_addens = _functionem_quaerere(conexio, "addens");
        index_geminare = _functionem_quaerere(conexio, "geminare");
        CREDO_VERUM (index_main >= ZEPHYRUM);
        CREDO_VERUM (index_addens >= ZEPHYRUM);
        CREDO_VERUM (index_geminare >= ZEPHYRUM);

        /* --- fundamentum: currere -> 42 --- */
        imprimere("\n--- Probans fundamentum (currere) ---\n");
        {
            MachinulaExitus fructus = machinula_currere(machinula,
                _ch("main"));

            CREDO_AEQUALIS_I32 ((i32)fructus.genus,
                (i32)MACHINULA_BENE);
            CREDO_AEQUALIS_I32 ((i32)fructus.codex, XLII);
            instructiones_cursus =
                machinula_numerus_instructionum(machinula);
            CREDO_VERUM (instructiones_cursus > ZEPHYRUM);
        }

        /* --- gradus singuli: numerus idem, fructus idem --- */
        imprimere("\n--- Probans gradus singulos ---\n");
        {
            i64 ante = machinula_numerus_instructionum(machinula);
            i64 numerati = ZEPHYRUM;
            s32 functio_visa = -I;
            i32 instructio_visa = (i32)-I;

            CREDO_VERUM (machinula_aperire(machinula, _ch("main")));
            CREDO_VERUM (machinula_positionem_inspicere(machinula,
                ZEPHYRUM, &functio_visa, &instructio_visa));
            CREDO_AEQUALIS_I32 ((i32)functio_visa, (i32)index_main);
            CREDO_AEQUALIS_I32 (instructio_visa, ZEPHYRUM);
            dum (machinula_gradus(machinula))
            {
                numerati += I;
            }
            numerati += I;   /* gradus ultimus FALSUM reddit sed
                              * instructionem (redde) exsecutus est */
            CREDO_AEQUALIS_I32 ((i32)(machinula_numerus_instructionum(
                machinula) - ante), (i32)numerati);
            CREDO_AEQUALIS_I32 ((i32)(machinula_numerus_instructionum(
                machinula) - ante), (i32)instructiones_cursus);
            CREDO_AEQUALIS_I32 ((i32)machinula_halitus_codex(
                machinula), XLII);
        }

        /* --- punctum in main: pausa ante exsecutionem --- */
        imprimere("\n--- Probans punctum in main ---\n");
        {
            s32 functio_visa = -I;
            i32 instructio_visa = ZEPHYRUM;
            i64 valor = ZEPHYRUM;

            CREDO_VERUM (machinula_punctum_ponere(machinula,
                index_main, IV));
            CREDO_VERUM (machinula_punctum_ponere(machinula,
                index_main, IV));   /* iterum = VERUM (iam positum) */
            CREDO_VERUM (machinula_aperire(machinula, _ch("main")));
            CREDO_AEQUALIS_I32 ((i32)machinula_pergere(machinula),
                (i32)MACHINULA_PAUSA);
            CREDO_AEQUALIS_I32 (machinula_tabulata_numerus(
                machinula), I);
            CREDO_VERUM (machinula_positionem_inspicere(machinula,
                ZEPHYRUM, &functio_visa, &instructio_visa));
            CREDO_AEQUALIS_I32 ((i32)functio_visa, (i32)index_main);
            CREDO_AEQUALIS_I32 (instructio_visa, IV);
            /* %t1 (registrum 0) = addens(20,1) = 21 iam computatum */
            CREDO_VERUM (machinula_registrum_legere(machinula,
                ZEPHYRUM, ZEPHYRUM, &valor));
            CREDO_AEQUALIS_I32 ((i32)valor, XXI);
            /* resumptio: transit punctum, finit 42 */
            CREDO_AEQUALIS_I32 ((i32)machinula_pergere(machinula),
                (i32)MACHINULA_BENE);
            CREDO_AEQUALIS_I32 ((i32)machinula_halitus_codex(
                machinula), XLII);
            CREDO_VERUM (machinula_punctum_tollere(machinula,
                index_main, IV));
        }

        /* --- punctum in instructione 0 --- */
        imprimere("\n--- Probans punctum in instructione 0 ---\n");
        {
            i64 ante;

            CREDO_VERUM (machinula_punctum_ponere(machinula,
                index_main, ZEPHYRUM));
            CREDO_VERUM (machinula_aperire(machinula, _ch("main")));
            ante = machinula_numerus_instructionum(machinula);
            CREDO_AEQUALIS_I32 ((i32)machinula_pergere(machinula),
                (i32)MACHINULA_PAUSA);
            /* NIHIL exsecutum ante pausam (anulus punctum videt,
             * numeratores quoque - sentinella numeratur, ops verae
             * nullae; delta = I sentinella) */
            CREDO_VERUM (machinula_numerus_instructionum(machinula)
                - ante <= I);
            CREDO_AEQUALIS_I32 ((i32)machinula_pergere(machinula),
                (i32)MACHINULA_BENE);
            CREDO_AEQUALIS_I32 ((i32)machinula_halitus_codex(
                machinula), XLII);
            CREDO_VERUM (machinula_punctum_tollere(machinula,
                index_main, ZEPHYRUM));
        }

        /* --- punctum in vocato (profunditas 2) --- */
        imprimere("\n--- Probans punctum in vocato ---\n");
        {
            s32 functio_visa = -I;
            i32 instructio_visa = (i32)-I;
            i64 valor = ZEPHYRUM;

            CREDO_VERUM (machinula_punctum_ponere(machinula,
                index_addens, ZEPHYRUM));
            CREDO_VERUM (machinula_aperire(machinula, _ch("main")));
            CREDO_AEQUALIS_I32 ((i32)machinula_pergere(machinula),
                (i32)MACHINULA_PAUSA);
            CREDO_AEQUALIS_I32 (machinula_tabulata_numerus(
                machinula), II);
            CREDO_VERUM (machinula_positionem_inspicere(machinula,
                I, &functio_visa, &instructio_visa));
            CREDO_AEQUALIS_I32 ((i32)functio_visa,
                (i32)index_addens);
            CREDO_AEQUALIS_I32 (instructio_visa, ZEPHYRUM);
            /* parametra iam canonicalizata: %a = 20 */
            CREDO_VERUM (machinula_registrum_legere(machinula, I,
                ZEPHYRUM, &valor));
            CREDO_AEQUALIS_I32 ((i32)valor, XX);
            /* tabulatum imum = main */
            CREDO_VERUM (machinula_positionem_inspicere(machinula,
                ZEPHYRUM, &functio_visa, NIHIL));
            CREDO_AEQUALIS_I32 ((i32)functio_visa, (i32)index_main);
            CREDO_AEQUALIS_I32 ((i32)machinula_pergere(machinula),
                (i32)MACHINULA_BENE);
            CREDO_AEQUALIS_I32 ((i32)machinula_halitus_codex(
                machinula), XLII);
            CREDO_VERUM (machinula_punctum_tollere(machinula,
                index_addens, ZEPHYRUM));
        }

        /* --- punctum per iter descriptoris (vocatio indirecta) --- */
        imprimere("\n--- Probans punctum per descriptorem ---\n");
        {
            CREDO_VERUM (machinula_punctum_ponere(machinula,
                index_geminare, ZEPHYRUM));
            CREDO_VERUM (machinula_aperire(machinula, _ch("main")));
            CREDO_AEQUALIS_I32 ((i32)machinula_pergere(machinula),
                (i32)MACHINULA_PAUSA);
            CREDO_AEQUALIS_I32 ((i32)machinula_pergere(machinula),
                (i32)MACHINULA_BENE);
            CREDO_AEQUALIS_I32 ((i32)machinula_halitus_codex(
                machinula), XLII);
            CREDO_VERUM (machinula_punctum_tollere(machinula,
                index_geminare, ZEPHYRUM));
        }

        /* --- tollere: cursus purus sine pausa --- */
        imprimere("\n--- Probans tollere ---\n");
        {
            MachinulaExitus fructus;

            CREDO_VERUM (machinula_punctum_ponere(machinula,
                index_main, IV));
            CREDO_VERUM (machinula_punctum_tollere(machinula,
                index_main, IV));
            CREDO_VERUM (!machinula_punctum_tollere(machinula,
                index_main, IV));   /* iam sublatum */
            fructus = machinula_currere(machinula, _ch("main"));
            CREDO_AEQUALIS_I32 ((i32)fructus.genus,
                (i32)MACHINULA_BENE);
            CREDO_AEQUALIS_I32 ((i32)fructus.codex, XLII);
        }

        /* --- fines insani reiecti --- */
        imprimere("\n--- Probans fines ---\n");
        CREDO_VERUM (!machinula_punctum_ponere(machinula, -I,
            ZEPHYRUM));
        CREDO_VERUM (!machinula_punctum_ponere(machinula,
            (s32)9999, ZEPHYRUM));
        CREDO_VERUM (!machinula_punctum_ponere(machinula,
            index_main, (i32)9999));

        regio_destruere(regio);
    }

    imprimere("\n");
    credo_imprimere_compendium();
    imprimere("========================================\n");
    successus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(piscina);
    redde successus ? ZEPHYRUM : I;
}
