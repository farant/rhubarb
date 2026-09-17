/* probatio_css_diagnostica.c - Diagnostica css ex declaratione (planum
 * materia-sedes B4)
 *
 * Numerus diagnosticorum DERIVATORUM (tabula CSS_DIAGNOSTICA per
 * ambulatorem materiae) == numerus nodorum generum malorum, per
 * ambulationem HIC SCRIPTAM numeratus: implementatio altera super
 * codicem disiunctum, ergo oraculum independens (spec S9). Corpus
 * idem quod porta STML gerit; deinde fixa parva quorum sedes
 * PINNATAE sunt (post inspectionem oculi).
 */

#include "latina.h"
#include "credo.h"
#include "css_arbor.h"
#include "css_registrum.h"
#include "css_selector.h"
#include "materia_diagnostica.h"
#include "piscina.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

hic_manens constans character* CORPUS[] = {
    "probationes/fixa/css/componentia_2026-08-18.css",
    "probationes/fixa/css/thema_2026-08-18.css",
    "probationes/fixa/css/adversarius.css",
    "probationes/fixa/css/adversarius_2.css",
    "lib/speculum_assets/speculum.css",
    "knotapel/atlas/web/style.css"
};

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

/* ambulatio ALTERA: genera mala numerat sine tabula diagnosticorum */
interior i32
_mala_numerare (
    constans MateriaValor* valor)
{
    i32 numerus = ZEPHYRUM;
    i32 k;

    si (   valor->genus       == MATERIA_VALOR_NODUS
        && valor->datum.nodus != NIHIL)
    {
        constans MateriaNodus* nodus = valor->datum.nodus;

        si (   nodus->genus == (s32)CSS_GENUS_REGULA_MALA
            || nodus->genus == (s32)CSS_GENUS_DECLARATIO_MALA
            || nodus->genus == (s32)CSS_GENUS_SELECTOR_MALUS)
        {
            numerus++;
        }
        per (k = ZEPHYRUM; k < nodus->numerus_locorum; k++)
        {
            numerus += _mala_numerare(&nodus->loci[k]);
        }
    }
    alioquin si (valor->genus == MATERIA_VALOR_LISTA)
    {
        per (k = ZEPHYRUM; k < materia_valor_lista_numerus(*valor);
             k++)
        {
            constans MateriaValor* elementum =
                materia_valor_lista_obtinere(*valor, k);

            si (elementum != NIHIL)
            {
                numerus += _mala_numerare(elementum);
            }
        }
    }
    redde numerus;
}

/* Arbor SELECTORUM super praeludium regulae primae. Arbor analyseos
 * est, disiuncta ab arbore plagulae (css_selector.h): 'selector-malus'
 * SOLUS ibi nascitur, ergo ambulatio super arborem plagulae eum
 * numquam attingit - declaratio tertia hic solum exercetur. */
interior MateriaNodus*
_selectorem_parsare (
               Piscina* piscina,
    constans character* selector)
{
       character* fons;
    MateriaNodus* plagula;
    MateriaNodus* regula;

    fons = (character*)piscina_allocare(piscina,
        (memoriae_index)strlen(selector) + III);
    sprintf(fons, "%s{}", selector);
    plagula = css_arbor_parsare(piscina, fons, (i32)strlen(fons));
    si (plagula == NIHIL)
    {
        redde NIHIL;
    }
    si (materia_valor_lista_numerus(plagula->loci[CSS_PLAGULA_REGULAE])
        < I)
    {
        redde NIHIL;
    }
    regula = materia_valor_lista_obtinere(
        plagula->loci[CSS_PLAGULA_REGULAE], ZEPHYRUM)->datum.nodus;
    si (regula->genus != (s32)CSS_GENUS_REGULA_QUALIFICATA)
    {
        redde NIHIL;
    }
    redde css_selector_parsare(piscina,
        regula->loci[CSS_REGULA_PRAELUDIUM]);
}

