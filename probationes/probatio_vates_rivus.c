/* probatio_vates_rivus.c - vates per rivum (quaestio-rivus-spec II.4,
 * T4)
 *
 * Provisor FICTUS (SSE verum per vecturam rivi, frustis V octetorum):
 * deltae ordine per rivum, responsum congregatum IDEM ac vates_mittere
 * pro eodem textu, "stream":true in corpore solum per rivum; eventus
 * 'error' post deltam non iteratur, ante deltam (et status DXXIX)
 * iteratur; rivus sine message_stop = VATES_ERROR_RETE; instrumentum
 * per input_json_delta; cogitatio per thinking_delta + signature_delta
 * (blocus OPACUS cum textu et signatura).
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "credo.h"
#include "json.h"
#include "vates.h"
#include <string.h>

nomen structura {
    ChordaAedificator* textus;
    ChordaAedificator* cogitatio;
} Deltae;

interior vacuum
_delta (
    vacuum* datum,
    chorda  delta,
       b32  cogitatio)
{
               Deltae* d    = (Deltae*)datum;
    ChordaAedificator* aed  = cogitatio ? d->cogitatio : d->textus;

    chorda_aedificator_appendere_chorda(aed, delta);
    chorda_aedificator_appendere_character(aed, '|');
}

interior vacuum
_deltae_initiare (
     Deltae* d,
    Piscina* p)
{
    d->textus     = chorda_aedificator_creare(p, LXIV);
    d->cogitatio  = chorda_aedificator_creare(p, LXIV);
}

interior chorda
_c (
    constans character* s,
               Piscina* p)
{
    redde chorda_ex_literis(s, p);
}

interior Vates*
_fictus (
    Piscina* p)
{
    VatesOptiones o = vates_optiones_ordinariae();

    o.sine_herbario     = VERUM;
    o.rationarium_via   = NIHIL;
    o.conatus_maximi    = III;
    o.mora_iterandi_ms  = I;
    redde vates_fictus_aperire(p, &o);
}

interior VatesUsus
_usus (vacuum)
{
    VatesUsus u;

    memset(&u, ZEPHYRUM, magnitudo(u));
    u.input    = XII;
    u.output   = XXXIV;
    u.pretium  = -I;
    redde u;
}

interior VatesPetitio*
_petitio (
    Piscina* p)
{
    VatesPetitio* pe  = vates_petitio_creare(p, _c("fictus", p));
    VatesNuntius* n   = vates_nuntium_addere(pe, VATES_USOR);

    (vacuum)vates_textum_addere(n, _c("salve", p));
    redde pe;
}

/* textus blocorum TEXTUS iunctus */
interior chorda
_textus (
    constans VatesResponsum* r,
                    Piscina* p)
{
    chorda t = _c("", p);
       i32 k;

    per (k = ZEPHYRUM; r->bloci && k < xar_numerus(r->bloci); k++)
    {
        VatesBlocus* b = (VatesBlocus*)xar_obtinere(r->bloci, k);

        si (b->genus == VATES_TEXTUS)
        {
            t = chorda_concatenare(t, b->textus, p);
        }
    }
    redde t;
}

interior vacuum
_sectio_deltae (
    CredoContextus* c)
{
             Vates* v = _fictus(c->piscina);
    VatesResponsum* r;
            Deltae  d;
            chorda  partes[III];

    CREDO_NECESSE_NON_NIHIL(v);
    _deltae_initiare(&d, c->piscina);
    partes[ZEPHYRUM]  = _c("Sal", c->piscina);
    partes[I]         = _c("ve mu", c->piscina);
    partes[II]        = _c("nde.", c->piscina);
    vates_fictus_rivum(v, partes, III, _usus());
    r = vates_mittere_rivum(v, _petitio(c->piscina),
        _c("probatio", c->piscina), _delta, &d, c->piscina);
    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_CHORDA_AEQUALIS_LITERIS(chorda_aedificator_finire(d.textus),
        "Sal|ve mu|nde.|");
    CREDO_VERUM(r->successus);
    CREDO_AEQUALIS_I32(xar_numerus(r->bloci), I);
    CREDO_CHORDA_AEQUALIS_LITERIS(_textus(r, c->piscina),
        "Salve munde.");
    CREDO_AEQUALIS_I32((i32)r->causa_finis, (i32)VATES_FINIS);
    CREDO_AEQUALIS_S64(r->usus.input, XII);
    CREDO_AEQUALIS_S64(r->usus.output, XXXIV);
    CREDO_CHORDA_CONTINET(vates_fictus_petitio(v, ZEPHYRUM),
        _c("\"stream\":true", c->piscina));
}

