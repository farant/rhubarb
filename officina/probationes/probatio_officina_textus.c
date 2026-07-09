/* probatio_officina_textus.c - Forma textualis: VECTIS M1a
 * (dump(parse(t)) == t byte-idem pro plagulis canonicis;
 * idempotentia pro commentatis; errores cum linea) */

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

interior MedullaInstructio
_instructio (s32 op, s32 typus, s32 destinatio,
    MedullaOperandum a, MedullaOperandum b)
{
    MedullaInstructio instructio;

    memset(&instructio, ZEPHYRUM, magnitudo(MedullaInstructio));
    instructio.op = op;
    instructio.typus = typus;
    instructio.typus_secundus = MEDULLA_TYPUS_NIHIL;
    instructio.destinatio = destinatio;
    instructio.extra_index = -I;
    instructio.a = a;
    instructio.b = b;
    instructio.c = medulla_op_nihil();
    redde instructio;
}

/* $summa per fabricam - eadem figura ac vocabularium §VIII */
interior MedullaModulus*
_summam_aedificare (Piscina* piscina)
{
    MedullaModulus* modulus = medulla_modulum_creare(piscina,
        _ch("probatio.c"));
    MedullaFunctio* functio = medulla_functionem_creare(modulus,
        _ch("summa"), MEDULLA_TYPUS_S32, FALSUM);
    s32 r_n = medulla_parametrum_addere(functio, _ch("n"),
        MEDULLA_TYPUS_S32);
    s32 r_s = medulla_registrum_novum(functio, _ch("s"));
    s32 r_i = medulla_registrum_novum(functio, _ch("i"));
    s32 r_c = medulla_registrum_novum(functio, _ch("c"));
    s32 b_initium = medulla_bloccum_creare(functio, _ch("initium"));
    s32 b_proba = medulla_bloccum_creare(functio, _ch("proba"));
    s32 b_corpus = medulla_bloccum_creare(functio, _ch("corpus"));
    s32 b_finis = medulla_bloccum_creare(functio, _ch("finis"));
    MedullaInstructio instructio;

    instructio = _instructio(MEDULLA_OP_MOVERE, MEDULLA_TYPUS_S32,
        r_s, medulla_op_immediatum(0), medulla_op_nihil());
    medulla_emittere(functio, b_initium, &instructio);
    instructio = _instructio(MEDULLA_OP_MOVERE, MEDULLA_TYPUS_S32,
        r_i, medulla_op_immediatum(1), medulla_op_nihil());
    medulla_emittere(functio, b_initium, &instructio);
    instructio = _instructio(MEDULLA_OP_SALIRE, MEDULLA_TYPUS_NIHIL,
        -I, medulla_op_bloccum(b_proba), medulla_op_nihil());
    medulla_emittere(functio, b_initium, &instructio);

    instructio = _instructio(MEDULLA_OP_MAIOR, MEDULLA_TYPUS_S32,
        r_c, medulla_op_registrum(r_i), medulla_op_registrum(r_n));
    medulla_emittere(functio, b_proba, &instructio);
    instructio = _instructio(MEDULLA_OP_RAMUS, MEDULLA_TYPUS_NIHIL,
        -I, medulla_op_registrum(r_c), medulla_op_bloccum(b_finis));
    instructio.c = medulla_op_bloccum(b_corpus);
    medulla_emittere(functio, b_proba, &instructio);

    instructio = _instructio(MEDULLA_OP_ADDERE, MEDULLA_TYPUS_S32,
        r_s, medulla_op_registrum(r_s), medulla_op_registrum(r_i));
    medulla_emittere(functio, b_corpus, &instructio);
    instructio = _instructio(MEDULLA_OP_ADDERE, MEDULLA_TYPUS_S32,
        r_i, medulla_op_registrum(r_i), medulla_op_immediatum(1));
    medulla_emittere(functio, b_corpus, &instructio);
    instructio = _instructio(MEDULLA_OP_SALIRE, MEDULLA_TYPUS_NIHIL,
        -I, medulla_op_bloccum(b_proba), medulla_op_nihil());
    medulla_emittere(functio, b_corpus, &instructio);

    instructio = _instructio(MEDULLA_OP_REDDE, MEDULLA_TYPUS_NIHIL,
        -I, medulla_op_registrum(r_s), medulla_op_nihil());
    medulla_emittere(functio, b_finis, &instructio);
    redde modulus;
}

