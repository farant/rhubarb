/* neumae.c - cantum gabc in imaginem PNG pingere, sine fenestra (gabc
 * pars II, P4): oculi Claudii et Frani in eadem pictura quam tabula
 * antiphonale pingit (gabc_compositio + gabc_pictura).
 *
 *   neumae VIA.gabc -imago VIA.png [-latitudo N] [-altitudo N]
 *          [-spatium S] [-pagina P] [-illuminare SYLLABA] [-scala K]
 *
 * Pagina P (I-basata, ordinarie I) forma ordinaria (CMLX x DLXXIII,
 * spatium VIII) ad pixela logica I x; -scala K (I..VIII) imaginem per
 * proximum amplificat (II = quod vicus in Retina monstrat). Cantus
 * cum VITIO: vitia in stderr, vexillum rubrum in summo imaginis
 * (vitium primum), pingit quod potest, exitus 1. stdout: linea una
 * (imago, pagina/paginae, mandata).
 *
 * Exitus: 0 bene | 1 cantus cum vitio (imago tamen scripta) | 2 usus
 * malus, plagula illegibilis, pagina extra, scriptura fracta.
 *
 * Fumus: tools/neumae_fumus.sh (porta neumae-fumus).
 */
/* plagula provenientiae (fabrica T7): '-provenientia' respondetur -
 * sine ea installatio (sanare installata) instrumentum recusat */
/* <aedilis obiectum="build/fabrica/provenientia/neumae.c"/> */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "filum.h"
#include "gabc.h"
#include "gabc_compositio.h"
#include "gabc_pictura.h"
#include "mandatum.h"
#include "delineare_mandata.h"
#include "imago_png.h"
#include "internamentum.h"
#include "thema.h"
#include "figura.h"
#include "fons.h"
#include "provenientia.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALTITUDO_VEXILLI  XII

externus constans ProvenientiaRelatio provenientia_neumae;

interior vacuum
usum_monstrare (vacuum)
{
    fprintf(stderr, "usus: neumae VIA.gabc -imago VIA.png [-latitudo N]"
        " [-altitudo N] [-spatium S] [-pagina P] [-illuminare S]"
        " [-scala K]\n");
}

/* numerus integer positivus ex argumento; -1 si malus */
interior s32
numerum_legere (
    constans character* s)
{
    s32 n;

    si (s == NIHIL || *s == '\0')
    {
        redde -I;
    }
    n = ZEPHYRUM;
    dum (*s)
    {
        si (*s < '0' || *s > '9' || n > M * M)
        {
            redde -I;
        }
        n = n * X + (*s - '0');
        s++;
    }
    redde n;
}

/* tabula per proximum amplificata */
interior TabulaPixelorum*
amplificare (
     TabulaPixelorum* fons,
             Piscina* piscina,
                 i32  latitudo,
                 i32  altitudo,
                 i32  scala)
{
    TabulaPixelorum* t;
                i32  x;
                i32  y;

    t = tabula_pixelorum_creare_nuda(piscina, latitudo * scala,
        altitudo * scala);
    si (t == NIHIL)
    {
        redde NIHIL;
    }
    per (y = ZEPHYRUM; y < altitudo * scala; y++)
    {
        per (x = ZEPHYRUM; x < latitudo * scala; x++)
        {
            tabula_pixelorum_ponere_pixelum(t, x, y,
                tabula_pixelorum_obtinere_pixelum(fons, x / scala,
                    y / scala));
        }
    }
    redde t;
}

/* vexillum vitii: fascia COLOR_ERROR, linea prima vitiorum */
interior vacuum
vexillum_pingere (
                Mandata* md,
                 chorda  vitia,
                    i32  latitudo)
{
     Fines f;
    chorda linea;
       i32 i;

    f.x         = ZEPHYRUM;
    f.y         = ZEPHYRUM;
    f.latitudo  = (s32)latitudo;
    f.altitudo  = ALTITUDO_VEXILLI;
    mandata_rectangulum(md, f, figura_color_thematis(COLOR_ERROR),
        VERUM);
    linea = vitia;
    per (i = ZEPHYRUM; i < vitia.mensura; i++)
    {
        si (vitia.datum[i] == '\n')
        {
            linea.mensura = i;
            frange;
        }
    }
    mandata_textus(md, II, II, linea, FONS_PARVUS,
        figura_color_thematis(COLOR_BACKGROUND));
}

