#include "concha.h"
#include "filum.h"
#include "delineare.h"
#include "thema.h"
#include "tempus.h"
#include "layout.h"
#include "biblia_visus.h"
#include "librarium_visus.h"
#include "thema_visus.h"
#include "sputnik_syntaxis.h"
#include "arx_caeli.h"
#include "navigator_entitatum.h"
#include "libro_paginarum.h"
#include "fons_visus.h"
#include <stdio.h>
#include <time.h>
#include <string.h>

/* ==================================================
 * Built-in Commands
 * ================================================== */

/* $date command - insert/update current date in output region */
interior b32
_concha_command_date(
    ContextusCommandi* ctx)
{
    character expectatum[XIII];  /* " MM/DD/YYYY\0" = 12 chars */
    character contentum[XIII];
    i32 longitudo;
    time_t tempus_nunc;
    structura tm* tempus_info;

    /* Generare datum expectatum */
    tempus_nunc = time(NIHIL);
    tempus_info = localtime(&tempus_nunc);
    sprintf(expectatum, " %02d/%02d/%04d",
            tempus_info->tm_mon + I,
            tempus_info->tm_mday,
            tempus_info->tm_year + MCMX);  /* 1900 */

    longitudo = XI;  /* " MM/DD/YYYY" = 11 characters */

    /* Legere contentum currentem ad positionem */
    tabula_legere_ad_positionem(&ctx->pagina->tabula,
        ctx->linea, ctx->columna, contentum, longitudo);

    /* Si iam correctum, nihil facere */
    si (memcmp(contentum, expectatum, (size_t)longitudo) == ZEPHYRUM)
    {
        redde VERUM;
    }

    /* Aliter: inserere spatium et scribere */
    si (!tabula_inserere_spatium(&ctx->pagina->tabula,
        ctx->linea, ctx->columna, longitudo))
    {
        redde FALSUM;  /* Pagina plena */
    }

    tabula_scribere_ad_positionem(&ctx->pagina->tabula,
        ctx->linea, ctx->columna, expectatum);

    redde VERUM;
}


/* ==================================================
 * Creatio
 * ================================================== */

