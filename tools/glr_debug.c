/* glr_debug.c - Quick GLR Parser Debug Tool
 *
 * Parses a string and shows result, errors, and debug info.
 */

#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "arbor2_lexema.h"
#include "arbor2_token.h"
#include "arbor2_expandere.h"
#include "arbor2_glr.h"
#include <stdio.h>
#include <string.h>

hic_manens Xar*
lexare(Piscina* p, InternamentumChorda* intern, constans character* input)
{
    Xar* tokens;
    Arbor2Lexator* lex;
    chorda* via;
    unio { constans character* c; i8* m; } u;

    tokens = xar_creare(p, magnitudo(Arbor2Token*));
    lex = arbor2_lexator_creare(p, intern, input, (i32)strlen(input));

    u.c = "<input>";
    via = piscina_allocare(p, magnitudo(chorda));
    via->datum = u.m;
    via->mensura = VII;

    dum (VERUM)
    {
        Arbor2Lexema* l = arbor2_lexema_proximum(lex);
        Arbor2Token* t;
        Arbor2Token** slot;

        si (l == NIHIL)
        {
            frange;
        }

        si (l->genus == ARBOR2_LEXEMA_NOVA_LINEA)
        {
            perge;
        }

        t = arbor2_token_ex_lexema(p, l, via, l->linea, l->columna);
        slot = xar_addere(tokens);
        *slot = t;

        si (l->genus == ARBOR2_LEXEMA_EOF)
        {
            frange;
        }
    }

    redde tokens;
}

hic_manens vacuum
imprimere_tokens(Xar* tokens)
{
    i32 i;
    i32 num;

    num = xar_numerus(tokens);
    printf("Tokens (%d):\n", num);

    per (i = ZEPHYRUM; i < num; i++)
    {
        Arbor2Token** t_ptr = xar_obtinere(tokens, i);
        Arbor2Token* t = *t_ptr;

        printf("  [%2d] %-16s", i, arbor2_lexema_genus_nomen(t->lexema->genus));

        si (t->lexema->valor.mensura > ZEPHYRUM)
        {
            printf(" = \"%.*s\"",
                   t->lexema->valor.mensura,
                   (constans character*)t->lexema->valor.datum);
        }

        printf("\n");
    }
}

hic_manens vacuum
imprimere_nodus(Arbor2Nodus* nodus, i32 depth)
{
    i32 j;

    si (nodus == NIHIL)
    {
        redde;
    }

    /* Indent */
    per (j = ZEPHYRUM; j < depth; j++)
    {
        printf("  ");
    }

    /* Node type */
    printf("%s", arbor2_nodus_genus_nomen(nodus->genus));

    /* Handle by node type */
    commutatio (nodus->genus)
    {
        casus ARBOR2_NODUS_IDENTIFICATOR:
        casus ARBOR2_NODUS_INTEGER:
            si (nodus->lexema != NIHIL && nodus->lexema->lexema != NIHIL)
            {
                chorda* val = &nodus->lexema->lexema->valor;
                si (val->mensura > ZEPHYRUM)
                {
                    printf(" \"%.*s\"", val->mensura, (constans character*)val->datum);
                }
            }
            printf("\n");
            frange;

        casus ARBOR2_NODUS_BINARIUM:
            si (nodus->lexema != NIHIL && nodus->lexema->lexema != NIHIL)
            {
                printf(" [%s]", arbor2_lexema_genus_nomen(nodus->lexema->lexema->genus));
            }
            printf("\n");
            imprimere_nodus(nodus->datum.binarium.sinister, depth + I);
            imprimere_nodus(nodus->datum.binarium.dexter, depth + I);
            frange;

        casus ARBOR2_NODUS_UNARIUM:
            si (nodus->lexema != NIHIL && nodus->lexema->lexema != NIHIL)
            {
                printf(" [%s]", arbor2_lexema_genus_nomen(nodus->lexema->lexema->genus));
            }
            printf("\n");
            imprimere_nodus(nodus->datum.unarium.operandum, depth + I);
            frange;

        casus ARBOR2_NODUS_TERNARIUS:
            printf(" [?:]\n");
            imprimere_nodus(nodus->datum.ternarius.conditio, depth + I);
            imprimere_nodus(nodus->datum.ternarius.verum, depth + I);
            imprimere_nodus(nodus->datum.ternarius.falsum, depth + I);
            frange;

        casus ARBOR2_NODUS_SUBSCRIPTIO:
            printf("\n");
            imprimere_nodus(nodus->datum.subscriptio.basis, depth + I);
            imprimere_nodus(nodus->datum.subscriptio.index, depth + I);
            frange;

        casus ARBOR2_NODUS_VOCATIO:
            printf("\n");
            imprimere_nodus(nodus->datum.vocatio.basis, depth + I);
            si (nodus->datum.vocatio.argumenta != NIHIL)
            {
                i32 num_args = xar_numerus(nodus->datum.vocatio.argumenta);
                i32 k;
                per (k = ZEPHYRUM; k < num_args; k++)
                {
                    Arbor2Nodus** arg = xar_obtinere(nodus->datum.vocatio.argumenta, k);
                    imprimere_nodus(*arg, depth + I);
                }
            }
            frange;

        casus ARBOR2_NODUS_MEMBRUM:
            printf(" %s \"%.*s\"\n",
                   nodus->datum.membrum.est_sagitta ? "->" : ".",
                   nodus->datum.membrum.membrum.mensura,
                   (constans character*)nodus->datum.membrum.membrum.datum);
            imprimere_nodus(nodus->datum.membrum.basis, depth + I);
            frange;

        ordinarius:
            printf("\n");
            frange;
    }
}

