#include "arbor_typus.h"
#include "arbor_lexema.h"
#include <stdio.h>

/* ==================================================
 * Resolver State Structure
 * ================================================== */

structura ArborTypusResolver {
    Piscina*              piscina;
    InternamentumChorda*  intern;

    /* Namespaces (C has separate namespaces) */
    TabulaDispersa*       typedef_nomina;   /* typedef names -> ArborTypus* */
    TabulaDispersa*       tag_nomina;       /* struct/union/enum tags -> ArborTypus* */

    /* Scope stack for locals/parameters */
    Xar*                  scopes;           /* Stack of TabulaDispersa* */

    /* Global scope (file scope) */
    TabulaDispersa*       globalia;

    /* Errors */
    Xar*                  errores;
};

/* ==================================================
 * Internal Prototypes
 * ================================================== */

interior ArborTypus* _creare_typus(ArborTypusResolver* res, ArborTypusGenus genus);
interior vacuum _aperire_scopum(ArborTypusResolver* res);
interior vacuum _claudere_scopum(ArborTypusResolver* res);
interior vacuum _registrare_symbolum(ArborTypusResolver* res, chorda* titulus, ArborTypus* typus, ArborNodus* decl);
interior ArborSymbolum* _quaerere_symbolum(ArborTypusResolver* res, chorda* titulus);
interior ArborTypus* _resolvere_specifiers(ArborTypusResolver* res, Xar* specifiers);
interior ArborTypus* _applicare_declarator(ArborTypusResolver* res, ArborTypus* basis, ArborNodus* decl);
interior vacuum _resolvere_declaratio(ArborTypusResolver* res, ArborNodus* decl);
interior vacuum _resolvere_functio(ArborTypusResolver* res, ArborNodus* func);
interior vacuum _resolvere_nodus(ArborTypusResolver* res, ArborNodus* nodus);

/* ==================================================
 * Type Creation
 * ================================================== */

interior ArborTypus*
_creare_typus(ArborTypusResolver* res, ArborTypusGenus genus)
{
    ArborTypus* t;

    t = piscina_allocare(res->piscina, magnitudo(ArborTypus));
    si (t == NIHIL)
    {
        redde NIHIL;
    }

    t->genus = genus;
    t->titulus = NIHIL;
    t->basis = NIHIL;
    t->array_mensura = (i32)-1;
    t->parametra = NIHIL;
    t->reditum = NIHIL;
    t->membra = NIHIL;
    t->est_const = FALSUM;
    t->est_volatile = FALSUM;
    t->est_signed = VERUM;
    t->est_unsigned = FALSUM;

    redde t;
}

interior ArborTypus*
_creare_pointer_typus(ArborTypusResolver* res, ArborTypus* basis)
{
    ArborTypus* t;

    t = _creare_typus(res, ARBOR_TYPUS_POINTER);
    si (t != NIHIL)
    {
        t->basis = basis;
    }

    redde t;
}

interior ArborTypus*
_creare_array_typus(ArborTypusResolver* res, ArborTypus* basis, i32 mensura)
{
    ArborTypus* t;

    t = _creare_typus(res, ARBOR_TYPUS_ARRAY);
    si (t != NIHIL)
    {
        t->basis = basis;
        t->array_mensura = mensura;
    }

    redde t;
}

interior ArborTypus*
_creare_function_typus(ArborTypusResolver* res, ArborTypus* reditum, Xar* parametra)
{
    ArborTypus* t;

    t = _creare_typus(res, ARBOR_TYPUS_FUNCTION);
    si (t != NIHIL)
    {
        t->reditum = reditum;
        t->parametra = parametra;
    }

    redde t;
}

/* ==================================================
 * Scope Management
 * ================================================== */

interior vacuum
_aperire_scopum(ArborTypusResolver* res)
{
    TabulaDispersa* scopum;
    TabulaDispersa** slot;

    scopum = tabula_dispersa_creare_chorda(res->piscina, LXIV);
    si (scopum == NIHIL)
    {
        redde;
    }

    slot = xar_addere(res->scopes);
    si (slot != NIHIL)
    {
        *slot = scopum;
    }
}

interior vacuum
_claudere_scopum(ArborTypusResolver* res)
{
    i32 n;

    n = xar_numerus(res->scopes);
    si (n > ZEPHYRUM)
    {
        /* Pop scopum - just remove last since arena allocated */
        xar_removere_ultimum(res->scopes);
    }
}

