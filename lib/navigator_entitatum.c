#include "navigator_entitatum.h"
#include "thema.h"
#include "xar.h"
#include <string.h>

/* ==================================================
 * Functiones Interiores
 * ================================================== */

/* Calcular altitudinem necessariam pro texto
 * "Calculate height needed for text wrapping"
 */
interior i32
_calcular_altitudinem_textus(
    i32 mensura,
    i32 latitudo_columnae)
{
    i32 lineae;

    si (mensura == ZEPHYRUM || latitudo_columnae == ZEPHYRUM)
    {
        redde I;
    }

    /* Calcular quot lineae necessariae pro mensura data */
    lineae = mensura / latitudo_columnae;
    si (mensura % latitudo_columnae != ZEPHYRUM)
    {
        lineae++;
    }

    redde lineae;
}

/* Sortare relationes alphabetice per genus
 * "Sort relationships alphabetically by genus"
 */
interior vacuum
_sortare_relationes(
    ItemNavigatoris* items,
    i32              numerus_relationum)
{
    i32 i;
    i32 j;
    i32 k;
    ItemNavigatoris temp;
    Relatio* rel_i;
    Relatio* rel_j;
    b32 debet_movere;

    /* Insertion sort */
    per (i = I; i < numerus_relationum; i++)
    {
        temp = items[i];
        rel_i = (Relatio*)temp.datum;

        /* Invenire positio pro insertione */
        k = i;
        per (j = ZEPHYRUM; j < i; j++)
        {
            rel_j = (Relatio*)items[j].datum;

            debet_movere = FALSUM;

            /* Comparare genera */
            si (rel_i && rel_i->genus && rel_j && rel_j->genus)
            {
                si (chorda_comparare(*rel_i->genus, *rel_j->genus) < ZEPHYRUM)
                {
                    debet_movere = VERUM;
                }
            }

            si (debet_movere)
            {
                k = j;
                frange;
            }
        }

        /* Movere items ad dextram */
        si (k < i)
        {
            per (j = i; j > k; j--)
            {
                items[j] = items[j - I];
            }
            items[k] = temp;
        }
    }
}

/* Construere items array ex entitate currente
 * Addit omnes relationes, deinde omnes proprietates
 */
interior vacuum
_construere_items(
    NavigatorEntitatum* nav,
    i32                 latitudo_columnae)
{
    Entitas*     ent;
    Relatio*     rel;
    Proprietas*  prop;
    ItemNavigatoris* item;
    i32          i;
    i32          numerus;
    i32          longitudo_totalis;

    si (!nav || !nav->entitas_currens)
    {
        redde;
    }

    ent = nav->entitas_currens;
    nav->numerus_itemorum = ZEPHYRUM;

    /* Addere relationes */
    numerus = xar_numerus(ent->relationes);
    per (i = ZEPHYRUM; i < numerus && nav->numerus_itemorum < CXXVIII; i++)
    {
        rel = (Relatio*)xar_obtinere(ent->relationes, i);
        si (!rel)
        {
            perge;
        }

        item = &nav->items[nav->numerus_itemorum];
        item->genus = ITEM_RELATIO;
        item->altitudo = I;  /* Relationes sunt semper 1 linea */
        item->datum = rel;

        nav->numerus_itemorum++;
    }

    /* Sortare relationes alphabetice */
    si (nav->numerus_itemorum > I)
    {
        _sortare_relationes(nav->items, nav->numerus_itemorum);
    }

    /* Addere proprietates */
    numerus = xar_numerus(ent->proprietates);
    per (i = ZEPHYRUM; i < numerus && nav->numerus_itemorum < CXXVIII; i++)
    {
        prop = (Proprietas*)xar_obtinere(ent->proprietates, i);
        si (!prop)
        {
            perge;
        }

        item = &nav->items[nav->numerus_itemorum];
        item->genus = ITEM_PROPRIETAS;

        /* Calcular altitudo basatus in mensura valoris */
        /* longitudo totalis = clavis + ": " + valor */
        longitudo_totalis = ZEPHYRUM;
        si (prop->clavis && prop->clavis->datum)
        {
            longitudo_totalis += prop->clavis->mensura;
        }
        longitudo_totalis += II;  /* ": " */
        si (prop->valor && prop->valor->datum)
        {
            longitudo_totalis += prop->valor->mensura;
        }

        /* Verificare longitudo minima */
        si (longitudo_totalis == ZEPHYRUM)
        {
            longitudo_totalis = I;
        }

        item->altitudo = _calcular_altitudinem_textus(
            longitudo_totalis,
            latitudo_columnae);

        item->datum = prop;

        nav->numerus_itemorum++;
    }
}

