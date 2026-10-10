/* probatio_cliens_tabularii.c - Probationes clientis daemonis.
 *
 * TEGUMENTUM PRIMUM huius codicis: ~CCCXL lineae in apps/forum/forum.c
 * vixerunt, et probationes radicis apps/ NON vident - ergo cliens
 * (cum defectu-CITO subtili incluso) nullum tegumentum automaticum
 * umquam habuit. Extractio in lib/ hoc sanat.
 *
 * OFFLINE: daemon EPHEMERUS portu 0 in mundo scratch (exemplar
 * probatio_fori.c) - nulla rete externa, nullus daemon vivus tactus.
 *
 * SECTIONES (credo v2, 2026-10-10): quaeque sectio daemonem SUUM habet
 * (parare incipit in scratch sub TMPDIR, purgare necat et delet -
 * etiam post NECESSE fractum), ergo sectiones ordine liberae sunt. Olim
 * gradus
 * I-VII daemonem unum communem sequebantur (III res_id ex II, V
 * 'pipatum' ab II additum legebat, VII daemonem omnium necabat).
 */

#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "json.h"
#include "credo.h"
#include "filum.h"
#include "cliens_tabularii.h"
#include "lectiones.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>   /* getpid */

/* DAEMON SECTIONIS: scratch per processum sub TMPDIR (area membri sub
 * fabrica; olim build/probatio_ct.* - debita VI), portus, cliens */
nomen structura {
          character scratch[DXII];
            integer portus;
    ClientTabularii cliens;
} DaemonSectionis;

interior constans character*
_in_scratch (
    constans DaemonSectionis* d,
          constans character* titulus,
                     Piscina* piscina)
{
    character* via = (character*)piscina_allocare(piscina,
        strlen(d->scratch) + strlen(titulus) + II);

    sprintf(via, "%s/%s", d->scratch, titulus);
    redde via;
}

interior vacuum
_daemon_parare (
    CredoContextus* c)
{
       DaemonSectionis* d;
    constans character* tmp = getenv("TMPDIR");
        memoriae_index  n;
             character  imperium[MMXLVIII];
               integer  k;

    d = (DaemonSectionis*)piscina_allocare(c->piscina,
        magnitudo(DaemonSectionis));
    CREDO_NECESSE_NON_NIHIL(d);
    memset(d, ZEPHYRUM, magnitudo(DaemonSectionis));
    c->datum = d;
    si (tmp == NIHIL || tmp[0] == '\0')
    {
        tmp = "/tmp";
    }
    n = strlen(tmp);
    dum (n > I && tmp[n - I] == '/')
    {
        n--;
    }
    sprintf(d->scratch, "%.*s/probatio_ct.%d", (integer)(n
        > CD ? CD : n),
        tmp, (integer)getpid());
    (vacuum)filum_arborem_delere(d->scratch);
    CREDO_NECESSE(filum_directorium_creare_cum_parentibus(d->scratch));
    /* daemon PRAESTRUCTUS (actio tabulariumd): probatio currit, non
     * struit (fabrica plan 5 T5b) */
    CREDO_NECESSE(filum_existit("gesta/build/tabulariumd"));
    sprintf(imperium, "gesta/build/tabulariumd -portus 0"
        " -scrinium %s/ct.db -annales %s/ct.jsonl > %s/portus.txt"
        " 2> /dev/null & echo $! > %s/pid.txt", d->scratch, d->scratch,
        d->scratch, d->scratch);
    CREDO_NECESSE(system(imperium) == ZEPHYRUM);
    per (k = ZEPHYRUM; k < XXV; k++)
    {
        FILE* pf = lectiones_fopen(_in_scratch(d, "portus.txt",
            c->piscina), "r");

        si (pf != NIHIL)
        {
            si (   fscanf(pf, "portus %d", &d->portus) == I
                && d->portus > 0)
            {
                fclose(pf);
                frange;
            }
            fclose(pf);
        }
        (vacuum)system("sleep 0.2");
    }
    CREDO_NECESSE(d->portus > 0);
    cliens_tabularii_incipere(&d->cliens, (i32)d->portus);
}

