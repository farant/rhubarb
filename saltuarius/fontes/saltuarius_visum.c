/* saltuarius_visum.c - Implementatio visus fontis */

#include "saltuarius_visum.h"
#include "saltuarius_penicillus.h"
#include "utf8.h"

/* Paleta (INTENTIO Phase B; explicita, temperabilis) */
#define COLOR_CUNULAE     0x00707070
#define COLOR_COMMENTI    0x006A9955
#define COLOR_CHORDAE     0x00CE9178
#define COLOR_NUMERI      0x00B5CEA8
#define COLOR_VERBI       0x00569CD6
#define COLOR_LATINAE     0x00C586C0
#define COLOR_DIRECTIVAE  0x00D7BA7D
#define COLOR_IGNOTI      0x00FF6666

interior TesseraStilus
_stilus_classis (i8 classis)
{
    i32 color;

    commutatio ((SaltuariusClassis)classis)
    {
        casus SALT_CLASSIS_VERBUM:     color = COLOR_VERBI; frange;
        casus SALT_CLASSIS_LATINA:     color = COLOR_LATINAE; frange;
        casus SALT_CLASSIS_COMMENTUM:  color = COLOR_COMMENTI; frange;
        casus SALT_CLASSIS_CHORDA:     color = COLOR_CHORDAE; frange;
        casus SALT_CLASSIS_NUMERUS:    color = COLOR_NUMERI; frange;
        casus SALT_CLASSIS_DIRECTIVA:  color = COLOR_DIRECTIVAE; frange;
        casus SALT_CLASSIS_IGNOTUM:    color = COLOR_IGNOTI; frange;
        ordinarius:
            redde tessera_stilus_nativus();
    }
    redde tessera_stilus(color, TESSERA_COLOR_NATIVUS, ZEPHYRUM);
}

/* Lineam coloratam ponere: saltus volumen_x runarum, deinde runae
 * cum stilo ex tabula classium (offset octeti in TEXTU pleno).
 * Octeti imperii purgati (disciplina penicilli - hic locali quia
 * stilus per runam variat). */
interior vacuum
_lineam_ponere (TesseraOpus* opus, s32 x, s32 y, chorda linea,
    i32 offset_lineae, constans i8* classis, s32 volumen_x,
    i32 runae_max)
{
    constans i8* cursor = linea.datum;
    constans i8* finis = linea.datum + linea.mensura;
    s32 saltandae = volumen_x;
    i32 positae = ZEPHYRUM;

    dum (cursor < finis && saltandae > ZEPHYRUM)
    {
        cursor = utf8_proxima_runa(cursor, finis);
        saltandae--;
    }
    dum (cursor < finis && positae < runae_max)
    {
        constans i8* proxima = utf8_proxima_runa(cursor, finis);
        i32 octeti = (i32)(memoriae_index)(proxima - cursor);
        i32 signum;
        TesseraStilus stilus;

        si (octeti == I
            && (cursor[ZEPHYRUM] < 0x20 || cursor[ZEPHYRUM] == 0x7F))
        {
            signum = (i32)' ';
        }
        alioquin
        {
            signum = tessera_signum_ex_octetis(cursor, octeti);
        }
        si (classis != NIHIL)
        {
            i32 offset = offset_lineae
                + (i32)(memoriae_index)(cursor - linea.datum);

            stilus = _stilus_classis(classis[offset]);
        }
        alioquin
        {
            stilus = tessera_stilus_nativus();
        }
        tessera_cellulam_ponere(opus, x + (s32)positae, y, signum,
            stilus);
        cursor = proxima;
        positae++;
    }
}

vacuum
saltuarius_visum_metiri (constans SaltuariusLiber* liber,
    i32 latitudo, i32 altitudo, SaltuariusVisumMetra* metra)
{
    metra->cunula_lat = (s32)saltuarius_pen_digiti(
        (s32)liber->numerus_linearum);
    metra->textus_x = metra->cunula_lat + I;
    metra->textus_lat = (s32)latitudo - metra->textus_x;
    si (metra->textus_lat < I)
    {
        metra->textus_lat = I;
    }
    metra->contentum_alt = (altitudo >= II)
        ? (s32)altitudo - I : ZEPHYRUM;
}

