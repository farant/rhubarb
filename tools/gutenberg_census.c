/* gutenberg_census.c - Numerare libros in Gutenberg mirror */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "iter_directoria.h"
#include "tabula_dispersa.h"
#include "exemplar.h"
#include <stdio.h>
#include <stdlib.h>

/* Contextus pro ambulatore */
nomen structura {
    TabulaDispersa* libri_visi;   /* Set numerorum librorum */
    i32 fila_totalia;              /* Omnia .txt fila */
    i32 fila_duplicata;            /* Fila cum -N suffixo */
    i32 fila_in_old;               /* Fila in old/ directoriis */
    i32 fila_non_numerica;         /* Fila sine numero (e.g., mayfl11.txt) */
    Piscina* piscina;
} CensusContextus;

/* Extrahere numerum libri ex titulo fili
 * "12345.txt" -> "12345"
 * "12345-0.txt" -> "12345"
 * "mayfl11.txt" -> "" (non numericum)
 * Redde: chorda numeri, vel vacua si non inventum
 */
hic_manens chorda
extrahere_numerum_libri(chorda titulus, Piscina* piscina)
{
    chorda fructus;
    i32 finis;
    i32 i;
    character c;

    fructus.datum = NIHIL;
    fructus.mensura = 0;

    si (titulus.mensura < 5) {  /* Minimum: "1.txt" */
        redde fructus;
    }

    /* Verificare si incipit cum digito */
    c = (character)titulus.datum[0];
    si (c < '0' || c > '9') {
        redde fructus;
    }

    /* Invenire finem digitorum */
    finis = 0;
    per (i = 0; i < titulus.mensura; i++) {
        c = (character)titulus.datum[i];
        si (c >= '0' && c <= '9') {
            finis = i + 1;
        } alioquin {
            frange;
        }
    }

    si (finis == 0) {
        redde fructus;
    }

    /* Extrahere numerum */
    fructus = chorda_sectio(titulus, 0, finis);
    fructus = chorda_transcribere(fructus, piscina);

    redde fructus;
}

/* Verificare si filum habet -N suffixum (duplicatum) */
hic_manens b32
est_duplicatum(chorda titulus)
{
    i32 i;

    /* Quaerere pattern: digiti, hyphen, digitus, ".txt" */
    per (i = 2; i < titulus.mensura - 5; i++) {
        character c = (character)titulus.datum[i];
        si (c == '-') {
            /* Verificare si sequitur digitus */
            si (i + 1 < titulus.mensura) {
                character next = (character)titulus.datum[i + 1];
                si (next >= '0' && next <= '9') {
                    redde VERUM;
                }
            }
        }
    }

    redde FALSUM;
}

/* Verificare si via continet "/old/" */
hic_manens b32
est_in_old(chorda via)
{
    i32 i;
    per (i = 0; i < via.mensura - 4; i++) {
        si (via.datum[i] == '/' &&
            via.datum[i+1] == 'o' &&
            via.datum[i+2] == 'l' &&
            via.datum[i+3] == 'd' &&
            via.datum[i+4] == '/') {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* Callback pro ambulatore */
hic_manens s32
processare_filum(
    chorda                         via_plena,
    constans DirectoriumIntroitus* introitus,
    vacuum*                        contextus)
{
    CensusContextus* ctx = (CensusContextus*)contextus;
    chorda numerus;

    /* Solum fila */
    si (introitus->genus != INTROITUS_FILUM) {
        redde 0;
    }

    /* Solum .txt */
    si (!chorda_terminatur(introitus->titulus, chorda_ex_literis(".txt", ctx->piscina))) {
        redde 0;
    }

    ctx->fila_totalia++;

    /* Verificare si in old/ directorio */
    si (est_in_old(via_plena)) {
        ctx->fila_in_old++;
    }

    /* Verificare duplicatum (-N suffix) */
    si (est_duplicatum(introitus->titulus)) {
        ctx->fila_duplicata++;
    }

    /* Extrahere et registrare numerum libri */
    numerus = extrahere_numerum_libri(introitus->titulus, ctx->piscina);
    si (numerus.datum != NIHIL) {
        si (!tabula_dispersa_continet(ctx->libri_visi, numerus)) {
            /* Novus liber */
            tabula_dispersa_inserere(ctx->libri_visi, numerus, NIHIL);
        }
    } alioquin {
        ctx->fila_non_numerica++;
    }

    redde 0;
}

s32 principale(s32 argc, character** argv)
{
    Piscina* piscina;
    CensusContextus ctx;
    DirectoriumFiltrum filtrum;
    constans character* via;
    i32 libri_unici;

    /* Via default vel ex argumento */
    si (argc > 1) {
        via = argv[1];
    } alioquin {
        via = "gutenberg-mirror";
    }

    /* Initiare */
    piscina = piscina_generare_dynamicum("census", 1024 * 1024);
    si (!piscina) {
        imprimere("Error: non possum allocare memoriam\n");
        redde 1;
    }

    ctx.libri_visi = tabula_dispersa_creare_chorda(piscina, 65536);
    ctx.fila_totalia = 0;
    ctx.fila_duplicata = 0;
    ctx.fila_in_old = 0;
    ctx.fila_non_numerica = 0;
    ctx.piscina = piscina;

    imprimere("Numerans libros in: %s\n", via);
    imprimere("...\n");

    /* Ambulare per directorium */
    filtrum = directorium_filtrum_omnia();
    filtrum.includere_occultos = FALSUM;

    si (directorium_ambulare(via, &filtrum, processare_filum, &ctx, piscina) < 0) {
        imprimere("Error: non possum aperire directorium '%s'\n", via);
        piscina_destruere(piscina);
        redde 1;
    }

    /* Numerare libros unicos */
    libri_unici = tabula_dispersa_numerus(ctx.libri_visi);

    /* Imprimere fructus */
    imprimere("\n");
    imprimere("=== CENSUS GUTENBERG ===\n");
    imprimere("Fila .txt totalia:   %u\n", ctx.fila_totalia);
    imprimere("Fila in old/:        %u\n", ctx.fila_in_old);
    imprimere("Fila duplicata (-N): %u\n", ctx.fila_duplicata);
    imprimere("Fila non numerica:   %u\n", ctx.fila_non_numerica);
    imprimere("Libri unici:         %u\n", libri_unici);
    imprimere("========================\n");

    piscina_destruere(piscina);
    redde 0;
}
