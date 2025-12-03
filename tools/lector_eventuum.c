/* lector_eventuum.c - Instrumentum CLI pro Introspectione Log Files
 *
 * Legit et ostendit eventus ex .log files (nuntium format).
 *
 * USUS:
 *   lector_eventuum <file.log>
 *   lector_eventuum data/combinado.log --stats
 *   lector_eventuum data/combinado.log --genus Page --limit 10
 *   lector_eventuum --help
 */

#include "latina.h"
#include "persistentia.h"
#include "argumenta.h"
#include "filum.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "tabula_dispersa.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* ==================================================
 * Nomen Eventus
 * ================================================== */

interior constans character*
_nomen_eventus(EventusGenus genus)
{
    commutatio (genus)
    {
        casus EVENTUS_CREARE_ENTITAS:    redde "CREARE_ENTITAS";
        casus EVENTUS_DELERE_ENTITAS:    redde "DELERE_ENTITAS";
        casus EVENTUS_PONERE_PROPRIETAS: redde "PONERE_PROPRIETAS";
        casus EVENTUS_DELERE_PROPRIETAS: redde "DELERE_PROPRIETAS";
        casus EVENTUS_ADDERE_RELATIO:    redde "ADDERE_RELATIO";
        casus EVENTUS_DELERE_RELATIO:    redde "DELERE_RELATIO";
        casus EVENTUS_ADDERE_NOTA:       redde "ADDERE_NOTA";
        casus EVENTUS_DELERE_NOTA:       redde "DELERE_NOTA";
        ordinarius:                      redde "IGNOTUS";
    }
}


/* ==================================================
 * Imprimere Eventum
 * ================================================== */

interior vacuum
_imprimere_eventum(Eventum* e, i32 index)
{
    printf("[%d] %s\n", index, _nomen_eventus(e->genus));

    /* Entitas info */
    si (e->entitas_id)
    {
        printf("    entitas_id: %.*s\n",
            e->entitas_id->mensura, e->entitas_id->datum);
    }
    si (e->entitas_genus)
    {
        printf("    genus: %.*s\n",
            e->entitas_genus->mensura, e->entitas_genus->datum);
    }

    /* Payload secundum genus */
    commutatio (e->genus)
    {
        casus EVENTUS_PONERE_PROPRIETAS:
            si (e->datum.proprietas.clavis)
            {
                printf("    clavis: %.*s\n",
                    e->datum.proprietas.clavis->mensura,
                    e->datum.proprietas.clavis->datum);
            }
            si (e->datum.proprietas.valor)
            {
                /* Truncare valorem longum */
                i32 max_len = LX;
                i32 len = e->datum.proprietas.valor->mensura;
                si (len > max_len)
                {
                    printf("    valor: %.*s...\n", max_len,
                        e->datum.proprietas.valor->datum);
                }
                alioquin
                {
                    printf("    valor: %.*s\n", len,
                        e->datum.proprietas.valor->datum);
                }
            }
            frange;

        casus EVENTUS_DELERE_PROPRIETAS:
            si (e->datum.proprietas.clavis)
            {
                printf("    clavis: %.*s\n",
                    e->datum.proprietas.clavis->mensura,
                    e->datum.proprietas.clavis->datum);
            }
            frange;

        casus EVENTUS_ADDERE_RELATIO:
            si (e->datum.relatio.relatio_id)
            {
                printf("    relatio_id: %.*s\n",
                    e->datum.relatio.relatio_id->mensura,
                    e->datum.relatio.relatio_id->datum);
            }
            si (e->datum.relatio.relatio_genus)
            {
                printf("    relatio_genus: %.*s\n",
                    e->datum.relatio.relatio_genus->mensura,
                    e->datum.relatio.relatio_genus->datum);
            }
            si (e->datum.relatio.destinatio_id)
            {
                printf("    destinatio_id: %.*s\n",
                    e->datum.relatio.destinatio_id->mensura,
                    e->datum.relatio.destinatio_id->datum);
            }
            frange;

        casus EVENTUS_DELERE_RELATIO:
            si (e->datum.relatio.relatio_id)
            {
                printf("    relatio_id: %.*s\n",
                    e->datum.relatio.relatio_id->mensura,
                    e->datum.relatio.relatio_id->datum);
            }
            frange;

        casus EVENTUS_ADDERE_NOTA:
        casus EVENTUS_DELERE_NOTA:
            si (e->datum.nota.nota)
            {
                printf("    nota: %.*s\n",
                    e->datum.nota.nota->mensura,
                    e->datum.nota.nota->datum);
            }
            frange;

        ordinarius:
            frange;
    }

    printf("\n");
}


