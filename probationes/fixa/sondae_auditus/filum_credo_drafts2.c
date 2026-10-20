#include "postulata_posix.h"
#include "latina.h"
#include "filum.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

s32 principale (vacuum)
{
    Piscina* piscina;
    piscina = piscina_generare_dynamicum("test_filum", 8192);
    credo_aperire(piscina);
    {
        constans character* via = "/tmp/test_rhubarb_filum_magnum.txt";
        hic_manens character magnum[X * M];
        chorda contentum;
        pid_t filius;
        integer status_filii;
        imprimere("\n--- D10 ---\n");
        memset(magnum, 'x', magnitudo(magnum));
        contentum.datum   = (i8*)magnum;
        contentum.mensura = X * M;
        CREDO_VERUM(filum_scribere(via, contentum));
        filius = fork();
        si (filius == ZEPHYRUM)
        {
            Piscina* certa;
            chorda lectum;
            certa  = piscina_generare_certae_magnitudinis("filum_certa", CCLVI);
            lectum = filum_legere_totum(via, certa);
            _exit(lectum.mensura == ZEPHYRUM ? XLII : ZEPHYRUM);
        }
        CREDO_VERUM(waitpid(filius, &status_filii, ZEPHYRUM) == filius);
        CREDO_VERUM(WIFEXITED(status_filii));
        CREDO_AEQUALIS_S32((s32)WEXITSTATUS(status_filii), XLII);
        (vacuum)filum_delere(via);
    }
    {
        constans character* via_serae = "/tmp/test_rhubarb_filum.sera";
        FilumSera* sera;
        imprimere("\n--- D12b ---\n");
        sera = filum_seram_capere(via_serae, ZEPHYRUM, piscina);
        CREDO_NON_NIHIL(sera);
        CREDO_VERUM(filum_directorium_creare_si_necesse("/tmp"));
        CREDO_NIHIL(filum_error_recens());
        CREDO_NIHIL(filum_seram_capere(via_serae, ZEPHYRUM, piscina));
        CREDO_NON_NIHIL(filum_error_recens());
        filum_seram_liberare(sera);
        (vacuum)filum_delere(via_serae);
    }
    credo_imprimere_compendium();
    credo_claudere();
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
