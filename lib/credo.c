/* credo.c */
#include "postulata_posix.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/wait.h>


/* ======================================================
 * Globalis Status
 * ====================================================== */

universalis        Piscina* _credo_piscina  = NIHIL;
universalis   CredoNotatio* _credo_primus   = NIHIL;
universalis   CredoNotatio* _credo_nunc     = NIHIL;
universalis memoriae_index  _credo_numerus  = ZEPHYRUM;


/* ======================================================
 * Adiutores Interni
 * ====================================================== */

interior CredoNotatio*
_credo_allocare_notatio (
    vacuum)
{
    CredoNotatio* notatio;

    si (!_credo_piscina)
    {
        imprimere("FATALE: credo nunquam aperitum! Vocate credo_aperire primum.\n");
        exit(I);
    }

    notatio = (CredoNotatio*)piscina_allocare(_credo_piscina, magnitudo(CredoNotatio));
    si (!notatio)
    {
        redde NIHIL;
    }

    notatio->sequens = NIHIL;  /* Initiare sequens ad NIHIL */

    return notatio;
}

interior vacuum
_credo_concatenare (
    CredoNotatio* notatio)
{
    si (!notatio) redde;

    si (!_credo_primus)
    {
        _credo_primus  = notatio;
        _credo_nunc    = notatio;
    }
    alioquin
    {
        _credo_nunc->sequens  = notatio;
        _credo_nunc           = notatio;
    }

    _credo_numerus++;
}


/* ======================================================
 * Apparatus Globalis
 * ====================================================== */

vacuum
credo_aperire (
    Piscina* piscina)
{
    _credo_piscina  = piscina;
    _credo_primus   = NIHIL;
    _credo_nunc     = NIHIL;
    _credo_numerus  = ZEPHYRUM;
}

vacuum
credo_claudere (
    vacuum)
{
    /* Piscina manet; conditio solum reficitur */
    _credo_piscina  = NIHIL;
    _credo_primus   = NIHIL;
    _credo_nunc     = NIHIL;
    _credo_numerus  = ZEPHYRUM;
}


/* ======================================================
 * Interrogatio
 * ====================================================== */

memoriae_index
credo_numerus_totalis (
    vacuum)
{
    redde _credo_numerus;
}

memoriae_index
credo_numerus_praeteriti (
    vacuum)
{
    memoriae_index  summa    = ZEPHYRUM;
      CredoNotatio* notatio  = _credo_primus;

    dum (notatio)
    {
        si (notatio->praeteritus)
        {
            summa++;
        }
        notatio = notatio->sequens;
    }

    redde summa;
}

memoriae_index
credo_numerus_fracti (
    vacuum)
{
    redde _credo_numerus - credo_numerus_praeteriti();
}

b32
credo_omnia_praeterierunt (
    vacuum)
{
    redde credo_numerus_fracti() == ZEPHYRUM;
}

/* Fractas invenire. COPIAS reddit, non catenam filtratam: 'sequens'
 * originalium filtrando corrumperetur, et catena principalis omnibus
 * ceteris interrogationibus servit.
 *
 * Declarata ab initio, definita 2026-08-13 - probatio prima quae eam
 * vocavit (probatio_manus, quae nuntium fracturae ipsum probat)
 * symbolum deesse invenit. */
CredoNotatio*
credo_invenire_fractos (
    memoriae_index* numerus_fructus)
{
      CredoNotatio* primus   = NIHIL;
      CredoNotatio* ultimus  = NIHIL;
      CredoNotatio* notatio  = _credo_primus;
    memoriae_index  summa    = ZEPHYRUM;

    si (numerus_fructus)
    {
        *numerus_fructus = ZEPHYRUM;
    }
    si (!_credo_piscina)
    {
        redde NIHIL;
    }

    dum (notatio)
    {
        si (!notatio->praeteritus)
        {
            CredoNotatio* copia = (CredoNotatio*)piscina_allocare(
                _credo_piscina, magnitudo(CredoNotatio));
            si (!copia)
            {
                frange;
            }
            *copia          = *notatio;
            copia->sequens  = NIHIL;

            si (ultimus)
            {
                ultimus->sequens = copia;
            }
            alioquin
            {
                primus = copia;
            }
            ultimus = copia;
            summa++;
        }
        notatio = notatio->sequens;
    }

    si (numerus_fructus)
    {
        *numerus_fructus = summa;
    }
    redde primus;
}

