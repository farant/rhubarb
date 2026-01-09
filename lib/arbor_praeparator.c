/* ==================================================
 * ARBOR PRAEPARATOR - C89 Preprocessor Implementation
 *
 * Tractat directivas (#include, #define, #if, etc.)
 * Expandit macros cum tracking originis.
 * ================================================== */

#include "arbor_praeparator.h"
#include "filum.h"
#include "via.h"
#include <string.h>
#include <stdio.h>

/* Maximum include depth to prevent infinite recursion */
#define ARBOR_MAX_INCLUDE_PROFUNDITAS  LXIV

/* ==================================================
 * Status Conditionalis (pro #if stack)
 * ================================================== */

nomen structura {
    b32     vidit_verum;      /* Vidit ramum verum? */
    b32     currens_activus;  /* Ramus currens activus? */
    b32     pater_activus;    /* Pater contextus activus? */
} ArborCondStatus;

/* ==================================================
 * Praeparator State (Internal)
 * ================================================== */

structura ArborPraeparator {
    Piscina*              piscina;
    InternamentumChorda*  intern;
    ArborPPModus          modus;            /* PROCESSARE vel PRESERVARE vel HYBRID */
    TabulaDispersa*       macros;           /* chorda -> ArborMacroDefinitio* */
    TabulaDispersa*       keyword_macros;   /* chorda -> i32 (keyword token type) */
    TabulaDispersa*       typedef_nomina;   /* chorda -> VERUM (typedef names) */
    Xar*                  viae_include;     /* Xar<chorda*> viae quaestionis */
    Xar*                  fila_inclusa;     /* Xar<chorda*> jam inclusa (custodia) */

    /* Acervus conditionalis compilationis */
    Xar*                  cond_acervus;     /* Xar<ArborCondStatus> */
    b32                   cond_activus;     /* Nunc emittens lexemata? */

    /* Contextus expansionis currentis */
    ArborOrigo*           origo_currens;    /* Pro tracking nidificato */
    i32                   profunditas_expansionis;

    /* Collectio errorum */
    Xar*                  errores;          /* Xar<ArborPPError*> */

    /* Acervus recursus macro (pro protectione) */
    Xar*                  macro_in_expansione;  /* Xar<chorda*> */
};

/* ==================================================
 * Nomina Directivarum (pro debugging)
 * ================================================== */

hic_manens constans character* NOMINA_DIRECTIVARUM[] = {
    "include",
    "define",
    "undef",
    "if",
    "ifdef",
    "ifndef",
    "elif",
    "else",
    "endif",
    "line",
    "error",
    "pragma",
    "unknown"
};

/* Forward declarations */
interior vacuum _registrare_keyword_macro(ArborPraeparator* pp, ArborMacroDefinitio* macro);
interior vacuum _detectare_typedef(ArborPraeparator* pp, Xar* lexemata, i32 pos_initium, i32 pos_finis);

/* ==================================================
 * API - Creatio et Configuratio
 * ================================================== */

ArborPraeparator*
arbor_praeparator_creare(
    Piscina*              piscina,
    InternamentumChorda*  intern)
{
    ArborPraeparator* pp;

    si (piscina == NIHIL || intern == NIHIL)
    {
        redde NIHIL;
    }

    pp = piscina_allocare(piscina, magnitudo(ArborPraeparator));
    si (pp == NIHIL)
    {
        redde NIHIL;
    }

    pp->piscina = piscina;
    pp->intern = intern;
    pp->modus = ARBOR_PP_MODUS_PROCESSARE;  /* Default: expandere */

    /* Creare tabula macro */
    pp->macros = tabula_dispersa_creare_chorda(piscina, CXXVIII);
    si (pp->macros == NIHIL)
    {
        redde NIHIL;
    }

    /* Creare tabula keyword macros (macro_name -> keyword token type) */
    pp->keyword_macros = tabula_dispersa_creare_chorda(piscina, CXXVIII);
    si (pp->keyword_macros == NIHIL)
    {
        redde NIHIL;
    }

    /* Creare tabula typedef nomina */
    pp->typedef_nomina = tabula_dispersa_creare_chorda(piscina, CXXVIII);
    si (pp->typedef_nomina == NIHIL)
    {
        redde NIHIL;
    }

    /* Registrare standard C typedef nomina (size_t, FILE, etc.) */
    {
        constans character* std_typedefs[] = {
            "size_t", "ptrdiff_t", "FILE", "fpos_t",
            "wchar_t", "wint_t", "time_t", "clock_t",
            "va_list", "jmp_buf", "sig_atomic_t",
            NIHIL
        };
        i32 i;
        per (i = ZEPHYRUM; std_typedefs[i] != NIHIL; i++)
        {
            chorda std_ch = chorda_ex_literis(std_typedefs[i], piscina);
            chorda* std_name = chorda_internare(pp->intern, std_ch);
            si (std_name != NIHIL)
            {
                tabula_dispersa_inserere(pp->typedef_nomina, *std_name,
                    (vacuum*)(longus)VERUM);
            }
        }
    }

    /* Creare viae include */
    pp->viae_include = xar_creare(piscina, magnitudo(chorda*));
    si (pp->viae_include == NIHIL)
    {
        redde NIHIL;
    }

    /* Creare lista filorum inclusorum */
    pp->fila_inclusa = xar_creare(piscina, magnitudo(chorda*));
    si (pp->fila_inclusa == NIHIL)
    {
        redde NIHIL;
    }

    /* Creare acervum conditionalem */
    pp->cond_acervus = xar_creare(piscina, magnitudo(ArborCondStatus));
    si (pp->cond_acervus == NIHIL)
    {
        redde NIHIL;
    }
    pp->cond_activus = VERUM;  /* Initiale activus */

    /* Creare lista errorum */
    pp->errores = xar_creare(piscina, magnitudo(ArborPPError*));
    si (pp->errores == NIHIL)
    {
        redde NIHIL;
    }

    /* Creare acervum recursus macro */
    pp->macro_in_expansione = xar_creare(piscina, magnitudo(chorda*));
    si (pp->macro_in_expansione == NIHIL)
    {
        redde NIHIL;
    }

    /* Initializare contextum expansionis */
    pp->origo_currens = NIHIL;
    pp->profunditas_expansionis = ZEPHYRUM;

    redde pp;
}

vacuum
arbor_praeparator_ponere_modum(
    ArborPraeparator*     pp,
    ArborPPModus          modus)
{
    si (pp != NIHIL)
    {
        pp->modus = modus;
    }
}

ArborPPModus
arbor_praeparator_obtinere_modum(
    ArborPraeparator*     pp)
{
    si (pp == NIHIL)
    {
        redde ARBOR_PP_MODUS_PROCESSARE;
    }
    redde pp->modus;
}

vacuum
arbor_praeparator_addere_via(
    ArborPraeparator*     pp,
    constans character*   via)
{
    chorda* via_internata;
    chorda** slotus;

    si (pp == NIHIL || via == NIHIL)
    {
        redde;
    }

    /* Internare via */
    via_internata = chorda_internare_ex_literis(pp->intern, via);
    si (via_internata == NIHIL)
    {
        redde;
    }

    /* Addere ad viae include */
    slotus = xar_addere(pp->viae_include);
    si (slotus != NIHIL)
    {
        *slotus = via_internata;
    }
}

vacuum
arbor_praeparator_definire(
    ArborPraeparator*     pp,
    constans character*   nomen_macro,
    constans character*   valor)
{
    ArborMacroDefinitio* macro;
    chorda* nomen_internatum;
    chorda nomen_ch;

    si (pp == NIHIL || nomen_macro == NIHIL)
    {
        redde;
    }

    /* Internare nomen */
    nomen_internatum = chorda_internare_ex_literis(pp->intern, nomen_macro);
    si (nomen_internatum == NIHIL)
    {
        redde;
    }

    /* Creare definitio macro */
    macro = piscina_allocare(pp->piscina, magnitudo(ArborMacroDefinitio));
    si (macro == NIHIL)
    {
        redde;
    }

    macro->titulus = nomen_internatum;
    macro->est_functio = FALSUM;
    macro->est_variadic = FALSUM;
    macro->parametra = NIHIL;
    macro->linea_def = ZEPHYRUM;
    macro->via_file.datum = NIHIL;
    macro->via_file.mensura = ZEPHYRUM;

    /* Creare corpus (lexemata valoris) */
    macro->corpus = xar_creare(pp->piscina, magnitudo(ArborLexema*));
    si (macro->corpus == NIHIL)
    {
        redde;
    }

    /* Si valor datus, lex et addere ad corpus */
    si (valor != NIHIL && valor[ZEPHYRUM] != '\0')
    {
        ArborLexator* lexator;
        Xar* lexemata;
        i32 i;
        i32 num;

        lexator = arbor_lexator_creare(
            pp->piscina,
            pp->intern,
            valor,
            (i32)strlen(valor));

        si (lexator != NIHIL)
        {
            lexemata = arbor_lexema_omnia(lexator);
            si (lexemata != NIHIL)
            {
                num = xar_numerus(lexemata);
                per (i = ZEPHYRUM; i < num; i++)
                {
                    ArborLexema* lex;
                    ArborLexema** slotus;

                    lex = *(ArborLexema**)xar_obtinere(lexemata, i);

                    /* Non addere EOF */
                    si (lex->genus != ARBOR_LEXEMA_EOF)
                    {
                        slotus = xar_addere(macro->corpus);
                        si (slotus != NIHIL)
                        {
                            *slotus = lex;
                        }
                    }
                }
            }
        }
    }

    /* Inserere in tabula */
    nomen_ch = *nomen_internatum;
    tabula_dispersa_inserere(pp->macros, nomen_ch, macro);

    /* Registrare si keyword macro */
    _registrare_keyword_macro(pp, macro);
}

vacuum
arbor_praeparator_undefinire(
    ArborPraeparator*     pp,
    constans character*   nomen_macro)
{
    chorda* nomen_internatum;

    si (pp == NIHIL || nomen_macro == NIHIL)
    {
        redde;
    }

    /* Internare nomen pro quaestione */
    nomen_internatum = chorda_internare_ex_literis(pp->intern, nomen_macro);
    si (nomen_internatum == NIHIL)
    {
        redde;
    }

    tabula_dispersa_delere(pp->macros, *nomen_internatum);
}

/* ==================================================
 * API - Query
 * ================================================== */

ArborMacroDefinitio*
arbor_praeparator_obtinere_macro(
    ArborPraeparator*     pp,
    chorda*               nomen_macro)
{
    vacuum* valor;

    si (pp == NIHIL || nomen_macro == NIHIL)
    {
        redde NIHIL;
    }

    si (tabula_dispersa_invenire(pp->macros, *nomen_macro, &valor))
    {
        redde (ArborMacroDefinitio*)valor;
    }

    redde NIHIL;
}

b32
arbor_praeparator_est_definitum(
    ArborPraeparator*     pp,
    chorda*               nomen_macro)
{
    si (pp == NIHIL || nomen_macro == NIHIL)
    {
        redde FALSUM;
    }

    redde tabula_dispersa_continet(pp->macros, *nomen_macro);
}

Xar*
arbor_praeparator_errores(ArborPraeparator* pp)
{
    si (pp == NIHIL)
    {
        redde NIHIL;
    }

    redde pp->errores;
}

TabulaDispersa*
arbor_praeparator_obtinere_keyword_macros(ArborPraeparator* pp)
{
    si (pp == NIHIL)
    {
        redde NIHIL;
    }

    redde pp->keyword_macros;
}

TabulaDispersa*
arbor_praeparator_obtinere_typedef_nomina(ArborPraeparator* pp)
{
    si (pp == NIHIL)
    {
        redde NIHIL;
    }

    redde pp->typedef_nomina;
}

/* ==================================================
 * API - Utilities
 * ================================================== */

constans character*
arbor_directiva_genus_nomen(ArborDirectivaGenus genus)
{
    si (genus >= ARBOR_DIRECTIVA_INCLUDE && genus <= ARBOR_DIRECTIVA_UNKNOWN)
    {
        redde NOMINA_DIRECTIVARUM[genus];
    }

    redde "ignotum";
}

