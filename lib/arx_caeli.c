#include "arx_caeli.h"
#include "thema.h"
#include "color.h"
#include "delineare.h"
#include "tempus.h"
#include "xar.h"
#include "entitas.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* ==================================================
 * Constantae Internae
 * ================================================== */

#define CARTA_LATITUDO_MINIMUS  X       /* 10 characters minimum */
#define CARTA_ALTITUDO_MINIMUS  III     /* 3 lines minimum */
#define CARTA_PADDING           I       /* 1 character padding */
#define CARTA_INSET_VISUAL      IV      /* 4 pixels visual inset from logical bounds */
#define CARTA_OFFSET_VERTICALIS (-IV)   /* -4 pixels: shift visual card up for better text centering */
#define CARTA_TRIM_INFERIOR     VIII    /* 8 pixels: trim bottom whitespace (1 char height) */
#define CARTA_RADIUS_ANGULI     III     /* 3 pixels corner radius */
#define DEBOUNCE_TEMPUS         2.0     /* 2 seconds */
#define NIHIL_SELECTA           0xFFFFFFFF  /* Sentinel pro "nihil selectum" */


/* ==================================================
 * Forward Declarations
 * ================================================== */

hic_manens i32 _obtinere_max_z_index(ArcCaeli* arc);
hic_manens vacuum _ordinare_per_z_index_asc(ArcCaeli* arc, i32* indices);
hic_manens vacuum _ordinare_per_z_index_desc(ArcCaeli* arc, i32* indices);


/* ==================================================
 * Functiones Auxiliares
 * ================================================== */

/* Calculare dimensiones cartae ex textu */
hic_manens vacuum
_calculare_dimensiones(
    Carta* carta)
{
    i32 linea_currens_lat;
    i32 max_lat;
    i32 num_lineae;
    i32 i;

    max_lat = ZEPHYRUM;
    num_lineae = I;
    linea_currens_lat = ZEPHYRUM;

    per (i = ZEPHYRUM; i < (i32)carta->textus.mensura; i++)
    {
        si (carta->textus.datum[i] == '\n')
        {
            si (linea_currens_lat > max_lat)
            {
                max_lat = linea_currens_lat;
            }
            linea_currens_lat = ZEPHYRUM;
            num_lineae++;
        }
        alioquin
        {
            linea_currens_lat++;
        }
    }

    /* Ultima linea */
    si (linea_currens_lat > max_lat)
    {
        max_lat = linea_currens_lat;
    }

    /* Addere padding et border */
    carta->latitudo = max_lat + (CARTA_PADDING * II) + II;  /* +2 pro border */
    carta->altitudo = num_lineae + (CARTA_PADDING * II) + II;

    /* Minima */
    si (carta->latitudo < CARTA_LATITUDO_MINIMUS)
    {
        carta->latitudo = CARTA_LATITUDO_MINIMUS;
    }
    si (carta->altitudo < CARTA_ALTITUDO_MINIMUS)
    {
        carta->altitudo = CARTA_ALTITUDO_MINIMUS;
    }
}

/* Extrahere titulum folderis ex textu (sine > et spatia initiales)
 * "> My Projects" -> "My Projects"
 * Redde: chorda cum titulo (sectio in textus), vel chorda vacua si error
 */
hic_manens chorda
_extrahere_titulum_folderis(
    chorda textus)
{
    i32 start;
    i32 end;
    chorda titulus;

    titulus.datum = NIHIL;
    titulus.mensura = ZEPHYRUM;

    si (textus.mensura == ZEPHYRUM || textus.datum == NIHIL)
    {
        redde titulus;
    }

    /* Saltare '>' et spatia initiales */
    start = ZEPHYRUM;
    si (textus.datum[ZEPHYRUM] == '>')
    {
        start = I;
    }
    dum (start < (i32)textus.mensura && textus.datum[start] == ' ')
    {
        start++;
    }

    /* Invenire finem (ante newline) */
    end = start;
    dum (end < (i32)textus.mensura && textus.datum[end] != '\n')
    {
        end++;
    }

    /* Creare sectio (view in textus, non copia) */
    si (end > start)
    {
        titulus = chorda_sectio(textus, start, end);
    }

    redde titulus;
}

/* Convertere (linea, columna) ad index in buffer */
hic_manens i32
_cursor_ad_index(
    ArcCaeli* arc)
{
    i32 linea_currens;
    i32 columna_currens;
    i32 i;

    linea_currens = ZEPHYRUM;
    columna_currens = ZEPHYRUM;

    per (i = ZEPHYRUM; i < arc->edit_longitudo; i++)
    {
        si (linea_currens == arc->cursor_linea &&
            columna_currens == arc->cursor_columna)
        {
            redde i;
        }

        si (arc->edit_buffer[i] == '\n')
        {
            linea_currens++;
            columna_currens = ZEPHYRUM;
        }
        alioquin
        {
            columna_currens++;
        }
    }

    /* Cursor ad finem */
    redde arc->edit_longitudo;
}

/* Obtinere numerum linearum et latitudinem maximam */
hic_manens vacuum
_obtinere_dimensiones_textus(
    i8*  textus,
    i32  longitudo,
    i32* num_lineae,
    i32* max_latitudo)
{
    i32 linea_lat;
    i32 i;

    *num_lineae = I;
    *max_latitudo = ZEPHYRUM;
    linea_lat = ZEPHYRUM;

    per (i = ZEPHYRUM; i < longitudo; i++)
    {
        si (textus[i] == '\n')
        {
            si (linea_lat > *max_latitudo)
            {
                *max_latitudo = linea_lat;
            }
            linea_lat = ZEPHYRUM;
            (*num_lineae)++;
        }
        alioquin
        {
            linea_lat++;
        }
    }

    si (linea_lat > *max_latitudo)
    {
        *max_latitudo = linea_lat;
    }
}

/* Obtinere latitudinem lineae specificae */
hic_manens i32
_latitudo_lineae(
    ArcCaeli* arc,
    i32       linea)
{
    i32 linea_currens;
    i32 latitudo;
    i32 i;

    linea_currens = ZEPHYRUM;
    latitudo = ZEPHYRUM;

    per (i = ZEPHYRUM; i < arc->edit_longitudo; i++)
    {
        si (linea_currens == linea)
        {
            si (arc->edit_buffer[i] == '\n')
            {
                redde latitudo;
            }
            latitudo++;
        }
        alioquin si (arc->edit_buffer[i] == '\n')
        {
            linea_currens++;
        }
    }

    /* Ultima linea */
    si (linea_currens == linea)
    {
        redde latitudo;
    }

    redde ZEPHYRUM;
}

/* Intrare modus inserere */
hic_manens vacuum
_intrare_inserere(
    ArcCaeli* arc,
    i32       carta_index)
{
    Carta* carta;
    i32 i;
    i32 num_lineae;
    i32 ultima_linea_lat;

    si (carta_index >= arc->numerus_cartarum)
    {
        redde;
    }

    carta = &arc->cartae[carta_index];

    /* Copiare textum ad edit buffer */
    arc->edit_longitudo = ZEPHYRUM;
    per (i = ZEPHYRUM; i < (i32)carta->textus.mensura && i < DXII - I; i++)
    {
        arc->edit_buffer[i] = (character)carta->textus.datum[i];
        arc->edit_longitudo++;
    }

    arc->index_selecta = carta_index;
    arc->modus = ARC_MODUS_INSERERE;

    /* Ponere cursor ad finem textus */
    num_lineae = ZEPHYRUM;
    ultima_linea_lat = ZEPHYRUM;
    per (i = ZEPHYRUM; i < arc->edit_longitudo; i++)
    {
        si (arc->edit_buffer[i] == '\n')
        {
            num_lineae++;
            ultima_linea_lat = ZEPHYRUM;
        }
        alioquin
        {
            ultima_linea_lat++;
        }
    }
    arc->cursor_linea = num_lineae;
    arc->cursor_columna = ultima_linea_lat;
}

/* Exire modus inserere et salvare */
hic_manens vacuum
_exire_inserere(
    ArcCaeli* arc)
{
    Carta* carta;
    chorda* nova_chorda;

    si (arc->index_selecta >= arc->numerus_cartarum)
    {
        arc->modus = ARC_MODUS_NORMALIS;
        redde;
    }

    carta = &arc->cartae[arc->index_selecta];

    /* Null-terminate pro internare */
    arc->edit_buffer[arc->edit_longitudo] = '\0';

    /* Internare novum textum */
    nova_chorda = chorda_internare_ex_literis(arc->ctx->intern, arc->edit_buffer);
    si (nova_chorda)
    {
        carta->textus = *nova_chorda;
    }

    /* Recalculare dimensiones */
    _calculare_dimensiones(carta);

    /* Detegere si folder (pro UI feedback - relatio creatur in salvare) */
    carta->est_folder = (carta->textus.datum && carta->textus.mensura > ZEPHYRUM &&
                         carta->textus.datum[ZEPHYRUM] == '>');

    arc->modus = ARC_MODUS_SELECTA;
    arx_caeli_marcare_immundum(arc);
}

/* Hit test: invenire cartam ad positionem (topmost = highest z_index)
 *
 * x, y: positio in pixelis (relativum ad widget)
 */
hic_manens i32
_invenire_carta_ad_punctum(
    ArcCaeli* arc,
    i32       x,
    i32       y)
{
    i32 sorted_indices[CARTAE_MAXIMUS];
    i32 i;
    i32 char_lat;
    i32 char_alt;

    si (!arc)
    {
        redde NIHIL_SELECTA;
    }

    char_lat = VI * arc->scala;
    char_alt = VIII * arc->scala;

    /* Sortare per z_index descendens (high z first) */
    _ordinare_per_z_index_desc(arc, sorted_indices);

    /* Iterare ab alto ad bassum - primus hit est topmost */
    per (i = ZEPHYRUM; i < arc->numerus_cartarum; i++)
    {
        i32 idx;
        Carta* carta;
        i32 carta_px_lat;
        i32 carta_px_alt;

        idx = sorted_indices[i];
        carta = &arc->cartae[idx];

        /* Convertere dimensiones ad pixela */
        carta_px_lat = carta->latitudo * char_lat;
        carta_px_alt = carta->altitudo * char_alt;

        si (x >= carta->x && x < carta->x + carta_px_lat &&
            y >= carta->y && y < carta->y + carta_px_alt)
        {
            redde idx;
        }
    }

    redde NIHIL_SELECTA;
}

