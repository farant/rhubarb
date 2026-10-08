/* fabrica.c - bin/fabrica: iudex aedificationis domus (plan 1a T3)
 *
 * Usus (ex radice repositorii):
 *   bin/fabrica iudicare [-plenus] [-omnia] [artificium...]
 *   bin/fabrica iudicare -plenus -tacta VIA...
 *       artificia GENERATA quae viae (commissionis) tangunt; nullum
 *       tactum = exitus 0 (silva.commissio, T8).
 *       quae artificia ex ingressibus hodiernis NON facta sint, cur,
 *       et ordinem sanationis. Sine -plenus = celer: nulla
 *       regeneratio (artificia regenerabilia NON IUDICATA dicuntur,
 *       numquam "recentia"). Nihil aedificat.
 *   bin/fabrica digestum TITULUS
 *       sigillum copiae ingressuum actionis (LXIV hex).
 *
 * Exitus: 0 nihil stalum aut ignotum · 1 stalum aut ignotum
 * aliquod · 2 nihil iudicatum (declarationes absentes/fractae,
 * usus). NON IUDICATA (celer) exitum non frangunt - modus electus
 * est, non vitium.
 *
 * Hic mandata solum (fabrica-6 H4); sutura vera in
 * tools/fabrica_sutura.c, memoria (build/fabrica.db) in
 * tools/fabrica_memoria.c, ansae graduum in tools/fabrica_ansae.c,
 * caput commune tools/fabrica_sutura.h.
 *
 * Sutura vera: plagulae per filum, directoria per iter_directoria,
 * generatores per processus_exsequi (FABRICA_SCRIPTURA = directorium
 * scripturae VACUATUM ante cursum; mora DC s), relatio
 * '-provenientia' (mora V s). SOLA binaria cum provenientia
 * declarata rogantur: binarium quod vexillum ignorat aliquid facere
 * posset (bin/manus claves mittit).
 *
 * Declarationes: fabrica.stml (radix) -> subsystema/aedificatio.stml.
 * Spec: project-specs/fabrica-spec-v2.md; lib/fabrica.c. */

/* plagula provenientiae (fabrica T7; tools/fabrica_struere.sh bis
 * struit - digestum a binario ipso computatum) */
/* <aedilis obiectum="build/fabrica/provenientia/fabrica.c"/> */

#include "postulata_posix.h"

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "filum.h"
#include "iter_directoria.h"
#include "processus.h"
#include "internamentum.h"
#include "sigillum.h"
#include "git.h"
#include "tabula_dispersa.h"
#include "chorda_aedificator.h"
#include "fabrica.h"
#include "provenientia.h"
#include "scrinium.h"
#include "numerus_romanus.h"
#include "thesaurus.h"
#include "via.h"
#include "aedilis.h"
#include "aedilis_silva.h"
#include "compilator.h"
#include "fabrica_sutura.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>


/* MACHINA (fabrica-6 H1): '-machina' in iudicare et sanare - lineae
 * TSV, genus in campo primo (IUDICIUM, COMPOSITUM, VERDICTUM, SANANDA,
 * ORPHANUM, BINARIA, PRAECONDICIO, SANATIO, AGITUR, UNDA, NOTA, SUMMA)
 * pro consumptoribus (pythonica/silva.py, instrumenta); homines formam
 * ordinariam legunt. Errata (exitus 2) in stderr manent. */
b32 suturae_machina = FALSUM;

/* campus TSV: tabula -> spatium, linea nova -> ' | ', CR omissum */
interior vacuum
_campus_n (
     constans character* datum,
                    i32  mensura)
{
    i32 i;

    putchar('\t');
    per (i = ZEPHYRUM; i < mensura; i++)
    {
        si (datum[i] == '\t')
        {
            putchar(' ');
        }
        alioquin si (datum[i] == '\n')
        {
            fputs(" | ", stdout);
        }
        alioquin si (datum[i] != '\r')
        {
            putchar(datum[i]);
        }
    }
}

vacuum
suturae_campus (
    chorda c)
{
    _campus_n((constans character*)c.datum, c.mensura);
}

interior vacuum
_campus_literis (
    constans character* literae)
{
    _campus_n(literae, (i32)strlen(literae));
}

interior vacuum
_campus_numerus (
    longus numerus)
{
    character textus[XXXII];

    sprintf(textus, "%ld", numerus);
    _campus_literis(textus);
}


/* ==================================================
 * Declarationes
 * ================================================== */

/* Omnes actiones ex fabrica.stml et aedificatio.stml subsystematum.
 * NIHIL (causa impressa) si quid deest aut fractum est - declaratio
 * fracta numquam tacite omittitur. */
interior Xar*
_declarationes_colligere (
    Piscina*  piscina,
        Xar** composita_out)
{
    InternamentumChorda* intern;
                 chorda  radix;
                 chorda  causa;
                    Xar* subsystemata;
                    Xar* omnes;
                    Xar* composita;
                    i32  i;
          FabricaSutura  enumerans;

    /* familiae (plan 2 T4) directoria per suturam enumerant */
    suturae_legentem_parare(&enumerans);
    /* gradus (fabrica-6 T6c-2): membra clausuram suam in clave
     * ferunt */
    enumerans.clausura_c  = suturae_clausura_c;
    intern                = internamentum_creare(piscina);
    causa                 = chorda_ex_literis("", piscina);
    radix                 = filum_legere_totum("fabrica.stml", piscina);
    subsystemata = fabrica_subsystemata_legere(radix, piscina, intern,
        &causa);
    si (subsystemata == NIHIL)
    {
        fprintf(stderr, "fabrica: %.*s\n", (s32)causa.mensura,
            (constans character*)causa.datum);
        redde NIHIL;
    }
    omnes      = xar_creare(piscina, (i32)magnitudo(FabricaActio));
    composita  = xar_creare(piscina, (i32)magnitudo(FabricaCompositum));
    per (i = ZEPHYRUM; i < xar_numerus(subsystemata); i++)
    {
        chorda  via;
        chorda  contentum;
           Xar* actiones;
           Xar* sua;
           i32  j;

        via = chorda_concatenare(*(chorda*)xar_obtinere(subsystemata,
            i),
            chorda_ex_literis("/aedificatio.stml", piscina), piscina);
        si (!filum_existit(chorda_ut_cstr(via, piscina)))
        {
            fprintf(stderr, "fabrica: subsystema declaratum sine "
                "declarationibus: %.*s\n", (s32)via.mensura,
                (constans character*)via.datum);
            redde NIHIL;
        }
        contentum = filum_legere_totum(chorda_ut_cstr(via, piscina),
            piscina);
        actiones = fabrica_declarationes_legere_cum_sutura(contentum,
            chorda_ut_cstr(via, piscina), &enumerans, piscina, intern,
            &causa);
        sua = (actiones != NIHIL)
            ? fabrica_composita_legere(contentum,
                  chorda_ut_cstr(via, piscina), piscina, intern, &causa)
            : NIHIL;
        si (actiones == NIHIL || sua == NIHIL)
        {
            fprintf(stderr, "fabrica: %.*s\n", (s32)causa.mensura,
                (constans character*)causa.datum);
            redde NIHIL;
        }
        /* titulus compositi unicus TRANS subsystemata */
        per (j = ZEPHYRUM; j < xar_numerus(sua); j++)
        {
            FabricaCompositum* compositum;
                          i32  k;

            compositum = (FabricaCompositum*)xar_obtinere(sua, j);
            per (k = ZEPHYRUM; k < xar_numerus(composita); k++)
            {
                FabricaCompositum* prior;

                prior = (FabricaCompositum*)xar_obtinere(composita, k);
                si (chorda_aequalis(prior->titulus,
                        compositum->titulus))
                {
                    fprintf(stderr, "fabrica: %.*s: titulus compositi "
                        "duplex '%.*s' (prior %.*s)\n",
                        (s32)compositum->sedes.mensura,
                        (constans character*)compositum->sedes.datum,
                        (s32)compositum->titulus.mensura,
                        (constans character*)compositum->titulus.datum,
                        (s32)prior->sedes.mensura,
                        (constans character*)prior->sedes.datum);
                    redde NIHIL;
                }
            }
            *(FabricaCompositum*)xar_addere(composita) = *compositum;
        }
        per (j = ZEPHYRUM; j < xar_numerus(actiones); j++)
        {
            FabricaActio* actio;
                     i32  k;

            actio = (FabricaActio*)xar_obtinere(actiones, j);
            /* titulus unicus TRANS subsystemata: directoria
             * scripturae per titulum */
            per (k = ZEPHYRUM; k < xar_numerus(omnes); k++)
            {
                FabricaActio* prior;

                prior = (FabricaActio*)xar_obtinere(omnes, k);
                si (chorda_aequalis(prior->titulus, actio->titulus))
                {
                    fprintf(stderr, "fabrica: %.*s: titulus duplex "
                        "'%.*s' (prior %.*s)\n",
                        (s32)actio->sedes.mensura,
                        (constans character*)actio->sedes.datum,
                        (s32)actio->titulus.mensura,
                        (constans character*)actio->titulus.datum,
                        (s32)prior->sedes.mensura,
                        (constans character*)prior->sedes.datum);
                    redde NIHIL;
                }
            }
            *(FabricaActio*)xar_addere(omnes) = *actio;
        }
    }
    /* GRADUS (fabrica-6 T5/T6c): actiones gradus in membra explicantur
     * (actiones syntheticae '<actio>/<membrum>') */
    omnes = fabrica_gradus_explicare(&enumerans, omnes, piscina,
        &causa);
    si (omnes == NIHIL)
    {
        fprintf(stderr, "fabrica: %.*s\n", (s32)causa.mensura,
            (constans character*)causa.datum);
        redde NIHIL;
    }
    /* COMPOSITA GRADUUM (fabrica-6 T7): actio gradus = compositum
     * membrorum eius (iudicare/sanare per titulum actionis) */
    {
        Xar* gradus;

        gradus = fabrica_gradus_composita(omnes, piscina);
        per (i = ZEPHYRUM; gradus != NIHIL && i < xar_numerus(gradus);
             i++)
        {
            FabricaCompositum* novum;
                          i32  k;

            novum = (FabricaCompositum*)xar_obtinere(gradus, i);
            per (k = ZEPHYRUM; k < xar_numerus(composita); k++)
            {
                si (chorda_aequalis(((FabricaCompositum*)xar_obtinere(
                        composita, k))->titulus, novum->titulus))
                {
                    fprintf(stderr, "fabrica: %.*s: compositum '%.*s' "
                        "titulum actionis gradus iam fert\n",
                        (s32)novum->sedes.mensura,
                        (constans character*)novum->sedes.datum,
                        (s32)novum->titulus.mensura,
                        (constans character*)novum->titulus.datum);
                    redde NIHIL;
                }
            }
            *(FabricaCompositum*)xar_addere(composita) = *novum;
        }
    }
    /* post omnes declarationes: praecondiciones et composita trans
     * subsystemata probantur - fracta numquam tacite */
    si (!fabrica_praecondiciones_probare(omnes, piscina, &causa))
    {
        fprintf(stderr, "fabrica: %.*s\n", (s32)causa.mensura,
            (constans character*)causa.datum);
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < xar_numerus(composita); i++)
    {
        si (fabrica_compositum_explicare(composita, omnes,
                ((FabricaCompositum*)xar_obtinere(composita,
                i))->titulus, piscina, &causa) == NIHIL)
        {
            fprintf(stderr, "fabrica: %.*s\n", (s32)causa.mensura,
                (constans character*)causa.datum);
            redde NIHIL;
        }
    }
    si (composita_out != NIHIL)
    {
        *composita_out = composita;
    }
    redde omnes;
}


