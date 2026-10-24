/* probatio_lectiones.c - liber lectionum (fabrica plan 2 T1): quod
 * processus legit, quaesivit, exstare confirmavit; lineae integrae
 * sub filiis parallelis */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "credo.h"
#include "chorda.h"
#include "filum.h"
#include "via.h"
#include "lectiones.h"
#include "processus.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define LIBER "build/probatio_lectiones.tsv"

/* lineas libri numerare quae '<littera>\t<via>' integrae sunt */
interior i32
_lineae_integrae (
    chorda  liber,
       i32* numerus_omnium)
{
    i32 i;
    i32 initium;
    i32 integrae;

    integrae         = ZEPHYRUM;
    *numerus_omnium  = ZEPHYRUM;
    initium          = ZEPHYRUM;
    per (i = ZEPHYRUM; i < liber.mensura; i++)
    {
        si (liber.datum[i] == '\n')
        {
            (*numerus_omnium)++;
            si (   i - initium > II
                && liber.datum[initium]     == 'L'
                && liber.datum[initium + I] == '\t')
            {
                integrae++;
            }
            initium = i + I;
        }
    }
    redde integrae;
}

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    unsetenv("FABRICA_LECTIONES");
    (vacuum)remove(LIBER);
    (vacuum)filum_legere_totum("probationes/probatio_lectiones.c",
        piscina);
    CREDO_FALSUM(filum_existit(LIBER));
}

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
     chorda  liber;

    piscina = c->piscina;
    setenv("FABRICA_LECTIONES", LIBER, I);
    (vacuum)filum_legere_totum("probationes/probatio_lectiones.c",
        piscina);
    (vacuum)via_existit(chorda_ex_literis("include/nusquam.h",
        piscina));
    (vacuum)via_existit(chorda_ex_literis("include/latina.h",
        piscina));
    unsetenv("FABRICA_LECTIONES");
    liber = filum_legere_totum(LIBER, piscina);
    CREDO_VERUM(chorda_continet(liber, chorda_ex_literis(
        "L\tprobationes/probatio_lectiones.c\n", piscina)));
    CREDO_VERUM(chorda_continet(liber, chorda_ex_literis(
        "A\tinclude/nusquam.h\n", piscina)));
    CREDO_VERUM(chorda_continet(liber, chorda_ex_literis(
        "X\tinclude/latina.h\n", piscina)));
}

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
     chorda  liber;

    piscina = c->piscina;
    (vacuum)remove(LIBER);
    /* plagula delenda SINE libro creata: S eius solum ex deletione */
    (vacuum)filum_scribere_literis("build/probatio_lectiones_d.txt",
        "d\n");
    setenv("FABRICA_LECTIONES", LIBER, I);
    (vacuum)filum_delere("build/probatio_lectiones_d.txt");
    setenv("LECTIONES_PROBATIO", "valor_fictus", I);
    (vacuum)filum_scribere_literis("build/probatio_lectiones_s.txt",
        "x\n");
    CREDO_CHORDA_AEQUALIS_LITERIS(chorda_ex_literis(
        lectiones_ambitus("LECTIONES_PROBATIO"), piscina),
        "valor_fictus");
    unsetenv("FABRICA_LECTIONES");
    liber = filum_legere_totum(LIBER, piscina);
    CREDO_VERUM(chorda_continet(liber, chorda_ex_literis(
        "S\tbuild/probatio_lectiones_s.txt\n", piscina)));
    /* deletio = S (fabrica spec 3 T6: compilator temporarium delet) */
    CREDO_VERUM(chorda_continet(liber, chorda_ex_literis(
        "S\tbuild/probatio_lectiones_d.txt\n", piscina)));
    CREDO_VERUM(chorda_continet(liber, chorda_ex_literis(
        "E\tLECTIONES_PROBATIO\tvalor_fictus\n", piscina)));
}

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
     chorda  liber;

    piscina = c->piscina;
    (vacuum)remove(LIBER);
    setenv("FABRICA_LECTIONES", LIBER, I);
    {
        FILE* f;
        structura stat st;

        f = lectiones_fopen("probationes/probatio_lectiones.c", "rb");
        CREDO_NON_NIHIL(f);
        si (f != NIHIL)
        {
            (vacuum)fclose(f);
        }
        CREDO_NIHIL(lectiones_fopen("include/nusquam_ii.h", "r"));
        f = lectiones_fopen("build/probatio_lectiones_w.txt", "wb");
        CREDO_NON_NIHIL(f);
        si (f != NIHIL)
        {
            (vacuum)fclose(f);
        }
        f = lectiones_fopen("/dev/null", "rb");
        si (f != NIHIL)
        {
            (vacuum)fclose(f);
        }
        CREDO_AEQUALIS_S32(lectiones_stat("include/latina.h", &st),
            ZEPHYRUM);
        CREDO_VERUM(lectiones_stat("include/nusquam_iii.h", &st)
            != ZEPHYRUM);
    }
    unsetenv("FABRICA_LECTIONES");
    liber = filum_legere_totum(LIBER, piscina);
    CREDO_VERUM(chorda_continet(liber, chorda_ex_literis(
        "L\tprobationes/probatio_lectiones.c\n", piscina)));
    CREDO_VERUM(chorda_continet(liber, chorda_ex_literis(
        "A\tinclude/nusquam_ii.h\n", piscina)));
    CREDO_VERUM(chorda_continet(liber, chorda_ex_literis(
        "S\tbuild/probatio_lectiones_w.txt\n", piscina)));
    /* machina (/dev/): non ingressus - nulla linea */
    CREDO_FALSUM(chorda_continet(liber, chorda_ex_literis(
        "/dev/null", piscina)));
    CREDO_VERUM(chorda_continet(liber, chorda_ex_literis(
        "X\tinclude/latina.h\n", piscina)));
    CREDO_VERUM(chorda_continet(liber, chorda_ex_literis(
        "A\tinclude/nusquam_iii.h\n", piscina)));
    (vacuum)remove("build/probatio_lectiones_w.txt");
}

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
     chorda  liber;

    piscina = c->piscina;
    {
        pid_t filii[II];
          i32 k;
          i32 j;
          i32 omnes;
          i32 integrae;

        (vacuum)remove(LIBER);
        per (k = ZEPHYRUM; k < II; k++)
        {
            filii[k] = fork();
            si (filii[k] == ZEPHYRUM)
            {
                setenv("FABRICA_LECTIONES", LIBER, I);
                per (j = ZEPHYRUM; j < D; j++)
                {
                    lectiones_notare(LECTIO_LEGIT,
                        "probationes/via_longior_ad_probandum_"
                        "lineas_integras.c");
                }
                _exit(ZEPHYRUM);
            }
        }
        per (k = ZEPHYRUM; k < II; k++)
        {
            (vacuum)waitpid(filii[k], NIHIL, ZEPHYRUM);
        }
        liber     = filum_legere_totum(LIBER, piscina);
        integrae  = _lineae_integrae(liber, &omnes);
        CREDO_AEQUALIS_I32(omnes, M);
        CREDO_AEQUALIS_I32(integrae, M);
    }
}

