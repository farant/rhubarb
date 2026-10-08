/* tools/fabrica_sutura.c - bin/fabrica: SUTURA VERA (fabrica-6 H4) -
 * plagulae per filum, directoria per iter_directoria, generatores per
 * processus_exsequi (FABRICA_SCRIPTURA vacuatum ante cursum),
 * relatio '-provenientia', agere (seriatim et simul), photographia
 * arboris; auxilia nectendi (suturae_legentem_parare et ceterae).
 * Vide tools/fabrica_sutura.h. */

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

#define MORA_GENERATORIS_MS 600000
#define MORA_RELATIONIS_MS  5000

/* mtime cum nanosecundis: nomen membri inter systemata differt
 * (stratum independentiae: hic solum, instrumentum non nucleus).
 * Bracchium Linux CUSTODITUR, non Apple (ut lib/vigilia.c): lexicon
 * silvae Darwin-veritas est, examen bracchium sine custodia aestimat */
#ifdef __linux__
#define FABRICA_MTIME(st) ((st).st_mtim)
#else
#define FABRICA_MTIME(st) ((st).st_mtimespec)
#endif


/* ==================================================
 * Sutura vera
 * ================================================== */

/* '~/X' -> $HOME/X (exitus extra arborem: copiae ~/.bin, 1b T5);
 * via nominata in declaratione '~/...' manet */
interior constans character*
_domum_expandere (
    constans character* via,
               Piscina* piscina)
{
    constans character* domus;
             character* plena;

    si (via[0] != '~' || via[1] != '/')
    {
        redde via;
    }
    domus = getenv("HOME");
    si (domus == NIHIL)
    {
        redde via;
    }
    plena = (character*)piscina_allocare(piscina,
        (memoriae_index)(strlen(domus) + strlen(via)));
    si (plena == NIHIL)
    {
        redde via;
    }
    strcpy(plena, domus);
    strcat(plena, via + I);
    redde plena;
}

interior b32
_legere (
                vacuum* datum,
    constans character* via,
               Piscina* piscina,
                chorda* contentum_out)
{
    FilumStatus status;

    (vacuum)datum;
    via = _domum_expandere(via, piscina);
    /* plagula REGULARIS sola: FIFO legere pendere faceret (spec 3
     * par. XII - vestigium portae species per _species iudicat) */
    si (!filum_status(via, &status) || !status.est_filum)
    {
        redde FALSUM;
    }
    /* plagula vacua = contentum vacuum, non absentia */
    *contentum_out = filum_legere_totum(via, piscina);
    redde VERUM;
}

/* ==== IUDICIUM (spec 3 par. XII, T5b): sutura portae ==== */

/* ambitus quem fabrica portae dat = ambitus processus huius */
interior b32
_ambitus (
                vacuum* datum,
    constans character* titulus,
               Piscina* piscina,
                chorda* valor_out)
{
    constans character* valor;

    (vacuum)datum;
    valor = getenv(titulus);
    si (valor == NIHIL)
    {
        redde FALSUM;
    }
    *valor_out = chorda_ex_literis(valor, piscina);
    redde VERUM;
}

interior i32
_species (
                vacuum* datum,
    constans character* via)
{
    FilumStatus status;

    (vacuum)datum;
    si (!filum_status(via, &status))
    {
        redde (i32)FABRICA_SPECIES_ABSENS;
    }
    si (status.est_directorium)
    {
        redde (i32)FABRICA_SPECIES_DIRECTORIUM;
    }
    redde status.est_filum ? (i32)FABRICA_SPECIES_PLAGULA
        : (i32)FABRICA_SPECIES_ALIA;
}

/* identitas clang ut bin/compilator (tools/compilator.c
 * _identitas_compilatoris): FABRICA_CLANG = octeti eius; aliter via
 * vera ('clang -print-prog-name=clang') cum mensura, mtime, inodo */
interior b32
_identitas (
     vacuum* datum,
    Piscina* piscina,
   Sigillum* identitas_out,
     chorda* causa_out)
{
       constans character* proprius;
       constans character* argv[III];
        ProcessusResultus  resultus;
                character  via[IV * MXXIV];
                character  memoria[IV * MXXIV + CXXVIII];
           memoriae_index  longitudo;
       structura stat    status;

    (vacuum)datum;
    proprius = getenv("FABRICA_CLANG");
    si (proprius != NIHIL && proprius[0] != '\0')
    {
        chorda octeti = filum_legere_totum(proprius, piscina);

        si (octeti.datum == NIHIL)
        {
            *causa_out = chorda_ex_literis("FABRICA_CLANG illegibile",
                piscina);
            redde FALSUM;
        }
        *identitas_out = sigillum_computare(octeti.datum,
            (memoriae_index)octeti.mensura);
        redde VERUM;
    }
    argv[0]   = "clang";
    argv[I]   = "-print-prog-name=clang";
    argv[II]  = NIHIL;
    resultus  = processus_exsequi(argv, XXX * M, piscina);
    si (   !resultus.successus || resultus.codex_exitus != ZEPHYRUM
        || resultus.effusio.mensura == ZEPHYRUM
        || resultus.effusio.mensura >= (i32)magnitudo(via))
    {
        *causa_out = chorda_ex_literis("clang -print-prog-name fractum",
            piscina);
        redde FALSUM;
    }
    longitudo = (memoriae_index)resultus.effusio.mensura;
    memcpy(via, resultus.effusio.datum, longitudo);
    dum (   longitudo > ZEPHYRUM
         && (via[longitudo - I] == '\n' || via[longitudo - I] == '\r'))
    {
        longitudo--;
    }
    via[longitudo] = '\0';
    si (stat(via, &status) != ZEPHYRUM)
    {
        *causa_out = chorda_concatenare(chorda_ex_literis(
            "clang verum absens: ", piscina), chorda_ex_literis(via,
            piscina), piscina);
        redde FALSUM;
    }
    sprintf(memoria, "compilator identitas I\n%s\n%ld %ld %lu", via,
        (longus)status.st_size, (longus)status.st_mtime,
        (longus insignatus)status.st_ino);
    *identitas_out = sigillum_computare(memoria, strlen(memoria));
    redde VERUM;
}

