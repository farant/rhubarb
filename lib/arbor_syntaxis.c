/* arbor_syntaxis.c - C89 Parser
 *
 * Pratt parser pro expressionibus, recursive descent pro sententiis.
 */

#include "arbor_syntaxis.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* ==================================================
 * Praecedentiae Operatorum (C89)
 *
 * Numeri maiores = praecedentia maior
 * ================================================== */

#define PREC_NIHIL           0
#define PREC_COMMA           I      /* , */
#define PREC_ASSIGNATIO      II     /* = += -= etc */
#define PREC_TERNARIUS       III    /* ?: */
#define PREC_OR_LOGICUS      IV     /* || */
#define PREC_AND_LOGICUS     V      /* && */
#define PREC_OR_BIT          VI     /* | */
#define PREC_XOR_BIT         VII    /* ^ */
#define PREC_AND_BIT         VIII   /* & */
#define PREC_AEQUALITAS      IX     /* == != */
#define PREC_RELATIO         X      /* < > <= >= */
#define PREC_SHIFT           XI     /* << >> */
#define PREC_ADDITIVUS       XII    /* + - */
#define PREC_MULTIPLICATIVUS XIII   /* * / % */
#define PREC_UNARIUS         XIV    /* - ! ~ ++ -- & * sizeof (cast) */
#define PREC_POSTFIX         XV     /* () [] . -> ++ -- */

/* ==================================================
 * Parser State
 * ================================================== */

structura ArborSyntaxis {
    Piscina*              piscina;
    InternamentumChorda*  intern;
    Xar*                  lexemata;     /* Xar of ArborLexemaOrigo */
    i32                   positus;
    i32                   numerus;      /* Total token count */
    Xar*                  errores;      /* Xar of ArborError */
    b32                   panico;       /* Error recovery mode */
};

/* ==================================================
 * Prototypi Interni
 * ================================================== */

interior ArborNodus* _parsere_expressio(ArborSyntaxis* syn, i32 praecedentia);
interior ArborNodus* _parsere_sententia(ArborSyntaxis* syn);
interior ArborNodus* _parsere_declaratio(ArborSyntaxis* syn);
interior ArborNodus* _parsere_compound(ArborSyntaxis* syn);
interior b32 _est_declaration_start(ArborSyntaxis* syn);
interior b32 _est_type_specifier(ArborLexemaGenus g);
interior b32 _est_type_qualifier(ArborLexemaGenus g);
interior b32 _est_storage_class(ArborLexemaGenus g);

/* ==================================================
 * Adiutores Interni - Navigation
 * ================================================== */

interior ArborLexemaOrigo*
_currens(ArborSyntaxis* syn)
{
    ArborLexemaOrigo** ptr;

    si (syn->positus >= syn->numerus)
    {
        redde NIHIL;
    }
    /* Xar stores ArborLexemaOrigo* pointers, so we dereference */
    ptr = xar_obtinere(syn->lexemata, syn->positus);
    si (ptr == NIHIL)
    {
        redde NIHIL;
    }
    redde *ptr;
}

interior ArborLexema*
_currens_lex(ArborSyntaxis* syn)
{
    ArborLexemaOrigo* lo;
    lo = _currens(syn);
    si (lo == NIHIL)
    {
        redde NIHIL;
    }
    redde lo->lexema;
}

interior ArborLexemaOrigo*
_prospicere(ArborSyntaxis* syn, i32 offset)
{
    i32 pos;
    ArborLexemaOrigo** ptr;

    pos = syn->positus + offset;
    si (pos < ZEPHYRUM || pos >= syn->numerus)
    {
        redde NIHIL;
    }
    /* Xar stores ArborLexemaOrigo* pointers, so we dereference */
    ptr = xar_obtinere(syn->lexemata, pos);
    si (ptr == NIHIL)
    {
        redde NIHIL;
    }
    redde *ptr;
}

interior ArborLexema*
_prospicere_lex(ArborSyntaxis* syn, i32 offset)
{
    ArborLexemaOrigo* lo;
    lo = _prospicere(syn, offset);
    si (lo == NIHIL)
    {
        redde NIHIL;
    }
    redde lo->lexema;
}

interior b32
_est_finis(ArborSyntaxis* syn)
{
    ArborLexema* lex;
    lex = _currens_lex(syn);
    redde lex == NIHIL || lex->genus == ARBOR_LEXEMA_EOF;
}

interior vacuum
_progredi(ArborSyntaxis* syn)
{
    si (!_est_finis(syn))
    {
        syn->positus++;
    }
}

interior b32
_congruit(ArborSyntaxis* syn, ArborLexemaGenus genus)
{
    ArborLexema* lex;
    lex = _currens_lex(syn);
    redde lex != NIHIL && lex->genus == genus;
}

interior b32
_consumere(ArborSyntaxis* syn, ArborLexemaGenus genus)
{
    si (_congruit(syn, genus))
    {
        _progredi(syn);
        redde VERUM;
    }
    redde FALSUM;
}

interior vacuum
_error(ArborSyntaxis* syn, constans character* nuntius)
{
    ArborError* err;
    ArborLexema* lex;

    si (syn->panico)
    {
        redde;  /* Iam in recovery mode */
    }

    err = piscina_allocare(syn->piscina, magnitudo(ArborError));
    si (err == NIHIL)
    {
        redde;
    }

    err->genus = ARBOR_ERROR_SYNTAXIS;
    err->severitas = ARBOR_SEVERITAS_ERROR;
    err->nuntius = chorda_ex_literis(nuntius, syn->piscina);
    err->via_file.datum = NIHIL;
    err->via_file.mensura = ZEPHYRUM;

    lex = _currens_lex(syn);
    si (lex != NIHIL)
    {
        err->linea = lex->linea;
        err->columna = lex->columna;
        err->longitudo = lex->longitudo;
    }
    alioquin
    {
        err->linea = ZEPHYRUM;
        err->columna = ZEPHYRUM;
        err->longitudo = ZEPHYRUM;
    }

    {
        ArborError** slot = xar_addere(syn->errores);
        si (slot != NIHIL) { *slot = err; }
    }
}

interior b32
_expectare(ArborSyntaxis* syn, ArborLexemaGenus genus, constans character* nuntius)
{
    si (_consumere(syn, genus))
    {
        redde VERUM;
    }
    _error(syn, nuntius);
    redde FALSUM;
}

/* ==================================================
 * Adiutores Interni - Node Creation
 * ================================================== */

interior ArborNodus*
_creare_nodum(ArborSyntaxis* syn, ArborNodusGenus genus)
{
    ArborNodus* nodus;
    ArborLexemaOrigo* lo;

    nodus = piscina_allocare(syn->piscina, magnitudo(ArborNodus));
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    memset(nodus, ZEPHYRUM, magnitudo(ArborNodus));
    nodus->genus = genus;
    nodus->standard = ARBOR_STANDARD_C89;

    /* Capere locum ex currente token */
    lo = _currens(syn);
    si (lo != NIHIL && lo->lexema != NIHIL)
    {
        nodus->byte_initium = lo->lexema->byte_offset;
        nodus->linea_initium = lo->lexema->linea;
        nodus->columna_initium = lo->lexema->columna;
        nodus->trivia_ante = lo->lexema->trivia_ante;
        nodus->origo = lo->origo;
    }

    redde nodus;
}

interior vacuum
_finire_nodum(ArborSyntaxis* syn, ArborNodus* nodus)
{
    ArborLexemaOrigo* lo;
    ArborLexema* lex;

    /* Capere locum finis ex praecedente token */
    si (syn->positus > ZEPHYRUM)
    {
        lo = xar_obtinere(syn->lexemata, syn->positus - I);
    }
    alioquin
    {
        lo = NIHIL;
    }
    si (lo != NIHIL && lo->lexema != NIHIL)
    {
        lex = lo->lexema;
        nodus->byte_finis = lex->byte_offset + lex->longitudo;
        nodus->linea_finis = lex->linea;
        nodus->columna_finis = lex->columna + lex->longitudo;
        nodus->trivia_post = lex->trivia_post;
    }
}

/* ==================================================
 * Error Recovery
 * ================================================== */

interior vacuum
_synchronizare(ArborSyntaxis* syn)
{
    syn->panico = VERUM;

    /* Saltare usque ad synchronization point */
    dum (!_est_finis(syn))
    {
        /* Boundaries sententiae */
        si (_congruit(syn, ARBOR_LEXEMA_SEMICOLON))
        {
            _progredi(syn);
            frange;
        }
        /* Boundaries block */
        si (_congruit(syn, ARBOR_LEXEMA_BRACE_CLAUSA))
        {
            frange;
        }
        /* Keywords quae incipiunt sententiam */
        si (_congruit(syn, ARBOR_LEXEMA_IF) ||
            _congruit(syn, ARBOR_LEXEMA_WHILE) ||
            _congruit(syn, ARBOR_LEXEMA_FOR) ||
            _congruit(syn, ARBOR_LEXEMA_DO) ||
            _congruit(syn, ARBOR_LEXEMA_SWITCH) ||
            _congruit(syn, ARBOR_LEXEMA_RETURN) ||
            _congruit(syn, ARBOR_LEXEMA_BREAK) ||
            _congruit(syn, ARBOR_LEXEMA_CONTINUE) ||
            _congruit(syn, ARBOR_LEXEMA_GOTO))
        {
            frange;
        }
        _progredi(syn);
    }

    syn->panico = FALSUM;
}

interior ArborNodus*
_creare_error_nodum(ArborSyntaxis* syn, constans character* nuntius)
{
    ArborNodus* nodus;

    nodus = _creare_nodum(syn, ARBOR_NODUS_ERROR);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    nodus->datum.error.nuntius = chorda_ex_literis(nuntius, syn->piscina);
    nodus->datum.error.lexemata_praetermissa = NIHIL;

    _synchronizare(syn);
    _finire_nodum(syn, nodus);

    redde nodus;
}

/* ==================================================
 * Praecedentia Lookup
 * ================================================== */

