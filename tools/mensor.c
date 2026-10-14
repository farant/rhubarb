/*
 * mensor.c - Instrumentum mensurandi (vide include/mensura.h)
 *
 * USUS:
 *   eval "$(mensor sessio)"                 # sessionem incipere
 *   mensor mensurare -titulus X -- cmd ...  # mandatum metiri
 *   mensor addere -titulus X -valor N       # valorem addere
 *   mensor condere                          # diarium in volumen
 *
 * NIDIFICATIO: mensurare MENSOR_PARENS in ambitu ponit antequam
 * prolem gignat, ergo quidquid proles ipsa metitur SPONTE sub ea
 * nidificatur - hereditas ambitus id gratis dat. Intra processum
 * unum (ansa conchae per plagulas currens) hereditas nihil dat,
 * ergo -parens explicitum adest.
 *
 * SESSIO: metadata sessionis (machina, commissum) SEMEL in
 * conditione scribuntur, non in linea quaque. Linea quaeque XL
 * octetos redundantes ferret et machina_identitas CL vicibus
 * computaretur - in via calida quam ipsam levem esse voluimus.
 *
 * EXITUS: 0 = factum, I = fractum, II = NIHIL ACTUM (usus malus).
 */

/* plagula provenientiae (fabrica T7): '-provenientia' respondetur */
/* <aedilis obiectum="build/fabrica/provenientia/mensor.c"/> */

#include "postulata_posix.h"

#include "latina.h"
#include "provenientia.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "mensura.h"
#include "machina.h"
#include "processus.h"
#include "volumen.h"
#include "mensura_contextus.h"
#include "internamentum.h"
#include "stml.h"
#include "filum.h"
#include "xar.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>   /* execvp pro -ui */
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/stat.h>

/* Mandatum metiendum horas capere potest (aedificatio plena) */
#define MORA_AMPLA (IV * MXXIV * M)

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

/* Vexillum nudum (sine valore sequente) adesse? */
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
_ambitus_aut (
    constans character* clavis,
    constans character* alterum)
{
    constans character* valor;

    valor = getenv(clavis);
    si (valor != NIHIL && valor[0] != '\0')
    {
        redde valor;
    }

    redde alterum;
}

interior constans character*
_diarium (
      integer   argc,
    character** argv)
{
    constans character* via;

    via = _arg(argc, argv, "-diarium");
    si (via != NIHIL)
    {
        redde via;
    }

    redde _ambitus_aut("MENSOR_DIARIUM", "/tmp/mensurae.diarium");
}

interior chorda
_volumen_via (
      integer   argc,
    character** argv,
      Piscina*  piscina)
{
    constans character* via;
     ChordaAedificator* aedificator;

    via = _arg(argc, argv, "-volumen");
    si (via != NIHIL)
    {
        redde chorda_ex_literis(via, piscina);
    }

    via = getenv("MENSOR_VOLUMEN");
    si (via != NIHIL && via[0] != '\0')
    {
        redde chorda_ex_literis(via, piscina);
    }

    aedificator = chorda_aedificator_creare(piscina, (memoriae_index)C);
    chorda_aedificator_appendere_literis(
        aedificator, _ambitus_aut("HOME", "/tmp"));
    chorda_aedificator_appendere_literis(
        aedificator, "/.rhubarb/mensurae.volumen");

    redde chorda_aedificator_finire(aedificator);
}

/* Chordam in litteras NUL-terminatas vertere (fopen eas poscit) */
interior character*
_litterae (
     chorda  c,
    Piscina* piscina)
{
    character* l;

    l = (character*)piscina_allocare(
            piscina, (memoriae_index)(c.mensura + I));
    si (l == NIHIL)
    {
        redde NIHIL;
    }

    si (c.mensura > ZEPHYRUM)
    {
        memcpy(l, c.datum, (size_t)c.mensura);
    }
    l[c.mensura] = '\0';

    redde l;
}


/* ============================================================
 * sessio
 * ============================================================ */

interior integer
_sessionem_incipere (
      Piscina*  piscina,
      integer   argc,
    character** argv)
{
       chorda sessio;
    character diarium[CCLVI];
          b32 nudus;
      integer i;

    nudus = FALSUM;
    per (i = I; i < argc; i++)
    {
        si (strcmp(argv[i], "-nudus") == 0)
        {
            nudus = VERUM;
        }
    }

    sessio = mensura_sessionem_novam(piscina);

    si (nudus)
    {
        imprimere("%.*s\n", (integer)sessio.mensura,
                  (constans character*)sessio.datum);
        redde 0;
    }

    /* Diarium PER SESSIONEM: cursus simul currentes se invicem non
     * conturbant, et conditio scit quid suum sit */
    sprintf(diarium, "/tmp/mensurae-%.*s.diarium",
            (integer)sessio.mensura, (constans character*)sessio.datum);

    /* Forma quam concha 'eval' consumit */
    imprimere("MENSOR_SESSIO=%.*s; export MENSOR_SESSIO;\n",
              (integer)sessio.mensura,
              (constans character*)sessio.datum);
    imprimere("MENSOR_DIARIUM=%s; export MENSOR_DIARIUM;\n", diarium);

    redde 0;
}


/* ============================================================
 * mensurare / addere
 * ============================================================ */

interior Mensura
_mensuram_struere (
                              Piscina*  piscina,
                              integer   argc,
                            character** argv,
                   constans character*  titulus,
                               duplex   valor,
                   constans character*  unitas)
{
               Mensura  m;
    constans character* parens;

    parens = _arg(argc, argv, "-parens");
    si (parens == NIHIL)
    {
        parens = _ambitus_aut("MENSOR_PARENS", "");
    }

    m.sessio   = chorda_ex_literis(
                     _ambitus_aut("MENSOR_SESSIO", "sine-sessione"),
                     piscina);
    m.id        = mensura_id_novum(piscina);
    m.parens    = chorda_ex_literis(parens, piscina);
    m.titulus   = chorda_ex_literis(titulus, piscina);
    m.valor     = valor;
    m.unitas    = chorda_ex_literis(unitas, piscina);
    m.momentum  = mensura_momentum_nunc();

    redde m;
}