/* crusta/effectus.sh -clavis <via> (effectus-plan T7) */
interior b32
_effectus (
                vacuum* datum,
    constans character* via,
               Piscina* piscina,
                chorda* effusio_out,
                   i32* codex_out)
{
     constans character* argv[VI];
     constans character* instrumentum;
      ProcessusResultus  resultus;
              character  radix[IV * MXXIV];

    (vacuum)datum;
    /* FABRICA_EFFECTUS: via instrumenti (radices temporariae,
     * iudicium-fumus); '-radix' = directorium operis - scripta in
     * arbore IUDICATA resolvuntur (tabula et regulae ex arbore
     * instrumenti, crusta/effectus.sh) */
    instrumentum = getenv("FABRICA_EFFECTUS");
    si (instrumentum == NIHIL || instrumentum[0] == '\0')
    {
        instrumentum = "./crusta/effectus.sh";
    }
    si (getcwd(radix, magnitudo(radix)) == NIHIL)
    {
        redde FALSUM;
    }
    argv[0]    = instrumentum;
    argv[I]    = "-clavis";
    argv[II]   = via;
    argv[III]  = "-radix";
    argv[IV]   = radix;
    argv[V]    = NIHIL;
    resultus   = processus_exsequi(argv, CXX * M, piscina);
    si (!resultus.successus)
    {
        redde FALSUM;
    }
    *effusio_out  = resultus.effusio;
    *codex_out    = (i32)resultus.codex_exitus;
    redde VERUM;
}

/* exitus omnium actionum: dominus lectionis sub build/ */
interior TabulaDispersa*
_exitus_notos_colligere (
    constans Xar* actiones,
         Piscina* piscina)
{
    TabulaDispersa* noti;
               i32  i;
               i32  j;

    noti = tabula_dispersa_creare_chorda(piscina, 512);
    per (i = ZEPHYRUM; noti != NIHIL && i < xar_numerus(actiones); i++)
    {
        constans FabricaActio* actio;

        actio = (constans FabricaActio*)xar_obtinere(actiones, i);
        per (j = ZEPHYRUM; j < xar_numerus(actio->exitus); j++)
        {
            (vacuum)tabula_dispersa_inserere(noti,
                ((FabricaExitus*)xar_obtinere(actio->exitus, j))->via,
                NIHIL);
        }
    }
    redde noti;
}

/* REPOSITORIUM (fabrica-6 T2): sha HEAD per lib/git (bibliotheca, non
 * processus 'git') - radix = directorium currens */
interior b32
_repositorium (
                vacuum* datum,
    constans character* clavis,
               Piscina* piscina,
                chorda* valor_out)
{
    GitRepositorium* repositorium;
          character  sha[GIT_SHA_HEX_MENSURA];

    (vacuum)datum;
    si (strcmp(clavis, "commissum") != ZEPHYRUM)
    {
        redde FALSUM;
    }
    repositorium = git_aperire(piscina, ".");
    si (   repositorium == NIHIL
        || !git_ref_resolvere(repositorium, "HEAD", sha))
    {
        redde FALSUM;
    }
    *valor_out = chorda_transcribere(chorda_ex_literis(sha, piscina),
        piscina);
    redde VERUM;
}

vacuum
suturae_iudicii_parare (
    FabricaSutura* sutura,
     constans Xar* actiones,
          Piscina* piscina)
{
    sutura->ambitus    = _ambitus;
    sutura->species    = _species;
    sutura->identitas  = _identitas;

    sutura->effectus      = _effectus;
    sutura->repositorium  = _repositorium;
    sutura->exitus_noti   = _exitus_notos_colligere(actiones, piscina);
    /* GRADUS (fabrica-6 T6c-2): membra agi possunt */
    sutura->area_parare      = suturae_area_parare;
    sutura->in_area_currere  = suturae_in_area_currere;
    sutura->clausura_c       = suturae_clausura_c;
    sutura->compilare        = suturae_compilare_gradus;
}

interior s32
_chordas_comparare (
    constans vacuum* a,
    constans vacuum* b)
{
    redde chorda_comparare(*(constans chorda*)a, *(constans chorda*)b);
}

interior b32
_enumerare (
                vacuum*  datum,
    constans character*  via,
               Piscina*  piscina,
                   Xar** nomina_out)
{
     DirectoriumIterator* iterator;
    DirectoriumIntroitus* introitus;
                     Xar* nomina;

    (vacuum)datum;
    iterator = directorium_iterator_aperire(via, piscina);
    si (iterator == NIHIL)
    {
        redde FALSUM;
    }
    nomina = xar_creare(piscina, (i32)magnitudo(chorda));
    dum ((introitus = directorium_iterator_proximum(iterator)) != NIHIL)
    {
        chorda* locus;

        si (   chorda_aequalis_literis(introitus->titulus, ".")
            || chorda_aequalis_literis(introitus->titulus, ".."))
        {
            perge;
        }
        locus = (chorda*)xar_addere(nomina);
        si (locus != NIHIL)
        {
            *locus = chorda_transcribere(introitus->titulus, piscina);
        }
    }
    directorium_iterator_claudere(iterator);
    xar_ordinare(nomina, _chordas_comparare);
    *nomina_out = nomina;
    redde VERUM;
}