interior i32
_praecedentia_infixum(ArborLexemaGenus genus)
{
    commutatio (genus)
    {
        /* Comma */
        casus ARBOR_LEXEMA_COMMA:
            redde PREC_COMMA;

        /* Assignatio */
        casus ARBOR_LEXEMA_ASSIGNATIO:
        casus ARBOR_LEXEMA_PLUS_ASSIGN:
        casus ARBOR_LEXEMA_MINUS_ASSIGN:
        casus ARBOR_LEXEMA_MULT_ASSIGN:
        casus ARBOR_LEXEMA_DIV_ASSIGN:
        casus ARBOR_LEXEMA_MOD_ASSIGN:
        casus ARBOR_LEXEMA_AND_ASSIGN:
        casus ARBOR_LEXEMA_OR_ASSIGN:
        casus ARBOR_LEXEMA_XOR_ASSIGN:
        casus ARBOR_LEXEMA_SHL_ASSIGN:
        casus ARBOR_LEXEMA_SHR_ASSIGN:
            redde PREC_ASSIGNATIO;

        /* Ternarius */
        casus ARBOR_LEXEMA_QUAESTIO:
            redde PREC_TERNARIUS;

        /* Logicus */
        casus ARBOR_LEXEMA_DUPIPA:
            redde PREC_OR_LOGICUS;
        casus ARBOR_LEXEMA_DUAMPERSAND:
            redde PREC_AND_LOGICUS;

        /* Bitwise */
        casus ARBOR_LEXEMA_PIPA:
            redde PREC_OR_BIT;
        casus ARBOR_LEXEMA_CARET:
            redde PREC_XOR_BIT;
        casus ARBOR_LEXEMA_AMPERSAND:
            redde PREC_AND_BIT;

        /* Aequalitas */
        casus ARBOR_LEXEMA_AEQUALIS:
        casus ARBOR_LEXEMA_NON_AEQUALIS:
            redde PREC_AEQUALITAS;

        /* Relatio */
        casus ARBOR_LEXEMA_MINOR:
        casus ARBOR_LEXEMA_MAIOR:
        casus ARBOR_LEXEMA_MINOR_AEQ:
        casus ARBOR_LEXEMA_MAIOR_AEQ:
            redde PREC_RELATIO;

        /* Shift */
        casus ARBOR_LEXEMA_SINISTRUM:
        casus ARBOR_LEXEMA_DEXTRUM:
            redde PREC_SHIFT;

        /* Additivus */
        casus ARBOR_LEXEMA_PLUS:
        casus ARBOR_LEXEMA_MINUS:
            redde PREC_ADDITIVUS;

        /* Multiplicativus */
        casus ARBOR_LEXEMA_ASTERISCUS:
        casus ARBOR_LEXEMA_SOLIDUS:
        casus ARBOR_LEXEMA_PERCENTUM:
            redde PREC_MULTIPLICATIVUS;

        /* Postfix */
        casus ARBOR_LEXEMA_PAREN_APERTA:
        casus ARBOR_LEXEMA_BRACKET_APERTA:
        casus ARBOR_LEXEMA_PUNCTUM:
        casus ARBOR_LEXEMA_SAGITTA:
        casus ARBOR_LEXEMA_DUPLUS:
        casus ARBOR_LEXEMA_DUMINUS:
            redde PREC_POSTFIX;

        ordinarius:
            redde PREC_NIHIL;
    }
}

interior b32
_est_dexter_associativum(ArborLexemaGenus genus)
{
    commutatio (genus)
    {
        /* Assignatio et ternarius sunt dexter associativi */
        casus ARBOR_LEXEMA_ASSIGNATIO:
        casus ARBOR_LEXEMA_PLUS_ASSIGN:
        casus ARBOR_LEXEMA_MINUS_ASSIGN:
        casus ARBOR_LEXEMA_MULT_ASSIGN:
        casus ARBOR_LEXEMA_DIV_ASSIGN:
        casus ARBOR_LEXEMA_MOD_ASSIGN:
        casus ARBOR_LEXEMA_AND_ASSIGN:
        casus ARBOR_LEXEMA_OR_ASSIGN:
        casus ARBOR_LEXEMA_XOR_ASSIGN:
        casus ARBOR_LEXEMA_SHL_ASSIGN:
        casus ARBOR_LEXEMA_SHR_ASSIGN:
        casus ARBOR_LEXEMA_QUAESTIO:
            redde VERUM;
        ordinarius:
            redde FALSUM;
    }
}

/* ==================================================
 * Type Checking Helpers
 * ================================================== */

interior b32
_est_type_specifier(ArborLexemaGenus g)
{
    commutatio (g)
    {
        casus ARBOR_LEXEMA_VOID:
        casus ARBOR_LEXEMA_CHAR:
        casus ARBOR_LEXEMA_SHORT:
        casus ARBOR_LEXEMA_INT:
        casus ARBOR_LEXEMA_LONG:
        casus ARBOR_LEXEMA_FLOAT:
        casus ARBOR_LEXEMA_DOUBLE:
        casus ARBOR_LEXEMA_SIGNED:
        casus ARBOR_LEXEMA_UNSIGNED:
        casus ARBOR_LEXEMA_STRUCT:
        casus ARBOR_LEXEMA_UNION:
        casus ARBOR_LEXEMA_ENUM:
            redde VERUM;
        ordinarius:
            redde FALSUM;
    }
}

interior b32
_est_type_qualifier(ArborLexemaGenus g)
{
    redde g == ARBOR_LEXEMA_CONST || g == ARBOR_LEXEMA_VOLATILE;
}

interior b32
_est_storage_class(ArborLexemaGenus g)
{
    commutatio (g)
    {
        casus ARBOR_LEXEMA_TYPEDEF:
        casus ARBOR_LEXEMA_EXTERN:
        casus ARBOR_LEXEMA_STATIC:
        casus ARBOR_LEXEMA_AUTO:
        casus ARBOR_LEXEMA_REGISTER:
            redde VERUM;
        ordinarius:
            redde FALSUM;
    }
}

interior b32
_est_declaration_start(ArborSyntaxis* syn)
{
    ArborLexema* lex;
    lex = _currens_lex(syn);
    si (lex == NIHIL)
    {
        redde FALSUM;
    }
    redde _est_type_specifier(lex->genus) ||
           _est_type_qualifier(lex->genus) ||
           _est_storage_class(lex->genus);
}

/* ==================================================
 * Primary Expression Parsing
 * ================================================== */

interior ArborNodus*
_parsere_integer(ArborSyntaxis* syn)
{
    ArborNodus* nodus;
    ArborLexema* lex;
    constans i8* p;
    constans i8* end;
    i64 val;
    i32 base;
    b32 est_unsigned;
    b32 est_long;

    lex = _currens_lex(syn);
    si (lex == NIHIL)
    {
        _error(syn, "Expectabatur integer");
        redde NIHIL;
    }

    nodus = _creare_nodum(syn, ARBOR_NODUS_INTEGER_LITERAL);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    nodus->datum.numerus.textus = chorda_internare(syn->intern, lex->valor);

    /* Parsere valorem */
    p = lex->valor.datum;
    end = p + lex->valor.mensura;
    val = ZEPHYRUM;
    base = X;
    est_unsigned = FALSUM;
    est_long = FALSUM;

    /* Detectere basem */
    si (lex->valor.mensura > II && p[ZEPHYRUM] == '0')
    {
        si (p[I] == 'x' || p[I] == 'X')
        {
            base = XVI;
            p += II;
        }
        alioquin si (p[I] >= '0' && p[I] <= '7')
        {
            base = VIII;
            p += I;
        }
    }

    /* Parsere digitos */
    dum (p < end)
    {
        si (*p >= '0' && *p <= '9')
        {
            val = val * base + (*p - '0');
        }
        alioquin si (base == XVI && *p >= 'a' && *p <= 'f')
        {
            val = val * base + (*p - 'a' + X);
        }
        alioquin si (base == XVI && *p >= 'A' && *p <= 'F')
        {
            val = val * base + (*p - 'A' + X);
        }
        alioquin si (*p == 'u' || *p == 'U')
        {
            est_unsigned = VERUM;
        }
        alioquin si (*p == 'l' || *p == 'L')
        {
            est_long = VERUM;
        }
        p++;
    }

    nodus->datum.numerus.valor = val;
    nodus->datum.numerus.est_unsigned = est_unsigned;
    nodus->datum.numerus.est_long = est_long;

    _progredi(syn);
    _finire_nodum(syn, nodus);
    redde nodus;
}

interior ArborNodus*
_parsere_float(ArborSyntaxis* syn)
{
    ArborNodus* nodus;
    ArborLexema* lex;
    character buffer[LXIV];
    i32 len;
    i32 i;

    lex = _currens_lex(syn);
    si (lex == NIHIL)
    {
        _error(syn, "Expectabatur float");
        redde NIHIL;
    }

    nodus = _creare_nodum(syn, ARBOR_NODUS_FLOAT_LITERAL);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    nodus->datum.floating.textus = chorda_internare(syn->intern, lex->valor);

    /* Convertere ad f64 */
    len = lex->valor.mensura;
    si (len >= LXIV)
    {
        len = LXIII;
    }
    per (i = ZEPHYRUM; i < len; i++)
    {
        buffer[i] = (character)lex->valor.datum[i];
    }
    buffer[len] = '\0';
    nodus->datum.floating.valor = atof(buffer);

    _progredi(syn);
    _finire_nodum(syn, nodus);
    redde nodus;
}

interior ArborNodus*
_parsere_char_lit(ArborSyntaxis* syn)
{
    ArborNodus* nodus;
    ArborLexema* lex;

    lex = _currens_lex(syn);
    si (lex == NIHIL)
    {
        _error(syn, "Expectabatur character literal");
        redde NIHIL;
    }

    nodus = _creare_nodum(syn, ARBOR_NODUS_CHAR_LITERAL);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    nodus->datum.character_lit.textus = chorda_internare(syn->intern, lex->valor);

    /* Parsere character value (simplified) */
    si (lex->valor.mensura >= III && lex->valor.datum[I] == '\\')
    {
        /* Escape sequence */
        commutatio (lex->valor.datum[II])
        {
            casus 'n': nodus->datum.character_lit.valor = '\n'; frange;
            casus 't': nodus->datum.character_lit.valor = '\t'; frange;
            casus 'r': nodus->datum.character_lit.valor = '\r'; frange;
            casus '0': nodus->datum.character_lit.valor = '\0'; frange;
            casus '\\': nodus->datum.character_lit.valor = '\\'; frange;
            casus '\'': nodus->datum.character_lit.valor = '\''; frange;
            ordinarius: nodus->datum.character_lit.valor = lex->valor.datum[II]; frange;
        }
    }
    alioquin si (lex->valor.mensura >= II)
    {
        nodus->datum.character_lit.valor = lex->valor.datum[I];
    }

    _progredi(syn);
    _finire_nodum(syn, nodus);
    redde nodus;
}

interior ArborNodus*
_parsere_string_lit(ArborSyntaxis* syn)
{
    ArborNodus* nodus;
    ArborLexema* lex;

    lex = _currens_lex(syn);
    si (lex == NIHIL)
    {
        _error(syn, "Expectabatur string literal");
        redde NIHIL;
    }

    nodus = _creare_nodum(syn, ARBOR_NODUS_STRING_LITERAL);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    nodus->datum.string_lit.textus = chorda_internare(syn->intern, lex->valor);

    _progredi(syn);

    /* Concatenare adjacent string literals */
    dum (_congruit(syn, ARBOR_LEXEMA_STRING_LIT))
    {
        /* TODO: proper string concatenation */
        _progredi(syn);
    }

    _finire_nodum(syn, nodus);
    redde nodus;
}

interior ArborNodus*
_parsere_identificator(ArborSyntaxis* syn)
{
    ArborNodus* nodus;
    ArborLexema* lex;

    lex = _currens_lex(syn);
    si (lex == NIHIL)
    {
        _error(syn, "Expectabatur identificator");
        redde NIHIL;
    }

    nodus = _creare_nodum(syn, ARBOR_NODUS_IDENTIFIER);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    nodus->datum.folium.valor = chorda_internare(syn->intern, lex->valor);
    nodus->datum.folium.keyword = ZEPHYRUM;

    _progredi(syn);
    _finire_nodum(syn, nodus);
    redde nodus;
}

interior ArborNodus*
_parsere_parenthesis(ArborSyntaxis* syn)
{
    ArborNodus* nodus;

    /* Skip ( */
    _progredi(syn);

    /* TODO: detect cast expression (type-name) */
    nodus = _parsere_expressio(syn, PREC_NIHIL);

    _expectare(syn, ARBOR_LEXEMA_PAREN_CLAUSA, "Expectabatur )");

    redde nodus;
}

/* ==================================================
 * Unary Expression Parsing
 * ================================================== */

