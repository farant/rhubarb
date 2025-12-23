#include "librarium_visus.h"
#include "thema.h"
#include "color.h"
#include "delineare.h"
#include "chorda.h"
#include "stml.h"
#include "filum.h"
#include "internamentum.h"
#include "registrum_commandi.h"
#include "pagina.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

/* ==================================================
 * Constantae
 * ================================================== */

#define CHAR_LATITUDO    VI     /* 6 pixels per character */
#define CHAR_ALTITUDO    VIII   /* 8 pixels per character */
#define PADDING          II     /* 2 characters padding */
#define LIBRI_PER_PAGINA_COMPACTA  XX  /* 20 books per page in compact mode */


/* ==================================================
 * Functiones Auxiliares
 * ================================================== */

/* Creare chorda ex C string (view, non allocatio) */
hic_manens chorda
_chorda_ex_cstr(
    constans character* cstr)
{
    chorda fructus;
    union { constans character* c; i8* m; } u;
    u.c = cstr;

    fructus.datum = u.m;
    fructus.mensura = (i32)strlen(cstr);

    redde fructus;
}

/* Comparare chordas insensitive ad casus */
hic_manens b32
_chorda_incipit_insensitive(
    chorda textus,
    chorda praefixum)
{
    i32 i;

    si (textus.mensura < praefixum.mensura)
    {
        redde FALSUM;
    }

    per (i = 0; i < praefixum.mensura; i++)
    {
        character a = (character)textus.datum[i];
        character b = (character)praefixum.datum[i];

        si (tolower((insignatus character)a) != tolower((insignatus character)b))
        {
            redde FALSUM;
        }
    }

    redde VERUM;
}

/* Comparare annum (pro sortatio) */
hic_manens s32
_comparare_annum(
    chorda a,
    chorda b)
{
    s32 annus_a = 0;
    s32 annus_b = 0;
    i32 i;
    b32 neg_a = FALSUM;
    b32 neg_b = FALSUM;

    /* Parse annus a */
    i = 0;
    si (a.mensura > 0 && (character)a.datum[0] == '-')
    {
        neg_a = VERUM;
        i = 1;
    }
    dum (i < a.mensura)
    {
        character c = (character)a.datum[i];
        si (c >= '0' && c <= '9')
        {
            annus_a = annus_a * X + (s32)(c - '0');
        }
        i++;
    }
    si (neg_a)
    {
        annus_a = -annus_a;
    }

    /* Parse annus b */
    i = 0;
    si (b.mensura > 0 && (character)b.datum[0] == '-')
    {
        neg_b = VERUM;
        i = 1;
    }
    dum (i < b.mensura)
    {
        character c = (character)b.datum[i];
        si (c >= '0' && c <= '9')
        {
            annus_b = annus_b * X + (s32)(c - '0');
        }
        i++;
    }
    si (neg_b)
    {
        annus_b = -annus_b;
    }

    redde annus_a - annus_b;
}

/* Comparare titulum (pro sortatio alphabetica) */
hic_manens s32
_comparare_titulum(
    chorda a,
    chorda b)
{
    i32 len;
    i32 i;

    len = a.mensura < b.mensura ? a.mensura : b.mensura;

    per (i = 0; i < len; i++)
    {
        character ca = (character)tolower((insignatus character)a.datum[i]);
        character cb = (character)tolower((insignatus character)b.datum[i]);

        si (ca != cb)
        {
            redde (s32)(ca - cb);
        }
    }

    redde (s32)(a.mensura) - (s32)(b.mensura);
}

/* Functiones _numerare_newlines et _est_versus nunc in paginarium.c */


/* ==================================================
 * Carcare Catalogum ex STML
 * ================================================== */

hic_manens vacuum
_librarium_visus_carcare_catalogum(
    LibrariumVisus* visus)
{
    chorda contentum;
    StmlResultus resultus;
    Xar* libri_stml;
    s32 i;
    s32 numerus_librorum;

    /* Legere filum */
    contentum = filum_legere_totum(LIBRARIUM_VIA_STML, visus->piscina);
    si (contentum.mensura == 0)
    {
        redde;
    }

    /* Parse STML */
    resultus = stml_legere(contentum, visus->piscina, visus->ctx->intern);
    si (!resultus.successus || !resultus.elementum_radix)
    {
        redde;
    }

    /* Invenire omnes <liber> elementa */
    libri_stml = stml_invenire_omnes_liberos(resultus.elementum_radix, "liber", visus->piscina);
    si (!libri_stml)
    {
        redde;
    }

    numerus_librorum = (s32)xar_numerus(libri_stml);

    /* Creare array librorum */
    visus->libri = xar_creare(visus->piscina, magnitudo(LibrumInfo*));
    visus->tags_unici = xar_creare(visus->piscina, magnitudo(chorda));

    per (i = 0; i < numerus_librorum; i++)
    {
        StmlNodus* nodus_liber;
        LibrumInfo* liber;
        StmlNodus* nodus_temp;
        chorda* attr_status;
        Xar* tags_stml;

        nodus_liber = *(StmlNodus**)xar_obtinere(libri_stml, (i32)i);
        si (!nodus_liber)
        {
            perge;
        }

        /* Verifica status = "enriched" */
        attr_status = stml_attributum_capere(nodus_liber, "status");
        si (!attr_status || attr_status->mensura < 8)
        {
            perge;  /* Solo libri enriched */
        }

        /* Allocare LibrumInfo */
        liber = (LibrumInfo*)piscina_allocare(visus->piscina, magnitudo(LibrumInfo));
        si (!liber)
        {
            perge;
        }

        /* Capere numerus */
        {
            chorda* attr_numerus;
            attr_numerus = stml_attributum_capere(nodus_liber, "numerus");
            si (attr_numerus)
            {
                liber->numerus = *attr_numerus;
            }
            alioquin
            {
                liber->numerus.mensura = 0;
            }
        }

        /* Capere titulus */
        nodus_temp = stml_invenire_liberum(nodus_liber, "titulus");
        si (nodus_temp)
        {
            liber->titulus = stml_textus_internus(nodus_temp, visus->piscina);
        }
        alioquin
        {
            liber->titulus.mensura = 0;
        }

        /* Capere auctor */
        nodus_temp = stml_invenire_liberum(nodus_liber, "auctor");
        si (nodus_temp)
        {
            liber->auctor = stml_textus_internus(nodus_temp, visus->piscina);
        }
        alioquin
        {
            liber->auctor.mensura = 0;
        }

        /* Capere annus */
        nodus_temp = stml_invenire_liberum(nodus_liber, "annus");
        si (nodus_temp)
        {
            liber->annus = stml_textus_internus(nodus_temp, visus->piscina);
        }
        alioquin
        {
            liber->annus.mensura = 0;
        }

        /* Capere summarium */
        nodus_temp = stml_invenire_liberum(nodus_liber, "summarium");
        si (nodus_temp)
        {
            liber->summarium = stml_textus_internus(nodus_temp, visus->piscina);
        }
        alioquin
        {
            liber->summarium.mensura = 0;
        }

        /* Capere notae */
        nodus_temp = stml_invenire_liberum(nodus_liber, "notae");
        si (nodus_temp)
        {
            liber->notae = stml_textus_internus(nodus_temp, visus->piscina);
        }
        alioquin
        {
            liber->notae.mensura = 0;
        }

        /* Capere via */
        nodus_temp = stml_invenire_liberum(nodus_liber, "via");
        si (nodus_temp)
        {
            liber->via = stml_textus_internus(nodus_temp, visus->piscina);
        }
        alioquin
        {
            liber->via.mensura = 0;
        }

        /* Capere tags */
        liber->tags = xar_creare(visus->piscina, magnitudo(chorda));
        nodus_temp = stml_invenire_liberum(nodus_liber, "tags");
        si (nodus_temp)
        {
            tags_stml = stml_invenire_omnes_liberos(nodus_temp, "tag", visus->piscina);
            si (tags_stml)
            {
                s32 j;
                s32 num_tags = (s32)xar_numerus(tags_stml);

                per (j = 0; j < num_tags; j++)
                {
                    StmlNodus* nodus_tag;
                    chorda tag_textus;

                    nodus_tag = *(StmlNodus**)xar_obtinere(tags_stml, (i32)j);
                    si (nodus_tag)
                    {
                        tag_textus = stml_textus_internus(nodus_tag, visus->piscina);
                        si (tag_textus.mensura > 0)
                        {
                            *(chorda*)xar_addere(liber->tags) = tag_textus;

                            /* Addere ad tags_unici si non existit */
                            {
                                b32 inventum = FALSUM;
                                s32 k;
                                s32 num_unici = (s32)xar_numerus(visus->tags_unici);

                                per (k = 0; k < num_unici; k++)
                                {
                                    chorda* existens;
                                    existens = (chorda*)xar_obtinere(visus->tags_unici, (i32)k);
                                    si (existens && chorda_aequalis(*existens, tag_textus))
                                    {
                                        inventum = VERUM;
                                        frange;
                                    }
                                }

                                si (!inventum)
                                {
                                    *(chorda*)xar_addere(visus->tags_unici) = tag_textus;
                                }
                            }
                        }
                    }
                }
            }
        }

        /* Addere ad libri */
        *(LibrumInfo**)xar_addere(visus->libri) = liber;
    }

    /* Sortare tags alphabetice (insertion sort) */
    {
        s32 num_tags = (s32)xar_numerus(visus->tags_unici);
        s32 i;
        s32 j;

        per (i = 1; i < num_tags; i++)
        {
            chorda tag_i = *(chorda*)xar_obtinere(visus->tags_unici, (i32)i);
            j = i - 1;

            dum (j >= 0)
            {
                chorda* tag_j = (chorda*)xar_obtinere(visus->tags_unici, (i32)j);

                si (_comparare_titulum(*tag_j, tag_i) <= 0)
                {
                    frange;
                }

                /* Movere tag_j ad j+1 */
                *(chorda*)xar_obtinere(visus->tags_unici, (i32)(j + 1)) = *tag_j;
                j--;
            }

            /* Inserere tag_i ad j+1 */
            *(chorda*)xar_obtinere(visus->tags_unici, (i32)(j + 1)) = tag_i;
        }
    }
}


