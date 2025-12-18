#include "schirmata.h"
#include "thema.h"
#include "delineare.h"
#include "tempus.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* Timeout pro praefixum ctrl-a (secundae) */
#define PRAEFIXUM_TIMEOUT  2.0

/* ==================================================
 * Datum Internum pro Widgets
 * ================================================== */

/* Datum pro libro widget wrapper */
nomen structura {
    LibroPaginarum* libro;
    Schirmata*      schirmata;  /* Pro accedere reg_commandi */
} SchirmataLibroDatum;

/* Datum pro navigator widget wrapper */
nomen structura {
    NavigatorEntitatum* navigator;
    Piscina*            piscina;
} SchirmataNavigatorDatum;


/* ==================================================
 * Widget Wrapper Functiones - Libro
 * ================================================== */

hic_manens vacuum
_schirmata_libro_reddere(
    Widget*          widget,
    TabulaPixelorum* tabula,
    i32              x,
    i32              y,
    i32              latitudo,
    i32              altitudo,
    i32              scala,
    b32              focused)
{
    SchirmataLibroDatum* datum;

    datum = (SchirmataLibroDatum*)widget->datum;

    libro_reddere(
        datum->schirmata->piscina,
        tabula,
        datum->libro,
        x,
        y,
        latitudo,
        altitudo,
        scala,
        focused);
}

hic_manens b32
_schirmata_libro_tractare_eventum(
    Widget*           widget,
    constans Eventus* eventus)
{
    SchirmataLibroDatum* datum;
    Pagina* pagina;

    datum = (SchirmataLibroDatum*)widget->datum;

    /* Tractare keyboard navigation */
    si (eventus->genus == EVENTUS_CLAVIS_DEPRESSUS)
    {
        /* Ctrl+Shift+Right -> pagina proxima */
        si ((eventus->datum.clavis.modificantes & MOD_IMPERIUM) &&
            (eventus->datum.clavis.modificantes & MOD_SHIFT))
        {
            si (eventus->datum.clavis.clavis == CLAVIS_DEXTER)
            {
                libro_pagina_proxima(datum->libro);
                redde VERUM;
            }
            /* Ctrl+Shift+Left -> pagina prior */
            si (eventus->datum.clavis.clavis == CLAVIS_SINISTER)
            {
                libro_pagina_prior(datum->libro);
                redde VERUM;
            }
        }
    }

    /* Tractare mouse clicks pro tag detection (commands et links) */
    si (eventus->genus == EVENTUS_MUS_DEPRESSUS && datum->schirmata->reg_commandi)
    {
        RegioClicca regio;
        i32 click_x;
        i32 click_y;
        i32 character_latitudo;
        i32 character_altitudo;

        pagina = libro_pagina_currens(datum->libro);
        si (!pagina)
        {
            redde VERUM;
        }

        character_latitudo = VI;   /* 6 pixels per character */
        character_altitudo = VIII; /* 8 pixels per character */

        /* Convertere pixel ad character coordinates */
        click_x = (eventus->datum.mus.x / character_latitudo) - widget->x - I;
        click_y = (eventus->datum.mus.y / character_altitudo) - widget->y - I;

        /* Tentare detegere tag ad click position */
        si (pagina_obtinere_regio_ad_punctum(pagina, click_y, click_x, &regio))
        {
            si (strcmp(regio.genus, "command") == ZEPHYRUM)
            {
                ContextusCommandi ctx;

                ctx.pagina = pagina;
                ctx.linea = regio.finis_linea;
                ctx.columna = regio.finis_columna;
                ctx.piscina = datum->schirmata->piscina;
                ctx.datum_custom = datum->libro;

                registrum_commandi_executare(datum->schirmata->reg_commandi, regio.datum, &ctx);

                redde VERUM;
            }
            alioquin si (strcmp(regio.genus, "link") == ZEPHYRUM)
            {
                constans character* link = regio.datum;

                si (link[ZEPHYRUM] == '#')
                {
                    link++;

                    si (strcmp(link, "back") == ZEPHYRUM)
                    {
                        libro_retro(datum->libro);
                    }
                    alioquin si (strcmp(link, "next") == ZEPHYRUM)
                    {
                        libro_pagina_proxima(datum->libro);
                    }
                    alioquin si (strcmp(link, "prev") == ZEPHYRUM)
                    {
                        libro_pagina_prior(datum->libro);
                    }
                    alioquin si (link[ZEPHYRUM] >= '0' && link[ZEPHYRUM] <= '9')
                    {
                        s32 page_num = atoi(link) - I;
                        libro_navigare_ad(datum->libro, page_num);
                    }
                    alioquin
                    {
                        libro_navigare_ad_nomen(datum->libro, link);
                    }
                    redde VERUM;
                }
            }
        }
    }

    /* Delegare ad libro tractare_eventum */
    redde libro_tractare_eventum(datum->libro, eventus);
}


