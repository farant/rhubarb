/* probatio_briar_nexus.c - Nexus: prosa -> arbor markdown, elementum
 * -> StmlNodus, regio -> attributa tagi; lineae .thistle; vitium STML
 * plantatum ad lineam .thistle translatum; <md!> ut prosa.
 */

#include "latina.h"
#include "credo.h"
#include "briar_arbor.h"
#include "briar_nexus.h"
#include "briar_registrum.h"
#include "internamentum.h"
#include "materia_nodus.h"
#include "md_registrum.h"
#include "piscina.h"
#include "stml.h"
#include "xar.h"
#include <stdio.h>
#include <string.h>

hic_manens constans character* VITREUM =
    "briar/probationes/fixa/thistle/salve_vitreum.thistle";

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

/* linea (I-basata) in qua 'acus' primum incipit in columna 0; ZEPHYRUM = nulla */
interior i32
_linea_ubi (
    constans character* fons,
                   i32  mensura,
    constans character* acus)
{
    i32 m      = (i32)strlen(acus);
    i32 linea  = I;
    i32 i;

    per (i = ZEPHYRUM; i + m <= mensura; i++)
    {
        si (   (i == ZEPHYRUM || fons[i - I] == '\n')
            && memcmp(fons + i, acus, (size_t)m) == ZEPHYRUM)
        {
            redde linea;
        }
        si (fons[i] == '\n')
        {
            linea = linea + I;
        }
    }
    redde ZEPHYRUM;
}

interior b32
_chorda_est (
                chorda  c,
    constans character* literae)
{
    redde (b32)(c.mensura == (i32)strlen(literae)
        && (c.mensura == ZEPHYRUM
            || memcmp(c.datum, literae, (size_t)c.mensura)
                == ZEPHYRUM));
}

interior BriarNexusRes*
_res (
    Xar* nexus,
    i32  i)
{
    redde (BriarNexusRes*)xar_obtinere(nexus, i);
}