Concha*
concha_creare(ConchaConfiguratio* config)
{
    Concha*              concha;
    Piscina*             piscina;
    InternamentumChorda* intern;
    Persistentia*        persistentia;
    EntitasRepositorium* repositorium;
    RegistrumCommandi*   reg_commandi;
    RegistrumWidget*     reg_widget;
    ContextusWidget*     ctx;
    Schirmata*           schirmata;
    LibroPaginarum*      libro;
    Fenestra*            fenestra;
    TabulaPixelorum*     tabula;
    FenestraConfiguratio fenestra_config;

    si (!config)
    {
        redde NIHIL;
    }

    /* Initiare thema */
    thema_initiare();

    /* Initiare tempus */
    tempus_initiare();

    /* Creare piscinam */
    piscina = piscina_generare_dynamicum("concha", M * M * II);  /* 2MB */
    si (!piscina)
    {
        imprimere("Concha: non potest creare piscinam\n");
        redde NIHIL;
    }

    /* Allocare concha */
    concha = piscina_allocare(piscina, magnitudo(Concha));
    si (!concha)
    {
        piscina_destruere(piscina);
        redde NIHIL;
    }

    concha->piscina = piscina;
    concha->latitudo = config->latitudo;
    concha->altitudo = config->altitudo;
    concha->via_persistentia = config->via_persistentia;
    concha->currens = FALSUM;

    /* Obtinere internamentum */
    intern = internamentum_globale();
    concha->intern = intern;

    /* Creare persistentia */
    si (config->via_persistentia)
    {
        /* Creare directorium si necesse */
        {
            character directorium[CXXVIII];
            constans character* via;
            i32 i;
            i32 ultima_sep;

            via = config->via_persistentia;
            ultima_sep = (i32)-1;

            /* Invenire ultimum separator */
            per (i = ZEPHYRUM; via[i] != '\0' && i < CXXVII; i++)
            {
                si (via[i] == '/')
                {
                    ultima_sep = i;
                }
            }

            si (ultima_sep > ZEPHYRUM)
            {
                memcpy(directorium, via, (size_t)ultima_sep);
                directorium[ultima_sep] = '\0';
                filum_directorium_creare_si_necesse(directorium);
            }
        }

        persistentia = persistentia_nuntium_creare(piscina, config->via_persistentia);
    }
    alioquin
    {
        persistentia = persistentia_memoria_creare(piscina);
    }

    si (!persistentia)
    {
        imprimere("Concha: non potest creare persistentiam\n");
        piscina_destruere(piscina);
        redde NIHIL;
    }
    concha->persistentia = persistentia;

    /* Creare repositorium */
    repositorium = entitas_repositorium_creare(piscina, persistentia);
    si (!repositorium)
    {
        imprimere("Concha: non potest creare repositorium\n");
        piscina_destruere(piscina);
        redde NIHIL;
    }
    concha->repositorium = repositorium;

    /* Creare registrum commandi */
    reg_commandi = registrum_commandi_creare(piscina);
    si (!reg_commandi)
    {
        imprimere("Concha: non potest creare registrum commandi\n");
        piscina_destruere(piscina);
        redde NIHIL;
    }
    concha->reg_commandi = reg_commandi;

    /* Creare registrum widget */
    reg_widget = registrum_widget_creare(piscina, intern);
    si (!reg_widget)
    {
        imprimere("Concha: non potest creare registrum widget\n");
        piscina_destruere(piscina);
        redde NIHIL;
    }
    concha->reg_widget = reg_widget;

    /* Registrare built-in widgets */
    layout_registrare_defalta(reg_widget);

    /* Creare contextum widget */
    ctx = contextus_widget_creare(piscina, intern, repositorium, reg_commandi, reg_widget, NIHIL, NIHIL);
    si (!ctx)
    {
        imprimere("Concha: non potest creare contextum widget\n");
        piscina_destruere(piscina);
        redde NIHIL;
    }
    concha->ctx = ctx;

    /* Creare libro paginarum (communicatus inter omnes schirmas) */
    libro = libro_creare(ctx);
    si (!libro)
    {
        imprimere("Concha: non potest creare libro\n");
        piscina_destruere(piscina);
        redde NIHIL;
    }
    libro_carcare(libro);

    /* Creare schirmata (screens controller) */
    schirmata = schirmata_creare(ctx, libro);
    si (!schirmata)
    {
        imprimere("Concha: non potest creare schirmata\n");
        piscina_destruere(piscina);
        redde NIHIL;
    }
    concha->schirmata = schirmata;

    /* Restituere status salvatum ex repository */
    schirmata_carcare_omnes(schirmata);

    /* Initiare widgets - registrant suos commandi */
    biblia_visus_init(ctx);
    librarium_visus_init(ctx);
    thema_visus_init(ctx);
    fons_visus_init(ctx);
    sputnik_syntaxis_init(ctx);
    arx_caeli_init(ctx);
    navigator_entitatum_init(ctx);
    libro_paginarum_init(ctx);

    /* Registrare built-in commands (non-widget) */
    registrum_commandi_registrare(reg_commandi, "date", _concha_command_date, NIHIL);

    /* Configurare fenestram */
    fenestra_config.titulus = config->titulus ? config->titulus : "Concha";
    fenestra_config.x = C;
    fenestra_config.y = C;
    fenestra_config.latitudo = config->latitudo;
    fenestra_config.altitudo = config->altitudo;
    fenestra_config.vexilla = FENESTRA_ORDINARIA;

    /* Creare fenestram */
    fenestra = fenestra_creare(piscina, &fenestra_config);
    si (!fenestra)
    {
        imprimere("Concha: non potest creare fenestram\n");
        piscina_destruere(piscina);
        redde NIHIL;
    }
    concha->fenestra = fenestra;

    /* Creare tabulam pixelorum */
    tabula = fenestra_creare_tabulam_pixelorum(piscina, fenestra, config->altitudo);
    si (!tabula)
    {
        imprimere("Concha: non potest creare tabulam\n");
        fenestra_destruere(fenestra);
        piscina_destruere(piscina);
        redde NIHIL;
    }
    concha->tabula = tabula;

    redde concha;
}


/* ==================================================
 * Registratio
 * ================================================== */