/* ==================================================
 * Filtrare et Sortare
 * ================================================== */

hic_manens vacuum
_librarium_visus_filtrare(
    LibrariumVisus* visus)
{
    s32 i;
    s32 num_libri;
    s32 j;
    s32 num_filtrati;

    si (!visus->libri)
    {
        redde;
    }

    /* Recreare array filtrati */
    visus->libri_filtrati = xar_creare(visus->piscina, magnitudo(LibrumInfo*));
    num_libri = (s32)xar_numerus(visus->libri);

    /* Applicare filtrum */
    per (i = 0; i < num_libri; i++)
    {
        LibrumInfo* liber;
        b32 includere = VERUM;

        liber = *(LibrumInfo**)xar_obtinere(visus->libri, (i32)i);
        si (!liber)
        {
            perge;
        }

        /* Filtrum per tag */
        si (visus->filtrum_tag.mensura > 0)
        {
            b32 habet_tag = FALSUM;
            s32 k;
            s32 num_tags = (s32)xar_numerus(liber->tags);

            per (k = 0; k < num_tags; k++)
            {
                chorda* tag;
                tag = (chorda*)xar_obtinere(liber->tags, (i32)k);
                si (tag && chorda_aequalis(*tag, visus->filtrum_tag))
                {
                    habet_tag = VERUM;
                    frange;
                }
            }

            si (!habet_tag)
            {
                includere = FALSUM;
            }
        }

        /* Filtrum per quaestio (titulus) */
        si (includere && visus->filtrum_quaestio.mensura > 0)
        {
            si (!_chorda_incipit_insensitive(liber->titulus, visus->filtrum_quaestio))
            {
                includere = FALSUM;
            }
        }

        si (includere)
        {
            *(LibrumInfo**)xar_addere(visus->libri_filtrati) = liber;
        }
    }

    /* Sortare */
    num_filtrati = (s32)xar_numerus(visus->libri_filtrati);

    /* Bubble sort (simplex, sufficit pro paucis libris) */
    per (i = 0; i < num_filtrati - 1; i++)
    {
        per (j = 0; j < num_filtrati - i - 1; j++)
        {
            LibrumInfo** ptr_a;
            LibrumInfo** ptr_b;
            LibrumInfo* a;
            LibrumInfo* b;
            s32 cmp;

            ptr_a = (LibrumInfo**)xar_obtinere(visus->libri_filtrati, (i32)j);
            ptr_b = (LibrumInfo**)xar_obtinere(visus->libri_filtrati, (i32)(j + 1));

            si (!ptr_a || !ptr_b)
            {
                perge;
            }

            a = *ptr_a;
            b = *ptr_b;

            si (visus->ordo == LIBRARIUM_ORDO_CHRONOLOGICUS)
            {
                cmp = _comparare_annum(a->annus, b->annus);
            }
            alioquin
            {
                cmp = _comparare_titulum(a->titulus, b->titulus);
            }

            si (cmp > 0)
            {
                /* Swap */
                *ptr_a = b;
                *ptr_b = a;
            }
        }
    }
}


/* ==================================================
 * Creatio
 * ================================================== */

LibrariumVisus*
librarium_visus_creare(
    Piscina*         piscina,
    ContextusWidget* ctx)
{
    LibrariumVisus* visus;

    si (!piscina || !ctx)
    {
        redde NIHIL;
    }

    visus = (LibrariumVisus*)piscina_allocare(piscina, magnitudo(LibrariumVisus));
    si (!visus)
    {
        redde NIHIL;
    }

    visus->piscina = piscina;
    visus->ctx = ctx;
    visus->modus = LIBRARIUM_MODUS_CATEGORIAE;
    visus->ordo = LIBRARIUM_ORDO_CHRONOLOGICUS;
    visus->categoria = LIBRARIUM_CAT_OMNES;
    visus->filtrum_tag.mensura = 0;
    visus->filtrum_quaestio.mensura = 0;
    visus->index_paginae = 0;
    visus->index_selecta = 0;
    visus->liber_currens = -1;
    visus->lector = librarium_lector_creare(piscina);
    visus->latitudo_characterum = 0;
    visus->altitudo_linearum = 0;
    visus->scala = 1;
    visus->widget_x = 0;
    visus->widget_y = 0;

    /* Carcare catalogum */
    _librarium_visus_carcare_catalogum(visus);

    /* Filtrare initialis */
    _librarium_visus_filtrare(visus);

    redde visus;
}


/* ==================================================
 * Reddere - Categoriae
 * ================================================== */

hic_manens vacuum
_reddere_categoriae(
    LibrariumVisus*  visus,
    TabulaPixelorum* tabula,
    i32              x,
    i32              y,
    i32              latitudo,
    i32              altitudo,
    i32              scala)
{
    ContextusDelineandi* ctx;
    i32 char_lat;
    i32 char_alt;
    i32 linea;
    i32 pixelum_text;
    i32 pixelum_text_dim;
    i32 pixelum_accent;
    Color color_background;
    chorda titulus;
    i32 i;
    constans character* optiones[III];
    i32 num_libri;

    (vacuum)altitudo;

    ctx = delineare_creare_contextum(visus->piscina, tabula);
    si (!ctx)
    {
        redde;
    }

    char_lat = CHAR_LATITUDO * scala;
    char_alt = CHAR_ALTITUDO * scala;

    color_background = thema_color(COLOR_BACKGROUND);
    pixelum_text = color_ad_pixelum(thema_color(COLOR_TEXT));
    pixelum_text_dim = color_ad_pixelum(thema_color(COLOR_TEXT_DIM));
    pixelum_accent = color_ad_pixelum(thema_color(COLOR_ACCENT_PRIMARY));

    /* Pingere fondum */
    delineare_rectangulum_plenum(ctx,
        x * char_lat, y * char_alt,
        latitudo * char_lat, altitudo * char_alt,
        color_background);

    linea = y + II;

    /* Titulus */
    titulus = _chorda_ex_cstr("LIBRARIUM - Bibliotheca");
    tabula_pixelorum_pingere_chordam_scalatam(tabula,
        (x + PADDING) * char_lat, linea * char_alt,
        titulus, pixelum_accent, scala);

    linea += II;

    /* Numerus librorum */
    num_libri = visus->libri ? xar_numerus(visus->libri) : 0;
    {
        character buffer[LXIV];
        sprintf(buffer, "%d libri in bibliotheca", num_libri);
        titulus = _chorda_ex_cstr(buffer);
        tabula_pixelorum_pingere_chordam_scalatam(tabula,
            (x + PADDING) * char_lat, linea * char_alt,
            titulus, pixelum_text_dim, scala);
    }

    linea += III;

    /* Menu optiones */
    optiones[0] = "Omnes (chronologice)";
    optiones[1] = "Per Titulum (alphabetice)";
    optiones[2] = "Per Tag";

    per (i = 0; i < III; i++)
    {
        i32 pixelum;

        si (visus->index_selecta == (s32)i)
        {
            pixelum = pixelum_accent;
            /* Praefixum selectum */
            titulus = _chorda_ex_cstr("> ");
            tabula_pixelorum_pingere_chordam_scalatam(tabula,
                (x + PADDING) * char_lat, linea * char_alt,
                titulus, pixelum_accent, scala);
        }
        alioquin
        {
            pixelum = pixelum_text;
        }

        titulus = _chorda_ex_cstr(optiones[i]);
        tabula_pixelorum_pingere_chordam_scalatam(tabula,
            (x + PADDING + II) * char_lat, linea * char_alt,
            titulus, pixelum, scala);

        linea += II;
    }

    /* Instructiones */
    linea = y + altitudo - II;
    titulus = _chorda_ex_cstr("j/k: selectare   Enter: aperire   Esc: exire");
    tabula_pixelorum_pingere_chordam_scalatam(tabula,
        (x + PADDING) * char_lat, linea * char_alt,
        titulus, pixelum_text_dim, scala);
}


/* ==================================================
 * Reddere - Tags
 * ================================================== */

