/* oratio_glossarium.c - Vide oratio_glossarium.h. */

#include "oratio_glossarium.h"
#include "oratio_vocabularium_la.h"
#include "stml.h"
#include "internamentum.h"
#include "tabula_dispersa.h"
#include <string.h>

nomen structura {
    s32 forma;
    s32 proximus;
} Nodus;

nomen structura {
    s32 primus;
} Caput;

structura OratioGlossarium {
               Xar* entria;           /* OratioGlossariumEntrium */
               Xar* formae;           /* OratioGlossariumForma */
               Xar* nodi;             /* Nodus */
    TabulaDispersa* per_formam;   /* forma plicata -> Caput */
           Piscina* piscina;
};

interior chorda
_vacua (vacuum)
{
    chorda c;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    redde c;
}

interior chorda
_attributum (
             StmlNodus* n,
    constans character* titulus)
{
    chorda* v = stml_attributum_capere(n, titulus);

    redde v == NIHIL ? _vacua() : *v;
}

interior b32
_aequalis_literis (
                chorda  c,
    constans character* literae)
{
    i32 l = (i32)strlen(literae);

    redde (b32)(c.mensura == l
        && (l == ZEPHYRUM
        || memcmp(c.datum, literae, (size_t)l) == ZEPHYRUM));
}

interior vacuum
_vitium (
    OratioVocabulariumVitium* v,
                         i32  linea,
          constans character* causa)
{
    si (v != NIHIL)
    {
        v->plagula  = "glossarium";
        v->linea    = linea;
        v->offset   = ZEPHYRUM;
        v->causa    = causa;
    }
}

interior b32
_catenare (
    OratioGlossarium* gl,
              chorda  clavis,
                 s32  forma)
{
    vacuum* valor = NIHIL;
     Caput* caput;
     Nodus* nodus;
       s32  n = (s32)xar_numerus(gl->nodi);

    nodus = (Nodus*)xar_addere(gl->nodi);
    si (nodus == NIHIL)
    {
        redde FALSUM;
    }
    nodus->forma = forma;
    si (tabula_dispersa_invenire(gl->per_formam, clavis, &valor))
    {
        caput            = (Caput*)valor;
        nodus->proximus  = caput->primus;
        caput->primus    = n;
        redde VERUM;
    }
    caput = (Caput*)piscina_allocare(gl->piscina,
        (memoriae_index)magnitudo(Caput));
    si (caput == NIHIL)
    {
        redde FALSUM;
    }
    caput->primus    = n;
    nodus->proximus  = (s32)-I;
    redde tabula_dispersa_inserere(gl->per_formam, clavis, caput);
}

/* formam addere; textus plicatus in tabulam */
interior b32
_formam_addere (
    OratioGlossarium* gl,
                 s32  entrium,
              chorda  textus,
           StmlNodus* nodus_formae,
                 i32  linea)
{
    OratioGlossariumForma* f =
        (OratioGlossariumForma*)xar_addere(gl->formae);
                      s32 index = (s32)xar_numerus(gl->formae) - I;

    si (f == NIHIL)
    {
        redde FALSUM;
    }
    memset(f, ZEPHYRUM, magnitudo(*f));
    f->entrium  = entrium;
    f->textus   = textus;
    f->linea    = linea;
    si (nodus_formae != NIHIL)
    {
        f->casus_grammaticus = _attributum(nodus_formae, "casus");
        f->numerus = _attributum(nodus_formae, "numerus");
        f->genus = _attributum(nodus_formae, "genus");
        f->persona = _attributum(nodus_formae, "persona");
        f->tempus = _attributum(nodus_formae, "tempus");
        f->modus = _attributum(nodus_formae, "modus");
        f->vox = _attributum(nodus_formae, "vox");
        f->forma_verbi = _attributum(nodus_formae, "forma-verbi");
        f->gradus = _attributum(nodus_formae, "gradus");
        f->nota = _attributum(nodus_formae, "nota");
    }
    redde _catenare(gl, oratio_vocabularium_la_plicare(gl->piscina,
        textus),
        index);
}