interior ArborNodus*
_parsere_unarium_prefix(ArborSyntaxis* syn)
{
    ArborNodus* nodus;
    ArborNodus* operandum;
    ArborLexema* lex;

    lex = _currens_lex(syn);
    si (lex == NIHIL)
    {
        _error(syn, "Expectabatur operator unarius");
        redde NIHIL;
    }

    nodus = _creare_nodum(syn, ARBOR_NODUS_UNARY_EXPRESSION);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    nodus->datum.unarium.operator = lex->genus;
    nodus->datum.unarium.est_postfix = FALSUM;

    /* Skip operator */
    _progredi(syn);

    /* Parsere operandum cum praecedentia unaria */
    operandum = _parsere_expressio(syn, PREC_UNARIUS);
    si (operandum == NIHIL)
    {
        redde NIHIL;
    }

    nodus->datum.unarium.operandum = operandum;
    operandum->pater = nodus;

    _finire_nodum(syn, nodus);
    redde nodus;
}

interior ArborNodus*
_parsere_sizeof(ArborSyntaxis* syn)
{
    ArborNodus* nodus;
    ArborNodus* operandum;

    nodus = _creare_nodum(syn, ARBOR_NODUS_SIZEOF_EXPRESSION);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    /* Skip sizeof */
    _progredi(syn);

    /* sizeof(type) vs sizeof expr */
    si (_congruit(syn, ARBOR_LEXEMA_PAREN_APERTA))
    {
        ArborLexema* next;
        next = _prospicere_lex(syn, I);

        /* Check if it looks like a type */
        si (next != NIHIL && (_est_type_specifier(next->genus) ||
                              _est_type_qualifier(next->genus)))
        {
            /* sizeof(type) - create type specifier node */
            ArborNodus* type_node;

            nodus->datum.sizeof_expr.est_typus = VERUM;
            _progredi(syn);  /* Skip ( */

            /* Parse type specifier */
            type_node = _creare_nodum(syn, ARBOR_NODUS_TYPE_SPECIFIER);
            si (type_node != NIHIL)
            {
                ArborLexema* type_lex = _currens_lex(syn);
                si (type_lex != NIHIL)
                {
                    type_node->datum.folium.keyword = type_lex->genus;
                    type_node->datum.folium.valor = chorda_internare(syn->intern, type_lex->valor);
                }
                _progredi(syn);  /* Skip type specifier */
                _finire_nodum(syn, type_node);
            }
            operandum = type_node;

            _expectare(syn, ARBOR_LEXEMA_PAREN_CLAUSA, "Expectabatur )");
        }
        alioquin
        {
            /* sizeof(expr) */
            nodus->datum.sizeof_expr.est_typus = FALSUM;
            operandum = _parsere_expressio(syn, PREC_UNARIUS);
        }
    }
    alioquin
    {
        /* sizeof expr */
        nodus->datum.sizeof_expr.est_typus = FALSUM;
        operandum = _parsere_expressio(syn, PREC_UNARIUS);
    }

    si (operandum == NIHIL)
    {
        redde NIHIL;
    }

    nodus->datum.sizeof_expr.operandum = operandum;
    operandum->pater = nodus;

    _finire_nodum(syn, nodus);
    redde nodus;
}

/* ==================================================
 * Infix/Postfix Expression Parsing
 * ================================================== */

interior ArborNodus*
_parsere_binarium(ArborSyntaxis* syn, ArborNodus* sinister)
{
    ArborNodus* nodus;
    ArborNodus* dexter;
    ArborLexema* lex;
    i32 praecedentia;
    b32 dexter_assoc;

    lex = _currens_lex(syn);
    si (lex == NIHIL)
    {
        redde sinister;
    }

    nodus = _creare_nodum(syn, ARBOR_NODUS_BINARY_EXPRESSION);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    nodus->datum.binarium.operator = lex->genus;
    nodus->datum.binarium.sinister = sinister;
    sinister->pater = nodus;

    praecedentia = _praecedentia_infixum(lex->genus);
    dexter_assoc = _est_dexter_associativum(lex->genus);

    /* Skip operator */
    _progredi(syn);

    /* Parsere dextrum */
    dexter = _parsere_expressio(syn, dexter_assoc ? praecedentia - I : praecedentia);
    si (dexter == NIHIL)
    {
        redde NIHIL;
    }

    nodus->datum.binarium.dexter = dexter;
    dexter->pater = nodus;

    _finire_nodum(syn, nodus);
    redde nodus;
}

interior ArborNodus*
_parsere_ternarium(ArborSyntaxis* syn, ArborNodus* conditio)
{
    ArborNodus* nodus;
    ArborNodus* verum_ramus;
    ArborNodus* falsum_ramus;

    nodus = _creare_nodum(syn, ARBOR_NODUS_CONDITIONAL_EXPRESSION);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    nodus->datum.ternarium.conditio = conditio;
    conditio->pater = nodus;

    /* Skip ? */
    _progredi(syn);

    /* Parsere verum ramus */
    verum_ramus = _parsere_expressio(syn, PREC_NIHIL);
    si (verum_ramus == NIHIL)
    {
        redde NIHIL;
    }
    nodus->datum.ternarium.verum = verum_ramus;
    verum_ramus->pater = nodus;

    /* Expectare : */
    si (!_expectare(syn, ARBOR_LEXEMA_COLON, "Expectabatur : in conditione"))
    {
        redde NIHIL;
    }

    /* Parsere falsum ramus */
    falsum_ramus = _parsere_expressio(syn, PREC_TERNARIUS - I);
    si (falsum_ramus == NIHIL)
    {
        redde NIHIL;
    }
    nodus->datum.ternarium.falsum = falsum_ramus;
    falsum_ramus->pater = nodus;

    _finire_nodum(syn, nodus);
    redde nodus;
}

interior ArborNodus*
_parsere_assignatio(ArborSyntaxis* syn, ArborNodus* target)
{
    ArborNodus* nodus;
    ArborNodus* valor;
    ArborLexema* lex;

    lex = _currens_lex(syn);
    si (lex == NIHIL)
    {
        redde target;
    }

    nodus = _creare_nodum(syn, ARBOR_NODUS_ASSIGNMENT_EXPRESSION);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    nodus->datum.assignatio.target = target;
    nodus->datum.assignatio.operator = lex->genus;
    target->pater = nodus;

    /* Skip operator */
    _progredi(syn);

    /* Parsere valorem (dexter associativum) */
    valor = _parsere_expressio(syn, PREC_ASSIGNATIO - I);
    si (valor == NIHIL)
    {
        redde NIHIL;
    }

    nodus->datum.assignatio.valor = valor;
    valor->pater = nodus;

    _finire_nodum(syn, nodus);
    redde nodus;
}

interior ArborNodus*
_parsere_vocatio(ArborSyntaxis* syn, ArborNodus* callee)
{
    ArborNodus* nodus;
    ArborNodus* argumentum;

    nodus = _creare_nodum(syn, ARBOR_NODUS_CALL_EXPRESSION);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    nodus->datum.vocatio.callee = callee;
    nodus->datum.vocatio.argumenta = xar_creare(syn->piscina, magnitudo(ArborNodus*));
    callee->pater = nodus;

    /* Skip ( */
    _progredi(syn);

    /* Parsere argumenta */
    si (!_congruit(syn, ARBOR_LEXEMA_PAREN_CLAUSA))
    {
        fac {
            argumentum = _parsere_expressio(syn, PREC_ASSIGNATIO);  /* No comma at this level */
            si (argumentum == NIHIL)
            {
                redde NIHIL;
            }
            argumentum->pater = nodus;
            {
                ArborNodus** slot = xar_addere(nodus->datum.vocatio.argumenta);
                si (slot != NIHIL) { *slot = argumentum; }
            }
        } dum (_consumere(syn, ARBOR_LEXEMA_COMMA));
    }

    _expectare(syn, ARBOR_LEXEMA_PAREN_CLAUSA, "Expectabatur )");

    _finire_nodum(syn, nodus);
    redde nodus;
}

interior ArborNodus*
_parsere_subscriptum(ArborSyntaxis* syn, ArborNodus* array)
{
    ArborNodus* nodus;
    ArborNodus* index;

    nodus = _creare_nodum(syn, ARBOR_NODUS_SUBSCRIPT_EXPRESSION);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    nodus->datum.subscriptum.array = array;
    array->pater = nodus;

    /* Skip [ */
    _progredi(syn);

    /* Parsere indicem */
    index = _parsere_expressio(syn, PREC_NIHIL);
    si (index == NIHIL)
    {
        redde NIHIL;
    }
    nodus->datum.subscriptum.index = index;
    index->pater = nodus;

    _expectare(syn, ARBOR_LEXEMA_BRACKET_CLAUSA, "Expectabatur ]");

    _finire_nodum(syn, nodus);
    redde nodus;
}

interior ArborNodus*
_parsere_membrum(ArborSyntaxis* syn, ArborNodus* objectum)
{
    ArborNodus* nodus;
    ArborLexema* lex;
    ArborLexemaGenus operator;

    lex = _currens_lex(syn);
    operator = lex->genus;

    nodus = _creare_nodum(syn, ARBOR_NODUS_MEMBER_EXPRESSION);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    nodus->datum.membrum.objectum = objectum;
    nodus->datum.membrum.operator = operator;
    objectum->pater = nodus;

    /* Skip . or -> */
    _progredi(syn);

    /* Expectare identificatorem */
    lex = _currens_lex(syn);
    si (lex == NIHIL || lex->genus != ARBOR_LEXEMA_IDENTIFICATOR)
    {
        _error(syn, "Expectabatur nomen membri");
        redde NIHIL;
    }

    nodus->datum.membrum.membrum = chorda_internare(syn->intern, lex->valor);
    _progredi(syn);

    _finire_nodum(syn, nodus);
    redde nodus;
}

interior ArborNodus*
_parsere_postfix_increment(ArborSyntaxis* syn, ArborNodus* operandum)
{
    ArborNodus* nodus;
    ArborLexema* lex;

    lex = _currens_lex(syn);

    nodus = _creare_nodum(syn, ARBOR_NODUS_UNARY_EXPRESSION);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    nodus->datum.unarium.operator = lex->genus;
    nodus->datum.unarium.operandum = operandum;
    nodus->datum.unarium.est_postfix = VERUM;
    operandum->pater = nodus;

    /* Skip ++ or -- */
    _progredi(syn);

    _finire_nodum(syn, nodus);
    redde nodus;
}

/* ==================================================
 * Pratt Parser Core
 * ================================================== */

