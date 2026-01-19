#include "arbor_quaestio.h"
#include <string.h>
#include <stdio.h>

/* ==================================================
 * Internal State Structure
 * ================================================== */

structura ArborQuaestio {
    Piscina*              piscina;
    InternamentumChorda*  intern;
    QuaestioSequentia*    selectio;
    TabulaDispersa*       genus_tabula;       /* nomen -> ArborNodusGenus */
    TabulaDispersa*       pseudo_functiones;  /* nomen -> pseudo info */
};

/* ==================================================
 * Internal Prototypes
 * ================================================== */

interior vacuum _aedificare_genus_tabulam(ArborQuaestio* q);
interior ArborNodusGenus _titulus_ad_genus(ArborQuaestio* q, chorda* titulus);

interior QuaestioSequentia* _parsere_selector(ArborQuaestio* q, constans character* s, i32 len);
interior QuaestioComposita* _parsere_composita(ArborQuaestio* q, constans character** cursor, constans character* finis);
interior QuaestioSimplex* _parsere_simplex_genus(ArborQuaestio* q, constans character** cursor, constans character* finis);
interior QuaestioSimplex* _parsere_simplex_attributum(ArborQuaestio* q, constans character** cursor, constans character* finis);
interior QuaestioSimplex* _parsere_simplex_pseudo(ArborQuaestio* q, constans character** cursor, constans character* finis);
interior chorda* _legere_identificator(ArborQuaestio* q, constans character** cursor, constans character* finis);

interior b32 _congruit_composita(QuaestioComposita* comp, ArborNodus* nodus, TabulaDispersa* capturae);
interior b32 _congruit_simplex(QuaestioSimplex* simp, ArborNodus* nodus, TabulaDispersa* capturae);
interior b32 _congruit_attributum(QuaestioSimplex* simp, ArborNodus* nodus);
interior b32 _congruit_pseudo(QuaestioSimplex* simp, ArborNodus* nodus);

interior b32 _iterare_liberos(ArborNodus* nodus, i32* index, ArborNodus** liberum);
interior vacuum _quaerere_recursivum(ArborQuaestio* q, QuaestioSequentia* sel, ArborNodus* nodus, Xar* resultata);
interior vacuum _quaerere_liberos(ArborQuaestio* q, QuaestioSequentia* sel, ArborNodus* nodus, Xar* resultata);

/* Built-in pseudo-selectors */
interior b32 _pseudo_calls(ArborNodus* n, chorda* target);
interior b32 _pseudo_returns(ArborNodus* n);
interior b32 _pseudo_defines(ArborNodus* n, chorda* target);
interior b32 _pseudo_uses(ArborNodus* n, chorda* target);

/* ==================================================
 * Character Classification
 * ================================================== */

