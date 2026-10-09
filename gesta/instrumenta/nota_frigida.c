/* nota_frigida.c - via scripturae FRIGIDA (residens absens):
 * programma NATIVUM compilatum (machinula sqlite vocare non potest
 * - registrum 42 functionum, exploratio infra; colloquium pro
 * bibliothecis puris manet). Usus:
 *   nota_frigida <res_id|titulus> <textus...>       nota
 *   nota_frigida -crea <genus> <titulus> [textus]   res nova
 *   nota_frigida [-actor A] [-origo O] -status <res> <novus>
 *   nota_frigida [...] -mutatio <res> <clavis> <valor>
 *   nota_frigida [...] -nexus <res> <verbum> <alterum>
 *   nota_frigida -sedes           sedes annalium (TSV, nihil scribit)
 *   nota_frigida -restituere      scrinium ex annalibus (absens solum)
 *   nota_frigida -genesis         tabularium NOVUM (sedes vacua solum)
 * Actor fran, origo frigida. Currendum ex radice (frigida.sh). Acta
 * in SEDE ANNALIUM (annales_sedes.h), custos ante omnem aperturam:
 * via frigida numquam annales novos gignit.
 *
 * DUAE IANUAE, CONSULTO (2026-09-21): formae VETERES (nota, -crea)
 * per gesta_scribere DIRECTE eunt - ianua tumultuaria muta et
 * robusta manet cum machina tabularii ipsa aegrotat. Formae NOVAE
 * per MACHINAM eunt (fontes/frigida.c): regulae eaedem ac MCP (verba
 * canonica), proiectiones eaedem, recusationes eaedem. */

#include "gesta.h"
#include "scrinium.h"
#include "frigida.h"
#include "annales_sedes.h"
#include "filum.h"
#include <stdio.h>
#include <string.h>

/* viae ex sede annalium (principale eas ponit ante omnem usum) */
interior constans character* VIA_DB = NIHIL;
interior constans character* VIA_AN = NIHIL;
interior constans character* VIA_TABULAE = NIHIL;
interior constans character* VIA_ENTIUM = NIHIL;

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

/* -sedes: sedes annalium ut TSV (clavis TAB via) - fons unus pro
 * scriptis (fumus, pythonica), nihil scribit */
interior s32
_sedes_imprimere (
    constans AnnaliumSedes* sedes,
                   Piscina* piscina)
{
    constans character* origines[III];
    constans character* claves[VI];
                   i32  k;

    origines[ANNALES_EX_AMBITU]         = "ambitus";
    origines[ANNALES_EX_DOMO]           = "domus";
    origines[ANNALES_EX_ARBORE]         = "arbor";
    claves[ANNALES_TABULARII]           = "annales";
    claves[ANNALES_SCRINIUM_TABULARII]  = "scrinium";
    claves[ANNALES_TABULA]              = "tabula";
    claves[ANNALES_ENTIA]               = "entia";
    claves[ANNALES_FORI]                = "annales_fori";
    claves[ANNALES_SCRINIUM_FORI]       = "scrinium_fori";
    imprimere("origo\t%s\n", origines[sedes->origo]);
    per (k = ZEPHYRUM; k < VI; k++)
    {
        chorda via;

        via = annales_via(sedes, (AnnaliumPlagula)k, piscina);
        imprimere("%s\t%.*s\n", claves[k], (integer)via.mensura,
            (constans character*)via.datum);
    }
    redde ZEPHYRUM;
}

/* -restituere: scrinium NOVUM ex annalibus (seq servata, deinde
 * replicatio); recusatur si scrinium iam exstat aut annales absunt */