vacuum
credo_imprimere_compendium (
    vacuum)
{
    memoriae_index totalis     = credo_numerus_totalis();
    memoriae_index praeteriti  = credo_numerus_praeteriti();
    memoriae_index fracti      = credo_numerus_fracti();

    imprimere("\n");
    imprimere("=== CREDO COMPENDIUM ===\n");
    imprimere("Totalis:    %lu\n", (insignatus longus)totalis);
    imprimere("Praeteriti: %lu\n", (insignatus longus)praeteriti);
    imprimere("Fracti:     %lu\n", (insignatus longus)fracti);

    si (fracti > ZEPHYRUM)
    {
        imprimere("\nConditio: FRACTA\n");
    }
    alioquin
    {
        imprimere("\nConditio: OMNIA PRAETERIERUNT\n");
    }
    imprimere("=========================\n\n");
}


/* ======================================================
 * Recordatio Interni
 * ====================================================== */

vacuum
_credo_notare (
    constans character* genus,
    constans character* expressio,
    constans character* valor_primus,
    constans character* valor_secundus,
    constans character* filum,
                   s32  versus,
                   b32  praeteritus)
{
    CredoNotatio* notatio = _credo_allocare_notatio();

    si (!notatio) redde;

    notatio->genus = chorda_ex_literis(genus,          _credo_piscina);
    notatio->expressio = chorda_ex_literis(expressio,      _credo_piscina);
    notatio->valor_primus = chorda_ex_literis(valor_primus,   _credo_piscina);
    notatio->valor_secundus = chorda_ex_literis(valor_secundus, _credo_piscina);
    notatio->filum = chorda_ex_literis(filum,          _credo_piscina);
    notatio->versus = versus;
    notatio->praeteritus = praeteritus;
    notatio->ordo = _credo_numerus;

    _credo_concatenare(notatio);

    /* Signum statim */
    si (praeteritus)
    {
        imprimere(".");
    }
    alioquin
    {
        imprimere("F");
        imprimere("\n FRACTA (%s): %s at %s:%d\n",
                  chorda_ut_cstr(notatio->genus, _credo_piscina),
                  chorda_ut_cstr(notatio->expressio, _credo_piscina),
                  chorda_ut_cstr(notatio->filum, _credo_piscina), versus);
        imprimere("  Speratus: %s\n",
                  chorda_ut_cstr(notatio->valor_secundus, _credo_piscina));
        imprimere("  Receptus: %s\n",
                  chorda_ut_cstr(notatio->valor_primus, _credo_piscina));
        /* effluere statim: probatio post fracturam saepe NIHIL
         * dereferens cadit, et per fistulam buffer plenus cum
         * relatione perit - segfault relationem absorbebat
         * (mensuratum 2026-07-31, desideratum 01KYWC5ZNX) */
        (vacuum)fflush(stdout);
    }
}

vacuum
_credo_notare_integrum (
    constans character* genus,
    constans character* expressio,
                   s32  valor_primus,
                   s32  valor_secundus,
    constans character* filum,
                   s32  versus)
{
    character buffer_primus[64];
    character buffer_secundus[64];
          b32 praeteritus;

    snprintf(buffer_primus,   64, "%d", valor_primus);
    snprintf(buffer_secundus, 64, "%d", valor_secundus);

    praeteritus = (valor_primus == valor_secundus);

    _credo_notare(genus, expressio, buffer_primus, buffer_secundus, filum, versus, praeteritus);
}

vacuum
_credo_notare_integrum_i32 (
    constans character* genus,
    constans character* expressio,
                   i32  valor_primus,
                   i32  valor_secundus,
    constans character* filum,
                   s32  versus)
{
    character buffer_primus[64];
    character buffer_secundus[64];
          b32 praeteritus;

    snprintf(buffer_primus,   64, "%d", valor_primus);
    snprintf(buffer_secundus, 64, "%d", valor_secundus);

    praeteritus = (valor_primus == valor_secundus);

    _credo_notare(genus, expressio, buffer_primus, buffer_secundus, filum, versus, praeteritus);
}

vacuum
_credo_notare_integrum_s64 (
    constans character* genus,
    constans character* expressio,
                   s64  valor_primus,
                   s64  valor_secundus,
    constans character* filum,
                   s32  versus)
{
    character buffer_primus[64];
    character buffer_secundus[64];
          b32 praeteritus;

    snprintf(buffer_primus,   64, "%lld", valor_primus);
    snprintf(buffer_secundus, 64, "%lld", valor_secundus);

    praeteritus = (valor_primus == valor_secundus);

    _credo_notare(genus, expressio, buffer_primus, buffer_secundus, filum, versus, praeteritus);
}

