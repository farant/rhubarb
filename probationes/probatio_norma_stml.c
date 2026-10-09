/* probatio_norma_stml.c - facies STML normae: canon infixus (A2),
 * lector (A3), scriptor et oracula I, II, V (A4) - norma-plan-3 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "json.h"
#include "filum.h"
#include "internamentum.h"
#include "stml.h"
#include "canon.h"
#include "norma.h"
#include "norma_canon.h"
#include "norma_stml.h"
#include "norma_gignere.h"
#include "chorda_aedificator.h"
#include "norma_ad_c.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

/* fons GENERATUS a bin/norma c (oraculum III, norma-plan-3 A5) */
#include "fixa/norma/normae_fixae.h"
#include "fixa/norma/normae_fixae.c"

/* canon infixus super documentum: numerus vitiorum; *genera = bit per
 * CanonVitiumGenus visum (canon unum defectum saepe bis nominat -
 * '<nemo/>' = LIBERUM_ILLICITUM + ELEMENTUM_IGNOTUM) */
interior i32
_canon_vitia (
    constans character* documentum,
                   i32* genera,
               Piscina* p)
{
    InternamentumChorda* in = internamentum_creare(p);
                 chorda  causa;
                  Canon* canon = canon_legere(norma_canon_textus(p), p,
                      in,
                                    &causa);
           StmlResultus  r = stml_legere_ex_literis(documentum, p, in);
                    Xar* v;
                    i32  i;

    CREDO_NON_NIHIL(canon);
    CREDO_VERUM(r.successus);
    si (!canon || !r.successus)
    {
        redde M;
    }
    v = canon_iudicare(canon, r.radix, p);
    si (genera)
    {
        *genera = 0;
        per (i = 0; i < xar_numerus(v); i++)
        {
            *genera |= (i32)I << (i32)((CanonVitium*)xar_obtinere(v,
                i))->genus;
        }
    }
    redde xar_numerus(v);
}

#define NORMA_A  "<normae versio=\"1\">\n<norma titulus=\"a\">\n"
#define NORMA_Z  "\n</norma>\n</normae>\n"

/* genus g inter vitia documenti */
interior b32
_canon_habet (
     constans character* documentum,
       CanonVitiumGenus  g,
                Piscina* p)
{
    i32 genera = 0;

    _canon_vitia(documentum, &genera, p);
    redde (genera & ((i32)I << (i32)g)) != 0;
}

interior vacuum
probatio_canon_infixus(Piscina* p)
{
    chorda discus = filum_legere_totum("norma.canon", p);

    imprimere("\n--- Probans canonem infixum ---\n");
    /* infixus == plagula: generatio non rancida */
    CREDO_CHORDA_AEQUALIS(norma_canon_textus(p), discus);
    CREDO_AEQUALIS_I32(_canon_vitia(
        "<normae versio=\"1\">\n"
        "<norma titulus=\"a\"><objectum modus=\"notandum\">"
        "<campus titulus=\"x\"><ad norma=\"b\"/></campus>"
        "</objectum></norma>\n"
        "<norma titulus=\"b\"><textus electio=\"p q\"/></norma>\n"
        "</normae>\n", NIHIL, p), 0);
    CREDO_VERUM(_canon_habet(NORMA_A "<nemo/>" NORMA_Z,
        CANON_ELEMENTUM_IGNOTUM, p));
    CREDO_VERUM(_canon_habet(NORMA_A "<integer forma=\"uuid\"/>" NORMA_Z,
        CANON_ATTRIBUTUM_IGNOTUM, p));
    CREDO_VERUM(_canon_habet(NORMA_A "<ad norma=\"nemo\"/>" NORMA_Z,
        CANON_CITATIO_IRRITA, p));
    CREDO_VERUM(_canon_habet("<normae versio=\"1\">\n"
        "<norma titulus=\"a\"><textus/></norma>\n"
        "<norma titulus=\"a\"><textus/></norma>\n</normae>\n",
        CANON_NOMEN_BIS, p));
    CREDO_VERUM(_canon_habet(NORMA_A "<objectum><campus titulus=\"x\""
        " requiritur=\"fortasse\"><textus/></campus></objectum>" NORMA_Z,
        CANON_VALOR_MALUS, p));
    CREDO_VERUM(_canon_habet(NORMA_A "<textus/><textus/>" NORMA_Z,
        CANON_LIBERI_MULTI, p));
    /* unicitas intra objectum PROXIMUM: clavis eadem in objecto
     * interiore et exteriore licet, bis in eodem non licet */
    CREDO_AEQUALIS_I32(_canon_vitia(NORMA_A "<objectum>"
        "<campus titulus=\"id\"><textus/></campus>"
        "<campus titulus=\"interius\"><objectum>"
        "<campus titulus=\"id\"><textus/></campus>"
        "</objectum></campus></objectum>" NORMA_Z, NIHIL, p), 0);
    CREDO_VERUM(_canon_habet(NORMA_A "<objectum>"
        "<campus titulus=\"id\"><textus/></campus>"
        "<campus titulus=\"id\"><integer/></campus>"
        "</objectum>" NORMA_Z, CANON_NOMEN_BIS, p));
}

#define FIXA "probationes/fixa/norma/normae_fixae.norma"

interior JsonValor*
_ex_functione (
       Sors* sors,
    Piscina* p,
     vacuum* datum)
{
    (vacuum)sors;
    (vacuum)datum;
    redde json_chorda_creare_literis(p, "ex functione");
}

hic_manens constans NormaGignensNominatum _registrum[] = {
    { "ex_functione", _ex_functione, NIHIL }
};

interior chorda
_js (
      Norma* n,
    Piscina* p)
{
    redde json_scribere(norma_json_schema(n, p), p);
}