interior integer
_mensurare (
      Piscina*  piscina,
      integer   argc,
    character** argv)
{
    constans character* titulus;
    integer              i, initium;
    s64                  ante, post;
    ProcessusResultus resultus;
              Mensura m;
            character id_tabula[LXIV];

    titulus = _arg(argc, argv, "-titulus");
    si (titulus == NIHIL)
    {
        fprintf(stderr, "mensor: -titulus deest\n");
        redde 2;
    }

    initium = ZEPHYRUM;
    per (i = I; i < argc; i++)
    {
        si (strcmp(argv[i], "--") == 0)
        {
            initium = i + I;
            frange;
        }
    }

    si (initium == ZEPHYRUM || initium >= argc)
    {
        fprintf(stderr, "mensor: '--' cum mandato deest\n");
        redde 2;
    }

    /* Clavem NOSTRAM ante prolem ponere: quidquid proles metitur
     * sub hac mensura nidificatur, sine ulla ratione explicita */
    m = _mensuram_struere(piscina, argc, argv, titulus, 0.0, "secunda");
    sprintf(id_tabula, "%.*s", (integer)m.id.mensura,
            (constans character*)m.id.datum);
    setenv("MENSOR_PARENS", id_tabula, I);

    ante     = mensura_momentum_nunc();
    resultus = processus_exsequi(
                   (constans character* constans*)(argv + initium),
                   (i32)MORA_AMPLA, piscina);
    post = mensura_momentum_nunc();

    m.valor     = (duplex)(post - ante) / 1000.0;
    m.momentum  = ante;

    /* Effusionem prolis TRANSMITTERE. processus_exsequi eam colligit,
     * ergo sine hoc instrumentum omnem exitum aedificationis quam
     * involvit DEVORARET.
     * LIMES NOTUS: colligitur, non fluit - nihil vides donec mandatum
     * perficiatur. Pro aedificatione trium minutarum id asperum est;
     * semita incrementalis (incipere/pulsare/metere) id postea sanet. */
    si (resultus.effusio.mensura > ZEPHYRUM)
    {
        fwrite(resultus.effusio.datum, (size_t)I,
               (size_t)resultus.effusio.mensura, stdout);
    }
    si (resultus.erratum.mensura > ZEPHYRUM)
    {
        fwrite(resultus.erratum.datum, (size_t)I,
               (size_t)resultus.erratum.mensura, stderr);
    }

    si (!mensura_annotare(_diarium(argc, argv), &m, piscina))
    {
        fprintf(stderr, "mensor: mensura annotari non potuit\n");
        redde I;
    }

    si (!resultus.successus)
    {
        fprintf(stderr, "mensor: mandatum fractum (%s)\n",
                processus_error_nomen(resultus.error));
        redde I;
    }

    /* Codicem exitus prolis PROPAGAMUS: instrumentum quod
     * fracturam mensurando celaret portam frangeret */
    redde (integer)resultus.codex_exitus;
}

/*
 * _addere_tabulam - Fascem mensurarum ex tabula "<valor> <titulus>"
 *
 * CUR FASCIS: aliter suita CXXX probationum CCLX processus gigneret.
 * Forma tabulae ea ipsa est quam compile_tests.sh iam scribit.
 * Formam lineae diarii BIBLIOTHECA tenet, non concha - aliter duo
 * exemplaria formae per tempus dissiderent.
 */
interior integer
_addere_tabulam (
                            Piscina*  piscina,
                            integer   argc,
                          character** argv,
                 constans character*  via_tabulae,
                 constans character*  praefixum,
                 constans character*  unitas)
{
    FILE*              plagula;
    character          linea[IV * MXXIV];
    i32                numerus;
    ChordaAedificator* aedificator;

    plagula = fopen(via_tabulae, "rb");
    si (plagula == NIHIL)
    {
        fprintf(stderr, "mensor: tabula aperiri non potest: %s\n",
                via_tabulae);
        redde I;
    }

    numerus = ZEPHYRUM;

    dum (fgets(linea, IV * MXXIV, plagula) != NIHIL)
    {
           duplex  valor;
        character* finis;
        character* titulus;
          Mensura  m;

        valor = strtod(linea, &finis);
        si (finis == linea)
        {
            perge;   /* linea sine numero */
        }

        dum (*finis == ' ' || *finis == '\t')
        {
            finis++;
        }

        titulus = finis;
        dum (*finis != '\0' && *finis != '\n' && *finis != '\r')
        {
            finis++;
        }
        *finis = '\0';

        si (titulus[0] == '\0')
        {
            perge;
        }

        aedificator = chorda_aedificator_creare(piscina,
                                                (memoriae_index)C);
        chorda_aedificator_appendere_literis(aedificator, praefixum);
        chorda_aedificator_appendere_literis(aedificator, titulus);

        m = _mensuram_struere(piscina, argc, argv, "", valor, unitas);
        m.titulus = chorda_aedificator_finire(aedificator);

        si (mensura_annotare(_diarium(argc, argv), &m, piscina))
        {
            numerus++;
        }
    }

    fclose(plagula);

    imprimere("mensor: %u mensurae ex tabula\n", numerus);

    redde 0;
}

