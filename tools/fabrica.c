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

/* SECTIONES MEMBRI (fabrica-7 T1): post IUDICIUM verdicti membri
 * (build/fabrica/area/<id>/verdictum.txt) plagula credo.tsv eiusdem
 * areae - machina: SECTIO pro omni sectione (sine tempore); homo:
 * sectiones non transeuntes et SUITA absens solae. Plagula absens
 * (nondum cursum, nexus solum) = nihil. */
interior vacuum
_sectiones_imprimere (
    constans FabricaSutura* sutura,
                    chorda  artificium,
                   Piscina* piscina)
{
    constans character* praefixum  = "build/fabrica/area/";
    constans character* suffixum   = "/verdictum.txt";
      FabricaSectiones  sectiones;
                chorda  id;
                chorda  causa;
        memoriae_index  p;
        memoriae_index  q;
                   i32  i;

    p = strlen(praefixum);
    q = strlen(suffixum);
    si (   (memoriae_index)artificium.mensura     <= p + q
        || memcmp(artificium.datum, praefixum, p) != ZEPHYRUM
        || memcmp(artificium.datum + artificium.mensura - (i32)q,
               suffixum, q) != ZEPHYRUM)
    {
        redde;
    }
    id = chorda_sectio(artificium, (i32)p, artificium.mensura - (i32)q);
    causa = chorda_ex_literis("", piscina);
    si (!fabrica_sectiones_legere(sutura,
        chorda_ut_cstr(chorda_concatenare(
            chorda_ex_literis(praefixum, piscina),
            chorda_concatenare(id,
            chorda_ex_literis("/credo.tsv", piscina), piscina),
            piscina),
            piscina), piscina, &sectiones, &causa))
    {
        si (   !suturae_machina && !chorda_incipit(causa,
                chorda_ex_literis("plagula sectionum absens", piscina)))
        {
            printf("  sectiones: %.*s\n", (s32)causa.mensura,
                (constans character*)causa.datum);
        }
        redde;
    }
    per (i = ZEPHYRUM; i < xar_numerus(sectiones.sectiones); i++)
    {
        constans FabricaSectio* s = (constans FabricaSectio*)
            xar_obtinere(sectiones.sectiones, i);

        si (suturae_machina)
        {
            fputs("SECTIO", stdout);
            suturae_campus(id);
            suturae_campus(s->titulus);
            suturae_campus(s->exitus);
            _campus_numerus((longus)s->praeteriti);
            _campus_numerus((longus)s->totales);
            suturae_campus(s->fractura);
            putchar('\n');
        }
        alioquin si (!chorda_aequalis_literis(s->exitus, "TRANSIIT"))
        {
            printf("  sectio '%.*s' %.*s %u/%u%s%.*s\n",
                (s32)s->titulus.mensura,
                (constans character*)s->titulus.datum,
                (s32)s->exitus.mensura,
                (constans character*)s->exitus.datum,
                (insignatus integer)s->praeteriti,
                (insignatus integer)s->totales,
                s->fractura.mensura > ZEPHYRUM ? " ad " : "",
                (s32)s->fractura.mensura,
                (constans character*)s->fractura.datum);
        }
    }
    si (!suturae_machina && !sectiones.completa)
    {
        printf("  SUITA absens (ruina post sectionem ultimam "
            "notatam)\n");
    }
}

interior vacuum
_sententiam_imprimere (
    constans FabricaSutura* sutura,
                   Piscina* piscina,
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
        _sectiones_imprimere(sutura, sententia->iudicium.artificium,
            piscina);
        redde;
    }
    printf("%s %.*s - %.*s\n", signum,
        (s32)sententia->iudicium.artificium.mensura,
        (constans character*)sententia->iudicium.artificium.datum,
        (s32)sententia->iudicium.causa.mensura,
        (constans character*)sententia->iudicium.causa.datum);
    _sectiones_imprimere(sutura, sententia->iudicium.artificium,
        piscina);
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


/* ==================================================
 * CURSUS MANDATI (fabrica-6 H4b): status communis iudicare et sanare -
 * declarationes, memoria, ordo per locos, electa, sutura, sera
 * ================================================== */

nomen structura {
          Piscina* piscina;
              Xar* actiones;     /* declarationes */
              Xar* composita;
              Xar* ordo;         /* per locos */
   TabulaDispersa* sigilla;      /* memoria sigillorum per cursum */
          Memoria  memoria;
              b32  memoria_aperta;
        FilumSera* sera;         /* NIHIL = non capta */
    FabricaSutura  sutura;
              Xar* electa;       /* NIHIL = omnia */
              Xar* nominata;     /* FabricaCompositum* nominata */
           chorda  causa;
} CursusFabricae;

/* declarationes, memoria aperta, ordo per locos. FALSUM = exitus II
 * (causa iam nuntiata; _cursum_finire tamen vocandum) */
interior b32
_cursum_parare (
    CursusFabricae* c,
           Piscina* piscina)
{
    memset(c, ZEPHYRUM, magnitudo(CursusFabricae));
    c->piscina   = piscina;
    c->nominata  = xar_creare(piscina,
        (i32)magnitudo(FabricaCompositum*));
    c->actiones  = _declarationes_colligere(piscina, &c->composita);
    si (c->actiones == NIHIL)
    {
        redde FALSUM;
    }
    c->causa           = chorda_ex_literis("", piscina);
    c->sigilla         = tabula_dispersa_creare_chorda(piscina, 1024);
    c->memoria_aperta  = suturae_memoriam_aperire(&c->memoria, piscina);
    c->ordo = _ordinare_per_locos(c->actiones, c->sigilla, piscina,
        c->memoria_aperta ? &c->memoria : NIHIL, &c->causa);
    si (c->ordo == NIHIL)
    {
        fprintf(stderr, "fabrica: %.*s\n", (s32)c->causa.mensura,
            (constans character*)c->causa.datum);
        redde FALSUM;
    }
    redde VERUM;
}