s32
principale (vacuum)
{
                  b32  praeteritus;
              Piscina* piscina;
  InternamentumChorda* intern;

    piscina = piscina_generare_dynamicum("probatio_briar_nexus",
        8388608);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);

    imprimere("\n--- Probans salve_vitreum: octo partes, tria genera ---\n");
    {
           character* textus;
                 i32  mensura = ZEPHYRUM;
        MateriaNodus* doc;
                 Xar* nexus;
       BriarNexusRes* r;

        textus = _plagulam_legere(piscina, VITREUM, &mensura);
        CREDO_NON_NIHIL (textus);
        doc    = briar_arbor_parsare(piscina, textus, mensura);
        nexus  = briar_nexus_texere(piscina, doc, intern);
        CREDO_NON_NIHIL (nexus);
        CREDO_AEQUALIS_I32 (xar_numerus(nexus), (i32)8);

        /* prosa prima: arbor md cum capitulo */
        r = _res(nexus, ZEPHYRUM);
        CREDO_AEQUALIS_S32 ((s32)r->genus, (s32)BRIAR_NEXUS_MD);
        CREDO_NON_NIHIL (r->arbor_md);
        CREDO_AEQUALIS_S32 (r->arbor_md->genus,
            (s32)MD_GENUS_DOCUMENTUM);
        CREDO_AEQUALIS_I32 (r->linea_initium, II);
        CREDO_AEQUALIS_I32 (r->linea_erroris, ZEPHYRUM);

        /* fenestra: attributa tria */
        r = _res(nexus, I);
        CREDO_AEQUALIS_S32 ((s32)r->genus, (s32)BRIAR_NEXUS_STML);
        CREDO_VERUM (briar_nexus_titulus_est(r, "fenestra"));
        CREDO_NON_NIHIL (r->elementum);
        CREDO_VERUM (_chorda_est(briar_nexus_attributum(r, "titulus"),
            "salve"));
        CREDO_VERUM (_chorda_est(briar_nexus_attributum(r, "latitudo"),
            "640"));
        CREDO_VERUM (_chorda_est(briar_nexus_attributum(r, "altitudo"),
            "400"));
        CREDO_FALSUM (briar_nexus_attributum_habet(r, "profunditas"));
        CREDO_AEQUALIS_I32 (r->linea_initium,
            _linea_ubi(textus, mensura, "<fenestra"));

        /* html: regio sine attributis */
        r = _res(nexus, III);
        CREDO_AEQUALIS_S32 ((s32)r->genus, (s32)BRIAR_NEXUS_REGIO);
        CREDO_VERUM (briar_nexus_titulus_est(r, "html"));
        CREDO_NON_NIHIL (r->elementum);
        CREDO_FALSUM (briar_nexus_attributum_habet(r, "methodus"));
        CREDO_CHORDA_INCIPIT (r->contentum, chorda_ex_literis("<h1>",
            piscina));
        CREDO_AEQUALIS_I32 (r->linea_initium,
            _linea_ubi(textus, mensura, "<html!>") + I);

        /* c methodus="salve" */
        r = _res(nexus, (i32)5);
        CREDO_AEQUALIS_S32 ((s32)r->genus, (s32)BRIAR_NEXUS_REGIO);
        CREDO_VERUM (briar_nexus_titulus_est(r, "c"));
        CREDO_VERUM (_chorda_est(briar_nexus_attributum(r, "methodus"),
            "salve"));
        CREDO_AEQUALIS_I32 (r->linea_initium,
            _linea_ubi(textus, mensura, "<c! methodus=") + I);

        /* c munus="probatio" */
        r = _res(nexus, (i32)7);
        CREDO_VERUM (_chorda_est(briar_nexus_attributum(r, "munus"),
            "probatio"));
        CREDO_VERUM (_chorda_est(briar_nexus_attributum(r, "methodus"),
            ""));
    }

    imprimere("\n--- Probans casus inlineares ---\n");
    {
          MateriaNodus* doc;
                   Xar* nexus;
         BriarNexusRes* r;
    constans character* a = "<md!>\n# T\n</md>\n";
    constans character* b = "prosa\n<fenestra>\n<a>\n</fenestra>\n";
    constans character* c = "<c!\\ methodus=\"m\">\n  x\n</c>\n";
    constans character* d = "<c! a=\"x>\nx\n</c>\n";

        /* regio 'md' = prosa explicita, linea contenti II */
        doc    = briar_arbor_parsare(piscina, a, (i32)strlen(a));
        nexus  = briar_nexus_texere(piscina, doc, intern);
        r      = _res(nexus, ZEPHYRUM);
        CREDO_AEQUALIS_S32 ((s32)r->genus, (s32)BRIAR_NEXUS_MD);
        CREDO_NON_NIHIL (r->arbor_md);
        CREDO_AEQUALIS_I32 (r->linea_initium, II);

        /* vitium STML plantatum: <a> non clausum intra elementum lineae II
         * -> linea erroris .thistle >= II, causa non vacua, elementum NIHIL */
        doc    = briar_arbor_parsare(piscina, b, (i32)strlen(b));
        nexus  = briar_nexus_texere(piscina, doc, intern);
        CREDO_AEQUALIS_I32 (xar_numerus(nexus), II);
        r = _res(nexus, I);
        CREDO_AEQUALIS_S32 ((s32)r->genus, (s32)BRIAR_NEXUS_STML);
        CREDO_NIHIL (r->elementum);
        CREDO_MAIOR_AUT_AEQUALIS_I32 (r->linea_erroris, II);
        CREDO_MINOR_AUT_AEQUALIS_I32 (r->linea_erroris, (i32)4);
        CREDO_CHORDA_NON_VACUA (r->causa);

        /* '\' dedens: attributa tamen leguntur */
        doc    = briar_arbor_parsare(piscina, c, (i32)strlen(c));
        nexus  = briar_nexus_texere(piscina, doc, intern);
        r      = _res(nexus, ZEPHYRUM);
        CREDO_VERUM (_chorda_est(briar_nexus_attributum(r, "methodus"),
            "m"));

        /* tag apertus malformatus (citatio non clausa - 'bad=' STML ut
         * valorem vacuum ACCIPIT, mensuratum): vitium nominatum, linea I */
        doc    = briar_arbor_parsare(piscina, d, (i32)strlen(d));
        nexus  = briar_nexus_texere(piscina, doc, intern);
        r      = _res(nexus, ZEPHYRUM);
        CREDO_NIHIL (r->elementum);
        CREDO_AEQUALIS_I32 (r->linea_erroris, I);
        CREDO_VERUM (_chorda_est(briar_nexus_attributum(r, "a"), ""));
    }

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
