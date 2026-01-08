/* arbor_syntaxis.c - C89 Parser
 *
 * Pratt parser pro expressionibus, recursive descent pro sententiis.
 */

#include "arbor_syntaxis.h"
#include "tabula_dispersa.h"
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
    TabulaDispersa*       typedef_nomina;  /* Typedef name table */
    TabulaDispersa*       keyword_macros;  /* From preprocessor: macro -> keyword type */
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
interior ArborNodus* _parsere_struct_specifier(ArborSyntaxis* syn, b32 est_unio);
interior ArborNodus* _parsere_enum_specifier(ArborSyntaxis* syn);
interior ArborNodus* _parsere_type_specifier(ArborSyntaxis* syn);
interior ArborNodus* _parsere_type_qualifier(ArborSyntaxis* syn);
interior ArborNodus* _parsere_declarator(ArborSyntaxis* syn);
interior ArborNodus* _parsere_specifiers(ArborSyntaxis* syn, Xar* specifiers);
interior ArborNodus* _parsere_type_name(ArborSyntaxis* syn);
interior ArborNodus* _parsere_directiva(ArborSyntaxis* syn);
interior b32 _est_directiva_initium(ArborSyntaxis* syn);

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

/* Verifica si currens token congruit genus, etiam per keyword macros */
interior b32
_congruit_effectivum(ArborSyntaxis* syn, ArborLexemaGenus genus)
{
    ArborLexema* lex;
    lex = _currens_lex(syn);
    si (lex == NIHIL)
    {
        redde FALSUM;
    }
    /* Check actual genus first */
    si (lex->genus == genus)
    {
        redde VERUM;
    }
    /* Check keyword macros */
    si (lex->genus == ARBOR_LEXEMA_IDENTIFICATOR && syn->keyword_macros != NIHIL)
    {
        chorda* titulus = chorda_internare(syn->intern, lex->valor);
        vacuum* keyword_val = NIHIL;
        si (tabula_dispersa_invenire(syn->keyword_macros, *titulus, &keyword_val))
        {
            redde (ArborLexemaGenus)(*(i32*)keyword_val) == genus;
        }
    }
    redde FALSUM;
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
 * Adiutores Interni - Trivia Preservation
 * ================================================== */

/* Addere trivia synthetica (textus) ad Xar */
interior vacuum
_addere_synth_trivia (
    ArborSyntaxis*       syn,
    Xar**                dest_xar,
    constans character*  textus)
{
    ArborTrivia*  trivia;
    ArborTrivia** locus;
    chorda        temp;
    chorda*       internata;

    si (textus == NIHIL)
    {
        redde;
    }

    si (*dest_xar == NIHIL)
    {
        *dest_xar = xar_creare(syn->piscina, magnitudo(ArborTrivia*));
    }

    trivia = piscina_allocare(syn->piscina, magnitudo(ArborTrivia));
    trivia->linea = ZEPHYRUM;
    trivia->columna = ZEPHYRUM;
    trivia->est_commentum = FALSUM;
    trivia->est_c99 = FALSUM;

    temp = chorda_ex_literis(textus, syn->piscina);
    internata = chorda_internare(syn->intern, temp);
    trivia->valor = *internata;

    locus = xar_addere(*dest_xar);
    si (locus != NIHIL)
    {
        *locus = trivia;
    }
}

/* Addere synthetic trivia ex chorda (pro keyword text preservation) */
interior vacuum
_addere_synth_trivia_chorda (
    ArborSyntaxis*       syn,
    Xar**                dest_xar,
    chorda               textus)
{
    ArborTrivia*  trivia;
    ArborTrivia** locus;
    chorda*       internata;

    si (textus.datum == NIHIL || textus.mensura == ZEPHYRUM)
    {
        redde;
    }

    si (*dest_xar == NIHIL)
    {
        *dest_xar = xar_creare(syn->piscina, magnitudo(ArborTrivia*));
    }

    trivia = piscina_allocare(syn->piscina, magnitudo(ArborTrivia));
    trivia->linea = ZEPHYRUM;
    trivia->columna = ZEPHYRUM;
    trivia->est_commentum = FALSUM;
    trivia->est_c99 = FALSUM;

    internata = chorda_internare(syn->intern, textus);
    trivia->valor = *internata;

    locus = xar_addere(*dest_xar);
    si (locus != NIHIL)
    {
        *locus = trivia;
    }
}

/* Copiare trivia ex fonte ad destinationem Xar */
interior vacuum
_copiare_trivia_ad_xar (
    ArborSyntaxis* syn,
    Xar**          dest_xar,
    Xar*           fons)
{
    i32 i;
    i32 num;

    si (fons == NIHIL)
    {
        redde;
    }

    num = xar_numerus(fons);
    si (num == ZEPHYRUM)
    {
        redde;
    }

    si (*dest_xar == NIHIL)
    {
        *dest_xar = xar_creare(syn->piscina, magnitudo(ArborTrivia*));
    }

    per (i = ZEPHYRUM; i < num; i++)
    {
        ArborTrivia** tp = xar_obtinere(fons, i);
        si (tp != NIHIL && *tp != NIHIL)
        {
            ArborTrivia** slot = xar_addere(*dest_xar);
            si (slot != NIHIL)
            {
                *slot = *tp;
            }
        }
    }
}

/* Consumere token et capere trivia ad nodes
 *
 * Consumit token expectatum et preservat trivia:
 * - trivia_ante token → addit ad nodus_ante.trivia_post
 * - trivia_post token → addit ad nodus_post.trivia_ante
 *
 * Formator iam emittet textum operatoris - non addimus illud ad trivia.
 *
 * Pro exemplo "x = 42":
 *   - token "=" habet trivia_ante=" " (spatium ante =)
 *   - token "=" habet trivia_post=" " (spatium post =)
 *   - x (nodus_ante) recipit " " in trivia_post
 *   - 42 (nodus_post) recipit " " in trivia_ante
 */
interior b32
_consumere_cum_trivia (
    ArborSyntaxis*      syn,
    ArborLexemaGenus    genus,
    ArborNodus*         nodus_ante,
    ArborNodus*         nodus_post)
{
    ArborLexema*  lex;

    si (!_congruit(syn, genus))
    {
        redde FALSUM;
    }

    lex = _currens_lex(syn);
    si (lex != NIHIL)
    {
        /* Capere trivia_ante et addere ad nodus_ante.trivia_post */
        si (nodus_ante != NIHIL)
        {
            _copiare_trivia_ad_xar(syn, &nodus_ante->trivia_post, lex->trivia_ante);
            /* NON addere textum operatoris - formator iam emittet illud */
        }

        /* Capere trivia_post et addere ad nodus_post.trivia_ante */
        si (nodus_post != NIHIL)
        {
            _copiare_trivia_ad_xar(syn, &nodus_post->trivia_ante, lex->trivia_post);
        }
    }

    _progredi(syn);
    redde VERUM;
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
    ArborLexemaOrigo** ptr;
    ArborLexemaOrigo*  lo;
    ArborLexema*       lex;

    /* Capere locum finis ex praecedente token */
    lo = NIHIL;
    si (syn->positus > ZEPHYRUM)
    {
        ptr = xar_obtinere(syn->lexemata, syn->positus - I);
        si (ptr != NIHIL)
        {
            lo = *ptr;
        }
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
 * Punctuation Attachment for Roundtrip
 *
 * Stores punctuation as synthetic trivia entries so
 * fidelis mode can emit them during roundtrip.
 * ================================================== */

interior ArborTrivia*
_creare_trivia_punctuationis (
    ArborSyntaxis*      syn,
    constans character* textus)
{
    ArborTrivia* trivia;
    chorda       temp;
    chorda*      internata;

    trivia = piscina_allocare(syn->piscina, magnitudo(ArborTrivia));
    trivia->linea = ZEPHYRUM;
    trivia->columna = ZEPHYRUM;
    trivia->est_commentum = FALSUM;
    trivia->est_c99 = FALSUM;

    temp = chorda_ex_literis(textus, syn->piscina);
    internata = chorda_internare(syn->intern, temp);
    trivia->valor = *internata;

    redde trivia;
}

/* Attach punctuation to trivia_ante (emitted BEFORE node content) */
interior vacuum
_praeponere_punctuationem (
    ArborSyntaxis*      syn,
    ArborNodus*         nodus,
    constans character* textus)
{
    ArborTrivia*  trivia;
    ArborTrivia** locus;

    si (nodus == NIHIL || textus == NIHIL) redde;

    si (nodus->trivia_ante == NIHIL)
    {
        nodus->trivia_ante = xar_creare(syn->piscina, magnitudo(ArborTrivia*));
    }

    trivia = _creare_trivia_punctuationis(syn, textus);
    locus = xar_addere(nodus->trivia_ante);
    *locus = trivia;
}

/* Attach punctuation to trivia_post (emitted AFTER node content)
 *
 * PRAEPONE punctuationem - punctuatio venit ANTE trivia_post existente.
 * Pro exemplo ")": token ")" sequitur spatium, ergo:
 *   - Emittere ")"
 *   - Deinde emittere spatium (ex trivia_post originali)
 */
interior vacuum
_attachere_punctuationem (
    ArborSyntaxis*      syn,
    ArborNodus*         nodus,
    constans character* textus)
{
    ArborTrivia*   trivia;
    ArborTrivia**  locus;
    Xar*           new_xar;
    i32            i;
    i32            num;

    si (nodus == NIHIL || textus == NIHIL) redde;

    trivia = _creare_trivia_punctuationis(syn, textus);

    /* PRAEPONE: primum punctuatio, deinde trivia existens */
    new_xar = xar_creare(syn->piscina, magnitudo(ArborTrivia*));

    /* Addere punctuationem primum */
    locus = xar_addere(new_xar);
    *locus = trivia;

    /* Deinde copiare trivia existens */
    si (nodus->trivia_post != NIHIL)
    {
        num = xar_numerus(nodus->trivia_post);
        per (i = ZEPHYRUM; i < num; i++)
        {
            ArborTrivia** tp = xar_obtinere(nodus->trivia_post, i);
            si (tp != NIHIL)
            {
                locus = xar_addere(new_xar);
                *locus = *tp;
            }
        }
    }

    nodus->trivia_post = new_xar;
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
 * Typedef Name Tracking
 * ================================================== */

interior vacuum
_registrare_typedef(ArborSyntaxis* syn, chorda* titulus)
{
    si (syn == NIHIL || titulus == NIHIL || syn->typedef_nomina == NIHIL)
    {
        redde;
    }
    /* Valor non gravis, solum clavis importat */
    tabula_dispersa_inserere(syn->typedef_nomina, *titulus, (vacuum*)(longus)VERUM);
}

interior b32
_est_typedef_nomen(ArborSyntaxis* syn, chorda* titulus)
{
    si (syn == NIHIL || titulus == NIHIL || syn->typedef_nomina == NIHIL)
    {
        redde FALSUM;
    }
    redde tabula_dispersa_continet(syn->typedef_nomina, *titulus);
}

/* Heuristic: identifier probabiliter est typedef nomen si sequitur:
 * - '*' (pointer ad typum)
 * - identifier (typus + nomen variabilis)
 * - ')' (abstract declarator vel finis parametrorum)
 * - ',' (separator parametrorum)
 *
 * Hoc est utile in PRESERVARE modo ubi typedef nomina
 * ex #include non sunt registrata.
 *
 * NOTA: Non verificamus _est_typedef_nomen hic quia iam verificatum
 * in vocante. Solum inspicimus quid sequitur.
 */
interior b32
_est_probable_typedef_nomen(ArborSyntaxis* syn)
{
    ArborLexema* next;

    si (syn == NIHIL || syn->lexemata == NIHIL)
    {
        redde FALSUM;
    }

    /* Inspicere quid sequitur currentem token */
    next = _prospicere_lex(syn, I);
    si (next == NIHIL)
    {
        redde FALSUM;
    }

    /* IDENTIFIER * - pointer ad typum */
    si (next->genus == ARBOR_LEXEMA_ASTERISCUS)
    {
        redde VERUM;
    }

    /* IDENTIFIER IDENTIFIER - typus + nomen */
    si (next->genus == ARBOR_LEXEMA_IDENTIFICATOR)
    {
        redde VERUM;
    }

    /* NOTA: Non includimus PAREN_CLAUSA vel COMMA hic quia
     * possunt causare falsos positivos in contextu ut:
     * - int (*fp)(int);  -- fp sequitur )
     * - int f(a, b);     -- a sequitur ,
     * Heuristic IDENTIFIER* et IDENTIFIER IDENTIFIER sunt satis. */

    redde FALSUM;
}

/* Extrahere nomen ex declarator (recursivo per nested declarators) */
interior chorda*
_extrahere_declarator_nomen(ArborNodus* decl)
{
    i32 i;
    i32 n;

    si (decl == NIHIL)
    {
        redde NIHIL;
    }

    /* IDENTIFIER nodus habet nomen directe */
    si (decl->genus == ARBOR_NODUS_IDENTIFIER)
    {
        redde decl->datum.folium.valor;
    }

    /* DECLARATOR habet liberi - iter per eos */
    si (decl->genus == ARBOR_NODUS_DECLARATOR)
    {
        si (decl->datum.genericum.liberi == NIHIL)
        {
            redde NIHIL;
        }
        n = xar_numerus(decl->datum.genericum.liberi);
        per (i = ZEPHYRUM; i < n; i++)
        {
            ArborNodus** child_ptr = xar_obtinere(decl->datum.genericum.liberi, i);
            si (child_ptr != NIHIL && *child_ptr != NIHIL)
            {
                chorda* res = _extrahere_declarator_nomen(*child_ptr);
                si (res != NIHIL)
                {
                    redde res;
                }
            }
        }
    }

    /* INIT_DECLARATOR wraps declarator */
    si (decl->genus == ARBOR_NODUS_INIT_DECLARATOR)
    {
        redde _extrahere_declarator_nomen(decl->datum.init_decl.declarator);
    }

    redde NIHIL;
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

/* Verifica si identificator est extensio (__attribute__, __extension__, etc.) */
interior b32
_est_extension_keyword(ArborSyntaxis* syn)
{
    ArborLexema* lex;
    lex = _currens_lex(syn);
    si (lex == NIHIL || lex->genus != ARBOR_LEXEMA_IDENTIFICATOR)
    {
        redde FALSUM;
    }
    /* Identifica keywords cum __ praefixo */
    si (lex->valor.mensura >= II &&
        lex->valor.datum[ZEPHYRUM] == '_' &&
        lex->valor.datum[I] == '_')
    {
        redde VERUM;
    }
    redde FALSUM;
}

/* Parse extension node (__attribute__, __extension__, __asm__, etc.) */
interior ArborNodus*
_parsere_extension(ArborSyntaxis* syn)
{
    ArborNodus* nodus;
    ArborLexema* lex;
    i32 paren_depth;

    nodus = _creare_nodum(syn, ARBOR_NODUS_EXTENSION);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    lex = _currens_lex(syn);
    si (lex == NIHIL)
    {
        redde NIHIL;
    }

    /* Conservare nomen extensionis */
    nodus->datum.folium.valor = chorda_internare(syn->intern, lex->valor);
    _progredi(syn);

    /* Saltare parentheses balancitas si praesentes: __attribute__((xxx)) */
    si (_congruit(syn, ARBOR_LEXEMA_PAREN_APERTA))
    {
        paren_depth = ZEPHYRUM;
        fac
        {
            si (_congruit(syn, ARBOR_LEXEMA_PAREN_APERTA))
            {
                paren_depth++;
            }
            alioquin si (_congruit(syn, ARBOR_LEXEMA_PAREN_CLAUSA))
            {
                paren_depth--;
            }
            _progredi(syn);
        } dum (paren_depth > ZEPHYRUM && !_est_finis(syn));
    }

    _finire_nodum(syn, nodus);
    redde nodus;
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

    /* Verifica keywords typorum */
    si (_est_type_specifier(lex->genus) ||
        _est_type_qualifier(lex->genus) ||
        _est_storage_class(lex->genus))
    {
        redde VERUM;
    }

    /* Verifica typedef nomen et keyword macros */
    si (lex->genus == ARBOR_LEXEMA_IDENTIFICATOR)
    {
        chorda* titulus = chorda_internare(syn->intern, lex->valor);

        /* Verifica registered typedef nomen */
        si (_est_typedef_nomen(syn, titulus))
        {
            redde VERUM;
        }

        /* Verifica keyword macros - sed solum pro declaration-related keywords */
        si (syn->keyword_macros != NIHIL)
        {
            vacuum* keyword_val = NIHIL;
            si (tabula_dispersa_invenire(syn->keyword_macros, *titulus, &keyword_val))
            {
                i32 keyword_type = *(i32*)keyword_val;
                /* Solo return VERUM pro declaration-starting keywords (non control flow) */
                si (keyword_type == ARBOR_LEXEMA_AUTO ||
                    keyword_type == ARBOR_LEXEMA_REGISTER ||
                    keyword_type == ARBOR_LEXEMA_STATIC ||
                    keyword_type == ARBOR_LEXEMA_EXTERN ||
                    keyword_type == ARBOR_LEXEMA_TYPEDEF ||
                    keyword_type == ARBOR_LEXEMA_CONST ||
                    keyword_type == ARBOR_LEXEMA_VOLATILE ||
                    keyword_type == ARBOR_LEXEMA_VOID ||
                    keyword_type == ARBOR_LEXEMA_CHAR ||
                    keyword_type == ARBOR_LEXEMA_SHORT ||
                    keyword_type == ARBOR_LEXEMA_INT ||
                    keyword_type == ARBOR_LEXEMA_LONG ||
                    keyword_type == ARBOR_LEXEMA_FLOAT ||
                    keyword_type == ARBOR_LEXEMA_DOUBLE ||
                    keyword_type == ARBOR_LEXEMA_SIGNED ||
                    keyword_type == ARBOR_LEXEMA_UNSIGNED ||
                    keyword_type == ARBOR_LEXEMA_STRUCT ||
                    keyword_type == ARBOR_LEXEMA_UNION ||
                    keyword_type == ARBOR_LEXEMA_ENUM)
                {
                    redde VERUM;
                }
                /* Control flow keywords (if, else, while, etc.) - NOT declaration starters */
                redde FALSUM;
            }
        }

        /* Verifica probable typedef nomen per heuristic (i32 -> identifier identifier) */
        si (_est_probable_typedef_nomen(syn))
        {
            redde VERUM;
        }
    }

    redde FALSUM;
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

/* Detegere si token currens est initium type-name (pro cast) */
interior b32
_est_type_initium(ArborSyntaxis* syn)
{
    ArborLexema* lex;
    chorda*      titulus;

    lex = _currens_lex(syn);
    si (lex == NIHIL)
    {
        redde FALSUM;
    }

    /* Type specifiers: void, char, int, long, etc. */
    si (_est_type_specifier(lex->genus))
    {
        redde VERUM;
    }

    /* Type qualifiers: const, volatile */
    si (_est_type_qualifier(lex->genus))
    {
        redde VERUM;
    }

    /* Typedef nomen */
    si (lex->genus == ARBOR_LEXEMA_IDENTIFICATOR)
    {
        titulus = chorda_internare(syn->intern, lex->valor);
        si (_est_typedef_nomen(syn, titulus))
        {
            redde VERUM;
        }
    }

    redde FALSUM;
}

interior ArborNodus*
_parsere_parenthesis(ArborSyntaxis* syn)
{
    ArborNodus*  nodus;
    ArborLexema* open_lex;
    ArborLexema* close_lex;
    Xar*         trivia_post_open;
    Xar*         trivia_ante_open;
    Xar*         old_ante;

    /* Capere trivia ex '(' ante consumere */
    open_lex = _currens_lex(syn);
    trivia_ante_open = (open_lex != NIHIL) ? open_lex->trivia_ante : NIHIL;
    trivia_post_open = (open_lex != NIHIL) ? open_lex->trivia_post : NIHIL;

    /* Skip ( */
    _progredi(syn);

    /* Detegere cast expression: (type-name)expr */
    si (_est_type_initium(syn))
    {
        /* Cast expression */
        ArborNodus* type_node;
        ArborNodus* cast_expr;
        Xar* trivia_post_close;
        ArborNodus** first_spec;

        cast_expr = _creare_nodum(syn, ARBOR_NODUS_CAST_EXPRESSION);
        si (cast_expr == NIHIL)
        {
            redde NIHIL;
        }

        /* Parsere type-name (specifiers + optional abstract declarator) */
        type_node = _parsere_type_name(syn);
        si (type_node == NIHIL)
        {
            redde NIHIL;
        }

        /* Consumere ')' */
        close_lex = _currens_lex(syn);
        trivia_post_close = (close_lex != NIHIL) ? close_lex->trivia_post : NIHIL;
        _expectare(syn, ARBOR_LEXEMA_PAREN_CLAUSA, "Expectabatur ) post type in cast");

        /* Trivia handling:
         * - Praepone "(" + trivia ad primum specifier
         * - Appone ")" + trivia ad type_node.trivia_post */
        first_spec = xar_obtinere(type_node->datum.parametrum.specifiers, ZEPHYRUM);
        si (first_spec != NIHIL && *first_spec != NIHIL)
        {
            Xar* old_ante;
            old_ante = (*first_spec)->trivia_ante;
            (*first_spec)->trivia_ante = NIHIL;
            _copiare_trivia_ad_xar(syn, &(*first_spec)->trivia_ante, trivia_ante_open);
            _addere_synth_trivia(syn, &(*first_spec)->trivia_ante, "(");
            _copiare_trivia_ad_xar(syn, &(*first_spec)->trivia_ante, trivia_post_open);
            _copiare_trivia_ad_xar(syn, &(*first_spec)->trivia_ante, old_ante);
        }

        /* Appone ')' trivia ad type_node (emittetur post declarator vel specifiers) */
        si (close_lex != NIHIL)
        {
            _copiare_trivia_ad_xar(syn, &type_node->trivia_post, close_lex->trivia_ante);
        }
        _addere_synth_trivia(syn, &type_node->trivia_post, ")");
        _copiare_trivia_ad_xar(syn, &type_node->trivia_post, trivia_post_close);

        cast_expr->datum.conversio.typus = type_node;
        type_node->pater = cast_expr;

        /* Parsere cast operand */
        cast_expr->datum.conversio.expressio = _parsere_expressio(syn, PREC_UNARIUS);
        si (cast_expr->datum.conversio.expressio != NIHIL)
        {
            cast_expr->datum.conversio.expressio->pater = cast_expr;
        }

        /* NON vocare _finire_nodum - expressio iam habet trivia_post */
        redde cast_expr;
    }

    /* Parenthesized expression */
    nodus = _parsere_expressio(syn, PREC_NIHIL);
    si (nodus == NIHIL)
    {
        _expectare(syn, ARBOR_LEXEMA_PAREN_CLAUSA, "Expectabatur )");
        redde NIHIL;
    }

    /* Praepone "(" + trivia ad nodus.trivia_ante */
    old_ante = nodus->trivia_ante;
    nodus->trivia_ante = NIHIL;
    _copiare_trivia_ad_xar(syn, &nodus->trivia_ante, trivia_ante_open);
    _addere_synth_trivia(syn, &nodus->trivia_ante, "(");
    _copiare_trivia_ad_xar(syn, &nodus->trivia_ante, trivia_post_open);
    _copiare_trivia_ad_xar(syn, &nodus->trivia_ante, old_ante);

    /* Capere trivia ex ')' et attachere */
    close_lex = _currens_lex(syn);
    si (close_lex != NIHIL && close_lex->genus == ARBOR_LEXEMA_PAREN_CLAUSA)
    {
        _copiare_trivia_ad_xar(syn, &nodus->trivia_post, close_lex->trivia_ante);
        _addere_synth_trivia(syn, &nodus->trivia_post, ")");
        _copiare_trivia_ad_xar(syn, &nodus->trivia_post, close_lex->trivia_post);
    }
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

    /* NON vocare _finire_nodum - operandum iam habet trivia */
    redde nodus;
}

interior ArborNodus*
_parsere_sizeof(ArborSyntaxis* syn)
{
    ArborNodus*  nodus;
    ArborNodus*  operandum;
    ArborLexema* sizeof_lex;
    Xar*         trivia_post_sizeof;

    nodus = _creare_nodum(syn, ARBOR_NODUS_SIZEOF_EXPRESSION);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    /* Capere trivia_post de 'sizeof' ante progredi */
    sizeof_lex = _currens_lex(syn);
    trivia_post_sizeof = (sizeof_lex != NIHIL) ? sizeof_lex->trivia_post : NIHIL;

    /* Skip sizeof */
    _progredi(syn);

    /* sizeof(type) vs sizeof expr */
    si (_congruit(syn, ARBOR_LEXEMA_PAREN_APERTA))
    {
        ArborLexema* open_paren_lex;
        ArborLexema* close_paren_lex;
        Xar*         trivia_post_open;
        Xar*         trivia_ante_open;

        /* Capere trivia de '(' */
        open_paren_lex = _currens_lex(syn);
        trivia_ante_open = (open_paren_lex != NIHIL) ? open_paren_lex->trivia_ante : NIHIL;
        trivia_post_open = (open_paren_lex != NIHIL) ? open_paren_lex->trivia_post : NIHIL;

        _progredi(syn);  /* Skip ( */

        /* Check if it looks like a type (including typedef names) */
        si (_est_type_initium(syn))
        {
            /* sizeof(type) - parse type-name */
            ArborNodus* type_node;
            ArborNodus** first_spec;

            nodus->datum.sizeof_expr.est_typus = VERUM;

            /* Parse type-name (specifiers + optional abstract declarator) */
            type_node = _parsere_type_name(syn);

            /* Capere trivia de ')' */
            close_paren_lex = _currens_lex(syn);

            /* Trivia handling: praepone trivia_post_sizeof + "(" + trivia_post_open ad primum spec */
            si (type_node != NIHIL)
            {
                first_spec = xar_obtinere(type_node->datum.parametrum.specifiers, ZEPHYRUM);
                si (first_spec != NIHIL && *first_spec != NIHIL)
                {
                    Xar* old_ante;
                    old_ante = (*first_spec)->trivia_ante;
                    (*first_spec)->trivia_ante = NIHIL;
                    _copiare_trivia_ad_xar(syn, &(*first_spec)->trivia_ante, trivia_post_sizeof);
                    _addere_synth_trivia(syn, &(*first_spec)->trivia_ante, "(");
                    _copiare_trivia_ad_xar(syn, &(*first_spec)->trivia_ante, trivia_post_open);
                    _copiare_trivia_ad_xar(syn, &(*first_spec)->trivia_ante, old_ante);
                }

                /* Appone ')' trivia ad type_node */
                si (close_paren_lex != NIHIL)
                {
                    _copiare_trivia_ad_xar(syn, &type_node->trivia_post, close_paren_lex->trivia_ante);
                }
                _addere_synth_trivia(syn, &type_node->trivia_post, ")");
                si (close_paren_lex != NIHIL)
                {
                    _copiare_trivia_ad_xar(syn, &type_node->trivia_post, close_paren_lex->trivia_post);
                }
            }
            operandum = type_node;

            _expectare(syn, ARBOR_LEXEMA_PAREN_CLAUSA, "Expectabatur )");
        }
        alioquin
        {
            /* sizeof(expr) - need to re-emit the '(' we consumed */
            ArborNodus* inner_expr;

            nodus->datum.sizeof_expr.est_typus = FALSUM;

            /* Parse expression inside parens */
            inner_expr = _parsere_expressio(syn, PREC_NIHIL);

            si (inner_expr != NIHIL)
            {
                Xar* old_ante;

                /* Praepone trivia_post_sizeof + "(" + trivia ad expressio */
                old_ante = inner_expr->trivia_ante;
                inner_expr->trivia_ante = NIHIL;
                _copiare_trivia_ad_xar(syn, &inner_expr->trivia_ante, trivia_post_sizeof);
                _copiare_trivia_ad_xar(syn, &inner_expr->trivia_ante, trivia_ante_open);
                _addere_synth_trivia(syn, &inner_expr->trivia_ante, "(");
                _copiare_trivia_ad_xar(syn, &inner_expr->trivia_ante, trivia_post_open);
                _copiare_trivia_ad_xar(syn, &inner_expr->trivia_ante, old_ante);

                /* Capere trivia de ')' et appone ad expressio.trivia_post */
                close_paren_lex = _currens_lex(syn);
                si (close_paren_lex != NIHIL)
                {
                    _copiare_trivia_ad_xar(syn, &inner_expr->trivia_post, close_paren_lex->trivia_ante);
                }
                _addere_synth_trivia(syn, &inner_expr->trivia_post, ")");
                si (close_paren_lex != NIHIL)
                {
                    _copiare_trivia_ad_xar(syn, &inner_expr->trivia_post, close_paren_lex->trivia_post);
                }
            }
            operandum = inner_expr;

            _expectare(syn, ARBOR_LEXEMA_PAREN_CLAUSA, "Expectabatur )");
        }
    }
    alioquin
    {
        /* sizeof expr - praepone trivia_post_sizeof ad expressio */
        nodus->datum.sizeof_expr.est_typus = FALSUM;
        operandum = _parsere_expressio(syn, PREC_UNARIUS);
        si (operandum != NIHIL && trivia_post_sizeof != NIHIL)
        {
            Xar* old_ante = operandum->trivia_ante;
            operandum->trivia_ante = NIHIL;
            _copiare_trivia_ad_xar(syn, &operandum->trivia_ante, trivia_post_sizeof);
            _copiare_trivia_ad_xar(syn, &operandum->trivia_ante, old_ante);
        }
    }

    si (operandum == NIHIL)
    {
        redde NIHIL;
    }

    nodus->datum.sizeof_expr.operandum = operandum;
    operandum->pater = nodus;

    /* NON vocare _finire_nodum - operandum iam habet trivia_post */
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
    Xar* trivia_post_op;
    Xar* old_ante;

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

    /* Capere trivia ex operator */
    _copiare_trivia_ad_xar(syn, &sinister->trivia_post, lex->trivia_ante);
    trivia_post_op = lex->trivia_post;

    /* Skip operator */
    _progredi(syn);

    /* Parsere dextrum */
    dexter = _parsere_expressio(syn, dexter_assoc ? praecedentia - I : praecedentia);
    si (dexter == NIHIL)
    {
        redde NIHIL;
    }

    /* Praepone trivia_post de operator ad dexter.trivia_ante */
    old_ante = dexter->trivia_ante;
    dexter->trivia_ante = NIHIL;
    _copiare_trivia_ad_xar(syn, &dexter->trivia_ante, trivia_post_op);
    _copiare_trivia_ad_xar(syn, &dexter->trivia_ante, old_ante);

    nodus->datum.binarium.dexter = dexter;
    dexter->pater = nodus;

    /* NON vocare _finire_nodum - liberi iam habent trivia */
    redde nodus;
}

interior ArborNodus*
_parsere_ternarium(ArborSyntaxis* syn, ArborNodus* conditio)
{
    ArborNodus* nodus;
    ArborNodus* verum_ramus;
    ArborNodus* falsum_ramus;
    ArborLexema* lex;
    Xar* trivia_post_q;
    Xar* trivia_post_colon;
    Xar* old_ante;

    lex = _currens_lex(syn);

    nodus = _creare_nodum(syn, ARBOR_NODUS_CONDITIONAL_EXPRESSION);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    nodus->datum.ternarium.conditio = conditio;
    conditio->pater = nodus;

    /* Capere trivia ex ? */
    si (lex != NIHIL)
    {
        _copiare_trivia_ad_xar(syn, &conditio->trivia_post, lex->trivia_ante);
        trivia_post_q = lex->trivia_post;
    }
    alioquin
    {
        trivia_post_q = NIHIL;
    }

    /* Skip ? */
    _progredi(syn);

    /* Parsere verum ramus */
    verum_ramus = _parsere_expressio(syn, PREC_NIHIL);
    si (verum_ramus == NIHIL)
    {
        redde NIHIL;
    }

    /* Praepone trivia_post de ? ad verum_ramus.trivia_ante */
    old_ante = verum_ramus->trivia_ante;
    verum_ramus->trivia_ante = NIHIL;
    _copiare_trivia_ad_xar(syn, &verum_ramus->trivia_ante, trivia_post_q);
    _copiare_trivia_ad_xar(syn, &verum_ramus->trivia_ante, old_ante);

    nodus->datum.ternarium.verum = verum_ramus;
    verum_ramus->pater = nodus;

    /* Capere trivia ex : */
    lex = _currens_lex(syn);
    si (lex != NIHIL)
    {
        _copiare_trivia_ad_xar(syn, &verum_ramus->trivia_post, lex->trivia_ante);
        trivia_post_colon = lex->trivia_post;
    }
    alioquin
    {
        trivia_post_colon = NIHIL;
    }

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

    /* Praepone trivia_post de : ad falsum_ramus.trivia_ante */
    old_ante = falsum_ramus->trivia_ante;
    falsum_ramus->trivia_ante = NIHIL;
    _copiare_trivia_ad_xar(syn, &falsum_ramus->trivia_ante, trivia_post_colon);
    _copiare_trivia_ad_xar(syn, &falsum_ramus->trivia_ante, old_ante);

    nodus->datum.ternarium.falsum = falsum_ramus;
    falsum_ramus->pater = nodus;

    /* NON vocare _finire_nodum - liberi iam habent trivia */
    redde nodus;
}

interior ArborNodus*
_parsere_assignatio(ArborSyntaxis* syn, ArborNodus* target)
{
    ArborNodus* nodus;
    ArborNodus* valor;
    ArborLexema* lex;
    Xar* trivia_post_eq;
    Xar* old_ante;

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

    /* Capere trivia ex '=' */
    _copiare_trivia_ad_xar(syn, &target->trivia_post, lex->trivia_ante);
    trivia_post_eq = lex->trivia_post;

    /* Skip operator */
    _progredi(syn);

    /* Parsere valorem (dexter associativum) */
    valor = _parsere_expressio(syn, PREC_ASSIGNATIO - I);
    si (valor == NIHIL)
    {
        redde NIHIL;
    }

    /* Praepone trivia_post de '=' ad valor.trivia_ante */
    old_ante = valor->trivia_ante;
    valor->trivia_ante = NIHIL;
    _copiare_trivia_ad_xar(syn, &valor->trivia_ante, trivia_post_eq);
    _copiare_trivia_ad_xar(syn, &valor->trivia_ante, old_ante);

    nodus->datum.assignatio.valor = valor;
    valor->pater = nodus;

    /* NON vocare _finire_nodum - liberi iam habent trivia */
    redde nodus;
}

interior ArborNodus*
_parsere_vocatio(ArborSyntaxis* syn, ArborNodus* callee)
{
    ArborNodus* nodus;
    ArborNodus* argumentum;
    ArborNodus* prev_arg;
    ArborLexema* comma_lex;
    Xar* trivia_post_comma;
    Xar* old_ante;

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
    prev_arg = NIHIL;
    trivia_post_comma = NIHIL;
    si (!_congruit(syn, ARBOR_LEXEMA_PAREN_CLAUSA))
    {
        fac {
            argumentum = _parsere_expressio(syn, PREC_ASSIGNATIO);
            si (argumentum == NIHIL)
            {
                redde NIHIL;
            }

            /* Praepone trivia_post de comma praecedenti */
            si (trivia_post_comma != NIHIL)
            {
                old_ante = argumentum->trivia_ante;
                argumentum->trivia_ante = NIHIL;
                _copiare_trivia_ad_xar(syn, &argumentum->trivia_ante, trivia_post_comma);
                _copiare_trivia_ad_xar(syn, &argumentum->trivia_ante, old_ante);
            }

            argumentum->pater = nodus;
            {
                ArborNodus** slot = xar_addere(nodus->datum.vocatio.argumenta);
                si (slot != NIHIL) { *slot = argumentum; }
            }
            prev_arg = argumentum;

            /* Capere comma trivia si praesens */
            si (_congruit(syn, ARBOR_LEXEMA_COMMA))
            {
                comma_lex = _currens_lex(syn);
                si (comma_lex != NIHIL && prev_arg != NIHIL)
                {
                    _copiare_trivia_ad_xar(syn, &prev_arg->trivia_post, comma_lex->trivia_ante);
                    trivia_post_comma = comma_lex->trivia_post;
                }
                _progredi(syn);
            }
            alioquin
            {
                frange;
            }
        } dum (VERUM);
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

/* Recursively clear duplicate trivia_ante from expression tree */
interior vacuum
_clarum_duplicatum_trivia(ArborNodus* nodus, Xar* trivia_parens)
{
    si (nodus == NIHIL || trivia_parens == NIHIL)
    {
        redde;
    }

    /* Si nodus habet eandem trivia_ante, clarum illud */
    si (nodus->trivia_ante == trivia_parens)
    {
        nodus->trivia_ante = NIHIL;
    }

    /* Recursively check children based on node type */
    commutatio (nodus->genus)
    {
    casus ARBOR_NODUS_ASSIGNMENT_EXPRESSION:
        _clarum_duplicatum_trivia(nodus->datum.assignatio.target, trivia_parens);
        frange;
    casus ARBOR_NODUS_BINARY_EXPRESSION:
        _clarum_duplicatum_trivia(nodus->datum.binarium.sinister, trivia_parens);
        frange;
    casus ARBOR_NODUS_UNARY_EXPRESSION:
        _clarum_duplicatum_trivia(nodus->datum.unarium.operandum, trivia_parens);
        frange;
    casus ARBOR_NODUS_CONDITIONAL_EXPRESSION:
        _clarum_duplicatum_trivia(nodus->datum.ternarium.conditio, trivia_parens);
        frange;
    casus ARBOR_NODUS_CALL_EXPRESSION:
        _clarum_duplicatum_trivia(nodus->datum.vocatio.callee, trivia_parens);
        frange;
    casus ARBOR_NODUS_SUBSCRIPT_EXPRESSION:
        _clarum_duplicatum_trivia(nodus->datum.subscriptum.array, trivia_parens);
        frange;
    casus ARBOR_NODUS_MEMBER_EXPRESSION:
        _clarum_duplicatum_trivia(nodus->datum.membrum.objectum, trivia_parens);
        frange;
    casus ARBOR_NODUS_CAST_EXPRESSION:
        _clarum_duplicatum_trivia(nodus->datum.conversio.expressio, trivia_parens);
        frange;
    casus ARBOR_NODUS_SIZEOF_EXPRESSION:
        _clarum_duplicatum_trivia(nodus->datum.sizeof_expr.operandum, trivia_parens);
        frange;
    casus ARBOR_NODUS_COMMA_EXPRESSION:
        si (nodus->datum.comma.expressiones != NIHIL &&
            xar_numerus(nodus->datum.comma.expressiones) > ZEPHYRUM)
        {
            ArborNodus** primum = xar_obtinere(nodus->datum.comma.expressiones, ZEPHYRUM);
            si (primum != NIHIL && *primum != NIHIL)
            {
                _clarum_duplicatum_trivia(*primum, trivia_parens);
            }
        }
        frange;
    /* Postfix expression - check first child */
    casus ARBOR_NODUS_POSTFIX_EXPRESSION:
        si (nodus->datum.genericum.liberi != NIHIL &&
            xar_numerus(nodus->datum.genericum.liberi) > ZEPHYRUM)
        {
            ArborNodus** primum = xar_obtinere(nodus->datum.genericum.liberi, ZEPHYRUM);
            si (primum != NIHIL && *primum != NIHIL)
            {
                _clarum_duplicatum_trivia(*primum, trivia_parens);
            }
        }
        frange;
    ordinarius:
        /* Terminal nodes (identifier, literals) - trivia already cleared above */
        frange;
    }
}

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

    /* Clear duplicate trivia from expression tree */
    _clarum_duplicatum_trivia(expr, nodus->trivia_ante);

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
    ArborLexema* if_lex;
    ArborLexema* open_lex;
    ArborLexema* close_lex;
    Xar* trivia_post_if;
    Xar* trivia_post_open;
    Xar* trivia_post_close;
    Xar* old_ante;

    nodus = _creare_nodum(syn, ARBOR_NODUS_IF_STATEMENT);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    /* Capere trivia ex 'if' ante skip */
    if_lex = _currens_lex(syn);
    trivia_post_if = (if_lex != NIHIL) ? if_lex->trivia_post : NIHIL;

    /* Skip if */
    _progredi(syn);

    /* Capere trivia ex '(' */
    open_lex = _currens_lex(syn);
    trivia_post_open = (open_lex != NIHIL) ? open_lex->trivia_post : NIHIL;

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

    /* Praepone trivia: keyword_text + trivia_post_if + "(" + trivia_post_open */
    old_ante = conditio->trivia_ante;
    conditio->trivia_ante = NIHIL;
    /* Preserve original keyword text (si -> if, etc.) */
    si (if_lex != NIHIL)
    {
        _addere_synth_trivia_chorda(syn, &conditio->trivia_ante, if_lex->valor);
    }
    _copiare_trivia_ad_xar(syn, &conditio->trivia_ante, trivia_post_if);
    _addere_synth_trivia(syn, &conditio->trivia_ante, "(");
    _copiare_trivia_ad_xar(syn, &conditio->trivia_ante, trivia_post_open);
    _copiare_trivia_ad_xar(syn, &conditio->trivia_ante, old_ante);

    nodus->datum.conditionale.conditio = conditio;
    conditio->pater = nodus;

    /* Capere trivia ex ')' */
    close_lex = _currens_lex(syn);
    trivia_post_close = (close_lex != NIHIL) ? close_lex->trivia_post : NIHIL;

    /* Attachere ')' et trivia ad conditio.trivia_post */
    _copiare_trivia_ad_xar(syn, &conditio->trivia_post, (close_lex != NIHIL) ? close_lex->trivia_ante : NIHIL);
    _addere_synth_trivia(syn, &conditio->trivia_post, ")");
    _copiare_trivia_ad_xar(syn, &conditio->trivia_post, trivia_post_close);

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

    /* optionalis else (vel alioquin keyword macro) */
    si (_congruit_effectivum(syn, ARBOR_LEXEMA_ELSE))
    {
        ArborLexema* else_lex;
        Xar*         trivia_post_else;
        Xar*         old_ante_alt;

        /* Capere trivia ex 'else' */
        else_lex = _currens_lex(syn);

        /* Attachere trivia_ante + keyword_text (else/alioquin) ad consequens.trivia_post */
        si (else_lex != NIHIL)
        {
            _copiare_trivia_ad_xar(syn, &consequens->trivia_post, else_lex->trivia_ante);
            /* Preserve original keyword text (alioquin -> else, etc.) */
            _addere_synth_trivia_chorda(syn, &consequens->trivia_post, else_lex->valor);
        }
        trivia_post_else = (else_lex != NIHIL) ? else_lex->trivia_post : NIHIL;

        _progredi(syn);

        alternans = _parsere_sententia(syn);
        si (alternans == NIHIL)
        {
            redde _creare_error_nodum(syn, "else sine corpore");
        }

        /* Praepone trivia_post_else ad alternans.trivia_ante */
        si (trivia_post_else != NIHIL)
        {
            old_ante_alt = alternans->trivia_ante;
            alternans->trivia_ante = NIHIL;
            _copiare_trivia_ad_xar(syn, &alternans->trivia_ante, trivia_post_else);
            _copiare_trivia_ad_xar(syn, &alternans->trivia_ante, old_ante_alt);
        }

        nodus->datum.conditionale.alternans = alternans;
        alternans->pater = nodus;
    }
    alioquin
    {
        nodus->datum.conditionale.alternans = NIHIL;
    }

    /* NON vocare _finire_nodum - consequens/alternans iam habet trivia_post */
    redde nodus;
}

interior ArborNodus*
_parsere_while(ArborSyntaxis* syn)
{
    ArborNodus*  nodus;
    ArborNodus*  conditio;
    ArborNodus*  corpus;
    ArborLexema* while_lex;
    ArborLexema* open_lex;
    ArborLexema* close_lex;
    Xar*         trivia_post_while;
    Xar*         trivia_post_open;
    Xar*         old_ante;

    nodus = _creare_nodum(syn, ARBOR_NODUS_WHILE_STATEMENT);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    /* Capere trivia ex 'while' ante skip */
    while_lex = _currens_lex(syn);
    trivia_post_while = (while_lex != NIHIL) ? while_lex->trivia_post : NIHIL;

    /* Skip while */
    _progredi(syn);

    /* Capere trivia ex '(' */
    open_lex = _currens_lex(syn);
    trivia_post_open = (open_lex != NIHIL) ? open_lex->trivia_post : NIHIL;

    /* ( conditio ) */
    _expectare(syn, ARBOR_LEXEMA_PAREN_APERTA, "Expectabatur (");
    conditio = _parsere_expressio(syn, PREC_NIHIL);
    si (conditio == NIHIL)
    {
        redde NIHIL;
    }

    /* Praepone trivia: trivia_post_while + "(" + trivia_post_open */
    old_ante = conditio->trivia_ante;
    conditio->trivia_ante = NIHIL;
    _copiare_trivia_ad_xar(syn, &conditio->trivia_ante, trivia_post_while);
    _addere_synth_trivia(syn, &conditio->trivia_ante, "(");
    _copiare_trivia_ad_xar(syn, &conditio->trivia_ante, trivia_post_open);
    _copiare_trivia_ad_xar(syn, &conditio->trivia_ante, old_ante);

    nodus->datum.iteratio.conditio = conditio;
    conditio->pater = nodus;

    /* Capere trivia ex ')' */
    close_lex = _currens_lex(syn);

    /* Attachere ')' et trivia_post ad conditio.trivia_post */
    _copiare_trivia_ad_xar(syn, &conditio->trivia_post, (close_lex != NIHIL) ? close_lex->trivia_ante : NIHIL);
    _addere_synth_trivia(syn, &conditio->trivia_post, ")");
    _copiare_trivia_ad_xar(syn, &conditio->trivia_post, (close_lex != NIHIL) ? close_lex->trivia_post : NIHIL);

    _expectare(syn, ARBOR_LEXEMA_PAREN_CLAUSA, "Expectabatur )");

    /* corpus */
    corpus = _parsere_sententia(syn);
    si (corpus == NIHIL)
    {
        redde NIHIL;
    }
    nodus->datum.iteratio.corpus = corpus;
    corpus->pater = nodus;

    /* NON vocare _finire_nodum - corpus iam habet trivia_post */
    redde nodus;
}

interior ArborNodus*
_parsere_do(ArborSyntaxis* syn)
{
    ArborNodus*  nodus;
    ArborNodus*  conditio;
    ArborNodus*  corpus;
    ArborLexema* do_lex;
    ArborLexema* while_lex;
    ArborLexema* open_lex;
    ArborLexema* close_lex;
    ArborLexema* semi_lex;
    Xar*         trivia_post_do;
    Xar*         trivia_post_while;
    Xar*         trivia_post_open;
    Xar*         old_ante;

    nodus = _creare_nodum(syn, ARBOR_NODUS_DO_STATEMENT);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    /* Capere trivia_post de 'do' ante progredi */
    do_lex = _currens_lex(syn);
    trivia_post_do = (do_lex != NIHIL) ? do_lex->trivia_post : NIHIL;

    /* Skip do */
    _progredi(syn);

    /* corpus */
    corpus = _parsere_sententia(syn);
    si (corpus == NIHIL)
    {
        redde NIHIL;
    }

    /* Praepone trivia_post_do ad corpus.trivia_ante */
    si (trivia_post_do != NIHIL)
    {
        old_ante = corpus->trivia_ante;
        corpus->trivia_ante = NIHIL;
        _copiare_trivia_ad_xar(syn, &corpus->trivia_ante, trivia_post_do);
        _copiare_trivia_ad_xar(syn, &corpus->trivia_ante, old_ante);
    }

    nodus->datum.iteratio.corpus = corpus;
    corpus->pater = nodus;

    /* Capere 'while' trivia */
    while_lex = _currens_lex(syn);
    si (while_lex != NIHIL)
    {
        /* while trivia_ante -> corpus.trivia_post */
        _copiare_trivia_ad_xar(syn, &corpus->trivia_post, while_lex->trivia_ante);
        _addere_synth_trivia(syn, &corpus->trivia_post, "while");
        trivia_post_while = while_lex->trivia_post;
    }
    alioquin
    {
        trivia_post_while = NIHIL;
    }

    _expectare(syn, ARBOR_LEXEMA_WHILE, "Expectabatur 'while'");

    /* Capere '(' trivia */
    open_lex = _currens_lex(syn);
    trivia_post_open = (open_lex != NIHIL) ? open_lex->trivia_post : NIHIL;

    _expectare(syn, ARBOR_LEXEMA_PAREN_APERTA, "Expectabatur (");

    conditio = _parsere_expressio(syn, PREC_NIHIL);
    si (conditio == NIHIL)
    {
        redde NIHIL;
    }

    /* Praepone trivia: trivia_post_while + "(" + trivia_post_open ad conditio.trivia_ante */
    old_ante = conditio->trivia_ante;
    conditio->trivia_ante = NIHIL;
    _copiare_trivia_ad_xar(syn, &conditio->trivia_ante, trivia_post_while);
    _addere_synth_trivia(syn, &conditio->trivia_ante, "(");
    _copiare_trivia_ad_xar(syn, &conditio->trivia_ante, trivia_post_open);
    _copiare_trivia_ad_xar(syn, &conditio->trivia_ante, old_ante);

    nodus->datum.iteratio.conditio = conditio;
    conditio->pater = nodus;

    /* Capere ')' trivia */
    close_lex = _currens_lex(syn);
    _copiare_trivia_ad_xar(syn, &conditio->trivia_post, (close_lex != NIHIL) ? close_lex->trivia_ante : NIHIL);
    _addere_synth_trivia(syn, &conditio->trivia_post, ")");
    _copiare_trivia_ad_xar(syn, &conditio->trivia_post, (close_lex != NIHIL) ? close_lex->trivia_post : NIHIL);

    _expectare(syn, ARBOR_LEXEMA_PAREN_CLAUSA, "Expectabatur )");

    /* Capere ';' trivia */
    semi_lex = _currens_lex(syn);
    _copiare_trivia_ad_xar(syn, &conditio->trivia_post, (semi_lex != NIHIL) ? semi_lex->trivia_ante : NIHIL);
    _addere_synth_trivia(syn, &conditio->trivia_post, ";");

    _expectare(syn, ARBOR_LEXEMA_SEMICOLON, "Expectabatur ;");

    /* Capere trivia_post de ';' */
    _finire_nodum(syn, nodus);
    redde nodus;
}

interior ArborNodus*
_parsere_for(ArborSyntaxis* syn)
{
    ArborNodus*  nodus;
    ArborNodus*  init;
    ArborNodus*  conditio;
    ArborNodus*  post;
    ArborNodus*  corpus;
    ArborLexema* for_lex;
    ArborLexema* open_lex;
    ArborLexema* semi1_lex;
    ArborLexema* semi2_lex;
    ArborLexema* close_lex;
    Xar*         trivia_post_for;
    Xar*         trivia_post_open;
    Xar*         trivia_post_semi1;
    Xar*         trivia_post_semi2;
    Xar*         old_ante;

    nodus = _creare_nodum(syn, ARBOR_NODUS_FOR_STATEMENT);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    /* Capere trivia ex 'for' */
    for_lex = _currens_lex(syn);
    trivia_post_for = (for_lex != NIHIL) ? for_lex->trivia_post : NIHIL;

    /* Skip for */
    _progredi(syn);

    /* Capere trivia ex '(' */
    open_lex = _currens_lex(syn);
    trivia_post_open = (open_lex != NIHIL) ? open_lex->trivia_post : NIHIL;

    _expectare(syn, ARBOR_LEXEMA_PAREN_APERTA, "Expectabatur (");

    /* init (optionalis) */
    si (_congruit(syn, ARBOR_LEXEMA_SEMICOLON))
    {
        init = NIHIL;
    }
    alioquin si (_est_declaration_start(syn))
    {
        init = _parsere_declaratio(syn);
        /* Declaratio iam habet ; - applicare trivia ad init */
        si (init != NIHIL)
        {
            old_ante = init->trivia_ante;
            init->trivia_ante = NIHIL;
            _copiare_trivia_ad_xar(syn, &init->trivia_ante, trivia_post_for);
            _addere_synth_trivia(syn, &init->trivia_ante, "(");
            _copiare_trivia_ad_xar(syn, &init->trivia_ante, trivia_post_open);
            _copiare_trivia_ad_xar(syn, &init->trivia_ante, old_ante);
        }
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

        /* Capere trivia ex ';' ante conditio */
        semi2_lex = _currens_lex(syn);
        si (conditio != NIHIL)
        {
            _copiare_trivia_ad_xar(syn, &conditio->trivia_post, (semi2_lex != NIHIL) ? semi2_lex->trivia_ante : NIHIL);
            _addere_synth_trivia(syn, &conditio->trivia_post, ";");
        }
        trivia_post_semi2 = (semi2_lex != NIHIL) ? semi2_lex->trivia_post : NIHIL;

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
            /* Applicare trivia_post_semi2 ad post.trivia_ante */
            si (post != NIHIL && trivia_post_semi2 != NIHIL)
            {
                old_ante = post->trivia_ante;
                post->trivia_ante = NIHIL;
                _copiare_trivia_ad_xar(syn, &post->trivia_ante, trivia_post_semi2);
                _copiare_trivia_ad_xar(syn, &post->trivia_ante, old_ante);
            }
        }
        nodus->datum.circuitus.post = post;
        si (post != NIHIL) post->pater = nodus;

        /* Capere trivia ex ')' */
        close_lex = _currens_lex(syn);
        si (post != NIHIL)
        {
            _copiare_trivia_ad_xar(syn, &post->trivia_post, (close_lex != NIHIL) ? close_lex->trivia_ante : NIHIL);
            _addere_synth_trivia(syn, &post->trivia_post, ")");
            _copiare_trivia_ad_xar(syn, &post->trivia_post, (close_lex != NIHIL) ? close_lex->trivia_post : NIHIL);
        }
        alioquin si (conditio != NIHIL)
        {
            _addere_synth_trivia(syn, &conditio->trivia_post, ")");
            _copiare_trivia_ad_xar(syn, &conditio->trivia_post, (close_lex != NIHIL) ? close_lex->trivia_post : NIHIL);
        }

        _expectare(syn, ARBOR_LEXEMA_PAREN_CLAUSA, "Expectabatur )");

        /* corpus */
        corpus = _parsere_sententia(syn);
        si (corpus == NIHIL)
        {
            redde NIHIL;
        }
        nodus->datum.circuitus.corpus = corpus;
        corpus->pater = nodus;

        /* NON vocare _finire_nodum - corpus iam habet trivia_post */
        redde nodus;
    }
    alioquin
    {
        init = _parsere_expressio(syn, PREC_NIHIL);
    }

    /* Applicare trivia: keyword + trivia_post_for + "(" + trivia_post_open ad init */
    si (init != NIHIL)
    {
        old_ante = init->trivia_ante;
        init->trivia_ante = NIHIL;
        /* Preserve original keyword text (per -> for, etc.) */
        si (for_lex != NIHIL)
        {
            _addere_synth_trivia_chorda(syn, &init->trivia_ante, for_lex->valor);
        }
        _copiare_trivia_ad_xar(syn, &init->trivia_ante, trivia_post_for);
        _addere_synth_trivia(syn, &init->trivia_ante, "(");
        _copiare_trivia_ad_xar(syn, &init->trivia_ante, trivia_post_open);
        _copiare_trivia_ad_xar(syn, &init->trivia_ante, old_ante);
    }

    /* Capere trivia ex prima ';' */
    semi1_lex = _currens_lex(syn);
    si (init != NIHIL)
    {
        _copiare_trivia_ad_xar(syn, &init->trivia_post, (semi1_lex != NIHIL) ? semi1_lex->trivia_ante : NIHIL);
        _addere_synth_trivia(syn, &init->trivia_post, ";");
    }
    trivia_post_semi1 = (semi1_lex != NIHIL) ? semi1_lex->trivia_post : NIHIL;

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
        /* Applicare trivia_post_semi1 ad conditio.trivia_ante */
        si (conditio != NIHIL && trivia_post_semi1 != NIHIL)
        {
            old_ante = conditio->trivia_ante;
            conditio->trivia_ante = NIHIL;
            _copiare_trivia_ad_xar(syn, &conditio->trivia_ante, trivia_post_semi1);
            _copiare_trivia_ad_xar(syn, &conditio->trivia_ante, old_ante);
        }
    }

    /* Capere trivia ex secunda ';' */
    semi2_lex = _currens_lex(syn);
    si (conditio != NIHIL)
    {
        _copiare_trivia_ad_xar(syn, &conditio->trivia_post, (semi2_lex != NIHIL) ? semi2_lex->trivia_ante : NIHIL);
        _addere_synth_trivia(syn, &conditio->trivia_post, ";");
    }
    trivia_post_semi2 = (semi2_lex != NIHIL) ? semi2_lex->trivia_post : NIHIL;

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
        /* Applicare trivia_post_semi2 ad post.trivia_ante */
        si (post != NIHIL && trivia_post_semi2 != NIHIL)
        {
            old_ante = post->trivia_ante;
            post->trivia_ante = NIHIL;
            _copiare_trivia_ad_xar(syn, &post->trivia_ante, trivia_post_semi2);
            _copiare_trivia_ad_xar(syn, &post->trivia_ante, old_ante);
        }
    }

    /* Capere trivia ex ')' */
    close_lex = _currens_lex(syn);
    si (post != NIHIL)
    {
        _copiare_trivia_ad_xar(syn, &post->trivia_post, (close_lex != NIHIL) ? close_lex->trivia_ante : NIHIL);
        _addere_synth_trivia(syn, &post->trivia_post, ")");
        /* Addere trivia_post ex ')' (spatium ante corpus) */
        _copiare_trivia_ad_xar(syn, &post->trivia_post, (close_lex != NIHIL) ? close_lex->trivia_post : NIHIL);
    }
    alioquin si (conditio != NIHIL)
    {
        _addere_synth_trivia(syn, &conditio->trivia_post, ")");
        _copiare_trivia_ad_xar(syn, &conditio->trivia_post, (close_lex != NIHIL) ? close_lex->trivia_post : NIHIL);
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

    /* NON vocare _finire_nodum - corpus iam habet trivia_post */
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
    ArborNodus*  nodus;
    ArborNodus*  valor;
    ArborLexema* return_lex;
    ArborLexema* semi_lex;
    Xar*         trivia_post_return;
    Xar*         old_ante;

    nodus = _creare_nodum(syn, ARBOR_NODUS_RETURN_STATEMENT);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    /* Capere trivia_post de 'return' ante progredi */
    return_lex = _currens_lex(syn);
    trivia_post_return = (return_lex != NIHIL) ? return_lex->trivia_post : NIHIL;

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

        /* Praepone keyword + trivia_post_return ad valor.trivia_ante */
        old_ante = valor->trivia_ante;
        valor->trivia_ante = NIHIL;
        si (return_lex != NIHIL)
        {
            _addere_synth_trivia_chorda(syn, &valor->trivia_ante, return_lex->valor);
        }
        _copiare_trivia_ad_xar(syn, &valor->trivia_ante, trivia_post_return);
        _copiare_trivia_ad_xar(syn, &valor->trivia_ante, old_ante);

        /* Capere ';' trivia */
        semi_lex = _currens_lex(syn);
        _copiare_trivia_ad_xar(syn, &valor->trivia_post, (semi_lex != NIHIL) ? semi_lex->trivia_ante : NIHIL);
        _addere_synth_trivia(syn, &valor->trivia_post, ";");
        _copiare_trivia_ad_xar(syn, &valor->trivia_post, (semi_lex != NIHIL) ? semi_lex->trivia_post : NIHIL);

        nodus->datum.reditio.valor = valor;
        valor->pater = nodus;
    }
    alioquin
    {
        /* return; sine valore - trivia ad nodum ipsum */
        nodus->datum.reditio.valor = NIHIL;

        /* Keyword ad trivia_ante */
        si (return_lex != NIHIL)
        {
            _addere_synth_trivia_chorda(syn, &nodus->trivia_ante, return_lex->valor);
        }

        /* Capere ';' trivia */
        semi_lex = _currens_lex(syn);
        _copiare_trivia_ad_xar(syn, &nodus->trivia_post, trivia_post_return);
        _copiare_trivia_ad_xar(syn, &nodus->trivia_post, (semi_lex != NIHIL) ? semi_lex->trivia_ante : NIHIL);
        _addere_synth_trivia(syn, &nodus->trivia_post, ";");
        _copiare_trivia_ad_xar(syn, &nodus->trivia_post, (semi_lex != NIHIL) ? semi_lex->trivia_post : NIHIL);
    }

    _expectare(syn, ARBOR_LEXEMA_SEMICOLON, "Expectabatur ;");

    /* NON vocare _finire_nodum - valor iam habet trivia_post cum ';' */
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
    ArborNodus*  nodus;
    ArborLexema* continue_lex;
    ArborLexema* semi_lex;

    nodus = _creare_nodum(syn, ARBOR_NODUS_CONTINUE_STATEMENT);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    /* Capere keyword */
    continue_lex = _currens_lex(syn);

    /* Skip continue */
    _progredi(syn);

    /* Keyword ad trivia_ante */
    si (continue_lex != NIHIL)
    {
        _addere_synth_trivia_chorda(syn, &nodus->trivia_ante, continue_lex->valor);
    }

    /* Capere ';' trivia */
    semi_lex = _currens_lex(syn);
    _copiare_trivia_ad_xar(syn, &nodus->trivia_post, (continue_lex != NIHIL) ? continue_lex->trivia_post : NIHIL);
    _copiare_trivia_ad_xar(syn, &nodus->trivia_post, (semi_lex != NIHIL) ? semi_lex->trivia_ante : NIHIL);
    _addere_synth_trivia(syn, &nodus->trivia_post, ";");
    _copiare_trivia_ad_xar(syn, &nodus->trivia_post, (semi_lex != NIHIL) ? semi_lex->trivia_post : NIHIL);

    _expectare(syn, ARBOR_LEXEMA_SEMICOLON, "Expectabatur ;");

    /* NON vocare _finire_nodum - iam habet trivia */
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
    ArborNodus* primum_item;
    ArborNodus* ultimum_item;
    i32 prev_pos;
    ArborLexema* open_lex;
    ArborLexema* close_lex;
    Xar* trivia_post_open;

    nodus = _creare_nodum(syn, ARBOR_NODUS_COMPOUND_STATEMENT);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    nodus->datum.compositum.sententiae = xar_creare(syn->piscina, magnitudo(ArborNodus*));

    /* Capere trivia_post ex '{' ante progredi */
    open_lex = _currens_lex(syn);
    trivia_post_open = (open_lex != NIHIL) ? open_lex->trivia_post : NIHIL;

    /* Skip { - trivia_ante iam captum per _creare_nodum */
    _progredi(syn);

    primum_item = NIHIL;
    ultimum_item = NIHIL;

    /* Parsere sententias et declarationes */
    dum (!_est_finis(syn) && !_congruit(syn, ARBOR_LEXEMA_BRACE_CLAUSA))
    {
        prev_pos = syn->positus;

        /* Verificare si directiva (PRESERVARE mode) */
        si (_est_directiva_initium(syn))
        {
            item = _parsere_directiva(syn);
        }
        alioquin si (_est_declaration_start(syn))
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

        /* Attachere trivia_post de '{' ad primum item */
        si (primum_item == NIHIL)
        {
            primum_item = item;
            si (trivia_post_open != NIHIL)
            {
                /* Praepone trivia_post de '{' ad trivia_ante de primo item */
                Xar* old_ante = item->trivia_ante;
                item->trivia_ante = NIHIL;
                _copiare_trivia_ad_xar(syn, &item->trivia_ante, trivia_post_open);
                _copiare_trivia_ad_xar(syn, &item->trivia_ante, old_ante);
            }
        }

        ultimum_item = item;

        item->pater = nodus;
        {
            ArborNodus** slot = xar_addere(nodus->datum.compositum.sententiae);
            si (slot != NIHIL) { *slot = item; }
        }
    }

    /* Capere trivia_ante ex '}' et attachere ad ultimum item vel nodus */
    close_lex = _currens_lex(syn);
    si (close_lex != NIHIL)
    {
        si (ultimum_item != NIHIL)
        {
            _copiare_trivia_ad_xar(syn, &ultimum_item->trivia_post, close_lex->trivia_ante);
        }
        alioquin
        {
            /* Corpus vacuum - attachere trivia inter {} ad trivia_vacuum */
            _copiare_trivia_ad_xar(syn, &nodus->datum.compositum.trivia_vacuum, close_lex->trivia_ante);
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
    ArborLexemaGenus effective_genus;

    si (syn->panico)
    {
        redde NIHIL;
    }

    lex = _currens_lex(syn);
    si (lex == NIHIL || lex->genus == ARBOR_LEXEMA_EOF)
    {
        redde NIHIL;
    }

    /* Determinate effective genus - check keyword macros for identifiers */
    effective_genus = lex->genus;
    si (lex->genus == ARBOR_LEXEMA_IDENTIFICATOR && syn->keyword_macros != NIHIL)
    {
        chorda* titulus = chorda_internare(syn->intern, lex->valor);
        vacuum* keyword_val = NIHIL;
        si (tabula_dispersa_invenire(syn->keyword_macros, *titulus, &keyword_val))
        {
            effective_genus = (ArborLexemaGenus)(*(i32*)keyword_val);
        }
    }

    commutatio (effective_genus)
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

/* Parse struct member declaration (allows bitfields) */
interior ArborNodus*
_parsere_struct_declaration(ArborSyntaxis* syn)
{
    ArborNodus* nodus;
    Xar* specifiers;
    Xar* declaratores;
    ArborLexema* lex;

    nodus = _creare_nodum(syn, ARBOR_NODUS_DECLARATION);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    specifiers = xar_creare(syn->piscina, magnitudo(ArborNodus*));
    declaratores = xar_creare(syn->piscina, magnitudo(ArborNodus*));

    /* Parse type specifiers (including typedef names) */
    {
        b32 habet_typedef_nomen = FALSUM;
        lex = _currens_lex(syn);
        dum (lex != NIHIL)
        {
            ArborNodus* spec = NIHIL;

            si (_est_type_specifier(lex->genus))
            {
                spec = _parsere_type_specifier(syn);
            }
            alioquin si (_est_type_qualifier(lex->genus))
            {
                spec = _parsere_type_qualifier(syn);
            }
            alioquin si (lex->genus == ARBOR_LEXEMA_IDENTIFICATOR && !habet_typedef_nomen)
            {
                /* Verifica typedef nomen - solum unum permissum */
                chorda* titulus = chorda_internare(syn->intern, lex->valor);
                si (_est_typedef_nomen(syn, titulus))
                {
                    spec = _creare_nodum(syn, ARBOR_NODUS_TYPEDEF_NAME);
                    si (spec != NIHIL)
                    {
                        spec->datum.folium.valor = titulus;
                        _progredi(syn);
                        _finire_nodum(syn, spec);
                    }
                    habet_typedef_nomen = VERUM;
                }
                alioquin
                {
                    frange;
                }
            }
            alioquin
            {
                frange;
            }

            si (spec != NIHIL)
            {
                ArborNodus** slot;
                /* Clear duplicate trivia from first specifier */
                si (spec->trivia_ante == nodus->trivia_ante)
                {
                    spec->trivia_ante = NIHIL;
                }
                slot = xar_addere(specifiers);
                si (slot != NIHIL) { *slot = spec; spec->pater = nodus; }
            }
            lex = _currens_lex(syn);
        }
    }

    /* Parse struct declarators with optional bitfields */
    si (!_congruit(syn, ARBOR_LEXEMA_SEMICOLON))
    {
        fac
        {
            ArborNodus* init_decl;
            ArborNodus* declarator = NIHIL;
            ArborNodus* bitfield = NIHIL;

            init_decl = _creare_nodum(syn, ARBOR_NODUS_INIT_DECLARATOR);
            si (init_decl == NIHIL)
            {
                redde NIHIL;
            }

            /* Optional declarator (can be just `: width` for anonymous bitfield) */
            si (!_congruit(syn, ARBOR_LEXEMA_COLON))
            {
                declarator = _parsere_declarator(syn);
            }

            /* Optional bitfield width */
            si (_consumere(syn, ARBOR_LEXEMA_COLON))
            {
                bitfield = _parsere_expressio(syn, PREC_TERNARIUS);
            }

            init_decl->datum.init_decl.declarator = declarator;
            init_decl->datum.init_decl.initializer = bitfield;  /* Repurpose for bitfield */
            si (declarator != NIHIL) declarator->pater = init_decl;
            si (bitfield != NIHIL) bitfield->pater = init_decl;

            _finire_nodum(syn, init_decl);
            init_decl->pater = nodus;
            {
                ArborNodus** slot = xar_addere(declaratores);
                si (slot != NIHIL) { *slot = init_decl; }
            }
        } dum (_consumere(syn, ARBOR_LEXEMA_COMMA));
    }

    _expectare(syn, ARBOR_LEXEMA_SEMICOLON, "Expectabatur ;");
    nodus->datum.declaratio.specifiers = specifiers;
    nodus->datum.declaratio.declaratores = declaratores;
    _finire_nodum(syn, nodus);
    _attachere_punctuationem(syn, nodus, ";");
    redde nodus;
}

/* Parse struct or union specifier */
interior ArborNodus*
_parsere_struct_specifier(ArborSyntaxis* syn, b32 est_unio)
{
    ArborNodus* nodus;
    ArborNodusGenus genus;
    chorda* titulus;
    Xar* trivia_post_tag;
    ArborLexema* lex;

    titulus = NIHIL;
    trivia_post_tag = NIHIL;

    genus = est_unio ? ARBOR_NODUS_UNION_SPECIFIER : ARBOR_NODUS_STRUCT_SPECIFIER;
    nodus = _creare_nodum(syn, genus);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    /* Skip 'struct' or 'union' */
    _progredi(syn);

    /* Optional tag name */
    si (_congruit(syn, ARBOR_LEXEMA_IDENTIFICATOR))
    {
        lex = _currens_lex(syn);
        titulus = chorda_internare(syn->intern, lex->valor);
        trivia_post_tag = lex->trivia_post;  /* Spatium ante '{' */
        _progredi(syn);
    }
    nodus->datum.aggregatum.titulus = titulus;

    /* Optional { members } */
    si (_congruit(syn, ARBOR_LEXEMA_BRACE_APERTA))
    {
        ArborLexema* open_lex;
        ArborLexema* close_lex;
        Xar* trivia_post_open;
        ArborNodus* primum_membrum;
        ArborNodus* ultimum_membrum;
        ArborNodus* membrum;
        ArborNodus** slot;
        Xar* old_ante;

        open_lex = _currens_lex(syn);
        trivia_post_open = (open_lex != NIHIL) ? open_lex->trivia_post : NIHIL;
        primum_membrum = NIHIL;
        ultimum_membrum = NIHIL;

        _progredi(syn);  /* Skip { */

        nodus->datum.aggregatum.membra = xar_creare(syn->piscina, magnitudo(ArborNodus*));
        dum (!_congruit(syn, ARBOR_LEXEMA_BRACE_CLAUSA) && !_est_finis(syn))
        {
            membrum = _parsere_struct_declaration(syn);
            si (membrum != NIHIL)
            {
                /* Attachere trivia ad primum membrum */
                si (primum_membrum == NIHIL)
                {
                    primum_membrum = membrum;
                    /* Praepone: trivia_tag + "{" + trivia_open + original */
                    old_ante = membrum->trivia_ante;
                    membrum->trivia_ante = NIHIL;
                    _copiare_trivia_ad_xar(syn, &membrum->trivia_ante, trivia_post_tag);
                    _addere_synth_trivia(syn, &membrum->trivia_ante, "{");
                    _copiare_trivia_ad_xar(syn, &membrum->trivia_ante, trivia_post_open);
                    _copiare_trivia_ad_xar(syn, &membrum->trivia_ante, old_ante);
                }
                ultimum_membrum = membrum;

                slot = xar_addere(nodus->datum.aggregatum.membra);
                si (slot != NIHIL) { *slot = membrum; membrum->pater = nodus; }
            }
        }

        /* Capere trivia_ante ex '}' et attachere ad ultimum membrum cum '}' synthetico */
        close_lex = _currens_lex(syn);
        si (close_lex != NIHIL && ultimum_membrum != NIHIL)
        {
            _copiare_trivia_ad_xar(syn, &ultimum_membrum->trivia_post, close_lex->trivia_ante);
            _addere_synth_trivia(syn, &ultimum_membrum->trivia_post, "}");
        }

        _expectare(syn, ARBOR_LEXEMA_BRACE_CLAUSA, "Expectabatur }");
    }
    alioquin
    {
        nodus->datum.aggregatum.membra = NIHIL;  /* Forward decl or reference */
    }

    _finire_nodum(syn, nodus);
    redde nodus;
}

/* Parse enumerator constant */
interior ArborNodus*
_parsere_enumerator(ArborSyntaxis* syn)
{
    ArborNodus* nodus;
    ArborLexema* lex;

    si (!_congruit(syn, ARBOR_LEXEMA_IDENTIFICATOR))
    {
        redde NIHIL;
    }

    nodus = _creare_nodum(syn, ARBOR_NODUS_ENUMERATOR);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    lex = _currens_lex(syn);
    nodus->datum.folium.valor = chorda_internare(syn->intern, lex->valor);
    _progredi(syn);

    /* Optional = value - use genericum.liberi to store */
    si (_consumere(syn, ARBOR_LEXEMA_ASSIGNATIO))
    {
        ArborNodus* valor = _parsere_expressio(syn, PREC_TERNARIUS);
        si (valor != NIHIL)
        {
            ArborNodus** slot;
            nodus->datum.genericum.liberi = xar_creare(syn->piscina, magnitudo(ArborNodus*));
            slot = xar_addere(nodus->datum.genericum.liberi);
            si (slot != NIHIL) { *slot = valor; valor->pater = nodus; }
        }
    }

    _finire_nodum(syn, nodus);
    redde nodus;
}

/* Parse enum specifier */
interior ArborNodus*
_parsere_enum_specifier(ArborSyntaxis* syn)
{
    ArborNodus* nodus;
    chorda* titulus = NIHIL;

    nodus = _creare_nodum(syn, ARBOR_NODUS_ENUM_SPECIFIER);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    /* Skip 'enum' */
    _progredi(syn);

    /* Optional tag name */
    si (_congruit(syn, ARBOR_LEXEMA_IDENTIFICATOR))
    {
        ArborLexema* lex = _currens_lex(syn);
        titulus = chorda_internare(syn->intern, lex->valor);
        _progredi(syn);
    }
    nodus->datum.enum_spec.titulus = titulus;

    /* Optional { enumerators } */
    si (_consumere(syn, ARBOR_LEXEMA_BRACE_APERTA))
    {
        nodus->datum.enum_spec.enumeratores = xar_creare(syn->piscina, magnitudo(ArborNodus*));
        si (!_congruit(syn, ARBOR_LEXEMA_BRACE_CLAUSA))
        {
            fac
            {
                ArborNodus* enumator = _parsere_enumerator(syn);
                si (enumator != NIHIL)
                {
                    ArborNodus** slot = xar_addere(nodus->datum.enum_spec.enumeratores);
                    si (slot != NIHIL) { *slot = enumator; enumator->pater = nodus; }
                }
            } dum (_consumere(syn, ARBOR_LEXEMA_COMMA) &&
                   !_congruit(syn, ARBOR_LEXEMA_BRACE_CLAUSA));
        }
        _expectare(syn, ARBOR_LEXEMA_BRACE_CLAUSA, "Expectabatur }");
    }
    alioquin
    {
        nodus->datum.enum_spec.enumeratores = NIHIL;
    }

    _finire_nodum(syn, nodus);
    redde nodus;
}

/* Parse type specifier - dispatch for struct/union/enum */
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

    /* Dispatch for struct/union/enum */
    commutatio (lex->genus)
    {
        casus ARBOR_LEXEMA_STRUCT:
            redde _parsere_struct_specifier(syn, FALSUM);

        casus ARBOR_LEXEMA_UNION:
            redde _parsere_struct_specifier(syn, VERUM);

        casus ARBOR_LEXEMA_ENUM:
            redde _parsere_enum_specifier(syn);

        ordinarius:
            /* Simple type specifier (int, char, etc.) */
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
    b32 habet_typedef_nomen;

    habet_typedef_nomen = FALSUM;

    dum (!_est_finis(syn))
    {
        lex = _currens_lex(syn);
        si (lex == NIHIL)
        {
            frange;
        }

        spec = NIHIL;

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
        alioquin si (_est_extension_keyword(syn))
        {
            /* Extensio: __attribute__, __extension__, etc. */
            spec = _parsere_extension(syn);
        }
        alioquin si (lex->genus == ARBOR_LEXEMA_IDENTIFICATOR)
        {
            chorda* titulus = chorda_internare(syn->intern, lex->valor);

            /* Verifica keyword macros (hic_manens -> static, constans -> const, etc.) */
            si (syn->keyword_macros != NIHIL)
            {
                vacuum* keyword_val = NIHIL;
                b32 found = tabula_dispersa_invenire(syn->keyword_macros, *titulus, &keyword_val);
                si (found)
                {
                    i32 keyword_type = *(i32*)keyword_val;

                    /* Storage class specifiers */
                    si (keyword_type == ARBOR_LEXEMA_STATIC ||
                        keyword_type == ARBOR_LEXEMA_EXTERN ||
                        keyword_type == ARBOR_LEXEMA_AUTO ||
                        keyword_type == ARBOR_LEXEMA_REGISTER ||
                        keyword_type == ARBOR_LEXEMA_TYPEDEF)
                    {
                        /* Creare STORAGE_CLASS node cum originale valor */
                        spec = _creare_nodum(syn, ARBOR_NODUS_STORAGE_CLASS);
                        si (spec != NIHIL)
                        {
                            spec->datum.folium.keyword = (ArborLexemaGenus)keyword_type;
                            spec->datum.folium.valor = titulus;  /* Keep original text */
                            _progredi(syn);
                            _finire_nodum(syn, spec);
                        }
                    }
                    /* Type qualifiers */
                    alioquin si (keyword_type == ARBOR_LEXEMA_CONST ||
                                 keyword_type == ARBOR_LEXEMA_VOLATILE)
                    {
                        spec = _creare_nodum(syn, ARBOR_NODUS_TYPE_QUALIFIER);
                        si (spec != NIHIL)
                        {
                            spec->datum.folium.keyword = (ArborLexemaGenus)keyword_type;
                            spec->datum.folium.valor = titulus;
                            _progredi(syn);
                            _finire_nodum(syn, spec);
                        }
                    }
                    /* Type specifiers */
                    alioquin si (keyword_type == ARBOR_LEXEMA_VOID ||
                                 keyword_type == ARBOR_LEXEMA_CHAR ||
                                 keyword_type == ARBOR_LEXEMA_SHORT ||
                                 keyword_type == ARBOR_LEXEMA_INT ||
                                 keyword_type == ARBOR_LEXEMA_LONG ||
                                 keyword_type == ARBOR_LEXEMA_FLOAT ||
                                 keyword_type == ARBOR_LEXEMA_DOUBLE ||
                                 keyword_type == ARBOR_LEXEMA_SIGNED ||
                                 keyword_type == ARBOR_LEXEMA_UNSIGNED)
                    {
                        spec = _creare_nodum(syn, ARBOR_NODUS_TYPE_SPECIFIER);
                        si (spec != NIHIL)
                        {
                            spec->datum.folium.keyword = (ArborLexemaGenus)keyword_type;
                            spec->datum.folium.valor = titulus;
                            _progredi(syn);
                            _finire_nodum(syn, spec);
                        }
                    }
                    /* Typedef macro (i8, i32, etc.) - treat as typedef name */
                    alioquin si (keyword_type == ARBOR_LEXEMA_TYPEDEF_MACRO && !habet_typedef_nomen)
                    {
                        spec = _creare_nodum(syn, ARBOR_NODUS_TYPEDEF_NAME);
                        si (spec != NIHIL)
                        {
                            spec->datum.folium.valor = titulus;
                            _progredi(syn);
                            _finire_nodum(syn, spec);
                        }
                        habet_typedef_nomen = VERUM;
                    }
                }
            }

            /* Si non keyword macro, verifica typedef nomen */
            si (spec == NIHIL && !habet_typedef_nomen)
            {
                si (_est_typedef_nomen(syn, titulus) || _est_probable_typedef_nomen(syn))
                {
                    /* Registratum vel probabiliter typedef nomen per heuristic */
                    spec = _creare_nodum(syn, ARBOR_NODUS_TYPEDEF_NAME);
                    si (spec != NIHIL)
                    {
                        spec->datum.folium.valor = titulus;
                        _progredi(syn);
                        _finire_nodum(syn, spec);
                    }
                    habet_typedef_nomen = VERUM;
                }
            }

            si (spec == NIHIL)
            {
                frange;
            }
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

/* ==================================================
 * Parsere Type-Name (pro cast et sizeof)
 * ================================================== */

interior ArborNodus*
_parsere_type_name(ArborSyntaxis* syn)
{
    ArborNodus* nodus;
    Xar*        specifiers;
    ArborLexema* lex;

    /* Usamus PARAMETER_DECLARATION quod habet specifiers + declarator */
    nodus = _creare_nodum(syn, ARBOR_NODUS_PARAMETER_DECLARATION);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    /* Parsere specifiers (const, int, unsigned, etc.) */
    specifiers = xar_creare(syn->piscina, magnitudo(ArborNodus*));
    _parsere_specifiers(syn, specifiers);
    nodus->datum.parametrum.specifiers = specifiers;

    /* Evitare trivia duplicata - primus specifier iam habet trivia */
    si (xar_numerus(specifiers) > ZEPHYRUM)
    {
        ArborNodus** first_spec = xar_obtinere(specifiers, ZEPHYRUM);
        si (first_spec != NIHIL && *first_spec != NIHIL &&
            (*first_spec)->trivia_ante == nodus->trivia_ante)
        {
            (*first_spec)->trivia_ante = NIHIL;
        }
    }

    /* Parsere abstract declarator si praesens (* vel () etc.) */
    lex = _currens_lex(syn);
    si (lex != NIHIL &&
        (lex->genus == ARBOR_LEXEMA_ASTERISCUS ||
         lex->genus == ARBOR_LEXEMA_PAREN_APERTA))
    {
        nodus->datum.parametrum.declarator = _parsere_declarator(syn);
        si (nodus->datum.parametrum.declarator != NIHIL)
        {
            nodus->datum.parametrum.declarator->pater = nodus;
        }
    }
    alioquin
    {
        nodus->datum.parametrum.declarator = NIHIL;
    }

    /* NON vocare _finire_nodum - liberi habent trivia */
    redde nodus;
}

interior ArborNodus*
_parsere_declarator(ArborSyntaxis* syn)
{
    ArborNodus* nodus;
    ArborNodus* inner;
    ArborNodus* prev_param;
    ArborLexema* lex;
    ArborLexema* comma_lex;
    Xar* trivia_post_comma;
    Xar* old_ante;

    prev_param = NIHIL;
    trivia_post_comma = NIHIL;

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

            /* Evitare trivia duplicata: nodus et inner habent idem trivia_ante */
            si (nodus->trivia_ante == inner->trivia_ante && inner->trivia_ante != NIHIL)
            {
                nodus->trivia_ante = NIHIL;
            }

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
        ArborLexema* open_paren_lex;
        Xar* trivia_post_open_paren;
        ArborLexema* close_paren_lex;
        b32 first_param;

        func = _creare_nodum(syn, ARBOR_NODUS_FUNCTION_DECLARATOR);
        si (func == NIHIL)
        {
            redde NIHIL;
        }
        func->datum.genericum.liberi = xar_creare(syn->piscina, magnitudo(ArborNodus*));
        _praeponere_punctuationem(syn, func, "(");

        /* Capere trivia_post de '(' ante progredi */
        open_paren_lex = _currens_lex(syn);
        trivia_post_open_paren = (open_paren_lex != NIHIL) ? open_paren_lex->trivia_post : NIHIL;

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
                    /* Just void - create parameter with void specifier for fidelis */
                    ArborNodus* void_param;
                    ArborNodus* void_spec;
                    Xar*        void_specs;
                    ArborLexema* void_lex;
                    ArborNodus** slot;

                    void_param = _creare_nodum(syn, ARBOR_NODUS_PARAMETER_DECLARATION);
                    si (void_param == NIHIL) { redde NIHIL; }

                    void_specs = xar_creare(syn->piscina, magnitudo(ArborNodus*));
                    void_spec = _creare_nodum(syn, ARBOR_NODUS_TYPE_SPECIFIER);
                    si (void_spec == NIHIL) { redde NIHIL; }
                    void_spec->datum.folium.keyword = ARBOR_LEXEMA_VOID;

                    /* Capture trivia from void token */
                    void_lex = _currens_lex(syn);
                    si (void_lex != NIHIL)
                    {
                        _copiare_trivia_ad_xar(syn, &void_spec->trivia_ante, void_lex->trivia_ante);
                        _copiare_trivia_ad_xar(syn, &void_spec->trivia_post, void_lex->trivia_post);
                    }

                    slot = xar_addere(void_specs);
                    si (slot != NIHIL) { *slot = void_spec; }

                    void_param->datum.parametrum.specifiers = void_specs;
                    void_param->datum.parametrum.declarator = NIHIL;

                    slot = xar_addere(func->datum.genericum.liberi);
                    si (slot != NIHIL) { *slot = void_param; }

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
                trivia_post_comma = NIHIL;
                first_param = VERUM;
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

                    /* Clear duplicate trivia from first specifier */
                    {
                        ArborNodus** fs = xar_obtinere(param_specs, ZEPHYRUM);
                        si (fs != NIHIL && *fs != NIHIL && (*fs)->trivia_ante == param->trivia_ante)
                        {
                            (*fs)->trivia_ante = NIHIL;
                        }
                    }

                    /* Praepone trivia_post de '(' ad primum param, vel trivia_post de comma */
                    si (first_param && trivia_post_open_paren != NIHIL)
                    {
                        ArborNodus** first_spec = xar_obtinere(param_specs, ZEPHYRUM);
                        si (first_spec != NIHIL && *first_spec != NIHIL)
                        {
                            old_ante = (*first_spec)->trivia_ante;
                            (*first_spec)->trivia_ante = NIHIL;
                            _copiare_trivia_ad_xar(syn, &(*first_spec)->trivia_ante, trivia_post_open_paren);
                            _copiare_trivia_ad_xar(syn, &(*first_spec)->trivia_ante, old_ante);
                        }
                        first_param = FALSUM;
                    }
                    alioquin si (trivia_post_comma != NIHIL)
                    {
                        ArborNodus** first_spec = xar_obtinere(param_specs, ZEPHYRUM);
                        si (first_spec != NIHIL && *first_spec != NIHIL)
                        {
                            old_ante = (*first_spec)->trivia_ante;
                            (*first_spec)->trivia_ante = NIHIL;
                            _copiare_trivia_ad_xar(syn, &(*first_spec)->trivia_ante, trivia_post_comma);
                            _copiare_trivia_ad_xar(syn, &(*first_spec)->trivia_ante, old_ante);
                        }
                    }

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

                    /* NON vocare _finire_nodum - declarator iam habet trivia_post */
                    param->pater = func;
                    {
                        ArborNodus** slot = xar_addere(func->datum.genericum.liberi);
                        si (slot != NIHIL) { *slot = param; }
                    }
                    prev_param = param;

                    /* Capere comma trivia si praesens */
                    si (_congruit(syn, ARBOR_LEXEMA_COMMA))
                    {
                        comma_lex = _currens_lex(syn);
                        si (comma_lex != NIHIL && prev_param != NIHIL)
                        {
                            _copiare_trivia_ad_xar(syn, &prev_param->trivia_post, comma_lex->trivia_ante);
                            _addere_synth_trivia(syn, &prev_param->trivia_post, ",");
                            trivia_post_comma = comma_lex->trivia_post;
                        }
                        _progredi(syn);
                        first_param = FALSUM;
                    }
                    alioquin
                    {
                        frange;
                    }
                } dum (VERUM);
            }
        }

        /* Capere ')' trivia_ante et attachere ad ultimum param */
        close_paren_lex = _currens_lex(syn);
        si (close_paren_lex != NIHIL && prev_param != NIHIL)
        {
            _copiare_trivia_ad_xar(syn, &prev_param->trivia_post, close_paren_lex->trivia_ante);
        }

        _expectare(syn, ARBOR_LEXEMA_PAREN_CLAUSA, "Expectabatur )");
        /* Capere trivia_post de ')' */
        _finire_nodum(syn, func);
        _attachere_punctuationem(syn, func, ")");
        {
            ArborNodus** slot = xar_addere(nodus->datum.genericum.liberi);
            si (slot != NIHIL) { *slot = func; }
        }
    }

    /* NON vocare _finire_nodum pro DECLARATOR - liberi iam habent trivia.
     * Si vocaremus, trivia_post duplicaretur (ex ultimo child ET ex hoc nodo). */
    redde nodus;
}

interior ArborNodus*
_parsere_initializer(ArborSyntaxis* syn)
{
    ArborNodus*  nodus;
    ArborLexema* open_lex;
    ArborLexema* close_lex;
    ArborLexema* comma_lex;
    ArborNodus*  prev_elem;

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

        /* Capere { trivia */
        open_lex = _currens_lex(syn);
        /* trivia_ante of { is captured by _creare_nodum into nodus->trivia_ante */
        /* Capere trivia_post of { + synthetic "{" ad primum elementum */
        _addere_synth_trivia(syn, &nodus->datum.init_list.trivia_post_open, "{");
        si (open_lex != NIHIL)
        {
            _copiare_trivia_ad_xar(syn, &nodus->datum.init_list.trivia_post_open,
                                   open_lex->trivia_post);
        }

        _progredi(syn);  /* Skip { */

        prev_elem = NIHIL;
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

                /* Si comma sequitur, capere trivia */
                si (_congruit(syn, ARBOR_LEXEMA_COMMA))
                {
                    comma_lex = _currens_lex(syn);
                    /* trivia_ante + "," ad elem.trivia_post */
                    si (comma_lex != NIHIL)
                    {
                        _copiare_trivia_ad_xar(syn, &elem->trivia_post, comma_lex->trivia_ante);
                    }
                    _addere_synth_trivia(syn, &elem->trivia_post, ",");
                    si (comma_lex != NIHIL)
                    {
                        _copiare_trivia_ad_xar(syn, &elem->trivia_post, comma_lex->trivia_post);
                    }
                    _progredi(syn);  /* Skip comma */

                    si (_congruit(syn, ARBOR_LEXEMA_BRACE_CLAUSA))
                    {
                        /* Trailing comma before } */
                        prev_elem = elem;
                        frange;
                    }
                }
                alioquin
                {
                    prev_elem = elem;
                    frange;
                }
            } dum (VERUM);
        }

        /* Capere } trivia */
        close_lex = _currens_lex(syn);
        si (close_lex != NIHIL)
        {
            si (prev_elem != NIHIL)
            {
                /* trivia_ante of } ad ultimum elementum */
                _copiare_trivia_ad_xar(syn, &prev_elem->trivia_post, close_lex->trivia_ante);
            }
            alioquin
            {
                /* Empty list {} - trivia_ante of } post trivia_post_open */
                _copiare_trivia_ad_xar(syn, &nodus->datum.init_list.trivia_post_open,
                                       close_lex->trivia_ante);
            }
            /* "}" + trivia_post ad nodus.trivia_post */
            _addere_synth_trivia(syn, &nodus->trivia_post, "}");
            _copiare_trivia_ad_xar(syn, &nodus->trivia_post, close_lex->trivia_post);
        }

        _progredi(syn);  /* Skip } */
        /* NON vocare _finire_nodum - trivia iam captata */
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
    ArborNodus* prev_decl;
    ArborLexema* comma_lex;
    Xar* specifiers;
    Xar* trivia_post_comma;
    Xar* old_ante;
    b32 habet_typedef;
    i32 i;
    i32 n;

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

    /* Evitare trivia duplicata - primus specifier iam habet trivia_ante.
     * Si non clearimus, emittitur bis: semel de declaration, semel de specifier. */
    si (xar_numerus(specifiers) > ZEPHYRUM)
    {
        nodus->trivia_ante = NIHIL;
    }

    /* Verifica si typedef est in specifiers */
    habet_typedef = FALSUM;
    n = xar_numerus(specifiers);
    per (i = ZEPHYRUM; i < n; i++)
    {
        ArborNodus** spec_ptr = xar_obtinere(specifiers, i);
        si (spec_ptr != NIHIL && *spec_ptr != NIHIL)
        {
            ArborNodus* spec = *spec_ptr;
            si (spec->genus == ARBOR_NODUS_STORAGE_CLASS &&
                spec->datum.folium.keyword == ARBOR_LEXEMA_TYPEDEF)
            {
                habet_typedef = VERUM;
                frange;
            }
        }
    }

    /* Parse declarators */
    prev_decl = NIHIL;
    trivia_post_comma = NIHIL;
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

            /* Praepone trivia_post de comma praecedenti */
            si (trivia_post_comma != NIHIL)
            {
                old_ante = decl->trivia_ante;
                decl->trivia_ante = NIHIL;
                _copiare_trivia_ad_xar(syn, &decl->trivia_ante, trivia_post_comma);
                _copiare_trivia_ad_xar(syn, &decl->trivia_ante, old_ante);
            }

            init_decl->datum.init_decl.declarator = decl;
            decl->pater = init_decl;

            /* Evitare trivia duplicata: init_decl et decl habent idem trivia_ante */
            si (init_decl->trivia_ante == decl->trivia_ante && decl->trivia_ante != NIHIL)
            {
                init_decl->trivia_ante = NIHIL;
            }

            /* Optional initializer */
            si (_congruit(syn, ARBOR_LEXEMA_ASSIGNATIO))
            {
                ArborLexema*  eq_lex;
                Xar*          trivia_post_eq;

                /* Capere trivia ex '=' ante consumere */
                eq_lex = _currens_lex(syn);
                trivia_post_eq = (eq_lex != NIHIL) ? eq_lex->trivia_post : NIHIL;

                /* Consumere '=' et attachere trivia_ante + "=" ad declarator */
                _consumere_cum_trivia(syn, ARBOR_LEXEMA_ASSIGNATIO, decl, NIHIL);

                init = _parsere_initializer(syn);
                si (init == NIHIL)
                {
                    redde NIHIL;
                }

                /* Attachere trivia_post ex '=' ad initializer.trivia_ante */
                _copiare_trivia_ad_xar(syn, &init->trivia_ante, trivia_post_eq);

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
            prev_decl = init_decl;

            /* Capere comma trivia si praesens */
            si (_congruit(syn, ARBOR_LEXEMA_COMMA))
            {
                comma_lex = _currens_lex(syn);
                si (comma_lex != NIHIL && prev_decl != NIHIL)
                {
                    _copiare_trivia_ad_xar(syn, &prev_decl->trivia_post, comma_lex->trivia_ante);
                    _addere_synth_trivia(syn, &prev_decl->trivia_post, ",");
                    trivia_post_comma = comma_lex->trivia_post;
                }
                _progredi(syn);
            }
            alioquin
            {
                frange;
            }
        } dum (VERUM);
    }

    _expectare(syn, ARBOR_LEXEMA_SEMICOLON, "Expectabatur ;");

    /* Registrare typedef nomina */
    si (habet_typedef)
    {
        n = xar_numerus(nodus->datum.declaratio.declaratores);
        per (i = ZEPHYRUM; i < n; i++)
        {
            ArborNodus** decl_ptr = xar_obtinere(nodus->datum.declaratio.declaratores, i);
            si (decl_ptr != NIHIL && *decl_ptr != NIHIL)
            {
                chorda* titulus = _extrahere_declarator_nomen(*decl_ptr);
                si (titulus != NIHIL)
                {
                    _registrare_typedef(syn, titulus);
                }
            }
        }
    }

    _finire_nodum(syn, nodus);
    _attachere_punctuationem(syn, nodus, ";");
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

    /* Clear duplicate trivia - trivia before '{' belongs to compound stmt, not func def */
    si (corpus->trivia_ante == nodus->trivia_ante)
    {
        nodus->trivia_ante = NIHIL;
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
        _attachere_punctuationem(syn, decl, ";");
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
        si (_congruit(syn, ARBOR_LEXEMA_ASSIGNATIO))
        {
            ArborLexema* eq_lex;
            Xar*         trivia_post_eq;

            /* Capere trivia ex '=' ante consumere */
            eq_lex = _currens_lex(syn);
            trivia_post_eq = (eq_lex != NIHIL) ? eq_lex->trivia_post : NIHIL;

            _consumere_cum_trivia(syn, ARBOR_LEXEMA_ASSIGNATIO, declarator, NIHIL);

            init = _parsere_initializer(syn);
            si (init != NIHIL)
            {
                _copiare_trivia_ad_xar(syn, &init->trivia_ante, trivia_post_eq);
            }
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
        {
            ArborNodus* prev_decl;
            ArborLexema* comma_lex;
            Xar* trivia_post_comma;
            Xar* old_ante;

            prev_decl = init_decl;
            trivia_post_comma = NIHIL;

            dum (_congruit(syn, ARBOR_LEXEMA_COMMA))
            {
                /* Capere comma trivia ante consumere */
                comma_lex = _currens_lex(syn);
                si (comma_lex != NIHIL && prev_decl != NIHIL)
                {
                    _copiare_trivia_ad_xar(syn, &prev_decl->trivia_post, comma_lex->trivia_ante);
                    _addere_synth_trivia(syn, &prev_decl->trivia_post, ",");
                    trivia_post_comma = comma_lex->trivia_post;
                }
                _progredi(syn);

                declarator = _parsere_declarator(syn);
                si (declarator == NIHIL)
                {
                    redde NIHIL;
                }

                /* Apply trivia_post_comma to declarator's trivia_ante */
                si (trivia_post_comma != NIHIL)
                {
                    old_ante = declarator->trivia_ante;
                    declarator->trivia_ante = NIHIL;
                    _copiare_trivia_ad_xar(syn, &declarator->trivia_ante, trivia_post_comma);
                    _copiare_trivia_ad_xar(syn, &declarator->trivia_ante, old_ante);
                }

                init_decl = _creare_nodum(syn, ARBOR_NODUS_INIT_DECLARATOR);
                si (init_decl == NIHIL)
                {
                    redde NIHIL;
                }
                init_decl->datum.init_decl.declarator = declarator;
                declarator->pater = init_decl;

                si (_congruit(syn, ARBOR_LEXEMA_ASSIGNATIO))
                {
                    ArborLexema* eq_lex;
                    Xar*         trivia_post_eq;

                    eq_lex = _currens_lex(syn);
                    trivia_post_eq = (eq_lex != NIHIL) ? eq_lex->trivia_post : NIHIL;

                    _consumere_cum_trivia(syn, ARBOR_LEXEMA_ASSIGNATIO, declarator, NIHIL);

                    init = _parsere_initializer(syn);
                    si (init != NIHIL)
                    {
                        _copiare_trivia_ad_xar(syn, &init->trivia_ante, trivia_post_eq);
                    }
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

                prev_decl = init_decl;
                trivia_post_comma = NIHIL;
            }
        }

        _expectare(syn, ARBOR_LEXEMA_SEMICOLON, "Expectabatur ;");

        /* Registrare typedef nomina */
        {
            b32 habet_typedef;
            i32 i;
            i32 n;

            habet_typedef = FALSUM;
            n = xar_numerus(specifiers);
            per (i = ZEPHYRUM; i < n; i++)
            {
                ArborNodus** spec_ptr = xar_obtinere(specifiers, i);
                si (spec_ptr != NIHIL && *spec_ptr != NIHIL)
                {
                    ArborNodus* spec = *spec_ptr;
                    si (spec->genus == ARBOR_NODUS_STORAGE_CLASS &&
                        spec->datum.folium.keyword == ARBOR_LEXEMA_TYPEDEF)
                    {
                        habet_typedef = VERUM;
                        frange;
                    }
                }
            }

            si (habet_typedef)
            {
                n = xar_numerus(nodus->datum.declaratio.declaratores);
                per (i = ZEPHYRUM; i < n; i++)
                {
                    ArborNodus** decl_ptr = xar_obtinere(nodus->datum.declaratio.declaratores, i);
                    si (decl_ptr != NIHIL && *decl_ptr != NIHIL)
                    {
                        chorda* titulus = _extrahere_declarator_nomen(*decl_ptr);
                        si (titulus != NIHIL)
                        {
                            _registrare_typedef(syn, titulus);
                        }
                    }
                }
            }
        }

        _finire_nodum(syn, nodus);
        _attachere_punctuationem(syn, nodus, ";");
        result = nodus;
    }

    redde result;
}

/* ==================================================
 * Directive Parsing (PRESERVARE mode)
 * ================================================== */

/* Verificare si currens token initiat directivam (# ad initium lineae) */
interior b32
_est_directiva_initium(ArborSyntaxis* syn)
{
    ArborLexema* lex;
    ArborLexema* lex_praev;
    ArborLexemaOrigo** ptr;

    lex = _currens_lex(syn);
    si (lex == NIHIL || lex->genus != ARBOR_LEXEMA_HASH)
    {
        redde FALSUM;
    }

    /* Verificare si ad initium lineae */
    si (syn->positus == ZEPHYRUM)
    {
        /* Primum token - est ad initium */
        redde VERUM;
    }

    /* Obtinere praevium lexema */
    lex_praev = NIHIL;
    ptr = xar_obtinere(syn->lexemata, syn->positus - I);
    si (ptr != NIHIL && *ptr != NIHIL)
    {
        lex_praev = (*ptr)->lexema;
    }

    si (lex_praev == NIHIL)
    {
        redde VERUM;
    }

    /* Si in linea diversa a praevio, ad initium novae lineae */
    si (lex->linea > lex_praev->linea)
    {
        redde VERUM;
    }

    redde FALSUM;
}

/* Identificare genus directivae ex token */
interior ArborDirectivaGenus
_identificare_directiva_genus(ArborLexema* lex)
{
    chorda valor;

    si (lex == NIHIL || lex->genus != ARBOR_LEXEMA_IDENTIFICATOR)
    {
        redde ARBOR_DIRECTIVA_UNKNOWN;
    }

    valor = lex->valor;

    si (chorda_aequalis_literis(valor, "include"))
    {
        redde ARBOR_DIRECTIVA_INCLUDE;
    }
    si (chorda_aequalis_literis(valor, "define"))
    {
        redde ARBOR_DIRECTIVA_DEFINE;
    }
    si (chorda_aequalis_literis(valor, "undef"))
    {
        redde ARBOR_DIRECTIVA_UNDEF;
    }
    si (chorda_aequalis_literis(valor, "if"))
    {
        redde ARBOR_DIRECTIVA_IF;
    }
    si (chorda_aequalis_literis(valor, "ifdef"))
    {
        redde ARBOR_DIRECTIVA_IFDEF;
    }
    si (chorda_aequalis_literis(valor, "ifndef"))
    {
        redde ARBOR_DIRECTIVA_IFNDEF;
    }
    si (chorda_aequalis_literis(valor, "elif"))
    {
        redde ARBOR_DIRECTIVA_ELIF;
    }
    si (chorda_aequalis_literis(valor, "else"))
    {
        redde ARBOR_DIRECTIVA_ELSE;
    }
    si (chorda_aequalis_literis(valor, "endif"))
    {
        redde ARBOR_DIRECTIVA_ENDIF;
    }
    si (chorda_aequalis_literis(valor, "line"))
    {
        redde ARBOR_DIRECTIVA_LINE;
    }
    si (chorda_aequalis_literis(valor, "error"))
    {
        redde ARBOR_DIRECTIVA_ERROR;
    }
    si (chorda_aequalis_literis(valor, "pragma"))
    {
        redde ARBOR_DIRECTIVA_PRAGMA;
    }

    redde ARBOR_DIRECTIVA_UNKNOWN;
}

/* Parsere directiva: colligere omnia lexemata usque ad finem lineae */
interior ArborNodus*
_parsere_directiva(ArborSyntaxis* syn)
{
    ArborNodus* nodus;
    ArborLexema* lex;
    ArborLexema* lex_directiva;
    i32 linea_directivae;
    Xar* lexemata_directivae;
    ArborLexema** slot;

    nodus = _creare_nodum(syn, ARBOR_NODUS_DIRECTIVE);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    lexemata_directivae = xar_creare(syn->piscina, magnitudo(ArborLexema*));
    si (lexemata_directivae == NIHIL)
    {
        redde NIHIL;
    }

    nodus->datum.directiva.lexemata = lexemata_directivae;
    nodus->datum.directiva.genus = ARBOR_DIRECTIVA_UNKNOWN;

    /* Addere # ad lexemata */
    lex = _currens_lex(syn);
    linea_directivae = lex->linea;
    slot = xar_addere(lexemata_directivae);
    si (slot != NIHIL)
    {
        *slot = lex;
    }
    _progredi(syn);

    /* Obtinere genus directivae ex proximo token */
    lex_directiva = _currens_lex(syn);
    si (lex_directiva != NIHIL && lex_directiva->linea == linea_directivae)
    {
        nodus->datum.directiva.genus = _identificare_directiva_genus(lex_directiva);
    }

    /* Colligere omnia lexemata in eadem linea */
    dum (!_est_finis(syn))
    {
        lex = _currens_lex(syn);
        si (lex == NIHIL)
        {
            frange;
        }

        /* Si in linea diversa, finire directivam */
        si (lex->linea > linea_directivae)
        {
            frange;
        }

        /* Addere lexema ad collectio */
        slot = xar_addere(lexemata_directivae);
        si (slot != NIHIL)
        {
            *slot = lex;
        }
        _progredi(syn);
    }

    /* NON vocare _finire_nodum - trivia iam inclusa in lexemata directivae */
    /* Tantum ponere locum finis */
    {
        ArborLexemaOrigo** ptr;
        ArborLexemaOrigo*  lo;
        ArborLexema*       lex_finis;

        lo = NIHIL;
        si (syn->positus > ZEPHYRUM)
        {
            ptr = xar_obtinere(syn->lexemata, syn->positus - I);
            si (ptr != NIHIL)
            {
                lo = *ptr;
            }
        }

        si (lo != NIHIL && lo->lexema != NIHIL)
        {
            lex_finis = lo->lexema;
            nodus->byte_finis = lex_finis->byte_offset + lex_finis->longitudo;
            nodus->linea_finis = lex_finis->linea;
            nodus->columna_finis = lex_finis->columna + lex_finis->longitudo;
            /* NON ponere trivia_post - evitare duplicationem */
        }
    }
    redde nodus;
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

        /* Verificare si directiva (PRESERVARE mode) */
        si (_est_directiva_initium(syn))
        {
            decl = _parsere_directiva(syn);
        }
        alioquin
        {
            decl = _parsere_external_declaration(syn);
        }

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

    /* NON vocare _finire_nodum pro translation_unit -
     * evitare duplicationem trivia si ultimum child est directiva.
     * Tantum capere trivia ex EOF. */
    si (_est_finis(syn))
    {
        ArborLexemaOrigo** ptr;
        ArborLexemaOrigo*  lo;

        ptr = xar_obtinere(syn->lexemata, syn->positus);
        si (ptr != NIHIL && *ptr != NIHIL)
        {
            lo = *ptr;
            si (lo->lexema != NIHIL)
            {
                /* Ponere locum finis */
                unit->byte_finis = lo->lexema->byte_offset;
                unit->linea_finis = lo->lexema->linea;
                unit->columna_finis = lo->lexema->columna;

                /* Capere EOF trivia_ante tantum - non ex ultimo token */
                si (lo->lexema->trivia_ante != NIHIL)
                {
                    _copiare_trivia_ad_xar(syn, &unit->trivia_post, lo->lexema->trivia_ante);
                }
            }
        }
    }

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
    syn->typedef_nomina = tabula_dispersa_creare_chorda(piscina, CXXVIII);
    syn->keyword_macros = NIHIL;  /* Set via arbor_syntaxis_ponere_keyword_macros */

    redde syn;
}

vacuum
arbor_syntaxis_ponere_keyword_macros(
    ArborSyntaxis*    syn,
    TabulaDispersa*   keyword_macros)
{
    si (syn != NIHIL)
    {
        syn->keyword_macros = keyword_macros;
    }
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
