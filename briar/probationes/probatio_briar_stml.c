/* probatio_briar_stml.c - Circuitus proiectionis STML thistle:
 * scribere -> legere -> scribere BIS octetim idem; comparator
 * STRUCTURALIS et FIDELIS (positiones ex cursore materiae, qui
 * lineas novas intra valores numerat); emissio arboris lectae ==
 * fons. Super fixa XIV et casus inlineares.
 */

#include "latina.h"
#include "credo.h"
#include "briar_arbor.h"
#include "briar_stml.h"
#include "briar_lexicon.h"
#include "materia_arbor.h"
#include "materia_nodus.h"
#include "materia_lexicon.h"
#include "piscina.h"
#include <stdio.h>
#include <string.h>

hic_manens constans character* FIXA[] = {
    "briar/probationes/fixa/thistle/salve.thistle",
    "briar/probationes/fixa/thistle/salve_vitreum.thistle",
    "briar/probationes/fixa/thistle/adversa/non_clausa.thistle",
    "briar/probationes/fixa/thistle/adversa/clausum_vagum.thistle",
    "briar/probationes/fixa/thistle/adversa/minor_in_prosa.thistle",
    "briar/probationes/fixa/thistle/adversa/vacuae.thistle",
    "briar/probationes/fixa/thistle/adversa/sine_interprete.thistle",
    "briar/probationes/fixa/thistle/adversa/saeptum_html.thistle",
    "briar/probationes/fixa/thistle/adversa/saeptum_apertum.thistle",
    "briar/probationes/fixa/thistle/adversa/vacuum.thistle",
    "briar/probationes/fixa/thistle/adversa/sine_linea_finali.thistle",
    "briar/probationes/fixa/thistle/adversa/crlf.thistle",
    "briar/probationes/fixa/thistle/adversa/elementum_non_clausum.thistle",
    "briar/probationes/fixa/thistle/adversa/elementum_multilineare.thistle"
};

hic_manens constans character* INLINEARES[] = {
    "",
    "\n",
    "#!/usr/bin/env briar\n",
    "<c!>\n</c>\n",
    "<c!>\nno close",
    "a\n<x/>\nb\n",
    "<c! a=\"1\">\n<b>\n</c>\n<d>\n</d>\n"
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

interior b32
_circuitum_probare (
                           Piscina* piscina,
    constans MateriaArborConsilium* consilium,
                constans character* fons,
                               i32  mensura)
{
              MateriaNodus* radix;
              MateriaNodus* lecta;
              MateriaNodus* relecta;
      MateriaArborScriptura s1;
      MateriaArborScriptura s2;
      MateriaArborScriptura s3;
         MateriaArborVitium vitium;
    MateriaArborDifferentia d;
           MateriaScriptura e;

    radix = briar_arbor_parsare(piscina, fons, mensura);
    si (radix == NIHIL)
    {
        imprimere("    (parsura NIHIL)\n");
        redde FALSUM;
    }
    s1 = materia_arbor_scribere_nodum(piscina, radix, consilium);
    si (!s1.successus)
    {
        imprimere("    (scriptura I: %s)\n", s1.causa ? s1.causa : "-");
        redde FALSUM;
    }
    lecta = materia_arbor_legere(piscina, NIHIL, s1.textus, consilium,
        &vitium);
    si (lecta == NIHIL)
    {
        imprimere("    (lectio I: %s, linea %d)\n",
            vitium.causa ? vitium.causa : "-", (integer)vitium.linea);
        redde FALSUM;
    }
    s2 = materia_arbor_scribere_nodum(piscina, lecta, consilium);
    si (   !s2.successus || s1.textus.mensura != s2.textus.mensura
        || memcmp(s1.textus.datum, s2.textus.datum,
               (size_t)s1.textus.mensura) != ZEPHYRUM)
    {
        imprimere("    (documenta I/II dispari)\n");
        redde FALSUM;
    }
    relecta = materia_arbor_legere(piscina, NIHIL, s2.textus, consilium,
        &vitium);
    s3 = materia_arbor_scribere_nodum(piscina, relecta, consilium);
    si (   relecta           == NIHIL || !s3.successus
        || s1.textus.mensura != s3.textus.mensura
        || memcmp(s1.textus.datum, s3.textus.datum,
               (size_t)s1.textus.mensura) != ZEPHYRUM)
    {
        imprimere("    (circuitus II dispar)\n");
        redde FALSUM;
    }
    /* emissio arboris LECTAE == fons */
    e = briar_emittere(piscina, lecta);
    si (   !e.successus || e.textus.mensura != mensura
        || (mensura > ZEPHYRUM
            && memcmp(e.textus.datum, fons, (size_t)mensura)
                != ZEPHYRUM))
    {
        imprimere("    (emissio lectae dispar)\n");
        redde FALSUM;
    }
    si (!materia_arbor_aequalis(radix, lecta,
        MATERIA_ARBOR_COMPARATIO_STRUCTURALIS, &d))
    {
        imprimere("    (STRUCTURALIS: %s)\n",
            d.campus ? d.campus : "-");
        redde FALSUM;
    }
    si (!materia_arbor_aequalis(radix, lecta,
        MATERIA_ARBOR_COMPARATIO_FIDELITAS, &d))
    {
        imprimere("    (FIDELITAS: %s)\n", d.campus ? d.campus : "-");
        redde FALSUM;
    }
    redde VERUM;
}

s32
principale (vacuum)
{
                      b32  praeteritus;
                  Piscina* piscina;
      MateriaLexiconRatum  ratum;
       MateriaLexIudicium  iudicium;
    MateriaArborConsilium  consilium;
                      i32  i;

    piscina = piscina_generare_dynamicum("probatio_briar_stml",
        4194304);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    CREDO_VERUM (materia_lexicon_ratum_facere(&ratum, &BRIAR_LEXICON,
        &iudicium));
    briar_stml_consilium(&consilium, &ratum);

    imprimere("\n--- Probans casus inlineares ---\n");
    per (i = ZEPHYRUM; i < (i32)(magnitudo(INLINEARES)
        / magnitudo(INLINEARES[0])); i++)
    {
        b32 bene = _circuitum_probare(piscina, &consilium,
            INLINEARES[i],
            (i32)strlen(INLINEARES[i]));

        si (!bene)
        {
            imprimere("  FRACTA inlinearis %d\n", (integer)i);
        }
        CREDO_VERUM (bene);
    }

    imprimere("\n--- Probans fixa ---\n");
    per (i = ZEPHYRUM; i < (i32)(magnitudo(FIXA)
        / magnitudo(FIXA[0])); i++)
    {
        character* textus;
              i32  mensura = ZEPHYRUM;
              b32  bene;

        textus = _plagulam_legere(piscina, FIXA[i], &mensura);
        CREDO_NON_NIHIL (textus);
        si (textus == NIHIL)
        {
            perge;
        }
        bene = _circuitum_probare(piscina, &consilium, textus, mensura);
        si (!bene)
        {
            imprimere("  FRACTA: %s\n", FIXA[i]);
        }
        CREDO_VERUM (bene);
    }

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