/* IId. EXSECUTIO (P): processus_exsequi argv[0] ANTE fork notat -
 * etiam exec fractum (programma absens); sine libro nihil */
interior vacuum
_sectio_exsecutionis (
    CredoContextus* c)
{
               Piscina* piscina;
                chorda  liber;
    constans character* verum[II];
    constans character* absens[II];
     ProcessusResultus  r;

    piscina    = c->piscina;
    verum[0]   = "/usr/bin/true";
    verum[I]   = NIHIL;
    absens[0]  = "programma_nusquam_exstans";
    absens[I]  = NIHIL;
    (vacuum)remove(LIBER);
    setenv("FABRICA_LECTIONES", LIBER, I);
    r = processus_exsequi(verum, 5000, piscina);
    CREDO_VERUM(r.successus);
    (vacuum)processus_exsequi(absens, 5000, piscina);
    unsetenv("FABRICA_LECTIONES");
    liber = filum_legere_totum(LIBER, piscina);
    CREDO_VERUM(chorda_continet(liber, chorda_ex_literis(
        "P\t/usr/bin/true\n", piscina)));
    CREDO_VERUM(chorda_continet(liber, chorda_ex_literis(
        "P\tprogramma_nusquam_exstans\n", piscina)));
    /* sine libro: nihil notatur */
    (vacuum)remove(LIBER);
    r = processus_exsequi(verum, 5000, piscina);
    CREDO_VERUM(r.successus);
    CREDO_FALSUM(filum_existit(LIBER));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "I. variabilis absens: nihil scribitur", _sectio_prima, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "II. variabilis praesens: L, A, X", _sectio_secunda, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "IIb. scriptura (S) et ambitus (E)", _sectio_tertia, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "IId. exsecutio (P) ante fork", _sectio_exsecutionis, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "IIc. fopen et stat per librum (plan 5 T5a)", _sectio_quarta,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "III. filii duo paralleli: lineae integrae", _sectio_quinta,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("lectiones", SECTIONES);
}
