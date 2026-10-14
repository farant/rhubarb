/*
 * mensura_contextus.c - contextus cursus: hereditas aut computatio
 * semel (analytica T3; ex tools/mensor.c T2 translatum ut bin/fabrica
 * quoque cursus notet). git, processus, machina HIC - lib/mensura
 * pura manet.
 */

#include "postulata_posix.h"

#include "latina.h"
#include "mensura_contextus.h"
#include "git.h"
#include "machina.h"
#include "processus.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

interior constans character*
_ambitus_aut (
    constans character* clavis,
    constans character* alterum)
{
    constans character* valor = getenv(clavis);

    si (valor != NIHIL && valor[0] != '\0')
    {
        redde valor;
    }
    redde alterum;
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

/* contextus ARBORIS datae (non sessionis): commissum HEAD, ramus,
 * sordidum, arbor = via. VERUM si repositorium est; aliter campi vacui
 * (arbor tamen ponitur). Ambitum non tangit. */
interior b32
_arborem_computare (
           chorda  arbor,
    CursusInitium* i,
          Piscina* piscina)
{
     GitRepositorium* repositorium = NIHIL;
           character  sha[GIT_SHA_HEX_MENSURA + I];
              chorda  status;
                 b32  bene = FALSUM;
  constans character* status_argv[V];

    sha[ZEPHYRUM]  = '\0';
    i->arbor       = arbor;
    i->commissum   = chorda_ex_literis("", piscina);
    i->ramus       = chorda_ex_literis("", piscina);
    i->sordidum    = FALSUM;
    si (arbor.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    repositorium = git_aperire(piscina, chorda_ut_cstr(arbor, piscina));
    si (repositorium == NIHIL)
    {
        redde FALSUM;
    }
    si (git_ref_resolvere(repositorium, "HEAD", sha))
    {
        i->commissum = chorda_transcribere(chorda_ex_literis(sha,
            piscina),
            piscina);
    }
    i->ramus               = _ramum_legere(repositorium, piscina);
    status_argv[ZEPHYRUM]  = "git";
    status_argv[I]         = "status";
    status_argv[II]        = "--porcelain";
    status_argv[III]       = "--untracked-files=no";
    status_argv[IV]        = NIHIL;
    status = _effusio_mandati(status_argv, chorda_ut_cstr(arbor,
        piscina),
        piscina, &bene);
    i->sordidum = bene && status.mensura > ZEPHYRUM;
    redde VERUM;
}

b32
mensura_contextum_arboris (
         CursusInitium* initium,
    constans character* via_arboris,
               Piscina* piscina)
{
    redde _arborem_computare(chorda_transcribere(chorda_ex_literis(
        via_arboris != NIHIL ? via_arboris : "", piscina), piscina),
        initium, piscina);
}

/* CONTEXTUS: ex ambitu (hereditas) aut SEMEL computatus et in ambitum
 * positus - cursus interiores eum hereditant, git non iterum vocatur */
vacuum
mensura_contextum_parare (
    CursusInitium* i,
          Piscina* piscina)
{
    constans character* commissum = getenv("MENSOR_COMMISSUM");

    si (commissum == NIHIL)
    {
        CursusInitium computatus;

        memset(&computatus, ZEPHYRUM, magnitudo(computatus));
        (vacuum)_arborem_computare(_arborem_invenire(piscina),
            &computatus,
            piscina);
        setenv("MENSOR_COMMISSUM",
            _litterae_aut_vacuae(computatus.commissum,
            piscina), I);
        setenv("MENSOR_SORDIDUM", computatus.sordidum ? "1" : "0", I);
        setenv("MENSOR_RAMUS", _litterae_aut_vacuae(computatus.ramus,
            piscina), I);
        setenv("MENSOR_ARBOR", _litterae_aut_vacuae(computatus.arbor,
            piscina), I);
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
    /* vocator: ab invocante positus (commissio, fabrica ...); aliter
     * agens si sub Claude Code (CLAUDECODE), aliter homo */
    i->vocator    = chorda_ex_literis(_ambitus_aut("MENSOR_VOCATOR",
        getenv("CLAUDECODE") != NIHIL ? "agens" : "homo"), piscina);
}

/* diarium cursuum: MENSOR_DIARIUM aut ~/.rhubarb/mensurae.diarium
 * (persistens - /tmp ad reboot perit) */
constans character*
mensura_diarium_cursuum (
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
