/* probatio_briar_arbor.c - Arbor thistle: partes, regiones, elementa,
 * interpres, vitia; emissio == fons octetim super fixa omnia.
 *
 * Casus adversi in fixa/thistle/adversa: regio non clausa (contentum
 * usque ad finem, vitium I), clausura vaga (prosa), '<' sine nomine
 * (prosa), regiones vacuae, sine interprete, saeptum cum tag (prosa),
 * saeptum apertum (vitium III documenti), plagula vacua, sine linea
 * finali, CRLF, elementum non clausum (vitium II), elementum
 * multilineare.
 */

#include "latina.h"
#include "credo.h"
#include "briar_arbor.h"
#include "briar_registrum.h"
#include "briar_lexicon.h"
#include "materia_nodus.h"
#include "materia_token.h"
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
_emissio_idem (
                  Piscina* piscina,
    constans MateriaNodus* radix,
       constans character* fons,
                      i32  mensura)
{
    MateriaScriptura s = briar_emittere(piscina, radix);

    si (!s.successus)
    {
        imprimere("    (emissio fracta: %s)\n",
            s.causa ? s.causa : "-");
        redde FALSUM;
    }
    si (s.textus.mensura != mensura)
    {
        imprimere("    (emissio %d octeti contra %d)\n",
            (integer)s.textus.mensura, (integer)mensura);
        redde FALSUM;
    }
    redde (b32)(mensura == ZEPHYRUM
        || memcmp(s.textus.datum, fons, (size_t)mensura) == ZEPHYRUM);
}

interior b32
_titulus_est (
                chorda  c,
    constans character* literae)
{
    redde (b32)(c.mensura == (i32)strlen(literae)
        && memcmp(c.datum, literae, (size_t)c.mensura) == ZEPHYRUM);
}

interior b32
_valor_est (
    constans MateriaToken* t,
       constans character* literae)
{
    redde (b32)(t != NIHIL && _titulus_est(t->valor, literae));
}

/* genera partium ut litterae: p=prosa r=regio e=elementum */
interior vacuum
_forma (
    constans MateriaNodus* doc,
                character* exitus)
{
    i32 n = briar_partes_numerus(doc);
    i32 i;

    per (i = ZEPHYRUM; i < n; i++)
    {
        constans MateriaNodus* p = briar_pars(doc, i);

        exitus[i] = (p->genus == (s32)BRIAR_GENUS_PROSA) ? 'p'
                  : (p->genus == (s32)BRIAR_GENUS_REGIO) ? 'r' : 'e';
    }
    exitus[n] = '\0';
}