/* Calcular items per pagina basatus in altitudine columnae
 */
interior vacuum
_calcular_items_per_pagina(
    NavigatorEntitatum* nav,
    i32                 altitudo_columnae)
{
    /* Pro nunc, assumere omnes items 1 linea */
    /* TODO: summa altitudines itemorum pro paginatio accurata */
    nav->items_per_pagina = altitudo_columnae;
    si (nav->items_per_pagina < I)
    {
        nav->items_per_pagina = I;
    }
}


/* ==================================================
 * Creatio
 * ================================================== */

NavigatorEntitatum*
navigator_entitatum_creare(
    Piscina*         piscina,
    EntitasProvidor* providor)
{
    NavigatorEntitatum* nav;
    Xar*                radices;
    Entitas**           radix_slot;

    si (!piscina || !providor)
    {
        redde NIHIL;
    }

    /* Allocare structuram */
    nav = (NavigatorEntitatum*)piscina_allocare(
        piscina,
        magnitudo(NavigatorEntitatum));
    si (!nav)
    {
        redde NIHIL;
    }

    /* Initiare campos */
    nav->providor           = providor;
    nav->piscina            = piscina;
    nav->entitas_currens    = NIHIL;
    nav->numerus_itemorum   = ZEPHYRUM;
    nav->selectio           = ZEPHYRUM;
    nav->pagina_currens     = ZEPHYRUM;
    nav->items_per_pagina   = XXX;  /* Valor initiarius */

    /* Creare Xar pro via navigationis */
    nav->via = xar_creare(piscina, magnitudo(ItemHistoriae));
    si (!nav->via)
    {
        redde NIHIL;
    }

    /* Tentare navigare ad primam radicem */
    radices = providor->capere_radices(providor->datum);
    si (radices && xar_numerus(radices) > ZEPHYRUM)
    {
        radix_slot = (Entitas**)xar_obtinere(radices, ZEPHYRUM);
        si (radix_slot && *radix_slot)
        {
            nav->entitas_currens = *radix_slot;
            /* Construere items cum latitudo assumpta - actualizabitur in reddere */
            _construere_items(nav, XL);  /* ~40 chars assumptum */
        }
    }

    redde nav;
}


/* ==================================================
 * Navigatio
 * ================================================== */

b32
navigator_entitatum_navigare_ad(
    NavigatorEntitatum* nav,
    chorda*             entitas_id)
{
    Entitas*        nova_entitas;
    ItemHistoriae*  item_historiae;

    si (!nav || !entitas_id)
    {
        redde FALSUM;
    }

    /* Capere novam entitatem ex providor */
    nova_entitas = nav->providor->capere_entitatem(
        nav->providor->datum,
        entitas_id);
    si (!nova_entitas)
    {
        redde FALSUM;  /* Non inventa */
    }

    /* Addere currens ad via (si existit) */
    si (nav->entitas_currens)
    {
        item_historiae = (ItemHistoriae*)xar_addere(nav->via);
        si (item_historiae)
        {
            item_historiae->entitas_id = nav->entitas_currens->id;
            item_historiae->entitas_id_destinatio = entitas_id;
        }
    }

    /* Ponere novam entitatem currens */
    nav->entitas_currens = nova_entitas;

    /* Reconstruere items - latitudo actualizabitur in proximo reddere */
    _construere_items(nav, XL);

    /* Reset selectio et pagina */
    nav->selectio = ZEPHYRUM;
    nav->pagina_currens = ZEPHYRUM;

    redde VERUM;
}