/* ==================================================
 * Widget Wrapper Functiones - Navigator
 * ================================================== */

hic_manens vacuum
_schirmata_navigator_reddere(
    Widget*          widget,
    TabulaPixelorum* tabula,
    i32              x,
    i32              y,
    i32              latitudo,
    i32              altitudo,
    i32              scala,
    b32              focused)
{
    SchirmataNavigatorDatum* datum;

    datum = (SchirmataNavigatorDatum*)widget->datum;

    navigator_entitatum_reddere(
        datum->navigator,
        tabula,
        x,
        y,
        latitudo,
        altitudo,
        scala,
        focused);
}

hic_manens b32
_schirmata_navigator_tractare_eventum(
    Widget*           widget,
    constans Eventus* eventus)
{
    SchirmataNavigatorDatum* datum;

    datum = (SchirmataNavigatorDatum*)widget->datum;

    redde navigator_entitatum_tractare_eventum(datum->navigator, eventus);
}


/* ==================================================
 * Widget Wrapper Functiones - Arx Caeli
 * ================================================== */

/* Datum pro arx caeli widget wrapper */
nomen structura {
    ArcCaeli*  arx_caeli;
    Schirmata* schirmata;
} SchirmataArcCaeliDatum;

hic_manens vacuum
_schirmata_arx_caeli_reddere(
    Widget*          widget,
    TabulaPixelorum* tabula,
    i32              x,
    i32              y,
    i32              latitudo,
    i32              altitudo,
    i32              scala,
    b32              focused)
{
    SchirmataArcCaeliDatum* datum;

    datum = (SchirmataArcCaeliDatum*)widget->datum;

    arx_caeli_reddere(
        datum->arx_caeli,
        tabula,
        x,
        y,
        latitudo,
        altitudo,
        scala,
        focused);
}

hic_manens b32
_schirmata_arx_caeli_tractare_eventum(
    Widget*           widget,
    constans Eventus* eventus)
{
    SchirmataArcCaeliDatum* datum;

    datum = (SchirmataArcCaeliDatum*)widget->datum;

    redde arx_caeli_tractare_eventum(datum->arx_caeli, eventus);
}


/* ==================================================
 * Status Salvare / Restituere
 * ================================================== */

hic_manens vacuum
_salvare_status(
    Schirma*        schirma,
    LibroPaginarum* libro)
{
    Pagina* pagina;

    /* Salvare index paginae */
    schirma->libro_status.index_paginae = libro->index_currens;

    /* Salvare cursor et modo ex pagina currenti */
    pagina = libro_pagina_currens(libro);
    si (pagina)
    {
        schirma->libro_status.cursor_linea = pagina->vim.cursor_linea;
        schirma->libro_status.cursor_columna = pagina->vim.cursor_columna;
        schirma->libro_status.modo = pagina->vim.modo;
    }
}

hic_manens vacuum
_restituere_status(
    Schirma*        schirma,
    LibroPaginarum* libro)
{
    Pagina* pagina;

    /* Navigare ad pagina salvata */
    libro_navigare_ad(libro, (s32)schirma->libro_status.index_paginae);

    /* Restituere cursor et modo */
    pagina = libro_pagina_currens(libro);
    si (pagina)
    {
        pagina->vim.cursor_linea = schirma->libro_status.cursor_linea;
        pagina->vim.cursor_columna = schirma->libro_status.cursor_columna;
        pagina->vim.modo = schirma->libro_status.modo;
    }
}


/* ==================================================
 * Creare Layout pro Schirma
 * ================================================== */

