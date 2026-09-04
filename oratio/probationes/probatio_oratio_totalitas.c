/* probatio_oratio_totalitas.c - Porta totalitatis orationis (T5)
 *
 * CONTRACTUS DUPLEX omnibus linguis materiae (Fran, 2026-09-01):
 *   (a) NUMQUAM RUIT pro quolibet fonte - furca, WIFSIGNALED
 *   (b) SEMPER arborem reddit, et emissio == fons octetim
 * Oratio textum QUEMLIBET habet (octeti quilibet), ergo (b) contractus
 * totus est. GENERATORES (ut md): octeti fortuiti (LCG), mutatio
 * corporis fixi (fixa/txt, I ex XL octetis), truncatio (XXIII gradus),
 * nidificatio formis orationis (parentheses, unci, citationes rectae et
 * curvae, abbreviationes catenatae, mixtum) profunditatibus I..M,
 * deinde L milia (parentheses, puncta, paragraphi vacui, sententia
 * unius verbi L milia vicibus) NON RUIT. IN FRACTURA: fons peccans in
 * oratio/build/totalitas_fractum.txt.
 */

#include "latina.h"
#include "credo.h"
#include "oratio_arbor.h"
#include "oratio_registrum.h"
#include "oratio_lexicon.h"
#include "materia_nodus.h"
#include "materia_scribere.h"
#include "piscina.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

hic_manens constans character* CORPUS[] = {
    "oratio/probationes/fixa/txt/hilarius.txt",
    "oratio/probationes/fixa/txt/propertius.txt",
    "oratio/probationes/fixa/txt/cicero.txt",
    "oratio/probationes/fixa/txt/lincoln.txt"
};

enumeratio {
    TOTUM_IDEM = 0,
    TOTUM_NIHIL,
    TOTUM_EMISSIO_FRACTA,
    TOTUM_DISPAR
};

hic_manens constans character* CAUSAE[] = {
    "idem", "parsator NIHIL reddidit", "emissio fracta",
    "emissio a fonte dispar"
};

hic_manens constans character* RADIX_VIAE = ".";

interior character*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura)
{
          FILE* f = fopen(via, "rb");
        longus  longitudo;
     character* memoria;

    si (f == NIHIL)
    {
        redde NIHIL;
    }
    fseek(f, 0L, SEEK_END);
    longitudo = ftell(f);
    rewind(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde NIHIL;
    }
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    si (fread(memoria, I, (size_t)longitudo, f) != (size_t)longitudo)
    {
        fclose(f);
        redde NIHIL;
    }
    fclose(f);
    *mensura = (i32)longitudo;
    redde memoria;
}

interior i32
_totum (
    constans character* fons,
                   i32  mensura)
{
    Piscina* piscina;
    MateriaNodus* radix;
    MateriaScriptura emissa;
    MateriaScripturaConsilium consilium;
    i32 fructus;

    piscina  = piscina_generare_dynamicum("totalitas_casus", 65536);
    radix    = oratio_arbor_parsare(piscina, fons, mensura);
    si (radix == NIHIL)
    {
        piscina_destruere(piscina);
        redde (i32)TOTUM_NIHIL;
    }
    materia_scriptura_consilium_nudum(&consilium, &ORATIO_REGISTRUM);
    consilium.fons_index = ORATIO_FONS_PLAGULAE;
    emissa = materia_scribere_nodum(piscina, radix, &consilium);
    si (!emissa.successus)
    {
        fructus = (i32)TOTUM_EMISSIO_FRACTA;
    }
    alioquin si (   emissa.textus.mensura != mensura
                 || (mensura > ZEPHYRUM
                     && memcmp(emissa.textus.datum, fons,
                     (size_t)mensura)
                         != ZEPHYRUM))
    {
        fructus = (i32)TOTUM_DISPAR;
    }
    alioquin
    {
        fructus = (i32)TOTUM_IDEM;
    }
    piscina_destruere(piscina);
    redde fructus;
}

interior vacuum
_fractum_scribere (
    constans character* fons,
                   i32  mensura)
{
    character via[DXII];
    FILE* f;

    sprintf(via, "%s/oratio/build/totalitas_fractum.txt", RADIX_VIAE);
    f = fopen(via, "wb");
    si (f == NIHIL)
    {
        redde;
    }
    fwrite(fons, I, (size_t)mensura, f);
    fclose(f);
    imprimere("    fons peccans scriptus: %s\n", via);
}

