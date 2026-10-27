/* probatio_filum.c - Probationes Fili */
#include "postulata_posix.h"
#include "latina.h"
#include "filum.h"
#include "piscina.h"
#include "credo.h"
#include "lectiones.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/un.h>


    /* ==================================================
	 * Probare filum_scribere_literis et filum_legere_totum
	 * ================================================== */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* test_via = "/tmp/test_rhubarb_filum.txt";
        constans character* test_contentum =
            "salve munde\nhoc est probatio\n";
                    chorda legere_contentum;
                    chorda speratus;

        /* Scribere */
        CREDO_VERUM(filum_scribere_literis(test_via, test_contentum));

        /* Legere */
        legere_contentum = filum_legere_totum(test_via, piscina);
        CREDO_VERUM(legere_contentum.mensura > ZEPHYRUM);
        CREDO_NON_NIHIL(legere_contentum.datum);

        /* Verificare contentum */
        speratus = chorda_ex_literis(test_contentum, piscina);
        CREDO_VERUM(chorda_aequalis(legere_contentum, speratus));

        /* Purgare */
        filum_delere(test_via);
    }
}


    /* ==================================================
	 * Probare filum_existit
	 * ================================================== */

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    (vacuum)c;
    {
        constans character* test_via = "/tmp/test_rhubarb_existit.txt";

        /* Non existit ante creationem */
        filum_delere(test_via); /* Purga si existit */
        CREDO_FALSUM(filum_existit(test_via));

        /* Existit post creationem */
        CREDO_VERUM(filum_scribere_literis(test_via, "test"));
        CREDO_VERUM(filum_existit(test_via));

        /* Non existit post deletionem */
        CREDO_VERUM(filum_delere(test_via));
        CREDO_FALSUM(filum_existit(test_via));
    }
}


    /* ==================================================
	 * Probare filum_mensura
	 * ================================================== */

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
    (vacuum)c;
    {
        constans character* test_via =
            "/tmp/test_rhubarb_mensura.txt";
        constans character* contentum = "12345678";
            memoriae_index  mensura;

        filum_scribere_literis(test_via, contentum);
        mensura = filum_mensura(test_via);

        CREDO_AEQUALIS_I32((i32)mensura, VIII);

        filum_delere(test_via);
    }
}


    /* ==================================================
	 * Probare filum_lector_aperire et lineam_proximam
	 * ================================================== */

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* test_via = "/tmp/test_rhubarb_lines.txt";
        constans character* contentum =
            "linea prima\nlinea secunda\nlinea tertia\n";
               FilumLector* lector;
                    chorda  linea;
                    chorda  speratus1, speratus2, speratus3;

        /* Crea filum cum tribus lineis */
        filum_scribere_literis(test_via, contentum);

        /* Aperi lector */
        lector = filum_lector_aperire(test_via, piscina);
        CREDO_NON_NIHIL(lector);

        /* Lege lineas */
        speratus1 = chorda_ex_literis("linea prima", piscina);
        speratus2 = chorda_ex_literis("linea secunda", piscina);
        speratus3 = chorda_ex_literis("linea tertia", piscina);

        /* Prima linea */
        CREDO_VERUM(filum_lector_lineam_proximam(lector, &linea));
        CREDO_AEQUALIS_I32(filum_lector_numerus_versus(lector), I);
        CREDO_VERUM(chorda_aequalis(linea, speratus1));

        /* Secunda linea */
        CREDO_VERUM(filum_lector_lineam_proximam(lector, &linea));
        CREDO_AEQUALIS_I32(filum_lector_numerus_versus(lector), II);
        CREDO_VERUM(chorda_aequalis(linea, speratus2));

        /* Tertia linea */
        CREDO_VERUM(filum_lector_lineam_proximam(lector, &linea));
        CREDO_AEQUALIS_I32(filum_lector_numerus_versus(lector), III);
        CREDO_VERUM(chorda_aequalis(linea, speratus3));

        /* EOF */
        CREDO_FALSUM(filum_lector_lineam_proximam(lector, &linea));
        CREDO_VERUM(filum_lector_finis(lector));

        filum_lector_claudere(lector);
        filum_delere(test_via);
    }
}


    /* ==================================================
	 * Probare filum_appendere
	 * ================================================== */

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* test_via = "/tmp/test_rhubarb_append.txt";
                    chorda  contentum_initial;
                    chorda  contentum_append;
                    chorda  contentum_final;
                    chorda  speratus;

        /* Scribe initial */
        contentum_initial = chorda_ex_literis("prima ", piscina);
        CREDO_VERUM(filum_scribere(test_via, contentum_initial));

        /* Appende */
        contentum_append = chorda_ex_literis("secunda", piscina);
        CREDO_VERUM(filum_appendere(test_via, contentum_append));

        /* Verifica */
        contentum_final  = filum_legere_totum(test_via, piscina);
        speratus         = chorda_ex_literis("prima secunda", piscina);
        CREDO_VERUM(chorda_aequalis(contentum_final, speratus));

        filum_delere(test_via);
    }
}


    /* ==================================================
	 * Probare filum_movere
	 * ================================================== */

interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
    (vacuum)c;
    {
        constans character* via_vetus  = "/tmp/test_rhubarb_vetus.txt";
        constans character* via_nova   = "/tmp/test_rhubarb_nova.txt";
        constans character* contentum  = "probatio movere";

        /* Crea filum */
        filum_scribere_literis(via_vetus, contentum);
        CREDO_VERUM(filum_existit(via_vetus));

        /* Movere */
        CREDO_VERUM(filum_movere(via_vetus, via_nova));
        CREDO_FALSUM(filum_existit(via_vetus));
        CREDO_VERUM(filum_existit(via_nova));

        /* Purga */
        filum_delere(via_nova);
    }
}


    /* ==================================================
	 * Probare filum_copiare
	 * ================================================== */