interior vacuum
_registrare_symbolum(ArborTypusResolver* res, chorda* titulus, ArborTypus* typus, ArborNodus* decl)
{
    TabulaDispersa* scopum;
    TabulaDispersa** scopum_ptr;
    ArborSymbolum* sym;
    i32 n;

    si (titulus == NIHIL || typus == NIHIL)
    {
        redde;
    }

    /* Use current scope or global */
    n = xar_numerus(res->scopes);
    si (n > ZEPHYRUM)
    {
        scopum_ptr = xar_obtinere(res->scopes, n - I);
        scopum = *scopum_ptr;
    }
    alioquin
    {
        scopum = res->globalia;
    }

    /* Create symbol */
    sym = piscina_allocare(res->piscina, magnitudo(ArborSymbolum));
    si (sym == NIHIL)
    {
        redde;
    }

    sym->titulus = titulus;
    sym->typus = typus;
    sym->declaratio = decl;

    tabula_dispersa_inserere(scopum, *titulus, sym);
}

interior ArborSymbolum*
_quaerere_symbolum(ArborTypusResolver* res, chorda* titulus)
{
    i32 i;
    TabulaDispersa* scopum;
    TabulaDispersa** scopum_ptr;
    vacuum* valor;

    si (titulus == NIHIL)
    {
        redde NIHIL;
    }

    /* Search scopes from innermost to outermost */
    per (i = xar_numerus(res->scopes) - I; i >= ZEPHYRUM; i--)
    {
        scopum_ptr = xar_obtinere(res->scopes, i);
        scopum = *scopum_ptr;

        si (tabula_dispersa_invenire(scopum, *titulus, &valor))
        {
            redde (ArborSymbolum*)valor;
        }
    }

    /* Check global scope */
    si (tabula_dispersa_invenire(res->globalia, *titulus, &valor))
    {
        redde (ArborSymbolum*)valor;
    }

    redde NIHIL;
}

/* ==================================================
 * Specifier Resolution
 * ================================================== */