s32
principale (vacuum)
{
          b32  praeteritus;
      Piscina* piscina;
    character  forma[64];

    piscina = piscina_generare_dynamicum("probatio_briar_arbor",
        4194304);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    imprimere("\n--- Probans salve.thistle ---\n");
    {
        character* textus;
              i32  mensura = ZEPHYRUM;
        MateriaNodus* doc;
        MateriaNodus* r;

        textus = _plagulam_legere(piscina, FIXA[0], &mensura);
        CREDO_NON_NIHIL (textus);
        doc = briar_arbor_parsare(piscina, textus, mensura);
        CREDO_NON_NIHIL (doc);
        _forma(doc, forma);
        CREDO_VERUM (strcmp(forma, "prpr") == ZEPHYRUM);
        CREDO_AEQUALIS_S32 ((s32)doc->loci[BRIAR_DOCUMENTUM_INTERPRES].genus,
            (s32)MATERIA_VALOR_NODUS);
        CREDO_VERUM (_valor_est(briar_lexema(
            doc->loci[BRIAR_DOCUMENTUM_INTERPRES].datum.nodus,
            (i32)BRIAR_INTERPRES_TOK), "#!/usr/bin/env briar\n"));
        r = briar_pars(doc, I);
        CREDO_VERUM (_titulus_est(briar_titulus(r), "c"));
        CREDO_VERUM (_valor_est(briar_lexema(r,
            (i32)BRIAR_REGIO_APERTUM),
            "<c!>\n"));
        CREDO_VERUM (_valor_est(briar_lexema(r,
            (i32)BRIAR_REGIO_CLAUSUM),
            "</c>\n"));
        CREDO_CHORDA_INCIPIT (briar_lexema(r,
            (i32)BRIAR_REGIO_CONTENTUM)->valor,
            chorda_ex_literis("#include \"chorda.h\"\n", piscina));
        CREDO_AEQUALIS_S32 (briar_vitium(r), (s32)BRIAR_VITIUM_NULLUM);
        CREDO_AEQUALIS_S32 (briar_vitium(doc),
            (s32)BRIAR_VITIUM_NULLUM);
        r = briar_pars(doc, III);
        CREDO_VERUM (_valor_est(briar_lexema(r,
            (i32)BRIAR_REGIO_APERTUM),
            "<c! munus=\"probatio\">\n"));
        CREDO_VERUM (_valor_est(briar_lexema(briar_pars(doc, II),
            (i32)BRIAR_PROSA_TOK), "\n"));
        CREDO_NON_NIHIL (briar_lexema(doc,
            (i32)BRIAR_DOCUMENTUM_FINIS));
        CREDO_VERUM (_emissio_idem(piscina, doc, textus, mensura));
    }

    imprimere("\n--- Probans salve_vitreum.thistle ---\n");
    {
        character* textus;
              i32  mensura = ZEPHYRUM;
        MateriaNodus* doc;
        MateriaNodus* e;

        textus = _plagulam_legere(piscina, FIXA[1], &mensura);
        CREDO_NON_NIHIL (textus);
        doc = briar_arbor_parsare(piscina, textus, mensura);
        CREDO_NON_NIHIL (doc);
        _forma(doc, forma);
        CREDO_VERUM (strcmp(forma, "peprprpr") == ZEPHYRUM);
        e = briar_pars(doc, I);
        CREDO_VERUM (_titulus_est(briar_titulus(e), "fenestra"));
        CREDO_VERUM (_valor_est(briar_lexema(e,
            (i32)BRIAR_ELEMENTUM_TOK),
            "<fenestra titulus=\"salve\" latitudo=\"640\" altitudo=\"400\"/>\n"));
        CREDO_VERUM (_titulus_est(briar_titulus(briar_pars(doc, III)),
            "html"));
        CREDO_VERUM (_titulus_est(briar_titulus(briar_pars(doc,
            (i32)5)), "c"));
        /* prosa prima saeptum ```html cum <button> continet */
        CREDO_CHORDA_CONTINET (briar_lexema(briar_pars(doc, ZEPHYRUM),
            (i32)BRIAR_PROSA_TOK)->valor,
            chorda_ex_literis("```html\n<button", piscina));
        CREDO_VERUM (_emissio_idem(piscina, doc, textus, mensura));
    }

    imprimere("\n--- Probans casus inlineares ---\n");
    {
        MateriaNodus* doc;

        doc = briar_arbor_parsare(piscina, "", ZEPHYRUM);
        CREDO_NON_NIHIL (doc);
        CREDO_AEQUALIS_I32 (briar_partes_numerus(doc), ZEPHYRUM);
        CREDO_NON_NIHIL (briar_lexema(doc,
            (i32)BRIAR_DOCUMENTUM_FINIS));
        CREDO_VERUM (_emissio_idem(piscina, doc, "", ZEPHYRUM));

        doc = briar_arbor_parsare(piscina, "<c!/>\nx\n", (i32)8);
        _forma(doc, forma);
        CREDO_VERUM (strcmp(forma, "rp") == ZEPHYRUM);
        CREDO_NIHIL (briar_lexema(briar_pars(doc, ZEPHYRUM),
            (i32)BRIAR_REGIO_CONTENTUM));
        CREDO_VERUM (_emissio_idem(piscina, doc, "<c!/>\nx\n", (i32)8));
    }

    imprimere("\n--- Probans adversa ---\n");
    {
        character* textus;
              i32  mensura;
        MateriaNodus* doc;
        MateriaNodus* r;

        /* regio non clausa: contentum usque ad finem, clausum absens */
        textus  = _plagulam_legere(piscina, FIXA[2], &mensura);
        doc     = briar_arbor_parsare(piscina, textus, mensura);
        _forma(doc, forma);
        CREDO_VERUM (strcmp(forma, "r") == ZEPHYRUM);
        r = briar_pars(doc, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (briar_vitium(r),
            (s32)BRIAR_VITIUM_REGIO_NON_CLAUSA);
        CREDO_NIHIL (briar_lexema(r, (i32)BRIAR_REGIO_CLAUSUM));
        CREDO_VERUM (_valor_est(briar_lexema(r,
            (i32)BRIAR_REGIO_CONTENTUM),
            "int x;\n"));

        /* clausura vaga = prosa una */
        textus  = _plagulam_legere(piscina, FIXA[3], &mensura);
        doc     = briar_arbor_parsare(piscina, textus, mensura);
        _forma(doc, forma);
        CREDO_VERUM (strcmp(forma, "p") == ZEPHYRUM);

        /* '<' sine nomine, indentatum, digitus: prosa una */
        textus  = _plagulam_legere(piscina, FIXA[4], &mensura);
        doc     = briar_arbor_parsare(piscina, textus, mensura);
        _forma(doc, forma);
        CREDO_VERUM (strcmp(forma, "p") == ZEPHYRUM);

        /* regiones vacuae: regio sine contento, elementum, regio auto */
        textus  = _plagulam_legere(piscina, FIXA[5], &mensura);
        doc     = briar_arbor_parsare(piscina, textus, mensura);
        _forma(doc, forma);
        CREDO_VERUM (strcmp(forma, "rer") == ZEPHYRUM);
        CREDO_NIHIL (briar_lexema(briar_pars(doc, ZEPHYRUM),
            (i32)BRIAR_REGIO_CONTENTUM));
        CREDO_NON_NIHIL (briar_lexema(briar_pars(doc, ZEPHYRUM),
            (i32)BRIAR_REGIO_CLAUSUM));
        CREDO_VERUM (_titulus_est(briar_titulus(briar_pars(doc, I)),
            "x"));
        CREDO_NIHIL (briar_lexema(briar_pars(doc, II),
            (i32)BRIAR_REGIO_CLAUSUM));
        CREDO_AEQUALIS_S32 (briar_vitium(briar_pars(doc, II)),
            (s32)BRIAR_VITIUM_NULLUM);

        /* sine interprete */
        textus  = _plagulam_legere(piscina, FIXA[6], &mensura);
        doc     = briar_arbor_parsare(piscina, textus, mensura);
        CREDO_AEQUALIS_S32 ((s32)doc->loci[BRIAR_DOCUMENTUM_INTERPRES].genus,
            (s32)MATERIA_VALOR_NIHIL);
        _forma(doc, forma);
        CREDO_VERUM (strcmp(forma, "pr") == ZEPHYRUM);

        /* saeptum cum tag: prosa, deinde regio vera */
        textus  = _plagulam_legere(piscina, FIXA[7], &mensura);
        doc     = briar_arbor_parsare(piscina, textus, mensura);
        _forma(doc, forma);
        CREDO_VERUM (strcmp(forma, "pr") == ZEPHYRUM);
        CREDO_VERUM (_titulus_est(briar_titulus(briar_pars(doc, I)),
            "html"));

        /* saeptum apertum in fine: vitium documenti */
        textus  = _plagulam_legere(piscina, FIXA[8], &mensura);
        doc     = briar_arbor_parsare(piscina, textus, mensura);
        _forma(doc, forma);
        CREDO_VERUM (strcmp(forma, "p") == ZEPHYRUM);
        CREDO_AEQUALIS_S32 (briar_vitium(doc),
            (s32)BRIAR_VITIUM_SAEPTUM_NON_CLAUSUM);

        /* sine linea finali: clausum sine terminatore */
        textus  = _plagulam_legere(piscina, FIXA[10], &mensura);
        doc     = briar_arbor_parsare(piscina, textus, mensura);
        CREDO_VERUM (_valor_est(briar_lexema(briar_pars(doc, ZEPHYRUM),
            (i32)BRIAR_REGIO_CLAUSUM), "</c>"));

        /* CRLF: terminatores in lexematibus, positio recta */
        textus  = _plagulam_legere(piscina, FIXA[11], &mensura);
        doc     = briar_arbor_parsare(piscina, textus, mensura);
        _forma(doc, forma);
        CREDO_VERUM (strcmp(forma, "r") == ZEPHYRUM);
        CREDO_VERUM (_valor_est(briar_lexema(briar_pars(doc, ZEPHYRUM),
            (i32)BRIAR_REGIO_CONTENTUM), "x\r\n"));
        CREDO_AEQUALIS_I32 (briar_lexema(briar_pars(doc, ZEPHYRUM),
            (i32)BRIAR_REGIO_CONTENTUM)->linea, III);

        /* elementum non clausum */
        textus  = _plagulam_legere(piscina, FIXA[12], &mensura);
        doc     = briar_arbor_parsare(piscina, textus, mensura);
        _forma(doc, forma);
        CREDO_VERUM (strcmp(forma, "e") == ZEPHYRUM);
        CREDO_AEQUALIS_S32 (briar_vitium(briar_pars(doc, ZEPHYRUM)),
            (s32)BRIAR_VITIUM_ELEMENTUM_NON_CLAUSUM);

        /* elementum multilineare, deinde prosa */
        textus  = _plagulam_legere(piscina, FIXA[13], &mensura);
        doc     = briar_arbor_parsare(piscina, textus, mensura);
        _forma(doc, forma);
        CREDO_VERUM (strcmp(forma, "ep") == ZEPHYRUM);
        CREDO_VERUM (_valor_est(briar_lexema(briar_pars(doc, ZEPHYRUM),
            (i32)BRIAR_ELEMENTUM_TOK),
            "<fenestra>\n  <a/>\n</fenestra>\n"));
        CREDO_VERUM (_valor_est(briar_lexema(briar_pars(doc, I),
            (i32)BRIAR_PROSA_TOK), "post\n"));
    }

    imprimere("\n--- Probans corpus fixturarum (emissio octetim) ---\n");
    {
        i32 i;
        i32 n = (i32)(magnitudo(FIXA) / magnitudo(FIXA[0]));

        per (i = ZEPHYRUM; i < n; i++)
        {
            character* textus;
                  i32  mensura = ZEPHYRUM;
            MateriaNodus* doc;

            textus = _plagulam_legere(piscina, FIXA[i], &mensura);
            si (textus == NIHIL)
            {
                imprimere("  absens: %s\n", FIXA[i]);
                CREDO_NON_NIHIL (textus);
                perge;
            }
            doc = briar_arbor_parsare(piscina, textus, mensura);
            CREDO_NON_NIHIL (doc);
            CREDO_VERUM (_emissio_idem(piscina, doc, textus, mensura));
        }
    }

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
