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
 * Non-Terminal Symbols
 * ================================================== */

nomen enumeratio {
    ARBOR2_NT_DISIUNCTIO,           /* logical OR expressions (||) - lowest precedence */
    ARBOR2_NT_CONIUNCTIO,           /* logical AND expressions (&&) */
    ARBOR2_NT_AEQUALITAS,           /* equality expressions (==, !=) */
    ARBOR2_NT_COMPARATIO,           /* relational expressions (<, >, <=, >=) */
    ARBOR2_NT_TRANSLATIO,           /* shift expressions (<<, >>) */
    ARBOR2_NT_EXPRESSIO,
    ARBOR2_NT_TERMINUS,             /* term in expr grammar */
    ARBOR2_NT_FACTOR,               /* factor in expr grammar */
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
    ARBOR2_NT_ENUMERATOR            /* single enumerator */
} Arbor2NonTerminalis;

/* ==================================================
 * AST Node Types (minimal for expressions)
 * ================================================== */

nomen enumeratio {
    ARBOR2_NODUS_IDENTIFICATOR,
    ARBOR2_NODUS_INTEGER,
    ARBOR2_NODUS_BINARIUM,          /* Binary operation */
    ARBOR2_NODUS_UNARIUM,           /* Unary operation */
    ARBOR2_NODUS_CONVERSIO,         /* Cast expression */
    ARBOR2_NODUS_SIZEOF,
    ARBOR2_NODUS_DECLARATIO,        /* Declaration: type *name */
    ARBOR2_NODUS_DECLARATOR,        /* Declarator: *name or name */
    ARBOR2_NODUS_DECLARATOR_FUNCTI, /* Function declarator: name() or name(void) */
    ARBOR2_NODUS_PARAMETER_DECL,    /* Parameter declaration: type name */
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
    ARBOR2_NODUS_AMBIGUUS,          /* Ambiguous node */
    ARBOR2_NODUS_ERROR
} Arbor2NodusGenus;

/* ==================================================
 * AST Node
 * ================================================== */

nomen structura Arbor2Nodus Arbor2Nodus;

