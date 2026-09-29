/* tessera_opus.c - Implementatio operis (Phase A) */

#include "tessera_opus.h"
#include "tessera_modi.h"
#include "utf8.h"
#include "runae.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Cellula vacua: signum 0, colores nativi, sine ornamentis.
 * Pictura prima cellulas HUIC aequales praeterit (ED 2J eas iam
 * pinxit). */
hic_manens constans TesseraCellula CELLULA_VACUA = {
    ZEPHYRUM, TESSERA_COLOR_NATIVUS, TESSERA_COLOR_NATIVUS, ZEPHYRUM
};

interior b32
_cellulae_aequales (
    constans TesseraCellula* a,
    constans TesseraCellula* b)
{
    redde (a->signum == b->signum
        && a->color_litterae == b->color_litterae
        && a->color_fundi == b->color_fundi
        && a->ornamenta == b->ornamenta) ? VERUM : FALSUM;
}

interior b32
_in_finibus (
    constans TesseraOpus* opus,
                     s32  x,
                     s32  y)
{
    redde (x >= ZEPHYRUM && y >= ZEPHYRUM
        && x < (s32)opus->latitudo && y < (s32)opus->altitudo)
        ? VERUM : FALSUM;
}

interior i32
_index (
    s32 x,
    s32 y)
{
    redde (i32)y * TESSERA_LATITUDO_MAXIMA + (i32)x;
}

/* CUP: "\033[<y+1>;<x+1>H" (1-basatum) */
interior vacuum
_positum_emittere (
    ChordaAedificator* aed,
                  i32  x,
                  i32  y)
{
    chorda_aedificator_appendere_literis(aed, "\033[");
    chorda_aedificator_appendere_i32(aed, y + I);
    chorda_aedificator_appendere_character(aed, ';');
    chorda_aedificator_appendere_i32(aed, x + I);
    chorda_aedificator_appendere_character(aed, 'H');
}

/* SGR: reditio plena + ornamenta + colores (nativus = nihil -
 * reditio nuda defaltas terminalis dat) */
interior vacuum
_stilum_emittere (
         ChordaAedificator* aed,
    constans TesseraStilus* st)
{
    chorda_aedificator_appendere_literis(aed, "\033[0");
    si (st->ornamenta & TESSERA_ORNAMENTUM_CRASSUM)
    {
        chorda_aedificator_appendere_literis(aed, ";1");
    }
    si (st->ornamenta & TESSERA_ORNAMENTUM_OBSCURUM)
    {
        chorda_aedificator_appendere_literis(aed, ";2");
    }
    si (st->ornamenta & TESSERA_ORNAMENTUM_CURSIVUM)
    {
        chorda_aedificator_appendere_literis(aed, ";3");
    }
    si (st->ornamenta & TESSERA_ORNAMENTUM_SUBLINEATUM)
    {
        chorda_aedificator_appendere_literis(aed, ";4");
    }
    si (st->ornamenta & TESSERA_ORNAMENTUM_INVERSUM)
    {
        chorda_aedificator_appendere_literis(aed, ";7");
    }
    si (st->ornamenta & TESSERA_ORNAMENTUM_TRANSFIXUM)
    {
        chorda_aedificator_appendere_literis(aed, ";9");
    }
    si (st->color_litterae != TESSERA_COLOR_NATIVUS)
    {
        chorda_aedificator_appendere_literis(aed, ";38;2;");
        chorda_aedificator_appendere_i32(aed,
            (st->color_litterae >> XVI) & 0xFF);
        chorda_aedificator_appendere_character(aed, ';');
        chorda_aedificator_appendere_i32(aed,
            (st->color_litterae >> VIII) & 0xFF);
        chorda_aedificator_appendere_character(aed, ';');
        chorda_aedificator_appendere_i32(aed,
            st->color_litterae & 0xFF);
    }
    si (st->color_fundi != TESSERA_COLOR_NATIVUS)
    {
        chorda_aedificator_appendere_literis(aed, ";48;2;");
        chorda_aedificator_appendere_i32(aed,
            (st->color_fundi >> XVI) & 0xFF);
        chorda_aedificator_appendere_character(aed, ';');
        chorda_aedificator_appendere_i32(aed,
            (st->color_fundi >> VIII) & 0xFF);
        chorda_aedificator_appendere_character(aed, ';');
        chorda_aedificator_appendere_i32(aed, st->color_fundi & 0xFF);
    }
    chorda_aedificator_appendere_character(aed, 'm');
}

