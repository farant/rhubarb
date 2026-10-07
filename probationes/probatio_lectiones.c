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

s32 principale (vacuum)
{
    Piscina* piscina;
     chorda  liber;
        b32  praeteritus;

    piscina = piscina_generare_dynamicum("probatio_lectiones",
        1048576);
    credo_aperire(piscina);

    imprimere("\n--- I. variabilis absens: nihil scribitur ---\n");
    unsetenv("FABRICA_LECTIONES");
    (vacuum)remove(LIBER);
    (vacuum)filum_legere_totum("probationes/probatio_lectiones.c",
        piscina);
    CREDO_FALSUM(filum_existit(LIBER));

    imprimere("\n--- II. variabilis praesens: L, A, X ---\n");
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

    imprimere("\n--- IIb. scriptura (S) et ambitus (E) ---\n");
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

    imprimere("\n--- IIc. fopen et stat per librum (plan 5 T5a) ---\n");
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

    imprimere("\n--- III. filii duo paralleli: lineae integrae ---\n");
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

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