interior b32
_est_spatium(character c)
{
    redde c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

interior b32
_est_identificator_initium(character c)
{
    redde (c >= 'a' && c <= 'z') ||
          (c >= 'A' && c <= 'Z') ||
          c == '_' || c == '-';
}

interior b32
_est_identificator_corpus(character c)
{
    redde _est_identificator_initium(c) ||
          (c >= '0' && c <= '9');
}

interior vacuum
_saltare_spatium(constans character** cursor, constans character* finis)
{
    dum (*cursor < finis && _est_spatium(**cursor))
    {
        (*cursor)++;
    }
}

/* ==================================================
 * Type Name Lookup Table
 * ================================================== */

/* Numerus of ArborNodusGenus values */
#define ARBOR_NODUS_NUMERUS 52

interior vacuum
_aedificare_genus_tabulam(ArborQuaestio* q)
{
    i32 i;
    constans character* nom;
    chorda* key;

    q->genus_tabula = tabula_dispersa_creare_chorda(q->piscina, CXXVIII);

    per (i = ZEPHYRUM; i < ARBOR_NODUS_NUMERUS; i++)
    {
        nom = arbor_nodus_genus_nomen((ArborNodusGenus)i);
        si (nom != NIHIL && nom[ZEPHYRUM] != '\0')
        {
            key = chorda_internare_ex_literis(q->intern, nom);
            /* Store enum value as pointer (cast via long) */
            tabula_dispersa_inserere(q->genus_tabula, *key, (vacuum*)(long)(i + I));
        }
    }
}

interior ArborNodusGenus
_titulus_ad_genus(ArborQuaestio* q, chorda* titulus)
{
    vacuum* valor;

    si (tabula_dispersa_invenire(q->genus_tabula, *titulus, &valor))
    {
        /* Subtract 1 because we stored i+1 (to distinguish from NULL/0) */
        redde (ArborNodusGenus)((long)valor - I);
    }

    /* Not found - return invalid */
    redde (ArborNodusGenus)-I;
}

/* ==================================================
 * API - Compilation
 * ================================================== */

ArborQuaestio*
arbor_quaestio_compilare(
    Piscina*              piscina,
    InternamentumChorda*  intern,
    constans character*   selector)
{
    ArborQuaestio* q;
    i32 len;

    si (piscina == NIHIL || intern == NIHIL || selector == NIHIL)
    {
        redde NIHIL;
    }

    q = piscina_allocare(piscina, magnitudo(ArborQuaestio));
    si (q == NIHIL)
    {
        redde NIHIL;
    }

    q->piscina = piscina;
    q->intern = intern;
    q->selectio = NIHIL;
    q->pseudo_functiones = tabula_dispersa_creare_chorda(piscina, XXXII);

    _aedificare_genus_tabulam(q);

    len = (i32)strlen(selector);
    q->selectio = _parsere_selector(q, selector, len);

    redde q;
}

/* ==================================================
 * Selector Parsing
 * ================================================== */

interior chorda*
_legere_identificator(ArborQuaestio* q, constans character** cursor, constans character* finis)
{
    constans character* initium;
    i32 len;
    chorda temp;

    initium = *cursor;

    si (*cursor >= finis || !_est_identificator_initium(**cursor))
    {
        redde NIHIL;
    }

    dum (*cursor < finis && _est_identificator_corpus(**cursor))
    {
        (*cursor)++;
    }

    len = (i32)(*cursor - initium);
    si (len == ZEPHYRUM)
    {
        redde NIHIL;
    }

    /* Copy to mutable buffer for chorda_ex_buffer */
    {
        i8 buffer[CCLVI];
        si (len >= (i32)magnitudo(buffer))
        {
            len = (i32)magnitudo(buffer) - I;
        }
        memcpy(buffer, initium, (size_t)len);
        temp = chorda_ex_buffer(buffer, len);
        redde chorda_internare(q->intern, temp);
    }
}

interior QuaestioSimplex*
_parsere_simplex_genus(ArborQuaestio* q, constans character** cursor, constans character* finis)
{
    QuaestioSimplex* simp;
    chorda* titulus;
    ArborNodusGenus genus;

    titulus = _legere_identificator(q, cursor, finis);
    si (titulus == NIHIL)
    {
        redde NIHIL;
    }

    genus = _titulus_ad_genus(q, titulus);
    si ((i32)genus < ZEPHYRUM)
    {
        /* Unknown type name - could be error or unsupported */
        redde NIHIL;
    }

    simp = piscina_allocare(q->piscina, magnitudo(QuaestioSimplex));
    si (simp == NIHIL)
    {
        redde NIHIL;
    }

    simp->genus = QUAESTIO_SIMPLEX_GENUS;
    simp->nodus_genus = genus;
    simp->attr_nomen = NIHIL;
    simp->attr_op = QUAESTIO_ATTR_EXISTIT;
    simp->attr_valor = NIHIL;
    simp->pseudo_genus = QUAESTIO_PSEUDO_CUSTOM;
    simp->pseudo_arg = NIHIL;
    simp->pseudo_functio = NIHIL;
    simp->pseudo_datum = NIHIL;
    simp->captura_nomen = NIHIL;

    redde simp;
}

interior QuaestioSimplex*
_parsere_simplex_attributum(ArborQuaestio* q, constans character** cursor, constans character* finis)
{
    QuaestioSimplex* simp;
    chorda* attr_nomen;
    chorda* attr_valor;
    QuaestioAttrOp op;
    constans character* valor_initium;
    i32 valor_len;

    /* Skip '[' */
    si (*cursor >= finis || **cursor != '[')
    {
        redde NIHIL;
    }
    (*cursor)++;

    _saltare_spatium(cursor, finis);

    /* Read attribute name */
    attr_nomen = _legere_identificator(q, cursor, finis);
    si (attr_nomen == NIHIL)
    {
        redde NIHIL;
    }

    _saltare_spatium(cursor, finis);

    /* Check for operator or just existence check */
    si (*cursor >= finis || **cursor == ']')
    {
        /* Just [attr] - existence check */
        op = QUAESTIO_ATTR_EXISTIT;
        attr_valor = NIHIL;
    }
    alioquin
    {
        /* Parse operator */
        si (**cursor == '=')
        {
            op = QUAESTIO_ATTR_AEQUALIS;
            (*cursor)++;
        }
        alioquin si (**cursor == '^' && *cursor + I < finis && (*cursor)[I] == '=')
        {
            op = QUAESTIO_ATTR_INCIPIT;
            (*cursor) += II;
        }
        alioquin si (**cursor == '$' && *cursor + I < finis && (*cursor)[I] == '=')
        {
            op = QUAESTIO_ATTR_TERMINAT;
            (*cursor) += II;
        }
        alioquin si (**cursor == '*' && *cursor + I < finis && (*cursor)[I] == '=')
        {
            op = QUAESTIO_ATTR_CONTINET;
            (*cursor) += II;
        }
        alioquin
        {
            redde NIHIL;
        }

        _saltare_spatium(cursor, finis);

        /* Parse quoted value */
        si (*cursor >= finis || **cursor != '"')
        {
            redde NIHIL;
        }
        (*cursor)++;

        valor_initium = *cursor;
        dum (*cursor < finis && **cursor != '"')
        {
            (*cursor)++;
        }
        valor_len = (i32)(*cursor - valor_initium);

        si (*cursor >= finis)
        {
            redde NIHIL;
        }
        (*cursor)++;  /* Skip closing quote */

        {
            i8 valor_buffer[CCLVI];
            chorda temp_valor;
            si (valor_len >= (i32)magnitudo(valor_buffer))
            {
                valor_len = (i32)magnitudo(valor_buffer) - I;
            }
            memcpy(valor_buffer, valor_initium, (size_t)valor_len);
            temp_valor = chorda_ex_buffer(valor_buffer, valor_len);
            attr_valor = chorda_internare(q->intern, temp_valor);
        }
    }

    _saltare_spatium(cursor, finis);

    /* Skip ']' */
    si (*cursor >= finis || **cursor != ']')
    {
        redde NIHIL;
    }
    (*cursor)++;

    simp = piscina_allocare(q->piscina, magnitudo(QuaestioSimplex));
    si (simp == NIHIL)
    {
        redde NIHIL;
    }

    simp->genus = QUAESTIO_SIMPLEX_ATTRIBUTUM;
    simp->nodus_genus = (ArborNodusGenus)ZEPHYRUM;
    simp->attr_nomen = attr_nomen;
    simp->attr_op = op;
    simp->attr_valor = attr_valor;
    simp->pseudo_genus = QUAESTIO_PSEUDO_CUSTOM;
    simp->pseudo_arg = NIHIL;
    simp->pseudo_functio = NIHIL;
    simp->pseudo_datum = NIHIL;
    simp->captura_nomen = NIHIL;

    redde simp;
}

interior QuaestioSimplex*
_parsere_simplex_pseudo(ArborQuaestio* q, constans character** cursor, constans character* finis)
{
    QuaestioSimplex* simp;
    chorda* pseudo_titulus;
    chorda* pseudo_arg;
    QuaestioPseudoGenus pseudo_genus;

    /* Skip ':' */
    si (*cursor >= finis || **cursor != ':')
    {
        redde NIHIL;
    }
    (*cursor)++;

    pseudo_titulus = _legere_identificator(q, cursor, finis);
    si (pseudo_titulus == NIHIL)
    {
        redde NIHIL;
    }

    /* Determine pseudo type */
    si (chorda_aequalis_literis(*pseudo_titulus, "calls"))
    {
        pseudo_genus = QUAESTIO_PSEUDO_CALLS;
    }
    alioquin si (chorda_aequalis_literis(*pseudo_titulus, "defines"))
    {
        pseudo_genus = QUAESTIO_PSEUDO_DEFINES;
    }
    alioquin si (chorda_aequalis_literis(*pseudo_titulus, "uses"))
    {
        pseudo_genus = QUAESTIO_PSEUDO_USES;
    }
    alioquin si (chorda_aequalis_literis(*pseudo_titulus, "returns"))
    {
        pseudo_genus = QUAESTIO_PSEUDO_RETURNS;
    }
    alioquin si (chorda_aequalis_literis(*pseudo_titulus, "c89"))
    {
        pseudo_genus = QUAESTIO_PSEUDO_C89;
    }
    alioquin si (chorda_aequalis_literis(*pseudo_titulus, "c99"))
    {
        pseudo_genus = QUAESTIO_PSEUDO_C99;
    }
    alioquin si (chorda_aequalis_literis(*pseudo_titulus, "extension"))
    {
        pseudo_genus = QUAESTIO_PSEUDO_EXTENSION;
    }
    alioquin
    {
        pseudo_genus = QUAESTIO_PSEUDO_CUSTOM;
    }

    /* Parse optional argument */
    pseudo_arg = NIHIL;
    si (*cursor < finis && **cursor == '(')
    {
        constans character* arg_initium;
        i32 arg_len;

        (*cursor)++;
        arg_initium = *cursor;

        dum (*cursor < finis && **cursor != ')')
        {
            (*cursor)++;
        }
        arg_len = (i32)(*cursor - arg_initium);

        si (*cursor < finis)
        {
            (*cursor)++;  /* Skip ')' */
        }

        si (arg_len > ZEPHYRUM)
        {
            i8 arg_buffer[CCLVI];
            chorda temp_arg;
            si (arg_len >= (i32)magnitudo(arg_buffer))
            {
                arg_len = (i32)magnitudo(arg_buffer) - I;
            }
            memcpy(arg_buffer, arg_initium, (size_t)arg_len);
            temp_arg = chorda_ex_buffer(arg_buffer, arg_len);
            pseudo_arg = chorda_internare(q->intern, temp_arg);
        }
    }

    simp = piscina_allocare(q->piscina, magnitudo(QuaestioSimplex));
    si (simp == NIHIL)
    {
        redde NIHIL;
    }

    simp->genus = QUAESTIO_SIMPLEX_PSEUDO;
    simp->nodus_genus = (ArborNodusGenus)ZEPHYRUM;
    simp->attr_nomen = NIHIL;
    simp->attr_op = QUAESTIO_ATTR_EXISTIT;
    simp->attr_valor = NIHIL;
    simp->pseudo_genus = pseudo_genus;
    simp->pseudo_arg = pseudo_arg;
    simp->pseudo_functio = NIHIL;
    simp->pseudo_datum = NIHIL;
    simp->captura_nomen = NIHIL;

    redde simp;
}

interior QuaestioComposita*
_parsere_composita(ArborQuaestio* q, constans character** cursor, constans character* finis)
{
    QuaestioComposita* comp;
    QuaestioSimplex* simp;
    QuaestioSimplex** slot;

    comp = piscina_allocare(q->piscina, magnitudo(QuaestioComposita));
    si (comp == NIHIL)
    {
        redde NIHIL;
    }

    comp->partes = xar_creare(q->piscina, magnitudo(QuaestioSimplex*));

    _saltare_spatium(cursor, finis);

    dum (*cursor < finis)
    {
        si (_est_identificator_initium(**cursor))
        {
            /* Type selector */
            simp = _parsere_simplex_genus(q, cursor, finis);
        }
        alioquin si (**cursor == '[')
        {
            /* Attribute selector */
            simp = _parsere_simplex_attributum(q, cursor, finis);
        }
        alioquin si (**cursor == ':')
        {
            /* Pseudo selector */
            simp = _parsere_simplex_pseudo(q, cursor, finis);
        }
        alioquin si (**cursor == '$')
        {
            /* Capture */
            (*cursor)++;
            simp = piscina_allocare(q->piscina, magnitudo(QuaestioSimplex));
            si (simp != NIHIL)
            {
                simp->genus = QUAESTIO_SIMPLEX_CAPTURA;
                simp->captura_nomen = _legere_identificator(q, cursor, finis);
            }
        }
        alioquin
        {
            /* End of this composita (space or combinator) */
            frange;
        }

        si (simp != NIHIL)
        {
            slot = xar_addere(comp->partes);
            si (slot != NIHIL)
            {
                *slot = simp;
            }
        }
    }

    si (xar_numerus(comp->partes) == ZEPHYRUM)
    {
        redde NIHIL;
    }

    redde comp;
}

interior QuaestioSequentia*
_parsere_selector(ArborQuaestio* q, constans character* s, i32 len)
{
    QuaestioSequentia* radix;
    QuaestioSequentia* current;
    QuaestioSequentia* nova;
    QuaestioComposita* comp;
    QuaestioCombinator comb;
    constans character* cursor;
    constans character* finis;

    radix = NIHIL;
    current = NIHIL;
    cursor = s;
    finis = s + len;

    dum (cursor < finis)
    {
        _saltare_spatium(&cursor, finis);

        si (cursor >= finis)
        {
            frange;
        }

        /* Check for combinator */
        comb = QUAESTIO_COMBINATOR_NULLUS;
        si (current != NIHIL)
        {
            si (*cursor == '>')
            {
                comb = QUAESTIO_COMBINATOR_LIBERUM;
                cursor++;
                _saltare_spatium(&cursor, finis);
            }
            alioquin
            {
                /* Space = descendant combinator */
                comb = QUAESTIO_COMBINATOR_DESCENDENS;
            }
        }

        /* Parse composita */
        comp = _parsere_composita(q, &cursor, finis);
        si (comp == NIHIL)
        {
            frange;
        }

        /* Create sequentia node */
        nova = piscina_allocare(q->piscina, magnitudo(QuaestioSequentia));
        si (nova == NIHIL)
        {
            frange;
        }

        nova->composita = comp;
        nova->combinator = QUAESTIO_COMBINATOR_NULLUS;
        nova->sequens = NIHIL;

        si (radix == NIHIL)
        {
            radix = nova;
        }
        alioquin
        {
            current->combinator = comb;
            current->sequens = nova;
        }

        current = nova;
    }

    redde radix;
}

/* ==================================================
 * Child Iterator
 * ================================================== */

interior b32
_iterare_liberos(ArborNodus* nodus, i32* index, ArborNodus** liberum)
{
    Xar* liberi_xar;
    ArborNodus** ptr;
    i32 n;

    si (nodus == NIHIL)
    {
        redde FALSUM;
    }

    commutatio (nodus->genus)
    {
        /* Binary expressions */
        casus ARBOR_NODUS_BINARY_EXPRESSION:
            si (*index == ZEPHYRUM && nodus->datum.binarium.sinister != NIHIL)
            {
                *liberum = nodus->datum.binarium.sinister;
                (*index)++;
                redde VERUM;
            }
            si (*index == I && nodus->datum.binarium.dexter != NIHIL)
            {
                *liberum = nodus->datum.binarium.dexter;
                (*index)++;
                redde VERUM;
            }
            redde FALSUM;

        /* Unary expressions */
        casus ARBOR_NODUS_UNARY_EXPRESSION:
        casus ARBOR_NODUS_POSTFIX_EXPRESSION:
        casus ARBOR_NODUS_SIZEOF_EXPRESSION:
            si (*index == ZEPHYRUM && nodus->datum.unarium.operandum != NIHIL)
            {
                *liberum = nodus->datum.unarium.operandum;
                (*index)++;
                redde VERUM;
            }
            redde FALSUM;

        /* Conditional expression (ternary) */
        casus ARBOR_NODUS_CONDITIONAL_EXPRESSION:
            si (*index == ZEPHYRUM && nodus->datum.ternarium.conditio != NIHIL)
            {
                *liberum = nodus->datum.ternarium.conditio;
                (*index)++;
                redde VERUM;
            }
            si (*index == I && nodus->datum.ternarium.verum != NIHIL)
            {
                *liberum = nodus->datum.ternarium.verum;
                (*index)++;
                redde VERUM;
            }
            si (*index == II && nodus->datum.ternarium.falsum != NIHIL)
            {
                *liberum = nodus->datum.ternarium.falsum;
                (*index)++;
                redde VERUM;
            }
            redde FALSUM;

        /* If statement */
        casus ARBOR_NODUS_IF_STATEMENT:
            si (*index == ZEPHYRUM && nodus->datum.conditionale.conditio != NIHIL)
            {
                *liberum = nodus->datum.conditionale.conditio;
                (*index)++;
                redde VERUM;
            }
            si (*index == I && nodus->datum.conditionale.consequens != NIHIL)
            {
                *liberum = nodus->datum.conditionale.consequens;
                (*index)++;
                redde VERUM;
            }
            si (*index == II && nodus->datum.conditionale.alternans != NIHIL)
            {
                *liberum = nodus->datum.conditionale.alternans;
                (*index)++;
                redde VERUM;
            }
            redde FALSUM;

        /* While/Do statement */
        casus ARBOR_NODUS_WHILE_STATEMENT:
        casus ARBOR_NODUS_DO_STATEMENT:
            si (*index == ZEPHYRUM && nodus->datum.iteratio.conditio != NIHIL)
            {
                *liberum = nodus->datum.iteratio.conditio;
                (*index)++;
                redde VERUM;
            }
            si (*index == I && nodus->datum.iteratio.corpus != NIHIL)
            {
                *liberum = nodus->datum.iteratio.corpus;
                (*index)++;
                redde VERUM;
            }
            redde FALSUM;

        /* For statement */
        casus ARBOR_NODUS_FOR_STATEMENT:
            si (*index == ZEPHYRUM && nodus->datum.circuitus.init != NIHIL)
            {
                *liberum = nodus->datum.circuitus.init;
                (*index)++;
                redde VERUM;
            }
            si (*index == I && nodus->datum.circuitus.conditio != NIHIL)
            {
                *liberum = nodus->datum.circuitus.conditio;
                (*index)++;
                redde VERUM;
            }
            si (*index == II && nodus->datum.circuitus.post != NIHIL)
            {
                *liberum = nodus->datum.circuitus.post;
                (*index)++;
                redde VERUM;
            }
            si (*index == III && nodus->datum.circuitus.corpus != NIHIL)
            {
                *liberum = nodus->datum.circuitus.corpus;
                (*index)++;
                redde VERUM;
            }
            redde FALSUM;

        /* Switch statement */
        casus ARBOR_NODUS_SWITCH_STATEMENT:
            si (*index == ZEPHYRUM && nodus->datum.selectio.conditio != NIHIL)
            {
                *liberum = nodus->datum.selectio.conditio;
                (*index)++;
                redde VERUM;
            }
            si (*index == I && nodus->datum.selectio.corpus != NIHIL)
            {
                *liberum = nodus->datum.selectio.corpus;
                (*index)++;
                redde VERUM;
            }
            redde FALSUM;

        /* Return statement */
        casus ARBOR_NODUS_RETURN_STATEMENT:
            si (*index == ZEPHYRUM && nodus->datum.reditio.valor != NIHIL)
            {
                *liberum = nodus->datum.reditio.valor;
                (*index)++;
                redde VERUM;
            }
            redde FALSUM;

        /* Function definition */
        casus ARBOR_NODUS_FUNCTION_DEFINITION:
            si (*index == ZEPHYRUM && nodus->datum.functio.declarator != NIHIL)
            {
                *liberum = nodus->datum.functio.declarator;
                (*index)++;
                redde VERUM;
            }
            si (*index == I && nodus->datum.functio.corpus != NIHIL)
            {
                *liberum = nodus->datum.functio.corpus;
                (*index)++;
                redde VERUM;
            }
            redde FALSUM;

        /* Call expression */
        casus ARBOR_NODUS_CALL_EXPRESSION:
            si (*index == ZEPHYRUM && nodus->datum.vocatio.callee != NIHIL)
            {
                *liberum = nodus->datum.vocatio.callee;
                (*index)++;
                redde VERUM;
            }
            /* Then iterate argumenta */
            si (nodus->datum.vocatio.argumenta != NIHIL)
            {
                n = xar_numerus(nodus->datum.vocatio.argumenta);
                si (*index - I < n)
                {
                    ptr = xar_obtinere(nodus->datum.vocatio.argumenta, *index - I);
                    si (ptr != NIHIL && *ptr != NIHIL)
                    {
                        *liberum = *ptr;
                        (*index)++;
                        redde VERUM;
                    }
                }
            }
            redde FALSUM;

        /* Member expression */
        casus ARBOR_NODUS_MEMBER_EXPRESSION:
            si (*index == ZEPHYRUM && nodus->datum.membrum.objectum != NIHIL)
            {
                *liberum = nodus->datum.membrum.objectum;
                (*index)++;
                redde VERUM;
            }
            redde FALSUM;

        /* Subscript expression */
        casus ARBOR_NODUS_SUBSCRIPT_EXPRESSION:
            si (*index == ZEPHYRUM && nodus->datum.subscriptum.array != NIHIL)
            {
                *liberum = nodus->datum.subscriptum.array;
                (*index)++;
                redde VERUM;
            }
            si (*index == I && nodus->datum.subscriptum.index != NIHIL)
            {
                *liberum = nodus->datum.subscriptum.index;
                (*index)++;
                redde VERUM;
            }
            redde FALSUM;

        /* Cast expression */
        casus ARBOR_NODUS_CAST_EXPRESSION:
            si (*index == ZEPHYRUM && nodus->datum.conversio.expressio != NIHIL)
            {
                *liberum = nodus->datum.conversio.expressio;
                (*index)++;
                redde VERUM;
            }
            redde FALSUM;

        /* Compound statement */
        casus ARBOR_NODUS_COMPOUND_STATEMENT:
            liberi_xar = nodus->datum.compositum.sententiae;
            si (liberi_xar != NIHIL)
            {
                n = xar_numerus(liberi_xar);
                si (*index < n)
                {
                    ptr = xar_obtinere(liberi_xar, *index);
                    si (ptr != NIHIL && *ptr != NIHIL)
                    {
                        *liberum = *ptr;
                        (*index)++;
                        redde VERUM;
                    }
                }
            }
            redde FALSUM;

        /* Case/default label */
        casus ARBOR_NODUS_CASE_LABEL:
            si (*index == ZEPHYRUM && nodus->datum.eventus.valor != NIHIL)
            {
                *liberum = nodus->datum.eventus.valor;
                (*index)++;
                redde VERUM;
            }
            si (*index == I && nodus->datum.eventus.sententia != NIHIL)
            {
                *liberum = nodus->datum.eventus.sententia;
                (*index)++;
                redde VERUM;
            }
            redde FALSUM;

        casus ARBOR_NODUS_DEFAULT_LABEL:
            si (*index == ZEPHYRUM && nodus->datum.eventus.sententia != NIHIL)
            {
                *liberum = nodus->datum.eventus.sententia;
                (*index)++;
                redde VERUM;
            }
            redde FALSUM;

        /* Labeled statement */
        casus ARBOR_NODUS_LABELED_STATEMENT:
            si (*index == ZEPHYRUM && nodus->datum.titulatum.sententia != NIHIL)
            {
                *liberum = nodus->datum.titulatum.sententia;
                (*index)++;
                redde VERUM;
            }
            redde FALSUM;

        /* Expression statement */
        casus ARBOR_NODUS_EXPRESSION_STATEMENT:
            /* Uses genericum.liberi */
            liberi_xar = nodus->datum.genericum.liberi;
            si (liberi_xar != NIHIL)
            {
                n = xar_numerus(liberi_xar);
                si (*index < n)
                {
                    ptr = xar_obtinere(liberi_xar, *index);
                    si (ptr != NIHIL && *ptr != NIHIL)
                    {
                        *liberum = *ptr;
                        (*index)++;
                        redde VERUM;
                    }
                }
            }
            redde FALSUM;

        /* Generic nodes with liberi */
        casus ARBOR_NODUS_TRANSLATION_UNIT:
        casus ARBOR_NODUS_DECLARATION:
        casus ARBOR_NODUS_DECLARATOR:
        casus ARBOR_NODUS_PARAMETER_LIST:
        casus ARBOR_NODUS_INIT_DECLARATOR:
        casus ARBOR_NODUS_INITIALIZER:
        casus ARBOR_NODUS_INITIALIZER_LIST:
        casus ARBOR_NODUS_COMMA_EXPRESSION:
        casus ARBOR_NODUS_ASSIGNMENT_EXPRESSION:
            liberi_xar = nodus->datum.genericum.liberi;
            si (liberi_xar != NIHIL)
            {
                n = xar_numerus(liberi_xar);
                si (*index < n)
                {
                    ptr = xar_obtinere(liberi_xar, *index);
                    si (ptr != NIHIL && *ptr != NIHIL)
                    {
                        *liberum = *ptr;
                        (*index)++;
                        redde VERUM;
                    }
                }
            }
            redde FALSUM;

        /* Struct/union/enum specifiers */
        casus ARBOR_NODUS_STRUCT_SPECIFIER:
        casus ARBOR_NODUS_UNION_SPECIFIER:
            si (nodus->datum.aggregatum.membra != NIHIL)
            {
                n = xar_numerus(nodus->datum.aggregatum.membra);
                si (*index < n)
                {
                    ptr = xar_obtinere(nodus->datum.aggregatum.membra, *index);
                    si (ptr != NIHIL && *ptr != NIHIL)
                    {
                        *liberum = *ptr;
                        (*index)++;
                        redde VERUM;
                    }
                }
            }
            redde FALSUM;

        casus ARBOR_NODUS_ENUM_SPECIFIER:
            si (nodus->datum.enum_spec.enumeratores != NIHIL)
            {
                n = xar_numerus(nodus->datum.enum_spec.enumeratores);
                si (*index < n)
                {
                    ptr = xar_obtinere(nodus->datum.enum_spec.enumeratores, *index);
                    si (ptr != NIHIL && *ptr != NIHIL)
                    {
                        *liberum = *ptr;
                        (*index)++;
                        redde VERUM;
                    }
                }
            }
            redde FALSUM;

        /* Leaf nodes - no children */
        casus ARBOR_NODUS_IDENTIFIER:
        casus ARBOR_NODUS_INTEGER_LITERAL:
        casus ARBOR_NODUS_FLOAT_LITERAL:
        casus ARBOR_NODUS_CHAR_LITERAL:
        casus ARBOR_NODUS_STRING_LITERAL:
        casus ARBOR_NODUS_TYPE_SPECIFIER:
        casus ARBOR_NODUS_TYPE_QUALIFIER:
        casus ARBOR_NODUS_STORAGE_CLASS:
        casus ARBOR_NODUS_TYPEDEF_NAME:
        casus ARBOR_NODUS_POINTER:
        casus ARBOR_NODUS_GOTO_STATEMENT:
        casus ARBOR_NODUS_CONTINUE_STATEMENT:
        casus ARBOR_NODUS_BREAK_STATEMENT:
        casus ARBOR_NODUS_ERROR:
        casus ARBOR_NODUS_EXTENSION:
        casus ARBOR_NODUS_ENUMERATOR:
        casus ARBOR_NODUS_PARAMETER_DECLARATION:
        casus ARBOR_NODUS_ARRAY_DECLARATOR:
        casus ARBOR_NODUS_FUNCTION_DECLARATOR:
            redde FALSUM;

        ordinarius:
            redde FALSUM;
    }

    redde FALSUM;
}

/* ==================================================
 * Matching Logic
 * ================================================== */

interior b32
_congruit_attributum(QuaestioSimplex* simp, ArborNodus* nodus)
{
    chorda* valor;

    si (simp->attr_nomen == NIHIL)
    {
        redde FALSUM;
    }

    /* Get attribute value based on node type and attribute name */
    valor = NIHIL;

    /* Common attribute: "value" for identifiers and literals */
    si (chorda_aequalis_literis(*simp->attr_nomen, "value"))
    {
        commutatio (nodus->genus)
        {
            casus ARBOR_NODUS_IDENTIFIER:
            casus ARBOR_NODUS_STRING_LITERAL:
            casus ARBOR_NODUS_CHAR_LITERAL:
                valor = nodus->datum.folium.valor;
                frange;

            ordinarius:
                frange;
        }
    }

    si (valor == NIHIL)
    {
        redde simp->attr_op == QUAESTIO_ATTR_EXISTIT ? FALSUM : FALSUM;
    }

    /* Just existence check */
    si (simp->attr_op == QUAESTIO_ATTR_EXISTIT)
    {
        redde VERUM;
    }

    si (simp->attr_valor == NIHIL)
    {
        redde FALSUM;
    }

    commutatio (simp->attr_op)
    {
        casus QUAESTIO_ATTR_AEQUALIS:
            redde chorda_aequalis(*valor, *simp->attr_valor);

        casus QUAESTIO_ATTR_INCIPIT:
            redde chorda_incipit(*valor, *simp->attr_valor);

        casus QUAESTIO_ATTR_TERMINAT:
            redde chorda_terminatur(*valor, *simp->attr_valor);

        casus QUAESTIO_ATTR_CONTINET:
            redde chorda_continet(*valor, *simp->attr_valor);

        ordinarius:
            redde FALSUM;
    }
}

interior b32
_pseudo_calls(ArborNodus* n, chorda* target)
{
    i32 index;
    ArborNodus* liberum;

    /* Check if this is a call expression to target */
    si (n->genus == ARBOR_NODUS_CALL_EXPRESSION)
    {
        ArborNodus* callee = n->datum.vocatio.callee;
        si (callee != NIHIL && callee->genus == ARBOR_NODUS_IDENTIFIER)
        {
            si (target == NIHIL || chorda_aequalis(*callee->datum.folium.valor, *target))
            {
                redde VERUM;
            }
        }
    }

    /* Recursively check children */
    index = ZEPHYRUM;
    dum (_iterare_liberos(n, &index, &liberum))
    {
        si (_pseudo_calls(liberum, target))
        {
            redde VERUM;
        }
    }

    redde FALSUM;
}

interior b32
_pseudo_returns(ArborNodus* n)
{
    i32 index;
    ArborNodus* liberum;

    si (n->genus == ARBOR_NODUS_RETURN_STATEMENT)
    {
        redde VERUM;
    }

    /* Recursively check children */
    index = ZEPHYRUM;
    dum (_iterare_liberos(n, &index, &liberum))
    {
        si (_pseudo_returns(liberum))
        {
            redde VERUM;
        }
    }

    redde FALSUM;
}

interior b32
_pseudo_defines(ArborNodus* n, chorda* target)
{
    /* Check if node defines a symbol with given name */
    si (n->genus == ARBOR_NODUS_FUNCTION_DEFINITION)
    {
        /* Check declarator for function name */
        ArborNodus* decl = n->datum.functio.declarator;
        si (decl != NIHIL)
        {
            i32 idx = ZEPHYRUM;
            ArborNodus* child;
            dum (_iterare_liberos(decl, &idx, &child))
            {
                si (child->genus == ARBOR_NODUS_IDENTIFIER)
                {
                    si (target == NIHIL || chorda_aequalis(*child->datum.folium.valor, *target))
                    {
                        redde VERUM;
                    }
                }
            }
        }
    }

    redde FALSUM;
}

interior b32
_pseudo_uses(ArborNodus* n, chorda* target)
{
    i32 index;
    ArborNodus* liberum;

    si (n->genus == ARBOR_NODUS_IDENTIFIER)
    {
        si (target == NIHIL || chorda_aequalis(*n->datum.folium.valor, *target))
        {
            redde VERUM;
        }
    }

    /* Recursively check children */
    index = ZEPHYRUM;
    dum (_iterare_liberos(n, &index, &liberum))
    {
        si (_pseudo_uses(liberum, target))
        {
            redde VERUM;
        }
    }

    redde FALSUM;
}

interior b32
_congruit_pseudo(QuaestioSimplex* simp, ArborNodus* nodus)
{
    commutatio (simp->pseudo_genus)
    {
        casus QUAESTIO_PSEUDO_CALLS:
            redde _pseudo_calls(nodus, simp->pseudo_arg);

        casus QUAESTIO_PSEUDO_DEFINES:
            redde _pseudo_defines(nodus, simp->pseudo_arg);

        casus QUAESTIO_PSEUDO_USES:
            redde _pseudo_uses(nodus, simp->pseudo_arg);

        casus QUAESTIO_PSEUDO_RETURNS:
            redde _pseudo_returns(nodus);

        casus QUAESTIO_PSEUDO_C89:
            redde nodus->standard == ARBOR_STANDARD_C89;

        casus QUAESTIO_PSEUDO_C99:
            redde nodus->standard == ARBOR_STANDARD_C99;

        casus QUAESTIO_PSEUDO_EXTENSION:
            redde nodus->standard == ARBOR_STANDARD_EXTENSION;

        casus QUAESTIO_PSEUDO_CUSTOM:
            si (simp->pseudo_functio != NIHIL)
            {
                redde simp->pseudo_functio(nodus, simp->pseudo_datum);
            }
            redde FALSUM;

        ordinarius:
            redde FALSUM;
    }
}

interior b32
_congruit_simplex(QuaestioSimplex* simp, ArborNodus* nodus, TabulaDispersa* capturae)
{
    b32 congruit;

    congruit = VERUM;

    commutatio (simp->genus)
    {
        casus QUAESTIO_SIMPLEX_GENUS:
            congruit = (nodus->genus == simp->nodus_genus);
            frange;

        casus QUAESTIO_SIMPLEX_ATTRIBUTUM:
            congruit = _congruit_attributum(simp, nodus);
            frange;

        casus QUAESTIO_SIMPLEX_PSEUDO:
            congruit = _congruit_pseudo(simp, nodus);
            frange;

        casus QUAESTIO_SIMPLEX_CAPTURA:
            /* Capture always matches, just records the node */
            si (simp->captura_nomen != NIHIL && capturae != NIHIL)
            {
                tabula_dispersa_inserere(capturae, *simp->captura_nomen, nodus);
            }
            congruit = VERUM;
            frange;

        ordinarius:
            congruit = FALSUM;
            frange;
    }

    redde congruit;
}

interior b32
_congruit_composita(QuaestioComposita* comp, ArborNodus* nodus, TabulaDispersa* capturae)
{
    i32 i;
    i32 n;
    QuaestioSimplex** simp_ptr;

    si (comp == NIHIL || comp->partes == NIHIL)
    {
        redde FALSUM;
    }

    n = xar_numerus(comp->partes);
    per (i = ZEPHYRUM; i < n; i++)
    {
        simp_ptr = xar_obtinere(comp->partes, i);
        si (simp_ptr == NIHIL || *simp_ptr == NIHIL)
        {
            perge;
        }

        si (!_congruit_simplex(*simp_ptr, nodus, capturae))
        {
            redde FALSUM;
        }
    }

    redde VERUM;
}

/* ==================================================
 * Query Execution
 * ================================================== */

interior vacuum
_quaerere_liberos(ArborQuaestio* q, QuaestioSequentia* sel, ArborNodus* nodus, Xar* resultata)
{
    i32 index;
    ArborNodus* liberum;

    index = ZEPHYRUM;
    dum (_iterare_liberos(nodus, &index, &liberum))
    {
        _quaerere_recursivum(q, sel, liberum, resultata);
    }
}

interior vacuum
_quaerere_sequens_liberum(ArborQuaestio* q, QuaestioSequentia* sel, ArborNodus* nodus, Xar* resultata)
{
    /* Match sel->sequens on direct children only */
    i32 index;
    ArborNodus* liberum;
    TabulaDispersa* capturae;
    ArborQuaestioResultatum* res;

    si (sel->sequens == NIHIL)
    {
        redde;
    }

    index = ZEPHYRUM;
    dum (_iterare_liberos(nodus, &index, &liberum))
    {
        capturae = tabula_dispersa_creare_chorda(q->piscina, XVI);

        si (_congruit_composita(sel->sequens->composita, liberum, capturae))
        {
            si (sel->sequens->sequens == NIHIL)
            {
                /* End of chain - add result */
                res = xar_addere(resultata);
                si (res != NIHIL)
                {
                    res->nodus = liberum;
                    res->capturae = capturae;
                }
            }
            alioquin
            {
                /* Continue matching */
                si (sel->sequens->combinator == QUAESTIO_COMBINATOR_LIBERUM)
                {
                    _quaerere_sequens_liberum(q, sel->sequens, liberum, resultata);
                }
                alioquin
                {
                    _quaerere_recursivum(q, sel->sequens, liberum, resultata);
                }
            }
        }
    }
}

interior vacuum
_quaerere_recursivum(ArborQuaestio* q, QuaestioSequentia* sel, ArborNodus* nodus, Xar* resultata)
{
    TabulaDispersa* capturae;
    ArborQuaestioResultatum* res;

    si (nodus == NIHIL || sel == NIHIL)
    {
        redde;
    }

    capturae = tabula_dispersa_creare_chorda(q->piscina, XVI);

    /* Check if this node matches current selector */
    si (_congruit_composita(sel->composita, nodus, capturae))
    {
        si (sel->sequens == NIHIL)
        {
            /* End of chain - add to results */
            res = xar_addere(resultata);
            si (res != NIHIL)
            {
                res->nodus = nodus;
                res->capturae = capturae;
            }
        }
        alioquin
        {
            /* Continue matching based on combinator */
            si (sel->combinator == QUAESTIO_COMBINATOR_LIBERUM)
            {
                /* Direct child only */
                _quaerere_sequens_liberum(q, sel, nodus, resultata);
            }
            alioquin
            {
                /* Descendant - recurse into all descendants for next match */
                _quaerere_liberos(q, sel->sequens, nodus, resultata);
            }
        }
    }

    /* Always recurse for descendant matching */
    _quaerere_liberos(q, sel, nodus, resultata);
}

/* ==================================================
 * API - Execution
 * ================================================== */

Xar*
arbor_quaestio_exsequi(ArborQuaestio* q, ArborNodus* radix)
{
    Xar* resultata;

    si (q == NIHIL || radix == NIHIL || q->selectio == NIHIL)
    {
        redde NIHIL;
    }

    resultata = xar_creare(q->piscina, magnitudo(ArborQuaestioResultatum));
    si (resultata == NIHIL)
    {
        redde NIHIL;
    }

    _quaerere_recursivum(q, q->selectio, radix, resultata);

    redde resultata;
}

b32
arbor_quaestio_congruit(ArborQuaestio* q, ArborNodus* nodus)
{
    si (q == NIHIL || nodus == NIHIL || q->selectio == NIHIL)
    {
        redde FALSUM;
    }

    /* Simple match - just check first composita without captures */
    redde _congruit_composita(q->selectio->composita, nodus, NIHIL);
}

/* ==================================================
 * API - Custom Pseudo-Selectors
 * ================================================== */

vacuum
arbor_quaestio_registrare_pseudo(
    ArborQuaestio*        q,
    constans character*   titulus,
    ArborPseudoFunctio    functio,
    vacuum*               datum)
{
    chorda* key;

    /* TODO: Proper datum storage pro custom pseudo-selectors */
    (vacuum)datum;

    si (q == NIHIL || titulus == NIHIL || functio == NIHIL)
    {
        redde;
    }

    key = chorda_internare_ex_literis(q->intern, titulus);
    /* Store function pointer - datum stored elsewhere */
    tabula_dispersa_inserere(q->pseudo_functiones, *key, (vacuum*)functio);
}

/* ==================================================
 * API - Utilities
 * ================================================== */

constans character*
arbor_quaestio_pseudo_nomen(QuaestioPseudoGenus genus)
{
    commutatio (genus)
    {
        casus QUAESTIO_PSEUDO_CALLS:     redde "calls";
        casus QUAESTIO_PSEUDO_DEFINES:   redde "defines";
        casus QUAESTIO_PSEUDO_USES:      redde "uses";
        casus QUAESTIO_PSEUDO_RETURNS:   redde "returns";
        casus QUAESTIO_PSEUDO_C89:       redde "c89";
        casus QUAESTIO_PSEUDO_C99:       redde "c99";
        casus QUAESTIO_PSEUDO_EXTENSION: redde "extension";
        casus QUAESTIO_PSEUDO_CUSTOM:    redde "custom";
        ordinarius:                      redde "unknown";
    }
}

constans character*
arbor_quaestio_combinator_nomen(QuaestioCombinator comb)
{
    commutatio (comb)
    {
        casus QUAESTIO_COMBINATOR_NULLUS:     redde "nullus";
        casus QUAESTIO_COMBINATOR_DESCENDENS: redde "descendens";
        casus QUAESTIO_COMBINATOR_LIBERUM:    redde "liberum";
        ordinarius:                           redde "unknown";
    }
}