interior vacuum
_sectio_septima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* via_fons = "/tmp/test_rhubarb_fons.txt";
        constans character* via_dest =
            "/tmp/test_rhubarb_destinatio.txt";
        constans character* contentum = "probatio copiare contentum";
                    chorda  legere_fons;
                    chorda  legere_dest;

        /* Crea fons */
        filum_scribere_literis(via_fons, contentum);

        /* Copia */
        CREDO_VERUM(filum_copiare(via_fons, via_dest));

        /* Verifica ambo existent */
        CREDO_VERUM(filum_existit(via_fons));
        CREDO_VERUM(filum_existit(via_dest));

        /* Verifica contentum aequalis */
        legere_fons = filum_legere_totum(via_fons, piscina);
        legere_dest = filum_legere_totum(via_dest, piscina);
        CREDO_VERUM(chorda_aequalis(legere_fons, legere_dest));

        /* Purga */
        filum_delere(via_fons);
        filum_delere(via_dest);
    }
}


    /* ==================================================
	 * Probare filum_status
	 * ================================================== */

interior vacuum
_sectio_octava (
    CredoContextus* c)
{
    (vacuum)c;
    {
        constans character* test_via   = "/tmp/test_rhubarb_status.txt";
        constans character* contentum  = "probatio status";
               FilumStatus  status;

        /* Crea filum */
        filum_scribere_literis(test_via, contentum);

        /* Cape status */
        CREDO_VERUM(filum_status(test_via, &status));
        CREDO_VERUM(status.est_filum);
        CREDO_FALSUM(status.est_directorium);
        CREDO_VERUM(status.potest_legere);
        CREDO_VERUM(status.potest_scribere);
        CREDO_VERUM(status.mensura > ZEPHYRUM);

        /* Purga */
        filum_delere(test_via);
    }
}


    /* ==================================================
	 * Probare tractatio fili vacui
	 * ================================================== */

interior vacuum
_sectio_nona (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* test_via = "/tmp/test_rhubarb_vacuum.txt";
                    chorda  contentum;

        /* Crea filum vacuum */
        filum_scribere_literis(test_via, "");

        /* Lege */
        contentum = filum_legere_totum(test_via, piscina);
        CREDO_AEQUALIS_I32(contentum.mensura, ZEPHYRUM);

        /* Mensura debet esse zero */
        CREDO_AEQUALIS_I32((i32)filum_mensura(test_via), ZEPHYRUM);

        filum_delere(test_via);
    }
}


    /* ==================================================
	 * Probare NIHIL handling
	 * ================================================== */

interior vacuum
_sectio_decima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda result;

        result = filum_legere_totum(NIHIL, piscina);
        CREDO_AEQUALIS_I32(result.mensura, ZEPHYRUM);

        CREDO_FALSUM(filum_existit(NIHIL));
        CREDO_AEQUALIS_I32((i32)filum_mensura(NIHIL), ZEPHYRUM);
        CREDO_FALSUM(filum_scribere_literis(NIHIL, "probatio"));
    }
}


    /* ==================================================
	 * Probare filum_error_recens
	 * ================================================== */

interior vacuum
_sectio_decima_prima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* error;
                    chorda  result;
                    chorda  contentum;

        /* Error post lectio fili non existentis */
        result =
            filum_legere_totum("/nonexistent/rhubarb/via/filum.txt",
            piscina);
        CREDO_AEQUALIS_I32(result.mensura, ZEPHYRUM);
        error = filum_error_recens();
        CREDO_NON_NIHIL(error);

        /* Post successum, error debet purgari */
        contentum = chorda_ex_literis("probatio", piscina);
        CREDO_VERUM(filum_scribere("/tmp/test_rhubarb_error.txt",
            contentum));
        error = filum_error_recens();
        CREDO_NIHIL(error);

        /* Purgare */
        filum_delere("/tmp/test_rhubarb_error.txt");
    }
}


    /* ==================================================
	 * Probare binary data cum nullis embedded
	 * ================================================== */

interior vacuum
_sectio_decima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* test_via = "/tmp/test_rhubarb_binary.txt";
                        i8  binary_data[] = {0x00, 0x01, (i8)0xFF,
                            0x00,
                    0x7F};
                    chorda contentum;
                    chorda legere;

        contentum.datum    = binary_data;
        contentum.mensura  = V;

        CREDO_VERUM(filum_scribere(test_via, contentum));
        legere = filum_legere_totum(test_via, piscina);

        CREDO_AEQUALIS_I32(legere.mensura, V);
        CREDO_VERUM(memcmp(legere.datum, binary_data, V) == ZEPHYRUM);

        filum_delere(test_via);
    }
}


    /* ==================================================
	 * Probare filum_appendere_literis
	 * ================================================== */

interior vacuum
_sectio_decima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* test_via =
            "/tmp/test_rhubarb_append_literis.txt";
                    chorda contentum_final;
                    chorda speratus;

        /* Scribe initial */
        CREDO_VERUM(filum_scribere_literis(test_via, "prima "));

        /* Appende literis */
        CREDO_VERUM(filum_appendere_literis(test_via, "secunda"));

        /* Verifica */
        contentum_final  = filum_legere_totum(test_via, piscina);
        speratus         = chorda_ex_literis("prima secunda", piscina);
        CREDO_VERUM(chorda_aequalis(contentum_final, speratus));

        filum_delere(test_via);
    }
}


    /* ==================================================
	 * Probare overwrite behavior
	 * ================================================== */

interior vacuum
_sectio_decima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* test_via =
            "/tmp/test_rhubarb_superscribere.txt";
                    chorda result;
                    chorda speratus;

        /* Scribe contentum longum */
        CREDO_VERUM(filum_scribere_literis(test_via,
            "hoc est contentum longum"));

        /* Superscribe cum contentum brevi */
        CREDO_VERUM(filum_scribere_literis(test_via, "breve"));

        result    = filum_legere_totum(test_via, piscina);
        speratus  = chorda_ex_literis("breve", piscina);
        CREDO_VERUM(chorda_aequalis(result, speratus));
        /* Debet esse exacte V bytes, non longitudo vetus */
        CREDO_AEQUALIS_I32(result.mensura, V);

        filum_delere(test_via);
    }
}


    /* ==================================================
	 * Probare lineas vacuas in filo
	 * ================================================== */

