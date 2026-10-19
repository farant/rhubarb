/* probatio_provenientia.c - relatio '-provenientia' binariorum
 * installatorum (plan fabrica 1a T7). */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "provenientia.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

interior vacuum
_suitam_parare (
    CredoContextus* c)
{
    ProvenientiaRelatio* relatio;

    relatio = (ProvenientiaRelatio*)piscina_allocare(c->piscina,
        magnitudo(ProvenientiaRelatio));
    relatio->artificium = "bin/manus";
    relatio->ingressus = "0123456789abcdef0123456789abcdef"
        "0123456789abcdef0123456789abcdef";
    relatio->commissum  = "abc12345 SORDIDUM";
    c->datum            = relatio;
}


/* ==================================================
 * PROBARE: textus - quattuor lineae exactae
 * ================================================== */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    constans ProvenientiaRelatio* relatio =
        (constans ProvenientiaRelatio*)c->suita;
                           chorda textus;

    textus = provenientia_textus(relatio, c->piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(textus,
        "provenientia 1\n"
        "artificium bin/manus\n"
        "ingressus 0123456789abcdef0123456789abcdef"
        "0123456789abcdef0123456789abcdef\n"
        "commissum abc12345 SORDIDUM\n");
}


/* ==================================================
 * PROBARE: respondere - vexillum solum, argumentum unicum
 * ================================================== */

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    constans ProvenientiaRelatio* relatio =
        (constans ProvenientiaRelatio*)c->suita;
                       character* sine[II];
                       character* cum[II];
                       character* cum_alio[III];
                       character  nomen_binarii[16];
                       character  vexillum[16];
                       character  aliud[16];

    strcpy(nomen_binarii, "bin/manus");
    strcpy(vexillum, "-provenientia");
    strcpy(aliud, "-versio");
    sine[0]      = nomen_binarii;
    sine[1]      = aliud;
    cum[0]       = nomen_binarii;
    cum[1]       = vexillum;
    cum_alio[0]  = nomen_binarii;
    cum_alio[1]  = vexillum;
    cum_alio[2]  = aliud;

    CREDO_FALSUM(provenientia_respondere(I, sine, relatio));
    CREDO_FALSUM(provenientia_respondere(II, sine, relatio));
    /* vexillum cum argumentis aliis = usus alius, non relatio */
    CREDO_FALSUM(provenientia_respondere(III, cum_alio, relatio));
    CREDO_VERUM(provenientia_respondere(II, cum, relatio));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "textum", _sectio_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "respondere", _sectio_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("provenientia", SECTIONES,
        _suitam_parare, NIHIL);
}
