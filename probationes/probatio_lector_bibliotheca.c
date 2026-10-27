/* probatio_lector_bibliotheca.c - bibliotheca codicum (lector L3 T2):
 * directorium (ordo titulorum sine casu, identitas duplex = liber unus,
 * filum non legibile nominatum, cetera ignorata, directorium absens),
 * codex servatus, positiones in volumine (ultima vincit, identitas cum
 * ':' et '/', vestigium ad runam sectum, volumen alterum idem legit),
 * progressus. Fixa: probationes/fixa/lector/bibliotheca/ (fontes in
 * ../fontes; beta et gamma eandem identitatem habent). */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "volumen.h"
#include "codex.h"
#include "lector_bibliotheca.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define BIBLIOTHECA "probationes/fixa/lector/bibliotheca"

interior b32
desinit (
                 chorda  c,
     constans character* finis)
{
    i32 n;

    n = (i32)strlen(finis);
    redde c.mensura >= n
        && memcmp(c.datum + c.mensura - n, finis, (size_t)n)
            == ZEPHYRUM;
}

interior LectorPositio
positio (
                    i32  caput,
                    i32  initium,
     constans character* vestigium,
                Piscina* p)
{
    LectorPositio x;

    x.caput      = caput;
    x.initium    = initium;
    x.vestigium  = chorda_ex_literis(vestigium, p);
    redde x;
}

interior vacuum
_sectio_bibliotheca (
    CredoContextus* c)
{
                 Piscina* p;
     InternamentumChorda* in;
       LectorBibliotheca* b;
    constans LectorLiber* l;
                   Codex* x;
           LectorPositio  q;
                  chorda  vitium;

    p   = c->piscina;
    in  = internamentum_creare(p);

    CREDO_GRADUS("directorium: III libri, ordo tituli sine casu, "
        "non legibilis ultimus, .txt et duplex omissi");
    b = lector_bibliotheca_aperire(BIBLIOTHECA, NIHIL, p, in);
    CREDO_NECESSE_NON_NIHIL(b);
    CREDO_AEQUALIS_I32(lector_bibliotheca_numerus(b), III);
    l = lector_bibliotheca_liber(b, ZEPHYRUM);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_CHORDA_AEQUALIS_LITERIS(l->titulus, "aeneis");
    CREDO_CHORDA_AEQUALIS_LITERIS(l->identitas, "urn:rhubarb:beta");
    CREDO_CHORDA_AEQUALIS_LITERIS(l->auctor, "Publius Vergilius");
    CREDO_VERUM(desinit(l->via, "/beta.epub"));
    CREDO_AEQUALIS_I32(l->vitium.mensura, ZEPHYRUM);
    l = lector_bibliotheca_liber(b, I);
    CREDO_CHORDA_AEQUALIS_LITERIS(l->titulus, "Alpha");
    CREDO_CHORDA_AEQUALIS_LITERIS(l->auctor, "Anna Scriptrix");
    l = lector_bibliotheca_liber(b, II);
    CREDO_CHORDA_AEQUALIS_LITERIS(l->titulus, "fractum.epub");
    CREDO_VERUM(l->vitium.mensura > ZEPHYRUM);
    CREDO_AEQUALIS_I32(l->identitas.mensura, ZEPHYRUM);
    CREDO_NIHIL(lector_bibliotheca_liber(b, III));

    CREDO_GRADUS("invenire per identitatem");
    CREDO_VERUM(lector_bibliotheca_invenire(b, chorda_ex_literis(
        "urn:rhubarb:beta", p)) == lector_bibliotheca_liber(b,
        ZEPHYRUM));
    CREDO_NIHIL(lector_bibliotheca_invenire(b, chorda_ex_literis("x",
        p)));

    CREDO_GRADUS("codex servatus; non legibilis -> NIHIL et vitium");
    x = lector_bibliotheca_codex(b, lector_bibliotheca_liber(b, I),
        &vitium);
    CREDO_NECESSE_NON_NIHIL(x);
    CREDO_CHORDA_AEQUALIS_LITERIS(codex_identitas(x),
        "urn:rhubarb:alpha");
    CREDO_VERUM(lector_bibliotheca_codex(b, lector_bibliotheca_liber(b,
        I), &vitium) == x);
    vitium.mensura = ZEPHYRUM;
    CREDO_NIHIL(lector_bibliotheca_codex(b, lector_bibliotheca_liber(b,
        II), &vitium));
    CREDO_VERUM(vitium.mensura > ZEPHYRUM);

    CREDO_GRADUS("sine volumine: positiones nec scribuntur nec "
        "leguntur");
    q = positio(I, ZEPHYRUM, "x", p);
    CREDO_FALSUM(lector_positionem_scribere(b, chorda_ex_literis(
        "urn:rhubarb:alpha", p), &q));
    CREDO_FALSUM(lector_positionem_legere(b, chorda_ex_literis(
        "urn:rhubarb:alpha", p), &q, p));

    CREDO_GRADUS("directorium absens: bibliotheca vacua");
    b = lector_bibliotheca_aperire("probationes/fixa/lector/nusquam",
        NIHIL, p, in);
    CREDO_NECESSE_NON_NIHIL(b);
    CREDO_AEQUALIS_I32(lector_bibliotheca_numerus(b), ZEPHYRUM);
}