interior vacuum
_sectio_decima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* test_via =
            "/tmp/test_rhubarb_lineas_vacuas.txt";
        constans character* contentum = "prima\n\ntertia\n";
               FilumLector* lector;
                    chorda  linea;
                    chorda  speratus_prima;
                    chorda  speratus_tertia;

        filum_scribere_literis(test_via, contentum);
        lector = filum_lector_aperire(test_via, piscina);
        CREDO_NON_NIHIL(lector);

        speratus_prima   = chorda_ex_literis("prima", piscina);
        speratus_tertia  = chorda_ex_literis("tertia", piscina);

        /* Prima linea */
        CREDO_VERUM(filum_lector_lineam_proximam(lector, &linea));
        CREDO_VERUM(chorda_aequalis(linea, speratus_prima));

        /* Linea vacua */
        CREDO_VERUM(filum_lector_lineam_proximam(lector, &linea));
        CREDO_AEQUALIS_I32(linea.mensura, ZEPHYRUM);

        /* Tertia linea */
        CREDO_VERUM(filum_lector_lineam_proximam(lector, &linea));
        CREDO_VERUM(chorda_aequalis(linea, speratus_tertia));

        filum_lector_claudere(lector);
        filum_delere(test_via);
    }
}


    /* ==================================================
	 * Probare fines linearum diversos
	 * ================================================== */

interior vacuum
_sectio_decima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* test_via =
            "/tmp/test_rhubarb_fines_linearum.txt";
                     FILUM* f;
               FilumLector* lector;
                    chorda  linea;
                    chorda  speratus_windows;
                    chorda  speratus_unix;
                    chorda  speratus_mac;

        f = lectiones_fopen(test_via, "wb");
        si (!f)
        {
            imprimere("FRACTA: fopen\n");
            redde;
        }
        fprintf(f, "linea windows\r\n");
        fprintf(f, "linea unix\n");
        fprintf(f, "linea mac vetus\r");
        fclose(f);

        lector = filum_lector_aperire(test_via, piscina);
        CREDO_NON_NIHIL(lector);

        speratus_windows  = chorda_ex_literis("linea windows", piscina);
        speratus_unix     = chorda_ex_literis("linea unix", piscina);
        speratus_mac = chorda_ex_literis("linea mac vetus",
            piscina);

        /* Linea Windows - debet removere \r\n */
        CREDO_VERUM(filum_lector_lineam_proximam(lector, &linea));
        CREDO_VERUM(chorda_aequalis(linea, speratus_windows));

        /* Linea Unix - debet removere \n */
        CREDO_VERUM(filum_lector_lineam_proximam(lector, &linea));
        CREDO_VERUM(chorda_aequalis(linea, speratus_unix));

        /* Linea Mac vetus - debet removere \r */
        CREDO_VERUM(filum_lector_lineam_proximam(lector, &linea));
        CREDO_VERUM(chorda_aequalis(linea, speratus_mac));

        filum_lector_claudere(lector);
        filum_delere(test_via);
    }
}


    /* ==================================================
	 * Probare filum sine newline finali (multi-linea)
	 * ================================================== */

interior vacuum
_sectio_decima_septima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* test_via =
            "/tmp/test_rhubarb_sine_newline_finali.txt";
                     FILUM* f;
               FilumLector* lector;
                    chorda  linea;
                    chorda  speratus1;
                    chorda  speratus2;

        f = lectiones_fopen(test_via, "w");
        si (!f)
        {
            imprimere("FRACTA: fopen\n");
            redde;
        }
        fprintf(f, "linea1\n");
        fprintf(f, "linea2");  /* sine newline */
        fclose(f);

        lector = filum_lector_aperire(test_via, piscina);
        CREDO_NON_NIHIL(lector);

        speratus1 = chorda_ex_literis("linea1", piscina);
        speratus2 = chorda_ex_literis("linea2", piscina);

        /* Prima linea */
        CREDO_VERUM(filum_lector_lineam_proximam(lector, &linea));
        CREDO_VERUM(chorda_aequalis(linea, speratus1));

        /* Secunda linea sine newline */
        CREDO_VERUM(filum_lector_lineam_proximam(lector, &linea));
        CREDO_VERUM(chorda_aequalis(linea, speratus2));

        /* EOF */
        CREDO_FALSUM(filum_lector_lineam_proximam(lector, &linea));

        filum_lector_claudere(lector);
        filum_delere(test_via);
    }
}


    /* ==================================================
	 * Probare aperire filum non existens
	 * ================================================== */

interior vacuum
_sectio_decima_octava (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
               FilumLector* lector;
        constans character* error;

        lector = filum_lector_aperire("/nonexistent/rhubarb/via.txt",
            piscina);
        CREDO_NIHIL(lector);

        error = filum_error_recens();
        CREDO_NON_NIHIL(error);
    }
}


    /* ==================================================
	 * Probare comportamentum post EOF
	 * ================================================== */

interior vacuum
_sectio_decima_nona (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* test_via =
            "/tmp/test_rhubarb_comportamentum_eof.txt";
        constans character* contentum = "linea sola\n";
               FilumLector* lector;
                    chorda  linea;

        filum_scribere_literis(test_via, contentum);
        lector = filum_lector_aperire(test_via, piscina);
        CREDO_NON_NIHIL(lector);

        /* Lege lineam solam */
        CREDO_VERUM(filum_lector_lineam_proximam(lector, &linea));

        /* Primus EOF */
        CREDO_FALSUM(filum_lector_lineam_proximam(lector, &linea));
        CREDO_VERUM(filum_lector_finis(lector));

        /* Voca iterum - debet adhuc reddere FALSUM tute */
        CREDO_FALSUM(filum_lector_lineam_proximam(lector, &linea));
        CREDO_VERUM(filum_lector_finis(lector));

        /* Et iterum - debet esse tutum vocare repetenter */
        CREDO_FALSUM(filum_lector_lineam_proximam(lector, &linea));

        filum_lector_claudere(lector);
        filum_delere(test_via);
    }
}


    /* ==================================================
	 * Probare limites longitudinis linearum - 4096 byte buffer
	 * ================================================== */

