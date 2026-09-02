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
#include "atrium.h"     /* fenestra+thema+vitrea+capsula+imperium intus */
#include "vitrea.h"     /* imaginem_petere: atrium partes suas non celat */
#include "capsula_mensor.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TICTUS_MS XVI
#define CAMPI_MAXIMI VIII

/* Photographia perfecta: tractator asynchronus hoc ponit */
interior b32 _imago_facta = FALSUM;

interior vacuum
_imago_tractare (
    vacuum* datum,
       b32  successus)
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
_vexillum (
               integer   argc,
             character** argv,
    constans character*  clavis)
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
_arg (
               integer   argc,
             character** argv,
    constans character*  clavis)
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
_campos (
    chorda  datum,
    chorda* campi,
       i32  maximum)
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
            campi[numerus].datum    = datum.datum + initium;
            campi[numerus].mensura  = i - initium;
            numerus++;
            initium = i + I;
        }
    }

    redde numerus;
}

/* Numerum ex campo RELEGERE et REEMITTERE: ita JSON validum esse
 * garantitur etiam si actum aliquando corruptum sit */
interior vacuum
_numerum (
    ChordaAedificator* a,
               chorda  campus,
                  b32  integrum)
{
    character  tabula[LXIV];
    character* finis;
       duplex  valor;

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
_par (
     ChordaAedificator* a,
    constans character* clavis,
                chorda  valor,
                   b32  primus)
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
_datum_struere (
    Volumen* volumen,
    Piscina* piscina)
{
    ChordaAedificator* a;
                  Xar* acta;
                  i32  index;
                  b32  primus;

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
                  chorda  campi[CAMPI_MAXIMI];
                     i32  numerus;

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
                  chorda  campi[CAMPI_MAXIMI];
                     i32  numerus;

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
main (
      integer   argc,
    character** argv)
{
               Piscina* piscina;
    constans character* via;
               Volumen* volumen;
                chorda  datum;
    AtriumConfiguratio  figura;
                Atrium* atrium;
                chorda  causa;
     ChordaAedificator* iniectio;
             character  tabula_viae[DXII];
    constans character* imago_via;
                   s32  mora_picturae;   /* SIGNATUM: -I = nondum petita */

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
                "  (curre ./compile_tests.sh ut mensurae fiant)\n",
                via);
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

    /* Atrium: fenestra + capsula + vitrea + internuntius + imperium
     * uno vocamine, ordine recto (vide atrium.h). Quod hic NON est
     * mensura est: ~LXX lineae quas quaeque app vitreae describebat. */
    memset(&figura, 0, magnitudo(figura));
    figura.titulus        = "mensor";
    figura.latitudo       = 1000;
    figura.altitudo       = 760;
    figura.capsula        = &capsula_mensor;
    figura.via_initialis  = "index.html";
    /* Tictus CELER de industria: ordinarius atrii CC ms est, sed
     * -imago numerationem tictuum ut moram adhibet, et haec facies
     * nihil computat quod tardius pulsare mereatur. */
    figura.tictus_ms     = (Mora)TICTUS_MS;

    /* -vivum, -portus: vexilla domus. MODUS EVOLUTIONIS SOLUS -
     * JS arbitrarium exsequitur (vide imperium.h). */
    atrium_vexilla_legere(&figura, argc, argv);

    atrium = atrium_creare(piscina, &figura, &causa);
    si (atrium == NIHIL)
    {
        fprintf(stderr, "mensor_ui: %.*s\n",
                (integer)causa.mensura,
                (constans character*)causa.datum);
        piscina_destruere(piscina);
        redde I;
    }

    si (atrium_portus(atrium) != ZEPHYRUM)
    {
        imprimere("[mensor_ui] imperium: http://127.0.0.1:%d/imperium\n",
                  (integer)atrium_portus(atrium));
        fflush(stdout);
    }

    /* Iniectio praeparata SEMEL: datum non mutatur dum fenestra vivit.
     * Atrium eam COPIAT et post quamque 'paratus' rursus currit - ergo
     * recargatio post interitum processus interretialis eam NON amittit
     * (quod codex prior amittebat, sed nemo vidit quia datum mensoris
     * numquam mutatur). */
    iniectio = chorda_aedificator_creare(piscina,
        (memoriae_index)(XVI * M));
    chorda_aedificator_appendere_literis(iniectio, "window.MENSURAE=");
    chorda_aedificator_appendere_chorda(iniectio, datum);
    chorda_aedificator_appendere_literis(
        iniectio, ";if(window.reddere){window.reddere();}");

    atrium_iniectionem_ponere(atrium,
        chorda_aedificator_spectare(iniectio));

    imago_via      = _arg(argc, argv, "-imago");
    mora_picturae  = (s32)(-I);

    atrium_monstrare(atrium);

    dum (atrium_currendum(atrium))
    {
        i32 actum = atrium_gressus(atrium);

        /* Paginam pingere sinere ANTE photographiam: iniectio
         * synchrona non est */
        si (   (actum & (i32)ATRIUM_ACTUM_PARATA) != ZEPHYRUM
            && imago_via != NIHIL && mora_picturae < ZEPHYRUM)
        {
            mora_picturae = (s32)X;
        }

        si (mora_picturae > ZEPHYRUM)
        {
            mora_picturae--;
        }
        alioquin si (mora_picturae == ZEPHYRUM)
        {
            mora_picturae = (s32)(-I);
            vitrea_imaginem_petere(atrium_vitrea(atrium), imago_via,
                                   _imago_tractare, NIHIL);
        }

        si (_imago_facta)
        {
            frange;
        }
    }

    atrium_destruere(atrium);
    piscina_destruere(piscina);

    redde 0;
}
