#include "librarium_visus.h"
#include "thema.h"
#include "color.h"
#include "delineare.h"
#include "chorda.h"
#include "stml.h"
#include "filum.h"
#include "internamentum.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>

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
    visus->textus_libri.mensura = 0;
    visus->pagina_lectio = 0;
    visus->paginae_totales = 0;
    visus->cache_liber = -1;
    visus->cache_latitudo = 0;
    visus->cache_altitudo = 0;
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

    /* Instructiones */
    linea = y + altitudo - II;
    titulus = _chorda_ex_cstr("j/k: selectare   Enter: filtrare   Esc: retro");
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
 * Calculare Paginationem Lectionis
 * ================================================== */

hic_manens vacuum
_librarium_visus_calculare_paginationem(
    LibrariumVisus* visus,
    i32             latitudo,
    i32             altitudo)
{
    i32 chars_disponibiles;
    i32 lineae_disponibiles;
    i32 pos;
    i32 pagina_idx;

    /* Check cache */
    si (visus->cache_liber == visus->liber_currens &&
        visus->cache_latitudo == (s32)latitudo &&
        visus->cache_altitudo == (s32)altitudo)
    {
        redde;
    }

    visus->cache_liber = visus->liber_currens;
    visus->cache_latitudo = (s32)latitudo;
    visus->cache_altitudo = (s32)altitudo;

    si (visus->textus_libri.mensura == 0)
    {
        visus->paginae_totales = 0;
        redde;
    }

    chars_disponibiles = latitudo - (PADDING * II);
    lineae_disponibiles = altitudo - IV;  /* Header et footer */

    si (chars_disponibiles < X)
    {
        chars_disponibiles = X;
    }
    si (lineae_disponibiles < V)
    {
        lineae_disponibiles = V;
    }

    /* Prima pagina incipit a 0 */
    visus->paginae_limites[0] = 0;
    pagina_idx = 1;
    pos = 0;

    dum (pos < visus->textus_libri.mensura && pagina_idx < LIBRARIUM_PAGINAE_MAX)
    {
        i32 lineae_usae = 0;

        dum (pos < visus->textus_libri.mensura && lineae_usae < lineae_disponibiles)
        {
            i32 chars_in_line;
            i32 break_pos;

            chars_in_line = chars_disponibiles;
            si (pos + chars_in_line > visus->textus_libri.mensura)
            {
                chars_in_line = visus->textus_libri.mensura - pos;
            }

            /* Check for newline */
            break_pos = chars_in_line;
            {
                i32 i;
                per (i = 0; i < chars_in_line; i++)
                {
                    character c = (character)visus->textus_libri.datum[pos + i];
                    si (c == '\n')
                    {
                        break_pos = i + 1;
                        frange;
                    }
                }
            }

            /* Find word boundary if not newline */
            si (break_pos == chars_in_line && pos + chars_in_line < visus->textus_libri.mensura)
            {
                i32 i;
                per (i = chars_in_line - 1; i > 0; i--)
                {
                    si ((character)visus->textus_libri.datum[pos + i] == ' ')
                    {
                        break_pos = i + 1;
                        frange;
                    }
                }
            }

            pos += break_pos;
            lineae_usae++;
        }

        si (pos < visus->textus_libri.mensura)
        {
            visus->paginae_limites[pagina_idx] = (s32)pos;
            pagina_idx++;
        }
    }

    visus->paginae_totales = (s32)pagina_idx;
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
    i32 chars_disponibiles;
    i32 lineae_disponibiles;
    i32 pos;
    i32 finis;
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
    chars_disponibiles = latitudo - (PADDING * II);
    lineae_disponibiles = altitudo - IV;

    /* Calculare paginationem */
    _librarium_visus_calculare_paginationem(visus, latitudo, altitudo);

    /* Clamp pagina */
    si (visus->pagina_lectio >= visus->paginae_totales)
    {
        visus->pagina_lectio = visus->paginae_totales - 1;
    }
    si (visus->pagina_lectio < 0)
    {
        visus->pagina_lectio = 0;
    }

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
    si (visus->textus_libri.mensura == 0)
    {
        titulus = _chorda_ex_cstr("Textus non carcatus");
        tabula_pixelorum_pingere_chordam_scalatam(tabula,
            (x + PADDING) * char_lat, linea * char_alt,
            titulus, pixelum_text_dim, scala);
    }
    alioquin
    {
        i32 lineae_usae = 0;

        pos = (i32)visus->paginae_limites[visus->pagina_lectio];
        finis = visus->pagina_lectio + 1 < visus->paginae_totales
            ? (i32)visus->paginae_limites[visus->pagina_lectio + 1]
            : visus->textus_libri.mensura;

        dum (pos < finis && lineae_usae < lineae_disponibiles)
        {
            character line_buffer[CXXVIII];
            i32 chars_in_line;
            i32 break_pos;
            chorda line_chorda;

            chars_in_line = chars_disponibiles;
            si (pos + chars_in_line > finis)
            {
                chars_in_line = finis - pos;
            }

            /* Check for newline */
            break_pos = chars_in_line;
            {
                i32 i;
                per (i = 0; i < chars_in_line; i++)
                {
                    character c = (character)visus->textus_libri.datum[pos + i];
                    si (c == '\n')
                    {
                        break_pos = i;
                        frange;
                    }
                }
            }

            si (break_pos > 0)
            {
                memcpy(line_buffer, visus->textus_libri.datum + pos, (size_t)break_pos);
                line_buffer[break_pos] = '\0';
                line_chorda = _chorda_ex_cstr(line_buffer);

                tabula_pixelorum_pingere_chordam_scalatam(tabula,
                    (x + PADDING) * char_lat, linea * char_alt,
                    line_chorda, pixelum_text, scala);
            }

            /* Avanzare pos */
            si ((character)visus->textus_libri.datum[pos + break_pos] == '\n')
            {
                pos += break_pos + 1;
            }
            alioquin
            {
                pos += break_pos;
                dum (pos < finis && (character)visus->textus_libri.datum[pos] == ' ')
                {
                    pos++;
                }
            }

            linea++;
            lineae_usae++;
        }
    }

    /* Footer */
    linea = y + altitudo - II;
    {
        character buffer[LXIV];
        sprintf(buffer, "Pagina %d/%d   j/k: paginae   Esc: retro",
            visus->pagina_lectio + 1,
            visus->paginae_totales > 0 ? visus->paginae_totales : 1);
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
 * Carcare Textus Libri
 * ================================================== */

hic_manens vacuum
_librarium_visus_carcare_textum(
    LibrariumVisus* visus)
{
    LibrumInfo* liber;
    s32 num_libri;
    character via_buffer[DXII];

    num_libri = visus->libri_filtrati ? (s32)xar_numerus(visus->libri_filtrati) : 0;

    si (visus->liber_currens < 0 || visus->liber_currens >= num_libri)
    {
        visus->textus_libri.mensura = 0;
        redde;
    }

    liber = *(LibrumInfo**)xar_obtinere(visus->libri_filtrati, (i32)visus->liber_currens);
    si (!liber || liber->via.mensura == 0)
    {
        visus->textus_libri.mensura = 0;
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

    /* Legere filum */
    visus->textus_libri = filum_legere_totum(via_buffer, visus->piscina);
    visus->pagina_lectio = 0;
    visus->cache_liber = -1;  /* Force recalc */
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
            s32 tags_in_pagina;

            si (tags_per_pagina < 1)
            {
                tags_per_pagina = 1;
            }

            tags_in_pagina = num_tags - (visus->index_paginae * tags_per_pagina);
            si (tags_in_pagina > tags_per_pagina)
            {
                tags_in_pagina = tags_per_pagina;
            }

            commutatio (eventus->datum.clavis.typus)
            {
                casus 'j':
                    visus->index_selecta++;
                    si (visus->index_selecta >= tags_in_pagina)
                    {
                        visus->index_selecta = tags_in_pagina - 1;
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
                    /* Aperire libro */
                    si (visus->index_paginae >= 0 && visus->index_paginae < num_libri)
                    {
                        visus->liber_currens = visus->index_paginae;
                        visus->modus = LIBRARIUM_MODUS_LIBRO;
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
                    si (visus->pagina_lectio < visus->paginae_totales - 1)
                    {
                        visus->pagina_lectio++;
                    }
                    redde VERUM;

                casus 'k':
                    si (visus->pagina_lectio > 0)
                    {
                        visus->pagina_lectio--;
                    }
                    redde VERUM;

                ordinarius:
                    frange;
            }

            si (eventus->datum.clavis.clavis == CLAVIS_EFFUGIUM)
            {
                /* Retro ad libro */
                visus->modus = LIBRARIUM_MODUS_LIBRO;
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

    /* Ponere filtrum */
    visus->filtrum_quaestio = _chorda_ex_cstr(quaestio);
    visus->filtrum_tag.mensura = 0;
    visus->ordo = LIBRARIUM_ORDO_ALPHABETICUS;

    /* Filtrare */
    _librarium_visus_filtrare(visus);

    num_inventum = visus->libri_filtrati ? (s32)xar_numerus(visus->libri_filtrati) : 0;

    si (num_inventum == 1)
    {
        /* Uno solo - salta ad LIBRO */
        visus->liber_currens = 0;
        visus->modus = LIBRARIUM_MODUS_LIBRO;
    }
    alioquin si (num_inventum > 1)
    {
        /* Plures - monstra catalogo */
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