/* ==================================================
 * Statisticae
 * ================================================== */

interior vacuum
_imprimere_stats(Xar* eventus, Piscina* piscina)
{
    TabulaDispersa* per_genus_entitas;
    TabulaIterator iter;
    chorda clavis;
    vacuum* valor;
    i32 i;
    i32 numerus;
    i32 counts[VIII];  /* Pro 8 event types */

    /* Initiare counts */
    per (i = ZEPHYRUM; i < VIII; i++)
    {
        counts[i] = ZEPHYRUM;
    }

    /* Creare tabula pro entitas genus counts */
    per_genus_entitas = tabula_dispersa_creare_chorda(piscina, LXIV);

    numerus = xar_numerus(eventus);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        Eventum* e;
        i32* count_ptr;

        e = *(Eventum**)xar_obtinere(eventus, i);
        si (!e)
        {
            perge;
        }

        /* Count per event type */
        si (e->genus >= I && e->genus <= VIII)
        {
            counts[e->genus - I]++;
        }

        /* Count per entitas genus */
        si (e->entitas_genus && e->entitas_genus->mensura > ZEPHYRUM)
        {
            b32 inventum;

            inventum = tabula_dispersa_invenire(per_genus_entitas,
                *e->entitas_genus, (vacuum**)&count_ptr);
            si (inventum && count_ptr)
            {
                (*count_ptr)++;
            }
            alioquin
            {
                count_ptr = piscina_allocare(piscina, magnitudo(i32));
                si (count_ptr)
                {
                    *count_ptr = I;
                    tabula_dispersa_inserere(per_genus_entitas,
                        *e->entitas_genus, count_ptr);
                }
            }
        }
    }

    /* Imprimere */
    printf("Eventus Totales: %d\n\n", numerus);

    printf("Per Eventus Genus:\n");
    printf("  CREARE_ENTITAS:    %d\n", counts[ZEPHYRUM]);
    printf("  DELERE_ENTITAS:    %d\n", counts[I]);
    printf("  PONERE_PROPRIETAS: %d\n", counts[II]);
    printf("  DELERE_PROPRIETAS: %d\n", counts[III]);
    printf("  ADDERE_RELATIO:    %d\n", counts[IV]);
    printf("  DELERE_RELATIO:    %d\n", counts[V]);
    printf("  ADDERE_NOTA:       %d\n", counts[VI]);
    printf("  DELERE_NOTA:       %d\n", counts[VII]);

    printf("\nPer Entitas Genus:\n");
    iter = tabula_dispersa_iterator_initium(per_genus_entitas);
    dum (tabula_dispersa_iterator_proximum(&iter, &clavis, &valor))
    {
        i32* c = (i32*)valor;
        printf("  %.*s: %d\n", clavis.mensura, clavis.datum, *c);
    }
}


/* ==================================================
 * Filtrare
 * ================================================== */

interior b32
_eventum_correspondet(
    Eventum*            e,
    chorda              filtrum_genus,
    chorda              filtrum_entitas,
    EventusGenus        filtrum_type)
{
    /* Filtrare per entitas genus */
    si (filtrum_genus.mensura > ZEPHYRUM)
    {
        si (!e->entitas_genus)
        {
            redde FALSUM;
        }
        si (!chorda_aequalis(*e->entitas_genus, filtrum_genus))
        {
            redde FALSUM;
        }
    }

    /* Filtrare per entitas ID */
    si (filtrum_entitas.mensura > ZEPHYRUM)
    {
        si (!e->entitas_id)
        {
            redde FALSUM;
        }
        /* Partial match (contains) */
        si (chorda_invenire_index(*e->entitas_id, filtrum_entitas) < ZEPHYRUM)
        {
            redde FALSUM;
        }
    }

    /* Filtrare per event type */
    si (filtrum_type != ZEPHYRUM)
    {
        si (e->genus != filtrum_type)
        {
            redde FALSUM;
        }
    }

    redde VERUM;
}