b32
navigator_entitatum_retro(
    NavigatorEntitatum* nav)
{
    ItemHistoriae*  item_historiae;
    Entitas*        entitas_praecedens;
    i32             numerus_items;

    si (!nav)
    {
        redde FALSUM;
    }

    numerus_items = xar_numerus(nav->via);
    si (numerus_items == ZEPHYRUM)
    {
        redde FALSUM;  /* Via vacua */
    }

    /* Obtinere item historiae */
    item_historiae = (ItemHistoriae*)xar_obtinere(nav->via, numerus_items - I);
    si (!item_historiae)
    {
        redde FALSUM;
    }

    /* Capere entitatem */
    entitas_praecedens = nav->providor->capere_entitatem(
        nav->providor->datum,
        item_historiae->entitas_id);
    si (!entitas_praecedens)
    {
        redde FALSUM;  /* Non inventa (non deberet accidere) */
    }

    /* Ponere currens */
    nav->entitas_currens = entitas_praecedens;

    /* Reconstruere items - latitudo actualizabitur in proximo reddere */
    _construere_items(nav, XL);

    /* Invenire selectionem: qua relatio ducit ad entitatem destinationis? */
    nav->selectio = ZEPHYRUM;
    si (item_historiae->entitas_id_destinatio)
    {
        i32 i;
        per (i = ZEPHYRUM; i < nav->numerus_itemorum; i++)
        {
            ItemNavigatoris* item;
            Relatio* rel;

            item = &nav->items[i];
            si (item->genus == ITEM_RELATIO)
            {
                rel = (Relatio*)item->datum;
                si (rel && rel->destinatio_id == item_historiae->entitas_id_destinatio)
                {
                    nav->selectio = i;
                    frange;
                }
            }
        }
    }

    nav->pagina_currens = nav->selectio / nav->items_per_pagina;

    /* Pop ex via */
    xar_truncare(nav->via, numerus_items - I);

    redde VERUM;
}


/* ==================================================
 * Tractatio Eventuum
 * ================================================== */

b32
navigator_entitatum_tractare_eventum(
    NavigatorEntitatum*  nav,
    constans Eventus*    eventus)
{
    ItemNavigatoris* item;
    Relatio*         rel;
    i32              ultimo_item_in_pagina;

    si (!nav || !eventus)
    {
        redde FALSUM;
    }

    si (eventus->genus != EVENTUS_CLAVIS_DEPRESSUS)
    {
        redde FALSUM;
    }

    /* j - Movere deorsum */
    si (eventus->datum.clavis.typus == 'j')
    {
        si (nav->selectio < nav->numerus_itemorum - I)
        {
            nav->selectio++;

            /* Verificare si necessarium movere ad paginam proximam */
            ultimo_item_in_pagina = (nav->pagina_currens + I) * nav->items_per_pagina - I;
            si (nav->selectio > ultimo_item_in_pagina)
            {
                nav->pagina_currens++;
            }
        }
        redde VERUM;
    }

    /* k - Movere sursum */
    si (eventus->datum.clavis.typus == 'k')
    {
        si (nav->selectio > ZEPHYRUM)
        {
            nav->selectio--;

            /* Verificare si necessarium movere ad paginam praecendentem */
            si (nav->selectio < nav->pagina_currens * nav->items_per_pagina)
            {
                nav->pagina_currens--;
            }
        }
        redde VERUM;
    }

    /* h - Retro in via */
    si (eventus->datum.clavis.typus == 'h')
    {
        navigator_entitatum_retro(nav);
        redde VERUM;
    }

    /* l - Intrare relationem */
    si (eventus->datum.clavis.typus == 'l')
    {
        si (nav->selectio >= nav->numerus_itemorum)
        {
            redde VERUM;  /* Selectio invalida */
        }

        item = &nav->items[nav->selectio];

        /* Pro nunc, solum relationes navigabiles */
        si (item->genus == ITEM_RELATIO)
        {
            rel = (Relatio*)item->datum;
            navigator_entitatum_navigare_ad(nav, rel->destinatio_id);
        }

        redde VERUM;
    }

    redde FALSUM;  /* Eventum non tractatum */
}


/* ==================================================
 * Redditionis
 * ================================================== */

/* Reddere entitatem in columna
 * "Render entity in a column"
 */