s32 principale (vacuum)
{
    Piscina* piscina;
    b32 praeteritus;

    piscina = piscina_generare_dynamicum("probatio_textus",
        16777216);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);

    /* ========================================================
     * PROBARE: nomina signata
     * ======================================================== */
    {
        imprimere("\n--- Probans nomina signata ---\n");
        CREDO_VERUM (strcmp(medulla_op_titulus(MEDULLA_OP_AUT),
            "aut") == ZEPHYRUM);
        CREDO_VERUM (strcmp(medulla_op_titulus(MEDULLA_OP_SISTERE),
            "sistere") == ZEPHYRUM);
        CREDO_VERUM (strcmp(medulla_typum_titulus(MEDULLA_TYPUS_S32),
            "s32") == ZEPHYRUM);
        CREDO_NIHIL ((constans vacuum*)medulla_op_titulus(
            (s32)MEDULLA_OP_NUMERUS));
        CREDO_NIHIL ((constans vacuum*)medulla_typum_titulus(-I));
    }

    /* ========================================================
     * PROBARE: fabrica -> scriptura == plagula canonica
     * (SCRIPTOR contra plagulam commissam fixus)
     * ======================================================== */
    {
        MedullaModulus* modulus = _summam_aedificare(piscina);
        chorda scriptura = medulla_textum_scribere(piscina,
            modulus);
        chorda plagula = _plagulam_legere(piscina,
            "officina/probationes/fixa/summa.medulla");

        imprimere("\n--- Probans scriptorem ($summa) ---\n");
        CREDO_NON_NIHIL (modulus);
        CREDO_VERUM (scriptura.mensura > ZEPHYRUM);
        CREDO_VERUM (plagula.mensura > ZEPHYRUM);
        CREDO_CHORDA_AEQUALIS (scriptura, plagula);
    }

    /* ========================================================
     * PROBARE: VECTIS - dump(parse(t)) == t (canonicae)
     * ======================================================== */
    {
        constans character* plagulae[II];
        i32 i;

        plagulae[ZEPHYRUM] =
            "officina/probationes/fixa/summa.medulla";
        plagulae[I] = "officina/probationes/fixa/omnia.medulla";
        imprimere("\n--- Probans vectem (byte-idem) ---\n");
        per (i = ZEPHYRUM; i < II; i++)
        {
            chorda plagula = _plagulam_legere(piscina, plagulae[i]);
            i32 linea = ZEPHYRUM;
            MedullaModulus* modulus = medulla_textum_legere(piscina,
                plagula, &linea);
            chorda scriptura;

            CREDO_VERUM (plagula.mensura > ZEPHYRUM);
            CREDO_NON_NIHIL (modulus);
            CREDO_AEQUALIS_I32 ((i32)linea, ZEPHYRUM);
            scriptura = medulla_textum_scribere(piscina, modulus);
            CREDO_CHORDA_AEQUALIS (scriptura, plagula);
        }
    }

    /* ========================================================
     * PROBARE: commenta praetermissa + idempotentia
     * ======================================================== */
    {
        chorda plagula = _plagulam_legere(piscina,
            "officina/probationes/fixa/commenta.medulla");
        MedullaModulus* modulus = medulla_textum_legere(piscina,
            plagula, NIHIL);
        chorda prima;
        chorda secunda;
        MedullaModulus* iterum;

        imprimere("\n--- Probans commenta + idempotentiam ---\n");
        CREDO_NON_NIHIL (modulus);
        /* octeti partiales: imago[0]=1, reliqua zephyrum */
        {
            MedullaDatum** datum = (MedullaDatum**)xar_obtinere(
                modulus->data, ZEPHYRUM);

            CREDO_NON_NIHIL (datum);
            CREDO_AEQUALIS_I32 ((i32)(*datum)->imago[ZEPHYRUM], I);
            CREDO_AEQUALIS_I32 ((i32)(*datum)->imago[XV],
                ZEPHYRUM);
        }
        prima = medulla_textum_scribere(piscina, modulus);
        iterum = medulla_textum_legere(piscina, prima, NIHIL);
        CREDO_NON_NIHIL (iterum);
        secunda = medulla_textum_scribere(piscina, iterum);
        CREDO_CHORDA_AEQUALIS (prima, secunda);
    }

    /* ========================================================
     * PROBARE: fluitantia bit-exacta per vectem
     * ======================================================== */
    {
        MedullaModulus* modulus = medulla_modulum_creare(piscina,
            _ch("fluitans.c"));
        MedullaFunctio* functio = medulla_functionem_creare(modulus,
            _ch("f"), MEDULLA_TYPUS_F64, FALSUM);
        s32 r_x = medulla_registrum_novum(functio, _ch("x"));
        s32 b = medulla_bloccum_creare(functio, _ch("initium"));
        MedullaInstructio instructio;
        chorda scriptura;
        MedullaModulus* iterum;

        imprimere("\n--- Probans fluitantia (%%.17g) ---\n");
        instructio = _instructio(MEDULLA_OP_MOVERE,
            MEDULLA_TYPUS_F64, r_x, medulla_op_immediatum_f(0.1),
            medulla_op_nihil());
        CREDO_VERUM (medulla_emittere(functio, b, &instructio));
        instructio = _instructio(MEDULLA_OP_REDDE,
            MEDULLA_TYPUS_NIHIL, -I, medulla_op_registrum(r_x),
            medulla_op_nihil());
        CREDO_VERUM (medulla_emittere(functio, b, &instructio));

        scriptura = medulla_textum_scribere(piscina, modulus);
        iterum = medulla_textum_legere(piscina, scriptura, NIHIL);
        CREDO_NON_NIHIL (iterum);
        {
            MedullaFunctio** f2 = (MedullaFunctio**)xar_obtinere(
                iterum->functiones, ZEPHYRUM);
            constans MedullaBloccus* b2 = medulla_bloccum_obtinere(
                *f2, ZEPHYRUM);
            constans MedullaInstructio* prima =
                (constans MedullaInstructio*)xar_obtinere(
                    b2->instructiones, ZEPHYRUM);

            CREDO_VERUM (prima->a.datum.immediatum_f == 0.1);
        }
        /* et byte-idem circulus secundus */
        CREDO_CHORDA_AEQUALIS (scriptura,
            medulla_textum_scribere(piscina, iterum));
    }

    /* ========================================================
     * PROBARE: errores cum linea (strictus)
     * ======================================================== */
    {
        i32 linea;

        imprimere("\n--- Probans errores lectoris ---\n");
        /* op ignotum in linea 4 */
        CREDO_NIHIL (medulla_textum_legere(piscina, _ch(
            "modulus \"x\"\n"
            "functio $f () -> s32\n"
            "@a:\n"
            "    ignotum %q, 1\n"), &linea));
        CREDO_AEQUALIS_I32 ((i32)linea, IV);
        /* bloccus ignotus */
        CREDO_NIHIL (medulla_textum_legere(piscina, _ch(
            "modulus \"x\"\n"
            "functio $f () -> s32\n"
            "@a:\n"
            "    salire @absens\n"), &linea));
        CREDO_AEQUALIS_I32 ((i32)linea, IV);
        /* instructio post terminatorem (disciplina fabricae) */
        CREDO_NIHIL (medulla_textum_legere(piscina, _ch(
            "modulus \"x\"\n"
            "functio $f () -> vacuum\n"
            "@a:\n"
            "    redde\n"
            "    redde\n"), &linea));
        CREDO_AEQUALIS_I32 ((i32)linea, V);
        /* reliquiae post instructionem */
        CREDO_NIHIL (medulla_textum_legere(piscina, _ch(
            "modulus \"x\"\n"
            "functio $f () -> vacuum\n"
            "@a:\n"
            "    redde iterum atque iterum\n"), &linea));
        /* linea sine modulo */
        CREDO_NIHIL (medulla_textum_legere(piscina, _ch(
            "functio $f () -> vacuum\n"), &linea));
        CREDO_AEQUALIS_I32 ((i32)linea, I);
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    redde I;
}
