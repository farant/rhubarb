/* codificator_a64.c - codificator instructionum A64 (AArch64)
 *
 * Verba ex ARM ARM (DDI 0487), conferta cum clang per aurum
 * (probationes/fixa/codificator_a64/aurum.tsv). Quisque casus
 * campos suos probat antequam verbum componit: nulla approximatio
 * tacita, nullus campus truncatus.
 */
#include "codificator_a64.h"
#include "chorda_aedificator.h"
#include "xar.h"
#include <string.h>


    /* ==================================================
     * Adiumenta
     * ================================================== */

interior b32
_registrum (
    s32 r)
{
    redde r >= ZEPHYRUM && r <= XXXI;
}

/* registra rd, rn, rm (si adhibentur) */
interior b32
_registra (
    constans CodificatorA64Instructio* i,
                                  b32  cum_rd,
                                  b32  cum_rn,
                                  b32  cum_rm)
{
    si (cum_rd && !_registrum(i->rd))
    {
        redde FALSUM;
    }
    si (cum_rn && !_registrum(i->rn))
    {
        redde FALSUM;
    }
    si (cum_rm && !_registrum(i->rm))
    {
        redde FALSUM;
    }
    redde VERUM;
}

interior b32
_latitudo_integra (
    s32 latitudo)
{
    redde latitudo == XXXII || latitudo == LXIV;
}

/* bit sf (XXXI) aut typus FP (XXII..XXIII) */
interior i32
_sf (
    s32 latitudo)
{
    redde (latitudo == LXIV) ? 0x80000000 : ZEPHYRUM;
}

interior i32
_typus_fp (
    s32 latitudo)
{
    redde (latitudo == LXIV) ? 0x00400000 : ZEPHYRUM;
}

interior i32
_r (
    s32 r,
    s32 locus)
{
    redde (i32)r << locus;
}

/* log2 octetorum (I, II, IV, VIII) aut -I */
interior s32
_scala (
    s32 octeti)
{
    commutatio (octeti)
    {
        casus I:    redde ZEPHYRUM;
        casus II:   redde I;
        casus IV:   redde II;
        casus VIII: redde III;
        ordinarius: redde -I;
    }
}