interior vacuum
_sectio_positio (
    CredoContextus* c)
{
                 Piscina* p;
     InternamentumChorda* in;
                 Volumen* vol;
       LectorBibliotheca* b;
       LectorBibliotheca* altera;
    constans LectorLiber* alpha;
                   Codex* x;
              CodexCaput* k;
           LectorPositio  q;
           LectorPositio  r;
                  chorda  vitium;
                  chorda  id;
                     i32  ante;
                     i32  omnes;
                     i32  primum;
                     i32  secundum;
                     i32  i;
                     b32  inventum;
               character  longum[XXXIV];

    p    = c->piscina;
    in   = internamentum_creare(p);
    vol  = volumen_temporarium(p, "lector_bibliotheca");
    CREDO_NECESSE_NON_NIHIL(vol);
    b      = lector_bibliotheca_aperire(BIBLIOTHECA, vol, p, in);
    alpha  = lector_bibliotheca_liber(b, I);
    id     = alpha->identitas;

    CREDO_GRADUS("liber novus: nulla positio, progressus -1");
    CREDO_FALSUM(lector_positionem_legere(b, id, &r, p));
    CREDO_AEQUALIS_S32(lector_progressus(b, alpha), -I);

    CREDO_GRADUS("scribere, legere; iterum: ultima vincit");
    q = positio(II, XL, "Secundum", p);
    CREDO_VERUM(lector_positionem_scribere(b, id, &q));
    CREDO_VERUM(lector_positionem_legere(b, id, &r, p));
    CREDO_AEQUALIS_I32(r.caput, II);
    CREDO_AEQUALIS_I32(r.initium, XL);
    CREDO_CHORDA_AEQUALIS_LITERIS(r.vestigium, "Secundum");
    q = positio(III, X, "Tertium", p);
    CREDO_VERUM(lector_positionem_scribere(b, id, &q));
    CREDO_VERUM(lector_positionem_legere(b, id, &r, p));
    CREDO_AEQUALIS_I32(r.caput, III);
    CREDO_CHORDA_AEQUALIS_LITERIS(r.vestigium, "Tertium");

    CREDO_GRADUS("bibliotheca altera eodem volumine eandem legit");
    altera = lector_bibliotheca_aperire(BIBLIOTHECA, vol, p, in);
    CREDO_VERUM(lector_positionem_legere(altera, id, &r, p));
    CREDO_AEQUALIS_I32(r.caput, III);

    CREDO_GRADUS("identitas cum ':' et '/' (Gutenberg) et vestigium "
        "cum "
        "citationibus et uncis");
    q = positio(I, VII, "Dixit \"salve\" <ait> & abiit", p);
    CREDO_VERUM(lector_positionem_scribere(b, chorda_ex_literis(
        "http://www.gutenberg.org/394", p), &q));
    CREDO_VERUM(lector_positionem_legere(b, chorda_ex_literis(
        "http://www.gutenberg.org/394", p), &r, p));
    CREDO_AEQUALIS_I32(r.initium, VII);
    CREDO_CHORDA_AEQUALIS_LITERIS(r.vestigium,
        "Dixit \"salve\" <ait> & abiit");
    CREDO_FALSUM(lector_positionem_legere(b, chorda_ex_literis(
        "http://www.gutenberg.org/395", p), &r, p));
    inventum = FALSUM;
    (vacuum)volumen_plagulam_promere(vol, chorda_ex_literis(
        "lector/positio/http%3A%2F%2Fwww.gutenberg.org%2F394", p), p,
        &inventum);
    CREDO_VERUM(inventum);

    CREDO_GRADUS("vestigium ad XXXII octetos, ad runam sectum");
    per (i = ZEPHYRUM; i < XXXI; i++)
    {
        longum[i] = 'a';
    }
    memcpy(longum + XXXI, "\xc3\xa9", II);
    longum[XXXIII]  = '\0';
    q               = positio(ZEPHYRUM, ZEPHYRUM, longum, p);
    CREDO_VERUM(lector_positionem_scribere(b, id, &q));
    CREDO_VERUM(lector_positionem_legere(b, id, &r, p));
    CREDO_AEQUALIS_I32(r.vestigium.mensura, XXXI);

    CREDO_GRADUS("progressus: initium 0, caput ultimum = octeti ante");
    CREDO_AEQUALIS_S32(lector_progressus(b, alpha), ZEPHYRUM);
    x         = lector_bibliotheca_codex(b, alpha, &vitium);
    ante      = ZEPHYRUM;
    omnes     = ZEPHYRUM;
    primum    = ZEPHYRUM;
    secundum  = ZEPHYRUM;
    per (i = ZEPHYRUM; i < codex_numerus_capitum(x); i++)
    {
        k = codex_caput_legere(x, i, p, &vitium);
        CREDO_NECESSE_NON_NIHIL(k);
        si (i == ZEPHYRUM)
        {
            primum = k->textus.mensura;
        }
        si (i == I)
        {
            secundum = k->textus.mensura;
        }
        si (i < codex_numerus_capitum(x) - I)
        {
            ante += k->textus.mensura;
        }
        omnes += k->textus.mensura;
    }
    q = positio(codex_numerus_capitum(x) - I, ZEPHYRUM, "line one", p);
    CREDO_VERUM(lector_positionem_scribere(b, id, &q));
    CREDO_AEQUALIS_S32(lector_progressus(b, alpha), (s32)(ante * C
        / omnes));
    CREDO_VERUM(lector_progressus(b, alpha) > ZEPHYRUM);
    CREDO_AEQUALIS_S32(lector_progressus(b, lector_bibliotheca_liber(b,
        II)), -I);

    CREDO_GRADUS("progressus intra caput: octeti ante initium "
        "numerantur");
    q = positio(I, secundum / II, "x", p);
    CREDO_VERUM(lector_positionem_scribere(b, id, &q));
    CREDO_AEQUALIS_S32(lector_progressus(b, alpha), (s32)((primum
        + secundum / II) * C / omnes));

    volumen_claudere(vol);
}