/* idem textus per vates_mittere: idem responsum, corpus sine stream */
interior vacuum
_sectio_aequale (
    CredoContextus* c)
{
              Vates* v = _fictus(c->piscina);
              Vates* w = _fictus(c->piscina);
     VatesResponsum* r;
     VatesResponsum* s;
             Deltae  d;
             chorda  partes[II];

    CREDO_NECESSE_NON_NIHIL(v);
    CREDO_NECESSE_NON_NIHIL(w);
    _deltae_initiare(&d, c->piscina);
    partes[ZEPHYRUM]  = _c("Salve ", c->piscina);
    partes[I]         = _c("munde.", c->piscina);
    vates_fictus_rivum(v, partes, II, _usus());
    vates_fictus_textum(w, _c("Salve munde.", c->piscina), _usus());
    r = vates_mittere_rivum(v, _petitio(c->piscina),
        _c("probatio", c->piscina), _delta, &d, c->piscina);
    s = vates_mittere(w, _petitio(c->piscina), _c("probatio",
        c->piscina), c->piscina);
    CREDO_VERUM(r->successus && s->successus);
    CREDO_CHORDA_AEQUALIS(_textus(r, c->piscina), _textus(s,
        c->piscina));
    CREDO_AEQUALIS_I32(xar_numerus(r->bloci), xar_numerus(s->bloci));
    CREDO_AEQUALIS_I32((i32)r->causa_finis, (i32)s->causa_finis);
    CREDO_AEQUALIS_S64(r->usus.input, s->usus.input);
    CREDO_AEQUALIS_S64(r->usus.output, s->usus.output);
    CREDO_CHORDA_AEQUALIS(r->id, s->id);
    CREDO_FALSUM(chorda_continet(vates_fictus_petitio(w, ZEPHYRUM),
        _c("\"stream\"", c->piscina)));
    /* corpora eadem praeter "stream":true */
    CREDO_AEQUALIS_I32(vates_fictus_petitio(v, ZEPHYRUM).mensura,
        vates_fictus_petitio(w, ZEPHYRUM).mensura
        + (i32)strlen(",\"stream\":true"));
}

