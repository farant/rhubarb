/* oratio_vocabularium_en.c - Vide oratio_vocabularium_en.h. */

#include "oratio_vocabularium_en.h"
#include "tabula_dispersa.h"
#include <string.h>

constans character* constans ORATIO_VOCABULARIUM_EN_CODICES =
    "NphVtiAvCP!rDIo";

/* Regulae morphologicae (T15b): ex relatione lintus prosae diei primi
 * (2026-09-04, LXII.DXXIV verba, 58 % ignota) - summa listae ignotorum
 * flexiones regulares erant quas Moby non fert. Ordines DATA: titulus,
 * suffixum, substitutio, geminatio, basis minima, codices basis, classis,
 * causa (verba relationis cum sedibus). */
constans OratioRegulaEn ORATIO_REGULAE_EN[] = {
        { "pluralis-s", "s", "", FALSUM, 2, "NphVti", NIHIL,
      "values MMLXXVI, functions MDCXL, tests, cells, types, results, gets, sets - relatio 2026-09-04: verba III.CCCXXVIII, sedes CXXXVI.XXXII" },
    { "pluralis-es", "es", "", FALSUM, 3, "NphVti", NIHIL,
      "matches CCCXCIV, boxes, passes - relatio 2026-09-04: verba CCCLVIII, sedes V.DCCLXXXVII" },
    { "pluralis-ies", "ies", "y", FALSUM, 3, "NphVti", NIHIL,
      "entries, libraries, queries - relatio 2026-09-04: verba LXVI, sedes I.DCXVI" },
    { "praeteritum-ed", "ed", "", FALSUM, 3, "VtiN", "verbum",
      "added DCCCVIII, needed, tested, recorded, expected - relatio 2026-09-04: verba DCXLIV, sedes XVI.CCCLXXII" },
    { "praeteritum-ed-e", "ed", "e", FALSUM, 3, "VtiN", "verbum",
      "shared DCLXI, changed, used - relatio 2026-09-04: verba XCI, sedes I.DXCII" },
    { "praeteritum-ied", "ied", "y", FALSUM, 3, "VtiN", "verbum",
      "tried, applied, copied - relatio 2026-09-04: verba V, sedes CCVII" },
    { "praeteritum-ed-geminatum", "ed", "", VERUM, 3, "VtiN", "verbum",
      "planned, mapped, dropped - relatio 2026-09-04: verba XVII, sedes LXVII" },
    { "participium-ing", "ing", "", FALSUM, 3, "VtiN", "verbum",
      "existing DCCCLIV, matching, tracking, testing, interning - relatio 2026-09-04: verba DLXX, sedes XII.XCVII" },
    { "participium-ing-e", "ing", "e", FALSUM, 3, "VtiN", "verbum",
      "making, using, parsing - relatio 2026-09-04: verba LXVIII, sedes DCCCVI" },
    { "participium-ing-geminatum", "ing", "", VERUM, 3, "VtiN",
        "verbum",
      "planning, mapping, running - relatio 2026-09-04: verba XX, sedes CLXXXIX" },
    { "possessivum", "'s", "", FALSUM, 2, NIHIL, "substantivum",
      "pliny's DIV, casey's, fran's, dkc's - relatio 2026-09-04: verba I.CXXIV, sedes VIII.CMLI" },
    { "contractio-'re", "'re", "", FALSUM, 2, "r", "pronomen",
      "we're CXXXV, they're, you're - relatio 2026-09-04: nova" },
    { "contractio-'m", "'m", "", FALSUM, 1, "r", "pronomen",
      "i'm CCCXVIII (apostrophus typographica plicata) - relatio 2026-09-04: nova" },
    { "contractio-'ll", "'ll", "", FALSUM, 1, "rN", "pronomen",
      "it'll, we'll - relatio 2026-09-04: nova" },
    { "contractio-'ve", "'ve", "", FALSUM, 1, "r", "pronomen",
      "we've, i've - relatio 2026-09-04: nova" },
    { "contractio-'d", "'d", "", FALSUM, 1, "rN", "pronomen",
      "it'd, i'd - relatio 2026-09-04: nova" },
    { "contractio-n't", "n't", "", FALSUM, 2, "Vti", "verbum",
      "don't, isn't, doesn't (won't, can't manent: basis wo, ca) - relatio 2026-09-04: nova" },
    { "adverbium-ly", "ly", "", FALSUM, 3, "A", "adverbium",
      "computationally CDXXXII, structurally - relatio 2026-09-04: verba LXI, sedes I.CCCXLV" },
    { "adverbium-ly-le", "ly", "le", FALSUM, 3, "A", "adverbium",
      "simply, possibly - relatio 2026-09-04: verba VII, sedes LXVI" },
    { "adverbium-ily", "ily", "y", FALSUM, 3, "A", "adverbium",
      "happily, easily (Moby adverbia ipsa fert: 0 in corpore) - relatio 2026-09-04: verba 0, sedes 0" },
    { "comparativus-er", "er", "", FALSUM, 3, "Av", "adiectivum",
      "higher CCCLXXXVII, faster; nearer (near v) - relatio 2026-09-04: verba XLII, sedes I.DLXXXIII" },
    { "comparativus-er-e", "er", "e", FALSUM, 3, "Av", "adiectivum",
      "larger, simpler - relatio 2026-09-04: verba IV, sedes CXXXIII" },
    { "superlativus-est", "est", "", FALSUM, 3, "Av", "adiectivum",
      "highest, fastest; nearest CXXXI (near v) - relatio 2026-09-04: verba XXXII, sedes DCLVIII" },
    { "superlativus-est-e", "est", "e", FALSUM, 3, "Av", "adiectivum",
      "largest, simplest - relatio 2026-09-04: verba IV, sedes LXXV" },
    { "compositum", "-", "", FALSUM, 1, NIHIL, NIHIL,
      "a-plot CDXLVII, non-null CCCLXX (praefixum), consists-in (pars per regulam), well-formed - relatio 2026-09-04: verba XI.CCLVII, sedes XXXVII.DCXCIII" },
};

