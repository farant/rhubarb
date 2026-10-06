/* probatio_pseudoterminale.c - infans in pseudo-terminali
 * (aemulator-plan B3)
 *
 * I-II: configuratio et pons memoriae (sutura probationum).
 * III: pons posix cum infantibus veris (/bin/sh): effusio per
 * terminalem (\r\n), magnitudo (stty size) et mutatio eius, codex
 * et signum exitus, ambitus (positus, sublatus, ordinarius),
 * directorium, exec fractum DISTINCTUM ab exitu 127, claudere sine
 * zombi et sine fossa relicta (etiam infans SIGHUP ignorans),
 * signa ignorata hospitis non hereditantur, scribere numquam
 * obstat. Omnis ansa mora et numero limitata. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "pseudoterminale.h"
#include "credo.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <poll.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

hic_manens Piscina* piscina;

/* litterae in chorda (non NUL-terminata) */
interior b32
continet (
                chorda  c,
    constans character* literae)
{
    i32 n;
    i32 i;

    n = (i32)strlen(literae);
    per (i = ZEPHYRUM; i + n <= c.mensura; i++)
    {
        si (memcmp(c.datum + i, literae, (memoriae_index)n) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* legere donec finis (-1) aut silentium trium secundorum */
interior chorda
omnia_legere (
    Pseudoterminale* pt)
{
     chorda c;
        s32 r;
        i32 gradus;

    c.datum    = (i8*)piscina_allocare(piscina, IV * MXXIV);
    c.mensura  = ZEPHYRUM;
    per (gradus = ZEPHYRUM; gradus < CC && c.mensura < IV * MXXIV;
         gradus++)
    {
        r = pt->legere(pt->datum, c.datum + c.mensura,
            IV * MXXIV - c.mensura, MMM);
        si (r <= ZEPHYRUM)
        {
            frange;
        }
        c.mensura += (i32)r;
    }
    redde c;
}

/* legere donec linea prima completa; numerus eius (pid) */
interior s32
numerum_legere (
    Pseudoterminale* pt)
{
     i8 b[CXXVIII];
    i32 n;
    s32 r;
    s32 numerus;
    i32 i;
    i32 gradus;

    n = ZEPHYRUM;
    per (gradus = ZEPHYRUM; gradus < C && n < CXXVII; gradus++)
    {
        r = pt->legere(pt->datum, b + n, CXXVII - n, MMM);
        si (r <= ZEPHYRUM)
        {
            frange;
        }
        n += (i32)r;
        si (memchr(b, '\n', (memoriae_index)n))
        {
            frange;
        }
    }
    numerus = ZEPHYRUM;
    per (i = ZEPHYRUM; i < n && b[i] >= '0' && b[i] <= '9'; i++)
    {
        numerus = numerus * X + (s32)(b[i] - '0');
    }
    redde numerus;
}

/* finitus intra duo secunda */
interior b32
exitum_exspectare (
                 Pseudoterminale* pt,
           PseudoterminaleExitus* exitus)
{
    i32 i;

    per (i = ZEPHYRUM; i < CC; i++)
    {
        si (pt->finitus(pt->datum, exitus))
        {
            redde VERUM;
        }
        (vacuum)poll(NIHIL, ZEPHYRUM, X);
    }
    redde FALSUM;
}

interior Pseudoterminale*
generare (
    constans character* constans* argumenta,
                              i32  latitudo,
                              i32  altitudo,
    constans character* constans* ambitus,
              constans character* directorium,
            PseudoterminaleError* error)
{
    PseudoterminaleConfiguratio cfg;

    pseudoterminale_configuratio_initiare(&cfg);
    cfg.argumenta    = argumenta;
    cfg.latitudo     = latitudo;
    cfg.altitudo     = altitudo;
    cfg.directorium  = directorium;
    si (ambitus)
    {
        cfg.ambitus = ambitus;
    }
    redde pseudoterminale_posix_creare(piscina, &cfg, error, NIHIL);
}

/* fossae apertae (0..CCLV) numeratae: aequales ante et post =
 * nulla relicta. (Fossa infima libera foramina supra eam non
 * videt - planta B3 P16.) */
interior i32
fossas_numerare (vacuum)
{
    integer f;
        i32 n;

    n = ZEPHYRUM;
    per (f = ZEPHYRUM; f < CCLVI; f++)
    {
        si (fcntl(f, F_GETFD) != -I)
        {
            n++;
        }
    }
    redde n;
}

/* scribere ad infantem qui non legit: multa, numquam obstat.
 * Reddit acceptos; -1 si responsum ullum extra 0..n. */
interior s32
multum_scribere (
    Pseudoterminale* pt)
{
     i8 b[MXXIV];
    i32 i;
    s32 r;
    s32 summa;

    memset(b, 'x', magnitudo(b));
    summa = ZEPHYRUM;
    per (i = ZEPHYRUM; i < CCLVI; i++)
    {
        r = pt->scribere(pt->datum, b, MXXIV);
        si (r < ZEPHYRUM || r > MXXIV)
        {
            redde -I;
        }
        summa += r;
    }
    redde summa;
}

interior vacuum
configurationem_probare (vacuum)
{
    PseudoterminaleConfiguratio cfg;

    imprimere("\n--- I: configuratio ---\n");
    pseudoterminale_configuratio_initiare(&cfg);
    CREDO_NIHIL(cfg.argumenta);
    CREDO_NIHIL(cfg.directorium);
    CREDO_AEQUALIS_I32(cfg.latitudo, LXXX);
    CREDO_AEQUALIS_I32(cfg.altitudo, XXIV);
    CREDO_AEQUALIS_I32(cfg.px_latitudo, ZEPHYRUM);
    CREDO_NON_NIHIL(cfg.ambitus);
    CREDO_VERUM(strcmp(cfg.ambitus[ZEPHYRUM], "TERM=xterm-256color")
                == ZEPHYRUM);
    CREDO_VERUM(strcmp(cfg.ambitus[I], "COLORTERM=truecolor")
                == ZEPHYRUM);
    CREDO_NIHIL(cfg.ambitus[II]);
    CREDO_VERUM(strlen(pseudoterminale_error_nomen(
        PSEUDOTERMINALE_ERROR_EXEC)) > ZEPHYRUM);
    CREDO_VERUM(strcmp(pseudoterminale_error_nomen(
        PSEUDOTERMINALE_ERROR_EXEC), pseudoterminale_error_nomen(
        PSEUDOTERMINALE_ERROR_APERIRE)) != ZEPHYRUM);
}

interior vacuum
memoriam_probare (vacuum)
{
          Pseudoterminale* pt;
    PseudoterminaleExitus  ex;
                       i8  b[IV];
                      i32  lat;
                      i32  alt;

    imprimere("\n--- II: pons memoriae ---\n");
    pt = pseudoterminale_memoriae_creare(piscina,
        (constans i8*)"salve", V, III);
    CREDO_NON_NIHIL(pt);
    CREDO_AEQUALIS_S32(pt->fossa(pt->datum), -I);
    /* effusio per partes, deinde finis */
    CREDO_AEQUALIS_S32(pt->legere(pt->datum, b, IV, ZEPHYRUM), IV);
    CREDO_VERUM(memcmp(b, "salv", IV) == ZEPHYRUM);
    CREDO_FALSUM(pt->finitus(pt->datum, &ex));
    CREDO_AEQUALIS_S32(pt->legere(pt->datum, b, IV, ZEPHYRUM), I);
    CREDO_VERUM(b[ZEPHYRUM] == 'e');
    CREDO_AEQUALIS_S32(pt->legere(pt->datum, b, IV, ZEPHYRUM), -I);
    CREDO_VERUM(pt->finitus(pt->datum, &ex));
    CREDO_AEQUALIS_I32(ex.codex, III);
    CREDO_AEQUALIS_I32(ex.signum, ZEPHYRUM);
    /* scripta capiuntur, tota accipiuntur */
    CREDO_AEQUALIS_S32(pt->scribere(pt->datum, (constans i8*)"ab", II),
                       II);
    CREDO_AEQUALIS_S32(pt->scribere(pt->datum, (constans i8*)"c", I),
        I);
    CREDO_VERUM(chorda_aequalis_literis(
        pseudoterminale_memoriae_captum(pt), "abc"));
    /* magnitudo servatur; mala recusatur */
    CREDO_VERUM(pt->amplitudo(pt->datum, XL, IX, ZEPHYRUM, ZEPHYRUM));
    CREDO_FALSUM(pt->amplitudo(pt->datum, ZEPHYRUM, IX, ZEPHYRUM,
                               ZEPHYRUM));
    pseudoterminale_memoriae_amplitudo(pt, &lat, &alt);
    CREDO_AEQUALIS_I32(lat, XL);
    CREDO_AEQUALIS_I32(alt, IX);
    /* claudere idempotens; post id nihil */
    pt->claudere(pt->datum);
    pt->claudere(pt->datum);
    CREDO_AEQUALIS_S32(pt->legere(pt->datum, b, IV, ZEPHYRUM), -I);
    CREDO_AEQUALIS_S32(pt->scribere(pt->datum, (constans i8*)"d", I),
                       -I);
}

interior vacuum
posix_probare (vacuum)
{
          Pseudoterminale* pt;
    PseudoterminaleExitus  ex;
     PseudoterminaleError  e;
                   chorda  c;
                      s32  pid;
                      s32  acceptum;
                  integer  status;
                      i32  fossae_ante;
                  integer  f;
                       i8  b[XVI];
                   vacuum (*pristinus)(integer);
    constans character* effusio[]   = { "/bin/sh", "-c",
        "printf 'a\\nb'", NIHIL };
    constans character* regens[]    = { "/bin/stty", "-f", "/dev/tty",
        "size", NIHIL };
    constans character* mensura[]   = { "/bin/sh", "-c", "stty size",
        NIHIL };
    constans character* mutata[]    = { "/bin/sh", "-c",
        "read x; stty size", NIHIL };
    constans character* codex[]     = { "/bin/sh", "-c", "exit 3",
        NIHIL };
    constans character* signum[]    = { "/bin/sh", "-c",
        "kill -TERM $$", NIHIL };
    constans character* ambitus_a[] = { "/bin/sh", "-c",
        "printf '%s|%s|%s' \"$TERM\" \"$RHUBARB_PROBA\" "
        "\"${HOME-absens}\"", NIHIL };
    constans character* ambitus_m[] = { "TERM=xterm-256color",
        "RHUBARB_PROBA=salve", "HOME", NIHIL };
    constans character* ordinarii[] = { "/bin/sh", "-c",
        "printf '%s|%s' \"$TERM\" \"$COLORTERM\"", NIHIL };
    constans character* locus[]     = { "/bin/sh", "-c", "pwd",
        NIHIL };
    constans character* absens[]    = { "/nusquam/binarium", NIHIL };
    constans character* exitus_absentis[]   = { "/bin/sh", "-c",
        "exit 127",
        NIHIL };
    constans character* dormiens[]  = { "/bin/sh", "-c",
        "echo $$; exec sleep 100", NIHIL };
    constans character* surdus[]    = { "/bin/sh", "-c",
        "trap '' HUP; echo $$; exec sleep 100", NIHIL };
    constans character* tubus[]     = { "/bin/sh", "-c",
        "kill -PIPE $$; exit 0", NIHIL };
    constans character* crudus[]    = { "/bin/sh", "-c",
        "stty raw -echo; exec sleep 100", NIHIL };

    imprimere("\n--- III: pons posix (infantes veri) ---\n");
    fossae_ante = fossas_numerare();

    /* effusio per terminalem: onlcr facit \r\n */
    pt = generare(effusio, LXXX, XXIV, NIHIL, NIHIL, &e);
    CREDO_NON_NIHIL(pt);
    CREDO_AEQUALIS_I32((i32)e, (i32)PSEUDOTERMINALE_OK);
    CREDO_VERUM(pt->fossa(pt->datum) >= ZEPHYRUM);
    c = omnia_legere(pt);
    CREDO_VERUM(chorda_aequalis_literis(c, "a\r\nb"));
    /* finis: -1, non 0 (nihil paratum) */
    CREDO_AEQUALIS_S32(pt->legere(pt->datum, b, XVI, C), -I);
    CREDO_VERUM(exitum_exspectare(pt, &ex));
    CREDO_AEQUALIS_I32(ex.codex, ZEPHYRUM);
    CREDO_AEQUALIS_I32(ex.signum, ZEPHYRUM);
    pt->claudere(pt->datum);

    /* terminale regens (setsid + TIOCSCTTY): /dev/tty aperitur.
     * SINE CONCHA: /bin/sh terminale regens SUA SPONTE capit, ergo
     * probatio per sh vitium TIOCSCTTY celabat (planta B3 P2). */
    pt = generare(regens, LXXX, XXIV, NIHIL, NIHIL, &e);
    CREDO_VERUM(chorda_aequalis_literis(omnia_legere(pt), "24 80\r\n"));
    pt->claudere(pt->datum);

    /* magnitudo creationis */
    pt = generare(mensura, XXX, VII, NIHIL, NIHIL, &e);
    CREDO_NON_NIHIL(pt);
    CREDO_VERUM(chorda_aequalis_literis(omnia_legere(pt), "7 30\r\n"));
    pt->claudere(pt->datum);

    /* mutatio magnitudinis ante 'stty size' */
    pt = generare(mutata, XXX, VII, NIHIL, NIHIL, &e);
    CREDO_NON_NIHIL(pt);
    CREDO_VERUM(pt->amplitudo(pt->datum, XL, IX, ZEPHYRUM, ZEPHYRUM));
    CREDO_FALSUM(pt->amplitudo(pt->datum, ZEPHYRUM, IX, ZEPHYRUM,
                               ZEPHYRUM));
    CREDO_AEQUALIS_S32(pt->scribere(pt->datum, (constans i8*)"x\n", II),
                       II);
    CREDO_VERUM(continet(omnia_legere(pt), "9 40\r\n"));
    pt->claudere(pt->datum);

    /* codex exitus; signum */
    pt = generare(codex, LXXX, XXIV, NIHIL, NIHIL, &e);
    (vacuum)omnia_legere(pt);
    CREDO_VERUM(exitum_exspectare(pt, &ex));
    CREDO_AEQUALIS_I32(ex.codex, III);
    CREDO_AEQUALIS_I32(ex.signum, ZEPHYRUM);
    /* iterum vocata idem reddit */
    CREDO_VERUM(pt->finitus(pt->datum, &ex));
    CREDO_AEQUALIS_I32(ex.codex, III);
    pt->claudere(pt->datum);
    pt = generare(signum, LXXX, XXIV, NIHIL, NIHIL, &e);
    (vacuum)omnia_legere(pt);
    CREDO_VERUM(exitum_exspectare(pt, &ex));
    CREDO_AEQUALIS_I32(ex.signum, SIGTERM);
    CREDO_AEQUALIS_I32(ex.codex, ZEPHYRUM);
    pt->claudere(pt->datum);

    /* ambitus: positus, sublatus; ordinarius ex initiare */
    pt = generare(ambitus_a, LXXX, XXIV, ambitus_m, NIHIL, &e);
    CREDO_VERUM(chorda_aequalis_literis(omnia_legere(pt),
        "xterm-256color|salve|absens"));
    pt->claudere(pt->datum);
    pt = generare(ordinarii, LXXX, XXIV, NIHIL, NIHIL, &e);
    CREDO_VERUM(chorda_aequalis_literis(omnia_legere(pt),
        "xterm-256color|truecolor"));
    pt->claudere(pt->datum);

    /* directorium */
    pt = generare(locus, LXXX, XXIV, NIHIL, "/", &e);
    CREDO_VERUM(chorda_aequalis_literis(omnia_legere(pt), "/\r\n"));
    pt->claudere(pt->datum);

    /* exec fractum: NIHIL et EXEC; exitus 127 verus: creatur */
    e   = PSEUDOTERMINALE_OK;
    pt  = generare(absens, LXXX, XXIV, NIHIL, NIHIL, &e);
    CREDO_NIHIL(pt);
    CREDO_AEQUALIS_I32((i32)e, (i32)PSEUDOTERMINALE_ERROR_EXEC);
    pt = generare(exitus_absentis, LXXX, XXIV, NIHIL, NIHIL, &e);
    CREDO_NON_NIHIL(pt);
    (vacuum)omnia_legere(pt);
    CREDO_VERUM(exitum_exspectare(pt, &ex));
    CREDO_AEQUALIS_I32(ex.codex, CXXVII);
    pt->claudere(pt->datum);
    /* argumenta mala; magnitudo mala */
    pt = generare(NIHIL, LXXX, XXIV, NIHIL, NIHIL, &e);
    CREDO_NIHIL(pt);
    CREDO_AEQUALIS_I32((i32)e, (i32)PSEUDOTERMINALE_ERROR_ARGUMENTA);
    pt = generare(effusio, ZEPHYRUM, XXIV, NIHIL, NIHIL, &e);
    CREDO_NIHIL(pt);
    CREDO_AEQUALIS_I32((i32)e, (i32)PSEUDOTERMINALE_ERROR_ARGUMENTA);

    /* claudere infantem currentem: messus, fossa clausa */
    pt   = generare(dormiens, LXXX, XXIV, NIHIL, NIHIL, &e);
    pid  = numerum_legere(pt);
    CREDO_VERUM(pid > I);
    f = (integer)pt->fossa(pt->datum);
    CREDO_FALSUM(pt->finitus(pt->datum, &ex));
    pt->claudere(pt->datum);
    pt->claudere(pt->datum);
    CREDO_VERUM(waitpid((pid_t)pid, &status, WNOHANG) == -I);
    CREDO_VERUM(kill((pid_t)pid, ZEPHYRUM) == -I);
    CREDO_VERUM(fcntl(f, F_GETFD) == -I);
    CREDO_AEQUALIS_S32(pt->legere(pt->datum, b, XVI, ZEPHYRUM), -I);
    CREDO_AEQUALIS_S32(pt->scribere(pt->datum, (constans i8*)"x", I),
                       -I);
    /* infans SIGHUP ignorans: post moram SIGKILL */
    pt   = generare(surdus, LXXX, XXIV, NIHIL, NIHIL, &e);
    pid  = numerum_legere(pt);
    CREDO_VERUM(pid > I);
    pt->claudere(pt->datum);
    CREDO_VERUM(waitpid((pid_t)pid, &status, WNOHANG) == -I);
    CREDO_VERUM(kill((pid_t)pid, ZEPHYRUM) == -I);

    /* SIGPIPE ignoratum in hospite non hereditatur */
    pristinus  = signal(SIGPIPE, SIG_IGN);
    pt         = generare(tubus, LXXX, XXIV, NIHIL, NIHIL, &e);
    (vacuum)signal(SIGPIPE, pristinus);
    (vacuum)omnia_legere(pt);
    CREDO_VERUM(exitum_exspectare(pt, &ex));
    CREDO_AEQUALIS_I32(ex.signum, SIGPIPE);
    pt->claudere(pt->datum);

    /* scribere ad infantem non legentem (modus crudus: nucleus post
     * ~MXXIV octetos plenus) numquam obstat: custodia prius in
     * filio; deinde, si nihil pependit, in parente acceptos numerat
     * (partim acceptum, non totum) */
    pt = generare(crudus, LXXX, XXIV, NIHIL, NIHIL, &e);
    (vacuum)pt->legere(pt->datum, b, XVI, CC);
    CREDO_NON_PENDET((vacuum)multum_scribere(pt), V * M);
    si (credo_omnia_praeterierunt())
    {
        acceptum = multum_scribere(pt);
        CREDO_VERUM(acceptum >= ZEPHYRUM);
        CREDO_VERUM(acceptum < CCLVI * MXXIV);
    }
    pt->claudere(pt->datum);

    /* nulla fossa relicta per omnes */
    CREDO_AEQUALIS_I32(fossas_numerare(), fossae_ante);
}

s32
principale (vacuum)
{
    piscina = piscina_generare_dynamicum("probatio_pseudoterminale",
        LXIV * MXXIV);
    credo_aperire(piscina);

    configurationem_probare();
    memoriam_probare();
    posix_probare();

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
