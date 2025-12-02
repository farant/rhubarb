#include "layout.h"
#include "stml.h"
#include "registrum_commandi.h"
#include "coloratio.h"
#include "xar.h"
#include "entitas.h"
#include <string.h>
#include <stdlib.h>

/* ==================================================
 * Datum Structurae pro Widgets et Entitates
 * ================================================== */

/* Datum internum pro pagina widget */
nomen structura {
    Pagina*    pagina;
    LayoutDom* dom;  /* Pro accedere registrum et piscina */
} LayoutDatumPagina;

/* Relatio differens - colligitur in prima passu, resolvitur in secunda */
nomen structura {
    Entitas* ab_entitas;       /* Entitas originis */
    chorda*  genus_relationis; /* Genus relationis e.g. "pages" */
    chorda*  ad_referentia;    /* Referentia destinationis e.g. "Page::introduction" */
} LayoutRelatioDifferens;

/* Datum internum pro navigator widget */
nomen structura {
    NavigatorEntitatum* navigator;
    Piscina*            piscina;
} LayoutDatumNavigator;

/* Datum internum pro libro widget */
nomen structura {
    LibroPaginarum* libro;
    LayoutDom*      dom;
} LayoutDatumLibro;


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
        datum->dom->piscina,
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

    /* Tractare mouse clicks pro tag detection */
    si (eventus->genus == EVENTUS_MUS_DEPRESSUS && datum->dom->reg_commandi)
    {
        RegioClicca regio;
        i32 click_x;
        i32 click_y;
        i32 character_latitudo;
        i32 character_altitudo;

        character_latitudo = VI;   /* 6 pixels per character */
        character_altitudo = VIII; /* 8 pixels per character */

        /* Convertere pixel ad character coordinates */
        /* Account for widget position et border */
        click_x = (eventus->datum.mus.x / character_latitudo) - widget->x - I;
        click_y = (eventus->datum.mus.y / character_altitudo) - widget->y - I;

        /* Tentare detegere tag ad click position */
        /* click_y = linea, click_x = columna */
        si (pagina_obtinere_regio_ad_punctum(datum->pagina, click_y, click_x, &regio))
        {
            si (strcmp(regio.genus, "command") == ZEPHYRUM)
            {
                ContextusCommandi ctx;

                ctx.pagina = datum->pagina;
                ctx.linea = regio.finis_linea;
                ctx.columna = regio.finis_columna;
                ctx.piscina = datum->dom->piscina;
                ctx.datum_custom = NIHIL;

                /* Executare command per reg_commandi */
                registrum_commandi_executare(datum->dom->reg_commandi, regio.datum, &ctx);

                redde VERUM;  /* Click consumptus */
            }
        }
    }

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
 * Widget Wrapper Functiones - Libro
 * ================================================== */

interior vacuum
_layout_libro_reddere(
    Widget*          widget,
    TabulaPixelorum* tabula,
    i32              x,
    i32              y,
    i32              latitudo,
    i32              altitudo,
    i32              scala,
    b32              focused)
{
    LayoutDatumLibro* datum;

    datum = (LayoutDatumLibro*)widget->datum;

    libro_reddere(
        datum->dom->piscina,
        tabula,
        datum->libro,
        x,
        y,
        latitudo,
        altitudo,
        scala,
        focused);
}

