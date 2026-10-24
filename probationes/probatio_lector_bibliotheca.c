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

hic_manens constans CredoSectio SECTIONES[] = {
    { "bibliotheca", _sectio_bibliotheca, NIHIL, NIHIL, NIHIL, NIHIL },
    { "positio", _sectio_positio, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("lector_bibliotheca", SECTIONES);
}
