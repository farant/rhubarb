/* probatio_quaestio.c - quaestio una ad vates ($question, Franus
 * 2026-10-13)
 *
 * Provisor FICTUS (sine rete, gratis, sine herbario): petitio exemplar
 * ordinarium aut datum, spatium et latitudinem in systemate, max_tokens
 * ex spatio, quaestionem verbatim fert; textus blocorum iungitur et ad
 * ASCII redigitur; error HTTP causam legibilem dat; quaestio vacua aut
 * spatium nullum nihil mittunt; usus redditur.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "vates.h"
#include "quaestio.h"
#include <string.h>

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
    o.conatus_maximi    = I;
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

interior vacuum
_sectio_petitio (
    CredoContextus* c)
{
                Vates* v = _fictus(c->piscina);
    QuaestioResponsum  r;
               chorda  corpus;

    CREDO_NECESSE_NON_NIHIL(v);
    vates_fictus_textum(v, _c("Rayleigh scattering.", c->piscina),
        _usus());
    r = quaestio_rogare(v, _c("", c->piscina),
        _c("Why is the sky blue?", c->piscina), D, LX, c->piscina);
    CREDO_VERUM(r.successus);
    CREDO_CHORDA_AEQUALIS_LITERIS(r.textus, "Rayleigh scattering.");
    CREDO_AEQUALIS_I32(vates_fictus_petitiones_numerus(v), I);
    corpus = vates_fictus_petitio(v, ZEPHYRUM);
    CREDO_CHORDA_CONTINET(corpus, _c(QUAESTIO_EXEMPLAR, c->piscina));
    CREDO_CHORDA_CONTINET(corpus, _c("Why is the sky blue?",
        c->piscina));
    /* spatium (D) et latitudo (LX) in systemate */
    CREDO_CHORDA_CONTINET(corpus, _c("at most 500 characters",
        c->piscina));
    CREDO_CHORDA_CONTINET(corpus, _c("60 columns wide", c->piscina));
    /* max_tokens = characteres / III + LXIV */
    CREDO_CHORDA_CONTINET(corpus, _c("\"max_tokens\":230", c->piscina));

    /* exemplar datum */
    vates_fictus_textum(v, _c("x", c->piscina), _usus());
    r = quaestio_rogare(v, _c("claude-haiku-5-5", c->piscina),
        _c("q", c->piscina), C, XL, c->piscina);
    CREDO_VERUM(r.successus);
    CREDO_CHORDA_CONTINET(vates_fictus_petitio(v, I),
        _c("claude-haiku-5-5", c->piscina));
}

/* Unicode commune -> ASCII; ceteri characteres multi-octeti -> '?';
 * imperia (praeter '\n') -> spatium, '\r' tollitur */
interior vacuum
_sectio_ascii (
    CredoContextus* c)
{
                Vates* v = _fictus(c->piscina);
    QuaestioResponsum  r;

    CREDO_NECESSE_NON_NIHIL(v);
    vates_fictus_textum(v, _c(
        "\xE2\x80\x9C" "quoted" "\xE2\x80\x9D" " it" "\xE2\x80\x99" "s "
        "\xE2\x80\x94" " caf" "\xC3\xA9" "\xE2\x80\xA6" "\r\nnext\tline",
        c->piscina), _usus());
    r = quaestio_rogare(v, _c("", c->piscina), _c("q", c->piscina), D,
        LX,
        c->piscina);
    CREDO_VERUM(r.successus);
    CREDO_CHORDA_AEQUALIS_LITERIS(r.textus,
        "\"quoted\" it's - caf?... next line");
}

interior vacuum
_sectio_error (
    CredoContextus* c)
{
                Vates* v = _fictus(c->piscina);
    QuaestioResponsum  r;

    CREDO_NECESSE_NON_NIHIL(v);
    vates_fictus_crudum(v, DXXIX,
        "{\"type\":\"error\",\"error\":{\"type\":\"overloaded_error\","
        "\"message\":\"Overloaded\"}}");
    r = quaestio_rogare(v, _c("", c->piscina), _c("q", c->piscina), D,
        LX,
        c->piscina);
    CREDO_FALSUM(r.successus);
    CREDO_CHORDA_CONTINET(r.error, _c("Overloaded", c->piscina));
}

