/* lapifex_c89_grammatica.c - Grammatica Expressionum C89 pro Lapifex GLR
 *
 * Continet:
 *   - Grammatica STML expressionum C89 (~94 productiones)
 *   - Functiones auxiliares pro creatione AST nodorum
 *   - Callback reductionis (commutatio magna)
 *   - Pipeline parsationis expressionum
 */

#include "lapifex_c89.h"
#include "xar.h"
#include "tabula_dispersa.h"
#include <stdio.h>
#include <string.h>

/* ================================================
 * Macros pro conversione valorum
 *
 * In lapifex GLR, valori sunt s64. Nos codificamus
 * pointers ad Arbor2Nodus* et Arbor2Lexema* in s64.
 * ================================================ */

#define NODUS_EX(v)  ((Arbor2Nodus*)(vacuum*)(longus)(v))
#define LEXEMA_EX(v) ((Arbor2Lexema*)(vacuum*)(longus)(v))
#define VALOR_EX(p)  ((s64)(longus)(vacuum*)(p))

/* ================================================
 * Grammatica STML Expressionum C89
 *
 * Terminalia correspondent ad NOMINA_GENERUM[] in arbor2_lexema.c.
 * Non-terminalia sequuntur hierarchiam praecedentiae C89.
 *
 * Praecedentia (ab infima ad summam):
 *   virga -> assignatio -> ternarius -> disiunctio ->
 *   coniunctio -> pipa_bw -> caret_bw -> amp_bw ->
 *   aequalitas -> comparatio -> translatio ->
 *   expressio -> terminus -> factor -> postfixum -> primarium
 * ================================================ */

hic_manens constans character* GRAMMATICA_EXPRESSIO =
    "<grammatica>"

    /* ---- Terminalia ---- */
    "  <terminalia>"

    /* Verba Clavium */
    "    <terminalis titulus=\"AUTO\"/>"
    "    <terminalis titulus=\"CHAR\"/>"
    "    <terminalis titulus=\"CONST\"/>"
    "    <terminalis titulus=\"DOUBLE\"/>"
    "    <terminalis titulus=\"ENUM\"/>"
    "    <terminalis titulus=\"EXTERN\"/>"
    "    <terminalis titulus=\"FLOAT\"/>"
    "    <terminalis titulus=\"INT\"/>"
    "    <terminalis titulus=\"LONG\"/>"
    "    <terminalis titulus=\"REGISTER\"/>"
    "    <terminalis titulus=\"SHORT\"/>"
    "    <terminalis titulus=\"SIGNED\"/>"
    "    <terminalis titulus=\"SIZEOF\"/>"
    "    <terminalis titulus=\"STATIC\"/>"
    "    <terminalis titulus=\"STRUCT\"/>"
    "    <terminalis titulus=\"TYPEDEF\"/>"
    "    <terminalis titulus=\"UNION\"/>"
    "    <terminalis titulus=\"UNSIGNED\"/>"
    "    <terminalis titulus=\"VOID\"/>"
    "    <terminalis titulus=\"VOLATILE\"/>"

    /* Identificatores et Literalia */
    "    <terminalis titulus=\"IDENTIFICATOR\"/>"
    "    <terminalis titulus=\"INTEGER\"/>"
    "    <terminalis titulus=\"FLOAT_LIT\"/>"
    "    <terminalis titulus=\"CHAR_LIT\"/>"
    "    <terminalis titulus=\"STRING_LIT\"/>"

    /* Operatores Arithmetici */
    "    <terminalis titulus=\"PLUS\"/>"
    "    <terminalis titulus=\"MINUS\"/>"
    "    <terminalis titulus=\"ASTERISCUS\"/>"
    "    <terminalis titulus=\"SOLIDUS\"/>"
    "    <terminalis titulus=\"PERCENTUM\"/>"

    /* Operatores Bitwise */
    "    <terminalis titulus=\"AMPERSAND\"/>"
    "    <terminalis titulus=\"PIPA\"/>"
    "    <terminalis titulus=\"CARET\"/>"
    "    <terminalis titulus=\"TILDE\"/>"

    /* Operatores Logici */
    "    <terminalis titulus=\"EXCLAMATIO\"/>"
    "    <terminalis titulus=\"DUAMPERSAND\"/>"
    "    <terminalis titulus=\"DUPIPA\"/>"

    /* Operatores Comparationis */
    "    <terminalis titulus=\"AEQUALIS\"/>"
    "    <terminalis titulus=\"NON_AEQUALIS\"/>"
    "    <terminalis titulus=\"MINOR\"/>"
    "    <terminalis titulus=\"MAIOR\"/>"
    "    <terminalis titulus=\"MINOR_AEQ\"/>"
    "    <terminalis titulus=\"MAIOR_AEQ\"/>"

    /* Operatores Translationis */
    "    <terminalis titulus=\"SINISTRUM\"/>"
    "    <terminalis titulus=\"DEXTRUM\"/>"

    /* Operatores Assignationis */
    "    <terminalis titulus=\"ASSIGNATIO\"/>"
    "    <terminalis titulus=\"PLUS_ASSIGN\"/>"
    "    <terminalis titulus=\"MINUS_ASSIGN\"/>"
    "    <terminalis titulus=\"MULT_ASSIGN\"/>"
    "    <terminalis titulus=\"DIV_ASSIGN\"/>"
    "    <terminalis titulus=\"MOD_ASSIGN\"/>"
    "    <terminalis titulus=\"AND_ASSIGN\"/>"
    "    <terminalis titulus=\"OR_ASSIGN\"/>"
    "    <terminalis titulus=\"XOR_ASSIGN\"/>"
    "    <terminalis titulus=\"SHL_ASSIGN\"/>"
    "    <terminalis titulus=\"SHR_ASSIGN\"/>"

    /* Incrementum/Decrementum */
    "    <terminalis titulus=\"DUPLUS\"/>"
    "    <terminalis titulus=\"DUMINUS\"/>"

    /* Accessus Membrorum */
    "    <terminalis titulus=\"PUNCTUM\"/>"
    "    <terminalis titulus=\"SAGITTA\"/>"

    /* Punctuatio */
    "    <terminalis titulus=\"COMMA\"/>"
    "    <terminalis titulus=\"COLON\"/>"
    "    <terminalis titulus=\"SEMICOLON\"/>"
    "    <terminalis titulus=\"QUAESTIO\"/>"

    /* Delimitatores */
    "    <terminalis titulus=\"PAREN_APERTA\"/>"
    "    <terminalis titulus=\"PAREN_CLAUSA\"/>"
    "    <terminalis titulus=\"BRACKET_APERTA\"/>"
    "    <terminalis titulus=\"BRACKET_CLAUSA\"/>"
    "    <terminalis titulus=\"BRACE_APERTA\"/>"
    "    <terminalis titulus=\"BRACE_CLAUSA\"/>"

    /* Verba Clavium Sententiarum */
    "    <terminalis titulus=\"IF\"/>"
    "    <terminalis titulus=\"ELSE\"/>"
    "    <terminalis titulus=\"WHILE\"/>"
    "    <terminalis titulus=\"DO\"/>"
    "    <terminalis titulus=\"FOR\"/>"
    "    <terminalis titulus=\"SWITCH\"/>"
    "    <terminalis titulus=\"CASE\"/>"
    "    <terminalis titulus=\"DEFAULT\"/>"
    "    <terminalis titulus=\"BREAK\"/>"
    "    <terminalis titulus=\"CONTINUE\"/>"
    "    <terminalis titulus=\"RETURN\"/>"
    "    <terminalis titulus=\"GOTO\"/>"

    /* Ellipsis */
    "    <terminalis titulus=\"ELLIPSIS\"/>"

    /* Nomen Typus (typedef name) */
    "    <terminalis titulus=\"NOMEN_TYPUS\"/>"

    "    <terminalis titulus=\"EOF\"/>"
    "  </terminalia>"

    /* ---- Regulae ---- */
    "  <regulae>"

    /* P0-P1: virga (comma expression, infima praecedentia) */
    "    <regula titulus=\"virga\">"
    "      <productio>assignatio</productio>"                           /* P0 */
    "      <productio>virga COMMA assignatio</productio>"               /* P1 */
    "    </regula>"

    /* P2-P13: assignatio (dextro-associativa) */
    "    <regula titulus=\"assignatio\">"
    "      <productio>ternarius</productio>"                            /* P2 */
    "      <productio>ternarius ASSIGNATIO assignatio</productio>"      /* P3 */
    "      <productio>ternarius PLUS_ASSIGN assignatio</productio>"     /* P4 */
    "      <productio>ternarius MINUS_ASSIGN assignatio</productio>"    /* P5 */
    "      <productio>ternarius MULT_ASSIGN assignatio</productio>"     /* P6 */
    "      <productio>ternarius DIV_ASSIGN assignatio</productio>"      /* P7 */
    "      <productio>ternarius MOD_ASSIGN assignatio</productio>"      /* P8 */
    "      <productio>ternarius AND_ASSIGN assignatio</productio>"      /* P9 */
    "      <productio>ternarius OR_ASSIGN assignatio</productio>"       /* P10 */
    "      <productio>ternarius XOR_ASSIGN assignatio</productio>"      /* P11 */
    "      <productio>ternarius SHL_ASSIGN assignatio</productio>"      /* P12 */
    "      <productio>ternarius SHR_ASSIGN assignatio</productio>"      /* P13 */
    "    </regula>"

    /* P14-P15: ternarius */
    "    <regula titulus=\"ternarius\">"
    "      <productio>disiunctio</productio>"                           /* P14 */
    "      <productio>disiunctio QUAESTIO virga COLON ternarius</productio>" /* P15 */
    "    </regula>"

    /* P16-P17: disiunctio (logical OR) */
    "    <regula titulus=\"disiunctio\">"
    "      <productio>coniunctio</productio>"                           /* P16 */
    "      <productio>disiunctio DUPIPA coniunctio</productio>"         /* P17 */
    "    </regula>"

    /* P18-P19: coniunctio (logical AND) */
    "    <regula titulus=\"coniunctio\">"
    "      <productio>pipa_bw</productio>"                              /* P18 */
    "      <productio>coniunctio DUAMPERSAND pipa_bw</productio>"       /* P19 */
    "    </regula>"

    /* P20-P21: pipa_bw (bitwise OR) */
    "    <regula titulus=\"pipa_bw\">"
    "      <productio>caret_bw</productio>"                             /* P20 */
    "      <productio>pipa_bw PIPA caret_bw</productio>"                /* P21 */
    "    </regula>"

    /* P22-P23: caret_bw (bitwise XOR) */
    "    <regula titulus=\"caret_bw\">"
    "      <productio>amp_bw</productio>"                               /* P22 */
    "      <productio>caret_bw CARET amp_bw</productio>"                /* P23 */
    "    </regula>"

    /* P24-P25: amp_bw (bitwise AND) */
    "    <regula titulus=\"amp_bw\">"
    "      <productio>aequalitas</productio>"                           /* P24 */
    "      <productio>amp_bw AMPERSAND aequalitas</productio>"          /* P25 */
    "    </regula>"

    /* P26-P28: aequalitas (== !=) */
    "    <regula titulus=\"aequalitas\">"
    "      <productio>comparatio</productio>"                           /* P26 */
    "      <productio>aequalitas AEQUALIS comparatio</productio>"       /* P27 */
    "      <productio>aequalitas NON_AEQUALIS comparatio</productio>"   /* P28 */
    "    </regula>"

    /* P29-P33: comparatio (< > <= >=) */
    "    <regula titulus=\"comparatio\">"
    "      <productio>translatio</productio>"                           /* P29 */
    "      <productio>comparatio MINOR translatio</productio>"          /* P30 */
    "      <productio>comparatio MAIOR translatio</productio>"          /* P31 */
    "      <productio>comparatio MINOR_AEQ translatio</productio>"      /* P32 */
    "      <productio>comparatio MAIOR_AEQ translatio</productio>"      /* P33 */
    "    </regula>"

    /* P34-P36: translatio (<< >>) */
    "    <regula titulus=\"translatio\">"
    "      <productio>expressio</productio>"                            /* P34 */
    "      <productio>translatio SINISTRUM expressio</productio>"       /* P35 */
    "      <productio>translatio DEXTRUM expressio</productio>"         /* P36 */
    "    </regula>"

    /* P37-P39: expressio (+ -) */
    "    <regula titulus=\"expressio\">"
    "      <productio>terminus</productio>"                             /* P37 */
    "      <productio>expressio PLUS terminus</productio>"              /* P38 */
    "      <productio>expressio MINUS terminus</productio>"             /* P39 */
    "    </regula>"

    /* P40-P43: terminus (* / %) */
    "    <regula titulus=\"terminus\">"
    "      <productio>factor</productio>"                               /* P40 */
    "      <productio>terminus ASTERISCUS factor</productio>"           /* P41 */
    "      <productio>terminus SOLIDUS factor</productio>"              /* P42 */
    "      <productio>terminus PERCENTUM factor</productio>"            /* P43 */
    "    </regula>"

    /* P44-P55: factor (unary, cast, sizeof) */
    "    <regula titulus=\"factor\">"
    "      <productio>postfixum</productio>"                            /* P44 */
    "      <productio>MINUS factor</productio>"                         /* P45 */
    "      <productio>PLUS factor</productio>"                          /* P46 */
    "      <productio>TILDE factor</productio>"                         /* P47 */
    "      <productio>EXCLAMATIO factor</productio>"                    /* P48 */
    "      <productio>AMPERSAND factor</productio>"                     /* P49 */
    "      <productio>ASTERISCUS factor</productio>"                    /* P50 */
    "      <productio>DUPLUS factor</productio>"                        /* P51 */
    "      <productio>DUMINUS factor</productio>"                       /* P52 */
    "      <productio>SIZEOF factor</productio>"                        /* P53 */
    "      <productio>SIZEOF PAREN_APERTA nomen_typi PAREN_CLAUSA</productio>"  /* P54 */
    "      <productio>PAREN_APERTA nomen_typi PAREN_CLAUSA factor</productio>"  /* P55 */
    "    </regula>"

    /* P56-P63: postfixum */
    "    <regula titulus=\"postfixum\">"
    "      <productio>primarium</productio>"                            /* P56 */
    "      <productio>postfixum PAREN_APERTA PAREN_CLAUSA</productio>"                     /* P57 */
    "      <productio>postfixum PAREN_APERTA argumenta PAREN_CLAUSA</productio>"            /* P58 */
    "      <productio>postfixum BRACKET_APERTA virga BRACKET_CLAUSA</productio>"            /* P59 */
    "      <productio>postfixum PUNCTUM IDENTIFICATOR</productio>"      /* P60 */
    "      <productio>postfixum SAGITTA IDENTIFICATOR</productio>"      /* P61 */
    "      <productio>postfixum DUPLUS</productio>"                     /* P62 */
    "      <productio>postfixum DUMINUS</productio>"                    /* P63 */
    "    </regula>"

    /* P64-P69: primarium */
    "    <regula titulus=\"primarium\">"
    "      <productio>IDENTIFICATOR</productio>"                        /* P64 */
    "      <productio>INTEGER</productio>"                              /* P65 */
    "      <productio>FLOAT_LIT</productio>"                            /* P66 */
    "      <productio>CHAR_LIT</productio>"                             /* P67 */
    "      <productio>STRING_LIT</productio>"                           /* P68 */
    "      <productio>PAREN_APERTA virga PAREN_CLAUSA</productio>"      /* P69 */
    "    </regula>"

    /* P70-P71: argumenta (comma-separated assignment expressions) */
    "    <regula titulus=\"argumenta\">"
    "      <productio>assignatio</productio>"                           /* P70 */
    "      <productio>argumenta COMMA assignatio</productio>"           /* P71 */
    "    </regula>"

    /* P72-P73: nomen_typi (minimal, keyword-only) */
    "    <regula titulus=\"nomen_typi\">"
    "      <productio>specifier_lista</productio>"                      /* P72 */
    "      <productio>specifier_lista pointer</productio>"              /* P73 */
    "    </regula>"

    /* P74-P75: specifier_lista */
    "    <regula titulus=\"specifier_lista\">"
    "      <productio>specifier_singulum</productio>"                   /* P74 */
    "      <productio>specifier_singulum specifier_lista</productio>"   /* P75 */
    "    </regula>"

    /* P76-P89: specifier_singulum */
    "    <regula titulus=\"specifier_singulum\">"
    "      <productio>VOID</productio>"                                 /* P76 */
    "      <productio>CHAR</productio>"                                 /* P77 */
    "      <productio>SHORT</productio>"                                /* P78 */
    "      <productio>INT</productio>"                                  /* P79 */
    "      <productio>LONG</productio>"                                 /* P80 */
    "      <productio>FLOAT</productio>"                                /* P81 */
    "      <productio>DOUBLE</productio>"                               /* P82 */
    "      <productio>SIGNED</productio>"                               /* P83 */
    "      <productio>UNSIGNED</productio>"                             /* P84 */
    "      <productio>CONST</productio>"                                /* P85 */
    "      <productio>VOLATILE</productio>"                             /* P86 */
    "      <productio>STRUCT IDENTIFICATOR</productio>"                 /* P87 */
    "      <productio>UNION IDENTIFICATOR</productio>"                  /* P88 */
    "      <productio>ENUM IDENTIFICATOR</productio>"                   /* P89 */
    "    </regula>"

    /* P90-P93: pointer */
    "    <regula titulus=\"pointer\">"
    "      <productio>ASTERISCUS</productio>"                           /* P90 */
    "      <productio>ASTERISCUS pointer</productio>"                   /* P91 */
    "      <productio>ASTERISCUS CONST</productio>"                     /* P92 */
    "      <productio>ASTERISCUS CONST pointer</productio>"             /* P93 */
    "    </regula>"

    /* ---- Declarationes (P94+) ---- */

    /* P94-P96: summum (novum initium) */
    "    <regula titulus=\"summum\">"
    "      <productio>declaratio</productio>"                           /* P94 */
    "      <productio>virga</productio>"                                /* P95 */
    "      <productio>sententia</productio>"                            /* P96 */
    "    </regula>"

    /* P97-P98: declaratio */
    "    <regula titulus=\"declaratio\">"
    "      <productio>decl_specifiers init_declarator_lista SEMICOLON</productio>" /* P97 */
    "      <productio>decl_specifiers SEMICOLON</productio>"            /* P98 */
    "    </regula>"

    /* P99-P118: decl_specifiers */
    "    <regula titulus=\"decl_specifiers\">"
    "      <productio>decl_specifier</productio>"                       /* P99 */
    "      <productio>decl_specifiers decl_specifier</productio>"       /* P100 */
    "    </regula>"

    "    <regula titulus=\"decl_specifier\">"
    "      <productio>AUTO</productio>"                                 /* P101 */
    "      <productio>REGISTER</productio>"                             /* P102 */
    "      <productio>STATIC</productio>"                               /* P103 */
    "      <productio>EXTERN</productio>"                               /* P104 */
    "      <productio>TYPEDEF</productio>"                              /* P105 */
    "      <productio>CONST</productio>"                                /* P106 */
    "      <productio>VOLATILE</productio>"                             /* P107 */
    "      <productio>VOID</productio>"                                 /* P108 */
    "      <productio>CHAR</productio>"                                 /* P109 */
    "      <productio>SHORT</productio>"                                /* P110 */
    "      <productio>INT</productio>"                                  /* P111 */
    "      <productio>LONG</productio>"                                 /* P112 */
    "      <productio>FLOAT</productio>"                                /* P113 */
    "      <productio>DOUBLE</productio>"                               /* P114 */
    "      <productio>SIGNED</productio>"                               /* P115 */
    "      <productio>UNSIGNED</productio>"                             /* P116 */
    "      <productio>struct_specifier</productio>"                     /* P117 */
    "      <productio>enum_specifier</productio>"                       /* P118 */
    "    </regula>"

    /* P119-P122: init_declarator_lista et init_declarator */
    "    <regula titulus=\"init_declarator_lista\">"
    "      <productio>init_declarator</productio>"                      /* P119 */
    "      <productio>init_declarator_lista COMMA init_declarator</productio>" /* P120 */
    "    </regula>"

    "    <regula titulus=\"init_declarator\">"
    "      <productio>declarator</productio>"                           /* P121 */
    "      <productio>declarator ASSIGNATIO initializer</productio>"    /* P122 */
    "    </regula>"

    /* P123-P131: declarator et directus_declarator */
    "    <regula titulus=\"declarator\">"
    "      <productio>directus_declarator</productio>"                  /* P123 */
    "      <productio>pointer directus_declarator</productio>"          /* P124 */
    "    </regula>"

    "    <regula titulus=\"directus_declarator\">"
    "      <productio>IDENTIFICATOR</productio>"                        /* P125 */
    "      <productio>PAREN_APERTA declarator PAREN_CLAUSA</productio>" /* P126 */
    "      <productio>directus_declarator BRACKET_APERTA BRACKET_CLAUSA</productio>" /* P127 */
    "      <productio>directus_declarator BRACKET_APERTA virga BRACKET_CLAUSA</productio>" /* P128 */
    "      <productio>directus_declarator PAREN_APERTA PAREN_CLAUSA</productio>" /* P129 */
    "      <productio>directus_declarator PAREN_APERTA parameter_lista PAREN_CLAUSA</productio>" /* P130 */
    "      <productio>directus_declarator PAREN_APERTA VOID PAREN_CLAUSA</productio>" /* P131 */
    "    </regula>"

    /* P132-P137: struct_specifier (struct et union) */
    "    <regula titulus=\"struct_specifier\">"
    "      <productio>STRUCT IDENTIFICATOR BRACE_APERTA struct_member_lista BRACE_CLAUSA</productio>" /* P132 */
    "      <productio>STRUCT BRACE_APERTA struct_member_lista BRACE_CLAUSA</productio>" /* P133 */
    "      <productio>STRUCT IDENTIFICATOR</productio>"                 /* P134 */
    "      <productio>UNION IDENTIFICATOR BRACE_APERTA struct_member_lista BRACE_CLAUSA</productio>" /* P135 */
    "      <productio>UNION BRACE_APERTA struct_member_lista BRACE_CLAUSA</productio>" /* P136 */
    "      <productio>UNION IDENTIFICATOR</productio>"                  /* P137 */
    "    </regula>"

    /* P138-P145: struct_member_lista, struct_member, struct_declarator */
    "    <regula titulus=\"struct_member_lista\">"
    "      <productio>struct_member</productio>"                        /* P138 */
    "      <productio>struct_member_lista struct_member</productio>"    /* P139 */
    "    </regula>"

    "    <regula titulus=\"struct_member\">"
    "      <productio>decl_specifiers struct_declarator_lista SEMICOLON</productio>" /* P140 */
    "    </regula>"

    "    <regula titulus=\"struct_declarator_lista\">"
    "      <productio>struct_declarator</productio>"                    /* P141 */
    "      <productio>struct_declarator_lista COMMA struct_declarator</productio>" /* P142 */
    "    </regula>"

    "    <regula titulus=\"struct_declarator\">"
    "      <productio>declarator</productio>"                           /* P143 */
    "      <productio>declarator COLON virga</productio>"               /* P144 */
    "      <productio>COLON virga</productio>"                          /* P145 */
    "    </regula>"

    /* P146-P152: enum_specifier, enumerator_lista, enumerator */
    "    <regula titulus=\"enum_specifier\">"
    "      <productio>ENUM IDENTIFICATOR BRACE_APERTA enumerator_lista BRACE_CLAUSA</productio>" /* P146 */
    "      <productio>ENUM BRACE_APERTA enumerator_lista BRACE_CLAUSA</productio>" /* P147 */
    "      <productio>ENUM IDENTIFICATOR</productio>"                   /* P148 */
    "    </regula>"

    "    <regula titulus=\"enumerator_lista\">"
    "      <productio>enumerator</productio>"                           /* P149 */
    "      <productio>enumerator_lista COMMA enumerator</productio>"    /* P150 */
    "    </regula>"

    "    <regula titulus=\"enumerator\">"
    "      <productio>IDENTIFICATOR</productio>"                        /* P151 */
    "      <productio>IDENTIFICATOR ASSIGNATIO virga</productio>"       /* P152 */
    "    </regula>"

    /* P153-P157: parameter_lista, parameter_decl */
    "    <regula titulus=\"parameter_lista\">"
    "      <productio>parameter_decl</productio>"                       /* P153 */
    "      <productio>parameter_lista COMMA parameter_decl</productio>" /* P154 */
    "      <productio>parameter_lista COMMA ELLIPSIS</productio>"       /* P155 */
    "    </regula>"

    "    <regula titulus=\"parameter_decl\">"
    "      <productio>decl_specifiers declarator</productio>"           /* P156 */
    "      <productio>decl_specifiers</productio>"                      /* P157 */
    "    </regula>"

    /* P158-P162: initializer, init_items */
    "    <regula titulus=\"initializer\">"
    "      <productio>assignatio</productio>"                           /* P158 */
    "      <productio>BRACE_APERTA init_items BRACE_CLAUSA</productio>" /* P159 */
    "      <productio>BRACE_APERTA init_items COMMA BRACE_CLAUSA</productio>" /* P160 */
    "    </regula>"

    "    <regula titulus=\"init_items\">"
    "      <productio>initializer</productio>"                          /* P161 */
    "      <productio>init_items COMMA initializer</productio>"         /* P162 */
    "    </regula>"

    /* ---- Sententiae (P163+) ---- */

    /* P163-P164: sententia -> compar | incompar */
    "    <regula titulus=\"sententia\">"
    "      <productio>sententia_compar</productio>"                     /* P163 */
    "      <productio>sententia_incompar</productio>"                   /* P164 */
    "    </regula>"

    /* P165-P171: sententia_compar (matched statements) */
    "    <regula titulus=\"sententia_compar\">"
    "      <productio>non_si_sententia</productio>"                     /* P165 */
    "      <productio>IF PAREN_APERTA virga PAREN_CLAUSA sententia_compar ELSE sententia_compar</productio>" /* P166 */
    "      <productio>WHILE PAREN_APERTA virga PAREN_CLAUSA sententia_compar</productio>" /* P167 */
    "      <productio>FOR PAREN_APERTA per_clausula PAREN_CLAUSA sententia_compar</productio>" /* P168 */
    "      <productio>IDENTIFICATOR COLON sententia_compar</productio>" /* P169 */
    "      <productio>CASE virga COLON sententia_compar</productio>"    /* P170 */
    "      <productio>DEFAULT COLON sententia_compar</productio>"       /* P171 */
    "    </regula>"

    /* P172-P178: sententia_incompar (unmatched statements) */
    "    <regula titulus=\"sententia_incompar\">"
    "      <productio>IF PAREN_APERTA virga PAREN_CLAUSA sententia</productio>" /* P172 */
    "      <productio>IF PAREN_APERTA virga PAREN_CLAUSA sententia_compar ELSE sententia_incompar</productio>" /* P173 */
    "      <productio>WHILE PAREN_APERTA virga PAREN_CLAUSA sententia_incompar</productio>" /* P174 */
    "      <productio>FOR PAREN_APERTA per_clausula PAREN_CLAUSA sententia_incompar</productio>" /* P175 */
    "      <productio>IDENTIFICATOR COLON sententia_incompar</productio>" /* P176 */
    "      <productio>CASE virga COLON sententia_incompar</productio>"  /* P177 */
    "      <productio>DEFAULT COLON sententia_incompar</productio>"     /* P178 */
    "    </regula>"

    /* P179-P188: non_si_sententia (non-if matched statements) */
    "    <regula titulus=\"non_si_sententia\">"
    "      <productio>corpus</productio>"                               /* P179 */
    "      <productio>virga SEMICOLON</productio>"                      /* P180 */
    "      <productio>SEMICOLON</productio>"                            /* P181 */
    "      <productio>DO sententia WHILE PAREN_APERTA virga PAREN_CLAUSA SEMICOLON</productio>" /* P182 */
    "      <productio>SWITCH PAREN_APERTA virga PAREN_CLAUSA sententia</productio>" /* P183 */
    "      <productio>GOTO IDENTIFICATOR SEMICOLON</productio>"         /* P184 */
    "      <productio>CONTINUE SEMICOLON</productio>"                   /* P185 */
    "      <productio>BREAK SEMICOLON</productio>"                      /* P186 */
    "      <productio>RETURN SEMICOLON</productio>"                     /* P187 */
    "      <productio>RETURN virga SEMICOLON</productio>"               /* P188 */
    "    </regula>"

    /* P189-P190: corpus (compound statement) */
    "    <regula titulus=\"corpus\">"
    "      <productio>BRACE_APERTA BRACE_CLAUSA</productio>"           /* P189 */
    "      <productio>BRACE_APERTA elenchus_corporis BRACE_CLAUSA</productio>" /* P190 */
    "    </regula>"

    /* P191-P192: elenchus_corporis */
    "    <regula titulus=\"elenchus_corporis\">"
    "      <productio>elementum_corporis</productio>"                   /* P191 */
    "      <productio>elenchus_corporis elementum_corporis</productio>" /* P192 */
    "    </regula>"

    /* P193-P194: elementum_corporis */
    "    <regula titulus=\"elementum_corporis\">"
    "      <productio>declaratio</productio>"                           /* P193 */
    "      <productio>sententia</productio>"                            /* P194 */
    "    </regula>"

    /* P195-P202: per_clausula (for-loop clauses) */
    "    <regula titulus=\"per_clausula\">"
    "      <productio>virga SEMICOLON virga SEMICOLON virga</productio>" /* P195 */
    "      <productio>virga SEMICOLON virga SEMICOLON</productio>"      /* P196 */
    "      <productio>virga SEMICOLON SEMICOLON virga</productio>"      /* P197 */
    "      <productio>SEMICOLON virga SEMICOLON virga</productio>"      /* P198 */
    "      <productio>virga SEMICOLON SEMICOLON</productio>"            /* P199 */
    "      <productio>SEMICOLON virga SEMICOLON</productio>"            /* P200 */
    "      <productio>SEMICOLON SEMICOLON virga</productio>"            /* P201 */
    "      <productio>SEMICOLON SEMICOLON</productio>"                  /* P202 */
    "    </regula>"

    /* ---- Definitio Functi (P203) ---- */

    /* P203: declaratio -> decl_specifiers declarator corpus (definitio functi) */
    "    <regula titulus=\"declaratio\">"
    "      <productio>decl_specifiers declarator corpus</productio>"       /* P203 */
    "    </regula>"

    /* P204-P206: NOMEN_TYPUS in decl_specifier, specifier_singulum, directus_declarator */
    "    <regula titulus=\"decl_specifier\">"
    "      <productio>NOMEN_TYPUS</productio>"                             /* P204 */
    "    </regula>"

    "    <regula titulus=\"specifier_singulum\">"
    "      <productio>NOMEN_TYPUS</productio>"                             /* P205 */
    "    </regula>"

    "    <regula titulus=\"directus_declarator\">"
    "      <productio>NOMEN_TYPUS</productio>"                             /* P206 */
    "    </regula>"

    "  </regulae>"
    "  <initium>summum</initium>"
    "</grammatica>";