interior vacuum
_sectio_vicesima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* test_via =
            "/tmp/test_rhubarb_lineas_longas.txt";
               FilumLector* lector;
                    chorda  linea;
                       i32  i;
                 character* buffer_longus;

        /* Genera lineam 4095 bytes longam (sine newline) - ex piscina */
        buffer_longus = (character*)piscina_allocare(piscina, 5001);
        si (!buffer_longus)
        {
            imprimere("FRACTA: piscina_allocare\n");
            redde;
        }

        /* Probatio 1: Linea 4095 bytes (debet capere exacte cum terminatore nullo) */
        imprimere("\n  Probatio: linea 4095 bytes\n");
        per (i = ZEPHYRUM; i < 4095; i++)
        {
            buffer_longus[i] = 'A';
        }
        buffer_longus[4095] = '\0';

        filum_scribere_literis(test_via, buffer_longus);
        lector = filum_lector_aperire(test_via, piscina);
        CREDO_NON_NIHIL(lector);

        /* Debet legere totam lineam semel */
        CREDO_VERUM(filum_lector_lineam_proximam(lector, &linea));
        CREDO_AEQUALIS_I32(linea.mensura, 4095);

        /* Debet esse EOF */
        CREDO_FALSUM(filum_lector_lineam_proximam(lector, &linea));
        CREDO_VERUM(filum_lector_finis(lector));

        filum_lector_claudere(lector);


        /* Probatio 2: Linea 5000 bytes - crescentia dynamica tractat transparenter */
        imprimere("  Probatio: linea 5000 bytes (crescentia dynamica)\n");
        per (i = ZEPHYRUM; i < 5000; i++)
        {
            buffer_longus[i] = 'B';
        }
        buffer_longus[5000] = '\0';

        filum_scribere_literis(test_via, buffer_longus);
        lector = filum_lector_aperire(test_via, piscina);
        CREDO_NON_NIHIL(lector);

        /* Debet legere totam lineam semel (5000 bytes) */
        CREDO_VERUM(filum_lector_lineam_proximam(lector, &linea));
        imprimere("    Linea legere: %d bytes\n", linea.mensura);
        CREDO_AEQUALIS_I32(linea.mensura, 5000);

        /* Debet esse EOF */
        CREDO_FALSUM(filum_lector_lineam_proximam(lector, &linea));

        filum_lector_claudere(lector);


        /* Probatio 3: Lineas multiplices cum newlines, una longa */
        imprimere("  Probatio: Longitudines linearum mixtae cum newlines\n");
        {
            FILUM* f = lectiones_fopen(test_via, "w");
            si (!f)
            {
                imprimere("FRACTA: fopen\n");
                redde;
            }

            /* Prima linea: brevis */
            fprintf(f, "linea brevis\n");

            /* Secunda linea: 5000 bytes */
            per (i = ZEPHYRUM; i < 5000; i++)
            {
                fputc('C', f);
            }
            fprintf(f, "\n");

            /* Tertia linea: brevis */
            fprintf(f, "alia linea brevis\n");

            fclose(f);
        }

        lector = filum_lector_aperire(test_via, piscina);
        CREDO_NON_NIHIL(lector);

        /* Prima linea: brevis */
        CREDO_VERUM(filum_lector_lineam_proximam(lector, &linea));
        imprimere("    Linea 1: %d bytes\n", linea.mensura);
        CREDO_AEQUALIS_I32(linea.mensura, XII); /* "linea brevis" */

        /* Secunda linea: 5000 bytes (longa, sed transparenter tractata) */
        CREDO_VERUM(filum_lector_lineam_proximam(lector, &linea));
        imprimere("    Linea 2: %d bytes\n", linea.mensura);
        CREDO_AEQUALIS_I32(linea.mensura, 5000);

        /* Tertia linea: brevis */
        CREDO_VERUM(filum_lector_lineam_proximam(lector, &linea));
        imprimere("    Linea 3: %d bytes\n", linea.mensura);
        CREDO_AEQUALIS_I32(linea.mensura, XVII); /* "alia linea brevis" */

        /* EOF */
        CREDO_FALSUM(filum_lector_lineam_proximam(lector, &linea));

        filum_lector_claudere(lector);
        filum_delere(test_via);

        imprimere("\n  NOTA: Lineae > 4095 bytes transparenter tractantur\n");
        imprimere("        ChordaAedificator crescit dynamice pro lineis longis\n");
        imprimere("        (buffer_longus ex piscina, liberabitur cum piscina_destruere)\n");
    }
}


    /* ==================================================
	 * Probare FilumScriptor - scriptio continua
	 * ================================================== */

interior vacuum
_sectio_vicesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
          constans character* test_via =
              "/tmp/test_rhubarb_scriptor.txt";
               FilumScriptor* scriptor;
                      chorda  contentum;
                      chorda  speratus;

        /* Aperire in modus creare */
        scriptor = filum_scriptor_aperire(test_via, FILUM_MODUS_CREARE,
            piscina);
        CREDO_NON_NIHIL(scriptor);

        /* Scribere literis */
        CREDO_VERUM(filum_scriptor_scribere_literis(scriptor,
            "prima "));
        CREDO_VERUM(filum_scriptor_scribere_literis(scriptor,
            "secunda"));

        /* Claudere */
        filum_scriptor_claudere(scriptor);

        /* Verificare */
        contentum  = filum_legere_totum(test_via, piscina);
        speratus   = chorda_ex_literis("prima secunda", piscina);
        CREDO_VERUM(chorda_aequalis(contentum, speratus));

        filum_delere(test_via);
    }
}


    /* ==================================================
	 * Probare FilumScriptor - modus appendere
	 * ================================================== */

interior vacuum
_sectio_vicesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* test_via =
            "/tmp/test_rhubarb_scriptor_append.txt";
               FilumScriptor* scriptor;
                      chorda  contentum;
                      chorda  speratus;

        /* Prima scriptio - creare */
        scriptor = filum_scriptor_aperire(test_via, FILUM_MODUS_CREARE,
            piscina);
        CREDO_NON_NIHIL(scriptor);
        CREDO_VERUM(filum_scriptor_scribere_literis(scriptor, "prima"));
        filum_scriptor_claudere(scriptor);

        /* Secunda scriptio - appendere */
        scriptor = filum_scriptor_aperire(test_via,
            FILUM_MODUS_APPENDERE,
            piscina);
        CREDO_NON_NIHIL(scriptor);
        CREDO_VERUM(filum_scriptor_scribere_literis(scriptor,
            " secunda"));
        filum_scriptor_claudere(scriptor);

        /* Verificare */
        contentum  = filum_legere_totum(test_via, piscina);
        speratus   = chorda_ex_literis("prima secunda", piscina);
        CREDO_VERUM(chorda_aequalis(contentum, speratus));

        filum_delere(test_via);
    }
}


    /* ==================================================
	 * Probare FilumScriptor - lineam_scribere
	 * ================================================== */