/* ultima linea non vacua textus (causa brevis) */
interior chorda
_ultima_linea (
    chorda textus)
{
    i32 finis;
    i32 initium;

    finis = textus.mensura;
    dum (   finis > 0 && (textus.datum[finis - 1] == '\n'
                       || textus.datum[finis - 1] == ' '))
    {
        finis--;
    }
    initium = finis;
    dum (initium > 0 && textus.datum[initium - 1] != '\n')
    {
        initium--;
    }
    redde chorda_sectio(textus, initium, finis);
}

interior b32
_currere (
                vacuum* datum,
          constans Xar* mandatum,
    constans character* scriptura_dir,
    constans character* liber_via,
               Piscina* piscina,
                chorda* causa_out,
                   i32* duratio_ms_out)
{
       constans character** argumenta;
        ProcessusResultus   resultus;
                      i32   numerus;
                      i32   i;
                character   exitus[64];

    (vacuum)datum;
    numerus = xar_numerus(mandatum);
    si (numerus == 0)
    {
        *causa_out = chorda_ex_literis("mandatum vacuum", piscina);
        redde FALSUM;
    }
    /* contractus suturae: directorium scripturae VACUUM */
    si (   !filum_arborem_delere(scriptura_dir)
        || !filum_directorium_creare_cum_parentibus(scriptura_dir))
    {
        *causa_out = chorda_ex_literis(
            "directorium scripturae parari non potest", piscina);
        redde FALSUM;
    }
    argumenta = (constans character**)piscina_allocare(piscina,
        magnitudo(character*) * (memoriae_index)(numerus + 1));
    si (argumenta == NIHIL)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        argumenta[i] = chorda_ut_cstr(
            *(chorda*)xar_obtinere(mandatum, i), piscina);
    }
    argumenta[numerus] = NIHIL;

    /* via ABSOLUTA: scripta directorium mutant (silva/amalgamare.sh
     * 'cd silva') - via relativa ibi alibi caderet */
    si (scriptura_dir[0] == '/')
    {
        (vacuum)setenv("FABRICA_SCRIPTURA", scriptura_dir, 1);
    }
    alioquin
    {
        character sedes[4096];

        si (   getcwd(sedes, magnitudo(sedes)) == NIHIL
            || strlen(sedes) + strlen(scriptura_dir) + 2
                > magnitudo(sedes))
        {
            *causa_out = chorda_ex_literis("getcwd fractum", piscina);
            redde FALSUM;
        }
        strcat(sedes, "/");
        strcat(sedes, scriptura_dir);
        (vacuum)setenv("FABRICA_SCRIPTURA", sedes, 1);
    }
    /* liber lectionum (plan 2 T2): via ABSOLUTA (scripta 'cd'),
     * plagula vetus deleta - liber cursus HUIUS solum */
    si (liber_via != NIHIL)
    {
        character liber[4096];

        si (   getcwd(liber, magnitudo(liber)) == NIHIL
            || strlen(liber) + strlen(liber_via) + 2 > magnitudo(liber))
        {
            *causa_out = chorda_ex_literis("getcwd fractum", piscina);
            redde FALSUM;
        }
        strcat(liber, "/");
        strcat(liber, liber_via);
        (vacuum)filum_directorium_creare_cum_parentibus(
            "build/fabrica/lectiones");
        (vacuum)remove(liber);
        (vacuum)setenv("FABRICA_LECTIONES", liber, 1);
    }
    resultus = processus_exsequi(
        (constans character* constans*)argumenta, MORA_GENERATORIS_MS,
        piscina);
    (vacuum)unsetenv("FABRICA_SCRIPTURA");
    (vacuum)unsetenv("FABRICA_LECTIONES");
    *duratio_ms_out = resultus.mora_ms;

    si (!resultus.successus)
    {
        *causa_out = resultus.error_descriptio;
        redde FALSUM;
    }
    si (resultus.codex_exitus != 0)
    {
        sprintf(exitus, "exitus %u: ",
            (insignatus integer)resultus.codex_exitus);
        *causa_out = chorda_concatenare(
            chorda_ex_literis(exitus, piscina),
            _ultima_linea(resultus.erratum.mensura > 0
                ? resultus.erratum : resultus.effusio),
            piscina);
        redde FALSUM;
    }
    redde VERUM;
}

interior b32
_rogare (
                vacuum* datum,
    constans character* via,
               Piscina* piscina,
                chorda* relatio_out)
{
     constans character* argumenta[III];
      ProcessusResultus  resultus;

    (vacuum)datum;
    si (!filum_existit(via))
    {
        redde FALSUM;
    }
    argumenta[0] = via;
    argumenta[1] = "-provenientia";
    argumenta[2] = NIHIL;
    resultus = processus_exsequi(argumenta, MORA_RELATIONIS_MS,
        piscina);
    si (   !resultus.successus
        || resultus.codex_exitus    != 0
        || resultus.effusio.mensura == 0)
    {
        redde FALSUM;
    }
    *relatio_out = resultus.effusio;
    redde VERUM;
}

