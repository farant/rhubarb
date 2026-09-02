/* probatio_officina_medulla.c - Medulla: structurae + fabrica
 * (M1a Chunk B)
 *
 * Aedificat $summa (vocabularium-medullae.md §VIII exemplum)
 * per fabricam, probat disciplinam terminatorum, stivam
 * operandorum (vocare), symbola internata, data cum
 * relocationibus, nomina automatica registrorum. */

#include "officina_medulla.h"
#include "credo.h"

#include <stdio.h>
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

interior MedullaInstructio
_instructio (
                 s32 op,
                 s32 typus,
                 s32 destinatio,
    MedullaOperandum a,
    MedullaOperandum b)
{
    MedullaInstructio instructio;

    memset(&instructio, ZEPHYRUM, magnitudo(MedullaInstructio));
    instructio.op              = op;
    instructio.typus           = typus;
    instructio.typus_secundus  = MEDULLA_TYPUS_NIHIL;
    instructio.destinatio      = destinatio;
    instructio.extra_index     = -I;
    instructio.extra_numerus   = ZEPHYRUM;
    instructio.a               = a;
    instructio.b               = b;
    instructio.c               = medulla_op_nihil();
    instructio.origo           = NIHIL;
    redde instructio;
}

s32
principale (vacuum)
{
    Piscina* piscina;
        b32  praeteritus;

    piscina = piscina_generare_dynamicum("probatio_medulla",
        16777216);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ========================================================
     * PROBARE: $summa - exemplum vocabularii §VIII per fabricam
     * ======================================================== */

    {
        MedullaModulus* modulus = medulla_modulum_creare(piscina,
            _ch("probatio.c"));
           MedullaFunctio* functio;
                      s32  r_n;
                      s32  r_s;
                      s32  r_i;
                      s32  r_c;
                      s32  b_initium;
                      s32  b_proba;
                      s32  b_corpus;
                      s32  b_finis;
        MedullaInstructio  instructio;

        imprimere("\n--- Probans $summa (fabrica) ---\n");
        CREDO_NON_NIHIL (modulus);
        functio = medulla_functionem_creare(modulus, _ch("summa"),
            MEDULLA_TYPUS_S32, FALSUM);
        CREDO_NON_NIHIL (functio);

        r_n = medulla_parametrum_addere(functio, _ch("n"),
            MEDULLA_TYPUS_S32);
        CREDO_AEQUALIS_S32 (r_n, ZEPHYRUM);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(functio->parametra), I);

        r_s = medulla_registrum_novum(functio, _ch("s"));
        r_i = medulla_registrum_novum(functio, _ch("i"));
        r_c = medulla_registrum_novum(functio, _ch("c"));
        CREDO_AEQUALIS_S32 (r_c, III);

        b_initium  = medulla_bloccum_creare(functio, _ch("initium"));
        b_proba    = medulla_bloccum_creare(functio, _ch("proba"));
        b_corpus   = medulla_bloccum_creare(functio, _ch("corpus"));
        b_finis    = medulla_bloccum_creare(functio, _ch("finis"));
        CREDO_AEQUALIS_S32 (b_initium, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (b_finis, III);

        /* @initium: %s = movere.s32 0; %i = movere.s32 1;
         * salire @proba */
        instructio = _instructio(MEDULLA_OP_MOVERE,
            MEDULLA_TYPUS_S32, r_s, medulla_op_immediatum(0),
            medulla_op_nihil());
        CREDO_VERUM (medulla_emittere(functio, b_initium,
            &instructio));
        instructio = _instructio(MEDULLA_OP_MOVERE,
            MEDULLA_TYPUS_S32, r_i, medulla_op_immediatum(1),
            medulla_op_nihil());
        CREDO_VERUM (medulla_emittere(functio, b_initium,
            &instructio));
        instructio = _instructio(MEDULLA_OP_SALIRE,
            MEDULLA_TYPUS_NIHIL, -I, medulla_op_bloccum(b_proba),
            medulla_op_nihil());
        CREDO_VERUM (medulla_emittere(functio, b_initium,
            &instructio));

        /* @proba: %c = maior.s32 %i, %n;
         * ramus %c -> @finis | @corpus */
        instructio = _instructio(MEDULLA_OP_MAIOR,
            MEDULLA_TYPUS_S32, r_c, medulla_op_registrum(r_i),
            medulla_op_registrum(r_n));
        CREDO_VERUM (medulla_emittere(functio, b_proba,
            &instructio));
        instructio = _instructio(MEDULLA_OP_RAMUS,
            MEDULLA_TYPUS_NIHIL, -I, medulla_op_registrum(r_c),
            medulla_op_bloccum(b_finis));
        instructio.c = medulla_op_bloccum(b_corpus);
        CREDO_VERUM (medulla_emittere(functio, b_proba,
            &instructio));

        /* @corpus: %s = addere.s32 %s, %i;
         * %i = addere.s32 %i, 1; salire @proba */
        instructio = _instructio(MEDULLA_OP_ADDERE,
            MEDULLA_TYPUS_S32, r_s, medulla_op_registrum(r_s),
            medulla_op_registrum(r_i));
        CREDO_VERUM (medulla_emittere(functio, b_corpus,
            &instructio));
        instructio = _instructio(MEDULLA_OP_ADDERE,
            MEDULLA_TYPUS_S32, r_i, medulla_op_registrum(r_i),
            medulla_op_immediatum(1));
        CREDO_VERUM (medulla_emittere(functio, b_corpus,
            &instructio));
        instructio = _instructio(MEDULLA_OP_SALIRE,
            MEDULLA_TYPUS_NIHIL, -I, medulla_op_bloccum(b_proba),
            medulla_op_nihil());
        CREDO_VERUM (medulla_emittere(functio, b_corpus,
            &instructio));

        /* @finis: redde %s */
        CREDO_VERUM (!medulla_functio_terminata(functio));
        instructio = _instructio(MEDULLA_OP_REDDE,
            MEDULLA_TYPUS_NIHIL, -I, medulla_op_registrum(r_s),
            medulla_op_nihil());
        CREDO_VERUM (medulla_emittere(functio, b_finis,
            &instructio));
        CREDO_VERUM (medulla_functio_terminata(functio));

        /* figura: IV blocci; instructiones III/II/III/I */
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(functio->blocci), IV);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(
            medulla_bloccum_obtinere(functio,
                b_initium)->instructiones), III);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(
            medulla_bloccum_obtinere(functio,
                b_finis)->instructiones), I);

        /* contenta: instructio prima @initium */
        {
            constans MedullaBloccus* bloccus =
                medulla_bloccum_obtinere(functio, b_initium);
            constans MedullaInstructio* prima =
                (constans MedullaInstructio*)xar_obtinere(
                    bloccus->instructiones, ZEPHYRUM);

            CREDO_NON_NIHIL (prima);
            CREDO_AEQUALIS_S32 (prima->op, MEDULLA_OP_MOVERE);
            CREDO_AEQUALIS_S32 (prima->typus, MEDULLA_TYPUS_S32);
            CREDO_AEQUALIS_S32 (prima->destinatio, r_s);
            CREDO_AEQUALIS_S32 (prima->a.genus,
                MEDULLA_OPERANDUM_IMMEDIATUM);
            CREDO_AEQUALIS_S64 (prima->a.datum.immediatum, 0);
        }

        /* tituli registrorum servati */
        {
            constans chorda* titulus = medulla_registrum_titulus(
                functio, r_i);

            CREDO_NON_NIHIL (titulus);
            CREDO_CHORDA_AEQUALIS_LITERIS (*titulus, "i");
        }

        /* disciplina: emissio in bloccum terminatum FALSUM;
         * bloccus ignotus FALSUM; op extra fines FALSUM;
         * registrum ignotum FALSUM */
        instructio = _instructio(MEDULLA_OP_MOVERE,
            MEDULLA_TYPUS_S32, r_s, medulla_op_immediatum(7),
            medulla_op_nihil());
        CREDO_VERUM (!medulla_emittere(functio, b_finis,
            &instructio));
        CREDO_VERUM (!medulla_emittere(functio, XCIX,
            &instructio));
        instructio.op = (s32)MEDULLA_OP_NUMERUS;
        CREDO_VERUM (!medulla_emittere(functio, b_initium,
            &instructio));
        instructio.op          = MEDULLA_OP_MOVERE;
        instructio.destinatio  = XCIX;
        CREDO_VERUM (!medulla_emittere(functio, b_initium,
            &instructio));

        /* terminator quaestio */
        CREDO_VERUM (medulla_op_terminator(MEDULLA_OP_REDDE));
        CREDO_VERUM (medulla_op_terminator(MEDULLA_OP_SISTERE));
        CREDO_VERUM (!medulla_op_terminator(MEDULLA_OP_VOCARE));
        CREDO_VERUM (!medulla_op_terminator(MEDULLA_OP_ADDERE));
    }


    /* ========================================================
     * PROBARE: symbola internata + vocare cum stiva + sistere
     * ======================================================== */

    {
        MedullaModulus* modulus = medulla_modulum_creare(piscina,
            _ch("probatio2.c"));
           MedullaFunctio* functio;
                      s32  s_impressio;
                      s32  s_iterum;
                      s32  b_solum;
                      s32  causa;
        MedullaInstructio  instructio;

        imprimere("\n--- Probans symbola + vocare + sistere ---\n");
        CREDO_NON_NIHIL (modulus);

        /* internare: idem titulus -> idem index; EXTERNUM initio */
        s_impressio = medulla_symbolum_internare(modulus,
            _ch("imprimere"));
        s_iterum = medulla_symbolum_internare(modulus,
            _ch("imprimere"));
        CREDO_AEQUALIS_S32 (s_impressio, s_iterum);
        CREDO_AEQUALIS_S32 (medulla_symbolum_obtinere(modulus,
            s_impressio)->genus, MEDULLA_SYMBOLUM_EXTERNUM);

        /* functionem_creare genus concretum ponit; duplex NIHIL */
        functio = medulla_functionem_creare(modulus, _ch("probare"),
            MEDULLA_TYPUS_NIHIL, FALSUM);
        CREDO_NON_NIHIL (functio);
        CREDO_NIHIL (medulla_functionem_creare(modulus,
            _ch("probare"), MEDULLA_TYPUS_NIHIL, FALSUM));
        {
            s32 s_probare = medulla_symbolum_internare(modulus,
                _ch("probare"));
            constans MedullaSymbolum* symbolum =
                medulla_symbolum_obtinere(modulus, s_probare);

            CREDO_AEQUALIS_S32 (symbolum->genus,
                MEDULLA_SYMBOLUM_FUNCTIO);
            CREDO_AEQUALIS_S32 (symbolum->index, ZEPHYRUM);
        }

        b_solum = medulla_bloccum_creare(functio, _ch("initium"));

        /* vocare $imprimere, args in stiva */
        {
            MedullaOperandum argumenta[II];
                         s32 initium_stivae;

            argumenta[ZEPHYRUM]  = medulla_op_immediatum(42);
            argumenta[I]         = medulla_op_immediatum_f(1.5);
            initium_stivae = medulla_operanda_addere(functio,
                argumenta, II);
            CREDO_AEQUALIS_S32 (initium_stivae, ZEPHYRUM);

            instructio = _instructio(MEDULLA_OP_VOCARE,
                MEDULLA_TYPUS_S32, -I,
                medulla_op_symbolum(s_impressio),
                medulla_op_nihil());
            instructio.extra_index    = initium_stivae;
            instructio.extra_numerus  = II;
            CREDO_VERUM (medulla_emittere(functio, b_solum,
                &instructio));
        }
        /* stiva legitur recte */
        {
            constans MedullaOperandum* argumentum =
                (constans MedullaOperandum*)xar_obtinere(
                    functio->operanda, I);

            CREDO_NON_NIHIL (argumentum);
            CREDO_AEQUALIS_S32 (argumentum->genus,
                MEDULLA_OPERANDUM_IMMEDIATUM_F);
            CREDO_VERUM (argumentum->datum.immediatum_f == 1.5);
        }

        /* sistere cum causa internata (terminator) */
        causa = medulla_causam_internare(modulus,
            _ch("accessus structurae incompletae"));
        CREDO_AEQUALIS_S32 (causa, medulla_causam_internare(modulus,
            _ch("accessus structurae incompletae")));
        instructio = _instructio(MEDULLA_OP_SISTERE,
            MEDULLA_TYPUS_NIHIL, -I, medulla_op_causa(causa),
            medulla_op_nihil());
        CREDO_VERUM (medulla_emittere(functio, b_solum,
            &instructio));
        CREDO_VERUM (medulla_functio_terminata(functio));
        CREDO_CHORDA_AEQUALIS_LITERIS (
            *medulla_causam_obtinere(modulus, causa),
            "accessus structurae incompletae");

        /* nomen automaticum registri */
        {
            s32 r_t = medulla_registrum_novum(functio, _ch(""));
            constans chorda* titulus = medulla_registrum_titulus(
                functio, r_t);

            CREDO_AEQUALIS_S32 (r_t, ZEPHYRUM);
            CREDO_NON_NIHIL (titulus);
            CREDO_CHORDA_AEQUALIS_LITERIS (*titulus, "t0");
        }
    }


    /* ========================================================
     * PROBARE: data + imago + relocationes (C8)
     * ======================================================== */

    {
        MedullaModulus* modulus = medulla_modulum_creare(piscina,
            _ch("probatio3.c"));
        MedullaDatum* datum;
                 s32  s_finis;

        imprimere("\n--- Probans data + relocationes ---\n");
        CREDO_NON_NIHIL (modulus);

        datum = medulla_datum_creare(modulus, _ch("tabula"), XVI,
            VIII);
        CREDO_NON_NIHIL (datum);
        CREDO_AEQUALIS_I32 (datum->magnitudo_octetorum, XVI);
        /* imago zephyrum-plena initio */
        CREDO_AEQUALIS_I32 ((i32)datum->imago[ZEPHYRUM], ZEPHYRUM);
        CREDO_AEQUALIS_I32 ((i32)datum->imago[XV], ZEPHYRUM);

        /* scribere octeti; extra fines FALSUM */
        {
            interior constans i8 octeti[IV] = { 1, 2, 3, 4 };

            CREDO_VERUM (medulla_datum_scribere(datum, IV, octeti,
                IV));
            CREDO_AEQUALIS_I32 ((i32)datum->imago[IV], I);
            CREDO_AEQUALIS_I32 ((i32)datum->imago[VII], IV);
            CREDO_VERUM (!medulla_datum_scribere(datum, XIV, octeti,
                IV));
        }

        /* relocatio: locellus 8 octetorum intra imaginem */
        s_finis = medulla_symbolum_internare(modulus,
            _ch("functio_externa"));
        CREDO_VERUM (medulla_relocationem_addere(datum, VIII,
            s_finis));
        CREDO_VERUM (!medulla_relocationem_addere(datum, XII,
            s_finis));
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(datum->relocationes),
            I);
        {
            constans MedullaRelocatio* relocatio =
                (constans MedullaRelocatio*)xar_obtinere(
                    datum->relocationes, ZEPHYRUM);

            CREDO_AEQUALIS_I32 (relocatio->offset, VIII);
            CREDO_AEQUALIS_S32 (relocatio->symbolum, s_finis);
        }

        /* definitio duplex dati NIHIL; magnitudo invalida NIHIL */
        CREDO_NIHIL (medulla_datum_creare(modulus, _ch("tabula"),
            VIII, IV));
        CREDO_NIHIL (medulla_datum_creare(modulus, _ch("vacua"),
            ZEPHYRUM, IV));
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
