/* probatio_argumenta.c - Probationes Argumenta */
#include "latina.h"
#include "argumenta.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>


    /* ==================================================
	 * Probare argumenta_creare
	 * ================================================== */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        ArgumentaParser* parser;

        parser = argumenta_creare(piscina);
        CREDO_NON_NIHIL(parser);
    }
}


    /* ==================================================
	 * Probare vexilla simplicia
	 * ================================================== */

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
           ArgumentaParser* parser;
          ArgumentaFructus* fructus;
        constans character* argv[]  = {"programa", "-v", "--help"};
                       i32  argc    = III;

        parser = argumenta_creare(piscina);
        argumenta_addere_vexillum(parser, "-v", "--verbose",
            "Verbose output");
        argumenta_addere_vexillum(parser, "-h", "--help", "Show help");

        fructus = argumenta_parsere(parser, argc, argv);
        CREDO_NON_NIHIL(fructus);

        CREDO_VERUM(argumenta_habet_vexillum(fructus, "-v"));
        CREDO_VERUM(argumenta_habet_vexillum(fructus, "--verbose"));
        CREDO_VERUM(argumenta_habet_vexillum(fructus, "--help"));
        CREDO_FALSUM(argumenta_habet_vexillum(fructus, "-q"));
    }
}


    /* ==================================================
	 * Probare optiones cum valoribus
	 * ================================================== */

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
           ArgumentaParser* parser;
          ArgumentaFructus* fructus;
        constans character* argv[] = {"programa",
                                      "-o",
                                      "output.txt",
                                      "--config=settings.ini"};
                       i32 argc = IV;
                    chorda output;
                    chorda config;

        parser = argumenta_creare(piscina);
        argumenta_addere_optionem(parser, "-o", "--output",
            "Output file");
        argumenta_addere_optionem(parser, "-c", "--config",
            "Config file");

        fructus = argumenta_parsere(parser, argc, argv);
        CREDO_NON_NIHIL(fructus);

        output = argumenta_obtinere_optionem(fructus, "-o", piscina);
        CREDO_INAEQUALITAS_I32(output.mensura, ZEPHYRUM);
        CREDO_VERUM(chorda_aequalis(output,
            chorda_ex_literis("output.txt",
            piscina)));

        config = argumenta_obtinere_optionem(fructus, "--config",
            piscina);
        CREDO_INAEQUALITAS_I32(config.mensura, ZEPHYRUM);
        CREDO_VERUM(chorda_aequalis(config,
            chorda_ex_literis("settings.ini",
            piscina)));
    }
}


    /* ==================================================
	 * Probare optiones cum syntax equals
	 * ================================================== */

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
           ArgumentaParser* parser;
          ArgumentaFructus* fructus;
        constans character* argv[] = {"programa",
                                      "--output=file.txt",
                                      "--name=test"};
                       i32 argc = III;
                    chorda output;
                    chorda titulus;

        parser = argumenta_creare(piscina);
        argumenta_addere_optionem(parser, "-o", "--output",
            "Output file");
        argumenta_addere_optionem(parser, "-n", "--name", "Name");

        fructus = argumenta_parsere(parser, argc, argv);
        CREDO_NON_NIHIL(fructus);

        output = argumenta_obtinere_optionem(fructus, "--output",
            piscina);
        CREDO_VERUM(chorda_aequalis(output,
            chorda_ex_literis("file.txt",
            piscina)));

        titulus = argumenta_obtinere_optionem(fructus, "--name",
            piscina);
        CREDO_VERUM(chorda_aequalis(titulus, chorda_ex_literis("test",
            piscina)));
    }
}


    /* ==================================================
	 * Probare argumenta positionalia
	 * ================================================== */

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
           ArgumentaParser* parser;
          ArgumentaFructus* fructus;
        constans character* argv[] = {"programa", "input.txt",
            "output.txt"};
                       i32 argc = III;
                    chorda input;
                    chorda output;

        parser = argumenta_creare(piscina);
        argumenta_addere_positionalem(parser, "input", "Input file",
            VERUM);
        argumenta_addere_positionalem(parser, "output", "Output file",
            FALSUM);

        fructus = argumenta_parsere(parser, argc, argv);
        CREDO_NON_NIHIL(fructus);

        CREDO_AEQUALIS_I32(argumenta_numerus_positionalium(fructus),
            II);

        input = argumenta_obtinere_positionalem(fructus, ZEPHYRUM,
            piscina);
        CREDO_VERUM(chorda_aequalis(input,
            chorda_ex_literis("input.txt",
            piscina)));

        output = argumenta_obtinere_positionalem(fructus, I, piscina);
        CREDO_VERUM(chorda_aequalis(output,
            chorda_ex_literis("output.txt",
            piscina)));
    }
}


    /* ==================================================
	 * Probare mixtura vexilla, optiones, et positionalia
	 * ================================================== */

interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
           ArgumentaParser* parser;
          ArgumentaFructus* fructus;
        constans character* argv[] = {"programa",
                                      "-v",
                                      "--output",
                                      "out.txt",
                                      "input.txt",
                                      "--debug"};
                       i32 argc = VI;
                    chorda output;
                    chorda input;

        parser = argumenta_creare(piscina);
        argumenta_addere_vexillum(parser, "-v", "--verbose", "Verbose");
        argumenta_addere_vexillum(parser, "-d", "--debug",
            "Debug mode");
        argumenta_addere_optionem(parser, "-o", "--output",
            "Output file");
        argumenta_addere_positionalem(parser, "input", "Input file",
            VERUM);

        fructus = argumenta_parsere(parser, argc, argv);
        CREDO_NON_NIHIL(fructus);

        CREDO_VERUM(argumenta_habet_vexillum(fructus, "-v"));
        CREDO_VERUM(argumenta_habet_vexillum(fructus, "--debug"));

        output = argumenta_obtinere_optionem(fructus, "--output",
            piscina);
        CREDO_VERUM(chorda_aequalis(output, chorda_ex_literis("out.txt",
            piscina)));

        CREDO_AEQUALIS_I32(argumenta_numerus_positionalium(fructus), I);
        input = argumenta_obtinere_positionalem(fructus, ZEPHYRUM,
            piscina);
        CREDO_VERUM(chorda_aequalis(input,
            chorda_ex_literis("input.txt",
            piscina)));
    }
}


    /* ==================================================
	 * Probare vexillum solum (sine nomen longum)
	 * ================================================== */

interior vacuum
_sectio_septima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
           ArgumentaParser* parser;
          ArgumentaFructus* fructus;
        constans character* argv[]  = {"programa", "-q"};
                       i32  argc    = II;

        parser = argumenta_creare(piscina);
        argumenta_addere_vexillum(parser, "-q", NIHIL, "Quiet mode");

        fructus = argumenta_parsere(parser, argc, argv);
        CREDO_NON_NIHIL(fructus);

        CREDO_VERUM(argumenta_habet_vexillum(fructus, "-q"));
    }
}


    /* ==================================================
	 * Probare vexillum breve MULTI-CHARACTERE (stilus domus
	 * "-scribere") - truncatio prior "-s" quaerebat (mensuratum
	 * 2026-08-04, silex proicere); congruentia nunc exacta
	 * ================================================== */

interior vacuum
_sectio_octava (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
           ArgumentaParser* parser;
          ArgumentaFructus* fructus;
        constans character* argv[] = {"programa", "-scribere",
            "-ad", "XII"};
                       i32 argc = IV;
                    chorda ad;

        parser = argumenta_creare(piscina);
        argumenta_addere_vexillum(parser, "-scribere", "--scribere",
            "Applicare");
        argumenta_addere_optionem(parser, "-ad", "--ad",
            "Usque ad seq");
        argumenta_addere_vexillum(parser, "-s", NIHIL,
            "Vexillum unius characteris - vicinus non captus");

        fructus = argumenta_parsere(parser, argc, argv);
        CREDO_NON_NIHIL(fructus);
        CREDO_VERUM(argumenta_habet_vexillum(fructus, "-scribere"));
        CREDO_VERUM(argumenta_habet_vexillum(fructus, "--scribere"));
        /* "-scribere" non est "-s" - nulla truncatio */
        CREDO_FALSUM(argumenta_habet_vexillum(fructus, "-s"));
        ad = argumenta_obtinere_optionem(fructus, "-ad", piscina);
        CREDO_VERUM(chorda_aequalis(ad,
            chorda_ex_literis("XII", piscina)));
    }
}


    /* ==================================================
	 * Probare optio solum (sine nomen breve)
	 * ================================================== */

interior vacuum
_sectio_nona (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
           ArgumentaParser* parser;
          ArgumentaFructus* fructus;
        constans character* argv[] = {"programa", "--config",
            "test.ini"};
                       i32 argc = III;
                    chorda config;

        parser = argumenta_creare(piscina);
        argumenta_addere_optionem(parser, NIHIL, "--config",
            "Config file");

        fructus = argumenta_parsere(parser, argc, argv);
        CREDO_NON_NIHIL(fructus);

        config = argumenta_obtinere_optionem(fructus, "--config",
            piscina);
        CREDO_VERUM(chorda_aequalis(config,
            chorda_ex_literis("test.ini",
            piscina)));
    }
}


    /* ==================================================
	 * Probare optio non data
	 * ================================================== */