chorda
arbor_pp_error_formare(Piscina* piscina, ArborPPError* err)
{
    character buffer[DXII];
    integer len_signed;
    i32 len;
    chorda fructus;

    si (piscina == NIHIL || err == NIHIL)
    {
        fructus.datum = NIHIL;
        fructus.mensura = ZEPHYRUM;
        redde fructus;
    }

    len_signed = snprintf(buffer, DXII,
        "%.*s:%d:%d: error: %.*s",
        (integer)err->via_file.mensura, (character*)err->via_file.datum,
        (integer)err->linea,
        (integer)err->columna,
        (integer)err->nuntius.mensura, (character*)err->nuntius.datum);

    si (len_signed < ZEPHYRUM || len_signed >= (integer)DXII)
    {
        len = DXII - I;
    }
    alioquin
    {
        len = (i32)len_signed;
    }

    fructus.datum = piscina_allocare(piscina, len + I);
    si (fructus.datum == NIHIL)
    {
        fructus.mensura = ZEPHYRUM;
        redde fructus;
    }

    memcpy(fructus.datum, buffer, (size_t)len);
    fructus.datum[len] = '\0';
    fructus.mensura = len;

    redde fructus;
}

/* ==================================================
 * Functiones Internae - Keyword Macro Detection
 * ================================================== */

/* Verifica si token genus est keyword quod parser debet cognoscere */
interior b32
_est_keyword_vel_typus(ArborLexemaGenus genus)
{
    /* Storage class specifiers */
    si (genus == ARBOR_LEXEMA_AUTO ||
        genus == ARBOR_LEXEMA_REGISTER ||
        genus == ARBOR_LEXEMA_STATIC ||
        genus == ARBOR_LEXEMA_EXTERN ||
        genus == ARBOR_LEXEMA_TYPEDEF)
    {
        redde VERUM;
    }

    /* Type qualifiers */
    si (genus == ARBOR_LEXEMA_CONST ||
        genus == ARBOR_LEXEMA_VOLATILE)
    {
        redde VERUM;
    }

    /* Type specifiers */
    si (genus == ARBOR_LEXEMA_VOID ||
        genus == ARBOR_LEXEMA_CHAR ||
        genus == ARBOR_LEXEMA_SHORT ||
        genus == ARBOR_LEXEMA_INT ||
        genus == ARBOR_LEXEMA_LONG ||
        genus == ARBOR_LEXEMA_FLOAT ||
        genus == ARBOR_LEXEMA_DOUBLE ||
        genus == ARBOR_LEXEMA_SIGNED ||
        genus == ARBOR_LEXEMA_UNSIGNED ||
        genus == ARBOR_LEXEMA_STRUCT ||
        genus == ARBOR_LEXEMA_UNION ||
        genus == ARBOR_LEXEMA_ENUM)
    {
        redde VERUM;
    }

    /* Control flow keywords */
    si (genus == ARBOR_LEXEMA_IF ||
        genus == ARBOR_LEXEMA_ELSE ||
        genus == ARBOR_LEXEMA_SWITCH ||
        genus == ARBOR_LEXEMA_CASE ||
        genus == ARBOR_LEXEMA_DEFAULT ||
        genus == ARBOR_LEXEMA_FOR ||
        genus == ARBOR_LEXEMA_WHILE ||
        genus == ARBOR_LEXEMA_DO ||
        genus == ARBOR_LEXEMA_BREAK ||
        genus == ARBOR_LEXEMA_CONTINUE ||
        genus == ARBOR_LEXEMA_GOTO ||
        genus == ARBOR_LEXEMA_RETURN ||
        genus == ARBOR_LEXEMA_SIZEOF)
    {
        redde VERUM;
    }

    redde FALSUM;
}

/* Registrare macro in keyword_macros tabula si expandit ad keyword vel typus */
interior vacuum
_registrare_keyword_macro(
    ArborPraeparator*      pp,
    ArborMacroDefinitio*   macro)
{
    ArborLexema* body_tok;
    i32* keyword_ptr;
    vacuum* existing_val;

    si (pp == NIHIL || macro == NIHIL)
    {
        redde;
    }

    /* Solum object-like macros (non function-like) */
    si (macro->est_functio)
    {
        redde;
    }

    /* Corpus debet habere exacte unum token */
    si (macro->corpus == NIHIL || xar_numerus(macro->corpus) != I)
    {
        redde;
    }

    body_tok = *(ArborLexema**)xar_obtinere(macro->corpus, ZEPHYRUM);
    si (body_tok == NIHIL)
    {
        redde;
    }

    /* Si corpus est keyword vel typus, registrare */
    si (_est_keyword_vel_typus(body_tok->genus))
    {
        /* Allocare i32 pro valore */
        keyword_ptr = piscina_allocare(pp->piscina, magnitudo(i32));
        si (keyword_ptr != NIHIL)
        {
            *keyword_ptr = (i32)body_tok->genus;
            tabula_dispersa_inserere(pp->keyword_macros, *macro->titulus, keyword_ptr);
        }
    }
    /* Si corpus est identifier, potentialiter typedef macro (i8 -> char via typedef) */
    alioquin si (body_tok->genus == ARBOR_LEXEMA_IDENTIFICATOR)
    {
        /* Verificare si body identifier est ipse keyword macro */
        chorda* body_name = chorda_internare(pp->intern, body_tok->valor);
        si (body_name != NIHIL)
        {
            si (tabula_dispersa_invenire(pp->keyword_macros, *body_name, &existing_val))
            {
                /* Propagare: si i32 -> int, et i8 -> i32, tunc i8 -> int */
                tabula_dispersa_inserere(pp->keyword_macros, *macro->titulus, existing_val);
            }
        }
    }
}

/* Detectare typedef in token stream et registrare nomen.
 * Detectat patterns:
 *   typedef <type-specifiers> <name>;
 *   nomen <type-specifiers> <name>;
 *
 * Pro simplicitate, assumimus nomen typedef est ultimum identifier ante ;
 */
interior vacuum
_detectare_typedef(
    ArborPraeparator*     pp,
    Xar*                  lexemata,
    i32                   pos_initium,
    i32                   pos_finis)
{
    ArborLexema* lex_first;
    ArborLexema* lex_last_id;
    chorda*      typedef_nomen;
    i32          i;

    si (pp == NIHIL || lexemata == NIHIL || pos_initium >= pos_finis)
    {
        redde;
    }

    /* Verificare si primum token est typedef/nomen keyword */
    lex_first = *(ArborLexema**)xar_obtinere(lexemata, pos_initium);
    si (lex_first == NIHIL)
    {
        redde;
    }

    /* Verificare si est typedef keyword (ARBOR_LEXEMA_TYPEDEF) vel
     * identifier "nomen" (Latin alias pro typedef) */
    si (lex_first->genus != ARBOR_LEXEMA_TYPEDEF)
    {
        /* Verificare si est "nomen" identifier */
        si (lex_first->genus != ARBOR_LEXEMA_IDENTIFICATOR)
        {
            redde;
        }
        si (lex_first->valor.mensura != V ||
            memcmp(lex_first->valor.datum, "nomen", V) != ZEPHYRUM)
        {
            redde;
        }
    }

    /* Invenire nomen typedef
     *
     * Duo casus:
     * 1. Simplex: nomen int MyInt;  -> ultimum identifier ante ;
     * 2. Struct:  nomen structura X { int a; } X;  -> ultimum identifier extra braces
     * 3. Functio pointer: nomen int (*FP)(int);  -> identifier post (*
     *
     * Pro functio pointer, pattern est: ( * IDENTIFIER ) (
     * Si invenitur, nomen est identifier post (*
     */
    lex_last_id = NIHIL;
    {
        i32 brace_depth = ZEPHYRUM;
        i32 paren_depth = ZEPHYRUM;
        b32 in_fn_ptr_name = FALSUM;  /* Post (* */
        b32 fn_ptr_found = FALSUM;
        ArborLexema* fn_ptr_name = NIHIL;
        ArborLexema* prev_lex = NIHIL;

        per (i = pos_initium + I; i < pos_finis; i++)
        {
            ArborLexema* lex = *(ArborLexema**)xar_obtinere(lexemata, i);
            si (lex == NIHIL)
            {
                perge;
            }

            /* Traceare braces */
            si (lex->genus == ARBOR_LEXEMA_BRACE_APERTA)
            {
                brace_depth++;
            }
            alioquin si (lex->genus == ARBOR_LEXEMA_BRACE_CLAUSA)
            {
                brace_depth--;
            }

            /* Traceare parentheses (tantum extra braces) */
            si (brace_depth == ZEPHYRUM)
            {
                si (lex->genus == ARBOR_LEXEMA_PAREN_APERTA)
                {
                    paren_depth++;
                    /* Verificare si (* pattern pro function pointer */
                    si (paren_depth == I)
                    {
                        in_fn_ptr_name = VERUM;
                    }
                }
                alioquin si (lex->genus == ARBOR_LEXEMA_PAREN_CLAUSA)
                {
                    in_fn_ptr_name = FALSUM;
                    paren_depth--;
                }
            }

            /* Detectare function pointer pattern: (* IDENTIFIER ) */
            si (in_fn_ptr_name && !fn_ptr_found && brace_depth == ZEPHYRUM)
            {
                /* Verificare si prev est * et curr est identifier */
                si (prev_lex != NIHIL &&
                    prev_lex->genus == ARBOR_LEXEMA_ASTERISCUS &&
                    lex->genus == ARBOR_LEXEMA_IDENTIFICATOR)
                {
                    fn_ptr_name = lex;
                    fn_ptr_found = VERUM;
                }
            }

            /* Si semicolon ad brace_depth 0, finis declarationis */
            si (lex->genus == ARBOR_LEXEMA_SEMICOLON && brace_depth == ZEPHYRUM)
            {
                frange;
            }

            /* Traceare ultimum identifier (extra braces et extra parens tantum)
             * Solum si non function pointer */
            si (!fn_ptr_found &&
                lex->genus == ARBOR_LEXEMA_IDENTIFICATOR &&
                brace_depth == ZEPHYRUM &&
                paren_depth == ZEPHYRUM)
            {
                lex_last_id = lex;
            }

            prev_lex = lex;
        }

        /* Si function pointer inventum, usa fn_ptr_name */
        si (fn_ptr_found && fn_ptr_name != NIHIL)
        {
            lex_last_id = fn_ptr_name;
        }
    }

    /* Registrare nomen typedef si inventum */
    si (lex_last_id != NIHIL)
    {
        typedef_nomen = chorda_internare(pp->intern, lex_last_id->valor);
        si (typedef_nomen != NIHIL)
        {
            tabula_dispersa_inserere(pp->typedef_nomina, *typedef_nomen,
                (vacuum*)(longus)VERUM);
        }
    }
}

/* ==================================================
 * Functiones Internae - Errores
 * ================================================== */

interior vacuum
_addere_error(
    ArborPraeparator*     pp,
    ArborPPErrorGenus     genus,
    constans character*   nuntius,
    constans character*   via_file,
    i32                   linea,
    i32                   columna)
{
    ArborPPError* err;
    ArborPPError** slotus;

    err = piscina_allocare(pp->piscina, magnitudo(ArborPPError));
    si (err == NIHIL)
    {
        redde;
    }

    err->genus = genus;
    err->linea = linea;
    err->columna = columna;

    /* Copiare nuntius */
    si (nuntius != NIHIL)
    {
        err->nuntius = chorda_ex_literis(nuntius, pp->piscina);
    }
    alioquin
    {
        err->nuntius.datum = NIHIL;
        err->nuntius.mensura = ZEPHYRUM;
    }

    /* Copiare via_file */
    si (via_file != NIHIL)
    {
        err->via_file = chorda_ex_literis(via_file, pp->piscina);
    }
    alioquin
    {
        err->via_file.datum = NIHIL;
        err->via_file.mensura = ZEPHYRUM;
    }

    slotus = xar_addere(pp->errores);
    si (slotus != NIHIL)
    {
        *slotus = err;
    }
}

/* ==================================================
 * Functiones Internae - Origo
 * ================================================== */

interior ArborLexemaOrigo*
_creare_lexema_origo(
    ArborPraeparator*     pp,
    ArborLexema*          lexema,
    ArborOrigo*           origo)
{
    ArborLexemaOrigo* lo;

    lo = piscina_allocare(pp->piscina, magnitudo(ArborLexemaOrigo));
    si (lo == NIHIL)
    {
        redde NIHIL;
    }

    lo->lexema = lexema;
    lo->origo = origo;

    redde lo;
}

/* ==================================================
 * Declarationes Praeviae
 * ================================================== */

interior i32 _invenire_finem_lineae(Xar* lexemata, i32 pos_initium, i32 num);
interior vacuum _expandere_et_emittere(
    ArborPraeparator* pp, ArborLexema* lex, ArborOrigo* origo, Xar* output);
interior b32 _est_in_expansione(ArborPraeparator* pp, chorda* nomen_macro);
interior Xar* _processare_include(
    ArborPraeparator* pp, Xar* lexemata, i32 pos_initium, i32 pos_finis,
    constans character* via_file_currens);

/* ==================================================
 * Functiones Internae - Identificare Directivas
 * ================================================== */