hic_manens b32
_creare_schirma_layout(
    Schirmata* schirmata,
    i32        index)
{
    Schirma*                 schirma;
    ManagerWidget*           manager;
    SchirmataLibroDatum*     libro_datum;
    SchirmataNavigatorDatum* nav_datum;
    NavigatorEntitatum*      navigator;

    schirma = &schirmata->schirmae[index];

    /* Creare manager pro hac schirma */
    manager = manager_widget_creare(schirmata->piscina);
    si (!manager)
    {
        redde FALSUM;
    }
    schirma->manager = manager;

    /* Creare libro widget datum */
    libro_datum = piscina_allocare(schirmata->piscina, magnitudo(SchirmataLibroDatum));
    si (!libro_datum)
    {
        redde FALSUM;
    }
    libro_datum->libro = schirmata->libro;
    libro_datum->schirmata = schirmata;

    /* Registrare libro widget (sinistra medietas) */
    /* altitudo = 59 (reservare unum versum pro tabula schirmarum) */
    manager_widget_registrare(
        manager,
        libro_datum,
        _schirmata_libro_reddere,
        _schirmata_libro_tractare_eventum,
        ZEPHYRUM,   /* x */
        ZEPHYRUM,   /* y */
        LXXI,       /* latitudo = 71 */
        LIX);       /* altitudo = 59 */

    /* Creare navigator si repositorium disponibilis */
    si (schirmata->repo)
    {
        navigator = navigator_entitatum_creare(schirmata->piscina, schirmata->repo);
        si (navigator)
        {
            nav_datum = piscina_allocare(schirmata->piscina, magnitudo(SchirmataNavigatorDatum));
            si (nav_datum)
            {
                nav_datum->navigator = navigator;
                nav_datum->piscina = schirmata->piscina;

                /* Registrare navigator widget (dextra medietas) */
                /* altitudo = 59 (reservare unum versum pro tabula schirmarum) */
                manager_widget_registrare(
                    manager,
                    nav_datum,
                    _schirmata_navigator_reddere,
                    _schirmata_navigator_tractare_eventum,
                    LXXI,   /* x = 71 */
                    ZEPHYRUM,
                    LXXI,   /* latitudo = 71 */
                    LIX);   /* altitudo = 59 */
            }
        }
    }

    /* Initiare libro status */
    schirma->libro_status.index_paginae = ZEPHYRUM;
    schirma->libro_status.cursor_linea = ZEPHYRUM;
    schirma->libro_status.cursor_columna = ZEPHYRUM;
    schirma->libro_status.modo = MODO_VIM_NORMALIS;

    /* Creare Arx Caeli */
    schirma->arx_caeli = arx_caeli_creare(schirmata->piscina, schirmata->intern, schirmata->repo);
    schirma->modus_arx_caeli = FALSUM;  /* Initare in modus navigator */

    schirma->initiatus = VERUM;

    redde VERUM;
}


/* ==================================================
 * Tab Bar Rendering
 * ================================================== */

hic_manens vacuum
_reddere_tabulam_schirmarum(
    Schirmata*       schirmata,
    TabulaPixelorum* tabula,
    i32              scala)
{
    i32 character_latitudo;
    i32 character_altitudo;
    i32 tab_y;
    i32 tab_x;
    i32 i;
    character buffer[IV];
    chorda label;
    Color color_normal;
    Color color_activum;
    Color color_fondum;

    character_latitudo = VI * scala;
    character_altitudo = VIII * scala;

    /* Tab bar ad fundum fenestrae */
    tab_y = tabula->altitudo - character_altitudo;
    tab_x = II;

    color_normal = thema_color(COLOR_BORDER);
    color_activum = thema_color(COLOR_STATUS_INSERT);
    color_fondum = thema_color(COLOR_BACKGROUND);

    /* Pingere fondum tab bar */
    {
        i32 bar_width;

        bar_width = SCHIRMATA_MAXIMUS * IV * character_latitudo;  /* "[N] " pro singulo */

        per (i = ZEPHYRUM; i < character_altitudo; i++)
        {
            i32 px;
            per (px = ZEPHYRUM; px < bar_width; px++)
            {
                si (tab_y + i >= ZEPHYRUM && tab_y + i < tabula->altitudo &&
                    tab_x + px >= ZEPHYRUM && tab_x + px < tabula->latitudo)
                {
                    tabula->pixela[(tab_y + i) * tabula->latitudo + tab_x + px] =
                        color_ad_pixelum(color_fondum);
                }
            }
        }
    }

    /* Pingere tabs */
    per (i = ZEPHYRUM; i < SCHIRMATA_MAXIMUS; i++)
    {
        Color color;
        i32 num;

        /* Numerus: 1-9, 0 pro 10 */
        num = (i + I) % X;
        sprintf(buffer, "[%d]", num);
        label.datum = (i8*)buffer;
        label.mensura = III;

        color = (i == schirmata->index_currens) ? color_activum : color_normal;

        tabula_pixelorum_pingere_chordam(
            tabula,
            tab_x,
            tab_y,
            label,
            color_ad_pixelum(color));

        tab_x += IV * character_latitudo;  /* "[N] " = 4 characters */
    }
}


