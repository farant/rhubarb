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

#include <stdio.h>
#include <string.h>

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
    credo_imprimere_compendium();
    successus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(p);
    redde successus ? 0 : I;
}
