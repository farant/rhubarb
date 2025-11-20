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
        si (prop->clavis)
        {
            longitudo_totalis += prop->clavis->mensura;
        }
        longitudo_totalis += II;  /* ": " */
        si (prop->valor)
        {
            longitudo_totalis += prop->valor->mensura;
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
            item_historiae->selectio = nav->selectio;
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

    /* Restituere selectionem et paginam */
    nav->selectio = item_historiae->selectio;
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
    i32              x_media;
    i32              character_latitudo;
    i32              character_altitudo;
    i32              primus_item;
    i32              ultimus_item;
    i32              i;
    i32              y_currens;
    ItemNavigatoris* item;
    Relatio*         rel;
    Proprietas*      prop;
    character        buffer[CCLVI];
    i32              buffer_mensura;
    chorda           textus;
    i32              color_textus;
    i32              color_fons;
    i32              pixel_x;
    i32              pixel_y;
    i32              altitudo_item_pixels;

    si (!nav || !tabula)
    {
        redde;
    }

    character_latitudo = VI * scala;
    character_altitudo = VIII * scala;

    /* Tres columnae aequales */
    latitudo_columnae = latitudo / III;

    /* Pro nunc, solum columna media reddita */
    x_media = x + latitudo_columnae;

    /* Reconstruere items si latitudo columnae mutata */
    _construere_items(nav, latitudo_columnae);

    /* Calcular items per pagina */
    _calcular_items_per_pagina(nav, altitudo);

    /* Calcular quales items reddere (paginatio) */
    primus_item = nav->pagina_currens * nav->items_per_pagina;
    ultimus_item = primus_item + nav->items_per_pagina;
    si (ultimus_item > nav->numerus_itemorum)
    {
        ultimus_item = nav->numerus_itemorum;
    }

    /* Reddere columnam mediam (entitas currens) */
    y_currens = y;

    per (i = primus_item; i < ultimus_item; i++)
    {
        item = &nav->items[i];

        /* Determinare colores */
        si (i == nav->selectio)
        {
            /* Item selectus - invertere colores */
            color_textus = thema_color(COLOR_BACKGROUND);
            color_fons   = thema_color(COLOR_TEXT);
        }
        alioquin
        {
            color_textus = thema_color(COLOR_TEXT);
            color_fons   = thema_color(COLOR_BACKGROUND);
        }

        /* Reddere fondum si selectus */
        si (i == nav->selectio)
        {
            i32 px, py;
            pixel_x = x_media * character_latitudo;
            pixel_y = y_currens * character_altitudo;
            altitudo_item_pixels = item->altitudo * character_altitudo;

            /* Pingere rectangulum pixel per pixel */
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
            rel = (Relatio*)item->datum;

            /* Format: "→ genus/ (count)" */
            buffer[ZEPHYRUM] = (character)0xE2;  /* UTF-8 → */
            buffer[I] = (character)0x86;
            buffer[II] = (character)0x92;
            buffer[III] = ' ';
            buffer_mensura = IV;

            /* Copiar genus */
            si (rel->genus && rel->genus->mensura < CCLVI - buffer_mensura - X)
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
                x_media * character_latitudo,
                y_currens * character_altitudo,
                textus,
                color_textus);
        }
        alioquin si (item->genus == ITEM_PROPRIETAS)
        {
            i32 offset_textus;
            i32 linea_currens;
            i32 caracteres_in_linea;
            i32 j;

            prop = (Proprietas*)item->datum;

            /* Format: "clavis: valor" */
            buffer_mensura = ZEPHYRUM;

            /* Copiar clavis */
            si (prop->clavis && prop->clavis->mensura < CCLVI - X)
            {
                memcpy(buffer + buffer_mensura,
                       prop->clavis->datum,
                       (memoriae_index)prop->clavis->mensura);
                buffer_mensura += prop->clavis->mensura;
            }

            buffer[buffer_mensura++] = ':';
            buffer[buffer_mensura++] = ' ';

            /* Copiar valor */
            si (prop->valor && prop->valor->mensura < CCLVI - buffer_mensura)
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

                /* Copiar caracteres pro hac linea */
                per (j = ZEPHYRUM; j < latitudo_columnae && offset_textus + j < buffer_mensura; j++)
                {
                    caracteres_in_linea++;
                }

                /* Reddere hanc lineam */
                textus.datum = (i8*)(buffer + offset_textus);
                textus.mensura = caracteres_in_linea;

                tabula_pixelorum_pingere_chordam(
                    tabula,
                    x_media * character_latitudo,
                    (y_currens + linea_currens) * character_altitudo,
                    textus,
                    color_textus);

                offset_textus += caracteres_in_linea;
                linea_currens++;
            }
        }

        y_currens += item->altitudo;
    }

    /* TODO: Reddere columnam sinistram (entitas parens) */
    /* TODO: Reddere columnam dextram (praeviso) */
    /* TODO: Indicatores paginationis */
}
