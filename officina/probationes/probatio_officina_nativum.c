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
#include "officina_machinula.h"
#include "officina_medulla_textus.h"
#include "credo.h"
#include <setjmp.h>
#include <signal.h>
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
    _regio_sectionis  = NIHIL;
    _prior_vocatus    = ZEPHYRUM;
    _prior_signum     = ZEPHYRUM;
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
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("officina_nativum", SECTIONES);
}