OratioGlossarium*
oratio_glossarium_legere (
                     Piscina* piscina,
                      chorda  fons,
    OratioVocabulariumVitium* vitium)
{
       OratioGlossarium* gl;
    InternamentumChorda* intern = internamentum_creare(piscina);
           StmlResultus  res;
              StmlNodus* radix;
                    i32  i;

    _vitium(vitium, ZEPHYRUM, "");
    si (intern == NIHIL)
    {
        redde NIHIL;
    }
    res = stml_legere(fons, piscina, intern);
    si (!res.successus || res.elementum_radix == NIHIL)
    {
        _vitium(vitium, ZEPHYRUM, "STML non legitur");
        redde NIHIL;
    }
    radix = res.elementum_radix;
    si (   radix->titulus == NIHIL
        || !_aequalis_literis(*radix->titulus, "glossarium"))
    {
        _vitium(vitium, radix->linea, "radix <glossarium> exspectata");
        redde NIHIL;
    }
    gl = (OratioGlossarium*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(OratioGlossarium));
    si (gl == NIHIL)
    {
        redde NIHIL;
    }
    gl->piscina = piscina;
    gl->entria = xar_creare(piscina,
        (i32)magnitudo(OratioGlossariumEntrium));
    gl->formae = xar_creare(piscina,
        (i32)magnitudo(OratioGlossariumForma));
    gl->nodi        = xar_creare(piscina, (i32)magnitudo(Nodus));
    gl->per_formam  = tabula_dispersa_creare_chorda(piscina, (i32)1024);
    si (   gl->entria     == NIHIL || gl->formae == NIHIL
        || gl->nodi       == NIHIL
        || gl->per_formam == NIHIL)
    {
        redde NIHIL;
    }
    si (radix->liberi == NIHIL)
    {
        redde gl;
    }
    per (i = ZEPHYRUM; i < xar_numerus(radix->liberi); i++)
    {
        StmlNodus* v = *(StmlNodus**)xar_obtinere(radix->liberi, i);
        OratioGlossariumEntrium* e;
        s32 index;
        b32 forma_ulla = FALSUM;
        i32 k;

        si (v->genus != STML_NODUS_ELEMENTUM || v->titulus == NIHIL)
        {
            perge;
        }
        si (!_aequalis_literis(*v->titulus, "vocabulum"))
        {
            _vitium(vitium, v->linea,
                "liberum glossarii non <vocabulum>");
            redde NIHIL;
        }
        e      = (OratioGlossariumEntrium*)xar_addere(gl->entria);
        index  = (s32)xar_numerus(gl->entria) - I;
        si (e == NIHIL)
        {
            redde NIHIL;
        }
        memset(e, ZEPHYRUM, magnitudo(*e));
        e->lemma    = _attributum(v, "lemma");
        e->lingua   = _attributum(v, "lingua");
        e->classis  = _attributum(v, "classis");
        e->nota     = _attributum(v, "nota");
        e->linea    = v->linea;
        si (   e->lemma.mensura   == ZEPHYRUM
            || e->lingua.mensura  == ZEPHYRUM
            || e->classis.mensura == ZEPHYRUM)
        {
            _vitium(vitium, v->linea,
                "vocabulum sine lemma/lingua/classis");
            redde NIHIL;
        }
                e->permissum = _aequalis_literis(e->classis,
                    "ignotum-permissum");
        e->contextus = _attributum(v, "contextus");
        e->latine  = (b32)(e->contextus.mensura == ZEPHYRUM
            || _aequalis_literis(e->contextus, "latinus")
            || _aequalis_literis(e->contextus, "ambo"));
        e->anglice = (b32)(e->contextus.mensura == ZEPHYRUM
            || _aequalis_literis(e->contextus, "anglicus")
            || _aequalis_literis(e->contextus, "ambo"));

        e->formae_ab = xar_numerus(gl->formae);
        si (v->liberi != NIHIL)
        {
            per (k = ZEPHYRUM; k < xar_numerus(v->liberi); k++)
            {
                StmlNodus* l = *(StmlNodus**)xar_obtinere(v->liberi, k);

                si (   l->genus   != STML_NODUS_ELEMENTUM
                    || l->titulus == NIHIL)
                {
                    perge;
                }
                si (_aequalis_literis(*l->titulus, "sensus"))
                {
                    e->sensus = stml_textus_normalizatus(l, piscina);
                    perge;
                }
                si (_aequalis_literis(*l->titulus, "forma"))
                {
                    chorda textus = _attributum(l, "textus");

                    si (textus.mensura == ZEPHYRUM)
                    {
                        _vitium(vitium, l->linea, "forma sine textu");
                        redde NIHIL;
                    }
                    si (!_formam_addere(gl, index, textus, l, l->linea))
                    {
                        redde NIHIL;
                    }
                    forma_ulla = VERUM;
                    perge;
                }
                _vitium(vitium, l->linea, "liberum vocabuli ignotum");
                redde NIHIL;
            }
        }
                /* lemma ipsum forma est nisi forma eiusdem textus (plicati) iam
         * data: 'offset' cum forma 'offsets' utrumque invenit; 'sum' cum
         * forma 'sum' semel */
        {
            chorda lemma_plicatum =
                oratio_vocabularium_la_plicare(piscina,
                e->lemma);
            b32 lemma_adest = FALSUM;

            per (k = e->formae_ab; k < xar_numerus(gl->formae); k++)
            {
                chorda t = oratio_vocabularium_la_plicare(piscina,
                    oratio_glossarium_forma(gl, (s32)k)->textus);

                si (   t.mensura == lemma_plicatum.mensura
                    && memcmp(t.datum, lemma_plicatum.datum,
                    (size_t)t.mensura)
                        == ZEPHYRUM)
                {
                    lemma_adest = VERUM;
                }
            }
            si (   !lemma_adest
                && !_formam_addere(gl, index, e->lemma, NIHIL,
                v->linea))
            {
                redde NIHIL;
            }
        }
        (vacuum)forma_ulla;

        /* e iterum capere: xar_addere formarum entria non movit, sed
         * cautela: index tenetur */
        e = (OratioGlossariumEntrium*)xar_obtinere(gl->entria,
            (i32)index);
        e->formae_numerus = xar_numerus(gl->formae) - e->formae_ab;
    }
    redde gl;
}

