/* lint_probatio.c - Linter for probatio files using arbor */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "arbor.h"
#include <stdio.h>
#include <string.h>

/* Check if a call to a specific function exists */
interior b32
_habet_vocationem(ArborNodus* radix, constans character* nomen_quaerendum, Piscina* piscina)
{
    Xar* liberi;
    i32 i;

    (vacuum)piscina;

    si (radix == NIHIL) redde FALSUM;

    si (radix->genus == ARBOR_NODUS_CALL_EXPRESSION)
    {
        ArborNodus* callee = radix->datum.vocatio.callee;
        si (callee != NIHIL && callee->genus == ARBOR_NODUS_IDENTIFIER)
        {
            chorda* valor = callee->datum.folium.valor;
            si (valor != NIHIL &&
                valor->mensura == (i32)strlen(nomen_quaerendum) &&
                memcmp(valor->datum, nomen_quaerendum, (size_t)valor->mensura) == ZEPHYRUM)
            {
                redde VERUM;
            }
        }
    }

    /* Check children based on node type */
    commutatio (radix->genus)
    {
        casus ARBOR_NODUS_TRANSLATION_UNIT:
        casus ARBOR_NODUS_EXPRESSION_STATEMENT:
        casus ARBOR_NODUS_FUNCTION_DECLARATOR:
        {
            liberi = radix->datum.genericum.liberi;
            si (liberi != NIHIL)
            {
                per (i = ZEPHYRUM; i < xar_numerus(liberi); i++)
                {
                    ArborNodus** slot = xar_obtinere(liberi, i);
                    si (_habet_vocationem(*slot, nomen_quaerendum, piscina)) redde VERUM;
                }
            }
            frange;
        }

        casus ARBOR_NODUS_FUNCTION_DEFINITION:
        {
            si (_habet_vocationem(radix->datum.functio.corpus, nomen_quaerendum, piscina))
                redde VERUM;
            frange;
        }

        casus ARBOR_NODUS_COMPOUND_STATEMENT:
        {
            Xar* sententiae = radix->datum.compositum.sententiae;
            si (sententiae != NIHIL)
            {
                per (i = ZEPHYRUM; i < xar_numerus(sententiae); i++)
                {
                    ArborNodus** slot = xar_obtinere(sententiae, i);
                    si (_habet_vocationem(*slot, nomen_quaerendum, piscina)) redde VERUM;
                }
            }
            frange;
        }

        casus ARBOR_NODUS_IF_STATEMENT:
        {
            si (_habet_vocationem(radix->datum.conditionale.conditio, nomen_quaerendum, piscina))
                redde VERUM;
            si (_habet_vocationem(radix->datum.conditionale.consequens, nomen_quaerendum, piscina))
                redde VERUM;
            si (_habet_vocationem(radix->datum.conditionale.alternans, nomen_quaerendum, piscina))
                redde VERUM;
            frange;
        }

        casus ARBOR_NODUS_DECLARATION:
        {
            Xar* decls = radix->datum.declaratio.declaratores;
            si (decls != NIHIL)
            {
                per (i = ZEPHYRUM; i < xar_numerus(decls); i++)
                {
                    ArborNodus** slot = xar_obtinere(decls, i);
                    si (_habet_vocationem(*slot, nomen_quaerendum, piscina)) redde VERUM;
                }
            }
            frange;
        }

        casus ARBOR_NODUS_INIT_DECLARATOR:
        {
            si (_habet_vocationem(radix->datum.init_decl.initializer, nomen_quaerendum, piscina))
                redde VERUM;
            frange;
        }

        casus ARBOR_NODUS_CALL_EXPRESSION:
        {
            Xar* args = radix->datum.vocatio.argumenta;
            si (args != NIHIL)
            {
                per (i = ZEPHYRUM; i < xar_numerus(args); i++)
                {
                    ArborNodus** slot = xar_obtinere(args, i);
                    si (_habet_vocationem(*slot, nomen_quaerendum, piscina)) redde VERUM;
                }
            }
            frange;
        }

        casus ARBOR_NODUS_BINARY_EXPRESSION:
        {
            si (_habet_vocationem(radix->datum.binarium.sinister, nomen_quaerendum, piscina))
                redde VERUM;
            si (_habet_vocationem(radix->datum.binarium.dexter, nomen_quaerendum, piscina))
                redde VERUM;
            frange;
        }

        casus ARBOR_NODUS_ASSIGNMENT_EXPRESSION:
        {
            si (_habet_vocationem(radix->datum.assignatio.target, nomen_quaerendum, piscina))
                redde VERUM;
            si (_habet_vocationem(radix->datum.assignatio.valor, nomen_quaerendum, piscina))
                redde VERUM;
            frange;
        }

        casus ARBOR_NODUS_RETURN_STATEMENT:
        {
            si (_habet_vocationem(radix->datum.reditio.valor, nomen_quaerendum, piscina))
                redde VERUM;
            frange;
        }

        ordinarius:
            frange;
    }

    redde FALSUM;
}

interior i32
_lint_filum(constans character* via, Piscina* piscina, InternamentumChorda* intern)
{
    ArborResultus res;
    i32 errores = ZEPHYRUM;

    imprimere("Linting: %s\n", via);

    /* Use default HYBRID mode - learns macros but doesn't expand them,
     * which handles __FILE__/__LINE__ gracefully */
    res = arbor_parsere_filum(via, piscina, intern, NIHIL);

    si (!res.successus)
    {
        imprimere("  FRACTA: Non potuit parsere filum\n");
        redde I;
    }

    /* Check for credo_aperire */
    si (!_habet_vocationem(res.radix, "credo_aperire", piscina))
    {
        imprimere("  ADMONITIO: credo_aperire() non inventa\n");
        errores++;
    }

    /* Check for credo_imprimere_compendium */
    si (!_habet_vocationem(res.radix, "credo_imprimere_compendium", piscina))
    {
        imprimere("  ADMONITIO: credo_imprimere_compendium() non inventa\n");
        errores++;
    }

    /* Check for credo_omnia_praeterierunt */
    si (!_habet_vocationem(res.radix, "credo_omnia_praeterierunt", piscina))
    {
        imprimere("  ADMONITIO: credo_omnia_praeterierunt() non inventa\n");
        errores++;
    }

    si (errores == ZEPHYRUM)
    {
        imprimere("  OK: Omnes credo functiones inventae\n");
    }

    redde errores;
}

s32 principale(s32 argc, character** argv)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    s32 i;
    s32 totalis_errores = ZEPHYRUM;

    si (argc < II)
    {
        imprimere("Usus: lint_probatio <filum.c> [filum2.c ...]\n");
        redde I;
    }

    piscina = piscina_generare_dynamicum("lint", MMMMXCVI * XVI);
    intern = internamentum_creare(piscina);

    per (i = I; i < argc; i++)
    {
        totalis_errores += _lint_filum(argv[i], piscina, intern);
    }

    imprimere("\n");
    si (totalis_errores == ZEPHYRUM)
    {
        imprimere("Linting completum: nullae admonitiones\n");
    }
    alioquin
    {
        imprimere("Linting completum: %d admonitiones\n", totalis_errores);
    }

    piscina_destruere(piscina);

    redde totalis_errores > ZEPHYRUM ? I : ZEPHYRUM;
}