vacuum
_credo_notare_integrum_inaequalitas (
    constans character* genus,
    constans character* expressio,
                   s32  valor_primus,
                   s32  valor_secundus,
    constans character* filum,
                   s32  versus)
{
    character buffer_primus[64];
    character buffer_secundus[64];
          b32 praeteritus;

    snprintf(buffer_primus,   64, "%d", valor_primus);
    snprintf(buffer_secundus, 64, "%d", valor_secundus);

    praeteritus = (valor_primus != valor_secundus);

    _credo_notare(genus, expressio, buffer_primus, buffer_secundus, filum, versus, praeteritus);
}

vacuum
_credo_notare_integrum_inaequalitas_i32 (
    constans character* genus,
    constans character* expressio,
                   i32  valor_primus,
                   i32  valor_secundus,
    constans character* filum,
                   s32  versus)
{
    character buffer_primus[64];
    character buffer_secundus[64];
          b32 praeteritus;

    snprintf(buffer_primus,   64, "%d", valor_primus);
    snprintf(buffer_secundus, 64, "%d", valor_secundus);

    praeteritus = (valor_primus != valor_secundus);

    _credo_notare(genus, expressio, buffer_primus, buffer_secundus, filum, versus, praeteritus);
}

vacuum
_credo_notare_chorda (
    constans character* genus,
    constans character* expressio,
    constans character* valor_primus,
    constans character* valor_secundus,
    constans character* filum,
                   s32  versus)
{
       b32 praeteritus;
    chorda chorda_primus;
    chorda chorda_secundus;

    chorda_primus = chorda_ex_literis(valor_primus, _credo_piscina);
    chorda_secundus = chorda_ex_literis(valor_secundus, _credo_piscina);

    praeteritus = chorda_aequalis(chorda_primus, chorda_secundus);

    _credo_notare(genus, expressio, valor_primus,
                  valor_secundus, filum, versus, praeteritus);
}

vacuum
_credo_notare_chorda_literis (
    constans character* genus,
    constans character* expressio,
                chorda  valor_primus,
    constans character* valor_secundus,
    constans character* filum,
                   s32  versus)
{
    character buffer_primus[256];
          b32 praeteritus;
          i32 len_to_copy;

    /* Transcribere chorda ad buffer pro imprimere */
    len_to_copy = valor_primus.mensura;
    si (len_to_copy > 255)
    {
        len_to_copy = 255;
    }

    si (valor_primus.datum && len_to_copy > ZEPHYRUM)
    {
        memcpy(buffer_primus, valor_primus.datum, (memoriae_index)len_to_copy);
    }
    buffer_primus[len_to_copy] = '\0';

    praeteritus = chorda_aequalis_literis(valor_primus, valor_secundus);

    _credo_notare(genus, expressio, buffer_primus,
                  valor_secundus, filum, versus, praeteritus);
}

vacuum
_credo_notare_integrum_maior (
    constans character* genus,
    constans character* expressio,
                   s32  valor_primus,
                   s32  valor_secundus,
    constans character* filum,
                   s32  versus)
{
    character buffer_primus[64];
    character buffer_secundus[64];
          b32 praeteritus;

    snprintf(buffer_primus,   64, "%d", valor_primus);
    snprintf(buffer_secundus, 64, "%d", valor_secundus);

    praeteritus = (valor_primus > valor_secundus);

    _credo_notare(genus, expressio, buffer_primus, buffer_secundus, filum, versus, praeteritus);
}

vacuum
_credo_notare_integrum_maior_i32 (
    constans character* genus,
    constans character* expressio,
                   i32  valor_primus,
                   i32  valor_secundus,
    constans character* filum,
                   s32  versus)
{
    character buffer_primus[64];
    character buffer_secundus[64];
          b32 praeteritus;

    snprintf(buffer_primus,   64, "%u", valor_primus);
    snprintf(buffer_secundus, 64, "%u", valor_secundus);

    praeteritus = (valor_primus > valor_secundus);

    _credo_notare(genus, expressio, buffer_primus, buffer_secundus, filum, versus, praeteritus);
}

vacuum
_credo_notare_integrum_minor (
    constans character* genus,
    constans character* expressio,
                   s32  valor_primus,
                   s32  valor_secundus,
    constans character* filum,
                   s32  versus)
{
    character buffer_primus[64];
    character buffer_secundus[64];
          b32 praeteritus;

    snprintf(buffer_primus,   64, "%d", valor_primus);
    snprintf(buffer_secundus, 64, "%d", valor_secundus);

    praeteritus = (valor_primus < valor_secundus);

    _credo_notare(genus, expressio, buffer_primus, buffer_secundus, filum, versus, praeteritus);
}