hic_manens vacuum
_reddere_tags(
    LibrariumVisus*  visus,
    TabulaPixelorum* tabula,
    i32              x,
    i32              y,
    i32              latitudo,
    i32              altitudo,
    i32              scala)
{
    ContextusDelineandi* ctx;
    i32 char_lat;
    i32 char_alt;
    i32 linea;
    i32 pixelum_text;
    i32 pixelum_text_dim;
    i32 pixelum_accent;
    Color color_background;
    chorda titulus;
    i32 i;
    i32 num_tags;
    i32 tags_per_pagina;
    i32 init_tag;
    i32 finis_tag;

    ctx = delineare_creare_contextum(visus->piscina, tabula);
    si (!ctx)
    {
        redde;
    }

    char_lat = CHAR_LATITUDO * scala;
    char_alt = CHAR_ALTITUDO * scala;

    color_background = thema_color(COLOR_BACKGROUND);
    pixelum_text = color_ad_pixelum(thema_color(COLOR_TEXT));
    pixelum_text_dim = color_ad_pixelum(thema_color(COLOR_TEXT_DIM));
    pixelum_accent = color_ad_pixelum(thema_color(COLOR_ACCENT_PRIMARY));

    /* Pingere fondum */
    delineare_rectangulum_plenum(ctx,
        x * char_lat, y * char_alt,
        latitudo * char_lat, altitudo * char_alt,
        color_background);

    linea = y + II;

    /* Titulus */
    titulus = _chorda_ex_cstr("SELECTA TAG");
    tabula_pixelorum_pingere_chordam_scalatam(tabula,
        (x + PADDING) * char_lat, linea * char_alt,
        titulus, pixelum_accent, scala);

    linea += III;

    /* Lista tags */
    num_tags = visus->tags_unici ? (i32)xar_numerus(visus->tags_unici) : 0;
    tags_per_pagina = altitudo - VIII;
    init_tag = (i32)visus->index_paginae * tags_per_pagina;
    finis_tag = init_tag + tags_per_pagina;
    si (finis_tag > num_tags)
    {
        finis_tag = num_tags;
    }

    per (i = init_tag; i < finis_tag; i++)
    {
        chorda* tag;
        i32 pixelum;
        i32 idx_in_pagina = i - init_tag;

        tag = (chorda*)xar_obtinere(visus->tags_unici, (i32)i);
        si (!tag)
        {
            perge;
        }

        si (visus->index_selecta == (s32)idx_in_pagina)
        {
            pixelum = pixelum_accent;
            titulus = _chorda_ex_cstr("> ");
            tabula_pixelorum_pingere_chordam_scalatam(tabula,
                (x + PADDING) * char_lat, linea * char_alt,
                titulus, pixelum_accent, scala);
        }
        alioquin
        {
            pixelum = pixelum_text;
        }

        tabula_pixelorum_pingere_chordam_scalatam(tabula,
            (x + PADDING + II) * char_lat, linea * char_alt,
            *tag, pixelum, scala);

        linea++;
    }

    /* Pagina indicator */
    {
        i32 num_paginae = (num_tags + tags_per_pagina - 1) / tags_per_pagina;
        si (num_paginae < 1)
        {
            num_paginae = 1;
        }
        si (num_paginae > 1)
        {
            character pagina_buf[XXXII];
            sprintf(pagina_buf, "Pagina %d/%d", (i32)visus->index_paginae + 1, num_paginae);
            titulus = _chorda_ex_cstr(pagina_buf);
            tabula_pixelorum_pingere_chordam_scalatam(tabula,
                (x + latitudo - PADDING - XVI) * char_lat, (y + II) * char_alt,
                titulus, pixelum_text_dim, scala);
        }
    }

    /* Instructiones */
    linea = y + altitudo - II;
    titulus = _chorda_ex_cstr("j/k: selectare   h/l: pagina   Enter: filtrare   Esc: retro");
    tabula_pixelorum_pingere_chordam_scalatam(tabula,
        (x + PADDING) * char_lat, linea * char_alt,
        titulus, pixelum_text_dim, scala);
}


/* ==================================================
 * Reddere - Catalogo Compacto
 * ================================================== */

hic_manens vacuum
_reddere_catalogo_compacto(
    LibrariumVisus*  visus,
    TabulaPixelorum* tabula,
    i32              x,
    i32              y,
    i32              latitudo,
    i32              altitudo,
    i32              scala)
{
    ContextusDelineandi* ctx;
    i32 char_lat;
    i32 char_alt;
    i32 linea;
    i32 pixelum_text;
    i32 pixelum_text_dim;
    i32 pixelum_accent;
    Color color_background;
    chorda titulus;
    i32 i;
    i32 num_libri;
    i32 libri_per_pagina;
    i32 init_liber;
    i32 finis_liber;
    i32 num_paginae;

    ctx = delineare_creare_contextum(visus->piscina, tabula);
    si (!ctx)
    {
        redde;
    }

    char_lat = CHAR_LATITUDO * scala;
    char_alt = CHAR_ALTITUDO * scala;

    color_background = thema_color(COLOR_BACKGROUND);
    pixelum_text = color_ad_pixelum(thema_color(COLOR_TEXT));
    pixelum_text_dim = color_ad_pixelum(thema_color(COLOR_TEXT_DIM));
    pixelum_accent = color_ad_pixelum(thema_color(COLOR_ACCENT_PRIMARY));

    /* Pingere fondum */
    delineare_rectangulum_plenum(ctx,
        x * char_lat, y * char_alt,
        latitudo * char_lat, altitudo * char_alt,
        color_background);

    linea = y + I;

    /* Header */
    {
        character buffer[CXXVIII];
        constans character* modo_str;

        si (visus->categoria == LIBRARIUM_CAT_PER_TITULUM)
        {
            modo_str = "Per Titulum";
        }
        alioquin si (visus->filtrum_tag.mensura > 0)
        {
            modo_str = "Per Tag";
        }
        alioquin
        {
            modo_str = "Omnes";
        }

        sprintf(buffer, "CATALOGUS - %s", modo_str);
        titulus = _chorda_ex_cstr(buffer);
        tabula_pixelorum_pingere_chordam_scalatam(tabula,
            (x + PADDING) * char_lat, linea * char_alt,
            titulus, pixelum_accent, scala);
    }

    linea += II;

    /* Lista librorum */
    num_libri = visus->libri_filtrati ? (i32)xar_numerus(visus->libri_filtrati) : 0;
    libri_per_pagina = altitudo - VI;
    init_liber = (i32)visus->index_paginae * libri_per_pagina;
    finis_liber = init_liber + libri_per_pagina;
    si (finis_liber > num_libri)
    {
        finis_liber = num_libri;
    }

    per (i = init_liber; i < finis_liber; i++)
    {
        LibrumInfo* liber;
        i32 pixelum;
        i32 idx_in_pagina = i - init_liber;
        character buffer[CCLVI];
        s32 annus_val = 0;
        i32 k;
        b32 neg = FALSUM;

        liber = *(LibrumInfo**)xar_obtinere(visus->libri_filtrati, i);
        si (!liber)
        {
            perge;
        }

        si (visus->index_selecta == (s32)idx_in_pagina)
        {
            pixelum = pixelum_accent;
        }
        alioquin
        {
            pixelum = pixelum_text;
        }

        /* Parse annus */
        k = 0;
        si (liber->annus.mensura > 0 && (character)liber->annus.datum[0] == '-')
        {
            neg = VERUM;
            k = 1;
        }
        dum (k < liber->annus.mensura)
        {
            character c = (character)liber->annus.datum[k];
            si (c >= '0' && c <= '9')
            {
                annus_val = annus_val * X + (s32)(c - '0');
            }
            k++;
        }
        si (neg)
        {
            annus_val = -annus_val;
        }

        /* Formattare linea: [annus] titulus - auctor */
        {
            character annus_str[XVI];
            character titulus_trunc[C];
            character auctor_trunc[L];
            i32 tit_len;
            i32 auc_len;

            si (annus_val < 0)
            {
                sprintf(annus_str, "%d BC", -annus_val);
            }
            alioquin
            {
                sprintf(annus_str, "%d", annus_val);
            }

            /* Truncare titulus */
            tit_len = liber->titulus.mensura;
            si (tit_len > XC)
            {
                tit_len = XC;
            }
            memcpy(titulus_trunc, liber->titulus.datum, (size_t)tit_len);
            titulus_trunc[tit_len] = '\0';

            /* Truncare auctor */
            auc_len = liber->auctor.mensura;
            si (auc_len > XL)
            {
                auc_len = XL;
            }
            memcpy(auctor_trunc, liber->auctor.datum, (size_t)auc_len);
            auctor_trunc[auc_len] = '\0';

            sprintf(buffer, "%8s  %s - %s", annus_str, titulus_trunc, auctor_trunc);
        }

        /* Praefixum si selectum */
        si (visus->index_selecta == (s32)idx_in_pagina)
        {
            titulus = _chorda_ex_cstr("> ");
            tabula_pixelorum_pingere_chordam_scalatam(tabula,
                (x + PADDING) * char_lat, linea * char_alt,
                titulus, pixelum_accent, scala);
        }

        titulus = _chorda_ex_cstr(buffer);
        tabula_pixelorum_pingere_chordam_scalatam(tabula,
            (x + PADDING + II) * char_lat, linea * char_alt,
            titulus, pixelum, scala);

        linea++;
    }

    /* Footer cum paginatione */
    linea = y + altitudo - II;
    num_paginae = (num_libri + libri_per_pagina - 1) / libri_per_pagina;
    si (num_paginae < 1)
    {
        num_paginae = 1;
    }
    {
        character buffer[LXIV];
        sprintf(buffer, "Pagina %d/%d   h/l: paginae   j/k: selectare   Enter: aperire",
            visus->index_paginae + 1, num_paginae);
        titulus = _chorda_ex_cstr(buffer);
        tabula_pixelorum_pingere_chordam_scalatam(tabula,
            (x + PADDING) * char_lat, linea * char_alt,
            titulus, pixelum_text_dim, scala);
    }
}


