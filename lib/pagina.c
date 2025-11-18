#include "pagina.h"
#include "chorda.h"
#include <string.h>

/* ==================================================
 * Adiutores Interni
 * ================================================== */

/* Computat positionem cursoris in linea currente */
interior vacuum
computare_lineam_et_columnam (
    constans Pagina* pagina,
    i32* linea_exitus,
    i32* columna_exitus)
{
    i32 i;
    i32 linea;
    i32 columna;
    i32 initium_lineae;

    linea = ZEPHYRUM;
    initium_lineae = ZEPHYRUM;

    /* Invenire lineam currentem et initium lineae */
    per (i = ZEPHYRUM; i < pagina->cursor; i++) {
        si (pagina->buffer[i] == '\n') {
            linea++;
            initium_lineae = i + I;
        }
    }

    columna = pagina->cursor - initium_lineae;

    *linea_exitus = linea;
    *columna_exitus = columna;
}

/* Invenire initium lineae pro offset dato */
interior i32
invenire_initium_lineae (
    constans Pagina* pagina,
    i32 offset)
{
    i32 i;

    /* Scandere retro ad newline vel initium */
    per (i = offset; i > ZEPHYRUM; i--) {
        si (pagina->buffer[i - I] == '\n') {
            redde i;
        }
    }

    redde ZEPHYRUM;
}

/* Invenire finem lineae pro offset dato */
interior i32
invenire_finem_lineae (
    constans Pagina* pagina,
    i32 offset)
{
    i32 i;

    /* Scandere ad newline vel finem */
    per (i = offset; i < pagina->longitudo; i++) {
        si (pagina->buffer[i] == '\n') {
            redde i;
        }
    }

    redde pagina->longitudo;
}


/* ==================================================
 * Creatio / Initiatio
 * ================================================== */

vacuum
pagina_initiare (
    Pagina* pagina,
    constans character* identificator)
{
    i32 i;

    /* Vacare buffer */
    per (i = ZEPHYRUM; i < PAGINA_CAPACITAS; i++) {
        pagina->buffer[i] = '\0';
    }

    pagina->longitudo = ZEPHYRUM;
    pagina->cursor = ZEPHYRUM;
    pagina->selectio_initium = -I;
    pagina->selectio_finis = -I;

    /* Copiere identificator */
    per (i = ZEPHYRUM; i < PAGINA_IDENTIFICATOR_LONGITUDO - I; i++) {
        si (identificator[i] == '\0') {
            frange;
        }
        pagina->identificator[i] = identificator[i];
    }
    pagina->identificator[i] = '\0';
}

vacuum
pagina_vacare (
    Pagina* pagina)
{
    pagina->longitudo = ZEPHYRUM;
    pagina->cursor = ZEPHYRUM;
    pagina->selectio_initium = -I;
    pagina->selectio_finis = -I;
}


/* ==================================================
 * Editio - Insertio
 * ================================================== */

b32
pagina_inserere_characterem (
    Pagina* pagina,
    character c)
{
    i32 i;

    /* Verificare si plena */
    si (pagina->longitudo >= PAGINA_CAPACITAS) {
        redde FALSUM;
    }

    /* Movere caudam dextram */
    per (i = pagina->longitudo; i > pagina->cursor; i--) {
        pagina->buffer[i] = pagina->buffer[i - I];
    }

    /* Inserere character */
    pagina->buffer[pagina->cursor] = c;
    pagina->cursor++;
    pagina->longitudo++;

    redde VERUM;
}

i32
pagina_inserere_chordam (
    Pagina* pagina,
    constans character* textus)
{
    i32 i;
    i32 numerus_insertus;

    numerus_insertus = ZEPHYRUM;

    per (i = ZEPHYRUM; textus[i] != '\0'; i++) {
        si (!pagina_inserere_characterem(pagina, textus[i])) {
            frange;  /* Plena */
        }
        numerus_insertus++;
    }

    redde numerus_insertus;
}