interior vacuum
_sectio_decima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
           ArgumentaParser* parser;
          ArgumentaFructus* fructus;
        constans character* argv[]  = {"programa"};
                       i32  argc    = I;
                    chorda  output;

        parser = argumenta_creare(piscina);
        argumenta_addere_optionem(parser, "-o", "--output",
            "Output file");

        fructus = argumenta_parsere(parser, argc, argv);
        CREDO_NON_NIHIL(fructus);

        output = argumenta_obtinere_optionem(fructus, "--output",
            piscina);
        CREDO_AEQUALIS_I32(output.mensura, ZEPHYRUM);
    }
}


    /* ==================================================
	 * Probare errore: optio ignota
	 * ================================================== */

interior vacuum
_sectio_decima_prima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
           ArgumentaParser* parser;
          ArgumentaFructus* fructus;
        constans character* argv[]  = {"programa", "--unknown"};
                       i32  argc    = II;

        parser = argumenta_creare(piscina);
        argumenta_addere_vexillum(parser, "-v", "--verbose", "Verbose");

        fructus = argumenta_conari_parsere(parser, argc, argv);
        CREDO_NIHIL(fructus);
    }
}


    /* ==================================================
	 * Probare errore: optio sine valore
	 * ================================================== */

interior vacuum
_sectio_decima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
           ArgumentaParser* parser;
          ArgumentaFructus* fructus;
        constans character* argv[]  = {"programa", "--output"};
                       i32  argc    = II;

        parser = argumenta_creare(piscina);
        argumenta_addere_optionem(parser, "-o", "--output",
            "Output file");

        fructus = argumenta_conari_parsere(parser, argc, argv);
        CREDO_NIHIL(fructus);
    }
}


    /* ==================================================
	 * Probare positionalia multa
	 * ================================================== */

interior vacuum
_sectio_decima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
           ArgumentaParser* parser;
          ArgumentaFructus* fructus;
        constans character* argv[] = {"programa",
                                      "file1.txt",
                                      "file2.txt",
                                      "file3.txt"};
                       i32 argc = IV;
                    chorda file1;
                    chorda file2;
                    chorda file3;

        parser = argumenta_creare(piscina);
        argumenta_addere_positionalem(parser, "file1", "First file",
            VERUM);
        argumenta_addere_positionalem(parser, "file2", "Second file",
            VERUM);
        argumenta_addere_positionalem(parser, "file3", "Third file",
            FALSUM);

        fructus = argumenta_parsere(parser, argc, argv);
        CREDO_NON_NIHIL(fructus);

        CREDO_AEQUALIS_I32(argumenta_numerus_positionalium(fructus),
            III);

        file1 = argumenta_obtinere_positionalem(fructus, ZEPHYRUM,
            piscina);
        file2 = argumenta_obtinere_positionalem(fructus, I, piscina);
        file3 = argumenta_obtinere_positionalem(fructus, II, piscina);

        CREDO_VERUM(chorda_aequalis(file1,
            chorda_ex_literis("file1.txt",
            piscina)));
        CREDO_VERUM(chorda_aequalis(file2,
            chorda_ex_literis("file2.txt",
            piscina)));
        CREDO_VERUM(chorda_aequalis(file3,
            chorda_ex_literis("file3.txt",
            piscina)));
    }
}


    /* ==================================================
	 * Probare vexillum cum valor (errore)
	 * ================================================== */

interior vacuum
_sectio_decima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
           ArgumentaParser* parser;
          ArgumentaFructus* fructus;
        constans character* argv[]  = {"programa", "--verbose=true"};
                       i32  argc    = II;

        parser = argumenta_creare(piscina);
        argumenta_addere_vexillum(parser, "-v", "--verbose", "Verbose");

        fructus = argumenta_conari_parsere(parser, argc, argv);
        CREDO_NIHIL(fructus);
    }
}


    /* ==================================================
	 * Probare sine argumenta
	 * ================================================== */