/* ==================================================
 * Orphana: manifesta aedilis quorum scopus abest
 * ================================================== */

interior i32
_orphana_imprimere (
    Piscina* piscina)
{
     DirectoriumIterator* iterator;
    DirectoriumIntroitus* introitus;
                     i32  numerus;

    numerus   = ZEPHYRUM;
    iterator  = directorium_iterator_aperire("build/aedilis", piscina);
    si (iterator == NIHIL)
    {
        redde ZEPHYRUM;
    }
    dum ((introitus = directorium_iterator_proximum(iterator)) != NIHIL)
    {
         chorda via;
         chorda contentum;
         chorda signum;
         chorda scopus;
            s32 inventum;
            i32 initium;
            i32 finis;

        si (introitus->genus != INTROITUS_DIRECTORIUM)
        {
            perge;
        }
        via = chorda_concatenare(
            chorda_ex_literis("build/aedilis/", piscina),
            chorda_concatenare(introitus->titulus,
                chorda_ex_literis("/manifestum.stml", piscina),
                piscina),
            piscina);
        si (!filum_existit(chorda_ut_cstr(via, piscina)))
        {
            perge;
        }
        contentum = filum_legere_totum(chorda_ut_cstr(via, piscina),
            piscina);
        signum    = chorda_ex_literis("scopus=\"", piscina);
        inventum  = chorda_invenire_index(contentum, signum);
        si (inventum < 0)
        {
            perge;
        }
        initium  = (i32)inventum + signum.mensura;
        finis    = initium;
        dum (finis < contentum.mensura && contentum.datum[finis] != '"')
        {
            finis++;
        }
        scopus = chorda_sectio(contentum, initium, finis);
        si (!filum_existit(chorda_ut_cstr(scopus, piscina)))
        {
            si (suturae_machina)
            {
                fputs("ORPHANUM", stdout);
                suturae_campus(chorda_concatenare(chorda_concatenare(
                    chorda_ex_literis("build/aedilis/", piscina),
                    introitus->titulus, piscina),
                    chorda_ex_literis("/", piscina), piscina));
                suturae_campus(chorda_concatenare(chorda_concatenare(
                    chorda_ex_literis("scopus ", piscina), scopus,
                    piscina), chorda_ex_literis(" absens", piscina),
                    piscina));
                putchar('\n');
            }
            alioquin
            {
                printf("ORPHANUM: build/aedilis/%.*s/ "
                    "(scopus %.*s absens)\n",
                    (s32)introitus->titulus.mensura,
                    (constans character*)introitus->titulus.datum,
                    (s32)scopus.mensura,
                    (constans character*)scopus.datum);
            }
            numerus++;
        }
    }
    directorium_iterator_claudere(iterator);
    redde numerus;
}


/* ==================================================
 * Q36: binaria in bin/ sine declaratione (numerus solus)
 * ================================================== */

/* Plagulae in bin/ quas nulla actio ut exitum nominat. Numerus solus
 * (spec v2 Q36): binaria probationum et compile_tools singillatim non
 * iudicantur - linea una, ut orphana, exitum non mutat (aliter celer
 * semper I redderet). */
interior i32
_binaria_sine_declaratione (
    constans Xar* actiones,
         Piscina* piscina)
{
     DirectoriumIterator* iterator;
    DirectoriumIntroitus* introitus;
                     i32  numerus;

    numerus   = ZEPHYRUM;
    iterator  = directorium_iterator_aperire("bin", piscina);
    si (iterator == NIHIL)
    {
        redde ZEPHYRUM;
    }
    dum ((introitus = directorium_iterator_proximum(iterator)) != NIHIL)
    {
        chorda via;
           b32 declaratum;
           i32 i;

        si (   introitus->genus == INTROITUS_DIRECTORIUM
            || chorda_aequalis_literis(introitus->titulus, ".")
            || chorda_aequalis_literis(introitus->titulus, ".."))
        {
            perge;
        }
        via = chorda_concatenare(chorda_ex_literis("bin/", piscina),
            introitus->titulus, piscina);
        declaratum = FALSUM;
        per (i = ZEPHYRUM; !declaratum && i < xar_numerus(actiones);
             i++)
        {
            FabricaActio* actio;
                     i32  j;

            actio = (FabricaActio*)xar_obtinere(actiones, i);
            per (j = ZEPHYRUM; j < xar_numerus(actio->exitus); j++)
            {
                si (chorda_aequalis(((FabricaExitus*)xar_obtinere(
                        actio->exitus, j))->via, via))
                {
                    declaratum = VERUM;
                    frange;
                }
            }
        }
        si (!declaratum)
        {
            numerus++;
        }
    }
    directorium_iterator_claudere(iterator);
    redde numerus;
}


/* ==================================================
 * iudicare
 * ================================================== */

nomen structura {
    FabricaIudicium  iudicium;
       FabricaActio* actio;
} Sententia;