interior vacuum
_casum_probare (
    constans character* titulus,
    constans character* fons,
                   i32  mensura,
                   i32* numerator)
{
    i32 fructus;

    CREDO_NON_RUIT (_totum(fons, mensura));
    fructus = _totum(fons, mensura);
    si (fructus != (i32)TOTUM_IDEM)
    {
        imprimere("  FRACTUM %s (%d octeti): %s\n", titulus,
            (integer)mensura,
            CAUSAE[fructus]);
        _fractum_scribere(fons, mensura);
    }
    CREDO_AEQUALIS_I32 (fructus, (i32)TOTUM_IDEM);
    *numerator = *numerator + I;
}

interior character
_fortuitus (
    i32* status)
{
    *status = *status * (i32)1103515245 + (i32)12345;
    redde (character)((*status >> 16) & (i32)0xFF);
}

nomen structura {
    constans character* titulus;
    constans character* apertura;
    constans character* clausura;
    constans character* praefixum;
    constans character* suffixum;
} FormaNidi;

hic_manens constans FormaNidi FORMAE[] = {
    { "parentheses",   "(",             ")",          "a ", " b.\n" },
    { "unci",          "[",             "]",          "",   "\n"    },
    { "citationes",    "\"",            "\"",         "a ", " b\n"  },
    { "curvae",        "\xE2\x80\x9C",  "\xE2\x80\x9D", "", ".\n"   },
    { "abbreviationes", "Mr. ",          "",           "",   "X.\n"  },
    { "mixtum",        "(\"Dr. ",       "\")",        "",   " y\n"  }
};

interior character*
_nidum_struere (
               Piscina* piscina,
    constans FormaNidi* forma,
                   i32  profunditas,
                   b32  clausum,
                   i32* mensura)
{
          i32  la = (i32)strlen(forma->apertura);
          i32  lc = (i32)strlen(forma->clausura);
          i32  lp = (i32)strlen(forma->praefixum);
          i32  ls = (i32)strlen(forma->suffixum);
          i32  summa;
          i32  i;
    character* textus;
    character* cursor;

    summa = lp + la * profunditas + ls;
    si (clausum)
    {
        summa = summa + lc * profunditas;
    }
    textus = (character*)piscina_allocare(piscina, (memoriae_index)summa
        + I);
    cursor = textus;
    memcpy(cursor, forma->praefixum, (size_t)lp);
    cursor = cursor + lp;
    per (i = ZEPHYRUM; i < profunditas; i++)
    {
        memcpy(cursor, forma->apertura, (size_t)la);
        cursor = cursor + la;
    }
    si (clausum)
    {
        per (i = ZEPHYRUM; i < profunditas; i++)
        {
            memcpy(cursor, forma->clausura, (size_t)lc);
            cursor = cursor + lc;
        }
    }
    memcpy(cursor, forma->suffixum, (size_t)ls);
    *mensura = summa;
    redde textus;
}

hic_manens constans i32 PROFUNDITATES[] = { I, X, C, M };

interior vacuum
_nidos_probare (
               Piscina* piscina,
    constans FormaNidi* forma,
                   i32* numerator)
{
    i32 c;
    i32 d;

    per (c = ZEPHYRUM; c < II; c++)
    {
        per (d = ZEPHYRUM; d < IV; d++)
        {
            character  titulus[LXIV];
            character* fons;
                  i32  mensura = ZEPHYRUM;

            fons = _nidum_struere(piscina, forma, PROFUNDITATES[d],
                (b32)(c == ZEPHYRUM), &mensura);
            sprintf(titulus, "nidus %s %s x%d", forma->titulus,
                c == ZEPHYRUM ? "clausus" : "apertus",
                (integer)PROFUNDITATES[d]);
            _casum_probare(titulus, fons, mensura, numerator);
        }
    }
}

