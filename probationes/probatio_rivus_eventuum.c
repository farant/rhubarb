/* probatio_rivus_eventuum.c - Server-Sent Events, lector incrementalis
 * (quaestio-rivus-spec II.3)
 *
 * Casus quisque TOTUS, OCTETO SINGULO et ad omnem divisionem binam
 * adicitur; acta "<eventus>=<data>|" per dispatchum eadem esse
 * debent. Nomen ordinarium "message", data multilinea, commentarium,
 * fines linearum CRLF / LF / CR (etiam CR ad finem adiectionis),
 * spatium unum post ':', campi neglecti, data vacua, BOM, finis
 * (eventus imperfectus abicitur), forma Anthropic vera.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "credo.h"
#include "rivus_eventuum.h"
#include <string.h>

interior vacuum
_notare (
    vacuum* datum,
    chorda  eventus,
    chorda  data)
{
    ChordaAedificator* aed = (ChordaAedificator*)datum;

    chorda_aedificator_appendere_chorda(aed, eventus);
    chorda_aedificator_appendere_character(aed, '=');
    chorda_aedificator_appendere_chorda(aed, data);
    chorda_aedificator_appendere_character(aed, '|');
}

/* modus: 0 totus, 1 octetus singulus, k > 1 = divisio ad k */
interior chorda
_currere (
    constans character* fluxus,
                   s32  modus,
                   b32  finire,
               Piscina* p)
{
    ChordaAedificator* aed  = chorda_aedificator_creare(p, LXIV);
        RivusEventuum* r    = rivus_eventuum_creare(p, _notare, aed);
                  i32  n    = (i32)strlen(fluxus);
                  i32  k;

    si (modus == ZEPHYRUM)
    {
        rivus_eventuum_adicere(r, (constans i8*)fluxus, n);
    }
    alioquin si (modus == I)
    {
        per (k = ZEPHYRUM; k < n; k++)
        {
            rivus_eventuum_adicere(r, (constans i8*)fluxus + k, I);
        }
    }
    alioquin
    {
        rivus_eventuum_adicere(r, (constans i8*)fluxus, (i32)modus);
        rivus_eventuum_adicere(r, (constans i8*)fluxus + modus,
            n - (i32)modus);
    }
    si (finire)
    {
        rivus_eventuum_finire(r);
    }
    redde chorda_aedificator_finire(aed);
}

/* omnes divisiones: acta aequalia exspectatis; numerus fractorum */
interior i32
_fracta (
    constans character* fluxus,
                   b32  finire,
    constans character* exspectata,
               Piscina* p)
{
    s32 modus;
    s32 n     = (s32)strlen(fluxus);
    i32 mala  = ZEPHYRUM;

    per (modus = ZEPHYRUM; modus < n; modus++)
    {
        si (!chorda_aequalis_literis(_currere(fluxus, modus, finire, p),
                exspectata))
        {
            mala++;
        }
    }
    redde mala;
}

interior vacuum
_sectio_commune (
    CredoContextus* c)
{
    /* nomen ordinarium "message"; data multilinea; commentarium */
    CREDO_AEQUALIS_I32(_fracta(": commentarium\ndata: a\ndata: b\n\n",
        FALSUM, "message=a\nb|", c->piscina), ZEPHYRUM);
}

interior vacuum
_sectio_fines_linearum (
    CredoContextus* c)
{
    /* event nominatum; CRLF et CR solum */
    CREDO_AEQUALIS_I32(_fracta(
        "event: x\r\ndata: 1\r\n\r\nevent: y\rdata: 2\r\r", FALSUM,
        "x=1|y=2|", c->piscina), ZEPHYRUM);
    /* LF post CR in adiectione proxima: finis unus, non duo */
    CREDO_AEQUALIS_I32(_fracta("data: a\r\n\r\ndata: b\r\n\r\n", FALSUM,
        "message=a|message=b|", c->piscina), ZEPHYRUM);
}

