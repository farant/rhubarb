/* statuae - instrumentum: statuae (simulacra GLB) in volumine vici et
 * quadra earum in PNG (project-specs/statuae-spec.md IV.7; plan
 * statuae-plan.md T6). Logica in lib/statuae_repositorium.c et
 * lib/statuae_quadrum.c; hic argumenta et effusio sola.
 *
 * Usus:
 *
 *   statuae addere <plagula.glb> -volumen V [-titulus T]
 *           [-locus | -res]
 *   statuae removere <titulus> -volumen V
 *   statuae enumerare -volumen V [-loci | -res]
 *   statuae inspicere <titulus> -volumen V
 *   statuae pingere <plagula.glb> -o <via.png> [-cyclus N|TITULUS]
 *           [-tempus S] [-gyrus G] [-inclinatio G] [-planum]
 *           [-modus atkinson|bayer|exemplaria|nullum|crudum]
 *           [-latitudo L] [-altitudo A]
 *
 * -volumen SEMPER nominandum: nulla via ordinaria (volumen vivum
 * Frani solum cum ipse id nominat scribitur). addere plagulam per
 * lectorem GLB probat ANTE scriptionem: recusata volumen non tangit.
 * Titulus ordinarius: nomen plagulae sine directorio et sine
 * suffixo. Titulus iam notus: octeti substituuntur.
 *
 * -locus (addere): plagula LOCUS est (latus $world in eo ambulat), non
 * persona; latus $models eum non monstrat. Titulus qui in genere
 * altero iam est recusatur. -loci (enumerare): locos pro personis.
 * -res (addere, enumerare): plagula RES est (res plastica: puteus,
 * saepes); latus $models eam in grege rerum monstrat, non inter
 * personas. Cum -locus aut -loci non datur.
 * removere genus quodlibet tollit.
 *
 * inspicere: quid in statua condita IUXTA figuram sit (generis
 * cuiuslibet): "<titulus>: N nodi, C cycli, M momenta", deinde
 * "nodus <index> <nomen>" pro quoque nodo nominato, "cyclus <index>
 * <titulus> <duratio>" et "momentum <cyclus> <titulus> <tempus>".

 *
 * pingere volumine non eget: quadrum quod latus ostenderet (charta
 * opaca). Ordinaria: 360 x 450, cyclus primus (quies si nullus),
 * tempus 0, visus ordinarius, illuminatum, modus crudum.
 *
 * Effusio: enumerare "<titulus>\t<sigillum>" per lineam, ordine
 * additionis. addere et pingere summarium in linea una.
 *
 * Exitus: 0 bene; 1 refutatum (causa nominata); 2 usus.
 *
 * Aedificatio: ./tools/statuae_struere.sh -> bin/statuae. */
/* plagula provenientiae (fabrica T7): '-provenientia' respondetur */
/* <aedilis obiectum="build/fabrica/provenientia/statuae.c"/> */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "filum.h"
#include "volumen.h"
#include "imago_png.h"
#include "statua.h"
#include "statua_glb.h"
#include "statua_pictura.h"
#include "statuae_quadrum.h"
#include "statuae_repositorium.h"
#include "provenientia.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LATITUDO_ORDINARIA 360
#define ALTITUDO_ORDINARIA 450
#define MENSURA_MAXIMA     0x1000

nomen structura {
    constans character* imperium;
    constans character* positivum;     /* plagula aut titulus */
    constans character* volumen;
    constans character* titulus;
    constans character* exitus;        /* -o */
    constans character* cyclus;
    constans character* modus;
                   f64  tempus;
                   f64  gyrus;
                   f64  inclinatio;
                   b32  planum;
                   b32  locus;         /* addere: genus locus */
                   b32  loci;          /* enumerare: loci */
                   b32  res;           /* addere, enumerare: res */
                   s32  latitudo;
                   s32  altitudo;
} Argumenta;

interior s32
usum_imprimere (vacuum)
{
    fprintf(stderr,
        "usus: statuae addere <plagula.glb> -volumen V [-titulus T]"
        " [-locus | -res]\n"
        "      statuae removere <titulus> -volumen V\n"
        "      statuae enumerare -volumen V [-loci | -res]\n"
        "      statuae inspicere <titulus> -volumen V\n"
        "      statuae pingere <plagula.glb> -o <via.png>"
        " [-cyclus N|TITULUS]\n"
        "              [-tempus S] [-gyrus G] [-inclinatio G]"
        " [-planum]\n"
        "              [-modus atkinson|bayer|exemplaria|nullum|"
        "crudum]\n"
        "              [-latitudo L] [-altitudo A]\n");
    redde II;
}

