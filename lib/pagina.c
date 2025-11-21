#include "pagina.h"
#include "chorda.h"
#include "tempus.h"
#include "delineare.h"
#include "thema.h"
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
    s32 i;

    /* Scandere ad newline vel finem */
    per (i = (s32)offset; i < (s32)pagina->longitudo; i++) {
        si (pagina->buffer[(i32)i] == '\n') {
            redde (i32)i;
        }
    }

    redde (i32)pagina->longitudo;
}

/* Public wrappers pro utilitas linearum */
i32
pagina_invenire_initium_lineae (
    constans Pagina* pagina,
    i32 offset)
{
    redde invenire_initium_lineae(pagina, offset);
}

i32
pagina_invenire_finem_lineae (
    constans Pagina* pagina,
    i32 offset)
{
    redde invenire_finem_lineae(pagina, offset);
}

/* Verificare si character est alphanumericus vel underscore */
interior b32
est_character_verbi (
    character c)
{
    redde (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           (c >= '0' && c <= '9') ||
           (c == '_');
}

/* Movere ad initium verbi proximi (w) */
vacuum
pagina_movere_ad_verbum_proximum (
    Pagina* pagina)
{
    s32 i;
    b32 in_verbo;

    si (pagina->cursor >= pagina->longitudo) {
        redde;
    }

    i = (s32)pagina->cursor;

    /* Saltare verbum currens */
    in_verbo = est_character_verbi(pagina->buffer[i]);
    dum (i < (s32)pagina->longitudo &&
         est_character_verbi(pagina->buffer[i]) == in_verbo &&
         pagina->buffer[i] != ' ' &&
         pagina->buffer[i] != '\t' &&
         pagina->buffer[i] != '\n') {
        i++;
    }

    /* Saltare spatia */
    dum (i < (s32)pagina->longitudo &&
         (pagina->buffer[i] == ' ' ||
          pagina->buffer[i] == '\t' ||
          pagina->buffer[i] == '\n')) {
        i++;
    }

    pagina->cursor = (i32)i;
}

/* Movere ad initium verbi praecedentis (b) */
vacuum
pagina_movere_ad_verbum_praecedens (
    Pagina* pagina)
{
    s32 i;

    si (pagina->cursor == ZEPHYRUM) {
        redde;
    }

    i = (s32)pagina->cursor - I;

    /* Saltare spatia retro */
    dum (i > ZEPHYRUM &&
         (pagina->buffer[i] == ' ' ||
          pagina->buffer[i] == '\t' ||
          pagina->buffer[i] == '\n')) {
        i--;
    }

    /* Saltare ad initium verbi */
    dum (i > ZEPHYRUM &&
         pagina->buffer[i - I] != ' ' &&
         pagina->buffer[i - I] != '\t' &&
         pagina->buffer[i - I] != '\n') {
        i--;
    }

    pagina->cursor = (i32)i;
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

    /* Initiare vim state */
    pagina->modo = MODO_NORMAL;
    pagina->clavis_praecedens = '\0';
    pagina->esperans_fd = FALSUM;
    pagina->tempus_f = 0.0;

    /* Initiare cursor blink state */
    pagina->tempus_cursor_ultimus = 0.0;
    pagina->cursor_visibilis = VERUM;

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
 * Indentatio
 * ================================================== */

i32
pagina_obtinere_indentationem_lineae (
    constans Pagina* pagina,
    character* exitus,
    i32 maxima_longitudo)
{
    i32 initium_lineae;
    s32 i;
    i32 longitudo;

    /* Invenire initium lineae currentis */
    initium_lineae = invenire_initium_lineae(pagina, pagina->cursor);

    /* Computare longitudinem indentationis */
    longitudo = ZEPHYRUM;
    per (i = (s32)initium_lineae; i < (s32)pagina->longitudo; i++) {
        character c;

        c = pagina->buffer[(i32)i];

        /* Si non spatium vel tab, finis indentationis */
        si (c != ' ' && c != '\t') {
            frange;
        }

        /* Copiere si spatium in buffer */
        si (longitudo < (i32)maxima_longitudo - I) {
            exitus[longitudo] = c;
            longitudo++;
        }
    }

    /* Terminare chordam */
    si (longitudo < (i32)maxima_longitudo) {
        exitus[longitudo] = '\0';
    } alioquin {
        exitus[(i32)maxima_longitudo - I] = '\0';
    }

    redde longitudo;
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
    Pagina* pagina,
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

        /* Tractare tab - reddere ut duo spatia */
        si (c == '\t') {
            current_x += II;  /* Avanzare duo spatia */
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
            thema_color(COLOR_TEXT));

        current_x++;
    }

    /* Si cursor ad finem textus */
    si (pagina->cursor == pagina->longitudo && !cursor_inventus) {
        cursor_x = current_x;
        cursor_y = current_y;
        cursor_inventus = VERUM;
    }

    /* Pingere cursorem (cum blinking) */
    si (cursor_inventus) {
        i32 j;
        i32 cursor_pixel_x;
        i32 cursor_pixel_y;
        f64 tempus_currens;
        f64 delta_tempus;

        cursor_pixel_x = x * character_latitudo + cursor_x * character_latitudo;
        cursor_pixel_y = y * character_altitudo + cursor_y * character_altitudo;

        /* Update blink state */
        tempus_currens = tempus_nunc();
        delta_tempus = tempus_currens - pagina->tempus_cursor_ultimus;

        /* Blink every 0.5 seconds */
        si (delta_tempus >= 0.5) {
            pagina->cursor_visibilis = !pagina->cursor_visibilis;
            pagina->tempus_cursor_ultimus = tempus_currens;
        }

        /* Pingere rectangulum pro cursore (linea verticalis) */
        /* TODO: Hic debemus usare delineare_rectangulum_plenum vel ponere pixela directe */
        /* Pro nunc, pingere lineam verticalem simplicem */
        si (pagina->cursor_visibilis) {
            per (j = ZEPHYRUM; j < character_altitudo; j++) {
                tabula_pixelorum_ponere_pixelum(
                    tabula,
                    cursor_pixel_x,
                    cursor_pixel_y + j,
                    thema_color(COLOR_CURSOR));
            }
        }
    }
}


/* ==================================================
 * Tractare Eventus (Vim Mode)
 * ================================================== */

b32
pagina_tractare_eventum (
    Pagina* pagina,
    constans Eventus* eventus)
{
    clavis_t clavis;
    character c;

    si (eventus->genus != EVENTUS_CLAVIS_DEPRESSUS) {
        redde VERUM;  /* Ignorare eventus non-clavis */
    }

    clavis = eventus->datum.clavis.clavis;
    c = eventus->datum.clavis.typus;

    /* Reset cursor blink on keypress (make cursor visible) */
    pagina->cursor_visibilis = VERUM;
    pagina->tempus_cursor_ultimus = tempus_nunc();

    si (pagina->modo == MODO_INSERT)
    {
        /* Modo Insert - tractare ut editor textus normalis */
        si (clavis == CLAVIS_EFFUGIUM || c == XXVII)
        {
            /* ESC vel Ctrl-[ - redire ad modum normalem */
            pagina->modo = MODO_NORMAL;
            pagina->clavis_praecedens = '\0';
            pagina->esperans_fd = FALSUM;
        }
        alioquin si (clavis == CLAVIS_RETRORSUM)
        {
            pagina_delere_characterem(pagina);
            pagina->esperans_fd = FALSUM;
        }
        alioquin si (clavis == CLAVIS_DELERE)
        {
            pagina_delere_characterem_ante(pagina);
        }
        alioquin si (c == '\n' || c == '\r')
        {
            character indentatio[C];
            i32 longitudo_indentationis;
            i32 i;

            longitudo_indentationis = pagina_obtinere_indentationem_lineae(
                pagina, indentatio, C);

            pagina_inserere_characterem(pagina, '\n');

            per (i = ZEPHYRUM; i < longitudo_indentationis; i++) {
                pagina_inserere_characterem(pagina, indentatio[i]);
            }
        }
        alioquin si (c == '\t')
        {
            pagina_inserere_characterem(pagina, '\t');
        }
        alioquin si (c != '\0' && c >= XXXII && c <= CXXVI)
        {
            /* Verificare si esperamus 'd' post 'f' */
            si (pagina->esperans_fd)
            {
                f64 tempus_elapsum;

                tempus_elapsum = tempus_nunc() - pagina->tempus_f;

                si (c == 'd' && tempus_elapsum < 0.5)
                {
                    /* fd intra tempus - delere 'f' et exire */
                    pagina_delere_characterem(pagina);
                    pagina->modo = MODO_NORMAL;
                    pagina->clavis_praecedens = '\0';
                    pagina->esperans_fd = FALSUM;
                }
                alioquin
                {
                    /* Timeout vel character differens - inserere normaliter */
                    pagina->esperans_fd = FALSUM;
                    pagina_inserere_characterem(pagina, c);

                    si (c == 'f')
                    {
                        pagina->esperans_fd = VERUM;
                        pagina->tempus_f = tempus_nunc();
                    }
                }
            }
            alioquin
            {
                pagina_inserere_characterem(pagina, c);

                /* Si 'f' typatum, initiare sequentiam */
                si (c == 'f')
                {
                    pagina->esperans_fd = VERUM;
                    pagina->tempus_f = tempus_nunc();
                }
            }
        }
    }
    alioquin si (pagina->modo == MODO_NORMAL)
    {
        /* Modo Normal - tractare vim commands */
        si (clavis == CLAVIS_EFFUGIUM)
        {
            redde FALSUM;  /* Signal quit */
        }
        /* TAB - non consumere, permittere manager tractare focus */
        alioquin si (clavis == CLAVIS_TABULA)
        {
            redde FALSUM;  /* Permittere focus switching */
        }
        /* Movimento hjkl */
        alioquin si (c == 'h')
        {
            pagina_movere_cursor_sinistram(pagina);
            pagina->clavis_praecedens = '\0';
        }
        alioquin si (c == 'j')
        {
            pagina_movere_cursor_deorsum(pagina);
            pagina->clavis_praecedens = '\0';
        }
        alioquin si (c == 'k')
        {
            pagina_movere_cursor_sursum(pagina);
            pagina->clavis_praecedens = '\0';
        }
        alioquin si (c == 'l')
        {
            pagina_movere_cursor_dextram(pagina);
            pagina->clavis_praecedens = '\0';
        }
        /* Navigatio verbi */
        alioquin si (c == 'w')
        {
            pagina_movere_ad_verbum_proximum(pagina);
            pagina->clavis_praecedens = '\0';
        }
        alioquin si (c == 'b')
        {
            pagina_movere_ad_verbum_praecedens(pagina);
            pagina->clavis_praecedens = '\0';
        }
        /* Intrare modum insert */
        alioquin si (c == 'i')
        {
            pagina->modo = MODO_INSERT;
            pagina->clavis_praecedens = '\0';
            pagina->esperans_fd = FALSUM;
        }
        alioquin si (c == 'a')
        {
            pagina_movere_cursor_dextram(pagina);
            pagina->modo = MODO_INSERT;
            pagina->clavis_praecedens = '\0';
            pagina->esperans_fd = FALSUM;
        }
        /* Commandos multi-clavis */
        alioquin si (c == 'd')
        {
            si (pagina->clavis_praecedens == 'd')
            {
                /* dd - delere lineam */
                i32 initium;
                i32 finis;

                initium = pagina_invenire_initium_lineae(pagina, (i32)pagina->cursor);
                finis = pagina_invenire_finem_lineae(pagina, (i32)pagina->cursor);

                /* Si non ultima linea, includere newline */
                si (finis < pagina->longitudo) {
                    finis++;
                }

                pagina_ponere_selectionem(pagina, initium, finis);
                pagina_delere_selectionem(pagina);
                pagina->clavis_praecedens = '\0';
            }
            alioquin si (pagina->clavis_praecedens == '\0')
            {
                pagina->clavis_praecedens = 'd';
            }
        }
        alioquin si (c == 'G')
        {
            si (pagina->clavis_praecedens == 'd')
            {
                /* dG - delere ad finem */
                pagina_ponere_selectionem(pagina, pagina->cursor, pagina->longitudo);
                pagina_delere_selectionem(pagina);
                pagina->clavis_praecedens = '\0';
            }
            alioquin
            {
                /* G - saltare ad finem */
                pagina_ponere_cursor(pagina, pagina->longitudo);
                pagina->clavis_praecedens = '\0';
            }
        }
        alioquin si (c == '$')
        {
            si (pagina->clavis_praecedens == 'd')
            {
                /* d$ - delere ad finem lineae */
                i32 finis;

                finis = pagina_invenire_finem_lineae(pagina, (i32)pagina->cursor);
                pagina_ponere_selectionem(pagina, pagina->cursor, (i32)finis);
                pagina_delere_selectionem(pagina);
                pagina->clavis_praecedens = '\0';
            }
            alioquin
            {
                /* $ - movere ad finem lineae */
                pagina_movere_cursor_finis(pagina);
                pagina->clavis_praecedens = '\0';
            }
        }
        alioquin si (c == 'g')
        {
            /* g - saltare ad initium */
            pagina_ponere_cursor(pagina, ZEPHYRUM);
            pagina->clavis_praecedens = '\0';
        }
        alioquin si (c == '^')
        {
            /* ^ - saltare ad primum characterem non-spatium lineae */
            i32 initium;
            s32 i;

            initium = pagina_invenire_initium_lineae(pagina, (i32)pagina->cursor);

            /* Invenire primum characterem non-spatium */
            per (i = (s32)initium; i < (s32)pagina->longitudo; i++) {
                character c_temp;

                c_temp = pagina->buffer[(i32)i];
                si (c_temp != ' ' && c_temp != '\t') {
                    frange;
                }
            }

            pagina_ponere_cursor(pagina, (i32)i);
            pagina->clavis_praecedens = '\0';
        }
        alioquin si (c == 'o')
        {
            /* o - nova linea post */
            character indentatio[C];
            i32 longitudo_indentationis;
            i32 i;

            pagina_movere_cursor_finis(pagina);

            longitudo_indentationis = pagina_obtinere_indentationem_lineae(
                pagina, indentatio, C);

            pagina_inserere_characterem(pagina, '\n');

            per (i = ZEPHYRUM; i < longitudo_indentationis; i++) {
                pagina_inserere_characterem(pagina, indentatio[i]);
            }

            pagina->modo = MODO_INSERT;
            pagina->clavis_praecedens = '\0';
            pagina->esperans_fd = FALSUM;
        }
        alioquin si (c == 'O')
        {
            /* O - nova linea ante */
            character indentatio[C];
            i32 longitudo_indentationis;
            i32 i;

            pagina_movere_cursor_domus(pagina);

            longitudo_indentationis = pagina_obtinere_indentationem_lineae(
                pagina, indentatio, C);

            pagina_inserere_characterem(pagina, '\n');
            pagina_movere_cursor_sinistram(pagina);

            per (i = ZEPHYRUM; i < longitudo_indentationis; i++) {
                pagina_inserere_characterem(pagina, indentatio[i]);
            }

            pagina->modo = MODO_INSERT;
            pagina->clavis_praecedens = '\0';
            pagina->esperans_fd = FALSUM;
        }
        alioquin
        {
            /* Clavis non recognita - vacare clavis praecedens */
            pagina->clavis_praecedens = '\0';
        }
    }

    redde VERUM;  /* Eventus tractatus */
}


/* ==================================================
 * Reddere Cum Margine
 * ================================================== */

vacuum
pagina_reddere_cum_margine (
    Piscina* piscina,
    TabulaPixelorum* tabula,
    Pagina* pagina,
    i32 x,
    i32 y,
    i32 latitudo,
    i32 altitudo,
    i32 scala,
    b32 focused)
{
    ContextusDelineandi* ctx;
    i32 character_latitudo;
    i32 character_altitudo;
    i32 box_x0, box_y0, box_x1, box_y1;
    i32 textus_latitudo;
    i32 textus_altitudo;
    i32 color_border;
    i32 color_border_inner;
    chorda titulo;
    chorda modo_textus;
    constans character* modo_str;

    character_latitudo = VI * scala;
    character_altitudo = VIII * scala;

    /* Computare dimensiones areae textus (minus border) */
    textus_latitudo = latitudo - II;
    textus_altitudo = altitudo - II;

    /* Computare coordinatas pixelorum pro box (with padding from edges) */
    box_x0 = x * character_latitudo + II;  /* 2px padding from left */
    box_y0 = y * character_altitudo + II;  /* 2px padding from top */
    box_x1 = (x + latitudo) * character_latitudo - I - II;  /* 2px padding from right */
    box_y1 = (y + altitudo) * character_altitudo - I - II;  /* 2px padding from bottom */

    color_border = thema_color(COLOR_BORDER);
    color_border_inner = focused ? thema_color(COLOR_BORDER_ACTIVE) : color_border;

    /* Creare contextum delineandi */
    ctx = delineare_creare_contextum(piscina, tabula);

    /* ==================================================
     * Computare positiones textus (centered)
     * ================================================== */

    titulo = chorda_ex_literis(pagina->identificator, piscina);
    modo_str = (pagina->modo == MODO_INSERT) ? "INSERT" : "NORMAL";
    modo_textus = chorda_ex_literis(modo_str, piscina);

    /* Computare latitudinem textus in pixels */
    {
        i32 titulo_pixel_width;
        i32 modo_pixel_width;
        i32 titulo_x, titulo_y;
        i32 modo_x, modo_y;
        i32 gap_padding;

        gap_padding = character_latitudo;  /* Space around text */

        titulo_pixel_width = titulo.mensura * character_latitudo;
        modo_pixel_width = modo_textus.mensura * character_latitudo;

        /* Center titulo on top line */
        titulo_x = box_x0 + ((box_x1 - box_x0) - titulo_pixel_width) / II;
        titulo_y = box_y0 - (character_altitudo / II) + III;  /* +3px to avoid clipping */

        /* Center modo on bottom line */
        modo_x = box_x0 + ((box_x1 - box_x0) - modo_pixel_width) / II;
        modo_y = box_y1 - (character_altitudo / II) + II;  /* +2px positioning */

        /* ==================================================
         * Pingere Double Border (line-gap-line style)
         * ================================================== */

        /* Top line - left segment (outer and inner with gap) */
        delineare_lineam_horizontalem(ctx, box_x0, titulo_x - gap_padding, box_y0, color_border);
        delineare_lineam_horizontalem(ctx, box_x0, titulo_x - gap_padding, box_y0 + II, color_border_inner);

        /* Top line - right segment */
        delineare_lineam_horizontalem(ctx, titulo_x + titulo_pixel_width + gap_padding, box_x1, box_y0, color_border);
        delineare_lineam_horizontalem(ctx, titulo_x + titulo_pixel_width + gap_padding, box_x1, box_y0 + II, color_border_inner);

        /* Bottom line - left segment */
        delineare_lineam_horizontalem(ctx, box_x0, modo_x - gap_padding, box_y1, color_border);
        delineare_lineam_horizontalem(ctx, box_x0, modo_x - gap_padding, box_y1 - II, color_border_inner);

        /* Bottom line - right segment */
        delineare_lineam_horizontalem(ctx, modo_x + modo_pixel_width + gap_padding, box_x1, box_y1, color_border);
        delineare_lineam_horizontalem(ctx, modo_x + modo_pixel_width + gap_padding, box_x1, box_y1 - II, color_border_inner);

        /* Left line (outer and inner with gap) */
        delineare_lineam_verticalem(ctx, box_x0, box_y0, box_y1, color_border);
        delineare_lineam_verticalem(ctx, box_x0 + II, box_y0, box_y1, color_border_inner);

        /* Right line (outer and inner with gap) */
        delineare_lineam_verticalem(ctx, box_x1, box_y0, box_y1, color_border);
        delineare_lineam_verticalem(ctx, box_x1 - II, box_y0, box_y1, color_border_inner);

        /* ==================================================
         * Pingere Textum (centered on lines)
         * ================================================== */

        tabula_pixelorum_pingere_chordam(tabula, titulo_x, titulo_y, titulo, color_border);
        tabula_pixelorum_pingere_chordam(tabula, modo_x, modo_y, modo_textus,
                                          (pagina->modo == MODO_INSERT) ?
                                              thema_color(COLOR_STATUS_INSERT) :
                                              thema_color(COLOR_STATUS_NORMAL));
    }

    /* ==================================================
     * Reddere Textum Paginae (interior)
     * ================================================== */

    pagina_reddere(tabula, pagina, x + I, y + I, textus_latitudo, textus_altitudo, scala);
}