/* ==================================================
 * Editio - Deletio
 * ================================================== */

b32
pagina_delere_characterem (
    Pagina* pagina)
{
    i32 i;

    /* Verificare si ad initium */
    si (pagina->cursor == ZEPHYRUM) {
        redde FALSUM;
    }

    /* Movere caudam sinistram */
    per (i = pagina->cursor - I; i < pagina->longitudo - I; i++) {
        pagina->buffer[i] = pagina->buffer[i + I];
    }

    pagina->cursor--;
    pagina->longitudo--;

    redde VERUM;
}

b32
pagina_delere_characterem_ante (
    Pagina* pagina)
{
    i32 i;

    /* Verificare si ad finem */
    si (pagina->cursor >= pagina->longitudo) {
        redde FALSUM;
    }

    /* Movere caudam sinistram */
    per (i = pagina->cursor; i < pagina->longitudo - I; i++) {
        pagina->buffer[i] = pagina->buffer[i + I];
    }

    pagina->longitudo--;

    redde VERUM;
}

b32
pagina_delere_selectionem (
    Pagina* pagina)
{
    i32 i;
    i32 initium;
    i32 finis;
    i32 amplitudo;

    /* Verificare si habet selectionem */
    si (!pagina_habet_selectionem(pagina)) {
        redde FALSUM;
    }

    /* Ordinare initium et finem */
    si (pagina->selectio_initium < pagina->selectio_finis) {
        initium = (i32)pagina->selectio_initium;
        finis = (i32)pagina->selectio_finis;
    } alioquin {
        initium = (i32)pagina->selectio_finis;
        finis = (i32)pagina->selectio_initium;
    }

    amplitudo = finis - initium;

    /* Movere caudam sinistram */
    per (i = initium; i < pagina->longitudo - amplitudo; i++) {
        pagina->buffer[i] = pagina->buffer[i + amplitudo];
    }

    pagina->longitudo -= (i32)amplitudo;
    pagina->cursor = (i32)initium;

    /* Vacare selectionem */
    pagina->selectio_initium = -I;
    pagina->selectio_finis = -I;

    redde VERUM;
}


/* ==================================================
 * Motus Cursoris
 * ================================================== */

b32
pagina_movere_cursor_sinistram (
    Pagina* pagina)
{
    si (pagina->cursor == ZEPHYRUM) {
        redde FALSUM;
    }

    pagina->cursor--;
    redde VERUM;
}

b32
pagina_movere_cursor_dextram (
    Pagina* pagina)
{
    si (pagina->cursor >= pagina->longitudo) {
        redde FALSUM;
    }

    pagina->cursor++;
    redde VERUM;
}

b32
pagina_movere_cursor_sursum (
    Pagina* pagina)
{
    i32 linea_currens;
    i32 columna_currens;
    i32 initium_lineae_currens;
    i32 initium_lineae_superior;
    i32 finis_lineae_superior;
    i32 longitudo_lineae_superior;
    i32 nova_columna;

    /* Computare lineam et columnam currentem */
    computare_lineam_et_columnam(pagina, &linea_currens, &columna_currens);

    /* Si ad primam lineam, non possumus movere sursum */
    si (linea_currens == ZEPHYRUM) {
        redde FALSUM;
    }

    /* Invenire initium lineae currens */
    initium_lineae_currens = invenire_initium_lineae(pagina, pagina->cursor);

    /* Invenire initium lineae superior (scandere retro ultra newline) */
    initium_lineae_superior = invenire_initium_lineae(pagina, initium_lineae_currens - I);

    /* Invenire finem lineae superior */
    finis_lineae_superior = initium_lineae_currens - I;  /* Character ante newline */

    /* Computare longitudinem lineae superior */
    longitudo_lineae_superior = finis_lineae_superior - initium_lineae_superior;

    /* Computare novam columnam (minimus de columna currens et longitudo lineae) */
    si (columna_currens < longitudo_lineae_superior) {
        nova_columna = columna_currens;
    } alioquin {
        nova_columna = longitudo_lineae_superior;
    }

    /* Ponere cursorem */
    pagina->cursor = initium_lineae_superior + nova_columna;

    redde VERUM;
}