constans i32 ORATIO_REGULAE_EN_NUMERUS =
    (i32)(magnitudo(ORATIO_REGULAE_EN)
        / magnitudo(ORATIO_REGULAE_EN[0]));

/* Praefixa compositorum (DATA): pars ante hyphen quae verbum Moby non
 * est sed praefixum notum (non-null CCCLXX, non-semisimple CDXXXI,
 * multi-strand CLXXIII); pars una saltem verbum verum esse debet. */
constans character* constans ORATIO_PRAEFIXA_EN[] = {
    "non", "un", "re", "pre", "sub", "multi", "semi", "anti", "pseudo",
    "quasi", "inter", "intra", "meta", "auto", "micro", "macro", "mono",
    "poly", "tri", "post", "mid", "hyper", "hypo", "mis", "proto",
        "ultra",
    NIHIL
};

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
    i32 o = ZEPHYRUM;

    si (datum == NIHIL)
    {
        redde _chorda(NIHIL, ZEPHYRUM);
    }
    i = ZEPHYRUM;
    dum (i < forma.mensura)
    {
        i8 c = forma.datum[i];

        /* apostrophus typographica U+2019 (E2 80 99) -> ' (i'm, dkc's) */
        si (   c == 0xE2 && i + (i32)II < forma.mensura
            && forma.datum[i + I] == 0x80
            && forma.datum[i + (i32)II] == 0x99)
        {
            datum[o]  = '\'';
            o         = o + I;
            i         = i + (i32)III;
            perge;
        }
        si (c >= 'A' && c <= 'Z')
        {
            c = (i8)(c - 'A' + 'a');
        }
        datum[o]  = c;
        o         = o + I;
        i         = i + I;
    }
    datum[o] = ZEPHYRUM;
    redde _chorda(datum, o);
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

interior b32
_finitur (
                chorda  forma,
    constans character* suffixum)
{
    i32 l = (i32)strlen(suffixum);

    redde (b32)(forma.mensura >= l
        && memcmp(forma.datum + forma.mensura - l, suffixum, (size_t)l)
            == ZEPHYRUM);
}

interior b32
_consonans (
    i8 c)
{
    redde (b32)(c >= 'a' && c <= 'z' && c != 'a' && c != 'e' && c != 'i'
        && c != 'o' && c != 'u');
}