/* ultimae 'numerus' lineae textus (cauda acti) */
chorda
suturae_ultimae_lineae (
    chorda textus,
       i32 numerus)
{
    i32 finis;
    i32 initium;
    i32 lineae;

    finis = textus.mensura;
    dum (finis > 0 && textus.datum[finis - 1] == '\n')
    {
        finis--;
    }
    initium  = finis;
    lineae   = ZEPHYRUM;
    dum (initium > 0)
    {
        si (textus.datum[initium - 1] == '\n')
        {
            lineae++;
            si (lineae >= numerus)
            {
                frange;
            }
        }
        initium--;
    }
    redde chorda_sectio(textus, initium, finis);
}

/* argumenta actionis et caput actorum ('$ verba'); 'agitur' impressum.
 * NIHIL (cauda posita) si mandatum vacuum. */
interior constans character**
_mandatum_parare (
    constans FabricaActio*  actio,
                  Piscina*  piscina,
        ChordaAedificator** acta_out,
             FabricaActum*  actum_out)
{
       constans character** argumenta;
        ChordaAedificator*  acta;
                      i32   numerus;
                      i32   i;

    actum_out->codex       = -I;
    actum_out->duratio_ms  = ZEPHYRUM;
    actum_out->cauda       = chorda_ex_literis("", piscina);
    numerus                = xar_numerus(actio->mandatum);
    si (numerus == 0)
    {
        actum_out->cauda = chorda_ex_literis(
            "mandatum vacuum (sanatio non declarata)", piscina);
        redde NIHIL;
    }
    argumenta = (constans character**)piscina_allocare(piscina,
        magnitudo(character*) * (memoriae_index)(numerus + 1));
    acta = chorda_aedificator_creare(piscina, 4096);
    si (argumenta == NIHIL || acta == NIHIL)
    {
        redde NIHIL;
    }
    (vacuum)chorda_aedificator_appendere_literis(acta, "$");
    si (suturae_machina)
    {
        fputs("AGITUR", stdout);
        suturae_campus(actio->titulus);
        putchar('\t');
    }
    alioquin
    {
        printf("  agitur %.*s:", (s32)actio->titulus.mensura,
            (constans character*)actio->titulus.datum);
    }
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        chorda verbum;

        verbum        = *(chorda*)xar_obtinere(actio->mandatum, i);
        argumenta[i]  = chorda_ut_cstr(verbum, piscina);
        printf(suturae_machina
            && i == ZEPHYRUM ? "%s" : " %s", argumenta[i]);
        (vacuum)chorda_aedificator_appendere_literis(acta, " ");
        (vacuum)chorda_aedificator_appendere_chorda(acta, verbum);
    }
    argumenta[numerus] = NIHIL;
    printf("\n");
    fflush(stdout);
    *acta_out = acta;
    redde argumenta;
}

/* resultus -> acta (verba, effusio, erratum separatim) et actum.
 * FALSUM si processus non cucurrit (cauda = descriptio erroris). */
b32
suturae_actum_complere (
       ProcessusResultus  resultus,
       ChordaAedificator* acta,
      constans character* acta_via,
                 Piscina* piscina,
            FabricaActum* actum_out)
{
    (vacuum)piscina;
    (vacuum)chorda_aedificator_appendere_literis(acta,
        "\n== effusio ==\n");
    (vacuum)chorda_aedificator_appendere_chorda(acta, resultus.effusio);
    (vacuum)chorda_aedificator_appendere_literis(acta,
        "\n== erratum ==\n");
    (vacuum)chorda_aedificator_appendere_chorda(acta, resultus.erratum);
    (vacuum)chorda_aedificator_appendere_literis(acta, "\n");
    (vacuum)filum_scribere(acta_via, chorda_aedificator_finire(acta));

    actum_out->duratio_ms = resultus.mora_ms;
    si (!resultus.successus)
    {
        actum_out->cauda = resultus.error_descriptio;
        redde FALSUM;
    }
    actum_out->codex = (resultus.signum != 0)
        ? (s32)(CXXVIII + resultus.signum)
        : (s32)resultus.codex_exitus;
    actum_out->cauda = suturae_ultimae_lineae(resultus.erratum.mensura
        > 0
        ? resultus.erratum : resultus.effusio, XX);
    redde VERUM;
}

/* agere (plan 1b T3): mandatum IN LOCO - FABRICA_SCRIPTURA expresse
 * deleta, generatores in arbore scribunt. Acta = verba, effusio,
 * erratum (processus ea separatim capit: non intertexta). */
