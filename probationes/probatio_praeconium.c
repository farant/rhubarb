/* probatio_praeconium.c - acta colloquiorum (colloquia-plan T2): ex
 * hamo (Notification, PreToolUse, UserPromptSubmit praecisus UTF-8
 * integro, Stop, SessionStart, JSON malum), via (RHUBARB_COLLOQUIA),
 * scribere et caudam legere, scriptores concurrentes (IV processus x L
 * eventus). Omnia in $TMPDIR/probatio_praeconium.<pid>/ (purgare
 * delet). */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "filum.h"
#include "praeconium.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define VIA_FIXORUM "probationes/fixa/praeconium/"

nomen structura {
    character directorium[CCLVI];
    character liber[CCLVI];
} Area;

interior vacuum
aream_parare (
    CredoContextus* c)
{
                  Area* a;
    constans character* tmp;

    a = (Area*)piscina_allocare(c->piscina, magnitudo(Area));
    memset(a, ZEPHYRUM, magnitudo(Area));
    tmp = getenv("TMPDIR");
    sprintf(a->directorium, "%s/probatio_praeconium.%d", tmp ? tmp
        : "/tmp", (integer)getpid());
    sprintf(a->liber, "%s/eventa.jsonl", a->directorium);
    c->datum = a;
}

interior vacuum
aream_purgare (
    CredoContextus* c)
{
    Area* a;

    a = (Area*)c->datum;
    si (a)
    {
        (vacuum)filum_delere(a->liber);
        (vacuum)rmdir(a->directorium);
    }
}

interior b32
ex_fixo (
    constans character* nomen_fixi,
    constans character* panellus,
    constans character* tmux,
     PraeconiumEventus* e,
               Piscina* p)
{
    character via[CCLVI];

    sprintf(via, VIA_FIXORUM "%s", nomen_fixi);
    redde praeconium_ex_hamo(filum_legere_totum(via, p), panellus, tmux,
        M, e, p);
}

/* ex hamo: campi communes et proprii cuiusque eventus */
interior vacuum
_probare_ex_hamo (
    CredoContextus* c)
{
    PraeconiumEventus e;

    CREDO_NECESSE(ex_fixo("notificatio.json", "%17",
        "/private/tmp/tmux-501/default,1091,2", &e, c->piscina));
    CREDO_AEQUALIS_S64(e.tempus, (s64)M);
    CREDO_CHORDA_AEQUALIS_LITERIS(e.sessio, "s-111");
    CREDO_CHORDA_AEQUALIS_LITERIS(e.eventus, "Notification");
    CREDO_CHORDA_AEQUALIS_LITERIS(e.panellus, "%17");
    CREDO_CHORDA_AEQUALIS_LITERIS(e.servitor,
        "/private/tmp/tmux-501/default");
    CREDO_CHORDA_AEQUALIS_LITERIS(e.via, "/Users/x/proj");
    CREDO_CHORDA_AEQUALIS_LITERIS(e.transcriptum,
        "/Users/x/.claude/projects/p/s-111.jsonl");
    CREDO_CHORDA_AEQUALIS_LITERIS(e.genus, "permission_prompt");
    CREDO_CHORDA_AEQUALIS_LITERIS(e.nuntius,
        "Claude needs your permission to use Bash");

    /* PreToolUse: genus = tool_name, nuntius = descriptio (aut
     * imperium) */
    CREDO_NECESSE(ex_fixo("pre_tool.json", "%3", NIHIL, &e,
        c->piscina));
    CREDO_CHORDA_AEQUALIS_LITERIS(e.genus, "Bash");
    CREDO_CHORDA_AEQUALIS_LITERIS(e.nuntius, "Run calendar suites");
    CREDO_AEQUALIS_I32(e.servitor.mensura, ZEPHYRUM);

    /* Stop: ultimum responsum (lineae novae servatae in chorda) */
    CREDO_NECESSE(ex_fixo("stop.json", NIHIL, NIHIL, &e, c->piscina));
    CREDO_CHORDA_AEQUALIS_LITERIS(e.eventus, "Stop");
    CREDO_CHORDA_AEQUALIS_LITERIS(e.nuntius,
        "Done: all gates green.\nNext: T2.");
    CREDO_AEQUALIS_I32(e.panellus.mensura, ZEPHYRUM);

    /* SessionStart: genus = trigger */
    CREDO_NECESSE(ex_fixo("session_start.json", NIHIL, NIHIL, &e,
        c->piscina));
    CREDO_CHORDA_AEQUALIS_LITERIS(e.genus, "resume");

    /* JSON malum */
    CREDO_FALSUM(ex_fixo("malum.json", NIHIL, NIHIL, &e, c->piscina));
}

/* prompt longus: praecisus ad CC octetos summum, praefixum originis,
 * nullus character UTF-8 fractus in fine */
interior vacuum
_probare_praecisionem (
    CredoContextus* c)
{
    PraeconiumEventus e;
                  s32 i;
                  i32 sequentes;

    CREDO_NECESSE(ex_fixo("prompt_longum.json", NIHIL, NIHIL, &e,
        c->piscina));
    CREDO_VERUM(e.nuntius.mensura > C && e.nuntius.mensura <= CC);
    CREDO_VERUM(chorda_incipit(e.nuntius,
        chorda_ex_literis("Fac hoc: añ",
        c->piscina)));
    /* finis: octetus ultimus non initium sequentiae imperfectae */
    sequentes = ZEPHYRUM;
    per (i = (s32)e.nuntius.mensura - I; i >= ZEPHYRUM; i--)
    {
        insignatus character o =
            (insignatus character)e.nuntius.datum[i];

        si ((o & CXCII) == CXXVIII)
        {
            sequentes++;
            perge;
        }
        si (o >= CXCII)
        {
            /* initium multi-octeti: totum adesse debet */
            CREDO_AEQUALIS_I32(sequentes, (o >= CCXXIV) ? II : I);
        }
        frange;
    }
}