/* electa NIHIL = nullum filtrum; aliter via in ea */
interior b32
_electum (
     constans Xar* electa,
           chorda  via)
{
    i32 i;

    si (electa == NIHIL)
    {
        redde VERUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(electa); i++)
    {
        si (chorda_aequalis(*(chorda*)xar_obtinere(electa, i), via))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior vacuum
_sententiam_imprimere (
    constans Sententia* sententia,
                   b32  omnia)
{
    constans character* signum;

    commutatio (sententia->iudicium.status)
    {
        casus FABRICA_RECENS:
            /* machina: RECENS semper (consumptor omnia videt) */
            si (!omnia && !suturae_machina)
            {
                redde;
            }
            signum = "RECENS";
            frange;
        casus FABRICA_STALUM:
            signum = "STALUM";
            frange;
        casus FABRICA_IGNOTUM:
            signum = "IGNOTUM";
            frange;
        casus FABRICA_NON_IUDICATUM:
            signum = "NON IUDICATUM";
            frange;
        ordinarius:
            redde;
    }
    si (suturae_machina)
    {
        fputs("IUDICIUM", stdout);
        _campus_literis(signum);
        suturae_campus(sententia->iudicium.artificium);
        suturae_campus(sententia->iudicium.causa);
        putchar('\n');
        redde;
    }
    printf("%s %.*s - %.*s\n", signum,
        (s32)sententia->iudicium.artificium.mensura,
        (constans character*)sententia->iudicium.artificium.datum,
        (s32)sententia->iudicium.causa.mensura,
        (constans character*)sententia->iudicium.causa.datum);
}

/* status ut verbum (lineae machinae) */
interior constans character*
_status_verbum (
    FabricaStatus status)
{
    commutatio (status)
    {
        casus FABRICA_RECENS:         redde "RECENS";
        casus FABRICA_STALUM:         redde "STALUM";
        casus FABRICA_IGNOTUM:        redde "IGNOTUM";
        casus FABRICA_NON_IUDICATUM:  redde "NON IUDICATUM";
    }
    redde "IGNOTUM";
}

/* VERDICTUM machinae: titulus, N recentia, M partes, non recentia
 * OMNIA (id membri ex 'build/fabrica/area/<id>/verdictum.txt', aliter
 * artificium), virgulis separata */
interior vacuum
_verdictum_machinae (
    constans Xar* iudicia,
          chorda  titulus)
{
     constans character* praefixum  = "build/fabrica/area/";
     constans character* suffixum   = "/verdictum.txt";
                    i32  recentia   = ZEPHYRUM;
                    i32  nominata   = ZEPHYRUM;
                    i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(iudicia); i++)
    {
        si (((constans FabricaIudicium*)xar_obtinere(iudicia,
                i))->status == FABRICA_RECENS)
        {
            recentia++;
        }
    }
    fputs("VERDICTUM", stdout);
    suturae_campus(titulus);
    _campus_numerus((longus)recentia);
    _campus_numerus((longus)xar_numerus(iudicia));
    putchar('\t');
    per (i = ZEPHYRUM; i < xar_numerus(iudicia); i++)
    {
        constans FabricaIudicium* iudicium = (constans FabricaIudicium*)
            xar_obtinere(iudicia, i);
                           chorda a = iudicium->artificium;
                   memoriae_index p = strlen(praefixum);
                   memoriae_index q = strlen(suffixum);

        si (iudicium->status == FABRICA_RECENS)
        {
            perge;
        }
        si (nominata++ > ZEPHYRUM)
        {
            putchar(',');
        }
        si (   (memoriae_index)a.mensura > p + q
            && memcmp(a.datum, praefixum, p) == ZEPHYRUM
            && memcmp(a.datum + a.mensura - (i32)q, suffixum, q)
               == ZEPHYRUM)
        {
            printf("%.*s", (s32)((memoriae_index)a.mensura - p - q),
                (constans character*)a.datum + p);
        }
        alioquin
        {
            printf("%.*s", (s32)a.mensura,
                (constans character*)a.datum);
        }
    }
    putchar('\n');
}

/* ordo per LOCOS (plan 1b T5): dependentiae ex 'enumerare' (clausurae
 * manifestorum quoque) et vestigiis (memoria NIHIL: sine), deinde ordo. Sigilla communia cum iudicio quod
 * sequitur (memoria per cursum: nihil bis sigillatur). */
interior Xar*
_ordinare_per_locos (
               Xar* actiones,
    TabulaDispersa* sigilla,
           Piscina* piscina,
           Memoria* memoria,
            chorda* causa_out)
{
    FabricaSutura levis;

    suturae_legentem_parare(&levis);
    levis.sigilla    = sigilla;
    /* vestigia lectionum (plan 2 T2): viae ultimae lectae = loci ordinis
     * actionum lectiones="verum" (manifesta iis dempta) */
    si (memoria != NIHIL)
    {
        levis.datum              = memoria;
        levis.lectiones_ultimae  = suturae_lectiones_ultimae;
    }
    si (!fabrica_dependentias_computare(&levis, actiones, piscina,
            causa_out))
    {
        redde NIHIL;
    }
    redde fabrica_ordinare(actiones, piscina, causa_out);
}

/* 'iudex X non recens - sana X prius' */
interior chorda
_iungere_iudicem (
     Piscina* piscina,
      chorda  titulus)
{
    character textus[CCLVI];

    sprintf(textus, "iudex %.*s non recens - sana %.*s prius",
        (integer)titulus.mensura, (constans character*)titulus.datum,
        (integer)titulus.mensura, (constans character*)titulus.datum);
    redde chorda_transcribere(chorda_ex_literis(textus, piscina),
        piscina);
}

interior s32
_iudicare (
          s32   argc,
    character** argv,
      Piscina*  piscina)
{
             chorda* iudex_stalus;
       FabricaActio* iudex_actio;
      FabricaSutura  sutura;
            Memoria  memoria;
                b32  memoria_aperta;
          FilumSera* sera;
     TabulaDispersa* sigilla;
                Xar* actiones;
                Xar* ordo;
                Xar* sententiae;
             chorda  causa;
                b32  plenus;
                b32  omnia;
                b32  tacta;
                i32  auditus;
                Xar* viae_tactae;
                Xar* composita;
                Xar* electa;
                Xar* nominata;
                s32  a;
                i32  i;
                i32  j;
                i32  recentia;
                i32  stala;
                i32  ignota;
                i32  non_iudicata;
                i32  orphana;

    plenus   = FALSUM;
    omnia    = FALSUM;
    tacta    = FALSUM;
    auditus  = XX;   /* A2: unus ex XX ictuum sub -plenus regeneratur */
    per (a = II; a < argc; a++)
    {
        si (strcmp(argv[a], "-plenus") == 0)
        {
            plenus = VERUM;
        }
        alioquin si (strcmp(argv[a], "-audit") == 0)
        {
            /* AUDITUS omnium (plan 2 T2): omnis ictus vestigii aut
             * memoriae regeneratur et confertur; -plenus implicat */
            plenus   = VERUM;
            auditus  = I;
        }
        alioquin si (strcmp(argv[a], "-tacta") == 0)
        {
            /* argumenta positionalia = viae commissionis: artificia
             * GENERATA (regeneratio) quae tangunt iudicantur, non
             * installata (binaria non committuntur; uncus sessionis ea
             * nominat). Nullum tactum = exitus 0 (T8 gradus III) */
            tacta = VERUM;
        }
        alioquin si (strcmp(argv[a], "-omnia") == 0)
        {
            /* RECENS quoque cum causa ('memoria', 'regeneratio
             * congruit', 'relatio congruit') - plantae T6 */
            omnia = VERUM;
        }
        alioquin si (strcmp(argv[a], "-machina") == 0)
        {
            suturae_machina = VERUM;   /* fabrica-6 H1: lineae TSV */
        }
        alioquin si (argv[a][0] == '-')
        {
            fprintf(stderr, "fabrica: vexillum ignotum %s\n", argv[a]);
            redde II;
        }
    }

    viae_tactae = xar_creare(piscina, (i32)magnitudo(chorda));
    per (a = II; tacta && a < argc; a++)
    {
        si (argv[a][0] != '-')
        {
            *(chorda*)xar_addere(viae_tactae) =
                chorda_ex_literis(argv[a],
                piscina);
        }
    }

    actiones = _declarationes_colligere(piscina, &composita);
    si (actiones == NIHIL)
    {
        redde II;
    }
    causa  = chorda_ex_literis("", piscina);

    /* argumenta positionalia (sine -tacta): via artificii aut titulus
     * COMPOSITI (partes eius eliguntur, iudicium coniunctum post eas
     * imprimitur) */
    electa    = NIHIL;
    nominata  = xar_creare(piscina, (i32)magnitudo(FabricaCompositum*));
    per (a = II; !tacta && a < argc; a++)
    {
        chorda argumentum;
           b32 compositum_est;

        si (argv[a][0] == '-')
        {
            perge;
        }
        si (electa == NIHIL)
        {
            electa = xar_creare(piscina, (i32)magnitudo(chorda));
        }
        argumentum      = chorda_ex_literis(argv[a], piscina);
        compositum_est  = FALSUM;
        per (i = ZEPHYRUM; i < xar_numerus(composita); i++)
        {
            FabricaCompositum* compositum;
                          Xar* partes;

            compositum = (FabricaCompositum*)xar_obtinere(composita, i);
            si (!chorda_aequalis(compositum->titulus, argumentum))
            {
                perge;
            }
            compositum_est                              = VERUM;
            *(FabricaCompositum**)xar_addere(nominata)  = compositum;
            partes = fabrica_compositum_explicare(composita, actiones,
                compositum->titulus, piscina, &causa);
            per (j = ZEPHYRUM; partes != NIHIL
                && j < xar_numerus(partes);
                 j++)
            {
                *(chorda*)xar_addere(electa) =
                    *(chorda*)xar_obtinere(partes, j);
            }
        }
        si (!compositum_est)
        {
            *(chorda*)xar_addere(electa) = argumentum;
        }
    }
    sigilla         = tabula_dispersa_creare_chorda(piscina, 1024);
    memoria_aperta  = suturae_memoriam_aperire(&memoria, piscina);
    ordo     = _ordinare_per_locos(actiones, sigilla, piscina,
        memoria_aperta ? &memoria : NIHIL, &causa);
    si (ordo == NIHIL)
    {
        fprintf(stderr, "fabrica: %.*s\n", (s32)causa.mensura,
            (constans character*)causa.datum);
        redde II;
    }

    /* SERA sub -plenus: generatores clausuras et scripturas VACUANT -
     * iudex plenus alter simul eas sub pedibus huius deleret (T6:
     * factum, claves ternae per actionem). flock: nucleus eam cum
     * processu liberat, nulla sera stala. Celer nihil scribit:
     * libera. */
    sera = NIHIL;
    si (plenus)
    {
        (vacuum)filum_directorium_creare_cum_parentibus(
            "build/fabrica");
        sera = filum_seram_capere("build/fabrica/sera", ZEPHYRUM,
            piscina);
        si (sera == NIHIL)
        {
            fprintf(stderr, "fabrica: iudex plenus alius currit "
                "(build/fabrica/sera tenetur) - exspecta eum aut "
                "siste\n");
            redde II;
        }
    }

    suturae_legentem_parare(&sutura);
    suturae_currentem_addere(&sutura);
    si (memoria_aperta)
    {
        suturae_memoriam_nectere(&sutura, &memoria);
    }
    sutura.radix    = suturae_radix_absoluta(piscina);
    suturae_iudicii_parare(&sutura, actiones, piscina);
    sutura.auditus  = auditus;
    /* memoria sigillorum per cursum: communis semel sigillatur (iam
     * impleta per ordinem per locos) */
    sutura.sigilla    = sigilla;
    /* generator semel per actionem (exitus multi, cursus unus) */
    sutura.regenerationes = tabula_dispersa_creare_chorda(piscina, 64);
    /* sigillum ingressuum semel per actionem */
    sutura.digesta = tabula_dispersa_creare_chorda(piscina, 128);

    /* PRAEVISIO (T6b): sub -plenus regenerationes actionum tutarum
     * (lectiones) quas iudicium hoc posceret SIMUL currunt; iudicium
     * infra (ordine suo, immutatum) eas memoratas invenit */
    suturae_fila_maxima = suturae_fila_computare(piscina);
    si (plenus && suturae_fila_maxima > I)
    {
        Xar* praevidendae;

        sutura.currere_simul = suturae_currere_simul;
        praevidendae = xar_creare(piscina,
            (i32)magnitudo(FabricaActio*));
        per (i = ZEPHYRUM; praevidendae != NIHIL
             && i < xar_numerus(ordo); i++)
        {
            FabricaActio* actio;
                     b32  iudicanda;

            actio = *(FabricaActio**)xar_obtinere(ordo, i);
            si (   !actio->lectiones
                || actio->genus == FABRICA_ACTIO_IUDICIUM)
            {
                perge;   /* iudicium: verdictum numquam praevidetur */
            }
            iudicanda = FALSUM;
            si (tacta)
            {
                iudicanda = fabrica_actio_tacta(&sutura, actio,
                    viae_tactae, piscina);
            }
            alioquin
            {
                per (j = ZEPHYRUM; j < xar_numerus(actio->exitus); j++)
                {
                    FabricaExitus* exitus;

                    exitus = (FabricaExitus*)xar_obtinere(actio->exitus,
                        j);
                    si (   exitus->strategia->iudicatur
                        && _electum(electa, exitus->via))
                    {
                        iudicanda = VERUM;
                    }
                }
            }
            si (iudicanda)
            {
                *(FabricaActio**)xar_addere(praevidendae) = actio;
            }
        }
        si (praevidendae != NIHIL)
        {
            fabrica_regenerationes_praevidere(&sutura, praevidendae,
                plenus, piscina);
        }
    }

    /* STADIUM IUDICUM (fabrica-6 T3): actiones iudex="verum" primum;
     * quaevis non recens -> ceteri NON IUDICATI nominantes eam (iudex
     * stalus sententias suspectas faceret) */
    iudex_stalus  = NIHIL;
    iudex_actio   = NIHIL;
    sententiae    = xar_creare(piscina, (i32)magnitudo(Sententia));
    per (i = ZEPHYRUM; i < xar_numerus(actiones)
        && iudex_stalus == NIHIL;
         i++)
    {
        FabricaActio* actio = (FabricaActio*)xar_obtinere(actiones, i);

        si (!actio->iudex)
        {
            perge;
        }
        per (j = ZEPHYRUM; j < xar_numerus(actio->exitus); j++)
        {
            FabricaExitus* exitus = (FabricaExitus*)xar_obtinere(
                actio->exitus, j);

            FabricaIudicium iudicium;

            si (!exitus->strategia->iudicatur)
            {
                perge;
            }
            iudicium = fabrica_iudicare(&sutura, actio, exitus, plenus,
                piscina);
            si (iudicium.status != FABRICA_RECENS)
            {
                /* sententia iudicis ipsa PRIMA (numeratur, sanatio eum
                 * nominat) - etiam si non electa */
                Sententia* sententia =
                    (Sententia*)xar_addere(sententiae);

                sententia->actio     = actio;
                sententia->iudicium  = iudicium;
                iudex_stalus         = &actio->titulus;
                iudex_actio          = actio;
                frange;
            }
        }
    }

    per (i = ZEPHYRUM; i < xar_numerus(ordo); i++)
    {
        FabricaActio* actio;

        actio = *(FabricaActio**)xar_obtinere(ordo, i);
        si (actio == iudex_actio)
        {
            perge;   /* sententia eius iam prima */
        }
        si (   tacta
            && !fabrica_actio_tacta(&sutura, actio, viae_tactae,
            piscina))
        {
            perge;
        }
        per (j = ZEPHYRUM; j < xar_numerus(actio->exitus); j++)
        {
            FabricaExitus* exitus;
                Sententia* sententia;

            exitus = (FabricaExitus*)xar_obtinere(actio->exitus, j);
            /* praecondicio (strategia ignota): numquam iudicatur */
            si (!exitus->strategia->iudicatur)
            {
                si (suturae_machina && _electum(electa, exitus->via))
                {
                    fputs("PRAECONDICIO", stdout);
                    suturae_campus(exitus->via);
                    suturae_campus(actio->titulus);
                    putchar('\n');
                }
                alioquin si (omnia && _electum(electa, exitus->via))
                {
                    printf("PRAECONDICIO %.*s - %.*s (ignota, numquam "
                        "iudicatur)\n", (s32)exitus->via.mensura,
                        (constans character*)exitus->via.datum,
                        (s32)actio->titulus.mensura,
                        (constans character*)actio->titulus.datum);
                }
                perge;
            }
            si (tacta)
            {
                /* commissa generata solum: strategia quae octetis
                 * comparat */
                si (!exitus->strategia->octetis_comparat)
                {
                    perge;
                }
            }
            alioquin si (!_electum(electa, exitus->via))
            {
                perge;
            }
            /* iudicium (porta, spec 3 par. XII) solum NOMINATUM
             * iudicatur: verritio sine argumentis id omittit */
            si (   electa       == NIHIL
                && actio->genus == FABRICA_ACTIO_IUDICIUM)
            {
                perge;
            }
            sententia         = (Sententia*)xar_addere(sententiae);
            sententia->actio  = actio;
            si (iudex_stalus != NIHIL && !actio->iudex)
            {
                sententia->iudicium.artificium  = exitus->via;
                sententia->iudicium.status      = FABRICA_NON_IUDICATUM;
                sententia->iudicium.causa       =
                    _iungere_iudicem(piscina,
                    *iudex_stalus);
                perge;
            }
            sententia->iudicium = fabrica_iudicare(&sutura, actio,
                exitus, plenus, piscina);
        }
    }
    si (xar_numerus(sententiae) == 0 && tacta)
    {
        printf(suturae_machina
            ? "NOTA\tnulla artificia generata a viis tacta\n"
            : "fabrica: nulla artificia generata a viis tacta\n");
        filum_seram_liberare(sera);
        redde ZEPHYRUM;
    }
    si (xar_numerus(sententiae) == 0)
    {
        fprintf(stderr, "fabrica: nihil iudicatum (nulla artificia "
            "declarata%s)\n",
            electa == NIHIL ? "" : " congruunt");
        redde II;
    }

    /* iudex ipse PRIMUS: si bin/fabrica stalum est, ceterae
     * sententiae ab eo scriptae suspectae sunt */
    per (i = ZEPHYRUM; i < xar_numerus(sententiae); i++)
    {
        Sententia* s;

        s = (Sententia*)xar_obtinere(sententiae, i);
        si (chorda_aequalis_literis(s->iudicium.artificium,
                "bin/fabrica"))
        {
            _sententiam_imprimere(s, omnia);
        }
    }
    recentia      = ZEPHYRUM;
    stala         = ZEPHYRUM;
    ignota        = ZEPHYRUM;
    non_iudicata  = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(sententiae); i++)
    {
        Sententia* s;

        s = (Sententia*)xar_obtinere(sententiae, i);
        commutatio (s->iudicium.status)
        {
            casus FABRICA_RECENS:
                recentia++;
                frange;
            casus FABRICA_STALUM:
                stala++;
                frange;
            casus FABRICA_IGNOTUM:
                ignota++;
                frange;
            ordinarius:
                non_iudicata++;
                frange;
        }
        si (!chorda_aequalis_literis(s->iudicium.artificium,
                "bin/fabrica"))
        {
            _sententiam_imprimere(s, omnia);
        }
    }

    /* composita nominata: pessimum partium, post partes */
    per (i = ZEPHYRUM; i < xar_numerus(nominata); i++)
    {
        FabricaCompositum* compositum;
          FabricaIudicium  coniunctum;
                      Xar* partes;
                      Xar* iudicia;

        compositum = *(FabricaCompositum**)xar_obtinere(nominata, i);
        partes = fabrica_compositum_explicare(composita, actiones,
            compositum->titulus, piscina, &causa);
        iudicia = xar_creare(piscina, (i32)magnitudo(FabricaIudicium));
        per (j = ZEPHYRUM; j < xar_numerus(sententiae); j++)
        {
            Sententia* s;

            s = (Sententia*)xar_obtinere(sententiae, j);
            si (_electum(partes, s->iudicium.artificium))
            {
                *(FabricaIudicium*)xar_addere(iudicia) = s->iudicium;
            }
        }
        coniunctum = fabrica_iudicia_coniungere(iudicia,
            compositum->titulus, piscina);
        si (suturae_machina)
        {
            fputs("COMPOSITUM", stdout);
            suturae_campus(coniunctum.artificium);
            _campus_literis(_status_verbum(coniunctum.status));
            suturae_campus(coniunctum.causa);
            putchar('\n');
            _verdictum_machinae(iudicia, compositum->titulus);
            perge;
        }
        printf("COMPOSITUM %.*s - %.*s\n",
            (s32)coniunctum.artificium.mensura,
            (constans character*)coniunctum.artificium.datum,
            (s32)coniunctum.causa.mensura,
            (constans character*)coniunctum.causa.datum);
        /* VERDICTUM (fabrica-6 T7): '<titulus>: N/M' ex partibus */
        {
            chorda verdictum;

            verdictum = fabrica_compositum_verdictum(iudicia,
                compositum->titulus, piscina);
            printf("VERDICTUM %.*s\n", (s32)verdictum.mensura,
                (constans character*)verdictum.datum);
        }
    }

    /* SANATIO: actiones artificiorum stalorum/ignotorum, ordine
     * dependentiae, semel quaeque */
    si (stala + ignota > 0)
    {
        FabricaActio* prior;

        si (!suturae_machina)
        {
            printf("SANATIO:\n");
        }
        prior = NIHIL;
        per (i = ZEPHYRUM; i < xar_numerus(sententiae); i++)
        {
            Sententia* s;
                  i32  k;

            s = (Sententia*)xar_obtinere(sententiae, i);
            si (   s->iudicium.status != FABRICA_STALUM
                && s->iudicium.status != FABRICA_IGNOTUM)
            {
                perge;
            }
            si (s->actio == prior)
            {
                perge;
            }
            prior = s->actio;
            si (suturae_machina)
            {
                fputs("SANANDA", stdout);
                suturae_campus(s->actio->titulus);
                suturae_campus(s->actio->sedes);
                putchar('\t');
                per (k = ZEPHYRUM; k < xar_numerus(s->actio->mandatum);
                     k++)
                {
                    chorda verbum = *(chorda*)xar_obtinere(
                        s->actio->mandatum, k);

                    printf(k > ZEPHYRUM ? " %.*s" : "%.*s",
                        (s32)verbum.mensura,
                        (constans character*)verbum.datum);
                }
                putchar('\n');
                perge;
            }
            printf(" ");
            si (xar_numerus(s->actio->mandatum) == 0)
            {
                printf(" (sanatio non declarata)");
            }
            per (k = ZEPHYRUM; k < xar_numerus(s->actio->mandatum); k++)
            {
                chorda verbum;

                verbum = *(chorda*)xar_obtinere(s->actio->mandatum, k);
                printf(" %.*s", (s32)verbum.mensura,
                    (constans character*)verbum.datum);
            }
            printf("   # %.*s (%.*s)\n",
                (s32)s->actio->titulus.mensura,
                (constans character*)s->actio->titulus.datum,
                (s32)s->actio->sedes.mensura,
                (constans character*)s->actio->sedes.datum);
        }
    }

    /* orphana et binaria sine declaratione solum sine filtro
     * artificiorum (vexilla non filtrant) */
    orphana = (!tacta && electa == NIHIL)
        ? _orphana_imprimere(piscina) : ZEPHYRUM;
    si (!tacta && electa == NIHIL)
    {
        i32  sine;
        Xar* areae;
        i32  a;

        /* areae graduum sine membro (fabrica-6 T5): nuntiatae */
        areae = fabrica_areas_orphanas(&sutura, actiones, piscina);
        per (a = ZEPHYRUM; areae != NIHIL
            && a < xar_numerus(areae); a++)
        {
            chorda via = *(chorda*)xar_obtinere(areae, a);

            si (suturae_machina)
            {
                fputs("ORPHANUM", stdout);
                suturae_campus(via);
                _campus_literis("area gradus sine membro");
                putchar('\n');
            }
            alioquin
            {
                printf("ORPHANUM: %.*s (area gradus sine membro)\n",
                    (s32)via.mensura, (constans character*)via.datum);
            }
            orphana++;
        }

        sine = _binaria_sine_declaratione(actiones, piscina);
        si (sine > 0)
        {
            printf(suturae_machina ? "BINARIA\t%u\n"
                : "IGNOTUM: %u binaria in bin/ sine declaratione "
                "(numerus solus, Q36)\n", (insignatus integer)sine);
        }
    }
    si (suturae_machina)
    {
        printf("SUMMA\t%u\t%u\t%u\t%u\t%u\t%s\n",
            (insignatus integer)recentia, (insignatus integer)stala,
            (insignatus integer)ignota,
            (insignatus integer)non_iudicata,
            (insignatus integer)orphana, plenus ? "plenus" : "celer");
    }
    alioquin
    {
        printf("fabrica: %u recentia, %u stala, %u ignota, %u non "
            "iudicata%s, %u orphana\n",
            (insignatus integer)recentia, (insignatus integer)stala,
            (insignatus integer)ignota,
            (insignatus integer)non_iudicata,
            plenus ? "" : " (celer)", (insignatus integer)orphana);
    }
    si (sutura.datum != NIHIL)
    {
        scrinium_claudere(memoria.scrinium);
    }
    filum_seram_liberare(sera);
    redde (stala + ignota > 0) ? I : ZEPHYRUM;
}