/* argumenta positionalia: via artificii aut titulus COMPOSITI (partes
 * eius eliguntur; compositum in 'nominata' - iudicium aut verdictum
 * eius post partes imprimitur). Nullum argumentum: electa NIHIL */
interior vacuum
_electa_colligere (
    CursusFabricae*  c,
               s32   argc,
         character** argv)
{
    s32 a;
    i32 i;
    i32 j;

    per (a = II; a < argc; a++)
    {
        chorda argumentum;
           b32 compositum_est;

        si (argv[a][0] == '-')
        {
            perge;
        }
        si (c->electa == NIHIL)
        {
            c->electa = xar_creare(c->piscina, (i32)magnitudo(chorda));
        }
        argumentum      = chorda_ex_literis(argv[a], c->piscina);
        compositum_est  = FALSUM;
        per (i = ZEPHYRUM; i < xar_numerus(c->composita); i++)
        {
            FabricaCompositum* compositum;
                          Xar* partes;

            compositum = (FabricaCompositum*)xar_obtinere(c->composita,
                i);
            si (!chorda_aequalis(compositum->titulus, argumentum))
            {
                perge;
            }
            compositum_est                                 = VERUM;
            *(FabricaCompositum**)xar_addere(c->nominata)  = compositum;
            partes = fabrica_compositum_explicare(c->composita,
                c->actiones, compositum->titulus, c->piscina,
                &c->causa);
            per (j = ZEPHYRUM; partes != NIHIL
                && j < xar_numerus(partes); j++)
            {
                *(chorda*)xar_addere(c->electa) =
                    *(chorda*)xar_obtinere(partes, j);
            }
        }
        si (!compositum_est)
        {
            *(chorda*)xar_addere(c->electa) = argumentum;
        }
    }
}

/* finis omnis exitus post parationem: memoria clausa, sera liberata */
interior vacuum
_cursum_finire (
    CursusFabricae* c)
{
    si (c->memoria_aperta)
    {
        scrinium_claudere(c->memoria.scrinium);
        c->memoria_aperta = FALSUM;
    }
    filum_seram_liberare(c->sera);
    c->sera = NIHIL;
}

/* partes compositi nominati: viae artificiorum eius */
interior Xar*
_partes_compositi (
       CursusFabricae* c,
    FabricaCompositum* compositum)
{
    redde fabrica_compositum_explicare(c->composita, c->actiones,
        compositum->titulus, c->piscina, &c->causa);
}

/* cursus iudicandi: optiones, sententiae, iudex stalus, numeri */
nomen structura {
    CursusFabricae  cursus;
               b32  plenus;
               b32  omnia;
               b32  tacta;
               i32  auditus;
               Xar* viae_tactae;   /* -tacta: viae commissionis */
               Xar* sententiae;    /* Sententia */
      FabricaActio* iudex_actio;   /* iudex stalus; NIHIL = nullus */
               i32  recentia;
               i32  stala;
               i32  ignota;
               i32  non_iudicata;
} CursusIudicandi;

/* vexilla iudicandi et viae tactae. FALSUM = vexillum ignotum */
interior b32
_iudicare_optiones (
    CursusIudicandi*  t,
                s32   argc,
          character** argv,
            Piscina*  piscina)
{
    s32 a;

    /* A2: unus ex XX ictuum sub -plenus regeneratur */
    t->auditus = XX;
    per (a = II; a < argc; a++)
    {
        si (strcmp(argv[a], "-plenus") == 0)
        {
            t->plenus = VERUM;
        }
        alioquin si (strcmp(argv[a], "-audit") == 0)
        {
            /* AUDITUS omnium (plan 2 T2): omnis ictus vestigii aut
             * memoriae regeneratur et confertur; -plenus implicat */
            t->plenus   = VERUM;
            t->auditus  = I;
        }
        alioquin si (strcmp(argv[a], "-tacta") == 0)
        {
            /* argumenta positionalia = viae commissionis: artificia
             * GENERATA (regeneratio) quae tangunt iudicantur, non
             * installata (binaria non committuntur; uncus sessionis ea
             * nominat). Nullum tactum = exitus 0 (T8 gradus III) */
            t->tacta = VERUM;
        }
        alioquin si (strcmp(argv[a], "-omnia") == 0)
        {
            /* RECENS quoque cum causa ('memoria', 'regeneratio
             * congruit', 'relatio congruit') - plantae T6 */
            t->omnia = VERUM;
        }
        alioquin si (strcmp(argv[a], "-machina") == 0)
        {
            suturae_machina = VERUM;   /* fabrica-6 H1: lineae TSV */
        }
        alioquin si (argv[a][0] == '-')
        {
            fprintf(stderr, "fabrica: vexillum ignotum %s\n", argv[a]);
            redde FALSUM;
        }
    }
    t->viae_tactae = xar_creare(piscina, (i32)magnitudo(chorda));
    per (a = II; t->tacta && a < argc; a++)
    {
        si (argv[a][0] != '-')
        {
            *(chorda*)xar_addere(t->viae_tactae) =
                chorda_ex_literis(argv[a], piscina);
        }
    }
    redde VERUM;
}

/* SERA sub -plenus: generatores clausuras et scripturas VACUANT -
 * iudex plenus alter simul eas sub pedibus huius deleret (T6:
 * factum, claves ternae per actionem). flock: nucleus eam cum
 * processu liberat, nulla sera stala. Celer nihil scribit: libera.
 * Deinde sutura vera nectitur. FALSUM = sera tenetur */
