#include "layout.h"
#include "stml.h"
#include <string.h>
#include <stdlib.h>

/* ==================================================
 * Datum Structurae pro Widgets
 * ================================================== */

/* Datum internum pro pagina widget */
nomen structura {
    Pagina*  pagina;
    Piscina* piscina;
} LayoutDatumPagina;

/* Datum internum pro navigator widget */
nomen structura {
    NavigatorEntitatum* navigator;
    Piscina*            piscina;
} LayoutDatumNavigator;


/* ==================================================
 * Widget Wrapper Functiones - Pagina
 * ================================================== */

interior vacuum
_layout_pagina_reddere(
    Widget*          widget,
    TabulaPixelorum* tabula,
    i32              x,
    i32              y,
    i32              latitudo,
    i32              altitudo,
    i32              scala,
    b32              focused)
{
    LayoutDatumPagina* datum;

    datum = (LayoutDatumPagina*)widget->datum;

    pagina_reddere_cum_margine(
        datum->piscina,
        tabula,
        datum->pagina,
        x,
        y,
        latitudo,
        altitudo,
        scala,
        focused);
}

interior b32
_layout_pagina_tractare_eventum(
    Widget*           widget,
    constans Eventus* eventus)
{
    LayoutDatumPagina* datum;

    datum = (LayoutDatumPagina*)widget->datum;

    redde pagina_tractare_eventum(datum->pagina, eventus);
}


/* ==================================================
 * Widget Wrapper Functiones - Navigator
 * ================================================== */

