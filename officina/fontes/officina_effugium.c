/* officina_effugium.c - introitus in codicem nativum et effugium
 * (M5 T6: area, trampolinum, vocare, salire, tractator signorum)
 *
 * TRAMPOLINUM (semel in aream scriptum, ad offset zephyrum):
 *   vacuum trampolinum (introitus, argumenta, fructus)
 * tabulatum proprium aperit (x29/x30, x19/x20 servata), verba stivae
 * infra sp transcribit (ordinatio XVI servata), d0-d7, x8, x0-x7 ex
 * argumentis onerat, 'blr' ad introitum, deinde x0:x1 et d0-d3 in
 * fructum scribit et tabulatum claudit.
 *
 * EFFUGIUM: vocare ambitum per sigsetjmp figit ANTE trampolinum;
 * salire fructum pendentem ponit et siglongjmp facit - stiva nativa
 * (tabulatum trampolini inclusum) simul relinquitur. Vocationes
 * nidatae: ambitus prior servatur et post reditum restituitur.
 *
 * VITIUM: tractator SIGSEGV/SIGBUS/SIGILL semel in parare installatur,
 * in stiva alterna (stiva nativa ipsa fracta esse potest). Signum cum
 * pc INTRA aream et vocatione pendente -> fructus VITIUM, siglongjmp
 * (larva signorum per sigsetjmp(.., I) restituitur). Omne aliud signum
 * ad actionem PRIOREM it (catena): tractator prior vocatur; prior
 * ordinarius aut neglegens restituitur et tractator redit - instructio
 * iterum currit et sub actione priore cadit. Fila nulla (lex domus):
 * stiva alterna fili principalis sola.
 */

#include "postulata_posix.h"
#include "officina_effugium.h"
#include "codificator_a64.h"

#include <setjmp.h>
#include <signal.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ucontext.h>

nomen vacuum (*Trampolinum)(
                       vacuum* introitus,
    constans EffugiumArgumenta* argumenta,
               EffugiumFructus* fructus);

hic_manens MemoriaExsecutabilis* _area         = NIHIL;
hic_manens          Trampolinum  _trampolinum  = NIHIL;
hic_manens                  s32  _installationes = ZEPHYRUM;

/* vocatio pendens: ambitus ad quem salire redit */
hic_manens      sigjmp_buf  _ambitus;
hic_manens             b32  _activum = FALSUM;
hic_manens EffugiumFructus  _redditum;   /* a trampolino scriptum */
hic_manens EffugiumFructus  _pendens;    /* a salire aut tractatore */

/* signa hardware quae codex nativus excitare potest */
#define SIGNA_NUMERUS III
hic_manens constans integer  SIGNA[SIGNA_NUMERUS] = {
    SIGSEGV, SIGBUS, SIGILL
};
hic_manens structura sigaction  _priores[SIGNA_NUMERUS];
hic_manens           character  _stiva_alterna[SIGSTKSZ];
/* fines areae: tractator eos sine vocatione legit */
hic_manens      memoriae_index  _area_basis = ZEPHYRUM;
hic_manens      memoriae_index  _area_finis = ZEPHYRUM;

/* registra trampolini */
#define R_FRUCTUS     XIX   /* x19: EffugiumFructus* (servatum) */
#define R_ARGUMENTA   XX    /* x20: EffugiumArgumenta* (servatum) */
#define R_INTROITUS   XVI   /* x16: functio vocanda */
#define R_NUMERUS     IX    /* x9:  verba stivae reliqua */
#define R_FONS        X     /* x10: cursor verborum fontis */
#define R_DESTINATIO  XI    /* x11: cursor in stiva nova */
#define R_OCTETI      XII   /* x12: octeti stivae (ordinati) */
#define R_VERBUM      XIV   /* x14: verbum in transitu */
#define R_SP          XXXI
#define R_FP          XXIX
#define R_LR          XXX
#define TABULATUM     XXXII /* octeti tabulati trampolini */

interior b32
_e (
    CodificatorA64* c,
               s32  forma,
               s32  latitudo,
               s32  rd,
               s32  rn,
               s32  rm,
               s64  immediatum,
               s32  translatio)
{
    CodificatorA64Instructio i;

    memset(&i, ZEPHYRUM, magnitudo(i));
    i.forma       = forma;
    i.latitudo    = latitudo;
    i.rd          = rd;
    i.rn          = rn;
    i.rm          = rm;
    i.immediatum  = immediatum;
    i.translatio  = translatio;
    redde codificator_a64_emittere(c, &i);
}

