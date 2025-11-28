#include "navigator_entitatum.h"
#include "thema.h"
#include "xar.h"
#include "delineare.h"
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

/* Obtinere chorda pro comparatione relationis
 * Pro "contains" relationes, reddit titulum destinationis
 * Pro aliis, reddit genus relationis
 */
interior chorda*
_obtinere_chorda_sortationis(
    Relatio*             rel,
    EntitasRepositorium* repositorium)
{
    hic_manens i8 contains_lit[] = "contains";

    si (!rel)
    {
        redde NIHIL;
    }

    /* Verificare si est "contains" relatio */
    si (rel->genus && rel->genus->datum && rel->genus->mensura == VIII)
    {
        si (memcmp(rel->genus->datum, contains_lit, VIII) == ZEPHYRUM)
        {
            /* Pro contains, usare titulum destinationis */
            si (repositorium && rel->destinatio_id)
            {
                Entitas* dest;
                dest = repositorium->capere_entitatem(
                    repositorium->datum,
                    rel->destinatio_id);
                si (dest)
                {
                    redde entitas_titulum_capere(dest);
                }
            }
        }
    }

    /* Pro aliis relationibus, usare genus */
    redde rel->genus;
}

/* Comparare duo chordas case-insensitive
 * Reddit: < 0 si a < b, 0 si a == b, > 0 si a > b
 */
interior s32
_comparare_case_insensitive(
    chorda* a,
    chorda* b)
{
    i32 i;
    i32 min_len;
    i8  char_a;
    i8  char_b;

    si (!a || !a->datum)
    {
        si (!b || !b->datum)
        {
            redde ZEPHYRUM;
        }
        redde (s32)(-I);
    }
    si (!b || !b->datum)
    {
        redde I;
    }

    min_len = a->mensura < b->mensura ? a->mensura : b->mensura;

    per (i = ZEPHYRUM; i < min_len; i++)
    {
        char_a = a->datum[i];
        char_b = b->datum[i];

        /* Convertere ad minusculas */
        si (char_a >= 'A' && char_a <= 'Z')
        {
            char_a = (i8)(char_a + ('a' - 'A'));
        }
        si (char_b >= 'A' && char_b <= 'Z')
        {
            char_b = (i8)(char_b + ('a' - 'A'));
        }

        si (char_a != char_b)
        {
            redde (s32)(char_a - char_b);
        }
    }

    redde (s32)(a->mensura - b->mensura);
}

/* Sortare relationes alphabetice
 * Pro "contains" relationes, sortat per titulum destinationis
 * Pro aliis relationibus, sortat per genus
 * Comparatio est case-insensitive
 */
