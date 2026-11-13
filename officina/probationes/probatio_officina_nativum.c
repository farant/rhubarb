/* probatio_officina_nativum.c - introitus in codicem nativum et
 * effugium (M5 T6: area, trampolinum, vocare, salire, tractator
 * signorum)
 *
 * Functiones nativae manu per codificator_a64 in aream effugii
 * scribuntur, post trampolinum (OFFICINA_EFFUGIUM_TRAMPOLINUM); sectio
 * quaeque locum proprium habet (area una per processum manet).
 *
 * CATENA: ante parare PRIMUM suita tractatorem SIGSEGV proprium ponit
 * (_tractator_prior) - effugium eum ut actionem priorem servat; vitium
 * EXTRA aream ad eum pervenire debet.
 */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "codificator_a64.h"
#include "memoria_exsecutabilis.h"
#include "officina_effugium.h"
#include "officina_dorsum.h"
#include "filum.h"
#include "lectiones.h"
#include "officina_machinula.h"
#include "officina_medulla_textus.h"
#include "credo.h"
#include <math.h>
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOCUS_INCREMENTI  (OFFICINA_EFFUGIUM_TRAMPOLINUM)
#define LOCUS_SUMMAE      (OFFICINA_EFFUGIUM_TRAMPOLINUM + 0x100UL)
#define LOCUS_FLUITANTIS  (OFFICINA_EFFUGIUM_TRAMPOLINUM + 0x200UL)
#define LOCUS_STIVAE      (OFFICINA_EFFUGIUM_TRAMPOLINUM + 0x300UL)
#define LOCUS_SALTUS      (OFFICINA_EFFUGIUM_TRAMPOLINUM + 0x400UL)
#define LOCUS_VITII       (OFFICINA_EFFUGIUM_TRAMPOLINUM + 0x500UL)
#define LOCUS_ILLICITI    (OFFICINA_EFFUGIUM_TRAMPOLINUM + 0x600UL)
#define LOCUS_MIXTI       (OFFICINA_EFFUGIUM_TRAMPOLINUM + 0x700UL)
#define LOCUS_PRINCIPALIS (OFFICINA_EFFUGIUM_TRAMPOLINUM + 0x800UL)
#define LOCUS_NULLI       (OFFICINA_EFFUGIUM_TRAMPOLINUM + 0x900UL)
#define LOCUS_NONI        (OFFICINA_EFFUGIUM_TRAMPOLINUM + 0xA00UL)

/* tractator prior suitae (catena) */
hic_manens sigjmp_buf  _ambitus_prioris;
hic_manens        s32  _prior_vocatus  = ZEPHYRUM;
hic_manens        s32  _prior_signum   = ZEPHYRUM;

/* regio machinulae sectionis currentis (basis fixa: una per
 * processum simul) - purgare sectionis eam destruit */
hic_manens Regio* _regio_sectionis = NIHIL;
/* conexio machinulae sectionis (dorsum eam legit) */
hic_manens Conexio* _conexio_sectionis = NIHIL;
/* dorsum sectionis (unum vivum per aream) */
hic_manens OfficinaDorsum* _dorsum_sectionis = NIHIL;

interior vacuum
_tractator_prior (
      integer  signum,
    siginfo_t* notitia,
       vacuum* contextus)
{
    (vacuum)notitia;
    (vacuum)contextus;
    _prior_vocatus++;
    _prior_signum = (s32)signum;
    siglongjmp(_ambitus_prioris, I);
}

interior CodificatorA64Instructio
_instructio (
    s32 forma,
    s32 latitudo,
    s32 rd,
    s32 rn,
    s32 rm,
    s64 immediatum)
{
    CodificatorA64Instructio i;

    memset(&i, ZEPHYRUM, magnitudo(i));
    i.forma       = forma;
    i.latitudo    = latitudo;
    i.rd          = rd;
    i.rn          = rn;
    i.rm          = rm;
    i.immediatum  = immediatum;
    redde i;
}

interior b32
_emittere (
    CodificatorA64* c,
               s32  forma,
               s32  latitudo,
               s32  rd,
               s32  rn,
               s32  rm,
               s64  immediatum)
{
    CodificatorA64Instructio i = _instructio(forma, latitudo, rd, rn,
        rm, immediatum);

    redde codificator_a64_emittere(c, &i);
}

/* verba emissa ad offset areae scribit et sigillat; reddit introitum
 * (NIHIL si quid fractum) */
interior vacuum*
_codicem_ponere (
    CodificatorA64* c,
    memoriae_index  offset)
{
    MemoriaExsecutabilis* area = officina_effugium_area();
                  chorda  causa;
          memoriae_index  mensura;
                      i8* basis;

    causa.mensura = ZEPHYRUM;
    si (area == NIHIL || !codificator_a64_finire(c, &causa))
    {
        redde NIHIL;
    }
    mensura  = (memoriae_index)codificator_a64_numerus(c) * IV;
    basis    = (i8*)memoria_exsecutabilis_basis(area);
    memoria_exsecutabilis_aperire(area);
    memcpy(basis + offset, codificator_a64_verba(c), mensura);
    si (!memoria_exsecutabilis_sigillare(area, offset, mensura))
    {
        redde NIHIL;
    }
    redde basis + offset;
}

interior b32
_parare (
    Piscina* piscina)
{
    chorda causa;

    causa.mensura = ZEPHYRUM;
    redde officina_effugium_parare(piscina, &causa);
}

/* parare sectionis cuiusque: numeratores tractatoris prioris
 * reponuntur; ANTE parare effugii primum (area nondum est) tractator
 * prior ponitur - effugium eum ut actionem priorem servat. Postea
 * numquam iterum (tractatorem effugii superscriberet). */
interior vacuum
_sectionem_parare (
    CredoContextus* c)
{
    (vacuum)c;
    _conexio_sectionis  = NIHIL;
    _dorsum_sectionis   = NIHIL;
    _regio_sectionis    = NIHIL;
    _prior_vocatus      = ZEPHYRUM;
    _prior_signum       = ZEPHYRUM;
    memset(&_ambitus_prioris, ZEPHYRUM, magnitudo(_ambitus_prioris));
    si (officina_effugium_area() == NIHIL)
    {
        structura sigaction actio;

        memset(&actio, ZEPHYRUM, magnitudo(actio));
        actio.sa_sigaction = _tractator_prior;
        (vacuum)sigemptyset(&actio.sa_mask);
        actio.sa_flags = SA_SIGINFO;
        (vacuum)sigaction(SIGSEGV, &actio, NIHIL);
    }
}

/* purgare sectionis cuiusque (SEMPER vocatur, etiam post CREDO_NECESSE
 * fractum): regio machinulae redditur */
interior vacuum
_sectionem_purgare (
    CredoContextus* c)
{
    (vacuum)c;
    si (_regio_sectionis != NIHIL)
    {
        regio_destruere(_regio_sectionis);
        _regio_sectionis = NIHIL;
    }
    _conexio_sectionis  = NIHIL;
    _dorsum_sectionis   = NIHIL;
}

/* add w0, w0, #1; ret */
interior vacuum*
_incrementum_ponere (
    Piscina* piscina)
{
    CodificatorA64* c = codificator_a64_creare(piscina);

    si (   c == NIHIL
        || !_emittere(c, CODIFICATOR_A64_ADD_IMM, XXXII, 0, 0, 0, I)
        || !_emittere(c, CODIFICATOR_A64_RET, LXIV, 0, XXX, 0, 0))
    {
        redde NIHIL;
    }
    redde _codicem_ponere(c, LOCUS_INCREMENTI);
}


    /* ==================================================
     * PROBARE: parare (semel; area ad locum fixum)
     * ================================================== */

interior vacuum
_sectio_parare (
    CredoContextus* c)
{
    MemoriaExsecutabilis* area;

    CREDO_NECESSE (_parare(c->piscina));
    area = officina_effugium_area();
    CREDO_NECESSE_NON_NIHIL (area);
    CREDO_VERUM (   (memoriae_index)memoria_exsecutabilis_basis(area)
                 == (memoriae_index)OFFICINA_NATIVUM_LOCUS);
    CREDO_VERUM (   memoria_exsecutabilis_mensura(area)
                 == (memoriae_index)OFFICINA_NATIVUM_MENSURA);
    /* iterum: nihil agit, area eadem */
    CREDO_VERUM (_parare(c->piscina));
    CREDO_VERUM (officina_effugium_area() == area);
}


    /* ==================================================
     * PROBARE: reditus per trampolinum (x0 intrat, x0 redit)
     * ================================================== */

interior vacuum
_sectio_reditus (
    CredoContextus* c)
{
               vacuum* introitus;
    EffugiumArgumenta  argumenta;
      EffugiumFructus  fructus;

    CREDO_NECESSE (_parare(c->piscina));
    introitus = _incrementum_ponere(c->piscina);
    CREDO_NECESSE_NON_NIHIL (introitus);
    memset(&argumenta, ZEPHYRUM, magnitudo(argumenta));
    argumenta.x[0] = (i64)XLI;
    argumenta.numerus_x = I;
    fructus = officina_effugium_vocare(introitus, &argumenta);
    CREDO_AEQUALIS_S32 (fructus.genus, EFFUGIUM_REDDITUM);
    CREDO_AEQUALIS_S32 ((s32)(fructus.x[0] & 0xFFFFFFFFUL), XLII);
}


    /* ==================================================
     * PROBARE: registra integra octo (x0..x7) et x8
     * ================================================== */

interior vacuum
_sectio_registra (
    CredoContextus* c)
{
       CodificatorA64* codificator;
               vacuum* introitus;
    EffugiumArgumenta  argumenta;
      EffugiumFructus  fructus;
                  s32  k;
                  b32  bene = VERUM;

    CREDO_NECESSE (_parare(c->piscina));
    codificator = codificator_a64_creare(c->piscina);
    CREDO_NECESSE_NON_NIHIL (codificator);
    /* x0 = x0 + x1*2 + x2*3 ... ponderibus distinctis: ordo probatur.
     * x9 = pondus currens; madd: rd = ra + rn*rm, campus ra = x0
     * (zephyrus). x1 = x8 (redditur in x1) */
    per (k = I; k < VIII; k++)
    {
        bene = bene
            && _emittere(codificator, CODIFICATOR_A64_MOVZ, LXIV, IX, 0,
            0,
                (s64)(k + I))
            && _emittere(codificator, CODIFICATOR_A64_MADD, LXIV, 0, k,
            IX, 0);
    }
    CREDO_NECESSE (bene);
    CREDO_NECESSE (
           _emittere(codificator, CODIFICATOR_A64_ORR_REG, LXIV, I,
           XXXI,
               VIII, 0)
        && _emittere(codificator, CODIFICATOR_A64_RET, LXIV, 0, XXX, 0,
        0));
    introitus = _codicem_ponere(codificator, LOCUS_SUMMAE);
    CREDO_NECESSE_NON_NIHIL (introitus);
    memset(&argumenta, ZEPHYRUM, magnitudo(argumenta));
    per (k = ZEPHYRUM; k < VIII; k++)
    {
        argumenta.x[k] = (i64)(k + I);
    }
    argumenta.numerus_x = VIII;
    argumenta.x8 = (i64)0x123456789ABCUL;
    fructus = officina_effugium_vocare(introitus, &argumenta);
    CREDO_AEQUALIS_S32 (fructus.genus, EFFUGIUM_REDDITUM);
    /* 1 + 2*2 + 3*3 + ... + 8*8 = 204 */
    CREDO_AEQUALIS_I64 (fructus.x[0], (i64)CCIV);
    CREDO_AEQUALIS_I64 (fructus.x[1], (i64)0x123456789ABCUL);
}


    /* ==================================================
     * PROBARE: registra fluitantia (d0..d7 intrant, d0..d3 redeunt)
     * ================================================== */

interior vacuum
_sectio_fluitantia (
    CredoContextus* c)
{
       CodificatorA64* codificator;
               vacuum* introitus;
    EffugiumArgumenta  argumenta;
      EffugiumFructus  fructus;
                  s32  k;

    CREDO_NECESSE (_parare(c->piscina));
    codificator = codificator_a64_creare(c->piscina);
    CREDO_NECESSE_NON_NIHIL (codificator);
    /* d0 = d0 - d7; d1 = d1 + d6; d2 = d2 + d5; d3 = d3 + d4; ret */
    CREDO_NECESSE (
           _emittere(codificator, CODIFICATOR_A64_FSUB, LXIV, 0, 0, VII,
           0)
        && _emittere(codificator, CODIFICATOR_A64_FADD, LXIV, I, I, VI,
        0)
        && _emittere(codificator, CODIFICATOR_A64_FADD, LXIV, II, II, V,
        0)
        && _emittere(codificator, CODIFICATOR_A64_FADD, LXIV, III, III,
        IV, 0)
        && _emittere(codificator, CODIFICATOR_A64_RET, LXIV, 0, XXX, 0,
        0));
    introitus = _codicem_ponere(codificator, LOCUS_FLUITANTIS);
    CREDO_NECESSE_NON_NIHIL (introitus);
    memset(&argumenta, ZEPHYRUM, magnitudo(argumenta));
    per (k = ZEPHYRUM; k < VIII; k++)
    {
        argumenta.d[k] = (f64)(k + I) * 1.5;
    }
    argumenta.numerus_d = VIII;
    fructus = officina_effugium_vocare(introitus, &argumenta);
    CREDO_AEQUALIS_S32 (fructus.genus, EFFUGIUM_REDDITUM);
    CREDO_VERUM (fructus.d[0] == 1.5 - 12.0);
    CREDO_VERUM (fructus.d[1] == 3.0 + 10.5);
    CREDO_VERUM (fructus.d[2] == 4.5 + 9.0);
    CREDO_VERUM (fructus.d[3] == 6.0 + 7.5);
}


    /* ==================================================
     * PROBARE: verba stivae (numerus impar: ordinatio XVI servatur)
     * ================================================== */

interior vacuum
_sectio_stiva (
    CredoContextus* c)
{
       CodificatorA64* codificator;
               vacuum* introitus;
    EffugiumArgumenta  argumenta;
      EffugiumFructus  fructus;
                  i64  verba[III];

    CREDO_NECESSE (_parare(c->piscina));
    codificator = codificator_a64_creare(c->piscina);
    CREDO_NECESSE_NON_NIHIL (codificator);
    /* x0 = [sp] + [sp+8]*16 + [sp+16]*256; x1 = sp & 15; ret */
    CREDO_NECESSE (
           _emittere(codificator, CODIFICATOR_A64_LDR_IMM, VIII, 0,
           XXXI, 0, 0)
        && _emittere(codificator, CODIFICATOR_A64_LDR_IMM, VIII, IX,
        XXXI, 0,
               VIII)
        && _emittere(codificator, CODIFICATOR_A64_MOVZ, LXIV, X, 0, 0,
        XVI)
        && _emittere(codificator, CODIFICATOR_A64_MADD, LXIV, 0, IX, X,
        0)
        && _emittere(codificator, CODIFICATOR_A64_LDR_IMM, VIII, IX,
        XXXI, 0,
               XVI)
        && _emittere(codificator, CODIFICATOR_A64_MOVZ, LXIV, X, 0, 0,
        CCLVI)
        && _emittere(codificator, CODIFICATOR_A64_MADD, LXIV, 0, IX, X,
        0)
        && _emittere(codificator, CODIFICATOR_A64_ADD_IMM, LXIV, IX,
        XXXI, 0,
               0)
        && _emittere(codificator, CODIFICATOR_A64_MOVZ, LXIV, X, 0, 0,
        XV)
        && _emittere(codificator, CODIFICATOR_A64_AND_REG, LXIV, I, IX,
        X, 0)
        && _emittere(codificator, CODIFICATOR_A64_RET, LXIV, 0, XXX, 0,
        0));
    introitus = _codicem_ponere(codificator, LOCUS_STIVAE);
    CREDO_NECESSE_NON_NIHIL (introitus);
    memset(&argumenta, ZEPHYRUM, magnitudo(argumenta));
    verba[0] = (i64)III;
    verba[1] = (i64)V;
    verba[2] = (i64)VII;
    argumenta.stiva = verba;
    argumenta.numerus_stivae = III;
    fructus = officina_effugium_vocare(introitus, &argumenta);
    CREDO_AEQUALIS_S32 (fructus.genus, EFFUGIUM_REDDITUM);
    CREDO_AEQUALIS_I64 (fructus.x[0], (i64)(III + V * XVI
        + VII * CCLVI));
    CREDO_AEQUALIS_I64 (fructus.x[1], (i64)ZEPHYRUM);
}


    /* ==================================================
     * PROBARE: saltus (salire ex codice nativo; effugium iterum
     * utile)
     * ================================================== */

/* stipes saltus: x0 = 7; x1 = 0x1234 (pc ficta); x2 = 0x5678 (fp
 * ficta); x16 = &officina_effugium_salire; blr x16. Reddit introitum
 * (NIHIL si quid fractum). Inscriptio per codificator_a64_constans
 * (verba cruda), ergo manu compositum. */
interior vacuum*
_saltum_ponere (
    Piscina* piscina)
{
        MemoriaExsecutabilis* area = officina_effugium_area();
              CodificatorA64* codificator =
                  codificator_a64_creare(piscina);
    CodificatorA64Instructio blr = _instructio(CODIFICATOR_A64_BLR,
        LXIV, 0, XVI, 0, 0);
                  vacuum* salire_locus;
                  chorda  causa;
                      i8* basis;
                     i32  verba[IV];
                     i32  cauda;
                     s32  n;
                     s32  k;
    vacuum (*salire)(s64, memoriae_index, memoriae_index)
        = officina_effugium_salire;

    causa.mensura = ZEPHYRUM;
    si (area == NIHIL || codificator == NIHIL)
    {
        redde NIHIL;
    }
    memcpy(&salire_locus, &salire, magnitudo(salire_locus));
    n = codificator_a64_constans((i64)(memoriae_index)salire_locus,
        LXIV, XVI, verba);
    si (   !_emittere(codificator, CODIFICATOR_A64_MOVZ, LXIV, 0, 0, 0,
        VII)
        || !_emittere(codificator, CODIFICATOR_A64_MOVZ, LXIV, I, 0, 0,
        0x1234)
        || !_emittere(codificator, CODIFICATOR_A64_MOVZ, LXIV, II, 0, 0,
               0x5678)
        || !codificator_a64_codificare(&blr, &cauda)
        || !codificator_a64_finire(codificator, &causa))
    {
        redde NIHIL;
    }
    basis = (i8*)memoria_exsecutabilis_basis(area) + LOCUS_SALTUS;
    memoria_exsecutabilis_aperire(area);
    memcpy(basis, codificator_a64_verba(codificator), XII);
    per (k = ZEPHYRUM; k < n; k++)
    {
        memcpy(basis + XII + (memoriae_index)k * IV, &verba[k], IV);
    }
    memcpy(basis + XII + (memoriae_index)n * IV, &cauda, IV);
    si (!memoria_exsecutabilis_sigillare(area, LOCUS_SALTUS,
            XII + ((memoriae_index)n + I) * IV))
    {
        redde NIHIL;
    }
    redde basis;
}

interior vacuum
_sectio_saltus (
    CredoContextus* c)
{
               vacuum* introitus;
               vacuum* incrementum;
    EffugiumArgumenta  argumenta;
      EffugiumFructus  fructus;

    CREDO_NECESSE (_parare(c->piscina));
    introitus = _saltum_ponere(c->piscina);
    CREDO_NECESSE_NON_NIHIL (introitus);
    memset(&argumenta, ZEPHYRUM, magnitudo(argumenta));
    fructus = officina_effugium_vocare(introitus, &argumenta);
    CREDO_AEQUALIS_S32 (fructus.genus, EFFUGIUM_SALTUS);
    CREDO_AEQUALIS_I64 ((i64)fructus.codex, (i64)VII);
    CREDO_VERUM (fructus.pc == (memoriae_index)0x1234);
    CREDO_VERUM (fructus.fp == (memoriae_index)0x5678);

    /* post saltum effugium iterum utile */
    incrementum = _incrementum_ponere(c->piscina);
    CREDO_NECESSE_NON_NIHIL (incrementum);
    argumenta.x[0] = (i64)XLI;
    argumenta.numerus_x = I;
    fructus = officina_effugium_vocare(incrementum, &argumenta);
    CREDO_AEQUALIS_S32 (fructus.genus, EFFUGIUM_REDDITUM);
    CREDO_AEQUALIS_S32 ((s32)(fructus.x[0] & 0xFFFFFFFFUL), XLII);
}


    /* ==================================================
     * PROBARE: vitium (lectio per monstratorem nullum in codice
     * nativo; processus supervivit, effugium iterum utile)
     * ================================================== */

interior vacuum
_sectio_vitium (
    CredoContextus* c)
{
       CodificatorA64* codificator;
               vacuum* introitus;
               vacuum* incrementum;
    EffugiumArgumenta  argumenta;
      EffugiumFructus  fructus;
                  s32  priores = _prior_vocatus;

    CREDO_NECESSE (_parare(c->piscina));
    codificator = codificator_a64_creare(c->piscina);
    CREDO_NECESSE_NON_NIHIL (codificator);
    /* movz x9,#0; ldr x0,[x9]; ret */
    CREDO_NECESSE (
           _emittere(codificator, CODIFICATOR_A64_MOVZ, LXIV, IX, 0, 0,
           0)
        && _emittere(codificator, CODIFICATOR_A64_LDR_IMM, VIII, 0, IX,
        0, 0)
        && _emittere(codificator, CODIFICATOR_A64_RET, LXIV, 0, XXX, 0,
        0));
    introitus = _codicem_ponere(codificator, LOCUS_VITII);
    CREDO_NECESSE_NON_NIHIL (introitus);
    memset(&argumenta, ZEPHYRUM, magnitudo(argumenta));
    fructus = officina_effugium_vocare(introitus, &argumenta);
    CREDO_AEQUALIS_S32 (fructus.genus, EFFUGIUM_VITIUM);
    CREDO_AEQUALIS_S32 (fructus.signum_systematis, (s32)SIGSEGV);
    /* positio: instructio altera (ldr) */
    CREDO_VERUM (fructus.pc == (memoriae_index)introitus + IV);
    /* vitium INTRA aream: tractator prior non vocatur */
    CREDO_AEQUALIS_S32 (_prior_vocatus, priores);

    incrementum = _incrementum_ponere(c->piscina);
    CREDO_NECESSE_NON_NIHIL (incrementum);
    argumenta.x[0] = (i64)XLI;
    argumenta.numerus_x = I;
    fructus = officina_effugium_vocare(incrementum, &argumenta);
    CREDO_AEQUALIS_S32 (fructus.genus, EFFUGIUM_REDDITUM);
    CREDO_AEQUALIS_S32 ((s32)(fructus.x[0] & 0xFFFFFFFFUL), XLII);
}


    /* ==================================================
     * PROBARE: instructio illicita (verbum zephyrum = udf) -> SIGILL
     * ================================================== */

interior vacuum
_sectio_illicitum (
    CredoContextus* c)
{
    MemoriaExsecutabilis* area;
                      i8* basis;
                     i32  verbum = ZEPHYRUM;
       EffugiumArgumenta  argumenta;
         EffugiumFructus  fructus;

    CREDO_NECESSE (_parare(c->piscina));
    area = officina_effugium_area();
    CREDO_NECESSE_NON_NIHIL (area);
    basis = (i8*)memoria_exsecutabilis_basis(area) + LOCUS_ILLICITI;
    memoria_exsecutabilis_aperire(area);
    memcpy(basis, &verbum, IV);
    CREDO_NECESSE (memoria_exsecutabilis_sigillare(area,
        LOCUS_ILLICITI, IV));
    memset(&argumenta, ZEPHYRUM, magnitudo(argumenta));
    fructus = officina_effugium_vocare(basis, &argumenta);
    CREDO_AEQUALIS_S32 (fructus.genus, EFFUGIUM_VITIUM);
    CREDO_AEQUALIS_S32 (fructus.signum_systematis, (s32)SIGILL);
    CREDO_VERUM (fructus.pc == (memoriae_index)basis);
}


    /* ==================================================
     * PROBARE: tractator semel installatus
     * ================================================== */

interior vacuum
_sectio_installationes (
    CredoContextus* c)
{
    CREDO_NECESSE (_parare(c->piscina));
    CREDO_VERUM (_parare(c->piscina));
    CREDO_VERUM (_parare(c->piscina));
    CREDO_AEQUALIS_S32 (officina_effugium_installationes(), I);
}


    /* ==================================================
     * PROBARE: catena (vitium EXTRA aream, nulla vocatio pendens ->
     * tractator prior suitae)
     * ================================================== */

interior vacuum
_sectio_catena (
    CredoContextus* c)
{
     volatilis memoriae_index  inscriptio = (memoriae_index)XVI;
                volatilis s32* monstrator;
                          s32  priores     = _prior_vocatus;

    CREDO_NECESSE (_parare(c->piscina));
    si (sigsetjmp(_ambitus_prioris, I) == ZEPHYRUM)
    {
        /* scriptura in paginam nullam, in codice C (extra aream) */
        monstrator   = (volatilis s32*)inscriptio;
        *monstrator  = I;
    }
    CREDO_AEQUALIS_S32 (_prior_vocatus, priores + I);
    CREDO_AEQUALIS_S32 (_prior_signum, (s32)SIGSEGV);
}


    /* ==================================================
     * MACHINULA: functio interpretata in NATIVAM versa (gradus III)
     * ================================================== */

interior chorda
_ch (
    constans character* literis)
{
    chorda ch;
    unio { constans character* c; i8* m; } u;

    u.c         = literis;
    ch.datum    = u.m;
    ch.mensura  = (i32)strlen(literis);
    redde ch;
}

/* textus -> machinula parata (regio/conexio recentes) */
interior Machinula*
_machinulam_parare (
               Piscina* piscina,
    constans character* textus)
{
               i32  linea_erroris = ZEPHYRUM;
    MedullaModulus* modulus = medulla_textum_legere(piscina,
        _ch(textus), &linea_erroris);
             Regio* regio;
           Conexio* conexio;

    si (modulus == NIHIL)
    {
        imprimere("FRACTA lectio (linea %d)\n", (int)linea_erroris);
        redde NIHIL;
    }
    regio = regio_generare(piscina);
    si (regio == NIHIL)
    {
        redde NIHIL;
    }
    _regio_sectionis  = regio;   /* purgare destruit */
    conexio           = conexio_creare(piscina, regio);
    si (   !conexio_modulum_addere(conexio, modulus)
        || !conexio_nectere(conexio))
    {
        redde NIHIL;
    }
    _conexio_sectionis = conexio;
    redde machinula_creare(piscina, conexio, regio);
}

/* fcvtzs w9, d0; add w0, w9, w0; ret - f64 et s32 mixta: classes
 * separatae probantur (f64 in d0, s32 in x0 - non in x1) */
interior vacuum*
_mixtum_ponere (
    Piscina* piscina)
{
              CodificatorA64* codificator =
                  codificator_a64_creare(piscina);
    CodificatorA64Instructio conversio = _instructio(
        CODIFICATOR_A64_FCVTZS, XXXII, IX, 0, 0, 0);

    conversio.translatio = LXIV;
    si (   codificator == NIHIL
        || !codificator_a64_emittere(codificator, &conversio)
        || !_emittere(codificator, CODIFICATOR_A64_ADD_REG, XXXII, 0,
        IX, 0, 0)
        || !_emittere(codificator, CODIFICATOR_A64_RET, LXIV, 0, XXX, 0,
        0))
    {
        redde NIHIL;
    }
    redde _codicem_ponere(codificator, LOCUS_MIXTI);
}

#define TEXTUS_VOCATI \
    "modulus \"probatio/nativum.medulla\"\n" \
    "\n" \
    "functio $f (f64 %a, s32 %b) -> s32\n" \
    "@initium:\n" \
    "    redde 0\n" \
    "\n" \
    "functio $g (s32 %a) -> s32\n" \
    "@initium:\n" \
    "    redde 0\n" \
    "\n" \
    "functio $main () -> s32\n" \
    "@initium:\n" \
    "    %t = vocare.s32 $f, f64 2.5, s32 40\n" \
    "    redde %t\n" \
    "\n" \
    "functio $alter () -> s32\n" \
    "@initium:\n" \
    "    %t = vocare.s32 $g, s32 41\n" \
    "    redde %t\n" \
    "\n" \
    "functio $novem () -> s32\n" \
    "@initium:\n" \
    "    %t = vocare.s32 $g, s32 1, s32 2, s32 3, s32 4, s32 5, " \
    "s32 6, s32 7, s32 8, s32 9\n" \
    "    redde %t\n" \
    "\n" \
    "functio $ansa (s32 %n) -> s32\n" \
    "@initium:\n" \
    "    %s = movere.s32 0\n" \
    "    %i = movere.s32 0\n" \
    "    salire @proba\n" \
    "@proba:\n" \
    "    %c = minor.s32 %i, %n\n" \
    "    ramus %c -> @corpus | @finis\n" \
    "@corpus:\n" \
    "    %s = vocare.s32 $g, s32 %s\n" \
    "    %i = addere.s32 %i, 1\n" \
    "    salire @proba\n" \
    "@finis:\n" \
    "    redde %s\n" \
    "\n" \
    "functio $brevis () -> s32\n" \
    "@initium:\n" \
    "    %t = vocare.s32 $ansa, s32 10\n" \
    "    redde %t\n" \
    "\n" \
    "functio $longa () -> s32\n" \
    "@initium:\n" \
    "    %t = vocare.s32 $ansa, s32 1000000\n" \
    "    redde %t\n"

interior vacuum
_sectio_nativum_vocatum (
    CredoContextus* c)
{
          Machinula* m;
             vacuum* mixtum;
    MachinulaExitus  exitus;

    CREDO_NECESSE (_parare(c->piscina));
    mixtum = _mixtum_ponere(c->piscina);
    CREDO_NECESSE_NON_NIHIL (mixtum);
    m = _machinulam_parare(c->piscina, TEXTUS_VOCATI);
    CREDO_NECESSE_NON_NIHIL (m);
    /* ante versionem: $f interpretata 0 reddit */
    exitus = machinula_currere(m, _ch("main"));
    CREDO_AEQUALIS_S32 (exitus.genus, MACHINULA_BENE);
    CREDO_VERUM (exitus.codex == (s64)ZEPHYRUM);
    CREDO_FALSUM (machinula_nativum_ponere(m, _ch("ignota"), mixtum));
    CREDO_VERUM (machinula_nativum_ponere(m, _ch("f"), mixtum));
    /* 2.5 -> 2 (ad zephyrum), + 40 */
    exitus = machinula_currere(m, _ch("main"));
    CREDO_AEQUALIS_S32 (exitus.genus, MACHINULA_BENE);
    CREDO_VERUM (exitus.codex == (s64)XLII);
}

interior vacuum
_sectio_nativum_principale (
    CredoContextus* c)
{
     CodificatorA64* codificator;
          Machinula* m;
             vacuum* introitus;
    MachinulaExitus  exitus;

    CREDO_NECESSE (_parare(c->piscina));
    codificator = codificator_a64_creare(c->piscina);
    CREDO_NECESSE_NON_NIHIL (codificator);
    /* movz w0, #42; ret */
    CREDO_NECESSE (
           _emittere(codificator, CODIFICATOR_A64_MOVZ, XXXII, 0, 0, 0,
           XLII)
        && _emittere(codificator, CODIFICATOR_A64_RET, LXIV, 0, XXX, 0,
        0));
    introitus = _codicem_ponere(codificator, LOCUS_PRINCIPALIS);
    CREDO_NECESSE_NON_NIHIL (introitus);
    m = _machinulam_parare(c->piscina, TEXTUS_VOCATI);
    CREDO_NECESSE_NON_NIHIL (m);
    CREDO_VERUM (machinula_nativum_ponere(m, _ch("main"), introitus));
    exitus = machinula_currere(m, _ch("main"));
    CREDO_AEQUALIS_S32 (exitus.genus, MACHINULA_BENE);
    CREDO_VERUM (exitus.codex == (s64)XLII);
    /* aperire: iam finita, non gradibilis */
    CREDO_FALSUM (machinula_aperire(m, _ch("main")));
    CREDO_VERUM (machinula_halitus_codex(m) == (s64)XLII);
}

interior vacuum
_sectio_nativum_saltus (
    CredoContextus* c)
{
          Machinula* m;
             vacuum* stipes;
             vacuum* incrementum;
    MachinulaExitus  exitus;

    CREDO_NECESSE (_parare(c->piscina));
    stipes       = _saltum_ponere(c->piscina);
    incrementum  = _incrementum_ponere(c->piscina);
    CREDO_NECESSE_NON_NIHIL (stipes);
    CREDO_NECESSE_NON_NIHIL (incrementum);
    m = _machinulam_parare(c->piscina, TEXTUS_VOCATI);
    CREDO_NECESSE_NON_NIHIL (m);
    CREDO_VERUM (machinula_nativum_ponere(m, _ch("f"), stipes));
    CREDO_VERUM (machinula_nativum_ponere(m, _ch("g"), incrementum));
    /* $main -> $f -> salire(7): halitus VITIUM, codex causae */
    exitus = machinula_currere(m, _ch("main"));
    CREDO_AEQUALIS_S32 (exitus.genus, MACHINULA_VITIUM);
    CREDO_VERUM (exitus.codex == (s64)VII);
    CREDO_CHORDA_CONTINET (exitus.nuntius, _ch("saltus"));
    /* machinula iterum utilis: $alter -> $g(41) = 42 */
    exitus = machinula_currere(m, _ch("alter"));
    CREDO_AEQUALIS_S32 (exitus.genus, MACHINULA_BENE);
    CREDO_VERUM (exitus.codex == (s64)XLII);
}

interior vacuum
_sectio_nativum_vitium (
    CredoContextus* c)
{
     CodificatorA64* codificator;
          Machinula* m;
             vacuum* nullum;
             vacuum* incrementum;
    MachinulaExitus  exitus;

    CREDO_NECESSE (_parare(c->piscina));
    codificator = codificator_a64_creare(c->piscina);
    CREDO_NECESSE_NON_NIHIL (codificator);
    /* movz x9,#0; ldr x0,[x9]; ret */
    CREDO_NECESSE (
           _emittere(codificator, CODIFICATOR_A64_MOVZ, LXIV, IX, 0, 0,
           0)
        && _emittere(codificator, CODIFICATOR_A64_LDR_IMM, VIII, 0, IX,
        0, 0)
        && _emittere(codificator, CODIFICATOR_A64_RET, LXIV, 0, XXX, 0,
        0));
    nullum       = _codicem_ponere(codificator, LOCUS_NULLI);
    incrementum  = _incrementum_ponere(c->piscina);
    CREDO_NECESSE_NON_NIHIL (nullum);
    CREDO_NECESSE_NON_NIHIL (incrementum);
    m = _machinulam_parare(c->piscina, TEXTUS_VOCATI);
    CREDO_NECESSE_NON_NIHIL (m);
    CREDO_VERUM (machinula_nativum_ponere(m, _ch("f"), nullum));
    CREDO_VERUM (machinula_nativum_ponere(m, _ch("g"), incrementum));
    exitus = machinula_currere(m, _ch("main"));
    CREDO_AEQUALIS_S32 (exitus.genus, MACHINULA_VITIUM);
    CREDO_CHORDA_CONTINET (exitus.nuntius, _ch("nativum: signum"));
    exitus = machinula_currere(m, _ch("alter"));
    CREDO_AEQUALIS_S32 (exitus.genus, MACHINULA_BENE);
    CREDO_VERUM (exitus.codex == (s64)XLII);
}

/* argumenta integra ultra octavum in stivam eunt (T8 gradus III):
 * functio nativa manu scripta nonum ex [sp] legit (s32, IV octeti) et
 * primo addit - 1 + 9 */
interior vacuum
_sectio_nativum_nimia (
    CredoContextus* c)
{
     CodificatorA64* codificator;
          Machinula* m;
             vacuum* introitus;
    MachinulaExitus  exitus;

    CREDO_NECESSE (_parare(c->piscina));
    codificator = codificator_a64_creare(c->piscina);
    CREDO_NECESSE_NON_NIHIL (codificator);
    /* ldr w9, [sp]; add w0, w0, w9; ret */
    CREDO_NECESSE (
           _emittere(codificator, CODIFICATOR_A64_LDR_IMM, IV, IX, XXXI,
               0, 0)
        && _emittere(codificator, CODIFICATOR_A64_ADD_REG, XXXII, 0, 0,
               IX, 0)
        && _emittere(codificator, CODIFICATOR_A64_RET, LXIV, 0, XXX, 0,
               0));
    introitus = _codicem_ponere(codificator, LOCUS_NONI);
    CREDO_NECESSE_NON_NIHIL (introitus);
    m = _machinulam_parare(c->piscina, TEXTUS_VOCATI);
    CREDO_NECESSE_NON_NIHIL (m);
    CREDO_VERUM (machinula_nativum_ponere(m, _ch("g"), introitus));
    exitus = machinula_currere(m, _ch("novem"));
    CREDO_AEQUALIS_S32 (exitus.genus, MACHINULA_BENE);
    CREDO_VERUM (exitus.codex == (s64)X);
}