interior ArborTypus*
_resolvere_specifiers(ArborTypusResolver* res, Xar* specifiers)
{
    ArborTypus* typus;
    ArborTypusGenus base_genus;
    b32 est_signed;
    b32 est_unsigned;
    b32 est_short;
    b32 est_long;
    b32 est_const;
    b32 est_volatile;
    i32 i;
    i32 n;
    ArborNodus** spec_ptr;
    ArborNodus* spec;

    base_genus = ARBOR_TYPUS_INT;
    est_signed = FALSUM;
    est_unsigned = FALSUM;
    est_short = FALSUM;
    est_long = FALSUM;
    est_const = FALSUM;
    est_volatile = FALSUM;

    si (specifiers == NIHIL)
    {
        redde _creare_typus(res, ARBOR_TYPUS_INT);
    }

    n = xar_numerus(specifiers);
    per (i = ZEPHYRUM; i < n; i++)
    {
        spec_ptr = xar_obtinere(specifiers, i);
        si (spec_ptr == NIHIL || *spec_ptr == NIHIL)
        {
            perge;
        }
        spec = *spec_ptr;

        commutatio (spec->genus)
        {
            casus ARBOR_NODUS_TYPE_SPECIFIER:
                commutatio (spec->datum.folium.keyword)
                {
                    casus ARBOR_LEXEMA_VOID:
                        base_genus = ARBOR_TYPUS_VOID;
                        frange;
                    casus ARBOR_LEXEMA_CHAR:
                        base_genus = ARBOR_TYPUS_CHAR;
                        frange;
                    casus ARBOR_LEXEMA_SHORT:
                        est_short = VERUM;
                        frange;
                    casus ARBOR_LEXEMA_INT:
                        base_genus = ARBOR_TYPUS_INT;
                        frange;
                    casus ARBOR_LEXEMA_LONG:
                        est_long = VERUM;
                        frange;
                    casus ARBOR_LEXEMA_FLOAT:
                        base_genus = ARBOR_TYPUS_FLOAT;
                        frange;
                    casus ARBOR_LEXEMA_DOUBLE:
                        base_genus = ARBOR_TYPUS_DOUBLE;
                        frange;
                    casus ARBOR_LEXEMA_SIGNED:
                        est_signed = VERUM;
                        frange;
                    casus ARBOR_LEXEMA_UNSIGNED:
                        est_unsigned = VERUM;
                        frange;
                    ordinarius:
                        frange;
                }
                frange;

            casus ARBOR_NODUS_TYPE_QUALIFIER:
                si (spec->datum.folium.keyword == ARBOR_LEXEMA_CONST)
                {
                    est_const = VERUM;
                }
                alioquin si (spec->datum.folium.keyword == ARBOR_LEXEMA_VOLATILE)
                {
                    est_volatile = VERUM;
                }
                frange;

            casus ARBOR_NODUS_STRUCT_SPECIFIER:
                typus = _creare_typus(res, ARBOR_TYPUS_STRUCT);
                si (typus != NIHIL)
                {
                    typus->titulus = spec->datum.aggregatum.titulus;
                    /* TODO: resolve members */
                }
                typus->est_const = est_const;
                typus->est_volatile = est_volatile;
                redde typus;

            casus ARBOR_NODUS_UNION_SPECIFIER:
                typus = _creare_typus(res, ARBOR_TYPUS_UNION);
                si (typus != NIHIL)
                {
                    typus->titulus = spec->datum.aggregatum.titulus;
                }
                typus->est_const = est_const;
                typus->est_volatile = est_volatile;
                redde typus;

            casus ARBOR_NODUS_ENUM_SPECIFIER:
                typus = _creare_typus(res, ARBOR_TYPUS_ENUM);
                si (typus != NIHIL)
                {
                    typus->titulus = spec->datum.enum_spec.titulus;
                }
                typus->est_const = est_const;
                typus->est_volatile = est_volatile;
                redde typus;

            casus ARBOR_NODUS_TYPEDEF_NAME:
                {
                    ArborSymbolum* sym;
                    sym = _quaerere_symbolum(res, spec->datum.folium.valor);
                    si (sym != NIHIL)
                    {
                        typus = _creare_typus(res, ARBOR_TYPUS_TYPEDEF);
                        si (typus != NIHIL)
                        {
                            typus->titulus = spec->datum.folium.valor;
                            typus->basis = sym->typus;
                        }
                        redde typus;
                    }
                }
                frange;

            ordinarius:
                /* Skip storage class, extensions, etc. */
                frange;
        }
    }

    /* Determine final type based on specifier combinations */
    si (est_short)
    {
        base_genus = ARBOR_TYPUS_SHORT;
    }
    alioquin si (est_long)
    {
        base_genus = ARBOR_TYPUS_LONG;
    }

    typus = _creare_typus(res, base_genus);
    si (typus != NIHIL)
    {
        typus->est_const = est_const;
        typus->est_volatile = est_volatile;
        typus->est_signed = est_signed || (!est_unsigned);
        typus->est_unsigned = est_unsigned;
    }

    redde typus;
}

/* ==================================================
 * Declarator Type Building
 * ================================================== */

interior chorda*
_extrahere_declarator_nomen(ArborNodus* decl);

interior ArborTypus*
_applicare_declarator(ArborTypusResolver* res, ArborTypus* basis, ArborNodus* decl)
{
    i32 i;
    i32 n;
    ArborNodus** child_ptr;
    ArborNodus* child;

    si (decl == NIHIL)
    {
        redde basis;
    }

    /* Process children in order (pointers, identifier, arrays/functions) */
    si (decl->datum.genericum.liberi != NIHIL)
    {
        n = xar_numerus(decl->datum.genericum.liberi);
        per (i = ZEPHYRUM; i < n; i++)
        {
            child_ptr = xar_obtinere(decl->datum.genericum.liberi, i);
            si (child_ptr == NIHIL || *child_ptr == NIHIL)
            {
                perge;
            }
            child = *child_ptr;

            commutatio (child->genus)
            {
                casus ARBOR_NODUS_POINTER:
                    basis = _creare_pointer_typus(res, basis);
                    /* Apply const/volatile from pointer qualifiers */
                    si (basis != NIHIL && child->datum.pointer.qualifiers != NIHIL)
                    {
                        i32 j;
                        i32 nq;
                        nq = xar_numerus(child->datum.pointer.qualifiers);
                        per (j = ZEPHYRUM; j < nq; j++)
                        {
                            ArborNodus** q_ptr = xar_obtinere(child->datum.pointer.qualifiers, j);
                            si (q_ptr != NIHIL && *q_ptr != NIHIL)
                            {
                                ArborNodus* q = *q_ptr;
                                si (q->datum.folium.keyword == ARBOR_LEXEMA_CONST)
                                {
                                    basis->est_const = VERUM;
                                }
                                alioquin si (q->datum.folium.keyword == ARBOR_LEXEMA_VOLATILE)
                                {
                                    basis->est_volatile = VERUM;
                                }
                            }
                        }
                    }
                    frange;

                casus ARBOR_NODUS_ARRAY_DECLARATOR:
                    {
                        i32 mensura = (i32)-1;
                        /* Extract array size if present */
                        si (child->datum.array.size != NIHIL &&
                            child->datum.array.size->genus == ARBOR_NODUS_INTEGER_LITERAL)
                        {
                            mensura = (i32)child->datum.array.size->datum.numerus.valor;
                        }
                        basis = _creare_array_typus(res, basis, mensura);
                    }
                    frange;

                casus ARBOR_NODUS_FUNCTION_DECLARATOR:
                    {
                        Xar* param_types;
                        param_types = xar_creare(res->piscina, magnitudo(ArborTypus*));
                        /* TODO: resolve parameter types */
                        basis = _creare_function_typus(res, basis, param_types);
                    }
                    frange;

                casus ARBOR_NODUS_IDENTIFIER:
                    /* Skip - identifier is just the name */
                    frange;

                ordinarius:
                    frange;
            }
        }
    }

    redde basis;
}

