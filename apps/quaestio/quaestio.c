/* quaestio.c - adiutor $question (quaestio-rivus-spec II.6): rogatio
 * per rivum, tabulae (T/E/U/F, quaestio.h) in stdout. Clavis ex
 * $HOME/.rhubarb/anthropic.clavis SOLUM hic legitur (numquam argv,
 * numquam stdout, numquam tabula); post vates_aperire nullatur. Exitus
 * 0 post F; II si argumenta prava (E + F tamen scribuntur).
 *
 * Usus: quaestio -exemplar X -lineae L -latitudo W -quaestio <textus>
 * (vicus eam per processum vocat et tabulas per pulsum legit). */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "vates.h"
#include "quaestio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

interior vacuum
_scribere (
    vacuum* datum,
    chorda  octeti)
{
    (vacuum)datum;
    si (octeti.mensura > ZEPHYRUM)
    {
        (vacuum)fwrite(octeti.datum, I, (size_t)octeti.mensura, stdout);
    }
    (vacuum)fflush(stdout);
}

/* E + "F error": vocator semper F videt */
interior vacuum
_errorem_scribere (
    constans character* causa,
               Piscina* piscina)
{
    QuaestioTabula t;

    memset(&t, ZEPHYRUM, magnitudo(t));
    t.genus   = QUAESTIO_TABULA_ERROR;
    t.textus  = chorda_ex_literis(causa, piscina);
    _scribere(NIHIL, quaestio_tabulam_scribere(&t, piscina));
    t.genus   = QUAESTIO_TABULA_FINIS;
    t.textus  = chorda_ex_literis("error", piscina);
    _scribere(NIHIL, quaestio_tabulam_scribere(&t, piscina));
}

s32
principale (
      integer   argc,
    character** argv)
{
                Piscina* piscina;
     constans character* exemplar = "";
     constans character* quaestio = NIHIL;
     constans character* domus;
                    s32  lineae    = ZEPHYRUM;
                    s32  latitudo  = ZEPHYRUM;
                integer  k;
              character  via[DXII];
              character  octeti[DXII];
                   FILE* f;
                 size_t  n;
                 size_t  a;
                  Vates* vates;
          VatesOptiones  optiones;

    piscina = piscina_generare_dynamicum("quaestio", CXXVIII * M);
    si (!piscina)
    {
        redde I;
    }
    per (k = I; k + I < argc; k += II)
    {
        si (strcmp(argv[k], "-exemplar") == ZEPHYRUM)
        {
            exemplar = argv[k + I];
        }
        alioquin si (strcmp(argv[k], "-lineae") == ZEPHYRUM)
        {
            lineae = (s32)atoi(argv[k + I]);
        }
        alioquin si (strcmp(argv[k], "-latitudo") == ZEPHYRUM)
        {
            latitudo = (s32)atoi(argv[k + I]);
        }
        alioquin si (strcmp(argv[k], "-quaestio") == ZEPHYRUM)
        {
            quaestio = argv[k + I];
        }
    }
    si (!quaestio || lineae <= ZEPHYRUM || latitudo <= ZEPHYRUM)
    {
        _errorem_scribere("usage: quaestio -exemplar X -lineae L "
            "-latitudo W -quaestio <text>", piscina);
        piscina_destruere(piscina);
        redde II;
    }
    domus = getenv("HOME");
    si (!domus || strlen(domus) > CD)
    {
        _errorem_scribere("HOME absent", piscina);
        piscina_destruere(piscina);
        redde ZEPHYRUM;
    }
    sprintf(via, "%s/.rhubarb/anthropic.clavis", domus);
    f = fopen(via, "rb");
    si (!f)
    {
        _errorem_scribere("no API key (~/.rhubarb/anthropic.clavis)",
            piscina);
        piscina_destruere(piscina);
        redde ZEPHYRUM;
    }
    n = fread(octeti, I, magnitudo(octeti) - I, f);
    (vacuum)fclose(f);
    a = ZEPHYRUM;
    dum (   a < n && (octeti[a] == ' ' || octeti[a] == '\n'
        || octeti[a] == '\r' || octeti[a] == '\t'))
    {
        a++;
    }
    dum (   n > a && (octeti[n - I] == ' ' || octeti[n - I] == '\n'
        || octeti[n - I] == '\r' || octeti[n - I] == '\t'))
    {
        n--;
    }
    optiones  = vates_optiones_ordinariae();
    vates     = n > a ? vates_aperire(piscina, chorda_ex_literis(
        "anthropic", piscina), chorda_ex_buffer((i8*)(octeti + a),
        (i32)(n - a)), &optiones) : NIHIL;
    /* vates copiam suam unam tenet; nostra nullatur */
    memset(octeti, ZEPHYRUM, magnitudo(octeti));
    si (!vates)
    {
        _errorem_scribere(n > a ? "vates anthropic not opened"
                                : "API key file empty", piscina);
        piscina_destruere(piscina);
        redde ZEPHYRUM;
    }
    quaestio_adiutor_currere(vates, chorda_ex_literis(exemplar,
        piscina), chorda_ex_literis(quaestio, piscina), lineae,
        latitudo, _scribere, NIHIL, piscina);
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
