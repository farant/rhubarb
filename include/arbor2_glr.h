#ifndef ARBOR2_GLR_H
#define ARBOR2_GLR_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"
#include "arbor2_lexema.h"
#include "arbor2_token.h"
#include "arbor2_expandere.h"

/* ==================================================
 * ARBOR2 GLR - Generalized LR Parser
 *
 * Handles C89 ambiguities (foo * bar, casts, sizeof)
 * via fork/merge with piscina checkpointing.
 *
 * EXEMPLUM:
 *   Arbor2GLR* glr = arbor2_glr_creare(piscina, intern, expansion);
 *   Arbor2GLRResultus res = arbor2_glr_parsere(glr, tokens);
 * ================================================== */

/* ==================================================
 * Action Types
 * ================================================== */

nomen enumeratio {
    ARBOR2_ACTIO_SHIFT,         /* Push state, consume token */
    ARBOR2_ACTIO_REDUCE,        /* Apply grammar rule */
    ARBOR2_ACTIO_ACCEPT,        /* Parse complete */
    ARBOR2_ACTIO_ERROR          /* No valid action */
} Arbor2ActioGenus;

/* ==================================================
 * Ambiguity Types
 * ================================================== */

nomen enumeratio {
    ARBOR2_AMBIG_ERROR,             /* Parse error */
    ARBOR2_AMBIG_TYPEDEF_IGNOTUM,   /* Unknown typedef */
    ARBOR2_AMBIG_MACRO_IGNOTUM      /* Unknown macro */
} Arbor2AmbigGenus;

/* ==================================================
 * Storage Class Flags (only one allowed in C89)
 * ================================================== */

#define ARBOR2_STORAGE_NONE         0
#define ARBOR2_STORAGE_TYPEDEF      (1 << 0)
#define ARBOR2_STORAGE_STATIC       (1 << 1)
#define ARBOR2_STORAGE_EXTERN       (1 << 2)
#define ARBOR2_STORAGE_REGISTER     (1 << 3)
#define ARBOR2_STORAGE_AUTO         (1 << 4)

/* ==================================================
 * Type Qualifier Flags (can combine)
 * ================================================== */

#define ARBOR2_QUAL_NONE            0
#define ARBOR2_QUAL_CONST           (1 << 0)
#define ARBOR2_QUAL_VOLATILE        (1 << 1)

/* ==================================================
 * Pointer Level (for roundtrip token preservation)
 * Each pointer level can have its own qualifiers:
 * e.g., int * const * volatile * p
 * ================================================== */

nomen structura {
    Arbor2Token*    tok_stella;      /* The * token */
    Arbor2Token*    tok_const;       /* NIHIL if no const */
    Arbor2Token*    tok_volatile;    /* NIHIL if no volatile */
} Arbor2PointerLevel;

/* ==================================================
 * Array Dimension (for roundtrip token preservation)
 * Each dimension stores its bracket tokens:
 * e.g., int arr[10][20][] has 3 dimensions
 * ================================================== */

/* Forward declaration */
structura Arbor2Nodus;

nomen structura {
    Arbor2Token*    tok_bracket_ap;  /* The [ token */
    structura Arbor2Nodus* dimensio; /* Size expression, NIHIL for unsized [] */
    Arbor2Token*    tok_bracket_cl;  /* The ] token */
} Arbor2ArrayDimension;

/* ==================================================
 * Non-Terminal Symbols
 * ================================================== */