interior vacuum
_sortare_relationes(
    ItemNavigatoris*     items,
    i32                  numerus_relationum,
    EntitasRepositorium* repositorium)
{
    i32 i;
    i32 j;
    i32 k;
    ItemNavigatoris temp;
    Relatio* rel_i;
    Relatio* rel_j;
    chorda* chorda_i;
    chorda* chorda_j;
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

            /* Obtinere chordas pro comparatione */
            chorda_i = _obtinere_chorda_sortationis(rel_i, repositorium);
            chorda_j = _obtinere_chorda_sortationis(rel_j, repositorium);

            /* Comparare case-insensitive */
            si (_comparare_case_insensitive(chorda_i, chorda_j) < ZEPHYRUM)
            {
                debet_movere = VERUM;
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
 * Si est in radice, addit etiam "Genus" entry point
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
    i32          numerus_items_via;

    si (!nav || !nav->entitas_currens)
    {
        redde;
    }

    ent = nav->entitas_currens;
    nav->numerus_itemorum = ZEPHYRUM;

    (vacuum)numerus_items_via;  /* Non usatum post remotionem genus mode */

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
        _sortare_relationes(nav->items, nav->numerus_itemorum, nav->repositorium);
    }

    /* Addere genus ut pseudo-proprietas */
    si (ent->genus && ent->genus->datum && nav->numerus_itemorum < CXXVIII)
    {
        hic_manens Proprietas prop_genus;
        hic_manens chorda clavis_genus;
        hic_manens i8 clavis_lit[] = "genus";

        clavis_genus.datum = clavis_lit;
        clavis_genus.mensura = V;

        prop_genus.clavis = &clavis_genus;
        prop_genus.valor = ent->genus;

        item = &nav->items[nav->numerus_itemorum];
        item->genus = ITEM_PROPRIETAS;

        /* Calcular altitudo */
        longitudo_totalis = clavis_genus.mensura + II + ent->genus->mensura;
        item->altitudo = _calcular_altitudinem_textus(
            longitudo_totalis,
            latitudo_columnae);

        item->datum = &prop_genus;
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
    Piscina*             piscina,
    EntitasRepositorium* repositorium)
{
    NavigatorEntitatum* nav;
    Xar*                radices;
    Entitas**           radix_slot;

    si (!piscina || !repositorium)
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
    nav->repositorium       = repositorium;
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
    radices = repositorium->capere_radices(repositorium->datum);
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

    /* Capere novam entitatem ex repositorium */
    nova_entitas = nav->repositorium->capere_entitatem(
        nav->repositorium->datum,
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
    entitas_praecedens = nav->repositorium->capere_entitatem(
        nav->repositorium->datum,
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
        /* Navigatio normalis retro */
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

        si (item->genus == ITEM_RELATIO)
        {
            /* Navigatio normalis per relationes */
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

/* Reddere items currens (usare nav->items directe)
 * "Render current items from navigator state"
 * Hoc includit ITEM_RELATIO, ITEM_PROPRIETAS
 */
interior vacuum
_reddere_items_currens(
    NavigatorEntitatum*  nav,
    TabulaPixelorum*     tabula,
    i32                  x_columna,
    i32                  y_initium,
    i32                  latitudo_columnae,
    i32                  altitudo_maxima,
    i32                  character_latitudo,
    i32                  character_altitudo)
{
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
    i32              color_arrow;
    i32              pixel_x;
    i32              pixel_y;
    i32              altitudo_item_pixels;
    b32              est_selectus;
    i32              j;

    (vacuum)altitudo_maxima;

    si (!nav || !tabula)
    {
        redde;
    }

    y_currens = y_initium;

    per (i = ZEPHYRUM; i < nav->numerus_itemorum; i++)
    {
        item = &nav->items[i];
        est_selectus = (i == nav->selectio);

        /* Addere spatium inter relationes et proprietates */
        si (i > ZEPHYRUM && item->genus == ITEM_PROPRIETAS)
        {
            ItemNavigatoris* item_praecedens;
            item_praecedens = &nav->items[i - I];
            si (item_praecedens->genus == ITEM_RELATIO)
            {
                y_currens++;
            }
        }

        /* Determinare colores */
        si (est_selectus)
        {
            color_textus = color_ad_pixelum(thema_color(COLOR_BACKGROUND));
            color_fons   = color_ad_pixelum(thema_color(COLOR_TEXT));
        }
        alioquin
        {
            color_textus = color_ad_pixelum(thema_color(COLOR_TEXT));
            color_fons   = color_ad_pixelum(thema_color(COLOR_BACKGROUND));
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

        /* Reddere item per genus */
        si (item->genus == ITEM_RELATIO)
        {
            chorda arrow_textus;
            character arrow_buffer[III];
            b32 est_contains;

            rel = (Relatio*)item->datum;
            si (!rel)
            {
                y_currens += item->altitudo;
                perge;
            }

            /* Verificare si relatio est "contains" */
            est_contains = FALSUM;
            si (rel->genus && rel->genus->datum && rel->genus->mensura == VIII)
            {
                hic_manens i8 contains_lit[] = "contains";
                si (memcmp(rel->genus->datum, contains_lit, VIII) == ZEPHYRUM)
                {
                    est_contains = VERUM;
                }
            }

            /* Reddere prefix - "(" pro contains (dark cyan), ">" pro aliis (bright cyan) */
            color_arrow = est_contains
                ? color_ad_pixelum(color_ex_palette(PALETTE_DARK_CYAN))
                : color_ad_pixelum(thema_color(COLOR_ACCENT_PRIMARY));
            arrow_buffer[ZEPHYRUM] = est_contains ? '(' : '>';
            arrow_buffer[I] = ' ';
            arrow_textus.datum = (i8*)arrow_buffer;
            arrow_textus.mensura = II;
            tabula_pixelorum_pingere_chordam(
                tabula,
                x_columna * character_latitudo,
                y_currens * character_altitudo,
                arrow_textus,
                est_selectus ? color_textus : color_arrow);

            si (est_contains)
            {
                /* Relatio "contains" - monstrare titulum destinationis */
                Entitas* dest_ent;
                chorda* display_chorda;

                dest_ent = nav->repositorium->capere_entitatem(
                    nav->repositorium->datum, rel->destinatio_id);

                display_chorda = entitas_titulum_capere(dest_ent);

                /* Format: "name/" (sine "contains") */
                buffer_mensura = ZEPHYRUM;
                si (display_chorda && display_chorda->datum && display_chorda->mensura > ZEPHYRUM &&
                    display_chorda->mensura < CCLVI - buffer_mensura - X)
                {
                    memcpy(buffer + buffer_mensura,
                           display_chorda->datum,
                           (memoriae_index)display_chorda->mensura);
                    buffer_mensura += display_chorda->mensura;
                }
                buffer[buffer_mensura++] = '/';
                buffer[buffer_mensura] = '\0';
            }
            alioquin
            {
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
            }

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

/* Reddere entitatem in columna
 * "Render entity in a column"
 */
interior vacuum
_reddere_columnam_entitatis(
    TabulaPixelorum*     tabula,
    Entitas*             entitas,
    EntitasRepositorium* selectio_repositorium,   /* Pro resolvere "contains" */
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
        _sortare_relationes(items_temp, numerus_items, selectio_repositorium);
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
                color_textus = color_ad_pixelum(thema_color(COLOR_BACKGROUND));
                color_fons   = color_ad_pixelum(thema_color(COLOR_TEXT_DIM));
            }
            alioquin
            {
                /* Item selectus in columna activa - invertere colores */
                color_textus = color_ad_pixelum(thema_color(COLOR_BACKGROUND));
                color_fons   = color_ad_pixelum(thema_color(COLOR_TEXT));
            }
        }
        alioquin si (dimmed)
        {
            /* Columna non-activa - usare colores obscuriores */
            color_textus = color_ad_pixelum(thema_color(COLOR_TEXT_DIM));
            color_fons   = color_ad_pixelum(thema_color(COLOR_BACKGROUND));
        }
        alioquin
        {
            color_textus = color_ad_pixelum(thema_color(COLOR_TEXT));
            color_fons   = color_ad_pixelum(thema_color(COLOR_BACKGROUND));
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
            b32 est_contains;

            rel = (Relatio*)item->datum;
            si (!rel)
            {
                y_currens += item->altitudo;
                perge;
            }

            /* Verificare si relatio est "contains" */
            est_contains = FALSUM;
            si (rel->genus && rel->genus->datum && rel->genus->mensura == VIII)
            {
                hic_manens i8 contains_lit[] = "contains";
                si (memcmp(rel->genus->datum, contains_lit, VIII) == ZEPHYRUM)
                {
                    est_contains = VERUM;
                }
            }

            /* Reddere prefix - "(" pro contains (dark cyan), ">" pro aliis (bright cyan) */
            si (dimmed)
            {
                color_arrow = color_ad_pixelum(thema_color(COLOR_TEXT_DIM));
            }
            alioquin si (est_contains)
            {
                color_arrow = color_ad_pixelum(color_ex_palette(PALETTE_DARK_CYAN));
            }
            alioquin
            {
                color_arrow = color_ad_pixelum(thema_color(COLOR_ACCENT_PRIMARY));
            }
            arrow_buffer[ZEPHYRUM] = est_contains ? '(' : '>';
            arrow_buffer[I] = ' ';
            arrow_textus.datum = (i8*)arrow_buffer;
            arrow_textus.mensura = II;
            tabula_pixelorum_pingere_chordam(
                tabula,
                x_columna * character_latitudo,
                y_currens * character_altitudo,
                arrow_textus,
                color_arrow);

            si (est_contains && selectio_repositorium)
            {
                /* Relatio "contains" - monstrare titulum destinationis */
                Entitas* dest_ent;
                chorda* display_chorda;

                dest_ent = selectio_repositorium->capere_entitatem(
                    selectio_repositorium->datum, rel->destinatio_id);

                display_chorda = entitas_titulum_capere(dest_ent);

                /* Format: "name/" (sine "contains") */
                buffer_mensura = ZEPHYRUM;
                si (display_chorda && display_chorda->datum && display_chorda->mensura > ZEPHYRUM &&
                    display_chorda->mensura < CCLVI - buffer_mensura - X)
                {
                    memcpy(buffer + buffer_mensura,
                           display_chorda->datum,
                           (memoriae_index)display_chorda->mensura);
                    buffer_mensura += display_chorda->mensura;
                }
                buffer[buffer_mensura++] = '/';
                buffer[buffer_mensura] = '\0';
            }
            alioquin
            {
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
            }

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
    i32                  scala,
    b32                  focused)
{
    i32              latitudo_columnae;
    i32              latitudo_sinistra;
    i32              latitudo_media;
    i32              x_sinistra;
    i32              x_media;
    i32              character_latitudo;
    i32              character_altitudo;
    i32              box_x0, box_y0, box_x1, box_y1;
    i32              textus_latitudo;
    i32              textus_altitudo;
    Color            color_border;
    Color            color_border_inner;
    Entitas*         entitas_parens;
    ItemHistoriae*   item_historiae;
    i32              numerus_items_via;
    ContextusDelineandi* ctx;
    chorda           titulo;

    si (!nav || !tabula)
    {
        redde;
    }

    character_latitudo = VI * scala;
    character_altitudo = VIII * scala;

    /* Computare dimensiones areae textus (minus border) */
    textus_latitudo = latitudo - II;
    textus_altitudo = altitudo - II;

    /* Computare coordinatas pixelorum pro box (with padding from edges) */
    box_x0 = x * character_latitudo + II;
    box_y0 = y * character_altitudo + II;
    box_x1 = (x + latitudo) * character_latitudo - I - II;
    box_y1 = (y + altitudo) * character_altitudo - I - II;

    color_border = thema_color(COLOR_BORDER);
    color_border_inner = focused ? thema_color(COLOR_BORDER_ACTIVE) : color_border;

    /* === REDDERE BORDER === */
    ctx = delineare_creare_contextum(nav->piscina, tabula);

    /* Titulo pro border - usare titulum entitatis (name/title/id) */
    {
        chorda* titulum_chorda = entitas_titulum_capere(nav->entitas_currens);
        si (titulum_chorda && titulum_chorda->datum)
        {
            titulo = *titulum_chorda;
        }
        alioquin
        {
            hic_manens character buffer_titulo[XVI];
            i32 i;
            constans character* texto = "Navigator";
            per (i = ZEPHYRUM; texto[i] != '\0' && i < XV; i++)
            {
                buffer_titulo[i] = texto[i];
            }
            buffer_titulo[i] = '\0';
            titulo.datum = (i8*)buffer_titulo;
            titulo.mensura = i;
        }
    }

    /* Computare positiones textus (centered on top line) */
    {
        i32 titulo_pixel_width;
        i32 titulo_x, titulo_y;
        i32 gap_padding;

        gap_padding = character_latitudo;

        titulo_pixel_width = titulo.mensura * character_latitudo;
        titulo_x = box_x0 + ((box_x1 - box_x0) - titulo_pixel_width) / II;
        titulo_y = box_y0 - (character_altitudo / II) + III;

        /* Top line - left segment (double line with gap) */
        delineare_lineam_horizontalem(ctx, box_x0, titulo_x - gap_padding, box_y0, color_border);
        delineare_lineam_horizontalem(ctx, box_x0, titulo_x - gap_padding, box_y0 + II, color_border_inner);

        /* Top line - right segment */
        delineare_lineam_horizontalem(ctx, titulo_x + titulo_pixel_width + gap_padding, box_x1, box_y0, color_border);
        delineare_lineam_horizontalem(ctx, titulo_x + titulo_pixel_width + gap_padding, box_x1, box_y0 + II, color_border_inner);

        /* Bottom line (full double line) */
        delineare_lineam_horizontalem(ctx, box_x0, box_x1, box_y1, color_border);
        delineare_lineam_horizontalem(ctx, box_x0, box_x1, box_y1 - II, color_border_inner);

        /* Left line (double with gap) */
        delineare_lineam_verticalem(ctx, box_x0, box_y0, box_y1, color_border);
        delineare_lineam_verticalem(ctx, box_x0 + II, box_y0, box_y1, color_border_inner);

        /* Right line (double with gap) */
        delineare_lineam_verticalem(ctx, box_x1, box_y0, box_y1, color_border);
        delineare_lineam_verticalem(ctx, box_x1 - II, box_y0, box_y1, color_border_inner);

        /* Pingere titulo */
        tabula_pixelorum_pingere_chordam(tabula, titulo_x, titulo_y, titulo, color_ad_pixelum(color_border));
    }

    /* Calcular latitudines columnarum - solum duo columnae (30/70 split) */
    latitudo_sinistra = (textus_latitudo * III) / X;  /* 30% */
    latitudo_media = textus_latitudo - latitudo_sinistra;  /* 70% */
    latitudo_columnae = latitudo_media;

    x_sinistra = x + I;  /* +1 pro border */
    x_media = x_sinistra + latitudo_sinistra;

    /* Reconstruere items si latitudo columnae mutata */
    _construere_items(nav, latitudo_columnae);

    /* Calcular items per pagina */
    _calcular_items_per_pagina(nav, textus_altitudo);

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
            entitas_parens = nav->repositorium->capere_entitatem(
                nav->repositorium->datum,
                item_historiae->entitas_id);
        }
    }

    si (entitas_parens)
    {
        _reddere_columnam_entitatis(
            tabula,
            entitas_parens,
            nav->repositorium,
            x_sinistra,
            y + I,  /* +1 pro border */
            latitudo_sinistra - I,  /* -1 pro spatio divider */
            textus_altitudo,
            character_latitudo,
            character_altitudo,
            (i32)(-I),   /* Non usare index */
            nav->entitas_currens ? nav->entitas_currens->id : NIHIL,
            VERUM);    /* Dimmed */
    }

    /* === COLUMNA MEDIA: Items currens (usare nav->items) === */
    si (nav->numerus_itemorum > ZEPHYRUM)
    {
        _reddere_items_currens(
            nav,
            tabula,
            x_media,
            y + I,  /* +1 pro border */
            latitudo_media,
            textus_altitudo,
            character_latitudo,
            character_altitudo);
    }

    /* === REDDERE DIVIDER INTER COLUMNAS === */
    {
        i32 divider_x;
        i32 divider_y0;
        i32 divider_y1;

        /* Coordinatas pixelorum pro divider */
        divider_x = x_media * character_latitudo - II;  /* 2px ad sinistram */
        divider_y0 = box_y0 + II;  /* Start at top border edge */
        divider_y1 = box_y1 - II;  /* End inside border */

        /* Divider inter columnam sinistram et mediam (double line cum gap) */
        delineare_lineam_verticalem(ctx, divider_x - II, divider_y0, divider_y1, color_border);
        delineare_lineam_verticalem(ctx, divider_x, divider_y0, divider_y1, color_border);
    }
}
