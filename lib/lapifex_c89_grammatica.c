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

    /* Verba Clavium (solum quae in expressionibus usantur) */
    "    <terminalis titulus=\"CHAR\"/>"
    "    <terminalis titulus=\"CONST\"/>"
    "    <terminalis titulus=\"DOUBLE\"/>"
    "    <terminalis titulus=\"ENUM\"/>"
    "    <terminalis titulus=\"FLOAT\"/>"
    "    <terminalis titulus=\"INT\"/>"
    "    <terminalis titulus=\"LONG\"/>"
    "    <terminalis titulus=\"SHORT\"/>"
    "    <terminalis titulus=\"SIGNED\"/>"
    "    <terminalis titulus=\"SIZEOF\"/>"
    "    <terminalis titulus=\"STRUCT\"/>"
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

    /* Punctuatio (solum quae in expressionibus usantur) */
    "    <terminalis titulus=\"COMMA\"/>"
    "    <terminalis titulus=\"COLON\"/>"
    "    <terminalis titulus=\"QUAESTIO\"/>"

    /* Delimitatores */
    "    <terminalis titulus=\"PAREN_APERTA\"/>"
    "    <terminalis titulus=\"PAREN_CLAUSA\"/>"
    "    <terminalis titulus=\"BRACKET_APERTA\"/>"
    "    <terminalis titulus=\"BRACKET_CLAUSA\"/>"

    "    <terminalis titulus=\"EOF\"/>"
    "  </terminalia>"

    /* ---- Regulae ---- */
    "  <regulae>"

    /* P0-P1: virga (comma expression, infima praecedentia) */
    "    <regula titulus=\"virga\">"
    "      <productio>assignatio</productio>"                           /* P0 */
    "      <productio>virga COMMA assignatio</productio>"               /* P1 */
    "    </regula>"

    /* P2-P12: assignatio (dextro-associativa) */
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

    "  </regulae>"
    "  <initium>virga</initium>"
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
        casus XCI:   /* pointer -> ASTERISCUS pointer */
        casus XCII:  /* pointer -> ASTERISCUS CONST */
        casus XCIII: /* pointer -> ASTERISCUS CONST pointer */
            /* Pointer nodi non creantur separatim in M2.3 --
             * type name nodus iam continet sufficientem informatem */
            redde valori[ZEPHYRUM];

        /* ---- Productio Augmentata ---- */
        ordinarius:
            /* __initium__ -> virga EOF */
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
        "lapifex_c89_grammatica", (memoriae_index)(MMMMXCVI * XVI));
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
