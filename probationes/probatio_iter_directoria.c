/* probatio_iter_directoria.c - Probationes Iterationis Directoriorum */
#include "latina.h"
#include "iter_directoria.h"
#include "piscina.h"
#include "credo.h"
#include "filum.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/* Via ad directorium temporarium pro probationibus */
hic_manens constans character* VIA_PROBATIONIS = "/tmp/rhubarb_iter_probatio";

/* Contextus pro ambulatore */
nomen structura {
    i32 numerus_filorum;
    i32 numerus_directoriorum;
    i32 numerus_totalis;
} AmbulatorContextus;


/* ==================================================
 * Functiones Auxiliares
 * ================================================== */

/* Creare structuram directorii pro probationibus */
hic_manens b32
_creare_structuram_probationis(vacuum)
{
    /* Creare directorium principale */
    system("rm -rf /tmp/rhubarb_iter_probatio");
    system("mkdir -p /tmp/rhubarb_iter_probatio");
    system("mkdir -p /tmp/rhubarb_iter_probatio/sub1");
    system("mkdir -p /tmp/rhubarb_iter_probatio/sub2");
    system("mkdir -p /tmp/rhubarb_iter_probatio/sub1/deep");
    system("mkdir -p /tmp/rhubarb_iter_probatio/.hidden_dir");

    /* Creare fila */
    system("echo 'content1' > /tmp/rhubarb_iter_probatio/file1.txt");
    system("echo 'content2' > /tmp/rhubarb_iter_probatio/file2.txt");
    system("echo 'markdown' > /tmp/rhubarb_iter_probatio/readme.md");
    system("echo 'sub file' > /tmp/rhubarb_iter_probatio/sub1/subfile.txt");
    system("echo 'deep file' > /tmp/rhubarb_iter_probatio/sub1/deep/deepfile.txt");
    system("echo 'hidden' > /tmp/rhubarb_iter_probatio/.hidden_file");
    system("echo 'hidden sub' > /tmp/rhubarb_iter_probatio/.hidden_dir/inside.txt");

    redde VERUM;
}


/* Purgare structuram probationis */
hic_manens vacuum
_purgare_structuram_probationis(vacuum)
{
    system("rm -rf /tmp/rhubarb_iter_probatio");
}


/* Callback pro ambulatore - numerare introitus */
hic_manens s32
_callback_numerare(
    chorda                         via_plena,
    constans DirectoriumIntroitus* introitus,
    vacuum*                        contextus)
{
    AmbulatorContextus* ctx = (AmbulatorContextus*)contextus;

    (vacuum)via_plena; /* Non usus */

    ctx->numerus_totalis++;

    si (introitus->genus == INTROITUS_FILUM) {
        ctx->numerus_filorum++;
    } alioquin si (introitus->genus == INTROITUS_DIRECTORIUM) {
        ctx->numerus_directoriorum++;
    }

    redde 0; /* Perge */
}


/* Callback pro terminatione praematura */
hic_manens s32
_callback_terminare_post_tres(
    chorda                         via_plena,
    constans DirectoriumIntroitus* introitus,
    vacuum*                        contextus)
{
    i32* numerus = (i32*)contextus;

    (vacuum)via_plena;
    (vacuum)introitus;

    (*numerus)++;

    si (*numerus >= III) {
        redde I; /* Termina */
    }

    redde 0; /* Perge */
}


/* ==================================================
 * Probationes
 * ================================================== */