interior vacuum
_sectio_vicesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* test_via =
            "/tmp/test_rhubarb_scriptor_lineas.txt";
               FilumScriptor* scriptor;
                 FilumLector* lector;
                      chorda  linea;
                      chorda speratus1, speratus2, speratus3;

        /* Scribere tres lineas */
        scriptor = filum_scriptor_aperire(test_via, FILUM_MODUS_CREARE,
            piscina);
        CREDO_NON_NIHIL(scriptor);

        CREDO_VERUM(filum_scriptor_lineam_scribere(scriptor,
            "linea prima"));
        CREDO_VERUM(filum_scriptor_lineam_scribere(scriptor,
            "linea secunda"));
        CREDO_VERUM(filum_scriptor_lineam_scribere(scriptor,
            "linea tertia"));

        filum_scriptor_claudere(scriptor);

        /* Legere et verificare */
        lector = filum_lector_aperire(test_via, piscina);
        CREDO_NON_NIHIL(lector);

        speratus1 = chorda_ex_literis("linea prima", piscina);
        speratus2 = chorda_ex_literis("linea secunda", piscina);
        speratus3 = chorda_ex_literis("linea tertia", piscina);

        CREDO_VERUM(filum_lector_lineam_proximam(lector, &linea));
        CREDO_VERUM(chorda_aequalis(linea, speratus1));

        CREDO_VERUM(filum_lector_lineam_proximam(lector, &linea));
        CREDO_VERUM(chorda_aequalis(linea, speratus2));

        CREDO_VERUM(filum_lector_lineam_proximam(lector, &linea));
        CREDO_VERUM(chorda_aequalis(linea, speratus3));

        CREDO_FALSUM(filum_lector_lineam_proximam(lector, &linea));

        filum_lector_claudere(lector);
        filum_delere(test_via);
    }
}


    /* ==================================================
	 * Probare FilumScriptor - scribere chorda
	 * ================================================== */

interior vacuum
_sectio_vicesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* test_via =
            "/tmp/test_rhubarb_scriptor_chorda.txt";
              FilumScriptor* scriptor;
                      chorda contentum1, contentum2;
                      chorda legere;
                      chorda speratus;

        scriptor = filum_scriptor_aperire(test_via, FILUM_MODUS_CREARE,
            piscina);
        CREDO_NON_NIHIL(scriptor);

        contentum1 = chorda_ex_literis("chorda ", piscina);
        contentum2 = chorda_ex_literis("probatio", piscina);

        CREDO_VERUM(filum_scriptor_scribere(scriptor, contentum1));
        CREDO_VERUM(filum_scriptor_scribere(scriptor, contentum2));

        filum_scriptor_claudere(scriptor);

        legere    = filum_legere_totum(test_via, piscina);
        speratus  = chorda_ex_literis("chorda probatio", piscina);
        CREDO_VERUM(chorda_aequalis(legere, speratus));

        filum_delere(test_via);
    }
}


    /* ==================================================
	 * Probare FilumScriptor - sync
	 * ================================================== */

interior vacuum
_sectio_vicesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
         constans character* test_via =
             "/tmp/test_rhubarb_scriptor_sync.txt";
              FilumScriptor* scriptor;

        scriptor = filum_scriptor_aperire(test_via, FILUM_MODUS_CREARE,
            piscina);
        CREDO_NON_NIHIL(scriptor);

        CREDO_VERUM(filum_scriptor_scribere_literis(scriptor,
            "ante sync"));
        CREDO_VERUM(filum_scriptor_sync(scriptor));
        CREDO_VERUM(filum_scriptor_scribere_literis(scriptor,
            " post sync"));

        filum_scriptor_claudere(scriptor);
        filum_delere(test_via);
    }
}


    /* ==================================================
	 * Probare FilumScriptor - NIHIL handling
	 * ================================================== */

interior vacuum
_sectio_vicesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        FilumScriptor* scriptor;

        /* Aperire cum NIHIL via */
        scriptor = filum_scriptor_aperire(NIHIL, FILUM_MODUS_CREARE,
            piscina);
        CREDO_NIHIL(scriptor);

        /* Scribere ad NIHIL scriptor */
        CREDO_FALSUM(filum_scriptor_scribere_literis(NIHIL, "test"));
        CREDO_FALSUM(filum_scriptor_lineam_scribere(NIHIL, "test"));
        CREDO_FALSUM(filum_scriptor_sync(NIHIL));

        /* Claudere NIHIL debet esse tutum */
        filum_scriptor_claudere(NIHIL);
    }
}


    /* ==================================================
	 * Probare FilumScriptor - multas lineas (event log simulation)
	 * ================================================== */

interior vacuum
_sectio_vicesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* test_via =
            "/tmp/test_rhubarb_scriptor_eventlog.txt";
               FilumScriptor* scriptor;
                 FilumLector* lector;
                      chorda  linea;
                         i32  i;
                         i32  numerus_lineas;

        scriptor = filum_scriptor_aperire(test_via, FILUM_MODUS_CREARE,
            piscina);
        CREDO_NON_NIHIL(scriptor);

        /* Scribere 100 eventus */
        per (i = ZEPHYRUM; i < C; i++)
        {
            character buffer[LXIV];
            sprintf(buffer, "E|entity_%03d|Page", i);
            CREDO_VERUM(filum_scriptor_lineam_scribere(scriptor,
                buffer));
        }

        filum_scriptor_claudere(scriptor);

        /* Legere et computare lineas */
        lector = filum_lector_aperire(test_via, piscina);
        CREDO_NON_NIHIL(lector);

        numerus_lineas = ZEPHYRUM;
        dum (filum_lector_lineam_proximam(lector, &linea))
        {
            numerus_lineas++;
        }

        CREDO_AEQUALIS_I32(numerus_lineas, C);

        filum_lector_claudere(lector);
        filum_delere(test_via);
    }
}

    /* ---- modus: ponere, et bit exsequendi in statu ---- */

