/* probatio_xml_totalitas.c - Porta totalitatis xml (planum P14)
 *
 * CONTRACTUS materiae: (a) NUMQUAM RUIT pro quolibet fonte; (b) SEMPER
 * arborem reddit, et emissio == fons octetim; xml addit (c) coctio
 * semper finitur, et documentum non bene formatum diagnosticum saltem
 * unum fert (bene formatum nullum erratum).
 * Classes: octeti fortuiti (semina I..LXIV, alphabetum XML), corpus W3C
 * mutatum (octetus mutatus III locis) et truncatum (VII gradus), nidi
 * (elementa C milia, entitates LXX catenatae, exemplar contenti CC
 * parenthesium, attributa V milia), NUL et characteres moderatores.
 * Suite absente: classis corporis OMITTITUR CUM NUMERO.
 */

#include "latina.h"
#include "credo.h"
#include "xml_arbor.h"
#include "xml_coctum.h"
#include "xml_registrum.h"
#include "xml_corpus_ambulare.h"
#include "materia_scribere.h"
#include "piscina.h"
#include <stdio.h>
#include <string.h>

nomen structura {
    i32 examinati;
    i32 fracti;     /* lex octetorum aut contractus (c) fractus */
    i32 bene;
} Summa;

/* (b) + (c) pro fonte uno; FALSUM si contractus fractus */
interior b32
_contractus (
               Piscina* p,
    constans character* fons,
                   s32  mensura,
                   b32* bene)
{
                 MateriaNodus* radix;
             MateriaScriptura  emissa;
    MateriaScripturaConsilium  consilium;
                   XmlParsura  r;
                    XmlCoctum  k;

    radix = xml_arbor_parsare(p, fons, mensura, &r);
    si (radix == NIHIL)
    {
        redde FALSUM;
    }
    materia_scriptura_consilium_nudum(&consilium, &XML_REGISTRUM);
    emissa = materia_scribere_nodum(p, radix, &consilium);
    si (   !emissa.successus || emissa.textus.mensura != (i32)mensura
        || (mensura > ZEPHYRUM
            && memcmp(emissa.textus.datum, fons, (size_t)mensura)
                != ZEPHYRUM))
    {
        redde FALSUM;
    }
    k = xml_coquere(p, fons, mensura, VERUM);
    si (k.diagnostica == NIHIL)
    {
        redde FALSUM;
    }
    *bene = k.bene_formatum;
    si (!k.bene_formatum && xar_numerus(k.diagnostica) == ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (k.bene_formatum && k.documentum == NIHIL)
    {
        redde FALSUM;
    }
    redde VERUM;
}

interior vacuum
_casus (
                 Summa* s,
               Piscina* p,
    constans character* fons,
                   s32  mensura,
    constans character* titulus)
{
    b32 bene = FALSUM;

    s->examinati++;
    si (!_contractus(p, fons, mensura, &bene))
    {
        s->fracti++;
        si (s->fracti <= (i32)X)
        {
            imprimere("    CONTRACTUS FRACTUS: %s (%d octeti)\n",
                titulus,
                (integer)mensura);
        }
    }
    si (bene)
    {
        s->bene++;
    }
}

hic_manens constans character ALPHABETUM[] =
    "<<<>>>&&;;//==\"\"''!![]?\?--%%##xaAbe:_ \n\t\r()|,*+#PCDATA";

interior vacuum
_fortuiti_probare (
    CredoContextus* c)
{
      Summa  s;
        i32  semen;
    Piscina* opus = piscina_generare_dynamicum("xml_totalitas",
                        MXXIV * LXIV);

    (vacuum)c;
    memset(&s, ZEPHYRUM, magnitudo(s));
    per (semen = I; semen <= (i32)LXIV && opus != NIHIL; semen++)
    {
        insignatus longus x  = (insignatus longus)semen * 2654435761UL;
                      i32 n   = (semen * XXXVII) % MMXLVIII
                          + I;
                character* b = (character*)piscina_allocare(opus,
                    (memoriae_index)n + I);
        i32 k;

        per (k = ZEPHYRUM; b != NIHIL && k < n; k++)
        {
            x = x * 1103515245UL + 12345UL;
            si ((x >> XVI) % X == ZEPHYRUM)
            {
                b[k] = (character)((x >> VIII) & 0xFF);
            }
            alioquin
            {
                b[k] = ALPHABETUM[(x >> XVI) % (magnitudo(ALPHABETUM)
                    - I)];
            }
        }
        si (b != NIHIL)
        {
            _casus(&s, opus, b, (s32)n, "fortuitus");
        }
        piscina_vacare(opus);
    }
    imprimere("  fortuiti %u, fracti %u, bene formati %u\n",
        (insignatus integer)s.examinati, (insignatus integer)s.fracti,
        (insignatus integer)s.bene);
    CREDO_AEQUALIS_I32 (s.fracti, ZEPHYRUM);
    si (opus != NIHIL)
    {
        piscina_destruere(opus);
    }
}

nomen structura {
    Summa s;
} StatusCorporis;

interior vacuum
_visor (
                       vacuum* datum,
                XmlCorpusFons  fons,
           constans character* via,
    constans XmlCasusSelectus* selectus,
                       chorda  textus,
                      Piscina* opus)
{
    StatusCorporis* st = (StatusCorporis*)datum;
         character* b;
               i32  g;

    (vacuum)fons;
    (vacuum)selectus;
    si (textus.mensura == ZEPHYRUM)
    {
        redde;
    }
    b = (character*)piscina_allocare(opus,
        (memoriae_index)textus.mensura
        + I);
    si (b == NIHIL)
    {
        redde;
    }
    /* truncati VII gradus */
    per (g = I; g < (i32)VIII; g++)
    {
        _casus(&st->s, opus, (constans character*)textus.datum,
            (s32)(textus.mensura * g / VIII), via);
    }
    /* octetus mutatus III locis: '<', '&', 0xFF */
    per (g = ZEPHYRUM; g < (i32)III; g++)
    {
        constans character mut[III] = { '<', '&', (character)0xFF };

        memcpy(b, textus.datum, (size_t)textus.mensura);
        b[(textus.mensura * (g + I)) / IV] = mut[g];
        _casus(&st->s, opus, b, (s32)textus.mensura, via);
    }
}

interior vacuum
_corpus_probare (
    CredoContextus* c)
{
     StatusCorporis  st;
    XmlCorpusNumeri  nn;
            Piscina* opus;

    opus = piscina_generare_dynamicum("xml_totalitas_c", MXXIV * CCLVI);
    memset(&st, ZEPHYRUM, magnitudo(st));
    CREDO_NON_NIHIL (opus);
    si (opus == NIHIL)
    {
        redde;
    }
    xml_corpus_ambulare(c->piscina, opus, xml_corpus_radix(),
        (i32)(I << XML_CORPUS_XMLTS), _visor, &st, &nn);
    si (nn.xmlts_absens)
    {
        imprimere("  OMISSA: suite W3C abest"
            " (./xml/xmlts_arcessere.sh)\n");
    }
    imprimere("  corpus mutatum: casus %u, fracti %u, bene formati"
        " %u\n",
        (insignatus integer)st.s.examinati,
        (insignatus integer)st.s.fracti,
        (insignatus integer)st.s.bene);
    CREDO_AEQUALIS_I32 (st.s.fracti, ZEPHYRUM);
    piscina_destruere(opus);
}

/* fons ex partibus repetitis: caput + medium x n + cauda */
interior character*
_repetere (
               Piscina* p,
    constans character* caput,
    constans character* medium,
                   i32  n,
    constans character* cauda,
                   s32* mensura)
{
          i32  longitudo_capitis  = (i32)strlen(caput);
          i32  longitudo_medii    = (i32)strlen(medium);
          i32  longitudo_caudae   = (i32)strlen(cauda);
    character* b = (character*)piscina_allocare(p,
        (memoriae_index)(longitudo_capitis + longitudo_medii * n
            + longitudo_caudae + I));
    i32 k;
    i32 m = ZEPHYRUM;

    si (b == NIHIL)
    {
        redde NIHIL;
    }
    memcpy(b, caput, (size_t)longitudo_capitis);
    m += longitudo_capitis;
    per (k = ZEPHYRUM; k < n; k++)
    {
        memcpy(b + m, medium, (size_t)longitudo_medii);
        m += longitudo_medii;
    }
    memcpy(b + m, cauda, (size_t)longitudo_caudae);
    m         += longitudo_caudae;
    b[m]      = '\0';
    *mensura  = (s32)m;
    redde b;
}

interior vacuum
_nidi_probare (
    CredoContextus* c)
{
        Summa  s;
          s32  m;
    character* b;
    character  catena[IV * MXXIV];
          i32  k;

    memset(&s, ZEPHYRUM, magnitudo(s));
    /* elementa C milia aperta (finis fontis) */
    b = _repetere(c->piscina, "", "<a>", (i32)C * M, "", &m);
    si (b != NIHIL)
    {
        b32 bene;

        s.examinati++;
        si (xml_arbor_parsare(c->piscina, b, m, NIHIL) == NIHIL)
        {
            s.fracti++;
        }
        (vacuum)bene;
    }
    /* exemplar contenti CC parenthesium */
    b = _repetere(c->piscina, "<!DOCTYPE a [<!ELEMENT a ", "(", CC,
        "b)>]><a/>", &m);
    si (b != NIHIL)
    {
        _casus(&s, c->piscina, b, m, "parentheses");
    }
    /* attributa V milia (nomina diversa) */
    {
        character* a = (character*)piscina_allocare(c->piscina,
            (memoriae_index)V * M * XVI + XVI);
        i32 n = ZEPHYRUM;

        si (a != NIHIL)
        {
            n += (i32)sprintf(a + n, "<e");
            per (k = ZEPHYRUM; k < (i32)V * M; k++)
            {
                n += (i32)sprintf(a + n, " a%u='v'",
                    (insignatus integer)k);
            }
            n += (i32)sprintf(a + n, "/>");
            _casus(&s, c->piscina, a, (s32)n, "attributa");
        }
    }
    /* entitates LXX catenatae (e0 -> e1 -> ...): limes profunditatis */
    {
        i32 n = ZEPHYRUM;

        n += (i32)sprintf(catena + n, "<!DOCTYPE r [");
        per (k = ZEPHYRUM; k < (i32)LXX; k++)
        {
            n += (i32)sprintf(catena + n, "<!ENTITY e%u '&e%u;'>",
                (insignatus integer)k, (insignatus integer)(k + I));
        }
        n += (i32)sprintf(catena + n,
            "<!ENTITY e70 'finis'>]><r>&e0;</r>");
        _casus(&s, c->piscina, catena, (s32)n, "catena entitatum");
    }
    /* NUL et moderatores */
    _casus(&s, c->piscina, "<a>\0</a>", VIII, "NUL in textu");
    _casus(&s, c->piscina, "<a b='\0'/>", X, "NUL in valore");
    _casus(&s, c->piscina, "<a>\x01\x1b</a>", IX, "moderatores");
    imprimere("  nidi et extrema: casus %u, fracti %u\n",
        (insignatus integer)s.examinati, (insignatus integer)s.fracti);
    CREDO_AEQUALIS_I32 (s.fracti, ZEPHYRUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "octeti fortuiti", _fortuiti_probare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "corpus W3C mutatum et truncatum", _corpus_probare, NIHIL, NIHIL,
        "lentus", NIHIL },
    { "nidi et extrema", _nidi_probare, NIHIL, NIHIL, "gravis", NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("xml_totalitas", SECTIONES, NIHIL,
        NIHIL);
}