interior vacuum
_layout_navigator_reddere(
    Widget*          widget,
    TabulaPixelorum* tabula,
    i32              x,
    i32              y,
    i32              latitudo,
    i32              altitudo,
    i32              scala,
    b32              focused)
{
    LayoutDatumNavigator* datum;

    datum = (LayoutDatumNavigator*)widget->datum;

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

interior b32
_layout_navigator_tractare_eventum(
    Widget*           widget,
    constans Eventus* eventus)
{
    LayoutDatumNavigator* datum;

    datum = (LayoutDatumNavigator*)widget->datum;

    redde navigator_entitatum_tractare_eventum(datum->navigator, eventus);
}


/* ==================================================
 * Auxiliares
 * ================================================== */

/* Legere attributum integer ex nodo STML */
interior i32
_layout_attributum_i32(
    StmlNodus*          nodus,
    constans character* titulus,
    i32                 defaltum)
{
    chorda* valor;
    i32     fructus;

    valor = stml_attributum_capere(nodus, titulus);
    si (!valor || !valor->datum || valor->mensura == ZEPHYRUM)
    {
        redde defaltum;
    }

    /* Usare chorda_ut_i32 quod tractat non-nulla-terminatas chordas */
    si (chorda_ut_i32(*valor, &fructus))
    {
        redde fructus;
    }

    redde defaltum;
}

/* Legere attributum chorda ex nodo STML ut C string (nulla-terminata)
 * NOTA: Allocat memoriam ex piscina */
interior constans character*
_layout_attributum_chorda(
    StmlNodus*          nodus,
    constans character* titulus,
    Piscina*            piscina)
{
    chorda* valor;

    valor = stml_attributum_capere(nodus, titulus);
    si (!valor || !valor->datum)
    {
        redde NIHIL;
    }

    /* Convertere ad C string nulla-terminatam */
    redde chorda_ut_cstr(*valor, piscina);
}


/* ==================================================
 * Processare Tags
 * ================================================== */

interior b32
_layout_processare_pagina(
    LayoutDom* dom,
    StmlNodus* nodus)
{
    constans character*   id;
    i32                   x, y, latitudo, altitudo;
    Pagina*               pagina;
    LayoutDatumPagina*    datum;
    LayoutWidgetIntroitus* introitus;
    chorda*               id_chorda;

    /* Legere attributa */
    id = _layout_attributum_chorda(nodus, "id", dom->piscina);
    si (!id)
    {
        id = "pagina";  /* Defaltum */
    }

    x = _layout_attributum_i32(nodus, "x", ZEPHYRUM);
    y = _layout_attributum_i32(nodus, "y", ZEPHYRUM);
    latitudo = _layout_attributum_i32(nodus, "latitudo", LXX);
    altitudo = _layout_attributum_i32(nodus, "altitudo", LV);

    /* Allocare et initiare pagina */
    pagina = piscina_allocare(dom->piscina, magnitudo(Pagina));
    si (!pagina)
    {
        redde FALSUM;
    }
    pagina_initiare(pagina, id);

    /* Creare datum wrapper */
    datum = piscina_allocare(dom->piscina, magnitudo(LayoutDatumPagina));
    si (!datum)
    {
        redde FALSUM;
    }
    datum->pagina = pagina;
    datum->piscina = dom->piscina;

    /* Registrare cum manager */
    manager_widget_registrare(
        dom->manager,
        datum,
        _layout_pagina_reddere,
        _layout_pagina_tractare_eventum,
        x,
        y,
        latitudo,
        altitudo);

    /* Addere ad tabula lookup */
    introitus = piscina_allocare(dom->piscina, magnitudo(LayoutWidgetIntroitus));
    si (!introitus)
    {
        redde FALSUM;
    }
    introitus->datum = pagina;
    introitus->genus = LAYOUT_WIDGET_PAGINA;

    id_chorda = chorda_internare_ex_literis(dom->intern, id);
    tabula_dispersa_inserere(dom->widgets, *id_chorda, introitus);

    redde VERUM;
}

interior b32
_layout_processare_navigator(
    LayoutDom*           dom,
    StmlNodus*           nodus,
    EntitasRepositorium* repositorium)
{
    constans character*    id;
    i32                    x, y, latitudo, altitudo;
    NavigatorEntitatum*    navigator;
    LayoutDatumNavigator*  datum;
    LayoutWidgetIntroitus* introitus;
    chorda*                id_chorda;

    si (!repositorium)
    {
        /* Navigator requirit repositorium */
        redde FALSUM;
    }

    /* Legere attributa */
    id = _layout_attributum_chorda(nodus, "id", dom->piscina);
    si (!id)
    {
        id = "navigator";  /* Defaltum */
    }

    x = _layout_attributum_i32(nodus, "x", ZEPHYRUM);
    y = _layout_attributum_i32(nodus, "y", ZEPHYRUM);
    latitudo = _layout_attributum_i32(nodus, "latitudo", LXX);
    altitudo = _layout_attributum_i32(nodus, "altitudo", LV);

    /* Creare navigator */
    navigator = navigator_entitatum_creare(dom->piscina, repositorium);
    si (!navigator)
    {
        redde FALSUM;
    }

    /* Creare datum wrapper */
    datum = piscina_allocare(dom->piscina, magnitudo(LayoutDatumNavigator));
    si (!datum)
    {
        redde FALSUM;
    }
    datum->navigator = navigator;
    datum->piscina = dom->piscina;

    /* Registrare cum manager */
    manager_widget_registrare(
        dom->manager,
        datum,
        _layout_navigator_reddere,
        _layout_navigator_tractare_eventum,
        x,
        y,
        latitudo,
        altitudo);

    /* Addere ad tabula lookup */
    introitus = piscina_allocare(dom->piscina, magnitudo(LayoutWidgetIntroitus));
    si (!introitus)
    {
        redde FALSUM;
    }
    introitus->datum = navigator;
    introitus->genus = LAYOUT_WIDGET_NAVIGATOR;

    id_chorda = chorda_internare_ex_literis(dom->intern, id);
    tabula_dispersa_inserere(dom->widgets, *id_chorda, introitus);

    redde VERUM;
}


/* ==================================================
 * API Publica
 * ================================================== */

LayoutDom*
layout_creare(
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  stml,
    EntitasRepositorium* repositorium)
{
    LayoutDom*   dom;
    StmlResultus res;
    StmlNodus*   layout_nodus;
    StmlNodus*   liberum;
    i32          i;
    i32          num_liberi;

    si (!piscina || !intern || !stml)
    {
        redde NIHIL;
    }

    /* Allocare DOM */
    dom = piscina_allocare(piscina, magnitudo(LayoutDom));
    si (!dom)
    {
        redde NIHIL;
    }

    dom->piscina = piscina;
    dom->intern = intern;

    /* Creare manager */
    dom->manager = manager_widget_creare(piscina);
    si (!dom->manager)
    {
        redde NIHIL;
    }

    /* Creare tabula lookup */
    dom->widgets = tabula_dispersa_creare_chorda(piscina, XXXII);
    si (!dom->widgets)
    {
        redde NIHIL;
    }

    /* Parsare STML */
    res = stml_legere_ex_literis(stml, piscina, intern);
    si (!res.successus || !res.elementum_radix)
    {
        redde NIHIL;
    }

    /* Invenire <layout> elementum */
    layout_nodus = res.elementum_radix;
    si (!layout_nodus->titulus ||
        !chorda_aequalis_literis(*layout_nodus->titulus, "layout"))
    {
        /* Radix non est <layout> */
        redde NIHIL;
    }

    /* Processare liberos */
    num_liberi = stml_numerus_liberorum(layout_nodus);
    per (i = ZEPHYRUM; i < num_liberi; i++)
    {
        liberum = stml_liberum_ad_indicem(layout_nodus, i);
        si (!liberum || liberum->genus != STML_NODUS_ELEMENTUM || !liberum->titulus)
        {
            perge;
        }

        si (chorda_aequalis_literis(*liberum->titulus, "pagina"))
        {
            _layout_processare_pagina(dom, liberum);
        }
        alioquin si (chorda_aequalis_literis(*liberum->titulus, "navigator"))
        {
            _layout_processare_navigator(dom, liberum, repositorium);
        }
        /* Alii tags ignorantur pro nunc */
    }

    redde dom;
}

vacuum*
layout_obtinere(
    LayoutDom*          dom,
    constans character* id)
{
    LayoutWidgetIntroitus* introitus;
    b32                    inventum;

    si (!dom || !id)
    {
        redde NIHIL;
    }

    inventum = tabula_dispersa_invenire_literis(dom->widgets, id, (vacuum**)&introitus);

    si (!inventum || !introitus)
    {
        redde NIHIL;
    }

    redde introitus->datum;
}

Pagina*
layout_obtinere_pagina(
    LayoutDom*          dom,
    constans character* id)
{
    LayoutWidgetIntroitus* introitus;
    b32                    inventum;

    si (!dom || !id)
    {
        redde NIHIL;
    }

    inventum = tabula_dispersa_invenire_literis(dom->widgets, id, (vacuum**)&introitus);

    si (!inventum || !introitus || introitus->genus != LAYOUT_WIDGET_PAGINA)
    {
        redde NIHIL;
    }

    redde (Pagina*)introitus->datum;
}

NavigatorEntitatum*
layout_obtinere_navigator(
    LayoutDom*          dom,
    constans character* id)
{
    LayoutWidgetIntroitus* introitus;
    b32                    inventum;

    si (!dom || !id)
    {
        redde NIHIL;
    }

    inventum = tabula_dispersa_invenire_literis(dom->widgets, id, (vacuum**)&introitus);

    si (!inventum || !introitus || introitus->genus != LAYOUT_WIDGET_NAVIGATOR)
    {
        redde NIHIL;
    }

    redde (NavigatorEntitatum*)introitus->datum;
}