/* RF4: ansa interpretata decies centena milia vocationum ad folium
 * nativum - summa recta, apex stivae idem ac post X vocationes,
 * tractator semel installatus */
interior vacuum
_sectio_transitus (
    CredoContextus* c)
{
          Machinula* m;
             vacuum* incrementum;
    MachinulaExitus  exitus;
     memoriae_index  apex_brevis;

    CREDO_NECESSE (_parare(c->piscina));
    incrementum = _incrementum_ponere(c->piscina);
    CREDO_NECESSE_NON_NIHIL (incrementum);
    m = _machinulam_parare(c->piscina, TEXTUS_VOCATI);
    CREDO_NECESSE_NON_NIHIL (m);
    CREDO_VERUM (machinula_nativum_ponere(m, _ch("g"), incrementum));
    exitus = machinula_currere(m, _ch("brevis"));
    CREDO_AEQUALIS_S32 (exitus.genus, MACHINULA_BENE);
    CREDO_VERUM (exitus.codex == (s64)X);
    apex_brevis  = machinula_stiva_apex(m);
    exitus       = machinula_currere(m, _ch("longa"));
    CREDO_AEQUALIS_S32 (exitus.genus, MACHINULA_BENE);
    CREDO_VERUM (exitus.codex == (s64)1000000L);
    CREDO_VERUM (machinula_stiva_apex(m) == apex_brevis);
    CREDO_AEQUALIS_S32 (officina_effugium_installationes(), I);
}


    /* ==================================================
     * DORSUM (M5 T7): functio medullae in codicem nativum versa;
     * probatio quaeque interpretatam et nativam confert
     * ================================================== */

/* index conexionis functionis nominatae (ANTE versionem: post eam
 * index descriptoris introitus est); -I si absens */
interior s32
_indicem_functionis (
    constans character* titulus)
{
    constans ConexioSymbolum* symbolum;
                         s32  globale;

    si (_conexio_sectionis == NIHIL)
    {
        redde -I;
    }
    globale = conexio_symbolum_quaerere(_conexio_sectionis,
        _ch(titulus));
    si (globale < ZEPHYRUM)
    {
        redde -I;
    }
    symbolum = conexio_symbolum_obtinere(_conexio_sectionis, globale);
    si (symbolum->genus != CONEXIO_SYMBOLUM_FUNCTIO)
    {
        redde -I;
    }
    redde (s32)((constans ConexioDescriptor*)symbolum->sedes)->index;
}

/* functionem per dorsum compilat et in machinula nativam ponit;
 * FALSUM + causa si dorsum recusat */
interior b32
_nativam_facere (
               Piscina* piscina,
             Machinula* m,
    constans character* titulus,
                chorda* causa_out)
{
    OfficinaDorsum* dorsum;
               s32  index = _indicem_functionis(titulus);
            vacuum* introitus;

    causa_out->mensura = ZEPHYRUM;
    si (index < ZEPHYRUM)
    {
        *causa_out = _ch("probatio: functio ignota");
        redde FALSUM;
    }
    /* dorsum UNUM per sectionem: alterum codicem prioris in area
     * superscriberet */
    si (_dorsum_sectionis == NIHIL)
    {
        _dorsum_sectionis = officina_dorsum_creare(piscina,
            _conexio_sectionis, officina_effugium_area());
    }
    dorsum = _dorsum_sectionis;
    si (dorsum == NIHIL)
    {
        *causa_out = _ch("probatio: dorsum creari nequit");
        redde FALSUM;
    }
    si (!officina_dorsum_compilare(dorsum, index, causa_out))
    {
        redde FALSUM;
    }
    introitus = officina_dorsum_introitus(dorsum, index);
    redde introitus != NIHIL
        && machinula_nativum_ponere(m, _ch(titulus), introitus);
}

/* $main interpretatam currit, deinde nativam; ambae exspectatum
 * reddere debent */
interior vacuum
_utramque_conferre (
        CredoContextus* c,
    constans character* textus,
                   s64  exspectatum)
{
          Machinula* m;
    MachinulaExitus  exitus;
             chorda  causa;

    CREDO_NECESSE (_parare(c->piscina));
    m = _machinulam_parare(c->piscina, textus);
    CREDO_NECESSE_NON_NIHIL (m);
    exitus = machinula_currere(m, _ch("main"));
    CREDO_AEQUALIS_S32 (exitus.genus, MACHINULA_BENE);
    CREDO_VERUM (exitus.codex == exspectatum);
    CREDO_VERUM (_nativam_facere(c->piscina, m, "main", &causa));
    CREDO_VERUM (causa.mensura == ZEPHYRUM);
    exitus = machinula_currere(m, _ch("main"));
    CREDO_AEQUALIS_S32 (exitus.genus, MACHINULA_BENE);
    CREDO_VERUM (exitus.codex == exspectatum);
}

interior vacuum
_sectio_dorsum_recta (
    CredoContextus* c)
{
    _utramque_conferre(c,
        "modulus \"probatio/recta.medulla\"\n"
        "\n"
        "functio $main () -> s32\n"
        "@initium:\n"
        "    %a = movere.s32 100\n"
        "    %b = movere.s32 7\n"
        "    %c = multiplicare.s32 %a, %b\n"
        "    %d = subtrahere.s32 %c, 658\n"
        "    %e = negare.s32 %d\n"
        "    %f = negare.s32 %e\n"
        "    %g = addere.s32 %f, 70000\n"
        "    %h = subtrahere.s32 %g, 70000\n"
        "    redde %h\n",
        (s64)XLII);
    _sectionem_purgare(c);
    /* registrum lectum ante omnem definitionem: zephyrus (F13) -
     * dorsum id in prologo purgat (x9 sordidum a probatione priore) */
    _utramque_conferre(c,
        "modulus \"probatio/ante.medulla\"\n"
        "\n"
        "functio $main () -> s32\n"
        "@initium:\n"
        "    %s = addere.s32 %s, 42\n"
        "    redde %s\n",
        (s64)XLII);
}

interior vacuum
_sectio_dorsum_bitorum (
    CredoContextus* c)
{
    constans character* radix = getenv("RHUBARB_RADIX");
             character  via[CCLVI];
                chorda  textus;
             character* literae;

    sprintf(via, "%s/officina/probationes/fixa/machinula/"
        "bitalia.medulla", (radix != NIHIL) ? radix : ".");
    textus = filum_legere_totum(via, c->piscina);
    CREDO_NECESSE (textus.mensura > ZEPHYRUM);
    literae = piscina_allocare(c->piscina,
        (memoriae_index)textus.mensura + I);
    CREDO_NECESSE_NON_NIHIL (literae);
    memcpy(literae, textus.datum, (memoriae_index)textus.mensura);
    literae[textus.mensura] = '\0';
    _utramque_conferre(c, literae, (s64)XLII);
}

/* registrum lectum ante definitionem in registro SORDIDO: functio
 * directe per effugium vocatur cum verbis stivae - trampolinum x10
 * (cursorem fontis, non zephyrum) relinquit, et %s in x10 sedet
 * (parametrum x9 capit). Sine purgatione prologi summa falsa. */
interior vacuum
_sectio_dorsum_ante_definitionem (
    CredoContextus* c)
{
            Machinula* m;
    EffugiumArgumenta  argumenta;
      EffugiumFructus  fructus;
               chorda  causa;
                  i64  verba[III];
                  s32  index;
               vacuum* introitus;

    CREDO_NECESSE (_parare(c->piscina));
    m = _machinulam_parare(c->piscina,
        "modulus \"probatio/sordidum.medulla\"\n"
        "\n"
        "functio $f (s32 %p) -> s32\n"
        "@initium:\n"
        "    %s = addere.s32 %s, %p\n"
        "    redde %s\n");
    CREDO_NECESSE_NON_NIHIL (m);
    index = _indicem_functionis("f");
    CREDO_NECESSE (_nativam_facere(c->piscina, m, "f", &causa));
    introitus = officina_dorsum_introitus(_dorsum_sectionis, index);
    CREDO_NECESSE_NON_NIHIL (introitus);
    memset(&argumenta, ZEPHYRUM, magnitudo(argumenta));
    verba[0] = (i64)I;
    verba[1] = (i64)II;
    verba[2] = (i64)III;
    argumenta.x[0] = (i64)XLII;
    argumenta.numerus_x = I;
    argumenta.stiva = verba;
    argumenta.numerus_stivae = III;
    fructus = officina_effugium_vocare(introitus, &argumenta);
    CREDO_AEQUALIS_S32 (fructus.genus, EFFUGIUM_REDDITUM);
    CREDO_AEQUALIS_S32 ((s32)(fructus.x[0] & 0xFFFFFFFFUL), XLII);
}

/* XX registra simul viva + summa: plura quam registra physica
 * (VII temporaria + X servata) - effusio in loculos tabulati */
interior vacuum
_sectio_dorsum_effusio (
    CredoContextus* c)
{
    character  textus[4096];
    character* cursor = textus;
          s32  k;

    cursor += sprintf(cursor,
        "modulus \"probatio/effusio.medulla\"\n\n"
        "functio $main () -> s32\n@initium:\n");
    per (k = I; k <= XX; k++)
    {
        cursor += sprintf(cursor, "    %%r%d = movere.s32 %d\n",
            (int)k, (int)k);
    }
    cursor += sprintf(cursor, "    %%s = movere.s32 0\n");
    per (k = I; k <= XX; k++)
    {
        cursor += sprintf(cursor, "    %%s = addere.s32 %%s, %%r%d\n",
            (int)k);
    }
    /* 1 + ... + 20 = 210; ponderata iterum: ordo loculorum probatur */
    per (k = I; k <= XX; k++)
    {
        cursor += sprintf(cursor,
            "    %%m%d = multiplicare.s32 %%r%d, %d\n"
            "    %%s = addere.s32 %%s, %%m%d\n",
            (int)k, (int)k, (int)k, (int)k);
    }
    /* + 1^2 + ... + 20^2 = 2870; 210 + 2870 - 3038 = 42 */
    sprintf(cursor, "    %%t = subtrahere.s32 %%s, 3038\n"
        "    redde %%t\n");
    _utramque_conferre(c, textus, (s64)XLII);
}

interior vacuum
_sectio_dorsum_parametra (
    CredoContextus* c)
{
          Machinula* m;
    MachinulaExitus  exitus;
             chorda  causa;

    CREDO_NECESSE (_parare(c->piscina));
    m = _machinulam_parare(c->piscina,
        "modulus \"probatio/parametra.medulla\"\n"
        "\n"
        "functio $p (s32 %a, s32 %b, s64 %c, s32 %d, s32 %e, s32 %f, "
        "s32 %g, s32 %h) -> s32\n"
        "@initium:\n"
        "    %u = subtrahere.s64 %c, 123456789000\n"
        "    %s = subtrahere.s32 %a, %b\n"
        "    %s = addere.s32 %s, %d\n"
        "    %s = subtrahere.s32 %s, %e\n"
        "    %s = addere.s32 %s, %f\n"
        "    %s = subtrahere.s32 %s, %g\n"
        "    %s = addere.s32 %s, %h\n"
        "    redde %s\n"
        "\n"
        "functio $q (s64 %c) -> s64\n"
        "@initium:\n"
        "    %u = subtrahere.s64 %c, 123456789000\n"
        "    redde %u\n"
        "\n"
        "functio $main () -> s32\n"
        "@initium:\n"
        "    %t = vocare.s32 $p, s32 50, s32 8, s64 123456789012, "
        "s32 1, s32 2, s32 3, s32 4, s32 2\n"
        "    redde %t\n"
        "\n"
        "functio $longa () -> s64\n"
        "@initium:\n"
        "    %t = vocare.s64 $q, s64 123456789012\n"
        "    redde %t\n");
    CREDO_NECESSE_NON_NIHIL (m);
    exitus = machinula_currere(m, _ch("main"));
    CREDO_VERUM (exitus.codex == (s64)XLII);
    CREDO_VERUM (_nativam_facere(c->piscina, m, "p", &causa));
    CREDO_VERUM (_nativam_facere(c->piscina, m, "q", &causa));
    exitus = machinula_currere(m, _ch("main"));
    CREDO_AEQUALIS_S32 (exitus.genus, MACHINULA_BENE);
    CREDO_VERUM (exitus.codex == (s64)XLII);
    exitus = machinula_currere(m, _ch("longa"));
    CREDO_AEQUALIS_S32 (exitus.genus, MACHINULA_BENE);
    CREDO_VERUM (exitus.codex == (s64)XII);
}

/* constantes latae et negativae: nativa == interpretata */
interior vacuum
_sectio_dorsum_constantes (
    CredoContextus* c)
{
    _utramque_conferre(c,
        "modulus \"probatio/constantes.medulla\"\n"
        "\n"
        "functio $main () -> s64\n"
        "@initium:\n"
        "    %a = movere.s64 -1\n"
        "    %b = movere.i64 1311768467463790320\n"
        "    %c = et.i64 %a, %b\n"
        "    %d = movere.s64 -65536\n"
        "    %e = aut.i64 %c, %d\n"
        "    %f = movere.s64 -4294967296\n"
        "    %g = vel.i64 %e, %f\n"
        "    %h = addere.s64 %g, 281474976710656\n"
        "    redde %h\n",
        (s64)(((i64)0x123456789ABCDEF0UL ^ (i64)0xFFFFFFFFFFFF0000UL)
            | (i64)0xFFFFFFFF00000000UL) + (s64)0x1000000000000L);
    _sectionem_purgare(c);
    _utramque_conferre(c,
        "modulus \"probatio/negativa.medulla\"\n"
        "\n"
        "functio $main () -> s32\n"
        "@initium:\n"
        "    %a = movere.s32 -5\n"
        "    %b = movere.i32 4000000000\n"
        "    %c = dextrorsum.i32 %b, 31\n"
        "    %d = dextrorsum.s32 %a, 1\n"
        "    %e = addere.s32 %d, %c\n"
        "    redde %e\n",
        (s64)(-II));
}

/* quod dorsum nondum emittit recusatur nominatim; functio
 * interpretata manet (arithmetica XVI bitorum: C promovet, dorsum
 * eam non emittit; ante T9 exemplum erat fluitare) */
interior vacuum
_sectio_dorsum_recusatio_operationis (
    CredoContextus* c)
{
          Machinula* m;
    MachinulaExitus  exitus;
             chorda  causa;

    CREDO_NECESSE (_parare(c->piscina));
    m = _machinulam_parare(c->piscina,
        "modulus \"probatio/arca.medulla\"\n"
        "\n"
        "functio $main () -> s32\n"
        "@initium:\n"
        "    %p = addere.s16 7, 1\n"
        "    redde 42\n");
    CREDO_NECESSE_NON_NIHIL (m);
    CREDO_FALSUM (_nativam_facere(c->piscina, m, "main", &causa));
    CREDO_VERUM (chorda_aequalis_literis(causa,
        "operatio addere nondum nativa"));
    exitus = machinula_currere(m, _ch("main"));
    CREDO_AEQUALIS_S32 (exitus.genus, MACHINULA_BENE);
    CREDO_VERUM (exitus.codex == (s64)XLII);
}


    /* ==================================================
     * DORSUM gradus II: saltus, rami, comparationes
     * ================================================== */

/* fixturam ex arbore legit ($main -> exspectatum) et utramque
 * confert */
interior character*
_fixturam_legere (
        CredoContextus* c,
    constans character* via_partis)
{
    constans character* radix = getenv("RHUBARB_RADIX");
             character  via[CCLVI];
                chorda  textus;
             character* literae;

    sprintf(via, "%s/%s", (radix != NIHIL) ? radix : ".", via_partis);
    textus = filum_legere_totum(via, c->piscina);
    si (textus.mensura <= ZEPHYRUM)
    {
        redde NIHIL;
    }
    literae = piscina_allocare(c->piscina,
        (memoriae_index)textus.mensura + I);
    si (literae == NIHIL)
    {
        redde NIHIL;
    }
    memcpy(literae, textus.datum, (memoriae_index)textus.mensura);
    literae[textus.mensura] = '\0';
    redde literae;
}

interior vacuum
_fixturam_conferre (
        CredoContextus* c,
    constans character* via_partis,
                   s64  exspectatum)
{
    character* literae = _fixturam_legere(c, via_partis);

    CREDO_NECESSE_NON_NIHIL (literae);
    _utramque_conferre(c, literae, exspectatum);
}

interior vacuum
_sectio_dorsum_fixturae_fluxus (
    CredoContextus* c)
{
    _fixturam_conferre(c,
        "officina/probationes/fixa/machinula/comparationes.medulla",
        (s64)XLII);
    _sectionem_purgare(c);
    _fixturam_conferre(c,
        "officina/probationes/fixa/machinula/fluxus.medulla",
        (s64)XLII);
}

/* summa (fixa/summa.medulla) nativa, a $main interpretata vocata:
 * 1 + ... + 8 = 36 */
interior vacuum
_sectio_dorsum_summa (
    CredoContextus* c)
{
          Machinula* m;
    MachinulaExitus  exitus;
             chorda  causa;

    CREDO_NECESSE (_parare(c->piscina));
    m = _machinulam_parare(c->piscina,
        "modulus \"probatio/summa.medulla\"\n"
        "\n"
        "functio $summa (s32 %n) -> s32\n"
        "@initium:\n"
        "    %s = movere.s32 0\n"
        "    %i = movere.s32 1\n"
        "    salire @proba\n"
        "@proba:\n"
        "    %c = maior.s32 %i, %n\n"
        "    ramus %c -> @finis | @corpus\n"
        "@corpus:\n"
        "    %s = addere.s32 %s, %i\n"
        "    %i = addere.s32 %i, 1\n"
        "    salire @proba\n"
        "@finis:\n"
        "    redde %s\n"
        "\n"
        "functio $main () -> s32\n"
        "@initium:\n"
        "    %t = vocare.s32 $summa, s32 8\n"
        "    %u = addere.s32 %t, 6\n"
        "    redde %u\n");
    CREDO_NECESSE_NON_NIHIL (m);
    exitus = machinula_currere(m, _ch("main"));
    CREDO_VERUM (exitus.codex == (s64)XLII);
    CREDO_VERUM (_nativam_facere(c->piscina, m, "summa", &causa));
    exitus = machinula_currere(m, _ch("main"));
    CREDO_AEQUALIS_S32 (exitus.genus, MACHINULA_BENE);
    CREDO_VERUM (exitus.codex == (s64)XLII);
}

/* ramus sine plicatione: fructus comparationis bis lectus; ramus in
 * registro LXIV cuius pars inferior zephyra est (probatio per X) */
interior vacuum
_sectio_dorsum_ramus (
    CredoContextus* c)
{
    _utramque_conferre(c,
        "modulus \"probatio/ramus.medulla\"\n"
        "\n"
        "functio $main () -> s32\n"
        "@initium:\n"
        "    %a = movere.s32 5\n"
        "    %c = minor.s32 %a, 9\n"
        "    ramus %c -> @verum | @falsum\n"
        "@verum:\n"
        "    %r = addere.s32 %c, 41\n"
        "    redde %r\n"
        "@falsum:\n"
        "    redde 0\n",
        (s64)XLII);
    _sectionem_purgare(c);
    _utramque_conferre(c,
        "modulus \"probatio/ramus_latus.medulla\"\n"
        "\n"
        "functio $main () -> s32\n"
        "@initium:\n"
        "    %h = movere.i64 4294967296\n"
        "    ramus %h -> @verum | @falsum\n"
        "@verum:\n"
        "    %z = movere.s32 0\n"
        "    ramus %z -> @falsum | @bonum\n"
        "@bonum:\n"
        "    redde 42\n"
        "@falsum:\n"
        "    redde 0\n",
        (s64)XLII);
}

/* comparationes insignatae et LXIV bitorum; plicatio insignata */
interior vacuum
_sectio_dorsum_comparationes (
    CredoContextus* c)
{
    _utramque_conferre(c,
        "modulus \"probatio/comparationes.medulla\"\n"
        "\n"
        "functio $main () -> s32\n"
        "@initium:\n"
        "    %big = movere.i32 4000000000\n"
        "    %c1 = minor.i32 %big, 1\n"
        "    %c2 = minor.s32 %big, 1\n"
        "    %c3 = maior.i32 %big, 1\n"
        "    %c4 = maior_aequalis.i32 %big, %big\n"
        "    %c5 = minor_aequalis.i32 1, %big\n"
        "    %l = movere.s64 -1\n"
        "    %c6 = minor.s64 %l, 0\n"
        "    %c7 = minor.i64 %l, 0\n"
        "    %c8 = maior.i64 %l, 5\n"
        "    %s = movere.s32 %c1\n"
        "    %t = multiplicare.s32 %c2, 2\n"
        "    %s = addere.s32 %s, %t\n"
        "    %t = multiplicare.s32 %c3, 4\n"
        "    %s = addere.s32 %s, %t\n"
        "    %t = multiplicare.s32 %c4, 8\n"
        "    %s = addere.s32 %s, %t\n"
        "    %t = multiplicare.s32 %c5, 16\n"
        "    %s = addere.s32 %s, %t\n"
        "    %t = multiplicare.s32 %c6, 32\n"
        "    %s = addere.s32 %s, %t\n"
        "    %t = multiplicare.s32 %c7, 64\n"
        "    %s = addere.s32 %s, %t\n"
        "    %t = multiplicare.s32 %c8, 128\n"
        "    %s = addere.s32 %s, %t\n"
        "    redde %s\n",
        (s64)CXC);
    _sectionem_purgare(c);
    /* plicatio: minor.i32 magni insignati -> ramus falsus */
    _utramque_conferre(c,
        "modulus \"probatio/plicatio.medulla\"\n"
        "\n"
        "functio $main () -> s32\n"
        "@initium:\n"
        "    %big = movere.i32 4000000000\n"
        "    %c = minor.i32 %big, 1\n"
        "    ramus %c -> @malum | @alter\n"
        "@alter:\n"
        "    %d = maior_aequalis.s32 %big, 1\n"
        "    ramus %d -> @malum | @tertius\n"
        "@tertius:\n"
        "    %l = movere.s64 -1\n"
        "    %e = minor_aequalis.i64 %l, 7\n"
        "    ramus %e -> @malum | @bonum\n"
        "@bonum:\n"
        "    redde 42\n"
        "@malum:\n"
        "    redde 0\n",
        (s64)XLII);
}

/* ansae: XX registra per ansam viva (effusio in ansa) et ansae
 * nidatae */
interior vacuum
_sectio_dorsum_ansae (
    CredoContextus* c)
{
    character  textus[4096];
    character* cursor = textus;
          s32  k;

    cursor += sprintf(cursor,
        "modulus \"probatio/ansa.medulla\"\n\n"
        "functio $main () -> s32\n@initium:\n");
    per (k = I; k <= XX; k++)
    {
        cursor += sprintf(cursor, "    %%r%d = movere.s32 %d\n",
            (int)k, (int)k);
    }
    cursor += sprintf(cursor,
        "    %%s = movere.s32 0\n"
        "    %%i = movere.s32 0\n"
        "    salire @proba\n"
        "@proba:\n"
        "    %%c = minor.s32 %%i, 3\n"
        "    ramus %%c -> @corpus | @finis\n"
        "@corpus:\n");
    per (k = I; k <= XX; k++)
    {
        cursor += sprintf(cursor, "    %%s = addere.s32 %%s, %%r%d\n",
            (int)k);
    }
    /* 3 * 210 = 630; 630 - 588 = 42 */
    sprintf(cursor,
        "    %%i = addere.s32 %%i, 1\n"
        "    salire @proba\n"
        "@finis:\n"
        "    %%t = subtrahere.s32 %%s, 588\n"
        "    redde %%t\n");
    _utramque_conferre(c, textus, (s64)XLII);
    _sectionem_purgare(c);
    _utramque_conferre(c,
        "modulus \"probatio/nidatae.medulla\"\n"
        "\n"
        "functio $main () -> s32\n"
        "@initium:\n"
        "    %n = movere.s32 0\n"
        "    %i = movere.s32 0\n"
        "    salire @exterior\n"
        "@exterior:\n"
        "    %c = minor.s32 %i, 6\n"
        "    ramus %c -> @praeparatio | @finis\n"
        "@praeparatio:\n"
        "    %j = movere.s32 0\n"
        "    salire @intima\n"
        "@intima:\n"
        "    %d = minor.s32 %j, 7\n"
        "    ramus %d -> @corpus | @incrementum\n"
        "@corpus:\n"
        "    %n = addere.s32 %n, 1\n"
        "    %j = addere.s32 %j, 1\n"
        "    salire @intima\n"
        "@incrementum:\n"
        "    %i = addere.s32 %i, 1\n"
        "    salire @exterior\n"
        "@finis:\n"
        "    redde %n\n",
        (s64)XLII);
}


    /* ==================================================
     * DORSUM gradus III: divisio, sistere, paritas halitus
     * (machinula_nativum_compilare: dorsum machinulae proprium -
     * positio halitus nativi per mappam pc invenitur)
     * ================================================== */

/* status machinulae post cursum: quod interpretata et nativa
 * paria reddere debent */
nomen structura {
       s32 genus;
       s64 codex;
    chorda nuntius;
       i32 tabulata;
       s32 functio_summa;
       i32 instructio_summa;
       s32 functio_ima;
       i32 instructio_ima;
       s64 vestigium;   /* positiones OMNIUM tabulatorum ponderatae */
} StatusCursus;

interior StatusCursus
_statum_capere (
               Piscina* piscina,
             Machinula* m,
    constans character* titulus)
{
       StatusCursus s;
    MachinulaExitus exitus;

    memset(&s, ZEPHYRUM, magnitudo(s));
    exitus              = machinula_currere(m, _ch(titulus));
    s.genus             = exitus.genus;
    s.codex             = exitus.codex;
    s.nuntius           = chorda_transcribere(exitus.nuntius, piscina);
    s.tabulata          = machinula_tabulata_numerus(m);
    s.functio_summa     = -I;
    s.functio_ima       = -I;
    s.instructio_summa  = ZEPHYRUM;
    s.instructio_ima    = ZEPHYRUM;
    si (s.tabulata > ZEPHYRUM)
    {
        (vacuum)machinula_positionem_inspicere(m, s.tabulata - I,
            &s.functio_summa, &s.instructio_summa);
        (vacuum)machinula_positionem_inspicere(m, ZEPHYRUM,
            &s.functio_ima, &s.instructio_ima);
    }
    {
        i32 t;

        per (t = ZEPHYRUM; t < s.tabulata; t++)
        {
            s32 functio     = -I;
            i32 instructio  = ZEPHYRUM;

            (vacuum)machinula_positionem_inspicere(m, t, &functio,
                &instructio);
            s.vestigium += ((s64)functio * 100003L + (s64)instructio)
                * (s64)(t + I);
        }
    }
    redde s;
}

/* VERUM si status pares (genus, codex, nuntius, tabulata, positio
 * summa et ima) */
interior b32
_status_pares (
    constans StatusCursus* a,
    constans StatusCursus* b)
{
    si (   a->genus != b->genus || a->codex != b->codex
        || !chorda_aequalis(a->nuntius, b->nuntius))
    {
        redde FALSUM;
    }
    /* post cursum BENE finitum tabulata non conferuntur: interpretata
     * tabulatum principalis in 'redde' relinquit, principalis nativa
     * nullum (iam finita, non gradibilis) */
    si (a->genus == MACHINULA_BENE)
    {
        redde VERUM;
    }
    redde a->tabulata == b->tabulata
        && a->functio_summa == b->functio_summa
        && a->instructio_summa == b->instructio_summa
        && a->functio_ima == b->functio_ima
        && a->instructio_ima == b->instructio_ima
        && a->vestigium == b->vestigium;
}

#define TEXTUS_DIVISIONIS \
    "modulus \"probatio/divisio.medulla\"\n" \
    "\n" \
    "functio $div (s32 %a, s32 %b) -> s32\n" \
    "@initium:\n" \
    "    %q = dividere.s32 %a, %b\n" \
    "    redde %q\n" \
    "\n" \
    "functio $res (s32 %a, s32 %b) -> s32\n" \
    "@initium:\n" \
    "    %q = residuum.s32 %a, %b\n" \
    "    redde %q\n" \
    "\n" \
    "functio $divi (i32 %a, i32 %b) -> i32\n" \
    "@initium:\n" \
    "    %q = dividere.i32 %a, %b\n" \
    "    %r = residuum.i32 %a, %b\n" \
    "    %s = addere.i32 %q, %r\n" \
    "    redde %s\n" \
    "\n" \
    "functio $divl (s64 %a, s64 %b) -> s64\n" \
    "@initium:\n" \
    "    %q = dividere.s64 %a, %b\n" \
    "    redde %q\n" \
    "\n" \
    "functio $resl (s64 %a, s64 %b) -> s64\n" \
    "@initium:\n" \
    "    %q = residuum.s64 %a, %b\n" \
    "    redde %q\n" \
    "\n" \
    "functio $bene () -> s32\n" \
    "@initium:\n" \
    "    %a = vocare.s32 $div, s32 -7, s32 2\n" \
    "    %b = vocare.s32 $res, s32 -7, s32 2\n" \
    "    %c = vocare.i32 $divi, i32 4000000000, i32 7\n" \
    "    %d = vocare.s64 $divl, s64 -123456789012, s64 1000\n" \
    "    %e = vocare.s64 $resl, s64 -123456789012, s64 1000\n" \
    "    %s = addere.s32 %a, %b\n" \
    "    %s = addere.s32 %s, %c\n" \
    "    %s = addere.s32 %s, %d\n" \
    "    %s = addere.s32 %s, %e\n" \
    "    redde %s\n" \
    "\n" \
    "functio $zephyrus () -> s32\n" \
    "@initium:\n" \
    "    %t = vocare.s32 $div, s32 5, s32 0\n" \
    "    redde %t\n" \
    "\n" \
    "functio $residuum_zephyri () -> s32\n" \
    "@initium:\n" \
    "    %t = vocare.s32 $res, s32 5, s32 0\n" \
    "    redde %t\n" \
    "\n" \
    "functio $minimum_breve () -> s32\n" \
    "@initium:\n" \
    "    %m = sinistrorsum.s32 1, 31\n" \
    "    %t = vocare.s32 $div, s32 %m, s32 -1\n" \
    "    %u = vocare.s32 $res, s32 %m, s32 -1\n" \
    "    %v = addere.s32 %t, %u\n" \
    "    redde %v\n" \
    "\n" \
    "functio $residuum_minimi_brevis () -> s32\n" \
    "@initium:\n" \
    "    %m = sinistrorsum.s32 1, 31\n" \
    "    %t = vocare.s32 $res, s32 %m, s32 -1\n" \
    "    redde %t\n" \
    "\n" \
    "functio $prope_minimum_breve () -> s32\n" \
    "@initium:\n" \
    "    %m = sinistrorsum.s32 1, 31\n" \
    "    %t = vocare.s32 $div, s32 %m, s32 1\n" \
    "    %u = vocare.s32 $div, s32 -5, s32 -1\n" \
    "    %v = addere.s32 %t, %u\n" \
    "    %w = vocare.i32 $divi, i32 %m, i32 4294967295\n" \
    "    %x = addere.s32 %v, %w\n" \
    "    redde %x\n" \
    "\n" \
    "functio $minimum_longum () -> s64\n" \
    "@initium:\n" \
    "    %m = sinistrorsum.s64 1, 63\n" \
    "    %t = vocare.s64 $divl, s64 %m, s64 -1\n" \
    "    redde %t\n" \
    "\n" \
    "functio $residuum_minimi () -> s64\n" \
    "@initium:\n" \
    "    %m = sinistrorsum.s64 1, 63\n" \
    "    %t = vocare.s64 $resl, s64 %m, s64 -1\n" \
    "    redde %t\n" \
    "\n" \
    "functio $prope_minimum () -> s64\n" \
    "@initium:\n" \
    "    %m = sinistrorsum.s64 1, 63\n" \
    "    %t = vocare.s64 $divl, s64 %m, s64 1\n" \
    "    %u = vocare.s64 $divl, s64 -5, s64 -1\n" \
    "    %v = addere.s64 %t, %u\n" \
    "    redde %v\n"

interior vacuum
_sectio_dorsum_divisio (
    CredoContextus* c)
{
    constans character* cursus[] = {
        "bene", "zephyrus", "residuum_zephyri", "minimum_breve",
        "minimum_longum", "residuum_minimi", "prope_minimum",
        "residuum_minimi_brevis", "prope_minimum_breve"
    };
    constans character* nativae[] = {
        "div", "res", "divi", "divl", "resl"
    };
       Machinula* m;
    StatusCursus  ante[IX];
    StatusCursus  post;
          chorda  causa;
             s32  k;

    CREDO_NECESSE (_parare(c->piscina));
    m = _machinulam_parare(c->piscina, TEXTUS_DIVISIONIS);
    CREDO_NECESSE_NON_NIHIL (m);
    per (k = ZEPHYRUM; k < IX; k++)
    {
        ante[k] = _statum_capere(c->piscina, m, cursus[k]);
    }
    /* interpretata: quod lex dicit */
    CREDO_AEQUALIS_S32 (ante[0].genus, MACHINULA_BENE);
    CREDO_AEQUALIS_S32 (ante[1].genus, MACHINULA_VITIUM);
    CREDO_VERUM (chorda_aequalis_literis(ante[1].nuntius,
        "divisio per zephyrum"));
    CREDO_AEQUALIS_S32 (ante[2].genus, MACHINULA_VITIUM);
    /* minimum XXXII / -1 et residuum eius: superfluxus (C: mores
     * indefiniti; arm64 sdiv tacite involvit) - 2026-11-13 */
    CREDO_AEQUALIS_S32 (ante[3].genus, MACHINULA_VITIUM);
    CREDO_VERUM (chorda_aequalis_literis(ante[3].nuntius,
        "divisio: superfluxus"));
    CREDO_AEQUALIS_S32 (ante[7].genus, MACHINULA_VITIUM);
    CREDO_VERUM (chorda_aequalis_literis(ante[7].nuntius,
        "divisio: superfluxus"));
    /* minimum / 1, -5 / -1, insignata minimum / maximum: nullus
     * superfluxus */
    CREDO_AEQUALIS_S32 (ante[8].genus, MACHINULA_BENE);
    CREDO_AEQUALIS_S32 (ante[4].genus, MACHINULA_VITIUM);
    CREDO_VERUM (chorda_aequalis_literis(ante[4].nuntius,
        "divisio: superfluxus"));
    CREDO_AEQUALIS_S32 (ante[5].genus, MACHINULA_VITIUM);
    CREDO_AEQUALIS_S32 (ante[6].genus, MACHINULA_BENE);
    CREDO_AEQUALIS_I32 (ante[1].tabulata, II);

    per (k = ZEPHYRUM; k < V; k++)
    {
        CREDO_VERUM (machinula_nativum_compilare(m, _ch(nativae[k]),
            &causa));
    }
    per (k = ZEPHYRUM; k < IX; k++)
    {
        post = _statum_capere(c->piscina, m, cursus[k]);
        si (!_status_pares(&ante[k], &post))
        {
            imprimere("DISPAR %s: genus %d/%d codex %ld/%ld tabulata "
                "%d/%d summa %d:%d/%d:%d nuntius '%.*s'/'%.*s'\n",
                cursus[k], (int)ante[k].genus, (int)post.genus,
                (long)ante[k].codex, (long)post.codex,
                (int)ante[k].tabulata, (int)post.tabulata,
                (int)ante[k].functio_summa,
                (int)ante[k].instructio_summa,
                (int)post.functio_summa, (int)post.instructio_summa,
                (int)ante[k].nuntius.mensura,
                (constans character*)ante[k].nuntius.datum,
                (int)post.nuntius.mensura,
                (constans character*)post.nuntius.datum);
        }
        CREDO_VERUM (_status_pares(&ante[k], &post));
    }
}

interior vacuum
_sectio_dorsum_arithmetica (
    CredoContextus* c)
{
    _fixturam_conferre(c,
        "officina/probationes/fixa/machinula/arithmetica.medulla",
        (s64)XLII);
}

/* sistere: fixtura ($cade principalis nativa; $main circa sistere
 * salit) et sistere in vocata nativa sub vocante interpretato */