interior ArborDirectivaGenus
_identificare_directiva(ArborLexema* lex, InternamentumChorda* intern)
{
    chorda* str_include;
    chorda* str_define;
    chorda* str_undef;
    chorda* str_if;
    chorda* str_ifdef;
    chorda* str_ifndef;
    chorda* str_elif;
    chorda* str_else;
    chorda* str_endif;
    chorda* str_line;
    chorda* str_error;
    chorda* str_pragma;

    si (lex == NIHIL)
    {
        redde ARBOR_DIRECTIVA_UNKNOWN;
    }

    /* Lexer tokenizat "else" et "if" ut keywords C.
     * Tractare haec specialiter */
    si (lex->genus == ARBOR_LEXEMA_ELSE)
    {
        redde ARBOR_DIRECTIVA_ELSE;
    }
    si (lex->genus == ARBOR_LEXEMA_IF)
    {
        redde ARBOR_DIRECTIVA_IF;
    }

    si (lex->genus != ARBOR_LEXEMA_IDENTIFICATOR)
    {
        redde ARBOR_DIRECTIVA_UNKNOWN;
    }

    /* Internare nomina directivarum pro comparatione */
    str_include = chorda_internare_ex_literis(intern, "include");
    str_define = chorda_internare_ex_literis(intern, "define");
    str_undef = chorda_internare_ex_literis(intern, "undef");
    str_if = chorda_internare_ex_literis(intern, "if");
    str_ifdef = chorda_internare_ex_literis(intern, "ifdef");
    str_ifndef = chorda_internare_ex_literis(intern, "ifndef");
    str_elif = chorda_internare_ex_literis(intern, "elif");
    str_else = chorda_internare_ex_literis(intern, "else");
    str_endif = chorda_internare_ex_literis(intern, "endif");
    str_line = chorda_internare_ex_literis(intern, "line");
    str_error = chorda_internare_ex_literis(intern, "error");
    str_pragma = chorda_internare_ex_literis(intern, "pragma");

    /* Comparare per pointer (quia internatae) */
    si (lex->valor.datum == str_include->datum) { redde ARBOR_DIRECTIVA_INCLUDE; }
    si (lex->valor.datum == str_define->datum)  { redde ARBOR_DIRECTIVA_DEFINE; }
    si (lex->valor.datum == str_undef->datum)   { redde ARBOR_DIRECTIVA_UNDEF; }
    si (lex->valor.datum == str_if->datum)      { redde ARBOR_DIRECTIVA_IF; }
    si (lex->valor.datum == str_ifdef->datum)   { redde ARBOR_DIRECTIVA_IFDEF; }
    si (lex->valor.datum == str_ifndef->datum)  { redde ARBOR_DIRECTIVA_IFNDEF; }
    si (lex->valor.datum == str_elif->datum)    { redde ARBOR_DIRECTIVA_ELIF; }
    si (lex->valor.datum == str_else->datum)    { redde ARBOR_DIRECTIVA_ELSE; }
    si (lex->valor.datum == str_endif->datum)   { redde ARBOR_DIRECTIVA_ENDIF; }
    si (lex->valor.datum == str_line->datum)    { redde ARBOR_DIRECTIVA_LINE; }
    si (lex->valor.datum == str_error->datum)   { redde ARBOR_DIRECTIVA_ERROR; }
    si (lex->valor.datum == str_pragma->datum)  { redde ARBOR_DIRECTIVA_PRAGMA; }

    redde ARBOR_DIRECTIVA_UNKNOWN;
}

/* ==================================================
 * Functiones Internae - Parsere #define
 * ================================================== */

/* Parsere definitio macro ex lexemata
 * pos_initium: positio post "define"
 * pos_finis: finis lineae (exclusive)
 * Redde macro definitio vel NIHIL si error
 */
interior ArborMacroDefinitio*
_parsere_define(
    ArborPraeparator*     pp,
    Xar*                  lexemata,
    i32                   pos_initium,
    i32                   pos_finis,
    constans character*   via_file)
{
    ArborMacroDefinitio* macro;
    ArborLexema* lex_nomen;
    ArborLexema* lex_post_nomen;
    i32 pos;
    i32 pos_corpus;

    (vacuum)via_file;

    si (pos_initium >= pos_finis)
    {
        /* Nihil post "define" */
        redde NIHIL;
    }

    /* Obtinere nomen macro */
    lex_nomen = *(ArborLexema**)xar_obtinere(lexemata, pos_initium);
    si (lex_nomen == NIHIL || lex_nomen->genus != ARBOR_LEXEMA_IDENTIFICATOR)
    {
        /* Nomen macro requiritur */
        redde NIHIL;
    }

    /* Creare definitio */
    macro = piscina_allocare(pp->piscina, magnitudo(ArborMacroDefinitio));
    si (macro == NIHIL)
    {
        redde NIHIL;
    }

    macro->titulus = chorda_internare(pp->intern, lex_nomen->valor);
    macro->est_functio = FALSUM;
    macro->est_variadic = FALSUM;
    macro->parametra = NIHIL;
    macro->linea_def = lex_nomen->linea;
    macro->via_file.datum = NIHIL;
    macro->via_file.mensura = ZEPHYRUM;

    /* Creare corpus */
    macro->corpus = xar_creare(pp->piscina, magnitudo(ArborLexema*));
    si (macro->corpus == NIHIL)
    {
        redde NIHIL;
    }

    pos = pos_initium + I;
    pos_corpus = pos;

    /* Verificare si function-like: ( immediate post nomen (sine spatio) */
    si (pos < pos_finis)
    {
        lex_post_nomen = *(ArborLexema**)xar_obtinere(lexemata, pos);

        /* Function-like si parenthesis immediate sequitur
         * (columna parenthesis == columna finis nominis)
         * Usamus: post_nomen.columna == nomen.columna + nomen.mensura */
        si (lex_post_nomen != NIHIL &&
            lex_post_nomen->genus == ARBOR_LEXEMA_PAREN_APERTA &&
            lex_post_nomen->columna == lex_nomen->columna + lex_nomen->valor.mensura)
        {
            macro->est_functio = VERUM;
            macro->parametra = xar_creare(pp->piscina, magnitudo(chorda*));
            si (macro->parametra == NIHIL)
            {
                redde NIHIL;
            }

            pos++;  /* Saltare ( */

            /* Parsere parametra */
            dum (pos < pos_finis)
            {
                ArborLexema* lex_param;

                lex_param = *(ArborLexema**)xar_obtinere(lexemata, pos);
                si (lex_param == NIHIL)
                {
                    pos++;
                    perge;
                }

                /* Finis parametrorum */
                si (lex_param->genus == ARBOR_LEXEMA_PAREN_CLAUSA)
                {
                    pos++;
                    frange;
                }

                /* Comma inter parametra */
                si (lex_param->genus == ARBOR_LEXEMA_COMMA)
                {
                    pos++;
                    perge;
                }

                /* Ellipsis pro variadic */
                si (lex_param->genus == ARBOR_LEXEMA_ELLIPSIS)
                {
                    macro->est_variadic = VERUM;
                    pos++;
                    perge;
                }

                /* Nomen parametri */
                si (lex_param->genus == ARBOR_LEXEMA_IDENTIFICATOR)
                {
                    chorda* param_internatum;
                    chorda** slotus;

                    param_internatum = chorda_internare(pp->intern, lex_param->valor);
                    slotus = xar_addere(macro->parametra);
                    si (slotus != NIHIL)
                    {
                        *slotus = param_internatum;
                    }
                    pos++;
                    perge;
                }

                /* Alium - errore vel ignorare */
                pos++;
            }

            pos_corpus = pos;
        }
    }

    /* Colligere corpus (omnia lexemata restantia in linea) */
    per (; pos_corpus < pos_finis; pos_corpus++)
    {
        ArborLexema* lex_corpus;
        ArborLexema** slotus;

        lex_corpus = *(ArborLexema**)xar_obtinere(lexemata, pos_corpus);
        si (lex_corpus == NIHIL)
        {
            perge;
        }

        /* Non addere EOF */
        si (lex_corpus->genus == ARBOR_LEXEMA_EOF)
        {
            perge;
        }

        slotus = xar_addere(macro->corpus);
        si (slotus != NIHIL)
        {
            *slotus = lex_corpus;
        }
    }

    redde macro;
}

/* ==================================================
 * Functiones Internae - #if Expression Evaluator
 *
 * Operator precedence (low to high):
 *  1: ||
 *  2: &&
 *  3: |
 *  4: ^
 *  5: &
 *  6: == !=
 *  7: < > <= >=
 *  8: << >>
 *  9: + -
 * 10: * / %
 * 11: unary ! ~ + -
 * ================================================== */

/* Status evaluationis */
nomen structura {
    ArborPraeparator*   pp;
    Xar*                lexemata;
    i32                 pos;
    i32                 pos_finis;
    b32                 habet_errorem;
} ArborEvalStatus;

/* Declarationes praeviae pro recursione */
interior i64 _eval_ternarius(ArborEvalStatus* st);
interior i64 _eval_or_logicus(ArborEvalStatus* st);

interior ArborLexema*
_eval_lexema_currens(ArborEvalStatus* st)
{
    si (st->pos >= st->pos_finis)
    {
        redde NIHIL;
    }
    redde *(ArborLexema**)xar_obtinere(st->lexemata, st->pos);
}

interior vacuum
_eval_avanzare(ArborEvalStatus* st)
{
    si (st->pos < st->pos_finis)
    {
        st->pos++;
    }
}

interior b32
_est_defined_keyword(ArborLexema* lex)
{
    si (lex == NIHIL || lex->genus != ARBOR_LEXEMA_IDENTIFICATOR)
    {
        redde FALSUM;
    }
    si (lex->valor.mensura != VII)
    {
        redde FALSUM;
    }
    redde memcmp(lex->valor.datum, "defined", VII) == ZEPHYRUM;
}

interior i64
_eval_primarius(ArborEvalStatus* st)
{
    ArborLexema* lex;
    i64 valor;

    lex = _eval_lexema_currens(st);
    si (lex == NIHIL)
    {
        st->habet_errorem = VERUM;
        redde ZEPHYRUM;
    }

    /* Integer literal */
    si (lex->genus == ARBOR_LEXEMA_INTEGER)
    {
        i32 i;
        valor = ZEPHYRUM;
        per (i = ZEPHYRUM; i < lex->valor.mensura; i++)
        {
            i8 c = lex->valor.datum[i];
            si (c >= '0' && c <= '9')
            {
                valor = valor * X + (i64)(c - '0');
            }
        }
        _eval_avanzare(st);
        redde valor;
    }

    /* defined(NAME) or defined NAME */
    si (_est_defined_keyword(lex))
    {
        ArborLexema* lex_nomen;
        b32 cum_paren;
        chorda* nomen_internatum;
        b32 est_def;

        _eval_avanzare(st);
        lex = _eval_lexema_currens(st);

        cum_paren = FALSUM;
        si (lex != NIHIL && lex->genus == ARBOR_LEXEMA_PAREN_APERTA)
        {
            cum_paren = VERUM;
            _eval_avanzare(st);
            lex = _eval_lexema_currens(st);
        }

        /* Obtinere nomen macro */
        si (lex == NIHIL || lex->genus != ARBOR_LEXEMA_IDENTIFICATOR)
        {
            st->habet_errorem = VERUM;
            redde ZEPHYRUM;
        }

        lex_nomen = lex;
        _eval_avanzare(st);

        si (cum_paren)
        {
            lex = _eval_lexema_currens(st);
            si (lex == NIHIL || lex->genus != ARBOR_LEXEMA_PAREN_CLAUSA)
            {
                st->habet_errorem = VERUM;
                redde ZEPHYRUM;
            }
            _eval_avanzare(st);
        }

        /* Quaerere si definitum */
        nomen_internatum = chorda_internare(st->pp->intern, lex_nomen->valor);
        si (nomen_internatum == NIHIL)
        {
            redde ZEPHYRUM;
        }

        est_def = tabula_dispersa_continet(st->pp->macros, *nomen_internatum);
        redde est_def ? I : ZEPHYRUM;
    }

    /* Parentheses */
    si (lex->genus == ARBOR_LEXEMA_PAREN_APERTA)
    {
        _eval_avanzare(st);
        valor = _eval_ternarius(st);
        lex = _eval_lexema_currens(st);
        si (lex != NIHIL && lex->genus == ARBOR_LEXEMA_PAREN_CLAUSA)
        {
            _eval_avanzare(st);
        }
        alioquin
        {
            st->habet_errorem = VERUM;
        }
        redde valor;
    }

    /* Unary ! */
    si (lex->genus == ARBOR_LEXEMA_EXCLAMATIO)
    {
        _eval_avanzare(st);
        valor = _eval_primarius(st);
        redde valor == ZEPHYRUM ? I : ZEPHYRUM;
    }

    /* Unary ~ */
    si (lex->genus == ARBOR_LEXEMA_TILDE)
    {
        _eval_avanzare(st);
        valor = _eval_primarius(st);
        redde ~valor;
    }

    /* Unary + */
    si (lex->genus == ARBOR_LEXEMA_PLUS)
    {
        _eval_avanzare(st);
        redde _eval_primarius(st);
    }

    /* Unary - */
    si (lex->genus == ARBOR_LEXEMA_MINUS)
    {
        _eval_avanzare(st);
        redde -_eval_primarius(st);
    }

    /* Identifier - check if defined macro, expand if so, else 0 */
    si (lex->genus == ARBOR_LEXEMA_IDENTIFICATOR)
    {
        chorda* nomen_internatum;
        ArborMacroDefinitio* macro;
        vacuum* macro_ptr;

        nomen_internatum = chorda_internare(st->pp->intern, lex->valor);
        _eval_avanzare(st);

        si (nomen_internatum == NIHIL)
        {
            redde ZEPHYRUM;
        }

        /* Quaerere macro */
        macro_ptr = NIHIL;
        si (tabula_dispersa_invenire(st->pp->macros, *nomen_internatum, &macro_ptr))
        {
            macro = (ArborMacroDefinitio*)macro_ptr;
            si (macro != NIHIL && !macro->est_functio && macro->corpus != NIHIL)
            {
                /* Macro simplex - obtinere primum token si integer */
                i32 num_corpus = xar_numerus(macro->corpus);
                si (num_corpus > ZEPHYRUM)
                {
                    ArborLexema* lex_corpus;
                    lex_corpus = *(ArborLexema**)xar_obtinere(macro->corpus, ZEPHYRUM);
                    si (lex_corpus != NIHIL &&
                        lex_corpus->genus == ARBOR_LEXEMA_INTEGER)
                    {
                        /* Parse integer value */
                        i64 val = ZEPHYRUM;
                        i32 k;
                        per (k = ZEPHYRUM; k < lex_corpus->valor.mensura; k++)
                        {
                            i8 c = lex_corpus->valor.datum[k];
                            si (c >= '0' && c <= '9')
                            {
                                val = val * X + (i64)(c - '0');
                            }
                        }
                        redde val;
                    }
                }
            }
        }

        redde ZEPHYRUM;
    }

    st->habet_errorem = VERUM;
    redde ZEPHYRUM;
}

