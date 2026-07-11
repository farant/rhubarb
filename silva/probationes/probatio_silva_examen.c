/* probatio_silva_examen.c - M4a chunk A: diagnosticum v2
 * (codex/severitas/positio/provisionale/socius; tabula codicum;
 * consilium project-specs/officina-m4a-spec.md chunk A) */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "silva_token.h"
#include "silva_nodus.h"
#include "silva_parsare.h"
#include "silva_commissio.h"
#include "silva_c89_oraculum.h"
#include "silva_tabulae_c89.h"
#include "silva_c89_semantica.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

interior SilvaParsura*
_parsare (Piscina* piscina, constans character* fons)
{
    redde silva_c89_parsare(piscina, "probatio_examen.c", fons,
        (i32)strlen(fons), NIHIL);
}

interior constans SemanticaDiagnosticum*
_diagnosticum_codicis (constans SilvaSemantica* sem, s32 codex)
{
    i32 i;
    i32 m = (i32)silva_c89_diagnostica_numerus(sem);

    per (i = ZEPHYRUM; i < m; i++)
    {
        constans SemanticaDiagnosticum* d =
            silva_c89_diagnosticum_per_indicem(sem, i);

        si (d != NIHIL && d->codex == codex)
        {
            redde d;
        }
    }
    redde NIHIL;
}