/* SEMPER (etiam post NECESSE fractum): daemon necatur, scratch
 * deletur */
interior vacuum
_daemon_purgare (
    CredoContextus* c)
{
    DaemonSectionis* d = (DaemonSectionis*)c->datum;
          character  imperium[MXXIV];

    si (d == NIHIL || d->scratch[0] == '\0')
    {
        redde;
    }
    sprintf(imperium, "kill $(cat %s/pid.txt) 2>/dev/null", d->scratch);
    (vacuum)system(imperium);
    (vacuum)filum_arborem_delere(d->scratch);
}

/* 'pipatum' unum addere (pro sectionibus quae eo innituntur);
 * textus responsi */
interior chorda
_notam_addere (
    DaemonSectionis* d,
            Piscina* piscina,
             chorda* culpa)
{
    JsonValor* args = json_objectum_creare(piscina);

    json_objectum_ponere(args, "genus",
        json_chorda_creare_literis(piscina, "pipatum"));
    json_objectum_ponere(args, "titulus",
        json_chorda_creare_literis(piscina, "pipatum clientis"));
    json_objectum_ponere(args, "corpus",
        json_chorda_creare_literis(piscina, "per clientem extractum."));
    redde cliens_tabularii_vocare(&d->cliens, piscina, "addere", args,
        culpa);
}

/* I. initium: valores ordinarii */
interior vacuum
_probare_initium (
    CredoContextus* c)
{
    DaemonSectionis* d = (DaemonSectionis*)c->datum;

    CREDO_AEQUALIS_I32(d->cliens.portus, (i32)d->portus);
    CREDO_VERUM(strcmp(d->cliens.actor, "fran") == ZEPHYRUM);
    CREDO_FALSUM(d->cliens.genitus);
}

/* II-III. vocare (instrumentum reale trans TCP) et res_id ex prosa */
interior vacuum
_probare_vocare (
    CredoContextus* c)
{
    DaemonSectionis* d      = (DaemonSectionis*)c->datum;
             chorda  culpa  = chorda_ex_literis("", c->piscina);
             chorda  textus;

    textus = _notam_addere(d, c->piscina, &culpa);
    CREDO_NECESSE(textus.mensura > ZEPHYRUM);
    CREDO_VERUM(strstr(cliens_tabularii_litterae(c->piscina, textus),
        "creata") != NIHIL);
    CREDO_AEQUALIS_I32(cliens_tabularii_res_id(textus,
        c->piscina).mensura, XXVI);
}

/* IV. index petitionis crescit (id JSON-RPC unicus) */
interior vacuum
_probare_index (
    CredoContextus* c)
{
    DaemonSectionis* d      = (DaemonSectionis*)c->datum;
             chorda  culpa  = chorda_ex_literis("", c->piscina);
                s64  ante;

    ante = d->cliens.petitio_index;
    (vacuum)_notam_addere(d, c->piscina, &culpa);
    CREDO_VERUM(d->cliens.petitio_index > ante);
    CREDO_VERUM(d->cliens.petitio_index > (s64)I);
}

/* V. legere: tabulatum parsatum, quantum honoratum */
interior vacuum
_probare_legere (
    CredoContextus* c)
{
    DaemonSectionis* d      = (DaemonSectionis*)c->datum;
             chorda  culpa  = chorda_ex_literis("", c->piscina);
          JsonValor* res;

    CREDO_NECESSE(_notam_addere(d, c->piscina, &culpa).mensura
        > ZEPHYRUM);
    res = cliens_tabularii_legere(&d->cliens, c->piscina,
        chorda_ex_literis("pipatum", c->piscina), (i32)C, &culpa);
    CREDO_NECESSE_NON_NIHIL(res);
    CREDO_VERUM(json_est_tabulatum(res));
    CREDO_VERUM(json_tabulatum_numerus(res) >= I);
}

/* VI. tractator transmittere: allowlist + actor iniectus; instrumentum
 * extra allowlist RECUSATUR */
