/* probatio_officina_textus.c - Forma textualis: VECTIS M1a
 * (dump(parse(t)) == t byte-idem pro plagulis canonicis;
 * idempotentia pro commentatis; errores cum linea) */

#include "officina_medulla_textus.h"
#include "credo.h"

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
    instructio.argumenta_fixa  = -I;
    instructio.a               = a;
    instructio.b               = b;
    instructio.c               = medulla_op_nihil();
    redde instructio;
}

/* $summa per fabricam - eadem figura ac vocabularium §VIII */
interior MedullaModulus*
_summam_aedificare (
    Piscina* piscina)
{
    MedullaModulus* modulus = medulla_modulum_creare(piscina,
        _ch("probatio.c"));
    MedullaFunctio* functio = medulla_functionem_creare(modulus,
        _ch("summa"), MEDULLA_TYPUS_S32, FALSUM);
    s32 r_n = medulla_parametrum_addere(functio, _ch("n"),
        MEDULLA_TYPUS_S32);
    s32 r_s        = medulla_registrum_novum(functio, _ch("s"));
    s32 r_i        = medulla_registrum_novum(functio, _ch("i"));
    s32 r_c        = medulla_registrum_novum(functio, _ch("c"));
    s32 b_initium  = medulla_bloccum_creare(functio, _ch("initium"));
    s32 b_proba    = medulla_bloccum_creare(functio, _ch("proba"));
    s32 b_corpus   = medulla_bloccum_creare(functio, _ch("corpus"));
    s32 b_finis    = medulla_bloccum_creare(functio, _ch("finis"));
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


/* ==================================================
 * PROBARE: nomina signata
 * ================================================== */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    (vacuum)c;
    {
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
}


/* ==================================================
 * PROBARE: fabrica -> scriptura == plagula canonica
 * (SCRIPTOR contra plagulam commissam fixus)
 * ================================================== */

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        MedullaModulus* modulus = _summam_aedificare(piscina);
                chorda  scriptura = medulla_textum_scribere(piscina,
                    modulus);
        chorda plagula = _plagulam_legere(piscina,
            "officina/probationes/fixa/summa.medulla");

        CREDO_NON_NIHIL (modulus);
        CREDO_VERUM (scriptura.mensura > ZEPHYRUM);
        CREDO_VERUM (plagula.mensura > ZEPHYRUM);
        CREDO_CHORDA_AEQUALIS (scriptura, plagula);
    }
}


/* ==================================================
 * PROBARE: VECTIS - dump(parse(t)) == t (canonicae)
 * ================================================== */

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* plagulae[II];
                       i32  i;

        plagulae[ZEPHYRUM] =
            "officina/probationes/fixa/summa.medulla";
        plagulae[I] = "officina/probationes/fixa/omnia.medulla";
        per (i = ZEPHYRUM; i < II; i++)
        {
            chorda plagula  = _plagulam_legere(piscina, plagulae[i]);
            i32 linea       = ZEPHYRUM;
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
}


/* ==================================================
 * PROBARE: commenta praetermissa + idempotentia
 * ================================================== */

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda plagula = _plagulam_legere(piscina,
            "officina/probationes/fixa/commenta.medulla");
        MedullaModulus* modulus = medulla_textum_legere(piscina,
            plagula, NIHIL);
                chorda prima;
                chorda secunda;
        MedullaModulus* iterum;

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
        prima   = medulla_textum_scribere(piscina, modulus);
        iterum  = medulla_textum_legere(piscina, prima, NIHIL);
        CREDO_NON_NIHIL (iterum);
        secunda = medulla_textum_scribere(piscina, iterum);
        CREDO_CHORDA_AEQUALIS (prima, secunda);
    }
}


/* ==================================================
 * PROBARE: fluitantia bit-exacta per vectem
 * ================================================== */

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        MedullaModulus* modulus = medulla_modulum_creare(piscina,
            _ch("fluitans.c"));
        MedullaFunctio* functio = medulla_functionem_creare(modulus,
            _ch("f"), MEDULLA_TYPUS_F64, FALSUM);
        s32 r_x  = medulla_registrum_novum(functio, _ch("x"));
        s32 b    = medulla_bloccum_creare(functio, _ch("initium"));
        MedullaInstructio instructio;
        chorda scriptura;
        MedullaModulus* iterum;

        instructio = _instructio(MEDULLA_OP_MOVERE,
            MEDULLA_TYPUS_F64, r_x, medulla_op_immediatum_f(0.1),
            medulla_op_nihil());
        CREDO_VERUM (medulla_emittere(functio, b, &instructio));
        instructio = _instructio(MEDULLA_OP_REDDE,
            MEDULLA_TYPUS_NIHIL, -I, medulla_op_registrum(r_x),
            medulla_op_nihil());
        CREDO_VERUM (medulla_emittere(functio, b, &instructio));

        scriptura  = medulla_textum_scribere(piscina, modulus);
        iterum     = medulla_textum_legere(piscina, scriptura, NIHIL);
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
}