interior integer
_addere (
      Piscina*  piscina,
      integer   argc,
    character** argv)
{
    constans character* titulus;
    constans character* valor_litterae;
    constans character* unitas;
    constans character* tabula;
    constans character* praefixum;
               Mensura  m;

    titulus         = _arg(argc, argv, "-titulus");
    valor_litterae  = _arg(argc, argv, "-valor");
    unitas          = _arg(argc, argv, "-unitas");
    tabula          = _arg(argc, argv, "-tabula");
    praefixum       = _arg(argc, argv, "-praefixum");

    si (tabula != NIHIL)
    {
        redde _addere_tabulam(piscina, argc, argv, tabula,
                              (praefixum != NIHIL) ? praefixum : "",
                              (unitas != NIHIL) ? unitas : "secunda");
    }

    si (titulus == NIHIL || valor_litterae == NIHIL)
    {
        fprintf(stderr, "mensor: -titulus et -valor poscuntur\n");
        redde 2;
    }

    si (unitas == NIHIL)
    {
        unitas = "numerus";
    }

    m = _mensuram_struere(piscina, argc, argv, titulus,
                          strtod(valor_litterae, NIHIL), unitas);

    si (!mensura_annotare(_diarium(argc, argv), &m, piscina))
    {
        fprintf(stderr, "mensor: mensura annotari non potuit\n");
        redde I;
    }

    /* -id: clavem creatam IMPRIMERE ut concha eam ut parentem
     * adhibere possit. Sine hoc nidificatio intra processum unum
     * impossibilis est - hereditas ambitus ibi nihil dat. */
    si (_vexillum(argc, argv, "-id"))
    {
        imprimere("%.*s\n", (integer)m.id.mensura,
                  (constans character*)m.id.datum);
    }

    redde 0;
}


/* ============================================================
 * condere
 * ============================================================ */

interior chorda
_commissum (
    Piscina* piscina)
{
    constans character* argumenta[IV];
     ProcessusResultus  resultus;

    argumenta[0]    = "git";
    argumenta[I]    = "rev-parse";
    argumenta[II]   = "HEAD";
    argumenta[III]  = NIHIL;

    resultus = processus_exsequi(argumenta, (i32)(X * M), piscina);
    si (!resultus.successus || resultus.codex_exitus != 0)
    {
        redde chorda_ex_literis("", piscina);
    }

    /* Lineam novam terminalem tollere */
    dum (   resultus.effusio.mensura > ZEPHYRUM
         && (resultus.effusio.datum[resultus.effusio.mensura - I]
             == '\n'
        || resultus.effusio.datum[resultus.effusio.mensura - I]
            == '\r'))
    {
        resultus.effusio.mensura--;
    }

    redde resultus.effusio;
}

interior integer
_condere (
      Piscina*  piscina,
      integer   argc,
    character** argv)
{
               chorda  via;
              Volumen* volumen;
       MensuraFructus  fructus;
          MachinaNota  nota;
    ChordaAedificator* aedificator;
               chorda  commissum;

    via = _volumen_via(argc, argv, piscina);
    volumen = volumen_aperire_aut_creare(piscina, _litterae(via,
        piscina));
    si (volumen == NIHIL)
    {
        fprintf(stderr, "mensor: volumen aperiri non potest: %.*s\n",
                (integer)via.mensura, (constans character*)via.datum);
        redde I;
    }

    /* Metadata sessionis SEMEL - hic, non in linea quaque */
    nota       = machina_nota(piscina);
    commissum  = _commissum(piscina);

    aedificator = chorda_aedificator_creare(piscina,
        (memoriae_index)CC);
    chorda_aedificator_appendere_literis(
        aedificator, _ambitus_aut("MENSOR_SESSIO", "sine-sessione"));
    chorda_aedificator_appendere_character(aedificator, '\t');
    chorda_aedificator_appendere_chorda(aedificator, nota.identitas);
    chorda_aedificator_appendere_character(aedificator, '\t');
    chorda_aedificator_appendere_chorda(aedificator, commissum);
    chorda_aedificator_appendere_character(aedificator, '\t');
    chorda_aedificator_appendere_chorda(aedificator, nota.systema);
    chorda_aedificator_appendere_character(aedificator, '\t');
    chorda_aedificator_appendere_chorda(aedificator, nota.architectura);

    volumen_actum_appendere(volumen, "sessio",
                            chorda_aedificator_finire(aedificator));

    fructus = mensura_condere(_diarium(argc, argv), volumen, piscina);

    volumen_claudere(volumen);

    si (!fructus.successus)
    {
        fprintf(stderr, "mensor: conditio fracta: %.*s\n",
                (integer)fructus.causa.mensura,
                (constans character*)fructus.causa.datum);
        redde I;
    }

    imprimere("mensor: %u mensurae conditae", fructus.conditae);
    si (fructus.causa.mensura > ZEPHYRUM)
    {
        imprimere(" (%.*s)", (integer)fructus.causa.mensura,
                  (constans character*)fructus.causa.datum);
    }
    imprimere("\n");

    redde 0;
}


/* ============================================================
 * cursus (analytica T2): currere, legere
 * ============================================================ */

/* acta: corpus maximum per fluxum (cauda servatur) */
#define ACTA_MAXIMA (IV * MXXIV * MXXIV)

/* horologium monotonum (duratio; murus pro initio/fine) */
interior s64
_horologium_ms (vacuum)
{
    structura timespec t;

    si (clock_gettime(CLOCK_MONOTONIC, &t) != 0)
    {
        redde mensura_momentum_nunc();
    }
    redde (s64)t.tv_sec * (s64)M + (s64)t.tv_nsec / ((s64)M * (s64)M);
}

interior vacuum
_dormire_ms (
    longus ms)
{
    structura timespec  mora;

    mora.tv_sec   = (time_t)(ms / (longus)M);
    mora.tv_nsec  = (ms % (longus)M) * (longus)M * (longus)M;
    (vacuum)nanosleep(&mora, NIHIL);
}

/* annotatio '<cursus titulus="X" versio="N"/>' in capite scripti
 * (lineae primae, MMXLVIII octeti); titulus vacuus si absens */
interior chorda
_attributum_annotationis (
    constans character* textus,
    constans character* nomen_attributi,
               Piscina* piscina)
{
    constans character* initium;
    constans character* finis;
             character  quaesitum[LXIV];

    sprintf(quaesitum, "%s=\"", nomen_attributi);
    initium = strstr(textus, quaesitum);
    si (initium == NIHIL)
    {
        redde chorda_ex_literis("", piscina);
    }
    initium  += strlen(quaesitum);
    finis    = strchr(initium, '"');
    si (finis == NIHIL)
    {
        redde chorda_ex_literis("", piscina);
    }
    redde chorda_transcribere(chorda_sectio(chorda_ex_literis(textus,
        piscina), (i32)(initium - textus), (i32)(finis - textus)),
        piscina);
}

