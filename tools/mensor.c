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

#include "postulata_posix.h"

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "mensura.h"
#include "machina.h"
#include "processus.h"
#include "volumen.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>   /* execvp pro -ui */

/* Mandatum metiendum horas capere potest (aedificatio plena) */
#define MORA_AMPLA MMMMXCVI * M

interior constans character*
_arg (integer argc, character** argv, constans character* clavis)
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
_vexillum (integer argc, character** argv, constans character* clavis)
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
_ambitus_aut (constans character* clavis, constans character* alterum)
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
_diarium (integer argc, character** argv)
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
_volumen_via (integer argc, character** argv, Piscina* piscina)
{
    constans character* via;
    ChordaAedificator*  aedificator;

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
_litterae (chorda c, Piscina* piscina)
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
_sessionem_incipere (Piscina* piscina, integer argc, character** argv)
{
    chorda    sessio;
    character diarium[CCLVI];
    b32       nudus;
    integer   i;

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
              (integer)sessio.mensura, (constans character*)sessio.datum);
    imprimere("MENSOR_DIARIUM=%s; export MENSOR_DIARIUM;\n", diarium);

    redde 0;
}

/* ============================================================
 * mensurare / addere
 * ============================================================ */

interior Mensura
_mensuram_struere (Piscina* piscina, integer argc, character** argv,
                   constans character* titulus, duplex valor,
                   constans character* unitas)
{
    Mensura             m;
    constans character* parens;

    parens = _arg(argc, argv, "-parens");
    si (parens == NIHIL)
    {
        parens = _ambitus_aut("MENSOR_PARENS", "");
    }

    m.sessio   = chorda_ex_literis(
                     _ambitus_aut("MENSOR_SESSIO", "sine-sessione"), piscina);
    m.id       = mensura_id_novum(piscina);
    m.parens   = chorda_ex_literis(parens, piscina);
    m.titulus  = chorda_ex_literis(titulus, piscina);
    m.valor    = valor;
    m.unitas   = chorda_ex_literis(unitas, piscina);
    m.momentum = mensura_momentum_nunc();

    redde m;
}

interior integer
_mensurare (Piscina* piscina, integer argc, character** argv)
{
    constans character*  titulus;
    integer              i, initium;
    s64                  ante, post;
    ProcessusResultus    resultus;
    Mensura              m;
    character            id_tabula[LXIV];

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

    m.valor    = (duplex)(post - ante) / 1000.0;
    m.momentum = ante;

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
_addere_tabulam (Piscina* piscina, integer argc, character** argv,
                 constans character* via_tabulae,
                 constans character* praefixum,
                 constans character* unitas)
{
    FILE*              plagula;
    character          linea[MMMMXCVI];
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

    dum (fgets(linea, MMMMXCVI, plagula) != NIHIL)
    {
        duplex     valor;
        character* finis;
        character* titulus;
        Mensura    m;

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
_addere (Piscina* piscina, integer argc, character** argv)
{
    constans character* titulus;
    constans character* valor_litterae;
    constans character* unitas;
    constans character* tabula;
    constans character* praefixum;
    Mensura             m;

    titulus        = _arg(argc, argv, "-titulus");
    valor_litterae = _arg(argc, argv, "-valor");
    unitas         = _arg(argc, argv, "-unitas");
    tabula         = _arg(argc, argv, "-tabula");
    praefixum      = _arg(argc, argv, "-praefixum");

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
_commissum (Piscina* piscina)
{
    constans character* argumenta[IV];
    ProcessusResultus   resultus;

    argumenta[0]   = "git";
    argumenta[I]   = "rev-parse";
    argumenta[II]  = "HEAD";
    argumenta[III] = NIHIL;

    resultus = processus_exsequi(argumenta, (i32)(X * M), piscina);
    si (!resultus.successus || resultus.codex_exitus != 0)
    {
        redde chorda_ex_literis("", piscina);
    }

    /* Lineam novam terminalem tollere */
    dum (resultus.effusio.mensura > ZEPHYRUM &&
         (resultus.effusio.datum[resultus.effusio.mensura - I] == '\n' ||
          resultus.effusio.datum[resultus.effusio.mensura - I] == '\r'))
    {
        resultus.effusio.mensura--;
    }

    redde resultus.effusio;
}

interior integer
_condere (Piscina* piscina, integer argc, character** argv)
{
    chorda             via;
    Volumen*           volumen;
    MensuraFructus     fructus;
    MachinaNota        nota;
    ChordaAedificator* aedificator;
    chorda             commissum;

    via     = _volumen_via(argc, argv, piscina);
    volumen = volumen_aperire_aut_creare(piscina, _litterae(via, piscina));
    si (volumen == NIHIL)
    {
        fprintf(stderr, "mensor: volumen aperiri non potest: %.*s\n",
                (integer)via.mensura, (constans character*)via.datum);
        redde I;
    }

    /* Metadata sessionis SEMEL - hic, non in linea quaque */
    nota      = machina_nota(piscina);
    commissum = _commissum(piscina);

    aedificator = chorda_aedificator_creare(piscina, (memoriae_index)CC);
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
        "  mensor condere [-volumen via]\n\n"
        "  Ambitus: MENSOR_SESSIO MENSOR_PARENS MENSOR_DIARIUM"
        " MENSOR_VOLUMEN\n");
}

integer
main (integer argc, character** argv)
{
    Piscina* piscina;
    integer  codex;

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
    alioquin
    {
        _usus();
        codex = 2;
    }

    piscina_destruere(piscina);

    redde codex;
}