b32
pagina_movere_cursor_deorsum (
    Pagina* pagina)
{
    i32 linea_currens;
    i32 columna_currens;
    i32 finis_lineae_currens;
    i32 initium_lineae_inferior;
    i32 finis_lineae_inferior;
    i32 longitudo_lineae_inferior;
    i32 nova_columna;

    /* Computare lineam et columnam currentem */
    computare_lineam_et_columnam(pagina, &linea_currens, &columna_currens);

    /* Invenire finem lineae currens */
    finis_lineae_currens = invenire_finem_lineae(pagina, pagina->cursor);

    /* Si ad ultimam lineam (nullus newline), non possumus movere deorsum */
    si (finis_lineae_currens >= pagina->longitudo) {
        redde FALSUM;
    }

    /* Initium lineae inferior est post newline */
    initium_lineae_inferior = finis_lineae_currens + I;

    /* Invenire finem lineae inferior */
    finis_lineae_inferior = invenire_finem_lineae(pagina, initium_lineae_inferior);

    /* Computare longitudinem lineae inferior */
    longitudo_lineae_inferior = finis_lineae_inferior - initium_lineae_inferior;

    /* Computare novam columnam */
    si (columna_currens < longitudo_lineae_inferior) {
        nova_columna = columna_currens;
    } alioquin {
        nova_columna = longitudo_lineae_inferior;
    }

    /* Ponere cursorem */
    pagina->cursor = initium_lineae_inferior + nova_columna;

    redde VERUM;
}

vacuum
pagina_movere_cursor_domus (
    Pagina* pagina)
{
    i32 initium_lineae;

    initium_lineae = invenire_initium_lineae(pagina, pagina->cursor);
    pagina->cursor = initium_lineae;
}

vacuum
pagina_movere_cursor_finis (
    Pagina* pagina)
{
    i32 finis_lineae;

    finis_lineae = invenire_finem_lineae(pagina, pagina->cursor);
    pagina->cursor = finis_lineae;
}

vacuum
pagina_ponere_cursor (
    Pagina* pagina,
    i32 positio)
{
    /* Cohibere intra limites */
    si (positio < ZEPHYRUM) {
        positio = ZEPHYRUM;
    }
    si (positio > pagina->longitudo) {
        positio = pagina->longitudo;
    }

    pagina->cursor = positio;
}


/* ==================================================
 * Selectio
 * ================================================== */

vacuum
pagina_ponere_selectionem (
    Pagina* pagina,
    i32 initium,
    i32 finis)
{
    pagina->selectio_initium = (s32)initium;
    pagina->selectio_finis = (s32)finis;
}

vacuum
pagina_vacare_selectionem (
    Pagina* pagina)
{
    pagina->selectio_initium = -I;
    pagina->selectio_finis = -I;
}

i32
pagina_obtinere_textum_selectum (
    constans Pagina* pagina,
    character* exitus,
    i32 maxima_longitudo)
{
    i32 i;
    i32 initium;
    i32 finis;
    i32 longitudo;

    si (!pagina_habet_selectionem(pagina)) {
        exitus[ZEPHYRUM] = '\0';
        redde ZEPHYRUM;
    }

    /* Ordinare initium et finem */
    si (pagina->selectio_initium < pagina->selectio_finis) {
        initium = (i32)pagina->selectio_initium;
        finis = (i32)pagina->selectio_finis;
    } alioquin {
        initium = (i32)pagina->selectio_finis;
        finis = (i32)pagina->selectio_initium;
    }

    longitudo = finis - initium;

    /* Cohibere longitudinem */
    si (longitudo >= (i32)maxima_longitudo) {
        longitudo = (i32)maxima_longitudo - I;
    }

    /* Copiere textum */
    per (i = ZEPHYRUM; i < longitudo; i++) {
        exitus[i] = pagina->buffer[initium + (i32)i];
    }
    exitus[longitudo] = '\0';

    redde longitudo;
}