interior ArborNodus*
_parsere_expressio(ArborSyntaxis* syn, i32 praecedentia)
{
    ArborNodus* sinister;
    ArborLexema* lex;
    i32 praec_currens;

    si (syn->panico)
    {
        redde NIHIL;
    }

    lex = _currens_lex(syn);
    si (lex == NIHIL || lex->genus == ARBOR_LEXEMA_EOF)
    {
        _error(syn, "Expectabatur expressio");
        redde NIHIL;
    }

    /* Prefix parselets */
    commutatio (lex->genus)
    {
        casus ARBOR_LEXEMA_INTEGER:
            sinister = _parsere_integer(syn);
            frange;
        casus ARBOR_LEXEMA_FLOAT_LIT:
            sinister = _parsere_float(syn);
            frange;
        casus ARBOR_LEXEMA_CHAR_LIT:
            sinister = _parsere_char_lit(syn);
            frange;
        casus ARBOR_LEXEMA_STRING_LIT:
            sinister = _parsere_string_lit(syn);
            frange;
        casus ARBOR_LEXEMA_IDENTIFICATOR:
            sinister = _parsere_identificator(syn);
            frange;
        casus ARBOR_LEXEMA_PAREN_APERTA:
            sinister = _parsere_parenthesis(syn);
            frange;
        casus ARBOR_LEXEMA_SIZEOF:
            sinister = _parsere_sizeof(syn);
            frange;
        /* Unary prefix operators */
        casus ARBOR_LEXEMA_MINUS:
        casus ARBOR_LEXEMA_PLUS:
        casus ARBOR_LEXEMA_EXCLAMATIO:
        casus ARBOR_LEXEMA_TILDE:
        casus ARBOR_LEXEMA_AMPERSAND:
        casus ARBOR_LEXEMA_ASTERISCUS:
        casus ARBOR_LEXEMA_DUPLUS:
        casus ARBOR_LEXEMA_DUMINUS:
            sinister = _parsere_unarium_prefix(syn);
            frange;
        ordinarius:
            _error(syn, "Lexema inexpectatum in expressione");
            redde NIHIL;
    }

    si (sinister == NIHIL || syn->panico)
    {
        redde sinister;
    }

    /* Infix parselets (Pratt loop) */
    dum (!_est_finis(syn))
    {
        lex = _currens_lex(syn);
        si (lex == NIHIL)
        {
            frange;
        }

        praec_currens = _praecedentia_infixum(lex->genus);
        si (praec_currens <= praecedentia)
        {
            frange;
        }

        commutatio (lex->genus)
        {
            /* Binary operators */
            casus ARBOR_LEXEMA_PLUS:
            casus ARBOR_LEXEMA_MINUS:
            casus ARBOR_LEXEMA_ASTERISCUS:
            casus ARBOR_LEXEMA_SOLIDUS:
            casus ARBOR_LEXEMA_PERCENTUM:
            casus ARBOR_LEXEMA_AEQUALIS:
            casus ARBOR_LEXEMA_NON_AEQUALIS:
            casus ARBOR_LEXEMA_MINOR:
            casus ARBOR_LEXEMA_MAIOR:
            casus ARBOR_LEXEMA_MINOR_AEQ:
            casus ARBOR_LEXEMA_MAIOR_AEQ:
            casus ARBOR_LEXEMA_DUAMPERSAND:
            casus ARBOR_LEXEMA_DUPIPA:
            casus ARBOR_LEXEMA_AMPERSAND:
            casus ARBOR_LEXEMA_PIPA:
            casus ARBOR_LEXEMA_CARET:
            casus ARBOR_LEXEMA_SINISTRUM:
            casus ARBOR_LEXEMA_DEXTRUM:
                sinister = _parsere_binarium(syn, sinister);
                frange;

            /* Ternary */
            casus ARBOR_LEXEMA_QUAESTIO:
                sinister = _parsere_ternarium(syn, sinister);
                frange;

            /* Assignment */
            casus ARBOR_LEXEMA_ASSIGNATIO:
            casus ARBOR_LEXEMA_PLUS_ASSIGN:
            casus ARBOR_LEXEMA_MINUS_ASSIGN:
            casus ARBOR_LEXEMA_MULT_ASSIGN:
            casus ARBOR_LEXEMA_DIV_ASSIGN:
            casus ARBOR_LEXEMA_MOD_ASSIGN:
            casus ARBOR_LEXEMA_AND_ASSIGN:
            casus ARBOR_LEXEMA_OR_ASSIGN:
            casus ARBOR_LEXEMA_XOR_ASSIGN:
            casus ARBOR_LEXEMA_SHL_ASSIGN:
            casus ARBOR_LEXEMA_SHR_ASSIGN:
                sinister = _parsere_assignatio(syn, sinister);
                frange;

            /* Function call */
            casus ARBOR_LEXEMA_PAREN_APERTA:
                sinister = _parsere_vocatio(syn, sinister);
                frange;

            /* Array subscript */
            casus ARBOR_LEXEMA_BRACKET_APERTA:
                sinister = _parsere_subscriptum(syn, sinister);
                frange;

            /* Member access */
            casus ARBOR_LEXEMA_PUNCTUM:
            casus ARBOR_LEXEMA_SAGITTA:
                sinister = _parsere_membrum(syn, sinister);
                frange;

            /* Postfix increment/decrement */
            casus ARBOR_LEXEMA_DUPLUS:
            casus ARBOR_LEXEMA_DUMINUS:
                sinister = _parsere_postfix_increment(syn, sinister);
                frange;

            /* Comma operator - but only at comma precedence level */
            casus ARBOR_LEXEMA_COMMA:
                si (praecedentia < PREC_COMMA)
                {
                    sinister = _parsere_binarium(syn, sinister);
                }
                alioquin
                {
                    redde sinister;
                }
                frange;

            ordinarius:
                redde sinister;
        }

        si (sinister == NIHIL || syn->panico)
        {
            redde sinister;
        }
    }

    redde sinister;
}

/* ==================================================
 * Statement Parsing
 * ================================================== */

interior ArborNodus*
_parsere_expression_statement(ArborSyntaxis* syn)
{
    ArborNodus* nodus;
    ArborNodus* expr;

    nodus = _creare_nodum(syn, ARBOR_NODUS_EXPRESSION_STATEMENT);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    nodus->datum.genericum.liberi = xar_creare(syn->piscina, magnitudo(ArborNodus*));

    expr = _parsere_expressio(syn, PREC_NIHIL);
    si (expr == NIHIL)
    {
        redde NIHIL;
    }
    expr->pater = nodus;
    {
        ArborNodus** slot = xar_addere(nodus->datum.genericum.liberi);
        si (slot != NIHIL) { *slot = expr; }
    }

    _expectare(syn, ARBOR_LEXEMA_SEMICOLON, "Expectabatur ;");

    _finire_nodum(syn, nodus);
    redde nodus;
}

interior ArborNodus*
_parsere_if(ArborSyntaxis* syn)
{
    ArborNodus* nodus;
    ArborNodus* conditio;
    ArborNodus* consequens;
    ArborNodus* alternans;

    nodus = _creare_nodum(syn, ARBOR_NODUS_IF_STATEMENT);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    /* Skip if */
    _progredi(syn);

    /* ( conditio ) */
    si (!_expectare(syn, ARBOR_LEXEMA_PAREN_APERTA, "Expectabatur ( post 'if'"))
    {
        redde _creare_error_nodum(syn, "if sine conditione");
    }

    conditio = _parsere_expressio(syn, PREC_NIHIL);
    si (conditio == NIHIL)
    {
        redde _creare_error_nodum(syn, "if conditio invalida");
    }
    nodus->datum.conditionale.conditio = conditio;
    conditio->pater = nodus;

    si (!_expectare(syn, ARBOR_LEXEMA_PAREN_CLAUSA, "Expectabatur )"))
    {
        redde _creare_error_nodum(syn, "if sine )");
    }

    /* consequens */
    consequens = _parsere_sententia(syn);
    si (consequens == NIHIL)
    {
        redde _creare_error_nodum(syn, "if sine corpore");
    }
    nodus->datum.conditionale.consequens = consequens;
    consequens->pater = nodus;

    /* optionalis else */
    si (_consumere(syn, ARBOR_LEXEMA_ELSE))
    {
        alternans = _parsere_sententia(syn);
        si (alternans == NIHIL)
        {
            redde _creare_error_nodum(syn, "else sine corpore");
        }
        nodus->datum.conditionale.alternans = alternans;
        alternans->pater = nodus;
    }
    alioquin
    {
        nodus->datum.conditionale.alternans = NIHIL;
    }

    _finire_nodum(syn, nodus);
    redde nodus;
}

interior ArborNodus*
_parsere_while(ArborSyntaxis* syn)
{
    ArborNodus* nodus;
    ArborNodus* conditio;
    ArborNodus* corpus;

    nodus = _creare_nodum(syn, ARBOR_NODUS_WHILE_STATEMENT);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    /* Skip while */
    _progredi(syn);

    /* ( conditio ) */
    _expectare(syn, ARBOR_LEXEMA_PAREN_APERTA, "Expectabatur (");
    conditio = _parsere_expressio(syn, PREC_NIHIL);
    si (conditio == NIHIL)
    {
        redde NIHIL;
    }
    nodus->datum.iteratio.conditio = conditio;
    conditio->pater = nodus;
    _expectare(syn, ARBOR_LEXEMA_PAREN_CLAUSA, "Expectabatur )");

    /* corpus */
    corpus = _parsere_sententia(syn);
    si (corpus == NIHIL)
    {
        redde NIHIL;
    }
    nodus->datum.iteratio.corpus = corpus;
    corpus->pater = nodus;

    _finire_nodum(syn, nodus);
    redde nodus;
}

interior ArborNodus*
_parsere_do(ArborSyntaxis* syn)
{
    ArborNodus* nodus;
    ArborNodus* conditio;
    ArborNodus* corpus;

    nodus = _creare_nodum(syn, ARBOR_NODUS_DO_STATEMENT);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    /* Skip do */
    _progredi(syn);

    /* corpus */
    corpus = _parsere_sententia(syn);
    si (corpus == NIHIL)
    {
        redde NIHIL;
    }
    nodus->datum.iteratio.corpus = corpus;
    corpus->pater = nodus;

    /* while ( conditio ) ; */
    _expectare(syn, ARBOR_LEXEMA_WHILE, "Expectabatur 'while'");
    _expectare(syn, ARBOR_LEXEMA_PAREN_APERTA, "Expectabatur (");

    conditio = _parsere_expressio(syn, PREC_NIHIL);
    si (conditio == NIHIL)
    {
        redde NIHIL;
    }
    nodus->datum.iteratio.conditio = conditio;
    conditio->pater = nodus;

    _expectare(syn, ARBOR_LEXEMA_PAREN_CLAUSA, "Expectabatur )");
    _expectare(syn, ARBOR_LEXEMA_SEMICOLON, "Expectabatur ;");

    _finire_nodum(syn, nodus);
    redde nodus;
}