Xar*
oratio_glossarium_quaerere (
                      Piscina* piscina,
    constans OratioGlossarium* gl,
                       chorda  forma)
{
       Xar* exitus  = xar_creare(piscina, (i32)magnitudo(s32));
    vacuum* valor   = NIHIL;
    chorda  plicata;
       s32  n;

    si (exitus == NIHIL)
    {
        redde NIHIL;
    }
    plicata = oratio_vocabularium_la_plicare(piscina, forma);
    si (   plicata.mensura == ZEPHYRUM
        || !tabula_dispersa_invenire(gl->per_formam, plicata, &valor))
    {
        redde exitus;
    }
    /* catena praeponit: ordinem plagulae restituere (indices crescentes) */
    n = ((Caput*)valor)->primus;
    dum (n >= ZEPHYRUM)
    {
        constans Nodus* nd = (constans Nodus*)xar_obtinere(gl->nodi,
            (i32)n);
                   s32* locus = (s32*)xar_addere(exitus);

        si (locus == NIHIL)
        {
            redde NIHIL;
        }
        *locus  = nd->forma;
        n       = nd->proximus;
    }
    xar_invertere(exitus);
    redde exitus;
}

b32
oratio_glossarium_permissum (
                      Piscina* piscina,
    constans OratioGlossarium* gl,
                       chorda  forma,
                          b32  latine)
{

    Xar* x = oratio_glossarium_quaerere(piscina, gl, forma);
    i32  i;

    si (x == NIHIL)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(x); i++)
    {
        s32 f = *(s32*)xar_obtinere(x, i);

                constans OratioGlossariumEntrium* en =
                    oratio_glossarium_entrium(gl,
                    oratio_glossarium_forma(gl, f)->entrium);

        si (en->permissum && (latine ? en->latine : en->anglice))
        {
            redde VERUM;
        }

    }
    redde FALSUM;
}

constans OratioGlossariumEntrium*
oratio_glossarium_entrium (
    constans OratioGlossarium* gl,
                          s32  i)
{
    redde (constans OratioGlossariumEntrium*)xar_obtinere(gl->entria,
        (i32)i);
}

constans OratioGlossariumForma*
oratio_glossarium_forma (
    constans OratioGlossarium* gl,
                          s32  i)
{
    redde (constans OratioGlossariumForma*)xar_obtinere(gl->formae,
        (i32)i);
}

i32
oratio_glossarium_numerus_entriorum (
    constans OratioGlossarium* gl)
{
    redde xar_numerus(gl->entria);
}

i32
oratio_glossarium_numerus_formarum (
    constans OratioGlossarium* gl)
{
    redde xar_numerus(gl->formae);
}
