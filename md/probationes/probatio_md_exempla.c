/* probatio_md_exempla.c - Lector spec.txt + exempla per portam octetorum
 *
 * PINNAE: octeti et numerus exemplorum utriusque plagulae (FONTES.md) -
 * re-vendere = mutatio nominata hic. Tabulae '→' -> '\t' asseruntur
 * (exemplum I: "\tfoo\tbaz\t\tbim"). Deinde OMNIS fons exempli
 * (DCLII + DCLXXII) per parsuram -> emissionem octetim: exempla
 * adversaria CommonMark sunt, plagulae domus non.
 */

#include "latina.h"
#include "credo.h"
#include "md_exempla.h"
#include "md_arbor.h"
#include "md_registrum.h"
#include "md_lexicon.h"
#include "materia_nodus.h"
#include "materia_scribere.h"
#include "piscina.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
_aequalis (
                chorda  c,
    constans character* litterae)
{
    redde (b32)(c.mensura == (i32)strlen(litterae)
                && memcmp(c.datum, litterae, (size_t)c.mensura)
                    == ZEPHYRUM);
}

/* parsura -> emissio (fons ZEPHYRUM) -> memcmp */
interior b32
_octeti (
    Piscina* piscina,
     chorda  fons)
{
              MateriaNodus* radix;
           MateriaScriptura emissa;
    MateriaScripturaConsilium consilium;

    radix = md_arbor_parsare(piscina, (constans character*)fons.datum,
        fons.mensura);
    si (radix == NIHIL)
    {
        redde FALSUM;
    }
    materia_scriptura_consilium_nudum(&consilium, &MD_REGISTRUM);
    consilium.fons_index = MD_FONS_PLAGULAE;
    emissa = materia_scribere_nodum(piscina, radix, &consilium);
    si (!emissa.successus || emissa.textus.mensura != fons.mensura)
    {
        redde FALSUM;
    }
    redde (b32)(fons.mensura == ZEPHYRUM
                || memcmp(emissa.textus.datum, fons.datum,
                       (size_t)fons.mensura) == ZEPHYRUM);
}

/* Plagulam legere, pinnas asserere, exempla per octetos. Reddit Xar
 * exemplorum aut NIHIL. */
interior Xar*
_plagula (
               Piscina* piscina,
    constans character* radix_viae,
    constans character* relativa,
                   i32  octeti_pinna,
                   i32  exempla_pinna)
{
    character  via[DXII];
    character* textus;
          i32  mensura = ZEPHYRUM;
          Xar* exempla;
          i32  n;
          i32  i;
          i32  fracta             = ZEPHYRUM;
          i32  cum_tabula         = ZEPHYRUM;
          s64  octeti_exemplorum  = ZEPHYRUM;

    sprintf(via, "%s/%s", radix_viae, relativa);
    textus = _plagulam_legere(piscina, via, &mensura);
    si (textus == NIHIL)
    {
        CREDO_CULPA ("plagula spec.txt absens (FONTES.md)");
        redde NIHIL;
    }
    imprimere("  %s: %d octeti\n", relativa, (integer)mensura);
    CREDO_AEQUALIS_I32 (mensura, octeti_pinna);
    exempla = md_exempla_legere(piscina, textus, mensura);
    CREDO_NON_NIHIL (exempla);
    si (exempla == NIHIL)
    {
        redde NIHIL;
    }
    n = xar_numerus(exempla);
    CREDO_AEQUALIS_I32 (n, exempla_pinna);
    per (i = ZEPHYRUM; i < n; i++)
    {
        constans MdExemplum* e =
            (constans MdExemplum*)xar_obtinere(exempla, i);
                    Piscina* p = piscina_generare_dynamicum("exemplum",
                        65536);

        CREDO_AEQUALIS_I32 (e->numerus, i + I);
        si (memchr(e->markdown.datum, '\t', (size_t)e->markdown.mensura)
            != NIHIL)
        {
            cum_tabula = cum_tabula + I;
        }
        octeti_exemplorum = octeti_exemplorum
            + (s64)e->markdown.mensura;
        si (!_octeti(p, e->markdown))
        {
            fracta = fracta + I;
            si (fracta <= X)
            {
                imprimere("    FRACTUM: exemplum %d (%.*s, linea %d)\n",
                    (integer)e->numerus, (integer)e->sectio.mensura,
                    (constans character*)e->sectio.datum,
                    (integer)e->linea);
            }
        }
        piscina_destruere(p);
    }
    imprimere("    exempla %d, cum tabulis %d, octeti fontium %ld, fracta %d\n",
        (integer)n, (integer)cum_tabula, (longus)octeti_exemplorum,
        (integer)fracta);
    CREDO_AEQUALIS_I32 (fracta, ZEPHYRUM);
    CREDO_MAIOR_I32 (cum_tabula, ZEPHYRUM);   /* '→' -> '\t' vivit */
    redde exempla;
}

