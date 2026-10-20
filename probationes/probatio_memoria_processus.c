/* probatio_memoria_processus.c - lectio vestigii processus (credo
 * memoria T1, project-specs/credo-memoria-spec.md par. III) */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "credo.h"
#include "memoria_processus.h"
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

#define UNITAS_MEMORIAE ((i64)MXXIV * (i64)MXXIV)   /* MiB */

/* ipse: vestigium positivum, apex non minor, residens positivum */
interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    MemoriaProcessus m;

    (vacuum)c;
    CREDO_NECESSE (memoria_processus_legere(ZEPHYRUM, &m));
    CREDO_MAIOR_I64 (m.vestigium, (i64)ZEPHYRUM);
    CREDO_MAIOR_AUT_AEQUALIS_I64 (m.vestigium_apex, m.vestigium);
    CREDO_MAIOR_I64 (m.residens, (i64)ZEPHYRUM);
}

/* LXIV MiB tacti in piscina: vestigium saltem LX MiB crescit, apex
 * sequitur */
interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    MemoriaProcessus  ante;
    MemoriaProcessus  post;
                  i8* alveus;

    CREDO_NECESSE (memoria_processus_legere(ZEPHYRUM, &ante));
    alveus = (i8*)piscina_allocare(c->piscina,
        (memoriae_index)(LXIV * UNITAS_MEMORIAE));
    CREDO_NECESSE_NON_NIHIL (alveus);
    memset(alveus, 0x5a, (memoriae_index)(LXIV * UNITAS_MEMORIAE));
    CREDO_NECESSE (memoria_processus_legere(ZEPHYRUM, &post));
    CREDO_MAIOR_AUT_AEQUALIS_I64 (post.vestigium - ante.vestigium,
        LX * UNITAS_MEMORIAE);
    CREDO_MAIOR_AUT_AEQUALIS_I64 (post.vestigium_apex,
        ante.vestigium + LX * UNITAS_MEMORIAE);
}

/* pid ignotus aut negativus: FALSUM et exitus in zephyrum */
interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
    MemoriaProcessus m;

    (vacuum)c;
    m.vestigium = (i64)VII;
    CREDO_FALSUM (memoria_processus_legere((s32)-I, &m));
    CREDO_AEQUALIS_I64 (m.vestigium, (i64)ZEPHYRUM);
    CREDO_FALSUM (memoria_processus_legere((s32)DCCCXCIX * (s32)MXXIV
        * (s32)M, &m));
    CREDO_FALSUM (memoria_processus_legere(ZEPHYRUM, NIHIL));
}

/* filius furcatus a parente legitur. MENSURAE (AUDIENDA specis, non
 * assertae - pro decisionibus C2/C3 impressae): (a) apex filii post
 * furcam parentis LXIV MiB tactos hereditatne; (b) filius exitus sed
 * nondum messus legine potest */
interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
    MemoriaProcessus  filii;
    MemoriaProcessus  parentis;
                 s32  fistula[II];
                 s32  pid;
                 s32  status;
           character  signum;
                  i8* alveus;

    alveus = (i8*)piscina_allocare(c->piscina,
        (memoriae_index)(LXIV * UNITAS_MEMORIAE));
    CREDO_NECESSE_NON_NIHIL (alveus);
    memset(alveus, 0x5a, (memoriae_index)(LXIV * UNITAS_MEMORIAE));
    CREDO_NECESSE (pipe(fistula) == ZEPHYRUM);
    fflush(NIHIL);
    pid = (s32)fork();
    CREDO_NECESSE (pid >= ZEPHYRUM);
    si (pid == ZEPHYRUM)
    {
        /* filius: exspectat donec parens fistulam claudat */
        close(fistula[I]);
        (vacuum)read(fistula[ZEPHYRUM], &signum, I);
        _exit(ZEPHYRUM);
    }
    close(fistula[ZEPHYRUM]);
    CREDO_VERUM (memoria_processus_legere(pid, &filii));
    CREDO_MAIOR_I64 (filii.vestigium, (i64)ZEPHYRUM);
    CREDO_VERUM (memoria_processus_legere(ZEPHYRUM, &parentis));
    imprimere("\n  MENSURA (a) apex filii %lld MiB, parentis %lld MiB, "
        "vestigium filii %lld MiB\n",
        (signatus longus longus)(filii.vestigium_apex
            / UNITAS_MEMORIAE),
        (signatus longus longus)(parentis.vestigium_apex
            / UNITAS_MEMORIAE),
        (signatus longus longus)(filii.vestigium / UNITAS_MEMORIAE));
    close(fistula[I]);
    /* filius exit; nondum messus (zombie) */
    usleep(CC * M);
    imprimere("  MENSURA (b) filius exitus, nondum messus: "
        "legitur %s\n",
        memoria_processus_legere(pid, &filii) ? "VERUM" : "FALSUM");
    CREDO_VERUM (waitpid((pid_t)pid, &status, ZEPHYRUM) == (pid_t)pid);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "ipse legitur", _sectio_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "vestigium post tactum crescit", _sectio_secunda, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "pid ignotus", _sectio_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "filius legitur", _sectio_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("memoria_processus", SECTIONES);
}