TesseraOpus*
tessera_aperire (
        Piscina* piscina,
    TesseraPons* pons)
{
       TesseraOpus* opus;
    memoriae_index  cellulae = (memoriae_index)TESSERA_LATITUDO_MAXIMA
        * (memoriae_index)TESSERA_ALTITUDO_MAXIMA;
    i32 latitudo;
    i32 altitudo;
    i32 k;

    si (piscina == NIHIL || pons == NIHIL)
    {
        redde NIHIL;  /* defalta posix = Phase B */
    }
    si (!pons->amplitudo(pons->datum, &latitudo, &altitudo))
    {
        redde NIHIL;
    }
    opus = (TesseraOpus*)piscina_allocare_ordinatum(piscina,
        (memoriae_index)magnitudo(TesseraOpus), IV);
    si (opus == NIHIL)
    {
        redde NIHIL;
    }
    opus->piscina  = piscina;
    opus->pons     = pons;
    opus->frons = (TesseraCellula*)piscina_allocare_ordinatum(piscina,
        cellulae * magnitudo(TesseraCellula), IV);
    opus->tergum = (TesseraCellula*)piscina_allocare_ordinatum(
        piscina, cellulae * magnitudo(TesseraCellula), IV);
    si (opus->frons == NIHIL || opus->tergum == NIHIL)
    {
        redde NIHIL;
    }
    opus->latitudo = (latitudo > ZEPHYRUM)
        ? ((latitudo <= TESSERA_LATITUDO_MAXIMA)
            ? latitudo : TESSERA_LATITUDO_MAXIMA)
        : I;
    opus->altitudo = (altitudo > ZEPHYRUM)
        ? ((altitudo <= TESSERA_ALTITUDO_MAXIMA)
            ? altitudo : TESSERA_ALTITUDO_MAXIMA)
        : I;

    /* tergum vacuum; frons post picturam primam impletur */
    per (k = ZEPHYRUM; k < (i32)cellulae; k++)
    {
        opus->tergum[k] = CELLULA_VACUA;
    }

    /* praedimensus: cellulae activae * ~20 octeti + effugia fixa -
     * status stabilis nihil allocat (spectare + reset) */
    opus->aed = chorda_aedificator_creare(piscina,
        (memoriae_index)(opus->latitudo * opus->altitudo) * XX
            + 1024);
    si (opus->aed == NIHIL)
    {
        redde NIHIL;
    }

    /* tabula graphematum (runae U5b): praeparata, crescit tantum */
    opus->graphemata_octeti = (i8*)piscina_allocare(piscina,
        (memoriae_index)TESSERA_GRAPHEMATA_OCTETI);
    opus->graphemata_initia = (i32*)piscina_allocare_ordinatum(piscina,
        (memoriae_index)TESSERA_GRAPHEMATA_MAXIMA * magnitudo(i32), IV);
    opus->graphemata_longitudines = (i8*)piscina_allocare(piscina,
        (memoriae_index)TESSERA_GRAPHEMATA_MAXIMA);
    opus->graphemata_index = (i32*)piscina_allocare_ordinatum(piscina,
        (memoriae_index)(II * TESSERA_GRAPHEMATA_MAXIMA) * magnitudo(i32),
        IV);
    si (   opus->graphemata_octeti       == NIHIL
        || opus->graphemata_initia       == NIHIL
        || opus->graphemata_longitudines == NIHIL
        || opus->graphemata_index        == NIHIL)
    {
        redde NIHIL;
    }
    memset(opus->graphemata_index, ZEPHYRUM,
        (memoriae_index)(II * TESSERA_GRAPHEMATA_MAXIMA) * magnitudo(i32));
    opus->graphemata_octeti_usi  = ZEPHYRUM;
    opus->graphemata_numerus     = ZEPHYRUM;
    opus->politica               = TESSERA_POLITICA_GRAPHEMATUM;

    opus->cursor_x                        = -I;
    opus->cursor_y                        = -I;
    opus->cursor_x_actus                  = -I;
    opus->cursor_y_actus                  = -I;
    opus->cursor_visibilis_actus          = FALSUM;
    opus->primum                          = VERUM;
    opus->fructus.cellulae_collatae       = ZEPHYRUM;
    opus->fructus.cellulae_mutatae        = ZEPHYRUM;
    opus->fructus.octeti_emissi           = ZEPHYRUM;
    opus->fructus.praesentationes         = ZEPHYRUM;
    opus->fructus.tempus_praesentandi_ms  = 0.0;

    si (!pons->intrare(pons->datum))
    {
        redde NIHIL;
    }
    redde opus;
}