b32
concha_registrare_widget(
    Concha*              concha,
    constans character*  titulus,
    FunctioWidgetFactory factory)
{
    si (!concha || !concha->reg_widget)
    {
        redde FALSUM;
    }

    redde registrum_widget_registrare(concha->reg_widget, titulus, factory);
}

b32
concha_registrare_commandi(
    Concha*             concha,
    constans character* nomen_cmd,
    FunctioCommand      functio,
    vacuum*             datum)
{
    si (!concha || !concha->reg_commandi)
    {
        redde FALSUM;
    }

    registrum_commandi_registrare(concha->reg_commandi, nomen_cmd, functio, datum);
    redde VERUM;
}


/* ==================================================
 * Layout
 * ================================================== */

b32
concha_ponere_layout(
    Concha*             concha,
    i32                 schirma_index,
    constans character* stml)
{
    si (!concha || !concha->schirmata || !stml)
    {
        redde FALSUM;
    }

    /* TODO: Implementare per-screen layout customization */
    /* Pro nunc, schirmata creat layouts automatice */
    (vacuum)schirma_index;
    (vacuum)stml;

    redde VERUM;
}


/* ==================================================
 * Executio
 * ================================================== */

s32
concha_currere(Concha* concha)
{
    Eventus eventus;

    si (!concha || !concha->fenestra || !concha->tabula)
    {
        redde I;
    }

    /* Monstrare fenestram */
    fenestra_monstrare(concha->fenestra);

    /* Cyclus principalis */
    concha->currens = VERUM;

    dum (concha->currens && !fenestra_debet_claudere(concha->fenestra))
    {
        /* Actualizare tempus */
        tempus_quadrum();

        /* Perscrutari eventus */
        fenestra_perscrutari_eventus(concha->fenestra);

        /* Tractare eventus */
        dum (fenestra_obtinere_eventus(concha->fenestra, &eventus))
        {
            si (eventus.genus == EVENTUS_CLAUDERE)
            {
                concha->currens = FALSUM;
            }
            alioquin
            {
                b32 tractatus;

                /* Schirmata tractat omnes eventus (screens, focus, routing, etc) */
                tractatus = schirmata_tractare_eventum(concha->schirmata, &eventus);

                /* Si pagina reddidit FALSUM (ESC in normal mode), exire */
                si (!tractatus && eventus.genus == EVENTUS_CLAVIS_DEPRESSUS)
                {
                    si (eventus.datum.clavis.clavis == CLAVIS_EFFUGIUM)
                    {
                        concha->currens = FALSUM;
                    }
                }
            }
        }

        /* Salvare libro si immundum et debounce praeteritum */
        libro_salvare_si_immundum(schirmata_libro(concha->schirmata));

        /* Vacare fondum cum colore background */
        tabula_pixelorum_vacare(concha->tabula,
            color_ad_pixelum(thema_color(COLOR_BACKGROUND)));

        /* Schirmata reddit schirmam currentem et tab bar */
        schirmata_reddere(concha->schirmata, concha->tabula, I);

        /* Praesentare pixela */
        fenestra_praesentare_pixela(concha->fenestra, concha->tabula);
    }

    /* Salvare status ante clausuram */
    schirmata_salvare_omnes(concha->schirmata);

    /* Purgare */
    fenestra_destruere(concha->fenestra);
    piscina_destruere(concha->piscina);

    redde ZEPHYRUM;
}


/* ==================================================
 * Accessores
 * ================================================== */

LibroPaginarum*
concha_libro(Concha* concha)
{
    si (!concha || !concha->schirmata)
    {
        redde NIHIL;
    }

    redde schirmata_libro(concha->schirmata);
}

Schirmata*
concha_schirmata(Concha* concha)
{
    si (!concha)
    {
        redde NIHIL;
    }

    redde concha->schirmata;
}

EntitasRepositorium*
concha_repositorium(Concha* concha)
{
    si (!concha)
    {
        redde NIHIL;
    }

    redde concha->repositorium;
}

Piscina*
concha_piscina(Concha* concha)
{
    si (!concha)
    {
        redde NIHIL;
    }

    redde concha->piscina;
}

InternamentumChorda*
concha_intern(Concha* concha)
{
    si (!concha)
    {
        redde NIHIL;
    }

    redde concha->intern;
}
