/* sonda capsula_r1: capsula_aperire_e_disco duo cursus (numerare, implere)
 * - plagulae inter cursus additae index[] ultra allocationem scribunt?
 *
 * Methodus DETERMINISTICA: opendir() hic interponitur (symbolum in
 * executabili praevalet; verum per dlsym(RTLD_NEXT)). Vocatione SECUNDA
 * directorii sondae (= initium cursus II) plagulae ADDENDAE creantur -
 * idem quod applicatio evolutionis facit cum editor plagulam scribit
 * dum capsula aperitur (capsula.h: modus disci = directorium vivum).
 *
 * argv[1] == "angusta": piscina alveo VIII byteorum, ut index ad finem
 * alvei malloc iaceat -> ASan scripturam ultra finem videt.
 * aliter: piscina 65536; probatio geometrica - index[] cum allocationibus
 * POSTERIORIBUS (copiae viarum cursus II) se tegit. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "capsula.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <dlfcn.h>
#if defined(__has_feature)
#if __has_feature(address_sanitizer)
#include <sanitizer/asan_interface.h>
#define HABET_ASAN 1
#endif
#endif

#define DIR_SONDAE "build/sonda/capsula_r1_dir"
#define ORIGINALES 100
#define ADDENDAE   60

hic_manens integer vocationes_sondae = 0;
hic_manens constans character* via_cursus_secundi = NIHIL;
hic_manens character via_ante[256];
hic_manens integer longitudo_viae = 0;

nomen DIR* (*OpendirFunctio)(constans character*);

DIR*
opendir (constans character* via)
{
    hic_manens OpendirFunctio verus = NIHIL;
    si (verus == NIHIL)
    {
        vacuum* sym = dlsym(RTLD_NEXT, "opendir");
        memcpy(&verus, &sym, sizeof(verus));
    }
    si (strstr(via, "capsula_r1_dir") != NIHIL)
    {
        vocationes_sondae++;
        si (vocationes_sondae == 2)
        {
            via_cursus_secundi = via;   /* chorda_ut_cstr in piscina, cursu II, POST index */
            longitudo_viae = (integer)strlen(via);
            memcpy(via_ante, via, (size_t)longitudo_viae + 1);
            integer k;
            character v[128];
            per (k = 0; k < ADDENDAE; k++)
            {
                integer fd;
                snprintf(v, sizeof(v), DIR_SONDAE "/nova_%02d.txt", k);
                fd = open(v, O_CREAT | O_WRONLY, 0644);
                si (fd >= 0) close(fd);
            }
            imprimere("[opendir #2 = initium cursus II: %d plagulae additae]\n",
                (integer)ADDENDAE);
        }
    }
    redde verus(via);
}

integer
principale (integer argc, character** argv)
{
    b32      angusta = (argc > 1 && strcmp(argv[1], "angusta") == 0);
    Piscina* piscina;
    Capsula* cap;
    i8*      initium_indicis;
    i8*      finis_allocati;
    i32      j;
    i32      tecta = 0;
    integer  k;

    (vacuum)system("rm -rf " DIR_SONDAE "; mkdir -p " DIR_SONDAE);
    per (k = 0; k < ORIGINALES; k++)
    {
        character v[128];
        FILE* f;
        snprintf(v, sizeof(v), DIR_SONDAE "/orig_%03d.txt", k);
        f = fopen(v, "w");
        si (f) { fputs("x", f); fclose(f); }
    }

    piscina = piscina_generare_dynamicum("sonda", angusta ? 8 : 65536);
    cap     = capsula_aperire_e_disco(DIR_SONDAE, piscina);
    si (cap == NIHIL)
    {
        imprimere("aperire NIHIL\n");
        redde II;
    }

    initium_indicis = (i8*)cap->index;
    finis_allocati  = (i8*)(cap->index + ORIGINALES);  /* cursus I: ORIGINALES */
    imprimere("cursus I numeravit %d (in disco ante cursum II); "
        "numerus_filorum post cursum II = %d\n",
        (integer)ORIGINALES, (integer)capsula_numerus(cap));
    imprimere("index allocatus [%p, %p) = %d introitus x %lu byte\n",
        (vacuum*)initium_indicis, (vacuum*)finis_allocati,
        (integer)ORIGINALES, (insignatus longus)sizeof(CapsulaIndexum));

#ifdef HABET_ASAN
    imprimere("ASan: index[%d] (ultimus allocatus) venenatus? %d; index[%d] (ultra) venenatus? %d\n",
        (integer)(ORIGINALES - 1), __asan_address_is_poisoned(cap->index + ORIGINALES - 1),
        (integer)ORIGINALES, __asan_address_is_poisoned(cap->index + ORIGINALES));
#endif
    {
        constans character* a = (constans character*)(cap->index + ORIGINALES);
        constans character* b = a + sizeof(CapsulaIndexum);
        integer m, mutata = 0;
        imprimere("index[%d] scriptus in [%p, %p); via_cstr cursus II (opendir arg, "
            "piscina_allocare post index) in [%p, %p)\n", (integer)ORIGINALES,
            (vacuum*)a, (vacuum*)b, (vacuum*)via_cursus_secundi,
            (vacuum*)(via_cursus_secundi + longitudo_viae + 1));
        per (m = 0; m < longitudo_viae; m++)
            si (via_cursus_secundi[m] != via_ante[m]) mutata++;
        imprimere("via_cstr ante: '%s'\n", via_ante);
        imprimere("via_cstr post: %d byte mutati (a capsula, non a nobis)\n", mutata);
    }
    per (j = 0; j < cap->numerus_filorum; j++)
    {
        i8* d = cap->index[j].via.datum;
        si (d >= initium_indicis
            && d < (i8*)(cap->index + cap->numerus_filorum))
        {
            si (tecta < 3)
            {
                imprimere("  index[%d].via.datum = %p jacet INTRA "
                    "index[0..%d) - copia viae (allocata post index) "
                    "ab index[] scripta est\n",
                    (integer)j, (vacuum*)d, (integer)cap->numerus_filorum);
            }
            tecta++;
        }
    }
    imprimere("introitus quorum via INTRA indicem iacet: %d\n", (integer)tecta);
    per (j = 0; j < cap->numerus_filorum && j < 6; j++)
    {
        imprimere("  index[%d].via = '%.*s'\n", (integer)j,
            (integer)cap->index[j].via.mensura,
            (constans character*)cap->index[j].via.datum);
    }

    piscina_destruere(piscina);
    (vacuum)system("rm -rf " DIR_SONDAE);
    redde tecta > 0 ? I : 0;
}