/* ==================================================
 * API Publica
 * ================================================== */

Schirmata*
schirmata_creare(
    Piscina*             piscina,
    InternamentumChorda* intern,
    LibroPaginarum*      libro,
    EntitasRepositorium* repo,
    RegistrumCommandi*   reg_commandi)
{
    Schirmata* schirmata;
    i32 i;

    si (!piscina || !intern || !libro)
    {
        redde NIHIL;
    }

    /* Allocare schirmata */
    schirmata = piscina_allocare(piscina, magnitudo(Schirmata));
    si (!schirmata)
    {
        redde NIHIL;
    }

    /* Initiare campos */
    schirmata->piscina = piscina;
    schirmata->intern = intern;
    schirmata->libro = libro;
    schirmata->repo = repo;
    schirmata->reg_commandi = reg_commandi;
    schirmata->index_currens = ZEPHYRUM;
    schirmata->praefixum_activum = FALSUM;
    schirmata->tempus_praefixum = 0.0;

    /* Initiare omnes schirmas */
    per (i = ZEPHYRUM; i < SCHIRMATA_MAXIMUS; i++)
    {
        schirmata->schirmae[i].manager = NIHIL;
        schirmata->schirmae[i].initiatus = FALSUM;
    }

    /* Creare layout pro omnibus schirmis */
    per (i = ZEPHYRUM; i < SCHIRMATA_MAXIMUS; i++)
    {
        si (!_creare_schirma_layout(schirmata, i))
        {
            redde NIHIL;
        }
    }

    redde schirmata;
}

vacuum
schirmata_commutare_ad(
    Schirmata* schirmata,
    i32        index)
{
    si (!schirmata)
    {
        redde;
    }

    /* Clamp index */
    si (index < ZEPHYRUM)
    {
        index = ZEPHYRUM;
    }
    si (index >= SCHIRMATA_MAXIMUS)
    {
        index = SCHIRMATA_MAXIMUS - I;
    }

    /* Si iam in hac schirma, nihil facere */
    si (index == schirmata->index_currens)
    {
        redde;
    }

    /* Salvare status schirmae currentis */
    _salvare_status(&schirmata->schirmae[schirmata->index_currens], schirmata->libro);

    /* Commutare */
    schirmata->index_currens = index;

    /* Restituere status novae schirmae */
    _restituere_status(&schirmata->schirmae[index], schirmata->libro);
}

vacuum
schirmata_proxima(
    Schirmata* schirmata)
{
    i32 nova_index;

    si (!schirmata)
    {
        redde;
    }

    nova_index = schirmata->index_currens + I;
    si (nova_index >= SCHIRMATA_MAXIMUS)
    {
        nova_index = ZEPHYRUM;
    }

    schirmata_commutare_ad(schirmata, nova_index);
}

vacuum
schirmata_prior(
    Schirmata* schirmata)
{
    i32 nova_index;

    si (!schirmata)
    {
        redde;
    }

    nova_index = schirmata->index_currens - I;
    si (nova_index < ZEPHYRUM)
    {
        nova_index = SCHIRMATA_MAXIMUS - I;
    }

    schirmata_commutare_ad(schirmata, nova_index);
}