interior ArborNodus*
_parsere_for(ArborSyntaxis* syn)
{
    ArborNodus* nodus;
    ArborNodus* init;
    ArborNodus* conditio;
    ArborNodus* post;
    ArborNodus* corpus;

    nodus = _creare_nodum(syn, ARBOR_NODUS_FOR_STATEMENT);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    /* Skip for */
    _progredi(syn);

    _expectare(syn, ARBOR_LEXEMA_PAREN_APERTA, "Expectabatur (");

    /* init (optionalis) */
    si (_congruit(syn, ARBOR_LEXEMA_SEMICOLON))
    {
        init = NIHIL;
    }
    alioquin si (_est_declaration_start(syn))
    {
        init = _parsere_declaratio(syn);
        /* Declaratio iam habet ; */
        nodus->datum.circuitus.init = init;
        si (init != NIHIL) init->pater = nodus;

        /* Parse conditio */
        si (_congruit(syn, ARBOR_LEXEMA_SEMICOLON))
        {
            conditio = NIHIL;
        }
        alioquin
        {
            conditio = _parsere_expressio(syn, PREC_NIHIL);
        }
        _expectare(syn, ARBOR_LEXEMA_SEMICOLON, "Expectabatur ;");
        nodus->datum.circuitus.conditio = conditio;
        si (conditio != NIHIL) conditio->pater = nodus;

        /* Parse post */
        si (_congruit(syn, ARBOR_LEXEMA_PAREN_CLAUSA))
        {
            post = NIHIL;
        }
        alioquin
        {
            post = _parsere_expressio(syn, PREC_NIHIL);
        }
        nodus->datum.circuitus.post = post;
        si (post != NIHIL) post->pater = nodus;

        _expectare(syn, ARBOR_LEXEMA_PAREN_CLAUSA, "Expectabatur )");

        /* corpus */
        corpus = _parsere_sententia(syn);
        si (corpus == NIHIL)
        {
            redde NIHIL;
        }
        nodus->datum.circuitus.corpus = corpus;
        corpus->pater = nodus;

        _finire_nodum(syn, nodus);
        redde nodus;
    }
    alioquin
    {
        init = _parsere_expressio(syn, PREC_NIHIL);
    }
    _expectare(syn, ARBOR_LEXEMA_SEMICOLON, "Expectabatur ;");
    nodus->datum.circuitus.init = init;
    si (init != NIHIL) init->pater = nodus;

    /* conditio (optionalis) */
    si (_congruit(syn, ARBOR_LEXEMA_SEMICOLON))
    {
        conditio = NIHIL;
    }
    alioquin
    {
        conditio = _parsere_expressio(syn, PREC_NIHIL);
    }
    _expectare(syn, ARBOR_LEXEMA_SEMICOLON, "Expectabatur ;");
    nodus->datum.circuitus.conditio = conditio;
    si (conditio != NIHIL) conditio->pater = nodus;

    /* post (optionalis) */
    si (_congruit(syn, ARBOR_LEXEMA_PAREN_CLAUSA))
    {
        post = NIHIL;
    }
    alioquin
    {
        post = _parsere_expressio(syn, PREC_NIHIL);
    }
    _expectare(syn, ARBOR_LEXEMA_PAREN_CLAUSA, "Expectabatur )");
    nodus->datum.circuitus.post = post;
    si (post != NIHIL) post->pater = nodus;

    /* corpus */
    corpus = _parsere_sententia(syn);
    si (corpus == NIHIL)
    {
        redde NIHIL;
    }
    nodus->datum.circuitus.corpus = corpus;
    corpus->pater = nodus;

    _finire_nodum(syn, nodus);
    redde nodus;
}

interior ArborNodus*
_parsere_switch(ArborSyntaxis* syn)
{
    ArborNodus* nodus;
    ArborNodus* conditio;
    ArborNodus* corpus;

    nodus = _creare_nodum(syn, ARBOR_NODUS_SWITCH_STATEMENT);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    /* Skip switch */
    _progredi(syn);

    /* ( conditio ) */
    _expectare(syn, ARBOR_LEXEMA_PAREN_APERTA, "Expectabatur (");
    conditio = _parsere_expressio(syn, PREC_NIHIL);
    si (conditio == NIHIL)
    {
        redde NIHIL;
    }
    nodus->datum.selectio.conditio = conditio;
    conditio->pater = nodus;
    _expectare(syn, ARBOR_LEXEMA_PAREN_CLAUSA, "Expectabatur )");

    /* corpus */
    corpus = _parsere_sententia(syn);
    si (corpus == NIHIL)
    {
        redde NIHIL;
    }
    nodus->datum.selectio.corpus = corpus;
    corpus->pater = nodus;

    _finire_nodum(syn, nodus);
    redde nodus;
}

interior ArborNodus*
_parsere_case(ArborSyntaxis* syn)
{
    ArborNodus* nodus;
    ArborNodus* valor;

    nodus = _creare_nodum(syn, ARBOR_NODUS_CASE_LABEL);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    /* Skip case */
    _progredi(syn);

    valor = _parsere_expressio(syn, PREC_NIHIL);
    si (valor == NIHIL)
    {
        redde NIHIL;
    }
    nodus->datum.eventus.valor = valor;
    valor->pater = nodus;

    _expectare(syn, ARBOR_LEXEMA_COLON, "Expectabatur :");

    /* Sententia post case - handled by caller */
    nodus->datum.eventus.sententia = NIHIL;

    _finire_nodum(syn, nodus);
    redde nodus;
}

interior ArborNodus*
_parsere_default(ArborSyntaxis* syn)
{
    ArborNodus* nodus;

    nodus = _creare_nodum(syn, ARBOR_NODUS_DEFAULT_LABEL);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    /* Skip default */
    _progredi(syn);

    _expectare(syn, ARBOR_LEXEMA_COLON, "Expectabatur :");

    _finire_nodum(syn, nodus);
    redde nodus;
}

interior ArborNodus*
_parsere_return(ArborSyntaxis* syn)
{
    ArborNodus* nodus;
    ArborNodus* valor;

    nodus = _creare_nodum(syn, ARBOR_NODUS_RETURN_STATEMENT);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    /* Skip return */
    _progredi(syn);

    /* optionalis valor */
    si (!_congruit(syn, ARBOR_LEXEMA_SEMICOLON))
    {
        valor = _parsere_expressio(syn, PREC_NIHIL);
        si (valor == NIHIL)
        {
            redde NIHIL;
        }
        nodus->datum.reditio.valor = valor;
        valor->pater = nodus;
    }
    alioquin
    {
        nodus->datum.reditio.valor = NIHIL;
    }

    _expectare(syn, ARBOR_LEXEMA_SEMICOLON, "Expectabatur ;");

    _finire_nodum(syn, nodus);
    redde nodus;
}

interior ArborNodus*
_parsere_break(ArborSyntaxis* syn)
{
    ArborNodus* nodus;

    nodus = _creare_nodum(syn, ARBOR_NODUS_BREAK_STATEMENT);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    /* Skip break */
    _progredi(syn);

    _expectare(syn, ARBOR_LEXEMA_SEMICOLON, "Expectabatur ;");

    _finire_nodum(syn, nodus);
    redde nodus;
}

interior ArborNodus*
_parsere_continue(ArborSyntaxis* syn)
{
    ArborNodus* nodus;

    nodus = _creare_nodum(syn, ARBOR_NODUS_CONTINUE_STATEMENT);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    /* Skip continue */
    _progredi(syn);

    _expectare(syn, ARBOR_LEXEMA_SEMICOLON, "Expectabatur ;");

    _finire_nodum(syn, nodus);
    redde nodus;
}

interior ArborNodus*
_parsere_goto(ArborSyntaxis* syn)
{
    ArborNodus* nodus;
    ArborLexema* lex;

    nodus = _creare_nodum(syn, ARBOR_NODUS_GOTO_STATEMENT);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    /* Skip goto */
    _progredi(syn);

    lex = _currens_lex(syn);
    si (lex == NIHIL || lex->genus != ARBOR_LEXEMA_IDENTIFICATOR)
    {
        _error(syn, "Expectabatur label post 'goto'");
        redde NIHIL;
    }

    nodus->datum.saltus.label = chorda_internare(syn->intern, lex->valor);
    _progredi(syn);

    _expectare(syn, ARBOR_LEXEMA_SEMICOLON, "Expectabatur ;");

    _finire_nodum(syn, nodus);
    redde nodus;
}

interior ArborNodus*
_parsere_labeled(ArborSyntaxis* syn)
{
    ArborNodus* nodus;
    ArborLexema* lex;

    lex = _currens_lex(syn);

    nodus = _creare_nodum(syn, ARBOR_NODUS_LABELED_STATEMENT);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    nodus->datum.titulatum.label = chorda_internare(syn->intern, lex->valor);
    _progredi(syn);

    _expectare(syn, ARBOR_LEXEMA_COLON, "Expectabatur :");

    /* Sententia post label */
    nodus->datum.titulatum.sententia = _parsere_sententia(syn);
    si (nodus->datum.titulatum.sententia != NIHIL)
    {
        nodus->datum.titulatum.sententia->pater = nodus;
    }

    _finire_nodum(syn, nodus);
    redde nodus;
}

interior ArborNodus*
_parsere_compound(ArborSyntaxis* syn)
{
    ArborNodus* nodus;
    ArborNodus* item;
    i32 prev_pos;

    nodus = _creare_nodum(syn, ARBOR_NODUS_COMPOUND_STATEMENT);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    nodus->datum.compositum.sententiae = xar_creare(syn->piscina, magnitudo(ArborNodus*));

    /* Skip { */
    _progredi(syn);

    /* Parsere sententias et declarationes */
    dum (!_est_finis(syn) && !_congruit(syn, ARBOR_LEXEMA_BRACE_CLAUSA))
    {
        prev_pos = syn->positus;

        si (_est_declaration_start(syn))
        {
            item = _parsere_declaratio(syn);
        }
        alioquin
        {
            item = _parsere_sententia(syn);
        }

        si (item == NIHIL)
        {
            /* Evitare infinitum loop */
            si (syn->positus == prev_pos && !_est_finis(syn))
            {
                _error(syn, "Unexpected token in compound statement");
                _progredi(syn);
            }
            perge;
        }

        item->pater = nodus;
        {
            ArborNodus** slot = xar_addere(nodus->datum.compositum.sententiae);
            si (slot != NIHIL) { *slot = item; }
        }
    }

    _expectare(syn, ARBOR_LEXEMA_BRACE_CLAUSA, "Expectabatur }");

    _finire_nodum(syn, nodus);
    redde nodus;
}

interior ArborNodus*
_parsere_sententia(ArborSyntaxis* syn)
{
    ArborLexema* lex;

    si (syn->panico)
    {
        redde NIHIL;
    }

    lex = _currens_lex(syn);
    si (lex == NIHIL || lex->genus == ARBOR_LEXEMA_EOF)
    {
        redde NIHIL;
    }

    commutatio (lex->genus)
    {
        casus ARBOR_LEXEMA_BRACE_APERTA:
            redde _parsere_compound(syn);

        casus ARBOR_LEXEMA_IF:
            redde _parsere_if(syn);

        casus ARBOR_LEXEMA_SWITCH:
            redde _parsere_switch(syn);

        casus ARBOR_LEXEMA_WHILE:
            redde _parsere_while(syn);

        casus ARBOR_LEXEMA_DO:
            redde _parsere_do(syn);

        casus ARBOR_LEXEMA_FOR:
            redde _parsere_for(syn);

        casus ARBOR_LEXEMA_GOTO:
            redde _parsere_goto(syn);

        casus ARBOR_LEXEMA_CONTINUE:
            redde _parsere_continue(syn);

        casus ARBOR_LEXEMA_BREAK:
            redde _parsere_break(syn);

        casus ARBOR_LEXEMA_RETURN:
            redde _parsere_return(syn);

        casus ARBOR_LEXEMA_CASE:
            redde _parsere_case(syn);

        casus ARBOR_LEXEMA_DEFAULT:
            redde _parsere_default(syn);

        casus ARBOR_LEXEMA_SEMICOLON:
            /* Empty statement */
            {
                ArborNodus* empty;
                empty = _creare_nodum(syn, ARBOR_NODUS_EXPRESSION_STATEMENT);
                si (empty != NIHIL)
                {
                    empty->datum.genericum.liberi = NIHIL;
                }
                _progredi(syn);
                _finire_nodum(syn, empty);
                redde empty;
            }

        casus ARBOR_LEXEMA_IDENTIFICATOR:
            /* Check for label: */
            {
                ArborLexema* next;
                next = _prospicere_lex(syn, I);
                si (next != NIHIL && next->genus == ARBOR_LEXEMA_COLON)
                {
                    redde _parsere_labeled(syn);
                }
            }
            /* Fall through to expression statement */

        ordinarius:
            redde _parsere_expression_statement(syn);
    }
}

