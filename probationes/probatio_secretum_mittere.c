/* probatio_secretum_mittere.c - tools/secretum_mittere.sh (communio-
 * plan-3 T1): clavis localis in hospitem per ssh stdin, numquam visa.
 * ssh fictum (probationes/fixa/secretum_mittere/ssh_fictus.sh) servum
 * in directorio temporario simulat et argumenta omnia notat. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include "lectiones.h"
#include "moneta.h"
#include "sigillum.h"
#include "processus.h"
#include "credo.h"
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#define FICTUM "probationes/fixa/secretum_mittere/ssh_fictus.sh"

nomen structura {
    character radix[CCLVI];
    character clavis[CCLVI];
    character diarium[CCLVI];
    character destinatio[CCLVI];
    character fictum[MXXIV];
    character secretum[LXIV];
    character sigillum[SIGILLUM_HEX_MENSURA];
} Mundus;

hic_manens Piscina* piscina;
hic_manens  Mundus  mundus;

interior vacuum
_parare (
    CredoContextus* c)
{
    character moneta[MONETA_ULID_MENSURA];
     Sigillum g;

    piscina = c->piscina;
    memset(&mundus, ZEPHYRUM, magnitudo(mundus));
    moneta_ulid(moneta);
    sprintf(mundus.radix, "/tmp/probatio_secretum_%s", moneta);
    sprintf(mundus.clavis, "%s/k.clavis", mundus.radix);
    sprintf(mundus.diarium, "%s/ssh.log", mundus.radix);
    sprintf(mundus.destinatio, "%s/var/lib/exemplar/k.clavis",
        mundus.radix);
    /* clavis probationis (non vera): signum quaerendum in effusione */
    sprintf(mundus.secretum, "re_PROBATIO_%s", moneta);
    g = sigillum_computare(mundus.secretum,
        (memoriae_index)strlen(mundus.secretum));
    sigillum_hex(&g, mundus.sigillum);
    (vacuum)getcwd(mundus.fictum, magnitudo(mundus.fictum) - XXXII);
    strcat(mundus.fictum, "/" FICTUM);
    (vacuum)filum_directorium_creare_cum_modo(mundus.radix, 0700);
    (vacuum)filum_scribere(mundus.clavis, chorda_ex_literis(
        mundus.secretum, piscina));
    (vacuum)filum_modum_ponere(mundus.clavis, 0600);
}

interior vacuum
_purgare (
    CredoContextus* c)
{
    (vacuum)c;
    (vacuum)filum_delere(mundus.destinatio);
    (vacuum)filum_delere(mundus.clavis);
    (vacuum)filum_delere(mundus.diarium);
    piscina = NIHIL;
}

/* instrumentum currere: optiones (NIHIL-terminatae) ante clavem; app,
 * alias; corrumpere = ssh fictum copiam corrumpit */
interior ProcessusResultus
currere (
    constans character* constans* optiones,
    constans character* app,
                   b32  corrumpere)
{
    constans character* argv[XX];
             character  e_ssh[MXXIV + XXXII];
             character  e_radix[CCLVI + XVI];
             character  e_log[CCLVI + XVI];
                   i32  n;
                   i32  i;

    sprintf(e_ssh, "SECRETUM_MITTERE_SSH=%s", mundus.fictum);
    sprintf(e_radix, "STUB_RADIX=%s", mundus.radix);
    sprintf(e_log, "STUB_LOG=%s", mundus.diarium);
    n          = ZEPHYRUM;
    argv[n++]  = "/usr/bin/env";
    argv[n++]  = e_ssh;
    argv[n++]  = e_radix;
    argv[n++]  = e_log;
    si (corrumpere)
    {
        argv[n++] = "STUB_CORRUMPERE=1";
    }
    argv[n++] = "/bin/bash";
    argv[n++] = "tools/secretum_mittere.sh";
    per (i = ZEPHYRUM; optiones && optiones[i]; i++)
    {
        argv[n++] = optiones[i];
    }
    argv[n++]  = mundus.clavis;
    argv[n++]  = app;
    argv[n++]  = "smaragda";
    argv[n]    = NIHIL;
    redde processus_exsequi(argv, XXX * M, piscina);
}

