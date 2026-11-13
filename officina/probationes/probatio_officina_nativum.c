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
#include "officina_machinula.h"
#include "officina_medulla_textus.h"
#include "credo.h"
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

/* argumenta integra plura quam VIII: stiva (Opus VIII) - halitus
 * nominans, numquam vocatio tacite trunca */
interior vacuum
_sectio_nativum_nimia (
    CredoContextus* c)
{
          Machinula* m;
             vacuum* incrementum;
    MachinulaExitus  exitus;

    CREDO_NECESSE (_parare(c->piscina));
    incrementum = _incrementum_ponere(c->piscina);
    CREDO_NECESSE_NON_NIHIL (incrementum);
    m = _machinulam_parare(c->piscina, TEXTUS_VOCATI);
    CREDO_NECESSE_NON_NIHIL (m);
    CREDO_VERUM (machinula_nativum_ponere(m, _ch("g"), incrementum));
    exitus = machinula_currere(m, _ch("novem"));
    CREDO_AEQUALIS_S32 (exitus.genus, MACHINULA_VITIUM);
    CREDO_CHORDA_CONTINET (exitus.nuntius, _ch("nativum: argumenta"));
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
 * interpretata manet */
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
        "    %p = arca 16, 8\n"
        "    redde 42\n");
    CREDO_NECESSE_NON_NIHIL (m);
    CREDO_FALSUM (_nativam_facere(c->piscina, m, "main", &causa));
    CREDO_VERUM (chorda_aequalis_literis(causa,
        "operatio arca nondum nativa"));
    exitus = machinula_currere(m, _ch("main"));
    CREDO_AEQUALIS_S32 (exitus.genus, MACHINULA_BENE);
    CREDO_VERUM (exitus.codex == (s64)XLII);
}


    /* ==================================================
     * DORSUM gradus II: saltus, rami, comparationes
     * ================================================== */

/* fixturam ex arbore legit ($main -> exspectatum) et utramque
 * confert */
interior vacuum
_fixturam_conferre (
        CredoContextus* c,
    constans character* via_partis,
                   s64  exspectatum)
{
    constans character* radix = getenv("RHUBARB_RADIX");
             character  via[CCLVI];
                chorda  textus;
             character* literae;

    sprintf(via, "%s/%s", (radix != NIHIL) ? radix : ".", via_partis);
    textus = filum_legere_totum(via, c->piscina);
    CREDO_NECESSE (textus.mensura > ZEPHYRUM);
    literae = piscina_allocare(c->piscina,
        (memoriae_index)textus.mensura + I);
    CREDO_NECESSE_NON_NIHIL (literae);
    memcpy(literae, textus.datum, (memoriae_index)textus.mensura);
    literae[textus.mensura] = '\0';
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
    redde a->genus == b->genus && a->codex == b->codex
        && chorda_aequalis(a->nuntius, b->nuntius)
        && a->tabulata == b->tabulata
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
        "    %p = arca 16, 8\n"
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
    /* vocatio per monstratorem et locus: nondum */
    CREDO_FALSUM (machinula_nativum_compilare(m,
        _ch("per_monstratorem"), &causa));
    CREDO_VERUM (chorda_aequalis_literis(causa,
        "operatio locus nondum nativa"));
    /* ignota */
    CREDO_FALSUM (machinula_nativum_compilare(m, _ch("nusquam"),
        &causa));
    CREDO_VERUM (chorda_aequalis_literis(causa, "functio ignota"));
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
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("officina_nativum", SECTIONES);
}