/* numerus fluitans totus; FALSUM si textus non numerus */
interior b32
fluitantem_legere (
    constans character* textus,
                   f64* exitus)
{
    character* finis = NIHIL;

    *exitus = strtod(textus, &finis);
    redde finis != textus && *finis == '\0' && *exitus == *exitus
        && *exitus > -1.0e9 && *exitus < 1.0e9;
}

/* integer in [minimum, maximum] */
interior b32
integrum_legere (
    constans character* textus,
                   s32  minimum,
                   s32  maximum,
                   s32* exitus)
{
    character* finis  = NIHIL;
       longus  v      = strtol(textus, &finis, X);

    si (   finis == textus || *finis != '\0' || v < (longus)minimum
        || v > (longus)maximum)
    {
        redde FALSUM;
    }
    *exitus = (s32)v;
    redde VERUM;
}

/* argumenta post imperium: positivum unum (si imperium vult) et
 * vexilla; FALSUM = usus */
interior b32
argumenta_legere (
       integer   argc,
     character** argv,
     Argumenta*  a)
{
    constans character* valor;
               integer  k;

    memset(a, ZEPHYRUM, magnitudo(Argumenta));
    a->gyrus       = (f64)STATUA_GYRUS_ORDINARIUS;
    a->inclinatio  = (f64)STATUA_INCLINATIO_ORDINARIA;
    a->latitudo    = LATITUDO_ORDINARIA;
    a->altitudo    = ALTITUDO_ORDINARIA;
    si (argc < II)
    {
        redde FALSUM;
    }
    a->imperium = argv[I];
    per (k = II; k < argc; k++)
    {
        si (strcmp(argv[k], "-planum") == ZEPHYRUM)
        {
            a->planum = VERUM;
            perge;
        }
        si (strcmp(argv[k], "-locus") == ZEPHYRUM)
        {
            a->locus = VERUM;
            perge;
        }
        si (strcmp(argv[k], "-loci") == ZEPHYRUM)
        {
            a->loci = VERUM;
            perge;
        }
        si (strcmp(argv[k], "-res") == ZEPHYRUM)
        {
            a->res = VERUM;
            perge;
        }
        si (argv[k][ZEPHYRUM] != '-')
        {
            si (a->positivum)
            {
                redde FALSUM;
            }
            a->positivum = argv[k];
            perge;
        }
        si (k + I >= argc)
        {
            redde FALSUM;
        }
        valor = argv[++k];
        si (strcmp(argv[k - I], "-volumen") == ZEPHYRUM)
        {
            a->volumen = valor;
        }
        alioquin si (strcmp(argv[k - I], "-titulus") == ZEPHYRUM)
        {
            a->titulus = valor;
        }
        alioquin si (strcmp(argv[k - I], "-o") == ZEPHYRUM)
        {
            a->exitus = valor;
        }
        alioquin si (strcmp(argv[k - I], "-cyclus") == ZEPHYRUM)
        {
            a->cyclus = valor;
        }
        alioquin si (strcmp(argv[k - I], "-modus") == ZEPHYRUM)
        {
            a->modus = valor;
        }
        alioquin si (strcmp(argv[k - I], "-tempus") == ZEPHYRUM)
        {
            si (!fluitantem_legere(valor, &a->tempus))
            {
                redde FALSUM;
            }
        }
        alioquin si (strcmp(argv[k - I], "-gyrus") == ZEPHYRUM)
        {
            si (!fluitantem_legere(valor, &a->gyrus))
            {
                redde FALSUM;
            }
        }
        alioquin si (strcmp(argv[k - I], "-inclinatio") == ZEPHYRUM)
        {
            si (!fluitantem_legere(valor, &a->inclinatio))
            {
                redde FALSUM;
            }
        }
        alioquin si (strcmp(argv[k - I], "-latitudo") == ZEPHYRUM)
        {
            si (!integrum_legere(valor, I, MENSURA_MAXIMA,
                &a->latitudo))
            {
                redde FALSUM;
            }
        }
        alioquin si (strcmp(argv[k - I], "-altitudo") == ZEPHYRUM)
        {
            si (!integrum_legere(valor, I, MENSURA_MAXIMA,
                &a->altitudo))
            {
                redde FALSUM;
            }
        }
        alioquin
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* plagula GLB lecta et per lectorem probata; NIHIL (causa impressa)
 * si legi aut intellegi non potest. *octeti = plagula ipsa */
interior Statua*
statuam_legere (
    constans character* via,
                chorda* octeti,
               Piscina* piscina)
{
    StatuaFructus f;

    *octeti = filum_legere_totum(via, piscina);
    si (octeti->mensura == ZEPHYRUM)
    {
        fprintf(stderr, "statuae: plagula '%s' legi non potest\n", via);
        redde NIHIL;
    }
    f = statua_glb_legere(*octeti, piscina);
    si (!f.successus)
    {
        fprintf(stderr, "statuae: %s: %.*s\n", via,
            (integer)f.error.mensura,
            (constans character*)f.error.datum);
        redde NIHIL;
    }
    redde f.statua;
}

/* titulus ordinarius: nomen plagulae sine directorio et suffixo */
interior chorda
titulum_ex_via (
    constans character* via,
               Piscina* piscina)
{
    constans character* initium = strrchr(via, '/');
    constans character* punctum;
                chorda  t;

    initium  = initium ? initium + I : via;
    punctum  = strrchr(initium, '.');
    t        = chorda_ex_literis(initium, piscina);
    si (punctum && punctum != initium)
    {
        t.mensura = (i32)(punctum - initium);
    }
    redde t;
}

/* genus ex vexillis (-locus / -loci, -res); neutrum = persona */
interior StatuaeGenus
genus_petitum (
    b32 locus,
    b32 res)
{
    si (locus)
    {
        redde STATUAE_GENUS_LOCUS;
    }
    redde res ? STATUAE_GENUS_RES : STATUAE_GENUS_STATUA;
}

interior s32
exsequi_addere (
    constans Argumenta* a,
               Volumen* volumen,
               Piscina* piscina)
{
    Statua* s;
    chorda  octeti;
    chorda  titulus;

    s = statuam_legere(a->positivum, &octeti, piscina);
    si (!s)
    {
        redde I;
    }
    titulus = a->titulus ? chorda_ex_literis(a->titulus, piscina)
                         : titulum_ex_via(a->positivum, piscina);
    si (!statuae_titulus_validus(titulus))
    {
        fprintf(stderr, "statuae: titulus '%.*s' invalidus (I..LXIV"
            " octeti, sine '/'); da -titulus\n",
            (integer)titulus.mensura,
            (constans character*)titulus.datum);
        redde I;
    }
    si (!statuae_addere_generis(volumen, titulus, octeti,
            genus_petitum(a->locus, a->res)))
    {
        fprintf(stderr, "statuae: volumen additionem recusavit"
            " (titulus fortasse in genere alio iam est: persona,"
            " locus, res): %s\n",
            volumen_error(volumen) ? volumen_error(volumen) : "?");
        redde I;
    }
    imprimere("%.*s: %u nodi, %u trianguli, %u cycli\n",
        (integer)titulus.mensura, (constans character*)titulus.datum,
        (insignatus)s->nodorum, (insignatus)(s->indicum / III),
        (insignatus)s->cyclorum);
    redde ZEPHYRUM;
}

interior s32
exsequi_enumerare (
         Volumen* volumen,
    StatuaeGenus  genus,
         Piscina* piscina)
{
                  Xar* index = statuae_enumerare_generis(volumen,
                      genus, piscina);
    StatuaeInscriptio* x;
                  i32  k;

    per (k = ZEPHYRUM; index && k < xar_numerus(index); k++)
    {
        x = (StatuaeInscriptio*)xar_obtinere(index, k);
        imprimere("%.*s\t%.*s\n", (integer)x->titulus.mensura,
            (constans character*)x->titulus.datum,
            (integer)x->sigillum_hex.mensura,
            (constans character*)x->sigillum_hex.datum);
    }
    redde ZEPHYRUM;
}

/* nomina nodorum, cycli et momenta statuae conditae */
interior s32
exsequi_inspicere (
    constans Argumenta* a,
               Volumen* volumen,
               Piscina* piscina)
{
    StatuaeInscriptio* inventa = NIHIL;
        StatuaFructus  f;
               chorda  titulus =
                   chorda_ex_literis(a->positivum, piscina);
               chorda octeti;
                  b32 adest = FALSUM;
                  i32 genus;
                  i32 k;

    per (genus = ZEPHYRUM; genus < STATUAE_GENERUM && !inventa; genus++)
    {
        Xar* index = statuae_enumerare_generis(volumen,
            (StatuaeGenus)genus, piscina);

        per (k = ZEPHYRUM; index && k < xar_numerus(index); k++)
        {
            StatuaeInscriptio* x =
                (StatuaeInscriptio*)xar_obtinere(index, k);

            si (chorda_aequalis(x->titulus, titulus))
            {
                inventa = x;
                frange;
            }
        }
    }
    si (!inventa)
    {
        fprintf(stderr, "statuae: '%s' in volumine non est\n",
            a->positivum);
        redde I;
    }
    octeti = volumen_massam_promere(volumen, inventa->sigillum_hex,
        piscina, &adest);
    si (!adest)
    {
        fprintf(stderr, "statuae: massa '%s' in volumine deest\n",
            a->positivum);
        redde I;
    }
    f = statua_glb_legere(octeti, piscina);
    si (!f.successus)
    {
        fprintf(stderr, "statuae: %s: %.*s\n", a->positivum,
            (integer)f.error.mensura,
            (constans character*)f.error.datum);
        redde I;
    }
    imprimere("%s: %u nodi, %u cycli, %u momenta\n", a->positivum,
        (insignatus)f.statua->nodorum, (insignatus)f.statua->cyclorum,
        (insignatus)f.momentorum);
    per (k = ZEPHYRUM; k < f.statua->nodorum; k++)
    {
        si (f.tituli[k].mensura)
        {
            imprimere("nodus %u %.*s\n", (insignatus)k,
                (integer)f.tituli[k].mensura,
                (constans character*)f.tituli[k].datum);
        }
    }
    per (k = ZEPHYRUM; k < f.statua->cyclorum; k++)
    {
        constans StatuaCyclus* c = &f.statua->cycli[k];

        imprimere("cyclus %u %.*s %.2f\n", (insignatus)k,
            (integer)c->titulus.mensura,
            (constans character*)c->titulus.datum, (f64)c->duratio);
    }
    per (k = ZEPHYRUM; k < f.momentorum; k++)
    {
        constans StatuaCyclus* c =
            &f.statua->cycli[f.momenta[k].cyclus];

        imprimere("momentum %.*s %.*s %.2f\n",
            (integer)c->titulus.mensura,
            (constans character*)c->titulus.datum,
            (integer)f.momenta[k].titulus.mensura,
            (constans character*)f.momenta[k].titulus.datum,
            (f64)f.momenta[k].tempus);
    }
    redde ZEPHYRUM;
}

/* cyclus ex argumento: numerus aut titulus; absens = primus (quies
 * si statua cyclos non habet). FALSUM (cycli impressi) si ignotus */
interior b32
cyclum_legere (
    constans Argumenta* a,
       constans Statua* s,
                   s32* cyclus)
{
    i32 k;

    *cyclus = s->cyclorum > ZEPHYRUM ? ZEPHYRUM : -I;
    si (!a->cyclus)
    {
        redde VERUM;
    }
    si (integrum_legere(a->cyclus, -I, (s32)s->cyclorum - I, cyclus))
    {
        redde VERUM;
    }
    per (k = ZEPHYRUM; k < s->cyclorum; k++)
    {
        si (chorda_aequalis_literis(s->cycli[k].titulus, a->cyclus))
        {
            *cyclus = (s32)k;
            redde VERUM;
        }
    }
    fprintf(stderr, "statuae: cyclus '%s' ignotus; cycli:\n",
        a->cyclus);
    per (k = ZEPHYRUM; k < s->cyclorum; k++)
    {
        fprintf(stderr, "  %u  %.*s  (%.2f s)\n", (insignatus)k,
            (integer)s->cycli[k].titulus.mensura,
            (constans character*)s->cycli[k].titulus.datum,
            (f64)s->cycli[k].duratio);
    }
    redde FALSUM;
}

interior s32
exsequi_pingere (
    constans Argumenta* a,
               Piscina* piscina)
{
    StatuaeQuadrumPetitio  p;
                   Statua* s;
                   chorda  octeti;
                    Imago  imago;
               PngFructus  png;

    memset(&p, ZEPHYRUM, magnitudo(p));
    p.modus = STATUAE_MODUS_CRUDUS;
    si (   a->modus
        && !statuae_modum_legere(chorda_ex_literis(a->modus, piscina),
               &p.modus))
    {
        fprintf(stderr, "statuae: modus '%s' ignotus\n", a->modus);
        redde II;
    }
    s = statuam_legere(a->positivum, &octeti, piscina);
    si (!s || !cyclum_legere(a, s, &p.cyclus))
    {
        redde I;
    }
    p.tempus             = (f32)a->tempus;
    p.visus.gyrus        = (f32)a->gyrus;
    p.visus.inclinatio   = (f32)a->inclinatio;
    p.visus.illuminatum  = !a->planum;
    p.latitudo           = (i32)a->latitudo;
    p.altitudo           = (i32)a->altitudo;
    si (!statuae_quadrum_pingere(s, &p, &imago, piscina))
    {
        fprintf(stderr, "statuae: quadrum pingi non potuit\n");
        redde I;
    }
    png = imago_png_scribere(&imago, a->exitus, piscina);
    si (!png.successus)
    {
        fprintf(stderr, "statuae: PNG scribi non potest: %s\n",
            a->exitus);
        redde I;
    }
    imprimere("%s: %u x %u, %s\n", a->exitus,
        (insignatus)imago.latitudo, (insignatus)imago.altitudo,
        statuae_modi_titulus(p.modus));
    redde ZEPHYRUM;
}

externus constans ProvenientiaRelatio provenientia_statuae;

s32
principale (
      integer   argc,
    character** argv)
{
      Piscina* piscina;
      Volumen* volumen;
    Argumenta  a;
          b32  addere;
          b32  removere;
          b32  enumerare;
          b32  inspicere;
          s32  exitus;


    si (provenientia_respondere((s32)argc, argv, &provenientia_statuae))
    {
        redde ZEPHYRUM;
    }
    si (!argumenta_legere(argc, argv, &a))
    {
        redde usum_imprimere();
    }
    piscina = piscina_generare_dynamicum("statuae", M * M);
    si (strcmp(a.imperium, "pingere") == ZEPHYRUM)
    {
        si (   !a.positivum || !a.exitus || a.volumen || a.titulus
            || a.locus || a.loci || a.res)
        {
            piscina_destruere(piscina);
            redde usum_imprimere();
        }
        exitus = exsequi_pingere(&a, piscina);
        piscina_destruere(piscina);
        redde exitus;
    }
    addere     = strcmp(a.imperium, "addere") == ZEPHYRUM;
    removere   = strcmp(a.imperium, "removere") == ZEPHYRUM;
    enumerare  = strcmp(a.imperium, "enumerare") == ZEPHYRUM;
    inspicere  = strcmp(a.imperium, "inspicere") == ZEPHYRUM;
    si (   (!addere && !removere && !enumerare && !inspicere)
        || !a.volumen
        || (inspicere && (a.locus || a.loci || a.res))
        || (enumerare ? a.positivum != NIHIL : a.positivum == NIHIL)
        || (a.titulus && !addere) || a.exitus || a.cyclus || a.modus
        || (a.locus && !addere) || (a.loci && !enumerare)
        || (a.res && (removere || a.locus || a.loci)))
    {
        piscina_destruere(piscina);
        redde usum_imprimere();
    }
    /* enumerare et removere volumen non creant */
    volumen = addere ? volumen_aperire_aut_creare(piscina, a.volumen)
                     : volumen_aperire(piscina, a.volumen);
    si (!volumen)
    {
        fprintf(stderr, "statuae: volumen %s aperiri non potest\n",
            a.volumen);
        piscina_destruere(piscina);
        redde I;
    }
    si (addere)
    {
        exitus = exsequi_addere(&a, volumen, piscina);
    }
    alioquin si (removere)
    {
        exitus = statuae_removere(volumen,
            chorda_ex_literis(a.positivum, piscina)) ? ZEPHYRUM : I;
        si (exitus)
        {
            fprintf(stderr, "statuae: volumen remotionem recusavit\n");
        }
    }
    alioquin si (inspicere)
    {
        exitus = exsequi_inspicere(&a, volumen, piscina);
    }
    alioquin
    {
        exitus = exsequi_enumerare(volumen,
            genus_petitum(a.loci, a.res), piscina);
    }
    volumen_claudere(volumen);
    piscina_destruere(piscina);
    redde exitus;
}