interior NormaStmlLectio
_lege (
    constans character* fons,
               Piscina* p)
{
    redde norma_stml_legere(chorda_ex_literis(fons, p), NIHIL, 0, p);
}

interior NormaStmlVitium*
_vitium_primum (
    NormaStmlLectio l)
{
    redde xar_numerus(l.vitia) > 0
        ? (NormaStmlVitium*)xar_obtinere(l.vitia, 0) : NIHIL;
}

/* causa vitii primi; bit in *visae */
interior b32
_causa (
    constans character* fons,
         NormaStmlCausa causa,
                   i32* visae,
               Piscina* p)
{
    NormaStmlLectio  l = _lege(fons, p);
    NormaStmlVitium* v = _vitium_primum(l);

    CREDO_FALSUM(l.successus);
    CREDO_AEQUALIS_I32(xar_numerus(l.normae), 0);
    si (!v)
    {
        redde FALSUM;
    }
    *visae |= (i32)I << (i32)v->causa;
    si (v->causa != causa)
    {
        imprimere("  exspectata %d, inventa %d: %.*s\n", (integer)causa,
            (integer)v->causa, (integer)v->nuntius.mensura,
            (constans character*)v->nuntius.datum);
    }
    redde v->causa == causa;
}

/* "usus" aedificatum manu - idem ac FIXA */
interior Norma*
_usus_manu (
    Piscina* p)
{
    Norma* o = norma_modus(norma_objectum(p), NORMA_NOTANDUM);

    norma_descriptio(o, "Usus signorum.");
    norma_campus(o, "input_tokens", norma_intra(norma_integer(p), 0,
        M * M), VERUM);
    norma_campus(o, "cache_read_input_tokens", norma_intra(
        norma_integer(p), 0, M * M), FALSUM);
    norma_campus(o, "ratio", norma_intra_fluitans(norma_numerus(p), 0.1,
        0.3), VERUM);
    norma_campus(o, "minima", norma_intra_fluitans(norma_numerus(p),
        -1e-300, 1e-300), FALSUM);
    redde o;
}

interior b32
_iud_validum (Norma* n, Piscina* p)
{
    NormaGenitum  g = norma_gignere(n, NORMA_TYPICA, VII, p);
       JsonValor* origo;

    si (!g.valor)
    {
        redde FALSUM;
    }
    si (json_est_nullum(g.valor))
    {
        g = norma_gignere(n, NORMA_TYPICA, VIII, p);   /* aut_nullum */
    }
    origo = json_objectum_capere(g.valor, "origo");
    CREDO_VERUM(origo && json_est_chorda(origo));
    redde norma_iudicare(n, g.valor, p).validum;
}

interior vacuum
probatio_lector(Piscina* p)
{
             chorda  fons = filum_legere_totum(FIXA, p);
    NormaStmlLectio  l;
    NormaStmlLectio  sine;
              Norma* omnia;
         NormaVisus  v;
        NormaCampus* primus;
        NormaCampus* secundus;
                i32  i;

    imprimere("\n--- Probans lectorem ---\n");
    l = norma_stml_legere(fons, _registrum, I, p);
    CREDO_VERUM(l.successus);
    CREDO_AEQUALIS_I32(xar_numerus(l.vitia), 0);
    CREDO_AEQUALIS_I32(xar_numerus(l.notae), 0);
    CREDO_AEQUALIS_I32(xar_numerus(l.normae), II);
    CREDO_CHORDA_AEQUALIS_LITERIS(((NormaNominata*)xar_obtinere(l.normae,
        0))->titulus, "usus");
    CREDO_CHORDA_AEQUALIS(_js(norma_stml_quaerere(&l, "usus"), p),
                          _js(_usus_manu(p), p));
    /* nomina sine signo (A2): '#usus' nomen non est */
    CREDO_NIHIL(norma_stml_quaerere(&l, "#usus"));
    CREDO_NIHIL(norma_stml_quaerere(&l, "nemo"));
    /* referentia communis: idem nodus bis, idem ac norma nominata */
    omnia     = norma_stml_quaerere(&l, "omnia");
    v         = norma_visus(omnia);
    primus    = NIHIL;
    secundus  = NIHIL;
    per (i = 0; i < xar_numerus(v.campi); i++)
    {
        NormaCampus* c = (NormaCampus*)xar_obtinere(v.campi, i);

        si (chorda_aequalis_literis(c->titulus, "usus_primus")) primus =
                                                                    c;
        si (chorda_aequalis_literis(c->titulus,
            "usus_secundus")) secundus = c;
    }
    CREDO_NON_NIHIL(primus);
    CREDO_NON_NIHIL(secundus);
    si (primus && secundus)
    {
        CREDO_VERUM(primus->valor == secundus->valor);
        CREDO_VERUM(primus->valor == norma_stml_quaerere(&l, "usus"));
        CREDO_FALSUM(secundus->requiritur);
    }
    CREDO_VERUM(v.aut_nullum);
    /* gignens: registro dato functio ponitur */
    CREDO_VERUM(_iud_validum(omnia, p));
    /* sine registro: nota una, titulus servatus, successus */
    sine = norma_stml_legere(fons, NIHIL, 0, p);
    CREDO_VERUM(sine.successus);
    CREDO_AEQUALIS_I32(xar_numerus(sine.notae), I);
    si (xar_numerus(sine.notae) == I)
    {
        CREDO_VERUM(((NormaStmlVitium*)xar_obtinere(sine.notae,
            0))->causa
                    == NORMA_STML_GIGNENS_SINE_REGISTRO);
    }
}

#define CAPUT "<normae versio=\"1\">\n<norma titulus=\"a\">\n"
#define CAUDA "\n</norma>\n</normae>\n"