/* ================================================
 * Functiones Auxiliares pro Creatione AST
 * ================================================ */

/* Creare Arbor2Token ex Arbor2Lexema */
hic_manens Arbor2Token*
_token_ex_lexema(
    LapifexC89Contextus*  ctx,
    Arbor2Lexema*         lexema)
{
    redde arbor2_token_ex_lexema(
        ctx->piscina, lexema, ctx->via_file,
        lexema->linea, lexema->columna);
}

/* Creare nodum folium (IDENTIFICATOR, INTEGER, etc.) */
hic_manens Arbor2Nodus*
_nodus_folium(
    LapifexC89Contextus*  ctx,
    Arbor2NodusGenus      genus,
    Arbor2Lexema*         lexema)
{
    Arbor2Nodus* nodus;

    nodus = (Arbor2Nodus*)piscina_allocare(ctx->piscina,
        (memoriae_index)magnitudo(Arbor2Nodus));
    memset(nodus, ZEPHYRUM, magnitudo(Arbor2Nodus));

    nodus->genus = genus;
    nodus->lexema = _token_ex_lexema(ctx, lexema);
    nodus->datum.folium.valor = lexema->valor;

    nodus->linea_initium = lexema->linea;
    nodus->columna_initium = lexema->columna;
    nodus->linea_finis = lexema->linea;
    nodus->columna_finis = lexema->columna + lexema->longitudo;

    redde nodus;
}

/* Creare nodum binarium (BINARIUM) */
hic_manens Arbor2Nodus*
_nodus_binarium(
    LapifexC89Contextus*  ctx,
    Arbor2Nodus*          sinister,
    Arbor2Lexema*         op_lexema,
    Arbor2Nodus*          dexter)
{
    Arbor2Nodus* nodus;

    nodus = (Arbor2Nodus*)piscina_allocare(ctx->piscina,
        (memoriae_index)magnitudo(Arbor2Nodus));
    memset(nodus, ZEPHYRUM, magnitudo(Arbor2Nodus));

    nodus->genus = ARBOR2_NODUS_BINARIUM;
    nodus->lexema = _token_ex_lexema(ctx, op_lexema);
    nodus->datum.binarium.sinister = sinister;
    nodus->datum.binarium.tok_operator = nodus->lexema;
    nodus->datum.binarium.operator = op_lexema->genus;
    nodus->datum.binarium.dexter = dexter;

    sinister->pater = nodus;
    dexter->pater = nodus;

    nodus->linea_initium = sinister->linea_initium;
    nodus->columna_initium = sinister->columna_initium;
    nodus->linea_finis = dexter->linea_finis;
    nodus->columna_finis = dexter->columna_finis;

    redde nodus;
}

/* Creare nodum unarium praefixum (UNARIUM) */
hic_manens Arbor2Nodus*
_nodus_unarium(
    LapifexC89Contextus*  ctx,
    Arbor2Lexema*         op_lexema,
    Arbor2Nodus*          operandum)
{
    Arbor2Nodus* nodus;

    nodus = (Arbor2Nodus*)piscina_allocare(ctx->piscina,
        (memoriae_index)magnitudo(Arbor2Nodus));
    memset(nodus, ZEPHYRUM, magnitudo(Arbor2Nodus));

    nodus->genus = ARBOR2_NODUS_UNARIUM;
    nodus->lexema = _token_ex_lexema(ctx, op_lexema);
    nodus->datum.unarium.tok_operator = nodus->lexema;
    nodus->datum.unarium.operator = op_lexema->genus;
    nodus->datum.unarium.operandum = operandum;

    operandum->pater = nodus;

    nodus->linea_initium = op_lexema->linea;
    nodus->columna_initium = op_lexema->columna;
    nodus->linea_finis = operandum->linea_finis;
    nodus->columna_finis = operandum->columna_finis;

    redde nodus;
}

/* Creare nodum post-unarium (POST_UNARIUM) */
hic_manens Arbor2Nodus*
_nodus_post_unarium(
    LapifexC89Contextus*  ctx,
    Arbor2Nodus*          operandum,
    Arbor2Lexema*         op_lexema)
{
    Arbor2Nodus* nodus;

    nodus = (Arbor2Nodus*)piscina_allocare(ctx->piscina,
        (memoriae_index)magnitudo(Arbor2Nodus));
    memset(nodus, ZEPHYRUM, magnitudo(Arbor2Nodus));

    nodus->genus = ARBOR2_NODUS_POST_UNARIUM;
    nodus->lexema = _token_ex_lexema(ctx, op_lexema);
    nodus->datum.post_unarium.operandum = operandum;
    nodus->datum.post_unarium.tok_operator = nodus->lexema;
    nodus->datum.post_unarium.operator = op_lexema->genus;

    operandum->pater = nodus;

    nodus->linea_initium = operandum->linea_initium;
    nodus->columna_initium = operandum->columna_initium;
    nodus->linea_finis = op_lexema->linea;
    nodus->columna_finis = op_lexema->columna + op_lexema->longitudo;

    redde nodus;
}

/* Creare nodum parenthesized */
hic_manens Arbor2Nodus*
_nodus_parenthesized(
    LapifexC89Contextus*  ctx,
    Arbor2Lexema*         tok_ap,
    Arbor2Nodus*          expr,
    Arbor2Lexema*         tok_cl)
{
    Arbor2Nodus* nodus;

    nodus = (Arbor2Nodus*)piscina_allocare(ctx->piscina,
        (memoriae_index)magnitudo(Arbor2Nodus));
    memset(nodus, ZEPHYRUM, magnitudo(Arbor2Nodus));

    nodus->genus = ARBOR2_NODUS_PARENTHESIZED;
    nodus->lexema = _token_ex_lexema(ctx, tok_ap);
    nodus->datum.parenthesized.tok_paren_ap = nodus->lexema;
    nodus->datum.parenthesized.expressio = expr;
    nodus->datum.parenthesized.tok_paren_cl = _token_ex_lexema(ctx, tok_cl);

    expr->pater = nodus;

    nodus->linea_initium = tok_ap->linea;
    nodus->columna_initium = tok_ap->columna;
    nodus->linea_finis = tok_cl->linea;
    nodus->columna_finis = tok_cl->columna + tok_cl->longitudo;

    redde nodus;
}

/* Creare nodum vocatio (function call) */
hic_manens Arbor2Nodus*
_nodus_vocatio(
    LapifexC89Contextus*  ctx,
    Arbor2Nodus*          basis,
    Arbor2Lexema*         tok_ap,
    Arbor2Nodus*          argumenta,
    Arbor2Lexema*         tok_cl)
{
    Arbor2Nodus* nodus;

    nodus = (Arbor2Nodus*)piscina_allocare(ctx->piscina,
        (memoriae_index)magnitudo(Arbor2Nodus));
    memset(nodus, ZEPHYRUM, magnitudo(Arbor2Nodus));

    nodus->genus = ARBOR2_NODUS_VOCATIO;
    nodus->lexema = basis->lexema;
    nodus->datum.vocatio.basis = basis;
    nodus->datum.vocatio.tok_paren_ap = _token_ex_lexema(ctx, tok_ap);
    nodus->datum.vocatio.argumenta = argumenta;
    nodus->datum.vocatio.tok_paren_cl = _token_ex_lexema(ctx, tok_cl);

    basis->pater = nodus;
    si (argumenta) argumenta->pater = nodus;

    nodus->linea_initium = basis->linea_initium;
    nodus->columna_initium = basis->columna_initium;
    nodus->linea_finis = tok_cl->linea;
    nodus->columna_finis = tok_cl->columna + tok_cl->longitudo;

    redde nodus;
}

/* Creare nodum subscriptio (array subscript) */
hic_manens Arbor2Nodus*
_nodus_subscriptio(
    LapifexC89Contextus*  ctx,
    Arbor2Nodus*          basis,
    Arbor2Lexema*         tok_ap,
    Arbor2Nodus*          index_expr,
    Arbor2Lexema*         tok_cl)
{
    Arbor2Nodus* nodus;

    nodus = (Arbor2Nodus*)piscina_allocare(ctx->piscina,
        (memoriae_index)magnitudo(Arbor2Nodus));
    memset(nodus, ZEPHYRUM, magnitudo(Arbor2Nodus));

    nodus->genus = ARBOR2_NODUS_SUBSCRIPTIO;
    nodus->lexema = basis->lexema;
    nodus->datum.subscriptio.basis = basis;
    nodus->datum.subscriptio.tok_bracket_ap = _token_ex_lexema(ctx, tok_ap);
    nodus->datum.subscriptio.index = index_expr;
    nodus->datum.subscriptio.tok_bracket_cl = _token_ex_lexema(ctx, tok_cl);

    basis->pater = nodus;
    index_expr->pater = nodus;

    nodus->linea_initium = basis->linea_initium;
    nodus->columna_initium = basis->columna_initium;
    nodus->linea_finis = tok_cl->linea;
    nodus->columna_finis = tok_cl->columna + tok_cl->longitudo;

    redde nodus;
}

/* Creare nodum membrum (member access . or ->) */
hic_manens Arbor2Nodus*
_nodus_membrum(
    LapifexC89Contextus*  ctx,
    Arbor2Nodus*          basis,
    Arbor2Lexema*         tok_acc,
    Arbor2Lexema*         tok_mem,
    b32                   est_sagitta)
{
    Arbor2Nodus* nodus;

    nodus = (Arbor2Nodus*)piscina_allocare(ctx->piscina,
        (memoriae_index)magnitudo(Arbor2Nodus));
    memset(nodus, ZEPHYRUM, magnitudo(Arbor2Nodus));

    nodus->genus = ARBOR2_NODUS_MEMBRUM;
    nodus->lexema = basis->lexema;
    nodus->datum.membrum.basis = basis;
    nodus->datum.membrum.tok_accessor = _token_ex_lexema(ctx, tok_acc);
    nodus->datum.membrum.tok_membrum = _token_ex_lexema(ctx, tok_mem);
    nodus->datum.membrum.membrum = tok_mem->valor;
    nodus->datum.membrum.est_sagitta = est_sagitta;

    basis->pater = nodus;

    nodus->linea_initium = basis->linea_initium;
    nodus->columna_initium = basis->columna_initium;
    nodus->linea_finis = tok_mem->linea;
    nodus->columna_finis = tok_mem->columna + tok_mem->longitudo;

    redde nodus;
}

/* Creare nodum ternarium */
hic_manens Arbor2Nodus*
_nodus_ternarius(
    LapifexC89Contextus*  ctx,
    Arbor2Nodus*          conditio,
    Arbor2Lexema*         tok_q,
    Arbor2Nodus*          verum_expr,
    Arbor2Lexema*         tok_c,
    Arbor2Nodus*          falsum_expr)
{
    Arbor2Nodus* nodus;

    nodus = (Arbor2Nodus*)piscina_allocare(ctx->piscina,
        (memoriae_index)magnitudo(Arbor2Nodus));
    memset(nodus, ZEPHYRUM, magnitudo(Arbor2Nodus));

    nodus->genus = ARBOR2_NODUS_TERNARIUS;
    nodus->lexema = conditio->lexema;
    nodus->datum.ternarius.conditio = conditio;
    nodus->datum.ternarius.tok_interrogatio = _token_ex_lexema(ctx, tok_q);
    nodus->datum.ternarius.verum = verum_expr;
    nodus->datum.ternarius.tok_colon = _token_ex_lexema(ctx, tok_c);
    nodus->datum.ternarius.falsum = falsum_expr;

    conditio->pater = nodus;
    verum_expr->pater = nodus;
    falsum_expr->pater = nodus;

    nodus->linea_initium = conditio->linea_initium;
    nodus->columna_initium = conditio->columna_initium;
    nodus->linea_finis = falsum_expr->linea_finis;
    nodus->columna_finis = falsum_expr->columna_finis;

    redde nodus;
}

/* Creare nodum sizeof expr */
hic_manens Arbor2Nodus*
_nodus_sizeof_expr(
    LapifexC89Contextus*  ctx,
    Arbor2Lexema*         tok_sizeof,
    Arbor2Nodus*          operandum)
{
    Arbor2Nodus* nodus;

    nodus = (Arbor2Nodus*)piscina_allocare(ctx->piscina,
        (memoriae_index)magnitudo(Arbor2Nodus));
    memset(nodus, ZEPHYRUM, magnitudo(Arbor2Nodus));

    nodus->genus = ARBOR2_NODUS_SIZEOF;
    nodus->lexema = _token_ex_lexema(ctx, tok_sizeof);
    nodus->datum.sizeof_expr.tok_sizeof = nodus->lexema;
    nodus->datum.sizeof_expr.tok_paren_ap = NIHIL;
    nodus->datum.sizeof_expr.est_typus = FALSUM;
    nodus->datum.sizeof_expr.operandum = operandum;
    nodus->datum.sizeof_expr.tok_paren_cl = NIHIL;

    operandum->pater = nodus;

    nodus->linea_initium = tok_sizeof->linea;
    nodus->columna_initium = tok_sizeof->columna;
    nodus->linea_finis = operandum->linea_finis;
    nodus->columna_finis = operandum->columna_finis;

    redde nodus;
}

/* Creare nodum sizeof(type) */
hic_manens Arbor2Nodus*
_nodus_sizeof_typus(
    LapifexC89Contextus*  ctx,
    Arbor2Lexema*         tok_sizeof,
    Arbor2Lexema*         tok_ap,
    Arbor2Nodus*          typus,
    Arbor2Lexema*         tok_cl)
{
    Arbor2Nodus* nodus;

    nodus = (Arbor2Nodus*)piscina_allocare(ctx->piscina,
        (memoriae_index)magnitudo(Arbor2Nodus));
    memset(nodus, ZEPHYRUM, magnitudo(Arbor2Nodus));

    nodus->genus = ARBOR2_NODUS_SIZEOF;
    nodus->lexema = _token_ex_lexema(ctx, tok_sizeof);
    nodus->datum.sizeof_expr.tok_sizeof = nodus->lexema;
    nodus->datum.sizeof_expr.tok_paren_ap = _token_ex_lexema(ctx, tok_ap);
    nodus->datum.sizeof_expr.est_typus = VERUM;
    nodus->datum.sizeof_expr.operandum = typus;
    nodus->datum.sizeof_expr.tok_paren_cl = _token_ex_lexema(ctx, tok_cl);

    typus->pater = nodus;

    nodus->linea_initium = tok_sizeof->linea;
    nodus->columna_initium = tok_sizeof->columna;
    nodus->linea_finis = tok_cl->linea;
    nodus->columna_finis = tok_cl->columna + tok_cl->longitudo;

    redde nodus;
}

/* Creare nodum conversio (cast) */
hic_manens Arbor2Nodus*
_nodus_conversio(
    LapifexC89Contextus*  ctx,
    Arbor2Lexema*         tok_ap,
    Arbor2Nodus*          typus,
    Arbor2Lexema*         tok_cl,
    Arbor2Nodus*          expr)
{
    Arbor2Nodus* nodus;

    nodus = (Arbor2Nodus*)piscina_allocare(ctx->piscina,
        (memoriae_index)magnitudo(Arbor2Nodus));
    memset(nodus, ZEPHYRUM, magnitudo(Arbor2Nodus));

    nodus->genus = ARBOR2_NODUS_CONVERSIO;
    nodus->lexema = _token_ex_lexema(ctx, tok_ap);
    nodus->datum.conversio.tok_paren_ap = nodus->lexema;
    nodus->datum.conversio.typus = typus;
    nodus->datum.conversio.tok_paren_cl = _token_ex_lexema(ctx, tok_cl);
    nodus->datum.conversio.expressio = expr;

    typus->pater = nodus;
    expr->pater = nodus;

    nodus->linea_initium = tok_ap->linea;
    nodus->columna_initium = tok_ap->columna;
    nodus->linea_finis = expr->linea_finis;
    nodus->columna_finis = expr->columna_finis;

    redde nodus;
}