interior b32
_iudicare_suturam (
    CursusIudicandi* t)
{
    CursusFabricae* c = &t->cursus;

    si (t->plenus)
    {
        (vacuum)filum_directorium_creare_cum_parentibus(
            "build/fabrica");
        c->sera = filum_seram_capere("build/fabrica/sera", ZEPHYRUM,
            c->piscina);
        si (c->sera == NIHIL)
        {
            fprintf(stderr, "fabrica: iudex plenus alius currit "
                "(build/fabrica/sera tenetur) - exspecta eum aut "
                "siste\n");
            redde FALSUM;
        }
    }
    suturae_legentem_parare(&c->sutura);
    suturae_currentem_addere(&c->sutura);
    si (c->memoria_aperta)
    {
        suturae_memoriam_nectere(&c->sutura, &c->memoria);
    }
    c->sutura.radix    = suturae_radix_absoluta(c->piscina);
    suturae_iudicii_parare(&c->sutura, c->actiones, c->piscina);
    c->sutura.auditus  = t->auditus;
    /* memoria sigillorum per cursum: communis semel sigillatur (iam
     * impleta per ordinem per locos) */
    c->sutura.sigilla  = c->sigilla;
    /* generator semel per actionem (exitus multi, cursus unus) */
    c->sutura.regenerationes = tabula_dispersa_creare_chorda(c->piscina,
        64);
    /* sigillum ingressuum semel per actionem */
    c->sutura.digesta = tabula_dispersa_creare_chorda(c->piscina, 128);
    redde VERUM;
}

/* PRAEVISIO (T6b): sub -plenus regenerationes actionum tutarum
 * (lectiones) quas iudicium hoc posceret SIMUL currunt; iudicium
 * postea (ordine suo, immutatum) eas memoratas invenit */
interior vacuum
_iudicare_praevidere (
    CursusIudicandi* t)
{
    CursusFabricae* c       = &t->cursus;
     FabricaSutura* sutura  = &c->sutura;
               Xar* praevidendae;
               i32  i;
               i32  j;

    suturae_fila_maxima = suturae_fila_computare(c->piscina);
    si (!t->plenus || suturae_fila_maxima <= I)
    {
        redde;
    }
    sutura->currere_simul = suturae_currere_simul;
    praevidendae = xar_creare(c->piscina,
        (i32)magnitudo(FabricaActio*));
    per (i = ZEPHYRUM; praevidendae != NIHIL
         && i < xar_numerus(c->ordo); i++)
    {
        FabricaActio* actio;
                 b32  iudicanda;

        actio = *(FabricaActio**)xar_obtinere(c->ordo, i);
        si (   !actio->lectiones
            || actio->genus == FABRICA_ACTIO_IUDICIUM)
        {
            perge;   /* iudicium: verdictum numquam praevidetur */
        }
        iudicanda = FALSUM;
        si (t->tacta)
        {
            iudicanda = fabrica_actio_tacta(sutura, actio,
                t->viae_tactae, c->piscina);
        }
        alioquin
        {
            per (j = ZEPHYRUM; j < xar_numerus(actio->exitus); j++)
            {
                FabricaExitus* exitus;

                exitus = (FabricaExitus*)xar_obtinere(actio->exitus, j);
                si (   exitus->strategia->iudicatur
                    && _electum(c->electa, exitus->via))
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
        fabrica_regenerationes_praevidere(sutura, praevidendae,
            t->plenus, c->piscina);
    }
}

/* STADIUM IUDICUM (fabrica-6 T3): actiones iudex="verum" primum;
 * quaevis non recens -> ceteri NON IUDICATI nominantes eam (iudex
 * stalus sententias suspectas faceret). Sententia iudicis stali ipsa
 * PRIMA (numeratur, sanatio eum nominat) - etiam si non electa */
interior vacuum
_iudicare_iudices (
    CursusIudicandi* t)
{
    CursusFabricae* c = &t->cursus;
               i32  i;
               i32  j;

    t->iudex_actio  = NIHIL;
    t->sententiae   = xar_creare(c->piscina, (i32)magnitudo(Sententia));
    per (i = ZEPHYRUM; i < xar_numerus(c->actiones)
        && t->iudex_actio == NIHIL;
         i++)
    {
        FabricaActio* actio = (FabricaActio*)xar_obtinere(c->actiones,
            i);

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
            iudicium = fabrica_iudicare(&c->sutura, actio, exitus,
                t->plenus, c->piscina);
            si (iudicium.status != FABRICA_RECENS)
            {
                Sententia* sententia =
                    (Sententia*)xar_addere(t->sententiae);

                sententia->actio     = actio;
                sententia->iudicium  = iudicium;
                t->iudex_actio       = actio;
                frange;
            }
        }
    }
}

/* sententiae ordine dependentiae: electa (aut tacta generata); sub
 * iudice stalo ceterae NON IUDICATAE */
interior vacuum
_iudicare_sententias (
    CursusIudicandi* t)
{
    CursusFabricae* c = &t->cursus;
               i32  i;
               i32  j;

    per (i = ZEPHYRUM; i < xar_numerus(c->ordo); i++)
    {
        FabricaActio* actio;

        actio = *(FabricaActio**)xar_obtinere(c->ordo, i);
        si (actio == t->iudex_actio)
        {
            perge;   /* sententia eius iam prima */
        }
        si (   t->tacta
            && !fabrica_actio_tacta(&c->sutura, actio, t->viae_tactae,
            c->piscina))
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
                si (suturae_machina && _electum(c->electa, exitus->via))
                {
                    fputs("PRAECONDICIO", stdout);
                    suturae_campus(exitus->via);
                    suturae_campus(actio->titulus);
                    putchar('\n');
                }
                alioquin si (   t->omnia && _electum(c->electa,
                             exitus->via))
                {
                    printf("PRAECONDICIO %.*s - %.*s (ignota, numquam "
                        "iudicatur)\n", (s32)exitus->via.mensura,
                        (constans character*)exitus->via.datum,
                        (s32)actio->titulus.mensura,
                        (constans character*)actio->titulus.datum);
                }
                perge;
            }
            si (t->tacta)
            {
                /* commissa generata solum: strategia quae octetis
                 * comparat */
                si (!exitus->strategia->octetis_comparat)
                {
                    perge;
                }
            }
            alioquin si (!_electum(c->electa, exitus->via))
            {
                perge;
            }
            /* iudicium (porta, spec 3 par. XII) solum NOMINATUM
             * iudicatur: verritio sine argumentis id omittit */
            si (   c->electa    == NIHIL
                && actio->genus == FABRICA_ACTIO_IUDICIUM)
            {
                perge;
            }
            sententia         = (Sententia*)xar_addere(t->sententiae);
            sententia->actio  = actio;
            si (t->iudex_actio != NIHIL && !actio->iudex)
            {
                sententia->iudicium.artificium  = exitus->via;
                sententia->iudicium.status      = FABRICA_NON_IUDICATUM;
                sententia->iudicium.causa       =
                    _iungere_iudicem(c->piscina,
                    t->iudex_actio->titulus);
                perge;
            }
            sententia->iudicium = fabrica_iudicare(&c->sutura, actio,
                exitus, t->plenus, c->piscina);
        }
    }
}