integer
principale (vacuum)
{
               Piscina* piscina;
    constans character* radix_viae;
                   b32  praeteritus;
                   i32  numerus_corporis;
                   i32  plagulae_lectae  = ZEPHYRUM;
                   i32  casus_fortuiti   = ZEPHYRUM;
                   i32  casus_mutati     = ZEPHYRUM;
                   i32  casus_truncati   = ZEPHYRUM;
                   i32  casus_nidorum    = ZEPHYRUM;
                   i32  i;

    piscina = piscina_generare_dynamicum("probatio_oratio_totalitas",
        1048576);
    credo_aperire(piscina);
    radix_viae = getenv("RHUBARB_RADIX");
    si (radix_viae != NIHIL)
    {
        RADIX_VIAE = radix_viae;
    }
    numerus_corporis = (i32)(magnitudo(CORPUS) / magnitudo(CORPUS[0]));

    imprimere("\n--- Probans octetos fortuitos ---\n");
    per (i = I; i <= XXXII; i++)
    {
              i32  status   = i;
              i32  mensura  = (i32)LXIV * i;
        character* fons = (character*)piscina_allocare(piscina,
            (memoriae_index)mensura);
        character titulus[LXIV];
              i32 j;

        per (j = ZEPHYRUM; j < mensura; j++)
        {
            fons[j] = _fortuitus(&status);
        }
        sprintf(titulus, "fortuiti semen=%d", (integer)i);
        _casum_probare(titulus, fons, mensura, &casus_fortuiti);
    }
    imprimere("  casus %d\n", (integer)casus_fortuiti);

    imprimere("\n--- Probans corpus mutatum et truncatum ---\n");
    per (i = ZEPHYRUM; i < numerus_corporis; i++)
    {
        character  via[DXII];
        character* textus;
        character  titulus[DXII];
              i32  mensura = ZEPHYRUM;
              i32  semen;
              i32  gradus;

        sprintf(via, "%s/%s", RADIX_VIAE, CORPUS[i]);
        textus = _plagulam_legere(piscina, via, &mensura);
        si (textus == NIHIL)
        {
            imprimere("  ABEST: %s\n", CORPUS[i]);
            perge;
        }
        plagulae_lectae = plagulae_lectae + I;
        per (semen = I; semen <= IV; semen++)
        {
                  i32  status  = semen * (i32)7919;
                  i32  ictus   = mensura / XL;
            character* mutatum = (character*)piscina_allocare(piscina,
                (memoriae_index)mensura + I);
            i32 k;

            memcpy(mutatum, textus, (size_t)mensura);
            per (k = ZEPHYRUM; k < ictus; k++)
            {
                i32 sedes;

                _fortuitus(&status);
                sedes = (status >> 8) % (i32)(mensura
                    > ZEPHYRUM ? mensura : I);
                mutatum[sedes] = _fortuitus(&status);
            }
            sprintf(titulus, "mutatum %s semen=%d", CORPUS[i],
                (integer)semen);
            _casum_probare(titulus, mutatum, mensura, &casus_mutati);
        }
        per (gradus = I; gradus < XXIV; gradus++)
        {
            i32 mensura_trunca = (i32)((s64)mensura * gradus / XXIV);

            sprintf(titulus, "truncatum %s @%d/XXIV", CORPUS[i],
                (integer)gradus);
            _casum_probare(titulus, textus, mensura_trunca,
                &casus_truncati);
        }
    }
    imprimere("  plagulae %d, mutati %d, truncati %d\n",
        (integer)plagulae_lectae,
        (integer)casus_mutati, (integer)casus_truncati);
    CREDO_AEQUALIS_I32 (plagulae_lectae, IV);

    imprimere("\n--- Probans nidificationem ---\n");
    {
        i32 numerus_formarum = (i32)(magnitudo(FORMAE)
            / magnitudo(FORMAE[0]));
        i32 f;

        per (f = ZEPHYRUM; f < numerus_formarum; f++)
        {
            _nidos_probare(piscina, &FORMAE[f], &casus_nidorum);
        }
        imprimere("  casus %d\n", (integer)casus_nidorum);
    }

    imprimere("\n--- Probans profunditatem ingentem (L milia) ---\n");
    {
                                  i32  mensura = ZEPHYRUM;
                            character* fons;
        hic_manens constans FormaNidi  VERBA = { "verba", "a ", "", "",
            "\n" };
        hic_manens constans FormaNidi PARAGRAPHI = { "paragraphi",
            "a\n\n", "", "", "" };
        hic_manens constans FormaNidi PUNCTA = { "puncta", ".", "", "",
            "\n" };

        fons = _nidum_struere(piscina, &FORMAE[ZEPHYRUM], (i32)50000,
            VERUM, &mensura);
        CREDO_NON_RUIT (_totum(fons, mensura));
        fons = _nidum_struere(piscina, &VERBA, (i32)50000, VERUM,
            &mensura);
        CREDO_NON_RUIT (_totum(fons, mensura));
        fons = _nidum_struere(piscina, &PARAGRAPHI, (i32)50000, VERUM,
            &mensura);
        CREDO_NON_RUIT (_totum(fons, mensura));
        fons = _nidum_struere(piscina, &PUNCTA, (i32)50000, VERUM,
            &mensura);
        CREDO_NON_RUIT (_totum(fons, mensura));
    }

    CREDO_MAIOR_I32 (casus_fortuiti,  ZEPHYRUM);
    CREDO_MAIOR_I32 (casus_mutati,    ZEPHYRUM);
    CREDO_MAIOR_I32 (casus_truncati,  ZEPHYRUM);
    CREDO_MAIOR_I32 (casus_nidorum,   ZEPHYRUM);
    CREDO_MAIOR_I32 (casus_fortuiti + casus_mutati + casus_truncati
        + casus_nidorum, (i32)CL);

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