nomen enumeratio {
    ARBOR2_NT_VIRGA,                /* comma expressions (,) - lowest precedence */
    ARBOR2_NT_ASSIGNATIO,           /* assignment expressions (= += -= etc) - right-associative */
    ARBOR2_NT_TERNARIUS,            /* ternary conditional (? :) - right-associative */
    ARBOR2_NT_DISIUNCTIO,           /* logical OR expressions (||) */
    ARBOR2_NT_CONIUNCTIO,           /* logical AND expressions (&&) */
    ARBOR2_NT_PIPA_BITWISE,         /* bitwise OR expressions (|) */
    ARBOR2_NT_CARET_BITWISE,        /* bitwise XOR expressions (^) */
    ARBOR2_NT_AMPERSAND_BITWISE,    /* bitwise AND expressions (&) */
    ARBOR2_NT_AEQUALITAS,           /* equality expressions (==, !=) */
    ARBOR2_NT_COMPARATIO,           /* relational expressions (<, >, <=, >=) */
    ARBOR2_NT_TRANSLATIO,           /* shift expressions (<<, >>) */
    ARBOR2_NT_EXPRESSIO,
    ARBOR2_NT_TERMINUS,             /* term in expr grammar */
    ARBOR2_NT_FACTOR,               /* factor in expr grammar */
    ARBOR2_NT_POSTFIXUM,            /* postfix expression (highest precedence) */
    ARBOR2_NT_ARGUMENTA,            /* function call argument list */
    ARBOR2_NT_PRIMARIUM,
    ARBOR2_NT_UNARIUM,
    ARBOR2_NT_BINARIUM,
    ARBOR2_NT_NOMEN_TYPI,
    ARBOR2_NT_SPECIFIER_TYPI,
    ARBOR2_NT_DECLARATIO,           /* Declaration */
    ARBOR2_NT_DECLARATOR,           /* Declarator */
    ARBOR2_NT_CONVERSIO,            /* Cast */
    ARBOR2_NT_SIZEOF,
    ARBOR2_NT_SENTENTIA,            /* Statement */
    ARBOR2_NT_CORPUS,               /* Compound statement */
    ARBOR2_NT_ELENCHUS_SENTENTIARUM, /* Statement list */
    ARBOR2_NT_SI,                   /* If statement */
    ARBOR2_NT_DUM,                  /* While statement */
    ARBOR2_NT_FAC,                  /* Do-while statement */
    ARBOR2_NT_PER,                  /* For statement */
    ARBOR2_NT_EXPRESSIO_OPTATIVA,   /* Optional expression (for for-loops) */
    ARBOR2_NT_NUMERUS,              /* Count */
    ARBOR2_NT_PARAMETER_LIST,       /* Parameter list */
    ARBOR2_NT_PARAMETER_DECL,       /* Parameter declaration */
    ARBOR2_NT_DEFINITIO_FUNCTI,     /* Function definition */
    ARBOR2_NT_STRUCT_SPECIFIER,     /* struct_specifier */
    ARBOR2_NT_STRUCT_MEMBER_LIST,   /* struct_member_list */
    ARBOR2_NT_ENUM_SPECIFIER,       /* enum_specifier */
    ARBOR2_NT_ENUMERATOR_LIST,      /* enumerator_list */
    ARBOR2_NT_ENUMERATOR,           /* single enumerator */
    ARBOR2_NT_INITIALIZOR_LISTA,    /* initializer list {a, b, c} */
    ARBOR2_NT_INIT_ITEMS,           /* list of initializers (internal) */
    ARBOR2_NT_INITIALIZER,          /* single initializer (expr or nested list) */
    ARBOR2_NT_DESIGNATOR,           /* [expr] or .field designator */
    ARBOR2_NT_DESIGNATOR_LIST,      /* chain of designators: [0][1].x */
    ARBOR2_NT_DESIGNATOR_ITEM,      /* designator_list '=' initializer */
    ARBOR2_NT_INIT_DECLARATOR,      /* Single declarator with optional init */
    ARBOR2_NT_INIT_DECLARATOR_LIST  /* Comma-separated declarators */
} Arbor2NonTerminalis;

/* ==================================================
 * AST Node Types (minimal for expressions)
 * ================================================== */

nomen enumeratio {
    ARBOR2_NODUS_IDENTIFICATOR,
    ARBOR2_NODUS_INTEGER,
    ARBOR2_NODUS_FLOAT,             /* Float literal */
    ARBOR2_NODUS_CHAR,              /* Character literal */
    ARBOR2_NODUS_STRING,            /* String literal */
    ARBOR2_NODUS_BINARIUM,          /* Binary operation */
    ARBOR2_NODUS_UNARIUM,           /* Unary operation */
    ARBOR2_NODUS_CONVERSIO,         /* Cast expression */
    ARBOR2_NODUS_TERNARIUS,         /* Ternary conditional a ? b : c */
    ARBOR2_NODUS_SIZEOF,
    ARBOR2_NODUS_VOCATIO,           /* Function call: base(args) */
    ARBOR2_NODUS_SUBSCRIPTIO,       /* Array subscript: base[index] */
    ARBOR2_NODUS_MEMBRUM,           /* Member access: base.member or base->member */
    ARBOR2_NODUS_POST_UNARIUM,      /* Post-increment/decrement: x++ or x-- */
    ARBOR2_NODUS_PARENTHESIZED,     /* Parenthesized expression: (expr) */
    ARBOR2_NODUS_DECLARATIO,        /* Declaration: type *name */
    ARBOR2_NODUS_DECLARATOR,        /* Declarator: *name or name */
    ARBOR2_NODUS_DECLARATOR_FUNCTI, /* Function declarator: name() or name(void) */
    ARBOR2_NODUS_PARAMETER_DECL,    /* Parameter declaration: type name */
    ARBOR2_NODUS_PARAMETER_LIST,    /* Parameter list (intermediate during parsing) */
    ARBOR2_NODUS_DEFINITIO_FUNCTI,  /* Function definition: type name() { body } */
    ARBOR2_NODUS_STRUCT_SPECIFIER,  /* Struct specifier: struct name { members } */
    ARBOR2_NODUS_ENUM_SPECIFIER,    /* Enum specifier: enum name { enumerators } */
    ARBOR2_NODUS_ENUMERATOR,        /* Single enumerator: NAME or NAME = value */
    ARBOR2_NODUS_SENTENTIA,         /* Expression statement */
    ARBOR2_NODUS_SENTENTIA_VACUA,   /* Empty statement ; */
    ARBOR2_NODUS_CORPUS,            /* Compound statement { ... } */
    ARBOR2_NODUS_SI,                /* If statement */
    ARBOR2_NODUS_DUM,               /* While statement */
    ARBOR2_NODUS_FAC,               /* Do-while statement */
    ARBOR2_NODUS_PER,               /* For statement */
    ARBOR2_NODUS_FRANGE,            /* break statement */
    ARBOR2_NODUS_PERGE,             /* continue statement */
    ARBOR2_NODUS_REDDE,             /* return statement */
    ARBOR2_NODUS_SALTA,             /* goto statement */
    ARBOR2_NODUS_TITULATUM,         /* labeled statement */
    ARBOR2_NODUS_COMMUTATIO,        /* switch statement */
    ARBOR2_NODUS_CASUS,             /* case label */
    ARBOR2_NODUS_ORDINARIUS,        /* default label */
    ARBOR2_NODUS_INITIALIZOR_LISTA, /* Brace-enclosed initializer list */
    ARBOR2_NODUS_DESIGNATOR_ITEM,   /* Designated initializer: .x = 1 or [5] = 100 */
    ARBOR2_NODUS_LISTA_SEPARATA,    /* Comma-separated list with separator tokens */
    ARBOR2_NODUS_AMBIGUUS,          /* Ambiguous node */
    ARBOR2_NODUS_ERROR,
    ARBOR2_NODUS_TRANSLATION_UNIT,  /* Translation unit (list of external declarations) */
    ARBOR2_NODUS_COMMENTUM,         /* Comment node (promoted from trivia) */
    ARBOR2_NODUS_CONDITIONALIS      /* Conditional compilation (#ifdef/#ifndef/#else/#endif) */
} Arbor2NodusGenus;