/* ==================================================
 * Reddere - Catalogo Plenus (uno libro per pagina)
 * ================================================== */

hic_manens vacuum
_reddere_catalogo_plenus(
    LibrariumVisus*  visus,
    TabulaPixelorum* tabula,
    i32              x,
    i32              y,
    i32              latitudo,
    i32              altitudo,
    i32              scala)
{
    ContextusDelineandi* ctx;
    i32 char_lat;
    i32 char_alt;
    i32 linea;
    i32 pixelum_text;
    i32 pixelum_text_dim;
    i32 pixelum_accent;
    Color color_background;
    chorda titulus;
    i32 num_libri;
    LibrumInfo* liber;
    i32 chars_disponibiles;

    ctx = delineare_creare_contextum(visus->piscina, tabula);
    si (!ctx)
    {
        redde;
    }

    char_lat = CHAR_LATITUDO * scala;
    char_alt = CHAR_ALTITUDO * scala;

    color_background = thema_color(COLOR_BACKGROUND);
    pixelum_text = color_ad_pixelum(thema_color(COLOR_TEXT));
    pixelum_text_dim = color_ad_pixelum(thema_color(COLOR_TEXT_DIM));
    pixelum_accent = color_ad_pixelum(thema_color(COLOR_ACCENT_PRIMARY));

    /* Pingere fondum */
    delineare_rectangulum_plenum(ctx,
        x * char_lat, y * char_alt,
        latitudo * char_lat, altitudo * char_alt,
        color_background);

    linea = y + I;
    chars_disponibiles = latitudo - (PADDING * II);

    num_libri = visus->libri_filtrati ? (i32)xar_numerus(visus->libri_filtrati) : 0;

    si (num_libri == 0)
    {
        titulus = _chorda_ex_cstr("Nihil inventum");
        tabula_pixelorum_pingere_chordam_scalatam(tabula,
            (x + PADDING) * char_lat, linea * char_alt,
            titulus, pixelum_text_dim, scala);
        redde;
    }

    si ((i32)visus->index_paginae >= num_libri)
    {
        visus->index_paginae = (s32)(num_libri - 1);
    }
    si (visus->index_paginae < 0)
    {
        visus->index_paginae = 0;
    }

    liber = *(LibrumInfo**)xar_obtinere(visus->libri_filtrati, (i32)visus->index_paginae);
    si (!liber)
    {
        redde;
    }

    /* Titulus */
    tabula_pixelorum_pingere_chordam_scalatam(tabula,
        (x + PADDING) * char_lat, linea * char_alt,
        liber->titulus, pixelum_accent, scala);
    linea += II;

    /* Auctor et Annus */
    {
        character buffer[CXXVIII];
        s32 annus_val = 0;
        i32 k = 0;
        b32 neg = FALSUM;

        si (liber->annus.mensura > 0 && (character)liber->annus.datum[0] == '-')
        {
            neg = VERUM;
            k = 1;
        }
        dum (k < liber->annus.mensura)
        {
            character c = (character)liber->annus.datum[k];
            si (c >= '0' && c <= '9')
            {
                annus_val = annus_val * X + (s32)(c - '0');
            }
            k++;
        }
        si (neg)
        {
            annus_val = -annus_val;
        }

        si (annus_val < 0)
        {
            sprintf(buffer, "%.*s (%d BC)",
                (int)liber->auctor.mensura, (constans character*)liber->auctor.datum,
                -annus_val);
        }
        alioquin
        {
            sprintf(buffer, "%.*s (%d)",
                (int)liber->auctor.mensura, (constans character*)liber->auctor.datum,
                annus_val);
        }
        titulus = _chorda_ex_cstr(buffer);
        tabula_pixelorum_pingere_chordam_scalatam(tabula,
            (x + PADDING) * char_lat, linea * char_alt,
            titulus, pixelum_text, scala);
    }
    linea++;

    /* Gutenberg Numerus */
    si (liber->numerus.mensura > 0)
    {
        character buffer[LXIV];
        sprintf(buffer, "Gutenberg #%.*s",
            (int)liber->numerus.mensura, (constans character*)liber->numerus.datum);
        titulus = _chorda_ex_cstr(buffer);
        tabula_pixelorum_pingere_chordam_scalatam(tabula,
            (x + PADDING) * char_lat, linea * char_alt,
            titulus, pixelum_text_dim, scala);
    }
    linea++;

    /* Tags */
    si (liber->tags && xar_numerus(liber->tags) > 0)
    {
        character tags_buffer[CXXVIII];
        i32 tags_pos = 0;
        i32 t;
        i32 num_tags_liber = (i32)xar_numerus(liber->tags);

        per (t = 0; t < num_tags_liber && tags_pos < CXX; t++)
        {
            chorda* tag = (chorda*)xar_obtinere(liber->tags, t);
            si (tag && tag->mensura > 0)
            {
                i32 len = tag->mensura;
                si (tags_pos + len + II > CXX)
                {
                    frange;
                }
                si (tags_pos > 0)
                {
                    tags_buffer[tags_pos++] = ',';
                    tags_buffer[tags_pos++] = ' ';
                }
                memcpy(tags_buffer + tags_pos, tag->datum, (size_t)len);
                tags_pos += len;
            }
        }
        tags_buffer[tags_pos] = '\0';

        si (tags_pos > 0)
        {
            titulus = _chorda_ex_cstr(tags_buffer);
            tabula_pixelorum_pingere_chordam_scalatam(tabula,
                (x + PADDING) * char_lat, linea * char_alt,
                titulus, pixelum_text_dim, scala);
            linea++;
        }
    }

    /* Separator */
    {
        i32 sep_x;
        per (sep_x = 0; sep_x < chars_disponibiles; sep_x++)
        {
            character sep_char[II];
            chorda sep_chorda;

            sep_char[0] = '-';
            sep_char[1] = '\0';
            sep_chorda = _chorda_ex_cstr(sep_char);
            tabula_pixelorum_pingere_chordam_scalatam(tabula,
                (x + PADDING + sep_x) * char_lat, linea * char_alt,
                sep_chorda, pixelum_text_dim, scala);
        }
    }
    linea++;

    /* Summarium cum word wrap */
    si (liber->summarium.mensura > 0)
    {
        i32 pos = 0;
        i32 lineae_max = altitudo - VIII;  /* Reservare spatium pro footer */
        i32 lineae_usae = 0;

        dum (pos < liber->summarium.mensura && lineae_usae < lineae_max)
        {
            character line_buffer[CXXVIII];
            i32 chars_in_line;
            i32 break_pos;
            chorda line_chorda;

            chars_in_line = chars_disponibiles;
            si (pos + chars_in_line > liber->summarium.mensura)
            {
                chars_in_line = liber->summarium.mensura - pos;
            }

            /* Find word boundary */
            break_pos = chars_in_line;
            si (pos + chars_in_line < liber->summarium.mensura)
            {
                i32 i;
                per (i = chars_in_line - 1; i > 0; i--)
                {
                    si ((character)liber->summarium.datum[pos + i] == ' ')
                    {
                        break_pos = i + 1;
                        frange;
                    }
                }
            }

            memcpy(line_buffer, liber->summarium.datum + pos, (size_t)break_pos);
            line_buffer[break_pos] = '\0';
            line_chorda = _chorda_ex_cstr(line_buffer);

            tabula_pixelorum_pingere_chordam_scalatam(tabula,
                (x + PADDING) * char_lat, linea * char_alt,
                line_chorda, pixelum_text, scala);

            pos += break_pos;
            dum (pos < liber->summarium.mensura && (character)liber->summarium.datum[pos] == ' ')
            {
                pos++;
            }

            /* Skip newlines */
            dum (pos < liber->summarium.mensura &&
                 ((character)liber->summarium.datum[pos] == '\n' ||
                  (character)liber->summarium.datum[pos] == '\r'))
            {
                pos++;
            }

            linea++;
            lineae_usae++;
        }
    }

    /* Footer */
    linea = y + altitudo - II;
    {
        character buffer[LXIV];
        sprintf(buffer, "Liber %d/%d   h/l: navigare   Enter: legere   Esc: retro",
            visus->index_paginae + 1, num_libri);
        titulus = _chorda_ex_cstr(buffer);
        tabula_pixelorum_pingere_chordam_scalatam(tabula,
            (x + PADDING) * char_lat, linea * char_alt,
            titulus, pixelum_text_dim, scala);
    }
}


/* ==================================================
 * Reddere - Libro (dettaglio singolo)
 * ================================================== */