/* ==================================================
 * PROBARE: errores cum linea (strictus)
 * ================================================== */

interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        i32 linea;

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
}


/* ==================================================
 * PROBARE: vocare typata (M5 B4) - argumenta typata, ' ... '
 * fixa a variadicis separat, externum signaturam fert
 * ================================================== */

interior constans MedullaFunctio*
_functionem_invenire (
    constans MedullaModulus* modulus,
         constans character* titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(modulus->functiones); i++)
    {
        MedullaFunctio** functio = (MedullaFunctio**)xar_obtinere(
            modulus->functiones, i);

        si (chorda_aequalis_literis((*functio)->titulus, titulus))
        {
            redde *functio;
        }
    }
    redde NIHIL;
}

interior constans MedullaSignatura*
_signaturam_invenire (
    constans MedullaModulus* modulus,
         constans character* titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(modulus->symbola); i++)
    {
        constans MedullaSymbolum* symbolum =
            medulla_symbolum_obtinere(modulus, (s32)i);

        si (   chorda_aequalis_literis(symbolum->titulus, titulus)
            && symbolum->signatura >= ZEPHYRUM)
        {
            redde (constans MedullaSignatura*)xar_obtinere(
                modulus->signaturae, (i32)symbolum->signatura);
        }
    }
    redde NIHIL;
}

/* lineae -> chorda una in piscina (litterae > DIX characteres C89
 * vetat) */
interior chorda
_lineas_iungere (
                Piscina*  piscina,
     constans character** lineae,
                    i32   numerus)
{
    chorda fructus;
       i32 summa = ZEPHYRUM;
       i32 i;

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        summa += (i32)strlen(lineae[i]);
    }
    fructus.datum = (i8*)piscina_allocare(piscina,
        (memoriae_index)summa + I);
    fructus.mensura = ZEPHYRUM;
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        i32 longitudo = (i32)strlen(lineae[i]);

        memcpy(fructus.datum + fructus.mensura, lineae[i],
            (memoriae_index)longitudo);
        fructus.mensura += longitudo;
    }
    redde fructus;
}