/* ==================================================
 * Comment Subtype
 * ================================================== */

nomen enumeratio {
    ARBOR2_COMMENTUM_CLAUSUM,       /* Block comment (slash-star) */
    ARBOR2_COMMENTUM_LINEA,         /* Line comment (C99 slash-slash) */
    ARBOR2_COMMENTUM_DOC            /* Doc comment (future) */
} Arbor2CommentumGenus;

/* ==================================================
 * Conditional Directive Type
 * ================================================== */

nomen enumeratio {
    ARBOR2_DIRECTIVUM_IFDEF,        /* #ifdef MACRO */
    ARBOR2_DIRECTIVUM_IFNDEF,       /* #ifndef MACRO */
    ARBOR2_DIRECTIVUM_IF,           /* #if EXPR (Phase 5b) */
    ARBOR2_DIRECTIVUM_ELIF,         /* #elif EXPR (Phase 5b) */
    ARBOR2_DIRECTIVUM_ELSE,         /* #else */
    ARBOR2_DIRECTIVUM_ENDIF         /* #endif */
} Arbor2DirectivumGenus;

/* ==================================================
 * Conditional Branch (for CONDITIONALIS node)
 *
 * Represents one branch of a conditional (#ifdef, #else, etc.)
 * ================================================== */

nomen structura Arbor2CondRamus Arbor2CondRamus;

/* Forward declaration - defined below in AST Node section */
structura Arbor2Nodus;

structura Arbor2CondRamus {
    Arbor2DirectivumGenus   genus;              /* IFDEF, IFNDEF, ELSE, ELIF, IF */
    chorda*                 conditio;           /* Macro name (ifdef/ifndef) */
    Xar*                    expressio_lexemata; /* Expression tokens (#if/#elif) - Xar of Arbor2Token* */
    i64                     valor_evaluatus;    /* Evaluated value (0=false, nonzero=true) */
    b32                     est_evaluatum;      /* VERUM if evaluation succeeded */
    Xar*                    lexemata;           /* Tokens in this branch (Xar of Arbor2Token*) */
    structura Arbor2Nodus*  parsed;             /* Parsed AST for branch (or NIHIL) */
    i32                     linea;              /* Line of directive */
};

/* ==================================================
 * AST Node
 * ================================================== */

nomen structura Arbor2Nodus Arbor2Nodus;