s32 principale (vacuum)
{
    Piscina* piscina;
    b32 praeteritus;

    piscina = piscina_generare_dynamicum("probatio_examen",
        16777216);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);

    /* ========================================================
     * I. Integritas tabulae: omnis codex causam + severitatem
     * validam habet (magnitudinem assertum staticum in .c
     * custodit; hic valores per additionem NIHIL-nodi)
     * ======================================================== */
    {
        SilvaParsura* parsura = _parsare(piscina,
            "static int probe;\n");
        SilvaSemantica* sem;
        s32 c;

        CREDO_NON_NIHIL (parsura);
        sem = silva_c89_semantica_analysare(piscina, parsura);
        CREDO_NON_NIHIL (sem);
        CREDO_AEQUALIS_I32 ((i32)silva_c89_diagnostica_numerus(sem),
            ZEPHYRUM);

        per (c = ZEPHYRUM; c < (s32)EXAMEN_CODEX_NUMERUS; c++)
        {
            constans SemanticaDiagnosticum* d;

            silva_c89_diagnosticum_addere(sem, NIHIL, c);
            d = silva_c89_diagnosticum_per_indicem(sem, (i32)c);
            CREDO_NON_NIHIL (d);
            si (d != NIHIL)
            {
                CREDO_VERUM (d->causa != NIHIL);
                CREDO_VERUM (d->severitas >= (s32)EXAMEN_VIOLATIO
                    && d->severitas <= (s32)EXAMEN_INFRA);
                CREDO_VERUM (d->codex == c);
                /* NIHIL nodus: positio absens, non provisionale */
                CREDO_AEQUALIS_I32 (d->linea, ZEPHYRUM);
                CREDO_VERUM (!d->provisionale);
                CREDO_VERUM (d->socius == NIHIL);
            }
        }
    }

    /* ========================================================
     * II. Positio materializata: identificator ignotus linea 2,
     * columna 16; via = titulus fontis
     * ======================================================== */
    {
        SilvaParsura* parsura = _parsare(piscina,
            "static int a = 1;\n"
            "static int q = mysterium;\n");
        SilvaSemantica* sem;
        constans SemanticaDiagnosticum* d;

        CREDO_NON_NIHIL (parsura);
        sem = silva_c89_semantica_analysare(piscina, parsura);
        CREDO_NON_NIHIL (sem);
        CREDO_AEQUALIS_I32 ((i32)silva_c89_diagnostica_numerus(sem),
            I);
        d = _diagnosticum_codicis(sem,
            (s32)EXAMEN_CODEX_IDENTIFICATOR_IGNOTUS);
        CREDO_NON_NIHIL (d);
        si (d != NIHIL)
        {
            CREDO_VERUM (d->severitas == (s32)EXAMEN_VIOLATIO);
            CREDO_AEQUALIS_I32 (d->linea, II);
            CREDO_AEQUALIS_I32 (d->columna, XVI);
            CREDO_AEQUALIS_I32 (d->via.mensura,
                (i32)strlen("probatio_examen.c"));
            CREDO_VERUM (d->via.datum != NIHIL
                && memcmp(d->via.datum, "probatio_examen.c",
                       (memoriae_index)d->via.mensura) == ZEPHYRUM);
            CREDO_VERUM (!d->provisionale);
            CREDO_VERUM (d->socius == NIHIL);
        }
    }

    /* ========================================================
     * III. Macro: positio = RADIX (sedes usus, non definitio) -
     * praecedens indicii (silva_token_radix in additione)
     * ======================================================== */
    {
        SilvaParsura* parsura = _parsare(piscina,
            "#define ARCANUM mysterium\n"
            "static int q = ARCANUM;\n");
        SilvaSemantica* sem;
        constans SemanticaDiagnosticum* d;

        CREDO_NON_NIHIL (parsura);
        sem = silva_c89_semantica_analysare(piscina, parsura);
        CREDO_NON_NIHIL (sem);
        d = _diagnosticum_codicis(sem,
            (s32)EXAMEN_CODEX_IDENTIFICATOR_IGNOTUS);
        CREDO_NON_NIHIL (d);
        si (d != NIHIL)
        {
            CREDO_AEQUALIS_I32 (d->linea, II);
            CREDO_AEQUALIS_I32 (d->columna, XVI);
        }
    }

    /* ========================================================
     * IV. Provisionale: furca decl/expr irresoluta
     * ('Ignotum * x;' intra corpus) - canonicum = multiplicatio;
     * identificator ignotus flagrat sed PROVISIONALE
     * ======================================================== */
    {
        SilvaParsura* parsura = _parsare(piscina,
            "static void f(void) { Ignotum * x; }\n");
        SilvaSemantica* sem;
        constans SemanticaDiagnosticum* d;

        CREDO_NON_NIHIL (parsura);
        sem = silva_c89_semantica_analysare(piscina, parsura);
        CREDO_NON_NIHIL (sem);
        d = _diagnosticum_codicis(sem,
            (s32)EXAMEN_CODEX_IDENTIFICATOR_IGNOTUS);
        CREDO_NON_NIHIL (d);
        si (d != NIHIL)
        {
            CREDO_VERUM (d->provisionale);
        }
    }

    /* ========================================================
     * V. Socius: addere_cum_socio sedem cognatam fert
     * ======================================================== */
    {
        SilvaParsura* parsura = _parsare(piscina,
            "static int a;\n");
        SilvaSemantica* sem;
        constans SilvaNodus* nodus_a = NIHIL;
        constans SemanticaDiagnosticum* d;
        SilvaValor* e;
        i32 m;

        CREDO_NON_NIHIL (parsura);
        sem = silva_c89_semantica_analysare(piscina, parsura);
        CREDO_NON_NIHIL (sem);
        e = silva_valor_lista_obtinere(parsura->commissio->radix,
            ZEPHYRUM);
        CREDO_NON_NIHIL (e);
        si (e != NIHIL && e->genus == SILVA_VALOR_NODUS)
        {
            nodus_a = e->datum.nodus;
        }
        CREDO_NON_NIHIL (nodus_a);
        silva_c89_diagnosticum_addere_cum_socio(sem, nodus_a,
            (s32)EXAMEN_CODEX_REDECLARATIO_GENERIS, nodus_a);
        m = (i32)silva_c89_diagnostica_numerus(sem);
        CREDO_VERUM (m > ZEPHYRUM);
        d = silva_c89_diagnosticum_per_indicem(sem, m - I);
        CREDO_NON_NIHIL (d);
        si (d != NIHIL)
        {
            CREDO_VERUM (d->socius == nodus_a);
            CREDO_VERUM (d->linea > ZEPHYRUM);
        }
    }

    /* ========================================================
     * VI. Sedes NIHIL sanatae: specificatores mixti positionem
     * ferunt (nodus primus listae)
     * ======================================================== */
    {
        SilvaParsura* parsura = _parsare(piscina,
            "typedef int T;\n"
            "static T int x;\n");
        SilvaSemantica* sem;
        constans SemanticaDiagnosticum* d;

        CREDO_NON_NIHIL (parsura);
        sem = silva_c89_semantica_analysare(piscina, parsura);
        CREDO_NON_NIHIL (sem);
        d = _diagnosticum_codicis(sem,
            (s32)EXAMEN_CODEX_SPECIFICATORES_MIXTI);
        si (d != NIHIL)
        {
            CREDO_AEQUALIS_I32 (d->linea, II);
        }
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    redde I;
}