interior vacuum
_sectio_dorsum_sistere (
    CredoContextus* c)
{
    constans character* radix = getenv("RHUBARB_RADIX");
             character  via[CCLVI];
                chorda  textus;
             character* literae;
             Machinula* m;
          StatusCursus  ante;
          StatusCursus  post;
                chorda  causa;

    CREDO_NECESSE (_parare(c->piscina));
    sprintf(via, "%s/officina/probationes/fixa/machinula/"
        "sistere.medulla", (radix != NIHIL) ? radix : ".");
    textus = filum_legere_totum(via, c->piscina);
    CREDO_NECESSE (textus.mensura > ZEPHYRUM);
    literae = piscina_allocare(c->piscina,
        (memoriae_index)textus.mensura + I);
    CREDO_NECESSE_NON_NIHIL (literae);
    memcpy(literae, textus.datum, (memoriae_index)textus.mensura);
    literae[textus.mensura]  = '\0';
    m                        = _machinulam_parare(c->piscina, literae);
    CREDO_NECESSE_NON_NIHIL (m);
    ante = _statum_capere(c->piscina, m, "cade");
    CREDO_AEQUALIS_S32 (ante.genus, MACHINULA_SISTERE);
    CREDO_VERUM (chorda_aequalis_literis(ante.nuntius,
        "probatio halitus"));
    CREDO_VERUM (machinula_nativum_compilare(m, _ch("cade"), &causa));
    CREDO_VERUM (machinula_nativum_compilare(m, _ch("main"), &causa));
    post = _statum_capere(c->piscina, m, "cade");
    CREDO_VERUM (_status_pares(&ante, &post));
    post = _statum_capere(c->piscina, m, "main");
    CREDO_AEQUALIS_S32 (post.genus, MACHINULA_BENE);
    CREDO_VERUM (post.codex == (s64)XLII);

    _sectionem_purgare(c);
    m = _machinulam_parare(c->piscina,
        "modulus \"probatio/sistere.medulla\"\n"
        "\n"
        "functio $proba (s32 %n) -> s32\n"
        "@initium:\n"
        "    %a = addere.s32 %n, 1\n"
        "    %c = maior.s32 %a, 10\n"
        "    ramus %c -> @nimis | @bene\n"
        "@bene:\n"
        "    redde %a\n"
        "@nimis:\n"
        "    %b = addere.s32 %a, 1\n"
        "    sistere \"numerus nimis magnus\"\n"
        "\n"
        "functio $parvus () -> s32\n"
        "@initium:\n"
        "    %t = vocare.s32 $proba, s32 41\n"
        "    redde %t\n"
        "\n"
        "functio $modicus () -> s32\n"
        "@initium:\n"
        "    %t = vocare.s32 $proba, s32 4\n"
        "    %u = addere.s32 %t, 37\n"
        "    redde %u\n");
    CREDO_NECESSE_NON_NIHIL (m);
    ante = _statum_capere(c->piscina, m, "parvus");
    CREDO_AEQUALIS_S32 (ante.genus, MACHINULA_SISTERE);
    CREDO_AEQUALIS_I32 (ante.tabulata, II);
    CREDO_VERUM (machinula_nativum_compilare(m, _ch("proba"), &causa));
    post = _statum_capere(c->piscina, m, "parvus");
    CREDO_VERUM (_status_pares(&ante, &post));
    CREDO_VERUM (chorda_aequalis_literis(post.nuntius,
        "numerus nimis magnus"));
    /* post halitum machinula iterum utilis */
    post = _statum_capere(c->piscina, m, "modicus");
    CREDO_AEQUALIS_S32 (post.genus, MACHINULA_BENE);
    CREDO_VERUM (post.codex == (s64)XLII);
}


    /* ==================================================
     * DORSUM gradus IV: latitudo (extendere, amputare, movere
     * angustum), vocationes inter nativas, recusationes
     * ================================================== */

/* cursus nominatos interpretatos capit, functiones nominatas
 * compilat, deinde cursus iterum: status pares esse debent */
interior vacuum
_cursus_conferre (
         CredoContextus* c,
              Machinula* m,
    constans character** cursus,
                    s32  numerus_cursuum,
    constans character** nativae,
                    s32  numerus_nativarum)
{
    StatusCursus ante[XVI];
    StatusCursus post;
          chorda causa;
             s32 k;

    CREDO_NECESSE (numerus_cursuum <= XVI);
    per (k = ZEPHYRUM; k < numerus_cursuum; k++)
    {
        ante[k] = _statum_capere(c->piscina, m, cursus[k]);
    }
    per (k = ZEPHYRUM; k < numerus_nativarum; k++)
    {
        b32 compilata = machinula_nativum_compilare(m,
            _ch(nativae[k]), &causa);

        si (!compilata)
        {
            imprimere("RECUSATA %s: %.*s\n", nativae[k],
                (int)causa.mensura,
                (constans character*)causa.datum);
        }
        CREDO_VERUM (compilata);
    }
    per (k = ZEPHYRUM; k < numerus_cursuum; k++)
    {
        post = _statum_capere(c->piscina, m, cursus[k]);
        si (!_status_pares(&ante[k], &post))
        {
            imprimere("DISPAR %s: genus %d/%d codex %ld/%ld tabulata "
                "%d/%d summa %d:%d/%d:%d ima %d:%d/%d:%d\n",
                cursus[k], (int)ante[k].genus, (int)post.genus,
                (long)ante[k].codex, (long)post.codex,
                (int)ante[k].tabulata, (int)post.tabulata,
                (int)ante[k].functio_summa,
                (int)ante[k].instructio_summa,
                (int)post.functio_summa, (int)post.instructio_summa,
                (int)ante[k].functio_ima, (int)ante[k].instructio_ima,
                (int)post.functio_ima, (int)post.instructio_ima);
        }
        CREDO_VERUM (_status_pares(&ante[k], &post));
    }
}

/* signum descriptoris functionis nominatae */
interior s64
_signum_functionis (
    constans character* titulus)
{
    constans ConexioDescriptor* descriptor = conexio_sedes_quaerere(
        _conexio_sectionis, _ch(titulus));

    redde (descriptor != NIHIL) ? descriptor->signum : (s64)ZEPHYRUM;
}

interior vacuum
_sectio_dorsum_latitudo (
    CredoContextus* c)
{
    constans character* cursus[] = {
        "m_ext", "m_amp", "m_angustus", "m_extz", "m_octo", "m_octoz",
        "m_brevis", "m_amps", "m_mov", "m_movz", "m_catena"
    };
    constans character* nativae[] = {
        "ext", "amp", "angustus", "extz", "octo", "octoz", "brevis",
        "amps", "mov", "movz", "catena"
    };
       Machinula* m;
    StatusCursus  s;

    CREDO_NECESSE (_parare(c->piscina));
    m = _machinulam_parare(c->piscina,
        "modulus \"probatio/latitudo.medulla\"\n"
        "\n"
        "functio $ext (s32 %a) -> s64\n"
        "@initium:\n"
        "    %x = extendere.s32.s64 %a\n"
        "    redde %x\n"
        "\n"
        "functio $amp (i64 %a) -> i32\n"
        "@initium:\n"
        "    %x = amputare.i64.i32 %a\n"
        "    redde %x\n"
        "\n"
        "functio $angustus () -> s8\n"
        "@initium:\n"
        "    redde 200\n"
        "\n"
        "functio $extz (i32 %a) -> i64\n"
        "@initium:\n"
        "    %x = extendere.i32.i64 %a\n"
        "    redde %x\n"
        "\n"
        "functio $octo (s32 %a) -> s64\n"
        "@initium:\n"
        "    %n = amputare.s32.s8 %a\n"
        "    %x = extendere.s8.s64 %n\n"
        "    redde %x\n"
        "\n"
        "functio $octoz (s32 %a) -> s32\n"
        "@initium:\n"
        "    %n = amputare.s32.i8 %a\n"
        "    %x = extendere.i8.s32 %n\n"
        "    %y = addere.s32 %x, 1\n"
        "    redde %y\n"
        "\n"
        "functio $brevis (s64 %a) -> s64\n"
        "@initium:\n"
        "    %n = amputare.s64.s16 %a\n"
        "    %x = extendere.s16.s64 %n\n"
        "    redde %x\n"
        "\n"
        "functio $amps (s64 %a) -> s32\n"
        "@initium:\n"
        "    %n = amputare.s64.s32 %a\n"
        "    %y = dextrorsum.s32 %n, 4\n"
        "    redde %y\n"
        "\n"
        "functio $mov (s32 %a) -> s32\n"
        "@initium:\n"
        "    %n = movere.s8 %a\n"
        "    %k = movere.s8 300\n"
        "    %y = addere.s32 %n, %k\n"
        "    redde %y\n"
        "\n"
        "functio $movz (s32 %a) -> i32\n"
        "@initium:\n"
        "    %n = movere.i16 %a\n"
        "    %k = movere.s64 -3\n"
        "    %l = amputare.s64.i32 %k\n"
        "    %y = addere.i32 %n, %l\n"
        "    redde %y\n"
        "\n"
        "functio $catena (s64 %x, s32 %n) -> s64\n"
        "@initium:\n"
        "    %y = sinistrorsum.s64 %x, %n\n"
        "    redde %y\n"
        "\n"
        "functio $m_ext () -> s64\n"
        "@initium:\n"
        "    %t = vocare.s64 $ext, s32 -1\n"
        "    redde %t\n"
        "functio $m_amp () -> i32\n"
        "@initium:\n"
        "    %t = vocare.i32 $amp, i64 4294967301\n"
        "    redde %t\n"
        "functio $m_angustus () -> s32\n"
        "@initium:\n"
        "    %t = vocare.s8 $angustus\n"
        "    redde %t\n"
        "functio $m_extz () -> i64\n"
        "@initium:\n"
        "    %t = vocare.i64 $extz, i32 4000000000\n"
        "    redde %t\n"
        "functio $m_octo () -> s64\n"
        "@initium:\n"
        "    %t = vocare.s64 $octo, s32 200\n"
        "    redde %t\n"
        "functio $m_octoz () -> s32\n"
        "@initium:\n"
        "    %t = vocare.s32 $octoz, s32 -1\n"
        "    redde %t\n"
        "functio $m_brevis () -> s64\n"
        "@initium:\n"
        "    %t = vocare.s64 $brevis, s64 40000\n"
        "    redde %t\n"
        "functio $m_amps () -> s32\n"
        "@initium:\n"
        "    %t = vocare.s32 $amps, s64 6442450944\n"
        "    redde %t\n"
        "functio $m_mov () -> s32\n"
        "@initium:\n"
        "    %t = vocare.s32 $mov, s32 511\n"
        "    redde %t\n"
        "functio $m_movz () -> i32\n"
        "@initium:\n"
        "    %t = vocare.i32 $movz, s32 -2\n"
        "    redde %t\n"
        "functio $m_catena () -> s64\n"
        "@initium:\n"
        "    %t = vocare.s64 $catena, s64 3, s32 40\n"
        "    redde %t\n");
    CREDO_NECESSE_NON_NIHIL (m);
    /* lex (interpretata): valores plani */
    s = _statum_capere(c->piscina, m, "m_ext");
    CREDO_VERUM (s.codex == (s64)-I);
    s = _statum_capere(c->piscina, m, "m_amp");
    CREDO_VERUM (s.codex == (s64)V);
    s = _statum_capere(c->piscina, m, "m_angustus");
    CREDO_VERUM (s.codex == (s64)-LVI);
    _cursus_conferre(c, m, cursus, XI, nativae, XI);
}

#define TEXTUS_VOCATIONUM \
    "modulus \"probatio/vocationes.medulla\"\n" \
    "\n" \
    "functio $g (s32 %x, s32 %y) -> s32\n" \
    "@initium:\n" \
    "    %t = multiplicare.s32 %x, %y\n" \
    "    redde %t\n" \
    "\n" \
    "functio $f (s32 %a) -> s32\n" \
    "@initium:\n" \
    "    %t = vocare.s32 $g, s32 %a, s32 3\n" \
    "    %u = addere.s32 %t, %a\n" \
    "    redde %u\n" \
    "\n" \
    "functio $fact (s32 %n) -> s32\n" \
    "@initium:\n" \
    "    %c = minor.s32 %n, 2\n" \
    "    ramus %c -> @basis | @gradus\n" \
    "@basis:\n" \
    "    redde 1\n" \
    "@gradus:\n" \
    "    %m = subtrahere.s32 %n, 1\n" \
    "    %r = vocare.s32 $fact, s32 %m\n" \
    "    %p = multiplicare.s32 %r, %n\n" \
    "    redde %p\n" \
    "\n" \
    "functio $octo (s32 %a, s32 %b, s32 %c, s32 %d, s32 %e, s32 %f, " \
    "s32 %g, s32 %h) -> s32\n" \
    "@initium:\n" \
    "    %s = subtrahere.s32 %a, %b\n" \
    "    %s = addere.s32 %s, %c\n" \
    "    %s = subtrahere.s32 %s, %d\n" \
    "    %s = addere.s32 %s, %e\n" \
    "    %s = subtrahere.s32 %s, %f\n" \
    "    %s = addere.s32 %s, %g\n" \
    "    %s = subtrahere.s32 %s, %h\n" \
    "    redde %s\n" \
    "\n" \
    "functio $octies (s32 %a) -> s32\n" \
    "@initium:\n" \
    "    %b = addere.s32 %a, 1\n" \
    "    %t = vocare.s32 $octo, s32 100, s32 %a, s32 %b, s32 4, " \
    "s32 %a, s32 6, s32 %b, s32 8\n" \
    "    %u = addere.s32 %t, %a\n" \
    "    %v = addere.s32 %u, %b\n" \
    "    redde %v\n" \
    "\n" \
    "functio $vacua (s32 %a) -> vacuum\n" \
    "@initium:\n" \
    "    redde\n" \
    "\n" \
    "functio $sine_fructu (s32 %a) -> s32\n" \
    "@initium:\n" \
    "    vocare $vacua, s32 %a\n" \
    "    %b = addere.s32 %a, 2\n" \
    "    redde %b\n" \
    "\n" \
    "functio $descende (s32 %n) -> s32\n" \
    "@initium:\n" \
    "    %c = aequalis.s32 %n, 0\n" \
    "    ramus %c -> @fundus | @gradus\n" \
    "@fundus:\n" \
    "    sistere \"fundus tactus\"\n" \
    "@gradus:\n" \
    "    %m = subtrahere.s32 %n, 1\n" \
    "    %r = vocare.s32 $descende, s32 %m\n" \
    "    redde %r\n" \
    "\n" \
    "functio $divisor (s32 %a, s32 %b) -> s32\n" \
    "@initium:\n" \
    "    %q = dividere.s32 %a, %b\n" \
    "    redde %q\n" \
    "\n" \
    "functio $medius (s32 %a) -> s32\n" \
    "@initium:\n" \
    "    %z = subtrahere.s32 %a, %a\n" \
    "    %q = vocare.s32 $divisor, s32 %a, s32 %z\n" \
    "    redde %q\n" \
    "\n" \
    "functio $iterata (s32 %a) -> s32\n" \
    "@initium:\n" \
    "    %t = movere.s32 5\n" \
    "    %t = vocare.s32 $g, s32 %a, s32 %t\n" \
    "    %u = addere.s32 %t, 2\n" \
    "    redde %u\n" \
    "\n" \
    "functio $m_iterata () -> s32\n" \
    "@initium:\n" \
    "    %t = vocare.s32 $iterata, s32 8\n" \
    "    redde %t\n" \
    "functio $m_f () -> s32\n" \
    "@initium:\n" \
    "    %t = vocare.s32 $f, s32 10\n" \
    "    %u = addere.s32 %t, 2\n" \
    "    redde %u\n" \
    "functio $m_fact () -> s32\n" \
    "@initium:\n" \
    "    %t = vocare.s32 $fact, s32 5\n" \
    "    redde %t\n" \
    "functio $m_octies () -> s32\n" \
    "@initium:\n" \
    "    %t = vocare.s32 $octies, s32 7\n" \
    "    redde %t\n" \
    "functio $m_sine () -> s32\n" \
    "@initium:\n" \
    "    %t = vocare.s32 $sine_fructu, s32 40\n" \
    "    redde %t\n" \
    "functio $m_descende () -> s32\n" \
    "@initium:\n" \
    "    %t = vocare.s32 $descende, s32 5\n" \
    "    redde %t\n" \
    "functio $m_medius () -> s32\n" \
    "@initium:\n" \
    "    %t = vocare.s32 $medius, s32 9\n" \
    "    redde %t\n"

interior vacuum
_sectio_dorsum_vocationes (
    CredoContextus* c)
{
    constans character* cursus[] = {
        "m_f", "m_fact", "m_octies", "m_sine", "m_descende",
        "m_medius", "m_iterata"
    };
    constans character* nativae[] = {
        "f", "fact", "octies", "sine_fructu", "descende", "medius",
        "iterata"
    };
       Machinula* m;
    StatusCursus  s;

    CREDO_NECESSE (_parare(c->piscina));
    m = _machinulam_parare(c->piscina, TEXTUS_VOCATIONUM);
    CREDO_NECESSE_NON_NIHIL (m);
    s = _statum_capere(c->piscina, m, "m_f");
    CREDO_VERUM (s.codex == (s64)XLII);
    s = _statum_capere(c->piscina, m, "m_fact");
    CREDO_VERUM (s.codex == (s64)CXX);
    s = _statum_capere(c->piscina, m, "m_sine");
    CREDO_VERUM (s.codex == (s64)XLII);
    /* halitus sub VI vocationibus nidatis et sub vocata vocatae */
    s = _statum_capere(c->piscina, m, "m_descende");
    CREDO_AEQUALIS_S32 (s.genus, MACHINULA_SISTERE);
    CREDO_AEQUALIS_I32 (s.tabulata, VII);
    s = _statum_capere(c->piscina, m, "m_medius");
    CREDO_AEQUALIS_S32 (s.genus, MACHINULA_VITIUM);
    CREDO_AEQUALIS_I32 (s.tabulata, III);
    CREDO_VERUM (_signum_functionis("g")
        == (s64)CONEXIO_SIGNUM_INTERPRETATUM);
    /* fructus vocationis in registrum iam vivum (servatum): 8 * 5 */
    s = _statum_capere(c->piscina, m, "m_iterata");
    CREDO_VERUM (s.codex == (s64)XLII);
    _cursus_conferre(c, m, cursus, VII, nativae, VII);
    /* clausura: vocatae quoque nativae factae sunt */
    CREDO_VERUM (_signum_functionis("f")
        == (s64)CONEXIO_SIGNUM_NATIVUM);
    CREDO_VERUM (_signum_functionis("g")
        == (s64)CONEXIO_SIGNUM_NATIVUM);
    CREDO_VERUM (_signum_functionis("octo")
        == (s64)CONEXIO_SIGNUM_NATIVUM);
    CREDO_VERUM (_signum_functionis("divisor")
        == (s64)CONEXIO_SIGNUM_NATIVUM);
    CREDO_VERUM (_signum_functionis("m_f")
        == (s64)CONEXIO_SIGNUM_INTERPRETATUM);
}

/* recusationes nominatae; functio recusata (et vocatae eius)
 * interpretata manet et currit ut ante */
interior vacuum
_sectio_dorsum_recusationes (
    CredoContextus* c)
{
       Machinula* m;
    StatusCursus  s;
          chorda  causa;

    CREDO_NECESSE (_parare(c->piscina));
    m = _machinulam_parare(c->piscina,
        "modulus \"probatio/recusationes.medulla\"\n"
        "\n"
        "functio $g (s32 %a) -> s32\n"
        "@initium:\n"
        "    %p = addere.s16 %a, 1\n"
        "    %b = addere.s32 %a, 1\n"
        "    redde %b\n"
        "\n"
        "functio $h (s32 %a) -> s32\n"
        "@initium:\n"
        "    %b = addere.s32 %a, 1\n"
        "    redde %b\n"
        "\n"
        "functio $f (s32 %a) -> s32\n"
        "@initium:\n"
        "    %t = vocare.s32 $h, s32 %a\n"
        "    %u = vocare.s32 $g, s32 %t\n"
        "    redde %u\n"
        "\n"
        "functio $latum (s32 %a) -> s64\n"
        "@initium:\n"
        "    %b = addere.s32 %a, 1\n"
        "    %c = addere.s64 %b, 1\n"
        "    redde %c\n"
        "\n"
        "functio $reditus_latus (s32 %a) -> s64\n"
        "@initium:\n"
        "    %b = addere.s32 %a, 1\n"
        "    redde %b\n"
        "\n"
        "functio $ping (s32 %a) -> s32\n"
        "@initium:\n"
        "    %c = minor.s32 %a, 1\n"
        "    ramus %c -> @finis | @gradus\n"
        "@finis:\n"
        "    redde 42\n"
        "@gradus:\n"
        "    %b = subtrahere.s32 %a, 1\n"
        "    %t = vocare.s32 $pong, s32 %b\n"
        "    redde %t\n"
        "\n"
        "functio $pong (s32 %a) -> s32\n"
        "@initium:\n"
        "    %t = vocare.s32 $ping, s32 %a\n"
        "    redde %t\n"
        "\n"
        "functio $per_monstratorem (s32 %a) -> s32\n"
        "@initium:\n"
        "    %l = locus $h\n"
        "    %t = vocare.s32 %l, s32 %a\n"
        "    redde %t\n"
        "\n"
        "functio $main () -> s32\n"
        "@initium:\n"
        "    %t = vocare.s32 $f, s32 40\n"
        "    redde %t\n"
        "\n"
        "functio $m_ping () -> s32\n"
        "@initium:\n"
        "    %t = vocare.s32 $ping, s32 6\n"
        "    redde %t\n");
    CREDO_NECESSE_NON_NIHIL (m);
    /* vocata quam dorsum nondum emittit */
    CREDO_FALSUM (machinula_nativum_compilare(m, _ch("f"), &causa));
    CREDO_VERUM (chorda_aequalis_literis(causa,
        "vocat $g interpretatum"));
    CREDO_VERUM (_signum_functionis("f")
        == (s64)CONEXIO_SIGNUM_INTERPRETATUM);
    CREDO_VERUM (_signum_functionis("g")
        == (s64)CONEXIO_SIGNUM_INTERPRETATUM);
    s = _statum_capere(c->piscina, m, "main");
    CREDO_AEQUALIS_S32 (s.genus, MACHINULA_BENE);
    CREDO_VERUM (s.codex == (s64)XLII);
    /* registrum signo minore scriptum in usu LXIV (census E17) */
    CREDO_FALSUM (machinula_nativum_compilare(m, _ch("latum"),
        &causa));
    CREDO_VERUM (chorda_aequalis_literis(causa,
        "registrum s32 in usu 64"));
    CREDO_FALSUM (machinula_nativum_compilare(m, _ch("reditus_latus"),
        &causa));
    CREDO_VERUM (chorda_aequalis_literis(causa,
        "registrum s32 in usu 64"));
    /* recursio mutua: vocata nondum locata */
    CREDO_FALSUM (machinula_nativum_compilare(m, _ch("ping"), &causa));
    CREDO_VERUM (chorda_aequalis_literis(causa,
        "vocat $pong interpretatum"));
    s = _statum_capere(c->piscina, m, "m_ping");
    CREDO_AEQUALIS_S32 (s.genus, MACHINULA_BENE);
    CREDO_VERUM (s.codex == (s64)XLII);
    /* vocatio per monstratorem nunc compilatur (T8 gradus III) */
    CREDO_VERUM (machinula_nativum_compilare(m,
        _ch("per_monstratorem"), &causa));
    /* ignota */
    CREDO_FALSUM (machinula_nativum_compilare(m, _ch("nusquam"),
        &causa));
    CREDO_VERUM (chorda_aequalis_literis(causa, "functio ignota"));
}


    /* ==================================================
     * DORSUM T8 gradus I: memoria (arca, legere, scribere, copia,
     * locus)
     * ================================================== */

interior vacuum
_sectio_dorsum_fixturae_memoriae (
    CredoContextus* c)
{
    _fixturam_conferre(c,
        "officina/probationes/fixa/machinula/memoria.medulla",
        (s64)XLII);
    _sectionem_purgare(c);
    _fixturam_conferre(c,
        "officina/probationes/fixa/machinula/data.medulla",
        (s64)XLII);
}

#define TEXTUS_MEMORIAE \
    "modulus \"probatio/memoria.medulla\"\n" \
    "\n" \
    "functio $latitudines () -> s64\n" \
    "@initium:\n" \
    "    %p = arca 32, 8\n" \
    "    %m = movere.s64 -2\n" \
    "    scribere.s64 %p, %m\n" \
    "    %a = legere.s8 %p\n" \
    "    %b = legere.i8 %p\n" \
    "    %c = legere.s16 %p\n" \
    "    %d = legere.i16 %p\n" \
    "    %e = legere.s32 %p\n" \
    "    %f = legere.i32 %p\n" \
    "    %g = legere.s64 %p\n" \
    "    %q = addere.i64 %p, 8\n" \
    "    scribere.i64 %q, 0\n" \
    "    scribere.s8 %q, 300\n" \
    "    %r = addere.i64 %p, 16\n" \
    "    scribere.i64 %r, 0\n" \
    "    scribere.i16 %r, 70000\n" \
    "    %h = legere.s64 %q\n" \
    "    %i = legere.i64 %r\n" \
    "    %s = addere.s64 %a, %b\n" \
    "    %s = addere.s64 %s, %c\n" \
    "    %s = addere.s64 %s, %d\n" \
    "    %s = addere.s64 %s, %e\n" \
    "    %s = addere.s64 %s, %f\n" \
    "    %s = addere.s64 %s, %g\n" \
    "    %s = addere.s64 %s, %h\n" \
    "    %s = addere.s64 %s, %i\n" \
    "    redde %s\n" \
    "\n" \
    "functio $ansa_arcae () -> s32\n" \
    "@initium:\n" \
    "    %i = movere.s32 0\n" \
    "    %s = movere.s32 0\n" \
    "    %vetus = movere.i64 0\n" \
    "    %idem = movere.s32 0\n" \
    "    salire @proba\n" \
    "@proba:\n" \
    "    %c = minor.s32 %i, 5\n" \
    "    ramus %c -> @corpus | @finis\n" \
    "@corpus:\n" \
    "    %p = arca 8, 8\n" \
    "    %e = aequalis.i64 %p, %vetus\n" \
    "    %idem = addere.s32 %idem, %e\n" \
    "    %vetus = movere.i64 %p\n" \
    "    scribere.s32 %p, %i\n" \
    "    %v = legere.s32 %p\n" \
    "    %s = addere.s32 %s, %v\n" \
    "    %i = addere.s32 %i, 1\n" \
    "    salire @proba\n" \
    "@finis:\n" \
    "    %t = multiplicare.s32 %idem, 8\n" \
    "    %u = addere.s32 %s, %t\n" \
    "    redde %u\n" \
    "\n" \
    "functio $arca_magna () -> s32\n" \
    "@initium:\n" \
    "    %p = arca 20000, 8\n" \
    "    %q = arca 16, 8\n" \
    "    %e = addere.i64 %p, 19996\n" \
    "    scribere.s32 %e, 40\n" \
    "    scribere.s32 %q, 2\n" \
    "    scribere.s32 %p, 7\n" \
    "    %a = legere.s32 %e\n" \
    "    %b = legere.s32 %q\n" \
    "    %s = addere.s32 %a, %b\n" \
    "    redde %s\n" \
    "\n" \
    "functio $copiae () -> s64\n" \
    "@initium:\n" \
    "    %p = arca 32, 8\n" \
    "    %v = movere.i64 578437695752307201\n" \
    "    scribere.i64 %p, %v\n" \
    "    %w = movere.i64 1157159078456920585\n" \
    "    %p8 = addere.i64 %p, 8\n" \
    "    scribere.i64 %p8, %w\n" \
    "    %p3 = addere.i64 %p, 3\n" \
    "    copia %p3, %p, 8\n" \
    "    %a = legere.i64 %p3\n" \
    "    %p1 = addere.i64 %p, 1\n" \
    "    copia %p, %p1, 12\n" \
    "    %b = legere.i64 %p\n" \
    "    %b2 = legere.i64 %p8\n" \
    "    %n = movere.i64 0\n" \
    "    copia %p8, %p, %n\n" \
    "    %b3 = legere.i64 %p8\n" \
    "    %s = aut.i64 %a, %b\n" \
    "    %s = aut.i64 %s, %b2\n" \
    "    %s = aut.i64 %s, %b3\n" \
    "    redde %s\n" \
    "\n" \
    "functio $scribe (i64 %p, s32 %v) -> s32\n" \
    "@initium:\n" \
    "    %vetus = legere.s32 %p\n" \
    "    scribere.s32 %p, %v\n" \
    "    redde %vetus\n" \
    "\n" \
    "functio $per_limen () -> s32\n" \
    "@initium:\n" \
    "    %p = arca 8, 8\n" \
    "    scribere.s32 %p, 30\n" \
    "    %vetus = vocare.s32 $scribe, i64 %p, s32 12\n" \
    "    %novum = legere.s32 %p\n" \
    "    %s = addere.s32 %vetus, %novum\n" \
    "    redde %s\n" \
    "\n" \
    "functio $numera () -> s32\n" \
    "@initium:\n" \
    "    %p = locus $numerator\n" \
    "    %v = legere.s32 %p\n" \
    "    %w = addere.s32 %v, 1\n" \
    "    scribere.s32 %p, %w\n" \
    "    redde %w\n" \
    "\n" \
    "functio $bis_numera () -> s32\n" \
    "@initium:\n" \
    "    %a = vocare.s32 $numera\n" \
    "    %b = vocare.s32 $numera\n" \
    "    %d = subtrahere.s32 %b, %a\n" \
    "    %s = addere.s32 %d, 41\n" \
    "    redde %s\n" \
    "\n" \
    "functio $nullus (i64 %p) -> s32\n" \
    "@initium:\n" \
    "    %v = legere.s32 %p\n" \
    "    redde %v\n" \
    "\n" \
    "functio $per_nullum () -> s32\n" \
    "@initium:\n" \
    "    %v = vocare.s32 $nullus, i64 0\n" \
    "    redde %v\n" \
    "\n" \
    "functio $angusta_inscriptio (s32 %a) -> s32\n" \
    "@initium:\n" \
    "    %i = addere.s32 %a, 1\n" \
    "    %v = legere.s32 %i\n" \
    "    redde %v\n" \
    "\n" \
    "datum $numerator magnitudo 4 ordinatio 4\n" \
    "    octeti 00000000\n"

interior vacuum
_sectio_dorsum_memoria (
    CredoContextus* c)
{
    constans character* cursus[] = {
        "latitudines", "ansa_arcae", "arca_magna", "copiae",
        "per_limen"
    };
    constans character* nativae[] = {
        "latitudines", "ansa_arcae", "arca_magna", "copiae", "scribe"
    };
       Machinula* m;
    StatusCursus  s;
    StatusCursus  t;
          chorda  causa;

    CREDO_NECESSE (_parare(c->piscina));
    m = _machinulam_parare(c->piscina, TEXTUS_MEMORIAE);
    CREDO_NECESSE_NON_NIHIL (m);
    /* lex (interpretata) */
    s = _statum_capere(c->piscina, m, "ansa_arcae");
    CREDO_AEQUALIS_S32 (s.genus, MACHINULA_BENE);
    CREDO_VERUM (s.codex == (s64)XLII);   /* 0+1+2+3+4 + 4*8 */
    s = _statum_capere(c->piscina, m, "arca_magna");
    CREDO_VERUM (s.codex == (s64)XLII);
    s = _statum_capere(c->piscina, m, "per_limen");
    CREDO_VERUM (s.codex == (s64)XLII);
    _cursus_conferre(c, m, cursus, V, nativae, V);

    /* datum globale: scripturae nativae manent (regio machinulae) */
    s = _statum_capere(c->piscina, m, "bis_numera");
    CREDO_VERUM (s.codex == (s64)XLII);
    CREDO_VERUM (machinula_nativum_compilare(m, _ch("bis_numera"),
        &causa));
    t = _statum_capere(c->piscina, m, "bis_numera");
    CREDO_AEQUALIS_S32 (t.genus, MACHINULA_BENE);
    CREDO_VERUM (t.codex == (s64)XLII);
    t = _statum_capere(c->piscina, m, "numera");
    CREDO_VERUM (t.codex == (s64)V);

    /* monstrator nullus: interpretata nuntio suo sistit; nativa per
     * tractatorem signorum - nuntii differunt (machinula severior),
     * tabulata et positiones pares */
    s = _statum_capere(c->piscina, m, "per_nullum");
    CREDO_AEQUALIS_S32 (s.genus, MACHINULA_VITIUM);
    CREDO_CHORDA_CONTINET (s.nuntius, _ch("monstrator nullus"));
    CREDO_VERUM (machinula_nativum_compilare(m, _ch("nullus"), &causa));
    t = _statum_capere(c->piscina, m, "per_nullum");
    CREDO_AEQUALIS_S32 (t.genus, MACHINULA_VITIUM);
    CREDO_CHORDA_CONTINET (t.nuntius, _ch("nativum: signum"));
    CREDO_AEQUALIS_I32 (t.tabulata, s.tabulata);
    CREDO_VERUM (t.vestigium == s.vestigium);
    /* machinula iterum utilis */
    t = _statum_capere(c->piscina, m, "arca_magna");
    CREDO_VERUM (t.codex == (s64)XLII);

    /* inscriptio in registro signo minore scripto: census E17 */
    CREDO_FALSUM (machinula_nativum_compilare(m,
        _ch("angusta_inscriptio"), &causa));
    CREDO_VERUM (chorda_aequalis_literis(causa,
        "registrum s32 in usu 64"));
}


    /* ==================================================
     * DORSUM T8 gradus II: aedificata - pura directe (inscriptio
     * libc), cum statu per pontem (machinula_pons_nativus)
     * ================================================== */

#define TEXTUS_AEDIFICATORUM \
    "modulus \"probatio/aedificata.medulla\"\n" \
    "externum $strlen (i64) -> i64\n" \
    "externum $memcpy (i64, i64, i64) -> i64\n" \
    "externum $memcmp (i64, i64, i64) -> s32\n" \
    "externum $toupper (s32) -> s32\n" \
    "externum $malloc (i64) -> i64\n" \
    "externum $free (i64) -> vacuum\n" \
    "externum $printf (i64, ...) -> s32\n" \
    "externum $exit (s32) -> vacuum\n" \
    "externum $ignota (s32) -> s32\n" \
    "\n" \
    "functio $pura () -> s32\n" \
    "@initium:\n" \
    "    %p = locus $verba\n" \
    "    %q = locus $alia\n" \
    "    %n = vocare.i64 $strlen, i64 %p\n" \
    "    %b = arca 16, 8\n" \
    "    %r = vocare.i64 $memcpy, i64 %b, i64 %p, i64 6\n" \
    "    %c = vocare.s32 $memcmp, i64 %b, i64 %p, i64 6\n" \
    "    %m = vocare.i64 $strlen, i64 %r\n" \
    "    %u = vocare.s32 $toupper, s32 97\n" \
    "    %d = vocare.s32 $memcmp, i64 %p, i64 %q, i64 1\n" \
    "    %lt = minor.s32 %d, 0\n" \
    "    %n32 = amputare.i64.s32 %n\n" \
    "    %m32 = amputare.i64.s32 %m\n" \
    "    %s = addere.s32 %n32, %m32\n" \
    "    %s = addere.s32 %s, %c\n" \
    "    %s = addere.s32 %s, %u\n" \
    "    %s = addere.s32 %s, %lt\n" \
    "    %s = subtrahere.s32 %s, 34\n" \
    "    redde %s\n" \
    "\n" \
    "functio $acervus () -> i64\n" \
    "@initium:\n" \
    "    %p = vocare.i64 $malloc, i64 64\n" \
    "    scribere.s32 %p, 42\n" \
    "    redde %p\n" \
    "\n" \
    "functio $acervus_usus () -> s32\n" \
    "@initium:\n" \
    "    %p = vocare.i64 $malloc, i64 64\n" \
    "    scribere.s32 %p, 40\n" \
    "    %v = legere.s32 %p\n" \
    "    vocare $free, i64 %p\n" \
    "    %w = addere.s32 %v, 2\n" \
    "    redde %w\n" \
    "\n" \
    "functio $scriptor (s32 %a) -> s32\n" \
    "@initium:\n" \
    "    %f = locus $forma\n" \
    "    %s = locus $verba\n" \
    "    %neg = subtrahere.s32 %a, 10\n" \
    "    %n = vocare.s32 $printf, i64 %f, s32 %a, f64 2.5, i64 %s, " \
    "s32 %neg\n" \
    "    redde %n\n" \
    "\n" \
    "functio $m_scriptor () -> s32\n" \
    "@initium:\n" \
    "    %t = vocare.s32 $scriptor, s32 7\n" \
    "    redde %t\n" \
    "\n" \
    "functio $liberator (i64 %p) -> s32\n" \
    "@initium:\n" \
    "    %a = addere.i64 %p, 1\n" \
    "    vocare $free, i64 %a\n" \
    "    redde 1\n" \
    "\n" \
    "functio $m_liberator () -> s32\n" \
    "@initium:\n" \
    "    %t = vocare.s32 $liberator, i64 12344\n" \
    "    redde %t\n" \
    "\n" \
    "functio $exitus (s32 %a) -> s32\n" \
    "@initium:\n" \
    "    vocare $exit, s32 %a\n" \
    "    redde 0\n" \
    "\n" \
    "functio $m_exitus () -> s32\n" \
    "@initium:\n" \
    "    %t = vocare.s32 $exitus, s32 7\n" \
    "    redde %t\n" \
    "\n" \
    "functio $ignotam () -> s32\n" \
    "@initium:\n" \
    "    %t = vocare.s32 $ignota, s32 1\n" \
    "    redde %t\n" \
    "\n" \
    "datum $verba magnitudo 8 ordinatio 1\n" \
    "    octeti 73616c76 65000000\n" \
    "\n" \
    "datum $alia magnitudo 4 ordinatio 1\n" \
    "    octeti 74000000\n" \
    "\n" \
    "datum $forma magnitudo 16 ordinatio 1\n" \
    "    octeti 25642025 2e326620 25732025 640a0000\n"