/* saltus condicionalis in registro (cbz/cbnz) ad notam */
interior b32
_saltum_registri_emittere (
    CodificatorA64* c,
               s32  forma,
               s32  latitudo,
               s32  locellus,
               s32  nota)
{
    CodificatorA64Instructio i;

    memset(&i, ZEPHYRUM, magnitudo(i));
    i.forma     = forma;
    i.latitudo  = latitudo;
    i.rd        = locellus;
    redde codificator_a64_saltum_emittere(c, &i, nota);
}

interior b32
_trampolinum_emittere (
    CodificatorA64* c)
{
    s64 off_x        = (s64)offsetof(EffugiumArgumenta, x);
    s64 off_d        = (s64)offsetof(EffugiumArgumenta, d);
    s64 off_stiva    = (s64)offsetof(EffugiumArgumenta, stiva);
    s64 off_numerus  = (s64)offsetof(EffugiumArgumenta, numerus_stivae);
    s64 off_x8       = (s64)offsetof(EffugiumArgumenta, x8);
    s64 off_fx       = (s64)offsetof(EffugiumFructus, x);
    s64 off_fd       = (s64)offsetof(EffugiumFructus, d);
    s32 nota_ansa;
    s32 nota_sine_stiva;
    s32 k;
    b32 bene;

    nota_ansa        = codificator_a64_notam_creare(c);
    nota_sine_stiva  = codificator_a64_notam_creare(c);

    /* prologus: stp x29,x30,[sp,#-32]!; mov x29,sp;
     * stp x19,x20,[sp,#16] */
    bene =
           _e(c, CODIFICATOR_A64_STP, VIII, R_FP, R_SP, R_LR,
               -(s64)TABULATUM, CODIFICATOR_A64_ANTE)
        && _e(c, CODIFICATOR_A64_ADD_IMM, LXIV, R_FP, R_SP, 0, 0, 0)
        && _e(c, CODIFICATOR_A64_STP, VIII, R_FRUCTUS, R_SP,
               R_ARGUMENTA, XVI, CODIFICATOR_A64_OFFSET)
        /* mov x19,x2; mov x20,x1; mov x16,x0 */
        && _e(c, CODIFICATOR_A64_ORR_REG, LXIV, R_FRUCTUS, R_SP, II, 0,
               0)
        && _e(c, CODIFICATOR_A64_ORR_REG, LXIV, R_ARGUMENTA, R_SP, I,
               0, 0)
        && _e(c, CODIFICATOR_A64_ORR_REG, LXIV, R_INTROITUS, R_SP, 0,
               0, 0)
        /* verba stivae: w9 = numerus; nullus -> transili */
        && _e(c, CODIFICATOR_A64_LDR_IMM, IV, R_NUMERUS, R_ARGUMENTA,
               0, off_numerus, 0)
        && _saltum_registri_emittere(c, CODIFICATOR_A64_CBZ, XXXII,
        R_NUMERUS,
               nota_sine_stiva)
        && _e(c, CODIFICATOR_A64_LDR_IMM, VIII, R_FONS, R_ARGUMENTA, 0,
               off_stiva, 0)
        /* x12 = (numerus * 8 + 15) & ~15 */
        && _e(c, CODIFICATOR_A64_UBFM, LXIV, R_OCTETI, R_NUMERUS, 0,
               LXI, LX)
        && _e(c, CODIFICATOR_A64_ADD_IMM, LXIV, R_OCTETI, R_OCTETI, 0,
               XV, 0)
        && _e(c, CODIFICATOR_A64_AND_IMM, LXIV, R_OCTETI, R_OCTETI, 0,
               (s64)~(i64)XV, 0)
        /* sp -= x12 (per x11: SUB_REG registrum XXXI ut XZR legit) */
        && _e(c, CODIFICATOR_A64_ADD_IMM, LXIV, R_DESTINATIO, R_SP, 0,
               0, 0)
        && _e(c, CODIFICATOR_A64_SUB_REG, LXIV, R_DESTINATIO,
               R_DESTINATIO, R_OCTETI, 0, 0)
        && _e(c, CODIFICATOR_A64_ADD_IMM, LXIV, R_SP, R_DESTINATIO, 0,
               0, 0);
    si (!bene)
    {
        redde FALSUM;
    }
    /* ansa: verbum unum per circuitum, ordine ascendente */
    codificator_a64_notam_ponere(c, nota_ansa);
    bene =
           _e(c, CODIFICATOR_A64_LDR_IMM, VIII, R_VERBUM, R_FONS, 0, 0,
               0)
        && _e(c, CODIFICATOR_A64_STR_IMM, VIII, R_VERBUM, R_DESTINATIO,
               0, 0, 0)
        && _e(c, CODIFICATOR_A64_ADD_IMM, LXIV, R_FONS, R_FONS, 0,
               VIII, 0)
        && _e(c, CODIFICATOR_A64_ADD_IMM, LXIV, R_DESTINATIO,
               R_DESTINATIO, 0, VIII, 0)
        && _e(c, CODIFICATOR_A64_SUB_IMM, LXIV, R_NUMERUS, R_NUMERUS,
               0, I, 0)
        && _saltum_registri_emittere(c, CODIFICATOR_A64_CBNZ, LXIV,
        R_NUMERUS,
               nota_ansa);
    si (!bene)
    {
        redde FALSUM;
    }
    codificator_a64_notam_ponere(c, nota_sine_stiva);
    /* d0-d7, x8, x0-x7 ex argumentis (x20 basis manet) */
    per (k = ZEPHYRUM; k < VIII && bene; k++)
    {
        bene = _e(c, CODIFICATOR_A64_LDR_FP_IMM, VIII, k, R_ARGUMENTA,
            0, off_d + (s64)k * VIII, 0);
    }
    bene = bene
        && _e(c, CODIFICATOR_A64_LDR_IMM, VIII, VIII, R_ARGUMENTA, 0,
               off_x8, 0);
    per (k = ZEPHYRUM; k < VIII && bene; k++)
    {
        bene = _e(c, CODIFICATOR_A64_LDR_IMM, VIII, k, R_ARGUMENTA, 0,
            off_x + (s64)k * VIII, 0);
    }
    /* vocatio; deinde x0:x1 et d0-d3 in fructum */
    bene = bene
        && _e(c, CODIFICATOR_A64_BLR, LXIV, 0, R_INTROITUS, 0, 0, 0)
        && _e(c, CODIFICATOR_A64_STR_IMM, VIII, 0, R_FRUCTUS, 0,
               off_fx, 0)
        && _e(c, CODIFICATOR_A64_STR_IMM, VIII, I, R_FRUCTUS, 0,
               off_fx + VIII, 0);
    per (k = ZEPHYRUM; k < IV && bene; k++)
    {
        bene = _e(c, CODIFICATOR_A64_STR_FP_IMM, VIII, k, R_FRUCTUS, 0,
            off_fd + (s64)k * VIII, 0);
    }
    /* epilogus: mov sp,x29 (verba stivae relinquuntur);
     * ldp x19,x20,[sp,#16]; ldp x29,x30,[sp],#32; ret */
    redde bene
        && _e(c, CODIFICATOR_A64_ADD_IMM, LXIV, R_SP, R_FP, 0, 0, 0)
        && _e(c, CODIFICATOR_A64_LDP, VIII, R_FRUCTUS, R_SP,
               R_ARGUMENTA, XVI, CODIFICATOR_A64_OFFSET)
        && _e(c, CODIFICATOR_A64_LDP, VIII, R_FP, R_SP, R_LR,
               (s64)TABULATUM, CODIFICATOR_A64_POST)
        && _e(c, CODIFICATOR_A64_RET, LXIV, 0, R_LR, 0, 0, 0);
}

