/* codificator_a64.h - codificator instructionum A64 (AArch64).
 *
 * Instructio structa -> verbum XXXII bitorum. Nulla approximatio
 * tacita: forma, latitudo aut immediatum extra fines -> FALSUM.
 * Registrum XXXI: SP in ADD/SUB_IMM et in basi memoriae (rn),
 * aliter XZR/WZR.
 */
#ifndef CODIFICATOR_A64_H
#define CODIFICATOR_A64_H

#include "latina.h"
#include "chorda.h"

/* formae; index stabilis (aurum.tsv eum nominat) */
nomen enumeratio {
    CODIFICATOR_A64_ADD_IMM = 0, CODIFICATOR_A64_SUB_IMM,
    CODIFICATOR_A64_SUBS_IMM,       /* cmp */
    CODIFICATOR_A64_ADD_REG, CODIFICATOR_A64_SUB_REG,
    CODIFICATOR_A64_ADDS_REG, CODIFICATOR_A64_SUBS_REG,
    CODIFICATOR_A64_AND_REG, CODIFICATOR_A64_ORR_REG,
    CODIFICATOR_A64_EOR_REG, CODIFICATOR_A64_ORN_REG,
    CODIFICATOR_A64_AND_IMM, CODIFICATOR_A64_ORR_IMM,
    CODIFICATOR_A64_EOR_IMM,        /* bitmask */
    CODIFICATOR_A64_LSLV, CODIFICATOR_A64_LSRV,
    CODIFICATOR_A64_ASRV,
    CODIFICATOR_A64_UBFM, CODIFICATOR_A64_SBFM,
                                    /* lsl/lsr/asr imm, sxt/uxt */
    CODIFICATOR_A64_MADD, CODIFICATOR_A64_MSUB,
    CODIFICATOR_A64_SDIV, CODIFICATOR_A64_UDIV,
    CODIFICATOR_A64_MOVZ, CODIFICATOR_A64_MOVK,
    CODIFICATOR_A64_MOVN,
    CODIFICATOR_A64_CSINC,          /* cset */
    CODIFICATOR_A64_CSEL,
    CODIFICATOR_A64_LDR_IMM, CODIFICATOR_A64_STR_IMM,
    CODIFICATOR_A64_LDRS_IMM,       /* ldrsb/ldrsh/ldrsw, ad X */
    CODIFICATOR_A64_LDR_REG, CODIFICATOR_A64_STR_REG,
    CODIFICATOR_A64_LDP, CODIFICATOR_A64_STP,
    CODIFICATOR_A64_LDR_FP_IMM, CODIFICATOR_A64_STR_FP_IMM,
    CODIFICATOR_A64_LDP_FP, CODIFICATOR_A64_STP_FP,
                                    /* s/d; d8-d15 servandi */
    CODIFICATOR_A64_ADR, CODIFICATOR_A64_ADRP,
    CODIFICATOR_A64_B, CODIFICATOR_A64_BL,
    CODIFICATOR_A64_B_COND,
    CODIFICATOR_A64_CBZ, CODIFICATOR_A64_CBNZ,
    CODIFICATOR_A64_BR, CODIFICATOR_A64_BLR,
    CODIFICATOR_A64_RET,
    CODIFICATOR_A64_FADD, CODIFICATOR_A64_FSUB,
    CODIFICATOR_A64_FMUL, CODIFICATOR_A64_FDIV,
    CODIFICATOR_A64_FNEG,
    CODIFICATOR_A64_FCMP, CODIFICATOR_A64_FCMP_ZERO,
    CODIFICATOR_A64_FMOV_REG, CODIFICATOR_A64_FMOV_IMM,
    CODIFICATOR_A64_FMOV_GEN,       /* x<->d, w<->s */
    CODIFICATOR_A64_FCVT,           /* s<->d */
    CODIFICATOR_A64_SCVTF, CODIFICATOR_A64_UCVTF,
    CODIFICATOR_A64_FCVTZS, CODIFICATOR_A64_FCVTZU,
    CODIFICATOR_A64_BRK,
    CODIFICATOR_A64_NUMERUS
} CodificatorA64Forma;