interior vacuum
_probare_transmittere (
    CredoContextus* c)
{
    DaemonSectionis* d          = (DaemonSectionis*)c->datum;
            Piscina* piscina    = c->piscina;
          JsonValor* arg        = json_objectum_creare(piscina);
          JsonValor* interiora  = json_objectum_creare(piscina);
          JsonValor* pravum     = json_objectum_creare(piscina);
          JsonValor* nihil_arg  = json_objectum_creare(piscina);
          JsonValor* fructus;
             chorda  culpa       = chorda_ex_literis("", piscina);

    json_objectum_ponere(interiora, "textus",
        json_chorda_creare_literis(piscina, "pipatum"));
    json_objectum_ponere(arg, "instrumentum",
        json_chorda_creare_literis(piscina, "quaerere"));
    json_objectum_ponere(arg, "argumenta", interiora);
    fructus = cliens_tabularii_transmittere(arg, piscina, &d->cliens,
        &culpa);
    CREDO_NON_NIHIL(fructus);

    json_objectum_ponere(pravum, "instrumentum",
        json_chorda_creare_literis(piscina, "delere-omnia"));
    json_objectum_ponere(pravum, "argumenta", nihil_arg);
    culpa = chorda_ex_literis("", piscina);
    fructus = cliens_tabularii_transmittere(pravum, piscina, &d->cliens,
        &culpa);
    CREDO_NIHIL(fructus);
    CREDO_VERUM(culpa.mensura > ZEPHYRUM);
}

/* VII. DEFECTUS CITUS: daemon mortuus non congelat. Lex subtilis quam
 * extractio servare debuit - launcher SEMEL excitatur, deinde defectus
 * statim, ne anquisitio IS (II s) tractatorem V s obstruentem ferat.
 * Daemon SECTIONIS necatur (olim communis - sectiones post VII nullae
 * esse poterant) */
interior vacuum
_probare_defectus_citus (
    CredoContextus* c)
{
     DaemonSectionis* d = (DaemonSectionis*)c->datum;
     ClientTabularii  mortuus;
           JsonValor* args;
              chorda  textus;
              chorda  culpa;
           character  imperium[MXXIV];

    sprintf(imperium, "kill $(cat %s/pid.txt) 2>/dev/null", d->scratch);
    (vacuum)system(imperium);
    (vacuum)system("sleep 0.3");

    cliens_tabularii_incipere(&mortuus, (i32)d->portus);
    /* launcher falsus: excitatio nihil facit, ergo conatus primus solus
     * moram fert; unus gradus (CC ms) sufficit */
    mortuus.launcher              = "true";
    mortuus.praefixum             = "[probatio]";
    mortuus.mora_excitationis_ms  = CC;

    args   = json_objectum_creare(c->piscina);
    culpa  = chorda_ex_literis("", c->piscina);
    textus = cliens_tabularii_vocare(&mortuus, c->piscina, "legere",
        args,
        &culpa);
    CREDO_AEQUALIS_I32(textus.mensura, ZEPHYRUM);
    CREDO_VERUM(culpa.mensura > ZEPHYRUM);
    CREDO_VERUM(mortuus.genitus);

    /* conatus SECUNDUS: statim, sine mora launcheri */
    culpa  = chorda_ex_literis("", c->piscina);
    textus = cliens_tabularii_vocare(&mortuus, c->piscina, "legere",
        args,
        &culpa);
    CREDO_AEQUALIS_I32(textus.mensura, ZEPHYRUM);
    CREDO_VERUM(culpa.mensura > ZEPHYRUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "initium clientis", _probare_initium,
      _daemon_parare, _daemon_purgare, NIHIL, NIHIL },
    { "vocare (addere) et res_id", _probare_vocare,
      _daemon_parare, _daemon_purgare, NIHIL, NIHIL },
    { "index petitionis", _probare_index,
      _daemon_parare, _daemon_purgare, NIHIL, NIHIL },
    { "legere", _probare_legere,
      _daemon_parare, _daemon_purgare, NIHIL, NIHIL },
    { "transmittere", _probare_transmittere,
      _daemon_parare, _daemon_purgare, NIHIL, NIHIL },
    { "defectus citus", _probare_defectus_citus,
      _daemon_parare, _daemon_purgare, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("cliens_tabularii", SECTIONES);
}
