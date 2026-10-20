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
#include "tabula_dispersa.h"
#include "iter_directoria.h"
#include "mensura_quaestio.h"
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
#include <fcntl.h>

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

/* horologium VIGILANS (duratio; murus pro initio/fine): somno non
 * crescit (colloquia T8d0) - Darwin CLOCK_MONOTONIC somnum numerat,
 * duratio cursus operculo clauso mentiebatur; CLOCK_UPTIME_RAW non */
#ifdef CLOCK_UPTIME_RAW
#define HOROLOGIUM_VIGILANS CLOCK_UPTIME_RAW
#else
#define HOROLOGIUM_VIGILANS CLOCK_MONOTONIC
#endif

interior s64
_horologium_ms (vacuum)
{
    structura timespec t;

    si (clock_gettime(HOROLOGIUM_VIGILANS, &t) != 0)
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

/* CUSTOS (colloquia T8b): pid possessoris cursus ut mensura ordinaria
 * (parens = id cursus; forma lineae cursus non mutatur - mensura.h).
 * Cursus apertus cuius custos abiit necatus est, non currens. Scriptura
 * fracta nihil frangit. */
interior vacuum
_custodem_notare (
    constans CursusInitium* initium,
                       s32  pid,
                   Piscina* piscina)
{
    Mensura m;

    memset(&m, ZEPHYRUM, magnitudo(m));
    m.sessio    = initium->sessio;
    m.id        = mensura_id_novum(piscina);
    m.parens    = initium->id;
    m.titulus   = chorda_ex_literis(MENSURA_CUSTOS, piscina);
    m.valor     = (duplex)pid;
    m.unitas    = chorda_ex_literis("pid", piscina);
    m.momentum  = initium->initium;
    (vacuum)mensura_annotare(mensura_diarium_cursuum(piscina), &m,
        piscina);
}

/* COLLOQUIUM (colloquia T9): cursus SUMMUS (parens nullus) sub
 * sessione Claude (CLAUDE_CODE_SESSION_ID, in ambitu Bash hereditatum,
 * etiam commissione detracta) mensuram 'colloquium:<id>' fert - titulus
 * clavis est, valor I. Filii per radicem sessioni pertinent. id impurum
 * (TAB, linea nova) a mensura_annotare recusatur: nihil scriptum. */
interior vacuum
_colloquium_notare (
    constans CursusInitium* initium,
                   Piscina* piscina)
{
    constans character* sessio = getenv("CLAUDE_CODE_SESSION_ID");
               Mensura  m;

    si (   initium->parens.mensura > ZEPHYRUM || sessio == NIHIL
        || sessio[ZEPHYRUM] == '\0')
    {
        redde;
    }
    memset(&m, ZEPHYRUM, magnitudo(m));
    m.sessio  = initium->sessio;
    m.id      = mensura_id_novum(piscina);
    m.parens  = initium->id;
    m.titulus   =
        chorda_concatenare(chorda_ex_literis(MENSURA_COLLOQUIUM,
        piscina), chorda_ex_literis(sessio, piscina), piscina);
    m.valor     = 1.0;
    m.unitas    = chorda_ex_literis("colloquium", piscina);
    m.momentum  = initium->initium;
    (vacuum)mensura_annotare(mensura_diarium_cursuum(piscina), &m,
        piscina);
}

/* curatio (T6c) infra definitur */
interior b32
_curatio_debita (
    Piscina* piscina);

interior vacuum
_curare_separatim (
    constans character* mensor);

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
    /* involucrum prolem exspectat et finem scribit: ipsum custos */
    _custodem_notare(&initium, (s32)getpid(), piscina);
    _colloquium_notare(&initium, piscina);
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
    /* cursus SUMMUS: curatio separata si debita (T6c) */
    si (initium.parens.mensura == ZEPHYRUM && _curatio_debita(piscina))
    {
        _curare_separatim(argv[ZEPHYRUM]);
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
    /* vocans (silva.py subprocess) finem scribet: is custos est */
    _custodem_notare(&initium, (s32)getppid(), piscina);
    _colloquium_notare(&initium, piscina);
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
    /* cursus SUMMUS (nullus parens in ambitu - commissio exterior):
     * curatio separata si debita (T6c) */
    si (getenv("MENSOR_PARENS") == NIHIL && _curatio_debita(piscina))
    {
        _curare_separatim(argv[ZEPHYRUM]);
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

/* censura infra (post quaestiones) definitur */
interior integer
_censura (
      Piscina*  piscina,
      integer   argc,
    character** argv);


/* FACTUM (colloquia T8c): '-titulus T -valor N [-unitas U]' - mensura
 * cursui APERTO (MENSOR_PARENS) adhaerens in diario CURSUUM (non
 * /tmp/mensurae.diarium 'addere' veteris): producentes sine involucro
 * (silva.commissio: consilium.portae) nec formam nec viam diarii
 * reimplementant. Sine cursu aut sine -valor: exitus 2, nihil
 * scriptum (factum sine cursu nihil significat). */
interior integer
_cursum_factum (
      Piscina*  piscina,
      integer   argc,
    character** argv)
{
    constans character* titulus  = _arg(argc, argv, "-titulus");
    constans character* valor    = _arg(argc, argv, "-valor");
    constans character* unitas   = _arg(argc, argv, "-unitas");
    constans character* parens   = getenv("MENSOR_PARENS");
               Mensura  m;

    si (   titulus == NIHIL || valor == NIHIL || parens == NIHIL
        || parens[ZEPHYRUM] == '\0')
    {
        fprintf(stderr, "mensor cursus factum: -titulus, -valor et "
            "MENSOR_PARENS (cursus apertus) poscuntur\n");
        redde 2;
    }
    m = _mensuram_struere(piscina, argc, argv, titulus, strtod(valor,
        NIHIL), unitas != NIHIL ? unitas : "numerus");
    si (!mensura_annotare(mensura_diarium_cursuum(piscina), &m,
        piscina))
    {
        fprintf(stderr, "mensor cursus factum: non annotatum\n");
        redde I;
    }
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
    si (argc >= III && strcmp(argv[II], "factum") == ZEPHYRUM)
    {
        redde _cursum_factum(piscina, argc, argv);
    }
    si (argc >= III && strcmp(argv[II], "acta") == ZEPHYRUM)
    {
        redde _cursus_acta(piscina, argc, argv);
    }
    si (argc >= III && strcmp(argv[II], "censura") == ZEPHYRUM)
    {
        redde _censura(piscina, argc, argv);
    }
    fprintf(stderr,
        "usus: mensor cursus currere [-titulus T] -- mandatum ...\n"
        "      mensor cursus legere [-machina] [-volumen via]\n"
        "      mensor cursus incipere -titulus T [-argv S]\n"
        "      mensor cursus finire -id X -exitus N [-duratio_ms D]"
        " [-nota S] [-titulus T -acta via|-]\n"
        "      mensor cursus factum -titulus T -valor N [-unitas U]\n"
        "      mensor cursus acta <id>\n"
        "      mensor cursus censura [-radix via]\n");
    redde 2;
}


/* ============================================================
 * quaestio (analytica T5): quaestiones nominatae (lib/mensura_quaestio)
 * ============================================================ */

/* parametra 'par=valor' cognita - aliud = recusatio nominata */
hic_manens constans character* constans PARAMETRA_QUAESTIONIS[] = {
    "titulus", "arbor", "ramus", "nota", "status", "post", "n", "id",
    "profunditas", "radix", "praefixum", "fenestra", "limes",
        "colloquium",
    NIHIL
};

nomen structura {
         CursusFiltrum  filtrum;
                chorda  id;
                   i32  profunditas;
    constans character* radix;
                   i32  fenestra;
                   i32  limes_permille;
                chorda  colloquium;   /* currentes (colloquia T9) */
} Parametra;

interior b32
_parametra_legere (
      integer   argc,
    character** argv,
    Parametra*  p,
      Piscina*  piscina)
{
    integer i;

    memset(p, ZEPHYRUM, magnitudo(Parametra));
    p->radix           = ".";
    p->fenestra        = V;
    p->limes_permille  = MCCL;
    per (i = III; i < argc; i++)
    {
        constans character* signum = strchr(argv[i], '=');
                    chorda  valor;
                    chorda  nomen_par;
                       i32  k;
                       b32  notum = FALSUM;

        si (argv[i][ZEPHYRUM] == '-')
        {
            perge;   /* vexilla (-machina) */
        }
        si (signum == NIHIL)
        {
            fprintf(stderr, "mensor quaestio: '%s' non 'par=valor'\n",
                argv[i]);
            redde FALSUM;
        }
        nomen_par = chorda_ex_buffer((i8*)argv[i], (i32)(signum
            - argv[i]));
        valor = chorda_ex_literis(signum + I, piscina);
        per (k = ZEPHYRUM; PARAMETRA_QUAESTIONIS[k] != NIHIL; k++)
        {
            si (chorda_aequalis_literis(nomen_par,
                PARAMETRA_QUAESTIONIS[k]))
            {
                notum = VERUM;
            }
        }
        si (!notum)
        {
            fprintf(stderr, "mensor quaestio: parametrum ignotum '%.*s'"
                " (nota: titulus praefixum arbor ramus nota status post"
                " n"
                " id profunditas radix fenestra limes)\n",
                (integer)nomen_par.mensura,
                (constans character*)nomen_par.datum);
            redde FALSUM;
        }
        si (chorda_aequalis_literis(nomen_par, "titulus"))
        {
            p->filtrum.titulus = valor;
        }
        alioquin si (chorda_aequalis_literis(nomen_par, "arbor"))
        {
            p->filtrum.arbor = valor;
        }
        alioquin si (chorda_aequalis_literis(nomen_par, "ramus"))
        {
            p->filtrum.ramus = valor;
        }
        alioquin si (chorda_aequalis_literis(nomen_par, "nota"))
        {
            p->filtrum.nota = valor;
        }
        alioquin si (chorda_aequalis_literis(nomen_par, "status"))
        {
            p->filtrum.status = chorda_aequalis_literis(valor,
                "transiit")
                ? CURSUS_MASCULA_TRANSIIT
                : chorda_aequalis_literis(valor, "fractus")
                ? CURSUS_MASCULA_FRACTUS
                : chorda_aequalis_literis(valor, "interruptus")
                ? CURSUS_MASCULA_INTERRUPTUS
                : chorda_aequalis_literis(valor, "fracta")
                ? (CURSUS_MASCULA_FRACTUS | CURSUS_MASCULA_INTERRUPTUS)
                : ZEPHYRUM;
            si (p->filtrum.status == ZEPHYRUM)
            {
                fprintf(stderr, "mensor quaestio: status '%s' ignotus"
                    " (transiit fractus interruptus fracta)\n", signum
                        + I);
                redde FALSUM;
            }
        }
        alioquin si (chorda_aequalis_literis(nomen_par, "post"))
        {
            p->filtrum.post_ms = mensura_momentum_nunc() - (s64)strtol(
                signum + I, NIHIL, X) * (s64)(LXXXVI * M + CD) * (s64)M;
        }
        alioquin si (chorda_aequalis_literis(nomen_par, "n"))
        {
            p->filtrum.numerus = (i32)strtol(signum + I, NIHIL, X);
        }
        alioquin si (chorda_aequalis_literis(nomen_par, "id"))
        {
            p->id = valor;
        }
        alioquin si (chorda_aequalis_literis(nomen_par, "colloquium"))
        {
            p->colloquium = valor;
        }
        alioquin si (chorda_aequalis_literis(nomen_par, "profunditas"))
        {
            p->profunditas = (i32)strtol(signum + I, NIHIL, X);
        }
        alioquin si (chorda_aequalis_literis(nomen_par, "praefixum"))
        {
            p->filtrum.praefixum = valor;
        }
        alioquin si (chorda_aequalis_literis(nomen_par, "fenestra"))
        {
            p->fenestra = (i32)strtol(signum + I, NIHIL, X);
        }
        alioquin si (chorda_aequalis_literis(nomen_par, "limes"))
        {
            /* ratio decimalis ('1.25') -> permille, sine fluitantibus
             */
            constans character* q           = signum + I;
                           i32  integra     = (i32)strtol(q, NIHIL, X);
                           i32  millesimae  = ZEPHYRUM;
                           i32  pondus      = C;
            constans character* punctum     = strchr(q, '.');

            si (punctum != NIHIL)
            {
                per (punctum++; *punctum >= '0' && *punctum <= '9'
                     && pondus > ZEPHYRUM; punctum++)
                {
                    millesimae  += (i32)(*punctum - '0') * pondus;
                    pondus      /= X;
                }
            }
            p->limes_permille = integra * (i32)M + millesimae;
        }
        alioquin
        {
            p->radix = signum + I;
        }
    }
    redde VERUM;
}

interior vacuum
_tempus_iso (
          s64  ms,
    character* tabula)
{
         time_t secunda  = (time_t)(ms / (s64)M);
    structura tm* t      = gmtime(&secunda);

    si (t == NIHIL)
    {
        sprintf(tabula, "?");
        redde;
    }
    sprintf(tabula, "%04d-%02d-%02dT%02d:%02d:%02dZ", t->tm_year + MCM,
        t->tm_mon + I, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
}

interior constans character*
_statum (
    CursusStatus status)
{
    redde _status_verbum(status);
}

interior vacuum
_cursum_imprimere (
    constans Cursus* c,
                b32  machina,
            Piscina* piscina)
{
    character tempus[XXXII];

    si (machina)
    {
        imprimere("HISTORIA\t%.*s\t%.*s\t%s\t%d\t%ld\t%ld\t%.*s\t%.*s"
            "\t%.*s"
            "\t%s\n", (integer)c->initium.id.mensura,
            (constans character*)c->initium.id.datum,
            (integer)c->initium.titulus.mensura,
            (constans character*)c->initium.titulus.datum,
            _statum(c->status), (integer)c->finis.exitus,
            (longus)c->initium.initium, (longus)c->finis.duratio_ms,
            (integer)c->initium.arbor.mensura,
            (constans character*)c->initium.arbor.datum,
            (integer)c->initium.ramus.mensura,
            (constans character*)c->initium.ramus.datum,
            (integer)c->initium.commissum.mensura,
            (constans character*)c->initium.commissum.datum,
            _visum(c->finis.nota, piscina));
        redde;
    }
    _tempus_iso(c->initium.initium, tempus);
    /* %.*s cum mensura - chorda non terminata (et fortasse NIHIL) */
    imprimere("%s  %-11s %-40.*s %8ld ms  %.*s@%.*s  %.*s  %s\n",
        tempus,
        _statum(c->status), (integer)c->initium.titulus.mensura,
        (constans character*)c->initium.titulus.datum,
        (longus)c->finis.duratio_ms, (integer)c->initium.arbor.mensura,
        (constans character*)c->initium.arbor.datum,
        (integer)c->initium.ramus.mensura,
        (constans character*)c->initium.ramus.datum,
        (integer)(c->initium.commissum.mensura < X
            ? c->initium.commissum.mensura : X),
        (constans character*)c->initium.commissum.datum,
        _visum(c->finis.nota, piscina));
}

/* SCRIPTUM: '*.sh' aut plagula sine puncto cuius linea prima '#!'
 * concham nominat (unci git: pre-commit, post-commit - sine suffixo) */
interior b32
_scriptum_est (
     constans character* plena,
                 chorda  titulus)
{
         FILE* f;
     character  caput[LXIV];
           i32  k;

    si (   titulus.mensura > III
        && memcmp(titulus.datum + titulus.mensura - III, ".sh",
        (size_t)III)
           == ZEPHYRUM)
    {
        redde VERUM;
    }
    per (k = ZEPHYRUM; k < titulus.mensura; k++)
    {
        si (titulus.datum[k] == '.')
        {
            redde FALSUM;
        }
    }
    f = fopen(plena, "rb");
    si (f == NIHIL)
    {
        redde FALSUM;
    }
    k = (i32)fread(caput, (size_t)I, magnitudo(caput) - I, f);
    fclose(f);
    caput[k] = '\0';
    redde caput[ZEPHYRUM] == '#' && caput[I] == '!'
        && (strstr(caput, "sh\n") != NIHIL
        || strstr(caput, "bash") != NIHIL);
}

/* *.sh sub radice (build, .git, probationes/fixa omissis): titulus
 * annotationis aut via relativa -> universum; via per titulum */
interior vacuum
_scripta_colligere (
    constans character* radix,
    constans character* relativa,
                   Xar* universum,
        TabulaDispersa* viae,
               Piscina* piscina)
{
               character  via[IV * MXXIV];
     DirectoriumIterator* it;
    DirectoriumIntroitus* introitus;

    sprintf(via, "%s%s%s", radix, relativa[ZEPHYRUM] ? "/" : "",
        relativa);
    it = directorium_iterator_aperire(via, piscina);
    si (it == NIHIL)
    {
        redde;
    }
    dum ((introitus = directorium_iterator_proximum(it)) != NIHIL)
    {
        character sub[IV * MXXIV];
        character plena[IV * MXXIV];

        si (   introitus->titulus.mensura         == ZEPHYRUM
            || introitus->titulus.datum[ZEPHYRUM] == '.')
        {
            perge;
        }
        sprintf(sub, "%s%s%.*s", relativa,
            relativa[ZEPHYRUM] ? "/" : "",
            (integer)introitus->titulus.mensura,
            (constans character*)introitus->titulus.datum);
        si (introitus->genus == INTROITUS_DIRECTORIUM)
        {
            si (   strcmp(sub, "build")            != ZEPHYRUM
                && strstr(sub, "/build")           == NIHIL
                && strcmp(sub, "probationes/fixa") != ZEPHYRUM)
            {
                _scripta_colligere(radix, sub, universum, viae,
                    piscina);
            }
        }
        alioquin si (_scriptum_est((sprintf(plena, "%s/%s", radix, sub),
                         plena), introitus->titulus))
        {
            chorda titulus;
            chorda relativa_chorda =
                chorda_transcribere(chorda_ex_literis(
                sub, piscina), piscina);

            sprintf(plena, "%s/%s", radix, sub);
            titulus = _titulum_annotationis(plena, piscina);
            si (titulus.mensura == ZEPHYRUM)
            {
                titulus = relativa_chorda;
            }
            *(chorda*)xar_addere(universum) = titulus;
            (vacuum)tabula_dispersa_inserere(viae, titulus,
                piscina_allocare(piscina, magnitudo(chorda)));
            {
                vacuum* locus = NIHIL;

                (vacuum)tabula_dispersa_invenire(viae, titulus, &locus);
                si (locus != NIHIL)
                {
                    *(chorda*)locus = relativa_chorda;
                }
            }
        }
    }
    directorium_iterator_claudere(it);
}

/* USUS QUAESTIONUM (2026-10-15): 'mensor quaestio' solum nomina
 * dabat, nec quid quaeque respondeat nec quae parametra valeant -
 * 'limes=5' pro 'n=5' tacite neglectum, titulus nudus pro
 * 'probationes_radicis/<probatio>' tacite vacuus */
/* CURRENTES (colloquia T8d): quid NUNC currit. Vita custodis hic
 * iudicatur (bibliotheca pura est): processus_vivus, et processus POST
 * cursum natus = pid iterum datus (fabrica furcat assidue). Radix
 * mortua cum arbore sua celatur et numeratur; radix sine custode (ante
 * T8) per diem ostenditur, deinde celatur. -machina omnes reddit cum
 * columna vitae. */
#define CURRENTES_IGNOTI_MS ((s64)LXXXVI * (s64)M * (s64)M \
    + (s64)CD * (s64)M)

interior CursusVita
_vitam_iudicare (
    constans CursusCurrens* r,
                   Piscina* piscina)
{
    s64 natus = (s64)ZEPHYRUM;

    si (r->custos <= ZEPHYRUM)
    {
        redde CURSUS_VITA_IGNOTA;
    }
    si (!processus_vivus(r->custos, &natus, piscina))
    {
        redde CURSUS_VITA_MORTUA;
    }
    /* etime secundis tantum: II s laxitas */
    si (natus > r->cursus->initium.initium + (s64)MM)
    {
        redde CURSUS_VITA_MORTUA;
    }
    redde CURSUS_VITA_VIVA;
}

/* mora humana: "15h11m", "4m12s" aut "37s" */
interior vacuum
_moram_scribere (
          s64  ms,
    character* alveus)
{
    s64 secundae = (ms > (s64)ZEPHYRUM) ? ms / (s64)M : (s64)ZEPHYRUM;

    si (secundae >= (s64)MMMDC)
    {
        sprintf(alveus, "%ldh%02ldm", (longus)(secundae / (s64)MMMDC),
            (longus)((secundae % (s64)MMMDC) / (s64)LX));
    }
    alioquin si (secundae >= (s64)LX)
    {
        sprintf(alveus, "%ldm%02lds", (longus)(secundae / (s64)LX),
            (longus)(secundae % (s64)LX));
    }
    alioquin
    {
        sprintf(alveus, "%lds", (longus)secundae);
    }
}

/* segmentum ultimum viae arboris (rhubarb-secunda) */
interior chorda
_arboris_nomen (
    chorda arbor)
{
    i32 k;

    per (k = arbor.mensura; k > ZEPHYRUM; k--)
    {
        si (arbor.datum[k - I] == '/')
        {
            arbor.datum    += k;
            arbor.mensura  -= k;
            frange;
        }
    }
    redde arbor;
}

interior vacuum
_currentem_imprimere (
    constans CursusCurrens* r,
                CursusVita  vita,
                       s64  nunc,
                       b32  machina,
                   Piscina* piscina)
{
    constans CursusInitium* i = &r->cursus->initium;
                 character  mora[XXXII];
                 character  minima[XXXII];
                 character  maxima[XXXII];
                    chorda  arbor = _arboris_nomen(i->arbor);
                    chorda  fracta;
                       i32  k;

    si (machina)
    {
        /* forma in bibliotheca (T10a): contractus cum lectoribus */
        chorda linea = cursus_currentem_ad_lineam(r, vita, piscina);

        (vacuum)fwrite(linea.datum, (size_t)I, (size_t)linea.mensura,
            stdout);
        redde;
    }
    _moram_scribere(nunc - i->initium, mora);
    imprimere("%*s%.*s", (integer)(II * r->profunditas), "",
        (integer)i->titulus.mensura,
        (constans character*)i->titulus.datum);
    si (r->profunditas == ZEPHYRUM)
    {
        imprimere("  %.*s@%.*s", (integer)arbor.mensura,
            (constans character*)arbor.datum, (integer)i->ramus.mensura,
            (constans character*)i->ramus.datum);
        si (r->colloquium.mensura > ZEPHYRUM)
        {
            /* praefixum VIII: satis ad sessionem in $sessions noscendam */
            imprimere("  [colloquium %.*s]", (integer)(
                r->colloquium.mensura < VIII ? r->colloquium.mensura
                : VIII), (constans character*)r->colloquium.datum);
        }
    }
    imprimere("  %s", mora);
    si (r->debiti > ZEPHYRUM)
    {
        imprimere("  portae %d/%d", (integer)r->perfecti,
            (integer)r->debiti);
    }
    si (r->ambitus > ZEPHYRUM)
    {
        imprimere("  membra %d/%d iudicata, %d acta",
            (integer)r->iudicata,
            (integer)r->ambitus, (integer)r->perfecti);
    }
    si (r->fracti > ZEPHYRUM)
    {
        imprimere(", %d fracta", (integer)r->fracti);
    }
    si (r->maxima_ms > (s64)ZEPHYRUM)
    {
        _moram_scribere(r->minima_ms, minima);
        _moram_scribere(r->maxima_ms, maxima);
        imprimere("  (ultimi V: %s-%s)", minima, maxima);
    }
    si (r->profunditas == ZEPHYRUM)
    {
        si (vita == CURSUS_VITA_VIVA)
        {
            imprimere("  (custos %d)", (integer)r->custos);
        }
        alioquin
        {
            imprimere("  (custos ignotus)");
        }
    }
    imprimere("\n");
    si (r->fracti == ZEPHYRUM)
    {
        redde;
    }
    /* fracta nominata (T10a): r->fracta, spatiis iuncta */
    fracta = r->fracta;
    dum (fracta.mensura > ZEPHYRUM)
    {
        per (k = ZEPHYRUM; k < fracta.mensura && fracta.datum[k] != ' ';
             k++)
        {
        }
        imprimere("%*sfractum: %.*s\n", (integer)(II * (r->profunditas
            + I)), "", (integer)k, (constans character*)fracta.datum);
        k               = (k < fracta.mensura) ? k + I : k;
        fracta.datum    += k;
        fracta.mensura  -= k;
    }
}

interior integer
_currentes_imprimere (
    constans CursusIndex* index,
           constans Xar* mensurae,
    constans CursusFiltrum* filtrum,
                  chorda  colloquium,
                    b32  machina,
                Piscina* piscina)
{
             Xar* ordines;
             s64  nunc        = mensura_momentum_nunc();
      CursusVita  vita        = CURSUS_VITA_IGNOTA;
             b32  monstranda  = FALSUM;
             i32  mortui      = ZEPHYRUM;
             i32  vetusti     = ZEPHYRUM;
             i32  monstrati   = ZEPHYRUM;
             i32  k;

    ordines = cursus_quaerere_currentes(index, mensurae, filtrum,
        piscina);
    si (ordines == NIHIL)
    {
        fprintf(stderr,
            "mensor quaestio currentes: legi non possunt\n");
        redde I;
    }
    per (k = ZEPHYRUM; k < xar_numerus(ordines); k++)
    {
        constans CursusCurrens* r =
            (constans CursusCurrens*)xar_obtinere(
            ordines, k);

        si (r->profunditas == ZEPHYRUM)
        {
            /* colloquium= (T9): arbores aliarum sessionum nec
             * ostenduntur nec numerantur */
            si (   colloquium.mensura > ZEPHYRUM
                && !chorda_aequalis(r->colloquium, colloquium))
            {
                monstranda = FALSUM;
                perge;
            }
            vita = _vitam_iudicare(r, piscina);
            monstranda = machina || vita == CURSUS_VITA_VIVA
                || (   vita == CURSUS_VITA_IGNOTA
                    && nunc - r->cursus->initium.initium
                       < CURRENTES_IGNOTI_MS);
            si (!monstranda)
            {
                si (vita == CURSUS_VITA_MORTUA)
                {
                    mortui++;
                }
                alioquin
                {
                    vetusti++;
                }
                perge;
            }
            monstrati++;
        }
        si (monstranda)
        {
            _currentem_imprimere(r, vita, nunc, machina,
                piscina);
        }
    }
    si (machina)
    {
        redde ZEPHYRUM;
    }
    si (monstrati == ZEPHYRUM)
    {
        imprimere("nullus cursus currit\n");
    }
    si (mortui > ZEPHYRUM || vetusti > ZEPHYRUM)
    {
        imprimere("celati: %d mortui (custos abiit), %d sine custode ultra"
            " diem - mensor quaestio fracta status=interruptus\n",
            (integer)mortui, (integer)vetusti);
    }
    redde ZEPHYRUM;
}

interior vacuum
_quaestionum_usum_imprimere (
    FILE* f)
{
    fputs(
        "usus: mensor quaestio <nomen> par=valor ... [-machina]\n"
        "  historia      cursus singuli, recentissimi primi\n"
        "  ultimum       cursus ultimus\n"
        "  fracta        cursus fracti aut interrupti\n"
        "  acta          quod cursus effudit (id= aut ultimus)\n"
        "  tendentia     mediana per commissum (tardiorne?)\n"
        "  lentescentes  quae constanter tardescunt\n"
        "  sectiones     sectiones intra cursum (membrum#)\n"
        "  filii         cursus filii (ubi tempus ivit?)\n"
        "  summa         per titulum: numerus, ms, parentes\n"
        "  statistica    per titulum: n, mediana, max, fracti\n"
        "                (fluctuatne X?)\n"
        "  iudicia       regenerationes iudicis fabricae\n"
        "  commissiones  commissiones cum portis suis\n"
        "  currentes     quid NUNC currit (cursus aperti, custos vivus)\n"
        "  numquam       scripta declarata numquam cursa\n"
        "  discus        quod in diurna spatium tenet\n"
        "parametra: titulus= (aequalis) praefixum= (incipit)\n"
        "  n= (ordines maximi) status= post= nota= arbor=\n"
        "  ramus= id= profunditas= radix= fenestra=\n"
        "  limes= (lentescentes: per mille - NON ordines)\n"
        "tituli: probationes_radicis/<probatio>,\n"
        "  <compositum>/<probatio>, porta:<nomen>,\n"
        "  pythonica#<grex>, commissio\n"
        "  (tituli omnes: mensor quaestio summa)\n",
        f);
}

/* NIHIL CONGRUIT (2026-10-15): quaestio vacua cum filtro tituli causam
 * nominat in stderr, homini solum (-machina vacua, exitus idem) */
interior vacuum
_nihil_congruit_nominare (
       constans CursusIndex* index,
     constans CursusFiltrum* filtrum,
                    Piscina* piscina)
{
       Xar* similes;
       i32  noti = ZEPHYRUM;
       i32  k;
    chorda  quaesitum;

    similes = cursus_quaerere_similes(index, filtrum, III, &noti,
        piscina);
    si (similes == NIHIL)
    {
        redde;
    }
    quaesitum = filtrum->titulus.mensura > ZEPHYRUM ? filtrum->titulus
        : filtrum->praefixum;
    si (noti > ZEPHYRUM)
    {
        fprintf(stderr, "mensor quaestio: nihil congruit - '%.*s' %d"
            " cursus habet, nullus filtris ceteris (status, post, nota,"
            " arbor, ramus)\n", (integer)quaesitum.mensura,
            (constans character*)quaesitum.datum, (integer)noti);
        redde;
    }
    fprintf(stderr, "mensor quaestio: nihil congruit - titulus '%.*s'"
        " nullum cursum habet", (integer)quaesitum.mensura,
        (constans character*)quaesitum.datum);
    si (xar_numerus(similes) == ZEPHYRUM)
    {
        fputs(", nullus similis (tituli omnes: mensor quaestio"
            " summa)\n", stderr);
        redde;
    }
    fputs("; simillimi:", stderr);
    per (k = ZEPHYRUM; k < xar_numerus(similes); k++)
    {
        CursusSumma* s = (CursusSumma*)xar_obtinere(similes, k);

        fprintf(stderr, "%s %.*s (n=%d)", k > ZEPHYRUM ? "," : "",
            (integer)s->titulus.mensura,
            (constans character*)s->titulus.datum, (integer)s->numerus);
    }
    fputc('\n', stderr);
}

interior integer
_quaestio (
      Piscina*  piscina,
      integer   argc,
    character** argv)
{
     constans character* nomen_q = argc >= III ? argv[II] : "";
                    b32  machina = _vexillum(argc, argv, "-machina");
              Parametra  par;
                Volumen* volumen;
                    Xar* cursus;
            CursusIndex* index;
                    Xar* ordines = NIHIL;
                    i32  k;

    si (argc < III)
    {
        _quaestionum_usum_imprimere(stderr);
        redde 2;
    }
    si (!_parametra_legere(argc, argv, &par, piscina))
    {
        redde 2;
    }
    si (strcmp(nomen_q, "discus") == ZEPHYRUM)
    {
        Diurna* diurna = mensura_diurna_aperire(piscina);

        ordines = (diurna != NIHIL) ? cursus_quaerere_discus(
            diurna_introitus(diurna, piscina), piscina) : NIHIL;
        per (k = ZEPHYRUM; ordines != NIHIL && k < xar_numerus(ordines);
             k++)
        {
            CursusSumma* s = (CursusSumma*)xar_obtinere(ordines, k);

            si (   par.filtrum.numerus > ZEPHYRUM
                && k >= par.filtrum.numerus)
            {
                frange;
            }
            imprimere(machina ? "DISCUS\t%.*s\t%d\t%ld\n"
                : "%-60.*s  n=%d  %ld octeti\n",
                (integer)s->titulus.mensura,
                (constans character*)s->titulus.datum,
                (integer)s->numerus,
                (longus)s->octeti);
        }
        redde ZEPHYRUM;
    }
    volumen = volumen_aperire_aut_creare(piscina,
        _litterae(_volumen_via(
        argc, argv, piscina), piscina));
    si (volumen == NIHIL)
    {
        fprintf(stderr,
            "mensor quaestio: volumen aperiri non potest\n");
        redde I;
    }
    (vacuum)_cursus_condere(volumen, piscina);
    cursus = mensura_cursus_legere(volumen, piscina);
    index = (cursus != NIHIL) ? cursus_indicem_struere(cursus, piscina)
        : NIHIL;
    si (index == NIHIL)
    {
        fprintf(stderr, "mensor quaestio: cursus legi non possunt\n");
        volumen_claudere(volumen);
        redde I;
    }
    si (strcmp(nomen_q, "currentes") == ZEPHYRUM)
    {
        integer codex = _currentes_imprimere(index,
            mensura_mensuras_legere(volumen, chorda_ex_literis("",
            piscina), piscina), &par.filtrum, par.colloquium, machina,
            piscina);

        volumen_claudere(volumen);
        redde codex;
    }
    si (   strcmp(nomen_q, "historia") == ZEPHYRUM
        || strcmp(nomen_q, "ultimum")  == ZEPHYRUM
        || strcmp(nomen_q, "fracta")   == ZEPHYRUM
        || strcmp(nomen_q, "acta")     == ZEPHYRUM)
    {
        si (   strcmp(nomen_q, "ultimum") == ZEPHYRUM
            || strcmp(nomen_q, "acta")    == ZEPHYRUM)
        {
            par.filtrum.numerus = I;
        }
        si (   strcmp(nomen_q, "fracta") == ZEPHYRUM
            && par.filtrum.status        == ZEPHYRUM)
        {
            par.filtrum.status = CURSUS_MASCULA_FRACTUS
                | CURSUS_MASCULA_INTERRUPTUS;
        }
        si (   strcmp(nomen_q, "acta") == ZEPHYRUM
            && par.id.mensura > ZEPHYRUM)
        {
            Diurna* diurna  = mensura_diurna_aperire(piscina);
      DiurnaStatus  st      = DIURNA_IGNOTUM;
            chorda  corpus = diurna != NIHIL ? diurna_legere(diurna,
                par.id,
                piscina, &st) : par.id;

            volumen_claudere(volumen);
            si (st != DIURNA_ADEST)
            {
                fprintf(stderr, "mensor quaestio acta: %s\n",
                    diurna_status_nomen(st));
                redde I;
            }
            fwrite(corpus.datum, (size_t)I, (size_t)corpus.mensura,
                stdout);
            redde ZEPHYRUM;
        }
        ordines = cursus_quaerere_historia(index, &par.filtrum,
            piscina);
        si (strcmp(nomen_q, "acta") == ZEPHYRUM)
        {
                      Diurna* diurna  = mensura_diurna_aperire(piscina);
                DiurnaStatus  st      = DIURNA_IGNOTUM;
             constans Cursus* c;
                      chorda  corpus;

            volumen_claudere(volumen);
            si (ordines == NIHIL || xar_numerus(ordines) == ZEPHYRUM)
            {
                fprintf(stderr, "mensor quaestio acta: nullus cursus"
                    " congruit\n");
                redde I;
            }
            c = *(constans Cursus**)xar_obtinere(ordines, ZEPHYRUM);
            corpus = diurna != NIHIL ? diurna_legere(diurna,
                c->initium.id,
                piscina, &st) : c->initium.id;
            si (st != DIURNA_ADEST)
            {
                fprintf(stderr, "mensor quaestio acta %.*s: %s\n",
                    (integer)c->initium.id.mensura,
                    (constans character*)c->initium.id.datum,
                    diurna_status_nomen(st));
                redde I;
            }
            fwrite(corpus.datum, (size_t)I, (size_t)corpus.mensura,
                stdout);
            redde ZEPHYRUM;
        }
        per (k = ZEPHYRUM; ordines != NIHIL && k < xar_numerus(ordines);
             k++)
        {
            _cursum_imprimere(*(constans Cursus**)xar_obtinere(ordines,
                k),
                machina, piscina);
        }
    }
    alioquin si (strcmp(nomen_q, "tendentia") == ZEPHYRUM)
    {
        ordines = cursus_quaerere_tendentia(index, &par.filtrum,
            piscina);
        per (k = ZEPHYRUM; ordines != NIHIL && k < xar_numerus(ordines);
             k++)
        {
            CursusTendentia* t = (CursusTendentia*)xar_obtinere(ordines,
                k);
                  character tempus[XXXII];

            _tempus_iso(t->primum, tempus);
            si (machina)
            {
                imprimere("TENDENTIA\t%.*s\t%s\t%d\t%ld\t%ld\t%ld"
                    "\t%ld\n",
                    (integer)t->commissum.mensura,
                    (constans character*)t->commissum.datum,
                    t->per_commissionem ? "commissio" : "arbor",
                    (integer)t->numerus, (longus)t->mediana_ms,
                    (longus)t->minima_ms, (longus)t->maxima_ms,
                    (longus)t->primum);
            }
            alioquin
            {
                /* signum: '*' = commissum a commissione PARTUM; '+'
                 * = arbor sordida super commissum (clavis '+' finitur -
                 * praefixum X characterum id celaret) */
                i32 longitudo = t->commissum.mensura;
                b32 sordida = longitudo > ZEPHYRUM
                    && t->commissum.datum[longitudo - I] == '+';

                si (sordida)
                {
                    longitudo--;
                }
                imprimere("%-10.*s %s n=%-4d mediana %8ld ms"
                    "  [%ld..%ld]"
                    "  primum %s\n", (integer)(longitudo < X ? longitudo
                    : X), (constans character*)t->commissum.datum,
                    t->per_commissionem ? "*" : sordida ? "+" : " ",
                    (integer)t->numerus, (longus)t->mediana_ms,
                    (longus)t->minima_ms, (longus)t->maxima_ms, tempus);
            }
        }
    }
    alioquin si (strcmp(nomen_q, "filii") == ZEPHYRUM)
    {
        si (par.id.mensura == ZEPHYRUM)
        {
            Xar* ultimi;

            par.filtrum.numerus = I;
            ultimi = cursus_quaerere_historia(index, &par.filtrum,
                piscina);
            si (ultimi == NIHIL || xar_numerus(ultimi) == ZEPHYRUM)
            {
                fprintf(stderr,
                    "mensor quaestio filii: id= aut titulus="
                    " cursus exstantis necessarium\n");
                volumen_claudere(volumen);
                redde 2;
            }
            par.id = (*(constans Cursus**)xar_obtinere(ultimi,
                ZEPHYRUM))
                ->initium.id;
        }
        ordines = cursus_quaerere_filii(index, par.id, par.profunditas,
            piscina);
        per (k = ZEPHYRUM; ordines != NIHIL && k < xar_numerus(ordines);
             k++)
        {
            CursusNodus* n = (CursusNodus*)xar_obtinere(ordines, k);

            si (machina)
            {
                imprimere("FILIUS\t%d\t%.*s\t%.*s\t%s\t%ld\t%d\t%s\n",
                    (integer)n->profunditas,
                    (integer)n->cursus->initium.id.mensura,
                    (constans character*)n->cursus->initium.id.datum,
                    (integer)n->cursus->initium.titulus.mensura,
                    (constans character*)
                    n->cursus->initium.titulus.datum,
                    _statum(n->cursus->status),
                    (longus)n->cursus->finis.duratio_ms,
                    (integer)n->pars,
                    _visum(n->cursus->finis.nota, piscina));
            }
            alioquin
            {
                imprimere("%*s%8ld ms %3d.%d%%  %.*s  %s  %.60s\n",
                    (integer)(n->profunditas * II), "",
                    (longus)n->cursus->finis.duratio_ms,
                    (integer)(n->pars / X), (integer)(n->pars % X),
                    (integer)n->cursus->initium.titulus.mensura,
                    (constans character*)
                    n->cursus->initium.titulus.datum,
                    _statum(n->cursus->status),
                    _visum(n->cursus->finis.nota, piscina));
            }
        }
    }
    alioquin si (   strcmp(nomen_q, "summa")   == ZEPHYRUM
                 || strcmp(nomen_q, "iudicia") == ZEPHYRUM)
    {
        si (strcmp(nomen_q, "iudicia") == ZEPHYRUM)
        {
            par.filtrum.nota = chorda_ex_literis("IUDICIUM", piscina);
        }
        /* numerus: ordines summae, non cursus - post ordinationem */
        {
            i32 numerus = par.filtrum.numerus;

            par.filtrum.numerus = ZEPHYRUM;
            ordines = cursus_quaerere_summa(index, &par.filtrum,
                piscina);
            par.filtrum.numerus = numerus;
        }
        per (k = ZEPHYRUM; ordines != NIHIL && k < xar_numerus(ordines);
             k++)
        {
            CursusSumma* s = (CursusSumma*)xar_obtinere(ordines, k);

            si (   par.filtrum.numerus > ZEPHYRUM
                && k >= par.filtrum.numerus)
            {
                frange;
            }
            si (machina)
            {
                imprimere("SUMMA\t%.*s\t%d\t%ld\t%d\n",
                    (integer)s->titulus.mensura,
                    (constans character*)s->titulus.datum,
                    (integer)s->numerus, (longus)s->ms_summa,
                    (integer)s->parentes);
            }
            alioquin
            {
                imprimere("%10ld ms  n=%-5d parentes=%-3d %.*s\n",
                    (longus)s->ms_summa, (integer)s->numerus,
                    (integer)s->parentes, (integer)s->titulus.mensura,
                    (constans character*)s->titulus.datum);
            }
        }
    }
    alioquin si (strcmp(nomen_q, "commissiones") == ZEPHYRUM)
    {
        ordines = cursus_quaerere_commissiones(index, &par.filtrum,
            piscina);
        per (k = ZEPHYRUM; ordines != NIHIL && k < xar_numerus(ordines);
             k++)
        {
            CursusCommissio* t = (CursusCommissio*)xar_obtinere(ordines,
                k);
             constans Cursus* c = t->commissio;
                   character  tempus[XXXII];
                      chorda  culpa_titulus = t->culpa != NIHIL
                          ? t->culpa->initium.titulus
                          : chorda_ex_literis("", piscina);
                      chorda culpa_nota = t->culpa != NIHIL
                          ? t->culpa->finis.nota : c->finis.nota;

            _tempus_iso(c->initium.initium, tempus);
            si (machina)
            {
                imprimere("COMMISSIO\t%.*s\t%ld\t%s\t%ld\t%.*s\t%.*s"
                    "\t%.*s"
                    "\t%.*s\t%s\n", (integer)c->initium.id.mensura,
                    (constans character*)c->initium.id.datum,
                    (longus)c->initium.initium, _statum(c->status),
                    (longus)c->finis.duratio_ms,
                    (integer)c->initium.arbor.mensura,
                    (constans character*)c->initium.arbor.datum,
                    (integer)c->initium.ramus.mensura,
                    (constans character*)c->initium.ramus.datum,
                    (integer)t->commissum.mensura,
                    (constans character*)t->commissum.datum,
                    (integer)culpa_titulus.mensura,
                    (constans character*)culpa_titulus.datum,
                    _visum(culpa_nota, piscina));
            }
            alioquin
            {
                imprimere("%s  %-11s %6ld s  %.*s@%.*s  %s%.*s%s%.*s"
                    "  %s\n",
                    tempus, _statum(c->status),
                    (longus)(c->finis.duratio_ms / (s64)M),
                    (integer)c->initium.arbor.mensura,
                    (constans character*)c->initium.arbor.datum,
                    (integer)c->initium.ramus.mensura,
                    (constans character*)c->initium.ramus.datum,
                    t->commissum.mensura > ZEPHYRUM ? "-> " : "",
                    (integer)t->commissum.mensura,
                    (constans character*)t->commissum.datum,
                    culpa_titulus.mensura > ZEPHYRUM ? "culpa: " : "",
                    (integer)culpa_titulus.mensura,
                    (constans character*)culpa_titulus.datum,
                    _visum(culpa_nota, piscina));
            }
        }
    }
    alioquin si (   strcmp(nomen_q, "statistica") == ZEPHYRUM
                 || strcmp(nomen_q, "sectiones")  == ZEPHYRUM)
    {
        /* 'sectiones titulus=M' = 'statistica praefixum=M#' */
        si (   strcmp(nomen_q, "sectiones") == ZEPHYRUM
            && par.filtrum.titulus.mensura > ZEPHYRUM)
        {
            par.filtrum.praefixum =
                chorda_concatenare(par.filtrum.titulus,
                chorda_ex_literis("#", piscina), piscina);
            par.filtrum.titulus = chorda_ex_literis("", piscina);
        }
        ordines = cursus_quaerere_statistica(index, &par.filtrum,
            piscina);
        per (k = ZEPHYRUM; ordines != NIHIL && k < xar_numerus(ordines);
             k++)
        {
            CursusStatistica* s =
                (CursusStatistica*)xar_obtinere(ordines,
                k);

            si (machina)
            {
                imprimere("STATISTICA\t%.*s\t%d\t%d\t%ld\t%ld\n",
                    (integer)s->titulus.mensura,
                    (constans character*)s->titulus.datum,
                    (integer)s->numerus, (integer)s->fracti,
                    (longus)s->mediana_ms, (longus)s->maxima_ms);
            }
            alioquin
            {
                imprimere("%8ld ms mediana  %8ld max  n=%-4d"
                    " fracti=%-3d"
                    " %.*s\n", (longus)s->mediana_ms,
                    (longus)s->maxima_ms,
                    (integer)s->numerus, (integer)s->fracti,
                    (integer)s->titulus.mensura,
                    (constans character*)s->titulus.datum);
            }
        }
    }
    alioquin si (strcmp(nomen_q, "lentescentes") == ZEPHYRUM)
    {
        ordines = cursus_quaerere_lentescentes(index, &par.filtrum,
            par.fenestra, par.limes_permille, piscina);
        per (k = ZEPHYRUM; ordines != NIHIL && k < xar_numerus(ordines);
             k++)
        {
            CursusLentescens* l =
                (CursusLentescens*)xar_obtinere(ordines,
                k);

            si (machina)
            {
                imprimere("LENTESCENS\t%.*s\t%d\t%ld\t%ld\t%d\t%d\n",
                    (integer)l->titulus.mensura,
                    (constans character*)l->titulus.datum,
                    (integer)l->priores, (longus)l->mediana_prior_ms,
                    (longus)l->mediana_recens_ms, (integer)l->supra,
                    (integer)l->ratio_permille);
            }
            alioquin
            {
                imprimere("x%d.%02d  %8ld -> %8ld ms  (ultimi %d supra"
                    " %d/%d, priores %d)  %.*s\n",
                    (integer)(l->ratio_permille / M),
                    (integer)((l->ratio_permille % M) / X),
                    (longus)l->mediana_prior_ms,
                    (longus)l->mediana_recens_ms, (integer)par.fenestra,
                    (integer)l->supra, (integer)par.fenestra,
                    (integer)l->priores, (integer)l->titulus.mensura,
                    (constans character*)l->titulus.datum);
            }
        }
    }
    alioquin si (strcmp(nomen_q, "numquam") == ZEPHYRUM)
    {
                    Xar* universum = xar_creare(piscina,
                        (i32)magnitudo(chorda));
         TabulaDispersa* viae = tabula_dispersa_creare_chorda(piscina,
                        (i32)CCLVI);

        si (universum != NIHIL && viae != NIHIL)
        {
            _scripta_colligere(par.radix, "", universum, viae, piscina);
        }
        ordines = cursus_quaerere_numquam(index, universum, piscina);
        per (k = ZEPHYRUM; ordines != NIHIL && k < xar_numerus(ordines);
             k++)
        {
            chorda  titulus  = *(chorda*)xar_obtinere(ordines, k);
            vacuum* locus    = NIHIL;
            chorda  via      = titulus;

            si (   tabula_dispersa_invenire(viae, titulus, &locus)
                && locus != NIHIL)
            {
                via = *(chorda*)locus;
            }
            imprimere(machina ? "NUMQUAM\t%.*s\n" : "%.*s\n",
                (integer)via.mensura, (constans character*)via.datum);
        }
    }
    alioquin
    {
        fprintf(stderr, "mensor quaestio: '%s' ignota\n", nomen_q);
        _quaestionum_usum_imprimere(stderr);
        volumen_claudere(volumen);
        redde 2;
    }
    /* homini solum: -machina vacua manet (consumptores stderr saepe
     * miscent - fumus mensor_cursus XI 'titulus=nemo -machina' = nihil) */
    si (   !machina
        && (ordines == NIHIL || xar_numerus(ordines) == ZEPHYRUM))
    {
        _nihil_congruit_nominare(index, &par.filtrum, piscina);
    }
    volumen_claudere(volumen);
    redde ZEPHYRUM;
}


/* ============================================================
 * censura (analytica T6): adoptores chassis versionem hodiernam ferunt
 * ============================================================ */

/* versio HODIERNA chassis: mutatio semanticae prologi/involucri ->
 * augenda; adoptores annotationem suam renovare debent (censura) */
#define CURSUS_VERSIO "I"

/* *.sh sub radice (build, .git, probationes/fixa, tools/cursus.sh ipso
 * omissis) - viae relativae */
interior vacuum
_scripta_enumerare (
    constans character* radix,
    constans character* relativa,
                   Xar* viae,
               Piscina* piscina)
{
               character  via[IV * MXXIV];
     DirectoriumIterator* it;
    DirectoriumIntroitus* introitus;

    sprintf(via, "%s%s%s", radix, relativa[ZEPHYRUM] ? "/" : "",
        relativa);
    it = directorium_iterator_aperire(via, piscina);
    si (it == NIHIL)
    {
        redde;
    }
    dum ((introitus = directorium_iterator_proximum(it)) != NIHIL)
    {
        character sub[IV * MXXIV];

        si (   introitus->titulus.mensura         == ZEPHYRUM
            || introitus->titulus.datum[ZEPHYRUM] == '.')
        {
            perge;
        }
        sprintf(sub, "%s%s%.*s", relativa,
            relativa[ZEPHYRUM] ? "/" : "",
            (integer)introitus->titulus.mensura,
            (constans character*)introitus->titulus.datum);
        si (introitus->genus == INTROITUS_DIRECTORIUM)
        {
            si (   strcmp(sub, "build")            != ZEPHYRUM
                && strstr(sub, "/build")           == NIHIL
                && strcmp(sub, "probationes/fixa") != ZEPHYRUM)
            {
                _scripta_enumerare(radix, sub, viae, piscina);
            }
        }
        alioquin si (strcmp(sub, "tools/cursus.sh") != ZEPHYRUM)
        {
            character plena_sub[IV * MXXIV];

            sprintf(plena_sub, "%s/%s", radix, sub);
            si (!_scriptum_est(plena_sub, introitus->titulus))
            {
                perge;
            }
            *(chorda*)xar_addere(viae) = chorda_transcribere(
                chorda_ex_literis(sub, piscina), piscina);
        }
    }
    directorium_iterator_claudere(it);
}

/* plagula: annotatio (linea '# <cursus ...' ) et prologus (linea non
 * commentaria 'source'/'.' cum tools/cursus.sh) */
interior vacuum
_scriptum_legere (
    constans character* via,
               Piscina* piscina,
                chorda* titulus_out,
                chorda* versio_out,
                   b32* annotatum_out,
                   b32* fontat_out)
{
       chorda textus   = filum_legere_totum(via, piscina);
          i32 initium  = ZEPHYRUM;
          i32 k;

    *annotatum_out  = FALSUM;
    *fontat_out     = FALSUM;
    *titulus_out    = chorda_ex_literis("", piscina);
    *versio_out     = chorda_ex_literis("", piscina);
    per (k = ZEPHYRUM; k <= textus.mensura; k++)
    {
        chorda linea;
        chorda nuda;
           i32 j = ZEPHYRUM;

        si (k < textus.mensura && textus.datum[k] != '\n')
        {
            perge;
        }
        linea    = chorda_sectio(textus, initium, k);
        initium  = k + I;
        dum (   j < linea.mensura
             && (linea.datum[j] == ' ' || linea.datum[j] == '\t'))
        {
            j++;
        }
        nuda = chorda_sectio(linea, j, linea.mensura);
        si (   !*annotatum_out
            && chorda_incipit(nuda, chorda_ex_literis("# <cursus ",
                   piscina)))
        {
            character* a = (character*)piscina_allocare(piscina,
                (memoriae_index)(nuda.mensura + I));

            si (a != NIHIL)
            {
                memcpy(a, nuda.datum, (size_t)nuda.mensura);
                a[nuda.mensura] = '\0';
                *titulus_out = _attributum_annotationis(a, "titulus",
                    piscina);
                *versio_out = _attributum_annotationis(a, "versio",
                    piscina);
            }
            *annotatum_out = VERUM;
        }
        alioquin si (   (   chorda_incipit(nuda, chorda_ex_literis(
                                "source ", piscina))
                         || chorda_incipit(nuda, chorda_ex_literis(". ",
                                piscina)))
                     && chorda_continet(nuda, chorda_ex_literis(
                            "cursus.sh", piscina)))
        {
            *fontat_out = VERUM;
        }
    }
}

interior integer
_censura (
      Piscina*  piscina,
      integer   argc,
    character** argv)
{
     constans character* radix = _arg(argc, argv, "-radix");
                    Xar* viae = xar_creare(piscina,
                        (i32)magnitudo(chorda));
         TabulaDispersa* tituli = tabula_dispersa_creare_chorda(piscina,
                    (i32)CXXVIII);
                    i32 k;
                    i32 utentes  = ZEPHYRUM;
                    i32 vitia    = ZEPHYRUM;

    si (radix == NIHIL)
    {
        radix = ".";
    }
    si (viae == NIHIL || tituli == NIHIL)
    {
        redde I;
    }
    _scripta_enumerare(radix, "", viae, piscina);
    per (k = ZEPHYRUM; k < xar_numerus(viae); k++)
    {
        chorda  relativa = *(chorda*)xar_obtinere(viae, k);
     character  plena[IV * MXXIV];
        chorda  titulus;
        chorda  versio;
           b32  annotatum;
           b32  fontat;
        vacuum* locus = NIHIL;

        sprintf(plena, "%s/%.*s", radix, (integer)relativa.mensura,
            (constans character*)relativa.datum);
        _scriptum_legere(plena, piscina, &titulus, &versio, &annotatum,
            &fontat);
        si (!annotatum && !fontat)
        {
            perge;
        }
        utentes++;
        si (fontat && !annotatum)
        {
            imprimere("VITIUM\t%.*s\tprologus sine annotatione"
                " '# <cursus"
                " titulus=... versio=\"%s\"/>'\n",
                (integer)relativa.mensura,
                (constans character*)relativa.datum, CURSUS_VERSIO);
            vitia++;
            perge;
        }
        si (annotatum && !fontat)
        {
            imprimere("VITIUM\t%.*s\tannotatio sine prologo (source"
                " tools/cursus.sh) - mortua\n",
                (integer)relativa.mensura,
                (constans character*)relativa.datum);
            vitia++;
        }
        si (!chorda_aequalis_literis(versio, CURSUS_VERSIO))
        {
            imprimere("VITIUM\t%.*s\tversio '%.*s', hodierna '%s'\n",
                (integer)relativa.mensura,
                (constans character*)relativa.datum,
                (integer)versio.mensura,
                (constans character*)versio.datum,
                CURSUS_VERSIO);
            vitia++;
        }
        si (titulus.mensura == ZEPHYRUM)
        {
            imprimere("VITIUM\t%.*s\tannotatio sine titulo\n",
                (integer)relativa.mensura,
                (constans character*)relativa.datum);
            vitia++;
        }
        alioquin si (tabula_dispersa_invenire(tituli, titulus, &locus))
        {
            chorda prior = *(chorda*)locus;

            imprimere("VITIUM\t%.*s\ttitulus '%.*s' iam in %.*s -"
                " identitas"
                " operis una\n", (integer)relativa.mensura,
                (constans character*)relativa.datum,
                (integer)titulus.mensura,
                (constans character*)titulus.datum,
                (integer)prior.mensura,
                (constans character*)prior.datum);
            vitia++;
        }
        alioquin
        {
            chorda* sedes = (chorda*)piscina_allocare(piscina,
                magnitudo(chorda));

            si (sedes != NIHIL)
            {
                *sedes = relativa;
                (vacuum)tabula_dispersa_inserere(tituli, titulus,
                    sedes);
            }
        }
    }
    imprimere("censura chassis (versio %s): adoptores %d, vitia %d\n",
        CURSUS_VERSIO, (integer)utentes, (integer)vitia);
    redde vitia > ZEPHYRUM ? I : ZEPHYRUM;
}


/* ============================================================
 * curare (analytica T6c): conditio + purgatio cotidiana, SEPARATIM
 * ============================================================ */

/* diarium maius quam hoc -> curatio debita (MENSOR_CURARE_LIMES
 * octeti); purgatio semel per diem (nota curatio.ultima) */
#define CURATIO_LIMES_DIARII (CCLVI * MXXIV)
#define CURATIO_INTERVALLUM_S ((longus)LXXXVI * (longus)M + (longus)CD)

interior constans character*
_curationis_via (
    constans character* nomen_plagulae,
               Piscina* piscina)
{
    character* via = (character*)piscina_allocare(piscina,
        (memoriae_index)DXII);

    si (via == NIHIL)
    {
        redde nomen_plagulae;
    }
    sprintf(via, "%s/%s", mensura_diurna_radix(piscina),
        nomen_plagulae);
    redde via;
}

/* nota purgationis ultimae vetustior quam dies (aut absens)? */
interior b32
_purgatio_debita (
    Piscina* piscina)
{
    structura stat  status;

    si (stat(_curationis_via("curatio.ultima", piscina), &status) != 0)
    {
        redde VERUM;
    }
    redde (longus)(time(NIHIL) - status.st_mtime)
        > CURATIO_INTERVALLUM_S;
}

/* debetne cursus summus curationem excitare? duo 'stat' soli - cursus
 * fere omnes nihil solvunt */
interior b32
_curatio_debita (
    Piscina* piscina)
{
    structura stat  status;
            longus limes = (longus)CURATIO_LIMES_DIARII;

    si (getenv("MENSOR_CURARE_NULLUS") != NIHIL)
    {
        redde FALSUM;
    }
    si (getenv("MENSOR_CURARE_LIMES") != NIHIL)
    {
        limes = strtol(getenv("MENSOR_CURARE_LIMES"), NIHIL, X);
    }
    si (   stat(mensura_diarium_cursuum(piscina), &status) == 0
        && (longus)status.st_size > limes)
    {
        redde VERUM;
    }
    redde _purgatio_debita(piscina);
}

/* curationem SEPARATIM excitare: fork + setsid, stdio ad /dev/null,
 * 'mensor curare' - scriptum vocatum statim redit (exemplar manus
 * _detrahere); orphanus a launchd metitur */
interior vacuum
_curare_separatim (
    constans character* mensor)
{
        pid_t  proles;
    character  verbum[XVI];
    character* argumenta[III];

    sprintf(verbum, "curare");
    argumenta[ZEPHYRUM]  = (character*)(memoriae_index)mensor;
    argumenta[I]         = verbum;
    argumenta[II]        = NIHIL;
    (vacuum)fflush(stdout);
    (vacuum)fflush(stderr);
    proles = fork();
    si (proles != 0)
    {
        redde;   /* parens (aut fork fractus): nihil exspectat */
    }
    (vacuum)setsid();
    {
        s32 fd = open("/dev/null", O_WRONLY);

        si (fd >= 0)
        {
            (vacuum)dup2(fd, I);
            (vacuum)dup2(fd, II);
            (vacuum)close(fd);
        }
    }
    si (strchr(mensor, '/') != NIHIL)
    {
        (vacuum)execv(mensor, argumenta);
    }
    alioquin
    {
        (vacuum)execvp(mensor, argumenta);
    }
    _exit(CXXVII);
}

/* regula retentionis (sectio purgare infra) */
interior b32
_regulam_legere (
     constans character* via,
           DiurnaRegula* regula,
                Piscina* piscina);

interior integer
_curare (
      Piscina*  piscina,
      integer   argc,
    character** argv)
{
               b32  vis = _vexillum(argc, argv, "-vis");
         FilumSera* sera;
     CursusInitium  initium;
       CursusFinis  finis;
           Volumen* volumen;
         character  nota[CCLVI];
         character  via_regulae[IV * MXXIV];
constans character* regula_via = getenv("MENSOR_RETENTIO");
               s64  ante;
    structura stat  status;
       longus octeti_diarii = ZEPHYRUM;

    /* sera NON exspectans: curatio altera currit -> nihil */
    (vacuum)filum_directorium_creare_cum_parentibus(
        mensura_diurna_radix(piscina));
    sera = filum_seram_capere(_curationis_via("curatio.sera", piscina),
        ZEPHYRUM, piscina);
    si (sera == NIHIL)
    {
        redde ZEPHYRUM;
    }
    memset(&initium, ZEPHYRUM, magnitudo(initium));
    memset(&finis, ZEPHYRUM, magnitudo(finis));
    mensura_contextum_parare(&initium, piscina);
    initium.titulus  = chorda_ex_literis("mensor:curare", piscina);
    initium.argv     = chorda_ex_literis(vis ? "-vis" : "", piscina);
    initium.id       = mensura_id_novum(piscina);
    initium.initium  = mensura_momentum_nunc();
    (vacuum)mensura_cursum_incipere(mensura_diarium_cursuum(piscina),
        &initium, piscina);
    ante = _horologium_ms();
    si (stat(mensura_diarium_cursuum(piscina), &status) == 0)
    {
        octeti_diarii = (longus)status.st_size;
    }
    /* I. conditio diarii (per renominationem: appensores non
     * perduntur) */
    volumen = volumen_aperire_aut_creare(piscina,
        _litterae(_volumen_via(
        argc, argv, piscina), piscina));
    si (volumen != NIHIL)
    {
        (vacuum)_cursus_condere(volumen, piscina);
        volumen_claudere(volumen);
    }
    sprintf(nota, "condita %ld octeti", octeti_diarii);
    /* II. purgatio semel per diem (aut -vis); regula: MENSOR_RETENTIO,
     * aliter arbor contextus, aliter cwd - absens = non purgatur */
    si (vis || _purgatio_debita(piscina))
    {
        DiurnaRegula  regula;
       DiurnaRelatio  relatio;
              Diurna* diurna;

        si (regula_via == NIHIL)
        {
            sprintf(via_regulae, "%s/tools/analytica_retentio.stml",
                _ambitus_aut("MENSOR_ARBOR", "."));
            regula_via = via_regulae;
        }
        diurna = mensura_diurna_aperire(piscina);
        si (!_regulam_legere(regula_via, &regula, piscina))
        {
            sprintf(nota + strlen(nota), "; regula absens (%.120s)",
                regula_via);
        }
        alioquin si (   diurna != NIHIL
                     && diurna_purgare(diurna, &regula,
                            mensura_momentum_nunc(), FALSUM, &relatio))
        {
            sprintf(nota + strlen(nota),
                "; purgata: compressa %d, deleta"
                " %d, liberati %ld", (integer)relatio.compressa,
                (integer)relatio.deleta,
                (longus)relatio.octeti_liberati);
        }
        alioquin
        {
            sprintf(nota + strlen(nota), "; purgatio fracta");
        }
        /* nota scribitur etiam sine regula: conatus semel per diem */
        (vacuum)filum_scribere_literis(_curationis_via("curatio.ultima",
            piscina), nota);
    }
    alioquin
    {
        sprintf(nota + strlen(nota), "; purgatio non debita");
    }
    finis.id          = initium.id;
    finis.momentum    = mensura_momentum_nunc();
    finis.duratio_ms  = _horologium_ms() - ante;
    finis.exitus      = ZEPHYRUM;
    finis.acta        = chorda_ex_literis("", piscina);
    finis.nota        = chorda_ex_literis(nota, piscina);
    (vacuum)mensura_cursum_finire(mensura_diarium_cursuum(piscina),
        &finis,
        piscina);
    imprimere("mensor curare: %s\n", nota);
    filum_seram_liberare(sera);
    redde ZEPHYRUM;
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
        "  mensor quaestio <nomen> par=valor ... [-machina]"
        "   (historia ultimum fracta acta tendentia filii\n"
        "      summa iudicia commissiones statistica sectiones"
        " lentescentes numquam discus)\n"
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
    alioquin si (strcmp(argv[I], "curare") == 0)
    {
        codex = _curare(piscina, argc, argv);
    }
    alioquin si (strcmp(argv[I], "quaestio") == 0)
    {
        codex = _quaestio(piscina, argc, argv);
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