/* ==================================================
 * sanare (plan 1b T3)
 * ================================================== */

/* causa multilinearis: prima linea post titulum, ceterae indentatae */
interior vacuum
_causam_imprimere (
    chorda causa)
{
    i32 i;
    i32 initium;
    b32 prima;

    initium  = ZEPHYRUM;
    prima    = VERUM;
    per (i = ZEPHYRUM; i <= causa.mensura; i++)
    {
        si (i == causa.mensura || causa.datum[i] == '\n')
        {
            si (prima)
            {
                printf(" - %.*s\n", (s32)(i - initium),
                    (constans character*)causa.datum + initium);
                prima = FALSUM;
            }
            alioquin si (i > initium)
            {
                printf("      | %.*s\n", (s32)(i - initium),
                    (constans character*)causa.datum + initium);
            }
            initium = i + I;
        }
    }
}

/* auditus sub sanare: -audit (I) aut FABRICA_AUDITUS=N */
hic_manens i32 auditus_sanandi = ZEPHYRUM;

interior s32
_sanare (
          s32   argc,
    character** argv,
      Piscina*  piscina)
{
                  Xar* nominata;
                  Xar* sanationes_iudicum;
        FabricaSutura  sutura;
              Memoria  memoria;
                  b32  memoria_aperta;
            FilumSera* sera;
       TabulaDispersa* sigilla;
                  Xar* actiones;
                  Xar* composita;
                  Xar* ordo;
                  Xar* electa;
                  Xar* sanationes;
               chorda  causa;
                  b32  siccum;
                  b32  commissa;
                  s32  a;
                  i32  i;
                  i32  j;
                  i32  numeri[FABRICA_IUDICIUM + I];   /* omnes eventus:
                                     * VI erat - AUDITUM_DISCORS (index VI)
                                     * ultra finem scribebat (T7) */
                i32 duratio;
                i32 aestimatio;
                i32 sine_tempore;

    siccum = FALSUM;
    per (a = II; a < argc; a++)
    {
        si (strcmp(argv[a], "-siccum") == 0)
        {
            siccum = VERUM;
        }
        alioquin si (strcmp(argv[a], "-audit") == 0)
        {
            /* AUDITUS TRANSITUS (spec 3 par. XIII): actio iudicium RECENS
             * tamen currit; defectus = AUDITUM DISCORS */
            auditus_sanandi = I;
        }
        alioquin si (strcmp(argv[a], "-machina") == 0)
        {
            suturae_machina = VERUM;   /* fabrica-6 H1: lineae TSV */
        }
        alioquin si (argv[a][0] == '-')
        {
            fprintf(stderr, "fabrica: vexillum ignotum %s\n", argv[a]);
            redde II;
        }
    }
    actiones = _declarationes_colligere(piscina, &composita);
    si (actiones == NIHIL)
    {
        redde II;
    }
    causa           = chorda_ex_literis("", piscina);
    sigilla         = tabula_dispersa_creare_chorda(piscina, 1024);
    memoria_aperta  = suturae_memoriam_aperire(&memoria, piscina);
    ordo     = _ordinare_per_locos(actiones, sigilla, piscina,
        memoria_aperta ? &memoria : NIHIL, &causa);
    si (ordo == NIHIL)
    {
        fprintf(stderr, "fabrica: %.*s\n", (s32)causa.mensura,
            (constans character*)causa.datum);
        redde II;
    }

    /* electa: via artificii aut titulus compositi */
    electa    = NIHIL;
    nominata  = xar_creare(piscina, (i32)magnitudo(FabricaCompositum*));
    per (a = II; a < argc; a++)
    {
        b32 compositum_est;

        si (argv[a][0] == '-')
        {
            perge;
        }
        si (electa == NIHIL)
        {
            electa = xar_creare(piscina, (i32)magnitudo(chorda));
        }
        compositum_est = FALSUM;
        per (i = ZEPHYRUM; i < xar_numerus(composita); i++)
        {
            FabricaCompositum* compositum;
                          Xar* partes;

            compositum = (FabricaCompositum*)xar_obtinere(composita, i);
            si (!chorda_aequalis_literis(compositum->titulus, argv[a]))
            {
                perge;
            }
            compositum_est = VERUM;
            si (nominata != NIHIL)
            {
                *(FabricaCompositum**)xar_addere(nominata) = compositum;
            }
            partes = fabrica_compositum_explicare(composita, actiones,
                compositum->titulus, piscina, &causa);
            per (j = ZEPHYRUM; partes != NIHIL
                && j < xar_numerus(partes);
                 j++)
            {
                *(chorda*)xar_addere(electa) =
                    *(chorda*)xar_obtinere(partes, j);
            }
        }
        si (!compositum_est)
        {
            *(chorda*)xar_addere(electa) = chorda_ex_literis(argv[a],
                piscina);
        }
    }

    suturae_legentem_parare(&sutura);
    suturae_currentem_addere(&sutura);
    suturae_agentem_addere(&sutura, actiones, siccum, piscina);
    sutura.sigilla         = sigilla;
    sutura.regenerationes  = tabula_dispersa_creare_chorda(piscina, 64);
    sutura.digesta = tabula_dispersa_creare_chorda(piscina,
        128);

    /* iudex ipse PRIMUS (Review Focus 1): iudex stalus digesta stala
     * scriberet et sanaret - nihil agitur */
    per (i = ZEPHYRUM; i < xar_numerus(actiones); i++)
    {
        FabricaActio* actio;

        actio = (FabricaActio*)xar_obtinere(actiones, i);
        per (j = ZEPHYRUM; j < xar_numerus(actio->exitus); j++)
        {
            FabricaExitus* exitus;
          FabricaIudicium  iudicium;

            exitus = (FabricaExitus*)xar_obtinere(actio->exitus, j);
            si (!chorda_aequalis_literis(exitus->via, "bin/fabrica"))
            {
                perge;
            }
            iudicium = fabrica_iudicare(&sutura, actio, exitus, FALSUM,
                piscina);
            si (iudicium.status != FABRICA_RECENS)
            {
                fprintf(stderr,
                    "fabrica: iudex ipse non recens (%.*s) - "
                    "./tools/fabrica_struere.sh prius, deinde iterum "
                    "sanare\n", (s32)iudicium.causa.mensura,
                    (constans character*)iudicium.causa.datum);
                redde II;
            }
        }
    }

    (vacuum)filum_directorium_creare_cum_parentibus(
        "build/fabrica/acta");
    sera = filum_seram_capere("build/fabrica/sera", ZEPHYRUM, piscina);
    si (sera == NIHIL)
    {
        fprintf(stderr, "fabrica: iudex plenus aut sanatio alia currit "
            "(build/fabrica/sera tenetur) - exspecta eam aut siste\n");
        redde II;
    }
    si (memoria_aperta)
    {
        suturae_memoriam_nectere(&sutura, &memoria);
    }
    sutura.radix = suturae_radix_absoluta(piscina);
    suturae_iudicii_parare(&sutura, actiones, piscina);
    {
        constans character* n = getenv("FABRICA_AUDITUS");

        si (auditus_sanandi == ZEPHYRUM && n != NIHIL && n[0] != '\0')
        {
            auditus_sanandi = (i32)strtoul(n, NIHIL, X);
        }
        sutura.auditus  = auditus_sanandi;
        sutura.audita   = tabula_dispersa_creare_chorda(piscina, 16);
    }

    printf(suturae_machina ? "NOTA\tsanare%s: iudicium plenum, ordine "
        "dependentiae\n" : "fabrica sanare%s: iudicium plenum, ordine "
        "dependentiae\n", siccum ? " -siccum" : "");
    fflush(stdout);
    /* STADIUM IUDICUM (fabrica-6 T3): iudices (iudex="verum") stali
     * PRIMUM sanantur, deinde cetera - nihil sub iudice stalo sanatur.
     * bin/fabrica ipse supra recusatur (se ipsum non restruit). */
    sanationes_iudicum = NIHIL;
    si (!siccum)
    {
        Xar* viae_iudicum = xar_creare(piscina, (i32)magnitudo(chorda));

        per (i = ZEPHYRUM; i < xar_numerus(actiones); i++)
        {
            FabricaActio* actio = (FabricaActio*)xar_obtinere(actiones,
                i);

            si (!actio->iudex)
            {
                perge;
            }
            per (j = ZEPHYRUM; j < xar_numerus(actio->exitus); j++)
            {
                FabricaExitus* exitus = (FabricaExitus*)xar_obtinere(
                    actio->exitus, j);

                si (   exitus->strategia->iudicatur
                    && fabrica_iudicare(&sutura, actio, exitus, FALSUM,
                           piscina).status != FABRICA_RECENS)
                {
                    *(chorda*)xar_addere(viae_iudicum) = exitus->via;
                }
            }
        }
        si (xar_numerus(viae_iudicum) > ZEPHYRUM)
        {
            printf(suturae_machina
                ? "NOTA\tstadium iudicum primum (%d)\n"
                : "fabrica sanare: stadium iudicum primum (%d)\n",
                (integer)xar_numerus(viae_iudicum));
            fflush(stdout);
            sanationes_iudicum = fabrica_sanare(&sutura, ordo,
                viae_iudicum,
                FALSUM, piscina, &causa);
            si (sanationes_iudicum == NIHIL)
            {
                fprintf(stderr, "fabrica: stadium iudicum: %.*s\n",
                    (s32)causa.mensura,
                    (constans character*)causa.datum);
                si (sutura.datum != NIHIL)
                {
                    scrinium_claudere(memoria.scrinium);
                }
                filum_seram_liberare(sera);
                redde II;
            }
        }
    }
    sanationes = fabrica_sanare(&sutura, ordo, electa, siccum, piscina,
        &causa);
    si (sanationes != NIHIL && sanationes_iudicum != NIHIL)
    {
        Xar* omnes = xar_creare(piscina,
            (i32)magnitudo(FabricaSanatio));

        per (i = ZEPHYRUM; i < xar_numerus(sanationes_iudicum); i++)
        {
            *(FabricaSanatio*)xar_addere(omnes) =
                *(FabricaSanatio*)xar_obtinere(sanationes_iudicum, i);
        }
        per (i = ZEPHYRUM; i < xar_numerus(sanationes); i++)
        {
            *(FabricaSanatio*)xar_addere(omnes) =
                *(FabricaSanatio*)xar_obtinere(sanationes, i);
        }
        sanationes = omnes;
    }
    si (sanationes == NIHIL)
    {
        fprintf(stderr, "fabrica: %.*s\n", (s32)causa.mensura,
            (constans character*)causa.datum);
        si (sutura.datum != NIHIL)
        {
            scrinium_claudere(memoria.scrinium);
        }
        filum_seram_liberare(sera);
        redde II;
    }

    per (i = ZEPHYRUM; i <= (i32)FABRICA_IUDICIUM; i++)
    {
        numeri[i] = ZEPHYRUM;
    }
    duratio       = ZEPHYRUM;
    aestimatio    = ZEPHYRUM;
    sine_tempore  = ZEPHYRUM;
    commissa      = FALSUM;
    per (i = ZEPHYRUM; i < xar_numerus(sanationes); i++)
    {
        FabricaSanatio* sanatio;

        sanatio = (FabricaSanatio*)xar_obtinere(sanationes, i);
        numeri[sanatio->eventus]++;
        duratio += sanatio->duratio_ms;
        si (suturae_machina)
        {
            fputs("SANATIO", stdout);
            _campus_literis(suturae_eventus_titulus(sanatio->eventus));
            suturae_campus(sanatio->actio->titulus);
            _campus_numerus((longus)sanatio->duratio_ms);
            suturae_campus(sanatio->causa);
            putchar('\n');
            si (   sanatio->eventus == FABRICA_AGENDUM
                || sanatio->eventus == FABRICA_FORTASSE)
            {
                si (sanatio->tempus_notum)
                {
                    aestimatio += sanatio->duratio_ms;
                }
                alioquin
                {
                    sine_tempore++;
                }
            }
            si (sanatio->eventus == FABRICA_SANATUM)
            {
                per (j = ZEPHYRUM; j
                    < xar_numerus(sanatio->actio->exitus);
                     j++)
                {
                    si (((FabricaExitus*)xar_obtinere(
                            sanatio->actio->exitus,
                            j))->strategia->octetis_comparat)
                    {
                        commissa = VERUM;
                    }
                }
            }
            perge;
        }
        printf("%-11s %.*s", suturae_eventus_titulus(sanatio->eventus),
            (s32)sanatio->actio->titulus.mensura,
            (constans character*)sanatio->actio->titulus.datum);
        si (   sanatio->eventus == FABRICA_AGENDUM
            || sanatio->eventus == FABRICA_FORTASSE)
        {
            /* aestimatio ex cursu ultimo (1b T7) */
            si (sanatio->tempus_notum)
            {
                printf(" (~%u.%u s)",
                    (insignatus integer)(sanatio->duratio_ms / M),
                    (insignatus integer)((sanatio->duratio_ms % M)
                        / C));
                aestimatio += sanatio->duratio_ms;
            }
            alioquin
            {
                printf(" (tempus ignotum)");
                sine_tempore++;
            }
        }
        alioquin si (sanatio->eventus != FABRICA_OMISSUM)
        {
            printf(" (%u.%u s)",
                (insignatus integer)(sanatio->duratio_ms / M),
                (insignatus integer)((sanatio->duratio_ms % M) / C));
        }
        si (sanatio->causa.mensura > 0)
        {
            _causam_imprimere(sanatio->causa);
        }
        alioquin
        {
            printf("\n");
        }
        si (sanatio->eventus == FABRICA_SANATUM)
        {
            per (j = ZEPHYRUM; j < xar_numerus(sanatio->actio->exitus);
                 j++)
            {
                FabricaExitus* exitus;

                exitus = (FabricaExitus*)xar_obtinere(
                    sanatio->actio->exitus, j);
                si (exitus->strategia->octetis_comparat)
                {
                    commissa = VERUM;
                }
            }
        }
    }
    si (siccum && xar_numerus(sanationes) > 0)
    {
        Xar* agendae;
        Xar* undae;

        /* undae (Q41): quae SIMUL currere possent - monstratur solum */
        agendae = xar_creare(piscina, (i32)magnitudo(FabricaActio*));
        per (i = ZEPHYRUM; i < xar_numerus(sanationes); i++)
        {
            *(constans FabricaActio**)xar_addere(agendae) =
                ((FabricaSanatio*)xar_obtinere(sanationes, i))->actio;
        }
        undae = fabrica_undas_formare(agendae, piscina);
        per (i = ZEPHYRUM; suturae_machina && undae != NIHIL
             && i < xar_numerus(undae); i++)
        {
            Xar* membra = *(Xar**)xar_obtinere(undae, i);

            fputs("UNDA", stdout);
            _campus_numerus((longus)(i + I));
            putchar('\t');
            per (j = ZEPHYRUM; j < xar_numerus(membra); j++)
            {
                FabricaActio* actio = *(FabricaActio**)xar_obtinere(
                    membra, j);

                printf(j > ZEPHYRUM ? " %.*s" : "%.*s",
                    (s32)actio->titulus.mensura,
                    (constans character*)actio->titulus.datum);
            }
            putchar('\n');
        }
        si (!suturae_machina)
        {
            printf("UNDAE (simul possent; sanare tutas - "
                "lectiones=\"verum\" - simul currit, fila %u; ceteras "
                "seriatim):\n",
                (insignatus integer)suturae_fila_maxima);
        }
        per (i = ZEPHYRUM; !suturae_machina && undae != NIHIL
            && i < xar_numerus(undae); i++)
        {
            Xar* membra;

            membra = *(Xar**)xar_obtinere(undae, i);
            printf("  %s:", chorda_ut_cstr(numerus_romanus_exprimere(
                (i64)(i + I), NIHIL, piscina), piscina));
            per (j = ZEPHYRUM; j < xar_numerus(membra); j++)
            {
                FabricaActio* actio;

                actio = *(FabricaActio**)xar_obtinere(membra, j);
                printf(" %.*s", (s32)actio->titulus.mensura,
                    (constans character*)actio->titulus.datum);
            }
            printf("\n");
        }
    }
    si (siccum && suturae_machina)
    {
        printf("SUMMA\t%u\t%u\t%u\t%u\tsiccum\n",
            (insignatus integer)numeri[FABRICA_AGENDUM],
            (insignatus integer)numeri[FABRICA_FORTASSE],
            (insignatus integer)aestimatio,
            (insignatus integer)sine_tempore);
    }
    alioquin si (suturae_machina)
    {
        printf("SUMMA\t%u\t%u\t%u\t%u\t%u\t%u\n",
            (insignatus integer)numeri[FABRICA_SANATUM],
            (insignatus integer)numeri[FABRICA_PRAEPARATUM],
            (insignatus integer)numeri[FABRICA_FRACTUM],
            (insignatus integer)numeri[FABRICA_OMISSUM],
            (insignatus integer)numeri[FABRICA_AUDITUM_DISCORS],
            (insignatus integer)duratio);
        si (commissa)
        {
            printf("NOTA\tplagulae generatae in arbore mutatae "
                "fortasse (git status) - committe per "
                "silva.commissio\n");
        }
    }
    alioquin si (siccum)
    {
        printf("fabrica sanare -siccum: %u agenda, %u fortasse - "
            "aestimatio ~%u s (%u sine tempore)\n",
            (insignatus integer)numeri[FABRICA_AGENDUM],
            (insignatus integer)numeri[FABRICA_FORTASSE],
            (insignatus integer)(aestimatio / M),
            (insignatus integer)sine_tempore);
    }
    alioquin
    {
        printf("fabrica sanare: %u sanata, %u praeparata, %u fracta, "
            "%u omissa%s (%u s)\n",
            (insignatus integer)numeri[FABRICA_SANATUM],
            (insignatus integer)numeri[FABRICA_PRAEPARATUM],
            (insignatus integer)numeri[FABRICA_FRACTUM],
            (insignatus integer)numeri[FABRICA_OMISSUM],
            numeri[FABRICA_AUDITUM_DISCORS]
                > 0 ? ", AUDITUM DISCORS" : "",
            (insignatus integer)(duratio / M));
        si (commissa)
        {
            printf("plagulae generatae in arbore mutatae fortasse "
                "(git status) - committe per silva.commissio; sanare "
                "numquam committit\n");
        }
    }
    /* VERDICTUM compositorum nominatorum (fabrica-6 T10) ex sanatione
     * ipsa: pars recens nisi actio eius fracta, omissa, discors aut
     * (siccum) agenda - iudicium alterum non eget */
    per (i = ZEPHYRUM; nominata != NIHIL
        && i < xar_numerus(nominata); i++)
    {
        FabricaCompositum* compositum;
                      Xar* partes;
                      Xar* iudicia;
                   chorda  verdictum;
                      i32  k;

        compositum = *(FabricaCompositum**)xar_obtinere(nominata, i);
        partes = fabrica_compositum_explicare(composita, actiones,
            compositum->titulus, piscina, &causa);
        iudicia = xar_creare(piscina, (i32)magnitudo(FabricaIudicium));
        per (j = ZEPHYRUM; partes != NIHIL && iudicia != NIHIL
             && j < xar_numerus(partes); j++)
        {
            FabricaIudicium* iudicium;
                     chorda  via;

            via = *(chorda*)xar_obtinere(partes, j);
            iudicium = (FabricaIudicium*)xar_addere(iudicia);
            iudicium->artificium = via;
            iudicium->status = FABRICA_RECENS;
            iudicium->causa = chorda_ex_literis("", piscina);
            per (k = ZEPHYRUM; k < xar_numerus(sanationes); k++)
            {
                FabricaSanatio* sanatio;
                           i32  e;

                sanatio = (FabricaSanatio*)xar_obtinere(sanationes, k);
                si (   sanatio->eventus != FABRICA_FRACTUM
                    && sanatio->eventus != FABRICA_OMISSUM
                    && sanatio->eventus != FABRICA_AUDITUM_DISCORS
                    && sanatio->eventus != FABRICA_AGENDUM
                    && sanatio->eventus != FABRICA_FORTASSE)
                {
                    perge;
                }
                per (e = ZEPHYRUM;
                     e < xar_numerus(sanatio->actio->exitus); e++)
                {
                    si (chorda_aequalis(((FabricaExitus*)xar_obtinere(
                            sanatio->actio->exitus, e))->via, via))
                    {
                        iudicium->status = FABRICA_STALUM;
                    }
                }
            }
        }
        si (suturae_machina)
        {
            _verdictum_machinae(iudicia, compositum->titulus);
            perge;
        }
        verdictum = fabrica_compositum_verdictum(iudicia,
            compositum->titulus, piscina);
        printf("VERDICTUM %.*s\n", (s32)verdictum.mensura,
            (constans character*)verdictum.datum);
    }
    si (sutura.datum != NIHIL)
    {
        scrinium_claudere(memoria.scrinium);
    }
    filum_seram_liberare(sera);
    si (siccum)
    {
        redde (numeri[FABRICA_AGENDUM] > 0) ? I : ZEPHYRUM;
    }
    redde (numeri[FABRICA_FRACTUM] + numeri[FABRICA_OMISSUM]
           + numeri[FABRICA_AUDITUM_DISCORS] > 0) ? I : ZEPHYRUM;
}


