/* oratio_vocabularium_en.c - Vide oratio_vocabularium_en.h. */

#include "oratio_vocabularium_en.h"
#include "tabula_dispersa.h"
#include <string.h>

constans character* constans ORATIO_VOCABULARIUM_EN_CODICES =
    "NphVtiAvCP!rDIo";

/* nodus catenae: index recordi; proximus -I = finis */
nomen structura {
    s32 index;
    s32 proximus;
} Nodus;

/* caput catenae in tabula dispersa (valor = indicator ad Caput) */
nomen structura {
    s32 primus;
} Caput;

structura OratioVocabulariumEn {
                     Piscina* piscina;
                         Xar* recorda;      /* OratioVocabulumEn */
                         Xar* nodi;         /* Nodus */
              TabulaDispersa* per_formam;   /* forma plicata -> Caput */
    OratioVocabulariumEnCensus  census;
};

interior chorda
_chorda (
     i8* datum,
    i32  mensura)
{
    chorda c;

    c.datum    = datum;
    c.mensura  = mensura;
    redde c;
}

interior b32
_codex_legendae (
    i8 c)
{
    constans character* l = ORATIO_VOCABULARIUM_EN_CODICES;

    dum (*l != '\0')
    {
        si ((i8)*l == c)
        {
            redde VERUM;
        }
        l = l + I;
    }
    redde FALSUM;
}

interior vacuum
_vitium (
    OratioVocabulariumVitium* v,
                         i32  linea,
          constans character* causa)
{
    si (v != NIHIL)
    {
        v->plagula  = "mobypos.txt";
        v->linea    = linea;
        v->offset   = (s32)-I;
        v->causa    = causa;
    }
}

interior b32
_catenare (
    OratioVocabulariumEn* voc,
                  chorda  clavis,
                     s32  index)
{
    vacuum* valor = NIHIL;
     Caput* caput;
     Nodus* nodus;
       s32  n = (s32)xar_numerus(voc->nodi);

    nodus = (Nodus*)xar_addere(voc->nodi);
    si (nodus == NIHIL)
    {
        redde FALSUM;
    }
    nodus->index = index;
    si (tabula_dispersa_invenire(voc->per_formam, clavis, &valor))
    {
        caput            = (Caput*)valor;
        nodus->proximus  = caput->primus;
        caput->primus    = n;
        redde VERUM;
    }
    caput = (Caput*)piscina_allocare(voc->piscina,
        (memoriae_index)magnitudo(Caput));
    si (caput == NIHIL)
    {
        redde FALSUM;
    }
    caput->primus    = n;
    nodus->proximus  = (s32)-I;
    redde tabula_dispersa_inserere(voc->per_formam, clavis, caput);
}

chorda
oratio_vocabularium_en_plicare (
    Piscina* piscina,
     chorda  forma)
{
    i8* datum = (i8*)piscina_allocare(piscina,
        (memoriae_index)forma.mensura + I);
    i32 i;

    si (datum == NIHIL)
    {
        redde _chorda(NIHIL, ZEPHYRUM);
    }
    per (i = ZEPHYRUM; i < forma.mensura; i++)
    {
        i8 c = forma.datum[i];

        si (c >= 'A' && c <= 'Z')
        {
            c = (i8)(c - 'A' + 'a');
        }
        datum[i] = c;
    }
    datum[forma.mensura] = ZEPHYRUM;
    redde _chorda(datum, forma.mensura);
}

