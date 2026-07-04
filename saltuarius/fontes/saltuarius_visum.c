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
        (s32)saltuarius_liber_stratum_activum(liber)
            ->numerus_linearum);
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

    /* TRIPLUM STRATI ACTIVI - textus ET classis ET lineae eiusdem
     * strati (morsus manualis: classis strati 0 in textu strati I
     * colores priores trahebat) */
    {
        constans SaltuariusStratum* visus =
            saltuarius_liber_stratum_activum(liber);

        per (k = ZEPHYRUM; k < metra.contentum_alt; k++)
        {
            s32 index = liber->volumen_y + k;
            chorda linea;

            si (!visus->parata
                || index >= (s32)visus->numerus_linearum)
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
                visus->lineae[index].offset, visus->classis,
                liber->volumen_x, (i32)metra.textus_lat);
        }
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
        i32 lat_strati;
        s32 x;

        dum (pittacium[lat_pittacii] != '\0')
        {
            lat_pittacii++;
        }
        lat_numeri = saltuarius_pen_digiti(liber->cursor_linea + I)
            + I + saltuarius_pen_digiti(liber->cursor_columna + I);
        lat_strati = VIII
            + saltuarius_pen_digiti(liber->stratum_currens) + I
            + saltuarius_pen_digiti(
                (s32)liber->numerus_stratorum - I);
        lat_dextrae = lat_numeri + II + lat_strati + II
            + lat_pittacii;
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
            saltuarius_pen_literis(opus, x, status_y, "stratum ",
                VIII, cunulae);
            x += VIII;
            x += (s32)saltuarius_pen_numerum(opus, x, status_y,
                liber->stratum_currens, cunulae);
            tessera_cellulam_ponere(opus, x, status_y, (i32)'/',
                cunulae);
            x += I;
            x += (s32)saltuarius_pen_numerum(opus, x, status_y,
                (s32)liber->numerus_stratorum - I, cunulae);
            x += II;
            saltuarius_pen_literis(opus, x, status_y, pittacium,
                lat_pittacii, cunulae);
        }
    }
}

vacuum
saltuarius_visum_tabella (constans SaltuariusOrigo* origo,
    TesseraOpus* opus)
{
    TesseraStilus nativus = tessera_stilus_nativus();
    i32 latitudo = tessera_latitudo(opus);
    s32 lat_tabellae;
    s32 alt_tabellae;
    s32 x;
    s32 y = II;
    s32 k;

    si (!origo->apertum || origo->numerus == ZEPHYRUM)
    {
        redde;
    }
    lat_tabellae = (s32)latitudo - IV;
    si (lat_tabellae > LX)
    {
        lat_tabellae = LX;
    }
    si (lat_tabellae < X)
    {
        redde;   /* terminal nimis angustum */
    }
    alt_tabellae = (s32)origo->numerus + II;
    x = ((s32)latitudo - lat_tabellae) / II;

    /* interior opacum PRIMO (tessera_replere - casus rectanguli),
     * deinde margo, deinde ordines */
    tessera_replere(opus, x, y, lat_tabellae, alt_tabellae,
        (i32)' ', nativus);
    tessera_quadrum_pingere(opus, x, y, lat_tabellae, alt_tabellae,
        TESSERA_LINEA_ROTUNDATA, nativus);
    saltuarius_pen_literis(opus, x + II, y, " ORIGO ", VII,
        tessera_stilus(0x00FFCC66, TESSERA_COLOR_NATIVUS,
            TESSERA_ORNAMENTUM_CRASSUM));

    per (k = ZEPHYRUM; k < (s32)origo->numerus; k++)
    {
        constans SaltuariusGradus* gradus = &origo->gradus[k];
        TesseraStilus stilus = nativus;

        si (k == origo->selectio)
        {
            stilus.ornamenta |= TESSERA_ORNAMENTUM_INVERSUM;
            tessera_replere(opus, x + I, y + I + k,
                lat_tabellae - II, I, (i32)' ', stilus);
        }
        alioquin si (!gradus->saltabile)
        {
            stilus = tessera_stilus(0x00808080,
                TESSERA_COLOR_NATIVUS, ZEPHYRUM);
        }
        (vacuum)saltuarius_pen_textum(opus, x + II, y + I + k,
            gradus->titulus.datum, gradus->titulus.mensura,
            (i32)(lat_tabellae - IV), stilus);
    }
}