b32
pagina_habet_selectionem (
    constans Pagina* pagina)
{
    redde (pagina->selectio_initium >= ZEPHYRUM &&
           pagina->selectio_finis >= ZEPHYRUM &&
           pagina->selectio_initium != pagina->selectio_finis);
}


/* ==================================================
 * Interrogationes
 * ================================================== */

b32
pagina_est_plena (
    constans Pagina* pagina)
{
    redde pagina->longitudo >= PAGINA_CAPACITAS;
}

b32
pagina_est_vacua (
    constans Pagina* pagina)
{
    redde pagina->longitudo == ZEPHYRUM;
}


/* ==================================================
 * Reddere
 * ================================================== */

vacuum
pagina_reddere (
    TabulaPixelorum* tabula,
    constans Pagina* pagina,
    i32 x,
    i32 y,
    i32 latitudo,
    i32 altitudo,
    i32 scala)
{
    s32 i;
    i32 cursor_x;
    i32 cursor_y;
    i32 current_x;
    i32 current_y;
    i32 character_latitudo;
    i32 character_altitudo;
    i32 pixel_x;
    i32 pixel_y;
    b32 cursor_inventus;

    character_latitudo = VI * scala;
    character_altitudo = VIII * scala;
    cursor_inventus = FALSUM;

    current_x = ZEPHYRUM;
    current_y = ZEPHYRUM;
    cursor_x = ZEPHYRUM;
    cursor_y = ZEPHYRUM;

    /* Pingere textum et invenire positionem cursoris */
    per (i = ZEPHYRUM; i < (s32)pagina->longitudo; i++) {
        character c;

        c = pagina->buffer[(i32)i];

        /* Verificare si cursor ad hunc characterem */
        si (i == (s32)pagina->cursor && !cursor_inventus) {
            cursor_x = current_x;
            cursor_y = current_y;
            cursor_inventus = VERUM;
        }

        /* Tractare newline */
        si (c == '\n') {
            current_x = ZEPHYRUM;
            current_y++;
            perge;
        }

        /* Verificare si extra limites */
        si (current_y >= altitudo) {
            frange;
        }

        /* Tractare line wrap */
        si (current_x >= latitudo) {
            current_x = ZEPHYRUM;
            current_y++;
            si (current_y >= altitudo) {
                frange;
            }
        }

        /* Pingere characterem */
        pixel_x = x * character_latitudo + current_x * character_latitudo;
        pixel_y = y * character_altitudo + current_y * character_altitudo;

        /* TODO: Hic debemus tractare selectionem (background color differentem) */

        tabula_pixelorum_pingere_characterem(
            tabula,
            pixel_x,
            pixel_y,
            c,
            RGB(CCLV, CCLV, CCLV));

        current_x++;
    }

    /* Si cursor ad finem textus */
    si (pagina->cursor == pagina->longitudo && !cursor_inventus) {
        cursor_x = current_x;
        cursor_y = current_y;
        cursor_inventus = VERUM;
    }

    /* Pingere cursorem */
    si (cursor_inventus) {
        i32 j;
        i32 cursor_pixel_x;
        i32 cursor_pixel_y;

        cursor_pixel_x = x * character_latitudo + cursor_x * character_latitudo;
        cursor_pixel_y = y * character_altitudo + cursor_y * character_altitudo;

        /* Pingere rectangulum pro cursore (linea verticalis) */
        /* TODO: Hic debemus usare delineare_rectangulum_plenum vel ponere pixela directe */
        /* Pro nunc, pingere lineam verticalem simplicem */
        per (j = ZEPHYRUM; j < character_altitudo; j++) {
            tabula_pixelorum_ponere_pixelum(
                tabula,
                cursor_pixel_x,
                cursor_pixel_y + j,
                RGB(CCLV, CCLV, ZEPHYRUM));
        }
    }
}
