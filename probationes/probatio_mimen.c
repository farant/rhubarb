/* probatio_mimen.c - Probationes pro tabula MIME communi */

#include "mimen.h"
#include "credo.h"
#include "piscina.h"
#include "chorda.h"

#include <stdio.h>
#include <string.h>


/* ========================================================================
 * PROBATIONES - PRO VIA (character*)
 * ======================================================================== */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    (vacuum)c;
    CREDO_VERUM(strcmp(mimen_pro_via("stilus.css"), "text/css") == 0);
    CREDO_VERUM(strcmp(mimen_pro_via("index.html"), "text/html") == 0);
    CREDO_VERUM(strcmp(mimen_pro_via("imago.jpeg"), "image/jpeg") == 0);
    CREDO_VERUM(strcmp(mimen_pro_via("via/alta/norma.js"),
        "text/javascript") == 0);

    /* ignotum / sine extensione / punctum caudale / punctum ante '/' */
    CREDO_VERUM(strcmp(mimen_pro_via("res.xyz"),
                       "application/octet-stream") == 0);
    CREDO_VERUM(strcmp(mimen_pro_via("sine_extensione"),
                       "application/octet-stream") == 0);
    CREDO_VERUM(strcmp(mimen_pro_via("caudalis."),
                       "application/octet-stream") == 0);
    CREDO_VERUM(strcmp(mimen_pro_via("a.b/c"),
                       "application/octet-stream") == 0);
    CREDO_VERUM(strcmp(mimen_pro_via(NIHIL),
                       "application/octet-stream") == 0);
}


/* ========================================================================
 * PROBATIONES - PARITAS FORMARUM (character* et chorda)
 * ======================================================================== */

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;

    constans character* probanda[] = {
        "stilus.css", "index.html", "res.xyz", "sine_extensione",
        "caudalis.", "a.b/c", "imago.png", "arca.woff2"
    };
    i32 i;

    per (i = 0; i < VIII; i++)
    {
        chorda via;
        i32 len     = (i32)strlen(probanda[i]);
        i8* buffer  = (i8*)piscina_allocare(piscina, (i64)(len + IV));

        /* copia SINE NUL - byteae venenatae post finem probant
         * lectionem longitudine limitatam */
        memcpy(buffer, probanda[i], (size_t)len);
        memset(buffer + len, 'X', IV);

        via.datum    = buffer;
        via.mensura  = len;

        CREDO_VERUM(strcmp(mimen_pro_via_chorda(via),
                           mimen_pro_via(probanda[i])) == 0);
    }

    printf("  Paritas in VIII viis (byteae venenatae post finem)\n");
}


interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
    chorda vacua;

    (vacuum)c;
    vacua.datum    = NIHIL;
    vacua.mensura  = 0;
    CREDO_VERUM(strcmp(mimen_pro_via_chorda(vacua),
                       "application/octet-stream") == 0);
}


hic_manens constans CredoSectio SECTIONES[] = {
    { "mimen_pro_via", _sectio_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "paritas char*/chorda", _sectio_secunda, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "chorda vacua", _sectio_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};


/* ========================================================================
 * PRINCIPALE
 * ======================================================================== */

integer
principale (vacuum)
{
    redde credo_suitam_currere("mimen", SECTIONES);
}