/* ==================================================
 * Declaration Parsing
 * ================================================== */

interior ArborNodus*
_parsere_type_specifier(ArborSyntaxis* syn)
{
    ArborNodus* nodus;
    ArborLexema* lex;

    lex = _currens_lex(syn);
    si (lex == NIHIL)
    {
        redde NIHIL;
    }

    nodus = _creare_nodum(syn, ARBOR_NODUS_TYPE_SPECIFIER);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    nodus->datum.folium.keyword = lex->genus;
    nodus->datum.folium.valor = chorda_internare(syn->intern, lex->valor);

    _progredi(syn);
    _finire_nodum(syn, nodus);
    redde nodus;
}

interior ArborNodus*
_parsere_type_qualifier(ArborSyntaxis* syn)
{
    ArborNodus* nodus;
    ArborLexema* lex;

    lex = _currens_lex(syn);
    si (lex == NIHIL)
    {
        redde NIHIL;
    }

    nodus = _creare_nodum(syn, ARBOR_NODUS_TYPE_QUALIFIER);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    nodus->datum.folium.keyword = lex->genus;
    nodus->datum.folium.valor = chorda_internare(syn->intern, lex->valor);

    _progredi(syn);
    _finire_nodum(syn, nodus);
    redde nodus;
}

interior ArborNodus*
_parsere_storage_class(ArborSyntaxis* syn)
{
    ArborNodus* nodus;
    ArborLexema* lex;

    lex = _currens_lex(syn);
    si (lex == NIHIL)
    {
        redde NIHIL;
    }

    nodus = _creare_nodum(syn, ARBOR_NODUS_STORAGE_CLASS);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    nodus->datum.folium.keyword = lex->genus;
    nodus->datum.folium.valor = chorda_internare(syn->intern, lex->valor);

    _progredi(syn);
    _finire_nodum(syn, nodus);
    redde nodus;
}

interior ArborNodus*
_parsere_specifiers(ArborSyntaxis* syn, Xar* specifiers)
{
    ArborLexema* lex;
    ArborNodus* spec;

    dum (!_est_finis(syn))
    {
        lex = _currens_lex(syn);
        si (lex == NIHIL)
        {
            frange;
        }

        si (_est_storage_class(lex->genus))
        {
            spec = _parsere_storage_class(syn);
        }
        alioquin si (_est_type_qualifier(lex->genus))
        {
            spec = _parsere_type_qualifier(syn);
        }
        alioquin si (_est_type_specifier(lex->genus))
        {
            spec = _parsere_type_specifier(syn);
        }
        alioquin
        {
            frange;
        }

        si (spec == NIHIL)
        {
            redde NIHIL;
        }
        {
            ArborNodus** slot = xar_addere(specifiers);
            si (slot != NIHIL) { *slot = spec; }
        }
    }

    redde (ArborNodus*)I;  /* Non-null to indicate success */
}

interior ArborNodus*
_parsere_declarator(ArborSyntaxis* syn)
{
    ArborNodus* nodus;
    ArborNodus* inner;
    ArborLexema* lex;

    nodus = _creare_nodum(syn, ARBOR_NODUS_DECLARATOR);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    nodus->datum.genericum.liberi = xar_creare(syn->piscina, magnitudo(ArborNodus*));

    /* Handle pointer(s) */
    dum (_congruit(syn, ARBOR_LEXEMA_ASTERISCUS))
    {
        ArborNodus* ptr;
        ptr = _creare_nodum(syn, ARBOR_NODUS_POINTER);
        si (ptr == NIHIL)
        {
            redde NIHIL;
        }
        ptr->datum.pointer.qualifiers = xar_creare(syn->piscina, magnitudo(ArborNodus*));
        _progredi(syn);

        /* Qualifiers after * */
        dum (_congruit(syn, ARBOR_LEXEMA_CONST) || _congruit(syn, ARBOR_LEXEMA_VOLATILE))
        {
            ArborNodus* qual;
            qual = _parsere_type_qualifier(syn);
            si (qual != NIHIL)
            {
                ArborNodus** slot = xar_addere(ptr->datum.pointer.qualifiers);
                si (slot != NIHIL) { *slot = qual; }
            }
        }

        _finire_nodum(syn, ptr);
        {
            ArborNodus** slot = xar_addere(nodus->datum.genericum.liberi);
            si (slot != NIHIL) { *slot = ptr; }
        }
    }

    /* Direct declarator */
    lex = _currens_lex(syn);
    si (lex == NIHIL)
    {
        _error(syn, "Expectabatur declarator");
        redde NIHIL;
    }

    si (lex->genus == ARBOR_LEXEMA_IDENTIFICATOR)
    {
        inner = _parsere_identificator(syn);
        si (inner != NIHIL)
        {
            inner->pater = nodus;
            {
                ArborNodus** slot = xar_addere(nodus->datum.genericum.liberi);
                si (slot != NIHIL) { *slot = inner; }
            }
        }
    }
    alioquin si (lex->genus == ARBOR_LEXEMA_PAREN_APERTA)
    {
        /* Parenthesized declarator */
        _progredi(syn);
        inner = _parsere_declarator(syn);
        si (inner != NIHIL)
        {
            inner->pater = nodus;
            {
                ArborNodus** slot = xar_addere(nodus->datum.genericum.liberi);
                si (slot != NIHIL) { *slot = inner; }
            }
        }
        _expectare(syn, ARBOR_LEXEMA_PAREN_CLAUSA, "Expectabatur )");
    }
    alioquin
    {
        /* Abstract declarator (no name) - allowed in some contexts */
    }

    /* Array suffix [] */
    dum (_congruit(syn, ARBOR_LEXEMA_BRACKET_APERTA))
    {
        ArborNodus* arr;
        arr = _creare_nodum(syn, ARBOR_NODUS_ARRAY_DECLARATOR);
        si (arr == NIHIL)
        {
            redde NIHIL;
        }

        _progredi(syn);  /* Skip [ */

        si (!_congruit(syn, ARBOR_LEXEMA_BRACKET_CLAUSA))
        {
            arr->datum.array.size = _parsere_expressio(syn, PREC_NIHIL);
        }
        alioquin
        {
            arr->datum.array.size = NIHIL;
        }

        _expectare(syn, ARBOR_LEXEMA_BRACKET_CLAUSA, "Expectabatur ]");
        _finire_nodum(syn, arr);
        {
            ArborNodus** slot = xar_addere(nodus->datum.genericum.liberi);
            si (slot != NIHIL) { *slot = arr; }
        }
    }

    /* Function suffix () */
    si (_congruit(syn, ARBOR_LEXEMA_PAREN_APERTA))
    {
        ArborNodus* func;
        func = _creare_nodum(syn, ARBOR_NODUS_FUNCTION_DECLARATOR);
        si (func == NIHIL)
        {
            redde NIHIL;
        }
        func->datum.genericum.liberi = xar_creare(syn->piscina, magnitudo(ArborNodus*));

        _progredi(syn);  /* Skip ( */

        /* Parse parameters */
        si (!_congruit(syn, ARBOR_LEXEMA_PAREN_CLAUSA))
        {
            /* Check for void parameter */
            si (_congruit(syn, ARBOR_LEXEMA_VOID))
            {
                ArborLexema* next;
                next = _prospicere_lex(syn, I);
                si (next != NIHIL && next->genus == ARBOR_LEXEMA_PAREN_CLAUSA)
                {
                    /* Just void - no parameters */
                    _progredi(syn);
                }
                alioquin
                {
                    /* void is type specifier for parameter */
                    salta parse_params;
                }
            }
            alioquin
            {
parse_params:
                fac {
                    ArborNodus* param;
                    Xar* param_specs;

                    param = _creare_nodum(syn, ARBOR_NODUS_PARAMETER_DECLARATION);
                    si (param == NIHIL)
                    {
                        redde NIHIL;
                    }

                    param_specs = xar_creare(syn->piscina, magnitudo(ArborNodus*));
                    _parsere_specifiers(syn, param_specs);
                    param->datum.parametrum.specifiers = param_specs;

                    /* Optional declarator */
                    lex = _currens_lex(syn);
                    si (lex != NIHIL &&
                        (lex->genus == ARBOR_LEXEMA_IDENTIFICATOR ||
                         lex->genus == ARBOR_LEXEMA_ASTERISCUS ||
                         lex->genus == ARBOR_LEXEMA_PAREN_APERTA))
                    {
                        param->datum.parametrum.declarator = _parsere_declarator(syn);
                    }
                    alioquin
                    {
                        param->datum.parametrum.declarator = NIHIL;
                    }

                    _finire_nodum(syn, param);
                    param->pater = func;
                    {
                        ArborNodus** slot = xar_addere(func->datum.genericum.liberi);
                        si (slot != NIHIL) { *slot = param; }
                    }

                } dum (_consumere(syn, ARBOR_LEXEMA_COMMA));
            }
        }

        _expectare(syn, ARBOR_LEXEMA_PAREN_CLAUSA, "Expectabatur )");
        _finire_nodum(syn, func);
        {
            ArborNodus** slot = xar_addere(nodus->datum.genericum.liberi);
            si (slot != NIHIL) { *slot = func; }
        }
    }

    _finire_nodum(syn, nodus);
    redde nodus;
}

interior ArborNodus*
_parsere_initializer(ArborSyntaxis* syn)
{
    ArborNodus* nodus;

    si (_congruit(syn, ARBOR_LEXEMA_BRACE_APERTA))
    {
        /* Initializer list { a, b, c } */
        ArborNodus* elem;

        nodus = _creare_nodum(syn, ARBOR_NODUS_INITIALIZER_LIST);
        si (nodus == NIHIL)
        {
            redde NIHIL;
        }

        nodus->datum.init_list.elementa = xar_creare(syn->piscina, magnitudo(ArborNodus*));

        _progredi(syn);  /* Skip { */

        si (!_congruit(syn, ARBOR_LEXEMA_BRACE_CLAUSA))
        {
            fac {
                elem = _parsere_initializer(syn);
                si (elem == NIHIL)
                {
                    redde NIHIL;
                }
                elem->pater = nodus;
                {
                    ArborNodus** slot = xar_addere(nodus->datum.init_list.elementa);
                    si (slot != NIHIL) { *slot = elem; }
                }
            } dum (_consumere(syn, ARBOR_LEXEMA_COMMA) &&
                   !_congruit(syn, ARBOR_LEXEMA_BRACE_CLAUSA));
        }

        _expectare(syn, ARBOR_LEXEMA_BRACE_CLAUSA, "Expectabatur }");
        _finire_nodum(syn, nodus);
    }
    alioquin
    {
        /* Assignment expression */
        nodus = _parsere_expressio(syn, PREC_ASSIGNATIO);
    }

    redde nodus;
}