interior i64
_eval_multiplicativus(ArborEvalStatus* st)
{
    i64 sinistra;
    ArborLexema* lex;

    sinistra = _eval_primarius(st);

    dum (!st->habet_errorem)
    {
        i64 dextra;
        lex = _eval_lexema_currens(st);
        si (lex == NIHIL)
        {
            frange;
        }

        si (lex->genus == ARBOR_LEXEMA_ASTERISCUS)
        {
            _eval_avanzare(st);
            dextra = _eval_primarius(st);
            sinistra = sinistra * dextra;
        }
        alioquin si (lex->genus == ARBOR_LEXEMA_SOLIDUS)
        {
            _eval_avanzare(st);
            dextra = _eval_primarius(st);
            si (dextra == ZEPHYRUM)
            {
                st->habet_errorem = VERUM;
                redde ZEPHYRUM;
            }
            sinistra = sinistra / dextra;
        }
        alioquin si (lex->genus == ARBOR_LEXEMA_PERCENTUM)
        {
            _eval_avanzare(st);
            dextra = _eval_primarius(st);
            si (dextra == ZEPHYRUM)
            {
                st->habet_errorem = VERUM;
                redde ZEPHYRUM;
            }
            sinistra = sinistra % dextra;
        }
        alioquin
        {
            frange;
        }
    }

    redde sinistra;
}

interior i64
_eval_additivus(ArborEvalStatus* st)
{
    i64 sinistra;
    ArborLexema* lex;

    sinistra = _eval_multiplicativus(st);

    dum (!st->habet_errorem)
    {
        i64 dextra;
        lex = _eval_lexema_currens(st);
        si (lex == NIHIL)
        {
            frange;
        }

        si (lex->genus == ARBOR_LEXEMA_PLUS)
        {
            _eval_avanzare(st);
            dextra = _eval_multiplicativus(st);
            sinistra = sinistra + dextra;
        }
        alioquin si (lex->genus == ARBOR_LEXEMA_MINUS)
        {
            _eval_avanzare(st);
            dextra = _eval_multiplicativus(st);
            sinistra = sinistra - dextra;
        }
        alioquin
        {
            frange;
        }
    }

    redde sinistra;
}

interior i64
_eval_shift(ArborEvalStatus* st)
{
    i64 sinistra;
    ArborLexema* lex;

    sinistra = _eval_additivus(st);

    dum (!st->habet_errorem)
    {
        i64 dextra;
        lex = _eval_lexema_currens(st);
        si (lex == NIHIL)
        {
            frange;
        }

        si (lex->genus == ARBOR_LEXEMA_SINISTRUM)
        {
            _eval_avanzare(st);
            dextra = _eval_additivus(st);
            sinistra = sinistra << dextra;
        }
        alioquin si (lex->genus == ARBOR_LEXEMA_DEXTRUM)
        {
            _eval_avanzare(st);
            dextra = _eval_additivus(st);
            sinistra = sinistra >> dextra;
        }
        alioquin
        {
            frange;
        }
    }

    redde sinistra;
}

interior i64
_eval_relatio(ArborEvalStatus* st)
{
    i64 sinistra;
    ArborLexema* lex;

    sinistra = _eval_shift(st);

    dum (!st->habet_errorem)
    {
        i64 dextra;
        lex = _eval_lexema_currens(st);
        si (lex == NIHIL)
        {
            frange;
        }

        si (lex->genus == ARBOR_LEXEMA_MINOR)
        {
            _eval_avanzare(st);
            dextra = _eval_shift(st);
            sinistra = sinistra < dextra ? I : ZEPHYRUM;
        }
        alioquin si (lex->genus == ARBOR_LEXEMA_MAIOR)
        {
            _eval_avanzare(st);
            dextra = _eval_shift(st);
            sinistra = sinistra > dextra ? I : ZEPHYRUM;
        }
        alioquin si (lex->genus == ARBOR_LEXEMA_MINOR_AEQ)
        {
            _eval_avanzare(st);
            dextra = _eval_shift(st);
            sinistra = sinistra <= dextra ? I : ZEPHYRUM;
        }
        alioquin si (lex->genus == ARBOR_LEXEMA_MAIOR_AEQ)
        {
            _eval_avanzare(st);
            dextra = _eval_shift(st);
            sinistra = sinistra >= dextra ? I : ZEPHYRUM;
        }
        alioquin
        {
            frange;
        }
    }

    redde sinistra;
}

interior i64
_eval_aequalitas(ArborEvalStatus* st)
{
    i64 sinistra;
    ArborLexema* lex;

    sinistra = _eval_relatio(st);

    dum (!st->habet_errorem)
    {
        i64 dextra;
        lex = _eval_lexema_currens(st);
        si (lex == NIHIL)
        {
            frange;
        }

        si (lex->genus == ARBOR_LEXEMA_AEQUALIS)
        {
            _eval_avanzare(st);
            dextra = _eval_relatio(st);
            sinistra = sinistra == dextra ? I : ZEPHYRUM;
        }
        alioquin si (lex->genus == ARBOR_LEXEMA_NON_AEQUALIS)
        {
            _eval_avanzare(st);
            dextra = _eval_relatio(st);
            sinistra = sinistra != dextra ? I : ZEPHYRUM;
        }
        alioquin
        {
            frange;
        }
    }

    redde sinistra;
}

interior i64
_eval_and_bit(ArborEvalStatus* st)
{
    i64 sinistra;
    ArborLexema* lex;

    sinistra = _eval_aequalitas(st);

    dum (!st->habet_errorem)
    {
        i64 dextra;
        lex = _eval_lexema_currens(st);
        si (lex == NIHIL || lex->genus != ARBOR_LEXEMA_AMPERSAND)
        {
            frange;
        }

        /* Non && (and logicus) */
        {
            ArborLexema* lex_next;
            si (st->pos + I < st->pos_finis)
            {
                lex_next = *(ArborLexema**)xar_obtinere(st->lexemata, st->pos + I);
                si (lex_next != NIHIL && lex_next->genus == ARBOR_LEXEMA_AMPERSAND)
                {
                    frange;
                }
            }
        }

        _eval_avanzare(st);
        dextra = _eval_aequalitas(st);
        sinistra = sinistra & dextra;
    }

    redde sinistra;
}

interior i64
_eval_xor_bit(ArborEvalStatus* st)
{
    i64 sinistra;
    ArborLexema* lex;

    sinistra = _eval_and_bit(st);

    dum (!st->habet_errorem)
    {
        i64 dextra;
        lex = _eval_lexema_currens(st);
        si (lex == NIHIL || lex->genus != ARBOR_LEXEMA_CARET)
        {
            frange;
        }

        _eval_avanzare(st);
        dextra = _eval_and_bit(st);
        sinistra = sinistra ^ dextra;
    }

    redde sinistra;
}

interior i64
_eval_or_bit(ArborEvalStatus* st)
{
    i64 sinistra;
    ArborLexema* lex;

    sinistra = _eval_xor_bit(st);

    dum (!st->habet_errorem)
    {
        i64 dextra;
        lex = _eval_lexema_currens(st);
        si (lex == NIHIL || lex->genus != ARBOR_LEXEMA_PIPA)
        {
            frange;
        }

        /* Non || (or logicus) */
        {
            ArborLexema* lex_next;
            si (st->pos + I < st->pos_finis)
            {
                lex_next = *(ArborLexema**)xar_obtinere(st->lexemata, st->pos + I);
                si (lex_next != NIHIL && lex_next->genus == ARBOR_LEXEMA_PIPA)
                {
                    frange;
                }
            }
        }

        _eval_avanzare(st);
        dextra = _eval_xor_bit(st);
        sinistra = sinistra | dextra;
    }

    redde sinistra;
}

interior i64
_eval_and_logicus(ArborEvalStatus* st)
{
    i64 sinistra;
    ArborLexema* lex;

    sinistra = _eval_or_bit(st);

    dum (!st->habet_errorem)
    {
        i64 dextra;
        lex = _eval_lexema_currens(st);
        si (lex == NIHIL || lex->genus != ARBOR_LEXEMA_DUAMPERSAND)
        {
            frange;
        }

        _eval_avanzare(st);
        dextra = _eval_or_bit(st);
        sinistra = (sinistra && dextra) ? I : ZEPHYRUM;
    }

    redde sinistra;
}

interior i64
_eval_or_logicus(ArborEvalStatus* st)
{
    i64 sinistra;
    ArborLexema* lex;

    sinistra = _eval_and_logicus(st);

    dum (!st->habet_errorem)
    {
        i64 dextra;
        lex = _eval_lexema_currens(st);
        si (lex == NIHIL || lex->genus != ARBOR_LEXEMA_DUPIPA)
        {
            frange;
        }

        _eval_avanzare(st);
        dextra = _eval_and_logicus(st);
        sinistra = (sinistra || dextra) ? I : ZEPHYRUM;
    }

    redde sinistra;
}

interior i64
_eval_ternarius(ArborEvalStatus* st)
{
    i64 conditio;
    ArborLexema* lex;

    conditio = _eval_or_logicus(st);

    lex = _eval_lexema_currens(st);
    si (lex != NIHIL && lex->genus == ARBOR_LEXEMA_QUAESTIO)
    {
        i64 valor_verum;
        i64 valor_falsum;

        _eval_avanzare(st);
        valor_verum = _eval_ternarius(st);

        lex = _eval_lexema_currens(st);
        si (lex != NIHIL && lex->genus == ARBOR_LEXEMA_COLON)
        {
            _eval_avanzare(st);
            valor_falsum = _eval_ternarius(st);
            redde conditio ? valor_verum : valor_falsum;
        }
        alioquin
        {
            st->habet_errorem = VERUM;
            redde ZEPHYRUM;
        }
    }

    redde conditio;
}

interior i64
_evaluare_expressio_if(ArborPraeparator* pp, Xar* lexemata, i32 pos, i32 pos_finis)
{
    ArborEvalStatus st;
    i64 fructus;

    st.pp = pp;
    st.lexemata = lexemata;
    st.pos = pos;
    st.pos_finis = pos_finis;
    st.habet_errorem = FALSUM;

    fructus = _eval_ternarius(&st);

    si (st.habet_errorem)
    {
        redde ZEPHYRUM;
    }

    redde fructus;
}