vacuum
tessera_claudere (
    TesseraOpus* opus)
{
    si (opus == NIHIL)
    {
        redde;
    }
    /* SGR reditio + cursor visibilis - scrinium mundum relinquere */
    chorda_aedificator_reset(opus->aed);
    chorda_aedificator_appendere_literis(opus->aed, "\033[0m\033[?25h");
    {
        chorda visus = chorda_aedificator_spectare(opus->aed);

        opus->pons->scribere(opus->pons->datum, visus.datum,
            (i32)visus.mensura);
    }
    opus->pons->egredi(opus->pons->datum);
}

vacuum
tessera_intermittere (
    TesseraOpus* opus)
{
    si (opus == NIHIL)
    {
        redde;
    }
    chorda_aedificator_reset(opus->aed);
    /* quadrum synchronum (si apertum) claudere ante $EDITOR - cautela */
    chorda_aedificator_appendere_literis(opus->aed,
        QUADRUM_FINIS "\033[0m\033[?25h");
    {
        chorda visus = chorda_aedificator_spectare(opus->aed);

        opus->pons->scribere(opus->pons->datum, visus.datum,
            (i32)visus.mensura);
    }
    opus->pons->egredi(opus->pons->datum);
}

vacuum
tessera_resumere (
    TesseraOpus* opus)
{
    si (opus == NIHIL)
    {
        redde;
    }
    opus->pons->intrare(opus->pons->datum);
    opus->primum                  = VERUM;   /* pictura plena sequitur */
    opus->cursor_x_actus          = -I;
    opus->cursor_y_actus          = -I;
    opus->cursor_visibilis_actus  = FALSUM;
}

i32
tessera_latitudo (
    constans TesseraOpus* opus)
{
    redde (opus != NIHIL) ? opus->latitudo : ZEPHYRUM;
}

i32
tessera_altitudo (
    constans TesseraOpus* opus)
{
    redde (opus != NIHIL) ? opus->altitudo : ZEPHYRUM;
}

vacuum
tessera_politicam_ponere (
        TesseraOpus* opus,
    TesseraPolitica  politica)
{
    si (opus != NIHIL)
    {
        opus->politica = politica;
    }
}

TesseraPolitica
tessera_politica_ambitus (vacuum)
{
    constans character* programma = getenv("TERM_PROGRAM");

    si (   programma                           != NIHIL
        && strcmp(programma, "Apple_Terminal") == ZEPHYRUM)
    {
        redde TESSERA_POLITICA_SIMPLEX;
    }
    redde TESSERA_POLITICA_GRAPHEMATUM;
}

vacuum
tessera_purgare (
      TesseraOpus* opus,
    TesseraStilus  stilus)
{
    s32 x;
    s32 y;

    si (opus == NIHIL)
    {
        redde;
    }
    per (y = ZEPHYRUM; y < (s32)opus->altitudo; y++)
    {
        per (x = ZEPHYRUM; x < (s32)opus->latitudo; x++)
        {
            TesseraCellula* cella = &opus->tergum[_index(x, y)];

            cella->signum          = ZEPHYRUM;
            cella->color_litterae  = stilus.color_litterae;
            cella->color_fundi     = stilus.color_fundi;
            cella->ornamenta       = stilus.ornamenta
                & TESSERA_ORNAMENTA_STILI;
        }
    }
}

/* Latitudo signi compacti (runae U5): ASCII = I sine decodificatione;
 * ceterum runae_latitudo. Latitudo 0 hic = I (vocans cellulam
 * explicite posuit; scriptio signa componentia ante omittit). */
interior i32
_latitudo_signi (
    i32 signum)
{
             i8  octeti[IV];
            i32  k;
    constans i8* cursor = octeti;
            s32  runa;

    si (signum < 0x80)
    {
        redde I;
    }
    per (k = ZEPHYRUM; k < IV; k++)
    {
        octeti[k] = (i8)((signum >> (VIII * k)) & 0xFF);
    }
    runa = utf8_decodere(&cursor, octeti
        + tessera_signum_mensura(signum));
    redde (runae_latitudo(runa) == II) ? II : I;
}

/* Runam latam quae per (x, y) scinderetur solvere: si (x, y) est
 * continuatio, initium eius vacuatur; si initium, continuatio eius.
 * Cellula ipsa a vocante mox scribitur. */