/* an recordum codicem unum ex 'codices' ferat (NIHIL = quilibet);
 * *codex = littera prima congruens */
interior b32
_codex_congruens (
    constans OratioVocabulumEn* r,
            constans character* codices,
                            i8* codex)
{
    i32 k;

    si (codices == NIHIL)
    {
        *codex = r->codices.datum[ZEPHYRUM];
        redde VERUM;
    }
    per (k = ZEPHYRUM; k < r->codices.mensura; k++)
    {
        si (strchr(codices, (integer)r->codices.datum[k]) != NIHIL)
        {
            *codex = r->codices.datum[k];
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior b32
_analysin_addere (
                   Xar* exitus,
                   s32  recordum,
                   s32  regula,
                chorda  basis,
    constans character* classis)
{
    OratioAnalysisEn* a = (OratioAnalysisEn*)xar_addere(exitus);

    si (a == NIHIL)
    {
        redde FALSUM;
    }
    a->recordum  = recordum;
    a->regula    = regula;
    a->basis     = basis;
    a->classis   = classis != NIHIL ? classis : "ignotum";
    redde VERUM;
}

/* recorda basis (plicatae) quae regulae congruunt appendere */
interior b32
_basin_quaerere (
                          Piscina* piscina,
    constans OratioVocabulariumEn* voc,
          constans OratioRegulaEn* regula,
                              s32  index_regulae,
                           chorda  basis,
                              Xar* exitus)
{
    Xar* recorda = oratio_vocabularium_en_quaerere(piscina, voc, basis);
    i32  k;

    si (recorda == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < xar_numerus(recorda); k++)
    {
        s32 ri = *(s32*)xar_obtinere(recorda, k);
        constans OratioVocabulumEn* r =
            oratio_vocabularium_en_recordum(voc,
            ri);
        i8 codex = 'N';

        si (!_codex_congruens(r, regula->codices, &codex))
        {
            perge;
        }
        si (!_analysin_addere(exitus, ri, index_regulae, basis,
                regula->classis != NIHIL ? regula->classis
                : oratio_vocabularium_en_classis(codex)))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior b32
_analysare_simplex (
                          Piscina* piscina,
    constans OratioVocabulariumEn* voc,
                           chorda  plicata,
                              Xar* exitus);

interior b32
_praefixum_est (
    chorda pars)
{
    i32 i;

    per (i = ZEPHYRUM; ORATIO_PRAEFIXA_EN[i] != NIHIL; i++)
    {
        si (   (i32)strlen(ORATIO_PRAEFIXA_EN[i]) == pars.mensura
            && memcmp(ORATIO_PRAEFIXA_EN[i], pars.datum,
                (size_t)pars.mensura) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* compositum: partes ad hyphen quaeque forma nota (exacta aut per
 * regulam suffixi) aut praefixum tabulae; pars una saltem verbum;
 * classis = partis ultimae notae */
interior b32
_compositum (
                          Piscina* piscina,
    constans OratioVocabulariumEn* voc,
                              s32  index_regulae,
                           chorda  forma,
                              Xar* exitus)
{
                   i32  a        = ZEPHYRUM;
                   i32  i;
                   i32  partes   = ZEPHYRUM;
                   i32  verba    = ZEPHYRUM;
    constans character* classis  = NIHIL;

    per (i = ZEPHYRUM; i <= forma.mensura; i++)
    {
        si (i == forma.mensura || forma.datum[i] == '-')
        {
            chorda  pars;
               Xar* sub;

            si (i == a)
            {
                redde VERUM;   /* pars vacua: non compositum */
            }
            pars.datum = forma.datum + a;
            pars.mensura = i - a;
            sub = xar_creare(piscina, (i32)magnitudo(OratioAnalysisEn));
            si (   sub == NIHIL
                || !_analysare_simplex(piscina, voc, pars, sub))
            {
                redde FALSUM;
            }
            si (xar_numerus(sub) > ZEPHYRUM)
            {
                classis = ((constans OratioAnalysisEn*)xar_obtinere(sub,
                    ZEPHYRUM))->classis;
                verba   = verba + I;
            }
            alioquin si (!_praefixum_est(pars))
            {
                redde VERUM;   /* pars ignota: non compositum */
            }
            partes  = partes + I;
            a       = i + I;
        }
    }
    si (partes < (i32)II || verba < I)
    {
        redde VERUM;
    }
    redde _analysin_addere(exitus, (s32)-I, index_regulae, forma,
        classis);
}

/* forma exacta primum, deinde regulae suffixorum (non compositum) */
interior b32
_analysare_simplex (
                          Piscina* piscina,
    constans OratioVocabulariumEn* voc,
                           chorda  plicata,
                              Xar* exitus)
{
       Xar* exacta;
       i32  k;
       i32  r;

    /* I. forma exacta, semper prior */
    exacta = oratio_vocabularium_en_quaerere(piscina, voc, plicata);
        si (exacta == NIHIL)
        {
        redde FALSUM;
        }
    per (k = ZEPHYRUM; k < xar_numerus(exacta); k++)
    {
        s32 ri = *(s32*)xar_obtinere(exacta, k);

        si (!_analysin_addere(exitus, ri, (s32)-I, plicata,
                oratio_vocabularium_en_classis(
                    oratio_vocabularium_en_recordum(voc, ri)
                        ->codices.datum[ZEPHYRUM])))
        {
                        redde FALSUM;
        }
    }
    /* II. regulae suffixorum ordine tabulae (compositum alibi) */
    per (r = ZEPHYRUM; r < ORATIO_REGULAE_EN_NUMERUS; r++)
    {
        constans OratioRegulaEn* regula = &ORATIO_REGULAE_EN[r];
                            i32  ls = (i32)strlen(regula->suffixum);
                                                        i32  mensura_substitutionis =
                                                            (i32)strlen(regula->substitutio);
                            i32 radix;
                         chorda basis;

        si (strcmp(regula->suffixum, "-") == ZEPHYRUM)
        {
            perge;
        }
        si (   plicata.mensura <= ls
            || !_finitur(plicata, regula->suffixum))
        {
            perge;
        }
        radix = plicata.mensura - ls;
        si (regula->geminatio)
        {
            /* consonans duplex finalis radicis: una manet (planned -> plan) */
            si (   radix < (i32)II
                || plicata.datum[radix - I] != plicata.datum[radix
                    - (i32)II]
                || !_consonans(plicata.datum[radix - I]))
            {
                perge;
            }
            radix = radix - I;
        }
        si (radix + mensura_substitutionis < regula->basis_minima)
        {
            perge;
        }
                basis.datum = (i8*)piscina_allocare(piscina,
                    (memoriae_index)(radix + mensura_substitutionis)
                        + I);
                si (basis.datum == NIHIL)
                {
            redde FALSUM;
                }
        memcpy(basis.datum, plicata.datum, (size_t)radix);
                memcpy(basis.datum + radix, regula->substitutio,
                    (size_t)mensura_substitutionis);
        basis.mensura               = radix + mensura_substitutionis;
        basis.datum[basis.mensura]  = ZEPHYRUM;
        si (!_basin_quaerere(piscina, voc, regula, (s32)r, basis,
            exitus))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

Xar*
oratio_vocabularium_en_analysare (
                          Piscina* piscina,
    constans OratioVocabulariumEn* voc,
                           chorda  forma)
{
       Xar* exitus = xar_creare(piscina,
           (i32)magnitudo(OratioAnalysisEn));
    chorda plicata;
       i32 r;

    si (exitus == NIHIL)
    {
        redde NIHIL;
    }
    plicata = oratio_vocabularium_en_plicare(piscina, forma);
    si (plicata.datum == NIHIL)
    {
        redde NIHIL;
    }
    si (!_analysare_simplex(piscina, voc, plicata, exitus))
    {
        redde NIHIL;
    }
    /* III. compositum (ordines suffixi "-"), partes per regulas simplices */
    per (r = ZEPHYRUM; r < ORATIO_REGULAE_EN_NUMERUS; r++)
    {
        si (   strcmp(ORATIO_REGULAE_EN[r].suffixum, "-") == ZEPHYRUM
            && !_compositum(piscina, voc, (s32)r, plicata, exitus))
        {
            redde NIHIL;
        }
    }
    redde exitus;
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