/* ==================================================
 * Link Detection
 * ================================================== */

/* Verificare si character est pars nominis link */
hic_manens b32
_est_character_link(
    character c)
{
    redde (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           (c >= '0' && c <= '9') ||
           (c == '_') || (c == '-');
}

/* Detectare link in textu ad positionem
 *
 * textus: datum textus (i8*)
 * longitudo: longitudo textus
 * positio: positio characteris in textu (0-indexed)
 * link_initium: output - initium link (inclusivum, ad #)
 * link_finis: output - finis link (exclusivum)
 *
 * Redde: VERUM si link inventus ad positionem
 */
hic_manens b32
_detecta_linkum(
    constans i8* textus,
    i32 longitudo,
    i32 positio,
    i32* link_initium,
    i32* link_finis)
{
    s32 col;
    character c;
    s32 hash_pos;

    si (!textus || positio < ZEPHYRUM || positio >= longitudo)
    {
        redde FALSUM;
    }

    /* Scandere retro ad invenire # */
    hash_pos = -I;
    per (col = (s32)positio; col >= ZEPHYRUM; col--)
    {
        c = (character)textus[col];

        si (c == '#')
        {
            hash_pos = col;
            frange;
        }

        /* Stop at whitespace, newline, or non-link char */
        si (c == ' ' || c == '\t' || c == '\n' || c == '\0')
        {
            redde FALSUM;
        }

        si (!_est_character_link(c))
        {
            redde FALSUM;
        }
    }

    si (hash_pos < ZEPHYRUM)
    {
        redde FALSUM;
    }

    /* Scandere ad finem link (post #) */
    *link_initium = (i32)hash_pos;
    *link_finis = (i32)hash_pos + I;

    per (col = (s32)hash_pos + I; col < (s32)longitudo; col++)
    {
        c = (character)textus[col];

        si (!_est_character_link(c))
        {
            frange;
        }

        *link_finis = (i32)col + I;
    }

    /* Verificare si link habet nomen (non solum #) */
    si (*link_finis <= *link_initium + I)
    {
        redde FALSUM;
    }

    redde VERUM;
}

/* Verificare si positio est intra aliquem link in textu
 *
 * Redde: VERUM si positio est pars link
 */
hic_manens b32
_est_in_link(
    constans i8* textus,
    i32 longitudo,
    i32 positio)
{
    i32 link_initium;
    i32 link_finis;

    si (_detecta_linkum(textus, longitudo, positio, &link_initium, &link_finis))
    {
        redde (positio >= link_initium && positio < link_finis);
    }

    redde FALSUM;
}

/* Convertere positionem click (in pixelis) ad index characteris in textu cartae
 *
 * arc: controller
 * carta: carta
 * click_px_x, click_px_y: positio click in pixelis (absoluta)
 *
 * Redde: index characteris si inventum, -1 si extra textum
 */
hic_manens s32
_pixel_ad_char_index(
    ArcCaeli* arc,
    Carta*    carta,
    i32       click_px_x,
    i32       click_px_y)
{
    i32 char_lat;
    i32 char_alt;
    i32 carta_px_x;
    i32 carta_px_y;
    i32 inset;
    i32 vis_x;
    i32 vis_y;
    i32 text_px_x;
    i32 text_px_y;
    i32 click_col;
    i32 click_linea;
    i32 textus_longitudo;
    constans i8* textus_datum;
    i32 linea_currens;
    i32 col_currens;
    i32 i;

    si (!arc || !carta)
    {
        redde -I;
    }

    char_lat = VI * arc->scala;
    char_alt = VIII * arc->scala;
    inset = CARTA_INSET_VISUAL * arc->scala;

    /* Calculare positio visualis cartae in pixelis
     * NOTA: carta->x/y jam sunt widget-relativum pixela */
    carta_px_x = (arc->widget_x * char_lat) + carta->x;
    carta_px_y = (arc->widget_y * char_alt) + carta->y;
    vis_x = carta_px_x + inset;
    vis_y = (i32)((s32)carta_px_y + (s32)inset + (CARTA_OFFSET_VERTICALIS * (s32)arc->scala));

    /* Positio textus (cum padding) */
    text_px_x = vis_x + CARTA_PADDING * char_lat;
    text_px_y = vis_y + CARTA_PADDING * char_alt;

    /* Convertere click ad columna et linea relativum ad textum */
    click_col = (click_px_x - text_px_x) / char_lat;
    click_linea = (click_px_y - text_px_y) / char_alt;

    si (click_col < ZEPHYRUM || click_linea < ZEPHYRUM)
    {
        redde -I;
    }

    /* Iterare per textum ad invenire character index */
    textus_datum = carta->textus.datum;
    textus_longitudo = (i32)carta->textus.mensura;
    linea_currens = ZEPHYRUM;
    col_currens = ZEPHYRUM;

    per (i = ZEPHYRUM; i < textus_longitudo; i++)
    {
        si (linea_currens == click_linea && col_currens == click_col)
        {
            redde (s32)i;
        }

        si (textus_datum[i] == '\n')
        {
            /* Si click est post finem lineae sed in linea correcta */
            si (linea_currens == click_linea && col_currens < click_col)
            {
                redde -I;  /* Extra finem lineae */
            }
            linea_currens++;
            col_currens = ZEPHYRUM;
        }
        alioquin
        {
            col_currens++;
        }
    }

    /* Verificare ultimum character */
    si (linea_currens == click_linea && col_currens == click_col)
    {
        redde (s32)(textus_longitudo - I);
    }

    redde -I;
}


/* ==================================================
 * Z-Index Helpers
 * ================================================== */

/* Obtinere maximum z_index inter omnes cartas
 *
 * Redde: maximum z_index, vel 0 si nullae cartae
 */
hic_manens i32
_obtinere_max_z_index(
    ArcCaeli* arc)
{
    i32 max_z;
    i32 i;

    si (!arc)
    {
        redde ZEPHYRUM;
    }

    max_z = ZEPHYRUM;
    per (i = ZEPHYRUM; i < arc->numerus_cartarum; i++)
    {
        si (arc->cartae[i].z_index > max_z)
        {
            max_z = arc->cartae[i].z_index;
        }
    }

    redde max_z;
}

/* Ordinare indices per z_index (ascending - low z first, high z last)
 *
 * arc: controller
 * indices: array output de indices sortatis (debet habere CARTAE_MAXIMUS elementa)
 *
 * Pro rendering: low z draws first, high z draws last (on top)
 */
hic_manens vacuum
_ordinare_per_z_index_asc(
    ArcCaeli* arc,
    i32*      indices)
{
    i32 i;
    s32 j;  /* DEBET esse signatus pro j-- dum j >= 0 */
    i32 temp;

    si (!arc || !indices)
    {
        redde;
    }

    /* Initiare cum indice originali */
    per (i = ZEPHYRUM; i < arc->numerus_cartarum; i++)
    {
        indices[i] = i;
    }

    /* Insertion sort (simplex pro max 64 elementa) */
    per (i = I; i < arc->numerus_cartarum; i++)
    {
        temp = indices[i];
        j = (s32)i - I;

        dum (j >= ZEPHYRUM && arc->cartae[indices[j]].z_index > arc->cartae[temp].z_index)
        {
            indices[j + I] = indices[j];
            j--;
        }
        indices[j + I] = temp;
    }
}

/* Ordinare indices per z_index (descending - high z first, low z last)
 *
 * Pro hit testing: high z checked first (topmost card)
 */
hic_manens vacuum
_ordinare_per_z_index_desc(
    ArcCaeli* arc,
    i32*      indices)
{
    i32 i;
    s32 j;  /* DEBET esse signatus pro j-- dum j >= 0 */
    i32 temp;

    si (!arc || !indices)
    {
        redde;
    }

    /* Initiare cum indice originali */
    per (i = ZEPHYRUM; i < arc->numerus_cartarum; i++)
    {
        indices[i] = i;
    }

    /* Insertion sort descending */
    per (i = I; i < arc->numerus_cartarum; i++)
    {
        temp = indices[i];
        j = (s32)i - I;

        dum (j >= ZEPHYRUM && arc->cartae[indices[j]].z_index < arc->cartae[temp].z_index)
        {
            indices[j + I] = indices[j];
            j--;
        }
        indices[j + I] = temp;
    }
}


/* ==================================================
 * Creatio
 * ================================================== */

ArcCaeli*
arx_caeli_creare(
    ContextusWidget* ctx)
{
    ArcCaeli* arc;

    si (!ctx || !ctx->piscina || !ctx->intern)
    {
        redde NIHIL;
    }

    arc = piscina_allocare(ctx->piscina, magnitudo(ArcCaeli));
    si (!arc)
    {
        redde NIHIL;
    }

    arc->ctx = ctx;

    arc->schirma_currens_id.datum = NIHIL;
    arc->schirma_currens_id.mensura = ZEPHYRUM;
    arc->schirma_currens_slug.datum = NIHIL;
    arc->schirma_currens_slug.mensura = ZEPHYRUM;

    arc->numerus_cartarum = ZEPHYRUM;
    arc->historia_index = ZEPHYRUM;

    arc->modus = ARC_MODUS_NORMALIS;
    arc->index_selecta = NIHIL_SELECTA;
    arc->cursor_linea = ZEPHYRUM;
    arc->cursor_columna = ZEPHYRUM;

    arc->trahens = FALSUM;
    arc->trahere_validum = FALSUM;
    arc->trahere_hover_folder = NIHIL_SELECTA;

    arc->immundum = FALSUM;
    arc->tempus_immundum = 0.0;

    arc->widget_x = ZEPHYRUM;
    arc->widget_y = ZEPHYRUM;
    arc->widget_latitudo = ZEPHYRUM;
    arc->widget_altitudo = ZEPHYRUM;
    arc->scala = I;  /* Defectus scala = 1 */

    /* Initiare entitas_ids ad NIHIL */
    {
        i32 i;
        per (i = ZEPHYRUM; i < CARTAE_MAXIMUS; i++)
        {
            arc->entitas_ids[i] = NIHIL;
        }
    }

    redde arc;
}


/* ==================================================
 * Functiones Auxiliares Schirmae
 * ================================================== */

/* Invenire vel creare entitatem schirmae per slug
 *
 * arc: controller
 * slug: slug schirmae (C string, null-terminata)
 *
 * Redde: Entitas* si inventum vel creatum, NIHIL si error
 *
 * Haec functio:
 * 1. Quaerit entitates cum tag "#Schirma"
 * 2. Invenit entitatem cum proprietate "slug" aequali
 * 3. Si non invenit, creat novam entitatem Schirma
 */
hic_manens Entitas*
_invenire_vel_creare_schirmam(
    ArcCaeli*           arc,
    constans character* slug)
{
    Entitas* schirma_entitas;
    chorda* clavis_slug;
    chorda* slug_valor;

    si (!arc || !arc->ctx->repo || !slug)
    {
        redde NIHIL;
    }

    /* Usare scaffoldare - creat cum UUID deterministico ex "Schirma" + slug
     * Idempotens: reddit existentem si iam creata */
    schirma_entitas = arc->ctx->repo->entitas_scaffoldare(arc->ctx->repo->datum, "Schirma", slug);
    si (!schirma_entitas)
    {
        redde NIHIL;
    }

    /* Ponere proprietates (scaffoldare non ponit proprietates) */
    clavis_slug = chorda_internare_ex_literis(arc->ctx->intern, "slug");
    slug_valor = entitas_proprietas_capere(schirma_entitas, clavis_slug);

    si (!slug_valor)
    {
        /* Nova entitas - ponere proprietates */
        arc->ctx->repo->proprietas_ponere(arc->ctx->repo->datum, schirma_entitas, "slug", slug);
        arc->ctx->repo->proprietas_ponere(arc->ctx->repo->datum, schirma_entitas, "titulus", slug);

        /* Addere #Radix tag solum pro schirma radice */
        si (strcmp(slug, "root") == ZEPHYRUM)
        {
            arc->ctx->repo->nota_addere(arc->ctx->repo->datum, schirma_entitas, "#Radix");
        }
    }

    redde schirma_entitas;
}


/* ==================================================
 * Navigatio
 * ================================================== */

vacuum
arx_caeli_navigare_ad(
    ArcCaeli*           arc,
    constans character* slug)
{
    chorda* slug_interned;
    constans character* slug_effectivum;
    Entitas* schirma_entitas;

    si (!arc)
    {
        redde;
    }

    /* Salvare schirma currens si immundum */
    si (arc->immundum)
    {
        arx_caeli_salvare(arc);
    }

    /* Addere ad historiam si non iam in radice */
    si (arc->schirma_currens_slug.mensura > ZEPHYRUM &&
        arc->historia_index < HISTORIA_MAXIMUS)
    {
        arc->historia[arc->historia_index] = arc->schirma_currens_slug;
        arc->historia_index++;
    }

    /* Determinare slug effectivum */
    slug_effectivum = slug ? slug : "root";

    /* Ponere novum slug */
    slug_interned = chorda_internare_ex_literis(arc->ctx->intern, slug_effectivum);
    si (slug_interned)
    {
        arc->schirma_currens_slug = *slug_interned;
    }

    /* Invenire vel creare entitatem schirmae */
    si (arc->ctx->repo)
    {
        schirma_entitas = _invenire_vel_creare_schirmam(arc, slug_effectivum);
        si (schirma_entitas)
        {
            arc->schirma_currens_id = *schirma_entitas->id;
        }
        alioquin
        {
            arc->schirma_currens_id.datum = NIHIL;
            arc->schirma_currens_id.mensura = ZEPHYRUM;
        }
    }

    /* Reset status */
    arc->numerus_cartarum = ZEPHYRUM;
    arc->modus = ARC_MODUS_NORMALIS;
    arc->index_selecta = NIHIL_SELECTA;
    arc->trahens = FALSUM;

    /* Carcare cartas ex repositorio */
    arx_caeli_carcare(arc);
}

vacuum
arx_caeli_retro(
    ArcCaeli* arc)
{
    Entitas* schirma_entitas;
    character* slug_cstr;

    si (!arc)
    {
        redde;
    }

    si (arc->historia_index > ZEPHYRUM)
    {
        arc->historia_index--;

        /* Salvare si immundum */
        si (arc->immundum)
        {
            arx_caeli_salvare(arc);
        }

        arc->schirma_currens_slug = arc->historia[arc->historia_index];

        /* Invenire entitatem schirmae */
        si (arc->ctx->repo && arc->schirma_currens_slug.datum)
        {
            slug_cstr = chorda_ut_cstr(arc->schirma_currens_slug, arc->ctx->piscina);
            si (slug_cstr)
            {
                schirma_entitas = _invenire_vel_creare_schirmam(arc, slug_cstr);
                si (schirma_entitas)
                {
                    arc->schirma_currens_id = *schirma_entitas->id;
                }
            }
        }

        /* Reset status */
        arc->numerus_cartarum = ZEPHYRUM;
        arc->modus = ARC_MODUS_NORMALIS;
        arc->index_selecta = NIHIL_SELECTA;
        arc->trahens = FALSUM;

        arx_caeli_carcare(arc);
    }
}


/* ==================================================
 * Operationes Cartarum
 * ================================================== */

vacuum
arx_caeli_carta_creare(
    ArcCaeli* arc,
    i32       px_x,
    i32       px_y)
{
    Carta* carta;
    chorda* textus_interned;

    si (!arc)
    {
        redde;
    }

    si (arc->numerus_cartarum >= CARTAE_MAXIMUS)
    {
        redde;
    }

    carta = &arc->cartae[arc->numerus_cartarum];

    carta->id.datum = NIHIL;
    carta->id.mensura = ZEPHYRUM;

    /* Positio in pixelis (widget-relativum) */
    carta->x = px_x;
    carta->y = px_y;

    textus_interned = chorda_internare_ex_literis(arc->ctx->intern, "Nova carta");
    si (textus_interned)
    {
        carta->textus = *textus_interned;
    }
    alioquin
    {
        carta->textus.datum = NIHIL;
        carta->textus.mensura = ZEPHYRUM;
    }

    /* Nova carta non est folder (defectus textus non incipit cum >) */
    carta->est_folder = FALSUM;

    _calculare_dimensiones(carta);

    /* Nulla collisio - cartae possunt overlap */

    /* Nova carta est super omnes */
    carta->z_index = _obtinere_max_z_index(arc) + I;

    arc->numerus_cartarum++;
    arc->index_selecta = arc->numerus_cartarum - I;
    arc->modus = ARC_MODUS_SELECTA;

    arx_caeli_marcare_immundum(arc);
}

vacuum
arx_caeli_carta_delere(
    ArcCaeli* arc,
    i32       index)
{
    i32 i;

    si (!arc)
    {
        redde;
    }

    si (index >= arc->numerus_cartarum)
    {
        redde;
    }

    /* Delere entitas ex repositorio si existit */
    si (arc->entitas_ids[index] != NIHIL && arc->ctx->repo != NIHIL)
    {
        arc->ctx->repo->entitas_delere(arc->ctx->repo->datum, arc->entitas_ids[index]);
    }

    /* Movere cartas et entitas_ids post index */
    per (i = index; i < arc->numerus_cartarum - I; i++)
    {
        arc->cartae[i] = arc->cartae[i + I];
        arc->entitas_ids[i] = arc->entitas_ids[i + I];
    }

    /* Mundare ultimum slot */
    arc->entitas_ids[arc->numerus_cartarum - I] = NIHIL;

    arc->numerus_cartarum--;
    arc->index_selecta = NIHIL_SELECTA;
    arc->modus = ARC_MODUS_NORMALIS;

    arx_caeli_marcare_immundum(arc);
}

vacuum
arx_caeli_carta_movere(
    ArcCaeli* arc,
    i32       index,
    i32       px_x,
    i32       px_y)
{
    Carta* carta;
    i32 char_lat;
    i32 char_alt;
    i32 widget_px_lat;
    i32 widget_px_alt;
    i32 carta_px_lat;
    i32 carta_px_alt;

    si (!arc)
    {
        redde;
    }

    si (index >= arc->numerus_cartarum)
    {
        redde;
    }

    carta = &arc->cartae[index];

    /* Dimensiones in pixelis */
    char_lat = VI * arc->scala;
    char_alt = VIII * arc->scala;
    widget_px_lat = arc->widget_latitudo * char_lat;
    widget_px_alt = arc->widget_altitudo * char_alt;
    carta_px_lat = carta->latitudo * char_lat;
    carta_px_alt = carta->altitudo * char_alt;

    /* Verificare limites widget (pixela) */
    si (px_x < ZEPHYRUM || px_y < ZEPHYRUM ||
        px_x + carta_px_lat > widget_px_lat ||
        px_y + carta_px_alt > widget_px_alt)
    {
        /* Extra limites - non movere */
        redde;
    }

    /* Nulla collisio - cartae possunt overlap */

    /* Actualizare positio (pixela) */
    carta->x = px_x;
    carta->y = px_y;

    /* Ponere z_index ad summum (carta nunc est super omnes) */
    carta->z_index = _obtinere_max_z_index(arc) + I;

    arx_caeli_marcare_immundum(arc);
}

b32
arx_caeli_verificare_collisio(
    ArcCaeli* arc,
    i32       x,
    i32       y,
    i32       latitudo,
    i32       altitudo,
    i32       ignorare_index)
{
    i32 i;

    si (!arc)
    {
        redde FALSUM;
    }

    per (i = ZEPHYRUM; i < arc->numerus_cartarum; i++)
    {
        Carta* carta;
        b32 overlap_x;
        b32 overlap_y;

        si (i == ignorare_index)
        {
            perge;
        }

        carta = &arc->cartae[i];

        /* AABB intersection test */
        overlap_x = (x < carta->x + carta->latitudo) && (x + latitudo > carta->x);
        overlap_y = (y < carta->y + carta->altitudo) && (y + altitudo > carta->y);

        si (overlap_x && overlap_y)
        {
            redde VERUM;
        }
    }

    redde FALSUM;
}

/* Invenire folder sub puncto (x, y), ignorando cartam specificam
 *
 * Redde: index folderis si inventum (topmost), NIHIL_SELECTA si non
 */
hic_manens i32
_invenire_folder_sub_puncto(
    ArcCaeli* arc,
    i32       px_x,
    i32       px_y,
    i32       ignorare_index)
{
    i32 sorted_indices[CARTAE_MAXIMUS];
    i32 char_lat;
    i32 char_alt;
    i32 i;

    si (!arc)
    {
        redde NIHIL_SELECTA;
    }

    /* Dimensiones characteris in pixelis */
    char_lat = VI * arc->scala;
    char_alt = VIII * arc->scala;

    /* Sortare per z_index descendens (high z first) */
    _ordinare_per_z_index_desc(arc, sorted_indices);

    per (i = ZEPHYRUM; i < arc->numerus_cartarum; i++)
    {
        i32 idx;
        Carta* carta;
        i32 carta_px_lat;
        i32 carta_px_alt;

        idx = sorted_indices[i];

        si (idx == ignorare_index)
        {
            perge;
        }

        carta = &arc->cartae[idx];

        /* Solum folders */
        si (!carta->est_folder)
        {
            perge;
        }

        /* Convertere latitudo/altitudo ad pixela */
        carta_px_lat = carta->latitudo * char_lat;
        carta_px_alt = carta->altitudo * char_alt;

        /* Punctum in carta? (omnia in pixelis) */
        si (px_x >= carta->x && px_x < carta->x + carta_px_lat &&
            px_y >= carta->y && px_y < carta->y + carta_px_alt)
        {
            redde idx;
        }
    }

    redde NIHIL_SELECTA;
}

/* Movere cartam ad aliam schirmam (folder)
 *
 * arc: controller
 * index: index cartae movendae
 * nova_schirma_slug: slug novae schirmae (chorda)
 */
hic_manens vacuum
_movere_carta_ad_schirmam(
    ArcCaeli* arc,
    i32       index,
    chorda    nova_schirma_slug)
{
    chorda* entitas_id;
    Entitas* entitas;
    Entitas* nova_schirma;
    character* slug_novus_cstr;
    chorda* genus_continet;
    Xar* relationes_ad;
    Relatio* nova_relatio;
    i32 i;

    si (!arc || !arc->ctx->repo || index < ZEPHYRUM || index >= arc->numerus_cartarum)
    {
        redde;
    }

    si (arc->entitas_ids[index] == NIHIL)
    {
        /* Carta non salvata adhuc - salvare primo */
        arx_caeli_salvare(arc);

        /* Verificare iterum */
        si (arc->entitas_ids[index] == NIHIL)
        {
            redde;
        }
    }

    entitas_id = arc->entitas_ids[index];
    entitas = arc->ctx->repo->capere_entitatem(arc->ctx->repo->datum, entitas_id);

    si (entitas == NIHIL)
    {
        redde;
    }

    slug_novus_cstr = chorda_ut_cstr(nova_schirma_slug, arc->ctx->piscina);
    si (!slug_novus_cstr)
    {
        redde;
    }

    /* Invenire vel creare novam schirmam */
    nova_schirma = _invenire_vel_creare_schirmam(arc, slug_novus_cstr);
    si (!nova_schirma)
    {
        redde;
    }

    /* Invenire et delere veterem "continet" relationem per back-reference */
    genus_continet = chorda_internare_ex_literis(arc->ctx->intern, "continet");
    relationes_ad = arc->ctx->repo->capere_relationes_ad(arc->ctx->repo->datum, entitas_id);

    si (relationes_ad)
    {
        per (i = ZEPHYRUM; i < xar_numerus(relationes_ad); i++)
        {
            Relatio** rel_ptr;
            Relatio* rel;

            rel_ptr = (Relatio**)xar_obtinere(relationes_ad, i);
            si (!rel_ptr || !*rel_ptr)
            {
                perge;
            }
            rel = *rel_ptr;

            /* Verificare si est "continet" relatio */
            si (rel->genus && chorda_aequalis(*rel->genus, *genus_continet))
            {
                arc->ctx->repo->relatio_delere(arc->ctx->repo->datum, rel->id);
                frange;
            }
        }
    }

    /* Creare novam "continet" relationem ex nova schirma */
    nova_relatio = arc->ctx->repo->relatio_addere(
        arc->ctx->repo->datum,
        nova_schirma,
        "continet",
        entitas_id);

    /* Si relatio non creata, non removere cartam */
    si (!nova_relatio)
    {
        redde;
    }

    /* Removere cartam ex array currenti (sine delere entitas) */
    per (i = index; i < arc->numerus_cartarum - I; i++)
    {
        arc->cartae[i] = arc->cartae[i + I];
        arc->entitas_ids[i] = arc->entitas_ids[i + I];
    }
    arc->entitas_ids[arc->numerus_cartarum - I] = NIHIL;
    arc->numerus_cartarum--;

    arc->index_selecta = NIHIL_SELECTA;
    arc->modus = ARC_MODUS_NORMALIS;
}


/* ==================================================
 * Widget Interface - Rendering
 * ================================================== */

hic_manens vacuum
_reddere_carta(
    ArcCaeli*            arc,
    ContextusDelineandi* ctx,
    Carta*               carta,
    i32                  carta_index,
    i32                  widget_x,
    i32                  widget_y,
    i32                  scala,
    b32                  selecta,
    b32                  in_inserere)
{
    i32 char_lat;
    i32 char_alt;
    i32 px_x;
    i32 px_y;
    i32 px_lat;
    i32 px_alt;
    i32 vis_x;
    i32 vis_y;
    i32 vis_lat;
    i32 vis_alt;
    i32 inset;
    i32 radius;
    Color color_border;
    Color color_background;
    Color color_textus;
    chorda linea;
    i32 linea_y;
    i32 i;
    i32 linea_start;
    i8* textus_datum;
    i32 textus_longitudo;

    char_lat = VI * scala;
    char_alt = VIII * scala;

    /* Si in inserere, usare edit buffer et recalculare dimensiones */
    si (in_inserere)
    {
        i32 num_lin;
        i32 max_lat;

        textus_datum = (i8*)arc->edit_buffer;
        textus_longitudo = arc->edit_longitudo;

        _obtinere_dimensiones_textus(textus_datum, textus_longitudo, &num_lin, &max_lat);

        px_lat = (max_lat + (CARTA_PADDING * II) + II) * char_lat;
        px_alt = (num_lin + (CARTA_PADDING * II) + II) * char_alt;

        /* Minima */
        si (px_lat < CARTA_LATITUDO_MINIMUS * char_lat)
        {
            px_lat = CARTA_LATITUDO_MINIMUS * char_lat;
        }
        si (px_alt < CARTA_ALTITUDO_MINIMUS * char_alt)
        {
            px_alt = CARTA_ALTITUDO_MINIMUS * char_alt;
        }
    }
    alioquin
    {
        textus_datum = carta->textus.datum;
        textus_longitudo = (i32)carta->textus.mensura;
        px_lat = carta->latitudo * char_lat;
        px_alt = carta->altitudo * char_alt;
    }

    /* Carta.x/y jam sunt in pixelis (relativum ad widget) */
    px_x = (widget_x * char_lat) + carta->x;
    px_y = (widget_y * char_alt) + carta->y;

    /* Calculare limites visuales (inset a limitibus logicis) */
    inset = CARTA_INSET_VISUAL * scala;
    radius = CARTA_RADIUS_ANGULI * scala;
    vis_x = px_x + inset;
    vis_y = (i32)((s32)px_y + (s32)inset + (CARTA_OFFSET_VERTICALIS * (s32)scala));  /* Shift up */
    vis_lat = px_lat - (inset * II);
    vis_alt = px_alt - (inset * II) - (CARTA_TRIM_INFERIOR * scala);  /* Trim bottom */

    /* Assecurare dimensiones minimae pro visual */
    si (vis_lat < radius * II + II) vis_lat = radius * II + II;
    si (vis_alt < radius * II + II) vis_alt = radius * II + II;

    color_textus = thema_color(COLOR_TEXT);

    /* Folders habent fondum obscuriorem */
    si (carta->est_folder)
    {
        /* Verificare si folder est hover target */
        si (arc->trahens && carta_index == arc->trahere_hover_folder)
        {
            color_background = color_ex_palette(PALETTE_BRIGHT_LEAF);
            color_border = color_ex_palette(PALETTE_DARK_LEAF);
        }
        alioquin
        {
            color_background = color_ex_palette(PALETTE_LIGHT_GRAY);
            color_border = thema_color(COLOR_STATUS_VISUAL);
        }
    }
    alioquin
    {
        color_background = color_ex_palette(PALETTE_WHITE);
        color_border = thema_color(COLOR_BORDER);
    }

    /* Override border pro selecta/inserere */
    si (in_inserere)
    {
        color_border = thema_color(COLOR_STATUS_INSERT);
    }
    alioquin si (selecta)
    {
        color_border = color_ex_palette(PALETTE_BLUE);
    }

    /* Pingere fondum cum angulis rotundis */
    delineare_rectangulum_rotundum_plenum(ctx, vis_x, vis_y, vis_lat, vis_alt, radius, color_background);

    /* Pingere border cum angulis rotundis */
    delineare_rectangulum_rotundum(ctx, vis_x, vis_y, vis_lat, vis_alt, radius, color_border);

    /* Pingere textum per characterem (pro link highlighting) */
    {
        i32 col;
        Color color_link;

        color_link = color_ex_palette(PALETTE_DARK_LEAF);
        linea_y = ZEPHYRUM;
        col = ZEPHYRUM;

        per (i = ZEPHYRUM; i < textus_longitudo; i++)
        {
            si (textus_datum[i] == '\n')
            {
                linea_y++;
                col = ZEPHYRUM;
            }
            alioquin
            {
                Color char_color;
                chorda char_chorda;

                /* Verificare si character est pars link */
                si (_est_in_link(textus_datum, textus_longitudo, i))
                {
                    char_color = color_link;
                }
                alioquin
                {
                    char_color = color_textus;
                }

                /* Pingere singulum characterem */
                char_chorda.datum = textus_datum + i;
                char_chorda.mensura = I;

                tabula_pixelorum_pingere_chordam(
                    ctx->tabula,
                    vis_x + (CARTA_PADDING + col) * char_lat,
                    vis_y + (CARTA_PADDING + linea_y) * char_alt,
                    char_chorda,
                    color_ad_pixelum(char_color));

                col++;
            }
        }
    }

    (vacuum)linea;
    (vacuum)linea_start;

    /* Pingere cursor si in inserere */
    si (in_inserere)
    {
        i32 cursor_px_x;
        i32 cursor_px_y;
        i32 j;
        f32 blink;

        cursor_px_x = vis_x + (CARTA_PADDING + arc->cursor_columna) * char_lat;
        cursor_px_y = vis_y + (CARTA_PADDING + arc->cursor_linea) * char_alt;

        /* Blink cursor - unda quadrata cum frequentia 2Hz */
        blink = unda_quadratus((f32)tempus_nunc(), 2.0f, 1.0f);

        /* Pingere solum si blink > 0 */
        si (blink > 0.0f)
        {
            /* Pingere lineam verticalem pro cursore - usare cyan obscurum */
            per (j = ZEPHYRUM; j < char_alt; j++)
            {
                tabula_pixelorum_ponere_pixelum(
                    ctx->tabula,
                    cursor_px_x,
                    cursor_px_y + j,
                    color_ad_pixelum(color_ex_palette(PALETTE_DARK_LEAF)));
            }
        }
    }
}

vacuum
arx_caeli_reddere(
    ArcCaeli*        arc,
    TabulaPixelorum* tabula,
    i32              x,
    i32              y,
    i32              latitudo,
    i32              altitudo,
    i32              scala,
    b32              focused)
{
    ContextusDelineandi* ctx;
    i32 char_lat;
    i32 char_alt;
    i32 px_x;
    i32 px_y;
    i32 px_lat;
    i32 px_alt;
    i32 i;
    Color color_background;
    Color color_border;

    si (!arc || !tabula)
    {
        redde;
    }

    /* Salvare widget positio et dimensiones pro tractare_eventum */
    arc->widget_x = x;
    arc->widget_y = y;
    arc->widget_latitudo = latitudo;
    arc->widget_altitudo = altitudo;
    arc->scala = scala;

    ctx = delineare_creare_contextum(arc->ctx->piscina, tabula);
    si (!ctx)
    {
        redde;
    }

    char_lat = VI * scala;
    char_alt = VIII * scala;

    px_x = x * char_lat;
    px_y = y * char_alt;
    px_lat = latitudo * char_lat;
    px_alt = altitudo * char_alt;

    color_background = thema_color(COLOR_BACKGROUND);
    color_border = focused ? thema_color(COLOR_BORDER) : thema_color(COLOR_TEXT_DIM);

    /* Pingere fondum */
    delineare_rectangulum_plenum(ctx, px_x, px_y, px_lat, px_alt, color_background);

    /* Pingere border */
    delineare_rectangulum(ctx, px_x, px_y, px_lat, px_alt, color_border);

    /* Reddere titulum schirmae si non in radice */
    si (arc->schirma_currens_slug.datum != NIHIL && arc->schirma_currens_slug.mensura > ZEPHYRUM)
    {
        chorda titulus;
        i32 titulus_x;
        Entitas* schirma_entitas;
        chorda* titulus_proprietas;
        chorda* clavis_titulus;

        /* Capere titulum ex entitate schirmae */
        titulus = arc->schirma_currens_slug;  /* Defectus: slug */

        si (arc->ctx->repo && arc->schirma_currens_id.datum != NIHIL)
        {
            schirma_entitas = arc->ctx->repo->capere_entitatem(
                arc->ctx->repo->datum, &arc->schirma_currens_id);

            si (schirma_entitas)
            {
                clavis_titulus = chorda_internare_ex_literis(arc->ctx->intern, "titulus");
                si (clavis_titulus)
                {
                    titulus_proprietas = entitas_proprietas_capere(schirma_entitas, clavis_titulus);
                    si (titulus_proprietas && titulus_proprietas->datum)
                    {
                        titulus = *titulus_proprietas;
                    }
                }
            }
        }

        /* Centrare titulum in parte superiore */
        titulus_x = px_x + (px_lat - (i32)titulus.mensura * char_lat) / II;

        tabula_pixelorum_pingere_chordam(
            tabula,
            titulus_x,
            px_y + char_alt,
            titulus,
            color_ad_pixelum(thema_color(COLOR_TEXT)));
    }

    /* Reddere < Retro si non in radice */
    si (arc->historia_index > ZEPHYRUM)
    {
        chorda retro_label;

        retro_label = chorda_ex_literis("< Retro", arc->ctx->piscina);

        tabula_pixelorum_pingere_chordam(
            tabula,
            px_x + char_lat,
            px_y + char_alt,
            retro_label,
            color_ad_pixelum(thema_color(COLOR_ACCENT_PRIMARY)));
    }

    /* Reddere cartas in ordine z_index (low first, high last = on top) */
    {
        i32 sorted_indices[CARTAE_MAXIMUS];

        _ordinare_per_z_index_asc(arc, sorted_indices);

        per (i = ZEPHYRUM; i < arc->numerus_cartarum; i++)
        {
            i32 idx;
            b32 est_selecta;
            b32 in_inserere;

            idx = sorted_indices[i];
            est_selecta = (idx == arc->index_selecta);
            in_inserere = (est_selecta && arc->modus == ARC_MODUS_INSERERE);

            _reddere_carta(arc, ctx, &arc->cartae[idx], idx, x, y, scala, est_selecta, in_inserere);
        }
    }

    /* Reddere drag ghost si trahens */
    si (arc->trahens && arc->index_selecta != NIHIL_SELECTA)
    {
        Carta* carta;
        i32 ghost_px_x;
        i32 ghost_px_y;
        i32 ghost_px_lat;
        i32 ghost_px_alt;
        i32 ghost_vis_x;
        i32 ghost_vis_y;
        i32 ghost_vis_lat;
        i32 ghost_vis_alt;
        i32 ghost_inset;
        i32 ghost_radius;
        Color ghost_color;
        Color ghost_background;
        chorda linea;
        i32 linea_y;
        i32 linea_start;
        i32 j;

        carta = &arc->cartae[arc->index_selecta];

        /* Usare pixel position directe pro smooth dragging */
        ghost_px_x = arc->trahere_px_x - arc->trahere_offset_px_x;
        ghost_px_y = arc->trahere_px_y - arc->trahere_offset_px_y;
        ghost_px_lat = carta->latitudo * char_lat;
        ghost_px_alt = carta->altitudo * char_alt;

        /* Calculare limites visuales ghost */
        ghost_inset = CARTA_INSET_VISUAL * scala;
        ghost_radius = CARTA_RADIUS_ANGULI * scala;
        ghost_vis_x = ghost_px_x + ghost_inset;
        ghost_vis_y = (i32)((s32)ghost_px_y + (s32)ghost_inset + (CARTA_OFFSET_VERTICALIS * (s32)scala));
        ghost_vis_lat = ghost_px_lat - (ghost_inset * II);
        ghost_vis_alt = ghost_px_alt - (ghost_inset * II) - (CARTA_TRIM_INFERIOR * scala);

        /* Assecurare dimensiones minimae */
        si (ghost_vis_lat < ghost_radius * II + II) ghost_vis_lat = ghost_radius * II + II;
        si (ghost_vis_alt < ghost_radius * II + II) ghost_vis_alt = ghost_radius * II + II;

        si (arc->trahere_validum)
        {
            ghost_color = thema_color(COLOR_STATUS_INSERT);
        }
        alioquin
        {
            ghost_color = thema_color(COLOR_ERROR);
        }

        /* Pingere fondum ghost cum angulis rotundis */
        ghost_background = color_ex_palette(PALETTE_WHITE);
        delineare_rectangulum_rotundum_plenum(ctx, ghost_vis_x, ghost_vis_y,
                                              ghost_vis_lat, ghost_vis_alt,
                                              ghost_radius, ghost_background);

        /* Pingere border ghost cum angulis rotundis */
        delineare_rectangulum_rotundum(ctx, ghost_vis_x, ghost_vis_y,
                                       ghost_vis_lat, ghost_vis_alt,
                                       ghost_radius, ghost_color);

        /* Pingere textum ghost (relativum ad limites visuales) */
        linea_y = ZEPHYRUM;
        linea_start = ZEPHYRUM;

        per (j = ZEPHYRUM; j <= (i32)carta->textus.mensura; j++)
        {
            si (j == (i32)carta->textus.mensura || carta->textus.datum[j] == '\n')
            {
                linea.datum = carta->textus.datum + linea_start;
                linea.mensura = j - linea_start;

                tabula_pixelorum_pingere_chordam(
                    ctx->tabula,
                    ghost_vis_x + CARTA_PADDING * char_lat,
                    ghost_vis_y + (CARTA_PADDING + linea_y) * char_alt,
                    linea,
                    color_ad_pixelum(thema_color(COLOR_TEXT)));

                linea_start = j + I;
                linea_y++;
            }
        }
    }

    /* Salvare si immundum */
    arx_caeli_salvare_si_immundum(arc);
}


/* ==================================================
 * Widget Interface - Eventus
 * ================================================== */

b32
arx_caeli_tractare_eventum(
    ArcCaeli*         arc,
    constans Eventus* eventus)
{
    si (!arc || !eventus)
    {
        redde FALSUM;
    }

    /* Tractare eventus muris - duplex click */
    si (eventus->genus == EVENTUS_MUS_DUPLEX)
    {
        i32 char_lat;
        i32 char_alt;
        i32 px_x;
        i32 px_y;
        i32 carta_index;

        /* Dimensiones characterum cum scala */
        char_lat = VI * arc->scala;
        char_alt = VIII * arc->scala;

        /* Convertere ad pixel coordinates relative ad widget */
        px_x = eventus->datum.mus.x - (arc->widget_x * char_lat);
        px_y = eventus->datum.mus.y - (arc->widget_y * char_alt);

        /* Si in modus inserere, exire primo (salvat mutationes) */
        si (arc->modus == ARC_MODUS_INSERERE)
        {
            _exire_inserere(arc);
        }

        /* Terminare drag si activum */
        arc->trahens = FALSUM;

        /* Hit test cartas (pixela) */
        carta_index = _invenire_carta_ad_punctum(arc, px_x, px_y);

        si (carta_index != NIHIL_SELECTA)
        {
            Carta* carta = &arc->cartae[carta_index];

            /* Si folder, navigare; alioquin intrare inserere */
            si (carta->est_folder)
            {
                /* Traversare portal_ad relationem ad destinationem */
                Entitas* carta_entitas;
                chorda* genus_portal;
                Xar* portal_rels;

                carta_entitas = arc->ctx->repo->capere_entitatem(
                    arc->ctx->repo->datum, arc->entitas_ids[carta_index]);

                si (carta_entitas)
                {
                    genus_portal = chorda_internare_ex_literis(arc->ctx->intern, "portal_ad");
                    portal_rels = entitas_relationes_generis_capere(
                        carta_entitas, genus_portal, arc->ctx->piscina);

                    si (portal_rels && xar_numerus(portal_rels) > ZEPHYRUM)
                    {
                        Relatio* rel;
                        Entitas* dest_schirma;
                        chorda* clavis_slug;
                        chorda* dest_slug;

                        /* entitas_relationes_generis_capere reddit Xar de Relatio (non Relatio*) */
                        rel = (Relatio*)xar_obtinere(portal_rels, ZEPHYRUM);
                        si (rel)
                        {
                            dest_schirma = arc->ctx->repo->capere_entitatem(
                                arc->ctx->repo->datum, rel->destinatio_id);

                            si (dest_schirma)
                            {
                                clavis_slug = chorda_internare_ex_literis(arc->ctx->intern, "slug");
                                dest_slug = entitas_proprietas_capere(dest_schirma, clavis_slug);

                                si (dest_slug && dest_slug->datum)
                                {
                                    character* slug_cstr;
                                    slug_cstr = chorda_ut_cstr(*dest_slug, arc->ctx->piscina);
                                    si (slug_cstr)
                                    {
                                        arx_caeli_navigare_ad(arc, slug_cstr);
                                    }
                                }
                            }
                        }
                    }
                }
                redde VERUM;
            }
            alioquin
            {
                /* Verificare si click est super link */
                s32 char_index;

                char_index = _pixel_ad_char_index(arc, carta,
                                                   eventus->datum.mus.x,
                                                   eventus->datum.mus.y);

                si (char_index >= ZEPHYRUM && arc->link_callback)
                {
                    i32 link_initium;
                    i32 link_finis;

                    si (_detecta_linkum(carta->textus.datum,
                                        (i32)carta->textus.mensura,
                                        (i32)char_index,
                                        &link_initium,
                                        &link_finis))
                    {
                        /* Extrahere nomen link (sine #) */
                        character link_buffer[LXIV];
                        i32 link_len;
                        i32 k;

                        link_len = link_finis - link_initium - I;  /* -1 pro # */
                        si (link_len > LXIII)
                        {
                            link_len = LXIII;
                        }

                        per (k = ZEPHYRUM; k < link_len; k++)
                        {
                            link_buffer[k] = (character)carta->textus.datum[link_initium + I + k];
                        }
                        link_buffer[link_len] = '\0';

                        /* Vocare callback */
                        arc->link_callback(arc->link_callback_datum, link_buffer);
                        redde VERUM;
                    }
                }

                /* Non in link - intrare modus inserere */
                _intrare_inserere(arc, carta_index);
            }
        }
        alioquin
        {
            /* Duplex click in vacuum - creare novam cartam (pixela) */
            arx_caeli_carta_creare(arc, px_x, px_y);
        }

        redde VERUM;
    }

    /* Tractare eventus muris - singulum click */
    si (eventus->genus == EVENTUS_MUS_DEPRESSUS)
    {
        i32 char_lat;
        i32 char_alt;
        i32 px_x;
        i32 px_y;
        i32 carta_index;

        /* Dimensiones characterum cum scala */
        char_lat = VI * arc->scala;
        char_alt = VIII * arc->scala;

        /* Convertere ad pixel coordinates relative ad widget */
        px_x = eventus->datum.mus.x - (arc->widget_x * char_lat);
        px_y = eventus->datum.mus.y - (arc->widget_y * char_alt);

        /* Si in modus inserere, exire primo (salvat mutationes) */
        si (arc->modus == ARC_MODUS_INSERERE)
        {
            _exire_inserere(arc);
        }

        /* Verificare < Retro click (primi 10 characteres, primae 2 lineae) */
        si (arc->historia_index > ZEPHYRUM &&
            px_y < (II * char_alt) && px_x < (X * char_lat))
        {
            arx_caeli_retro(arc);
            redde VERUM;
        }

        /* Hit test cartas (pixela) */
        carta_index = _invenire_carta_ad_punctum(arc, px_x, px_y);

        si (carta_index != NIHIL_SELECTA)
        {
            Carta* carta = &arc->cartae[carta_index];

            /* Verificare si click est super link (prioritas ante drag) */
            si (!carta->est_folder && arc->link_callback)
            {
                s32 char_index;

                char_index = _pixel_ad_char_index(arc, carta,
                                                   eventus->datum.mus.x,
                                                   eventus->datum.mus.y);

                si (char_index >= ZEPHYRUM)
                {
                    i32 link_initium;
                    i32 link_finis;

                    si (_detecta_linkum(carta->textus.datum,
                                        (i32)carta->textus.mensura,
                                        (i32)char_index,
                                        &link_initium,
                                        &link_finis))
                    {
                        /* Extrahere nomen link (sine #) */
                        character link_buffer[LXIV];
                        i32 link_len;
                        i32 k;

                        link_len = link_finis - link_initium - I;
                        si (link_len > LXIII)
                        {
                            link_len = LXIII;
                        }

                        per (k = ZEPHYRUM; k < link_len; k++)
                        {
                            link_buffer[k] = (character)carta->textus.datum[link_initium + I + k];
                        }
                        link_buffer[link_len] = '\0';

                        arc->link_callback(arc->link_callback_datum, link_buffer);
                        redde VERUM;
                    }
                }
            }

            /* Selectare et initiare drag */
            {
                i32 carta_screen_px_x;
                i32 carta_screen_px_y;

                arc->index_selecta = carta_index;
                arc->modus = ARC_MODUS_SELECTA;
                arc->trahens = VERUM;
                arc->trahere_origin_x = carta->x;
                arc->trahere_origin_y = carta->y;

                /* Calculare offset in pixelis
                 * carta->x/y jam sunt widget-relativum pixela
                 * Convertere ad screen pixels pro offset */
                carta_screen_px_x = (arc->widget_x * char_lat) + carta->x;
                carta_screen_px_y = (arc->widget_y * char_alt) + carta->y;
                arc->trahere_offset_px_x = eventus->datum.mus.x - carta_screen_px_x;
                arc->trahere_offset_px_y = eventus->datum.mus.y - carta_screen_px_y;

                /* Initiare trahere_px pro ghost rendering */
                arc->trahere_px_x = eventus->datum.mus.x;
                arc->trahere_px_y = eventus->datum.mus.y;

                /* trahere_grid_x/y nunc tenent widget-relativum pixela */
                arc->trahere_grid_x = carta->x;
                arc->trahere_grid_y = carta->y;
                arc->trahere_validum = VERUM;
            }
        }
        alioquin
        {
            /* Click in vacuum - deselect */
            arc->index_selecta = NIHIL_SELECTA;
            arc->modus = ARC_MODUS_NORMALIS;
        }

        redde VERUM;
    }

    si (eventus->genus == EVENTUS_MUS_MOTUS)
    {
        si (arc->trahens && arc->index_selecta != NIHIL_SELECTA)
        {
            Carta* carta;
            s32 ghost_px_x;
            s32 ghost_px_y;
            s32 widget_px_x;
            s32 widget_px_y;
            s32 widget_px_lat;
            s32 widget_px_alt;
            s32 char_lat;
            s32 char_alt;

            carta = &arc->cartae[arc->index_selecta];

            /* Dimensiones characterum cum scala */
            char_lat = (s32)(VI * arc->scala);
            char_alt = (s32)(VIII * arc->scala);

            /* Calculare positio ghost in pixelis (relativa ad widget) */
            widget_px_x = (s32)arc->widget_x * char_lat;
            widget_px_y = (s32)arc->widget_y * char_alt;
            ghost_px_x = (s32)eventus->datum.mus.x - (s32)arc->trahere_offset_px_x - widget_px_x;
            ghost_px_y = (s32)eventus->datum.mus.y - (s32)arc->trahere_offset_px_y - widget_px_y;

            /* Cohibere ghost intra limites widget (in pixelis)
             * NOTA: Cohibere dextrum/fundum primo, deinde sinistrum/summum,
             * sic sinistrum/summum vincit si carta maior quam widget */
            widget_px_lat = (s32)arc->widget_latitudo * char_lat;
            widget_px_alt = (s32)arc->widget_altitudo * char_alt;

            {
                s32 carta_px_lat = (s32)carta->latitudo * char_lat;
                s32 carta_px_alt = (s32)carta->altitudo * char_alt;

                si (ghost_px_x + carta_px_lat > widget_px_lat)
                {
                    ghost_px_x = widget_px_lat - carta_px_lat;
                }
                si (ghost_px_y + carta_px_alt > widget_px_alt)
                {
                    ghost_px_y = widget_px_alt - carta_px_alt;
                }
            }
            si (ghost_px_x < 0)
            {
                ghost_px_x = 0;
            }
            si (ghost_px_y < 0)
            {
                ghost_px_y = 0;
            }

            /* Memorare positio ghost in pixelis (widget-relativum)
             * NOTA: trahere_grid_x/y nunc tenent pixela, non grid chars */
            arc->trahere_grid_x = (i32)ghost_px_x;
            arc->trahere_grid_y = (i32)ghost_px_y;

            /* Recalculare trahere_px ex positione cohibita (screen pixels) */
            arc->trahere_px_x = (i32)(ghost_px_x + (s32)arc->trahere_offset_px_x + widget_px_x);
            arc->trahere_px_y = (i32)(ghost_px_y + (s32)arc->trahere_offset_px_y + widget_px_y);

            /* Nulla collisio - cartae possunt overlap */
            arc->trahere_validum = VERUM;

            /* Detectare folder hover (pixela) */
            arc->trahere_hover_folder = _invenire_folder_sub_puncto(
                arc,
                arc->trahere_grid_x,
                arc->trahere_grid_y,
                arc->index_selecta);

            redde VERUM;
        }
    }

    si (eventus->genus == EVENTUS_MUS_LIBERATUS)
    {
        si (arc->trahens && arc->index_selecta != NIHIL_SELECTA)
        {
            i32 folder_index;

            /* Invenire si drop est super folder (pixela) */
            folder_index = _invenire_folder_sub_puncto(
                arc,
                arc->trahere_grid_x,
                arc->trahere_grid_y,
                arc->index_selecta);

            si (folder_index != NIHIL_SELECTA)
            {
                /* Movere cartam ad folder - traversare portal_ad */
                Entitas* folder_entitas;
                chorda* genus_portal;
                Xar* portal_rels;

                folder_entitas = arc->ctx->repo->capere_entitatem(
                    arc->ctx->repo->datum, arc->entitas_ids[folder_index]);

                si (folder_entitas)
                {
                    genus_portal = chorda_internare_ex_literis(arc->ctx->intern, "portal_ad");
                    portal_rels = entitas_relationes_generis_capere(
                        folder_entitas, genus_portal, arc->ctx->piscina);

                    si (portal_rels && xar_numerus(portal_rels) > ZEPHYRUM)
                    {
                        Relatio* rel;
                        Entitas* dest_schirma;
                        chorda* clavis_slug;
                        chorda* dest_slug;

                        /* entitas_relationes_generis_capere reddit Xar de Relatio (non Relatio*) */
                        rel = (Relatio*)xar_obtinere(portal_rels, ZEPHYRUM);
                        si (rel)
                        {
                            dest_schirma = arc->ctx->repo->capere_entitatem(
                                arc->ctx->repo->datum, rel->destinatio_id);

                            si (dest_schirma)
                            {
                                clavis_slug = chorda_internare_ex_literis(arc->ctx->intern, "slug");
                                dest_slug = entitas_proprietas_capere(dest_schirma, clavis_slug);

                                si (dest_slug && dest_slug->datum)
                                {
                                    _movere_carta_ad_schirmam(
                                        arc,
                                        arc->index_selecta,
                                        *dest_slug);
                                }
                            }
                        }
                    }
                }
            }
            alioquin si (arc->trahere_validum)
            {
                /* Committere move ad novam positionem (pixela) */
                arx_caeli_carta_movere(
                    arc,
                    arc->index_selecta,
                    arc->trahere_grid_x,
                    arc->trahere_grid_y);
            }
            /* Si extra limites, carta manet in loco originali */

            arc->trahens = FALSUM;
            arc->trahere_hover_folder = NIHIL_SELECTA;
            redde VERUM;
        }
    }

    /* Tractare keyboard */
    si (eventus->genus == EVENTUS_CLAVIS_DEPRESSUS)
    {
        si (arc->modus == ARC_MODUS_SELECTA)
        {
            /* x = delere */
            si (eventus->datum.clavis.clavis == 'X' ||
                eventus->datum.clavis.clavis == 'x')
            {
                si (arc->index_selecta != NIHIL_SELECTA)
                {
                    arx_caeli_carta_delere(arc, arc->index_selecta);
                    redde VERUM;
                }
            }
        }
        alioquin si (arc->modus == ARC_MODUS_INSERERE)
        {
            i32 clavis;
            i32 index;
            i32 linea_lat;
            i32 num_lineae;
            i32 i;

            clavis = eventus->datum.clavis.clavis;

            /* ESC = exire inserere */
            si (clavis == CLAVIS_EFFUGIUM)
            {
                _exire_inserere(arc);
                redde VERUM;
            }

            /* Enter = inserere newline */
            si (clavis == CLAVIS_REDITUS)
            {
                si (arc->edit_longitudo < DXII - I)
                {
                    index = _cursor_ad_index(arc);

                    /* Movere characteres post cursor */
                    per (i = arc->edit_longitudo; i > index; i--)
                    {
                        arc->edit_buffer[i] = arc->edit_buffer[i - I];
                    }
                    arc->edit_buffer[index] = '\n';
                    arc->edit_longitudo++;

                    arc->cursor_linea++;
                    arc->cursor_columna = ZEPHYRUM;
                }
                redde VERUM;
            }

            /* Backspace = delere characterem ante cursor */
            si (clavis == CLAVIS_RETRORSUM)
            {
                index = _cursor_ad_index(arc);
                si (index > ZEPHYRUM)
                {
                    /* Si delendo newline, cursor ad finem lineae prioris */
                    si (arc->edit_buffer[index - I] == '\n')
                    {
                        arc->cursor_linea--;
                        arc->cursor_columna = _latitudo_lineae(arc, arc->cursor_linea);
                        /* Corrigere quia linea nondum mutata */
                        linea_lat = ZEPHYRUM;
                        per (i = ZEPHYRUM; i < index - I; i++)
                        {
                            si (arc->edit_buffer[i] == '\n')
                            {
                                linea_lat = ZEPHYRUM;
                            }
                            alioquin
                            {
                                linea_lat++;
                            }
                        }
                        arc->cursor_columna = linea_lat;
                    }
                    alioquin
                    {
                        arc->cursor_columna--;
                    }

                    /* Movere characteres */
                    per (i = index - I; i < arc->edit_longitudo - I; i++)
                    {
                        arc->edit_buffer[i] = arc->edit_buffer[i + I];
                    }
                    arc->edit_longitudo--;
                }
                redde VERUM;
            }

            /* Arrow keys */
            si (clavis == CLAVIS_SINISTER)
            {
                si (arc->cursor_columna > ZEPHYRUM)
                {
                    arc->cursor_columna--;
                }
                alioquin si (arc->cursor_linea > ZEPHYRUM)
                {
                    arc->cursor_linea--;
                    arc->cursor_columna = _latitudo_lineae(arc, arc->cursor_linea);
                }
                redde VERUM;
            }

            si (clavis == CLAVIS_DEXTER)
            {
                linea_lat = _latitudo_lineae(arc, arc->cursor_linea);
                si (arc->cursor_columna < linea_lat)
                {
                    arc->cursor_columna++;
                }
                alioquin
                {
                    /* Numerare lineas */
                    _obtinere_dimensiones_textus((i8*)arc->edit_buffer, arc->edit_longitudo,
                                                  &num_lineae, &linea_lat);
                    si (arc->cursor_linea < num_lineae - I)
                    {
                        arc->cursor_linea++;
                        arc->cursor_columna = ZEPHYRUM;
                    }
                }
                redde VERUM;
            }

            si (clavis == CLAVIS_SURSUM)
            {
                si (arc->cursor_linea > ZEPHYRUM)
                {
                    arc->cursor_linea--;
                    linea_lat = _latitudo_lineae(arc, arc->cursor_linea);
                    si (arc->cursor_columna > linea_lat)
                    {
                        arc->cursor_columna = linea_lat;
                    }
                }
                redde VERUM;
            }

            si (clavis == CLAVIS_DEORSUM)
            {
                _obtinere_dimensiones_textus((i8*)arc->edit_buffer, arc->edit_longitudo,
                                              &num_lineae, &linea_lat);
                si (arc->cursor_linea < num_lineae - I)
                {
                    arc->cursor_linea++;
                    linea_lat = _latitudo_lineae(arc, arc->cursor_linea);
                    si (arc->cursor_columna > linea_lat)
                    {
                        arc->cursor_columna = linea_lat;
                    }
                }
                redde VERUM;
            }

            /* Printable characters - usare typus (respectat shift/caps) */
            si (eventus->datum.clavis.typus >= XXXII &&
                eventus->datum.clavis.typus < CXXVII)
            {
                si (arc->edit_longitudo < DXII - I)
                {
                    index = _cursor_ad_index(arc);

                    /* Movere characteres post cursor */
                    per (i = arc->edit_longitudo; i > index; i--)
                    {
                        arc->edit_buffer[i] = arc->edit_buffer[i - I];
                    }
                    arc->edit_buffer[index] = eventus->datum.clavis.typus;
                    arc->edit_longitudo++;

                    arc->cursor_columna++;
                }
                redde VERUM;
            }
        }
    }

    redde FALSUM;
}


/* ==================================================
 * Persistentia
 * ================================================== */

vacuum
arx_caeli_carcare(
    ArcCaeli* arc)
{
    Xar* entitates;
    i32 i;
    constans character* slug_str;
    Entitas* schirma_entitas;
    chorda* genus_continet;
    chorda* genus_portal;
    chorda* clavis_textus;
    chorda* clavis_x;
    chorda* clavis_y;
    chorda* clavis_z;

    si (!arc || !arc->ctx->repo)
    {
        redde;
    }

    /* Reset cartas et entitas_ids */
    arc->numerus_cartarum = ZEPHYRUM;
    per (i = ZEPHYRUM; i < CARTAE_MAXIMUS; i++)
    {
        arc->entitas_ids[i] = NIHIL;
    }

    /* Obtinere slug ut C string (DEBET esse null-terminatum!) */
    si (arc->schirma_currens_slug.datum)
    {
        slug_str = chorda_ut_cstr(arc->schirma_currens_slug, arc->ctx->piscina);
    }
    alioquin
    {
        slug_str = "root";
    }

    /* Invenire schirma entitas */
    schirma_entitas = _invenire_vel_creare_schirmam(arc, slug_str);
    si (!schirma_entitas)
    {
        redde;
    }

    /* Capere cartas per relatio "continet" */
    genus_continet = chorda_internare_ex_literis(arc->ctx->intern, "continet");
    entitates = arc->ctx->repo->capere_entitates_relatae(
        arc->ctx->repo->datum, schirma_entitas, genus_continet);

    si (!entitates)
    {
        redde;
    }

    /* Internare claves semel */
    clavis_textus = chorda_internare_ex_literis(arc->ctx->intern, "textus");
    clavis_x = chorda_internare_ex_literis(arc->ctx->intern, "x");
    clavis_y = chorda_internare_ex_literis(arc->ctx->intern, "y");
    clavis_z = chorda_internare_ex_literis(arc->ctx->intern, "z_index");
    genus_portal = chorda_internare_ex_literis(arc->ctx->intern, "portal_ad");

    /* Iterare per entitates */
    per (i = ZEPHYRUM; i < xar_numerus(entitates) && arc->numerus_cartarum < CARTAE_MAXIMUS; i++)
    {
        Entitas** entitas_slot;
        Entitas* entitas;
        Carta* carta;
        chorda* textus_valor;
        chorda* x_valor;
        chorda* y_valor;
        chorda* z_valor;
        Xar* portal_relationes;

        entitas_slot = (Entitas**)xar_obtinere(entitates, i);
        si (!entitas_slot || !*entitas_slot)
        {
            perge;
        }
        entitas = *entitas_slot;

        /* Creare carta in array */
        carta = &arc->cartae[arc->numerus_cartarum];

        /* Memorare entity ID */
        arc->entitas_ids[arc->numerus_cartarum] = entitas->id;

        /* Capere proprietates */
        textus_valor = entitas_proprietas_capere(entitas, clavis_textus);
        x_valor = entitas_proprietas_capere(entitas, clavis_x);
        y_valor = entitas_proprietas_capere(entitas, clavis_y);
        z_valor = entitas_proprietas_capere(entitas, clavis_z);

        si (textus_valor)
        {
            carta->textus = *textus_valor;
        }
        alioquin
        {
            chorda* defectus = chorda_internare_ex_literis(arc->ctx->intern, "");
            si (defectus)
            {
                carta->textus = *defectus;
            }
            alioquin
            {
                carta->textus.datum = NIHIL;
                carta->textus.mensura = ZEPHYRUM;
            }
        }

        si (x_valor && x_valor->datum)
        {
            carta->x = (i32)atoi((constans character*)x_valor->datum);
        }
        alioquin
        {
            carta->x = ZEPHYRUM;
        }

        si (y_valor && y_valor->datum)
        {
            carta->y = (i32)atoi((constans character*)y_valor->datum);
        }
        alioquin
        {
            carta->y = ZEPHYRUM;
        }

        si (z_valor && z_valor->datum)
        {
            carta->z_index = (i32)atoi((constans character*)z_valor->datum);
        }
        alioquin
        {
            carta->z_index = ZEPHYRUM;
        }

        /* Detegere folder per portal_ad relatio */
        portal_relationes = entitas_relationes_generis_capere(entitas, genus_portal, arc->ctx->piscina);
        carta->est_folder = (portal_relationes && xar_numerus(portal_relationes) > ZEPHYRUM);

        /* Calculare dimensiones */
        _calculare_dimensiones(carta);

        arc->numerus_cartarum++;
    }
}

vacuum
arx_caeli_salvare(
    ArcCaeli* arc)
{
    i32 i;
    character x_buffer[XVI];
    character y_buffer[XVI];
    character z_buffer[XVI];
    constans character* slug_str;
    Entitas* schirma_entitas;

    si (!arc || !arc->ctx->repo)
    {
        redde;
    }

    /* Obtinere slug ut C string (DEBET esse null-terminatum!) */
    si (arc->schirma_currens_slug.datum)
    {
        slug_str = chorda_ut_cstr(arc->schirma_currens_slug, arc->ctx->piscina);
    }
    alioquin
    {
        slug_str = "root";
    }

    /* Obtinere schirma entitas pro relationibus */
    schirma_entitas = _invenire_vel_creare_schirmam(arc, slug_str);

    /* Salvare omnes cartas */
    per (i = ZEPHYRUM; i < arc->numerus_cartarum; i++)
    {
        Carta* carta;
        Entitas* entitas;

        carta = &arc->cartae[i];

        /* Si iam habemus entity ID, usare capere_entitatem */
        si (arc->entitas_ids[i] != NIHIL)
        {
            entitas = arc->ctx->repo->capere_entitatem(arc->ctx->repo->datum, arc->entitas_ids[i]);
        }
        alioquin
        {
            /* Prima vice: creare novam entitas */
            entitas = arc->ctx->repo->entitas_creare(arc->ctx->repo->datum, "Carta");
            si (entitas != NIHIL)
            {
                /* Memorare entity ID */
                arc->entitas_ids[i] = entitas->id;

                /* Creare relatio: Schirma --[continet]--> Carta */
                si (schirma_entitas)
                {
                    arc->ctx->repo->relatio_addere(
                        arc->ctx->repo->datum,
                        schirma_entitas,
                        "continet",
                        entitas->id);
                }
            }
        }

        si (!entitas)
        {
            perge;
        }

        /* Ponere proprietates */
        /* Copiare textus ad buffer null-terminatum */
        {
            character textus_buffer[DXII];
            i32 len;

            len = (i32)carta->textus.mensura;
            si (len > DXII - I)
            {
                len = DXII - I;
            }

            si (carta->textus.datum && len > ZEPHYRUM)
            {
                memcpy(textus_buffer, carta->textus.datum, (size_t)len);
            }
            textus_buffer[len] = '\0';

            arc->ctx->repo->proprietas_ponere(
                arc->ctx->repo->datum,
                entitas,
                "textus",
                textus_buffer);
        }

        /* Convertere x, y, z_index ad chordas */
        sprintf(x_buffer, "%d", carta->x);
        sprintf(y_buffer, "%d", carta->y);
        sprintf(z_buffer, "%d", carta->z_index);

        arc->ctx->repo->proprietas_ponere(
            arc->ctx->repo->datum,
            entitas,
            "x",
            x_buffer);

        arc->ctx->repo->proprietas_ponere(
            arc->ctx->repo->datum,
            entitas,
            "y",
            y_buffer);

        arc->ctx->repo->proprietas_ponere(
            arc->ctx->repo->datum,
            entitas,
            "z_index",
            z_buffer);

        /* Creare portal_ad relatio si carta est folder (incipit cum >) */
        si (carta->textus.datum && carta->textus.mensura > ZEPHYRUM &&
            carta->textus.datum[ZEPHYRUM] == '>')
        {
            chorda titulus;
            chorda slug;
            character* dest_slug_cstr;
            Entitas* dest_schirma;
            chorda* genus_portal;
            Xar* extantes;

            /* Extrahere titulum et creare slug */
            titulus = _extrahere_titulum_folderis(carta->textus);
            si (titulus.mensura > ZEPHYRUM)
            {
                slug = chorda_kebab(titulus, arc->ctx->piscina);
                si (slug.mensura > ZEPHYRUM)
                {
                    dest_slug_cstr = chorda_ut_cstr(slug, arc->ctx->piscina);
                    si (dest_slug_cstr)
                    {
                        /* Invenire vel creare destinationem schirmae */
                        dest_schirma = _invenire_vel_creare_schirmam(arc, dest_slug_cstr);
                        si (dest_schirma)
                        {
                            /* Verificare si relatio iam existit */
                            genus_portal = chorda_internare_ex_literis(arc->ctx->intern, "portal_ad");
                            extantes = entitas_relationes_generis_capere(entitas, genus_portal, arc->ctx->piscina);

                            si (!extantes || xar_numerus(extantes) == ZEPHYRUM)
                            {
                                /* Creare nova relatio: Carta --[portal_ad]--> Schirma */
                                arc->ctx->repo->relatio_addere(
                                    arc->ctx->repo->datum,
                                    entitas,
                                    "portal_ad",
                                    dest_schirma->id);
                            }
                            /* TODO: si destinatio mutavit, delere veterem et creare novam */
                        }
                    }
                }
            }
        }
    }

    arc->immundum = FALSUM;
}

vacuum
arx_caeli_salvare_si_immundum(
    ArcCaeli* arc)
{
    f64 nunc;
    f64 delta;

    si (!arc || !arc->immundum)
    {
        redde;
    }

    nunc = tempus_nunc();
    delta = nunc - arc->tempus_immundum;

    si (delta >= DEBOUNCE_TEMPUS)
    {
        arx_caeli_salvare(arc);
    }
}

vacuum
arx_caeli_marcare_immundum(
    ArcCaeli* arc)
{
    si (!arc)
    {
        redde;
    }

    arc->immundum = VERUM;
    arc->tempus_immundum = tempus_nunc();
}


/* ==================================================
 * Link Callback
 * ================================================== */

vacuum
arx_caeli_ponere_link_callback(
    ArcCaeli*           arc,
    FunctioLinkCallback callback,
    vacuum*             datum)
{
    si (!arc)
    {
        redde;
    }

    arc->link_callback = callback;
    arc->link_callback_datum = datum;
}