interior b32
_layout_libro_tractare_eventum(
    Widget*           widget,
    constans Eventus* eventus)
{
    LayoutDatumLibro* datum;
    Pagina* pagina;

    datum = (LayoutDatumLibro*)widget->datum;

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
    si (eventus->genus == EVENTUS_MUS_DEPRESSUS && datum->dom->reg_commandi)
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
        /* Account for widget position et border */
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
                ctx.piscina = datum->dom->piscina;
                ctx.datum_custom = datum->libro;  /* Pass libro as custom datum */

                /* Executare command per reg_commandi */
                registrum_commandi_executare(datum->dom->reg_commandi, regio.datum, &ctx);

                redde VERUM;  /* Click consumptus */
            }
            alioquin si (strcmp(regio.genus, "link") == ZEPHYRUM)
            {
                /* Tractare link navigation */
                constans character* link = regio.datum;

                si (link[ZEPHYRUM] == '#')
                {
                    /* Hash link */
                    link++;  /* Skip '#' */

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
                        /* Numeric page - user expects 1-indexed */
                        s32 page_num = atoi(link) - I;
                        libro_navigare_ad(datum->libro, page_num);
                    }
                    alioquin
                    {
                        /* Named page */
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

/* ==================================================
 * Processare Tags
 * ================================================== */

interior b32
_layout_processare_pagina(
    LayoutDom* dom,
    StmlNodus* nodus)
{
    chorda*               id_chorda;
    i32                   x, y, latitudo, altitudo;
    Pagina*               pagina;
    LayoutDatumPagina*    datum;
    LayoutWidgetIntroitus* introitus;

    /* Legere attributa - id est chorda* ex STML (iam internata) */
    id_chorda = stml_attributum_capere(nodus, "id");
    si (!id_chorda)
    {
        id_chorda = chorda_internare_ex_literis(dom->intern, "pagina");
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
    /* Tabula dimensiones = widget dimensiones - border (II pro utroque latere) */
    pagina_initiare_cum_dimensionibus(pagina, dom->piscina, latitudo - II, altitudo - II, id_chorda);

    /* Si nodus habet contentum (raw vel liberi), inserere in pagina */
    si (nodus->crudus || stml_numerus_liberorum(nodus) > ZEPHYRUM)
    {
        chorda textus = stml_textus_internus(nodus, dom->piscina);
        si (textus.mensura > ZEPHYRUM)
        {
            /* Pagina_inserere_textum requirit null-terminated */
            character* textus_nt = piscina_allocare(dom->piscina,
                (memoriae_index)(textus.mensura + I));
            si (textus_nt)
            {
                memcpy(textus_nt, textus.datum, (size_t)textus.mensura);
                textus_nt[textus.mensura] = '\0';
                pagina_inserere_textum_crudus(pagina, textus_nt);
            }
        }
    }

    /* Creare datum wrapper */
    datum = piscina_allocare(dom->piscina, magnitudo(LayoutDatumPagina));
    si (!datum)
    {
        redde FALSUM;
    }
    datum->pagina = pagina;
    datum->dom = dom;

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

    tabula_dispersa_inserere(dom->widgets, *id_chorda, introitus);

    redde VERUM;
}

interior b32
_layout_processare_libro(
    LayoutDom*           dom,
    StmlNodus*           nodus,
    EntitasRepositorium* repositorium)
{
    chorda*                id_chorda;
    i32                    x, y, latitudo, altitudo;
    LibroPaginarum*        libro;
    LayoutDatumLibro*      datum;
    LayoutWidgetIntroitus* introitus;

    /* Legere attributa - id est chorda* ex STML (iam internata) */
    id_chorda = stml_attributum_capere(nodus, "id");
    si (!id_chorda)
    {
        id_chorda = chorda_internare_ex_literis(dom->intern, "libro");
    }

    x = _layout_attributum_i32(nodus, "x", ZEPHYRUM);
    y = _layout_attributum_i32(nodus, "y", ZEPHYRUM);
    latitudo = _layout_attributum_i32(nodus, "latitudo", LXX);
    altitudo = _layout_attributum_i32(nodus, "altitudo", LV);

    /* Creare libro */
    libro = libro_creare(dom->piscina, dom->intern);
    si (!libro)
    {
        redde FALSUM;
    }

    /* Connectere repositorium si disponibilis (pro persistentia) */
    si (repositorium)
    {
        libro_connectere_repo(libro, repositorium);
        /* Carcare paginas existentes */
        libro_carcare(libro);
    }

    /* Ponere nomen initiale si attributum "nomen" existit */
    {
        chorda* nomen_chorda;
        nomen_chorda = stml_attributum_capere(nodus, "nomen");
        si (nomen_chorda && nomen_chorda->mensura > ZEPHYRUM)
        {
            /* Creare null-terminated string */
            character* nomen_nt = piscina_allocare(dom->piscina,
                (memoriae_index)(nomen_chorda->mensura + I));
            si (nomen_nt)
            {
                memcpy(nomen_nt, nomen_chorda->datum, (size_t)nomen_chorda->mensura);
                nomen_nt[nomen_chorda->mensura] = '\0';
                libro_pagina_nominare(libro, ZEPHYRUM, nomen_nt);
            }
        }
    }

    /* Si nodus habet contentum (raw vel liberi), inserere in prima pagina
     * SOLUM si nihil carcatum ex repositorio (entitas_ids[0] == NIHIL) */
    si ((nodus->crudus || stml_numerus_liberorum(nodus) > ZEPHYRUM) &&
        libro->entitas_ids[ZEPHYRUM] == NIHIL)
    {
        Pagina* pagina;
        chorda textus;

        textus = stml_textus_internus(nodus, dom->piscina);
        si (textus.mensura > ZEPHYRUM)
        {
            pagina = libro_pagina_currens(libro);
            si (pagina)
            {
                /* Pagina_inserere_textum requirit null-terminated */
                character* textus_nt = piscina_allocare(dom->piscina,
                    (memoriae_index)(textus.mensura + I));
                si (textus_nt)
                {
                    memcpy(textus_nt, textus.datum, (size_t)textus.mensura);
                    textus_nt[textus.mensura] = '\0';
                    pagina_inserere_textum_crudus(pagina, textus_nt);

                    /* Salvare statim post contentum initiale */
                    libro_salvare_paginam(libro);
                }
            }
        }
    }

    /* Creare datum wrapper */
    datum = piscina_allocare(dom->piscina, magnitudo(LayoutDatumLibro));
    si (!datum)
    {
        redde FALSUM;
    }
    datum->libro = libro;
    datum->dom = dom;

    /* Registrare cum manager */
    manager_widget_registrare(
        dom->manager,
        datum,
        _layout_libro_reddere,
        _layout_libro_tractare_eventum,
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
    introitus->datum = libro;
    introitus->genus = LAYOUT_WIDGET_LIBRO;

    tabula_dispersa_inserere(dom->widgets, *id_chorda, introitus);

    redde VERUM;
}

interior b32
_layout_processare_navigator(
    LayoutDom*           dom,
    StmlNodus*           nodus,
    EntitasRepositorium* repositorium)
{
    chorda*                id_chorda;
    i32                    x, y, latitudo, altitudo;
    NavigatorEntitatum*    navigator;
    LayoutDatumNavigator*  datum;
    LayoutWidgetIntroitus* introitus;

    si (!repositorium)
    {
        /* Navigator requirit repositorium */
        redde FALSUM;
    }

    /* Legere attributa - id est chorda* ex STML (iam internata) */
    id_chorda = stml_attributum_capere(nodus, "id");
    si (!id_chorda)
    {
        id_chorda = chorda_internare_ex_literis(dom->intern, "navigator");
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

    tabula_dispersa_inserere(dom->widgets, *id_chorda, introitus);

    redde VERUM;
}

/* ==================================================
 * Schema Creation from typus/literalis Attributes
 * ================================================== */

/* Creare vel verificare schema pro proprietas
 *
 * Processus:
 * 1. Scaffoldare TypusSemanticus::typus cum typus_literalis
 * 2. Invenire vel creare ProprietasDefinitio pro (entitas_genus, clavis)
 * 3. Verificare non conflictus
 * 4. Linkare via "est" relatio
 *
 * Redde: VERUM si successus, FALSUM si conflictus (error entity creata)
 */
interior b32
_layout_processare_schema_proprietatis(
    LayoutDom*           dom,
    EntitasRepositorium* repositorium,
    chorda*              entitas_genus,
    chorda*              clavis,
    chorda*              typus,
    chorda*              literalis)
{
    Entitas*  typus_sem;
    Entitas*  prop_def;
    chorda*   literalis_existens;
    chorda*   literalis_default;
    chorda*   est_genus;
    chorda*   clavis_intern;
    chorda*   typus_literalis_clavis;
    Relatio*  rel;
    Entitas*  typus_sem_existens;
    i32       i;
    i32       num_rel;

    /* Literalis default est "chorda" */
    literalis_default = chorda_internare_ex_literis(dom->intern, "chorda");
    si (!literalis)
    {
        literalis = literalis_default;
    }

    /* Scaffoldare TypusSemanticus::typus */
    typus_sem = repositorium->entitas_scaffoldare(
        repositorium->datum,
        "TypusSemanticus",
        chorda_ut_cstr(*typus, dom->piscina));

    si (!typus_sem)
    {
        redde FALSUM;
    }

    /* Verificare/ponere typus_literalis */
    typus_literalis_clavis = chorda_internare_ex_literis(dom->intern, "typus_literalis");
    literalis_existens = entitas_proprietas_capere(typus_sem, typus_literalis_clavis);

    si (literalis_existens)
    {
        /* Verificare non conflictus */
        si (literalis_existens != literalis &&
            !chorda_aequalis(*literalis_existens, *literalis))
        {
            /* Conflictus! Creare error entity */
            Entitas* error;

            error = repositorium->entitas_creare(repositorium->datum, "SchemaConflictus");
            si (error)
            {
                repositorium->proprietas_ponere(repositorium->datum, error,
                    "genus_conflictus", "TypusSemanticus");
                repositorium->proprietas_ponere(repositorium->datum, error,
                    "typus_semanticus", chorda_ut_cstr(*typus, dom->piscina));
                repositorium->proprietas_ponere(repositorium->datum, error,
                    "literalis_existens", chorda_ut_cstr(*literalis_existens, dom->piscina));
                repositorium->proprietas_ponere(repositorium->datum, error,
                    "literalis_novus", chorda_ut_cstr(*literalis, dom->piscina));
            }
            redde FALSUM;
        }
    }
    alioquin
    {
        /* Ponere typus_literalis et name */
        repositorium->proprietas_ponere(repositorium->datum, typus_sem,
            "typus_literalis", chorda_ut_cstr(*literalis, dom->piscina));
        repositorium->proprietas_ponere(repositorium->datum, typus_sem,
            "name", chorda_ut_cstr(*typus, dom->piscina));
    }

    /* Invenire ProprietasDefinitio existens */
    clavis_intern = chorda_internare_ex_literis(dom->intern,
        chorda_ut_cstr(*clavis, dom->piscina));
    prop_def = entitas_repositorium_proprietas_definitio_invenire(
        repositorium,
        entitas_genus,
        clavis_intern);

    si (prop_def)
    {
        /* Verificare "est" relatio punctat ad idem TypusSemanticus
         * (non ad Genus - omnes entitates habent "est" ad suum Genus)
         */
        est_genus = chorda_internare_ex_literis(dom->intern, "est");
        typus_sem_existens = NIHIL;

        num_rel = xar_numerus(prop_def->relationes);
        per (i = ZEPHYRUM; i < num_rel; i++)
        {
            Entitas* dest_ent;

            rel = (Relatio*)xar_obtinere(prop_def->relationes, i);
            si (rel && rel->genus == est_genus)
            {
                dest_ent = repositorium->capere_entitatem(
                    repositorium->datum, rel->destinatio_id);
                /* Verificare destinatio est TypusSemanticus, non Genus */
                si (dest_ent && dest_ent->genus &&
                    chorda_aequalis_literis(*dest_ent->genus, "TypusSemanticus"))
                {
                    typus_sem_existens = dest_ent;
                    frange;
                }
            }
        }

        si (typus_sem_existens && typus_sem_existens != typus_sem)
        {
            /* Conflictus! ProprietasDefinitio iam habet alium typum */
            Entitas* error;

            error = repositorium->entitas_creare(repositorium->datum, "SchemaConflictus");
            si (error)
            {
                repositorium->proprietas_ponere(repositorium->datum, error,
                    "genus_conflictus", "ProprietasDefinitio");
                repositorium->proprietas_ponere(repositorium->datum, error,
                    "entitas_genus", chorda_ut_cstr(*entitas_genus, dom->piscina));
                repositorium->proprietas_ponere(repositorium->datum, error,
                    "name", chorda_ut_cstr(*clavis, dom->piscina));
                repositorium->proprietas_ponere(repositorium->datum, error,
                    "typus_existens", chorda_ut_cstr(
                        *entitas_titulum_capere(typus_sem_existens), dom->piscina));
                repositorium->proprietas_ponere(repositorium->datum, error,
                    "typus_novus", chorda_ut_cstr(*typus, dom->piscina));
            }
            redde FALSUM;
        }
    }
    alioquin
    {
        Entitas* genus_ent;

        /* Creare novam ProprietasDefinitio */
        prop_def = repositorium->entitas_creare(repositorium->datum, "ProprietasDefinitio");
        si (!prop_def)
        {
            redde FALSUM;
        }

        repositorium->proprietas_ponere(repositorium->datum, prop_def,
            "entitas_genus", chorda_ut_cstr(*entitas_genus, dom->piscina));
        repositorium->proprietas_ponere(repositorium->datum, prop_def,
            "name", chorda_ut_cstr(*clavis, dom->piscina));

        /* Addere "est" relatio ad TypusSemanticus */
        repositorium->relatio_addere(repositorium->datum, prop_def, "est", typus_sem->id);

        /* Scaffoldare Genus::{entitas_genus} et addere relatio "habet_typum" */
        genus_ent = repositorium->entitas_scaffoldare(
            repositorium->datum,
            "Genus",
            chorda_ut_cstr(*entitas_genus, dom->piscina));

        si (genus_ent)
        {
            /* Ponere name si nondum */
            si (!entitas_proprietas_capere(genus_ent,
                chorda_internare_ex_literis(dom->intern, "name")))
            {
                repositorium->proprietas_ponere(repositorium->datum, genus_ent,
                    "name", chorda_ut_cstr(*entitas_genus, dom->piscina));
            }

            /* Addere relatio Genus --[habet_typum]--> ProprietasDefinitio */
            repositorium->relatio_addere(repositorium->datum, genus_ent,
                "habet_typum", prop_def->id);
        }
    }

    redde VERUM;
}


/* Resolvere referentia "Genus::slug" ad Entitas */
interior Entitas*
_layout_resolvere_referentia(
    LayoutDom*           dom,
    EntitasRepositorium* repositorium,
    chorda*              referentia)
{
    s32    sep_index;
    i32    sep_pos;
    chorda genus;
    chorda slug;
    chorda separator;

    /* Invenire ultimum "::" separator (permittit namespaced genus) */
    separator = chorda_ex_literis("::", dom->piscina);
    sep_index = chorda_invenire_ultimum_index(*referentia, separator);
    si (sep_index < ZEPHYRUM)
    {
        redde NIHIL;
    }

    /* Convertere ad i32 post validationem */
    sep_pos = (i32)sep_index;

    /* Scindere in genus et slug */
    genus = chorda_sectio(*referentia, ZEPHYRUM, sep_pos);
    slug = chorda_sectio(*referentia, sep_pos + II, referentia->mensura);

    /* Obtinere vel creare entitas (scaffoldare est idempotens) */
    redde repositorium->entitas_scaffoldare(
        repositorium->datum,
        chorda_ut_cstr(genus, dom->piscina),
        chorda_ut_cstr(slug, dom->piscina));
}

interior b32
_layout_processare_entitas(
    LayoutDom*           dom,
    StmlNodus*           nodus,
    EntitasRepositorium* repositorium,
    Xar*                 relationes_differentes)
{
    chorda*     genus_attr;
    chorda*     slug_attr;
    Entitas*    entitas;
    StmlNodus*  liberum;
    i32         i;
    i32         num_liberi;

    si (!repositorium)
    {
        redde FALSUM;
    }

    /* Legere genus et slug attributa */
    genus_attr = stml_attributum_capere(nodus, "genus");
    slug_attr = stml_attributum_capere(nodus, "slug");

    si (!genus_attr || !slug_attr)
    {
        redde FALSUM;
    }

    /* Creare entitas (idempotens) */
    entitas = repositorium->entitas_scaffoldare(
        repositorium->datum,
        chorda_ut_cstr(*genus_attr, dom->piscina),
        chorda_ut_cstr(*slug_attr, dom->piscina));

    si (!entitas)
    {
        redde FALSUM;
    }

    /* Processare liberos */
    num_liberi = stml_numerus_liberorum(nodus);
    per (i = ZEPHYRUM; i < num_liberi; i++)
    {
        liberum = stml_liberum_ad_indicem(nodus, i);
        si (!liberum || liberum->genus != STML_NODUS_ELEMENTUM || !liberum->titulus)
        {
            perge;
        }

        si (chorda_aequalis_literis(*liberum->titulus, "proprietas"))
        {
            /* <proprietas clavis='name' valor='value' typus='SemanticType' literalis='s32'/> */
            chorda* clavis = stml_attributum_capere(liberum, "clavis");
            chorda* valor = stml_attributum_capere(liberum, "valor");
            chorda* typus = stml_attributum_capere(liberum, "typus");
            chorda* literalis = stml_attributum_capere(liberum, "literalis");

            /* Verificare: literalis requirit typus */
            si (literalis && !typus)
            {
                /* Error: literalis sine typus */
                Entitas* error;

                error = repositorium->entitas_creare(repositorium->datum, "SchemaConflictus");
                si (error)
                {
                    repositorium->proprietas_ponere(repositorium->datum, error,
                        "genus_conflictus", "LiteralisSineTypus");
                    repositorium->proprietas_ponere(repositorium->datum, error,
                        "entitas_genus", chorda_ut_cstr(*genus_attr, dom->piscina));
                    repositorium->proprietas_ponere(repositorium->datum, error,
                        "name", chorda_ut_cstr(*clavis, dom->piscina));
                    repositorium->proprietas_ponere(repositorium->datum, error,
                        "literalis", chorda_ut_cstr(*literalis, dom->piscina));
                }
            }
            alioquin si (typus)
            {
                /* Processare schema */
                _layout_processare_schema_proprietatis(
                    dom,
                    repositorium,
                    genus_attr,
                    clavis,
                    typus,
                    literalis);
            }

            /* Ponere valorem proprietatis */
            si (clavis && valor)
            {
                repositorium->proprietas_ponere(
                    repositorium->datum,
                    entitas,
                    chorda_ut_cstr(*clavis, dom->piscina),
                    chorda_ut_cstr(*valor, dom->piscina));
            }
        }
        alioquin si (chorda_aequalis_literis(*liberum->titulus, "nota"))
        {
            /* <nota>#tag</nota> - textus contentum est nota */
            chorda textus = stml_textus_internus(liberum, dom->piscina);
            si (textus.datum && textus.mensura > ZEPHYRUM)
            {
                repositorium->nota_addere(
                    repositorium->datum,
                    entitas,
                    chorda_ut_cstr(textus, dom->piscina));
            }
        }
        alioquin si (chorda_aequalis_literis(*liberum->titulus, "relatio"))
        {
            /* <relatio genus='pages' ad='Page::introduction'/> */
            /* Differre ad secundam passam */
            LayoutRelatioDifferens* diff;
            chorda* genus_rel = stml_attributum_capere(liberum, "genus");
            chorda* ad = stml_attributum_capere(liberum, "ad");

            si (genus_rel && ad)
            {
                diff = xar_addere(relationes_differentes);
                si (diff)
                {
                    diff->ab_entitas = entitas;
                    diff->genus_relationis = genus_rel;
                    diff->ad_referentia = ad;
                }
            }
        }
    }

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
    LayoutDom*              dom;
    StmlResultus            res;
    StmlNodus*              layout_nodus;
    StmlNodus*              liberum;
    i32                     i;
    i32                     num_liberi;
    Xar*                    relationes_differentes;
    LayoutRelatioDifferens* diff;

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
    dom->reg_commandi = NIHIL;  /* Potest poni post per layout_ponere_reg_commandi */

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

    /* Creare xar pro relationes differentes */
    relationes_differentes = xar_creare(piscina, magnitudo(LayoutRelatioDifferens));
    si (!relationes_differentes)
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

    /* === PRIMA PASSA: Processare entitates (ante widgets!) === */
    num_liberi = stml_numerus_liberorum(layout_nodus);
    per (i = ZEPHYRUM; i < num_liberi; i++)
    {
        liberum = stml_liberum_ad_indicem(layout_nodus, i);
        si (!liberum || liberum->genus != STML_NODUS_ELEMENTUM || !liberum->titulus)
        {
            perge;
        }

        si (chorda_aequalis_literis(*liberum->titulus, "entitas"))
        {
            _layout_processare_entitas(dom, liberum, repositorium, relationes_differentes);
        }
    }

    /* === SECUNDA PASSA: Resolvere relationes === */
    per (i = ZEPHYRUM; i < xar_numerus(relationes_differentes); i++)
    {
        Entitas* destinatio;

        diff = (LayoutRelatioDifferens*)xar_obtinere(relationes_differentes, i);

        /* Resolvere "Genus::slug" ad entitas */
        destinatio = _layout_resolvere_referentia(dom, repositorium, diff->ad_referentia);
        si (destinatio)
        {
            repositorium->relatio_addere(
                repositorium->datum,
                diff->ab_entitas,
                chorda_ut_cstr(*diff->genus_relationis, dom->piscina),
                destinatio->id);
        }
    }

    /* === Addere relatio root --contains--> Genus::Genus === */
    si (repositorium)
    {
        Xar*      radices;
        Entitas*  entitas_radix;
        Entitas*  genus_genus;

        radices = repositorium->capere_radices(repositorium->datum);
        si (radices && xar_numerus(radices) > ZEPHYRUM)
        {
            entitas_radix = *(Entitas**)xar_obtinere(radices, ZEPHYRUM);
            si (entitas_radix)
            {
                /* Scaffoldare Genus::Genus (creabitur si non existit) */
                genus_genus = repositorium->entitas_scaffoldare(
                    repositorium->datum, "Genus", "Genus");
                si (genus_genus)
                {
                    /* Addere relatio "contains" si nondum existit */
                    chorda* contains_genus;
                    b32     iam_habet;
                    i32     j;
                    i32     num_rel;
                    Relatio* rel;

                    contains_genus = chorda_internare_ex_literis(intern, "contains");
                    iam_habet = FALSUM;

                    num_rel = xar_numerus(entitas_radix->relationes);
                    per (j = ZEPHYRUM; j < num_rel; j++)
                    {
                        rel = (Relatio*)xar_obtinere(entitas_radix->relationes, j);
                        si (rel && rel->genus == contains_genus &&
                            rel->destinatio_id == genus_genus->id)
                        {
                            iam_habet = VERUM;
                            frange;
                        }
                    }

                    si (!iam_habet)
                    {
                        repositorium->relatio_addere(
                            repositorium->datum,
                            entitas_radix,
                            "contains",
                            genus_genus->id);
                    }
                }
            }
        }
    }

    /* === TERTIA PASSA: Processare widgets (post entitates) === */
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
        alioquin si (chorda_aequalis_literis(*liberum->titulus, "libro"))
        {
            _layout_processare_libro(dom, liberum, repositorium);
        }
        alioquin si (chorda_aequalis_literis(*liberum->titulus, "navigator"))
        {
            _layout_processare_navigator(dom, liberum, repositorium);
        }
        /* entitas iam processata in prima passa */
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

LibroPaginarum*
layout_obtinere_libro(
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

    si (!inventum || !introitus || introitus->genus != LAYOUT_WIDGET_LIBRO)
    {
        redde NIHIL;
    }

    redde (LibroPaginarum*)introitus->datum;
}

vacuum
layout_ponere_reg_commandi(
    LayoutDom*         dom,
    RegistrumCommandi* reg_commandi)
{
    TabulaIterator iter;
    chorda clavis;
    vacuum* valor;
    LayoutWidgetIntroitus* introitus;
    Pagina* pagina;

    si (!dom)
    {
        redde;
    }

    dom->reg_commandi = reg_commandi;

    /* Iterare per omnes widgets et ponere registrum in Pagina coloratios */
    iter = tabula_dispersa_iterator_initium(dom->widgets);
    dum (tabula_dispersa_iterator_proximum(&iter, &clavis, &valor))
    {
        introitus = (LayoutWidgetIntroitus*)valor;
        si (introitus && introitus->genus == LAYOUT_WIDGET_PAGINA)
        {
            pagina = (Pagina*)introitus->datum;
            si (pagina && pagina->coloratio)
            {
                coloratio_ponere_registrum(pagina->coloratio, reg_commandi);
            }
        }
        alioquin si (introitus && introitus->genus == LAYOUT_WIDGET_LIBRO)
        {
            LibroPaginarum* libro;

            libro = (LibroPaginarum*)introitus->datum;
            si (libro)
            {
                libro_ponere_reg_commandi(libro, reg_commandi);
            }
        }
    }
}