interior vacuum
probatio_vitia_lectoris(Piscina* p)
{
                i32  visae = 0;
    NormaStmlLectio  l;
    NormaStmlVitium* v;

    imprimere("\n--- Probans vitia lectoris ---\n");
    CREDO_VERUM(_causa("<normae versio=\"1\"", NORMA_STML_FRACTUM,
        &visae, p));
    CREDO_VERUM(_causa(CAPUT "<nemo/>" CAUDA, NORMA_STML_CANON, &visae,
        p));
    CREDO_VERUM(_causa(CAPUT "<objectum><campus titulus=\"x\"/></objectum>"
        CAUDA, NORMA_STML_LIBERI_TYPI, &visae, p));
    CREDO_VERUM(_causa(CAPUT "<objectum><campus titulus=\"x\"><textus/>"
        "<integer/></campus></objectum>" CAUDA, NORMA_STML_LIBERI_TYPI,
        &visae, p));
    CREDO_VERUM(_causa(CAPUT "<numerus minimum=\"abc\" maximum=\"2\"/>"
        CAUDA, NORMA_STML_FINIS_PRAVUS, &visae, p));
    CREDO_VERUM(_causa(CAPUT "<integer minimum=\"99999999999999999999\""
        " maximum=\"1\"/>" CAUDA, NORMA_STML_FINIS_PRAVUS, &visae, p));
    CREDO_VERUM(_causa(CAPUT "<numerus minimum=\"0\" maximum=\"inf\"/>"
        CAUDA, NORMA_STML_FINIS_PRAVUS, &visae, p));
    CREDO_VERUM(_causa(CAPUT "<integer minimum=\"0\"/>" CAUDA,
        NORMA_STML_FINIS_DIMIDIATUS, &visae, p));
    CREDO_VERUM(_causa(CAPUT "<textus longitudo_maxima=\"3\"/>" CAUDA,
        NORMA_STML_FINIS_DIMIDIATUS, &visae, p));
    CREDO_VERUM(_causa(CAPUT "<integer minimum=\"5\" maximum=\"2\"/>"
        CAUDA, NORMA_STML_FINES_INVERSI, &visae, p));
    CREDO_VERUM(_causa(CAPUT "<textus electio=\"a\"><licitum>b</licitum>"
        "</textus>" CAUDA, NORMA_STML_ELECTIO_DUPLEX, &visae, p));
    CREDO_VERUM(_causa(CAPUT "<textus electio=\"   \"/>" CAUDA,
        NORMA_STML_ELECTIO_VACUA, &visae, p));
    CREDO_VERUM(_causa("<normae versio=\"1\">\n"
        "<norma titulus=\"a\"><ad norma=\"b\"/></norma>\n"
        "<norma titulus=\"b\"><ad norma=\"a\"/></norma>\n</normae>\n",
        NORMA_STML_CIRCULUS, &visae, p));
    /* circulus catenam nominat */
    v = _vitium_primum(_lege("<normae versio=\"1\">\n"
        "<norma titulus=\"a\"><ad norma=\"b\"/></norma>\n"
        "<norma titulus=\"b\"><ad norma=\"a\"/></norma>\n</normae>\n",
        p));
    CREDO_VERUM(v && chorda_continet(v->nuntius,
        chorda_ex_literis("a -> b -> a", p)));
    /* gignens ignotum: registro dato */
    l = norma_stml_legere(chorda_ex_literis(CAPUT
        "<textus gignens=\"nemo\"/>" CAUDA, p), _registrum, I, p);
    CREDO_FALSUM(l.successus);
    v = _vitium_primum(l);
    CREDO_VERUM(v && v->causa == NORMA_STML_GIGNENS_IGNOTUM);
    si (v)
    {
        visae |= (i32)I << (i32)v->causa;
    }
    /* nota sine registro: successus, nota */
    l = _lege(CAPUT "<textus gignens=\"sententia\"/>" CAUDA, p);
    CREDO_VERUM(l.successus);
    CREDO_AEQUALIS_I32(xar_numerus(l.notae), I);
    si (xar_numerus(l.notae) == I)
    {
        visae |= (i32)I << (i32)((NormaStmlVitium*)xar_obtinere(l.notae,
            0))->causa;
    }
    /* linea et columna: elementum integer in linea III, columna I */
    v = _vitium_primum(_lege(CAPUT "<integer minimum=\"0\"/>" CAUDA,
        p));
    CREDO_VERUM(v != NIHIL);
    si (v)
    {
        CREDO_AEQUALIS_I32(v->linea, III);
        CREDO_AEQUALIS_I32(v->columna, I);
    }
    /* vitia OMNIA, non primum solum */
    l =
        _lege(CAPUT "<objectum><campus titulus=\"x\"><integer minimum=\"0\"/>"
        "</campus><campus titulus=\"y\"><integer minimum=\"5\""
        " maximum=\"2\"/></campus></objectum>" CAUDA, p);
    CREDO_AEQUALIS_I32(xar_numerus(l.vitia), II);
    /* tegumentum: classis omnis visa (FRACTUM..SINE_REGISTRO) */
    CREDO_AEQUALIS_I32(visae, ((i32)I
        << (NORMA_STML_GIGNENS_SINE_REGISTRO
        + I)) - I);
}

interior NormaNominata*
_tabula (
        Xar* x,
    Piscina* p)
{
    NormaNominata* t = (NormaNominata*)piscina_allocare(p,
        (memoriae_index)((xar_numerus(x)
            + I) * magnitudo(NormaNominata)));

    xar_copiare_ad_tabulam(x, t, 0, xar_numerus(x));
    redde t;
}