/* Creare nodum LISTA_SEPARATA (pro argumentis et comma expressions) */
hic_manens Arbor2Nodus*
_nodus_lista_separata(
    LapifexC89Contextus*  ctx,
    Arbor2Nodus*          primum,
    Arbor2Lexema*         tok_comma,
    Arbor2Nodus*          secundum)
{
    Arbor2Nodus* nodus;
    Arbor2Token* sep_tok;

    /* Si primum iam est LISTA_SEPARATA, addere ad eam */
    si (primum->genus == ARBOR2_NODUS_LISTA_SEPARATA)
    {
        Arbor2Token** sep_slot;
        Arbor2Nodus** elem_slot;

        sep_tok = _token_ex_lexema(ctx, tok_comma);
        sep_slot = (Arbor2Token**)xar_addere(primum->datum.lista_separata.separatores);
        *sep_slot = sep_tok;
        elem_slot = (Arbor2Nodus**)xar_addere(primum->datum.lista_separata.elementa);
        *elem_slot = secundum;
        secundum->pater = primum;
        /* Actualizare finis */
        primum->linea_finis = secundum->linea_finis;
        primum->columna_finis = secundum->columna_finis;
        redde primum;
    }

    /* Creare novam LISTA_SEPARATA */
    {
        Arbor2Nodus** elem_slot;
        Arbor2Token** sep_slot;

        nodus = (Arbor2Nodus*)piscina_allocare(ctx->piscina,
            (memoriae_index)magnitudo(Arbor2Nodus));
        memset(nodus, ZEPHYRUM, magnitudo(Arbor2Nodus));

        nodus->genus = ARBOR2_NODUS_LISTA_SEPARATA;
        nodus->lexema = primum->lexema;
        nodus->datum.lista_separata.elementa =
            xar_creare(ctx->piscina, (i32)magnitudo(Arbor2Nodus*));
        nodus->datum.lista_separata.separatores =
            xar_creare(ctx->piscina, (i32)magnitudo(Arbor2Token*));

        elem_slot = (Arbor2Nodus**)xar_addere(nodus->datum.lista_separata.elementa);
        *elem_slot = primum;
        sep_tok = _token_ex_lexema(ctx, tok_comma);
        sep_slot = (Arbor2Token**)xar_addere(nodus->datum.lista_separata.separatores);
        *sep_slot = sep_tok;
        elem_slot = (Arbor2Nodus**)xar_addere(nodus->datum.lista_separata.elementa);
        *elem_slot = secundum;
    }

    primum->pater = nodus;
    secundum->pater = nodus;

    nodus->linea_initium = primum->linea_initium;
    nodus->columna_initium = primum->columna_initium;
    nodus->linea_finis = secundum->linea_finis;
    nodus->columna_finis = secundum->columna_finis;

    redde nodus;
}

/* Creare nodum pro nomen_typi (placeholder nodus pro type names)
 * Usamus ARBOR2_NODUS_DECLARATIO genus ad minimum pro type info
 * Sed simpliciter: creare IDENTIFICATOR nodum cum specifier text
 */
hic_manens Arbor2Nodus*
_nodus_specifier_folium(
    LapifexC89Contextus*  ctx,
    Arbor2Lexema*         lexema)
{
    /* Reusare folium -- genus IDENTIFICATOR sed cum keyword token */
    redde _nodus_folium(ctx, ARBOR2_NODUS_IDENTIFICATOR, lexema);
}

/* Creare nodum pro specifier_lista (chain of specifiers) */
hic_manens Arbor2Nodus*
_nodus_specifier_lista(
    LapifexC89Contextus*  ctx,
    Arbor2Nodus*          spec,
    Arbor2Nodus*          lista)
{
    Arbor2Nodus* nodus;

    /* Pro simplici specifier, solum redde spec */
    si (!lista) redde spec;

    /* Creare LISTA_SEPARATA sine separatoribus (spatia-separata) */
    /* Sed pro simplicitate: creare BINARIUM nodum ad cathena specifiers */
    /* Realiter: melius est uti DECLARATIO nodo, sed pro M2.3 usamus
     * approach simplex -- sinister est primum specifier,
     * dexter est reliqua lista */
    nodus = (Arbor2Nodus*)piscina_allocare(ctx->piscina,
        (memoriae_index)magnitudo(Arbor2Nodus));
    memset(nodus, ZEPHYRUM, magnitudo(Arbor2Nodus));

    /* Usamus IDENTIFICATOR genus pro type name node --
     * hoc est placeholder quod M2.4+ meliorabit */
    nodus->genus = ARBOR2_NODUS_IDENTIFICATOR;
    nodus->lexema = spec->lexema;
    nodus->datum.folium.valor = spec->datum.folium.valor;
    nodus->pater = NIHIL;

    nodus->linea_initium = spec->linea_initium;
    nodus->columna_initium = spec->columna_initium;
    nodus->linea_finis = lista->linea_finis;
    nodus->columna_finis = lista->columna_finis;

    redde nodus;
}

/* ================================================
 * Functiones Auxiliares pro Declarationibus
 * ================================================ */

/* Creare nodum pointer chain ex productionibus P90-P93.
 * Quisque * creatur ut Arbor2PointerLevel in Xar.
 * In M2.3 pointer reductions reddebant valori[ZEPHYRUM] solum.
 * In M2.4 aedificamus Xar de Arbor2PointerLevel*.
 */
hic_manens Xar*
_pointer_chain_creare(
    LapifexC89Contextus*  ctx,
    Arbor2Lexema*         tok_stella,
    b32                   habet_const,
    Arbor2Lexema*         tok_const_lex,
    Xar*                  catena_interior)
{
    Xar*                 catena;
    Arbor2PointerLevel*  gradus;
    Arbor2PointerLevel** slot;

    /* Si iam habemus catenam interiorem, addere ad eam in fronte */
    si (catena_interior)
        catena = catena_interior;
    alioquin
        catena = xar_creare(ctx->piscina, (i32)magnitudo(Arbor2PointerLevel*));

    /* Creare novum gradum */
    gradus = (Arbor2PointerLevel*)piscina_allocare(ctx->piscina,
        (memoriae_index)magnitudo(Arbor2PointerLevel));
    gradus->tok_stella = _token_ex_lexema(ctx, tok_stella);
    gradus->tok_const = habet_const ? _token_ex_lexema(ctx, tok_const_lex) : NIHIL;
    gradus->tok_volatile = NIHIL;

    /* Inserere in fronte (externus pointer primus) */
    /* Quia Xar non habet "inserere in fronte", addimus ad finem
     * et invertimus ordinem in _nodus_declarator.
     * Sed realiter: pointer -> ASTERISCUS pointer significat
     * externus * est in valori[0], interior est in valori[1].
     * Ergo addere ad finem est correctum: externi primi. */
    slot = (Arbor2PointerLevel**)xar_addere(catena);
    *slot = gradus;

    redde catena;
}

/* Creare nodum declarator (DECLARATOR) */
hic_manens Arbor2Nodus*
_nodus_declarator(
    LapifexC89Contextus*  ctx,
    Xar*                  pointer_levels,
    Arbor2Nodus*          directus)
{
    Arbor2Nodus* nodus;

    nodus = (Arbor2Nodus*)piscina_allocare(ctx->piscina,
        (memoriae_index)magnitudo(Arbor2Nodus));
    memset(nodus, ZEPHYRUM, magnitudo(Arbor2Nodus));

    nodus->genus = ARBOR2_NODUS_DECLARATOR;
    nodus->lexema = directus->lexema;
    nodus->datum.declarator.pointer_levels = pointer_levels;
    nodus->datum.declarator.titulus = directus->datum.folium.valor;
    nodus->datum.declarator.latitudo_biti = NIHIL;
    nodus->datum.declarator.dimensiones = NIHIL;

    nodus->linea_initium = directus->linea_initium;
    nodus->columna_initium = directus->columna_initium;
    nodus->linea_finis = directus->linea_finis;
    nodus->columna_finis = directus->columna_finis;

    redde nodus;
}

/* Auxiliaris: creare nodum DECLARATIO ab init_declarator_lista et decl_specifiers.
 * decl_specifiers est LISTA_SEPARATA vel singulum nodus de specifiers.
 * init_declarator_lista est LISTA_SEPARATA vel singulum declarator.
 *
 * Pro simplici casu (int x;): specifiers = unum folium, declarator = unum DECLARATOR.
 * Pro multi (int a, b;): init_declarator_lista est LISTA_SEPARATA.
 */
hic_manens Arbor2Nodus*
_nodus_declaratio(
    LapifexC89Contextus*  ctx,
    Arbor2Nodus*          specifiers,
    Arbor2Nodus*          init_decl_lista,
    Arbor2Lexema*         tok_semi)
{
    Arbor2Nodus* nodus;
    Arbor2Nodus* primus_decl;

    nodus = (Arbor2Nodus*)piscina_allocare(ctx->piscina,
        (memoriae_index)magnitudo(Arbor2Nodus));
    memset(nodus, ZEPHYRUM, magnitudo(Arbor2Nodus));

    nodus->genus = ARBOR2_NODUS_DECLARATIO;

    /* Specifier est primus token */
    nodus->lexema = specifiers->lexema;
    nodus->datum.declaratio.specifier = specifiers;

    /* Si init_decl_lista est LISTA_SEPARATA, primus elementum est declarator */
    si (init_decl_lista)
    {
        si (init_decl_lista->genus == ARBOR2_NODUS_LISTA_SEPARATA)
        {
            Arbor2Nodus** elem = (Arbor2Nodus**)xar_obtinere(
                init_decl_lista->datum.lista_separata.elementa, ZEPHYRUM);
            primus_decl = *elem;
        }
        alioquin
        {
            primus_decl = init_decl_lista;
        }
        nodus->datum.declaratio.declarator = primus_decl;

        /* Si plures declaratores, servare totam listam ut proxima */
        si (init_decl_lista->genus == ARBOR2_NODUS_LISTA_SEPARATA)
            nodus->datum.declaratio.proxima = init_decl_lista;
    }

    nodus->datum.declaratio.tok_semicolon = _token_ex_lexema(ctx, tok_semi);

    /* Locatio */
    nodus->linea_initium = specifiers->linea_initium;
    nodus->columna_initium = specifiers->columna_initium;
    nodus->linea_finis = tok_semi->linea;
    nodus->columna_finis = tok_semi->columna + tok_semi->longitudo;

    redde nodus;
}

/* Creare nodum declaratio sine declaratore (forward decl / standalone struct) */
hic_manens Arbor2Nodus*
_nodus_declaratio_vacua(
    LapifexC89Contextus*  ctx,
    Arbor2Nodus*          specifiers,
    Arbor2Lexema*         tok_semi)
{
    redde _nodus_declaratio(ctx, specifiers, NIHIL, tok_semi);
}

/* Creare nodum init_declarator: declarator = initializer */
hic_manens Arbor2Nodus*
_nodus_init_declarator(
    LapifexC89Contextus*  ctx,
    Arbor2Nodus*          declarator,
    Arbor2Lexema*         tok_assign,
    Arbor2Nodus*          initializor)
{
    Arbor2Nodus* nodus;

    /* Reusare DECLARATIO nodum pro init_declarator */
    nodus = (Arbor2Nodus*)piscina_allocare(ctx->piscina,
        (memoriae_index)magnitudo(Arbor2Nodus));
    memset(nodus, ZEPHYRUM, magnitudo(Arbor2Nodus));

    nodus->genus = ARBOR2_NODUS_DECLARATIO;
    nodus->lexema = declarator->lexema;
    nodus->datum.declaratio.declarator = declarator;
    nodus->datum.declaratio.tok_assignatio = _token_ex_lexema(ctx, tok_assign);
    nodus->datum.declaratio.initializor = initializor;

    nodus->linea_initium = declarator->linea_initium;
    nodus->columna_initium = declarator->columna_initium;
    nodus->linea_finis = initializor->linea_finis;
    nodus->columna_finis = initializor->columna_finis;

    redde nodus;
}

/* Catena specifier: accumulare specifiers in LISTA_SEPARATA */
hic_manens Arbor2Nodus*
_nodus_specifier_chain(
    LapifexC89Contextus*  ctx,
    Arbor2Nodus*          catena,
    Arbor2Nodus*          novum)
{
    Arbor2Nodus* nodus;
    Arbor2Nodus** elem_slot;

    /* Si catena iam est LISTA_SEPARATA, addere ad eam */
    si (catena->genus == ARBOR2_NODUS_LISTA_SEPARATA)
    {
        elem_slot = (Arbor2Nodus**)xar_addere(catena->datum.lista_separata.elementa);
        *elem_slot = novum;
        novum->pater = catena;
        catena->linea_finis = novum->linea_finis;
        catena->columna_finis = novum->columna_finis;
        redde catena;
    }

    /* Creare novam LISTA_SEPARATA */
    nodus = (Arbor2Nodus*)piscina_allocare(ctx->piscina,
        (memoriae_index)magnitudo(Arbor2Nodus));
    memset(nodus, ZEPHYRUM, magnitudo(Arbor2Nodus));

    nodus->genus = ARBOR2_NODUS_LISTA_SEPARATA;
    nodus->lexema = catena->lexema;
    nodus->datum.lista_separata.elementa =
        xar_creare(ctx->piscina, (i32)magnitudo(Arbor2Nodus*));
    nodus->datum.lista_separata.separatores =
        xar_creare(ctx->piscina, (i32)magnitudo(Arbor2Token*));

    elem_slot = (Arbor2Nodus**)xar_addere(nodus->datum.lista_separata.elementa);
    *elem_slot = catena;
    elem_slot = (Arbor2Nodus**)xar_addere(nodus->datum.lista_separata.elementa);
    *elem_slot = novum;

    catena->pater = nodus;
    novum->pater = nodus;

    nodus->linea_initium = catena->linea_initium;
    nodus->columna_initium = catena->columna_initium;
    nodus->linea_finis = novum->linea_finis;
    nodus->columna_finis = novum->columna_finis;

    redde nodus;
}

/* Creare nodum array dimension - extendere declarator */
hic_manens Arbor2Nodus*
_nodus_array_dim(
    LapifexC89Contextus*  ctx,
    Arbor2Nodus*          basis,
    Arbor2Lexema*         tok_brk_ap,
    Arbor2Nodus*          dimensio,
    Arbor2Lexema*         tok_brk_cl)
{
    Arbor2ArrayDimension*  dim;
    Arbor2ArrayDimension** slot;

    /* Si basis nondum habet dimensiones, creare Xar */
    si (!basis->datum.declarator.dimensiones)
        basis->datum.declarator.dimensiones =
            xar_creare(ctx->piscina, (i32)magnitudo(Arbor2ArrayDimension*));

    dim = (Arbor2ArrayDimension*)piscina_allocare(ctx->piscina,
        (memoriae_index)magnitudo(Arbor2ArrayDimension));
    dim->tok_bracket_ap = _token_ex_lexema(ctx, tok_brk_ap);
    dim->dimensio = dimensio;
    dim->tok_bracket_cl = _token_ex_lexema(ctx, tok_brk_cl);

    slot = (Arbor2ArrayDimension**)xar_addere(basis->datum.declarator.dimensiones);
    *slot = dim;

    basis->linea_finis = tok_brk_cl->linea;
    basis->columna_finis = tok_brk_cl->columna + tok_brk_cl->longitudo;

    redde basis;
}

/* Creare nodum function declarator */
hic_manens Arbor2Nodus*
_nodus_declarator_functi(
    LapifexC89Contextus*  ctx,
    Arbor2Nodus*          basis,
    Arbor2Lexema*         tok_ap,
    Arbor2Nodus*          parametri,
    Arbor2Lexema*         tok_cl,
    b32                   habet_void,
    b32                   est_variadicus)
{
    Arbor2Nodus* nodus;

    nodus = (Arbor2Nodus*)piscina_allocare(ctx->piscina,
        (memoriae_index)magnitudo(Arbor2Nodus));
    memset(nodus, ZEPHYRUM, magnitudo(Arbor2Nodus));

    nodus->genus = ARBOR2_NODUS_DECLARATOR_FUNCTI;
    nodus->lexema = basis->lexema;
    nodus->datum.declarator_functi.declarator_interior = basis;
    nodus->datum.declarator_functi.tok_paren_ap = _token_ex_lexema(ctx, tok_ap);
    nodus->datum.declarator_functi.parametri = parametri;
    nodus->datum.declarator_functi.tok_paren_cl = _token_ex_lexema(ctx, tok_cl);
    nodus->datum.declarator_functi.habet_void = habet_void;
    nodus->datum.declarator_functi.est_variadicus = est_variadicus;
    nodus->datum.declarator_functi.pointer_levels = NIHIL;
    nodus->datum.declarator_functi.tok_void = NIHIL;

    basis->pater = nodus;
    si (parametri) parametri->pater = nodus;

    nodus->linea_initium = basis->linea_initium;
    nodus->columna_initium = basis->columna_initium;
    nodus->linea_finis = tok_cl->linea;
    nodus->columna_finis = tok_cl->columna + tok_cl->longitudo;

    redde nodus;
}

/* Creare nodum struct/union specifier */
hic_manens Arbor2Nodus*
_nodus_struct_specifier(
    LapifexC89Contextus*  ctx,
    Arbor2Lexema*         tok_kw,
    Arbor2Nodus*          tag,
    Arbor2Lexema*         tok_br_ap,
    Arbor2Nodus*          membra,
    Arbor2Lexema*         tok_br_cl,
    b32                   est_unio)
{
    Arbor2Nodus* nodus;

    nodus = (Arbor2Nodus*)piscina_allocare(ctx->piscina,
        (memoriae_index)magnitudo(Arbor2Nodus));
    memset(nodus, ZEPHYRUM, magnitudo(Arbor2Nodus));

    nodus->genus = ARBOR2_NODUS_STRUCT_SPECIFIER;
    nodus->lexema = _token_ex_lexema(ctx, tok_kw);
    nodus->datum.struct_specifier.tok_struct_or_union = nodus->lexema;
    nodus->datum.struct_specifier.tag = tag;
    nodus->datum.struct_specifier.est_unio = est_unio;

    si (tok_br_ap)
    {
        nodus->datum.struct_specifier.tok_brace_ap = _token_ex_lexema(ctx, tok_br_ap);
        /* membra est LISTA_SEPARATA vel singulum, convertere ad Xar */
        si (membra)
        {
            Xar* membra_xar = xar_creare(ctx->piscina, (i32)magnitudo(Arbor2Nodus*));
            si (membra->genus == ARBOR2_NODUS_LISTA_SEPARATA)
            {
                i32 i;
                per (i = ZEPHYRUM; i < (i32)xar_numerus(membra->datum.lista_separata.elementa); i++)
                {
                    Arbor2Nodus** src = (Arbor2Nodus**)xar_obtinere(
                        membra->datum.lista_separata.elementa, i);
                    Arbor2Nodus** dst = (Arbor2Nodus**)xar_addere(membra_xar);
                    *dst = *src;
                }
            }
            alioquin
            {
                Arbor2Nodus** dst = (Arbor2Nodus**)xar_addere(membra_xar);
                *dst = membra;
            }
            nodus->datum.struct_specifier.membra = membra_xar;
        }
        nodus->datum.struct_specifier.tok_brace_cl = _token_ex_lexema(ctx, tok_br_cl);
        nodus->linea_finis = tok_br_cl->linea;
        nodus->columna_finis = tok_br_cl->columna + tok_br_cl->longitudo;
    }
    alioquin
    {
        /* Forward ref: struct Foo */
        nodus->datum.struct_specifier.tok_brace_ap = NIHIL;
        nodus->datum.struct_specifier.membra = NIHIL;
        nodus->datum.struct_specifier.tok_brace_cl = NIHIL;
        si (tag)
        {
            nodus->linea_finis = tag->linea_finis;
            nodus->columna_finis = tag->columna_finis;
        }
    }

    nodus->linea_initium = tok_kw->linea;
    nodus->columna_initium = tok_kw->columna;

    redde nodus;
}

/* Creare nodum enum specifier */
hic_manens Arbor2Nodus*
_nodus_enum_specifier(
    LapifexC89Contextus*  ctx,
    Arbor2Lexema*         tok_enum,
    Arbor2Nodus*          tag,
    Arbor2Lexema*         tok_br_ap,
    Arbor2Nodus*          enumeratores,
    Arbor2Lexema*         tok_br_cl)
{
    Arbor2Nodus* nodus;

    nodus = (Arbor2Nodus*)piscina_allocare(ctx->piscina,
        (memoriae_index)magnitudo(Arbor2Nodus));
    memset(nodus, ZEPHYRUM, magnitudo(Arbor2Nodus));

    nodus->genus = ARBOR2_NODUS_ENUM_SPECIFIER;
    nodus->lexema = _token_ex_lexema(ctx, tok_enum);
    nodus->datum.enum_specifier.tok_enum = nodus->lexema;
    nodus->datum.enum_specifier.tag = tag;

    si (tok_br_ap)
    {
        nodus->datum.enum_specifier.tok_brace_ap = _token_ex_lexema(ctx, tok_br_ap);
        nodus->datum.enum_specifier.enumeratores = enumeratores;
        nodus->datum.enum_specifier.tok_brace_cl = _token_ex_lexema(ctx, tok_br_cl);
        nodus->linea_finis = tok_br_cl->linea;
        nodus->columna_finis = tok_br_cl->columna + tok_br_cl->longitudo;
    }
    alioquin
    {
        nodus->datum.enum_specifier.tok_brace_ap = NIHIL;
        nodus->datum.enum_specifier.enumeratores = NIHIL;
        nodus->datum.enum_specifier.tok_brace_cl = NIHIL;
        si (tag)
        {
            nodus->linea_finis = tag->linea_finis;
            nodus->columna_finis = tag->columna_finis;
        }
    }

    nodus->linea_initium = tok_enum->linea;
    nodus->columna_initium = tok_enum->columna;

    redde nodus;
}

/* Creare nodum enumerator */
hic_manens Arbor2Nodus*
_nodus_enumerator(
    LapifexC89Contextus*  ctx,
    Arbor2Lexema*         tok_titulus,
    Arbor2Lexema*         tok_assign,
    Arbor2Nodus*          valor)
{
    Arbor2Nodus* nodus;

    nodus = (Arbor2Nodus*)piscina_allocare(ctx->piscina,
        (memoriae_index)magnitudo(Arbor2Nodus));
    memset(nodus, ZEPHYRUM, magnitudo(Arbor2Nodus));

    nodus->genus = ARBOR2_NODUS_ENUMERATOR;
    nodus->lexema = _token_ex_lexema(ctx, tok_titulus);
    nodus->datum.enumerator.titulus = tok_titulus->valor;
    nodus->datum.enumerator.tok_assignatio =
        tok_assign ? _token_ex_lexema(ctx, tok_assign) : NIHIL;
    nodus->datum.enumerator.valor = valor;

    nodus->linea_initium = tok_titulus->linea;
    nodus->columna_initium = tok_titulus->columna;
    si (valor)
    {
        nodus->linea_finis = valor->linea_finis;
        nodus->columna_finis = valor->columna_finis;
    }
    alioquin
    {
        nodus->linea_finis = tok_titulus->linea;
        nodus->columna_finis = tok_titulus->columna + tok_titulus->longitudo;
    }

    redde nodus;
}

/* Creare nodum parameter_decl */
hic_manens Arbor2Nodus*
_nodus_parameter_decl(
    LapifexC89Contextus*  ctx,
    Arbor2Nodus*          specifiers,
    Arbor2Nodus*          declarator)
{
    Arbor2Nodus* nodus;

    nodus = (Arbor2Nodus*)piscina_allocare(ctx->piscina,
        (memoriae_index)magnitudo(Arbor2Nodus));
    memset(nodus, ZEPHYRUM, magnitudo(Arbor2Nodus));

    nodus->genus = ARBOR2_NODUS_PARAMETER_DECL;
    nodus->lexema = specifiers->lexema;
    nodus->datum.parameter_decl.type_specifier = specifiers;
    nodus->datum.parameter_decl.declarator = declarator;

    nodus->linea_initium = specifiers->linea_initium;
    nodus->columna_initium = specifiers->columna_initium;
    si (declarator)
    {
        nodus->linea_finis = declarator->linea_finis;
        nodus->columna_finis = declarator->columna_finis;
    }
    alioquin
    {
        nodus->linea_finis = specifiers->linea_finis;
        nodus->columna_finis = specifiers->columna_finis;
    }

    redde nodus;
}