structura Arbor2Nodus {
    Arbor2NodusGenus            genus;
    Arbor2Token*                lexema;         /* Associated token */

    /* Location span (invocation site, tight bounds) */
    i32                         linea_initium;
    i32                         columna_initium;
    i32                         linea_finis;
    i32                         columna_finis;
    i32                         layer_index;    /* 0 = source, >0 = macro-expanded */

    /* Parens nodus - NIHIL pro radice */
    structura Arbor2Nodus*      pater;

    /* Attached comments (promoted from trivia) */
    Xar*                        commenta_ante;  /* Xar of Arbor2Nodus* (COMMENTUM) */
    Xar*                        commenta_post;  /* Xar of Arbor2Nodus* (COMMENTUM) */

    unio {
        /* IDENTIFICATOR, INTEGER */
        structura {
            chorda              valor;
        } folium;

        /* BINARIUM */
        structura {
            Arbor2Nodus*        sinister;
            Arbor2Token*        tok_operator;   /* Operator token for roundtrip */
            Arbor2LexemaGenus   operator;       /* Operator type (kept for compatibility) */
            Arbor2Nodus*        dexter;
        } binarium;

        /* UNARIUM */
        structura {
            Arbor2Token*        tok_operator;   /* Operator token for roundtrip */
            Arbor2LexemaGenus   operator;       /* Operator type (kept for compatibility) */
            Arbor2Nodus*        operandum;
        } unarium;

        /* CONVERSIO (cast) */
        structura {
            Arbor2Token*        tok_paren_ap;   /* ( */
            Arbor2Nodus*        typus;
            Arbor2Token*        tok_paren_cl;   /* ) */
            Arbor2Nodus*        expressio;
        } conversio;

        /* TERNARIUS (ternary conditional) */
        structura {
            Arbor2Nodus*        conditio;       /* condition */
            Arbor2Token*        tok_interrogatio; /* ? */
            Arbor2Nodus*        verum;          /* true-branch */
            Arbor2Token*        tok_colon;      /* : */
            Arbor2Nodus*        falsum;         /* false-branch */
        } ternarius;

        /* SIZEOF */
        structura {
            Arbor2Token*        tok_sizeof;     /* 'sizeof' keyword */
            Arbor2Token*        tok_paren_ap;   /* ( - NIHIL for sizeof expr */
            b32                 est_typus;      /* sizeof(type) vs sizeof expr */
            Arbor2Nodus*        operandum;
            Arbor2Token*        tok_paren_cl;   /* ) - NIHIL for sizeof expr */
        } sizeof_expr;

        /* VOCATIO (function call) */
        structura {
            Arbor2Nodus*        basis;          /* Function expression */
            Arbor2Token*        tok_paren_ap;   /* ( */
            Arbor2Nodus*        argumenta;      /* LISTA_SEPARATA node, NIHIL if no args */
            Arbor2Token*        tok_paren_cl;   /* ) */
        } vocatio;

        /* SUBSCRIPTIO (array subscript) */
        structura {
            Arbor2Nodus*        basis;          /* Array expression */
            Arbor2Token*        tok_bracket_ap; /* [ */
            Arbor2Nodus*        index;          /* Index expression */
            Arbor2Token*        tok_bracket_cl; /* ] */
        } subscriptio;

        /* MEMBRUM (member access . or ->) */
        structura {
            Arbor2Nodus*        basis;          /* Struct/union expression */
            Arbor2Token*        tok_accessor;   /* . or -> token */
            Arbor2Token*        tok_membrum;    /* Member identifier token */
            chorda              membrum;        /* Member name */
            b32                 est_sagitta;    /* VERUM for ->, FALSUM for . */
        } membrum;

        /* POST_UNARIUM (post-increment/decrement) */
        structura {
            Arbor2Nodus*        operandum;
            Arbor2Token*        tok_operator;   /* Operator token for roundtrip */
            Arbor2LexemaGenus   operator;       /* DUPLUS or DUMINUS (kept for compatibility) */
        } post_unarium;

        /* PARENTHESIZED (parenthesized expression) */
        structura {
            Arbor2Token*        tok_paren_ap;   /* ( */
            Arbor2Nodus*        expressio;      /* Inner expression */
            Arbor2Token*        tok_paren_cl;   /* ) */
        } parenthesized;

        /* DECLARATIO */
        structura {
            Arbor2Token*        tok_storage;    /* static/extern/auto/register keyword (NIHIL if none) */
            Arbor2Token*        tok_const;      /* const keyword (NIHIL if none) */
            Arbor2Token*        tok_volatile;   /* volatile keyword (NIHIL if none) */
            Arbor2Token*        tok_unsigned;   /* unsigned keyword (NIHIL if none) */
            Arbor2Token*        tok_signed;     /* signed keyword (NIHIL if none) */
            Arbor2Token*        tok_long;       /* long keyword (NIHIL if none) */
            Arbor2Token*        tok_long2;      /* second long keyword for 'long long' (NIHIL if none) */
            Arbor2Token*        tok_short;      /* short keyword (NIHIL if none) */
            Arbor2Nodus*        specifier;      /* Type specifier (identifier) */
            Arbor2Nodus*        declarator;     /* The declarator (*name or name) */
            Arbor2Token*        tok_assignatio; /* = token (NIHIL if no initializer) */
            Arbor2Nodus*        initializor;    /* Initializer expression (NIHIL if none) */
            Arbor2Token*        tok_semicolon;  /* ; */
            Arbor2Nodus*        proxima;        /* Next declaration in comma-sep list */
            b32                 est_typedef;    /* VERUM if this is a typedef declaration */
            i32                 storage_class;  /* ARBOR2_STORAGE_* flags (only one in C89) */
            i32                 qualifiers;     /* ARBOR2_QUAL_* flags (can combine) */
        } declaratio;

        /* DECLARATOR */
        structura {
            Xar*                pointer_levels; /* Xar of Arbor2PointerLevel*, NIHIL if no pointers */
            chorda              titulus;        /* Variable name (empty for anonymous bit fields) */
            Arbor2Nodus*        latitudo_biti;  /* Bit field width expr, NIHIL if not bit field */
            Xar*                dimensiones;    /* Xar of Arbor2ArrayDimension*, NIHIL if not array */
        } declarator;

        /* DECLARATOR_FUNCTI (function declarator) */
        structura {
            Arbor2Nodus*        declarator_interior;  /* Inner declarator (name + pointers) */
            Arbor2Token*        tok_paren_ap;         /* ( */
            Arbor2Nodus*        parametri;            /* LISTA_SEPARATA node, NIHIL for () or (void) */
            Arbor2Token*        tok_paren_cl;         /* ) */
            Arbor2Token*        tok_void;             /* 'void' token for (void) params, NIHIL otherwise */
            b32                 habet_void;           /* true if explicitly (void) */
            b32                 est_variadicus;       /* true if ends with ... */
            Xar*                pointer_levels;       /* Xar of Arbor2PointerLevel*, NIHIL if no pointers */
        } declarator_functi;

        /* PARAMETER_DECL (parameter declaration) */
        structura {
            Arbor2Nodus*        type_specifier;       /* Type (identifier node) */
            Arbor2Nodus*        declarator;           /* Name + pointers */
        } parameter_decl;

        /* PARAMETER_LIST (intermediate representation during parsing) */
        structura {
            Xar*                parametra;            /* Xar of Arbor2Nodus* */
            b32                 est_variadicus;       /* true if ends with ... */
        } parameter_list;

        /* DEFINITIO_FUNCTI (function definition) */
        structura {
            Arbor2Nodus*        specifier;            /* Return type (identifier node) */
            Arbor2Nodus*        declarator;           /* Function declarator (DECLARATOR_FUNCTI) */
            Arbor2Nodus*        corpus;               /* Function body (CORPUS node) */
            Arbor2Token*        tok_const;            /* 'const' qualifier (P505) */
            Arbor2Token*        tok_volatile;         /* 'volatile' qualifier (P506) */
        } definitio_functi;

        /* STRUCT_SPECIFIER (also used for union) */
        structura {
            Arbor2Token*        tok_struct_or_union;  /* 'struct' or 'union' keyword */
            Arbor2Nodus*        tag;                  /* Tag name (IDENTIFICATOR) or NIHIL for anonymous */
            Arbor2Token*        tok_brace_ap;         /* { (NIHIL if forward decl) */
            Xar*                membra;               /* List of member declarations (DECLARATIO nodes) */
            Arbor2Token*        tok_brace_cl;         /* } (NIHIL if forward decl) */
            b32                 est_unio;             /* VERUM for union, FALSUM for struct */
            Arbor2Token*        tok_semicolon;        /* ; for standalone type definition */
        } struct_specifier;

        /* ENUM_SPECIFIER */
        structura {
            Arbor2Token*        tok_enum;             /* 'enum' keyword */
            Arbor2Nodus*        tag;                  /* Tag name (IDENTIFICATOR) or NIHIL for anonymous */
            Arbor2Token*        tok_brace_ap;         /* { (NIHIL if forward decl) */
            Arbor2Nodus*        enumeratores;         /* LISTA_SEPARATA node with ENUMERATOR nodes */
            Arbor2Token*        tok_brace_cl;         /* } (NIHIL if forward decl) */
            Arbor2Token*        tok_semicolon;        /* ; for standalone type definition */
        } enum_specifier;

        /* ENUMERATOR */
        structura {
            chorda              titulus;              /* Enumerator name */
            Arbor2Token*        tok_assignatio;       /* = token, NIHIL if no value */
            Arbor2Nodus*        valor;                /* Optional value expr, NIHIL if not specified */
        } enumerator;

        /* SENTENTIA (expression statement) */
        structura {
            Arbor2Nodus*        expressio;      /* The expression (NIHIL for empty stmt) */
            Arbor2Token*        tok_semicolon;  /* ; */
        } sententia;

        /* CORPUS (compound statement) */
        structura {
            Arbor2Token*        tok_brace_ap;   /* { */
            Xar*                sententiae;     /* Xar of Arbor2Nodus* - list of statements */
            Arbor2Token*        tok_brace_cl;   /* } */
        } corpus;

        /* SI (if statement) */
        structura {
            Arbor2Token*        tok_si;         /* 'if' keyword */
            Arbor2Token*        tok_paren_ap;   /* ( */
            Arbor2Nodus*        conditio;       /* condition expression */
            Arbor2Token*        tok_paren_cl;   /* ) */
            Arbor2Nodus*        consequens;     /* then-branch */
            Arbor2Token*        tok_alioquin;   /* 'else' keyword (NIHIL if no else) */
            Arbor2Nodus*        alternans;      /* else-branch (NIHIL if no else) */
        } conditionale;

        /* DUM/FAC (while/do-while statement) */
        structura {
            Arbor2Token*        tok_fac;        /* 'do' keyword (NIHIL for while) */
            Arbor2Token*        tok_dum;        /* 'while' keyword */
            Arbor2Token*        tok_paren_ap;   /* ( */
            Arbor2Nodus*        conditio;       /* loop condition */
            Arbor2Token*        tok_paren_cl;   /* ) */
            Arbor2Nodus*        corpus;         /* loop body */
            Arbor2Token*        tok_semicolon;  /* ; (for do-while only, NIHIL for while) */
        } iteratio;

        /* PER (for statement) */
        structura {
            Arbor2Token*        tok_per;        /* 'for' keyword */
            Arbor2Token*        tok_paren_ap;   /* ( */
            Arbor2Nodus*        initium;        /* init expression (NIHIL if omitted) */
            Arbor2Token*        tok_semicolon1; /* first ; */
            Arbor2Nodus*        conditio;       /* condition (NIHIL if omitted) */
            Arbor2Token*        tok_semicolon2; /* second ; */
            Arbor2Nodus*        incrementum;    /* increment (NIHIL if omitted) */
            Arbor2Token*        tok_paren_cl;   /* ) */
            Arbor2Nodus*        corpus;         /* loop body */
        } circuitus;

        /* REDDE (return statement) */
        structura {
            Arbor2Token*        tok_redde;      /* 'return' keyword */
            Arbor2Nodus*        valor;          /* return value (NIHIL if omitted) */
            Arbor2Token*        tok_semicolon;  /* ; */
        } reditio;

        /* SALTA (goto statement) */
        structura {
            Arbor2Token*        tok_salta;      /* 'goto' keyword */
            Arbor2Token*        tok_destinatio; /* label identifier token */
            chorda              destinatio;     /* label name */
            Arbor2Token*        tok_semicolon;  /* ; */
        } saltus;

        /* TITULATUM (labeled statement) */
        structura {
            Arbor2Token*        tok_titulus;    /* label identifier token */
            chorda              titulus;        /* label name */
            Arbor2Token*        tok_colon;      /* : */
            Arbor2Nodus*        sententia;      /* the labeled statement */
        } titulatum;

        /* COMMUTATIO (switch statement) */
        structura {
            Arbor2Token*        tok_commutatio; /* 'switch' keyword */
            Arbor2Token*        tok_paren_ap;   /* ( */
            Arbor2Nodus*        expressio;      /* switch expression */
            Arbor2Token*        tok_paren_cl;   /* ) */
            Arbor2Nodus*        corpus;         /* body (usually compound) */
        } selectivum;  /* 'commutatio' expands to 'switch' keyword */

        /* CASUS (case label) */
        structura {
            Arbor2Token*        tok_casus;      /* 'case' keyword */
            Arbor2Nodus*        valor;          /* constant expression */
            Arbor2Token*        tok_colon;      /* : */
            Arbor2Nodus*        sententia;      /* following statement */
        } electio;  /* 'casus' expands to 'case' keyword */

        /* ORDINARIUS (default label) */
        structura {
            Arbor2Token*        tok_ordinarius; /* 'default' keyword */
            Arbor2Token*        tok_colon;      /* : */
            Arbor2Nodus*        sententia;      /* following statement */
        } defectus;  /* 'ordinarius' expands to 'default' keyword */

        /* FRANGE (break statement) */
        structura {
            Arbor2Token*        tok_frange;     /* 'break' keyword */
            Arbor2Token*        tok_semicolon;  /* ; */
        } frangendum;

        /* PERGE (continue statement) */
        structura {
            Arbor2Token*        tok_perge;      /* 'continue' keyword */
            Arbor2Token*        tok_semicolon;  /* ; */
        } pergendum;

        /* INITIALIZOR_LISTA (brace-enclosed initializer) */
        structura {
            Arbor2Token*        tok_brace_ap;   /* { */
            Arbor2Nodus*        items;          /* LISTA_SEPARATA of initializers */
            Arbor2Token*        tok_brace_cl;   /* } */
        } initializor_lista;

        /* DESIGNATOR_ITEM (designated initializer: .x = 1 or [5] = 100) */
        structura {
            Xar*                designatores;   /* Xar of Arbor2Nodus* - chain of [i] or .field */
            Arbor2Nodus*        valor;          /* The initialized value (expression or init_lista) */
        } designator_item;

        /* LISTA_SEPARATA (comma-separated list with separator tokens for roundtrip) */
        structura {
            Xar*                elementa;       /* Xar of Arbor2Nodus* - the list items */
            Xar*                separatores;    /* Xar of Arbor2Token* - comma tokens between items */
        } lista_separata;

        /* TRANSLATION_UNIT (list of external declarations) */
        structura {
            Xar*                declarationes;      /* Xar of Arbor2Nodus* */
        } translation_unit;

        /* AMBIGUUS */
        structura {
            Arbor2AmbigGenus    genus;
            chorda*             identificator;
            Xar*                interpretationes;   /* Xar of Arbor2Nodus* */
        } ambiguus;

        /* ERROR */
        structura {
            chorda*             nuntius;
            Xar*                lexemata_saltata;   /* Skipped tokens */
        } error;

        /* COMMENTUM (promoted from spatia) */
        structura {
            Arbor2CommentumGenus    subgenus;       /* Block, line, or doc */
            chorda                  textus;         /* Comment text (without delimiters) */
            chorda                  textus_crudus;  /* Full original text */
            b32                     est_fluitans;   /* True if floating (blank lines around) */
            Xar*                    fragmenta;      /* Parsed sub-structure (NIHIL initially) */
            Xar*                    spatia_ante;    /* Leading whitespace (Xar of Arbor2Lexema*) */
            Xar*                    spatia_post;    /* Trailing whitespace (Xar of Arbor2Lexema*) */
        } commentum;

        /* CONDITIONALIS (conditional compilation) */
        structura {
            Xar*                    rami;           /* Xar of Arbor2CondRamus* */
            i32                     linea_if;       /* Line of opening #if/#ifdef */
            i32                     linea_endif;    /* Line of closing #endif */
        } conditionalis;
    } datum;
};