integer
principale(integer argc, constans character* constans* argv)
{
    Piscina* p;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;
    Xar* tokens;
    Arbor2GLRResultus result;
    constans character* input;
    i32 i;

    si (argc < II)
    {
        fprintf(stderr, "Usage: glr_debug <expression>\n");
        fprintf(stderr, "       glr_debug --validate\n");
        fprintf(stderr, "       glr_debug --validate-tags\n");
        fprintf(stderr, "Example: glr_debug \"a + b * c\"\n");
        redde I;
    }

    /* Check for --validate option */
    si (strcmp(argv[I], "--validate") == ZEPHYRUM)
    {
        b32 valida = arbor2_glr_validare_tabulas();
        redde valida ? ZEPHYRUM : I;
    }

    /* Check for --validate-tags option */
    si (strcmp(argv[I], "--validate-tags") == ZEPHYRUM)
    {
        b32 valida = arbor2_glr_validare_tags();
        redde valida ? ZEPHYRUM : I;
    }

    input = argv[I];

    /* Initialize */
    p = piscina_generare_dynamicum("glr_debug", 262144);
    intern = internamentum_creare(p);
    expansion = arbor2_expansion_creare(p, intern);
    glr = arbor2_glr_creare(p, intern, expansion);

    printf("=== Input: \"%s\" ===\n\n", input);

    /* Lex */
    tokens = lexare(p, intern, input);
    imprimere_tokens(tokens);
    printf("\n");

    /* Parse */
    printf("Parsing...\n");
    result = arbor2_glr_parsere_expressio(glr, tokens);

    printf("Result: %s\n\n", result.successus ? "SUCCESS" : "FAILED");

    /* Show errors */
    si (result.errores != NIHIL && xar_numerus(result.errores) > ZEPHYRUM)
    {
        printf("Errors (%d):\n", xar_numerus(result.errores));
        per (i = ZEPHYRUM; i < xar_numerus(result.errores); i++)
        {
            chorda** err = xar_obtinere(result.errores, i);
            si (err != NIHIL && *err != NIHIL)
            {
                printf("  %.*s\n", (*err)->mensura, (constans character*)(*err)->datum);
            }
        }
        printf("\n");
    }

    /* Show AST */
    si (result.successus && result.radix != NIHIL)
    {
        printf("AST:\n");
        imprimere_nodus(result.radix, I);
        printf("\n");
    }

    /* Show stats */
    printf("Parser stats:\n");
    printf("  Forks:     %d\n", glr->num_furcae);
    printf("  Merges:    %d\n", glr->num_mergae);
    printf("  Max front: %d\n", glr->max_frons);

    redde result.successus ? ZEPHYRUM : I;
}