/* $m_scriptor currit cum flumine 1 in plagulam capto; reddit textum
 * captum */
interior chorda
_scripturam_capere (
        CredoContextus* c,
             Machinula* m,
    constans character* titulus_plagulae,
          StatusCursus* status_out)
{
    constans character* area = getenv("TMPDIR");
             character  via[DXII];
                  FILE* plagula;

    sprintf(via, "%s/%s", (area != NIHIL) ? area : "/tmp",
        titulus_plagulae);
    plagula = lectiones_fopen(via, "w");
    si (plagula == NIHIL)
    {
        chorda vacua;

        vacua.datum    = NIHIL;
        vacua.mensura  = ZEPHYRUM;
        redde vacua;
    }
    machinula_ansam_ponere(m, I, plagula);
    *status_out = _statum_capere(c->piscina, m, "m_scriptor");
    machinula_ansam_ponere(m, I, NIHIL);
    fclose(plagula);
    redde filum_legere_totum(via, c->piscina);
}

interior vacuum
_sectio_dorsum_aedificata (
    CredoContextus* c)
{
    constans character* cursus[] = {
        "pura", "acervus_usus", "m_liberator", "m_exitus"
    };
    constans character* nativae[] = {
        "pura", "acervus", "acervus_usus", "scriptor", "liberator",
        "exitus"
    };
       Machinula* m;
    StatusCursus  s;
    StatusCursus  t;
          chorda  captum_ante;
          chorda  captum_post;
          chorda  causa;

    CREDO_NECESSE (_parare(c->piscina));
    m = _machinulam_parare(c->piscina, TEXTUS_AEDIFICATORUM);
    CREDO_NECESSE_NON_NIHIL (m);
    /* lex (interpretata) */
    s = _statum_capere(c->piscina, m, "pura");
    CREDO_AEQUALIS_S32 (s.genus, MACHINULA_BENE);
    CREDO_VERUM (s.codex == (s64)XLII);
    s = _statum_capere(c->piscina, m, "acervus_usus");
    CREDO_VERUM (s.codex == (s64)XLII);
    s = _statum_capere(c->piscina, m, "m_liberator");
    CREDO_AEQUALIS_S32 (s.genus, MACHINULA_VITIUM);
    CREDO_CHORDA_CONTINET (s.nuntius, _ch("free: monstrator insanus"));
    s = _statum_capere(c->piscina, m, "m_exitus");
    CREDO_AEQUALIS_S32 (s.genus, MACHINULA_BENE);
    CREDO_VERUM (s.codex == (s64)VII);
    s = _statum_capere(c->piscina, m, "acervus");
    CREDO_VERUM (regio_continet(_regio_sectionis,
        (constans vacuum*)(memoriae_index)s.codex));
    captum_ante = _scripturam_capere(c, m, "scriptura_ante.txt", &s);
    CREDO_VERUM (chorda_aequalis_literis(captum_ante,
        "7 2.50 salve -3\n"));
    CREDO_VERUM (s.codex == (s64)XVI);

    _cursus_conferre(c, m, cursus, IV, nativae, VI);
    CREDO_VERUM (_signum_functionis("scriptor")
        == (s64)CONEXIO_SIGNUM_NATIVUM);
    /* malloc per pontem: inscriptio in acervo regionis machinulae
     * (malloc hospitis directus extra regionem iaceret) */
    t = _statum_capere(c->piscina, m, "acervus");
    CREDO_AEQUALIS_S32 (t.genus, MACHINULA_BENE);
    CREDO_VERUM (regio_continet(_regio_sectionis,
        (constans vacuum*)(memoriae_index)t.codex));
    CREDO_AEQUALIS_S32 (*(constans s32*)(memoriae_index)t.codex, XLII);
    /* printf variadica per pontem: captura eadem */
    captum_post = _scripturam_capere(c, m, "scriptura_post.txt", &t);
    CREDO_VERUM (chorda_aequalis(captum_ante, captum_post));
    CREDO_VERUM (t.codex == (s64)XVI);
    /* decipula quae aedificatum non est: recusatur */
    CREDO_FALSUM (machinula_nativum_compilare(m, _ch("ignotam"),
        &causa));
    CREDO_VERUM (chorda_aequalis_literis(causa,
        "vocat $ignota interpretatum"));

    /* superficies pontis publica */
    CREDO_VERUM (machinula_aedificatum_quaerere(_ch("strlen"))
        >= ZEPHYRUM);
    CREDO_AEQUALIS_S32 (machinula_aedificatum_quaerere(
        _ch("nusquam")), -I);
    CREDO_VERUM (machinula_aedificatum_purum(
        machinula_aedificatum_quaerere(_ch("strlen"))));
    CREDO_FALSUM (machinula_aedificatum_purum(
        machinula_aedificatum_quaerere(_ch("malloc"))));
    CREDO_FALSUM (machinula_aedificatum_purum(
        machinula_aedificatum_quaerere(_ch("printf"))));
    CREDO_NON_NIHIL (machinula_aedificatum_inscriptio(
        machinula_aedificatum_quaerere(_ch("strlen"))));
    CREDO_NIHIL (machinula_aedificatum_inscriptio(
        machinula_aedificatum_quaerere(_ch("malloc"))));
}


    /* ==================================================
     * DORSUM T8 gradus III: argumenta integra ultra octavum (in
     * stiva, magnitudine naturali - Apple AAPCS64) et vocationes
     * indirectae
     * ================================================== */

#define TEXTUS_MULTORUM \
    "modulus \"probatio/multa.medulla\"\n" \
    "\n" \
    "functio $multa (s32 %a1, s32 %a2, s32 %a3, s32 %a4, s32 %a5, " \
    "s32 %a6, s32 %a7, s32 %a8, s32 %i, s64 %l, s8 %c, i16 %w, " \
    "s32 %j, i8 %o, s64 %ll) -> s64\n" \
    "@initium:\n" \
    "    %e0 = extendere.s32.s64 %a1\n" \
    "    %e8 = extendere.s32.s64 %a8\n" \
    "    %e1 = extendere.s32.s64 %i\n" \
    "    %e3 = extendere.s8.s64 %c\n" \
    "    %e4 = extendere.i16.s64 %w\n" \
    "    %e5 = extendere.s32.s64 %j\n" \
    "    %e6 = extendere.i8.s64 %o\n" \
    "    %s = movere.s64 %e0\n" \
    "    %t = multiplicare.s64 %e8, 3\n" \
    "    %s = addere.s64 %s, %t\n" \
    "    %t = multiplicare.s64 %e1, 5\n" \
    "    %s = addere.s64 %s, %t\n" \
    "    %t = multiplicare.s64 %l, 7\n" \
    "    %s = addere.s64 %s, %t\n" \
    "    %t = multiplicare.s64 %e3, 11\n" \
    "    %s = addere.s64 %s, %t\n" \
    "    %t = multiplicare.s64 %e4, 13\n" \
    "    %s = addere.s64 %s, %t\n" \
    "    %t = multiplicare.s64 %e5, 17\n" \
    "    %s = addere.s64 %s, %t\n" \
    "    %t = multiplicare.s64 %e6, 19\n" \
    "    %s = addere.s64 %s, %t\n" \
    "    %t = multiplicare.s64 %ll, 23\n" \
    "    %s = addere.s64 %s, %t\n" \
    "    %c32 = addere.s32 %c, 1000\n" \
    "    %e7 = extendere.s32.s64 %c32\n" \
    "    %t = multiplicare.s64 %e7, 29\n" \
    "    %s = addere.s64 %s, %t\n" \
    "    redde %s\n" \
    "\n" \
    "functio $vocans (s32 %x, s64 %magnus) -> s64\n" \
    "@initium:\n" \
    "    %y = subtrahere.s32 %x, 10\n" \
    "    %c = amputare.s32.s8 %y\n" \
    "    %w = amputare.s32.i16 %y\n" \
    "    %o = amputare.s32.i8 %y\n" \
    "    %t = vocare.s64 $multa, s32 %x, s32 2, s32 3, s32 4, s32 5, " \
    "s32 6, s32 7, s32 %y, s32 %y, s64 %magnus, s8 %c, i16 %w, " \
    "s32 %x, i8 %o, s64 -5\n" \
    "    %u = addere.s64 %t, %magnus\n" \
    "    redde %u\n" \
    "\n" \
    "functio $effusa (s32 %x) -> s64\n" \
    "@initium:\n" \
    "    %r1 = addere.s32 %x, 1\n" \
    "    %r2 = addere.s32 %x, 2\n" \
    "    %r3 = addere.s32 %x, 3\n" \
    "    %r4 = addere.s32 %x, 4\n" \
    "    %r5 = addere.s32 %x, 5\n" \
    "    %r6 = addere.s32 %x, 6\n" \
    "    %r7 = addere.s32 %x, 7\n" \
    "    %r8 = addere.s32 %x, 8\n" \
    "    %r9 = addere.s32 %x, 9\n" \
    "    %r10 = addere.s32 %x, 10\n" \
    "    %r11 = addere.s32 %x, 11\n" \
    "    %r12 = addere.s32 %x, 12\n" \
    "    %r13 = addere.s32 %x, 13\n" \
    "    %r14 = addere.s32 %x, 14\n" \
    "    %t = vocare.s64 $multa, s32 %r1, s32 %r2, s32 %r3, s32 %r4, " \
    "s32 %r5, s32 %r6, s32 %r7, s32 %r8, s32 %r9, s64 77, s8 -1, " \
    "i16 3, s32 %r10, i8 5, s64 -6\n" \
    "    %s = addere.s32 %r1, %r2\n" \
    "    %s = addere.s32 %s, %r3\n" \
    "    %s = addere.s32 %s, %r4\n" \
    "    %s = addere.s32 %s, %r5\n" \
    "    %s = addere.s32 %s, %r6\n" \
    "    %s = addere.s32 %s, %r7\n" \
    "    %s = addere.s32 %s, %r8\n" \
    "    %s = addere.s32 %s, %r9\n" \
    "    %s = addere.s32 %s, %r10\n" \
    "    %s = addere.s32 %s, %r11\n" \
    "    %s = addere.s32 %s, %r12\n" \
    "    %s = addere.s32 %s, %r13\n" \
    "    %s = addere.s32 %s, %r14\n" \
    "    %e = extendere.s32.s64 %s\n" \
    "    %u = addere.s64 %t, %e\n" \
    "    redde %u\n" \
    "\n" \
    "functio $m_effusa () -> s64\n" \
    "@initium:\n" \
    "    %t = vocare.s64 $effusa, s32 100\n" \
    "    redde %t\n" \
    "\n" \
    "functio $m_multa () -> s64\n" \
    "@initium:\n" \
    "    %t = vocare.s64 $multa, s32 1, s32 2, s32 3, s32 4, s32 5, " \
    "s32 6, s32 7, s32 8, s32 -9, s64 10000000000, s8 -11, " \
    "i16 60000, s32 13, i8 200, s64 -70000000000\n" \
    "    redde %t\n" \
    "\n" \
    "functio $m_vocans () -> s64\n" \
    "@initium:\n" \
    "    %t = vocare.s64 $vocans, s32 1, s64 123456789012\n" \
    "    redde %t\n" \
    "\n" \
    "functio $h (s32 %a) -> s32\n" \
    "@initium:\n" \
    "    %b = addere.s32 %a, 1\n" \
    "    redde %b\n" \
    "\n" \
    "functio $bis (s32 %a) -> s32\n" \
    "@initium:\n" \
    "    %b = addere.s32 %a, 2\n" \
    "    redde %b\n" \
    "\n" \
    "functio $per (i64 %f, s32 %a) -> s32\n" \
    "@initium:\n" \
    "    %t = movere.s32 0\n" \
    "    %t = vocare.s32 %f, s32 %a\n" \
    "    %u = addere.s32 %t, %a\n" \
    "    %v = subtrahere.s32 %u, %a\n" \
    "    redde %v\n" \
    "\n" \
    "functio $m_per_h () -> s32\n" \
    "@initium:\n" \
    "    %l = locus $h\n" \
    "    %t = vocare.s32 $per, i64 %l, s32 41\n" \
    "    redde %t\n" \
    "\n" \
    "functio $m_per_bis () -> s32\n" \
    "@initium:\n" \
    "    %l = locus $bis\n" \
    "    %t = vocare.s32 $per, i64 %l, s32 40\n" \
    "    redde %t\n"

interior vacuum
_sectio_dorsum_argumenta_multa (
    CredoContextus* c)
{
    constans character* cursus[]   = {
        "m_multa", "m_vocans", "m_effusa"
    };
    constans character* nativae[]  = { "multa", "vocans", "effusa" };
       Machinula* m;
    StatusCursus  s;

    CREDO_NECESSE (_parare(c->piscina));
    m = _machinulam_parare(c->piscina, TEXTUS_MULTORUM);
    CREDO_NECESSE_NON_NIHIL (m);
    /* lex (interpretata): 1 + 8*3 - 9*5 + 10^10*7 - 11*11 + 60000*13
     * + 13*17 + 200*19 - 7*10^10*23 */
    s = _statum_capere(c->piscina, m, "m_multa");
    CREDO_AEQUALIS_S32 (s.genus, MACHINULA_BENE);
    CREDO_VERUM (s.codex == (s64)I + XXIV - XLV + (s64)70000000000L
        - CXXI + (s64)780000L + CCXXI + (s64)3800L
        - (s64)1610000000000L + (s64)(989L * 29L));
    _cursus_conferre(c, m, cursus, III, nativae, III);
    CREDO_VERUM (_signum_functionis("multa")
        == (s64)CONEXIO_SIGNUM_NATIVUM);
}

interior vacuum
_sectio_dorsum_indirecta (
    CredoContextus* c)
{
       Machinula* m;
    StatusCursus  s;
          chorda  causa;

    CREDO_NECESSE (_parare(c->piscina));
    m = _machinulam_parare(c->piscina, TEXTUS_MULTORUM);
    CREDO_NECESSE_NON_NIHIL (m);
    s = _statum_capere(c->piscina, m, "m_per_h");
    CREDO_VERUM (s.codex == (s64)XLII);
    s = _statum_capere(c->piscina, m, "m_per_bis");
    CREDO_VERUM (s.codex == (s64)XLII);

    CREDO_VERUM (machinula_nativum_compilare(m, _ch("h"), &causa));
    CREDO_VERUM (machinula_nativum_compilare(m, _ch("per"), &causa));
    CREDO_VERUM (_signum_functionis("bis")
        == (s64)CONEXIO_SIGNUM_INTERPRETATUM);
    /* per descriptorem functionis NATIVAE: vocatur */
    s = _statum_capere(c->piscina, m, "m_per_h");
    CREDO_AEQUALIS_S32 (s.genus, MACHINULA_BENE);
    CREDO_VERUM (s.codex == (s64)XLII);
    /* per descriptorem INTERPRETATUM: halitus nominatus, non ruina
     * (limes C9 in exsecutione); tabulata: m_per_bis, per */
    s = _statum_capere(c->piscina, m, "m_per_bis");
    CREDO_AEQUALIS_S32 (s.genus, MACHINULA_VITIUM);
    CREDO_VERUM (chorda_aequalis_literis(s.nuntius,
        "vocatio indirecta non nativa"));
    CREDO_AEQUALIS_I32 (s.tabulata, II);
    /* machinula iterum utilis; vocata deinde compilata: vocatur */
    s = _statum_capere(c->piscina, m, "m_per_h");
    CREDO_VERUM (s.codex == (s64)XLII);
    CREDO_VERUM (machinula_nativum_compilare(m, _ch("bis"), &causa));
    s = _statum_capere(c->piscina, m, "m_per_bis");
    CREDO_AEQUALIS_S32 (s.genus, MACHINULA_BENE);
    CREDO_VERUM (s.codex == (s64)XLII);
}


    /* ==================================================
     * DORSUM T8 gradus IV: aggregata (B5, Apple AAPCS64). Reditus
     * <= XVI octetorum in x0:x1 (vocans in locum suum condit, octetos
     * EXACTE), maior per x8; parametrum <= XVI octetorum in registris
     * x (aut totum in stiva), maius inscriptione copiae.
     * Arca vocantis signis impletur: octeti extra aggregatum manere
     * debent.
     * ================================================== */

#define TEXTUS_STRUCTURARUM \
    "modulus \"probatio/structurae.medulla\"\n" \
    "\n" \
    "functio $implere (i64 %l) -> vacuum\n" \
    "@initium:\n" \
    "    scribere.s64 %l, 1234605616436508552\n" \
    "    %p = addere.i64 %l, 8\n" \
    "    scribere.s64 %p, 1234605616436508553\n" \
    "    %p = addere.i64 %l, 16\n" \
    "    scribere.s64 %p, 1234605616436508554\n" \
    "    %p = addere.i64 %l, 24\n" \
    "    scribere.s64 %p, 1234605616436508555\n" \
    "    redde\n" \
    "\n" \
    "functio $miscere (i64 %l) -> s64\n" \
    "@initium:\n" \
    "    %s = legere.s64 %l\n" \
    "    %p = addere.i64 %l, 8\n" \
    "    %w = legere.s64 %p\n" \
    "    %t = multiplicare.s64 %w, 3\n" \
    "    %s = addere.s64 %s, %t\n" \
    "    %p = addere.i64 %l, 16\n" \
    "    %w = legere.s64 %p\n" \
    "    %t = multiplicare.s64 %w, 5\n" \
    "    %s = addere.s64 %s, %t\n" \
    "    %p = addere.i64 %l, 24\n" \
    "    %w = legere.s64 %p\n" \
    "    %t = multiplicare.s64 %w, 7\n" \
    "    %s = addere.s64 %s, %t\n" \
    "    redde %s\n" \
    "\n" \
    "functio $par (r16 %reditus, s64 %a, s64 %b) -> vacuum\n" \
    "@initium:\n" \
    "    scribere.s64 %reditus, %b\n" \
    "    %p = addere.i64 %reditus, 8\n" \
    "    scribere.s64 %p, %a\n" \
    "    redde\n" \
    "\n" \
    "functio $unum (r8 %reditus, s64 %a) -> vacuum\n" \
    "@initium:\n" \
    "    %b = multiplicare.s64 %a, 9\n" \
    "    scribere.s64 %reditus, %b\n" \
    "    redde\n" \
    "\n" \
    "functio $duodecim (r12 %reditus, s64 %a, s32 %b) -> vacuum\n" \
    "@initium:\n" \
    "    scribere.s64 %reditus, %a\n" \
    "    %p = addere.i64 %reditus, 8\n" \
    "    scribere.s32 %p, %b\n" \
    "    redde\n" \
    "\n" \
    "functio $tria (r3 %reditus, s32 %a) -> vacuum\n" \
    "@initium:\n" \
    "    scribere.i8 %reditus, %a\n" \
    "    %p = addere.i64 %reditus, 1\n" \
    "    %b = addere.s32 %a, 1\n" \
    "    scribere.i8 %p, %b\n" \
    "    %q = addere.i64 %reditus, 2\n" \
    "    %c = addere.s32 %a, 2\n" \
    "    scribere.i8 %q, %c\n" \
    "    redde\n" \
    "\n" \
    "functio $triplex (r24 %reditus, s64 %a) -> vacuum\n" \
    "@initium:\n" \
    "    scribere.s64 %reditus, %a\n" \
    "    %p = addere.i64 %reditus, 8\n" \
    "    %b = addere.s64 %a, 1\n" \
    "    scribere.s64 %p, %b\n" \
    "    %q = addere.i64 %reditus, 16\n" \
    "    %c = addere.s64 %a, 2\n" \
    "    scribere.s64 %q, %c\n" \
    "    redde\n" \
    "\n" \
    "functio $usus_par (s64 %a) -> s64\n" \
    "@initium:\n" \
    "    %l = arca 32, 8\n" \
    "    vocare $implere, i64 %l\n" \
    "    vocare $par, r16 %l, s64 %a, s64 77\n" \
    "    %t = vocare.s64 $miscere, i64 %l\n" \
    "    redde %t\n" \
    "\n" \
    "functio $usus_unum (s64 %a) -> s64\n" \
    "@initium:\n" \
    "    %l = arca 32, 8\n" \
    "    vocare $implere, i64 %l\n" \
    "    vocare $unum, r8 %l, s64 %a\n" \
    "    %t = vocare.s64 $miscere, i64 %l\n" \
    "    redde %t\n" \
    "\n" \
    "functio $usus_duodecim (s64 %a) -> s64\n" \
    "@initium:\n" \
    "    %l = arca 32, 8\n" \
    "    vocare $implere, i64 %l\n" \
    "    vocare $duodecim, r12 %l, s64 %a, s32 -9\n" \
    "    %t = vocare.s64 $miscere, i64 %l\n" \
    "    redde %t\n" \
    "\n" \
    "functio $usus_tria (s32 %a) -> s64\n" \
    "@initium:\n" \
    "    %l = arca 32, 8\n" \
    "    vocare $implere, i64 %l\n" \
    "    vocare $tria, r3 %l, s32 %a\n" \
    "    %t = vocare.s64 $miscere, i64 %l\n" \
    "    redde %t\n" \
    "\n" \
    "functio $usus_triplex (s64 %a) -> s64\n" \
    "@initium:\n" \
    "    %l = arca 32, 8\n" \
    "    vocare $implere, i64 %l\n" \
    "    vocare $triplex, r24 %l, s64 %a\n" \
    "    %t = vocare.s64 $miscere, i64 %l\n" \
    "    redde %t\n" \
    "\n" \
    "functio $usus_ultimus (s64 %a) -> s64\n" \
    "@initium:\n" \
    "    %l = arca 32, 8\n" \
    "    vocare $implere, i64 %l\n" \
    "    %d = addere.i64 %l, 16\n" \
    "    vocare $par, r16 %d, s64 %a, s64 78\n" \
    "    %t = vocare.s64 $miscere, i64 %l\n" \
    "    redde %t\n" \
    "\n" \
    "functio $summa_a (a16 %p, s64 %k) -> s64\n" \
    "@initium:\n" \
    "    %x = legere.s64 %p\n" \
    "    %q = addere.i64 %p, 8\n" \
    "    %y = legere.s64 %q\n" \
    "    %t = multiplicare.s64 %y, 3\n" \
    "    %u = addere.s64 %x, %t\n" \
    "    %v = addere.s64 %u, %k\n" \
    "    redde %v\n" \
    "\n" \
    "functio $usus_summa (s64 %a) -> s64\n" \
    "@initium:\n" \
    "    %l = arca 16, 8\n" \
    "    scribere.s64 %l, %a\n" \
    "    %q = addere.i64 %l, 8\n" \
    "    %b = addere.s64 %a, 100\n" \
    "    scribere.s64 %q, %b\n" \
    "    %t = vocare.s64 $summa_a, a16 %l, s64 1000000\n" \
    "    redde %t\n" \
    "\n" \
    "functio $parva (a3 %t, a12 %d, a8 %o, a24 %g, s32 %k) -> " \
    "s64\n" \
    "@initium:\n" \
    "    %b0 = legere.s8 %t\n" \
    "    %p = addere.i64 %t, 2\n" \
    "    %b2 = legere.i8 %p\n" \
    "    %d0 = legere.s64 %d\n" \
    "    %p = addere.i64 %d, 8\n" \
    "    %d1 = legere.s32 %p\n" \
    "    %o0 = legere.s64 %o\n" \
    "    %p = addere.i64 %g, 16\n" \
    "    %g2 = legere.s64 %p\n" \
    "    %s = extendere.s8.s64 %b0\n" \
    "    %e = extendere.i8.s64 %b2\n" \
    "    %m = multiplicare.s64 %e, 3\n" \
    "    %s = addere.s64 %s, %m\n" \
    "    %m = multiplicare.s64 %d0, 5\n" \
    "    %s = addere.s64 %s, %m\n" \
    "    %e = extendere.s32.s64 %d1\n" \
    "    %m = multiplicare.s64 %e, 7\n" \
    "    %s = addere.s64 %s, %m\n" \
    "    %m = multiplicare.s64 %o0, 11\n" \
    "    %s = addere.s64 %s, %m\n" \
    "    %m = multiplicare.s64 %g2, 13\n" \
    "    %s = addere.s64 %s, %m\n" \
    "    %e = extendere.s32.s64 %k\n" \
    "    %m = multiplicare.s64 %e, 17\n" \
    "    %s = addere.s64 %s, %m\n" \
    "    redde %s\n" \
    "\n" \
    "functio $usus_parva (s32 %k) -> s64\n" \
    "@initium:\n" \
    "    %l = arca 64, 8\n" \
    "    vocare $implere, i64 %l\n" \
    "    %g = addere.i64 %l, 32\n" \
    "    vocare $implere, i64 %g\n" \
    "    %d = addere.i64 %l, 8\n" \
    "    %o = addere.i64 %l, 24\n" \
    "    %r = vocare.s64 $parva, a3 %l, a12 %d, a8 %o, a24 %g, " \
    "s32 %k\n" \
    "    redde %r\n" \
    "\n" \
    "functio $sex (s64 %a1, s64 %a2, s64 %a3, s64 %a4, s64 %a5, " \
    "s64 %a6, a16 %p, s64 %k) -> s64\n" \
    "@initium:\n" \
    "    %x = legere.s64 %p\n" \
    "    %q = addere.i64 %p, 8\n" \
    "    %y = legere.s64 %q\n" \
    "    %s = multiplicare.s64 %x, 3\n" \
    "    %m = multiplicare.s64 %y, 5\n" \
    "    %s = addere.s64 %s, %m\n" \
    "    %m = multiplicare.s64 %k, 7\n" \
    "    %s = addere.s64 %s, %m\n" \
    "    %s = addere.s64 %s, %a1\n" \
    "    %m = multiplicare.s64 %a6, 11\n" \
    "    %s = addere.s64 %s, %m\n" \
    "    redde %s\n" \
    "\n" \
    "functio $usus_sex (s64 %a) -> s64\n" \
    "@initium:\n" \
    "    %l = arca 64, 8\n" \
    "    vocare $implere, i64 %l\n" \
    "    %g = addere.i64 %l, 32\n" \
    "    vocare $implere, i64 %g\n" \
    "    %d = addere.i64 %l, 8\n" \
    "    %o = addere.i64 %l, 24\n" \
    "    %r = vocare.s64 $sex, s64 %a, s64 2, s64 3, s64 4, s64 " \
    "5, s64 6, a16 %d, s64 -8\n" \
    "    redde %r\n" \
    "\n" \
    "functio $mixta (s64 %a1, s64 %a2, s64 %a3, s64 %a4, s64 " \
    "%a5, s64 %a6, s64 %a7, a16 %p, s64 %k, a8 %o, a12 %d, a24 " \
    "%g, s8 %c) -> s64\n" \
    "@initium:\n" \
    "    %x = legere.s64 %p\n" \
    "    %q = addere.i64 %p, 8\n" \
    "    %y = legere.s64 %q\n" \
    "    %s = multiplicare.s64 %x, 3\n" \
    "    %m = multiplicare.s64 %y, 5\n" \
    "    %s = addere.s64 %s, %m\n" \
    "    %m = multiplicare.s64 %k, 7\n" \
    "    %s = addere.s64 %s, %m\n" \
    "    %s = addere.s64 %s, %a1\n" \
    "    %m = multiplicare.s64 %a7, 2\n" \
    "    %s = addere.s64 %s, %m\n" \
    "    %o0 = legere.s64 %o\n" \
    "    %m = multiplicare.s64 %o0, 11\n" \
    "    %s = addere.s64 %s, %m\n" \
    "    %d0 = legere.s64 %d\n" \
    "    %m = multiplicare.s64 %d0, 13\n" \
    "    %s = addere.s64 %s, %m\n" \
    "    %q = addere.i64 %d, 8\n" \
    "    %d1 = legere.s32 %q\n" \
    "    %e = extendere.s32.s64 %d1\n" \
    "    %m = multiplicare.s64 %e, 17\n" \
    "    %s = addere.s64 %s, %m\n" \
    "    %q = addere.i64 %g, 16\n" \
    "    %g2 = legere.s64 %q\n" \
    "    %m = multiplicare.s64 %g2, 19\n" \
    "    %s = addere.s64 %s, %m\n" \
    "    %c32 = addere.s32 %c, 1000\n" \
    "    %e = extendere.s32.s64 %c32\n" \
    "    %m = multiplicare.s64 %e, 23\n" \
    "    %s = addere.s64 %s, %m\n" \
    "    redde %s\n" \
    "\n" \
    "functio $usus_mixta (s64 %a) -> s64\n" \
    "@initium:\n" \
    "    %l = arca 64, 8\n" \
    "    vocare $implere, i64 %l\n" \
    "    %g = addere.i64 %l, 32\n" \
    "    vocare $implere, i64 %g\n" \
    "    %d = addere.i64 %l, 8\n" \
    "    %o = addere.i64 %l, 24\n" \
    "    %r = vocare.s64 $mixta, s64 %a, s64 2, s64 3, s64 4, " \
    "s64 5, s64 6, s64 7, a16 %l, s64 -8, a8 %o, a12 %d, a24 %g, " \
    "s8 -3\n" \
    "    redde %r\n" \
    "\n" \
    "functio $m_par () -> s64\n" \
    "@initium:\n" \
    "    %t = vocare.s64 $usus_par, s64 5\n" \
    "    redde %t\n" \
    "\n" \
    "functio $m_unum () -> s64\n" \
    "@initium:\n" \
    "    %t = vocare.s64 $usus_unum, s64 5\n" \
    "    redde %t\n" \
    "\n" \
    "functio $m_duodecim () -> s64\n" \
    "@initium:\n" \
    "    %t = vocare.s64 $usus_duodecim, s64 5\n" \
    "    redde %t\n" \
    "\n" \
    "functio $m_tria () -> s64\n" \
    "@initium:\n" \
    "    %t = vocare.s64 $usus_tria, s32 200\n" \
    "    redde %t\n" \
    "\n" \
    "functio $m_triplex () -> s64\n" \
    "@initium:\n" \
    "    %t = vocare.s64 $usus_triplex, s64 5\n" \
    "    redde %t\n" \
    "\n" \
    "functio $m_ultimus () -> s64\n" \
    "@initium:\n" \
    "    %t = vocare.s64 $usus_ultimus, s64 5\n" \
    "    redde %t\n" \
    "\n" \
    "functio $m_summa () -> s64\n" \
    "@initium:\n" \
    "    %t = vocare.s64 $usus_summa, s64 5\n" \
    "    redde %t\n" \
    "\n" \
    "functio $m_parva () -> s64\n" \
    "@initium:\n" \
    "    %t = vocare.s64 $usus_parva, s32 -4\n" \
    "    redde %t\n" \
    "\n" \
    "functio $m_sex () -> s64\n" \
    "@initium:\n" \
    "    %t = vocare.s64 $usus_sex, s64 5\n" \
    "    redde %t\n" \
    "\n" \
    "functio $m_mixta () -> s64\n" \
    "@initium:\n" \
    "    %t = vocare.s64 $usus_mixta, s64 5\n" \
    "    redde %t\n" \
    "\n" \
    "functio $hfa (h2xf64 %p) -> s32\n" \
    "@initium:\n" \
    "    redde 1\n"

interior vacuum
_sectio_dorsum_chorda_reddita (
    CredoContextus* c)
{
    constans character* cursus[]    = {
        "m_par", "m_unum", "m_duodecim", "m_tria", "m_triplex",
        "m_ultimus"
    };
    constans character* vocatae[]   = {
        "par", "unum", "duodecim", "tria", "triplex"
    };
    constans character* vocantes[]  = {
        "usus_par", "usus_unum", "usus_duodecim", "usus_tria",
        "usus_triplex", "usus_ultimus"
    };
             i64 signum = (i64)0x11223344UL << XXXII
                         | (i64)0x55667788UL;
       Machinula* m;
    StatusCursus  s;

    CREDO_NECESSE (_parare(c->piscina));
    m = _machinulam_parare(c->piscina, TEXTUS_STRUCTURARUM);
    CREDO_NECESSE_NON_NIHIL (m);
    /* lex (interpretata): verba {77, 5, S+2, S+3} per 1, 3, 5, 7
     * ($par verba PERMUTAT: x1 intrans non est verbum secundum) */
    s = _statum_capere(c->piscina, m, "m_par");
    CREDO_AEQUALIS_S32 (s.genus, MACHINULA_BENE);
    CREDO_VERUM ((i64)s.codex == (i64)LXXVII + (i64)V * III
        + (signum + II) * V + (signum + III) * VII);
    /* r12: octeti XII - XV verbi secundi signum servant */
    s = _statum_capere(c->piscina, m, "m_duodecim");
    CREDO_VERUM ((i64)s.codex == (i64)V
        + ((((signum + I) >> XXXII) << XXXII) | (i64)0xFFFFFFF7UL) * III
        + (signum + II) * V + (signum + III) * VII);
    /* vocans interpretatus, vocata nativa (limen machinulae) */
    _cursus_conferre(c, m, cursus, VI, vocatae, V);
    CREDO_VERUM (_signum_functionis("par")
        == (s64)CONEXIO_SIGNUM_NATIVUM);
    CREDO_VERUM (_signum_functionis("usus_par")
        == (s64)CONEXIO_SIGNUM_INTERPRETATUM);
    /* vocans nativus, vocata nativa */
    _cursus_conferre(c, m, cursus, VI, vocantes, VI);
    CREDO_VERUM (_signum_functionis("usus_ultimus")
        == (s64)CONEXIO_SIGNUM_NATIVUM);
}

interior vacuum
_sectio_dorsum_aggregata (
    CredoContextus* c)
{
    constans character* cursus[]    = {
        "m_summa", "m_parva", "m_sex", "m_mixta"
    };
    constans character* vocatae[]   = {
        "summa_a", "parva", "sex", "mixta"
    };
    constans character* vocantes[]  = {
        "usus_summa", "usus_parva", "usus_sex", "usus_mixta"
    };
       Machinula* m;
    StatusCursus  s;
          chorda  causa;

    CREDO_NECESSE (_parare(c->piscina));
    m = _machinulam_parare(c->piscina, TEXTUS_STRUCTURARUM);
    CREDO_NECESSE_NON_NIHIL (m);
    /* lex (interpretata): 5 + 105*3 + 1000000 */
    s = _statum_capere(c->piscina, m, "m_summa");
    CREDO_AEQUALIS_S32 (s.genus, MACHINULA_BENE);
    CREDO_VERUM (s.codex == (s64)V + CCCXV + (s64)1000000L);
    _cursus_conferre(c, m, cursus, IV, vocatae, IV);
    CREDO_VERUM (_signum_functionis("mixta")
        == (s64)CONEXIO_SIGNUM_NATIVUM);
    CREDO_VERUM (_signum_functionis("usus_mixta")
        == (s64)CONEXIO_SIGNUM_INTERPRETATUM);
    _cursus_conferre(c, m, cursus, IV, vocantes, IV);
    CREDO_VERUM (_signum_functionis("usus_mixta")
        == (s64)CONEXIO_SIGNUM_NATIVUM);
    /* aggregatum fluitans homogeneum: a T9 gradu III compilatur
     * (sectio 'dorsum aggregata fluitantia') */
    CREDO_VERUM (machinula_nativum_compilare(m, _ch("hfa"), &causa));
}


    /* ==================================================
     * DORSUM T8 gradus IV: aggregata contra COMPILATOREM hospitis.
     * Probationes superiores dorsum cum machinula conferunt (ambo
     * eodem modo errare possent); hic iudex est clang ipse:
     *  I.   vocans interpretatus -> functio C (limen machinulae);
     *  II.  vocans nativus -> functio C (per descriptorem);
     *  III. C -> functio dorsi (introitus ut monstrator C typatus).
     * ================================================== */

nomen structura { s64 a; s64 b; } ParHospitis;
nomen structura { s32 a; s32 b; s32 c; } TernaHospitis;
nomen structura { i8 a; i8 b; i8 c; } TriaHospitis;
nomen structura { s64 a; s64 b; s64 c; } TriplexHospitis;
nomen structura { s64 a; } UnumHospitis;
nomen structura { s64 a; s32 b; } DuodecimHospitis;