hic_manens vacuum
_reddere_libro(
    LibrariumVisus*  visus,
    TabulaPixelorum* tabula,
    i32              x,
    i32              y,
    i32              latitudo,
    i32              altitudo,
    i32              scala)
{
    ContextusDelineandi* ctx;
    i32 char_lat;
    i32 char_alt;
    i32 linea;
    i32 pixelum_text;
    i32 pixelum_text_dim;
    i32 pixelum_accent;
    Color color_background;
    chorda titulus;
    LibrumInfo* liber;
    i32 chars_disponibiles;
    i32 num_libri;

    ctx = delineare_creare_contextum(visus->piscina, tabula);
    si (!ctx)
    {
        redde;
    }

    char_lat = CHAR_LATITUDO * scala;
    char_alt = CHAR_ALTITUDO * scala;

    color_background = thema_color(COLOR_BACKGROUND);
    pixelum_text = color_ad_pixelum(thema_color(COLOR_TEXT));
    pixelum_text_dim = color_ad_pixelum(thema_color(COLOR_TEXT_DIM));
    pixelum_accent = color_ad_pixelum(thema_color(COLOR_ACCENT_PRIMARY));

    /* Pingere fondum */
    delineare_rectangulum_plenum(ctx,
        x * char_lat, y * char_alt,
        latitudo * char_lat, altitudo * char_alt,
        color_background);

    linea = y + I;
    chars_disponibiles = latitudo - (PADDING * II);

    num_libri = visus->libri_filtrati ? (i32)xar_numerus(visus->libri_filtrati) : 0;

    si (visus->liber_currens < 0 || (i32)visus->liber_currens >= num_libri)
    {
        titulus = _chorda_ex_cstr("Liber non selectus");
        tabula_pixelorum_pingere_chordam_scalatam(tabula,
            (x + PADDING) * char_lat, linea * char_alt,
            titulus, pixelum_text_dim, scala);
        redde;
    }

    liber = *(LibrumInfo**)xar_obtinere(visus->libri_filtrati, (i32)visus->liber_currens);
    si (!liber)
    {
        redde;
    }

    /* Titulus */
    tabula_pixelorum_pingere_chordam_scalatam(tabula,
        (x + PADDING) * char_lat, linea * char_alt,
        liber->titulus, pixelum_accent, scala);
    linea += II;

    /* Auctor et Annus */
    {
        character buffer[CXXVIII];
        s32 annus_val = 0;
        i32 k = 0;
        b32 neg = FALSUM;

        si (liber->annus.mensura > 0 && (character)liber->annus.datum[0] == '-')
        {
            neg = VERUM;
            k = 1;
        }
        dum (k < liber->annus.mensura)
        {
            character c = (character)liber->annus.datum[k];
            si (c >= '0' && c <= '9')
            {
                annus_val = annus_val * X + (s32)(c - '0');
            }
            k++;
        }
        si (neg)
        {
            annus_val = -annus_val;
        }

        si (annus_val < 0)
        {
            sprintf(buffer, "%.*s (%d BC)",
                (int)liber->auctor.mensura, (constans character*)liber->auctor.datum,
                -annus_val);
        }
        alioquin
        {
            sprintf(buffer, "%.*s (%d)",
                (int)liber->auctor.mensura, (constans character*)liber->auctor.datum,
                annus_val);
        }
        titulus = _chorda_ex_cstr(buffer);
        tabula_pixelorum_pingere_chordam_scalatam(tabula,
            (x + PADDING) * char_lat, linea * char_alt,
            titulus, pixelum_text, scala);
    }
    linea++;

    /* Gutenberg Numerus */
    si (liber->numerus.mensura > 0)
    {
        character buffer[LXIV];
        sprintf(buffer, "Gutenberg #%.*s",
            (int)liber->numerus.mensura, (constans character*)liber->numerus.datum);
        titulus = _chorda_ex_cstr(buffer);
        tabula_pixelorum_pingere_chordam_scalatam(tabula,
            (x + PADDING) * char_lat, linea * char_alt,
            titulus, pixelum_text_dim, scala);
        linea++;
    }

    /* Tags */
    si (liber->tags && xar_numerus(liber->tags) > 0)
    {
        character buffer[CCLVI];
        i32 pos = 0;
        i32 i;
        i32 num_tags = (i32)xar_numerus(liber->tags);

        pos += (i32)sprintf(buffer + pos, "Tags: ");

        per (i = 0; i < num_tags && pos < CCL; i++)
        {
            chorda* tag = (chorda*)xar_obtinere(liber->tags, i);
            si (tag && tag->mensura > 0)
            {
                si (i > 0)
                {
                    pos += (i32)sprintf(buffer + pos, ", ");
                }
                {
                    i32 len = tag->mensura;
                    si (pos + len > CCL)
                    {
                        len = CCL - pos;
                    }
                    memcpy(buffer + pos, tag->datum, (size_t)len);
                    pos += len;
                }
            }
        }
        buffer[pos] = '\0';

        titulus = _chorda_ex_cstr(buffer);
        tabula_pixelorum_pingere_chordam_scalatam(tabula,
            (x + PADDING) * char_lat, linea * char_alt,
            titulus, pixelum_text_dim, scala);
    }
    linea += II;

    /* Separator */
    {
        i32 sep_x;
        per (sep_x = 0; sep_x < chars_disponibiles; sep_x++)
        {
            character sep_char[II];
            chorda sep_chorda;

            sep_char[0] = '-';
            sep_char[1] = '\0';
            sep_chorda = _chorda_ex_cstr(sep_char);
            tabula_pixelorum_pingere_chordam_scalatam(tabula,
                (x + PADDING + sep_x) * char_lat, linea * char_alt,
                sep_chorda, pixelum_text_dim, scala);
        }
    }
    linea++;

    /* Summarium */
    si (liber->summarium.mensura > 0)
    {
        i32 pos = 0;
        i32 lineae_max = (altitudo - linea + y) - IV;

        dum (pos < liber->summarium.mensura && lineae_max > 0)
        {
            character line_buffer[CXXVIII];
            i32 chars_in_line;
            i32 break_pos;
            chorda line_chorda;

            chars_in_line = chars_disponibiles;
            si (pos + chars_in_line > liber->summarium.mensura)
            {
                chars_in_line = liber->summarium.mensura - pos;
            }

            break_pos = chars_in_line;
            si (pos + chars_in_line < liber->summarium.mensura)
            {
                i32 i;
                per (i = chars_in_line - 1; i > 0; i--)
                {
                    si ((character)liber->summarium.datum[pos + i] == ' ')
                    {
                        break_pos = i + 1;
                        frange;
                    }
                }
            }

            memcpy(line_buffer, liber->summarium.datum + pos, (size_t)break_pos);
            line_buffer[break_pos] = '\0';
            line_chorda = _chorda_ex_cstr(line_buffer);

            tabula_pixelorum_pingere_chordam_scalatam(tabula,
                (x + PADDING) * char_lat, linea * char_alt,
                line_chorda, pixelum_text, scala);

            pos += break_pos;
            dum (pos < liber->summarium.mensura && (character)liber->summarium.datum[pos] == ' ')
            {
                pos++;
            }
            dum (pos < liber->summarium.mensura &&
                 ((character)liber->summarium.datum[pos] == '\n' ||
                  (character)liber->summarium.datum[pos] == '\r'))
            {
                pos++;
            }

            linea++;
            lineae_max--;
        }
    }

    /* Footer */
    linea = y + altitudo - II;
    titulus = _chorda_ex_cstr("Enter: legere   Esc: retro ad catalogum");
    tabula_pixelorum_pingere_chordam_scalatam(tabula,
        (x + PADDING) * char_lat, linea * char_alt,
        titulus, pixelum_text_dim, scala);
}


/* ==================================================
 * Reddere - Lectio
 * ================================================== */

hic_manens vacuum
_reddere_lectio(
    LibrariumVisus*  visus,
    TabulaPixelorum* tabula,
    i32              x,
    i32              y,
    i32              latitudo,
    i32              altitudo,
    i32              scala)
{
    ContextusDelineandi* ctx;
    i32 char_lat;
    i32 char_alt;
    i32 linea;
    i32 pixelum_text;
    i32 pixelum_text_dim;
    i32 pixelum_accent;
    Color color_background;
    chorda titulus;
    LibrumInfo* liber;

    ctx = delineare_creare_contextum(visus->piscina, tabula);
    si (!ctx)
    {
        redde;
    }

    char_lat = CHAR_LATITUDO * scala;
    char_alt = CHAR_ALTITUDO * scala;

    color_background = thema_color(COLOR_BACKGROUND);
    pixelum_text = color_ad_pixelum(thema_color(COLOR_TEXT));
    pixelum_text_dim = color_ad_pixelum(thema_color(COLOR_TEXT_DIM));
    pixelum_accent = color_ad_pixelum(thema_color(COLOR_ACCENT_PRIMARY));

    /* Pingere fondum */
    delineare_rectangulum_plenum(ctx,
        x * char_lat, y * char_alt,
        latitudo * char_lat, altitudo * char_alt,
        color_background);

    linea = y + I;

    /* Calculare paginationem via lector */
    librarium_lector_paginare(visus->lector, (s32)latitudo, (s32)altitudo);

    /* Header: titulus libri */
    si (visus->liber_currens >= 0)
    {
        i32 num_libri = visus->libri_filtrati ? (i32)xar_numerus(visus->libri_filtrati) : 0;
        si ((i32)visus->liber_currens < num_libri)
        {
            liber = *(LibrumInfo**)xar_obtinere(visus->libri_filtrati, (i32)visus->liber_currens);
            si (liber)
            {
                character buffer[CXXVIII];
                i32 tit_len = liber->titulus.mensura;
                si (tit_len > C)
                {
                    tit_len = C;
                }
                memcpy(buffer, liber->titulus.datum, (size_t)tit_len);
                buffer[tit_len] = '\0';

                titulus = _chorda_ex_cstr(buffer);
                tabula_pixelorum_pingere_chordam_scalatam(tabula,
                    (x + PADDING) * char_lat, linea * char_alt,
                    titulus, pixelum_accent, scala);
            }
        }
    }
    linea += II;

    /* Textus */
    si (!librarium_lector_habet_textum(visus->lector))
    {
        titulus = _chorda_ex_cstr("Textus non carcatus");
        tabula_pixelorum_pingere_chordam_scalatam(tabula,
            (x + PADDING) * char_lat, linea * char_alt,
            titulus, pixelum_text_dim, scala);
    }
    alioquin
    {
        PaginariumPagina* pagina;
        i32 numerus_linearum;
        i32 i;
        s32 pagina_currens = librarium_lector_pagina_currens(visus->lector);
        chorda textus_libri = librarium_lector_textus(visus->lector);

        pagina = librarium_lector_pagina_obtinere(visus->lector, pagina_currens);

        si (pagina)
        {
            numerus_linearum = paginarium_pagina_numerus_linearum(pagina);

            per (i = 0; i < numerus_linearum; i++)
            {
                PaginariumLinea* pag_linea;
                chorda linea_reddita;

                pag_linea = paginarium_linea_obtinere(pagina, i);
                si (!pag_linea)
                {
                    perge;
                }

                si (pag_linea->est_vacua)
                {
                    /* Linea vacua - nihil pingere */
                    linea++;
                    perge;
                }

                /* Reddere lineam via paginarium */
                linea_reddita = paginarium_linea_reddere(
                    textus_libri, pag_linea, visus->piscina);

                si (linea_reddita.mensura > 0)
                {
                    tabula_pixelorum_pingere_chordam_scalatam(tabula,
                        (x + PADDING) * char_lat, linea * char_alt,
                        linea_reddita, pixelum_text, scala);
                }

                linea++;
            }
        }
    }

    /* Footer */
    linea = y + altitudo - II;
    {
        character buffer[LXIV];
        s32 pag_curr = librarium_lector_pagina_currens(visus->lector);
        s32 pag_tot = librarium_lector_paginae_totales(visus->lector);
        sprintf(buffer, "Pagina %d/%d   j/k: paginae   Esc: retro",
            pag_curr + 1,
            pag_tot > 0 ? pag_tot : 1);
        titulus = _chorda_ex_cstr(buffer);
        tabula_pixelorum_pingere_chordam_scalatam(tabula,
            (x + PADDING) * char_lat, linea * char_alt,
            titulus, pixelum_text_dim, scala);
    }
}