/* ==================================================
 * Declaration Resolution
 * ================================================== */

interior chorda*
_extrahere_declarator_nomen(ArborNodus* decl)
{
    i32 i;
    i32 n;

    si (decl == NIHIL)
    {
        redde NIHIL;
    }

    si (decl->genus == ARBOR_NODUS_IDENTIFIER)
    {
        redde decl->datum.folium.valor;
    }

    si (decl->genus == ARBOR_NODUS_DECLARATOR && decl->datum.genericum.liberi != NIHIL)
    {
        n = xar_numerus(decl->datum.genericum.liberi);
        per (i = ZEPHYRUM; i < n; i++)
        {
            ArborNodus** child_ptr = xar_obtinere(decl->datum.genericum.liberi, i);
            si (child_ptr != NIHIL && *child_ptr != NIHIL)
            {
                chorda* result = _extrahere_declarator_nomen(*child_ptr);
                si (result != NIHIL)
                {
                    redde result;
                }
            }
        }
    }

    si (decl->genus == ARBOR_NODUS_INIT_DECLARATOR)
    {
        redde _extrahere_declarator_nomen(decl->datum.init_decl.declarator);
    }

    redde NIHIL;
}

interior vacuum
_resolvere_declaratio(ArborTypusResolver* res, ArborNodus* decl)
{
    ArborTypus* base_typus;
    i32 i;
    i32 n;
    ArborNodus** init_decl_ptr;
    ArborNodus* init_decl;
    ArborNodus* declarator;
    ArborTypus* final_typus;
    chorda* titulus;
    b32 est_typedef;

    si (decl == NIHIL || decl->genus != ARBOR_NODUS_DECLARATION)
    {
        redde;
    }

    /* Check for typedef */
    est_typedef = FALSUM;
    si (decl->datum.declaratio.specifiers != NIHIL)
    {
        n = xar_numerus(decl->datum.declaratio.specifiers);
        per (i = ZEPHYRUM; i < n; i++)
        {
            ArborNodus** spec_ptr = xar_obtinere(decl->datum.declaratio.specifiers, i);
            si (spec_ptr != NIHIL && *spec_ptr != NIHIL)
            {
                ArborNodus* spec = *spec_ptr;
                si (spec->genus == ARBOR_NODUS_STORAGE_CLASS &&
                    spec->datum.folium.keyword == ARBOR_LEXEMA_TYPEDEF)
                {
                    est_typedef = VERUM;
                    frange;
                }
            }
        }
    }

    /* Resolve base type from specifiers */
    base_typus = _resolvere_specifiers(res, decl->datum.declaratio.specifiers);
    si (base_typus == NIHIL)
    {
        redde;
    }

    /* Process each declarator */
    si (decl->datum.declaratio.declaratores == NIHIL)
    {
        redde;
    }

    n = xar_numerus(decl->datum.declaratio.declaratores);
    per (i = ZEPHYRUM; i < n; i++)
    {
        init_decl_ptr = xar_obtinere(decl->datum.declaratio.declaratores, i);
        si (init_decl_ptr == NIHIL || *init_decl_ptr == NIHIL)
        {
            perge;
        }
        init_decl = *init_decl_ptr;

        si (init_decl->genus == ARBOR_NODUS_INIT_DECLARATOR)
        {
            declarator = init_decl->datum.init_decl.declarator;
        }
        alioquin
        {
            declarator = init_decl;
        }

        /* Apply declarator modifiers to base type */
        final_typus = _applicare_declarator(res, base_typus, declarator);

        /* Extract name */
        titulus = _extrahere_declarator_nomen(init_decl);

        /* Register symbol */
        si (titulus != NIHIL && final_typus != NIHIL)
        {
            si (est_typedef)
            {
                /* Register as typedef */
                tabula_dispersa_inserere(res->typedef_nomina, *titulus, final_typus);
            }
            alioquin
            {
                /* Register as variable/function */
                _registrare_symbolum(res, titulus, final_typus, decl);
            }
        }

        /* Set typus_resolutum on the declarator node */
        si (declarator != NIHIL)
        {
            declarator->typus_resolutum = final_typus;
        }
    }
}

