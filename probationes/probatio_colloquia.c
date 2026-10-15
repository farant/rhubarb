/* probatio_colloquia.c - sessiones Claude Code (colloquia-plan T3):
 * transitus status per praefixa libri fixi, linea explicita per
 * panellum (panellus iterum adhibitus a sessione nova), ordo, tituli
 * ex caudis transcriptorum, panelli vivi (fixum multiplexoris), totum
 * per colloquia_legere sine tmux. Nullum tmux verum tangitur. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "filum.h"
#include "praeconium.h"
#include "multiplexor.h"
#include "colloquia.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define VIA_FIXORUM "probationes/fixa/colloquia/"

/* eventus fixi omnes (linea mala omissa) */
interior Xar*
eventus_fixi (
    Piscina* p)
{
    redde praeconium_caudam_legere(VIA_FIXORUM "eventa.jsonl", M * M,
        p);
}

/* praefixum k eventuum */
interior Xar*
praefixum (
        Xar* omnes,
        i32  k,
    Piscina* p)
{
    Xar* l;
    i32  i;

    l = xar_creare(p, (i32)magnitudo(PraeconiumEventus));
    per (i = ZEPHYRUM; l && i < k && i < xar_numerus(omnes); i++)
    {
        *(PraeconiumEventus*)xar_addere(l) = *(PraeconiumEventus*)
            xar_obtinere(omnes, i);
    }
    redde l;
}

interior constans Colloquium*
colloquium (
                   Xar* l,
    constans character* sessio)
{
    constans Colloquium* c;
                    i32  i;

    per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
    {
        c = (constans Colloquium*)xar_obtinere(l, i);
        si (chorda_aequalis_literis(c->sessio, sessio))
        {
            redde c;
        }
    }
    redde NIHIL;
}

/* status et linea post k eventus */
interior vacuum
statum_probare (
                 Xar* omnes,
                  i32  k,
    constans character* sessio,
       StatusColloquii  status,
    constans character* linea,
              Piscina* p)
{
    constans Colloquium* c;

    c = colloquium(colloquia_ex_eventibus(praefixum(omnes, k, p), p),
        sessio);
    CREDO_NECESSE(c != NIHIL);
    si (   (s32)c->status != (s32)status
        || !chorda_aequalis_literis(c->linea, linea))
    {
        imprimere("  post %d: %s status %d linea '%.*s'\n", (integer)k,
            sessio, (integer)c->status, (integer)c->linea.mensura,
            (constans character*)c->linea.datum);
    }
    CREDO_AEQUALIS_S32((s32)c->status, (s32)status);
    CREDO_CHORDA_AEQUALIS_LITERIS(c->linea, linea);
}

/* transitus: start otiosum, prompt laborat, instrumentum cum linea
 * "genus: nuntius", status explicitus vincit, licentia exspectat, Stop
 * perfecit (prima linea responsi), idle otiosum, StopFailure exspectat,
 * SessionEnd clausum */
interior vacuum
_probare_transitus (
    CredoContextus* c)
{
    Xar* omnes;

    omnes = eventus_fixi(c->piscina);
    CREDO_NECESSE(omnes != NIHIL);
    CREDO_AEQUALIS_I32(xar_numerus(omnes), XV);
    statum_probare(omnes, II, "s-a", COLLOQUIUM_OTIOSUM, "",
        c->piscina);
    statum_probare(omnes, III, "s-a", COLLOQUIUM_LABORAT,
        "build the thing", c->piscina);
    statum_probare(omnes, IV, "s-a", COLLOQUIUM_LABORAT,
        "Bash: Run calendar suites", c->piscina);
    /* status explicitus (sine sessione, panellus %1) */
    statum_probare(omnes, V, "s-a", COLLOQUIUM_LABORAT,
        "merging main into secunda", c->piscina);
    statum_probare(omnes, VI, "s-a", COLLOQUIUM_LABORAT,
        "merging main into secunda", c->piscina);
    statum_probare(omnes, VII, "s-a", COLLOQUIUM_EXSPECTAT,
        "merging main into secunda", c->piscina);
    statum_probare(omnes, VIII, "s-a", COLLOQUIUM_PERFECIT,
        "merging main into secunda", c->piscina);
    /* s-b: Stop sine linea explicita - prima linea responsi */
    statum_probare(omnes, X, "s-b", COLLOQUIUM_PERFECIT,
        "All logs clean.",
        c->piscina);
    statum_probare(omnes, XI, "s-b", COLLOQUIUM_OTIOSUM,
        "All logs clean.",
        c->piscina);
    statum_probare(omnes, XII, "s-b", COLLOQUIUM_EXSPECTAT,
        "API error: overloaded", c->piscina);
    statum_probare(omnes, XIII, "s-a", COLLOQUIUM_CLAUSUM,
        "merging main into secunda", c->piscina);
}