/* ==================================================
 * Functiones Internae - Processare Directivas
 * ================================================== */

interior i32
_processare_directiva(
    ArborPraeparator*     pp,
    Xar*                  lexemata,
    i32                   pos_hash,
    i32                   num,
    constans character*   via_file,
    Xar*                  fructus)
{
    ArborLexema* lex_directiva;
    ArborDirectivaGenus genus;
    i32 pos;
    i32 pos_finis;

    pos = pos_hash + I;  /* Saltare # */

    /* Obtinere finem lineae */
    pos_finis = _invenire_finem_lineae(lexemata, pos, num);

    si (pos >= pos_finis)
    {
        /* Linea vacua post # - saltare */
        redde pos_finis;
    }

    /* Obtinere nomen directivae */
    lex_directiva = *(ArborLexema**)xar_obtinere(lexemata, pos);
    si (lex_directiva == NIHIL)
    {
        redde pos_finis;
    }

    genus = _identificare_directiva(lex_directiva, pp->intern);
    pos++;  /* Saltare nomen directivae */

    commutatio (genus)
    {
        casus ARBOR_DIRECTIVA_INCLUDE:
        {
            /* Processare #include */
            si (pp->cond_activus)
            {
                Xar* lexemata_inclusa;

                lexemata_inclusa = _processare_include(
                    pp, lexemata, pos, pos_finis, via_file);

                si (lexemata_inclusa != NIHIL && fructus != NIHIL)
                {
                    /* Addere lexemata inclusa ad output */
                    i32 j;
                    i32 num_inc = xar_numerus(lexemata_inclusa);
                    per (j = ZEPHYRUM; j < num_inc; j++)
                    {
                        ArborLexemaOrigo* lo_inc;
                        ArborLexemaOrigo** slotus;

                        lo_inc = *(ArborLexemaOrigo**)xar_obtinere(lexemata_inclusa, j);
                        si (lo_inc != NIHIL)
                        {
                            slotus = xar_addere(fructus);
                            si (slotus != NIHIL)
                            {
                                *slotus = lo_inc;
                            }
                        }
                    }
                }
            }
            frange;
        }

        casus ARBOR_DIRECTIVA_DEFINE:
        {
            /* Processare #define */
            si (pp->cond_activus)
            {
                ArborMacroDefinitio* macro;
                chorda nomen_ch;

                macro = _parsere_define(pp, lexemata, pos, pos_finis, via_file);
                si (macro != NIHIL)
                {
                    nomen_ch = *macro->titulus;
                    tabula_dispersa_inserere(pp->macros, nomen_ch, macro);

                    /* Registrare si keyword macro */
                    _registrare_keyword_macro(pp, macro);
                }
            }
            frange;
        }

        casus ARBOR_DIRECTIVA_UNDEF:
        {
            /* Processare #undef */
            si (pp->cond_activus)
            {
                ArborLexema* lex_nomen;

                si (pos < pos_finis)
                {
                    lex_nomen = *(ArborLexema**)xar_obtinere(lexemata, pos);
                    si (lex_nomen != NIHIL &&
                        lex_nomen->genus == ARBOR_LEXEMA_IDENTIFICATOR)
                    {
                        chorda* nomen_internatum;
                        nomen_internatum = chorda_internare(pp->intern, lex_nomen->valor);
                        si (nomen_internatum != NIHIL)
                        {
                            tabula_dispersa_delere(pp->macros, *nomen_internatum);
                        }
                    }
                }
            }
            frange;
        }

        casus ARBOR_DIRECTIVA_IFDEF:
        casus ARBOR_DIRECTIVA_IFNDEF:
        {
            /* Processare #ifdef / #ifndef */
            ArborCondStatus* status_novus;
            ArborLexema* lex_nomen;
            b32 est_definitum;
            b32 ramus_activus;

            status_novus = xar_addere(pp->cond_acervus);
            si (status_novus == NIHIL)
            {
                frange;
            }

            /* Pater activus est status currens ante hanc directiva */
            status_novus->pater_activus = pp->cond_activus;
            status_novus->vidit_verum = FALSUM;
            status_novus->currens_activus = FALSUM;

            /* Si pater non activus, hic ramus non potest esse activus */
            si (!pp->cond_activus)
            {
                frange;
            }

            /* Obtinere nomen macro */
            est_definitum = FALSUM;
            si (pos < pos_finis)
            {
                lex_nomen = *(ArborLexema**)xar_obtinere(lexemata, pos);
                si (lex_nomen != NIHIL &&
                    lex_nomen->genus == ARBOR_LEXEMA_IDENTIFICATOR)
                {
                    chorda* nomen_internatum;
                    nomen_internatum = chorda_internare(pp->intern, lex_nomen->valor);
                    si (nomen_internatum != NIHIL)
                    {
                        est_definitum = tabula_dispersa_continet(
                            pp->macros, *nomen_internatum);
                    }
                }
            }

            /* #ifdef: activus si definitum; #ifndef: activus si NON definitum */
            si (genus == ARBOR_DIRECTIVA_IFDEF)
            {
                ramus_activus = est_definitum;
            }
            alioquin
            {
                ramus_activus = !est_definitum;
            }

            status_novus->currens_activus = ramus_activus;
            status_novus->vidit_verum = ramus_activus;
            pp->cond_activus = ramus_activus;

            frange;
        }

        casus ARBOR_DIRECTIVA_ELSE:
        {
            /* Processare #else */
            i32 num_acervus;
            ArborCondStatus* status;

            num_acervus = xar_numerus(pp->cond_acervus);
            si (num_acervus == ZEPHYRUM)
            {
                /* Error: #else sine #if/#ifdef/#ifndef */
                _addere_error(pp, ARBOR_PP_ERROR_UNEXPECTED_DIRECTIVE,
                    "#else sine #if", via_file, lex_directiva->linea, I);
                frange;
            }

            status = (ArborCondStatus*)xar_obtinere(pp->cond_acervus, num_acervus - I);

            /* Si pater non activus, nihil mutare */
            si (!status->pater_activus)
            {
                frange;
            }

            /* #else activus solum si nondum vidimus verum ramum */
            si (status->vidit_verum)
            {
                status->currens_activus = FALSUM;
                pp->cond_activus = FALSUM;
            }
            alioquin
            {
                status->currens_activus = VERUM;
                status->vidit_verum = VERUM;
                pp->cond_activus = VERUM;
            }

            frange;
        }

        casus ARBOR_DIRECTIVA_ENDIF:
        {
            /* Processare #endif */
            i32 num_acervus;
            ArborCondStatus* status;

            num_acervus = xar_numerus(pp->cond_acervus);
            si (num_acervus == ZEPHYRUM)
            {
                /* Error: #endif sine #if/#ifdef/#ifndef */
                _addere_error(pp, ARBOR_PP_ERROR_UNEXPECTED_DIRECTIVE,
                    "#endif sine #if", via_file, lex_directiva->linea, I);
                frange;
            }

            /* Obtinere status ad restaurandum pater */
            status = (ArborCondStatus*)xar_obtinere(pp->cond_acervus, num_acervus - I);

            /* Restaurare pater activus */
            pp->cond_activus = status->pater_activus;

            /* Removere de acervo */
            xar_removere_ultimum(pp->cond_acervus);

            frange;
        }

        casus ARBOR_DIRECTIVA_IF:
        {
            /* Processare #if */
            ArborCondStatus* status_novus;
            i64 valor_expr;
            b32 ramus_activus;

            status_novus = xar_addere(pp->cond_acervus);
            si (status_novus == NIHIL)
            {
                frange;
            }

            /* Pater activus est status currens ante hanc directiva */
            status_novus->pater_activus = pp->cond_activus;
            status_novus->vidit_verum = FALSUM;
            status_novus->currens_activus = FALSUM;

            /* Si pater non activus, hic ramus non potest esse activus */
            si (!pp->cond_activus)
            {
                frange;
            }

            /* Evaluare expressionem */
            valor_expr = _evaluare_expressio_if(pp, lexemata, pos, pos_finis);
            ramus_activus = valor_expr != ZEPHYRUM;

            status_novus->currens_activus = ramus_activus;
            status_novus->vidit_verum = ramus_activus;
            pp->cond_activus = ramus_activus;

            frange;
        }

        casus ARBOR_DIRECTIVA_ELIF:
        {
            /* Processare #elif */
            i32 num_acervus;
            ArborCondStatus* status;
            i64 valor_expr;

            num_acervus = xar_numerus(pp->cond_acervus);
            si (num_acervus == ZEPHYRUM)
            {
                /* Error: #elif sine #if */
                _addere_error(pp, ARBOR_PP_ERROR_UNEXPECTED_DIRECTIVE,
                    "#elif sine #if", via_file, lex_directiva->linea, I);
                frange;
            }

            status = (ArborCondStatus*)xar_obtinere(pp->cond_acervus, num_acervus - I);

            /* Si pater non activus, nihil mutare */
            si (!status->pater_activus)
            {
                frange;
            }

            /* Si jam vidimus verum, non activus */
            si (status->vidit_verum)
            {
                status->currens_activus = FALSUM;
                pp->cond_activus = FALSUM;
                frange;
            }

            /* Evaluare expressionem */
            valor_expr = _evaluare_expressio_if(pp, lexemata, pos, pos_finis);
            si (valor_expr != ZEPHYRUM)
            {
                status->currens_activus = VERUM;
                status->vidit_verum = VERUM;
                pp->cond_activus = VERUM;
            }
            alioquin
            {
                status->currens_activus = FALSUM;
                pp->cond_activus = FALSUM;
            }

            frange;
        }

        ordinarius:
            frange;
    }

    redde pos_finis;
}

/* ==================================================
 * Functiones Internae - Utilitas Lexemata
 * ================================================== */

interior b32
_est_hash(ArborLexema* lex)
{
    si (lex == NIHIL)
    {
        redde FALSUM;
    }

    redde lex->genus == ARBOR_LEXEMA_HASH;
}

/* ==================================================
 * Functiones Internae - Include Processing
 * ================================================== */

/* Verificare si filum jam inclusum (protectio contra recursione) */
interior b32
_est_jam_inclusum(ArborPraeparator* pp, chorda* via_file)
{
    i32 i;
    i32 num;

    si (pp->fila_inclusa == NIHIL || via_file == NIHIL)
    {
        redde FALSUM;
    }

    num = xar_numerus(pp->fila_inclusa);
    per (i = ZEPHYRUM; i < num; i++)
    {
        chorda* via;
        via = *(chorda**)xar_obtinere(pp->fila_inclusa, i);
        si (via != NIHIL && via->datum == via_file->datum)
        {
            redde VERUM;
        }
    }

    redde FALSUM;
}

/* Addere filum ad lista inclusorum */
interior vacuum
_signare_inclusum(ArborPraeparator* pp, chorda* via_file)
{
    chorda** slotus;

    si (pp->fila_inclusa == NIHIL || via_file == NIHIL)
    {
        redde;
    }

    slotus = xar_addere(pp->fila_inclusa);
    si (slotus != NIHIL)
    {
        *slotus = via_file;
    }
}

/* Resolvere via include
 * est_angularis: VERUM pro <file>, FALSUM pro "file"
 * via_currens: directorium fili currentis (pro "file" quaestione)
 */
interior chorda*
_resolvere_include_path(
    ArborPraeparator*     pp,
    chorda                filename,
    b32                   est_angularis,
    chorda                via_currens)
{
    i32 i;
    i32 num_viae;
    chorda partes[II];
    chorda via_candidata;
    chorda* via_internata;

    /* Pro "file" includes, primum quaerere in directorium currens */
    si (!est_angularis && via_currens.mensura > ZEPHYRUM)
    {
        partes[ZEPHYRUM] = via_currens;
        partes[I] = filename;
        via_candidata = via_iungere(partes, II, pp->piscina);

        si (via_candidata.mensura > ZEPHYRUM)
        {
            /* Verificare si filum existit - debemus convertere ad C string */
            character* via_cstr;
            via_cstr = piscina_allocare(pp->piscina, via_candidata.mensura + I);
            si (via_cstr != NIHIL)
            {
                memcpy(via_cstr, via_candidata.datum, (size_t)via_candidata.mensura);
                via_cstr[via_candidata.mensura] = '\0';

                si (filum_existit(via_cstr))
                {
                    via_internata = chorda_internare(pp->intern, via_candidata);
                    redde via_internata;
                }
            }
        }
    }

    /* Quaerere in include paths */
    num_viae = xar_numerus(pp->viae_include);
    per (i = ZEPHYRUM; i < num_viae; i++)
    {
        chorda* via_include;
        character* via_cstr;

        via_include = *(chorda**)xar_obtinere(pp->viae_include, i);
        si (via_include == NIHIL)
        {
            perge;
        }

        partes[ZEPHYRUM] = *via_include;
        partes[I] = filename;
        via_candidata = via_iungere(partes, II, pp->piscina);

        si (via_candidata.mensura > ZEPHYRUM)
        {
            via_cstr = piscina_allocare(pp->piscina, via_candidata.mensura + I);
            si (via_cstr != NIHIL)
            {
                memcpy(via_cstr, via_candidata.datum, (size_t)via_candidata.mensura);
                via_cstr[via_candidata.mensura] = '\0';

                si (filum_existit(via_cstr))
                {
                    via_internata = chorda_internare(pp->intern, via_candidata);
                    redde via_internata;
                }
            }
        }
    }

    /* Non inventum */
    redde NIHIL;
}

