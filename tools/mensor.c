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
#include "git.h"
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
#define ACTA_MAXIMA (VIII * MXXIV * MXXIV)

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

interior chorda
_effusio_mandati (
    constans character* constans* argumenta,
    constans character*           directorium,
                       Piscina*   piscina,
                           b32*   successus_out)
{
     ProcessusOptiones optiones;
     ProcessusResultus r;

    memset(&optiones, ZEPHYRUM, magnitudo(optiones));
    optiones.directorium = directorium;
    r = processus_exsequi_cum(argumenta, &optiones, (i32)(X * M),
        piscina);
    *successus_out = r.successus && r.codex_exitus == ZEPHYRUM;
    dum (   r.effusio.mensura > ZEPHYRUM
         && (   r.effusio.datum[r.effusio.mensura - I] == '\n'
             || r.effusio.datum[r.effusio.mensura - I] == '\r'))
    {
        r.effusio.mensura--;
    }
    redde r.effusio;
}

/* radix arboris operis: ascendit ex cwd donec '.git' (directorium aut
 * plagula - arbor operis) inveniat; vacua si nulla */
interior chorda
_arborem_invenire (
    Piscina* piscina)
{
         character  via[IV * MXXIV];
         character  proba[IV * MXXIV + XVI];
         character* ultima;
    structura stat  status;

    si (getcwd(via, magnitudo(via)) == NIHIL)
    {
        redde chorda_ex_literis("", piscina);
    }
    dum (VERUM)
    {
        sprintf(proba, "%s/.git", via);
        si (stat(proba, &status) == ZEPHYRUM)
        {
            redde chorda_transcribere(chorda_ex_literis(via, piscina),
                piscina);
        }
        ultima = strrchr(via, '/');
        si (ultima == NIHIL || ultima == via)
        {
            redde chorda_ex_literis("", piscina);
        }
        *ultima = '\0';
    }
}

/* ramus ex HEAD repositorii ('ref: refs/heads/X') aut 'detached' */
interior chorda
_ramum_legere (
    GitRepositorium* repositorium,
            Piscina* piscina)
{
         character via[IV * MXXIV];
         character linea[DXII];
             FILE*  f;
    constans character* praefixum = "ref: refs/heads/";
    memoriae_index  longitudo;

    sprintf(via, "%s/HEAD", repositorium->via_git);
    f = fopen(via, "rb");
    si (f == NIHIL)
    {
        redde chorda_ex_literis("", piscina);
    }
    si (fgets(linea, (integer)magnitudo(linea), f) == NIHIL)
    {
        fclose(f);
        redde chorda_ex_literis("", piscina);
    }
    fclose(f);
    longitudo = strlen(linea);
    dum (   longitudo > ZEPHYRUM
         && (linea[longitudo - I] == '\n'
         || linea[longitudo - I] == '\r'))
    {
        linea[--longitudo] = '\0';
    }
    si (strncmp(linea, praefixum, strlen(praefixum)) != ZEPHYRUM)
    {
        redde chorda_ex_literis("detached", piscina);
    }
    redde chorda_transcribere(chorda_ex_literis(linea
        + strlen(praefixum),
        piscina), piscina);
}

/* chorda vacua datum NIHIL habet (allocatio nulla) et chorda_ut_cstr
 * NIHIL reddit - setenv(titulus, NIHIL) ruit (planta D T2, extra
 * repositorium). Litterae vacuae pro chorda vacua. */
interior constans character*
_litterae_aut_vacuae (
    chorda  s,
   Piscina* piscina)
{
    constans character* litterae = chorda_ut_cstr(s, piscina);

    redde (litterae != NIHIL) ? litterae : "";
}

/* CONTEXTUS: ex ambitu (hereditas) aut SEMEL computatus et in ambitum
 * positus - cursus interiores eum hereditant, git non iterum vocatur */