interior vacuum
_sectio_decima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
           ArgumentaParser* parser;
          ArgumentaFructus* fructus;
        constans character* argv[]  = {"programa"};
                       i32  argc    = I;

        parser = argumenta_creare(piscina);
        argumenta_addere_vexillum(parser, "-v", "--verbose", "Verbose");
        argumenta_addere_optionem(parser, "-o", "--output", "Output");

        fructus = argumenta_parsere(parser, argc, argv);
        CREDO_NON_NIHIL(fructus);

        CREDO_FALSUM(argumenta_habet_vexillum(fructus, "-v"));
        CREDO_AEQUALIS_I32(argumenta_numerus_positionalium(fructus),
            ZEPHYRUM);
    }
}


    /* ==================================================
	 * Probare --help cum positionalibus necessariis
	 * ================================================== */

interior vacuum
_sectio_decima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
           ArgumentaParser* parser;
          ArgumentaFructus* fructus;
        constans character* argv[]  = {"programa", "--help"};
                       i32  argc    = II;

        parser = argumenta_creare(piscina);
        argumenta_addere_vexillum(parser, "-h", "--help", "Show help");
        argumenta_addere_vexillum(parser, "-v", "--verbose", "Verbose");
        argumenta_addere_positionalem(parser, "input", "Input file",
            VERUM);

        /* --help debet succedere etiam sine positionali necessario */
        fructus = argumenta_conari_parsere(parser, argc, argv);
        CREDO_NON_NIHIL(fructus);
        CREDO_VERUM(argumenta_habet_vexillum(fructus, "--help"));
    }
}


    /* ==================================================
	 * Probare -h cum positionalibus necessariis
	 * ================================================== */

interior vacuum
_sectio_decima_septima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
           ArgumentaParser* parser;
          ArgumentaFructus* fructus;
        constans character* argv[]  = {"programa", "-h"};
                       i32  argc    = II;

        parser = argumenta_creare(piscina);
        argumenta_addere_vexillum(parser, "-h", "--help", "Show help");
        argumenta_addere_positionalem(parser, "input", "Input file",
            VERUM);
        argumenta_addere_positionalem(parser, "output", "Output file",
            VERUM);

        /* -h debet succedere etiam sine duobus positionalibus necessariis */
        fructus = argumenta_conari_parsere(parser, argc, argv);
        CREDO_NON_NIHIL(fructus);
        CREDO_VERUM(argumenta_habet_vexillum(fructus, "-h"));
    }
}


    /* ==================================================
	 * Probare sine --help debet adhuc verificare positionalia
	 * ================================================== */

interior vacuum
_sectio_decima_octava (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
           ArgumentaParser* parser;
          ArgumentaFructus* fructus;
        constans character* argv[]  = {"programa", "-v"};
                       i32  argc    = II;

        parser = argumenta_creare(piscina);
        argumenta_addere_vexillum(parser, "-h", "--help", "Show help");
        argumenta_addere_vexillum(parser, "-v", "--verbose", "Verbose");
        argumenta_addere_positionalem(parser, "input", "Input file",
            VERUM);

        /* Sine --help, positionale necessarium debet verificari */
        fructus = argumenta_conari_parsere(parser, argc, argv);
        CREDO_NIHIL(fructus);
    }
}


    /* ==================================================
	 * Probare descriptio et exempla
	 * ================================================== */

interior vacuum
_sectio_decima_nona (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        ArgumentaParser* parser;

        parser = argumenta_creare(piscina);
        argumenta_ponere_descriptionem(parser,
            "Test program - does testing");
        argumenta_addere_exemplum(parser, "programa -v input.txt");
        argumenta_addere_exemplum(parser,
            "programa --output=out.txt file.txt");
        argumenta_addere_vexillum(parser, "-v", "--verbose",
            "Verbose output");

        /* Non fracta */
        CREDO_NON_NIHIL(parser);
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "argumenta_creare", _sectio_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "vexilla simplicia", _sectio_secunda, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "optiones cum valoribus", _sectio_tertia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "optiones cum syntax equals", _sectio_quarta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "argumenta positionalia", _sectio_quinta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "mixtura vexilla, optiones, et positionalia", _sectio_sexta,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "vexillum solum breve", _sectio_septima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "vexillum breve longius", _sectio_octava, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "optio solum longum", _sectio_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "optio non data", _sectio_decima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "errore: optio ignota", _sectio_decima_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "errore: optio sine valore", _sectio_decima_secunda, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "positionalia multa", _sectio_decima_tertia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "vexillum cum valor (errore)", _sectio_decima_quarta, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "sine argumenta", _sectio_decima_quinta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "--help cum positionalibus necessariis", _sectio_decima_sexta,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "-h cum positionalibus necessariis", _sectio_decima_septima,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "sine --help debet verificare positionalia",
        _sectio_decima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "descriptio et exempla", _sectio_decima_nona, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("argumenta", SECTIONES);
}