interior vacuum
_reddere_columnam_entitatis(
    TabulaPixelorum*     tabula,
    Entitas*             entitas,
    i32                  x_columna,
    i32                  y_initium,
    i32                  latitudo_columnae,
    i32                  altitudo_maxima,
    i32                  character_latitudo,
    i32                  character_altitudo,
    i32                  selectio_index,           /* Index item selecti, vel -1 */
    chorda*              selectio_destinatio_id,  /* ID destinationis pro highlight, vel NIHIL */
    b32                  dimmed)            /* VERUM pro colores obscuriores */
{
    ItemNavigatoris items_temp[CXXVIII];
    i32             numerus_items;
    i32             i;
    i32             y_currens;
    ItemNavigatoris* item;
    Relatio*        rel;
    Proprietas*     prop;
    character       buffer[CCLVI];
    i32             buffer_mensura;
    chorda          textus;
    i32             color_textus;
    i32             color_fons;
    i32             pixel_x;
    i32             pixel_y;
    i32             altitudo_item_pixels;
    i32             numerus;
    i32             j;
    b32             est_selectus;

    si (!tabula || !entitas)
    {
        redde;
    }

    /* Construere items pro hac entitate */
    numerus_items = ZEPHYRUM;

    /* Addere relationes */
    numerus = xar_numerus(entitas->relationes);
    per (i = ZEPHYRUM; i < numerus && numerus_items < CXXVIII; i++)
    {
        rel = (Relatio*)xar_obtinere(entitas->relationes, i);
        si (!rel)
        {
            perge;
        }

        item = &items_temp[numerus_items];
        item->genus = ITEM_RELATIO;
        item->altitudo = I;
        item->datum = rel;
        numerus_items++;
    }

    /* Sortare relationes alphabetice */
    si (numerus_items > I)
    {
        _sortare_relationes(items_temp, numerus_items);
    }

    /* Addere proprietates */
    numerus = xar_numerus(entitas->proprietates);
    per (i = ZEPHYRUM; i < numerus && numerus_items < CXXVIII; i++)
    {
        prop = (Proprietas*)xar_obtinere(entitas->proprietates, i);
        si (!prop)
        {
            perge;
        }

        item = &items_temp[numerus_items];
        item->genus = ITEM_PROPRIETAS;

        /* Calcular altitudo */
        {
            i32 longitudo_totalis;
            longitudo_totalis = ZEPHYRUM;
            si (prop->clavis && prop->clavis->datum)
            {
                longitudo_totalis += prop->clavis->mensura;
            }
            longitudo_totalis += II;  /* ": " */
            si (prop->valor && prop->valor->datum)
            {
                longitudo_totalis += prop->valor->mensura;
            }
            si (longitudo_totalis == ZEPHYRUM)
            {
                longitudo_totalis = I;
            }

            item->altitudo = _calcular_altitudinem_textus(
                longitudo_totalis,
                latitudo_columnae);
        }
        item->datum = prop;
        numerus_items++;
    }

    /* Reddere items */
    y_currens = y_initium;

    per (i = ZEPHYRUM; i < numerus_items && y_currens < y_initium + altitudo_maxima; i++)
    {
        item = &items_temp[i];

        /* Addere spatium inter relationes et proprietates */
        si (i > ZEPHYRUM && item->genus == ITEM_PROPRIETAS)
        {
            ItemNavigatoris* item_praecedens;
            item_praecedens = &items_temp[i - I];
            si (item_praecedens->genus == ITEM_RELATIO)
            {
                y_currens++;
            }
        }

        /* Determinare si hoc item debet esse selectus */
        est_selectus = FALSUM;

        /* Verificare si selectus per index */
        si (selectio_index >= ZEPHYRUM && i == selectio_index)
        {
            est_selectus = VERUM;
        }
        /* Verificare si selectus per destinatio ID */
        alioquin si (selectio_destinatio_id && item->genus == ITEM_RELATIO)
        {
            rel = (Relatio*)item->datum;
            si (rel && rel->destinatio_id == selectio_destinatio_id)
            {
                est_selectus = VERUM;
            }
        }

        /* Determinare colores */
        si (est_selectus)
        {
            si (dimmed)
            {
                /* Item selectus in columna dimmed - usare TEXT_DIM pro fondum */
                color_textus = thema_color(COLOR_BACKGROUND);
                color_fons   = thema_color(COLOR_TEXT_DIM);
            }
            alioquin
            {
                /* Item selectus in columna activa - invertere colores */
                color_textus = thema_color(COLOR_BACKGROUND);
                color_fons   = thema_color(COLOR_TEXT);
            }
        }
        alioquin si (dimmed)
        {
            /* Columna non-activa - usare colores obscuriores */
            color_textus = thema_color(COLOR_TEXT_DIM);
            color_fons   = thema_color(COLOR_BACKGROUND);
        }
        alioquin
        {
            color_textus = thema_color(COLOR_TEXT);
            color_fons   = thema_color(COLOR_BACKGROUND);
        }

        /* Reddere fondum si selectus */
        si (est_selectus)
        {
            i32 px, py;
            pixel_x = x_columna * character_latitudo;
            pixel_y = y_currens * character_altitudo;
            altitudo_item_pixels = item->altitudo * character_altitudo;

            per (py = pixel_y; py < pixel_y + altitudo_item_pixels; py++)
            {
                per (px = pixel_x; px < pixel_x + (latitudo_columnae * character_latitudo); px++)
                {
                    tabula_pixelorum_ponere_pixelum(tabula, px, py, color_fons);
                }
            }
        }

        /* Reddere textum item */
        si (item->genus == ITEM_RELATIO)
        {
            chorda arrow_textus;
            i32 color_arrow;
            character arrow_buffer[III];

            rel = (Relatio*)item->datum;
            si (!rel)
            {
                y_currens += item->altitudo;
                perge;
            }

            /* Reddere "> " */
            color_arrow = dimmed ? thema_color(COLOR_TEXT_DIM) : thema_color(COLOR_ACCENT_PRIMARY);
            arrow_buffer[ZEPHYRUM] = '>';
            arrow_buffer[I] = ' ';
            arrow_textus.datum = (i8*)arrow_buffer;
            arrow_textus.mensura = II;
            tabula_pixelorum_pingere_chordam(
                tabula,
                x_columna * character_latitudo,
                y_currens * character_altitudo,
                arrow_textus,
                color_arrow);

            /* Format: "genus/" */
            buffer_mensura = ZEPHYRUM;
            si (rel->genus && rel->genus->datum && rel->genus->mensura > ZEPHYRUM &&
                rel->genus->mensura < CCLVI - buffer_mensura - X)
            {
                memcpy(buffer + buffer_mensura,
                       rel->genus->datum,
                       (memoriae_index)rel->genus->mensura);
                buffer_mensura += rel->genus->mensura;
            }

            buffer[buffer_mensura++] = '/';
            buffer[buffer_mensura] = '\0';

            textus.datum = (i8*)buffer;
            textus.mensura = buffer_mensura;

            tabula_pixelorum_pingere_chordam(
                tabula,
                x_columna * character_latitudo + (II * character_latitudo),
                y_currens * character_altitudo,
                textus,
                color_textus);
        }
        alioquin si (item->genus == ITEM_PROPRIETAS)
        {
            i32 offset_textus;
            i32 linea_currens;
            i32 caracteres_in_linea;

            prop = (Proprietas*)item->datum;
            si (!prop)
            {
                y_currens += item->altitudo;
                perge;
            }

            /* Format: "clavis: valor" */
            buffer_mensura = ZEPHYRUM;

            si (prop->clavis && prop->clavis->datum && prop->clavis->mensura > ZEPHYRUM &&
                prop->clavis->mensura < CCLVI - X)
            {
                memcpy(buffer + buffer_mensura,
                       prop->clavis->datum,
                       (memoriae_index)prop->clavis->mensura);
                buffer_mensura += prop->clavis->mensura;
            }

            buffer[buffer_mensura++] = ':';
            buffer[buffer_mensura++] = ' ';

            si (prop->valor && prop->valor->datum && prop->valor->mensura > ZEPHYRUM &&
                prop->valor->mensura < CCLVI - buffer_mensura)
            {
                memcpy(buffer + buffer_mensura,
                       prop->valor->datum,
                       (memoriae_index)prop->valor->mensura);
                buffer_mensura += prop->valor->mensura;
            }

            buffer[buffer_mensura] = '\0';

            /* Reddere cum wrapping */
            offset_textus = ZEPHYRUM;
            linea_currens = ZEPHYRUM;

            dum (offset_textus < buffer_mensura)
            {
                caracteres_in_linea = ZEPHYRUM;

                per (j = ZEPHYRUM; j < latitudo_columnae && offset_textus + j < buffer_mensura; j++)
                {
                    caracteres_in_linea++;
                }

                textus.datum = (i8*)(buffer + offset_textus);
                textus.mensura = caracteres_in_linea;

                tabula_pixelorum_pingere_chordam(
                    tabula,
                    x_columna * character_latitudo,
                    (y_currens + linea_currens) * character_altitudo,
                    textus,
                    color_textus);

                offset_textus += caracteres_in_linea;
                linea_currens++;
            }
        }

        y_currens += item->altitudo;
    }
}