interior vacuum
_contextum_parare (
    CursusInitium* i,
          Piscina* piscina)
{
    constans character* commissum = getenv("MENSOR_COMMISSUM");

    si (commissum == NIHIL)
    {
                 chorda  arbor         = _arborem_invenire(piscina);
        GitRepositorium* repositorium  = NIHIL;
              character  sha[GIT_SHA_HEX_MENSURA + I];
                 chorda  ramus = chorda_ex_literis("", piscina);
                 chorda  status;
                    b32  bene = FALSUM;
              character  sordidum[II];
     constans character* status_argv[V];

        sha[ZEPHYRUM] = '\0';
        si (arbor.mensura > ZEPHYRUM)
        {
            repositorium = git_aperire(piscina, chorda_ut_cstr(arbor,
                piscina));
        }
        si (repositorium != NIHIL)
        {
            si (!git_ref_resolvere(repositorium, "HEAD", sha))
            {
                sha[ZEPHYRUM] = '\0';
            }
            ramus = _ramum_legere(repositorium, piscina);
        }
        status_argv[ZEPHYRUM]  = "git";
        status_argv[I]         = "status";
        status_argv[II]        = "--porcelain";
        status_argv[III]       = "--untracked-files=no";
        status_argv[IV]        = NIHIL;
        status = (arbor.mensura > ZEPHYRUM)
            ? _effusio_mandati(status_argv, chorda_ut_cstr(arbor,
                  piscina), piscina, &bene)
            : chorda_ex_literis("", piscina);
        sordidum[ZEPHYRUM] = (bene && status.mensura > ZEPHYRUM)
            ? '1' : '0';
        sordidum[I] = '\0';
        setenv("MENSOR_COMMISSUM", sha, I);
        setenv("MENSOR_SORDIDUM", sordidum, I);
        setenv("MENSOR_RAMUS", _litterae_aut_vacuae(ramus, piscina), I);
        setenv("MENSOR_ARBOR", _litterae_aut_vacuae(arbor, piscina), I);
        si (getenv("MENSOR_MACHINA") == NIHIL)
        {
            setenv("MENSOR_MACHINA", _litterae_aut_vacuae(machina_nota(
                piscina).identitas, piscina), I);
        }
    }
    si (getenv("MENSOR_SESSIO") == NIHIL)
    {
        setenv("MENSOR_SESSIO", _litterae_aut_vacuae(
            mensura_sessionem_novam(piscina), piscina), I);
    }
    i->sessio     = chorda_ex_literis(_ambitus_aut("MENSOR_SESSIO", ""),
        piscina);
    i->parens     = chorda_ex_literis(_ambitus_aut("MENSOR_PARENS", ""),
        piscina);
    i->commissum  = chorda_ex_literis(_ambitus_aut("MENSOR_COMMISSUM",
        ""),
        piscina);
    i->sordidum   = strcmp(_ambitus_aut("MENSOR_SORDIDUM", "0"), "1")
        == ZEPHYRUM;
    i->ramus      = chorda_ex_literis(_ambitus_aut("MENSOR_RAMUS", ""),
        piscina);
    i->arbor      = chorda_ex_literis(_ambitus_aut("MENSOR_ARBOR", ""),
        piscina);
    i->machina    = chorda_ex_literis(_ambitus_aut("MENSOR_MACHINA",
        ""),
        piscina);
    i->vocator    = chorda_ex_literis(_ambitus_aut("MENSOR_VOCATOR",
        "homo"), piscina);
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

/* SUTURA ACTORUM (spec par. IV): corpus servatur per hanc functionem
 * SOLAM - redactio postea filtrum intra eam fiet. v1: <acta>/<id>.log
 * (MENSOR_ACTA aut ~/.rhubarb/cursus/acta), forma ut acta fabricae;
 * fluxus quisque ACTA_MAXIMA (cauda servatur). Referentia = "<id>.log"
 * radici actorum relativa; vacua si scribi nequit. */
interior vacuum
_fluxum_scribere (
                FILE* f,
    constans character* titulus,
               chorda  fluxus)
{
    fprintf(f, "== %s ==\n", titulus);
    si (fluxus.mensura > (i32)ACTA_MAXIMA)
    {
        fprintf(f, "[... %ld octeti praecisi, cauda servata]\n",
            (longus)(fluxus.mensura - (i32)ACTA_MAXIMA));
        fluxus.datum    += fluxus.mensura - (i32)ACTA_MAXIMA;
        fluxus.mensura  = (i32)ACTA_MAXIMA;
    }
    si (fluxus.mensura > ZEPHYRUM)
    {
        fwrite(fluxus.datum, (size_t)I, (size_t)fluxus.mensura, f);
        si (fluxus.datum[fluxus.mensura - I] != '\n')
        {
            fputc('\n', f);
        }
    }
}

interior chorda
_acta_servare (
               chorda  id,
               chorda  effusio,
               chorda  erratum,
              Piscina* piscina,
                  s64* mensura_out)
{
    constans character* radix;
             character  directorium[IV * MXXIV];
             character  via[IV * MXXIV + C];
              FILE*  f;
              long   magnitudo_plagulae;

    *mensura_out  = (s64)ZEPHYRUM;
    radix         = getenv("MENSOR_ACTA");
    si (radix != NIHIL && radix[0] != '\0')
    {
        sprintf(directorium, "%s", radix);
    }
    alioquin
    {
        sprintf(directorium, "%s/.rhubarb/cursus/acta",
            _ambitus_aut("HOME", "/tmp"));
    }
    {
        character  gradus[IV * MXXIV];
        character* p;

        sprintf(gradus, "%s", directorium);
        per (p = gradus + I; *p != '\0'; p++)
        {
            si (*p == '/')
            {
                *p = '\0';
                (vacuum)mkdir(gradus, (mode_t)0755);
                *p = '/';
            }
        }
        (vacuum)mkdir(gradus, (mode_t)0755);
    }
    sprintf(via, "%s/%.*s.log", directorium, (integer)id.mensura,
        (constans character*)id.datum);
    f = fopen(via, "wb");
    si (f == NIHIL)
    {
        redde chorda_ex_literis("", piscina);
    }
    _fluxum_scribere(f, "effusio", effusio);
    _fluxum_scribere(f, "erratum", erratum);
    magnitudo_plagulae = ftell(f);
    fclose(f);
    *mensura_out = (s64)magnitudo_plagulae;
    redde chorda_concatenare(id, chorda_ex_literis(".log", piscina),
        piscina);
}

/* diarium cursuum: MENSOR_DIARIUM aut ~/.rhubarb/mensurae.diarium
 * (persistens - /tmp ad reboot perit) */
interior constans character*
_diarium_cursuum (
    Piscina* piscina)
{
    constans character* via = getenv("MENSOR_DIARIUM");
             character* plena;

    si (via != NIHIL && via[0] != '\0')
    {
        redde via;
    }
    plena = (character*)piscina_allocare(piscina, (memoriae_index)DXII);
    sprintf(plena, "%s/.rhubarb/mensurae.diarium",
        _ambitus_aut("HOME", "/tmp"));
    redde plena;
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
    _contextum_parare(&initium, piscina);
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
    diarium          = _diarium_cursuum(piscina);
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
        (vacuum)usleep((useconds_t)(XX * M));
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
    finis.acta = _acta_servare(initium.id, r.effusio, r.erratum,
        piscina,
        &finis.acta_mensura);
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
    constans character* diarium = _diarium_cursuum(piscina);
             character  condendum[DXII + LXIV];
        MensuraFructus  fructus;

    sprintf(condendum, "%s.condendum.%ld", diarium, (longus)getpid());
    si (rename(diarium, condendum) != ZEPHYRUM)
    {
        redde VERUM;   /* diarium absens: nihil condendum */
    }
    (vacuum)usleep((useconds_t)(L * M));
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
                 chorda  argv_visum = c->initium.argv;
                    i32  j;
              character* a = (character*)piscina_allocare(piscina,
                  (memoriae_index)(argv_visum.mensura + I));

        per (j = ZEPHYRUM; a != NIHIL && j < argv_visum.mensura; j++)
        {
            a[j] = (argv_visum.datum[j] == '\t' || argv_visum.datum[j]
                == '\n') ? ' ' : (character)argv_visum.datum[j];
        }
        si (a != NIHIL)
        {
            a[argv_visum.mensura] = '\0';
        }
        imprimere(machina
            ? "CURSUS\t%.*s\t%.*s\t%.*s\t%s\t%d\t%ld\t%.*s\t%d"
              "\t%.*s\t%.*s\t%.*s\t%.*s\t%s\n"
            : "%.*s  parens=%.*s  %.*s  %s  exitus %d  %ld ms  %.*s"
              " sordidum=%d  %.*s  %.*s  %.*s  acta %.*s  [%s]\n",
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
            (constans character*)c->finis.acta.datum,
            a != NIHIL ? a : "");
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
    fprintf(stderr,
        "usus: mensor cursus currere [-titulus T] -- mandatum ...\n"
        "      mensor cursus legere [-machina] [-volumen via]\n");
    redde 2;
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
        "  mensor cursus legere [-machina]\n\n"
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