interior b32
_agere (
                   vacuum* datum,
    constans FabricaActio* actio,
       constans character* acta_via,
                  Piscina* piscina,
             FabricaActum* actum_out)
{
       constans character** argumenta;
        ProcessusResultus   resultus;
        ChordaAedificator*  acta;

    (vacuum)datum;
    argumenta = _mandatum_parare(actio, piscina, &acta, actum_out);
    si (argumenta == NIHIL)
    {
        redde FALSUM;
    }
    (vacuum)unsetenv("FABRICA_SCRIPTURA");
    /* IUDICIUM (spec 3 par. XII): porta in loco cum libro lectionum
     * suo - via ABSOLUTA (cursores 'cd' faciunt), liber vacuatus ante
     * cursum (vestigium cursus HUIUS solius) */
    si (actio->genus == FABRICA_ACTIO_IUDICIUM)
    {
        chorda relativa;
        chorda absoluta;
        chorda radix;

        relativa  = fabrica_liber_via(actio->titulus, piscina);
        radix     = suturae_radix_absoluta(piscina);
        absoluta = chorda_concatenare(chorda_concatenare(radix,
            chorda_ex_literis("/", piscina), piscina), relativa,
            piscina);
        (vacuum)filum_directorium_creare_cum_parentibus(
            "build/fabrica/lectiones");
        /* liber vetus DELETUR (ut generatores): filum_scribere chordae
         * vacuae (datum NIHIL) nihil agit - liber per cursus crescebat
         * (LXV MB, inventum T6) et vestigium unionem cursorum omnium
         * ferebat, lectiones stalas comprehendens */
        (vacuum)filum_delere(chorda_ut_cstr(relativa, piscina));
        (vacuum)setenv("FABRICA_LECTIONES",
            chorda_ut_cstr(absoluta, piscina), 1);
    }
    /* FABRICA_AGIT (1b T5, D3/D4): sub executore scripta productores
     * NIDIFICATOS omittunt (portae vetustatis, aedilis restructus) -
     * fabrica eos ut actiones suas ordine iam currit; manu vocata
     * scripta commoditates suas servant */
    (vacuum)setenv("FABRICA_AGIT", "1", 1);
    resultus = processus_exsequi(
        (constans character* constans*)argumenta, MORA_SANATIONIS_MS,
        piscina);
    (vacuum)unsetenv("FABRICA_AGIT");
    (vacuum)unsetenv("FABRICA_LECTIONES");
    redde suturae_actum_complere(resultus, acta, acta_via, piscina,
        actum_out);
}

/* FILA (plan 2 T6): actiones tutae simul. FABRICA_FILA=N aut nuclei
 * efficientiae (sysctl hw.perflevel0.physicalcpu); I = seriatim (via
 * vetus, agere_simul NIHIL). */
i32 suturae_fila_maxima = I;

i32
suturae_fila_computare (
    Piscina* piscina)
{
    constans character* textus;
    constans character* argv[IV];
     ProcessusResultus  resultus;
                   s32  n;

    textus = getenv("FABRICA_FILA");
    si (textus != NIHIL && textus[0] != '\0')
    {
        n = (s32)atoi(textus);
        redde (n < I) ? I : (n > LXIV ? LXIV : (i32)n);
    }
    argv[0]    = "/usr/sbin/sysctl";
    argv[I]    = "-n";
    argv[II]   = "hw.perflevel0.physicalcpu";
    argv[III]  = NIHIL;
    resultus   = processus_exsequi(argv, V * M, piscina);
    si (   resultus.successus && resultus.codex_exitus == ZEPHYRUM
        && resultus.effusio.mensura > ZEPHYRUM)
    {
        n = (s32)atoi(chorda_ut_cstr(resultus.effusio, piscina));
        si (n >= I)
        {
            redde (n > LXIV) ? LXIV : (i32)n;
        }
    }
    redde IV;
}

/* agere SIMUL (plan 2 T6): usque ad suturae_fila_maxima filii simul per
 * processus_incipere/pulsare/metere; FABRICA_LECTIONES per filium
 * (via absoluta, liber vetus deletus) ANTE incipere positus. Fractura:
 * currentes finiunt, nova non incipiunt (incepta FALSUM, cauda vacua =
 * consulto omissum). */
interior vacuum
_agere_simul (
                            vacuum*  datum,
    constans FabricaActio* constans* actiones,
                               i32  numerus,
       constans character* constans* acta_viae,
       constans character* constans* libri,
                           Piscina*  piscina,
                      FabricaActum*  acta_out,
                               b32*  incepta_out)
{
               Processus** processus;
       ChordaAedificator** acta;
               character   radix[IV * MXXIV];
                     i32   proximus;
                     i32   currentes;
                     i32   k;
                     b32   fractum;

    (vacuum)datum;
    processus = (Processus**)piscina_allocare(piscina,
        magnitudo(Processus*) * (memoriae_index)numerus);
    acta = (ChordaAedificator**)piscina_allocare(piscina,
        magnitudo(ChordaAedificator*) * (memoriae_index)numerus);
    si (   processus                       == NIHIL || acta == NIHIL
        || getcwd(radix, magnitudo(radix)) == NIHIL)
    {
        redde;
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        processus[k]            = NIHIL;
        acta[k]                 = NIHIL;
        incepta_out[k]          = FALSUM;
        acta_out[k].codex       = -I;
        acta_out[k].duratio_ms  = ZEPHYRUM;
        acta_out[k].cauda       = chorda_ex_literis("", piscina);
    }
    (vacuum)filum_directorium_creare_cum_parentibus(
        "build/fabrica/lectiones");
    proximus   = ZEPHYRUM;
    currentes  = ZEPHYRUM;
    fractum    = FALSUM;
    dum (VERUM)
    {
        b32 mutatum;

        dum (   !fractum && proximus < numerus
             && currentes < suturae_fila_maxima)
        {
              constans character** argumenta;
                       character   liber[IV * MXXIV];

            k = proximus++;
            argumenta = _mandatum_parare(actiones[k], piscina, &acta[k],
                &acta_out[k]);
            si (argumenta == NIHIL)
            {
                fractum = VERUM;
                perge;
            }
            sprintf(liber, "%s/%s", radix, libri[k]);
            (vacuum)filum_delere(liber);
            (vacuum)unsetenv("FABRICA_SCRIPTURA");
            (vacuum)setenv("FABRICA_AGIT", "1", 1);
            (vacuum)setenv("FABRICA_LECTIONES", liber, 1);
            processus[k] = processus_incipere(
                (constans character* constans*)argumenta,
                MORA_SANATIONIS_MS, piscina);
            (vacuum)unsetenv("FABRICA_LECTIONES");
            (vacuum)unsetenv("FABRICA_AGIT");
            si (processus[k] == NIHIL)
            {
                acta_out[k].cauda = chorda_ex_literis(
                    "processus incipi nequit", piscina);
                fractum = VERUM;
                perge;
            }
            incepta_out[k] = VERUM;
            currentes++;
        }
        si (currentes == ZEPHYRUM)
        {
            frange;
        }
        mutatum = FALSUM;
        per (k = ZEPHYRUM; k < numerus; k++)
        {
            ProcessusResultus resultus;

            si (   processus[k]                    == NIHIL
                || processus_pulsare(processus[k]) != PROCESSUS_PARATUS)
            {
                perge;
            }
            resultus = processus_metere(processus[k]);
            si (!suturae_actum_complere(resultus, acta[k], acta_viae[k],
                piscina,
                    &acta_out[k]))
            {
                incepta_out[k]  = FALSUM;
                fractum         = VERUM;
            }
            alioquin si (acta_out[k].codex != ZEPHYRUM)
            {
                fractum = VERUM;
            }
            processus[k] = NIHIL;
            currentes--;
            mutatum = VERUM;
        }
        si (!mutatum)
        {
            (vacuum)usleep(XX * M);   /* XX ms: pulsus sine obstaculo */
        }
    }
}