/* ==================================================
 * GSS Node (Graph-Structured Stack)
 * ================================================== */

nomen structura Arbor2GSSNodus Arbor2GSSNodus;

structura Arbor2GSSNodus {
    s32                         status;             /* LR state number */
    Arbor2GSSNodus**            praedecessores;     /* Parent nodes (array) */
    s32                         num_praed;          /* Number of predecessors */
    s32                         capacitas_praed;    /* Allocated capacity */
    Arbor2Nodus*                valor;              /* Semantic value (AST node) */
    Arbor2Token*                lexema;             /* Token at this point */
    PiscinaNotatio              punctum_salutis;    /* Checkpoint for rollback */
    s32                         furca_id;           /* Fork group ID (0 = no fork) */
};

/* ==================================================
 * LR Table Entry Types
 * ================================================== */

nomen structura {
    Arbor2LexemaGenus       lexema;         /* Token triggering action */
    Arbor2ActioGenus        actio;          /* SHIFT, REDUCE, ACCEPT, ERROR */
    i32                     valor;          /* State number or rule number */
    b32                     conflictus_intentus;  /* VERUM if GLR fork expected */
} Arbor2TabulaActio;

nomen structura {
    s32                     status;         /* Current state */
    s32                     non_terminalis; /* Non-terminal symbol (internal index) */
    s32                     status_novus;   /* Next state */
} Arbor2TabulaGoto;