/* Sedes EXPECTATA unius diagnostici: post inspectionem oculi
 * (2026-09-17, Fran) pinnata - octeti fine EXCLUSIVO. */
nomen structura {
    constans character* codex;
                   i32  linea;
                   i32  columna;
                   s32  initium;
                   s32  finis;
} SedesExpectata;

nomen structura {
     constans character* fons;
                    i32  numerus;
         SedesExpectata  expectata[II];
} Fixum;

/* fixum unum: numerum et sedem CUIUSQUE diagnostici contra tabulam */
interior vacuum
_fixum_probare (
             Piscina* piscina,
  constans character* titulus,
        MateriaNodus* arbor,
      constans Fixum* fixum)
{
    Xar* derivata;
    i32  j;

    CREDO_NON_NIHIL (arbor);
    si (arbor == NIHIL)
    {
        redde;
    }
    derivata = materia_diagnostica_derivare(piscina, arbor,
        &CSS_REGISTRUM, &CSS_DIAGNOSTICA, NIHIL, NIHIL);
    CREDO_NON_NIHIL (derivata);
    si (derivata == NIHIL)
    {
        redde;
    }
    imprimere("    %s '%s': %d\n", titulus, fixum->fons,
        (integer)xar_numerus(derivata));
    CREDO_AEQUALIS_I32 (xar_numerus(derivata), fixum->numerus);
    si (xar_numerus(derivata) != fixum->numerus)
    {
        redde;
    }
    per (j = ZEPHYRUM; j < fixum->numerus; j++)
    {
        constans MateriaDiagnosticum* d =
            (constans MateriaDiagnosticum*)xar_obtinere(derivata, j);
        constans SedesExpectata* e = &fixum->expectata[j];

        imprimere("      %s linea %d columna %d (%d-%d) '%s'\n",
            d->codex, (integer)d->tractus.linea,
            (integer)d->tractus.columna, (integer)d->tractus.initium,
            (integer)d->tractus.finis, d->causa);
        CREDO_VERUM (strcmp(d->codex, e->codex) == ZEPHYRUM);
        CREDO_AEQUALIS_I32 (d->tractus.linea, e->linea);
        CREDO_AEQUALIS_I32 (d->tractus.columna, e->columna);
        CREDO_AEQUALIS_S32 (d->tractus.initium, e->initium);
        CREDO_AEQUALIS_S32 (d->tractus.finis, e->finis);
        CREDO_VERUM (d->gravitas == (s32)MATERIA_GRAVITAS_ERRATUM);
    }
}

/* diagnostica derivare et contra ambulationem alteram conferre;
 * numerum derivatorum per 'numerus' reddit. FALSUM = derivatio
 * defecit (i32 INSIGNATUS est: sentinella negativa nulla) */
interior b32
_conferre (
                Piscina* piscina,
     constans character* titulus,
           MateriaNodus* arbor,
                    i32* numerus)
{
         MateriaValor valor;
                 Xar*  derivata;
                  i32  expectata;

    valor      = materia_valor_nodus(arbor);
    expectata  = _mala_numerare(&valor);
    derivata  = materia_diagnostica_derivare(piscina, arbor,
        &CSS_REGISTRUM, &CSS_DIAGNOSTICA, NIHIL, NIHIL);
    CREDO_NON_NIHIL (derivata);
    si (derivata == NIHIL)
    {
        redde FALSUM;
    }
    *numerus = xar_numerus(derivata);
    imprimere("    %s: derivata %d, mala %d\n", titulus,
        (integer)*numerus, (integer)expectata);
    CREDO_AEQUALIS_I32 (*numerus, expectata);
    redde VERUM;
}