/* linea explicita sine prompto novo: Stop prima linea solum ubi nulla
 * explicita (s-a sine eventu V) */
interior vacuum
_probare_stop_lineam (
    CredoContextus* c)
{
                    Xar* omnes;
                    Xar* sine;
                    i32  i;
    constans Colloquium* k;

    omnes  = eventus_fixi(c->piscina);
    sine   = xar_creare(c->piscina, (i32)magnitudo(PraeconiumEventus));
    CREDO_NECESSE(omnes && sine);
    per (i = ZEPHYRUM; i < VIII; i++)
    {
        si (i != IV)
        {
            *(PraeconiumEventus*)xar_addere(sine) =
                *(PraeconiumEventus*)
                xar_obtinere(omnes, i);
        }
    }
    k = colloquium(colloquia_ex_eventibus(sine, c->piscina), "s-a");
    CREDO_NECESSE(k != NIHIL);
    CREDO_AEQUALIS_S32((s32)k->status, (s32)COLLOQUIUM_PERFECIT);
    CREDO_CHORDA_AEQUALIS_LITERIS(k->linea, "Done: built.");
}

/* panellus iterum adhibitus: status %1 post SessionEnd s-a et
 * SessionStart s-c ad s-c pertinet; ordo recentissimum primum */
interior vacuum
_probare_panellum_et_ordinem (
    CredoContextus* c)
{
                    Xar* l;
    constans Colloquium* k;

    l = colloquia_ex_eventibus(eventus_fixi(c->piscina), c->piscina);
    CREDO_NECESSE(l != NIHIL);
    CREDO_AEQUALIS_I32(xar_numerus(l), IV);
    k = colloquium(l, "s-c");
    CREDO_NECESSE(k != NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS(k->linea, "testing C");
    CREDO_AEQUALIS_S32((s32)k->status, (s32)COLLOQUIUM_OTIOSUM);
    k = colloquium(l, "s-a");
    CREDO_CHORDA_AEQUALIS_LITERIS(k->linea,
        "merging main into secunda");
    CREDO_CHORDA_AEQUALIS_LITERIS(k->panellus, "%1");
    CREDO_CHORDA_AEQUALIS_LITERIS(k->via, "/w/alpha");
    CREDO_AEQUALIS_S64(k->tempus, (s64)CCC);
    /* ordo: c (CCCXX), a (CCC), b (CCXX), d (L) */
    CREDO_CHORDA_AEQUALIS_LITERIS(((constans Colloquium*)xar_obtinere(l,
        ZEPHYRUM))->sessio, "s-c");
    CREDO_CHORDA_AEQUALIS_LITERIS(((constans Colloquium*)xar_obtinere(l,
        I))->sessio, "s-a");
    CREDO_CHORDA_AEQUALIS_LITERIS(((constans Colloquium*)xar_obtinere(l,
        II))->sessio, "s-b");
    CREDO_CHORDA_AEQUALIS_LITERIS(((constans Colloquium*)xar_obtinere(l,
        III))->sessio, "s-d");
}

/* tituli: custom-title vincit ai-title; ai-title ultimus; absens
 * vacuus; linea fracta in initio caudae omissa */
interior vacuum
_probare_titulos (
    CredoContextus* c)
{
    CREDO_CHORDA_AEQUALIS_LITERIS(colloquia_titulum_legere(VIA_FIXORUM
        "transcriptum_a.jsonl", c->piscina), "secunda");
    CREDO_CHORDA_AEQUALIS_LITERIS(colloquia_titulum_legere(VIA_FIXORUM
        "transcriptum_b.jsonl", c->piscina), "Log check session");
    CREDO_AEQUALIS_I32(colloquia_titulum_legere(VIA_FIXORUM
        "nullum.jsonl", c->piscina).mensura, ZEPHYRUM);
}

/* panelli vivi: %9 absens -> CLAUSUM; %1 vivit */
interior vacuum
_probare_panellos (
    CredoContextus* c)
{
                    Xar* l;
                    Xar* panelli;
    constans Colloquium* k;

    l        = colloquia_ex_eventibus(eventus_fixi(c->piscina),
        c->piscina);
    panelli  =
        multiplexor_panellos_legere(filum_legere_totum(VIA_FIXORUM
        "panelli.txt", c->piscina), c->piscina);
    CREDO_NECESSE(l && panelli && xar_numerus(panelli) == II);
    colloquia_panellos_applicare(l, panelli);
    k = colloquium(l, "s-d");
    CREDO_NECESSE(k != NIHIL);
    CREDO_FALSUM(k->panellus_vivit);
    CREDO_AEQUALIS_S32((s32)k->status, (s32)COLLOQUIUM_CLAUSUM);
    k = colloquium(l, "s-c");
    CREDO_VERUM(k->panellus_vivit);
    CREDO_AEQUALIS_S32((s32)k->status, (s32)COLLOQUIUM_OTIOSUM);
    k = colloquium(l, "s-b");
    CREDO_VERUM(k->panellus_vivit);
    CREDO_AEQUALIS_S32((s32)k->status, (s32)COLLOQUIUM_EXSPECTAT);
}

/* totum sine tmux: tituli ex transcriptis, aliter nomen directorii;
 * panelli non applicantur (d manet OTIOSUM) */
interior vacuum
_probare_legere (
    CredoContextus* c)
{
                    Xar* l;
    constans Colloquium* k;

    l = colloquia_legere(VIA_FIXORUM "eventa.jsonl", NIHIL, M * M,
        c->piscina);
    CREDO_NECESSE(l != NIHIL && xar_numerus(l) == IV);
    CREDO_CHORDA_AEQUALIS_LITERIS(colloquium(l, "s-a")->titulus,
        "secunda");
    CREDO_CHORDA_AEQUALIS_LITERIS(colloquium(l, "s-b")->titulus,
        "Log check session");
    CREDO_CHORDA_AEQUALIS_LITERIS(colloquium(l, "s-c")->titulus,
        "gamma");
    k = colloquium(l, "s-d");
    CREDO_CHORDA_AEQUALIS_LITERIS(k->titulus, "delta");
    CREDO_AEQUALIS_S32((s32)k->status, (s32)COLLOQUIUM_OTIOSUM);
    /* liber absens: Xar vacuus */
    l = colloquia_legere("/nusquam/eventa.jsonl", NIHIL, M, c->piscina);
    CREDO_NECESSE(l != NIHIL);
    CREDO_AEQUALIS_I32(xar_numerus(l), ZEPHYRUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "transitus", _probare_transitus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "stop_linea", _probare_stop_lineam, NIHIL, NIHIL, NIHIL, NIHIL },
    { "panellus_ordo", _probare_panellum_et_ordinem, NIHIL, NIHIL,
        NIHIL,
      NIHIL },
    { "tituli", _probare_titulos, NIHIL, NIHIL, NIHIL, NIHIL },
    { "panelli", _probare_panellos, NIHIL, NIHIL, NIHIL, NIHIL },
    { "legere", _probare_legere, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("colloquia", SECTIONES);
}