/* ==================================================
 * State Metadata (for array-of-arrays structure)
 * ================================================== */

nomen structura {
    constans Arbor2TabulaActio* actiones;   /* Actions for this state */
    s32                         numerus;    /* Count (computed via sizeof) */
    constans character*         descriptio; /* Human-readable state context */
} Arbor2StatusInfo;

nomen structura {
    s32                     non_terminalis; /* Non-terminal symbol */
    s32                     status_novus;   /* Target state */
} Arbor2StatusGotoEntry;

nomen structura {
    constans Arbor2StatusGotoEntry* transitus;  /* GOTO entries for this state */
    s32                             numerus;    /* Count */
} Arbor2StatusGoto;

/* ==================================================
 * Grammar Rule
 * ================================================== */

nomen structura {
    Arbor2NonTerminalis     sinister;       /* Left-hand side */
    i32                     longitudo;      /* Number of symbols on right */
    Arbor2NodusGenus        nodus_genus;    /* AST node type to create */
    constans character*     descriptio;     /* Human-readable production */
} Arbor2Regula;

/* ==================================================
 * GLR Parser State
 * ================================================== */

nomen structura {
    Piscina*                piscina;
    InternamentumChorda*    intern;
    Arbor2Expansion*        expansion;      /* For macro/typedef lookahead */

    /* Token stream */
    Xar*                    lexemata;       /* Xar of Arbor2Token* */
    i32                     positus;        /* Current position */

    /* GSS state */
    Xar*                    frons_activa;   /* Xar of Arbor2GSSNodus* */
    Xar*                    frons_nova;     /* Next frontier being built */

    /* Tables (static references) */
    Arbor2TabulaActio*      actiones;
    i32                     num_actiones;
    Arbor2TabulaGoto*       goto_tabula;
    i32                     num_goto;
    Arbor2Regula*           regulae;
    i32                     num_regulae;

    /* Results */
    Xar*                    errores;        /* Xar of chorda* */
    Xar*                    ambigui;        /* Xar of Arbor2Nodus* (AMBIGUUS nodes) */

    /* Statistics */
    i32                     num_furcae;     /* Fork count */
    i32                     num_mergae;     /* Merge count */
    i32                     max_frons;      /* Max frontier size */

    /* Error recovery */
    i32                     num_errores;    /* Error count for recovery limit */
} Arbor2GLR;