s32
principale (vacuum)
{
                   b32  praeteritus;
               Piscina* piscina;
    constans character* radix = getenv("RHUBARB_RADIX");
                   i32  numerus;
                   i32  plagulae  = ZEPHYRUM;
                   i32  summa     = ZEPHYRUM;
                   i32  i;

    piscina = piscina_generare_dynamicum("probatio_css_diagnostica",
        4194304);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    si (radix == NIHIL)
    {
        radix = ".";
    }


    /* ==================================================
     * PROBARE: corpus contra ambulationem alteram
     * ================================================== */

    imprimere("\n--- Probans corpus contra numerationem suam ---\n");

    numerus = (i32)(magnitudo(CORPUS) / magnitudo(CORPUS[0]));
    per (i = ZEPHYRUM; i < numerus; i++)
    {
             character  via[DXII];
             character* textus;
                   i32  mensura = ZEPHYRUM;
         MateriaNodus*  arbor;
                  i32   derivata = ZEPHYRUM;

        sprintf(via, "%s/%s", radix, CORPUS[i]);
        textus = _plagulam_legere(piscina, via, &mensura);
        si (textus == NIHIL)
        {
            imprimere("    ABEST: %s\n", CORPUS[i]);
            CREDO_CULPA ("plagula corporis absens");
            perge;
        }
        arbor = css_arbor_parsare(piscina, textus, mensura);
        CREDO_NON_NIHIL (arbor);
        si (arbor == NIHIL)
        {
            perge;
        }
        plagulae++;
        si (_conferre(piscina, CORPUS[i], arbor, &derivata))
        {
            summa += derivata;
        }
    }
    imprimere("    plagulae %d, diagnostica %d\n", (integer)plagulae,
        (integer)summa);
    CREDO_AEQUALIS_I32 (plagulae, numerus);


    /* ==================================================
     * PROBARE: fixa parva, sedes pinnatae
     * ================================================== */

    imprimere("\n--- Probans fixa parva ---\n");

    {
        hic_manens constans Fixum FIXA[] = {
            /* '}' vaga ad indicem XVIII */
            { "a { color: red; } }", I,
              { { "regula-mala", I, (i32)XIX, (s32)XVIII, (s32)XIX },
                { NIHIL, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM } } },
            /* ': x;' sine titulo proprietatis, indices IV-VII */
            { "a { : x; }", I,
              { { "declaratio-mala", I, V, (s32)IV, (s32)VIII },
                { NIHIL, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM } } }
        };
        i32 n;

        per (n = ZEPHYRUM;
             n < (i32)(magnitudo(FIXA) / magnitudo(FIXA[0]));
             n++)
        {
            MateriaNodus* arbor = css_arbor_parsare(piscina,
                FIXA[n].fons, (i32)strlen(FIXA[n].fons));

            _fixum_probare(piscina, "fixum", arbor, &FIXA[n]);
        }
    }


    /* ==================================================
     * PROBARE: arbor selectorum (declaratio tertia)
     * ================================================== */

    imprimere("\n--- Probans arborem selectorum ---\n");

    {
        hic_manens constans Fixum SELECTORES[] = {
            /* fons '<selector>{}': octetus nullus selectoris est */
            { "1", I,
              { { "selector-malus", I, I, ZEPHYRUM, (s32)I },
                { NIHIL, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM } } },
            /* '1' culpatur, non 'a' */
            { "a, 1", I,
              { { "selector-malus", I, (i32)IV, (s32)III, (s32)IV },
                { NIHIL, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM } } },
            /* pars mala QUAEQUE suum diagnosticum habet */
            { "..", (i32)II,
              { { "selector-malus", I, I, ZEPHYRUM, (s32)I },
                { "selector-malus", I, (i32)II, (s32)I, (s32)II } } }
        };
        i32 n;

        per (n = ZEPHYRUM;
             n < (i32)(magnitudo(SELECTORES)
                 / magnitudo(SELECTORES[0]));
             n++)
        {
            MateriaNodus* arbor = _selectorem_parsare(piscina,
                SELECTORES[n].fons);

            _fixum_probare(piscina, "selector", arbor,
                &SELECTORES[n]);
        }
    }

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