/* via plagulae longitudinum (prima "lector/longitudines/"); vacua si
 * nulla */
interior chorda
via_longitudinum (
     Volumen* vol,
     Piscina* p)
{
    Xar* x;
    i32  i;

    x = volumen_plagulas_enumerare(vol, p);
    per (i = ZEPHYRUM; x && i < xar_numerus(x); i++)
    {
        constans VolumenPlagula* g = (constans VolumenPlagula*)
            xar_obtinere(x, i);
        si (chorda_continet(g->via, chorda_ex_literis(
                "lector/longitudines/", p)))
        {
            redde g->via;
        }
    }
    redde chorda_ex_literis("", p);
}

/* lector L6: longitudines capitum in volumine servatae ("<capita>
 * <mensura fili> L0 L1 ..."): bibliotheca altera eas legit, non
 * capita; signum mutatum (capita alia) -> recomputantur */
interior vacuum
_sectio_longitudines (
    CredoContextus* c)
{
                 Piscina* p;
     InternamentumChorda* in;
                 Volumen* vol;
       LectorBibliotheca* b;
       LectorBibliotheca* altera;
                   Codex* x;
           LectorPositio  q;
                  chorda  vitium;
                  chorda  via;
                  chorda  servatum;
                  chorda  textus;
                     s32  verum;
                     i32  n;
                     i32  i;
                     i64  mensura;
                     b32  inventum;
               character  fictum[CCLVI];
               character  numerus[XXIV];

    p    = c->piscina;
    in   = internamentum_creare(p);
    vol  = volumen_temporarium(p, "lector_longitudines");
    CREDO_NECESSE_NON_NIHIL(vol);
    b  = lector_bibliotheca_aperire(BIBLIOTHECA, vol, p, in);
    x  = lector_bibliotheca_codex(b, lector_bibliotheca_liber(b, I),
        &vitium);
    CREDO_NECESSE_NON_NIHIL(x);
    n = codex_numerus_capitum(x);
    q = positio(n - I, ZEPHYRUM, "line one", p);
    CREDO_VERUM(lector_positionem_scribere(b,
        lector_bibliotheca_liber(b, I)->identitas, &q));

    CREDO_GRADUS("progressus primus: longitudines in volumine "
        "servatae");
    verum = lector_progressus(b, lector_bibliotheca_liber(b, I));
    CREDO_VERUM(verum > ZEPHYRUM);
    via = chorda_transcribere(via_longitudinum(vol, p), p);
    CREDO_VERUM(via.mensura > ZEPHYRUM);
    servatum = chorda_transcribere(volumen_plagulam_promere(vol, via, p,
        &inventum), p);
    CREDO_VERUM(inventum);

    CREDO_GRADUS("bibliotheca altera: longitudines ex volumine "
        "(fictae: C per caput)");
    /* signum servati: "<capita> <mensura>" */
    mensura = ZEPHYRUM;
    per (i = ZEPHYRUM; i < servatum.mensura && servatum.datum[i] != ' ';
        i++)
    {
    }
    per (i = i + I; i < servatum.mensura && servatum.datum[i] != ' ';
        i++)
    {
        mensura = mensura * X + (i64)(servatum.datum[i] - '0');
    }
    sprintf(fictum, "%u %lu", (insignatus integer)n,
        (insignatus longus)mensura);
    per (i = ZEPHYRUM; i < n; i++)
    {
        strcat(fictum, " 100");
    }
    CREDO_VERUM(volumen_plagulam_condere(vol, via, chorda_ex_literis(
        fictum, p), "probatio"));
    altera = lector_bibliotheca_aperire(BIBLIOTHECA, vol, p, in);
    CREDO_AEQUALIS_S32(lector_progressus(altera,
        lector_bibliotheca_liber(altera, I)), (s32)((n - I) * C / n));

    CREDO_GRADUS("signum alienum (capita alia): recomputantur et "
        "rescribuntur");
    sprintf(numerus, "%u %lu 1 2", (insignatus integer)(n + I),
        (insignatus longus)mensura);
    CREDO_VERUM(volumen_plagulam_condere(vol, via, chorda_ex_literis(
        numerus, p), "probatio"));
    altera = lector_bibliotheca_aperire(BIBLIOTHECA, vol, p, in);
    CREDO_AEQUALIS_S32(lector_progressus(altera,
        lector_bibliotheca_liber(altera, I)), verum);
    textus = volumen_plagulam_promere(vol, via, p, &inventum);
    CREDO_VERUM(chorda_aequalis(textus, servatum));
    volumen_claudere(vol);
}