#define SIGNUM_HOSPITIS \
    ((i64)0x11223344UL << XXXII | (i64)0x55667788UL)

interior ParHospitis
_hospes_par (
    s64 a,
    s64 b)
{
    ParHospitis p;

    p.a = a;
    p.b = b;
    redde p;
}

interior TernaHospitis
_hospes_terna (
    s32 a)
{
    TernaHospitis p;

    p.a = a;
    p.b = a + I;
    p.c = a + II;
    redde p;
}

interior TriaHospitis
_hospes_tria (
    s32 a)
{
    TriaHospitis p;

    p.a = (i8)a;
    p.b = (i8)(a + I);
    p.c = (i8)(a + II);
    redde p;
}

interior TriplexHospitis
_hospes_triplex (
    s64 a)
{
    TriplexHospitis p;

    p.a = a;
    p.b = a + I;
    p.c = a + II;
    redde p;
}

interior s64
_hospes_mixta (
                   s64 a1,
                   s64 a2,
                   s64 a3,
                   s64 a4,
                   s64 a5,
                   s64 a6,
                   s64 a7,
           ParHospitis p,
                   s64 k,
          UnumHospitis o,
      DuodecimHospitis d,
       TriplexHospitis g,
    signatus character c)
{
    (vacuum)a2;
    (vacuum)a3;
    (vacuum)a4;
    (vacuum)a5;
    (vacuum)a6;
    redde (s64)((i64)p.a * III + (i64)p.b * V + (i64)k * VII + (i64)a1
        + (i64)a7 * II + (i64)o.a * XI + (i64)d.a * XIII
        + (i64)(s64)d.b * XVII + (i64)g.c * XIX
        + (i64)(s64)((s32)c + M) * XXIII);
}

interior s64
_hospes_impar (
                   s64 a1,
                   s64 a2,
                   s64 a3,
                   s64 a4,
                   s64 a5,
                   s64 a6,
                   s64 a7,
                   s64 a8,
    signatus character c,
          UnumHospitis o,
          TriaHospitis t)
{
    (vacuum)a1;
    (vacuum)a2;
    (vacuum)a3;
    (vacuum)a4;
    (vacuum)a5;
    (vacuum)a6;
    (vacuum)a7;
    redde (s64)((i64)o.a * III + (i64)a8
        + (i64)(s64)((s32)c + M) * V + (i64)t.c * VII
        + (i64)(s64)(signatus character)t.a * XI);
}

#define TEXTUS_HOSPITIS \
    "modulus \"probatio/hospes.medulla\"\n" \
    "\n" \
    "functio $implere (i64 %l) -> vacuum\n" \
    "@initium:\n" \
    "    scribere.s64 %l, 1234605616436508552\n" \
    "    %p = addere.i64 %l, 8\n" \
    "    scribere.s64 %p, 1234605616436508553\n" \
    "    %p = addere.i64 %l, 16\n" \
    "    scribere.s64 %p, 1234605616436508554\n" \
    "    %p = addere.i64 %l, 24\n" \
    "    scribere.s64 %p, 1234605616436508555\n" \
    "    redde\n" \
    "\n" \
    "functio $miscere (i64 %l) -> s64\n" \
    "@initium:\n" \
    "    %s = legere.s64 %l\n" \
    "    %p = addere.i64 %l, 8\n" \
    "    %w = legere.s64 %p\n" \
    "    %t = multiplicare.s64 %w, 3\n" \
    "    %s = addere.s64 %s, %t\n" \
    "    %p = addere.i64 %l, 16\n" \
    "    %w = legere.s64 %p\n" \
    "    %t = multiplicare.s64 %w, 5\n" \
    "    %s = addere.s64 %s, %t\n" \
    "    %p = addere.i64 %l, 24\n" \
    "    %w = legere.s64 %p\n" \
    "    %t = multiplicare.s64 %w, 7\n" \
    "    %s = addere.s64 %s, %t\n" \
    "    redde %s\n" \
    "\n" \
    "functio $h_par (r16 %reditus, s64 %a, s64 %b) -> vacuum\n" \
    "@initium:\n" \
    "    redde\n" \
    "\n" \
    "functio $h_terna (r12 %reditus, s32 %a) -> vacuum\n" \
    "@initium:\n" \
    "    redde\n" \
    "\n" \
    "functio $h_tria (r3 %reditus, s32 %a) -> vacuum\n" \
    "@initium:\n" \
    "    redde\n" \
    "\n" \
    "functio $h_triplex (r24 %reditus, s64 %a) -> vacuum\n" \
    "@initium:\n" \
    "    redde\n" \
    "\n" \
    "functio $h_mixta (s64 %a1, s64 %a2, s64 %a3, s64 %a4, s64 " \
    "%a5, s64 %a6, s64 %a7, a16 %p, s64 %k, a8 %o, a12 %d, a24 " \
    "%g, s8 %c) -> s64\n" \
    "@initium:\n" \
    "    redde 0\n" \
    "\n" \
    "functio $h_impar (s64 %a1, s64 %a2, s64 %a3, s64 %a4, s64 " \
    "%a5, s64 %a6, s64 %a7, s64 %a8, s8 %c, a8 %o, a3 %t) -> " \
    "s64\n" \
    "@initium:\n" \
    "    redde 0\n" \
    "\n" \
    "functio $per_par (i64 %f, s64 %a) -> s64\n" \
    "@initium:\n" \
    "    %l = arca 32, 8\n" \
    "    vocare $implere, i64 %l\n" \
    "    vocare %f, r16 %l, s64 %a, s64 77\n" \
    "    %t = vocare.s64 $miscere, i64 %l\n" \
    "    redde %t\n" \
    "\n" \
    "functio $per_terna (i64 %f, s32 %a) -> s64\n" \
    "@initium:\n" \
    "    %l = arca 32, 8\n" \
    "    vocare $implere, i64 %l\n" \
    "    vocare %f, r12 %l, s32 %a\n" \
    "    %t = vocare.s64 $miscere, i64 %l\n" \
    "    redde %t\n" \
    "\n" \
    "functio $per_tria (i64 %f, s32 %a) -> s64\n" \
    "@initium:\n" \
    "    %l = arca 32, 8\n" \
    "    vocare $implere, i64 %l\n" \
    "    vocare %f, r3 %l, s32 %a\n" \
    "    %t = vocare.s64 $miscere, i64 %l\n" \
    "    redde %t\n" \
    "\n" \
    "functio $per_triplex (i64 %f, s64 %a) -> s64\n" \
    "@initium:\n" \
    "    %l = arca 32, 8\n" \
    "    vocare $implere, i64 %l\n" \
    "    vocare %f, r24 %l, s64 %a\n" \
    "    %t = vocare.s64 $miscere, i64 %l\n" \
    "    redde %t\n" \
    "\n" \
    "functio $per_mixta (i64 %f, s64 %a) -> s64\n" \
    "@initium:\n" \
    "    %l = arca 64, 8\n" \
    "    vocare $implere, i64 %l\n" \
    "    %g = addere.i64 %l, 32\n" \
    "    vocare $implere, i64 %g\n" \
    "    %d = addere.i64 %l, 8\n" \
    "    %o = addere.i64 %l, 24\n" \
    "    %r = vocare.s64 %f, s64 %a, s64 2, s64 3, s64 4, s64 5, " \
    "s64 6, s64 7, a16 %l, s64 -8, a8 %o, a12 %d, a24 %g, s8 -3\n" \
    "    redde %r\n" \
    "\n" \
    "functio $per_impar (i64 %f, s64 %a) -> s64\n" \
    "@initium:\n" \
    "    %l = arca 64, 8\n" \
    "    vocare $implere, i64 %l\n" \
    "    %g = addere.i64 %l, 32\n" \
    "    vocare $implere, i64 %g\n" \
    "    %d = addere.i64 %l, 8\n" \
    "    %o = addere.i64 %l, 24\n" \
    "    %r = vocare.s64 %f, s64 %a, s64 2, s64 3, s64 4, s64 5, " \
    "s64 6, s64 7, s64 8, s8 -3, a8 %o, a3 %l\n" \
    "    redde %r\n" \
    "\n" \
    "functio $mh_par () -> s64\n" \
    "@initium:\n" \
    "    %f = locus $h_par\n" \
    "    %t = vocare.s64 $per_par, i64 %f, s64 5\n" \
    "    redde %t\n" \
    "\n" \
    "functio $mh_terna () -> s64\n" \
    "@initium:\n" \
    "    %f = locus $h_terna\n" \
    "    %t = vocare.s64 $per_terna, i64 %f, s32 -70000\n" \
    "    redde %t\n" \
    "\n" \
    "functio $mh_tria () -> s64\n" \
    "@initium:\n" \
    "    %f = locus $h_tria\n" \
    "    %t = vocare.s64 $per_tria, i64 %f, s32 200\n" \
    "    redde %t\n" \
    "\n" \
    "functio $mh_triplex () -> s64\n" \
    "@initium:\n" \
    "    %f = locus $h_triplex\n" \
    "    %t = vocare.s64 $per_triplex, i64 %f, s64 5\n" \
    "    redde %t\n" \
    "\n" \
    "functio $mh_mixta () -> s64\n" \
    "@initium:\n" \
    "    %f = locus $h_mixta\n" \
    "    %t = vocare.s64 $per_mixta, i64 %f, s64 5\n" \
    "    redde %t\n" \
    "\n" \
    "functio $mh_impar () -> s64\n" \
    "@initium:\n" \
    "    %f = locus $h_impar\n" \
    "    %t = vocare.s64 $per_impar, i64 %f, s64 5\n" \
    "    redde %t\n" \
    "\n" \
    "functio $par (r16 %reditus, s64 %a, s64 %b) -> vacuum\n" \
    "@initium:\n" \
    "    scribere.s64 %reditus, %b\n" \
    "    %p = addere.i64 %reditus, 8\n" \
    "    scribere.s64 %p, %a\n" \
    "    redde\n" \
    "\n" \
    "functio $tria (r3 %reditus, s32 %a) -> vacuum\n" \
    "@initium:\n" \
    "    scribere.i8 %reditus, %a\n" \
    "    %p = addere.i64 %reditus, 1\n" \
    "    %b = addere.s32 %a, 1\n" \
    "    scribere.i8 %p, %b\n" \
    "    %q = addere.i64 %reditus, 2\n" \
    "    %c = addere.s32 %a, 2\n" \
    "    scribere.i8 %q, %c\n" \
    "    redde\n" \
    "\n" \
    "functio $triplex (r24 %reditus, s64 %a) -> vacuum\n" \
    "@initium:\n" \
    "    scribere.s64 %reditus, %a\n" \
    "    %p = addere.i64 %reditus, 8\n" \
    "    %b = addere.s64 %a, 1\n" \
    "    scribere.s64 %p, %b\n" \
    "    %q = addere.i64 %reditus, 16\n" \
    "    %c = addere.s64 %a, 2\n" \
    "    scribere.s64 %q, %c\n" \
    "    redde\n" \
    "\n" \
    "functio $mixta (s64 %a1, s64 %a2, s64 %a3, s64 %a4, s64 " \
    "%a5, s64 %a6, s64 %a7, a16 %p, s64 %k, a8 %o, a12 %d, a24 " \
    "%g, s8 %c) -> s64\n" \
    "@initium:\n" \
    "    %x = legere.s64 %p\n" \
    "    %q = addere.i64 %p, 8\n" \
    "    %y = legere.s64 %q\n" \
    "    %s = multiplicare.s64 %x, 3\n" \
    "    %m = multiplicare.s64 %y, 5\n" \
    "    %s = addere.s64 %s, %m\n" \
    "    %m = multiplicare.s64 %k, 7\n" \
    "    %s = addere.s64 %s, %m\n" \
    "    %s = addere.s64 %s, %a1\n" \
    "    %m = multiplicare.s64 %a7, 2\n" \
    "    %s = addere.s64 %s, %m\n" \
    "    %o0 = legere.s64 %o\n" \
    "    %m = multiplicare.s64 %o0, 11\n" \
    "    %s = addere.s64 %s, %m\n" \
    "    %d0 = legere.s64 %d\n" \
    "    %m = multiplicare.s64 %d0, 13\n" \
    "    %s = addere.s64 %s, %m\n" \
    "    %q = addere.i64 %d, 8\n" \
    "    %d1 = legere.s32 %q\n" \
    "    %e = extendere.s32.s64 %d1\n" \
    "    %m = multiplicare.s64 %e, 17\n" \
    "    %s = addere.s64 %s, %m\n" \
    "    %q = addere.i64 %g, 16\n" \
    "    %g2 = legere.s64 %q\n" \
    "    %m = multiplicare.s64 %g2, 19\n" \
    "    %s = addere.s64 %s, %m\n" \
    "    %c32 = addere.s32 %c, 1000\n" \
    "    %e = extendere.s32.s64 %c32\n" \
    "    %m = multiplicare.s64 %e, 23\n" \
    "    %s = addere.s64 %s, %m\n" \
    "    redde %s\n" \
    "\n" \
    "functio $impar (s64 %a1, s64 %a2, s64 %a3, s64 %a4, s64 " \
    "%a5, s64 %a6, s64 %a7, s64 %a8, s8 %c, a8 %o, a3 %t) -> " \
    "s64\n" \
    "@initium:\n" \
    "    %o0 = legere.s64 %o\n" \
    "    %s = multiplicare.s64 %o0, 3\n" \
    "    %s = addere.s64 %s, %a8\n" \
    "    %c32 = addere.s32 %c, 1000\n" \
    "    %e = extendere.s32.s64 %c32\n" \
    "    %m = multiplicare.s64 %e, 5\n" \
    "    %s = addere.s64 %s, %m\n" \
    "    %q = addere.i64 %t, 2\n" \
    "    %b2 = legere.i8 %q\n" \
    "    %e = extendere.i8.s64 %b2\n" \
    "    %m = multiplicare.s64 %e, 7\n" \
    "    %s = addere.s64 %s, %m\n" \
    "    %b0 = legere.s8 %t\n" \
    "    %e = extendere.s8.s64 %b0\n" \
    "    %m = multiplicare.s64 %e, 11\n" \
    "    %s = addere.s64 %s, %m\n" \
    "    redde %s\n"

/* descriptorem functionis nominatae in functionem C vertit */
interior b32
_hospitem_ponere (
             Machinula* m,
    constans character* titulus,
        memoriae_index  inscriptio)
{
    vacuum* introitus;

    memcpy(&introitus, &inscriptio, magnitudo(introitus));
    redde machinula_nativum_ponere(m, _ch(titulus), introitus);
}

/* introitus functionis a dorso compilatae (descriptor NATIVUM) */
interior memoriae_index
_introitus_nativae (
    constans character* titulus)
{
    constans ConexioDescriptor* descriptor = conexio_sedes_quaerere(
        _conexio_sectionis, _ch(titulus));

    redde (descriptor != NIHIL
            && descriptor->signum == (s64)CONEXIO_SIGNUM_NATIVUM)
        ? (memoriae_index)descriptor->index : (memoriae_index)ZEPHYRUM;
}

/* lex $miscere super verba IV */
interior s64
_miscere_hospitis (
    constans i64* w)
{
    redde (s64)(w[ZEPHYRUM] + w[I] * III + w[II] * V + w[III] * VII);
}

interior vacuum
_sectio_dorsum_aggregata_hospitis (
    CredoContextus* c)
{
    constans character* cursus[]  = {
        "mh_par", "mh_terna", "mh_tria", "mh_triplex", "mh_mixta",
        "mh_impar"
    };
    constans character* vocantes[]  = {
        "per_par", "per_terna", "per_tria", "per_triplex", "per_mixta",
        "per_impar"
    };
    constans character* vocatae[]  = {
        "par", "tria", "triplex", "mixta", "impar"
    };
    ParHospitis (*h_par)(s64, s64)     = _hospes_par;
    TernaHospitis (*h_terna)(s32)      = _hospes_terna;
    TriaHospitis (*h_tria)(s32)        = _hospes_tria;
    TriplexHospitis (*h_triplex)(s64)  = _hospes_triplex;
    s64 (*h_mixta)(s64, s64, s64, s64, s64, s64, s64, ParHospitis, s64,
        UnumHospitis, DuodecimHospitis, TriplexHospitis,
        signatus character)                 = _hospes_mixta;
    s64 (*h_impar)(s64, s64, s64, s64, s64, s64, s64, s64,
        signatus character, UnumHospitis, TriaHospitis)
                                            = _hospes_impar;
                 i64 signum = SIGNUM_HOSPITIS;
                 i64 w[IV];
                 s64 exspectata[VI];
      memoriae_index inscriptio;
         ParHospitis p;
        UnumHospitis o;
    DuodecimHospitis d;
     TriplexHospitis g;
        TriaHospitis t;
           Machinula* m;
        StatusCursus  s;
              chorda  causa;
                 s32  k;
                 s32  transitus;

    CREDO_NECESSE (_parare(c->piscina));
    m = _machinulam_parare(c->piscina, TEXTUS_HOSPITIS);
    CREDO_NECESSE_NON_NIHIL (m);
    memcpy(&inscriptio, &h_par, magnitudo(inscriptio));
    CREDO_VERUM (_hospitem_ponere(m, "h_par", inscriptio));
    memcpy(&inscriptio, &h_terna, magnitudo(inscriptio));
    CREDO_VERUM (_hospitem_ponere(m, "h_terna", inscriptio));
    memcpy(&inscriptio, &h_tria, magnitudo(inscriptio));
    CREDO_VERUM (_hospitem_ponere(m, "h_tria", inscriptio));
    memcpy(&inscriptio, &h_triplex, magnitudo(inscriptio));
    CREDO_VERUM (_hospitem_ponere(m, "h_triplex", inscriptio));
    memcpy(&inscriptio, &h_mixta, magnitudo(inscriptio));
    CREDO_VERUM (_hospitem_ponere(m, "h_mixta", inscriptio));
    memcpy(&inscriptio, &h_impar, magnitudo(inscriptio));
    CREDO_VERUM (_hospitem_ponere(m, "h_impar", inscriptio));

    /* exspectata: arca vocantis verba S, S+1, S+2, S+3 (bis) tenet */
    w[ZEPHYRUM]           = V;
    w[I]                  = LXXVII;
    w[II]                 = signum + II;
    w[III]                = signum + III;
    exspectata[ZEPHYRUM]  = _miscere_hospitis(w);
    /* terna {-70000, -69999, -69998}: octeti XII; pars superior verbi
     * secundi signum servat */
    w[ZEPHYRUM] = ((i64)(i32)(s32)-70000L)
        | ((i64)(i32)(s32)-69999L << XXXII);
    w[I]        = (((signum + I) >> XXXII) << XXXII)
        | (i64)(i32)(s32)-69998L;
    exspectata[I] = _miscere_hospitis(w);
    /* tria {200, 201, 202}: octeti III infimi verbi primi */
    w[ZEPHYRUM]      = ((signum >> XXIV) << XXIV) | (i64)0xCAC9C8UL;
    w[I]             = signum + I;
    exspectata[II]   = _miscere_hospitis(w);
    w[ZEPHYRUM]      = V;
    w[I]             = VI;
    w[II]            = VII;
    exspectata[III]  = _miscere_hospitis(w);
    p.a              = (s64)signum;
    p.b              = (s64)(signum + I);
    o.a              = (s64)(signum + III);
    d.a              = (s64)(signum + I);
    d.b              = (s32)((signum + II) & (i64)0xFFFFFFFFUL);
    g.a              = (s64)signum;
    g.b              = (s64)(signum + I);
    g.c              = (s64)(signum + II);
    t.a              = (i8)0x88;
    t.b              = (i8)0x77;
    t.c              = (i8)0x66;
    exspectata[IV] = _hospes_mixta(V, II, III, IV, V, VI, VII, p,
        -(s64)VIII, o, d, g, (signatus character)-III);
    exspectata[V] = _hospes_impar(V, II, III, IV, V, VI, VII, VIII,
        (signatus character)-III, o, t);

    /* I. vocans interpretatus -> C; II. vocans nativus -> C */
    per (transitus = ZEPHYRUM; transitus < II; transitus++)
    {
        per (k = ZEPHYRUM; k < VI; k++)
        {
            s = _statum_capere(c->piscina, m, cursus[k]);
            si (s.genus != MACHINULA_BENE || s.codex != exspectata[k])
            {
                imprimere("HOSPES DISPAR %s (transitus %d): genus %d "
                    "codex %ld exspectatum %ld\n", cursus[k],
                    (int)transitus, (int)s.genus, (long)s.codex,
                    (long)exspectata[k]);
            }
            CREDO_AEQUALIS_S32 (s.genus, MACHINULA_BENE);
            CREDO_VERUM (s.codex == exspectata[k]);
        }
        si (transitus == ZEPHYRUM)
        {
            per (k = ZEPHYRUM; k < VI; k++)
            {
                CREDO_VERUM (machinula_nativum_compilare(m,
                    _ch(vocantes[k]), &causa));
            }
            CREDO_VERUM (_signum_functionis("per_mixta")
                == (s64)CONEXIO_SIGNUM_NATIVUM);
        }
    }

    /* III. C -> functio dorsi */
    per (k = ZEPHYRUM; k < V; k++)
    {
        CREDO_NECESSE (machinula_nativum_compilare(m, _ch(vocatae[k]),
            &causa));
        CREDO_NECESSE (_introitus_nativae(vocatae[k]) != ZEPHYRUM);
    }
    {
        ParHospitis (*f_par)(s64, s64);
        TriaHospitis (*f_tria)(s32);
        TriplexHospitis (*f_triplex)(s64);
        s64 (*f_mixta)(s64, s64, s64, s64, s64, s64, s64, ParHospitis,
            s64, UnumHospitis, DuodecimHospitis, TriplexHospitis,
            signatus character);
        s64 (*f_impar)(s64, s64, s64, s64, s64, s64, s64, s64,
            signatus character, UnumHospitis, TriaHospitis);
            ParHospitis rp;
           TriaHospitis rt;
        TriplexHospitis rg;
                    s64 fructus;

        inscriptio = _introitus_nativae("par");
        memcpy(&f_par, &inscriptio, magnitudo(f_par));
        rp = f_par((s64)XLI, -(s64)XLII);
        /* $par verba permutat */
        CREDO_VERUM (rp.a == -(s64)XLII);
        CREDO_VERUM (rp.b == (s64)XLI);

        inscriptio = _introitus_nativae("tria");
        memcpy(&f_tria, &inscriptio, magnitudo(f_tria));
        rt = f_tria((s32)CC);
        CREDO_VERUM (rt.a == (i8)CC);
        CREDO_VERUM (rt.b == (i8)CCI);
        CREDO_VERUM (rt.c == (i8)CCII);

        inscriptio = _introitus_nativae("triplex");
        memcpy(&f_triplex, &inscriptio, magnitudo(f_triplex));
        rg = f_triplex((s64)IX);
        CREDO_VERUM (rg.a == (s64)IX);
        CREDO_VERUM (rg.b == (s64)X);
        CREDO_VERUM (rg.c == (s64)XI);

        /* lex $mixta = lex _hospes_mixta; lex $impar = _hospes_impar */
        inscriptio = _introitus_nativae("mixta");
        memcpy(&f_mixta, &inscriptio, magnitudo(f_mixta));
        fructus = f_mixta(V, II, III, IV, V, VI, VII, p, -(s64)VIII, o,
            d, g, (signatus character)-III);
        CREDO_VERUM (fructus == exspectata[IV]);

        inscriptio = _introitus_nativae("impar");
        memcpy(&f_impar, &inscriptio, magnitudo(f_impar));
        fructus = f_impar(V, II, III, IV, V, VI, VII, VIII,
            (signatus character)-III, o, t);
        CREDO_VERUM (fructus == exspectata[V]);
    }
}


    /* ==================================================
     * DORSUM T9 gradus I: fluitantia intra functionem (registra S/D,
     * constantes, arithmetica, comparationes ordinatae, conversiones,
     * memoria). Functiones signaturam integram habent - argumenta et
     * reditus fluitantes gradus II sunt.
     * ================================================== */

