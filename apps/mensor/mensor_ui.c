/*
 * mensor_ui.c - Explorator mensurarum (vitrea)
 *
 * BINARIUM SEORSUM CONSULTO. 'mensor' in via calida CL vicibus per
 * aedificationem vocatur; si vitrea in idem binarium ligaretur,
 * vocatio quaeque AppKit et WebKit ad exec ligaret - etiam cum
 * facies numquam aperiatur. Id est ipsum vitium observatoris quod
 * diarium vitare debebat, per posticum reductum.
 * Ergo: mensor tenuis manet, 'mensor -ui' hoc exsequitur.
 *
 * LEGIT SOLUM. Nihil scribit, nihil mutat.
 */

#include "postulata_posix.h"

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "xar.h"
#include "volumen.h"
#include "fenestra.h"
#include "thema.h"
#include "vitrea.h"
#include "capsula.h"
#include "capsula_mensor.h"
#include "imperium.h"   /* vivarium: hospitium+internuntius intus */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TICTUS_MS XVI
#define CAMPI_MAXIMI VIII

/* Photographia perfecta: tractator asynchronus hoc ponit */
interior b32 _imago_facta = FALSUM;

interior vacuum
_imago_tractare (vacuum* datum, b32 successus)
{
    (vacuum)datum;
    si (!successus)
    {
        fprintf(stderr, "mensor_ui: imago capi non potuit\n");
    }
    _imago_facta = VERUM;
}

/* Vexillum NUDUM ubicumque in argumentis. _arg valorem SEQUENTEM
 * poscit, ergo vexillum sine valore ei invisibile est nisi primum
 * stat - vitium quod fenestram tacite aperiebat cum '-json' post
 * '-volumen X' veniret. */
interior b32
_vexillum (integer argc, character** argv, constans character* clavis)
{
    integer i;

    per (i = I; i < argc; i++)
    {
        si (strcmp(argv[i], clavis) == 0)
        {
            redde VERUM;
        }
    }

    redde FALSUM;
}

interior constans character*
_arg (integer argc, character** argv, constans character* clavis)
{
    integer i;

    per (i = I; i < argc - I; i++)
    {
        si (strcmp(argv[i], clavis) == 0)
        {
            redde argv[i + I];
        }
    }

    redde NIHIL;
}

/* Lineam TSV in campos secare. Redde numerum camporum. */
interior i32
_campos (chorda datum, chorda* campi, i32 maximum)
{
    i32 numerus, initium, i;

    numerus = ZEPHYRUM;
    initium = ZEPHYRUM;

    per (i = ZEPHYRUM; i <= datum.mensura; i++)
    {
        si (i == datum.mensura || datum.datum[i] == '\t')
        {
            si (numerus >= maximum)
            {
                frange;
            }
            campi[numerus].datum   = datum.datum + initium;
            campi[numerus].mensura = i - initium;
            numerus++;
            initium = i + I;
        }
    }

    redde numerus;
}

/* Numerum ex campo RELEGERE et REEMITTERE: ita JSON validum esse
 * garantitur etiam si actum aliquando corruptum sit */
interior vacuum
_numerum (ChordaAedificator* a, chorda campus, b32 integrum)
{
    character tabula[LXIV];
    character* finis;
    duplex     valor;

    si (campus.mensura == ZEPHYRUM || campus.mensura >= (i32)LXIV)
    {
        chorda_aedificator_appendere_literis(a, "0");
        redde;
    }

    memcpy(tabula, campus.datum, (size_t)campus.mensura);
    tabula[campus.mensura] = '\0';

    valor = strtod(tabula, &finis);
    si (finis == tabula)
    {
        chorda_aedificator_appendere_literis(a, "0");
        redde;
    }

    si (integrum)
    {
        sprintf(tabula, "%.0f", valor);
    }
    alioquin
    {
        sprintf(tabula, "%.6f", valor);
    }
    chorda_aedificator_appendere_literis(a, tabula);
}