interior vacuum
_dimidium_solvere (
    TesseraOpus* opus,
            s32  x,
            s32  y)
{
    TesseraCellula* cella = &opus->tergum[_index(x, y)];

    si (   (cella->ornamenta & TESSERA_ORNAMENTUM_CONTINUATIO)
        && _in_finibus(opus, x - I, y))
    {
        TesseraCellula* initium = &opus->tergum[_index(x - I, y)];

        initium->signum     = ZEPHYRUM;
        initium->ornamenta &= ~(i32)(TESSERA_ORNAMENTUM_LATUM
                                     | TESSERA_ORNAMENTUM_GRAPHEMA);
    }
    si (   (cella->ornamenta & TESSERA_ORNAMENTUM_LATUM)
        && _in_finibus(opus, x + I, y))
    {
        TesseraCellula* continuatio = &opus->tergum[_index(x + I, y)];

        continuatio->signum     = ZEPHYRUM;
        continuatio->ornamenta  &= ~(i32)TESSERA_ORNAMENTUM_CONTINUATIO;
    }
}

/* Graphema plurium runarum internare: ID (idem pro octetis aequalibus)
 * aut -1 si limes (tabula plena, arena plena, graphema longius).
 * Dispersio FNV-1a, tentatio linearis; loculi ID+1 (0 = vacuus). */
interior s32
_graphema_internare (
    TesseraOpus* opus,
    constans i8* octeti,
            i32  mensura)
{
    i32 friatio = 2166136261u;
    i32 mascula = (i32)(II * TESSERA_GRAPHEMATA_MAXIMA) - I;
    i32 loculus;
    i32 k;
    i32 id;

    si (mensura <= ZEPHYRUM || mensura > TESSERA_GRAPHEMA_OCTETI_MAXIMI)
    {
        redde -I;
    }
    per (k = ZEPHYRUM; k < mensura; k++)
    {
        friatio ^= (i32)octeti[k];
        friatio *= 16777619;
    }
    loculus = friatio & mascula;
    dum (opus->graphemata_index[loculus] != ZEPHYRUM)
    {
        id = opus->graphemata_index[loculus] - I;
        si (   (i32)opus->graphemata_longitudines[id] == mensura
            && memcmp(opus->graphemata_octeti
                + opus->graphemata_initia[id],
                   octeti, (memoriae_index)mensura) == ZEPHYRUM)
        {
            redde (s32)id;
        }
        loculus = (loculus + I) & mascula;
    }
    si (   opus->graphemata_numerus >= (i32)TESSERA_GRAPHEMATA_MAXIMA
        || opus->graphemata_octeti_usi + mensura
               > (i32)TESSERA_GRAPHEMATA_OCTETI)
    {
        redde -I;   /* limes: vocans ad runam primam redit */
    }
    id = opus->graphemata_numerus++;
    memcpy(opus->graphemata_octeti + opus->graphemata_octeti_usi,
        octeti,
        (memoriae_index)mensura);
    opus->graphemata_initia[id]        = opus->graphemata_octeti_usi;
    opus->graphemata_longitudines[id]  = (i8)mensura;
    opus->graphemata_octeti_usi        += mensura;
    opus->graphemata_index[loculus]    = id + I;
    redde (s32)id;
}

/* Nucleus collocationis: latitudo data (I aut II), graphema = signum
 * est ID internatum. Regulae U5: dimidia scissa vacuantur, columna
 * ultima spatium fit. */
interior vacuum
_cellulam_collocare (
      TesseraOpus* opus,
              s32  x,
              s32  y,
              i32  signum,
    TesseraStilus  stilus,
              i32  latitudo,
              b32  graphema)
{
    TesseraCellula* cella;
               i32  ornamenta =
                   stilus.ornamenta & TESSERA_ORNAMENTA_STILI;

    si (opus == NIHIL || !_in_finibus(opus, x, y))
    {
        redde;  /* praecisio taciturna */
    }
    _dimidium_solvere(opus, x, y);
    si (latitudo == II)
    {
        si (_in_finibus(opus, x + I, y))
        {
            _dimidium_solvere(opus, x + I, y);
        }
        alioquin
        {
            signum    = ZEPHYRUM;   /* columna ultima: spatium, numquam
                                     * scissa neque involuta */
            latitudo = I;
            graphema = FALSUM;
        }
    }
    cella                  = &opus->tergum[_index(x, y)];
    cella->signum          = signum;
    cella->color_litterae  = stilus.color_litterae;
    cella->color_fundi     = stilus.color_fundi;
    cella->ornamenta       = ornamenta
        | ((latitudo == II) ? TESSERA_ORNAMENTUM_LATUM : ZEPHYRUM)
        | (graphema ? TESSERA_ORNAMENTUM_GRAPHEMA : ZEPHYRUM);
    si (latitudo == II)
    {
        TesseraCellula* continuatio = &opus->tergum[_index(x + I, y)];

        continuatio->signum          = ZEPHYRUM;
        continuatio->color_litterae  = stilus.color_litterae;
        continuatio->color_fundi     = stilus.color_fundi;
        continuatio->ornamenta       = ornamenta
            | TESSERA_ORNAMENTUM_CONTINUATIO;
    }
}