/* ==================================================
 * Reddere Principale
 * ================================================== */

vacuum
librarium_visus_reddere(
    LibrariumVisus*  visus,
    TabulaPixelorum* tabula,
    s32              x,
    s32              y,
    s32              latitudo,
    s32              altitudo,
    s32              scala,
    b32              focused)
{
    (vacuum)focused;

    si (!visus || !tabula)
    {
        redde;
    }

    /* Cache layout */
    visus->latitudo_characterum = latitudo;
    visus->altitudo_linearum = altitudo;
    visus->scala = scala;
    visus->widget_x = x;
    visus->widget_y = y;

    commutatio (visus->modus)
    {
        casus LIBRARIUM_MODUS_CATEGORIAE:
            _reddere_categoriae(visus, tabula, (i32)x, (i32)y, (i32)latitudo, (i32)altitudo, (i32)scala);
            frange;

        casus LIBRARIUM_MODUS_TAGS:
            _reddere_tags(visus, tabula, (i32)x, (i32)y, (i32)latitudo, (i32)altitudo, (i32)scala);
            frange;

        casus LIBRARIUM_MODUS_CATALOGO:
            si (visus->categoria == LIBRARIUM_CAT_PER_TITULUM)
            {
                _reddere_catalogo_compacto(visus, tabula, (i32)x, (i32)y, (i32)latitudo, (i32)altitudo, (i32)scala);
            }
            alioquin
            {
                _reddere_catalogo_plenus(visus, tabula, (i32)x, (i32)y, (i32)latitudo, (i32)altitudo, (i32)scala);
            }
            frange;

        casus LIBRARIUM_MODUS_LIBRO:
            _reddere_libro(visus, tabula, (i32)x, (i32)y, (i32)latitudo, (i32)altitudo, (i32)scala);
            frange;

        casus LIBRARIUM_MODUS_LECTIO:
            _reddere_lectio(visus, tabula, (i32)x, (i32)y, (i32)latitudo, (i32)altitudo, (i32)scala);
            frange;

        ordinarius:
            frange;
    }
}


/* ==================================================
 * Carcare Textus Libri (via Lector)
 * ================================================== */

hic_manens vacuum
_librarium_visus_carcare_textum(
    LibrariumVisus* visus)
{
    LibrumInfo* liber;
    s32 num_libri;
    character via_buffer[DXII];
    character numerus_buffer[XXXII];

    num_libri = visus->libri_filtrati ? (s32)xar_numerus(visus->libri_filtrati) : 0;

    si (visus->liber_currens < 0 || visus->liber_currens >= num_libri)
    {
        redde;
    }

    liber = *(LibrumInfo**)xar_obtinere(visus->libri_filtrati, (i32)visus->liber_currens);
    si (!liber || liber->via.mensura == 0)
    {
        redde;
    }

    /* Costruire via completa */
    {
        s32 len = (s32)liber->via.mensura;
        si (len > DX)
        {
            len = DX;
        }
        memcpy(via_buffer, liber->via.datum, (size_t)len);
        via_buffer[len] = '\0';
    }

    /* Costruire numerus (ID) */
    {
        s32 len = (s32)liber->numerus.mensura;
        si (len > XXX)
        {
            len = XXX;
        }
        memcpy(numerus_buffer, liber->numerus.datum, (size_t)len);
        numerus_buffer[len] = '\0';
    }

    /* Carcare via lector */
    librarium_lector_carcare(visus->lector, via_buffer, numerus_buffer);

    /* Carcare progressum salvatum */
    librarium_lector_carcare_progressum(visus->lector, visus->ctx->repo, visus->ctx->intern);
}


/* ==================================================
 * Eventus
 * ================================================== */