interior vacuum
_sectio_vicesima_octava (
    CredoContextus* c)
{
    (vacuum)c;
    {
        constans character* via_modi =
            "/tmp/probatio_filum_modus.txt";
        constans character* via_copiae =
            "/tmp/probatio_filum_copia.txt";
                FilumStatus st;

        /* STATUS DETERMINATUS: fopen("w") modum NON restituit, ergo
         * plagula ex cursu interrupto cum 0755 relicta assertionem
         * primam frangeret de causa aliena */
        filum_delere(via_modi);
        filum_delere(via_copiae);

        CREDO_VERUM(filum_scribere_literis(via_modi, "x"));
        CREDO_VERUM(filum_status(via_modi, &st));
        CREDO_FALSUM(st.potest_exsequi);

        CREDO_VERUM(filum_modum_ponere(via_modi, 0755));
        CREDO_VERUM(filum_status(via_modi, &st));
        CREDO_VERUM(st.potest_exsequi);
        CREDO_VERUM(st.potest_legere);
        CREDO_VERUM(st.potest_scribere);

        /* modus restrictus: bit exsequendi iterum abest */
        CREDO_VERUM(filum_modum_ponere(via_modi, 0644));
        CREDO_VERUM(filum_status(via_modi, &st));
        CREDO_FALSUM(st.potest_exsequi);

        /* argumenta mala RECUSANTUR, non tacent */
        CREDO_FALSUM(filum_modum_ponere(
            "/tmp/probatio_filum_nusquam_xyz", 0755));
        CREDO_FALSUM(filum_modum_ponere(NIHIL, 0755));

        /* CAUSA huius paris: copia modum NON servat */
        CREDO_VERUM(filum_modum_ponere(via_modi, 0755));
        CREDO_VERUM(filum_copiare(via_modi, via_copiae));
        CREDO_VERUM(filum_status(via_copiae, &st));
        CREDO_FALSUM(st.potest_exsequi);

        filum_delere(via_copiae);
        filum_delere(via_modi);
    }
}

    /* ---- directoria nidificata: semita tota uno actu ---- */

interior vacuum
_sectio_vicesima_nona (
    CredoContextus* c)
{
    (vacuum)c;
    {
        constans character* radix_areae =
            "/tmp/probatio_filum_semita";
        constans character* primum =
            "/tmp/probatio_filum_semita/a";
        constans character* medium =
            "/tmp/probatio_filum_semita/a/b";
        constans character* nidus =
            "/tmp/probatio_filum_semita/a/b/c";
        constans character* via_fili =
            "/tmp/probatio_filum_semita/a/b/c/opus.txt";

        /* STATUS DETERMINATUS: arborem cursus prioris tollere,
         * altissima primum - aliter 'creavit' nihil probat, quia
         * directoria iam adessent (porta viridis de causa falsa) */
        filum_delere(via_fili);
        filum_delere(nidus);
        filum_delere(medium);
        filum_delere(primum);
        filum_delere(radix_areae);
        CREDO_FALSUM(filum_directorium_existit(radix_areae));

        /* CAUSA huius functionis, pinnata: creare_si_necesse UNUM
         * gradum facit (mkdir unus), ergo in semita virgine
         * nidificata DEFICIT. Si quis eam recursivam faciat, haec
         * assertio mutationem NOMINAT. */
        CREDO_FALSUM(filum_directorium_creare_si_necesse(nidus));
        CREDO_FALSUM(filum_directorium_existit(nidus));

        /* semita tota uno actu */
        CREDO_VERUM(filum_directorium_creare_cum_parentibus(nidus));
        CREDO_VERUM(filum_directorium_existit(primum));
        CREDO_VERUM(filum_directorium_existit(medium));
        CREDO_VERUM(filum_directorium_existit(nidus));

        /* IDEMPOTENS: iterum vocata VERUM reddit */
        CREDO_VERUM(filum_directorium_creare_cum_parentibus(nidus));
        CREDO_VERUM(filum_directorium_existit(nidus));

        /* directoria VERA sunt, non nomina sola */
        CREDO_VERUM(filum_scribere_literis(via_fili, "opus"));
        CREDO_VERUM(filum_existit(via_fili));

        /* argumenta mala RECUSANTUR, non tacent */
        CREDO_FALSUM(filum_directorium_creare_cum_parentibus(NIHIL));

        /* filum_delere directorium VACUUM tollit, quia remove() est non
         * unlink(): fundamentum purgationis supra et in fasciculo.
         * NOTA: C89 remove() de directorio implementationi relinquit;
         * POSIX ad rmdir mittit. Morem platformae consulto pinnamus. */
        filum_delere(via_fili);
        CREDO_VERUM(filum_delere(nidus));
        CREDO_FALSUM(filum_directorium_existit(nidus));
        CREDO_VERUM(filum_directorium_existit(medium));

        CREDO_VERUM(filum_delere(medium));
        CREDO_VERUM(filum_delere(primum));
        CREDO_VERUM(filum_delere(radix_areae));
        CREDO_FALSUM(filum_directorium_existit(radix_areae));
    }
}


    /* ==================================================
     * Probare filum_arborem_delere (nexum NON sequitur)
     * ================================================== */