#define TEXTUS_FLUITANTIUM \
    "modulus \"probatio/fluitantia.medulla\"\n" \
    "\n" \
    "functio $arith (s32 %a) -> s32\n" \
    "@initium:\n" \
    "    %f = fluitare.s32.f64 %a\n" \
    "    %g = multiplicare.f64 %f, 1.5\n" \
    "    %g = addere.f64 %g, 0.10000000000000001\n" \
    "    %h = subtrahere.f64 %g, 3.1415926535897931\n" \
    "    %h = dividere.f64 %h, -0.75\n" \
    "    %n = negare.f64 %h\n" \
    "    %m = multiplicare.f64 %n, 1000.0\n" \
    "    %p = movere.f64 %m\n" \
    "    %r = integrare.f64.s32 %p\n" \
    "    redde %r\n" \
    "\n" \
    "functio $arith32 (s32 %a) -> s32\n" \
    "@initium:\n" \
    "    %f = fluitare.s32.f32 %a\n" \
    "    %g = multiplicare.f32 %f, 1.5\n" \
    "    %g = addere.f32 %g, 0.10000000000000001\n" \
    "    %h = subtrahere.f32 %g, 3.1415926535897931\n" \
    "    %h = dividere.f32 %h, -0.75\n" \
    "    %n = negare.f32 %h\n" \
    "    %m = multiplicare.f32 %n, 1000.0\n" \
    "    %p = movere.f32 %m\n" \
    "    %r = integrare.f32.s32 %p\n" \
    "    redde %r\n" \
    "\n" \
    "functio $bita (s32 %a) -> s64\n" \
    "@initium:\n" \
    "    %l = arca 16, 8\n" \
    "    %f = fluitare.s32.f64 %a\n" \
    "    %g = dividere.f64 %f, 3.0\n" \
    "    scribere.f64 %l, %g\n" \
    "    %b = legere.s64 %l\n" \
    "    redde %b\n" \
    "\n" \
    "functio $bita32 (s32 %a) -> s64\n" \
    "@initium:\n" \
    "    %l = arca 16, 8\n" \
    "    scribere.s64 %l, -1\n" \
    "    %f = fluitare.s32.f32 %a\n" \
    "    %g = multiplicare.f32 %f, 0.20000000000000001\n" \
    "    %g = addere.f32 %g, 0.10000000000000001\n" \
    "    scribere.f32 %l, %g\n" \
    "    %b = legere.s64 %l\n" \
    "    redde %b\n" \
    "\n" \
    "functio $lege (s32 %a) -> s32\n" \
    "@initium:\n" \
    "    %l = arca 16, 8\n" \
    "    scribere.s64 %l, 4611686018427387904\n" \
    "    %p = addere.i64 %l, 8\n" \
    "    scribere.s32 %p, 1075838976\n" \
    "    %d = legere.f64 %l\n" \
    "    %s = legere.f32 %p\n" \
    "    %e = dilatare.f32.f64 %s\n" \
    "    %f = fluitare.s32.f64 %a\n" \
    "    %g = multiplicare.f64 %f, %d\n" \
    "    %g = addere.f64 %g, %e\n" \
    "    %r = integrare.f64.s32 %g\n" \
    "    redde %r\n" \
    "\n" \
    "functio $nan (s32 %x) -> s32\n" \
    "@initium:\n" \
    "    %z = movere.f64 0.0\n" \
    "    %n = dividere.f64 %z, %z\n" \
    "    %u = fluitare.s32.f64 %x\n" \
    "    %s = artare.f64.f32 %n\n" \
    "    %v = artare.f64.f32 %u\n" \
    "    %m = movere.s32 0\n" \
    "    %c = aequalis.f64 %n, %u\n" \
    "    %m = vel.s32 %m, %c\n" \
    "    %c = inaequalis.f64 %n, %u\n" \
    "    %c = sinistrorsum.s32 %c, 1\n" \
    "    %m = vel.s32 %m, %c\n" \
    "    %c = minor.f64 %n, %u\n" \
    "    %c = sinistrorsum.s32 %c, 2\n" \
    "    %m = vel.s32 %m, %c\n" \
    "    %c = minor_aequalis.f64 %n, %u\n" \
    "    %c = sinistrorsum.s32 %c, 3\n" \
    "    %m = vel.s32 %m, %c\n" \
    "    %c = maior.f64 %n, %u\n" \
    "    %c = sinistrorsum.s32 %c, 4\n" \
    "    %m = vel.s32 %m, %c\n" \
    "    %c = maior_aequalis.f64 %n, %u\n" \
    "    %c = sinistrorsum.s32 %c, 5\n" \
    "    %m = vel.s32 %m, %c\n" \
    "    %c = aequalis.f64 %u, %n\n" \
    "    %c = sinistrorsum.s32 %c, 6\n" \
    "    %m = vel.s32 %m, %c\n" \
    "    %c = inaequalis.f64 %u, %n\n" \
    "    %c = sinistrorsum.s32 %c, 7\n" \
    "    %m = vel.s32 %m, %c\n" \
    "    %c = minor.f64 %u, %n\n" \
    "    %c = sinistrorsum.s32 %c, 8\n" \
    "    %m = vel.s32 %m, %c\n" \
    "    %c = minor_aequalis.f64 %u, %n\n" \
    "    %c = sinistrorsum.s32 %c, 9\n" \
    "    %m = vel.s32 %m, %c\n" \
    "    %c = maior.f64 %u, %n\n" \
    "    %c = sinistrorsum.s32 %c, 10\n" \
    "    %m = vel.s32 %m, %c\n" \
    "    %c = maior_aequalis.f64 %u, %n\n" \
    "    %c = sinistrorsum.s32 %c, 11\n" \
    "    %m = vel.s32 %m, %c\n" \
    "    %c = aequalis.f64 %u, %u\n" \
    "    %c = sinistrorsum.s32 %c, 12\n" \
    "    %m = vel.s32 %m, %c\n" \
    "    %c = inaequalis.f64 %u, %u\n" \
    "    %c = sinistrorsum.s32 %c, 13\n" \
    "    %m = vel.s32 %m, %c\n" \
    "    %c = minor.f64 %u, %u\n" \
    "    %c = sinistrorsum.s32 %c, 14\n" \
    "    %m = vel.s32 %m, %c\n" \
    "    %c = minor_aequalis.f64 %u, %u\n" \
    "    %c = sinistrorsum.s32 %c, 15\n" \
    "    %m = vel.s32 %m, %c\n" \
    "    %c = maior.f64 %u, %u\n" \
    "    %c = sinistrorsum.s32 %c, 16\n" \
    "    %m = vel.s32 %m, %c\n" \
    "    %c = maior_aequalis.f64 %u, %u\n" \
    "    %c = sinistrorsum.s32 %c, 17\n" \
    "    %m = vel.s32 %m, %c\n" \
    "    %c = aequalis.f32 %s, %v\n" \
    "    %c = sinistrorsum.s32 %c, 18\n" \
    "    %m = vel.s32 %m, %c\n" \
    "    %c = inaequalis.f32 %s, %v\n" \
    "    %c = sinistrorsum.s32 %c, 19\n" \
    "    %m = vel.s32 %m, %c\n" \
    "    %c = minor.f32 %s, %v\n" \
    "    %c = sinistrorsum.s32 %c, 20\n" \
    "    %m = vel.s32 %m, %c\n" \
    "    %c = minor_aequalis.f32 %s, %v\n" \
    "    %c = sinistrorsum.s32 %c, 21\n" \
    "    %m = vel.s32 %m, %c\n" \
    "    %c = maior.f32 %s, %v\n" \
    "    %c = sinistrorsum.s32 %c, 22\n" \
    "    %m = vel.s32 %m, %c\n" \
    "    %c = maior_aequalis.f32 %s, %v\n" \
    "    %c = sinistrorsum.s32 %c, 23\n" \
    "    %m = vel.s32 %m, %c\n" \
    "    %c = aequalis.f32 %v, %v\n" \
    "    %c = sinistrorsum.s32 %c, 24\n" \
    "    %m = vel.s32 %m, %c\n" \
    "    %c = inaequalis.f32 %v, %v\n" \
    "    %c = sinistrorsum.s32 %c, 25\n" \
    "    %m = vel.s32 %m, %c\n" \
    "    %c = minor.f32 %v, %v\n" \
    "    %c = sinistrorsum.s32 %c, 26\n" \
    "    %m = vel.s32 %m, %c\n" \
    "    %c = minor_aequalis.f32 %v, %v\n" \
    "    %c = sinistrorsum.s32 %c, 27\n" \
    "    %m = vel.s32 %m, %c\n" \
    "    %c = maior.f32 %v, %v\n" \
    "    %c = sinistrorsum.s32 %c, 28\n" \
    "    %m = vel.s32 %m, %c\n" \
    "    %c = maior_aequalis.f32 %v, %v\n" \
    "    %c = sinistrorsum.s32 %c, 29\n" \
    "    %m = vel.s32 %m, %c\n" \
    "    redde %m\n" \
    "\n" \
    "functio $nan_rami (s32 %x) -> s32\n" \
    "@initium:\n" \
    "    %z = movere.f64 0.0\n" \
    "    %n = dividere.f64 %z, %z\n" \
    "    %u = fluitare.s32.f64 %x\n" \
    "    %m = movere.s32 0\n" \
    "    salire @p0\n" \
    "@p0:\n" \
    "    %c = aequalis.f64 %n, %u\n" \
    "    ramus %c -> @v0 | @p1\n" \
    "@v0:\n" \
    "    %m = vel.s32 %m, 1\n" \
    "    salire @p1\n" \
    "@p1:\n" \
    "    %c = inaequalis.f64 %n, %u\n" \
    "    ramus %c -> @v1 | @p2\n" \
    "@v1:\n" \
    "    %m = vel.s32 %m, 2\n" \
    "    salire @p2\n" \
    "@p2:\n" \
    "    %c = minor.f64 %n, %u\n" \
    "    ramus %c -> @v2 | @p3\n" \
    "@v2:\n" \
    "    %m = vel.s32 %m, 4\n" \
    "    salire @p3\n" \
    "@p3:\n" \
    "    %c = minor_aequalis.f64 %n, %u\n" \
    "    ramus %c -> @v3 | @p4\n" \
    "@v3:\n" \
    "    %m = vel.s32 %m, 8\n" \
    "    salire @p4\n" \
    "@p4:\n" \
    "    %c = maior.f64 %n, %u\n" \
    "    ramus %c -> @v4 | @p5\n" \
    "@v4:\n" \
    "    %m = vel.s32 %m, 16\n" \
    "    salire @p5\n" \
    "@p5:\n" \
    "    %c = maior_aequalis.f64 %n, %u\n" \
    "    ramus %c -> @v5 | @p6\n" \
    "@v5:\n" \
    "    %m = vel.s32 %m, 32\n" \
    "    salire @p6\n" \
    "@p6:\n" \
    "    %c = aequalis.f64 %u, %u\n" \
    "    ramus %c -> @v6 | @p7\n" \
    "@v6:\n" \
    "    %m = vel.s32 %m, 64\n" \
    "    salire @p7\n" \
    "@p7:\n" \
    "    %c = inaequalis.f64 %u, %u\n" \
    "    ramus %c -> @v7 | @p8\n" \
    "@v7:\n" \
    "    %m = vel.s32 %m, 128\n" \
    "    salire @p8\n" \
    "@p8:\n" \
    "    %c = minor.f64 %u, %u\n" \
    "    ramus %c -> @v8 | @p9\n" \
    "@v8:\n" \
    "    %m = vel.s32 %m, 256\n" \
    "    salire @p9\n" \
    "@p9:\n" \
    "    %c = minor_aequalis.f64 %u, %u\n" \
    "    ramus %c -> @v9 | @p10\n" \
    "@v9:\n" \
    "    %m = vel.s32 %m, 512\n" \
    "    salire @p10\n" \
    "@p10:\n" \
    "    %c = maior.f64 %u, %u\n" \
    "    ramus %c -> @v10 | @p11\n" \
    "@v10:\n" \
    "    %m = vel.s32 %m, 1024\n" \
    "    salire @p11\n" \
    "@p11:\n" \
    "    %c = maior_aequalis.f64 %u, %u\n" \
    "    ramus %c -> @v11 | @p12\n" \
    "@v11:\n" \
    "    %m = vel.s32 %m, 2048\n" \
    "    salire @p12\n" \
    "@p12:\n" \
    "    redde %m\n" \
    "\n" \
    "functio $zephyrus_negativus () -> s32\n" \
    "@initium:\n" \
    "    %l = arca 16, 8\n" \
    "    %d = movere.f64 -0.0\n" \
    "    scribere.f64 %l, %d\n" \
    "    %b = legere.s64 %l\n" \
    "    %s = minor.s64 %b, 0\n" \
    "    %c = inaequalis.f64 %d, 0.0\n" \
    "    ramus %c -> @verum | @falsum\n" \
    "@verum:\n" \
    "    redde 1\n" \
    "@falsum:\n" \
    "    %r = addere.s32 %s, 41\n" \
    "    redde %r\n" \
    "\n" \
    "functio $rotundatio () -> s64\n" \
    "@initium:\n" \
    "    %a = movere.s64 1152921573326323713\n" \
    "    %f = fluitare.s64.f32 %a\n" \
    "    %d = dilatare.f32.f64 %f\n" \
    "    %r = integrare.f64.s64 %d\n" \
    "    redde %r\n" \
    "\n" \
    "functio $conversiones () -> s64\n" \
    "@initium:\n" \
    "    %u = movere.i64 -9223372036854773760\n" \
    "    %f = fluitare.i64.f64 %u\n" \
    "    %r1 = integrare.f64.i64 %f\n" \
    "    %n = movere.s32 -5\n" \
    "    %g = fluitare.i32.f64 %n\n" \
    "    %r2 = integrare.f64.s64 %g\n" \
    "    %w = movere.s32 200\n" \
    "    %c = amputare.s32.s8 %w\n" \
    "    %h = fluitare.s8.f64 %c\n" \
    "    %r3 = integrare.f64.s64 %h\n" \
    "    %q = movere.f64 -2.75\n" \
    "    %r4 = integrare.f64.s32 %q\n" \
    "    %e4 = extendere.s32.s64 %r4\n" \
    "    %t = movere.f64 300.0\n" \
    "    %r5 = integrare.f64.i8 %t\n" \
    "    %x5 = addere.s32 %r5, 1000\n" \
    "    %e5 = extendere.s32.s64 %x5\n" \
    "    %o = movere.f64 0.10000000000000001\n" \
    "    %o32 = artare.f64.f32 %o\n" \
    "    %o64 = dilatare.f32.f64 %o32\n" \
    "    %o64 = multiplicare.f64 %o64, 1000000000.0\n" \
    "    %r6 = integrare.f64.s64 %o64\n" \
    "    %y = movere.s32 -7\n" \
    "    %k = fluitare.s32.f32 %y\n" \
    "    %r7 = integrare.f32.s64 %k\n" \
    "    %i8 = fluitare.i8.f64 300\n" \
    "    %r8 = integrare.f64.s64 %i8\n" \
    "    %j8 = fluitare.s8.f32 200\n" \
    "    %r9 = integrare.f32.s64 %j8\n" \
    "    %s = movere.s64 %r1\n" \
    "    %m = multiplicare.s64 %r8, 19\n" \
    "    %s = addere.s64 %s, %m\n" \
    "    %m = multiplicare.s64 %r9, 23\n" \
    "    %s = addere.s64 %s, %m\n" \
    "    %m = multiplicare.s64 %r2, 3\n" \
    "    %s = addere.s64 %s, %m\n" \
    "    %m = multiplicare.s64 %r3, 5\n" \
    "    %s = addere.s64 %s, %m\n" \
    "    %m = multiplicare.s64 %e4, 7\n" \
    "    %s = addere.s64 %s, %m\n" \
    "    %m = multiplicare.s64 %e5, 11\n" \
    "    %s = addere.s64 %s, %m\n" \
    "    %m = multiplicare.s64 %r6, 13\n" \
    "    %s = addere.s64 %s, %m\n" \
    "    %m = multiplicare.s64 %r7, 17\n" \
    "    %s = addere.s64 %s, %m\n" \
    "    redde %s\n" \
    "\n" \
    "functio $hf (s32 %a) -> s32\n" \
    "@initium:\n" \
    "    %f = fluitare.s32.f64 %a\n" \
    "    %g = multiplicare.f64 %f, 2.5\n" \
    "    %t = vocare.s32 $h, s32 %a\n" \
    "    %u = fluitare.s32.f64 %t\n" \
    "    %g = addere.f64 %g, %u\n" \
    "    %r = integrare.f64.s32 %g\n" \
    "    redde %r\n" \
    "\n" \
    "functio $ante (s32 %a) -> s32\n" \
    "@initium:\n" \
    "    ramus %a -> @pone | @finis\n" \
    "@pone:\n" \
    "    %f = movere.f64 2.5\n" \
    "    salire @finis\n" \
    "@finis:\n" \
    "    %g = addere.f64 %f, 1.0\n" \
    "    %r = integrare.f64.s32 %g\n" \
    "    redde %r\n" \
    "\n" \
    "functio $ante_vocans (s32 %a) -> s32\n" \
    "@initium:\n" \
    "    %x = fluitare.s32.f64 %a\n" \
    "    %y = addere.f64 %x, 7.5\n" \
    "    %z = multiplicare.f64 %y, %x\n" \
    "    %i = integrare.f64.s32 %z\n" \
    "    %t = vocare.s32 $ante, s32 0\n" \
    "    %r = addere.s32 %t, %i\n" \
    "    redde %r\n" \
    "\n" \
    "functio $m_ante () -> s32\n" \
    "@initium:\n" \
    "    %t = vocare.s32 $ante_vocans, s32 3\n" \
    "    %u = vocare.s32 $ante, s32 1\n" \
    "    %v = multiplicare.s32 %u, 10\n" \
    "    %w = addere.s32 %t, %v\n" \
    "    redde %w\n" \
    "\n" \
    "functio $integrum_latum (s32 %a) -> s64\n" \
    "@initium:\n" \
    "    %f = fluitare.s32.f64 %a\n" \
    "    %r = integrare.f64.s32 %f\n" \
    "    %x = addere.s64 %r, 1\n" \
    "    redde %x\n" \
    "\n" \
    "functio $h (s32 %a) -> s32\n" \
    "@initium:\n" \
    "    %b = addere.s32 %a, 1\n" \
    "    redde %b\n" \
    "\n" \
    "functio $multa_f (s32 %a) -> s32\n" \
    "@initium:\n" \
    "    %f = fluitare.s32.f64 %a\n" \
    "    %f1 = addere.f64 %f, 1.5\n" \
    "    %f2 = addere.f64 %f, 2.5\n" \
    "    %f3 = addere.f64 %f, 3.5\n" \
    "    %f4 = addere.f64 %f, 4.5\n" \
    "    %f5 = addere.f64 %f, 5.5\n" \
    "    %f6 = addere.f64 %f, 6.5\n" \
    "    %f7 = addere.f64 %f, 7.5\n" \
    "    %f8 = addere.f64 %f, 8.5\n" \
    "    %f9 = addere.f64 %f, 9.5\n" \
    "    %f10 = addere.f64 %f, 10.5\n" \
    "    %f11 = addere.f64 %f, 11.5\n" \
    "    %f12 = addere.f64 %f, 12.5\n" \
    "    %f13 = addere.f64 %f, 13.5\n" \
    "    %f14 = addere.f64 %f, 14.5\n" \
    "    %f15 = addere.f64 %f, 15.5\n" \
    "    %f16 = addere.f64 %f, 16.5\n" \
    "    %f17 = addere.f64 %f, 17.5\n" \
    "    %f18 = addere.f64 %f, 18.5\n" \
    "    %f19 = addere.f64 %f, 19.5\n" \
    "    %f20 = addere.f64 %f, 20.5\n" \
    "    %t = vocare.s32 $hf, s32 %a\n" \
    "    %s = fluitare.s32.f64 %t\n" \
    "    %w = multiplicare.f64 %f1, 1.0\n" \
    "    %s = addere.f64 %s, %w\n" \
    "    %w = multiplicare.f64 %f2, 2.0\n" \
    "    %s = addere.f64 %s, %w\n" \
    "    %w = multiplicare.f64 %f3, 3.0\n" \
    "    %s = addere.f64 %s, %w\n" \
    "    %w = multiplicare.f64 %f4, 4.0\n" \
    "    %s = addere.f64 %s, %w\n" \
    "    %w = multiplicare.f64 %f5, 5.0\n" \
    "    %s = addere.f64 %s, %w\n" \
    "    %w = multiplicare.f64 %f6, 6.0\n" \
    "    %s = addere.f64 %s, %w\n" \
    "    %w = multiplicare.f64 %f7, 7.0\n" \
    "    %s = addere.f64 %s, %w\n" \
    "    %w = multiplicare.f64 %f8, 8.0\n" \
    "    %s = addere.f64 %s, %w\n" \
    "    %w = multiplicare.f64 %f9, 9.0\n" \
    "    %s = addere.f64 %s, %w\n" \
    "    %w = multiplicare.f64 %f10, 10.0\n" \
    "    %s = addere.f64 %s, %w\n" \
    "    %w = multiplicare.f64 %f11, 11.0\n" \
    "    %s = addere.f64 %s, %w\n" \
    "    %w = multiplicare.f64 %f12, 12.0\n" \
    "    %s = addere.f64 %s, %w\n" \
    "    %w = multiplicare.f64 %f13, 13.0\n" \
    "    %s = addere.f64 %s, %w\n" \
    "    %w = multiplicare.f64 %f14, 14.0\n" \
    "    %s = addere.f64 %s, %w\n" \
    "    %w = multiplicare.f64 %f15, 15.0\n" \
    "    %s = addere.f64 %s, %w\n" \
    "    %w = multiplicare.f64 %f16, 16.0\n" \
    "    %s = addere.f64 %s, %w\n" \
    "    %w = multiplicare.f64 %f17, 17.0\n" \
    "    %s = addere.f64 %s, %w\n" \
    "    %w = multiplicare.f64 %f18, 18.0\n" \
    "    %s = addere.f64 %s, %w\n" \
    "    %w = multiplicare.f64 %f19, 19.0\n" \
    "    %s = addere.f64 %s, %w\n" \
    "    %w = multiplicare.f64 %f20, 20.0\n" \
    "    %s = addere.f64 %s, %w\n" \
    "    %r = integrare.f64.s32 %s\n" \
    "    redde %r\n" \
    "\n" \
    "functio $multa_t (s32 %a) -> s32\n" \
    "@initium:\n" \
    "    %f = fluitare.s32.f32 %a\n" \
    "    %f1 = addere.f32 %f, 1.25\n" \
    "    %f2 = addere.f32 %f, 2.25\n" \
    "    %f3 = addere.f32 %f, 3.25\n" \
    "    %f4 = addere.f32 %f, 4.25\n" \
    "    %f5 = addere.f32 %f, 5.25\n" \
    "    %f6 = addere.f32 %f, 6.25\n" \
    "    %f7 = addere.f32 %f, 7.25\n" \
    "    %f8 = addere.f32 %f, 8.25\n" \
    "    %f9 = addere.f32 %f, 9.25\n" \
    "    %f10 = addere.f32 %f, 10.25\n" \
    "    %f11 = addere.f32 %f, 11.25\n" \
    "    %f12 = addere.f32 %f, 12.25\n" \
    "    %f13 = addere.f32 %f, 13.25\n" \
    "    %f14 = addere.f32 %f, 14.25\n" \
    "    %f15 = addere.f32 %f, 15.25\n" \
    "    %f16 = addere.f32 %f, 16.25\n" \
    "    %f17 = addere.f32 %f, 17.25\n" \
    "    %f18 = addere.f32 %f, 18.25\n" \
    "    %f19 = addere.f32 %f, 19.25\n" \
    "    %f20 = addere.f32 %f, 20.25\n" \
    "    %s = movere.f32 0.0\n" \
    "    %w = multiplicare.f32 %f1, 1.0\n" \
    "    %s = subtrahere.f32 %s, %w\n" \
    "    %w = multiplicare.f32 %f2, 2.0\n" \
    "    %s = subtrahere.f32 %s, %w\n" \
    "    %w = multiplicare.f32 %f3, 3.0\n" \
    "    %s = subtrahere.f32 %s, %w\n" \
    "    %w = multiplicare.f32 %f4, 4.0\n" \
    "    %s = subtrahere.f32 %s, %w\n" \
    "    %w = multiplicare.f32 %f5, 5.0\n" \
    "    %s = subtrahere.f32 %s, %w\n" \
    "    %w = multiplicare.f32 %f6, 6.0\n" \
    "    %s = subtrahere.f32 %s, %w\n" \
    "    %w = multiplicare.f32 %f7, 7.0\n" \
    "    %s = subtrahere.f32 %s, %w\n" \
    "    %w = multiplicare.f32 %f8, 8.0\n" \
    "    %s = subtrahere.f32 %s, %w\n" \
    "    %w = multiplicare.f32 %f9, 9.0\n" \
    "    %s = subtrahere.f32 %s, %w\n" \
    "    %w = multiplicare.f32 %f10, 10.0\n" \
    "    %s = subtrahere.f32 %s, %w\n" \
    "    %w = multiplicare.f32 %f11, 11.0\n" \
    "    %s = subtrahere.f32 %s, %w\n" \
    "    %w = multiplicare.f32 %f12, 12.0\n" \
    "    %s = subtrahere.f32 %s, %w\n" \
    "    %w = multiplicare.f32 %f13, 13.0\n" \
    "    %s = subtrahere.f32 %s, %w\n" \
    "    %w = multiplicare.f32 %f14, 14.0\n" \
    "    %s = subtrahere.f32 %s, %w\n" \
    "    %w = multiplicare.f32 %f15, 15.0\n" \
    "    %s = subtrahere.f32 %s, %w\n" \
    "    %w = multiplicare.f32 %f16, 16.0\n" \
    "    %s = subtrahere.f32 %s, %w\n" \
    "    %w = multiplicare.f32 %f17, 17.0\n" \
    "    %s = subtrahere.f32 %s, %w\n" \
    "    %w = multiplicare.f32 %f18, 18.0\n" \
    "    %s = subtrahere.f32 %s, %w\n" \
    "    %w = multiplicare.f32 %f19, 19.0\n" \
    "    %s = subtrahere.f32 %s, %w\n" \
    "    %w = multiplicare.f32 %f20, 20.0\n" \
    "    %s = subtrahere.f32 %s, %w\n" \
    "    %r = integrare.f32.s32 %s\n" \
    "    redde %r\n" \
    "\n" \
    "functio $m_arith () -> s32\n" \
    "@initium:\n" \
    "    %t = vocare.s32 $arith, s32 7\n" \
    "    redde %t\n" \
    "\n" \
    "functio $m_arith32 () -> s32\n" \
    "@initium:\n" \
    "    %t = vocare.s32 $arith32, s32 7\n" \
    "    redde %t\n" \
    "\n" \
    "functio $m_bita () -> s64\n" \
    "@initium:\n" \
    "    %t = vocare.s64 $bita, s32 10\n" \
    "    redde %t\n" \
    "\n" \
    "functio $m_bita32 () -> s64\n" \
    "@initium:\n" \
    "    %t = vocare.s64 $bita32, s32 3\n" \
    "    redde %t\n" \
    "\n" \
    "functio $m_lege () -> s32\n" \
    "@initium:\n" \
    "    %t = vocare.s32 $lege, s32 -9\n" \
    "    redde %t\n" \
    "\n" \
    "functio $m_nan () -> s32\n" \
    "@initium:\n" \
    "    %t = vocare.s32 $nan, s32 3\n" \
    "    redde %t\n" \
    "\n" \
    "functio $m_nan_rami () -> s32\n" \
    "@initium:\n" \
    "    %t = vocare.s32 $nan_rami, s32 3\n" \
    "    redde %t\n" \
    "\n" \
    "functio $m_zephyrus () -> s32\n" \
    "@initium:\n" \
    "    %t = vocare.s32 $zephyrus_negativus\n" \
    "    redde %t\n" \
    "\n" \
    "functio $m_rotundatio () -> s64\n" \
    "@initium:\n" \
    "    %t = vocare.s64 $rotundatio\n" \
    "    redde %t\n" \
    "\n" \
    "functio $m_conversiones () -> s64\n" \
    "@initium:\n" \
    "    %t = vocare.s64 $conversiones\n" \
    "    redde %t\n" \
    "\n" \
    "functio $m_multa_f () -> s32\n" \
    "@initium:\n" \
    "    %t = vocare.s32 $multa_f, s32 3\n" \
    "    redde %t\n" \
    "\n" \
    "functio $m_multa_t () -> s32\n" \
    "@initium:\n" \
    "    %t = vocare.s32 $multa_t, s32 3\n" \
    "    redde %t\n" \
    "\n" \
    "functio $classis_mixta (s32 %a) -> s32\n" \
    "@initium:\n" \
    "    %r = movere.s32 1\n" \
    "    %r = movere.f64 1.0\n" \
    "    redde %a\n" \
    "\n" \
    "functio $usus_mixtus (s32 %a) -> s32\n" \
    "@initium:\n" \
    "    %f = movere.f64 1.0\n" \
    "    %x = addere.s32 %f, 1\n" \
    "    redde %x\n" \
    "\n" \
    "functio $usus_integri (s32 %a) -> s32\n" \
    "@initium:\n" \
    "    %f = addere.f64 %a, 1.0\n" \
    "    %x = integrare.f64.s32 %f\n" \
    "    redde %x\n"

interior vacuum
_sectio_dorsum_fluitantia (
    CredoContextus* c)
{
    constans character* cursus[]   = {
        "m_arith", "m_arith32", "m_bita", "m_bita32", "m_lege",
        "m_zephyrus", "m_rotundatio", "m_conversiones", "m_multa_f",
        "m_multa_t", "m_ante"
    };
    constans character* nativae[]  = {
        "arith", "arith32", "bita", "bita32", "lege",
        "zephyrus_negativus", "rotundatio", "conversiones", "multa_f",
        "multa_t", "ante_vocans"
    };
       Machinula* m;
    StatusCursus  s;
          chorda  causa;
             f64  tertia = 10.0 / 3.0;
             f32  angusta = (f32)3 * 0.2f + 0.1f;
             i64  bita;
             i32  bita_angusta;

    CREDO_NECESSE (_parare(c->piscina));
    m = _machinulam_parare(c->piscina, TEXTUS_FLUITANTIUM);
    CREDO_NECESSE_NON_NIHIL (m);
    /* lex (interpretata, contra C hospitis) */
    s = _statum_capere(c->piscina, m, "m_arith");
    CREDO_AEQUALIS_S32 (s.genus, MACHINULA_BENE);
    CREDO_VERUM (s.codex == (s64)(s32)(-((7.0 * 1.5 + 0.1
        - 3.1415926535897931) / -0.75) * 1000.0));
    memcpy(&bita, &tertia, magnitudo(bita));
    s = _statum_capere(c->piscina, m, "m_bita");
    CREDO_VERUM ((i64)s.codex == bita);
    /* f32 in dimidio inferiore; dimidium superius arcae non tangitur */
    memcpy(&bita_angusta, &angusta, magnitudo(bita_angusta));
    s = _statum_capere(c->piscina, m, "m_bita32");
    CREDO_VERUM ((i64)s.codex == (((i64)0xFFFFFFFFUL << XXXII)
        | (i64)bita_angusta));
    /* -9 * 2.0 + 2.5 = -15.5 -> -15 */
    s = _statum_capere(c->piscina, m, "m_lege");
    CREDO_VERUM (s.codex == -(s64)XV);
    /* -0.0: != 0.0 falsum (ramus @falsum), signum in bitis servatum */
    s = _statum_capere(c->piscina, m, "m_zephyrus");
    CREDO_VERUM (s.codex == (s64)XLII);
    /* s64 -> f32 rotundatione UNA (lex C): 2^60 + 2^37 */
    s = _statum_capere(c->piscina, m, "m_rotundatio");
    CREDO_VERUM (s.codex == (s64)1152921642045800448L);
    /* registrum fluitans ante definitionem lectum: zephyrus (F13),
     * etiam post vocantem qui temporaria inquinavit - (1 + 31),
     * deinde 3 -> 62 */
    s = _statum_capere(c->piscina, m, "m_ante");
    CREDO_VERUM (s.codex == (s64)LXII);
    _cursus_conferre(c, m, cursus, XI, nativae, XI);
    CREDO_VERUM (_signum_functionis("hf")
        == (s64)CONEXIO_SIGNUM_NATIVUM);
    CREDO_VERUM (_signum_functionis("multa_t")
        == (s64)CONEXIO_SIGNUM_NATIVUM);

    /* fructus integrare s32 verbum canonicum non fert (census E17) */
    CREDO_FALSUM (machinula_nativum_compilare(m, _ch("integrum_latum"),
        &causa));
    CREDO_VERUM (chorda_aequalis_literis(causa,
        "registrum s32 in usu 64"));
    /* registrum medullae classem UNAM habet */
    CREDO_FALSUM (machinula_nativum_compilare(m, _ch("classis_mixta"),
        &causa));
    CREDO_VERUM (chorda_aequalis_literis(causa,
        "dorsum: registrum classis mixtae"));
    CREDO_FALSUM (machinula_nativum_compilare(m, _ch("usus_mixtus"),
        &causa));
    CREDO_VERUM (chorda_aequalis_literis(causa,
        "dorsum: registrum fluitans in usu integro"));
    CREDO_FALSUM (machinula_nativum_compilare(m, _ch("usus_integri"),
        &causa));
    CREDO_VERUM (chorda_aequalis_literis(causa,
        "dorsum: registrum integrum in usu fluitante"));
}

interior vacuum
_sectio_dorsum_non_numeri (
    CredoContextus* c)
{
    constans character* cursus[]   = { "m_nan", "m_nan_rami" };
    constans character* nativae[]  = { "nan", "nan_rami" };
       Machinula* m;
    StatusCursus  s;

    CREDO_NECESSE (_parare(c->piscina));
    m = _machinulam_parare(c->piscina, TEXTUS_FLUITANTIUM);
    CREDO_NECESSE_NON_NIHIL (m);
    /* tabula C: cum NaN sola != vera; ordo bitorum ==, !=, <, <=, >,
     * >= per paria (n,u) (u,n) (u,u) f64, deinde (n,u) (u,u) f32 */
    s = _statum_capere(c->piscina, m, "m_nan");
    CREDO_AEQUALIS_S32 (s.genus, MACHINULA_BENE);
    CREDO_VERUM (s.codex == (s64)(II | (II << VI) | (XLI << XII)
        | (II << XVIII) | (XLI << XXIV)));
    /* per ramos: (n,u) deinde (u,u) */
    s = _statum_capere(c->piscina, m, "m_nan_rami");
    CREDO_VERUM (s.codex == (s64)(II | (XLI << VI)));
    _cursus_conferre(c, m, cursus, II, nativae, II);
    CREDO_VERUM (_signum_functionis("nan_rami")
        == (s64)CONEXIO_SIGNUM_NATIVUM);
}

interior vacuum
_sectio_dorsum_fixturae_conversionum (
    CredoContextus* c)
{
    _fixturam_conferre(c,
        "officina/probationes/fixa/machinula/conversiones.medulla",
        (s64)XLII);
}


    /* ==================================================
     * DORSUM T9 gradus II: argumenta et reditus fluitantes (d0-d7,
     * ultra octavum in stiva magnitudine naturali), aedificata pura
     * fluitantia directe, fluitantia per pontem; clang ut oraculum
     * ================================================== */

#define TEXTUS_LIMINUM_FLUITANTIUM \
    "modulus \"probatio/limina_f.medulla\"\n" \
    "externum $sqrt (f64) -> f64\n" \
    "externum $pow (f64, f64) -> f64\n" \
    "externum $fabs (f64) -> f64\n" \
    "externum $floor (f64) -> f64\n" \
    "externum $ldexp (f64, s32) -> f64\n" \
    "externum $atof (i64) -> f64\n" \
    "externum $strtod (i64, i64) -> f64\n" \
    "externum $printf (i64, ...) -> s32\n" \
    "\n" \
    "functio $bita (f64 %v) -> s64\n" \
    "@initium:\n" \
    "    %l = arca 8, 8\n" \
    "    scribere.f64 %l, %v\n" \
    "    %b = legere.s64 %l\n" \
    "    redde %b\n" \
    "\n" \
    "functio $media (f64 %a, f64 %b) -> f64\n" \
    "@initium:\n" \
    "    %t = multiplicare.f64 %b, 3.0\n" \
    "    %s = addere.f64 %a, %t\n" \
    "    %m = dividere.f64 %s, 2.0\n" \
    "    redde %m\n" \
    "\n" \
    "functio $media32 (f32 %a, f32 %b) -> f32\n" \
    "@initium:\n" \
    "    %t = multiplicare.f32 %b, 3.0\n" \
    "    %s = addere.f32 %a, %t\n" \
    "    %m = dividere.f32 %s, 2.0\n" \
    "    redde %m\n" \
    "\n" \
    "functio $mixta (s32 %i, f64 %a, s64 %j, f32 %b, s32 %k) -> " \
    "f64\n" \
    "@initium:\n" \
    "    %fi = fluitare.s32.f64 %i\n" \
    "    %fj = fluitare.s64.f64 %j\n" \
    "    %fk = fluitare.s32.f64 %k\n" \
    "    %fb = dilatare.f32.f64 %b\n" \
    "    %s = multiplicare.f64 %a, 2.0\n" \
    "    %s = addere.f64 %s, %fi\n" \
    "    %t = multiplicare.f64 %fj, 3.0\n" \
    "    %s = addere.f64 %s, %t\n" \
    "    %t = multiplicare.f64 %fb, 5.0\n" \
    "    %s = addere.f64 %s, %t\n" \
    "    %t = multiplicare.f64 %fk, 7.0\n" \
    "    %s = addere.f64 %s, %t\n" \
    "    redde %s\n" \
    "\n" \
    "functio $decem (f64 %a1, f64 %a2, f64 %a3, f64 %a4, f64 " \
    "%a5, f64 %a6, f64 %a7, f64 %a8, f64 %a9, f64 %a10, f32 %b1, " \
    "f32 %b2, f64 %c) -> f64\n" \
    "@initium:\n" \
    "    %s = movere.f64 %a1\n" \
    "    %t = multiplicare.f64 %a2, 2.0\n" \
    "    %s = addere.f64 %s, %t\n" \
    "    %t = multiplicare.f64 %a3, 3.0\n" \
    "    %s = addere.f64 %s, %t\n" \
    "    %t = multiplicare.f64 %a4, 4.0\n" \
    "    %s = addere.f64 %s, %t\n" \
    "    %t = multiplicare.f64 %a5, 5.0\n" \
    "    %s = addere.f64 %s, %t\n" \
    "    %t = multiplicare.f64 %a6, 6.0\n" \
    "    %s = addere.f64 %s, %t\n" \
    "    %t = multiplicare.f64 %a7, 7.0\n" \
    "    %s = addere.f64 %s, %t\n" \
    "    %t = multiplicare.f64 %a8, 8.0\n" \
    "    %s = addere.f64 %s, %t\n" \
    "    %t = multiplicare.f64 %a9, 9.0\n" \
    "    %s = addere.f64 %s, %t\n" \
    "    %t = multiplicare.f64 %a10, 10.0\n" \
    "    %s = addere.f64 %s, %t\n" \
    "    %e = dilatare.f32.f64 %b1\n" \
    "    %t = multiplicare.f64 %e, 11.0\n" \
    "    %s = addere.f64 %s, %t\n" \
    "    %e = dilatare.f32.f64 %b2\n" \
    "    %t = multiplicare.f64 %e, 13.0\n" \
    "    %s = addere.f64 %s, %t\n" \
    "    %t = multiplicare.f64 %c, 17.0\n" \
    "    %s = addere.f64 %s, %t\n" \
    "    redde %s\n" \
    "\n" \
    "functio $plena (s64 %x1, s64 %x2, s64 %x3, s64 %x4, s64 " \
    "%x5, s64 %x6, s64 %x7, s64 %x8, s64 %x9, f64 %d1, f64 %d2, " \
    "f64 %d3, f64 %d4, f64 %d5, f64 %d6, f64 %d7, f64 %d8, f64 " \
    "%d9, s8 %c, f32 %e) -> f64\n" \
    "@initium:\n" \
    "    %s = fluitare.s64.f64 %x1\n" \
    "    %f = fluitare.s64.f64 %x9\n" \
    "    %t = multiplicare.f64 %f, 3.0\n" \
    "    %s = addere.f64 %s, %t\n" \
    "    %t = multiplicare.f64 %d1, 5.0\n" \
    "    %s = addere.f64 %s, %t\n" \
    "    %t = multiplicare.f64 %d8, 7.0\n" \
    "    %s = addere.f64 %s, %t\n" \
    "    %t = multiplicare.f64 %d9, 11.0\n" \
    "    %s = addere.f64 %s, %t\n" \
    "    %f = fluitare.s8.f64 %c\n" \
    "    %t = multiplicare.f64 %f, 13.0\n" \
    "    %s = addere.f64 %s, %t\n" \
    "    %f = dilatare.f32.f64 %e\n" \
    "    %t = multiplicare.f64 %f, 17.0\n" \
    "    %s = addere.f64 %s, %t\n" \
    "    redde %s\n" \
    "\n" \
    "functio $trans (f64 %a1, f64 %a2, f64 %a3, f64 %a4, f64 " \
    "%a5, f64 %a6, f64 %a7, f64 %a8, f64 %a9, f64 %a10) -> f64\n" \
    "@initium:\n" \
    "    %s = vocare.f64 $media, f64 %a1, f64 %a2\n" \
    "    %t = multiplicare.f64 %a1, 1.0\n" \
    "    %s = addere.f64 %s, %t\n" \
    "    %t = multiplicare.f64 %a2, 2.0\n" \
    "    %s = addere.f64 %s, %t\n" \
    "    %t = multiplicare.f64 %a3, 3.0\n" \
    "    %s = addere.f64 %s, %t\n" \
    "    %t = multiplicare.f64 %a4, 4.0\n" \
    "    %s = addere.f64 %s, %t\n" \
    "    %t = multiplicare.f64 %a5, 5.0\n" \
    "    %s = addere.f64 %s, %t\n" \
    "    %t = multiplicare.f64 %a6, 6.0\n" \
    "    %s = addere.f64 %s, %t\n" \
    "    %t = multiplicare.f64 %a7, 7.0\n" \
    "    %s = addere.f64 %s, %t\n" \
    "    %t = multiplicare.f64 %a8, 8.0\n" \
    "    %s = addere.f64 %s, %t\n" \
    "    %t = multiplicare.f64 %a9, 9.0\n" \
    "    %s = addere.f64 %s, %t\n" \
    "    %t = multiplicare.f64 %a10, 10.0\n" \
    "    %s = addere.f64 %s, %t\n" \
    "    redde %s\n" \
    "\n" \
    "functio $per_trans () -> f64\n" \
    "@initium:\n" \
    "    %r = vocare.f64 $trans, f64 1.5, f64 2.5, f64 3.5, f64 " \
    "4.5, f64 5.5, f64 6.5, f64 7.5, f64 8.5, f64 9.5, f64 10.5\n" \
    "    redde %r\n" \
    "\n" \
    "functio $m_trans () -> s64\n" \
    "@initium:\n" \
    "    %r = vocare.f64 $per_trans\n" \
    "    %b = vocare.s64 $bita, f64 %r\n" \
    "    redde %b\n" \
    "\n" \
    "functio $potentia (f64 %b, s32 %n) -> f64\n" \
    "@initium:\n" \
    "    %z = aequalis.s32 %n, 0\n" \
    "    ramus %z -> @unum | @gradus\n" \
    "@unum:\n" \
    "    %u = movere.f64 1.0\n" \
    "    redde %u\n" \
    "@gradus:\n" \
    "    %m = subtrahere.s32 %n, 1\n" \
    "    %p = vocare.f64 $potentia, f64 %b, s32 %m\n" \
    "    %r = multiplicare.f64 %b, %p\n" \
    "    redde %r\n" \
    "\n" \
    "functio $h_decem (f64 %a1, f64 %a2, f64 %a3, f64 %a4, f64 " \
    "%a5, f64 %a6, f64 %a7, f64 %a8, f64 %a9, f64 %a10, f32 %b1, " \
    "f32 %b2, f64 %c) -> f64\n" \
    "@initium:\n" \
    "    %z = movere.f64 0.0\n" \
    "    redde %z\n" \
    "\n" \
    "functio $h_plena (s64 %x1, s64 %x2, s64 %x3, s64 %x4, s64 " \
    "%x5, s64 %x6, s64 %x7, s64 %x8, s64 %x9, f64 %d1, f64 %d2, " \
    "f64 %d3, f64 %d4, f64 %d5, f64 %d6, f64 %d7, f64 %d8, f64 " \
    "%d9, s8 %c, f32 %e) -> f64\n" \
    "@initium:\n" \
    "    %z = movere.f64 0.0\n" \
    "    redde %z\n" \
    "\n" \
    "functio $h_media32 (f32 %a, f32 %b) -> f32\n" \
    "@initium:\n" \
    "    %z = movere.f32 0.0\n" \
    "    redde %z\n" \
    "\n" \
    "functio $per_media (s32 %n) -> f64\n" \
    "@initium:\n" \
    "    %x = fluitare.s32.f64 %n\n" \
    "    %y = vocare.f64 $media, f64 %x, f64 0.25\n" \
    "    %z = addere.f64 %x, %y\n" \
    "    %w = vocare.f64 $media, f64 %z, f64 %y\n" \
    "    %r = subtrahere.f64 %w, %x\n" \
    "    redde %r\n" \
    "\n" \
    "functio $per_media32 (s32 %n) -> f32\n" \
    "@initium:\n" \
    "    %x = fluitare.s32.f32 %n\n" \
    "    %y = vocare.f32 $media32, f32 %x, f32 " \
    "0.10000000000000001\n" \
    "    %z = addere.f32 %x, %y\n" \
    "    redde %z\n" \
    "\n" \
    "functio $per_h_media32 (i64 %f, s32 %n) -> f32\n" \
    "@initium:\n" \
    "    %x = fluitare.s32.f32 %n\n" \
    "    %y = vocare.f32 %f, f32 %x, f32 " \
    "0.10000000000000001\n" \
    "    %z = addere.f32 %x, %y\n" \
    "    redde %z\n" \
    "\n" \
    "functio $per_mixta () -> f64\n" \
    "@initium:\n" \
    "    %x = movere.f64 1.5\n" \
    "    %j = movere.s64 1000000000000\n" \
    "    %b = movere.f32 0.75\n" \
    "    %r = vocare.f64 $mixta, s32 -4, f64 %x, s64 %j, f32 %b, " \
    "s32 9\n" \
    "    redde %r\n" \
    "\n" \
    "functio $per_decem () -> f64\n" \
    "@initium:\n" \
    "    %x = movere.f64 2.5\n" \
    "    %y = movere.f64 9.5\n" \
    "    %z = movere.f32 1.25\n" \
    "    %c = movere.f64 100.125\n" \
    "    %r = vocare.f64 $decem, f64 1.5, f64 %x, f64 3.5, f64 " \
    "4.5, f64 5.5, f64 6.5, f64 7.5, f64 8.5, f64 %y, f64 10.5, " \
    "f32 %z, f32 -2.75, f64 %c\n" \
    "    redde %r\n" \
    "\n" \
    "functio $per_h_decem (i64 %f) -> f64\n" \
    "@initium:\n" \
    "    %x = movere.f64 2.5\n" \
    "    %y = movere.f64 9.5\n" \
    "    %z = movere.f32 1.25\n" \
    "    %c = movere.f64 100.125\n" \
    "    %r = vocare.f64 %f, f64 1.5, f64 %x, f64 3.5, f64 " \
    "4.5, f64 5.5, f64 6.5, f64 7.5, f64 8.5, f64 %y, f64 10.5, " \
    "f32 %z, f32 -2.75, f64 %c\n" \
    "    redde %r\n" \
    "\n" \
    "functio $per_plena () -> f64\n" \
    "@initium:\n" \
    "    %x = movere.s64 -9\n" \
    "    %d = movere.f64 -9.5\n" \
    "    %e = movere.f32 0.375\n" \
    "    %r = vocare.f64 $plena, s64 5, s64 2, s64 3, s64 4, s64 " \
    "5, s64 6, s64 7, s64 8, s64 %x, f64 1.5, f64 2.5, f64 3.5, " \
    "f64 4.5, f64 5.5, f64 6.5, f64 7.5, f64 8.5, f64 %d, s8 -3, " \
    "f32 %e\n" \
    "    redde %r\n" \
    "\n" \
    "functio $per_h_plena (i64 %f) -> f64\n" \
    "@initium:\n" \
    "    %x = movere.s64 -9\n" \
    "    %d = movere.f64 -9.5\n" \
    "    %e = movere.f32 0.375\n" \
    "    %r = vocare.f64 %f, s64 5, s64 2, s64 3, s64 4, " \
    "s64 5, s64 6, s64 7, s64 8, s64 %x, f64 1.5, f64 2.5, f64 " \
    "3.5, f64 4.5, f64 5.5, f64 6.5, f64 7.5, f64 8.5, f64 %d, " \
    "s8 -3, f32 %e\n" \
    "    redde %r\n" \
    "\n" \
    "functio $per_indirecta () -> f64\n" \
    "@initium:\n" \
    "    %f = locus $media\n" \
    "    %r = vocare.f64 %f, f64 2.0, f64 4.5\n" \
    "    redde %r\n" \
    "\n" \
    "functio $pura (f64 %a) -> f64\n" \
    "@initium:\n" \
    "    %s = vocare.f64 $sqrt, f64 %a\n" \
    "    %p = vocare.f64 $pow, f64 %a, f64 10.0\n" \
    "    %s = addere.f64 %s, %p\n" \
    "    %n = negare.f64 %a\n" \
    "    %p = vocare.f64 $fabs, f64 %n\n" \
    "    %s = addere.f64 %s, %p\n" \
    "    %h = subtrahere.f64 %n, 0.5\n" \
    "    %p = vocare.f64 $floor, f64 %h\n" \
    "    %s = addere.f64 %s, %p\n" \
    "    %p = vocare.f64 $ldexp, f64 0.75, s32 4\n" \
    "    %s = addere.f64 %s, %p\n" \
    "    %q = locus $numerus\n" \
    "    %p = vocare.f64 $atof, i64 %q\n" \
    "    %s = addere.f64 %s, %p\n" \
    "    %s = addere.f64 %s, %a\n" \
    "    redde %s\n" \
    "\n" \
    "functio $scriptor (s32 %a) -> s32\n" \
    "@initium:\n" \
    "    %x = fluitare.s32.f64 %a\n" \
    "    %h = dividere.f64 %x, 4.0\n" \
    "    %g = artare.f64.f32 %h\n" \
    "    %e = dilatare.f32.f64 %g\n" \
    "    %q = locus $numerus\n" \
    "    %d = vocare.f64 $strtod, i64 %q, i64 0\n" \
    "    %f = locus $forma\n" \
    "    %n = vocare.s32 $printf, i64 %f, f64 %h, f64 %d, s32 " \
    "%a, f64 %e\n" \
    "    %k = multiplicare.f64 %d, 4.0\n" \
    "    %i = integrare.f64.s32 %k\n" \
    "    %n = addere.s32 %n, %i\n" \
    "    redde %n\n" \
    "\n" \
    "functio $m_scriptor () -> s32\n" \
    "@initium:\n" \
    "    %t = vocare.s32 $scriptor, s32 7\n" \
    "    redde %t\n" \
    "\n" \
    "functio $m_media () -> s64\n" \
    "@initium:\n" \
    "    %r = vocare.f64 $per_media, s32 7\n" \
    "    %b = vocare.s64 $bita, f64 %r\n" \
    "    redde %b\n" \
    "\n" \
    "functio $m_media32 () -> s64\n" \
    "@initium:\n" \
    "    %r = vocare.f32 $per_media32, s32 7\n" \
    "    %d = dilatare.f32.f64 %r\n" \
    "    %b = vocare.s64 $bita, f64 %d\n" \
    "    redde %b\n" \
    "\n" \
    "functio $m_mixta () -> s64\n" \
    "@initium:\n" \
    "    %r = vocare.f64 $per_mixta\n" \
    "    %b = vocare.s64 $bita, f64 %r\n" \
    "    redde %b\n" \
    "\n" \
    "functio $m_decem () -> s64\n" \
    "@initium:\n" \
    "    %r = vocare.f64 $per_decem\n" \
    "    %b = vocare.s64 $bita, f64 %r\n" \
    "    redde %b\n" \
    "\n" \
    "functio $m_plena () -> s64\n" \
    "@initium:\n" \
    "    %r = vocare.f64 $per_plena\n" \
    "    %b = vocare.s64 $bita, f64 %r\n" \
    "    redde %b\n" \
    "\n" \
    "functio $m_potentia () -> s64\n" \
    "@initium:\n" \
    "    %r = vocare.f64 $potentia, f64 1.5, s32 10\n" \
    "    %b = vocare.s64 $bita, f64 %r\n" \
    "    redde %b\n" \
    "\n" \
    "functio $m_indirecta () -> s64\n" \
    "@initium:\n" \
    "    %r = vocare.f64 $per_indirecta\n" \
    "    %b = vocare.s64 $bita, f64 %r\n" \
    "    redde %b\n" \
    "\n" \
    "functio $m_pura () -> s64\n" \
    "@initium:\n" \
    "    %r = vocare.f64 $pura, f64 2.0\n" \
    "    %b = vocare.s64 $bita, f64 %r\n" \
    "    redde %b\n" \
    "\n" \
    "functio $m_h_decem () -> s64\n" \
    "@initium:\n" \
    "    %f = locus $h_decem\n" \
    "    %r = vocare.f64 $per_h_decem, i64 %f\n" \
    "    %b = vocare.s64 $bita, f64 %r\n" \
    "    redde %b\n" \
    "\n" \
    "functio $m_h_plena () -> s64\n" \
    "@initium:\n" \
    "    %f = locus $h_plena\n" \
    "    %r = vocare.f64 $per_h_plena, i64 %f\n" \
    "    %b = vocare.s64 $bita, f64 %r\n" \
    "    redde %b\n" \
    "\n" \
    "functio $m_h_media32 () -> s64\n" \
    "@initium:\n" \
    "    %f = locus $h_media32\n" \
    "    %r = vocare.f32 $per_h_media32, i64 %f, s32 7\n" \
    "    %d = dilatare.f32.f64 %r\n" \
    "    %b = vocare.s64 $bita, f64 %d\n" \
    "    redde %b\n" \
    "\n" \
    "datum $numerus magnitudo 4 ordinatio 1\n" \
    "    octeti 322e3500\n" \
    "\n" \
    "datum $forma magnitudo 20 ordinatio 1\n" \
    "    octeti 252e3266 20252e31 66202564 20252e33 660a0000\n"