b32
schirmata_tractare_eventum(
    Schirmata*        schirmata,
    constans Eventus* eventus)
{
    ManagerWidget* manager;

    si (!schirmata || !eventus)
    {
        redde FALSUM;
    }

    /* Tractare mouse click in tab bar */
    si (eventus->genus == EVENTUS_MUS_DEPRESSUS)
    {
        i32 click_y;
        i32 click_x;
        i32 tab_y_min;
        i32 character_altitudo;
        i32 character_latitudo;
        i32 fenestra_altitudo;

        character_altitudo = VIII;
        character_latitudo = VI;

        /* Fenestra altitudo in pixelis (hardcoded ut in probatio_combinado) */
        fenestra_altitudo = CDLXXX;  /* 480 */

        click_y = eventus->datum.mus.y;
        click_x = eventus->datum.mus.x;

        /* Tab bar est ad fundum fenestrae: altitudo - character_altitudo - 2 */
        tab_y_min = fenestra_altitudo - character_altitudo - II;

        si (click_y >= tab_y_min)
        {
            /* Click in tab bar area */
            i32 tab_index;

            tab_index = (click_x - II) / (IV * character_latitudo);
            si (tab_index >= ZEPHYRUM && tab_index < SCHIRMATA_MAXIMUS)
            {
                schirmata_commutare_ad(schirmata, tab_index);
                redde VERUM;
            }
        }
    }

    /* Si praefixum activum, tractare commandum schirmae */
    si (schirmata->praefixum_activum)
    {
        f64 tempus_currens;
        f64 tempus_elapsum;

        tempus_currens = tempus_nunc();
        tempus_elapsum = tempus_currens - schirmata->tempus_praefixum;

        /* Verificare timeout */
        si (tempus_elapsum > PRAEFIXUM_TIMEOUT)
        {
            /* Timeout - reset praefixum et passare eventum */
            schirmata->praefixum_activum = FALSUM;
        }
        alioquin
        {
            si (eventus->genus == EVENTUS_CLAVIS_DEPRESSUS)
            {
                s32 clavis;

                clavis = (s32)eventus->datum.clavis.clavis;

                /* n = proxima */
                si (clavis == 'n' || clavis == 'N')
                {
                    schirmata->praefixum_activum = FALSUM;
                    schirmata_proxima(schirmata);
                    redde VERUM;
                }
                /* p = prior */
                si (clavis == 'p' || clavis == 'P')
                {
                    schirmata->praefixum_activum = FALSUM;
                    schirmata_prior(schirmata);
                    redde VERUM;
                }
                /* 1-9 = schirma 0-8 */
                si (clavis >= '1' && clavis <= '9')
                {
                    schirmata->praefixum_activum = FALSUM;
                    schirmata_commutare_ad(schirmata, (i32)(clavis - '1'));
                    redde VERUM;
                }
                /* 0 = schirma 9 (decima) */
                si (clavis == '0')
                {
                    schirmata->praefixum_activum = FALSUM;
                    schirmata_commutare_ad(schirmata, IX);
                    redde VERUM;
                }
                /* ctrl-a iterum = passare ad widget */
                si ((clavis == 'a' || clavis == 'A') && (eventus->datum.clavis.modificantes & MOD_IMPERIUM))
                {
                    schirmata->praefixum_activum = FALSUM;
                    manager = schirmata->schirmae[schirmata->index_currens].manager;
                    redde manager_widget_tractare_eventum(manager, eventus);
                }

                /* Clavis non recognita - reset praefixum */
                schirmata->praefixum_activum = FALSUM;
            }

            /* Non-clavis eventus (e.g. key-up) - ignorare sed manere in praefixum modo */
            redde VERUM;
        }
    }

    /* Detegere ctrl-a (A quia convertere_clavem reddit maiusculas) */
    si (eventus->genus == EVENTUS_CLAVIS_DEPRESSUS)
    {
        si ((eventus->datum.clavis.clavis == 'a' || eventus->datum.clavis.clavis == 'A') &&
            (eventus->datum.clavis.modificantes & MOD_IMPERIUM))
        {
            schirmata->praefixum_activum = VERUM;
            schirmata->tempus_praefixum = tempus_nunc();
            redde VERUM;
        }
    }

    /* Delegare ad manager schirmae currentis */
    manager = schirmata->schirmae[schirmata->index_currens].manager;
    redde manager_widget_tractare_eventum(manager, eventus);
}