vacuum
tessera_cellulam_ponere (
      TesseraOpus* opus,
              s32  x,
              s32  y,
              i32  signum,
    TesseraStilus  stilus)
{
    _cellulam_collocare(opus, x, y, signum, stilus,
        _latitudo_signi(signum), FALSUM);
}

TesseraCellula
tessera_cellulam_legere (
    constans TesseraOpus* opus,
                     s32  x,
                     s32  y)
{
    si (opus == NIHIL || !_in_finibus(opus, x, y))
    {
        redde CELLULA_VACUA;
    }
    redde opus->tergum[_index(x, y)];
}

i32
tessera_cellulae_octeti (
    constans TesseraOpus* opus,
                     s32  x,
                     s32  y,
                      i8* exitus,
                     i32  capacitas)
{
    TesseraCellula cella;
               i32 n;
               i32 k;

    si (opus == NIHIL || exitus == NIHIL || !_in_finibus(opus, x, y))
    {
        redde ZEPHYRUM;
    }
    cella = opus->tergum[_index(x, y)];
    si (cella.ornamenta & TESSERA_ORNAMENTUM_GRAPHEMA)
    {
        n = (i32)opus->graphemata_longitudines[cella.signum];
        si (n > capacitas)
        {
            redde ZEPHYRUM;
        }
        memcpy(exitus, opus->graphemata_octeti
            + opus->graphemata_initia[cella.signum], (memoriae_index)n);
        redde n;
    }
    n = tessera_signum_mensura(cella.signum);
    si (n > capacitas)
    {
        redde ZEPHYRUM;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        exitus[k] = (i8)((cella.signum >> (VIII * k)) & 0xFF);
    }
    redde n;
}

/* Nucleus scriptionis (parametra constantia - scribere_literis
 * qualificatorem numquam abicit) */
interior vacuum
_octetos_scribere (
      TesseraOpus* opus,
              s32  x,
              s32  y,
      constans i8* datum,
              i32  mensura,
    TesseraStilus  stilus)
{
    constans i8* cursor;
    constans i8* finis;
            s32  cx = x;

    si (opus == NIHIL || datum == NIHIL)
    {
        redde;
    }
    cursor  = datum;
    finis   = datum + mensura;
    dum (cursor < finis)
    {
                  i8  primus      = *cursor;
         constans i8* post_runae  = cursor;
         constans i8* post;
                 s32  runa;
                 i32  latitudo;

        si (primus < 0x20 || primus == 0x7F)
        {
            /* octetus regiminis -> '?' */
            _cellulam_collocare(opus, cx, y, '?', stilus, I, FALSUM);
            cursor++;
            cx++;
            perge;
        }
        runa = utf8_decodere(&post_runae, finis);
        si (runa < ZEPHYRUM)
        {
            /* series invalida -> '?' (octetus unus) */
            _cellulam_collocare(opus, cx, y, '?', stilus, I, FALSUM);
            cursor++;
            cx++;
            perge;
        }
        /* graphema (UAX #29) et latitudo eius (Ghostty) */
        post = runae_graphema_ex_politica(cursor, finis,
            (opus->politica == TESSERA_POLITICA_SIMPLEX)
                ? RUNAE_POLITICA_SIMPLEX : RUNAE_POLITICA_GRAPHEMATUM,
            &latitudo);
        si (latitudo == ZEPHYRUM)
        {
            cursor = post;   /* signum sine basi: nihil pingitur */
            perge;
        }
        si (post == post_runae)
        {
            _cellulam_collocare(opus, cx, y,
                tessera_signum_ex_octetis(cursor, (i32)(post - cursor)),
                stilus, latitudo, FALSUM);
        }
        alioquin
        {
            s32 id = _graphema_internare(opus, cursor, (i32)(post
                - cursor));

            si (id >= ZEPHYRUM)
            {
                _cellulam_collocare(opus, cx, y, (i32)id, stilus,
                    latitudo,
                    VERUM);
            }
            alioquin
            {
                /* limes tabulae: runa prima sola (modus U5) */
                latitudo = runae_latitudo(runa);
                si (latitudo > ZEPHYRUM)
                {
                    _cellulam_collocare(opus, cx, y,
                        tessera_signum_ex_octetis(cursor,
                            (i32)(post_runae - cursor)),
                        stilus, latitudo, FALSUM);
                }
            }
        }
        cursor  = post;
        cx      += (s32)latitudo;
    }
}

