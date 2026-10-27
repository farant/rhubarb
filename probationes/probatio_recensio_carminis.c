/* probatio_recensio_carminis.c - Probationes recensionis
 * (include/recensio_carminis.h)
 *
 * 'vesperae' (liber ordinarius) munda: monita 0; carmen 'mala'
 * (quintae parallelae in mensuram 2, pars sine instrumento in mensura
 * 4, et tonsura VERA ad CXX - B2 trianguli cum apice melodiae): monita
 * III. JSON relegitur et campi iudicantur; textus inventi in
 * JSON = linea censurae.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "json.h"
#include "credo.h"
#include "carmen.h"
#include "censura.h"
#include "recensio_carminis.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

#define CARMEN_MALUM                                                   \
    "<carmina><carmen titulus=\"mala\" tempus=\"120\" "                \
    "clavis=\"d maior\">"                                              \
    "<pars titulus=\"melodia\" instrumentum=\"aether\"><frustum "      \
    "mensura=\"1\"><textus!>o5 d4 e8 f#8 g4 f#4 | e4 d4 c#4 <a4 | "    \
    "r1 | r1</textus></frustum></pars>"                                \
    "<pars titulus=\"bassus\" instrumentum=\"triangulum\"><frustum "   \
    "mensura=\"1\"><textus!>o3 d2 o2 b2 | o2 a2 a2 | r1 | r1</textus>" \
    "</frustum></pars>"                                                \
    "<pars titulus=\"nullus\" instrumentum=\"nullum\"><frustum "       \
    "mensura=\"4\"><textus!>o4 e1</textus></frustum></pars>"           \
    "</carmen></carmina>"

interior constans Carmen*
_vesperae (
    CredoContextus* c)
{
           Carmina* liber;
    CarminisVitium  vitium;

    liber = (Carmina*)piscina_allocare(c->piscina, magnitudo(Carmina));
    si (!carmina_ordinaria(c->piscina, liber, &vitium))
    {
        redde NIHIL;
    }
    redde carmina_invenire(liber, chorda_ex_literis("vesperae",
        c->piscina));
}

interior constans Carmen*
_mala (
    CredoContextus* c)
{
           Carmina* liber;
    CarminisVitium  vitium;

    liber = (Carmina*)piscina_allocare(c->piscina, magnitudo(Carmina));
    si (!carmina_legere(chorda_ex_literis(CARMEN_MALUM, c->piscina),
                        c->piscina, liber, &vitium))
    {
        imprimere("  mala: %s (offset %u)\n", vitium.causa,
            vitium.offset);
        redde NIHIL;
    }
    redde &liber->carmina[ZEPHYRUM];
}

interior chorda
_textus (
               CredoContextus* c,
    constans RecensioCarminis* r)
{
    ChordaAedificator* a = chorda_aedificator_creare(c->piscina, M);
               chorda  vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    si (a == NIHIL || !recensio_carminis_textus(r, a))
    {
        redde vacua;
    }
    redde chorda_aedificator_finire(a);
}

/* JSON scriptum et relectum: radix NIHIL si fractum */
interior JsonValor*
_json (
               CredoContextus* c,
    constans RecensioCarminis* r)
{
       JsonValor* v = recensio_carminis_json(r, c->piscina);
    JsonResultus  lectum;

    si (v == NIHIL)
    {
        redde NIHIL;
    }
    lectum = json_legere(json_scribere(v, c->piscina), c->piscina);
    redde lectum.successus ? lectum.radix : NIHIL;
}

interior b32
_chorda_est (
             JsonValor* objectum,
    constans character* clavis,
    constans character* exspectata)
{
    JsonValor* v = objectum != NIHIL
        ? json_objectum_capere(objectum, clavis) : NIHIL;

    si (   v != NIHIL && chorda_aequalis_literis(json_ad_chorda(v),
                                              exspectata))
    {
        redde VERUM;
    }
    imprimere("  '%s' non '%s'\n", clavis, exspectata);
    redde FALSUM;
}


    /* ==================================================
     * MUNDA: vesperae - monita 0, textus, JSON, sine partitura
     * ================================================== */