interior chorda
_titulum_annotationis (
    constans character* via_scripti,
               Piscina* piscina)
{
         character caput[MMXLVIII + I];
             FILE*  f;
            size_t  lecta;
         character* annotatio;
         character* finis;

    f = fopen(via_scripti, "rb");
    si (f == NIHIL)
    {
        redde chorda_ex_literis("", piscina);
    }
    lecta = fread(caput, (size_t)I, (size_t)MMXLVIII, f);
    fclose(f);
    caput[lecta]  = '\0';
    annotatio     = strstr(caput, "<cursus ");
    si (annotatio == NIHIL)
    {
        redde chorda_ex_literis("", piscina);
    }
    finis = strstr(annotatio, "/>");
    si (finis == NIHIL)
    {
        redde chorda_ex_literis("", piscina);
    }
    *finis = '\0';
    redde _attributum_annotationis(annotatio, "titulus", piscina);
}

/* SUTURA ACTORUM (spec par. IV): corpus cursus per hanc functionem
 * SOLAM in diurna ponitur (T4; redactio postea intra diurna_ponere).
 * Corpus = "== effusio ==" + "== erratum ==", fluxus quisque ad
 * ACTA_MAXIMA (cauda servatur) - summa intra DIURNA_CORPUS_MAXIMUM.
 * Referentia = sigillum corporis (LXIV hex); vacua si poni nequit. */
interior vacuum
_fluxum_addere (
    ChordaAedificator* a,
   constans character* titulus,
                chorda  fluxus)
{
    character caput[CXXVIII];

    sprintf(caput, "== %s ==\n", titulus);
    chorda_aedificator_appendere_literis(a, caput);
    si (fluxus.mensura > (i32)ACTA_MAXIMA)
    {
        sprintf(caput, "[... %ld octeti praecisi, cauda servata]\n",
            (longus)(fluxus.mensura - (i32)ACTA_MAXIMA));
        chorda_aedificator_appendere_literis(a, caput);
        fluxus.datum    += fluxus.mensura - (i32)ACTA_MAXIMA;
        fluxus.mensura  = (i32)ACTA_MAXIMA;
    }
    si (fluxus.mensura > ZEPHYRUM)
    {
        chorda_aedificator_appendere_chorda(a, fluxus);
        si (fluxus.datum[fluxus.mensura - I] != '\n')
        {
            chorda_aedificator_appendere_character(a, '\n');
        }
    }
}

/* corpus ponere sub id cursus; referentia (sigillum) et mensura */
interior chorda
_corpus_ponere (
               chorda  id,
               chorda  titulus,
                  b32  fractum,
               chorda  corpus,
              Piscina* piscina,
                  s64* mensura_out)
{
              Diurna* diurna = mensura_diurna_aperire(piscina);
     DiurnaIntroitus  positum;

    *mensura_out = (s64)ZEPHYRUM;
    si (   diurna == NIHIL
        || !diurna_ponere(diurna, id, titulus, fractum, corpus,
        &positum))
    {
        fprintf(stderr, "mensor cursus: corpus non positum (%.*s)\n",
            (integer)id.mensura, (constans character*)id.datum);
        redde chorda_ex_literis("", piscina);
    }
    *mensura_out = positum.mensura;
    redde chorda_transcribere(chorda_ex_literis(positum.sigillum,
        piscina),
        piscina);
}

interior chorda
_acta_servare (
               chorda  id,
               chorda  titulus,
                  b32  fractum,
               chorda  effusio,
               chorda  erratum,
              Piscina* piscina,
                  s64* mensura_out)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)(effusio.mensura + erratum.mensura + CXXVIII));

    *mensura_out = (s64)ZEPHYRUM;
    si (a == NIHIL)
    {
        redde chorda_ex_literis("", piscina);
    }
    _fluxum_addere(a, "effusio", effusio);
    _fluxum_addere(a, "erratum", erratum);
    redde _corpus_ponere(id, titulus, fractum,
        chorda_aedificator_finire(a),
        piscina, mensura_out);
}

/* corpus ex stdin ('-') aut plagula legere */
interior chorda
_corpus_legere (
    constans character* via,
               Piscina* piscina)
{
     ChordaAedificator* a;
             character  frustum[IV * MXXIV];
                size_t  n;

    si (strcmp(via, "-") != ZEPHYRUM)
    {
        redde filum_legere_totum(via, piscina);
    }
    a = chorda_aedificator_creare(piscina,
        (memoriae_index)(IV * MXXIV));
    si (a == NIHIL)
    {
        redde chorda_ex_literis("", piscina);
    }
    dum ((n = fread(frustum, (size_t)I, magnitudo(frustum), stdin)) > 0)
    {
        chorda pars;

        pars.datum    = (i8*)frustum;
        pars.mensura  = (i32)n;
        chorda_aedificator_appendere_chorda(a, pars);
    }
    redde chorda_aedificator_finire(a);
}

/* fluxum novum (post offset) effundere; offset promovetur */
interior vacuum
_novum_effundere (
     chorda  fluxus,
        i32* offset,
       FILE* exitus)
{
    si (fluxus.mensura > *offset)
    {
        fwrite(fluxus.datum + *offset, (size_t)I,
            (size_t)(fluxus.mensura - *offset), exitus);
        fflush(exitus);
        *offset = fluxus.mensura;
    }
}