/* iudex ipse PRIMUS: si bin/fabrica stalum est, ceterae sententiae ab
 * eo scriptae suspectae sunt; deinde ceterae, numeratae */
interior vacuum
_iudicare_imprimere (
    CursusIudicandi* t)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(t->sententiae); i++)
    {
        Sententia* s;

        s = (Sententia*)xar_obtinere(t->sententiae, i);
        si (chorda_aequalis_literis(s->iudicium.artificium,
                "bin/fabrica"))
        {
            _sententiam_imprimere(&t->cursus.sutura,
                t->cursus.piscina, s, t->omnia);
        }
    }
    per (i = ZEPHYRUM; i < xar_numerus(t->sententiae); i++)
    {
        Sententia* s;

        s = (Sententia*)xar_obtinere(t->sententiae, i);
        commutatio (s->iudicium.status)
        {
            casus FABRICA_RECENS:
                t->recentia++;
                frange;
            casus FABRICA_STALUM:
                t->stala++;
                frange;
            casus FABRICA_IGNOTUM:
                t->ignota++;
                frange;
            ordinarius:
                t->non_iudicata++;
                frange;
        }
        si (!chorda_aequalis_literis(s->iudicium.artificium,
                "bin/fabrica"))
        {
            _sententiam_imprimere(&t->cursus.sutura,
                t->cursus.piscina, s, t->omnia);
        }
    }
}

/* composita nominata: pessimum partium, post partes; VERDICTUM
 * (fabrica-6 T7) '<titulus>: N/M' ex partibus */
interior vacuum
_iudicare_composita (
    CursusIudicandi* t)
{
    CursusFabricae* c = &t->cursus;
               i32  i;
               i32  j;

    per (i = ZEPHYRUM; i < xar_numerus(c->nominata); i++)
    {
        FabricaCompositum* compositum;
          FabricaIudicium  coniunctum;
                      Xar* partes;
                      Xar* iudicia;
                   chorda  verdictum;

        compositum = *(FabricaCompositum**)xar_obtinere(c->nominata, i);
        partes = _partes_compositi(c, compositum);
        iudicia = xar_creare(c->piscina,
            (i32)magnitudo(FabricaIudicium));
        per (j = ZEPHYRUM; j < xar_numerus(t->sententiae); j++)
        {
            Sententia* s;

            s = (Sententia*)xar_obtinere(t->sententiae, j);
            si (_electum(partes, s->iudicium.artificium))
            {
                *(FabricaIudicium*)xar_addere(iudicia) = s->iudicium;
            }
        }
        coniunctum = fabrica_iudicia_coniungere(iudicia,
            compositum->titulus, c->piscina);
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
        verdictum = fabrica_compositum_verdictum(iudicia,
            compositum->titulus, c->piscina);
        printf("VERDICTUM %.*s\n", (s32)verdictum.mensura,
            (constans character*)verdictum.datum);
    }
}

/* SANATIO: actiones artificiorum stalorum/ignotorum, ordine
 * dependentiae, semel quaeque */