/* eventus 'error' post deltam: nulla iteratio */
interior vacuum
_sectio_error_eventus (
    CredoContextus* c)
{
              Vates* v = _fictus(c->piscina);
     VatesResponsum* r;
             Deltae  d;
             chorda  partes[I];

    CREDO_NECESSE_NON_NIHIL(v);
    _deltae_initiare(&d, c->piscina);
    partes[ZEPHYRUM] = _c("a", c->piscina);
    vates_fictus_rivum_fractum(v, partes, I, "overloaded_error",
        "Overloaded");
    vates_fictus_rivum(v, partes, I, _usus());   /* numquam petitum */
    r = vates_mittere_rivum(v, _petitio(c->piscina),
        _c("probatio", c->piscina), _delta, &d, c->piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(chorda_aedificator_finire(d.textus),
        "a|");
    CREDO_FALSUM(r->successus);
    CREDO_AEQUALIS_I32((i32)r->error, (i32)VATES_ERROR_STATUS);
    CREDO_CHORDA_AEQUALIS_LITERIS(r->error_genus, "overloaded_error");
    CREDO_CHORDA_AEQUALIS_LITERIS(r->error_nuntius, "Overloaded");
    CREDO_AEQUALIS_I32(vates_fictus_petitiones_numerus(v), I);
}

/* status DXXIX ante rivum: iteratur, delta semel */
interior vacuum
_sectio_iteratio_ante_deltam (
    CredoContextus* c)
{
              Vates* v = _fictus(c->piscina);
     VatesResponsum* r;
             Deltae  d;
             chorda  partes[I];

    CREDO_NECESSE_NON_NIHIL(v);
    _deltae_initiare(&d, c->piscina);
    partes[ZEPHYRUM] = _c("ok", c->piscina);
    vates_fictus_crudum(v, DXXIX,
        "{\"type\":\"error\",\"error\":{\"type\":\"overloaded_error\","
        "\"message\":\"Overloaded\"}}");
    vates_fictus_rivum(v, partes, I, _usus());
    r = vates_mittere_rivum(v, _petitio(c->piscina),
        _c("probatio", c->piscina), _delta, &d, c->piscina);
    CREDO_VERUM(r->successus);
    CREDO_AEQUALIS_I32(vates_fictus_petitiones_numerus(v), II);
    CREDO_CHORDA_AEQUALIS_LITERIS(chorda_aedificator_finire(d.textus),
        "ok|");
    CREDO_AEQUALIS_I32(r->conatus, II);
}

/* eventus 'error' ante deltam (overloaded): iteratur */
interior vacuum
_sectio_fractum_ante_deltam (
    CredoContextus* c)
{
              Vates* v = _fictus(c->piscina);
     VatesResponsum* r;
             Deltae  d;
             chorda  partes[I];

    CREDO_NECESSE_NON_NIHIL(v);
    _deltae_initiare(&d, c->piscina);
    partes[ZEPHYRUM] = _c("ok", c->piscina);
    vates_fictus_rivum_fractum(v, NIHIL, ZEPHYRUM, "overloaded_error",
        "o");
    vates_fictus_rivum(v, partes, I, _usus());
    r = vates_mittere_rivum(v, _petitio(c->piscina),
        _c("probatio", c->piscina), _delta, &d, c->piscina);
    CREDO_VERUM(r->successus);
    CREDO_AEQUALIS_I32(vates_fictus_petitiones_numerus(v), II);
    CREDO_CHORDA_AEQUALIS_LITERIS(chorda_aedificator_finire(d.textus),
        "ok|");
}

#define INITIUM_NUNTII \
    "event: message_start\ndata: {\"type\":\"message_start\"," \
    "\"message\":{\"id\":\"msg_1\",\"type\":\"message\",\"role\":" \
    "\"assistant\",\"model\":\"m\",\"content\":[]," \
    "\"stop_reason\":null," \
    "\"stop_sequence\":null,\"usage\":{\"input_tokens\":5," \
    "\"output_tokens\":1}}}\n\n"

/* rivus sine message_stop */
interior vacuum
_sectio_truncatus (
    CredoContextus* c)
{
     VatesResponsum* r;
             Deltae  d;

    _deltae_initiare(&d, c->piscina);
    r = vates_anthropic_rivum_legere(_c(INITIUM_NUNTII
        "event: content_block_start\ndata: {\"type\":"
        "\"content_block_start\",\"index\":0,\"content_block\":"
        "{\"type\":\"text\",\"text\":\"\"}}\n\n"
        "event: content_block_delta\ndata: {\"type\":"
        "\"content_block_delta\",\"index\":0,\"delta\":{\"type\":"
        "\"text_delta\",\"text\":\"semi\"}}\n\n", c->piscina), _delta,
        &d, c->piscina);
    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_FALSUM(r->successus);
    CREDO_AEQUALIS_I32((i32)r->error, (i32)VATES_ERROR_RETE);
    CREDO_CHORDA_CONTINET(r->error_nuntius, _c("truncatus",
        c->piscina));
    CREDO_CHORDA_AEQUALIS_LITERIS(chorda_aedificator_finire(d.textus),
        "semi|");
}

/* tool_use per input_json_delta in duabus partibus */
interior vacuum
_sectio_instrumentum (
    CredoContextus* c)
{
    VatesResponsum* r;
       VatesBlocus* b;

    r = vates_anthropic_rivum_legere(_c(INITIUM_NUNTII
        "event: content_block_start\ndata: {\"type\":"
        "\"content_block_start\",\"index\":0,\"content_block\":"
        "{\"type\":\"tool_use\",\"id\":\"t1\",\"name\":\"quaere\","
        "\"input\":{}}}\n\n"
        "event: content_block_delta\ndata: {\"type\":"
        "\"content_block_delta\",\"index\":0,\"delta\":{\"type\":"
        "\"input_json_delta\",\"partial_json\":\"{\\\"q\\\":\"}}\n\n"
        "event: content_block_delta\ndata: {\"type\":"
        "\"content_block_delta\",\"index\":0,\"delta\":{\"type\":"
        "\"input_json_delta\",\"partial_json\":\"\\\"x\\\"}\"}}\n\n"
        "event: content_block_stop\ndata: {\"type\":"
        "\"content_block_stop\",\"index\":0}\n\n"
        "event: message_delta\ndata: {\"type\":\"message_delta\","
        "\"delta\":{\"stop_reason\":\"tool_use\",\"stop_sequence\":"
        "null},\"usage\":{\"output_tokens\":9}}\n\n"
        "event: message_stop\ndata: {\"type\":\"message_stop\"}\n\n",
        c->piscina), NIHIL, NIHIL, c->piscina);
    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_VERUM(r->successus);
    CREDO_AEQUALIS_I32((i32)r->causa_finis,
        (i32)VATES_FINIS_INSTRUMENTUM);
    CREDO_NECESSE(xar_numerus(r->bloci) == I);
    b = (VatesBlocus*)xar_obtinere(r->bloci, ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)b->genus, (i32)VATES_INSTRUMENTUM_PETITUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(b->titulus, "quaere");
    CREDO_CHORDA_AEQUALIS_LITERIS(json_capere_chorda(b->input, "q",
        _c("", c->piscina)), "x");
    CREDO_AEQUALIS_S64(r->usus.output, IX);
    CREDO_AEQUALIS_S64(r->usus.input, V);
}

/* cogitatio: thinking_delta per rivum (VERUM), signatura servata,
 * blocus OPACUS cum textu congregato */
interior vacuum
_sectio_cogitatio (
    CredoContextus* c)
{
     VatesResponsum* r;
        VatesBlocus* b;
             Deltae  d;

    _deltae_initiare(&d, c->piscina);
    r = vates_anthropic_rivum_legere(_c(INITIUM_NUNTII
        "event: content_block_start\ndata: {\"type\":"
        "\"content_block_start\",\"index\":0,\"content_block\":"
        "{\"type\":\"thinking\",\"thinking\":\"\"}}\n\n"
        "event: content_block_delta\ndata: {\"type\":"
        "\"content_block_delta\",\"index\":0,\"delta\":{\"type\":"
        "\"thinking_delta\",\"thinking\":\"puto\"}}\n\n"
        "event: content_block_delta\ndata: {\"type\":"
        "\"content_block_delta\",\"index\":0,\"delta\":{\"type\":"
        "\"signature_delta\",\"signature\":\"sig1\"}}\n\n"
        "event: content_block_stop\ndata: {\"type\":"
        "\"content_block_stop\",\"index\":0}\n\n"
        "event: content_block_start\ndata: {\"type\":"
        "\"content_block_start\",\"index\":1,\"content_block\":"
        "{\"type\":\"text\",\"text\":\"\"}}\n\n"
        "event: content_block_delta\ndata: {\"type\":"
        "\"content_block_delta\",\"index\":1,\"delta\":{\"type\":"
        "\"text_delta\",\"text\":\"Hic\"}}\n\n"
        "event: message_delta\ndata: {\"type\":\"message_delta\","
        "\"delta\":{\"stop_reason\":\"end_turn\"},\"usage\":"
        "{\"output_tokens\":3}}\n\n"
        "event: message_stop\ndata: {\"type\":\"message_stop\"}\n\n",
        c->piscina), _delta, &d, c->piscina);
    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_VERUM(r->successus);
    CREDO_CHORDA_AEQUALIS_LITERIS(
        chorda_aedificator_finire(d.cogitatio), "puto|");
    CREDO_CHORDA_AEQUALIS_LITERIS(chorda_aedificator_finire(d.textus),
        "Hic|");
    CREDO_NECESSE(xar_numerus(r->bloci) == II);
    b = (VatesBlocus*)xar_obtinere(r->bloci, ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)b->genus, (i32)VATES_OPACUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(json_capere_chorda(b->crudum,
        "thinking", _c("", c->piscina)), "puto");
    CREDO_CHORDA_AEQUALIS_LITERIS(json_capere_chorda(b->crudum,
        "signature", _c("", c->piscina)), "sig1");
    CREDO_CHORDA_AEQUALIS_LITERIS(_textus(r, c->piscina), "Hic");
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "deltae", _sectio_deltae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "aequale", _sectio_aequale, NIHIL, NIHIL, NIHIL, NIHIL },
    { "error eventus", _sectio_error_eventus, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "iteratio ante deltam", _sectio_iteratio_ante_deltam, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "fractum ante deltam iteratur", _sectio_fractum_ante_deltam,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "truncatus", _sectio_truncatus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "instrumentum", _sectio_instrumentum, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "cogitatio", _sectio_cogitatio, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("vates_rivus", SECTIONES);
}