interior integer
_cursum_currere (
      Piscina*  piscina,
      integer   argc,
    character** argv)
{
          CursusInitium  initium;
            CursusFinis  finis;
                integer  i;
                integer  primum = ZEPHYRUM;
              Processus* processus;
      ProcessusResultus  r;
                    i32  offset_ef = ZEPHYRUM;
                    i32  offset_er = ZEPHYRUM;
                    s64  ante;
      structura rusage  usus;
      ChordaAedificator* argv_textus;
              character  id_tabula[LXIV];
     constans character* diarium;
     constans character* titulus;
                integer  codex;

    per (i = I; i < argc; i++)
    {
        si (strcmp(argv[i], "--") == 0)
        {
            primum = i + I;
            frange;
        }
    }
    si (primum == ZEPHYRUM || primum >= argc)
    {
        fprintf(stderr,
            "mensor cursus currere: '--' cum mandato deest\n");
        redde 2;
    }
    memset(&initium, ZEPHYRUM, magnitudo(initium));
    memset(&finis, ZEPHYRUM, magnitudo(finis));
    mensura_contextum_parare(&initium, piscina);
    titulus = _arg(argc, argv, "-titulus");
    initium.titulus = (titulus != NIHIL)
        ? chorda_ex_literis(titulus, piscina)
        : _titulum_annotationis(argv[primum], piscina);
    si (initium.titulus.mensura == ZEPHYRUM)
    {
        initium.titulus = chorda_ex_literis(argv[primum], piscina);
    }
    argv_textus = chorda_aedificator_creare(piscina,
        (memoriae_index)CC);
    per (i = primum; i < argc; i++)
    {
        si (i > primum)
        {
            chorda_aedificator_appendere_character(argv_textus, ' ');
        }
        chorda_aedificator_appendere_literis(argv_textus, argv[i]);
    }
    initium.argv     = chorda_aedificator_finire(argv_textus);
    initium.id       = mensura_id_novum(piscina);
    initium.initium  = mensura_momentum_nunc();
    diarium          = mensura_diarium_cursuum(piscina);
    si (!mensura_cursum_incipere(diarium, &initium, piscina))
    {
        fprintf(stderr, "mensor cursus: initium non annotatum (%s)\n",
            diarium);
    }
    /* proles sub hoc cursu: parens, et scriptum ipsum non iterum
     * involvitur */
    sprintf(id_tabula, "%.*s", (integer)initium.id.mensura,
        (constans character*)initium.id.datum);
    setenv("MENSOR_PARENS", id_tabula, I);
    setenv("CURSUS_INTUS", argv[primum], I);

    ante       = _horologium_ms();
    processus  = processus_incipere(
        (constans character* constans*)(argv + primum), ZEPHYRUM,
        piscina);
    si (processus == NIHIL)
    {
        redde 127;
    }
    dum (processus_pulsare(processus) == PROCESSUS_CURRIT)
    {
        _novum_effundere(processus_effusio_hactenus(processus),
            &offset_ef, stdout);
        _novum_effundere(processus_erratum_hactenus(processus),
            &offset_er, stderr);
        _dormire_ms((longus)XX);
    }
    r = processus_metere(processus);
    _novum_effundere(r.effusio, &offset_ef, stdout);
    _novum_effundere(r.erratum, &offset_er, stderr);

    codex = (r.signum != ZEPHYRUM) ? (integer)(CXXVIII + r.signum)
        : (r.successus || r.codex_exitus != ZEPHYRUM)
            ? (integer)r.codex_exitus : 127;
    finis.id          = initium.id;
    finis.momentum    = mensura_momentum_nunc();
    finis.duratio_ms  = _horologium_ms() - ante;
    finis.exitus      = (s32)codex;
    si (getrusage(RUSAGE_CHILDREN, &usus) == ZEPHYRUM)
    {
        /* macOS: ru_maxrss in octetis */
        finis.rss_maxima         = (s64)usus.ru_maxrss;
        finis.cpu_usoris_ms      = (s64)usus.ru_utime.tv_sec * (s64)M
            + (s64)usus.ru_utime.tv_usec / (s64)M;
        finis.cpu_systematis_ms  = (s64)usus.ru_stime.tv_sec * (s64)M
            + (s64)usus.ru_stime.tv_usec / (s64)M;
    }
    finis.acta = _acta_servare(initium.id, initium.titulus, codex != 0,
        r.effusio, r.erratum, piscina, &finis.acta_mensura);
    si (!mensura_cursum_finire(diarium, &finis, piscina))
    {
        fprintf(stderr, "mensor cursus: finis non annotatus (%s)\n",
            diarium);
    }
    redde codex;
}

/* diarium in volumen condere PER RENOMINATIONEM: appensores quae post
 * eam scribunt plagulam novam creant, nihil inter lectionem et
 * truncationem perditur (fenestra: appensio quae ante renominationem
 * aperuit - mora brevis eam claudit) */
interior b32
_cursus_condere (
    Volumen* volumen,
    Piscina* piscina)
{
    constans character* diarium = mensura_diarium_cursuum(piscina);
             character  condendum[DXII + LXIV];
        MensuraFructus  fructus;

    sprintf(condendum, "%s.condendum.%ld", diarium, (longus)getpid());
    si (rename(diarium, condendum) != ZEPHYRUM)
    {
        redde VERUM;   /* diarium absens: nihil condendum */
    }
    _dormire_ms((longus)L);
    fructus = mensura_condere(condendum, volumen, piscina);
    (vacuum)remove(condendum);
    redde fructus.successus;
}

interior constans character*
_status_verbum (
    CursusStatus status)
{
    commutatio (status)
    {
        casus CURSUS_TRANSIIT:  redde "transiit";
        casus CURSUS_FRACTUS:   redde "fractus";
        ordinarius:             redde "interruptus";
    }
}

/* campus ad lineam TSV visibilem: TAB et linea nova -> spatium */
interior constans character*
_visum (
     chorda  campus,
    Piscina* piscina)
{
    character* a = (character*)piscina_allocare(piscina,
        (memoriae_index)(campus.mensura + I));
          i32 j;

    si (a == NIHIL)
    {
        redde "";
    }
    per (j = ZEPHYRUM; j < campus.mensura; j++)
    {
        a[j] = (campus.datum[j] == '\t' || campus.datum[j] == '\n')
            ? ' ' : (character)campus.datum[j];
    }
    a[campus.mensura] = '\0';
    redde a;
}

/* INCIPERE (analytica T3): producentes sine involucro (silva.porta,
 * commissio, phases) cursum aperiunt; stdout = lineae CLAVIS=valor
 * ambitus quem proles ferant (MENSOR_PARENS = id novum + contextus) -
 * vocans nihil formae reimplementat. Exitus 1 = non annotatum. */
