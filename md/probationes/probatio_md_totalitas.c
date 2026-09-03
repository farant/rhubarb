/* probatio_md_totalitas.c - Porta totalitatis markdown (A8c, 2026-09-03)
 *
 * CONTRACTUS DUPLEX omnibus linguis materiae (Fran, 2026-09-01):
 *   (a) NUMQUAM RUIT pro quolibet fonte - furca, WIFSIGNALED
 *   (b) SEMPER arborem reddit, et emissio == fons octetim
 * Materia dimidium (b) structuraliter tenet; parsator dimidium (a) et
 * arborem ipsam praestat. Markdown textum QUEMLIBET validum habet, ergo
 * (b) hic contractus totus est: nulla plagula recusatur.
 *
 * GENERATORES (ut css): octeti fortuiti (LCG seminatus), mutatio
 * corporis fixi (I ex XL octetis), truncatio (XXIII gradus),
 * nidificatio clausa et aperta formis markdown (citationes, listae,
 * unci, asterisci, graves, mixtum) profunditatibus I..M, deinde L
 * milia (continentia ultra LXIV litterae fiunt - MD_PROFUNDITAS_MAXIMA;
 * unci et delimitatores polynomiales) NON RUIT - md ruinam notam nullam
 * habet quam pinnet (css 01M1FAD8 pinnabat).
 *
 * IN FRACTURA: fons peccans in md/build/totalitas_fractum.md scribitur.
 */

#include "latina.h"
#include "credo.h"
#include "md_arbor.h"
#include "md_registrum.h"
#include "md_lexicon.h"
#include "materia_nodus.h"
#include "materia_scribere.h"
#include "piscina.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


hic_manens constans character* CORPUS[] = {
    "md/probationes/fixa/md/spica.md",
    "md/probationes/fixa/md/episodium.md",
    "md/probationes/fixa/md/piscina.md",
    "md/probationes/fixa/md/sabaw.md"
};

/* Fructus casus unius - causa NOMINATA, non b32 mutum */
enumeratio {
    TOTUM_IDEM = 0,
    TOTUM_NIHIL,
    TOTUM_EMISSIO_FRACTA,
    TOTUM_DISPAR
};

hic_manens constans character* CAUSAE[] = {
    "idem",
    "parsator NIHIL reddidit",
    "emissio fracta",
    "emissio a fonte dispar"
};

hic_manens constans character* RADIX_VIAE = ".";


interior character*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura)
{
         FILE* f;
        longus longitudo;
    character* memoria;
        size_t lecti;

    f = fopen(via, "rb");
    si (f == NIHIL)
    {
        redde NIHIL;
    }
    si (fseek(f, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(f);
        redde NIHIL;
    }
    longitudo = ftell(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde NIHIL;
    }
    rewind(f);

    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    lecti = fread(memoria, I, (size_t)longitudo, f);
    fclose(f);
    si (lecti != (size_t)longitudo)
    {
        redde NIHIL;
    }

    *mensura = (i32)longitudo;
    redde memoria;
}

/* Casus unus, piscina SUA: parsare, emittere, conferre. In filio
 * furcato et in parente idem curritur - piscina propria ne
 * parens milia arborum accumulet. */
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
    radix    = md_arbor_parsare(piscina, fons, mensura);
    si (radix == NIHIL)
    {
        piscina_destruere(piscina);
        redde (i32)TOTUM_NIHIL;
    }
    materia_scriptura_consilium_nudum(&consilium, &MD_REGISTRUM);
    consilium.fons_index = MD_FONS_PLAGULAE;
    emissa = materia_scribere_nodum(piscina, radix, &consilium);
    si (!emissa.successus)
    {
        fructus = (i32)TOTUM_EMISSIO_FRACTA;
    }
    alioquin si (   emissa.textus.mensura != mensura
                 || (mensura > ZEPHYRUM
                 && memcmp(emissa.textus.datum, fons,
                      (size_t)mensura) != ZEPHYRUM))
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

    sprintf(via, "%s/md/build/totalitas_fractum.md", RADIX_VIAE);
    f = fopen(via, "wb");
    si (f == NIHIL)
    {
        redde;
    }
    fwrite(fons, I, (size_t)mensura, f);
    fclose(f);
    imprimere("    fons peccans scriptus: %s\n", via);
}

/* Furca prima (ruina/gyrus), deinde iudicium in parente. Casus
 * numeratur ut classis vacua rubeat, non taceat. */
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
            (integer)mensura, CAUSAE[fructus]);
        _fractum_scribere(fons, mensura);
    }
    CREDO_AEQUALIS_I32 (fructus, (i32)TOTUM_IDEM);
    *numerator = *numerator + I;
}

/* LCG classicus: status * 1103515245 + 12345, octetus ex medio.
 * i32 INSIGNATUS est (latina), ergo circumvolutio definita. */
interior character
_fortuitus (
    i32* status)
{
    *status = *status * (i32)1103515245 + (i32)12345;
    redde (character)((*status >> 16) & (i32)0xFF);
}