interior vacuum
_sectio_tricesima (
    CredoContextus* c)
{
    (vacuum)c;
    {
        constans character* radix   = "/tmp/test_rhubarb_arbor";
        constans character* aliena  = "/tmp/test_rhubarb_arbor_aliena";
        constans character* testis =
            "/tmp/test_rhubarb_arbor_aliena/testis.txt";
        constans character* planum =
            "/tmp/test_rhubarb_arbor_filum.txt";

        /* reliquiae cursus prioris, a fundo sursum */
        (vacuum)filum_delere(
            "/tmp/test_rhubarb_arbor/sub/subsub/c.txt");
        (vacuum)filum_delere("/tmp/test_rhubarb_arbor/sub/subsub");
        (vacuum)filum_delere("/tmp/test_rhubarb_arbor/sub/b.txt");
        (vacuum)filum_delere("/tmp/test_rhubarb_arbor/sub");
        (vacuum)filum_delere("/tmp/test_rhubarb_arbor/a.txt");
        (vacuum)filum_delere("/tmp/test_rhubarb_arbor/nexus");
        (vacuum)filum_delere(radix);
        (vacuum)filum_delere(testis);
        (vacuum)filum_delere(aliena);
        (vacuum)filum_delere(planum);

        /* arbor: a.txt, sub/b.txt, sub/subsub/c.txt, nexus -> aliena */
        CREDO_VERUM(filum_directorium_creare_cum_parentibus(
            "/tmp/test_rhubarb_arbor/sub/subsub"));
        CREDO_VERUM(filum_scribere_literis(
            "/tmp/test_rhubarb_arbor/a.txt", "a"));
        CREDO_VERUM(filum_scribere_literis(
            "/tmp/test_rhubarb_arbor/sub/b.txt", "b"));
        CREDO_VERUM(filum_scribere_literis(
            "/tmp/test_rhubarb_arbor/sub/subsub/c.txt", "c"));
        CREDO_VERUM(filum_directorium_creare_cum_parentibus(aliena));
        CREDO_VERUM(filum_scribere_literis(testis, "testis"));
        CREDO_VERUM(symlink(aliena, "/tmp/test_rhubarb_arbor/nexus")
                    == ZEPHYRUM);

        CREDO_VERUM(filum_arborem_delere(radix));
        CREDO_FALSUM(filum_directorium_existit(radix));
        /* destinatio nexus INTACTA - hoc est ius functionis */
        CREDO_VERUM(filum_existit(testis));
        /* iterum: nihil delendum = VERUM */
        CREDO_VERUM(filum_arborem_delere(radix));

        CREDO_FALSUM(filum_arborem_delere(NIHIL));
        CREDO_FALSUM(filum_arborem_delere(""));
        /* NUMQUAM culpam in recusatione radicis plantes: haec
         * assertio functionem in "/" VOCAT, et recusatio fracta
         * discum deleret */
        CREDO_FALSUM(filum_arborem_delere("/"));

        CREDO_VERUM(filum_scribere_literis(planum, "planum"));
        CREDO_VERUM(filum_arborem_delere(planum));
        CREDO_FALSUM(filum_existit(planum));

        (vacuum)filum_delere(testis);
        (vacuum)filum_delere(aliena);
    }
}


    /* ==================================================
     * Probans filum_seram_capere (lapide feature-requests/020)
     * ================================================== */

interior vacuum
_sectio_tricesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        /* in build/ arboris, non /tmp: arbores duae simul probantes
         * eandem seram contenderent */
        constans character* via = "build/probatio_filum.sera";
                 FilumSera* a;
                 FilumSera* b;
                     pid_t  filius;
                   integer  status;

        (vacuum)filum_delere(via);

        a = filum_seram_capere(via, ZEPHYRUM, piscina);
        CREDO_NON_NIHIL(a);
        CREDO_VERUM(filum_existit(via));
        /* altera apertura, idem processus: exclusa, post moram NIHIL */
        CREDO_NIHIL(filum_seram_capere(via, L, piscina));
        filum_seram_liberare(a);
        filum_seram_liberare(a);    /* iterata: nihil agit */
        /* filum serae manet, sera libera */
        CREDO_VERUM(filum_existit(via));
        b = filum_seram_capere(via, ZEPHYRUM, piscina);
        CREDO_NON_NIHIL(b);
        filum_seram_liberare(b);

        /* MORS liberat: filius capit et exit SINE liberatione */
        filius = fork();
        si (filius == ZEPHYRUM)
        {
            FilumSera* f = filum_seram_capere(via, ZEPHYRUM, piscina);

            _exit(f != NIHIL ? ZEPHYRUM : I);
        }
        CREDO_VERUM(filius > ZEPHYRUM);
        CREDO_VERUM(waitpid(filius, &status, ZEPHYRUM) == filius);
        CREDO_VERUM(WIFEXITED(status)
            && WEXITSTATUS(status) == ZEPHYRUM);
        b = filum_seram_capere(via, ZEPHYRUM, piscina);
        CREDO_NON_NIHIL(b);
        filum_seram_liberare(b);

        /* recusationes */
        CREDO_NIHIL(filum_seram_capere(NIHIL, ZEPHYRUM, piscina));
        CREDO_NIHIL(filum_seram_capere("build", ZEPHYRUM, piscina));
        filum_seram_liberare(NIHIL);
        (vacuum)filum_delere(via);
    }
}


    /* ==================================================
     * Probare cum_parentibus sub CONCURSU (fabrica plan 2 T3)
     * ================================================== */

interior vacuum
_sectio_tricesima_secunda (
    CredoContextus* c)
{
    (vacuum)c;
    {
        constans character* radix = "/tmp/test_rhubarb_concursus";
                     pid_t  filii[II];
                       i32  f;
                       s32  status;
                       s32  fracta;

        /* CAUSA: creare_si_necesse stat() deinde mkdir() - alter
         * processus inter ea creare potest; mkdir EEXIST tum
         * 'iam existit' est, non defectus (contractus: 'vel iam
         * existit'). Inventum per thesaurum (II scriptores, eadem
         * directoria blobi). Filii II semitas CCC novas eodem
         * ordine creant; omnis vocatio VERUM. */
        (vacuum)filum_arborem_delere(radix);
        fflush(stdout);
        per (f = ZEPHYRUM; f < II; f++)
        {
            filii[f] = fork();
            si (filii[f] == ZEPHYRUM)
            {
                character semita[CCLVI];
                      i32 k;
                      s32 defectus;

                defectus = ZEPHYRUM;
                per (k = ZEPHYRUM; k < CCC; k++)
                {
                    sprintf(semita, "%s/%u/medium/nidus", radix,
                        (insignatus integer)k);
                    si (!filum_directorium_creare_cum_parentibus(semita))
                    {
                        defectus++;
                    }
                }
                _exit((integer)(defectus > CC ? CC : defectus));
            }
        }
        fracta = ZEPHYRUM;
        per (f = ZEPHYRUM; f < II; f++)
        {
            status = ZEPHYRUM;
            (vacuum)waitpid(filii[f], &status, ZEPHYRUM);
            fracta += WIFEXITED(status) ? WEXITSTATUS(status) : CC;
        }
        CREDO_AEQUALIS_S32(fracta, ZEPHYRUM);
        (vacuum)filum_arborem_delere(radix);
    }
}


    /* ==================================================
	 * Probare filum_appendere_firmiter (vates-plan-2 T1)
	 * ================================================== */