vacuum
saltuarius_visum_pingere (SaltuariusLiber* liber,
    constans SaltuariusRes* res, TesseraOpus* opus)
{
    TesseraStilus nativus = tessera_stilus_nativus();
    TesseraStilus cunulae = tessera_stilus(COLOR_CUNULAE,
        TESSERA_COLOR_NATIVUS, ZEPHYRUM);
    SaltuariusVisumMetra metra;
    i32 latitudo = tessera_latitudo(opus);
    i32 altitudo = tessera_altitudo(opus);
    s32 status_y = (s32)altitudo - I;
    s32 k;

    tessera_purgare(opus, nativus);
    saltuarius_visum_metiri(liber, latitudo, altitudo, &metra);
    saltuarius_liber_aptare(liber, (i32)metra.contentum_alt,
        (i32)metra.textus_lat);

    per (k = ZEPHYRUM; k < metra.contentum_alt; k++)
    {
        s32 index = liber->volumen_y + k;
        chorda linea;

        si (index >= (s32)liber->numerus_linearum)
        {
            frange;
        }
        /* cunula: numerus 1-basatus, dextro-ordinatus */
        {
            s32 numerus = index + I;
            i32 digiti = saltuarius_pen_digiti(numerus);

            (vacuum)saltuarius_pen_numerum(opus,
                metra.cunula_lat - (s32)digiti, k, numerus,
                cunulae);
        }
        linea = saltuarius_liber_linea(liber, index);
        _lineam_ponere(opus, metra.textus_x, k, linea,
            liber->lineae[index].offset, liber->classis,
            liber->volumen_x, (i32)metra.textus_lat);
    }

    /* cursor verus terminalis (aptare visibilitatem praestat) */
    tessera_cursorem_ponere(opus,
        metra.textus_x + (liber->cursor_columna - liber->volumen_x),
        liber->cursor_linea - liber->volumen_y);

    /* linea status: dextra ([L:C] [stratum 0] [pittacium]) prior;
     * via/nuntius sinistra id quod restat accipit */
    {
        constans character* pittacium = liber->est_colorata
            ? "silva" : "textus merus";
        i32 lat_pittacii = ZEPHYRUM;
        i32 lat_numeri;
        i32 lat_dextrae;
        s32 x;

        dum (pittacium[lat_pittacii] != '\0')
        {
            lat_pittacii++;
        }
        lat_numeri = saltuarius_pen_digiti(liber->cursor_linea + I)
            + I + saltuarius_pen_digiti(liber->cursor_columna + I);
        lat_dextrae = lat_numeri + II + IX + II + lat_pittacii;
        x = (s32)latitudo - (s32)lat_dextrae;

        si (x > II)
        {
            constans chorda* sinistra = (res->nuntius_aetas
                > ZEPHYRUM) ? &res->nuntius : &liber->via;

            (vacuum)saltuarius_pen_textum(opus, ZEPHYRUM, status_y,
                sinistra->datum, sinistra->mensura,
                (i32)(x - II), nativus);
        }
        si (x >= ZEPHYRUM)
        {
            i32 scripti;

            scripti = saltuarius_pen_numerum(opus, x, status_y,
                liber->cursor_linea + I, nativus);
            tessera_cellulam_ponere(opus, x + (s32)scripti,
                status_y, (i32)':', nativus);
            (vacuum)saltuarius_pen_numerum(opus,
                x + (s32)scripti + I, status_y,
                liber->cursor_columna + I, nativus);
            x += (s32)lat_numeri + II;
            saltuarius_pen_literis(opus, x, status_y, "stratum 0",
                IX, cunulae);
            x += IX + II;
            saltuarius_pen_literis(opus, x, status_y, pittacium,
                lat_pittacii, cunulae);
        }
    }
}
