/* probatio_iter_directoria.c - Probationes Iterationis Directoriorum */
#include "latina.h"
#include "iter_directoria.h"
#include "piscina.h"
#include "credo.h"
#include "filum.h"
#include "lectiones.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

/* Via ad directorium temporarium pro probationibus */
hic_manens constans character* VIA_PROBATIONIS =
    "/tmp/rhubarb_iter_probatio";

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
hic_manens vacuum
_creare_structuram_probationis (
    CredoContextus* c)
{
    (vacuum)c;

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
}

/* Purgare structuram probationis */
hic_manens vacuum
_purgare_structuram_probationis (
    CredoContextus* c)
{
    (vacuum)c;
    system("rm -rf /tmp/rhubarb_iter_probatio");
}

/* Callback pro ambulatore - numerare introitus */
hic_manens s32
_callback_numerare (
                           chorda  via_plena,
    constans DirectoriumIntroitus* introitus,
                           vacuum* contextus)
{
    AmbulatorContextus* ctx = (AmbulatorContextus*)contextus;

    (vacuum)via_plena; /* Non usus */

    ctx->numerus_totalis++;

    si (introitus->genus == INTROITUS_FILUM)
    {
        ctx->numerus_filorum++;
    } alioquin si (introitus->genus == INTROITUS_DIRECTORIUM)
    {
        ctx->numerus_directoriorum++;
    }

    redde 0; /* Perge */
}

/* Callback pro terminatione praematura */
hic_manens s32
_callback_terminare_post_tres (
                           chorda  via_plena,
    constans DirectoriumIntroitus* introitus,
                           vacuum* contextus)
{
    i32* numerus = (i32*)contextus;

    (vacuum)via_plena;
    (vacuum)introitus;

    (*numerus)++;

    si (*numerus >= III)
    {
        redde I; /* Termina */
    }

    redde 0; /* Perge */
}


/* ==================================================
 * Probationes
 * ================================================== */


    /* ==================================================
     * Probare: Iteratio Simplex
     * ================================================== */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
         DirectoriumIterator* iter;
        DirectoriumIntroitus* e;
                         i32  numerus;

        iter = directorium_iterator_aperire(VIA_PROBATIONIS, piscina);
        CREDO_NON_NIHIL(iter);

        numerus = 0;
        dum ((e = directorium_iterator_proximum(iter)) != NIHIL)
        {
            numerus++;
            CREDO_CHORDA_NON_VACUA(e->titulus);
        }

        /* Debet esse: file1.txt, file2.txt, readme.md, sub1, sub2,
           .hidden_file, .hidden_dir = 7 introitus */
        CREDO_AEQUALIS_I32(numerus, VII);

        directorium_iterator_claudere(iter);
    }
}


    /* ==================================================
     * Probare: Directorium Non Existens
     * ================================================== */

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        DirectoriumIterator* iter;

        iter = directorium_iterator_aperire("/non/existens/via",
            piscina);
        CREDO_NIHIL(iter);
    }
}


    /* ==================================================
     * Probare: Genera Introituum
     * ================================================== */

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
         DirectoriumIterator* iter;
        DirectoriumIntroitus* e;
                         i32  fila;
                         i32  directoria;

        iter = directorium_iterator_aperire(VIA_PROBATIONIS, piscina);
        CREDO_NON_NIHIL(iter);

        fila        = 0;
        directoria  = 0;

        dum ((e = directorium_iterator_proximum(iter)) != NIHIL)
        {
            si (e->genus == INTROITUS_FILUM)
            {
                fila++;
            } alioquin si (e->genus == INTROITUS_DIRECTORIUM)
            {
                directoria++;
            }
        }

        /* 4 fila: file1.txt, file2.txt, readme.md, .hidden_file */
        CREDO_AEQUALIS_I32(fila, IV);

        /* 3 directoria: sub1, sub2, .hidden_dir */
        CREDO_AEQUALIS_I32(directoria, III);

        directorium_iterator_claudere(iter);
    }
}


    /* ==================================================
     * Probare: Mensura Fili
     * ================================================== */

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
         DirectoriumIterator* iter;
        DirectoriumIntroitus* e;
                         b32  inventum;

        iter = directorium_iterator_aperire(VIA_PROBATIONIS, piscina);
        CREDO_NON_NIHIL(iter);

        inventum = FALSUM;
        dum ((e = directorium_iterator_proximum(iter)) != NIHIL)
        {
            si (chorda_aequalis_literis(e->titulus, "file1.txt"))
            {
                /* "content1\n" = 9 bytes */
                CREDO_AEQUALIS_I32((i32)e->mensura, IX);
                inventum = VERUM;
            }
        }

        CREDO_VERUM(inventum);
        directorium_iterator_claudere(iter);
    }
}


    /* ==================================================
     * Probare: Pattern Matching - *
     * ================================================== */

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda titulus;
        chorda exemplar;

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
}


    /* ==================================================
     * Probare: Pattern Matching - ?
     * ================================================== */

interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda titulus;
        chorda exemplar;

        titulus = chorda_ex_literis("file.txt", piscina);

        exemplar = chorda_ex_literis("file.???", piscina);
        CREDO_VERUM(directorium_titulus_congruit(titulus, exemplar));

        exemplar = chorda_ex_literis("????.txt", piscina);
        CREDO_VERUM(directorium_titulus_congruit(titulus, exemplar));

        exemplar = chorda_ex_literis("???.txt", piscina);
        CREDO_FALSUM(directorium_titulus_congruit(titulus, exemplar));
    }
}


    /* ==================================================
     * Probare: Pattern Matching - Vacua
     * ================================================== */

interior vacuum
_sectio_septima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda titulus;
        chorda exemplar_vacua;

        titulus = chorda_ex_literis("anything.txt", piscina);
        exemplar_vacua.datum = NIHIL;
        exemplar_vacua.mensura = 0;

        /* Exemplar vacua congruit cum omnibus */
        CREDO_VERUM(directorium_titulus_congruit(titulus,
            exemplar_vacua));
    }
}


    /* ==================================================
     * Probare: directorium_existit
     * ================================================== */

interior vacuum
_sectio_octava (
    CredoContextus* c)
{
    (vacuum)c;
    {
        CREDO_VERUM(directorium_existit(VIA_PROBATIONIS));
        CREDO_VERUM(directorium_existit("/tmp/rhubarb_iter_probatio/sub1"));
        CREDO_FALSUM(directorium_existit("/tmp/rhubarb_iter_probatio/file1.txt"));
        CREDO_FALSUM(directorium_existit("/non/existens"));
    }
}


    /* ==================================================
     * Probare: Ambulator Simplex
     * ================================================== */

interior vacuum
_sectio_nona (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        AmbulatorContextus ctx;
                       s32 fructus;

        ctx.numerus_filorum        = 0;
        ctx.numerus_directoriorum  = 0;
        ctx.numerus_totalis        = 0;

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
}


    /* ==================================================
     * Probare: Ambulator cum Filtro - Sine Occultis
     * ================================================== */

interior vacuum
_sectio_decima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        AmbulatorContextus ctx;
        DirectoriumFiltrum filtrum;
                       s32 fructus;

        ctx.numerus_filorum        = 0;
        ctx.numerus_directoriorum  = 0;
        ctx.numerus_totalis        = 0;

        filtrum                     = directorium_filtrum_omnia();
        filtrum.includere_occultos  = FALSUM;

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
}


    /* ==================================================
     * Probare: Ambulator cum Filtro - Pattern
     * ================================================== */

interior vacuum
_sectio_decima_prima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        AmbulatorContextus ctx;
        DirectoriumFiltrum filtrum;
                       s32 fructus;

        ctx.numerus_filorum        = 0;
        ctx.numerus_directoriorum  = 0;
        ctx.numerus_totalis        = 0;

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
}


    /* ==================================================
     * Probare: Ambulator - Terminatio Praematura
     * ================================================== */

interior vacuum
_sectio_decima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        i32 numerus;
        s32 fructus;

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
}


    /* ==================================================
     * Probare: Ambulator - Profunditas Maxima
     * ================================================== */

interior vacuum
_sectio_decima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        AmbulatorContextus ctx;
        DirectoriumFiltrum filtrum;
                       s32 fructus;

        ctx.numerus_filorum        = 0;
        ctx.numerus_directoriorum  = 0;
        ctx.numerus_totalis        = 0;

        filtrum                     = directorium_filtrum_omnia();
        filtrum.profunditas_max     = I; /* Solum primus gradus */
        filtrum.includere_occultos  = FALSUM;

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
}


/* ==================================================
 * ENUMERATIO UNO VOCAMINE (2026-10-22): omnis campus introitus contra
 * stat() - genera, nexus (ad filum, ad directorium, fractus), fistula,
 * nomen longum, et MD fila (massa XXXII KiB pluries impletur)
 * ================================================== */