/* ==================================================
 * Function Definition Resolution
 * ================================================== */

interior vacuum
_resolvere_functio(ArborTypusResolver* res, ArborNodus* func)
{
    ArborTypus* base_typus;
    ArborTypus* func_typus;
    chorda* titulus;

    si (func == NIHIL || func->genus != ARBOR_NODUS_FUNCTION_DEFINITION)
    {
        redde;
    }

    /* Resolve return type from specifiers */
    base_typus = _resolvere_specifiers(res, func->datum.functio.specifiers);

    /* Apply declarator to get function type */
    func_typus = _applicare_declarator(res, base_typus, func->datum.functio.declarator);

    /* Extract function name */
    titulus = _extrahere_declarator_nomen(func->datum.functio.declarator);

    /* Register function in global scope */
    si (titulus != NIHIL && func_typus != NIHIL)
    {
        _registrare_symbolum(res, titulus, func_typus, func);
    }

    /* Set typus_resolutum */
    func->typus_resolutum = func_typus;

    /* Open scope for function body */
    _aperire_scopum(res);

    /* TODO: Add parameters to scope */

    /* Resolve function body */
    si (func->datum.functio.corpus != NIHIL)
    {
        _resolvere_nodus(res, func->datum.functio.corpus);
    }

    /* Close function scope */
    _claudere_scopum(res);
}

/* ==================================================
 * AST Walker
 * ================================================== */

interior vacuum
_resolvere_liberos(ArborTypusResolver* res, ArborNodus* nodus)
{
    i32 i;
    i32 n;
    ArborNodus** child_ptr;

    si (nodus == NIHIL)
    {
        redde;
    }

    /* Check for genericum.liberi */
    si (nodus->datum.genericum.liberi != NIHIL)
    {
        n = xar_numerus(nodus->datum.genericum.liberi);
        per (i = ZEPHYRUM; i < n; i++)
        {
            child_ptr = xar_obtinere(nodus->datum.genericum.liberi, i);
            si (child_ptr != NIHIL && *child_ptr != NIHIL)
            {
                _resolvere_nodus(res, *child_ptr);
            }
        }
    }
}

interior vacuum
_resolvere_nodus(ArborTypusResolver* res, ArborNodus* nodus)
{
    si (nodus == NIHIL)
    {
        redde;
    }

    commutatio (nodus->genus)
    {
        casus ARBOR_NODUS_TRANSLATION_UNIT:
            _resolvere_liberos(res, nodus);
            frange;

        casus ARBOR_NODUS_DECLARATION:
            _resolvere_declaratio(res, nodus);
            frange;

        casus ARBOR_NODUS_FUNCTION_DEFINITION:
            _resolvere_functio(res, nodus);
            frange;

        casus ARBOR_NODUS_COMPOUND_STATEMENT:
            _aperire_scopum(res);
            si (nodus->datum.compositum.sententiae != NIHIL)
            {
                i32 i;
                i32 n = xar_numerus(nodus->datum.compositum.sententiae);
                per (i = ZEPHYRUM; i < n; i++)
                {
                    ArborNodus** stmt_ptr = xar_obtinere(nodus->datum.compositum.sententiae, i);
                    si (stmt_ptr != NIHIL && *stmt_ptr != NIHIL)
                    {
                        _resolvere_nodus(res, *stmt_ptr);
                    }
                }
            }
            _claudere_scopum(res);
            frange;

        casus ARBOR_NODUS_IDENTIFIER:
            {
                ArborSymbolum* sym;
                sym = _quaerere_symbolum(res, nodus->datum.folium.valor);
                si (sym != NIHIL)
                {
                    nodus->typus_resolutum = sym->typus;
                }
            }
            frange;

        ordinarius:
            /* For other nodes, just recurse into children */
            _resolvere_liberos(res, nodus);
            frange;
    }
}