interior vacuum
_iudicare_sanationem (
    CursusIudicandi* t)
{
    FabricaActio* prior;
             i32  i;
             i32  k;

    si (t->stala + t->ignota == 0)
    {
        redde;
    }
    si (!suturae_machina)
    {
        printf("SANATIO:\n");
    }
    prior = NIHIL;
    per (i = ZEPHYRUM; i < xar_numerus(t->sententiae); i++)
    {
        Sententia* s;

        s = (Sententia*)xar_obtinere(t->sententiae, i);
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
            per (k = ZEPHYRUM; k < xar_numerus(s->actio->mandatum); k++)
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

/* orphana et binaria sine declaratione solum sine filtro artificiorum
 * (vexilla non filtrant); numerus orphanorum */
interior i32
_iudicare_orphana (
    CursusIudicandi* t)
{
    CursusFabricae* c = &t->cursus;
               i32  orphana;
               i32  sine;
               Xar* areae;
               i32  a;

    si (t->tacta || c->electa != NIHIL)
    {
        redde ZEPHYRUM;
    }
    orphana = _orphana_imprimere(c->piscina);
    /* areae graduum sine membro (fabrica-6 T5): nuntiatae */
    areae = fabrica_areas_orphanas(&c->sutura, c->actiones, c->piscina);
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
    sine = _binaria_sine_declaratione(c->actiones, c->piscina);
    si (sine > 0)
    {
        printf(suturae_machina ? "BINARIA\t%u\n"
            : "IGNOTUM: %u binaria in bin/ sine declaratione "
            "(numerus solus, Q36)\n", (insignatus integer)sine);
    }
    redde orphana;
}

interior s32
_iudicare (
          s32   argc,
    character** argv,
      Piscina*  piscina)
{
    CursusIudicandi t;
                i32 orphana;
                s32 exitus;

    memset(&t, ZEPHYRUM, magnitudo(CursusIudicandi));
    si (!_iudicare_optiones(&t, argc, argv, piscina))
    {
        redde II;
    }
    si (!_cursum_parare(&t.cursus, piscina))
    {
        _cursum_finire(&t.cursus);
        redde II;
    }
    si (!t.tacta)
    {
        _electa_colligere(&t.cursus, argc, argv);
    }
    si (!_iudicare_suturam(&t))
    {
        _cursum_finire(&t.cursus);
        redde II;
    }
    _iudicare_praevidere(&t);
    _iudicare_iudices(&t);
    _iudicare_sententias(&t);
    si (xar_numerus(t.sententiae) == 0 && t.tacta)
    {
        printf(suturae_machina
            ? "NOTA\tnulla artificia generata a viis tacta\n"
            : "fabrica: nulla artificia generata a viis tacta\n");
        _cursum_finire(&t.cursus);
        redde ZEPHYRUM;
    }
    si (xar_numerus(t.sententiae) == 0)
    {
        fprintf(stderr, "fabrica: nihil iudicatum (nulla artificia "
            "declarata%s)\n",
            t.cursus.electa == NIHIL ? "" : " congruunt");
        _cursum_finire(&t.cursus);
        redde II;
    }
    _iudicare_imprimere(&t);
    _iudicare_composita(&t);
    _iudicare_sanationem(&t);
    orphana = _iudicare_orphana(&t);
    si (suturae_machina)
    {
        printf("SUMMA\t%u\t%u\t%u\t%u\t%u\t%s\n",
            (insignatus integer)t.recentia, (insignatus integer)t.stala,
            (insignatus integer)t.ignota,
            (insignatus integer)t.non_iudicata,
            (insignatus integer)orphana, t.plenus ? "plenus" : "celer");
    }
    alioquin
    {
        printf("fabrica: %u recentia, %u stala, %u ignota, %u non "
            "iudicata%s, %u orphana\n",
            (insignatus integer)t.recentia, (insignatus integer)t.stala,
            (insignatus integer)t.ignota,
            (insignatus integer)t.non_iudicata,
            t.plenus ? "" : " (celer)", (insignatus integer)orphana);
    }
    exitus = (t.stala + t.ignota > 0) ? I : ZEPHYRUM;
    _cursum_finire(&t.cursus);
    redde exitus;
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

/* cursus sanandi: sanationes et numeri earum */
nomen structura {
    CursusFabricae  cursus;
               b32  siccum;
               Xar* sanationes;    /* iudicum primae, deinde ceterae */
               i32  numeri[FABRICA_IUDICIUM + I];   /* omnes eventus:
                                     * VI erat - AUDITUM_DISCORS (index
                                     * VI) ultra finem scribebat (T7) */
               i32 duratio;
               i32 aestimatio;    /* siccum: ex cursibus ultimis */
               i32 sine_tempore;
               b32 commissa;      /* generata in arbore mutata */
} CursusSanandi;

/* vexilla sanandi. FALSUM = vexillum ignotum */
interior b32
_sanare_optiones (
    CursusSanandi*  m,
              s32   argc,
        character** argv)
{
    s32 a;

    per (a = II; a < argc; a++)
    {
        si (strcmp(argv[a], "-siccum") == 0)
        {
            m->siccum = VERUM;
        }
        alioquin si (strcmp(argv[a], "-audit") == 0)
        {
            /* AUDITUS TRANSITUS (spec 3 par. XIII): actio iudicium
             * RECENS tamen currit; defectus = AUDITUM DISCORS */
            auditus_sanandi = I;
        }
        alioquin si (strcmp(argv[a], "-machina") == 0)
        {
            suturae_machina = VERUM;   /* fabrica-6 H1: lineae TSV */
        }
        alioquin si (argv[a][0] == '-')
        {
            fprintf(stderr, "fabrica: vexillum ignotum %s\n", argv[a]);
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* iudex ipse PRIMUS (Review Focus 1): iudex stalus digesta stala
 * scriberet et sanaret - nihil agitur. FALSUM = bin/fabrica non
 * recens */
interior b32
_sanare_iudicem_ipsum (
    CursusSanandi* m)
{
    CursusFabricae* c = &m->cursus;
               i32  i;
               i32  j;

    per (i = ZEPHYRUM; i < xar_numerus(c->actiones); i++)
    {
        FabricaActio* actio;

        actio = (FabricaActio*)xar_obtinere(c->actiones, i);
        per (j = ZEPHYRUM; j < xar_numerus(actio->exitus); j++)
        {
            FabricaExitus* exitus;
          FabricaIudicium  iudicium;

            exitus = (FabricaExitus*)xar_obtinere(actio->exitus, j);
            si (!chorda_aequalis_literis(exitus->via, "bin/fabrica"))
            {
                perge;
            }
            iudicium = fabrica_iudicare(&c->sutura, actio, exitus,
                FALSUM, c->piscina);
            si (iudicium.status != FABRICA_RECENS)
            {
                fprintf(stderr,
                    "fabrica: iudex ipse non recens (%.*s) - "
                    "./tools/fabrica_struere.sh prius, deinde iterum "
                    "sanare\n", (s32)iudicium.causa.mensura,
                    (constans character*)iudicium.causa.datum);
                redde FALSUM;
            }
        }
    }
    redde VERUM;
}

/* sera capta, deinde memoria et ansae iudicii nexae, auditus.
 * FALSUM = sera tenetur */
interior b32
_sanare_seram_capere (
    CursusSanandi* m)
{
        CursusFabricae* c = &m->cursus;
    constans character* n;

    (vacuum)filum_directorium_creare_cum_parentibus(
        "build/fabrica/acta");
    c->sera = filum_seram_capere("build/fabrica/sera", ZEPHYRUM,
        c->piscina);
    si (c->sera == NIHIL)
    {
        fprintf(stderr, "fabrica: iudex plenus aut sanatio alia currit "
            "(build/fabrica/sera tenetur) - exspecta eam aut siste\n");
        redde FALSUM;
    }
    si (c->memoria_aperta)
    {
        suturae_memoriam_nectere(&c->sutura, &c->memoria);
    }
    c->sutura.radix = suturae_radix_absoluta(c->piscina);
    suturae_iudicii_parare(&c->sutura, c->actiones, c->piscina);
    n = getenv("FABRICA_AUDITUS");
    si (auditus_sanandi == ZEPHYRUM && n != NIHIL && n[0] != '\0')
    {
        auditus_sanandi = (i32)strtoul(n, NIHIL, X);
    }
    c->sutura.auditus  = auditus_sanandi;
    c->sutura.audita   = tabula_dispersa_creare_chorda(c->piscina, 16);
    redde VERUM;
}

/* STADIUM IUDICUM (fabrica-6 T3): iudices (iudex="verum") stali PRIMUM
 * sanantur, deinde cetera - nihil sub iudice stalo sanatur. bin/fabrica
 * ipse supra recusatur (se ipsum non restruit). FALSUM = fractura
 * (causa nuntiata); *sanationes_out NIHIL si nullus iudex stalus */
interior b32
_sanare_iudices (
    CursusSanandi*  m,
              Xar** sanationes_out)
{
    CursusFabricae* c = &m->cursus;
               Xar* viae_iudicum;
               i32  i;
               i32  j;

    *sanationes_out = NIHIL;
    si (m->siccum)
    {
        redde VERUM;
    }
    viae_iudicum = xar_creare(c->piscina, (i32)magnitudo(chorda));
    per (i = ZEPHYRUM; i < xar_numerus(c->actiones); i++)
    {
        FabricaActio* actio = (FabricaActio*)xar_obtinere(c->actiones,
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
                && fabrica_iudicare(&c->sutura, actio, exitus, FALSUM,
                       c->piscina).status != FABRICA_RECENS)
            {
                *(chorda*)xar_addere(viae_iudicum) = exitus->via;
            }
        }
    }
    si (xar_numerus(viae_iudicum) == ZEPHYRUM)
    {
        redde VERUM;
    }
    printf(suturae_machina
        ? "NOTA\tstadium iudicum primum (%d)\n"
        : "fabrica sanare: stadium iudicum primum (%d)\n",
        (integer)xar_numerus(viae_iudicum));
    fflush(stdout);
    *sanationes_out = fabrica_sanare(&c->sutura, c->ordo, viae_iudicum,
        FALSUM, c->piscina, &c->causa);
    si (*sanationes_out == NIHIL)
    {
        fprintf(stderr, "fabrica: stadium iudicum: %.*s\n",
            (s32)c->causa.mensura, (constans character*)c->causa.datum);
        redde FALSUM;
    }
    redde VERUM;
}

/* sanatio una: numeri (semel, utrique formae communes), deinde linea
 * machinae aut humana */
interior vacuum
_sanare_eventum (
              CursusSanandi* m,
    constans FabricaSanatio* sanatio)
{
    b32 agenda;
    i32 j;

    agenda = (   sanatio->eventus == FABRICA_AGENDUM
              || sanatio->eventus == FABRICA_FORTASSE);
    m->numeri[sanatio->eventus]++;
    m->duratio += sanatio->duratio_ms;
    si (agenda && sanatio->tempus_notum)
    {
        m->aestimatio += sanatio->duratio_ms;   /* ex cursu ultimo */
    }
    alioquin si (agenda)
    {
        m->sine_tempore++;
    }
    si (sanatio->eventus == FABRICA_SANATUM)
    {
        per (j = ZEPHYRUM; j < xar_numerus(sanatio->actio->exitus); j++)
        {
            si (((FabricaExitus*)xar_obtinere(sanatio->actio->exitus,
                    j))->strategia->octetis_comparat)
            {
                m->commissa = VERUM;
            }
        }
    }

    si (suturae_machina)
    {
        fputs("SANATIO", stdout);
        _campus_literis(suturae_eventus_titulus(sanatio->eventus));
        suturae_campus(sanatio->actio->titulus);
        _campus_numerus((longus)sanatio->duratio_ms);
        suturae_campus(sanatio->causa);
        putchar('\n');
        redde;
    }
    printf("%-11s %.*s", suturae_eventus_titulus(sanatio->eventus),
        (s32)sanatio->actio->titulus.mensura,
        (constans character*)sanatio->actio->titulus.datum);
    si (agenda && sanatio->tempus_notum)
    {
        /* aestimatio ex cursu ultimo (1b T7) */
        printf(" (~%u.%u s)",
            (insignatus integer)(sanatio->duratio_ms / M),
            (insignatus integer)((sanatio->duratio_ms % M) / C));
    }
    alioquin si (agenda)
    {
        printf(" (tempus ignotum)");
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
}

/* undae (Q41, siccum): quae SIMUL currere possent - monstratur solum */
interior vacuum
_sanare_undas (
    CursusSanandi* m)
{
    CursusFabricae* c = &m->cursus;
               Xar* agendae;
               Xar* undae;
               i32  i;
               i32  j;

    si (!m->siccum || xar_numerus(m->sanationes) == 0)
    {
        redde;
    }
    agendae = xar_creare(c->piscina, (i32)magnitudo(FabricaActio*));
    per (i = ZEPHYRUM; i < xar_numerus(m->sanationes); i++)
    {
        *(constans FabricaActio**)xar_addere(agendae) =
            ((FabricaSanatio*)xar_obtinere(m->sanationes, i))->actio;
    }
    undae = fabrica_undas_formare(agendae, c->piscina);
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
            (i64)(i + I), NIHIL, c->piscina), c->piscina));
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

/* linea summae (machina aut humana; siccum aut actum) */
interior vacuum
_sanare_summam (
    constans CursusSanandi* m)
{
    si (m->siccum && suturae_machina)
    {
        printf("SUMMA\t%u\t%u\t%u\t%u\tsiccum\n",
            (insignatus integer)m->numeri[FABRICA_AGENDUM],
            (insignatus integer)m->numeri[FABRICA_FORTASSE],
            (insignatus integer)m->aestimatio,
            (insignatus integer)m->sine_tempore);
    }
    alioquin si (suturae_machina)
    {
        printf("SUMMA\t%u\t%u\t%u\t%u\t%u\t%u\n",
            (insignatus integer)m->numeri[FABRICA_SANATUM],
            (insignatus integer)m->numeri[FABRICA_PRAEPARATUM],
            (insignatus integer)m->numeri[FABRICA_FRACTUM],
            (insignatus integer)m->numeri[FABRICA_OMISSUM],
            (insignatus integer)m->numeri[FABRICA_AUDITUM_DISCORS],
            (insignatus integer)m->duratio);
        si (m->commissa)
        {
            printf("NOTA\tplagulae generatae in arbore mutatae "
                "fortasse (git status) - committe per "
                "silva.commissio\n");
        }
    }
    alioquin si (m->siccum)
    {
        printf("fabrica sanare -siccum: %u agenda, %u fortasse - "
            "aestimatio ~%u s (%u sine tempore)\n",
            (insignatus integer)m->numeri[FABRICA_AGENDUM],
            (insignatus integer)m->numeri[FABRICA_FORTASSE],
            (insignatus integer)(m->aestimatio / M),
            (insignatus integer)m->sine_tempore);
    }
    alioquin
    {
        printf("fabrica sanare: %u sanata, %u praeparata, %u fracta, "
            "%u omissa%s (%u s)\n",
            (insignatus integer)m->numeri[FABRICA_SANATUM],
            (insignatus integer)m->numeri[FABRICA_PRAEPARATUM],
            (insignatus integer)m->numeri[FABRICA_FRACTUM],
            (insignatus integer)m->numeri[FABRICA_OMISSUM],
            m->numeri[FABRICA_AUDITUM_DISCORS]
                > 0 ? ", AUDITUM DISCORS" : "",
            (insignatus integer)(m->duratio / M));
        si (m->commissa)
        {
            printf("plagulae generatae in arbore mutatae fortasse "
                "(git status) - committe per silva.commissio; sanare "
                "numquam committit\n");
        }
    }
}

/* VERDICTUM compositorum nominatorum (fabrica-6 T10) ex sanatione
 * ipsa: pars recens nisi actio eius fracta, omissa, discors aut
 * (siccum) agenda - iudicium alterum non eget */
interior vacuum
_sanare_verdicta (
    CursusSanandi* m)
{
    CursusFabricae* c = &m->cursus;
               i32  i;
               i32  j;

    per (i = ZEPHYRUM; i < xar_numerus(c->nominata); i++)
    {
        FabricaCompositum* compositum;
                      Xar* partes;
                      Xar* iudicia;
                   chorda  verdictum;
                      i32  k;

        compositum = *(FabricaCompositum**)xar_obtinere(c->nominata, i);
        partes = _partes_compositi(c, compositum);
        iudicia = xar_creare(c->piscina,
            (i32)magnitudo(FabricaIudicium));
        per (j = ZEPHYRUM; partes != NIHIL && iudicia != NIHIL
             && j < xar_numerus(partes); j++)
        {
            FabricaIudicium* iudicium;
                     chorda  via;

            via = *(chorda*)xar_obtinere(partes, j);
            iudicium = (FabricaIudicium*)xar_addere(iudicia);
            iudicium->artificium = via;
            iudicium->status = FABRICA_RECENS;
            iudicium->causa = chorda_ex_literis("", c->piscina);
            per (k = ZEPHYRUM; k < xar_numerus(m->sanationes); k++)
            {
                FabricaSanatio* sanatio;
                           i32  e;

                sanatio = (FabricaSanatio*)xar_obtinere(m->sanationes,
                    k);
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
            compositum->titulus, c->piscina);
        printf("VERDICTUM %.*s\n", (s32)verdictum.mensura,
            (constans character*)verdictum.datum);
    }
}

interior s32
_sanare (
          s32   argc,
    character** argv,
      Piscina*  piscina)
{
    CursusSanandi  m;
              Xar* iudicum;
              Xar* ceterae;
              i32  i;
              s32  exitus;

    memset(&m, ZEPHYRUM, magnitudo(CursusSanandi));
    si (!_sanare_optiones(&m, argc, argv))
    {
        redde II;
    }
    si (!_cursum_parare(&m.cursus, piscina))
    {
        _cursum_finire(&m.cursus);
        redde II;
    }
    _electa_colligere(&m.cursus, argc, argv);

    suturae_legentem_parare(&m.cursus.sutura);
    suturae_currentem_addere(&m.cursus.sutura);
    suturae_agentem_addere(&m.cursus.sutura, m.cursus.actiones,
        m.siccum, piscina);
    m.cursus.sutura.sigilla         = m.cursus.sigilla;
    m.cursus.sutura.regenerationes  = tabula_dispersa_creare_chorda(
        piscina, 64);
    m.cursus.sutura.digesta = tabula_dispersa_creare_chorda(piscina,
        128);
    si (   !_sanare_iudicem_ipsum(&m)
        || !_sanare_seram_capere(&m))
    {
        _cursum_finire(&m.cursus);
        redde II;
    }

    printf(suturae_machina ? "NOTA\tsanare%s: iudicium plenum, ordine "
        "dependentiae\n" : "fabrica sanare%s: iudicium plenum, ordine "
        "dependentiae\n", m.siccum ? " -siccum" : "");
    fflush(stdout);
    si (!_sanare_iudices(&m, &iudicum))
    {
        _cursum_finire(&m.cursus);
        redde II;
    }
    ceterae = fabrica_sanare(&m.cursus.sutura, m.cursus.ordo,
        m.cursus.electa, m.siccum, piscina, &m.cursus.causa);
    si (ceterae == NIHIL)
    {
        fprintf(stderr, "fabrica: %.*s\n", (s32)m.cursus.causa.mensura,
            (constans character*)m.cursus.causa.datum);
        _cursum_finire(&m.cursus);
        redde II;
    }
    /* sanationes iudicum primae, deinde ceterae */
    m.sanationes = ceterae;
    si (iudicum != NIHIL)
    {
        m.sanationes = xar_creare(piscina,
            (i32)magnitudo(FabricaSanatio));
        per (i = ZEPHYRUM; i < xar_numerus(iudicum); i++)
        {
            *(FabricaSanatio*)xar_addere(m.sanationes) =
                *(FabricaSanatio*)xar_obtinere(iudicum, i);
        }
        per (i = ZEPHYRUM; i < xar_numerus(ceterae); i++)
        {
            *(FabricaSanatio*)xar_addere(m.sanationes) =
                *(FabricaSanatio*)xar_obtinere(ceterae, i);
        }
    }

    per (i = ZEPHYRUM; i < xar_numerus(m.sanationes); i++)
    {
        _sanare_eventum(&m,
            (FabricaSanatio*)xar_obtinere(m.sanationes, i));
    }
    _sanare_undas(&m);
    _sanare_summam(&m);
    _sanare_verdicta(&m);
    _cursum_finire(&m.cursus);
    si (m.siccum)
    {
        redde (m.numeri[FABRICA_AGENDUM] > 0) ? I : ZEPHYRUM;
    }
    exitus = (m.numeri[FABRICA_FRACTUM] + m.numeri[FABRICA_OMISSUM]
              + m.numeri[FABRICA_AUDITUM_DISCORS] > 0) ? I : ZEPHYRUM;
    redde exitus;
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
/* DEBITA SCRIPTURAE (fabrica-7 T3): linea 'debitum' per debitum
 * declaratum (actio, membrum, via, status cursus ultimi, causa, sedes),
 * deinde summa - numerus ad nihil ducendus. Declarationes solae et
 * areae membrorum leguntur (nulla explicatio, nulla clausura). */
interior vacuum
_debita_censere (
    Piscina* piscina)
{
    FabricaSutura  levis;
              Xar* actiones;
              Xar* composita;
              i32  numerus     = ZEPHYRUM;
              i32  actiones_n  = ZEPHYRUM;
              i32  stala       = ZEPHYRUM;
              i32  ignota      = ZEPHYRUM;
              i32  i;
              i32  j;

    actiones = _declarationes_colligere(piscina, &composita);
    si (actiones == NIHIL)
    {
        printf("census: debita nescita (declarationes non lectae)\n");
        redde;
    }
    suturae_legentem_parare(&levis);
    per (i = ZEPHYRUM; i < xar_numerus(actiones); i++)
    {
        constans FabricaActio* actio = (constans FabricaActio*)
            xar_obtinere(actiones, i);

        /* parentes soli: membrum copiam debitorum suorum fert */
        si (   actio->membrum != NIHIL || actio->debita == NIHIL
            || xar_numerus(actio->debita) == 0)
        {
            perge;
        }
        actiones_n++;
        per (j = ZEPHYRUM; j < xar_numerus(actio->debita); j++)
        {
            constans FabricaDebitum* d = (constans FabricaDebitum*)
                xar_obtinere(actio->debita, j);
                FabricaDebitiStatus status;

            status = fabrica_debitum_iudicare(&levis, actio, d,
                piscina);
            numerus++;
            stala   += status == FABRICA_DEBITUM_STALUM ? I : ZEPHYRUM;
            ignota  += status == FABRICA_DEBITUM_IGNOTUM ? I : ZEPHYRUM;
            fputs("debitum", stdout);
            suturae_campus(actio->titulus);
            suturae_campus(d->membrum);
            suturae_campus(d->via);
            _campus_literis(status == FABRICA_DEBITUM_SCRIPTUM
                ? "scriptum" : status == FABRICA_DEBITUM_STALUM
                ? "stalum" : "ignotum");
            suturae_campus(d->causa);
            suturae_campus(d->sedes);
            putchar('\n');
        }
    }
    printf("census: debita %d (actiones %d, stala %d, ignota %d)\n",
        (integer)numerus, (integer)actiones_n, (integer)stala,
        (integer)ignota);
}

interior s32
_censum (
    Piscina* piscina)
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
    _debita_censere(piscina);
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
        exitus = _censum(piscina);
    }
    alioquin
    {
        _usus();
        exitus = II;
    }
    piscina_destruere(piscina);
    redde exitus;
}
