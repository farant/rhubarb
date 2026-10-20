#include "postulata_posix.h"
#include "latina.h"
#include "filum.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

s32 principale (vacuum)
{
    Piscina* piscina;
    piscina = piscina_generare_dynamicum("test_filum", 8192);
    credo_aperire(piscina);

    /* D1 arbor nexus/ */
    {
        constans character* radix  = "/tmp/test_rhubarb_arbor_nexus";
        constans character* aliena = "/tmp/test_rhubarb_arbor_nexus/aliena";
        constans character* testis = "/tmp/test_rhubarb_arbor_nexus/aliena/testis.txt";
        constans character* nexus  = "/tmp/test_rhubarb_arbor_nexus/nexus";

        imprimere("\n--- D1 ---\n");
        (vacuum)filum_arborem_delere(radix);
        CREDO_VERUM(filum_directorium_creare(radix));
        CREDO_VERUM(filum_directorium_creare(aliena));
        CREDO_VERUM(filum_scribere_literis(testis, "testis"));
        CREDO_AEQUALIS_S32((s32)symlink("aliena", nexus), ZEPHYRUM);
        (vacuum)filum_arborem_delere("/tmp/test_rhubarb_arbor_nexus/nexus/");
        CREDO_VERUM(filum_existit(testis));
        CREDO_VERUM(filum_directorium_existit(aliena));
        (vacuum)filum_arborem_delere(radix);
    }
    /* D16 arbor x/. */
    {
        constans character* radix = "/tmp/test_rhubarb_arbor_punctum";
        constans character* x     = "/tmp/test_rhubarb_arbor_punctum/x.txt";
        imprimere("\n--- D16 ---\n");
        (vacuum)filum_arborem_delere(radix);
        CREDO_VERUM(filum_directorium_creare(radix));
        CREDO_VERUM(filum_scribere_literis(x, "x"));
        CREDO_FALSUM(filum_arborem_delere("/tmp/test_rhubarb_arbor_punctum/."));
        CREDO_VERUM(filum_existit(x));
        (vacuum)filum_arborem_delere(radix);
    }
    /* D2 copiare idem */
    {
        constans character* via = "/tmp/test_rhubarb_copiare_idem.txt";
        chorda lectum;
        imprimere("\n--- D2 ---\n");
        CREDO_VERUM(filum_scribere_literis(via, "CONTENTUM PRETIOSUM"));
        (vacuum)filum_copiare(via, via);
        lectum = filum_legere_totum(via, piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS(lectum, "CONTENTUM PRETIOSUM");
        CREDO_VERUM(filum_scribere_literis(via, "CONTENTUM PRETIOSUM"));
        (vacuum)filum_copiare(via, "/tmp/./test_rhubarb_copiare_idem.txt");
        lectum = filum_legere_totum(via, piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS(lectum, "CONTENTUM PRETIOSUM");
        (vacuum)filum_delere(via);
    }
    /* D3 copiare directorium */
    {
        constans character* dir  = "/tmp/test_rhubarb_copiare_fons_dir";
        constans character* dest = "/tmp/test_rhubarb_copiare_dest.txt";
        chorda lectum;
        imprimere("\n--- D3 ---\n");
        CREDO_VERUM(filum_directorium_creare_si_necesse(dir));
        CREDO_VERUM(filum_scribere_literis(dest, "VETUS DESTINATIO"));
        CREDO_FALSUM(filum_copiare(dir, dest));
        CREDO_NON_NIHIL(filum_error_recens());
        lectum = filum_legere_totum(dest, piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS(lectum, "VETUS DESTINATIO");
        (vacuum)filum_delere(dest);
        (vacuum)filum_delere(dir);
    }
    /* D5/D6/D7 lector */
    {
        constans character* via = "/tmp/test_rhubarb_lector_limites.txt";
        hic_manens character buf[IV * MXXIV + VIII];
        character nul_data[IX];
        FilumLector* lector;
        chorda contentum;
        chorda linea;

        imprimere("\n--- D5 ---\n");
        memset(buf, 'A', IV * MXXIV - II);
        memcpy(buf + IV * MXXIV - II, "\r\nB\n", IV);
        contentum.datum   = (i8*)buf;
        contentum.mensura = IV * MXXIV + II;
        CREDO_VERUM(filum_scribere(via, contentum));
        lector = filum_lector_aperire(via, piscina);
        CREDO_NON_NIHIL(lector);
        CREDO_VERUM(filum_lector_lineam_proximam(lector, &linea));
        CREDO_AEQUALIS_I32(linea.mensura, IV * MXXIV - II);
        CREDO_VERUM(filum_lector_lineam_proximam(lector, &linea));
        CREDO_CHORDA_AEQUALIS_LITERIS(linea, "B");
        CREDO_FALSUM(filum_lector_lineam_proximam(lector, &linea));
        CREDO_AEQUALIS_I32(filum_lector_numerus_versus(lector), II);
        filum_lector_claudere(lector);

        imprimere("\n--- D6 ---\n");
        memset(buf, 'A', IV * MXXIV - II);
        memcpy(buf + IV * MXXIV - II, "\rX\n", III);
        contentum.mensura = IV * MXXIV + I;
        CREDO_VERUM(filum_scribere(via, contentum));
        lector = filum_lector_aperire(via, piscina);
        CREDO_VERUM(filum_lector_lineam_proximam(lector, &linea));
        CREDO_AEQUALIS_I32(linea.mensura, IV * MXXIV);
        CREDO_FALSUM(filum_lector_lineam_proximam(lector, &linea));
        filum_lector_claudere(lector);

        imprimere("\n--- D7 ---\n");
        nul_data[0] = 'a'; nul_data[1] = 'b'; nul_data[2] = '\0';
        nul_data[3] = 'c'; nul_data[4] = 'd'; nul_data[5] = '\n';
        nul_data[6] = 'e'; nul_data[7] = 'f'; nul_data[8] = '\n';
        contentum.datum   = (i8*)nul_data;
        contentum.mensura = IX;
        CREDO_VERUM(filum_scribere(via, contentum));
        lector = filum_lector_aperire(via, piscina);
        CREDO_VERUM(filum_lector_lineam_proximam(lector, &linea));
        CREDO_AEQUALIS_I32(linea.mensura, V);
        CREDO_VERUM(linea.mensura == V && linea.datum[III] == 'c');
        CREDO_VERUM(filum_lector_lineam_proximam(lector, &linea));
        CREDO_CHORDA_AEQUALIS_LITERIS(linea, "ef");
        filum_lector_claudere(lector);

        imprimere("\n--- D8 ---\n");
        CREDO_VERUM(filum_scribere_literis(via, "una\nduae\n"));
        lector = filum_lector_aperire(via, piscina);
        CREDO_VERUM(filum_lector_lineam_proximam(lector, &linea));
        filum_lector_claudere(lector);
        CREDO_NON_RUIT((vacuum)filum_lector_lineam_proximam(lector, &linea));
        (vacuum)filum_delere(via);
    }
    /* D9 FIFO */
    {
        constans character* fifo = "/tmp/test_rhubarb_filum_fifo";
        imprimere("\n--- D9 ---\n");
        (vacuum)filum_delere(fifo);
        CREDO_AEQUALIS_S32((s32)mkfifo(fifo, 0644), ZEPHYRUM);
        CREDO_NON_PENDET((vacuum)filum_existit(fifo), M);
        CREDO_NON_PENDET((vacuum)filum_mensura(fifo), M);
        (vacuum)filum_delere(fifo);
    }
    /* D10 piscina certa */
    {
        constans character* via = "/tmp/test_rhubarb_filum_magnum.txt";
        hic_manens character magnum[X * M];
        Piscina* certa;
        chorda contentum;
        imprimere("\n--- D10 ---\n");
        memset(magnum, 'x', magnitudo(magnum));
        contentum.datum   = (i8*)magnum;
        contentum.mensura = X * M;
        CREDO_VERUM(filum_scribere(via, contentum));
        certa = piscina_generare_certae_magnitudinis("filum_certa", CCLVI);
        CREDO_NON_RUIT((vacuum)filum_legere_totum(via, certa));
        (vacuum)filum_delere(via);
    }
    /* D11 vacuum + D12 error stale */
    {
        constans character* via = "/tmp/test_rhubarb_filum_vacuum.txt";
        chorda lectum;
        FilumSera* sera;
        FilumSera* sera_altera;
        imprimere("\n--- D11 ---\n");
        CREDO_VERUM(filum_scribere_literis(via, ""));
        lectum = filum_legere_totum(via, piscina);
        CREDO_AEQUALIS_I32(lectum.mensura, ZEPHYRUM);
        CREDO_NIHIL(filum_error_recens());
        imprimere("\n--- D12 ---\n");
        (vacuum)filum_legere_totum("/tmp/test_rhubarb_absens_xyz", piscina);
        CREDO_NON_NIHIL(filum_error_recens());
        CREDO_VERUM(filum_existit(via));
        CREDO_NIHIL(filum_error_recens());
        sera = filum_seram_capere("/tmp/test_rhubarb_filum.sera", ZEPHYRUM, piscina);
        CREDO_NON_NIHIL(sera);
        sera_altera = filum_seram_capere("/tmp/test_rhubarb_filum.sera", ZEPHYRUM, piscina);
        CREDO_NIHIL(sera_altera);
        CREDO_NON_NIHIL(filum_error_recens());
        filum_seram_liberare(sera);
        (vacuum)filum_delere(via);
    }
    /* D13 status */
    {
        FilumStatus status;
        imprimere("\n--- D13 ---\n");
        si (getuid() != ZEPHYRUM)
        {
            CREDO_VERUM(filum_status("/", &status));
            CREDO_FALSUM(status.potest_scribere);
        }
    }
    /* D14 mensura dir */
    {
        constans character* dir = "/tmp/test_rhubarb_filum_mensura_dir";
        imprimere("\n--- D14 ---\n");
        CREDO_VERUM(filum_directorium_creare_si_necesse(dir));
        CREDO_AEQUALIS_I32((i32)filum_mensura(dir), ZEPHYRUM);
        (vacuum)filum_delere(dir);
    }
    /* D15 scribere vacua */
    {
        constans character* via = "/tmp/test_rhubarb_filum_scribere_vacua.txt";
        chorda vacua;
        imprimere("\n--- D15 ---\n");
        vacua.datum   = NIHIL;
        vacua.mensura = ZEPHYRUM;
        CREDO_VERUM(filum_scribere_literis(via, "VETUS"));
        CREDO_VERUM(filum_scribere(via, vacua));
        CREDO_AEQUALIS_I32((i32)filum_mensura(via), ZEPHYRUM);
        (vacuum)filum_delere(via);
        CREDO_VERUM(filum_scribere(via, vacua));
        CREDO_VERUM(filum_existit(via));
        CREDO_VERUM(filum_appendere(via, vacua));
        (vacuum)filum_delere(via);
    }
    /* D4 magnum > 4 GiB */
    {
        constans character* via = "/tmp/test_rhubarb_filum_quattuor_gib.bin";
        FILE* f;
        chorda lectum;
        imprimere("\n--- D4 ---\n");
        f = fopen(via, "wb");
        CREDO_NON_NIHIL(f);
        CREDO_AEQUALIS_S32((s32)fseek(f, 4294967296L, SEEK_SET), ZEPHYRUM);
        CREDO_VERUM(fwrite("xyz", I, III, f) == III);
        fclose(f);
        lectum = filum_legere_totum(via, piscina);
        CREDO_AEQUALIS_I32(lectum.mensura, ZEPHYRUM);
        CREDO_NON_NIHIL(filum_error_recens());
        (vacuum)filum_delere(via);
    }
    credo_imprimere_compendium();
    credo_claudere();
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