OratioVocabulariumEn*
oratio_vocabularium_en_onerare (
                     Piscina* piscina,
                      chorda  fons,
    OratioVocabulariumVitium* vitium)
{
    OratioVocabulariumEn* voc = (OratioVocabulariumEn*)piscina_allocare(
        piscina, (memoriae_index)magnitudo(OratioVocabulariumEn));
    i32 cursor  = ZEPHYRUM;
    i32 linea   = ZEPHYRUM;

    si (voc == NIHIL)
    {
        redde NIHIL;
    }
    memset(voc, ZEPHYRUM, magnitudo(*voc));
    voc->piscina = piscina;
    voc->recorda = xar_creare(piscina,
        (i32)magnitudo(OratioVocabulumEn));
    voc->nodi = xar_creare(piscina, (i32)magnitudo(Nodus));
    voc->per_formam = tabula_dispersa_creare_chorda(piscina,
        (i32)524288);
    si (   voc->recorda    == NIHIL || voc->nodi == NIHIL
        || voc->per_formam == NIHIL)
    {
        redde NIHIL;
    }
    dum (cursor < fons.mensura)
    {
                      i32  a        = cursor;
                      i32  b        = a;
                      s32  signum   = (s32)-I;
                      i32  signa    = ZEPHYRUM;
                      b32  spatium  = FALSUM;
                      b32  altus    = FALSUM;
                   chorda  forma;
                   chorda  codices;
        OratioVocabulumEn* r;
                      i32  k;

        linea = linea + I;
        dum (b < fons.mensura && fons.datum[b] != '\n')
        {
            si (fons.datum[b] == '\\')
            {
                signa = signa + I;
                si (signum < ZEPHYRUM)
                {
                    signum = (s32)b;
                }
            }
            b = b + I;
        }
        si (b >= fons.mensura)
        {
            _vitium(vitium, linea, "linea ultima sine CRLF");
            redde NIHIL;
        }
        si (b == a || fons.datum[b - I] != '\r')
        {
            _vitium(vitium, linea, "finis lineae non CRLF");
            redde NIHIL;
        }
        si (signa == ZEPHYRUM)
        {
            _vitium(vitium, linea, "signum '\\' deest");
            redde NIHIL;
        }
        si (signa > I)
        {
            _vitium(vitium, linea, "signa '\\' plura");
            redde NIHIL;
        }
        forma    = _chorda(fons.datum + a, (i32)signum - a);
        codices  = _chorda(fons.datum + signum + I,
            (b - I) - ((i32)signum + I));
        si (forma.mensura == ZEPHYRUM)
        {
            _vitium(vitium, linea, "forma vacua");
            redde NIHIL;
        }
        si (codices.mensura == ZEPHYRUM)
        {
            _vitium(vitium, linea, "codices vacui");
            redde NIHIL;
        }
        per (k = ZEPHYRUM; k < forma.mensura; k++)
        {
            si (forma.datum[k] == ' ')
            {
                spatium = VERUM;
            }
            si (forma.datum[k] >= 0x80)
            {
                altus = VERUM;
            }
        }
        per (k = ZEPHYRUM; k < codices.mensura; k++)
        {
            si (!_codex_legendae(codices.datum[k]))
            {
                voc->census.codices_ignoti = voc->census.codices_ignoti
                    + I;
                si (voc->census.codex_ignotus_primus.mensura
                    == ZEPHYRUM)
                {
                    voc->census.codex_ignotus_primus = forma;
                }
                frange;
            }
        }
        r = (OratioVocabulumEn*)xar_addere(voc->recorda);
        si (r == NIHIL)
        {
            redde NIHIL;
        }
        r->forma             = forma;
        r->codices           = codices;
        r->linea             = linea;
        voc->census.recorda  = voc->census.recorda + I;
        si (altus)
        {
            voc->census.octeti_alti = voc->census.octeti_alti + I;
        }
        si (spatium)
        {
            voc->census.locutiones = voc->census.locutiones + I;
        }
        alioquin
        {
            chorda clavis = oratio_vocabularium_en_plicare(piscina,
                forma);

            voc->census.verba = voc->census.verba + I;
            si (   clavis.datum == NIHIL
                || !_catenare(voc, clavis,
                    (s32)xar_numerus(voc->recorda) - I))
            {
                redde NIHIL;
            }
        }
        cursor = b + I;
    }
    redde voc;
}

Xar*
oratio_vocabularium_en_quaerere (
                          Piscina* piscina,
    constans OratioVocabulariumEn* voc,
                           chorda  forma)
{
       Xar* exitus  = xar_creare(piscina, (i32)magnitudo(s32));
    vacuum* valor   = NIHIL;
    chorda  clavis;
       s32  n;
       i32  i;
       i32  j;

    si (exitus == NIHIL)
    {
        redde NIHIL;
    }
    clavis = oratio_vocabularium_en_plicare(piscina, forma);
    si (   clavis.datum == NIHIL
        || !tabula_dispersa_invenire(voc->per_formam, clavis, &valor))
    {
        redde exitus;
    }
    /* catena recentissimum primum fert: colligere, deinde invertere ut
     * ordo plagulae maneat */
    n = ((Caput*)valor)->primus;
    dum (n >= ZEPHYRUM)
    {
        constans Nodus* nodus = (constans Nodus*)xar_obtinere(voc->nodi,
            (i32)n);
        s32* locus = (s32*)xar_addere(exitus);

        si (locus == NIHIL)
        {
            redde NIHIL;
        }
        *locus  = nodus->index;
        n       = nodus->proximus;
    }
    i = ZEPHYRUM;
    j = xar_numerus(exitus) - I;
    dum (i < j)
    {
        s32* a = (s32*)xar_obtinere(exitus, i);
        s32* b = (s32*)xar_obtinere(exitus, j);
        s32  t = *a;

        *a  = *b;
        *b  = t;
        i   = i + I;
        j   = j - I;
    }
    redde exitus;
}

constans OratioVocabulumEn*
oratio_vocabularium_en_recordum (
    constans OratioVocabulariumEn* voc,
                              s32  i)
{
    redde (constans OratioVocabulumEn*)xar_obtinere(voc->recorda,
        (i32)i);
}

OratioVocabulariumEnCensus
oratio_vocabularium_en_census (
    constans OratioVocabulariumEn* voc)
{
    redde voc->census;
}

constans character*
oratio_vocabularium_en_classis (
    i8 codex)
{
    commutatio (codex)
    {
        casus 'N': casus 'p': casus 'h': casus 'o':
            redde "substantivum";
        casus 'V': casus 't': casus 'i':
            redde "verbum";
        casus 'A':
            redde "adiectivum";
        casus 'v':
            redde "adverbium";
        casus 'C':
            redde "coniunctio-coordinans";
        casus 'P':
            redde "adpositio";
        casus '!':
            redde "interiectio";
        casus 'r':
            redde "pronomen";
        casus 'D': casus 'I':
            redde "determinans";
        ordinarius:
            redde NIHIL;
    }
}