vacuum
tessera_scribere (
      TesseraOpus* opus,
              s32  x,
              s32  y,
           chorda  textus,
    TesseraStilus  stilus)
{
    _octetos_scribere(opus, x, y, textus.datum, textus.mensura,
        stilus);
}

vacuum
tessera_scribere_literis (
           TesseraOpus* opus,
                   s32  x,
                   s32  y,
    constans character* textus,
         TesseraStilus  stilus)
{
    si (textus == NIHIL)
    {
        redde;
    }
    _octetos_scribere(opus, x, y, (constans i8*)textus,
        (i32)strlen(textus), stilus);
}

/* Signa marginum per genus: h, v, ss, sd, is, id */
interior vacuum
_signa_lineae (
    TesseraLineaGenus  genus,
                  i32* signa)
{
    commutatio (genus)
    {
        casus TESSERA_LINEA_DUPLEX:
            signa[ZEPHYRUM] = TESSERA_SIGNUM_DUPLEX_H;
            signa[I] = TESSERA_SIGNUM_DUPLEX_V;
            signa[II] = TESSERA_SIGNUM_DUPLEX_SS;
            signa[III] = TESSERA_SIGNUM_DUPLEX_SD;
            signa[IV] = TESSERA_SIGNUM_DUPLEX_IS;
            signa[V] = TESSERA_SIGNUM_DUPLEX_ID;
            frange;
        casus TESSERA_LINEA_ROTUNDATA:
            signa[ZEPHYRUM] = TESSERA_SIGNUM_SIMPLEX_H;
            signa[I] = TESSERA_SIGNUM_SIMPLEX_V;
            signa[II] = TESSERA_SIGNUM_ROTUNDATUM_SS;
            signa[III] = TESSERA_SIGNUM_ROTUNDATUM_SD;
            signa[IV] = TESSERA_SIGNUM_ROTUNDATUM_IS;
            signa[V] = TESSERA_SIGNUM_ROTUNDATUM_ID;
            frange;
        ordinarius:
            signa[ZEPHYRUM] = TESSERA_SIGNUM_SIMPLEX_H;
            signa[I] = TESSERA_SIGNUM_SIMPLEX_V;
            signa[II] = TESSERA_SIGNUM_SIMPLEX_SS;
            signa[III] = TESSERA_SIGNUM_SIMPLEX_SD;
            signa[IV] = TESSERA_SIGNUM_SIMPLEX_IS;
            signa[V] = TESSERA_SIGNUM_SIMPLEX_ID;
            frange;
    }
}

vacuum
tessera_quadrum_pingere (
          TesseraOpus* opus,
                  s32  x,
                  s32  y,
                  s32  latitudo,
                  s32  altitudo,
    TesseraLineaGenus  genus,
        TesseraStilus  stilus)
{
    i32 signa[VI];
    s32 k;

    si (opus == NIHIL || latitudo < II || altitudo < II)
    {
        redde;
    }
    _signa_lineae(genus, signa);

    tessera_cellulam_ponere(opus, x, y, signa[II], stilus);
    tessera_cellulam_ponere(opus, x + latitudo - I, y, signa[III],
        stilus);
    tessera_cellulam_ponere(opus, x, y + altitudo - I, signa[IV],
        stilus);
    tessera_cellulam_ponere(opus, x + latitudo - I, y + altitudo - I,
        signa[V], stilus);
    per (k = I; k < latitudo - I; k++)
    {
        tessera_cellulam_ponere(opus, x + k, y, signa[ZEPHYRUM],
            stilus);
        tessera_cellulam_ponere(opus, x + k, y + altitudo - I,
            signa[ZEPHYRUM], stilus);
    }
    per (k = I; k < altitudo - I; k++)
    {
        tessera_cellulam_ponere(opus, x, y + k, signa[I], stilus);
        tessera_cellulam_ponere(opus, x + latitudo - I, y + k,
            signa[I], stilus);
    }
}