interior f64
_hospes_decem (
    f64 a1,
    f64 a2,
    f64 a3,
    f64 a4,
    f64 a5,
    f64 a6,
    f64 a7,
    f64 a8,
    f64 a9,
    f64 a10,
    f32 b1,
    f32 b2,
    f64 c)
{
    redde a1 + a2 * 2.0 + a3 * 3.0 + a4 * 4.0 + a5 * 5.0 + a6 * 6.0
        + a7 * 7.0 + a8 * 8.0 + a9 * 9.0 + a10 * 10.0 + (f64)b1 * 11.0
        + (f64)b2 * 13.0 + c * 17.0;
}

interior f64
_hospes_plena (
                   s64 x1,
                   s64 x2,
                   s64 x3,
                   s64 x4,
                   s64 x5,
                   s64 x6,
                   s64 x7,
                   s64 x8,
                   s64 x9,
                   f64 d1,
                   f64 d2,
                   f64 d3,
                   f64 d4,
                   f64 d5,
                   f64 d6,
                   f64 d7,
                   f64 d8,
                   f64 d9,
    signatus character c,
                   f32 e)
{
    (vacuum)x2;
    (vacuum)x3;
    (vacuum)x4;
    (vacuum)x5;
    (vacuum)x6;
    (vacuum)x7;
    (vacuum)x8;
    (vacuum)d2;
    (vacuum)d3;
    (vacuum)d4;
    (vacuum)d5;
    (vacuum)d6;
    (vacuum)d7;
    redde (f64)x1 + (f64)x9 * 3.0 + d1 * 5.0 + d8 * 7.0 + d9 * 11.0
        + (f64)c * 13.0 + (f64)e * 17.0;
}

interior f32
_hospes_media_angusta (
    f32 a,
    f32 b)
{
    redde (a + b * 3.0f) / 2.0f;
}

interior i64
_bita_f64 (
    f64 valor)
{
    i64 bita;

    memcpy(&bita, &valor, magnitudo(bita));
    redde bita;
}

/* valores quos $per_decem et $per_plena tradunt, per legem C */
interior f64
_lex_decem (vacuum)
{
    redde _hospes_decem(1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.5, 8.5, 9.5,
        10.5, 1.25f, -2.75f, 100.125);
}

interior f64
_lex_plena (vacuum)
{
    redde _hospes_plena(V, II, III, IV, V, VI, VII, VIII, -(s64)IX, 1.5,
        2.5, 3.5, 4.5, 5.5, 6.5, 7.5, 8.5, -9.5,
        (signatus character)-III,
        0.375f);
}

interior vacuum
_sectio_dorsum_limina_fluitantia (
    CredoContextus* c)
{
    constans character* cursus[]   = {
        "m_media", "m_media32", "m_mixta", "m_decem", "m_plena",
        "m_potentia", "m_indirecta", "m_pura", "m_trans"
    };
    constans character* nativae[]  = {
        "bita", "per_media", "per_media32", "per_mixta", "per_decem",
        "per_plena", "potentia", "per_indirecta", "pura", "scriptor",
        "per_trans"
    };
    volatilis f64 duo     = 2.0;
    volatilis f32 septem  = 7.0f;
    volatilis f32 decima  = 0.1f;
        Machinula* m;
     StatusCursus  s;
     StatusCursus  t;
           chorda  captum_ante;
           chorda  captum_post;
              f64  lex;
              f32  lex_angusta;

    CREDO_NECESSE (_parare(c->piscina));
    m = _machinulam_parare(c->piscina, TEXTUS_LIMINUM_FLUITANTIUM);
    CREDO_NECESSE_NON_NIHIL (m);
    /* lex (interpretata, contra C hospitis) */
    s = _statum_capere(c->piscina, m, "m_media");
    CREDO_AEQUALIS_S32 (s.genus, MACHINULA_BENE);
    CREDO_VERUM ((i64)s.codex == _bita_f64(4.25));
    lex_angusta  = septem + _hospes_media_angusta(septem, decima);
    s            = _statum_capere(c->piscina, m, "m_media32");
    CREDO_VERUM ((i64)s.codex == _bita_f64((f64)lex_angusta));
    s = _statum_capere(c->piscina, m, "m_mixta");
    CREDO_VERUM ((i64)s.codex == _bita_f64(3000000000065.75));
    s = _statum_capere(c->piscina, m, "m_decem");
    CREDO_VERUM ((i64)s.codex == _bita_f64(_lex_decem()));
    s = _statum_capere(c->piscina, m, "m_plena");
    CREDO_VERUM ((i64)s.codex == _bita_f64(_lex_plena()));
    s = _statum_capere(c->piscina, m, "m_potentia");
    CREDO_VERUM ((i64)s.codex == _bita_f64(57.6650390625));
    s = _statum_capere(c->piscina, m, "m_indirecta");
    CREDO_VERUM ((i64)s.codex == _bita_f64(7.75));
    /* parametra X fluitantia vocationem transeunt (servata d VIII:
     * duo effunduntur): media(1.5, 2.5) + summa ponderata */
    s = _statum_capere(c->piscina, m, "m_trans");
    CREDO_VERUM ((i64)s.codex == _bita_f64(417.0));
    lex  = sqrt(duo);
    lex  += pow(duo, 10.0);
    lex  += fabs(-duo);
    lex  += floor(-duo - 0.5);
    lex  += ldexp(0.75, IV);
    lex  += 2.5;
    lex  += duo;
    s    = _statum_capere(c->piscina, m, "m_pura");
    CREDO_VERUM ((i64)s.codex == _bita_f64(lex));
    captum_ante = _scripturam_capere(c, m, "scriptura_f_ante.txt", &s);
    CREDO_VERUM (chorda_aequalis_literis(captum_ante,
        "1.75 2.5 7 1.750\n"));
    CREDO_VERUM (s.codex == (s64)XXVII);

    _cursus_conferre(c, m, cursus, IX, nativae, XI);
    CREDO_VERUM (_signum_functionis("media")
        == (s64)CONEXIO_SIGNUM_NATIVUM);
    CREDO_VERUM (_signum_functionis("decem")
        == (s64)CONEXIO_SIGNUM_NATIVUM);
    CREDO_VERUM (_signum_functionis("plena")
        == (s64)CONEXIO_SIGNUM_NATIVUM);
    /* printf et strtod per pontem cum registris fluitantibus */
    captum_post = _scripturam_capere(c, m, "scriptura_f_post.txt", &t);
    CREDO_VERUM (chorda_aequalis(captum_ante, captum_post));
    CREDO_VERUM (t.codex == (s64)XXVII);
}

interior vacuum
_sectio_dorsum_fluitantia_hospitis (
    CredoContextus* c)
{
    constans character* cursus[]    = {
        "m_h_decem", "m_h_plena", "m_h_media32"
    };
    constans character* vocantes[]  = {
        "per_h_decem", "per_h_plena", "per_h_media32"
    };
    constans character* vocatae[]   = {
        "decem", "plena", "media32", "mixta", "potentia", "trans"
    };
    f64 (*h_decem)(f64, f64, f64, f64, f64, f64, f64, f64, f64, f64,
        f32,
        f32, f64)                     = _hospes_decem;
    f64 (*h_plena)(s64, s64, s64, s64, s64, s64, s64, s64, s64, f64,
        f64,
        f64, f64, f64, f64, f64, f64, f64, signatus character, f32)
                                      = _hospes_plena;
    f32 (*h_media_angusta)(f32, f32)  = _hospes_media_angusta;
     volatilis f32 septem             = 7.0f;
     volatilis f32 decima             = 0.1f;
    memoriae_index inscriptio;
               i64 exspectata[III];
         Machinula* m;
      StatusCursus  s;
            chorda  causa;
               s32  k;
               s32  transitus;

    CREDO_NECESSE (_parare(c->piscina));
    m = _machinulam_parare(c->piscina, TEXTUS_LIMINUM_FLUITANTIUM);
    CREDO_NECESSE_NON_NIHIL (m);
    memcpy(&inscriptio, &h_decem, magnitudo(inscriptio));
    CREDO_VERUM (_hospitem_ponere(m, "h_decem", inscriptio));
    memcpy(&inscriptio, &h_plena, magnitudo(inscriptio));
    CREDO_VERUM (_hospitem_ponere(m, "h_plena", inscriptio));
    memcpy(&inscriptio, &h_media_angusta, magnitudo(inscriptio));
    CREDO_VERUM (_hospitem_ponere(m, "h_media32", inscriptio));
    exspectata[ZEPHYRUM]  = _bita_f64(_lex_decem());
    exspectata[I]         = _bita_f64(_lex_plena());
    exspectata[II]        = _bita_f64((f64)(f32)(septem
        + _hospes_media_angusta(septem, decima)));

    /* I. vocans interpretatus -> C; II. vocans nativus -> C */
    per (transitus = ZEPHYRUM; transitus < II; transitus++)
    {
        per (k = ZEPHYRUM; k < III; k++)
        {
            s = _statum_capere(c->piscina, m, cursus[k]);
            si (   s.genus      != MACHINULA_BENE
                || (i64)s.codex != exspectata[k])
            {
                imprimere("HOSPES F DISPAR %s (transitus %d): genus %d "
                    "codex %lx exspectatum %lx %.*s\n", cursus[k],
                    (int)transitus, (int)s.genus,
                    (insignatus longus)s.codex,
                    (insignatus longus)exspectata[k],
                    (int)s.nuntius.mensura,
                    (constans character*)s.nuntius.datum);
            }
            CREDO_AEQUALIS_S32 (s.genus, MACHINULA_BENE);
            CREDO_VERUM ((i64)s.codex == exspectata[k]);
        }
        si (transitus == ZEPHYRUM)
        {
            per (k = ZEPHYRUM; k < III; k++)
            {
                b32 compilata = machinula_nativum_compilare(m,
                    _ch(vocantes[k]), &causa);

                si (!compilata)
                {
                    imprimere("RECUSATA %s: %.*s\n", vocantes[k],
                        (int)causa.mensura,
                        (constans character*)causa.datum);
                }
                CREDO_VERUM (compilata);
            }
            CREDO_VERUM (_signum_functionis("per_h_plena")
                == (s64)CONEXIO_SIGNUM_NATIVUM);
        }
    }

    /* III. C -> functio dorsi */
    per (k = ZEPHYRUM; k < VI; k++)
    {
        CREDO_NECESSE (machinula_nativum_compilare(m, _ch(vocatae[k]),
            &causa));
        CREDO_NECESSE (_introitus_nativae(vocatae[k]) != ZEPHYRUM);
    }
    {
        f64 (*f_decem)(f64, f64, f64, f64, f64, f64, f64, f64, f64, f64,
            f32, f32, f64);
        f64 (*f_plena)(s64, s64, s64, s64, s64, s64, s64, s64, s64, f64,
            f64, f64, f64, f64, f64, f64, f64, f64, signatus character,
            f32);
        f32 (*f_media_angusta)(f32, f32);
        f64 (*f_mixta)(s32, f64, s64, f32, s32);
        f64 (*f_potentia)(f64, s32);
        f64 (*f_trans)(f64, f64, f64, f64, f64, f64, f64, f64, f64,
            f64);

        inscriptio = _introitus_nativae("decem");
        memcpy(&f_decem, &inscriptio, magnitudo(f_decem));
        CREDO_VERUM (f_decem(1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.5, 8.5,
            9.5,
            10.5, 1.25f, -2.75f, 100.125) == _lex_decem());

        inscriptio = _introitus_nativae("plena");
        memcpy(&f_plena, &inscriptio, magnitudo(f_plena));
        CREDO_VERUM (f_plena(V, II, III, IV, V, VI, VII, VIII, -(s64)IX,
            1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.5, 8.5, -9.5,
            (signatus character)-III, 0.375f) == _lex_plena());

        inscriptio = _introitus_nativae("media32");
        memcpy(&f_media_angusta, &inscriptio,
            magnitudo(f_media_angusta));
        CREDO_VERUM (f_media_angusta(septem, decima)
            == _hospes_media_angusta(septem, decima));

        inscriptio = _introitus_nativae("mixta");
        memcpy(&f_mixta, &inscriptio, magnitudo(f_mixta));
        CREDO_VERUM (f_mixta(-(s32)IV, 1.5, (s64)1000000000000L, 0.75f,
            (s32)IX) == 3000000000065.75);

        inscriptio = _introitus_nativae("potentia");
        memcpy(&f_potentia, &inscriptio, magnitudo(f_potentia));
        CREDO_VERUM (f_potentia(1.5, (s32)X) == 57.6650390625);

        inscriptio = _introitus_nativae("trans");
        memcpy(&f_trans, &inscriptio, magnitudo(f_trans));
        CREDO_VERUM (f_trans(1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.5, 8.5,
            9.5,
            10.5) == 417.0);
    }
}

interior vacuum
_sectio_dorsum_fixturae_vocationum (
    CredoContextus* c)
{
          character* literae = _fixturam_legere(c,
              "officina/probationes/fixa/machinula/vocationes.medulla");
          Machinula* m;
    MachinulaExitus  exitus;
             chorda  causa;

    CREDO_NECESSE_NON_NIHIL (literae);
    CREDO_NECESSE (_parare(c->piscina));
    m = _machinulam_parare(c->piscina, literae);
    CREDO_NECESSE_NON_NIHIL (m);
    exitus = machinula_currere(m, _ch("main"));
    CREDO_AEQUALIS_S32 (exitus.genus, MACHINULA_BENE);
    CREDO_VERUM (exitus.codex == (s64)XLII);
    /* $geminare per descriptorem vocatur (locus): nativa esse debet
     * antequam $main nativa eam vocet (limes C9) */
    CREDO_VERUM (_nativam_facere(c->piscina, m, "geminare", &causa));
    CREDO_VERUM (_nativam_facere(c->piscina, m, "main", &causa));
    CREDO_VERUM (causa.mensura == ZEPHYRUM);
    exitus = machinula_currere(m, _ch("main"));
    CREDO_AEQUALIS_S32 (exitus.genus, MACHINULA_BENE);
    CREDO_VERUM (exitus.codex == (s64)XLII);
}


    /* ==================================================
     * DORSUM T9 gradus III: aggregata fluitantia homogenea (HFA,
     * membra I - IV eiusdem typi fluitantis): membra in registris d
     * CONTINUIS, aut totum in stiva (ordinatione membri, magnitudine
     * EXACTA; d reliqua vacant); %reditus HFA in d0 - d3 redit. clang
     * ut oraculum.
     * ================================================== */

#define TEXTUS_AGGREGATORUM_FLUITANTIUM \
    "modulus \"probatio/hfa.medulla\"\n" \
    "\n" \
    "functio $bita (f64 %v) -> s64\n" \
    "@initium:\n" \
    "    %l = arca 8, 8\n" \
    "    scribere.f64 %l, %v\n" \
    "    %b = legere.s64 %l\n" \
    "    redde %b\n" \
    "\n" \
    "functio $hfa_mixta (f64 %d1, f64 %d2, f64 %d3, f64 %d4, f64 " \
    "%d5, f64 %d6, f64 %d7, f32 %f8, f32 %s1, h3xf32 %t, f32 " \
    "%s2, s8 %c, h2xf64 %d, f32 %s3) -> f64\n" \
    "@initium:\n" \
    "    %s = movere.f64 %d1\n" \
    "    %q = multiplicare.f64 %d7, 2.0\n" \
    "    %s = addere.f64 %s, %q\n" \
    "    %e = dilatare.f32.f64 %f8\n" \
    "    %q = multiplicare.f64 %e, 3.0\n" \
    "    %s = addere.f64 %s, %q\n" \
    "    %e = dilatare.f32.f64 %s1\n" \
    "    %q = multiplicare.f64 %e, 5.0\n" \
    "    %s = addere.f64 %s, %q\n" \
    "    %m = legere.f32 %t\n" \
    "    %e = dilatare.f32.f64 %m\n" \
    "    %q = multiplicare.f64 %e, 7.0\n" \
    "    %s = addere.f64 %s, %q\n" \
    "    %p = addere.i64 %t, 4\n" \
    "    %m = legere.f32 %p\n" \
    "    %e = dilatare.f32.f64 %m\n" \
    "    %q = multiplicare.f64 %e, 11.0\n" \
    "    %s = addere.f64 %s, %q\n" \
    "    %p = addere.i64 %t, 8\n" \
    "    %m = legere.f32 %p\n" \
    "    %e = dilatare.f32.f64 %m\n" \
    "    %q = multiplicare.f64 %e, 13.0\n" \
    "    %s = addere.f64 %s, %q\n" \
    "    %e = dilatare.f32.f64 %s2\n" \
    "    %q = multiplicare.f64 %e, 17.0\n" \
    "    %s = addere.f64 %s, %q\n" \
    "    %i = fluitare.s8.f64 %c\n" \
    "    %q = multiplicare.f64 %i, 19.0\n" \
    "    %s = addere.f64 %s, %q\n" \
    "    %n = legere.f64 %d\n" \
    "    %q = multiplicare.f64 %n, 23.0\n" \
    "    %s = addere.f64 %s, %q\n" \
    "    %p = addere.i64 %d, 8\n" \
    "    %n = legere.f64 %p\n" \
    "    %q = multiplicare.f64 %n, 29.0\n" \
    "    %s = addere.f64 %s, %q\n" \
    "    %e = dilatare.f32.f64 %s3\n" \
    "    %q = multiplicare.f64 %e, 31.0\n" \
    "    %s = addere.f64 %s, %q\n" \
    "    redde %s\n" \
    "\n" \
    "functio $h_hfa_mixta (f64 %d1, f64 %d2, f64 %d3, f64 %d4, " \
    "f64 %d5, f64 %d6, f64 %d7, f32 %f8, f32 %s1, h3xf32 %t, f32 " \
    "%s2, s8 %c, h2xf64 %d, f32 %s3) -> f64\n" \
    "@initium:\n" \
    "    %z = movere.f64 0.0\n" \
    "    redde %z\n" \
    "\n" \
    "functio $hfa_plena (f64 %d1, f64 %d2, f64 %d3, f64 %d4, f64 " \
    "%d5, f64 %d6, h3xf32 %t, f32 %u, f64 %z) -> f64\n" \
    "@initium:\n" \
    "    %s = movere.f64 %d1\n" \
    "    %q = multiplicare.f64 %d6, 2.0\n" \
    "    %s = addere.f64 %s, %q\n" \
    "    %m = legere.f32 %t\n" \
    "    %e = dilatare.f32.f64 %m\n" \
    "    %q = multiplicare.f64 %e, 3.0\n" \
    "    %s = addere.f64 %s, %q\n" \
    "    %p = addere.i64 %t, 4\n" \
    "    %m = legere.f32 %p\n" \
    "    %e = dilatare.f32.f64 %m\n" \
    "    %q = multiplicare.f64 %e, 5.0\n" \
    "    %s = addere.f64 %s, %q\n" \
    "    %p = addere.i64 %t, 8\n" \
    "    %m = legere.f32 %p\n" \
    "    %e = dilatare.f32.f64 %m\n" \
    "    %q = multiplicare.f64 %e, 7.0\n" \
    "    %s = addere.f64 %s, %q\n" \
    "    %g = dilatare.f32.f64 %u\n" \
    "    %q = multiplicare.f64 %g, 11.0\n" \
    "    %s = addere.f64 %s, %q\n" \
    "    %q = multiplicare.f64 %z, 13.0\n" \
    "    %s = addere.f64 %s, %q\n" \
    "    redde %s\n" \
    "\n" \
    "functio $h_hfa_plena (f64 %d1, f64 %d2, f64 %d3, f64 %d4, " \
    "f64 %d5, f64 %d6, h3xf32 %t, f32 %u, f64 %z) -> f64\n" \
    "@initium:\n" \
    "    %z = movere.f64 0.0\n" \
    "    redde %z\n" \
    "\n" \
    "functio $hfa_registra (h3xf32 %t, f32 %x, h2xf64 %d, f64 " \
    "%y) -> f64\n" \
    "@initium:\n" \
    "    %s = movere.f64 0.0\n" \
    "    %m = legere.f32 %t\n" \
    "    %e = dilatare.f32.f64 %m\n" \
    "    %q = multiplicare.f64 %e, 1.0\n" \
    "    %s = addere.f64 %s, %q\n" \
    "    %p = addere.i64 %t, 4\n" \
    "    %m = legere.f32 %p\n" \
    "    %e = dilatare.f32.f64 %m\n" \
    "    %q = multiplicare.f64 %e, 2.0\n" \
    "    %s = addere.f64 %s, %q\n" \
    "    %p = addere.i64 %t, 8\n" \
    "    %m = legere.f32 %p\n" \
    "    %e = dilatare.f32.f64 %m\n" \
    "    %q = multiplicare.f64 %e, 3.0\n" \
    "    %s = addere.f64 %s, %q\n" \
    "    %e = dilatare.f32.f64 %x\n" \
    "    %q = multiplicare.f64 %e, 5.0\n" \
    "    %s = addere.f64 %s, %q\n" \
    "    %n = legere.f64 %d\n" \
    "    %q = multiplicare.f64 %n, 7.0\n" \
    "    %s = addere.f64 %s, %q\n" \
    "    %p = addere.i64 %d, 8\n" \
    "    %n = legere.f64 %p\n" \
    "    %q = multiplicare.f64 %n, 11.0\n" \
    "    %s = addere.f64 %s, %q\n" \
    "    %q = multiplicare.f64 %y, 13.0\n" \
    "    %s = addere.f64 %s, %q\n" \
    "    redde %s\n" \
    "\n" \
    "functio $h_hfa_registra (h3xf32 %t, f32 %x, h2xf64 %d, f64 " \
    "%y) -> f64\n" \
    "@initium:\n" \
    "    %z = movere.f64 0.0\n" \
    "    redde %z\n" \
    "\n" \
    "functio $per_hfa_registra (i64 %f) -> f64\n" \
    "@initium:\n" \
    "    %t = arca 16, 4\n" \
    "    %v = movere.f32 1.25\n" \
    "    scribere.f32 %t, %v\n" \
    "    %p = addere.i64 %t, 4\n" \
    "    %v = movere.f32 -2.5\n" \
    "    scribere.f32 %p, %v\n" \
    "    %p = addere.i64 %t, 8\n" \
    "    %v = movere.f32 3.75\n" \
    "    scribere.f32 %p, %v\n" \
    "    %d = arca 16, 8\n" \
    "    %w = movere.f64 4.125\n" \
    "    scribere.f64 %d, %w\n" \
    "    %p = addere.i64 %d, 8\n" \
    "    %w = movere.f64 -5.25\n" \
    "    scribere.f64 %p, %w\n" \
    "    %x = movere.f32 9.5\n" \
    "    %r = vocare.f64 %f, h3xf32 %t, f32 %x, h2xf64 %d, f64 " \
    "6.5\n" \
    "    redde %r\n" \
    "\n" \
    "functio $m_hfa_registra () -> s64\n" \
    "@initium:\n" \
    "    %f = locus $hfa_registra\n" \
    "    %r = vocare.f64 $per_hfa_registra, i64 %f\n" \
    "    %b = vocare.s64 $bita, f64 %r\n" \
    "    redde %b\n" \
    "\n" \
    "functio $m_h_hfa_registra () -> s64\n" \
    "@initium:\n" \
    "    %f = locus $h_hfa_registra\n" \
    "    %r = vocare.f64 $per_hfa_registra, i64 %f\n" \
    "    %b = vocare.s64 $bita, f64 %r\n" \
    "    redde %b\n" \
    "\n" \
    "functio $r4 (rh4xf64 %reditus, f64 %x) -> vacuum\n" \
    "@initium:\n" \
    "    scribere.f64 %reditus, %x\n" \
    "    %v = addere.f64 %x, 1.0\n" \
    "    %p = addere.i64 %reditus, 8\n" \
    "    scribere.f64 %p, %v\n" \
    "    %v = addere.f64 %x, 2.0\n" \
    "    %p = addere.i64 %reditus, 16\n" \
    "    scribere.f64 %p, %v\n" \
    "    %v = addere.f64 %x, 3.0\n" \
    "    %p = addere.i64 %reditus, 24\n" \
    "    scribere.f64 %p, %v\n" \
    "    redde\n" \
    "\n" \
    "functio $r3 (rh3xf32 %reditus, f32 %x) -> vacuum\n" \
    "@initium:\n" \
    "    scribere.f32 %reditus, %x\n" \
    "    %v = addere.f32 %x, 1.0\n" \
    "    %p = addere.i64 %reditus, 4\n" \
    "    scribere.f32 %p, %v\n" \
    "    %v = addere.f32 %x, 2.0\n" \
    "    %p = addere.i64 %reditus, 8\n" \
    "    scribere.f32 %p, %v\n" \
    "    redde\n" \
    "\n" \
    "functio $r2 (rh2xf64 %reditus, h3xf32 %t) -> vacuum\n" \
    "@initium:\n" \
    "    %a = legere.f32 %t\n" \
    "    %p = addere.i64 %t, 4\n" \
    "    %b = legere.f32 %p\n" \
    "    %p = addere.i64 %t, 8\n" \
    "    %c = legere.f32 %p\n" \
    "    %s = addere.f32 %a, %b\n" \
    "    %e = dilatare.f32.f64 %s\n" \
    "    scribere.f64 %reditus, %e\n" \
    "    %g = dilatare.f32.f64 %c\n" \
    "    %g = multiplicare.f64 %g, 2.0\n" \
    "    %p = addere.i64 %reditus, 8\n" \
    "    scribere.f64 %p, %g\n" \
    "    redde\n" \
    "\n" \
    "functio $h_r4 (rh4xf64 %reditus, f64 %x) -> vacuum\n" \
    "@initium:\n" \
    "    redde\n" \
    "\n" \
    "functio $h_r3 (rh3xf32 %reditus, f32 %x) -> vacuum\n" \
    "@initium:\n" \
    "    redde\n" \
    "\n" \
    "functio $h_r2 (rh2xf64 %reditus, h3xf32 %t) -> vacuum\n" \
    "@initium:\n" \
    "    redde\n" \
    "\n" \
    "functio $per_hfa_mixta (i64 %f) -> f64\n" \
    "@initium:\n" \
    "    %t = arca 16, 4\n" \
    "    %v = movere.f32 1.25\n" \
    "    scribere.f32 %t, %v\n" \
    "    %p = addere.i64 %t, 4\n" \
    "    %v = movere.f32 -2.5\n" \
    "    scribere.f32 %p, %v\n" \
    "    %p = addere.i64 %t, 8\n" \
    "    %v = movere.f32 3.75\n" \
    "    scribere.f32 %p, %v\n" \
    "    %d = arca 16, 8\n" \
    "    %w = movere.f64 4.125\n" \
    "    scribere.f64 %d, %w\n" \
    "    %p = addere.i64 %d, 8\n" \
    "    %w = movere.f64 -5.25\n" \
    "    scribere.f64 %p, %w\n" \
    "    %x = movere.f32 9.5\n" \
    "    %r = vocare.f64 %f, f64 1.5, f64 2.5, f64 3.5, f64 4.5, " \
    "f64 5.5, f64 6.5, f64 7.5, f32 8.25, f32 %x, h3xf32 %t, f32 " \
    "10.5, s8 -3, h2xf64 %d, f32 12.75\n" \
    "    redde %r\n" \
    "\n" \
    "functio $per_hfa_plena (i64 %f) -> f64\n" \
    "@initium:\n" \
    "    %t = arca 16, 4\n" \
    "    %v = movere.f32 1.25\n" \
    "    scribere.f32 %t, %v\n" \
    "    %p = addere.i64 %t, 4\n" \
    "    %v = movere.f32 -2.5\n" \
    "    scribere.f32 %p, %v\n" \
    "    %p = addere.i64 %t, 8\n" \
    "    %v = movere.f32 3.75\n" \
    "    scribere.f32 %p, %v\n" \
    "    %z = movere.f64 100.125\n" \
    "    %r = vocare.f64 %f, f64 1.5, f64 2.5, f64 3.5, f64 4.5, " \
    "f64 5.5, f64 6.5, h3xf32 %t, f32 0.375, f64 %z\n" \
    "    redde %r\n" \
    "\n" \
    "functio $per_r4 (i64 %f) -> f64\n" \
    "@initium:\n" \
    "    %l = arca 48, 8\n" \
    "    %p = addere.i64 %l, 32\n" \
    "    scribere.s64 %p, 1234605616436508552\n" \
    "    vocare %f, rh4xf64 %l, f64 2.5\n" \
    "    %s = legere.f64 %l\n" \
    "    %p = addere.i64 %l, 8\n" \
    "    %n = legere.f64 %p\n" \
    "    %q = multiplicare.f64 %n, 2.0\n" \
    "    %s = addere.f64 %s, %q\n" \
    "    %p = addere.i64 %l, 16\n" \
    "    %n = legere.f64 %p\n" \
    "    %q = multiplicare.f64 %n, 3.0\n" \
    "    %s = addere.f64 %s, %q\n" \
    "    %p = addere.i64 %l, 24\n" \
    "    %n = legere.f64 %p\n" \
    "    %q = multiplicare.f64 %n, 4.0\n" \
    "    %s = addere.f64 %s, %q\n" \
    "    %p = addere.i64 %l, 32\n" \
    "    %k = legere.s64 %p\n" \
    "    %c = aequalis.s64 %k, 1234605616436508552\n" \
    "    %i = fluitare.s32.f64 %c\n" \
    "    %q = multiplicare.f64 %i, 1000.0\n" \
    "    %s = addere.f64 %s, %q\n" \
    "    redde %s\n" \
    "\n" \
    "functio $per_r3 (i64 %f) -> f64\n" \
    "@initium:\n" \
    "    %l = arca 16, 4\n" \
    "    %p = addere.i64 %l, 12\n" \
    "    %v = movere.f32 64.0\n" \
    "    scribere.f32 %p, %v\n" \
    "    vocare %f, rh3xf32 %l, f32 1.5\n" \
    "    %s = movere.f64 0.0\n" \
    "    %m = legere.f32 %l\n" \
    "    %e = dilatare.f32.f64 %m\n" \
    "    %q = multiplicare.f64 %e, 1.0\n" \
    "    %s = addere.f64 %s, %q\n" \
    "    %p = addere.i64 %l, 4\n" \
    "    %m = legere.f32 %p\n" \
    "    %e = dilatare.f32.f64 %m\n" \
    "    %q = multiplicare.f64 %e, 2.0\n" \
    "    %s = addere.f64 %s, %q\n" \
    "    %p = addere.i64 %l, 8\n" \
    "    %m = legere.f32 %p\n" \
    "    %e = dilatare.f32.f64 %m\n" \
    "    %q = multiplicare.f64 %e, 3.0\n" \
    "    %s = addere.f64 %s, %q\n" \
    "    %p = addere.i64 %l, 12\n" \
    "    %m = legere.f32 %p\n" \
    "    %e = dilatare.f32.f64 %m\n" \
    "    %q = multiplicare.f64 %e, 1.0\n" \
    "    %s = addere.f64 %s, %q\n" \
    "    redde %s\n" \
    "\n" \
    "functio $per_r2 (i64 %f) -> f64\n" \
    "@initium:\n" \
    "    %t = arca 16, 4\n" \
    "    %v = movere.f32 1.25\n" \
    "    scribere.f32 %t, %v\n" \
    "    %p = addere.i64 %t, 4\n" \
    "    %v = movere.f32 -2.5\n" \
    "    scribere.f32 %p, %v\n" \
    "    %p = addere.i64 %t, 8\n" \
    "    %v = movere.f32 3.75\n" \
    "    scribere.f32 %p, %v\n" \
    "    %l = arca 16, 8\n" \
    "    vocare %f, rh2xf64 %l, h3xf32 %t\n" \
    "    %s = movere.f64 0.0\n" \
    "    %n = legere.f64 %l\n" \
    "    %q = multiplicare.f64 %n, 3.0\n" \
    "    %s = addere.f64 %s, %q\n" \
    "    %p = addere.i64 %l, 8\n" \
    "    %n = legere.f64 %p\n" \
    "    %q = multiplicare.f64 %n, 1.0\n" \
    "    %s = addere.f64 %s, %q\n" \
    "    redde %s\n" \
    "\n" \
    "functio $m_hfa_mixta () -> s64\n" \
    "@initium:\n" \
    "    %f = locus $hfa_mixta\n" \
    "    %r = vocare.f64 $per_hfa_mixta, i64 %f\n" \
    "    %b = vocare.s64 $bita, f64 %r\n" \
    "    redde %b\n" \
    "\n" \
    "functio $m_h_hfa_mixta () -> s64\n" \
    "@initium:\n" \
    "    %f = locus $h_hfa_mixta\n" \
    "    %r = vocare.f64 $per_hfa_mixta, i64 %f\n" \
    "    %b = vocare.s64 $bita, f64 %r\n" \
    "    redde %b\n" \
    "\n" \
    "functio $m_hfa_plena () -> s64\n" \
    "@initium:\n" \
    "    %f = locus $hfa_plena\n" \
    "    %r = vocare.f64 $per_hfa_plena, i64 %f\n" \
    "    %b = vocare.s64 $bita, f64 %r\n" \
    "    redde %b\n" \
    "\n" \
    "functio $m_h_hfa_plena () -> s64\n" \
    "@initium:\n" \
    "    %f = locus $h_hfa_plena\n" \
    "    %r = vocare.f64 $per_hfa_plena, i64 %f\n" \
    "    %b = vocare.s64 $bita, f64 %r\n" \
    "    redde %b\n" \
    "\n" \
    "functio $m_r4 () -> s64\n" \
    "@initium:\n" \
    "    %f = locus $r4\n" \
    "    %r = vocare.f64 $per_r4, i64 %f\n" \
    "    %b = vocare.s64 $bita, f64 %r\n" \
    "    redde %b\n" \
    "\n" \
    "functio $m_h_r4 () -> s64\n" \
    "@initium:\n" \
    "    %f = locus $h_r4\n" \
    "    %r = vocare.f64 $per_r4, i64 %f\n" \
    "    %b = vocare.s64 $bita, f64 %r\n" \
    "    redde %b\n" \
    "\n" \
    "functio $m_r3 () -> s64\n" \
    "@initium:\n" \
    "    %f = locus $r3\n" \
    "    %r = vocare.f64 $per_r3, i64 %f\n" \
    "    %b = vocare.s64 $bita, f64 %r\n" \
    "    redde %b\n" \
    "\n" \
    "functio $m_h_r3 () -> s64\n" \
    "@initium:\n" \
    "    %f = locus $h_r3\n" \
    "    %r = vocare.f64 $per_r3, i64 %f\n" \
    "    %b = vocare.s64 $bita, f64 %r\n" \
    "    redde %b\n" \
    "\n" \
    "functio $m_r2 () -> s64\n" \
    "@initium:\n" \
    "    %f = locus $r2\n" \
    "    %r = vocare.f64 $per_r2, i64 %f\n" \
    "    %b = vocare.s64 $bita, f64 %r\n" \
    "    redde %b\n" \
    "\n" \
    "functio $m_h_r2 () -> s64\n" \
    "@initium:\n" \
    "    %f = locus $h_r2\n" \
    "    %r = vocare.f64 $per_r2, i64 %f\n" \
    "    %b = vocare.s64 $bita, f64 %r\n" \
    "    redde %b\n"