s32 principale(vacuum)
{
    Piscina* piscina;

    /* Aperire piscina et credo */
    piscina = piscina_generare_dynamicum("test_iter_directoria", 8192);
    si (!piscina) {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    /* Creare structuram probationis */
    _creare_structuram_probationis();


    /* ==================================================
     * Probare: Iteratio Simplex
     * ================================================== */
    {
        DirectoriumIterator* iter;
        DirectoriumIntroitus* e;
        i32 numerus;

        imprimere("\n--- Probans iterationem simplicem ---\n");

        iter = directorium_iterator_aperire(VIA_PROBATIONIS, piscina);
        CREDO_NON_NIHIL(iter);

        numerus = 0;
        dum ((e = directorium_iterator_proximum(iter)) != NIHIL) {
            numerus++;
            CREDO_CHORDA_NON_VACUA(e->titulus);
        }

        /* Debet esse: file1.txt, file2.txt, readme.md, sub1, sub2,
           .hidden_file, .hidden_dir = 7 introitus */
        CREDO_AEQUALIS_I32(numerus, VII);

        directorium_iterator_claudere(iter);
    }


    /* ==================================================
     * Probare: Directorium Non Existens
     * ================================================== */
    {
        DirectoriumIterator* iter;

        imprimere("\n--- Probans directorium non existens ---\n");

        iter = directorium_iterator_aperire("/non/existens/via", piscina);
        CREDO_NIHIL(iter);
    }


    /* ==================================================
     * Probare: Genera Introituum
     * ================================================== */
    {
        DirectoriumIterator* iter;
        DirectoriumIntroitus* e;
        i32 fila;
        i32 directoria;

        imprimere("\n--- Probans genera introituum ---\n");

        iter = directorium_iterator_aperire(VIA_PROBATIONIS, piscina);
        CREDO_NON_NIHIL(iter);

        fila = 0;
        directoria = 0;

        dum ((e = directorium_iterator_proximum(iter)) != NIHIL) {
            si (e->genus == INTROITUS_FILUM) {
                fila++;
            } alioquin si (e->genus == INTROITUS_DIRECTORIUM) {
                directoria++;
            }
        }

        /* 4 fila: file1.txt, file2.txt, readme.md, .hidden_file */
        CREDO_AEQUALIS_I32(fila, IV);

        /* 3 directoria: sub1, sub2, .hidden_dir */
        CREDO_AEQUALIS_I32(directoria, III);

        directorium_iterator_claudere(iter);
    }


    /* ==================================================
     * Probare: Mensura Fili
     * ================================================== */
    {
        DirectoriumIterator* iter;
        DirectoriumIntroitus* e;
        b32 inventum;

        imprimere("\n--- Probans mensuram fili ---\n");

        iter = directorium_iterator_aperire(VIA_PROBATIONIS, piscina);
        CREDO_NON_NIHIL(iter);

        inventum = FALSUM;
        dum ((e = directorium_iterator_proximum(iter)) != NIHIL) {
            si (chorda_aequalis_literis(e->titulus, "file1.txt")) {
                /* "content1\n" = 9 bytes */
                CREDO_AEQUALIS_I32((i32)e->mensura, IX);
                inventum = VERUM;
            }
        }

        CREDO_VERUM(inventum);
        directorium_iterator_claudere(iter);
    }


    /* ==================================================
     * Probare: Pattern Matching - *
     * ================================================== */
    {
        chorda titulus;
        chorda exemplar;

        imprimere("\n--- Probans pattern matching (*) ---\n");

        titulus = chorda_ex_literis("file.txt", piscina);

        exemplar = chorda_ex_literis("*.txt", piscina);
        CREDO_VERUM(directorium_titulus_congruit(titulus, exemplar));

        exemplar = chorda_ex_literis("*.md", piscina);
        CREDO_FALSUM(directorium_titulus_congruit(titulus, exemplar));

        exemplar = chorda_ex_literis("file.*", piscina);
        CREDO_VERUM(directorium_titulus_congruit(titulus, exemplar));

        exemplar = chorda_ex_literis("*", piscina);
        CREDO_VERUM(directorium_titulus_congruit(titulus, exemplar));
    }


    /* ==================================================
     * Probare: Pattern Matching - ?
     * ================================================== */
    {
        chorda titulus;
        chorda exemplar;

        imprimere("\n--- Probans pattern matching (?) ---\n");

        titulus = chorda_ex_literis("file.txt", piscina);

        exemplar = chorda_ex_literis("file.???", piscina);
        CREDO_VERUM(directorium_titulus_congruit(titulus, exemplar));

        exemplar = chorda_ex_literis("????.txt", piscina);
        CREDO_VERUM(directorium_titulus_congruit(titulus, exemplar));

        exemplar = chorda_ex_literis("???.txt", piscina);
        CREDO_FALSUM(directorium_titulus_congruit(titulus, exemplar));
    }


    /* ==================================================
     * Probare: Pattern Matching - Vacua
     * ================================================== */
    {
        chorda titulus;
        chorda exemplar_vacua;

        imprimere("\n--- Probans pattern matching (vacua) ---\n");

        titulus = chorda_ex_literis("anything.txt", piscina);
        exemplar_vacua.datum = NIHIL;
        exemplar_vacua.mensura = 0;

        /* Exemplar vacua congruit cum omnibus */
        CREDO_VERUM(directorium_titulus_congruit(titulus, exemplar_vacua));
    }


    /* ==================================================
     * Probare: directorium_existit
     * ================================================== */
    {
        imprimere("\n--- Probans directorium_existit ---\n");

        CREDO_VERUM(directorium_existit(VIA_PROBATIONIS));
        CREDO_VERUM(directorium_existit("/tmp/rhubarb_iter_probatio/sub1"));
        CREDO_FALSUM(directorium_existit("/tmp/rhubarb_iter_probatio/file1.txt"));
        CREDO_FALSUM(directorium_existit("/non/existens"));
    }


    /* ==================================================
     * Probare: Ambulator Simplex
     * ================================================== */
    {
        AmbulatorContextus ctx;
        s32 fructus;

        imprimere("\n--- Probans ambulatorem simplicem ---\n");

        ctx.numerus_filorum = 0;
        ctx.numerus_directoriorum = 0;
        ctx.numerus_totalis = 0;

        fructus = directorium_ambulare(
            VIA_PROBATIONIS,
            NIHIL,  /* Nullum filtrum */
            _callback_numerare,
            &ctx,
            piscina
        );

        CREDO_AEQUALIS_S32(fructus, 0);

        /* Totalis cum recursione (includendo occultos):
           7 introitus in radice + 1 in sub1 + 1 in sub1/deep + 1 in .hidden_dir = 10 */
        CREDO_MAIOR_I32(ctx.numerus_totalis, VIII);
    }


    /* ==================================================
     * Probare: Ambulator cum Filtro - Sine Occultis
     * ================================================== */
    {
        AmbulatorContextus ctx;
        DirectoriumFiltrum filtrum;
        s32 fructus;

        imprimere("\n--- Probans ambulatorem sine occultis ---\n");

        ctx.numerus_filorum = 0;
        ctx.numerus_directoriorum = 0;
        ctx.numerus_totalis = 0;

        filtrum = directorium_filtrum_omnia();
        filtrum.includere_occultos = FALSUM;

        fructus = directorium_ambulare(
            VIA_PROBATIONIS,
            &filtrum,
            _callback_numerare,
            &ctx,
            piscina
        );

        CREDO_AEQUALIS_S32(fructus, 0);

        /* Sine occultis: file1.txt, file2.txt, readme.md, sub1, sub2,
           subfile.txt, deep, deepfile.txt = 8 */
        CREDO_AEQUALIS_I32(ctx.numerus_totalis, VIII);
    }


    /* ==================================================
     * Probare: Ambulator cum Filtro - Pattern
     * ================================================== */
    {
        AmbulatorContextus ctx;
        DirectoriumFiltrum filtrum;
        s32 fructus;

        imprimere("\n--- Probans ambulatorem cum pattern *.txt ---\n");

        ctx.numerus_filorum = 0;
        ctx.numerus_directoriorum = 0;
        ctx.numerus_totalis = 0;

        filtrum = directorium_filtrum_omnia();
        filtrum.exemplar = chorda_ex_literis("*.txt", piscina);
        filtrum.includere_occultos = FALSUM;

        fructus = directorium_ambulare(
            VIA_PROBATIONIS,
            &filtrum,
            _callback_numerare,
            &ctx,
            piscina
        );

        CREDO_AEQUALIS_S32(fructus, 0);

        /* .txt files sine occultis + directoria:
           file1.txt, file2.txt, subfile.txt, deepfile.txt + sub1, sub2, deep = 7 */
        CREDO_AEQUALIS_I32(ctx.numerus_totalis, VII);
    }


    /* ==================================================
     * Probare: Ambulator - Terminatio Praematura
     * ================================================== */
    {
        i32 numerus;
        s32 fructus;

        imprimere("\n--- Probans terminationem praematuram ---\n");

        numerus = 0;

        fructus = directorium_ambulare(
            VIA_PROBATIONIS,
            NIHIL,
            _callback_terminare_post_tres,
            &numerus,
            piscina
        );

        /* Callback reddit non-0 post 3 */
        CREDO_AEQUALIS_S32(fructus, I);
        CREDO_AEQUALIS_I32(numerus, III);
    }


    /* ==================================================
     * Probare: Ambulator - Profunditas Maxima
     * ================================================== */
    {
        AmbulatorContextus ctx;
        DirectoriumFiltrum filtrum;
        s32 fructus;

        imprimere("\n--- Probans profunditatem maximam ---\n");

        ctx.numerus_filorum = 0;
        ctx.numerus_directoriorum = 0;
        ctx.numerus_totalis = 0;

        filtrum = directorium_filtrum_omnia();
        filtrum.profunditas_max = I; /* Solum primus gradus */
        filtrum.includere_occultos = FALSUM;

        fructus = directorium_ambulare(
            VIA_PROBATIONIS,
            &filtrum,
            _callback_numerare,
            &ctx,
            piscina
        );

        CREDO_AEQUALIS_S32(fructus, 0);

        /* Cum profunditas 1: radix + sub1, sub2 contents
           file1.txt, file2.txt, readme.md, sub1, sub2, subfile.txt, deep = 7
           (deep est directorium in sub1, non recursimus in illud) */
        CREDO_AEQUALIS_I32(ctx.numerus_totalis, VII);
    }


    /* ==================================================
     * Purgare et Imprimere Compendium
     * ================================================== */

    _purgare_structuram_probationis();

    credo_imprimere_compendium();
    credo_claudere();
    piscina_destruere(piscina);

    redde credo_omnia_praeterierunt() ? 0 : I;
}
