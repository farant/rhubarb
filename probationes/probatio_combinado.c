/* probatio_combinado.c - Demo applicatio cum Concha */
#include "concha.h"
#include "registrum_commandi.h"
#include <stdio.h>
#include <time.h>
#include <string.h>

#define LATITUDO_FENESTRA  DCCCLIII  /* 853 */
#define ALTITUDO_FENESTRA  CDLXXX    /* 480 */

#define VIA_LOG          "data/combinado.log"

/* ==================================================
 * Commands
 * ================================================== */

/* Legere argumentum ex textu post command (usque ad finem lineae vel spatium)
 * Redde: longitudo argumenti
 */
interior i32
_legere_argumentum(
    ContextusCommandi* ctx,
    character*         buffer,
    i32                max_long)
{
    i32 i;
    character c;

    /* Skip leading space */
    si (ctx->columna < ctx->pagina->tabula.latitudo)
    {
        c = tabula_cellula(&ctx->pagina->tabula, ctx->linea, ctx->columna);
        si (c == ' ')
        {
            ctx->columna++;
        }
    }

    /* Read until space or end of line */
    per (i = ZEPHYRUM; i < max_long - I && (s32)(ctx->columna + i) < (s32)ctx->pagina->tabula.latitudo; i++)
    {
        c = tabula_cellula(&ctx->pagina->tabula, ctx->linea, ctx->columna + i);
        si (c == '\0' || c == ' ' || c == '\n')
        {
            frange;
        }
        buffer[i] = c;
    }
    buffer[i] = '\0';

    redde i;
}


/* $rename <name> - rename current page */
interior b32
command_rename(
    ContextusCommandi* ctx)
{
    LibroPaginarum* libro;
    character titulus[LXIV];
    i32 longitudo;
    i32 index;

    libro = (LibroPaginarum*)ctx->datum_custom;
    si (!libro)
    {
        redde FALSUM;
    }

    longitudo = _legere_argumentum(ctx, titulus, LXIV);
    si (longitudo == ZEPHYRUM)
    {
        redde FALSUM;  /* No name provided */
    }

    index = libro_index_currens(libro);
    libro_pagina_nominare(libro, index, titulus);

    redde VERUM;
}


/* $goto <N> - go to page N */
interior b32
command_goto(
    ContextusCommandi* ctx)
{
    LibroPaginarum* libro;
    character num_buffer[XVI];
    i32 longitudo;
    s32 page_num;
    i32 i;

    libro = (LibroPaginarum*)ctx->datum_custom;
    si (!libro)
    {
        redde FALSUM;
    }

    longitudo = _legere_argumentum(ctx, num_buffer, XVI);
    si (longitudo == ZEPHYRUM)
    {
        redde FALSUM;  /* No number provided */
    }

    /* Parse integer */
    page_num = ZEPHYRUM;
    per (i = ZEPHYRUM; i < longitudo; i++)
    {
        si (num_buffer[i] >= '0' && num_buffer[i] <= '9')
        {
            page_num = page_num * X + (s32)(num_buffer[i] - '0');
        }
    }

    libro_navigare_ad(libro, page_num);

    redde VERUM;
}


/* $new - create new page and navigate to it */
interior b32
command_new(
    ContextusCommandi* ctx)
{
    LibroPaginarum* libro;
    s32 new_index;

    libro = (LibroPaginarum*)ctx->datum_custom;
    si (!libro)
    {
        redde FALSUM;
    }

    new_index = libro_pagina_nova(libro, NIHIL);
    si (new_index >= ZEPHYRUM)
    {
        libro_navigare_ad(libro, new_index);
    }

    redde VERUM;
}


/* $cards [slug] - switch to card view */
interior b32
command_cards(
    ContextusCommandi* ctx)
{
    Schirmata* schirmata;
    character slug[LXIV];
    i32 longitudo;

    schirmata = (Schirmata*)ctx->datum_registratus;
    si (!schirmata)
    {
        redde FALSUM;
    }

    longitudo = _legere_argumentum(ctx, slug, LXIV);
    si (longitudo > ZEPHYRUM)
    {
        schirmata_commutare_ad_arx_caeli(schirmata, slug);
    }
    alioquin
    {
        schirmata_commutare_ad_arx_caeli(schirmata, NIHIL);
    }

    redde VERUM;
}


/* $navigator - switch to navigator view */
interior b32
command_navigator(
    ContextusCommandi* ctx)
{
    Schirmata* schirmata;

    schirmata = (Schirmata*)ctx->datum_registratus;
    si (!schirmata)
    {
        redde FALSUM;
    }

    schirmata_commutare_ad_navigator(schirmata);

    redde VERUM;
}


/* ==================================================
 * Main
 * ================================================== */

int
main(int argc, char** argv)
{
    Concha*             concha;
    ConchaConfiguratio  config;
    LibroPaginarum*     libro;
    Schirmata*          schirmata;
    constans character* via_log;
    character           via_temp[CXXVIII];
    b32                 utere_temp;
    i32                 i;

    /* Verificare pro --temp-database vexillo */
    utere_temp = FALSUM;
    per (i = I; i < (i32)argc; i++)
    {
        si (strcmp(argv[i], "--temp-database") == ZEPHYRUM ||
            strcmp(argv[i], "-t") == ZEPHYRUM)
        {
            utere_temp = VERUM;
            frange;
        }
    }

    /* Determinare via ad log file */
    si (utere_temp)
    {
        /* Generare via temporanea unica */
        sprintf(via_temp, "/tmp/rhubarb_%ld.log", (longus)time(NIHIL));
        via_log = via_temp;
        imprimere("Utens database temporanea: %s\n", via_log);
    }
    alioquin si (argc > I && argv[I][ZEPHYRUM] != '-')
    {
        via_log = argv[I];
    }
    alioquin
    {
        via_log = VIA_LOG;
    }

    /* Configurare concha */
    config.titulus = "LibroPaginarum + Navigator Demo";
    config.latitudo = LATITUDO_FENESTRA;
    config.altitudo = ALTITUDO_FENESTRA;
    config.via_persistentia = via_log;

    /* Creare concha */
    concha = concha_creare(&config);
    si (!concha)
    {
        imprimere("Fractura: non potest creare concha\n");
        redde I;
    }

    /* Obtinere libro et schirmata pro commands */
    libro = concha_libro(concha);
    schirmata = concha_schirmata(concha);

    /* Registrare app-specific commands */
    concha_registrare_commandi(concha, "rename", command_rename, libro);
    concha_registrare_commandi(concha, "goto", command_goto, libro);
    concha_registrare_commandi(concha, "new", command_new, libro);
    concha_registrare_commandi(concha, "cards", command_cards, schirmata);
    concha_registrare_commandi(concha, "navigator", command_navigator, schirmata);

    /* Currere event loop */
    redde concha_currere(concha);
}