/* Creare ArborOrigo pro include */
interior ArborOrigo*
_creare_origo_include(
    ArborPraeparator*     pp,
    chorda*               via_file,
    i32                   linea,
    i32                   columna,
    ArborOrigo*           origo_pater)
{
    ArborOrigo* origo;

    origo = piscina_allocare(pp->piscina, magnitudo(ArborOrigo));
    si (origo == NIHIL)
    {
        redde NIHIL;
    }

    origo->nomen_macro = NIHIL;  /* Non macro, sed include */
    origo->via_file = via_file;
    origo->linea_originalis = linea;
    origo->columna_originalis = columna;
    origo->expansio_index = pp->profunditas_expansionis;
    origo->pater = origo_pater;

    redde origo;
}

/* Processare #include et reddere lexemata inclusa */
interior Xar*
_processare_include(
    ArborPraeparator*     pp,
    Xar*                  lexemata,
    i32                   pos_post_include,
    i32                   pos_finis,
    constans character*   via_file_currens)
{
    ArborLexema* lex_path;
    chorda filename;
    b32 est_angularis;
    chorda* via_resoluta;
    chorda via_currens_dir;
    chorda contentum;
    ArborLexator* lexator;
    Xar* lexemata_inclusa;
    Xar* fructus_inclusa;
    character* via_cstr;
    ArborOrigo* origo_include;
    Xar* output;
    i32 i;
    i32 num;

    si (pos_post_include >= pos_finis)
    {
        redde NIHIL;
    }

    /* Obtinere lexema viae */
    lex_path = *(ArborLexema**)xar_obtinere(lexemata, pos_post_include);
    si (lex_path == NIHIL)
    {
        redde NIHIL;
    }

    /* Parsere filename - potest esse <file> vel "file" */
    si (lex_path->genus == ARBOR_LEXEMA_STRING_LIT)
    {
        /* "file" - removere quotes */
        est_angularis = FALSUM;
        si (lex_path->valor.mensura >= II)
        {
            filename.datum = lex_path->valor.datum + I;
            filename.mensura = lex_path->valor.mensura - II;
        }
        alioquin
        {
            redde NIHIL;
        }
    }
    alioquin si (lex_path->genus == ARBOR_LEXEMA_MINOR)
    {
        /* <file> - colligere tokens usque ad > */
        i32 j;
        i32 start_pos;
        i32 end_pos;
        i32 total_len;
        i8* buf;
        i8* p;

        est_angularis = VERUM;
        start_pos = pos_post_include + I;
        end_pos = start_pos;

        /* Invenire > */
        per (j = start_pos; j < pos_finis; j++)
        {
            ArborLexema* lex_j;
            lex_j = *(ArborLexema**)xar_obtinere(lexemata, j);
            si (lex_j != NIHIL && lex_j->genus == ARBOR_LEXEMA_MAIOR)
            {
                end_pos = j;
                frange;
            }
        }

        si (end_pos <= start_pos)
        {
            redde NIHIL;
        }

        /* Computare longitudinem totalem et concatenare tokens */
        total_len = ZEPHYRUM;
        per (j = start_pos; j < end_pos; j++)
        {
            ArborLexema* lex_j;
            lex_j = *(ArborLexema**)xar_obtinere(lexemata, j);
            si (lex_j != NIHIL)
            {
                total_len += lex_j->valor.mensura;
            }
        }

        si (total_len == ZEPHYRUM)
        {
            redde NIHIL;
        }

        /* Allocare et concatenare */
        buf = piscina_allocare(pp->piscina, total_len);
        si (buf == NIHIL)
        {
            redde NIHIL;
        }

        p = buf;
        per (j = start_pos; j < end_pos; j++)
        {
            ArborLexema* lex_j;
            lex_j = *(ArborLexema**)xar_obtinere(lexemata, j);
            si (lex_j != NIHIL && lex_j->valor.mensura > ZEPHYRUM)
            {
                memcpy(p, lex_j->valor.datum, (size_t)lex_j->valor.mensura);
                p += lex_j->valor.mensura;
            }
        }

        filename.datum = buf;
        filename.mensura = total_len;
    }
    alioquin
    {
        /* Genus ignotum */
        redde NIHIL;
    }

    /* Obtinere directorium fili currentis */
    si (via_file_currens != NIHIL)
    {
        chorda via_ch;
        via_ch = chorda_ex_literis(via_file_currens, pp->piscina);
        via_currens_dir = via_directorium(via_ch, pp->piscina);
    }
    alioquin
    {
        via_currens_dir.datum = NIHIL;
        via_currens_dir.mensura = ZEPHYRUM;
    }

    /* Resolvere via */
    via_resoluta = _resolvere_include_path(pp, filename, est_angularis, via_currens_dir);
    si (via_resoluta == NIHIL)
    {
        /* Filum non inventum */
        _addere_error(pp, ARBOR_PP_ERROR_FILE_NOT_FOUND,
            "include file non inventum", via_file_currens,
            lex_path->linea, lex_path->columna);
        redde NIHIL;
    }

    /* Verificare si jam inclusum */
    si (_est_jam_inclusum(pp, via_resoluta))
    {
        /* Jam processum - reddere lista vacua */
        output = xar_creare(pp->piscina, magnitudo(ArborLexemaOrigo*));
        redde output;
    }

    /* Signare ut inclusum */
    _signare_inclusum(pp, via_resoluta);

    /* Legere filum */
    via_cstr = piscina_allocare(pp->piscina, via_resoluta->mensura + I);
    si (via_cstr == NIHIL)
    {
        redde NIHIL;
    }
    memcpy(via_cstr, via_resoluta->datum, (size_t)via_resoluta->mensura);
    via_cstr[via_resoluta->mensura] = '\0';

    contentum = filum_legere_totum(via_cstr, pp->piscina);
    si (contentum.datum == NIHIL)
    {
        _addere_error(pp, ARBOR_PP_ERROR_FILE_NOT_FOUND,
            "non potest legere include file", via_file_currens,
            lex_path->linea, lex_path->columna);
        redde NIHIL;
    }

    /* Lexere filum inclusum */
    lexator = arbor_lexator_creare(
        pp->piscina,
        pp->intern,
        (constans character*)contentum.datum,
        contentum.mensura);

    si (lexator == NIHIL)
    {
        redde NIHIL;
    }

    lexemata_inclusa = arbor_lexema_omnia(lexator);
    si (lexemata_inclusa == NIHIL)
    {
        redde NIHIL;
    }

    /* Creare origo pro include */
    origo_include = _creare_origo_include(pp, via_resoluta,
        lex_path->linea, lex_path->columna, pp->origo_currens);

    /* Verificare profunditatem maximam */
    si (pp->profunditas_expansionis >= ARBOR_MAX_INCLUDE_PROFUNDITAS)
    {
        _addere_error(pp, ARBOR_PP_ERROR_INCLUDE_DEPTH,
            "maximum include depth exceeded", via_file_currens,
            lex_path->linea, lex_path->columna);
        redde NIHIL;
    }

    /* Incrementare profunditatem */
    pp->profunditas_expansionis++;

    /* Recursive processare filum inclusum */
    fructus_inclusa = arbor_praeparator_processare_lexemata(pp, lexemata_inclusa, via_cstr);

    /* Decrementare profunditatem */
    pp->profunditas_expansionis--;

    si (fructus_inclusa == NIHIL)
    {
        redde NIHIL;
    }

    /* Actualizare origo pro omnia lexemata inclusa */
    output = xar_creare(pp->piscina, magnitudo(ArborLexemaOrigo*));
    si (output == NIHIL)
    {
        redde NIHIL;
    }

    num = xar_numerus(fructus_inclusa);
    per (i = ZEPHYRUM; i < num; i++)
    {
        ArborLexemaOrigo* lo;
        ArborLexemaOrigo** slotus;

        lo = *(ArborLexemaOrigo**)xar_obtinere(fructus_inclusa, i);
        si (lo != NIHIL)
        {
            /* Non emittere EOF de include files */
            si (lo->lexema != NIHIL && lo->lexema->genus == ARBOR_LEXEMA_EOF)
            {
                perge;
            }

            /* Si origo est NIHIL, ponere origo_include */
            si (lo->origo == NIHIL)
            {
                lo->origo = origo_include;
            }
            alioquin
            {
                /* Traversare ad radicem et ponere pater */
                ArborOrigo* o = lo->origo;
                dum (o->pater != NIHIL)
                {
                    o = o->pater;
                }
                o->pater = origo_include;
            }

            slotus = xar_addere(output);
            si (slotus != NIHIL)
            {
                *slotus = lo;
            }
        }
    }

    redde output;
}

/* ==================================================
 * Functiones Internae - Macro Expansion
 * ================================================== */

/* Verificare si macro jam in expansione (recursio protectio) */
interior b32
_est_in_expansione(ArborPraeparator* pp, chorda* nomen_macro)
{
    i32 i;
    i32 num;

    si (pp->macro_in_expansione == NIHIL)
    {
        redde FALSUM;
    }

    num = xar_numerus(pp->macro_in_expansione);
    per (i = ZEPHYRUM; i < num; i++)
    {
        chorda* n;
        n = *(chorda**)xar_obtinere(pp->macro_in_expansione, i);
        si (n != NIHIL && n->datum == nomen_macro->datum)
        {
            redde VERUM;  /* Jam in expansione */
        }
    }

    redde FALSUM;
}

/* Addere macro ad acervum expansionis */
interior vacuum
_push_expansio(ArborPraeparator* pp, chorda* nomen_macro)
{
    chorda** slotus;

    si (pp->macro_in_expansione == NIHIL)
    {
        redde;
    }

    slotus = xar_addere(pp->macro_in_expansione);
    si (slotus != NIHIL)
    {
        *slotus = nomen_macro;
    }
}

/* Removere macro de acervo expansionis */
interior vacuum
_pop_expansio(ArborPraeparator* pp)
{
    si (pp->macro_in_expansione != NIHIL &&
        xar_numerus(pp->macro_in_expansione) > ZEPHYRUM)
    {
        xar_removere_ultimum(pp->macro_in_expansione);
    }
}

/* Creare ArborOrigo pro macro expansion */
interior ArborOrigo*
_creare_origo_macro(
    ArborPraeparator*     pp,
    chorda*               nomen_macro,
    ArborLexema*          lex_invocatio,
    ArborOrigo*           origo_pater)
{
    ArborOrigo* origo;

    origo = piscina_allocare(pp->piscina, magnitudo(ArborOrigo));
    si (origo == NIHIL)
    {
        redde NIHIL;
    }

    origo->nomen_macro = nomen_macro;
    origo->via_file = NIHIL;  /* TODO: populate */
    origo->linea_originalis = (lex_invocatio != NIHIL) ? lex_invocatio->linea : ZEPHYRUM;
    origo->columna_originalis = (lex_invocatio != NIHIL) ? lex_invocatio->columna : ZEPHYRUM;
    origo->expansio_index = pp->profunditas_expansionis;
    origo->pater = origo_pater;

    redde origo;
}

/* ==================================================
 * Functiones Internae - Function-like Macro Expansion
 * ================================================== */

/* Colligere argumenta pro function-like macro invocatione
 * Redde numerum tokenorum consumptorum (includendo '(' et ')')
 * argumenta est Xar<Xar<ArborLexema*>*> - array of argument token arrays */