interior vacuum
_par (ChordaAedificator* a, constans character* clavis, chorda valor,
      b32 primus)
{
    si (!primus)
    {
        chorda_aedificator_appendere_character(a, ',');
    }
    chorda_aedificator_appendere_character(a, '"');
    chorda_aedificator_appendere_literis(a, clavis);
    chorda_aedificator_appendere_literis(a, "\":\"");
    chorda_aedificator_appendere_evasus_json(a, valor);
    chorda_aedificator_appendere_character(a, '"');
}

/*
 * _datum_struere - Acta in JSON vertere
 */
interior chorda
_datum_struere (Volumen* volumen, Piscina* piscina)
{
    ChordaAedificator* a;
    Xar*               acta;
    i32                index;
    b32                primus;

    a = chorda_aedificator_creare(piscina, (memoriae_index)(XVI * M));
    si (a == NIHIL)
    {
        redde chorda_ex_literis("{\"sessiones\":[],\"mensurae\":[]}",
                                piscina);
    }

    acta = volumen_acta_legere(volumen, (s64)0, piscina);

    chorda_aedificator_appendere_literis(a, "{\"sessiones\":[");
    primus = VERUM;
    si (acta != NIHIL)
    {
        per (index = ZEPHYRUM; index < xar_numerus(acta); index++)
        {
            VolumenActum* actum;
            chorda        campi[CAMPI_MAXIMI];
            i32           numerus;

            actum = (VolumenActum*)xar_obtinere(acta, index);
            si (!chorda_aequalis_literis(actum->genus, "sessio"))
            {
                perge;
            }

            numerus = _campos(actum->datum, campi, (i32)CAMPI_MAXIMI);
            si (numerus < (i32)V)
            {
                perge;
            }

            si (!primus)
            {
                chorda_aedificator_appendere_character(a, ',');
            }
            chorda_aedificator_appendere_character(a, '{');
            _par(a, "clavis", campi[0], VERUM);
            _par(a, "machina", campi[I], FALSUM);
            _par(a, "commissum", campi[II], FALSUM);
            _par(a, "systema", campi[III], FALSUM);
            _par(a, "architectura", campi[IV], FALSUM);
            chorda_aedificator_appendere_character(a, '}');
            primus = FALSUM;
        }
    }

    chorda_aedificator_appendere_literis(a, "],\"mensurae\":[");
    primus = VERUM;
    si (acta != NIHIL)
    {
        per (index = ZEPHYRUM; index < xar_numerus(acta); index++)
        {
            VolumenActum* actum;
            chorda        campi[CAMPI_MAXIMI];
            i32           numerus;

            actum = (VolumenActum*)xar_obtinere(acta, index);
            si (!chorda_aequalis_literis(actum->genus, "mensura"))
            {
                perge;
            }

            numerus = _campos(actum->datum, campi, (i32)CAMPI_MAXIMI);
            si (numerus < (i32)VII)
            {
                perge;
            }

            si (!primus)
            {
                chorda_aedificator_appendere_character(a, ',');
            }
            chorda_aedificator_appendere_character(a, '{');
            _par(a, "sessio", campi[0], VERUM);
            _par(a, "id", campi[I], FALSUM);
            _par(a, "parens", campi[II], FALSUM);
            _par(a, "titulus", campi[III], FALSUM);
            chorda_aedificator_appendere_literis(a, ",\"valor\":");
            _numerum(a, campi[IV], FALSUM);
            _par(a, "unitas", campi[V], FALSUM);
            chorda_aedificator_appendere_literis(a, ",\"momentum\":");
            _numerum(a, campi[VI], VERUM);
            chorda_aedificator_appendere_character(a, '}');
            primus = FALSUM;
        }
    }

    chorda_aedificator_appendere_literis(a, "]}");

    redde chorda_aedificator_finire(a);
}