interior EventusGenus
_parsere_eventus_type(chorda type_str)
{
    si (chorda_aequalis_literis(type_str, "creare"))
    {
        redde EVENTUS_CREARE_ENTITAS;
    }
    si (chorda_aequalis_literis(type_str, "delere"))
    {
        redde EVENTUS_DELERE_ENTITAS;
    }
    si (chorda_aequalis_literis(type_str, "proprietas"))
    {
        redde EVENTUS_PONERE_PROPRIETAS;
    }
    si (chorda_aequalis_literis(type_str, "relatio"))
    {
        redde EVENTUS_ADDERE_RELATIO;
    }
    si (chorda_aequalis_literis(type_str, "nota"))
    {
        redde EVENTUS_ADDERE_NOTA;
    }
    redde ZEPHYRUM;
}


/* ==================================================
 * Principale
 * ================================================== */

integer principale(integer argc, constans character* constans* argv)
{
    Piscina* piscina;
    ArgumentaParser* parser;
    ArgumentaFructus* fructus;
    Persistentia* persistentia;
    Xar* eventus;
    chorda filum_via;
    chorda filtrum_genus;
    chorda filtrum_entitas;
    chorda filtrum_type_str;
    EventusGenus filtrum_type;
    i32 limit;
    i32 tail;
    b32 stats_mode;
    i32 i;
    i32 numerus;
    i32 printed;
    character* via_cstr;

    /* Creare piscina */
    piscina = piscina_generare_dynamicum("lector_eventuum", 1024 * 512);
    si (piscina == NIHIL)
    {
        fprintf(stderr, "Error: Non potest creare piscinam\n");
        redde I;
    }

    /* Configurare parser argumentorum */
    parser = argumenta_creare(piscina);
    argumenta_ponere_descriptionem(parser,
        "lector_eventuum - Instrumentum pro introspectione log files");

    argumenta_addere_vexillum(parser, "-h", "--help",
        "Ostendere auxilium");
    argumenta_addere_vexillum(parser, "-s", "--stats",
        "Ostendere statisticas");
    argumenta_addere_optionem(parser, "-g", "--genus",
        "Filtrare per entitas genus (e.g. Page, Root)");
    argumenta_addere_optionem(parser, "-e", "--entitas",
        "Filtrare per entitas ID (partial match)");
    argumenta_addere_optionem(parser, "-t", "--type",
        "Filtrare per eventus type (creare, proprietas, relatio, nota)");
    argumenta_addere_optionem(parser, "-n", "--limit",
        "Limitare ad N eventus");
    argumenta_addere_optionem(parser, NIHIL, "--tail",
        "Ostendere ultimos N eventus");
    argumenta_addere_positionalem(parser, "filum",
        "Via ad .log file", VERUM);

    argumenta_addere_exemplum(parser, "lector_eventuum data/combinado.log");
    argumenta_addere_exemplum(parser, "lector_eventuum data/combinado.log --stats");
    argumenta_addere_exemplum(parser, "lector_eventuum data/combinado.log --genus Page --limit 10");
    argumenta_addere_exemplum(parser, "lector_eventuum data/combinado.log --tail 20");

    /* Parsere argumenta */
    fructus = argumenta_conari_parsere(parser, (i32)argc, argv);
    si (fructus == NIHIL)
    {
        argumenta_imprimere_errorem(parser);
        piscina_destruere(piscina);
        redde I;
    }

    /* Tractare --help */
    si (argumenta_habet_vexillum(fructus, "--help"))
    {
        argumenta_imprimere_auxilium(parser);
        piscina_destruere(piscina);
        redde ZEPHYRUM;
    }

    /* Obtinere filum via */
    filum_via = argumenta_obtinere_positionalem(fructus, ZEPHYRUM, piscina);
    si (filum_via.mensura == ZEPHYRUM)
    {
        fprintf(stderr, "Error: Requiritur via ad .log file\n");
        argumenta_imprimere_auxilium(parser);
        piscina_destruere(piscina);
        redde I;
    }

    /* Convertere ad C string */
    via_cstr = chorda_ut_cstr(filum_via, piscina);
    si (!via_cstr)
    {
        fprintf(stderr, "Error: Memoria exhausta\n");
        piscina_destruere(piscina);
        redde I;
    }

    /* Verificare si filum existit */
    si (!filum_existit(via_cstr))
    {
        fprintf(stderr, "Error: Filum non existit: %s\n", via_cstr);
        piscina_destruere(piscina);
        redde I;
    }

    /* Obtinere optiones */
    stats_mode = argumenta_habet_vexillum(fructus, "--stats");
    filtrum_genus = argumenta_obtinere_optionem(fructus, "--genus", piscina);
    filtrum_entitas = argumenta_obtinere_optionem(fructus, "--entitas", piscina);
    filtrum_type_str = argumenta_obtinere_optionem(fructus, "--type", piscina);
    filtrum_type = ZEPHYRUM;
    si (filtrum_type_str.mensura > ZEPHYRUM)
    {
        filtrum_type = _parsere_eventus_type(filtrum_type_str);
    }

    /* Obtinere limit/tail */
    {
        chorda limit_str;
        chorda tail_str;

        limit_str = argumenta_obtinere_optionem(fructus, "--limit", piscina);
        tail_str = argumenta_obtinere_optionem(fructus, "--tail", piscina);

        limit = ZEPHYRUM;  /* 0 = no limit */
        tail = ZEPHYRUM;   /* 0 = no tail */

        si (limit_str.mensura > ZEPHYRUM)
        {
            chorda_ut_i32(limit_str, &limit);
        }
        si (tail_str.mensura > ZEPHYRUM)
        {
            chorda_ut_i32(tail_str, &tail);
        }
    }

    /* Aperire et legere eventus */
    persistentia = persistentia_nuntium_aperire(piscina, via_cstr);
    si (!persistentia)
    {
        fprintf(stderr, "Error: Non potest aperire: %s\n", via_cstr);
        piscina_destruere(piscina);
        redde I;
    }

    eventus = persistentia->legere_eventus(persistentia->datum, piscina);
    si (!eventus)
    {
        fprintf(stderr, "Error: Non potest legere eventus\n");
        piscina_destruere(piscina);
        redde I;
    }

    /* Stats mode */
    si (stats_mode)
    {
        _imprimere_stats(eventus, piscina);
        persistentia->claudere(persistentia->datum);
        piscina_destruere(piscina);
        redde ZEPHYRUM;
    }

    /* Regular output */
    numerus = xar_numerus(eventus);
    printed = ZEPHYRUM;

    /* Output events with filtering */
    {
        i32 start_idx;
        i32 match_idx;

        start_idx = ZEPHYRUM;
        match_idx = ZEPHYRUM;

        /* Computare start index pro tail */
        si (tail > ZEPHYRUM)
        {
            /* Count matching events first */
            i32 matching;

            matching = ZEPHYRUM;
            per (i = ZEPHYRUM; i < numerus; i++)
            {
                Eventum* e;

                e = *(Eventum**)xar_obtinere(eventus, i);
                si (e && _eventum_correspondet(e, filtrum_genus, filtrum_entitas, filtrum_type))
                {
                    matching++;
                }
            }
            si (matching > tail)
            {
                start_idx = matching - tail;
            }
        }

        per (i = ZEPHYRUM; i < numerus; i++)
        {
            Eventum* e;

            e = *(Eventum**)xar_obtinere(eventus, i);
            si (!e)
            {
                perge;
            }

            /* Applicare filtrum */
            si (!_eventum_correspondet(e, filtrum_genus, filtrum_entitas, filtrum_type))
            {
                perge;
            }

            /* Skip for tail */
            si (tail > ZEPHYRUM && match_idx < start_idx)
            {
                match_idx++;
                perge;
            }
            match_idx++;

            /* Imprimere */
            _imprimere_eventum(e, i + I);
            printed++;

            /* Check limit */
            si (limit > ZEPHYRUM && printed >= limit)
            {
                frange;
            }
        }
    }

    si (printed == ZEPHYRUM)
    {
        printf("Nullae eventus inventi.\n");
    }
    alioquin
    {
        printf("--- %d eventus ostensa ---\n", printed);
    }

    /* Cleanup */
    persistentia->claudere(persistentia->datum);
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