interior vacuum
_sectio_typata (
    CredoContextus* c)
{
               Piscina* piscina = c->piscina;
    constans character* lineae[] = {
        "modulus \"typata.c\"\n",
        "externum $externa (s32, f64) -> s32\n",
        "externum $scribens (s32, ...) -> s32\n",
        "externum $facere (r8, s32, s32) -> vacuum\n",
        "externum $printf (i64, ...) -> s32\n",
        "externum $vectores (rh2xf64, h2xf64, h4xf32, a24) -> vacuum\n",
        "\n",
        "functio $duplicare (r8 %reditus, a8 %p) -> vacuum\n",
        "@initium:\n",
        "    copia %reditus, %p, 8\n",
        "    redde\n",
        "\n",
        "functio $voca (s32 %n) -> s32\n",
        "@initium:\n",
        "    %t1 = addere.s32 %n, 1\n",
        "    %t2 = vocare.s32 $externa, s32 %t1, f64 2.5\n",
        "    %t3 = vocare.s32 $scribens, s32 2 ... s32 %n, s64 7\n",
        "    %t4 = arca 8, 4\n",
        "    vocare $facere, r8 %t4, s32 %n, s32 %t1\n",
        "    %t5 = vocare.s32 $printf, i64 $forma ...\n",
        "    vocare $duplicare, r8 %t4, a8 %t4\n",
        "    redde %t2\n"
    };
             chorda canon = _lineas_iungere(piscina, lineae,
                 (i32)(magnitudo(lineae)
                     / magnitudo(lineae[ZEPHYRUM])));
                i32  linea = ZEPHYRUM;
     MedullaModulus* modulus = medulla_textum_legere(piscina, canon,
         &linea);

    CREDO_NON_NIHIL (modulus);
    CREDO_AEQUALIS_I32 ((i32)linea, ZEPHYRUM);
    si (modulus == NIHIL)
    {
        redde;
    }
    /* vectis: dump(parse(t)) == t */
    CREDO_CHORDA_AEQUALIS (medulla_textum_scribere(piscina, modulus),
        canon);

    /* structura: argumenta_fixa et typi in stiva parallela */
    {
        constans MedullaFunctio* voca = _functionem_invenire(modulus,
            "voca");
             constans MedullaBloccus* bloccus;
          constans MedullaInstructio* externa;
          constans MedullaInstructio* scribens;
        constans MedullaTypusLiminis* typus;

        CREDO_NON_NIHIL ((constans vacuum*)voca);
        si (voca == NIHIL)
        {
            redde;
        }
        bloccus  = medulla_bloccum_obtinere(voca, ZEPHYRUM);
        externa  = (constans MedullaInstructio*)xar_obtinere(
            bloccus->instructiones, I);
        scribens = (constans MedullaInstructio*)xar_obtinere(
            bloccus->instructiones, II);
        CREDO_AEQUALIS_S32 (externa->argumenta_fixa, -I);
        CREDO_AEQUALIS_S32 (scribens->argumenta_fixa, I);
        CREDO_AEQUALIS_S32 (scribens->extra_numerus, III);
        typus = (constans MedullaTypusLiminis*)xar_obtinere(
            voca->operanda_typi,
            (i32)(scribens->extra_index + II));
        CREDO_NON_NIHIL ((constans vacuum*)typus);
        CREDO_AEQUALIS_S32 (typus->typus, MEDULLA_TYPUS_S64);
        typus = (constans MedullaTypusLiminis*)xar_obtinere(
            voca->operanda_typi,
            (i32)(externa->extra_index + I));
        CREDO_AEQUALIS_S32 (typus->typus, MEDULLA_TYPUS_F64);
    }
    /* parametra aggregata: typus registri i64, limen aggregatum */
    {
        constans MedullaFunctio* duplicare = _functionem_invenire(
            modulus, "duplicare");
        constans MedullaParametrum* reditus;
        constans MedullaParametrum* p;

        CREDO_NON_NIHIL ((constans vacuum*)duplicare);
        si (duplicare == NIHIL)
        {
            redde;
        }
        reditus = (constans MedullaParametrum*)xar_obtinere(
            duplicare->parametra, ZEPHYRUM);
        p = (constans MedullaParametrum*)xar_obtinere(
            duplicare->parametra, I);
        CREDO_AEQUALIS_S32 (reditus->typus, MEDULLA_TYPUS_I64);
        CREDO_VERUM (reditus->limen.est_reditus);
        CREDO_AEQUALIS_S32 (p->typus, MEDULLA_TYPUS_I64);
        CREDO_AEQUALIS_S32 (p->limen.typus, MEDULLA_TYPUS_AGGREGATUM);
        CREDO_AEQUALIS_I32 (p->limen.magnitudo_octetorum, VIII);
        CREDO_AEQUALIS_S32 (p->limen.hfa_typus, MEDULLA_TYPUS_NIHIL);
        CREDO_FALSUM (p->limen.est_reditus);
    }
    /* signaturae externorum */
    {
        constans MedullaSignatura* facere = _signaturam_invenire(
            modulus, "facere");
        constans MedullaSignatura* vectores = _signaturam_invenire(
            modulus, "vectores");
        constans MedullaSignatura* printf_s = _signaturam_invenire(
            modulus, "printf");
        constans MedullaTypusLiminis* limen;

        CREDO_NON_NIHIL ((constans vacuum*)facere);
        CREDO_NON_NIHIL ((constans vacuum*)vectores);
        CREDO_NON_NIHIL ((constans vacuum*)printf_s);
        si (facere == NIHIL || vectores == NIHIL || printf_s == NIHIL)
        {
            redde;
        }
        CREDO_AEQUALIS_S32 (facere->typus_reditus,
            MEDULLA_TYPUS_NIHIL);
        CREDO_FALSUM (facere->est_variadica);
        CREDO_AEQUALIS_I32 (xar_numerus(facere->parametra), III);
        limen = (constans MedullaTypusLiminis*)xar_obtinere(
            facere->parametra, ZEPHYRUM);
        CREDO_VERUM (limen->est_reditus);
        CREDO_AEQUALIS_I32 (limen->magnitudo_octetorum, VIII);
        CREDO_VERUM (printf_s->est_variadica);
        CREDO_AEQUALIS_I32 (xar_numerus(printf_s->parametra), I);
        limen = (constans MedullaTypusLiminis*)xar_obtinere(
            vectores->parametra, ZEPHYRUM);
        CREDO_VERUM (limen->est_reditus);
        CREDO_AEQUALIS_S32 (limen->hfa_typus, MEDULLA_TYPUS_F64);
        CREDO_AEQUALIS_I32 (limen->magnitudo_octetorum, XVI);
        limen = (constans MedullaTypusLiminis*)xar_obtinere(
            vectores->parametra, II);
        CREDO_AEQUALIS_S32 (limen->hfa_typus, MEDULLA_TYPUS_F32);
        CREDO_AEQUALIS_I32 (limen->magnitudo_octetorum, XVI);
        limen = (constans MedullaTypusLiminis*)xar_obtinere(
            vectores->parametra, III);
        CREDO_AEQUALIS_S32 (limen->typus, MEDULLA_TYPUS_AGGREGATUM);
        CREDO_AEQUALIS_I32 (limen->magnitudo_octetorum, XXIV);
        CREDO_AEQUALIS_S32 (limen->hfa_typus, MEDULLA_TYPUS_NIHIL);
    }
}