interior vacuum
_sectio_munda (
    CredoContextus* c)
{
     constans Carmen* k = _vesperae(c);
    RecensioCarminis  r;
              chorda  t;
           JsonValor* j;
           JsonValor* partes;
           JsonValor* regulae;

    CREDO_NECESSE (k != NIHIL);
    CREDO_NECESSE (recensio_carminis_facere(k, NIHIL, NIHIL, c->piscina,
                                            &r));
    CREDO_AEQUALIS_I32 (recensio_carminis_monita(&r), ZEPHYRUM);
    CREDO_AEQUALIS_I32 (r.mensurae, VIII);
    CREDO_AEQUALIS_I32 (r.numerus_inventorum, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (r.numerus_tacitarum, ZEPHYRUM);
    CREDO_VERUM (r.partes[ZEPHYRUM].rms > 0.01f);
    CREDO_VERUM (r.partitura.mensura == ZEPHYRUM);

    t = _textus(c, &r);
    CREDO_CHORDA_INCIPIT (t, chorda_ex_literis("recensio vesperae: d "
        "maior, 4/4, 84, 8 mensurae, 23.636 s; monita 0\n",
        c->piscina));
    CREDO_CHORDA_CONTINET (t, chorda_ex_literis("\n  pars melodia ",
        c->piscina));
    CREDO_CHORDA_CONTINET (t, chorda_ex_literis("  censura (lineae "
        "clavis ambitus dissonantia): 0 inventa\n", c->piscina));
    CREDO_CHORDA_CONTINET (t, chorda_ex_literis("  tacitae: nullae\n",
        c->piscina));

    j = _json(c, &r);
    CREDO_NECESSE (j != NIHIL);
    CREDO_VERUM (_chorda_est(j, "carmen", "vesperae"));
    CREDO_VERUM (_chorda_est(j, "clavis", "d maior"));
    CREDO_VERUM (_chorda_est(j, "metrum", "4/4"));
    CREDO_AEQUALIS_S64 (json_ad_integer(json_objectum_capere(j,
        "tempus")),
        LXXXIV);
    CREDO_AEQUALIS_S64 (json_ad_integer(json_objectum_capere(j,
        "mensurae")), VIII);
    CREDO_AEQUALIS_S64 (json_ad_integer(json_objectum_capere(j,
        "monita")),
        ZEPHYRUM);
    partes = json_objectum_capere(j, "partes");
    CREDO_NECESSE (partes != NIHIL
        && json_tabulatum_numerus(partes) == II);
    CREDO_VERUM (_chorda_est(json_tabulatum_obtinere(partes, ZEPHYRUM),
        "titulus", "melodia"));
    CREDO_VERUM (_chorda_est(json_tabulatum_obtinere(partes, ZEPHYRUM),
        "infima", "A4"));
    CREDO_VERUM (_chorda_est(json_tabulatum_obtinere(partes, ZEPHYRUM),
        "summa", "B5"));
    CREDO_AEQUALIS_S64 (json_ad_integer(json_objectum_capere(
        json_tabulatum_obtinere(partes, ZEPHYRUM), "notae")), XXIX);
    /* rms ad millesimas exacte (duplex proximum) */
    CREDO_VERUM (json_ad_fluitans(json_objectum_capere(
        json_tabulatum_obtinere(partes, ZEPHYRUM), "rms"))
        == floor((f64)r.partes[ZEPHYRUM].rms * 1000.0 + 0.5) / 1000.0);
    regulae = json_objectum_capere(json_objectum_capere(j, "censura"),
        "regulae");
    CREDO_NECESSE (regulae != NIHIL
                   && json_tabulatum_numerus(regulae) == IV);
    CREDO_CHORDA_AEQUALIS_LITERIS (json_ad_chorda(
        json_tabulatum_obtinere(regulae, ZEPHYRUM)), "lineae");
    CREDO_AEQUALIS_I32 (json_tabulatum_numerus(json_objectum_capere(
        json_objectum_capere(j, "sonus"), "mensurae")), VIII);
    CREDO_FALSUM (json_objectum_habet(j, "partitura"));
    /* numeri rotundati breves scribuntur (non -12.200000000000001) */
    CREDO_FALSUM (chorda_continet(json_scribere(
        recensio_carminis_json(&r, c->piscina), c->piscina),
        chorda_ex_literis("0000000", c->piscina)));
}


    /* ==================================================
     * MALA: inventum, tacita; monita; JSON inventi = linea censurae
     * ================================================== */

interior vacuum
_sectio_mala (
    CredoContextus* c)
{
      constans Carmen* k = _mala(c);
        RecensioForma  f;
     RecensioCarminis  r;
               chorda  t;
            JsonValor* j;
            JsonValor* inventum;
            JsonValor* notae;
            JsonValor* mensura;
    ChordaAedificator* linea;
               chorda  exspectata;

    CREDO_NECESSE (k != NIHIL);
    recensio_carminis_ordinaria(&f);
    f.censura.activae[CENSURA_PARALLELAE] = VERUM;
    CREDO_NECESSE (recensio_carminis_facere(k, &f, NIHIL, c->piscina,
        &r));
    CREDO_AEQUALIS_I32 (r.numerus_inventorum, I);
    CREDO_NECESSE (r.numerus_tacitarum == I);
    CREDO_AEQUALIS_I32 (r.tacitae[ZEPHYRUM], IV);
    /* inventum + tacita + tonsura (ad CXX apices coincidunt: inventum
     * verum, non consilium probationis - 'vesperae' eisdem partibus ad
     * LXXXIV apicem -1.8 dBFS habet) */
    CREDO_VERUM (r.sonus.tonsa > ZEPHYRUM);
    CREDO_AEQUALIS_I32 (recensio_carminis_monita(&r), III);

    linea = chorda_aedificator_creare(c->piscina, CXXVIII);
    CREDO_NECESSE (linea != NIHIL && censura_inventum_scribere(k,
        &r.inventa[ZEPHYRUM], linea));
    exspectata = chorda_aedificator_finire(linea);
    exspectata.mensura--;   /* sine '\n' */

    t = _textus(c, &r);
    CREDO_CHORDA_CONTINET (t, chorda_ex_literis("    mensura 2 "
        "melodia/bassus: quintae parallelae F#5/B2 -> E5/A2 "
        "(frustum 0, offset 22)\n", c->piscina));
    CREDO_CHORDA_CONTINET (t, chorda_ex_literis("  tacitae: 4\n",
        c->piscina));
    CREDO_CHORDA_INCIPIT (t,
        chorda_ex_literis("recensio mala: d maior, "
        "4/4, 120, 4 mensurae, 8.000 s; monita 3\n", c->piscina));

    j = _json(c, &r);
    CREDO_NECESSE (j != NIHIL);
    inventum = json_tabulatum_obtinere(json_objectum_capere(
        json_objectum_capere(j, "censura"), "inventa"), ZEPHYRUM);
    CREDO_NECESSE (inventum != NIHIL);
    CREDO_VERUM (_chorda_est(inventum, "regula", "parallelae"));
    CREDO_VERUM (_chorda_est(inventum, "pars", "melodia"));
    CREDO_VERUM (_chorda_est(inventum, "pars_altera", "bassus"));
    CREDO_AEQUALIS_S64 (json_ad_integer(json_objectum_capere(inventum,
        "mensura")), II);
    CREDO_AEQUALIS_S64 (json_ad_integer(json_objectum_capere(inventum,
        "offset")), XXII);
    notae = json_objectum_capere(inventum, "notae");
    CREDO_NECESSE (notae != NIHIL
        && json_tabulatum_numerus(notae) == II);
    CREDO_CHORDA_AEQUALIS_LITERIS (json_ad_chorda(
        json_tabulatum_obtinere(notae, ZEPHYRUM)), "E5");
    CREDO_CHORDA_AEQUALIS_LITERIS (json_ad_chorda(
        json_tabulatum_obtinere(json_objectum_capere(inventum,
        "priores"),
        I)), "B2");
    CREDO_CHORDA_AEQUALIS (json_ad_chorda(json_objectum_capere(inventum,
        "textus")), exspectata);
    mensura = json_tabulatum_obtinere(json_objectum_capere(
        json_objectum_capere(j, "sonus"), "mensurae"), III);
    CREDO_NECESSE (mensura != NIHIL);
    CREDO_VERUM (json_ad_boolean(json_objectum_capere(mensura,
        "tacita")));
    CREDO_VERUM (json_ad_fluitans(json_objectum_capere(mensura, "rms"))
                 == 0.0);
    /* mensura 1: rms suum (non totius), ad millesimas */
    mensura = json_tabulatum_obtinere(json_objectum_capere(
        json_objectum_capere(j, "sonus"), "mensurae"), ZEPHYRUM);
    CREDO_NECESSE (mensura != NIHIL);
    CREDO_VERUM (json_ad_fluitans(json_objectum_capere(mensura, "rms"))
        == floor((f64)r.sonus.mensurae[ZEPHYRUM] * 1000.0 + 0.5)
            / 1000.0);
    CREDO_AEQUALIS_S64 (json_ad_integer(json_objectum_capere(j,
        "monita")),
        III);
    CREDO_VERUM (json_ad_integer(json_objectum_capere(
        json_objectum_capere(j, "sonus"), "tonsa")) > ZEPHYRUM);
}


    /* ==================================================
     * PARTITURA: rogata includitur (cum harmonia), in JSON quoque
     * ================================================== */

interior vacuum
_sectio_partitura (
    CredoContextus* c)
{
     constans Carmen* k = _vesperae(c);
       RecensioForma  f;
    RecensioCarminis  r;
           JsonValor* j;

    CREDO_NECESSE (k != NIHIL);
    recensio_carminis_ordinaria(&f);
    f.partitura  = VERUM;
    f.harmonia   = VERUM;
    CREDO_NECESSE (recensio_carminis_facere(k, &f, NIHIL, c->piscina,
        &r));
    CREDO_CHORDA_CONTINET (r.partitura,
        chorda_ex_literis("\nmensura 8  "
        "tempus 66\n", c->piscina));
    CREDO_CHORDA_CONTINET (r.partitura,
        chorda_ex_literis("\n  harmonia  ",
        c->piscina));
    CREDO_CHORDA_CONTINET (_textus(c, &r), chorda_ex_literis("\ncarmen "
        "vesperae: d maior, 4/4, 84\n", c->piscina));
    j = _json(c, &r);
    CREDO_NECESSE (j != NIHIL);
    CREDO_CHORDA_AEQUALIS (json_ad_chorda(json_objectum_capere(j,
        "partitura")), r.partitura);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "munda", _sectio_munda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "mala", _sectio_mala, NIHIL, NIHIL, NIHIL, NIHIL },
    { "partitura", _sectio_partitura, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("recensio_carminis", SECTIONES);
}