vacuum
schirmata_reddere(
    Schirmata*       schirmata,
    TabulaPixelorum* tabula,
    i32              scala)
{
    ManagerWidget* manager;

    si (!schirmata || !tabula)
    {
        redde;
    }

    /* Reddere widgets schirmae currentis */
    manager = schirmata->schirmae[schirmata->index_currens].manager;
    manager_widget_reddere(manager, tabula, scala);

    /* Reddere tab bar */
    _reddere_tabulam_schirmarum(schirmata, tabula, scala);
}

i32
schirmata_index_currens(
    Schirmata* schirmata)
{
    si (!schirmata)
    {
        redde ZEPHYRUM;
    }

    redde schirmata->index_currens;
}

ManagerWidget*
schirmata_manager_currens(
    Schirmata* schirmata)
{
    si (!schirmata)
    {
        redde NIHIL;
    }

    redde schirmata->schirmae[schirmata->index_currens].manager;
}


/* ==================================================
 * Mode Switching - Arx Caeli / Navigator
 * ================================================== */

vacuum
schirmata_commutare_ad_arx_caeli(
    Schirmata*          schirmata,
    constans character* slug)
{
    Schirma*                schirma;
    ManagerWidget*          manager;
    SchirmataArcCaeliDatum* arc_datum;

    si (!schirmata)
    {
        redde;
    }

    schirma = &schirmata->schirmae[schirmata->index_currens];

    si (schirma->modus_arx_caeli)
    {
        /* Iam in modus arx caeli - navigare ad slug */
        arx_caeli_navigare_ad(schirma->arx_caeli, slug);
        redde;
    }

    /* Commutare ex navigator ad arx caeli */
    manager = schirma->manager;

    /* Removere widget index 1 (navigator) et addere arx caeli */
    /* Nota: ManagerWidget non habet functionem removere, ergo recreare */

    /* Creare arx caeli datum */
    arc_datum = piscina_allocare(schirmata->piscina, magnitudo(SchirmataArcCaeliDatum));
    si (!arc_datum)
    {
        redde;
    }
    arc_datum->arx_caeli = schirma->arx_caeli;
    arc_datum->schirmata = schirmata;

    /* Substituere widget index 1 */
    si (manager->numerus_widgetorum > I)
    {
        manager->widgets[I].datum = arc_datum;
        manager->widgets[I].reddere = _schirmata_arx_caeli_reddere;
        manager->widgets[I].tractare_eventum = _schirmata_arx_caeli_tractare_eventum;
    }

    schirma->modus_arx_caeli = VERUM;

    /* Navigare ad slug */
    arx_caeli_navigare_ad(schirma->arx_caeli, slug);
}

vacuum
schirmata_commutare_ad_navigator(
    Schirmata* schirmata)
{
    Schirma*                 schirma;
    ManagerWidget*           manager;
    SchirmataNavigatorDatum* nav_datum;
    NavigatorEntitatum*      navigator;

    si (!schirmata)
    {
        redde;
    }

    schirma = &schirmata->schirmae[schirmata->index_currens];

    si (!schirma->modus_arx_caeli)
    {
        /* Iam in modus navigator */
        redde;
    }

    /* Commutare ex arx caeli ad navigator */
    manager = schirma->manager;

    /* Creare navigator si non existit */
    navigator = navigator_entitatum_creare(schirmata->piscina, schirmata->repo);
    si (!navigator)
    {
        redde;
    }

    nav_datum = piscina_allocare(schirmata->piscina, magnitudo(SchirmataNavigatorDatum));
    si (!nav_datum)
    {
        redde;
    }
    nav_datum->navigator = navigator;
    nav_datum->piscina = schirmata->piscina;

    /* Substituere widget index 1 */
    si (manager->numerus_widgetorum > I)
    {
        manager->widgets[I].datum = nav_datum;
        manager->widgets[I].reddere = _schirmata_navigator_reddere;
        manager->widgets[I].tractare_eventum = _schirmata_navigator_tractare_eventum;
    }

    schirma->modus_arx_caeli = FALSUM;
}