/* condiciones; valores = codices A64 */
nomen enumeratio {
    CODIFICATOR_A64_EQ = 0, CODIFICATOR_A64_NE,
    CODIFICATOR_A64_HS, CODIFICATOR_A64_LO,
    CODIFICATOR_A64_MI, CODIFICATOR_A64_PL,
    CODIFICATOR_A64_VS, CODIFICATOR_A64_VC,
    CODIFICATOR_A64_HI, CODIFICATOR_A64_LS,
    CODIFICATOR_A64_GE, CODIFICATOR_A64_LT,
    CODIFICATOR_A64_GT, CODIFICATOR_A64_LE,
    CODIFICATOR_A64_AL
} CodificatorA64Conditio;

/* modi LDP/STP (in campo translatio) */
nomen enumeratio {
    CODIFICATOR_A64_OFFSET = 0,     /* [rn, #imm] */
    CODIFICATOR_A64_ANTE,           /* [rn, #imm]! */
    CODIFICATOR_A64_POST            /* [rn], #imm */
} CodificatorA64Modus;

/* Campi per formam (praeter communia):
 *   UBFM/SBFM        immediatum = immr, translatio = imms
 *   *_IMM logicae    immediatum = exemplar bitorum crudum
 *   MOVZ/MOVK/MOVN   translatio = LSL (0, 16, 32, 48)
 *   FMOV_IMM         immediatum = imm8 (vide fmov_immediatum)
 *   FMOV_GEN         translatio 0: generale -> FP; I: FP -> generale
 *   FCVT             latitudo = destinationis
 *   SCVTF .. FCVTZU  latitudo = registri generalis, translatio =
 *                    registri FP (XXXII/LXIV)
 *   LDP/STP (+FP)    rd = Rt, rm = Rt2, rn = basis
 *   saltus, ADR      immediatum = distantia in octetis ab
 *                    instructione ipsa (ADRP: paginae * 4096)
 */
nomen structura {
    s32 forma;
    s32 latitudo;      /* XXXII/LXIV (W/X; S/D); in memoria:
                        * octeti (I/II/IV/VIII; FP IV/VIII) */
    s32 rd, rn, rm, ra;
    s64 immediatum;    /* imm/offset/saltus in octetis */
    s32 translatio;    /* quantitas LSL; LDP/STP: modus */
    s32 conditio;      /* CodificatorA64Conditio */
} CodificatorA64Instructio;

/* verbum unius instructionis; FALSUM si extra fines */
b32
codificator_a64_codificare (
    constans CodificatorA64Instructio* instructio,
                                  i32* verbum_out);

/* algorithmi publici */

/* valor: exemplar bitorum crudum; FALSUM si non repraesentabile
 * (0 et ~0 numquam) */
b32
codificator_a64_bitmask (
    i64  valor,
    s32  latitudo,
    i32* n_immr_imms_out);

/* reddit numerum verborum (I-IV) quae valorem in rd ponunt */
s32
codificator_a64_constans (
    i64  valor,
    s32  latitudo,
    s32  rd,
    i32* verba_out /* IV max */);

/* FALSUM nisi valor imm8 exacte (+-(16..31)/16 * 2^(-3..4)) */
b32
codificator_a64_fmov_immediatum (
    f64  valor,
    i32* imm8_out);

/* emissor cum notis saltuum */
nomen structura CodificatorA64 CodificatorA64;

CodificatorA64*
codificator_a64_creare (
    Piscina* piscina);

b32
codificator_a64_emittere (
                       CodificatorA64* c,
    constans CodificatorA64Instructio* instructio);

s32
codificator_a64_notam_creare (
    CodificatorA64* c);

vacuum
codificator_a64_notam_ponere (
    CodificatorA64* c,
               s32  nota);

/* B, BL, B_COND, CBZ, CBNZ, ADR ad notam */
b32
codificator_a64_saltum_emittere (
                       CodificatorA64* c,
    constans CodificatorA64Instructio* instructio,
                                  s32  nota);

/* notas resolvit; FALSUM + causa si nota non posita aut saltus
 * extra fines */
b32
codificator_a64_finire (
    CodificatorA64* c,
            chorda* causa_out);

i32
codificator_a64_numerus (
    constans CodificatorA64* c);

constans i32*
codificator_a64_verba (
    constans CodificatorA64* c);

#endif /* CODIFICATOR_A64_H */