interior integer
_cursum_incipere (
      Piscina*  piscina,
      integer   argc,
    character** argv)
{
          CursusInitium  initium;
     constans character* titulus    = _arg(argc, argv, "-titulus");
     constans character* argumenta  = _arg(argc, argv, "-argv");
     constans character* claves[VI];
                    i32  k;

    si (titulus == NIHIL || titulus[ZEPHYRUM] == '\0')
    {
        fprintf(stderr, "mensor cursus incipere: -titulus deest\n");
        redde 2;
    }
    memset(&initium, ZEPHYRUM, magnitudo(initium));
    mensura_contextum_parare(&initium, piscina);
    initium.titulus  = chorda_ex_literis(titulus, piscina);
    initium.argv     = chorda_ex_literis(argumenta != NIHIL ? argumenta
        : "", piscina);
    initium.id       = mensura_id_novum(piscina);
    initium.initium  = mensura_momentum_nunc();
    si (!mensura_cursum_incipere(mensura_diarium_cursuum(piscina),
            &initium, piscina))
    {
        fprintf(stderr,
            "mensor cursus incipere: initium non annotatum\n");
        redde I;
    }
    imprimere("MENSOR_PARENS=%.*s\n", (integer)initium.id.mensura,
        (constans character*)initium.id.datum);
    claves[ZEPHYRUM]  = "MENSOR_SESSIO";
    claves[I]         = "MENSOR_COMMISSUM";
    claves[II]        = "MENSOR_SORDIDUM";
    claves[III]       = "MENSOR_RAMUS";
    claves[IV]        = "MENSOR_ARBOR";
    claves[V]         = "MENSOR_MACHINA";
    per (k = ZEPHYRUM; k < (i32)VI; k++)
    {
        constans character* valor = getenv(claves[k]);

        imprimere("%s=%s\n", claves[k], valor != NIHIL ? valor : "");
    }
    redde ZEPHYRUM;
}

/* FINIRE: '-id X -exitus N [-duratio_ms D] [-nota S]' - finis cursi
 * aperti per incipere; momentum = nunc, rusage ignota (0) */
interior integer
_cursum_finire (
      Piscina*  piscina,
      integer   argc,
    character** argv)
{
            CursusFinis  finis;
     constans character* id       = _arg(argc, argv, "-id");
     constans character* exitus   = _arg(argc, argv, "-exitus");
     constans character* duratio  = _arg(argc, argv, "-duratio_ms");
     constans character* nota     = _arg(argc, argv, "-nota");
     constans character* acta     = _arg(argc, argv, "-acta");
     constans character* titulus  = _arg(argc, argv, "-titulus");

    si (id == NIHIL || exitus == NIHIL)
    {
        fprintf(stderr,
            "mensor cursus finire: -id et -exitus necessaria\n");
        redde 2;
    }
    memset(&finis, ZEPHYRUM, magnitudo(finis));
    finis.id        = chorda_ex_literis(id, piscina);
    finis.momentum  = mensura_momentum_nunc();
    finis.exitus    = (s32)strtol(exitus, NIHIL, X);
    finis.duratio_ms  = (duratio != NIHIL) ? (s64)strtol(duratio, NIHIL,
        X) : (s64)ZEPHYRUM;
    finis.nota        = chorda_ex_literis(nota != NIHIL ? nota : "",
        piscina);
    si (acta != NIHIL)
    {
        finis.acta = _corpus_ponere(finis.id, chorda_ex_literis(
            titulus != NIHIL ? titulus : "", piscina), finis.exitus
                != 0,
            _corpus_legere(acta, piscina), piscina,
            &finis.acta_mensura);
    }
    si (!mensura_cursum_finire(mensura_diarium_cursuum(piscina), &finis,
            piscina))
    {
        fprintf(stderr, "mensor cursus finire: finis non annotatus\n");
        redde I;
    }
    redde ZEPHYRUM;
}

/* ACTA: 'cursus acta <id>' - corpus cursus ex diurna; exitus 1 cum
 * statu nominato (ignotum, deletum, corruptum) */
interior integer
_cursus_acta (
      Piscina*  piscina,
      integer   argc,
    character** argv)
{
          Diurna* diurna;
    DiurnaStatus  status = DIURNA_IGNOTUM;
          chorda  corpus;

    si (argc < IV)
    {
        fprintf(stderr, "usus: mensor cursus acta <id>\n");
        redde 2;
    }
    diurna = mensura_diurna_aperire(piscina);
    si (diurna == NIHIL)
    {
        fprintf(stderr,
            "mensor cursus acta: diurna aperiri non potest\n");
        redde I;
    }
    corpus = diurna_legere(diurna, chorda_ex_literis(argv[III],
        piscina),
        piscina, &status);
    si (status != DIURNA_ADEST)
    {
        fprintf(stderr, "mensor cursus acta %s: %s\n", argv[III],
            diurna_status_nomen(status));
        redde I;
    }
    fwrite(corpus.datum, (size_t)I, (size_t)corpus.mensura, stdout);
    redde ZEPHYRUM;
}

