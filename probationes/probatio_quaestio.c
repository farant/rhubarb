/* probatio_quaestio.c - quaestio una ad vates ($question, Franus
 * 2026-10-13)
 *
 * Provisor FICTUS (sine rete, gratis, sine herbario): petitio exemplar
 * ordinarium aut datum, spatium et latitudinem in systemate, max_tokens
 * ex spatio, quaestionem verbatim fert; textus blocorum iungitur et ad
 * ASCII redigitur; error HTTP causam legibilem dat; quaestio vacua aut
 * spatium nullum nihil mittunt; usus redditur. Rivus (quaestio-rivus
 * T5): planare publicum idem ac responsum, deltae crudae per rivum,
 * tabulae adiutoris (scriptio + lectio ad omnem divisionem, PLURA,
 * PRAVA), adiutor tabulas ordine scribit (T..., U, F; error: E, F).
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "vates.h"
#include "quaestio.h"
#include "chorda_aedificator.h"
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
        _c("Why is the sky blue?", c->piscina), X, LVI, c->piscina);
    CREDO_VERUM(r.successus);
    CREDO_CHORDA_AEQUALIS_LITERIS(r.textus, "Rayleigh scattering.");
    CREDO_AEQUALIS_I32(vates_fictus_petitiones_numerus(v), I);
    corpus = vates_fictus_petitio(v, ZEPHYRUM);
    CREDO_CHORDA_CONTINET(corpus, _c(QUAESTIO_EXEMPLAR, c->piscina));
    CREDO_CHORDA_CONTINET(corpus, _c("Why is the sky blue?",
        c->piscina));
    /* forma paginae (X lineae, LVI columnae) in systemate (Franus
     * 2026-10-14: lineae, non characteres) */
    CREDO_CHORDA_CONTINET(corpus, _c("The page has 10 lines of 56 "
        "characters", c->piscina));
    CREDO_CHORDA_CONTINET(corpus, _c("a blank line between paragraphs "
        "uses a whole line", c->piscina));
    CREDO_CHORDA_CONTINET(corpus, _c("Light Markdown is fine",
        c->piscina));
    CREDO_FALSUM(chorda_continet(corpus, _c("no Markdown",
        c->piscina)));
    /* max_tokens = lineae * latitudo / III + LXIV */
    CREDO_CHORDA_CONTINET(corpus, _c("\"max_tokens\":250", c->piscina));

    /* exemplar datum */
    vates_fictus_textum(v, _c("x", c->piscina), _usus());
    r = quaestio_rogare(v, _c("claude-haiku-5-5", c->piscina),
        _c("q", c->piscina), IV, XL, c->piscina);
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
    r = quaestio_rogare(v, _c("", c->piscina), _c("q", c->piscina), X,
        LVI,
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
    r = quaestio_rogare(v, _c("", c->piscina), _c("q", c->piscina), X,
        LVI,
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
    r = quaestio_rogare(v, _c("", c->piscina), _c("", c->piscina), X,
        LVI,
        c->piscina);
    CREDO_FALSUM(r.successus);
    CREDO_VERUM(r.error.mensura > ZEPHYRUM);
    r = quaestio_rogare(v, _c("", c->piscina), _c("q", c->piscina),
        ZEPHYRUM, LVI, c->piscina);
    CREDO_FALSUM(r.successus);
    r = quaestio_rogare(NIHIL, _c("", c->piscina), _c("q", c->piscina),
        X,
        LVI, c->piscina);
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
    r = quaestio_rogare(v, _c("", c->piscina), _c("q", c->piscina), X,
        LVI,
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
    r = quaestio_rogare(v, _c("", c->piscina), _c("q", c->piscina), X,
        LVI,
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

/* planare publicum = quod quaestio_rogare reddit */
interior vacuum
_sectio_planare (
    CredoContextus* c)
{
    CREDO_CHORDA_AEQUALIS_LITERIS(quaestio_textum_planare(_c(
        "# T\n\nSalve **munde**,\nlinea.\n\n```c\nint x;\n```\n",
        c->piscina), c->piscina),
        "# T\n\nSalve munde, linea.\n\n\tint x;");
}

nomen structura {
    ChordaAedificator* aed;
} Deltae;

interior vacuum
_delta (
    vacuum* datum,
    chorda  delta,
       b32  cogitatio)
{
    Deltae* d = (Deltae*)datum;

    (vacuum)cogitatio;
    chorda_aedificator_appendere_chorda(d->aed, delta);
    chorda_aedificator_appendere_character(d->aed, '|');
}

/* rivus: deltae CRUDAE per rivum, textus finalis planatus */
interior vacuum
_sectio_rivus (
    CredoContextus* c)
{
                Vates* v = _fictus(c->piscina);
    QuaestioResponsum  r;
               Deltae  d;
               chorda  partes[III];

    CREDO_NECESSE_NON_NIHIL(v);
    d.aed             = chorda_aedificator_creare(c->piscina, LXIV);
    partes[ZEPHYRUM]  = _c("# T", c->piscina);
    partes[I]         = _c("itle\n\nSal", c->piscina);
    partes[II]        = _c("ve", c->piscina);
    vates_fictus_rivum(v, partes, III, _usus());
    r = quaestio_rogare_rivum(v, _c("", c->piscina), _c("q",
        c->piscina), X, LVI, _delta, &d, c->piscina);
    CREDO_VERUM(r.successus);
    CREDO_CHORDA_AEQUALIS_LITERIS(chorda_aedificator_finire(d.aed),
        "# T|itle\n\nSal|ve|");
    CREDO_CHORDA_AEQUALIS_LITERIS(r.textus, "# Title\n\nSalve");
    CREDO_AEQUALIS_S64(r.usus.output, XXXIV);
    CREDO_CHORDA_CONTINET(vates_fictus_petitio(v, ZEPHYRUM),
        _c("\"stream\":true", c->piscina));
    CREDO_CHORDA_CONTINET(vates_fictus_petitio(v, ZEPHYRUM),
        _c("The page has 10 lines", c->piscina));
}

interior QuaestioTabula
_tabula (
    QuaestioTabulaGenus  genus,
     constans character* textus,
                    s64  input,
                    s64  output,
                Piscina* p)
{
    QuaestioTabula t;

    memset(&t, ZEPHYRUM, magnitudo(t));
    t.genus   = genus;
    t.textus  = _c(textus, p);
    t.input   = input;
    t.output  = output;
    redde t;
}

/* tabulae: scriptio + lectio; omnis divisio fluxus (praefixum, deinde
 * totum) easdem IV tabulas reddit; praefixum medio in tabula PLURA */
interior vacuum
_sectio_tabulae (
    CredoContextus* c)
{
    QuaestioTabula t[IV];
    QuaestioTabula lecta;
            chorda fluxus;
               i32 k;
               i32 n;
               i32 mala = ZEPHYRUM;

    t[ZEPHYRUM]  = _tabula(QUAESTIO_TABULA_TEXTUS, "ab\ncd", 0, 0,
        c->piscina);
    t[I]         = _tabula(QUAESTIO_TABULA_USUS, "", XII, XXXIV,
        c->piscina);
    t[II]        = _tabula(QUAESTIO_TABULA_ERROR, "x", 0, 0,
        c->piscina);
    t[III]       = _tabula(QUAESTIO_TABULA_FINIS, "end_turn", 0, 0,
        c->piscina);
    fluxus = _c("", c->piscina);
    per (k = ZEPHYRUM; k < IV; k++)
    {
        fluxus = chorda_concatenare(fluxus,
            quaestio_tabulam_scribere(&t[k], c->piscina), c->piscina);
    }
    CREDO_CHORDA_AEQUALIS_LITERIS(fluxus,
        "T 5\nab\ncdU 12 34\nE 1\nxF end_turn\n");
    per (n = ZEPHYRUM; n <= (i32)fluxus.mensura; n++)
    {
        chorda praefixum;
           i32 positio  = ZEPHYRUM;
           i32 lectae   = ZEPHYRUM;
QuaestioLectio l;

        /* praefixum n: tabulae integrae solum, deinde PLURA */
        praefixum.datum    = fluxus.datum;
        praefixum.mensura  = n;
        dum ((l = quaestio_tabulam_legere(praefixum, &positio, &lecta,
                 c->piscina)) == QUAESTIO_LECTIO_TABULA)
        {
            lectae++;
        }
        /* numquam ultra praefixum legitur (planta: tabula T ante
         * octetos omnes accepta - fluxus communis eam celabat) */
        si (l != QUAESTIO_LECTIO_PLURA || positio > n)
        {
            mala++;
        }
        /* deinde totum ab eodem loco: reliquae, omnes IV ordine */
        dum (quaestio_tabulam_legere(fluxus, &positio, &lecta,
                 c->piscina) == QUAESTIO_LECTIO_TABULA)
        {
            si (   lecta.genus  != t[lectae].genus
                || !chorda_aequalis(lecta.textus, t[lectae].textus)
                || lecta.input  != t[lectae].input
                || lecta.output != t[lectae].output)
            {
                mala++;
            }
            lectae++;
        }
        si (lectae != IV || positio != fluxus.mensura)
        {
            mala++;
        }
    }
    CREDO_AEQUALIS_I32(mala, ZEPHYRUM);
    /* pravae */
    {
        i32 positio = ZEPHYRUM;

        CREDO_AEQUALIS_I32((i32)quaestio_tabulam_legere(_c("Q 3\n",
            c->piscina), &positio, &lecta, c->piscina),
            (i32)QUAESTIO_LECTIO_PRAVA);
        positio = ZEPHYRUM;
        CREDO_AEQUALIS_I32((i32)quaestio_tabulam_legere(_c("T zz\n",
            c->piscina), &positio, &lecta, c->piscina),
            (i32)QUAESTIO_LECTIO_PRAVA);
        positio = ZEPHYRUM;
        CREDO_AEQUALIS_I32((i32)quaestio_tabulam_legere(_c("U 1\n",
            c->piscina), &positio, &lecta, c->piscina),
            (i32)QUAESTIO_LECTIO_PRAVA);
    }
}

/* adiutor: tabulae per scriptorem collectae */
interior vacuum
_colligere (
    vacuum* datum,
    chorda  octeti)
{
    chorda_aedificator_appendere_chorda((ChordaAedificator*)datum,
        octeti);
}

interior chorda
_adiutor (
      Vates* v,
    Piscina* p)
{
    ChordaAedificator* aed = chorda_aedificator_creare(p, CXXVIII);

    quaestio_adiutor_currere(v, _c("", p), _c("q", p), X, LVI,
        _colligere, aed, p);
    redde chorda_aedificator_finire(aed);
}

interior vacuum
_sectio_adiutor (
    CredoContextus* c)
{
     Vates* v = _fictus(c->piscina);
    chorda  partes[II];

    CREDO_NECESSE_NON_NIHIL(v);
    partes[ZEPHYRUM]  = _c("a", c->piscina);
    partes[I]         = _c("b", c->piscina);
    vates_fictus_rivum(v, partes, II, _usus());
    CREDO_CHORDA_AEQUALIS_LITERIS(_adiutor(v, c->piscina),
        "T 1\naT 1\nbU 12 34\nF end_turn\n");
    /* DXXIX (conatus I): error, deinde F error */
    vates_fictus_crudum(v, DXXIX,
        "{\"type\":\"error\",\"error\":{\"type\":\"overloaded_error\","
        "\"message\":\"Overloaded\"}}");
    {
        chorda f = _adiutor(v, c->piscina);

        CREDO_CHORDA_CONTINET(f, _c("Overloaded", c->piscina));
        CREDO_VERUM(f.mensura > ZEPHYRUM && f.datum[ZEPHYRUM] == 'E');
        CREDO_VERUM(f.mensura >= VIII && memcmp(f.datum + f.mensura
            - VIII, "F error\n", VIII) == ZEPHYRUM);
    }
    /* rivus fractus post partem: T, E, F error */
    vates_fictus_rivum_fractum(v, partes, I, "overloaded_error", "o");
    CREDO_CHORDA_AEQUALIS_LITERIS(_adiutor(v, c->piscina),
        "T 1\naE 25\nstatus HTTP provisoris: oF error\n");
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "petitio", _sectio_petitio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ascii", _sectio_ascii, NIHIL, NIHIL, NIHIL, NIHIL },
    { "error", _sectio_error, NIHIL, NIHIL, NIHIL, NIHIL },
    { "invalida", _sectio_invalida, NIHIL, NIHIL, NIHIL, NIHIL },
    { "usus", _sectio_usus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "markdown", _sectio_markdown, NIHIL, NIHIL, NIHIL, NIHIL },
    { "planare", _sectio_planare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "rivus", _sectio_rivus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "tabulae", _sectio_tabulae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "adiutor", _sectio_adiutor, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("quaestio", SECTIONES);
}