/* lector strictus: argumentum sine typo, typi mali, externum post
 * functionem - omnes recusati cum linea */
interior vacuum
_sectio_typata_errores (
    CredoContextus* c)
{
     Piscina* piscina = c->piscina;
         i32  linea;

    /* argumentum sine typo (forma ante M5) */
    CREDO_NIHIL (medulla_textum_legere(piscina, _ch(
        "modulus \"x\"\n"
        "functio $g (s32 %t1) -> s32\n"
        "@a:\n"
        "    %t2 = vocare.s32 $f, %t1\n"
        "    redde %t2\n"), &linea));
    CREDO_AEQUALIS_I32 ((i32)linea, IV);
    /* HFA membra > IV; HFA non fluitans; aggregatum vacuum */
    CREDO_NIHIL (medulla_textum_legere(piscina, _ch(
        "modulus \"x\"\n"
        "externum $f (h5xf64) -> vacuum\n"), &linea));
    CREDO_AEQUALIS_I32 ((i32)linea, II);
    CREDO_NIHIL (medulla_textum_legere(piscina, _ch(
        "modulus \"x\"\n"
        "externum $f (h2xs32) -> vacuum\n"), &linea));
    CREDO_NIHIL (medulla_textum_legere(piscina, _ch(
        "modulus \"x\"\n"
        "externum $f (a0) -> vacuum\n"), &linea));
    /* ' ... ' bis */
    CREDO_NIHIL (medulla_textum_legere(piscina, _ch(
        "modulus \"x\"\n"
        "functio $g () -> vacuum\n"
        "@a:\n"
        "    vocare $f, s32 1 ... s32 2 ... s32 3\n"
        "    redde\n"), &linea));
    CREDO_AEQUALIS_I32 ((i32)linea, IV);
    /* externum post functionem; externum duplex */
    CREDO_NIHIL (medulla_textum_legere(piscina, _ch(
        "modulus \"x\"\n"
        "functio $g () -> vacuum\n"
        "@a:\n"
        "    redde\n"
        "externum $f (s32) -> s32\n"), &linea));
    CREDO_AEQUALIS_I32 ((i32)linea, V);
    CREDO_NIHIL (medulla_textum_legere(piscina, _ch(
        "modulus \"x\"\n"
        "externum $f (s32) -> s32\n"
        "externum $f (s32) -> s32\n"), &linea));
    CREDO_AEQUALIS_I32 ((i32)linea, III);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "nomina signata", _sectio_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "scriptorem ($summa)", _sectio_secunda, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "vectem (byte-idem)", _sectio_tertia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "commenta + idempotentiam", _sectio_quarta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "fluitantia (%.17g)", _sectio_quinta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "errores lectoris", _sectio_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "vocare typata", _sectio_typata, NIHIL, NIHIL, NIHIL, NIHIL },
    { "vocare typata: errores", _sectio_typata_errores, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("officina_textus", SECTIONES);
}
