/* probatio_praeconium_cantus.c - eventus 'cantus' (praeco cantus)
 *
 * parare: cantilena iudicata -> eventus "cantus", genus "intactus"
 * aut "", nuntius = textus; campi vocantis intacti. limes: CC octeti
 * licent, CCI recusantur (offset CC) - liber praecideret. vitia: offset
 * lectoris cantus, eventus intactus. iter: scriptum per
 * praeconium_scribere et relectum per praeconium_legere_ab idem est
 * ('#', '<', '>' per JSON).
 */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "praeconium.h"
#include "praeconium_cantus.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

/* eventus cum campis vocantis notis */
interior vacuum
_eventum_parare (
       CredoContextus* c,
    PraeconiumEventus* e)
{
    memset(e, ZEPHYRUM, magnitudo(PraeconiumEventus));
    e->tempus    = CXXIII;
    e->eventus   = chorda_ex_literis("x", c->piscina);
    e->genus     = chorda_ex_literis("y", c->piscina);
    e->nuntius   = chorda_ex_literis("z", c->piscina);
    e->panellus  = chorda_ex_literis("%3", c->piscina);
    e->via       = chorda_ex_literis("/w/alpha", c->piscina);
}

/* eventus ut _eventum_parare reliquit */
interior b32
_intactus (
    constans PraeconiumEventus* e)
{
    redde chorda_aequalis_literis(e->eventus, "x")
        && chorda_aequalis_literis(e->genus, "y")
        && chorda_aequalis_literis(e->nuntius, "z");
}

interior vacuum
_probare_parare (
    CredoContextus* c)
{
     PraeconiumEventus e;
        CantualeVitium vitium;

    _eventum_parare(c, &e);
    CREDO_VERUM(praeconium_cantum_parare(chorda_ex_literis(
        "t150 o5 l16 c e g8", c->piscina), FALSUM, &e, c->piscina,
        &vitium));
    CREDO_CHORDA_AEQUALIS_LITERIS(e.eventus, "cantus");
    CREDO_CHORDA_AEQUALIS_LITERIS(e.genus, "");
    CREDO_CHORDA_AEQUALIS_LITERIS(e.nuntius, "t150 o5 l16 c e g8");
    /* campi vocantis */
    CREDO_AEQUALIS_S64(e.tempus, (s64)CXXIII);
    CREDO_CHORDA_AEQUALIS_LITERIS(e.panellus, "%3");
    CREDO_CHORDA_AEQUALIS_LITERIS(e.via, "/w/alpha");

    _eventum_parare(c, &e);
    CREDO_VERUM(praeconium_cantum_parare(chorda_ex_literis(
        "@punctum c r c", c->piscina), VERUM, &e, c->piscina, &vitium));
    CREDO_CHORDA_AEQUALIS_LITERIS(e.genus, "intactus");
    CREDO_CHORDA_AEQUALIS_LITERIS(e.eventus, "cantus");
}

interior vacuum
_probare_limitem (
    CredoContextus* c)
{
     PraeconiumEventus e;
        CantualeVitium vitium;
             character alveus[CCLVI];
                chorda textus;

    memset(alveus, ' ', CCI);
    memcpy(alveus, "t240 c", VI);
    textus.datum    = (i8*)alveus;
    textus.mensura  = PRAECONIUM_NUNTIUS_MAXIMUS;
    _eventum_parare(c, &e);
    CREDO_VERUM(praeconium_cantum_parare(textus, FALSUM, &e, c->piscina,
        &vitium));
    CREDO_AEQUALIS_I32(e.nuntius.mensura, PRAECONIUM_NUNTIUS_MAXIMUS);

    textus.mensura  = PRAECONIUM_NUNTIUS_MAXIMUS + I;
    vitium.offset   = ZEPHYRUM;
    vitium.causa    = NIHIL;
    _eventum_parare(c, &e);
    CREDO_FALSUM(praeconium_cantum_parare(textus, FALSUM, &e,
        c->piscina, &vitium));
    CREDO_AEQUALIS_S32((s32)vitium.offset,
        (s32)PRAECONIUM_NUNTIUS_MAXIMUS);
    CREDO_VERUM(vitium.causa != NIHIL);
    CREDO_VERUM(_intactus(&e));
}

interior vacuum
_probare_vitia (
    CredoContextus* c)
{
     PraeconiumEventus e;
        CantualeVitium vitium;

    _eventum_parare(c, &e);
    vitium.offset  = CMXCIX;
    vitium.causa   = NIHIL;
    CREDO_FALSUM(praeconium_cantum_parare(chorda_ex_literis("t180 l3 c",
        c->piscina), FALSUM, &e, c->piscina, &vitium));
    CREDO_AEQUALIS_S32((s32)vitium.offset, VI);
    CREDO_VERUM(vitium.causa != NIHIL);
    CREDO_VERUM(_intactus(&e));

    _eventum_parare(c, &e);
    CREDO_FALSUM(praeconium_cantum_parare(chorda_ex_literis(
        "c @nullum d", c->piscina), FALSUM, &e, c->piscina, &vitium));
    CREDO_AEQUALIS_S32((s32)vitium.offset, II);
    CREDO_VERUM(_intactus(&e));

    _eventum_parare(c, &e);
    CREDO_FALSUM(praeconium_cantum_parare(chorda_ex_literis("",
        c->piscina), FALSUM, &e, c->piscina, &vitium));
    CREDO_VERUM(_intactus(&e));
}

interior vacuum
_probare_iter (
    CredoContextus* c)
{
             PraeconiumEventus  e;
                CantualeVitium  vitium;
    constans PraeconiumEventus* r;
            constans character* area;
                     character  via[CCLVI];
                           Xar* lecti;
                           s64  positio;

    area = credo_area(c);
    CREDO_NECESSE(area != NIHIL);
    sprintf(via, "%s/eventa.jsonl", area);
    _eventum_parare(c, &e);
    CREDO_NECESSE(praeconium_cantum_parare(chorda_ex_literis(
        "t150 o5 l16 c# > c < g8", c->piscina), VERUM, &e, c->piscina,
        &vitium));
    CREDO_NECESSE(praeconium_scribere(via, &e, c->piscina));
    positio  = ZEPHYRUM;
    lecti    = praeconium_legere_ab(via, &positio, c->piscina);
    CREDO_NECESSE(lecti != NIHIL && xar_numerus(lecti) == I);
    r = (constans PraeconiumEventus*)xar_obtinere(lecti, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(r->eventus, "cantus");
    CREDO_CHORDA_AEQUALIS_LITERIS(r->genus, "intactus");
    CREDO_CHORDA_AEQUALIS_LITERIS(r->nuntius,
        "t150 o5 l16 c# > c < g8");
    CREDO_CHORDA_AEQUALIS_LITERIS(r->panellus, "%3");
    CREDO_AEQUALIS_S64(r->tempus, (s64)CXXIII);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "parare", _probare_parare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "limes", _probare_limitem, NIHIL, NIHIL, NIHIL, NIHIL },
    { "vitia", _probare_vitia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "iter", _probare_iter, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("praeconium_cantus", SECTIONES);
}