/* ==================================================
 * digestum
 * ================================================== */

interior s32
_digestum (
    constans character* titulus,
               Piscina* piscina)
{
    FabricaSutura  sutura;
              Xar* actiones;
           chorda  causa;
              i32  i;

    actiones = _declarationes_colligere(piscina, NIHIL);
    si (actiones == NIHIL)
    {
        redde II;
    }
    suturae_legentem_parare(&sutura);
    /* memoria sigillorum per cursum: communis semel sigillatur */
    sutura.sigilla    = tabula_dispersa_creare_chorda(piscina, 1024);
    per (i = ZEPHYRUM; i < xar_numerus(actiones); i++)
    {
         FabricaActio* actio;
             Sigillum  sigillum;
            character  hex[SIGILLUM_HEX_MENSURA];

        actio = (FabricaActio*)xar_obtinere(actiones, i);
        si (!chorda_aequalis_literis(actio->titulus, titulus))
        {
            perge;
        }
        causa = chorda_ex_literis("", piscina);
        /* EADEM functio ac iudex (fabrica_iudicare): plagula
         * provenientiae exclusa - aliter scriptum et iudex
         * dissentirent (T7: sic factum, statim deprehensum) */
        si (!fabrica_actionem_sigillare(&sutura, actio, piscina,
                &sigillum, &causa))
        {
            fprintf(stderr, "fabrica: %.*s\n", (s32)causa.mensura,
                (constans character*)causa.datum);
            redde I;
        }
        sigillum_hex(&sigillum, hex);
        printf("%s\n", hex);
        redde ZEPHYRUM;
    }
    fprintf(stderr, "fabrica: actio ignota '%s'\n", titulus);
    redde II;
}