interior vacuum
_sectio_spatia (
    CredoContextus* c)
{
    /* spatium unum solum tollitur; sine ':' campus totus, valor
     * vacuus */
    CREDO_AEQUALIS_I32(_fracta("data:  duo\ndata\n\n", FALSUM,
        "message= duo\n|", c->piscina), ZEPHYRUM);
    /* sine spatio post ':' */
    CREDO_AEQUALIS_I32(_fracta("event:e\ndata:v\n\n", FALSUM, "e=v|",
        c->piscina), ZEPHYRUM);
}

interior vacuum
_sectio_neglecta (
    CredoContextus* c)
{
    /* id, retry, campi ignoti neglecti; eventus sine data nihil */
    CREDO_AEQUALIS_I32(_fracta(
        "id: 7\nretry: 10\nignotum: z\n\nevent: q\n\ndata: v\n\n",
        FALSUM,
        "message=v|", c->piscina), ZEPHYRUM);
    /* nomen eventus non transit ad eventum proximum */
    CREDO_AEQUALIS_I32(_fracta("event: a\ndata: 1\n\ndata: 2\n\n",
        FALSUM, "a=1|message=2|", c->piscina), ZEPHYRUM);
}

interior vacuum
_sectio_nota_ordinis (
    CredoContextus* c)
{
    CREDO_AEQUALIS_I32(_fracta("\xEF\xBB\xBF" "data: b\n\n", FALSUM,
        "message=b|", c->piscina), ZEPHYRUM);
    /* BOM solum in initio: in medio textus est */
    CREDO_AEQUALIS_I32(_fracta("data: a\xEF\xBB\xBF\n\n", FALSUM,
        "message=a\xEF\xBB\xBF|", c->piscina), ZEPHYRUM);
}

interior vacuum
_sectio_finis (
    CredoContextus* c)
{
    ChordaAedificator* aed;
        RivusEventuum* r;

    /* eventus imperfectus in fine abicitur */
    CREDO_AEQUALIS_I32(_fracta("data: a\n\ndata: b\n", VERUM,
        "message=a|", c->piscina), ZEPHYRUM);
    CREDO_AEQUALIS_I32(_fracta("data: a\n\ndata: b", VERUM,
        "message=a|", c->piscina), ZEPHYRUM);
    /* numerus dispatchatorum */
    aed  = chorda_aedificator_creare(c->piscina, LXIV);
    r    = rivus_eventuum_creare(c->piscina, _notare, aed);
    rivus_eventuum_adicere(r, (constans i8*)"data: 1\n\ndata: 2\n\n",
        XVIII);
    CREDO_AEQUALIS_I32(rivus_eventuum_numerus(r), II);
    /* NIHIL tutum */
    rivus_eventuum_adicere(NIHIL, (constans i8*)"x", I);
    rivus_eventuum_finire(NIHIL);
    CREDO_AEQUALIS_I32(rivus_eventuum_numerus(NIHIL), ZEPHYRUM);
}

interior vacuum
_sectio_anthropic (
    CredoContextus* c)
{
    CREDO_AEQUALIS_I32(_fracta("event: content_block_delta\n"
        "data: {\"type\":\"content_block_delta\",\"index\":0,"
        "\"delta\":{\"type\":\"text_delta\",\"text\":\"Hi\"}}\n\n"
        "event: ping\ndata: {\"type\": \"ping\"}\n\n", FALSUM,
        "content_block_delta={\"type\":\"content_block_delta\","
        "\"index\":0,"
        "\"delta\":{\"type\":\"text_delta\",\"text\":\"Hi\"}}|"
        "ping={\"type\": \"ping\"}|", c->piscina), ZEPHYRUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "commune", _sectio_commune, NIHIL, NIHIL, NIHIL, NIHIL },
    { "fines linearum", _sectio_fines_linearum, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "spatia", _sectio_spatia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "neglecta", _sectio_neglecta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "nota ordinis", _sectio_nota_ordinis, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "finis", _sectio_finis, NIHIL, NIHIL, NIHIL, NIHIL },
    { "anthropic", _sectio_anthropic, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("rivus_eventuum", SECTIONES);
}