nomen structura { f32 a; f32 b; f32 c; } TriaFluitantia;
nomen structura { f64 a; f64 b; } ParFluitantium;
nomen structura { f64 a; f64 b; f64 c; f64 d; } QuattuorFluitantia;

interior f64
_hospes_hfa_mixta (
                   f64 d1,
                   f64 d2,
                   f64 d3,
                   f64 d4,
                   f64 d5,
                   f64 d6,
                   f64 d7,
                   f32 f8,
                   f32 s1,
        TriaFluitantia t,
                   f32 s2,
    signatus character c,
        ParFluitantium d,
                   f32 s3)
{
    (vacuum)d2;
    (vacuum)d3;
    (vacuum)d4;
    (vacuum)d5;
    (vacuum)d6;
    redde d1 + d7 * 2.0 + (f64)f8 * 3.0 + (f64)s1 * 5.0 + (f64)t.a * 7.0
        + (f64)t.b * 11.0 + (f64)t.c * 13.0 + (f64)s2 * 17.0
        + (f64)c * 19.0 + d.a * 23.0 + d.b * 29.0 + (f64)s3 * 31.0;
}

interior f64
_hospes_hfa_plena (
               f64 d1,
               f64 d2,
               f64 d3,
               f64 d4,
               f64 d5,
               f64 d6,
    TriaFluitantia t,
               f32 e,
               f64 z)
{
    (vacuum)d2;
    (vacuum)d3;
    (vacuum)d4;
    (vacuum)d5;
    redde d1 + d6 * 2.0 + (f64)t.a * 3.0 + (f64)t.b * 5.0
        + (f64)t.c * 7.0 + (f64)e * 11.0 + z * 13.0;
}

/* HFA in registris: t in s0 - s2, x in s3, d in d4:d5, y in d6 */
interior f64
_hospes_hfa_registra (
    TriaFluitantia t,
               f32 x,
    ParFluitantium d,
               f64 y)
{
    redde (f64)t.a + (f64)t.b * 2.0 + (f64)t.c * 3.0 + (f64)x * 5.0
        + d.a * 7.0 + d.b * 11.0 + y * 13.0;
}

interior QuattuorFluitantia
_hospes_quattuor (
    f64 x)
{
    QuattuorFluitantia r;

    r.a = x;
    r.b = x + 1.0;
    r.c = x + 2.0;
    r.d = x + 3.0;
    redde r;
}

interior TriaFluitantia
_hospes_tria_fluitantia (
    f32 x)
{
    TriaFluitantia r;

    r.a = x;
    r.b = x + 1.0f;
    r.c = x + 2.0f;
    redde r;
}

interior ParFluitantium
_hospes_par_fluitantium (
    TriaFluitantia t)
{
    ParFluitantium r;

    r.a = (f64)(t.a + t.b);
    r.b = (f64)t.c * 2.0;
    redde r;
}

interior TriaFluitantia
_tria_probationis (vacuum)
{
    TriaFluitantia t;

    t.a = 1.25f;
    t.b = -2.5f;
    t.c = 3.75f;
    redde t;
}

interior f64
_lex_hfa_mixta (vacuum)
{
    ParFluitantium d;

    d.a = 4.125;
    d.b = -5.25;
    redde _hospes_hfa_mixta(1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.5, 8.25f,
        9.5f, _tria_probationis(), 10.5f, (signatus character)-III, d,
        12.75f);
}

interior f64
_lex_hfa_plena (vacuum)
{
    redde _hospes_hfa_plena(1.5, 2.5, 3.5, 4.5, 5.5, 6.5,
        _tria_probationis(), 0.375f, 100.125);
}

interior f64
_lex_hfa_registra (vacuum)
{
    ParFluitantium d;

    d.a = 4.125;
    d.b = -5.25;
    redde _hospes_hfa_registra(_tria_probationis(), 9.5f, d, 6.5);
}

interior vacuum
_sectio_dorsum_aggregata_fluitantia (
    CredoContextus* c)
{
    constans character* cursus[]    = {
        "m_hfa_mixta", "m_h_hfa_mixta", "m_hfa_plena", "m_h_hfa_plena",
        "m_r4", "m_h_r4", "m_r3", "m_h_r3", "m_r2", "m_h_r2",
        "m_hfa_registra", "m_h_hfa_registra"
    };
    constans character* nativae[]   = {
        "bita", "hfa_mixta", "hfa_plena", "r4", "r3", "r2",
        "per_hfa_mixta", "per_hfa_plena", "per_r4", "per_r3", "per_r2",
        "hfa_registra", "per_hfa_registra"
    };
    f64 (*h_mixta)(f64, f64, f64, f64, f64, f64, f64, f32, f32,
        TriaFluitantia, f32, signatus character, ParFluitantium, f32)
                                             = _hospes_hfa_mixta;
    f64 (*h_plena)(f64, f64, f64, f64, f64, f64, TriaFluitantia, f32,
        f64)                                 = _hospes_hfa_plena;
    QuattuorFluitantia (*h_quattuor)(f64)    = _hospes_quattuor;
    TriaFluitantia (*h_tria)(f32)            = _hospes_tria_fluitantia;
    ParFluitantium (*h_par)(TriaFluitantia)  = _hospes_par_fluitantium;
    f64 (*h_registra)(TriaFluitantia, f32, ParFluitantium, f64)
                                             = _hospes_hfa_registra;
    memoriae_index inscriptio;
               i64 exspectata[XII];
         Machinula* m;
      StatusCursus  s;
               s32  k;

    CREDO_NECESSE (_parare(c->piscina));
    m = _machinulam_parare(c->piscina, TEXTUS_AGGREGATORUM_FLUITANTIUM);
    CREDO_NECESSE_NON_NIHIL (m);
    memcpy(&inscriptio, &h_mixta, magnitudo(inscriptio));
    CREDO_VERUM (_hospitem_ponere(m, "h_hfa_mixta", inscriptio));
    memcpy(&inscriptio, &h_plena, magnitudo(inscriptio));
    CREDO_VERUM (_hospitem_ponere(m, "h_hfa_plena", inscriptio));
    memcpy(&inscriptio, &h_quattuor, magnitudo(inscriptio));
    CREDO_VERUM (_hospitem_ponere(m, "h_r4", inscriptio));
    memcpy(&inscriptio, &h_tria, magnitudo(inscriptio));
    CREDO_VERUM (_hospitem_ponere(m, "h_r3", inscriptio));
    memcpy(&inscriptio, &h_par, magnitudo(inscriptio));
    CREDO_VERUM (_hospitem_ponere(m, "h_r2", inscriptio));
    memcpy(&inscriptio, &h_registra, magnitudo(inscriptio));
    CREDO_VERUM (_hospitem_ponere(m, "h_hfa_registra", inscriptio));

    /* lex: functio medullae et functio C idem reddunt; r4: 2.5 + 7 +
     * 13.5 + 22 + custos M; r3: 1.5 + 5 + 10.5 + custos 64; r2:
     * (1.25 - 2.5) * 3 + 7.5 */
    exspectata[ZEPHYRUM]  = _bita_f64(_lex_hfa_mixta());
    exspectata[II]        = _bita_f64(_lex_hfa_plena());
    exspectata[IV]        = _bita_f64(1045.0);
    exspectata[VI]        = _bita_f64(81.0);
    exspectata[VIII]      = _bita_f64(3.75);
    exspectata[X]         = _bita_f64(_lex_hfa_registra());
    per (k = ZEPHYRUM; k < XII; k += II)
    {
        exspectata[k + I] = exspectata[k];
    }
    /* I. omnia interpretata (cursus pares) et vocans interpretatus ->
     * C (cursus impares) */
    per (k = ZEPHYRUM; k < XII; k++)
    {
        s = _statum_capere(c->piscina, m, cursus[k]);
        si (s.genus != MACHINULA_BENE || (i64)s.codex != exspectata[k])
        {
            imprimere("HFA DISPAR %s: genus %d codex %lx exspectatum "
                "%lx %.*s\n", cursus[k], (int)s.genus,
                (insignatus longus)s.codex,
                (insignatus longus)exspectata[k],
                (int)s.nuntius.mensura,
                (constans character*)s.nuntius.datum);
        }
        CREDO_AEQUALIS_S32 (s.genus, MACHINULA_BENE);
        CREDO_VERUM ((i64)s.codex == exspectata[k]);
    }
    /* II. vocans nativus -> functio dorsi et -> C */
    _cursus_conferre(c, m, cursus, XII, nativae, XIII);
    CREDO_VERUM (_signum_functionis("per_r4")
        == (s64)CONEXIO_SIGNUM_NATIVUM);
    CREDO_VERUM (_signum_functionis("hfa_plena")
        == (s64)CONEXIO_SIGNUM_NATIVUM);

    /* III. C -> functio dorsi */
    {
        f64 (*f_mixta)(f64, f64, f64, f64, f64, f64, f64, f32, f32,
            TriaFluitantia, f32, signatus character, ParFluitantium,
            f32);
        f64 (*f_plena)(f64, f64, f64, f64, f64, f64, TriaFluitantia,
            f32,
            f64);
        QuattuorFluitantia (*f_quattuor)(f64);
        TriaFluitantia (*f_tria)(f32);
        ParFluitantium (*f_par)(TriaFluitantia);
        f64 (*f_registra)(TriaFluitantia, f32, ParFluitantium, f64);
        QuattuorFluitantia q;
            TriaFluitantia t;
            ParFluitantium p;

        p.a         = 4.125;
        p.b         = -5.25;
        inscriptio  = _introitus_nativae("hfa_mixta");
        CREDO_NECESSE (inscriptio != ZEPHYRUM);
        memcpy(&f_mixta, &inscriptio, magnitudo(f_mixta));
        CREDO_VERUM (f_mixta(1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.5, 8.25f,
            9.5f, _tria_probationis(), 10.5f, (signatus character)-III,
            p, 12.75f) == _lex_hfa_mixta());

        inscriptio = _introitus_nativae("hfa_plena");
        CREDO_NECESSE (inscriptio != ZEPHYRUM);
        memcpy(&f_plena, &inscriptio, magnitudo(f_plena));
        CREDO_VERUM (f_plena(1.5, 2.5, 3.5, 4.5, 5.5, 6.5,
            _tria_probationis(), 0.375f, 100.125) == _lex_hfa_plena());

        inscriptio = _introitus_nativae("r4");
        CREDO_NECESSE (inscriptio != ZEPHYRUM);
        memcpy(&f_quattuor, &inscriptio, magnitudo(f_quattuor));
        q = f_quattuor(2.5);
        CREDO_VERUM (q.a == 2.5 && q.b == 3.5 && q.c == 4.5
            && q.d == 5.5);

        inscriptio = _introitus_nativae("r3");
        CREDO_NECESSE (inscriptio != ZEPHYRUM);
        memcpy(&f_tria, &inscriptio, magnitudo(f_tria));
        t = f_tria(1.5f);
        CREDO_VERUM (t.a == 1.5f && t.b == 2.5f && t.c == 3.5f);

        inscriptio = _introitus_nativae("r2");
        CREDO_NECESSE (inscriptio != ZEPHYRUM);
        memcpy(&f_par, &inscriptio, magnitudo(f_par));
        p = f_par(_tria_probationis());
        CREDO_VERUM (p.a == -1.25 && p.b == 7.5);

        p.a         = 4.125;
        p.b         = -5.25;
        inscriptio  = _introitus_nativae("hfa_registra");
        CREDO_NECESSE (inscriptio != ZEPHYRUM);
        memcpy(&f_registra, &inscriptio, magnitudo(f_registra));
        CREDO_VERUM (f_registra(_tria_probationis(), 9.5f, p, 6.5)
            == _lex_hfa_registra());
    }
}


    /* ==================================================
     * DORSUM T9 gradus IV: vitia. Vitium hardware in codice nativo ->
     * VITIUM cum positione (functio, index planus) et tabulatis
     * '[nativum]' signatis in relatione; stiva nativa exhausta ->
     * VITIUM nominatum, processus vivit; vitium EXTRA codicem nativum
     * ad actionem priorem (credo) pergit. Classes collationis pro
     * Opere X.
     * ================================================== */

/* classis collationis cursus interpretati et nativi (Opus X):
 *  AEQUALE              status pares (genus, codex, nuntius, tabulata)
 *  MACHINULA_STRICTIOR  machinula interpretans vitium NOMINAT quod
 *                       codex nativus aliter (aut non) videt - causa
 *                       nominata:
 *                         'memoria'  machinula omnem accessum probat
 *                                    ("memoria: ..."); nativa signum
 *                                    hardware accipit aut nihil
 *                         'recursio' machinula tabulata numerat;
 *                                    nativa stivam hospitis exhaurit
 *  DIFFERT              omne aliud - vitium dorsi */
#define COLLATIO_AEQUALE              ZEPHYRUM
#define COLLATIO_DIFFERT              I
#define COLLATIO_MACHINULA_STRICTIOR  II

interior b32
_nuntius_incipit (
    constans StatusCursus* s,
       constans character* literae)
{
    memoriae_index n = strlen(literae);

    redde (memoriae_index)s->nuntius.mensura >= n
        && memcmp(s->nuntius.datum, literae, n) == ZEPHYRUM;
}

interior s32
_classem_conferre (
     constans StatusCursus*  interpretata,
     constans StatusCursus*  nativa,
        constans character** causa_out)
{
    *causa_out = "";
    si (_status_pares(interpretata, nativa))
    {
        redde COLLATIO_AEQUALE;
    }
    si (interpretata->genus != MACHINULA_VITIUM)
    {
        redde COLLATIO_DIFFERT;
    }
    si (_nuntius_incipit(interpretata, "memoria: "))
    {
        *causa_out = "memoria";
        redde COLLATIO_MACHINULA_STRICTIOR;
    }
    si (   (   _nuntius_incipit(interpretata,
        "stiva vocationum exhausta")
            || _nuntius_incipit(interpretata, "stiva exhausta"))
        && nativa->genus == MACHINULA_VITIUM
        && _nuntius_incipit(nativa, "stiva nativa exhausta"))
    {
        *causa_out = "recursio";
        redde COLLATIO_MACHINULA_STRICTIOR;
    }
    redde COLLATIO_DIFFERT;
}

#define TEXTUS_VITIORUM \
    "modulus \"probatio/vitia.medulla\"\n" \
    "\n" \
    "functio $nullus (i64 %p) -> s32\n" \
    "@initium:\n" \
    "    %a = addere.i64 %p, 0\n" \
    "    %v = legere.s32 %a\n" \
    "    redde %v\n" \
    "\n" \
    "functio $per_nullum (i64 %p) -> s32\n" \
    "@initium:\n" \
    "    %t = vocare.s32 $nullus, i64 %p\n" \
    "    %u = addere.s32 %t, 1\n" \
    "    redde %u\n" \
    "\n" \
    "functio $m_nullus () -> s32\n" \
    "@initium:\n" \
    "    %t = vocare.s32 $per_nullum, i64 0\n" \
    "    redde %t\n" \
    "\n" \
    "functio $infinita (s64 %n) -> s64\n" \
    "@initium:\n" \
    "    %m = addere.s64 %n, 1\n" \
    "    %r = vocare.s64 $infinita, s64 %m\n" \
    "    %s = addere.s64 %r, 1\n" \
    "    redde %s\n" \
    "\n" \
    "functio $m_infinita () -> s64\n" \
    "@initium:\n" \
    "    %t = vocare.s64 $infinita, s64 0\n" \
    "    redde %t\n" \
    "\n" \
    "functio $lata (s64 %n) -> s64\n" \
    "@initium:\n" \
    "    %l = arca 200000, 8\n" \
    "    scribere.s64 %l, %n\n" \
    "    %m = addere.s64 %n, 1\n" \
    "    %r = vocare.s64 $lata, s64 %m\n" \
    "    %w = legere.s64 %l\n" \
    "    %s = addere.s64 %r, %w\n" \
    "    redde %s\n" \
    "\n" \
    "functio $m_lata () -> s64\n" \
    "@initium:\n" \
    "    %t = vocare.s64 $lata, s64 0\n" \
    "    redde %t\n" \
    "\n" \
    "functio $folium () -> vacuum\n" \
    "@initium:\n" \
    "    redde\n" \
    "\n" \
    "functio $gradus () -> vacuum\n" \
    "@initium:\n" \
    "    vocare $folium\n" \
    "    vocare $gradus\n" \
    "    redde\n" \
    "\n" \
    "functio $m_gradus () -> s64\n" \
    "@initium:\n" \
    "    vocare $gradus\n" \
    "    redde 1\n" \
    "\n" \
    "functio $viva (s64 %n) -> s64\n" \
    "@initium:\n" \
    "    %m = addere.s64 %n, 2\n" \
    "    redde %m\n" \
    "\n" \
    "functio $m_viva () -> s64\n" \
    "@initium:\n" \
    "    %t = vocare.s64 $viva, s64 40\n" \
    "    redde %t\n"

/* cursum currit cum relatione halitus (ansa II) in plagulam capta */
interior chorda
_relationem_capere (
        CredoContextus* c,
             Machinula* m,
    constans character* cursus,
    constans character* titulus_plagulae,
          StatusCursus* status_out)
{
    constans character* area = getenv("TMPDIR");
             character  via[DXII];
                  FILE* plagula;

    sprintf(via, "%s/%s", (area != NIHIL) ? area : "/tmp",
        titulus_plagulae);
    plagula = lectiones_fopen(via, "w");
    si (plagula == NIHIL)
    {
        chorda vacua;

        vacua.datum    = NIHIL;
        vacua.mensura  = ZEPHYRUM;
        redde vacua;
    }
    machinula_ansam_ponere(m, II, plagula);
    *status_out = _statum_capere(c->piscina, m, cursus);
    machinula_ansam_ponere(m, II, NIHIL);
    fclose(plagula);
    redde filum_legere_totum(via, c->piscina);
}

/* vitium EXTRA codicem nativum: lectio per monstratorem nullum in
 * codice hospitis */
interior vacuum
_vitium_hospitis (vacuum)
{
    volatilis s32* volatilis monstrator = NIHIL;

    *monstrator = (s32)I;
}

interior vacuum
_sectio_dorsum_nullus (
    CredoContextus* c)
{
    constans character* nativae[]  = { "nullus", "per_nullum" };
    constans character* causa_classis;
              Machinula* m;
           StatusCursus  ante;
           StatusCursus  post;
                 chorda  relatio_ante;
                 chorda  relatio_post;
                 chorda  causa;
                    s32  k;

    CREDO_NECESSE (_parare(c->piscina));
    m = _machinulam_parare(c->piscina, TEXTUS_VITIORUM);
    CREDO_NECESSE_NON_NIHIL (m);
    relatio_ante = _relationem_capere(c, m, "m_nullus",
        "relatio_nulli_ante.txt", &ante);
    CREDO_AEQUALIS_S32 (ante.genus, MACHINULA_VITIUM);
    CREDO_CHORDA_CONTINET (ante.nuntius, _ch("memoria: monstrator"));
    CREDO_FALSUM (chorda_continet(relatio_ante, _ch("[nativum]")));
    per (k = ZEPHYRUM; k < II; k++)
    {
        CREDO_VERUM (machinula_nativum_compilare(m, _ch(nativae[k]),
            &causa));
    }
    relatio_post = _relationem_capere(c, m, "m_nullus",
        "relatio_nulli_post.txt", &post);
    /* VITIUM; positio = index planus instructionis 'legere' (I) in
     * $nullus, tabulata eadem ac interpretata */
    CREDO_AEQUALIS_S32 (post.genus, MACHINULA_VITIUM);
    CREDO_CHORDA_CONTINET (post.nuntius, _ch("nativum: signum "));
    CREDO_VERUM (post.instructio_summa == I);
    CREDO_VERUM (post.tabulata == ante.tabulata);
    CREDO_VERUM (post.tabulata == III);
    CREDO_AEQUALIS_S32 (post.functio_summa, ante.functio_summa);
    CREDO_VERUM (post.instructio_summa == ante.instructio_summa);
    CREDO_VERUM (post.vestigium == ante.vestigium);
    /* classis: machinula strictior, causa 'memoria' */
    CREDO_AEQUALIS_S32 (_classem_conferre(&ante, &post, &causa_classis),
        COLLATIO_MACHINULA_STRICTIOR);
    CREDO_VERUM (strcmp(causa_classis, "memoria") == ZEPHYRUM);
    CREDO_AEQUALIS_S32 (_classem_conferre(&ante, &ante, &causa_classis),
        COLLATIO_AEQUALE);
    /* relatio: tabulata nativa signata cum titulo functionis;
     * tabulatum interpretatum ($m_nullus) sine signo */
    CREDO_CHORDA_CONTINET (relatio_post,
        _ch("    nullus @initium+1 [nativum]\n"));
    CREDO_CHORDA_CONTINET (relatio_post,
        _ch("    per_nullum @initium+1 [nativum]\n"));
    CREDO_CHORDA_CONTINET (relatio_post,
        _ch("    m_nullus @initium+1\n"));

    /* RF5 'vitium et credo': post vitium nativum, vitium EXTRA
     * codicem nativum ad actionem PRIOREM pergit (tractator noster
     * id non devorat). Actio prior in hac suita _tractator_prior est
     * (ante effugium installatus - locum tractatoris credo tenet) */
    {
        s32 priores = _prior_vocatus;

        si (sigsetjmp(_ambitus_prioris, I) == ZEPHYRUM)
        {
            _vitium_hospitis();
        }
        CREDO_AEQUALIS_S32 (_prior_vocatus, priores + I);
        CREDO_AEQUALIS_S32 (_prior_signum, (s32)SIGSEGV);
    }
    /* et machinula iterum utilis */
    post = _statum_capere(c->piscina, m, "m_viva");
    CREDO_AEQUALIS_S32 (post.genus, MACHINULA_BENE);
    CREDO_VERUM (post.codex == (s64)XLII);
}

interior vacuum
_sectio_dorsum_stiva_exhausta (
    CredoContextus* c)
{
    constans character* cursus[]   = { "m_infinita", "m_lata" };
    constans character* nativae[]  = { "infinita", "lata", "viva" };
    constans character* causa_classis;
              Machinula* m;
           StatusCursus  ante[II];
           StatusCursus  post;
                 chorda  relatio;
                 chorda  causa;
                    s32  k;
                    s32  iteratio;

    CREDO_NECESSE (_parare(c->piscina));
    m = _machinulam_parare(c->piscina, TEXTUS_VITIORUM);
    CREDO_NECESSE_NON_NIHIL (m);
    /* relatio interpretata in plagulam (tabulata multa) */
    per (k = ZEPHYRUM; k < II; k++)
    {
        (vacuum)_relationem_capere(c, m, cursus[k],
            "relatio_stivae_ante.txt", &ante[k]);
        CREDO_AEQUALIS_S32 (ante[k].genus, MACHINULA_VITIUM);
    }
    per (k = ZEPHYRUM; k < III; k++)
    {
        CREDO_VERUM (machinula_nativum_compilare(m, _ch(nativae[k]),
            &causa));
    }
    /* bis: tractator et stiva alterna post primum casum integra */
    per (iteratio = ZEPHYRUM; iteratio < II; iteratio++)
    {
        per (k = ZEPHYRUM; k < II; k++)
        {
            relatio = _relationem_capere(c, m, cursus[k],
                "relatio_stivae_post.txt", &post);
            CREDO_AEQUALIS_S32 (post.genus, MACHINULA_VITIUM);
            CREDO_VERUM (chorda_aequalis_literis(post.nuntius,
                "stiva nativa exhausta"));
            /* cacumen in prologo functionis recurrentis (instructio
             * prima), vocantes post vocationem */
            CREDO_CHORDA_CONTINET (relatio, _ch((k == ZEPHYRUM)
                ? "    infinita @initium+0 [nativum]\n"
                  "    infinita @initium+2 [nativum]\n"
                : "    lata @initium+0 [nativum]\n"
                  "    lata @initium+4 [nativum]\n"));
            CREDO_AEQUALIS_S32 (_classem_conferre(&ante[k], &post,
                &causa_classis), COLLATIO_MACHINULA_STRICTIOR);
            CREDO_VERUM (strcmp(causa_classis, "recursio") == ZEPHYRUM);
        }
        /* processus vivit; codex nativus iterum currit */
        post = _statum_capere(c->piscina, m, "m_viva");
        CREDO_AEQUALIS_S32 (post.genus, MACHINULA_BENE);
        CREDO_VERUM (post.codex == (s64)XLII);
    }
    /* vitium in 'stp' folii (tabulatum eius nondum positum): vocans
     * verus ($gradus post vocationem folii, +1) ex x30 restituitur,
     * non vocans vocantis (+2) */
    CREDO_VERUM (machinula_nativum_compilare(m, _ch("gradus"), &causa));
    relatio = _relationem_capere(c, m, "m_gradus",
        "relatio_stivae_post.txt", &post);
    CREDO_VERUM (chorda_aequalis_literis(post.nuntius,
        "stiva nativa exhausta"));
    CREDO_CHORDA_CONTINET (relatio, _ch(
        "    folium @initium+0 [nativum]\n"
        "    gradus @initium+1 [nativum]\n"
        "    gradus @initium+2 [nativum]\n"));
    /* collatio falsa DIFFERT manet */
    post        = ante[ZEPHYRUM];
    post.genus  = MACHINULA_BENE;
    CREDO_AEQUALIS_S32 (_classem_conferre(&ante[ZEPHYRUM], &post,
        &causa_classis), COLLATIO_DIFFERT);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "parare", _sectio_parare, _sectionem_parare, _sectionem_purgare,
        NIHIL,
        NIHIL },
    { "reditus per trampolinum", _sectio_reditus, _sectionem_parare,
        _sectionem_purgare,
        NIHIL, NIHIL },
    { "registra integra", _sectio_registra, _sectionem_parare,
        _sectionem_purgare,
        NIHIL, NIHIL },
    { "registra fluitantia", _sectio_fluitantia, _sectionem_parare,
        _sectionem_purgare,
        NIHIL, NIHIL },
    { "verba stivae", _sectio_stiva, _sectionem_parare,
        _sectionem_purgare, NIHIL,
        NIHIL },
    { "saltus", _sectio_saltus, _sectionem_parare, _sectionem_purgare,
        NIHIL,
        NIHIL },
    { "vitium", _sectio_vitium, _sectionem_parare, _sectionem_purgare,
        NIHIL,
        NIHIL },
    { "instructio illicita", _sectio_illicitum, _sectionem_parare,
        _sectionem_purgare,
        NIHIL, NIHIL },
    { "installationes", _sectio_installationes, _sectionem_parare,
        _sectionem_purgare,
        NIHIL, NIHIL },
    { "catena", _sectio_catena, _sectionem_parare, _sectionem_purgare,
        NIHIL,
        NIHIL },
    { "nativum vocatum", _sectio_nativum_vocatum, _sectionem_parare,
        _sectionem_purgare, NIHIL, NIHIL },
    { "nativum principale", _sectio_nativum_principale,
        _sectionem_parare, _sectionem_purgare, NIHIL, NIHIL },
    { "nativum saltus", _sectio_nativum_saltus, _sectionem_parare,
        _sectionem_purgare, NIHIL, NIHIL },
    { "nativum vitium", _sectio_nativum_vitium, _sectionem_parare,
        _sectionem_purgare, NIHIL, NIHIL },
    { "nativum argumenta nimia", _sectio_nativum_nimia,
        _sectionem_parare, _sectionem_purgare, NIHIL, NIHIL },
    { "transitus iteratus", _sectio_transitus, _sectionem_parare,
        _sectionem_purgare, NIHIL, NIHIL },
    { "dorsum recta", _sectio_dorsum_recta, _sectionem_parare,
        _sectionem_purgare, NIHIL, NIHIL },
    { "dorsum bitalia", _sectio_dorsum_bitorum, _sectionem_parare,
        _sectionem_purgare, NIHIL, NIHIL },
    { "dorsum ante definitionem", _sectio_dorsum_ante_definitionem,
        _sectionem_parare, _sectionem_purgare, NIHIL, NIHIL },
    { "dorsum effusio registrorum", _sectio_dorsum_effusio,
        _sectionem_parare,
        _sectionem_purgare, NIHIL, NIHIL },
    { "dorsum parametra", _sectio_dorsum_parametra, _sectionem_parare,
        _sectionem_purgare, NIHIL, NIHIL },
    { "dorsum constantes", _sectio_dorsum_constantes, _sectionem_parare,
        _sectionem_purgare, NIHIL, NIHIL },
    { "dorsum recusatio operationis",
        _sectio_dorsum_recusatio_operationis, _sectionem_parare,
        _sectionem_purgare, NIHIL, NIHIL },
    { "dorsum fixturae fluxus", _sectio_dorsum_fixturae_fluxus,
        _sectionem_parare,
        _sectionem_purgare, NIHIL, NIHIL },
    { "dorsum summa", _sectio_dorsum_summa, _sectionem_parare,
        _sectionem_purgare, NIHIL, NIHIL },
    { "dorsum ramus", _sectio_dorsum_ramus, _sectionem_parare,
        _sectionem_purgare, NIHIL, NIHIL },
    { "dorsum comparationes", _sectio_dorsum_comparationes,
        _sectionem_parare,
        _sectionem_purgare, NIHIL, NIHIL },
    { "dorsum ansae", _sectio_dorsum_ansae, _sectionem_parare,
        _sectionem_purgare, NIHIL, NIHIL },
    { "dorsum arithmetica", _sectio_dorsum_arithmetica,
        _sectionem_parare,
        _sectionem_purgare, NIHIL, NIHIL },
    { "dorsum divisio", _sectio_dorsum_divisio, _sectionem_parare,
        _sectionem_purgare, NIHIL, NIHIL },
    { "dorsum sistere", _sectio_dorsum_sistere, _sectionem_parare,
        _sectionem_purgare, NIHIL, NIHIL },
    { "dorsum latitudo", _sectio_dorsum_latitudo, _sectionem_parare,
        _sectionem_purgare, NIHIL, NIHIL },
    { "dorsum vocationes", _sectio_dorsum_vocationes, _sectionem_parare,
        _sectionem_purgare, NIHIL, NIHIL },
    { "dorsum recusationes", _sectio_dorsum_recusationes,
        _sectionem_parare,
        _sectionem_purgare, NIHIL, NIHIL },
    { "dorsum fixturae memoriae", _sectio_dorsum_fixturae_memoriae,
        _sectionem_parare,
        _sectionem_purgare, NIHIL, NIHIL },
    { "dorsum memoria", _sectio_dorsum_memoria, _sectionem_parare,
        _sectionem_purgare, NIHIL, NIHIL },
    { "dorsum aedificata", _sectio_dorsum_aedificata, _sectionem_parare,
        _sectionem_purgare, NIHIL, NIHIL },
    { "dorsum argumenta multa", _sectio_dorsum_argumenta_multa,
        _sectionem_parare,
        _sectionem_purgare, NIHIL, NIHIL },
    { "dorsum indirecta", _sectio_dorsum_indirecta, _sectionem_parare,
        _sectionem_purgare, NIHIL, NIHIL },
    { "dorsum chorda reddita", _sectio_dorsum_chorda_reddita,
        _sectionem_parare, _sectionem_purgare, NIHIL, NIHIL },
    { "dorsum aggregata", _sectio_dorsum_aggregata, _sectionem_parare,
        _sectionem_purgare, NIHIL, NIHIL },
    { "dorsum aggregata hospitis", _sectio_dorsum_aggregata_hospitis,
        _sectionem_parare, _sectionem_purgare, NIHIL, NIHIL },
    { "dorsum fluitantia", _sectio_dorsum_fluitantia, _sectionem_parare,
        _sectionem_purgare, NIHIL, NIHIL },
    { "dorsum nan", _sectio_dorsum_non_numeri, _sectionem_parare,
        _sectionem_purgare, NIHIL, NIHIL },
    { "dorsum fixturae conversionum",
        _sectio_dorsum_fixturae_conversionum, _sectionem_parare,
        _sectionem_purgare, NIHIL, NIHIL },
    { "dorsum limina fluitantia", _sectio_dorsum_limina_fluitantia,
        _sectionem_parare, _sectionem_purgare, NIHIL, NIHIL },
    { "dorsum fluitantia hospitis", _sectio_dorsum_fluitantia_hospitis,
        _sectionem_parare, _sectionem_purgare, NIHIL, NIHIL },
    { "dorsum fixturae vocationum", _sectio_dorsum_fixturae_vocationum,
        _sectionem_parare, _sectionem_purgare, NIHIL, NIHIL },
    { "dorsum aggregata fluitantia",
        _sectio_dorsum_aggregata_fluitantia,
        _sectionem_parare, _sectionem_purgare, NIHIL, NIHIL },
    { "dorsum nullus", _sectio_dorsum_nullus, _sectionem_parare,
        _sectionem_purgare, NIHIL, NIHIL },
    /* gravis: recursio INTERPRETATA sine fine (collatio classis
     * 'recursio') stivam regionis machinulae totam tangit - apex
     * ~CDXXXII MiB; cursus nativus stivam hospitis VIII MiB solum */
    { "dorsum stiva exhausta", _sectio_dorsum_stiva_exhausta,
        _sectionem_parare, _sectionem_purgare, "gravis", NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("officina_nativum", SECTIONES);
}