/* ==================================================
 * API Implementation
 * ================================================== */

ArborTypusResolver*
arbor_typus_creare(
    Piscina*              piscina,
    InternamentumChorda*  intern)
{
    ArborTypusResolver* res;

    si (piscina == NIHIL || intern == NIHIL)
    {
        redde NIHIL;
    }

    res = piscina_allocare(piscina, magnitudo(ArborTypusResolver));
    si (res == NIHIL)
    {
        redde NIHIL;
    }

    res->piscina = piscina;
    res->intern = intern;
    res->typedef_nomina = tabula_dispersa_creare_chorda(piscina, CXXVIII);
    res->tag_nomina = tabula_dispersa_creare_chorda(piscina, CXXVIII);
    res->scopes = xar_creare(piscina, magnitudo(TabulaDispersa*));
    res->globalia = tabula_dispersa_creare_chorda(piscina, CCLVI);
    res->errores = xar_creare(piscina, magnitudo(ArborError));

    redde res;
}

vacuum
arbor_typus_resolvere(
    ArborTypusResolver*   res,
    ArborNodus*           radix)
{
    si (res == NIHIL || radix == NIHIL)
    {
        redde;
    }

    _resolvere_nodus(res, radix);
}

ArborTypus*
arbor_typus_expressionis(
    ArborTypusResolver*   res,
    ArborNodus*           expr)
{
    (vacuum)res;

    si (expr == NIHIL)
    {
        redde NIHIL;
    }

    redde expr->typus_resolutum;
}

ArborNodus*
arbor_typus_declaratio(
    ArborTypusResolver*   res,
    chorda*               titulus)
{
    ArborSymbolum* sym;

    si (res == NIHIL || titulus == NIHIL)
    {
        redde NIHIL;
    }

    sym = _quaerere_symbolum(res, titulus);
    si (sym != NIHIL)
    {
        redde sym->declaratio;
    }

    redde NIHIL;
}

ArborTypus*
arbor_typus_identificatoris(
    ArborTypusResolver*   res,
    chorda*               titulus)
{
    ArborSymbolum* sym;

    si (res == NIHIL || titulus == NIHIL)
    {
        redde NIHIL;
    }

    sym = _quaerere_symbolum(res, titulus);
    si (sym != NIHIL)
    {
        redde sym->typus;
    }

    redde NIHIL;
}

/* ==================================================
 * Type Utilities
 * ================================================== */

constans character*
arbor_typus_genus_nomen(ArborTypusGenus genus)
{
    commutatio (genus)
    {
        casus ARBOR_TYPUS_VOID:     redde "void";
        casus ARBOR_TYPUS_CHAR:     redde "char";
        casus ARBOR_TYPUS_SHORT:    redde "short";
        casus ARBOR_TYPUS_INT:      redde "int";
        casus ARBOR_TYPUS_LONG:     redde "long";
        casus ARBOR_TYPUS_FLOAT:    redde "float";
        casus ARBOR_TYPUS_DOUBLE:   redde "double";
        casus ARBOR_TYPUS_STRUCT:   redde "struct";
        casus ARBOR_TYPUS_UNION:    redde "union";
        casus ARBOR_TYPUS_ENUM:     redde "enum";
        casus ARBOR_TYPUS_POINTER:  redde "pointer";
        casus ARBOR_TYPUS_ARRAY:    redde "array";
        casus ARBOR_TYPUS_FUNCTION: redde "function";
        casus ARBOR_TYPUS_TYPEDEF:  redde "typedef";
        ordinarius:                 redde "unknown";
    }
}