/* Creare nodum initializer lista {a, b, c} */
hic_manens Arbor2Nodus*
_nodus_initializor_lista(
    LapifexC89Contextus*  ctx,
    Arbor2Lexema*         tok_br_ap,
    Arbor2Nodus*          items,
    Arbor2Lexema*         tok_br_cl)
{
    Arbor2Nodus* nodus;

    nodus = (Arbor2Nodus*)piscina_allocare(ctx->piscina,
        (memoriae_index)magnitudo(Arbor2Nodus));
    memset(nodus, ZEPHYRUM, magnitudo(Arbor2Nodus));

    nodus->genus = ARBOR2_NODUS_INITIALIZOR_LISTA;
    nodus->lexema = _token_ex_lexema(ctx, tok_br_ap);
    nodus->datum.initializor_lista.tok_brace_ap = nodus->lexema;
    nodus->datum.initializor_lista.items = items;
    nodus->datum.initializor_lista.tok_brace_cl = _token_ex_lexema(ctx, tok_br_cl);

    si (items) items->pater = nodus;

    nodus->linea_initium = tok_br_ap->linea;
    nodus->columna_initium = tok_br_ap->columna;
    nodus->linea_finis = tok_br_cl->linea;
    nodus->columna_finis = tok_br_cl->columna + tok_br_cl->longitudo;

    redde nodus;
}

/* struct_member_lista: accumulare membra in LISTA_SEPARATA */
hic_manens Arbor2Nodus*
_nodus_struct_member_append(
    LapifexC89Contextus*  ctx,
    Arbor2Nodus*          lista,
    Arbor2Nodus*          membrum)
{
    /* Reusare _nodus_specifier_chain (same logic: append to LISTA_SEPARATA) */
    redde _nodus_specifier_chain(ctx, lista, membrum);
}

/* Creare nodum struct_member: decl_specifiers struct_declarator_lista SEMICOLON */
hic_manens Arbor2Nodus*
_nodus_struct_member(
    LapifexC89Contextus*  ctx,
    Arbor2Nodus*          specifiers,
    Arbor2Nodus*          decl_lista,
    Arbor2Lexema*         tok_semi)
{
    /* Reusare DECLARATIO nodum */
    redde _nodus_declaratio(ctx, specifiers, decl_lista, tok_semi);
}

/* Creare nodum struct_declarator cum bit field */
hic_manens Arbor2Nodus*
_nodus_bit_field(
    LapifexC89Contextus*  ctx,
    Arbor2Nodus*          declarator,
    Arbor2Lexema*         tok_colon,
    Arbor2Nodus*          latitudo)
{
    (vacuum)tok_colon;

    /* Extendere declarator nodum cum bit field info */
    si (declarator && declarator->genus == ARBOR2_NODUS_DECLARATOR)
    {
        declarator->datum.declarator.latitudo_biti = latitudo;
        declarator->linea_finis = latitudo->linea_finis;
        declarator->columna_finis = latitudo->columna_finis;
        redde declarator;
    }

    /* Si declarator non est DECLARATOR, creare novum */
    {
        Arbor2Nodus* nodus;
        nodus = (Arbor2Nodus*)piscina_allocare(ctx->piscina,
            (memoriae_index)magnitudo(Arbor2Nodus));
        memset(nodus, ZEPHYRUM, magnitudo(Arbor2Nodus));
        nodus->genus = ARBOR2_NODUS_DECLARATOR;
        nodus->lexema = declarator ? declarator->lexema : NIHIL;
        nodus->datum.declarator.latitudo_biti = latitudo;
        si (declarator)
        {
            nodus->datum.declarator.pointer_levels = NIHIL;
            nodus->datum.declarator.titulus = declarator->datum.folium.valor;
            nodus->linea_initium = declarator->linea_initium;
            nodus->columna_initium = declarator->columna_initium;
        }
        nodus->linea_finis = latitudo->linea_finis;
        nodus->columna_finis = latitudo->columna_finis;
        redde nodus;
    }
}

/* Creare nodum anonymous bit field (: expr) */
hic_manens Arbor2Nodus*
_nodus_anon_bit_field(
    LapifexC89Contextus*  ctx,
    Arbor2Lexema*         tok_colon,
    Arbor2Nodus*          latitudo)
{
    Arbor2Nodus* nodus;

    nodus = (Arbor2Nodus*)piscina_allocare(ctx->piscina,
        (memoriae_index)magnitudo(Arbor2Nodus));
    memset(nodus, ZEPHYRUM, magnitudo(Arbor2Nodus));

    nodus->genus = ARBOR2_NODUS_DECLARATOR;
    nodus->lexema = _token_ex_lexema(ctx, tok_colon);
    nodus->datum.declarator.pointer_levels = NIHIL;
    /* titulus vacua = anonymus */
    memset(&nodus->datum.declarator.titulus, ZEPHYRUM, magnitudo(chorda));
    nodus->datum.declarator.latitudo_biti = latitudo;
    nodus->datum.declarator.dimensiones = NIHIL;

    nodus->linea_initium = tok_colon->linea;
    nodus->columna_initium = tok_colon->columna;
    nodus->linea_finis = latitudo->linea_finis;
    nodus->columna_finis = latitudo->columna_finis;

    redde nodus;
}

/* ================================================
 * Functiones Auxiliares pro Sententiis
 * ================================================ */

/* PerClausula: structura interna pro for-loop clausulis */
nomen structura {
    Arbor2Nodus*  initium;
    Arbor2Lexema* tok_semicolon1;
    Arbor2Nodus*  conditio;
    Arbor2Lexema* tok_semicolon2;
    Arbor2Nodus*  incrementum;
} PerClausula;

/* Creare nodum SI (if statement) */
hic_manens Arbor2Nodus*
_nodus_si(
    LapifexC89Contextus*  ctx,
    Arbor2Lexema*         tok_si,
    Arbor2Lexema*         tok_pa,
    Arbor2Nodus*          conditio,
    Arbor2Lexema*         tok_pc,
    Arbor2Nodus*          consequens,
    Arbor2Lexema*         tok_alioquin,
    Arbor2Nodus*          alternans)
{
    Arbor2Nodus* nodus;

    nodus = (Arbor2Nodus*)piscina_allocare(ctx->piscina,
        (memoriae_index)magnitudo(Arbor2Nodus));
    memset(nodus, ZEPHYRUM, magnitudo(Arbor2Nodus));

    nodus->genus = ARBOR2_NODUS_SI;
    nodus->lexema = _token_ex_lexema(ctx, tok_si);
    nodus->datum.conditionale.tok_si = nodus->lexema;
    nodus->datum.conditionale.tok_paren_ap = _token_ex_lexema(ctx, tok_pa);
    nodus->datum.conditionale.conditio = conditio;
    nodus->datum.conditionale.tok_paren_cl = _token_ex_lexema(ctx, tok_pc);
    nodus->datum.conditionale.consequens = consequens;
    nodus->datum.conditionale.tok_alioquin =
        tok_alioquin ? _token_ex_lexema(ctx, tok_alioquin) : NIHIL;
    nodus->datum.conditionale.alternans = alternans;

    conditio->pater = nodus;
    consequens->pater = nodus;
    si (alternans) alternans->pater = nodus;

    nodus->linea_initium = tok_si->linea;
    nodus->columna_initium = tok_si->columna;
    si (alternans)
    {
        nodus->linea_finis = alternans->linea_finis;
        nodus->columna_finis = alternans->columna_finis;
    }
    alioquin
    {
        nodus->linea_finis = consequens->linea_finis;
        nodus->columna_finis = consequens->columna_finis;
    }

    redde nodus;
}

/* Creare nodum DUM (while statement) */
hic_manens Arbor2Nodus*
_nodus_dum(
    LapifexC89Contextus*  ctx,
    Arbor2Lexema*         tok_dum,
    Arbor2Lexema*         tok_pa,
    Arbor2Nodus*          conditio,
    Arbor2Lexema*         tok_pc,
    Arbor2Nodus*          corpus)
{
    Arbor2Nodus* nodus;

    nodus = (Arbor2Nodus*)piscina_allocare(ctx->piscina,
        (memoriae_index)magnitudo(Arbor2Nodus));
    memset(nodus, ZEPHYRUM, magnitudo(Arbor2Nodus));

    nodus->genus = ARBOR2_NODUS_DUM;
    nodus->lexema = _token_ex_lexema(ctx, tok_dum);
    nodus->datum.iteratio.tok_fac = NIHIL;
    nodus->datum.iteratio.tok_dum = nodus->lexema;
    nodus->datum.iteratio.tok_paren_ap = _token_ex_lexema(ctx, tok_pa);
    nodus->datum.iteratio.conditio = conditio;
    nodus->datum.iteratio.tok_paren_cl = _token_ex_lexema(ctx, tok_pc);
    nodus->datum.iteratio.corpus = corpus;
    nodus->datum.iteratio.tok_semicolon = NIHIL;

    conditio->pater = nodus;
    corpus->pater = nodus;

    nodus->linea_initium = tok_dum->linea;
    nodus->columna_initium = tok_dum->columna;
    nodus->linea_finis = corpus->linea_finis;
    nodus->columna_finis = corpus->columna_finis;

    redde nodus;
}

/* Creare nodum FAC (do-while statement) */
hic_manens Arbor2Nodus*
_nodus_fac_dum(
    LapifexC89Contextus*  ctx,
    Arbor2Lexema*         tok_fac,
    Arbor2Nodus*          corpus,
    Arbor2Lexema*         tok_dum,
    Arbor2Lexema*         tok_pa,
    Arbor2Nodus*          conditio,
    Arbor2Lexema*         tok_pc,
    Arbor2Lexema*         tok_semi)
{
    Arbor2Nodus* nodus;

    nodus = (Arbor2Nodus*)piscina_allocare(ctx->piscina,
        (memoriae_index)magnitudo(Arbor2Nodus));
    memset(nodus, ZEPHYRUM, magnitudo(Arbor2Nodus));

    nodus->genus = ARBOR2_NODUS_FAC;
    nodus->lexema = _token_ex_lexema(ctx, tok_fac);
    nodus->datum.iteratio.tok_fac = nodus->lexema;
    nodus->datum.iteratio.tok_dum = _token_ex_lexema(ctx, tok_dum);
    nodus->datum.iteratio.tok_paren_ap = _token_ex_lexema(ctx, tok_pa);
    nodus->datum.iteratio.conditio = conditio;
    nodus->datum.iteratio.tok_paren_cl = _token_ex_lexema(ctx, tok_pc);
    nodus->datum.iteratio.corpus = corpus;
    nodus->datum.iteratio.tok_semicolon = _token_ex_lexema(ctx, tok_semi);

    corpus->pater = nodus;
    conditio->pater = nodus;

    nodus->linea_initium = tok_fac->linea;
    nodus->columna_initium = tok_fac->columna;
    nodus->linea_finis = tok_semi->linea;
    nodus->columna_finis = tok_semi->columna + tok_semi->longitudo;

    redde nodus;
}

/* Creare nodum PER (for statement) ex PerClausula */
hic_manens Arbor2Nodus*
_nodus_per(
    LapifexC89Contextus*  ctx,
    Arbor2Lexema*         tok_per,
    Arbor2Lexema*         tok_pa,
    Arbor2Nodus*          clausula_nodus,
    Arbor2Lexema*         tok_pc,
    Arbor2Nodus*          corpus)
{
    Arbor2Nodus* nodus;
    PerClausula* clausula = (PerClausula*)(vacuum*)(longus)
        ((s64)(longus)(vacuum*)clausula_nodus);

    nodus = (Arbor2Nodus*)piscina_allocare(ctx->piscina,
        (memoriae_index)magnitudo(Arbor2Nodus));
    memset(nodus, ZEPHYRUM, magnitudo(Arbor2Nodus));

    nodus->genus = ARBOR2_NODUS_PER;
    nodus->lexema = _token_ex_lexema(ctx, tok_per);
    nodus->datum.circuitus.tok_per = nodus->lexema;
    nodus->datum.circuitus.tok_paren_ap = _token_ex_lexema(ctx, tok_pa);
    nodus->datum.circuitus.initium = clausula->initium;
    nodus->datum.circuitus.tok_semicolon1 = _token_ex_lexema(ctx, clausula->tok_semicolon1);
    nodus->datum.circuitus.conditio = clausula->conditio;
    nodus->datum.circuitus.tok_semicolon2 = _token_ex_lexema(ctx, clausula->tok_semicolon2);
    nodus->datum.circuitus.incrementum = clausula->incrementum;
    nodus->datum.circuitus.tok_paren_cl = _token_ex_lexema(ctx, tok_pc);
    nodus->datum.circuitus.corpus = corpus;

    si (clausula->initium) clausula->initium->pater = nodus;
    si (clausula->conditio) clausula->conditio->pater = nodus;
    si (clausula->incrementum) clausula->incrementum->pater = nodus;
    corpus->pater = nodus;

    nodus->linea_initium = tok_per->linea;
    nodus->columna_initium = tok_per->columna;
    nodus->linea_finis = corpus->linea_finis;
    nodus->columna_finis = corpus->columna_finis;

    redde nodus;
}

/* Creare nodum COMMUTATIO (switch statement) */
hic_manens Arbor2Nodus*
_nodus_commutatio(
    LapifexC89Contextus*  ctx,
    Arbor2Lexema*         tok_sw,
    Arbor2Lexema*         tok_pa,
    Arbor2Nodus*          expressio,
    Arbor2Lexema*         tok_pc,
    Arbor2Nodus*          corpus)
{
    Arbor2Nodus* nodus;

    nodus = (Arbor2Nodus*)piscina_allocare(ctx->piscina,
        (memoriae_index)magnitudo(Arbor2Nodus));
    memset(nodus, ZEPHYRUM, magnitudo(Arbor2Nodus));

    nodus->genus = ARBOR2_NODUS_COMMUTATIO;
    nodus->lexema = _token_ex_lexema(ctx, tok_sw);
    nodus->datum.selectivum.tok_commutatio = nodus->lexema;
    nodus->datum.selectivum.tok_paren_ap = _token_ex_lexema(ctx, tok_pa);
    nodus->datum.selectivum.expressio = expressio;
    nodus->datum.selectivum.tok_paren_cl = _token_ex_lexema(ctx, tok_pc);
    nodus->datum.selectivum.corpus = corpus;

    expressio->pater = nodus;
    corpus->pater = nodus;

    nodus->linea_initium = tok_sw->linea;
    nodus->columna_initium = tok_sw->columna;
    nodus->linea_finis = corpus->linea_finis;
    nodus->columna_finis = corpus->columna_finis;

    redde nodus;
}

/* Creare nodum REDDE (return statement) */
hic_manens Arbor2Nodus*
_nodus_redde(
    LapifexC89Contextus*  ctx,
    Arbor2Lexema*         tok_ret,
    Arbor2Nodus*          valor,
    Arbor2Lexema*         tok_semi)
{
    Arbor2Nodus* nodus;

    nodus = (Arbor2Nodus*)piscina_allocare(ctx->piscina,
        (memoriae_index)magnitudo(Arbor2Nodus));
    memset(nodus, ZEPHYRUM, magnitudo(Arbor2Nodus));

    nodus->genus = ARBOR2_NODUS_REDDE;
    nodus->lexema = _token_ex_lexema(ctx, tok_ret);
    nodus->datum.reditio.tok_redde = nodus->lexema;
    nodus->datum.reditio.valor = valor;
    nodus->datum.reditio.tok_semicolon = _token_ex_lexema(ctx, tok_semi);

    si (valor) valor->pater = nodus;

    nodus->linea_initium = tok_ret->linea;
    nodus->columna_initium = tok_ret->columna;
    nodus->linea_finis = tok_semi->linea;
    nodus->columna_finis = tok_semi->columna + tok_semi->longitudo;

    redde nodus;
}

/* Creare nodum SALTA (goto statement) */
hic_manens Arbor2Nodus*
_nodus_salta(
    LapifexC89Contextus*  ctx,
    Arbor2Lexema*         tok_salta,
    Arbor2Lexema*         tok_label,
    Arbor2Lexema*         tok_semi)
{
    Arbor2Nodus* nodus;

    nodus = (Arbor2Nodus*)piscina_allocare(ctx->piscina,
        (memoriae_index)magnitudo(Arbor2Nodus));
    memset(nodus, ZEPHYRUM, magnitudo(Arbor2Nodus));

    nodus->genus = ARBOR2_NODUS_SALTA;
    nodus->lexema = _token_ex_lexema(ctx, tok_salta);
    nodus->datum.saltus.tok_salta = nodus->lexema;
    nodus->datum.saltus.tok_destinatio = _token_ex_lexema(ctx, tok_label);
    nodus->datum.saltus.destinatio = tok_label->valor;
    nodus->datum.saltus.tok_semicolon = _token_ex_lexema(ctx, tok_semi);

    nodus->linea_initium = tok_salta->linea;
    nodus->columna_initium = tok_salta->columna;
    nodus->linea_finis = tok_semi->linea;
    nodus->columna_finis = tok_semi->columna + tok_semi->longitudo;

    redde nodus;
}

/* Creare nodum FRANGE (break statement) */
hic_manens Arbor2Nodus*
_nodus_frange(
    LapifexC89Contextus*  ctx,
    Arbor2Lexema*         tok_frange,
    Arbor2Lexema*         tok_semi)
{
    Arbor2Nodus* nodus;

    nodus = (Arbor2Nodus*)piscina_allocare(ctx->piscina,
        (memoriae_index)magnitudo(Arbor2Nodus));
    memset(nodus, ZEPHYRUM, magnitudo(Arbor2Nodus));

    nodus->genus = ARBOR2_NODUS_FRANGE;
    nodus->lexema = _token_ex_lexema(ctx, tok_frange);
    nodus->datum.frangendum.tok_frange = nodus->lexema;
    nodus->datum.frangendum.tok_semicolon = _token_ex_lexema(ctx, tok_semi);

    nodus->linea_initium = tok_frange->linea;
    nodus->columna_initium = tok_frange->columna;
    nodus->linea_finis = tok_semi->linea;
    nodus->columna_finis = tok_semi->columna + tok_semi->longitudo;

    redde nodus;
}

/* Creare nodum PERGE (continue statement) */
hic_manens Arbor2Nodus*
_nodus_perge(
    LapifexC89Contextus*  ctx,
    Arbor2Lexema*         tok_perge,
    Arbor2Lexema*         tok_semi)
{
    Arbor2Nodus* nodus;

    nodus = (Arbor2Nodus*)piscina_allocare(ctx->piscina,
        (memoriae_index)magnitudo(Arbor2Nodus));
    memset(nodus, ZEPHYRUM, magnitudo(Arbor2Nodus));

    nodus->genus = ARBOR2_NODUS_PERGE;
    nodus->lexema = _token_ex_lexema(ctx, tok_perge);
    nodus->datum.pergendum.tok_perge = nodus->lexema;
    nodus->datum.pergendum.tok_semicolon = _token_ex_lexema(ctx, tok_semi);

    nodus->linea_initium = tok_perge->linea;
    nodus->columna_initium = tok_perge->columna;
    nodus->linea_finis = tok_semi->linea;
    nodus->columna_finis = tok_semi->columna + tok_semi->longitudo;

    redde nodus;
}

/* Creare nodum SENTENTIA (expression statement) */
hic_manens Arbor2Nodus*
_nodus_sententia_expr(
    LapifexC89Contextus*  ctx,
    Arbor2Nodus*          expressio,
    Arbor2Lexema*         tok_semi)
{
    Arbor2Nodus* nodus;

    nodus = (Arbor2Nodus*)piscina_allocare(ctx->piscina,
        (memoriae_index)magnitudo(Arbor2Nodus));
    memset(nodus, ZEPHYRUM, magnitudo(Arbor2Nodus));

    nodus->genus = ARBOR2_NODUS_SENTENTIA;
    nodus->lexema = expressio->lexema;
    nodus->datum.sententia.expressio = expressio;
    nodus->datum.sententia.tok_semicolon = _token_ex_lexema(ctx, tok_semi);

    expressio->pater = nodus;

    nodus->linea_initium = expressio->linea_initium;
    nodus->columna_initium = expressio->columna_initium;
    nodus->linea_finis = tok_semi->linea;
    nodus->columna_finis = tok_semi->columna + tok_semi->longitudo;

    redde nodus;
}

/* Creare nodum SENTENTIA_VACUA (empty statement) */
hic_manens Arbor2Nodus*
_nodus_sententia_vacua(
    LapifexC89Contextus*  ctx,
    Arbor2Lexema*         tok_semi)
{
    Arbor2Nodus* nodus;

    nodus = (Arbor2Nodus*)piscina_allocare(ctx->piscina,
        (memoriae_index)magnitudo(Arbor2Nodus));
    memset(nodus, ZEPHYRUM, magnitudo(Arbor2Nodus));

    nodus->genus = ARBOR2_NODUS_SENTENTIA_VACUA;
    nodus->lexema = _token_ex_lexema(ctx, tok_semi);

    nodus->linea_initium = tok_semi->linea;
    nodus->columna_initium = tok_semi->columna;
    nodus->linea_finis = tok_semi->linea;
    nodus->columna_finis = tok_semi->columna + tok_semi->longitudo;

    redde nodus;
}

/* Creare nodum CORPUS (compound statement) */
hic_manens Arbor2Nodus*
_nodus_corpus(
    LapifexC89Contextus*  ctx,
    Arbor2Lexema*         tok_ba,
    Arbor2Nodus*          elenchus,
    Arbor2Lexema*         tok_bc)
{
    Arbor2Nodus* nodus;

    nodus = (Arbor2Nodus*)piscina_allocare(ctx->piscina,
        (memoriae_index)magnitudo(Arbor2Nodus));
    memset(nodus, ZEPHYRUM, magnitudo(Arbor2Nodus));

    nodus->genus = ARBOR2_NODUS_CORPUS;
    nodus->lexema = _token_ex_lexema(ctx, tok_ba);
    nodus->datum.corpus.tok_brace_ap = nodus->lexema;
    nodus->datum.corpus.tok_brace_cl = _token_ex_lexema(ctx, tok_bc);

    si (elenchus)
    {
        si (elenchus->genus == (Arbor2NodusGenus)0xDEAD)
        {
            /* Elenchus venit de _nodus_corpus_append, Xar* in datum.corpus */
            nodus->datum.corpus.sententiae = elenchus->datum.corpus.sententiae;
        }
        alioquin
        {
            /* Singulare elementum (P191 passthrough), involvi in Xar */
            Arbor2Nodus** slot;
            Xar* xar_unum = xar_creare(ctx->piscina,
                (i32)magnitudo(Arbor2Nodus*));
            slot = (Arbor2Nodus**)xar_addere(xar_unum);
            *slot = elenchus;
            nodus->datum.corpus.sententiae = xar_unum;
        }
    }
    alioquin
    {
        nodus->datum.corpus.sententiae = NIHIL;
    }

    nodus->linea_initium = tok_ba->linea;
    nodus->columna_initium = tok_ba->columna;
    nodus->linea_finis = tok_bc->linea;
    nodus->columna_finis = tok_bc->columna + tok_bc->longitudo;

    redde nodus;
}