interior b32
continet (
                chorda  c,
    constans character* sub)
{
    i32 n;
    i32 i;

    n = (i32)strlen(sub);
    per (i = ZEPHYRUM; i + n <= c.mensura; i++)
    {
        si (memcmp(c.datum + i, sub, (size_t)n) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* secretum (et sigillum eius) in nulla effusione nec in diario */
interior b32
tacitum (
    ProcessusResultus r)
{
    chorda d;

    d = filum_existit(mundus.diarium)
        ? filum_legere_totum(mundus.diarium, piscina)
        : chorda_ex_literis("", piscina);
    redde !continet(r.effusio, mundus.secretum)
        && !continet(r.erratum, mundus.secretum)
        && !continet(d, mundus.secretum)
        && !continet(r.effusio, mundus.sigillum)
        && !continet(r.erratum, mundus.sigillum)
        && !continet(d, mundus.sigillum);
}

interior vacuum
_sectio_modus (
    CredoContextus* c)
{
    constans character* agere[II];
     ProcessusResultus  r;

    (vacuum)c;
    agere[ZEPHYRUM]  = "-agere";
    agere[I]         = NIHIL;
    CREDO_NECESSE(filum_modum_ponere(mundus.clavis, 0644));
    r = currere(agere, "exemplar", FALSUM);
    CREDO_NECESSE(r.successus);
    CREDO_AEQUALIS_I32(r.codex_exitus, I);
    CREDO_FALSUM(filum_existit(mundus.diarium));
    CREDO_FALSUM(filum_existit(mundus.destinatio));
    CREDO_VERUM(tacitum(r));
}

interior vacuum
_sectio_siccum (
    CredoContextus* c)
{
    ProcessusResultus r;

    (vacuum)c;
    r = currere(NIHIL, "exemplar", FALSUM);
    CREDO_NECESSE(r.successus);
    CREDO_AEQUALIS_I32(r.codex_exitus, ZEPHYRUM);
    CREDO_VERUM(continet(r.effusio, "install -o exemplar -g exemplar"
        " -m 0600 /dev/stdin /var/lib/exemplar/k.clavis"));
    CREDO_FALSUM(filum_existit(mundus.diarium));
    CREDO_FALSUM(filum_existit(mundus.destinatio));
    CREDO_VERUM(tacitum(r));
}

interior vacuum
_sectio_agere (
    CredoContextus* c)
{
    constans character* agere[II];
    constans character* renovare[III];
     ProcessusResultus  r;
                chorda  copia;
                chorda  diarium;
      structura stat  st;

    (vacuum)c;
    agere[ZEPHYRUM]     = "-agere";
    agere[I]            = NIHIL;
    renovare[ZEPHYRUM]  = "-agere";
    renovare[I]         = "-renovare";
    renovare[II]        = NIHIL;
    r                   = currere(agere, "exemplar", FALSUM);
    CREDO_NECESSE(r.successus);
    CREDO_AEQUALIS_I32(r.codex_exitus, ZEPHYRUM);
    CREDO_VERUM(continet(r.effusio, "congruit"));
    CREDO_NECESSE(filum_existit(mundus.destinatio));
    copia = filum_legere_totum(mundus.destinatio, piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(copia, mundus.secretum);
    CREDO_NECESSE(lectiones_stat(mundus.destinatio, &st) == ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)(st.st_mode & 0777), (i32)0600);
    diarium = filum_legere_totum(mundus.diarium, piscina);
    CREDO_VERUM(continet(diarium, "install -o exemplar -g exemplar"
        " -m 0600 /dev/stdin /var/lib/exemplar/k.clavis"));
    CREDO_VERUM(tacitum(r));

    CREDO_GRADUS("iterum sine -renovare: recusatur");
    r = currere(agere, "exemplar", FALSUM);
    CREDO_AEQUALIS_I32(r.codex_exitus, I);
    CREDO_VERUM(tacitum(r));

    CREDO_GRADUS("-renovare: clavis nova");
    strcat(mundus.secretum, "N");
    {
        Sigillum g = sigillum_computare(mundus.secretum,
            (memoriae_index)strlen(mundus.secretum));

        sigillum_hex(&g, mundus.sigillum);
    }
    (vacuum)filum_scribere(mundus.clavis, chorda_ex_literis(
        mundus.secretum, piscina));
    (vacuum)filum_modum_ponere(mundus.clavis, 0600);
    r = currere(renovare, "exemplar", FALSUM);
    CREDO_AEQUALIS_I32(r.codex_exitus, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(filum_legere_totum(mundus.destinatio,
        piscina), mundus.secretum);
    CREDO_VERUM(tacitum(r));

    CREDO_GRADUS("copia corrupta: differt");
    r = currere(renovare, "exemplar", VERUM);
    CREDO_AEQUALIS_I32(r.codex_exitus, I);
    CREDO_VERUM(continet(r.effusio, "differt"));
    CREDO_VERUM(tacitum(r));
}

interior vacuum
_sectio_nomen_malum (
    CredoContextus* c)
{
    constans character* agere[II];
     ProcessusResultus  r;

    (vacuum)c;
    agere[ZEPHYRUM]  = "-agere";
    agere[I]         = NIHIL;
    r                = currere(agere, "ex;emplar", FALSUM);
    CREDO_NECESSE(r.successus);
    CREDO_AEQUALIS_I32(r.codex_exitus, II);
    CREDO_FALSUM(filum_existit(mundus.diarium));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "modus latior", _sectio_modus, _parare, _purgare, NIHIL, NIHIL },
    { "siccum", _sectio_siccum, _parare, _purgare, NIHIL, NIHIL },
    { "agere, iterum, renovare, differt", _sectio_agere, _parare,
        _purgare, NIHIL, NIHIL },
    { "nomen malum", _sectio_nomen_malum, _parare, _purgare, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("secretum_mittere", SECTIONES);
}