vacuum
arbor_typus_imprimere(ArborTypus* typus)
{
    si (typus == NIHIL)
    {
        imprimere("(null)");
        redde;
    }

    si (typus->est_const)
    {
        imprimere("const ");
    }
    si (typus->est_volatile)
    {
        imprimere("volatile ");
    }

    commutatio (typus->genus)
    {
        casus ARBOR_TYPUS_POINTER:
            imprimere("pointer to ");
            arbor_typus_imprimere(typus->basis);
            frange;

        casus ARBOR_TYPUS_ARRAY:
            imprimere("array[%d] of ", typus->array_mensura);
            arbor_typus_imprimere(typus->basis);
            frange;

        casus ARBOR_TYPUS_FUNCTION:
            imprimere("function returning ");
            arbor_typus_imprimere(typus->reditum);
            frange;

        casus ARBOR_TYPUS_TYPEDEF:
            si (typus->titulus != NIHIL)
            {
                imprimere("typedef %.*s = ",
                    (integer)typus->titulus->mensura,
                    typus->titulus->datum);
            }
            arbor_typus_imprimere(typus->basis);
            frange;

        casus ARBOR_TYPUS_STRUCT:
        casus ARBOR_TYPUS_UNION:
        casus ARBOR_TYPUS_ENUM:
            imprimere("%s", arbor_typus_genus_nomen(typus->genus));
            si (typus->titulus != NIHIL)
            {
                imprimere(" %.*s",
                    (integer)typus->titulus->mensura,
                    typus->titulus->datum);
            }
            frange;

        ordinarius:
            si (typus->est_unsigned)
            {
                imprimere("unsigned ");
            }
            imprimere("%s", arbor_typus_genus_nomen(typus->genus));
            frange;
    }
}

b32
arbor_typus_aequalis(ArborTypus* a, ArborTypus* b)
{
    si (a == NIHIL && b == NIHIL)
    {
        redde VERUM;
    }
    si (a == NIHIL || b == NIHIL)
    {
        redde FALSUM;
    }
    si (a->genus != b->genus)
    {
        redde FALSUM;
    }

    /* For simple types, genus match is enough (plus qualifiers) */
    commutatio (a->genus)
    {
        casus ARBOR_TYPUS_POINTER:
        casus ARBOR_TYPUS_ARRAY:
            redde arbor_typus_aequalis(a->basis, b->basis);

        casus ARBOR_TYPUS_STRUCT:
        casus ARBOR_TYPUS_UNION:
        casus ARBOR_TYPUS_ENUM:
            /* Named types compare by name */
            si (a->titulus != NIHIL && b->titulus != NIHIL)
            {
                redde chorda_aequalis(*a->titulus, *b->titulus);
            }
            redde a == b;  /* Anonymous types: pointer equality */

        ordinarius:
            redde VERUM;
    }
}

ArborTypus*
arbor_typus_basis(ArborTypus* typus)
{
    dum (typus != NIHIL && typus->genus == ARBOR_TYPUS_TYPEDEF)
    {
        typus = typus->basis;
    }
    redde typus;
}

b32
arbor_typus_est_integer(ArborTypus* typus)
{
    typus = arbor_typus_basis(typus);
    si (typus == NIHIL)
    {
        redde FALSUM;
    }

    commutatio (typus->genus)
    {
        casus ARBOR_TYPUS_CHAR:
        casus ARBOR_TYPUS_SHORT:
        casus ARBOR_TYPUS_INT:
        casus ARBOR_TYPUS_LONG:
        casus ARBOR_TYPUS_ENUM:
            redde VERUM;
        ordinarius:
            redde FALSUM;
    }
}

b32
arbor_typus_est_arithmeticus(ArborTypus* typus)
{
    typus = arbor_typus_basis(typus);
    si (typus == NIHIL)
    {
        redde FALSUM;
    }

    si (arbor_typus_est_integer(typus))
    {
        redde VERUM;
    }

    commutatio (typus->genus)
    {
        casus ARBOR_TYPUS_FLOAT:
        casus ARBOR_TYPUS_DOUBLE:
            redde VERUM;
        ordinarius:
            redde FALSUM;
    }
}

b32
arbor_typus_est_scalaris(ArborTypus* typus)
{
    typus = arbor_typus_basis(typus);
    si (typus == NIHIL)
    {
        redde FALSUM;
    }

    si (arbor_typus_est_arithmeticus(typus))
    {
        redde VERUM;
    }

    redde typus->genus == ARBOR_TYPUS_POINTER;
}
