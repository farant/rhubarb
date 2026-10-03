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
#include "tabula_dispersa.h"
#include "chorda_aedificator.h"
#include "fabrica.h"
#include "provenientia.h"
#include "scrinium.h"
#include "numerus_romanus.h"
#include "thesaurus.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>

#define MORA_GENERATORIS_MS 600000
#define MORA_RELATIONIS_MS  5000
/* sanare: terminus unus per actum (briar ~3 min, obiecta radicis
 * frigida ~150 s); termini per actionem non in 1b */
#define MORA_SANATIONIS_MS  1800000

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

/* crusta/fontationes.sh <via> (spec 3 par. III.2) */
interior b32
_fontationes (
                vacuum* datum,
    constans character* via,
               Piscina* piscina,
                chorda* effusio_out,
                   i32* codex_out)
{
     constans character* argv[III];
      ProcessusResultus  resultus;

    (vacuum)datum;
    argv[0]   = "./crusta/fontationes.sh";
    argv[I]   = via;
    argv[II]  = NIHIL;
    resultus  = processus_exsequi(argv, CXX * M, piscina);
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

interior vacuum
_suturam_iudicii_parare (
    FabricaSutura* sutura,
     constans Xar* actiones,
          Piscina* piscina)
{
    sutura->ambitus      = _ambitus;
    sutura->species      = _species;
    sutura->identitas    = _identitas;
    sutura->fontationes  = _fontationes;
    sutura->exitus_noti  = _exitus_notos_colligere(actiones, piscina);
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
interior chorda
_ultimae_lineae (
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
    printf("  agitur %.*s:", (s32)actio->titulus.mensura,
        (constans character*)actio->titulus.datum);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        chorda verbum;

        verbum        = *(chorda*)xar_obtinere(actio->mandatum, i);
        argumenta[i]  = chorda_ut_cstr(verbum, piscina);
        printf(" %s", argumenta[i]);
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
interior b32
_actum_complere (
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
    actum_out->cauda = _ultimae_lineae(resultus.erratum.mensura > 0
        ? resultus.erratum : resultus.effusio, XX);
    redde VERUM;
}

interior chorda
_radix_absoluta (
    Piscina* piscina);

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
        radix     = _radix_absoluta(piscina);
        absoluta = chorda_concatenare(chorda_concatenare(radix,
            chorda_ex_literis("/", piscina), piscina), relativa,
            piscina);
        (vacuum)filum_directorium_creare_cum_parentibus(
            "build/fabrica/lectiones");
        (vacuum)filum_scribere(chorda_ut_cstr(relativa, piscina),
            chorda_ex_literis("", piscina));
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
    redde _actum_complere(resultus, acta, acta_via, piscina, actum_out);
}

/* FILA (plan 2 T6): actiones tutae simul. FABRICA_FILA=N aut nuclei
 * efficientiae (sysctl hw.perflevel0.physicalcpu); I = seriatim (via
 * vetus, agere_simul NIHIL). */
hic_manens i32 _fila_maxima = I;

interior i32
_fila_computare (
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

/* agere SIMUL (plan 2 T6): usque ad _fila_maxima filii simul per
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

        dum (!fractum && proximus < numerus && currentes < _fila_maxima)
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
            si (!_actum_complere(resultus, acta[k], acta_viae[k],
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
 * _fila_maxima simul, quaeque in scripturam suam VACUAM (FABRICA_SCRIPTURA
 * absoluta) cum libro suo (FABRICA_LECTIONES absoluta, vetus deletus),
 * ANTE incipere positis. Causa ut _currere. Iudicium nihil sistit:
 * omnes currunt. */
interior vacuum
_currere_simul (
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

        dum (proximus < numerus && currentes < _fila_maxima)
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


/* ==================================================
 * Photographia arboris (plan 1b T4): vestigium_capere
 * ================================================== */

/* directoria '~/...' quae declarationes nominant (exitus aut
 * vestigia extra arborem) - _sanare ea ponit ante photographias */
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


/* ==================================================
 * Memoria verificationum (build/fabrica.db; plan 1a T6)
 *
 * Ordo = (titulus, clavis, artificium): clavis = sigillum ingressuum
 * ET mandati, artificium = sigillum commissi - ambo congruere debent
 * (artificium manu mutatum numquam memoria tegitur). Nucleus solus
 * decernit quando scribitur (post regenerationem congruentem actionis
 * memorabilis) et quando legitur (actio memorabilis). sqlite HIC
 * solum, numquam in lib/ (nexus caeci obiectorum build/).
 * ================================================== */

nomen structura {
    Scrinium* scrinium;
     Piscina* piscina;
} Memoria;

/* migratio II (plan 1b T7): cursus - omne actum sanationis (tempus,
 * duratio, eventus, causa); -siccum ex eo aestimat, lens 'tempus'
 * inventarii impletur */
hic_manens constans character* constans MIGRATIONES_MEMORIAE[IV] = {
    "CREATE TABLE verificationes ("
    " titulus TEXT NOT NULL, clavis TEXT NOT NULL,"
    " artificium TEXT NOT NULL, tempus INTEGER NOT NULL,"
    " PRIMARY KEY (titulus, clavis, artificium))",
    "CREATE TABLE cursus ("
    " titulus TEXT NOT NULL, initium INTEGER NOT NULL,"
    " duratio_ms INTEGER NOT NULL, eventus TEXT NOT NULL,"
    " causa TEXT NOT NULL)",
    /* migratio III (plan 2 T2): vestigia lectionum - clavis ut
     * verificationes; sigilla ut massae (XXXII octeti) */
    "CREATE TABLE lectiones ("
    " titulus TEXT NOT NULL, clavis TEXT NOT NULL,"
    " artificium TEXT NOT NULL, genus TEXT NOT NULL,"
    " via TEXT NOT NULL, sigillum BLOB NOT NULL)",
    /* migratio IV: vestigium per (titulus, EXITUS) - actio exituum
     * multorum vestigia sua inter se delebat (fragmenta_silva) */
    "ALTER TABLE lectiones ADD COLUMN exitus TEXT NOT NULL DEFAULT ''"
};

interior vacuum
_hex_ligare (
    ScriniumEnuntiatum* enuntiatum,
               integer  index,
     constans Sigillum* sigillum,
               Piscina* piscina)
{
    character hex[SIGILLUM_HEX_MENSURA];

    sigillum_hex(sigillum, hex);
    (vacuum)scrinium_ligare_textum(enuntiatum, index,
        chorda_ex_literis(hex, piscina));
}

interior b32
_meminisse (
                vacuum* datum,
    constans character* titulus,
     constans Sigillum* clavis,
     constans Sigillum* artificium)
{
               Memoria* memoria;
    ScriniumEnuntiatum* enuntiatum;
                   b32  inventum;

    memoria    = (Memoria*)datum;
    enuntiatum = scrinium_praeparare(memoria->scrinium,
        "SELECT 1 FROM verificationes WHERE titulus = ?"
        " AND clavis = ? AND artificium = ?");
    si (enuntiatum == NIHIL)
    {
        redde FALSUM;
    }
    (vacuum)scrinium_ligare_textum(enuntiatum, I,
        chorda_ex_literis(titulus, memoria->piscina));
    _hex_ligare(enuntiatum, II, clavis, memoria->piscina);
    _hex_ligare(enuntiatum, III, artificium, memoria->piscina);
    inventum = (scrinium_gradi(enuntiatum) == SCRINIUM_ORDO);
    scrinium_finire(enuntiatum);
    redde inventum;
}

interior vacuum
_inscribere (
                vacuum* datum,
    constans character* titulus,
     constans Sigillum* clavis,
     constans Sigillum* artificium)
{
               Memoria* memoria;
    ScriniumEnuntiatum* enuntiatum;

    memoria    = (Memoria*)datum;
    enuntiatum = scrinium_praeparare(memoria->scrinium,
        "INSERT OR REPLACE INTO verificationes"
        " (titulus, clavis, artificium, tempus) VALUES (?, ?, ?, ?)");
    si (enuntiatum == NIHIL)
    {
        redde;
    }
    (vacuum)scrinium_ligare_textum(enuntiatum, I,
        chorda_ex_literis(titulus, memoria->piscina));
    _hex_ligare(enuntiatum, II, clavis, memoria->piscina);
    _hex_ligare(enuntiatum, III, artificium, memoria->piscina);
    (vacuum)scrinium_ligare_numerum(enuntiatum, IV,
        (s64)time(NIHIL));
    si (scrinium_gradi(enuntiatum) != SCRINIUM_FACTUM)
    {
        fprintf(stderr, "fabrica: verificatio non scripta (%s): %s\n",
            titulus, scrinium_error(memoria->scrinium));
    }
    scrinium_finire(enuntiatum);
}

/* vestigium lectionum (plan 2 T2) legere: lectiones sub clave exacta;
 * FALSUM = nullum vestigium */
interior b32
_lectiones_legere (
                vacuum*  datum,
    constans character*  titulus,
    constans character*  exitus,
     constans Sigillum*  clavis,
     constans Sigillum*  artificium,
               Piscina*  piscina,
                   Xar** lectiones_out)
{
               Memoria* memoria;
    ScriniumEnuntiatum* enuntiatum;
                   Xar* lectiones;

    memoria    = (Memoria*)datum;
    enuntiatum = scrinium_praeparare(memoria->scrinium,
        "SELECT genus, via, sigillum FROM lectiones WHERE titulus = ?"
        " AND clavis = ? AND artificium = ? AND exitus = ?");
    si (enuntiatum == NIHIL)
    {
        redde FALSUM;
    }
    (vacuum)scrinium_ligare_textum(enuntiatum, I,
        chorda_ex_literis(titulus, piscina));
    _hex_ligare(enuntiatum, II, clavis, piscina);
    _hex_ligare(enuntiatum, III, artificium, piscina);
    (vacuum)scrinium_ligare_textum(enuntiatum, IV,
        chorda_ex_literis(exitus, piscina));
    lectiones = xar_creare(piscina, (i32)magnitudo(FabricaLectio));
    dum (   lectiones                  != NIHIL
         && scrinium_gradi(enuntiatum) == SCRINIUM_ORDO)
    {
               chorda  genus;
               chorda  massa;
        FabricaLectio* lectio;

        genus = scrinium_columna_textus(enuntiatum, 0, piscina);
        massa = scrinium_columna_massa(enuntiatum, II, piscina);
        si (genus.mensura != I || massa.mensura != SIGILLUM_OCTETI)
        {
            perge;
        }
        lectio = (FabricaLectio*)xar_addere(lectiones);
        si (lectio == NIHIL)
        {
            frange;
        }
        commutatio (genus.datum[0])
        {
            casus 'A': lectio->genus = LECTIO_ABSENS;      frange;
            casus 'X': lectio->genus = LECTIO_EXSTAT;      frange;
            casus 'D': lectio->genus = LECTIO_ENUMERAVIT;  frange;
            casus 'E': lectio->genus = LECTIO_AMBITUS;     frange;
            ordinarius: lectio->genus = LECTIO_LEGIT;      frange;
        }
        lectio->via = scrinium_columna_textus(enuntiatum, I, piscina);
        memcpy(lectio->sigillum.octeti, massa.datum, SIGILLUM_OCTETI);
    }
    scrinium_finire(enuntiatum);
    si (lectiones == NIHIL || xar_numerus(lectiones) == 0)
    {
        redde FALSUM;
    }
    *lectiones_out = lectiones;
    redde VERUM;
}

/* vestigia ultima tituli (omnes exitus): viae distinctae - ORDO */
interior b32
_lectiones_ultimae (
                vacuum*  datum,
    constans character*  titulus,
               Piscina*  piscina,
                   Xar** lectiones_out)
{
               Memoria* memoria;
    ScriniumEnuntiatum* enuntiatum;
                   Xar* lectiones;

    memoria    = (Memoria*)datum;
    enuntiatum = scrinium_praeparare(memoria->scrinium,
        "SELECT DISTINCT genus, via FROM lectiones WHERE titulus = ?");
    si (enuntiatum == NIHIL)
    {
        redde FALSUM;
    }
    (vacuum)scrinium_ligare_textum(enuntiatum, I,
        chorda_ex_literis(titulus, piscina));
    lectiones = xar_creare(piscina, (i32)magnitudo(FabricaLectio));
    dum (   lectiones                  != NIHIL
         && scrinium_gradi(enuntiatum) == SCRINIUM_ORDO)
    {
               chorda  genus;
        FabricaLectio* lectio;

        genus   = scrinium_columna_textus(enuntiatum, 0, piscina);
        lectio  = (FabricaLectio*)xar_addere(lectiones);
        si (lectio == NIHIL || genus.mensura != I)
        {
            frange;
        }
        lectio->genus = (genus.datum[0] == 'D') ? LECTIO_ENUMERAVIT
            : LECTIO_LEGIT;
        lectio->via = scrinium_columna_textus(enuntiatum, I, piscina);
    }
    scrinium_finire(enuntiatum);
    si (lectiones == NIHIL || xar_numerus(lectiones) == 0)
    {
        redde FALSUM;
    }
    *lectiones_out = lectiones;
    redde VERUM;
}

/* vestigium scribere: vetera tituli deleta (vestigium ultimum solum),
 * nova in transactione una */
interior vacuum
_lectiones_scribere (
                vacuum* datum,
    constans character* titulus,
    constans character* exitus,
     constans Sigillum* clavis,
     constans Sigillum* artificium,
          constans Xar* lectiones)
{
               Memoria* memoria;
    ScriniumEnuntiatum* enuntiatum;
                   i32  i;

    memoria = (Memoria*)datum;
    (vacuum)scrinium_incipere(memoria->scrinium);
    enuntiatum = scrinium_praeparare(memoria->scrinium,
        "DELETE FROM lectiones WHERE titulus = ? AND exitus = ?");
    si (enuntiatum != NIHIL)
    {
        (vacuum)scrinium_ligare_textum(enuntiatum, I,
            chorda_ex_literis(titulus, memoria->piscina));
        (vacuum)scrinium_ligare_textum(enuntiatum, II,
            chorda_ex_literis(exitus, memoria->piscina));
        (vacuum)scrinium_gradi(enuntiatum);
        scrinium_finire(enuntiatum);
    }
    per (i = ZEPHYRUM; i < xar_numerus(lectiones); i++)
    {
        constans FabricaLectio* lectio;
                     character  littera[II];
                        chorda  massa;
                            i8  octeti[SIGILLUM_OCTETI];

        lectio = (constans FabricaLectio*)xar_obtinere(lectiones, i);
        enuntiatum = scrinium_praeparare(memoria->scrinium,
            "INSERT INTO lectiones (titulus, clavis, artificium, genus,"
            " via, sigillum, exitus) VALUES (?, ?, ?, ?, ?, ?, ?)");
        si (enuntiatum == NIHIL)
        {
            frange;
        }
        commutatio (lectio->genus)
        {
            casus LECTIO_ABSENS:      littera[0] = 'A'; frange;
            casus LECTIO_EXSTAT:      littera[0] = 'X'; frange;
            casus LECTIO_ENUMERAVIT:  littera[0] = 'D'; frange;
            casus LECTIO_AMBITUS:     littera[0] = 'E'; frange;
            ordinarius:               littera[0] = 'L'; frange;
        }
        littera[I] = '\0';
        memcpy(octeti, lectio->sigillum.octeti, SIGILLUM_OCTETI);
        massa.datum    = octeti;
        massa.mensura  = SIGILLUM_OCTETI;
        (vacuum)scrinium_ligare_textum(enuntiatum, I,
            chorda_ex_literis(titulus, memoria->piscina));
        _hex_ligare(enuntiatum, II, clavis, memoria->piscina);
        _hex_ligare(enuntiatum, III, artificium, memoria->piscina);
        (vacuum)scrinium_ligare_textum(enuntiatum, IV,
            chorda_ex_literis(littera, memoria->piscina));
        (vacuum)scrinium_ligare_textum(enuntiatum, V, lectio->via);
        (vacuum)scrinium_ligare_massam(enuntiatum, VI, massa);
        (vacuum)scrinium_ligare_textum(enuntiatum, VII,
            chorda_ex_literis(exitus, memoria->piscina));
        si (scrinium_gradi(enuntiatum) != SCRINIUM_FACTUM)
        {
            fprintf(stderr,
                "fabrica: vestigium non scriptum (%s): %s\n",
                titulus, scrinium_error(memoria->scrinium));
        }
        scrinium_finire(enuntiatum);
    }
    (vacuum)scrinium_committere(memoria->scrinium);
}

/* radix arboris absoluta (praefixum viarum libri demendum) */
interior chorda
_radix_absoluta (
    Piscina* piscina)
{
    character sedes[4096];

    si (getcwd(sedes, magnitudo(sedes)) == NIHIL)
    {
        redde chorda_ex_literis("", piscina);
    }
    redde chorda_ex_literis(sedes, piscina);
}

/* build/fabrica.db aperire; FALSUM (cautio impressa) = iudicium sine
 * memoria - numquam fractum propter memoriam */
interior b32
_memoriam_aperire (
    Memoria* memoria,
    Piscina* piscina)
{
    memoria->piscina = piscina;
    si (!filum_directorium_creare_cum_parentibus("build"))
    {
        redde FALSUM;
    }
    memoria->scrinium = scrinium_aperire(piscina, "build/fabrica.db");
    si (   memoria->scrinium == NIHIL
        || !scrinium_migrare(memoria->scrinium, MIGRATIONES_MEMORIAE,
        IV))
    {
        fprintf(stderr, "fabrica: CAUTIO memoria build/fabrica.db "
            "aperiri nequit - iudicium sine memoria\n");
        redde FALSUM;
    }
    redde VERUM;
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
    fabrica_suturam_parare(&enumerans);
    enumerans.legere     = _legere;
    enumerans.enumerare  = _enumerare;
    intern               = internamentum_creare(piscina);
    causa                = chorda_ex_literis("", piscina);
    radix                = filum_legere_totum("fabrica.stml", piscina);
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
            printf("ORPHANUM: build/aedilis/%.*s/ "
                "(scopus %.*s absens)\n",
                (s32)introitus->titulus.mensura,
                (constans character*)introitus->titulus.datum,
                (s32)scopus.mensura, (constans character*)scopus.datum);
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
            si (!omnia)
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
    printf("%s %.*s - %.*s\n", signum,
        (s32)sententia->iudicium.artificium.mensura,
        (constans character*)sententia->iudicium.artificium.datum,
        (s32)sententia->iudicium.causa.mensura,
        (constans character*)sententia->iudicium.causa.datum);
}

/* cursus (sectio sanare infra): iudicium quoque regenerationes suas
 * scribit (parcum …AR15) */
interior vacuum
_cursum_inscribere (
                     vacuum* datum,
    constans FabricaSanatio* sanatio);

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

    fabrica_suturam_parare(&levis);
    levis.legere     = _legere;
    levis.enumerare  = _enumerare;
    levis.sigilla    = sigilla;
    /* vestigia lectionum (plan 2 T2): viae ultimae lectae = loci ordinis
     * actionum lectiones="verum" (manifesta iis dempta) */
    si (memoria != NIHIL)
    {
        levis.datum              = memoria;
        levis.lectiones_ultimae  = _lectiones_ultimae;
    }
    si (!fabrica_dependentias_computare(&levis, actiones, piscina,
            causa_out))
    {
        redde NIHIL;
    }
    redde fabrica_ordinare(actiones, piscina, causa_out);
}

interior s32
_iudicare (
          s32   argc,
    character** argv,
      Piscina*  piscina)
{
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
    memoria_aperta  = _memoriam_aperire(&memoria, piscina);
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

    fabrica_suturam_parare(&sutura);
    sutura.legere     = _legere;
    sutura.enumerare  = _enumerare;
    sutura.currere    = _currere;
    sutura.rogare     = _rogare;
    si (memoria_aperta)
    {
        sutura.datum               = &memoria;
        sutura.meminisse           = _meminisse;
        sutura.inscribere          = _inscribere;
        sutura.cursum_inscribere   = _cursum_inscribere;
        sutura.lectiones_legere    = _lectiones_legere;
        sutura.lectiones_scribere  = _lectiones_scribere;
        sutura.lectiones_ultimae   = _lectiones_ultimae;
    }
    sutura.radix    = _radix_absoluta(piscina);
    _suturam_iudicii_parare(&sutura, actiones, piscina);
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
    _fila_maxima = _fila_computare(piscina);
    si (plenus && _fila_maxima > I)
    {
        Xar* praevidendae;

        sutura.currere_simul = _currere_simul;
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

    sententiae = xar_creare(piscina, (i32)magnitudo(Sententia));
    per (i = ZEPHYRUM; i < xar_numerus(ordo); i++)
    {
        FabricaActio* actio;

        actio = *(FabricaActio**)xar_obtinere(ordo, i);
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
                si (omnia && _electum(electa, exitus->via))
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
            sententia->iudicium = fabrica_iudicare(&sutura, actio,
                exitus, plenus, piscina);
        }
    }
    si (xar_numerus(sententiae) == 0 && tacta)
    {
        printf("fabrica: nulla artificia generata a viis tacta\n");
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
        printf("COMPOSITUM %.*s - %.*s\n",
            (s32)coniunctum.artificium.mensura,
            (constans character*)coniunctum.artificium.datum,
            (s32)coniunctum.causa.mensura,
            (constans character*)coniunctum.causa.datum);
    }

    /* SANATIO: actiones artificiorum stalorum/ignotorum, ordine
     * dependentiae, semel quaeque */
    si (stala + ignota > 0)
    {
        FabricaActio* prior;

        printf("SANATIO:\n");
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
        i32 sine;

        sine = _binaria_sine_declaratione(actiones, piscina);
        si (sine > 0)
        {
            printf("IGNOTUM: %u binaria in bin/ sine declaratione "
                "(numerus solus, Q36)\n", (insignatus integer)sine);
        }
    }
    printf("fabrica: %u recentia, %u stala, %u ignota, %u non iudicata"
        "%s, %u orphana\n",
        (insignatus integer)recentia, (insignatus integer)stala,
        (insignatus integer)ignota, (insignatus integer)non_iudicata,
        plenus ? "" : " (celer)", (insignatus integer)orphana);
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

interior constans character*
_eventus_titulus (
    FabricaEventus eventus)
{
    commutatio (eventus)
    {
        casus FABRICA_SANATUM:
            redde "SANATUM";
        casus FABRICA_PRAEPARATUM:
            redde "PRAEPARATUM";
        casus FABRICA_FRACTUM:
            redde "FRACTUM";
        casus FABRICA_OMISSUM:
            redde "OMISSUM";
        casus FABRICA_AGENDUM:
            redde "AGENDUM";
        casus FABRICA_IUDICIUM:
            redde "IUDICIUM";
        casus FABRICA_AUDITUM_DISCORS:
            redde "AUDITUM_DISCORS";
        ordinarius:
            redde "FORTASSE";
    }
}

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

/* textus vacuus VERUS: scrinium_ligare_textum chordam vacuam cuius
 * datum NIHIL est ut SQL NULL ligat (sqlite3_bind_text cum NULL) -
 * causa sanati vacua 'NOT NULL' frangebat (1b T7) */
hic_manens character TEXTUS_VACUUS[I] = { '\0' };

/* cursus (1b T7): actum scribere; tempus = initium ~ finis - duratio */
interior vacuum
_cursum_inscribere (
                     vacuum* datum,
    constans FabricaSanatio* sanatio)
{
               Memoria* memoria;
    ScriniumEnuntiatum* enuntiatum;

    memoria    = (Memoria*)datum;
    enuntiatum = scrinium_praeparare(memoria->scrinium,
        "INSERT INTO cursus (titulus, initium, duratio_ms, eventus,"
        " causa) VALUES (?, ?, ?, ?, ?)");
    si (enuntiatum == NIHIL)
    {
        redde;
    }
    (vacuum)scrinium_ligare_textum(enuntiatum, I,
        sanatio->actio->titulus);
    (vacuum)scrinium_ligare_numerum(enuntiatum, II,
        (s64)time(NIHIL) - (s64)(sanatio->duratio_ms / M));
    (vacuum)scrinium_ligare_numerum(enuntiatum, III,
        (s64)sanatio->duratio_ms);
    (vacuum)scrinium_ligare_textum(enuntiatum, IV, chorda_ex_literis(
        _eventus_titulus(sanatio->eventus), memoria->piscina));
    si (sanatio->causa.mensura > 0)
    {
        (vacuum)scrinium_ligare_textum(enuntiatum, V, sanatio->causa);
    }
    alioquin
    {
        chorda vacua;

        vacua.datum    = (i8*)TEXTUS_VACUUS;
        vacua.mensura  = ZEPHYRUM;
        (vacuum)scrinium_ligare_textum(enuntiatum, V, vacua);
    }
    si (scrinium_gradi(enuntiatum) != SCRINIUM_FACTUM)
    {
        fprintf(stderr, "fabrica: cursus non scriptus: %s\n",
            scrinium_error(memoria->scrinium));
    }
    scrinium_finire(enuntiatum);
}

/* duratio cursus ULTIMI sanati (aut praeparati) tituli */
interior b32
_cursum_legere (
                vacuum* datum,
    constans character* titulus,
                   i32* duratio_ms_out)
{
               Memoria* memoria;
    ScriniumEnuntiatum* enuntiatum;
                   b32  inventum;

    memoria    = (Memoria*)datum;
    enuntiatum = scrinium_praeparare(memoria->scrinium,
        "SELECT duratio_ms FROM cursus WHERE titulus = ?"
        " AND eventus IN ('SANATUM', 'PRAEPARATUM')"
        " ORDER BY initium DESC, rowid DESC LIMIT 1");
    si (enuntiatum == NIHIL)
    {
        redde FALSUM;
    }
    (vacuum)scrinium_ligare_textum(enuntiatum, I,
        chorda_ex_literis(titulus, memoria->piscina));
    inventum = (scrinium_gradi(enuntiatum) == SCRINIUM_ORDO);
    si (inventum)
    {
        *duratio_ms_out = (i32)scrinium_columna_numerus(enuntiatum,
            ZEPHYRUM);
    }
    scrinium_finire(enuntiatum);
    redde inventum;
}

interior s32
_sanare (
          s32   argc,
    character** argv,
      Piscina*  piscina)
{
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
                i32  numeri[VI];
                i32  duratio;
                i32  aestimatio;
                i32  sine_tempore;

    siccum = FALSUM;
    per (a = II; a < argc; a++)
    {
        si (strcmp(argv[a], "-siccum") == 0)
        {
            siccum = VERUM;
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
    memoria_aperta  = _memoriam_aperire(&memoria, piscina);
    ordo     = _ordinare_per_locos(actiones, sigilla, piscina,
        memoria_aperta ? &memoria : NIHIL, &causa);
    si (ordo == NIHIL)
    {
        fprintf(stderr, "fabrica: %.*s\n", (s32)causa.mensura,
            (constans character*)causa.datum);
        redde II;
    }

    /* electa: via artificii aut titulus compositi */
    electa = NIHIL;
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

    fabrica_suturam_parare(&sutura);
    sutura.legere            = _legere;
    sutura.enumerare         = _enumerare;
    sutura.currere           = _currere;
    sutura.rogare            = _rogare;
    sutura.agere             = _agere;
    sutura.vestigium_capere  = _vestigium_capere;
    /* fila (plan 2 T6): tutae simul; I = seriatim, via vetus */
    _fila_maxima = _fila_computare(piscina);
    si (_fila_maxima > I && !siccum)
    {
        sutura.agere_simul    = _agere_simul;
        sutura.currere_simul  = _currere_simul;
    }
    _directoria_extra = _directoria_extra_colligere(actiones, piscina);
    sutura.sigilla = sigilla;
    sutura.regenerationes = tabula_dispersa_creare_chorda(piscina, 64);
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
        sutura.datum               = &memoria;
        sutura.meminisse           = _meminisse;
        sutura.inscribere          = _inscribere;
        sutura.cursum_inscribere   = _cursum_inscribere;
        sutura.cursum_legere       = _cursum_legere;
        sutura.lectiones_legere    = _lectiones_legere;
        sutura.lectiones_scribere  = _lectiones_scribere;
        sutura.lectiones_ultimae   = _lectiones_ultimae;
    }
    sutura.radix = _radix_absoluta(piscina);
    _suturam_iudicii_parare(&sutura, actiones, piscina);

    printf("fabrica sanare%s: iudicium plenum, ordine dependentiae\n",
        siccum ? " -siccum" : "");
    fflush(stdout);
    sanationes = fabrica_sanare(&sutura, ordo, electa, siccum, piscina,
        &causa);
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

    per (i = ZEPHYRUM; i < VI; i++)
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
        printf("%-11s %.*s", _eventus_titulus(sanatio->eventus),
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
        printf("UNDAE (simul possent; sanare tutas - lectiones=\"verum\" - "
            "simul currit, fila %u; ceteras seriatim):\n",
            (insignatus integer)_fila_maxima);
        per (i = ZEPHYRUM; undae != NIHIL
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
    si (siccum)
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
            "%u omissa (%u s)\n",
            (insignatus integer)numeri[FABRICA_SANATUM],
            (insignatus integer)numeri[FABRICA_PRAEPARATUM],
            (insignatus integer)numeri[FABRICA_FRACTUM],
            (insignatus integer)numeri[FABRICA_OMISSUM],
            (insignatus integer)(duratio / M));
        si (commissa)
        {
            printf("plagulae generatae in arbore mutatae fortasse "
                "(git status) - committe per silva.commissio; sanare "
                "numquam committit\n");
        }
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
    redde (numeri[FABRICA_FRACTUM] + numeri[FABRICA_OMISSUM] > 0)
        ? I : ZEPHYRUM;
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
    fabrica_suturam_parare(&sutura);
    sutura.legere     = _legere;
    sutura.enumerare  = _enumerare;
    sutura.currere    = NIHIL;
    sutura.rogare     = NIHIL;
    sutura.meminisse  = NIHIL;
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
    alioquin
    {
        _usus();
        exitus = II;
    }
    piscina_destruere(piscina);
    redde exitus;
}
