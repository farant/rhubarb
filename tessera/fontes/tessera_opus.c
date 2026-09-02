/* tessera_opus.c - Implementatio operis (Phase A) */

#include "tessera_opus.h"
#include "utf8.h"
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
    chorda_aedificator_appendere_literis(opus->aed,
        "\033[0m\033[?25h");
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
            cella->ornamenta       = stilus.ornamenta;
        }
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
    TesseraCellula* cella;

    si (opus == NIHIL || !_in_finibus(opus, x, y))
    {
        redde;  /* praecisio taciturna */
    }
    cella                  = &opus->tergum[_index(x, y)];
    cella->signum          = signum;
    cella->color_litterae  = stilus.color_litterae;
    cella->color_fundi     = stilus.color_fundi;
    cella->ornamenta       = stilus.ornamenta;
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
        i8 primus = *cursor;

        si (primus < 0x20 || primus == 0x7F)
        {
            /* octetus regiminis -> '?' */
            tessera_cellulam_ponere(opus, cx, y, '?', stilus);
            cursor++;
        }
        alioquin
        {
            constans i8* post = cursor;
                    s32  runa = utf8_decodere(&post, finis);

            si (runa < ZEPHYRUM)
            {
                /* series invalida -> '?' (octetus unus) */
                tessera_cellulam_ponere(opus, cx, y, '?', stilus);
                cursor++;
            }
            alioquin
            {
                i32 n = (i32)(post - cursor);

                tessera_cellulam_ponere(opus, cx, y,
                    tessera_signum_ex_octetis(cursor, n), stilus);
                cursor = post;
            }
        }
        cx++;
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
            si (opus->primum)
            {
                pingenda = !_cellulae_aequales(cella, &CELLULA_VACUA);
                opus->frons[idx] = *cella;
            }
            alioquin
            {
                pingenda = !_cellulae_aequales(cella,
                    &opus->frons[idx]);
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
                stilus_cellae.ornamenta       = cella->ornamenta;
                si (   !stilus_validus
                    || !tessera_stilus_aequalis(stilus_currens,
                           stilus_cellae))
                {
                    _stilum_emittere(opus->aed, &stilus_cellae);
                    stilus_currens = stilus_cellae;
                    stilus_validus = VERUM;
                }
            }
            tessera_signum_scribere(opus->aed, cella->signum);
            opus->frons[idx] = *cella;
            opus->fructus.cellulae_mutatae++;
            mutatae_quadri++;

            pos_x = x + I;
            pos_y = y;
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

        si (visus.mensura > ZEPHYRUM)
        {
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
