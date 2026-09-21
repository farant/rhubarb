/* nota_frigida.c - via scripturae FRIGIDA (residens absens):
 * programma NATIVUM compilatum (machinula sqlite vocare non potest
 * - registrum 42 functionum, exploratio infra; colloquium pro
 * bibliothecis puris manet). Usus:
 *   nota_frigida <res_id|titulus> <textus...>       nota
 *   nota_frigida -crea <genus> <titulus> [textus]   res nova
 *   nota_frigida [-actor A] [-origo O] -status <res> <novus>
 *   nota_frigida [...] -mutatio <res> <clavis> <valor>
 *   nota_frigida [...] -nexus <res> <verbum> <alterum>
 * Actor fran, origo frigida. Currendum ex radice (frigida.sh).
 *
 * DUAE IANUAE, CONSULTO (2026-09-21): formae VETERES (nota, -crea)
 * per gesta_scribere DIRECTE eunt - ianua tumultuaria muta et
 * robusta manet cum machina tabularii ipsa aegrotat. Formae NOVAE
 * per MACHINAM eunt (fontes/frigida.c): regulae eaedem ac MCP (verba
 * canonica), proiectiones eaedem, recusationes eaedem. */

#include "gesta.h"
#include "scrinium.h"
#include "frigida.h"
#include <stdio.h>
#include <string.h>

interior constans character* VIA_DB = "tabularium.db";
interior constans character* VIA_AN =
    "gesta/annales/tabularium.jsonl";

/* res_id per titulum exactum (chorda vacua = absens) */
interior chorda
_per_titulum (
           GestaMundus* m,
    constans character* titulus,
               Piscina* piscina)
{
    ScriniumEnuntiatum* e;
                chorda  fructus;
                chorda  t;
    unio { constans character* l; i8* d; } u;

    fructus.mensura  = ZEPHYRUM;
    fructus.datum    = NIHIL;
    u.l              = titulus;
    t.datum          = u.d;
    t.mensura        = (i32)strlen(titulus);
    e = scrinium_praeparare(gesta_scrinium(m),
        "SELECT res_id FROM res WHERE titulus = ? LIMIT 2");
    si (e == NIHIL)
    {
        redde fructus;
    }
    scrinium_ligare_textum(e, I, t);
    si (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        fructus = scrinium_columna_textus(e, 0, piscina);
        si (scrinium_gradi(e) == SCRINIUM_ORDO)
        {
            /* titulus ambiguus - recusatio honesta (acies
             * titulorum duplicatorum; res_id discernit) */
            fprintf(stderr, "nota_frigida: titulus ambiguus"
                " (plures res) - res_id adhibe\n");
            fructus.mensura  = ZEPHYRUM;
            fructus.datum    = NIHIL;
        }
    }
    scrinium_finire(e);
    redde fructus;
}

interior constans character*
_textus_iungere (
      Piscina*  piscina,
    character** argv,
      integer   initium,
      integer   argc)
{
    memoriae_index  mensura = I;
         character* textus;
           integer  k;

    per (k = initium; k < argc; k++)
    {
        mensura += strlen(argv[k]) + I;
    }
    textus = (character*)piscina_allocare(piscina, mensura);
    si (textus == NIHIL)
    {
        redde "";
    }
    textus[0] = '\0';
    per (k = initium; k < argc; k++)
    {
        si (k > initium)
        {
            strcat(textus, " ");
        }
        strcat(textus, argv[k]);
    }
    redde textus;
}