b32
librarium_visus_tractare_eventum(
    LibrariumVisus*   visus,
    constans Eventus* eventus)
{
    si (!visus || !eventus)
    {
        redde FALSUM;
    }

    si (eventus->genus != EVENTUS_CLAVIS_DEPRESSUS)
    {
        redde FALSUM;
    }

    commutatio (visus->modus)
    {
        casus LIBRARIUM_MODUS_CATEGORIAE:
        {
            commutatio (eventus->datum.clavis.typus)
            {
                casus 'j':
                    visus->index_selecta++;
                    si (visus->index_selecta > II)
                    {
                        visus->index_selecta = II;
                    }
                    redde VERUM;

                casus 'k':
                    visus->index_selecta--;
                    si (visus->index_selecta < 0)
                    {
                        visus->index_selecta = 0;
                    }
                    redde VERUM;

                ordinarius:
                    frange;
            }

            si (eventus->datum.clavis.clavis == CLAVIS_REDITUS)
            {
                /* Activare optio selecta */
                commutatio (visus->index_selecta)
                {
                    casus 0:  /* Omnes */
                        visus->categoria = LIBRARIUM_CAT_OMNES;
                        visus->ordo = LIBRARIUM_ORDO_CHRONOLOGICUS;
                        visus->filtrum_tag.mensura = 0;
                        _librarium_visus_filtrare(visus);
                        visus->index_paginae = 0;
                        visus->modus = LIBRARIUM_MODUS_CATALOGO;
                        frange;

                    casus 1:  /* Per Titulum */
                        visus->categoria = LIBRARIUM_CAT_PER_TITULUM;
                        visus->ordo = LIBRARIUM_ORDO_ALPHABETICUS;
                        visus->filtrum_tag.mensura = 0;
                        _librarium_visus_filtrare(visus);
                        visus->index_paginae = 0;
                        visus->index_selecta = 0;
                        visus->modus = LIBRARIUM_MODUS_CATALOGO;
                        frange;

                    casus 2:  /* Per Tag */
                        visus->index_paginae = 0;
                        visus->index_selecta = 0;
                        visus->modus = LIBRARIUM_MODUS_TAGS;
                        frange;

                    ordinarius:
                        frange;
                }
                redde VERUM;
            }

            si (eventus->datum.clavis.clavis == CLAVIS_EFFUGIUM)
            {
                redde FALSUM;  /* Exit widget */
            }
            frange;
        }

        casus LIBRARIUM_MODUS_TAGS:
        {
            s32 num_tags = visus->tags_unici ? (s32)xar_numerus(visus->tags_unici) : 0;
            s32 tags_per_pagina = visus->altitudo_linearum - VIII;
            s32 num_paginae;
            s32 tags_in_pagina;

            si (tags_per_pagina < 1)
            {
                tags_per_pagina = 1;
            }

            num_paginae = (num_tags + tags_per_pagina - 1) / tags_per_pagina;
            si (num_paginae < 1)
            {
                num_paginae = 1;
            }

            tags_in_pagina = num_tags - (visus->index_paginae * tags_per_pagina);
            si (tags_in_pagina > tags_per_pagina)
            {
                tags_in_pagina = tags_per_pagina;
            }
            si (tags_in_pagina < 0)
            {
                tags_in_pagina = 0;
            }

            commutatio (eventus->datum.clavis.typus)
            {
                casus 'j':
                    visus->index_selecta++;
                    si (visus->index_selecta >= tags_in_pagina)
                    {
                        visus->index_selecta = tags_in_pagina - 1;
                    }
                    si (visus->index_selecta < 0)
                    {
                        visus->index_selecta = 0;
                    }
                    redde VERUM;

                casus 'k':
                    visus->index_selecta--;
                    si (visus->index_selecta < 0)
                    {
                        visus->index_selecta = 0;
                    }
                    redde VERUM;

                casus 'h':
                    si (visus->index_paginae > 0)
                    {
                        visus->index_paginae--;
                        visus->index_selecta = 0;
                    }
                    redde VERUM;

                casus 'l':
                    si (visus->index_paginae < num_paginae - 1)
                    {
                        visus->index_paginae++;
                        visus->index_selecta = 0;
                    }
                    redde VERUM;

                ordinarius:
                    frange;
            }

            si (eventus->datum.clavis.clavis == CLAVIS_REDITUS)
            {
                /* Selectare tag */
                s32 tag_idx = visus->index_paginae * tags_per_pagina + visus->index_selecta;
                si (tag_idx < num_tags)
                {
                    chorda* tag = (chorda*)xar_obtinere(visus->tags_unici, (i32)tag_idx);
                    si (tag)
                    {
                        visus->filtrum_tag = *tag;
                        visus->categoria = LIBRARIUM_CAT_PER_TAG;
                        visus->ordo = LIBRARIUM_ORDO_CHRONOLOGICUS;
                        _librarium_visus_filtrare(visus);
                        visus->index_paginae = 0;
                        visus->modus = LIBRARIUM_MODUS_CATALOGO;
                    }
                }
                redde VERUM;
            }

            si (eventus->datum.clavis.clavis == CLAVIS_EFFUGIUM)
            {
                visus->modus = LIBRARIUM_MODUS_CATEGORIAE;
                visus->index_selecta = II;  /* Per Tag */
                redde VERUM;
            }
            frange;
        }

        casus LIBRARIUM_MODUS_CATALOGO:
        {
            s32 num_libri = visus->libri_filtrati ? (s32)xar_numerus(visus->libri_filtrati) : 0;

            si (visus->categoria == LIBRARIUM_CAT_PER_TITULUM)
            {
                /* Modo compacto */
                s32 libri_per_pagina = visus->altitudo_linearum - VI;
                s32 num_paginae;
                s32 libri_in_pagina;

                si (libri_per_pagina < 1)
                {
                    libri_per_pagina = 1;
                }

                num_paginae = (num_libri + libri_per_pagina - 1) / libri_per_pagina;
                si (num_paginae < 1)
                {
                    num_paginae = 1;
                }

                libri_in_pagina = num_libri - (visus->index_paginae * libri_per_pagina);
                si (libri_in_pagina > libri_per_pagina)
                {
                    libri_in_pagina = libri_per_pagina;
                }
                si (libri_in_pagina < 0)
                {
                    libri_in_pagina = 0;
                }

                commutatio (eventus->datum.clavis.typus)
                {
                    casus 'j':
                        visus->index_selecta++;
                        si (visus->index_selecta >= libri_in_pagina)
                        {
                            visus->index_selecta = libri_in_pagina - 1;
                        }
                        si (visus->index_selecta < 0)
                        {
                            visus->index_selecta = 0;
                        }
                        redde VERUM;

                    casus 'k':
                        visus->index_selecta--;
                        si (visus->index_selecta < 0)
                        {
                            visus->index_selecta = 0;
                        }
                        redde VERUM;

                    casus 'l':
                        si (visus->index_paginae < num_paginae - 1)
                        {
                            visus->index_paginae++;
                            visus->index_selecta = 0;
                        }
                        redde VERUM;

                    casus 'h':
                        si (visus->index_paginae > 0)
                        {
                            visus->index_paginae--;
                            visus->index_selecta = 0;
                        }
                        redde VERUM;

                    ordinarius:
                        frange;
                }

                si (eventus->datum.clavis.clavis == CLAVIS_REDITUS)
                {
                    /* Aperire libro */
                    s32 liber_idx = visus->index_paginae * libri_per_pagina + visus->index_selecta;
                    si (liber_idx >= 0 && liber_idx < num_libri)
                    {
                        visus->liber_currens = liber_idx;
                        visus->modus = LIBRARIUM_MODUS_LIBRO;
                    }
                    redde VERUM;
                }
            }
            alioquin
            {
                /* Modo plenus (uno per pagina) */
                commutatio (eventus->datum.clavis.typus)
                {
                    casus 'l':
                    casus 'j':
                        si (visus->index_paginae < num_libri - 1)
                        {
                            visus->index_paginae++;
                        }
                        redde VERUM;

                    casus 'h':
                    casus 'k':
                        si (visus->index_paginae > 0)
                        {
                            visus->index_paginae--;
                        }
                        redde VERUM;

                    ordinarius:
                        frange;
                }

                si (eventus->datum.clavis.clavis == CLAVIS_REDITUS)
                {
                    /* Aperire libro directe ad lectio */
                    si (visus->index_paginae >= 0 && visus->index_paginae < num_libri)
                    {
                        visus->liber_currens = visus->index_paginae;
                        _librarium_visus_carcare_textum(visus);
                        visus->modus = LIBRARIUM_MODUS_LECTIO;
                    }
                    redde VERUM;
                }
            }

            si (eventus->datum.clavis.clavis == CLAVIS_EFFUGIUM)
            {
                /* Retro ad categoriae */
                visus->modus = LIBRARIUM_MODUS_CATEGORIAE;
                visus->index_selecta = 0;
                visus->filtrum_tag.mensura = 0;
                visus->filtrum_quaestio.mensura = 0;
                redde VERUM;
            }
            frange;
        }

        casus LIBRARIUM_MODUS_LIBRO:
        {
            si (eventus->datum.clavis.clavis == CLAVIS_REDITUS)
            {
                /* Incipit lectio */
                _librarium_visus_carcare_textum(visus);
                visus->modus = LIBRARIUM_MODUS_LECTIO;
                redde VERUM;
            }

            si (eventus->datum.clavis.clavis == CLAVIS_EFFUGIUM)
            {
                /* Retro ad catalogo */
                visus->modus = LIBRARIUM_MODUS_CATALOGO;
                redde VERUM;
            }
            frange;
        }

        casus LIBRARIUM_MODUS_LECTIO:
        {
            commutatio (eventus->datum.clavis.typus)
            {
                casus 'j':
                    librarium_lector_pagina_proxima(visus->lector);
                    librarium_lector_salvare_progressum(visus->lector, visus->ctx->repo);
                    redde VERUM;

                casus 'k':
                    librarium_lector_pagina_prior(visus->lector);
                    librarium_lector_salvare_progressum(visus->lector, visus->ctx->repo);
                    redde VERUM;

                ordinarius:
                    frange;
            }

            si (eventus->datum.clavis.clavis == CLAVIS_EFFUGIUM)
            {
                /* Salvare progressum ante exire */
                librarium_lector_salvare_progressum(visus->lector, visus->ctx->repo);

                /* Retro ad catalogo */
                visus->index_paginae = visus->liber_currens;
                visus->modus = LIBRARIUM_MODUS_CATALOGO;
                redde VERUM;
            }
            frange;
        }

        ordinarius:
            frange;
    }

    redde FALSUM;
}


/* ==================================================
 * Quaerere
 * ================================================== */

vacuum
librarium_visus_quaerere(
    LibrariumVisus*     visus,
    constans character* quaestio)
{
    s32 num_inventum;

    si (!visus || !quaestio)
    {
        redde;
    }

    /* Ponere filtrum - internare quaestio pro stabilitate memoriae */
    visus->filtrum_quaestio = *chorda_internare_ex_literis(visus->ctx->intern, quaestio);
    visus->filtrum_tag.mensura = 0;
    visus->ordo = LIBRARIUM_ORDO_ALPHABETICUS;

    /* Filtrare */
    _librarium_visus_filtrare(visus);

    num_inventum = visus->libri_filtrati ? (s32)xar_numerus(visus->libri_filtrati) : 0;

    si (num_inventum == 1)
    {
        /* Uno solo - monstra in catalogo plenus */
        visus->categoria = LIBRARIUM_CAT_OMNES;
        visus->index_paginae = 0;
        visus->modus = LIBRARIUM_MODUS_CATALOGO;
    }
    alioquin si (num_inventum > 1)
    {
        /* Plures - monstra catalogo compacto */
        visus->categoria = LIBRARIUM_CAT_PER_TITULUM;
        visus->index_paginae = 0;
        visus->index_selecta = 0;
        visus->modus = LIBRARIUM_MODUS_CATALOGO;
    }
    alioquin
    {
        /* Nihil inventum - reset */
        visus->filtrum_quaestio.mensura = 0;
        _librarium_visus_filtrare(visus);
        visus->modus = LIBRARIUM_MODUS_CATEGORIAE;
    }
}

vacuum
librarium_visus_reset_ad_radix(
    LibrariumVisus* visus)
{
    si (!visus)
    {
        redde;
    }

    /* Reset ad modus categoriae */
    visus->modus = LIBRARIUM_MODUS_CATEGORIAE;
    visus->index_selecta = 0;
    visus->index_paginae = 0;
    visus->filtrum_tag.mensura = 0;
    visus->filtrum_quaestio.mensura = 0;
    visus->liber_currens = -1;

    /* Re-filtrare cum categoria currens */
    _librarium_visus_filtrare(visus);
}


/* ==================================================
 * Lifecycle (Init / Status)
 * ================================================== */