interior vacuum
_effugii_causam_ponere (
               Piscina* piscina,
                chorda* causa_out,
    constans character* nuntius)
{
    si (causa_out != NIHIL && piscina != NIHIL)
    {
        *causa_out = chorda_ex_literis(nuntius, piscina);
    }
}

/* tractator signorum: VITIUM si pc in area et vocatio pendet, aliter
 * catena ad actionem priorem */
interior vacuum
_tractator (
      integer  signum,
    siginfo_t* notitia,
       vacuum* contextus_crudus)
{
             ucontext_t* contextus  = (ucontext_t*)contextus_crudus;
         memoriae_index  pc         = ZEPHYRUM;
         memoriae_index  fp         = ZEPHYRUM;
    structura sigaction* prior      = NIHIL;
                    s32 k;

    si (contextus != NIHIL && contextus->uc_mcontext != NIHIL)
    {
        /* arm64 (non arm64e): campi crudi sine signatura */
        pc = (memoriae_index)contextus->uc_mcontext->__ss.__pc;
        fp = (memoriae_index)contextus->uc_mcontext->__ss.__fp;
    }
    si (_activum && pc >= _area_basis && pc < _area_finis)
    {
        memset(&_pendens, ZEPHYRUM, magnitudo(_pendens));
        _pendens.genus              = EFFUGIUM_VITIUM;
        _pendens.signum_systematis  = (s32)signum;
        _pendens.pc                 = pc;
        _pendens.fp                 = fp;
        siglongjmp(_ambitus, I);
    }
    /* catena: signum non nostrum */
    per (k = ZEPHYRUM; k < SIGNA_NUMERUS; k++)
    {
        si (SIGNA[k] == signum)
        {
            prior = &_priores[k];
        }
    }
    si (prior == NIHIL)
    {
        redde;
    }
    si ((prior->sa_flags & SA_SIGINFO) != ZEPHYRUM)
    {
        si (prior->sa_sigaction != NIHIL)
        {
            prior->sa_sigaction(signum, notitia, contextus_crudus);
        }
        redde;
    }
    si (prior->sa_handler == SIG_DFL || prior->sa_handler == SIG_IGN)
    {
        /* actio prior restituitur; post reditum instructio iterum
         * currit et sub ea cadit (vitium hardware neglegi nequit) */
        (vacuum)sigaction(signum, prior, NIHIL);
        redde;
    }
    prior->sa_handler(signum);
}