s32
principale (
      integer   argc,
    character** argv)
{
    Piscina* piscina = piscina_generare_dynamicum("frigida",
        33554432);
     GestaMundus* m;
    GestaEventum  e;
       character  res_id[GESTA_RES_ID_MENSURA];

    si (piscina == NIHIL)
    {
        redde I;
    }
    /* OMNE vexillum praeter '-crea' ad machinam it - etiam IGNOTUM,
     * ut recusationem claram cum formis validis accipiat. Olim
     * solum vexilla NOTA eo ibant, et typographum ('-statum') in
     * formam veterem cadebat ubi ut TITULUS REI legebatur: 'res
     * ignota -statum'. Porta fumi id primo cursu cepit. */
    si (   argc >= II
        && (   frigida_verbum_novit(argv[I])
            || (   argv[I][0] == '-'
                && strcmp(argv[I], "-crea") != ZEPHYRUM)))
    {
        /* formae novae: machina tabularii (viae eaedem ac
         * tabularium_principale.c; vigilia quieta) */
        TabulariumConfiguratio cfg;

        cfg.radix             = ".";
        cfg.via_scrinii       = VIA_DB;
        cfg.via_annalium      = VIA_AN;
        cfg.via_nexus         = "build/nexus.tsv";
        cfg.via_identitatum   = "build/identitates.tsv";
        cfg.via_citationum    = "build/citationes.tsv";
        cfg.via_tabulae       = "gesta/annales/tabula.md";
        cfg.via_entitatum     = "gesta/annales/entities";
        cfg.signum            = NIHIL;
        cfg.via_binarii       = NIHIL;
        cfg.via_manifesti     = NIHIL;
        cfg.via_renovatoris   = NIHIL;
        cfg.renovatio_exitus  = FALSUM;
        cfg.renatus           = FALSUM;
        piscina_destruere(piscina);
        redde (s32)frigida_currere(&cfg, argc, argv, stdout,
            stderr);
    }
    si (argc < III)
    {
        fprintf(stderr, "usus: nota_frigida <res|titulus>"
            " <textus...>\n     aut: nota_frigida -crea <genus>"
            " <titulus> [textus]\n     aut: nota_frigida [-actor A]"
            " [-origo O] -status <res> <novus>\n     aut:"
            " nota_frigida [-actor A] [-origo O] -mutatio <res>"
            " <clavis> <valor>\n     aut: nota_frigida [-actor A]"
            " [-origo O] -nexus <res> <verbum> <alterum>\n");
        redde II;
    }
    m = gesta_aperire(piscina, VIA_DB, VIA_AN);
    si (m == NIHIL)
    {
        fprintf(stderr, "nota_frigida: scrinium aperiri non"
            " potuit (ex radice curre)\n");
        redde I;
    }
    si (strcmp(argv[I], "-crea") == ZEPHYRUM)
    {
        {
                     character  datum[2048];
            constans character* corpus = argc > IV
                ? _textus_iungere(piscina, argv, IV, argc) : NIHIL;

            si (   argc < IV || strlen(argv[II]) > (memoriae_index)128
                || strlen(argv[III]) > (memoriae_index)256
                || (corpus != NIHIL
                    && strlen(corpus) > (memoriae_index)1024))
            {
                fprintf(stderr, "nota_frigida: -crea <genus>"
                    " <titulus> [textus] (mensurae modicae)\n");
                redde II;
            }
            /* NB tituli/corpora cum '"' hic non effugiuntur -
             * via frigida est pro notis simplicibus; JSON plenum
             * per MCP */
            si (corpus != NIHIL)
            {
                sprintf(datum, "{\"genus\":\"%s\",\"titulus\":"
                    "\"%s\",\"corpus\":\"%s\"}", argv[II],
                    argv[III], corpus);
            }
            alioquin
            {
                sprintf(datum, "{\"genus\":\"%s\",\"titulus\":"
                    "\"%s\"}", argv[II], argv[III]);
            }
            e.res_id         = NIHIL;
            e.genus_eventus  = "creatio";
            e.datum          = datum;
            e.actor          = "fran";
            e.origo          = "frigida";
            si (!gesta_scribere(m, &e, res_id))
            {
                fprintf(stderr, "nota_frigida: %s\n",
                    gesta_error(m));
                redde I;
            }
            imprimere("res %s creata\n", res_id);
        }
    }
    alioquin
    {
        chorda datum_r = gesta_res_datum(m, argv[I], piscina);
        constans character* res_effectiva = argv[I];
        constans character* textus = _textus_iungere(piscina,
            argv, II, argc);
        character datum[4096];

        si (datum_r.mensura == ZEPHYRUM)
        {
            chorda per_t = _per_titulum(m, argv[I], piscina);

            si (per_t.mensura == ZEPHYRUM)
            {
                fprintf(stderr, "nota_frigida: res ignota '%s'\n",
                    argv[I]);
                redde I;
            }
            {
                character* copia = (character*)piscina_allocare(
                    piscina, (memoriae_index)per_t.mensura + I);

                memcpy(copia, per_t.datum,
                    (memoriae_index)per_t.mensura);
                copia[per_t.mensura]  = '\0';
                res_effectiva         = copia;
            }
        }
        si (strlen(textus) > (memoriae_index)3800)
        {
            fprintf(stderr, "nota_frigida: textus nimis longus\n");
            redde II;
        }
        sprintf(datum, "{\"textus\":\"%s\"}", textus);
        e.res_id         = res_effectiva;
        e.genus_eventus  = "nota";
        e.datum          = datum;
        e.actor          = "fran";
        e.origo          = "frigida";
        si (!gesta_scribere(m, &e, NIHIL))
        {
            fprintf(stderr, "nota_frigida: %s\n", gesta_error(m));
            redde I;
        }
        imprimere("nota scripta in %s\n", res_effectiva);
    }
    gesta_claudere(m);
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