/* Accumulare elementa in elenchus corporis (ut Xar) */
hic_manens Arbor2Nodus*
_nodus_corpus_append(
    LapifexC89Contextus*  ctx,
    Arbor2Nodus*          elenchus_nodus,
    Arbor2Nodus*          elementum)
{
    /* elenchus_nodus is either a single item (first call) or a Xar* packed as Nodus* */
    Xar* xar_elenchus;

    /* Check if this is already a Xar (from previous _nodus_corpus_append) */
    /* Heuristic: si elenchus_nodus non est standard nodus genus, est Xar* */
    /* Realiter: primum elementum est nodus normalis, deinde Xar */
    /* Usamus approach simplex: primum call creat Xar, deinde appendit */

    /* Si hoc est primum call: elenchus_nodus est normalis nodus */
    si (elenchus_nodus->genus != (Arbor2NodusGenus)0xDEAD)
    {
        Arbor2Nodus** slot;
        xar_elenchus = xar_creare(ctx->piscina, (i32)magnitudo(Arbor2Nodus*));
        slot = (Arbor2Nodus**)xar_addere(xar_elenchus);
        *slot = elenchus_nodus;
        slot = (Arbor2Nodus**)xar_addere(xar_elenchus);
        *slot = elementum;
    }
    alioquin
    {
        /* Iam habemus Xar in wrapper, extrahere et addere */
        Arbor2Nodus** slot;
        xar_elenchus = elenchus_nodus->datum.corpus.sententiae;
        slot = (Arbor2Nodus**)xar_addere(xar_elenchus);
        *slot = elementum;
    }

    /* Redde Xar* codificatum ut Arbor2Nodus* cum marker */
    {
        /* Creamus wrapper nodum cum marcatore speciali */
        Arbor2Nodus* wrapper = (Arbor2Nodus*)piscina_allocare(ctx->piscina,
            (memoriae_index)magnitudo(Arbor2Nodus));
        memset(wrapper, ZEPHYRUM, magnitudo(Arbor2Nodus));
        wrapper->genus = (Arbor2NodusGenus)0xDEAD;
        /* Servare Xar* in folium.valor pointer regione */
        /* Sed chorda non est pointer... usamus datum campo directe */
        /* Realiter, melius est stare Xar* in unione */
        /* Usamus CORPUS nodum ipsum */
        wrapper->datum.corpus.sententiae = xar_elenchus;
        redde wrapper;
    }
}

/* Creare nodum DEFINITIO_FUNCTI (function definition) */
hic_manens Arbor2Nodus*
_nodus_definitio_functi(
    LapifexC89Contextus*  ctx,
    Arbor2Nodus*          specifiers,
    Arbor2Nodus*          declarator,
    Arbor2Nodus*          corpus)
{
    Arbor2Nodus* nodus;

    nodus = (Arbor2Nodus*)piscina_allocare(ctx->piscina,
        (memoriae_index)magnitudo(Arbor2Nodus));
    memset(nodus, ZEPHYRUM, magnitudo(Arbor2Nodus));

    nodus->genus = ARBOR2_NODUS_DEFINITIO_FUNCTI;
    nodus->lexema = specifiers->lexema;
    nodus->datum.definitio_functi.specifier = specifiers;
    nodus->datum.definitio_functi.declarator = declarator;
    nodus->datum.definitio_functi.corpus = corpus;

    /* Locatio: ab initio specifiers ad finem corporis */
    nodus->linea_initium = specifiers->linea_initium;
    nodus->columna_initium = specifiers->columna_initium;
    nodus->linea_finis = corpus->linea_finis;
    nodus->columna_finis = corpus->columna_finis;

    redde nodus;
}

/* (_nodus_translatio et _nodus_translatio_append remotae — translatio
 * nunc per ansam in translationem_parsare construitur) */

/* Creare nodum TITULATUM (labeled statement) */
hic_manens Arbor2Nodus*
_nodus_titulatum(
    LapifexC89Contextus*  ctx,
    Arbor2Lexema*         tok_label,
    Arbor2Lexema*         tok_colon,
    Arbor2Nodus*          stmt)
{
    Arbor2Nodus* nodus;

    nodus = (Arbor2Nodus*)piscina_allocare(ctx->piscina,
        (memoriae_index)magnitudo(Arbor2Nodus));
    memset(nodus, ZEPHYRUM, magnitudo(Arbor2Nodus));

    nodus->genus = ARBOR2_NODUS_TITULATUM;
    nodus->lexema = _token_ex_lexema(ctx, tok_label);
    nodus->datum.titulatum.tok_titulus = nodus->lexema;
    nodus->datum.titulatum.titulus = tok_label->valor;
    nodus->datum.titulatum.tok_colon = _token_ex_lexema(ctx, tok_colon);
    nodus->datum.titulatum.sententia = stmt;

    stmt->pater = nodus;

    nodus->linea_initium = tok_label->linea;
    nodus->columna_initium = tok_label->columna;
    nodus->linea_finis = stmt->linea_finis;
    nodus->columna_finis = stmt->columna_finis;

    redde nodus;
}

/* Creare nodum CASUS (case label) */
hic_manens Arbor2Nodus*
_nodus_casus(
    LapifexC89Contextus*  ctx,
    Arbor2Lexema*         tok_casus,
    Arbor2Nodus*          valor,
    Arbor2Lexema*         tok_colon,
    Arbor2Nodus*          stmt)
{
    Arbor2Nodus* nodus;

    nodus = (Arbor2Nodus*)piscina_allocare(ctx->piscina,
        (memoriae_index)magnitudo(Arbor2Nodus));
    memset(nodus, ZEPHYRUM, magnitudo(Arbor2Nodus));

    nodus->genus = ARBOR2_NODUS_CASUS;
    nodus->lexema = _token_ex_lexema(ctx, tok_casus);
    nodus->datum.electio.tok_casus = nodus->lexema;
    nodus->datum.electio.valor = valor;
    nodus->datum.electio.tok_colon = _token_ex_lexema(ctx, tok_colon);
    nodus->datum.electio.sententia = stmt;

    valor->pater = nodus;
    stmt->pater = nodus;

    nodus->linea_initium = tok_casus->linea;
    nodus->columna_initium = tok_casus->columna;
    nodus->linea_finis = stmt->linea_finis;
    nodus->columna_finis = stmt->columna_finis;

    redde nodus;
}

/* Creare nodum ORDINARIUS (default label) */
hic_manens Arbor2Nodus*
_nodus_ordinarius(
    LapifexC89Contextus*  ctx,
    Arbor2Lexema*         tok_ordinarius,
    Arbor2Lexema*         tok_colon,
    Arbor2Nodus*          stmt)
{
    Arbor2Nodus* nodus;

    nodus = (Arbor2Nodus*)piscina_allocare(ctx->piscina,
        (memoriae_index)magnitudo(Arbor2Nodus));
    memset(nodus, ZEPHYRUM, magnitudo(Arbor2Nodus));

    nodus->genus = ARBOR2_NODUS_ORDINARIUS;
    nodus->lexema = _token_ex_lexema(ctx, tok_ordinarius);
    nodus->datum.defectus.tok_ordinarius = nodus->lexema;
    nodus->datum.defectus.tok_colon = _token_ex_lexema(ctx, tok_colon);
    nodus->datum.defectus.sententia = stmt;

    stmt->pater = nodus;

    nodus->linea_initium = tok_ordinarius->linea;
    nodus->columna_initium = tok_ordinarius->columna;
    nodus->linea_finis = stmt->linea_finis;
    nodus->columna_finis = stmt->columna_finis;

    redde nodus;
}

/* Creare PerClausula (for-loop clausulae) */
hic_manens Arbor2Nodus*
_per_clausula_creare(
    LapifexC89Contextus*  ctx,
    Arbor2Nodus*          initium,
    Arbor2Lexema*         tok_s1,
    Arbor2Nodus*          conditio,
    Arbor2Lexema*         tok_s2,
    Arbor2Nodus*          incrementum)
{
    PerClausula* clausula;

    clausula = (PerClausula*)piscina_allocare(ctx->piscina,
        (memoriae_index)magnitudo(PerClausula));
    clausula->initium = initium;
    clausula->tok_semicolon1 = tok_s1;
    clausula->conditio = conditio;
    clausula->tok_semicolon2 = tok_s2;
    clausula->incrementum = incrementum;

    /* Codificare PerClausula* ut Arbor2Nodus* (reinterpretare) */
    redde (Arbor2Nodus*)(vacuum*)clausula;
}

/* ================================================
 * Callback Reductionis
 *
 * Numeratio productionum:
 * P0:  virga -> assignatio
 * P1:  virga -> virga COMMA assignatio
 * P2:  assignatio -> ternarius
 * P3:  assignatio -> ternarius ASSIGNATIO assignatio
 * P4:  assignatio -> ternarius PLUS_ASSIGN assignatio
 * P5:  assignatio -> ternarius MINUS_ASSIGN assignatio
 * P6:  assignatio -> ternarius MULT_ASSIGN assignatio
 * P7:  assignatio -> ternarius DIV_ASSIGN assignatio
 * P8:  assignatio -> ternarius MOD_ASSIGN assignatio
 * P9:  assignatio -> ternarius AND_ASSIGN assignatio
 * P10: assignatio -> ternarius OR_ASSIGN assignatio
 * P11: assignatio -> ternarius XOR_ASSIGN assignatio
 * P12: assignatio -> ternarius SHL_ASSIGN assignatio
 * P13: assignatio -> ternarius SHR_ASSIGN assignatio
 * P14: ternarius -> disiunctio
 * P15: ternarius -> disiunctio QUAESTIO virga COLON ternarius
 * P16: disiunctio -> coniunctio
 * P17: disiunctio -> disiunctio DUPIPA coniunctio
 * P18: coniunctio -> pipa_bw
 * P19: coniunctio -> coniunctio DUAMPERSAND pipa_bw
 * P20: pipa_bw -> caret_bw
 * P21: pipa_bw -> pipa_bw PIPA caret_bw
 * P22: caret_bw -> amp_bw
 * P23: caret_bw -> caret_bw CARET amp_bw
 * P24: amp_bw -> aequalitas
 * P25: amp_bw -> amp_bw AMPERSAND aequalitas
 * P26: aequalitas -> comparatio
 * P27: aequalitas -> aequalitas AEQUALIS comparatio
 * P28: aequalitas -> aequalitas NON_AEQUALIS comparatio
 * P29: comparatio -> translatio
 * P30: comparatio -> comparatio MINOR translatio
 * P31: comparatio -> comparatio MAIOR translatio
 * P32: comparatio -> comparatio MINOR_AEQ translatio
 * P33: comparatio -> comparatio MAIOR_AEQ translatio
 * P34: translatio -> expressio
 * P35: translatio -> translatio SINISTRUM expressio
 * P36: translatio -> translatio DEXTRUM expressio
 * P37: expressio -> terminus
 * P38: expressio -> expressio PLUS terminus
 * P39: expressio -> expressio MINUS terminus
 * P40: terminus -> factor
 * P41: terminus -> terminus ASTERISCUS factor
 * P42: terminus -> terminus SOLIDUS factor
 * P43: terminus -> terminus PERCENTUM factor
 * P44: factor -> postfixum
 * P45: factor -> MINUS factor
 * P46: factor -> PLUS factor
 * P47: factor -> TILDE factor
 * P48: factor -> EXCLAMATIO factor
 * P49: factor -> AMPERSAND factor
 * P50: factor -> ASTERISCUS factor
 * P51: factor -> DUPLUS factor
 * P52: factor -> DUMINUS factor
 * P53: factor -> SIZEOF factor
 * P54: factor -> SIZEOF PAREN_APERTA nomen_typi PAREN_CLAUSA
 * P55: factor -> PAREN_APERTA nomen_typi PAREN_CLAUSA factor  (cast)
 * P56: postfixum -> primarium
 * P57: postfixum -> postfixum PAREN_APERTA PAREN_CLAUSA
 * P58: postfixum -> postfixum PAREN_APERTA argumenta PAREN_CLAUSA
 * P59: postfixum -> postfixum BRACKET_APERTA virga BRACKET_CLAUSA
 * P60: postfixum -> postfixum PUNCTUM IDENTIFICATOR
 * P61: postfixum -> postfixum SAGITTA IDENTIFICATOR
 * P62: postfixum -> postfixum DUPLUS
 * P63: postfixum -> postfixum DUMINUS
 * P64: primarium -> IDENTIFICATOR
 * P65: primarium -> INTEGER
 * P66: primarium -> FLOAT_LIT
 * P67: primarium -> CHAR_LIT
 * P68: primarium -> STRING_LIT
 * P69: primarium -> PAREN_APERTA virga PAREN_CLAUSA
 * P70: argumenta -> assignatio
 * P71: argumenta -> argumenta COMMA assignatio
 * P72: nomen_typi -> specifier_lista
 * P73: nomen_typi -> specifier_lista pointer
 * P74: specifier_lista -> specifier_singulum
 * P75: specifier_lista -> specifier_singulum specifier_lista
 * P76: specifier_singulum -> VOID
 * P77: specifier_singulum -> CHAR
 * P78: specifier_singulum -> SHORT
 * P79: specifier_singulum -> INT
 * P80: specifier_singulum -> LONG
 * P81: specifier_singulum -> FLOAT
 * P82: specifier_singulum -> DOUBLE
 * P83: specifier_singulum -> SIGNED
 * P84: specifier_singulum -> UNSIGNED
 * P85: specifier_singulum -> CONST
 * P86: specifier_singulum -> VOLATILE
 * P87: specifier_singulum -> STRUCT IDENTIFICATOR
 * P88: specifier_singulum -> UNION IDENTIFICATOR
 * P89: specifier_singulum -> ENUM IDENTIFICATOR
 * P90: pointer -> ASTERISCUS
 * P91: pointer -> ASTERISCUS pointer
 * P92: pointer -> ASTERISCUS CONST
 * P93: pointer -> ASTERISCUS CONST pointer
 *
 * ---- Declarationes ----
 * P94:  summum -> declaratio
 * P95:  summum -> virga
 * P96:  declaratio -> decl_specifiers init_declarator_lista SEMICOLON
 * P97:  declaratio -> decl_specifiers SEMICOLON
 * P98:  decl_specifiers -> decl_specifier
 * P99:  decl_specifiers -> decl_specifiers decl_specifier
 * P100: decl_specifier -> AUTO
 * P101: decl_specifier -> REGISTER
 * P102: decl_specifier -> STATIC
 * P103: decl_specifier -> EXTERN
 * P104: decl_specifier -> TYPEDEF
 * P105: decl_specifier -> CONST
 * P106: decl_specifier -> VOLATILE
 * P107: decl_specifier -> VOID
 * P108: decl_specifier -> CHAR
 * P109: decl_specifier -> SHORT
 * P110: decl_specifier -> INT
 * P111: decl_specifier -> LONG
 * P112: decl_specifier -> FLOAT
 * P113: decl_specifier -> DOUBLE
 * P114: decl_specifier -> SIGNED
 * P115: decl_specifier -> UNSIGNED
 * P116: decl_specifier -> struct_specifier
 * P117: decl_specifier -> enum_specifier
 * P118: init_declarator_lista -> init_declarator
 * P119: init_declarator_lista -> init_declarator_lista COMMA init_declarator
 * P120: init_declarator -> declarator
 * P121: init_declarator -> declarator ASSIGNATIO initializer
 * P122: declarator -> directus_declarator
 * P123: declarator -> pointer directus_declarator
 * P124: directus_declarator -> IDENTIFICATOR
 * P125: directus_declarator -> PAREN_APERTA declarator PAREN_CLAUSA
 * P126: directus_declarator -> directus_declarator BRACKET_APERTA BRACKET_CLAUSA
 * P127: directus_declarator -> directus_declarator BRACKET_APERTA virga BRACKET_CLAUSA
 * P128: directus_declarator -> directus_declarator PAREN_APERTA PAREN_CLAUSA
 * P129: directus_declarator -> directus_declarator PAREN_APERTA parameter_lista PAREN_CLAUSA
 * P130: directus_declarator -> directus_declarator PAREN_APERTA VOID PAREN_CLAUSA
 * P131: struct_specifier -> STRUCT IDENTIFICATOR BRACE_APERTA struct_member_lista BRACE_CLAUSA
 * P132: struct_specifier -> STRUCT BRACE_APERTA struct_member_lista BRACE_CLAUSA
 * P133: struct_specifier -> STRUCT IDENTIFICATOR
 * P134: struct_specifier -> UNION IDENTIFICATOR BRACE_APERTA struct_member_lista BRACE_CLAUSA
 * P135: struct_specifier -> UNION BRACE_APERTA struct_member_lista BRACE_CLAUSA
 * P136: struct_specifier -> UNION IDENTIFICATOR
 * P137: struct_member_lista -> struct_member
 * P138: struct_member_lista -> struct_member_lista struct_member
 * P139: struct_member -> decl_specifiers struct_declarator_lista SEMICOLON
 * P140: struct_declarator_lista -> struct_declarator
 * P141: struct_declarator_lista -> struct_declarator_lista COMMA struct_declarator
 * P142: struct_declarator -> declarator
 * P143: struct_declarator -> declarator COLON virga
 * P144: struct_declarator -> COLON virga
 * P145: enum_specifier -> ENUM IDENTIFICATOR BRACE_APERTA enumerator_lista BRACE_CLAUSA
 * P146: enum_specifier -> ENUM BRACE_APERTA enumerator_lista BRACE_CLAUSA
 * P147: enum_specifier -> ENUM IDENTIFICATOR
 * P148: enumerator_lista -> enumerator
 * P149: enumerator_lista -> enumerator_lista COMMA enumerator
 * P150: enumerator -> IDENTIFICATOR
 * P151: enumerator -> IDENTIFICATOR ASSIGNATIO virga
 * P152: parameter_lista -> parameter_decl
 * P153: parameter_lista -> parameter_lista COMMA parameter_decl
 * P154: parameter_lista -> parameter_lista COMMA ELLIPSIS
 * P155: parameter_decl -> decl_specifiers declarator
 * P156: parameter_decl -> decl_specifiers
 * P157: initializer -> assignatio
 * P158: initializer -> BRACE_APERTA init_items BRACE_CLAUSA
 * P159: initializer -> BRACE_APERTA init_items COMMA BRACE_CLAUSA
 * P160: init_items -> initializer
 * P161: init_items -> init_items COMMA initializer
 * ================================================ */