/* stiva alterna + tractator pro SIGNA; actiones priores servantur */
interior b32
_signa_installare (vacuum)
{
              stack_t stiva;
    structura sigaction  actio;
                  s32 k;

    memset(&stiva, ZEPHYRUM, magnitudo(stiva));
    stiva.ss_sp    = _stiva_alterna;
    stiva.ss_size  = magnitudo(_stiva_alterna);
    si (sigaltstack(&stiva, NIHIL) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    memset(&actio, ZEPHYRUM, magnitudo(actio));
    actio.sa_sigaction = _tractator;
    (vacuum)sigemptyset(&actio.sa_mask);
    actio.sa_flags = SA_SIGINFO | SA_ONSTACK;
    per (k = ZEPHYRUM; k < SIGNA_NUMERUS; k++)
    {
        si (sigaction(SIGNA[k], &actio, &_priores[k]) != ZEPHYRUM)
        {
            /* quae iam posita sunt restituuntur */
            dum (k > ZEPHYRUM)
            {
                k--;
                (vacuum)sigaction(SIGNA[k], &_priores[k], NIHIL);
            }
            redde FALSUM;
        }
    }
    _installationes++;
    redde VERUM;
}

b32
officina_effugium_parare (
    Piscina* piscina,
     chorda* causa_out)
{
                 Piscina* propria;
    MemoriaExsecutabilis* area;
          CodificatorA64* c;
                  vacuum* basis;
          memoriae_index  mensura;

    si (_area != NIHIL)
    {
        redde VERUM;
    }
    /* piscina propria permanens: status processus globalis piscinam
     * vocantis (sectionis credo) supervivit */
    propria = piscina_generare_dynamicum("officina_effugium", 65536);
    si (propria == NIHIL)
    {
        _effugii_causam_ponere(piscina, causa_out,
            "effugium: piscina propria fracta");
        redde FALSUM;
    }
    area = memoria_exsecutabilis_reservare(propria,
        (memoriae_index)OFFICINA_NATIVUM_LOCUS,
        (memoriae_index)OFFICINA_NATIVUM_MENSURA, causa_out);
    si (area == NIHIL)
    {
        si (causa_out != NIHIL && piscina != NIHIL)
        {
            /* causa in piscina propria nata: in piscinam vocantis
             * transcribitur ante destructionem */
            *causa_out = chorda_transcribere(*causa_out, piscina);
        }
        piscina_destruere(propria);
        redde FALSUM;
    }
    c = codificator_a64_creare(propria);
    si (   c == NIHIL || !_trampolinum_emittere(c)
        || !codificator_a64_finire(c, causa_out))
    {
        _effugii_causam_ponere(piscina, causa_out,
            "effugium: trampolinum codificari nequit");
        memoria_exsecutabilis_liberare(area);
        piscina_destruere(propria);
        redde FALSUM;
    }
    mensura = (memoriae_index)codificator_a64_numerus(c) * IV;
    si (mensura > (memoriae_index)OFFICINA_EFFUGIUM_TRAMPOLINUM)
    {
        _effugii_causam_ponere(piscina, causa_out,
            "effugium: trampolinum spatium suum excedit");
        memoria_exsecutabilis_liberare(area);
        piscina_destruere(propria);
        redde FALSUM;
    }
    basis = memoria_exsecutabilis_basis(area);
    memoria_exsecutabilis_aperire(area);
    memcpy(basis, codificator_a64_verba(c), mensura);
    si (!memoria_exsecutabilis_sigillare(area, ZEPHYRUM, mensura))
    {
        _effugii_causam_ponere(piscina, causa_out,
            "effugium: trampolinum sigillari nequit");
        memoria_exsecutabilis_liberare(area);
        piscina_destruere(propria);
        redde FALSUM;
    }
    _area_basis = (memoriae_index)basis;
    _area_finis = _area_basis + memoria_exsecutabilis_mensura(area);
    si (!_signa_installare())
    {
        _effugii_causam_ponere(piscina, causa_out,
            "effugium: tractator signorum installari nequit");
        _area_basis = ZEPHYRUM;
        _area_finis = ZEPHYRUM;
        memoria_exsecutabilis_liberare(area);
        piscina_destruere(propria);
        redde FALSUM;
    }
    /* C89 -pedantic conversionem obiecti in functionem vetat */
    memcpy(&_trampolinum, &basis, magnitudo(_trampolinum));
    _area = area;
    redde VERUM;
}

MemoriaExsecutabilis*
officina_effugium_area (vacuum)
{
    redde _area;
}

s32
officina_effugium_installationes (vacuum)
{
    redde _installationes;
}

EffugiumFructus
officina_effugium_vocare (
                        vacuum* introitus,
    constans EffugiumArgumenta* argumenta)
{
    EffugiumFructus fructus;
         sigjmp_buf ambitus_prior;
                b32 activum_prius;

    memset(&fructus, ZEPHYRUM, magnitudo(fructus));
    si (   _trampolinum == NIHIL || introitus == NIHIL
        || argumenta    == NIHIL)
    {
        /* sine area aut sine introitu: vitium sine signo */
        fructus.genus = EFFUGIUM_VITIUM;
        redde fructus;
    }
    /* locales post sigsetjmp non mutantur (aliter post saltum
     * indeterminati): fructus vocationis in staticis iacet */
    memcpy(&ambitus_prior, &_ambitus, magnitudo(ambitus_prior));
    activum_prius = _activum;
    si (sigsetjmp(_ambitus, I) == ZEPHYRUM)
    {
        _activum = VERUM;
        memset(&_redditum, ZEPHYRUM, magnitudo(_redditum));
        _trampolinum(introitus, argumenta, &_redditum);
        _redditum.genus = EFFUGIUM_REDDITUM;
    }
    alioquin
    {
        _redditum = _pendens;
    }
    memcpy(&_ambitus, &ambitus_prior, magnitudo(_ambitus));
    _activum = activum_prius;
    redde _redditum;
}

vacuum
officina_effugium_salire (
               s64 codex,
    memoriae_index pc,
    memoriae_index fp)
{
    si (!_activum)
    {
        /* nulla vocatio pendens: nullus locus ad quem redeat */
        abort();
    }
    memset(&_pendens, ZEPHYRUM, magnitudo(_pendens));
    _pendens.genus  = EFFUGIUM_SALTUS;
    _pendens.codex  = codex;
    _pendens.pc     = pc;
    _pendens.fp     = fp;
    siglongjmp(_ambitus, I);
}