s32
principale (
          s32   argc,
    character** argv)
{
    GabcCompositionisForma  forma;
         GabcPicturaStatus  status;
        constans character* via_gabc;
        constans character* via_imago;
                   Piscina* piscina;
                    chorda  textus;
                GabcCantus* cantus;
            GabcCompositio* comp;
                   Mandata* md;
           TabulaPixelorum* tp;
                     Imago  imago;
                PngFructus  png;
         ChordaAedificator* a;
                    chorda  vitia;
                     Fines  f;
                       b32  successus;
                       s32  pagina;
                       s32  scala;
                       s32  valor;
                       s32  i;

    si (provenientia_respondere(argc, argv, &provenientia_neumae))
    {
        redde ZEPHYRUM;
    }
    via_gabc   = NIHIL;
    via_imago  = NIHIL;
    pagina     = I;
    scala      = I;
    gabc_compositionis_formam_ordinariam(&forma);
    status.syllaba_illuminata  = -I;
    status.nota_illuminata     = -I;
    per (i = I; i < argc; i++)
    {
        si (strcmp(argv[i], "-imago") == ZEPHYRUM && i + I < argc)
        {
            via_imago = argv[++i];
        }
        alioquin si (   strcmp(argv[i], "-latitudo") == ZEPHYRUM
                     && i + I < argc)
        {
            valor = numerum_legere(argv[++i]);
            si (valor < ZEPHYRUM)
            {
                usum_monstrare();
                redde II;
            }
            forma.latitudo = (i32)valor;
        }
        alioquin si (   strcmp(argv[i], "-altitudo") == ZEPHYRUM
                     && i + I < argc)
        {
            valor = numerum_legere(argv[++i]);
            si (valor <= ZEPHYRUM)
            {
                usum_monstrare();
                redde II;
            }
            forma.altitudo = (i32)valor;
        }
        alioquin si (   strcmp(argv[i], "-spatium") == ZEPHYRUM
                     && i + I < argc)
        {
            valor = numerum_legere(argv[++i]);
            si (valor < ZEPHYRUM)
            {
                usum_monstrare();
                redde II;
            }
            forma.spatium = (i32)valor;
        }
        alioquin si (   strcmp(argv[i], "-pagina") == ZEPHYRUM
                     && i + I < argc)
        {
            pagina = numerum_legere(argv[++i]);
            si (pagina < I)
            {
                usum_monstrare();
                redde II;
            }
        }
        alioquin si (   strcmp(argv[i], "-scala") == ZEPHYRUM
                     && i + I < argc)
        {
            scala = numerum_legere(argv[++i]);
            si (scala < I || scala > VIII)
            {
                usum_monstrare();
                redde II;
            }
        }
        alioquin si (   strcmp(argv[i], "-illuminare") == ZEPHYRUM
                     && i + I < argc)
        {
            status.syllaba_illuminata = numerum_legere(argv[++i]);
        }
        alioquin si (argv[i][ZEPHYRUM] != '-' && via_gabc == NIHIL)
        {
            via_gabc = argv[i];
        }
        alioquin
        {
            usum_monstrare();
            redde II;
        }
    }
    si (via_gabc == NIHIL || via_imago == NIHIL)
    {
        usum_monstrare();
        redde II;
    }

    piscina = piscina_generare_dynamicum("neumae",
        (memoriae_index)(XVI * MXXIV * MXXIV));
    si (piscina == NIHIL || !filum_existit(via_gabc))
    {
        fprintf(stderr, "neumae: %s: legi non potest\n", via_gabc);
        redde II;
    }
    textus         = filum_legere_totum(via_gabc, piscina);
    cantus         = gabc_legere(textus, piscina);
    successus      = cantus != NIHIL && gabc_successus(cantus);
    vitia.mensura  = ZEPHYRUM;
    vitia.datum    = NIHIL;
    si (cantus != NIHIL && !successus)
    {
        a = chorda_aedificator_creare(piscina, MMM);
        gabc_vitia_scribere(cantus, via_gabc, a);
        vitia = chorda_aedificator_finire(a);
        fprintf(stderr, "%.*s", (integer)vitia.mensura,
            (constans character*)vitia.datum);
    }
    comp = cantus ? gabc_componere(cantus, &forma, piscina) : NIHIL;
    si (comp == NIHIL)
    {
        fprintf(stderr, "neumae: forma mala (spatium par VI..XLVIII,"
            " latitudo >= XVI * spatium)\n");
        redde II;
    }
    si (pagina > (s32)comp->numerus_paginarum)
    {
        fprintf(stderr, "neumae: pagina %d extra (paginae %u)\n",
            (integer)pagina, (unsigned)comp->numerus_paginarum);
        redde II;
    }

    thema_initiare();
    md = mandata_creare(piscina,
        internamentum_creare(piscina));
    f.x         = ZEPHYRUM;
    f.y         = ZEPHYRUM;
    f.latitudo  = (s32)forma.latitudo;
    f.altitudo  = (s32)forma.altitudo;
    mandata_rectangulum(md, f, figura_color_thematis(COLOR_BACKGROUND),
        VERUM);
    gabc_paginam_pingere(comp, (i32)(pagina - I), ZEPHYRUM, ZEPHYRUM,
        &status, md);
    si (!successus)
    {
        vexillum_pingere(md, vitia, forma.latitudo);
    }
    tp = tabula_pixelorum_creare_nuda(piscina, forma.latitudo,
        forma.altitudo);
    si (tp == NIHIL)
    {
        redde II;
    }
    delineare_mandata(md, tp, NIHIL, NIHIL);
    si (scala > I)
    {
        tp = amplificare(tp, piscina, forma.latitudo, forma.altitudo,
            (i32)scala);
        si (tp == NIHIL)
        {
            redde II;
        }
    }
    imago  = imago_ex_tabula(tp);
    png    = imago_png_scribere(&imago, via_imago, piscina);
    si (!png.successus)
    {
        fprintf(stderr, "neumae: %s: scribi non potest\n", via_imago);
        redde II;
    }
    imprimere("neumae: %s: pagina %d/%u, %u mandata, %u x %u\n",
        via_imago,
        (integer)pagina, (unsigned)comp->numerus_paginarum,
        (unsigned)mandata_numerus(md), (unsigned)forma.latitudo,
        (unsigned)forma.altitudo);
    piscina_destruere(piscina);
    redde successus ? ZEPHYRUM : I;
}