interior integer
_cursus_legere (
      Piscina*  piscina,
      integer   argc,
    character** argv)
{
          Volumen* volumen;
              Xar* cursus;
              i32  k;
              b32  machina  = _vexillum(argc, argv, "-machina");
           chorda  via      = _volumen_via(argc, argv, piscina);

    volumen = volumen_aperire_aut_creare(piscina, _litterae(via,
        piscina));
    si (volumen == NIHIL)
    {
        fprintf(stderr, "mensor: volumen aperiri non potest\n");
        redde I;
    }
    si (!_cursus_condere(volumen, piscina))
    {
        fprintf(stderr, "mensor: diarium cursuum condi non potuit\n");
    }
    cursus = mensura_cursus_legere(volumen, piscina);
    per (k = ZEPHYRUM; cursus != NIHIL && k < xar_numerus(cursus); k++)
    {
        constans Cursus* c = (constans Cursus*)xar_obtinere(cursus, k);
     constans character* a = _visum(c->initium.argv, piscina);
     constans character* nota = _visum(c->finis.nota, piscina);

        imprimere(machina
            ? "CURSUS\t%.*s\t%.*s\t%.*s\t%s\t%d\t%ld\t%.*s\t%d"
              "\t%.*s\t%.*s\t%.*s\t%.*s\t%s\t%s\n"
            : "%.*s  parens=%.*s  %.*s  %s  exitus %d  %ld ms  %.*s"
              " sordidum=%d  %.*s  %.*s  %.*s  acta %.*s  [%s]  %s\n",
            (integer)c->initium.id.mensura,
            (constans character*)c->initium.id.datum,
            (integer)c->initium.parens.mensura,
            (constans character*)c->initium.parens.datum,
            (integer)c->initium.titulus.mensura,
            (constans character*)c->initium.titulus.datum,
            _status_verbum(c->status), (integer)c->finis.exitus,
            (longus)c->finis.duratio_ms,
            (integer)c->initium.commissum.mensura,
            (constans character*)c->initium.commissum.datum,
            (integer)(c->initium.sordidum ? I : ZEPHYRUM),
            (integer)c->initium.ramus.mensura,
            (constans character*)c->initium.ramus.datum,
            (integer)c->initium.arbor.mensura,
            (constans character*)c->initium.arbor.datum,
            (integer)c->initium.vocator.mensura,
            (constans character*)c->initium.vocator.datum,
            (integer)c->finis.acta.mensura,
            (constans character*)c->finis.acta.datum, a, nota);
    }
    volumen_claudere(volumen);
    redde ZEPHYRUM;
}

interior integer
_cursus (
      Piscina*  piscina,
      integer   argc,
    character** argv)
{
    si (argc >= III && strcmp(argv[II], "currere") == ZEPHYRUM)
    {
        redde _cursum_currere(piscina, argc, argv);
    }
    si (argc >= III && strcmp(argv[II], "legere") == ZEPHYRUM)
    {
        redde _cursus_legere(piscina, argc, argv);
    }
    si (argc >= III && strcmp(argv[II], "incipere") == ZEPHYRUM)
    {
        redde _cursum_incipere(piscina, argc, argv);
    }
    si (argc >= III && strcmp(argv[II], "finire") == ZEPHYRUM)
    {
        redde _cursum_finire(piscina, argc, argv);
    }
    si (argc >= III && strcmp(argv[II], "acta") == ZEPHYRUM)
    {
        redde _cursus_acta(piscina, argc, argv);
    }
    fprintf(stderr,
        "usus: mensor cursus currere [-titulus T] -- mandatum ...\n"
        "      mensor cursus legere [-machina] [-volumen via]\n"
        "      mensor cursus incipere -titulus T [-argv S]\n"
        "      mensor cursus finire -id X -exitus N [-duratio_ms D]"
        " [-nota S] [-titulus T -acta via|-]\n"
        "      mensor cursus acta <id>\n");
    redde 2;
}


/* ============================================================
 * purgare (analytica T4 2c): regula retentionis ex STML -> diurna
 * ============================================================ */

/* attributum numericum (dies) elementi regulae; FALSUM si deest */
interior b32
_regulae_dies (
             StmlNodus* radix,
    constans character* elementum,
    constans character* attributum,
                   s64* ms_out)
{
      StmlNodus* nodus = stml_invenire_liberum(radix, elementum);
         chorda* valor;
      character  tabula[XXXII];
      character* finis;
         longus  dies;

    si (nodus == NIHIL)
    {
        redde FALSUM;
    }
    valor = stml_attributum_capere(nodus, attributum);
    si (   valor          == NIHIL || valor->mensura <= ZEPHYRUM
        || valor->mensura >= (i32)XXXII)
    {
        redde FALSUM;
    }
    memcpy(tabula, valor->datum, (size_t)valor->mensura);
    tabula[valor->mensura]  = '\0';
    dies                    = strtol(tabula, &finis, X);
    si (*finis != '\0' || dies < 0)
    {
        redde FALSUM;
    }
    *ms_out = (s64)dies * (s64)(LXXXVI * M + CD) * (s64)M;
    redde VERUM;
}

/* regula TOTA aut nulla: elementum absens = recusatio nominata (nullae
 * constantes tacitae - interview Q8) */
interior b32
_regulam_legere (
     constans character* via,
           DiurnaRegula* regula,
                Piscina* piscina)
{
                chorda  fons = filum_legere_totum(via, piscina);
          StmlResultus  r;
             StmlNodus* radix;
             StmlNodus* ultimum;
                chorda* servare;
    constans character* deest = NIHIL;

    memset(regula, ZEPHYRUM, magnitudo(DiurnaRegula));
    si (fons.mensura == ZEPHYRUM)
    {
        fprintf(stderr, "mensor purgare: regula retentionis absens: %s"
            " (-regula via aut MENSOR_RETENTIO)\n", via);
        redde FALSUM;
    }
    r      = stml_legere(fons, piscina, internamentum_creare(piscina));
    radix  = r.successus ? r.elementum_radix : NIHIL;
    si (   radix == NIHIL || radix->titulus == NIHIL
        || !chorda_aequalis_literis(*radix->titulus, "retentio"))
    {
        fprintf(stderr, "mensor purgare: %s: elementum 'retentio' deest"
            " aut STML fractum\n", via);
        redde FALSUM;
    }
    si (!_regulae_dies(radix, "sana", "cruda", &regula->crudum_ms))
    {
        deest = "sana cruda";
    }
    alioquin si (!_regulae_dies(radix, "sana", "compressa",
                     &regula->compressum_ms))
    {
        deest = "sana compressa";
    }
    alioquin si (!_regulae_dies(radix, "fracta", "cruda",
                     &regula->crudum_fracti_ms))
    {
        deest = "fracta cruda";
    }
    alioquin si (!_regulae_dies(radix, "fracta", "compressa",
                     &regula->compressum_fracti_ms))
    {
        deest = "fracta compressa";
    }
    alioquin si (!_regulae_dies(radix, "oblivio", "dies",
                     &regula->oblivio_ms))
    {
        deest = "oblivio dies";
    }
    ultimum = stml_invenire_liberum(radix, "ultimum");
    servare = (ultimum != NIHIL) ? stml_attributum_capere(ultimum,
        "servare") : NIHIL;
    si (deest == NIHIL && servare == NIHIL)
    {
        deest = "ultimum servare";
    }
    si (deest != NIHIL)
    {
        fprintf(stderr, "mensor purgare: %s: '%s' deest aut non numerus"
            " dierum\n", via, deest);
        redde FALSUM;
    }
    regula->ultimum_servare = chorda_aequalis_literis(*servare,
        "verum");
    redde VERUM;
}