vacuum
_credo_notare_integrum_minor_i32 (
    constans character* genus,
    constans character* expressio,
                   i32  valor_primus,
                   i32  valor_secundus,
    constans character* filum,
                   s32  versus)
{
    character buffer_primus[64];
    character buffer_secundus[64];
          b32 praeteritus;

    snprintf(buffer_primus,   64, "%u", valor_primus);
    snprintf(buffer_secundus, 64, "%u", valor_secundus);

    praeteritus = (valor_primus < valor_secundus);

    _credo_notare(genus, expressio, buffer_primus, buffer_secundus, filum, versus, praeteritus);
}

vacuum
_credo_notare_integrum_maior_aut_aequalis (
    constans character* genus,
    constans character* expressio,
                   s32  valor_primus,
                   s32  valor_secundus,
    constans character* filum,
                   s32  versus)
{
    character buffer_primus[64];
    character buffer_secundus[64];
          b32 praeteritus;

    snprintf(buffer_primus,   64, "%d", valor_primus);
    snprintf(buffer_secundus, 64, "%d", valor_secundus);

    praeteritus = (valor_primus >= valor_secundus);

    _credo_notare(genus, expressio, buffer_primus, buffer_secundus, filum, versus, praeteritus);
}

vacuum
_credo_notare_integrum_maior_aut_aequalis_i32 (
    constans character* genus,
    constans character* expressio,
                   i32  valor_primus,
                   i32  valor_secundus,
    constans character* filum,
                   s32  versus)
{
    character buffer_primus[64];
    character buffer_secundus[64];
          b32 praeteritus;

    snprintf(buffer_primus,   64, "%u", valor_primus);
    snprintf(buffer_secundus, 64, "%u", valor_secundus);

    praeteritus = (valor_primus >= valor_secundus);

    _credo_notare(genus, expressio, buffer_primus, buffer_secundus, filum, versus, praeteritus);
}

vacuum
_credo_notare_integrum_minor_aut_aequalis (
    constans character* genus,
    constans character* expressio,
                   s32  valor_primus,
                   s32  valor_secundus,
    constans character* filum,
                   s32  versus)
{
    character buffer_primus[64];
    character buffer_secundus[64];
          b32 praeteritus;

    snprintf(buffer_primus,   64, "%d", valor_primus);
    snprintf(buffer_secundus, 64, "%d", valor_secundus);

    praeteritus = (valor_primus <= valor_secundus);

    _credo_notare(genus, expressio, buffer_primus, buffer_secundus, filum, versus, praeteritus);
}

vacuum
_credo_notare_integrum_minor_aut_aequalis_i32 (
    constans character* genus,
    constans character* expressio,
                   i32  valor_primus,
                   i32  valor_secundus,
    constans character* filum,
                   s32  versus)
{
    character buffer_primus[64];
    character buffer_secundus[64];
          b32 praeteritus;

    snprintf(buffer_primus,   64, "%u", valor_primus);
    snprintf(buffer_secundus, 64, "%u", valor_secundus);

    praeteritus = (valor_primus <= valor_secundus);

    _credo_notare(genus, expressio, buffer_primus, buffer_secundus, filum, versus, praeteritus);
}

vacuum
_credo_notare_f64_proximus (
    constans character* genus,
    constans character* expressio,
                   f64  valor_primus,
                   f64  valor_secundus,
                   f64  epsilon,
    constans character* filum,
                   s32  versus)
{
    character buffer_primus[64];
    character buffer_secundus[64];
          b32 praeteritus;
          f64 differentia;

    snprintf(buffer_primus,   64, "%.10f", valor_primus);
    snprintf(buffer_secundus, 64, "%.10f (ε=%.10f)", valor_secundus, epsilon);

    differentia = fabs(valor_primus - valor_secundus);
    praeteritus = (differentia <= epsilon);

    _credo_notare(genus, expressio, buffer_primus, buffer_secundus, filum, versus, praeteritus);
}


/* ==================================================
 * Assertiones processus (spec stml-instrumentum 7.5.0)
 * ================================================== */

interior constans character*
_credo_signi_nomen (
    s32 signum)
{
    commutatio (signum)
    {
        casus SIGSEGV:
            redde "SEGV";
        casus SIGBUS:
            redde "BUS";
        casus SIGABRT:
            redde "ABRT";
        casus SIGFPE:
            redde "FPE";
        casus SIGILL:
            redde "ILL";
        casus SIGKILL:
            redde "KILL (mora excessa)";
        ordinarius:
            redde "signum ignotum";
    }
}