interior vacuum
_sectio_tricesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* test_via =
            "/tmp/test_rhubarb_appendere_firmiter.txt";
                    chorda lectum;

        filum_delere(test_via);

        /* creat si abest, deinde appendit - ordo servatur */
        CREDO_VERUM(filum_appendere_firmiter(test_via,
            chorda_ex_literis("prima\n", piscina)));
        CREDO_VERUM(filum_appendere_firmiter(test_via,
            chorda_ex_literis("secunda\n", piscina)));
        lectum = filum_legere_totum(test_via, piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS(lectum, "prima\nsecunda\n");

        /* contentum vacuum: filum manet, nihil additur */
        CREDO_VERUM(filum_appendere_firmiter(test_via,
            chorda_ex_literis("", piscina)));
        lectum = filum_legere_totum(test_via, piscina);
        CREDO_AEQUALIS_I32(lectum.mensura, XIV);  /* VI + VIII */

        /* directorium absens -> FALSUM, non ruina */
        CREDO_FALSUM(filum_appendere_firmiter(
            "/tmp/test_rhubarb_absens_directorium_xyz/x.txt",
            chorda_ex_literis("x", piscina)));

        filum_delere(test_via);
    }
}

    /* ---- status: modus (bits permissionum) et socket (canalis T1) ---- */

interior vacuum
_sectio_tricesima_quarta (
    CredoContextus* c)
{
    constans character* via_fili =
        "/tmp/probatio_filum_modus_status.txt";
    constans character* via_dir   = "/tmp/probatio_filum_modus_dir";
    constans character* via_sock  = "/tmp/probatio_filum_modus_dir/s";
     structura sockaddr_un  a;
                FilumStatus st;
                    integer fd;

    (vacuum)c;
    (vacuum)filum_delere(via_fili);
    (vacuum)filum_arborem_delere(via_dir);

    /* filum regulare: modus exactus, non socket */
    CREDO_VERUM(filum_scribere_literis(via_fili, "x"));
    CREDO_VERUM(filum_modum_ponere(via_fili, 0640));
    CREDO_VERUM(filum_status(via_fili, &st));
    CREDO_AEQUALIS_S32((s32)st.modus, (s32)0640);
    CREDO_VERUM(st.est_filum);
    CREDO_FALSUM(st.est_socket);

    /* directorium: modus 0700, non socket */
    CREDO_VERUM(filum_directorium_creare_cum_modo(via_dir, 0700));
    CREDO_VERUM(filum_status(via_dir, &st));
    CREDO_AEQUALIS_S32((s32)st.modus, (s32)0700);
    CREDO_VERUM(st.est_directorium);
    CREDO_FALSUM(st.est_socket);

    /* socket AF_UNIX: est_socket, nec filum nec directorium */
    fd = socket(AF_UNIX, SOCK_STREAM, ZEPHYRUM);
    CREDO_NECESSE(fd >= ZEPHYRUM);
    memset(&a, ZEPHYRUM, magnitudo(a));
    a.sun_family = AF_UNIX;
    strcpy(a.sun_path, via_sock);
    CREDO_NECESSE(bind(fd, (structura sockaddr*)(vacuum*)&a,
        (socklen_t)magnitudo(a)) == ZEPHYRUM);
    CREDO_VERUM(filum_status(via_sock, &st));
    CREDO_VERUM(st.est_socket);
    CREDO_FALSUM(st.est_filum);
    CREDO_FALSUM(st.est_directorium);

    close(fd);
    (vacuum)filum_delere(via_fili);
    (vacuum)filum_arborem_delere(via_dir);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "filum_scribere et filum_legere_totum", _sectio_prima, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "filum_existit", _sectio_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "filum_mensura", _sectio_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "filum_lector lineam_proximam", _sectio_quarta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "filum_appendere", _sectio_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "filum_movere", _sectio_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "filum_copiare", _sectio_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "filum_status", _sectio_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "filum vacuum", _sectio_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "NIHIL handling", _sectio_decima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "filum_error_recens", _sectio_decima_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "binary data cum nullis embedded", _sectio_decima_secunda, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "filum_appendere_literis", _sectio_decima_tertia, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "superscribere", _sectio_decima_quarta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "lineas vacuas in filo", _sectio_decima_quinta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "fines linearum diversos", _sectio_decima_sexta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "filum sine newline finali (multi-linea)", _sectio_decima_septima,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "aperire filum non existens", _sectio_decima_octava, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "comportamentum post EOF", _sectio_decima_nona, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "limites longitudinis linearum (4096 buffer)", _sectio_vicesima,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "FilumScriptor basicum", _sectio_vicesima_prima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "FilumScriptor modus appendere", _sectio_vicesima_secunda, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "FilumScriptor lineam_scribere", _sectio_vicesima_tertia, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "FilumScriptor scribere chorda", _sectio_vicesima_quarta, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "FilumScriptor sync", _sectio_vicesima_quinta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "FilumScriptor NIHIL handling", _sectio_vicesima_sexta, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "FilumScriptor event log simulation", _sectio_vicesima_septima,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "filum_modum_ponere", _sectio_vicesima_octava, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "directoria cum parentibus", _sectio_vicesima_nona, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "filum_arborem_delere", _sectio_tricesima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "filum_seram_capere", _sectio_tricesima_prima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "cum_parentibus sub concursu", _sectio_tricesima_secunda, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "filum_appendere_firmiter", _sectio_tricesima_tertia, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "status modus et socket", _sectio_tricesima_quarta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("filum", SECTIONES);
}