/* Nidificatio: forma cum apertura/clausura sua; praefixum et
 * suffixum eae formae quae intra declarationem vivunt. */
nomen structura {
    constans character* titulus;
    constans character* apertura;
    constans character* clausura;
    constans character* praefixum;
    constans character* suffixum;
} FormaNidi;

hic_manens constans FormaNidi FORMAE[] = {
    { "citationes", "> ",     "",   "",  "x\n" },
    { "listae",     "- ",     "",   "",  "x\n" },
    { "unci",       "[",      "]",  "",  "\n"  },
    { "asterisci",  "*",      "*",  "a", "b\n" },
    { "graves",     "`",      "`",  "a", "b\n" },
    { "mixtum",     "> - [*", "*]", "",  "x\n" }
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
    textus = (character*)piscina_allocare(piscina,
        (memoriae_index)summa + I);
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

/* Forma una: {clausa, aperta} x profunditates */
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
                   i32  plagulae_lectae;
                   i32  casus_fortuiti;
                   i32  casus_mutati;
                   i32  casus_truncati;
                   i32  casus_nidorum;
                   i32  i;

    piscina = piscina_generare_dynamicum("probatio_md_totalitas",
        1048576);
    credo_aperire(piscina);

    radix_viae = getenv("RHUBARB_RADIX");
    si (radix_viae != NIHIL)
    {
        RADIX_VIAE = radix_viae;
    }

    numerus_corporis  = (i32)(magnitudo(CORPUS) / magnitudo(CORPUS[0]));
    plagulae_lectae   = ZEPHYRUM;
    casus_fortuiti    = ZEPHYRUM;
    casus_mutati      = ZEPHYRUM;
    casus_truncati    = ZEPHYRUM;
    casus_nidorum     = ZEPHYRUM;


    /* ==================================================
     * OCTETI FORTUITI: semina I..XXXII, LXIV..MMXLVIII octeti
     * ================================================== */

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


    /* ==================================================
     * CORPUS MUTATUM + TRUNCATUM: VI plagulae verae
     * ================================================== */

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

        /* mutatio: I ex XL octetis, semina IV */
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
                sedes = (status >> 8)
                      % (i32)(mensura > ZEPHYRUM ? mensura : I);
                mutatum[sedes] = _fortuitus(&status);
            }
            sprintf(titulus, "mutatum %s semen=%d", CORPUS[i],
                (integer)semen);
            _casum_probare(titulus, mutatum, mensura, &casus_mutati);
        }

        /* truncatio: gradus I..XXIII ex XXIV */
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
        (integer)plagulae_lectae, (integer)casus_mutati,
        (integer)casus_truncati);
    CREDO_AEQUALIS_I32 (plagulae_lectae, IV);


    /* ==================================================
     * NIDIFICATIO: formae VI x {clausa, aperta} x {I, X, C, M}
     * ================================================== */

    imprimere("\n--- Probans nidificationem ---\n");
    {
        i32 numerus_formarum;
        i32 f;

        numerus_formarum = (i32)(magnitudo(FORMAE)
                               / magnitudo(FORMAE[0]));
        per (f = ZEPHYRUM; f < numerus_formarum; f++)
        {
            _nidos_probare(piscina, &FORMAE[f], &casus_nidorum);
        }
        imprimere("  casus %d\n", (integer)casus_nidorum);
    }


    /* ==================================================
     * PROFUNDITAS INGENS: L milia - continentia ultra LXIV litterae
     * fiunt (md_arbor: MD_PROFUNDITAS_MAXIMA), inlinea polynomialia:
     * NON RUIT asseritur (css ruinam notam hic pinnat; md nullam habet)
     * ================================================== */

    imprimere("\n--- Probans profunditatem ingentem (L milia) ---\n");
    {
              i32  mensura = ZEPHYRUM;
        character* fons = _nidum_struere(piscina, &FORMAE[ZEPHYRUM],
            (i32)50000, VERUM, &mensura);
        CREDO_NON_RUIT (_totum(fons, mensura));
        fons = _nidum_struere(piscina, &FORMAE[II], (i32)50000, VERUM,
            &mensura);
        CREDO_NON_RUIT (_totum(fons, mensura));
    }


    /* Tegumentum SUUM: classis vacua rubet */
    CREDO_MAIOR_I32 (casus_fortuiti,  ZEPHYRUM);
    CREDO_MAIOR_I32 (casus_mutati,    ZEPHYRUM);
    CREDO_MAIOR_I32 (casus_truncati,  ZEPHYRUM);
    CREDO_MAIOR_I32 (casus_nidorum,   ZEPHYRUM);
    /* IV plagulae fixae (css VI): XXXII + XVI + XCII + XLVIII = CLXXXVIII */
    CREDO_MAIOR_I32 (casus_fortuiti + casus_mutati + casus_truncati
        + casus_nidorum, (i32)CL);

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