/* distantia in octetis -> campus verborum signatus 'bita' latus */
interior b32
_saltus (
    s64  distantia,
    s32  bita,
    i32* campus)
{
    s64 verba;
    s64 limes;

    si ((distantia & III) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    verba = distantia / IV;
    limes = (s64)I << (bita - I);
    si (verba < -limes || verba >= limes)
    {
        redde FALSUM;
    }
    *campus = (i32)((i64)verba & (((i64)I << bita) - (i64)I));
    redde VERUM;
}


    /* ==================================================
     * Formae per genus
     * ================================================== */

/* ADD/SUB/SUBS immediatum */
interior b32
_arithmetica_immediata (
    constans CodificatorA64Instructio* i,
                                  i32  basis,
                                  i32* verbum)
{
    si (   !_latitudo_integra(i->latitudo)
        || !_registra(i, VERUM, VERUM, FALSUM)
        || i->immediatum < ZEPHYRUM || i->immediatum > 0xFFF
        || (i->translatio != ZEPHYRUM && i->translatio != XII))
    {
        redde FALSUM;
    }
    *verbum = basis | _sf(i->latitudo)
        | ((i->translatio == XII) ? 0x00400000 : ZEPHYRUM)
        | ((i32)i->immediatum << X) | _r(i->rn, V) | _r(i->rd, 0);
    redde VERUM;
}

/* registrum translatum (LSL): ADD/SUB/ADDS/SUBS/AND/ORR/EOR/ORN */
interior b32
_registrum_translatum (
    constans CodificatorA64Instructio* i,
                                  i32  basis,
                                  i32* verbum)
{
    si (   !_latitudo_integra(i->latitudo)
        || !_registra(i, VERUM, VERUM, VERUM)
        || i->translatio < ZEPHYRUM || i->translatio >= i->latitudo)
    {
        redde FALSUM;
    }
    *verbum = basis | _sf(i->latitudo) | _r(i->rm, XVI)
        | _r(i->translatio, X) | _r(i->rn, V) | _r(i->rd, 0);
    redde VERUM;
}

/* AND/ORR/EOR immediatum (bitmask) */
interior b32
_logica_immediata (
    constans CodificatorA64Instructio* i,
                                  i32  basis,
                                  i32* verbum)
{
    i32 triplex;

    si (   !_latitudo_integra(i->latitudo)
        || !_registra(i, VERUM, VERUM, FALSUM)
        || !codificator_a64_bitmask((i64)i->immediatum, i->latitudo,
                &triplex))
    {
        redde FALSUM;
    }
    *verbum = basis | _sf(i->latitudo) | (triplex << X)
        | _r(i->rn, V) | _r(i->rd, 0);
    redde VERUM;
}

/* tres registra sine immediato: LSLV/LSRV/ASRV/SDIV/UDIV */
interior b32
_tres_registra (
    constans CodificatorA64Instructio* i,
                                  i32  basis,
                                  i32* verbum)
{
    si (   !_latitudo_integra(i->latitudo)
        || !_registra(i, VERUM, VERUM, VERUM))
    {
        redde FALSUM;
    }
    *verbum = basis | _sf(i->latitudo) | _r(i->rm, XVI)
        | _r(i->rn, V) | _r(i->rd, 0);
    redde VERUM;
}

/* UBFM/SBFM: immediatum = immr, translatio = imms; N = sf */
interior b32
_campus_bitorum (
    constans CodificatorA64Instructio* i,
                                  i32  basis,
                                  i32* verbum)
{
    si (   !_latitudo_integra(i->latitudo)
        || !_registra(i, VERUM, VERUM, FALSUM)
        || i->immediatum < ZEPHYRUM
        || i->immediatum >= (s64)i->latitudo
        || i->translatio < ZEPHYRUM || i->translatio >= i->latitudo)
    {
        redde FALSUM;
    }
    *verbum = basis | _sf(i->latitudo)
        | ((i->latitudo == LXIV) ? 0x00400000 : ZEPHYRUM)
        | ((i32)i->immediatum << XVI) | _r(i->translatio, X)
        | _r(i->rn, V) | _r(i->rd, 0);
    redde VERUM;
}

/* MADD/MSUB */
interior b32
_multiplicatio (
    constans CodificatorA64Instructio* i,
                                  i32  basis,
                                  i32* verbum)
{
    si (   !_latitudo_integra(i->latitudo)
        || !_registra(i, VERUM, VERUM, VERUM) || !_registrum(i->ra))
    {
        redde FALSUM;
    }
    *verbum = basis | _sf(i->latitudo) | _r(i->rm, XVI)
        | _r(i->ra, X) | _r(i->rn, V) | _r(i->rd, 0);
    redde VERUM;
}

/* MOVZ/MOVK/MOVN: translatio = LSL (0..48, gradu XVI) */
interior b32
_motus_latus (
    constans CodificatorA64Instructio* i,
                                  i32  basis,
                                  i32* verbum)
{
    si (   !_latitudo_integra(i->latitudo)
        || !_registra(i, VERUM, FALSUM, FALSUM)
        || i->immediatum < ZEPHYRUM || i->immediatum > 0xFFFF
        || i->translatio < ZEPHYRUM
        || i->translatio         >= i->latitudo
        || (i->translatio % XVI) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    *verbum = basis | _sf(i->latitudo)
        | _r(i->translatio / XVI, XXI)
        | ((i32)i->immediatum << V) | _r(i->rd, 0);
    redde VERUM;
}

/* CSINC/CSEL */
interior b32
_electio (
    constans CodificatorA64Instructio* i,
                                  i32  basis,
                                  i32* verbum)
{
    si (   !_latitudo_integra(i->latitudo)
        || !_registra(i, VERUM, VERUM, VERUM)
        || i->conditio < ZEPHYRUM
        || i->conditio > (s32)CODIFICATOR_A64_AL)
    {
        redde FALSUM;
    }
    *verbum = basis | _sf(i->latitudo) | _r(i->rm, XVI)
        | _r(i->conditio, XII) | _r(i->rn, V) | _r(i->rd, 0);
    redde VERUM;
}

/* LDR/STR/LDRS cum offset insignato scalato (12 bita).
 * basis iam continet magnitudinem si opus est. */
interior b32
_memoria_immediata (
    constans CodificatorA64Instructio* i,
                                  i32  basis,
                                  s32  scala,
                                  i32* verbum)
{
    s64 index;

    si (   scala < ZEPHYRUM || !_registra(i, VERUM, VERUM, FALSUM)
        || i->immediatum < ZEPHYRUM)
    {
        redde FALSUM;
    }
    si ((i->immediatum & (((s64)I << scala) - I)) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    index = i->immediatum >> scala;
    si (index > 0xFFF)
    {
        redde FALSUM;
    }
    *verbum = basis | ((i32)index << X) | _r(i->rn, V)
        | _r(i->rd, 0);
    redde VERUM;
}

/* LDR/STR [rn, rm, LSL #s]; s = 0 aut scala */
interior b32
_memoria_registro (
    constans CodificatorA64Instructio* i,
                                  i32  basis,
                                  i32* verbum)
{
    s32 scala;

    scala = _scala(i->latitudo);
    si (scala < ZEPHYRUM || !_registra(i, VERUM, VERUM, VERUM))
    {
        redde FALSUM;
    }
    si (   i->translatio != ZEPHYRUM
        && (i->translatio != scala || scala == ZEPHYRUM))
    {
        redde FALSUM;
    }
    *verbum = basis | _r(scala, XXX) | _r(i->rm, XVI)
        | ((i->translatio != ZEPHYRUM) ? 0x1000 : ZEPHYRUM)
        | _r(i->rn, V) | _r(i->rd, 0);
    redde VERUM;
}

/* LDP/STP (generalia et FP): rd = Rt, rm = Rt2, rn = basis */
interior b32
_par (
    constans CodificatorA64Instructio* i,
                                  i32  basis,
                                  s32  scala,
                                  i32* verbum)
{
    s64 index;
    i32 modus;

    si (!_registra(i, VERUM, VERUM, VERUM))
    {
        redde FALSUM;
    }
    commutatio (i->translatio)
    {
        casus CODIFICATOR_A64_OFFSET: modus = 0x01000000; frange;
        casus CODIFICATOR_A64_ANTE:   modus = 0x01800000; frange;
        casus CODIFICATOR_A64_POST:   modus = 0x00800000; frange;
        ordinarius:                   redde FALSUM;
    }
    si ((i->immediatum & (((s64)I << scala) - I)) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    index = i->immediatum / ((s64)I << scala);
    si (index < -LXIV || index > LXIII)
    {
        redde FALSUM;
    }
    *verbum = basis | modus | (((i32)((i64)index & 0x7F)) << XV)
        | _r(i->rm, X) | _r(i->rn, V) | _r(i->rd, 0);
    redde VERUM;
}

/* FADD/FSUB/FMUL/FDIV/FCMP (tres aut duo registra FP) */
interior b32
_fp_tria (
    constans CodificatorA64Instructio* i,
                                  i32  basis,
                                  b32  cum_rd,
                                  i32* verbum)
{
    si (   !_latitudo_integra(i->latitudo)
        || !_registra(i, cum_rd, VERUM, VERUM))
    {
        redde FALSUM;
    }
    *verbum = basis | _typus_fp(i->latitudo) | _r(i->rm, XVI)
        | _r(i->rn, V) | (cum_rd ? _r(i->rd, 0) : ZEPHYRUM);
    redde VERUM;
}

/* FNEG/FMOV_REG/FCMP_ZERO (rd et/aut rn) */
interior b32
_fp_duo (
    constans CodificatorA64Instructio* i,
                                  i32  basis,
                                  b32  cum_rd,
                                  i32* verbum)
{
    si (   !_latitudo_integra(i->latitudo)
        || !_registra(i, cum_rd, VERUM, FALSUM))
    {
        redde FALSUM;
    }
    *verbum = basis | _typus_fp(i->latitudo) | _r(i->rn, V)
        | (cum_rd ? _r(i->rd, 0) : ZEPHYRUM);
    redde VERUM;
}

/* SCVTF/UCVTF/FCVTZS/FCVTZU: latitudo = generalis, translatio = FP */
interior b32
_conversio (
    constans CodificatorA64Instructio* i,
                                  i32  basis,
                                  i32* verbum)
{
    si (   !_latitudo_integra(i->latitudo)
        || !_latitudo_integra(i->translatio)
        || !_registra(i, VERUM, VERUM, FALSUM))
    {
        redde FALSUM;
    }
    *verbum = basis | _sf(i->latitudo) | _typus_fp(i->translatio)
        | _r(i->rn, V) | _r(i->rd, 0);
    redde VERUM;
}


    /* ==================================================
     * Codificare
     * ================================================== */

b32
codificator_a64_codificare (
    constans CodificatorA64Instructio* instructio,
                                  i32* verbum_out)
{
    constans CodificatorA64Instructio* i;
                                  i32  campus;
                                  s32  scala;

    i = instructio;
    si (i == NIHIL || verbum_out == NIHIL)
    {
        redde FALSUM;
    }
    commutatio (i->forma)
    {
        casus CODIFICATOR_A64_ADD_IMM:
            redde _arithmetica_immediata(i, 0x11000000, verbum_out);
        casus CODIFICATOR_A64_SUB_IMM:
            redde _arithmetica_immediata(i, 0x51000000, verbum_out);
        casus CODIFICATOR_A64_SUBS_IMM:
            redde _arithmetica_immediata(i, 0x71000000, verbum_out);

        casus CODIFICATOR_A64_ADD_REG:
            redde _registrum_translatum(i, 0x0B000000, verbum_out);
        casus CODIFICATOR_A64_SUB_REG:
            redde _registrum_translatum(i, 0x4B000000, verbum_out);
        casus CODIFICATOR_A64_ADDS_REG:
            redde _registrum_translatum(i, 0x2B000000, verbum_out);
        casus CODIFICATOR_A64_SUBS_REG:
            redde _registrum_translatum(i, 0x6B000000, verbum_out);
        casus CODIFICATOR_A64_AND_REG:
            redde _registrum_translatum(i, 0x0A000000, verbum_out);
        casus CODIFICATOR_A64_ORR_REG:
            redde _registrum_translatum(i, 0x2A000000, verbum_out);
        casus CODIFICATOR_A64_EOR_REG:
            redde _registrum_translatum(i, 0x4A000000, verbum_out);
        casus CODIFICATOR_A64_ORN_REG:
            redde _registrum_translatum(i, 0x2A200000, verbum_out);

        casus CODIFICATOR_A64_AND_IMM:
            redde _logica_immediata(i, 0x12000000, verbum_out);
        casus CODIFICATOR_A64_ORR_IMM:
            redde _logica_immediata(i, 0x32000000, verbum_out);
        casus CODIFICATOR_A64_EOR_IMM:
            redde _logica_immediata(i, 0x52000000, verbum_out);

        casus CODIFICATOR_A64_LSLV:
            redde _tres_registra(i, 0x1AC02000, verbum_out);
        casus CODIFICATOR_A64_LSRV:
            redde _tres_registra(i, 0x1AC02400, verbum_out);
        casus CODIFICATOR_A64_ASRV:
            redde _tres_registra(i, 0x1AC02800, verbum_out);
        casus CODIFICATOR_A64_SDIV:
            redde _tres_registra(i, 0x1AC00C00, verbum_out);
        casus CODIFICATOR_A64_UDIV:
            redde _tres_registra(i, 0x1AC00800, verbum_out);

        casus CODIFICATOR_A64_UBFM:
            redde _campus_bitorum(i, 0x53000000, verbum_out);
        casus CODIFICATOR_A64_SBFM:
            redde _campus_bitorum(i, 0x13000000, verbum_out);

        casus CODIFICATOR_A64_MADD:
            redde _multiplicatio(i, 0x1B000000, verbum_out);
        casus CODIFICATOR_A64_MSUB:
            redde _multiplicatio(i, 0x1B008000, verbum_out);

        casus CODIFICATOR_A64_MOVZ:
            redde _motus_latus(i, 0x52800000, verbum_out);
        casus CODIFICATOR_A64_MOVK:
            redde _motus_latus(i, 0x72800000, verbum_out);
        casus CODIFICATOR_A64_MOVN:
            redde _motus_latus(i, 0x12800000, verbum_out);

        casus CODIFICATOR_A64_CSINC:
            redde _electio(i, 0x1A800400, verbum_out);
        casus CODIFICATOR_A64_CSEL:
            redde _electio(i, 0x1A800000, verbum_out);

        casus CODIFICATOR_A64_LDR_IMM:
            scala = _scala(i->latitudo);
            redde _memoria_immediata(i, 0x39400000 | _r(scala, XXX),
                scala, verbum_out);
        casus CODIFICATOR_A64_STR_IMM:
            scala = _scala(i->latitudo);
            redde _memoria_immediata(i, 0x39000000 | _r(scala, XXX),
                scala, verbum_out);
        casus CODIFICATOR_A64_LDRS_IMM:
            /* ldrsb/ldrsh/ldrsw ad X (opc 10) */
            scala = (i->latitudo == VIII) ? -I : _scala(i->latitudo);
            redde _memoria_immediata(i, 0x39800000 | _r(scala, XXX),
                scala, verbum_out);
        casus CODIFICATOR_A64_LDR_REG:
            redde _memoria_registro(i, 0x38606800, verbum_out);
        casus CODIFICATOR_A64_STR_REG:
            redde _memoria_registro(i, 0x38206800, verbum_out);

        casus CODIFICATOR_A64_LDP:
        casus CODIFICATOR_A64_STP:
            si (i->latitudo != IV && i->latitudo != VIII)
            {
                redde FALSUM;
            }
            redde _par(i,
                ((i->latitudo == VIII) ? 0xA8000000 : 0x28000000)
                | ((i->forma == CODIFICATOR_A64_LDP)
                    ? 0x00400000 : ZEPHYRUM),
                _scala(i->latitudo), verbum_out);

        casus CODIFICATOR_A64_LDR_FP_IMM:
        casus CODIFICATOR_A64_STR_FP_IMM:
            si (i->latitudo != IV && i->latitudo != VIII)
            {
                redde FALSUM;
            }
            scala = _scala(i->latitudo);
            redde _memoria_immediata(i, 0x3D000000 | _r(scala, XXX)
                | ((i->forma == CODIFICATOR_A64_LDR_FP_IMM)
                    ? 0x00400000 : ZEPHYRUM),
                scala, verbum_out);

        casus CODIFICATOR_A64_LDP_FP:
        casus CODIFICATOR_A64_STP_FP:
            si (i->latitudo != IV && i->latitudo != VIII)
            {
                redde FALSUM;
            }
            redde _par(i,
                ((i->latitudo == VIII) ? 0x6C000000 : 0x2C000000)
                | ((i->forma == CODIFICATOR_A64_LDP_FP)
                    ? 0x00400000 : ZEPHYRUM),
                _scala(i->latitudo), verbum_out);

        casus CODIFICATOR_A64_ADR:
        casus CODIFICATOR_A64_ADRP:
        {
            s64 distantia;

            si (   i->latitudo != LXIV
                || !_registra(i, VERUM, FALSUM, FALSUM))
            {
                redde FALSUM;
            }
            distantia = i->immediatum;
            si (i->forma == CODIFICATOR_A64_ADRP)
            {
                si ((distantia & 0xFFF) != ZEPHYRUM)
                {
                    redde FALSUM;
                }
                distantia /= 4096;
            }
            si (   distantia < -((s64)I << XX)
                || distantia >= ((s64)I << XX))
            {
                redde FALSUM;
            }
            campus = (i32)((i64)distantia & 0x1FFFFF);
            *verbum_out = ((i->forma == CODIFICATOR_A64_ADRP)
                    ? 0x90000000 : 0x10000000)
                | ((campus & III) << XXIX)
                | ((campus >> II) << V) | _r(i->rd, 0);
            redde VERUM;
        }

        casus CODIFICATOR_A64_B:
        casus CODIFICATOR_A64_BL:
            si (!_saltus(i->immediatum, XXVI, &campus))
            {
                redde FALSUM;
            }
            *verbum_out = ((i->forma == CODIFICATOR_A64_BL)
                ? 0x94000000 : 0x14000000) | campus;
            redde VERUM;
        casus CODIFICATOR_A64_B_COND:
            si (   i->conditio < ZEPHYRUM
                || i->conditio > (s32)CODIFICATOR_A64_AL
                || !_saltus(i->immediatum, XIX, &campus))
            {
                redde FALSUM;
            }
            *verbum_out = 0x54000000 | (campus << V)
                | (i32)i->conditio;
            redde VERUM;
        casus CODIFICATOR_A64_CBZ:
        casus CODIFICATOR_A64_CBNZ:
            si (   !_latitudo_integra(i->latitudo)
                || !_registra(i, VERUM, FALSUM, FALSUM)
                || !_saltus(i->immediatum, XIX, &campus))
            {
                redde FALSUM;
            }
            *verbum_out = ((i->forma == CODIFICATOR_A64_CBNZ)
                    ? 0x35000000 : 0x34000000)
                | _sf(i->latitudo) | (campus << V) | _r(i->rd, 0);
            redde VERUM;
        casus CODIFICATOR_A64_BR:
        casus CODIFICATOR_A64_BLR:
        casus CODIFICATOR_A64_RET:
            si (!_registra(i, FALSUM, VERUM, FALSUM))
            {
                redde FALSUM;
            }
            *verbum_out = ((i->forma == CODIFICATOR_A64_BR) ? 0xD61F0000
                : (i->forma == CODIFICATOR_A64_BLR) ? 0xD63F0000
                : 0xD65F0000) | _r(i->rn, V);
            redde VERUM;

        casus CODIFICATOR_A64_FADD:
            redde _fp_tria(i, 0x1E202800, VERUM, verbum_out);
        casus CODIFICATOR_A64_FSUB:
            redde _fp_tria(i, 0x1E203800, VERUM, verbum_out);
        casus CODIFICATOR_A64_FMUL:
            redde _fp_tria(i, 0x1E200800, VERUM, verbum_out);
        casus CODIFICATOR_A64_FDIV:
            redde _fp_tria(i, 0x1E201800, VERUM, verbum_out);
        casus CODIFICATOR_A64_FCMP:
            redde _fp_tria(i, 0x1E202000, FALSUM, verbum_out);
        casus CODIFICATOR_A64_FNEG:
            redde _fp_duo(i, 0x1E214000, VERUM, verbum_out);
        casus CODIFICATOR_A64_FMOV_REG:
            redde _fp_duo(i, 0x1E204000, VERUM, verbum_out);
        casus CODIFICATOR_A64_FCMP_ZERO:
            redde _fp_duo(i, 0x1E202008, FALSUM, verbum_out);

        casus CODIFICATOR_A64_FMOV_IMM:
            si (   !_latitudo_integra(i->latitudo)
                || !_registra(i, VERUM, FALSUM, FALSUM)
                || i->immediatum < ZEPHYRUM || i->immediatum > CCLV)
            {
                redde FALSUM;
            }
            *verbum_out = 0x1E201000 | _typus_fp(i->latitudo)
                | ((i32)i->immediatum << XIII) | _r(i->rd, 0);
            redde VERUM;

        casus CODIFICATOR_A64_FMOV_GEN:
            /* x<->d (sf I, typus 01), w<->s (sf 0, typus 00);
             * opcode 111 generale -> FP, 110 FP -> generale */
            si (   !_latitudo_integra(i->latitudo)
                || (i->translatio != ZEPHYRUM && i->translatio != I)
                || !_registra(i, VERUM, VERUM, FALSUM))
            {
                redde FALSUM;
            }
            *verbum_out = 0x1E260000 | _sf(i->latitudo)
                | _typus_fp(i->latitudo)
                | ((i->translatio == ZEPHYRUM) ? 0x00010000 : ZEPHYRUM)
                | _r(i->rn, V) | _r(i->rd, 0);
            redde VERUM;

        casus CODIFICATOR_A64_FCVT:
            /* latitudo destinationis: D ex S (typus 00, opc 01),
             * S ex D (typus 01, opc 00) */
            si (   !_latitudo_integra(i->latitudo)
                || !_registra(i, VERUM, VERUM, FALSUM))
            {
                redde FALSUM;
            }
            *verbum_out = ((i->latitudo == LXIV)
                    ? 0x1E22C000 : 0x1E624000)
                | _r(i->rn, V) | _r(i->rd, 0);
            redde VERUM;

        casus CODIFICATOR_A64_SCVTF:
            redde _conversio(i, 0x1E220000, verbum_out);
        casus CODIFICATOR_A64_UCVTF:
            redde _conversio(i, 0x1E230000, verbum_out);
        casus CODIFICATOR_A64_FCVTZS:
            redde _conversio(i, 0x1E380000, verbum_out);
        casus CODIFICATOR_A64_FCVTZU:
            redde _conversio(i, 0x1E390000, verbum_out);

        casus CODIFICATOR_A64_BRK:
            si (i->immediatum < ZEPHYRUM || i->immediatum > 0xFFFF)
            {
                redde FALSUM;
            }
            *verbum_out = 0xD4200000 | ((i32)i->immediatum << V);
            redde VERUM;

        ordinarius:
            redde FALSUM;
    }
}


    /* ==================================================
     * Algorithmi publici
     * ================================================== */

interior i64
_larva (
    s32 amplitudo)
{
    redde (amplitudo >= LXIV) ? ~(i64)ZEPHYRUM
        : (((i64)I << amplitudo) - (i64)I);
}

/* Exemplar = elementum (amplitudo bitorum) repetitum; elementum =
 * cursus 'unae' unarum ad bitum 0 rotatus dextrorsum per immr. Ergo:
 * periodum minimam quaere, unas numera, rotationem sinistrorsum
 * quaere quae cursum ad bitum 0 reducit (LXIV temptamina summum). */
b32
codificator_a64_bitmask (
    i64  valor,
    s32  latitudo,
    i32* n_immr_imms_out)
{
    i64 v;
    i64 larva;
    s32 amplitudo;
    s32 unae;
    s32 k;
    s32 rotatio;

    si (latitudo == XXXII)
    {
        si ((valor >> XXXII) != ZEPHYRUM)
        {
            redde FALSUM;
        }
        v = valor | (valor << XXXII);
    }
    alioquin si (latitudo == LXIV)
    {
        v = valor;
    }
    alioquin
    {
        redde FALSUM;
    }
    si (v == ZEPHYRUM || v == ~(i64)ZEPHYRUM)
    {
        redde FALSUM;
    }

    amplitudo = LXIV;
    dum (amplitudo > II)
    {
        s32 dimidium;

        dimidium  = amplitudo / II;
        larva     = _larva(dimidium);
        si ((v & larva) != ((v >> dimidium) & larva))
        {
            frange;
        }
        amplitudo = dimidium;
    }
    larva  = _larva(amplitudo);
    v      &= larva;

    unae = ZEPHYRUM;
    per (k = ZEPHYRUM; k < amplitudo; k++)
    {
        si ((v & ((i64)I << k)) != ZEPHYRUM)
        {
            unae++;
        }
    }

    rotatio = -I;
    per (k = ZEPHYRUM; k < amplitudo; k++)
    {
        i64 rotatum;

        rotatum = (k == ZEPHYRUM) ? v
            : (((v << k) | (v >> (amplitudo - k))) & larva);
        si (rotatum == _larva(unae))
        {
            rotatio = k;
            frange;
        }
    }
    si (rotatio < ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (n_immr_imms_out != NIHIL)
    {
        *n_immr_imms_out =
              ((amplitudo == LXIV) ? 0x1000 : ZEPHYRUM)
            | ((i32)rotatio << VI)
            | ((~((i32)amplitudo - I) << I) & 0x3F)
            | (i32)(unae - I);
    }
    redde VERUM;
}

/* frusta XVI bitorum: MOVN si plura 0xFFFF quam 0x0000, aliter
 * MOVZ; deinde MOVK pro frustis non trivialibus */
s32
codificator_a64_constans (
    i64  valor,
    s32  latitudo,
    s32  rd,
    i32* verba_out)
{
    CodificatorA64Instructio instructio;
                         s32 frusta;
                         s32 k;
                         s32 zephyra;
                         s32 plena;
                         b32 negatus;
                         s32 numerus;

    si (   !_latitudo_integra(latitudo) || !_registrum(rd)
        || verba_out == NIHIL)
    {
        redde ZEPHYRUM;
    }
    si (latitudo == XXXII && (valor >> XXXII) != ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    frusta   = latitudo / XVI;
    zephyra  = ZEPHYRUM;
    plena    = ZEPHYRUM;
    per (k = ZEPHYRUM; k < frusta; k++)
    {
        i64 frustum;

        frustum = (valor >> (k * XVI)) & 0xFFFF;
        si (frustum == ZEPHYRUM)
        {
            zephyra++;
        }
        alioquin si (frustum == 0xFFFF)
        {
            plena++;
        }
    }
    negatus = plena > zephyra;
    numerus = ZEPHYRUM;
    memset(&instructio, ZEPHYRUM, magnitudo(instructio));
    instructio.latitudo  = latitudo;
    instructio.rd        = rd;
    per (k = ZEPHYRUM; k < frusta; k++)
    {
        i64 frustum;

        frustum = (valor >> (k * XVI)) & 0xFFFF;
        si (frustum == (negatus ? (i64)0xFFFF : (i64)ZEPHYRUM))
        {
            perge;
        }
        si (numerus == ZEPHYRUM)
        {
            instructio.forma = negatus ? CODIFICATOR_A64_MOVN
                : CODIFICATOR_A64_MOVZ;
            instructio.immediatum = negatus
                ? (s64)(~frustum & 0xFFFF) : (s64)frustum;
        }
        alioquin
        {
            instructio.forma       = CODIFICATOR_A64_MOVK;
            instructio.immediatum  = (s64)frustum;
        }
        instructio.translatio = k * XVI;
        si (!codificator_a64_codificare(&instructio,
                &verba_out[numerus]))
        {
            redde ZEPHYRUM;
        }
        numerus++;
    }
    si (numerus == ZEPHYRUM)
    {
        /* 0 aut ~0 */
        instructio.forma = negatus ? CODIFICATOR_A64_MOVN
            : CODIFICATOR_A64_MOVZ;
        instructio.immediatum = ZEPHYRUM;
        instructio.translatio = ZEPHYRUM;
        si (!codificator_a64_codificare(&instructio, &verba_out[0]))
        {
            redde ZEPHYRUM;
        }
        numerus = I;
    }
    redde numerus;
}

/* VFPExpandImm: +-(16 + f)/16 * 2^e, e in -3..4 */
interior f64
_fmov_valor (
    i32 imm8)
{
    s32 exponens;
    f64 valor;

    exponens = (((imm8 >> VI) & I) != ZEPHYRUM)
        ? (s32)((imm8 >> IV) & III) - III
        : (s32)((imm8 >> IV) & III) + I;
    valor = (f64)(XVI + (imm8 & 0xF)) / 16.0;
    dum (exponens > ZEPHYRUM)
    {
        valor *= 2.0;
        exponens--;
    }
    dum (exponens < ZEPHYRUM)
    {
        valor /= 2.0;
        exponens++;
    }
    redde ((imm8 & 0x80) != ZEPHYRUM) ? -valor : valor;
}

b32
codificator_a64_fmov_immediatum (
    f64  valor,
    i32* imm8_out)
{
    i32 i;

    per (i = ZEPHYRUM; i <= CCLV; i++)
    {
        si (_fmov_valor(i) == valor)
        {
            si (imm8_out != NIHIL)
            {
                *imm8_out = i;
            }
            redde VERUM;
        }
    }
    redde FALSUM;
}


    /* ==================================================
     * Emissor cum notis
     * ================================================== */

nomen structura {
                         i32 index;
                         s32 nota;
    CodificatorA64Instructio instructio;
} SaltusPendens;

structura CodificatorA64 {
    Piscina* piscina;
        i32* verba;
        i32  numerus;
        i32  capacitas;
        Xar* notae;        /* s32: index verbi, aut -I */
        Xar* saltus;       /* SaltusPendens */
};

CodificatorA64*
codificator_a64_creare (
    Piscina* piscina)
{
    CodificatorA64* c;

    si (piscina == NIHIL)
    {
        redde NIHIL;
    }
    c = (CodificatorA64*)piscina_allocare(piscina,
        magnitudo(CodificatorA64));
    si (c == NIHIL)
    {
        redde NIHIL;
    }
    c->piscina    = piscina;
    c->verba      = NIHIL;
    c->numerus    = ZEPHYRUM;
    c->capacitas  = ZEPHYRUM;
    c->notae      = xar_creare(piscina, (i32)magnitudo(s32));
    c->saltus     = xar_creare(piscina, (i32)magnitudo(SaltusPendens));
    si (c->notae == NIHIL || c->saltus == NIHIL)
    {
        redde NIHIL;
    }
    redde c;
}

/* verba contigua manent (codificator_a64_verba): duplicatio cum
 * copia; vetera in piscina relinquuntur */
interior b32
_verbum_addere (
    CodificatorA64* c,
               i32  verbum)
{
    si (c->numerus == c->capacitas)
    {
        i32  nova;
        i32* verba;

        nova  = (c->capacitas == ZEPHYRUM) ? CCLVI : c->capacitas * II;
        verba = (i32*)piscina_allocare(c->piscina,
            (memoriae_index)nova * magnitudo(i32));
        si (verba == NIHIL)
        {
            redde FALSUM;
        }
        si (c->numerus > ZEPHYRUM)
        {
            memcpy(verba, c->verba,
                (memoriae_index)c->numerus * magnitudo(i32));
        }
        c->verba      = verba;
        c->capacitas  = nova;
    }
    c->verba[c->numerus] = verbum;
    c->numerus++;
    redde VERUM;
}

b32
codificator_a64_emittere (
                       CodificatorA64* c,
    constans CodificatorA64Instructio* instructio)
{
    i32 verbum;

    si (   c == NIHIL
        || !codificator_a64_codificare(instructio, &verbum))
    {
        redde FALSUM;
    }
    redde _verbum_addere(c, verbum);
}

s32
codificator_a64_notam_creare (
    CodificatorA64* c)
{
    s32* nota;

    si (c == NIHIL)
    {
        redde -I;
    }
    nota = (s32*)xar_addere(c->notae);
    si (nota == NIHIL)
    {
        redde -I;
    }
    *nota = -I;
    redde (s32)xar_numerus(c->notae) - I;
}

vacuum
codificator_a64_notam_ponere (
    CodificatorA64* c,
               s32  nota)
{
    s32* locus;

    si (   c    == NIHIL || nota < ZEPHYRUM
        || nota >= (s32)xar_numerus(c->notae))
    {
        redde;
    }
    locus   = (s32*)xar_obtinere(c->notae, (i32)nota);
    *locus  = (s32)c->numerus;
}

b32
codificator_a64_saltum_emittere (
                       CodificatorA64* c,
    constans CodificatorA64Instructio* instructio,
                                  s32  nota)
{
    CodificatorA64Instructio  copia;
               SaltusPendens* pendens;
                         i32  verbum;

    si (   c    == NIHIL || instructio == NIHIL || nota < ZEPHYRUM
        || nota >= (s32)xar_numerus(c->notae))
    {
        redde FALSUM;
    }
    commutatio (instructio->forma)
    {
        casus CODIFICATOR_A64_B:
        casus CODIFICATOR_A64_BL:
        casus CODIFICATOR_A64_B_COND:
        casus CODIFICATOR_A64_CBZ:
        casus CODIFICATOR_A64_CBNZ:
        casus CODIFICATOR_A64_ADR:
            frange;
        ordinarius:
            redde FALSUM;
    }
    /* campi ceteri nunc probantur; distantia in finire */
    copia             = *instructio;
    copia.immediatum  = ZEPHYRUM;
    si (!codificator_a64_codificare(&copia, &verbum))
    {
        redde FALSUM;
    }
    pendens = (SaltusPendens*)xar_addere(c->saltus);
    si (pendens == NIHIL || !_verbum_addere(c, verbum))
    {
        redde FALSUM;
    }
    pendens->index       = c->numerus - I;
    pendens->nota        = nota;
    pendens->instructio  = copia;
    redde VERUM;
}

interior vacuum
_causam_ponere (
        CodificatorA64* c,
                chorda* causa_out,
    constans character* initium,
                   i32  index,
                   s32  nota,
                   s64  distantia,
                   b32  cum_distantia)
{
    ChordaAedificator* aedificator;

    si (causa_out == NIHIL)
    {
        redde;
    }
    aedificator = chorda_aedificator_creare(c->piscina, LXIV);
    si (aedificator == NIHIL)
    {
        redde;
    }
    chorda_aedificator_appendere_literis(aedificator, initium);
    chorda_aedificator_appendere_literis(aedificator, ": verbum ");
    chorda_aedificator_appendere_s32(aedificator, (s32)index);
    chorda_aedificator_appendere_literis(aedificator, ", nota ");
    chorda_aedificator_appendere_s32(aedificator, nota);
    si (cum_distantia)
    {
        chorda_aedificator_appendere_literis(aedificator,
            ", distantia ");
        chorda_aedificator_appendere_s32(aedificator, (s32)distantia);
        chorda_aedificator_appendere_literis(aedificator, " octeti");
    }
    *causa_out = chorda_aedificator_finire(aedificator);
}

b32
codificator_a64_finire (
    CodificatorA64* c,
            chorda* causa_out)
{
    i32 i;

    si (c == NIHIL)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(c->saltus); i++)
    {
        SaltusPendens* pendens;
                  s32  meta;
                  s64  distantia;
                  i32  verbum;

        pendens = (SaltusPendens*)xar_obtinere(c->saltus, i);
        meta    = *(constans s32*)xar_obtinere(c->notae,
            (i32)pendens->nota);
        si (meta < ZEPHYRUM)
        {
            _causam_ponere(c, causa_out, "nota non posita",
                pendens->index, pendens->nota, ZEPHYRUM, FALSUM);
            redde FALSUM;
        }
        distantia = ((s64)meta - (s64)pendens->index) * IV;
        pendens->instructio.immediatum = distantia;
        si (!codificator_a64_codificare(&pendens->instructio,
                &verbum))
        {
            _causam_ponere(c, causa_out, "saltus extra fines",
                pendens->index, pendens->nota, distantia, VERUM);
            redde FALSUM;
        }
        c->verba[pendens->index] = verbum;
    }
    redde VERUM;
}

i32
codificator_a64_numerus (
    constans CodificatorA64* c)
{
    redde (c == NIHIL) ? ZEPHYRUM : c->numerus;
}

constans i32*
codificator_a64_verba (
    constans CodificatorA64* c)
{
    redde (c == NIHIL) ? NIHIL : c->verba;
}