interior ArborNodus*
_parsere_declaratio(ArborSyntaxis* syn)
{
    ArborNodus* nodus;
    ArborNodus* decl;
    ArborNodus* init;
    Xar* specifiers;

    nodus = _creare_nodum(syn, ARBOR_NODUS_DECLARATION);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    specifiers = xar_creare(syn->piscina, magnitudo(ArborNodus*));
    nodus->datum.declaratio.specifiers = specifiers;
    nodus->datum.declaratio.declaratores = xar_creare(syn->piscina, magnitudo(ArborNodus*));

    /* Parse specifiers */
    _parsere_specifiers(syn, specifiers);

    /* Parse declarators */
    si (!_congruit(syn, ARBOR_LEXEMA_SEMICOLON))
    {
        fac {
            ArborNodus* init_decl;

            init_decl = _creare_nodum(syn, ARBOR_NODUS_INIT_DECLARATOR);
            si (init_decl == NIHIL)
            {
                redde NIHIL;
            }

            decl = _parsere_declarator(syn);
            si (decl == NIHIL)
            {
                redde NIHIL;
            }
            init_decl->datum.init_decl.declarator = decl;
            decl->pater = init_decl;

            /* Optional initializer */
            si (_consumere(syn, ARBOR_LEXEMA_ASSIGNATIO))
            {
                init = _parsere_initializer(syn);
                si (init == NIHIL)
                {
                    redde NIHIL;
                }
                init_decl->datum.init_decl.initializer = init;
                init->pater = init_decl;
            }
            alioquin
            {
                init_decl->datum.init_decl.initializer = NIHIL;
            }

            _finire_nodum(syn, init_decl);
            init_decl->pater = nodus;
            {
                ArborNodus** slot = xar_addere(nodus->datum.declaratio.declaratores);
                si (slot != NIHIL) { *slot = init_decl; }
            }

        } dum (_consumere(syn, ARBOR_LEXEMA_COMMA));
    }

    _expectare(syn, ARBOR_LEXEMA_SEMICOLON, "Expectabatur ;");

    _finire_nodum(syn, nodus);
    redde nodus;
}

/* ==================================================
 * Function Definition Parsing
 * ================================================== */

interior ArborNodus*
_parsere_function_definition(ArborSyntaxis* syn, Xar* specifiers, ArborNodus* declarator)
{
    ArborNodus* nodus;
    ArborNodus* corpus;

    nodus = _creare_nodum(syn, ARBOR_NODUS_FUNCTION_DEFINITION);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    nodus->datum.functio.specifiers = specifiers;
    nodus->datum.functio.declarator = declarator;
    nodus->datum.functio.old_params = NIHIL;  /* K&R style - not supported yet */
    declarator->pater = nodus;

    /* Parse compound statement */
    corpus = _parsere_compound(syn);
    si (corpus == NIHIL)
    {
        redde NIHIL;
    }
    nodus->datum.functio.corpus = corpus;
    corpus->pater = nodus;

    _finire_nodum(syn, nodus);
    redde nodus;
}

/* ==================================================
 * Translation Unit Parsing
 * ================================================== */

interior ArborNodus*
_parsere_external_declaration(ArborSyntaxis* syn)
{
    Xar* specifiers;
    ArborNodus* declarator;
    ArborNodus* result;

    specifiers = xar_creare(syn->piscina, magnitudo(ArborNodus*));

    /* Parse specifiers */
    _parsere_specifiers(syn, specifiers);

    /* Check for empty declaration (just specifiers + ;) */
    si (_congruit(syn, ARBOR_LEXEMA_SEMICOLON))
    {
        ArborNodus* decl;
        decl = _creare_nodum(syn, ARBOR_NODUS_DECLARATION);
        si (decl != NIHIL)
        {
            decl->datum.declaratio.specifiers = specifiers;
            decl->datum.declaratio.declaratores = xar_creare(syn->piscina, magnitudo(ArborNodus*));
        }
        _progredi(syn);
        _finire_nodum(syn, decl);
        redde decl;
    }

    /* Parse first declarator */
    declarator = _parsere_declarator(syn);
    si (declarator == NIHIL)
    {
        redde NIHIL;
    }

    /* Check if this is a function definition (has { ) */
    si (_congruit(syn, ARBOR_LEXEMA_BRACE_APERTA))
    {
        redde _parsere_function_definition(syn, specifiers, declarator);
    }

    /* Otherwise it's a declaration */
    {
        ArborNodus* nodus;
        ArborNodus* init_decl;
        ArborNodus* init;

        nodus = _creare_nodum(syn, ARBOR_NODUS_DECLARATION);
        si (nodus == NIHIL)
        {
            redde NIHIL;
        }

        nodus->datum.declaratio.specifiers = specifiers;
        nodus->datum.declaratio.declaratores = xar_creare(syn->piscina, magnitudo(ArborNodus*));

        /* Create init-declarator for first declarator */
        init_decl = _creare_nodum(syn, ARBOR_NODUS_INIT_DECLARATOR);
        si (init_decl == NIHIL)
        {
            redde NIHIL;
        }
        init_decl->datum.init_decl.declarator = declarator;
        declarator->pater = init_decl;

        /* Optional initializer */
        si (_consumere(syn, ARBOR_LEXEMA_ASSIGNATIO))
        {
            init = _parsere_initializer(syn);
            init_decl->datum.init_decl.initializer = init;
            si (init != NIHIL) init->pater = init_decl;
        }
        alioquin
        {
            init_decl->datum.init_decl.initializer = NIHIL;
        }

        _finire_nodum(syn, init_decl);
        init_decl->pater = nodus;
        {
            ArborNodus** slot = xar_addere(nodus->datum.declaratio.declaratores);
            si (slot != NIHIL) { *slot = init_decl; }
        }

        /* More declarators? */
        dum (_consumere(syn, ARBOR_LEXEMA_COMMA))
        {
            declarator = _parsere_declarator(syn);
            si (declarator == NIHIL)
            {
                redde NIHIL;
            }

            init_decl = _creare_nodum(syn, ARBOR_NODUS_INIT_DECLARATOR);
            si (init_decl == NIHIL)
            {
                redde NIHIL;
            }
            init_decl->datum.init_decl.declarator = declarator;
            declarator->pater = init_decl;

            si (_consumere(syn, ARBOR_LEXEMA_ASSIGNATIO))
            {
                init = _parsere_initializer(syn);
                init_decl->datum.init_decl.initializer = init;
                si (init != NIHIL) init->pater = init_decl;
            }
            alioquin
            {
                init_decl->datum.init_decl.initializer = NIHIL;
            }

            _finire_nodum(syn, init_decl);
            init_decl->pater = nodus;
            {
                ArborNodus** slot = xar_addere(nodus->datum.declaratio.declaratores);
                si (slot != NIHIL) { *slot = init_decl; }
            }
        }

        _expectare(syn, ARBOR_LEXEMA_SEMICOLON, "Expectabatur ;");
        _finire_nodum(syn, nodus);
        result = nodus;
    }

    redde result;
}

interior ArborNodus*
_parsere_translation_unit(ArborSyntaxis* syn)
{
    ArborNodus* unit;
    ArborNodus* decl;
    i32 prev_pos;

    unit = _creare_nodum(syn, ARBOR_NODUS_TRANSLATION_UNIT);
    si (unit == NIHIL)
    {
        redde NIHIL;
    }

    unit->datum.genericum.liberi = xar_creare(syn->piscina, magnitudo(ArborNodus*));

    dum (!_est_finis(syn))
    {
        prev_pos = syn->positus;
        decl = _parsere_external_declaration(syn);

        si (decl == NIHIL)
        {
            /* Evitare infinitum loop - progredi si non movimus */
            si (syn->positus == prev_pos && !_est_finis(syn))
            {
                _error(syn, "Unexpected token in translation unit");
                _progredi(syn);  /* Saltare token problematicum */
            }
            perge;
        }

        decl->pater = unit;
        {
            ArborNodus** slot = xar_addere(unit->datum.genericum.liberi);
            si (slot != NIHIL) { *slot = decl; }
        }
    }

    _finire_nodum(syn, unit);
    redde unit;
}

/* ==================================================
 * API Implementation
 * ================================================== */

ArborSyntaxis*
arbor_syntaxis_creare(
    Piscina*              piscina,
    InternamentumChorda*  intern)
{
    ArborSyntaxis* syn;

    si (piscina == NIHIL || intern == NIHIL)
    {
        redde NIHIL;
    }

    syn = piscina_allocare(piscina, magnitudo(ArborSyntaxis));
    si (syn == NIHIL)
    {
        redde NIHIL;
    }

    syn->piscina = piscina;
    syn->intern = intern;
    syn->lexemata = NIHIL;
    syn->positus = ZEPHYRUM;
    syn->numerus = ZEPHYRUM;
    syn->errores = xar_creare(piscina, magnitudo(ArborError));
    syn->panico = FALSUM;

    redde syn;
}

ArborSyntaxisResultus
arbor_syntaxis_parsere(
    ArborSyntaxis*        syn,
    Xar*                  lexemata)
{
    ArborSyntaxisResultus res;

    res.successus = FALSUM;
    res.radix = NIHIL;
    res.errores = NIHIL;

    si (syn == NIHIL || lexemata == NIHIL)
    {
        redde res;
    }

    /* Setup */
    syn->lexemata = lexemata;
    syn->positus = ZEPHYRUM;
    syn->numerus = xar_numerus(lexemata);
    syn->panico = FALSUM;

    /* Clear previous errors */
    syn->errores = xar_creare(syn->piscina, magnitudo(ArborError));

    /* Parse */
    res.radix = _parsere_translation_unit(syn);
    res.errores = syn->errores;

    si (res.radix != NIHIL && xar_numerus(syn->errores) == ZEPHYRUM)
    {
        res.successus = VERUM;
    }

    redde res;
}

ArborSyntaxisResultus
arbor_syntaxis_parsere_fontem(
    ArborSyntaxis*        syn,
    constans character*   fons,
    i32                   mensura,
    constans character*   via_file)
{
    ArborSyntaxisResultus res;
    ArborLexator* lexator;
    ArborPraeparator* pp;
    Xar* lexemata;
    Xar* expandita;

    res.successus = FALSUM;
    res.radix = NIHIL;
    res.errores = NIHIL;

    si (syn == NIHIL || fons == NIHIL)
    {
        redde res;
    }

    /* Lex */
    lexator = arbor_lexator_creare(syn->piscina, syn->intern, fons, mensura);
    si (lexator == NIHIL)
    {
        redde res;
    }

    lexemata = arbor_lexema_omnia(lexator);
    si (lexemata == NIHIL)
    {
        redde res;
    }

    /* Preprocess */
    pp = arbor_praeparator_creare(syn->piscina, syn->intern);
    si (pp == NIHIL)
    {
        redde res;
    }

    expandita = arbor_praeparator_processare_lexemata(pp, lexemata, via_file);
    si (expandita == NIHIL)
    {
        redde res;
    }

    /* Parse */
    redde arbor_syntaxis_parsere(syn, expandita);
}

/* ==================================================
 * Utilities
 * ================================================== */