structura Arbor2Nodus {
    Arbor2NodusGenus            genus;
    Arbor2Token*                lexema;         /* Associated token */

    unio {
        /* IDENTIFICATOR, INTEGER */
        structura {
            chorda              valor;
        } folium;

        /* BINARIUM */
        structura {
            Arbor2Nodus*        sinister;
            Arbor2LexemaGenus   operator;
            Arbor2Nodus*        dexter;
        } binarium;

        /* UNARIUM */
        structura {
            Arbor2LexemaGenus   operator;
            Arbor2Nodus*        operandum;
        } unarium;

        /* CONVERSIO (cast) */
        structura {
            Arbor2Nodus*        typus;
            Arbor2Nodus*        expressio;
        } conversio;

        /* SIZEOF */
        structura {
            b32                 est_typus;      /* sizeof(type) vs sizeof expr */
            Arbor2Nodus*        operandum;
        } sizeof_expr;

        /* DECLARATIO */
        structura {
            Arbor2Nodus*        specifier;      /* Type specifier (identifier) */
            Arbor2Nodus*        declarator;     /* The declarator (*name or name) */
            b32                 est_typedef;    /* VERUM if this is a typedef declaration */
        } declaratio;

        /* DECLARATOR */
        structura {
            s32                 num_stellae;    /* Number of * pointers */
            chorda              titulus;        /* Variable name (empty for anonymous bit fields) */
            Arbor2Nodus*        latitudo_biti;  /* Bit field width expr, NIHIL if not bit field */
            Xar*                dimensiones;    /* Array dimensions (Xar of Arbor2Nodus*), NIHIL if not array */
        } declarator;

        /* DECLARATOR_FUNCTI (function declarator) */
        structura {
            Arbor2Nodus*        declarator_interior;  /* Inner declarator (name + pointers) */
            Xar*                parametri;            /* NULL for (), params for D2 */
            b32                 habet_void;           /* true if explicitly (void) */
            s32                 num_stellae;          /* Pointer depth (for * fn()) */
        } declarator_functi;

        /* PARAMETER_DECL (parameter declaration) */
        structura {
            Arbor2Nodus*        type_specifier;       /* Type (identifier node) */
            Arbor2Nodus*        declarator;           /* Name + pointers */
        } parameter_decl;

        /* DEFINITIO_FUNCTI (function definition) */
        structura {
            Arbor2Nodus*        specifier;            /* Return type (identifier node) */
            Arbor2Nodus*        declarator;           /* Function declarator (DECLARATOR_FUNCTI) */
            Arbor2Nodus*        corpus;               /* Function body (CORPUS node) */
        } definitio_functi;

        /* STRUCT_SPECIFIER (also used for union) */
        structura {
            Arbor2Nodus*        tag;                  /* Tag name (IDENTIFICATOR) or NIHIL for anonymous */
            Xar*                membra;               /* List of member declarations (DECLARATIO nodes) */
            b32                 est_unio;             /* VERUM for union, FALSUM for struct */
        } struct_specifier;

        /* ENUM_SPECIFIER */
        structura {
            Arbor2Nodus*        tag;                  /* Tag name (IDENTIFICATOR) or NIHIL for anonymous */
            Xar*                enumeratores;         /* List of ENUMERATOR nodes */
        } enum_specifier;

        /* ENUMERATOR */
        structura {
            chorda              titulus;              /* Enumerator name */
            Arbor2Nodus*        valor;                /* Optional value expr, NIHIL if not specified */
        } enumerator;

        /* SENTENTIA (expression statement) */
        structura {
            Arbor2Nodus*        expressio;      /* The expression (NULL for empty stmt) */
        } sententia;

        /* CORPUS (compound statement) */
        structura {
            Xar*                sententiae;     /* Xar of Arbor2Nodus* - list of statements */
        } corpus;

        /* SI (if statement) */
        structura {
            Arbor2Nodus*        conditio;       /* condition expression */
            Arbor2Nodus*        consequens;     /* then-branch */
            Arbor2Nodus*        alternans;      /* else-branch (NULL if no else) */
        } conditionale;

        /* DUM/FAC (while/do-while statement) */
        structura {
            Arbor2Nodus*        conditio;       /* loop condition */
            Arbor2Nodus*        corpus;         /* loop body */
        } iteratio;

        /* PER (for statement) */
        structura {
            Arbor2Nodus*        initium;        /* init expression (NULL if omitted) */
            Arbor2Nodus*        conditio;       /* condition (NULL if omitted) */
            Arbor2Nodus*        incrementum;    /* increment (NULL if omitted) */
            Arbor2Nodus*        corpus;         /* loop body */
        } circuitus;

        /* REDDE (return statement) */
        structura {
            Arbor2Nodus*        valor;          /* return value (NULL if omitted) */
        } reditio;

        /* SALTA (goto statement) */
        structura {
            chorda              destinatio;     /* label name */
        } saltus;

        /* TITULATUM (labeled statement) */
        structura {
            chorda              titulus;        /* label name */
            Arbor2Nodus*        sententia;      /* the labeled statement */
        } titulatum;

        /* COMMUTATIO (switch statement) */
        structura {
            Arbor2Nodus*        expressio;      /* switch expression */
            Arbor2Nodus*        corpus;         /* body (usually compound) */
        } selectivum;  /* 'commutatio' expands to 'switch' keyword */

        /* CASUS (case label) */
        structura {
            Arbor2Nodus*        valor;          /* constant expression */
            Arbor2Nodus*        sententia;      /* following statement */
        } electio;  /* 'casus' expands to 'case' keyword */

        /* ORDINARIUS (default label) */
        structura {
            Arbor2Nodus*        sententia;      /* following statement */
        } defectus;  /* 'ordinarius' expands to 'default' keyword */

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
} Arbor2TabulaActio;

nomen structura {
    s32                     status;         /* Current state */
    s32                     non_terminalis; /* Non-terminal symbol (internal index) */
    s32                     status_novus;   /* Next state */
} Arbor2TabulaGoto;

/* ==================================================
 * Grammar Rule
 * ================================================== */

nomen structura {
    Arbor2NonTerminalis     sinister;       /* Left-hand side */
    i32                     longitudo;      /* Number of symbols on right */
    Arbor2NodusGenus        nodus_genus;    /* AST node type to create */
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
} Arbor2GLR;

/* ==================================================
 * Parse Result
 * ================================================== */

nomen structura {
    b32                     successus;
    Arbor2Nodus*            radix;          /* Root AST node */
    Xar*                    ambigui;        /* Unresolved ambiguities */
    Xar*                    errores;        /* Parse errors */
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
i32 arbor2_tabula_numerus_actionum(vacuum);
i32 arbor2_tabula_numerus_goto(vacuum);

/* Iterate actions for a specific state */
vacuum arbor2_tabula_iterare_actiones_status(
    i32 status,
    vacuum (*callback)(constans Arbor2TabulaActio* actio, vacuum* ctx),
    vacuum* ctx);

/* Iterate all states for a specific token */
vacuum arbor2_tabula_iterare_actiones_lexema(
    Arbor2LexemaGenus lexema,
    vacuum (*callback)(i32 status, constans Arbor2TabulaActio* actio, vacuum* ctx),
    vacuum* ctx);

/* Iterate goto entries for a specific non-terminal */
vacuum arbor2_tabula_iterare_goto_nt(
    s32 nt,
    vacuum (*callback)(constans Arbor2TabulaGoto* entry, vacuum* ctx),
    vacuum* ctx);

/* Get rule by index (no GLR instance needed) */
Arbor2Regula* arbor2_tabula_obtinere_regula(i32 index);

#endif /* ARBOR2_GLR_H */