interior s32
_restituere (
    Piscina* piscina)
{
    GestaMundus* m;

    si (!filum_existit(VIA_AN))
    {
        fprintf(stderr, "nota_frigida: RECUSATUM - annales absentes:"
            " %s\n", VIA_AN);
        redde I;
    }
    si (filum_existit(VIA_DB))
    {
        fprintf(stderr, "nota_frigida: RECUSATUM - scrinium iam"
            " exstat: %s\n", VIA_DB);
        redde I;
    }
    m = gesta_ex_annalibus_restituere(piscina, VIA_AN, VIA_DB);
    si (m == NIHIL)
    {
        fprintf(stderr, "nota_frigida: restitutio fracta (%s ->"
            " %s)\n", VIA_AN, VIA_DB);
        redde I;
    }
    si (!gesta_annales_verificare(m))
    {
        fprintf(stderr, "nota_frigida: restitutum sed annales !="
            " acta\n");
        gesta_claudere(m);
        redde I;
    }
    gesta_claudere(m);
    imprimere("restitutum: %s ex %s\n", VIA_DB, VIA_AN);
    redde ZEPHYRUM;
}

/* -genesis: tabularium NOVUM (annales vacui + scrinium) - solum in
 * sede vacua; genesis numquam tacita (custos alibi recusat) */
interior s32
_genesis (
    Piscina* piscina)
{
    GestaMundus* m;

    si (filum_existit(VIA_AN) || filum_existit(VIA_DB))
    {
        fprintf(stderr, "nota_frigida: RECUSATUM - genesis in sede"
            " non vacua: %s\n", VIA_AN);
        redde I;
    }
    m = gesta_aperire(piscina, VIA_DB, VIA_AN);
    si (m == NIHIL)
    {
        fprintf(stderr, "nota_frigida: genesis fracta (%s)\n", VIA_DB);
        redde I;
    }
    gesta_claudere(m);
    imprimere("genesis: %s + %s\n", VIA_AN, VIA_DB);
    redde ZEPHYRUM;
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
    /* SEDES ANNALIUM: piscina propria (piscina prima ante machinam
     * destruitur, viae superesse debent) */
    {
              Piscina* ps;
        AnnaliumSedes  sedes;
               chorda  causa;

        ps = piscina_generare_dynamicum("frigida_sedes", 65536);
        si (   ps == NIHIL
            || !annales_sedem_invenire(".", ps, &sedes, &causa))
        {
            si (ps != NIHIL)
            {
                fprintf(stderr, "nota_frigida: RECUSATUM - %.*s\n",
                    (integer)causa.mensura,
                    (constans character*)causa.datum);
            }
            redde I;
        }
        VIA_DB = chorda_ut_cstr(annales_via(&sedes,
            ANNALES_SCRINIUM_TABULARII, ps), ps);
        VIA_AN = chorda_ut_cstr(annales_via(&sedes, ANNALES_TABULARII,
            ps), ps);
        VIA_TABULAE = chorda_ut_cstr(annales_via(&sedes, ANNALES_TABULA,
            ps), ps);
        VIA_ENTIUM = chorda_ut_cstr(annales_via(&sedes, ANNALES_ENTIA,
            ps), ps);
        si (argc >= II && strcmp(argv[I], "-sedes") == ZEPHYRUM)
        {
            redde _sedes_imprimere(&sedes, ps);
        }
        si (argc >= II && strcmp(argv[I], "-restituere") == ZEPHYRUM)
        {
            redde _restituere(piscina);
        }
        si (argc >= II && strcmp(argv[I], "-genesis") == ZEPHYRUM)
        {
            redde _genesis(piscina);
        }
        /* CUSTOS: via frigida numquam gignit (VSY50E) - annales sine
         * scrinio aut absentes recusantur, nihil scriptum */
        si (   argc >= II
            && !annales_custodire(&sedes, ANNALES_TABULARII,
                   ANNALES_SCRINIUM_TABULARII, FALSUM, ps, &causa))
        {
            fprintf(stderr, "nota_frigida: RECUSATUM - %.*s\n",
                (integer)causa.mensura,
                (constans character*)causa.datum);
            redde I;
        }
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
        cfg.via_tabulae       = VIA_TABULAE;
        cfg.via_entitatum     = VIA_ENTIUM;
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
            " [-origo O] -nexus <res> <verbum> <alterum>\n     aut:"
            " nota_frigida -res <res>   (lectio, nihil scribit)\n");
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