interior LectorEmendatio
emendatio (
    CodexEmendationisGenus  genus,
                       i32  caput,
                       i32  positio,
        constans character* vestigium,
                   Piscina* p)
{
    LectorEmendatio x;

    x.genus      = genus;
    x.caput      = caput;
    x.positio    = positio;
    x.vestigium  = chorda_ex_literis(vestigium, p);
    redde x;
}

interior b32
emendatio_est (
                       Xar* x,
                       i32  k,
    CodexEmendationisGenus  genus,
                       i32  caput,
                       i32  positio,
        constans character* vestigium)
{
    constans LectorEmendatio* e;

    si (!x || k >= xar_numerus(x))
    {
        redde FALSUM;
    }
    e = (constans LectorEmendatio*)xar_obtinere(x, k);
    redde e->genus == genus && e->caput == caput
        && e->positio == positio
        && chorda_aequalis_literis(e->vestigium, vestigium);
}

interior vacuum
_sectio_emendationes (
    CredoContextus* c)
{
                 Piscina* p;
     InternamentumChorda* in;
                 Volumen* vol;
       LectorBibliotheca* b;
       LectorBibliotheca* sine;
                  chorda  id;
                  chorda  gutenberg;
                     Xar* e;
                     Xar* f;
                     Xar* x;
           LectorPositio  q;
                     b32  inventum;

    p    = c->piscina;
    in   = internamentum_creare(p);
    vol  = volumen_temporarium(p, "lector_emendationes");
    CREDO_NECESSE_NON_NIHIL(vol);
    b          = lector_bibliotheca_aperire(BIBLIOTHECA, vol, p, in);
    id         = chorda_ex_literis("urn:rhubarb:alpha", p);
    gutenberg  = chorda_ex_literis("http://www.gutenberg.org/394", p);

    CREDO_GRADUS("nullae: Xar vacuum");
    x = lector_emendationes_legere(b, id, p);
    CREDO_NECESSE_NON_NIHIL(x);
    CREDO_AEQUALIS_I32(xar_numerus(x), ZEPHYRUM);
    x = lector_favores_legere(b, id, p);
    CREDO_NECESSE_NON_NIHIL(x);
    CREDO_AEQUALIS_I32(xar_numerus(x), ZEPHYRUM);

    CREDO_GRADUS("III emendationes ordine, genus, vestigium cum '\"' "
        "'<' "
        "'&'");
    e = xar_creare(p, (i32)magnitudo(LectorEmendatio));
    *(LectorEmendatio*)xar_addere(e) = emendatio(CODEX_IUNGERE, III,
        XIX,
        "Primum hic.", p);
    *(LectorEmendatio*)xar_addere(e) = emendatio(CODEX_FINDERE, III, XL,
        "ibi.", p);
    *(LectorEmendatio*)xar_addere(e) = emendatio(CODEX_IUNGERE, I,
        ZEPHYRUM, "Dixit \"x\" <y> & z", p);
    CREDO_VERUM(lector_emendationes_scribere(b, id, e));
    x = lector_emendationes_legere(b, id, p);
    CREDO_AEQUALIS_I32(xar_numerus(x), III);
    CREDO_VERUM(emendatio_est(x, ZEPHYRUM, CODEX_IUNGERE, III, XIX,
        "Primum hic."));
    CREDO_VERUM(emendatio_est(x, I, CODEX_FINDERE, III, XL, "ibi."));
    CREDO_VERUM(emendatio_est(x, II, CODEX_IUNGERE, I, ZEPHYRUM,
        "Dixit \"x\" <y> & z"));

    CREDO_GRADUS("bibliotheca altera eodem volumine easdem legit");
    x = lector_emendationes_legere(lector_bibliotheca_aperire(
        BIBLIOTHECA, vol, p, in), id, p);
    CREDO_AEQUALIS_I32(xar_numerus(x), III);

    CREDO_GRADUS("iterum: ultima vincit; lista vacua licet");
    e = xar_creare(p, (i32)magnitudo(LectorEmendatio));
    *(LectorEmendatio*)xar_addere(e) = emendatio(CODEX_FINDERE, II, V,
        "Secundum", p);
    CREDO_VERUM(lector_emendationes_scribere(b, id, e));
    x = lector_emendationes_legere(b, id, p);
    CREDO_AEQUALIS_I32(xar_numerus(x), I);
    CREDO_VERUM(emendatio_est(x, ZEPHYRUM, CODEX_FINDERE, II, V,
        "Secundum"));
    CREDO_VERUM(lector_emendationes_scribere(b, id, xar_creare(p,
        (i32)magnitudo(LectorEmendatio))));
    CREDO_AEQUALIS_I32(xar_numerus(lector_emendationes_legere(b, id,
        p)),
        ZEPHYRUM);

    CREDO_GRADUS("favores: II ordine");
    f = xar_creare(p, (i32)magnitudo(LectorPositio));
    q.caput = III;
    q.initium = XIX;
    q.vestigium = chorda_ex_literis("Primum hic.", p);
    *(LectorPositio*)xar_addere(f) = q;
    q.caput = I;
    q.initium = ZEPHYRUM;
    q.vestigium = chorda_ex_literis("Caput I", p);
    *(LectorPositio*)xar_addere(f) = q;
    CREDO_VERUM(lector_favores_scribere(b, id, f));
    x = lector_favores_legere(b, id, p);
    CREDO_AEQUALIS_I32(xar_numerus(x), II);
    CREDO_AEQUALIS_I32(((LectorPositio*)xar_obtinere(x,
        ZEPHYRUM))->caput,
        III);
    CREDO_AEQUALIS_I32(((LectorPositio*)xar_obtinere(x,
        ZEPHYRUM))->initium, XIX);
    CREDO_CHORDA_AEQUALIS_LITERIS(((LectorPositio*)xar_obtinere(x,
        I))->vestigium, "Caput I");

    CREDO_GRADUS("claves %XX (identitas Gutenberg) pro utroque genere");
    CREDO_VERUM(lector_emendationes_scribere(b, gutenberg, e));
    CREDO_VERUM(lector_favores_scribere(b, gutenberg, f));
    inventum = FALSUM;
    (vacuum)volumen_plagulam_promere(vol, chorda_ex_literis(
        "lector/emendationes/http%3A%2F%2Fwww.gutenberg.org%2F394", p),
        p, &inventum);
    CREDO_VERUM(inventum);
    inventum = FALSUM;
    (vacuum)volumen_plagulam_promere(vol, chorda_ex_literis(
        "lector/favores/http%3A%2F%2Fwww.gutenberg.org%2F394", p), p,
        &inventum);
    CREDO_VERUM(inventum);

    CREDO_GRADUS("sine volumine: NIHIL et FALSUM");
    sine = lector_bibliotheca_aperire(BIBLIOTHECA, NIHIL, p, in);
    CREDO_NIHIL(lector_emendationes_legere(sine, id, p));
    CREDO_FALSUM(lector_emendationes_scribere(sine, id, e));
    CREDO_NIHIL(lector_favores_legere(sine, id, p));
    CREDO_FALSUM(lector_favores_scribere(sine, id, f));

    volumen_claudere(vol);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "bibliotheca", _sectio_bibliotheca, NIHIL, NIHIL, NIHIL, NIHIL },
    { "positio", _sectio_positio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "longitudines", _sectio_longitudines, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "emendationes", _sectio_emendationes, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("lector_bibliotheca", SECTIONES);
}
