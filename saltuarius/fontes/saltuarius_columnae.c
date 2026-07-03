/* saltuarius_columnae.c - Implementatio visus columnarum */

#include "saltuarius_columnae.h"
#include "saltuarius_penicillus.h"

/* Colores (spec-v2 par 5: fusca explicita, non OBSCURUM) */
#define COLOR_VIAE        0x00FFCC66
#define COLOR_DIRECTORII  0x0066AAFF
#define COLOR_ALIENI      0x00808080
#define COLOR_VACUI       0x00808080

/* ==================================================
 * Panni
 * ================================================== */

interior vacuum
_pannum_pingere (TesseraOpus* opus, SaltuariusTabularium* tab,
    s32 x, i32 latitudo, constans SaltuariusMetra* metra,
    b32 est_focus)
{
    TesseraStilus nativus = tessera_stilus_nativus();
    s32 k;

    si (tab == NIHIL || latitudo == ZEPHYRUM)
    {
        redde;
    }
    si (tab->numerus == ZEPHYRUM)
    {
        saltuarius_pen_literis(opus, x, metra->contentum_y, "(vacuum)",
            latitudo, tessera_stilus(COLOR_VACUI,
                TESSERA_COLOR_NATIVUS, ZEPHYRUM));
        redde;
    }

    saltuarius_tabularium_aptare(tab, (i32)metra->contentum_alt);

    per (k = ZEPHYRUM; k < metra->contentum_alt; k++)
    {
        s32 index = tab->volumen + k;
        s32 ordo = metra->contentum_y + k;
        constans DirectoriumIntroitus* intr;
        TesseraStilus stilus;
        b32 est_selectum;
        i32 positae;
        character suffixum = '\0';

        si (index >= (s32)tab->numerus)
        {
            frange;
        }
        intr = &tab->introitus[index];
        est_selectum = (index == tab->selectio);

        commutatio (intr->genus)
        {
            casus INTROITUS_DIRECTORIUM:
                stilus = tessera_stilus(COLOR_DIRECTORII,
                    TESSERA_COLOR_NATIVUS,
                    TESSERA_ORNAMENTUM_CRASSUM);
                suffixum = '/';
                frange;
            casus INTROITUS_FILUM:
                stilus = nativus;
                frange;
            ordinarius:
                stilus = tessera_stilus(COLOR_ALIENI,
                    TESSERA_COLOR_NATIVUS, ZEPHYRUM);
                suffixum = '?';
                frange;
        }

        si (est_selectum && est_focus)
        {
            stilus.ornamenta |= TESSERA_ORNAMENTUM_INVERSUM;
            /* tessera_replere: 1.1, signatura ab HOC usu nata */
            tessera_replere(opus, x, ordo, (s32)latitudo, I,
                (i32)' ', stilus);
        }
        alioquin si (est_selectum)
        {
            stilus.ornamenta |= TESSERA_ORNAMENTUM_CRASSUM;
        }

        positae = saltuarius_pen_textum(opus, x, ordo,
            intr->titulus.datum, intr->titulus.mensura,
            (suffixum != '\0') ? latitudo - I : latitudo, stilus);
        si (suffixum != '\0')
        {
            tessera_cellulam_ponere(opus, x + (s32)positae, ordo,
                (i32)suffixum, stilus);
        }
    }
}

interior vacuum
_praevisum_textus_pingere (TesseraOpus* opus, chorda textus,
    constans SaltuariusMetra* metra)
{
    TesseraStilus nativus = tessera_stilus_nativus();
    constans i8* cursor = textus.datum;
    constans i8* finis = textus.datum + textus.mensura;
    s32 ordo = metra->contentum_y;

    dum (cursor < finis
        && ordo < metra->contentum_y + metra->contentum_alt)
    {
        constans i8* linea_finis = cursor;
        i32 linea_mensura;

        dum (linea_finis < finis && *linea_finis != '\n')
        {
            linea_finis++;
        }
        linea_mensura = (i32)(memoriae_index)(linea_finis - cursor);
        /* \r caudae (plagulae CRLF) tondere */
        si (linea_mensura > ZEPHYRUM
            && cursor[linea_mensura - I] == '\r')
        {
            linea_mensura--;
        }
        (vacuum)saltuarius_pen_textum(opus, metra->praevisus_x, ordo,
            cursor, linea_mensura, (i32)metra->praevisus_lat,
            nativus);
        cursor = (linea_finis < finis) ? linea_finis + I : finis;
        ordo++;
    }
}

/* ==================================================
 * Interfacies publica
 * ================================================== */

SaltuariusColumnae*
saltuarius_columnae_creare (Piscina* piscina)
{
    SaltuariusColumnae* columnae;

    columnae = (SaltuariusColumnae*)piscina_allocare_ordinatum(
        piscina, (memoriae_index)magnitudo(SaltuariusColumnae), IV);
    si (columnae == NIHIL)
    {
        redde NIHIL;
    }
    columnae->parens = NIHIL;
    columnae->currens = NIHIL;
    columnae->praevisus_genus = SALT_PRAEVISUS_NIHIL;
    columnae->praevisus_tab = NIHIL;
    columnae->praevisus_textus.mensura = ZEPHYRUM;
    columnae->praevisus_textus.datum = NIHIL;
    columnae->praevisus_nuntius.mensura = ZEPHYRUM;
    columnae->praevisus_nuntius.datum = NIHIL;
    redde columnae;
}