/* currere SIMUL (T6b, praevisio iudicii): regenerationes usque ad
 * suturae_fila_maxima simul, quaeque in scripturam suam VACUAM
 * (FABRICA_SCRIPTURA absoluta) cum libro suo (FABRICA_LECTIONES
 * absoluta, vetus deletus), ANTE incipere positis. Causa ut _currere.
 * Iudicium nihil sistit: omnes currunt. */
vacuum
suturae_currere_simul (
                            vacuum*  datum,
    constans FabricaActio* constans* actiones,
                               i32  numerus,
       constans character* constans* scripturae,
       constans character* constans* libri,
                           Piscina*  piscina,
                               b32*  felices_out,
                            chorda*  causae_out,
                               i32*  durationes_out)
{
     Processus** processus;
     character   radix[IV * MXXIV];
           i32   proximus;
           i32   currentes;
           i32   k;

    (vacuum)datum;
    processus = (Processus**)piscina_allocare(piscina,
        magnitudo(Processus*) * (memoriae_index)numerus);
    si (processus == NIHIL || getcwd(radix, magnitudo(radix)) == NIHIL)
    {
        redde;
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        processus[k]       = NIHIL;
        felices_out[k]     = FALSUM;
        causae_out[k]      = chorda_ex_literis("", piscina);
        durationes_out[k]  = ZEPHYRUM;
    }
    (vacuum)filum_directorium_creare_cum_parentibus(
        "build/fabrica/lectiones");
    proximus   = ZEPHYRUM;
    currentes  = ZEPHYRUM;
    dum (VERUM)
    {
        b32 mutatum;

        dum (proximus < numerus && currentes < suturae_fila_maxima)
        {
              constans character** argumenta;
                       character   scriptura[IV * MXXIV];
                       character   liber[IV * MXXIV];
                             i32   n;
                             i32   i;

            k = proximus++;
            n = xar_numerus(actiones[k]->mandatum);
            argumenta = (constans character**)piscina_allocare(piscina,
                magnitudo(character*) * (memoriae_index)(n + 1));
            si (n == 0 || argumenta == NIHIL)
            {
                causae_out[k] = chorda_ex_literis("mandatum vacuum",
                    piscina);
                perge;
            }
            per (i = ZEPHYRUM; i < n; i++)
            {
                argumenta[i] = chorda_ut_cstr(*(chorda*)xar_obtinere(
                    actiones[k]->mandatum, i), piscina);
            }
            argumenta[n] = NIHIL;
            /* contractus suturae: directorium scripturae VACUUM */
            si (   !filum_arborem_delere(scripturae[k])
                || !filum_directorium_creare_cum_parentibus(scripturae[k]))
            {
                causae_out[k] = chorda_ex_literis(
                    "directorium scripturae parari non potest",
                    piscina);
                perge;
            }
            sprintf(scriptura, "%s/%s", radix, scripturae[k]);
            sprintf(liber, "%s/%s", radix, libri[k]);
            (vacuum)remove(liber);
            (vacuum)setenv("FABRICA_SCRIPTURA", scriptura, 1);
            (vacuum)setenv("FABRICA_LECTIONES", liber, 1);
            processus[k] = processus_incipere(
                (constans character* constans*)argumenta,
                MORA_GENERATORIS_MS, piscina);
            (vacuum)unsetenv("FABRICA_SCRIPTURA");
            (vacuum)unsetenv("FABRICA_LECTIONES");
            si (processus[k] == NIHIL)
            {
                causae_out[k] = chorda_ex_literis(
                    "processus incipi nequit", piscina);
                perge;
            }
            currentes++;
        }
        si (currentes == ZEPHYRUM)
        {
            frange;
        }
        mutatum = FALSUM;
        per (k = ZEPHYRUM; k < numerus; k++)
        {
            ProcessusResultus resultus;

            si (   processus[k]                    == NIHIL
                || processus_pulsare(processus[k]) != PROCESSUS_PARATUS)
            {
                perge;
            }
            resultus           = processus_metere(processus[k]);
            durationes_out[k]  = resultus.mora_ms;
            si (!resultus.successus)
            {
                causae_out[k] = resultus.error_descriptio;
            }
            alioquin si (resultus.codex_exitus != 0)
            {
                character exitus[LXIV];

                sprintf(exitus, "exitus %u: ",
                    (insignatus integer)resultus.codex_exitus);
                causae_out[k] = chorda_concatenare(
                    chorda_ex_literis(exitus, piscina),
                    _ultima_linea(resultus.erratum.mensura > 0
                        ? resultus.erratum : resultus.effusio),
                        piscina);
            }
            alioquin
            {
                felices_out[k] = VERUM;
            }
            processus[k] = NIHIL;
            currentes--;
            mutatum = VERUM;
        }
        si (!mutatum)
        {
            (vacuum)usleep(XX * M);
        }
    }
}