interior vacuum
_sectio_enumerationis (
    CredoContextus* c)
{
                Piscina* piscina = c->piscina;
              character  radix[CCLVI];
              character  mandatum[MXXIV];
              character  via[MXXIV];
    DirectoriumIterator* it;
   DirectoriumIntroitus* in;
                    i32  numerus        = ZEPHYRUM;
                    i32  discrepantiae  = ZEPHYRUM;
                    i32  nexus_visi     = ZEPHYRUM;
                    b32  fractus_visus  = FALSUM;
                    b32  fistula_visa   = FALSUM;
                    b32  longum_visum   = FALSUM;

    sprintf(radix, "/tmp/rhubarb_iter_enumeratio_%d",
        (integer)getpid());
    sprintf(mandatum, "rm -rf %s && mkdir -p %s/sub && cd %s && "
        "echo abc > filum && echo de > sub/intus && "
        "ln -s filum nexus_fili && ln -s sub nexus_dir && "
        "ln -s absens nexus_fractus && mkfifo fistula && "
        ": > %0200d && touch $(seq -f m%%g 1 1500)",
        radix, radix, radix, 7);
    CREDO_AEQUALIS_I32 ((i32)system(mandatum), ZEPHYRUM);

    it = directorium_iterator_aperire(radix, piscina);
    CREDO_NECESSE_NON_NIHIL (it);
    dum ((in = directorium_iterator_proximum(it)) != NIHIL)
    {
        structura stat st;
              integer r;

        numerus++;
        sprintf(via, "%s/%.*s", radix, (integer)in->titulus.mensura,
            (constans character*)in->titulus.datum);
        r = lectiones_stat(via, &st);
        si (r != 0)
        {
            /* solum nexus fractus stat fallit: IGNOTUS, nexus */
            fractus_visus = (b32)(in->genus == INTROITUS_IGNOTUS
                && in->est_nexus && in->mensura == 0
                && in->tempus_ns == 0);
            si (!fractus_visus)
            {
                discrepantiae++;
            }
            perge;
        }
        si (   in->mensura != (memoriae_index)st.st_size
            || in->tempus_ns != (s64)st.st_mtimespec.tv_sec
                * (s64)M * (s64)M * (s64)M
                + (s64)st.st_mtimespec.tv_nsec
            || in->genus != (S_ISREG(st.st_mode) ? INTROITUS_FILUM
                : S_ISDIR(st.st_mode) ? INTROITUS_DIRECTORIUM
                : INTROITUS_ALIUS))
        {
            discrepantiae++;
        }
        si (in->est_nexus)
        {
            nexus_visi++;
        }
        si (   in->titulus.mensura                       == VII
            && memcmp(in->titulus.datum, "fistula", VII) == 0)
        {
            fistula_visa = (b32)(in->genus == INTROITUS_ALIUS
                && !in->est_nexus);
        }
        si (in->titulus.mensura == CC)
        {
            longum_visum = VERUM;
        }
        /* nexus secutus: ad filum FILUM, ad directorium DIRECTORIUM */
        si (   in->titulus.mensura                        == X
            && memcmp(in->titulus.datum, "nexus_fili", X) == 0)
        {
            CREDO_VERUM (in->est_nexus && in->genus == INTROITUS_FILUM);
        }
        si (   in->titulus.mensura                        == IX
            && memcmp(in->titulus.datum, "nexus_dir", IX) == 0)
        {
            CREDO_VERUM (in->est_nexus
                && in->genus == INTROITUS_DIRECTORIUM);
        }
    }
    directorium_iterator_claudere(it);
    /* filum, sub, III nexus, fistula, nomen longum, MD */
    CREDO_AEQUALIS_I32 (numerus, (i32)(VII + MD));
    CREDO_AEQUALIS_I32 (discrepantiae, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (nexus_visi, II);
    CREDO_VERUM (fractus_visus);
    CREDO_VERUM (fistula_visa);
    CREDO_VERUM (longum_visum);
    sprintf(mandatum, "rm -rf %s", radix);
    (vacuum)system(mandatum);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "iterationem simplicem", _sectio_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "directorium non existens", _sectio_secunda, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "genera introituum", _sectio_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "mensuram fili", _sectio_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "pattern matching (*)", _sectio_quinta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "pattern matching (?)", _sectio_sexta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "pattern matching (vacua)", _sectio_septima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "directorium_existit", _sectio_octava, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "ambulatorem simplicem", _sectio_nona, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "ambulatorem sine occultis", _sectio_decima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "ambulatorem cum pattern *.txt", _sectio_decima_prima, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "terminationem praematuram", _sectio_decima_secunda, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "profunditatem maximam", _sectio_decima_tertia, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "enumerationem uno vocamine", _sectio_enumerationis, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("iter_directoria", SECTIONES,
        _creare_structuram_probationis,
        _purgare_structuram_probationis);
}