externus constans ProvenientiaRelatio provenientia_fabrica;

/* PURGARE (plan 2 T3): thesaurus build/aedilis/obiecta - generationes
 * ultimae GENERATIONES_SERVANDAE (A3) servantur, cetera deleta;
 * -verificare blobos servatos verificat. Sub sera fabricae (nec iudex
 * plenus nec sanatio simul). Scriptor alienus simul (porta, aedilis)
 * cache solum amittere potest - numquam fructum falsum (blobi per
 * sigilla, actio sine blobo = defectus memoriae). */
#define GENERATIONES_SERVANDAE V
#define THESAURI_RADIX "build/aedilis/obiecta"

interior s32
_purgare (
          s32   argc,
    character** argv,
      Piscina*  piscina)
{
     Thesaurus* thesaurus;
     FilumSera* sera;
           b32  verificare;
           i32  deleta;
           s32  a;

    verificare = FALSUM;
    per (a = II; a < argc; a++)
    {
        si (strcmp(argv[a], "-verificare") == 0)
        {
            verificare = VERUM;
        }
        alioquin
        {
            fprintf(stderr, "fabrica: vexillum ignotum %s\n", argv[a]);
            redde II;
        }
    }
    si (!filum_directorium_existit(THESAURI_RADIX))
    {
        printf("fabrica purgare: thesaurus nullus (%s)\n",
            THESAURI_RADIX);
        redde ZEPHYRUM;
    }
    (vacuum)filum_directorium_creare_cum_parentibus("build/fabrica");
    sera = filum_seram_capere("build/fabrica/sera", ZEPHYRUM, piscina);
    si (sera == NIHIL)
    {
        fprintf(stderr, "fabrica: iudex plenus aut sanatio alia currit "
            "(build/fabrica/sera tenetur) - exspecta eam aut siste\n");
        redde II;
    }
    thesaurus  = thesaurus_aperire(THESAURI_RADIX, piscina);
    deleta     = ZEPHYRUM;
    si (   thesaurus == NIHIL
        || !thesaurus_purgare(thesaurus, GENERATIONES_SERVANDAE,
        verificare, &deleta))
    {
        fprintf(stderr, "fabrica purgare: thesaurus purgari nequit "
            "(%s)\n", THESAURI_RADIX);
        filum_seram_liberare(sera);
        redde II;
    }
    printf("fabrica purgare: %u deleta (generationes servatae %u%s)\n",
        (insignatus integer)deleta,
        (insignatus integer)GENERATIONES_SERVANDAE,
        verificare ? ", blobi verificati" : "");
    filum_seram_liberare(sera);
    redde ZEPHYRUM;
}