/* SUTURA LEGENS (fabrica-6 H4): sutura vacua cum disco vero legendo et
 * enumerando - basis omnium mandatorum */
vacuum
suturae_legentem_parare (
    FabricaSutura* sutura)
{
    fabrica_suturam_parare(sutura);
    sutura->legere     = _legere;
    sutura->enumerare  = _enumerare;
}

/* processus veri: generatores currere, relationes rogare */
vacuum
suturae_currentem_addere (
    FabricaSutura* sutura)
{
    sutura->currere  = _currere;
    sutura->rogare   = _rogare;
}


/* ==================================================
 * Photographia arboris (plan 1b T4): vestigium_capere
 * ================================================== */

/* directoria '~/...' quae declarationes nominant (exitus aut
 * vestigia extra arborem) - suturae_agentem_addere ea ponit ante
 * photographias */
hic_manens Xar* _directoria_extra = NIHIL;

interior s32
_vestigia_ordinare (
    constans vacuum* a,
    constans vacuum* b)
{
    redde chorda_comparare(((constans FabricaVestigium*)a)->via,
        ((constans FabricaVestigium*)b)->via);
}

interior vacuum
_vestigium_addere (
                   Xar* vestigia,
                chorda  via_nominata,
    constans character* via_vera)
{
      structura stat  status;
  structura timespec  tempus;
    FabricaVestigium* v;

    si (lstat(via_vera, &status) != 0)
    {
        redde;
    }
    v = (FabricaVestigium*)xar_addere(vestigia);
    si (v == NIHIL)
    {
        redde;
    }
    tempus = FABRICA_MTIME(status);
    v->via = via_nominata;
    v->tempus_ns  = (s64)tempus.tv_sec * (s64)M * (s64)M * (s64)M
        + (s64)tempus.tv_nsec;
    v->mensura    = (s64)status.st_size;
}

/* directorium VERUM (lstat: nexus symbolicus modum suum, non
 * destinationis, refert) */
interior b32
_directorium_verum_est (
    constans character* via)
{
    structura stat status;

    redde lstat(via, &status) == 0 && S_ISDIR(status.st_mode);
}

/* arbor recursive (nexus symbolici non sequuntur; .git radicis
 * praetermittitur) */
interior vacuum
_arborem_photographare (
      chorda  directorium,
         Xar* vestigia,
     Piscina* piscina)
{
     DirectoriumIterator* iterator;
    DirectoriumIntroitus* introitus;

    iterator = directorium_iterator_aperire(directorium.mensura > 0
        ? chorda_ut_cstr(directorium, piscina) : ".", piscina);
    si (iterator == NIHIL)
    {
        redde;
    }
    dum ((introitus = directorium_iterator_proximum(iterator)) != NIHIL)
    {
        chorda via;

        si (   chorda_aequalis_literis(introitus->titulus, ".")
            || chorda_aequalis_literis(introitus->titulus, "..")
            || (   directorium.mensura == 0
                && chorda_aequalis_literis(introitus->titulus, ".git")))
        {
            perge;
        }
        via = (directorium.mensura > 0)
            ? chorda_concatenare(chorda_concatenare(directorium,
                  chorda_ex_literis("/", piscina), piscina),
                  introitus->titulus, piscina)
            : chorda_transcribere(introitus->titulus, piscina);
        /* LOCI RESIDENTIUM: gesta/annales (entia .md, acta .jsonl) et in
         * radice tabularium.db* / forum.db* (proiectiones sqlite, -wal,
         * -shm) - residentes tabularii et fori eas QUOVIS tempore
         * scribunt, numquam artificium aedificationis
         * (nulla actio ibi exitum declarat). Photographiae eas
         * praetermittunt: alias nota in tabulario dum porta XXXV-L s
         * currit actionem FRACTAM faceret ('scripsit extra vestigium',
         * fabrica spec 3 T9, mensuratum). Limes: actio quae ibi errans
         * scriberet non capitur. */
        si (   chorda_aequalis_literis(via, "gesta/annales")
            || (   directorium.mensura == 0
                && (   chorda_incipit(via, chorda_ex_literis(
                           "tabularium.db", piscina))
                    || chorda_incipit(via, chorda_ex_literis("forum.db",
                           piscina)))))
        {
            perge;   /* et proiectiones residentium (sqlite, -wal, -shm) */
        }
        /* lstat: nexus symbolicus ad directorium NON sequitur (iterator
         * eum ut directorium refert) - radices umbrae scripturae (1b
         * T6) arborem veram per nexus ostendunt; secuta, scriptura vera
         * bis appareret, sub via umbrae extra omne vestigium */
        si (   introitus->genus == INTROITUS_DIRECTORIUM
            && _directorium_verum_est(chorda_ut_cstr(via, piscina)))
        {
            _arborem_photographare(via, vestigia, piscina);
        }
        alioquin
        {
            _vestigium_addere(vestigia, via, chorda_ut_cstr(via,
                piscina));
        }
    }
    directorium_iterator_claudere(iterator);
}