vacuum
saltuarius_columnae_metiri (i32 latitudo, i32 altitudo,
    SaltuariusMetra* metra)
{
    /* ratio 1:3:4 in octavis; separatores singuli inter pannos */
    metra->parens_x = ZEPHYRUM;
    metra->parens_lat = (s32)(latitudo / VIII);
    si (metra->parens_lat < I)
    {
        metra->parens_lat = I;
    }
    metra->currens_x = metra->parens_lat + I;
    metra->currens_lat = (s32)(latitudo * III / VIII);
    si (metra->currens_lat < I)
    {
        metra->currens_lat = I;
    }
    metra->praevisus_x = metra->currens_x + metra->currens_lat + I;
    metra->praevisus_lat = (s32)latitudo - metra->praevisus_x;
    si (metra->praevisus_lat < ZEPHYRUM)
    {
        metra->praevisus_lat = ZEPHYRUM;
    }
    metra->contentum_y = I;
    metra->contentum_alt = (altitudo >= III)
        ? (s32)altitudo - II : ZEPHYRUM;
}

vacuum
saltuarius_columnae_pingere (SaltuariusColumnae* columnae,
    constans SaltuariusRes* res, TesseraOpus* opus)
{
    TesseraStilus nativus = tessera_stilus_nativus();
    SaltuariusMetra metra;
    i32 latitudo = tessera_latitudo(opus);
    i32 altitudo = tessera_altitudo(opus);
    s32 status_y = (s32)altitudo - I;

    tessera_purgare(opus, nativus);
    saltuarius_columnae_metiri(latitudo, altitudo, &metra);

    /* ordo 0: via currens */
    (vacuum)saltuarius_pen_textum(opus, ZEPHYRUM, ZEPHYRUM,
        res->via_currens.datum, res->via_currens.mensura, latitudo,
        tessera_stilus(COLOR_VIAE, TESSERA_COLOR_NATIVUS,
            TESSERA_ORNAMENTUM_CRASSUM));

    /* separatores verticales */
    tessera_lineam_pingere(opus, metra.currens_x - I,
        metra.contentum_y, metra.contentum_alt, VERUM,
        TESSERA_LINEA_SIMPLEX, nativus);
    tessera_lineam_pingere(opus, metra.praevisus_x - I,
        metra.contentum_y, metra.contentum_alt, VERUM,
        TESSERA_LINEA_SIMPLEX, nativus);

    /* panni */
    _pannum_pingere(opus, columnae->parens, metra.parens_x,
        (i32)metra.parens_lat, &metra, FALSUM);
    _pannum_pingere(opus, columnae->currens, metra.currens_x,
        (i32)metra.currens_lat, &metra, VERUM);

    commutatio (columnae->praevisus_genus)
    {
        casus SALT_PRAEVISUS_DIRECTORIUM:
            _pannum_pingere(opus, columnae->praevisus_tab,
                metra.praevisus_x, (i32)metra.praevisus_lat,
                &metra, FALSUM);
            frange;
        casus SALT_PRAEVISUS_TEXTUS:
            _praevisum_textus_pingere(opus,
                columnae->praevisus_textus, &metra);
            frange;
        casus SALT_PRAEVISUS_NUNTIUS:
            (vacuum)saltuarius_pen_textum(opus, metra.praevisus_x,
                metra.contentum_y, columnae->praevisus_nuntius.datum,
                columnae->praevisus_nuntius.mensura,
                (i32)metra.praevisus_lat,
                tessera_stilus(COLOR_VACUI, TESSERA_COLOR_NATIVUS,
                    ZEPHYRUM));
            frange;
        ordinarius:
            frange;
    }

    /* ordo status: sinistra = nuntius aut titulus selectus;
     * dextra = selectio/numerus */
    si (res->nuntius_aetas > ZEPHYRUM)
    {
        (vacuum)saltuarius_pen_textum(opus, ZEPHYRUM, status_y,
            res->nuntius.datum, res->nuntius.mensura, latitudo,
            nativus);
    }
    alioquin si (columnae->currens != NIHIL)
    {
        constans DirectoriumIntroitus* selectum =
            saltuarius_tabularium_selectum(columnae->currens);

        si (selectum != NIHIL && latitudo > X)
        {
            (vacuum)saltuarius_pen_textum(opus, ZEPHYRUM, status_y,
                selectum->titulus.datum, selectum->titulus.mensura,
                latitudo - X, nativus);
        }
    }
    si (columnae->currens != NIHIL
        && columnae->currens->numerus > ZEPHYRUM)
    {
        s32 n = columnae->currens->selectio + I;
        s32 m = (s32)columnae->currens->numerus;
        i32 lat_totalis = saltuarius_pen_digiti(n) + I + saltuarius_pen_digiti(m);
        s32 x = (s32)latitudo - (s32)lat_totalis;
        i32 scripti;

        scripti = saltuarius_pen_numerum(opus, x, status_y, n, nativus);
        tessera_cellulam_ponere(opus, x + (s32)scripti, status_y,
            (i32)'/', nativus);
        (vacuum)saltuarius_pen_numerum(opus, x + (s32)scripti + I,
            status_y, m, nativus);
    }
}