interior vacuum
_usus (vacuum);

/* CENSUS (fabrica-6 T1): genera ingressus registrata cum proprietatibus
 * chassis, TSV - 'genus<TAB>titulus<TAB>particulae<TAB>reproducibile
 * <TAB>sumptus<TAB>enumerat<TAB>locat'. Additio quae proprietatem
 * implere nequit hic CLASSIFICATUR (signum designi), non recusatur. */
interior s32
_censum (vacuum)
{
    i32 i;
    i32 nominatae       = ZEPHYRUM;
    i32 reproducibilia  = ZEPHYRUM;
    i32 invisibilia     = ZEPHYRUM;

    printf("# census chassis fabricae (genera ingressus)\n");
    per (i = ZEPHYRUM; i < fabrica_genera_numerus(); i++)
    {
        constans FabricaGenus* g = fabrica_genus_obtinere(i);
           constans character* sumptus =
               g->sumptus == FABRICA_SUMPTUS_VILIS ? "vilis"
               : g->sumptus
                   == FABRICA_SUMPTUS_MEDIUS ? "medius" : "carus";

        printf("genus\t%s\tpar:%s\tparticulae:%s\treproducibile:%s"
            "\tsumptus:%s\tloci:%s\tlocat:%s\n", g->titulus,
            fabrica_genus_par(g)
                != NIHIL ? fabrica_genus_par(g) : "nullum",
            g->particulae_nominatae ? "nominatae" : "crassae",
            g->reproducibile ? "ita" : "non", sumptus,
            fabrica_genus_loci(g),
            g->locare != NIHIL ? "ita" : "non");
        si (strcmp(fabrica_genus_loci(g), "nulli") == ZEPHYRUM)
        {
            invisibilia++;
        }
        si (g->particulae_nominatae)
        {
            nominatae++;
        }
        si (g->reproducibile)
        {
            reproducibilia++;
        }
    }
    printf("census: genera %d (particulae nominatae %d, reproducibilia %d,"
        " quaestionibus inversis invisibilia %d)\n",
        (integer)fabrica_genera_numerus(), (integer)nominatae,
        (integer)reproducibilia, (integer)invisibilia);
    redde ZEPHYRUM;
}