vacuum
navigator_entitatum_reddere(
    NavigatorEntitatum*  nav,
    TabulaPixelorum*     tabula,
    i32                  x,
    i32                  y,
    i32                  latitudo,
    i32                  altitudo,
    i32                  scala)
{
    i32              latitudo_columnae;
    i32              x_sinistra;
    i32              x_media;
    i32              x_dextra;
    i32              character_latitudo;
    i32              character_altitudo;
    Entitas*         entitas_parens;
    Entitas*         entitas_praeviso;
    ItemNavigatoris* item_selectus;
    Relatio*         rel;
    ItemHistoriae*   item_historiae;
    i32              numerus_items_via;

    si (!nav || !tabula)
    {
        redde;
    }

    character_latitudo = VI * scala;
    character_altitudo = VIII * scala;

    /* Tres columnae aequales */
    latitudo_columnae = latitudo / III;
    x_sinistra = x;
    x_media = x + latitudo_columnae;
    x_dextra = x + (latitudo_columnae * II);

    /* Reconstruere items si latitudo columnae mutata */
    _construere_items(nav, latitudo_columnae);

    /* Calcular items per pagina */
    _calcular_items_per_pagina(nav, altitudo);

    /* === COLUMNA SINISTRA: Entitas parens === */
    entitas_parens = NIHIL;
    item_historiae = NIHIL;
    numerus_items_via = xar_numerus(nav->via);
    si (numerus_items_via > ZEPHYRUM)
    {
        /* Capere entitatem parens ex via */
        item_historiae = (ItemHistoriae*)xar_obtinere(nav->via, numerus_items_via - I);
        si (item_historiae)
        {
            entitas_parens = nav->providor->capere_entitatem(
                nav->providor->datum,
                item_historiae->entitas_id);
        }
    }

    si (entitas_parens)
    {
        _reddere_columnam_entitatis(
            tabula,
            entitas_parens,
            x_sinistra,
            y,
            latitudo_columnae,
            altitudo,
            character_latitudo,
            character_altitudo,
            (i32)(-I),   /* Non usare index */
            nav->entitas_currens ? nav->entitas_currens->id : NIHIL,
            VERUM);    /* Dimmed */
    }

    /* === COLUMNA MEDIA: Entitas currens === */
    si (nav->entitas_currens)
    {
        _reddere_columnam_entitatis(
            tabula,
            nav->entitas_currens,
            x_media,
            y,
            latitudo_columnae,
            altitudo,
            character_latitudo,
            character_altitudo,
            nav->selectio,   /* Usare index pro columna activa */
            NIHIL,           /* Non usare destinatio ID */
            FALSUM);   /* Non dimmed */
    }

    /* === COLUMNA DEXTRA: Praeviso item selecti === */
    entitas_praeviso = NIHIL;

    si (nav->selectio >= ZEPHYRUM && nav->selectio < nav->numerus_itemorum)
    {
        item_selectus = &nav->items[nav->selectio];

        /* Si item selectus est relatio, monstrare entitatem destinationis */
        si (item_selectus->genus == ITEM_RELATIO)
        {
            rel = (Relatio*)item_selectus->datum;
            si (rel && rel->destinatio_id)
            {
                entitas_praeviso = nav->providor->capere_entitatem(
                    nav->providor->datum,
                    rel->destinatio_id);
            }
        }
        /* Si proprietas, pro nunc nihil (in futuro possumus monstrare valorem plenum) */
    }

    si (entitas_praeviso)
    {
        _reddere_columnam_entitatis(
            tabula,
            entitas_praeviso,
            x_dextra,
            y,
            latitudo_columnae,
            altitudo,
            character_latitudo,
            character_altitudo,
            (i32)(-I),   /* Nulla selectio per index */
            NIHIL,       /* Nulla selectio per ID */
            VERUM);    /* Dimmed */
    }

    /* TODO: Indicatores paginationis */
}