interior vacuum
_sectio_invalida (
    CredoContextus* c)
{
                Vates* v = _fictus(c->piscina);
    QuaestioResponsum  r;

    CREDO_NECESSE_NON_NIHIL(v);
    r = quaestio_rogare(v, _c("", c->piscina), _c("", c->piscina), D,
        LX,
        c->piscina);
    CREDO_FALSUM(r.successus);
    CREDO_VERUM(r.error.mensura > ZEPHYRUM);
    r = quaestio_rogare(v, _c("", c->piscina), _c("q", c->piscina),
        ZEPHYRUM, LX, c->piscina);
    CREDO_FALSUM(r.successus);
    r = quaestio_rogare(NIHIL, _c("", c->piscina), _c("q", c->piscina),
        D,
        LX, c->piscina);
    CREDO_FALSUM(r.successus);
    CREDO_AEQUALIS_I32(vates_fictus_petitiones_numerus(v), ZEPHYRUM);
}

interior vacuum
_sectio_usus (
    CredoContextus* c)
{
                Vates* v = _fictus(c->piscina);
    QuaestioResponsum  r;

    CREDO_NECESSE_NON_NIHIL(v);
    vates_fictus_textum(v, _c("ok", c->piscina), _usus());
    r = quaestio_rogare(v, _c("", c->piscina), _c("q", c->piscina), D,
        LX,
        c->piscina);
    CREDO_VERUM(r.successus);
    CREDO_AEQUALIS_S64(r.usus.input, XII);
    CREDO_AEQUALIS_S64(r.usus.output, XXXIV);
}

/* markdown -> lineae logicae: paragraphus una linea (fracturae
 * spatia, emphasis et nexus sine marcis, codex inlineus cum gravibus),
 * capitulum "# ", elementa cum praefixis (nidificata indentata),
 * saeptum verbatim cum '\t' ducente et indentatione propria (tabula ->
 * IV spatia), citatio "> ", lineae vacuae ubi fons eas habuit */
interior vacuum
_sectio_markdown (
    CredoContextus* c)
{
                Vates* v = _fictus(c->piscina);
    QuaestioResponsum  r;

    CREDO_NECESSE_NON_NIHIL(v);
    vates_fictus_textum(v, _c(
        "# TestFlight API\n\nApple offers the **App Store Connect API**,\n"
        "which covers `builds` and [testers](https://x.y).\n\n"
        "- first item\n  continues here\n- second\n  1. nested one\n\n"
        "```c\nint main(void)\n{\n\treturn 0;\n}\n```\n\n> quoted\n",
        c->piscina), _usus());
    r = quaestio_rogare(v, _c("", c->piscina), _c("q", c->piscina), D,
        LX,
        c->piscina);
    CREDO_VERUM(r.successus);
    CREDO_CHORDA_AEQUALIS_LITERIS(r.textus,
        "# TestFlight API\n"
        "\n"
        "Apple offers the App Store Connect API, which covers `builds` and testers.\n"
        "\n"
        "- first item continues here\n"
        "- second\n"
        "  1. nested one\n"
        "\n"
        "\tint main(void)\n"
        "\t{\n"
        "\t    return 0;\n"
        "\t}\n"
        "\n"
        "> quoted");
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "petitio", _sectio_petitio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ascii", _sectio_ascii, NIHIL, NIHIL, NIHIL, NIHIL },
    { "error", _sectio_error, NIHIL, NIHIL, NIHIL, NIHIL },
    { "invalida", _sectio_invalida, NIHIL, NIHIL, NIHIL, NIHIL },
    { "usus", _sectio_usus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "markdown", _sectio_markdown, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("quaestio", SECTIONES);
}
