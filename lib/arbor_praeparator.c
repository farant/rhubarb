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
    TabulaDispersa*       macros;           /* chorda -> ArborMacroDefinitio* */
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

    /* Creare tabula macro */
    pp->macros = tabula_dispersa_creare_chorda(piscina, CXXVIII);
    si (pp->macros == NIHIL)
    {
        redde NIHIL;
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
 * Functiones Internae - Processare Directivas
 * ================================================== */

interior i32
_processare_directiva(
    ArborPraeparator*     pp,
    Xar*                  lexemata,
    i32                   pos_hash,
    i32                   num,
    constans character*   via_file)
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

        /* TODO: Aliae directivae (#if, #elif) */
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

    /* Obtinere lineam initialem */
    si (pos_initium > ZEPHYRUM)
    {
        lex_init = *(ArborLexema**)xar_obtinere(lexemata, pos_initium - I);
        linea_init = (lex_init != NIHIL) ? lex_init->linea : I;
    }
    alioquin
    {
        lex_init = *(ArborLexema**)xar_obtinere(lexemata, ZEPHYRUM);
        linea_init = (lex_init != NIHIL) ? lex_init->linea : I;
    }

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
            i32 pos_novus = _processare_directiva(pp, lexemata, pos, num, via_file);

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

        /* Emittere lexema cum macro expansion */
        _expandere_et_emittere(pp, lex, NIHIL, output);

        lex_praevius = lex;
        pos++;
    }

    redde output;
}