vacuum
tessera_lineam_pingere (
          TesseraOpus* opus,
                  s32  x,
                  s32  y,
                  s32  longitudo,
                  b32  verticalis,
    TesseraLineaGenus  genus,
        TesseraStilus  stilus)
{
    i32 signa[VI];
    i32 signum;
    s32 k;

    si (opus == NIHIL || longitudo <= ZEPHYRUM)
    {
        redde;
    }
    _signa_lineae(genus, signa);
    signum = verticalis ? signa[I] : signa[ZEPHYRUM];
    per (k = ZEPHYRUM; k < longitudo; k++)
    {
        tessera_cellulam_ponere(opus, verticalis ? x : (x + k),
            verticalis ? (y + k) : y, signum, stilus);
    }
}

vacuum
tessera_replere (
      TesseraOpus* opus,
              s32  x,
              s32  y,
              s32  latitudo,
              s32  altitudo,
              i32  signum,
    TesseraStilus  stilus)
{
    s32 dx;
    s32 dy;

    si (opus == NIHIL)
    {
        redde;
    }
    per (dy = ZEPHYRUM; dy < altitudo; dy++)
    {
        per (dx = ZEPHYRUM; dx < latitudo; dx++)
        {
            tessera_cellulam_ponere(opus, x + dx, y + dy, signum,
                stilus);
        }
    }
}

vacuum
tessera_cursorem_ponere (
    TesseraOpus* opus,
            s32  x,
            s32  y)
{
    si (opus == NIHIL)
    {
        redde;
    }
    opus->cursor_x = x;
    opus->cursor_y = y;
}