/* Maximum errors before giving up recovery */
#define ARBOR2_GLR_MAX_ERRORES X

/* ==================================================
 * Parse Result
 * ================================================== */

nomen structura {
    b32                     successus;
    Arbor2Nodus*            radix;          /* Root AST node */
    Xar*                    ambigui;        /* Unresolved ambiguities */
    Xar*                    errores;        /* Parse errors */
    i32                     tokens_consumed; /* Number of tokens consumed */
} Arbor2GLRResultus;

/* ==================================================
 * API - Creation
 * ================================================== */

/* Create GLR parser with expansion context for lookahead */
Arbor2GLR* arbor2_glr_creare(
    Piscina*                piscina,
    InternamentumChorda*    intern,
    Arbor2Expansion*        expansion);

/* ==================================================
 * API - Parsing
 * ================================================== */

/* Parse token stream to AST */
Arbor2GLRResultus arbor2_glr_parsere(
    Arbor2GLR*              glr,
    Xar*                    lexemata);      /* Xar of Arbor2Token* */

/* Parse expression only (for testing) */
Arbor2GLRResultus arbor2_glr_parsere_expressio(
    Arbor2GLR*              glr,
    Xar*                    lexemata);

/* Parse translation unit (multiple declarations/functions) */
Arbor2GLRResultus arbor2_glr_parsere_translation_unit(
    Arbor2GLR*              glr,
    Xar*                    lexemata);