/* via: RHUBARB_COLLOQUIA/eventa.jsonl; aliter
 * HOME/.rhubarb/colloquia/eventa.jsonl */
interior vacuum
_probare_viam (
    CredoContextus* c)
{
         Area* a;
    character  exspectata[CCLVI];

    a = (Area*)c->datum;
    CREDO_NECESSE(setenv("RHUBARB_COLLOQUIA", a->directorium, I)
        == ZEPHYRUM);
    CREDO_VERUM(strcmp(praeconium_via(c->piscina), a->liber)
        == ZEPHYRUM);
    (vacuum)unsetenv("RHUBARB_COLLOQUIA");
    sprintf(exspectata, "%s/.rhubarb/colloquia/eventa.jsonl",
        getenv("HOME"));
    CREDO_VERUM(strcmp(praeconium_via(c->piscina), exspectata)
        == ZEPHYRUM);
}

/* scribere et caudam legere: eventus integri, ordo; cauda parva solum
 * ultimos reddit (linea prima fracta omissa) */
interior vacuum
_probare_scribere_legere (
    CredoContextus* c)
{
                          Area* a;
             PraeconiumEventus  e;
                           Xar* l;
    constans PraeconiumEventus* r;
                           i32  i;
                     character  numerus[XVI];

    a = (Area*)c->datum;
    CREDO_NECESSE(ex_fixo("stop.json", "%9", NIHIL, &e, c->piscina));
    per (i = ZEPHYRUM; i < X; i++)
    {
        sprintf(numerus, "eventus %d", (integer)i);
        e.nuntius  = chorda_ex_literis(numerus, c->piscina);
        e.tempus   = (s64)i;
        CREDO_NECESSE(praeconium_scribere(a->liber, &e, c->piscina));
    }
    l = praeconium_caudam_legere(a->liber, M * M, c->piscina);
    CREDO_NECESSE(l != NIHIL);
    CREDO_AEQUALIS_I32(xar_numerus(l), X);
    r = (constans PraeconiumEventus*)xar_obtinere(l, IX);
    CREDO_CHORDA_AEQUALIS_LITERIS(r->nuntius, "eventus 9");
    CREDO_CHORDA_AEQUALIS_LITERIS(r->sessio, "s-444");
    CREDO_CHORDA_AEQUALIS_LITERIS(r->panellus, "%9");
    CREDO_AEQUALIS_S64(r->tempus, (s64)IX);

    /* cauda CCL octetorum: pauci ultimi, primus integer */
    l = praeconium_caudam_legere(a->liber, CCL, c->piscina);
    CREDO_NECESSE(l != NIHIL);
    CREDO_VERUM(xar_numerus(l) >= I && xar_numerus(l) < X);
    r = (constans PraeconiumEventus*)xar_obtinere(l,
        (i32)xar_numerus(l) - I);
    CREDO_CHORDA_AEQUALIS_LITERIS(r->nuntius, "eventus 9");
    r = (constans PraeconiumEventus*)xar_obtinere(l, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(r->sessio, "s-444");

    /* liber absens: Xar vacuus */
    l = praeconium_caudam_legere("/nusquam/eventa.jsonl", M,
        c->piscina);
    CREDO_NECESSE(l != NIHIL);
    CREDO_AEQUALIS_I32(xar_numerus(l), ZEPHYRUM);
}

/* scriptores concurrentes: IV processus x L eventus - omnes lineae
 * integrae */
interior vacuum
_probare_concurrentes (
    CredoContextus* c)
{
                  Area* a;
                   Xar* l;
                   i32  k;
                   i32  i;
                 pid_t  filii[IV];
               integer  status;

    a = (Area*)c->datum;
    (vacuum)filum_delere(a->liber);
    per (k = ZEPHYRUM; k < IV; k++)
    {
        filii[k] = fork();
        si (filii[k] == ZEPHYRUM)
        {
                      Piscina* p;
            PraeconiumEventus  e;

            p = piscina_generare_dynamicum("praeconium_filius",
                M * LXIV);
            si (!p || !ex_fixo("pre_tool.json", "%1", NIHIL, &e, p))
            {
                _exit(I);
            }
            per (i = ZEPHYRUM; i < L; i++)
            {
                e.tempus = (s64)(k * C + i);
                si (!praeconium_scribere(a->liber, &e, p))
                {
                    _exit(II);
                }
            }
            _exit(ZEPHYRUM);
        }
    }
    per (k = ZEPHYRUM; k < IV; k++)
    {
        CREDO_NECESSE(filii[k] > ZEPHYRUM);
        CREDO_NECESSE(waitpid(filii[k], &status, ZEPHYRUM) == filii[k]);
        CREDO_VERUM(WIFEXITED(status) && WEXITSTATUS(status)
            == ZEPHYRUM);
    }
    l = praeconium_caudam_legere(a->liber, M * M, c->piscina);
    CREDO_NECESSE(l != NIHIL);
    CREDO_AEQUALIS_I32(xar_numerus(l), IV * L);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "ex_hamo", _probare_ex_hamo, NIHIL, NIHIL, NIHIL, NIHIL },
    { "praecisio", _probare_praecisionem, NIHIL, NIHIL, NIHIL, NIHIL },
    { "via", _probare_viam, aream_parare, aream_purgare, NIHIL, NIHIL },
    { "scribere_legere", _probare_scribere_legere, aream_parare,
      aream_purgare, NIHIL, NIHIL },
    { "concurrentes", _probare_concurrentes, aream_parare,
      aream_purgare, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("praeconium", SECTIONES);
}