constans character*
arbor_nodus_genus_nomen(ArborNodusGenus genus)
{
    commutatio (genus)
    {
        casus ARBOR_NODUS_TRANSLATION_UNIT:     redde "translation-unit";
        casus ARBOR_NODUS_FUNCTION_DEFINITION:  redde "function-definition";
        casus ARBOR_NODUS_DECLARATION:          redde "declaration";
        casus ARBOR_NODUS_PARAMETER_DECLARATION:redde "parameter-declaration";
        casus ARBOR_NODUS_STRUCT_SPECIFIER:     redde "struct-specifier";
        casus ARBOR_NODUS_UNION_SPECIFIER:      redde "union-specifier";
        casus ARBOR_NODUS_ENUM_SPECIFIER:       redde "enum-specifier";
        casus ARBOR_NODUS_ENUMERATOR:           redde "enumerator";
        casus ARBOR_NODUS_TYPEDEF_NAME:         redde "typedef-name";
        casus ARBOR_NODUS_DECLARATOR:           redde "declarator";
        casus ARBOR_NODUS_POINTER:              redde "pointer";
        casus ARBOR_NODUS_ARRAY_DECLARATOR:     redde "array-declarator";
        casus ARBOR_NODUS_FUNCTION_DECLARATOR:  redde "function-declarator";
        casus ARBOR_NODUS_PARAMETER_LIST:       redde "parameter-list";
        casus ARBOR_NODUS_INIT_DECLARATOR:      redde "init-declarator";
        casus ARBOR_NODUS_TYPE_SPECIFIER:       redde "type-specifier";
        casus ARBOR_NODUS_TYPE_QUALIFIER:       redde "type-qualifier";
        casus ARBOR_NODUS_STORAGE_CLASS:        redde "storage-class";
        casus ARBOR_NODUS_COMPOUND_STATEMENT:   redde "compound-statement";
        casus ARBOR_NODUS_IF_STATEMENT:         redde "if-statement";
        casus ARBOR_NODUS_SWITCH_STATEMENT:     redde "switch-statement";
        casus ARBOR_NODUS_CASE_LABEL:           redde "case-label";
        casus ARBOR_NODUS_DEFAULT_LABEL:        redde "default-label";
        casus ARBOR_NODUS_WHILE_STATEMENT:      redde "while-statement";
        casus ARBOR_NODUS_DO_STATEMENT:         redde "do-statement";
        casus ARBOR_NODUS_FOR_STATEMENT:        redde "for-statement";
        casus ARBOR_NODUS_GOTO_STATEMENT:       redde "goto-statement";
        casus ARBOR_NODUS_CONTINUE_STATEMENT:   redde "continue-statement";
        casus ARBOR_NODUS_BREAK_STATEMENT:      redde "break-statement";
        casus ARBOR_NODUS_RETURN_STATEMENT:     redde "return-statement";
        casus ARBOR_NODUS_LABELED_STATEMENT:    redde "labeled-statement";
        casus ARBOR_NODUS_EXPRESSION_STATEMENT: redde "expression-statement";
        casus ARBOR_NODUS_BINARY_EXPRESSION:    redde "binary-expression";
        casus ARBOR_NODUS_UNARY_EXPRESSION:     redde "unary-expression";
        casus ARBOR_NODUS_POSTFIX_EXPRESSION:   redde "postfix-expression";
        casus ARBOR_NODUS_CALL_EXPRESSION:      redde "call-expression";
        casus ARBOR_NODUS_MEMBER_EXPRESSION:    redde "member-expression";
        casus ARBOR_NODUS_SUBSCRIPT_EXPRESSION: redde "subscript-expression";
        casus ARBOR_NODUS_CAST_EXPRESSION:      redde "cast-expression";
        casus ARBOR_NODUS_SIZEOF_EXPRESSION:    redde "sizeof-expression";
        casus ARBOR_NODUS_CONDITIONAL_EXPRESSION: redde "conditional-expression";
        casus ARBOR_NODUS_ASSIGNMENT_EXPRESSION:redde "assignment-expression";
        casus ARBOR_NODUS_COMMA_EXPRESSION:     redde "comma-expression";
        casus ARBOR_NODUS_IDENTIFIER:           redde "identifier";
        casus ARBOR_NODUS_INTEGER_LITERAL:      redde "integer-literal";
        casus ARBOR_NODUS_FLOAT_LITERAL:        redde "float-literal";
        casus ARBOR_NODUS_CHAR_LITERAL:         redde "char-literal";
        casus ARBOR_NODUS_STRING_LITERAL:       redde "string-literal";
        casus ARBOR_NODUS_INITIALIZER:          redde "initializer";
        casus ARBOR_NODUS_INITIALIZER_LIST:     redde "initializer-list";
        casus ARBOR_NODUS_ERROR:                redde "error";
        casus ARBOR_NODUS_EXTENSION:            redde "extension";
        ordinarius:                             redde "unknown";
    }
}

vacuum
arbor_nodus_imprimere(ArborNodus* nodus, i32 indentatio)
{
    i32 i;
    i32 num;

    si (nodus == NIHIL)
    {
        per (i = ZEPHYRUM; i < indentatio; i++)
        {
            imprimere("  ");
        }
        imprimere("(nihil)\n");
        redde;
    }

    /* Indentatio */
    per (i = ZEPHYRUM; i < indentatio; i++)
    {
        imprimere("  ");
    }

    /* Genus */
    imprimere("%s", arbor_nodus_genus_nomen(nodus->genus));

    /* Extra info based on type */
    commutatio (nodus->genus)
    {
        casus ARBOR_NODUS_IDENTIFIER:
        casus ARBOR_NODUS_TYPE_SPECIFIER:
        casus ARBOR_NODUS_TYPE_QUALIFIER:
        casus ARBOR_NODUS_STORAGE_CLASS:
            si (nodus->datum.folium.valor != NIHIL)
            {
                imprimere(" \"%.*s\"",
                    (integer)nodus->datum.folium.valor->mensura,
                    nodus->datum.folium.valor->datum);
            }
            frange;
        casus ARBOR_NODUS_INTEGER_LITERAL:
            imprimere(" = %lld", nodus->datum.numerus.valor);
            frange;
        casus ARBOR_NODUS_FLOAT_LITERAL:
            imprimere(" = %g", nodus->datum.floating.valor);
            frange;
        casus ARBOR_NODUS_BINARY_EXPRESSION:
            imprimere(" [%s]", arbor_lexema_genus_nomen(nodus->datum.binarium.operator));
            frange;
        casus ARBOR_NODUS_UNARY_EXPRESSION:
            imprimere(" [%s%s]",
                nodus->datum.unarium.est_postfix ? "postfix " : "",
                arbor_lexema_genus_nomen(nodus->datum.unarium.operator));
            frange;
        ordinarius:
            frange;
    }

    imprimere(" @%d:%d\n", nodus->linea_initium, nodus->columna_initium);

    /* Print children based on node type */
    commutatio (nodus->genus)
    {
        casus ARBOR_NODUS_BINARY_EXPRESSION:
            arbor_nodus_imprimere(nodus->datum.binarium.sinister, indentatio + I);
            arbor_nodus_imprimere(nodus->datum.binarium.dexter, indentatio + I);
            frange;
        casus ARBOR_NODUS_UNARY_EXPRESSION:
            arbor_nodus_imprimere(nodus->datum.unarium.operandum, indentatio + I);
            frange;
        casus ARBOR_NODUS_CONDITIONAL_EXPRESSION:
            arbor_nodus_imprimere(nodus->datum.ternarium.conditio, indentatio + I);
            arbor_nodus_imprimere(nodus->datum.ternarium.verum, indentatio + I);
            arbor_nodus_imprimere(nodus->datum.ternarium.falsum, indentatio + I);
            frange;
        casus ARBOR_NODUS_CALL_EXPRESSION:
            arbor_nodus_imprimere(nodus->datum.vocatio.callee, indentatio + I);
            si (nodus->datum.vocatio.argumenta != NIHIL)
            {
                num = xar_numerus(nodus->datum.vocatio.argumenta);
                per (i = ZEPHYRUM; i < num; i++)
                {
                    ArborNodus** np;
                    np = xar_obtinere(nodus->datum.vocatio.argumenta, i);
                    arbor_nodus_imprimere(*np, indentatio + I);
                }
            }
            frange;
        casus ARBOR_NODUS_IF_STATEMENT:
            arbor_nodus_imprimere(nodus->datum.conditionale.conditio, indentatio + I);
            arbor_nodus_imprimere(nodus->datum.conditionale.consequens, indentatio + I);
            si (nodus->datum.conditionale.alternans != NIHIL)
            {
                arbor_nodus_imprimere(nodus->datum.conditionale.alternans, indentatio + I);
            }
            frange;
        casus ARBOR_NODUS_COMPOUND_STATEMENT:
            si (nodus->datum.compositum.sententiae != NIHIL)
            {
                num = xar_numerus(nodus->datum.compositum.sententiae);
                per (i = ZEPHYRUM; i < num; i++)
                {
                    ArborNodus** np;
                    np = xar_obtinere(nodus->datum.compositum.sententiae, i);
                    arbor_nodus_imprimere(*np, indentatio + I);
                }
            }
            frange;
        casus ARBOR_NODUS_TRANSLATION_UNIT:
        casus ARBOR_NODUS_DECLARATOR:
        casus ARBOR_NODUS_EXPRESSION_STATEMENT:
        casus ARBOR_NODUS_FUNCTION_DECLARATOR:
            si (nodus->datum.genericum.liberi != NIHIL)
            {
                num = xar_numerus(nodus->datum.genericum.liberi);
                per (i = ZEPHYRUM; i < num; i++)
                {
                    ArborNodus** np;
                    np = xar_obtinere(nodus->datum.genericum.liberi, i);
                    arbor_nodus_imprimere(*np, indentatio + I);
                }
            }
            frange;
        casus ARBOR_NODUS_FUNCTION_DEFINITION:
            si (nodus->datum.functio.specifiers != NIHIL)
            {
                num = xar_numerus(nodus->datum.functio.specifiers);
                per (i = ZEPHYRUM; i < num; i++)
                {
                    ArborNodus** np;
                    np = xar_obtinere(nodus->datum.functio.specifiers, i);
                    arbor_nodus_imprimere(*np, indentatio + I);
                }
            }
            arbor_nodus_imprimere(nodus->datum.functio.declarator, indentatio + I);
            arbor_nodus_imprimere(nodus->datum.functio.corpus, indentatio + I);
            frange;
        casus ARBOR_NODUS_DECLARATION:
            si (nodus->datum.declaratio.specifiers != NIHIL)
            {
                num = xar_numerus(nodus->datum.declaratio.specifiers);
                per (i = ZEPHYRUM; i < num; i++)
                {
                    ArborNodus** np;
                    np = xar_obtinere(nodus->datum.declaratio.specifiers, i);
                    arbor_nodus_imprimere(*np, indentatio + I);
                }
            }
            si (nodus->datum.declaratio.declaratores != NIHIL)
            {
                num = xar_numerus(nodus->datum.declaratio.declaratores);
                per (i = ZEPHYRUM; i < num; i++)
                {
                    ArborNodus** np;
                    np = xar_obtinere(nodus->datum.declaratio.declaratores, i);
                    arbor_nodus_imprimere(*np, indentatio + I);
                }
            }
            frange;
        casus ARBOR_NODUS_INIT_DECLARATOR:
            arbor_nodus_imprimere(nodus->datum.init_decl.declarator, indentatio + I);
            si (nodus->datum.init_decl.initializer != NIHIL)
            {
                arbor_nodus_imprimere(nodus->datum.init_decl.initializer, indentatio + I);
            }
            frange;
        ordinarius:
            frange;
    }
}

chorda
arbor_error_formare(Piscina* piscina, ArborError* err)
{
    character buffer[CCLVI];

    si (err == NIHIL)
    {
        redde chorda_ex_literis("(nihil error)", piscina);
    }

    (vacuum)snprintf(buffer, CCLVI, "%d:%d: %.*s",
        err->linea,
        err->columna,
        (integer)err->nuntius.mensura,
        (constans character*)err->nuntius.datum);

    redde chorda_ex_literis(buffer, piscina);
}