/* ==================================================
 * API - Queries
 * ================================================== */

/* Get action type name */
constans character* arbor2_actio_genus_nomen(Arbor2ActioGenus genus);

/* Get non-terminal name */
constans character* arbor2_nt_nomen(Arbor2NonTerminalis nt);

/* Get node type name */
constans character* arbor2_nodus_genus_nomen(Arbor2NodusGenus genus);

/* Check if token is probably a type (typedef or type-suggesting macro) */
b32 arbor2_glr_est_probabiliter_typus(Arbor2GLR* glr, Arbor2Token* tok);

/* ==================================================
 * API - AST Utilities
 * ================================================== */

/* Create AST nodes */
Arbor2Nodus* arbor2_nodus_creare_folium(
    Arbor2GLR*              glr,
    Arbor2NodusGenus        genus,
    Arbor2Token*            lexema);

Arbor2Nodus* arbor2_nodus_creare_binarium(
    Arbor2GLR*              glr,
    Arbor2Nodus*            sinister,
    Arbor2LexemaGenus       operator,
    Arbor2Nodus*            dexter);

Arbor2Nodus* arbor2_nodus_creare_unarium(
    Arbor2GLR*              glr,
    Arbor2LexemaGenus       operator,
    Arbor2Nodus*            operandum);

/* ==================================================
 * Internal - Table Lookup (defined in arbor2_glr_tabula.c)
 * ================================================== */

/* Get actions for state + token (may return multiple for ambiguity) */
Xar* arbor2_glr_quaerere_actiones(
    Arbor2GLR*              glr,
    i32                     status,
    Arbor2LexemaGenus       lexema);

/* Get goto state for state + non-terminal (-1 if not found) */
s32 arbor2_glr_quaerere_goto(
    Arbor2GLR*              glr,
    s32                     status,
    Arbor2NonTerminalis     nt);

/* Get grammar rule */
Arbor2Regula* arbor2_glr_obtinere_regula(
    Arbor2GLR*              glr,
    i32                     index);

/* Initialize tables (called during glr_creare) */
vacuum arbor2_glr_initializare_tabulas(Arbor2GLR* glr);

/* ==================================================
 * Table Query API (no GLR instance required)
 * For tools and debugging
 * ================================================== */

/* Table statistics */
i32 arbor2_tabula_numerus_statuum(vacuum);
i32 arbor2_tabula_numerus_regularum(vacuum);

/* Get rule by index (no GLR instance needed) */
Arbor2Regula* arbor2_tabula_obtinere_regula(i32 index);

/* Get state info (for new array-of-arrays structure) */
constans Arbor2StatusInfo* arbor2_tabula_obtinere_status_info(i32 status);

/* Validate table integrity (debug builds) */
b32 arbor2_glr_validare_tabulas(vacuum);

/* Get expected tokens for a state (for error messages) */
chorda* arbor2_glr_exspectata_pro_statu(Piscina* piscina, s32 status);

/* ==================================================
 * State Metadata Tags (for validation)
 * ================================================== */

#define ARBOR2_TAG_EXPR_INITIUM       0x0001  /* Can start an expression */
#define ARBOR2_TAG_CONDITIO_CONTEXTUS 0x0002  /* Condition context (if, while, etc.) */
#define ARBOR2_TAG_POST_COMPARATIO    0x0004  /* After comparison operator */
#define ARBOR2_TAG_POST_BINARIUM      0x0008  /* After binary operator (expects operand) */
#define ARBOR2_TAG_POST_DISIUNCTIO    0x0010  /* After || */
#define ARBOR2_TAG_POST_CONIUNCTIO    0x0020  /* After && */
#define ARBOR2_TAG_SENTENTIA_INITIUM  0x0040  /* Can start a statement */

/* Validate state tags - checks that tagged states have required tokens */
b32 arbor2_glr_validare_tags(vacuum);

#endif /* ARBOR2_GLR_H */
