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
probatio_pro_via(vacuum)
{
    printf("--- Probans mimen_pro_via ---\n");

    CREDO_VERUM(strcmp(mimen_pro_via("stilus.css"), "text/css") == 0);
    CREDO_VERUM(strcmp(mimen_pro_via("index.html"), "text/html") == 0);
    CREDO_VERUM(strcmp(mimen_pro_via("imago.jpeg"), "image/jpeg") == 0);
    CREDO_VERUM(strcmp(mimen_pro_via("via/alta/norma.js"), "text/javascript") == 0);

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

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - PARITAS FORMARUM (character* et chorda)
 * ======================================================================== */

interior vacuum
probatio_paritas_chorda(Piscina* piscina)
{
    constans character* probanda[] = {
        "stilus.css", "index.html", "res.xyz", "sine_extensione",
        "caudalis.", "a.b/c", "imago.png", "arca.woff2"
    };
    i32 i;

    printf("--- Probans paritas char*/chorda ---\n");

    per (i = 0; i < VIII; i++)
    {
        chorda via;
        i32 len = (i32)strlen(probanda[i]);
        i8* buffer = (i8*)piscina_allocare(piscina, (i64)(len + IV));

        /* copia SINE NUL - byteae venenatae post finem probant
         * lectionem longitudine limitatam */
        memcpy(buffer, probanda[i], (size_t)len);
        memset(buffer + len, 'X', IV);

        via.datum = buffer;
        via.mensura = len;

        CREDO_VERUM(strcmp(mimen_pro_via_chorda(via),
                           mimen_pro_via(probanda[i])) == 0);
    }

    printf("  Paritas in VIII viis (byteae venenatae post finem)\n");
    printf("\n");
}


interior vacuum
probatio_chorda_vacua(vacuum)
{
    chorda vacua;

    printf("--- Probans chorda vacua ---\n");

    vacua.datum = NIHIL;
    vacua.mensura = 0;
    CREDO_VERUM(strcmp(mimen_pro_via_chorda(vacua),
                       "application/octet-stream") == 0);

    printf("\n");
}


/* ========================================================================
 * PRINCIPALE
 * ======================================================================== */

integer
principale(vacuum)
{
    Piscina* piscina;
    b32 successus;

    printf("\n");
    printf("========================================\n");
    printf("PROBATIONES MIMEN\n");
    printf("========================================\n");
    printf("\n");

    piscina = piscina_generare_dynamicum("probatio_mimen", XVI * M);
    credo_aperire(piscina);

    probatio_pro_via();
    probatio_paritas_chorda(piscina);
    probatio_chorda_vacua();

    credo_imprimere_compendium();

    printf("========================================\n");
    printf("\n");

    successus = credo_omnia_praeterierunt();

    credo_claudere();
    piscina_destruere(piscina);

    redde successus ? 0 : I;
}
