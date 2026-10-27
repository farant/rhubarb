/* specula_macos.c - specula plagulae per kqueue (vicus quies T6c;
 * varians aedilis macos - etiam iOS)
 *
 * kqueue unum; EVFILT_VNODE in plagula (scripta, extensa, deleta,
 * renominata) aut, dum plagula abest, in directorio parentis
 * (scriptum = introitus additus). EV_CLEAR: nota semel redditur, fossa
 * kqueue legenda dum notae pendent. Descriptores O_EVTONLY: nullum
 * octetum legitur. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "specula.h"
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/event.h>

structura Specula {
      integer  fossa_notarum;            /* -1 = clausa */
      integer  plagula;       /* O_EVTONLY plagulae; -1 = absens */
      integer  directorium;   /* O_EVTONLY parentis dum plagula abest */
    character* via;
    character* parens;
};

/* notae plagulae: quidquid contentum aut viam mutat */
#define NOTAE_PLAGULAE                                              \
    (NOTE_WRITE | NOTE_EXTEND | NOTE_DELETE | NOTE_RENAME)

interior b32
notare (
    integer fossa_notarum,
    integer fd,
        i32 notae)
{
    structura kevent k;

    memset(&k, ZEPHYRUM, magnitudo(k));
    k.ident   = (insignatus longus)fd;
    k.filter  = EVFILT_VNODE;
    k.flags   = (uint16_t)(EV_ADD | EV_CLEAR);
    k.fflags  = (uint32_t)notae;
    redde kevent(fossa_notarum, &k, I, NIHIL, ZEPHYRUM, NIHIL)
        == ZEPHYRUM;
}

interior integer
speculatorem_aperire (
    constans character* via)
{
    /* O_EVTONLY: notitiae solae, nullum octetum legitur - per librum
     * lectionum nihil sigillandum */
    /* <tolera codex="CANALIS_PRAETERITUS" (>notitiae solae O_EVTONLY */
    redde open(via, O_EVTONLY | O_CLOEXEC);
}

/* plagulam speculari si exstat (directorium tunc dimittitur), aliter
 * directorium parentis (si nondum). VERUM si plagula speculatur */
interior b32
plagulam_quaerere (
    Specula* s)
{
    si (s->plagula >= ZEPHYRUM)
    {
        redde VERUM;
    }
    s->plagula = speculatorem_aperire(s->via);
    si (s->plagula >= ZEPHYRUM)
    {
        si (notare(s->fossa_notarum, s->plagula, (i32)NOTAE_PLAGULAE))
        {
            si (s->directorium >= ZEPHYRUM)
            {
                (vacuum)close(s->directorium);
                s->directorium = -I;
            }
            redde VERUM;
        }
        (vacuum)close(s->plagula);
        s->plagula = -I;
    }
    si (s->directorium < ZEPHYRUM)
    {
        s->directorium = speculatorem_aperire(s->parens);
        si (   s->directorium >= ZEPHYRUM
            && !notare(s->fossa_notarum, s->directorium,
            (i32)NOTE_WRITE))
        {
            (vacuum)close(s->directorium);
            s->directorium = -I;
        }
    }
    redde FALSUM;
}

Specula*
specula_creare (
               Piscina* piscina,
    constans character* via)
{
                    Specula* s;
         constans character* ultima;
             memoriae_index  n;

    si (!piscina || !via || via[ZEPHYRUM] == '\0')
    {
        redde NIHIL;
    }
    s = (Specula*)piscina_allocare(piscina, magnitudo(Specula));
    si (!s)
    {
        redde NIHIL;
    }
    n          = strlen(via);
    s->via     = (character*)piscina_allocare(piscina, (i64)n + I);
    s->parens  = (character*)piscina_allocare(piscina, (i64)n + II);
    si (!s->via || !s->parens)
    {
        redde NIHIL;
    }
    memcpy(s->via, via, n + I);
    ultima = strrchr(via, '/');
    si (!ultima)
    {
        strcpy(s->parens, ".");
    }
    alioquin si (ultima == via)
    {
        strcpy(s->parens, "/");
    }
    alioquin
    {
        memcpy(s->parens, via, (memoriae_index)(ultima - via));
        s->parens[ultima - via] = '\0';
    }
    s->plagula        = -I;
    s->directorium    = -I;
    s->fossa_notarum  = kqueue();
    si (s->fossa_notarum < ZEPHYRUM)
    {
        redde NIHIL;
    }
    (vacuum)fcntl(s->fossa_notarum, F_SETFD, FD_CLOEXEC);
    (vacuum)plagulam_quaerere(s);
    si (s->plagula < ZEPHYRUM && s->directorium < ZEPHYRUM)
    {
        (vacuum)close(s->fossa_notarum);
        redde NIHIL;
    }
    redde s;
}

s32
specula_fossa (
    constans Specula* s)
{
    redde s ? (s32)s->fossa_notarum : -I;
}

b32
specula_exhaurire (
    Specula* s)
{
    structura kevent eventus[VIII];
    structura timespec nulla;
             integer n;
             integer k;
                 b32 mutata;

    si (!s || s->fossa_notarum < ZEPHYRUM)
    {
        redde FALSUM;
    }
    nulla.tv_sec   = ZEPHYRUM;
    nulla.tv_nsec  = ZEPHYRUM;
    mutata         = FALSUM;
    dum ((n = kevent(s->fossa_notarum, NIHIL, ZEPHYRUM, eventus, VIII,
        &nulla))
         > ZEPHYRUM)
    {
        mutata = VERUM;
        per (k = ZEPHYRUM; k < n; k++)
        {
            si (   s->plagula       >= ZEPHYRUM
                && eventus[k].ident == (insignatus longus)s->plagula
                && (eventus[k].fflags
                    & (uint32_t)(NOTE_DELETE | NOTE_RENAME))
                        != ZEPHYRUM)
            {
                /* rotatio: vetus dimittitur (nota eius cum fossa
                 * perit), via iterum quaeritur infra */
                (vacuum)close(s->plagula);
                s->plagula = -I;
            }
        }
    }
    si (s->plagula < ZEPHYRUM)
    {
        (vacuum)plagulam_quaerere(s);
    }
    redde mutata;
}

vacuum
specula_claudere (
    Specula* s)
{
    si (!s)
    {
        redde;
    }
    si (s->plagula >= ZEPHYRUM)
    {
        (vacuum)close(s->plagula);
    }
    si (s->directorium >= ZEPHYRUM)
    {
        (vacuum)close(s->directorium);
    }
    si (s->fossa_notarum >= ZEPHYRUM)
    {
        (vacuum)close(s->fossa_notarum);
    }
    s->plagula        = -I;
    s->directorium    = -I;
    s->fossa_notarum  = -I;
}