/* CAUSAE (plan 5 T1): cur actiones actae sunt - cursus per (titulus,
 * eventus, stalum), frequentissimae primum; titulus optionalis. Ordines
 * ante migrationem V causam non habent ('non notata'). */
interior s32
_causae (
          s32   argc,
    character** argv,
      Piscina*  piscina)
{
               Memoria  memoria;
    ScriniumEnuntiatum* enuntiatum;
                   s64  summa = ZEPHYRUM;

    si (argc > III)
    {
        _usus();
        redde II;
    }
    si (!suturae_memoriam_aperire(&memoria, piscina))
    {
        redde II;
    }
    enuntiatum = scrinium_praeparare(memoria.scrinium, argc == III
        ? "SELECT titulus, eventus, stalum, COUNT(*) FROM cursus"
          " WHERE eventus != 'IUDICIUM' AND titulus = ?"
          " GROUP BY titulus, eventus, stalum ORDER BY COUNT(*) DESC"
        : "SELECT titulus, eventus, stalum, COUNT(*) FROM cursus"
          " WHERE eventus != 'IUDICIUM'"
          " GROUP BY titulus, eventus, stalum ORDER BY COUNT(*) DESC");
    si (enuntiatum == NIHIL)
    {
        scrinium_claudere(memoria.scrinium);
        redde II;
    }
    si (argc == III)
    {
        (vacuum)scrinium_ligare_textum(enuntiatum, I,
            chorda_ex_literis(argv[2], piscina));
    }
    dum (scrinium_gradi(enuntiatum) == SCRINIUM_ORDO)
    {
        chorda titulus = scrinium_columna_textus(enuntiatum, ZEPHYRUM,
            piscina);
        chorda eventus = scrinium_columna_textus(enuntiatum, I,
            piscina);
        chorda stalum = scrinium_columna_textus(enuntiatum, II,
            piscina);
           s64 numerus = scrinium_columna_numerus(enuntiatum, III);

        summa += numerus;
        printf("%6ld  %-28.*s %-9.*s %.*s\n", (longus)numerus,
            (s32)titulus.mensura, (constans character*)titulus.datum,
            (s32)eventus.mensura, (constans character*)eventus.datum,
            stalum.mensura > 0 ? (s32)stalum.mensura : XV,
            stalum.mensura > 0 ? (constans character*)stalum.datum
                               : "(non notata)");
    }
    scrinium_finire(enuntiatum);
    scrinium_claudere(memoria.scrinium);
    printf("fabrica causae: %ld cursus\n", (longus)summa);
    redde ZEPHYRUM;
}

interior vacuum
_usus (vacuum)
{
    fprintf(stderr,
        "usus: bin/fabrica iudicare [-plenus] [-audit] [-omnia] "
        "[artificium...]\n"
        "      bin/fabrica iudicare -plenus -tacta VIA...\n"
        "      bin/fabrica sanare [-siccum] "
        "[artificium|compositum...]\n"
        "      bin/fabrica digestum TITULUS\n"
        "      bin/fabrica purgare [-verificare]\n"
        "      bin/fabrica causae [titulus]  (cur actae sunt)\n"
        "      bin/fabrica census  (genera chassis et proprietates)\n"
        "(ex radice repositorii; exitus 0 sanum, 1 stalum/ignotum, "
        "2 nihil iudicatum)\n");
}

s32
principale (
          s32   argc,
    character** argv)
{
     Piscina* piscina;
         s32  exitus;

    /* ante custodiam cwd: relatio ex quovis directorio */
    si (provenientia_respondere(argc, argv, &provenientia_fabrica))
    {
        redde ZEPHYRUM;
    }
    si (argc < II)
    {
        _usus();
        redde II;
    }
    si (   !filum_existit("fabrica.stml")
        || !filum_existit("aedilis.stml"))
    {
        fprintf(stderr, "fabrica: curre ex radice repositorii "
            "(fabrica.stml et aedilis.stml desunt)\n");
        redde II;
    }
    piscina = piscina_generare_dynamicum("fabrica", 1048576);
    si (piscina == NIHIL)
    {
        redde II;
    }
    /* praefixum thesauri extractoris */
    suturae_via_binarii = argv[0];
    /* THESAURUS (plan 2 T3): cursus hic = generatio una pro omnibus
     * actionibus eius (aedilis in generatoribus); cursus exterior
     * (porta) suam servat */
    si (getenv("THESAURUS_GENERATIO") == NIHIL)
    {
           character generatio[LXIV];
              time_t nunc;

        nunc = time(NIHIL);
        (vacuum)strftime(generatio, magnitudo(generatio),
            "%Y%m%dT%H%M%S", gmtime(&nunc));
        sprintf(generatio + strlen(generatio), "-fabrica-%ld",
            (longus)getpid());
        (vacuum)setenv("THESAURUS_GENERATIO", generatio, I);
    }
    si (strcmp(argv[1], "iudicare") == 0)
    {
        exitus = _iudicare(argc, argv, piscina);
    }
    alioquin si (strcmp(argv[1], "sanare") == 0)
    {
        exitus = _sanare(argc, argv, piscina);
    }
    alioquin si (strcmp(argv[1], "digestum") == 0 && argc == III)
    {
        exitus = _digestum(argv[2], piscina);
    }
    alioquin si (strcmp(argv[1], "purgare") == 0)
    {
        exitus = _purgare(argc, argv, piscina);
    }
    alioquin si (strcmp(argv[1], "causae") == 0)
    {
        exitus = _causae(argc, argv, piscina);
    }
    alioquin si (strcmp(argv[1], "census") == 0)
    {
        exitus = _censum();
    }
    alioquin
    {
        _usus();
        exitus = II;
    }
    piscina_destruere(piscina);
    redde exitus;
}