integer
main (integer argc, character** argv)
{
    Piscina*             piscina;
    constans character*  via;
    Volumen*             volumen;
    chorda               datum;
    FenestraConfiguratio figura_fenestrae;
    VitreaConfiguratio   figura_vitreae;
    Fenestra*            fenestra;
    Capsula*             capsula;
    Vitrea*              vitrea;
    ChordaAedificator*   iniectio;
    character            tabula_viae[DXII];
    constans character*  imago_via;
    s32                  mora_picturae;   /* SIGNATUM: -I = nondum petita */
    Vivarium             vivarium;

    piscina = piscina_generare_dynamicum("mensor_ui", M * M * XVI);
    si (piscina == NIHIL)
    {
        fprintf(stderr, "mensor_ui: piscina fracta\n");
        redde I;
    }

    via = _arg(argc, argv, "-volumen");
    si (via == NIHIL)
    {
        via = getenv("MENSOR_VOLUMEN");
    }
    si (via == NIHIL || via[0] == '\0')
    {
        constans character* domus = getenv("HOME");
        sprintf(tabula_viae, "%s/.rhubarb/mensurae.volumen",
                (domus != NIHIL) ? domus : "/tmp");
        via = tabula_viae;
    }

    volumen = volumen_aperire(piscina, via);
    si (volumen == NIHIL)
    {
        fprintf(stderr,
                "mensor_ui: volumen aperiri non potest: %s\n"
                "  (curre ./compile_tests.sh ut mensurae fiant)\n", via);
        piscina_destruere(piscina);
        redde I;
    }

    datum = _datum_struere(volumen, piscina);
    volumen_claudere(volumen);

    /* -json: semita datorum SINE fenestra probari potest. Pars dura
     * (volumen -> JSON) ita in probatione automatica vivit; sola
     * pictura oculos poscit. */
    si (_vexillum(argc, argv, "-json"))
    {
        fwrite(datum.datum, (size_t)I, (size_t)datum.mensura, stdout);
        imprimere("\n");
        piscina_destruere(piscina);
        redde 0;
    }

    thema_initiare();

    figura_fenestrae.titulus  = "mensor";
    figura_fenestrae.x        = CC;
    figura_fenestrae.y        = CC;
    figura_fenestrae.latitudo = 1000;
    figura_fenestrae.altitudo = 760;
    figura_fenestrae.vexilla  = FENESTRA_CLAUDIBILIS
                              | FENESTRA_MUTABILIS | FENESTRA_CENTRATA;

    fenestra = fenestra_creare(piscina, &figura_fenestrae);
    si (fenestra == NIHIL)
    {
        fprintf(stderr, "mensor_ui: fenestra fracta\n");
        piscina_destruere(piscina);
        redde I;
    }

    capsula = capsula_aperire(&capsula_mensor, piscina);
    si (capsula == NIHIL)
    {
        fprintf(stderr, "mensor_ui: capsula fracta\n");
        redde I;
    }

    figura_vitreae.origo         = VITREA_ORIGO_CAPSULA;
    figura_vitreae.capsula       = capsula;
    figura_vitreae.via_initialis = "index.html";
    figura_vitreae.url           = NIHIL;
    figura_vitreae.inspectabilis = VERUM;

    vitrea = vitrea_creare(piscina, fenestra, &figura_vitreae);
    si (vitrea == NIHIL)
    {
        fprintf(stderr, "mensor_ui: vitrea fracta\n");
        redde I;
    }

    /* -vivum: canalis imperii uno vocamine (vide imperium.h).
     * MODUS EVOLUTIONIS SOLUS - JS arbitrarium exsequitur. */
    memset(&vivarium, 0, magnitudo(vivarium));

    si (_vexillum(argc, argv, "-vivum"))
    {
        VivariumConfiguratio figura_vivarii;
        constans character*  portus_litterae;

        memset(&figura_vivarii, 0, magnitudo(figura_vivarii));
        portus_litterae = _arg(argc, argv, "-portus");
        figura_vivarii.portus     = (portus_litterae != NIHIL)
                                  ? (i32)atoi(portus_litterae) : ZEPHYRUM;
        figura_vivarii.aestimator = vitrea_aestimator;
        figura_vivarii.imaginator = vitrea_imaginator;
        figura_vivarii.missor     = vitrea_missor;
        figura_vivarii.datum      = vitrea;
        figura_vivarii.fenestra   = fenestra_numerus_nativus(fenestra);

        vivarium = imperium_vivarium(piscina, &figura_vivarii);
        si (!vivarium.successus)
        {
            fprintf(stderr, "mensor_ui: imperium: %.*s\n",
                    (integer)vivarium.causa.mensura,
                    (constans character*)vivarium.causa.datum);
            redde I;
        }

        imprimere("[mensor_ui] imperium: http://127.0.0.1:%d/imperium\n",
                  (integer)vivarium.portus);
        fflush(stdout);
    }

    /* Iniectio praeparata SEMEL: datum non mutatur dum fenestra vivit */
    iniectio = chorda_aedificator_creare(piscina, (memoriae_index)(XVI * M));
    chorda_aedificator_appendere_literis(iniectio, "window.MENSURAE=");
    chorda_aedificator_appendere_chorda(iniectio, datum);
    chorda_aedificator_appendere_literis(
        iniectio, ";if(window.reddere){window.reddere();}");

    imago_via     = _arg(argc, argv, "-imago");
    mora_picturae = (s32)(-I);

    fenestra_monstrare(fenestra);

    dum (!fenestra_debet_claudere(fenestra))
    {
        Eventus            eventus;
        chorda             nuntium;
        VitreaNuntiusGenus genus;

        fenestra_expectare_eventus(fenestra, (i32)TICTUS_MS);

        dum (fenestra_obtinere_eventus(fenestra, &eventus))
        {
            /* eventus fenestrae: vitrea eos ipsa tractat */
        }

        vivarium_gressus(&vivarium);

        dum (vitrea_obtinere_nuntium(vitrea, &nuntium, &genus))
        {
            si (genus == VITREA_NUNTIUS_PONS)
            {
                /* Pagina se paratam nuntiat. Sine hoc nuntio
                 * iniectio cum onere certaret et interdum
                 * paginam vacuam inveniret. */
                vitrea_aestimare(vitrea,
                    chorda_aedificator_spectare(iniectio));

                /* Nuntium 'paratus' chorda NUDA est, non JSON-RPC.
                 * Sine hac custodia internuntius de eo queritur
                 * ('culpa orba') in consola omni onere - strepitus
                 * qui culpas VERAS obtegeret. */
                /* Nuntium 'paratus' chorda NUDA est, non JSON-RPC.
                 * Sine hac custodia internuntius de eo queritur
                 * ('culpa orba') in consola omni onere - strepitus
                 * qui culpas VERAS obtegeret. */
                si (vivarium.internuntius != NIHIL &&
                    nuntium.mensura > ZEPHYRUM &&
                    nuntium.datum[0] == '{')
                {
                    internuntius_tractare(vivarium.internuntius,
                                          nuntium, piscina);
                }

                /* Paginam pingere sinere ANTE photographiam:
                 * iniectio synchrona non est */
                si (imago_via != NIHIL && mora_picturae < ZEPHYRUM)
                {
                    mora_picturae = (s32)X;
                }
            }
            alioquin
            {
                vitrea_recargare(vitrea);
            }
        }

        si (mora_picturae > ZEPHYRUM)
        {
            mora_picturae--;
        }
        alioquin si (mora_picturae == ZEPHYRUM)
        {
            mora_picturae = (s32)(-I);
            vitrea_imaginem_petere(vitrea, imago_via,
                                   _imago_tractare, NIHIL);
        }

        si (_imago_facta)
        {
            frange;
        }
    }

    vitrea_destruere(vitrea);
    fenestra_destruere(fenestra);
    piscina_destruere(piscina);

    redde 0;
}