s64
lapifex_c89_expressio_reductio(
    s32           productio_index,
    constans s64* valori,
    s32           numerus,
    vacuum*       contextus)
{
    LapifexC89Contextus* ctx = (LapifexC89Contextus*)contextus;

    (vacuum)numerus;

    commutatio (productio_index)
    {
        /* ---- Virga (comma) ---- */
        casus ZEPHYRUM:  /* virga -> assignatio */
            redde valori[ZEPHYRUM];

        casus I:  /* virga -> virga COMMA assignatio */
            redde VALOR_EX(_nodus_lista_separata(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                NODUS_EX(valori[II])));

        /* ---- Assignatio ---- */
        casus II:  /* assignatio -> ternarius */
            redde valori[ZEPHYRUM];

        casus III:  /* ternarius ASSIGNATIO assignatio */
        casus IV:   /* ternarius PLUS_ASSIGN assignatio */
        casus V:    /* ternarius MINUS_ASSIGN assignatio */
        casus VI:   /* ternarius MULT_ASSIGN assignatio */
        casus VII:  /* ternarius DIV_ASSIGN assignatio */
        casus VIII: /* ternarius MOD_ASSIGN assignatio */
        casus IX:   /* ternarius AND_ASSIGN assignatio */
        casus X:    /* ternarius OR_ASSIGN assignatio */
        casus XI:   /* ternarius XOR_ASSIGN assignatio */
        casus XII:  /* ternarius SHL_ASSIGN assignatio */
        casus XIII: /* ternarius SHR_ASSIGN assignatio */
            redde VALOR_EX(_nodus_binarium(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                NODUS_EX(valori[II])));

        /* ---- Ternarius ---- */
        casus XIV:  /* ternarius -> disiunctio */
            redde valori[ZEPHYRUM];

        casus XV:  /* disiunctio QUAESTIO virga COLON ternarius */
            redde VALOR_EX(_nodus_ternarius(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                NODUS_EX(valori[II]),
                LEXEMA_EX(valori[III]),
                NODUS_EX(valori[IV])));

        /* ---- Disiunctio (||) ---- */
        casus XVI:  /* disiunctio -> coniunctio */
            redde valori[ZEPHYRUM];

        casus XVII:  /* disiunctio DUPIPA coniunctio */
            redde VALOR_EX(_nodus_binarium(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                NODUS_EX(valori[II])));

        /* ---- Coniunctio (&&) ---- */
        casus XVIII:  /* coniunctio -> pipa_bw */
            redde valori[ZEPHYRUM];

        casus XIX:  /* coniunctio DUAMPERSAND pipa_bw */
            redde VALOR_EX(_nodus_binarium(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                NODUS_EX(valori[II])));

        /* ---- Pipa BW (|) ---- */
        casus XX:  /* pipa_bw -> caret_bw */
            redde valori[ZEPHYRUM];

        casus XXI:  /* pipa_bw PIPA caret_bw */
            redde VALOR_EX(_nodus_binarium(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                NODUS_EX(valori[II])));

        /* ---- Caret BW (^) ---- */
        casus XXII:  /* caret_bw -> amp_bw */
            redde valori[ZEPHYRUM];

        casus XXIII:  /* caret_bw CARET amp_bw */
            redde VALOR_EX(_nodus_binarium(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                NODUS_EX(valori[II])));

        /* ---- Amp BW (&) ---- */
        casus XXIV:  /* amp_bw -> aequalitas */
            redde valori[ZEPHYRUM];

        casus XXV:  /* amp_bw AMPERSAND aequalitas */
            redde VALOR_EX(_nodus_binarium(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                NODUS_EX(valori[II])));

        /* ---- Aequalitas (== !=) ---- */
        casus XXVI:  /* aequalitas -> comparatio */
            redde valori[ZEPHYRUM];

        casus XXVII:  /* aequalitas AEQUALIS comparatio */
        casus XXVIII: /* aequalitas NON_AEQUALIS comparatio */
            redde VALOR_EX(_nodus_binarium(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                NODUS_EX(valori[II])));

        /* ---- Comparatio (< > <= >=) ---- */
        casus XXIX:  /* comparatio -> translatio */
            redde valori[ZEPHYRUM];

        casus XXX:   /* comparatio MINOR translatio */
        casus XXXI:  /* comparatio MAIOR translatio */
        casus XXXII: /* comparatio MINOR_AEQ translatio */
        casus XXXIII: /* comparatio MAIOR_AEQ translatio */
            redde VALOR_EX(_nodus_binarium(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                NODUS_EX(valori[II])));

        /* ---- Translatio (<< >>) ---- */
        casus XXXIV:  /* translatio -> expressio */
            redde valori[ZEPHYRUM];

        casus XXXV:  /* translatio SINISTRUM expressio */
        casus XXXVI: /* translatio DEXTRUM expressio */
            redde VALOR_EX(_nodus_binarium(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                NODUS_EX(valori[II])));

        /* ---- Expressio (+ -) ---- */
        casus XXXVII:  /* expressio -> terminus */
            redde valori[ZEPHYRUM];

        casus XXXVIII: /* expressio PLUS terminus */
        casus XXXIX:   /* expressio MINUS terminus */
            redde VALOR_EX(_nodus_binarium(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                NODUS_EX(valori[II])));

        /* ---- Terminus (* / %) ---- */
        casus XL:  /* terminus -> factor */
            redde valori[ZEPHYRUM];

        casus XLI:   /* terminus ASTERISCUS factor */
        casus XLII:  /* terminus SOLIDUS factor */
        casus XLIII: /* terminus PERCENTUM factor */
            redde VALOR_EX(_nodus_binarium(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                NODUS_EX(valori[II])));

        /* ---- Factor (unary, cast, sizeof) ---- */
        casus XLIV:  /* factor -> postfixum */
            redde valori[ZEPHYRUM];

        casus XLV:    /* MINUS factor */
        casus XLVI:   /* PLUS factor */
        casus XLVII:  /* TILDE factor */
        casus XLVIII: /* EXCLAMATIO factor */
        casus XLIX:   /* AMPERSAND factor */
        casus L:      /* ASTERISCUS factor */
        casus LI:     /* DUPLUS factor */
        casus LII:    /* DUMINUS factor */
            redde VALOR_EX(_nodus_unarium(ctx,
                LEXEMA_EX(valori[ZEPHYRUM]),
                NODUS_EX(valori[I])));

        casus LIII:  /* SIZEOF factor */
            redde VALOR_EX(_nodus_sizeof_expr(ctx,
                LEXEMA_EX(valori[ZEPHYRUM]),
                NODUS_EX(valori[I])));

        casus LIV:  /* SIZEOF PAREN_APERTA nomen_typi PAREN_CLAUSA */
            redde VALOR_EX(_nodus_sizeof_typus(ctx,
                LEXEMA_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                NODUS_EX(valori[II]),
                LEXEMA_EX(valori[III])));

        casus LV:  /* PAREN_APERTA nomen_typi PAREN_CLAUSA factor  (cast) */
            redde VALOR_EX(_nodus_conversio(ctx,
                LEXEMA_EX(valori[ZEPHYRUM]),
                NODUS_EX(valori[I]),
                LEXEMA_EX(valori[II]),
                NODUS_EX(valori[III])));

        /* ---- Postfixum ---- */
        casus LVI:  /* postfixum -> primarium */
            redde valori[ZEPHYRUM];

        casus LVII:  /* postfixum PAREN_APERTA PAREN_CLAUSA (empty call) */
            redde VALOR_EX(_nodus_vocatio(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                NIHIL,
                LEXEMA_EX(valori[II])));

        casus LVIII:  /* postfixum PAREN_APERTA argumenta PAREN_CLAUSA */
            redde VALOR_EX(_nodus_vocatio(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                NODUS_EX(valori[II]),
                LEXEMA_EX(valori[III])));

        casus LIX:  /* postfixum BRACKET_APERTA virga BRACKET_CLAUSA */
            redde VALOR_EX(_nodus_subscriptio(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                NODUS_EX(valori[II]),
                LEXEMA_EX(valori[III])));

        casus LX:  /* postfixum PUNCTUM IDENTIFICATOR */
            redde VALOR_EX(_nodus_membrum(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                LEXEMA_EX(valori[II]),
                FALSUM));

        casus LXI:  /* postfixum SAGITTA IDENTIFICATOR */
            redde VALOR_EX(_nodus_membrum(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                LEXEMA_EX(valori[II]),
                VERUM));

        casus LXII:  /* postfixum DUPLUS */
            redde VALOR_EX(_nodus_post_unarium(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I])));

        casus LXIII:  /* postfixum DUMINUS */
            redde VALOR_EX(_nodus_post_unarium(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I])));

        /* ---- Primarium ---- */
        casus LXIV:  /* IDENTIFICATOR */
            redde VALOR_EX(_nodus_folium(ctx,
                ARBOR2_NODUS_IDENTIFICATOR,
                LEXEMA_EX(valori[ZEPHYRUM])));

        casus LXV:  /* INTEGER */
            redde VALOR_EX(_nodus_folium(ctx,
                ARBOR2_NODUS_INTEGER,
                LEXEMA_EX(valori[ZEPHYRUM])));

        casus LXVI:  /* FLOAT_LIT */
            redde VALOR_EX(_nodus_folium(ctx,
                ARBOR2_NODUS_FLOAT,
                LEXEMA_EX(valori[ZEPHYRUM])));

        casus LXVII:  /* CHAR_LIT */
            redde VALOR_EX(_nodus_folium(ctx,
                ARBOR2_NODUS_CHAR,
                LEXEMA_EX(valori[ZEPHYRUM])));

        casus LXVIII:  /* STRING_LIT */
            redde VALOR_EX(_nodus_folium(ctx,
                ARBOR2_NODUS_STRING,
                LEXEMA_EX(valori[ZEPHYRUM])));

        casus LXIX:  /* PAREN_APERTA virga PAREN_CLAUSA */
            redde VALOR_EX(_nodus_parenthesized(ctx,
                LEXEMA_EX(valori[ZEPHYRUM]),
                NODUS_EX(valori[I]),
                LEXEMA_EX(valori[II])));

        /* ---- Argumenta ---- */
        casus LXX:  /* argumenta -> assignatio */
            redde valori[ZEPHYRUM];

        casus LXXI:  /* argumenta COMMA assignatio */
            redde VALOR_EX(_nodus_lista_separata(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                NODUS_EX(valori[II])));

        /* ---- Nomen Typi ---- */
        casus LXXII:  /* nomen_typi -> specifier_lista */
            redde valori[ZEPHYRUM];

        casus LXXIII:  /* nomen_typi -> specifier_lista pointer */
            /* Pro simplicitate, ignorare pointer info in type name nodo */
            redde valori[ZEPHYRUM];

        /* ---- Specifier Lista ---- */
        casus LXXIV:  /* specifier_lista -> specifier_singulum */
            redde valori[ZEPHYRUM];

        casus LXXV:  /* specifier_lista -> specifier_singulum specifier_lista */
            redde VALOR_EX(_nodus_specifier_lista(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                NODUS_EX(valori[I])));

        /* ---- Specifier Singulum (keyword types) ---- */
        casus LXXVI:    /* VOID */
        casus LXXVII:   /* CHAR */
        casus LXXVIII:  /* SHORT */
        casus LXXIX:    /* INT */
        casus LXXX:     /* LONG */
        casus LXXXI:    /* FLOAT */
        casus LXXXII:   /* DOUBLE */
        casus LXXXIII:  /* SIGNED */
        casus LXXXIV:   /* UNSIGNED */
        casus LXXXV:    /* CONST */
        casus LXXXVI:   /* VOLATILE */
            redde VALOR_EX(_nodus_specifier_folium(ctx,
                LEXEMA_EX(valori[ZEPHYRUM])));

        casus LXXXVII:  /* STRUCT IDENTIFICATOR */
        casus LXXXVIII: /* UNION IDENTIFICATOR */
        casus LXXXIX:   /* ENUM IDENTIFICATOR */
            /* Creare folium ex tag nomine (secundum token) */
            redde VALOR_EX(_nodus_specifier_folium(ctx,
                LEXEMA_EX(valori[I])));

        /* ---- Pointer ---- */
        casus XC:    /* pointer -> ASTERISCUS */
            redde VALOR_EX(_pointer_chain_creare(ctx,
                LEXEMA_EX(valori[ZEPHYRUM]),
                FALSUM, NIHIL, NIHIL));

        casus XCI:   /* pointer -> ASTERISCUS pointer */
            redde VALOR_EX(_pointer_chain_creare(ctx,
                LEXEMA_EX(valori[ZEPHYRUM]),
                FALSUM, NIHIL,
                (Xar*)(vacuum*)(longus)(valori[I])));

        casus XCII:  /* pointer -> ASTERISCUS CONST */
            redde VALOR_EX(_pointer_chain_creare(ctx,
                LEXEMA_EX(valori[ZEPHYRUM]),
                VERUM, LEXEMA_EX(valori[I]), NIHIL));

        casus XCIII: /* pointer -> ASTERISCUS CONST pointer */
            redde VALOR_EX(_pointer_chain_creare(ctx,
                LEXEMA_EX(valori[ZEPHYRUM]),
                VERUM, LEXEMA_EX(valori[I]),
                (Xar*)(vacuum*)(longus)(valori[II])));

        /* ---- Summum (P94-P96) ---- */
        casus XCIV:  /* summum -> declaratio */
            redde valori[ZEPHYRUM];

        casus XCV:   /* summum -> virga */
            redde valori[ZEPHYRUM];

        casus XCVI:  /* summum -> sententia */
            redde valori[ZEPHYRUM];

        /* ---- Declaratio (P97-P98) ---- */
        casus XCVII:  /* declaratio -> decl_specifiers init_declarator_lista SEMICOLON */
            redde VALOR_EX(_nodus_declaratio(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                NODUS_EX(valori[I]),
                LEXEMA_EX(valori[II])));

        casus XCVIII: /* declaratio -> decl_specifiers SEMICOLON */
            redde VALOR_EX(_nodus_declaratio_vacua(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I])));

        /* ---- Decl Specifiers (P99-P100) ---- */
        casus XCIX:   /* decl_specifiers -> decl_specifier */
            redde valori[ZEPHYRUM];

        casus C:      /* decl_specifiers -> decl_specifiers decl_specifier */
            redde VALOR_EX(_nodus_specifier_chain(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                NODUS_EX(valori[I])));

        /* ---- Decl Specifier keywords (P101-P116) ---- */
        casus CI:      /* AUTO */
        casus CII:     /* REGISTER */
        casus CIII:    /* STATIC */
        casus CIV:     /* EXTERN */
        casus CV:      /* TYPEDEF */
        casus CVI:     /* CONST */
        casus CVII:    /* VOLATILE */
        casus CVIII:   /* VOID */
        casus CIX:     /* CHAR */
        casus CX:      /* SHORT */
        casus CXI:     /* INT */
        casus CXII:    /* LONG */
        casus CXIII:   /* FLOAT */
        casus CXIV:    /* DOUBLE */
        casus CXV:     /* SIGNED */
        casus CXVI:    /* UNSIGNED */
            redde VALOR_EX(_nodus_specifier_folium(ctx,
                LEXEMA_EX(valori[ZEPHYRUM])));

        /* ---- Decl Specifier struct/enum (P117-P118) ---- */
        casus CXVII:   /* struct_specifier */
        casus CXVIII:  /* enum_specifier */
            redde valori[ZEPHYRUM];

        /* ---- Init Declarator Lista (P119-P120) ---- */
        casus CXIX:   /* init_declarator_lista -> init_declarator */
            redde valori[ZEPHYRUM];

        casus CXX:    /* init_declarator_lista -> init_declarator_lista COMMA init_declarator */
            redde VALOR_EX(_nodus_lista_separata(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                NODUS_EX(valori[II])));

        /* ---- Init Declarator (P121-P122) ---- */
        casus CXXI:   /* init_declarator -> declarator */
            redde valori[ZEPHYRUM];

        casus CXXII:  /* init_declarator -> declarator ASSIGNATIO initializer */
            redde VALOR_EX(_nodus_init_declarator(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                NODUS_EX(valori[II])));

        /* ---- Declarator (P123-P124) ---- */
        casus CXXIII:  /* declarator -> directus_declarator */
            /* Impaccare in DECLARATOR nodum si nondum est */
        {
            Arbor2Nodus* dir = NODUS_EX(valori[ZEPHYRUM]);
            si (dir->genus == ARBOR2_NODUS_DECLARATOR
                || dir->genus == ARBOR2_NODUS_DECLARATOR_FUNCTI)
                redde valori[ZEPHYRUM];
            redde VALOR_EX(_nodus_declarator(ctx, NIHIL, dir));
        }

        casus CXXIV:  /* declarator -> pointer directus_declarator */
        {
            Xar* ptr = (Xar*)(vacuum*)(longus)(valori[ZEPHYRUM]);
            Arbor2Nodus* dir = NODUS_EX(valori[I]);
            si (dir->genus == ARBOR2_NODUS_DECLARATOR)
            {
                dir->datum.declarator.pointer_levels = ptr;
                redde valori[I];
            }
            si (dir->genus == ARBOR2_NODUS_DECLARATOR_FUNCTI)
            {
                dir->datum.declarator_functi.pointer_levels = ptr;
                redde valori[I];
            }
            redde VALOR_EX(_nodus_declarator(ctx, ptr, dir));
        }

        /* ---- Directus Declarator (P125-P131) ---- */
        casus CXXV:   /* directus_declarator -> IDENTIFICATOR */
        {
            Arbor2Nodus* folium = _nodus_folium(ctx,
                ARBOR2_NODUS_IDENTIFICATOR,
                LEXEMA_EX(valori[ZEPHYRUM]));
            redde VALOR_EX(_nodus_declarator(ctx, NIHIL, folium));
        }

        casus CXXVI:  /* directus_declarator -> PAREN_APERTA declarator PAREN_CLAUSA */
            /* Grouped declarator: (*fp) - just pass through the inner declarator */
            redde valori[I];

        casus CXXVII: /* directus_declarator -> directus_declarator BRACKET_APERTA BRACKET_CLAUSA */
            redde VALOR_EX(_nodus_array_dim(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                NIHIL,
                LEXEMA_EX(valori[II])));

        casus CXXVIII: /* directus_declarator -> directus_declarator BRACKET_APERTA virga BRACKET_CLAUSA */
            redde VALOR_EX(_nodus_array_dim(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                NODUS_EX(valori[II]),
                LEXEMA_EX(valori[III])));

        casus CXXIX:  /* directus_declarator -> directus_declarator PAREN_APERTA PAREN_CLAUSA */
            redde VALOR_EX(_nodus_declarator_functi(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                NIHIL,
                LEXEMA_EX(valori[II]),
                FALSUM, FALSUM));

        casus CXXX:   /* directus_declarator -> directus_declarator PAREN_APERTA parameter_lista PAREN_CLAUSA */
        {
            Arbor2Nodus* params = NODUS_EX(valori[II]);
            b32 variadicus = FALSUM;
            si (params->genus == ARBOR2_NODUS_PARAMETER_LIST)
                variadicus = params->datum.parameter_list.est_variadicus;
            redde VALOR_EX(_nodus_declarator_functi(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                params,
                LEXEMA_EX(valori[III]),
                FALSUM, variadicus));
        }

        casus CXXXI:  /* directus_declarator -> directus_declarator PAREN_APERTA VOID PAREN_CLAUSA */
            redde VALOR_EX(_nodus_declarator_functi(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                NIHIL,
                LEXEMA_EX(valori[III]),
                VERUM, FALSUM));

        /* ---- Struct Specifier (P132-P137) ---- */
        casus CXXXII:  /* STRUCT IDENTIFICATOR BRACE_APERTA struct_member_lista BRACE_CLAUSA */
            redde VALOR_EX(_nodus_struct_specifier(ctx,
                LEXEMA_EX(valori[ZEPHYRUM]),
                _nodus_folium(ctx, ARBOR2_NODUS_IDENTIFICATOR, LEXEMA_EX(valori[I])),
                LEXEMA_EX(valori[II]),
                NODUS_EX(valori[III]),
                LEXEMA_EX(valori[IV]),
                FALSUM));

        casus CXXXIII: /* STRUCT BRACE_APERTA struct_member_lista BRACE_CLAUSA */
            redde VALOR_EX(_nodus_struct_specifier(ctx,
                LEXEMA_EX(valori[ZEPHYRUM]),
                NIHIL,
                LEXEMA_EX(valori[I]),
                NODUS_EX(valori[II]),
                LEXEMA_EX(valori[III]),
                FALSUM));

        casus CXXXIV:  /* STRUCT IDENTIFICATOR (forward ref) */
            redde VALOR_EX(_nodus_struct_specifier(ctx,
                LEXEMA_EX(valori[ZEPHYRUM]),
                _nodus_folium(ctx, ARBOR2_NODUS_IDENTIFICATOR, LEXEMA_EX(valori[I])),
                NIHIL, NIHIL, NIHIL,
                FALSUM));

        casus CXXXV:  /* UNION IDENTIFICATOR BRACE_APERTA struct_member_lista BRACE_CLAUSA */
            redde VALOR_EX(_nodus_struct_specifier(ctx,
                LEXEMA_EX(valori[ZEPHYRUM]),
                _nodus_folium(ctx, ARBOR2_NODUS_IDENTIFICATOR, LEXEMA_EX(valori[I])),
                LEXEMA_EX(valori[II]),
                NODUS_EX(valori[III]),
                LEXEMA_EX(valori[IV]),
                VERUM));

        casus CXXXVI: /* UNION BRACE_APERTA struct_member_lista BRACE_CLAUSA */
            redde VALOR_EX(_nodus_struct_specifier(ctx,
                LEXEMA_EX(valori[ZEPHYRUM]),
                NIHIL,
                LEXEMA_EX(valori[I]),
                NODUS_EX(valori[II]),
                LEXEMA_EX(valori[III]),
                VERUM));

        casus CXXXVII: /* UNION IDENTIFICATOR (forward ref) */
            redde VALOR_EX(_nodus_struct_specifier(ctx,
                LEXEMA_EX(valori[ZEPHYRUM]),
                _nodus_folium(ctx, ARBOR2_NODUS_IDENTIFICATOR, LEXEMA_EX(valori[I])),
                NIHIL, NIHIL, NIHIL,
                VERUM));

        /* ---- Struct Member Lista (P138-P139) ---- */
        casus CXXXVIII: /* struct_member_lista -> struct_member */
            redde valori[ZEPHYRUM];

        casus CXXXIX:   /* struct_member_lista -> struct_member_lista struct_member */
            redde VALOR_EX(_nodus_struct_member_append(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                NODUS_EX(valori[I])));

        /* ---- Struct Member (P140) ---- */
        casus CXL:    /* struct_member -> decl_specifiers struct_declarator_lista SEMICOLON */
            redde VALOR_EX(_nodus_struct_member(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                NODUS_EX(valori[I]),
                LEXEMA_EX(valori[II])));

        /* ---- Struct Declarator Lista (P141-P142) ---- */
        casus CXLI:   /* struct_declarator_lista -> struct_declarator */
            redde valori[ZEPHYRUM];

        casus CXLII:  /* struct_declarator_lista -> struct_declarator_lista COMMA struct_declarator */
            redde VALOR_EX(_nodus_lista_separata(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                NODUS_EX(valori[II])));

        /* ---- Struct Declarator (P143-P145) ---- */
        casus CXLIII: /* struct_declarator -> declarator */
            redde valori[ZEPHYRUM];

        casus CXLIV:  /* struct_declarator -> declarator COLON virga (bit field) */
            redde VALOR_EX(_nodus_bit_field(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                NODUS_EX(valori[II])));

        casus CXLV:   /* struct_declarator -> COLON virga (anonymous bit field) */
            redde VALOR_EX(_nodus_anon_bit_field(ctx,
                LEXEMA_EX(valori[ZEPHYRUM]),
                NODUS_EX(valori[I])));

        /* ---- Enum Specifier (P146-P148) ---- */
        casus CXLVI:  /* ENUM IDENTIFICATOR BRACE_APERTA enumerator_lista BRACE_CLAUSA */
            redde VALOR_EX(_nodus_enum_specifier(ctx,
                LEXEMA_EX(valori[ZEPHYRUM]),
                _nodus_folium(ctx, ARBOR2_NODUS_IDENTIFICATOR, LEXEMA_EX(valori[I])),
                LEXEMA_EX(valori[II]),
                NODUS_EX(valori[III]),
                LEXEMA_EX(valori[IV])));

        casus CXLVII: /* ENUM BRACE_APERTA enumerator_lista BRACE_CLAUSA */
            redde VALOR_EX(_nodus_enum_specifier(ctx,
                LEXEMA_EX(valori[ZEPHYRUM]),
                NIHIL,
                LEXEMA_EX(valori[I]),
                NODUS_EX(valori[II]),
                LEXEMA_EX(valori[III])));

        casus CXLVIII: /* ENUM IDENTIFICATOR (forward ref) */
            redde VALOR_EX(_nodus_enum_specifier(ctx,
                LEXEMA_EX(valori[ZEPHYRUM]),
                _nodus_folium(ctx, ARBOR2_NODUS_IDENTIFICATOR, LEXEMA_EX(valori[I])),
                NIHIL, NIHIL, NIHIL));

        /* ---- Enumerator Lista (P149-P150) ---- */
        casus CXLIX:  /* enumerator_lista -> enumerator */
            redde valori[ZEPHYRUM];

        casus CL:     /* enumerator_lista -> enumerator_lista COMMA enumerator */
            redde VALOR_EX(_nodus_lista_separata(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                NODUS_EX(valori[II])));

        /* ---- Enumerator (P151-P152) ---- */
        casus CLI:    /* enumerator -> IDENTIFICATOR */
            redde VALOR_EX(_nodus_enumerator(ctx,
                LEXEMA_EX(valori[ZEPHYRUM]),
                NIHIL, NIHIL));

        casus CLII:   /* enumerator -> IDENTIFICATOR ASSIGNATIO virga */
            redde VALOR_EX(_nodus_enumerator(ctx,
                LEXEMA_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                NODUS_EX(valori[II])));

        /* ---- Parameter Lista (P153-P155) ---- */
        casus CLIII:  /* parameter_lista -> parameter_decl */
            redde valori[ZEPHYRUM];

        casus CLIV:   /* parameter_lista -> parameter_lista COMMA parameter_decl */
            redde VALOR_EX(_nodus_lista_separata(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                NODUS_EX(valori[II])));

        casus CLV:    /* parameter_lista -> parameter_lista COMMA ELLIPSIS */
        {
            /* Marcare listam ut variadicam et addere ellipsis token */
            Arbor2Nodus* lista = NODUS_EX(valori[ZEPHYRUM]);
            Arbor2Nodus* nodus_param;

            /* Creare PARAMETER_LIST wrapper si nondum est */
            si (lista->genus != ARBOR2_NODUS_PARAMETER_LIST)
            {
                nodus_param = (Arbor2Nodus*)piscina_allocare(ctx->piscina,
                    (memoriae_index)magnitudo(Arbor2Nodus));
                memset(nodus_param, ZEPHYRUM, magnitudo(Arbor2Nodus));
                nodus_param->genus = ARBOR2_NODUS_PARAMETER_LIST;
                nodus_param->lexema = lista->lexema;
                nodus_param->datum.parameter_list.parametra =
                    xar_creare(ctx->piscina, (i32)magnitudo(Arbor2Nodus*));
                /* Copiere elementa ex lista */
                si (lista->genus == ARBOR2_NODUS_LISTA_SEPARATA)
                {
                    i32 i;
                    per (i = ZEPHYRUM; i < (i32)xar_numerus(lista->datum.lista_separata.elementa); i++)
                    {
                        Arbor2Nodus** src = (Arbor2Nodus**)xar_obtinere(
                            lista->datum.lista_separata.elementa, i);
                        Arbor2Nodus** dst = (Arbor2Nodus**)xar_addere(
                            nodus_param->datum.parameter_list.parametra);
                        *dst = *src;
                    }
                }
                alioquin
                {
                    Arbor2Nodus** dst = (Arbor2Nodus**)xar_addere(
                        nodus_param->datum.parameter_list.parametra);
                    *dst = lista;
                }
                nodus_param->linea_initium = lista->linea_initium;
                nodus_param->columna_initium = lista->columna_initium;
            }
            alioquin
            {
                nodus_param = lista;
            }
            nodus_param->datum.parameter_list.est_variadicus = VERUM;
            {
                Arbor2Lexema* tok_ell = LEXEMA_EX(valori[II]);
                nodus_param->linea_finis = tok_ell->linea;
                nodus_param->columna_finis = tok_ell->columna + tok_ell->longitudo;
            }
            redde VALOR_EX(nodus_param);
        }

        /* ---- Parameter Decl (P156-P157) ---- */
        casus CLVI:   /* parameter_decl -> decl_specifiers declarator */
            redde VALOR_EX(_nodus_parameter_decl(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                NODUS_EX(valori[I])));

        casus CLVII:  /* parameter_decl -> decl_specifiers (abstract) */
            redde VALOR_EX(_nodus_parameter_decl(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                NIHIL));

        /* ---- Initializer (P158-P160) ---- */
        casus CLVIII: /* initializer -> assignatio */
            redde valori[ZEPHYRUM];

        casus CLIX:   /* initializer -> BRACE_APERTA init_items BRACE_CLAUSA */
            redde VALOR_EX(_nodus_initializor_lista(ctx,
                LEXEMA_EX(valori[ZEPHYRUM]),
                NODUS_EX(valori[I]),
                LEXEMA_EX(valori[II])));

        casus CLX:    /* initializer -> BRACE_APERTA init_items COMMA BRACE_CLAUSA */
            redde VALOR_EX(_nodus_initializor_lista(ctx,
                LEXEMA_EX(valori[ZEPHYRUM]),
                NODUS_EX(valori[I]),
                LEXEMA_EX(valori[III])));

        /* ---- Init Items (P161-P162) ---- */
        casus CLXI:   /* init_items -> initializer */
            redde valori[ZEPHYRUM];

        casus CLXII:  /* init_items -> init_items COMMA initializer */
            redde VALOR_EX(_nodus_lista_separata(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                NODUS_EX(valori[II])));

        /* ========================================================
         * SENTENTIAE (P163-P202)
         * ======================================================== */

        /* ---- Sententia (P163-P164) ---- */
        casus CLXIII: /* sententia -> sententia_compar */
            redde valori[ZEPHYRUM];

        casus CLXIV:  /* sententia -> sententia_incompar */
            redde valori[ZEPHYRUM];

        /* ---- Sententia Compar (P165-P171) ---- */
        casus CLXV:   /* sententia_compar -> non_si_sententia */
            redde valori[ZEPHYRUM];

        casus CLXVI:  /* IF PAREN_APERTA virga PAREN_CLAUSA sententia_compar ELSE sententia_compar */
            redde VALOR_EX(_nodus_si(ctx,
                LEXEMA_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                NODUS_EX(valori[II]),
                LEXEMA_EX(valori[III]),
                NODUS_EX(valori[IV]),
                LEXEMA_EX(valori[V]),
                NODUS_EX(valori[VI])));

        casus CLXVII: /* WHILE PAREN_APERTA virga PAREN_CLAUSA sententia_compar */
            redde VALOR_EX(_nodus_dum(ctx,
                LEXEMA_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                NODUS_EX(valori[II]),
                LEXEMA_EX(valori[III]),
                NODUS_EX(valori[IV])));

        casus CLXVIII: /* FOR PAREN_APERTA per_clausula PAREN_CLAUSA sententia_compar */
            redde VALOR_EX(_nodus_per(ctx,
                LEXEMA_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                NODUS_EX(valori[II]),
                LEXEMA_EX(valori[III]),
                NODUS_EX(valori[IV])));

        casus CLXIX:  /* IDENTIFICATOR COLON sententia_compar */
            redde VALOR_EX(_nodus_titulatum(ctx,
                LEXEMA_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                NODUS_EX(valori[II])));

        casus CLXX:   /* CASE virga COLON sententia_compar */
            redde VALOR_EX(_nodus_casus(ctx,
                LEXEMA_EX(valori[ZEPHYRUM]),
                NODUS_EX(valori[I]),
                LEXEMA_EX(valori[II]),
                NODUS_EX(valori[III])));

        casus CLXXI:  /* DEFAULT COLON sententia_compar */
            redde VALOR_EX(_nodus_ordinarius(ctx,
                LEXEMA_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                NODUS_EX(valori[II])));

        /* ---- Sententia Incompar (P172-P178) ---- */
        casus CLXXII: /* IF PAREN_APERTA virga PAREN_CLAUSA sententia */
            redde VALOR_EX(_nodus_si(ctx,
                LEXEMA_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                NODUS_EX(valori[II]),
                LEXEMA_EX(valori[III]),
                NODUS_EX(valori[IV]),
                NIHIL,
                NIHIL));

        casus CLXXIII: /* IF PAREN_APERTA virga PAREN_CLAUSA sententia_compar ELSE sententia_incompar */
            redde VALOR_EX(_nodus_si(ctx,
                LEXEMA_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                NODUS_EX(valori[II]),
                LEXEMA_EX(valori[III]),
                NODUS_EX(valori[IV]),
                LEXEMA_EX(valori[V]),
                NODUS_EX(valori[VI])));

        casus CLXXIV: /* WHILE PAREN_APERTA virga PAREN_CLAUSA sententia_incompar */
            redde VALOR_EX(_nodus_dum(ctx,
                LEXEMA_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                NODUS_EX(valori[II]),
                LEXEMA_EX(valori[III]),
                NODUS_EX(valori[IV])));

        casus CLXXV:  /* FOR PAREN_APERTA per_clausula PAREN_CLAUSA sententia_incompar */
            redde VALOR_EX(_nodus_per(ctx,
                LEXEMA_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                NODUS_EX(valori[II]),
                LEXEMA_EX(valori[III]),
                NODUS_EX(valori[IV])));

        casus CLXXVI: /* IDENTIFICATOR COLON sententia_incompar */
            redde VALOR_EX(_nodus_titulatum(ctx,
                LEXEMA_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                NODUS_EX(valori[II])));

        casus CLXXVII: /* CASE virga COLON sententia_incompar */
            redde VALOR_EX(_nodus_casus(ctx,
                LEXEMA_EX(valori[ZEPHYRUM]),
                NODUS_EX(valori[I]),
                LEXEMA_EX(valori[II]),
                NODUS_EX(valori[III])));

        casus CLXXVIII: /* DEFAULT COLON sententia_incompar */
            redde VALOR_EX(_nodus_ordinarius(ctx,
                LEXEMA_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                NODUS_EX(valori[II])));

        /* ---- Non-Si Sententia (P179-P188) ---- */
        casus CLXXIX: /* non_si_sententia -> corpus */
            redde valori[ZEPHYRUM];

        casus CLXXX:  /* non_si_sententia -> virga SEMICOLON (expression stmt) */
            redde VALOR_EX(_nodus_sententia_expr(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I])));

        casus CLXXXI: /* non_si_sententia -> SEMICOLON (empty stmt) */
            redde VALOR_EX(_nodus_sententia_vacua(ctx,
                LEXEMA_EX(valori[ZEPHYRUM])));

        casus CLXXXII: /* DO sententia WHILE PAREN_APERTA virga PAREN_CLAUSA SEMICOLON */
            redde VALOR_EX(_nodus_fac_dum(ctx,
                LEXEMA_EX(valori[ZEPHYRUM]),
                NODUS_EX(valori[I]),
                LEXEMA_EX(valori[II]),
                LEXEMA_EX(valori[III]),
                NODUS_EX(valori[IV]),
                LEXEMA_EX(valori[V]),
                LEXEMA_EX(valori[VI])));

        casus CLXXXIII: /* SWITCH PAREN_APERTA virga PAREN_CLAUSA sententia */
            redde VALOR_EX(_nodus_commutatio(ctx,
                LEXEMA_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                NODUS_EX(valori[II]),
                LEXEMA_EX(valori[III]),
                NODUS_EX(valori[IV])));

        casus CLXXXIV: /* GOTO IDENTIFICATOR SEMICOLON */
            redde VALOR_EX(_nodus_salta(ctx,
                LEXEMA_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                LEXEMA_EX(valori[II])));

        casus CLXXXV:  /* CONTINUE SEMICOLON */
            redde VALOR_EX(_nodus_perge(ctx,
                LEXEMA_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I])));

        casus CLXXXVI: /* BREAK SEMICOLON */
            redde VALOR_EX(_nodus_frange(ctx,
                LEXEMA_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I])));

        casus CLXXXVII: /* RETURN SEMICOLON */
            redde VALOR_EX(_nodus_redde(ctx,
                LEXEMA_EX(valori[ZEPHYRUM]),
                NIHIL,
                LEXEMA_EX(valori[I])));

        casus CLXXXVIII: /* RETURN virga SEMICOLON */
            redde VALOR_EX(_nodus_redde(ctx,
                LEXEMA_EX(valori[ZEPHYRUM]),
                NODUS_EX(valori[I]),
                LEXEMA_EX(valori[II])));

        /* ---- Corpus (P189-P190) ---- */
        casus CLXXXIX: /* corpus -> BRACE_APERTA BRACE_CLAUSA */
            redde VALOR_EX(_nodus_corpus(ctx,
                LEXEMA_EX(valori[ZEPHYRUM]),
                NIHIL,
                LEXEMA_EX(valori[I])));

        casus CXC:    /* corpus -> BRACE_APERTA elenchus_corporis BRACE_CLAUSA */
            redde VALOR_EX(_nodus_corpus(ctx,
                LEXEMA_EX(valori[ZEPHYRUM]),
                NODUS_EX(valori[I]),
                LEXEMA_EX(valori[II])));

        /* ---- Elenchus Corporis (P191-P192) ---- */
        casus CXCI:   /* elenchus_corporis -> elementum_corporis */
            redde valori[ZEPHYRUM];

        casus CXCII:  /* elenchus_corporis -> elenchus_corporis elementum_corporis */
            redde VALOR_EX(_nodus_corpus_append(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                NODUS_EX(valori[I])));

        /* ---- Elementum Corporis (P193-P194) ---- */
        casus CXCIII: /* elementum_corporis -> declaratio */
            redde valori[ZEPHYRUM];

        casus CXCIV:  /* elementum_corporis -> sententia */
            redde valori[ZEPHYRUM];

        /* ---- Per Clausula (P195-P202) ---- */
        casus CXCV:   /* virga SEMICOLON virga SEMICOLON virga */
            redde VALOR_EX(_per_clausula_creare(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                NODUS_EX(valori[II]),
                LEXEMA_EX(valori[III]),
                NODUS_EX(valori[IV])));

        casus CXCVI:  /* virga SEMICOLON virga SEMICOLON */
            redde VALOR_EX(_per_clausula_creare(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                NODUS_EX(valori[II]),
                LEXEMA_EX(valori[III]),
                NIHIL));

        casus CXCVII: /* virga SEMICOLON SEMICOLON virga */
            redde VALOR_EX(_per_clausula_creare(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                NIHIL,
                LEXEMA_EX(valori[II]),
                NODUS_EX(valori[III])));

        casus CXCVIII: /* SEMICOLON virga SEMICOLON virga */
            redde VALOR_EX(_per_clausula_creare(ctx,
                NIHIL,
                LEXEMA_EX(valori[ZEPHYRUM]),
                NODUS_EX(valori[I]),
                LEXEMA_EX(valori[II]),
                NODUS_EX(valori[III])));

        casus CXCIX:  /* virga SEMICOLON SEMICOLON */
            redde VALOR_EX(_per_clausula_creare(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                LEXEMA_EX(valori[I]),
                NIHIL,
                LEXEMA_EX(valori[II]),
                NIHIL));

        casus CC:     /* SEMICOLON virga SEMICOLON */
            redde VALOR_EX(_per_clausula_creare(ctx,
                NIHIL,
                LEXEMA_EX(valori[ZEPHYRUM]),
                NODUS_EX(valori[I]),
                LEXEMA_EX(valori[II]),
                NIHIL));

        casus CCI:    /* SEMICOLON SEMICOLON virga */
            redde VALOR_EX(_per_clausula_creare(ctx,
                NIHIL,
                LEXEMA_EX(valori[ZEPHYRUM]),
                NIHIL,
                LEXEMA_EX(valori[I]),
                NODUS_EX(valori[II])));

        casus CCII:   /* SEMICOLON SEMICOLON */
            redde VALOR_EX(_per_clausula_creare(ctx,
                NIHIL,
                LEXEMA_EX(valori[ZEPHYRUM]),
                NIHIL,
                LEXEMA_EX(valori[I]),
                NIHIL));

        /* ---- Definitio Functi et NOMEN_TYPUS (P203-P206) ---- */

        casus CCIII:  /* declaratio -> decl_specifiers declarator corpus */
            redde VALOR_EX(_nodus_definitio_functi(ctx,
                NODUS_EX(valori[ZEPHYRUM]),
                NODUS_EX(valori[I]),
                NODUS_EX(valori[II])));

        casus CCIV:   /* decl_specifier -> NOMEN_TYPUS */
            /* Tractatur ut IDENTIFICATOR pro specifier creatione */
            redde VALOR_EX(_nodus_folium(ctx,
                ARBOR2_NODUS_IDENTIFICATOR,
                LEXEMA_EX(valori[ZEPHYRUM])));

        casus CCV:    /* specifier_singulum -> NOMEN_TYPUS (pro castis) */
            redde valori[ZEPHYRUM];

        casus CCVI:   /* directus_declarator -> NOMEN_TYPUS */
            /* Tractatur ut IDENTIFICATOR pro declarator creatione */
            redde VALOR_EX(_nodus_folium(ctx,
                ARBOR2_NODUS_IDENTIFICATOR,
                LEXEMA_EX(valori[ZEPHYRUM])));

        /* ---- Productio Augmentata ---- */
        ordinarius:
            /* __initium__ -> summum EOF */
            redde valori[ZEPHYRUM];
    }
}