interior i32
_colligere_argumenta(
    ArborPraeparator* pp,
    Xar*              lexemata,
    i32               pos_paren,
    i32               num,
    Xar*              argumenta)
{
    i32 pos;
    i32 paren_depth;
    Xar* arg_currens;
    ArborLexema* lex;

    /* Verificare '(' */
    lex = *(ArborLexema**)xar_obtinere(lexemata, pos_paren);
    si (lex == NIHIL || lex->genus != ARBOR_LEXEMA_PAREN_APERTA)
    {
        redde ZEPHYRUM;
    }

    pos = pos_paren + I;
    paren_depth = I;

    /* Creare primum argumentum */
    arg_currens = xar_creare(pp->piscina, magnitudo(ArborLexema*));
    si (arg_currens == NIHIL)
    {
        redde ZEPHYRUM;
    }

    dum (pos < num && paren_depth > ZEPHYRUM)
    {
        ArborLexema** slot;

        lex = *(ArborLexema**)xar_obtinere(lexemata, pos);
        si (lex == NIHIL)
        {
            pos++;
            perge;
        }

        si (lex->genus == ARBOR_LEXEMA_PAREN_APERTA)
        {
            paren_depth++;
            slot = xar_addere(arg_currens);
            si (slot != NIHIL) { *slot = lex; }
        }
        alioquin si (lex->genus == ARBOR_LEXEMA_PAREN_CLAUSA)
        {
            paren_depth--;
            si (paren_depth > ZEPHYRUM)
            {
                slot = xar_addere(arg_currens);
                si (slot != NIHIL) { *slot = lex; }
            }
        }
        alioquin si (lex->genus == ARBOR_LEXEMA_COMMA && paren_depth == I)
        {
            /* Finire argumentum currens, incipiare novum */
            Xar** arg_slot = xar_addere(argumenta);
            si (arg_slot != NIHIL) { *arg_slot = arg_currens; }

            arg_currens = xar_creare(pp->piscina, magnitudo(ArborLexema*));
            si (arg_currens == NIHIL) { redde ZEPHYRUM; }
        }
        alioquin
        {
            slot = xar_addere(arg_currens);
            si (slot != NIHIL) { *slot = lex; }
        }

        pos++;
    }

    /* Addere ultimum argumentum */
    {
        Xar** arg_slot = xar_addere(argumenta);
        si (arg_slot != NIHIL) { *arg_slot = arg_currens; }
    }

    redde pos - pos_paren;  /* Numerus tokenorum consumptorum */
}

/* Invenire parametrum index in macro definitione */
interior i32
_invenire_param_index(ArborMacroDefinitio* macro, chorda* titulus)
{
    i32 i;
    i32 num;

    si (macro->parametra == NIHIL)
    {
        redde (i32)(-(I));
    }

    num = xar_numerus(macro->parametra);
    per (i = ZEPHYRUM; i < num; i++)
    {
        chorda* param = *(chorda**)xar_obtinere(macro->parametra, i);
        si (param != NIHIL &&
            param->mensura == titulus->mensura &&
            param->datum == titulus->datum)  /* Internati, potes comparare indicatores */
        {
            redde i;
        }
    }

    redde (i32)(-(I));
}

/* Creare string literal token ex tokens */
interior ArborLexema*
_stringify_tokens(ArborPraeparator* pp, Xar* tokens)
{
    ArborLexema* result;
    character* buffer;
    i32 buf_size;
    i32 buf_pos;
    i32 i;
    i32 num;

    /* Allocare buffer pro string */
    buf_size = C;
    buffer = piscina_allocare(pp->piscina, buf_size);
    si (buffer == NIHIL) { redde NIHIL; }

    buf_pos = ZEPHYRUM;
    buffer[buf_pos++] = '"';

    num = xar_numerus(tokens);
    per (i = ZEPHYRUM; i < num; i++)
    {
        ArborLexema* lex = *(ArborLexema**)xar_obtinere(tokens, i);
        i32 j;

        si (lex == NIHIL) { perge; }

        per (j = ZEPHYRUM; j < lex->valor.mensura && buf_pos < buf_size - II; j++)
        {
            i8 c = lex->valor.datum[j];
            si (c == '"' || c == '\\')
            {
                buffer[buf_pos++] = '\\';
            }
            buffer[buf_pos++] = (character)c;
        }

        /* Addere spatium inter tokens */
        si (i < num - I && buf_pos < buf_size - II)
        {
            buffer[buf_pos++] = ' ';
        }
    }

    buffer[buf_pos++] = '"';
    buffer[buf_pos] = '\0';

    /* Creare lexema */
    result = (ArborLexema*)piscina_allocare(pp->piscina, magnitudo(ArborLexema));
    si (result == NIHIL) { redde NIHIL; }

    result->genus = ARBOR_LEXEMA_STRING_LIT;
    result->valor.datum = (i8*)buffer;
    result->valor.mensura = buf_pos;
    result->linea = I;
    result->columna = I;
    result->trivia_ante = NIHIL;
    result->trivia_post = NIHIL;

    redde result;
}

/* Concatenare duo tokens (token pasting ##) */
interior ArborLexema*
_paste_tokens(ArborPraeparator* pp, ArborLexema* a, ArborLexema* b)
{
    ArborLexema* result;
    character* buffer;
    i32 len;
    ArborLexator* lexator;
    Xar* tokens;

    si (a == NIHIL && b == NIHIL) { redde NIHIL; }
    si (a == NIHIL) { redde b; }
    si (b == NIHIL) { redde a; }

    /* Concatenare valores */
    len = a->valor.mensura + b->valor.mensura;
    buffer = piscina_allocare(pp->piscina, len + I);
    si (buffer == NIHIL) { redde NIHIL; }

    memcpy(buffer, a->valor.datum, (size_t)a->valor.mensura);
    memcpy(buffer + a->valor.mensura, b->valor.datum, (size_t)b->valor.mensura);
    buffer[len] = '\0';

    /* Re-lex result */
    lexator = arbor_lexator_creare(pp->piscina, pp->intern, buffer, len);
    si (lexator == NIHIL) { redde NIHIL; }

    tokens = arbor_lexema_omnia(lexator);
    si (tokens == NIHIL || xar_numerus(tokens) == ZEPHYRUM)
    {
        redde NIHIL;
    }

    result = *(ArborLexema**)xar_obtinere(tokens, ZEPHYRUM);
    redde result;
}

/* Expandere function-like macro cum argumentis */
interior vacuum
_expandere_macro_functio(
    ArborPraeparator*     pp,
    ArborMacroDefinitio*  macro,
    Xar*                  argumenta,
    ArborOrigo*           origo,
    Xar*                  output)
{
    i32 i;
    i32 num;
    b32 next_is_stringify;
    ArborLexema* pending_paste;

    si (macro->corpus == NIHIL) { redde; }

    num = xar_numerus(macro->corpus);
    next_is_stringify = FALSUM;
    pending_paste = NIHIL;

    per (i = ZEPHYRUM; i < num; i++)
    {
        ArborLexema* lex;
        ArborLexema* lex_next;

        lex = *(ArborLexema**)xar_obtinere(macro->corpus, i);
        si (lex == NIHIL) { perge; }

        /* Look ahead pro ## */
        lex_next = NIHIL;
        si (i + I < num)
        {
            lex_next = *(ArborLexema**)xar_obtinere(macro->corpus, i + I);
        }

        /* Si # (stringification) */
        si (lex->genus == ARBOR_LEXEMA_HASH && !next_is_stringify)
        {
            next_is_stringify = VERUM;
            perge;
        }

        /* Si ## (paste) - prima pars */
        si (lex_next != NIHIL && lex_next->genus == ARBOR_LEXEMA_HASH_HASH)
        {
            /* Salvare hoc token pro paste */
            si (lex->genus == ARBOR_LEXEMA_IDENTIFICATOR)
            {
                chorda* titulus = chorda_internare(pp->intern, lex->valor);
                i32 param_idx = _invenire_param_index(macro, titulus);
                si (param_idx >= ZEPHYRUM)
                {
                    /* Get first token of argument */
                    Xar* arg = *(Xar**)xar_obtinere(argumenta, param_idx);
                    si (arg != NIHIL && xar_numerus(arg) > ZEPHYRUM)
                    {
                        pending_paste = *(ArborLexema**)xar_obtinere(arg, ZEPHYRUM);
                    }
                }
                alioquin
                {
                    pending_paste = lex;
                }
            }
            alioquin
            {
                pending_paste = lex;
            }
            i++;  /* Saltare ## */
            perge;
        }

        /* Si ## (paste) - secunda pars */
        si (pending_paste != NIHIL)
        {
            ArborLexema* to_paste = lex;

            si (lex->genus == ARBOR_LEXEMA_IDENTIFICATOR)
            {
                chorda* titulus = chorda_internare(pp->intern, lex->valor);
                i32 param_idx = _invenire_param_index(macro, titulus);
                si (param_idx >= ZEPHYRUM)
                {
                    Xar* arg = *(Xar**)xar_obtinere(argumenta, param_idx);
                    si (arg != NIHIL && xar_numerus(arg) > ZEPHYRUM)
                    {
                        to_paste = *(ArborLexema**)xar_obtinere(arg, ZEPHYRUM);
                    }
                }
            }

            {
                ArborLexema* pasted = _paste_tokens(pp, pending_paste, to_paste);
                si (pasted != NIHIL)
                {
                    _expandere_et_emittere(pp, pasted, origo, output);
                }
            }
            pending_paste = NIHIL;
            perge;
        }

        /* Si identificator et parametrum */
        si (lex->genus == ARBOR_LEXEMA_IDENTIFICATOR)
        {
            chorda* titulus = chorda_internare(pp->intern, lex->valor);
            i32 param_idx = _invenire_param_index(macro, titulus);

            si (param_idx >= ZEPHYRUM)
            {
                Xar* arg = *(Xar**)xar_obtinere(argumenta, param_idx);

                si (next_is_stringify)
                {
                    /* Stringify argumentum */
                    ArborLexema* stringified = _stringify_tokens(pp, arg);
                    si (stringified != NIHIL)
                    {
                        _expandere_et_emittere(pp, stringified, origo, output);
                    }
                    next_is_stringify = FALSUM;
                }
                alioquin
                {
                    /* Substituere argumentum */
                    i32 j;
                    i32 arg_num = xar_numerus(arg);
                    per (j = ZEPHYRUM; j < arg_num; j++)
                    {
                        ArborLexema* arg_lex = *(ArborLexema**)xar_obtinere(arg, j);
                        si (arg_lex != NIHIL)
                        {
                            _expandere_et_emittere(pp, arg_lex, origo, output);
                        }
                    }
                }
                perge;
            }
        }

        next_is_stringify = FALSUM;

        /* Token normale */
        _expandere_et_emittere(pp, lex, origo, output);
    }
}

/* Expandere et emittere lexemata de macro corpus */
interior vacuum
_expandere_corpus_macro(
    ArborPraeparator*     pp,
    ArborMacroDefinitio*  macro,
    ArborOrigo*           origo,
    Xar*                  output)
{
    i32 i;
    i32 num;

    si (macro->corpus == NIHIL)
    {
        redde;
    }

    num = xar_numerus(macro->corpus);
    per (i = ZEPHYRUM; i < num; i++)
    {
        ArborLexema* lex_corpus;

        lex_corpus = *(ArborLexema**)xar_obtinere(macro->corpus, i);
        si (lex_corpus == NIHIL)
        {
            perge;
        }

        /* Recursive expansion: si hoc lexema est identificator et macro, expandere */
        _expandere_et_emittere(pp, lex_corpus, origo, output);
    }
}

/* Expandere et emittere lexema (cum macro detection) */
interior vacuum
_expandere_et_emittere(
    ArborPraeparator*     pp,
    ArborLexema*          lex,
    ArborOrigo*           origo,
    Xar*                  output)
{
    ArborLexemaOrigo* lo;
    ArborLexemaOrigo** slotus;

    si (lex == NIHIL)
    {
        redde;
    }

    /* Verificare si identificator et macro */
    si (lex->genus == ARBOR_LEXEMA_IDENTIFICATOR)
    {
        chorda* nomen_internatum;
        ArborMacroDefinitio* macro;

        nomen_internatum = chorda_internare(pp->intern, lex->valor);
        si (nomen_internatum != NIHIL)
        {
            macro = arbor_praeparator_obtinere_macro(pp, nomen_internatum);
            si (macro != NIHIL && !macro->est_functio)
            {
                /* Object-like macro - verificare recursio */
                si (!_est_in_expansione(pp, nomen_internatum))
                {
                    ArborOrigo* origo_nova;

                    /* Push ad acervum expansionis */
                    _push_expansio(pp, nomen_internatum);
                    pp->profunditas_expansionis++;

                    /* Creare origo */
                    origo_nova = _creare_origo_macro(pp, nomen_internatum, lex, origo);

                    /* Expandere corpus */
                    _expandere_corpus_macro(pp, macro, origo_nova, output);

                    /* Pop de acervo */
                    pp->profunditas_expansionis--;
                    _pop_expansio(pp);

                    redde;  /* Jam emittimus lexemata expandita */
                }
            }
            /* Function-like macros require arguments - non expandere sine () */
        }
    }

    /* Non macro vel recursio - emittere ut est */
    lo = _creare_lexema_origo(pp, lex, origo);
    si (lo != NIHIL)
    {
        slotus = xar_addere(output);
        si (slotus != NIHIL)
        {
            *slotus = lo;
        }
    }
}