/* Command handler pro $library */
hic_manens b32
_librarium_command_handler(
    ContextusCommandi* ctx)
{
    ContextusWidget* widget_ctx;
    character argumentum[LXIV];
    i32 col;
    i32 idx;
    character c;

    widget_ctx = (ContextusWidget*)ctx->datum_registratus;
    si (!widget_ctx || !widget_ctx->commutare_widget)
    {
        redde FALSUM;
    }

    /* Legere argumentum post commandum (skip leading space) */
    idx = ZEPHYRUM;
    per (col = ctx->columna; col < ctx->pagina->tabula.latitudo && idx < LX; col++)
    {
        c = tabula_cellula(&ctx->pagina->tabula, ctx->linea, col);

        /* Skip leading spaces */
        si (idx == ZEPHYRUM && c == ' ')
        {
            perge;
        }

        /* Stop at end of line or null */
        si (c == '\0' || c == '\n')
        {
            frange;
        }

        argumentum[idx++] = c;
    }
    argumentum[idx] = '\0';

    /* Vocare callback ad commutare */
    widget_ctx->commutare_widget(
        widget_ctx->schirmata_datum,
        "librarium",
        idx > ZEPHYRUM ? argumentum : NIHIL);

    redde VERUM;
}

vacuum
librarium_visus_init(
    ContextusWidget* ctx)
{
    si (!ctx || !ctx->reg_commandi)
    {
        redde;
    }

    registrum_commandi_registrare(
        ctx->reg_commandi,
        "library",
        _librarium_command_handler,
        ctx);
}

vacuum
librarium_visus_salvare_status(
    LibrariumVisus*      visus,
    EntitasRepositorium* repo,
    constans character*  entitas_id)
{
    Entitas* entitas;
    character valor[XXXII];

    si (!visus || !repo || !entitas_id)
    {
        redde;
    }

    /* Scaffoldare entitas (creat si non existit) */
    entitas = repo->entitas_scaffoldare(repo->datum, "LibrariumStatus", entitas_id);
    si (!entitas)
    {
        redde;
    }

    /* Salvare modus */
    sprintf(valor, "%d", (int)visus->modus);
    repo->proprietas_ponere(repo->datum, entitas, "modus", valor);

    /* Salvare categoria */
    sprintf(valor, "%d", (int)visus->categoria);
    repo->proprietas_ponere(repo->datum, entitas, "categoria", valor);

    /* Salvare index_paginae */
    sprintf(valor, "%d", visus->index_paginae);
    repo->proprietas_ponere(repo->datum, entitas, "index_paginae", valor);

    /* Salvare index_selecta */
    sprintf(valor, "%d", visus->index_selecta);
    repo->proprietas_ponere(repo->datum, entitas, "index_selecta", valor);

    /* Salvare liber_currens */
    sprintf(valor, "%d", visus->liber_currens);
    repo->proprietas_ponere(repo->datum, entitas, "liber_currens", valor);

    /* Salvare pagina_lectio (ex lector) */
    sprintf(valor, "%d", librarium_lector_pagina_currens(visus->lector));
    repo->proprietas_ponere(repo->datum, entitas, "pagina_lectio", valor);

    /* Salvare filtrum_tag */
    si (visus->filtrum_tag.mensura > 0)
    {
        character tag_buf[CXXVIII];
        i32 len = visus->filtrum_tag.mensura;
        si (len > CXXVII)
        {
            len = CXXVII;
        }
        memcpy(tag_buf, visus->filtrum_tag.datum, (size_t)len);
        tag_buf[len] = '\0';
        repo->proprietas_ponere(repo->datum, entitas, "filtrum_tag", tag_buf);
    }
    alioquin
    {
        repo->proprietas_ponere(repo->datum, entitas, "filtrum_tag", "");
    }

    /* Salvare filtrum_quaestio */
    si (visus->filtrum_quaestio.mensura > 0)
    {
        character quaestio_buf[CXXVIII];
        i32 len = visus->filtrum_quaestio.mensura;
        si (len > CXXVII)
        {
            len = CXXVII;
        }
        memcpy(quaestio_buf, visus->filtrum_quaestio.datum, (size_t)len);
        quaestio_buf[len] = '\0';
        repo->proprietas_ponere(repo->datum, entitas, "filtrum_quaestio", quaestio_buf);
    }
    alioquin
    {
        repo->proprietas_ponere(repo->datum, entitas, "filtrum_quaestio", "");
    }

    /* Salvare ordo */
    sprintf(valor, "%d", (int)visus->ordo);
    repo->proprietas_ponere(repo->datum, entitas, "ordo", valor);
}

vacuum
librarium_visus_carcare_status(
    LibrariumVisus*      visus,
    EntitasRepositorium* repo,
    constans character*  entitas_id)
{
    Entitas* entitas;
    chorda* valor;
    chorda* clavis;

    si (!visus || !repo || !entitas_id)
    {
        redde;
    }

    /* Scaffoldare entitas */
    entitas = repo->entitas_scaffoldare(repo->datum, "LibrariumStatus", entitas_id);
    si (!entitas)
    {
        redde;
    }

    /* Carcare modus */
    clavis = chorda_internare_ex_literis(visus->ctx->intern, "modus");
    valor = entitas_proprietas_capere(entitas, clavis);
    si (valor)
    {
        s32 temp;
        si (chorda_ut_s32(*valor, &temp))
        {
            visus->modus = (LibrariumModus)temp;
        }
    }

    /* Carcare categoria */
    clavis = chorda_internare_ex_literis(visus->ctx->intern, "categoria");
    valor = entitas_proprietas_capere(entitas, clavis);
    si (valor)
    {
        s32 temp;
        si (chorda_ut_s32(*valor, &temp))
        {
            visus->categoria = (LibrariumCategoria)temp;
        }
    }

    /* Carcare index_paginae */
    clavis = chorda_internare_ex_literis(visus->ctx->intern, "index_paginae");
    valor = entitas_proprietas_capere(entitas, clavis);
    si (valor)
    {
        s32 temp;
        si (chorda_ut_s32(*valor, &temp))
        {
            visus->index_paginae = temp;
        }
    }

    /* Carcare index_selecta */
    clavis = chorda_internare_ex_literis(visus->ctx->intern, "index_selecta");
    valor = entitas_proprietas_capere(entitas, clavis);
    si (valor)
    {
        s32 temp;
        si (chorda_ut_s32(*valor, &temp))
        {
            visus->index_selecta = temp;
        }
    }

    /* Carcare liber_currens */
    clavis = chorda_internare_ex_literis(visus->ctx->intern, "liber_currens");
    valor = entitas_proprietas_capere(entitas, clavis);
    si (valor)
    {
        s32 temp;
        si (chorda_ut_s32(*valor, &temp))
        {
            visus->liber_currens = temp;
        }
    }

    /* Carcare filtrum_tag */
    clavis = chorda_internare_ex_literis(visus->ctx->intern, "filtrum_tag");
    valor = entitas_proprietas_capere(entitas, clavis);
    si (valor && valor->mensura > ZEPHYRUM)
    {
        /* Internare filtrum_tag pro stabilitate memoriae */
        visus->filtrum_tag = *chorda_internare(visus->ctx->intern, *valor);
    }
    alioquin
    {
        visus->filtrum_tag.mensura = 0;
    }

    /* Carcare filtrum_quaestio */
    clavis = chorda_internare_ex_literis(visus->ctx->intern, "filtrum_quaestio");
    valor = entitas_proprietas_capere(entitas, clavis);
    si (valor && valor->mensura > ZEPHYRUM)
    {
        /* Internare filtrum_quaestio pro stabilitate memoriae */
        visus->filtrum_quaestio = *chorda_internare(visus->ctx->intern, *valor);
    }
    alioquin
    {
        visus->filtrum_quaestio.mensura = 0;
    }

    /* Carcare ordo */
    clavis = chorda_internare_ex_literis(visus->ctx->intern, "ordo");
    valor = entitas_proprietas_capere(entitas, clavis);
    si (valor)
    {
        s32 temp;
        si (chorda_ut_s32(*valor, &temp))
        {
            visus->ordo = (LibrariumOrdo)temp;
        }
    }

    /* Carcare pagina_lectio (temporarius) */
    {
        s32 pagina_lectio_salvata = 0;
        clavis = chorda_internare_ex_literis(visus->ctx->intern, "pagina_lectio");
        valor = entitas_proprietas_capere(entitas, clavis);
        si (valor)
        {
            chorda_ut_s32(*valor, &pagina_lectio_salvata);
        }

        /* Re-filtrare post carcare categoria */
        _librarium_visus_filtrare(visus);

        /* Si in modus lectio, carcare textum */
        si (visus->modus == LIBRARIUM_MODUS_LECTIO)
        {
            s32 num_libri;

            num_libri = visus->libri_filtrati ? (s32)xar_numerus(visus->libri_filtrati) : 0;

            si (visus->liber_currens >= 0 && visus->liber_currens < num_libri)
            {
                _librarium_visus_carcare_textum(visus);
                /* Ponere pagina ex status salvata */
                librarium_lector_pagina_saltare(visus->lector, pagina_lectio_salvata);
            }
            alioquin
            {
                /* Index invalido - reset ad categoriae */
                visus->modus = LIBRARIUM_MODUS_CATEGORIAE;
                visus->liber_currens = -1;
            }
        }
    }
}