interior chorda
_scribe (
        Xar* normae,
    Piscina* p)
{
    chorda causa;
    chorda s = norma_stml_scribere(_tabula(normae, p),
        xar_numerus(normae),
                   p, &causa);

    si (s.mensura == 0)
    {
        imprimere("  recusatum: %.*s\n", (integer)causa.mensura,
            (constans character*)causa.datum);
    }
    redde s;
}

interior b32
_iudicia_aequalia (
    NormaIudicium a,
    NormaIudicium b)
{
    i32 i;

    si (   a.validum            != b.validum
        || xar_numerus(a.vitia) != xar_numerus(b.vitia)
        || xar_numerus(a.notae) != xar_numerus(b.notae))
    {
        redde FALSUM;
    }
    per (i = 0; i < xar_numerus(a.vitia) + xar_numerus(a.notae); i++)
    {
                b32  vit  = i < xar_numerus(a.vitia);
                i32  k    = vit ? i : i - xar_numerus(a.vitia);
        NormaVitium* x =
            (NormaVitium*)xar_obtinere(vit ? a.vitia : a.notae, k);
        NormaVitium* y =
            (NormaVitium*)xar_obtinere(vit ? b.vitia : b.notae, k);

        si (x->causa != y->causa || !chorda_aequalis(x->via, y->via))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* duae normae idem dicunt: exportatio eadem, iudicia eadem super
 * valores ab UTRAQUE genitos (TYPICA, FINES, INVALIDA; L semina) */
interior i32
_idem_dicunt (
      Norma* a,
      Norma* b,
    Piscina* p)
{
    i32 diversa = 0;
    s64 semen;
    i32 m;
    i32 q;

    si (!chorda_aequalis(_js(a, p), _js(b, p)))
    {
        diversa++;
    }
    per (q = 0; q < II; q++)
    {
        per (semen = 0; semen < L; semen++)
        {
            per (m = (i32)NORMA_TYPICA; m <= (i32)NORMA_INVALIDA; m++)
            {
                NormaGenitum g = norma_gignere(q == 0 ? a : b,
                    (NormaModusGignendi)m, semen, p);

                si (   g.valor && !_iudicia_aequalia(norma_iudicare(a,
                        g.valor, p), norma_iudicare(b, g.valor, p)))
                {
                    diversa++;
                }
            }
        }
    }
    redde diversa;
}

/* oraculum V: forma pulchra = forma plagulae (formator nihil mutaret) */
interior b32
_formatum (
     chorda  x,
    Piscina* p)
{
    StmlResultus r = stml_legere(x, p, internamentum_creare(p));

    redde r.successus
        && chorda_aequalis(stml_scribere(r.radix, p, VERUM),
                                         x);
}

/* corpus aedificatorum (copiae ex probatio_norma_gignere.c, titulo
 * gignentis addito; aliter scriptor recte recusat) */
interior Norma*
_schema_simplex (
    Piscina* p)
{
    constans character* constans licita[]  = { "a", "b", "c", NIHIL };
    Norma* o                               = norma_objectum(p);

    norma_campus(o, "id", norma_intra(norma_integer(p), 0, MMXLVIII),
        VERUM);
    norma_campus(o, "nomen", norma_longitudo(norma_textus(p), I, XL),
        FALSUM);
    norma_campus(o, "email", norma_forma(norma_textus(p), "email"),
        VERUM);
    norma_campus(o, "codex",
        norma_longitudo(norma_forma(norma_textus(p),
        "uuid"), XXXVI, XXXVI), VERUM);
    norma_campus(o, "tags", norma_longitudo(norma_tabulatum(p,
        norma_electio(norma_textus(p), licita)), 0, IV), FALSUM);
    norma_campus(o, "ratio", norma_intra_fluitans(norma_numerus(p), 0.1,
        0.3), VERUM);
    norma_campus(o, "nota", norma_aut_nullum(norma_textus(p)), FALSUM);
    norma_campus(o, "datum", norma_forma(norma_textus(p), "date-time"),
        VERUM);
    norma_campus(o, "origo", norma_gignens_titulus(norma_gignens(
        norma_textus(p), _ex_functione, NIHIL), "ex_functione"), VERUM);
    redde o;
}

interior Norma*
_schema_responsi (
    Piscina* p)
{
    constans character* constans fines[] = { "end_turn", "tool_use",
        "max_tokens", NIHIL };
    Norma* textus   = norma_objectum(p);
    Norma* petitum  = norma_objectum(p);
    Norma* blocus   = norma_discrimen(p, "type");
    Norma* usus     = norma_objectum(p);
    Norma* r        = norma_objectum(p);

    norma_campus(textus, "text", norma_textus(p), VERUM);
    norma_campus(petitum, "id", norma_textus(p), VERUM);
    norma_campus(petitum, "name", norma_textus(p), VERUM);
    norma_campus(petitum, "input", norma_modus(norma_objectum(p),
        NORMA_APERTUM), VERUM);
    norma_variatio(blocus, "text", textus);
    norma_variatio(blocus, "tool_use", petitum);
    norma_campus(usus, "input_tokens", norma_intra(norma_integer(p), 0,
        M * M), VERUM);
    norma_campus(usus, "output_tokens", norma_intra(norma_integer(p), 0,
        M * M), VERUM);
    norma_campus(r, "id", norma_textus(p), VERUM);
    norma_campus(r, "content", norma_tabulatum(p, blocus), VERUM);
    norma_campus(r, "stop_reason", norma_electio(norma_textus(p),
        fines),
        VERUM);
    norma_campus(r, "usage", usus, VERUM);
    redde r;
}

interior vacuum
probatio_oraculum_primum(Piscina* p)
{
             chorda fixa  = filum_legere_totum(FIXA, p);
    NormaStmlLectio l1    = norma_stml_legere(fixa, _registrum, I, p);
             chorda s1    = _scribe(l1.normae, p);
    NormaStmlLectio l2    = norma_stml_legere(s1, _registrum, I, p);
             chorda s2    = _scribe(l2.normae, p);
    NormaStmlLectio l3    = norma_stml_legere(s2, _registrum, I, p);
             chorda s3    = _scribe(l3.normae, p);
         NormaVisus ratio;

    imprimere("\n--- Oraculum I: scribere(legere(x)) == x, bis ---\n");
    CREDO_VERUM(l1.successus && l2.successus && l3.successus);
    CREDO_CHORDA_AEQUALIS(s1, fixa);
    CREDO_CHORDA_AEQUALIS(s2, s1);
    CREDO_CHORDA_AEQUALIS(s3, s2);
    CREDO_AEQUALIS_I32(_idem_dicunt(norma_stml_quaerere(&l1, "omnia"),
        norma_stml_quaerere(&l3, "omnia"), p), 0);
    /* fines fluitantes per textum intacti */
    ratio = norma_visus(((NormaCampus*)xar_obtinere(norma_visus(
        norma_stml_quaerere(&l3, "usus")).campi, II))->valor);
    CREDO_VERUM(ratio.minimum_fluitans == 0.1);
    CREDO_VERUM(ratio.maximum_fluitans == 0.3);
    imprimere("\n--- Oraculum V: plagula forma pulchra ---\n");
    CREDO_VERUM(_formatum(fixa, p));
}

interior vacuum
probatio_oraculum_secundum(Piscina* p)
{
    Norma* corpus[II];
      i32  i;

    imprimere("\n--- Oraculum II: aedificator -> scriptor -> lector ---\n");
    corpus[0] = _schema_simplex(p);
    corpus[I] = _schema_responsi(p);
    per (i = 0; i < II; i++)
    {
          NormaNominata nn;
                 chorda causa;
                 chorda s;
        NormaStmlLectio l;

        nn.titulus  = chorda_ex_literis("x", p);
        nn.norma    = corpus[i];
        s           = norma_stml_scribere(&nn, I, p, &causa);
        CREDO_VERUM(s.mensura > 0);
        l = norma_stml_legere(s, _registrum, I, p);
        CREDO_VERUM(l.successus);
        si (!l.successus)
        {
            perge;
        }
        CREDO_AEQUALIS_I32(_idem_dicunt(corpus[i],
            norma_stml_quaerere(&l, "x"), p), 0);
        CREDO_CHORDA_AEQUALIS(_scribe(l.normae, p), s);
        CREDO_VERUM(_formatum(s, p));
    }
}

interior vacuum
probatio_scriptor_communis_et_recusationes(Piscina* p)
{
              Norma* usus      = norma_objectum(p);
              Norma* r         = norma_objectum(p);
              Norma* circulus  = norma_objectum(p);
      NormaNominata  nn[II];
    NormaStmlLectio  l;
             chorda  causa;
             chorda  s;
         NormaVisus  v;

    imprimere("\n--- Probans scriptorem: communia, recusationes ---\n");
    norma_campus(usus, "n", norma_integer(p), VERUM);
    norma_campus(r, "a", usus, VERUM);
    norma_campus(r, "b", usus, FALSUM);
    nn[0].titulus  = chorda_ex_literis("usus", p);
    nn[0].norma    = usus;
    nn[I].titulus  = chorda_ex_literis("r", p);
    nn[I].norma    = r;
    s              = norma_stml_scribere(nn, II, p, &causa);
    CREDO_VERUM(chorda_continet(s, chorda_ex_literis(
        "<ad norma=\"usus\"/>", p)));
    l = norma_stml_legere(s, NIHIL, 0, p);
    v = norma_visus(norma_stml_quaerere(&l, "r"));
    CREDO_VERUM(((NormaCampus*)xar_obtinere(v.campi, 0))->valor
                == ((NormaCampus*)xar_obtinere(v.campi, I))->valor);
    /* clavis quam STML ferre nequit (R6) */
    nn[0].norma = norma_campus(norma_objectum(p), "a\"b",
        norma_textus(p),
        VERUM);
    CREDO_AEQUALIS_I32(norma_stml_scribere(nn, I, p, &causa).mensura,
        0);
    CREDO_VERUM(chorda_continet(causa,
        chorda_ex_literis("STML ferre nequit",
        p)));
    nn[0].norma = norma_campus(norma_objectum(p), "a\nb",
        norma_textus(p),
        VERUM);
    CREDO_AEQUALIS_I32(norma_stml_scribere(nn, I, p, &causa).mensura,
        0);
    /* electio cum '<' -> licitum (textus effugitur), non recusatio */
    {
        constans character* constans licita[] = { "a<b", "c d", NIHIL };

        nn[0].norma  = norma_electio(norma_textus(p), licita);
        s            = norma_stml_scribere(nn, I, p, &causa);
        CREDO_VERUM(s.mensura > 0);
        l = norma_stml_legere(s, NIHIL, 0, p);
        CREDO_VERUM(l.successus);
        CREDO_AEQUALIS_I32(_idem_dicunt(nn[0].norma,
            norma_stml_quaerere(&l, "usus"), p), 0);
    }
    /* gignens cum functione sine titulo */
    nn[0].norma = norma_gignens(norma_textus(p), _ex_functione, NIHIL);
    CREDO_AEQUALIS_I32(norma_stml_scribere(nn, I, p, &causa).mensura,
        0);
    /* nodus pravus */
    nn[0].norma = norma_longitudo(norma_textus(p), V, II);
    CREDO_AEQUALIS_I32(norma_stml_scribere(nn, I, p, &causa).mensura,
        0);
    /* circulus aedificatorum: nominatus et innominatus */
    norma_campus(circulus, "ego", circulus, VERUM);
    nn[0].norma = circulus;
    CREDO_AEQUALIS_I32(norma_stml_scribere(nn, I, p, &causa).mensura,
        0);
    CREDO_VERUM(chorda_continet(causa, chorda_ex_literis("circulus",
        p)));
    nn[0].norma = norma_campus(norma_objectum(p), "intus", circulus,
        VERUM);
    CREDO_AEQUALIS_I32(norma_stml_scribere(nn, I, p, &causa).mensura,
        0);
    CREDO_VERUM(chorda_continet(causa, chorda_ex_literis("profunditas",
        p)));
}

/* gignens quem fons generatus externum declarat (R7) */
JsonValor*
fixa_norma_gignens_ex_functione (
       Sors* sors,
    Piscina* p,
     vacuum* datum)
{
    redde _ex_functione(sors, p, datum);
}

interior vacuum
probatio_oraculum_tertium(Piscina* p)
{
    NormaGignensNominatum  gignentes[I];
                   chorda  fixa = filum_legere_totum(FIXA, p);
          NormaStmlLectio  l;
               NormaFonsC  fons;
                   chorda  causa;
                    Norma* omnia;
               NormaVisus  v;
               NormaVisus  a;
               NormaVisus  b;

    imprimere("\n--- Oraculum III: emissor contra lectorem ---\n");
    gignentes[0].titulus  = "ex_functione";
    gignentes[0].functio  = fixa_norma_gignens_ex_functione;
    gignentes[0].datum    = NIHIL;
    l                     = norma_stml_legere(fixa, gignentes, I, p);
    CREDO_VERUM(l.successus);
    CREDO_AEQUALIS_I32(_idem_dicunt(norma_stml_quaerere(&l, "usus"),
        fixa_norma_usus(p), p), 0);
    CREDO_AEQUALIS_I32(_idem_dicunt(norma_stml_quaerere(&l, "omnia"),
        fixa_norma_omnia(p), p), 0);
    /* DAG in C servatum */
    omnia  = fixa_norma_omnia(p);
    v      = norma_visus(omnia);
    {
        Norma* primus    = NIHIL;
        Norma* secundus  = NIHIL;
          i32  i;

        per (i = 0; i < xar_numerus(v.campi); i++)
        {
            NormaCampus* c = (NormaCampus*)xar_obtinere(v.campi, i);

            si (chorda_aequalis_literis(c->titulus, "usus_primus"))
            {
                primus = c->valor;
            }
            si (chorda_aequalis_literis(c->titulus, "usus_secundus"))
            {
                secundus = c->valor;
            }
        }
        CREDO_VERUM(primus && primus == secundus);
    }
    /* fines fluitantes per litteras C intacti: campus 'minima' (III) */
    a = norma_visus(((NormaCampus*)xar_obtinere(norma_visus(
        norma_stml_quaerere(&l, "usus")).campi, III))->valor);
    b = norma_visus(((NormaCampus*)xar_obtinere(norma_visus(
        fixa_norma_usus(p)).campi, III))->valor);
    CREDO_VERUM(a.minimum_fluitans == b.minimum_fluitans);
    CREDO_VERUM(a.maximum_fluitans == b.maximum_fluitans);
    /* fons commissus recens: emissor hodiernus idem scribit */
    fons = norma_ad_c(_tabula(l.normae, p), xar_numerus(l.normae),
        "fixa_norma_", "normae_fixae.h", FIXA, p, &causa);
    CREDO_CHORDA_AEQUALIS(fons.caput, filum_legere_totum(
        "probationes/fixa/norma/normae_fixae.h", p));
    CREDO_CHORDA_AEQUALIS(fons.corpus, filum_legere_totum(
        "probationes/fixa/norma/normae_fixae.c", p));
}

interior vacuum
probatio_emissor_recusat(Piscina* p)
{
    NormaNominata nn;
           chorda causa;
        character longus_textus[DCCC];

    imprimere("\n--- Probans emissorem recusantem ---\n");
    nn.titulus  = chorda_ex_literis("a-b", p);
    nn.norma    = norma_textus(p);
    CREDO_AEQUALIS_I32(norma_ad_c(&nn, I, "x_", "x.h", "x.norma", p,
        &causa).corpus.mensura, 0);
    nn.titulus = chorda_ex_literis("bonum", p);
    memset(longus_textus, 'a', DCCC - I);
    longus_textus[DCCC - I] = '\0';
    nn.norma = norma_descriptio(norma_textus(p), longus_textus);
    CREDO_AEQUALIS_I32(norma_ad_c(&nn, I, "x_", "x.h", "x.norma", p,
        &causa).corpus.mensura, 0);
    CREDO_VERUM(chorda_continet(causa, chorda_ex_literis("DIX", p)));
    nn.norma = norma_intra_fluitans(norma_numerus(p), 0.0, HUGE_VAL);
    CREDO_AEQUALIS_I32(norma_ad_c(&nn, I, "x_", "x.h", "x.norma", p,
        &causa).corpus.mensura, 0);
    nn.norma = norma_gignens(norma_textus(p), _ex_functione, NIHIL);
    CREDO_AEQUALIS_I32(norma_ad_c(&nn, I, "x_", "x.h", "x.norma", p,
        &causa).corpus.mensura, 0);
    /* literae: '?' et '"' et linea nova effugiuntur (trigraphi!) */
    nn.norma = norma_descriptio(norma_textus(p), "a?\?-b \"c\"\nd\\e");
    CREDO_VERUM(chorda_continet(norma_ad_c(&nn, I, "x_", "x.h",
        "x.norma",
        p, &causa).corpus, chorda_ex_literis(
        "\"a\\?\\?-b \\\"c\\\"\\nd\\\\e\"", p)));
}

/* recensio finalis (norma-plan-3): textus FIDELIS per scriptorem et
 * lectorem - linea nova in descriptione et in licito servatur (STML
 * lineas novas textus ordinarii molles facit); clavis VACUA recusatur
 * (STML attributum vacuum nudum scribit, quod 'true' relegitur) */
interior vacuum
probatio_textus_fidelis(Piscina* p)
{
    constans character* constans licita[] = { "a\nb", "c", NIHIL };
      NormaNominata nn;
    NormaStmlLectio l;
             chorda causa;
             chorda s;

    imprimere("\n--- Probans textum fidelem ---\n");
    nn.titulus = chorda_ex_literis("x", p);
    nn.norma   = norma_descriptio(norma_electio(norma_textus(p),
        licita),
        "Linea prima.\nLinea secunda.");
    s = norma_stml_scribere(&nn, I, p, &causa);
    CREDO_VERUM(s.mensura > 0);
    l = norma_stml_legere(s, NIHIL, 0, p);
    CREDO_VERUM(l.successus);
    si (l.successus)
    {
        CREDO_AEQUALIS_I32(_idem_dicunt(nn.norma,
            norma_stml_quaerere(&l, "x"), p), 0);
        CREDO_CHORDA_AEQUALIS(_scribe(l.normae, p), s);
        CREDO_VERUM(_formatum(s, p));
    }
    /* clavis vacua, valor variationis vacuus, clavis discriminis vacua */
    nn.norma = norma_campus(norma_objectum(p), "", norma_textus(p),
        VERUM);
    CREDO_AEQUALIS_I32(norma_stml_scribere(&nn, I, p, &causa).mensura,
        0);
    CREDO_VERUM(chorda_continet(causa, chorda_ex_literis("vacu", p)));
    nn.norma = norma_variatio(norma_discrimen(p, "type"), "",
        norma_objectum(p));
    CREDO_AEQUALIS_I32(norma_stml_scribere(&nn, I, p, &causa).mensura,
        0);
    nn.norma = norma_variatio(norma_discrimen(p, ""), "a",
        norma_objectum(p));
    CREDO_AEQUALIS_I32(norma_stml_scribere(&nn, I, p, &causa).mensura,
        0);
}

/* recensio externa (norma-plan-3, recensor alter): scriptor aut
 * fideliter scribit aut nominatim recusat; lector numquam ruit;
 * circulus inter normas nominatas recusatur */
interior b32
_textus_recusatus (
    constans character* textus,
               Piscina* p)
{
    NormaNominata nn;
           chorda causa;

    nn.titulus  = chorda_ex_literis("x", p);
    nn.norma    = norma_descriptio(norma_textus(p), textus);
    redde norma_stml_scribere(&nn, I, p, &causa).mensura == 0;
}

interior b32
_textus_redit (
    constans character* textus,
               Piscina* p)
{
      NormaNominata nn;
    NormaStmlLectio l;
             chorda causa;
             chorda s;

    nn.titulus  = chorda_ex_literis("x", p);
    nn.norma    = norma_descriptio(norma_textus(p), textus);
    s           = norma_stml_scribere(&nn, I, p, &causa);
    si (s.mensura == 0)
    {
        redde FALSUM;
    }
    l = norma_stml_legere(s, NIHIL, 0, p);
    redde l.successus && _idem_dicunt(nn.norma,
        norma_stml_quaerere(&l, "x"), p) == 0;
}

/* scriptor recusat (mensura 0) et causa verbum continet */
interior b32
_recusat (
                 Norma* n,
    constans character* verbum,
               Piscina* p)
{
    NormaNominata nn;
           chorda causa;
           chorda s;

    nn.titulus  = chorda_ex_literis("x", p);
    nn.norma    = n;
    s           = norma_stml_scribere(&nn, I, p, &causa);
    redde s.mensura == 0 && chorda_continet(causa,
        chorda_ex_literis(verbum, p));
}

interior b32
_redit (
      Norma* n,
    Piscina* p)
{
      NormaNominata nn;
    NormaStmlLectio l;
             chorda causa;
             chorda s;

    nn.titulus  = chorda_ex_literis("x", p);
    nn.norma    = n;
    s           = norma_stml_scribere(&nn, I, p, &causa);
    si (s.mensura == 0)
    {
        imprimere("  recusatum: %.*s\n", (integer)causa.mensura,
            (constans character*)causa.datum);
        redde FALSUM;
    }
    l = norma_stml_legere(s, NIHIL, 0, p);
    redde l.successus && _idem_dicunt(n, norma_stml_quaerere(&l, "x"),
        p) == 0;
}

interior vacuum
probatio_recensio_externa(Piscina* p)
{
    constans character* constans licita[] = { "a\n b", NIHIL };
          character  longum[MMMCC];
          character  documentum[MMMD];
      NormaNominata  nn[II];
    NormaStmlLectio  l;
             chorda  causa;
              Norma* a;
              Norma* b;

    imprimere("\n--- Probans recensionem externam ---\n");
    /* I. textus multilineae: aut idem redit aut recusatur */
    CREDO_VERUM(_textus_redit("Linea prima.\nLinea secunda.", p));
    CREDO_VERUM(_textus_redit("a\nb\nc", p));
    CREDO_VERUM(_textus_redit("  a\nb", p));
    CREDO_VERUM(_textus_redit("Paragraphus primus.\n\nParagraphus secundus.",
        p));
    CREDO_VERUM(_textus_recusatus("  a\n  b", p));
    CREDO_VERUM(_textus_recusatus("a\n  b", p));
    CREDO_VERUM(_textus_recusatus("\ta\n\tb", p));
    CREDO_VERUM(_textus_recusatus(" a\n b ", p));
    CREDO_VERUM(_textus_recusatus("a \nb", p));
    CREDO_VERUM(_textus_recusatus("a\n", p));
    CREDO_VERUM(_textus_recusatus("a\n\n", p));
    CREDO_VERUM(_textus_recusatus("\na", p));
    CREDO_VERUM(_textus_recusatus("\n\na", p));
    CREDO_VERUM(_textus_recusatus("a\r\nb", p));
    CREDO_VERUM(_textus_recusatus("a\n  \nb", p));
    CREDO_VERUM(_recusat(norma_electio(norma_textus(p), licita),
        "multilineae", p));
    /* II. forma et gignens generibus canonis subiecta */
    CREDO_VERUM(_recusat(norma_forma(norma_textus(p), "date time"),
        "forma", p));
    CREDO_VERUM(_recusat(norma_forma(norma_textus(p), "x.y"), "forma",
        p));
    CREDO_VERUM(_redit(norma_forma(norma_textus(p), "date-time"), p));
    CREDO_VERUM(_recusat(norma_gignens_titulus(norma_textus(p),
        "my-gen"),
        "gignens", p));
    CREDO_VERUM(_recusat(norma_gignens_titulus(norma_textus(p), "a b"),
        "gignens", p));
    /* III. fines fluitantes: non finiti recusantur, subnormales redeunt */
    CREDO_VERUM(_recusat(norma_intra_fluitans(norma_numerus(p), 0.0,
        HUGE_VAL), "finitus", p));
    CREDO_VERUM(_recusat(norma_intra_fluitans(norma_numerus(p),
        -HUGE_VAL,
        0.0), "finitus", p));
    CREDO_VERUM(_redit(norma_intra_fluitans(norma_numerus(p),
        4.9406564584124654e-324, 1.0), p));
    CREDO_VERUM(_redit(norma_intra_fluitans(norma_numerus(p), -1e-320,
        1e-320), p));
    l = _lege(CAPUT "<numerus minimum=\"1e-400\" maximum=\"1\"/>" CAUDA,
        p);
    CREDO_VERUM(_vitium_primum(l) != NIHIL
        && _vitium_primum(l)->causa == NORMA_STML_FINIS_PRAVUS);
    /* IV. documentum sine elemento radicis: vitium, non ruina */
    CREDO_NON_RUIT((vacuum)norma_stml_legere(chorda_ex_literis("  \n\t ",
        p),
        NIHIL, 0, p));
    CREDO_NON_RUIT((vacuum)norma_stml_legere(chorda_ex_literis(
        "<!-- x -->", p), NIHIL, 0, p));
    l = _lege("  \n\t ", p);
    CREDO_FALSUM(l.successus);
    CREDO_VERUM(_vitium_primum(l) != NIHIL
        && _vitium_primum(l)->causa == NORMA_STML_FRACTUM);
    /* V. nomina longa in nuntiis canonis: non ruina */
    memset(longum, 'A', MMMCC - I);
    longum[MMMCC - I] = '\0';
    sprintf(documentum,
        "<normae versio=\"1\">\n<norma titulus=\"a\"><%s/>"
        "</norma>\n</normae>\n", longum);
    CREDO_NON_RUIT((vacuum)norma_stml_legere(chorda_ex_literis(documentum,
        p), NIHIL, 0, p));
    sprintf(documentum, "<normae versio=\"1\">\n<norma titulus=\"a\">"
        "<ad norma=\"%s\"/></norma>\n</normae>\n", longum);
    CREDO_NON_RUIT((vacuum)norma_stml_legere(chorda_ex_literis(documentum,
        p), NIHIL, 0, p));
    l = _lege(documentum, p);
    CREDO_VERUM(_vitium_primum(l) != NIHIL
        && _vitium_primum(l)->causa == NORMA_STML_CANON);
    /* VI. circulus inter normas nominatas: scriptor et emissor recusant */
    a = norma_objectum(p);
    b = norma_objectum(p);
    norma_campus(a, "b", b, VERUM);
    norma_campus(b, "a", a, VERUM);
    nn[0].titulus  = chorda_ex_literis("a", p);
    nn[0].norma    = a;
    nn[I].titulus  = chorda_ex_literis("b", p);
    nn[I].norma    = b;
    CREDO_AEQUALIS_I32(norma_stml_scribere(nn, II, p, &causa).mensura,
        0);
    CREDO_VERUM(chorda_continet(causa, chorda_ex_literis("a -> b -> a",
        p)));
    CREDO_AEQUALIS_I32(norma_ad_c(nn, II, "x_", "x.h", "x.norma", p,
        &causa).corpus.mensura, 0);
    CREDO_VERUM(chorda_continet(causa, chorda_ex_literis("a -> b -> a",
        p)));
}

s32
principale (vacuum)
{
    Piscina* p = piscina_generare_dynamicum("probatio_norma_stml",
        M * M);
        b32 successus;

    credo_aperire(p);
    probatio_canon_infixus(p);
    probatio_lector(p);
    probatio_vitia_lectoris(p);
    probatio_oraculum_primum(p);
    probatio_oraculum_secundum(p);
    probatio_scriptor_communis_et_recusationes(p);
    probatio_oraculum_tertium(p);
    probatio_emissor_recusat(p);
    probatio_textus_fidelis(p);
    probatio_recensio_externa(p);
    credo_imprimere_compendium();
    successus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(p);
    redde successus ? 0 : I;
}
