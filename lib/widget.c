#include "widget.h"

/* ==================================================
 * Manager - Creatio
 * ================================================== */

ManagerWidget*
manager_widget_creare(
    Piscina* piscina)
{
    ManagerWidget* manager;
    s32 i;

    manager = piscina_allocare(piscina, magnitudo(ManagerWidget));
    si (!manager)
    {
        redde NIHIL;
    }

    manager->piscina = piscina;
    manager->numerus_widgetorum = ZEPHYRUM;
    manager->focus_index = -I;  /* Nullus focus initio */

    /* Initiare widgets */
    per (i = ZEPHYRUM; i < XVI; i++)
    {
        manager->widgets[i].datum = NIHIL;
        manager->widgets[i].reddere = NIHIL;
        manager->widgets[i].tractare_eventum = NIHIL;
        manager->widgets[i].x = ZEPHYRUM;
        manager->widgets[i].y = ZEPHYRUM;
        manager->widgets[i].latitudo = ZEPHYRUM;
        manager->widgets[i].altitudo = ZEPHYRUM;
    }

    redde manager;
}


/* ==================================================
 * Manager - Registratio Widgetorum
 * ================================================== */

s32
manager_widget_registrare(
    ManagerWidget*         manager,
    vacuum*                datum,
    FunctioReddere         reddere,
    FunctioTractareEventum tractare_eventum,
    i32                    x,
    i32                    y,
    i32                    latitudo,
    i32                    altitudo)
{
    Widget* widget;
    s32 index;

    si (!manager || manager->numerus_widgetorum >= XVI)
    {
        redde -I;  /* Capacitas plena */
    }

    index = manager->numerus_widgetorum;
    widget = &manager->widgets[index];

    widget->datum = datum;
    widget->reddere = reddere;
    widget->tractare_eventum = tractare_eventum;
    widget->x = x;
    widget->y = y;
    widget->latitudo = latitudo;
    widget->altitudo = altitudo;

    manager->numerus_widgetorum++;

    /* Si primus widget, damus ei focus */
    si (manager->focus_index == -I)
    {
        manager->focus_index = index;
    }

    redde index;
}


/* ==================================================
 * Manager - Focus Management
 * ================================================== */

vacuum
manager_widget_ponere_focus(
    ManagerWidget* manager,
    s32            index)
{
    si (!manager || index < ZEPHYRUM || index >= manager->numerus_widgetorum)
    {
        redde;
    }

    manager->focus_index = index;
}

vacuum
manager_widget_focus_proximum(
    ManagerWidget* manager)
{
    si (!manager || manager->numerus_widgetorum == ZEPHYRUM)
    {
        redde;
    }

    manager->focus_index++;
    si (manager->focus_index >= manager->numerus_widgetorum)
    {
        manager->focus_index = ZEPHYRUM;  /* Cyclus ad initium */
    }
}

vacuum
manager_widget_focus_praecedens(
    ManagerWidget* manager)
{
    si (!manager || manager->numerus_widgetorum == ZEPHYRUM)
    {
        redde;
    }

    manager->focus_index--;
    si (manager->focus_index < ZEPHYRUM)
    {
        manager->focus_index = manager->numerus_widgetorum - I;  /* Cyclus ad finem */
    }
}

b32
manager_widget_focus_ad_punctum(
    ManagerWidget* manager,
    i32            x,
    i32            y)
{
    s32 i;
    Widget* widget;

    si (!manager)
    {
        redde FALSUM;
    }

    /* Quaerere widget ad coordinatas */
    per (i = ZEPHYRUM; i < manager->numerus_widgetorum; i++)
    {
        widget = &manager->widgets[i];

        /* Verificare si punctum intra limites widget */
        si (x >= widget->x && x < widget->x + widget->latitudo &&
            y >= widget->y && y < widget->y + widget->altitudo)
        {
            manager->focus_index = i;
            redde VERUM;
        }
    }

    redde FALSUM;  /* Nullus widget inventus */
}


/* ==================================================
 * Manager - Event Handling
 * ================================================== */

b32
manager_widget_tractare_eventum(
    ManagerWidget*    manager,
    constans Eventus* eventus)
{
    Widget* focused_widget;
    b32 consumptus;

    si (!manager || !eventus)
    {
        redde FALSUM;
    }

    /* Click-to-focus */
    si (eventus->genus == EVENTUS_MUS_DEPRESSUS)
    {
        i32 character_latitudo;
        i32 click_x_char;
        i32 click_y_char;

        character_latitudo = VI;  /* 6 pixels per character (scala = 1) */
        click_x_char = eventus->datum.mus.x / character_latitudo;
        click_y_char = eventus->datum.mus.y / (VIII);  /* 8 pixels per character height */

        manager_widget_focus_ad_punctum(manager, click_x_char, click_y_char);
        redde VERUM;  /* Consumere click eventum */
    }

    /* Passare eventum ad widget cum focus */
    si (manager->focus_index >= ZEPHYRUM &&
        manager->focus_index < manager->numerus_widgetorum)
    {
        focused_widget = &manager->widgets[manager->focus_index];

        si (focused_widget->tractare_eventum)
        {
            consumptus = focused_widget->tractare_eventum(focused_widget, eventus);

            si (consumptus)
            {
                redde VERUM;  /* Widget tractavit eventum */
            }
        }
    }

    /* Si widget non consumpsit, manager tractat TAB pro focus cycling */
    si (eventus->genus == EVENTUS_CLAVIS_DEPRESSUS)
    {
        si (eventus->datum.clavis.clavis == CLAVIS_TABULA)
        {
            manager_widget_focus_proximum(manager);
            redde VERUM;
        }
    }

    redde FALSUM;
}


/* ==================================================
 * Manager - Rendering
 * ================================================== */

vacuum
manager_widget_reddere(
    ManagerWidget*   manager,
    TabulaPixelorum* tabula,
    i32              scala)
{
    s32 i;
    Widget* widget;
    b32 focused;

    si (!manager || !tabula)
    {
        redde;
    }

    /* Reddere omnes widgets */
    per (i = ZEPHYRUM; i < manager->numerus_widgetorum; i++)
    {
        widget = &manager->widgets[i];
        focused = (i == manager->focus_index);

        si (widget->reddere)
        {
            widget->reddere(
                widget,
                tabula,
                widget->x,
                widget->y,
                widget->latitudo,
                widget->altitudo,
                scala,
                focused);
        }
    }
}