/* ================================================
 * API: Obtinere grammaticam
 * ================================================ */

constans character*
lapifex_c89_expressio_grammatica(vacuum)
{
    redde GRAMMATICA_EXPRESSIO;
}

/* ================================================
 * API: Parsare expressionem
 * ================================================ */

/* Cache staticum pro grammatica et tabula (aedificatur semel) */
hic_manens Piscina*                 s_grammatica_piscina = NIHIL;
hic_manens InternamentumChorda*     s_grammatica_intern = NIHIL;
hic_manens LapifexGrammatica*       s_grammatica = NIHIL;
hic_manens LapifexTabula*           s_tabula = NIHIL;

hic_manens vacuum
_tabulam_parare(vacuum)
{
    LapifexCollectio* collectio;

    si (s_tabula) redde; /* Iam parata */

    s_grammatica_piscina = piscina_generare_dynamicum(
        "lapifex_c89_grammatica", (memoriae_index)(MMMMXCVI * XXXII));
    s_grammatica_intern = internamentum_creare(s_grammatica_piscina);

    s_grammatica = lapifex_grammaticam_legere(s_grammatica_piscina,
        s_grammatica_intern, GRAMMATICA_EXPRESSIO);
    si (!s_grammatica)
    {
        fprintf(stderr, "lapifex_c89: grammatica non lecta\n");
        redde;
    }

    lapifex_first_computare(s_grammatica);
    lapifex_follow_computare(s_grammatica);

    collectio = lapifex_collectio_lalr_construere(s_grammatica);
    si (!collectio)
    {
        fprintf(stderr, "lapifex_c89: collectio non constructa\n");
        redde;
    }

    s_tabula = lapifex_tabulam_construere(collectio);
    si (!s_tabula)
    {
        fprintf(stderr, "lapifex_c89: tabula non constructa\n");
    }
}

Arbor2Nodus*
lapifex_c89_expressionem_parsare(
    Piscina*              piscina,
    InternamentumChorda*  intern,
    constans character*   fons,
    i32                   mensura)
{
    Arbor2Lexator*          lexator;
    Xar*                    lexemata;
    Arbor2LapifexAdaptator* adaptator;
    Arbor2LapifexFructus    conv_fructus;
    LapifexGLR*             glr;
    LapifexGLRFructus       fructus;
    LapifexC89Contextus     ctx;
    chorda*                 via;

    si (!piscina || !intern || !fons) redde NIHIL;

    /* Parare grammaticam et tabulam (semel) */
    _tabulam_parare();
    si (!s_tabula || !s_grammatica) redde NIHIL;

    /* 1. Lexere fontem */
    lexator = arbor2_lexator_creare(piscina, intern, fons, mensura);
    si (!lexator)
    {
        fprintf(stderr, "lapifex_c89: lexator non creatus\n");
        redde NIHIL;
    }

    lexemata = arbor2_lexema_omnia(lexator);
    si (!lexemata)
    {
        fprintf(stderr, "lapifex_c89: lexemata non obtenta\n");
        redde NIHIL;
    }

    /* 2. Creare adaptatorem et convertere lexemata */
    adaptator = arbor2_lapifex_adaptator_creare(piscina, s_grammatica);
    si (!adaptator)
    {
        fprintf(stderr, "lapifex_c89: adaptator non creatus\n");
        redde NIHIL;
    }

    conv_fructus = arbor2_lapifex_convertere(adaptator, lexemata);
    si (!conv_fructus.successus)
    {
        fprintf(stderr, "lapifex_c89: conversio fracta\n");
        redde NIHIL;
    }

    /* 3. Creare GLR motorem et parsare */
    glr = lapifex_glr_creare(piscina, s_tabula);
    si (!glr)
    {
        fprintf(stderr, "lapifex_c89: GLR motor non creatus\n");
        redde NIHIL;
    }

    /* Parare contextum pro callback */
    via = chorda_internare_ex_literis(intern, "<expressio>");
    ctx.piscina = piscina;
    ctx.intern = intern;
    ctx.via_file = via;

    fructus = lapifex_glr_parsare(glr,
        conv_fructus.signa, conv_fructus.numerus,
        lapifex_c89_expressio_reductio, &ctx);

    si (!fructus.successus)
    {
        fprintf(stderr, "lapifex_c89: parsatio fracta (status=%d, term=%d, pos=%d)\n",
            (int)fructus.error.status,
            (int)fructus.error.terminalis,
            (int)fructus.error.positio);
        redde NIHIL;
    }

    si (fructus.numerus_fructuum < I)
    {
        fprintf(stderr, "lapifex_c89: nullus fructus\n");
        redde NIHIL;
    }

    redde NODUS_EX(fructus.valori[ZEPHYRUM]);
}

/* ================================================
 * API: Parsare declarationem
 *
 * Eadem pipeline ut expressionem_parsare, sed contextus
 * via est "<declaratio>" et grammatica iam acceptat
 * utramque (summum -> declaratio | virga).
 * ================================================ */

Arbor2Nodus*
lapifex_c89_declarationem_parsare(
    Piscina*              piscina,
    InternamentumChorda*  intern,
    constans character*   fons,
    i32                   mensura)
{
    Arbor2Lexator*          lexator;
    Xar*                    lexemata;
    Arbor2LapifexAdaptator* adaptator;
    Arbor2LapifexFructus    conv_fructus;
    LapifexGLR*             glr;
    LapifexGLRFructus       fructus;
    LapifexC89Contextus     ctx;
    chorda*                 via;

    si (!piscina || !intern || !fons) redde NIHIL;

    _tabulam_parare();
    si (!s_tabula || !s_grammatica) redde NIHIL;

    lexator = arbor2_lexator_creare(piscina, intern, fons, mensura);
    si (!lexator)
    {
        fprintf(stderr, "lapifex_c89_decl: lexator non creatus\n");
        redde NIHIL;
    }

    lexemata = arbor2_lexema_omnia(lexator);
    si (!lexemata)
    {
        fprintf(stderr, "lapifex_c89_decl: lexemata non obtenta\n");
        redde NIHIL;
    }

    adaptator = arbor2_lapifex_adaptator_creare(piscina, s_grammatica);
    si (!adaptator)
    {
        fprintf(stderr, "lapifex_c89_decl: adaptator non creatus\n");
        redde NIHIL;
    }

    conv_fructus = arbor2_lapifex_convertere(adaptator, lexemata);
    si (!conv_fructus.successus)
    {
        fprintf(stderr, "lapifex_c89_decl: conversio fracta\n");
        redde NIHIL;
    }

    glr = lapifex_glr_creare(piscina, s_tabula);
    si (!glr)
    {
        fprintf(stderr, "lapifex_c89_decl: GLR motor non creatus\n");
        redde NIHIL;
    }

    via = chorda_internare_ex_literis(intern, "<declaratio>");
    ctx.piscina = piscina;
    ctx.intern = intern;
    ctx.via_file = via;

    fructus = lapifex_glr_parsare(glr,
        conv_fructus.signa, conv_fructus.numerus,
        lapifex_c89_expressio_reductio, &ctx);

    si (!fructus.successus)
    {
        fprintf(stderr, "lapifex_c89_decl: parsatio fracta (status=%d, term=%d, pos=%d)\n",
            (int)fructus.error.status,
            (int)fructus.error.terminalis,
            (int)fructus.error.positio);
        redde NIHIL;
    }

    si (fructus.numerus_fructuum < I)
    {
        fprintf(stderr, "lapifex_c89_decl: nullus fructus\n");
        redde NIHIL;
    }

    redde NODUS_EX(fructus.valori[ZEPHYRUM]);
}

/* ================================================
 * API: Parsare sententiam (statement)
 * ================================================ */
Arbor2Nodus*
lapifex_c89_sententiam_parsare(
    Piscina*              piscina,
    InternamentumChorda*  intern,
    constans character*   fons,
    i32                   mensura)
{
    Arbor2Lexator*          lexator;
    Xar*                    lexemata;
    Arbor2LapifexAdaptator* adaptator;
    Arbor2LapifexFructus    conv_fructus;
    LapifexGLR*             glr;
    LapifexGLRFructus       fructus;
    LapifexC89Contextus     ctx;
    chorda*                 via;

    si (!piscina || !intern || !fons) redde NIHIL;

    _tabulam_parare();
    si (!s_tabula || !s_grammatica) redde NIHIL;

    lexator = arbor2_lexator_creare(piscina, intern, fons, mensura);
    si (!lexator)
    {
        fprintf(stderr, "lapifex_c89_sent: lexator non creatus\n");
        redde NIHIL;
    }

    lexemata = arbor2_lexema_omnia(lexator);
    si (!lexemata)
    {
        fprintf(stderr, "lapifex_c89_sent: lexemata non obtenta\n");
        redde NIHIL;
    }

    adaptator = arbor2_lapifex_adaptator_creare(piscina, s_grammatica);
    si (!adaptator)
    {
        fprintf(stderr, "lapifex_c89_sent: adaptator non creatus\n");
        redde NIHIL;
    }

    conv_fructus = arbor2_lapifex_convertere(adaptator, lexemata);
    si (!conv_fructus.successus)
    {
        fprintf(stderr, "lapifex_c89_sent: conversio fracta\n");
        redde NIHIL;
    }

    glr = lapifex_glr_creare(piscina, s_tabula);
    si (!glr)
    {
        fprintf(stderr, "lapifex_c89_sent: GLR motor non creatus\n");
        redde NIHIL;
    }

    via = chorda_internare_ex_literis(intern, "<sententia>");
    ctx.piscina = piscina;
    ctx.intern = intern;
    ctx.via_file = via;

    fructus = lapifex_glr_parsare(glr,
        conv_fructus.signa, conv_fructus.numerus,
        lapifex_c89_expressio_reductio, &ctx);

    si (!fructus.successus)
    {
        fprintf(stderr, "lapifex_c89_sent: parsatio fracta (status=%d, term=%d, pos=%d)\n",
            (int)fructus.error.status,
            (int)fructus.error.terminalis,
            (int)fructus.error.positio);
        redde NIHIL;
    }

    si (fructus.numerus_fructuum < I)
    {
        fprintf(stderr, "lapifex_c89_sent: nullus fructus\n");
        redde NIHIL;
    }

    redde NODUS_EX(fructus.valori[ZEPHYRUM]);
}

/* ================================================
 * API: Praescandere typedef nomina
 *
 * Scandere lexemata et remappare IDENTIFICATOR -> NOMEN_TYPUS
 * ubi nomen est typedef notum.
 * ================================================ */