/* ==================================================
 * Functiones Internae - Utilitas Detectio Lineae
 * ================================================== */

/* Verificare si lexema est ad initium lineae
 * In C89, # potest habere spatium ante se
 * Usamus simpliciorem methodum: comparare lineas inter lexemata */
interior b32
_est_ad_initium_lineae(ArborLexema* lex, ArborLexema* lex_praevius)
{
    si (lex == NIHIL)
    {
        redde FALSUM;
    }

    /* Si primum lexema (nullum praevium), ad initium */
    si (lex_praevius == NIHIL)
    {
        redde VERUM;
    }

    /* Si in linea diversa a praevio, ad initium novae lineae */
    si (lex->linea > lex_praevius->linea)
    {
        redde VERUM;
    }

    redde FALSUM;
}

/* ==================================================
 * API - Processare
 * ================================================== */

Xar*
arbor_praeparator_processare(
    ArborPraeparator*     pp,
    constans character*   via_file)
{
    chorda contentum;
    Xar* lexemata;
    ArborLexator* lexator;

    si (pp == NIHIL || via_file == NIHIL)
    {
        redde NIHIL;
    }

    /* Legere filum */
    contentum = filum_legere_totum(via_file, pp->piscina);
    si (contentum.datum == NIHIL)
    {
        _addere_error(pp, ARBOR_PP_ERROR_FILE_NOT_FOUND,
            "filum non inventum", via_file, I, I);
        redde NIHIL;
    }

    /* Lexere filum */
    lexator = arbor_lexator_creare(
        pp->piscina,
        pp->intern,
        (constans character*)contentum.datum,
        contentum.mensura);

    si (lexator == NIHIL)
    {
        redde NIHIL;
    }

    lexemata = arbor_lexema_omnia(lexator);
    si (lexemata == NIHIL)
    {
        redde NIHIL;
    }

    /* Processare lexemata */
    redde arbor_praeparator_processare_lexemata(pp, lexemata, via_file);
}

/* Invenire finem lineae (proximum lexema in linea diversa) */
interior i32
_invenire_finem_lineae(Xar* lexemata, i32 pos_initium, i32 num)
{
    i32 pos;
    ArborLexema* lex_init;
    i32 linea_init;

    /* Obtinere lineam initialem - usare pos_initium, non pos_initium - 1 */
    lex_init = *(ArborLexema**)xar_obtinere(lexemata, pos_initium);
    linea_init = (lex_init != NIHIL) ? lex_init->linea : I;

    per (pos = pos_initium; pos < num; pos++)
    {
        ArborLexema* lex;

        lex = *(ArborLexema**)xar_obtinere(lexemata, pos);
        si (lex == NIHIL)
        {
            perge;
        }

        si (lex->genus == ARBOR_LEXEMA_EOF)
        {
            redde pos;
        }

        /* Si in linea diversa, haec est proxima linea */
        si (lex->linea > linea_init)
        {
            redde pos;
        }
    }

    redde num;
}

Xar*
arbor_praeparator_processare_lexemata(
    ArborPraeparator*     pp,
    Xar*                  lexemata,
    constans character*   via_file)
{
    Xar* output;
    i32 pos;
    i32 num;
    ArborLexema* lex_praevius;

    (vacuum)via_file;  /* TODO: usare pro error reporting */

    si (pp == NIHIL || lexemata == NIHIL)
    {
        redde NIHIL;
    }

    output = xar_creare(pp->piscina, magnitudo(ArborLexemaOrigo*));
    si (output == NIHIL)
    {
        redde NIHIL;
    }

    pos = ZEPHYRUM;
    num = xar_numerus(lexemata);
    lex_praevius = NIHIL;

    /* PRESERVARE mode: emittere omnia lexemata sine processamento */
    si (pp->modus == ARBOR_PP_MODUS_PRESERVARE)
    {
        dum (pos < num)
        {
            ArborLexema* lex_curr;
            ArborLexemaOrigo* lo;
            ArborLexemaOrigo** slotus;

            lex_curr = *(ArborLexema**)xar_obtinere(lexemata, pos);
            si (lex_curr != NIHIL)
            {
                lo = _creare_lexema_origo(pp, lex_curr, NIHIL);
                si (lo != NIHIL)
                {
                    slotus = xar_addere(output);
                    si (slotus != NIHIL)
                    {
                        *slotus = lo;
                    }
                }
            }
            pos++;
        }
        redde output;
    }

    /* HYBRID mode: processare directivas (discere macros), sed emittere
     * originales tokens (sine expansione). Combines learning from PROCESSARE
     * with output preservation from PRESERVARE. */
    si (pp->modus == ARBOR_PP_MODUS_HYBRID)
    {
        /* Pre-scan pro typedefs (detectare nomen/typedef declarationes) */
        {
            i32 scan_pos = ZEPHYRUM;
            dum (scan_pos < num)
            {
                ArborLexema* scan_lex = *(ArborLexema**)xar_obtinere(lexemata, scan_pos);
                si (scan_lex != NIHIL)
                {
                    /* Detectare typedef/nomen keyword */
                    si (scan_lex->genus == ARBOR_LEXEMA_TYPEDEF ||
                        (scan_lex->genus == ARBOR_LEXEMA_IDENTIFICATOR &&
                         scan_lex->valor.mensura == V &&
                         memcmp(scan_lex->valor.datum, "nomen", V) == ZEPHYRUM))
                    {
                        /* Invenire finem statementum (;)
                         * Tractare imbricatae braces pro struct/union typedefs:
                         * nomen structura X { int a; } X;
                         * Semicolon post "a" non est finis, semicolon post "X" est. */
                        i32 end_pos = scan_pos + I;
                        i32 brace_depth = ZEPHYRUM;
                        dum (end_pos < num)
                        {
                            ArborLexema* end_lex = *(ArborLexema**)xar_obtinere(lexemata, end_pos);
                            si (end_lex != NIHIL)
                            {
                                si (end_lex->genus == ARBOR_LEXEMA_BRACE_APERTA)
                                {
                                    brace_depth++;
                                }
                                alioquin si (end_lex->genus == ARBOR_LEXEMA_BRACE_CLAUSA)
                                {
                                    brace_depth--;
                                }
                                alioquin si (end_lex->genus == ARBOR_LEXEMA_SEMICOLON &&
                                             brace_depth == ZEPHYRUM)
                                {
                                    frange;
                                }
                            }
                            end_pos++;
                        }
                        _detectare_typedef(pp, lexemata, scan_pos, end_pos + I);
                        scan_pos = end_pos;
                    }
                }
                scan_pos++;
            }
        }

        dum (pos < num)
        {
            ArborLexema* lex_curr;
            ArborLexemaOrigo* lo;
            ArborLexemaOrigo** slotus;
            b32 ad_initium;

            lex_curr = *(ArborLexema**)xar_obtinere(lexemata, pos);
            si (lex_curr == NIHIL)
            {
                pos++;
                perge;
            }

            /* EOF - finis */
            si (lex_curr->genus == ARBOR_LEXEMA_EOF)
            {
                lo = _creare_lexema_origo(pp, lex_curr, NIHIL);
                si (lo != NIHIL)
                {
                    slotus = xar_addere(output);
                    si (slotus != NIHIL)
                    {
                        *slotus = lo;
                    }
                }
                frange;
            }

            /* Detectare si ad initium lineae */
            ad_initium = _est_ad_initium_lineae(lex_curr, lex_praevius);

            /* Detectare directiva: # ad initium lineae */
            si (ad_initium && _est_hash(lex_curr))
            {
                i32 pos_finis;

                /* Invenire finem lineae */
                pos_finis = _invenire_finem_lineae(lexemata, pos, num);

                /* Processare directiva INTERNE (discere macros, sequi includes)
                 * sed non emittere - nos emittimus originales tokens infra */
                _processare_directiva(pp, lexemata, pos, num, via_file, NIHIL);

                /* Emittere originales directive tokens */
                dum (pos < pos_finis)
                {
                    ArborLexema* dir_lex;
                    dir_lex = *(ArborLexema**)xar_obtinere(lexemata, pos);
                    si (dir_lex != NIHIL)
                    {
                        lo = _creare_lexema_origo(pp, dir_lex, NIHIL);
                        si (lo != NIHIL)
                        {
                            slotus = xar_addere(output);
                            si (slotus != NIHIL)
                            {
                                *slotus = lo;
                            }
                        }
                    }
                    lex_praevius = dir_lex;
                    pos++;
                }
                perge;
            }

            /* Regular token: emittere sine expansione */
            lo = _creare_lexema_origo(pp, lex_curr, NIHIL);
            si (lo != NIHIL)
            {
                slotus = xar_addere(output);
                si (slotus != NIHIL)
                {
                    *slotus = lo;
                }
            }
            lex_praevius = lex_curr;
            pos++;
        }
        redde output;
    }

    dum (pos < num)
    {
        ArborLexema* lex;
        ArborLexemaOrigo* lo;
        ArborLexemaOrigo** slotus;
        b32 ad_initium;

        lex = *(ArborLexema**)xar_obtinere(lexemata, pos);
        si (lex == NIHIL)
        {
            pos++;
            perge;
        }

        /* EOF - finis */
        si (lex->genus == ARBOR_LEXEMA_EOF)
        {
            lo = _creare_lexema_origo(pp, lex, NIHIL);
            si (lo != NIHIL)
            {
                slotus = xar_addere(output);
                si (slotus != NIHIL)
                {
                    *slotus = lo;
                }
            }
            frange;
        }

        /* Detectare si ad initium lineae */
        ad_initium = _est_ad_initium_lineae(lex, lex_praevius);

        /* Detectare directiva: # ad initium lineae */
        si (ad_initium && _est_hash(lex))
        {
            /* Processare directiva et obtinere novam positionem */
            i32 pos_novus = _processare_directiva(pp, lexemata, pos, num, via_file, output);

            /* Actualizare lex_praevius ad ultimum token ante novam positionem */
            si (pos_novus > ZEPHYRUM)
            {
                lex_praevius = *(ArborLexema**)xar_obtinere(lexemata, pos_novus - I);
            }
            pos = pos_novus;
            perge;
        }

        /* Si non activus, saltare */
        si (!pp->cond_activus)
        {
            lex_praevius = lex;
            pos++;
            perge;
        }

        /* Verificare function-like macro invocatio */
        si (lex->genus == ARBOR_LEXEMA_IDENTIFICATOR)
        {
            chorda* nomen_internatum;
            ArborMacroDefinitio* macro;

            nomen_internatum = chorda_internare(pp->intern, lex->valor);
            si (nomen_internatum != NIHIL)
            {
                macro = arbor_praeparator_obtinere_macro(pp, nomen_internatum);
                si (macro != NIHIL && macro->est_functio)
                {
                    /* Function-like macro - quaerere '(' */
                    ArborLexema* lex_next = NIHIL;
                    si (pos + I < num)
                    {
                        lex_next = *(ArborLexema**)xar_obtinere(lexemata, pos + I);
                    }

                    si (lex_next != NIHIL && lex_next->genus == ARBOR_LEXEMA_PAREN_APERTA)
                    {
                        /* Colligere argumenta et expandere */
                        si (!_est_in_expansione(pp, nomen_internatum))
                        {
                            Xar* argumenta;
                            i32 tokens_consumed;
                            ArborOrigo* origo_nova;

                            argumenta = xar_creare(pp->piscina, magnitudo(Xar*));
                            tokens_consumed = _colligere_argumenta(
                                pp, lexemata, pos + I, num, argumenta);

                            si (tokens_consumed > ZEPHYRUM)
                            {
                                _push_expansio(pp, nomen_internatum);
                                pp->profunditas_expansionis++;

                                origo_nova = _creare_origo_macro(
                                    pp, nomen_internatum, lex, NIHIL);

                                _expandere_macro_functio(
                                    pp, macro, argumenta, origo_nova, output);

                                pp->profunditas_expansionis--;
                                _pop_expansio(pp);

                                lex_praevius = lex;
                                pos += I + tokens_consumed;
                                perge;
                            }
                        }
                    }
                }
            }
        }

        /* Emittere lexema cum macro expansion */
        _expandere_et_emittere(pp, lex, NIHIL, output);

        lex_praevius = lex;
        pos++;
    }

    redde output;
}