/* directorium '~/X' (HOME expanditur; via nominata manet '~/X/...') */
interior vacuum
_directorium_extra_photographare (
      chorda  nominatum,
         Xar* vestigia,
     Piscina* piscina)
{
     DirectoriumIterator* iterator;
    DirectoriumIntroitus* introitus;
      constans character* domus;
                  chorda  verum;

    domus = getenv("HOME");
    si (domus == NIHIL)
    {
        redde;
    }
    verum = chorda_concatenare(chorda_ex_literis(domus, piscina),
        chorda_sectio(nominatum, I, nominatum.mensura), piscina);
    iterator = directorium_iterator_aperire(chorda_ut_cstr(verum,
        piscina), piscina);
    si (iterator == NIHIL)
    {
        redde;
    }
    dum ((introitus = directorium_iterator_proximum(iterator)) != NIHIL)
    {
        chorda pars;

        si (   chorda_aequalis_literis(introitus->titulus, ".")
            || chorda_aequalis_literis(introitus->titulus, ".."))
        {
            perge;
        }
        pars = chorda_concatenare(chorda_ex_literis("/", piscina),
            introitus->titulus, piscina);
        _vestigium_addere(vestigia,
            chorda_concatenare(nominatum, pars, piscina),
            chorda_ut_cstr(chorda_concatenare(verum, pars,
            piscina),
                piscina));
    }
    directorium_iterator_claudere(iterator);
}

interior b32
_vestigium_capere (
     vacuum*  datum,
    Piscina*  piscina,
        Xar** vestigia_out)
{
    Xar* vestigia;
    i32  i;

    (vacuum)datum;
    vestigia = xar_creare(piscina, (i32)magnitudo(FabricaVestigium));
    si (vestigia == NIHIL)
    {
        redde FALSUM;
    }
    _arborem_photographare(chorda_ex_literis("", piscina), vestigia,
        piscina);
    per (i = ZEPHYRUM;
         _directoria_extra != NIHIL
             && i < xar_numerus(_directoria_extra);
         i++)
    {
        _directorium_extra_photographare(
            *(chorda*)xar_obtinere(_directoria_extra, i), vestigia,
            piscina);
    }
    xar_ordinare(vestigia, _vestigia_ordinare);
    *vestigia_out = vestigia;
    redde VERUM;
}

/* directoria '~/...' ex exitibus et vestigiis declaratis */
interior Xar*
_directoria_extra_colligere (
    constans Xar* actiones,
         Piscina* piscina)
{
    Xar* directoria;
    i32  i;
    i32  j;

    directoria = xar_creare(piscina, (i32)magnitudo(chorda));
    per (i = ZEPHYRUM; i < xar_numerus(actiones); i++)
    {
        FabricaActio* actio;
                 Xar* viae;

        actio  = (FabricaActio*)xar_obtinere(actiones, i);
        viae   = xar_creare(piscina, (i32)magnitudo(chorda));
        per (j = ZEPHYRUM; j < xar_numerus(actio->exitus); j++)
        {
            *(chorda*)xar_addere(viae) = ((FabricaExitus*)xar_obtinere(
                actio->exitus, j))->via;
        }
        per (j = ZEPHYRUM; j < xar_numerus(actio->vestigia); j++)
        {
            *(chorda*)xar_addere(viae) = ((FabricaLocus*)xar_obtinere(
                actio->vestigia, j))->via;
        }
        per (j = ZEPHYRUM; j < xar_numerus(viae); j++)
        {
            chorda via;
               s32 k;
               i32 l;
               b32 iam;

            via = *(chorda*)xar_obtinere(viae, j);
            si (via.mensura < III || via.datum[0] != '~')
            {
                perge;
            }
            k = (s32)via.mensura - I;
            dum (k > 0 && via.datum[k] != '/')
            {
                k--;
            }
            via = chorda_sectio(via, 0, (i32)k);
            iam = FALSUM;
            per (l = ZEPHYRUM; l < xar_numerus(directoria); l++)
            {
                si (chorda_aequalis(*(chorda*)xar_obtinere(directoria,
                        l), via))
                {
                    iam = VERUM;
                }
            }
            si (!iam)
            {
                *(chorda*)xar_addere(directoria) = via;
            }
        }
    }
    redde directoria;
}

/* AGENS (sanare, fabrica-6 H4): agere et photographia; fila (plan 2
 * T6) - tutae simul si fila > I et non siccum, I = seriatim, via vetus;
 * directoria extra arborem ex actionibus */
vacuum
suturae_agentem_addere (
     FabricaSutura* sutura,
      constans Xar* actiones,
               b32  siccum,
           Piscina* piscina)
{
    sutura->agere             = _agere;
    sutura->vestigium_capere  = _vestigium_capere;
    suturae_fila_maxima       = suturae_fila_computare(piscina);
    si (suturae_fila_maxima > I && !siccum)
    {
        sutura->agere_simul    = _agere_simul;
        sutura->currere_simul  = suturae_currere_simul;
    }
    _directoria_extra = _directoria_extra_colligere(actiones, piscina);
}