b32
tessera_praesentare (
    TesseraOpus* opus)
{
          clock_t t0;
              s32 pos_x = -I;
              s32 pos_y = -I;
    TesseraStilus stilus_currens;
              b32 stilus_validus  = FALSUM;
              i32 mutatae_quadri  = ZEPHYRUM;
              b32 successus       = VERUM;
              s32 x;
              s32 y;

    si (opus == NIHIL)
    {
        redde FALSUM;
    }
    t0 = clock();
    chorda_aedificator_reset(opus->aed);
    /* Quadrum synchronum (?2026): initium semper praemittitur; si
     * nihil sequitur, quadrum vacuum manet (nulli octeti, ut prius) */
    chorda_aedificator_appendere_literis(opus->aed, QUADRUM_INITIUM);
    stilus_currens = tessera_stilus_nativus();

    si (opus->primum)
    {
        chorda_aedificator_appendere_literis(opus->aed,
            "\033[?25l\033[2J");
        opus->cursor_visibilis_actus  = FALSUM;
        opus->cursor_x_actus          = -I;
        opus->cursor_y_actus          = -I;
    }

    per (y = ZEPHYRUM; y < (s32)opus->altitudo; y++)
    {
        per (x = ZEPHYRUM; x < (s32)opus->latitudo; x++)
        {
                                i32 idx     =
                                    _index(x, y);
            constans TesseraCellula* cella  = &opus->tergum[idx];
                                b32  pingenda;

            opus->fructus.cellulae_collatae++;
            si (cella->ornamenta & TESSERA_ORNAMENTUM_CONTINUATIO)
            {
                /* dimidium secundum numquam emittitur: initium eius
                 * (cellula praecedens) pro utroque pingitur */
                opus->frons[idx] = *cella;
                perge;
            }
            si (opus->primum)
            {
                pingenda = !_cellulae_aequales(cella, &CELLULA_VACUA);
                opus->frons[idx] = *cella;
            }
            alioquin
            {
                pingenda = !_cellulae_aequales(cella,
                    &opus->frons[idx]);
                si (   !pingenda
                    && (cella->ornamenta & TESSERA_ORNAMENTUM_LATUM)
                    && x + I < (s32)opus->latitudo)
                {
                    /* continuatio mutata, initium non: repingere */
                    pingenda = !_cellulae_aequales(&opus->tergum[idx
                        + I],
                        &opus->frons[idx + I]);
                }
            }
            si (!pingenda)
            {
                perge;
            }

            si (!(pos_x == x && pos_y == y))
            {
                _positum_emittere(opus->aed, (i32)x, (i32)y);
            }
            {
                TesseraStilus stilus_cellae;

                stilus_cellae.color_litterae  = cella->color_litterae;
                stilus_cellae.color_fundi     = cella->color_fundi;
                stilus_cellae.ornamenta       = cella->ornamenta
                    & TESSERA_ORNAMENTA_STILI;
                si (   !stilus_validus
                    || !tessera_stilus_aequalis(stilus_currens,
                           stilus_cellae))
                {
                    _stilum_emittere(opus->aed, &stilus_cellae);
                    stilus_currens = stilus_cellae;
                    stilus_validus = VERUM;
                }
            }
            si (cella->ornamenta & TESSERA_ORNAMENTUM_GRAPHEMA)
            {
                chorda graphema;

                graphema.datum    = opus->graphemata_octeti
                    + opus->graphemata_initia[cella->signum];
                graphema.mensura  = (i32)
                    opus->graphemata_longitudines[cella->signum];
                chorda_aedificator_appendere_chorda(opus->aed,
                    graphema);
            }
            alioquin
            {
                tessera_signum_scribere(opus->aed, cella->signum);
            }
            opus->frons[idx] = *cella;
            opus->fructus.cellulae_mutatae++;
            mutatae_quadri++;

            /* terminal consentiens cursorem per latitudinem movet */
            pos_x = x + ((cella->ornamenta & TESSERA_ORNAMENTUM_LATUM)
                             ? II : I);
            pos_y = y;
                        si (cella->ornamenta & (TESSERA_ORNAMENTUM_LATUM
                                    | TESSERA_ORNAMENTUM_GRAPHEMA))
                        {
                /* CONTINENTIA (etiam post graphema plurium runarum:
                 * ibi terminalia maxime dissentiunt): terminal dissentiens de latitudine
                 * cellulam unam laedit, non ordinem - CUP ante
                 * proximam (features/004) */
                pos_x = -I;
                        }
            si (pos_x >= (s32)opus->latitudo)
            {
                pos_x = -I;  /* involutio numquam creditur */
            }
        }
    }

    /* Cursor in fine quadri applicatur */
    si (opus->cursor_x >= ZEPHYRUM && opus->cursor_y >= ZEPHYRUM)
    {
        si (   mutatae_quadri > ZEPHYRUM
            || opus->cursor_x != opus->cursor_x_actus
            || opus->cursor_y != opus->cursor_y_actus
            || !opus->cursor_visibilis_actus)
        {
            _positum_emittere(opus->aed, (i32)opus->cursor_x,
                (i32)opus->cursor_y);
            si (!opus->cursor_visibilis_actus)
            {
                chorda_aedificator_appendere_literis(opus->aed,
                    "\033[?25h");
            }
            opus->cursor_x_actus          = opus->cursor_x;
            opus->cursor_y_actus          = opus->cursor_y;
            opus->cursor_visibilis_actus  = VERUM;
        }
    }
    alioquin si (opus->cursor_visibilis_actus)
    {
        chorda_aedificator_appendere_literis(opus->aed, "\033[?25l");
        opus->cursor_visibilis_actus = FALSUM;
    }

    {
        chorda visus = chorda_aedificator_spectare(opus->aed);

        /* plus quam initium synchroniae = quadrum non vacuum: claudere
         * et scribere; aliter NIHIL scribitur (quadrum vacuum) */
        si (visus.mensura > (i32)(magnitudo(QUADRUM_INITIUM) - I))
        {
            chorda_aedificator_appendere_literis(opus->aed,
                QUADRUM_FINIS);
            visus = chorda_aedificator_spectare(opus->aed);
            successus = opus->pons->scribere(opus->pons->datum,
                visus.datum, (i32)visus.mensura);
            opus->fructus.octeti_emissi += visus.mensura;
        }
    }
    opus->primum = FALSUM;
    opus->fructus.praesentationes++;
    opus->fructus.tempus_praesentandi_ms +=
        (f64)(clock() - t0) * 1000.0 / (f64)CLOCKS_PER_SEC;
    redde successus;
}

b32
tessera_magnitudinem_renovare (
    TesseraOpus* opus)
{
    i32 latitudo;
    i32 altitudo;

    si (   opus == NIHIL
        || !opus->pons->amplitudo(opus->pons->datum, &latitudo,
               &altitudo))
    {
        redde FALSUM;
    }
    opus->latitudo = (latitudo > ZEPHYRUM)
        ? ((latitudo <= TESSERA_LATITUDO_MAXIMA)
            ? latitudo : TESSERA_LATITUDO_MAXIMA)
        : I;
    opus->altitudo = (altitudo > ZEPHYRUM)
        ? ((altitudo <= TESSERA_ALTITUDO_MAXIMA)
            ? altitudo : TESSERA_ALTITUDO_MAXIMA)
        : I;
    opus->primum = VERUM;  /* pictura plena sequitur */
    redde VERUM;
}