CredoProcessusFructus
credo_processus_incipere (
    vacuum)
{
    CredoProcessusFructus fructus;
                    pid_t pid;

    fructus.in_filio  = FALSUM;
    fructus.pid       = -I;

    /* Buffra ANTE furcam eicienda: aliter quidquid pendet in
     * filium transcribitur et BIS emittitur. */
    fflush(NIHIL);

    pid = fork();
    si (pid == ZEPHYRUM)
    {
        fructus.in_filio  = VERUM;
        fructus.pid       = ZEPHYRUM;
        redde fructus;
    }

    fructus.pid = (s32)pid;   /* -I si furca defecit */
    redde fructus;
}

vacuum
credo_processus_filium_finire (
    vacuum)
{
    /* _exit, NON exit: buffra parentis hereditata sunt, et exit()
     * ea eiceret. */
    _exit(ZEPHYRUM);
}

vacuum
credo_processus_iudicare (
    CredoProcessusFructus* fructus,
       constans character* genus,
       constans character* expressio,
                      i32  mora_ms,
                      s32  signum_exspectatum,
       constans character* filum,
                      s32  versus)
{
              integer status;
                pid_t peractus;
                  i32 elapsum_ms;
                  b32 praeteritus;
                  s32 signum_acceptum;
    structura timespec pausa;
  constans character* exspectatum;
  constans character* acceptum;

    si (fructus == NIHIL || fructus->in_filio)
    {
        redde;   /* filius numquam iudicat */
    }

    si (fructus->pid < ZEPHYRUM)
    {
        _credo_notare(genus, expressio, "furca defecit", "",
                      filum, versus, FALSUM);
        redde;
    }

    /* Terminus per POLLATIONEM, non per alarm(): SIGALRM cum omni
     * usu signorum in codice probato colliditur, et manubria sua
     * problemata reentrantiae ferunt. */
    status           = ZEPHYRUM;
    elapsum_ms       = ZEPHYRUM;
    signum_acceptum  = ZEPHYRUM;
    pausa.tv_sec     = (time_t)ZEPHYRUM;
    pausa.tv_nsec    = (longus)(M * M);   /* I ms */

    dum (VERUM)
    {
        peractus = waitpid((pid_t)fructus->pid, &status, WNOHANG);
        si (peractus == (pid_t)fructus->pid)
        {
            frange;
        }
        si (elapsum_ms >= mora_ms)
        {
            /* SIGKILL, non SIGTERM: processus impeditus manubrium
             * numquam curret. Praeterea SIGKILL in relatione
             * inambiguus est - NOS eum occidimus, non ipse ruit. */
            kill((pid_t)fructus->pid, SIGKILL);
            waitpid((pid_t)fructus->pid, &status, ZEPHYRUM);
            frange;
        }
        (vacuum)nanosleep(&pausa, NIHIL);
        elapsum_ms = elapsum_ms + I;
    }

    si (WIFSIGNALED(status))
    {
        signum_acceptum = (s32)WTERMSIG(status);
    }

    si (signum_exspectatum == ZEPHYRUM)
    {
        /* Praeterit SOLUM si normaliter exiit. Codex exitus ruinam
         * NARRARE NON POTEST - processus signo occisus nullum
         * habet; ergo WIFSIGNALED est mechanismus, non
         * WEXITSTATUS. */
        praeteritus = (b32)(WIFEXITED(status) != ZEPHYRUM);
    }
    alioquin
    {
        praeteritus = (b32)(signum_acceptum == signum_exspectatum);
    }

    /* 'Speratus' nominandus est, non vacuus relinquendus: relatio
     * 'Receptus SEGV / Speratus (null)' dimidiam narrat solum -
     * legens quid EXSPECTAVERIMUS scire debet. */
    si (signum_exspectatum == ZEPHYRUM)
    {
        exspectatum = "exitus normalis (nullum signum)";
    }
    alioquin
    {
        exspectatum = _credo_signi_nomen(signum_exspectatum);
    }

    si (signum_acceptum != ZEPHYRUM)
    {
        acceptum = _credo_signi_nomen(signum_acceptum);
    }
    alioquin si (WIFEXITED(status))
    {
        acceptum = "exitus normalis";
    }
    alioquin
    {
        acceptum = "status ignotus";
    }

    _credo_notare(genus, expressio, acceptum, exspectatum,
                  filum, versus, praeteritus);
}