s32
principale (vacuum)
{
                   b32  praeteritus;
               Piscina* piscina;
    constans character* radix_viae;
                   Xar* cm;
                   Xar* gfm;

    piscina = piscina_generare_dynamicum("probatio_md_exempla",
        1048576);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    radix_viae = getenv("RHUBARB_RADIX");
    si (radix_viae == NIHIL)
    {
        radix_viae = ".";
    }

    imprimere("\n--- Probans lectorem: forma exempli ---\n");
    {
        constans character* f =
            "# S\n\n## T\n\n"
            "```````````````````````````````` example\n"
            "→foo\n"
            ".\n"
            "<pre>\tfoo</pre>\n"
            "````````````````````````````````\n"
            "text\n"
            "```````````````````````````````` example\n"
            ".\n"
            "````````````````````````````````\n"
            "```````````````````````````````` example\n"
            "sine clausura\n";
        Xar* xs = md_exempla_legere(piscina, f, (i32)strlen(f));
        constans MdExemplum* e;

        CREDO_NON_NIHIL (xs);
        CREDO_AEQUALIS_I32 (xar_numerus(xs), II);   /* apertum sine clausura omissum */
        e = (constans MdExemplum*)xar_obtinere(xs, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (e->numerus, I);
        CREDO_VERUM (_aequalis(e->sectio, "T"));
        CREDO_VERUM (_aequalis(e->markdown, "\tfoo\n"));
        CREDO_VERUM (_aequalis(e->html, "<pre>\tfoo</pre>\n"));
        CREDO_AEQUALIS_I32 (e->linea, V);
        e = (constans MdExemplum*)xar_obtinere(xs, I);
        CREDO_AEQUALIS_I32 (e->markdown.mensura, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (e->html.mensura, ZEPHYRUM);
    }

    imprimere("\n--- CommonMark 0.31.2 ---\n");
    cm = _plagula(piscina, radix_viae,
        "md/probationes/fixa/commonmark/spec.txt",
        (i32)204857, (i32)652);
    si (cm != NIHIL)
    {
        constans MdExemplum* e = (constans MdExemplum*)xar_obtinere(cm,
            ZEPHYRUM);

        CREDO_VERUM (_aequalis(e->sectio, "Tabs"));
        CREDO_VERUM (_aequalis(e->markdown, "\tfoo\tbaz\t\tbim\n"));
        CREDO_VERUM (_aequalis(e->html,
            "<pre><code>foo\tbaz\t\tbim\n</code></pre>\n"));
    }

    imprimere("\n--- GFM 0.29 ---\n");
    gfm = _plagula(piscina, radix_viae,
        "md/probationes/fixa/gfm/spec.txt",
        (i32)216680, (i32)672);
    si (gfm != NIHIL)
    {
        i32 n = xar_numerus(gfm);
        i32 i;
        i32 tabulae         = ZEPHYRUM;
        i32 officia         = ZEPHYRUM;
        i32 deletiones      = ZEPHYRUM;
        i32 autonexus       = ZEPHYRUM;
        i32 cum_extensione  = ZEPHYRUM;

        per (i = ZEPHYRUM; i < n; i++)
        {
            constans MdExemplum* e =
                (constans MdExemplum*)xar_obtinere(gfm, i);

            si (_aequalis(e->sectio, "Tables (extension)")) tabulae++;
            si (_aequalis(e->sectio,
                                                                         "Task list items (extension)")) officia++;
            si (_aequalis(e->sectio,
                                                                       "Strikethrough (extension)")) deletiones++;
            si (_aequalis(e->sectio,
                                                                   "Autolinks (extension)")) autonexus++;
            si (e->extensio.mensura > ZEPHYRUM) cum_extensione++;
        }
        imprimere("    sectiones extensionum: tabulae %d, officia %d, deletiones %d,"
            " autonexus %d; cum extensione in limite %d\n",
            (integer)tabulae, (integer)officia, (integer)deletiones,
            (integer)autonexus, (integer)cum_extensione);
        CREDO_MAIOR_I32 (tabulae, ZEPHYRUM);
        CREDO_MAIOR_I32 (officia, ZEPHYRUM);
        CREDO_MAIOR_I32 (deletiones, ZEPHYRUM);
        CREDO_MAIOR_I32 (autonexus, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (cum_extensione, (i32)24);   /* DCLXXII - DCXLVIII */
    }

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