vacuum
lapifex_c89_typedef_praescandere(
    Xar*                     lexemata,
    InternamentumChorda*     intern,
    constans character* constans* externa_nomina,
    i32                      numerus_ext)
{
    Piscina* piscina_loc;
    TabulaDispersa* nomina_typorum;
    i32 num_lex;
    i32 i;

    si (!lexemata) redde;

    num_lex = (i32)xar_numerus(lexemata);
    si (num_lex == ZEPHYRUM) redde;

    /* Creare piscinam temporariam pro tabula dispersa */
    piscina_loc = piscina_generare_dynamicum(
        "typedef_praescandere", (memoriae_index)(MMMMXCVI * IV));
    nomina_typorum = tabula_dispersa_creare_chorda(piscina_loc, LXIV);

    /* Addere externa nomina (si provisa) */
    si (externa_nomina && numerus_ext > ZEPHYRUM)
    {
        i32 j;
        per (j = ZEPHYRUM; j < numerus_ext; j++)
        {
            chorda clavis;
            clavis.datum = (i8*)(longus)externa_nomina[j];
            clavis.mensura = (i32)strlen(externa_nomina[j]);
            tabula_dispersa_inserere(nomina_typorum, clavis, NIHIL);
        }
    }

    (vacuum)intern;

    /* Scandere omnia lexemata */
    per (i = ZEPHYRUM; i < num_lex; i++)
    {
        Arbor2Lexema** lex_ptr = (Arbor2Lexema**)xar_obtinere(lexemata, i);
        Arbor2Lexema* lex;

        si (!lex_ptr) perge;
        lex = *lex_ptr;
        si (!lex) perge;

        /* Si TYPEDEF verbum clavium, extrahere nomen definitum */
        si (lex->genus == ARBOR2_LEXEMA_TYPEDEF)
        {
            /* Quaerere ultimum IDENTIFICATOR ante SEMICOLON ad profunditatem 0.
             * Tractat struct/enum/union corpora per profunditatem brace. */
            i32 k;
            i32 prof = ZEPHYRUM;
            Arbor2Lexema* ultimum_id = NIHIL;

            per (k = i + I; k < num_lex; k++)
            {
                Arbor2Lexema** k_ptr = (Arbor2Lexema**)xar_obtinere(lexemata, k);
                Arbor2Lexema* k_lex;

                si (!k_ptr) perge;
                k_lex = *k_ptr;
                si (!k_lex) perge;

                /* Sequi profunditatem brace */
                si (k_lex->genus == ARBOR2_LEXEMA_BRACE_APERTA)
                {
                    prof++;
                    perge;
                }
                si (k_lex->genus == ARBOR2_LEXEMA_BRACE_CLAUSA)
                {
                    prof--;
                    perge;
                }

                /* Solum ; ad profunditatem 0 terminat typedef */
                si (k_lex->genus == ARBOR2_LEXEMA_SEMICOLON &&
                    prof == ZEPHYRUM)
                {
                    /* Inventum semicolon — addere ultimum IDENTIFICATOR */
                    si (ultimum_id)
                    {
                        tabula_dispersa_inserere(nomina_typorum,
                            ultimum_id->valor, NIHIL);
                    }

                    /* Remappare quoslibet IDENTIFICATOR notos in hac typedef decl */
                    {
                        i32 m;
                        per (m = i + I; m < k; m++)
                        {
                            Arbor2Lexema** m_ptr =
                                (Arbor2Lexema**)xar_obtinere(lexemata, m);
                            Arbor2Lexema* m_lex;

                            si (!m_ptr) perge;
                            m_lex = *m_ptr;
                            si (!m_lex) perge;

                            /* Saltare ultimum_id (est nomen definitionis) */
                            si (m_lex == ultimum_id) perge;

                            si (m_lex->genus == ARBOR2_LEXEMA_IDENTIFICATOR &&
                                tabula_dispersa_continet(nomina_typorum,
                                    m_lex->valor))
                            {
                                m_lex->genus = ARBOR2_LEXEMA_NOMEN_TYPUS;
                            }
                        }
                    }

                    /* Saltare ad post semicolon */
                    i = k;
                    frange;
                }

                /* Solum tracta IDENTIFICATOR ad profunditatem 0 */
                si (k_lex->genus == ARBOR2_LEXEMA_IDENTIFICATOR &&
                    prof == ZEPHYRUM)
                {
                    ultimum_id = k_lex;
                }
            }
            perge;
        }

        /* Si IDENTIFICATOR et notum ut typedef nomen, remappare */
        si (lex->genus == ARBOR2_LEXEMA_IDENTIFICATOR &&
            tabula_dispersa_continet(nomina_typorum, lex->valor))
        {
            lex->genus = ARBOR2_LEXEMA_NOMEN_TYPUS;
        }
    }

    piscina_destruere(piscina_loc);
}

/* ================================================
 * API: Parsare translationem (translation unit)
 *
 * Scandere fontem et parsare singulas declarationes/definitiones.
 * Colligere in TRANSLATION_UNIT nodum.
 * ================================================ */

/* Invenire finem segmenti in lexematibus (token-level).
 * Redit index post ultimum lexema segmenti.
 *
 * Logica: } ad profunditatem 0 frangit segmentum SOLUM si { praecedebatur
 * per ) (definitio functi). Alioquin (struct/enum/union) pergit scandere
 * usque ad ; ad profunditatem 0.
 */
hic_manens i32
_segmentum_finem_in_lexematibus(
    Xar*  lexemata,
    i32   initium,
    i32   num_lex)
{
    i32              profunditas = ZEPHYRUM;
    b32              functi_corpus = FALSUM;
    Arbor2LexemaGenus ultimum_genus = ARBOR2_LEXEMA_EOF;
    i32              i;

    per (i = initium; i < num_lex; i++)
    {
        Arbor2Lexema** lex_ptr = (Arbor2Lexema**)xar_obtinere(lexemata, i);
        Arbor2Lexema*  lex;

        si (!lex_ptr) perge;
        lex = *lex_ptr;
        si (!lex) perge;

        /* Saltare trivia */
        si (arbor2_lapifex_est_trivia(lex->genus)) perge;

        /* Sistere ad EOF */
        si (lex->genus == ARBOR2_LEXEMA_EOF) frange;

        si (lex->genus == ARBOR2_LEXEMA_BRACE_APERTA)
        {
            si (profunditas == ZEPHYRUM)
            {
                /* { ad profunditatem 0 -> 1: definitio functi si ) praecedit */
                functi_corpus =
                    (ultimum_genus == ARBOR2_LEXEMA_PAREN_CLAUSA);
            }
            profunditas++;
        }
        alioquin si (lex->genus == ARBOR2_LEXEMA_BRACE_CLAUSA)
        {
            profunditas--;
            si (profunditas == ZEPHYRUM && functi_corpus)
            {
                /* Finis definitionis functi */
                redde i + I;
            }
            /* Si non functi_corpus (struct/enum), pergere ad ; */
        }
        alioquin si (lex->genus == ARBOR2_LEXEMA_SEMICOLON &&
                     profunditas == ZEPHYRUM)
        {
            redde i + I;
        }

        ultimum_genus = lex->genus;
    }
    redde num_lex;
}

Arbor2Nodus*
lapifex_c89_translationem_parsare(
    Piscina*              piscina,
    InternamentumChorda*  intern,
    constans character*   fons,
    i32                   mensura)
{
    Arbor2Nodus*            nodus_trans;
    Xar*                    declarationes;
    Arbor2Lexator*          lexator;
    Xar*                    lexemata;
    Arbor2LapifexAdaptator* adaptator;
    i32                     num_lex;
    i32                     cursor;
    Arbor2Lexema*           eof_lex;
    LapifexC89Contextus     ctx;
    chorda*                 via;

    si (!piscina || !intern || !fons) redde NIHIL;

    _tabulam_parare();
    si (!s_tabula || !s_grammatica) redde NIHIL;

    /* 1. Lexere totum fontem */
    lexator = arbor2_lexator_creare(piscina, intern, fons, mensura);
    si (!lexator) redde NIHIL;

    lexemata = arbor2_lexema_omnia(lexator);
    si (!lexemata) redde NIHIL;

    /* 2. Praescandere pro typedef (remappat in loco) */
    lapifex_c89_typedef_praescandere(lexemata, intern, NIHIL, ZEPHYRUM);

    /* 3. Creare adaptatorem */
    adaptator = arbor2_lapifex_adaptator_creare(piscina, s_grammatica);
    si (!adaptator) redde NIHIL;

    num_lex = (i32)xar_numerus(lexemata);

    /* Invenire EOF lexema */
    eof_lex = NIHIL;
    {
        i32 k;
        per (k = num_lex - I; k >= ZEPHYRUM; k--)
        {
            Arbor2Lexema** ep = (Arbor2Lexema**)xar_obtinere(lexemata, k);
            si (ep && *ep && (*ep)->genus == ARBOR2_LEXEMA_EOF)
            {
                eof_lex = *ep;
                frange;
            }
        }
    }

    /* Parare contextum */
    via = chorda_internare_ex_literis(intern, "<translatio>");
    ctx.piscina = piscina;
    ctx.intern = intern;
    ctx.via_file = via;

    /* 4. Creare TRANSLATION_UNIT nodum */
    nodus_trans = (Arbor2Nodus*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(Arbor2Nodus));
    memset(nodus_trans, ZEPHYRUM, magnitudo(Arbor2Nodus));
    nodus_trans->genus = ARBOR2_NODUS_TRANSLATION_UNIT;
    declarationes = xar_creare(piscina, (i32)magnitudo(Arbor2Nodus*));
    nodus_trans->datum.translation_unit.declarationes = declarationes;

    /* 5. Segmentare lexemata et parsare singula */
    cursor = ZEPHYRUM;

    /* Saltare trivia initiales */
    dum (cursor < num_lex)
    {
        Arbor2Lexema** lp = (Arbor2Lexema**)xar_obtinere(lexemata, cursor);
        si (!lp || !*lp) frange;
        si ((*lp)->genus == ARBOR2_LEXEMA_EOF) frange;
        si (!arbor2_lapifex_est_trivia((*lp)->genus)) frange;
        cursor++;
    }

    dum (cursor < num_lex)
    {
        i32                   finis;
        Xar*                  seg_lex;
        Arbor2LapifexFructus  conv;
        LapifexGLR*           glr;
        LapifexGLRFructus     fructus;
        i32                   k;

        /* Confer si solum EOF remanet */
        {
            Arbor2Lexema** lp = (Arbor2Lexema**)xar_obtinere(lexemata, cursor);
            si (!lp || !*lp || (*lp)->genus == ARBOR2_LEXEMA_EOF) frange;
        }

        /* Invenire finem segmenti */
        finis = _segmentum_finem_in_lexematibus(lexemata, cursor, num_lex);

        /* Creare sub-Xar pro hoc segmento */
        seg_lex = xar_creare(piscina, (i32)magnitudo(Arbor2Lexema*));
        per (k = cursor; k < finis; k++)
        {
            Arbor2Lexema** lp = (Arbor2Lexema**)xar_obtinere(lexemata, k);
            si (lp)
            {
                Arbor2Lexema** slot = (Arbor2Lexema**)xar_addere(seg_lex);
                *slot = *lp;
            }
        }

        /* Addere EOF ad finem segmenti */
        si (eof_lex)
        {
            Arbor2Lexema** slot = (Arbor2Lexema**)xar_addere(seg_lex);
            *slot = eof_lex;
        }

        /* Convertere per adaptatorem */
        conv = arbor2_lapifex_convertere(adaptator, seg_lex);
        si (!conv.successus)
        {
            fprintf(stderr,
                "lapifex_c89_trans: conversio fracta (cursor=%d)\n", cursor);
            cursor = finis;
            perge;
        }

        /* Parsare per GLR */
        glr = lapifex_glr_creare(piscina, s_tabula);
        si (!glr)
        {
            cursor = finis;
            perge;
        }

        fructus = lapifex_glr_parsare(glr,
            conv.signa, conv.numerus,
            lapifex_c89_expressio_reductio, &ctx);

        si (fructus.successus && fructus.numerus_fructuum >= I)
        {
            Arbor2Nodus*  seg_nodus = NODUS_EX(fructus.valori[ZEPHYRUM]);
            Arbor2Nodus** slot = (Arbor2Nodus**)xar_addere(declarationes);
            *slot = seg_nodus;
        }
        alioquin
        {
            fprintf(stderr,
                "lapifex_c89_trans: parsatio fracta (cursor=%d)\n", cursor);
        }

        cursor = finis;

        /* Saltare trivia inter segmenta */
        dum (cursor < num_lex)
        {
            Arbor2Lexema** lp =
                (Arbor2Lexema**)xar_obtinere(lexemata, cursor);
            si (!lp || !*lp) frange;
            si ((*lp)->genus == ARBOR2_LEXEMA_EOF) frange;
            si (!arbor2_lapifex_est_trivia((*lp)->genus)) frange;
            cursor++;
        }
    }

    /* Si singulum elementum, redde directe */
    si ((i32)xar_numerus(declarationes) == I)
    {
        Arbor2Nodus** elem =
            (Arbor2Nodus**)xar_obtinere(declarationes, ZEPHYRUM);
        si (elem) redde *elem;
    }

    si ((i32)xar_numerus(declarationes) == ZEPHYRUM)
    {
        fprintf(stderr, "lapifex_c89_trans: nullae declarationes\n");
        redde NIHIL;
    }

    /* Locatio */
    {
        Arbor2Nodus** primum =
            (Arbor2Nodus**)xar_obtinere(declarationes, ZEPHYRUM);
        Arbor2Nodus** ultimum = (Arbor2Nodus**)xar_obtinere(declarationes,
            (i32)xar_numerus(declarationes) - I);
        si (primum && *primum)
        {
            nodus_trans->linea_initium = (*primum)->linea_initium;
            nodus_trans->columna_initium = (*primum)->columna_initium;
            nodus_trans->lexema = (*primum)->lexema;
        }
        si (ultimum && *ultimum)
        {
            nodus_trans->linea_finis = (*ultimum)->linea_finis;
            nodus_trans->columna_finis = (*ultimum)->columna_finis;
        }
    }

    redde nodus_trans;
}

/* ================================================
 * Pons: Extrahere Arbor2Lexema* ex Arbor2Token*
 *
 * Expandere producit Xar* de Arbor2Token*.
 * Lapifex pipeline expectat Xar* de Arbor2Lexema*.
 * Haec functio extrahit lexema ex unoquoque token,
 * filtrans trivia (NOVA_LINEA, SPATIA, etc.).
 * ================================================ */

hic_manens Xar*
_lexemata_ex_tokens(
    Piscina*  piscina,
    Xar*      tokens)
{
    Xar*  lexemata;
    i32   num_tok;
    i32   i;

    si (!tokens) redde NIHIL;

    num_tok = (i32)xar_numerus(tokens);
    lexemata = xar_creare(piscina, (i32)magnitudo(Arbor2Lexema*));

    per (i = ZEPHYRUM; i < num_tok; i++)
    {
        Arbor2Token** tok_ptr = (Arbor2Token**)xar_obtinere(tokens, i);
        Arbor2Token*  tok;
        Arbor2Lexema* lex;
        Arbor2Lexema** slot;

        si (!tok_ptr) perge;
        tok = *tok_ptr;
        si (!tok || !tok->lexema) perge;

        lex = tok->lexema;

        /* Filtrare trivia */
        si (arbor2_lapifex_est_trivia(lex->genus)) perge;

        /* Filtrare EOF — addemus nostrum */
        si (lex->genus == ARBOR2_LEXEMA_EOF) perge;

        slot = (Arbor2Lexema**)xar_addere(lexemata);
        *slot = lex;
    }

    redde lexemata;
}

/* ================================================
 * Remappare typedef nomina ex expansione
 *
 * Utitur tabula typedef expansionis (quae includit
 * typedefs ex #include headeribus) vice praescandendi.
 * ================================================ */

hic_manens vacuum
_typedef_ex_expansione_remappare(
    Xar*              lexemata,
    Arbor2Expansion*  exp)
{
    i32 num_lex;
    i32 i;

    si (!lexemata || !exp) redde;

    num_lex = (i32)xar_numerus(lexemata);

    per (i = ZEPHYRUM; i < num_lex; i++)
    {
        Arbor2Lexema** lex_ptr = (Arbor2Lexema**)xar_obtinere(lexemata, i);
        Arbor2Lexema*  lex;

        si (!lex_ptr) perge;
        lex = *lex_ptr;
        si (!lex) perge;

        si (lex->genus == ARBOR2_LEXEMA_IDENTIFICATOR &&
            arbor2_expansion_est_typedef(exp, lex->valor))
        {
            lex->genus = ARBOR2_LEXEMA_NOMEN_TYPUS;
        }
    }
}

/* ================================================
 * API: Parsare fontem per expansionem
 *
 * Pipeline completus:
 *   1. Expandere (macros, #include, #if, etc.)
 *   2. Extrahere lexemata ex tokens
 *   3. Remappare typedef nomina ex expansione
 *   4. Segmentare et parsare per GLR
 * ================================================ */

Arbor2Nodus*
lapifex_c89_fontem_parsare(
    Piscina*              piscina,
    InternamentumChorda*  intern,
    Arbor2Expansion*      exp,
    constans character*   fons,
    i32                   mensura,
    constans character*   via_file)
{
    Xar*                    tokens;
    Xar*                    lexemata;
    Arbor2Nodus*            nodus_trans;
    Xar*                    declarationes;
    Arbor2LapifexAdaptator* adaptator;
    i32                     num_lex;
    i32                     cursor;
    Arbor2Lexema*           eof_lex;
    LapifexC89Contextus     ctx;
    chorda*                 via;

    si (!piscina || !intern || !exp || !fons) redde NIHIL;

    _tabulam_parare();
    si (!s_tabula || !s_grammatica) redde NIHIL;

    /* 1. Expandere fontem per preprocessorem */
    tokens = arbor2_expansion_processare(exp, fons, mensura, via_file);
    si (!tokens) redde NIHIL;

    /* 2. Extrahere lexemata ex tokens */
    lexemata = _lexemata_ex_tokens(piscina, tokens);
    si (!lexemata) redde NIHIL;

    /* 3. Remappare typedef nomina ex expansione */
    _typedef_ex_expansione_remappare(lexemata, exp);

    /* 4. Creare adaptatorem */
    adaptator = arbor2_lapifex_adaptator_creare(piscina, s_grammatica);
    si (!adaptator) redde NIHIL;

    num_lex = (i32)xar_numerus(lexemata);
    si (num_lex == ZEPHYRUM) redde NIHIL;

    /* Creare EOF lexema sinteticum */
    eof_lex = (Arbor2Lexema*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(Arbor2Lexema));
    memset(eof_lex, ZEPHYRUM, magnitudo(Arbor2Lexema));
    eof_lex->genus = ARBOR2_LEXEMA_EOF;

    /* Parare contextum */
    via = chorda_internare_ex_literis(intern,
        via_file ? via_file : "<fons>");
    ctx.piscina = piscina;
    ctx.intern = intern;
    ctx.via_file = via;

    /* 5. Creare TRANSLATION_UNIT nodum */
    nodus_trans = (Arbor2Nodus*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(Arbor2Nodus));
    memset(nodus_trans, ZEPHYRUM, magnitudo(Arbor2Nodus));
    nodus_trans->genus = ARBOR2_NODUS_TRANSLATION_UNIT;
    declarationes = xar_creare(piscina, (i32)magnitudo(Arbor2Nodus*));
    nodus_trans->datum.translation_unit.declarationes = declarationes;

    /* 6. Segmentare lexemata et parsare singula */
    cursor = ZEPHYRUM;

    /* Saltare trivia initiales */
    dum (cursor < num_lex)
    {
        Arbor2Lexema** lp = (Arbor2Lexema**)xar_obtinere(lexemata, cursor);
        si (!lp || !*lp) frange;
        si ((*lp)->genus == ARBOR2_LEXEMA_EOF) frange;
        si (!arbor2_lapifex_est_trivia((*lp)->genus)) frange;
        cursor++;
    }

    dum (cursor < num_lex)
    {
        i32                   finis;
        Xar*                  seg_lex;
        Arbor2LapifexFructus  conv;
        LapifexGLR*           glr;
        LapifexGLRFructus     fructus;
        i32                   k;

        /* Confer si solum EOF remanet */
        {
            Arbor2Lexema** lp = (Arbor2Lexema**)xar_obtinere(lexemata, cursor);
            si (!lp || !*lp || (*lp)->genus == ARBOR2_LEXEMA_EOF) frange;
        }

        /* Invenire finem segmenti */
        finis = _segmentum_finem_in_lexematibus(lexemata, cursor, num_lex);

        /* Creare sub-Xar pro hoc segmento */
        seg_lex = xar_creare(piscina, (i32)magnitudo(Arbor2Lexema*));
        per (k = cursor; k < finis; k++)
        {
            Arbor2Lexema** lp = (Arbor2Lexema**)xar_obtinere(lexemata, k);
            si (lp)
            {
                Arbor2Lexema** slot = (Arbor2Lexema**)xar_addere(seg_lex);
                *slot = *lp;
            }
        }

        /* Addere EOF ad finem segmenti */
        {
            Arbor2Lexema** slot = (Arbor2Lexema**)xar_addere(seg_lex);
            *slot = eof_lex;
        }

        /* Convertere per adaptatorem */
        conv = arbor2_lapifex_convertere(adaptator, seg_lex);
        si (!conv.successus)
        {
            fprintf(stderr,
                "lapifex_c89_fontem: conversio fracta (cursor=%d)\n", cursor);
            cursor = finis;
            perge;
        }

        /* Parsare per GLR */
        glr = lapifex_glr_creare(piscina, s_tabula);
        si (!glr)
        {
            cursor = finis;
            perge;
        }

        fructus = lapifex_glr_parsare(glr,
            conv.signa, conv.numerus,
            lapifex_c89_expressio_reductio, &ctx);

        si (fructus.successus && fructus.numerus_fructuum >= I)
        {
            Arbor2Nodus*  seg_nodus = NODUS_EX(fructus.valori[ZEPHYRUM]);
            Arbor2Nodus** slot = (Arbor2Nodus**)xar_addere(declarationes);
            *slot = seg_nodus;
        }
        alioquin
        {
            fprintf(stderr,
                "lapifex_c89_fontem: parsatio fracta (cursor=%d)\n", cursor);
        }

        cursor = finis;

        /* Saltare trivia inter segmenta */
        dum (cursor < num_lex)
        {
            Arbor2Lexema** lp =
                (Arbor2Lexema**)xar_obtinere(lexemata, cursor);
            si (!lp || !*lp) frange;
            si ((*lp)->genus == ARBOR2_LEXEMA_EOF) frange;
            si (!arbor2_lapifex_est_trivia((*lp)->genus)) frange;
            cursor++;
        }
    }

    /* Si singulum elementum, redde directe */
    si ((i32)xar_numerus(declarationes) == I)
    {
        Arbor2Nodus** elem =
            (Arbor2Nodus**)xar_obtinere(declarationes, ZEPHYRUM);
        si (elem) redde *elem;
    }

    si ((i32)xar_numerus(declarationes) == ZEPHYRUM)
    {
        fprintf(stderr, "lapifex_c89_fontem: nullae declarationes\n");
        redde NIHIL;
    }

    /* Locatio */
    {
        Arbor2Nodus** primum =
            (Arbor2Nodus**)xar_obtinere(declarationes, ZEPHYRUM);
        Arbor2Nodus** ultimum = (Arbor2Nodus**)xar_obtinere(declarationes,
            (i32)xar_numerus(declarationes) - I);
        si (primum && *primum)
        {
            nodus_trans->linea_initium = (*primum)->linea_initium;
            nodus_trans->columna_initium = (*primum)->columna_initium;
            nodus_trans->lexema = (*primum)->lexema;
        }
        si (ultimum && *ultimum)
        {
            nodus_trans->linea_finis = (*ultimum)->linea_finis;
            nodus_trans->columna_finis = (*ultimum)->columna_finis;
        }
    }

    redde nodus_trans;
}
