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

/* Verificare si proprietas est "contentum"
 * Redde: VERUM si clavis == "contentum"
 */
interior b32
_est_contentum(
    Proprietas* prop)
{
    hic_manens i8 contentum_lit[] = "contentum";

    si (!prop || !prop->clavis || !prop->clavis->datum)
    {
        redde FALSUM;
    }

    si (prop->clavis->mensura != IX)
    {
        redde FALSUM;
    }

    redde memcmp(prop->clavis->datum, contentum_lit, IX) == ZEPHYRUM;
}

/* Sanitizare contentum valorem pro navigator display
 * - Praetermittere spatium initiale
 * - Substituere newlines cum spatiis
 * - Truncare ad max_mensura
 * - Si totum est spatium, ponere "[vacuum]"
 *
 * buffer:      destinatio buffer
 * max_mensura: magnitudo buffer (includit '\0')
 * valor:       chorda valor originalis
 *
 * Redde: mensura scripti (sine '\0')
 */
interior i32
_sanitizare_contentum(
    character* buffer,
    i32        max_mensura,
    chorda*    valor)
{
    i32 i;
    i32 initium;
    i32 mensura_output;
    i32 max_output;
    i8  c;
    b32 habet_non_spatium;

    si (!buffer || max_mensura < II)
    {
        redde ZEPHYRUM;
    }

    /* Si valor nihil, ponere [vacuum] */
    si (!valor || !valor->datum || valor->mensura == ZEPHYRUM)
    {
        hic_manens i8 vacuum_lit[] = "[vacuum]";
        i32 len;
        len = VIII;  /* "[vacuum]" = 8 */
        si (len >= max_mensura)
        {
            len = max_mensura - I;
        }
        memcpy(buffer, vacuum_lit, (memoriae_index)len);
        buffer[len] = '\0';
        redde len;
    }

    /* Praetermittere spatium initiale */
    initium = ZEPHYRUM;
    dum (initium < valor->mensura)
    {
        c = valor->datum[initium];
        si (c != ' ' && c != '\t' && c != '\n' && c != '\r')
        {
            frange;
        }
        initium++;
    }

    /* Verificare si totum est spatium */
    si (initium >= valor->mensura)
    {
        hic_manens i8 vacuum_lit[] = "[vacuum]";
        i32 len;
        len = VIII;
        si (len >= max_mensura)
        {
            len = max_mensura - I;
        }
        memcpy(buffer, vacuum_lit, (memoriae_index)len);
        buffer[len] = '\0';
        redde len;
    }

    /* Copiare et sanitizare */
    max_output = max_mensura - I;  /* Reservare pro '\0' */
    si (max_output > L)  /* Truncare ad ~50 characteres */
    {
        max_output = L;
    }

    mensura_output = ZEPHYRUM;
    habet_non_spatium = FALSUM;

    per (i = initium; i < valor->mensura && mensura_output < max_output; i++)
    {
        c = valor->datum[i];

        /* Substituere newlines et tabs cum spatiis */
        si (c == '\n' || c == '\r' || c == '\t')
        {
            /* Addere spatium solum si non iam spatium */
            si (mensura_output > ZEPHYRUM && buffer[mensura_output - I] != ' ')
            {
                buffer[mensura_output++] = ' ';
            }
        }
        alioquin
        {
            buffer[mensura_output++] = (character)c;
            si (c != ' ')
            {
                habet_non_spatium = VERUM;
            }
        }
    }

    /* Si truncatum, addere "..." */
    si (i < valor->mensura && mensura_output >= III)
    {
        buffer[mensura_output - III] = '.';
        buffer[mensura_output - II] = '.';
        buffer[mensura_output - I] = '.';
    }

    buffer[mensura_output] = '\0';

    /* Si nullum non-spatium, usare [vacuum] */
    si (!habet_non_spatium)
    {
        hic_manens i8 vacuum_lit[] = "[vacuum]";
        i32 len;
        len = VIII;
        si (len >= max_mensura)
        {
            len = max_mensura - I;
        }
        memcpy(buffer, vacuum_lit, (memoriae_index)len);
        buffer[len] = '\0';
        redde len;
    }

    redde mensura_output;
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

/* Sortare relationes inversas alphabetice per titulum originis
 * "Sort inverse relations alphabetically by origin entity title"
 */
interior vacuum
_sortare_relationes_inversas(
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
    Entitas* origo_i;
    Entitas* origo_j;
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

            /* Obtinere titulum originis pro comparatione */
            chorda_i = NIHIL;
            chorda_j = NIHIL;

            si (repositorium && rel_i && rel_i->origo_id)
            {
                origo_i = repositorium->capere_entitatem(
                    repositorium->datum, rel_i->origo_id);
                si (origo_i)
                {
                    chorda_i = entitas_titulum_capere(origo_i);
                }
            }

            si (repositorium && rel_j && rel_j->origo_id)
            {
                origo_j = repositorium->capere_entitatem(
                    repositorium->datum, rel_j->origo_id);
                si (origo_j)
                {
                    chorda_j = entitas_titulum_capere(origo_j);
                }
            }

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

/* Verificare si clavis est "name" vel "title"
 * Redde: 1 pro "name", 2 pro "title", 0 pro aliis
 */
interior i32
_prioritas_proprietatis(chorda* clavis)
{
    si (!clavis || !clavis->datum)
    {
        redde ZEPHYRUM;
    }

    /* "name" = 4 litterae */
    si (clavis->mensura == IV)
    {
        si (clavis->datum[ZEPHYRUM] == 'n' &&
            clavis->datum[I] == 'a' &&
            clavis->datum[II] == 'm' &&
            clavis->datum[III] == 'e')
        {
            redde I;  /* Prioritas maxima */
        }
    }

    /* "title" = 5 litterae */
    si (clavis->mensura == V)
    {
        si (clavis->datum[ZEPHYRUM] == 't' &&
            clavis->datum[I] == 'i' &&
            clavis->datum[II] == 't' &&
            clavis->datum[III] == 'l' &&
            clavis->datum[IV] == 'e')
        {
            redde II;  /* Prioritas secunda */
        }
    }

    redde ZEPHYRUM;  /* Nulla prioritas specialis */
}

/* Sortare proprietates: name primo, title secundo, deinde alphabetice
 */
interior vacuum
_sortare_proprietates(
    ItemNavigatoris* items,
    i32              numerus_proprietatum)
{
    i32 i;
    i32 j;
    i32 k;
    ItemNavigatoris temp;
    Proprietas* prop_i;
    Proprietas* prop_j;
    i32 prio_i;
    i32 prio_j;
    b32 debet_movere;

    /* Insertion sort */
    per (i = I; i < numerus_proprietatum; i++)
    {
        temp = items[i];
        prop_i = (Proprietas*)temp.datum;
        prio_i = _prioritas_proprietatis(prop_i ? prop_i->clavis : NIHIL);

        /* Invenire positio pro insertione */
        k = i;
        per (j = ZEPHYRUM; j < i; j++)
        {
            prop_j = (Proprietas*)items[j].datum;
            prio_j = _prioritas_proprietatis(prop_j ? prop_j->clavis : NIHIL);

            debet_movere = FALSUM;

            /* Prioritas specialis (name=1, title=2) venit primo */
            si (prio_i > ZEPHYRUM && prio_j == ZEPHYRUM)
            {
                /* i habet prioritatem, j non habet */
                debet_movere = VERUM;
            }
            alioquin si (prio_i > ZEPHYRUM && prio_j > ZEPHYRUM)
            {
                /* Ambo habent prioritatem - name (1) ante title (2) */
                si (prio_i < prio_j)
                {
                    debet_movere = VERUM;
                }
            }
            alioquin si (prio_i == ZEPHYRUM && prio_j == ZEPHYRUM)
            {
                /* Nullus habet prioritatem - alphabetice */
                si (prop_i && prop_i->clavis && prop_j && prop_j->clavis)
                {
                    si (_comparare_case_insensitive(prop_i->clavis, prop_j->clavis) < ZEPHYRUM)
                    {
                        debet_movere = VERUM;
                    }
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
 * Ordo: relationes non-contains, relationes contains, proprietates, backlinks
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
    i32          initium_non_contains;
    i32          numerus_non_contains;
    i32          initium_contains;
    i32          numerus_contains;

    si (!nav || !nav->entitas_currens)
    {
        redde;
    }

    ent = nav->entitas_currens;
    nav->numerus_itemorum = ZEPHYRUM;

    (vacuum)numerus_items_via;  /* Non usatum post remotionem genus mode */

    /* Primo: addere relationes NON-contains */
    initium_non_contains = nav->numerus_itemorum;
    numerus = xar_numerus(ent->relationes);
    per (i = ZEPHYRUM; i < numerus && nav->numerus_itemorum < CXXVIII; i++)
    {
        b32 est_contains;
        hic_manens i8 contains_lit[] = "contains";

        rel = (Relatio*)xar_obtinere(ent->relationes, i);
        si (!rel)
        {
            perge;
        }

        /* Verificare si est "contains" relatio */
        est_contains = FALSUM;
        si (rel->genus && rel->genus->datum && rel->genus->mensura == VIII)
        {
            si (memcmp(rel->genus->datum, contains_lit, VIII) == ZEPHYRUM)
            {
                est_contains = VERUM;
            }
        }

        /* Solum non-contains in hac sectione */
        si (est_contains)
        {
            perge;
        }

        item = &nav->items[nav->numerus_itemorum];
        item->genus = ITEM_RELATIO;
        item->altitudo = II;  /* Semper 2 lineae pro non-contains */
        item->datum = rel;

        nav->numerus_itemorum++;
    }
    numerus_non_contains = nav->numerus_itemorum - initium_non_contains;

    /* Sortare relationes non-contains alphabetice */
    si (numerus_non_contains > I)
    {
        _sortare_relationes(
            &nav->items[initium_non_contains],
            numerus_non_contains,
            nav->ctx->repo);
    }

    /* Secundo: addere relationes CONTAINS */
    initium_contains = nav->numerus_itemorum;
    per (i = ZEPHYRUM; i < numerus && nav->numerus_itemorum < CXXVIII; i++)
    {
        b32 est_contains;
        hic_manens i8 contains_lit[] = "contains";

        rel = (Relatio*)xar_obtinere(ent->relationes, i);
        si (!rel)
        {
            perge;
        }

        /* Verificare si est "contains" relatio */
        est_contains = FALSUM;
        si (rel->genus && rel->genus->datum && rel->genus->mensura == VIII)
        {
            si (memcmp(rel->genus->datum, contains_lit, VIII) == ZEPHYRUM)
            {
                est_contains = VERUM;
            }
        }

        /* Solum contains in hac sectione */
        si (!est_contains)
        {
            perge;
        }

        item = &nav->items[nav->numerus_itemorum];
        item->genus = ITEM_RELATIO;
        item->altitudo = I;  /* Semper 1 linea pro contains */
        item->datum = rel;

        nav->numerus_itemorum++;
    }
    numerus_contains = nav->numerus_itemorum - initium_contains;

    /* Sortare relationes contains alphabetice */
    si (numerus_contains > I)
    {
        _sortare_relationes(
            &nav->items[initium_contains],
            numerus_contains,
            nav->ctx->repo);
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

    /* Addere proprietates (non-genus) */
    {
        i32 initium_proprietatum;
        i32 numerus_proprietatum;

        initium_proprietatum = nav->numerus_itemorum;
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

            /* Pro contentum, usare longitudinem truncatam (~50 + "[vacuum]") */
            si (_est_contentum(prop))
            {
                longitudo_totalis += LVIII;  /* ~50 chars max + margine */
            }
            alioquin si (prop->valor && prop->valor->datum)
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

        /* Sortare proprietates: name primo, title secundo, deinde alphabetice */
        numerus_proprietatum = nav->numerus_itemorum - initium_proprietatum;
        si (numerus_proprietatum > I)
        {
            _sortare_proprietates(
                &nav->items[initium_proprietatum],
                numerus_proprietatum);
        }
    }

    /* Addere relationes inversas (backlinks) */
    {
        Xar*      relationes_ad;
        Relatio*  rel_inv;
        Relatio** rel_slot;
        i32       numerus_backlinks;
        i32       initium_backlinks;

        relationes_ad = nav->ctx->repo->capere_relationes_ad(
            nav->ctx->repo->datum,
            ent->id);

        si (relationes_ad)
        {
            initium_backlinks = nav->numerus_itemorum;
            numerus = xar_numerus(relationes_ad);

            per (i = ZEPHYRUM; i < numerus && nav->numerus_itemorum < CXXVIII; i++)
            {
                hic_manens i8 contains_lit[] = "contains";

                rel_slot = (Relatio**)xar_obtinere(relationes_ad, i);
                si (!rel_slot || !*rel_slot)
                {
                    perge;
                }

                rel_inv = *rel_slot;

                /* Filtrare "contains" relationes - iam monstrantur in columna parente */
                si (rel_inv->genus && rel_inv->genus->datum &&
                    rel_inv->genus->mensura == VIII &&
                    memcmp(rel_inv->genus->datum, contains_lit, VIII) == ZEPHYRUM)
                {
                    perge;
                }

                item = &nav->items[nav->numerus_itemorum];
                item->genus = ITEM_RELATIO_INVERSA;
                item->altitudo = II;  /* Semper 2 lineae */
                item->datum = rel_inv;

                nav->numerus_itemorum++;
            }

            /* Sortare backlinks alphabetice (per titulum originis) */
            numerus_backlinks = nav->numerus_itemorum - initium_backlinks;
            si (numerus_backlinks > I)
            {
                _sortare_relationes_inversas(
                    &nav->items[initium_backlinks],
                    numerus_backlinks,
                    nav->ctx->repo);
            }
        }
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
    ContextusWidget* ctx)
{
    NavigatorEntitatum* nav;
    Xar*                radices;
    Entitas**           radix_slot;

    si (!ctx || !ctx->piscina || !ctx->repo)
    {
        redde NIHIL;
    }

    /* Allocare structuram */
    nav = (NavigatorEntitatum*)piscina_allocare(
        ctx->piscina,
        magnitudo(NavigatorEntitatum));
    si (!nav)
    {
        redde NIHIL;
    }

    /* Initiare campos */
    nav->ctx = ctx;
    nav->entitas_currens    = NIHIL;
    nav->numerus_itemorum   = ZEPHYRUM;
    nav->selectio           = ZEPHYRUM;
    nav->pagina_currens     = ZEPHYRUM;
    nav->items_per_pagina   = XXX;  /* Valor initiarius */

    /* Creare Xar pro via navigationis */
    nav->via = xar_creare(ctx->piscina, magnitudo(ItemHistoriae));
    si (!nav->via)
    {
        redde NIHIL;
    }

    /* Tentare navigare ad primam radicem */
    radices = ctx->repo->capere_radices(ctx->repo->datum);
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
    chorda*             entitas_id,
    b32                 per_backlink)
{
    Entitas*        nova_entitas;
    ItemHistoriae*  item_historiae;

    si (!nav || !entitas_id)
    {
        redde FALSUM;
    }

    /* Capere novam entitatem ex repositorium */
    nova_entitas = nav->ctx->repo->capere_entitatem(
        nav->ctx->repo->datum,
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
            item_historiae->per_backlink = per_backlink;
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
    entitas_praecedens = nav->ctx->repo->capere_entitatem(
        nav->ctx->repo->datum,
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

            /* Solum quaerere in tipo correcta (relatio vel backlink) */
            si (!item_historiae->per_backlink && item->genus == ITEM_RELATIO)
            {
                /* Pro relatione normali, destinatio_id est quo navigavimus */
                rel = (Relatio*)item->datum;
                si (rel && rel->destinatio_id == item_historiae->entitas_id_destinatio)
                {
                    nav->selectio = i;
                    frange;
                }
            }
            alioquin si (item_historiae->per_backlink && item->genus == ITEM_RELATIO_INVERSA)
            {
                /* Pro backlink, origo_id est quo navigavimus */
                rel = (Relatio*)item->datum;
                si (rel && rel->origo_id == item_historiae->entitas_id_destinatio)
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
            /* Navigatio normalis per relationes (ad destinationem) */
            rel = (Relatio*)item->datum;
            navigator_entitatum_navigare_ad(nav, rel->destinatio_id, FALSUM);
        }
        alioquin si (item->genus == ITEM_RELATIO_INVERSA)
        {
            /* Navigatio per backlink (ad originem) */
            rel = (Relatio*)item->datum;
            navigator_entitatum_navigare_ad(nav, rel->origo_id, VERUM);
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
 * Hoc includit ITEM_RELATIO, ITEM_PROPRIETAS, ITEM_RELATIO_INVERSA
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

        /* Addere spatium inter sectiones */
        si (i > ZEPHYRUM)
        {
            ItemNavigatoris* item_praecedens;
            item_praecedens = &nav->items[i - I];

            /* Spatium inter non-contains et contains relationes
             * (non-contains habet altitudo=2, contains habet altitudo=1)
             */
            si (item->genus == ITEM_RELATIO &&
                item_praecedens->genus == ITEM_RELATIO &&
                item->altitudo == I &&
                item_praecedens->altitudo == II)
            {
                y_currens++;
            }
            /* Spatium inter relationes et proprietates */
            si (item->genus == ITEM_PROPRIETAS &&
                item_praecedens->genus == ITEM_RELATIO)
            {
                y_currens++;
            }
            /* Spatium inter proprietates et backlinks */
            si (item->genus == ITEM_RELATIO_INVERSA &&
                item_praecedens->genus == ITEM_PROPRIETAS)
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
                ? color_ad_pixelum(color_ex_palette(PALETTE_DARK_LEAF))
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

                dest_ent = nav->ctx->repo->capere_entitatem(
                    nav->ctx->repo->datum, rel->destinatio_id);

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

                textus.datum = (i8*)buffer;
                textus.mensura = buffer_mensura;

                tabula_pixelorum_pingere_chordam(
                    tabula,
                    x_columna * character_latitudo + (II * character_latitudo),
                    y_currens * character_altitudo,
                    textus,
                    color_textus);
            }
            alioquin
            {
                /* Non-contains: 2 lineae */
                Entitas* dest_ent;
                chorda* display_chorda;
                i32 color_genus;

                /* Linea 1: genus (sine "/") - in medium gold */
                color_genus = est_selectus
                    ? color_textus
                    : color_ad_pixelum(color_ex_palette(PALETTE_MEDIUM_GOLD));

                buffer_mensura = ZEPHYRUM;
                si (rel->genus && rel->genus->datum && rel->genus->mensura > ZEPHYRUM &&
                    rel->genus->mensura < CCLVI - buffer_mensura - X)
                {
                    memcpy(buffer + buffer_mensura,
                           rel->genus->datum,
                           (memoriae_index)rel->genus->mensura);
                    buffer_mensura += rel->genus->mensura;
                }
                buffer[buffer_mensura] = '\0';

                textus.datum = (i8*)buffer;
                textus.mensura = buffer_mensura;

                tabula_pixelorum_pingere_chordam(
                    tabula,
                    x_columna * character_latitudo + (II * character_latitudo),
                    y_currens * character_altitudo,
                    textus,
                    color_genus);

                /* Linea 2: "    titulus/" (indentatus cum 4 spatia) */
                dest_ent = nav->ctx->repo->capere_entitatem(
                    nav->ctx->repo->datum, rel->destinatio_id);

                display_chorda = entitas_titulum_capere(dest_ent);

                buffer_mensura = ZEPHYRUM;
                buffer[buffer_mensura++] = ' ';
                buffer[buffer_mensura++] = ' ';
                buffer[buffer_mensura++] = ' ';
                buffer[buffer_mensura++] = ' ';
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

                textus.datum = (i8*)buffer;
                textus.mensura = buffer_mensura;

                tabula_pixelorum_pingere_chordam(
                    tabula,
                    x_columna * character_latitudo,
                    (y_currens + I) * character_altitudo,
                    textus,
                    color_textus);
            }
        }
        alioquin si (item->genus == ITEM_PROPRIETAS)
        {
            i32 offset_textus;
            i32 linea_currens;
            i32 caracteres_in_linea;
            i32 prioritas;
            i32 clavis_et_separator_mensura;
            i32 color_valor;

            prop = (Proprietas*)item->datum;
            si (!prop)
            {
                y_currens += item->altitudo;
                perge;
            }

            /* Verificare si est name/title proprietas */
            prioritas = _prioritas_proprietatis(prop->clavis);

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

            clavis_et_separator_mensura = buffer_mensura;

            /* Pro contentum, usare sanitizationem */
            si (_est_contentum(prop))
            {
                buffer_mensura += _sanitizare_contentum(
                    buffer + buffer_mensura,
                    CCLVI - buffer_mensura,
                    prop->valor);
            }
            alioquin si (prop->valor && prop->valor->datum && prop->valor->mensura > ZEPHYRUM &&
                prop->valor->mensura < CCLVI - buffer_mensura)
            {
                memcpy(buffer + buffer_mensura,
                       prop->valor->datum,
                       (memoriae_index)prop->valor->mensura);
                buffer_mensura += prop->valor->mensura;
                buffer[buffer_mensura] = '\0';
            }
            alioquin
            {
                buffer[buffer_mensura] = '\0';
            }

            /* Determinare color pro valore */
            color_valor = color_textus;
            si (!est_selectus && prioritas > ZEPHYRUM)
            {
                /* name/title valor in dark cyan */
                color_valor = color_ad_pixelum(color_ex_palette(PALETTE_DARK_LEAF));
            }

            /* Reddere cum wrapping */
            offset_textus = ZEPHYRUM;
            linea_currens = ZEPHYRUM;

            dum (offset_textus < buffer_mensura)
            {
                i32 color_hic;

                caracteres_in_linea = ZEPHYRUM;

                per (j = ZEPHYRUM; j < latitudo_columnae && offset_textus + j < buffer_mensura; j++)
                {
                    caracteres_in_linea++;
                }

                textus.datum = (i8*)(buffer + offset_textus);
                textus.mensura = caracteres_in_linea;

                /* Si tota portio est in valore, usare color_valor */
                si (offset_textus >= clavis_et_separator_mensura)
                {
                    color_hic = color_valor;
                }
                /* Si tota portio est in clave, usare color_textus */
                alioquin si (offset_textus + caracteres_in_linea <= clavis_et_separator_mensura)
                {
                    color_hic = color_textus;
                }
                /* Si portio transit inter clave et valore, reddere separatim */
                alioquin
                {
                    i32 clavis_pars;
                    i32 valor_pars;
                    chorda textus_clavis;
                    chorda textus_valor;

                    clavis_pars = clavis_et_separator_mensura - offset_textus;
                    valor_pars = caracteres_in_linea - clavis_pars;

                    /* Reddere partem clavis */
                    textus_clavis.datum = (i8*)(buffer + offset_textus);
                    textus_clavis.mensura = clavis_pars;
                    tabula_pixelorum_pingere_chordam(
                        tabula,
                        x_columna * character_latitudo,
                        (y_currens + linea_currens) * character_altitudo,
                        textus_clavis,
                        color_textus);

                    /* Reddere partem valoris */
                    textus_valor.datum = (i8*)(buffer + offset_textus + clavis_pars);
                    textus_valor.mensura = valor_pars;
                    tabula_pixelorum_pingere_chordam(
                        tabula,
                        x_columna * character_latitudo + (clavis_pars * character_latitudo),
                        (y_currens + linea_currens) * character_altitudo,
                        textus_valor,
                        color_valor);

                    offset_textus += caracteres_in_linea;
                    linea_currens++;
                    perge;
                }

                tabula_pixelorum_pingere_chordam(
                    tabula,
                    x_columna * character_latitudo,
                    (y_currens + linea_currens) * character_altitudo,
                    textus,
                    color_hic);

                offset_textus += caracteres_in_linea;
                linea_currens++;
            }
        }
        alioquin si (item->genus == ITEM_RELATIO_INVERSA)
        {
            /* Backlink: < genus + titulum originis */
            Entitas* origo_ent;
            chorda*  display_chorda;
            chorda   arrow_textus;
            character arrow_buffer[III];
            i32      color_genus;

            rel = (Relatio*)item->datum;
            si (!rel)
            {
                y_currens += item->altitudo;
                perge;
            }

            /* Reddere prefix "<" in light gray */
            color_arrow = est_selectus
                ? color_textus
                : color_ad_pixelum(thema_color(COLOR_TEXT_DIM));
            arrow_buffer[ZEPHYRUM] = '<';
            arrow_buffer[I] = ' ';
            arrow_textus.datum = (i8*)arrow_buffer;
            arrow_textus.mensura = II;
            tabula_pixelorum_pingere_chordam(
                tabula,
                x_columna * character_latitudo,
                y_currens * character_altitudo,
                arrow_textus,
                color_arrow);

            /* Linea 1: genus relationis (in light gray) */
            color_genus = est_selectus
                ? color_textus
                : color_ad_pixelum(thema_color(COLOR_TEXT_DIM));

            buffer_mensura = ZEPHYRUM;
            si (rel->genus && rel->genus->datum && rel->genus->mensura > ZEPHYRUM &&
                rel->genus->mensura < CCLVI - X)
            {
                memcpy(buffer + buffer_mensura,
                       rel->genus->datum,
                       (memoriae_index)rel->genus->mensura);
                buffer_mensura += rel->genus->mensura;
            }
            buffer[buffer_mensura] = '\0';

            textus.datum = (i8*)buffer;
            textus.mensura = buffer_mensura;

            tabula_pixelorum_pingere_chordam(
                tabula,
                x_columna * character_latitudo + (II * character_latitudo),
                y_currens * character_altitudo,
                textus,
                color_genus);

            /* Linea 2: "    titulus_originis/" (indentatus cum 4 spatia) */
            origo_ent = nav->ctx->repo->capere_entitatem(
                nav->ctx->repo->datum, rel->origo_id);

            display_chorda = entitas_titulum_capere(origo_ent);

            buffer_mensura = ZEPHYRUM;
            buffer[buffer_mensura++] = ' ';
            buffer[buffer_mensura++] = ' ';
            buffer[buffer_mensura++] = ' ';
            buffer[buffer_mensura++] = ' ';
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

            textus.datum = (i8*)buffer;
            textus.mensura = buffer_mensura;

            tabula_pixelorum_pingere_chordam(
                tabula,
                x_columna * character_latitudo,
                (y_currens + I) * character_altitudo,
                textus,
                color_genus);  /* Usare gray pro backlink */
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
    b32                  selectio_per_backlink,   /* VERUM si selectio est backlink */
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

    /* Primo: addere relationes NON-contains */
    {
        i32 initium_non_contains;
        i32 numerus_non_contains;

        initium_non_contains = numerus_items;
        numerus = xar_numerus(entitas->relationes);
        per (i = ZEPHYRUM; i < numerus && numerus_items < CXXVIII; i++)
        {
            b32 est_contains;
            hic_manens i8 contains_lit[] = "contains";

            rel = (Relatio*)xar_obtinere(entitas->relationes, i);
            si (!rel)
            {
                perge;
            }

            /* Verificare si est "contains" relatio */
            est_contains = FALSUM;
            si (rel->genus && rel->genus->datum && rel->genus->mensura == VIII)
            {
                si (memcmp(rel->genus->datum, contains_lit, VIII) == ZEPHYRUM)
                {
                    est_contains = VERUM;
                }
            }

            /* Solum non-contains in hac sectione */
            si (est_contains)
            {
                perge;
            }

            item = &items_temp[numerus_items];
            item->genus = ITEM_RELATIO;
            item->altitudo = II;  /* Semper 2 lineae pro non-contains */
            item->datum = rel;
            numerus_items++;
        }
        numerus_non_contains = numerus_items - initium_non_contains;

        /* Sortare relationes non-contains alphabetice */
        si (numerus_non_contains > I)
        {
            _sortare_relationes(
                &items_temp[initium_non_contains],
                numerus_non_contains,
                selectio_repositorium);
        }
    }

    /* Secundo: addere relationes CONTAINS */
    {
        i32 initium_contains;
        i32 numerus_contains;

        initium_contains = numerus_items;
        numerus = xar_numerus(entitas->relationes);
        per (i = ZEPHYRUM; i < numerus && numerus_items < CXXVIII; i++)
        {
            b32 est_contains;
            hic_manens i8 contains_lit[] = "contains";

            rel = (Relatio*)xar_obtinere(entitas->relationes, i);
            si (!rel)
            {
                perge;
            }

            /* Verificare si est "contains" relatio */
            est_contains = FALSUM;
            si (rel->genus && rel->genus->datum && rel->genus->mensura == VIII)
            {
                si (memcmp(rel->genus->datum, contains_lit, VIII) == ZEPHYRUM)
                {
                    est_contains = VERUM;
                }
            }

            /* Solum contains in hac sectione */
            si (!est_contains)
            {
                perge;
            }

            item = &items_temp[numerus_items];
            item->genus = ITEM_RELATIO;
            item->altitudo = I;  /* Semper 1 linea pro contains */
            item->datum = rel;
            numerus_items++;
        }
        numerus_contains = numerus_items - initium_contains;

        /* Sortare relationes contains alphabetice */
        si (numerus_contains > I)
        {
            _sortare_relationes(
                &items_temp[initium_contains],
                numerus_contains,
                selectio_repositorium);
        }
    }

    /* Addere proprietates */
    {
        i32 initium_proprietatum;
        i32 numerus_proprietatum;

        initium_proprietatum = numerus_items;
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

                /* Pro contentum, usare longitudinem truncatam */
                si (_est_contentum(prop))
                {
                    longitudo_totalis += LVIII;
                }
                alioquin si (prop->valor && prop->valor->datum)
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

        /* Sortare proprietates: name primo, title secundo, deinde alphabetice */
        numerus_proprietatum = numerus_items - initium_proprietatum;
        si (numerus_proprietatum > I)
        {
            _sortare_proprietates(
                &items_temp[initium_proprietatum],
                numerus_proprietatum);
        }
    }

    /* Addere relationes inversas (backlinks) */
    si (selectio_repositorium)
    {
        Xar*      relationes_ad;
        Relatio*  rel_inv;
        Relatio** rel_slot;
        i32       numerus_backlinks;
        i32       initium_backlinks;

        relationes_ad = selectio_repositorium->capere_relationes_ad(
            selectio_repositorium->datum,
            entitas->id);

        si (relationes_ad)
        {
            initium_backlinks = numerus_items;
            numerus = xar_numerus(relationes_ad);

            per (i = ZEPHYRUM; i < numerus && numerus_items < CXXVIII; i++)
            {
                hic_manens i8 contains_lit[] = "contains";

                rel_slot = (Relatio**)xar_obtinere(relationes_ad, i);
                si (!rel_slot || !*rel_slot)
                {
                    perge;
                }

                rel_inv = *rel_slot;

                /* Filtrare "contains" relationes */
                si (rel_inv->genus && rel_inv->genus->datum &&
                    rel_inv->genus->mensura == VIII &&
                    memcmp(rel_inv->genus->datum, contains_lit, VIII) == ZEPHYRUM)
                {
                    perge;
                }

                item = &items_temp[numerus_items];
                item->genus = ITEM_RELATIO_INVERSA;
                item->altitudo = II;  /* Semper 2 lineae */
                item->datum = rel_inv;

                numerus_items++;
            }

            /* Sortare backlinks alphabetice */
            numerus_backlinks = numerus_items - initium_backlinks;
            si (numerus_backlinks > I)
            {
                _sortare_relationes_inversas(
                    &items_temp[initium_backlinks],
                    numerus_backlinks,
                    selectio_repositorium);
            }
        }
    }

    /* Reddere items */
    y_currens = y_initium;

    per (i = ZEPHYRUM; i < numerus_items && y_currens < y_initium + altitudo_maxima; i++)
    {
        item = &items_temp[i];

        /* Addere spatium inter sectiones */
        si (i > ZEPHYRUM)
        {
            ItemNavigatoris* item_praecedens;
            item_praecedens = &items_temp[i - I];

            /* Spatium inter non-contains et contains relationes
             * (non-contains habet altitudo=2, contains habet altitudo=1)
             */
            si (item->genus == ITEM_RELATIO &&
                item_praecedens->genus == ITEM_RELATIO &&
                item->altitudo == I &&
                item_praecedens->altitudo == II)
            {
                y_currens++;
            }
            /* Spatium inter relationes et proprietates */
            si (item->genus == ITEM_PROPRIETAS &&
                item_praecedens->genus == ITEM_RELATIO)
            {
                y_currens++;
            }
            /* Spatium inter proprietates et backlinks */
            si (item->genus == ITEM_RELATIO_INVERSA &&
                item_praecedens->genus == ITEM_PROPRIETAS)
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
        /* Verificare si selectus per destinatio ID (pro relatione normali) */
        alioquin si (selectio_destinatio_id &&
                     !selectio_per_backlink &&
                     item->genus == ITEM_RELATIO)
        {
            rel = (Relatio*)item->datum;
            si (rel && rel->destinatio_id == selectio_destinatio_id)
            {
                est_selectus = VERUM;
            }
        }
        /* Verificare si selectus per origo ID (pro backlink) */
        alioquin si (selectio_destinatio_id &&
                     selectio_per_backlink &&
                     item->genus == ITEM_RELATIO_INVERSA)
        {
            rel = (Relatio*)item->datum;
            si (rel && rel->origo_id == selectio_destinatio_id)
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
                color_arrow = color_ad_pixelum(color_ex_palette(PALETTE_DARK_LEAF));
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

                textus.datum = (i8*)buffer;
                textus.mensura = buffer_mensura;

                /* Truncare ad latitudinem columnae (minus 2 pro prefix) */
                si (textus.mensura > latitudo_columnae - II)
                {
                    textus.mensura = latitudo_columnae - II;
                }

                tabula_pixelorum_pingere_chordam(
                    tabula,
                    x_columna * character_latitudo + (II * character_latitudo),
                    y_currens * character_altitudo,
                    textus,
                    color_textus);
            }
            alioquin
            {
                /* Non-contains: 2 lineae */
                Entitas* dest_ent;
                chorda* display_chorda;
                i32 color_genus;

                /* Linea 1: genus (sine "/") - in medium gold */
                si (est_selectus)
                {
                    color_genus = color_textus;
                }
                alioquin si (dimmed)
                {
                    color_genus = color_ad_pixelum(thema_color(COLOR_TEXT_DIM));
                }
                alioquin
                {
                    color_genus = color_ad_pixelum(color_ex_palette(PALETTE_MEDIUM_GOLD));
                }

                buffer_mensura = ZEPHYRUM;
                si (rel->genus && rel->genus->datum && rel->genus->mensura > ZEPHYRUM &&
                    rel->genus->mensura < CCLVI - buffer_mensura - X)
                {
                    memcpy(buffer + buffer_mensura,
                           rel->genus->datum,
                           (memoriae_index)rel->genus->mensura);
                    buffer_mensura += rel->genus->mensura;
                }
                buffer[buffer_mensura] = '\0';

                textus.datum = (i8*)buffer;
                textus.mensura = buffer_mensura;

                /* Truncare ad latitudinem columnae (minus 2 pro prefix) */
                si (textus.mensura > latitudo_columnae - II)
                {
                    textus.mensura = latitudo_columnae - II;
                }

                tabula_pixelorum_pingere_chordam(
                    tabula,
                    x_columna * character_latitudo + (II * character_latitudo),
                    y_currens * character_altitudo,
                    textus,
                    color_genus);

                /* Linea 2: "    titulus/" (indentatus cum 4 spatia) */
                si (selectio_repositorium)
                {
                    dest_ent = selectio_repositorium->capere_entitatem(
                        selectio_repositorium->datum, rel->destinatio_id);

                    display_chorda = entitas_titulum_capere(dest_ent);

                    buffer_mensura = ZEPHYRUM;
                    buffer[buffer_mensura++] = ' ';
                    buffer[buffer_mensura++] = ' ';
                    buffer[buffer_mensura++] = ' ';
                    buffer[buffer_mensura++] = ' ';
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

                    textus.datum = (i8*)buffer;
                    textus.mensura = buffer_mensura;

                    /* Truncare ad latitudinem columnae */
                    si (textus.mensura > latitudo_columnae)
                    {
                        textus.mensura = latitudo_columnae;
                    }

                    tabula_pixelorum_pingere_chordam(
                        tabula,
                        x_columna * character_latitudo,
                        (y_currens + I) * character_altitudo,
                        textus,
                        color_textus);
                }
            }
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

            /* Pro contentum, usare sanitizationem */
            si (_est_contentum(prop))
            {
                buffer_mensura += _sanitizare_contentum(
                    buffer + buffer_mensura,
                    CCLVI - buffer_mensura,
                    prop->valor);
            }
            alioquin si (prop->valor && prop->valor->datum && prop->valor->mensura > ZEPHYRUM &&
                prop->valor->mensura < CCLVI - buffer_mensura)
            {
                memcpy(buffer + buffer_mensura,
                       prop->valor->datum,
                       (memoriae_index)prop->valor->mensura);
                buffer_mensura += prop->valor->mensura;
                buffer[buffer_mensura] = '\0';
            }
            alioquin
            {
                buffer[buffer_mensura] = '\0';
            }

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
        alioquin si (item->genus == ITEM_RELATIO_INVERSA)
        {
            /* Backlink: < genus + titulum originis */
            Entitas* origo_ent;
            chorda*  display_chorda;
            chorda   arrow_textus;
            character arrow_buffer[III];
            i32      color_genus;
            i32      color_arrow;

            rel = (Relatio*)item->datum;
            si (!rel)
            {
                y_currens += item->altitudo;
                perge;
            }

            /* Reddere prefix "<" in light gray */
            color_arrow = color_ad_pixelum(thema_color(COLOR_TEXT_DIM));
            arrow_buffer[ZEPHYRUM] = '<';
            arrow_buffer[I] = ' ';
            arrow_textus.datum = (i8*)arrow_buffer;
            arrow_textus.mensura = II;
            tabula_pixelorum_pingere_chordam(
                tabula,
                x_columna * character_latitudo,
                y_currens * character_altitudo,
                arrow_textus,
                color_arrow);

            /* Linea 1: genus relationis (in light gray) */
            si (est_selectus)
            {
                color_genus = color_textus;
            }
            alioquin
            {
                color_genus = color_ad_pixelum(thema_color(COLOR_TEXT_DIM));
            }

            buffer_mensura = ZEPHYRUM;
            si (rel->genus && rel->genus->datum && rel->genus->mensura > ZEPHYRUM &&
                rel->genus->mensura < CCLVI - X)
            {
                memcpy(buffer + buffer_mensura,
                       rel->genus->datum,
                       (memoriae_index)rel->genus->mensura);
                buffer_mensura += rel->genus->mensura;
            }
            buffer[buffer_mensura] = '\0';

            textus.datum = (i8*)buffer;
            textus.mensura = buffer_mensura;

            /* Truncare ad latitudinem columnae (minus 2 pro prefix) */
            si (textus.mensura > latitudo_columnae - II)
            {
                textus.mensura = latitudo_columnae - II;
            }

            tabula_pixelorum_pingere_chordam(
                tabula,
                x_columna * character_latitudo + (II * character_latitudo),
                y_currens * character_altitudo,
                textus,
                color_genus);

            /* Linea 2: "    titulus_originis/" (indentatus cum 4 spatia) */
            si (selectio_repositorium)
            {
                origo_ent = selectio_repositorium->capere_entitatem(
                    selectio_repositorium->datum, rel->origo_id);

                display_chorda = entitas_titulum_capere(origo_ent);

                buffer_mensura = ZEPHYRUM;
                buffer[buffer_mensura++] = ' ';
                buffer[buffer_mensura++] = ' ';
                buffer[buffer_mensura++] = ' ';
                buffer[buffer_mensura++] = ' ';
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

                textus.datum = (i8*)buffer;
                textus.mensura = buffer_mensura;

                /* Truncare ad latitudinem columnae */
                si (textus.mensura > latitudo_columnae)
                {
                    textus.mensura = latitudo_columnae;
                }

                tabula_pixelorum_pingere_chordam(
                    tabula,
                    x_columna * character_latitudo,
                    (y_currens + I) * character_altitudo,
                    textus,
                    color_genus);  /* Usare gray pro backlink */
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
    ctx = delineare_creare_contextum(nav->ctx->piscina, tabula);

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
            entitas_parens = nav->ctx->repo->capere_entitatem(
                nav->ctx->repo->datum,
                item_historiae->entitas_id);
        }
    }

    si (entitas_parens)
    {
        _reddere_columnam_entitatis(
            tabula,
            entitas_parens,
            nav->ctx->repo,
            x_sinistra,
            y + I,  /* +1 pro border */
            latitudo_sinistra - I,  /* -1 pro spatio divider */
            textus_altitudo,
            character_latitudo,
            character_altitudo,
            (i32)(-I),   /* Non usare index */
            nav->entitas_currens ? nav->entitas_currens->id : NIHIL,
            item_historiae ? item_historiae->per_backlink : FALSUM,
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