interior integer
_purgare (
      Piscina*  piscina,
      integer   argc,
    character** argv)
{
                   b32  siccum   = _vexillum(argc, argv, "-siccum");
                   b32  machina  = _vexillum(argc, argv, "-machina");
    constans character* via      = _arg(argc, argv, "-regula");
          DiurnaRegula  regula;
         DiurnaRelatio  relatio;
                Diurna* diurna;

    si (via == NIHIL)
    {
        via = _ambitus_aut("MENSOR_RETENTIO",
            "tools/analytica_retentio.stml");
    }
    si (!_regulam_legere(via, &regula, piscina))
    {
        redde 2;
    }
    diurna = mensura_diurna_aperire(piscina);
    si (diurna == NIHIL)
    {
        fprintf(stderr, "mensor purgare: diurna aperiri non potest\n");
        redde I;
    }
    si (!diurna_purgare(diurna, &regula, mensura_momentum_nunc(),
        siccum,
            &relatio))
    {
        fprintf(stderr, "mensor purgare: purgatio fracta (sera aut"
            " index)\n");
        redde I;
    }
    imprimere(machina
        ? "PURGATIO\t%s\t%d\t%d\t%d\t%d\t%ld\t%ld\t%d\t%d\t%d\n"
        : "mensor purgare%s: introitus %d, corpora %d, comprimenda %d,"
          " delenda %d, octeti %ld (liberati %ld); index lineae %d"
          " -> %d,"
          " oblita %d\n",
        siccum ? (machina ? "siccum" : " (siccum)") : (machina ? "verum"
            : ""), (integer)relatio.introitus, (integer)relatio.corpora,
        (integer)relatio.compressa, (integer)relatio.deleta,
        (longus)relatio.octeti_ante, (longus)relatio.octeti_liberati,
        (integer)relatio.lineae_ante, (integer)relatio.lineae_post,
        (integer)relatio.oblita);
    redde ZEPHYRUM;
}


/* ============================================================ */

interior vacuum
_usus (vacuum)
{
    fprintf(stderr,
        "mensor - mensurae nidificatae\n\n"
        "  eval \"$(mensor sessio)\"      sessionem incipere\n"
        "  mensor sessio -nudus          clavem solam imprimere\n"
        "  mensor mensurare -titulus X [-unitas U] [-parens ID] -- cmd\n"
        "  mensor addere -titulus X -valor N [-unitas U] [-parens ID]\n"
        "  mensor condere [-volumen via]\n"
        "  mensor cursus currere [-titulus T] -- cmd  (tools/cursus.sh)\n"
        "  mensor cursus legere [-machina]\n"
        "  mensor purgare [-siccum] [-machina] [-regula via]"
        "   (diurna)\n\n"
        "  Ambitus: MENSOR_SESSIO MENSOR_PARENS MENSOR_DIARIUM"
        " MENSOR_VOLUMEN\n");
}

externus constans ProvenientiaRelatio provenientia_mensor;


integer
main (
      integer   argc,
    character** argv)
{
    Piscina* piscina;
    integer  codex;

    si (provenientia_respondere(argc, argv, &provenientia_mensor))
    {
        redde ZEPHYRUM;
    }
    si (argc < II)
    {
        _usus();
        redde 2;
    }

    piscina = piscina_generare_dynamicum("mensor", M * M);
    si (piscina == NIHIL)
    {
        fprintf(stderr, "mensor: piscina fracta\n");
        redde I;
    }

    /* -ui: BINARIUM ALIUD exsequimur, non vitream hic ligamus.
     * Ligata, omnis vocatio in via calida AppKit et WebKit ad exec
     * ferret - vitium observatoris per posticum. Semita ex argv[0]
     * ducitur ut binaria iuxta se posita se inveniant. */
    si (strcmp(argv[I], "-ui") == 0 || strcmp(argv[I], "ui") == 0)
    {
        character  semita[DXII];
        character* ultima;

        piscina_destruere(piscina);

        strncpy(semita, argv[0], magnitudo(semita) - I);
        semita[magnitudo(semita) - I] = '\0';

        ultima = strrchr(semita, '/');
        si (ultima != NIHIL)
        {
            strcpy(ultima + I, "mensor_ui");
        }
        alioquin
        {
            strcpy(semita, "mensor_ui");
        }

        argv[I] = semita;
        execvp(semita, argv + I);

        /* execvp solum redit cum fallit */
        fprintf(stderr, "mensor: mensor_ui inveniri non potest (%s)\n",
                semita);
        redde I;
    }

    si (strcmp(argv[I], "sessio") == 0)
    {
        codex = _sessionem_incipere(piscina, argc, argv);
    }
    alioquin si (strcmp(argv[I], "mensurare") == 0)
    {
        codex = _mensurare(piscina, argc, argv);
    }
    alioquin si (strcmp(argv[I], "addere") == 0)
    {
        codex = _addere(piscina, argc, argv);
    }
    alioquin si (strcmp(argv[I], "condere") == 0)
    {
        codex = _condere(piscina, argc, argv);
    }
    alioquin si (strcmp(argv[I], "purgare") == 0)
    {
        codex = _purgare(piscina, argc, argv);
    }
    alioquin si (strcmp(argv[I], "cursus") == 0)
    {
        codex = _cursus(piscina, argc, argv);
    }
    alioquin
    {
        _usus();
        codex = 2;
    }

    piscina_destruere(piscina);

    redde codex;
}
